%{
#include myparser.tab.h""
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

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

// Convert integer from its base representation to decimal
int convert_to_decimal(char* value_str, int base) {
    int result = 0;
    int len = strlen(value_str);
    
    for (int i = 0; i < len; ++i) {
        int digit = value_str[i] - '0';
        if (digit >= base) {
            fprintf(stderr, "Invalid digit '%c' for base %d\n", value_str[i], base);
            exit(1);
        }
        result = result * base + digit;
    }
    
    return result;
}

// Process (value, base) integer format
char* process_integer_constant(char* text) {
    // Make a copy of the input
    char* copy = strdup(text);
    if (!copy) {
        fprintf(stderr, "Memory allocation failed for integer constant\n");
        return NULL;
    }
    
    // Extract value and base
    char* value_start = copy + 1;  // Skip '('
    char* comma = strchr(value_start, ',');
    if (!comma) {
        fprintf(stderr, "Invalid integer constant format: %s\n", text);
        free(copy);
        return NULL;
    }
    
    *comma = '\0';  // Null-terminate value string
    char* value_str = value_start;
    
    // Skip whitespace after comma
    char* base_start = comma + 1;
    while (*base_start == ' ' || *base_start == '\t') base_start++;
    
    // Remove closing parenthesis
    char* paren = strchr(base_start, ')');
    if (!paren) {
        fprintf(stderr, "Missing closing parenthesis in integer constant: %s\n", text);
        free(copy);
        return NULL;
    }
    *paren = '\0';
    
    // Convert base to integer
    int base = atoi(base_start);
    if (base != 2 && base != 8 && base != 10) {
        fprintf(stderr, "Invalid base %d in integer constant: %s\n", base, text);
        free(copy);
        return NULL;
    }
    
    // Validate digits for the given base
    for (int i = 0; i < strlen(value_str); ++i) {
        char c = value_str[i];
        if (!isdigit(c)) {
            fprintf(stderr, "Invalid character '%c' in integer constant: %s\n", c, text);
            free(copy);
            return NULL;
        }
        if ((base == 2 && c > '1') || (base == 8 && c > '7')) {
            fprintf(stderr, "Digit '%c' is out of range for base %d in: %s\n", c, base, text);
            free(copy);
            return NULL;
        }
    }
    
    // Convert to decimal string representation
    int decimal_value = convert_to_decimal(value_str, base);
    char* result = malloc(20);  // Enough for any 32-bit integer
    if (!result) {
        fprintf(stderr, "Memory allocation failed for integer result\n");
        free(copy);
        return NULL;
    }
    sprintf(result, "%d", decimal_value);
    
    free(copy);
    return result;
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
"main"     { return MAIN; }

":="       { return ASSIGN; }
"+="       { return PLUSEQ; }
"-="       { return MINUSEQ; }
"*="       { return MULEQ; }
"/="       { return DIVEQ; }
"%="       { return MODEQ; }

"and"      { return AND; }
"or"       { return OR; }
"not"      { return NOT; }

"//".*     { /* Single line comment */ }
"/*"([^*]|\*+[^/*])*\*+"/" { 
    /* Multi-line comment */
    for (char *s = yytext; *s; ++s) 
        if (*s == '\n') yylineno++;
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
    yylval.sval = create_string(yytext);
    return STRING_LITERAL;
}

'[^']' {
    yylval.sval = create_string(yytext);
    return CHAR_LITERAL;
}

[a-z][a-z0-9_]* {
    yylval.sval = create_string(yytext);;
    return IDENTIFIER;
}       fprintf(stderr, "Error processing integer constant at line %d: %s\n", yylineno, yytext);
        exit(1);
[0-9]+\.[0-9]+ {
    yylval.dval = atof(yytext);
    return DOUBLE;
}}

[0-9]+ {[ \t\r]+   { /* Ignore whitespace */ }
    yylval.sval = create_string(yytext);ylineno++; }
    return INTEGER_CONST;
}   fprintf(stderr, "Unexpected character: %s at line %d\n", yytext, yylineno); 
    exit(1); 















}    return 1;int yywrap() {%%.          { printf("Unexpected character: %s at line %d\n", yytext, yylineno); }\n         { yylineno++; }[ \t\r]+   { /* Ignore whitespace */ }}    return INTEGER_CONST;    yylval.sval = process_integer_constant(yytext);\([0-9]+,[ ]*(2|8|10)\) {}

%%

int yywrap() {
    return 1;
}
