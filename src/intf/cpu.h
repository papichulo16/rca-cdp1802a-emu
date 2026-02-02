#pragma once

#include "hw.h"

typedef void (*instr_handler_t)(cdp1802a_chip_t*);

void cpu_handle_state(cdp1802a_chip_t* c);
void cpu_opc_decode(cdp1802a_chip_t* c);
void cpu_execute(cdp1802a_chip_t* c);

