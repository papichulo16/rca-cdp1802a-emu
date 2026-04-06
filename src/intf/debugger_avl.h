#ifndef DEBUGGER_AVL_H
#define DEBUGGER_AVL_H

#include "instruction.h"

struct opcode_node {
    Instruction instr;
    struct opcode_node *left;
    struct opcode_node *right;
    int height;
};

struct opcode_node *opcode_insert(struct opcode_node *node, Instruction instr);
Instruction *opcode_find_by_opcode(struct opcode_node *root, int opcode);

//bfs version of the search
Instruction *opcode_find_by_mnemonic(struct opcode_node *root, const char *mnemonic);

#endif
