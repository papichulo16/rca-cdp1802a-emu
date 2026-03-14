#include "../intf/token.h"

#include "../intf/tokenQueue.h"

// #include "../intf/lexer.h"

#include "../intf/assem.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>


void assem(queue *tokens){
  // We should have a queue full of tokens
  //  But if not, this will tell us (until its removed)
  if (isEmpty(tokens)) {
    printf("Somehow we got to assembling, but the queue is empty\n");
    return;
  } else {
    printf("The queue is not empty, we can move on\n");
  }

  printf("Did absolutely nothing yet but I am making it look like I did something\n");

  return;

}
