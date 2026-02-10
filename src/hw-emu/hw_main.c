#include <stdio.h>
#include <unistd.h>

#include "hw.h"
#include "cpu.h"

void set_bit(uint64_t* n, uint8_t k) {
    *n = (*n | (1 << k));
}

void clr_bit(uint64_t* n, uint8_t k) {
    *n = (*n & (~(1 << k)));
}

void toggle_clock(cdp1802a_chip_t* chip) {

  // sysbus_handle_event();
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

void hw_sys_init(cdp1802a_chip_t* chip) {

  hw_pin_bar_init(chip);

}

void hw_sys_run(cdp1802a_chip_t* chip) {

  while(1) {

    toggle_clock(chip);

    sleep(1);
  }

}

