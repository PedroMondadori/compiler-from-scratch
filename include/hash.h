#ifndef HASH_HEADER
#define HASH_HEADER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASH_SZ 997

typedef enum
{
    SYMBOL_IDENTIFIER,
    SYMBOL_LIT_CHAR,
    SYMBOL_LIT_INTEGER,
    SYMBOL_LIT_STRING,
    SYMBOL_VARIABLE,
    SYMBOL_VECTOR,
    SYMBOL_FUNC,
    SYMBOL_LABEL,
    SYMBOL_JMP_LABEL,
} SYMBOLType;

typedef enum
{
    DATATYPE_CHAR,
    DATATYPE_INT,
    DATATYPE_STRING,
    DATATYPE_FLOAT,
    DATATYPE_BOOL,
} DATATYPEType;

typedef struct Node
{
    int type;
    char *text;
    struct Node *next;
    int initValue[512];
    int datatype;
    int paramCount;
    char *params[32];
    char *stringLabel;
} HASH_NODE;

void hashInit();
void hashPrint();
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type);
int hashCheckUndeclared();
HASH_NODE *makeTemp();
HASH_NODE *makeLabel();
void printASM(FILE *fout);
void makeString();

#endif
