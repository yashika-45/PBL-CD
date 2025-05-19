%{
#include "ast.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
void printASTToFile(struct ASTNode *node, int level, FILE *file);

void yyerror(void *root, const char *msg);
int yylex(void);
%}

%union {
    char *str;
    struct ASTNode *node;
}

%parse-param { ASTNode **root }

%token <str> ID KEYWORD NUM
%token <str> IF ELSE WHILE FOR RETURN STRUCT
%token <str> EQ NEQ LE GE LT GT
%token <str> PLUS MINUS MUL DIV ASSIGN
%token <str> SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
%token <str> AUTO BREAK CASE CHAR CONST CONTINUE DEFAULT DO DOUBLE 
%token <str> ENUM EXTERN FLOAT GOTO INT LONG REGISTER SHORT SIGNED SIZEOF 
%token <str> STATIC SWITCH TYPEDEF UNION UNSIGNED VOID VOLATILE 

%type <node> program declarations declaration function_declarations function_declaration parameter_list block statements statement assignment function_call argument_list expression

%%

program:
    declarations {
        *root = $1;
    }
    | function_declarations {
        *root = $1;
    }
    ;

declarations:
    declaration {
        $$ = $1;
    }
    | declarations declaration {
        addChild($1, $2);
        $$ = $1;
    }
    ;

declaration:
    KEYWORD ID SEMICOLON {
        $$ = createNode("Declaration");
        addChild($$, createNode($1));
        addChild($$, createNode($2));
    }
    | KEYWORD ID ASSIGN expression SEMICOLON {
        $$ = createNode("DeclarationAssignment");
        addChild($$, createNode($1));
        addChild($$, createNode($2));
        addChild($$, $4);
    }
    | KEYWORD ID LBRACKET NUM RBRACKET SEMICOLON {
        $$ = createNode("ArrayDeclaration");
        addChild($$, createNode($1));
        addChild($$, createNode($2));
        addChild($$, createNode($4));
    }
    ;

function_declarations:
    function_declaration {
        $$ = $1;
    }
    | function_declarations function_declaration {
        addChild($1, $2);
        $$ = $1;
    }
    ;

function_declaration:
    KEYWORD ID LPAREN parameter_list RPAREN block {
        $$ = createNode("FunctionDeclaration");
        addChild($$, createNode($1));
        addChild($$, createNode($2));
        addChild($$, $4);
        addChild($$, $6);
    }
    ;

parameter_list:
    KEYWORD ID {
        $$ = createNode("ParameterList");
        addChild($$, createNode($1));
        addChild($$, createNode($2));
    }
    | parameter_list COMMA KEYWORD ID {
        addChild($1, createNode($3));
        addChild($1, createNode($4));
        $$ = $1;
    }
    | /* empty */ {
        $$ = createNode("ParameterList");
    }
    ;

block:
    LBRACE statements RBRACE {
        $$ = createNode("Block");
        addChild($$, $2);
    }
    ;

statements:
    statement {
        $$ = $1;
    }
    | statements statement {
        addChild($1, $2);
        $$ = $1;
    }
    ;

statement:
    assignment
    | function_call
    | IF LPAREN expression RPAREN statement ELSE statement {
        $$ = createNode("IfElse");
        addChild($$, $3);
        addChild($$, $5);
        addChild($$, $7);
    }
    | WHILE LPAREN expression RPAREN statement {
        $$ = createNode("While");
        addChild($$, $3);
        addChild($$, $5);
    }
    | RETURN expression SEMICOLON {
        $$ = createNode("Return");
        addChild($$, $2);
    }
    ;

assignment:
    ID ASSIGN expression SEMICOLON {
        $$ = createNode("Assignment");
        addChild($$, createNode($1));
        addChild($$, $3);
    }
    ;

function_call:
    ID LPAREN argument_list RPAREN SEMICOLON {
        $$ = createNode("FunctionCall");
        addChild($$, createNode($1));
        addChild($$, $3);
    }
    ;

argument_list:
    expression {
        $$ = createNode("Arguments");
        addChild($$, $1);
    }
    | argument_list COMMA expression {
        addChild($1, $3);
        $$ = $1;
    }
    | /* empty */ {
        $$ = createNode("Arguments");
    }
    ;

expression:
    NUM {
        $$ = createNode($1);
    }
    | ID {
        $$ = createNode($1);
    }
    | expression PLUS expression {
        $$ = createNode("+");
        addChild($$, $1);
        addChild($$, $3);
    }
    | expression MINUS expression {
        $$ = createNode("-");
        addChild($$, $1);
        addChild($$, $3);
    }
    | expression MUL expression {
        $$ = createNode("*");
        addChild($$, $1);
        addChild($$, $3);
    }
    | expression DIV expression {
        $$ = createNode("/");
        addChild($$, $1);
        addChild($$, $3);
    }
    | LPAREN expression RPAREN {
        $$ = $2;
    }
    ;

%%

void yyerror(void *root, const char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
}
extern FILE *yyin;
FILE *resultFile;

int main(int argc, char *argv[]) {
    ASTNode *root = NULL;

    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            fprintf(stderr, "Failed to open input file: %s\n", argv[1]);
            return 1;
        }
    } else {
        yyin = stdin;
    }

    resultFile = fopen("result.txt", "w");
    if (!resultFile) {
        perror("Could not open result.txt");
        return 1;
    }

    if (yyparse(&root) == 0) {
        fprintf(resultFile, "Parsing completed successfully.\n");
        printASTToFile(root, 0, resultFile);
    } else {
        fprintf(resultFile, "Parsing failed.\n");
    }

    fclose(resultFile);
    if (yyin != stdin) fclose(yyin);
    return 0;
}


// Helper function to print AST to a file
void printASTToFile(ASTNode *node, int level, FILE *file) {
    if (!node) return;

    for (int i = 0; i < level; ++i) {
        fprintf(file, "  ");
    }
    fprintf(file, "%s\n", node->label);

    for (int i = 0; i < node->child_count; ++i) {
        printASTToFile(node->children[i], level + 1, file);
    }
}
