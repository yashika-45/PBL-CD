// apt.c
#include "apt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

APTNode* createAPTNode(const char *label) {
    if (!label) {
        fprintf(stderr, "⚠️ Warning: createAPTNode received NULL label\n");
        label = "NULL";
    }
    APTNode *node = (APTNode *)malloc(sizeof(APTNode));
    node->label = label ? strdup(label) : strdup("NULL");
    node->type = NULL;
    node->is_function = 0;
    node->param_count = 0;
    node->child_count = 0;
    node->capacity = 4;
    node->children = (APTNode **)malloc(sizeof(APTNode *) * node->capacity);
    return node;
}

void addAPTChild(APTNode *parent, APTNode *child) {
    if (parent->child_count >= parent->capacity) {
        parent->capacity *= 2;
        parent->children = (APTNode **)realloc(parent->children, sizeof(APTNode *) * parent->capacity);
    }
    parent->children[parent->child_count++] = child;
}
// apt.c

void printAPT(APTNode* node, int depth) {
    if (!node) return;

    // Indentation for tree structure
    for (int i = 0; i < depth; ++i) {
        printf("  ");
    }

    // Print node label
    printf("%s", node->label ? node->label : "(null)");

    // Show type annotation
    if (node->type) {
        printf(" [type: %s]", node->type);
    }

    // Show if node is a function
    if (node->is_function) {
        printf(" [function]");
        printf(" [param_count: %d]", node->param_count);
    }

    printf("\n");

    // Recursively print children
    for (int i = 0; i < node->child_count; ++i) {
        printAPT(node->children[i], depth + 1);
    }
}


// Text-based output (tree format)
void printAPTToFile(APTNode *node, FILE *out, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; ++i) {
        fprintf(out, "  ");
    }

    fprintf(out, "%s", node->label ? node->label : "(null)");

    if (node->type) {
        fprintf(out, " [type: %s]", node->type);
    }

    if (node->is_function) {
        fprintf(out, " [function] [param_count: %d]", node->param_count);
    }

    fprintf(out, "\n");

    for (int i = 0; i < node->child_count; ++i) {
        printAPTToFile(node->children[i], out, depth + 1);
    }
}

// JSON format (frontend-friendly)
void printAPTJSON(APTNode *node, FILE *out, int indent) {
    if (!node) return;

    for (int i = 0; i < indent; ++i) fprintf(out, "  ");
    fprintf(out, "{\n");

    // Label
    for (int i = 0; i < indent + 1; ++i) fprintf(out, "  ");
    fprintf(out, "\"label\": \"%s\"", node->label ? node->label : "null");

    // Optional type
    if (node->type) {
        fprintf(out, ",\n");
        for (int i = 0; i < indent + 1; ++i) fprintf(out, "  ");
        fprintf(out, "\"type\": \"%s\"", node->type);
    }

    // Function info
    if (node->is_function) {
        fprintf(out, ",\n");
        for (int i = 0; i < indent + 1; ++i) fprintf(out, "  ");
        fprintf(out, "\"is_function\": true,\n");
        for (int i = 0; i < indent + 1; ++i) fprintf(out, "  ");
        fprintf(out, "\"param_count\": %d", node->param_count);
    }

    // Children
    if (node->child_count > 0) {
        fprintf(out, ",\n");
        for (int i = 0; i < indent + 1; ++i) fprintf(out, "  ");
        fprintf(out, "\"children\": [\n");
        for (int i = 0; i < node->child_count; ++i) {
            printAPTJSON(node->children[i], out, indent + 2);
            if (i < node->child_count - 1) fprintf(out, ",\n");
            else fprintf(out, "\n");
        }
        for (int i = 0; i < indent + 1; ++i) fprintf(out, "  ");
        fprintf(out, "]");
    }

    fprintf(out, "\n");
    for (int i = 0; i < indent; ++i) fprintf(out, "  ");
    fprintf(out, "}");
}
