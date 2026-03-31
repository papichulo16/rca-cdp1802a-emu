#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "dbg.h"
#include "sysbus.h"
#include "utils.h"

hw_sysbus_t* create_fake_sysb(cdp1802a_chip_t* c) {

  hw_sysbus_t* fake_sysb = malloc(sizeof(hw_sysbus_t));
  memcpy((void *) fake_sysb, (void *) c->sysbus, sizeof(hw_sysbus_t));
  fake_sysb->data_bus = 0;

  return fake_sysb;
}

dbg_state_enum dbg_write_mem_setb(cdp1802a_chip_t* c, char* args) {
  hw_sysbus_t* fake_sysb = create_fake_sysb(c);
  char* space = strchr(args, ' ');

  if (!space) {

    set_red();
    printf("Usage: setb <addr> <byte>\n\n");
    reset_color();

    return STATE_DBG;
  }

  *space = '\0';

  uint16_t addr = strtol(args, NULL, 0); 
  uint16_t byte = strtol(space + 1, NULL, 16) & 0xff; 

  fake_sysb->ctrl_bus = SYS_MWR;
  fake_sysb->addr_bus = addr;
  fake_sysb->data_bus = byte;
  sysbus_chip_select(fake_sysb);

  free(fake_sysb);

  return STATE_DBG;
}

dbg_state_enum dbg_read_mem_printw(cdp1802a_chip_t* c, char* args) {
  uint16_t addr = strtol(args, NULL, 0); 
  uint16_t sz = 0x80;
  hw_sysbus_t* fake_sysb = create_fake_sysb(c);
  uint8_t tmp = 0;

  fake_sysb->ctrl_bus = SYS_MRD;

  for (int i = 0; i < sz; i += 2) {
    if (i % 8 == 0) {

      set_red();
      printf("0x%04x: ", addr + i);
      reset_color();
    }

    fake_sysb->addr_bus = addr + i;
    sysbus_chip_select(fake_sysb);
    tmp = fake_sysb->data_bus;

    fake_sysb->addr_bus = addr + i + 1;
    sysbus_chip_select(fake_sysb);
    print_word((uint16_t)(fake_sysb->data_bus << 8) + tmp);

    if (i % 8 == 6)
      putchar('\n');
    else
      putchar(' ');
  }

  putchar('\n');

  free(fake_sysb);

  return STATE_DBG;
}

dbg_state_enum dbg_read_mem_printb(cdp1802a_chip_t* c, char* args) {

  uint16_t addr = strtol(args, NULL, 0); 
  uint16_t sz = 0x80;
  hw_sysbus_t* fake_sysb = create_fake_sysb(c);

  fake_sysb->ctrl_bus = SYS_MRD;

  for (int i = 0; i < sz; i++) {
    if (i % 8 == 0) {

      set_red();
      printf("0x%04x: ", addr + i);
      reset_color();
    }

    fake_sysb->addr_bus = addr + i;

    sysbus_chip_select(fake_sysb);
    print_byte(fake_sysb->data_bus);

    if (i % 8 == 7)
      putchar('\n');
    else
      putchar(' ');
  }

  putchar('\n');

  free(fake_sysb);

  return STATE_DBG;
}

