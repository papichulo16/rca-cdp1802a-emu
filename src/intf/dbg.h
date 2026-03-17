#pragma once

#include <stdint.h>

#include "hw.h"

typedef enum {
  STATE_DBG,
  STATE_CONT,
  STATE_QUIT
} dbg_state_enum;

typedef dbg_state_enum (*dbg_entry)(cdp1802a_chip_t*, char*);

typedef struct {
  char* cmd;
  void* entry;

  void* next;
} dbg_cmd_t;


void dbg_cmds_init();
void dbg_cmds_clean();
dbg_state_enum dbg_ask(cdp1802a_chip_t* c);

dbg_state_enum dbg_write_mem_setb(cdp1802a_chip_t* c, char* args);
dbg_state_enum dbg_read_mem_printb(cdp1802a_chip_t* c, char* args);
dbg_state_enum dbg_read_mem_printw(cdp1802a_chip_t* c, char* args);

