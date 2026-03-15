#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "dbg.h"
#include "sysbus.h"
#include "utils.h"

#define MAX_CMD_SZ 0x80

uint8_t stop = 1;

void dbg_get_input(char* buf, int s) {

  set_green();
  printf("rca1802_dbg> ");
  reset_color();
  fflush(stdout);

  fgets(buf, s, stdin);

}

void dbg_cmd_dispatch(cdp1802a_chip_t* c, char* cmd) {
  char* space = strchr(cmd, ' ');

  if (space)
    *space = '\0';

  // temporary, have LL of commands
  if (!strcmp(cmd, "readb") && space) {
    dbg_read_mem_printb(c, (uint16_t) strtol(space + 1, NULL, 0), 0x80);
  }
}

void dbg_ask(cdp1802a_chip_t* c) {

  // TODO: code to check for breakpoint
  
  if (!stop)
    return;

  char cmd[MAX_CMD_SZ];
  memset(cmd, '\0', MAX_CMD_SZ);

  while (strlen(cmd) == 0)
    dbg_get_input(cmd, MAX_CMD_SZ);

  dbg_cmd_dispatch(c, cmd);
}

