#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, void** argv) {
  printf("hello world\n");

  return 0;
}
typedef struct Node{
  unsigned int bit_data; // data field//
  unsigned int* opcode;
  struct Node* left; 
  struct Node* right; //pointer
} Node;
char *dynamic_buffer = (char *)malloc(8 * sizeof(char));
char* fgets_result = fgets(dynamic_buffer, 8, stdin); // 8 bytes limit

// adds node to the binary tree

int add_to_tree(Node** root, unsigned int bit_data) {
  Node* new_node = (Node*)malloc(sizeof(Node));
  if (!new_node) {
    free(dynamic_buffer);
    printf("Memory error\n or issue while reading input");
    return -1; // Memory allocation failed
  }
  new_node->op_code = op_code;
  new_node->bit_data = bit_data;
  new_node->next = NULL;

  if (*root == NULL) {
    *root = new_node;
  } else {
    Node* curr = *root;
    while (curr->left != NULL && curr->right != NULL) {
      curr = curr->left;
    }
    if (curr->left == NULL) {
      curr->left = new_node;
    } else {
      curr->right = new_node;
    }
  }
  
  return 0; // worked
}
free(dynamic_buffer);
int count_node(Node* root){
  if (root == NULL) {
    return 0;
  }
  return 1 + count_node(root->left) + count_node(root->right);
}

int node_count = count_node(root);  
int get_height(Node* node) {
  if (node == NULL) {
    return 0;
  }
  int left_height = get_height(node->left);
  int right_height = get_height(node->right);
  return (left_height > right_height ? left_height : right_height) + 1;
}
int get_data(Node* node) {
  if (node == NULL) {
    return -1; 
  }
  return node->bit_data;
}
int get_OPcode(Node* node) {
  if( node == NULL) {
    return -1;}
return node->op_code;
}

int get_balance_factor(Node* node) {
  if (node == NULL) {
    return 0;
  }
  return get_height(node->left) - get_height(node->right);
}

Node* rotate_right(Node* root) {
  Node* new_root = root->left;
  Node* temp = new_root->right;
  

  new_root->right = root;
  root->left = temp;
  
  return new_root;
}

Node* rotate_left(Node* root) {
  Node* new_root = root->right;
  Node* temp = new_root->left;
  
  new_root->left = root;
  root->right = temp;
  
  return new_root;
}
Node* balance_avl_tree(Node* root) {
  if (root == NULL) {
    return NULL;
  }
  
  root->left = balance_avl_tree(root->left);
  root->right = balance_avl_tree(root->right);
  
  int balance_factor = get_balance_factor(root);
  
  if (balance_factor > 1) {
    if (get_balance_factor(root->left) < 0) {
      root->left = rotate_left(root->left);
    }
    return rotate_right(root);
  }
  
  if (balance_factor < -1) {
    if (get_balance_factor(root->right) > 0) {
      root->right = rotate_right(root->right);
    }
    return rotate_left(root);
  }
  
  return root;
}

int avl_tree_setup(int node_count){  
  if (node_count < 4) {
    return -1; 
  }
  *root = balance_avl_tree(*root);
  return 0;
}
// claude debugged this 
