#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SCOPES 100

SymbolTable* all_scopes[MAX_SCOPES];
int scope_count = 0;

// --------- Internal hash function ---------
unsigned int hash(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str++;
    }
    return hash % TABLE_SIZE;
}

// --------- Create a new symbol table ---------
SymbolTable *createSymbolTable(SymbolTable *parent) {
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (!table) return NULL;

    for (int i = 0; i < TABLE_SIZE; ++i) {
        table->buckets[i] = NULL;
    }
    table->scope_level = parent ? parent->scope_level + 1 : 0;
    table->parent = parent;
    if (scope_count < MAX_SCOPES) {
        all_scopes[scope_count++] = table;
    }
    return table;
}

// --------- Insert a symbol ---------
bool insertSymbol(SymbolTable *table, const char *name, const char *type, SymbolKind kind, int line) {
    unsigned int index = hash(name);
    Symbol *existing = table->buckets[index];

    // Check for redeclaration in the same scope
    while (existing) {
        if (strcmp(existing->name, name) == 0) {
            return false; // Redeclaration
        }
        existing = existing->next;
    }

    Symbol *sym = (Symbol *)malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->type = strdup(type);
    sym->kind = kind;
    sym->line_declared = line;
    sym->scope_level = table->scope_level;
    sym->next = table->buckets[index];
    table->buckets[index] = sym;

    return true;
}

// --------- Lookup a symbol recursively ---------
Symbol *lookupSymbol(SymbolTable *table, const char *name) {
    while (table) {
        unsigned int index = hash(name);
        Symbol *sym = table->buckets[index];
        while (sym) {
            if (strcmp(sym->name, name) == 0) {
                return sym;
            }
            sym = sym->next;
        }
        table = table->parent;
    }
    return NULL;
}

// --------- Print a single symbol ---------
const char *kindToString(SymbolKind kind) {
    switch (kind) {
        case SYMBOL_VAR: return "Variable";
        case SYMBOL_FUNC: return "Function";
        case SYMBOL_PARAM: return "Parameter";
        default: return "Unknown";
    }
}

void printSymbolTable(SymbolTable *table, FILE *out) {
    if (!table) return;

    fprintf(out, "===== Symbol Table (Scope Level %d) =====\n", table->scope_level);

    for (int i = 0; i < TABLE_SIZE; ++i) {
        Symbol *sym = table->buckets[i];
        while (sym) {
            const char *kindStr = sym->kind == SYMBOL_VAR ? "Variable" :
                                  sym->kind == SYMBOL_FUNC ? "Function" : "Parameter";
            fprintf(out, "Name: %-10s | Type: %-10s | Kind: %-9s | Line: %d\n",
                    sym->name, sym->type, kindStr, sym->line_declared);
            sym = sym->next;
        }
    }
}


void printSymbolTableJSON(SymbolTable *table, FILE *out) {
    if (!table || !out) return;

    Symbol *symbols[1000];  // Flat array to store all symbols
    int count = 0;

    // Collect all symbols from this table and child scopes
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Symbol *sym = table->buckets[i];
        while (sym) {
            symbols[count++] = sym;
            sym = sym->next;
        }
    }

    fprintf(out, "[\n");
    for (int i = 0; i < count; ++i) {
        Symbol *sym = symbols[i];
        fprintf(out,
            "  {\n"
            "    \"name\": \"%s\",\n"
            "    \"type\": \"%s\",\n"
            "    \"kind\": \"%s\",\n"
            "    \"line\": %d,\n"
            "    \"scope\": %d\n"
            "  }%s\n",
            sym->name,
            sym->type,
            sym->kind == SYMBOL_FUNC ? "Function" :
            sym->kind == SYMBOL_VAR ? "Variable" : "Parameter",
            sym->line_declared,
            sym->scope_level,
            (i == count - 1) ? "" : ","
        );
    }
    fprintf(out, "]\n");
}
