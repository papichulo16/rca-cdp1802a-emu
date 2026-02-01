#include <stdio.h>
#include <stdint.h>

#include "hw.h"
#include "cpu.h"

void cpu_w_bus_pins(cdp1802a_chip_t* c, uint8_t v) {

  c->pinout = (c ->pinout & ~(0xff << PIN_BUS0)) | (v << PIN_BUS0);

}
uint8_t cpu_r_bus_pins(cdp1802a_chip_t* c) {
  return (uint8_t) (c->pinout & (0xff << PIN_BUS0));
}

void cpu_w_ma_pins(cdp1802a_chip_t* c, uint8_t v) {

  c->pinout = (c ->pinout & ~(0xff << PIN_MA0)) | (v << PIN_MA0);

}
uint8_t cpu_r_ma_pins(cdp1802a_chip_t* c) {
  return (uint8_t) (c->pinout & (0xff << PIN_MA0));
}

void handle_mc_end(cdp1802a_chip_t* c) {
  uint8_t status = (uint8_t) (c->pinout & (3 << PIN_SC0));

  c->state.t_state = 0;
  c->state.mc = NON_MEM;

  switch (status) {

    // OPF
    case 0:
      opc_decode(c);

      break;

    // EXEC
    case 1:
      break;

    // DMA
    case 2:
      break;

    // INT
    case 3:
      break;

  }
}

void mem_read(cdp1802a_chip_t* c) {

  switch (c->state.t_state) {

    case 0:
      break;

    case 1:
      cpu_w_ma_pins(c, (uint8_t) (c->addr & (0xff << 8)));
      clr_bit(&c->pinout, PIN_TPA);

      break;

    case 2:
      cpu_w_ma_pins(c, (uint8_t) (c->addr & 0xff));

      clr_bit(&c->pinout, PIN_TPA);
      clr_bit(&c->pinout, PIN_MRD);

      break;

    case 3:
      break;

    case 4:
      break;

    case 5:
      break;

    case 6:
      set_bit(&c->pinout, PIN_TPB);

      break;

    case 7:
      clr_bit(&c->pinout, PIN_TPB);
      set_bit(&c->pinout, PIN_MRD);

      c->data_bus = cpu_r_bus_pins(c);

      handle_mc_end(c);

      break;
  }

}
void mem_write(cdp1802a_chip_t* c) {

  switch (c->state.t_state) {

    case 0:
      break;

    case 1:
      cpu_w_ma_pins(c, (uint8_t) (c->addr & (0xff << 8)));
      set_bit(&c->pinout, PIN_TPA);

      break;

    case 2:
      cpu_w_ma_pins(c, (uint8_t) (c->addr & 0xff));
      clr_bit(&c->pinout, PIN_TPA);

      break;

    case 3:
      cpu_w_bus_pins(c, c->data_bus);

      break;

    case 4:
      clr_bit(&c->pinout, PIN_MWR);

      break;

    case 5:
      break;

    case 6:
      set_bit(&c->pinout, PIN_TPB);

      break;

    case 7:
      clr_bit(&c->pinout, PIN_TPB);
      set_bit(&c->pinout, PIN_MWR);

      handle_mc_end(c);

      break;
  }

}

void non_memory(cdp1802a_chip_t* c) {

  if (c->state.t_state < 7)
    return

  handle_mc_end(c);

}

void opc_fetch (cdp1802a_chip_t* c) {

  c->state.mc = MRD;
  c->state.t_state = 0;

  mem_read(c);

}


void cpu_handle_state(cdp1802a_chip_t* c) {

  switch (c->state.mc) {
    case RDY:
      opc_fetch(c);

      break;

    case MRD:
      mem_read(c);

      break;

    case MWR:
      mem_write(c);

      break;

    case NON_MEM:
      non_memory(c);

      break;

    default:
      perror("cpu.c: cpu machine cycle unknown\n");
      break;
  }

  c->state.t_state++;

}

