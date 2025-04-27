#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

// Add a flag to mark nodes as freed to prevent double frees
static int ast_cleanup_in_progress = 0;

// Safe conversion from one number base to another
int convert_base(int value, int from_base) {
    // If base is 10, no conversion needed
    if (from_base == 10) return value;
    
    // Convert to string
    char num_str[64];
    sprintf(num_str, "%d", value);
    
    // Validate digits for the given base
    for (int i = 0; num_str[i] != '\0'; i++) {
        int digit = num_str[i] - '0';
        if ((from_base == 2 && digit > 1) || (from_base == 8 && digit > 7)) {
            printf("Invalid digit %d for base %d\n", digit, from_base);
            return value; // Return original value on error
        }
    }
    
    // Convert from the given base to decimal
    int result = 0;
    for (int i = 0; num_str[i] != '\0'; i++) {
        int digit = num_str[i] - '0';
        result = result * from_base + digit;
    }
    
    return result;
}

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
    node->already_freed = 0;
    return node;
}

void add_child(ASTNode* parent, ASTNode* child) {
    if (!parent || !child) return;
    
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

    // Print indentation
    for (int i = 0; i < depth; i++) printf("  ");

    // Print node content based on type
    switch (node->type) {
        case NODE_PROGRAM:
            printf("PROGRAM\n");
            break;
        case NODE_VARDECL:
            if (node->data.compound.num_children > 0 && 
                node->data.compound.children[0] && 
                node->data.compound.children[0]->type == NODE_IDENTIFIER) {
                printf("VARDECL: %s\n", node->data.compound.children[0]->data.string_value);
            } else {
                printf("VARDECL\n");
            }
            break;
        case NODE_ASSIGN:
            printf("ASSIGN: %s\n", node->operator ? node->operator : "unknown");
            break;
        case NODE_IF:
            printf("IF\n");
            break;
        case NODE_WHILE:
            printf("WHILE\n");
            break;
        case NODE_FOR:
            printf("FOR\n");
            break;
        case NODE_PRINT:
            printf("PRINT\n");
            break;
        case NODE_SCAN:
            printf("SCAN\n");
            break;
        case NODE_BLOCK:
            printf("BLOCK\n");
            break;
        case NODE_INT_CONST:
            printf("INT_CONST: %d (base %d)\n", node->data.int_const.int_value, node->data.int_const.base);
            break;
        case NODE_CHAR_CONST:
            printf("CHAR_CONST: '%c'\n", node->data.char_value);
            break;
        case NODE_STRING_CONST:
            printf("STRING_CONST: \"%s\"\n", node->data.string_value ? node->data.string_value : "");
            break;
        case NODE_IDENTIFIER:
            printf("IDENTIFIER: %s\n", node->data.string_value ? node->data.string_value : "");
            break;
        case NODE_BINARY_OP:
            printf("BINARY_OP: %s\n", node->operator ? node->operator : "unknown");
            break;
        case NODE_RELOP:
            printf("RELOP: %s\n", node->operator ? node->operator : "unknown");
            break;
        case NODE_COMMENT:
            printf("COMMENT\n");
            break;
        case NODE_TYPE:
            printf("TYPE: %s\n", node->data.string_value ? node->data.string_value : "unknown");
            break;
    }

    // Print children
    for (int i = 0; i < node->data.compound.num_children; i++) {
        print_ast(node->data.compound.children[i], depth + 1);
    }
}

void free_ast(ASTNode* node) {
    if (!node || node->already_freed) return;

    // Mark this node as being freed
    node->already_freed = 1;
    
    // Safe cleanup of operator if present
    if (node->operator) {
        free(node->operator);
        node->operator = NULL;
    }
    
    // Safe cleanup of string value if present and appropriate type
    if (node->type == NODE_IDENTIFIER || node->type == NODE_STRING_CONST || node->type == NODE_TYPE) {
        if (node->data.string_value) {
            free(node->data.string_value);
            node->data.string_value = NULL;
        }
    }
    
    // Safe cleanup of children
    if (node->data.compound.children) {
        for (int i = 0; i < node->data.compound.num_children; i++) {
            if (node->data.compound.children[i]) {
                free_ast(node->data.compound.children[i]);
                node->data.compound.children[i] = NULL;
            }
        }
        
        free(node->data.compound.children);
        node->data.compound.children = NULL;
    }
    
    // Finally, free the node itself
    free(node);
}
