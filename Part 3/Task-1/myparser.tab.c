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
#line 163 "myparser.y"
{
    int ival;
    double dval;
    char* sval;
}
/* Line 193 of yacc.c.  */
#line 381 "myparser.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 394 "myparser.tab.c"

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
#define YYLAST   133

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  71
/* YYNRULES -- Number of states.  */
#define YYNSTATES  143

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
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,    11,    18,    19,    22,    25,    28,    31,
      34,    36,    38,    40,    43,    47,    49,    51,    53,    55,
      57,    59,    60,    68,    69,    81,    82,    83,    93,    95,
      99,   103,   105,   109,   113,   117,   121,   123,   125,   127,
     129,   131,   135,   139,   142,   146,   148,   150,   152,   154,
     156,   158,   162,   164,   166,   168,   175,   183,   184,   187,
     189,   193,   195,   197,   199,   206,   208,   211,   219,   220,
     224,   226
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      60,     0,    -1,     9,    11,    34,    87,    61,    10,    11,
      -1,     9,    11,    34,    87,    61,     1,    -1,    -1,    62,
      61,    -1,    63,    35,    -1,    69,    35,    -1,    65,    35,
      -1,    67,    35,    -1,    82,    -1,    83,    -1,    79,    -1,
      79,    35,    -1,    75,    64,    72,    -1,    26,    -1,    27,
      -1,    28,    -1,    29,    -1,    30,    -1,    31,    -1,    -1,
      15,    42,    76,    43,    20,    66,    79,    -1,    -1,    16,
      75,    26,    72,    17,    72,    80,    81,    20,    68,    79,
      -1,    -1,    -1,    13,    42,    76,    43,    70,    79,    71,
      14,    79,    -1,    73,    -1,    72,    45,    73,    -1,    72,
      46,    73,    -1,    74,    -1,    73,    44,    74,    -1,    73,
      47,    74,    -1,    73,    48,    74,    -1,    42,    72,    43,
      -1,    75,    -1,     8,    -1,     7,    -1,     5,    -1,    77,
      -1,    76,    55,    76,    -1,    76,    56,    76,    -1,    57,
      76,    -1,    72,    78,    72,    -1,    49,    -1,    54,    -1,
      50,    -1,    52,    -1,    51,    -1,    53,    -1,     9,    61,
      10,    -1,    18,    -1,    19,    -1,    72,    -1,    21,    42,
       6,    84,    43,    35,    -1,    22,    42,     6,    36,    85,
      43,    35,    -1,    -1,    36,    85,    -1,    86,    -1,    86,
      36,    85,    -1,    75,    -1,     8,    -1,     7,    -1,     9,
      12,    34,    88,    10,    12,    -1,    89,    -1,    89,    88,
      -1,    42,     5,    90,    36,    91,    43,    35,    -1,    -1,
      37,     3,    38,    -1,    23,    -1,    24,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   203,   203,   206,   211,   213,   217,   218,   219,   220,
     221,   222,   223,   224,   227,   241,   242,   243,   244,   245,
     246,   253,   252,   277,   275,   328,   335,   327,   352,   353,
     357,   364,   365,   369,   373,   380,   381,   382,   383,   387,
     397,   398,   402,   406,   413,   425,   426,   427,   428,   429,
     430,   434,   439,   440,   444,   453,   463,   470,   471,   475,
     478,   487,   488,   489,   495,   499,   500,   504,   509,   511,
     515,   516
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
  "LOWER_THAN_ELSE", "$accept", "program", "statementBlock", "statement",
  "assignment", "assignment_op", "while_loop", "@1", "for_stmt", "@2",
  "if_cond", "@3", "@4", "expression", "term", "factor", "identifier",
  "bool_expr", "condition", "relop", "block", "for_dir", "for_step",
  "print", "scan", "formatArgs", "argList", "value", "varDecBlock",
  "VarDecList", "VarDeclaration", "arrayDec", "type", 0
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
       0,    59,    60,    60,    61,    61,    62,    62,    62,    62,
      62,    62,    62,    62,    63,    64,    64,    64,    64,    64,
      64,    66,    65,    68,    67,    70,    71,    69,    72,    72,
      72,    73,    73,    73,    73,    74,    74,    74,    74,    75,
      76,    76,    76,    76,    77,    78,    78,    78,    78,    78,
      78,    79,    80,    80,    81,    82,    83,    84,    84,    85,
      85,    86,    86,    86,    87,    88,    88,    89,    90,    90,
      91,    91
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
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

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,     4,     0,    39,
       4,     0,     0,     0,     0,     0,     0,     4,     0,     0,
       0,     0,     0,    12,    10,    11,     0,     0,     0,     0,
       0,     0,     0,     3,     0,     5,     6,     8,     9,     7,
      15,    16,    17,    18,    19,    20,     0,    13,     0,     0,
      65,    51,    38,    37,     0,     0,     0,    28,    31,    36,
       0,    40,     0,     0,    57,     0,     2,    14,    68,     0,
      66,     0,    43,     0,     0,    45,    47,    49,    48,    50,
      46,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,    35,    29,    30,    44,
      32,    33,    34,     0,    41,    42,    21,     0,    63,    62,
      61,    58,    59,     0,     0,     0,     0,    26,     0,     0,
       0,    55,     0,    69,    70,    71,     0,     0,    22,    52,
      53,     0,    60,    56,     0,     0,    54,     0,    67,    27,
      23,     0,    24
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,    16,    17,    18,    46,    19,   118,    20,   141,
      21,   103,   127,    56,    57,    58,    59,    60,    61,    81,
      23,   131,   137,    24,    25,    91,   111,   112,     7,    49,
      50,    94,   126
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -93
static const yytype_int8 yypact[] =
{
      12,    16,    39,    -9,   -93,    49,    36,    56,    29,   -93,
      56,    26,    28,    69,    37,    57,    13,    56,    65,    67,
      70,    71,    25,    72,   -93,   -93,    59,    74,     0,     0,
      77,    98,   102,   -93,    99,   -93,   -93,   -93,   -93,   -93,
     -93,   -93,   -93,   -93,   -93,   -93,     8,   -93,   104,   101,
      59,   -93,   -93,   -93,     8,     0,    42,    -3,   -93,   -93,
     -26,   -93,   -21,     8,    76,    78,   -93,    -8,    79,   103,
     -93,    21,   -93,     8,     8,   -93,   -93,   -93,   -93,   -93,
     -93,     8,     8,     8,     8,   -93,     0,     0,    97,   -13,
      68,    75,    68,   116,    84,   -93,   -93,    -3,    -3,    -8,
     -93,   -93,   -93,   112,   -93,    73,   -93,     8,   -93,   -93,
     -93,   -93,    86,    88,    81,    87,    66,   -93,   112,     1,
      68,   -93,    91,   -93,   -93,   -93,    89,   113,   -93,   -93,
     -93,     8,   -93,   -93,    94,   112,    -8,   110,   -93,   -93,
     -93,   112,   -93
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -93,   -93,    14,   -93,   -93,   -93,   -93,   -93,   -93,   -93,
     -93,   -93,   -93,   -45,    24,    -2,    -7,   -27,   -93,   -93,
     -92,   -93,   -93,   -93,   -93,   -93,   -80,   -93,   -93,    83,
     -93,   -93,   -93
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      22,    67,    62,    22,   107,     9,    30,    52,    53,    71,
      22,   117,   114,     9,    33,    52,    53,    85,    89,   129,
     130,     1,    88,    34,    27,     5,   128,     3,    72,    86,
      87,    35,    73,    74,    86,    87,    99,    73,    74,     4,
     132,    82,    54,   139,    83,    84,    73,    74,     8,   142,
      54,    40,    41,    42,    43,    44,    45,    55,     6,   104,
     105,     9,   119,    26,    96,    10,    73,    74,    28,    11,
      29,    12,    13,     9,     9,   108,   109,    14,    15,    31,
     100,   101,   102,   110,    51,   110,   136,    73,    74,   124,
     125,    75,    76,    77,    78,    79,    80,    97,    98,    32,
      36,    48,    37,    63,    64,    38,    39,    47,    65,    68,
      66,    69,    90,   110,    92,    95,    93,   106,   113,   115,
     116,    10,   120,   121,   122,   123,   133,   135,    86,   138,
     140,     0,   134,    70
};

static const yytype_int16 yycheck[] =
{
       7,    46,    29,    10,    17,     5,    13,     7,     8,    54,
      17,   103,    92,     5,     1,     7,     8,    43,    63,    18,
      19,     9,    43,    10,    10,    34,   118,    11,    55,    55,
      56,    17,    45,    46,    55,    56,    81,    45,    46,     0,
     120,    44,    42,   135,    47,    48,    45,    46,    12,   141,
      42,    26,    27,    28,    29,    30,    31,    57,     9,    86,
      87,     5,   107,    34,    43,     9,    45,    46,    42,    13,
      42,    15,    16,     5,     5,     7,     8,    21,    22,    42,
      82,    83,    84,    90,    10,    92,   131,    45,    46,    23,
      24,    49,    50,    51,    52,    53,    54,    73,    74,    42,
      35,    42,    35,    26,     6,    35,    35,    35,     6,     5,
      11,    10,    36,   120,    36,    12,    37,    20,    43,     3,
      36,     9,    36,    35,    43,    38,    35,    14,    55,    35,
      20,    -1,    43,    50
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     9,    60,    11,     0,    34,     9,    87,    12,     5,
       9,    13,    15,    16,    21,    22,    61,    62,    63,    65,
      67,    69,    75,    79,    82,    83,    34,    61,    42,    42,
      75,    42,    42,     1,    10,    61,    35,    35,    35,    35,
      26,    27,    28,    29,    30,    31,    64,    35,    42,    88,
      89,    10,     7,     8,    42,    57,    72,    73,    74,    75,
      76,    77,    76,    26,     6,     6,    11,    72,     5,    10,
      88,    72,    76,    45,    46,    49,    50,    51,    52,    53,
      54,    78,    44,    47,    48,    43,    55,    56,    43,    72,
      36,    84,    36,    37,    90,    12,    43,    73,    73,    72,
      74,    74,    74,    70,    76,    76,    20,    17,     7,     8,
      75,    85,    86,    43,    85,     3,    36,    79,    66,    72,
      36,    35,    43,    38,    23,    24,    91,    71,    79,    18,
      19,    80,    85,    35,    43,    14,    72,    81,    35,    79,
      20,    68,    79
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
#line 203 "myparser.y"
    {
        printf("Parsing and TAC generation completed successfully.\n");
    ;}
    break;

  case 3:
#line 206 "myparser.y"
    {
        printf("Warning: Syntax error near the end of the program. TAC generation may be incomplete.\n");
        yyerrok;
    ;}
    break;

  case 14:
#line 227 "myparser.y"
    {
        if (strcmp((yyvsp[(2) - (3)].sval), ":=") == 0) {
            emit((yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].sval), NULL, NULL);
        } else {
            char* baseOp = getBaseOperator((yyvsp[(2) - (3)].sval));
            char* temp = newTemp();
            emit(temp, (yyvsp[(1) - (3)].sval), baseOp, (yyvsp[(3) - (3)].sval));
            emit((yyvsp[(1) - (3)].sval), temp, NULL, NULL);
        }
        (yyval.sval) = strdup((yyvsp[(1) - (3)].sval)); track_allocation((yyval.sval));
    ;}
    break;

  case 15:
#line 241 "myparser.y"
    { (yyval.sval) = strdup(":="); track_allocation((yyval.sval)); ;}
    break;

  case 16:
#line 242 "myparser.y"
    { (yyval.sval) = strdup("+="); track_allocation((yyval.sval)); ;}
    break;

  case 17:
#line 243 "myparser.y"
    { (yyval.sval) = strdup("-="); track_allocation((yyval.sval)); ;}
    break;

  case 18:
#line 244 "myparser.y"
    { (yyval.sval) = strdup("*="); track_allocation((yyval.sval)); ;}
    break;

  case 19:
#line 245 "myparser.y"
    { (yyval.sval) = strdup("/="); track_allocation((yyval.sval)); ;}
    break;

  case 20:
#line 246 "myparser.y"
    { (yyval.sval) = strdup("%="); track_allocation((yyval.sval)); ;}
    break;

  case 21:
#line 253 "myparser.y"
    {
        /* 1) at “do”, generate labels & test */
        char* start = newLabel();  track_allocation(start);
        char* end   = newLabel();  track_allocation(end);
        current_loop_start = start;
        current_loop_end   = end;

        emitLabel(start);            /* L1: */
        emitCondJump((yyvsp[(3) - (5)].sval), end);       /* if t_cond1 == 0 goto L2 */
      ;}
    break;

  case 22:
#line 264 "myparser.y"
    {
        /* 2) after the block, loop back & close */
        emitGoto(current_loop_start);  
        emitLabel(current_loop_end);  /* L2: */
      ;}
    break;

  case 23:
#line 277 "myparser.y"
    {
        /* 1) init loop var */
        emit((yyvsp[(2) - (9)].sval), (yyvsp[(4) - (9)].sval), NULL, NULL);             /* a := (10, 8) */

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
        emit(bound, (yyvsp[(6) - (9)].sval), NULL, NULL);          /* t3 := b + (10, 2) */

        current_for_step_temp = newTemp();
        emit(current_for_step_temp, (yyvsp[(8) - (9)].sval), NULL, NULL);  
                                              /* t4 := (1, 10) */
        printf("\n");

        /* 4) test and conditional jump */
        char* cond = newCondTemp();
        emit(cond, (yyvsp[(2) - (9)].sval), ">", bound);           /* t_cond2 := a > t3 */
        emitCondJumpTrue(cond, end);          /* if t_cond2 == 1 goto L4 */
        printf("\n");
      ;}
    break;

  case 24:
#line 306 "myparser.y"
    {
        /* 5) body already emitted by `block` */

        /* 6) increment by saved step temp */
        char* tmp = newTemp();
        emit(tmp, (yyvsp[(2) - (11)].sval), "+", current_for_step_temp);  
        emit((yyvsp[(2) - (11)].sval), tmp, NULL, NULL);             /* a := tmp */
        printf("\n");

        /* 7) back‐edge and end label */
        emitGoto(current_for_start);
        emitLabel(current_for_end);            /* L4: */
        printf("\n");
      ;}
    break;

  case 25:
#line 328 "myparser.y"
    { 
        /* 1) at “)”, make labels and emit the conditional jump */
        __if_false = newLabel();    track_allocation(__if_false);
        __if_end   = newLabel();    track_allocation(__if_end);
        emitCondJump((yyvsp[(3) - (4)].sval), __if_false);  
      ;}
    break;

  case 26:
#line 335 "myparser.y"
    {
        /* 2) right after THEN block */
        emitGoto(__if_end);
        emitLabel(__if_false);  
      ;}
    break;

  case 27:
#line 342 "myparser.y"
    {
        /* 3) after ELSE block */
        emitLabel(__if_end);
      ;}
    break;

  case 28:
#line 352 "myparser.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].sval); ;}
    break;

  case 29:
#line 353 "myparser.y"
    {
        char* temp = newTemp();
        emit(temp, (yyvsp[(1) - (3)].sval), "+", (yyvsp[(3) - (3)].sval)); (yyval.sval) = temp;
    ;}
    break;

  case 30:
#line 357 "myparser.y"
    {
        char* temp = newTemp();
        emit(temp, (yyvsp[(1) - (3)].sval), "-", (yyvsp[(3) - (3)].sval)); (yyval.sval) = temp;
    ;}
    break;

  case 31:
#line 364 "myparser.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].sval); ;}
    break;

  case 32:
#line 365 "myparser.y"
    {
        char* temp = newTemp();
        emit(temp, (yyvsp[(1) - (3)].sval), "*", (yyvsp[(3) - (3)].sval)); (yyval.sval) = temp;
    ;}
    break;

  case 33:
#line 369 "myparser.y"
    {
        char* temp = newTemp();
        emit(temp, (yyvsp[(1) - (3)].sval), "/", (yyvsp[(3) - (3)].sval)); (yyval.sval) = temp;
    ;}
    break;

  case 34:
#line 373 "myparser.y"
    {
        char* temp = newTemp();
        emit(temp, (yyvsp[(1) - (3)].sval), "%", (yyvsp[(3) - (3)].sval)); (yyval.sval) = temp;
    ;}
    break;

  case 35:
#line 380 "myparser.y"
    { (yyval.sval) = (yyvsp[(2) - (3)].sval); ;}
    break;

  case 36:
#line 381 "myparser.y"
    { (yyval.sval) = strdup((yyvsp[(1) - (1)].sval)); track_allocation((yyval.sval)); ;}
    break;

  case 37:
#line 382 "myparser.y"
    { (yyval.sval) = strdup((yyvsp[(1) - (1)].sval)); track_allocation((yyval.sval)); ;}
    break;

  case 38:
#line 383 "myparser.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].sval); ;}
    break;

  case 39:
#line 387 "myparser.y"
    {
        if (!is_declared((yyvsp[(1) - (1)].sval))) {
            fprintf(stderr, "Semantic error: variable '%s' used before declaration.\n", (yyvsp[(1) - (1)].sval));
            exit(1);
        }
        (yyval.sval) = strdup((yyvsp[(1) - (1)].sval)); track_allocation((yyval.sval));
    ;}
    break;

  case 40:
#line 397 "myparser.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].sval); ;}
    break;

  case 41:
#line 398 "myparser.y"
    {
        char* temp = newTemp();
        emit(temp, (yyvsp[(1) - (3)].sval), "and", (yyvsp[(3) - (3)].sval)); (yyval.sval) = temp;
    ;}
    break;

  case 42:
#line 402 "myparser.y"
    {
        char* temp = newTemp();
        emit(temp, (yyvsp[(1) - (3)].sval), "or", (yyvsp[(3) - (3)].sval)); (yyval.sval) = temp;
    ;}
    break;

  case 43:
#line 406 "myparser.y"
    {
        char* temp = newTemp();
        emit(temp, "not", (yyvsp[(2) - (2)].sval), NULL); (yyval.sval) = temp;
    ;}
    break;

  case 44:
#line 413 "myparser.y"
    {
      char* tmp = newCondTemp();      
      emit(tmp, (yyvsp[(1) - (3)].sval), (yyvsp[(2) - (3)].sval), (yyvsp[(3) - (3)].sval));          /* t_cond1 := lhs relop rhs */
      (yyval.sval) = tmp;
  ;}
    break;

  case 45:
#line 425 "myparser.y"
    { (yyval.sval) = strdup("="); track_allocation((yyval.sval)); ;}
    break;

  case 46:
#line 426 "myparser.y"
    { (yyval.sval) = strdup("!="); track_allocation((yyval.sval)); ;}
    break;

  case 47:
#line 427 "myparser.y"
    { (yyval.sval) = strdup(">"); track_allocation((yyval.sval)); ;}
    break;

  case 48:
#line 428 "myparser.y"
    { (yyval.sval) = strdup(">="); track_allocation((yyval.sval)); ;}
    break;

  case 49:
#line 429 "myparser.y"
    { (yyval.sval) = strdup("<"); track_allocation((yyval.sval)); ;}
    break;

  case 50:
#line 430 "myparser.y"
    { (yyval.sval) = strdup("<="); track_allocation((yyval.sval)); ;}
    break;

  case 52:
#line 439 "myparser.y"
    { (yyval.sval) = strdup("inc"); track_allocation((yyval.sval)); ;}
    break;

  case 53:
#line 440 "myparser.y"
    { (yyval.sval) = strdup("dec"); track_allocation((yyval.sval)); ;}
    break;

  case 54:
#line 444 "myparser.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].sval); ;}
    break;

  case 55:
#line 453 "myparser.y"
    {
        if ((yyvsp[(4) - (6)].sval)) {
            printf("print \"%s\", %s\n", (yyvsp[(3) - (6)].sval), (yyvsp[(4) - (6)].sval));
        } else {
            printf("print \"%s\"\n", (yyvsp[(3) - (6)].sval));
        }
    ;}
    break;

  case 56:
#line 463 "myparser.y"
    {
        printf("scan \"%s\"\n", (yyvsp[(3) - (7)].sval));
        if ((yyvsp[(5) - (7)].sval)) printf("vars: %s\n", (yyvsp[(5) - (7)].sval)); // variables list
    ;}
    break;

  case 57:
#line 470 "myparser.y"
    { (yyval.sval) = NULL; ;}
    break;

  case 58:
#line 471 "myparser.y"
    { (yyval.sval) = (yyvsp[(2) - (2)].sval); ;}
    break;

  case 59:
#line 475 "myparser.y"
    {
        (yyval.sval) = strdup((yyvsp[(1) - (1)].sval)); track_allocation((yyval.sval));
    ;}
    break;

  case 60:
#line 478 "myparser.y"
    {
        char* temp = malloc(strlen((yyvsp[(1) - (3)].sval)) + strlen((yyvsp[(3) - (3)].sval)) + 2);
        sprintf(temp, "%s,%s", (yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].sval));
        track_allocation(temp);
        (yyval.sval) = temp;
    ;}
    break;

  case 61:
#line 487 "myparser.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].sval); ;}
    break;

  case 62:
#line 488 "myparser.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].sval); ;}
    break;

  case 63:
#line 489 "myparser.y"
    { (yyval.sval) = (yyvsp[(1) - (1)].sval); ;}
    break;

  case 67:
#line 504 "myparser.y"
    {
        declare_variable((yyvsp[(2) - (7)].sval));
    ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2112 "myparser.tab.c"
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


#line 521 "myparser.y"


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

   // cleanup_memory();
    fclose(yyin);
    return 0;
}

void yyerror(const char* s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, s);
    cleanup_memory();
    exit(1);
}

