#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "cpu.h"
#include "perif.h"
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

dbg_state_enum dbg_cpuinfo(cdp1802a_chip_t* c, char* args) {

  hw_sysbus_t* sysb = c->sysbus;
  int arr_sz = power(2, sysb->decode_pin_count);

  set_magenta();
  printf("CPU label: ");
  set_white();
  printf("%s\n\n", c->name);

  set_magenta();
  printf("IDLE? ");
  set_white();
  printf("%s\n", c->idle ? "YES": "NO");

  set_magenta();
  printf("IRQs ENABLED? ");
  set_white();
  printf("%s\n", c->regs.io ? "YES": "NO");

  set_magenta();
  printf("\nSysbus info:\n");
  set_green();

  for (int i = 0; i < arr_sz; i++) {

    if (!sysb->peripherals[i])
      continue;

    uint16_t start = i << (16 - sysb->decode_pin_count);

    printf("MEMORY: ");
    set_white();
    printf("0x%04x-0x%04x\n", start, start + perif_mem_get_max(sysb->peripherals[i]->id));
    set_green();
  }

  putchar('\n');

  reset_color();

  return STATE_DBG;
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

void dbg_write_reg(cdp1802a_chip_t* c, char* args) {

  if (!args) {
    set_red();
    printf("Usage: cpu write <reg> <val>\n");
    reset_color();
    return;
  }

  char* space = strchr(args, ' ');

  if (!space) {
    set_red();
    printf("Usage: cpu write <reg> <val>\n");
    reset_color();
    return;
  }

  *space++ = '\0';

  unsigned long val = strtoul(space, NULL, 16);

  if (args[0] == 'R' || args[0] == 'r') {
    int idx = atoi(args + 1);

    if (idx < 0 || idx > 15) {

      set_red();
      printf("Invalid register: %s\n", args);
      reset_color();
      return;
    }

    c->regs.sp_r[idx] = (uint16_t)val;
    return;
  }

  if      (!strcasecmp(args, "X")) { c->regs.xp = (c->regs.xp & 0x0F) | ((val & 0xF) << 4); }
  else if (!strcasecmp(args, "P")) { c->regs.xp = (c->regs.xp & 0xF0) | (val & 0x7);        }
  else if (!strcasecmp(args, "I")) { c->regs.in = (c->regs.in & 0x0F) | ((val & 0xF) << 4); }
  else if (!strcasecmp(args, "N")) { c->regs.in = (c->regs.in & 0xF0) | (val & 0x7);        }
  else if (!strcasecmp(args, "D")) { c->regs.d = (uint8_t)val;  }
  else if (!strcasecmp(args, "B")) { c->regs.b = (uint8_t)val;  }
  else if (!strcasecmp(args, "T")) { c->regs.t = (uint8_t)val;  }
  else {
    set_red();
    printf("Unknown register: %s\n", args);
    reset_color();
  }
}

void dbg_read_reg(cdp1802a_chip_t* c, char* args) {

  if (!args) {
    set_red();
    printf("Usage: cpu read <reg>\n");
    reset_color();
    return;
  }

  char* end = args + strlen(args) - 1;
  while (end > args && (*end == '\n' || *end == '\r' || *end == ' '))
    *end-- = '\0';

  if (args[0] == 'R' || args[0] == 'r') {

    int idx = atoi(args + 1);

    if (idx < 0 || idx > 15) {

      set_red();
      printf("Invalid register: %s\n", args);
      reset_color();
      return;
    }

    print_regw(args, c->regs.sp_r[idx]);
    putchar('\n');
    return;
  }

  if      (!strcasecmp(args, "X")) { print_regb("X", c->regs.xp >> 4);   }
  else if (!strcasecmp(args, "P")) { print_regb("P", c->regs.xp & 0x7);  }
  else if (!strcasecmp(args, "I")) { print_regb("I", c->regs.in >> 4);   }
  else if (!strcasecmp(args, "N")) { print_regb("N", c->regs.in & 0x7);  }
  else if (!strcasecmp(args, "D")) { print_regb("D", c->regs.d);          }
  else if (!strcasecmp(args, "B")) { print_regb("B", c->regs.b);          }
  else if (!strcasecmp(args, "T")) { print_regb("T", c->regs.t);          }
  else {
    set_red();
    printf("Unknown register: %s\n", args);
    reset_color();
    return;
  }

  putchar('\n');
}

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

