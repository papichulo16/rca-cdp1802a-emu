#pragma once

#include <stdint.h>
#include <stdbool.h>

#define CDP1802A_GET_PIN(po, p) (po & (1 << p))

typedef enum {
    PIN_CLOCK,
    PIN_WAIT,
    PIN_CLEAR,

    PIN_Q,

    PIN_SC0,
    PIN_SC1,

    PIN_MRD,

    PIN_BUS7,
    PIN_BUS6,
    PIN_BUS5,
    PIN_BUS4,
    PIN_BUS3,
    PIN_BUS2,
    PIN_BUS1,
    PIN_BUS0,

    PIN_N2,
    PIN_N1,
    PIN_N0,

    PIN_EF4,
    PIN_EF3,
    PIN_EF2,
    PIN_EF1,

    PIN_MA0,
    PIN_MA1,
    PIN_MA2,
    PIN_MA3,
    PIN_MA4,
    PIN_MA5,
    PIN_MA6,
    PIN_MA7,

    PIN_TPB,
    PIN_TPA,

    PIN_MWR,

    PIN_INT,

    PIN_DMAOUT,
    PIN_DMAIN,

    PIN_XTAL,

    PIN_COUNT
} cdp1802_pin_t;

typedef struct {
  uint16_t addr;
  uint8_t data;
  uint8_t ctrl;
} sysbus_t;

typedef struct {

  uint8_t d;
  bool df;
  uint8_t b;

  uint16_t scratchpad[16];

  uint8_t xp; // X is high nibble P is low nibble
  uint8_t in; // I is high nibble N is low nibble

  uint8_t t; // holds old XP pair on interrupt

  bool io;
  bool q;

} regs_t;

typedef struct {

  uint64_t pinout;
  regs_t regs;
  
} cdp1802a_chip_t;

void hw_oscillate_clock();

extern cdp1802a_chip_t chip;
