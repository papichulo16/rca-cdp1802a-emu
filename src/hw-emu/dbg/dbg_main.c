#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "dbg.h"
#include "sysbus.h"
#include "utils.h"

#define MAX_CMD_SZ 0x80

uint8_t stop = 1;

dbg_cmd_t* cmds_head = NULL;

void dbg_add_cmd(char* cmd, void* entry) {

  dbg_cmd_t* obj = malloc(sizeof(dbg_cmd_t));

  obj->cmd = cmd;
  obj->entry = entry;
  obj->next = cmds_head;

  cmds_head = obj;
}

void dbg_cmds_init() {

  dbg_add_cmd("setb", dbg_write_mem_setb);
  dbg_add_cmd("readb", dbg_read_mem_printb);
  dbg_add_cmd("readw", dbg_read_mem_printw);
}

void dbg_cmds_clean() {

  dbg_cmd_t* c = cmds_head;
  dbg_cmd_t* tmp;

  while (c) {

    tmp = c->next;
    free(c);
    c = tmp;
  }

  cmds_head = NULL;
}

void dbg_get_input(char* buf, int s) {

  set_green();
  printf("rca1802_dbg> ");
  reset_color();
  fflush(stdout);

  fgets(buf, s, stdin);

}

dbg_state_enum dbg_cmd_dispatch(cdp1802a_chip_t* c, char* cmd) {
  char* space = strchr(cmd, ' ');

  if (space)
    *space = '\0';

  dbg_cmd_t* cur = cmds_head;

  while (cur) {

    if (!strcmp(cmd, cur->cmd)) {

      return ((dbg_entry)cur->entry) (c, space + 1);
    }

    cur = cur->next;
  }

  return STATE_DBG;
}

dbg_state_enum dbg_ask(cdp1802a_chip_t* c) {

  // TODO: code to check for breakpoint
  
  if (!stop)
    return STATE_CONT;

  char cmd[MAX_CMD_SZ];
  memset(cmd, '\0', MAX_CMD_SZ);

  while (strlen(cmd) == 0)
    dbg_get_input(cmd, MAX_CMD_SZ);

  return dbg_cmd_dispatch(c, cmd);
}

