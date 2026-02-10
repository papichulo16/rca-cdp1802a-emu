#ifndef TOKENQUEUE_H_
#define TOKENQUEUE_H_

#include "token.h"

typedef struct node {
  Token data;
  struct node *next;
} node;

typedef struct queue {
  node *front;
  node *rear;
} queue;

node *createNode(Token data);

queue *createQueue();

int isEmpty(queue *q);

void enqueue(queue *q, Token data);

Token dequeue(queue *q);

Token peek(queue *q);

#endif
