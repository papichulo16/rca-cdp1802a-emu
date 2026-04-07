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

slave_t* get_slave_w_val(slave_t* master, char* name) {
  
  slave_t* cur_slave = master->slaves;
  
  for (int i = 0; i < master->slaves_count; i++) {

    if (!strcmp(name, cur_slave->name))
      return cur_slave;

    cur_slave = cur_slave->next;
  }

  return NULL;
}

void build_cpu_obj(hw_sys_t* sys, slave_t* slave) {

  slave_t* decoder_lines;
  slave_t* sysbus_slave = get_slave_w_val(slave, "sysbus");
  
  if (!sysbus_slave) {

    printf("[sys_build.c] WARNING: CPU %s has no sysbus, ignoring.\n", slave->value ? slave->value : "(NULL)");
    return;
  }

  decoder_lines = get_slave_w_val(sysbus_slave, "decoder_line_count");

  cdp1802a_chip_t* c = malloc(sizeof(cdp1802a_chip_t));
  hw_cpu_init(c, decoder_lines ? atoi(decoder_lines->value) : 0);

  c->name = slave->value;
  c->next = sys->head;
  sys->head = c;
}

void dispatch_sys_device(hw_sys_t* sys, slave_t* slave) {

  if (slave->name == NULL) {

    puts("[sys_build.c] WARNING: System device not specified, ignoring");
    return;
  }
  
  if (!strcmp("cpu", slave->name)) {
    build_cpu_obj(sys, slave);
  }
}

void sys_build(hw_sys_t* sys, master_t* master) {

  slave_t* cur_slave = master->slaves;
  
  for (int i = 0; i < master->slaves_count; i++) {
    dispatch_sys_device(sys, cur_slave);
    cur_slave = cur_slave->next;
  }
}

