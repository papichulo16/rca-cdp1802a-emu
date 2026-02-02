#include <stdio.h>
#include <stdint.h>

#include "hw.h"
#include "cpu.h"

void exec_mem_trans(cdp1802a_chip_t* c, void* dest, void* src) {
      c->exec.mem_trans = true;
      c->exec.trans_src = src;
      c->exec.trans_dest = dest;
}

void exec_alu_oper(cdp1802a_chip_t* c, alu_oper_t oper) {
      c->exec.alu = true;
      c->exec.alu_oper = oper;
}

void i_reg_0(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {
    // idl
    case 0:
      c->idle = true;

      break;

    // ldn r{1-f}
    default:
      exec_mem_trans(c, &c->regs.d, &c->data_bus);

      c->state.mc = MRD;
      c->addr = c->regs.sp_r[c->regs.in & 0xf];

      break;
  }

}

void i_reg_1(cdp1802a_chip_t* c) {

  // inc r{0-f}
  c->regs.sp_r[c->regs.in & 0xf]++;

}

void i_reg_2(cdp1802a_chip_t* c) {

  // dec r{0-f}
  c->regs.sp_r[c->regs.in & 0xf]--;

}

void i_reg_3(cdp1802a_chip_t* c) {

  // short branch

}

void i_reg_4(cdp1802a_chip_t* c) {

  // lda r{0-f}

}

void i_reg_5(cdp1802a_chip_t* c) {

  // str mrn??
  // store D at memory region R points to
  c->state.mc = MWR;
  c->addr = c->regs.sp_r[c->regs.in & 0xf];
  c->data_bus = c->regs.d;

}

void i_reg_6(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {
    // irx rx
    case 0:
      break;

    // out N
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      break;

    case 8:
      perror("instr_decode.c: undefined instruction\n");
      break;

    // in N - 8
    case 9:
    case 0xa:
    case 0xb:
    case 0xc:
    case 0xd:
    case 0xe:
    case 0xf:
      break;
  }

}

void i_reg_7(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {
    // ret
    case 0:
      break;

    // dis 
    case 1:
      break;

    // ldxa
    case 2:
      break;

    // stxd
    case 3:
      break;

    // adc
    case 4:
      break;

    // sdb 
    case 5:
      break;

    // shrc 
    case 6:
      break;

    // smb 
    case 7:
      break;

    // sav 
    case 8:
      break;

    // mark 
    case 9:
      break;

    // req 
    case 0xa:
      break;

    // seq
    case 0xb:
      break;

    // adci
    case 0xc:
      break;

    // sdbi 
    case 0xd:
      break;

    // shlc 
    case 0xe:
      break;

    // smbi 
    case 0xf:
      break;
  }

}

void i_reg_8(cdp1802a_chip_t* c) {

  // glo r{0-f}
  c->regs.d = (uint8_t) (c->regs.sp_r[c->regs.in & 0xf] & 0xff);

}

void i_reg_9(cdp1802a_chip_t* c) {

  // ghi r{0-f}
  c->regs.d = (uint8_t) (c->regs.sp_r[c->regs.in & 0xf] >> 8);

}

void i_reg_a(cdp1802a_chip_t* c) {

  // plo r{0-f} 
  c->regs.sp_r[c->regs.in & 0xf] = (c->regs.sp_r[c->regs.in & 0xf] & 0xff00) | ((uint16_t) c->regs.d);

}

void i_reg_b(cdp1802a_chip_t* c) {

  // phi r{0-f}
  c->regs.sp_r[c->regs.in & 0xf] = (c->regs.sp_r[c->regs.in & 0xf] & 0x00ff) | ((uint16_t) c->regs.d << 8);

}

void i_reg_c(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

    // nop
    case 4:
      break;

    // long branch
    case 0:
    case 1:
    case 2:
    case 3:
    case 8:
    case 9:
    case 0xa:
    case 0xb:
      break;

    // long skip
    case 5:
    case 6:
    case 7:
    case 0xc:
    case 0xd:
    case 0xe:
    case 0xf:
      break;

  }

}

void i_reg_d(cdp1802a_chip_t* c) {

  // sep 
  c->regs.xp = (c->regs.xp & 0xf0) | (c->regs.in & 0x0f);
  c->pinout = (c->pinout & ~(3 << PIN_SC0));
  c->state.mc = RDY;

}

void i_reg_e(cdp1802a_chip_t* c) {

  // sex
  c->regs.xp = (c->regs.xp & 0x0f) | ((c->regs.in & 0x0f) << 4);

}

void i_reg_f(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

    // ldx
    case 0:
      break;

    // or 
    case 1:
      break;

    // and
    case 2:
      break;

    // xor
    case 3:
      break;

    // add
    case 4:
      break;

    // sd 
    case 5:
      break;

    // shr 
    case 6:
      break;

    // sm 
    case 7:
      break;

    // ldi
    case 8:
      exec_mem_trans(c, &c->regs.d, &c->data_bus);

      c->state.mc = MRD;
      c->addr = c->regs.sp_r[c->regs.xp & 0xf];

      break;

    // ori
    case 9:
      exec_mem_trans(c, &c->regs.b, &c->data_bus);
      exec_alu_oper(c, OR);

      c->state.mc = MRD;
      c->addr = c->regs.sp_r[c->regs.xp & 0xf];

      break;

    // ani 
    case 0xa:
      exec_mem_trans(c, &c->regs.b, &c->data_bus);
      exec_alu_oper(c, AND);

      c->state.mc = MRD;
      c->addr = c->regs.sp_r[c->regs.xp & 0xf];

      break;

    // xri
    case 0xb:
      exec_mem_trans(c, &c->regs.b, &c->data_bus);
      exec_alu_oper(c, XOR);

      c->state.mc = MRD;
      c->addr = c->regs.sp_r[c->regs.xp & 0xf];

      break;

    // adi
    case 0xc:
      exec_mem_trans(c, &c->regs.b, &c->data_bus);
      exec_alu_oper(c, ADD);

      c->state.mc = MRD;
      c->addr = c->regs.sp_r[c->regs.xp & 0xf];

      break;

    // sdi 
    case 0xd:
      break;

    // shl 
    case 0xe:
      break;

    // smi 
    case 0xf:
      break;
  }

}

instr_handler_t instr_func_arr[16] = {
  &i_reg_0, &i_reg_1, &i_reg_2, &i_reg_3,
  &i_reg_4, &i_reg_5, &i_reg_6, &i_reg_7,
  &i_reg_8, &i_reg_9, &i_reg_a, &i_reg_b,
  &i_reg_c, &i_reg_d, &i_reg_e, &i_reg_f
};

void cpu_opc_decode(cdp1802a_chip_t* c) {

  // set status pins to exec mode
  // set the I and N regs
  c->pinout = (c->pinout & ~(3 << PIN_SC0) | (1 << PIN_SC0));
  c->regs.in = c->data_bus;

  uint8_t i_reg = (c->regs.in & 0xf0) >> 4; 

  (instr_func_arr[i_reg])(c);

}

