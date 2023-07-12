#ifndef TACS_HEADER
#define TACS_HEADER

#include "hash.h"
#include "ast.h"

typedef enum
{
    TAC_SYMBOL,
    TAC_ADD,
    TAC_SUB,
    TAC_MUL,
    TAC_DIV,
    TAC_G,
    TAC_L,
    TAC_GE,
    TAC_LE,
    TAC_EQ,
    TAC_DIF,
    TAC_MOVE,
    TAC_LABEL,
    TAC_JMP_FALSE,
    TAC_JMP_ABS,
    TAC_BEGINFUN,
    TAC_ENDFUN,
    TAC_RET,
    TAC_READ,
    TAC_ARRAY_ACCESS,
    TAC_ARG,
    TAC_CALL,
    TAC_PRINT_INT,
    TAC_PRINT_STR,
    TAC_PRINT,
    TAC_STRING,
} TACType;

typedef struct tacNode
{
    int type;
    HASH_NODE *res;
    HASH_NODE *op1;
    HASH_NODE *op2;
    struct tacNode *prev;
    struct tacNode *next;
} TAC_NODE;

TAC_NODE *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2);
TAC_NODE *tacJoin(TAC_NODE *l1, TAC_NODE *l2);
void tacPrint(TAC_NODE *tac);
void tacPrintBackwards(TAC_NODE *tac);

// CODE GENERATION

TAC_NODE *generateCode(AST_NODE *node);

// ASM GENERATION

TAC_NODE *tacReverse(TAC_NODE *tac);
void generateASM(TAC_NODE *first);
const char *tacCheckOperandType(HASH_NODE *node);
void tacHandleRelational(TAC_NODE *tac, FILE *fout, const char *op, const char *comment);

#endif
