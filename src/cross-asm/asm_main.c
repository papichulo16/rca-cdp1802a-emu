#include "token.h"

#include "tokenQueue.h"
// specific
#include "scanner.h"

#include "lexer.h"

#include "asm.h"

#include <stdio.h>
#include <string.h>

void asm_main(char* inf) {

  // turns out the first arg is the executable name
  char *fileName = inf;

  // initialize file
  FILE *srcptr;
  srcptr = fopen(fileName, "r");

  // buffer for lines
  char lineBuf[255];

  // check if file exists before reading each line of the file, otherwise print
  // error
  if (srcptr != NULL) {
    while (fgets(lineBuf, 255, srcptr)) {
      printf("%s", lineBuf);
    }

  } else {
    printf("bro where is your source code\n");
  }

  // Currently we need to rewind the file pointer
  // otherwise literally nothing happens
  // Wont be necessary once testing code is removed
  rewind(srcptr);

  // build queue
  queue *funnyTokens;
  funnyTokens = createQueue();

  tokenize(srcptr, funnyTokens);

  // close the file
  fclose(srcptr);
}

