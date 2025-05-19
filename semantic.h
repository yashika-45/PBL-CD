#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

typedef struct Symbol {
    char name[100];
    char type[50];
    struct Symbol *next;
} Symbol;

typedef struct {
    Symbol *head;
} SymbolTable;

void initSymbolTable(SymbolTable *table);
int addSymbol(SymbolTable *table, const char *name, const char *type);
int findSymbol(SymbolTable *table, const char *name);
void analyzeSemantics(ASTNode *node, SymbolTable *table);
void freeSymbolTable(SymbolTable *table);

#endif