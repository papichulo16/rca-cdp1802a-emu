#pragma once

#include "hw.h"

void sysbus_handle_state(hw_sysbus_t* sysb);
hw_sysbus_t* create_sysbus_obj(cdp1802a_chip_t* c, uint8_t addr_decode_pins);
void sysbus_cleanup(hw_sysbus_t* s);

