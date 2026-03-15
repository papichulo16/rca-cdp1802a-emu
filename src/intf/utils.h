#pragma once

#include <stdint.h> 

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void set_bit(uint64_t* n, uint8_t k);
void clr_bit(uint64_t* n, uint8_t k);
int power(int a, int b);

