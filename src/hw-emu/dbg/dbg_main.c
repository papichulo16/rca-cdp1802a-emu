#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "dbg.h"
#include "sysbus.h"
#include "utils.h"

#define MAX_CMD_SZ 0x80

uint8_t stop = 1;

dbg_cmd_t* cmds_head = NULL;

dbg_state_enum dbg_quit(cdp1802a_chip_t* c, char* args) { return STATE_QUIT; }

dbg_state_enum dbg_step(cdp1802a_chip_t* c, char* args) { return STATE_CONT; }

dbg_state_enum dbg_cont(cdp1802a_chip_t* c, char* args) {

  stop = 0;

  return STATE_CONT;
}

dbg_state_enum dbg_clear(cdp1802a_chip_t* c, char* args) {

  clear_screen();

  return STATE_DBG;
}

dbg_state_enum dbg_help(cdp1802a_chip_t* c, char* args) {
  set_red();

  printf("List of cmds: ");

  dbg_cmd_t* cur = cmds_head;

  while (cur) {
    printf("%s, ", cur->cmd);
    cur = cur->next;
  }

  printf("\b\b \b\n");

  reset_color();

  return STATE_DBG;
}

dbg_state_enum dbg_bp(cdp1802a_chip_t* c, char* args) {

  if (!args) {

    set_red();
    printf("Usage: bp <set/dis> <addr>\n");
    reset_color();

    goto end;
  }

  char* space = strchr(args, ' ');

  if (!space) {

    set_red();
    printf("Usage: bp <set/dis> <addr>\n");
    reset_color();

    goto end;
  }

  *space++ = '\0';

  uint16_t addr = strtol(space, NULL, 16); 
  dbg_bp_t* d;
  
  if (!strcmp(args, "set")) {
    d = malloc(sizeof(dbg_bp_t));

    d->addr = addr;
    d->next = c->bps;
    c->bps = d;

    goto end;
  }
 
  if (!strcmp(args, "dis")) {
    d = c->bps;

    if (((dbg_bp_t *) c->bps)->addr == addr) {
      c->bps = ((dbg_bp_t *) c->bps)->next;

      free(d);
      goto end;
    }

    dbg_bp_t* cur = ((dbg_bp_t *) c->bps)->next;

    while (cur) {
      if (cur->addr == addr) {

        d->next = cur->next;
        free(cur);

        goto end;
      }

      d = cur;
      cur = cur->next;
    }

    goto end;
  }

  set_red();
  printf("Usage: bp <set/dis> <addr>\n");
  reset_color();

end:
  return STATE_DBG; 
}

void dbg_add_cmd(char* cmd, void* entry) {

  dbg_cmd_t* obj = malloc(sizeof(dbg_cmd_t));

  obj->cmd = cmd;
  obj->entry = entry;
  obj->next = cmds_head;

  cmds_head = obj;
}

void handle_sigint(int sig) {

  if (stop)
    return;

  const char *msg = "\nCaught Ctrl+C. Breaking...\n";
  write(STDOUT_FILENO, msg, strlen(msg));

  stop = 1;
}

void dbg_cmds_init() {

  dbg_add_cmd("help", dbg_help);
  dbg_add_cmd("quit", dbg_quit);
  dbg_add_cmd("bp", dbg_bp);
  dbg_add_cmd("step", dbg_step);
  dbg_add_cmd("cont", dbg_cont);
  dbg_add_cmd("clear", dbg_clear);

  dbg_add_cmd("setb", dbg_write_mem_setb);
  dbg_add_cmd("readb", dbg_read_mem_printb);
  dbg_add_cmd("readw", dbg_read_mem_printw);
  dbg_add_cmd("readi", dbg_read_mem_instr);

  dbg_add_cmd("cpu", dbg_cpu);
  dbg_add_cmd("tim_diag", dbg_print_mc);

  struct sigaction sa;
  sa.sa_handler = handle_sigint;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART; 

  sigaction(SIGINT, &sa, NULL);

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

  *(char *) strchr(cmd, '\n') = '\0';

  if (space) {

    *space++ = '\0';
  }

  dbg_cmd_t* cur = cmds_head;

  while (cur) {

    if (!strcmp(cmd, cur->cmd)) {

      return ((dbg_entry)cur->entry) (c, space);
    }

    cur = cur->next;
  }

  set_red(); 
  printf("Unsupported command %s!\n", cmd);
  reset_color();

  return STATE_DBG;
}

uint16_t last_pc = 0xffff;

void print_cur_state(cdp1802a_chip_t* c, uint16_t pc) {

  char buf[16];

  snprintf(buf, sizeof(buf), "%x 5", pc);

  set_yellow();
  printf("IN STOPPED STATE, CUR CPU PC (R%d): ", c->regs.xp & 0xf);
  set_white();
  printf("0x%x\n\n", pc);
  reset_color();
  dbg_read_mem_instr(c, buf);

}

void check_breakpoints(cdp1802a_chip_t* c, uint16_t pc) {

  dbg_bp_t* cur = (dbg_bp_t *) c->bps;

  while (cur) {
    if (pc == cur->addr) {
      stop = 1;

      return;
    }

    cur = cur->next;
  }
}

dbg_state_enum dbg_ask(cdp1802a_chip_t* c) {

  // TODO: code to check for breakpoint
  uint16_t pc = c->regs.sp_r[c->regs.xp & 0xf];

  check_breakpoints(c, pc);
  
  if (!stop)
    return STATE_CONT;

  if (pc != last_pc) {

    print_cur_state(c, pc);
    last_pc = pc;
  }

  char cmd[MAX_CMD_SZ];
  memset(cmd, '\0', MAX_CMD_SZ);

  while (strlen(cmd) == 0)
    dbg_get_input(cmd, MAX_CMD_SZ);

  return dbg_cmd_dispatch(c, cmd);
}

