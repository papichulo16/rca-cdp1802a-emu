#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "hw.h"
#include "cpu.h"
#include "sys_build.h"
#include "sysbus.h"
#include "perif.h"
#include "dbg.h"
#include "utils.h"
#include "yaml_parser.h"

struct file_metadata {
  uint16_t org;
  uint16_t off;
};

int mem_arr_get_id(cdp1802a_chip_t* c, struct file_metadata* md, mem_info_t* mem_arr, size_t n) {
  
  int tmp;
  int idx = sysbus_perif_idx_get(c->sysbus, md->org);

  for (int i = 0; i < n; i++) {

    tmp = sysbus_perif_idx_get(c->sysbus, mem_arr[i].addr);

    if (tmp != idx)
      continue;

    if (idx << (16 - c->sysbus->decode_pin_count) != mem_arr[i].addr) {
      printf("[sys_build.c] WARNING: initialized memory not aligned.\n");
    }

    if (idx << (16 - c->sysbus->decode_pin_count) != md->org) {
      printf("[sys_build.c] WARNING: ORG memory not aligned.\n");
    }

    return mem_arr[i].id;
  }

  return -1;
}

void sys_build_parse_file(cdp1802a_chip_t* c, char* f, mem_info_t* mem_arr, size_t n) {
  
  int count;
  int id;
  int sz;
  int off;
  long start;
  uint8_t* buf;
  struct stat st;
  struct file_metadata* arr;
  FILE* fd = fopen(f, "r");

  if (!fd) {
    
    printf("[sys_build.c] ERROR: Could not find file %s\n", f);
    exit(-1);
  }

  fstat(fileno(fd), &st);
  fread(&count, sizeof(count), 1, fd);

  if (count) {

    arr = malloc(count * sizeof(struct file_metadata));
    fread(arr, sizeof(struct file_metadata), count, fd);
  } 

  else {

    arr = malloc(sizeof(struct file_metadata)); 

    arr->org = 0;
    arr->off = 0;
    
    count = 1;
  }

  start = ftell(fd);

  for (int i = 0; i < count; i++) {

    id = mem_arr_get_id(c, &arr[i], mem_arr, n);

    if (id == -1)
      continue;
 
    if (i + 1 == count)
      sz = st.st_size - arr[i].off - start;      
    else 
      sz = arr[i+1].off - arr[i].off;

    buf = malloc(sz);

    fseek(fd, start + arr[i].off, SEEK_SET);
    fread(buf, sz, 1, fd);

    off = arr[i].org - (sysbus_perif_idx_get(c->sysbus, arr[i].org) << (16 - c->sysbus->decode_pin_count));
    
    perif_mem_memcpy(id, buf, sz, off);

    free(buf);
  }

  fclose(fd);
  free(arr);
}

slave_t* get_slave_w_name(slave_t* master, char* name) {
  
  slave_t* cur_slave = master->slaves;
  
  for (int i = 0; i < master->slaves_count; i++) {

    if (!strcmp(name, cur_slave->name))
      return cur_slave;

    cur_slave = cur_slave->next;
  }

  return NULL;
}

int get_count_w_name(slave_t* master, char* name) {
  
  slave_t* cur_slave = master->slaves;
  int count = 0;
  
  for (int i = 0; i < master->slaves_count; i++) {

    if (!strcmp(name, cur_slave->name))
      count++;

    cur_slave = cur_slave->next;
  }

  return count;
}

void build_mem_arr(cdp1802a_chip_t* c, slave_t* devices, mem_info_t* mem_arr) {

  slave_t* cur_slave = devices->slaves;
  int count = 0;
  
  for (int i = 0; i < devices->slaves_count; i++) {

    if (!strcmp("memory", cur_slave->name)) {

      int decoder_line = atoi(get_slave_w_name(cur_slave, "decoder_line")->value);
      int address_lines = atoi(get_slave_w_name(cur_slave, "address_lines")->value);

      if (address_lines + c->sysbus->decode_pin_count > 16) {

        printf("[sys_build.c] WARNING: CPU %s mix of decoder lines and address lines for memory device %d exceeds 16. Skipping.\n", c->name, count);

        cur_slave = cur_slave->next;
        continue;
      }

      mem_arr[count].addr = decoder_line << (16 - c->sysbus->decode_pin_count);
      mem_arr[count].id = perif_mem_init(c->sysbus, mem_arr[count].addr, address_lines);
      
      count++;
    }

    cur_slave = cur_slave->next;
  }
}

void build_cpu_obj(hw_sys_t* sys, slave_t* slave) {

  slave_t* decoder_lines;
  slave_t* sysbus_slave = get_slave_w_name(slave, "sysbus");
  
  if (!sysbus_slave) {

    printf("[sys_build.c] WARNING: CPU %s has no sysbus, ignoring CPU.\n", slave->value ? slave->value : "(NULL)");
    return;
  }

  slave_t* devices_slave = get_slave_w_name(sysbus_slave, "devices");

  if (!devices_slave) {

    printf("[sys_build.c] WARNING: CPU %s sysbus has no devices, ignoring CPU.\n", slave->value ? slave->value : "(NULL)");
    return;
  }

  int mem_count = get_count_w_name(devices_slave, "memory");

  if (!mem_count) {

    printf("[sys_build.c] WARNING: CPU %s sysbus has no memory, ignoring CPU.\n", slave->value ? slave->value : "(NULL)");
    return;
  }

  decoder_lines = get_slave_w_name(sysbus_slave, "decoder_line_count");

  mem_info_t* mem_arr = calloc(mem_count, sizeof(mem_info_t));
  cdp1802a_chip_t* c = calloc(1, sizeof(cdp1802a_chip_t));

  hw_cpu_init(c, decoder_lines ? atoi(decoder_lines->value) : 0);

  if (slave->value) {

    c->name = malloc(strlen(slave->value)+1);
    strcpy(c->name, slave->value);
  } else 
    c->name = "no_name";

  c->next = sys->head;
  sys->head = c;

  build_mem_arr(c, devices_slave, mem_arr);

  sys_build_parse_file(c, devices_slave->value, mem_arr, mem_count);

  free(mem_arr);
}

void dispatch_sys_device(hw_sys_t* sys, slave_t* slave) {

  if (slave->name == NULL) {

    puts("[sys_build.c] WARNING: System device not specified, ignoring");
    return;
  }
  
  if (!strcmp("cpu", slave->name)) {
    build_cpu_obj(sys, slave);
  }
}

void sys_build(hw_sys_t* sys, master_t* master) {

  slave_t* cur_slave = master->slaves;
  
  for (int i = 0; i < master->slaves_count; i++) {
    dispatch_sys_device(sys, cur_slave);
    cur_slave = cur_slave->next;
  }
}

