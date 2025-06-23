#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>
#include <stdio.h>  // For FILE

typedef enum {
    SYMBOL_VAR,
    SYMBOL_FUNC,
    SYMBOL_PARAM
} SymbolKind;

// Forward declaration before using it in extern or struct
typedef struct SymbolTable SymbolTable;

typedef struct Symbol {
    char *name;
    char *type;
    SymbolKind kind;
    int line_declared;
    int scope_level;
    struct Symbol *next;
} Symbol;

#define TABLE_SIZE 256
#define MAX_SCOPES 100

// Symbol Table definition
struct SymbolTable {
    Symbol *buckets[TABLE_SIZE];
    int scope_level;
    SymbolTable *parent;
};

// Declare these only once (used in main.c/semantic.c)
extern SymbolTable* all_scopes[MAX_SCOPES];
extern int scope_count;

// Function declarations
SymbolTable *createSymbolTable(SymbolTable *parent);
bool insertSymbol(SymbolTable *table, const char *name, const char *type, SymbolKind kind, int line);
Symbol *lookupSymbol(SymbolTable *table, const char *name);
void printSymbolTable(SymbolTable *table, FILE *out);
void printSymbolTableJSON(SymbolTable *table, FILE *out);

#endif
