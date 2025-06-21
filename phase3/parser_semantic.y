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
%type <node> statement_list // New: for list of statements in a block
%type <node> selection_statement // New: for if-else
%type <node> iteration_statement // New: for loops
%type <node> return_statement // New: for return
%type <node> type_specifier // New: for int, void, etc. as types
%type <node> expression_list // Crucial for printf(c)

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
        addChild($$, $1); // Type
        addChild($$, createNode($2)); // Identifier
    }
|   type_specifier ID ASSIGN expression SEMICOLON
    {
        $$ = createNode("VariableDeclarationWithInit");
        addChild($$, $1); // Type
        addChild($$, createNode($2)); // Identifier
        addChild($$, $4); // Initializer expression
    }
;
type_specifier:
    KEYWORD { $$ = createNode("Type"); addChild($$, createNode($1)); }
|   ID      { $$ = createNode("Type"); addChild($$, createNode($1)); } // For user-defined types (e.g., struct names)
;
function_definition:
    type_specifier ID LPAREN parameter_list RPAREN compound_statement
    {
        $$ = createNode("FunctionDefinition");
        addChild($$, $1); // Return Type (e.g., "int")
        addChild($$, createNode($2)); // Function Name (e.g., "main")
        addChild($$, $4); // Parameter List
        addChild($$, $6); // Function Body (compound statement)
    }
|   type_specifier ID LPAREN RPAREN compound_statement // Function with no parameters
    {
        $$ = createNode("FunctionDefinition");
        addChild($$, $1); // Return Type
        addChild($$, createNode($2)); // Function Name
        addChild($$, createNode("EmptyParameterList")); // Explicitly mark empty
        addChild($$, $5); // Function Body
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
        $$ = $1; // Reuse the existing ParameterList node
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
        addChild($$, $2); // Add the list of statements
    }
|   LBRACE RBRACE
    {
        $$ = createNode("CompoundStatement");
        addChild($$, createNode("EmptyStatementList")); // Empty block
    }
;

// Rule for a list of statements within a compound statement
statement_list:
    /* empty */
    { $$ = createNode("StatementList"); } // Can be an empty list
|   statement_list statement
    {
        // This logic helps ensure a proper StatementList node is always the parent
        // of a sequence of statements.
        if (strcmp($1->label, "StatementList") != 0) {
            // This case should ideally not be hit if program starts with empty list
            // or if logic is simpler. For robustness:
            ASTNode *new_list = createNode("StatementList");
            addChild(new_list, $1); // Add the previous statement
            addChild(new_list, $2); // Add the current statement
            $$ = new_list;
        } else {
            $$ = $1; // Continue with existing list
            addChild($$, $2); // Add the new statement
        }
    }
|   statement_list declaration // Allow local declarations within blocks
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


// Rule for a generic statement
statement:
    expression_statement
|   compound_statement
|   selection_statement // For if-else
|   iteration_statement // For loops
|   return_statement // For return statements
;

expression_statement:
    expression SEMICOLON
    { $$ = createNode("ExpressionStatement"); addChild($$, $1); }
|   SEMICOLON
    { $$ = createNode("EmptyStatement"); }
;

// Rule for Selection Statement (if-else)
selection_statement:
    IF LPAREN expression RPAREN statement
    {
        $$ = createNode("IfStatement");
        addChild($$, $3); // Condition
        addChild($$, $5); // If body
    }
|   IF LPAREN expression RPAREN statement ELSE statement
    {
        $$ = createNode("IfElseStatement");
        addChild($$, $3); // Condition
        addChild($$, $5); // If body
        addChild($$, $7); // Else body
    }
;
iteration_statement:
    WHILE LPAREN expression RPAREN statement
    {
        $$ = createNode("WhileLoop");
        addChild($$, $3); // Condition
        addChild($$, $5); // Loop Body
    }
|   FOR LPAREN expression_statement expression_statement expression RPAREN statement
    {
        $$ = createNode("ForLoop");
        addChild($$, $3); // Initialization (can be empty)
        addChild($$, $4); // Condition (can be empty)
        addChild($$, $5); // Increment/Decrement (no semicolon needed here)
        addChild($$, $7); // Loop Body
    }
;
return_statement:
    RETURN expression SEMICOLON
    { $$ = createNode("ReturnStatement"); addChild($$, $2); }
|   RETURN SEMICOLON
    { $$ = createNode("ReturnStatement"); addChild($$, createNode("VoidReturn")); } // For 'return;'
;

// --- Expressions (simplified) ---
expression:
    assignment_expression
    { $$ = $1; }
;

assignment_expression:
    ID ASSIGN expression
    {
        $$ = createNode("Assignment");
        addChild($$, createNode($1)); // Variable being assigned to
        addChild($$, $3); // Value being assigned
    }
|   primary_expression // If not an assignment, it's just a primary expression
    { $$ = $1; }
|   expression PLUS expression // Simple binary operations
    { $$ = createNode("Addition"); addChild($$, $1); addChild($$, $3); }
|   expression MINUS expression
    { $$ = createNode("Subtraction"); addChild($$, $1); addChild($$, $3); }
|   expression MUL expression
    { $$ = createNode("Multiplication"); addChild($$, $1); addChild($$, $3); }
|   expression DIV expression
    { $$ = createNode("Division"); addChild($$, $1); addChild($$, $3); }
|   expression EQ expression
    { $$ = createNode("Equality"); addChild($$, $1); addChild($$, $3); }
|   expression NEQ expression
    { $$ = createNode("Inequality"); addChild($$, $1); addChild($$, $3); }
|   expression LT expression
    { $$ = createNode("LessThan"); addChild($$, $1); addChild($$, $3); }
|   expression GT expression
    { $$ = createNode("GreaterThan"); addChild($$, $1); addChild($$, $3); }
|   expression LE expression
    { $$ = createNode("LessThanOrEqual"); addChild($$, $1); addChild($$, $3); }
|   expression GE expression
    { $$ = createNode("GreaterThanOrEqual"); addChild($$, $1); addChild($$, $3); }
;


primary_expression:
    ID
    { $$ = createNode("Identifier"); addChild($$, createNode($1)); }
|   NUM
    { $$ = createNode("Number"); addChild($$, createNode($1)); }
|   LPAREN expression RPAREN
    { $$ = $2; } // Parentheses just group, don't create a node
|   ID LPAREN RPAREN // Simple function call with no arguments
    {
        $$ = createNode("FunctionCall");
        addChild($$, createNode($1));
        addChild($$, createNode("EmptyArgumentList"));
    }
|   ID LPAREN expression_list RPAREN // Simple function call with arguments
    {
        $$ = createNode("FunctionCall");
        addChild($$, createNode($1));
        addChild($$, $3); // Argument list (expression_list is below)
    }
;

// A simple list of expressions for function arguments or comma expressions
expression_list:
    expression
    { $$ = createNode("ArgumentList"); addChild($$, $1); }
|   expression_list COMMA expression
    { $$ = $1; addChild($$, $3); }
;

%%
void yyerror(ASTNode **root, const char *s) {
    fprintf(stderr, "Parser error: %s at line %d\n", s, yylineno);
}