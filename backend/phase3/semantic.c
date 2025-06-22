// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include "semantic.h"
// #include "apt.h"

// // ---------------- Symbol Table Functions ----------------

// SymbolTable* createSymbolTable(SymbolTable *parent) {
//     SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
//     table->symbols = NULL;
//     table->parent = parent;
//     return table;
// }

// const char* getTypeFromNode(ASTNode *typeNode) {
//     if (!typeNode || typeNode->child_count == 0 || !typeNode->children[0] || !typeNode->children[0]->label) {
//         return NULL;
//     }
//     return typeNode->children[0]->label;
// }


// Symbol* findSymbol(SymbolTable *table, const char *name) {
//     while (table) {
//         Symbol *sym = table->symbols;
//         while (sym) {
//             if (strcmp(sym->name, name) == 0) return sym;
//             sym = sym->next;
//         }
//         table = table->parent;
//     }
//     return NULL;
// }

// int addSymbol(SymbolTable *table, const char *name, const char *type, int is_function, int param_count) {
//     if (findSymbol(table, name)) return 0;
//     Symbol *sym = (Symbol *)malloc(sizeof(Symbol));
//     sym->name = strdup(name);
//     sym->type = strdup(type);
//     sym->is_function = is_function;
//     sym->param_count = param_count;
//     sym->next = table->symbols;
//     table->symbols = sym;
//     return 1;
// }

// // ---------------- Semantic Analysis + APT Generation ----------------

// APTNode* semanticCheck(ASTNode *node, SymbolTable *table) {
//     if (!node) return NULL;
// APTNode *aptNode = createAPTNode(node->label);
// if (node && node->label)
//     printf("Adding APT child node: %s\n", node->label);
// else
//     printf("Adding APT child node: (null)\n");


// if (strcmp(node->label, "Declaration") == 0 ||
//     strcmp(node->label, "DeclarationAssignment") == 0 ||
//     strcmp(node->label, "VariableDeclaration") == 0 ||
//     strcmp(node->label, "VariableDeclarationWithInit") == 0) {

//     const char *type = getTypeFromNode(node->children[0]);
//     char *name = node->children[1]->label;

//     if (!type || !name) {
//         printf("❌ Semantic Error: Invalid declaration structure\n");
//     } else {
//         if (!addSymbol(table, name, type, 0, 0)) {
//             printf("Semantic Error: Redeclaration of variable '%s'\n", name);
//         }
//         aptNode->type = strdup(type);
//     }
// }
//  else if (strcmp(node->label, "ArrayDeclaration") == 0) {
    
//     char *type = node->children[0]->children[0]->label;
//     char *name = node->children[1]->label;
//     if (!addSymbol(table, name, type, 0, 0)) {
//         printf("Semantic Error: Redeclaration of array '%s'\n", name);
//     }
//     aptNode->type = strdup(type);

// } else if (strcmp(node->label, "Assignment") == 0) {

//     char *name = node->children[0]->label;
//     Symbol *sym = findSymbol(table, name);
//     if (!sym) {
//         printf("Semantic Error: Variable '%s' used before declaration\n", name);
//     } else {
//         aptNode->type = strdup(sym->type);
//     }

// } else if (strcmp(node->label, "FunctionDeclaration") == 0) {

//     char *type = node->children[0]->children[0]->label;
//     char *name = node->children[1]->label;
//     ASTNode *params = node->children[2];
//     int param_count = params->child_count / 2;

//     if (!addSymbol(table, name, type, 1, param_count)) {
//         printf("Semantic Error: Redeclaration of function '%s'\n", name);
//     }

//     aptNode->type = strdup(type);
//     aptNode->is_function = 1;
//     aptNode->param_count = param_count;

//     SymbolTable *funcScope = createSymbolTable(table);
//     for (int i = 0; i < params->child_count; i += 2) {
//         char *ptype = params->children[i]->children[0]->label;
//         char *pname = params->children[i + 1]->label;
//         addSymbol(funcScope, pname, ptype, 0, 0);
//     }

//     APTNode *bodyAPT = semanticCheck(node->children[3], funcScope);
//     if (bodyAPT) addAPTChild(aptNode, bodyAPT);
//     return aptNode;

// } else if (strcmp(node->label, "FunctionCall") == 0) {

//     char *name = node->children[0]->label;
//     Symbol *func = findSymbol(table, name);
//     int arg_count = node->children[1]->child_count;

//     if (!func || !func->is_function) {
//         printf("Semantic Error: Function '%s' not declared\n", name);
//     } else if (func->param_count != arg_count) {
//         printf("Semantic Error: Function '%s' expects %d args, got %d\n", name, func->param_count, arg_count);
//     }

//     aptNode->is_function = 1;
//     aptNode->param_count = arg_count;
// }

// // Base case: leaf node (like identifier or number)
// if (node->child_count == 0) {
//     return aptNode;
// }

// // Recursive case
// for (int i = 0; i < node->child_count; ++i) {
//     if (!node->children[i]) continue;
//     APTNode *childAPT = semanticCheck(node->children[i], table);
//     if (childAPT && childAPT->label) {
//         addAPTChild(aptNode, childAPT);
//     } else {
//         printf("⚠️ Warning: child APT is null for %s\n", node->label);
//     }
// }

// return aptNode;

// }



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"
#include "apt.h"

// Global error file pointer
FILE *error_file = NULL;

// ---------------- Symbol Table Functions ----------------

SymbolTable* createSymbolTable(SymbolTable *parent) {
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
    table->symbols = NULL;
    table->parent = parent;
    return table;
}

const char* getTypeFromNode(ASTNode *typeNode) {
    if (!typeNode || typeNode->child_count == 0 || !typeNode->children[0] || !typeNode->children[0]->label) {
        return NULL;
    }
    return typeNode->children[0]->label;
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

// ---------------- Semantic Analysis + APT Generation ----------------

APTNode* semanticCheck(ASTNode *node, SymbolTable *table) {
    if (!node) return NULL;

    APTNode *aptNode = createAPTNode(node->label);
    if (node && node->label)
        printf("Adding APT child node: %s\n", node->label);
    else
        printf("Adding APT child node: (null)\n");

    if (strcmp(node->label, "Declaration") == 0 ||
        strcmp(node->label, "DeclarationAssignment") == 0 ||
        strcmp(node->label, "VariableDeclaration") == 0 ||
        strcmp(node->label, "VariableDeclarationWithInit") == 0) {

        const char *type = getTypeFromNode(node->children[0]);
        char *name = node->children[1]->label;

        if (!type || !name) {
            if (error_file) fprintf(error_file, "Semantic Error: Invalid declaration structure\n");
            else printf("Semantic Error: Invalid declaration structure\n");
        } else {
            if (!addSymbol(table, name, type, 0, 0)) {
                if (error_file) fprintf(error_file, "Semantic Error: Redeclaration of variable '%s'\n", name);
                else printf("Semantic Error: Redeclaration of variable '%s'\n", name);
            }
            aptNode->type = strdup(type);
        }
    } 
    else if (strcmp(node->label, "ArrayDeclaration") == 0) {
        char *type = node->children[0]->children[0]->label;
        char *name = node->children[1]->label;
        if (!addSymbol(table, name, type, 0, 0)) {
            if (error_file) fprintf(error_file, "Semantic Error: Redeclaration of array '%s'\n", name);
            else printf("Semantic Error: Redeclaration of array '%s'\n", name);
        }
        aptNode->type = strdup(type);
    } 
    else if (strcmp(node->label, "Assignment") == 0) {
        char *name = node->children[0]->label;
        Symbol *sym = findSymbol(table, name);
        if (!sym) {
            if (error_file) fprintf(error_file, "Semantic Error: Variable '%s' used before declaration\n", name);
            else printf("Semantic Error: Variable '%s' used before declaration\n", name);
        } else {
            aptNode->type = strdup(sym->type);
        }
    } 
    else if (strcmp(node->label, "FunctionDeclaration") == 0) {
        char *type = node->children[0]->children[0]->label;
        char *name = node->children[1]->label;
        ASTNode *params = node->children[2];
        int param_count = params->child_count / 2;

        if (!addSymbol(table, name, type, 1, param_count)) {
            if (error_file) fprintf(error_file, "Semantic Error: Redeclaration of function '%s'\n", name);
            else printf("Semantic Error: Redeclaration of function '%s'\n", name);
        }

        aptNode->type = strdup(type);
        aptNode->is_function = 1;
        aptNode->param_count = param_count;

        SymbolTable *funcScope = createSymbolTable(table);
        for (int i = 0; i < params->child_count; i += 2) {
            char *ptype = params->children[i]->children[0]->label;
            char *pname = params->children[i + 1]->label;
            addSymbol(funcScope, pname, ptype, 0, 0);
        }

        APTNode *bodyAPT = semanticCheck(node->children[3], funcScope);
        if (bodyAPT) addAPTChild(aptNode, bodyAPT);
        return aptNode;
    } 
    else if (strcmp(node->label, "FunctionCall") == 0) {
        char *name = node->children[0]->label;
        Symbol *func = findSymbol(table, name);
        int arg_count = node->children[1]->child_count;

        if (!func || !func->is_function) {
            if (error_file) fprintf(error_file, "Semantic Error: Function '%s' not declared\n", name);
            else printf("Semantic Error: Function '%s' not declared\n", name);
        } 
        else if (func->param_count != arg_count) {
            if (error_file) fprintf(error_file, "Semantic Error: Function '%s' expects %d args, got %d\n", name, func->param_count, arg_count);
            else printf("Semantic Error: Function '%s' expects %d args, got %d\n", name, func->param_count, arg_count);
        }

        aptNode->is_function = 1;
        aptNode->param_count = arg_count;
    }

    // Base case: leaf node
    if (node->child_count == 0) return aptNode;

    // Recursive case
    for (int i = 0; i < node->child_count; ++i) {
        if (!node->children[i]) continue;
        APTNode *childAPT = semanticCheck(node->children[i], table);
        if (childAPT && childAPT->label) {
            addAPTChild(aptNode, childAPT);
        } else {
            printf("⚠️ Warning: child APT is null for %s\n", node->label);
        }
    }

    return aptNode;
}
