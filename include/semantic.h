#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include "hash.h"
#include "ast.h"

extern int SEMANTIC_ERRORS;

void checkAndSetDeclarations(AST_NODE *node);
void checkUndeclared();
void checkOperands(AST_NODE *node);
int isNumber(AST_NODE *node);
int isArithmetic(int type);
void checkAndSetNature(AST_NODE *node);
void checkBool(AST_NODE *node);
void checkVectorIndex(AST_NODE *node);
void checkVectorSize(AST_NODE *node);
int countVectorSize(AST_NODE *node);
void checkFuncParams(AST_NODE *node);
int countFuncParams(AST_NODE *node);
int countFuncDeclParams(AST_NODE *node, HASH_NODE *symbol, int index);
void checkFuncParamTypes(AST_NODE *node);

#endif
