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

// Add these at the top of your file with other globals
char* current_loop_start = NULL;
char* current_loop_end = NULL;
char* current_for_start = NULL;
char* current_for_end = NULL;
char* loop_var = NULL;
char* loop_end = NULL;
char *__if_else_L1, *__if_else_L2;
  char* __if_false;
  char* __if_end;
  /* above all your rules */
char* current_for_step_temp;

// ---- Extended symbol table for simulation ----
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
#define MAX_TEMP_VARS 1000
#define MAX_INSTRUCTIONS 5000

Symbol symbol_table[MAX_SYMBOLS];
Symbol temp_vars[MAX_TEMP_VARS];  // Store values for temporary variables
int symbol_count = 0;

// Instructions for simulation
typedef struct {
    char* result;
    char* arg1;
    char* op;
    char* arg2;
    int is_label;
    int is_goto;
    int is_cond_jump;
    int is_print;
    int is_scan;
    char* label;
    char* condition;
} Instruction;

Instruction instructions[MAX_INSTRUCTIONS];
int instruction_count = 0;

// Memory management ----
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
int temp_var_counter = 1;  // For regular temp variables (t1, t2, etc.)
int cond_var_counter = 1;  // For condition variables (t_cond1, t_cond2, etc.)

char* newCondTemp() {
    char* temp = malloc(20);
    sprintf(temp, "t_cond%d", cond_var_counter++);
    track_allocation(temp);
    return temp;
}

char* newTemp() {
    char* temp = malloc(10);
    sprintf(temp, "t%d", temp_count+1);
    temp_count++;
    track_allocation(temp);
    return temp;
}

char* newLabel() {
    char* label = malloc(10);
    sprintf(label, "L%d", label_count+1);
    label_count++;
    track_allocation(label);
    return label;
}

// ---- Emit TAC and store for simulation ----
void emit(char* result, char* arg1, char* op, char* arg2) {
    if (arg2 && op) printf("%s := %s %s %s\n", result, arg1, op, arg2);
    else if (op)    printf("%s := %s %s\n", result, op, arg1);
    else            printf("%s := %s\n", result, arg1);
    
    // Store instruction for simulation
    instructions[instruction_count].result = strdup(result);
    instructions[instruction_count].arg1 = arg1 ? strdup(arg1) : NULL;
    instructions[instruction_count].op = op ? strdup(op) : NULL;
    instructions[instruction_count].arg2 = arg2 ? strdup(arg2) : NULL;
    instructions[instruction_count].is_label = 0;
    instructions[instruction_count].is_goto = 0;
    instructions[instruction_count].is_cond_jump = 0;
    instructions[instruction_count].is_print = 0;
    instructions[instruction_count].is_scan = 0;
    instructions[instruction_count].label = NULL;
    instructions[instruction_count].condition = NULL;
    instruction_count++;
}

void emitPrint(char* str, char* args) {
    // Store print instruction
    instructions[instruction_count].is_print = 1;
    instructions[instruction_count].arg1 = strdup(str);
    instructions[instruction_count].arg2 = args ? strdup(args) : NULL;
    instruction_count++;
    
    // Check format specifiers match argument count
    if (args) {
        int format_count = 0;
        int arg_count = 1; // Start with 1 for the first argument
        
        // Count format specifiers
        for (int i = 0; i < strlen(str); i++) {
            if (str[i] == '%' && i+1 < strlen(str) && str[i+1] == 'd') {
                format_count++;
            }
        }
        
        // Count arguments
        for (int i = 0; i < strlen(args); i++) {
            if (args[i] == ',') arg_count++;
        }
        
        if (format_count != arg_count) {
            fprintf(stderr, "Warning at line %d: Format specifier count (%d) doesn't match argument count (%d)\n", 
                    yylineno, format_count, arg_count);
        }
    }
}

void emitScan(char* str, char* args) {
    // Store scan instruction
    instructions[instruction_count].is_scan = 1;
    instructions[instruction_count].arg1 = strdup(str);
    instructions[instruction_count].arg2 = args ? strdup(args) : NULL;
    instruction_count++;
}

void emitCondJump(char* cond, char* label) {
    printf("if %s == 0 goto %s\n", cond, label);
    
    // Store conditional jump for simulation
    instructions[instruction_count].is_cond_jump = 1;
    instructions[instruction_count].condition = strdup(cond);
    instructions[instruction_count].label = strdup(label);
    instruction_count++;
}

void emitCondJumpTrue(char* cond, char* label) {
    printf("if %s == 1 goto %s\n", cond, label);
    
    // Store conditional jump for simulation
    instructions[instruction_count].is_cond_jump = 1;
    instructions[instruction_count].condition = strdup(cond);
    instructions[instruction_count].label = strdup(label);
    // This is a jump if true, so we need to invert our condition check in simulation
    instructions[instruction_count].arg1 = strdup("true");
    instruction_count++;
}

void emitGoto(char* label) {
    printf("goto %s\n", label);
    
    // Store goto for simulation
    instructions[instruction_count].is_goto = 1;
    instructions[instruction_count].label = strdup(label);
    instruction_count++;
}

void emitLabel(char* label) {
    printf("%s:\n", label);
    
    // Store label for simulation
    instructions[instruction_count].is_label = 1;
    instructions[instruction_count].label = strdup(label);
    instruction_count++;
}

// ---- Variable and symbol table management ----
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

// Find symbol by name in symbol table
int find_symbol(const char* name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0)
            return i;
    }
    return -1;
}

// Find temp variable
int find_temp_var(const char* name) {
    // Check if it's a temporary variable (t1, t2, etc.)
    if (name[0] == 't' && (name[1] == '_' || isdigit(name[1]))) {
        int idx = -1;
        if (name[1] == '_') {
            // Handle t_cond variables
            sscanf(name, "t_cond%d", &idx);
            if (idx > 0 && idx <= cond_var_counter)
                return idx - 1;
        } else {
            sscanf(name, "t%d", &idx);
            if (idx > 0 && idx <= temp_var_counter)
                return idx - 1;
        }
    }
    return -1;
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
    
    // Also add to symbol table for simulation
    if (symbol_count < MAX_SYMBOLS) {
        symbol_table[symbol_count].name = strdup(name);
        symbol_table[symbol_count].type = TYPE_UNKNOWN;
        symbol_table[symbol_count].initialized = 0;
        symbol_table[symbol_count].is_array = 0;
        symbol_count++;
    } else {
        fprintf(stderr, "Error: Symbol table full\n");
        exit(1);
    }
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

// ---- Simulation functions ----
// Helper function to get the actual value from (value, base) format
int parse_integer_const(const char* str) {
    // Skip the opening bracket
    char* copy = strdup(str + 1);
    // Find the comma
    char* comma = strchr(copy, ',');
    if (!comma) {
        free(copy);
        return atoi(str); // Default fallback if format is invalid
    }
    *comma = '\0';
    
    // Get the value and base
    int value = atoi(copy);
    int base = atoi(comma + 1);
    
    // Convert from the given base to decimal (only relevant for base 2 and 8)
    if (base == 2 || base == 8) {
        int decimal_value = 0;
        int digit_value;
        for (int i = 0; copy[i] != '\0'; i++) {
            digit_value = copy[i] - '0';
            if ((base == 2 && digit_value > 1) || 
                (base == 8 && digit_value > 7)) {
                fprintf(stderr, "Semantic error: Invalid digit for base %d\n", base);
                exit(1);
            }
            decimal_value = decimal_value * base + digit_value;
        }
        value = decimal_value;
    }
    
    free(copy);
    return value;
}

// Get integer value of a variable or literal
int get_int_value(const char* name) {
    // Check if it's an array access
    char* bracket_pos = strchr(name, '[');
    if (bracket_pos) {
        char base_name[256];
        int index;
        
        // Extract base name and index
        int base_len = bracket_pos - name;
        strncpy(base_name, name, base_len);
        base_name[base_len] = '\0';
        
        // Parse the index
        sscanf(bracket_pos + 1, "%d", &index);
        
        // Check array bounds
        check_array_bounds(base_name, index);
        
        // Get the array element value
        int array_idx = find_symbol(base_name);
        if (array_idx >= 0) {
            if (!symbol_table[array_idx].initialized) {
                fprintf(stderr, "Semantic error at line %d: Array '%s' used before initialization.\n", 
                        yylineno, base_name);
                exit(1);
            }
            // For simplicity, we're treating all arrays as int arrays
            // In a real interpreter, this would depend on the array type
            return symbol_table[array_idx].value.int_val + index; // Placeholder for array access
        }
    }

    // Check if it's a literal number directly
    if (isdigit(name[0]) || (name[0] == '-' && isdigit(name[1]))) {
        return atoi(name);
    }
    
    // Check if it's in (value, base) format
    if (name[0] == '(') {
        return parse_integer_const(name);
    }
    
    // Check symbol table
    int idx = find_symbol(name);
    if (idx >= 0) {
        if (!symbol_table[idx].initialized) {
            fprintf(stderr, "Semantic error at line %d: Variable '%s' used before initialization.\n", 
                    yylineno, name);
            exit(1);
        }
        return symbol_table[idx].value.int_val;
    }
    
    // Check temp variables
    idx = find_temp_var(name);
    if (idx >= 0) {
        if (!temp_vars[idx].initialized) {
            fprintf(stderr, "Internal error: Temporary variable '%s' used before initialization.\n", name);
            exit(1);
        }
        return temp_vars[idx].value.int_val;
    }
    
    fprintf(stderr, "Semantic error: Undefined variable '%s'\n", name);
    exit(1);
    return 0;  // never reached
}

// Set integer value of a variable
void set_int_value(const char* name, int value) {
    // Check if it's a temporary variable
    int idx = find_temp_var(name);
    if (idx >= 0) {
        temp_vars[idx].value.int_val = value;
        temp_vars[idx].initialized = 1;
        temp_vars[idx].type = TYPE_INT;
        return;
    }
    
    // Check symbol table
    idx = find_symbol(name);
    if (idx >= 0) {
        symbol_table[idx].value.int_val = value;
        symbol_table[idx].initialized = 1;
        symbol_table[idx].type = TYPE_INT;
        return;
    }
    
    fprintf(stderr, "Semantic error: Undefined variable '%s'\n", name);
    exit(1);
}

// Perform arithmetic operation
int perform_op(int left, const char* op, int right) {
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
    if (strcmp(op, "and") == 0) return left && right;
    if (strcmp(op, "or") == 0) return left || right;
    
    // Comparison operations
    if (strcmp(op, "=") == 0) return left == right;
    if (strcmp(op, "!=") == 0) return left != right;
    if (strcmp(op, ">") == 0) return left > right;
    if (strcmp(op, ">=") == 0) return left >= right;
    if (strcmp(op, "<") == 0) return left < right;
    if (strcmp(op, "<=") == 0) return left <= right;
    
    fprintf(stderr, "Semantic error: Unknown operator '%s'\n", op);
    exit(1);
    return 0;
}

// Add this function to check array access bounds
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

// Add a debug flag to help with debugging
int debug_simulation = 0;  // Set to 1 to enable debug messages during simulation

// Modify execute_instruction to add debug information
int execute_instruction(int idx) {
    Instruction* instr = &instructions[idx];
    
    if (debug_simulation) {
        printf("DEBUG[%d]: ", idx);
        if (instr->is_label) printf("Label: %s\n", instr->label);
        else if (instr->is_goto) printf("Goto: %s\n", instr->label);
        else if (instr->is_cond_jump) printf("Cond jump: %s to %s\n", instr->condition, instr->label);
        else if (instr->is_print) printf("Print: %s\n", instr->arg1);
        else if (instr->is_scan) printf("Scan: %s, vars: %s\n", instr->arg1, instr->arg2 ? instr->arg2 : "none");
        else if (instr->result) {
            if (instr->op && instr->arg2)
                printf("Op: %s := %s %s %s\n", instr->result, instr->arg1, instr->op, instr->arg2);
            else if (instr->op)
                printf("Op: %s := %s %s\n", instr->result, instr->op, instr->arg1);
            else
                printf("Assign: %s := %s\n", instr->result, instr->arg1);
        }
    }
    
    // Handle label (no action needed, just for jumps)
    if (instr->is_label) return idx + 1;
    
    // Handle goto
    if (instr->is_goto) {
        // Find the label
        for (int i = 0; i < instruction_count; i++) {
            if (instructions[i].is_label && strcmp(instructions[i].label, instr->label) == 0) {
                return i;  // Return the index to jump to
            }
        }
        fprintf(stderr, "Runtime error: Label '%s' not found\n", instr->label);
        exit(1);
    }
    
    // Handle conditional jump
    if (instr->is_cond_jump) {
        int condition_value = get_int_value(instr->condition);
        if (instr->arg1 && strcmp(instr->arg1, "true") == 0) {
            // Jump if true (condition == 1)
            if (condition_value == 1) {
                // Find the label
                for (int i = 0; i < instruction_count; i++) {
                    if (instructions[i].is_label && strcmp(instructions[i].label, instr->label) == 0) {
                        return i;  // Return the index to jump to
                    }
                }
            }
        } else {
            // Jump if false (condition == 0)
            if (condition_value == 0) {
                // Find the label
                for (int i = 0; i < instruction_count; i++) {
                    if (instructions[i].is_label && strcmp(instructions[i].label, instr->label) == 0) {
                        return i;  // Return the index to jump to
                    }
                }
            }
        }
        return idx + 1;  // Continue to next instruction
    }
    
    // Handle print
    if (instr->is_print) {
        // Extract string without quotes
        char* str = instr->arg1;
        int len = strlen(str);
        char* clean_str = malloc(len - 1);
        strncpy(clean_str, str + 1, len - 2);
        clean_str[len - 2] = '\0';
        
        if (instr->arg2) {
            // Format string with arguments
            char* args_copy = strdup(instr->arg2);
            char* token = strtok(args_copy, ",");
            int arg_count = 0;
            int values[10];  // Support up to 10 arguments
            
            // Parse all arguments
            while (token && arg_count < 10) {
                values[arg_count++] = get_int_value(token);
                token = strtok(NULL, ",");
            }
            
            // Simple formatted output - replace %d with values
            char* fmt = clean_str;
            char* output = malloc(1000);
            char* out_ptr = output;
            int val_idx = 0;
            
            while (*fmt) {
                if (*fmt == '%' && *(fmt + 1) == 'd') {
                    if (val_idx < arg_count) {
                        // We have a value for this format specifier
                        int chars = sprintf(out_ptr, "%d", values[val_idx++]);
                        out_ptr += chars;
                    } else {
                        // More format specifiers than values - print error indicator
                        strcpy(out_ptr, "[ERROR:missing arg]");
                        out_ptr += strlen("[ERROR:missing arg]");
                    }
                    fmt += 2;
                } else {
                    *out_ptr++ = *fmt++;
                }
            }
            *out_ptr = '\0';
            
            printf("%s\n", output);
            free(output);
            free(args_copy);
        } else {
            printf("%s\n", clean_str);
        }
        free(clean_str);
        return idx + 1;
    }

    // Handle scan
    if (instr->is_scan) {
        // Extract string without quotes
        char* str = instr->arg1;
        int len = strlen(str);
        char* clean_str = malloc(len - 1);
        strncpy(clean_str, str + 1, len - 2);
        clean_str[len - 2] = '\0';
        
        printf("%s", clean_str);
        
        if (instr->arg2) {
            // Handle variables
            char* args_copy = strdup(instr->arg2);
            char* token = strtok(args_copy, ",");
            
            while (token) {
                int val;
                printf(" %s: ", token);
                if (scanf("%d", &val) != 1) {
                    fprintf(stderr, "Error reading input for variable %s\n", token);
                    val = 0;  // Default value on error
                }
                set_int_value(token, val);
                token = strtok(NULL, ",");
            }
            free(args_copy);
        }
        free(clean_str);
        return idx + 1;
    }
    
    // Handle assignment and operations
    if (instr->result) {
        if (instr->op) {
            if (strcmp(instr->op, "not") == 0) {
                // Unary operation
                int val = get_int_value(instr->arg1);
                set_int_value(instr->result, !val);
            } else if (instr->arg2) {
                // Binary operation
                int left = get_int_value(instr->arg1);
                int right = get_int_value(instr->arg2);
                int result = perform_op(left, instr->op, right);
                set_int_value(instr->result, result);
            }
        } else if (instr->arg1) {
            // Simple assignment
            int val = get_int_value(instr->arg1);
            set_int_value(instr->result, val);
        }
    }
    
    return idx + 1;  // Continue to next instruction
}

// Execute the program
void execute_program() {
    printf("\n=== Program Output ===\n");
    
    // Check if we have any instructions to execute
    if (instruction_count == 0) {
        printf("Warning: No instructions to execute\n");
        return;
    }
    
    if (debug_simulation) {
        printf("DEBUG: Executing %d instructions\n", instruction_count);
    }
    
    int pc = 0;  // Program counter
    int max_iterations = 1000; // Prevent infinite loops during development
    int iteration_count = 0;
    
    while (pc < instruction_count && iteration_count < max_iterations) {
        pc = execute_instruction(pc);
        iteration_count++;
    }
    
    if (iteration_count >= max_iterations) {
        printf("\nWarning: Execution stopped after %d instructions - possible infinite loop\n", max_iterations);
    }
    
    printf("\n=== End of Program Output ===\n");
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


#line 818 "myparser.tab.c"

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
  YYSYMBOL_BEGIN_TOKEN = 3,                /* BEGIN_TOKEN  */
  YYSYMBOL_END_TOKEN = 4,                  /* END_TOKEN  */
  YYSYMBOL_PROGRAM = 5,                    /* PROGRAM  */
  YYSYMBOL_VARDECL = 6,                    /* VARDECL  */
  YYSYMBOL_IF = 7,                         /* IF  */
  YYSYMBOL_ELSE = 8,                       /* ELSE  */
  YYSYMBOL_WHILE = 9,                      /* WHILE  */
  YYSYMBOL_FOR = 10,                       /* FOR  */
  YYSYMBOL_TO = 11,                        /* TO  */
  YYSYMBOL_INC = 12,                       /* INC  */
  YYSYMBOL_DEC = 13,                       /* DEC  */
  YYSYMBOL_DO = 14,                        /* DO  */
  YYSYMBOL_PRINT = 15,                     /* PRINT  */
  YYSYMBOL_SCAN = 16,                      /* SCAN  */
  YYSYMBOL_INT_TYPE = 17,                  /* INT_TYPE  */
  YYSYMBOL_CHAR_TYPE = 18,                 /* CHAR_TYPE  */
  YYSYMBOL_MAIN = 19,                      /* MAIN  */
  YYSYMBOL_ASSIGN = 20,                    /* ASSIGN  */
  YYSYMBOL_PLUSEQ = 21,                    /* PLUSEQ  */
  YYSYMBOL_MINUSEQ = 22,                   /* MINUSEQ  */
  YYSYMBOL_MULEQ = 23,                     /* MULEQ  */
  YYSYMBOL_DIVEQ = 24,                     /* DIVEQ  */
  YYSYMBOL_MODEQ = 25,                     /* MODEQ  */
  YYSYMBOL_SLCOMMENT = 26,                 /* SLCOMMENT  */
  YYSYMBOL_MLCOMMENT = 27,                 /* MLCOMMENT  */
  YYSYMBOL_COLON = 28,                     /* COLON  */
  YYSYMBOL_SEMICOLON = 29,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 30,                     /* COMMA  */
  YYSYMBOL_LBRACKET = 31,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 32,                  /* RBRACKET  */
  YYSYMBOL_QUOTE = 33,                     /* QUOTE  */
  YYSYMBOL_DQUOTE = 34,                    /* DQUOTE  */
  YYSYMBOL_AT = 35,                        /* AT  */
  YYSYMBOL_OB = 36,                        /* OB  */
  YYSYMBOL_CB = 37,                        /* CB  */
  YYSYMBOL_MUL = 38,                       /* MUL  */
  YYSYMBOL_ADD = 39,                       /* ADD  */
  YYSYMBOL_SUB = 40,                       /* SUB  */
  YYSYMBOL_DIV = 41,                       /* DIV  */
  YYSYMBOL_MOD = 42,                       /* MOD  */
  YYSYMBOL_EQ = 43,                        /* EQ  */
  YYSYMBOL_GT = 44,                        /* GT  */
  YYSYMBOL_LT = 45,                        /* LT  */
  YYSYMBOL_GE = 46,                        /* GE  */
  YYSYMBOL_LE = 47,                        /* LE  */
  YYSYMBOL_NE = 48,                        /* NE  */
  YYSYMBOL_AND = 49,                       /* AND  */
  YYSYMBOL_OR = 50,                        /* OR  */
  YYSYMBOL_NOT = 51,                       /* NOT  */
  YYSYMBOL_DECIMAL = 52,                   /* DECIMAL  */
  YYSYMBOL_DOUBLE = 53,                    /* DOUBLE  */
  YYSYMBOL_IDENTIFIER = 54,                /* IDENTIFIER  */
  YYSYMBOL_STRING_LITERAL = 55,            /* STRING_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 56,              /* CHAR_LITERAL  */
  YYSYMBOL_INTEGER_CONST = 57,             /* INTEGER_CONST  */
  YYSYMBOL_LOWER_THAN_ELSE = 58,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_YYACCEPT = 59,                  /* $accept  */
  YYSYMBOL_program = 60,                   /* program  */
  YYSYMBOL_statementBlock = 61,            /* statementBlock  */
  YYSYMBOL_statement = 62,                 /* statement  */
  YYSYMBOL_assignment = 63,                /* assignment  */
  YYSYMBOL_assignment_op = 64,             /* assignment_op  */
  YYSYMBOL_while_loop = 65,                /* while_loop  */
  YYSYMBOL_66_1 = 66,                      /* $@1  */
  YYSYMBOL_for_stmt = 67,                  /* for_stmt  */
  YYSYMBOL_68_2 = 68,                      /* $@2  */
  YYSYMBOL_if_cond = 69,                   /* if_cond  */
  YYSYMBOL_70_3 = 70,                      /* $@3  */
  YYSYMBOL_71_4 = 71,                      /* $@4  */
  YYSYMBOL_expression = 72,                /* expression  */
  YYSYMBOL_term = 73,                      /* term  */
  YYSYMBOL_factor = 74,                    /* factor  */
  YYSYMBOL_identifier = 75,                /* identifier  */
  YYSYMBOL_bool_expr = 76,                 /* bool_expr  */
  YYSYMBOL_condition = 77,                 /* condition  */
  YYSYMBOL_relop = 78,                     /* relop  */
  YYSYMBOL_block = 79,                     /* block  */
  YYSYMBOL_for_step = 80,                  /* for_step  */
  YYSYMBOL_for_dir = 81,                   /* for_dir  */
  YYSYMBOL_print = 82,                     /* print  */
  YYSYMBOL_scan = 83,                      /* scan  */
  YYSYMBOL_formatArgs = 84,                /* formatArgs  */
  YYSYMBOL_argList = 85,                   /* argList  */
  YYSYMBOL_value = 86,                     /* value  */
  YYSYMBOL_varDecBlock = 87,               /* varDecBlock  */
  YYSYMBOL_VarDecList = 88,                /* VarDecList  */
  YYSYMBOL_VarDeclaration = 89,            /* VarDeclaration  */
  YYSYMBOL_arrayDec = 90,                  /* arrayDec  */
  YYSYMBOL_type = 91                       /* type  */
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
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   137

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  71
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  143

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   313


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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   786,   786,   789,   796,   797,   801,   802,   803,   804,
     805,   806,   807,   808,   812,   826,   827,   828,   829,   830,
     831,   836,   835,   856,   855,   907,   914,   906,   928,   929,
     933,   940,   941,   945,   949,   956,   957,   958,   959,   963,
     973,   974,   978,   982,   989,   997,   998,   999,  1000,  1001,
    1002,  1006,  1010,  1014,  1015,  1020,  1032,  1040,  1041,  1045,
    1046,  1055,  1056,  1057,  1062,  1066,  1067,  1071,  1107,  1108,
    1112,  1113
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
  "\"end of file\"", "error", "\"invalid token\"", "BEGIN_TOKEN",
  "END_TOKEN", "PROGRAM", "VARDECL", "IF", "ELSE", "WHILE", "FOR", "TO",
  "INC", "DEC", "DO", "PRINT", "SCAN", "INT_TYPE", "CHAR_TYPE", "MAIN",
  "ASSIGN", "PLUSEQ", "MINUSEQ", "MULEQ", "DIVEQ", "MODEQ", "SLCOMMENT",
  "MLCOMMENT", "COLON", "SEMICOLON", "COMMA", "LBRACKET", "RBRACKET",
  "QUOTE", "DQUOTE", "AT", "OB", "CB", "MUL", "ADD", "SUB", "DIV", "MOD",
  "EQ", "GT", "LT", "GE", "LE", "NE", "AND", "OR", "NOT", "DECIMAL",
  "DOUBLE", "IDENTIFIER", "STRING_LITERAL", "CHAR_LITERAL",
  "INTEGER_CONST", "LOWER_THAN_ELSE", "$accept", "program",
  "statementBlock", "statement", "assignment", "assignment_op",
  "while_loop", "$@1", "for_stmt", "$@2", "if_cond", "$@3", "$@4",
  "expression", "term", "factor", "identifier", "bool_expr", "condition",
  "relop", "block", "for_step", "for_dir", "print", "scan", "formatArgs",
  "argList", "value", "varDecBlock", "VarDecList", "VarDeclaration",
  "arrayDec", "type", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-95)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      19,     6,    42,    10,   -95,    30,    57,     5,    49,     5,
      48,    52,    24,    56,    62,   -95,    45,     5,    70,    71,
      72,    73,    33,    74,   -95,   -95,    68,   101,    -6,    -6,
      86,    53,    54,   -95,   102,   -95,   -95,   -95,   -95,   -95,
     -95,   -95,   -95,   -95,   -95,   -95,   -17,   -95,    58,   106,
      68,   -95,   -17,    -6,   -95,   -95,    27,    38,   -95,   -95,
     -24,   -95,    15,   -17,    81,    84,   -95,   -11,    85,   109,
     -95,    50,   -95,   -17,   -17,   -95,   -95,   -95,   -95,   -95,
     -95,   -17,   -17,   -17,   -17,   -95,    -6,    -6,   103,    -5,
      37,    82,    37,    66,    90,   -95,   -95,    38,    38,   -11,
     -95,   -95,   -95,   118,   -95,    75,   -95,   -17,   -95,   -95,
     -95,   -95,    92,    94,    88,    95,    51,   -95,   118,     4,
      37,   -95,    97,   -95,   -95,   -95,    91,   121,   -95,   -95,
     -95,   -17,   -95,   -95,   104,   118,   -11,   116,   -95,   -95,
     -95,   118,   -95
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,     4,     0,     4,
       0,     0,     0,     0,     0,    39,     0,     4,     0,     0,
       0,     0,     0,    12,    10,    11,     0,     0,     0,     0,
       0,     0,     0,     3,     0,     5,     6,     8,     9,     7,
      15,    16,    17,    18,    19,    20,     0,    13,     0,     0,
      65,    51,     0,     0,    38,    37,     0,    28,    31,    36,
       0,    40,     0,     0,    57,     0,     2,    14,    68,     0,
      66,     0,    43,     0,     0,    45,    47,    49,    48,    50,
      46,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,    35,    29,    30,    44,
      32,    33,    34,     0,    41,    42,    21,     0,    63,    62,
      61,    58,    59,     0,     0,     0,     0,    26,     0,     0,
       0,    55,     0,    69,    70,    71,     0,     0,    22,    53,
      54,     0,    60,    56,     0,     0,    52,     0,    67,    27,
      23,     0,    24
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -95,   -95,    14,   -95,   -95,   -95,   -95,   -95,   -95,   -95,
     -95,   -95,   -95,   -45,     8,    13,    -7,   -26,   -95,   -95,
     -94,   -95,   -95,   -95,   -95,   -95,   -88,   -95,   -95,    87,
     -95,   -95,   -95
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,    16,    17,    18,    46,    19,   118,    20,   141,
      21,   103,   127,    56,    57,    58,    59,    60,    61,    81,
      23,   137,   131,    24,    25,    91,   111,   112,     7,    49,
      50,    94,   126
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      22,    67,    22,    62,   114,    30,   107,    71,     9,   117,
      22,     3,    10,    85,    11,    12,   129,   130,    89,    52,
      13,    14,     1,    27,   128,    86,    87,    72,    73,    74,
      52,    35,   132,     6,    73,    74,    99,    15,     5,    54,
      55,   139,     4,    73,    74,    53,    33,   142,    15,    34,
      54,    55,    88,    40,    41,    42,    43,    44,    45,    15,
     104,   105,   119,     8,    86,    87,    73,    74,   124,   125,
      75,    76,    77,    78,    79,    80,    82,    26,    15,    83,
      84,    97,    98,   110,    28,   110,   136,    96,    29,    73,
      74,    15,    31,   108,   109,   100,   101,   102,    32,    36,
      37,    38,    39,    47,    48,    51,    63,    66,    64,    65,
      69,    90,    68,   110,    92,    95,    93,   106,   115,   113,
     116,     9,   120,   121,    86,   122,   133,   123,   134,   135,
     140,     0,     0,   138,     0,     0,     0,    70
};

static const yytype_int16 yycheck[] =
{
       7,    46,     9,    29,    92,    12,    11,    52,     3,   103,
      17,     5,     7,    37,     9,    10,    12,    13,    63,    36,
      15,    16,     3,     9,   118,    49,    50,    53,    39,    40,
      36,    17,   120,     3,    39,    40,    81,    54,    28,    56,
      57,   135,     0,    39,    40,    51,     1,   141,    54,     4,
      56,    57,    37,    20,    21,    22,    23,    24,    25,    54,
      86,    87,   107,     6,    49,    50,    39,    40,    17,    18,
      43,    44,    45,    46,    47,    48,    38,    28,    54,    41,
      42,    73,    74,    90,    36,    92,   131,    37,    36,    39,
      40,    54,    36,    56,    57,    82,    83,    84,    36,    29,
      29,    29,    29,    29,    36,     4,    20,     5,    55,    55,
       4,    30,    54,   120,    30,     6,    31,    14,    52,    37,
      30,     3,    30,    29,    49,    37,    29,    32,    37,     8,
      14,    -1,    -1,    29,    -1,    -1,    -1,    50
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    60,     5,     0,    28,     3,    87,     6,     3,
       7,     9,    10,    15,    16,    54,    61,    62,    63,    65,
      67,    69,    75,    79,    82,    83,    28,    61,    36,    36,
      75,    36,    36,     1,     4,    61,    29,    29,    29,    29,
      20,    21,    22,    23,    24,    25,    64,    29,    36,    88,
      89,     4,    36,    51,    56,    57,    72,    73,    74,    75,
      76,    77,    76,    20,    55,    55,     5,    72,    54,     4,
      88,    72,    76,    39,    40,    43,    44,    45,    46,    47,
      48,    78,    38,    41,    42,    37,    49,    50,    37,    72,
      30,    84,    30,    31,    90,     6,    37,    73,    73,    72,
      74,    74,    74,    70,    76,    76,    14,    11,    56,    57,
      75,    85,    86,    37,    85,    52,    30,    79,    66,    72,
      30,    29,    37,    32,    17,    18,    91,    71,    79,    12,
      13,    81,    85,    29,    37,     8,    72,    80,    29,    79,
      14,    68,    79
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    59,    60,    60,    61,    61,    62,    62,    62,    62,
      62,    62,    62,    62,    63,    64,    64,    64,    64,    64,
      64,    66,    65,    68,    67,    70,    71,    69,    72,    72,
      72,    73,    73,    73,    73,    74,    74,    74,    74,    75,
      76,    76,    76,    76,    77,    78,    78,    78,    78,    78,
      78,    79,    80,    81,    81,    82,    83,    84,    84,    85,
      85,    86,    86,    86,    87,    88,    88,    89,    90,    90,
      91,    91
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     7,     6,     0,     2,     2,     2,     2,     2,
       1,     1,     1,     2,     3,     1,     1,     1,     1,     1,
       1,     0,     7,     0,    11,     0,     0,     9,     1,     3,
       3,     1,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     3,     3,     2,     3,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     6,     7,     0,     2,     1,
       3,     1,     1,     1,     6,     1,     2,     7,     0,     3,
       1,     1
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
  case 2: /* program: BEGIN_TOKEN PROGRAM COLON varDecBlock statementBlock END_TOKEN PROGRAM  */
#line 786 "myparser.y"
                                                                             {
        printf("Parsing and TAC generation completed successfully.\n");
    }
#line 1998 "myparser.tab.c"
    break;

  case 3: /* program: BEGIN_TOKEN PROGRAM COLON varDecBlock statementBlock error  */
#line 789 "myparser.y"
                                                                 {
        printf("Warning: Syntax error near the end of the program. TAC generation may be incomplete.\n");
        yyerrok;
    }
#line 2007 "myparser.tab.c"
    break;

  case 14: /* assignment: identifier assignment_op expression  */
#line 812 "myparser.y"
                                          {
        if (strcmp((yyvsp[-1].sval), ":=") == 0) {
            emit((yyvsp[-2].sval), (yyvsp[0].sval), NULL, NULL);
        } else {
            char* baseOp = getBaseOperator((yyvsp[-1].sval));
            char* temp = newTemp();
            emit(temp, (yyvsp[-2].sval), baseOp, (yyvsp[0].sval));
            emit((yyvsp[-2].sval), temp, NULL, NULL);
        }
        (yyval.sval) = strdup((yyvsp[-2].sval)); track_allocation((yyval.sval));
    }
#line 2023 "myparser.tab.c"
    break;

  case 15: /* assignment_op: ASSIGN  */
#line 826 "myparser.y"
              { (yyval.sval) = strdup(":="); track_allocation((yyval.sval)); }
#line 2029 "myparser.tab.c"
    break;

  case 16: /* assignment_op: PLUSEQ  */
#line 827 "myparser.y"
              { (yyval.sval) = strdup("+="); track_allocation((yyval.sval)); }
#line 2035 "myparser.tab.c"
    break;

  case 17: /* assignment_op: MINUSEQ  */
#line 828 "myparser.y"
              { (yyval.sval) = strdup("-="); track_allocation((yyval.sval)); }
#line 2041 "myparser.tab.c"
    break;

  case 18: /* assignment_op: MULEQ  */
#line 829 "myparser.y"
              { (yyval.sval) = strdup("*="); track_allocation((yyval.sval)); }
#line 2047 "myparser.tab.c"
    break;

  case 19: /* assignment_op: DIVEQ  */
#line 830 "myparser.y"
              { (yyval.sval) = strdup("/="); track_allocation((yyval.sval)); }
#line 2053 "myparser.tab.c"
    break;

  case 20: /* assignment_op: MODEQ  */
#line 831 "myparser.y"
              { (yyval.sval) = strdup("%="); track_allocation((yyval.sval)); }
#line 2059 "myparser.tab.c"
    break;

  case 21: /* $@1: %empty  */
#line 836 "myparser.y"
      {
        /* 1) at "do", generate labels & test */
        char* start = newLabel();  track_allocation(start);
        char* end   = newLabel();  track_allocation(end);
        current_loop_start = start;
        current_loop_end   = end;
        
        emitLabel(start);            /* L1: */
        emitCondJump((yyvsp[-2].sval), end);       /* if t_cond1 == 0 goto L2 */
      }
#line 2074 "myparser.tab.c"
    break;

  case 22: /* while_loop: WHILE OB bool_expr CB DO $@1 block  */
#line 847 "myparser.y"
      {
        /* 2) after the block, loop back & close */
        emitGoto(current_loop_start);  
        emitLabel(current_loop_end);  /* L2: */
      }
#line 2084 "myparser.tab.c"
    break;

  case 23: /* $@2: %empty  */
#line 856 "myparser.y"
      {
        /* 1) init loop var */
        emit((yyvsp[-7].sval), (yyvsp[-5].sval), NULL, NULL);             /* a := (10, 8) */

        /* 2) blank and loop‐start label */
        char* start = newLabel();  track_allocation(start);
        char* end   = newLabel();  track_allocation(end);
        current_for_start = start;
        current_for_end   = end;
        emitLabel(start);  

        /* 3) compute bound (t3) and step (t4) */
        char* bound = newTemp();
        emit(bound, (yyvsp[-3].sval), NULL, NULL);          /* t3 := b + (10, 2) */
        current_for_step_temp = newTemp();
        emit(current_for_step_temp, (yyvsp[-1].sval), NULL, NULL);                              /* t4 := (1, 10) */

        /* 4) test and conditional jump - handle inc/dec direction */
        char* cond = newCondTemp();
        if (strcmp((yyvsp[-2].sval), "inc") == 0) {
            emit(cond, (yyvsp[-7].sval), ">", bound);           /* t_cond2 := a > t3 */
        } else {
            emit(cond, (yyvsp[-7].sval), "<", bound);           /* t_cond2 := a < t3 */
        }
        emitCondJumpTrue(cond, end);          /* if t_cond2 == 1 goto L4 */
      }
#line 2115 "myparser.tab.c"
    break;

  case 24: /* for_stmt: FOR identifier ASSIGN expression TO expression for_dir for_step DO $@2 block  */
#line 883 "myparser.y"
      {
        /* 5) body already emitted by `block` */

        /* 6) increment by saved step temp based on direction */
        char* tmp = newTemp();
        char* dir = strdup(current_for_step_temp);  // Save the direction
        track_allocation(dir);
        
        // Check the for_dir type
        if (strstr(dir, "inc")) {
            emit(tmp, (yyvsp[-9].sval), "+", current_for_step_temp);  
        } else {
            emit(tmp, (yyvsp[-9].sval), "-", current_for_step_temp);  
        }
        emit((yyvsp[-9].sval), tmp, NULL, NULL);             /* a := tmp */

        /* 7) back‐edge and end label */
        emitGoto(current_for_start);
        emitLabel(current_for_end);            /* L4: */
      }
#line 2140 "myparser.tab.c"
    break;

  case 25: /* $@3: %empty  */
#line 907 "myparser.y"
      {
        /* 1) at “)”, make labels and emit the conditional jump */
        __if_false = newLabel();    track_allocation(__if_false);
        __if_end   = newLabel();    track_allocation(__if_end);
        emitCondJump((yyvsp[-1].sval), __if_false);  
      }
#line 2151 "myparser.tab.c"
    break;

  case 26: /* $@4: %empty  */
#line 914 "myparser.y"
      {
        /* 2) right after THEN block */
        emitGoto(__if_end);
        emitLabel(__if_false);  
      }
#line 2161 "myparser.tab.c"
    break;

  case 27: /* if_cond: IF OB bool_expr CB $@3 block $@4 ELSE block  */
#line 921 "myparser.y"
      {
        /* 3) after ELSE block */
        emitLabel(__if_end);
      }
#line 2170 "myparser.tab.c"
    break;

  case 28: /* expression: term  */
#line 928 "myparser.y"
                             { (yyval.sval) = (yyvsp[0].sval); }
#line 2176 "myparser.tab.c"
    break;

  case 29: /* expression: expression ADD term  */
#line 929 "myparser.y"
                             {
        char* temp = newTemp();
        emit(temp, (yyvsp[-2].sval), "+", (yyvsp[0].sval)); (yyval.sval) = temp;
    }
#line 2185 "myparser.tab.c"
    break;

  case 30: /* expression: expression SUB term  */
#line 933 "myparser.y"
                             {
        char* temp = newTemp();
        emit(temp, (yyvsp[-2].sval), "-", (yyvsp[0].sval)); (yyval.sval) = temp;
    }
#line 2194 "myparser.tab.c"
    break;

  case 31: /* term: factor  */
#line 940 "myparser.y"
                            { (yyval.sval) = (yyvsp[0].sval); }
#line 2200 "myparser.tab.c"
    break;

  case 32: /* term: term MUL factor  */
#line 941 "myparser.y"
                            {
        char* temp = newTemp();
        emit(temp, (yyvsp[-2].sval), "*", (yyvsp[0].sval)); (yyval.sval) = temp;
    }
#line 2209 "myparser.tab.c"
    break;

  case 33: /* term: term DIV factor  */
#line 945 "myparser.y"
                            {
        char* temp = newTemp();
        emit(temp, (yyvsp[-2].sval), "/", (yyvsp[0].sval)); (yyval.sval) = temp;
    }
#line 2218 "myparser.tab.c"
    break;

  case 34: /* term: term MOD factor  */
#line 949 "myparser.y"
                            {
        char* temp = newTemp();
        emit(temp, (yyvsp[-2].sval), "%", (yyvsp[0].sval)); (yyval.sval) = temp;
    }
#line 2227 "myparser.tab.c"
    break;

  case 35: /* factor: OB expression CB  */
#line 956 "myparser.y"
                             { (yyval.sval) = (yyvsp[-1].sval); }
#line 2233 "myparser.tab.c"
    break;

  case 36: /* factor: identifier  */
#line 957 "myparser.y"
                             { (yyval.sval) = strdup((yyvsp[0].sval)); track_allocation((yyval.sval)); }
#line 2239 "myparser.tab.c"
    break;

  case 37: /* factor: INTEGER_CONST  */
#line 958 "myparser.y"
                             { (yyval.sval) = strdup((yyvsp[0].sval)); track_allocation((yyval.sval)); }
#line 2245 "myparser.tab.c"
    break;

  case 38: /* factor: CHAR_LITERAL  */
#line 959 "myparser.y"
                             { (yyval.sval) = (yyvsp[0].sval); }
#line 2251 "myparser.tab.c"
    break;

  case 39: /* identifier: IDENTIFIER  */
#line 963 "myparser.y"
                 {
        if (!is_declared((yyvsp[0].sval))) {
            fprintf(stderr, "Semantic error: variable '%s' used before declaration.\n", (yyvsp[0].sval));
            exit(1);
        }
        (yyval.sval) = strdup((yyvsp[0].sval)); track_allocation((yyval.sval));
    }
#line 2263 "myparser.tab.c"
    break;

  case 40: /* bool_expr: condition  */
#line 973 "myparser.y"
                { (yyval.sval) = (yyvsp[0].sval); }
#line 2269 "myparser.tab.c"
    break;

  case 41: /* bool_expr: bool_expr AND bool_expr  */
#line 974 "myparser.y"
                              {
        char* temp = newTemp();
        emit(temp, (yyvsp[-2].sval), "and", (yyvsp[0].sval)); (yyval.sval) = temp;
    }
#line 2278 "myparser.tab.c"
    break;

  case 42: /* bool_expr: bool_expr OR bool_expr  */
#line 978 "myparser.y"
                             {
        char* temp = newTemp();
        emit(temp, (yyvsp[-2].sval), "or", (yyvsp[0].sval)); (yyval.sval) = temp;
    }
#line 2287 "myparser.tab.c"
    break;

  case 43: /* bool_expr: NOT bool_expr  */
#line 982 "myparser.y"
                    {
        char* temp = newTemp();
        emit(temp, "not", (yyvsp[0].sval), NULL); (yyval.sval) = temp;
    }
#line 2296 "myparser.tab.c"
    break;

  case 44: /* condition: expression relop expression  */
#line 989 "myparser.y"
                                {
      char* tmp = newCondTemp();      
      emit(tmp, (yyvsp[-2].sval), (yyvsp[-1].sval), (yyvsp[0].sval));          /* t_cond1 := lhs relop rhs */
      (yyval.sval) = tmp;
  }
#line 2306 "myparser.tab.c"
    break;

  case 45: /* relop: EQ  */
#line 997 "myparser.y"
         { (yyval.sval) = strdup("="); track_allocation((yyval.sval)); }
#line 2312 "myparser.tab.c"
    break;

  case 46: /* relop: NE  */
#line 998 "myparser.y"
         { (yyval.sval) = strdup("!="); track_allocation((yyval.sval)); }
#line 2318 "myparser.tab.c"
    break;

  case 47: /* relop: GT  */
#line 999 "myparser.y"
         { (yyval.sval) = strdup(">"); track_allocation((yyval.sval)); }
#line 2324 "myparser.tab.c"
    break;

  case 48: /* relop: GE  */
#line 1000 "myparser.y"
         { (yyval.sval) = strdup(">="); track_allocation((yyval.sval)); }
#line 2330 "myparser.tab.c"
    break;

  case 49: /* relop: LT  */
#line 1001 "myparser.y"
         { (yyval.sval) = strdup("<"); track_allocation((yyval.sval)); }
#line 2336 "myparser.tab.c"
    break;

  case 50: /* relop: LE  */
#line 1002 "myparser.y"
         { (yyval.sval) = strdup("<="); track_allocation((yyval.sval)); }
#line 2342 "myparser.tab.c"
    break;

  case 52: /* for_step: expression  */
#line 1010 "myparser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 2348 "myparser.tab.c"
    break;

  case 53: /* for_dir: INC  */
#line 1014 "myparser.y"
          { (yyval.sval) = strdup("inc"); track_allocation((yyval.sval)); }
#line 2354 "myparser.tab.c"
    break;

  case 54: /* for_dir: DEC  */
#line 1015 "myparser.y"
          { (yyval.sval) = strdup("dec"); track_allocation((yyval.sval)); }
#line 2360 "myparser.tab.c"
    break;

  case 55: /* print: PRINT OB STRING_LITERAL formatArgs CB SEMICOLON  */
#line 1020 "myparser.y"
                                                      {
        if ((yyvsp[-2].sval)) {
            printf("print \"%s\", %s\n", (yyvsp[-3].sval), (yyvsp[-2].sval));
            emitPrint((yyvsp[-3].sval), (yyvsp[-2].sval));
        } else {
            printf("print \"%s\"\n", (yyvsp[-3].sval));
            emitPrint((yyvsp[-3].sval), NULL);
        }
    }
#line 2374 "myparser.tab.c"
    break;

  case 56: /* scan: SCAN OB STRING_LITERAL COMMA argList CB SEMICOLON  */
#line 1032 "myparser.y"
                                                        {
        printf("scan \"%s\"\n", (yyvsp[-4].sval));
        if ((yyvsp[-2].sval)) printf("vars: %s\n", (yyvsp[-2].sval)); // variables list
        emitScan((yyvsp[-4].sval), (yyvsp[-2].sval));
    }
#line 2384 "myparser.tab.c"
    break;

  case 57: /* formatArgs: %empty  */
#line 1040 "myparser.y"
                  { (yyval.sval) = NULL; }
#line 2390 "myparser.tab.c"
    break;

  case 58: /* formatArgs: COMMA argList  */
#line 1041 "myparser.y"
                    { (yyval.sval) = (yyvsp[0].sval); }
#line 2396 "myparser.tab.c"
    break;

  case 59: /* argList: value  */
#line 1045 "myparser.y"
            { (yyval.sval) = strdup((yyvsp[0].sval)); track_allocation((yyval.sval)); }
#line 2402 "myparser.tab.c"
    break;

  case 60: /* argList: value COMMA argList  */
#line 1046 "myparser.y"
                          {
        char* temp = malloc(strlen((yyvsp[-2].sval)) + strlen((yyvsp[0].sval)) + 2);
        sprintf(temp, "%s,%s", (yyvsp[-2].sval), (yyvsp[0].sval));
        track_allocation(temp);
        (yyval.sval) = temp;
    }
#line 2413 "myparser.tab.c"
    break;

  case 61: /* value: identifier  */
#line 1055 "myparser.y"
                 { (yyval.sval) = (yyvsp[0].sval); }
#line 2419 "myparser.tab.c"
    break;

  case 62: /* value: INTEGER_CONST  */
#line 1056 "myparser.y"
                    { (yyval.sval) = (yyvsp[0].sval); }
#line 2425 "myparser.tab.c"
    break;

  case 63: /* value: CHAR_LITERAL  */
#line 1057 "myparser.y"
                    { (yyval.sval) = (yyvsp[0].sval); }
#line 2431 "myparser.tab.c"
    break;

  case 67: /* VarDeclaration: OB IDENTIFIER arrayDec COMMA type CB SEMICOLON  */
#line 1071 "myparser.y"
                                                     {
        // Handle array declaration
        int is_array = 0;
        int array_size = 0;
        char* base_name = strdup((yyvsp[-5].sval));
        
        // Check for the brackets in the identifier
        char* bracket_pos = strchr(base_name, '[');
        if (bracket_pos != NULL) {
            *bracket_pos = '\0';  // Terminate the string at the bracket
            is_array = 1;
            // Extract array size if available
            char* size_str = bracket_pos + 1;
            char* close_bracket = strchr(size_str, ']');
            if (close_bracket) {
                *close_bracket = '\0';
                array_size = atoi(size_str);
            }
        }
        
        // Declare the variable
        declare_variable(base_name);
        
        // If it's an array, update the symbol table entry
        if (is_array) {
            int idx = find_symbol(base_name);
            if (idx >= 0) {
                symbol_table[idx].is_array = 1;
                symbol_table[idx].array_size = array_size;
            }
        }
        free(base_name);
    }
#line 2469 "myparser.tab.c"
    break;


#line 2473 "myparser.tab.c"

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

#line 1116 "myparser.y"


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

    if (yyparse() == 0) {
        printf("\nParsing completed successfully.\n");
        
        // Initialize temporary variables for simulation
        for (int i = 0; i < MAX_TEMP_VARS; i++) {
            temp_vars[i].initialized = 0;
            temp_vars[i].type = TYPE_UNKNOWN;
        }
        
        printf("\nStarting program simulation...\n");
        // Execute the program
        execute_program();
        
        // Print the symbol table
        printf("\nPrinting symbol table...\n");
        print_symbol_table();
    }

    fclose(yyin);
    cleanup_memory();
    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, s);
    cleanup_memory();
    exit(1);
}
