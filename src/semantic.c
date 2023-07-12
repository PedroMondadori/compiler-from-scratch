#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "semantic.h"

int SEMANTIC_ERRORS = 0;

void setDeclared(AST_NODE *node, const char *nature, int symbolType)
{
    if (node->child[1]->symbol->type != SYMBOL_IDENTIFIER)
    {
        fprintf(stderr, "Semantic Error: %s %s already declared\n", nature ? nature : "",
                node->child[1] ? (node->child[1]->symbol->text ? node->child[1]->symbol->text
                                                               : "")
                               : "");
        SEMANTIC_ERRORS++;
    }
    node->child[1]->symbol->type = symbolType;
    if (node->child[0]->type == AST_INT)
    {
        node->child[1]->symbol->datatype = DATATYPE_INT;
    }
    if (node->child[0]->type == AST_CHAR)
    {
        node->child[1]->symbol->datatype = DATATYPE_CHAR;
    }
    if (node->child[0]->type == AST_FLOAT)
    {
        node->child[1]->symbol->datatype = DATATYPE_FLOAT;
    }
}

void checkAndSetDeclarations(AST_NODE *node)
{
    if (node == NULL)
        return;

    AST_NODE *aux;
    int index;

    switch (node->type)
    {
    case AST_VAR_DECL_01:
        node->child[1]->symbol->initValue[0] = atoi(node->child[2]->symbol->text);
        setDeclared(node, "variable", SYMBOL_VARIABLE);
        break;
    case AST_VAR_DECL_04:
        node->child[1]->symbol->initValue[0] = atoi(node->child[2]->symbol->text);
        node->child[1]->symbol->initValue[1] = atoi(node->child[3]->symbol->text);
        setDeclared(node, "variable", SYMBOL_VARIABLE);
        break;

    case AST_VAR_DECL_02:
        setDeclared(node, "vector", SYMBOL_VECTOR);
        node->child[1]->symbol->paramCount = atoi(node->child[2]->symbol->text);
        break;

    case AST_VAR_DECL_03:
        setDeclared(node, "vector", SYMBOL_VECTOR);
        aux = node->child[3];
        index = 0;
        while (aux != NULL)
        {
            node->child[1]->symbol->paramCount += 1;
            node->child[1]->symbol->initValue[index] = atoi(aux->child[0]->symbol->text);
            index++;
            aux = aux->child[1];
        }
        break;

    case AST_FUNC_DECL:
        setDeclared(node, "function", SYMBOL_FUNC);
        if (node->child[2] == NULL)
            node->child[1]->symbol->paramCount = 0;
        else
        {
            node->child[1]->symbol->paramCount = countFuncDeclParams(node->child[2], node->child[1]->symbol, 0);
            // if(node->child[1] && node->child[1]->symbol){
            //     fprintf(stderr, "%s\n", node->child[1]->symbol->params[0]);
            //     fprintf(stderr, "%s\n", node->child[1]->symbol->params[1]);
            //     fprintf(stderr, "%s\n", node->child[1]->symbol->params[2]);
            // }
            // else
            //     fprintf(stderr, "");
        }
        break;

    case AST_LABEL:
        if (node->child[0]->symbol->type != SYMBOL_IDENTIFIER)
        {
            fprintf(stderr, "Semantic Error: label %s already declared\n", node->child[0]->symbol->text);
            SEMANTIC_ERRORS++;
        }
        node->child[0]->symbol->type = SYMBOL_LABEL;
        break;

    case AST_PARAM_LST:
        setDeclared(node, "variable", SYMBOL_VARIABLE);

    default:
        break;
    }

    for (int i = 0; i < MAX_CHILDREN; i++)
    {
        checkAndSetDeclarations(node->child[i]);
    }
}

void checkUndeclared()
{
    SEMANTIC_ERRORS += hashCheckUndeclared();
}

void checkOperands(AST_NODE *node)
{
    if (node == NULL)
        return;

    for (int i = 0; i < MAX_CHILDREN; i++)
    {
        checkOperands(node->child[i]);
    }

    switch (node->type)
    {
    case AST_ADD:
    case AST_SUB:
    case AST_MUL:
    case AST_DIV:
        if (!isNumber(node->child[0]))
        {
            fprintf(stderr, "Semantic Error: Invalid left operand\n");
            SEMANTIC_ERRORS++;
        }
        if (!isNumber(node->child[1]))
        {
            fprintf(stderr, "Semantic Error: Invalid Right operand\n");
            SEMANTIC_ERRORS++;
        }

        if ((node->child[0]->symbol && node->child[0]->symbol->datatype == DATATYPE_FLOAT) || (node->child[0]->datatype == DATATYPE_FLOAT))
            node->datatype = DATATYPE_FLOAT;
        if ((node->child[0]->symbol && node->child[0]->symbol->datatype == DATATYPE_INT) || (node->child[0]->datatype == DATATYPE_INT))
            node->datatype = DATATYPE_INT;
        if ((node->child[0]->symbol && node->child[0]->symbol->datatype == DATATYPE_CHAR) || (node->child[0]->datatype == DATATYPE_CHAR))
            node->datatype = DATATYPE_CHAR;

        if ((node->child[1]->symbol && node->child[1]->symbol->datatype == DATATYPE_FLOAT) || (node->child[1]->datatype == DATATYPE_FLOAT))
            node->datatype = DATATYPE_FLOAT;
        if ((node->child[1]->symbol && node->child[1]->symbol->datatype == DATATYPE_INT) || (node->child[1]->datatype == DATATYPE_INT))
            node->datatype = DATATYPE_INT;
        if ((node->child[1]->symbol && node->child[1]->symbol->datatype == DATATYPE_CHAR) || (node->child[1]->datatype == DATATYPE_CHAR))
            node->datatype = DATATYPE_CHAR;
        break;

    case AST_G:
    case AST_L:
    case AST_GE:
    case AST_LE:
    case AST_EQ:
    case AST_DIF:
        if (!isNumber(node->child[0]))
        {
            fprintf(stderr, "Semantic Error: Invalid left operand\n");
            SEMANTIC_ERRORS++;
        }
        if (!isNumber(node->child[1]))
        {
            fprintf(stderr, "Semantic Error: Invalid Right operand\n");
            SEMANTIC_ERRORS++;
        }
        node->datatype = DATATYPE_BOOL;
        break;

    case AST_EXPR_PARENTHESIS:
        node->datatype = node->child[0]->datatype;
        if (node->child[0]->symbol)
        {
            node->datatype = node->child[0]->symbol->datatype;
        }
        break;
    
    case AST_WHILE:
        if (isNumber(node->child[0])) {
            node->child[0]->datatype = DATATYPE_BOOL;
        }
        break;

    default:
        break;
    }
}

void checkAndSetNature(AST_NODE *node)
{
    if (node == NULL)
        return;

    switch (node->type)
    {
    case AST_ASSIGN:
        if (node->child[0]->type == AST_SYMBOL)
        {
            if (node->child[0]->symbol->type != SYMBOL_VARIABLE)
            {
                fprintf(stderr, "Semantic Error: identifier is not a variable\n");
                SEMANTIC_ERRORS++;
            }
        }
        if (node->child[0]->type == AST_ARRAY_ACCESS)
        {
            if (node->child[0]->child[0]->symbol->type != SYMBOL_VECTOR)
            {
                fprintf(stderr, "Semantic Error: identifier is not a vector\n");
                SEMANTIC_ERRORS++;
            }
        }
        if (node->child[1]->type == AST_SYMBOL)
        {
            if (node->child[1]->symbol->type == SYMBOL_VECTOR)
            {
                fprintf(stderr, "Semantic Error: vector cannot be used as scalar\n");
                SEMANTIC_ERRORS++;
            }
            if (node->child[1]->symbol->type == SYMBOL_FUNC)
            {
                fprintf(stderr, "Semantic Error: function cannot be used as scalar\n");
                SEMANTIC_ERRORS++;
            }
        }
        break;
    case AST_FUNC_CALL:
        if (node->child[0]->symbol->type != SYMBOL_FUNC)
        {
            fprintf(stderr, "Semantic Error: identifier is not a function\n");
            SEMANTIC_ERRORS++;
        }
        break;
    case AST_ARRAY_ACCESS:
        if (node->child[0]->symbol->type != SYMBOL_VECTOR)
        {
            fprintf(stderr, "Semantic Error: identifier is not a vector\n");
            SEMANTIC_ERRORS++;
        }
        break;
    case AST_GOTO:
        if (node->child[0]->symbol->type != SYMBOL_LABEL)
        {
            fprintf(stderr, "Semantic Error: identifier is not a label\n");
            SEMANTIC_ERRORS++;
        }
        break;
    }

    for (int i = 0; i < MAX_CHILDREN; i++)
    {
        checkAndSetNature(node->child[i]);
    }
}

void checkBool(AST_NODE *node)
{
    if (node == NULL)
        return;

    switch (node->type)
    {
    case AST_RETURN:
        if (node->child[0]->datatype == DATATYPE_BOOL ||
            (node->child[0]->symbol && (node->child[0]->symbol->datatype == DATATYPE_BOOL)))
        {
            fprintf(stderr, "Semantic Error: return value does not match function type\n");
            SEMANTIC_ERRORS++;
        }
        break;

    case AST_ASSIGN:
        if (node->child[1]->datatype == DATATYPE_BOOL ||
            (node->child[1]->symbol && (node->child[1]->symbol->datatype == DATATYPE_BOOL)))
        {
            fprintf(stderr, "Semantic Error: cannot assign bool expression to variable\n");
            SEMANTIC_ERRORS++;
        }
        break;

    case AST_WHILE:
    case AST_IF:
        if (node->child[0]->datatype != DATATYPE_BOOL ||
            (node->child[0]->symbol && (node->child[0]->symbol->datatype != DATATYPE_BOOL)))
        {
            fprintf(stderr, "Semantic Error: expression of conditional statement must be of type bool\n");
            SEMANTIC_ERRORS++;
        }
        break;

    case AST_FUNC_CALL:
        checkFuncParamTypes(node->child[1]);
        break;
    }

    for (int i = 0; i < MAX_CHILDREN; i++)
    {
        checkBool(node->child[i]);
    }
}

void checkVectorIndex(AST_NODE *node)
{
    if (node == NULL)
        return;

    for (int i = 0; i < MAX_CHILDREN; i++)
    {
        checkVectorIndex(node->child[i]);
    }

    switch (node->type)
    {
    case AST_ARRAY_ACCESS:
        if ((node->child[1]->datatype == DATATYPE_BOOL ||
             (node->child[1]->symbol && (node->child[1]->symbol->datatype == DATATYPE_BOOL))) ||
            (node->child[1]->datatype == DATATYPE_FLOAT ||
             (node->child[1]->symbol && (node->child[1]->symbol->datatype == DATATYPE_FLOAT))))
        {
            fprintf(stderr, "Semantic Error: vector index must be of type int or char\n");
            SEMANTIC_ERRORS++;
        }
        node->datatype = node->child[0]->symbol->datatype;
    }
}

void checkVectorSize(AST_NODE *node)
{
    if (node == NULL)
        return;

    int count;

    switch (node->type)
    {
    case AST_VAR_DECL_03:
        count = countVectorSize(node->child[3]);
        if (atoi(node->child[2]->symbol->text) != count)
        {
            fprintf(stderr, "Semantic Error: vector size %s differs from number of members %d\n", node->child[2]->symbol->text, count);
            SEMANTIC_ERRORS++;
        }
    }

    for (int i = 0; i < MAX_CHILDREN; i++)
    {
        checkVectorSize(node->child[i]);
    }
}

int countVectorSize(AST_NODE *node)
{
    if (node->child[1] == NULL)
    {
        return 1;
    }
    return 1 + countVectorSize(node->child[1]);
}

void checkFuncParams(AST_NODE *node)
{
    if (node == NULL)
        return;

    int count;

    switch (node->type)
    {
    case AST_FUNC_CALL:
        if (node->child[1])
        {
            count = countFuncParams(node->child[1]);
        }
        else
            count = 0;

        if (node->child[0]->symbol && (node->child[0]->symbol->paramCount != count))
        {
            fprintf(stderr, "Semantic Error: number of function parameters %d differs from function declaration %d\n", count, node->child[0]->symbol->paramCount);
            SEMANTIC_ERRORS++;
        }
    }

    for (int i = 0; i < MAX_CHILDREN; i++)
    {
        checkFuncParams(node->child[i]);
    }
}

void checkFuncParamTypes(AST_NODE *node)
{
    if (node == NULL)
        return;

    if (node->child[0]->datatype == DATATYPE_BOOL || (node->child[0]->symbol && (node->child[0]->symbol->datatype == DATATYPE_BOOL)))
    {
        fprintf(stderr, "Semantic Error: function call parameter cannot be of type bool\n");
        SEMANTIC_ERRORS++;
    }

    for (int i = 0; i < MAX_CHILDREN; i++)
    {
        checkFuncParams(node->child[i]);
    }
}

int countFuncParams(AST_NODE *node)
{
    if (node->child[1] == NULL)
    {
        return 1;
    }
    return 1 + countFuncParams(node->child[1]);
}

int countFuncDeclParams(AST_NODE *node, HASH_NODE *symbol, int index)
{
    symbol->params[index] = (char *)malloc(sizeof(char) * (strlen(node->child[1]->symbol->text)));
    strcpy(symbol->params[index], node->child[1]->symbol->text);
    if (node->child[2] == NULL)
    {
        return 1;
    }
    return 1 + countFuncDeclParams(node->child[2], symbol, ++index);
}

int isArithmetic(int type)
{
    return (type == AST_ADD) || (type == AST_SUB) || (type == AST_DIV) || (type == AST_MUL);
}

int isNumber(AST_NODE *node)
{
    return isArithmetic(node->type) ||
           ((node->type == AST_EXPR_PARENTHESIS) && isNumber(node->child[0])) ||
           ((node->type == AST_SYMBOL) && (node->symbol->type == SYMBOL_VARIABLE)) ||
           ((node->type == AST_ARRAY_ACCESS) && (node->child[0]->symbol->type == SYMBOL_VECTOR)) ||
           ((node->type == AST_FUNC_CALL) && (node->child[0]->symbol->type == SYMBOL_FUNC)) ||
           ((node->type == AST_SYMBOL) && (node->symbol->type == SYMBOL_LIT_INTEGER)) ||
           ((node->type == AST_SYMBOL) && (node->symbol->type == SYMBOL_LIT_CHAR)) ||
           ((node->type == AST_READ));
}
