%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ast.h"

extern int yylex();
extern int yyparse();
extern FILE* yyin;
extern int yylineno;
void yyerror(const char* s);

// Root of the AST
ASTNode* ast_root = NULL;

// Debug flag
int debug_simulation = 0;

// ---- Symbol table for runtime simulation ----
typedef enum {
    TYPE_INT,
    TYPE_CHAR,
    TYPE_UNKNOWN
} VarType;

typedef struct {
    char* name;
    VarType type;
    union {
        int int_val;
        char char_val;
    } value;
    int initialized;  // Flag to track if variable has been initialized
    int is_array;     // Flag to indicate if variable is an array
    int array_size;   // Size of array (if is_array is true)
} Symbol;

#define MAX_SYMBOLS 1000
Symbol symbol_table[MAX_SYMBOLS];
int symbol_count = 0;

// Memory management
char** allocated_memory = NULL;
int allocated_count = 0;
int allocated_capacity = 0;
int cleanup_started = 0;

void track_allocation(char* ptr) {
    if (!ptr || cleanup_started) return; // Don't track NULL pointers or during cleanup
    
    // First check if this pointer is already being tracked
    for (int i = 0; i < allocated_count; i++) {
        if (allocated_memory[i] == ptr) {
            return; // Already tracked, don't add it again
        }
    }
    
    // Grow array if needed with some extra capacity
    if (allocated_count >= allocated_capacity) {
        allocated_capacity = allocated_capacity ? allocated_capacity * 2 : 32;
        char** new_memory = realloc(allocated_memory, sizeof(char*) * allocated_capacity);
        if (!new_memory) {
            fprintf(stderr, "Memory allocation failed during track_allocation\n");
            exit(1);
        }
        allocated_memory = new_memory;
    }
    
    allocated_memory[allocated_count++] = ptr;
}

void cleanup_memory() {
    if (!allocated_memory || cleanup_started) return;
    
    cleanup_started = 1; // Set flag to prevent reentry
    
    for (int i = 0; i < allocated_count; i++) {
        if (allocated_memory[i]) {
            free(allocated_memory[i]);
            allocated_memory[i] = NULL;
        }
    }
    
    free(allocated_memory);
    allocated_memory = NULL;
    allocated_count = 0;
    allocated_capacity = 0;
}

// Symbol table functions
int find_symbol(const char* name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0)
            return i;
    }
    return -1;
}

void declare_variable(const char* name, VarType type, int is_array, int array_size) {
    int idx = find_symbol(name);
    if (idx >= 0) {
        fprintf(stderr, "Semantic error at line %d: Variable '%s' redeclared.\n", yylineno, name);
        exit(1);
    }

    if (symbol_count < MAX_SYMBOLS) {
        symbol_table[symbol_count].name = strdup(name);
        symbol_table[symbol_count].type = type;
        symbol_table[symbol_count].initialized = 0;
        symbol_table[symbol_count].is_array = is_array;
        symbol_table[symbol_count].array_size = array_size;
        track_allocation(symbol_table[symbol_count].name);
        symbol_count++;
    } else {
        fprintf(stderr, "Error: Symbol table full\n");
        exit(1);
    }
}

// Check array bounds
void check_array_bounds(const char* name, int index) {
    int symbol_idx = find_symbol(name);
    if (symbol_idx >= 0 && symbol_table[symbol_idx].is_array) {
        if (index < 0 || index >= symbol_table[symbol_idx].array_size) {
            fprintf(stderr, "Runtime error at line %d: Array index %d out of bounds for array '%s' [size %d]\n",
                    yylineno, index, name, symbol_table[symbol_idx].array_size);
            exit(1);
        }
    } else if (symbol_idx >= 0 && !symbol_table[symbol_idx].is_array) {
        fprintf(stderr, "Runtime error at line %d: '%s' is not an array\n", yylineno, name);
        exit(1);
    }
}

// Get value of variable or literal
int get_int_value(const ASTNode* node) {
    switch (node->type) {
        case NODE_INT_CONST:
            return node->data.int_const.int_value;
        
        case NODE_CHAR_CONST:
            return node->data.char_value;
        
        case NODE_IDENTIFIER: {
            char* name = node->data.string_value;
            int index = -1;
            
            // Handle array access if there's a child node (array index)
            if (node->data.compound.num_children > 0) {
                ASTNode* idx_node = node->data.compound.children[0];
                index = get_int_value(idx_node);
                
                // Extract base name (without brackets)
                char base_name[256];
                strncpy(base_name, name, sizeof(base_name) - 1);
                char* bracket_pos = strchr(base_name, '[');
                if (bracket_pos) *bracket_pos = '\0';
                
                check_array_bounds(base_name, index);
                
                int symbol_idx = find_symbol(base_name);
                if (symbol_idx >= 0) {
                    if (!symbol_table[symbol_idx].initialized) {
                        fprintf(stderr, "Semantic error at line %d: Variable '%s' used before initialization.\n", 
                                yylineno, base_name);
                        exit(1);
                    }
                    // For arrays, just use the index as an offset to the base value for simplicity
                    return symbol_table[symbol_idx].value.int_val + index;
                }
            } else {
                // Regular variable access
                int idx = find_symbol(name);
                if (idx >= 0) {
                    if (!symbol_table[idx].initialized) {
                        fprintf(stderr, "Semantic error at line %d: Variable '%s' used before initialization.\n", 
                                yylineno, name);
                        exit(1);
                    }
                    return symbol_table[idx].value.int_val;
                }
            }
            fprintf(stderr, "Semantic error: Undefined variable '%s'\n", name);
            exit(1);
        }
        
        default:
            fprintf(stderr, "Internal error: Invalid node type for value evaluation\n");
            exit(1);
    }
    return 0;
}

// Set value of a variable
void set_int_value(const char* name, int value) {
    char base_name[256];
    strncpy(base_name, name, sizeof(base_name) - 1);
    
    // Handle array assignment
    char* bracket_pos = strchr(base_name, '[');
    if (bracket_pos) {
        *bracket_pos = '\0';
        int index;
        sscanf(bracket_pos + 1, "%d", &index);
        
        check_array_bounds(base_name, index);
        
        int idx = find_symbol(base_name);
        if (idx >= 0) {
            symbol_table[idx].value.int_val = value - index; // Adjust the base value
            symbol_table[idx].initialized = 1;
            symbol_table[idx].type = TYPE_INT;
            return;
        }
    } else {
        // Regular variable assignment
        int idx = find_symbol(name);
        if (idx >= 0) {
            symbol_table[idx].value.int_val = value;
            symbol_table[idx].initialized = 1;
            symbol_table[idx].type = TYPE_INT;
            return;
        }
    }
    
    fprintf(stderr, "Semantic error: Undefined variable '%s'\n", name);
    exit(1);
}

// Evaluate binary operations
int perform_binary_op(int left, const char* op, int right) {
    if (strcmp(op, "+") == 0) return left + right;
    if (strcmp(op, "-") == 0) return left - right;
    if (strcmp(op, "*") == 0) return left * right;
    if (strcmp(op, "/") == 0) {
        if (right == 0) {
            fprintf(stderr, "Semantic error: Division by zero\n");
            exit(1);
        }
        return left / right;
    }
    if (strcmp(op, "%") == 0) {
        if (right == 0) {
            fprintf(stderr, "Semantic error: Modulo by zero\n");
            exit(1);
        }
        return left % right;
    }
    if (strcmp(op, "=") == 0) return left == right;
    if (strcmp(op, "!=") == 0) return left != right;
    if (strcmp(op, ">") == 0) return left > right;
    if (strcmp(op, ">=") == 0) return left >= right;
    if (strcmp(op, "<") == 0) return left < right;
    if (strcmp(op, "<=") == 0) return left <= right;
    if (strcmp(op, "and") == 0) return left && right;
    if (strcmp(op, "or") == 0) return left || right;
    
    fprintf(stderr, "Semantic error: Unknown operator '%s'\n", op);
    exit(1);
}

// Evaluate expressions
int evaluate_expression(const ASTNode* node) {
    if (!node) {
        fprintf(stderr, "Internal error: NULL node in evaluation\n");
        exit(1);
    }
    
    switch (node->type) {
        case NODE_INT_CONST:
        case NODE_CHAR_CONST:
        case NODE_IDENTIFIER:
            return get_int_value(node);
        
        case NODE_BINARY_OP: {
            int left = evaluate_expression(node->data.compound.children[0]);
            int right = evaluate_expression(node->data.compound.children[1]);
            return perform_binary_op(left, node->operator, right);
        }
        
        case NODE_RELOP: {
            int left = evaluate_expression(node->data.compound.children[0]);
            int right = evaluate_expression(node->data.compound.children[1]);
            return perform_binary_op(left, node->operator, right);
        }
        
        default:
            fprintf(stderr, "Internal error: Unsupported node type %d in expression evaluation\n", node->type);
            exit(1);
    }
    return 0;
}

// Forward declarations for functions
void execute_node(const ASTNode* node);
void execute_assignment(const ASTNode* node);
void execute_if(const ASTNode* node);
void execute_while(const ASTNode* node);
void execute_for(const ASTNode* node);
void execute_print(const ASTNode* node);
void execute_scan(const ASTNode* node);
void execute_block(const ASTNode* node);

// Execute a single statement or block
void execute_node(const ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_PROGRAM:
        case NODE_BLOCK:
            execute_block(node);
            break;
        
        case NODE_ASSIGN:
            execute_assignment(node);
            break;
        
        case NODE_IF:
            execute_if(node);
            break;
        
        case NODE_WHILE:
            execute_while(node);
            break;
        
        case NODE_FOR:
            execute_for(node);
            break;
        
        case NODE_PRINT:
            execute_print(node);
            break;
        
        case NODE_SCAN:
            execute_scan(node);
            break;
        
        default:
            if (node->type != NODE_COMMENT && node->type != NODE_VARDECL) {
                fprintf(stderr, "Internal error: Unsupported node type %d in execution\n", node->type);
            }
            break;
    }
}

// Execute an assignment statement
void execute_assignment(const ASTNode* node) {
    if (node->data.compound.num_children < 2) {
        fprintf(stderr, "Internal error: Assignment node missing children\n");
        return;
    }
    
    ASTNode* lhs = node->data.compound.children[0];
    ASTNode* rhs = node->data.compound.children[1];
    
    if (lhs->type != NODE_IDENTIFIER) {
        fprintf(stderr, "Internal error: Assignment LHS is not an identifier\n");
        return;
    }
    
    char* var_name = lhs->data.string_value;
    int value = evaluate_expression(rhs);
    
    // Handle compound assignments
    if (node->operator && strcmp(node->operator, ":=") != 0) {
        int idx = find_symbol(var_name);
        if (idx >= 0) {
            if (!symbol_table[idx].initialized) {
                fprintf(stderr, "Semantic error: Variable '%s' used before initialization.\n", var_name);
                exit(1);
            }
            
            int current = symbol_table[idx].value.int_val;
            
            if (strcmp(node->operator, "+=") == 0) {
                value = current + value;
            } else if (strcmp(node->operator, "-=") == 0) {
                value = current - value;
            } else if (strcmp(node->operator, "*=") == 0) {
                value = current * value;
            } else if (strcmp(node->operator, "/=") == 0) {
                if (value == 0) {
                    fprintf(stderr, "Semantic error: Division by zero\n");
                    exit(1);
                }
                value = current / value;
            } else if (strcmp(node->operator, "%=") == 0) {
                if (value == 0) {
                    fprintf(stderr, "Semantic error: Modulo by zero\n");
                    exit(1);
                }
                value = current % value;
            }
        }
    }
    
    set_int_value(var_name, value);
}

// Execute an if statement
void execute_if(const ASTNode* node) {
    if (node->data.compound.num_children < 2) {
        fprintf(stderr, "Internal error: If node missing children\n");
        return;
    }
    
    ASTNode* condition = node->data.compound.children[0];
    ASTNode* true_block = node->data.compound.children[1];
    ASTNode* false_block = NULL;
    
    if (node->data.compound.num_children > 2) {
        false_block = node->data.compound.children[2];
    }
    
    int cond_value = evaluate_expression(condition);
    
    if (cond_value) {
        execute_node(true_block);
    } else if (false_block) {
        execute_node(false_block);
    }
}

// Execute a while loop
void execute_while(const ASTNode* node) {
    if (node->data.compound.num_children < 2) {
        fprintf(stderr, "Internal error: While node missing children\n");
        return;
    }
    
    ASTNode* condition = node->data.compound.children[0];
    ASTNode* body = node->data.compound.children[1];
    
    int max_iterations = 10000; // Safety limit
    int iteration = 0;
    
    while (evaluate_expression(condition)) {
        execute_node(body);
        
        iteration++;
        if (iteration >= max_iterations) {
            fprintf(stderr, "Warning: Maximum iteration count reached, possible infinite loop\n");
            break;
        }
    }
}

// Execute a for loop
void execute_for(const ASTNode* node) {
    if (node->data.compound.num_children < 4) {
        fprintf(stderr, "Internal error: For node missing children\n");
        return;
    }
    
    ASTNode* init_assign = node->data.compound.children[0];
    ASTNode* limit_expr = node->data.compound.children[1];
    ASTNode* step_node = node->data.compound.children[2];
    ASTNode* body = node->data.compound.children[3];
    
    // Execute initialization assignment
    execute_assignment(init_assign);
    
    // Get loop variable from initial assignment
    char* loop_var = init_assign->data.compound.children[0]->data.string_value;
    
    // Get step value and direction
    int step_value = evaluate_expression(step_node->data.compound.children[0]);
    int is_inc = strcmp(step_node->operator, "inc") == 0;
    
    // Calculate the limit ONCE before entering the loop, not on every iteration
    int limit = evaluate_expression(limit_expr);
    
    int max_iterations = 10000; // Safety limit
    int iteration = 0;
    
    while (1) {
        // Get current loop variable value
        int current = get_int_value(init_assign->data.compound.children[0]);
        
        if (debug_simulation) {
            printf("DEBUG: For loop - %s=%d, limit=%d, step=%d, is_inc=%d\n", 
                   loop_var, current, limit, step_value, is_inc);
        }
        
        // Fix the termination condition for incrementing and decrementing loops
        if ((is_inc && current > limit) || (!is_inc && current < limit)) {
            break;
        }
        
        // Execute the loop body
        execute_node(body);
        
        // Update the loop variable
        if (is_inc) {
            set_int_value(loop_var, current + step_value);
        } else {
            set_int_value(loop_var, current - step_value);
        }
        
        iteration++;
        if (iteration >= max_iterations) {
            fprintf(stderr, "Warning: Maximum iteration count reached, possible infinite loop\n");
            break;
        }
    }
}

// Execute a print statement
void execute_print(const ASTNode* node) {
    if (node->data.compound.num_children == 0) {
        fprintf(stderr, "Internal error: Print node missing children\n");
        return;
    }
    
    ASTNode* string_node = node->data.compound.children[0];
    if (string_node->type != NODE_STRING_CONST) {
        fprintf(stderr, "Internal error: Print first arg is not a string\n");
        return;
    }
    
    char* format = string_node->data.string_value;
    
    // If there are arguments, format the string
    if (node->data.compound.num_children > 1) {
        ASTNode* args_block = node->data.compound.children[1];
        
        int arg_count = args_block ? args_block->data.compound.num_children : 0;
        int values[100]; // Support up to 100 arguments
        
        // Collect argument values
        for (int i = 0; i < arg_count && i < 100; i++) {
            ASTNode* arg = args_block->data.compound.children[i];
            values[i] = evaluate_expression(arg);
        }
        
        // Format the string with arguments
        char* fmt_copy = strdup(format);
        char output[1024] = {0};
        char* out_ptr = output;
        char* fmt_ptr = fmt_copy;
        int val_idx = 0;
        
        while (*fmt_ptr) {
            if (*fmt_ptr == '%' && *(fmt_ptr + 1) == 'd') {
                if (val_idx < arg_count) {
                    out_ptr += sprintf(out_ptr, "%d", values[val_idx++]);
                } else {
                    strcpy(out_ptr, "[ERROR:missing arg]");
                    out_ptr += strlen("[ERROR:missing arg]");
                }
                fmt_ptr += 2; // Skip %d
            } else {
                *out_ptr++ = *fmt_ptr++;
            }
        }
        *out_ptr = '\0';
        
        printf("%s\n", output);
        free(fmt_copy);
    } else {
        // Just print the raw string
        printf("%s\n", format);
    }
}

// Execute a scan statement
void execute_scan(const ASTNode* node) {
    if (node->data.compound.num_children == 0) {
        fprintf(stderr, "Internal error: Scan node missing children\n");
        return;
    }
    
    ASTNode* args_block = node->data.compound.children[0];
    
    // Check if there is a format string
    ASTNode* prompt = NULL;
    ASTNode* vars_block = args_block;
    
    if (args_block->data.compound.num_children > 0 && 
        args_block->data.compound.children[0]->type == NODE_STRING_CONST) {
        prompt = args_block->data.compound.children[0];
        // If first child is a string, use the second child as vars_block
        if (args_block->data.compound.num_children > 1) {
            vars_block = args_block->data.compound.children[1];
        } else {
            vars_block = NULL;
        }
    }
    
    // Print the prompt if present
    if (prompt) {
        printf("%s", prompt->data.string_value);
    }
    
    // Process each identifier
    if (vars_block) {
        for (int i = 0; i < vars_block->data.compound.num_children; i++) {
            ASTNode* id_node = vars_block->data.compound.children[i];
            if (id_node->type == NODE_IDENTIFIER) {
                int value;
                char* var_name = id_node->data.string_value;
                
                printf(" %s: ", var_name);
                if (scanf("%d", &value) != 1) {
                    fprintf(stderr, "Error reading input for variable %s\n", var_name);
                    value = 0; // Default value on error
                }
                
                set_int_value(var_name, value);
            }
        }
    }
}

// Execute a block of statements
void execute_block(const ASTNode* node) {
    for (int i = 0; i < node->data.compound.num_children; i++) {
        execute_node(node->data.compound.children[i]);
    }
}

// Print symbol table
void print_symbol_table() {
    printf("\n=== Symbol Table ===\n");
    printf("%-15s %-10s %-15s %-15s %-10s\n", "Name", "Type", "Value", "Initialized", "IsArray");
    printf("-------------------------------------------------------------\n");
    
    for (int i = 0; i < symbol_count; i++) {
        char value_str[20];
        
        if (symbol_table[i].initialized) {
            if (symbol_table[i].type == TYPE_INT) {
                sprintf(value_str, "%d", symbol_table[i].value.int_val);
            } else if (symbol_table[i].type == TYPE_CHAR) {
                if (symbol_table[i].value.char_val == '\n') {
                    strcpy(value_str, "'\\n'");
                } else if (symbol_table[i].value.char_val == '\t') {
                    strcpy(value_str, "'\\t'");
                } else {
                    sprintf(value_str, "'%c'", symbol_table[i].value.char_val);
                }
            } else {
                strcpy(value_str, "unknown");
            }
        } else {
            strcpy(value_str, "undefined");
        }
        
        printf("%-15s %-10s %-15s %-15s %-10s\n", 
               symbol_table[i].name, 
               symbol_table[i].type == TYPE_INT ? "int" : 
               (symbol_table[i].type == TYPE_CHAR ? "char" : "unknown"),
               value_str,
               symbol_table[i].initialized ? "yes" : "no",
               symbol_table[i].is_array ? "yes" : "no");
    }
}

%}

%code requires {
    #include "ast.h"  // Ensures ASTNode is known in parser.tab.h
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
        num->data.int_const.int_value = $3;
        num->data.int_const.base = 10;
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
        int idx = find_symbol($1);
        if (idx < 0) {
            fprintf(stderr, "Error: Undeclared variable '%s' at line %d\n", $1, yylineno);
            exit(1);
        }
        $$ = create_node(NODE_ASSIGN);
        $$->operator = $2;
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = strdup($1);
        track_allocation(id->data.string_value);
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
        int idx = find_symbol($1);
        if (idx < 0) {
            fprintf(stderr, "Error: Undeclared variable '%s' at line %d\n", $1, yylineno);
            exit(1);
        }
        $$ = create_node(NODE_IDENTIFIER);
        $$->data.string_value = strdup($1);
        track_allocation($$->data.string_value);
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
    : IF OB condition CB block SEMICOLON {
        $$ = create_node(NODE_IF);
        add_child($$, $3);
        add_child($$, $5);
    }
    | IF OB condition CB block ELSE block SEMICOLON {
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
        int idx = find_symbol($2);
        if (idx < 0) {
            fprintf(stderr, "Error: Undeclared variable '%s' at line %d\n", $2, yylineno);
            exit(1);
        }
        $$ = create_node(NODE_FOR);
        ASTNode* assign = create_node(NODE_ASSIGN);
        assign->operator = strdup(":=");
        track_allocation(assign->operator);
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = strdup($2);
        track_allocation(id->data.string_value);
        add_child(assign, id);
        add_child(assign, $4);
        add_child($$, assign);
        add_child($$, $6);
        ASTNode* inc = create_node(NODE_BINARY_OP);
        inc->operator = strdup("inc");
        track_allocation(inc->operator);
        add_child(inc, $8);
        add_child($$, inc);
        add_child($$, $10);
    }
    | FOR IDENTIFIER ASSIGN expression TO expression DEC expression DO block SEMICOLON {
        int idx = find_symbol($2);
        if (idx < 0) {
            fprintf(stderr, "Error: Undeclared variable '%s' at line %d\n", $2, yylineno);
            exit(1);
        }
        $$ = create_node(NODE_FOR);
        ASTNode* assign = create_node(NODE_ASSIGN);
        assign->operator = strdup(":=");
        track_allocation(assign->operator);
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = strdup($2);
        track_allocation(id->data.string_value);
        add_child(assign, id);
        add_child(assign, $4);
        add_child($$, assign);
        add_child($$, $6);
        ASTNode* dec = create_node(NODE_BINARY_OP);
        dec->operator = strdup("dec");
        track_allocation(dec->operator);
        add_child(dec, $8);
        add_child($$, dec);
        add_child($$, $10);
    }
    ;

print
    : PRINT OB STRING_LITERAL formatArgs CB SEMICOLON {
        $$ = create_node(NODE_PRINT);
        ASTNode* str = create_node(NODE_STRING_CONST);
        str->data.string_value = strdup($3);
        track_allocation(str->data.string_value);
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
        int idx = find_symbol($1);
        if (idx < 0) {
            fprintf(stderr, "Error: Undeclared variable '%s' at line %d\n", $1, yylineno);
            exit(1);
        }
        $$ = create_node(NODE_IDENTIFIER);
        $$->data.string_value = strdup($1);
        track_allocation($$->data.string_value);
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
        str->data.string_value = strdup($1);
        track_allocation(str->data.string_value);
        add_child($$, str);
        add_child($$, $3);
    }
    ;

identifierList
    : IDENTIFIER identifierListT {
        int idx = find_symbol($1);
        if (idx < 0) {
            fprintf(stderr, "Error: Undeclared variable '%s' at line %d\n", $1, yylineno);
            exit(1);
        }
        $$ = create_node(NODE_BLOCK);
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = strdup($1);
        track_allocation(id->data.string_value);
        add_child($$, id);
        add_child($$, $2);
    }
    ;

identifierListT
    : /* empty */ { $$ = NULL; }
    | COMMA IDENTIFIER identifierListT {
        int idx = find_symbol($2);
        if (idx < 0) {
            fprintf(stderr, "Error: Undeclared variable '%s' at line %d\n", $2, yylineno);
            exit(1);
        }
        $$ = create_node(NODE_BLOCK);
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = strdup($2);
        track_allocation(id->data.string_value);
        add_child($$, id);
        add_child($$, $3);
    }
    ;

assignment_op
    : ASSIGN { $$ = strdup(":="); track_allocation($$); }
    | PLUSEQ { $$ = strdup("+="); track_allocation($$); }
    | MINUSEQ { $$ = strdup("-="); track_allocation($$); }
    | MULEQ { $$ = strdup("*="); track_allocation($$); }
    | DIVEQ { $$ = strdup("/="); track_allocation($$); }
    | MODEQ { $$ = strdup("%="); track_allocation($$); }
    ;

relop
    : EQ { $$ = strdup("="); track_allocation($$); }
    | GE { $$ = strdup(">="); track_allocation($$); }
    | GT { $$ = strdup(">"); track_allocation($$); }
    | LE { $$ = strdup("<="); track_allocation($$); }
    | LT { $$ = strdup("<"); track_allocation($$); }
    | NE { $$ = strdup("!="); track_allocation($$); }
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
        VarType var_type;
        if (strcmp($5->data.string_value, "int") == 0) {
            var_type = TYPE_INT;
        } else if (strcmp($5->data.string_value, "char") == 0) {
            var_type = TYPE_CHAR;
        } else {
            var_type = TYPE_UNKNOWN;
        }
        
        int array_size = $3 ? $3->data.int_const.int_value : 0;
        declare_variable($2, var_type, $3 != NULL, array_size);
        
        $$ = create_node(NODE_VARDECL);
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = strdup($2);
        track_allocation(id->data.string_value);
        add_child($$, id);
        add_child($$, $5);
        if ($3) add_child($$, $3);
    }
    ;

arrayDec
    : /* empty */ { $$ = NULL; }
    | LBRACKET DECIMAL RBRACKET {
        $$ = create_node(NODE_INT_CONST);
        $$->data.int_const.int_value = $2;
        $$->data.int_const.base = 10;
    }
    ;

type
    : INT_TYPE {
        $$ = create_node(NODE_TYPE);
        $$->data.string_value = strdup("int");
        track_allocation($$->data.string_value);
    }
    | CHAR_TYPE {
        $$ = create_node(NODE_TYPE);
        $$->data.string_value = strdup("char");
        track_allocation($$->data.string_value);
    }
     ;

// Update the integerConst rule to properly handle base conversions
integerConst
    : OB DECIMAL COMMA DECIMAL CB {
        if ($4 != 2 && $4 != 8 && $4 != 10) {
            printf("Invalid base %d at line %d\n", $4, yylineno);
            exit(1);
        }
        
        char num_str[32];
        sprintf(num_str, "%d", $2);
        
        // Convert to decimal using the convert_base function
        int decimal_value = convert_base($2, $4);
        
        if (debug_simulation) {
            printf("DEBUG: Integer constant %d in base %d = %d decimal\n", $2, $4, decimal_value);
        }
        
        $$ = create_node(NODE_INT_CONST);
        $$->data.int_const.int_value = decimal_value;  // Store the converted decimal value
        $$->data.int_const.base = $4;  // Keep original base for reference
    }
    ;

%%

int main(int argc, char *argv[]) {
    // Check for debug flag
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0) {
            debug_simulation = 1;
            // Remove this argument
            for (int j = i; j < argc - 1; j++) {
                argv[j] = argv[j + 1];
            }
            argc--;
            break;
        }
    }
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [-d|--debug] <input file>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error opening file");
        return 1;
    }

    int parse_result = yyparse();
    
    if (parse_result == 0) {
        printf("Parsing completed successfully.\n");
        
        printf("\n=== Program Output ===\n");
        
        // Execute the program by walking the AST
        if (ast_root) {
            execute_node(ast_root);
        }
        
        printf("\n=== End of Program Output ===\n");
        
        // Print the symbol table
        print_symbol_table();
    }

    // Clean up sequence - critical to avoid segmentation fault
    fclose(yyin);
    
    // First set the global flag to prevent any AST cleanup during memory release
    memory_cleanup_in_progress = 1;
    
    // Now clean up tracked memory first, which avoids problems with freeing the AST
    cleanup_memory();
    
    // Only now free the AST when we know other memory is cleaned
    if (ast_root) {
        free_ast(ast_root);
        ast_root = NULL;
    }
    
    return parse_result ? 1 : 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, s);
    exit(1);
}
