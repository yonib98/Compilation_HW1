%{

/* Declarations section */
#include <stdio.h>
#include "tokens.hpp"

%}

%option yylineno
%option noyywrap
digit   		([0-9])
letter  		([a-zA-Z])
whitespace		([\t\n \r])
relop           ([<>!]=?|==)
char            ([^\\"\n\r])
legal_escape \\[\\"nrt0x]
illegal_escape \\[^\\"nrt0x]
qu              ([\"])
comment         (\/\/)
%x WORK_STRING
%x WORK_COMMENT
%%

0|[1-9]{digit}*      			return NUM;
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
break                      return BREAK;
continue                   return CONTINUE;
;                           return SC;
,                           return COMMA;
\(                          return LPAREN;
\)                          return RPAREN;
\{                          return LBRACE;
\}                          return RBRACE;
{comment}[^\n\r]*           return COMMENT;
{relop}                      return RELOP;
=                           return ASSIGN;
[*+/-]                      return BINOP;
{letter}({letter}|{digit})* return ID;

{qu}                        BEGIN(WORK_STRING);
<WORK_STRING>({char}|{legal_escape})*{qu}  BEGIN(INITIAL); return STRING;
<WORK_STRING>({char}|{illegal_escape})*{qu} BEGIN(INITIAL); return 30;
<WORK_STRING>.                              BEGIN(INITIAL); return 31;

{whitespace}				;

.	                        return -2;

%%
