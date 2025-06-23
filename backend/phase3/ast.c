#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
extern int yylineno;
ASTNode* createNode(const char *label) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->label = strdup(label);
    node->child_count = 0;
    node->line = yylineno;
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

void printASTToFile(ASTNode *node, int level, FILE *file) {
    if (!node) return;
    for (int i = 0; i < level; i++) fprintf(file, "  ");
    fprintf(file, "%s\n", node->label);
    for (int i = 0; i < node->child_count; i++) {
        printASTToFile(node->children[i], level + 1, file);
    }
}

// ✅ Escaping function
char* escapeJSONString(const char* str) {
    static char buffer[1024]; // Adjust size if needed
    char *p = buffer;
    while (*str) {
        switch (*str) {
            case '\"': *p++ = '\\'; *p++ = '\"'; break;
            case '\\': *p++ = '\\'; *p++ = '\\'; break;
            case '\n': *p++ = '\\'; *p++ = 'n'; break;
            case '\r': *p++ = '\\'; *p++ = 'r'; break;
            case '\t': *p++ = '\\'; *p++ = 't'; break;
            default: *p++ = *str;
        }
        str++;
    }
    *p = '\0';
    return buffer;
}

// void printASTToJSON(ASTNode *node, FILE *file) {
//     fprintf(stderr, "💡 Entered printASTToJSON()\n");
//     if (!node) {
//         fprintf(file, "null");
//         return;
//     }

//     fprintf(file, "{");
//     fprintf(file, "\"label\": \"%s\"", escapeJSONString(node->label));

//     if (node->child_count > 0) {
//         fprintf(file, ", \"children\": [");
//         for (int i = 0; i < node->child_count; ++i) {
//             printASTToJSON(node->children[i], file);
//             if (i != node->child_count - 1)
//                 fprintf(file, ", ");
//         }
//         fprintf(file, "]");
//     }

//     fprintf(file, "}");
// }


void printASTToJSON(ASTNode *node, FILE *file) {
    if (!node) {
        fprintf(stderr, "⚠️ NULL node encountered\n");
        fprintf(file, "null");
        return;
    }

    fprintf(stderr, "🧠 Visiting node: %s (%d children)\n", node->label ? node->label : "NULL", node->child_count);

    fprintf(file, "{");
    fprintf(file, "\"label\": \"%s\"", escapeJSONString(node->label ? node->label : "NULL"));

    if (node->child_count > 0) {
        fprintf(file, ", \"children\": [");
        for (int i = 0; i < node->child_count; ++i) {
            printASTToJSON(node->children[i], file);
            if (i != node->child_count - 1)
                fprintf(file, ", ");
        }
        fprintf(file, "]");
    }

    fprintf(file, "}");
}

