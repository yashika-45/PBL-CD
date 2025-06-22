// #include <stdio.h>
// #include <stdlib.h>
// #include "ast.h"
// #include "apt.h"
// #include "semantic.h"  // contains semanticCheck + SymbolTable functions

// // Declare yyparse and yyin from parser
// extern FILE* yyin;
// extern int yyparse(ASTNode **root);

// int main(int argc, char **argv) {
//     if (argc < 2) {
//         fprintf(stderr, "Usage: %s <input_file.c>\n", argv[0]);
//         return 1;
//     }

//     FILE *fp = fopen(argv[1], "r");
//     if (!fp) {
//         perror("Error opening input file");
//         return 1;
//     }

//     yyin = fp;
//     ASTNode *ast_root = NULL;

//     if (yyparse(&ast_root) == 0) {
//         printf("Γ£à Parsing successful.\n\n");

//         printf("≡ƒºá Abstract Syntax Tree (AST):\n");
//         printAST(ast_root, 0);

//         printf("\n≡ƒöì Semantic Analysis and APT Generation...\n");
//         SymbolTable *globalScope = createSymbolTable(NULL);
//         APTNode *apt_root = semanticCheck(ast_root, globalScope);

//         if (!apt_root) {
// printf("❌ APT Generation failed. semanticCheck returned NULL.\n");
// return 1;
// }
//         printf("\n≡ƒìä Annotated Parse Tree (APT):\n");
//         printAPT(apt_root, 0);
//     } else {
//         printf("❌ Parsing failed.\n");
//     }

//     fclose(fp);
//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "apt.h"
#include "semantic.h"  // contains semanticCheck + SymbolTable functions

// Declare yyparse and yyin from parser
extern FILE* yyin;
extern int yyparse(ASTNode **root);

// Declare the error_file globally (defined in semantic.c)
extern FILE* error_file;

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file.c>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Error opening input file");
        return 1;
    }

    yyin = fp;
    ASTNode *ast_root = NULL;

    // Open error.txt
    error_file = fopen("error.txt", "w");
    if (!error_file) {
        perror("Failed to open error.txt");
        fclose(fp);
        return 1;
    }

    if (yyparse(&ast_root) == 0) {
        printf("✅ Parsing successful.\n\n");

        printf("🧩 Abstract Syntax Tree (AST):\n");
        printAST(ast_root, 0);

        printf("\n🧠 Semantic Analysis and APT Generation...\n");
        SymbolTable *globalScope = createSymbolTable(NULL);
        APTNode *apt_root = semanticCheck(ast_root, globalScope);

        if (!apt_root) {
            printf("❌ APT Generation failed. semanticCheck returned NULL.\n");
            fclose(fp);
            fclose(error_file);
            return 1;
        }

        printf("\n📘 Annotated Parse Tree (APT):\n");
        printAPT(apt_root, 0);

        // Save APT as text
        FILE *apt_txt = fopen("apt.txt", "w");
        if (apt_txt) {
            printAPTToFile(apt_root, apt_txt, 0);
            fclose(apt_txt);
        } else {
            fprintf(stderr, "❌ Could not write to apt.txt\n");
        }

        // Save APT as JSON
        FILE *apt_json = fopen("apt.json", "w");
        if (apt_json) {
            printAPTJSON(apt_root, apt_json, 0);
            fclose(apt_json);
        } else {
            fprintf(stderr, "❌ Could not write to apt.json\n");
        }

    } else {
        printf("❌ Parsing failed.\n");
    }

    fclose(fp);
    fclose(error_file);
    return 0;
}
