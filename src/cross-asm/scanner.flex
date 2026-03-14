%{
#include "../intf/token.h"
#include "../intf/tokenQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void emit_token(yyscan_t scanner, TokenType type);
%}

%option header-file="scanner.h" reentrant noyywrap
%option extra-type="queue*"

%option noinput
%option nounput

DIGIT [0-9]
LETTER [a-zA-Z]
HEX         [0-9A-Fa-f]
REGISTER    [rR]({DIGIT}|1[0-5]|[a-fA-F])

%%

[ \t]+      { /* skip spaces lol */}
{REGISTER}  { emit_token(yyscanner, TOKEN_REGISTER); }
[A-Za-z]{3} { emit_token(yyscanner, TOKEN_MNEMONIC); }
(\n)        { emit_token(yyscanner, TOKEN_EOL); } 
.           { emit_token(yyscanner, TOKEN_UNKNOWN); }

%%

void emit_token(yyscan_t scanner, TokenType type){
  queue* qu = yyget_extra(scanner);

  char* text = yyget_text(scanner);

  Token result;

  result.type=type;

  strncpy(result.text, text, 31);
  result.text[31]='\0';

  //result.text=text;

  /*
  if(type == TOKEN_NUMBER){
    //do funny number conversion stuff here and modify number field

  }
  */

  enqueue(qu, result);
  //debug
  //printf("Enqueued"); 


}
