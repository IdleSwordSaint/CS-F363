%option noyywrap

%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char name[50];
    char type[10];
} Variable;

#define MAX_VARIABLES 100
Variable symbolTable[MAX_VARIABLES];
int symbolCount = 0;
int line_no = 1;

int isKeyword(const char *word);
int isValidIdentifier(const char *identifier);
int isVariableDeclared(const char *name);
void addVariable(const char *name, const char *type);
void trimSpaces(char *str);
int validateIntegerFormat(const char *value, int base);
int flag = 0;
%}

lowercase_letter [a-z]
uppercase_letter [A-Z]
digit [0-9]
special_symbols [+-%/<>*=_();,:{}]
arithmetic_operators [-+*/%]
separators [\(\),;{}\"\@\:]
decimal [0-9]+
binary [01]+
octal [0-7]+
/* integer_constant (\({decimal},10\) | \({binary},2\) | \({octal},8\)) */
/* string_constant [a-zA-Z0-9+\-%/<>=_();,:{}]* */
integer_constant  \({decimal},[ \t]*{decimal}\)
string_constant \"([^\"\n])*\"
keyword (int|char|if|else|while|for|main|begin|end|print|scan|program|VarDecl|inc|dec|do)
identifier [a-z][a-z0-9]*(_[a-z0-9]+)?
variable \([ ]*{identifier}[ ]*,[ ]*(int|char)[ ]*\)[ ]*;?
array \([ ]*{identifier}[ ]*\[[ ]*{decimal}[ ]*\][ ]*,[ ]*(int|char)[ ]*\)[ ]*;?

%x VARDECL
%x STRING
%x CONDITIONAL

%%

"VarDecl"[ \t]*":" {
    char *ptr = yytext;
    while(*ptr && *ptr != ':') ptr++; // Find the colon
    *ptr = '\0'; // Temporarily split the string
    printf("Keyword: %s\n", yytext);
    printf("Separator: :\n");
    BEGIN(VARDECL);
    flag = 1;
}

<VARDECL>{
    \/\/.*   { printf("Single-line comment: %s\n", yytext); }
\/\*[^*]*\*+([^/*][^*]*\*+)*\/ { printf("Multi-line comment: %s\n", yytext); }
   {variable} {
        char name[50], type[10];
        char *ptr = yytext;
        int has_semicolon = 0;
        
        // Check if the variable declaration ends with a semicolon
        has_semicolon = (yytext[strlen(yytext)-1] == ';');
        
        // Extract variable name and type
        if(*ptr == '(') printf("separator: %c\n", *ptr);
        while(*ptr && *ptr != '('){
            ptr++;
            if(*ptr == ',') printf("separator: %c\n", *ptr);
        }   // Skip to '('
        ptr++;
        sscanf(ptr, "%[^,],%s", name, type);

        // Trim spaces from name and type
        trimSpaces(name);
        trimSpaces(type);

        // Remove trailing ) and possible ; from type
        char *paren = strchr(type, ')');
        if(paren) *paren = '\0';

        if(isKeyword(name)){
            printf("Error (line no. %d): Keyword '%s' used as an identifier\n", line_no, name);
        } else if(isVariableDeclared(name)){
            printf("Error (line no. %d): Duplicate variable '%s'\n",line_no, name);
        } else if(!isValidIdentifier(name)){
            printf("Error (line no. %d): Invalid variable name '%s'\n",line_no, name);
        } else {
            addVariable(name, type);
            printf("identifier: %s\n", name);
            printf("Variable: %s , type : %s\n", name, type);
        }

        printf("Keyword: %s\n", type);
        printf("Separator: )\n");
        
        if(has_semicolon) {
            printf("Separator: ;\n");
        } else {
            printf("Error (line no. %d): Missing semicolon at the end of variable declaration\n", line_no);
        }
    }
    
    \([ ]*{identifier}[ ]*,[ ]*(int|char)[ ]*\)[ ]*[^;] {
        printf("Error (line no. %d): Missing semicolon at the end of variable declaration\n", line_no);
    }
    
    {array} {
        char name[50], type[10];
        int size;
        char *ptr = yytext;
        int has_semicolon = 0;
        
        // Check if the array declaration ends with a semicolon
        has_semicolon = (yytext[strlen(yytext)-1] == ';');
        
        // Extract variable name and type
        while(*ptr && *ptr != '(') ptr++;  // Skip to '('
        ptr++;
        sscanf(ptr, "%[^[][%d],%s", name, &size, type);

        trimSpaces(name);
        trimSpaces(type);

        char *paren = strchr(type, ')');
        if(paren) *paren = '\0';

        if(isKeyword(name)){
            printf("Error (line no. %d): Keyword '%s' used as variable\n", line_no, name);
        } else if(isVariableDeclared(name)){
            printf("Error (line no. %d): Duplicate variable '%s'\n",line_no, name);
        } else if(!isValidIdentifier(name)){
            printf("Error (line no. %d): Invalid variable name '%s'\n",line_no, name);
        } else {
            addVariable(name, type);
            printf("array: %s , size:%d, type : %s\n", name, size, type);
        }
        
        if(!has_semicolon) {
            printf("Error (line no. %d): Missing semicolon at the end of array declaration\n", line_no);
        }
    }
    
    "end"[ \t]+"VarDecl"    { BEGIN(INITIAL); printf("Keyword: end\nKeyword: VarDecl\n"); flag = 0; }
    "\n" { line_no++; printf("\n"); }
    [\t]+   ;
}

{integer_constant} {
    char value[100], baseStr[10];
    int base, i, valid = 1;
    /* Use sscanf to extract the numeric value (as a string) and the base */
    if(sscanf(yytext, "(%[^,],%s", value, baseStr) != 2) {
        printf("Error (line %d): Malformed integer constant %s\n", line_no, yytext);
        valid = 0;
    } else {
        base = atoi(baseStr);
        if(base != 2 && base != 8 && base != 10) {
            printf("Error (line %d): Invalid base %d in constant %s\n", line_no, base, yytext);
            valid = 0;
        } else if(base == 2) {
            for(i = 0; i < (int)strlen(value); i++){
                if(value[i] != '0' && value[i] != '1'){
                    printf("Error (line %d): Invalid digit '%c' in binary constant %s\n", line_no, value[i], yytext);
                    valid = 0;
                    break;
                }
            }
        } else if(base == 8) {
            for(i = 0; i < (int)strlen(value); i++){
                if(value[i] < '0' || value[i] > '7'){
                    printf("Error (line %d): Invalid digit '%c' in octal constant %s\n", line_no, value[i], yytext);
                    valid = 0;
                    break;
                }
            }
        }
    }
    if(valid)
        printf("Integer constant: %s\n", yytext);
}

"("{decimal}","[\t]*")" {   /* This rule catches incomplete integer constants like (124, ) */
    printf("Error (line %d): Incomplete integer constant %s\n", line_no, yytext);
}

"program:"  {
    printf("Keyword: program\nSeparator: :\n");
}

{keyword} { 
    printf("Keyword: %s\n", yytext); 
}

{decimal} { printf("Decimal value: %s\n", yytext); }

{identifier} {
    if(!isVariableDeclared(yytext)){
        printf("Error (line no. %d): Undeclared variable '%s := (10, 2);'\n", line_no, yytext);
    } else {
        printf("Identifier: %s\n", yytext);
    }
}

\([ ]*{digit}+[ ]*,[ ]*2[ ]*\) {
    if(!validateIntegerFormat(yytext, 2)){
        printf("Error (line no. %d): Invalid binary constant '%s'\n", line_no, yytext);
    } else {
        printf("Binary constant: %s\n", yytext);
    }
}

\([ ]*{digit}+[ ]*,[ ]*8[ ]*\) {
    if(!validateIntegerFormat(yytext, 8)){
        printf("Error (line no. %d): Invalid octal constant '%s'\n", line_no, yytext);
    } else {
        printf("Octal constant: %s\n", yytext);
    }
}

\([ ]*{digit}+[ ]*,[ ]*10[ ]*\) {
    if(!validateIntegerFormat(yytext, 10)){
        printf("Error (line no. %d): Invalid decimal constant '%s'\n", line_no, yytext);
    } else {
        printf("Decimal constant: %s\n", yytext);
    }
}

"/n" { line_no++; }
[\t]+   ;
\n { line_no++; }
\" {BEGIN(STRING); printf("Start of string constant: \n");}

<STRING>{
    \@ { printf("placeholder\n"); }
    {string_constant} { printf("String constant: %s\n", yytext); }
    \" {BEGIN(INITIAL); printf("End of string constant\n"); }
    \n  ;
    [\t]+   ;
}

{arithmetic_operators} { printf("Arithmetic operator: %s\n", yytext); }
">"|"<"|">="|"<="|"="|"<>" { printf("Relational operator: %s\n", yytext); }
"-="|":="|"+="|"*="|"/="|"%=" { printf("Assignment operator: %s\n", yytext); } 
{separators} { printf("Separator: %s\n", yytext); }

\/\/.*   { printf("Single-line comment: %s\n", yytext); }
\/\*[^*]*\*+([^/*][^*]*\*+)*\/ { printf("Multi-line comment: %s\n", yytext); }

'if' { BEGIN(CONDITIONAL); printf("Start of conditional statement\n"); }

print\([ \t]*{string_constant}(([ \t]*,[ \t]*[^)]*)*)\)[ \t]* {
    if(yytext[strlen(yytext)-1] != ';')
        printf("Error (line %d): Missing semicolon in print statement: %s\n", line_no, yytext);
}

print\([ \t]*{string_constant}(([ \t]*,[ \t]*[^)]*)*)\)[ \t]*; {
    /* Check if the statement properly ends with ';' */
    if(yytext[strlen(yytext)-1] != ';') {
        printf("Error (line %d): Missing semicolon in print statement: %s\n", line_no, yytext);
        return 0;
    }
    char args[1024];
    strncpy(args, yytext, sizeof(args)-1);
    args[sizeof(args)-1] = '\0';
    /* Extract the format string (the first string constant between quotes) */
    char format[1024];
    char *start = strchr(args, '"');
    char *end = strrchr(args, '"');
    if(start && end && end > start) {
        int len = end - start - 1;
        strncpy(format, start+1, len);
        format[len] = '\0';
    } else {
        printf("Error (line %d): Invalid format string in print statement\n", line_no);
        return 0;
    }
    /* Count number of '@' symbols in format string */
    int atCount = 0;
    for (int i = 0; i < (int)strlen(format); i++) {
        if(format[i] == '@') atCount++;
    }
    /* Now, count the comma-separated expressions after the format string */
    char *after = end + 1;
    while(*after == ' ' || *after == '\t' || *after == ',')
         after++;
    int exprCount = 0;
    int errorFound = 0;
    if(*after != ')') {
        char tokenBuffer[1024];
        strncpy(tokenBuffer, after, sizeof(tokenBuffer)-1);
        tokenBuffer[sizeof(tokenBuffer)-1] = '\0';
        /* Remove the closing ')' if present */
        char *closeParen = strchr(tokenBuffer, ')');
        if(closeParen)
            *closeParen = '\0';
        char *tok = strtok(tokenBuffer, ",");
        while(tok != NULL) {
            /* Trim leading whitespace */
            while(*tok == ' ' || *tok == '\t') tok++;
            if(strlen(tok) > 0) {
                /* Check if token is a valid identifier and declared */
                if(isValidIdentifier(tok)) {
                    if(!isVariableDeclared(tok)){
                        printf("Error (line %d): Undeclared variable '%s' in print statement\n", line_no, tok);
                        errorFound = 1;
                    }
                }
                exprCount++;
            }
            tok = strtok(NULL, ",");
        }
    }
    if(atCount != exprCount) {
        printf("Error (line %d): Number of '@' (%d) does not match number of expressions (%d) in print statement\n",
               line_no, atCount, exprCount);
    } else if(!errorFound) {
        printf("Valid print statement: %s\n", yytext);
    }
}

scan\([ \t]*{string_constant}([ \t]*,[ \t]*{identifier})*\)[ \t]* {
    if(yytext[strlen(yytext)-1] != ';')
        printf("Error (line %d): Missing semicolon in scan statement: %s\n", line_no, yytext);
}

scan\([ \t]*{identifier}([ \t]*,[ \t]*{identifier})*\)[ \t]*; {  
    printf("Error (line %d): Scan statement must begin with a format string containing '@'\n", line_no);
} 

scan\([ \t]*{string_constant}(([ \t]*,[ \t]*[^)]*)*)\)[ \t]*; {

    char args[1024];
    strncpy(args, yytext, sizeof(args)-1);
    args[sizeof(args)-1] = '\0';
    /* Extract the format string */
    char format[1024];
    char *start = strchr(args, '"');
    char *end = strrchr(args, '"');
    if(start && end && end > start) {
        int len = end - start - 1;
        strncpy(format, start+1, len);
        format[len] = '\0';
    } else {
        printf("Error (line %d): Invalid format string in scan statement\n", line_no);
        return 0;
    }
    /* Count the number of '@' symbols in the format string */
    int atCount = 0;
    for (int i = 0; i < (int)strlen(format); i++) {
        if(format[i] == '@') atCount++;
    }
    /* Count the number of comma-separated variables after the format string. */
    char *after = end + 1;
    while(*after == ' ' || *after == '\t' || *after == ',')
         after++;
    int varCount = 0;
    int errorFound = 0;
    if(*after != ')') {
        char tokenBuffer[1024];
        strncpy(tokenBuffer, after, sizeof(tokenBuffer)-1);
        tokenBuffer[sizeof(tokenBuffer)-1] = '\0';
        char *closeParen = strchr(tokenBuffer, ')');
        if(closeParen)
            *closeParen = '\0';
        char *tok = strtok(tokenBuffer, ",");
        while(tok != NULL) {
            while(*tok == ' ' || *tok == '\t') tok++;
            if(strlen(tok) > 0) {
                /* For scan, each token must be an identifier that is already declared */
                if(isValidIdentifier(tok)) {
                    if(!isVariableDeclared(tok)) {
                        printf("Error (line %d): Undeclared variable '%s' in scan statement\n", line_no, tok);
                        errorFound = 1;
                    }
                } else {
                    printf("Error (line %d): Invalid variable '%s' in scan statement\n", line_no, tok);
                    errorFound = 1;
                }
                varCount++;
            }
            tok = strtok(NULL, ",");
        }
    }
    if(atCount != varCount) {
        printf("Error (line %d): Number of '@' (%d) does not match number of variables (%d) in scan statement\n",
               line_no, atCount, varCount);
    } else if(!errorFound){
        printf("Valid scan statement: %s\n", yytext);
    }
}

<CONDITIONAL>{
    {identifier} {
        if(!isVariableDeclared(yytext)){
            printf("Error (line no. %d): Undeclared variable '%s'\n", line_no, yytext);
        } else {
            printf("Identifier: %s\n", yytext);
        }
    }
    {arithmetic_operators} { printf("Arithmetic operator: %s\n", yytext); }
    ">"|"<"|">="|"<="|"="|"<>" { printf("Relational operator: %s\n", yytext); }
    "-="|":="|"+="|"*="|"/="|"%=" { printf("Assignment operator: %s\n", yytext); } 
    {separators} { printf("Separator: %s\n", yytext); }
    'then' { printf(" executable statements if the condition is true: \n"); }
    \n  ;
    'else' { printf(" executable statements if the condition is false: \n"); }
    'begin' { printf(" beginning to execute statements\n"); }
    'end' { BEGIN(INITIAL); printf(" end of the statements \n"); }
    [\t]+   ;
}


%%

// Refactored
int isKeyword(const char *word) {
    const char* keywords[] = {"VarDecl", "program", "begin", "end", "char", 
                             "int", "if", "else", "while", "for", "main", 
                             "print", "scan", "inc", "dec", "do"};
    const size_t total = sizeof(keywords)/sizeof(*keywords);
    size_t counter = 0;
    
    while(counter < total) {
        if (strcmp(keywords[counter], word) == 0) {
            return 1;
        }
        counter++;
    }
    return 0;
}


int validateIntegerFormat(const char *inputStr, int radix) {
    const char *ptr = inputStr;
    const char *ignore_chars = " \t(),";
    
    while(*ptr) {
        // Skip ignored characters
        if(strchr(ignore_chars, *ptr)) {
            ptr++;
            continue;
        }
        
        // Validate based on numerical base
        int valid = 0;
        switch(radix) {
            case 2:
                valid = (*ptr >= '0' && *ptr <= '1');
                break;
            case 8:
                valid = (*ptr >= '0' && *ptr <= '7');
                break;
            case 10:
                valid = isdigit(*ptr);
                break;
            default:
                valid = 1; // Unknown base skips validation
        }
        
        if(!valid) return 0;
        ptr++;
    }
    return 1;
}


void trimSpaces(char *text) {
    char *front = text;
    char *back = text + strlen(text);
    const char *whitespace = " \t";
    const char *line_endings = "\n\r";
    
    // Find first non-whitespace character
    while(*front && strchr(whitespace, *front)) front++;
    
    // Find last valid character
    while(back > front) {
        char current = *(back - 1);
        if(strchr(whitespace, current) || strchr(line_endings, current)) {
            back--;
        } else {
            break;
        }
    }
    *back = '\0';
    
    // Rebase string if needed
    if(front != text) {
        char *writer = text;
        do {
            *writer++ = *front;
        } while(*front++);
    }
}


int isValidIdentifier(const char *identifier) {
    int underscore_count = 0;
    for (int i = 0; identifier[i] != '\0'; i++) {
        if (identifier[i] == '_') underscore_count++;
        if (!(islower(identifier[i]) || isdigit(identifier[i]) || identifier[i] == '_') ||
            (i == 0 && identifier[i] == '_') || underscore_count > 1 || (i == 0 && isdigit(identifier[i]))) {
            return 0;
        }
    }
    return 1;
}

int isVariableDeclared(const char *name) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0)
            return 1;
    }
    return 0;
}

void addVariable(const char *name, const char *type) {
    strcpy(symbolTable[symbolCount].name, name);
    strcpy(symbolTable[symbolCount].type, type);
    symbolCount++;
}


int main(int argc, char const *argv[]){
    yyin = fopen("input.txt", "r");
    
    yylex();

    /* printf("\nSymbol Table:\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("Variable %d: Name = %s, Type = %s\n", i + 1, symbolTable[i].name, symbolTable[i].type);
    } */
    return 0;
}








