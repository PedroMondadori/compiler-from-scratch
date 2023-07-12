#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

AST_NODE *astCreate(int type, HASH_NODE *symbol, AST_NODE *c0, AST_NODE *c1, AST_NODE *c2, AST_NODE *c3)
{
    AST_NODE *newnode = (AST_NODE *)calloc(1, sizeof(AST_NODE));
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->child[0] = c0;
    newnode->child[1] = c1;
    newnode->child[2] = c2;
    newnode->child[3] = c3;
    return newnode;
}

void astPrint(AST_NODE *node, int depth)
{
    if (node == NULL)
        return;

    for (int i = 0; i < depth; i++)
        printf("  ");
    printf("(");
    switch (node->type)
    {
    case AST_SYMBOL:
        printf("AST_SYMBOL");
        break;
    case AST_ADD:
        printf("AST_ADD");
        break;
    case AST_SUB:
        printf("AST_SUB");
        break;
    case AST_MUL:
        printf("AST_MUL");
        break;
    case AST_DIV:
        printf("AST_DIV");
        break;
    case AST_G:
        printf("AST_G");
        break;
    case AST_L:
        printf("AST_L");
        break;
    case AST_GE:
        printf("AST_GE");
        break;
    case AST_LE:
        printf("AST_LE");
        break;
    case AST_EQ:
        printf("AST_EQ");
        break;
    case AST_DIF:
        printf("AST_DIF");
        break;
    case AST_DECL_LST:
        printf("AST_DECL_LST");
        break;
    case AST_INT:
        printf("AST_INT");
        break;
    case AST_CHAR:
        printf("AST_CHAR");
        break;
    case AST_FLOAT:
        printf("AST_FLOAT");
        break;
    case AST_STRING:
        printf("AST_STRING");
        break;
    case AST_BODY:
        printf("AST_BODY");
        break;
    case AST_PARAM_LST:
        printf("AST_PARAM_LST");
        break;
    case AST_PARAM_CALL:
        printf("AST_PARAM_CALL");
        break;
    case AST_FUNC_DECL:
        printf("AST_FUNC_DECL");
        break;
    case AST_FUNC_CALL:
        printf("AST_FUNC_CALL");
        break;
    case AST_PRINT:
        printf("AST_PRINT");
        break;
    case AST_PARAM_PRINT_01:
        printf("AST_PARAM_PRINT_01");
        break;
    case AST_PARAM_PRINT_02:
        printf("AST_PARAM_PRINT_02");
        break;
    case AST_ARRAY_ACCESS:
        printf("AST_ARRAY_ACCESS");
        break;
    case AST_ASSIGN:
        printf("AST_ASSIGN");
        break;
    case AST_ARRAY_ASSIGN:
        printf("AST_ARRAY_ASSIGN");
        break;
    case AST_CMD_LST_01:
        printf("AST_CMD_LST_01");
        break;
    case AST_CMD_LST_02:
        printf("AST_CMD_LST_02");
        break;
    case AST_READ:
        printf("AST_READ");
        break;
    case AST_IF:
        printf("AST_IF");
        break;
    case AST_WHILE:
        printf("AST_WHILE");
        break;
    case AST_RETURN:
        printf("AST_RETURN");
        break;
    case AST_LABEL:
        printf("AST_LABEL");
        break;
    case AST_GOTO:
        printf("AST_GOTO");
        break;
    case AST_ARRAY:
        printf("AST_ARRAY");
        break;
    case AST_VAR_DECL_01:
        printf("AST_VAR_DECL_01");
        break;
    case AST_VAR_DECL_02:
        printf("AST_VAR_DECL_02");
        break;
    case AST_VAR_DECL_03:
        printf("AST_VAR_DECL_03");
        break;
    case AST_VAR_DECL_04:
        printf("AST_VAR_DECL_04");
        break;
    case AST_EXPR_PARENTHESIS:
        printf("AST_EXPR_PARENTHESIS");
        break;
    default:
        printf("AST_UNKNOWN");
    }
    if (node->symbol != NULL)
        printf(",%s)\n", node->symbol->text);
    else
        printf(",$)\n");

    for (int i = 0; i < MAX_CHILDREN; i++)
    {
        astPrint(node->child[i], depth + 1);
    }
}

void astDecompile(AST_NODE *node, const char *fileName)
{
    FILE *fp = fopen(fileName, "w");
    _astDecompile(node, fp);
    fclose(fp);
}

void _astDecompile(AST_NODE *node, FILE *fp)
{
    if (node == NULL)
        return;

    switch (node->type)
    {
    case AST_SYMBOL:
        fprintf(fp, "%s", node->symbol->text);
        break;
    case AST_ADD:
        _astDecompile(node->child[0], fp);
        fprintf(fp, " + ");
        _astDecompile(node->child[1], fp);
        break;
    case AST_SUB:
        _astDecompile(node->child[0], fp);
        fprintf(fp, " - ");
        _astDecompile(node->child[1], fp);
        break;
    case AST_MUL:
        _astDecompile(node->child[0], fp);
        fprintf(fp, " * ");
        _astDecompile(node->child[1], fp);
        break;
    case AST_DIV:
        _astDecompile(node->child[0], fp);
        fprintf(fp, " / ");
        _astDecompile(node->child[1], fp);
        break;
    case AST_G:
        _astDecompile(node->child[0], fp);
        fprintf(fp, " > ");
        _astDecompile(node->child[1], fp);
        break;
    case AST_L:
        _astDecompile(node->child[0], fp);
        fprintf(fp, " < ");
        _astDecompile(node->child[1], fp);
        break;
    case AST_EQ:
        _astDecompile(node->child[0], fp);
        fprintf(fp, " == ");
        _astDecompile(node->child[1], fp);
        break;
    case AST_GE:
        _astDecompile(node->child[0], fp);
        fprintf(fp, " >= ");
        _astDecompile(node->child[1], fp);
        break;
    case AST_LE:
        _astDecompile(node->child[0], fp);
        fprintf(fp, " <= ");
        _astDecompile(node->child[1], fp);
        break;
    case AST_DIF:
        _astDecompile(node->child[0], fp);
        fprintf(fp, " != ");
        _astDecompile(node->child[1], fp);
        break;
    case AST_DECL_LST:
        _astDecompile(node->child[0], fp);
        _astDecompile(node->child[1], fp);
        break;
    case AST_INT:
        fprintf(fp, "int");
        break;
    case AST_CHAR:
        fprintf(fp, "char");
        break;
    case AST_FLOAT:
        fprintf(fp, "float");
        break;
    case AST_STRING:
        break;
    case AST_BODY:
        fprintf(fp, "{\n");
        _astDecompile(node->child[0], fp);
        fprintf(fp, "}");
        break;
    case AST_PARAM_LST:
        _astDecompile(node->child[0], fp);
        fprintf(fp, " %s", node->child[1]->symbol->text);
        if (node->child[2] != NULL)
            fprintf(fp, ", ");
        _astDecompile(node->child[2], fp);
        break;
    case AST_PARAM_CALL:
        _astDecompile(node->child[0], fp);
        if (node->child[1] != NULL)
            fprintf(fp, ", ");
        _astDecompile(node->child[1], fp);
        break;
    case AST_FUNC_DECL:
        _astDecompile(node->child[0], fp);
        fprintf(fp, " %s(", node->child[1]->symbol->text);
        _astDecompile(node->child[2], fp); // params
        fprintf(fp, ")\n");
        _astDecompile(node->child[3], fp); // block
        fprintf(fp, "\n");
        break;
    case AST_FUNC_CALL:
        fprintf(fp, "%s(", node->child[0]->symbol->text);
        _astDecompile(node->child[1], fp);
        fprintf(fp, ")");
        break;
    case AST_PRINT:
        fprintf(fp, "print ");
        _astDecompile(node->child[0], fp);
        break;
    case AST_PARAM_PRINT_01:
        fprintf(fp, "%s", node->child[0]->symbol->text);
        if (node->child[1] != NULL)
            fprintf(fp, ", ");
        _astDecompile(node->child[1], fp);
        break;
    case AST_PARAM_PRINT_02:
        _astDecompile(node->child[0], fp);
        if (node->child[1] != NULL)
            fprintf(fp, ", ");
        _astDecompile(node->child[1], fp);
        break;
    case AST_ARRAY_ACCESS:
        fprintf(fp, "%s[", node->child[0]->symbol->text);
        _astDecompile(node->child[1], fp);
        fprintf(fp, "]");
        break;
    case AST_ASSIGN:
        _astDecompile(node->child[0], fp);
        fprintf(fp, " = ");
        _astDecompile(node->child[1], fp);
        break;
    case AST_ARRAY_ASSIGN:
        _astDecompile(node->child[0], fp);
        fprintf(fp, "[");
        _astDecompile(node->child[1], fp);
        fprintf(fp, "]");
        fprintf(fp, " = ");
        _astDecompile(node->child[2], fp);
        break;
    case AST_CMD_LST_01:
        _astDecompile(node->child[0], fp);
        fprintf(fp, ";\n");
        _astDecompile(node->child[1], fp);
        break;
    case AST_CMD_LST_02:
        _astDecompile(node->child[0], fp);
        _astDecompile(node->child[1], fp);
        break;
    case AST_READ:
        fprintf(fp, "read");
        break;
    case AST_IF:
        fprintf(fp, "if ");
        _astDecompile(node->child[0], fp);
        fprintf(fp, " then\n");
        _astDecompile(node->child[1], fp);
        if (node->child[2] != NULL)
        {
            fprintf(fp, "\nelse\n");
            _astDecompile(node->child[2], fp);
        }
        break;
    case AST_WHILE:
        fprintf(fp, "while ");
        _astDecompile(node->child[0], fp);
        fprintf(fp, "\n");
        _astDecompile(node->child[1], fp);
        break;
    case AST_RETURN:
        fprintf(fp, "return ");
        _astDecompile(node->child[0], fp);
        break;
    case AST_LABEL:
        fprintf(fp, "%s:\n", node->child[0]->symbol->text);
        break;
    case AST_GOTO:
        fprintf(fp, "goto %s", node->child[0]->symbol->text);
        break;
    case AST_ARRAY:
        fprintf(fp, "%s", node->child[0]->symbol->text);
        if (node->child[1] == NULL)
            fprintf(fp, ";\n");
        else
        {
            fprintf(fp, " ");
            _astDecompile(node->child[1], fp);
        }
        break;
    case AST_VAR_DECL_01:
        _astDecompile(node->child[0], fp);
        fprintf(fp, " %s: %s;\n", node->child[1]->symbol->text, node->child[2]->symbol->text);
        break;
    case AST_VAR_DECL_02:
        _astDecompile(node->child[0], fp);
        fprintf(fp, " %s[%s];\n", node->child[1]->symbol->text, node->child[2]->symbol->text);
        break;
    case AST_VAR_DECL_03:
        _astDecompile(node->child[0], fp);
        fprintf(fp, " %s[%s]: ", node->child[1]->symbol->text, node->child[2]->symbol->text);
        _astDecompile(node->child[3], fp);
        break;
    case AST_VAR_DECL_04:
        _astDecompile(node->child[0], fp);
        fprintf(fp, " %s: %s / %s;\n", node->child[1]->symbol->text, node->child[2]->symbol->text, node->child[3]->symbol->text);
        break;
    case AST_EXPR_PARENTHESIS:
        fprintf(fp, "(");
        _astDecompile(node->child[0], fp);
        fprintf(fp, ")");
        break;
    }
}
