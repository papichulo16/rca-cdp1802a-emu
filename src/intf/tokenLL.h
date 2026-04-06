#pragma once

#ifndef TOKENLL_H_
#define TOKENLL_H_

#include <stdio.h>
#include <stdlib.h>
#include "token.h"

typedef struct tokenNode
{
    Token data;
    struct tokenNode *next;
} tokenNode;

void insertTK(tokenNode **head, Token toInsert);


#endif
