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
  free(dynamic_buffer);// Free the allocated memory then cycle back to adding more data
  return 0; // worked
}
