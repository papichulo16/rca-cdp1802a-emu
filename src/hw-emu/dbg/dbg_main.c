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
uint8_t switched = 0;

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

  printf("List of cmds: switch,");

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

void change_sigint() {

  struct sigaction sa;
  sa.sa_handler = handle_sigint;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART; 

  sigaction(SIGINT, &sa, NULL);
}

void dbg_cmds_init() {

  dbg_add_cmd("help", dbg_help);
  dbg_add_cmd("quit", dbg_quit);
  dbg_add_cmd("bp", dbg_bp);
  dbg_add_cmd("step", dbg_step);
  dbg_add_cmd("cont", dbg_cont);
  dbg_add_cmd("clear", dbg_clear);

  dbg_add_cmd("int", dbg_do_int);
  dbg_add_cmd("setb", dbg_write_mem_setb);
  dbg_add_cmd("readb", dbg_read_mem_printb);
  dbg_add_cmd("readw", dbg_read_mem_printw);
  dbg_add_cmd("readi", dbg_read_mem_instr);

  dbg_add_cmd("cpu", dbg_cpu);
  dbg_add_cmd("cpuinfo", dbg_cpuinfo);
  dbg_add_cmd("tim_diag", dbg_print_mc);

  change_sigint();
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

dbg_state_enum dbg_switch_cpus(hw_sys_t* sys, char* args) {

  if (!args) {

    set_red();
    printf("Usage: switch <cpu_label>\n");
    reset_color();

    return STATE_DBG;
  }

  cdp1802a_chip_t* prev = NULL;
  cdp1802a_chip_t* cur = sys->head;

  while (cur) {

    if (!strcmp(cur->name, args)) {

      if (!prev)
        return STATE_DBG;

      prev->next = cur->next;
      cur->next = sys->head;
      sys->head = cur;
      switched = 1;

      return STATE_CONT;
    }

    prev = cur;
    cur = cur->next;
  }

  set_red();
  printf("CPU \"%s\" not found\n", args);
  reset_color();

  return STATE_DBG;
}

dbg_state_enum dbg_cmd_dispatch(hw_sys_t* sys, char* cmd) {

  cdp1802a_chip_t* c = sys->head;
  char* space = strchr(cmd, ' ');

  *(char *) strchr(cmd, '\n') = '\0';

  if (space)
    *space++ = '\0';

  if (!strcmp("switch", cmd)) {

    stop = 1;
    return dbg_switch_cpus(sys, space);
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

void print_q_pin_state(cdp1802a_chip_t* c) {

  printf("\033[H\033[J"); // clear screen
  printf("Sysinfo CPUs:\n");

  cdp1802a_chip_t* cur = c;

  while (cur) {

    printf("%s: ", cur->name ? cur->name : "unk_cpu");  
    printf("LED %s\n", ((cur->pinout & (1UL << PIN_Q)) >> PIN_Q) ? "\033[32m● ON\033[0m" : "\033[31m● OFF\033[0m");

    cur = cur->next;
  }

  putchar('\n');
}

dbg_state_enum dbg_ask(hw_sys_t* sys) {

  cdp1802a_chip_t* c = sys->head;
  uint16_t pc = c->regs.sp_r[c->regs.xp & 0xf];

  check_breakpoints(c, pc);
  
  if (!stop)
    return STATE_CONT;

  if (pc != last_pc || switched) {

    print_q_pin_state(c);

    print_cur_state(c, pc);
    last_pc = pc;
    switched = 0;
  }

  char cmd[MAX_CMD_SZ];
  memset(cmd, '\0', MAX_CMD_SZ);

  while (strlen(cmd) == 0)
    dbg_get_input(cmd, MAX_CMD_SZ);

  return dbg_cmd_dispatch(sys, cmd);
}

