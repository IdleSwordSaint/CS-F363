%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <ctype.h>

typedef struct { 
    char id[50];     
    char data_type[10]; 
} SymTabEntry;        

#define MAX_ENTRIES 100
#define MAX_SYMBOLS 100 
#define SYM_MAX 255
SymTabEntry symbolList[MAX_ENTRIES];
int marker = 0;
int symbolCount = 0;
int current_line = 1;


int symbolExists(const char *id_name);
int reservedWord(const char *word);
int NumValidator(const char *value, int radix);
void storeSymbol(const char *num_str, const char *type_spec);
int idValidator(const char *identifier);
void whiteSpaceTrimmer(char *buffer);



%}

keyword (dec|main|while|if|scan|else|for|do|inc|print|begin|VarDecl|char|int|end|program)
SpecialChars [+-%/<>*=_();,:{}]
arithOp [-+*/%]
Arr \([ ]*{identifier}[ ]*\[[ ]*{decimal}[ ]*\][ ]*,[ ]*(int|char)[ ]*\)[ ]*;?
delim [\(\),;{}\"\@\:]
lc [a-z]
digit [0-9]
decimal [0-9]+
binary [01]+
octal [0-7]+
intConstant  \({decimal},[ \t]*{decimal}\)
string_constant \"([^\"\n])*\"
identifier [a-z][a-z0-9]*(_[a-z0-9]+)?
variable \([ ]*{identifier}[ ]*,[ ]*(int|char)[ ]*\)[ ]*;?


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
    marker = 1;
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
        whiteSpaceTrimmer(name);
        whiteSpaceTrimmer(type);

        // Remove trailing ) and possible ; from type
        char *paren = strchr(type, ')');
        if(paren) *paren = '\0';

        if(reservedWord(name)){
            printf("Error (line no. %d): Keyword '%s' used as an identifier\n", current_line, name);
        } else if(symbolExists(name)){
            printf("Error (line no. %d): Duplicate variable '%s'\n",current_line, name);
        } else if(!idValidator(name)){
            printf("Error (line no. %d): Invalid variable name '%s'\n",current_line, name);
        } else {
            storeSymbol(name, type);
            printf("identifier: %s\n", name);
            printf("Variable: %s , type : %s\n", name, type);
        }

        printf("Keyword: %s\n", type);
        printf("Separator: )\n");
        
        if(has_semicolon) {
            printf("Separator: ;\n");
        } else {
            printf("Error (line no. %d): Missing semicolon at the end of variable declaration\n", current_line);
        }
    }
    
    \([ ]*{identifier}[ ]*,[ ]*(int|char)[ ]*\)[ ]*[^;] {
        printf("Error (line no. %d): Missing semicolon at the end of variable declaration\n", current_line);
    }
    
    {Arr} {
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

        whiteSpaceTrimmer(name);
        whiteSpaceTrimmer(type);

        char *paren = strchr(type, ')');
        if(paren) *paren = '\0';

        if(reservedWord(name)){
            printf("Error (line no. %d): Keyword '%s' used as variable\n", current_line, name);
        } else if(symbolExists(name)){
            printf("Error (line no. %d): Duplicate variable '%s'\n",current_line, name);
        } else if(!idValidator(name)){
            printf("Error (line no. %d): Invalid variable name '%s'\n",current_line, name);
        } else {
            storeSymbol(name, type);
            printf("array: %s , size:%d, type : %s\n", name, size, type);
        }
        
        if(!has_semicolon) {
            printf("Error (line no. %d): Missing semicolon at the end of array declaration\n", current_line);
        }
    }
    
    "end"[ \t]+"VarDecl"    { BEGIN(INITIAL); printf("Keyword: end\nKeyword: VarDecl\n"); marker = 0; }
    "\n" { current_line++; printf("\n"); }
    [\t]+   ;
}

{intConstant} {
    char value[100], baseStr[10];
    int base, i, valid = 1;
    /* Use sscanf to extract the numeric value (as a string) and the base */
    if(sscanf(yytext, "(%[^,],%s", value, baseStr) != 2) {
        printf("Error (line %d): Malformed integer constant %s\n", current_line, yytext);
        valid = 0;
    } else {
        base = atoi(baseStr);
        if(base != 2 && base != 8 && base != 10) {
            printf("Error (line %d): Invalid base %d in constant %s\n", current_line, base, yytext);
            valid = 0;
        } else if(base == 2) {
            for(i = 0; i < (int)strlen(value); i++){
                if(value[i] != '0' && value[i] != '1'){
                    printf("Error (line %d): Invalid digit '%c' in binary constant %s\n", current_line, value[i], yytext);
                    valid = 0;
                    break;
                }
            }
        } else if(base == 8) {
            for(i = 0; i < (int)strlen(value); i++){
                if(value[i] < '0' || value[i] > '7'){
                    printf("Error (line %d): Invalid digit '%c' in octal constant %s\n", current_line, value[i], yytext);
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
    printf("Error (line %d): Incomplete integer constant %s\n", current_line, yytext);
}

"program:"  {
    printf("Keyword: program\nSeparator: :\n");
}

{keyword} { 
    printf("Keyword: %s\n", yytext); 
}

{decimal} { printf("Decimal value: %s\n", yytext); }

{identifier} {
    if(!symbolExists(yytext)){
        printf("Error (line no. %d): Undeclared variable '%s := (10, 2);'\n", current_line, yytext);
    } else {
        printf("Identifier: %s\n", yytext);
    }
}

\([ ]*{digit}+[ ]*,[ ]*2[ ]*\) {
    if(!NumValidator(yytext, 2)){
        printf("Error (line no. %d): Invalid binary constant '%s'\n", current_line, yytext);
    } else {
        printf("Binary constant: %s\n", yytext);
    }
}

\([ ]*{digit}+[ ]*,[ ]*8[ ]*\) {
    if(!NumValidator(yytext, 8)){
        printf("Error (line no. %d): Invalid octal constant '%s'\n", current_line, yytext);
    } else {
        printf("Octal constant: %s\n", yytext);
    }
}

\([ ]*{digit}+[ ]*,[ ]*10[ ]*\) {
    if(!NumValidator(yytext, 10)){
        printf("Error (line no. %d): Invalid decimal constant '%s'\n", current_line, yytext);
    } else {
        printf("Decimal constant: %s\n", yytext);
    }
}

"/n" { current_line++; }
[\t]+   ;
\n { current_line++; }
\" {BEGIN(STRING); printf("Start of string constant: \n");}

<STRING>{
    \@ { printf("placeholder\n"); }
    {string_constant} { printf("String constant: %s\n", yytext); }
    \" {BEGIN(INITIAL); printf("End of string constant\n"); }
    \n  ;
    [\t]+   ;
}

{arithOp} { printf("Arithmetic operator: %s\n", yytext); }
">"|"<"|">="|"<="|"="|"<>" { printf("Relational operator: %s\n", yytext); }
"-="|":="|"+="|"*="|"/="|"%=" { printf("Assignment operator: %s\n", yytext); } 
{delim} { printf("Separator: %s\n", yytext); }

\/\/.*   { printf("Single-line comment: %s\n", yytext); }
\/\*[^*]*\*+([^/*][^*]*\*+)*\/ { printf("Multi-line comment: %s\n", yytext); }

'if' { BEGIN(CONDITIONAL); printf("Start of conditional statement\n"); }

print\([ \t]*{string_constant}(([ \t]*,[ \t]*[^)]*)*)\)[ \t]* {
    if(yytext[strlen(yytext)-1] != ';')
        printf("Error (line %d): Missing semicolon in print statement: %s\n", current_line, yytext);
}

print\([ \t]*{string_constant}(([ \t]*,[ \t]*[^)]*)*)\)[ \t]*; {
    /* Check if the statement properly ends with ';' */
    if(yytext[strlen(yytext)-1] != ';') {
        printf("Error (line %d): Missing semicolon in print statement: %s\n", current_line, yytext);
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
        printf("Error (line %d): Invalid format string in print statement\n", current_line);
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
                if(idValidator(tok)) {
                    if(!symbolExists(tok)){
                        printf("Error (line %d): Undeclared variable '%s' in print statement\n", current_line, tok);
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
               current_line, atCount, exprCount);
    } else if(!errorFound) {
        printf("Valid print statement: %s\n", yytext);
    }
}

scan\([ \t]*{string_constant}([ \t]*,[ \t]*{identifier})*\)[ \t]* {
    if(yytext[strlen(yytext)-1] != ';')
        printf("Error (line %d): Missing semicolon in scan statement: %s\n", current_line, yytext);
}

scan\([ \t]*{identifier}([ \t]*,[ \t]*{identifier})*\)[ \t]*; {  
    printf("Error (line %d): Scan statement must begin with a format string containing '@'\n", current_line);
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
        printf("Error (line %d): Invalid format string in scan statement\n", current_line);
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
                if(idValidator(tok)) {
                    if(!symbolExists(tok)) {
                        printf("Error (line %d): Undeclared variable '%s' in scan statement\n", current_line, tok);
                        errorFound = 1;
                    }
                } else {
                    printf("Error (line %d): Invalid variable '%s' in scan statement\n", current_line, tok);
                    errorFound = 1;
                }
                varCount++;
            }
            tok = strtok(NULL, ",");
        }
    }
    if(atCount != varCount) {
        printf("Error (line %d): Number of '@' (%d) does not match number of variables (%d) in scan statement\n",
               current_line, atCount, varCount);
    } else if(!errorFound){
        printf("Valid scan statement: %s\n", yytext);
    }
}

<CONDITIONAL>{
    {identifier} {
        if(!symbolExists(yytext)){
            printf("Error (line no. %d): Undeclared variable '%s'\n", current_line, yytext);
        } else {
            printf("Identifier: %s\n", yytext);
        }
    }
    {arithOp} { printf("Arithmetic operator: %s\n", yytext); }
    ">"|"<"|">="|"<="|"="|"<>" { printf("Relational operator: %s\n", yytext); }
    "-="|":="|"+="|"*="|"/="|"%=" { printf("Assignment operator: %s\n", yytext); } 
    {delim} { printf("Separator: %s\n", yytext); }
    'then' { printf(" executable statements if the condition is true: \n"); }
    \n  ;
    'else' { printf(" executable statements if the condition is false: \n"); }
    'begin' { printf(" beginning to execute statements\n"); }
    'end' { BEGIN(INITIAL); printf(" end of the statements \n"); }
    [\t]+   ;
}


%%

// Refactored
int reservedWord(const char *word) {
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


int NumValidator(const char *inputStr, int radix) {
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


void whiteSpaceTrimmer(char *text) {
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


int idValidator(const char *identifier) {
    int us = 0;  // Underscore counter
    int i = 0;
    
    // Check empty string case
    if(identifier[0] == '\0') return 0;
    
    // First character checks
    if(isdigit(identifier[0])) return 0;
    if(identifier[0] == '_') us = 1;  // Count first underscore
    
    while(identifier[i] != '\0') {
        // Check valid characters
        if(!( // OR conditions
            (identifier[i] >= 'a' && identifier[i] <= 'z') ||
            (identifier[i] >= '0' && identifier[i] <= '9') ||
            (identifier[i] == '_')
        )) {
            return 0;
        }
        
        // Handle underscore counting
        if(identifier[i] == '_') {
            us++;
            if(us > 1) {  // More than one underscore
                return 0;
            }
        }
        
        // Redundant first character check
        if(i == 0) {
            if(identifier[i] == '_') { // Already checked above
                if(us > 1) return 0;   // Never true here
            }
        }
        
        i++;  // Move to next char
    }
    
    return 1; // If all checks passed
}

// done changes 9:40 pm


int symbolExists(const char *name) {
    int index = 0;
    int foundFlag = 0;  // Unnecessary flag variable
    
    // Check if symbol table is empty
    if(symbolCount <= 0) {
        return 0;  // Should be handled by loop anyway
    }
    
    while(index < symbolCount) {
        // Compare strings the hard way
        int cmpResult = strcmp(symbolList[index].id, name);
        if(cmpResult == 0) {
            foundFlag = 1;  // Should return immediately
        }
        index++;  // Manual increment
    }
    
    // Redundant multiple returns
    if(foundFlag == 1) {
        return 1;
    } else {
        return 0;
    }
}


void storeSymbol(const char *name, const char *type) {
    int currentPosition = symbolCount; // Redundant temp variable
    int nameLength = 0;
    int typeLength = 0;
    
    /* Check if table is full */
    if(currentPosition >= MAX_SYMBOLS) { // Assuming MAX_SYMBOLS exists
        // printf("Table full!"); // Commented-out debug code
        return; // Silent failure
    }
    
    // Copy name character by character (reinventing strcpy)
    for(int j = 0; name[j] != '\0'; j++) {
        symbolList[currentPosition].id[j] = name[j];
        nameLength++;
        if(j >= 99) break; // Magic number buffer size check
    }
    symbolList[currentPosition].id[nameLength] = '\0';
    
    // Repeat for type
    for(int k = 0; type[k] != '\0'; k++) {
        symbolList[currentPosition].data_type[k] = type[k];
        typeLength++;
        if(k >= 99) break; // Duplicate code
    }
    symbolList[currentPosition].data_type[typeLength] = '\0';

    symbolCount = symbolCount + 1; // Verbose increment
}



int main(int argc, char const *argv[]){
    yyin = fopen("input.txt", "r");
    yylex();
    return 0;
}








