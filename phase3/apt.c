// apt.c
#include "apt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

APTNode* createAPTNode(const char *label) {
    APTNode *node = (APTNode *)malloc(sizeof(APTNode));
    node->label = strdup(label);
    node->type = NULL;
    node->is_function = 0;
    node->param_count = 0;
    node->child_count = 0;
    return node;
}

void addAPTChild(APTNode *parent, APTNode *child) {
    if (parent->child_count < MAX_CHILDREN) {
        parent->children[parent->child_count++] = child;
    }
}

void printAPT(APTNode *node, int level) {
    if (!node) return;
    for (int i = 0; i < level; ++i) printf("  ");
    printf("%s", node->label);
    if (node->type) printf(" [type: %s]", node->type);
    if (node->is_function) printf(" [function, params: %d]", node->param_count);
    printf("\n");
    for (int i = 0; i < node->child_count; ++i)
        printAPT(node->children[i], level + 1);
}