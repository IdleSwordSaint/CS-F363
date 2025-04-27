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
    
    // Convert from the given base to decimal
    if (base == 2 || base == 8) {
        int decimal_value = 0;
        int digit_value;
        for (int i = 0; copy[i] != '\0'; i++) {
            digit_value = copy[i] - '0';
            decimal_value = decimal_value * base + digit_value;
        }
        value = decimal_value;
    }
    
    free(copy);
    return value;
}

// Get integer value of a variable or literal
int get_int_value(const char* name) {
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

// Execute a single instruction
int execute_instruction(int idx) {
    Instruction* instr = &instructions[idx];
    
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
            
            while (*fmt && val_idx < arg_count) {
                if (*fmt == '%' && *(fmt + 1) == 'd') {
                    int chars = sprintf(out_ptr, "%d", values[val_idx++]);
                    out_ptr += chars;
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
    
    int pc = 0;  // Program counter
    while (pc < instruction_count) {
        pc = execute_instruction(pc);
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
    | BEGIN_TOKEN PROGRAM COLON varDecBlock statementBlock error {
        printf("Warning: Syntax error near the end of the program. TAC generation may be incomplete.\n");
        yyerrok;
    }
    ;
statementBlock
    : /* empty */
    | statement statementBlock
    ;

statement
    : assignment SEMICOLON
    | if_cond SEMICOLON
    | while_loop SEMICOLON
    | for_stmt SEMICOLON
    | print
    | scan
    | block
    | block SEMICOLON  // Optional semicolon after blocks
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



while_loop
  : WHILE OB bool_expr CB DO
      {
        /* 1) at “do”, generate labels & test */
        char* start = newLabel();  track_allocation(start);
        char* end   = newLabel();  track_allocation(end);
        current_loop_start = start;
        current_loop_end   = end;

        emitLabel(start);            /* L1: */
        emitCondJump($3, end);       /* if t_cond1 == 0 goto L2 */
      }
    block
      {
        /* 2) after the block, loop back & close */
        emitGoto(current_loop_start);  
        emitLabel(current_loop_end);  /* L2: */
      }
  ;



// Correct implementation for for loop
for_stmt
  : FOR identifier ASSIGN expression
        TO expression for_dir for_step DO
      {
        /* 1) init loop var */
        emit($2, $4, NULL, NULL);             /* a := (10, 8) */

        /* 2) blank and loop‐start label */
        printf("\n");
        char* start = newLabel();  track_allocation(start);
        char* end   = newLabel();  track_allocation(end);
        current_for_start = start;
        current_for_end   = end;
        emitLabel(start);  
        printf("\n");

        /* 3) compute bound (t3) and step (t4) */
        char* bound = newTemp();
        emit(bound, $6, NULL, NULL);          /* t3 := b + (10, 2) */

        current_for_step_temp = newTemp();
        emit(current_for_step_temp, $8, NULL, NULL);  
                                              /* t4 := (1, 10) */
        printf("\n");

        /* 4) test and conditional jump */
        char* cond = newCondTemp();
        emit(cond, $2, ">", bound);           /* t_cond2 := a > t3 */
        emitCondJumpTrue(cond, end);          /* if t_cond2 == 1 goto L4 */
        printf("\n");
      }
    block
      {
        /* 5) body already emitted by `block` */

        /* 6) increment by saved step temp */
        char* tmp = newTemp();
        emit(tmp, $2, "+", current_for_step_temp);  
        emit($2, tmp, NULL, NULL);             /* a := tmp */
        printf("\n");

        /* 7) back‐edge and end label */
        emitGoto(current_for_start);
        emitLabel(current_for_end);            /* L4: */
        printf("\n");
      }
  ;




// -- IF-ELSE Statement --
if_cond
  : IF OB bool_expr CB
      { 
        /* 1) at “)”, make labels and emit the conditional jump */
        __if_false = newLabel();    track_allocation(__if_false);
        __if_end   = newLabel();    track_allocation(__if_end);
        emitCondJump($3, __if_false);  
      }
    block
      {
        /* 2) right after THEN block */
        emitGoto(__if_end);
        emitLabel(__if_false);  
      }
    ELSE
    block
      {
        /* 3) after ELSE block */
        emitLabel(__if_end);
      }
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
      char* tmp = newCondTemp();      
      emit(tmp, $1, $2, $3);          /* t_cond1 := lhs relop rhs */
      $$ = tmp;
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

// Fix for the while loop implementation
// Correct implementation for while loop

// -- PRINT and SCAN --

print
    : PRINT OB STRING_LITERAL formatArgs CB SEMICOLON {
        if ($4) {
            printf("print \"%s\", %s\n", $3, $4);
            emitPrint($3, $4);
        } else {
            printf("print \"%s\"\n", $3);
            emitPrint($3, NULL);
        }
    }
    ;

scan
    : SCAN OB STRING_LITERAL COMMA argList CB SEMICOLON {
        printf("scan \"%s\"\n", $3);
        if ($5) printf("vars: %s\n", $5); // variables list
        emitScan($3, $5);
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
        // Handle array declaration
        int is_array = 0;
        int array_size = 0;
        char* base_name = strdup($2);
        
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
        printf("Parsing completed successfully.\n");
        
        // Initialize temporary variables for simulation
        for (int i = 0; i < MAX_TEMP_VARS; i++) {
            temp_vars[i].initialized = 0;
            temp_vars[i].type = TYPE_UNKNOWN;
        }
        
        // Execute the program
        execute_program();
        
        // Print the symbol table
        print_symbol_table();
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
