/*
 * sysbus design and reqs:
 *
 * a cpu
 * address decoder
 * address bus
 * data bus
 * control bus
 *
 * cpu -> addr bus -> addr decoder -> peripheral chip select
 * cpu -> control bus (MRD/MWR/IOR/IOW?) -> peripherals
 * cpu <-> data bus <-> peripherals
 *
 * */

#include <stdlib.h>

#include "sysbus.h"

int power(int a, int b) {

  int ret = 1;

  for (int i = 0; i < b; i++)
    ret *= a;

  return ret;
  
}

void sysbus_handle_data_bus(hw_sysbus_t* s, cdp1802a_chip_t* c) {

  if (!(c->pinout & (1ULL << PIN_MWR))) 
    s->data_bus = ((c->pinout & (0xff << PIN_BUS0)) >> PIN_BUS0);

}

void sysbus_handle_addr_bus(hw_sysbus_t* s, cdp1802a_chip_t* c) {

  if (!(c->pinout & (1ULL << PIN_TPA))) {
    s->addr_bus &= 0xff;
    s->addr_bus |= ((c->pinout & (0xff << PIN_MA0)) >> PIN_MA0) << 8;
  }

  if (!(c->pinout & (1ULL << PIN_TPB))) {
    s->addr_bus &= 0xff00;
    s->addr_bus |= ((c->pinout & (0xff << PIN_MA0)) >> PIN_MA0);
  }

}

void sysbus_handle_ctrl_bus(hw_sysbus_t* s, cdp1802a_chip_t* c) {

  if (!(c->pinout & (1ULL << PIN_MRD))) 
    s->ctrl_bus = SYS_MRD;

  if (!(c->pinout & (1ULL << PIN_MWR))) 
    s->ctrl_bus = SYS_MWR;

}

hw_sysbus_t* create_sysbus_obj(cdp1802a_chip_t* c, uint8_t addr_decode_pins) {

  hw_sysbus_t* s = malloc(sizeof(hw_sysbus_t));

  s->cpu = c;
  s->peripherals = malloc(sizeof(hw_peripheral_t*) * power(2, addr_decode_pins));
  s->decode_pin_count = addr_decode_pins;
  
  return s;
}

void sysbus_update(hw_sysbus_t* s) {
  cdp1802a_chip_t* c = (cdp1802a_chip_t*) s->cpu;

  sysbus_handle_data_bus(s, c);
  sysbus_handle_addr_bus(s, c);
  sysbus_handle_ctrl_bus(s, c);

}

void sysbus_chip_select(hw_sysbus_t* sysb) {

  int idx = (sysb->addr_bus & (power(2, sysb->decode_pin_count) << (16 - sysb->decode_pin_count))) >> (16 - sysb->decode_pin_count);

  ((perif_entry) (sysb->peripherals[idx])->entry)(sysb);

}

void sysbus_handle_state(hw_sysbus_t* sysb) {

  sysbus_update(sysb);
  sysbus_chip_select(sysb);

}

