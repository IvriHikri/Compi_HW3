%{

#include "classes.h"
#include "hw3_output.hpp"
#include "parser.tab.hpp"
#include <iostream>
#include <stdlib.h>
%}

%option yylineno
%option noyywrap

whitespace ([\t\n\r ])

%%
void {yylval = new Node(yytext); return VOID;}
int {yylval = new Node(yytext); return INT;}
byte {yylval = new Node(yytext); return BYTE;}
bool {yylval = new Node(yytext); return BOOL;}
b {yylval = new Node(yytext); return B;}
and {yylval = new Node(yytext); return AND;}
or {yylval = new Node(yytext); return OR;}
not {yylval = new Node(yytext); return NOT;}
true {yylval = new Node(yytext); return TRUE;}
false {yylval = new Node(yytext); return FALSE;}
return {yylval = new Node(yytext); return RETURN;}
if {yylval = new Node(yytext); return IF;}
else {yylval = new Node(yytext); return ELSE;}
while {yylval = new Node(yytext); return WHILE;}
break {yylval = new Node(yytext); return BREAK;}
continue {yylval = new Node(yytext); return CONTINUE;}
\; {yylval = new Node(yytext); return SC;}
\, {yylval = new Node(yytext); return COMMA;}
\( {yylval = new Node(yytext); return LPAREN;}
\) {yylval = new Node(yytext); return RPAREN;}
\{ {yylval = new Node(yytext); return LBRACE;}
\} {yylval = new Node(yytext); return RBRACE;}
(<|>|<=|>=) {yylval = new Node(yytext); return RELOP;}
(==|!=) {yylval = new Node(yytext); return RELOP_EQ;}
[\+\-] {yylval = new Node(yytext); return BINOP;} 
[\*\/] {yylval = new Node(yytext); return BINOP_MUL_DIV;}
\= return ASSIGN;
([a-zA-Z][a-zA-Z0-9]*) {yylval = new Node(yytext); return ID;}
([1-9][0-9]*)|[0] {yylval = new Node(yytext, V_INT); return NUM;}
(\/\/[^\r\n]*[\r|\n|\r\n]?) ;
{whitespace} ;

(\"([^\n\r\"\\]|\\[rnt\"\\])+\") {yylval = new Node (yytext, V_STRING); return STRING;}

. {output::errorLex(yylineno);}
%%