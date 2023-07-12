#include <stdio.h>
#include <string.h>

#include "hash.h"
#include "ast.h"

extern FILE* yyin;
extern AST_NODE* AST;
extern int yyparse();
extern int SEMANTIC_ERRORS;

int main(int argc, char** argv) {
    if (argc != 3){
        fprintf(stderr, "ERROR: Incorrect number of arguments! Expected 3, but received %d\n", argc);
        exit(1);
    }

    if (strcmp(strrchr(argv[1], '.'), ".cfs")) {
        fprintf(stderr, "ERROR: Invalid file extension for %s\n", argv[1]);
        exit(5);
    }

    if((yyin = fopen(argv[1], "r")) != NULL){
        yyparse();
    }
    else{
        exit(2);
    }

#ifdef PRINT_AST
    printf("\nAST:\n\n");
    astPrint(AST, 0);
#endif
#ifdef PRINT_HASH
    printf("\nHASH:\n\n");
    hashPrint();
#endif

    astDecompile(AST, argv[2]);

    if(SEMANTIC_ERRORS > 0){
        fprintf(stderr, "ERROR: Compilation Failed...\n");
        exit(4);
    }

    fprintf(stderr, "SUCCESS: Compilation finished!\n");
    exit(0);
}
