#pragma once

#include <stdint.h>
#include "hw.h"

typedef struct {
  int id;
  uint16_t max;
  uint8_t* data;

  void* next;
} perif_mem_t;

int perif_mem_init(hw_sysbus_t* sysb, uint16_t addr, uint8_t addr_lines);

uint16_t perif_mem_get_max(int id);
void perif_mem_cleanup();
void perif_mem_memcpy(int id, void* src, size_t len, size_t off);

