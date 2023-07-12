%{

#include "hash.h"
#include "ast.h"
#include "semantic.h"
#include "tacs.h"

extern int yyerror();
extern int yylex();
extern int getLineNumber();

int bSyntaxError = 0;

AST_NODE* AST;
%}

%union
{
    AST_NODE* ast;
    HASH_NODE* symbol;
}

%token KW_CHAR           
%token KW_INT          
%token KW_FLOAT        

%token KW_IF           
%token KW_THEN        
%token KW_ELSE        
%token KW_WHILE        
%token KW_GOTO        
%token KW_READ         
%token KW_PRINT        
%token KW_RETURN       

%token OPERATOR_LE  
%token OPERATOR_GE   
%token OPERATOR_EQ    
%token OPERATOR_DIF 

%token<symbol> TK_IDENTIFIER

%token<symbol> LIT_INTEGER   
%token<symbol> LIT_CHAR        
%token<symbol> LIT_STRING        

%token TOKEN_ERROR

%type<ast> value
%type<ast> expr
%type<ast> decList
%type<ast> type
%type<ast> array
%type<ast> variableDec
%type<ast> functionDec
%type<ast> param
%type<ast> paramList
%type<ast> body
%type<ast> cmdList
%type<ast> cmd
%type<ast> variable
%type<ast> assignment
%type<ast> functionCall
%type<ast> callParam
%type<ast> print
%type<ast> printParam
%type<ast> block
%type<ast> if
%type<ast> while
%type<ast> return
%type<ast> label
%type<ast> goto

%left '<' '>' OPERATOR_GE OPERATOR_LE OPERATOR_EQ OPERATOR_DIF
%left '+' '-'
%left '*' '/'

%%

program: decList { 
    AST = $1;
    TAC_NODE* code;
    checkAndSetDeclarations(AST);
    checkUndeclared();
    checkOperands(AST);
    checkAndSetNature(AST);
    checkBool(AST);
    checkVectorIndex(AST);
    checkVectorSize(AST);
    checkFuncParams(AST);

    if(bSyntaxError){
        exit(3);
    }

    code = generateCode(AST);
#ifdef PRINT_TAC
    printf("TAC:\n\n");
    tacPrintBackwards(code);
#endif
    code = tacReverse(code);
    generateASM(code);

    }
    ;

decList: 
      variableDec ';' decList   { $$ = astCreate(AST_DECL_LST, NULL, $1, $3, 0, 0); }
    | functionDec decList       { $$ = astCreate(AST_DECL_LST, NULL, $1, $2, 0, 0); }
    |                           { $$ = NULL; }
    ;

type: 
      KW_INT    { $$ = astCreate(AST_INT, NULL, 0, 0, 0, 0); }
    | KW_CHAR   { $$ = astCreate(AST_CHAR, NULL, 0, 0, 0, 0); }
    ;

value: 
      LIT_INTEGER   { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    | LIT_CHAR      { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    ; 

array: 
      value array   { $$ = astCreate(AST_ARRAY, NULL, $1, $2, 0, 0); }
    | value         { $$ = astCreate(AST_ARRAY, NULL, $1, 0, 0, 0); }
    ;

variableDec:
    // AST_VAR_DECL_01
      type TK_IDENTIFIER ':' value                              { $$ = astCreate(AST_VAR_DECL_01, NULL, $1, astCreate(AST_SYMBOL, $2, 0, 0, 0, 0), $4, 0); }
    | type TK_IDENTIFIER error value                            { $$ = astCreate(TOKEN_ERROR, NULL, 0, 0, 0, 0); fprintf(stderr, "Missing \":\" in variable declaration.\n"); }
    // AST_VAR_DECL_02
    | type TK_IDENTIFIER '[' LIT_INTEGER ']'                    { $$ = astCreate(AST_VAR_DECL_02, NULL, $1, astCreate(AST_SYMBOL, $2, 0, 0, 0, 0), astCreate(AST_SYMBOL, $4, 0, 0, 0, 0), 0); }
    | KW_FLOAT TK_IDENTIFIER '[' LIT_INTEGER ']'                { $$ = astCreate(AST_VAR_DECL_02, NULL, astCreate(AST_FLOAT, NULL, 0, 0, 0, 0), astCreate(AST_SYMBOL, $2, 0, 0, 0, 0), astCreate(AST_SYMBOL, $4, 0, 0, 0, 0), 0); }
    // AST_VAR_DECL_03
    | type TK_IDENTIFIER '[' LIT_INTEGER ']' ':' array          { $$ = astCreate(AST_VAR_DECL_03, NULL, $1, astCreate(AST_SYMBOL, $2, 0, 0, 0, 0), astCreate(AST_SYMBOL, $4, 0, 0, 0, 0), $7); }
    | KW_FLOAT TK_IDENTIFIER '[' LIT_INTEGER ']' ':' array      { $$ = astCreate(AST_VAR_DECL_03, NULL, astCreate(AST_FLOAT, NULL, 0, 0, 0, 0), astCreate(AST_SYMBOL, $2, 0, 0, 0, 0), astCreate(AST_SYMBOL, $4, 0, 0, 0, 0), $7); }
    | KW_FLOAT TK_IDENTIFIER '[' LIT_INTEGER ']' error array    { $$ = astCreate(TOKEN_ERROR, NULL, 0, 0, 0, 0); fprintf(stderr, "Missing \":\" in array declaration.\n"); }
    | type TK_IDENTIFIER '[' LIT_INTEGER ']' error array        { $$ = astCreate(TOKEN_ERROR, NULL, 0, 0, 0, 0); fprintf(stderr, "Missing \":\" in array declaration.\n"); }
    //AST_VAR_DECL_04
    | KW_FLOAT TK_IDENTIFIER ':' LIT_INTEGER '/' LIT_INTEGER    { $$ = astCreate(AST_VAR_DECL_04, NULL, astCreate(AST_FLOAT, NULL, 0, 0, 0, 0), astCreate(AST_SYMBOL, $2, 0, 0, 0, 0), astCreate(AST_SYMBOL, $4, 0, 0, 0, 0), astCreate(AST_SYMBOL, $6, 0, 0, 0, 0)); }
    | KW_FLOAT TK_IDENTIFIER error LIT_INTEGER '/' LIT_INTEGER  { $$ = astCreate(TOKEN_ERROR, NULL, 0, 0, 0, 0); fprintf(stderr, "Missing \":\" in variable declaration.\n"); }
    | KW_FLOAT TK_IDENTIFIER ':' LIT_INTEGER error LIT_INTEGER  { $$ = astCreate(TOKEN_ERROR, NULL, 0, 0, 0, 0); fprintf(stderr, "Value for initialization of variable of type 'float' must be in the format \"INTEGER / INTEGER\".\n"); }
    ;

functionDec: 
      type TK_IDENTIFIER param block        { $$ = astCreate(AST_FUNC_DECL, NULL, $1, astCreate(AST_SYMBOL, $2, 0, 0, 0, 0), $3, $4); }
    | KW_FLOAT TK_IDENTIFIER param block    { $$ = astCreate(AST_FUNC_DECL, NULL, astCreate(AST_FLOAT, NULL, 0, 0, 0, 0), astCreate(AST_SYMBOL, $2, 0, 0, 0, 0), $3, $4); }
    ;

param: 
      '(' ')'            { $$ = NULL; }
    | '(' paramList ')'  { $$ = $2; }
    ;

paramList: 
      type TK_IDENTIFIER ',' paramList      { $$ = astCreate(AST_PARAM_LST, NULL, $1, astCreate(AST_SYMBOL, $2, 0, 0, 0, 0), $4, 0); }
    | type TK_IDENTIFIER                    { $$ = astCreate(AST_PARAM_LST, NULL, $1, astCreate(AST_SYMBOL, $2, 0, 0, 0, 0), 0, 0); }
    | KW_FLOAT TK_IDENTIFIER ',' paramList  { $$ = astCreate(AST_PARAM_LST, NULL, astCreate(AST_FLOAT, NULL, 0, 0, 0, 0), astCreate(AST_SYMBOL, $2, 0, 0, 0, 0), $4, 0); }
    | KW_FLOAT TK_IDENTIFIER                { $$ = astCreate(AST_PARAM_LST, NULL, astCreate(AST_FLOAT, NULL, 0, 0, 0, 0), astCreate(AST_SYMBOL, $2, 0, 0, 0, 0), 0, 0); }
    | type TK_IDENTIFIER error paramList    { $$ = astCreate(TOKEN_ERROR, NULL, 0, 0, 0, 0); fprintf(stderr, "Function parameters must be separated by ','.\n"); }
    | KW_FLOAT TK_IDENTIFIER error paramList{ $$ = astCreate(TOKEN_ERROR, NULL, 0, 0, 0, 0); fprintf(stderr, "Function parameters must be separated by ','.\n"); }
    ;

body: '{' cmdList '}' { $$ = astCreate(AST_BODY, NULL, $2, 0, 0, 0); }
    ;

cmdList: 
      cmd ';' cmdList   { $$ = astCreate(AST_CMD_LST_01, NULL, $1, $3, 0, 0); }
    | label cmdList     { $$ = astCreate(AST_CMD_LST_02, NULL, $1, $2, 0, 0); }
    | body ';' cmdList  { $$ = astCreate(AST_CMD_LST_01, NULL, $1, $3, 0, 0); }
    |                   { $$ = NULL; }
    ;

cmd: 
      assignment    { $$ = $1; }
    | print         { $$ = $1; }
    | while         { $$ = $1; }
    | return        { $$ = $1; }
    | if            { $$ = $1; }      
    | goto          { $$ = $1; }
    |               { $$ = NULL; }
    ;

variable: 
      TK_IDENTIFIER                 { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    | TK_IDENTIFIER '[' expr ']'    { $$ = astCreate(AST_ARRAY_ACCESS, NULL, astCreate(AST_SYMBOL, $1, 0, 0, 0, 0), $3, 0, 0); } 
    ;

assignment: 
      TK_IDENTIFIER '=' expr                { $$ = astCreate(AST_ASSIGN, NULL, astCreate(AST_SYMBOL, $1, 0, 0, 0, 0), $3, 0, 0); }
    | TK_IDENTIFIER '[' expr ']' '=' expr   { $$ = astCreate(AST_ARRAY_ASSIGN, NULL, astCreate(AST_SYMBOL, $1, 0, 0, 0, 0), $3, $6, 0); }
    | TK_IDENTIFIER '=' error               { $$ = 0; fprintf(stderr, "Incorrect use of assignment operator.\n"); }
    ;

expr: 
      expr '+' expr             { $$ = astCreate(AST_ADD, NULL, $1, $3, 0, 0); }
    | expr '-' expr             { $$ = astCreate(AST_SUB, NULL, $1, $3, 0, 0); }
    | expr '*' expr             { $$ = astCreate(AST_MUL, NULL, $1, $3, 0, 0); }
    | expr '/' expr             { $$ = astCreate(AST_DIV, NULL, $1, $3, 0, 0); }
    | expr '>' expr             { $$ = astCreate(AST_G, NULL, $1, $3, 0, 0); }
    | expr '<' expr             { $$ = astCreate(AST_L, NULL, $1, $3, 0, 0); }
    | expr OPERATOR_LE expr     { $$ = astCreate(AST_LE, NULL, $1, $3, 0, 0); }
    | expr OPERATOR_GE expr     { $$ = astCreate(AST_GE, NULL, $1, $3, 0, 0); }
    | expr OPERATOR_DIF expr    { $$ = astCreate(AST_DIF, NULL, $1, $3, 0, 0); }
    | expr OPERATOR_EQ expr     { $$ = astCreate(AST_EQ, NULL, $1, $3, 0, 0); }
    | '(' expr ')'              { $$ = astCreate(AST_EXPR_PARENTHESIS, NULL, $2, 0, 0, 0); }
    | variable                  { $$ = $1; }
    | functionCall              { $$ = $1; }
    | value                     { $$ = $1; }
    | KW_READ                   { $$ = astCreate(AST_READ, NULL, 0, 0, 0, 0); } 
    ;

functionCall: 
      TK_IDENTIFIER '(' callParam ')'   { $$ = astCreate(AST_FUNC_CALL, NULL, astCreate(AST_SYMBOL, $1, 0, 0, 0, 0), $3, 0, 0); }
    | TK_IDENTIFIER '(' ')'             { $$ = astCreate(AST_FUNC_CALL, NULL, astCreate(AST_SYMBOL, $1, 0, 0, 0, 0), 0, 0, 0); }
    ;

callParam: 
      expr ',' callParam    { $$ = astCreate(AST_PARAM_CALL, NULL, $1, $3, 0, 0); }
    | expr                  { $$ = astCreate(AST_PARAM_CALL, NULL, $1, 0, 0, 0); }
    ;

print: KW_PRINT printParam  { $$ = astCreate(AST_PRINT, NULL, $2, 0, 0, 0); }
    ;

printParam: 
      LIT_STRING ',' printParam { $$ = astCreate(AST_PARAM_PRINT_01, NULL, astCreate(AST_STRING, $1, 0, 0, 0, 0), $3, 0, 0); }
    | LIT_STRING                { $$ = astCreate(AST_PARAM_PRINT_01, NULL, astCreate(AST_STRING, $1, 0, 0, 0, 0), 0, 0, 0); }
    | expr ',' printParam       { $$ = astCreate(AST_PARAM_PRINT_02, NULL, $1, $3, 0, 0); }
    | expr                      { $$ = astCreate(AST_PARAM_PRINT_02, NULL, $1, 0, 0, 0); }
    ;

block: 
      body  { $$ = $1; }
    | cmd   { $$ = $1; }
    ;

if: 
      KW_IF expr KW_THEN block                  { $$ = astCreate(AST_IF, NULL, $2, $4, 0, 0); }
    | KW_IF expr KW_THEN block KW_ELSE block    { $$ = astCreate(AST_IF, NULL, $2, $4, $6, 0); }
    ;

while: KW_WHILE expr block { $$ = astCreate(AST_WHILE, NULL, $2, $3, 0, 0); }
    ;

return: KW_RETURN expr { $$ = astCreate(AST_RETURN, NULL, $2, 0, 0, 0); }
    ;

label: TK_IDENTIFIER ':' { $$ = astCreate(AST_LABEL, NULL, astCreate(AST_SYMBOL, $1, 0, 0, 0, 0), 0, 0, 0); }
    ;

goto: KW_GOTO TK_IDENTIFIER { $$ = astCreate(AST_GOTO, NULL, astCreate(AST_SYMBOL, $2, 0, 0, 0, 0), 0, 0, 0); }
    ;

%%

int yyerror()
{
    fprintf(stderr, "Syntax Error: Line [%d]: ", getLineNumber());
    /* exit(3); */
    bSyntaxError = 1;
    return 0;
}
