#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "dbg.h"
#include "sysbus.h"
#include "utils.h"

void print_timing_diagram(uint32_t pin_log, int bytes) {

    for (int i = 0; i < bytes*8; i++) {
        int bit     = (pin_log >> i) & 1;
        int prevbit = (i > 0) ? (pin_log >> (i - 1)) & 1 : bit;

        if (!prevbit && bit)              
          printf(" __");
        else if (bit)
          printf("___");
        else 
          printf("   ");

        if (i % 8 == 0 && i != 0) printf("|");
    }

    printf("\n");

    for (int i = 0; i < bytes*8; i++) {
        int bit     = (pin_log >> i) & 1;
        int prevbit = (i > 0) ? (pin_log >> (i - 1)) & 1 : bit;

        if (bit && !prevbit)       
          printf("/  ");   
        else if (!bit && prevbit)
          printf("\\__");
        else if (!bit)             
          printf("___");
        else                       
          printf("   ");

        if (i % 8 == 0 && i != 0) printf("|");
    }
    printf("\n");
}

void log_pin(uint32_t* dst, uint64_t po, int dbit, int sbit) {

  if (po & (1 << sbit))
    set_bit((uint64_t *) dst, dbit);
  else 
    clr_bit((uint64_t *) dst, dbit);
}

void dbg_log_po(cdp1802a_chip_t* c) {

  int bit_pos = ((dbg_po_t *) c->po_log)->bytes * 8 + c->state.t_state;

  log_pin(&((dbg_po_t *) c->po_log)->mrd, c->pinout, bit_pos, PIN_MRD);
  log_pin(&((dbg_po_t *) c->po_log)->mwr, c->pinout, bit_pos, PIN_MWR);
  log_pin(&((dbg_po_t *) c->po_log)->sc0, c->pinout, bit_pos, PIN_SC0);
  log_pin(&((dbg_po_t *) c->po_log)->sc1, c->pinout, bit_pos, PIN_SC1);
  log_pin(&((dbg_po_t *) c->po_log)->tpa, c->pinout, bit_pos, PIN_TPA);
  log_pin(&((dbg_po_t *) c->po_log)->tpb, c->pinout, bit_pos, PIN_TPB);
}

void dbg_clr_po_log(cdp1802a_chip_t* c) {
  memset(c->po_log, '\0', sizeof(dbg_po_t));
}

dbg_state_enum dbg_print_mc(cdp1802a_chip_t* c, char* args) {

  set_green();
  printf("CLK\n");
  set_white();
  print_timing_diagram(0xaaaaaa, ((dbg_po_t *) c->po_log)->bytes);

  set_green();
  printf("MRD\n");
  set_white();
  print_timing_diagram(((dbg_po_t *) c->po_log)->mrd, ((dbg_po_t *) c->po_log)->bytes);

  set_green();
  printf("MWR\n");
  set_white();
  print_timing_diagram(((dbg_po_t *) c->po_log)->mwr, ((dbg_po_t *) c->po_log)->bytes);

  set_green();
  printf("SC0\n");
  set_white();
  print_timing_diagram(((dbg_po_t *) c->po_log)->sc0, ((dbg_po_t *) c->po_log)->bytes);

  set_green();
  printf("SC1\n");
  set_white();
  print_timing_diagram(((dbg_po_t *) c->po_log)->sc1, ((dbg_po_t *) c->po_log)->bytes);

  set_green();
  printf("TPA\n");
  set_white();
  print_timing_diagram(((dbg_po_t *) c->po_log)->tpa, ((dbg_po_t *) c->po_log)->bytes);

  set_green();
  printf("TPB\n");
  set_white();
  print_timing_diagram(((dbg_po_t *) c->po_log)->tpb, ((dbg_po_t *) c->po_log)->bytes);

  putchar('\n');

  reset_color();

  return STATE_DBG;
}

