#include <stdlib.h>
#include <string.h>

#include "tacs.h"

TAC_NODE *makeRelational(int type, TAC_NODE *code0, TAC_NODE *code1);
TAC_NODE *makeArithmetic(int type, TAC_NODE *code0, TAC_NODE *code1);
TAC_NODE *makeIfThen(TAC_NODE *code0, TAC_NODE *code1);
TAC_NODE *makeIfThenElse(TAC_NODE *code0, TAC_NODE *code1, TAC_NODE *code2);
TAC_NODE *makeWhile(TAC_NODE *code0, TAC_NODE *code1);
TAC_NODE *makeFunction(TAC_NODE *funIdentifier, TAC_NODE *funBody);

TAC_NODE *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2)
{
    TAC_NODE *newTac = (TAC_NODE *)calloc(1, sizeof(TAC_NODE));
    newTac->type = type;
    newTac->res = res;
    newTac->op1 = op1;
    newTac->op2 = op2;
    newTac->prev = NULL;
    newTac->next = NULL;
    return newTac;
}

void tacPrint(TAC_NODE *tac)
{
    if (tac == NULL)
        return;
    if (tac->type == TAC_SYMBOL)
        return;
    if (tac->type == TAC_STRING)
        return;
    printf("(");
    switch (tac->type)
    {
    // case TAC_SYMBOL: printf("TAC_SYMBOL"); break;
    case TAC_ADD:
        printf("TAC_ADD");
        break;
    case TAC_SUB:
        printf("TAC_SUB");
        break;
    case TAC_MUL:
        printf("TAC_MUL");
        break;
    case TAC_DIV:
        printf("TAC_DIV");
        break;
    case TAC_G:
        printf("TAC_G");
        break;
    case TAC_L:
        printf("TAC_L");
        break;
    case TAC_GE:
        printf("TAC_GE");
        break;
    case TAC_LE:
        printf("TAC_LE");
        break;
    case TAC_EQ:
        printf("TAC_EQ");
        break;
    case TAC_DIF:
        printf("TAC_DIF");
        break;
    case TAC_MOVE:
        printf("TAC_MOVE");
        break;
    case TAC_JMP_FALSE:
        printf("TAC_JMP_FALSE");
        break;
    case TAC_JMP_ABS:
        printf("TAC_JMP_ABS");
        break;
    case TAC_LABEL:
        printf("TAC_LABEL");
        break;
    case TAC_BEGINFUN:
        printf("TAC_BEGINFUN");
        break;
    case TAC_ENDFUN:
        printf("TAC_ENDFUN");
        break;
    case TAC_RET:
        printf("TAC_RET");
        break;
    case TAC_READ:
        printf("TAC_READ");
        break;
    case TAC_ARRAY_ACCESS:
        printf("TAC_ARRAY_ACCESS");
        break;
    case TAC_ARG:
        printf("TAC_ARG");
        break;
    case TAC_CALL:
        printf("TAC_CALL");
        break;
    case TAC_PRINT_INT:
        printf("TAC_PRINT_INT");
        break;
    case TAC_PRINT_STR:
        printf("TAC_PRINT_STR");
        break;
    case TAC_PRINT:
        printf("TAC_PRINT");
        break;
    default:
        printf("TAC_UNKNOWN");
    }
    printf(",%s", tac->res ? tac->res->text : "0");
    printf(",%s", tac->op1 ? tac->op1->text : "0");
    printf(",%s", tac->op2 ? tac->op2->text : "0");
    printf(")\n");
}

void tacPrintBackwards(TAC_NODE *tac)
{
    if (tac == NULL)
        return;
    else
    {
        tacPrintBackwards(tac->prev);
        tacPrint(tac);
    }
}

TAC_NODE *tacJoin(TAC_NODE *l1, TAC_NODE *l2)
{
    if (l1 == NULL)
        return l2;
    if (l2 == NULL)
        return l1;

    TAC_NODE *point;
    for (point = l2; point->prev != NULL; point = point->prev)
    {
        ;
    }
    point->prev = l1;
    return l2;
}

// CODE GENERATION

TAC_NODE *generateCode(AST_NODE *node)
{
    if (node == NULL)
        return NULL;

    TAC_NODE *code[MAX_CHILDREN] = {NULL};

    for (int i = 0; i < MAX_CHILDREN; i++)
    {
        code[i] = generateCode(node->child[i]);
    }

    TAC_NODE *result = NULL;

    switch (node->type)
    {
    case AST_SYMBOL:
        result = tacCreate(TAC_SYMBOL, node->symbol, 0, 0);
        break;
    case AST_STRING:
        result = tacCreate(TAC_STRING, node->symbol, 0, 0);
        break;
    case AST_ADD:
        result = makeArithmetic(TAC_ADD, code[0], code[1]);
        break;
    case AST_SUB:
        result = makeArithmetic(TAC_SUB, code[0], code[1]);
        break;
    case AST_MUL:
        result = makeArithmetic(TAC_MUL, code[0], code[1]);
        break;
    case AST_DIV:
        result = makeArithmetic(TAC_DIV, code[0], code[1]);
        break;
    case AST_G:
        result = makeRelational(TAC_G, code[0], code[1]);
        break;
    case AST_L:
        result = makeRelational(TAC_L, code[0], code[1]);
        break;
    case AST_GE:
        result = makeRelational(TAC_GE, code[0], code[1]);
        break;
    case AST_LE:
        result = makeRelational(TAC_LE, code[0], code[1]);
        break;
    case AST_EQ:
        result = makeRelational(TAC_EQ, code[0], code[1]);
        break;
    case AST_DIF:
        result = makeRelational(TAC_DIF, code[0], code[1]);
        break;
    case AST_ASSIGN:
        result = tacJoin(tacJoin(code[0], code[1]), tacCreate(TAC_MOVE, code[0] ? code[0]->res : NULL, code[1] ? code[1]->res : NULL, 0));
        break;
    case AST_ARRAY_ASSIGN:
        result = tacJoin(tacJoin(code[2], tacJoin(code[0], code[1])), tacCreate(TAC_MOVE, code[0] ? code[0]->res : NULL, code[1] ? code[1]->res : NULL, code[2] ? code[2]->res : NULL));
        break;
    case AST_IF:
        result = code[2] ? makeIfThenElse(code[0], code[1], code[2]) : makeIfThen(code[0], code[1]);
        break;
    case AST_WHILE:
        result = makeWhile(code[0], code[1]);
        break;
    case AST_FUNC_DECL:
        result = makeFunction(code[1], code[3]);
        break;
    case AST_RETURN:
        result = tacJoin(code[0], tacCreate(TAC_RET, code[0] ? code[0]->res : NULL, 0, 0));
        break;
    case AST_READ:
        result = tacCreate(TAC_READ, makeTemp(), 0, 0);
        break;
    case AST_LABEL:
        result = tacJoin(code[0], tacCreate(TAC_LABEL, code[0] ? code[0]->res : NULL, 0, 0));
        break;
    case AST_GOTO:
        result = tacJoin(code[0], tacCreate(TAC_JMP_ABS, code[0] ? code[0]->res : NULL, 0, 0));
        break;
    case AST_ARRAY_ACCESS:
        result = tacJoin(tacJoin(code[0], code[1]), tacCreate(TAC_ARRAY_ACCESS, makeTemp(), code[0] ? code[0]->res : NULL, code[1] ? code[1]->res : NULL));
        break;
    case AST_PARAM_CALL:
        result = tacJoin(tacJoin(code[0], code[1]), tacCreate(TAC_ARG, code[0] ? code[0]->res : NULL, 0, 0));
        break;
    case AST_FUNC_CALL:
        result = tacJoin(code[1], tacCreate(TAC_CALL, makeTemp(), code[0] ? code[0]->res : NULL, 0));
        break;
    case AST_PARAM_PRINT_02:
        result = tacJoin(tacJoin(code[0], code[1]), tacCreate(TAC_PRINT_INT, code[0] ? code[0]->res : NULL, 0, 0));
        break;
    case AST_PARAM_PRINT_01:
        result = tacJoin(tacJoin(code[0], code[1]), tacCreate(TAC_PRINT_STR, code[0] ? code[0]->res : NULL, 0, 0));
        break;
    case AST_PRINT:
        result = tacJoin(code[0], tacCreate(TAC_PRINT, 0, 0, 0));
        break;

    default:
        result = tacJoin(code[0], tacJoin(code[1], tacJoin(code[2], code[3])));
    }

    return result;
}

TAC_NODE *makeRelational(int type, TAC_NODE *code0, TAC_NODE *code1)
{
    TAC_NODE *l1 = tacJoin(code0, code1);
    TAC_NODE *l2 = tacCreate(type, makeTemp(), code0 ? code0->res : NULL, code1 ? code1->res : NULL);
    return tacJoin(l1, l2);
}

TAC_NODE *makeArithmetic(int type, TAC_NODE *code0, TAC_NODE *code1)
{
    TAC_NODE *l1 = tacJoin(code0, code1);
    TAC_NODE *l2 = tacCreate(type, makeTemp(), code0 ? code0->res : NULL, code1 ? code1->res : NULL);
    return tacJoin(l1, l2);
}

TAC_NODE *makeIfThen(TAC_NODE *code0, TAC_NODE *code1)
{
    TAC_NODE *jmp = NULL;
    TAC_NODE *label = NULL;
    HASH_NODE *newLabel = makeLabel();

    jmp = tacCreate(TAC_JMP_FALSE, newLabel, code0 ? code0->res : NULL, NULL);
    jmp = tacJoin(code0, jmp);
    label = tacCreate(TAC_LABEL, newLabel, NULL, NULL);
    label = tacJoin(code1, label);
    return tacJoin(jmp, label);
}

TAC_NODE *makeIfThenElse(TAC_NODE *code0, TAC_NODE *code1, TAC_NODE *code2)
{
    TAC_NODE *jmpElse = NULL;
    TAC_NODE *jmpAbs = NULL;
    TAC_NODE *tacLabelElse = NULL;
    TAC_NODE *tacLabelAbs = NULL;
    HASH_NODE *labelElse = makeLabel();
    HASH_NODE *labelAbs = makeLabel();

    jmpElse = tacCreate(TAC_JMP_FALSE, labelElse, code0 ? code0->res : NULL, NULL);
    jmpElse = tacJoin(tacJoin(code0, jmpElse), code1);
    jmpAbs = tacCreate(TAC_JMP_ABS, labelAbs, NULL, NULL);
    jmpAbs = tacJoin(code1, jmpAbs);
    tacLabelElse = tacCreate(TAC_LABEL, labelElse, NULL, NULL);
    tacLabelElse = tacJoin(tacJoin(jmpAbs, tacLabelElse), code2);
    tacLabelAbs = tacCreate(TAC_LABEL, labelAbs, NULL, NULL);
    return tacJoin(code2, tacLabelAbs);
}

TAC_NODE *makeWhile(TAC_NODE *code0, TAC_NODE *code1)
{
    TAC_NODE *jmpFalse = NULL;
    TAC_NODE *jmpAbs = NULL;
    TAC_NODE *tacLabelFalse = NULL;
    TAC_NODE *tacLabelStart = NULL;
    HASH_NODE *labelStart = makeLabel();
    HASH_NODE *labelFalse = makeLabel();

    tacLabelStart = tacCreate(TAC_LABEL, labelStart, NULL, NULL);
    code0 = tacJoin(tacLabelStart, code0);
    jmpFalse = tacCreate(TAC_JMP_FALSE, labelFalse, code0 ? code0->res : NULL, NULL);
    code1 = tacJoin(tacJoin(code0, jmpFalse), code1);
    jmpAbs = tacJoin(code1, tacCreate(TAC_JMP_ABS, labelStart, NULL, NULL));
    tacLabelFalse = tacJoin(jmpAbs, tacCreate(TAC_LABEL, labelFalse, NULL, NULL));
    return tacLabelFalse;
}

TAC_NODE *makeFunction(TAC_NODE *funIdentifier, TAC_NODE *funBody)
{
    TAC_NODE *beginFun = NULL;
    TAC_NODE *endFun = NULL;

    beginFun = tacCreate(TAC_BEGINFUN, funIdentifier ? funIdentifier->res : NULL, NULL, NULL);
    funBody = tacJoin(beginFun, funBody);
    endFun = tacCreate(TAC_ENDFUN, funIdentifier ? funIdentifier->res : NULL, NULL, NULL);
    return tacJoin(funBody, endFun);
}

// ASM GENERATION

TAC_NODE *tacReverse(TAC_NODE *tac)
{
    TAC_NODE *t = tac;
    for (t = tac; t->prev; t = t->prev)
        t->prev->next = t;
    return t;
}

void generateASM(TAC_NODE *first)
{
    FILE *fout;
    fout = fopen("out.s", "w");

    // Init
    fprintf(fout,
            "\t.section .rodata\n"
            ".PRINT_INT_STR:\n"
            "\t.string	\"%%d\\n\"\n"
            ".PRINT_STRING_STR:\n"
            "\t.string \"%%s\\n\"\n"
            ".READ_INT_STR:\n"
            "\t.string \"%%d\"\n"
            "\n");

    // Tacs
    for (TAC_NODE *tac = first; tac; tac = tac->next)
    {
        switch (tac->type)
        {
        case TAC_ADD:
            fprintf(fout,
                    "## TAC_ADD\n"
                    "\tmovl\t%s, %%edx\n"
                    "\tmovl\t%s, %%eax\n"
                    "\taddl\t%%edx, %%eax\n"
                    "\tmovl\t%%eax, _%s(%%rip)\n"
                    "\n",
                    tac->op1 ? tacCheckOperandType(tac->op1) : "",
                    tac->op2 ? tacCheckOperandType(tac->op2) : "",
                    tac->res ? tac->res->text : "");
            break;
        case TAC_SUB:
            fprintf(fout,
                    "## TAC_SUB\n"
                    "\tmovl\t%s, %%edx\n"
                    "\tmovl\t%s, %%eax\n"
                    "\tsubl\t%%eax, %%edx\n"
                    "\tmovl\t%%edx, %%eax\n"
                    "\tmovl\t%%eax, _%s(%%rip)\n"
                    "\n",
                    tac->op1 ? tacCheckOperandType(tac->op1) : "",
                    tac->op2 ? tacCheckOperandType(tac->op2) : "",
                    tac->res ? tac->res->text : "");
            break;
        case TAC_MUL:
            fprintf(fout,
                    "## TAC_MUL\n"
                    "\tmovl\t%s, %%edx\n"
                    "\tmovl\t%s, %%eax\n"
                    "\timull\t%%edx, %%eax\n"
                    "\tmovl\t%%eax, _%s(%%rip)\n"
                    "\n",
                    tac->op1 ? tacCheckOperandType(tac->op1) : "",
                    tac->op2 ? tacCheckOperandType(tac->op2) : "",
                    tac->res ? tac->res->text : "");
            break;
        case TAC_DIV:
            fprintf(fout,
                    "## TAC_DIV\n"
                    "\tmovl\t%s, %%eax\n"
                    "\tmovl\t%s, %%ecx\n"
                    "\tcltd\n"
                    "\tidivl\t%%ecx\n"
                    "\tmovl\t%%eax, _%s(%%rip)\n"
                    "\n",
                    tac->op1 ? tacCheckOperandType(tac->op1) : "",
                    tac->op2 ? tacCheckOperandType(tac->op2) : "",
                    tac->res ? tac->res->text : "");
            break;
        case TAC_G:
            tacHandleRelational(tac, fout, "setg", "TAC_G");
            break;
        case TAC_L:
            tacHandleRelational(tac, fout, "setl", "TAC_L");
            break;
        case TAC_GE:
            tacHandleRelational(tac, fout, "setge", "TAC_GE");
            break;
        case TAC_LE:
            tacHandleRelational(tac, fout, "setle", "TAC_LE");
            break;
        case TAC_EQ:
            tacHandleRelational(tac, fout, "sete", "TAC_EQ");
            break;
        case TAC_DIF:
            tacHandleRelational(tac, fout, "setne", "TAC_DIF");
            break;
        case TAC_MOVE:
            if (tac->op2 != 0)
            {
                // fprintf(fout,
                //     "## TAC_ARRAY_ASSIGN\n"
                //     "\tmovl\t%s, %%eax\n"
                //     "\tmovl\t%%eax, %d+_%s(%%rip)\n"
                //     "\n",
                //     tac->op2 ? tacCheckOperandType(tac->op2) : "",
                //     tac->op1 ? atoi(tac->op1->text)*4 : 0,
                //     tac->res ? tac->res->text : ""
                // );
                fprintf(fout,
                        "## TAC_ARRAY_ASSIGN\n"
                        "\tmovl\t%s, %%edx\n"
                        "\tmovl\t%s, %%eax\n"
                        "\tmovslq\t%%edx, %%rdx\n"
                        "\tleaq\t0(,%%rdx,4), %%rcx\n"
                        "\tleaq\t_%s(%%rip), %%rdx\n"
                        "\tmovl\t%%eax, (%%rcx,%%rdx)\n"
                        "\tmovl\t$0, %%eax\n"
                        "\n",
                        tac->op1 ? tacCheckOperandType(tac->op1) : "",
                        tac->op2 ? tacCheckOperandType(tac->op2) : "",
                        tac->res ? tac->res->text : "");
            }
            else
            {
                fprintf(fout,
                        "## TAC_MOVE\n"
                        "\tmovl\t%s, %%eax\n"
                        "\tmovl\t%%eax, %s\n"
                        "\n",
                        tac->op1 ? tacCheckOperandType(tac->op1) : "",
                        tac->res ? tacCheckOperandType(tac->res) : "");
            }
            break;
        case TAC_LABEL:
            fprintf(fout, "## TAC_LABEL\n%s:\n", tac->res->text);
            break;
        case TAC_JMP_FALSE:
            fprintf(fout,
                    "## TAC_JMP_FALSE\n"
                    "\tmovl\t_%s(%%rip), %%eax\n"
                    "\ttestl\t%%eax, %%eax\n"
                    "\tje\t%s\n"
                    "\n",
                    tac->op1 ? tac->op1->text : "",
                    tac->res ? tac->res->text : "");
            break;
        case TAC_JMP_ABS:
            fprintf(fout, "## TAC_JMP_ABS\n\tjmp\t%s\n\n", tac->res->text);
            break;
        case TAC_BEGINFUN:
            fprintf(fout,
                    "## TAC_BEGINFUN\n"
                    "\t.section .text\n");

            if (tac->res && !strcmp(tac->res->text, "main"))
            {
                fprintf(fout, "\t.globl\t%s\n"
                              "\t.type\t%s, @function\n"
                              "%s:\n"
                              "\tpushq\t%%rbp\n"
                              "\tmovq\t%%rsp, %%rbp\n",
                        tac->res ? tac->res->text : NULL,
                        tac->res ? tac->res->text : NULL,
                        tac->res ? tac->res->text : NULL);
            }
            else
            {
                fprintf(fout, "\t.globl\t_%s\n"
                              "\t.type\t_%s, @function\n"
                              "_%s:\n"
                              "\tpushq\t%%rbp\n"
                              "\tmovq\t%%rsp, %%rbp\n",
                        tac->res ? tac->res->text : NULL,
                        tac->res ? tac->res->text : NULL,
                        tac->res ? tac->res->text : NULL);
            }

            // pop args
            for (int i = 0; i < tac->res->paramCount; i++)
            {
                fprintf(fout,
                        "## TAC_POP_PARAM\n"
                        "\tmovl %d(%%rbp), %%edx\n"
                        "\tmovl\t%%edx, _%s(%%rip)\n"
                        "\n",
                        16 + 8 * i,
                        tac->res->params[i]);
            }

            break;
        case TAC_ENDFUN:
            fprintf(fout,
                    "## TAC_ENDFUN\n"
                    "\tpopq\t%%rbp\n"
                    "\tret\n"
                    "\n");
            break;
        case TAC_RET:
            fprintf(fout,
                    "## TAC_RET\n"
                    "\tmovl\t%s, %%eax\n"
                    "\n",
                    tac->res ? tacCheckOperandType(tac->res) : "");
            break;
        case TAC_READ:
            fprintf(fout,
                    "## TAC_READ\n"
                    "\tleaq\t_%s(%%rip), %%rsi\n"
                    "\tleaq\t.READ_INT_STR(%%rip), %%rdi\n"
                    "\tcall\t__isoc99_scanf@PLT\n"
                    "\n",
                    tac->res->text);
            break;
        case TAC_ARRAY_ACCESS:
            // fprintf(fout,
            //     "## TAC_ARRAY_ACCESS\n"
            // 	"\tmovl\t%d+_%s(%%rip), %%eax\n"
            //     "\tmovl\t%%eax, _%s(%%rip)\n"
            //     "\n",
            //     tac->op2 ? atoi(tac->op2->text)*4 : 0,
            //     tac->op1 ? tac->op1->text : "",
            //     tac->res ? tac->res->text : ""
            // );
            fprintf(fout,
                    "## TAC_ARRAY_ACCESS\n"
                    "\tmovl %s, %%eax\n"
                    "\tcltq\n"
                    "\tleaq 0(,%%rax,4), %%rdx\n"
                    "\tleaq _%s(%%rip), %%rax\n"
                    "\tmovl (%%rdx,%%rax), %%eax\n"
                    "\tmovl %%eax, %s\n"
                    "\n",
                    tac->op2 ? tacCheckOperandType(tac->op2) : "",
                    tac->op1 ? tac->op1->text : "",
                    tac->res ? tacCheckOperandType(tac->res) : "");
            break;
        case TAC_ARG:
            fprintf(fout,
                    "## TAC_ARG\n"
                    "\tmovl\t%s, %%edx\n"
                    "\tpushq\t%%rdx\n"
                    "\n",
                    tac->res ? tacCheckOperandType(tac->res) : "");
            break;
        case TAC_CALL:
            fprintf(fout,
                    "## TAC_CALL\n"
                    "\tcall\t_%s\n"
                    "\taddq $%d, %%rsp\n"
                    "\tmovl\t%%eax, %s\n"
                    "\n",
                    tac->op1 ? tac->op1->text : "",
                    tac->op1 ? 8 * (tac->op1->paramCount) : 0,
                    tac->res ? tacCheckOperandType(tac->res) : "");
            break;
        case TAC_PRINT_INT:
            fprintf(fout,
                    "## TAC_PRINT_INT\n"
                    "\tmovl\t%s, %%esi\n"
                    "\tleaq\t.PRINT_INT_STR(%%rip), %%rdi\n"
                    "\tcall\tprintf@PLT\n"
                    "\n",
                    tac->res ? tacCheckOperandType(tac->res) : NULL);
            break;
        case TAC_PRINT_STR:
            fprintf(fout,
                    "## TAC_PRINT_STR\n"
                    "\tleaq\t_%s(%%rip), %%rsi\n"
                    "\tleaq\t.PRINT_STRING_STR(%%rip), %%rdi\n"
                    "\tcall\tprintf@PLT\n"
                    "\n",
                    tac->res ? tac->res->stringLabel : "");
            break;
        }
    }

    // Hash Table
    printASM(fout);

    fclose(fout);
}

const char *tacCheckOperandType(HASH_NODE *node)
{
    char *buffer = (char *)calloc(32, sizeof(char *));
    if (node->type == SYMBOL_VARIABLE)
    {
        strcat(buffer, "_");
        strcat(buffer, node->text);
        strcat(buffer, "(%rip)");
    }
    if (node->type == SYMBOL_LIT_INTEGER)
    {
        strcat(buffer, "$");
        strcat(buffer, node->text);
    }
    return buffer;
}

void tacHandleRelational(TAC_NODE *tac, FILE *fout, const char *op, const char *comment)
{
    fprintf(fout,
            "## %s\n"
            "\tmovl\t%s, %%edx\n"
            "\tmovl\t%s, %%eax\n"
            "\tcmpl\t%%eax, %%edx\n"
            "\t%s\t%%al\n"
            "\tmovzbl\t%%al, %%eax\n"
            "\tmovl\t%%eax, _%s(%%rip)\n"
            "\n",
            comment,
            tac->op1 ? tacCheckOperandType(tac->op1) : "",
            tac->op2 ? tacCheckOperandType(tac->op2) : "",
            op,
            tac->res ? tac->res->text : "");
}
