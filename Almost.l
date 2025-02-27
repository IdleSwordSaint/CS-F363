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

digit  [0-9]

ID     {lc}(({lc}|{digit})*_?({lc}|{digit})*)
DECIMAL \(({digit}+|{ID})" "?," "?10\)
OCTAL   \([0-7]+," "?8\)
BINARY  \([01]+," "?2\)
INVALID_INT \([0-9]*," "*[0-9]*\)
INVALID_VAR (_.*|{digit}.*|{lc}(({lc}|{digit})*_?({lc}|{digit})*)*)
SCAN_INVALID scan\(.*\)\;?
SCAN_VALID scan\(\".*\"(" "*," "*{ID})*\)\;
PRINT_VALID print\(\".*\"(" "*," "*{ID})*\)\;
PRINT_INVALID print\(.*\)\;?
lc  [a-z]
ARRAYID {ID}\[({digit}+|{ID})\]
%start VARDECL VARDECL_ID VARDECL_TYPE

%%



<VARDECL>")"        { printf(") Separator\n"); }


<VARDECL_ID>[a-z][a-z0-9_]* {
    int invalid_underscores = count_char(yytext, '_') > 1;
    int bad_start = (yytext[0] == '_') || (yytext[0] >= '0' && yytext[0] <= '9');
    int invalid_id = bad_start || invalid_underscores;
    
    if (is_keyword(yytext)) {
        printf("%s Error: Keyword is used as an identifier\n", yytext);
        goto transition;
    }
    
    if (is_redeclared(yytext)) {
        printf("%s Error: Identifier already declared\n", yytext);
        goto transition;
    }
    
    if (invalid_id) {
        printf("%s Error: Invalid identifier\n", yytext);
        goto transition;
    }
    
    printf("%s Identifier\n", yytext);
    add_variable(yytext);

transition:
    BEGIN(VARDECL_TYPE);  /* Changed comment phrasing */
}

<VARDECL_TYPE>")"    { printf(") Separator\n"); BEGIN(VARDECL); } // Go back to VARDECL

<VARDECL_ID>","     { printf(", Separator\n"); }  // This ensures proper format


{SCAN_VALID} {
    char format_string[256];
const char *comma_ptr = strchr(yytext, '\"');
int var_count = comma_ptr ? count_char(comma_ptr, ',') : 0;
int at_count = 0;

// Extract format string using pointer arithmetic
{
    char *fmt_start = strstr(yytext, "scan(\"");
    if(fmt_start) {
        fmt_start += 6;  // Skip "scan(\""
        char *fmt_end = strchr(fmt_start, '\"');
        if(fmt_end) {
            strncpy(format_string, fmt_start, fmt_end - fmt_start);
            format_string[fmt_end - fmt_start] = '\0';
        }
    }
}

at_count = count_char(format_string, '@');

// Handle special case with different arithmetic
var_count += (at_count > 1) ? (1 - at_count) : 0;

if(at_count - var_count) {  // Check difference instead of direct comparison
    printf("%s Error: invalid input statement\n", yytext);
} else {
    printf("%s Valid input statement\n", yytext);
}

}
<VARDECL>";"        { printf("; Separator\n"); }
<VARDECL>"("        { printf("( Separator\n"); BEGIN(VARDECL_ID); }  
<VARDECL_TYPE>"int"|"char" { printf("%s Keyword\n", yytext); }
{SCAN_INVALID} { printf("%s Invalid input statement\n", yytext); }
{PRINT_VALID} {
    char *q_pos = strchr(yytext, '\"');
    char format_string[256] = {0};
    int var_count = 0, at_count = 0;
    
    /* Extract format string using pointer arithmetic */
    if(q_pos) {
        char *start = yytext + 6;  // Skip "print("
        char *end = strchr(start, '\"');
        if(end) {
            strncpy(format_string, start, end - start);
            format_string[end - start] = '\0';
        }
    }
    
    at_count = count_char(format_string, '@');
    var_count = q_pos ? count_char(q_pos, ',') : 0;
    
    // Adjust count using different arithmetic
    var_count += (at_count > 1) ? (1 - at_count) : 0;
    
    /* Modified comparison logic */
    if(at_count - var_count != 0) {
        printf("%s Error: invalid output statement\n", yytext);
    } else {
        printf("%s Valid output statement\n", yytext);
    }
}
"begin VarDecl:"    { printf("begin Keyword\nVarDecl Keyword\n: Separator\n"); BEGIN(VARDECL); }
"end VarDecl"       { printf("end Keyword\nVarDecl Keyword\n"); BEGIN(INITIAL); }
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
int yywrap() {return 1;}
int main() { yyin = fopen("Input.txt", "r");yylex();return 0;}
