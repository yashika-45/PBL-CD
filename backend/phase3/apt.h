// apt.h
#ifndef APT_H
#define APT_H
#include<stdio.h>

typedef struct APTNode {
    char *label;
    char *type;
    int is_function;
    int param_count;
    int child_count;
    int capacity;
    struct APTNode **children;
} APTNode;

APTNode* createAPTNode(const char *label);
void addAPTChild(APTNode *parent, APTNode *child);
void printAPT(APTNode *node, int level);
void printAPTToFile(APTNode *node, FILE *out, int depth);
void printAPTJSON(APTNode *node, FILE *out, int indent);


#endif
