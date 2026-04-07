#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//the split between the two
#include "../intf/instruction.h"
#include "../intf/debugger_avl.h"



static int get_height(struct opcode_node *n) {
  if (n == NULL) {
    return 0;
  }
  return n->height;
}

static struct opcode_node *create_instr_node(Instruction instr) {
  struct opcode_node *node =
      (struct opcode_node *)malloc(sizeof(struct opcode_node));
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

static int get_balance_factor(struct opcode_node *n) {
  if (n == NULL) {
    return 0;
  }
  return get_height(n->left) - get_height(n->right);
}

// Right rotation
static struct opcode_node *r_rotate(struct opcode_node *y) {
  struct opcode_node *x = y->left;
  struct opcode_node *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = max(get_height(y->left), get_height(y->right)) + 1;
  x->height = max(get_height(x->left), get_height(x->right)) + 1;

  return x;
}

// Left Rotation
static struct opcode_node *l_rotate(struct opcode_node *x) {
  struct opcode_node *y = x->right;
  struct opcode_node *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(get_height(x->left), get_height(x->right)) + 1;
  y->height = max(get_height(y->left), get_height(y->right)) + 1;

  return y;
}

struct opcode_node *opcode_insert(struct opcode_node *node, Instruction instr) {
  if (node == NULL) {
    return create_instr_node(instr);
  }

  if (instr.opcode < node->instr.opcode) {
    node->left = opcode_insert(node->left, instr);
  } else if (instr.opcode > node->instr.opcode) {
    node->right = opcode_insert(node->right, instr);
  } else {
    return node;
  }

  node->height = 1 + max(get_height(node->left), get_height(node->right));
  int balance = get_balance_factor(node);

  // Left Left
  if (balance > 1 && instr.opcode < node->left->instr.opcode)
    return r_rotate(node);

  // Right Right
  if (balance < -1 && instr.opcode > node->right->instr.opcode)
    return l_rotate(node);

  // Left Right
  if (balance > 1 && instr.opcode > node->left->instr.opcode) {
    node->left = l_rotate(node->left);
    return r_rotate(node);
  }

  // Right Left
  if (balance < -1 && instr.opcode < node->right->instr.opcode) {
    node->right = r_rotate(node->right);
    return l_rotate(node);
  }

  return node;
}

Instruction *opcode_find_by_opcode(struct opcode_node *root, int opcode) {
  if (!root) {
    return NULL;
  }
  if (opcode == root->instr.opcode) {
    return &root->instr;
  } else if (opcode < root->instr.opcode) {
    return opcode_find_by_opcode(root->left, opcode);
  } else {
    return opcode_find_by_opcode(root->right, opcode);
  }
}

Instruction *opcode_find_by_mnemonic(struct opcode_node *root,
                                  const char *mnemonic) {
  if (!root) {
    return NULL;
  }

  struct opcode_node *queue[256];
  int front = 0;
  int back = 0;

  queue[back++] = root;

  while (front < back) {
    struct opcode_node *curr = queue[front++];

    if (strcmp(curr->instr.mnemonic, mnemonic) == 0) {
      return &curr->instr;
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

