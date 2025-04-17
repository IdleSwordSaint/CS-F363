%{
#include "myparser.tab.h"
#include <stdio.h>
#include <stdlib.h>
extern int yylineno;
%}

%%

"begin"  { return BEGIN_TOKEN; }
"end"    { return END_TOKEN; }
"program" { return PROGRAM; }
"if"     { return IF; }
"else"   { return ELSE; }
"while"  { return WHILE; }
"for"    { return FOR; }
"print"  { return PRINT; }
"scan"   { return SCAN; }
"to"     { return TO; }
"inc"    { return INC; }
"dec"    { return DEC; }
"do"     { return DO; }
"VarDecl" { return VARDECL; }
"int"    { return INT_TYPE; }
"char"   { return CHAR_TYPE; }
":="     { return ASSIGN; }
"+="     { return PLUSEQ; }
"-="     { return MINUSEQ; }
"*="     { return MULEQ; }
"/="     { return DIVEQ; }
"%="     { return MODEQ; }
"//".*"\n" { yylineno++; return SLCOMMENT; }
"/*"([^*]|\*+[^/*])*\*+"/" { char *s = yytext; while (*s) if (*s++ == '\n') yylineno++; return MLCOMMENT; }
":"      { return COLON; }
";"      { return SEMICOLON; }
","      { return COMMA; }
"["      { return LBRACKET; }
"]"      { return RBRACKET; }
"'"      { return QUOTE; }
"\""     { return DQUOTE; }
"@"      { return AT; }

[a-z][a-z0-9_]* { return IDENTIFIER; }

[0-9]+  { return DECIMAL; }
\"[^\"]*\" { return STRING_LITERAL; }
'[^']' { return CHAR_LITERAL; }
"+"     { return ADD; }
"-"     { return SUB; }
"*"     { return MUL; }
"/"     { return DIV; }
"%"     { return MOD; }
"("     { return OB; }
")"     { return CB; }
"="     { return EQ; }
">"     { return GT; }
"<"     { return LT; }
">="    { return GE; }
"<="    { return LE; }
"<>"    { return NE; }
[ \t]   { /* Ignore whitespace */ }
\n      { yylineno++; }
.       { printf("Unexpected character: %s at line %d\n", yytext, yylineno); exit(1); }

%%

int yywrap() {
    return 1;
}
