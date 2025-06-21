// semantic.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apt.h"
#include "apt.h"

typedef struct Symbol {
    char *name;
    char *type;
    int is_function;
    int param_count;
    struct Symbol *next;
} Symbol;

typedef struct SymbolTable {
    struct Symbol *symbols;
    struct SymbolTable *parent;
} SymbolTable;

SymbolTable* createSymbolTable(SymbolTable *parent) {
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
    table->symbols = NULL;
    table->parent = parent;
    return table;
}

Symbol* findSymbol(SymbolTable *table, const char *name) {
    while (table) {
        Symbol *sym = table->symbols;
        while (sym) {
            if (strcmp(sym->name, name) == 0) return sym;
            sym = sym->next;
        }
        table = table->parent;
    }
    return NULL;
}

int addSymbol(SymbolTable *table, const char *name, const char *type, int is_function, int param_count) {
    if (findSymbol(table, name)) return 0;
    Symbol *sym = (Symbol *)malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->type = strdup(type);
    sym->is_function = is_function;
    sym->param_count = param_count;
    sym->next = table->symbols;
    table->symbols = sym;
    return 1;
}

APTNode* semanticCheck(struct APTNode *node, SymbolTable *table) {
    if (!node) return NULL;

    APTNode *aptNode = createAPTNode(node->label);

    if (strcmp(node->label, "Declaration") == 0 || strcmp(node->label, "DeclarationAssignment") == 0) {
        char *type = node->children[0]->label;
        char *name = node->children[1]->label;
        if (!addSymbol(table, name, type, 0, 0)) {
            printf("Semantic Error: Redeclaration of variable '%s'\n", name);
        }
        aptNode->type = strdup(type);
    }
    else if (strcmp(node->label, "ArrayDeclaration") == 0) {
        char *type = node->children[0]->label;
        char *name = node->children[1]->label;
        if (!addSymbol(table, name, type, 0, 0)) {
            printf("Semantic Error: Redeclaration of array '%s'\n", name);
        }
        aptNode->type = strdup(type);
    }
    else if (strcmp(node->label, "Assignment") == 0) {
        char *name = node->children[0]->label;
        Symbol *sym = findSymbol(table, name);
        if (!sym) {
            printf("Semantic Error: Variable '%s' used before declaration\n", name);
        } else {
            aptNode->type = strdup(sym->type);
        }
    }
    else if (strcmp(node->label, "FunctionDeclaration") == 0) {
        char *type = node->children[0]->label;
        char *name = node->children[1]->label;
        APTNode *params = node->children[2];
        int param_count = params->child_count / 2;
        if (!addSymbol(table, name, type, 1, param_count)) {
            printf("Semantic Error: Redeclaration of function '%s'\n", name);
        }
        aptNode->type = strdup(type);
        aptNode->is_function = 1;
        aptNode->param_count = param_count;

        SymbolTable *funcScope = createSymbolTable(table);
        for (int i = 0; i < params->child_count; i += 2) {
            char *ptype = params->children[i]->label;
            char *pname = params->children[i + 1]->label;
            addSymbol(funcScope, pname, ptype, 0, 0);
        }

        addAPTChild(aptNode, semanticCheck(node->children[3], funcScope));
        return aptNode;
    }
    else if (strcmp(node->label, "FunctionCall") == 0) {
        char *name = node->children[0]->label;
        Symbol *func = findSymbol(table, name);
        int arg_count = node->children[1]->child_count;
        if (!func || !func->is_function) {
            printf("Semantic Error: Function '%s' not declared\n", name);
        } else if (func->param_count != arg_count) {
            printf("Semantic Error: Function '%s' expects %d args, got %d\n", name, func->param_count, arg_count);
        }
        aptNode->is_function = 1;
        aptNode->param_count = arg_count;
    }

    for (int i = 0; i < node->child_count; ++i) {
        APTNode *childAPT = semanticCheck(node->children[i], table);
        if (childAPT) addAPTChild(aptNode, childAPT);
    }

    return aptNode;
}

// Driver
extern int yyparse(struct APTNode **root);

// int main() {
//     APTNode *root = NULL;

//     if (yyparse(&root) == 0) {
//         printf("✅ Parsing completed.\n\n");

//         printf("🧠 Abstract Syntax Tree (AST):\n");
//         printAST(root, 0);

//         SymbolTable *globalTable = createSymbolTable(NULL);
//         printf("\n🔍 Starting semantic analysis + APT generation...\n");
//         APTNode *aptRoot = semanticCheck(root, globalTable);

//         printf("\n🌳 Annotated Parse Tree (APT):\n");
//         printAPT(aptRoot, 0);
//     } else {
//         printf("❌ Parsing failed.\n");
//     }

//     return 0;
// }