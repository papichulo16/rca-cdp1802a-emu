#pragma once

#ifndef INSTRLL_H_
#define INSTRLL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct label
{
  char name[64];
  uint16_t address;
} label;


typedef struct labelNode
{
    label data;
    struct labelNode *next;
} labelNode;

void insertLabel(labelNode **head, label toInsert);


#endif
