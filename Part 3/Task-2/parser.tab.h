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

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

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
    DOUBLE = 259,                  /* DOUBLE  */
    IDENTIFIER = 260,              /* IDENTIFIER  */
    STRING_LITERAL = 261,          /* STRING_LITERAL  */
    CHAR_LITERAL = 262,            /* CHAR_LITERAL  */
    INTEGER_CONST = 263,           /* INTEGER_CONST  */
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
    INT_TYPE = 278,                /* INT_TYPE  */
    CHAR_TYPE = 279,               /* CHAR_TYPE  */
    MAIN = 280,                    /* MAIN  */
    ASSIGN = 281,                  /* ASSIGN  */
    PLUSEQ = 282,                  /* PLUSEQ  */
    MINUSEQ = 283,                 /* MINUSEQ  */
    MULEQ = 284,                   /* MULEQ  */
    DIVEQ = 285,                   /* DIVEQ  */
    MODEQ = 286,                   /* MODEQ  */
    SLCOMMENT = 287,               /* SLCOMMENT  */
    MLCOMMENT = 288,               /* MLCOMMENT  */
    COLON = 289,                   /* COLON  */
    SEMICOLON = 290,               /* SEMICOLON  */
    COMMA = 291,                   /* COMMA  */
    LBRACKET = 292,                /* LBRACKET  */
    RBRACKET = 293,                /* RBRACKET  */
    QUOTE = 294,                   /* QUOTE  */
    DQUOTE = 295,                  /* DQUOTE  */
    AT = 296,                      /* AT  */
    OB = 297,                      /* OB  */
    CB = 298,                      /* CB  */
    MUL = 299,                     /* MUL  */
    ADD = 300,                     /* ADD  */
    SUB = 301,                     /* SUB  */
    DIV = 302,                     /* DIV  */
    MOD = 303,                     /* MOD  */
    EQ = 304,                      /* EQ  */
    GT = 305,                      /* GT  */
    LT = 306,                      /* LT  */
    GE = 307,                      /* GE  */
    LE = 308,                      /* LE  */
    NE = 309,                      /* NE  */
    AND = 310,                     /* AND  */
    OR = 311,                      /* OR  */
    NOT = 312,                     /* NOT  */
    LOWER_THAN_ELSE = 313          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 163 "parser.y"

    int ival;
    double dval;
    char* sval;

#line 128 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
