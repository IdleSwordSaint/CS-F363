/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_MYPARSER_TAB_H_INCLUDED
# define YY_YY_MYPARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 662 "myparser.y"

    #include "ast.h"  // Ensures ASTNode is known in parser.tab.h

#line 53 "myparser.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    DECIMAL = 258,                 /* DECIMAL  */
    IDENTIFIER = 259,              /* IDENTIFIER  */
    STRING_LITERAL = 260,          /* STRING_LITERAL  */
    INT_TYPE = 261,                /* INT_TYPE  */
    CHAR_TYPE = 262,               /* CHAR_TYPE  */
    CHAR_LITERAL = 263,            /* CHAR_LITERAL  */
    BEGIN_TOKEN = 264,             /* BEGIN_TOKEN  */
    END_TOKEN = 265,               /* END_TOKEN  */
    PROGRAM = 266,                 /* PROGRAM  */
    VARDECL = 267,                 /* VARDECL  */
    IF = 268,                      /* IF  */
    ELSE = 269,                    /* ELSE  */
    WHILE = 270,                   /* WHILE  */
    FOR = 271,                     /* FOR  */
    TO = 272,                      /* TO  */
    INC = 273,                     /* INC  */
    DEC = 274,                     /* DEC  */
    DO = 275,                      /* DO  */
    PRINT = 276,                   /* PRINT  */
    SCAN = 277,                    /* SCAN  */
    ASSIGN = 278,                  /* ASSIGN  */
    PLUSEQ = 279,                  /* PLUSEQ  */
    MINUSEQ = 280,                 /* MINUSEQ  */
    MULEQ = 281,                   /* MULEQ  */
    DIVEQ = 282,                   /* DIVEQ  */
    MODEQ = 283,                   /* MODEQ  */
    SLCOMMENT = 284,               /* SLCOMMENT  */
    MLCOMMENT = 285,               /* MLCOMMENT  */
    COLON = 286,                   /* COLON  */
    SEMICOLON = 287,               /* SEMICOLON  */
    COMMA = 288,                   /* COMMA  */
    LBRACKET = 289,                /* LBRACKET  */
    RBRACKET = 290,                /* RBRACKET  */
    QUOTE = 291,                   /* QUOTE  */
    DQUOTE = 292,                  /* DQUOTE  */
    AT = 293,                      /* AT  */
    ADD = 294,                     /* ADD  */
    SUB = 295,                     /* SUB  */
    MUL = 296,                     /* MUL  */
    DIV = 297,                     /* DIV  */
    MOD = 298,                     /* MOD  */
    EQ = 299,                      /* EQ  */
    GT = 300,                      /* GT  */
    LT = 301,                      /* LT  */
    GE = 302,                      /* GE  */
    LE = 303,                      /* LE  */
    NE = 304,                      /* NE  */
    OB = 305,                      /* OB  */
    CB = 306                       /* CB  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 666 "myparser.y"

    int int_val;
    char char_val;
    char* string;
    ASTNode* node;

#line 128 "myparser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_MYPARSER_TAB_H_INCLUDED  */
