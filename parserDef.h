/*
Group Number 16
Sanshrav Arora 2021A7PS2690P
Ajey Malik 2021A7PS2542P
Nachiketh S Shastry 2021A7PS2686P
Rikhil Gupta 2021A7PS0533P
Rishabh Sahni 2021A7PS1630P
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#define GRAMMER_TABLE_SIZE 53
#define TERMINALS_SIZE 61

// typedef enum NONTERMINAL {program, mainFunction, otherFunctions, function, input_par, output_par, parameter_list, remaining_list, declaration, dataType, primitiveDatatype, constructedDatatype, stmts, typeDefinitions, actualOrRedefined, typeDefinition, fieldDefinitions, fieldDefinition, fieldType, moreFields, declarations, global_or_not, iterativeStmt, elsePart, otherStmts, stmt, conditionalStmt, assignmentStmt, ioStmt, term, termPrime, factor, var, arithmeticExpression, expPrime, singleOrRecId, option_single_constructed, oneExpansion, moreExpansions, funCallStmt, outputParameters, inputParameters, lowPrecedenceOperators, highPrecedenceOperators, booleanExpression, logicalOp, relationalOp, returnStmt, optionalReturn, idList, more_ids, definetypestmt, A} NONTERMINAL;

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
    int lineNumber;
    char lexeme[31];
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
