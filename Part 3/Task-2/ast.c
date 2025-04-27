#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

// Global flag to prevent double frees during cleanup
int memory_cleanup_in_progress = 0;

// Convert a number from source base to decimal
int convert_base(int value, int from_base) {
    // For base 10, no conversion needed
    if (from_base == 10) return value;
    
    // Convert to string for digit-by-digit processing
    char buffer[64];
    sprintf(buffer, "%d", value);
    
    int result = 0;
    int len = strlen(buffer);
    
    // Process from left to right, building the decimal value
    for (int i = 0; i < len; i++) {
        int digit = buffer[i] - '0';
        
        // Validate digit against the base
        if (digit >= from_base) {
            fprintf(stderr, "Invalid digit %d for base %d\n", digit, from_base);
            return value; // Return original on error
        }
        
        // Multiply accumulated result by base and add current digit
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
    
    // Initialize all fields to prevent undefined behavior
    memset(node, 0, sizeof(ASTNode));
    
    node->type = type;
    node->operator = NULL;
    node->already_freed = 0;
    node->data.compound.children = NULL;
    node->data.compound.num_children = 0;
    
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
    if (!value) return 0;
    
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
    for (int i = 0; i < depth; i++)
        printf("  ");
    
    // Print based on node type
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
            printf("INT_CONST: %d (base %d)\n", node->data.int_const.int_value, 
                   node->data.int_const.base);
            break;
        
        case NODE_CHAR_CONST:
            printf("CHAR_CONST: '%c'\n", node->data.char_value);
            break;
        
        case NODE_STRING_CONST:
            if (node->data.string_value) {
                printf("STRING_CONST: \"%s\"\n", node->data.string_value);
            } else {
                printf("STRING_CONST: NULL\n");
            }
            break;
        
        case NODE_IDENTIFIER:
            if (node->data.string_value) {
                printf("IDENTIFIER: %s\n", node->data.string_value);
            } else {
                printf("IDENTIFIER: NULL\n");
            }
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
            if (node->data.string_value) {
                printf("TYPE: %s\n", node->data.string_value);
            } else {
                printf("TYPE: NULL\n");
            }
            break;
    }
    
    // Print all children
    for (int i = 0; i < node->data.compound.num_children; i++) {
        if (node->data.compound.children[i]) {
            print_ast(node->data.compound.children[i], depth + 1);
        }
    }
}

// Completely rewritten free_ast function to safely handle memory cleanup
void free_ast(ASTNode* node) {
    // Skip if we're in global cleanup or node is NULL/already freed
    if (!node || node->already_freed || memory_cleanup_in_progress) {
        return;
    }
    
    // Mark as freed immediately to prevent double-free
    node->already_freed = 1;
    
    // Free the operator string if present
    if (node->operator) {
        free(node->operator);
        node->operator = NULL;
    }
    
    // Free string value for appropriate node types
    if ((node->type == NODE_IDENTIFIER || node->type == NODE_STRING_CONST || node->type == NODE_TYPE) && 
        node->data.string_value) {
        free(node->data.string_value);
        node->data.string_value = NULL;
    }
    
    // Free all children
    if (node->data.compound.children) {
        for (int i = 0; i < node->data.compound.num_children; i++) {
            if (node->data.compound.children[i]) {
                free_ast(node->data.compound.children[i]);
                node->data.compound.children[i] = NULL;  // Set to NULL after freeing
            }
        }
        
        // Free the children array pointer
        free(node->data.compound.children);
        node->data.compound.children = NULL;
        node->data.compound.num_children = 0;
    }
    
    // Finally free the node itself
    free(node);
}
