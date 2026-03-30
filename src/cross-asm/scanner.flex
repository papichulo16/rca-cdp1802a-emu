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
LETTER [a-zA-Z_]
HEX         [0-9A-Fa-f]
REGISTER    [rR]({DIGIT}|1[0-5]|[a-fA-F])

%%

[ \t\r]+                      { /* skip spaces lol */}
;.*                           { emit_token(yyscanner, TOKEN_COMMENT); }
{REGISTER}                    { emit_token(yyscanner, TOKEN_REGISTER); }
{LETTER}({LETTER}|{DIGIT})*:  { emit_token(yyscanner, TOKEN_LABEL); }
{LETTER}({LETTER}|{DIGIT})*   { emit_token(yyscanner, TOKEN_MNEMONIC); }
0[xX]{HEX}+                   { emit_token(yyscanner, TOKEN_NUMBER); }
\${HEX}+                      { emit_token(yyscanner, TOKEN_NUMBER); }
[0-9]{HEX}*[hH]               { emit_token(yyscanner, TOKEN_NUMBER); } 
{DIGIT}+                      { emit_token(yyscanner, TOKEN_NUMBER); }
(,)                           { emit_token(yyscanner, TOKEN_COMMA); } 
(\n)                          { emit_token(yyscanner, TOKEN_EOL); }
<<EOF>>                       { emit_token(yyscanner, TOKEN_EOF); yyterminate(); } 
.                             { emit_token(yyscanner, TOKEN_UNKNOWN); }

%%

void emit_token(yyscan_t scanner, TokenType type){
  queue* qu = yyget_extra(scanner);

  char* text = yyget_text(scanner);

  Token result;

  result.type=type;

  strncpy(result.text, text, 31);
  result.text[31]='\0';

  //result.text=text;

  //number handling because apparently I forgot to do this
  //strtol does this well but also need to cast to int since it outputs a long
  if(type == TOKEN_NUMBER){
    if (text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) {
        //0x...
        result.value = (int)strtol(text, NULL, 16);
    } else if (text[0] == '$') {
        //$...
        result.value = (int)strtol(text + 1, NULL, 16);
    } else if (text[strlen(text)-1] == 'h' || text[strlen(text)-1] == 'H') {
        //...h
        result.value = (int)strtol(text, NULL, 16);
    } else {
        //just a regular decimal number
        result.value = (int)strtol(text, NULL, 10);
    }
  }

  enqueue(qu, result);
  //debug
  //printf("Enqueued"); 


}
