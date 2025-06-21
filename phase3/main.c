// main.c
#include <stdio.h>
#include "ast.h"
#include "apt.h"

// Declare parser function from parser.tab.c
int yyparse(ASTNode **root);

extern FILE *yyin;

APTNode* semanticCheck(ASTNode *node, void *table);  // Forward declaration

int main(int argc, char *argv[]) {
    // Input file
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file.c>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Failed to open input file");
        return 1;
    }

    yyin = fp;

    ASTNode *astRoot = NULL;

    if (yyparse(&astRoot) == 0) {
        printf("✅ Parsing successful.\n\n");

        printf("🧠 Abstract Syntax Tree (AST):\n");
        printAST(astRoot, 0);

        printf("\n🔍 Semantic Analysis and APT Generation...\n");
        APTNode *aptRoot = semanticCheck(astRoot, NULL);

        printf("\n🌳 Annotated Parse Tree (APT):\n");
        printAPT(aptRoot, 0);
    } else {
        printf("❌ Parsing failed.\n");
    }

    fclose(fp);
    return 0;
}