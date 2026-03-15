#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef void(*perif_entry) (void*, int);

typedef enum {
    PIN_CLOCK,
    PIN_WAIT,
    PIN_CLEAR,

    PIN_Q,

    PIN_SC0,
    PIN_SC1,

    PIN_MRD,

    PIN_BUS0,
    PIN_BUS1,
    PIN_BUS2,
    PIN_BUS3,
    PIN_BUS4,
    PIN_BUS5,
    PIN_BUS6,
    PIN_BUS7,

    PIN_N2,
    PIN_N1,
    PIN_N0,

    PIN_EF1,
    PIN_EF2,
    PIN_EF3,
    PIN_EF4,

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

typedef enum {

  RDY,
  MRD,
  MWR,
  NON_MEM

} mach_cycle_t;

typedef enum {

  ADD,
  SUB,

  OR,
  AND,
  XOR,

  SHRC,
  SHLC,
  SHL,
  SHR

} alu_oper_t;


typedef enum {

  SYS_MRD,
  SYS_MWR,
  SYS_IOR,
  SYS_IOW

} ctrl_enum_t;

typedef struct {

  int id;
  perif_entry* entry;

} hw_peripheral_t;

typedef struct {

  uint16_t addr_bus;
  ctrl_enum_t ctrl_bus;
  uint8_t data_bus;
  uint8_t decode_pin_count;

  void* cpu;
  hw_peripheral_t** peripherals;

} hw_sysbus_t;

typedef struct {

  bool df;
  uint8_t d;
  uint8_t b;

  uint16_t sp_r[16]; // scratchpad registers

  uint8_t xp; // X is high nibble P is low nibble
  uint8_t in; // I is high nibble N is low nibble

  uint8_t t; // holds old XP pair on interrupt

  bool io;

} regs_t;

typedef struct {

  mach_cycle_t mc;
  uint8_t t_state;

} cpu_state_t;

typedef struct {

  bool mem_trans;
  bool mem_hi;
  void* trans_src;
  void* trans_dest;

  bool alu;
  bool carry;
  bool reversed;
  alu_oper_t alu_oper;

} cpu_exec_sm_t;

typedef struct {

  bool idle;
  uint64_t pinout;
  uint16_t addr;
  uint8_t data_bus;

  regs_t regs;

  cpu_state_t state;
  cpu_exec_sm_t exec;

  hw_sysbus_t* sysbus;
  
  void* next;
} cdp1802a_chip_t;

typedef struct {

  cdp1802a_chip_t* head;

} hw_sys_t;

void hw_sys_init(char* conf, hw_sys_t* sys);
void hw_sys_run(hw_sys_t* sys);
void hw_sys_cleanup(hw_sys_t* sys);

