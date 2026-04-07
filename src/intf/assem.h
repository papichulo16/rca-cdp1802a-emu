#pragma once

#ifndef ASSEM_H_
#define ASSEM_H_

#include <stdint.h>
#include <stdio.h>

void assem(queue *tokens, char *outf);

typedef struct memorySegment {
  uint16_t ORG;
  uint16_t OFF;
} memorySegment;

typedef struct codeBlock {
  uint16_t start;
  uint16_t end;
} codeBlock;

#endif
