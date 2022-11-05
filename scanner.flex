%{

/* Declarations section */
#include <stdio.h>
#include "tokens.hpp"

%}

%option yylineno
%option noyywrap
digit   		([0-9])
letter  		([a-zA-Z])
whitespace		([\t\n ])
relop           ([<>!]=?|==)
char            ([^\n\r\\\"])
legal_escape [nrt0]|x[0-9a-fA-F][0-9a-fA-F]
%x WORK_STRING
%%

{digit}+          			return NUM;

void                        return VOID;
int                         return INT;
byte                        return BYTE;
b                           return B;
bool                        return BOOL;
and                         return AND;
or                          return OR;
not                         return NOT;
true                        return TRUE;
false                       return FALSE;
return                      return RETURN;
if                          return IF;
else                        return ELSE;
while                       return WHILE;
break                       return BREAK;
;                           return SC;
,                           return COMMA;
\(                          return LPAREN;
\)                          return RPAREN;
\{                          return LBRACE;
\}                          return RBRACE;
{relop}                      return RELOP;
=                           return ASSIGN;
[*+/-]                      return BINOP;
{letter}({letter}|{digit})* return ID;
\"                          BEGIN(WORK_STRING);
<WORK_STRING>[\n]                        printf("unclosed string :(\n"); BEGIN(INITIAL); return -1;
<WORK_STRING>\\[^{legal_escape}]         printf("illegal escape:(\n"); BEGIN(INITIAL);return -1;
<WORK_STRING>({char}|\\{legal_escape})*  return STRING;
<WORK_STRING>\"                          BEGIN(INITIAL);
{whitespace}				;
.		printf("Lex doesn't know what that is!: %s\n", yytext); return -1;

%%
