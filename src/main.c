#include <stdio.h>

int main(int argc, void** argv) {
  printf("hello world\n");

  return 0;
}
typedef struct Node{
  unsigned int bit_data; // data field//
  struct Node* left; 
  struct Node* right; //pointer
} Node;
char *dynamic_buffer = (char *)malloc(8 * sizeof(char));
int fgets(dynamic_buffer, 8, stdin); // 8 bytes limit
# adds node to the binary tree

int add_to_tree(Node** root, unsigned int bit_data) {
  Node* new_node = (Node*)malloc(sizeof(Node));
  if (!new_node) {
    printf("Memory error\n or issue while reading input");
    return -1; // Memory allocation failed
  }
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
int count_node(){
  if (root == NULL) {
    return 0;
  }
  return 1 + count_node(root->left) + count_node(root->right);
}
int node_count = count_node();
int avl_tree_setup(){
  if (node_count < 4) {
    return 1; // Not enough nodes to form an AVL tree
  }
  
  return 0; // AVL tree setup successful
}

int rotate_left(Node* root) {
  Node* new_root = root->right;
  root->right = new_root->left;
  new_root->left = root;
  return new_root; 
}
int rotate_right(Node* root) {
  Node* new_root = root->left;
  root->left = new_root->right;
  new_root->right = root;
  return new_root; 
}
int aVL_structure(){
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
