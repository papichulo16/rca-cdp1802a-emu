#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "instr_tree.h"

int get_height(struct instr_node* n) {
	if (n == NULL) {
		return 0;
	}
	return n->height;
}

struct instr_node* create_instr_node(Instruction* instr) {
	struct instr_node* node = (struct instr_node*)malloc(sizeof(struct instr_node));
	node->instr = instr;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	return node;
}


int max(int a, int b) {
	if (a > b) {
		return a;
	} else {
		return b;
	}
}

int get_balance_factor(struct instr_node* n) {
	if (n == NULL) {
		return 0;
	}
	return get_height(n->left) - get_height(n->right);
}

//Right rotation
struct instr_node* r_rotate(struct instr_node* y) {
	struct instr_node* x = y->left;
	struct instr_node* T2 = x->right;

	x->right = y;
	y->left = T2;
	
	y->height = max(get_height(y->left), get_height(y->right))+1;
	x->height = max(get_height(x->left), get_height(x->right))+1;

	return x;
}

//Left Rotation
struct instr_node* l_rotate(struct instr_node* x) {
	struct instr_node* y = x->right;
	struct instr_node* T2 = y->left;

	y->left = x;
	x->right = T2;

        x->height = max(get_height(x->left), get_height(x->right))+1;
        y->height = max(get_height(y->left), get_height(y->right))+1;

	return y;
}

struct instr_node* insert(struct instr_node* node, Instruction* instr) {
    if (node == NULL) {
        return create_instr_node(instr);
    }

    if (instr->opcode < node->instr->opcode) {
        node->left = insert(node->left, instr);
    } else if (instr->opcode > node->instr->opcode) {
        node->right = insert(node->right, instr);
    } else {
        return node;
    }

    node->height = 1 + max(get_height(node->left), get_height(node->right));
    int balance = get_balance_factor(node);

    // Left Left
    if (balance > 1 && instr->opcode < node->left->instr->opcode)
        return r_rotate(node);

    // Right Right
    if (balance < -1 && instr->opcode > node->right->instr->opcode)
        return l_rotate(node);

    // Left Right
    if (balance > 1 && instr->opcode > node->left->instr->opcode) {
        node->left = l_rotate(node->left);
        return r_rotate(node);
    }

    // Right Left
    if (balance < -1 && instr->opcode < node->right->instr->opcode) {
        node->right = r_rotate(node->right);
        return l_rotate(node);
    }

    return node;
}

Instruction* avl_find_by_opcode(struct instr_node* root, int opcode) {
	if (!root) {
		return NULL;
	}
	if (opcode == root->instr->opcode) {
		return root->instr;
	} else if (opcode < root->instr->opcode) {
		return avl_find_by_opcode(root->left, opcode);
	} else {
		return avl_find_by_opcode(root->right, opcode);
	}
}

Instruction* avl_find_by_mnemonic(struct instr_node* root, const char* mnemonic) {
	if (!root) {
		return NULL;
	}

	struct instr_node* queue[256];
	int front = 0;
	int back = 0;

	queue[back++] = root;

	while (front < back) {
		struct instr_node* curr = queue[front++];

		if (strcmp(curr->instr->mnemonic, mnemonic) == 0) {
			return curr->instr;
		}

		if (curr->left) {
			queue[back++] = curr->left;
		}

		if (curr->right) {
			queue[back++] = curr->right;
		}
	}
	return NULL;
}

struct instr_node* instr_tree_root = NULL;

void instr_tree_add_node(int op, char* mnem, int bytes) {
  char* name = malloc(strlen(mnem) + 1);
  strcpy(name, mnem);

  Instruction* i = malloc(sizeof(Instruction));

  i->opcode = op;
  i->mnemonic = name;
  i->bytes = bytes;

  instr_tree_root = insert(instr_tree_root, i);
}

Instruction* instr_tree_find_by_op(int op) {
  return avl_find_by_opcode(instr_tree_root, op);
}

void instr_tree_init() {

  char buf[16];

  for (uint8_t i = 0; i < 16; i++) {

    memset(buf, '\0', sizeof(buf));

    if (i % 16 != 0) {
      snprintf(buf, sizeof(buf), "LDN R%d", i);
      instr_tree_add_node(i, buf, 1);
    }

    snprintf(buf, sizeof(buf), "INC R%d", i);
    instr_tree_add_node(0x10 + i, buf, 1);

    snprintf(buf, sizeof(buf), "DEC R%d", i);
    instr_tree_add_node(0x20 + i, buf, 1);

    instr_tree_add_node(0x30 + i, "B", 2);

    snprintf(buf, sizeof(buf), "LDA R%d", i);
    instr_tree_add_node(0x40 + i, buf, 1);

    snprintf(buf, sizeof(buf), "STR R%d", i);
    instr_tree_add_node(0x50 + i, buf, 1);

    snprintf(buf, sizeof(buf), "GLO R%d", i);
    instr_tree_add_node(0x80 + i, buf, 1);

    snprintf(buf, sizeof(buf), "GHI R%d", i);
    instr_tree_add_node(0x90 + i, buf, 1);

    snprintf(buf, sizeof(buf), "PLO R%d", i);
    instr_tree_add_node(0xa0 + i, buf, 1);

    snprintf(buf, sizeof(buf), "PHI R%d", i);
    instr_tree_add_node(0xb0 + i, buf, 1);

    snprintf(buf, sizeof(buf), "SEP R%d", i);
    instr_tree_add_node(0xd0 + i, buf, 1);

    snprintf(buf, sizeof(buf), "SEX R%d", i);
    instr_tree_add_node(0xe0 + i, buf, 1);

  }

  instr_tree_add_node(0, "IDL", 1);

  instr_tree_add_node(0x60, "IRX", 1);

  for (uint8_t i = 1; i <= 7; i++) {
      snprintf(buf, sizeof(buf), "OUT %d", i);
      instr_tree_add_node(0x60 + i, buf, 1);

      snprintf(buf, sizeof(buf), "INP %d", i);
      instr_tree_add_node(0x68 + i, buf, 1);
  }

  instr_tree_add_node(0x70, "RET",  1);
  instr_tree_add_node(0x71, "DIS",  1);
  instr_tree_add_node(0x72, "LDXA", 1);
  instr_tree_add_node(0x73, "STXD", 1);
  instr_tree_add_node(0x74, "ADC",  1);
  instr_tree_add_node(0x75, "SDBC", 1);
  instr_tree_add_node(0x76, "RSHR", 1);
  instr_tree_add_node(0x77, "SMB",  1);
  instr_tree_add_node(0x78, "SAV",  1);
  instr_tree_add_node(0x79, "MARK", 1);
  instr_tree_add_node(0x7a, "REQ",  1);
  instr_tree_add_node(0x7b, "SEQ",  1);
  instr_tree_add_node(0x7c, "ADCI", 2);
  instr_tree_add_node(0x7d, "SDBI", 2);
  instr_tree_add_node(0x7e, "RSHL", 1);
  instr_tree_add_node(0x7f, "SMBI", 2);

  instr_tree_add_node(0xf0, "LDX",  1);  
  instr_tree_add_node(0xf1, "OR",   1);   
  instr_tree_add_node(0xf2, "AND",  1);  
  instr_tree_add_node(0xf3, "XOR",  1);  
  instr_tree_add_node(0xf4, "ADD",  1);  
  instr_tree_add_node(0xf5, "SD",   1);  
  instr_tree_add_node(0xf6, "SHR",  1);  
  instr_tree_add_node(0xf7, "SM",   1);  
  instr_tree_add_node(0xf8, "LDI",  2);  
  instr_tree_add_node(0xf9, "ORI",  2);  
  instr_tree_add_node(0xfa, "ANI",  2);  
  instr_tree_add_node(0xfb, "XRI",  2); 
  instr_tree_add_node(0xfc, "ADI",  2);   
  instr_tree_add_node(0xfd, "SDI",  2);  
  instr_tree_add_node(0xfe, "SHL",  1);  
  instr_tree_add_node(0xff, "SMI",  2);   

  instr_tree_add_node(0x30, "SBR",  2);   
  instr_tree_add_node(0x31, "SBQ",  2);   
  instr_tree_add_node(0x32, "SBZ",  2);   
  instr_tree_add_node(0x33, "SDF",  2);   
  instr_tree_add_node(0x34, "SB1",  2);   
  instr_tree_add_node(0x35, "SB2",  2);   
  instr_tree_add_node(0x36, "SB3",  2);   
  instr_tree_add_node(0x37, "SB4",  2);   
  instr_tree_add_node(0x38, "SNBR",  2);   
  instr_tree_add_node(0x39, "SBNQ",  2);   
  instr_tree_add_node(0x3a, "SBNZ",  2);   
  instr_tree_add_node(0x3b, "SBNF",  2);   
  instr_tree_add_node(0x3c, "SBN1",  2);   
  instr_tree_add_node(0x3d, "SBN2",  2);   
  instr_tree_add_node(0x3e, "SBN3",  2);   
  instr_tree_add_node(0x3f, "SBN4",  2);   

  instr_tree_add_node(0xc0, "LBR",  3);   
  instr_tree_add_node(0xc1, "LBQ",  3);   
  instr_tree_add_node(0xc2, "LBZ",  3);   
  instr_tree_add_node(0xc3, "LBDF",  3);   
  instr_tree_add_node(0xc4, "NOP",  1);   
  instr_tree_add_node(0xc5, "LSNQ",  3);   
  instr_tree_add_node(0xc6, "LSNZ",  3);   
  instr_tree_add_node(0xc7, "LSNF",  3);   
  instr_tree_add_node(0xc8, "LSKP",  3);   
  instr_tree_add_node(0xc9, "LBNQ",  3);   
  instr_tree_add_node(0xca, "LBNZ",  3);   
  instr_tree_add_node(0xcb, "LBNF",  3);   
  instr_tree_add_node(0xcc, "LSIE",  3);   
  instr_tree_add_node(0xcd, "LSQ",  3);   
  instr_tree_add_node(0xce, "LSZ",  3);   
  instr_tree_add_node(0xcf, "LSDF",  3);   
}

