#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode* create_node(NodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->type = type;
    node->operator = NULL;
    node->data.compound.children = NULL;
    node->data.compound.num_children = 0;
    return node;
}

void add_child(ASTNode* parent, ASTNode* child) {
    if (!child) return;
    parent->data.compound.num_children++;
    parent->data.compound.children = (ASTNode**)realloc(
        parent->data.compound.children,
        parent->data.compound.num_children * sizeof(ASTNode*)
    );
    if (!parent->data.compound.children) {
        fprintf(stderr, "Memory reallocation failed\n");
        exit(1);
    }
    parent->data.compound.children[parent->data.compound.num_children - 1] = child;
}

int is_valid_integer(const char* value, int base) {
    switch (base) {
        case 2:  // Binary
            for (int i = 0; value[i]; i++)
                if (value[i] != '0' && value[i] != '1')
                    return 0;
            return 1;
        case 8:  // Octal
            for (int i = 0; value[i]; i++)
                if (value[i] < '0' || value[i] > '7')
                    return 0;
            return 1;
        case 10: // Decimal
            for (int i = 0; value[i]; i++)
                if (value[i] < '0' || value[i] > '9')
                    return 0;
            return 1;
        default:
            return 0; // Invalid base
    }
}

void print_ast(ASTNode* node, int depth) {
    if (!node) return;

    // Print opening parenthesis
    if(node->data.compound.num_children != 0)
        printf("(");

    // Print node content based on type
    switch (node->type) {
        case NODE_PROGRAM:
        case NODE_BLOCK:
            // Skip printing label for program and block
            break;
        case NODE_VARDECL: {
            ASTNode* id = node->data.compound.children[0];
            ASTNode* type = node->data.compound.children[1];
            printf("%s %s", id->data.string_value, type->data.string_value);
            break;
        }
        case NODE_ASSIGN:
        case NODE_BINARY_OP:
        case NODE_RELOP:
            if (node->operator) printf("%s", node->operator);
            break;
        case NODE_IF:
            printf("if");
            break;
        case NODE_WHILE:
            printf("while");
            break;
        case NODE_FOR:
            printf("for");
            break;
        case NODE_PRINT:
            printf("print");
            break;
        case NODE_SCAN:
            printf("scan");
            break;
        case NODE_INT_CONST:
            printf("(%d %d)", node->data.int_const.int_value, node->data.int_const.base);
            break;
        case NODE_CHAR_CONST:
            printf("'%c'", node->data.char_value);
            break;
        case NODE_STRING_CONST:
            printf("\"%s\"", node->data.string_value);
            break;
        case NODE_IDENTIFIER:
            printf("%s", node->data.string_value);
            break;
        case NODE_COMMENT:
            // Skip comments
            return;
        case NODE_TYPE:
            // Handled in VARDECL
            break;
    }

    // Print children, except for VARDECL
    if (node->type != NODE_VARDECL) {
        for (int i = 0; i < node->data.compound.num_children; i++) {
            ASTNode* child = node->data.compound.children[i];
            if (child && child->type != NODE_COMMENT) {
                printf(" ");
                print_ast(child, depth + 1);
            }
        }
    }

    // Print closing parenthesis
    if(node->data.compound.num_children != 0)
        printf(")");
}

void free_ast(ASTNode* node) {
    if (!node) return;
    if (node->operator) free(node->operator);
    if (node->type == NODE_IDENTIFIER || node->type == NODE_STRING_CONST || node->type == NODE_TYPE) {
        if (node->data.string_value) free(node->data.string_value);
    }
    for (int i = 0; i < node->data.compound.num_children; i++) {
        free_ast(node->data.compound.children[i]);
    }
    if (node->data.compound.children) {
        free(node->data.compound.children);
    }
    free(node);
}
