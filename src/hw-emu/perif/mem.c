#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "perif.h"
#include "sysbus.h"
#include "utils.h"

perif_mem_t* mem_head = NULL;

void perif_mem_cleanup() {
  perif_mem_t* c = mem_head;
  perif_mem_t* tmp;

  while (c) {

    tmp = c->next;
    free(c);
    c = tmp;
  }
}

void perif_mem_remove(int id) {

  perif_mem_t* c = mem_head;
  perif_mem_t* prev = NULL;

  while (c) {

    if (c->id == id) {

      if (prev) 
        prev->next = c->next;
      else
        mem_head = c->next;
      
      free(c);
      return;
    }

    prev = c;
    c = c->next;
  }
}

perif_mem_t* perif_mem_find(int id) {

  perif_mem_t* c = mem_head;
  perif_mem_t* prev = NULL;

  while (c) {

    if (c->id == id) {

      // like a mini cache, make latest accesses faster to access
      if (prev) {

        prev->next = c->next;
        c->next = mem_head;
        mem_head = c;
      }

      return c;
    }

    prev = c;
    c = c->next;
  }

  return NULL;
}

int perif_mem_push(uint8_t addr_lines) {

  int id = 1;

  while(perif_mem_find(id))
    id++;

  perif_mem_t* mem = malloc(sizeof(perif_mem_t));
  int sz = power(2, addr_lines);

  mem->id = id;
  mem->max =  sz - 1;
  mem->data = malloc(sz);
  mem->next = mem_head;

  mem_head = mem;

  return id;
}

void perif_mem_entry(hw_sysbus_t* sysb, int id) {

  perif_mem_t* mem = perif_mem_find(id);

  if (!mem)
    return;

  uint16_t off = sysb->addr_bus & mem->max;


  switch (sysb->ctrl_bus) {
    case SYS_MRD:

      sysb->data_bus = mem->data[off];
      break;

    case SYS_MWR:

      mem->data[off] = sysb->data_bus;
      break;
  }
}

int perif_mem_init(hw_sysbus_t* sysb, uint16_t addr, uint8_t addr_lines) {

  if (addr_lines > 16) {
    printf("[mem.c] ERROR: too many address lines in memory device. 16 bit addressing max.\n");

    exit(-1);
  }

  int id = perif_mem_push(addr_lines);

  sysbus_perif_init(sysb, addr, (perif_entry *) &perif_mem_entry, id);

  return id;
}

void perif_mem_memcpy(int id, void* src, size_t len, size_t off) {

  perif_mem_t* mem = perif_mem_find(id);

  if (mem == NULL) {
    printf("[mem.c] ERROR: initial mem memcpy not found\n");

    exit(-1);
  }

  if (len + off > mem->max + 1) {
    printf("[mem.c] WARNING: target mem memcpy too large for perif\n");
    memcpy(mem->data, src, mem->max + 1);

    return;
  }

  memcpy(mem->data + off, src, len);
}

