#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "hw.h"
#include "cpu.h"

void cpu_handle_alu(cdp1802a_chip_t* c) {
  uint8_t temp = 0;

  switch (c->exec.alu_oper) {
    case ADD:
      temp = c->regs.d + c->regs.b;
         
      // OF
      if (temp < c->regs.d)
        c->regs.df = true;
      else
        c->regs.df = false;

      c->regs.d = temp;

      break;

    case AND:
      c->regs.d &= c->regs.b;

      break;

    case OR:
      c->regs.d |= c->regs.b;

      break;

    case XOR:
      c->regs.d ^= c->regs.b;

      break;
  }
}

void _cpu_execute(cdp1802a_chip_t* c) {

  if (c->exec.mem_trans) 
    *((uint8_t *) c->exec.trans_dest + c->exec.mem_hi) = *(uint8_t *) c->exec.trans_src;

  if (!c->exec.alu)
    return;

  cpu_handle_alu(c);

}

void cpu_execute(cdp1802a_chip_t* c) {

  c->state.mc = RDY;
  _cpu_execute(c);

  memset(&c->exec, '\0', sizeof(cpu_exec_sm_t));

}

