#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "dbg.h"
#include "sysbus.h"
#include "utils.h"
#include "instr_tree.h"

int read_instr_print(cdp1802a_chip_t* c, uint16_t addr) {

  hw_sysbus_t* fake_sysb = create_fake_sysb(c);

  fake_sysb->ctrl_bus = SYS_MRD;
  fake_sysb->addr_bus = addr;
  sysbus_chip_select(fake_sysb);

  uint8_t op = fake_sysb->data_bus;
  Instruction* instr = instr_tree_find_by_op(op);
  uint32_t operands = 0;

  set_red();
  printf("0x%04x:\t", addr);
  set_magenta();

  printf("%x ", op);

  if (!instr) {
    set_white();
    printf("\tUnknown instruction!!\n");
    reset_color();

    return 1;
  }

  for (int i = 1; i < instr->bytes; i++) {

    fake_sysb->ctrl_bus = SYS_MRD;
    fake_sysb->addr_bus = addr + i;
    sysbus_chip_select(fake_sysb);

    operands += fake_sysb->data_bus << ((i - 1) * 8);
    printf("%x ", fake_sysb->data_bus);

  }

  set_white();
  printf("\t%s ", instr->mnemonic);

  if (instr->bytes > 1) {
    printf("%XH", operands);
  }

  putchar('\n');
  reset_color();

  free(fake_sysb);

  return instr->bytes;
}

dbg_state_enum dbg_read_mem_instr(cdp1802a_chip_t* c, char* args) {

  if (!args) {

    set_red();
    printf("Usage: readi <addr> <num_i>\n");
    reset_color();

    goto end;
  }

  char* space = strchr(args, ' ');

  if (!space) {

    set_red();
    printf("Usage: readi <addr> <num_i>\n");
    reset_color();

    goto end;
  }

  *space++ = '\0';

  uint16_t addr = strtol(args, NULL, 16); 
  int cnt = strtol(space, NULL, 0); 

  for (int i = 0; i < cnt; i++) {
    addr += read_instr_print(c, addr);
  }

end:
  return STATE_DBG; 
}

