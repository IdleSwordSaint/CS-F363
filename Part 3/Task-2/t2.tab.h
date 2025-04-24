/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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
/* Line 1529 of yacc.c.  */
#line 176 "t2.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

