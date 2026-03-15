#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "hw.h"
#include "cpu.h"
#include "sysbus.h"
#include "perif.h"
#include "utils.h"

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

void hw_sysbus_start(cdp1802a_chip_t* c) {

  c->sysbus = create_sysbus_obj(c, ADDR_DECODE_PINS);

}

void hw_cpu_init(cdp1802a_chip_t* chip) {

  hw_pin_bar_init(chip);
  hw_sysbus_start(chip);
  chip->next = NULL;

}

// based on YAML given, initialize system
void hw_sys_init(char* conf, hw_sys_t* sys) {

  // temporary, hardcoded cpu
  // TODO: YAML parsing
  cdp1802a_chip_t* c = malloc(sizeof(cdp1802a_chip_t));
  hw_cpu_init(c);

  sys->head = c;

  // temporary, hardcoded memory
  int id = perif_mem_init(c->sysbus, 0, 10);

  int fd = open(conf, O_RDONLY);
  struct stat st;
  uint8_t buf[0x1000];
  int sz = MIN(sizeof(buf), st.st_size);

  fstat(fd, &st);
  read(fd, buf, sz);
  close(fd);

  perif_mem_memcpy(id, buf, sz);

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

    while (chip != NULL) {
      toggle_clock(chip);

      chip = chip->next;
    }

    sleep(0.1);
  }

}

