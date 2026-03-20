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

typedef struct {
  uint16_t addr;
  
  void* next;
} dbg_bp_t;

typedef struct {
  int bytes;

  uint32_t mrd;
  uint32_t mwr;
  uint32_t sc0;
  uint32_t sc1;
  uint32_t tpa;
  uint32_t tpb;
} dbg_po_t;

void dbg_cmds_init();
void dbg_cmds_clean();
hw_sysbus_t* create_fake_sysb(cdp1802a_chip_t* c);
dbg_state_enum dbg_ask(cdp1802a_chip_t* c);

void dbg_log_po(cdp1802a_chip_t* c);
void dbg_clr_po_log(cdp1802a_chip_t* c);

dbg_state_enum dbg_cpu(cdp1802a_chip_t* c, char* args);
dbg_state_enum dbg_write_mem_setb(cdp1802a_chip_t* c, char* args);
dbg_state_enum dbg_read_mem_printb(cdp1802a_chip_t* c, char* args);
dbg_state_enum dbg_read_mem_printw(cdp1802a_chip_t* c, char* args);
dbg_state_enum dbg_read_mem_instr(cdp1802a_chip_t* c, char* args);
dbg_state_enum dbg_print_mc(cdp1802a_chip_t* c, char* args);

