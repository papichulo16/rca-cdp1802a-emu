#pragma once

#include <stdint.h>

#include "hw.h"
#include "yaml_parser.h"

typedef struct {
  int id;
  uint16_t addr;
} mem_info_t;

void sys_build(hw_sys_t* sys, master_t* master);
void sys_build_parse_file(cdp1802a_chip_t* c, char* f, mem_info_t* mem_arr, size_t n);

