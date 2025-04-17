#ifndef AST_H
#define AST_H

typedef enum {
    NODE_PROGRAM,
    NODE_VARDECL,
    NODE_ASSIGN,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_PRINT,
    NODE_SCAN,
    NODE_BLOCK,
    NODE_INT_CONST,
    NODE_CHAR_CONST,
    NODE_STRING_CONST,
    NODE_IDENTIFIER,
    NODE_BINARY_OP,
    NODE_RELOP,
    NODE_COMMENT,
    NODE_TYPE
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char* operator; // For nodes with operators, like NODE_BINARY_OP, NODE_ASSIGN, NODE_RELOP
    union {
        char char_value;   // For character constants
        char* string_value; // For identifiers, strings, types
        struct {
            struct ASTNode** children;
            int num_children;
        } compound;
        struct {
            int int_value; // For integer constant value
            int base;      // For integer constant base
        } int_const;
    } data;
} ASTNode;

ASTNode* create_node(NodeType type);
void add_child(ASTNode* parent, ASTNode* child);
void print_ast(ASTNode* node, int depth);
void free_ast(ASTNode* node);
int is_valid_integer(const char* value, int base);

#endif
