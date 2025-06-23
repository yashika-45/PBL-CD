#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"
#include "apt.h"
#include "symbol_table.h"
#include "ast.h"


// Global error file pointer
FILE *error_file = NULL;

// ---------------- Semantic Analysis + APT Generation ----------------

APTNode* semanticCheck(ASTNode *node, SymbolTable *table) {
    if (!node) return NULL;

    APTNode *aptNode = createAPTNode(node->label);
    if (node && node->label)
        printf("Adding APT child node: %s\n", node->label);
    else
        printf("Adding APT child node: (null)\n");

    if (strcmp(node->label, "VariableDeclaration") == 0) {
        const char *type = node->children[0]->children[0]->label;
        char *name = node->children[1]->label;

        if (!insertSymbol(table, name, type, SYMBOL_VAR, node->line)) {
            fprintf(error_file, "Semantic Error: Redeclaration of variable '%s'\n", name);
        }
        aptNode->type = strdup(type);
    }
    else if (strcmp(node->label, "Assignment") == 0) {
        char *name = node->children[0]->label;
        Symbol *sym = lookupSymbol(table, name);

        if (!sym) {
            fprintf(error_file, "Semantic Error: Variable '%s' used before declaration\n", name);
        } else if (sym->kind != SYMBOL_VAR && sym->kind != SYMBOL_PARAM) {
            fprintf(error_file, "Semantic Error: '%s' is not a variable\n", name);
        }
        aptNode->type = sym ? strdup(sym->type) : NULL;
    }
    else if (strcmp(node->label, "FunctionDefinition") == 0) {
        const char *type = node->children[0]->children[0]->label;
        const char *name = node->children[1]->label;
        ASTNode *params = node->children[2];

        if (!insertSymbol(table, name, type, SYMBOL_FUNC, node->line)) {
            fprintf(error_file, "Semantic Error: Redeclaration of function '%s'\n", name);
        }

        SymbolTable *funcScope = createSymbolTable(table);

        // Insert parameters into function scope
        for (int i = 0; i < params->child_count; ++i) {
            ASTNode *param = params->children[i];
            if (strcmp(param->label, "Parameter") == 0) {
                const char *ptype = param->children[0]->children[0]->label;
                const char *pname = param->children[1]->label;
                insertSymbol(funcScope, pname, ptype, SYMBOL_PARAM, param->line);
            }
        }

        APTNode *bodyAPT = semanticCheck(node->children[3], funcScope);
        if (bodyAPT) addAPTChild(aptNode, bodyAPT);
        return aptNode;
    }
    else if (strcmp(node->label, "FunctionCall") == 0) {
        char *name = node->children[0]->label;
        Symbol *func = lookupSymbol(table, name);

        if (!func || func->kind != SYMBOL_FUNC) {
            fprintf(error_file, "Semantic Error: Function '%s' not declared or not a function\n", name);
        }

        aptNode->is_function = 1;
        aptNode->param_count = node->children[1]->child_count;  // argument count
    }

    // Recurse on children
    for (int i = 0; i < node->child_count; ++i) {
        if (!node->children[i]) continue;
        APTNode *childAPT = semanticCheck(node->children[i], table);
        if (childAPT) addAPTChild(aptNode, childAPT);
    }

    return aptNode;
}
