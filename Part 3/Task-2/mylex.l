%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "myparser.tab.h"
extern int yylineno;

char* create_string(const char* text) {
    char* str = strdup(text);
    if (!str) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    return str;
}

void remove_quotes(char* str) {
    int len = strlen(str);
    if (len >= 2) {
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}

%}

%option noyywrap
%option yylineno

%%

[ \t\r]+    { /* Skip whitespace */ }
[\n]        { /* Skip newlines */ }

"begin"     { return BEGIN_TOKEN; }
"end"       { return END_TOKEN; }
"program"   { return PROGRAM; }
"VarDecl"   { return VARDECL; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"for"       { return FOR; }
"to"        { return TO; }
"inc"       { return INC; }
"dec"       { return DEC; }
"do"        { return DO; }
"print"     { return PRINT; }
"scan"      { return SCAN; }
"int"       { yylval.string = strdup("int"); return INT_TYPE; }
"char"      { yylval.string = strdup("char"); return CHAR_TYPE; }

":"         { return COLON; }
";"         { return SEMICOLON; }
","         { return COMMA; }
"["         { return LBRACKET; }
"]"         { return RBRACKET; }
"'"         { return QUOTE; }
"\""        { return DQUOTE; }
"@"         { return AT; }

"("         { yylval.string = strdup("("); return OB; }
")"         { yylval.string = strdup(")"); return CB; }
"*"         { yylval.string = strdup("*"); return MUL; }
"+"         { yylval.string = strdup("+"); return ADD; }
"-"         { yylval.string = strdup("-"); return SUB; }
"/"         { yylval.string = strdup("/"); return DIV; }
"%"         { yylval.string = strdup("%"); return MOD; }
"="         { yylval.string = strdup("="); return EQ; }
">"         { yylval.string = strdup(">"); return GT; }
"<"         { yylval.string = strdup("<"); return LT; }
">="        { yylval.string = strdup(">="); return GE; }
"<="        { yylval.string = strdup("<="); return LE; }
"!="        { yylval.string = strdup("!="); return NE; }

":="        { return ASSIGN; }
"+="        { return PLUSEQ; }
"-="        { return MINUSEQ; }
"*="        { return MULEQ; }
"/="        { return DIVEQ; }
"%="        { return MODEQ; }

"//".*      { return SLCOMMENT; }
"/*"([^*]|"*"[^/])*"*/" { return MLCOMMENT; }

\"[^"]*\"   { 
    yylval.string = strdup(yytext);
    char* str = yylval.string;
    int len = strlen(str);
    // Remove quotes but keep them in the original string for printing
    str[len-1] = '\0';
    str++;
    return STRING_LITERAL;
}

'.'         { 
    yylval.char_val = yytext[1]; 
    return CHAR_LITERAL; 
}

[0-9]+      { 
    yylval.int_val = atoi(yytext); 
    return DECIMAL; 
}

[a-zA-Z][a-zA-Z0-9_]*  { 
    yylval.string = strdup(yytext);
    return IDENTIFIER; 
}

.           { printf("Unrecognized character: %s\n", yytext); }

%%
