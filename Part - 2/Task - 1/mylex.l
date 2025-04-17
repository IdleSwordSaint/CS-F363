%{
#include "myparser.tab.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
"int"    { yylval.string = strdup("int"); return INT_TYPE; }
"char"   { yylval.string = strdup("char"); return CHAR_TYPE; }
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

[a-z][a-z0-9_]* {
    if (strchr(yytext, '_') && strchr(strchr(yytext, '_') + 1, '_')) {
        printf("Invalid identifier: %s at line %d (multiple underscores)\n", yytext, yylineno);
        exit(1);
    }
    yylval.string = strdup(yytext);
    return IDENTIFIER;
}

[0-9]+  {
    yylval.int_val = atoi(yytext);
    return DECIMAL;
}

\"[^\"]*\" {
    yylval.string = strdup(yytext);
    return STRING_LITERAL;
}

'[^']' {
    yylval.char_val = yytext[1];
    return CHAR_LITERAL;
}

"+"     { yylval.string = strdup("+"); return ADD; }
"-"     { yylval.string = strdup("-"); return SUB; }
"*"     { yylval.string = strdup("*"); return MUL; }
"/"     { yylval.string = strdup("/"); return DIV; }
"%"     { yylval.string = strdup("%"); return MOD; }
"("     { return OB; }
")"     { return CB; }
"="     { yylval.string = strdup("="); return EQ; }
">"     { yylval.string = strdup(">"); return GT; }
"<"     { yylval.string = strdup("<"); return LT; }
">="    { yylval.string = strdup(">="); return GE; }
"<="    { yylval.string = strdup("<="); return LE; }
"<>"    { yylval.string = strdup("<>"); return NE; }

[ \t]   { /* Ignore whitespace */ }
\n      { yylineno++; }
.       { printf("Unexpected character: %s at line %d\n", yytext, yylineno); exit(1); }

%%

int yywrap() {
    return 1;
}
