%{

#include "output.hpp"
#include "parser.tab.hpp"
#include <iostream>
#include <stdlib.h>
%}

%option yylineno
%option noyywrap

whitespace ([\t\n\r ])

%%

void return VOID;
int return INT;
byte return BYTE;
bool return BOOL;
b return B;
and return AND;
or return OR;
not return NOT;
true return TRUE;
false return FALSE;
return return RETURN;
if return IF;
else return ELSE;
while return WHILE;
break return BREAK;
continue return CONTINUE;
\; return SC;
\, return COMMA;
\( return LPAREN;
\) return RPAREN;
\{ return LBRACE;
\} return RBRACE;
(<|>|<=|>=) return RELOP;
(==|!=)  return RELOP_EQ;
[\+\-] return BINOP;
[\*\/] return BINOP_MUL_DIV;
\= return ASSIGN;
([a-zA-Z][a-zA-Z0-9]*) return ID;
([1-9][0-9]*)|[0] return NUM;
(\/\/[^\r\n]*[\r|\n|\r\n]?) ;
{whitespace} ;

(\"([^\n\r\"\\]|\\[rnt\"\\])+\") return STRING;

. {output::errorLex(yylineno);}
%%