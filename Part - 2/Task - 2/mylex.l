%{
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "myparser.tab.h"

extern int yylineno;
%}

%option yylineno

%%

"begin"         { return BEGIN_TOKEN; }
"end"           { return END_TOKEN; }
"program"       { return PROGRAM; }
"VarDecl"       { return VARDECL; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"for"           { return FOR; }
"to"            { return TO; }
"inc"           { return INC; }
"dec"           { return DEC; }
"do"            { return DO; }
"print"         { return PRINT; }
"scan"          { return SCAN; }
"int"           { yylval.string = strdup("int"); return INT_TYPE; }
"char"          { yylval.string = strdup("char"); return CHAR_TYPE; }

[a-z][a-z0-9_]* { 
    if (strlen(yytext) > 1 && strchr(yytext, '_') && strchr(strchr(yytext, '_')+1, '_')) {
        printf("Error: Invalid identifier '%s' at line %d\n", yytext, yylineno);
        exit(1);
    }
    yylval.string = strdup(yytext);
    return IDENTIFIER;
}

[0-9]+          { yylval.int_val = atoi(yytext); return DECIMAL; }

\"[^\"]*\"      { 
    yylval.string = strdup(yytext); 
    return STRING_LITERAL; 
}

\'.\'           { yylval.char_val = yytext[1]; return CHAR_LITERAL; }
\'\\n\'         { yylval.char_val = '\n'; return CHAR_LITERAL; }
\'\\t\'         { yylval.char_val = '\t'; return CHAR_LITERAL; }

":="            { return ASSIGN; }
"+="            { return PLUSEQ; }
"-="            { return MINUSEQ; }
"*="            { return MULEQ; }
"/="            { return DIVEQ; }
"%="            { return MODEQ; }

"+"             { yylval.string = strdup("+"); return ADD; }
"-"             { yylval.string = strdup("-"); return SUB; }
"*"             { yylval.string = strdup("*"); return MUL; }
"/"             { yylval.string = strdup("/"); return DIV; }
"%"             { yylval.string = strdup("%"); return MOD; }

"=="            { yylval.string = strdup("=="); return EQ; }
">"             { yylval.string = strdup(">"); return GT; }
"<"             { yylval.string = strdup("<"); return LT; }
">="            { yylval.string = strdup(">="); return GE; }
"<="            { yylval.string = strdup("<="); return LE; }
"!="            { yylval.string = strdup("!="); return NE; }

":"             { return COLON; }
";"             { return SEMICOLON; }
","             { return COMMA; }
"("             { return OB; }
")"             { return CB; }
"["             { return LBRACKET; }
"]"             { return RBRACKET; }
"{"             { return LBRACKET; }
"}"             { return RBRACKET; }
"\""            { return DQUOTE; }
"@"             { return AT; }

"//"[^\n]*      { return SLCOMMENT; }
"/*"([^*]|\*+[^*/])*\*+"/" { return MLCOMMENT; }

[ \t\n]+        { /* Ignore whitespace */ }
.               { printf("Unexpected character '%s' at line %d\n", yytext, yylineno); exit(1); }

%%

int yywrap() {
    return 1;
}
