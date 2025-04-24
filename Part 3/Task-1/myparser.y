%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;
extern int yylineno;
void yyerror(const char* s);

int temp_count = 0, label_count = 0;

// ---- Memory management ----
char** allocated_memory = NULL;
int allocated_count = 0;

void track_allocation(char* ptr) {
    allocated_memory = realloc(allocated_memory, sizeof(char*) * (allocated_count + 1));
    allocated_memory[allocated_count++] = ptr;
}

void cleanup_memory() {
    for (int i = 0; i < allocated_count; i++)
        free(allocated_memory[i]);
    free(allocated_memory);
}

// ---- Temporary and Label Generators ----
char* newTemp() {
    char* temp = malloc(10);
    sprintf(temp, "t%d", temp_count++);
    track_allocation(temp);
    return temp;
}

char* newLabel() {
    char* label = malloc(10);
    sprintf(label, "L%d", label_count++);
    track_allocation(label);
    return label;
}

// ---- Emit TAC ----
void emit(char* result, char* arg1, char* op, char* arg2) {
    if (arg2 && op) printf("%s = %s %s %s\n", result, arg1, op, arg2);
    else if (op)     printf("%s = %s %s\n", result, op, arg1);
    else             printf("%s = %s\n", result, arg1);
}

void emitCondJump(char* cond, char* label) {
    printf("ifFalse %s goto %s\n", cond, label);
}

void emitGoto(char* label) {
    printf("goto %s\n", label);
}

void emitLabel(char* label) {
    printf("%s:\n", label);
}

// ---- Variable table for declarations ----
#define MAX_VARS 1000
char* declared_vars[MAX_VARS];
int var_count = 0;

int is_keyword(const char* name) {
    const char* keywords[] = {
        "int", "char", "if", "else", "while", "for", "main", "begin", "end",
        "print", "scan", "program", "VarDecl", "inc", "dec", NULL
    };
    for (int i = 0; keywords[i]; ++i)
        if (strcmp(name, keywords[i]) == 0)
            return 1;
    return 0;
}

int is_declared(const char* name) {
    for (int i = 0; i < var_count; i++)
        if (strcmp(declared_vars[i], name) == 0)
            return 1;
    return 0;
}

void declare_variable(const char* name) {
    if (is_declared(name)) {
        fprintf(stderr, "Semantic error at line %d: Variable '%s' redeclared.\n", yylineno, name);
        exit(1);
    }
    if (is_keyword(name)) {
        fprintf(stderr, "Semantic error at line %d: Keyword '%s' used as identifier.\n", yylineno, name);
        exit(1);
    }

    // Basic validation: starts with lowercase, one underscore max
    int len = strlen(name), underscore_count = 0;
    if (!islower(name[0])) {
        fprintf(stderr, "Invalid variable name at line %d: '%s'. Must start with lowercase.\n", yylineno, name);
        exit(1);
    }

    for (int i = 0; i < len; i++) {
        if (!(islower(name[i]) || isdigit(name[i]) || name[i] == '_')) {
            fprintf(stderr, "Invalid character in variable name '%s'.\n", name);
            exit(1);
        }
        if (name[i] == '_') underscore_count++;
    }

    if (underscore_count > 1) {
        fprintf(stderr, "Invalid variable name at line %d: '%s'. Only one underscore allowed.\n", yylineno, name);
        exit(1);
    }

    declared_vars[var_count++] = strdup(name);
}

char* getBaseOperator(char* op) {
    const char* ops[][2] = {
        {"+=", "+"}, {"-=", "-"}, {"*=", "*"}, {"/=", "/"}, {"%=", "%"}, {NULL, NULL}
    };
    for (int i = 0; ops[i][0]; ++i)
        if (strcmp(op, ops[i][0]) == 0)
            return strdup(ops[i][1]);
    return strdup(op);
}
%}
%union {
    int ival;
    double dval;
    char* sval;
}

%token <ival> DECIMAL
%token <dval> DOUBLE
%token <sval> IDENTIFIER STRING_LITERAL CHAR_LITERAL INTEGER_CONST

%token BEGIN_TOKEN END_TOKEN PROGRAM VARDECL IF ELSE WHILE FOR TO INC DEC DO PRINT SCAN
%token INT_TYPE CHAR_TYPE MAIN
%token ASSIGN PLUSEQ MINUSEQ MULEQ DIVEQ MODEQ
%token SLCOMMENT MLCOMMENT
%token COLON SEMICOLON COMMA LBRACKET RBRACKET QUOTE DQUOTE AT
%token OB CB MUL ADD SUB DIV MOD EQ GT LT GE LE NE
%token AND OR NOT

%type <sval> expression term factor condition assignment_op relop value bool_expr
%type <sval> for_step for_dir
%type <sval> assignment
%type <sval> identifier
%type <sval> formatArgs
%type <sval> argList

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE



%left OR
%left AND
%left EQ GT LT GE LE NE
%left ADD SUB
%left MUL DIV MOD
%right NOT

%%

program
    : BEGIN_TOKEN PROGRAM COLON varDecBlock statementBlock END_TOKEN PROGRAM {
        printf("Parsing and TAC generation completed successfully.\n");
    }
    ;

statementBlock
    : /* empty */
    | statement statementBlock
    ;

statement
    : assignment SEMICOLON
    | if_cond
    | while_loop
    | for_loop
    | print
    | scan
    | block
    ;

assignment
    : identifier assignment_op expression {
        if (strcmp($2, ":=") == 0) {
            emit($1, $3, NULL, NULL);
        } else {
            char* baseOp = getBaseOperator($2);
            char* temp = newTemp();
            emit(temp, $1, baseOp, $3);
            emit($1, temp, NULL, NULL);
        }
        $$ = strdup($1); track_allocation($$);
    }
    ;

assignment_op
    : ASSIGN  { $$ = strdup(":="); track_allocation($$); }
    | PLUSEQ  { $$ = strdup("+="); track_allocation($$); }
    | MINUSEQ { $$ = strdup("-="); track_allocation($$); }
    | MULEQ   { $$ = strdup("*="); track_allocation($$); }
    | DIVEQ   { $$ = strdup("/="); track_allocation($$); }
    | MODEQ   { $$ = strdup("%="); track_allocation($$); }
    ;

expression
    : term                   { $$ = $1; }
    | expression ADD term    {
        char* temp = newTemp();
        emit(temp, $1, "+", $3); $$ = temp;
    }
    | expression SUB term    {
        char* temp = newTemp();
        emit(temp, $1, "-", $3); $$ = temp;
    }
    ;

term
    : factor                { $$ = $1; }
    | term MUL factor       {
        char* temp = newTemp();
        emit(temp, $1, "*", $3); $$ = temp;
    }
    | term DIV factor       {
        char* temp = newTemp();
        emit(temp, $1, "/", $3); $$ = temp;
    }
    | term MOD factor       {
        char* temp = newTemp();
        emit(temp, $1, "%", $3); $$ = temp;
    }
    ;

factor
    : OB expression CB       { $$ = $2; }
    | identifier             { $$ = strdup($1); track_allocation($$); }
    | INTEGER_CONST          { $$ = strdup($1); track_allocation($$); }
    | CHAR_LITERAL           { $$ = $1; }
    ;

identifier
    : IDENTIFIER {
        if (!is_declared($1)) {
            fprintf(stderr, "Semantic error: variable '%s' used before declaration.\n", $1);
            exit(1);
        }
        $$ = strdup($1); track_allocation($$);
    }
    ;

bool_expr
    : condition { $$ = $1; }
    | bool_expr AND bool_expr {
        char* temp = newTemp();
        emit(temp, $1, "and", $3); $$ = temp;
    }
    | bool_expr OR bool_expr {
        char* temp = newTemp();
        emit(temp, $1, "or", $3); $$ = temp;
    }
    | NOT bool_expr {
        char* temp = newTemp();
        emit(temp, "not", $2, NULL); $$ = temp;
    }
    ;

condition
    : expression relop expression {
        char* temp = newTemp();
        emit(temp, $1, $2, $3); $$ = temp;
    }
    ;

relop
    : EQ { $$ = strdup("="); track_allocation($$); }
    | NE { $$ = strdup("!="); track_allocation($$); }
    | GT { $$ = strdup(">"); track_allocation($$); }
    | GE { $$ = strdup(">="); track_allocation($$); }
    | LT { $$ = strdup("<"); track_allocation($$); }
    | LE { $$ = strdup("<="); track_allocation($$); }
    ;

block
    : BEGIN_TOKEN statementBlock END_TOKEN
    ;
// -- FOR loop support --

for_dir
    : INC { $$ = strdup("inc"); track_allocation($$); }
    | DEC { $$ = strdup("dec"); track_allocation($$); }
    ;

for_step
    : expression { $$ = $1; }
    ;

for_loop
    : FOR identifier ASSIGN expression TO expression for_dir for_step DO block SEMICOLON {
        char* loop_var = $2;
        emit(loop_var, $4, NULL, NULL); // a = t1

        char* upper = $6;
        char* step = $8;
        char* start_label = newLabel();
        char* end_label = newLabel();

        emitLabel(start_label);

        char* cond = newTemp();
        if (strcmp($7, "inc") == 0)
            emit(cond, loop_var, "<=", upper);
        else
            emit(cond, loop_var, ">=", upper);

        emitCondJump(cond, end_label);
        // $10: loop body block
        char* step_temp = newTemp();
        if (strcmp($7, "inc") == 0)
            emit(step_temp, loop_var, "+", step);
        else
            emit(step_temp, loop_var, "-", step);
        emit(loop_var, step_temp, NULL, NULL);
        emitGoto(start_label);
        emitLabel(end_label);
    }


// -- IF-ELSE Statement --

if_cond
    : IF OB bool_expr CB block %prec LOWER_THAN_ELSE {
        char* after = newLabel();
        emitCondJump($3, after);
        // $5 is block
        emitLabel(after);
    }
    | IF OB bool_expr CB block ELSE block {
        char* else_label = newLabel();
        char* end_label = newLabel();
        emitCondJump($3, else_label);
        // true block: $5
        emitGoto(end_label);
        emitLabel(else_label);
        // false block: $7
        emitLabel(end_label);
    }
    ;

// -- WHILE loop --

while_loop
    : WHILE OB expression relop expression CB DO block SEMICOLON {
        char* start = newLabel();
        char* end = newLabel();

        emitLabel(start);

        // Emit fresh condition each time
        char* cond = newTemp();
        emit(cond, $3, $4, $5);

        emitCondJump(cond, end);

        // Emit loop body
        // block already emits statements
        emitGoto(start);
        emitLabel(end);
    }


// -- PRINT and SCAN --

print
    : PRINT OB STRING_LITERAL formatArgs CB SEMICOLON {
        if ($4) {
            printf("print \"%s\", %s\n", $3, $4);
        } else {
            printf("print \"%s\"\n", $3);
        }
    }
    ;

scan
    : SCAN OB STRING_LITERAL COMMA argList CB SEMICOLON {
        printf("scan \"%s\"\n", $3);
        if ($5) printf("vars: %s\n", $5); // variables list
    }
    ;

formatArgs
    : /* empty */ { $$ = NULL; }
    | COMMA argList { $$ = $2; }
    ;

argList
    : value {
        $$ = strdup($1); track_allocation($$);
    }
    | value COMMA argList {
        char* temp = malloc(strlen($1) + strlen($3) + 2);
        sprintf(temp, "%s,%s", $1, $3);
        track_allocation(temp);
        $$ = temp;
    }
    ;

value
    : identifier { $$ = $1; }
    | INTEGER_CONST { $$ = $1; }
    | CHAR_LITERAL  { $$ = $1; }
    ;

// -- VarDecl Handling --

varDecBlock
    : BEGIN_TOKEN VARDECL COLON VarDecList END_TOKEN VARDECL
    ;

VarDecList
    : VarDeclaration
    | VarDeclaration VarDecList
    ;

VarDeclaration
    : OB IDENTIFIER arrayDec COMMA type CB SEMICOLON {
        declare_variable($2);
    }
    ;

arrayDec
    : /* empty */
    | LBRACKET DECIMAL RBRACKET
    ;

type
    : INT_TYPE
    | CHAR_TYPE
    ;

// -- Final Section --

%%

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error opening file");
        return 1;
    }

    if (yyparse() == 0) {
        printf("Parsing done.\n");
    }

    cleanup_memory();
    fclose(yyin);
    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, s);
    cleanup_memory();
    exit(1);
}
