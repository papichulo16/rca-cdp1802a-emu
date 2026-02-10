#pragma once

#ifndef TOKEN_H_
#define TOKEN_H_

#include <stdio.h>

// magic

typedef enum {
  TOKEN_EOF,      // we shouldn't be encountering this, but we do have it
  TOKEN_EOL,      // this means \n in our situation
  TOKEN_MNEMONIC, // self explanatory
  TOKEN_REGISTER, // registers
  TOKEN_LABEL,    // you know how you do stuff like start:
  TOKEN_COMMA,    // commas
  TOKEN_NUMBER,   // hex, decimal, etc
  TOKEN_COMMENT,  // comments in assembly, obivously we make a token but then it
                  // gets erased
  TOKEN_UNKNOWN,  // bro is just typing stuff in his source code
  TOKEN_ERROR // This  token would only appear in an error state in which the
              // lexer immediately kills operation
} TokenType;

typedef struct {
  TokenType type;
  char text[32]; // lets run with 32 bytes for now and see what happens
  int value;     // if this token is actually a number
} Token;

#endif
