#include <stdio.h>

int main(int argc, void** argv) {
  printf("hello world\n");

  return 0;
}
typedef struct Node{
  unsigned int bit_data; // data field//
  struct Node* next; //pointer
} Node;
