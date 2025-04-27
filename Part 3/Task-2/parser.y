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

char* getBaseOperator(char* op) { simulation ----
    const char* ops[][2] = {
        {"+=", "+"}, {"-=", "-"}, {"*=", "*"}, {"/=", "/"}, {"%=", "%"}, {NULL, NULL}
    };PE_CHAR,
    for (int i = 0; ops[i][0]; ++i)
        if (strcmp(op, ops[i][0]) == 0)
            return strdup(ops[i][1]);
    return strdup(op);
}   char* name;
%}  VarType type;
%union {n {
    int ival;nt_val;
    double dval;r_val;
    char* sval;
}   int initialized;  // Flag to track if variable has been initialized
    int is_array;     // Flag to indicate if variable is an array
%token <ival> DECIMAL // Size of array (if is_array is true)
%token <dval> DOUBLE
%token <sval> IDENTIFIER STRING_LITERAL CHAR_LITERAL INTEGER_CONST
#define MAX_SYMBOLS 1000
%token BEGIN_TOKEN END_TOKEN PROGRAM VARDECL IF ELSE WHILE FOR TO INC DEC DO PRINT SCAN
%token INT_TYPE CHAR_TYPE MAIN
%token ASSIGN PLUSEQ MINUSEQ MULEQ DIVEQ MODEQ
%token SLCOMMENT MLCOMMENTMBOLS];
%token COLON SEMICOLON COMMA LBRACKET RBRACKET QUOTE DQUOTE ATy variables
%token OB CB MUL ADD SUB DIV MOD EQ GT LT GE LE NE
%token AND OR NOT
// Instructions for simulation
%type <sval> expression term factor condition assignment_op relop value bool_expr
%type <sval> for_step for_dir
%type <sval> assignment
%type <sval> identifier
%type <sval> formatArgs
%type <sval> argList
    int is_goto;
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSEnt;
    int is_scan;
    char* label;
    char* condition;
%left ORction;
%left AND
%left EQ GT LT GE LE NEs[MAX_INSTRUCTIONS];
%left ADD SUBon_count = 0;
%left MUL DIV MOD
%right NOTC Emission with instruction storage ----
void emit(char* result, char* arg1, char* op, char* arg2) {
%%  if (arg2 && op) printf("%s := %s %s %s\n", result, arg1, op, arg2);
    else if (op)    printf("%s := %s %s\n", result, op, arg1);
programe            printf("%s := %s\n", result, arg1);
    : BEGIN_TOKEN PROGRAM COLON varDecBlock statementBlock END_TOKEN PROGRAM {
        printf("Parsing and TAC generation completed successfully.\n");
    }nstructions[instruction_count].result = strdup(result);
    | BEGIN_TOKEN PROGRAM COLON varDecBlock statementBlock error {ULL;
        printf("Warning: Syntax error near the end of the program. TAC generation may be incomplete.\n");
        yyerrok;[instruction_count].arg2 = arg2 ? strdup(arg2) : NULL;
    }nstructions[instruction_count].is_label = 0;
    ;nstructions[instruction_count].is_goto = 0;
statementBlockns[instruction_count].is_cond_jump = 0;
    : /* empty */instruction_count].is_print = 0;
    | statement statementBlockount].is_scan = 0;
    ;nstructions[instruction_count].label = NULL;
    instructions[instruction_count].condition = NULL;
statementuction_count++;
    : assignment SEMICOLON
    | if_cond SEMICOLON
    | while_loop SEMICOLONchar* args) {
    | for_stmt SEMICOLONuction
    | printtions[instruction_count].is_print = 1;
    | scanctions[instruction_count].arg1 = strdup(str);
    | blocktions[instruction_count].arg2 = args ? strdup(args) : NULL;
    | block SEMICOLON  // Optional semicolon after blocks
    ;
assignment
    : identifier assignment_op expression {
        if (strcmp($2, ":=") == 0) {
            emit($1, $3, NULL, NULL);s_scan = 1;
        } else {[instruction_count].arg1 = strdup(str);
            char* baseOp = getBaseOperator($2); ? strdup(args) : NULL;
            char* temp = newTemp();
            emit(temp, $1, baseOp, $3);
            emit($1, temp, NULL, NULL);
        }CondJump(char* cond, char* label) {
        $$ = strdup($1); track_allocation($$););
    }
    ;/ Store conditional jump for simulation
    instructions[instruction_count].is_cond_jump = 1;
assignment_opons[instruction_count].condition = strdup(cond);
    : ASSIGN  { $$ = strdup(":="); track_allocation($$); }
    | PLUSEQ  { $$ = strdup("+="); track_allocation($$); }
    | MINUSEQ { $$ = strdup("-="); track_allocation($$); }
    | MULEQ   { $$ = strdup("*="); track_allocation($$); }
    | DIVEQ   { $$ = strdup("/="); track_allocation($$); }
    | MODEQ   { $$ = strdup("%="); track_allocation($$); }
    ;
    // Store conditional jump for simulation
    instructions[instruction_count].is_cond_jump = 1;
    instructions[instruction_count].condition = strdup(cond);
while_loopctions[instruction_count].label = strdup(label);
  : WHILE OB bool_expr CB DOe, so we need to invert our condition check in simulation
      {tructions[instruction_count].arg1 = strdup("true");
        /* 1) at “do”, generate labels & test */
        char* start = newLabel();  track_allocation(start);
        char* end   = newLabel();  track_allocation(end);
        current_loop_start = start;
        current_loop_end   = end;
    
        emitLabel(start);            /* L1: */
        emitCondJump($3, end);       /* if t_cond1 == 0 goto L2 */
      }tructions[instruction_count].label = strdup(label);
    blockuction_count++;
      {
        /* 2) after the block, loop back & close */
        emitGoto(current_loop_start);  
        emitLabel(current_loop_end);  /* L2: */
      }
  ; // Store label for simulation
    instructions[instruction_count].is_label = 1;
    instructions[instruction_count].label = strdup(label);
    instruction_count++;
// Correct implementation for for loop
for_stmt
  : FOR identifier ASSIGN expression
        TO expression for_dir for_step DO
      {d_symbol(const char* name) {
        /* 1) init loop var */_count; i++) {
        emit($2, $4, NULL, NULL);             /* a := (10, 8) */
            return i;
        /* 2) blank and loop‐start label */
        printf("\n");
        char* start = newLabel();  track_allocation(start);
        char* end   = newLabel();  track_allocation(end);
        current_for_start = start;
        current_for_end   = end;me) {
        emitLabel(start);  orary variable (t1, t2, etc.)
        printf("\n");' && (name[1] == '_' || isdigit(name[1]))) {
        int idx = -1;
        /* 3) compute bound (t3) and step (t4) */
        char* bound = newTemp();iables
        emit(bound, $6, NULL, NULL);          /* t3 := b + (10, 2) */
            if (idx > 0 && idx <= cond_var_counter)
        current_for_step_temp = newTemp();
        emit(current_for_step_temp, $8, NULL, NULL);  
                                              /* t4 := (1, 10) */
        printf("\n"); 0 && idx <= temp_var_counter)
                return idx - 1;
        /* 4) test and conditional jump - handle inc/dec direction */
        char* cond = newCondTemp();
        if (strcmp($7, "inc") == 0) {
            emit(cond, $2, ">", bound);           /* t_cond2 := a > t3 */
        } else {
            emit(cond, $2, "<", bound);           /* t_cond2 := a < t3 */e integer constants in various formats
        }_integer_const(const char* str) {
        emitCondJumpTrue(cond, end);          /* if t_cond2 == 1 goto L4 */Skip the opening bracket
        printf("\n");
      }    // Find the comma
    block
      {
        /* 5) body already emitted by `block` */
valid
        /* 6) increment by saved step temp based on direction */
        char* tmp = newTemp();    *comma = '\0';
        if (strcmp($7, "inc") == 0) {
            emit(tmp, $2, "+", current_for_step_temp);
        } else {
            emit(tmp, $2, "-", current_for_step_temp);omma + 1);
        }
        emit($2, tmp, NULL, NULL);             /* a := tmp */ // Convert from the given base to decimal
        printf("\n");    if (base == 2 || base == 8) {
        int decimal_value = 0;
        /* 7) back‐edge and end label */        int digit_value;
        emitGoto(current_for_start);        for (int i = 0; copy[i] != '\0'; i++) {
        emitLabel(current_for_end);            /* L4: */copy[i] - '0';
        printf("\n");     if ((base == 2 && digit_value > 1) || 
      }== 8 && digit_value > 7)) {
  ;        fprintf(stderr, "Semantic error: Invalid digit for base %d\n", base);




// -- IF-ELSE Statement -- value = decimal_value;
if_cond
  : IF OB bool_expr CB
      { 
        /* 1) at “)”, make labels and emit the conditional jump */
        __if_false = newLabel();    track_allocation(__if_false);
        __if_end   = newLabel();    track_allocation(__if_end);
        emitCondJump($3, __if_false);  nteger value of a variable or literal
      }nt_value(const char* name) {
    blockCheck if it's a literal number directly
      {[0] == '-' && isdigit(name[1]))) {
        /* 2) right after THEN block */
        emitGoto(__if_end);
        emitLabel(__if_false);   
      }    // Check if it's in (value, base) format
    ELSE    if (name[0] == '(') {
    block        return parse_integer_const(name);
      {    }
        /* 3) after ELSE block */
        emitLabel(__if_end);
      });
  ;
 {
       fprintf(stderr, "Semantic error at line %d: Variable '%s' used before initialization.\n", 
name);

expression
    : term                   { $$ = $1; }   return symbol_table[idx].value.int_val;
    | expression ADD term    {
        char* temp = newTemp();    
        emit(temp, $1, "+", $3); $$ = temp;// Check temp variables
    }
    | expression SUB term    {
        char* temp = newTemp();tialized) {
        emit(temp, $1, "-", $3); $$ = temp;: Temporary variable '%s' used before initialization.\n", name);
    }       exit(1);
    ;
alue.int_val;
term
    : factor                { $$ = $1; }
    | term MUL factor       { error: Undefined variable '%s'\n", name);
        char* temp = newTemp();
        emit(temp, $1, "*", $3); $$ = temp;
    }
    | term DIV factor       {
        char* temp = newTemp();// Set integer value of a variable
        emit(temp, $1, "/", $3); $$ = temp;et_int_value(const char* name, int value) {
    }
    | term MOD factor       {
        char* temp = newTemp();
        emit(temp, $1, "%", $3); $$ = temp;lue;
    }   temp_vars[idx].initialized = 1;
    ;        temp_vars[idx].type = TYPE_INT;
turn;
factor
    : OB expression CB       { $$ = $2; }
    | identifier             { $$ = strdup($1); track_allocation($$); }
    | INTEGER_CONST          { $$ = strdup($1); track_allocation($$); }l(name);
    | CHAR_LITERAL           { $$ = $1; }dx >= 0) {
    ;e;
   symbol_table[idx].initialized = 1;
identifier   symbol_table[idx].type = TYPE_INT;
    : IDENTIFIER {        return;
        if (!is_declared($1)) {
            fprintf(stderr, "Semantic error: variable '%s' used before declaration.\n", $1);
            exit(1);rror: Undefined variable '%s'\n", name);
        }
        $$ = strdup($1); track_allocation($$);
    }
    ;n
char* op, int right) {
bool_exprright;
    : condition { $$ = $1; }f (strcmp(op, "-") == 0) return left - right;
    | bool_expr AND bool_expr {") == 0) return left * right;
        char* temp = newTemp();
        emit(temp, $1, "and", $3); $$ = temp;
    }       fprintf(stderr, "Semantic error: Division by zero\n");
    | bool_expr OR bool_expr {       exit(1);
        char* temp = newTemp();        }
        emit(temp, $1, "or", $3); $$ = temp;eturn left / right;
    }
    | NOT bool_expr {
        char* temp = newTemp();
        emit(temp, "not", $2, NULL); $$ = temp;intf(stderr, "Semantic error: Modulo by zero\n");
    }         exit(1);
    ;     }
        return left % right;
condition    }
  : expression relop expression {    if (strcmp(op, "and") == 0) return left && right;
      char* tmp = newCondTemp();          if (strcmp(op, "or") == 0) return left || right;
      emit(tmp, $1, $2, $3);          /* t_cond1 := lhs relop rhs */    
      $$ = tmp;/ Comparison operations
  }
  ;





relop    fprintf(stderr, "Semantic error: Unknown operator '%s'\n", op);
    : EQ { $$ = strdup("="); track_allocation($$); }xit(1);
    | NE { $$ = strdup("!="); track_allocation($$); }
    | GT { $$ = strdup(">"); track_allocation($$); }
    | GE { $$ = strdup(">="); track_allocation($$); }
    | LT { $$ = strdup("<"); track_allocation($$); }// Execute a single instruction
    | LE { $$ = strdup("<="); track_allocation($$); }cute_instruction(int idx) {
    ;

block/ Handle label (no action needed)
    : BEGIN_TOKEN statementBlock END_TOKEN    if (instr->is_label) return idx + 1;
    ;
// -- FOR loop support --
f (instr->is_goto) {
for_dir        // Find the label
    : INC { $$ = strdup("inc"); track_allocation($$); }count; i++) {
    | DEC { $$ = strdup("dec"); track_allocation($$); } && strcmp(instructions[i].label, instr->label) == 0) {
    ;                return i;  // Return the index to jump to

for_step        }
    : expression { $$ = $1; }   fprintf(stderr, "Runtime error: Label '%s' not found\n", instr->label);
    ;

// Fix for the while loop implementation
// Correct implementation for while loopPrint($3, $4);nditional jump

// -- PRINT and SCAN --   printf("print \"%s\"\n", $3);nt condition_value = get_int_value(instr->condition);
       emitPrint($3, NULL);   if (instr->arg1 && strcmp(instr->arg1, "true") == 0) {
print   }       // Jump if true (condition == 1)
    : PRINT OB STRING_LITERAL formatArgs CB SEMICOLON {    }            if (condition_value == 1) {
        if ($4) {;            // Find the label
            printf("print \"%s\", %s\n", $3, $4);
        } else {ons[i].label, instr->label) == 0) {
            printf("print \"%s\"\n", $3);
        }   printf("scan \"%s\"\n", $3);               }
    }   if ($5) printf("vars: %s\n", $5); // variables list           }
    ;        emitScan($3, $5);            }
{
scan
    : SCAN OB STRING_LITERAL COMMA argList CB SEMICOLON {
        printf("scan \"%s\"\n", $3); VarDecl Handling --           // Find the label
        if ($5) printf("vars: %s\n", $5); // variables list                for (int i = 0; i < instruction_count; i++) {
    }lock             if (instructions[i].is_label && strcmp(instructions[i].label, instr->label) == 0) {
    ;OKEN VARDECL COLON VarDecList END_TOKEN VARDECL           return i;

formatArgs      }
    : /* empty */ { $$ = NULL; }
    | COMMA argList { $$ = $2; }
    ;

argList
    : value {claration/ Handle print
        $$ = strdup($1); track_allocation($$); OB IDENTIFIER arrayDec COMMA type CB SEMICOLON {f (instr->is_print) {
    }        declare_variable($2);        // Extract string without quotes
    | value COMMA argList {   char* str = instr->arg1;
        char* temp = malloc(strlen($1) + strlen($3) + 2);
        sprintf(temp, "%s,%s", $1, $3);
        track_allocation(temp);
        $$ = temp; /* empty */   clean_str[len - 2] = '\0';
    }    | LBRACKET DECIMAL RBRACKET        
    ;
            // Format string with arguments
valueargs_copy = strdup(instr->arg2);
    : identifier { $$ = $1; }
    | INTEGER_CONST { $$ = $1; } CHAR_TYPE       int arg_count = 0;
    | CHAR_LITERAL  { $$ = $1; }    ;            int values[10];  // Support up to 10 arguments
    ;
-- all arguments
// -- VarDecl Handling --
        values[arg_count++] = get_int_value(token);
varDecBlock                token = strtok(NULL, ",");
    : BEGIN_TOKEN VARDECL COLON VarDecList END_TOKEN VARDECLrgc, char *argv[]) {
    ;
e: %s <input file>\n", argv[0]);rationed output - replace %d with values
VarDecList   return 1;   int is_array = 0;       char* fmt = clean_str;
    : VarDeclaration   int array_size = 0;       char* output = malloc(1000);
    | VarDeclaration VarDecList        char* base_name = strdup($2);            char* out_ptr = output;
    ; = fopen(argv[1], "r");    int val_idx = 0;
or the brackets in the identifier
VarDeclarationile");chr(base_name, '[');idx < arg_count) {
    : OB IDENTIFIER arrayDec COMMA type CB SEMICOLON {   return 1;   if (bracket_pos != NULL) {           if (*fmt == '%' && *(fmt + 1) == 'd') {
        declare_variable($2);    }            *bracket_pos = '\0';  // Terminate the string at the bracket                    int chars = sprintf(out_ptr, "%d", values[val_idx++]);
    }    is_array = 1;                out_ptr += chars;
    ;e() == 0) {    fmt += 2;
"Parsing done.\n");Extract array size if available } else {
arrayDec       char* size_str = bracket_pos + 1;               *out_ptr++ = *fmt++;
    : /* empty */            char* close_bracket = strchr(size_str, ']');                }
    | LBRACKET DECIMAL RBRACKET;bracket) {
    ;    fclose(yyin);                *close_bracket = '\0';            *out_ptr = '\0';
  return 0;              array_size = atoi(size_str);          
type}            }            printf("%s\n", output);
    : INT_TYPE
    | CHAR_TYPEhar* s) {
    ;, s);
y();ariable(base_name);f("%s\n", clean_str);
// -- Final Section --xit(1);      }
}
%%

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
        return 1;
    }
d successfully.\n");
    yyin = fopen(argv[1], "r");   
    if (!yyin) {        // Initialize temporary variables for simulation
        perror("Error opening file"); i < MAX_TEMP_VARS; i++) {
        return 1;vars[i].initialized = 0;
    }emp_vars[i].type = TYPE_UNKNOWN;
       }
    if (yyparse() == 0) {        
        printf("Parsing done.\n");ogram simulation...\n");
    }
am();
   // cleanup_memory();
    fclose(yyin);       // Print the symbol table








}    exit(1);    cleanup_memory();    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, s);void yyerror(const char* s) {}    return 0;













}    exit(1);    cleanup_memory();    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, s);void yyerror(const char* s) {}    return 0;    fclose(yyin);    cleanup_memory();    }        print_symbol_table();        printf("\nPrinting symbol table...\n");
}    exit(1);    cleanup_memory();    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, s);void yyerror(const char* s) {}    return 0;    fclose(yyin);   // cleanup_memory();    }        printf("Parsing done.\n");    if (yyparse() == 0) {    }        return 1;        perror("Error opening file");    if (!yyin) {    yyin = fopen(argv[1], "r");    }                int val;
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

// Update declare_variable to include symbol table
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

    execute_program();
    print_symbol_table();
    cleanup_memory();
    fclose(yyin);
    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, s);
    cleanup_memory();
    exit(1);
}