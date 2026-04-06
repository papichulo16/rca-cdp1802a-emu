#include "../intf/token.h"

#include "../intf/tokenQueue.h"
// specific
#include "scanner.h"

#include "../intf/lexer.h"

#include "../intf/asm.h"

#include "../intf/assem.h"

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


  //where the actual magic happens
  assem(funnyTokens);



  // close the file
  fclose(srcptr);
}

/*
int main(int argc, char** argv){
    asm_main(argv[1]);
    return 0;

}
*/
