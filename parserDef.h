#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define GRAMMER_TABLE_SIZE 53
#define TERMINALS_SIZE 61

typedef enum NONTERMINAL {program, mainFunction, otherFunctions, function, input_par, output_par, parameter_list, remaining_list, declaration, dataType, primitiveDatatype, constructedDatatype, stmts, typeDefinitions, actualOrRedefined, typeDefinition, fieldDefinitions, fieldDefinition, fieldType, moreFields, declarations, global_or_not, iterativeStmt, elsePart, otherStmts, stmt, conditionalStmt, assignmentStmt, ioStmt, term, termPrime, factor, var, arithmeticExpression, expPrime, singleOrRecId, option_single_constructed, oneExpansion, moreExpansions, funCallStmt, outputParameters, inputParameters, lowPrecedenceOperators, highPrecedenceOperators, booleanExpression, logicalOp, relationalOp, returnStmt, optionalReturn, idList, more_ids, definetypestmt, A} NONTERMINAL;
typedef enum TOKENS {TK_ERROR, TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM, TK_FUNID, TK_RUID, TK_WITH, TK_PARAMETERS, TK_END, TK_WHILE, TK_UNION, TK_ENDUNION, TK_DEFINETYPE, TK_AS, TK_TYPE, TK_MAIN, TK_GLOBAL, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_INPUT, TK_OUTPUT, TK_INT, TK_REAL, TK_COMMA, TK_SEM, TK_COLON, TK_DOT, TK_ENDWHILE, TK_OP, TK_CL, TK_IF, TK_THEN, TK_ENDIF, TK_READ, TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV, TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE, TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_EQ, TK_GT, TK_GE, TK_NE, dollar, nf, epsilon} TOKENS;

typedef struct GrammerElement{
    bool isTerminal;
    union {
        NONTERMINAL NonTerminal;
        TOKENS Terminal;
    } TNT; // TNT ==> Terminal Non - Terminal
    struct GrammerElement* next;
} GrammerElement;

typedef struct RHS {
    int count;
    GrammerElement* first;
    GrammerElement* last;
    struct RHS* next;
} RHS;

typedef struct RHSHead {
    int count;
    RHS* first;
    RHS* last;
} RHSHead;

typedef struct TerminalNode {
    int terminal;
    struct TerminalNode* next;
} TerminalNode;

typedef struct TreeNode {
    GrammerElement* ge;
    struct TreeNode* firstChild;
    struct TreeNode* nextSibling;
} TreeNode;

typedef struct ParseStackElement {
    GrammerElement* ge;
    struct ParseStackElement* next;
    TreeNode* nodeReference;
} ParseStackElement;

typedef struct ParseStack {
    int count;
    struct ParseStackElement* first;
} ParseStack;
