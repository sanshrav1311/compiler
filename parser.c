#include <stdio.h>
#include <stdbool.h>
#define GRAMMER_TABLE_SIZE 47

typedef enum NONTERMINAL {program, mainFunction, otherFunctions, function, input_par, output_par, parameter_list, dataType, primitiveDatatype, constructedDatatype, remaining_list, stmts, typeDefinitions, typeDefinition, fieldDefinitions, fieldDefinition, moreFields, declarations, delaration, globar_or_not, otherStmts, stmt, assignmentStmt, singleOrRecId, B,funCallStmt, outputParameters, inputParameters, iterativeStmt, conditionalStmt, ioStmt, aritmeticExpression, addORsub, mulORdiv, brackets, MULDIV, ADDSUB, booleanExpression, var, logicalOp, relationalOp, returnStmt, optionalReturn, idList, more_ids, definetypestmt, A} NONTERMINAL;
typedef enum TOKENS {TK_ERROR, TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM, TK_FUNID, TK_RUID, TK_WITH, TK_PARAMETERS, TK_END, TK_WHILE, TK_UNION, TK_ENDUNION, TK_DEFINETYPE, TK_AS, TK_TYPE, TK_MAIN, TK_GLOBAL, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_INPUT, TK_OUTPUT, TK_INT, TK_REAL, TK_COMMA, TK_SEM, TK_COLON, TK_DOT, TK_ENDWHILE, TK_OP, TK_CL, TK_IF, TK_THEN, TK_ENDIF, TK_READ, TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV, TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE, TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_EQ, TK_GT, TK_GE, TK_NE} TOKENS;

void intialiseGrammer(){
    //<constructedDatatype> ===> TK_RECORD TK_RUID | TK_UNION TK_RUID | TK_RUID
    GrammerElement* tempG1 = createGrammerElement(true, (TOKENS)TK_RECORD);
    GrammerElement* tempG2 = createGrammerElement(true, (TOKENS)TK_RUID);
    GrammerElement* tempG3 = createGrammerElement(true, (TOKENS)TK_UNION);
    GrammerElement* tempG4 = createGrammerElement(true, (TOKENS)TK_RUID);
    GrammerElement* tempG5 = createGrammerElement(true, (TOKENS)TK_RUID);
    RHS* tempR1 = createRHSNode();
    RHS* tempR2 = createRHSNode();
    RHS* tempR3 = createRHSNode();
    insertGrammerElement(tempR1, tempG1);
    insertGrammerElement(tempR1, tempG2);
    insertGrammerElement(tempR2, tempG3);
    insertGrammerElement(tempR2, tempG4);
    insertGrammerElement(tempR3, tempG5);
    RHSHead* tempRH = createRHSHead();
    insertRHS(tempRH, tempR1);
    insertRHS(tempRH, tempR2);
    insertRHS(tempRH, tempR3);
    insertInHashTable(program, tempRH);
}

int hash(NONTERMINAL nt){
    return nt % GRAMMER_TABLE_SIZE;
}


typedef struct GrammerElement{
    bool isTerminal;
    union {
        NONTERMINAL NonTerminal;
        TOKENS Terminal;
    } TNT; // TNT ==> Terminal Non - Terminal
    GrammerElement* next;
} GrammerElement;

// typedef struct GrammerElementHead{
//     int count;
//     GrammerElement* first;
//     GrammerElement* last;
// } GrammerElementHead;

typedef struct RHSHead {
    int count;
    RHS* first;
    RHS* last;
} RHSHead;

typedef struct RHS {
    // GrammerElementHead *grammerHead;
    int count;
    GrammerElement* first;
    GrammerElement* last;
    RHS* next;
} RHS;

RHSHead* createRHSHead() {
    RHSHead* newNode = (RHSHead*)malloc(sizeof(RHSHead));
    newNode->count = 0;
    newNode->last = NULL;
    newNode->first = NULL;
    return newNode;
}

RHS* createRHSNode() {
    RHS* newNode = (RHS*)malloc(sizeof(RHS));
    // newNode->grammerHead = grammerElementHead;
    newNode->count = 0;
    newNode->first = NULL;
    newNode->last = NULL;
    newNode->next = NULL;
    return newNode;
}

// GrammerElementHead* createGrammerElementHead() {
//     GrammerElementHead* newNode = (GrammerElementHead*)malloc(sizeof(GrammerElementHead));
//     newNode->count = 0;
//     newNode->first = NULL;
//     newNode->last = NULL;
//     return newNode;
// }

GrammerElement* createGrammerElement(bool terminalORnot, int enumVal) {
    GrammerElement* newNode = (GrammerElement*)malloc(sizeof(GrammerElement));
    newNode->next = NULL;
    newNode->isTerminal = terminalORnot;
    if(terminalORnot) newNode->TNT.Terminal = enumVal;
    else newNode->TNT.NonTerminal = enumVal;
    return newNode;
}

void insertRHS(RHSHead* head, RHS* rhs) {
    // if (head == NULL || rhs == NULL) return;
    if (head->first == NULL) {
        head->first = rhs;
    } else {
        head->last->next = rhs;
    }
    head->last = rhs;
    head->count++;
}
void insertGrammerElement(RHS* head, GrammerElement* ge) {
    // if (head == NULL || rhs == NULL) return;
    if (head->first == NULL) {
        head->first = ge;
    } else {
        head->last->next = ge;
    }
    head->last = ge;
    head->count++;
}

RHSHead* LHSmap[GRAMMER_TABLE_SIZE] = { NULL };

void insertInHashTable(NONTERMINAL nt, RHSHead* rhsHead){
    int index = hash(nt);
    LHSmap[index] = rhsHead;
}

int main(int argc, char const *argv[]){
    
    return 0;
}
