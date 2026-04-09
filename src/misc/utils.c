#include "utils.h"
#include <stdio.h>

char nibbles[] = "0123456789ABCDEF";

void set_red()    { printf("\033[31m"); }
void set_green()  { printf("\033[32m"); }
void set_yellow() { printf("\033[33m"); }
void set_blue()   { printf("\033[34m"); }
void set_magenta(){ printf("\033[35m"); }
void set_cyan()   { printf("\033[36m"); }
void set_white()  { printf("\033[37m"); }
void reset_color() { printf("\033[0m"); }
void clear_screen() { printf("\033[2J\033[H"); }

void print_nibble(uint8_t byte) {
    putchar(nibbles[byte & 0xf]);
}

void print_byte(uint8_t byte) {
    printf("0x");

    for (int i = 0 ; i < 2; i++)
        print_nibble(byte >> (4 - i * 4));
}

void print_word(uint16_t word) {
    printf("0x");

    for (int i = 0 ; i < 4; i++)
        print_nibble(word >> (12 - i * 4));
}

void print_dword(uint32_t dword) {
    printf("0x");

    for (int i = 0 ; i < 8; i++)
        print_nibble(dword >> (28 - i * 4));
}

void print_qword(uint64_t qword) {
    printf("0x");

    for (int i = 0 ; i < 16; i++)
        print_nibble(qword >> (60 - i * 4));
}

int power(int a, int b) {

  int ret = 1;

  for (int i = 0; i < b; i++)
    ret *= a;

  return ret;
  
}

void set_bit(uint64_t* n, uint8_t k) {
    *n = (*n | (1ULL << k));
}

void clr_bit(uint64_t* n, uint8_t k) {
    *n = (*n & (~(1ULL << k)));
}

