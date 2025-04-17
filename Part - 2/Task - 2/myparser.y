%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex();
extern int yyparse();
extern FILE* yyin;
extern int yylineno;
void yyerror(const char* s);

// Root of the AST
ASTNode* ast_root = NULL;

// Symbol table for variable declarations
typedef struct Symbol {
    char* name;
    char* type;
    int is_array;
    int array_size;
} Symbol;

Symbol* symbol_table = NULL;
int symbol_count = 0;

void add_symbol(const char* name, const char* type, int is_array, int array_size) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            printf("Error: Variable '%s' redeclared at line %d\n", name, yylineno);
            exit(1);
        }
    }
    const char* keywords[] = {"begin", "end", "program", "if", "else", "while", "for",
                              "print", "scan", "to", "inc", "dec", "do", "VarDecl",
                              "int", "char"};
    for (int i = 0; i < 16; i++) {
        if (strcmp(name, keywords[i]) == 0) {
            printf("Error: Keyword '%s' used as identifier at line %d\n", name, yylineno);
            exit(1);
        }
    }
    symbol_table = (Symbol*)realloc(symbol_table, (symbol_count + 1) * sizeof(Symbol));
    symbol_table[symbol_count].name = strdup(name);
    symbol_table[symbol_count].type = strdup(type);
    symbol_table[symbol_count].is_array = is_array;
    symbol_table[symbol_count].array_size = array_size;
    symbol_count++;
}

int check_variable(const char* name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) return 1;
    }
    return 0;
}
%}

%code requires {
    #include "ast.h"
}

%union {
    int int_val;
    char char_val;
    char* string;
    ASTNode* node;
}

%token <int_val> DECIMAL
%token <string> IDENTIFIER STRING_LITERAL INT_TYPE CHAR_TYPE
%token <char_val> CHAR_LITERAL
%token BEGIN_TOKEN END_TOKEN PROGRAM VARDECL IF ELSE WHILE FOR TO INC DEC DO PRINT SCAN
%token ASSIGN PLUSEQ MINUSEQ MULEQ DIVEQ MODEQ SLCOMMENT MLCOMMENT
%token COLON SEMICOLON COMMA LBRACKET RBRACKET QUOTE DQUOTE AT
%token <string> ADD SUB MUL DIV MOD EQ GT LT GE LE NE OB CB

%type <node> program comment statementBlock statement assignment expression term factor
%type <node> block if_cond while_loop for_loop print scan condition
%type <node> varDecBlock VarDecList VarDeclaration arrayDec type integerConst
%type <node> formatArgs argList value scanArgs identifierList identifierListT arrIndex
%type <string> assignment_op relop

%left EQ GT LT GE LE NE
%left ADD SUB
%left MUL DIV MOD

%%

program
    : comment BEGIN_TOKEN PROGRAM COLON varDecBlock statementBlock END_TOKEN PROGRAM comment {
        ast_root = create_node(NODE_PROGRAM);
        add_child(ast_root, $5);
        add_child(ast_root, $6);
    }
    ;

condition
    : expression relop expression {
        $$ = create_node(NODE_RELOP);
        $$->operator = $2;
        add_child($$, $1);
        add_child($$, $3);
    }
    | expression relop DECIMAL {
        $$ = create_node(NODE_RELOP);
        $$->operator = $2;
        add_child($$, $1);
        ASTNode* num = create_node(NODE_INT_CONST);
        num->data.int_value = $3;
        num->data.base = 10;
        add_child($$, num);
    }
    | expression {
        $$ = $1;
    }
    ;

comment
    : SLCOMMENT {
        $$ = create_node(NODE_COMMENT);
    }
    | MLCOMMENT {
        $$ = create_node(NODE_COMMENT);
    }
    | /* empty */ {
        $$ = NULL;
    }
    ;

statementBlock
    : /* empty */ {
        $$ = create_node(NODE_BLOCK);
    }
    | statement statementBlock {
        $$ = create_node(NODE_BLOCK);
        add_child($$, $1);
        add_child($$, $2);
    }
    ;

statement
    : assignment SEMICOLON {
        $$ = $1;
    }
    | if_cond {
        $$ = $1;
    }
    | while_loop {
        $$ = $1;
    }
    | for_loop {
        $$ = $1;
    }
    | print {
        $$ = $1;
    }
    | scan {
        $$ = $1;
    }
    | block {
        $$ = $1;
    }
    ;

assignment
    : IDENTIFIER assignment_op expression {
        if (!check_variable($1)) {
            printf("Error: Undeclared variable '%s' at line %d\n", $1, yylineno);
            exit(1);
        }
        $$ = create_node(NODE_ASSIGN);
        $$->operator = $2;
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = $1;
        add_child($$, id);
        add_child($$, $3);
    }
    ;

expression
    : term { $$ = $1; }
    | expression ADD expression {
        $$ = create_node(NODE_BINARY_OP);
        $$->operator = $2;
        add_child($$, $1);
        add_child($$, $3);
    }
    | expression SUB expression {
        $$ = create_node(NODE_BINARY_OP);
        $$->operator = $2;
        add_child($$, $1);
        add_child($$, $3);
    }
    | expression MUL expression {
        $$ = create_node(NODE_BINARY_OP);
        $$->operator = $2;
        add_child($$, $1);
        add_child($$, $3);
    }
    | expression DIV expression {
        $$ = create_node(NODE_BINARY_OP);
        $$->operator = $2;
        add_child($$, $1);
        add_child($$, $3);
    }
    | expression MOD expression {
        $$ = create_node(NODE_BINARY_OP);
        $$->operator = $2;
        add_child($$, $1);
        add_child($$, $3);
    }
    ;

term
    : factor { $$ = $1; }
    ;

factor
    : OB expression CB { $$ = $2; }
    | IDENTIFIER arrIndex {
        if (!check_variable($1)) {
            printf("Error: Undeclared variable '%s' at line %d\n", $1, yylineno);
            exit(1);
        }
        $$ = create_node(NODE_IDENTIFIER);
        $$->data.string_value = $1;
        if ($2) add_child($$, $2);
    }
    | integerConst { $$ = $1; }
    | CHAR_LITERAL {
        $$ = create_node(NODE_CHAR_CONST);
        $$->data.char_value = $1;
    }
    ;

block
    : BEGIN_TOKEN statementBlock END_TOKEN { $$ = $2; }
    ;

if_cond
    : IF OB condition CB block {
        $$ = create_node(NODE_IF);
        add_child($$, $3);
        add_child($$, $5);
    }
    | IF OB condition CB block ELSE block {
        $$ = create_node(NODE_IF);
        add_child($$, $3);
        add_child($$, $5);
        add_child($$, $7);
    }
    ;

while_loop
    : WHILE OB condition CB DO block SEMICOLON {
        $$ = create_node(NODE_WHILE);
        add_child($$, $3);
        add_child($$, $6);
    }
    ;

for_loop
    : FOR IDENTIFIER ASSIGN expression TO expression INC expression DO block SEMICOLON {
        if (!check_variable($2)) {
            printf("Error: Undeclared variable '%s' at line %d\n", $2, yylineno);
            exit(1);
        }
        $$ = create_node(NODE_FOR);
        ASTNode* assign = create_node(NODE_ASSIGN);
        assign->operator = strdup(":=");
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = $2;
        add_child(assign, id);
        add_child(assign, $4);
        add_child($$, assign);
        add_child($$, $6);
        ASTNode* inc = create_node(NODE_BINARY_OP);
        inc->operator = strdup("inc");
        add_child(inc, $8);
        add_child($$, inc);
        add_child($$, $10);
    }
    | FOR IDENTIFIER ASSIGN expression TO expression DEC expression DO block SEMICOLON {
        if (!check_variable($2)) {
            printf("Error: Undeclared variable '%s' at line %d\n", $2, yylineno);
            exit(1);
        }
        $$ = create_node(NODE_FOR);
        ASTNode* assign = create_node(NODE_ASSIGN);
        assign->operator = strdup(":=");
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = $2;
        add_child(assign, id);
        add_child(assign, $4);
        add_child($$, assign);
        ASTNode* to = $6; // End expression
        ASTNode* dec = create_node(NODE_BINARY_OP);
        dec->operator = strdup("dec");
        add_child(dec, $8);
        add_child($$, to);
        add_child($$, dec);
        add_child($$, $10);
    }
    ;

print
    : PRINT OB STRING_LITERAL formatArgs CB SEMICOLON {
        $$ = create_node(NODE_PRINT);
        ASTNode* str = create_node(NODE_STRING_CONST);
        str->data.string_value = $3;
        add_child($$, str);
        add_child($$, $4);
    }
    ;

formatArgs
    : /* empty */ { $$ = NULL; }
    | COMMA argList { $$ = $2; }
    ;

argList
    : value {
        $$ = create_node(NODE_BLOCK);
        add_child($$, $1);
    }
    | value COMMA argList {
        $$ = $3;
        add_child($$, $1);
    }
    ;

value
    : IDENTIFIER {
        if (!check_variable($1)) {
            printf("Error: Undeclared variable '%s' at line %d\n", $1, yylineno);
            exit(1);
        }
        $$ = create_node(NODE_IDENTIFIER);
        $$->data.string_value = $1;
    }
    | integerConst { $$ = $1; }
    ;

scan
    : SCAN OB scanArgs CB SEMICOLON {
        $$ = create_node(NODE_SCAN);
        add_child($$, $3);
    }
    ;

scanArgs
    : identifierList { $$ = $1; }
    | STRING_LITERAL COMMA identifierList {
        $$ = create_node(NODE_BLOCK);
        ASTNode* str = create_node(NODE_STRING_CONST);
        str->data.string_value = $1;
        add_child($$, str);
        add_child($$, $3);
    }
    ;

identifierList
    : IDENTIFIER identifierListT {
        $$ = create_node(NODE_BLOCK);
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = $1;
        add_child($$, id);
        add_child($$, $2);
    }
    ;

identifierListT
    : /* empty */ { $$ = NULL; }
    | COMMA IDENTIFIER identifierListT {
        $$ = create_node(NODE_BLOCK);
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = $2;
        add_child($$, id);
        add_child($$, $3);
    }
    ;

assignment_op
    : ASSIGN { $$ = strdup(":="); }
    | PLUSEQ { $$ = strdup("+="); }
    | MINUSEQ { $$ = strdup("-="); }
    | MULEQ { $$ = strdup("*="); }
    | DIVEQ { $$ = strdup("/="); }
    | MODEQ { $$ = strdup("%="); }
    ;

relop
    : EQ { $$ = $1; }
    | GE { $$ = $1; }
    | GT { $$ = $1; }
    | LE { $$ = $1; }
    | LT { $$ = $1; }
    | NE { $$ = $1; }
    ;

arrIndex
    : /* empty */ { $$ = NULL; }
    | LBRACKET expression RBRACKET { $$ = $2; }
    ;

varDecBlock
    : BEGIN_TOKEN VARDECL COLON comment VarDecList comment END_TOKEN VARDECL {
        $$ = create_node(NODE_BLOCK);
        add_child($$, $5);
    }
    ;

VarDecList
    : VarDeclaration {
        $$ = create_node(NODE_BLOCK);
        add_child($$, $1);
    }
    | VarDecList VarDeclaration {
        $$ = $1;
        add_child($$, $2);
    }
    ;

VarDeclaration
    : OB IDENTIFIER arrayDec COMMA type CB SEMICOLON {
        add_symbol($2, $5->data.string_value, $3 != NULL, $3 ? $3->data.int_value : 0);
        $$ = create_node(NODE_VARDECL);
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = $2;
        add_child($$, id);
        add_child($$, $5);
        if ($3) add_child($$, $3);
    }
    ;

arrayDec
    : /* empty */ { $$ = NULL; }
    | LBRACKET DECIMAL RBRACKET {
        $$ = create_node(NODE_INT_CONST);
        $$->data.int_value = $2;
        $$->data.base = 10;
    }
    ;

type
    : INT_TYPE {
        $$ = create_node(NODE_TYPE);
        $$->data.string_value = $1;
    }
    | CHAR_TYPE {
        $$ = create_node(NODE_TYPE);
        $$->data.string_value = $1;
    }
    ;

integerConst
    : OB DECIMAL COMMA DECIMAL CB {
        if ($4 != 2 && $4 != 8 && $4 != 10) {
            printf("Invalid base %d at line %d\n", $4, yylineno);
            exit(1);
        }
        char num_str[32];
        sprintf(num_str, "%d", $2);
        if (!is_valid_integer(num_str, $4)) {
            printf("Invalid integer %s for base %d at line %d\n", num_str, $4, yylineno);
            exit(1);
        }
        $$ = create_node(NODE_INT_CONST);
        $$->data.int_value = $2;
        $$->data.base = $4;
    }
    ;

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
        printf("Successfully parsed !!!\n");
        if (ast_root) {
            print_ast(ast_root, 0);
            printf("\n");
            // free_ast(ast_root);
        }
    }

    fclose(yyin);
    for (int i = 0; i < symbol_count; i++) {
        free(symbol_table[i].name);
        free(symbol_table[i].type);
    }
    free(symbol_table);
    return 0;
}

void yyerror(const char *s) {
    printf("Syntax error at line %d: %s\n", yylineno, s);
    exit(1);
}