#include "hash.h"

HASH_NODE *Table[HASH_SZ];

void hashInit()
{
    int i;
    for (i = 0; i < HASH_SZ; i++)
        Table[i] = 0;
}

int hashAddress(char *text)
{
    int address = 1;
    size_t i;
    for (i = 0; i < strlen(text); i++)
        address = (address * text[i]) % HASH_SZ + 1;
    return address - 1;
}

HASH_NODE *hashFind(char *text)
{
    HASH_NODE *node;
    int address = hashAddress(text);
    for (node = Table[address]; node; node = node->next)
        if (strcmp(node->text, text) == 0)
            return node;
    return 0;
}

HASH_NODE *hashInsert(char *text, int type)
{
    HASH_NODE *newNode;
    int address = hashAddress(text);
    if ((newNode = hashFind(text)) != 0)
        return newNode;

    newNode = (HASH_NODE *)calloc(1, sizeof(HASH_NODE));

    if (type == SYMBOL_LIT_STRING)
        makeString(newNode);

    newNode->type = type;
    newNode->initValue[0] = 0;
    newNode->initValue[1] = 0;
    newNode->text = (char *)calloc(strlen(text) + 1, sizeof(char));
    strcpy(newNode->text, text);
    newNode->next = Table[address];
    Table[address] = newNode;
    return newNode;
}

void hashPrint()
{
    int i;
    HASH_NODE *node;
    for (i = 0; i < HASH_SZ; i++)
    {
        for (node = Table[i]; node; node = node->next)
        {
            printf("Table[%-3d] :   Nature => %-3d | Datatype => %-3d | Value => %-10s | Params => %-3d\n", i, node->type, node->datatype, node->text, node->paramCount);
        }
    }
    printf("\n");
}

int hashCheckUndeclared()
{
    int undeclared = 0;
    for (int i = 0; i < HASH_SZ; i++)
    {
        for (HASH_NODE *node = Table[i]; node; node = node->next)
        {
            if (node->type == SYMBOL_IDENTIFIER)
            {
                fprintf(stderr, "Semantic Error: Identifier %s undeclared\n", node->text);
                undeclared++;
            }
        }
    }
    return undeclared;
}

HASH_NODE *makeTemp()
{
    static int serial = 0;
    char buffer[256] = "";
    sprintf(buffer, "TEMP__%d", serial++);
    return hashInsert(buffer, SYMBOL_VARIABLE);
}

HASH_NODE *makeLabel()
{
    static int serial = 0;
    char buffer[256] = "";
    sprintf(buffer, "LABEL__%d", serial++);
    return hashInsert(buffer, SYMBOL_JMP_LABEL);
}

void makeString(HASH_NODE *node)
{
    static int serial = 0;
    node->stringLabel = (char *)malloc(sizeof(char) * 16);
    sprintf(node->stringLabel, "STRING__%d", serial++);
}

void printASM(FILE *fout)
{
    fprintf(fout, "\t.section .data\n");
    int i;
    HASH_NODE *node;
    for (i = 0; i < HASH_SZ; i++)
    {
        for (node = Table[i]; node; node = node->next)
        {
            if (node->type == SYMBOL_VARIABLE)
            {
                if (node->datatype == DATATYPE_FLOAT)
                    fprintf(fout, "_%s:\t.long\t%d\n", node->text, node->initValue[0] / node->initValue[1]);
                else
                    fprintf(fout, "_%s:\t.long\t%d\n", node->text, node->initValue[0]);
            }
            if (node->type == SYMBOL_LIT_STRING)
            {
                fprintf(fout, "_%s:\t.string\t%s\n", node->stringLabel, node->text);
            }
            if (node->type == SYMBOL_VECTOR)
            {
                fprintf(fout, "_%s:\n", node->text);
                for (int i = 0; i < node->paramCount; i++)
                {
                    fprintf(fout, "\t.long\t%d\n", node->initValue[i]);
                }
            }
        }
    }
}
