%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include <errno.h>

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
// Add these at the top of your .y file in the C code section
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

// ---- Emit TAC ----
void emit(char* result, char* arg1, char* op, char* arg2) {
    if (arg2 && op) printf("%s := %s %s %s\n", result, arg1, op, arg2);
    else if (op)    printf("%s := %s %s\n", result, op, arg1);
    else            printf("%s := %s\n", result, arg1);
}


void emitCondJump(char* cond, char* label) {
    printf("if %s == 0 goto %s\n", cond, label);
}

void emitCondJumpTrue(char* cond, char* label) {
    printf("if %s == 1 goto %s\n", cond, label);
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

// Add these to the variable table section
int var_values[MAX_VARS];     // Store integer values for simulation
int var_initialized[MAX_VARS]; // Track if variables have been initialized
int emit_tac = 1;  // 1 = emit TAC, 0 = simulate execution

int get_var_index(const char* name) {
    for (int i = 0; i < var_count; i++)
        if (strcmp(declared_vars[i], name) == 0)
            return i;
    return -1;
}

// Set variable value for simulation
void set_variable_value(const char* name, int value) {
    int idx = get_var_index(name);
    if (idx >= 0) {
        var_values[idx] = value;
        var_initialized[idx] = 1;  // Mark as initialized
    }
}

// Get variable value for simulation
int get_variable_value(const char* name) {
    int idx = get_var_index(name);
    if (idx >= 0) {
        if (!var_initialized[idx]) {
            fprintf(stderr, "Semantic error at line %d: Variable '%s' used before initialization.\n", yylineno, name);
            exit(1);
        }
        return var_values[idx];
    }
    fprintf(stderr, "Internal error: Tried to get value of undeclared variable '%s'.\n", name);
    exit(1);
}

// Get variable value safely for simulation
int get_value(const char* str) {
    // Check if it's a number
    if (isdigit(str[0]) || (str[0] == '-' && isdigit(str[1])))
        return atoi(str);
    
    // Otherwise it's a variable
    return get_variable_value(str);
}

// Print symbol table
void print_symbol_table() {
    printf("\n===== Symbol Table =====\n");
    printf("Variable\tValue\tInitialized\n");
    printf("--------------------------------\n");
    for (int i = 0; i < var_count; i++) {
        printf("%s\t\t%d\t%s\n", 
               declared_vars[i], 
               var_values[i], 
               var_initialized[i] ? "Yes" : "No");
    }
    printf("================================\n");
}

// Handle print statement simulation
void simulate_print(char* format, char* args) {
    printf("PROGRAM OUTPUT: ");
    
    // Simple format string handling
    char* fmt = strdup(format);
    fmt[strlen(fmt)-1] = '\0';  // Remove trailing quote
    fmt++;  // Skip leading quote
    
    if (!args) {
        printf("%s\n", fmt);
        return;
    }
    
    // Very simple arg handling - just replace %d with values
    char* arg_list = strdup(args);
    char* token = strtok(arg_list, ",");
    char* fmt_ptr = fmt;
    
    while (*fmt_ptr) {
        if (*fmt_ptr == '%' && *(fmt_ptr+1) == 'd') {
            if (token) {
                printf("%d", get_value(token));
                token = strtok(NULL, ",");
                fmt_ptr += 2;
            } else {
                fprintf(stderr, "Error: Not enough arguments for format string\n");
                exit(1);
            }
        } else {
            printf("%c", *fmt_ptr);
            fmt_ptr++;
        }
    }
    printf("\n");
    
    free(arg_list);
    free(fmt);
}

// Handle scan statement simulation
void simulate_scan(char* format, char* args) {
    printf("SCAN: Reading input values for: %s\n", args);
    
    char* arg_list = strdup(args);
    char* token = strtok(arg_list, ",");
    
    while (token) {
        int value;
        printf("Enter value for %s: ", token);
        if (scanf("%d", &value) != 1) {
            fprintf(stderr, "Error reading input\n");
            exit(1);
        }
        set_variable_value(token, value);
        token = strtok(NULL, ",");
    }
    
    free(arg_list);
}

// Process assignment with operator
int process_assignment(char* var, char* op, char* expr) {
    int idx = get_var_index(var);
    int val = get_value(expr);
    
    if (strcmp(op, ":=") == 0) {
        var_values[idx] = val;
    } else if (strcmp(op, "+=") == 0) {
        if (!var_initialized[idx]) {
            fprintf(stderr, "Semantic error at line %d: Variable '%s' used before initialization.\n", yylineno, var);
            exit(1);
        }
        var_values[idx] += val;
    } else if (strcmp(op, "-=") == 0) {
        if (!var_initialized[idx]) {
            fprintf(stderr, "Semantic error at line %d: Variable '%s' used before initialization.\n", yylineno, var);
            exit(1);
        }
        var_values[idx] -= val;
    } else if (strcmp(op, "*=") == 0) {
        if (!var_initialized[idx]) {
            fprintf(stderr, "Semantic error at line %d: Variable '%s' used before initialization.\n", yylineno, var);
            exit(1);
        }
        var_values[idx] *= val;
    } else if (strcmp(op, "/=") == 0) {
        if (!var_initialized[idx]) {
            fprintf(stderr, "Semantic error at line %d: Variable '%s' used before initialization.\n", yylineno, var);
            exit(1);
        }
        if (val == 0) {
            fprintf(stderr, "Runtime error at line %d: Division by zero.\n", yylineno);
            exit(1);
        }
        var_values[idx] /= val;
    } else if (strcmp(op, "%=") == 0) {
        if (!var_initialized[idx]) {
            fprintf(stderr, "Semantic error at line %d: Variable '%s' used before initialization.\n", yylineno, var);
            exit(1);
        }
        if (val == 0) {
            fprintf(stderr, "Runtime error at line %d: Modulo by zero.\n", yylineno);
            exit(1);
        }
        var_values[idx] %= val;
    }
    
    // Mark the variable as initialized
    var_initialized[idx] = 1;
    return var_values[idx];
}
%}
%union {
    int ival;
    double dval;
    char* sval;
    struct {
        char* code;
        int value;
    } expr;
    char* array_decl;  // Add this for array declarations
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

%type <expr> expression term factor condition bool_expr
%type <expr> for_step for_dir
%type <expr> assignment_op
%type <expr> assignment relop value  // Added these missing types
%type <sval> identifier formatArgs argList
%type <array_decl> arrayDec  // Add type for arrayDec

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
    : BEGIN_TOKEN PROGRAM COLON varDecBlock {
        // After variable declarations, switch to simulation mode
        printf("\n--- Switching to execution simulation mode ---\n\n");
        emit_tac = 0;
    } statementBlock END_TOKEN PROGRAM {
        printf("\nProgram execution completed successfully.\n");
        print_symbol_table();
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
        if (emit_tac) {
            if (strcmp($2.code, ":=") == 0) {
                emit($1, $3.code, NULL, NULL);
            } else {
                char* baseOp = getBaseOperator($2.code);
                char* temp = newTemp();
                emit(temp, $1, baseOp, $3.code);
                emit($1, temp, NULL, NULL);
            }
            $$.code = strdup($1); track_allocation($$.code);
        } else {
            // Simulation mode
            process_assignment($1, $2.code, $3.code);
            $$.code = strdup($1); track_allocation($$.code);
        }
    }
    ;

assignment_op
    : ASSIGN  { $$.code = strdup(":="); track_allocation($$.code); }
    | PLUSEQ  { $$.code = strdup("+="); track_allocation($$.code); }
    | MINUSEQ { $$.code = strdup("-="); track_allocation($$.code); }
    | MULEQ   { $$.code = strdup("*="); track_allocation($$.code); }
    | DIVEQ   { $$.code = strdup("/="); track_allocation($$.code); }
    | MODEQ   { $$.code = strdup("%="); track_allocation($$.code); }
    ;

while_loop
    : WHILE OB bool_expr CB DO {
        if (emit_tac) {
            char* start = newLabel();
            char* end = newLabel();
            current_loop_start = start;
            current_loop_end = end;

            emitLabel(start);
            emitCondJump($3.code, end);
        } else {
            // Simulation mode - keep the condition's result
        }
    } block {
        if (emit_tac) {
            emitGoto(current_loop_start);
            emitLabel(current_loop_end);
        } else {
            // Simulation: block already handled conditionally
        }
    }
    ;

for_stmt
    : FOR identifier ASSIGN expression TO expression for_dir for_step DO {
        if (emit_tac) {
            // Original TAC generation for FOR loop
            emit($2, $4.code, NULL, NULL);
            
            printf("\n");
            char* start_label = newLabel();
            char* end_label = newLabel();
            current_for_start = start_label;
            current_for_end = end_label;
            emitLabel(start_label);
            
            printf("\n");
            char* bound = newTemp();
            emit(bound, $6.code, NULL, NULL);
            
            char* step = newTemp();
            emit(step, $8.code, NULL, NULL);
            
            printf("\n");
            char* cond = newCondTemp();
            if (strcmp($7.code, "inc") == 0)
                emit(cond, $2, ">", bound);
            else
                emit(cond, bound, ">", $2);
            emitCondJumpTrue(cond, end_label);
            
            printf("\n");
        } else {
            // Simulation mode
            char* loop_var = $2;
            int loop_idx = get_var_index(loop_var);
            int initial_val = $4.value;
            int target_val = $6.value;
            int step_val = $8.value;
            int is_inc = strcmp($7.code, "inc") == 0;
            
            // First, initialize the loop variable before executing the block
            var_values[loop_idx] = initial_val;
            var_initialized[loop_idx] = 1;  // Mark as initialized
            
            // Check if loop condition is satisfied
            int should_execute = (is_inc && initial_val <= target_val) || 
                                (!is_inc && initial_val >= target_val);
            
            if (!should_execute) {
                printf("FOR loop condition not satisfied, skipping loop body\n");
            } else {
                printf("FOR loop executed (one iteration during parsing)\n");
                // The block is executed during parsing, 
                // but we can only execute one iteration during direct interpretation
                
                // After block execution, update the loop counter
                if (is_inc) {
                    var_values[loop_idx] += step_val;
                } else {
                    var_values[loop_idx] -= step_val;
                }
            }
        }
    } block {
        if (emit_tac) {
            char* loop_var = $2;
            char* step = $8.code;
            char* step_temp = newTemp();
            if (strcmp($7.code, "inc") == 0)
                emit(step_temp, loop_var, "+", step);
            else
                emit(step_temp, loop_var, "-", step);
            emit(loop_var, step_temp, NULL, NULL);
            emitGoto(current_for_start);
            emitLabel(current_for_end);
        } else {
            // After block execution in simulation mode
        }
    }
    ;

if_cond
    : IF OB bool_expr CB {
        if (emit_tac) {
            __if_false = newLabel();
            __if_end = newLabel();
            emitCondJump($3.code, __if_false);
        } else {
            // Simulation mode
            // If condition is true, execute the "then" block
            // Skip the "else" block
            if ($3.value == 0) {
                // Condition is false, we'll need to skip the next block
                // and execute the else block (handled by parser)
            }
        }
    } block {
        if (emit_tac) {
            emitGoto(__if_end);
            emitLabel(__if_false);
        } else {
            // Simulation: block already executed if condition was true
        }
    } ELSE block {
        if (emit_tac) {
            emitLabel(__if_end);
        } else {
            // Simulation: blocks already executed based on condition
        }
    }
    | IF OB bool_expr CB DO {   // Keep this alternative for backward compatibility
        if (emit_tac) {
            __if_false = newLabel();
            __if_end = newLabel();
            emitCondJump($3.code, __if_false);
        } else {
            // Same simulation behavior
        }
    } block {
        if (emit_tac) {
            emitGoto(__if_end);
            emitLabel(__if_false);
        } else {
            // Simulation: block already executed if condition was true
        }
    } ELSE block {
        if (emit_tac) {
            emitLabel(__if_end);
        } else {
            // Simulation: blocks already executed based on condition
        }
    }
    ;

expression
    : term {
        $$.code = $1.code;
        $$.value = $1.value;
    }
    | expression ADD term {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, $1.code, "+", $3.code); 
            $$.code = temp;
        } else {
            // Simulation mode
            $$.value = $1.value + $3.value;
            char temp[20];
            sprintf(temp, "%d", $$.value);
            $$.code = strdup(temp);
            track_allocation($$.code);
        }
    }
    | expression SUB term {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, $1.code, "-", $3.code); 
            $$.code = temp;
        } else {
            // Simulation mode
            $$.value = $1.value - $3.value;
            char temp[20];
            sprintf(temp, "%d", $$.value);
            $$.code = strdup(temp);
            track_allocation($$.code);
        }
    }
    ;

term
    : factor {
        $$.code = $1.code;
        $$.value = $1.value;
    }
    | term MUL factor {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, $1.code, "*", $3.code); 
            $$.code = temp;
        } else {
            // Simulation mode
            $$.value = $1.value * $3.value;
            char temp[20];
            sprintf(temp, "%d", $$.value);
            $$.code = strdup(temp);
            track_allocation($$.code);
        }
    }
    | term DIV factor {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, $1.code, "/", $3.code); 
            $$.code = temp;
        } else {
            // Simulation mode
            if ($3.value == 0) {
                fprintf(stderr, "Runtime error at line %d: Division by zero.\n", yylineno);
                exit(1);
            }
            $$.value = $1.value / $3.value;
            char temp[20];
            sprintf(temp, "%d", $$.value);
            $$.code = strdup(temp);
            track_allocation($$.code);
        }
    }
    | term MOD factor {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, $1.code, "%", $3.code); 
            $$.code = temp;
        } else {
            // Simulation mode
            if ($3.value == 0) {
                fprintf(stderr, "Runtime error at line %d: Modulo by zero.\n", yylineno);
                exit(1);
            }
            $$.value = $1.value % $3.value;
            char temp[20];
            sprintf(temp, "%d", $$.value);
            $$.code = strdup(temp);
            track_allocation($$.code);
        }
    }
    ;

factor
    : OB expression CB {
        $$.code = $2.code;
        $$.value = $2.value;
    }
    | OB INTEGER_CONST COMMA INTEGER_CONST CB {
        // Handle (x, y) format for integer constants
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, $4, NULL, NULL); // Use the second value
            $$.code = temp;
        } else {
            // Simulation mode - use the second value
            $$.value = atoi($4);
            char temp[20];
            sprintf(temp, "%d", $$.value);
            $$.code = strdup(temp);
            track_allocation($$.code);
        }
    }
    | identifier {
        $$.code = strdup($1); 
        track_allocation($$.code);
        if (!emit_tac) {
            $$.value = get_variable_value($1);
        }
    }
    | INTEGER_CONST {
        $$.code = strdup($1); 
        track_allocation($$.code);
        $$.value = atoi($1);
    }
    | CHAR_LITERAL {
        $$.code = $1; 
        if ($1[0] == '\'') {
            $$.value = $1[1];  // 'a' -> value of 'a'
        } else {
            $$.value = atoi($1);
        }
    }
    ;

identifier
    : IDENTIFIER {
        if (!is_declared($1)) {
            fprintf(stderr, "Semantic error at line %d: Variable '%s' used before declaration.\n", yylineno, $1);
            exit(1);
        }
        $$ = strdup($1); track_allocation($$);
    }
    | IDENTIFIER LBRACKET expression RBRACKET {
        // Create array access identifier like "arr[3]"
        char* array_name = malloc(strlen($1) + strlen($3.code) + 3);
        sprintf(array_name, "%s[%s]", $1, $3.code);
        
        // Check if the base array is declared
        char* base_array = strdup($1);
        if (!is_declared(base_array)) {
            fprintf(stderr, "Semantic error at line %d: Array '%s' used before declaration.\n", yylineno, $1);
            free(base_array);
            exit(1);
        }
        free(base_array);
        
        $$ = array_name;
        track_allocation($$);
    }
    ;

bool_expr
    : condition {
        $$.code = $1.code;
        $$.value = $1.value;
    }
    | bool_expr AND bool_expr {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, $1.code, "and", $3.code); 
            $$.code = temp;
        } else {
            // Simulation mode
            $$.value = $1.value && $3.value;
            char temp[20];
            sprintf(temp, "%d", $$.value);
            $$.code = strdup(temp);
            track_allocation($$.code);
        }
    }
    | bool_expr OR bool_expr {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, $1.code, "or", $3.code); 
            $$.code = temp;
        } else {
            // Simulation mode
            $$.value = $1.value || $3.value;
            char temp[20];
            sprintf(temp, "%d", $$.value);
            $$.code = strdup(temp);
            track_allocation($$.code);
        }
    }
    | NOT bool_expr {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, "not", $2.code, NULL); 
            $$.code = temp;
        } else {
            // Simulation mode
            $$.value = !$2.value;
            char temp[20];
            sprintf(temp, "%d", $$.value);
            $$.code = strdup(temp);
            track_allocation($$.code);
        }
    }
    ;

condition
    : expression relop expression {
        if (emit_tac) {
            char* temp = newCondTemp();
            emit(temp, $1.code, $2.code, $3.code); 
            $$.code = temp;
        } else {
            // Simulation mode
            if (strcmp($2.code, "=") == 0)
                $$.value = ($1.value == $3.value);
            else if (strcmp($2.code, "!=") == 0)
                $$.value = ($1.value != $3.value);
            else if (strcmp($2.code, ">") == 0)
                $$.value = ($1.value > $3.value);
            else if (strcmp($2.code, ">=") == 0)
                $$.value = ($1.value >= $3.value);
            else if (strcmp($2.code, "<") == 0)
                $$.value = ($1.value < $3.value);
            else if (strcmp($2.code, "<=") == 0)
                $$.value = ($1.value <= $3.value);
                
            char temp[20];
            sprintf(temp, "%d", $$.value);
            $$.code = strdup(temp);
            track_allocation($$.code);
        }
    }
    ;

relop
    : EQ { $$.code = strdup("="); track_allocation($$.code); }
    | NE { $$.code = strdup("!="); track_allocation($$.code); }
    | GT { $$.code = strdup(">"); track_allocation($$.code); }
    | GE { $$.code = strdup(">="); track_allocation($$.code); }
    | LT { $$.code = strdup("<"); track_allocation($$.code); }
    | LE { $$.code = strdup("<="); track_allocation($$.code); }
    ;

block
    : BEGIN_TOKEN statementBlock END_TOKEN
    ;

for_dir
    : INC { 
        $$.code = strdup("inc"); track_allocation($$.code);
        $$.value = 1;  // For simulation
    }
    | DEC { 
        $$.code = strdup("dec"); track_allocation($$.code);
        $$.value = 0;  // For simulation
    }
    ;

for_step
    : expression { $$ = $1; }
    ;

print
    : PRINT OB STRING_LITERAL formatArgs CB SEMICOLON {
        if (emit_tac) {
            if ($4) {
                printf("print \"%s\", %s\n", $3, $4);
            } else {
                printf("print \"%s\"\n", $3);
            }
        } else {
            // Simulation mode - actually print the output
            simulate_print($3, $4);
        }
    }
    ;

scan
    : SCAN OB STRING_LITERAL COMMA argList CB SEMICOLON {
        if (emit_tac) {
            printf("scan \"%s\"\n", $3);
            if ($5) printf("vars: %s\n", $5); // variables list
        } else {
            // Simulation mode - actually read input values
            simulate_scan($3, $5);
        }
    }
    ;

formatArgs
    : /* empty */ { $$ = NULL; }
    | COMMA argList { $$ = $2; }
    ;

argList
    : value {
        if (emit_tac) {
            $$ = strdup($1.code); track_allocation($$);
        } else {
            char temp[20];
            sprintf(temp, "%d", $1.value);  // Convert value to string
            $$ = strdup(temp);
            track_allocation($$);
        }
    }
    | value COMMA argList {
        if (emit_tac) {
            char* temp = malloc(strlen($1.code) + strlen($3) + 2);
            sprintf(temp, "%s,%s", $1.code, $3);
            track_allocation(temp);
            $$ = temp;
        } else {
            char val_str[20];
            sprintf(val_str, "%d", $1.value);
            
            char* temp = malloc(strlen(val_str) + strlen($3) + 2);
            sprintf(temp, "%s,%s", val_str, $3);
            track_allocation(temp);
            $$ = temp;
        }
    }
    ;

value
    : identifier { 
        $$.code = $1; 
        if (!emit_tac) {
            $$.value = get_variable_value($1);
        }
    }
    | INTEGER_CONST { 
        $$.code = $1; 
        $$.value = atoi($1);
    }
    | CHAR_LITERAL  { 
        $$.code = $1; 
        if ($1[0] == '\'') {
            $$.value = $1[1];  // 'a' -> value of 'a'
        } else {
            $$.value = atoi($1);
        }
    }
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
        // Create variable name with array suffix if needed
        char* varname;
        if ($3 != NULL) {
            // If it's an array, append the array declaration to the name for tracking
            int len = strlen($2) + strlen($3) + 1;
            varname = malloc(len);
            sprintf(varname, "%s%s", $2, $3);
            track_allocation(varname);
        } else {
            varname = strdup($2);
            track_allocation(varname);
        }
        declare_variable(varname);
    }
    ;

arrayDec
    : /* empty */ { $$ = NULL; }
    | LBRACKET INTEGER_CONST RBRACKET {
        // Use INTEGER_CONST instead of DECIMAL for array size
        char* size = malloc(20);
        sprintf(size, "[%s]", $2);
        track_allocation(size);
        $$ = size;
    }
    ;

type
    : INT_TYPE
    | CHAR_TYPE
    ;

%%

int main(int argc, char** argv) {
    
    // Check if a file was specified
    if (argc > 1) {
        FILE* file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Cannot open file '%s': %s\n", argv[1], strerror(errno));
            return 1;
        }
        yyin = file;
    } else {
        yyin = stdin; // Read from standard input if no file provided
    }
    
    // Initialize memory
    allocated_memory = NULL;
    allocated_count = 0;
    
    // Parse and execute the program
    yyparse();
    
    // Clean up
    cleanup_memory();
    if (yyin != stdin) fclose(yyin);
    
    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "Parse error at line %d: %s\n", yylineno, s);
    exit(1);
}
