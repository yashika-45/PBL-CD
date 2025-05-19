%{
#include "ast.h"
#include "phase2.tab.h"
#include <string.h>
%}

%%

"if"        { yylval.str = strdup(yytext); return KEYWORD; }
"else"      { yylval.str = strdup(yytext); return KEYWORD; }
"while"     { yylval.str = strdup(yytext); return KEYWORD; }
"for"       { yylval.str = strdup(yytext); return KEYWORD; }
"return"    { yylval.str = strdup(yytext); return KEYWORD; }
"struct"    { yylval.str = strdup(yytext); return KEYWORD; }

"auto"      { yylval.str = strdup(yytext); return KEYWORD; }
"break"     { yylval.str = strdup(yytext); return KEYWORD; }
"case"      { yylval.str = strdup(yytext); return KEYWORD; }
"char"      { yylval.str = strdup(yytext); return KEYWORD; }
"const"     { yylval.str = strdup(yytext); return KEYWORD; }
"continue"  { yylval.str = strdup(yytext); return KEYWORD; }
"void"   "  { yylval.str = strdup(yytext); return KEYWORD; }
"default"   { yylval.str = strdup(yytext); return KEYWORD; }
"do"        { yylval.str = strdup(yytext); return KEYWORD; }
"double"    { yylval.str = strdup(yytext); return KEYWORD; }
"enum"      { yylval.str = strdup(yytext); return KEYWORD; }
"extern"    { yylval.str = strdup(yytext); return KEYWORD; }
"float"     { yylval.str = strdup(yytext); return KEYWORD; }
"goto"      { yylval.str = strdup(yytext); return KEYWORD; }
"int"       { yylval.str = strdup(yytext); return KEYWORD; }
"long"      { yylval.str = strdup(yytext); return KEYWORD; }
"register"  { yylval.str = strdup(yytext); return KEYWORD; }
"short"     { yylval.str = strdup(yytext); return KEYWORD; }
"signed"    { yylval.str = strdup(yytext); return KEYWORD; }
"sizeof"    { yylval.str = strdup(yytext); return KEYWORD; }
"static"    { yylval.str = strdup(yytext); return KEYWORD; }
"switch"    { yylval.str = strdup(yytext); return KEYWORD; }
"typedef"   { yylval.str = strdup(yytext); return KEYWORD; }
"union"     { yylval.str = strdup(yytext); return KEYWORD; }
"unsigned   { yylval.str = strdup(yytext); return KEYWORD; }
"volatile"  { yylval.str = strdup(yytext); return KEYWORD; }

"=="        { return EQ; }
"!="        { return NEQ; }
"<="        { return LE; }
">="        { return GE; }
"<"         { return LT; }
">"         { return GT; }

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MUL; }
"/"         { return DIV; }
"="         { return ASSIGN; }

";"         { return SEMICOLON; }
","         { return COMMA; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
"["         { return LBRACKET; }
"]"         { return RBRACKET; }

[0-9]+      {
    yylval.str = strdup(yytext);
    return NUM;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = strdup(yytext);
    return ID;
}

[ \t\n]+    ; 
.           { return yytext[0]; }

%%

int yywrap() {
    return 1;
}
