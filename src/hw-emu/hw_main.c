#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "hw.h"
#include "cpu.h"
#include "sysbus.h"

#define ADDR_DECODE_PINS 3U

void set_bit(uint64_t* n, uint8_t k) {
    *n = (*n | (1 << k));
}

void clr_bit(uint64_t* n, uint8_t k) {
    *n = (*n & (~(1 << k)));
}

void toggle_clock(cdp1802a_chip_t* chip) {

  sysbus_handle_state(chip->sysbus);
  cpu_handle_state(chip);
  
}

void hw_pin_bar_init(cdp1802a_chip_t* c) {

  set_bit(&c->pinout, PIN_WAIT);
  set_bit(&c->pinout, PIN_CLEAR);
  set_bit(&c->pinout, PIN_MRD);
  set_bit(&c->pinout, PIN_MWR);
  set_bit(&c->pinout, PIN_DMAIN);
  set_bit(&c->pinout, PIN_DMAOUT);
  set_bit(&c->pinout, PIN_XTAL);
  set_bit(&c->pinout, PIN_INT);
  set_bit(&c->pinout, PIN_EF1);
  set_bit(&c->pinout, PIN_EF2);
  set_bit(&c->pinout, PIN_EF3);
  set_bit(&c->pinout, PIN_EF4);

}

void hw_sysbus_start(cdp1802a_chip_t* c) {

  c->sysbus = create_sysbus_obj(c, ADDR_DECODE_PINS);

}

void hw_cpu_init(cdp1802a_chip_t* chip) {

  hw_pin_bar_init(chip);
  hw_sysbus_start(chip);
  chip->next = NULL;

}

// based on YAML given, initialize system
void hw_sys_init(hw_sys_t* sys) {

  // temporary, hardcoded cpu
  // TODO: YAML parsing
  cdp1802a_chip_t* c = malloc(sizeof(cdp1802a_chip_t));
  hw_cpu_init(c);

  sys->head = c;
}

void hw_sys_cleanup(hw_sys_t* sys) {

  cdp1802a_chip_t* c = sys->head;
  cdp1802a_chip_t* tmp;

  while (c != NULL) {
    sysbus_cleanup(c->sysbus);
    free(c->sysbus);

    tmp = c->next;
    free(c);
    c = tmp;
  }

  sys->head = NULL;
}

void hw_sys_run(hw_sys_t* sys) {

  while(1) {

    cdp1802a_chip_t* chip = sys->head;

    while (chip != NULL) {
      toggle_clock(chip);

      chip = chip->next;
    }

    sleep(0.1);
  }

}

