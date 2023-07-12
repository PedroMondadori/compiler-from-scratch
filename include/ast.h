#ifndef AST_HEADER
#define AST_HEADER

#include "hash.h"

#define MAX_CHILDREN 4

typedef enum
{
    AST_SYMBOL,
    AST_ADD,
    AST_SUB,
    AST_MUL,
    AST_DIV,
    AST_G,
    AST_L,
    AST_EQ,
    AST_GE,
    AST_LE,
    AST_DIF,
    AST_DECL_LST,
    AST_INT,
    AST_CHAR,
    AST_FLOAT,
    AST_STRING,
    AST_BODY,
    AST_PARAM_LST,
    AST_PARAM_CALL,
    AST_FUNC_DECL,
    AST_FUNC_CALL,
    AST_PRINT,
    AST_PARAM_PRINT_01,
    AST_PARAM_PRINT_02,
    AST_ARRAY_ACCESS,
    AST_ASSIGN,
    AST_ARRAY_ASSIGN,
    AST_CMD_LST_01,
    AST_CMD_LST_02,
    AST_READ,
    AST_IF,
    AST_WHILE,
    AST_RETURN,
    AST_LABEL,
    AST_GOTO,
    AST_ARRAY,
    AST_VAR_DECL_01,
    AST_VAR_DECL_02,
    AST_VAR_DECL_03,
    AST_VAR_DECL_04,
    AST_EXPR_PARENTHESIS,
} ASTNodeType;

typedef struct astNode
{
    int type;
    int datatype;
    HASH_NODE *symbol;
    struct astNode *child[MAX_CHILDREN];
} AST_NODE;

AST_NODE *astCreate(int type, HASH_NODE *symbol, AST_NODE *c0, AST_NODE *c1, AST_NODE *c2, AST_NODE *c3);
void astPrint(AST_NODE *node, int depth);
void astDecompile(AST_NODE *node, const char *fileName);
void _astDecompile(AST_NODE *node, FILE *fp);

#endif
