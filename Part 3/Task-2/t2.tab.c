/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     DECIMAL = 258,
     DOUBLE = 259,
     IDENTIFIER = 260,
     STRING_LITERAL = 261,
     CHAR_LITERAL = 262,
     INTEGER_CONST = 263,
     BEGIN_TOKEN = 264,
     END_TOKEN = 265,
     PROGRAM = 266,
     VARDECL = 267,
     IF = 268,
     ELSE = 269,
     WHILE = 270,
     FOR = 271,
     TO = 272,
     INC = 273,
     DEC = 274,
     DO = 275,
     PRINT = 276,
     SCAN = 277,
     INT_TYPE = 278,
     CHAR_TYPE = 279,
     MAIN = 280,
     ASSIGN = 281,
     PLUSEQ = 282,
     MINUSEQ = 283,
     MULEQ = 284,
     DIVEQ = 285,
     MODEQ = 286,
     SLCOMMENT = 287,
     MLCOMMENT = 288,
     COLON = 289,
     SEMICOLON = 290,
     COMMA = 291,
     LBRACKET = 292,
     RBRACKET = 293,
     QUOTE = 294,
     DQUOTE = 295,
     AT = 296,
     OB = 297,
     CB = 298,
     MUL = 299,
     ADD = 300,
     SUB = 301,
     DIV = 302,
     MOD = 303,
     EQ = 304,
     GT = 305,
     LT = 306,
     GE = 307,
     LE = 308,
     NE = 309,
     AND = 310,
     OR = 311,
     NOT = 312,
     LOWER_THAN_ELSE = 313
   };
#endif
/* Tokens.  */
#define DECIMAL 258
#define DOUBLE 259
#define IDENTIFIER 260
#define STRING_LITERAL 261
#define CHAR_LITERAL 262
#define INTEGER_CONST 263
#define BEGIN_TOKEN 264
#define END_TOKEN 265
#define PROGRAM 266
#define VARDECL 267
#define IF 268
#define ELSE 269
#define WHILE 270
#define FOR 271
#define TO 272
#define INC 273
#define DEC 274
#define DO 275
#define PRINT 276
#define SCAN 277
#define INT_TYPE 278
#define CHAR_TYPE 279
#define MAIN 280
#define ASSIGN 281
#define PLUSEQ 282
#define MINUSEQ 283
#define MULEQ 284
#define DIVEQ 285
#define MODEQ 286
#define SLCOMMENT 287
#define MLCOMMENT 288
#define COLON 289
#define SEMICOLON 290
#define COMMA 291
#define LBRACKET 292
#define RBRACKET 293
#define QUOTE 294
#define DQUOTE 295
#define AT 296
#define OB 297
#define CB 298
#define MUL 299
#define ADD 300
#define SUB 301
#define DIV 302
#define MOD 303
#define EQ 304
#define GT 305
#define LT 306
#define GE 307
#define LE 308
#define NE 309
#define AND 310
#define OR 311
#define NOT 312
#define LOWER_THAN_ELSE 313




/* Copy the first part of user declarations.  */
#line 1 "t2.y"

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

int process_assignment(char* var, char* op, char* expr);

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
char* newTemp() {
    char* temp = malloc(10);
    sprintf(temp, "t%d", temp_count++);
    track_allocation(temp);
    return temp;
}

char* newLabel() {
    char* label = malloc(10);
    sprintf(label, "L%d", label_count++);
    track_allocation(label);
    return label;
}

// ---- Emit TAC ----
void emit(char* result, char* arg1, char* op, char* arg2) {
    if (arg2 && op) printf("%s = %s %s %s\n", result, arg1, op, arg2);
    else if (op)     printf("%s = %s %s\n", result, op, arg1);
    else             printf("%s = %s\n", result, arg1);
}

void emitCondJump(char* cond, char* label) {
    printf("ifFalse %s goto %s\n", cond, label);
}

void emitGoto(char* label) {
    printf("goto %s\n", label);
}

void emitLabel(char* label) {
    printf("%s:\n", label);
}

// ---- Variable table for declarations and simulation ----
#define MAX_VARS 1000
char* declared_vars[MAX_VARS];
int var_values[MAX_VARS];     // Store integer values for simulation
int var_initialized[MAX_VARS]; // Track if variables have been initialized
int var_count = 0;

// Flag to enable/disable TAC emission
int emit_tac = 1;  // 1 = emit TAC, 0 = simulate execution

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

int get_var_index(const char* name) {
    for (int i = 0; i < var_count; i++)
        if (strcmp(declared_vars[i], name) == 0)
            return i;
    return -1; // Not found
}

void declare_variable(const char* name) {
    char* base_name = strdup(name);
    char* bracket_pos = strchr(base_name, '[');
    
    // If this is an array, truncate the name at the bracket for validation
    if (bracket_pos != NULL) {
        *bracket_pos = '\0';  // Terminate the string at the bracket
    }
    
    if (is_declared(base_name)) {
        fprintf(stderr, "Semantic error at line %d: Variable '%s' redeclared.\n", yylineno, base_name);
        free(base_name);
        exit(1);
    }
    if (is_keyword(base_name)) {
        fprintf(stderr, "Semantic error at line %d: Keyword '%s' used as identifier.\n", yylineno, base_name);
        free(base_name);
        exit(1);
    }

    // Basic validation: starts with lowercase, one underscore max
    int len = strlen(base_name), underscore_count = 0;
    if (!islower(base_name[0])) {
        fprintf(stderr, "Invalid variable name at line %d: '%s'. Must start with lowercase.\n", yylineno, base_name);
        free(base_name);
        exit(1);
    }

    for (int i = 0; i < len; i++) {
        if (!(islower(base_name[i]) || isdigit(base_name[i]) || base_name[i] == '_')) {
            fprintf(stderr, "Invalid character in variable name '%s'.\n", base_name);
            free(base_name);
            exit(1);
        }
        if (base_name[i] == '_') underscore_count++;
    }

    if (underscore_count > 1) {
        fprintf(stderr, "Invalid variable name at line %d: '%s'. Only one underscore allowed.\n", yylineno, base_name);
        free(base_name);
        exit(1);
    }

    // Store the full name (with array brackets if present)
    declared_vars[var_count] = strdup(name);
    var_values[var_count] = 0;            // Initialize to 0
    var_initialized[var_count] = 0;       // Mark as uninitialized
    var_count++;
    free(base_name);
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

char* getBaseOperator(char* op) {
    const char* ops[][2] = {
        {"+=", "+"}, {"-=", "-"}, {"*=", "*"}, {"/=", "/"}, {"%=", "%"}, {NULL, NULL}
    };
    for (int i = 0; ops[i][0]; ++i)
        if (strcmp(op, ops[i][0]) == 0)
            return strdup(ops[i][1]);
    return strdup(op);
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


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 275 "t2.y"
{
    int ival;
    double dval;
    char* sval;
    struct {
        char* code;
        int value;
    } expr;
    char* array_decl;  // Add this for array declarations
}
/* Line 193 of yacc.c.  */
#line 497 "t2.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 510 "t2.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   153

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  73
/* YYNRULES -- Number of states.  */
#define YYNSTATES  150

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   313

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
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
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,    13,    14,    17,    20,    22,    24,
      26,    28,    30,    32,    35,    39,    41,    43,    45,    47,
      49,    51,    53,    57,    61,    63,    67,    71,    75,    79,
      85,    87,    89,    91,    93,    98,   100,   104,   108,   111,
     115,   117,   119,   121,   123,   125,   127,   131,   137,   145,
     154,   163,   173,   181,   183,   185,   187,   199,   206,   214,
     215,   218,   220,   224,   226,   228,   230,   237,   239,   242,
     250,   251,   255,   257
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      60,     0,    -1,    -1,     9,    11,    34,    84,    61,    62,
      10,    11,    -1,    -1,    63,    62,    -1,    64,    35,    -1,
      74,    -1,    75,    -1,    78,    -1,    79,    -1,    80,    -1,
      73,    -1,    73,    35,    -1,    69,    65,    66,    -1,    26,
      -1,    27,    -1,    28,    -1,    29,    -1,    30,    -1,    31,
      -1,    67,    -1,    66,    45,    67,    -1,    66,    46,    67,
      -1,    68,    -1,    67,    44,    68,    -1,    67,    47,    68,
      -1,    67,    48,    68,    -1,    42,    66,    43,    -1,    42,
       8,    36,     8,    43,    -1,    69,    -1,     8,    -1,     7,
      -1,     5,    -1,     5,    37,    66,    38,    -1,    71,    -1,
      70,    55,    70,    -1,    70,    56,    70,    -1,    57,    70,
      -1,    66,    72,    66,    -1,    49,    -1,    54,    -1,    50,
      -1,    52,    -1,    51,    -1,    53,    -1,     9,    62,    10,
      -1,    13,    42,    70,    43,    73,    -1,    13,    42,    70,
      43,    73,    14,    73,    -1,    13,    42,    70,    43,    73,
      35,    14,    73,    -1,    13,    42,    70,    43,    73,    14,
      73,    35,    -1,    13,    42,    70,    43,    73,    35,    14,
      73,    35,    -1,    15,    42,    70,    43,    20,    73,    35,
      -1,    18,    -1,    19,    -1,    66,    -1,    16,    69,    26,
      66,    17,    66,    76,    77,    20,    73,    35,    -1,    21,
      42,     6,    81,    43,    35,    -1,    22,    42,     6,    36,
      82,    43,    35,    -1,    -1,    36,    82,    -1,    83,    -1,
      83,    36,    82,    -1,    69,    -1,     8,    -1,     7,    -1,
       9,    12,    34,    85,    10,    12,    -1,    86,    -1,    86,
      85,    -1,    42,     5,    87,    36,    88,    43,    35,    -1,
      -1,    37,     8,    38,    -1,    23,    -1,    24,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   323,   323,   323,   332,   334,   338,   339,   340,   341,
     342,   343,   344,   345,   349,   369,   370,   371,   372,   373,
     374,   378,   382,   396,   413,   417,   431,   449,   470,   474,
     489,   496,   501,   513,   520,   540,   544,   558,   572,   589,
     618,   619,   620,   621,   622,   623,   627,   636,   646,   660,
     674,   688,   709,   750,   751,   755,   759,   825,   840,   852,
     853,   857,   867,   886,   892,   896,   909,   913,   914,   918,
     936,   937,   947,   948
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "DECIMAL", "DOUBLE", "IDENTIFIER",
  "STRING_LITERAL", "CHAR_LITERAL", "INTEGER_CONST", "BEGIN_TOKEN",
  "END_TOKEN", "PROGRAM", "VARDECL", "IF", "ELSE", "WHILE", "FOR", "TO",
  "INC", "DEC", "DO", "PRINT", "SCAN", "INT_TYPE", "CHAR_TYPE", "MAIN",
  "ASSIGN", "PLUSEQ", "MINUSEQ", "MULEQ", "DIVEQ", "MODEQ", "SLCOMMENT",
  "MLCOMMENT", "COLON", "SEMICOLON", "COMMA", "LBRACKET", "RBRACKET",
  "QUOTE", "DQUOTE", "AT", "OB", "CB", "MUL", "ADD", "SUB", "DIV", "MOD",
  "EQ", "GT", "LT", "GE", "LE", "NE", "AND", "OR", "NOT",
  "LOWER_THAN_ELSE", "$accept", "program", "@1", "statementBlock",
  "statement", "assignment", "assignment_op", "expression", "term",
  "factor", "identifier", "bool_expr", "condition", "relop", "block",
  "if_cond", "while_loop", "for_dir", "for_step", "for_loop", "print",
  "scan", "formatArgs", "argList", "value", "varDecBlock", "VarDecList",
  "VarDeclaration", "arrayDec", "type", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    59,    61,    60,    62,    62,    63,    63,    63,    63,
      63,    63,    63,    63,    64,    65,    65,    65,    65,    65,
      65,    66,    66,    66,    67,    67,    67,    67,    68,    68,
      68,    68,    68,    69,    69,    70,    70,    70,    70,    71,
      72,    72,    72,    72,    72,    72,    73,    74,    74,    74,
      74,    74,    75,    76,    76,    77,    78,    79,    80,    81,
      81,    82,    82,    83,    83,    83,    84,    85,    85,    86,
      87,    87,    88,    88
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     8,     0,     2,     2,     1,     1,     1,
       1,     1,     1,     2,     3,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     1,     3,     3,     3,     3,     5,
       1,     1,     1,     1,     4,     1,     3,     3,     2,     3,
       1,     1,     1,     1,     1,     1,     3,     5,     7,     8,
       8,     9,     7,     1,     1,     1,    11,     6,     7,     0,
       2,     1,     3,     1,     1,     1,     6,     1,     2,     7,
       0,     3,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,     2,     0,     4,
       0,    33,     4,     0,     0,     0,     0,     0,     0,     4,
       0,     0,    12,     7,     8,     9,    10,    11,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       6,    15,    16,    17,    18,    19,    20,     0,    13,    70,
       0,    68,    32,    31,     0,     0,    21,    24,    30,    46,
       0,     0,     0,    35,     0,     0,    59,     0,     3,    14,
       0,     0,    66,    31,     0,    34,     0,     0,     0,     0,
       0,    38,    40,    42,    44,    43,    45,    41,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      28,    22,    23,    25,    26,    27,    39,    47,    36,    37,
       0,     0,    65,    64,    63,    60,    61,     0,     0,    71,
      72,    73,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,    29,    48,     0,    52,    53,    54,     0,    62,
      58,    69,    50,    49,    55,     0,    51,     0,     0,    56
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     9,    18,    19,    20,    47,    61,    56,    57,
      58,    62,    63,    88,    22,    23,    24,   138,   145,    25,
      26,    27,    95,   115,   116,     7,    29,    30,    71,   122
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -93
static const yytype_int8 yypact[] =
{
       0,    14,    51,    25,   -93,    67,    68,   -93,    40,    62,
      37,    45,    62,    44,    48,    86,    69,    70,    97,    62,
      74,    72,    75,   -93,   -93,   -93,   -93,   -93,   108,   104,
      37,    11,   105,     6,     6,    90,   111,   112,   109,   -93,
     -93,   -93,   -93,   -93,   -93,   -93,   -93,    11,   -93,    84,
     110,   -93,   -93,   -93,    22,   -12,     2,   -93,   -93,   -93,
       6,    43,   -35,   -93,   -11,    11,    87,    88,   -93,    -3,
     117,    91,   -93,    92,     9,   -93,    11,    11,    11,    11,
      11,   -93,   -93,   -93,   -93,   -93,   -93,   -93,    11,   120,
       6,     6,   106,    -5,    65,    89,    65,    93,    33,   122,
     -93,     2,     2,   -93,   -93,   -93,    -3,    -7,   -93,    78,
     120,    11,   -93,   -93,   -93,   -93,    98,   100,    94,   -93,
     -93,   -93,    95,    96,   120,   126,   101,    20,    65,   -93,
     107,   113,   -93,   114,   120,   -93,   -93,   -93,    11,   -93,
     -93,   -93,   -93,   115,    -3,   121,   -93,   120,   116,   -93
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -93,   -93,   -93,     3,   -93,   -93,   -93,   -30,    -8,    26,
      -9,   -29,   -93,   -93,   -87,   -93,   -93,   -93,   -93,   -93,
     -93,   -93,   -93,   -92,   -93,   -93,   123,   -93,   -93,   -93
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      21,    55,   107,    21,   118,    64,    35,   124,    89,     1,
      21,    11,   111,    52,    53,    32,    11,    69,    52,    53,
      90,    91,    39,   126,    74,     3,    75,    11,   125,    52,
      73,    81,    92,    76,    77,    93,   139,   133,   136,   137,
      76,    77,    76,    77,    90,    91,    78,   143,    54,    79,
      80,     4,   100,    54,    76,    77,   120,   121,   106,     5,
     148,   108,   109,    60,    54,    76,    77,    11,   101,   102,
      11,    12,   112,   113,    10,    13,     6,    14,    15,    28,
       8,   127,    31,    16,    17,   114,    33,   114,    76,    77,
      34,    11,    82,    83,    84,    85,    86,    87,    41,    42,
      43,    44,    45,    46,   103,   104,   105,    38,   144,    40,
      48,    36,    37,    49,    50,    59,    65,    66,    67,   114,
      68,    70,    72,    94,    96,    97,   110,    98,    99,    12,
     123,   119,   117,    90,   128,   129,   135,   130,   131,   132,
     134,   147,   140,     0,     0,     0,     0,     0,   141,   142,
     146,   149,     0,    51
};

static const yytype_int16 yycheck[] =
{
       9,    31,    89,    12,    96,    34,    15,    14,    43,     9,
      19,     5,    17,     7,     8,    12,     5,    47,     7,     8,
      55,    56,    19,   110,    54,    11,    38,     5,    35,     7,
       8,    60,    43,    45,    46,    65,   128,   124,    18,    19,
      45,    46,    45,    46,    55,    56,    44,   134,    42,    47,
      48,     0,    43,    42,    45,    46,    23,    24,    88,    34,
     147,    90,    91,    57,    42,    45,    46,     5,    76,    77,
       5,     9,     7,     8,    34,    13,     9,    15,    16,    42,
      12,   111,    37,    21,    22,    94,    42,    96,    45,    46,
      42,     5,    49,    50,    51,    52,    53,    54,    26,    27,
      28,    29,    30,    31,    78,    79,    80,    10,   138,    35,
      35,    42,    42,     5,    10,    10,    26,     6,     6,   128,
      11,    37,    12,    36,    36,     8,    20,    36,    36,     9,
       8,    38,    43,    55,    36,    35,    35,    43,    43,    43,
      14,    20,    35,    -1,    -1,    -1,    -1,    -1,    35,    35,
      35,    35,    -1,    30
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     9,    60,    11,     0,    34,     9,    84,    12,    61,
      34,     5,     9,    13,    15,    16,    21,    22,    62,    63,
      64,    69,    73,    74,    75,    78,    79,    80,    42,    85,
      86,    37,    62,    42,    42,    69,    42,    42,    10,    62,
      35,    26,    27,    28,    29,    30,    31,    65,    35,     5,
      10,    85,     7,     8,    42,    66,    67,    68,    69,    10,
      57,    66,    70,    71,    70,    26,     6,     6,    11,    66,
      37,    87,    12,     8,    66,    38,    45,    46,    44,    47,
      48,    70,    49,    50,    51,    52,    53,    54,    72,    43,
      55,    56,    43,    66,    36,    81,    36,     8,    36,    36,
      43,    67,    67,    68,    68,    68,    66,    73,    70,    70,
      20,    17,     7,     8,    69,    82,    83,    43,    82,    38,
      23,    24,    88,     8,    14,    35,    73,    66,    36,    35,
      43,    43,    43,    73,    14,    35,    18,    19,    76,    82,
      35,    35,    35,    73,    66,    77,    35,    20,    73,    35
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 323 "t2.y"
    {
        // After variable declarations, switch to simulation mode
        emit_tac = 0;
    ;}
    break;

  case 3:
#line 326 "t2.y"
    {
        printf("\nProgram execution completed successfully.\n");
        print_symbol_table();
    ;}
    break;

  case 14:
#line 349 "t2.y"
    {
        if (emit_tac) {
            if (strcmp((yyvsp[(2) - (3)].expr).code, ":=") == 0) {
                emit((yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].expr).code, NULL, NULL);
            } else {
                char* baseOp = getBaseOperator((yyvsp[(2) - (3)].expr).code);
                char* temp = newTemp();
                emit(temp, (yyvsp[(1) - (3)].sval), baseOp, (yyvsp[(3) - (3)].expr).code);
                emit((yyvsp[(1) - (3)].sval), temp, NULL, NULL);
            }
            (yyval.expr).code = strdup((yyvsp[(1) - (3)].sval)); track_allocation((yyval.expr).code);
        } else {
            // Simulation mode
            process_assignment((yyvsp[(1) - (3)].sval), (yyvsp[(2) - (3)].expr).code, (yyvsp[(3) - (3)].expr).code);
            (yyval.expr).code = strdup((yyvsp[(1) - (3)].sval)); track_allocation((yyval.expr).code);
        }
    ;}
    break;

  case 15:
#line 369 "t2.y"
    { (yyval.expr).code = strdup(":="); track_allocation((yyval.expr).code); ;}
    break;

  case 16:
#line 370 "t2.y"
    { (yyval.expr).code = strdup("+="); track_allocation((yyval.expr).code); ;}
    break;

  case 17:
#line 371 "t2.y"
    { (yyval.expr).code = strdup("-="); track_allocation((yyval.expr).code); ;}
    break;

  case 18:
#line 372 "t2.y"
    { (yyval.expr).code = strdup("*="); track_allocation((yyval.expr).code); ;}
    break;

  case 19:
#line 373 "t2.y"
    { (yyval.expr).code = strdup("/="); track_allocation((yyval.expr).code); ;}
    break;

  case 20:
#line 374 "t2.y"
    { (yyval.expr).code = strdup("%="); track_allocation((yyval.expr).code); ;}
    break;

  case 21:
#line 378 "t2.y"
    {
        (yyval.expr).code = (yyvsp[(1) - (1)].expr).code;
        (yyval.expr).value = (yyvsp[(1) - (1)].expr).value;
    ;}
    break;

  case 22:
#line 382 "t2.y"
    {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, (yyvsp[(1) - (3)].expr).code, "+", (yyvsp[(3) - (3)].expr).code); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            (yyval.expr).value = (yyvsp[(1) - (3)].expr).value + (yyvsp[(3) - (3)].expr).value;
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    ;}
    break;

  case 23:
#line 396 "t2.y"
    {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, (yyvsp[(1) - (3)].expr).code, "-", (yyvsp[(3) - (3)].expr).code); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            (yyval.expr).value = (yyvsp[(1) - (3)].expr).value - (yyvsp[(3) - (3)].expr).value;
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    ;}
    break;

  case 24:
#line 413 "t2.y"
    {
        (yyval.expr).code = (yyvsp[(1) - (1)].expr).code;
        (yyval.expr).value = (yyvsp[(1) - (1)].expr).value;
    ;}
    break;

  case 25:
#line 417 "t2.y"
    {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, (yyvsp[(1) - (3)].expr).code, "*", (yyvsp[(3) - (3)].expr).code); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            (yyval.expr).value = (yyvsp[(1) - (3)].expr).value * (yyvsp[(3) - (3)].expr).value;
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    ;}
    break;

  case 26:
#line 431 "t2.y"
    {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, (yyvsp[(1) - (3)].expr).code, "/", (yyvsp[(3) - (3)].expr).code); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            if ((yyvsp[(3) - (3)].expr).value == 0) {
                fprintf(stderr, "Runtime error at line %d: Division by zero.\n", yylineno);
                exit(1);
            }
            (yyval.expr).value = (yyvsp[(1) - (3)].expr).value / (yyvsp[(3) - (3)].expr).value;
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    ;}
    break;

  case 27:
#line 449 "t2.y"
    {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, (yyvsp[(1) - (3)].expr).code, "%", (yyvsp[(3) - (3)].expr).code); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            if ((yyvsp[(3) - (3)].expr).value == 0) {
                fprintf(stderr, "Runtime error at line %d: Modulo by zero.\n", yylineno);
                exit(1);
            }
            (yyval.expr).value = (yyvsp[(1) - (3)].expr).value % (yyvsp[(3) - (3)].expr).value;
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    ;}
    break;

  case 28:
#line 470 "t2.y"
    {
        (yyval.expr).code = (yyvsp[(2) - (3)].expr).code;
        (yyval.expr).value = (yyvsp[(2) - (3)].expr).value;
    ;}
    break;

  case 29:
#line 474 "t2.y"
    {
        // Handle (x, y) format for integer constants
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, (yyvsp[(4) - (5)].sval), NULL, NULL); // Use the second value
            (yyval.expr).code = temp;
        } else {
            // Simulation mode - use the second value
            (yyval.expr).value = atoi((yyvsp[(4) - (5)].sval));
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    ;}
    break;

  case 30:
#line 489 "t2.y"
    {
        (yyval.expr).code = strdup((yyvsp[(1) - (1)].sval)); 
        track_allocation((yyval.expr).code);
        if (!emit_tac) {
            (yyval.expr).value = get_variable_value((yyvsp[(1) - (1)].sval));
        }
    ;}
    break;

  case 31:
#line 496 "t2.y"
    {
        (yyval.expr).code = strdup((yyvsp[(1) - (1)].sval)); 
        track_allocation((yyval.expr).code);
        (yyval.expr).value = atoi((yyvsp[(1) - (1)].sval));
    ;}
    break;

  case 32:
#line 501 "t2.y"
    {
        (yyval.expr).code = (yyvsp[(1) - (1)].sval);
        // Char literals are stored as their ASCII value
        if ((yyvsp[(1) - (1)].sval)[0] == '\'') {
            (yyval.expr).value = (yyvsp[(1) - (1)].sval)[1];  // 'a' -> value of 'a'
        } else {
            (yyval.expr).value = atoi((yyvsp[(1) - (1)].sval));
        }
    ;}
    break;

  case 33:
#line 513 "t2.y"
    {
        if (!is_declared((yyvsp[(1) - (1)].sval))) {
            fprintf(stderr, "Semantic error at line %d: Variable '%s' used before declaration.\n", yylineno, (yyvsp[(1) - (1)].sval));
            exit(1);
        }
        (yyval.sval) = strdup((yyvsp[(1) - (1)].sval)); track_allocation((yyval.sval));
    ;}
    break;

  case 34:
#line 520 "t2.y"
    {
        // Create array access identifier like "arr[3]"
        char* array_name = malloc(strlen((yyvsp[(1) - (4)].sval)) + strlen((yyvsp[(3) - (4)].expr).code) + 3);
        sprintf(array_name, "%s[%s]", (yyvsp[(1) - (4)].sval), (yyvsp[(3) - (4)].expr).code);
        
        // Check if the base array is declared
        char* base_array = strdup((yyvsp[(1) - (4)].sval));
        if (!is_declared(base_array)) {
            fprintf(stderr, "Semantic error at line %d: Array '%s' used before declaration.\n", yylineno, (yyvsp[(1) - (4)].sval));
            free(base_array);
            exit(1);
        }
        free(base_array);
        
        (yyval.sval) = array_name;
        track_allocation((yyval.sval));
    ;}
    break;

  case 35:
#line 540 "t2.y"
    {
        (yyval.expr).code = (yyvsp[(1) - (1)].expr).code;
        (yyval.expr).value = (yyvsp[(1) - (1)].expr).value;
    ;}
    break;

  case 36:
#line 544 "t2.y"
    {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, (yyvsp[(1) - (3)].expr).code, "and", (yyvsp[(3) - (3)].expr).code); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            (yyval.expr).value = (yyvsp[(1) - (3)].expr).value && (yyvsp[(3) - (3)].expr).value;
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    ;}
    break;

  case 37:
#line 558 "t2.y"
    {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, (yyvsp[(1) - (3)].expr).code, "or", (yyvsp[(3) - (3)].expr).code); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            (yyval.expr).value = (yyvsp[(1) - (3)].expr).value || (yyvsp[(3) - (3)].expr).value;
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    ;}
    break;

  case 38:
#line 572 "t2.y"
    {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, "not", (yyvsp[(2) - (2)].expr).code, NULL); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            (yyval.expr).value = !(yyvsp[(2) - (2)].expr).value;
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    ;}
    break;

  case 39:
#line 589 "t2.y"
    {
        if (emit_tac) {
            char* temp = newTemp();
            emit(temp, (yyvsp[(1) - (3)].expr).code, (yyvsp[(2) - (3)].expr).code, (yyvsp[(3) - (3)].expr).code); 
            (yyval.expr).code = temp;
        } else {
            // Simulation mode
            if (strcmp((yyvsp[(2) - (3)].expr).code, "=") == 0)
                (yyval.expr).value = ((yyvsp[(1) - (3)].expr).value == (yyvsp[(3) - (3)].expr).value);
            else if (strcmp((yyvsp[(2) - (3)].expr).code, "!=") == 0)
                (yyval.expr).value = ((yyvsp[(1) - (3)].expr).value != (yyvsp[(3) - (3)].expr).value);
            else if (strcmp((yyvsp[(2) - (3)].expr).code, ">") == 0)
                (yyval.expr).value = ((yyvsp[(1) - (3)].expr).value > (yyvsp[(3) - (3)].expr).value);
            else if (strcmp((yyvsp[(2) - (3)].expr).code, ">=") == 0)
                (yyval.expr).value = ((yyvsp[(1) - (3)].expr).value >= (yyvsp[(3) - (3)].expr).value);
            else if (strcmp((yyvsp[(2) - (3)].expr).code, "<") == 0)
                (yyval.expr).value = ((yyvsp[(1) - (3)].expr).value < (yyvsp[(3) - (3)].expr).value);
            else if (strcmp((yyvsp[(2) - (3)].expr).code, "<=") == 0)
                (yyval.expr).value = ((yyvsp[(1) - (3)].expr).value <= (yyvsp[(3) - (3)].expr).value);
                
            char temp[20];
            sprintf(temp, "%d", (yyval.expr).value);
            (yyval.expr).code = strdup(temp);
            track_allocation((yyval.expr).code);
        }
    ;}
    break;

  case 40:
#line 618 "t2.y"
    { (yyval.expr).code = strdup("="); track_allocation((yyval.expr).code); ;}
    break;

  case 41:
#line 619 "t2.y"
    { (yyval.expr).code = strdup("!="); track_allocation((yyval.expr).code); ;}
    break;

  case 42:
#line 620 "t2.y"
    { (yyval.expr).code = strdup(">"); track_allocation((yyval.expr).code); ;}
    break;

  case 43:
#line 621 "t2.y"
    { (yyval.expr).code = strdup(">="); track_allocation((yyval.expr).code); ;}
    break;

  case 44:
#line 622 "t2.y"
    { (yyval.expr).code = strdup("<"); track_allocation((yyval.expr).code); ;}
    break;

  case 45:
#line 623 "t2.y"
    { (yyval.expr).code = strdup("<="); track_allocation((yyval.expr).code); ;}
    break;

  case 47:
#line 636 "t2.y"
    {
        if (emit_tac) {
            char* after = newLabel();
            emitCondJump((yyvsp[(3) - (5)].expr).code, after);
            // $5 is block
            emitLabel(after);
        } else {
            // Simulation: block already executed based on condition
        }
    ;}
    break;

  case 48:
#line 646 "t2.y"
    {
        if (emit_tac) {
            char* else_label = newLabel();
            char* end_label = newLabel();
            emitCondJump((yyvsp[(3) - (7)].expr).code, else_label);
            // true block: $5
            emitGoto(end_label);
            emitLabel(else_label);
            // false block: $7
            emitLabel(end_label);
        } else {
            // Simulation: blocks already executed based on condition
        }
    ;}
    break;

  case 49:
#line 660 "t2.y"
    {
        if (emit_tac) {
            char* else_label = newLabel();
            char* end_label = newLabel();
            emitCondJump((yyvsp[(3) - (8)].expr).code, else_label);
            // true block: $5
            emitGoto(end_label);
            emitLabel(else_label);
            // false block: $8
            emitLabel(end_label);
        } else {
            // Simulation: blocks already executed based on condition
        }
    ;}
    break;

  case 50:
#line 674 "t2.y"
    {
        if (emit_tac) {
            char* else_label = newLabel();
            char* end_label = newLabel();
            emitCondJump((yyvsp[(3) - (8)].expr).code, else_label);
            // true block: $5
            emitGoto(end_label);
            emitLabel(else_label);
            // false block: $7
            emitLabel(end_label);
        } else {
            // Simulation: blocks already executed based on condition
        }
    ;}
    break;

  case 51:
#line 688 "t2.y"
    {
        if (emit_tac) {
            char* else_label = newLabel();
            char* end_label = newLabel();
            emitCondJump((yyvsp[(3) - (9)].expr).code, else_label);
            // true block: $5
            emitGoto(end_label);
            emitLabel(else_label);
            // false block: $8
            emitLabel(end_label);
        } else {
            // Simulation: blocks already executed based on condition
        }
    ;}
    break;

  case 52:
#line 709 "t2.y"
    {
        if (emit_tac) {
            char* start = newLabel();
            char* end = newLabel();

            emitLabel(start);

            // Emit fresh condition each time
            char* cond = newTemp();
            emit(cond, (yyvsp[(3) - (7)].expr).code, NULL, NULL);  // Emit the bool_expr evaluation

            emitCondJump(cond, end);

            // Emit loop body
            // block already emits statements
            emitGoto(start);
            emitLabel(end);
        } else {
            // Simulation mode
            int condition = (yyvsp[(3) - (7)].expr).value;  // Get the initial condition value
            int max_iterations = 1000; // Safety limit
            int iteration = 0;
            
            // Execute the loop body as long as condition is true
            // Note: the body is already executed once during parsing
            // This is the only iteration we can execute during direct interpretation
            
            if (!condition) {
                printf("WHILE loop condition is false, skipping loop body\n");
            } else {
                printf("WHILE loop executed (one iteration during parsing)\n");
                // We can't execute multiple iterations during direct interpretation
                // The loop body has already been executed once during parsing
            }
        }
    ;}
    break;

  case 53:
#line 750 "t2.y"
    { (yyval.expr).code = strdup("inc"); track_allocation((yyval.expr).code); (yyval.expr).value = 1; ;}
    break;

  case 54:
#line 751 "t2.y"
    { (yyval.expr).code = strdup("dec"); track_allocation((yyval.expr).code); (yyval.expr).value = 0; ;}
    break;

  case 55:
#line 755 "t2.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 56:
#line 759 "t2.y"
    {
        if (emit_tac) {
            char* loop_var = (yyvsp[(2) - (11)].sval);
            emit(loop_var, (yyvsp[(4) - (11)].expr).code, NULL, NULL); // a = t1

            char* upper = (yyvsp[(6) - (11)].expr).code;
            char* step = (yyvsp[(8) - (11)].expr).code;
            char* start_label = newLabel();
            char* end_label = newLabel();

            emitLabel(start_label);

            char* cond = newTemp();
            if (strcmp((yyvsp[(7) - (11)].expr).code, "inc") == 0)
                emit(cond, loop_var, "<=", upper);
            else
                emit(cond, loop_var, ">=", upper);

            emitCondJump(cond, end_label);
            // $10: loop body block
            char* step_temp = newTemp();
            if (strcmp((yyvsp[(7) - (11)].expr).code, "inc") == 0)
                emit(step_temp, loop_var, "+", step);
            else
                emit(step_temp, loop_var, "-", step);
            emit(loop_var, step_temp, NULL, NULL);
            emitGoto(start_label);
            emitLabel(end_label);
        } else {
            // Simulation mode
            char* loop_var = (yyvsp[(2) - (11)].sval);
            int loop_idx = get_var_index(loop_var);
            int initial_val = (yyvsp[(4) - (11)].expr).value;
            int target_val = (yyvsp[(6) - (11)].expr).value;
            int step_val = (yyvsp[(8) - (11)].expr).value;
            int is_inc = (yyvsp[(7) - (11)].expr).value;  // 1 for inc, 0 for dec
            
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
    ;}
    break;

  case 57:
#line 825 "t2.y"
    {
        if (emit_tac) {
            if ((yyvsp[(4) - (6)].sval)) {
                printf("print \"%s\", %s\n", (yyvsp[(3) - (6)].sval), (yyvsp[(4) - (6)].sval));
            } else {
                printf("print \"%s\"\n", (yyvsp[(3) - (6)].sval));
            }
        } else {
            // Simulation mode - actually print the output
            simulate_print((yyvsp[(3) - (6)].sval), (yyvsp[(4) - (6)].sval));
        }
    ;}
    break;

  case 58:
#line 840 "t2.y"
    {
        if (emit_tac) {
            printf("scan \"%s\"\n", (yyvsp[(3) - (7)].sval));
            if ((yyvsp[(5) - (7)].sval)) printf("vars: %s\n", (yyvsp[(5) - (7)].sval)); // variables list
        } else {
            // Simulation mode - actually read input values
            simulate_scan((yyvsp[(3) - (7)].sval), (yyvsp[(5) - (7)].sval));
        }
    ;}
    break;

  case 59:
#line 852 "t2.y"
    { (yyval.sval) = NULL; ;}
    break;

  case 60:
#line 853 "t2.y"
    { (yyval.sval) = (yyvsp[(2) - (2)].sval); ;}
    break;

  case 61:
#line 857 "t2.y"
    {
        if (emit_tac) {
            (yyval.sval) = strdup((yyvsp[(1) - (1)].expr).code); track_allocation((yyval.sval));
        } else {
            char temp[20];
            sprintf(temp, "%d", (yyvsp[(1) - (1)].expr).value);  // Convert value to string
            (yyval.sval) = strdup(temp);
            track_allocation((yyval.sval));
        }
    ;}
    break;

  case 62:
#line 867 "t2.y"
    {
        if (emit_tac) {
            char* temp = malloc(strlen((yyvsp[(1) - (3)].expr).code) + strlen((yyvsp[(3) - (3)].sval)) + 2);
            sprintf(temp, "%s,%s", (yyvsp[(1) - (3)].expr).code, (yyvsp[(3) - (3)].sval));
            track_allocation(temp);
            (yyval.sval) = temp;
        } else {
            char val_str[20];
            sprintf(val_str, "%d", (yyvsp[(1) - (3)].expr).value);
            
            char* temp = malloc(strlen(val_str) + strlen((yyvsp[(3) - (3)].sval)) + 2);
            sprintf(temp, "%s,%s", val_str, (yyvsp[(3) - (3)].sval));
            track_allocation(temp);
            (yyval.sval) = temp;
        }
    ;}
    break;

  case 63:
#line 886 "t2.y"
    { 
        (yyval.expr).code = (yyvsp[(1) - (1)].sval); 
        if (!emit_tac) {
            (yyval.expr).value = get_variable_value((yyvsp[(1) - (1)].sval));
        }
    ;}
    break;

  case 64:
#line 892 "t2.y"
    { 
        (yyval.expr).code = (yyvsp[(1) - (1)].sval); 
        (yyval.expr).value = atoi((yyvsp[(1) - (1)].sval));
    ;}
    break;

  case 65:
#line 896 "t2.y"
    { 
        (yyval.expr).code = (yyvsp[(1) - (1)].sval); 
        if ((yyvsp[(1) - (1)].sval)[0] == '\'') {
            (yyval.expr).value = (yyvsp[(1) - (1)].sval)[1];  // 'a' -> value of 'a'
        } else {
            (yyval.expr).value = atoi((yyvsp[(1) - (1)].sval));
        }
    ;}
    break;

  case 69:
#line 918 "t2.y"
    {
        // Create variable name with array suffix if needed
        char* varname;
        if ((yyvsp[(3) - (7)].array_decl) != NULL) {
            // If it's an array, append the array declaration to the name for tracking
            int len = strlen((yyvsp[(2) - (7)].sval)) + strlen((yyvsp[(3) - (7)].array_decl)) + 1;
            varname = malloc(len);
            sprintf(varname, "%s%s", (yyvsp[(2) - (7)].sval), (yyvsp[(3) - (7)].array_decl));
            track_allocation(varname);
        } else {
            varname = strdup((yyvsp[(2) - (7)].sval));
            track_allocation(varname);
        }
        declare_variable(varname);
    ;}
    break;

  case 70:
#line 936 "t2.y"
    { (yyval.array_decl) = NULL; ;}
    break;

  case 71:
#line 937 "t2.y"
    {
        // Use INTEGER_CONST instead of DECIMAL for array size
        char* size = malloc(20);
        sprintf(size, "[%s]", (yyvsp[(2) - (3)].sval));
        track_allocation(size);
        (yyval.array_decl) = size;
    ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2580 "t2.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 953 "t2.y"

// Function to evaluate expressions
int process_assignment(char* var, char* op, char* expr) {
    int idx = get_var_index(var);
    int val = get_value(expr);
    
    if (idx < 0) {
        fprintf(stderr, "Internal error: Assignment to undeclared variable '%s'.\n", var);
        exit(1);
    }
    
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

// Error handling function required by yacc/bison
void yyerror(const char* s) {
    fprintf(stderr, "Parse error at line %d: %s\n", yylineno, s);
    exit(1);
}

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
    
    // Enable debug tracing
    
    
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

