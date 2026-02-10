#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "hw.h"
#include "cpu.h"

void cpu_handle_alu(cdp1802a_chip_t* c) {
  uint8_t temp = 0;
  uint8_t lsb, msb;

  switch (c->exec.alu_oper) {
    case ADD:
      temp = c->regs.d + c->regs.b;

      if (c->exec.carry)
        temp += c->regs.df;
         
      // OF
      if (temp < c->regs.d)
        c->regs.df = true;
      else
        c->regs.df = false;

      c->regs.d = temp;

      break;

    case SUB:
      if (c->exec.reversed) {
        temp = c->regs.b - c->regs.d;
        if (c->exec.carry)
          temp -= (c->regs.df ? 0 : 1); 
      } else {
        temp = c->regs.d - c->regs.b;
        if (c->exec.carry)
          temp -= (c->regs.df ? 0 : 1); 
      }
      
      if (c->exec.reversed) {
        c->regs.df = (c->regs.b >= (c->regs.d + (c->exec.carry ? (c->regs.df ? 0 : 1) : 0)));
      } else {
        c->regs.df = (c->regs.d >= (c->regs.b + (c->exec.carry ? (c->regs.df ? 0 : 1) : 0)));
      }
      
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

    case SHR:
      lsb = c->regs.d & 0x01;
      c->regs.d = c->regs.d >> 1;
      c->regs.df = lsb;

      break;
      
    case SHRC:
      lsb = c->regs.d & 0x01;
      c->regs.d = (c->regs.d >> 1) | (c->regs.df ? 0x80 : 0x00);
      c->regs.df = lsb;

      break;
      
    case SHL:
      msb = (c->regs.d & 0x80) >> 7;
      c->regs.d = c->regs.d << 1;
      c->regs.df = msb;

      break;
      
    case SHLC:
      msb = (c->regs.d & 0x80) >> 7;
      c->regs.d = (c->regs.d << 1) | (c->regs.df ? 0x01 : 0x00);
      c->regs.df = msb;

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

