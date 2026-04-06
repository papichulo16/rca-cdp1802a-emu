#pragma once

#ifndef INSTRTABLE_H_
#define INSTRTABLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


//What kind of token I might need next after a given instruction
typedef enum {
    OP_NONE,     // 1 byte total: No operands (e.g., IDL -> 00)
    OP_REG,      // 1 byte total: Register 0-F (e.g., GLO R1 -> 81)
    OP_REG_N0,   // 1 byte total: Register 1-F, 0 is invalid (e.g., LDN)
    OP_PORT_OUT, // 1 byte total: I/O Port 1-7 (e.g., OUT 1 -> 61)
    OP_PORT_INP, // 1 byte total: I/O Port 1-7 (e.g., INP 1 -> 69)
    OP_IMM,      // 2 bytes total: 8-bit immediate data (e.g., LDI 0xFF -> F8 FF)
    OP_ADDR8,    // 2 bytes total: 8-bit short branch address (e.g., BR 0x12 -> 30 12)
    OP_ADDR16    // 3 bytes total: 16-bit long branch address (e.g., LBR 0x1234 -> C0 12 34)
} OperandType;

typedef struct {
    const char *mnemonic;
    int8_t i;           // First nibble (High)
    int8_t n;           // Second nibble (Low). -1 if I need to grab something to complete it
    OperandType type;   // How the assembler should parse tokens and increment PC
} InstructionInfo;

//take this with a huge grain of salt
//Need to go back later and ensure correctness as it could be catastrophic
const InstructionInfo opcode_table[] = {
    {"ADC",   0x7, 0x4, OP_NONE},
    {"ADCI",  0x7, 0xC, OP_IMM},
    {"ADD",   0xF, 0x4, OP_NONE},
    {"ADI",   0xF, 0xC, OP_IMM},
    {"AND",   0xF, 0x2, OP_NONE},
    {"ANI",   0xF, 0xA, OP_IMM},
    {"B1",    0x3, 0x4, OP_ADDR8},  // Short branches are OP_ADDR8
    {"B2",    0x3, 0x5, OP_ADDR8},
    {"B3",    0x3, 0x6, OP_ADDR8},
    {"B4",    0x3, 0x7, OP_ADDR8},
    {"BDF",   0x3, 0x3, OP_ADDR8},
    {"BN1",   0x3, 0xC, OP_ADDR8},
    {"BN2",   0x3, 0xD, OP_ADDR8},
    {"BN3",   0x3, 0xE, OP_ADDR8},
    {"BN4",   0x3, 0xF, OP_ADDR8},
    {"BNF",   0x3, 0xB, OP_ADDR8},
    {"BNQ",   0x3, 0x9, OP_ADDR8},
    {"BNZ",   0x3, 0xA, OP_ADDR8},
    {"BQ",    0x3, 0x1, OP_ADDR8},
    {"BR",    0x3, 0x0, OP_ADDR8},
    {"BZ",    0x3, 0x2, OP_ADDR8},
    {"DEC",   0x2,  -1, OP_REG},
    {"DIS",   0x7, 0x1, OP_NONE},
    {"GHI",   0x9,  -1, OP_REG},
    {"GLO",   0x8,  -1, OP_REG},
    {"IDL",   0x0, 0x0, OP_NONE},
    {"INC",   0x1,  -1, OP_REG},
    {"INP",   0x6,  -1, OP_PORT_INP}, 
    {"IRX",   0x6, 0x0, OP_NONE},
    {"LBDF",  0xC, 0x3, OP_ADDR16}, // Long branches are OP_ADDR16
    {"LBNF",  0xC, 0xB, OP_ADDR16},
    {"LBNQ",  0xC, 0x9, OP_ADDR16},
    {"LBNZ",  0xC, 0xA, OP_ADDR16},
    {"LBQ",   0xC, 0x1, OP_ADDR16},
    {"LBR",   0xC, 0x0, OP_ADDR16},
    {"LBZ",   0xC, 0x2, OP_ADDR16},
    {"LDA",   0x4,  -1, OP_REG},
    {"LDI",   0xF, 0x8, OP_IMM},
    {"LDN",   0x0,  -1, OP_REG_N0}, 
    {"LDX",   0xF, 0x0, OP_NONE},
    {"LDXA",  0x7, 0x2, OP_NONE},
    {"LSDF",  0xC, 0xF, OP_NONE},   // Long skips take 0 operands in source code
    {"LSIE",  0xC, 0xC, OP_NONE},
    {"LSKP",  0xC, 0x8, OP_NONE},
    {"LSNF",  0xC, 0x7, OP_NONE},
    {"LSNQ",  0xC, 0x5, OP_NONE},
    {"LSNZ",  0xC, 0x6, OP_NONE},
    {"LSQ",   0xC, 0xD, OP_NONE},
    {"LSZ",   0xC, 0xE, OP_NONE},
    {"MARK",  0x7, 0x9, OP_NONE},
    {"NOP",   0xC, 0x4, OP_NONE},
    {"OR",    0xF, 0x1, OP_NONE},
    {"ORI",   0xF, 0x9, OP_IMM},
    {"OUT",   0x6,  -1, OP_PORT_OUT}, 
    {"PHI",   0xB,  -1, OP_REG},
    {"PLO",   0xA,  -1, OP_REG},
    {"REQ",   0x7, 0xA, OP_NONE},
    {"RET",   0x7, 0x0, OP_NONE},
    {"RSHL",  0x7, 0xE, OP_NONE}, 
    {"RSHR",  0x7, 0x6, OP_NONE}, 
    {"SAV",   0x7, 0x8, OP_NONE},
    {"SD",    0xF, 0x5, OP_NONE},
    {"SDB",   0x7, 0x5, OP_NONE},
    {"SDBI",  0x7, 0xD, OP_IMM},
    {"SDI",   0xF, 0xD, OP_IMM},
    {"SEP",   0xD,  -1, OP_REG},
    {"SEQ",   0x7, 0xB, OP_NONE},
    {"SEX",   0xE,  -1, OP_REG},
    {"SHL",   0xF, 0xE, OP_NONE},
    {"SHLC",  0x7, 0xE, OP_NONE},
    {"SHR",   0xF, 0x6, OP_NONE},
    {"SHRC",  0x7, 0x6, OP_NONE},
    {"SM",    0xF, 0x7, OP_NONE},
    {"SMB",   0x7, 0x7, OP_NONE},
    {"SMBI",  0x7, 0xF, OP_IMM},
    {"SMI",   0xF, 0xF, OP_IMM},
    {"STR",   0x5,  -1, OP_REG},
    {"STXD",  0x7, 0x3, OP_NONE},
    {"XOR",   0xF, 0x3, OP_NONE},
    {"XRI",   0xF, 0xB, OP_IMM}
};

const int OPCODE_TABLE_SIZE = sizeof(opcode_table) / sizeof(opcode_table[0]);

#endif
