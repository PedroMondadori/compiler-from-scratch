%option nounput
%option noinput

%{

#include "hash.h"
#include "ast.h"
#include "y.tab.h"

#define TRUE 1
#define FALSE 0

int yyerror();

int bRunning = TRUE;
int nLineNumber = 1;

%}

%x COMMENT

%%

char                        { return KW_CHAR; }
int                         { return KW_INT; }
float                       { return KW_FLOAT; }
if                          { return KW_IF; }
then                        { return KW_THEN; }
else                        { return KW_ELSE; }
while                       { return KW_WHILE; }
goto                        { return KW_GOTO; }
read                        { return KW_READ; }
print                       { return KW_PRINT; }
return                      { return KW_RETURN; }
"<="                        { return OPERATOR_LE; }
">="                        { return OPERATOR_GE; }
"=="                        { return OPERATOR_EQ; }
"!="                        { return OPERATOR_DIF; }
[-,;:\(\)\[\]\{\}\+\*/<>=]  { return yytext[0]; }
[-_a-z]+                    { yylval.symbol = hashInsert(yytext, SYMBOL_IDENTIFIER); return TK_IDENTIFIER; }
[0-9]+                      { yylval.symbol = hashInsert(yytext, SYMBOL_LIT_INTEGER); return LIT_INTEGER; }
\'.\'                       { yylval.symbol = hashInsert(yytext, SYMBOL_LIT_CHAR); return LIT_CHAR; }
\"([^"\\\n]|\\.)*\"         { yylval.symbol = hashInsert(yytext, SYMBOL_LIT_STRING); return LIT_STRING; }
"\n"                        { nLineNumber++; }
[ \t]
"\\\\".*
"\\*"                       { BEGIN(COMMENT); }
.                           { return TOKEN_ERROR; }

<COMMENT>"*\\"              { BEGIN(INITIAL); }
<COMMENT>"\n"               { nLineNumber++; }
<COMMENT>.             

%%

void initMe(void);
int getLineNumber(void);
int isRunning(void);

void initMe(void){
    hashInit();
}

int getLineNumber(void){
    return nLineNumber;
}

int isRunning(void){
    return bRunning;
}

int yywrap() {
    bRunning = FALSE;
    return 1;
}
