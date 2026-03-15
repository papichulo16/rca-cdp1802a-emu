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

void dbg_read_mem_printb(cdp1802a_chip_t* c, uint16_t addr, uint16_t sz) {

  hw_sysbus_t* fake_sysb = malloc(sizeof(hw_sysbus_t));
  memcpy(fake_sysb, c->sysbus, sizeof(hw_sysbus_t));
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
}

