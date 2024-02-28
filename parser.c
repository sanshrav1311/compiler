#include <stdio.h>
#include <stdbool.h>
#define GRAMMER_TABLE_SIZE 47

int hash(NONTERMINAL nt){
    return nt % GRAMMER_TABLE_SIZE;
}

typedef enum NONTERMINAL {program, main_function, other_functions, function, input_par, output_par, parameter_list, dataType, primitiveDatatype, constructedDatatype, remaining_list, stmts, typeDefinitions, typeDefinition, fieldDefinitions, fieldDefinition, moreFields, declarations, delaration, globar_or_not, otherStmts, stmt, assignmentStmt, singleOrRecId, B,funCallStmt, outputParameters, inputParameters, iterativeStmt, conditionalStmt, ioStmt, aritmeticExpression, addORsub, mulORdiv, brackets, MULDIV, ADDSUB, booleanExpression, var, logicalOp, relationalOp, returnStmt, optionalReturn, idList, more_ids, definetypestmt, A} NONTERMINAL;
typedef enum TOKENS {TK_ERROR, TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM, TK_FUNID, TK_RUID, TK_WITH, TK_PARAMETERS, TK_END, TK_WHILE, TK_UNION, TK_ENDUNION, TK_DEFINETYPE, TK_AS, TK_TYPE, TK_MAIN, TK_GLOBAL, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_INPUT, TK_OUTPUT, TK_INT, TK_REAL, TK_COMMA, TK_SEM, TK_COLON, TK_DOT, TK_ENDWHILE, TK_OP, TK_CL, TK_IF, TK_THEN, TK_ENDIF, TK_READ, TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV, TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE, TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_EQ, TK_GT, TK_GE, TK_NE} TOKENS;

typedef struct GrammerElement{
    bool isTerminal;
    union {
        NONTERMINAL NonTerminal;
        TOKENS Terminal;
    } TNT; // TNT ==> Terminal Non - Terminal
    GrammerElement* next;
} GrammerElement;

typedef struct GrammerElementHead{
    int count;
    GrammerElement* first;
    GrammerElement* last;
} GrammerElementHead;

typedef struct RHSHead {
    int count;
    RHS* first;
    RHS* last;
} RHSHead;

typedef struct RHS {
    NONTERMINAL nonTerminal;
    GrammerElementHead *grammerHead;
    RHS* next;
} RHS;

RHSHead* LHSmap[GRAMMER_TABLE_SIZE];

RHSHead* createRHSHead() {
    RHSHead* newNode = (RHSHead*)malloc(sizeof(RHSHead));
    newNode->count = 0;
    newNode->last = NULL;
    newNode->first = NULL;
    return newNode;
}

RHS* createRHSNode(NONTERMINAL nt, GrammerElementHead* grammerElementHead) {
    RHS* newNode = (RHS*)malloc(sizeof(RHS));
    newNode->nonTerminal = nt;
    newNode->grammerHead = grammerElementHead;
    newNode->next = NULL;
    return newNode;
}

GrammerElementHead* createGrammerElementHead() {
    GrammerElementHead* newNode = (GrammerElementHead*)malloc(sizeof(GrammerElementHead));
    newNode->count = 0;
    newNode->first = NULL;
    newNode->last = NULL;
    return newNode;
}

GrammerElement* createGrammerElement(bool terminalORnot, int enumVal) {
    GrammerElement* newNode = (GrammerElement*)malloc(sizeof(GrammerElement));
    newNode->next = NULL;
    newNode->isTerminal = terminalORnot;
    if(terminalORnot) newNode->TNT.Terminal = enumVal;
    else newNode->TNT.NonTerminal = enumVal;
    return newNode;
}

int main(int argc, char const *argv[]){

    return 0;
}
