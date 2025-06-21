/* A Bison parser, made by GNU Bison 3.5.0.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_SEMANTIC_TAB_H_INCLUDED
# define YY_YY_PARSER_SEMANTIC_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "parser_semantic.y"

    #include "ast.h"

#line 52 "parser_semantic.tab.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    KEYWORD = 259,
    NUM = 260,
    IF = 261,
    ELSE = 262,
    WHILE = 263,
    FOR = 264,
    RETURN = 265,
    STRUCT = 266,
    EQ = 267,
    NEQ = 268,
    LE = 269,
    GE = 270,
    LT = 271,
    GT = 272,
    PLUS = 273,
    MINUS = 274,
    MUL = 275,
    DIV = 276,
    ASSIGN = 277,
    SEMICOLON = 278,
    COMMA = 279,
    LPAREN = 280,
    RPAREN = 281,
    LBRACE = 282,
    RBRACE = 283,
    LBRACKET = 284,
    RBRACKET = 285,
    AUTO = 286,
    BREAK = 287,
    CASE = 288,
    CHAR = 289,
    CONST = 290,
    CONTINUE = 291,
    DEFAULT = 292,
    DO = 293,
    DOUBLE = 294,
    ENUM = 295,
    EXTERN = 296,
    FLOAT = 297,
    GOTO = 298,
    INT = 299,
    LONG = 300,
    REGISTER = 301,
    SHORT = 302,
    SIGNED = 303,
    SIZEOF = 304,
    STATIC = 305,
    SWITCH = 306,
    TYPEDEF = 307,
    UNION = 308,
    UNSIGNED = 309,
    VOID = 310,
    VOLATILE = 311
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 16 "parser_semantic.y"

    char *str;
    ASTNode *node;

#line 125 "parser_semantic.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (ASTNode **root);

#endif /* !YY_YY_PARSER_SEMANTIC_TAB_H_INCLUDED  */
