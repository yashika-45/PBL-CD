#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode* createNode(const char *label) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->label = strdup(label);
    node->child_count = 0;
    return node;
}

void addChild(ASTNode *parent, ASTNode *child) {
    if (parent->child_count < 10) {
        parent->children[parent->child_count++] = child;
    }
}

void printAST(ASTNode *node, int level) {
    if (!node) return;
    for (int i = 0; i < level; i++) printf("  ");
    printf("%s\n", node->label);
    for (int i = 0; i < node->child_count; i++) {
        printAST(node->children[i], level + 1);
    }
}
