#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "hw.h"
#include "cpu.h"
#include "sys_build.h"
#include "sysbus.h"
#include "perif.h"
#include "dbg.h"
#include "utils.h"
#include "yaml_parser.h"

#define ADDR_DECODE_PINS 3U

void toggle_clock(cdp1802a_chip_t* chip) {

  sysbus_handle_state(chip->sysbus);
  cpu_handle_state(chip);
  
}

void hw_pin_bar_init(cdp1802a_chip_t* c) {

  set_bit(&c->pinout, PIN_WAIT);
  set_bit(&c->pinout, PIN_CLEAR);
  set_bit(&c->pinout, PIN_MRD);
  set_bit(&c->pinout, PIN_MWR);
  set_bit(&c->pinout, PIN_DMAIN);
  set_bit(&c->pinout, PIN_DMAOUT);
  set_bit(&c->pinout, PIN_XTAL);
  set_bit(&c->pinout, PIN_INT);
  set_bit(&c->pinout, PIN_EF1);
  set_bit(&c->pinout, PIN_EF2);
  set_bit(&c->pinout, PIN_EF3);
  set_bit(&c->pinout, PIN_EF4);

}

void hw_sysbus_start(cdp1802a_chip_t* c, int decode_pins) {

  c->sysbus = create_sysbus_obj(c, decode_pins);

}

void hw_cpu_init(cdp1802a_chip_t* chip, int decode_pins) {

  chip->state.mc = RDY;
  hw_pin_bar_init(chip);
  hw_sysbus_start(chip, decode_pins);
  chip->next = NULL;
  chip->bps = NULL;
  chip->po_log = malloc(sizeof(dbg_po_t));

  memset(chip->po_log, '\0', sizeof(dbg_po_t));

}

void small_system(char* conf, hw_sys_t* sys) {

  // temporary, hardcoded cpu
  // TODO: YAML parsing
  cdp1802a_chip_t* c = malloc(sizeof(cdp1802a_chip_t));
  hw_cpu_init(c, ADDR_DECODE_PINS);

  c->name = "lone_cpu";
  sys->head = c;

  // temporary, hardcoded memory
  int id = perif_mem_init(c->sysbus, 0, 13);
  mem_info_t* tmp = malloc(sizeof(mem_info_t));

  tmp->id = id;
  tmp->addr = 0;

  sys_build_parse_file(c, conf, tmp, 1);

  free(tmp);
}

void manual_system(char* conf, hw_sys_t* sys) {
  master_t* master = parse_yaml(conf);

  sys_build(sys, master);

  free_master_yaml(master);
}

// based on YAML given, initialize system
void hw_sys_init(char* conf, hw_sys_t* sys, int yaml) {
  if (yaml) {
    manual_system(conf, sys);
  } else {
    small_system(conf, sys);
  }
}

void hw_sys_cleanup(hw_sys_t* sys) {

  cdp1802a_chip_t* c = sys->head;
  cdp1802a_chip_t* tmp;

  while (c) {
    perif_mem_cleanup();
    sysbus_cleanup(c->sysbus);
    free(c->sysbus);

    tmp = c->next;
    free(c);
    c = tmp;
  }

  sys->head = NULL;
}

void hw_sys_run(hw_sys_t* sys) {

  while(1) {

    cdp1802a_chip_t* chip = sys->head;

    if (chip->state.mc == RDY || chip->idle) {

      dbg_state_enum ret = dbg_ask(chip);

      while (ret == STATE_DBG)
        ret = dbg_ask(chip);

      dbg_clr_po_log(chip);

      if (ret == STATE_QUIT)
        return;
    }

    while (chip != NULL) {

      toggle_clock(chip);

      if (chip == sys->head)
        dbg_log_po(chip);

      chip = chip->next;
    }

  }

}

