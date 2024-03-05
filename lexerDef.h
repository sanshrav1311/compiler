/*
Group Number 16
Sanshrav Arora 2021A7PS2690P
Ajey Malik 2021A7PS2542P
Nachiketh S Shastry 2021A7PS2686P
Rikhil Gupta 2021A7PS0533P
Rishabh Sahni 2021A7PS1630P
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
# define BUFFER_SIZE 204896
# define NO_OF_KEYWORDS 28
# define HASH_TABLE_SIZE 100000007

typedef enum NONTERMINAL {program, mainFunction, otherFunctions, function, input_par, output_par, parameter_list, remaining_list, declaration, dataType, primitiveDatatype, constructedDatatype, stmts, typeDefinitions, actualOrRedefined, typeDefinition, fieldDefinitions, fieldDefinition, fieldType, moreFields, declarations, global_or_not, iterativeStmt, elsePart, otherStmts, stmt, conditionalStmt, assignmentStmt, ioStmt, term, termPrime, factor, var, arithmeticExpression, expPrime, singleOrRecId, option_single_constructed, oneExpansion, moreExpansions, funCallStmt, outputParameters, inputParameters, lowPrecedenceOperators, highPrecedenceOperators, booleanExpression, logicalOp, relationalOp, returnStmt, optionalReturn, idList, more_ids, definetypestmt, A} NONTERMINAL;
typedef enum TOKENS {TK_ERROR, TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM, TK_FUNID, TK_RUID, TK_WITH, TK_PARAMETERS, TK_END, TK_WHILE, TK_UNION, TK_ENDUNION, TK_DEFINETYPE, TK_AS, TK_TYPE, TK_MAIN, TK_GLOBAL, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_INPUT, TK_OUTPUT, TK_INT, TK_REAL, TK_COMMA, TK_SEM, TK_COLON, TK_DOT, TK_ENDWHILE, TK_OP, TK_CL, TK_IF, TK_THEN, TK_ENDIF, TK_READ, TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV, TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE, TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_EQ, TK_GT, TK_GE, TK_NE, dollar, nf, epsilon} TOKENS;

typedef struct twinBuffer{
    char buffer[BUFFER_SIZE];
    int currentIndex;
    int fileSize;
} twinBuffer;

typedef struct keyword{
    char key[31];
    struct keyword* next;
}keyword;

typedef struct tokenInfo {
    int line;
    char lexeme[21474837]; //lexeme[21];
    TOKENS tokenName;
} tokenInfo;
