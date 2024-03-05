/*
Group Number 16
Sanshrav Arora 2021A7PS2690P
Ajey Malik 2021A7PS2542P
Nachiketh S Shastry 2021A7PS2686P
Rikhil Gupta 2021A7PS0533P
Rishabh Sahni 2021A7PS1630P
*/

#include "parser.h"

int hash(NONTERMINAL nt){
    return nt % GRAMMER_TABLE_SIZE;
}

const char* nonterminalToString(NONTERMINAL nonterminal) {
    switch(nonterminal) {
        case program: return "program";
        case mainFunction: return "mainFunction";
        case otherFunctions: return "otherFunctions";
        case function: return "function";
        case input_par: return "input_par";
        case output_par: return "output_par";
        case parameter_list: return "parameter_list";
        case dataType: return "dataType";
        case primitiveDatatype: return "primitiveDatatype";
        case constructedDatatype: return "constructedDatatype";
        case remaining_list: return "remaining_list";
        case stmts: return "stmts";
        case typeDefinitions: return "typeDefinitions";
        case typeDefinition: return "typeDefinition";
        case fieldDefinitions: return "fieldDefinitions";
        case fieldDefinition: return "fieldDefinition";
        case moreFields: return "moreFields";
        case declarations: return "declarations";
        case declaration: return "declaration";
        case otherStmts: return "otherStmts";
        case stmt: return "stmt";
        case assignmentStmt: return "assignmentStmt";
        case singleOrRecId: return "singleOrRecId";
        case funCallStmt: return "funCallStmt";
        case outputParameters: return "outputParameters";
        case inputParameters: return "inputParameters";
        case iterativeStmt: return "iterativeStmt";
        case conditionalStmt: return "conditionalStmt";
        case ioStmt: return "ioStmt";
        case arithmeticExpression: return "arithmeticExpression";
        case booleanExpression: return "booleanExpression";
        case var: return "var";
        case logicalOp: return "logicalOp";
        case relationalOp: return "relationalOp";
        case returnStmt: return "returnStmt";
        case optionalReturn: return "optionalReturn";
        case idList: return "idList";
        case more_ids: return "more_ids";
        case definetypestmt: return "definetypestmt";
        case A: return "A";
        case actualOrRedefined: return "actualOrRedefined";
        case fieldType: return "fieldType";
        case global_or_not: return "global_or_not";
        case option_single_constructed: return "option_single_constructed";
        case oneExpansion: return "oneExpansion";
        case moreExpansions: return "moreExpansions";
        case elsePart: return "elsePart";
        case term: return "term";
        case expPrime: return "expPrime";
        case lowPrecedenceOperators: return "lowPrecedenceOperators";
        case factor: return "factor";
        case termPrime: return "termPrime";
        case highPrecedenceOperators: return "highPrecedenceOperators";
        default: return "Unknown NONTERMINAL";
    }
}

int ntCount[GRAMMER_TABLE_SIZE] = {0}; // for constructing follow such that it does not go into a recursive loop;

RHSHead* createRHSHead() {
    RHSHead* newNode = (RHSHead*)malloc(sizeof(RHSHead));
    newNode->count = 0;
    newNode->last = NULL;
    newNode->first = NULL;
    return newNode;
}

RHS* createRHSNode() {
    RHS* newNode = (RHS*)malloc(sizeof(RHS));
    newNode->count = 0;
    newNode->first = NULL;
    newNode->last = NULL;
    newNode->next = NULL;
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

void insertRHS(RHSHead* head, RHS* rhs) {
    if (head->first == NULL) {
        head->first = rhs;
    } else {
        head->last->next = rhs;
    }
    head->last = rhs;
    head->count++;
}

void insertGrammerElement(RHS* head, GrammerElement* ge) {
    if (head->first == NULL) {
        head->first = ge;
    } else {
        head->last->next = ge;
    }
    head->last = ge;
    head->count++;
}

RHSHead* Grammer[GRAMMER_TABLE_SIZE] = { NULL };

void insertInHashTable(NONTERMINAL nt, RHSHead* rhsHead){
    int index = hash(nt);
    Grammer[index] = rhsHead;
}

void intialiseGrammer() {
    // <program> ===> <otherFunctions> <mainFunction>
    RHSHead* tempRHProgram = createRHSHead();

    RHS* tempRProgram = createRHSNode();
    insertGrammerElement(tempRProgram, createGrammerElement(false, (NONTERMINAL)otherFunctions));
    insertGrammerElement(tempRProgram, createGrammerElement(false, (NONTERMINAL)mainFunction));
    insertRHS(tempRHProgram, tempRProgram);

    insertInHashTable(program, tempRHProgram);

    // <mainFunction>===> TK_MAIN <stmts> TK_END
    RHSHead* tempRHMainFunction = createRHSHead();

    RHS* tempRMainFunction = createRHSNode();
    insertGrammerElement(tempRMainFunction, createGrammerElement(true, (TOKENS)TK_MAIN));
    insertGrammerElement(tempRMainFunction, createGrammerElement(false, (NONTERMINAL)stmts));
    insertGrammerElement(tempRMainFunction, createGrammerElement(true, (TOKENS)TK_END));
    insertRHS(tempRHMainFunction, tempRMainFunction);

    insertInHashTable(mainFunction, tempRHMainFunction);

    // <otherFunctions>===> <function><otherFunctions> | ε
    RHSHead* tempRHOtherFunctions = createRHSHead();

    RHS* tempROtherFunctions1 = createRHSNode();
    insertGrammerElement(tempROtherFunctions1, createGrammerElement(false, (NONTERMINAL)function));
    insertGrammerElement(tempROtherFunctions1, createGrammerElement(false, (NONTERMINAL)otherFunctions));
    insertRHS(tempRHOtherFunctions, tempROtherFunctions1);

    RHS* tempROtherFunctions2 = createRHSNode(); // ε production
    insertRHS(tempRHOtherFunctions, tempROtherFunctions2);

    insertInHashTable(otherFunctions, tempRHOtherFunctions);

    // <function>===>TK_FUNID <input_par> <output_par> TK_SEM <stmts> TK_END
    RHSHead* tempRHFunction = createRHSHead();

    RHS* tempRFunction = createRHSNode();
    insertGrammerElement(tempRFunction, createGrammerElement(true, (TOKENS)TK_FUNID));
    insertGrammerElement(tempRFunction, createGrammerElement(false, (NONTERMINAL)input_par));
    insertGrammerElement(tempRFunction, createGrammerElement(false, (NONTERMINAL)output_par));
    insertGrammerElement(tempRFunction, createGrammerElement(true, (TOKENS)TK_SEM));
    insertGrammerElement(tempRFunction, createGrammerElement(false, (NONTERMINAL)stmts));
    insertGrammerElement(tempRFunction, createGrammerElement(true, (TOKENS)TK_END));
    insertRHS(tempRHFunction, tempRFunction);

    insertInHashTable(function, tempRHFunction);

    // <input_par>===>TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
    RHSHead* tempRHInputPar = createRHSHead();

    RHS* tempRInputPar = createRHSNode();
    insertGrammerElement(tempRInputPar, createGrammerElement(true, (TOKENS)TK_INPUT));
    insertGrammerElement(tempRInputPar, createGrammerElement(true, (TOKENS)TK_PARAMETER));
    insertGrammerElement(tempRInputPar, createGrammerElement(true, (TOKENS)TK_LIST));
    insertGrammerElement(tempRInputPar, createGrammerElement(true, (TOKENS)TK_SQL));
    insertGrammerElement(tempRInputPar, createGrammerElement(false, (NONTERMINAL)parameter_list));
    insertGrammerElement(tempRInputPar, createGrammerElement(true, (TOKENS)TK_SQR));
    insertRHS(tempRHInputPar, tempRInputPar);

    insertInHashTable(input_par, tempRHInputPar);

    // <output_par>===>TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR | ε
    RHSHead* tempRHOutputPar = createRHSHead();

    RHS* tempROutputPar1 = createRHSNode();
    insertGrammerElement(tempROutputPar1, createGrammerElement(true, (TOKENS)TK_OUTPUT));
    insertGrammerElement(tempROutputPar1, createGrammerElement(true, (TOKENS)TK_PARAMETER));
    insertGrammerElement(tempROutputPar1, createGrammerElement(true, (TOKENS)TK_LIST));
    insertGrammerElement(tempROutputPar1, createGrammerElement(true, (TOKENS)TK_SQL));
    insertGrammerElement(tempROutputPar1, createGrammerElement(false, (NONTERMINAL)parameter_list));
    insertGrammerElement(tempROutputPar1, createGrammerElement(true, (TOKENS)TK_SQR));
    insertRHS(tempRHOutputPar, tempROutputPar1);

    RHS* tempROutputPar2 = createRHSNode(); // ε production
    insertRHS(tempRHOutputPar, tempROutputPar2);

    insertInHashTable(output_par, tempRHOutputPar);

    // <parameter_list>===><dataType> TK_ID <remaining_list>
    RHSHead* tempRHParameterList = createRHSHead();

    RHS* tempRParameterList = createRHSNode();
    insertGrammerElement(tempRParameterList, createGrammerElement(false, (NONTERMINAL)dataType));
    insertGrammerElement(tempRParameterList, createGrammerElement(true, (TOKENS)TK_ID));
    insertGrammerElement(tempRParameterList, createGrammerElement(false, (NONTERMINAL)remaining_list));
    insertRHS(tempRHParameterList, tempRParameterList);

    insertInHashTable(parameter_list, tempRHParameterList);

    // <dataType>===> <primitiveDatatype> |<constructedDatatype>
    RHSHead* tempRHDataType = createRHSHead();

    RHS* tempRDataType1 = createRHSNode();
    insertGrammerElement(tempRDataType1, createGrammerElement(false, (NONTERMINAL)primitiveDatatype));
    insertRHS(tempRHDataType, tempRDataType1);

    RHS* tempRDataType2 = createRHSNode();
    insertGrammerElement(tempRDataType2, createGrammerElement(false, (NONTERMINAL)constructedDatatype));
    insertRHS(tempRHDataType, tempRDataType2);

    insertInHashTable(dataType, tempRHDataType);

    // <primitiveDatatype>===> TK_INT | TK_REAL
    RHSHead* tempRHPrimitiveDatatype = createRHSHead();

    RHS* tempRPrimitiveDatatype1 = createRHSNode();
    insertGrammerElement(tempRPrimitiveDatatype1, createGrammerElement(true, (TOKENS)TK_INT));
    insertRHS(tempRHPrimitiveDatatype, tempRPrimitiveDatatype1);

    RHS* tempRPrimitiveDatatype2 = createRHSNode();
    insertGrammerElement(tempRPrimitiveDatatype2, createGrammerElement(true, (TOKENS)TK_REAL));
    insertRHS(tempRHPrimitiveDatatype, tempRPrimitiveDatatype2);

    insertInHashTable(primitiveDatatype, tempRHPrimitiveDatatype);

    // <constructedDatatype>===> TK_RECORD TK_RUID | TK_UNION TK_RUID | TK_RUID
    RHSHead* tempRHConstructedDatatype = createRHSHead();

    RHS* tempRConstructedDatatype1 = createRHSNode();
    insertGrammerElement(tempRConstructedDatatype1, createGrammerElement(true, (TOKENS)TK_RECORD));
    insertGrammerElement(tempRConstructedDatatype1, createGrammerElement(true, (TOKENS)TK_RUID));
    insertRHS(tempRHConstructedDatatype, tempRConstructedDatatype1);

    RHS* tempRConstructedDatatype2 = createRHSNode();
    insertGrammerElement(tempRConstructedDatatype2, createGrammerElement(true, (TOKENS)TK_UNION));
    insertGrammerElement(tempRConstructedDatatype2, createGrammerElement(true, (TOKENS)TK_RUID));
    insertRHS(tempRHConstructedDatatype, tempRConstructedDatatype2);

    RHS* tempRConstructedDatatype3 = createRHSNode();
    insertGrammerElement(tempRConstructedDatatype3, createGrammerElement(true, (TOKENS)TK_RUID));
    insertRHS(tempRHConstructedDatatype, tempRConstructedDatatype3);

    insertInHashTable(constructedDatatype, tempRHConstructedDatatype);

    // <remaining_list>===>TK_COMMA <parameter_list> | ε
    RHSHead* tempRHRemainingList = createRHSHead();

    RHS* tempRRemainingList1 = createRHSNode();
    insertGrammerElement(tempRRemainingList1, createGrammerElement(true, (TOKENS)TK_COMMA));
    insertGrammerElement(tempRRemainingList1, createGrammerElement(false, (NONTERMINAL)parameter_list));
    insertRHS(tempRHRemainingList, tempRRemainingList1);

    RHS* tempRRemainingList2 = createRHSNode(); // ε production
    insertRHS(tempRHRemainingList, tempRRemainingList2);

    insertInHashTable(remaining_list, tempRHRemainingList);

    // <stmts>===><typeDefinitions> <declarations> <otherStmts><returnStmt>
    RHSHead* tempRHStmts = createRHSHead();

    RHS* tempRStmts = createRHSNode();
    insertGrammerElement(tempRStmts, createGrammerElement(false, (NONTERMINAL)typeDefinitions));
    insertGrammerElement(tempRStmts, createGrammerElement(false, (NONTERMINAL)declarations));
    insertGrammerElement(tempRStmts, createGrammerElement(false, (NONTERMINAL)otherStmts));
    insertGrammerElement(tempRStmts, createGrammerElement(false, (NONTERMINAL)returnStmt));
    insertRHS(tempRHStmts, tempRStmts);

    insertInHashTable(stmts, tempRHStmts);

    // <typeDefinitions>===> <actualOrRedefined> <typeDefinitions> | ε
    RHSHead* tempRHTypeDefinitions = createRHSHead();

    RHS* tempRTypeDefinitions1 = createRHSNode();
    insertGrammerElement(tempRTypeDefinitions1, createGrammerElement(false, (NONTERMINAL)actualOrRedefined));
    insertGrammerElement(tempRTypeDefinitions1, createGrammerElement(false, (NONTERMINAL)typeDefinitions));
    insertRHS(tempRHTypeDefinitions, tempRTypeDefinitions1);

    RHS* tempRTypeDefinitions2 = createRHSNode(); // ε production
    insertRHS(tempRHTypeDefinitions, tempRTypeDefinitions2);

    insertInHashTable(typeDefinitions, tempRHTypeDefinitions);

    // <actualOrRedefined> ===> <typeDefinition> | <definetypestmt>
    RHSHead* tempRHActualOrRedefined = createRHSHead();

    RHS* tempRActualOrRedefined1 = createRHSNode();
    insertGrammerElement(tempRActualOrRedefined1, createGrammerElement(false, (NONTERMINAL)typeDefinition));
    insertRHS(tempRHActualOrRedefined, tempRActualOrRedefined1);

    RHS* tempRActualOrRedefined2 = createRHSNode();
    insertGrammerElement(tempRActualOrRedefined2, createGrammerElement(false, (NONTERMINAL)definetypestmt));
    insertRHS(tempRHActualOrRedefined, tempRActualOrRedefined2);

    insertInHashTable(actualOrRedefined, tempRHActualOrRedefined);

    // <typeDefinition>===>TK_RECORD TK_RUID <fieldDefinitions> TK_ENDRECORD | TK_UNION TK_RUID <fieldDefinitions> TK_ENDUNION
    RHSHead* tempRHTypeDefinition = createRHSHead();

    RHS* tempRTypeDefinition1 = createRHSNode();
    insertGrammerElement(tempRTypeDefinition1, createGrammerElement(true, (TOKENS)TK_RECORD));
    insertGrammerElement(tempRTypeDefinition1, createGrammerElement(true, (TOKENS)TK_RUID));
    insertGrammerElement(tempRTypeDefinition1, createGrammerElement(false, (NONTERMINAL)fieldDefinitions));
    insertGrammerElement(tempRTypeDefinition1, createGrammerElement(true, (TOKENS)TK_ENDRECORD));
    insertRHS(tempRHTypeDefinition, tempRTypeDefinition1);

    RHS* tempRTypeDefinition2 = createRHSNode();
    insertGrammerElement(tempRTypeDefinition2, createGrammerElement(true, (TOKENS)TK_UNION));
    insertGrammerElement(tempRTypeDefinition2, createGrammerElement(true, (TOKENS)TK_RUID));
    insertGrammerElement(tempRTypeDefinition2, createGrammerElement(false, (NONTERMINAL)fieldDefinitions));
    insertGrammerElement(tempRTypeDefinition2, createGrammerElement(true, (TOKENS)TK_ENDUNION));
    insertRHS(tempRHTypeDefinition, tempRTypeDefinition2);

    insertInHashTable(typeDefinition, tempRHTypeDefinition);

    // <fieldDefinitions>===> <fieldDefinition><fieldDefinition><moreFields>| TK_ENDUNION
    RHSHead* tempRHFieldDefinitions = createRHSHead();

    RHS* tempRFieldDefinitions1 = createRHSNode();
    insertGrammerElement(tempRFieldDefinitions1, createGrammerElement(false, (NONTERMINAL)fieldDefinition));
    insertGrammerElement(tempRFieldDefinitions1, createGrammerElement(false, (NONTERMINAL)fieldDefinition));
    insertGrammerElement(tempRFieldDefinitions1, createGrammerElement(false, (NONTERMINAL)moreFields));
    insertRHS(tempRHFieldDefinitions, tempRFieldDefinitions1);

    RHS* tempRFieldDefinitions2 = createRHSNode();
    insertGrammerElement(tempRFieldDefinitions2, createGrammerElement(true, (TOKENS)TK_ENDUNION));
    insertRHS(tempRHFieldDefinitions, tempRFieldDefinitions2);

    insertInHashTable(fieldDefinitions, tempRHFieldDefinitions);


    // <fieldDefinition>===> TK_TYPE <fieldType> TK_COLON TK_FIELDID TK_SEM
    RHSHead* tempRHFieldDefinition = createRHSHead();

    RHS* tempRFieldDefinition = createRHSNode();
    insertGrammerElement(tempRFieldDefinition, createGrammerElement(true, (TOKENS)TK_TYPE));
    insertGrammerElement(tempRFieldDefinition, createGrammerElement(false, (NONTERMINAL)fieldType));
    insertGrammerElement(tempRFieldDefinition, createGrammerElement(true, (TOKENS)TK_COLON));
    insertGrammerElement(tempRFieldDefinition, createGrammerElement(true, (TOKENS)TK_FIELDID));
    insertGrammerElement(tempRFieldDefinition, createGrammerElement(true, (TOKENS)TK_SEM));
    insertRHS(tempRHFieldDefinition, tempRFieldDefinition);

    insertInHashTable(fieldDefinition, tempRHFieldDefinition);

    // <fieldType>===> <primitiveDatatype> | <constructedDatatype>
    RHSHead* tempRHFieldType = createRHSHead();

    RHS* tempRFieldType1 = createRHSNode();
    insertGrammerElement(tempRFieldType1, createGrammerElement(false, (NONTERMINAL)primitiveDatatype));
    insertRHS(tempRHFieldType, tempRFieldType1);

    RHS* tempRFieldType2 = createRHSNode();
    insertGrammerElement(tempRFieldType2, createGrammerElement(false, (NONTERMINAL)constructedDatatype));
    insertRHS(tempRHFieldType, tempRFieldType2);

    insertInHashTable(fieldType, tempRHFieldType);

    // <moreFields>===><fieldDefinition><moreFields> | ε
    RHSHead* tempRHMoreFields = createRHSHead();

    RHS* tempRMoreFields1 = createRHSNode();
    insertGrammerElement(tempRMoreFields1, createGrammerElement(false, (NONTERMINAL)fieldDefinition));
    insertGrammerElement(tempRMoreFields1, createGrammerElement(false, (NONTERMINAL)moreFields));
    insertRHS(tempRHMoreFields, tempRMoreFields1);

    RHS* tempRMoreFields2 = createRHSNode(); // ε production
    insertRHS(tempRHMoreFields, tempRMoreFields2);

    insertInHashTable(moreFields, tempRHMoreFields);

    // <declarations> ===> <declaration><declarations>| ε
    RHSHead* tempRHDeclarations = createRHSHead();

    RHS* tempRDeclarations1 = createRHSNode();
    insertGrammerElement(tempRDeclarations1, createGrammerElement(false, (NONTERMINAL)declaration));
    insertGrammerElement(tempRDeclarations1, createGrammerElement(false, (NONTERMINAL)declarations));
    insertRHS(tempRHDeclarations, tempRDeclarations1);

    RHS* tempRDeclarations2 = createRHSNode(); // ε production
    insertRHS(tempRHDeclarations, tempRDeclarations2);

    insertInHashTable(declarations, tempRHDeclarations);

    // <declaration>===> TK_TYPE <dataType> TK_COLON TK_ID <global_or_not> TK_SEM
    RHSHead* tempRHDeclaration = createRHSHead();

    RHS* tempRDeclaration = createRHSNode();
    insertGrammerElement(tempRDeclaration, createGrammerElement(true, (TOKENS)TK_TYPE));
    insertGrammerElement(tempRDeclaration, createGrammerElement(false, (NONTERMINAL)dataType));
    insertGrammerElement(tempRDeclaration, createGrammerElement(true, (TOKENS)TK_COLON));
    insertGrammerElement(tempRDeclaration, createGrammerElement(true, (TOKENS)TK_ID));
    insertGrammerElement(tempRDeclaration, createGrammerElement(false, (NONTERMINAL)global_or_not));
    insertGrammerElement(tempRDeclaration, createGrammerElement(true, (TOKENS)TK_SEM));
    insertRHS(tempRHDeclaration, tempRDeclaration);

    insertInHashTable(declaration, tempRHDeclaration);

    // <global_or_not>===>TK_COLON TK_GLOBAL| ε
    RHSHead* tempRHGlobalOrNot = createRHSHead();

    RHS* tempRGlobalOrNot1 = createRHSNode();
    insertGrammerElement(tempRGlobalOrNot1, createGrammerElement(true, (TOKENS)TK_COLON));
    insertGrammerElement(tempRGlobalOrNot1, createGrammerElement(true, (TOKENS)TK_GLOBAL));
    insertRHS(tempRHGlobalOrNot, tempRGlobalOrNot1);

    RHS* tempRGlobalOrNot2 = createRHSNode(); // ε production
    insertRHS(tempRHGlobalOrNot, tempRGlobalOrNot2);

    insertInHashTable(global_or_not, tempRHGlobalOrNot);

    // <otherStmts>===> <stmt><otherStmts> | ε
    RHSHead* tempRHOtherStmts = createRHSHead();

    RHS* tempROtherStmts1 = createRHSNode();
    insertGrammerElement(tempROtherStmts1, createGrammerElement(false, (NONTERMINAL)stmt));
    insertGrammerElement(tempROtherStmts1, createGrammerElement(false, (NONTERMINAL)otherStmts));
    insertRHS(tempRHOtherStmts, tempROtherStmts1);

    RHS* tempROtherStmts2 = createRHSNode(); // ε production
    insertRHS(tempRHOtherStmts, tempROtherStmts2);

    insertInHashTable(otherStmts, tempRHOtherStmts);

    // <stmt>===> <assignmentStmt> | <iterativeStmt>|<conditionalStmt>|<ioStmt>| <funCallStmt>
    RHSHead* tempRHStmt = createRHSHead();

    RHS* tempRStmt1 = createRHSNode();
    insertGrammerElement(tempRStmt1, createGrammerElement(false, (NONTERMINAL)assignmentStmt));
    insertRHS(tempRHStmt, tempRStmt1);

    RHS* tempRStmt2 = createRHSNode();
    insertGrammerElement(tempRStmt2, createGrammerElement(false, (NONTERMINAL)iterativeStmt));
    insertRHS(tempRHStmt, tempRStmt2);

    RHS* tempRStmt3 = createRHSNode();
    insertGrammerElement(tempRStmt3, createGrammerElement(false, (NONTERMINAL)conditionalStmt));
    insertRHS(tempRHStmt, tempRStmt3);

    RHS* tempRStmt4 = createRHSNode();
    insertGrammerElement(tempRStmt4, createGrammerElement(false, (NONTERMINAL)ioStmt));
    insertRHS(tempRHStmt, tempRStmt4);

    RHS* tempRStmt5 = createRHSNode();
    insertGrammerElement(tempRStmt5, createGrammerElement(false, (NONTERMINAL)funCallStmt));
    insertRHS(tempRHStmt, tempRStmt5);

    insertInHashTable(stmt, tempRHStmt);

    // <assignmentStmt>===><SingleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM
    RHSHead* tempRHAssignmentStmt = createRHSHead();
    // SingleOrRecId, arithmeticExpression, option_single_constructed,oneExpansion, moreExpansions

    RHS* tempRAssignmentStmt = createRHSNode();
    insertGrammerElement(tempRAssignmentStmt, createGrammerElement(false, (NONTERMINAL)singleOrRecId));
    insertGrammerElement(tempRAssignmentStmt, createGrammerElement(true, (TOKENS)TK_ASSIGNOP));
    insertGrammerElement(tempRAssignmentStmt, createGrammerElement(false, (NONTERMINAL)arithmeticExpression));
    insertGrammerElement(tempRAssignmentStmt, createGrammerElement(true, (TOKENS)TK_SEM));
    insertRHS(tempRHAssignmentStmt, tempRAssignmentStmt);

    insertInHashTable(assignmentStmt, tempRHAssignmentStmt);

    // <SingleOrRecId>===>TK_ID <option_single_constructed>
    RHSHead* tempRHSingleOrRecId = createRHSHead();

    RHS* tempRSingleOrRecId = createRHSNode();
    insertGrammerElement(tempRSingleOrRecId, createGrammerElement(true, (TOKENS)TK_ID));
    insertGrammerElement(tempRSingleOrRecId, createGrammerElement(false, (NONTERMINAL)option_single_constructed));
    insertRHS(tempRHSingleOrRecId, tempRSingleOrRecId);

    insertInHashTable(singleOrRecId, tempRHSingleOrRecId);

    // <option_single_constructed>===> ε | <oneExpansion><moreExpansions>
    RHSHead* tempRHOptionSingleConstructed = createRHSHead();

    RHS* tempROptionSingleConstructed1 = createRHSNode(); // ε production
    insertRHS(tempRHOptionSingleConstructed, tempROptionSingleConstructed1);

    RHS* tempROptionSingleConstructed2 = createRHSNode();
    insertGrammerElement(tempROptionSingleConstructed2, createGrammerElement(false, (NONTERMINAL)oneExpansion));
    insertGrammerElement(tempROptionSingleConstructed2, createGrammerElement(false, (NONTERMINAL)moreExpansions));
    insertRHS(tempRHOptionSingleConstructed, tempROptionSingleConstructed2);

    insertInHashTable(option_single_constructed, tempRHOptionSingleConstructed);

    // <oneExpansion>===> TK_DOT TK_FIELDID 
    RHSHead* tempRHOneExpansion = createRHSHead();

    RHS* tempROneExpansion = createRHSNode();
    insertGrammerElement(tempROneExpansion, createGrammerElement(true, (TOKENS)TK_DOT));
    insertGrammerElement(tempROneExpansion, createGrammerElement(true, (TOKENS)TK_FIELDID));
    insertRHS(tempRHOneExpansion, tempROneExpansion);

    insertInHashTable(oneExpansion, tempRHOneExpansion);

    // <moreExpansions>===> <oneExpansion> <moreExpansions> | ε
    RHSHead* tempRHMoreExpansions = createRHSHead();

    RHS* tempRMoreExpansions1 = createRHSNode();
    insertGrammerElement(tempRMoreExpansions1, createGrammerElement(false, (NONTERMINAL)oneExpansion));
    insertGrammerElement(tempRMoreExpansions1, createGrammerElement(false, (NONTERMINAL)moreExpansions));
    insertRHS(tempRHMoreExpansions, tempRMoreExpansions1);

    RHS* tempRMoreExpansions2 = createRHSNode(); // ε production
    insertRHS(tempRHMoreExpansions, tempRMoreExpansions2);

    insertInHashTable(moreExpansions, tempRHMoreExpansions);

    // <funCallStmt>===><outputParameters> TK_CALL TK_FUNID TK_WITH TK_PARAMETERS <inputParameters> TK_SEM
    RHSHead* tempRHFunCallStmt = createRHSHead();

    RHS* tempRFunCallStmt = createRHSNode();
    insertGrammerElement(tempRFunCallStmt, createGrammerElement(false, (NONTERMINAL)outputParameters));
    insertGrammerElement(tempRFunCallStmt, createGrammerElement(true, (TOKENS)TK_CALL));
    insertGrammerElement(tempRFunCallStmt, createGrammerElement(true, (TOKENS)TK_FUNID));
    insertGrammerElement(tempRFunCallStmt, createGrammerElement(true, (TOKENS)TK_WITH));
    insertGrammerElement(tempRFunCallStmt, createGrammerElement(true, (TOKENS)TK_PARAMETERS));
    insertGrammerElement(tempRFunCallStmt, createGrammerElement(false, (NONTERMINAL)inputParameters));
    insertGrammerElement(tempRFunCallStmt, createGrammerElement(true, (TOKENS)TK_SEM));
    insertRHS(tempRHFunCallStmt, tempRFunCallStmt);

    insertInHashTable(funCallStmt, tempRHFunCallStmt);

    // <outputParameters> ==> TK_SQL <idList> TK_SQR TK_ASSIGNOP | ε
    RHSHead* tempRHOutputParameters = createRHSHead();

    RHS* tempROutputParameters1 = createRHSNode();
    insertGrammerElement(tempROutputParameters1, createGrammerElement(true, (TOKENS)TK_SQL));
    insertGrammerElement(tempROutputParameters1, createGrammerElement(false, (NONTERMINAL)idList));
    insertGrammerElement(tempROutputParameters1, createGrammerElement(true, (TOKENS)TK_SQR));
    insertGrammerElement(tempROutputParameters1, createGrammerElement(true, (TOKENS)TK_ASSIGNOP));
    insertRHS(tempRHOutputParameters, tempROutputParameters1);

    RHS* tempROutputParameters2 = createRHSNode(); // ε production
    insertRHS(tempRHOutputParameters, tempROutputParameters2);

    insertInHashTable(outputParameters, tempRHOutputParameters);

    // <inputParameters>===> TK_SQL <idList> TK_SQR
    RHSHead* tempRHInputParameters = createRHSHead();

    RHS* tempRInputParameters = createRHSNode();
    insertGrammerElement(tempRInputParameters, createGrammerElement(true, (TOKENS)TK_SQL));
    insertGrammerElement(tempRInputParameters, createGrammerElement(false, (NONTERMINAL)idList));
    insertGrammerElement(tempRInputParameters, createGrammerElement(true, (TOKENS)TK_SQR));
    insertRHS(tempRHInputParameters, tempRInputParameters);

    insertInHashTable(inputParameters, tempRHInputParameters);

    // <iterativeStmt>===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt><otherStmts> TK_ENDWHILE
    RHSHead* tempRHIterativeStmt = createRHSHead();

    RHS* tempRIterativeStmt = createRHSNode();
    insertGrammerElement(tempRIterativeStmt, createGrammerElement(true, (TOKENS)TK_WHILE));
    insertGrammerElement(tempRIterativeStmt, createGrammerElement(true, (TOKENS)TK_OP));
    insertGrammerElement(tempRIterativeStmt, createGrammerElement(false, (NONTERMINAL)booleanExpression));
    insertGrammerElement(tempRIterativeStmt, createGrammerElement(true, (TOKENS)TK_CL));
    insertGrammerElement(tempRIterativeStmt, createGrammerElement(false, (NONTERMINAL)stmt));
    insertGrammerElement(tempRIterativeStmt, createGrammerElement(false, (NONTERMINAL)otherStmts));
    insertGrammerElement(tempRIterativeStmt, createGrammerElement(true, (TOKENS)TK_ENDWHILE));
    insertRHS(tempRHIterativeStmt, tempRIterativeStmt);

    insertInHashTable(iterativeStmt, tempRHIterativeStmt);

    // <conditionalStmt>===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt><otherStmts> <elsePart>
    RHSHead* tempRHConditionalStmt = createRHSHead();

    RHS* tempRConditionalStmt = createRHSNode();
    insertGrammerElement(tempRConditionalStmt, createGrammerElement(true, (TOKENS)TK_IF));
    insertGrammerElement(tempRConditionalStmt, createGrammerElement(true, (TOKENS)TK_OP));
    insertGrammerElement(tempRConditionalStmt, createGrammerElement(false, (NONTERMINAL)booleanExpression));
    insertGrammerElement(tempRConditionalStmt, createGrammerElement(true, (TOKENS)TK_CL));
    insertGrammerElement(tempRConditionalStmt, createGrammerElement(true, (TOKENS)TK_THEN));
    insertGrammerElement(tempRConditionalStmt, createGrammerElement(false, (NONTERMINAL)stmt));
    insertGrammerElement(tempRConditionalStmt, createGrammerElement(false, (NONTERMINAL)otherStmts));
    insertGrammerElement(tempRConditionalStmt, createGrammerElement(false, (NONTERMINAL)elsePart));
    insertRHS(tempRHConditionalStmt, tempRConditionalStmt);

    insertInHashTable(conditionalStmt, tempRHConditionalStmt);

    // <elsePart>===>TK_ELSE <stmt><otherStmts> TK_ENDIF | TK_ENDIF
    RHSHead* tempRHElsePart = createRHSHead();

    RHS* tempRElsePart1 = createRHSNode();
    insertGrammerElement(tempRElsePart1, createGrammerElement(true, (TOKENS)TK_ELSE));
    insertGrammerElement(tempRElsePart1, createGrammerElement(false, (NONTERMINAL)stmt));
    insertGrammerElement(tempRElsePart1, createGrammerElement(false, (NONTERMINAL)otherStmts));
    insertGrammerElement(tempRElsePart1, createGrammerElement(true, (TOKENS)TK_ENDIF));
    insertRHS(tempRHElsePart, tempRElsePart1);

    RHS* tempRElsePart2 = createRHSNode();
    insertGrammerElement(tempRElsePart2, createGrammerElement(true, (TOKENS)TK_ENDIF));
    insertRHS(tempRHElsePart, tempRElsePart2);

    insertInHashTable(elsePart, tempRHElsePart);

    // <ioStmt>===>TK_READ TK_OP <var> TK_CL TK_SEM | TK_WRITE TK_OP <var> TK_CL TK_SEM
    RHSHead* tempRHIOStmt = createRHSHead();

    RHS* tempRIOStmt1 = createRHSNode();
    insertGrammerElement(tempRIOStmt1, createGrammerElement(true, (TOKENS)TK_READ));
    insertGrammerElement(tempRIOStmt1, createGrammerElement(true, (TOKENS)TK_OP));
    insertGrammerElement(tempRIOStmt1, createGrammerElement(false, (NONTERMINAL)var));
    insertGrammerElement(tempRIOStmt1, createGrammerElement(true, (TOKENS)TK_CL));
    insertGrammerElement(tempRIOStmt1, createGrammerElement(true, (TOKENS)TK_SEM));
    insertRHS(tempRHIOStmt, tempRIOStmt1);

    RHS* tempRIOStmt2 = createRHSNode();
    insertGrammerElement(tempRIOStmt2, createGrammerElement(true, (TOKENS)TK_WRITE));
    insertGrammerElement(tempRIOStmt2, createGrammerElement(true, (TOKENS)TK_OP));
    insertGrammerElement(tempRIOStmt2, createGrammerElement(false, (NONTERMINAL)var));
    insertGrammerElement(tempRIOStmt2, createGrammerElement(true, (TOKENS)TK_CL));
    insertGrammerElement(tempRIOStmt2, createGrammerElement(true, (TOKENS)TK_SEM));
    insertRHS(tempRHIOStmt, tempRIOStmt2);

    insertInHashTable(ioStmt, tempRHIOStmt);

    // <arithmeticExpression> ===> <term> <expPrime>
    RHSHead* tempRHArithmeticExpression = createRHSHead();
    // term,expPrime, lowPrecedenceOperators, factor, termPrime, highPrecedenceOperators
    RHS* tempRArithmeticExpression = createRHSNode();
    insertGrammerElement(tempRArithmeticExpression, createGrammerElement(false, (NONTERMINAL)term));
    insertGrammerElement(tempRArithmeticExpression, createGrammerElement(false, (NONTERMINAL)expPrime));
    insertRHS(tempRHArithmeticExpression, tempRArithmeticExpression);

    insertInHashTable(arithmeticExpression, tempRHArithmeticExpression);

    // <expPrime> ===> <lowPrecedenceOperators> <term> <expPrime> | ε
    RHSHead* tempRHExpPrime = createRHSHead();

    RHS* tempRExpPrime1 = createRHSNode();
    insertGrammerElement(tempRExpPrime1, createGrammerElement(false, (NONTERMINAL)lowPrecedenceOperators));
    insertGrammerElement(tempRExpPrime1, createGrammerElement(false, (NONTERMINAL)term));
    insertGrammerElement(tempRExpPrime1, createGrammerElement(false, (NONTERMINAL)expPrime));
    insertRHS(tempRHExpPrime, tempRExpPrime1);

    RHS* tempRExpPrime2 = createRHSNode(); // ε production
    insertRHS(tempRHExpPrime, tempRExpPrime2);

    insertInHashTable(expPrime, tempRHExpPrime);

    // <term>===> <factor> <termPrime>
    RHSHead* tempRHTerm = createRHSHead();

    RHS* tempRTerm = createRHSNode();
    insertGrammerElement(tempRTerm, createGrammerElement(false, (NONTERMINAL)factor));
    insertGrammerElement(tempRTerm, createGrammerElement(false, (NONTERMINAL)termPrime));
    insertRHS(tempRHTerm, tempRTerm);

    insertInHashTable(term, tempRHTerm);

    // <termPrime> ===> <highPrecedenceOperators><factor> <termPrime> | ε
    RHSHead* tempRHTermPrime = createRHSHead();

    RHS* tempRTermPrime1 = createRHSNode();
    insertGrammerElement(tempRTermPrime1, createGrammerElement(false, (NONTERMINAL)highPrecedenceOperators));
    insertGrammerElement(tempRTermPrime1, createGrammerElement(false, (NONTERMINAL)factor));
    insertGrammerElement(tempRTermPrime1, createGrammerElement(false, (NONTERMINAL)termPrime));
    insertRHS(tempRHTermPrime, tempRTermPrime1);

    RHS* tempRTermPrime2 = createRHSNode(); // ε production
    insertRHS(tempRHTermPrime, tempRTermPrime2);

    insertInHashTable(termPrime, tempRHTermPrime);

    // <factor> ===> TK_OP <arithmeticExpression> TK_CL | <var>
    RHSHead* tempRHFactor = createRHSHead();

    RHS* tempRFactor1 = createRHSNode();
    insertGrammerElement(tempRFactor1, createGrammerElement(true, (TOKENS)TK_OP));
    insertGrammerElement(tempRFactor1, createGrammerElement(false, (NONTERMINAL)arithmeticExpression));
    insertGrammerElement(tempRFactor1, createGrammerElement(true, (TOKENS)TK_CL));
    insertRHS(tempRHFactor, tempRFactor1);

    RHS* tempRFactor2 = createRHSNode();
    insertGrammerElement(tempRFactor2, createGrammerElement(false, (NONTERMINAL)var));
    insertRHS(tempRHFactor, tempRFactor2);

    insertInHashTable(factor, tempRHFactor);

    // <highPrecedenceOperators>===> TK_MUL | TK_DIV
    RHSHead* tempRHHighPrecedenceOperators = createRHSHead();

    RHS* tempRHighPrecedenceOperators1 = createRHSNode();
    insertGrammerElement(tempRHighPrecedenceOperators1, createGrammerElement(true, (TOKENS)TK_MUL));
    insertRHS(tempRHHighPrecedenceOperators, tempRHighPrecedenceOperators1);

    RHS* tempRHighPrecedenceOperators2 = createRHSNode();
    insertGrammerElement(tempRHighPrecedenceOperators2, createGrammerElement(true, (TOKENS)TK_DIV));
    insertRHS(tempRHHighPrecedenceOperators, tempRHighPrecedenceOperators2);

    insertInHashTable(highPrecedenceOperators, tempRHHighPrecedenceOperators);

    // <lowPrecedenceOperators> ===> TK_PLUS | TK_MINUS
    RHSHead* tempRHLowPrecedenceOperators = createRHSHead();

    RHS* tempRLowPrecedenceOperators1 = createRHSNode();
    insertGrammerElement(tempRLowPrecedenceOperators1, createGrammerElement(true, (TOKENS)TK_PLUS));
    insertRHS(tempRHLowPrecedenceOperators, tempRLowPrecedenceOperators1);

    RHS* tempRLowPrecedenceOperators2 = createRHSNode();
    insertGrammerElement(tempRLowPrecedenceOperators2, createGrammerElement(true, (TOKENS)TK_MINUS));
    insertRHS(tempRHLowPrecedenceOperators, tempRLowPrecedenceOperators2);

    insertInHashTable(lowPrecedenceOperators, tempRHLowPrecedenceOperators);

    // <booleanExpression>===>TK_OP <booleanExpression> TK_CL <logicalOp> TK_OP <booleanExpression> TK_CL | <var> <relationalOp> <var> | TK_NOT TK_OP <booleanExpression> TK_CL
    RHSHead* tempRHBooleanExpression = createRHSHead();

    RHS* tempRBooleanExpression1 = createRHSNode();
    insertGrammerElement(tempRBooleanExpression1, createGrammerElement(true, (TOKENS)TK_OP));
    insertGrammerElement(tempRBooleanExpression1, createGrammerElement(false, (NONTERMINAL)booleanExpression));
    insertGrammerElement(tempRBooleanExpression1, createGrammerElement(true, (TOKENS)TK_CL));
    insertGrammerElement(tempRBooleanExpression1, createGrammerElement(false, (NONTERMINAL)logicalOp));
    insertGrammerElement(tempRBooleanExpression1, createGrammerElement(true, (TOKENS)TK_OP));
    insertGrammerElement(tempRBooleanExpression1, createGrammerElement(false, (NONTERMINAL)booleanExpression));
    insertGrammerElement(tempRBooleanExpression1, createGrammerElement(true, (TOKENS)TK_CL));
    insertRHS(tempRHBooleanExpression, tempRBooleanExpression1);

    RHS* tempRBooleanExpression2 = createRHSNode();
    insertGrammerElement(tempRBooleanExpression2, createGrammerElement(false, (NONTERMINAL)var));
    insertGrammerElement(tempRBooleanExpression2, createGrammerElement(false, (NONTERMINAL)relationalOp));
    insertGrammerElement(tempRBooleanExpression2, createGrammerElement(false, (NONTERMINAL)var));
    insertRHS(tempRHBooleanExpression, tempRBooleanExpression2);

    RHS* tempRBooleanExpression3 = createRHSNode();
    insertGrammerElement(tempRBooleanExpression3, createGrammerElement(true, (TOKENS)TK_NOT));
    insertGrammerElement(tempRBooleanExpression3, createGrammerElement(true, (TOKENS)TK_OP));
    insertGrammerElement(tempRBooleanExpression3, createGrammerElement(false, (NONTERMINAL)booleanExpression));
    insertGrammerElement(tempRBooleanExpression3, createGrammerElement(true, (TOKENS)TK_CL));
    insertRHS(tempRHBooleanExpression, tempRBooleanExpression3);

    insertInHashTable(booleanExpression, tempRHBooleanExpression);

    // <var>===> <SingleOrRecId> | TK_NUM | TK_RNUM
    RHSHead* tempRHVar = createRHSHead();

    RHS* tempRVar1 = createRHSNode();
    insertGrammerElement(tempRVar1, createGrammerElement(false, (NONTERMINAL)singleOrRecId));
    insertRHS(tempRHVar, tempRVar1);

    RHS* tempRVar2 = createRHSNode();
    insertGrammerElement(tempRVar2, createGrammerElement(true, (TOKENS)TK_NUM));
    insertRHS(tempRHVar, tempRVar2);

    RHS* tempRVar3 = createRHSNode();
    insertGrammerElement(tempRVar3, createGrammerElement(true, (TOKENS)TK_RNUM));
    insertRHS(tempRHVar, tempRVar3);

    insertInHashTable(var, tempRHVar);

    // <logicalOp>===>TK_AND | TK_OR
    RHSHead* tempRHLogicalOp = createRHSHead();

    RHS* tempRLogicalOp1 = createRHSNode();
    insertGrammerElement(tempRLogicalOp1, createGrammerElement(true, (TOKENS)TK_AND));
    insertRHS(tempRHLogicalOp, tempRLogicalOp1);

    RHS* tempRLogicalOp2 = createRHSNode();
    insertGrammerElement(tempRLogicalOp2, createGrammerElement(true, (TOKENS)TK_OR));
    insertRHS(tempRHLogicalOp, tempRLogicalOp2);

    insertInHashTable(logicalOp, tempRHLogicalOp);

    // <relationalOp>===> TK_LT | TK_LE | TK_EQ |TK_GT | TK_GE | TK_NE
    RHSHead* tempRHRelationalOp = createRHSHead();

    RHS* tempRRelationalOp1 = createRHSNode();
    insertGrammerElement(tempRRelationalOp1, createGrammerElement(true, (TOKENS)TK_LT));
    insertRHS(tempRHRelationalOp, tempRRelationalOp1);

    RHS* tempRRelationalOp2 = createRHSNode();
    insertGrammerElement(tempRRelationalOp2, createGrammerElement(true, (TOKENS)TK_LE));
    insertRHS(tempRHRelationalOp, tempRRelationalOp2);

    RHS* tempRRelationalOp3 = createRHSNode();
    insertGrammerElement(tempRRelationalOp3, createGrammerElement(true, (TOKENS)TK_EQ));
    insertRHS(tempRHRelationalOp, tempRRelationalOp3);

    RHS* tempRRelationalOp4 = createRHSNode();
    insertGrammerElement(tempRRelationalOp4, createGrammerElement(true, (TOKENS)TK_GT));
    insertRHS(tempRHRelationalOp, tempRRelationalOp4);

    RHS* tempRRelationalOp5 = createRHSNode();
    insertGrammerElement(tempRRelationalOp5, createGrammerElement(true, (TOKENS)TK_GE));
    insertRHS(tempRHRelationalOp, tempRRelationalOp5);

    RHS* tempRRelationalOp6 = createRHSNode();
    insertGrammerElement(tempRRelationalOp6, createGrammerElement(true, (TOKENS)TK_NE));
    insertRHS(tempRHRelationalOp, tempRRelationalOp6);

    insertInHashTable(relationalOp, tempRHRelationalOp);

    // <returnStmt>===>TK_RETURN <optionalReturn> TK_SEM
    RHSHead* tempRHReturnStmt = createRHSHead();

    RHS* tempRReturnStmt = createRHSNode();
    insertGrammerElement(tempRReturnStmt, createGrammerElement(true, (TOKENS)TK_RETURN));
    insertGrammerElement(tempRReturnStmt, createGrammerElement(false, (NONTERMINAL)optionalReturn));
    insertGrammerElement(tempRReturnStmt, createGrammerElement(true, (TOKENS)TK_SEM));
    insertRHS(tempRHReturnStmt, tempRReturnStmt);

    insertInHashTable(returnStmt, tempRHReturnStmt);

    // <optionalReturn>===>TK_SQL <idList> TK_SQR | ε
    RHSHead* tempRHOptionalReturn = createRHSHead();

    RHS* tempROptionalReturn1 = createRHSNode();
    insertGrammerElement(tempROptionalReturn1, createGrammerElement(true, (TOKENS)TK_SQL));
    insertGrammerElement(tempROptionalReturn1, createGrammerElement(false, (NONTERMINAL)idList));
    insertGrammerElement(tempROptionalReturn1, createGrammerElement(true, (TOKENS)TK_SQR));
    insertRHS(tempRHOptionalReturn, tempROptionalReturn1);

    RHS* tempROptionalReturn2 = createRHSNode(); // ε production
    insertRHS(tempRHOptionalReturn, tempROptionalReturn2);

    insertInHashTable(optionalReturn, tempRHOptionalReturn);

    // <idList>===> TK_ID <more_ids>
    RHSHead* tempRHIdList = createRHSHead();

    RHS* tempRIdList = createRHSNode();
    insertGrammerElement(tempRIdList, createGrammerElement(true, (TOKENS)TK_ID));
    insertGrammerElement(tempRIdList, createGrammerElement(false, (NONTERMINAL)more_ids));
    insertRHS(tempRHIdList, tempRIdList);

    insertInHashTable(idList, tempRHIdList);

    // <more_ids>===> TK_COMMA <idList> | ε
    RHSHead* tempRHMoreIds = createRHSHead();

    RHS* tempRMoreIds1 = createRHSNode();
    insertGrammerElement(tempRMoreIds1, createGrammerElement(true, (TOKENS)TK_COMMA));
    insertGrammerElement(tempRMoreIds1, createGrammerElement(false, (NONTERMINAL)idList));
    insertRHS(tempRHMoreIds, tempRMoreIds1);

    RHS* tempRMoreIds2 = createRHSNode(); // ε production
    insertRHS(tempRHMoreIds, tempRMoreIds2);

    insertInHashTable(more_ids, tempRHMoreIds);

    // <definetypestmt>===>TK_DEFINETYPE <A> TK_RUID TK_AS TK_RUID
    RHSHead* tempRHDefineTypeStmt = createRHSHead();

    RHS* tempRDefineTypeStmt = createRHSNode();
    insertGrammerElement(tempRDefineTypeStmt, createGrammerElement(true, (TOKENS)TK_DEFINETYPE));
    insertGrammerElement(tempRDefineTypeStmt, createGrammerElement(false, (NONTERMINAL)A));
    insertGrammerElement(tempRDefineTypeStmt, createGrammerElement(true, (TOKENS)TK_RUID));
    insertGrammerElement(tempRDefineTypeStmt, createGrammerElement(true, (TOKENS)TK_AS));
    insertGrammerElement(tempRDefineTypeStmt, createGrammerElement(true, (TOKENS)TK_RUID));
    insertRHS(tempRHDefineTypeStmt, tempRDefineTypeStmt);

    insertInHashTable(definetypestmt, tempRHDefineTypeStmt);

    // <A>==>TK_RECORD | TK_UNION
    RHSHead* tempRHA = createRHSHead();

    RHS* tempRA1 = createRHSNode();
    insertGrammerElement(tempRA1, createGrammerElement(true, (TOKENS)TK_RECORD));
    insertRHS(tempRHA, tempRA1);

    RHS* tempRA2 = createRHSNode();
    insertGrammerElement(tempRA2, createGrammerElement(true, (TOKENS)TK_UNION));
    insertRHS(tempRHA, tempRA2);

    insertInHashTable(A, tempRHA);

}

TerminalNode* FIRST[GRAMMER_TABLE_SIZE] = { NULL };
TerminalNode* FOLLOW[GRAMMER_TABLE_SIZE] = { NULL };

void initFF(){
    for(int i = 0; i < GRAMMER_TABLE_SIZE; i++){
        FIRST[i] = NULL;
        FOLLOW[i] = NULL;
    }
}

bool isFIRST(NONTERMINAL nt, TOKENS t){
    TerminalNode* curr = FIRST[nt];
    while(curr != NULL){
        if(curr->terminal == t) return true;
        curr = curr->next;
    }
    return false;
}

bool isFOLLOW(NONTERMINAL nt, TOKENS t){
    TerminalNode* curr = FOLLOW[nt];
    while(curr != NULL){
        if(curr->terminal == t) return true;
        curr = curr->next;
    }
    return false;
}

TerminalNode* createTerminalNode(int t){
    TerminalNode* new = (TerminalNode*)malloc(sizeof(TerminalNode));
    new->next = NULL;
    new->terminal = t;
    return new;
}

void insertFirstFollow(TerminalNode** FF, NONTERMINAL nt, int terminal){
    TerminalNode* new = createTerminalNode(terminal);
    if(FF[nt] != NULL)new->next = FF[nt];
    FF[nt] = new;
}

void insertFollow(NONTERMINAL nt, int terminal){
    TerminalNode* new = createTerminalNode(terminal);
    if(FOLLOW[nt]) new->next = FOLLOW[nt];
    FOLLOW[nt] = new;
}

TerminalNode* ComputeFirst(NONTERMINAL nt){
    if(Grammer[nt] == NULL) return NULL;
    if(FIRST[nt] != NULL) return FIRST[nt];
    RHS* rhsCurr = Grammer[nt]->first;
    while(rhsCurr != NULL){
        GrammerElement* geCurr = rhsCurr->first;
        if(geCurr == NULL) {
            insertFirstFollow(FIRST, nt, -1);
            rhsCurr = rhsCurr->next;
            continue;
        }
        bool epsilonFound = true;
        while (epsilonFound == true){
            epsilonFound = false;
            if(geCurr->isTerminal == true) insertFirstFollow(FIRST, nt, geCurr->TNT.Terminal);
            else {
                TerminalNode* toCopy = ComputeFirst(geCurr->TNT.NonTerminal);
                while (toCopy != NULL && geCurr != NULL){
                    if(toCopy->terminal != -1) insertFirstFollow(FIRST, nt, toCopy->terminal);
                    else{
                        epsilonFound = true;
                        geCurr = geCurr->next;
                    }
                    toCopy = toCopy->next;
                }         
            }
        }
        rhsCurr = rhsCurr->next;
    }
    return FIRST[nt];
}

void initialiseFirst(){
    initFF();
    for(int i = 0; i < GRAMMER_TABLE_SIZE; i++) ComputeFirst((NONTERMINAL)i);
}

void initialiseNTCOUNT(){
    for(int i = 0; i < GRAMMER_TABLE_SIZE; i++){
        RHS* curr = Grammer[i]->first;
        while(curr != NULL){
            GrammerElement* geCurr = curr->first;
            while(geCurr != NULL){
                if(geCurr->isTerminal == false) ntCount[geCurr->TNT.NonTerminal]++;
                geCurr = geCurr->next;
            }
            curr = curr->next;
        }
    }
}

TerminalNode* ComputeFollow(NONTERMINAL nt, GrammerElement* geCurr, NONTERMINAL lhs){
    // printf("debug %s ", nonterminalToString((NONTERMINAL)geCurr->TNT.NonTerminal));
    if((int)lhs == (int)nt) {
        return FOLLOW[nt];
    }
    if(ntCount[nt] == 0) return FOLLOW[nt];
    ntCount[nt]--;
    // if(geCurr->next == NULL) return FOLLOW[nt];
    if(geCurr->next->isTerminal == true) insertFollow(geCurr->TNT.NonTerminal, geCurr->next->TNT.Terminal);
    else {
        bool isEpsilon = true;
        GrammerElement* firstGeCurr = geCurr->next;
        while(isEpsilon == true && firstGeCurr != NULL){
            isEpsilon = false;
            if(firstGeCurr->isTerminal == true) insertFollow(nt, firstGeCurr->TNT.Terminal);
            else{
                TerminalNode* toCopy = ComputeFirst(firstGeCurr->TNT.NonTerminal);
                // printf("debug %s ", nonterminalToString((NONTERMINAL)geCurr->TNT.NonTerminal));
                while (toCopy != NULL){
                    // printf(" DEBUG ");
                    if(toCopy->terminal != -1) insertFollow(geCurr->TNT.NonTerminal, toCopy->terminal);
                    else {
                        //if has epsilon
                        isEpsilon = true;
                        firstGeCurr = firstGeCurr->next;
                    }
                    toCopy = toCopy->next;
                }
            }
        }
        if(isEpsilon == true){
                // printf("debug %s ", nonterminalToString((NONTERMINAL)geCurr->TNT.NonTerminal));

            TerminalNode* toCopy = ComputeFollow(lhs, geCurr, lhs);
            while (toCopy != NULL){
                insertFollow(geCurr->TNT.NonTerminal, toCopy->terminal);
                toCopy = toCopy->next;
            }
        }
    }
    return FOLLOW[nt];
}

void initialiseFollow(){
    initialiseNTCOUNT();
    insertFollow(program, dollar);
    for(int i = 0; i < GRAMMER_TABLE_SIZE; i++){
        RHS* curr = Grammer[i]->first;
        while(curr != NULL){
            GrammerElement* geCurr = curr->first;
            while(geCurr != NULL){
                // printf("debug %s ", nonterminalToString((NONTERMINAL)geCurr->TNT.NonTerminal));

                // printf(" debug "); COMPUTEFOLLOW ERORRROROR
                if(geCurr->isTerminal == true);
                else if(geCurr->next == NULL){

                    TerminalNode* toCopy = ComputeFollow((NONTERMINAL)i, geCurr, (NONTERMINAL)i);
                    // printf(" debug NT = %s ", tokenToString((TOKENS)FOLLOW[1]->terminal)); //COMPUTEFOLLOW ERORRROROR
                    while (toCopy != NULL){
                        insertFollow(geCurr->TNT.NonTerminal, toCopy->terminal);
                        toCopy = toCopy->next;
                    }
                }
                else if(geCurr->isTerminal == false){
                    ComputeFollow(geCurr->TNT.NonTerminal, geCurr, (NONTERMINAL)i);
                    // printf("debug %s ", nonterminalToString((NONTERMINAL)geCurr->TNT.NonTerminal));
                }
                geCurr = geCurr->next;
            }
            curr = curr->next;
        }
    }
}

void ComputeFirstAndFollowSets(){
    initialiseFirst();
    initialiseFollow();
    initialiseFollow();
}

RHS* synch;

RHS* PREDICTIVE_PARSE_TABLE[GRAMMER_TABLE_SIZE][TERMINALS_SIZE] = { NULL };

bool GrammerRuleHasFirst(GrammerElement* geCurrent, TOKENS t){
    if(geCurrent == NULL) return false;
    if(geCurrent->isTerminal == true) {
        if(geCurrent->TNT.Terminal == t) return true;
        else return false;
    }
    else if(geCurrent->isTerminal == false){
        if(isFIRST(geCurrent->TNT.NonTerminal, t) == true) return true;
        else if(isFIRST(geCurrent->TNT.NonTerminal, -1) == true) return GrammerRuleHasFirst(geCurrent->next, t);
        return false;
    }
    return false;
}

RHS* GrammerRule(NONTERMINAL nt, TOKENS t){
    // int size = Grammer[hash(nt)]->count;
    RHS *current = Grammer[hash(nt)]->first;
    GrammerElement *geCurrent = current->first;
    while(current != NULL){
        if(GrammerRuleHasFirst(geCurrent, t) == true) return current;
        current = current->next;
        geCurrent = current->first;
    }
    return NULL;
}

RHS* findEpsilonRule(RHSHead* rhsHead){
    RHS* curr = rhsHead->first;
    while(curr->count != 0) curr = curr->next;
    return curr;
}

void createParseTable(){
    synch = createRHSNode();
    for(int i = 0; i < GRAMMER_TABLE_SIZE; i++){ // iterate through all non terminals
        if(Grammer[i] == NULL) continue;
        TerminalNode* FirstCurr = FIRST[i];
        // printf(" %s ", nonterminalToString(i));
        while(FirstCurr != NULL){
            int Terminal = FirstCurr->terminal;
                // printf(" %s ", tokenToString(Terminal));
                // if(i == (NONTERMINAL)primitiveDatatype)
                // printf("%d", PREDICTIVE_PARSE_TABLE[i][Terminal] == NULL);
            if(Terminal != -1 && PREDICTIVE_PARSE_TABLE[i][Terminal] == NULL) {
                PREDICTIVE_PARSE_TABLE[i][Terminal] = GrammerRule(i, Terminal);
                // if(i == (NONTERMINAL)primitiveDatatype) printf("%d ", PREDICTIVE_PARSE_TABLE[i][Terminal]);
            }
            else if(Terminal == -1){
                TerminalNode* FollowCurr = FOLLOW[i];
                RHS* epsilonRule = findEpsilonRule(Grammer[i]);
                while(FollowCurr != NULL){
                    int Terminal = FollowCurr->terminal;
                    PREDICTIVE_PARSE_TABLE[i][Terminal] = epsilonRule;
                    FollowCurr = FollowCurr->next;
                }
            }
            FirstCurr = FirstCurr->next;
        }
    }
    for(int i = 0; i < GRAMMER_TABLE_SIZE; i++){
        TerminalNode* curr = FOLLOW[i];
        while(curr != NULL){
            int Terminal = curr->terminal;
            if(PREDICTIVE_PARSE_TABLE[i][Terminal] == NULL) PREDICTIVE_PARSE_TABLE[i][Terminal] = synch;
            curr = curr->next;
        }
    }
}

TreeNode* createTreeNode(GrammerElement* ge, int line, char* lex) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->ge = ge;
    node->firstChild = NULL;
    node->nextSibling = NULL;
    node->lineNumber = line;
    if(ge->isTerminal == true){
        for(int i = 0; i < strlen(lex); i++){
            node->lexeme[i] = lex[i];
            node->lexeme[i+1] = '\0'; 
        }
    }
    else{
        node->lexeme[0] = '-';
        node->lexeme[1] = '-';
        node->lexeme[2] = '-';
        node->lexeme[3] = '\0';
    }
    return node;
}

void addChild(TreeNode* parent, TreeNode* child) {
    if (parent->firstChild == NULL) {
        parent->firstChild = child;
    } else {
        TreeNode* temp = parent->firstChild;
        while (temp->nextSibling != NULL) {
            temp = temp->nextSibling;
        }
        temp->nextSibling = child;
    }
}

ParseStack* createParseStack(){
    ParseStack* stack = (ParseStack*)malloc(sizeof(ParseStack));
    stack->count=0;
    stack->first=NULL;
    return stack;
}

ParseStackElement* createParseStackElement(GrammerElement* ge, int line, char* lexeme){
    ParseStackElement* new = (ParseStackElement*)malloc(sizeof(ParseStackElement));
    new->nodeReference = createTreeNode(ge, line, lexeme);
    new->next = NULL;
    new->ge = ge;
}

void pushInStack(ParseStack* head, GrammerElement* ge, int line, char* lex){
    ParseStackElement* new = createParseStackElement(ge, line, lex);
    new->next = head->first;
    head->first = new;
    head->count++;
}

TreeNode* popFromStack(ParseStack* head){
    TreeNode* popcorned = head->first->nodeReference;
    ParseStackElement* temp = head->first;
    head->first = head->first->next;
    head->count--;
    return popcorned;
}

GrammerElement* peekInStack(ParseStack* head){
    return head->first->ge;
}

ParseStackElement* peekStack(ParseStack* head){
    return head->first;
}

void insertRuleInStack(ParseStack* head, RHS* rule, char* lexeme, int line){
    TreeNode* node = popFromStack(head);
    // printf("%s ", nonterminalToString(node->ge->TNT.NonTerminal));
    ParseStack* ReverseStack = createParseStack();
    GrammerElement* curr = rule->first;
    if(curr == NULL){
        node->firstChild = createTreeNode(createGrammerElement(true, epsilon), line, lexeme);
    }
    else{
        pushInStack(ReverseStack, curr, line, lexeme);
        node->firstChild = ReverseStack->first->nodeReference;
        node = node->firstChild;
        curr = curr->next;
    }
    while(curr != NULL){
        pushInStack(ReverseStack, curr, line, lexeme);
        node->nextSibling = ReverseStack->first->nodeReference;
        node = node->nextSibling;
        curr = curr->next;
    }
    while(ReverseStack->count > 0){
        ParseStackElement* stackElement = peekStack(ReverseStack);
        popFromStack(ReverseStack);
        // pushInStack(head, curr);
        stackElement->next = head->first;
        head->first = stackElement;
        head->count++;
    }
}

void printTree(TreeNode* parent, TreeNode* root, FILE* fileptr){
    if(root == NULL) return;
    printTree(root, root->firstChild, fileptr);
    
    fprintf(fileptr, "%s %d ", root->lexeme, root->lineNumber);
    if(root->ge->isTerminal) fprintf(fileptr, "%s ", tokenToString(root->ge->TNT.Terminal));
    else fprintf(fileptr, "--- ");
    if(root->ge->isTerminal == true){
        if(root->ge->TNT.Terminal == TK_INT || root->ge->TNT.Terminal == TK_REAL) fprintf(fileptr, "%s ", root->lexeme);
        else{
            fprintf(fileptr, "--- ");
        }
    }
    else fprintf(fileptr, "%s ", root->lexeme);
    fprintf(fileptr, "%s ", nonterminalToString(parent->ge->TNT.NonTerminal));
    if(root->ge->isTerminal == true) fprintf(fileptr, "Yes ");
    else fprintf(fileptr, "No ");
    if(root->ge->isTerminal == true) fprintf(fileptr, "---\n");
    else fprintf(fileptr, "%s\n", nonterminalToString(root->ge->TNT.NonTerminal));
    printTree(parent, root->nextSibling, fileptr);
}

void printParseTree(TreeNode* root, char* filename) {
    FILE* file = fopen(filename, "wb");
    // if (root == NULL)
    //     return;
    printTree(root, root, file);
    // printParseTree(root->firstChild);
    // if(root->ge->isTerminal == true){
    //     printf("%s \n", tokenToString(root->ge->TNT.Terminal));
    // }
    // else printf("%s \n", nonterminalToString(root->ge->TNT.NonTerminal));
    // printParseTree(root->nextSibling);
    fclose(file);
}

void parseInputSourceCode(char *testcaseFile, char* outputFile){
    char* outputFilename = addSuffix(testcaseFile, "lexerout.txt");
    // Tokenize(testcaseFile);
    FILE* file = fopen(outputFilename, "rb");
    intialiseGrammer();
    ComputeFirstAndFollowSets();

    // printf(" DEBUG ");
    // for(int i = 0; i < GRAMMER_TABLE_SIZE; i++){
    //     TerminalNode* curr = FIRST[i];
    //     printf("%s ===> ", nonterminalToString((NONTERMINAL)i));
    //     while(curr != NULL){
    //         printf("%s ", tokenToString(curr->terminal));
    //         curr = curr->next;
    //     }
    //     printf("\n");
    // }
    createParseTable();
    
    ParseStack* code = createParseStack();
    pushInStack(code, createGrammerElement(true, (int)dollar), 0, "---");
    pushInStack(code, createGrammerElement(false, (int)program), 0, "---");
    TreeNode* ROOT = code->first->nodeReference;

    char line[1000];

    // while (fgets(line, sizeof(line), file)) {
        int lineNo;
        char lexeme[31];
        char tokenstring[31];

    while (fgets(line, sizeof(line), file)) {
        // Parse each line to extract the line number, lexeme, and token name
        if (sscanf(line, "Line no. %d Lexeme %s Token %s", &lineNo, lexeme, tokenstring) == 3) {
            // Here you can store `entry` in an array or process it as needed
            // For demonstration, we just print the extracted data
            // printf("Line: %d Lexeme: %s Token: %s\n", lineNo, lexeme, tokenstring);
            // printf("%d", token);/
            // count++;
        }
        TOKENS token = getTokenFromString(tokenstring);
        // printf("%s ", tokenToString(token));
        GrammerElement* top = peekInStack(code);
        while(top->isTerminal == false){
            if(PREDICTIVE_PARSE_TABLE[top->TNT.NonTerminal][token] == synch) {
                popFromStack(code);
                // printf(" if ");
            }
            else if(PREDICTIVE_PARSE_TABLE[top->TNT.NonTerminal][token] != NULL) {
                insertRuleInStack(code, PREDICTIVE_PARSE_TABLE[top->TNT.NonTerminal][token], lexeme, lineNo);
                // printf(" elseif ");
            }
            else {
                // printf(" else ");
                // printf("token = %s ", tokenToString(token));
                // printf("%s ", nonterminalToString(top->TNT.NonTerminal));
                TOKENS exp = FIRST[top->TNT.NonTerminal]->next->terminal;
                printf("Line %d  Error: The token TK_SQL for lexeme %s  does not match with the expected token %s\n", lineNo, lexeme, tokenToString(exp));
                memset(tokenstring, 0, sizeof(tokenstring));
                break;
            }
            top = peekInStack(code);
        }
        memset(tokenstring, 0, sizeof(tokenstring));
        if (top->isTerminal == true && token == top->TNT.Terminal){
            popFromStack(code);
            top = peekInStack(code);
            continue;
        }
        if(top->isTerminal == true && token != top->TNT.Terminal){
            // handle this error
                // printf(" else ");
            printf("Line %d  Error: The token TK_SQL for lexeme %s  does not match with the expected token %s\n", lineNo, lexeme, tokenToString(top->TNT.Terminal));
            popFromStack(code);
            top = peekInStack(code);
            continue;
        }
    }
    printParseTree(ROOT, outputFile);
    // do{
        // c = (char)fgetc(file);
        // // printf("%c", c);
        // if(c == ' ') {
        //     spaceCount--;
        //     continue;
        // }
        // else if(spaceCount != 0) continue;
        // if (spaceCount == 0 && c != '\n') {
        //     currToken[currTokenSize++] = c;
        //     currToken[currTokenSize] = '\0';
        //     continue;
        // }
        // if(c == '\n'){
        //     spaceCount = 6;
        //     currTokenSize = 0;
        //     // printf("%s", currToken);
        //     TOKENS token = getTokenFromString(currToken);
        //     // printf("token = %s ", tokenToString(token));
        //     GrammerElement* top = peekInStack(code);
            // while(top->isTerminal == false){
            //     if(PREDICTIVE_PARSE_TABLE[top->TNT.NonTerminal][token] == synch) {
            //         popFromStack(code);
            //         // printf(" if ");
            //     }
            //     else if(PREDICTIVE_PARSE_TABLE[top->TNT.NonTerminal][token] != NULL) {
            //         insertRuleInStack(code, PREDICTIVE_PARSE_TABLE[top->TNT.NonTerminal][token]);
            //         // printf(" elseif ");
            //     }
            //     else {
            //         // printf(" else ");
            //         // printf("token = %s ", tokenToString(token));
            //         // printf("%s ", nonterminalToString(top->TNT.NonTerminal));
            //         break;
            //     }
            //     top = peekInStack(code);
            // }
            // if (top->isTerminal == true && token == top->TNT.Terminal){
            //     popFromStack(code);
            //     top = peekInStack(code);
            //     continue;
            // }
            // if(top->isTerminal == true && token != top->TNT.Terminal){
            //     // handle this error
            //     popFromStack(code);
            //     top = peekInStack(code);
            //     continue;
            // }
        // }
    // }while(c != EOF);
    fclose(file);
}

// int main(int argc, char const *argv[]){
//     // intialiseGrammer();
//     // ComputeFirstAndFollowSets();

//     // // printf(" DEBUG ");
//     // // for(int i = 0; i < GRAMMER_TABLE_SIZE; i++){
//     // //     TerminalNode* curr = FIRST[i];
//     // //     printf("%s ===> ", nonterminalToString((NONTERMINAL)i));
//     // //     while(curr != NULL){
//     // //         printf("%s ", tokenToString(curr->terminal));
//     // //         curr = curr->next;
//     // //     }
//     // //     printf("\n");
//     // // }
//     // createParseTable();
//     // // createParseTable();

//     // // for(int i = 0; i < GRAMMER_TABLE_SIZE; i++){
//     // //         printf("\n");
//     // //     printf("%s ",nonterminalToString((NONTERMINAL)i));
//     // //     for(int ii = 0; ii < TERMINALS_SIZE; ii++){
//     // //         // printf("%s ", tokenToString((TOKENS)ii));
//     // //         if(PREDICTIVE_PARSE_TABLE[i][ii] == NULL) printf("error ");
//     // //         else if(PREDICTIVE_PARSE_TABLE[i][ii] == synch) {
//     // //             printf("SYNCH = %s ", tokenToString((TOKENS)ii));
//     // //         }
//     // //         else {
//     // //             printf("%s ", tokenToString((TOKENS)ii));
//     // //             // printf("rule ");
//     // //             // RHS* rule = PREDICTIVE_PARSE_TABLE[i][ii];
//     // //             // GrammerElement* ge = rule->first;
//     // //             // while(ge != NULL){
//     // //             //     if(ge->isTerminal == true){
//     // //             //         printf("%s ", tokenToString((TOKENS)ge->TNT.Terminal));
//     // //             //     }
//     // //             //     else printf("%s ", nonterminalToString((NONTERMINAL)ge->TNT.NonTerminal));
//     // //             //     ge = ge->next;
//     // //             // }
//     // //         }
//     // //     }
//     // //         printf("\n");
//     // // }
//     // FILE* file = fopen("lexer_output.txt", "rb");
//     // char c;
//     // int spaceCount = 6;
//     // char currToken[16];
//     // int currTokenSize = 0;
//     // ParseStack* code = createParseStack();
//     // pushInStack(code, createGrammerElement(true, (int)dollar));
//     // pushInStack(code, createGrammerElement(false, (int)program));
//     // TreeNode* ROOT = code->first->nodeReference;
//     // do{
//     //     c = (char)fgetc(file);
//     //     // printf("%c", c);
//     //     if(c == ' ') {
//     //         spaceCount--;
//     //         continue;
//     //     }
//     //     else if(spaceCount != 0) continue;
//     //     if (spaceCount == 0 && c != '\n') {
//     //         currToken[currTokenSize++] = c;
//     //         currToken[currTokenSize] = '\0';
//     //         continue;
//     //     }
//     //     if(c == '\n'){
//     //         spaceCount = 6;
//     //         currTokenSize = 0;
//     //         TOKENS token = getTokenFromString(currToken);
//     //         printf("token = %s ", tokenToString(token));
//     //         GrammerElement* top = peekInStack(code);
//     //         while(top->isTerminal == false){
//     //             if(PREDICTIVE_PARSE_TABLE[top->TNT.NonTerminal][token] == synch) {
//     //                 popFromStack(code);
//     //                 printf(" if ");
//     //             }
//     //             else if(PREDICTIVE_PARSE_TABLE[top->TNT.NonTerminal][token] != NULL) {
//     //                 insertRuleInStack(code, PREDICTIVE_PARSE_TABLE[top->TNT.NonTerminal][token]);
//     //                 printf(" elseif ");
//     //             }
//     //             else {
//     //                 printf(" else ");
//     //                 // printf("token = %s ", tokenToString(token));
//     //                 printf("%s ", nonterminalToString(top->TNT.NonTerminal));
//     //                 break;
//     //             }
//     //             top = peekInStack(code);
//     //         }
//     //         if (top->isTerminal == true && token == top->TNT.Terminal){
//     //             popFromStack(code);
//     //             top = peekInStack(code);
//     //         }
//     //     }
//     // }while(c != EOF);
//     // fclose(file);
//     parseInputSourceCode("t2.txt");
//     // printTree(ROOT, 0);

//     return 0;
// }
