#pragma once

#include <stdint.h>

#include "hw.h"

void dbg_ask(cdp1802a_chip_t* c);

void dbg_read_mem_printb(cdp1802a_chip_t* c, uint16_t addr, uint16_t sz);

