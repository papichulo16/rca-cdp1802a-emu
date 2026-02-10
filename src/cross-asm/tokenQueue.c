#include "tokenQueue.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>

// thanjs geeksforgeeks
//
// TODO: Test it

// Define the structure for a node of the linked list

node *createNode(Token data) {
  // Allocate memory for a new node
  node *newNode = (node *)malloc(sizeof(node));
  // Check if memory allocation was successful
  if (newNode == NULL)
    return NULL;
  // Initialize the node's data and next pointer
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}

queue *createQueue() {
  queue *newQueue = (queue *)malloc(sizeof(queue));

  newQueue->front = NULL;
  newQueue->rear = NULL;
  return newQueue;
}

int isEmpty(queue *q) { return q->front == NULL; }

void enqueue(queue *q, Token data) {
  node *newNode = createNode(data);

  if (!newNode) {
    printf("For some reason the node didn't get created");
    return;
  }

  // deal with when the queue is empty
  if (q->rear == NULL) {
    q->front = newNode;
    q->rear = newNode;
    return;
  }

  // if its not empty, attach it to whatever node currently exists
  q->rear->next = newNode;
  q->rear = newNode;
}

Token dequeue(queue *q) {
  Token result;
  if (isEmpty(q)) {
    printf("dawg you can't dequeue while the queue is empty");
    result.type = TOKEN_ERROR;
    return result;
  }

  node *temp = q->front;
  q->front = q->front->next;

  if (q->front == NULL) {
    q->rear = NULL;
  }

  Token data = temp->data;

  free(temp);

  return data;
}

Token peek(queue *q) {
  Token result;
  if (isEmpty(q)) {
    result.type = TOKEN_ERROR;
    return result;
  }

  result = q->front->data;
  return result;
}
