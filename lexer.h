/*
Group Number 16
Sanshrav Arora 2021A7PS2690P
Ajey Malik 2021A7PS2542P
Nachiketh S Shastry 2021A7PS2686P
Rikhil Gupta 2021A7PS0533P
Rishabh Sahni 2021A7PS1630P
*/

#include "lexerDef.h"

// int state = 0;
// int lineNumber = 1;
// char currLexeme[21474837];
// int currLexemeSize = 0;
// tokenInfo* currToken;

// file read function
twinBuffer getStream(FILE *fp); 

// functions for lookup table
const char* tokenToString(TOKENS token);
TOKENS getTokenFromString(const char *input);
TOKENS getTokensFromLookup(const char* word);
int hashVal(char* s);
keyword* createKeyword(char* s);
void insertLookupTable(char* s);
bool findLookupTable(char* s);
void initializeLookupTable();

//tokenizing functions
void removeComments(char *testcaseFile, char *cleanFile);
char* addSuffix(const char* filename, const char* suffix);
void Tokenize(const char* filename);
tokenInfo* createToken(TOKENS token);
void retract(int *index, long int steps);
void printToken(FILE* output_file, tokenInfo *token);
void printError(FILE* output_file);

