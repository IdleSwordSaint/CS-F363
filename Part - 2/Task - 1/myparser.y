%{
#include<stdio.h>
#include<stdlib.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;
extern int yylineno;
void yyerror(const char* s);
%}

%token DECIMAL IDENTIFIER BEGIN_TOKEN END_TOKEN PROGRAM VARDECL IF ELSE WHILE FOR TO INC DEC DO PRINT SCAN INT_TYPE CHAR_TYPE ASSIGN PLUSEQ MINUSEQ MULEQ DIVEQ MODEQ SLCOMMENT MLCOMMENT COLON SEMICOLON COMMA LBRACKET RBRACKET QUOTE DQUOTE AT STRING_LITERAL CHAR_LITERAL OB CB MUL ADD SUB DIV MOD EQ GT LT GE LE NE

/* Define precedence and associativity to resolve conflicts */
%left EQ GT LT GE LE NE
%left ADD SUB
%left MUL DIV MOD

%%

program 
        : comment BEGIN_TOKEN PROGRAM COLON varDecBlock statementBlock END_TOKEN PROGRAM comment {}
        ;

condition 
        : expression relop expression
        | expression relop DECIMAL
        | expression
        ;

comment 
        : SLCOMMENT
        | MLCOMMENT
        | /* empty */
        ;

statementBlock 
        : /* empty */
        | comment statement statementBlock
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
        : IDENTIFIER assignment_op expression
        ;

expression 
        : term
        | expression ADD expression
        | expression SUB expression
        ;

term 
        : factor
        | term MUL factor
        | term DIV factor
        | term MOD factor
        ;

factor 
        : OB expression CB
        | IDENTIFIER arrIndex
        | integerConst
        | CHAR_LITERAL
        ;

block 
        : BEGIN_TOKEN statementBlock END_TOKEN
        ;

if_cond 
        : IF OB condition CB block
        | IF OB condition CB block ELSE block
        ;

while_loop 
        : WHILE OB condition CB DO block SEMICOLON
        ;

for_loop 
        : FOR IDENTIFIER ASSIGN expression TO expression INC expression DO block SEMICOLON
        | FOR IDENTIFIER ASSIGN expression TO expression DEC expression DO block SEMICOLON
        ;

print 
        : PRINT OB STRING_LITERAL formatArgs CB SEMICOLON
        ;

formatArgs 
        : /* empty */
        | COMMA argList
        ;

argList
        : value
        | value COMMA argList
        ;

value
        : IDENTIFIER
        | integerConst
        ;

scan
        : SCAN OB scanArgs CB SEMICOLON
        ;

scanArgs
        : identifierList
        | STRING_LITERAL COMMA identifierList
        ;

identifierList
        : IDENTIFIER identifierListT
        ;

identifierListT
        : /* empty */
        | COMMA IDENTIFIER identifierListT
        ;

assignment_op
        : ASSIGN
        | PLUSEQ
        | MINUSEQ
        | MULEQ
        | DIVEQ
        | MODEQ
        ;

relop
        : EQ
        | GE
        | GT
        | LE
        | LT
        | NE
        ;

arrIndex
        : /* empty */
        | LBRACKET expression RBRACKET
        ;

varDecBlock
        : BEGIN_TOKEN VARDECL COLON comment VarDecList comment END_TOKEN VARDECL
        ;

VarDecList
        : VarDeclaration
        | VarDeclaration VarDecList
        ;

VarDeclaration
        : OB IDENTIFIER arrayDec COMMA type CB SEMICOLON
        ;

arrayDec
        : /* empty */
        | LBRACKET DECIMAL RBRACKET
        ;

type
        : INT_TYPE
        | CHAR_TYPE
        ;

integerConst
        : OB DECIMAL COMMA DECIMAL CB
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
    }

    fclose(yyin);
    return 0;
}

void yyerror(const char *s) {
    printf("Syntax error !!!\n");
    exit(1);
}
