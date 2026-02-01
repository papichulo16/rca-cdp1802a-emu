#include <stdio.h>
#include <stdint.h>

#include "hw.h"
#include "cpu.h"

void i_reg_0(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

  }

}

void i_reg_1(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

  }

}

void i_reg_2(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

  }

}

void i_reg_3(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

  }

}

void i_reg_4(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

  }

}

void i_reg_5(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

  }

}

void i_reg_6(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

  }

}

void i_reg_7(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

  }

}

void i_reg_8(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

  }

}

void i_reg_9(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

  }

}

void i_reg_a(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

  }

}

void i_reg_b(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

  }

}

void i_reg_c(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

  }

}

void i_reg_d(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

  }

}

void i_reg_e(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

  }

}

void i_reg_f(cdp1802a_chip_t* c) {

  switch (c->regs.in & 0xf) {

  }

}

instr_handler_t instr_func_arr[16] = {
  &i_reg_0, &i_reg_1, &i_reg_2, &i_reg_3,
  &i_reg_4, &i_reg_5, &i_reg_6, &i_reg_7,
  &i_reg_8, &i_reg_9, &i_reg_a, &i_reg_b,
  &i_reg_c, &i_reg_d, &i_reg_e, &i_reg_f
};

void opc_decode(cdp1802a_chip_t* c) {

  // set SC to execute
  c->pinout = (c->pinout & ~(3 << PIN_SC0) | (1 << PIN_SC0));

  uint8_t i_reg = (c->regs.in & 0xf0) >> 4; 

  (instr_func_arr[i_reg])(c);

}

