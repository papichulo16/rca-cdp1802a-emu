#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "cpu.h"
#include "dbg.h"
#include "sysbus.h"
#include "utils.h"

void print_regb(char* reg, uint8_t val) {

  set_magenta();
  printf("%s:\t", reg);
  set_white();
  printf("0x%02x\t\t", val);
  reset_color();

}

void print_regw(char* reg, uint16_t val) {

  set_magenta();
  printf("%s:\t", reg);
  set_white();
  printf("0x%04x\t\t", val);
  reset_color();

}

dbg_state_enum dbg_do_int(cdp1802a_chip_t* c, char* args) {

  if (!args) {

    set_red();
    printf("Usage: int <1-4>\n");
    reset_color();

    return STATE_DBG;
  }

  int i = atoi(args);

  if (i < 1 || i > 4) {

    set_red();
    printf("Usage: int <1-4>\n");
    reset_color();

    return STATE_DBG;
  }

  cpu_fire_int(c, i);

  return STATE_DBG;
}

// TODO: implement inspecting specific registers
void dbg_write_reg(cdp1802a_chip_t* c, char* args) {}
void dbg_read_reg(cdp1802a_chip_t* c, char* args) {}

void dbg_read_all_regs(cdp1802a_chip_t* c) {

  for (int i = 0; i < 16; i++) {

    char buf[4];

    snprintf(buf, sizeof(buf), "R%d", i);
    print_regw(buf, c->regs.sp_r[i]);

    if (i % 2 == 1)
      putchar('\n');
  }

  print_regb("X", c->regs.xp >> 4);
  print_regb("P", c->regs.xp & 7);
  putchar('\n');
  print_regb("I", c->regs.in >> 4);
  print_regb("N", c->regs.in & 7);
  putchar('\n');

  print_regb("D", c->regs.d);
  print_regb("B", c->regs.b);
  putchar('\n');
  print_regb("T", c->regs.t);
  putchar('\n');

}

void _dbg_cpu(cdp1802a_chip_t* c, char* args) {

  if (!args) {

    dbg_read_all_regs(c);
    return; 
  }

  char* space = strchr(args, ' ');

  if (!space) {

    dbg_read_all_regs(c);
    return; 
  }

  *space++ = '\0';

  if (!strcmp("read", args)) {

    dbg_read_reg(c, space);
    return;
  }

  if (!strcmp("write", args)) {

    dbg_write_reg(c, space);
    return;
  }

  set_red();
  printf("Usage: cpu <read?/write?> <reg?> <val?>\n");
  reset_color();

}

dbg_state_enum dbg_cpu(cdp1802a_chip_t* c, char* args) {

  _dbg_cpu(c, args);

  return STATE_DBG;
}

