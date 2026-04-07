
#include "token.h"

#include "tokenQueue.h"

#include "lexer.h"

#include "./scanner.h"

#include <stdio.h>
#include <string.h>


//This is now done in assem.c since im not interested in queueing and dequeuing a bunch lol
/*
// Function to actually tell what tokens we have from the enums
// switch statement actually makes it very intuitive
const char *get_token_name(TokenType type) {
  switch (type) {
  case TOKEN_EOF:
    return "TOKEN_EOF";
  case TOKEN_EOL:
    return "TOKEN_EOL";
  case TOKEN_MNEMONIC:
    return "TOKEN_MNEMONIC";
  case TOKEN_REGISTER:
    return "TOKEN_REGISTER";
  case TOKEN_LABEL:
    return "TOKEN_LABEL";
  case TOKEN_COMMA:
    return "TOKEN_COMMA";
  case TOKEN_NUMBER:
    return "TOKEN_NUMBER";
  case TOKEN_COMMENT:
    return "TOKEN_COMMENT";
  case TOKEN_UNKNOWN:
    return "TOKEN_UNKNOWN";
  case TOKEN_ERROR:
    return "TOKEN_ERROR";
  default:
    return "UNDEFINED_TOKEN";
  }
}

*/

void tokenize(FILE *srcptr, queue *result) {

  // initialize flex scanner, the extra data (queue), set file pointer in, then
  // call the lexing function
  yyscan_t scanner;
  yylex_init_extra(result, &scanner);
  yyset_in(srcptr, scanner);
  yylex(scanner);

  // debug code

  // We should have a queue full of tokens
  //  But if not, this will tell us (until its removed)
  if (isEmpty(result)) {
    printf("it's empty\n");
  } else {
    printf("it's not empty\n");
  }


  //This is now done in assem.c since im not interested in queueing and dequeueing lol
  /*
  // List out the tokens and their raw text
  while (!isEmpty(result)) {
    Token currToken = dequeue(result);
    printf("%s : %s\n", get_token_name(currToken.type), currToken.text);
  }

  */
}
