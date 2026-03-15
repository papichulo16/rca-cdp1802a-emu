#pragma once

#include "hw.h"

void sysbus_handle_state(hw_sysbus_t* sysb);
hw_sysbus_t* create_sysbus_obj(cdp1802a_chip_t* c, uint8_t addr_decode_pins);
void sysbus_cleanup(hw_sysbus_t* s);

void sysbus_perif_init(hw_sysbus_t* sysb, uint16_t addr, perif_entry* entry, int id);

