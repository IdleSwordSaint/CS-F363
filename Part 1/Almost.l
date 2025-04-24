%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char padawan[50];
} Variable;

Variable redWings[1000];
int ultra_marines = 0; 

// Helper to avoid direct string comparisons
static int match(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

int forceSensitive(char *word) {
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

int sithApprentice(char *word) {
    int i = ultra_marines;
    while(i-- > 0) {
        if(match(redWings[i].padawan, word)) return 1;
    }
    return 0;
}

void train_padawan(char *word) {
    char *dest = redWings[ultra_marines].padawan;
    while((*dest++ = *word++));
    ultra_marines++;
}

int count_midichlorians(const char *str, char c) {
    int n = 0;
    for(; *str; str++) {
        n += (*str == c);
    }
    return n;
}



Variable redWings[1000];
int line_number = 1;

%}

lc      [a-z]
dgt     [0-9]
oct     [0-7]
bin     [01]

DECIMAL \(({dgt}+|{ID})[ ]*,[ ]*10\)
BINARY  \([ ]*{bin}+[ ]*,[ ]*2[ ]*\)
OCTAL   \([ ]*{oct}+[ ]*,[ ]*8[ ]*\)

DARTH_INT \([ ]*{dgt}*[ ]*,[ ]*{dgt}*[ ]*\)
DARTH_VAR (_[^ ]*|{dgt}[^ ]*|{lc}([_]?{lc}*{dgt}*)*[_]?{lc}*{dgt}*)

SCAN scan\(\"[^\"]*\"([ ]*,[ ]*{ID})*\)[ ]*;
DARTH_SCAN scan\([^\)]*\)[ ]*;?
PRINT print\(\"[^\"]*\"([ ]*,[ ]*{ID})*\)[ ]*;
DARTH_PRINT print\([^\)]*\)[ ]*;?

FORCE_ARR {ID}\[[ ]*({dgt}+|{ID})[ ]*\]

ID      {lc}(({dgt}*[_]?{lc}*)|({lc}*[_]?{dgt}*))+

%start train_var_id force_type force


%%



<force>")"        { printf(") Separator\n"); }


<train_var_id>[a-z][a-z0-9_]* {
    int invalid_underscores = count_midichlorians(yytext, '_') > 1;
    int bad_start = (yytext[0] == '_') || (yytext[0] >= '0' && yytext[0] <= '9');
    int invalid_id = bad_start || invalid_underscores;
    
    if (forceSensitive(yytext)) {
        printf("%s Error: Keyword is used as an identifier\n", yytext);
        goto transition;
    }
    
    if (sithApprentice(yytext)) {
        printf("%s Error: identifier already declared\n", yytext);
        goto transition;
    }
    
    if (invalid_id) {
        printf("%s Error: Invalid identifier\n", yytext);
        goto transition;
    }
    
    printf("%s Identifier\n", yytext);
    train_padawan(yytext);

transition:
    BEGIN(force_type);
}

<force_type>")"    { printf(") Separator\n"); BEGIN(force); }

<train_var_id>","     { printf(", Separator\n"); }


{SCAN} {
    char format_string[256];
const char *comma_ptr = strchr(yytext, '\"');
int ultra_marines = comma_ptr ? count_midichlorians(comma_ptr, ',') : 0;
int at_count = 0;


{
    char *fmt_start = strstr(yytext, "scan(\"");
    if(fmt_start) {
        for(int q = 0; q < 6; q++) fmt_start++;
        char *fmt_end = strchr(fmt_start, '\"');
        if(fmt_end) {
            strncpy(format_string, fmt_start, fmt_end - fmt_start);
            format_string[fmt_end - fmt_start] = '\0';
        }
    }
}

at_count = count_midichlorians(format_string, '@');

ultra_marines += (at_count > 1) ? (1 - at_count) : 0;

if(at_count - ultra_marines) {
    printf("%s Error: invalid input statement\n", yytext);
} else {
    printf("%s Valid input statement\n", yytext);
}

}
<force>";"        { printf("; Separator\n"); }
<force>"("        { printf("( Separator\n"); BEGIN(train_var_id); }  
<force_type>"int"|"char" { printf("%s Keyword\n", yytext); }
{DARTH_SCAN} { printf("%s Invalid input statement\n", yytext); }
{PRINT} {
    char format_string[256];
    int at_count = 0, ultra_marines = 0;
    sscanf(yytext, "print(\"%[^\"]\"", format_string);

    at_count = count_midichlorians(format_string, '@');

    const char *comma_ptr = strchr(yytext, '\"');

    ultra_marines = count_midichlorians(comma_ptr, ',');

    if(at_count >= 2){
    	ultra_marines += 1;
    	ultra_marines -= at_count;
    }

    if (at_count - ultra_marines != 0) {
        printf("%s Error: invalid output statement\n", yytext);
    } else {
        printf("%s Valid output statement\n", yytext);
    }
}
"begin VarDecl:"    { printf("begin Keyword\nVarDecl Keyword\n: Separator\n"); BEGIN(force); }
"end VarDecl"       { printf("end Keyword\nVarDecl Keyword\n"); BEGIN(INITIAL); }
{DARTH_PRINT} { printf("%s Invalid output statement\n", yytext); }



"int"|"char"|"if"|"else"|"while"|"for"|"main"|"begin"|"end"|"print"|"scan"|"program"|"VarDecl"|"inc"|"dec"|"then"|"to"|"do" { 
    printf("%s Keyword\n", yytext); 
}

\'([^\n\\']|(\\.))\'    { printf("%s CHARACTER CONSTANT\n", yytext); }

{DECIMAL}|{OCTAL}|{BINARY}  { printf("%s Integer constant\n", yytext); }

[][(),;{}:]  { printf("%s Separator\n", yytext); }

([+*\-/%])       { printf("%s Arithmetic operator\n", yytext); }
(:=|\+=|-=|\*=|\/=|%=)  { printf("%s Assignment operator\n", yytext); }

{FORCE_ARR} { printf("%s FORCE_ARR\n", yytext);}

\"([^\"\n\\]|(\\.))*\" { printf("%s STRING CONSTANT\n", yytext); }

{ID} { if (yyleng < 16) printf("%s Identifier\n", yytext); }

(<|>|=|<=|>=)    { printf("%s Relational operator\n", yytext); }

{DARTH_VAR} { printf("%s Error: invalid identifier constant\n", yytext); }

\/\*([^*]|(\*+[^*/]))*\*\/   { /* Block comment */ }
\/\/[^\n]*       { /* Line comment */ }

{DARTH_INT} { printf("%s Error: invalid integer\n", yytext); }

[ \t]+   { /* Whitespace */ }
\n       { line_number += 1; }

.        { printf("%s LEXICAL ERROR: Unrecognized token\n", yytext); }



%%
int yywrap() {return 1;}
int main() { yyin = fopen("Input.txt", "r");yylex();return 0;}
