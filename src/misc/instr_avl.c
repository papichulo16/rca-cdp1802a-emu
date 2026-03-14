#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// refactored to be based off of mnemonic instead of opcodes
// also added an int size since we increment a program counter
#include "../intf/instruction.h"
#include "../intf/instr_avl.h"


static int get_height(struct mnemonic_node *n) {
  if (n == NULL) {
    return 0;
  }
  return n->height;
}

struct mnemonic_node *create_mnemonic_node(Instruction instr) {
  struct mnemonic_node *node =
      (struct mnemonic_node *)malloc(sizeof(struct mnemonic_node));
  node->instr = instr;
  node->left = NULL;
  node->right = NULL;
  node->height = 1;
  return node;
}

static int max(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

static int get_balance_factor(struct mnemonic_node *n) {
  if (n == NULL) {
    return 0;
  }
  return get_height(n->left) - get_height(n->right);
}

// Right rotation
static struct mnemonic_node *r_rotate(struct mnemonic_node *y) {
  struct mnemonic_node *x = y->left;
  struct mnemonic_node *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = max(get_height(y->left), get_height(y->right)) + 1;
  x->height = max(get_height(x->left), get_height(x->right)) + 1;

  return x;
}

// Left Rotation
static struct mnemonic_node *l_rotate(struct mnemonic_node *x) {
  struct mnemonic_node *y = x->right;
  struct mnemonic_node *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(get_height(x->left), get_height(x->right)) + 1;
  y->height = max(get_height(y->left), get_height(y->right)) + 1;

  return y;
}

struct mnemonic_node *mnemonic_insert(struct mnemonic_node *node, Instruction instr) {
  if (node == NULL) {
    return create_mnemonic_node(instr);
  }

  int compare = strcmp(instr.mnemonic, node->instr.mnemonic);

  if (compare < 0) {
    node->left = mnemonic_insert(node->left, instr);
  } else if (compare > 0) {
    node->right = mnemonic_insert(node->right, instr);
  } else {
    return node;
  }

  node->height = 1 + max(get_height(node->left), get_height(node->right));
  int balance = get_balance_factor(node);

  // Left Left
  if (balance > 1 && strcmp(instr.mnemonic, node->left->instr.mnemonic) < 0)
    return r_rotate(node);

  // Right Right
  if (balance < -1 && strcmp(instr.mnemonic, node->right->instr.mnemonic) > 0)
    return l_rotate(node);

  // Left Right
  if (balance > 1 && strcmp(instr.mnemonic, node->left->instr.mnemonic) > 0) {
    node->left = l_rotate(node->left);
    return r_rotate(node);
  }

  // Right Left
  if (balance < -1 && strcmp(instr.mnemonic, node->right->instr.mnemonic) < 0) {
    node->right = r_rotate(node->right);
    return l_rotate(node);
  }

  return node;
}

// queue logic has been murdered
//
Instruction *mnemonic_find(struct mnemonic_node *root,
                                  const char *mnemonic) {
  if (!root)
    return NULL;

  int cmp = strcmp(mnemonic, root->instr.mnemonic);

  if (cmp == 0) {
    return &root->instr;
  } else if (cmp < 0) {
    return mnemonic_find(root->left, mnemonic);
  } else {
    return mnemonic_find(root->right, mnemonic);
  }
}

// ai generated main testing func (lol)

int main() {
  struct mnemonic_node *root = NULL;

  // 1. Populate the AVL Tree with some RCA 1802 instructions
  // Format is {opcode, mnemonic, size_in_bytes}
  Instruction insts[] = {{0x00, "IDL", 1},
                         {0x01, "LDN", 1},
                         {0x30, "BR", 2},
                         {0xF8, "LDI", 2},
                         {0xA0, "PLO", 1}};

  int num_insts = sizeof(insts) / sizeof(insts[0]);
  for (int i = 0; i < num_insts; i++) {
    root = mnemonic_insert(root, insts[i]);
  }

  printf("RCA 1802 Instruction AVL Tree Initialized.\n");
  printf("------------------------------------------\n");

  // 2. Interactive test loop
  char input[50];

  while (1) {
    printf("Enter a mnemonic to search (e.g., LDI, BR) or type 'QUIT': ");

    if (scanf("%49s", input) != 1) {
      break; // Exit on read error
    }

    if (strcmp(input, "QUIT") == 0) {
      break;
    }

    // 3. Test the lookup
    Instruction *result = mnemonic_find(root, input);

    if (result) {
      printf(
          ">>> MATCH: Mnemonic '%s' -> Opcode: 0x%02X | Size: %d byte(s)\n\n",
          result->mnemonic, result->opcode, result->size);
    } else {
      printf(">>> MISS: Mnemonic '%s' not found in the instruction set.\n\n",
             input);
    }
  }

  printf("Exiting test.\n");
  // Note: In a full program, you'd want to write a free_tree(root) function
  // to avoid memory leaks before exiting.

  return 0;
}
