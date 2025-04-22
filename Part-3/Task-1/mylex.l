%{
#include "myparser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


extern int yylineno;

// Helper function to allocate and set string values
char* create_string(char* text) {
    char* str = strdup(text);
    if (!str) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    return str;
}

// Validate (value, base) integer format
int validate_integer(char* value_str, char* base_str) {
    int base = atoi(base_str);
    if (base != 2 && base != 8 && base != 10)
        return 0;

    int len = strlen(value_str);
    for (int i = 0; i < len; ++i) {
        char c = value_str[i];
        if (!isdigit(c)) return 0;
        if ((base == 2 && c > '1') ||
            (base == 8 && c > '7'))
            return 0;
    }
    return 1;
}
%}

%%

"begin"    { return BEGIN_TOKEN; }
"end"      { return END_TOKEN; }
"program"  { return PROGRAM; }
"if"       { return IF; }
"else"     { return ELSE; }
"while"    { return WHILE; }
"for"      { return FOR; }
"print"    { return PRINT; }
"scan"     { return SCAN; }
"to"       { return TO; }
"inc"      { return INC; }
"dec"      { return DEC; }
"do"       { return DO; }
"VarDecl"  { return VARDECL; }
"int"      { return INT_TYPE; }
"char"     { return CHAR_TYPE; }

":="       { return ASSIGN; }
"+="       { return PLUSEQ; }
"-="       { return MINUSEQ; }
"*="       { return MULEQ; }
"/="       { return DIVEQ; }
"%="       { return MODEQ; }

"and"      { return AND; }
"or"       { return OR; }
"not"      { return NOT; }

"//".*     { yylineno++; return SLCOMMENT; }
"/*"([^*]|\*+[^/*])*\*+"/" { 
    for (char *s = yytext; *s; ++s) if (*s == '\n') yylineno++;
    return MLCOMMENT; 
}

":"        { return COLON; }
";"        { return SEMICOLON; }
","        { return COMMA; }
"["        { return LBRACKET; }
"]"        { return RBRACKET; }
"'"        { return QUOTE; }
"\""       { return DQUOTE; }
"@"        { return AT; }

"+"        { return ADD; }
"-"        { return SUB; }
"*"        { return MUL; }
"/"        { return DIV; }
"%"        { return MOD; }
"("        { return OB; }
")"        { return CB; }
"="        { return EQ; }
">"        { return GT; }
"<"        { return LT; }
">="       { return GE; }
"<="       { return LE; }
"<>"       { return NE; }

\"[^\"]*\" {
    yytext[strlen(yytext) - 1] = '\0';
    yylval.sval = create_string(yytext + 1);  // remove quotes
    return STRING_LITERAL;
}

'[^']' {
    yylval.sval = create_string(yytext);
    return CHAR_LITERAL;
}

[a-z][a-z0-9_]* {
    yylval.sval = create_string(yytext);
    return IDENTIFIER;
}

[0-9]+\.[0-9]+ {
    yylval.dval = atof(yytext);
    return DOUBLE;
}

[0-9]+ {
    yylval.ival = atoi(yytext);
    return DECIMAL;
}

\([0-9]+,[ ]*(2|8|10)\) {
    char* copy = strdup(yytext);
    char* inner = copy + 1;              // skip '('
    char* comma = strchr(inner, ',');
    *comma = '\0';                       // null-terminate value
    char* value_str = inner;
    char* base_str = comma + 1;
    base_str[strcspn(base_str, ")")] = '\0';  // remove closing ')'

    if (!validate_integer(value_str, base_str)) {
        fprintf(stderr, "Lexical error at line %d: Invalid integer constant format %s\n", yylineno, yytext);
        free(copy);
        exit(1);
    }

    yylval.sval = create_string(yytext);
    free(copy);
    return INTEGER_CONST;
}

[ \t\r]+   { /* Ignore whitespace */ }
\n         { yylineno++; }
.          { printf("Unexpected character: %s at line %d\n", yytext, yylineno); exit(1); }

%%

int yywrap() {
    return 1;
}
