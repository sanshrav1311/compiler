/*
Group Number 16
Sanshrav Arora 2021A7PS2690P
Ajey Malik 2021A7PS2542P
Nachiketh S Shastry 2021A7PS2686P
Rikhil Gupta 2021A7PS0533P
Rishabh Sahni 2021A7PS1630P
*/

#include "parserDef.h"

int hash(NONTERMINAL nt);

// terminal - non terminal to string funcitons
const char* nonterminalToString(NONTERMINAL nonterminal);
TOKENS getTokenFromString(const char *input);
const char* tokenToString(TOKENS token);

// grammer Data structure functions and initialisation
RHSHead* createRHSHead();
RHS* createRHSNode();
GrammerElement* createGrammerElement(bool terminalORnot, int enumVal);
void insertRHS(RHSHead* head, RHS* rhs);
void insertGrammerElement(RHS* head, GrammerElement* ge);
void insertInHashTable(NONTERMINAL nt, RHSHead* rhsHead);
void intialiseGrammer();

// first and follow functions
void initFF();
bool isFIRST(NONTERMINAL nt, TOKENS t);
bool isFOLLOW(NONTERMINAL nt, TOKENS t);
TerminalNode* createTerminalNode(int t);
void insertFirstFollow(TerminalNode** FF, NONTERMINAL nt, int terminal);
void insertFollow(NONTERMINAL nt, int terminal);
TerminalNode* ComputeFirst(NONTERMINAL nt);
void initialiseFirst();
void initialiseNTCOUNT();
TerminalNode* ComputeFollow(NONTERMINAL nt, GrammerElement* geCurr, NONTERMINAL lhs);
void initialiseFollow();
void ComputeFirstAndFollowSets();

// predictive parse table functions
bool GrammerRuleHasFirst(GrammerElement* geCurrent, TOKENS t);
RHS* GrammerRule(NONTERMINAL nt, TOKENS t);
RHS* findEpsilonRule(RHSHead* rhsHead);
void createParseTable();

//parse tree functions
TreeNode* createTreeNode(GrammerElement* ge, int line, char* lex);
void addChild(TreeNode* parent, TreeNode* child);

//parse stack functions
ParseStack* createParseStack();
ParseStackElement* createParseStackElement(GrammerElement* ge, int line, char* lexeme);
void pushInStack(ParseStack* head, GrammerElement* ge, int line, char* lex);
TreeNode* popFromStack(ParseStack* head);
GrammerElement* peekInStack(ParseStack* head);
ParseStackElement* peekStack(ParseStack* head);
void insertRuleInStack(ParseStack* head, RHS* rule, char* lexeme, int line);
void parseInputSourceCode(char *testcaseFile, char* outputfile);

//print treee
void printTree(TreeNode* parent, TreeNode* root, FILE* fileptr);
void printParseTree(TreeNode* root, char* filename);