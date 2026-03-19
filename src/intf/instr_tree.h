#pragma once

typedef struct {
	int opcode;
  int bytes;
	const char *mnemonic;
} Instruction;

struct instr_node {
	Instruction* instr;
	struct instr_node* left;
	struct instr_node* right;
	int height;
};

Instruction* instr_tree_find_by_op(int op);
void instr_tree_add_node(int op, char* mnem, int bytes);
void instr_tree_init();

