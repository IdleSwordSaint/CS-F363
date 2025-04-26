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

#line 405 "myparser.tab.c"

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
  YYSYMBOL_DOUBLE = 4,                     /* DOUBLE  */
  YYSYMBOL_IDENTIFIER = 5,                 /* IDENTIFIER  */
  YYSYMBOL_STRING_LITERAL = 6,             /* STRING_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 7,               /* CHAR_LITERAL  */
  YYSYMBOL_INTEGER_CONST = 8,              /* INTEGER_CONST  */
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
  YYSYMBOL_INT_TYPE = 23,                  /* INT_TYPE  */
  YYSYMBOL_CHAR_TYPE = 24,                 /* CHAR_TYPE  */
  YYSYMBOL_MAIN = 25,                      /* MAIN  */
  YYSYMBOL_ASSIGN = 26,                    /* ASSIGN  */
  YYSYMBOL_PLUSEQ = 27,                    /* PLUSEQ  */
  YYSYMBOL_MINUSEQ = 28,                   /* MINUSEQ  */
  YYSYMBOL_MULEQ = 29,                     /* MULEQ  */
  YYSYMBOL_DIVEQ = 30,                     /* DIVEQ  */
  YYSYMBOL_MODEQ = 31,                     /* MODEQ  */
  YYSYMBOL_SLCOMMENT = 32,                 /* SLCOMMENT  */
  YYSYMBOL_MLCOMMENT = 33,                 /* MLCOMMENT  */
  YYSYMBOL_COLON = 34,                     /* COLON  */
  YYSYMBOL_SEMICOLON = 35,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 36,                     /* COMMA  */
  YYSYMBOL_LBRACKET = 37,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 38,                  /* RBRACKET  */
  YYSYMBOL_QUOTE = 39,                     /* QUOTE  */
  YYSYMBOL_DQUOTE = 40,                    /* DQUOTE  */
  YYSYMBOL_AT = 41,                        /* AT  */
  YYSYMBOL_OB = 42,                        /* OB  */
  YYSYMBOL_CB = 43,                        /* CB  */
  YYSYMBOL_MUL = 44,                       /* MUL  */
  YYSYMBOL_ADD = 45,                       /* ADD  */
  YYSYMBOL_SUB = 46,                       /* SUB  */
  YYSYMBOL_DIV = 47,                       /* DIV  */
  YYSYMBOL_MOD = 48,                       /* MOD  */
  YYSYMBOL_EQ = 49,                        /* EQ  */
  YYSYMBOL_GT = 50,                        /* GT  */
  YYSYMBOL_LT = 51,                        /* LT  */
  YYSYMBOL_GE = 52,                        /* GE  */
  YYSYMBOL_LE = 53,                        /* LE  */
  YYSYMBOL_NE = 54,                        /* NE  */
  YYSYMBOL_AND = 55,                       /* AND  */
  YYSYMBOL_OR = 56,                        /* OR  */
  YYSYMBOL_NOT = 57,                       /* NOT  */
  YYSYMBOL_LOWER_THAN_ELSE = 58,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_YYACCEPT = 59,                  /* $accept  */
  YYSYMBOL_program = 60,                   /* program  */
  YYSYMBOL_61_1 = 61,                      /* $@1  */
  YYSYMBOL_statementBlock = 62,            /* statementBlock  */
  YYSYMBOL_statement = 63,                 /* statement  */
  YYSYMBOL_assignment = 64,                /* assignment  */
  YYSYMBOL_assignment_op = 65,             /* assignment_op  */
  YYSYMBOL_while_loop = 66,                /* while_loop  */
  YYSYMBOL_67_2 = 67,                      /* $@2  */
  YYSYMBOL_for_stmt = 68,                  /* for_stmt  */
  YYSYMBOL_69_3 = 69,                      /* $@3  */
  YYSYMBOL_if_cond = 70,                   /* if_cond  */
  YYSYMBOL_71_4 = 71,                      /* $@4  */
  YYSYMBOL_72_5 = 72,                      /* $@5  */
  YYSYMBOL_expression = 73,                /* expression  */
  YYSYMBOL_term = 74,                      /* term  */
  YYSYMBOL_factor = 75,                    /* factor  */
  YYSYMBOL_identifier = 76,                /* identifier  */
  YYSYMBOL_bool_expr = 77,                 /* bool_expr  */
  YYSYMBOL_condition = 78,                 /* condition  */
  YYSYMBOL_relop = 79,                     /* relop  */
  YYSYMBOL_block = 80,                     /* block  */
  YYSYMBOL_for_dir = 81,                   /* for_dir  */
  YYSYMBOL_for_step = 82,                  /* for_step  */
  YYSYMBOL_print = 83,                     /* print  */
  YYSYMBOL_scan = 84,                      /* scan  */
  YYSYMBOL_formatArgs = 85,                /* formatArgs  */
  YYSYMBOL_argList = 86,                   /* argList  */
  YYSYMBOL_value = 87,                     /* value  */
  YYSYMBOL_varDecBlock = 88,               /* varDecBlock  */
  YYSYMBOL_VarDecList = 89,                /* VarDecList  */
  YYSYMBOL_VarDeclaration = 90,            /* VarDeclaration  */
  YYSYMBOL_arrayDec = 91,                  /* arrayDec  */
  YYSYMBOL_type = 92                       /* type  */
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
#define YYLAST   156

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  74
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  152

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
       0,   379,   379,   379,   386,   392,   393,   397,   398,   399,
     400,   401,   402,   403,   404,   407,   427,   428,   429,   430,
     431,   432,   436,   436,   459,   459,   538,   547,   538,   564,
     568,   582,   599,   603,   617,   635,   656,   660,   675,   682,
     687,   698,   705,   725,   729,   743,   757,   774,   803,   804,
     805,   806,   807,   808,   812,   816,   820,   827,   831,   846,
     858,   859,   863,   873,   892,   898,   902,   915,   919,   920,
     924,   942,   943,   953,   954
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
  "\"end of file\"", "error", "\"invalid token\"", "DECIMAL", "DOUBLE",
  "IDENTIFIER", "STRING_LITERAL", "CHAR_LITERAL", "INTEGER_CONST",
  "BEGIN_TOKEN", "END_TOKEN", "PROGRAM", "VARDECL", "IF", "ELSE", "WHILE",
  "FOR", "TO", "INC", "DEC", "DO", "PRINT", "SCAN", "INT_TYPE",
  "CHAR_TYPE", "MAIN", "ASSIGN", "PLUSEQ", "MINUSEQ", "MULEQ", "DIVEQ",
  "MODEQ", "SLCOMMENT", "MLCOMMENT", "COLON", "SEMICOLON", "COMMA",
  "LBRACKET", "RBRACKET", "QUOTE", "DQUOTE", "AT", "OB", "CB", "MUL",
  "ADD", "SUB", "DIV", "MOD", "EQ", "GT", "LT", "GE", "LE", "NE", "AND",
  "OR", "NOT", "LOWER_THAN_ELSE", "$accept", "program", "$@1",
  "statementBlock", "statement", "assignment", "assignment_op",
  "while_loop", "$@2", "for_stmt", "$@3", "if_cond", "$@4", "$@5",
  "expression", "term", "factor", "identifier", "bool_expr", "condition",
  "relop", "block", "for_dir", "for_step", "print", "scan", "formatArgs",
  "argList", "value", "varDecBlock", "VarDecList", "VarDeclaration",
  "arrayDec", "type", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-109)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-6)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -1,     2,    10,   -18,  -109,    28,    33,    62,    18,    21,
      76,    13,    27,    77,    44,    69,    76,   111,    76,    82,
      83,    84,    85,    79,    87,  -109,  -109,    81,    12,   114,
       7,     7,    99,   120,   121,   118,  -109,  -109,  -109,  -109,
    -109,  -109,  -109,  -109,  -109,  -109,  -109,  -109,    12,  -109,
     124,   122,    81,  -109,  -109,    17,    15,   -21,  -109,  -109,
    -109,     7,    50,   -12,  -109,    -9,    12,    94,    95,   123,
      -5,    96,   125,  -109,   100,     5,  -109,    12,    12,    12,
      12,    12,  -109,  -109,  -109,  -109,  -109,  -109,  -109,    12,
    -109,     7,     7,   115,   -13,    65,    97,    65,  -109,   130,
     103,  -109,   133,  -109,   -21,   -21,  -109,  -109,  -109,    -5,
     134,  -109,    89,  -109,    12,  -109,  -109,  -109,  -109,   106,
     110,   104,   108,    56,   105,  -109,   134,    11,    65,  -109,
     116,  -109,  -109,  -109,   107,  -109,   135,  -109,  -109,  -109,
      12,  -109,  -109,   117,   134,    -5,   136,  -109,  -109,  -109,
     134,  -109
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,     2,     0,    41,
       5,     0,     0,     0,     0,     0,     5,     0,     5,     0,
       0,     0,     0,     0,    13,    11,    12,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     6,     7,     9,
      10,     8,    16,    17,    18,    19,    20,    21,     0,    14,
       0,     0,    68,    40,    39,     0,     0,    29,    32,    38,
      54,     0,     0,     0,    43,     0,     0,    60,     0,     0,
      15,    71,     0,    69,    39,     0,    42,     0,     0,     0,
       0,     0,    46,    48,    50,    52,    51,    53,    49,     0,
      26,     0,     0,     0,     0,     0,     0,     0,     3,     0,
       0,    67,     0,    36,    30,    31,    33,    34,    35,    47,
       0,    44,    45,    22,     0,    66,    65,    64,    61,    62,
       0,     0,     0,     0,     0,    27,     0,     0,     0,    58,
       0,    72,    73,    74,     0,    37,     0,    23,    55,    56,
       0,    63,    59,     0,     0,    57,     0,    70,    28,    24,
       0,    25
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -109,  -109,  -109,    58,  -109,  -109,  -109,  -109,  -109,  -109,
    -109,  -109,  -109,  -109,   -27,    16,    35,    -7,   -26,  -109,
    -109,  -108,  -109,  -109,  -109,  -109,  -109,   -90,  -109,  -109,
     101,  -109,  -109,  -109
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,    16,    17,    18,    19,    48,    20,   126,    21,
     150,    22,   110,   136,    62,    57,    58,    59,    63,    64,
      89,    24,   140,   146,    25,    26,    96,   118,   119,     7,
      51,    52,   100,   134
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      23,    56,   125,    23,   114,    65,    32,   121,     1,    23,
       4,    23,     9,     3,    53,    54,     5,     9,   137,    53,
      54,    70,     9,    79,    53,    74,    80,    81,    75,   138,
     139,    90,    77,    78,    93,    82,   148,     6,   141,    94,
      77,    78,   151,    91,    92,     8,    91,    92,   103,    55,
      77,    78,    27,    76,    55,    30,    77,    78,    28,    55,
      77,    78,   109,    -5,    61,   111,   112,     9,    29,    31,
       9,    10,   115,   116,    35,    11,    37,    12,    13,   132,
     133,     9,     9,    14,    15,    10,    33,   127,   117,    11,
     117,    12,    13,   104,   105,    77,    78,    14,    15,    83,
      84,    85,    86,    87,    88,    42,    43,    44,    45,    46,
      47,    34,    36,   145,   106,   107,   108,    38,    39,    40,
      41,   117,    49,    50,    60,    66,    67,    68,    69,    71,
      95,    97,    72,    99,    98,   113,   102,   101,   122,   123,
     120,   124,   128,    10,    91,   129,   131,   130,   135,   144,
     143,   142,   147,    73,     0,     0,   149
};

static const yytype_int16 yycheck[] =
{
       7,    28,   110,    10,    17,    31,    13,    97,     9,    16,
       0,    18,     5,    11,     7,     8,    34,     5,   126,     7,
       8,    48,     5,    44,     7,     8,    47,    48,    55,    18,
      19,    43,    45,    46,    43,    61,   144,     9,   128,    66,
      45,    46,   150,    55,    56,    12,    55,    56,    43,    42,
      45,    46,    34,    38,    42,    42,    45,    46,    37,    42,
      45,    46,    89,     1,    57,    91,    92,     5,    10,    42,
       5,     9,     7,     8,    16,    13,    18,    15,    16,    23,
      24,     5,     5,    21,    22,     9,    42,   114,    95,    13,
      97,    15,    16,    77,    78,    45,    46,    21,    22,    49,
      50,    51,    52,    53,    54,    26,    27,    28,    29,    30,
      31,    42,     1,   140,    79,    80,    81,    35,    35,    35,
      35,   128,    35,    42,    10,    26,     6,     6,    10,     5,
      36,    36,    10,    37,    11,    20,    36,    12,     8,    36,
      43,     8,    36,     9,    55,    35,    38,    43,    43,    14,
      43,    35,    35,    52,    -1,    -1,    20
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     9,    60,    11,     0,    34,     9,    88,    12,     5,
       9,    13,    15,    16,    21,    22,    61,    62,    63,    64,
      66,    68,    70,    76,    80,    83,    84,    34,    37,    62,
      42,    42,    76,    42,    42,    62,     1,    62,    35,    35,
      35,    35,    26,    27,    28,    29,    30,    31,    65,    35,
      42,    89,    90,     7,     8,    42,    73,    74,    75,    76,
      10,    57,    73,    77,    78,    77,    26,     6,     6,    10,
      73,     5,    10,    89,     8,    73,    38,    45,    46,    44,
      47,    48,    77,    49,    50,    51,    52,    53,    54,    79,
      43,    55,    56,    43,    73,    36,    85,    36,    11,    37,
      91,    12,    36,    43,    74,    74,    75,    75,    75,    73,
      71,    77,    77,    20,    17,     7,     8,    76,    86,    87,
      43,    86,     8,    36,     8,    80,    67,    73,    36,    35,
      43,    38,    23,    24,    92,    43,    72,    80,    18,    19,
      81,    86,    35,    43,    14,    73,    82,    35,    80,    20,
      69,    80
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    59,    61,    60,    60,    62,    62,    63,    63,    63,
      63,    63,    63,    63,    63,    64,    65,    65,    65,    65,
      65,    65,    67,    66,    69,    68,    71,    72,    70,    73,
      73,    73,    74,    74,    74,    74,    75,    75,    75,    75,
      75,    76,    76,    77,    77,    77,    77,    78,    79,    79,
      79,    79,    79,    79,    80,    81,    81,    82,    83,    84,
      85,    85,    86,    86,    87,    87,    87,    88,    89,    89,
      90,    91,    91,    92,    92
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     8,     6,     0,     2,     2,     2,     2,
       2,     1,     1,     1,     2,     3,     1,     1,     1,     1,
       1,     1,     0,     7,     0,    11,     0,     0,     9,     1,
       3,     3,     1,     3,     3,     3,     3,     5,     1,     1,
       1,     1,     4,     1,     3,     3,     2,     3,     1,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     6,     7,
       0,     2,     1,     3,     1,     1,     1,     6,     1,     2,
       7,     0,     3,     1,     1
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
  case 2: /* $@1: %empty  */
#line 379 "myparser.y"
                                            {
        // After variable declarations, switch to simulation mode
        emit_tac = 0;
    }
#line 1594 "myparser.tab.c"
    break;

  case 3: /* program: BEGIN_TOKEN PROGRAM COLON varDecBlock $@1 statementBlock END_TOKEN PROGRAM  */
#line 382 "myparser.y"
                                       {
        printf("\nProgram execution completed successfully.\n");
        print_symbol_table();
    }
#line 1603 "myparser.tab.c"
    break;

  case 4: /* program: BEGIN_TOKEN PROGRAM COLON varDecBlock statementBlock error  */
#line 386 "myparser.y"
                                                                 {
        printf("Warning: Syntax error near the end of the program. TAC generation may be incomplete.\n");
        yyerrok;
    }
#line 1612 "myparser.tab.c"
    break;

  case 15: /* assignment: identifier assignment_op expression  */
#line 407 "myparser.y"
                                          {
        if (emit_tac) {
            if (strcmp((yyvsp[-1].expr).code, ":=") == 0) {
                emit((yyvsp[-2].sval), (yyvsp[0].expr).code, NULL, NULL);
            } else {
                char* baseOp = getBaseOperator((yyvsp[-1].expr).code);
                char* temp = newTemp();
                emit(temp, (yyvsp[-2].sval), baseOp, (yyvsp[0].expr).code);
                emit((yyvsp[-2].sval), temp, NULL, NULL);
            }
            (yyval.expr).code = strdup((yyvsp[-2].sval)); track_allocation((yyval.expr).code);
        } else {
            // Simulation mode
            process_assignment((yyvsp[-2].sval), (yyvsp[-1].expr).code, (yyvsp[0].expr).code);
            (yyval.expr).code = strdup((yyvsp[-2].sval)); track_allocation((yyval.expr).code);
        }
    }
#line 1634 "myparser.tab.c"
    break;

  case 16: /* assignment_op: ASSIGN  */
#line 427 "myparser.y"
              { (yyval.expr).code = strdup(":="); track_allocation((yyval.expr).code); }
#line 1640 "myparser.tab.c"
    break;

  case 17: /* assignment_op: PLUSEQ  */
#line 428 "myparser.y"
              { (yyval.expr).code = strdup("+="); track_allocation((yyval.expr).code); }
#line 1646 "myparser.tab.c"
    break;

  case 18: /* assignment_op: MINUSEQ  */
#line 429 "myparser.y"
              { (yyval.expr).code = strdup("-="); track_allocation((yyval.expr).code); }
#line 1652 "myparser.tab.c"
    break;

  case 19: /* assignment_op: MULEQ  */
#line 430 "myparser.y"
              { (yyval.expr).code = strdup("*="); track_allocation((yyval.expr).code); }
#line 1658 "myparser.tab.c"
    break;

  case 20: /* assignment_op: DIVEQ  */
#line 431 "myparser.y"
              { (yyval.expr).code = strdup("/="); track_allocation((yyval.expr).code); }
#line 1664 "myparser.tab.c"
    break;

  case 21: /* assignment_op: MODEQ  */
#line 432 "myparser.y"
              { (yyval.expr).code = strdup("%="); track_allocation((yyval.expr).code); }
#line 1670 "myparser.tab.c"
    break;

  case 22: /* $@2: %empty  */
#line 436 "myparser.y"
                               {
        if (emit_tac) {
            char* start = newLabel();
            char* end = newLabel();
            current_loop_start = start;
            current_loop_end = end;

            emitLabel(start);
            emitCondJump((yyvsp[-2].expr).code, end);
        } else {
            // Simulation mode - keep the condition's result
        }
    }
#line 1688 "myparser.tab.c"
    break;

  case 23: /* while_loop: WHILE OB bool_expr CB DO $@2 block  */
#line 448 "myparser.y"
            {
        if (emit_tac) {
            emitGoto(current_loop_start);
            emitLabel(current_loop_end);
        } else {
            // Simulation: block already handled conditionally
        }
    }
#line 1701 "myparser.tab.c"
    break;

  case 24: /* $@3: %empty  */
#line 459 "myparser.y"
                                                                         {
        if (emit_tac) {
            // Original TAC generation for FOR loop
            emit((yyvsp[-7].sval), (yyvsp[-5].expr).code, NULL, NULL);
            
            printf("\n");
            char* start_label = newLabel();
            char* end_label = newLabel();
            current_for_start = start_label;
            current_for_end = end_label;
            emitLabel(start_label);
            
            printf("\n");
            char* bound = newTemp();
            emit(bound, (yyvsp[-3].expr).code, NULL, NULL);
            
            char* step = newTemp();
            emit(step, (yyvsp[-1].expr).code, NULL, NULL);
            
            printf("\n");
            char* cond = newCondTemp();
            if (strcmp((yyvsp[-2].expr).code, "inc") == 0)
                emit(cond, (yyvsp[-7].sval), ">", bound);
            else
                emit(cond, bound, ">", (yyvsp[-7].sval));
            emitCondJumpTrue(cond, end_label);
            
            printf("\n");
        } else {
            // Simulation mode
            char* loop_var = (yyvsp[-7].sval);
            int loop_idx = get_var_index(loop_var);
            int initial_val = (yyvsp[-5].expr).value;
            int target_val = (yyvsp[-3].expr).value;
            int step_val = (yyvsp[-1].expr).value;
            int is_inc = strcmp((yyvsp[-2].expr).code, "inc") == 0;
            
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
    }
#line 1767 "myparser.tab.c"
    break;

  case 25: /* for_stmt: FOR identifier ASSIGN expression TO expression for_dir for_step DO $@3 block  */
#line 519 "myparser.y"
            {
        if (emit_tac) {
            char* loop_var = (yyvsp[-9].sval);
            char* step = (yyvsp[-3].expr).code;
            char* step_temp = newTemp();
            if (strcmp((yyvsp[-4].expr).code, "inc") == 0)
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
#line 1788 "myparser.tab.c"
    break;

  case 26: /* $@4: %empty  */
#line 538 "myparser.y"
                         {
        if (emit_tac) {
            __if_false = newLabel();
            __if_end = newLabel();
            emitCondJump((yyvsp[-1].expr).code, __if_false);
        } else {
            // If condition is true (1), the following block will be executed
            // by the parser
        }
    }
#line 1803 "myparser.tab.c"
    break;

  case 27: /* $@5: %empty  */
#line 547 "myparser.y"
            {
        if (emit_tac) {
            emitGoto(__if_end);
            emitLabel(__if_false);
        } else {
            // Simulation: block already executed if condition was true
        }
    }
#line 1816 "myparser.tab.c"
    break;

  case 28: /* if_cond: IF OB bool_expr CB $@4 block $@5 ELSE block  */
#line 554 "myparser.y"
                 {
        if (emit_tac) {
            emitLabel(__if_end);
        } else {
            // Simulation: blocks already executed based on condition
        }
    }
#line 1828 "myparser.tab.c"
    break;

  case 29: /* expression: term  */
#line 564 "myparser.y"
           {
        (yyval.expr).code = (yyvsp[0].expr).code;
        (yyval.expr).value = (yyvsp[0].expr).value;
    }
#line 1837 "myparser.tab.c"
    break;

  case 30: /* expression: expression ADD term  */
#line 568 "myparser.y"
                          {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, (yyvsp[-2].expr).code, "+", (yyvsp[0].expr).code); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            (yyval.expr).value = (yyvsp[-2].expr).value + (yyvsp[0].expr).value;
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    }
#line 1856 "myparser.tab.c"
    break;

  case 31: /* expression: expression SUB term  */
#line 582 "myparser.y"
                          {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, (yyvsp[-2].expr).code, "-", (yyvsp[0].expr).code); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            (yyval.expr).value = (yyvsp[-2].expr).value - (yyvsp[0].expr).value;
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    }
#line 1875 "myparser.tab.c"
    break;

  case 32: /* term: factor  */
#line 599 "myparser.y"
             {
        (yyval.expr).code = (yyvsp[0].expr).code;
        (yyval.expr).value = (yyvsp[0].expr).value;
    }
#line 1884 "myparser.tab.c"
    break;

  case 33: /* term: term MUL factor  */
#line 603 "myparser.y"
                      {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, (yyvsp[-2].expr).code, "*", (yyvsp[0].expr).code); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            (yyval.expr).value = (yyvsp[-2].expr).value * (yyvsp[0].expr).value;
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    }
#line 1903 "myparser.tab.c"
    break;

  case 34: /* term: term DIV factor  */
#line 617 "myparser.y"
                      {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, (yyvsp[-2].expr).code, "/", (yyvsp[0].expr).code); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            if ((yyvsp[0].expr).value == 0) {
                fprintf(stderr, "Runtime error at line %d: Division by zero.\n", yylineno);
                exit(1);
            }
            (yyval.expr).value = (yyvsp[-2].expr).value / (yyvsp[0].expr).value;
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    }
#line 1926 "myparser.tab.c"
    break;

  case 35: /* term: term MOD factor  */
#line 635 "myparser.y"
                      {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, (yyvsp[-2].expr).code, "%", (yyvsp[0].expr).code); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            if ((yyvsp[0].expr).value == 0) {
                fprintf(stderr, "Runtime error at line %d: Modulo by zero.\n", yylineno);
                exit(1);
            }
            (yyval.expr).value = (yyvsp[-2].expr).value % (yyvsp[0].expr).value;
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    }
#line 1949 "myparser.tab.c"
    break;

  case 36: /* factor: OB expression CB  */
#line 656 "myparser.y"
                       {
        (yyval.expr).code = (yyvsp[-1].expr).code;
        (yyval.expr).value = (yyvsp[-1].expr).value;
    }
#line 1958 "myparser.tab.c"
    break;

  case 37: /* factor: OB INTEGER_CONST COMMA INTEGER_CONST CB  */
#line 660 "myparser.y"
                                              {
        // Handle (x, y) format for integer constants
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, (yyvsp[-1].sval), NULL, NULL); // Use the second value
            (yyval.expr).code = temp;
        } else {
            // Simulation mode - use the second value
            (yyval.expr).value = atoi((yyvsp[-1].sval));
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    }
#line 1978 "myparser.tab.c"
    break;

  case 38: /* factor: identifier  */
#line 675 "myparser.y"
                 {
        (yyval.expr).code = strdup((yyvsp[0].sval)); 
        track_allocation((yyval.expr).code);
        if (!emit_tac) {
            (yyval.expr).value = get_variable_value((yyvsp[0].sval));
        }
    }
#line 1990 "myparser.tab.c"
    break;

  case 39: /* factor: INTEGER_CONST  */
#line 682 "myparser.y"
                    {
        (yyval.expr).code = strdup((yyvsp[0].sval)); 
        track_allocation((yyval.expr).code);
        (yyval.expr).value = atoi((yyvsp[0].sval));
    }
#line 2000 "myparser.tab.c"
    break;

  case 40: /* factor: CHAR_LITERAL  */
#line 687 "myparser.y"
                   {
        (yyval.expr).code = (yyvsp[0].sval); 
        if ((yyvsp[0].sval)[0] == '\'') {
            (yyval.expr).value = (yyvsp[0].sval)[1];  // 'a' -> value of 'a'
        } else {
            (yyval.expr).value = atoi((yyvsp[0].sval));
        }
    }
#line 2013 "myparser.tab.c"
    break;

  case 41: /* identifier: IDENTIFIER  */
#line 698 "myparser.y"
                 {
        if (!is_declared((yyvsp[0].sval))) {
            fprintf(stderr, "Semantic error at line %d: Variable '%s' used before declaration.\n", yylineno, (yyvsp[0].sval));
            exit(1);
        }
        (yyval.sval) = strdup((yyvsp[0].sval)); track_allocation((yyval.sval));
    }
#line 2025 "myparser.tab.c"
    break;

  case 42: /* identifier: IDENTIFIER LBRACKET expression RBRACKET  */
#line 705 "myparser.y"
                                              {
        // Create array access identifier like "arr[3]"
        char* array_name = malloc(strlen((yyvsp[-3].sval)) + strlen((yyvsp[-1].expr).code) + 3);
        sprintf(array_name, "%s[%s]", (yyvsp[-3].sval), (yyvsp[-1].expr).code);
        
        // Check if the base array is declared
        char* base_array = strdup((yyvsp[-3].sval));
        if (!is_declared(base_array)) {
            fprintf(stderr, "Semantic error at line %d: Array '%s' used before declaration.\n", yylineno, (yyvsp[-3].sval));
            free(base_array);
            exit(1);
        }
        free(base_array);
        
        (yyval.sval) = array_name;
        track_allocation((yyval.sval));
    }
#line 2047 "myparser.tab.c"
    break;

  case 43: /* bool_expr: condition  */
#line 725 "myparser.y"
                {
        (yyval.expr).code = (yyvsp[0].expr).code;
        (yyval.expr).value = (yyvsp[0].expr).value;
    }
#line 2056 "myparser.tab.c"
    break;

  case 44: /* bool_expr: bool_expr AND bool_expr  */
#line 729 "myparser.y"
                              {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, (yyvsp[-2].expr).code, "and", (yyvsp[0].expr).code); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            (yyval.expr).value = (yyvsp[-2].expr).value && (yyvsp[0].expr).value;
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    }
#line 2075 "myparser.tab.c"
    break;

  case 45: /* bool_expr: bool_expr OR bool_expr  */
#line 743 "myparser.y"
                             {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, (yyvsp[-2].expr).code, "or", (yyvsp[0].expr).code); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            (yyval.expr).value = (yyvsp[-2].expr).value || (yyvsp[0].expr).value;
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    }
#line 2094 "myparser.tab.c"
    break;

  case 46: /* bool_expr: NOT bool_expr  */
#line 757 "myparser.y"
                    {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, "not", (yyvsp[0].expr).code, NULL); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            (yyval.expr).value = !(yyvsp[0].expr).value;
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    }
#line 2113 "myparser.tab.c"
    break;

  case 47: /* condition: expression relop expression  */
#line 774 "myparser.y"
                                  {
        if (emit_tac) {
            char* temp = newCondTemp();
            emit(temp, (yyvsp[-2].expr).code, (yyvsp[-1].expr).code, (yyvsp[0].expr).code); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            if (strcmp((yyvsp[-1].expr).code, "=") == 0)
                (yyval.expr).value = ((yyvsp[-2].expr).value == (yyvsp[0].expr).value);
            else if (strcmp((yyvsp[-1].expr).code, "!=") == 0)
                (yyval.expr).value = ((yyvsp[-2].expr).value != (yyvsp[0].expr).value);
            else if (strcmp((yyvsp[-1].expr).code, ">") == 0)
                (yyval.expr).value = ((yyvsp[-2].expr).value > (yyvsp[0].expr).value);
            else if (strcmp((yyvsp[-1].expr).code, ">=") == 0)
                (yyval.expr).value = ((yyvsp[-2].expr).value >= (yyvsp[0].expr).value);
            else if (strcmp((yyvsp[-1].expr).code, "<") == 0)
                (yyval.expr).value = ((yyvsp[-2].expr).value < (yyvsp[0].expr).value);
            else if (strcmp((yyvsp[-1].expr).code, "<=") == 0)
                (yyval.expr).value = ((yyvsp[-2].expr).value <= (yyvsp[0].expr).value);
                
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    }
#line 2144 "myparser.tab.c"
    break;

  case 48: /* relop: EQ  */
#line 803 "myparser.y"
         { (yyval.expr).code = strdup("="); track_allocation((yyval.expr).code); }
#line 2150 "myparser.tab.c"
    break;

  case 49: /* relop: NE  */
#line 804 "myparser.y"
         { (yyval.expr).code = strdup("!="); track_allocation((yyval.expr).code); }
#line 2156 "myparser.tab.c"
    break;

  case 50: /* relop: GT  */
#line 805 "myparser.y"
         { (yyval.expr).code = strdup(">"); track_allocation((yyval.expr).code); }
#line 2162 "myparser.tab.c"
    break;

  case 51: /* relop: GE  */
#line 806 "myparser.y"
         { (yyval.expr).code = strdup(">="); track_allocation((yyval.expr).code); }
#line 2168 "myparser.tab.c"
    break;

  case 52: /* relop: LT  */
#line 807 "myparser.y"
         { (yyval.expr).code = strdup("<"); track_allocation((yyval.expr).code); }
#line 2174 "myparser.tab.c"
    break;

  case 53: /* relop: LE  */
#line 808 "myparser.y"
         { (yyval.expr).code = strdup("<="); track_allocation((yyval.expr).code); }
#line 2180 "myparser.tab.c"
    break;

  case 55: /* for_dir: INC  */
#line 816 "myparser.y"
          { 
        (yyval.expr).code = strdup("inc"); track_allocation((yyval.expr).code);
        (yyval.expr).value = 1;  // For simulation
    }
#line 2189 "myparser.tab.c"
    break;

  case 56: /* for_dir: DEC  */
#line 820 "myparser.y"
          { 
        (yyval.expr).code = strdup("dec"); track_allocation((yyval.expr).code);
        (yyval.expr).value = 0;  // For simulation
    }
#line 2198 "myparser.tab.c"
    break;

  case 57: /* for_step: expression  */
#line 827 "myparser.y"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 2204 "myparser.tab.c"
    break;

  case 58: /* print: PRINT OB STRING_LITERAL formatArgs CB SEMICOLON  */
#line 831 "myparser.y"
                                                      {
        if (emit_tac) {
            if ((yyvsp[-2].sval)) {
                printf("print \"%s\", %s\n", (yyvsp[-3].sval), (yyvsp[-2].sval));
            } else {
                printf("print \"%s\"\n", (yyvsp[-3].sval));
            }
        } else {
            // Simulation mode - actually print the output
            simulate_print((yyvsp[-3].sval), (yyvsp[-2].sval));
        }
    }
#line 2221 "myparser.tab.c"
    break;

  case 59: /* scan: SCAN OB STRING_LITERAL COMMA argList CB SEMICOLON  */
#line 846 "myparser.y"
                                                        {
        if (emit_tac) {
            printf("scan \"%s\"\n", (yyvsp[-4].sval));
            if ((yyvsp[-2].sval)) printf("vars: %s\n", (yyvsp[-2].sval)); // variables list
        } else {
            // Simulation mode - actually read input values
            simulate_scan((yyvsp[-4].sval), (yyvsp[-2].sval));
        }
    }
#line 2235 "myparser.tab.c"
    break;

  case 60: /* formatArgs: %empty  */
#line 858 "myparser.y"
                  { (yyval.sval) = NULL; }
#line 2241 "myparser.tab.c"
    break;

  case 61: /* formatArgs: COMMA argList  */
#line 859 "myparser.y"
                    { (yyval.sval) = (yyvsp[0].sval); }
#line 2247 "myparser.tab.c"
    break;

  case 62: /* argList: value  */
#line 863 "myparser.y"
            {
        if (emit_tac) {
            (yyval.sval) = strdup((yyvsp[0].expr).code); track_allocation((yyval.sval));
        } else {
            char temp[20];
            sprintf(temp, "%d", (yyvsp[0].expr).value);  // Convert value to string
            (yyval.sval) = strdup(temp);
            track_allocation((yyval.sval));
        }
    }
#line 2262 "myparser.tab.c"
    break;

  case 63: /* argList: value COMMA argList  */
#line 873 "myparser.y"
                          {
        if (emit_tac) {
            char* temp = malloc(strlen((yyvsp[-2].expr).code) + strlen((yyvsp[0].sval)) + 2);
            sprintf(temp, "%s,%s", (yyvsp[-2].expr).code, (yyvsp[0].sval));
            track_allocation(temp);
            (yyval.sval) = temp;
        } else {
            char val_str[20];
            sprintf(val_str, "%d", (yyvsp[-2].expr).value);
            
            char* temp = malloc(strlen(val_str) + strlen((yyvsp[0].sval)) + 2);
            sprintf(temp, "%s,%s", val_str, (yyvsp[0].sval));
            track_allocation(temp);
            (yyval.sval) = temp;
        }
    }
#line 2283 "myparser.tab.c"
    break;

  case 64: /* value: identifier  */
#line 892 "myparser.y"
                 { 
        (yyval.expr).code = (yyvsp[0].sval); 
        if (!emit_tac) {
            (yyval.expr).value = get_variable_value((yyvsp[0].sval));
        }
    }
#line 2294 "myparser.tab.c"
    break;

  case 65: /* value: INTEGER_CONST  */
#line 898 "myparser.y"
                    { 
        (yyval.expr).code = (yyvsp[0].sval); 
        (yyval.expr).value = atoi((yyvsp[0].sval));
    }
#line 2303 "myparser.tab.c"
    break;

  case 66: /* value: CHAR_LITERAL  */
#line 902 "myparser.y"
                    { 
        (yyval.expr).code = (yyvsp[0].sval); 
        if ((yyvsp[0].sval)[0] == '\'') {
            (yyval.expr).value = (yyvsp[0].sval)[1];  // 'a' -> value of 'a'
        } else {
            (yyval.expr).value = atoi((yyvsp[0].sval));
        }
    }
#line 2316 "myparser.tab.c"
    break;

  case 70: /* VarDeclaration: OB IDENTIFIER arrayDec COMMA type CB SEMICOLON  */
#line 924 "myparser.y"
                                                     {
        // Create variable name with array suffix if needed
        char* varname;
        if ((yyvsp[-4].array_decl) != NULL) {
            // If it's an array, append the array declaration to the name for tracking
            int len = strlen((yyvsp[-5].sval)) + strlen((yyvsp[-4].array_decl)) + 1;
            varname = malloc(len);
            sprintf(varname, "%s%s", (yyvsp[-5].sval), (yyvsp[-4].array_decl));
            track_allocation(varname);
        } else {
            varname = strdup((yyvsp[-5].sval));
            track_allocation(varname);
        }
        declare_variable(varname);
    }
#line 2336 "myparser.tab.c"
    break;

  case 71: /* arrayDec: %empty  */
#line 942 "myparser.y"
                  { (yyval.array_decl) = NULL; }
#line 2342 "myparser.tab.c"
    break;

  case 72: /* arrayDec: LBRACKET INTEGER_CONST RBRACKET  */
#line 943 "myparser.y"
                                      {
        // Use INTEGER_CONST instead of DECIMAL for array size
        char* size = malloc(20);
        sprintf(size, "[%s]", (yyvsp[-1].sval));
        track_allocation(size);
        (yyval.array_decl) = size;
    }
#line 2354 "myparser.tab.c"
    break;


#line 2358 "myparser.tab.c"

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

#line 957 "myparser.y"


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
