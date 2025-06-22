%{
#include "phase2.tab.h"  
#include <string.h>
%}

%option noyywrap
%option yylineno

%%

"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"for"           { return FOR; }
"return"        { return RETURN; }

"auto"          { yylval.str = strdup("auto"); return KEYWORD; }
"break"         { yylval.str = strdup("break"); return KEYWORD; }
"case"          { yylval.str = strdup("case"); return KEYWORD; }
"char"          { yylval.str = strdup("char"); return KEYWORD; }
"const"         { yylval.str = strdup("const"); return KEYWORD; }
"continue"      { yylval.str = strdup("continue"); return KEYWORD; }
"default"       { yylval.str = strdup("default"); return KEYWORD; }
"do"            { yylval.str = strdup("do"); return KEYWORD; }
"double"        { yylval.str = strdup("double"); return KEYWORD; }
"enum"          { yylval.str = strdup("enum"); return KEYWORD; }
"extern"        { yylval.str = strdup("extern"); return KEYWORD; }
"float"         { yylval.str = strdup("float"); return KEYWORD; }
"goto"          { yylval.str = strdup("goto"); return KEYWORD; }
"int"           { yylval.str = strdup("int"); return KEYWORD; }
"long"          { yylval.str = strdup("long"); return KEYWORD; }
"register"      { yylval.str = strdup("register"); return KEYWORD; }
"short"         { yylval.str = strdup("short"); return KEYWORD; }
"signed"        { yylval.str = strdup("signed"); return KEYWORD; }
"sizeof"        { yylval.str = strdup("sizeof"); return KEYWORD; }
"static"        { yylval.str = strdup("static"); return KEYWORD; }
"struct"        { yylval.str = strdup("struct"); return KEYWORD; }
"switch"        { yylval.str = strdup("switch"); return KEYWORD; }
"typedef"       { yylval.str = strdup("typedef"); return KEYWORD; }
"union"         { yylval.str = strdup("union"); return KEYWORD; }
"unsigned"      { yylval.str = strdup("unsigned"); return KEYWORD; }
"void"          { yylval.str = strdup("void"); return KEYWORD; }
"volatile"      { yylval.str = strdup("volatile"); return KEYWORD; }


"=="            { return EQ; }
"!="            { return NEQ; }
"<="            { return LE; }
">="            { return GE; }
"<"             { return LT; }
">"             { return GT; }

"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MUL; }
"/"             { return DIV; }
"="             { return ASSIGN; }

";"             { return SEMICOLON; }
","             { return COMMA; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
"["             { return LBRACKET; }
"]"             { return RBRACKET; }

[0-9]+          {
    yylval.str = strdup(yytext);
    return NUM;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = strdup(yytext);
    return ID;
}

[ \t\r\n]+      ;  

. {
    fprintf(stderr, "Lexical error: Unexpected character '%s' at line %d\n", yytext, yylineno);
    return yytext[0]; 
}

%%
