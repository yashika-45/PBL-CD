#include "ast.h"
#include <ctype.h>
#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initSymbolTable(SymbolTable *table) {
    table->head = NULL;
}

int addSymbol(SymbolTable *table, const char *name, const char *type) {
    if (findSymbol(table, name)) {
        printf("Semantic Error: Redeclaration of '%s'\n", name);
        return 0;
    }
    Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));
    strcpy(symbol->name, name);
    strcpy(symbol->type, type);
    symbol->next = table->head;
    table->head = symbol;
    return 1;
}

int findSymbol(SymbolTable *table, const char *name) {
    Symbol *curr = table->head;
    while (curr) {
        if (strcmp(curr->name, name) == 0) return 1;
        curr = curr->next;
    }
    return 0;
}

void analyzeSemantics(ASTNode *node, SymbolTable *table) {
    if (!node) return;

    if (strcmp(node->label, "Declaration") == 0) {
        const char *type = node->children[0]->label;
        const char *id = node->children[1]->label;
        addSymbol(table, id, type);
    } else if (strcmp(node->label, "DeclarationAssignment") == 0) {
        const char *type = node->children[0]->label;
        const char *id = node->children[1]->label;
        if (addSymbol(table, id, type)) {
            analyzeSemantics(node->children[2], table); // Analyze expression
        }
    } else if (strcmp(node->label, "ArrayDeclaration") == 0) {
        const char *type = node->children[0]->label;
        const char *id = node->children[1]->label;
        addSymbol(table, id, type);
    } else if (strcmp(node->label, "Assignment") == 0) {
        const char *id = node->children[0]->label;
        if (!findSymbol(table, id)) {
            printf("Semantic Error: Variable '%s' used before declaration\n", id);
        }
        analyzeSemantics(node->children[1], table); // expression
    } else if (strcmp(node->label, "FunctionDeclaration") == 0) {
        const char *retType = node->children[0]->label;
        const char *funcName = node->children[1]->label;

        addSymbol(table, funcName, retType); // You may extend with function signatures

        // Process parameters
        ASTNode *params = node->children[2];
        for (int i = 0; i < params->child_count; i += 2) {
            const char *ptype = params->children[i]->label;
            const char *pname = params->children[i + 1]->label;
            addSymbol(table, pname, ptype);
        }

        analyzeSemantics(node->children[3], table); // body
    } else if (strcmp(node->label, "FunctionCall") == 0) {
        const char *fname = node->children[0]->label;
        if (!findSymbol(table, fname)) {
            printf("Semantic Error: Function '%s' not declared\n", fname);
        }
        analyzeSemantics(node->children[1], table); // arguments
    } else if (strcmp(node->label, "IfElse") == 0 || strcmp(node->label, "While") == 0) {
        analyzeSemantics(node->children[0], table); // condition
        analyzeSemantics(node->children[1], table); // if/while body
        if (node->child_count > 2) analyzeSemantics(node->children[2], table); // else body
    } else if (strcmp(node->label, "Return") == 0) {
        analyzeSemantics(node->children[0], table);
    } else if (strcmp(node->label, "+") == 0 || strcmp(node->label, "-") == 0 ||
               strcmp(node->label, "*") == 0 || strcmp(node->label, "/") == 0) {
        analyzeSemantics(node->children[0], table);
        analyzeSemantics(node->children[1], table);
    } else if (strcmp(node->label, "Block") == 0 || strcmp(node->label, "Statements") == 0) {
        for (int i = 0; i < node->child_count; ++i) {
            analyzeSemantics(node->children[i], table);
        }
    } else if (node->label[0] >= 'a' && node->label[0] <= 'z') {
        // Possibly a variable or literal
        if (!findSymbol(table, node->label) && !isdigit(node->label[0])) {
            printf("Semantic Error: Variable '%s' used before declaration\n", node->label);
        }
    }

    // Recursively check other children (for any unprocessed)
    for (int i = 0; i < node->child_count; ++i) {
        analyzeSemantics(node->children[i], table);
    }
}

void freeSymbolTable(SymbolTable *table) {
    Symbol *curr = table->head;
    while (curr) {
        Symbol *temp = curr;
        curr = curr->next;
        free(temp);
    }
}