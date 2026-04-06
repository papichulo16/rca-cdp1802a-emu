#ifndef INSTRUCTION_H
#define INSTRUCTION_H

typedef struct {
    int opcode;
    const char *mnemonic;
    int size;
} Instruction;

#endif // INSTRUCTION_H
