#include<stdio.h>
#ifndef AST_H
#define AST_H

#define MAX_CHILDREN 10

// Define the structure of an AST node
typedef struct ASTNode {
    char *label;                        // Label or token for the node
    struct ASTNode *children[MAX_CHILDREN]; // Array of child pointers
    int child_count;                   // Number of children
} ASTNode;

// Function declarations
ASTNode* createNode(const char *label);
void addChild(ASTNode *parent, ASTNode *child);
void printAST(ASTNode *node, int level);
void printASTToFile(struct ASTNode *node, int level, FILE *file);
void printASTToJSON(struct ASTNode *node, FILE *file); // <--- Add this declaration


#endif // AST_H