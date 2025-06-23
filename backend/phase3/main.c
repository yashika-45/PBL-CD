// #include <stdio.h>
// #include <stdlib.h>
// #include "ast.h"
// #include "apt.h"
// #include "symbol_table.h"
// #include "semantic.h"

// // Declare yyparse and yyin from parser
// extern FILE* yyin;
// extern int yyparse(ASTNode **root);

// // Declare the error_file globally (defined in semantic.c)
// extern FILE* error_file;

// int main(int argc, char **argv) {
//     if (argc < 2) {
//         fprintf(stderr, "Usage: %s <input_file.c>\n", argv[0]);
//         return 1;
//     }

//     FILE *fp = fopen(argv[1], "r");
//     if (!fp) {
//         perror("❌ Error opening input file");
//         return 1;
//     }

//     yyin = fp;
//     ASTNode *ast_root = NULL;

//     // Open error.txt
//     error_file = fopen("error.txt", "w");
//     if (!error_file) {
//         perror("❌ Failed to open error.txt");
//         fclose(fp);
//         return 1;
//     }

//     // Phase 1: Parsing
//     if (yyparse(&ast_root) == 0) {
//         printf("✅ Parsing completed successfully.\n");

//         // Phase 2: Print AST
//         printf("\n🧩 Abstract Syntax Tree (AST):\n");
//         printAST(ast_root, 0);

//         // Phase 3: Semantic Analysis + APT
//         printf("\n🧠 Performing Semantic Analysis...\n");
//         SymbolTable *globalScope = createSymbolTable(NULL);
//         APTNode *apt_root = semanticCheck(ast_root, globalScope);

//         if (!apt_root) {
//             fprintf(stderr, "❌ APT Generation failed. semanticCheck returned NULL.\n");
//             fclose(fp);
//             fclose(error_file);
//             return 1;
//         }

//         // Output APT
//         printf("\n📘 Annotated Parse Tree (APT):\n");
//         printAPT(apt_root, 0);

//         // Save APT as text
//         FILE *apt_txt = fopen("apt.txt", "w");
//         if (apt_txt) {
//             printAPTToFile(apt_root, apt_txt, 0);
//             fclose(apt_txt);
//         } else {
//             fprintf(stderr, "❌ Could not write to apt.txt\n");
//         }

//         // Save APT as JSON
//         FILE *apt_json = fopen("apt.json", "w");
//         if (apt_json) {
//             printAPTJSON(apt_root, apt_json, 0);
//             fclose(apt_json);
//         } else {
//             fprintf(stderr, "❌ Could not write to apt.json\n");
//         }

//         // Save Symbol Table
//         FILE *sym_file = fopen("symbol_table.txt", "w");
//         if (sym_file) {
//             printSymbolTable(globalScope, sym_file);
//             fclose(sym_file);
//             printf("\n📄 Symbol table written to symbol_table.txt\n");
//         } else {
//             fprintf(stderr, "❌ Failed to write symbol_table.txt\n");
//         }

//     } else {
//         printf("❌ Parsing failed due to syntax errors.\n");
//     }

//     fclose(fp);
//     fclose(error_file);
//     return 0;
// }



#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "apt.h"
#include "symbol_table.h"
#include "semantic.h"

// Declare yyparse and yyin from parser
extern FILE* yyin;
extern int yyparse(ASTNode **root);

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file.c>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("❌ Error opening input file");
        return 1;
    }

    yyin = fp;
    ASTNode *ast_root = NULL;

    // Open error.txt
    error_file = fopen("error.txt", "w");
    if (!error_file) {
        perror("❌ Failed to open error.txt");
        fclose(fp);
        return 1;
    }

    // Phase 1: Parsing
    if (yyparse(&ast_root) == 0) {
        printf("✅ Parsing completed successfully.\n");

        // Phase 2: Print AST
        printf("\n🧩 Abstract Syntax Tree (AST):\n");
        printAST(ast_root, 0);

        // Phase 3: Semantic Analysis + APT
        printf("\n🧠 Performing Semantic Analysis...\n");
        SymbolTable *globalScope = createSymbolTable(NULL);
        APTNode *apt_root = semanticCheck(ast_root, globalScope);

        if (!apt_root) {
            fprintf(stderr, "❌ APT Generation failed. semanticCheck returned NULL.\n");
            fclose(fp);
            fclose(error_file);
            return 1;
        }

        // Output APT
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

        // Save ALL Symbol Tables
        FILE *sym_file = fopen("symbol_table.txt", "w");
        if (sym_file) {
            for (int i = 0; i < scope_count; ++i) {
                printSymbolTable(all_scopes[i], sym_file);
            }
            fclose(sym_file);
            printf("\n📄 Symbol table written to symbol_table.txt\n");
        } else {
            fprintf(stderr, "❌ Failed to write symbol_table.txt\n");
        }

        // Save symbol table as JSON
FILE *sym_json = fopen("symbol_table.json", "w");
if (sym_json) {
    printSymbolTableJSON(globalScope, sym_json);
    fclose(sym_json);
    printf("📄 Symbol table written to symbol_table.json\n");
} else {
    fprintf(stderr, "❌ Failed to write symbol_table.json\n");
}


    } else {
        printf("❌ Parsing failed due to syntax errors.\n");
    }

    fclose(fp);
    fclose(error_file);
    return 0;
}
