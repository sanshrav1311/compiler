#include "lexerDef.h";

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
removeComments(char *testcaseFile, char *cleanFile);
char* addSuffix(const char* filename, const char* suffix);
void Tokenize(char* filename);
tokenInfo* createToken(TOKENS token);
void retract(int *index, long int steps);
void printToken(FILE* output_file, tokenInfo *token);
void printError(FILE* output_file);