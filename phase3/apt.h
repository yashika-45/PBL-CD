// apt.h
#ifndef APT_H
#define APT_H

#define MAX_CHILDREN 10

typedef struct APTNode {
    char *label;
    char *type;              // Annotation: data type like "int", "float", etc.
    int is_function;         // 1 if it's a function
    int param_count;         // number of parameters if function
    struct APTNode *children[MAX_CHILDREN];
    int child_count;
} APTNode;

APTNode* createAPTNode(const char *label);
void addAPTChild(APTNode *parent, APTNode *child);
void printAPT(APTNode *node, int level);

#endif