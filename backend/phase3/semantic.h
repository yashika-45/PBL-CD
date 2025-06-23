#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "apt.h"
#include "symbol_table.h"  // ✅ Use existing symbol table structs

extern FILE *error_file;

APTNode* semanticCheck(ASTNode *node, SymbolTable *table);

#endif
