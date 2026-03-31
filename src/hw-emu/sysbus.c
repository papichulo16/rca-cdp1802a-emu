#include <stdio.h>
#include <stdlib.h>

#include "sysbus.h"
#include "utils.h"

void sysbus_handle_data_bus(hw_sysbus_t* s, cdp1802a_chip_t* c) {

  if (!(c->pinout & (1ULL << PIN_MWR))) 
    s->data_bus = ((c->pinout & (0xff << PIN_BUS0)) >> PIN_BUS0);

}

void sysbus_handle_addr_bus(hw_sysbus_t* s, cdp1802a_chip_t* c) {

  if ((c->pinout & (1ULL << PIN_TPA))) {
    s->addr_bus &= 0xff;
    s->addr_bus |= ((c->pinout & (0xff << PIN_MA0)) >> PIN_MA0) << 8;
  }

  if ((c->pinout & (1ULL << PIN_TPB))) {
    s->addr_bus &= 0xff00;
    s->addr_bus |= ((c->pinout & (0xff << PIN_MA0)) >> PIN_MA0);
  }

}

void sysbus_handle_ctrl_bus(hw_sysbus_t* s, cdp1802a_chip_t* c) {

  s->ctrl_bus = SYS_NONE;

  if (!((c->pinout & (1ULL << PIN_MRD)) >> PIN_MRD)) {

    s->ctrl_bus = SYS_MRD;
  }

  if (!((c->pinout & (1ULL << PIN_MWR))>> PIN_MWR)) 
    s->ctrl_bus = SYS_MWR;

}

hw_sysbus_t* create_sysbus_obj(cdp1802a_chip_t* c, uint8_t addr_decode_pins) {

  hw_sysbus_t* s = malloc(sizeof(hw_sysbus_t));

  s->cpu = c;
  s->peripherals = malloc(sizeof(hw_peripheral_t*) * power(2, addr_decode_pins));
  s->decode_pin_count = addr_decode_pins;
  
  return s;
}

void sysbus_cleanup(hw_sysbus_t* s) {
  for (int i = 0; i < power(2, s->decode_pin_count); i++) {
    if (s->peripherals[i] == NULL)
      continue;

    free(s->peripherals[i]);
  }
}

void sysbus_update(hw_sysbus_t* s) {
  cdp1802a_chip_t* c = (cdp1802a_chip_t*) s->cpu;

  sysbus_handle_data_bus(s, c);
  sysbus_handle_addr_bus(s, c);
  sysbus_handle_ctrl_bus(s, c);

}

int sysbus_perif_idx_get(hw_sysbus_t* sysb, uint16_t addr) {

  return (addr & ((power(2, sysb->decode_pin_count) - 1) << (16 - sysb->decode_pin_count))) >> (16 - sysb->decode_pin_count);
}

int sysbus_cs_perif_idx_get(hw_sysbus_t* sysb) {
  return sysbus_perif_idx_get(sysb, sysb->addr_bus);
}

void sysbus_perif_init(hw_sysbus_t* sysb, uint16_t addr, perif_entry* entry, int id) {

  int idx = sysbus_perif_idx_get(sysb, addr);

  if (sysb->peripherals[idx] != NULL) {
    printf("[sysbus.c] ERROR: PERIPHERAL INITIALIZE CHIP SELECT LINE IS IN USE\n");

    exit(-1);
  }

  hw_peripheral_t* perif = malloc(sizeof(hw_peripheral_t));
  
  perif->entry = entry;
  perif->id = id;

  sysb->peripherals[idx] = perif;

}

void sysbus_chip_select(hw_sysbus_t* sysb) {

  int idx = sysbus_cs_perif_idx_get(sysb);   
  hw_peripheral_t* perif = sysb->peripherals[idx];

  if (perif == NULL)
    return;

  if (perif->entry == NULL) {
    printf("[sysbus.c] ERROR: PERIPHARAL IS INITIALIZED BUT NO ENTRY\n");

    exit(-1);
  }

  ((perif_entry) perif->entry)(sysb, perif->id);

}

void sysbus_update_chip_bus(hw_sysbus_t* sysb) {
  cdp1802a_chip_t* c = sysb->cpu;

  c->pinout &= ~(0xffULL << PIN_BUS0);
  c->pinout |= sysb->data_bus << PIN_BUS0;
}

int count = 0;

void sysbus_handle_state(hw_sysbus_t* sysb) {

  sysbus_update(sysb);
  sysbus_chip_select(sysb);
  sysbus_update_chip_bus(sysb);

  count++;
}

