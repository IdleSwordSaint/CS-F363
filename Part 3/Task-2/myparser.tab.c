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
  YYSYMBOL_statementBlock = 61,            /* statementBlock  */
  YYSYMBOL_statement = 62,                 /* statement  */
  YYSYMBOL_assignment = 63,                /* assignment  */
  YYSYMBOL_assignment_op = 64,             /* assignment_op  */
  YYSYMBOL_while_loop = 65,                /* while_loop  */
  YYSYMBOL_66_1 = 66,                      /* $@1  */
  YYSYMBOL_for_stmt = 67,                  /* for_stmt  */
  YYSYMBOL_68_2 = 68,                      /* $@2  */
  YYSYMBOL_if_cond = 69,                   /* if_cond  */
  YYSYMBOL_expression = 70,                /* expression  */
  YYSYMBOL_term = 71,                      /* term  */
  YYSYMBOL_factor = 72,                    /* factor  */
  YYSYMBOL_identifier = 73,                /* identifier  */
  YYSYMBOL_bool_expr = 74,                 /* bool_expr  */
  YYSYMBOL_condition = 75,                 /* condition  */
  YYSYMBOL_relop = 76,                     /* relop  */
  YYSYMBOL_block = 77,                     /* block  */
  YYSYMBOL_for_dir = 78,                   /* for_dir  */
  YYSYMBOL_for_step = 79,                  /* for_step  */
  YYSYMBOL_print = 80,                     /* print  */
  YYSYMBOL_scan = 81,                      /* scan  */
  YYSYMBOL_formatArgs = 82,                /* formatArgs  */
  YYSYMBOL_argList = 83,                   /* argList  */
  YYSYMBOL_value = 84,                     /* value  */
  YYSYMBOL_varDecBlock = 85,               /* varDecBlock  */
  YYSYMBOL_VarDecList = 86,                /* VarDecList  */
  YYSYMBOL_VarDeclaration = 87,            /* VarDeclaration  */
  YYSYMBOL_arrayDec = 88,                  /* arrayDec  */
  YYSYMBOL_type = 89                       /* type  */
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
#define YYLAST   152

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  72
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
       0,   379,   379,   383,   389,   390,   394,   395,   396,   397,
     398,   399,   400,   401,   404,   428,   429,   430,   431,   432,
     433,   437,   437,   460,   460,   540,   552,   566,   570,   584,
     601,   605,   619,   637,   658,   662,   676,   693,   698,   709,
     716,   736,   740,   754,   768,   785,   814,   815,   816,   817,
     818,   819,   823,   827,   831,   838,   842,   857,   869,   870,
     874,   884,   903,   909,   913,   926,   930,   931,   935,   953,
     954,   964,   965
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
  "OR", "NOT", "LOWER_THAN_ELSE", "$accept", "program", "statementBlock",
  "statement", "assignment", "assignment_op", "while_loop", "$@1",
  "for_stmt", "$@2", "if_cond", "expression", "term", "factor",
  "identifier", "bool_expr", "condition", "relop", "block", "for_dir",
  "for_step", "print", "scan", "formatArgs", "argList", "value",
  "varDecBlock", "VarDecList", "VarDeclaration", "arrayDec", "type", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-89)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      30,     7,    24,     1,   -89,    36,    29,    61,    19,    10,
      61,    17,    25,    68,    42,    43,    20,    61,    51,    58,
      72,    80,    74,    82,   -89,   -89,    50,     4,   108,     0,
       0,    93,   114,   116,   -89,   112,   -89,   -89,   -89,   -89,
     -89,   -89,   -89,   -89,   -89,   -89,   -89,     4,   -89,   119,
     115,    50,   -89,   -89,     8,    -2,    31,   -89,   -89,   -89,
       0,    45,   -24,   -89,    13,     4,    90,    91,   -89,    26,
      92,   118,   -89,    95,    15,   -89,     4,     4,     4,     4,
       4,   -89,   -89,   -89,   -89,   -89,   -89,   -89,     4,     5,
       0,     0,   113,     6,   101,    85,   101,   124,    98,   -89,
     127,   -89,    31,    31,   -89,   -89,   -89,    26,   128,   122,
     -89,    83,   -89,     4,   -89,   -89,   -89,   -89,   103,   105,
      99,   106,    57,   100,   131,   128,   128,     9,   101,   -89,
     111,   -89,   -89,   -89,   104,   -89,   128,   -89,   -89,   -89,
     -89,     4,   -89,   -89,   117,   -89,    26,   121,   -89,   -89,
     128,   -89
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,     4,     0,    39,
       4,     0,     0,     0,     0,     0,     0,     4,     0,     0,
       0,     0,     0,    12,    10,    11,     0,     0,     0,     0,
       0,     0,     0,     0,     3,     0,     5,     6,     8,     9,
       7,    15,    16,    17,    18,    19,    20,     0,    13,     0,
       0,    66,    38,    37,     0,     0,    27,    30,    36,    52,
       0,     0,     0,    41,     0,     0,    58,     0,     2,    14,
      69,     0,    67,    37,     0,    40,     0,     0,     0,     0,
       0,    44,    46,    48,    50,    49,    51,    47,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,    34,    28,    29,    31,    32,    33,    45,     0,     0,
      42,    43,    21,     0,    64,    63,    62,    59,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,    70,    71,    72,     0,    35,     0,    25,    22,    53,
      54,     0,    61,    57,     0,    26,    55,     0,    68,    23,
       0,    24
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -89,   -89,    16,   -89,   -89,   -89,   -89,   -89,   -89,   -89,
     -89,   -25,    37,    32,    -7,   -26,   -89,   -89,   -88,   -89,
     -89,   -89,   -89,   -89,   -79,   -89,   -89,    97,   -89,   -89,
     -89
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,    16,    17,    18,    47,    19,   126,    20,   150,
      21,    61,    56,    57,    58,    62,    63,    88,    23,   141,
     147,    24,    25,    95,   117,   118,     7,    50,    51,    98,
     134
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      22,   109,    55,    22,    64,     9,    31,    52,    53,     9,
      22,    52,    53,     9,    10,    52,    73,   120,     3,    89,
     124,    34,    69,   113,     4,   108,    28,   139,   140,    74,
      35,    90,    91,    36,    81,     5,    75,   137,   138,     1,
      93,     8,    54,    76,    77,     6,    54,    27,   145,   142,
      54,    76,    77,    26,    76,    77,    92,    60,   101,    29,
      76,    77,   151,   107,   110,   111,     9,    30,    90,    91,
      10,    76,    77,     9,    11,    78,    12,    13,    79,    80,
     132,   133,    14,    15,    32,    33,    37,   116,   127,   116,
      76,    77,    49,    38,    82,    83,    84,    85,    86,    87,
      41,    42,    43,    44,    45,    46,     9,    39,   114,   115,
     104,   105,   106,   102,   103,    40,   146,    48,    59,    65,
      66,   116,    67,    68,    70,    71,    94,    96,   119,    97,
      99,   100,   121,   112,   122,   123,   125,    10,    90,   128,
     129,   149,   130,   135,   131,   136,   143,   144,    72,     0,
       0,     0,   148
};

static const yytype_int16 yycheck[] =
{
       7,    89,    27,    10,    30,     5,    13,     7,     8,     5,
      17,     7,     8,     5,     9,     7,     8,    96,    11,    43,
     108,     1,    47,    17,     0,    20,    10,    18,    19,    54,
      10,    55,    56,    17,    60,    34,    38,   125,   126,     9,
      65,    12,    42,    45,    46,     9,    42,    37,   136,   128,
      42,    45,    46,    34,    45,    46,    43,    57,    43,    42,
      45,    46,   150,    88,    90,    91,     5,    42,    55,    56,
       9,    45,    46,     5,    13,    44,    15,    16,    47,    48,
      23,    24,    21,    22,    42,    42,    35,    94,   113,    96,
      45,    46,    42,    35,    49,    50,    51,    52,    53,    54,
      26,    27,    28,    29,    30,    31,     5,    35,     7,     8,
      78,    79,    80,    76,    77,    35,   141,    35,    10,    26,
       6,   128,     6,    11,     5,    10,    36,    36,    43,    37,
      12,    36,     8,    20,    36,     8,    14,     9,    55,    36,
      35,    20,    43,    43,    38,    14,    35,    43,    51,    -1,
      -1,    -1,    35
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     9,    60,    11,     0,    34,     9,    85,    12,     5,
       9,    13,    15,    16,    21,    22,    61,    62,    63,    65,
      67,    69,    73,    77,    80,    81,    34,    37,    61,    42,
      42,    73,    42,    42,     1,    10,    61,    35,    35,    35,
      35,    26,    27,    28,    29,    30,    31,    64,    35,    42,
      86,    87,     7,     8,    42,    70,    71,    72,    73,    10,
      57,    70,    74,    75,    74,    26,     6,     6,    11,    70,
       5,    10,    86,     8,    70,    38,    45,    46,    44,    47,
      48,    74,    49,    50,    51,    52,    53,    54,    76,    43,
      55,    56,    43,    70,    36,    82,    36,    37,    88,    12,
      36,    43,    71,    71,    72,    72,    72,    70,    20,    77,
      74,    74,    20,    17,     7,     8,    73,    83,    84,    43,
      83,     8,    36,     8,    77,    14,    66,    70,    36,    35,
      43,    38,    23,    24,    89,    43,    14,    77,    77,    18,
      19,    78,    83,    35,    43,    77,    70,    79,    35,    20,
      68,    77
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    59,    60,    60,    61,    61,    62,    62,    62,    62,
      62,    62,    62,    62,    63,    64,    64,    64,    64,    64,
      64,    66,    65,    68,    67,    69,    69,    70,    70,    70,
      71,    71,    71,    71,    72,    72,    72,    72,    72,    73,
      73,    74,    74,    74,    74,    75,    76,    76,    76,    76,
      76,    76,    77,    78,    78,    79,    80,    81,    82,    82,
      83,    83,    84,    84,    84,    85,    86,    86,    87,    88,
      88,    89,    89
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     7,     6,     0,     2,     2,     2,     2,     2,
       1,     1,     1,     2,     3,     1,     1,     1,     1,     1,
       1,     0,     7,     0,    11,     7,     8,     1,     3,     3,
       1,     3,     3,     3,     3,     5,     1,     1,     1,     1,
       4,     1,     3,     3,     2,     3,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     6,     7,     0,     2,
       1,     3,     1,     1,     1,     6,     1,     2,     7,     0,
       3,     1,     1
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
#line 379 "myparser.y"
                                                                             {
        printf("\nProgram parsing and TAC generation completed successfully.\n");
        print_symbol_table();
    }
#line 1590 "myparser.tab.c"
    break;

  case 3: /* program: BEGIN_TOKEN PROGRAM COLON varDecBlock statementBlock error  */
#line 383 "myparser.y"
                                                                 {
        printf("Warning: Syntax error near the end of the program. TAC generation may be incomplete.\n");
        yyerrok;
    }
#line 1599 "myparser.tab.c"
    break;

  case 14: /* assignment: identifier assignment_op expression  */
#line 404 "myparser.y"
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
            int idx = get_var_index((yyvsp[-2].sval));
            if (idx >= 0) {
                process_assignment((yyvsp[-2].sval), (yyvsp[-1].expr).code, (yyvsp[0].expr).code);
                printf("Simulation: %s %s %s = %d\n", (yyvsp[-2].sval), (yyvsp[-1].expr).code, (yyvsp[0].expr).code, var_values[idx]);
            }
            (yyval.expr).code = strdup((yyvsp[-2].sval)); track_allocation((yyval.expr).code);
        }
    }
#line 1625 "myparser.tab.c"
    break;

  case 15: /* assignment_op: ASSIGN  */
#line 428 "myparser.y"
              { (yyval.expr).code = strdup(":="); track_allocation((yyval.expr).code); }
#line 1631 "myparser.tab.c"
    break;

  case 16: /* assignment_op: PLUSEQ  */
#line 429 "myparser.y"
              { (yyval.expr).code = strdup("+="); track_allocation((yyval.expr).code); }
#line 1637 "myparser.tab.c"
    break;

  case 17: /* assignment_op: MINUSEQ  */
#line 430 "myparser.y"
              { (yyval.expr).code = strdup("-="); track_allocation((yyval.expr).code); }
#line 1643 "myparser.tab.c"
    break;

  case 18: /* assignment_op: MULEQ  */
#line 431 "myparser.y"
              { (yyval.expr).code = strdup("*="); track_allocation((yyval.expr).code); }
#line 1649 "myparser.tab.c"
    break;

  case 19: /* assignment_op: DIVEQ  */
#line 432 "myparser.y"
              { (yyval.expr).code = strdup("/="); track_allocation((yyval.expr).code); }
#line 1655 "myparser.tab.c"
    break;

  case 20: /* assignment_op: MODEQ  */
#line 433 "myparser.y"
              { (yyval.expr).code = strdup("%="); track_allocation((yyval.expr).code); }
#line 1661 "myparser.tab.c"
    break;

  case 21: /* $@1: %empty  */
#line 437 "myparser.y"
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
#line 1679 "myparser.tab.c"
    break;

  case 22: /* while_loop: WHILE OB bool_expr CB DO $@1 block  */
#line 449 "myparser.y"
            {
        if (emit_tac) {
            emitGoto(current_loop_start);
            emitLabel(current_loop_end);
        } else {
            // Simulation: block already handled conditionally
        }
    }
#line 1692 "myparser.tab.c"
    break;

  case 23: /* $@2: %empty  */
#line 460 "myparser.y"
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
#line 1758 "myparser.tab.c"
    break;

  case 24: /* for_stmt: FOR identifier ASSIGN expression TO expression for_dir for_step DO $@2 block  */
#line 520 "myparser.y"
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
#line 1779 "myparser.tab.c"
    break;

  case 25: /* if_cond: IF OB bool_expr CB block ELSE block  */
#line 540 "myparser.y"
                                          {
        __if_false = newLabel();
        __if_end = newLabel();
        emitCondJump((yyvsp[-4].expr).code, __if_false);
        
        // The true block has already been parsed, so emit the jump to skip else
        emitGoto(__if_end);
        emitLabel(__if_false);
        
        // After the else block
        emitLabel(__if_end);
    }
#line 1796 "myparser.tab.c"
    break;

  case 26: /* if_cond: IF OB bool_expr CB DO block ELSE block  */
#line 552 "myparser.y"
                                             {
        // Keep this for backward compatibility
        __if_false = newLabel();
        __if_end = newLabel();
        emitCondJump((yyvsp[-5].expr).code, __if_false);
        
        emitGoto(__if_end);
        emitLabel(__if_false);
        
        emitLabel(__if_end);
    }
#line 1812 "myparser.tab.c"
    break;

  case 27: /* expression: term  */
#line 566 "myparser.y"
           {
        (yyval.expr).code = (yyvsp[0].expr).code;
        (yyval.expr).value = (yyvsp[0].expr).value;
    }
#line 1821 "myparser.tab.c"
    break;

  case 28: /* expression: expression ADD term  */
#line 570 "myparser.y"
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
#line 1840 "myparser.tab.c"
    break;

  case 29: /* expression: expression SUB term  */
#line 584 "myparser.y"
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
#line 1859 "myparser.tab.c"
    break;

  case 30: /* term: factor  */
#line 601 "myparser.y"
             {
        (yyval.expr).code = (yyvsp[0].expr).code;
        (yyval.expr).value = (yyvsp[0].expr).value;
    }
#line 1868 "myparser.tab.c"
    break;

  case 31: /* term: term MUL factor  */
#line 605 "myparser.y"
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
#line 1887 "myparser.tab.c"
    break;

  case 32: /* term: term DIV factor  */
#line 619 "myparser.y"
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
#line 1910 "myparser.tab.c"
    break;

  case 33: /* term: term MOD factor  */
#line 637 "myparser.y"
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
#line 1933 "myparser.tab.c"
    break;

  case 34: /* factor: OB expression CB  */
#line 658 "myparser.y"
                       {
        (yyval.expr).code = (yyvsp[-1].expr).code;
        (yyval.expr).value = (yyvsp[-1].expr).value;
    }
#line 1942 "myparser.tab.c"
    break;

  case 35: /* factor: OB INTEGER_CONST COMMA INTEGER_CONST CB  */
#line 662 "myparser.y"
                                              {
        // Handle (x, y) format for integer constants
        if (emit_tac) {
            (yyval.expr).code = strdup((yyvsp[-3].sval)); // Use the first value for TAC
            track_allocation((yyval.expr).code);
        } else {
            // Simulation mode - use the second value
            (yyval.expr).value = atoi((yyvsp[-3].sval)); // We'll use the first value for simulation too
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    }
#line 1961 "myparser.tab.c"
    break;

  case 36: /* factor: identifier  */
#line 676 "myparser.y"
                 {
        (yyval.expr).code = strdup((yyvsp[0].sval)); 
        track_allocation((yyval.expr).code);
        if (!emit_tac) {
            if (is_declared((yyvsp[0].sval))) {
                int idx = get_var_index((yyvsp[0].sval));
                if (var_initialized[idx]) {
                    (yyval.expr).value = var_values[idx];
                } else {
                    (yyval.expr).value = 0; // Default value for uninitialized
                    printf("Warning: Using uninitialized variable '%s'\n", (yyvsp[0].sval));
                }
            } else {
                (yyval.expr).value = 0; // Default value
            }
        }
    }
#line 1983 "myparser.tab.c"
    break;

  case 37: /* factor: INTEGER_CONST  */
#line 693 "myparser.y"
                    {
        (yyval.expr).code = strdup((yyvsp[0].sval)); 
        track_allocation((yyval.expr).code);
        (yyval.expr).value = atoi((yyvsp[0].sval));
    }
#line 1993 "myparser.tab.c"
    break;

  case 38: /* factor: CHAR_LITERAL  */
#line 698 "myparser.y"
                   {
        (yyval.expr).code = (yyvsp[0].sval); 
        if ((yyvsp[0].sval)[0] == '\'') {
            (yyval.expr).value = (yyvsp[0].sval)[1];  // 'a' -> value of 'a'
        } else {
            (yyval.expr).value = atoi((yyvsp[0].sval));
        }
    }
#line 2006 "myparser.tab.c"
    break;

  case 39: /* identifier: IDENTIFIER  */
#line 709 "myparser.y"
                 {
        if (!is_declared((yyvsp[0].sval))) {
            fprintf(stderr, "Semantic error at line %d: Variable '%s' used before declaration.\n", yylineno, (yyvsp[0].sval));
            exit(1);
        }
        (yyval.sval) = strdup((yyvsp[0].sval)); track_allocation((yyval.sval));
    }
#line 2018 "myparser.tab.c"
    break;

  case 40: /* identifier: IDENTIFIER LBRACKET expression RBRACKET  */
#line 716 "myparser.y"
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
#line 2040 "myparser.tab.c"
    break;

  case 41: /* bool_expr: condition  */
#line 736 "myparser.y"
                {
        (yyval.expr).code = (yyvsp[0].expr).code;
        (yyval.expr).value = (yyvsp[0].expr).value;
    }
#line 2049 "myparser.tab.c"
    break;

  case 42: /* bool_expr: bool_expr AND bool_expr  */
#line 740 "myparser.y"
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
#line 2068 "myparser.tab.c"
    break;

  case 43: /* bool_expr: bool_expr OR bool_expr  */
#line 754 "myparser.y"
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
#line 2087 "myparser.tab.c"
    break;

  case 44: /* bool_expr: NOT bool_expr  */
#line 768 "myparser.y"
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
#line 2106 "myparser.tab.c"
    break;

  case 45: /* condition: expression relop expression  */
#line 785 "myparser.y"
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
#line 2137 "myparser.tab.c"
    break;

  case 46: /* relop: EQ  */
#line 814 "myparser.y"
         { (yyval.expr).code = strdup("="); track_allocation((yyval.expr).code); }
#line 2143 "myparser.tab.c"
    break;

  case 47: /* relop: NE  */
#line 815 "myparser.y"
         { (yyval.expr).code = strdup("!="); track_allocation((yyval.expr).code); }
#line 2149 "myparser.tab.c"
    break;

  case 48: /* relop: GT  */
#line 816 "myparser.y"
         { (yyval.expr).code = strdup(">"); track_allocation((yyval.expr).code); }
#line 2155 "myparser.tab.c"
    break;

  case 49: /* relop: GE  */
#line 817 "myparser.y"
         { (yyval.expr).code = strdup(">="); track_allocation((yyval.expr).code); }
#line 2161 "myparser.tab.c"
    break;

  case 50: /* relop: LT  */
#line 818 "myparser.y"
         { (yyval.expr).code = strdup("<"); track_allocation((yyval.expr).code); }
#line 2167 "myparser.tab.c"
    break;

  case 51: /* relop: LE  */
#line 819 "myparser.y"
         { (yyval.expr).code = strdup("<="); track_allocation((yyval.expr).code); }
#line 2173 "myparser.tab.c"
    break;

  case 53: /* for_dir: INC  */
#line 827 "myparser.y"
          { 
        (yyval.expr).code = strdup("inc"); track_allocation((yyval.expr).code);
        (yyval.expr).value = 1;  // For simulation
    }
#line 2182 "myparser.tab.c"
    break;

  case 54: /* for_dir: DEC  */
#line 831 "myparser.y"
          { 
        (yyval.expr).code = strdup("dec"); track_allocation((yyval.expr).code);
        (yyval.expr).value = 0;  // For simulation
    }
#line 2191 "myparser.tab.c"
    break;

  case 55: /* for_step: expression  */
#line 838 "myparser.y"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 2197 "myparser.tab.c"
    break;

  case 56: /* print: PRINT OB STRING_LITERAL formatArgs CB SEMICOLON  */
#line 842 "myparser.y"
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
#line 2214 "myparser.tab.c"
    break;

  case 57: /* scan: SCAN OB STRING_LITERAL COMMA argList CB SEMICOLON  */
#line 857 "myparser.y"
                                                        {
        if (emit_tac) {
            printf("scan \"%s\"\n", (yyvsp[-4].sval));
            if ((yyvsp[-2].sval)) printf("vars: %s\n", (yyvsp[-2].sval)); // variables list
        } else {
            // Simulation mode - actually read input values
            simulate_scan((yyvsp[-4].sval), (yyvsp[-2].sval));
        }
    }
#line 2228 "myparser.tab.c"
    break;

  case 58: /* formatArgs: %empty  */
#line 869 "myparser.y"
                  { (yyval.sval) = NULL; }
#line 2234 "myparser.tab.c"
    break;

  case 59: /* formatArgs: COMMA argList  */
#line 870 "myparser.y"
                    { (yyval.sval) = (yyvsp[0].sval); }
#line 2240 "myparser.tab.c"
    break;

  case 60: /* argList: value  */
#line 874 "myparser.y"
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
#line 2255 "myparser.tab.c"
    break;

  case 61: /* argList: value COMMA argList  */
#line 884 "myparser.y"
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
#line 2276 "myparser.tab.c"
    break;

  case 62: /* value: identifier  */
#line 903 "myparser.y"
                 { 
        (yyval.expr).code = (yyvsp[0].sval); 
        if (!emit_tac) {
            (yyval.expr).value = get_variable_value((yyvsp[0].sval));
        }
    }
#line 2287 "myparser.tab.c"
    break;

  case 63: /* value: INTEGER_CONST  */
#line 909 "myparser.y"
                    { 
        (yyval.expr).code = (yyvsp[0].sval); 
        (yyval.expr).value = atoi((yyvsp[0].sval));
    }
#line 2296 "myparser.tab.c"
    break;

  case 64: /* value: CHAR_LITERAL  */
#line 913 "myparser.y"
                    { 
        (yyval.expr).code = (yyvsp[0].sval); 
        if ((yyvsp[0].sval)[0] == '\'') {
            (yyval.expr).value = (yyvsp[0].sval)[1];  // 'a' -> value of 'a'
        } else {
            (yyval.expr).value = atoi((yyvsp[0].sval));
        }
    }
#line 2309 "myparser.tab.c"
    break;

  case 68: /* VarDeclaration: OB IDENTIFIER arrayDec COMMA type CB SEMICOLON  */
#line 935 "myparser.y"
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
#line 2329 "myparser.tab.c"
    break;

  case 69: /* arrayDec: %empty  */
#line 953 "myparser.y"
                  { (yyval.array_decl) = NULL; }
#line 2335 "myparser.tab.c"
    break;

  case 70: /* arrayDec: LBRACKET INTEGER_CONST RBRACKET  */
#line 954 "myparser.y"
                                      {
        // Use INTEGER_CONST instead of DECIMAL for array size
        char* size = malloc(20);
        sprintf(size, "[%s]", (yyvsp[-1].sval));
        track_allocation(size);
        (yyval.array_decl) = size;
    }
#line 2347 "myparser.tab.c"
    break;


#line 2351 "myparser.tab.c"

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

#line 968 "myparser.y"


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
    
    // Parse the program
    if (yyparse() == 0) {
        printf("\nParsing completed successfully.\n");
    }
    
    // Clean up
    cleanup_memory();
    if (yyin != stdin) fclose(yyin);
    
    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "Parse error at line %d: %s\n", yylineno, s);
    exit(1);
}
