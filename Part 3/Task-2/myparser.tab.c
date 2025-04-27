/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "myparser.y"

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


#line 732 "myparser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "myparser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_DECIMAL = 3,                    /* DECIMAL  */
  YYSYMBOL_IDENTIFIER = 4,                 /* IDENTIFIER  */
  YYSYMBOL_STRING_LITERAL = 5,             /* STRING_LITERAL  */
  YYSYMBOL_INT_TYPE = 6,                   /* INT_TYPE  */
  YYSYMBOL_CHAR_TYPE = 7,                  /* CHAR_TYPE  */
  YYSYMBOL_CHAR_LITERAL = 8,               /* CHAR_LITERAL  */
  YYSYMBOL_BEGIN_TOKEN = 9,                /* BEGIN_TOKEN  */
  YYSYMBOL_END_TOKEN = 10,                 /* END_TOKEN  */
  YYSYMBOL_PROGRAM = 11,                   /* PROGRAM  */
  YYSYMBOL_VARDECL = 12,                   /* VARDECL  */
  YYSYMBOL_IF = 13,                        /* IF  */
  YYSYMBOL_ELSE = 14,                      /* ELSE  */
  YYSYMBOL_WHILE = 15,                     /* WHILE  */
  YYSYMBOL_FOR = 16,                       /* FOR  */
  YYSYMBOL_TO = 17,                        /* TO  */
  YYSYMBOL_INC = 18,                       /* INC  */
  YYSYMBOL_DEC = 19,                       /* DEC  */
  YYSYMBOL_DO = 20,                        /* DO  */
  YYSYMBOL_PRINT = 21,                     /* PRINT  */
  YYSYMBOL_SCAN = 22,                      /* SCAN  */
  YYSYMBOL_ASSIGN = 23,                    /* ASSIGN  */
  YYSYMBOL_PLUSEQ = 24,                    /* PLUSEQ  */
  YYSYMBOL_MINUSEQ = 25,                   /* MINUSEQ  */
  YYSYMBOL_MULEQ = 26,                     /* MULEQ  */
  YYSYMBOL_DIVEQ = 27,                     /* DIVEQ  */
  YYSYMBOL_MODEQ = 28,                     /* MODEQ  */
  YYSYMBOL_SLCOMMENT = 29,                 /* SLCOMMENT  */
  YYSYMBOL_MLCOMMENT = 30,                 /* MLCOMMENT  */
  YYSYMBOL_COLON = 31,                     /* COLON  */
  YYSYMBOL_SEMICOLON = 32,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 33,                     /* COMMA  */
  YYSYMBOL_LBRACKET = 34,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 35,                  /* RBRACKET  */
  YYSYMBOL_QUOTE = 36,                     /* QUOTE  */
  YYSYMBOL_DQUOTE = 37,                    /* DQUOTE  */
  YYSYMBOL_AT = 38,                        /* AT  */
  YYSYMBOL_ADD = 39,                       /* ADD  */
  YYSYMBOL_SUB = 40,                       /* SUB  */
  YYSYMBOL_MUL = 41,                       /* MUL  */
  YYSYMBOL_DIV = 42,                       /* DIV  */
  YYSYMBOL_MOD = 43,                       /* MOD  */
  YYSYMBOL_EQ = 44,                        /* EQ  */
  YYSYMBOL_GT = 45,                        /* GT  */
  YYSYMBOL_LT = 46,                        /* LT  */
  YYSYMBOL_GE = 47,                        /* GE  */
  YYSYMBOL_LE = 48,                        /* LE  */
  YYSYMBOL_NE = 49,                        /* NE  */
  YYSYMBOL_OB = 50,                        /* OB  */
  YYSYMBOL_CB = 51,                        /* CB  */
  YYSYMBOL_YYACCEPT = 52,                  /* $accept  */
  YYSYMBOL_program = 53,                   /* program  */
  YYSYMBOL_condition = 54,                 /* condition  */
  YYSYMBOL_comment = 55,                   /* comment  */
  YYSYMBOL_statementBlock = 56,            /* statementBlock  */
  YYSYMBOL_statement = 57,                 /* statement  */
  YYSYMBOL_assignment = 58,                /* assignment  */
  YYSYMBOL_expression = 59,                /* expression  */
  YYSYMBOL_term = 60,                      /* term  */
  YYSYMBOL_factor = 61,                    /* factor  */
  YYSYMBOL_block = 62,                     /* block  */
  YYSYMBOL_if_cond = 63,                   /* if_cond  */
  YYSYMBOL_while_loop = 64,                /* while_loop  */
  YYSYMBOL_for_loop = 65,                  /* for_loop  */
  YYSYMBOL_print = 66,                     /* print  */
  YYSYMBOL_formatArgs = 67,                /* formatArgs  */
  YYSYMBOL_argList = 68,                   /* argList  */
  YYSYMBOL_value = 69,                     /* value  */
  YYSYMBOL_scan = 70,                      /* scan  */
  YYSYMBOL_scanArgs = 71,                  /* scanArgs  */
  YYSYMBOL_identifierList = 72,            /* identifierList  */
  YYSYMBOL_identifierListT = 73,           /* identifierListT  */
  YYSYMBOL_assignment_op = 74,             /* assignment_op  */
  YYSYMBOL_relop = 75,                     /* relop  */
  YYSYMBOL_arrIndex = 76,                  /* arrIndex  */
  YYSYMBOL_varDecBlock = 77,               /* varDecBlock  */
  YYSYMBOL_VarDecList = 78,                /* VarDecList  */
  YYSYMBOL_VarDeclaration = 79,            /* VarDeclaration  */
  YYSYMBOL_arrayDec = 80,                  /* arrayDec  */
  YYSYMBOL_type = 81,                      /* type  */
  YYSYMBOL_integerConst = 82               /* integerConst  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   178

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  71
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  154

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   306


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   694,   694,   702,   708,   717,   723,   726,   729,   735,
     738,   746,   749,   752,   755,   758,   761,   764,   770,   787,
     788,   794,   800,   806,   812,   821,   825,   826,   837,   838,
     845,   849,   854,   863,   871,   895,   922,   933,   934,   938,
     942,   949,   959,   963,   970,   971,   982,   998,   999,  1015,
    1016,  1017,  1018,  1019,  1020,  1024,  1025,  1026,  1027,  1028,
    1029,  1033,  1034,  1038,  1045,  1049,  1056,  1080,  1081,  1089,
    1094,  1103
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "DECIMAL",
  "IDENTIFIER", "STRING_LITERAL", "INT_TYPE", "CHAR_TYPE", "CHAR_LITERAL",
  "BEGIN_TOKEN", "END_TOKEN", "PROGRAM", "VARDECL", "IF", "ELSE", "WHILE",
  "FOR", "TO", "INC", "DEC", "DO", "PRINT", "SCAN", "ASSIGN", "PLUSEQ",
  "MINUSEQ", "MULEQ", "DIVEQ", "MODEQ", "SLCOMMENT", "MLCOMMENT", "COLON",
  "SEMICOLON", "COMMA", "LBRACKET", "RBRACKET", "QUOTE", "DQUOTE", "AT",
  "ADD", "SUB", "MUL", "DIV", "MOD", "EQ", "GT", "LT", "GE", "LE", "NE",
  "OB", "CB", "$accept", "program", "condition", "comment",
  "statementBlock", "statement", "assignment", "expression", "term",
  "factor", "block", "if_cond", "while_loop", "for_loop", "print",
  "formatArgs", "argList", "value", "scan", "scanArgs", "identifierList",
  "identifierListT", "assignment_op", "relop", "arrIndex", "varDecBlock",
  "VarDecList", "VarDeclaration", "arrayDec", "type", "integerConst", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-85)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       0,   -85,   -85,    21,    22,   -85,    31,     2,    35,    34,
      96,    25,   101,    96,    10,    53,   100,    58,    60,    97,
      96,    74,   -85,   -85,   -85,   -85,   -85,   -85,     0,   -85,
     -85,   -85,   -85,   -85,   -85,     4,   103,     4,     4,    91,
     110,    57,   105,   -85,   -85,    87,   104,   -85,     1,    93,
     -85,   -85,   -85,   -85,    79,    45,    88,     4,   107,   108,
     109,    92,   -85,     0,   140,   -15,   -85,     4,   -85,   112,
      80,     4,     4,     4,     4,     4,   137,   -85,   -85,   -85,
     -85,   -85,   -85,     3,   127,    24,    -3,    98,   144,   -85,
     146,   119,   -85,   118,   143,   -85,   -16,   151,   -85,   -25,
     -25,   -85,   -85,   -85,    -4,   -85,    93,   137,     4,   -85,
     152,   -85,   123,   -85,   125,   108,   -85,   -85,   155,   126,
     148,   -85,   111,   137,   -85,   129,    40,    -3,   -85,   -85,
     128,    71,   -85,   -85,   132,   -85,     4,     4,   -85,   -85,
     -85,   -85,   114,   -85,    29,    56,   134,   137,   137,   -85,
     135,   136,   -85,   -85
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       8,     6,     7,     0,     0,     1,     0,     0,     0,     0,
       9,     0,     0,     9,     0,     0,     0,     0,     0,     0,
       9,     0,    17,    12,    13,    14,    15,    16,     8,    49,
      50,    51,    52,    53,    54,     0,     0,     0,     0,     0,
       0,     0,     0,    10,    11,     0,    61,    29,     0,    18,
      19,    25,    28,    30,     0,     5,     0,     0,    37,    47,
       0,     0,    44,     8,     0,     8,    64,     0,    27,     0,
       0,     0,     0,     0,     0,     0,     0,    55,    57,    59,
      56,    58,    60,     0,     0,     0,     0,     0,     0,    46,
       0,     0,     2,    67,     0,    65,     0,     0,    26,    20,
      21,    22,    23,    24,     0,     4,     3,     0,     0,    41,
       0,    38,    39,    42,     0,    47,    45,    43,     0,     0,
       0,    62,     0,     0,    31,     0,     0,     0,    36,    48,
       0,     0,    63,    71,     0,    33,     0,     0,    40,    68,
      69,    70,     0,    32,     0,     0,     0,     0,     0,    66,
       0,     0,    34,    35
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -85,   -85,   131,    -8,    32,   -85,   -85,   -35,   -85,   -85,
     -73,   -85,   -85,   -85,   -85,   -85,    43,   -85,   -85,   -85,
      81,    59,   -85,   -85,   -85,   -85,   -85,   113,   -85,   -85,
     -84
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     3,    54,     4,    19,    20,    21,    55,    50,    51,
      22,    23,    24,    25,    26,    87,   111,   112,    27,    61,
      62,    89,    35,    83,    68,    10,    65,    66,   119,   142,
      52
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      49,   109,   113,   104,    69,    46,   105,    46,    46,    47,
     123,    47,    47,    70,     1,     2,    73,    74,    75,   121,
      45,     5,    85,    71,    72,    73,    74,    75,   124,     1,
       2,     6,    96,     8,   125,    64,    99,   100,   101,   102,
     103,   108,     7,   113,     9,    36,    11,   110,   106,   147,
     134,    48,    43,    48,    48,    92,    28,    94,   136,   137,
      37,    59,    60,    71,    72,    73,    74,    75,    71,    72,
      73,    74,    75,   126,   150,   151,   148,   140,   141,    71,
      72,    73,    74,    75,    71,    72,    73,    74,    75,    77,
      78,    79,    80,    81,    82,    71,    72,    73,    74,    75,
      12,   144,   145,    38,    39,    13,    44,    42,    40,    14,
      41,    15,    16,    53,    57,    58,    63,    17,    18,    71,
      72,    73,    74,    75,    29,    30,    31,    32,    33,    34,
      76,    98,    71,    72,    73,    74,    75,    64,    67,    84,
      86,    88,    90,    91,    93,    97,    13,   107,   115,   114,
      59,   117,   118,   120,   122,    69,   127,   128,   130,   131,
     132,   135,   133,   139,   143,   146,   149,   152,   153,    56,
     138,   116,     0,     0,   129,     0,     0,     0,    95
};

static const yytype_int16 yycheck[] =
{
      35,     4,    86,    76,     3,     4,     3,     4,     4,     8,
      14,     8,     8,    48,    29,    30,    41,    42,    43,    35,
      28,     0,    57,    39,    40,    41,    42,    43,    32,    29,
      30,     9,    67,    31,   107,    50,    71,    72,    73,    74,
      75,    17,    11,   127,     9,    13,    12,    50,    83,    20,
     123,    50,    20,    50,    50,    63,    31,    65,    18,    19,
      50,     4,     5,    39,    40,    41,    42,    43,    39,    40,
      41,    42,    43,   108,   147,   148,    20,     6,     7,    39,
      40,    41,    42,    43,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    39,    40,    41,    42,    43,
       4,   136,   137,    50,     4,     9,    32,    10,    50,    13,
      50,    15,    16,    10,    23,     5,    11,    21,    22,    39,
      40,    41,    42,    43,    23,    24,    25,    26,    27,    28,
      51,    51,    39,    40,    41,    42,    43,    50,    34,    51,
      33,    33,    33,    51,     4,    33,     9,    20,     4,    51,
       4,    32,    34,    10,     3,     3,    33,    32,     3,    33,
      12,    32,    51,    35,    32,    51,    32,    32,    32,    38,
     127,    90,    -1,    -1,   115,    -1,    -1,    -1,    65
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    29,    30,    53,    55,     0,     9,    11,    31,     9,
      77,    12,     4,     9,    13,    15,    16,    21,    22,    56,
      57,    58,    62,    63,    64,    65,    66,    70,    31,    23,
      24,    25,    26,    27,    28,    74,    56,    50,    50,     4,
      50,    50,    10,    56,    32,    55,     4,     8,    50,    59,
      60,    61,    82,    10,    54,    59,    54,    23,     5,     4,
       5,    71,    72,    11,    50,    78,    79,    34,    76,     3,
      59,    39,    40,    41,    42,    43,    51,    44,    45,    46,
      47,    48,    49,    75,    51,    59,    33,    67,    33,    73,
      33,    51,    55,     4,    55,    79,    59,    33,    51,    59,
      59,    59,    59,    59,    62,     3,    59,    20,    17,     4,
      50,    68,    69,    82,    51,     4,    72,    32,    34,    80,
      10,    35,     3,    14,    32,    62,    59,    33,    32,    73,
       3,    33,    12,    51,    62,    32,    18,    19,    68,    35,
       6,     7,    81,    32,    59,    59,    51,    20,    20,    32,
      62,    62,    32,    32
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    52,    53,    54,    54,    54,    55,    55,    55,    56,
      56,    57,    57,    57,    57,    57,    57,    57,    58,    59,
      59,    59,    59,    59,    59,    60,    61,    61,    61,    61,
      62,    63,    63,    64,    65,    65,    66,    67,    67,    68,
      68,    69,    69,    70,    71,    71,    72,    73,    73,    74,
      74,    74,    74,    74,    74,    75,    75,    75,    75,    75,
      75,    76,    76,    77,    78,    78,    79,    80,    80,    81,
      81,    82
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     9,     3,     3,     1,     1,     1,     0,     0,
       2,     2,     1,     1,     1,     1,     1,     1,     3,     1,
       3,     3,     3,     3,     3,     1,     3,     2,     1,     1,
       3,     6,     8,     7,    11,    11,     6,     0,     2,     1,
       3,     1,     1,     5,     1,     3,     2,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     8,     1,     2,     7,     0,     3,     1,
       1,     5
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: comment BEGIN_TOKEN PROGRAM COLON varDecBlock statementBlock END_TOKEN PROGRAM comment  */
#line 694 "myparser.y"
                                                                                             {
        ast_root = create_node(NODE_PROGRAM);
        add_child(ast_root, (yyvsp[-4].node));
        add_child(ast_root, (yyvsp[-3].node));
    }
#line 1913 "myparser.tab.c"
    break;

  case 3: /* condition: expression relop expression  */
#line 702 "myparser.y"
                                  {
        (yyval.node) = create_node(NODE_RELOP);
        (yyval.node)->operator = (yyvsp[-1].string);
        add_child((yyval.node), (yyvsp[-2].node));
        add_child((yyval.node), (yyvsp[0].node));
    }
#line 1924 "myparser.tab.c"
    break;

  case 4: /* condition: expression relop DECIMAL  */
#line 708 "myparser.y"
                               {
        (yyval.node) = create_node(NODE_RELOP);
        (yyval.node)->operator = (yyvsp[-1].string);
        add_child((yyval.node), (yyvsp[-2].node));
        ASTNode* num = create_node(NODE_INT_CONST);
        num->data.int_const.int_value = (yyvsp[0].int_val);
        num->data.int_const.base = 10;
        add_child((yyval.node), num);
    }
#line 1938 "myparser.tab.c"
    break;

  case 5: /* condition: expression  */
#line 717 "myparser.y"
                 {
        (yyval.node) = (yyvsp[0].node);
    }
#line 1946 "myparser.tab.c"
    break;

  case 6: /* comment: SLCOMMENT  */
#line 723 "myparser.y"
                {
        (yyval.node) = create_node(NODE_COMMENT);
    }
#line 1954 "myparser.tab.c"
    break;

  case 7: /* comment: MLCOMMENT  */
#line 726 "myparser.y"
                {
        (yyval.node) = create_node(NODE_COMMENT);
    }
#line 1962 "myparser.tab.c"
    break;

  case 8: /* comment: %empty  */
#line 729 "myparser.y"
                  {
        (yyval.node) = NULL;
    }
#line 1970 "myparser.tab.c"
    break;

  case 9: /* statementBlock: %empty  */
#line 735 "myparser.y"
                  {
        (yyval.node) = create_node(NODE_BLOCK);
    }
#line 1978 "myparser.tab.c"
    break;

  case 10: /* statementBlock: statement statementBlock  */
#line 738 "myparser.y"
                               {
        (yyval.node) = create_node(NODE_BLOCK);
        add_child((yyval.node), (yyvsp[-1].node));
        add_child((yyval.node), (yyvsp[0].node));
    }
#line 1988 "myparser.tab.c"
    break;

  case 11: /* statement: assignment SEMICOLON  */
#line 746 "myparser.y"
                           {
        (yyval.node) = (yyvsp[-1].node);
    }
#line 1996 "myparser.tab.c"
    break;

  case 12: /* statement: if_cond  */
#line 749 "myparser.y"
              {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2004 "myparser.tab.c"
    break;

  case 13: /* statement: while_loop  */
#line 752 "myparser.y"
                 {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2012 "myparser.tab.c"
    break;

  case 14: /* statement: for_loop  */
#line 755 "myparser.y"
               {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2020 "myparser.tab.c"
    break;

  case 15: /* statement: print  */
#line 758 "myparser.y"
            {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2028 "myparser.tab.c"
    break;

  case 16: /* statement: scan  */
#line 761 "myparser.y"
           {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2036 "myparser.tab.c"
    break;

  case 17: /* statement: block  */
#line 764 "myparser.y"
            {
        (yyval.node) = (yyvsp[0].node);
    }
#line 2044 "myparser.tab.c"
    break;

  case 18: /* assignment: IDENTIFIER assignment_op expression  */
#line 770 "myparser.y"
                                          {
        int idx = find_symbol((yyvsp[-2].string));
        if (idx < 0) {
            fprintf(stderr, "Error: Undeclared variable '%s' at line %d\n", (yyvsp[-2].string), yylineno);
            exit(1);
        }
        (yyval.node) = create_node(NODE_ASSIGN);
        (yyval.node)->operator = (yyvsp[-1].string);
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = strdup((yyvsp[-2].string));
        track_allocation(id->data.string_value);
        add_child((yyval.node), id);
        add_child((yyval.node), (yyvsp[0].node));
    }
#line 2063 "myparser.tab.c"
    break;

  case 19: /* expression: term  */
#line 787 "myparser.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2069 "myparser.tab.c"
    break;

  case 20: /* expression: expression ADD expression  */
#line 788 "myparser.y"
                                {
        (yyval.node) = create_node(NODE_BINARY_OP);
        (yyval.node)->operator = (yyvsp[-1].string);
        add_child((yyval.node), (yyvsp[-2].node));
        add_child((yyval.node), (yyvsp[0].node));
    }
#line 2080 "myparser.tab.c"
    break;

  case 21: /* expression: expression SUB expression  */
#line 794 "myparser.y"
                                {
        (yyval.node) = create_node(NODE_BINARY_OP);
        (yyval.node)->operator = (yyvsp[-1].string);
        add_child((yyval.node), (yyvsp[-2].node));
        add_child((yyval.node), (yyvsp[0].node));
    }
#line 2091 "myparser.tab.c"
    break;

  case 22: /* expression: expression MUL expression  */
#line 800 "myparser.y"
                                {
        (yyval.node) = create_node(NODE_BINARY_OP);
        (yyval.node)->operator = (yyvsp[-1].string);
        add_child((yyval.node), (yyvsp[-2].node));
        add_child((yyval.node), (yyvsp[0].node));
    }
#line 2102 "myparser.tab.c"
    break;

  case 23: /* expression: expression DIV expression  */
#line 806 "myparser.y"
                                {
        (yyval.node) = create_node(NODE_BINARY_OP);
        (yyval.node)->operator = (yyvsp[-1].string);
        add_child((yyval.node), (yyvsp[-2].node));
        add_child((yyval.node), (yyvsp[0].node));
    }
#line 2113 "myparser.tab.c"
    break;

  case 24: /* expression: expression MOD expression  */
#line 812 "myparser.y"
                                {
        (yyval.node) = create_node(NODE_BINARY_OP);
        (yyval.node)->operator = (yyvsp[-1].string);
        add_child((yyval.node), (yyvsp[-2].node));
        add_child((yyval.node), (yyvsp[0].node));
    }
#line 2124 "myparser.tab.c"
    break;

  case 25: /* term: factor  */
#line 821 "myparser.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2130 "myparser.tab.c"
    break;

  case 26: /* factor: OB expression CB  */
#line 825 "myparser.y"
                       { (yyval.node) = (yyvsp[-1].node); }
#line 2136 "myparser.tab.c"
    break;

  case 27: /* factor: IDENTIFIER arrIndex  */
#line 826 "myparser.y"
                          {
        int idx = find_symbol((yyvsp[-1].string));
        if (idx < 0) {
            fprintf(stderr, "Error: Undeclared variable '%s' at line %d\n", (yyvsp[-1].string), yylineno);
            exit(1);
        }
        (yyval.node) = create_node(NODE_IDENTIFIER);
        (yyval.node)->data.string_value = strdup((yyvsp[-1].string));
        track_allocation((yyval.node)->data.string_value);
        if ((yyvsp[0].node)) add_child((yyval.node), (yyvsp[0].node));
    }
#line 2152 "myparser.tab.c"
    break;

  case 28: /* factor: integerConst  */
#line 837 "myparser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 2158 "myparser.tab.c"
    break;

  case 29: /* factor: CHAR_LITERAL  */
#line 838 "myparser.y"
                   {
        (yyval.node) = create_node(NODE_CHAR_CONST);
        (yyval.node)->data.char_value = (yyvsp[0].char_val);
    }
#line 2167 "myparser.tab.c"
    break;

  case 30: /* block: BEGIN_TOKEN statementBlock END_TOKEN  */
#line 845 "myparser.y"
                                           { (yyval.node) = (yyvsp[-1].node); }
#line 2173 "myparser.tab.c"
    break;

  case 31: /* if_cond: IF OB condition CB block SEMICOLON  */
#line 849 "myparser.y"
                                         {
        (yyval.node) = create_node(NODE_IF);
        add_child((yyval.node), (yyvsp[-3].node));
        add_child((yyval.node), (yyvsp[-1].node));
    }
#line 2183 "myparser.tab.c"
    break;

  case 32: /* if_cond: IF OB condition CB block ELSE block SEMICOLON  */
#line 854 "myparser.y"
                                                    {
        (yyval.node) = create_node(NODE_IF);
        add_child((yyval.node), (yyvsp[-5].node));
        add_child((yyval.node), (yyvsp[-3].node));
        add_child((yyval.node), (yyvsp[-1].node));
    }
#line 2194 "myparser.tab.c"
    break;

  case 33: /* while_loop: WHILE OB condition CB DO block SEMICOLON  */
#line 863 "myparser.y"
                                               {
        (yyval.node) = create_node(NODE_WHILE);
        add_child((yyval.node), (yyvsp[-4].node));
        add_child((yyval.node), (yyvsp[-1].node));
    }
#line 2204 "myparser.tab.c"
    break;

  case 34: /* for_loop: FOR IDENTIFIER ASSIGN expression TO expression INC expression DO block SEMICOLON  */
#line 871 "myparser.y"
                                                                                       {
        int idx = find_symbol((yyvsp[-9].string));
        if (idx < 0) {
            fprintf(stderr, "Error: Undeclared variable '%s' at line %d\n", (yyvsp[-9].string), yylineno);
            exit(1);
        }
        (yyval.node) = create_node(NODE_FOR);
        ASTNode* assign = create_node(NODE_ASSIGN);
        assign->operator = strdup(":=");
        track_allocation(assign->operator);
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = strdup((yyvsp[-9].string));
        track_allocation(id->data.string_value);
        add_child(assign, id);
        add_child(assign, (yyvsp[-7].node));
        add_child((yyval.node), assign);
        add_child((yyval.node), (yyvsp[-5].node));
        ASTNode* inc = create_node(NODE_BINARY_OP);
        inc->operator = strdup("inc");
        track_allocation(inc->operator);
        add_child(inc, (yyvsp[-3].node));
        add_child((yyval.node), inc);
        add_child((yyval.node), (yyvsp[-1].node));
    }
#line 2233 "myparser.tab.c"
    break;

  case 35: /* for_loop: FOR IDENTIFIER ASSIGN expression TO expression DEC expression DO block SEMICOLON  */
#line 895 "myparser.y"
                                                                                       {
        int idx = find_symbol((yyvsp[-9].string));
        if (idx < 0) {
            fprintf(stderr, "Error: Undeclared variable '%s' at line %d\n", (yyvsp[-9].string), yylineno);
            exit(1);
        }
        (yyval.node) = create_node(NODE_FOR);
        ASTNode* assign = create_node(NODE_ASSIGN);
        assign->operator = strdup(":=");
        track_allocation(assign->operator);
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = strdup((yyvsp[-9].string));
        track_allocation(id->data.string_value);
        add_child(assign, id);
        add_child(assign, (yyvsp[-7].node));
        add_child((yyval.node), assign);
        add_child((yyval.node), (yyvsp[-5].node));
        ASTNode* dec = create_node(NODE_BINARY_OP);
        dec->operator = strdup("dec");
        track_allocation(dec->operator);
        add_child(dec, (yyvsp[-3].node));
        add_child((yyval.node), dec);
        add_child((yyval.node), (yyvsp[-1].node));
    }
#line 2262 "myparser.tab.c"
    break;

  case 36: /* print: PRINT OB STRING_LITERAL formatArgs CB SEMICOLON  */
#line 922 "myparser.y"
                                                      {
        (yyval.node) = create_node(NODE_PRINT);
        ASTNode* str = create_node(NODE_STRING_CONST);
        str->data.string_value = strdup((yyvsp[-3].string));
        track_allocation(str->data.string_value);
        add_child((yyval.node), str);
        add_child((yyval.node), (yyvsp[-2].node));
    }
#line 2275 "myparser.tab.c"
    break;

  case 37: /* formatArgs: %empty  */
#line 933 "myparser.y"
                  { (yyval.node) = NULL; }
#line 2281 "myparser.tab.c"
    break;

  case 38: /* formatArgs: COMMA argList  */
#line 934 "myparser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2287 "myparser.tab.c"
    break;

  case 39: /* argList: value  */
#line 938 "myparser.y"
            {
        (yyval.node) = create_node(NODE_BLOCK);
        add_child((yyval.node), (yyvsp[0].node));
    }
#line 2296 "myparser.tab.c"
    break;

  case 40: /* argList: value COMMA argList  */
#line 942 "myparser.y"
                          {
        (yyval.node) = (yyvsp[0].node);
        add_child((yyval.node), (yyvsp[-2].node));
    }
#line 2305 "myparser.tab.c"
    break;

  case 41: /* value: IDENTIFIER  */
#line 949 "myparser.y"
                 {
        int idx = find_symbol((yyvsp[0].string));
        if (idx < 0) {
            fprintf(stderr, "Error: Undeclared variable '%s' at line %d\n", (yyvsp[0].string), yylineno);
            exit(1);
        }
        (yyval.node) = create_node(NODE_IDENTIFIER);
        (yyval.node)->data.string_value = strdup((yyvsp[0].string));
        track_allocation((yyval.node)->data.string_value);
    }
#line 2320 "myparser.tab.c"
    break;

  case 42: /* value: integerConst  */
#line 959 "myparser.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 2326 "myparser.tab.c"
    break;

  case 43: /* scan: SCAN OB scanArgs CB SEMICOLON  */
#line 963 "myparser.y"
                                    {
        (yyval.node) = create_node(NODE_SCAN);
        add_child((yyval.node), (yyvsp[-2].node));
    }
#line 2335 "myparser.tab.c"
    break;

  case 44: /* scanArgs: identifierList  */
#line 970 "myparser.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 2341 "myparser.tab.c"
    break;

  case 45: /* scanArgs: STRING_LITERAL COMMA identifierList  */
#line 971 "myparser.y"
                                          {
        (yyval.node) = create_node(NODE_BLOCK);
        ASTNode* str = create_node(NODE_STRING_CONST);
        str->data.string_value = strdup((yyvsp[-2].string));
        track_allocation(str->data.string_value);
        add_child((yyval.node), str);
        add_child((yyval.node), (yyvsp[0].node));
    }
#line 2354 "myparser.tab.c"
    break;

  case 46: /* identifierList: IDENTIFIER identifierListT  */
#line 982 "myparser.y"
                                 {
        int idx = find_symbol((yyvsp[-1].string));
        if (idx < 0) {
            fprintf(stderr, "Error: Undeclared variable '%s' at line %d\n", (yyvsp[-1].string), yylineno);
            exit(1);
        }
        (yyval.node) = create_node(NODE_BLOCK);
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = strdup((yyvsp[-1].string));
        track_allocation(id->data.string_value);
        add_child((yyval.node), id);
        add_child((yyval.node), (yyvsp[0].node));
    }
#line 2372 "myparser.tab.c"
    break;

  case 47: /* identifierListT: %empty  */
#line 998 "myparser.y"
                  { (yyval.node) = NULL; }
#line 2378 "myparser.tab.c"
    break;

  case 48: /* identifierListT: COMMA IDENTIFIER identifierListT  */
#line 999 "myparser.y"
                                       {
        int idx = find_symbol((yyvsp[-1].string));
        if (idx < 0) {
            fprintf(stderr, "Error: Undeclared variable '%s' at line %d\n", (yyvsp[-1].string), yylineno);
            exit(1);
        }
        (yyval.node) = create_node(NODE_BLOCK);
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = strdup((yyvsp[-1].string));
        track_allocation(id->data.string_value);
        add_child((yyval.node), id);
        add_child((yyval.node), (yyvsp[0].node));
    }
#line 2396 "myparser.tab.c"
    break;

  case 49: /* assignment_op: ASSIGN  */
#line 1015 "myparser.y"
             { (yyval.string) = strdup(":="); track_allocation((yyval.string)); }
#line 2402 "myparser.tab.c"
    break;

  case 50: /* assignment_op: PLUSEQ  */
#line 1016 "myparser.y"
             { (yyval.string) = strdup("+="); track_allocation((yyval.string)); }
#line 2408 "myparser.tab.c"
    break;

  case 51: /* assignment_op: MINUSEQ  */
#line 1017 "myparser.y"
              { (yyval.string) = strdup("-="); track_allocation((yyval.string)); }
#line 2414 "myparser.tab.c"
    break;

  case 52: /* assignment_op: MULEQ  */
#line 1018 "myparser.y"
            { (yyval.string) = strdup("*="); track_allocation((yyval.string)); }
#line 2420 "myparser.tab.c"
    break;

  case 53: /* assignment_op: DIVEQ  */
#line 1019 "myparser.y"
            { (yyval.string) = strdup("/="); track_allocation((yyval.string)); }
#line 2426 "myparser.tab.c"
    break;

  case 54: /* assignment_op: MODEQ  */
#line 1020 "myparser.y"
            { (yyval.string) = strdup("%="); track_allocation((yyval.string)); }
#line 2432 "myparser.tab.c"
    break;

  case 55: /* relop: EQ  */
#line 1024 "myparser.y"
         { (yyval.string) = strdup("="); track_allocation((yyval.string)); }
#line 2438 "myparser.tab.c"
    break;

  case 56: /* relop: GE  */
#line 1025 "myparser.y"
         { (yyval.string) = strdup(">="); track_allocation((yyval.string)); }
#line 2444 "myparser.tab.c"
    break;

  case 57: /* relop: GT  */
#line 1026 "myparser.y"
         { (yyval.string) = strdup(">"); track_allocation((yyval.string)); }
#line 2450 "myparser.tab.c"
    break;

  case 58: /* relop: LE  */
#line 1027 "myparser.y"
         { (yyval.string) = strdup("<="); track_allocation((yyval.string)); }
#line 2456 "myparser.tab.c"
    break;

  case 59: /* relop: LT  */
#line 1028 "myparser.y"
         { (yyval.string) = strdup("<"); track_allocation((yyval.string)); }
#line 2462 "myparser.tab.c"
    break;

  case 60: /* relop: NE  */
#line 1029 "myparser.y"
         { (yyval.string) = strdup("!="); track_allocation((yyval.string)); }
#line 2468 "myparser.tab.c"
    break;

  case 61: /* arrIndex: %empty  */
#line 1033 "myparser.y"
                  { (yyval.node) = NULL; }
#line 2474 "myparser.tab.c"
    break;

  case 62: /* arrIndex: LBRACKET expression RBRACKET  */
#line 1034 "myparser.y"
                                   { (yyval.node) = (yyvsp[-1].node); }
#line 2480 "myparser.tab.c"
    break;

  case 63: /* varDecBlock: BEGIN_TOKEN VARDECL COLON comment VarDecList comment END_TOKEN VARDECL  */
#line 1038 "myparser.y"
                                                                             {
        (yyval.node) = create_node(NODE_BLOCK);
        add_child((yyval.node), (yyvsp[-3].node));
    }
#line 2489 "myparser.tab.c"
    break;

  case 64: /* VarDecList: VarDeclaration  */
#line 1045 "myparser.y"
                     {
        (yyval.node) = create_node(NODE_BLOCK);
        add_child((yyval.node), (yyvsp[0].node));
    }
#line 2498 "myparser.tab.c"
    break;

  case 65: /* VarDecList: VarDecList VarDeclaration  */
#line 1049 "myparser.y"
                                {
        (yyval.node) = (yyvsp[-1].node);
        add_child((yyval.node), (yyvsp[0].node));
    }
#line 2507 "myparser.tab.c"
    break;

  case 66: /* VarDeclaration: OB IDENTIFIER arrayDec COMMA type CB SEMICOLON  */
#line 1056 "myparser.y"
                                                     {
        VarType var_type;
        if (strcmp((yyvsp[-2].node)->data.string_value, "int") == 0) {
            var_type = TYPE_INT;
        } else if (strcmp((yyvsp[-2].node)->data.string_value, "char") == 0) {
            var_type = TYPE_CHAR;
        } else {
            var_type = TYPE_UNKNOWN;
        }
        
        int array_size = (yyvsp[-4].node) ? (yyvsp[-4].node)->data.int_const.int_value : 0;
        declare_variable((yyvsp[-5].string), var_type, (yyvsp[-4].node) != NULL, array_size);
        
        (yyval.node) = create_node(NODE_VARDECL);
        ASTNode* id = create_node(NODE_IDENTIFIER);
        id->data.string_value = strdup((yyvsp[-5].string));
        track_allocation(id->data.string_value);
        add_child((yyval.node), id);
        add_child((yyval.node), (yyvsp[-2].node));
        if ((yyvsp[-4].node)) add_child((yyval.node), (yyvsp[-4].node));
    }
#line 2533 "myparser.tab.c"
    break;

  case 67: /* arrayDec: %empty  */
#line 1080 "myparser.y"
                  { (yyval.node) = NULL; }
#line 2539 "myparser.tab.c"
    break;

  case 68: /* arrayDec: LBRACKET DECIMAL RBRACKET  */
#line 1081 "myparser.y"
                                {
        (yyval.node) = create_node(NODE_INT_CONST);
        (yyval.node)->data.int_const.int_value = (yyvsp[-1].int_val);
        (yyval.node)->data.int_const.base = 10;
    }
#line 2549 "myparser.tab.c"
    break;

  case 69: /* type: INT_TYPE  */
#line 1089 "myparser.y"
               {
        (yyval.node) = create_node(NODE_TYPE);
        (yyval.node)->data.string_value = strdup("int");
        track_allocation((yyval.node)->data.string_value);
    }
#line 2559 "myparser.tab.c"
    break;

  case 70: /* type: CHAR_TYPE  */
#line 1094 "myparser.y"
                {
        (yyval.node) = create_node(NODE_TYPE);
        (yyval.node)->data.string_value = strdup("char");
        track_allocation((yyval.node)->data.string_value);
    }
#line 2569 "myparser.tab.c"
    break;

  case 71: /* integerConst: OB DECIMAL COMMA DECIMAL CB  */
#line 1103 "myparser.y"
                                  {
        if ((yyvsp[-1].int_val) != 2 && (yyvsp[-1].int_val) != 8 && (yyvsp[-1].int_val) != 10) {
            printf("Invalid base %d at line %d\n", (yyvsp[-1].int_val), yylineno);
            exit(1);
        }
        
        char num_str[32];
        sprintf(num_str, "%d", (yyvsp[-3].int_val));
        
        // Convert to decimal using the convert_base function
        int decimal_value = convert_base((yyvsp[-3].int_val), (yyvsp[-1].int_val));
        
        if (debug_simulation) {
            printf("DEBUG: Integer constant %d in base %d = %d decimal\n", (yyvsp[-3].int_val), (yyvsp[-1].int_val), decimal_value);
        }
        
        (yyval.node) = create_node(NODE_INT_CONST);
        (yyval.node)->data.int_const.int_value = decimal_value;  // Store the converted decimal value
        (yyval.node)->data.int_const.base = (yyvsp[-1].int_val);  // Keep original base for reference
    }
#line 2594 "myparser.tab.c"
    break;


#line 2598 "myparser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1125 "myparser.y"


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
