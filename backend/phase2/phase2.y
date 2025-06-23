%code requires {
    #include "ast.h"
}
%{
#include "ast.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
extern int yylex();
extern FILE *yyin;
extern int yylineno;
ASTNode* ast_root = NULL;
void yyerror(ASTNode** root, const char* s);
%}

%union {
    char *str;
    ASTNode *node;
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

%type <node> program statement expression_statement declaration
%type <node> expression assignment_expression primary_expression
%type <node> function_definition
%type <node> parameter_list 
%type <node> compound_statement 
%type <node> statement_list 
%type <node> selection_statement 
%type <node> iteration_statement 
%type <node> return_statement 
%type <node> type_specifier 
%type <node> expression_list 

%left EQ NEQ
%left LT GT LE GE
%left PLUS MINUS
%left MUL DIV
%right ASSIGN

%start program
%%
program:
    program declaration
    { $$ = $1; addChild($$, $2); *root = $$; fprintf(stderr, "✅ root set in program declaration\n"); }
|   program function_definition
    { $$ = $1; addChild($$, $2); *root = $$; fprintf(stderr, "✅ root set in function definition\n"); }
|   /* empty */
    { $$ = createNode("Program"); *root = $$; fprintf(stderr, "✅ root set to empty program\n"); }
;

declaration:
    type_specifier ID SEMICOLON
    {
        $$ = createNode("VariableDeclaration");
        addChild($$, $1);
        addChild($$, createNode($2));
    }
|   type_specifier ID ASSIGN expression SEMICOLON
    {
        $$ = createNode("Assignment");
         addChild($$, createNode($2));  // 'a'
         addChild($$, $4);
    }
|   error SEMICOLON
    {
        $$ = createNode("ErrorDeclaration");
        fprintf(stderr, "Syntax error in declaration at line %d\n", yylineno);
    }
;

type_specifier:
    KEYWORD { $$ = createNode("Type"); addChild($$, createNode($1)); }
|   ID      { $$ = createNode("Type"); addChild($$, createNode($1)); }
;

function_definition:
    type_specifier ID LPAREN parameter_list RPAREN compound_statement
    {
        $$ = createNode("FunctionDefinition");
        addChild($$, $1);
        addChild($$, createNode($2));
        addChild($$, $4);
        addChild($$, $6);
    }
|   type_specifier ID LPAREN RPAREN compound_statement
    {
        $$ = createNode("FunctionDefinition");
        addChild($$, $1);
        addChild($$, createNode($2));
        addChild($$, createNode("EmptyParameterList"));
        addChild($$, $5);
    }
;

parameter_list:
    type_specifier ID
    {
        $$ = createNode("ParameterList");
        ASTNode *param = createNode("Parameter");
        addChild(param, $1);
        addChild(param, createNode($2));
        addChild($$, param);
    }
|   parameter_list COMMA type_specifier ID
    {
        $$ = $1;
        ASTNode *param = createNode("Parameter");
        addChild(param, $3);
        addChild(param, createNode($4));
        addChild($$, param);
    }
;

compound_statement:
    LBRACE statement_list RBRACE
    {
        $$ = createNode("CompoundStatement");
        addChild($$, $2);
    }
|   LBRACE RBRACE
    {
        $$ = createNode("CompoundStatement");
        addChild($$, createNode("EmptyStatementList"));
    }
;

statement_list:
    /* empty */
    { $$ = createNode("StatementList"); }
|   statement_list statement
    {
        if (strcmp($1->label, "StatementList") != 0) {
            ASTNode *new_list = createNode("StatementList");
            addChild(new_list, $1);
            addChild(new_list, $2);
            $$ = new_list;
        } else {
            $$ = $1;
            addChild($$, $2);
        }
    }
|   statement_list declaration
    {
        if (strcmp($1->label, "StatementList") != 0) {
            ASTNode *new_list = createNode("StatementList");
            addChild(new_list, $1);
            addChild(new_list, $2);
            $$ = new_list;
        } else {
            $$ = $1;
            addChild($$, $2);
        }
    }
;

statement:
    expression_statement
|   compound_statement
|   selection_statement
|   iteration_statement
|   return_statement
|   error SEMICOLON {
        $$ = createNode("ErrorStatement");
        fprintf(stderr, "Syntax error in statement at line %d\n", yylineno);
    }
;

expression_statement:
    expression SEMICOLON
    { $$ = createNode("ExpressionStatement"); addChild($$, $1); }
|   SEMICOLON
    { $$ = createNode("EmptyStatement"); }
;

selection_statement:
    IF LPAREN expression RPAREN statement
    {
        $$ = createNode("IfStatement");
        addChild($$, $3);
        addChild($$, $5);
    }
|   IF LPAREN expression RPAREN statement ELSE statement
    {
        $$ = createNode("IfElseStatement");
        addChild($$, $3);
        addChild($$, $5);
        addChild($$, $7);
    }
;

iteration_statement:
    WHILE LPAREN expression RPAREN statement
    {
        $$ = createNode("WhileLoop");
        addChild($$, $3);
        addChild($$, $5);
    }
|   FOR LPAREN expression_statement expression_statement expression RPAREN statement
    {
        $$ = createNode("ForLoop");
        addChild($$, $3);
        addChild($$, $4);
        addChild($$, $5);
        addChild($$, $7);
    }
;

return_statement:
    RETURN expression SEMICOLON
    { $$ = createNode("ReturnStatement"); addChild($$, $2); }
|   RETURN SEMICOLON
    { $$ = createNode("ReturnStatement"); addChild($$, createNode("VoidReturn")); }
;

expression:
    assignment_expression { $$ = $1; }
;

assignment_expression:
    ID ASSIGN expression
    {
        $$ = createNode("Assignment");
        addChild($$, createNode($1));
        addChild($$, $3);
    }
|   primary_expression { $$ = $1; }
|   expression PLUS expression { $$ = createNode("Addition"); addChild($$, $1); addChild($$, $3); }
|   expression MINUS expression { $$ = createNode("Subtraction"); addChild($$, $1); addChild($$, $3); }
|   expression MUL expression { $$ = createNode("Multiplication"); addChild($$, $1); addChild($$, $3); }
|   expression DIV expression { $$ = createNode("Division"); addChild($$, $1); addChild($$, $3); }
|   expression EQ expression { $$ = createNode("Equality"); addChild($$, $1); addChild($$, $3); }
|   expression NEQ expression { $$ = createNode("Inequality"); addChild($$, $1); addChild($$, $3); }
|   expression LT expression { $$ = createNode("LessThan"); addChild($$, $1); addChild($$, $3); }
|   expression GT expression { $$ = createNode("GreaterThan"); addChild($$, $1); addChild($$, $3); }
|   expression LE expression { $$ = createNode("LessThanOrEqual"); addChild($$, $1); addChild($$, $3); }
|   expression GE expression { $$ = createNode("GreaterThanOrEqual"); addChild($$, $1); addChild($$, $3); }
;

primary_expression:
    ID { $$ = createNode("Identifier"); addChild($$, createNode($1)); }
|   NUM { $$ = createNode("Number"); addChild($$, createNode($1)); }
|   LPAREN expression RPAREN { $$ = $2; }
|   ID LPAREN RPAREN
    {
        $$ = createNode("FunctionCall");
        addChild($$, createNode($1));
        addChild($$, createNode("EmptyArgumentList"));
    }
|   ID LPAREN expression_list RPAREN
    {
        $$ = createNode("FunctionCall");
        addChild($$, createNode($1));
        addChild($$, $3);
    }
;

expression_list:
    expression { $$ = createNode("ArgumentList"); addChild($$, $1); }
|   expression_list COMMA expression { $$ = $1; addChild($$, $3); }
;

%%
void yyerror(ASTNode **root, const char *s) {
    FILE *errFile = fopen("syntax_errors.txt", "a");
    if (errFile) {
        fprintf(errFile, "Syntax error: %s at line %d\n", s, yylineno);
        fclose(errFile);
    }
}

FILE *resultFile;

int main(int argc, char *argv[]) {
    ASTNode *root = NULL;


FILE *errFile = fopen("syntax_errors.txt", "w");
if (errFile) fclose(errFile);
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

        FILE *jsonFile = fopen("tree.json", "w");
        if (!jsonFile) {
            perror("Could not open tree.json");
            return 1;
        }
        printASTToJSON(root, jsonFile);
        fclose(jsonFile);
        fprintf(stderr, "✅ Finished writing JSON\n");
    } else {
        fprintf(resultFile, "Parsing failed.\n");
        FILE *jsonFile = fopen("tree.json", "w");
        if (jsonFile) {
            fprintf(jsonFile, "{ \"error\": \"Syntax error occurred. AST not generated.\" }");
            fclose(jsonFile);
        }
    }

    fclose(resultFile);
    if (yyin != stdin) fclose(yyin);
    return 0;
}