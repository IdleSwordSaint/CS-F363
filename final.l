%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXVARS 1000

typedef struct {
    char name[50];
} Variable;

Variable var_table[MAXVARS];  // Variable storage
int var_count = 0;            // Number of declared variables

// Helper to avoid direct string comparisons
static int match(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

int is_keyword(char *word) {
    char *keywords[] = {"program", "if", "int", "then", "else", "for", 
                      "do", "while", "char", "VarDecl", "scan", "print", 
                      "main", "begin", "end", "to", "inc", "dec", NULL};
    int i = 0;
    while(keywords[i]) {
        if(match(word, keywords[i++])) {
            return 1;
        }
    }
    return 0;
}

int is_redeclared(char *word) {
    int i = var_count;
    while(i-- > 0) {
        if(match(var_table[i].name, word)) return 1;
    }
    return 0;
}

void add_variable(char *word) {
    char *dest = var_table[var_count].name;
    while((*dest++ = *word++));
    var_count++;
}

int count_char(const char *str, char c) {
    int n = 0;
    for(; *str; str++) {
        n += (*str == c);
    }
    return n;
}

// Global variables with different ordering

Variable var_table[MAXVARS];
int line_number = 1;

%}

DIGIT  [0-9]
LOWER  [a-z]
ID     {LOWER}(({LOWER}|{DIGIT})*_?({LOWER}|{DIGIT})*)
DECIMAL \(({DIGIT}+|{ID})" "?," "?10\)
OCTAL   \([0-7]+," "?8\)
BINARY  \([01]+," "?2\)
INVALID_INT \([0-9]*," "*[0-9]*\)
INVALID_VAR (_.*|{DIGIT}.*|{LOWER}(({LOWER}|{DIGIT})*_?({LOWER}|{DIGIT})*)*)
SCAN_INVALID scan\(.*\)\;?
SCAN_VALID scan\(\".*\"(" "*," "*{ID})*\)\;
PRINT_VALID print\(\".*\"(" "*," "*{ID})*\)\;
PRINT_INVALID print\(.*\)\;?
ARRAYID {ID}\[({DIGIT}+|{ID})\]
%start VARDECL VARDECL_ID VARDECL_TYPE

%%
"begin VarDecl:"    { printf("begin Keyword\nVarDecl Keyword\n: Separator\n"); BEGIN(VARDECL); }
"end VarDecl"       { printf("end Keyword\nVarDecl Keyword\n"); BEGIN(INITIAL); }

<VARDECL>"("        { printf("( Separator\n"); BEGIN(VARDECL_ID); }   // Switch to ID check state
<VARDECL>")"        { printf(") Separator\n"); }
<VARDECL>";"        { printf("; Separator\n"); }

<VARDECL_ID>[a-z][a-z0-9_]* {
    if (is_keyword(yytext)) {
        printf("%s Error: Keyword is used as an identifier\n", yytext);
    } else if (is_redeclared(yytext)) {
        printf("%s Error: Identifier already declared\n", yytext);
    } else if (yytext[0] == '_' || count_char(yytext, '_') > 1 || (yytext[0] >= '0' && yytext[0] <= '9')) {
        printf("%s Error: Invalid identifier\n", yytext);
    }else {
        printf("%s Identifier\n", yytext);
        add_variable(yytext);
    }
    BEGIN(VARDECL_TYPE);   // Switch to TYPE check state
}

<VARDECL_ID>","     { printf(", Separator\n"); }  // This ensures proper format

<VARDECL_TYPE>"int"|"char" { printf("%s Keyword\n", yytext); }
<VARDECL_TYPE>")"    { printf(") Separator\n"); BEGIN(VARDECL); } // Go back to VARDECL
{SCAN_VALID} {
    char format_string[256];
    int at_count = 0, var_count = 0;
    sscanf(yytext, "scan(\"%[^\"]\"", format_string);
    //printf("format_string: %s\n", format_string);
    at_count = count_char(format_string, '@');
    //printf("at_count:%d\n", at_count);
    const char *comma_ptr = strchr(yytext, '\"');
    //printf("comma_ptr:%s\n", comma_ptr);
    var_count = count_char(comma_ptr, ',');

    if(at_count > 1){
    	var_count += 1;
    	var_count -= at_count;
    }

    /* Check if counts match */
    if (at_count != var_count) {
        printf("%s Error: invalid input statement\n", yytext);
    } else {
        printf("%s Valid input statement\n", yytext);
    }
}
{SCAN_INVALID} { printf("%s Invalid input statement\n", yytext); }
{PRINT_VALID} {
    char format_string[256];
    int at_count = 0, var_count = 0;
    sscanf(yytext, "print(\"%[^\"]\"", format_string);
    //printf("format_string: %s\n", format_string);
    at_count = count_char(format_string, '@');
    //printf("at_count:%d\n", at_count);
    const char *comma_ptr = strchr(yytext, '\"');
    //printf("comma_ptr:%s\n", comma_ptr);
    var_count = count_char(comma_ptr, ',');

    if(at_count > 1){
    	var_count += 1;
    	var_count -= at_count;
    }

    /* Check if counts match */
    if (at_count != var_count) {
        printf("%s Error: invalid output statement\n", yytext);
    } else {
        printf("%s Valid output statement\n", yytext);
    }
}
{PRINT_INVALID} { printf("%s Invalid output statement\n", yytext); }

(int|char|if|else|while|for|main|begin|end|print|scan|program|VarDecl|inc|dec|then|to|do) { printf("%s Keyword \n", yytext); }
{ID} { printf("%s Identifier \n", yytext); }
{DECIMAL} { printf("%s Integer Constant \n", yytext); }
{OCTAL}   { printf("%s Integer Constant \n", yytext); }
{BINARY}  { printf("%s Integer Constant \n", yytext); }
{ARRAYID} { printf("%s ARRAYID \n", yytext);}
{INVALID_INT} { printf("%s Error: Invalid integer constant \n", yytext); }
{INVALID_VAR} { printf("%s Error: Invalid identifier constant \n", yytext); }
'([^\\'\n]|\\.)' { printf("%s CHARACTER CONSTANT\n", yytext); }
\"([^\\\"\n]|\\.)*\" { printf("%s STRING CONSTANT \n", yytext); }
[+\-*/%] { printf("%s Arithmetic operator\n", yytext); }
[=<>]=? { printf("%s Relational operator\n", yytext); }
[:=+\-*/%]= { printf("%s Assignment operator\n", yytext); }
[(),;{}:] { printf("%s Separator\n", yytext); }

\/\/.* {  }
\/\*([^*]|\*+[^*/])*\*\/ {  }
\n { line_number++; }
[ \t]+ ;
. { printf("%s LEXICAL ERROR: Unrecognized token\n", yytext); }

%%

int main() {
    yyin = fopen("Input.txt", "r");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
