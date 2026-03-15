#pragma once

#include <stdint.h> 

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void set_red();
void set_green();
void set_yellow();
void set_blue();
void set_magenta();
void set_cyan();
void set_white();
void reset_color();
void clear_screen();

void print_byte(uint8_t word);
void print_word(uint16_t word);
void print_dword(uint32_t word);
void print_qword(uint64_t word);

void set_bit(uint64_t* n, uint8_t k);
void clr_bit(uint64_t* n, uint8_t k);
int power(int a, int b);

