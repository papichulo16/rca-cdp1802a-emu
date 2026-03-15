#include "utils.h"

int power(int a, int b) {

  int ret = 1;

  for (int i = 0; i < b; i++)
    ret *= a;

  return ret;
  
}

void set_bit(uint64_t* n, uint8_t k) {
    *n = (*n | (1 << k));
}

void clr_bit(uint64_t* n, uint8_t k) {
    *n = (*n & (~(1 << k)));
}


