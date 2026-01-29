#include <stdio.h>
#include <stdint.h>

#include "hw.h"
#include "cpu.h"

void cpu_w_bus_pins(uint8_t v){}
uint8_t cpu_r_bus_pins(){}

void cpu_w_ma_pins(cdp_1802a_chip_t* c, uint8_t v){}
uint8_t cpu_r_ma_pins(cdp_1802a_chip_t* c){}

void mem_read(cdp_1802a_chip_t* c){}
void mem_write(cdp_1802a_chip_t* c){}

void opc_fetch(cdp_1802a_chip_t* c){

  c->state.mc = MRD;
  c->state.t_state = 0;

  mem_read(c);

}

void opc_decode(cdp_1802a_chip_t* c){}

void cpu_handle_state(cdp_1802a_chip_t* c) {

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
      opc_decode(c);

      break;

    default:
      perror("cpu.c: cpu machine cycle state unknown\n");
      break;
  }

}


