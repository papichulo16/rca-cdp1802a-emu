#ifndef INSTR_AVL_H
#define INSTR_AVL_H

#include "instruction.h"

struct mnemonic_node {
    Instruction instr;
    struct mnemonic_node *left;
    struct mnemonic_node *right;
    int height;
};

struct mnemonic_node *mnemonic_insert(struct mnemonic_node *node, Instruction instr);
Instruction *mnemonic_find(struct mnemonic_node *root, const char *mnemonic);

#endif
