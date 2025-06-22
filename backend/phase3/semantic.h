#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "apt.h"

// ---------------- Symbol Table Structs ----------------

typedef struct Symbol {
    char *name;
    char *type;
    int is_function;
    int param_count;
    struct Symbol *next;
} Symbol;

typedef struct SymbolTable {
    Symbol *symbols;
    struct SymbolTable *parent;
} SymbolTable;

// ---------------- Function Declarations ----------------

SymbolTable* createSymbolTable(SymbolTable *parent);
Symbol* findSymbol(SymbolTable *table, const char *name);
int addSymbol(SymbolTable *table, const char *name, const char *type, int is_function, int param_count);

APTNode* semanticCheck(ASTNode *node, SymbolTable *table);

#endif
