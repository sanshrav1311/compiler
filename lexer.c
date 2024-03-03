#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
# define NO_OF_KEYWORDS 28
# define HASH_TABLE_SIZE 100000007

FILE *output_file; // Declare a file pointer

typedef enum TOKENS {TK_ERROR, TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM, TK_FUNID, TK_RUID, TK_WITH, TK_PARAMETERS, TK_END, TK_WHILE, TK_UNION, TK_ENDUNION, TK_DEFINETYPE, TK_AS, TK_TYPE, TK_MAIN, TK_GLOBAL, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_INPUT, TK_OUTPUT, TK_INT, TK_REAL, TK_COMMA, TK_SEM, TK_COLON, TK_DOT, TK_ENDWHILE, TK_OP, TK_CL, TK_IF, TK_THEN, TK_ENDIF, TK_READ, TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV, TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE, TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_EQ, TK_GT, TK_GE, TK_NE, dollar, nf, synch} TOKENS;
const char* tokenToString(TOKENS token) {
    switch(token) {
        case TK_ERROR: return "TK_ERROR";
        case TK_ASSIGNOP: return "TK_ASSIGNOP";
        case TK_COMMENT: return "TK_COMMENT";
        case TK_FIELDID: return "TK_FIELDID";
        case TK_ID: return "TK_ID";
        case TK_NUM: return "TK_NUM";
        case TK_RNUM: return "TK_RNUM";
        case TK_FUNID: return "TK_FUNID";
        case TK_RUID: return "TK_RUID";
        case TK_WITH: return "TK_WITH";
        case TK_PARAMETERS: return "TK_PARAMETERS";
        case TK_END: return "TK_END";
        case TK_WHILE: return "TK_WHILE";
        case TK_UNION: return "TK_UNION";
        case TK_ENDUNION: return "TK_ENDUNION";
        case TK_DEFINETYPE: return "TK_DEFINETYPE";
        case TK_AS: return "TK_AS";
        case TK_TYPE: return "TK_TYPE";
        case TK_MAIN: return "TK_MAIN";
        case TK_GLOBAL: return "TK_GLOBAL";
        case TK_PARAMETER: return "TK_PARAMETER";
        case TK_LIST: return "TK_LIST";
        case TK_SQL: return "TK_SQL";
        case TK_SQR: return "TK_SQR";
        case TK_INPUT: return "TK_INPUT";
        case TK_OUTPUT: return "TK_OUTPUT";
        case TK_INT: return "TK_INT";
        case TK_REAL: return "TK_REAL";
        case TK_COMMA: return "TK_COMMA";
        case TK_SEM: return "TK_SEM";
        case TK_COLON: return "TK_COLON";
        case TK_DOT: return "TK_DOT";
        case TK_ENDWHILE: return "TK_ENDWHILE";
        case TK_OP: return "TK_OP";
        case TK_CL: return "TK_CL";
        case TK_IF: return "TK_IF";
        case TK_THEN: return "TK_THEN";
        case TK_ENDIF: return "TK_ENDIF";
        case TK_READ: return "TK_READ";
        case TK_WRITE: return "TK_WRITE";
        case TK_RETURN: return "TK_RETURN";
        case TK_PLUS: return "TK_PLUS";
        case TK_MINUS: return "TK_MINUS";
        case TK_MUL: return "TK_MUL";
        case TK_DIV: return "TK_DIV";
        case TK_CALL: return "TK_CALL";
        case TK_RECORD: return "TK_RECORD";
        case TK_ENDRECORD: return "TK_ENDRECORD";
        case TK_ELSE: return "TK_ELSE";
        case TK_AND: return "TK_AND";
        case TK_OR: return "TK_OR";
        case TK_NOT: return "TK_NOT";
        case TK_LT: return "TK_LT";
        case TK_LE: return "TK_LE";
        case TK_EQ: return "TK_EQ";
        case TK_GT: return "TK_GT";
        case TK_GE: return "TK_GE";
        case TK_NE: return "TK_NE";
        default: return "Unknown";
    }
}

TOKENS getTokenFromString(const char *input) {
    if (strcmp(input, "with") == 0) return TK_WITH;
    else if (strcmp(input, "parameters") == 0) return TK_PARAMETERS;
    else if (strcmp(input, "end") == 0) return TK_END;
    else if (strcmp(input, "while") == 0) return TK_WHILE;
    else if (strcmp(input, "union") == 0) return TK_UNION;
    else if (strcmp(input, "endunion") == 0) return TK_ENDUNION;
    else if (strcmp(input, "definetype") == 0) return TK_DEFINETYPE;
    else if (strcmp(input, "as") == 0) return TK_AS;
    else if (strcmp(input, "type") == 0) return TK_TYPE;
    else if (strcmp(input, "_main") == 0) return TK_MAIN;
    else if (strcmp(input, "global") == 0) return TK_GLOBAL;
    else if (strcmp(input, "parameter") == 0) return TK_PARAMETER;
    else if (strcmp(input, "list") == 0) return TK_LIST;
    else if (strcmp(input, "input") == 0) return TK_INPUT;
    else if (strcmp(input, "output") == 0) return TK_OUTPUT;
    else if (strcmp(input, "int") == 0) return TK_INT;
    else if (strcmp(input, "real") == 0) return TK_REAL;
    else if (strcmp(input, "endwhile") == 0) return TK_ENDWHILE;
    else if (strcmp(input, "if") == 0) return TK_IF;
    else if (strcmp(input, "then") == 0) return TK_THEN;
    else if (strcmp(input, "endif") == 0) return TK_ENDIF;
    else if (strcmp(input, "read") == 0) return TK_READ;
    else if (strcmp(input, "write") == 0) return TK_WRITE;
    else if (strcmp(input, "return") == 0) return TK_RETURN;
    else if (strcmp(input, "call") == 0) return TK_CALL;
    else if (strcmp(input, "record") == 0) return TK_RECORD;
    else if (strcmp(input, "endrecord") == 0) return TK_ENDRECORD;
    else if (strcmp(input, "else") == 0) return TK_ELSE;
    else return nf;
}

typedef struct keyword{
    char key[31];
    struct keyword* next;
}keyword;

keyword* LookupTable[HASH_TABLE_SIZE] = { NULL };

int hashVal(char* s){
    long long p = 27, m = HASH_TABLE_SIZE;
    long long hash = 0;
   
    int n = strlen(s);
    for(int i = 0; i < n; i++) {
        int valAdd;
        if(s[i] == '_') valAdd = 27;
        else valAdd = s[i] - 'a';
        hash = (hash*27 + valAdd)%m;
    }
    return hash%m;
}

keyword* createKeyword(char* s){
    keyword* newKeyword = (keyword*) malloc(sizeof(keyword));
    for(int i = 0; i < strlen(s); i++){
        newKeyword->key[i] = s[i];
    }
    newKeyword->key[strlen(s)] = '\0';
    newKeyword->next = NULL;
}

void insertLookupTable(char* s){
    int index;
    index = hashVal(s);
    if(LookupTable[index] == NULL){
        LookupTable[index] = createKeyword(s);
    }
    else {
        keyword* new = createKeyword(s);
        new->next = LookupTable[index];
        LookupTable[index]->next = new;
    }
}
bool findLookupTable(char* s){
    int index=hashVal(s);
    keyword* current = LookupTable[index];
    while(current != NULL){
        if(strcmp(current->key, s) == 0) return true;
        current = current->next;
    }
    return false;
}

void initializeLookupTable(){
    char* keywords[] = {"with", "parameters", "end", "while", "union", "endunion", "definetype", "as", "type", "_main", "global", "parameter", "list", "input", "output", "int", "real", "endwhile", "if", "then", "endif", "read", "write", "return", "call", "record", "endrecord", "else"};
    for(int i = 0; i < NO_OF_KEYWORDS; i++){
        insertLookupTable(keywords[i]);
    }
}

typedef struct TOKEN {
    int line;
    char lexeme[21474837]; //lexeme[21];
    TOKENS tokenName;
} TOKEN;

TOKEN* currToken;

int state = 0;
int lineNumber = 1;
char currLexeme[21474837];
int currLexemeSize = 0;

TOKEN* createToken(TOKENS token){
    TOKEN* t = (TOKEN*)malloc(sizeof(TOKEN));
    t->line = lineNumber;
    t->tokenName = token;
    for(int i = 0; i < currLexemeSize; i++){
        t->lexeme[i] = currLexeme[i];
    }
    return t;
}

void retract(FILE* file_ptr, long int steps){
    fseek(file_ptr, steps * (long int)-1, SEEK_CUR);
    currLexemeSize--;
    currLexeme[currLexemeSize] = '\0';
}

void printToken(TOKEN *token){ // lineNumber lexeme token
    if(token->tokenName == TK_ID && currLexemeSize > 20){
        fprintf(output_file,"%d too long %s\n", token->line, tokenToString(TK_ERROR));
    }
    else if(token->tokenName == TK_FIELDID && currLexemeSize > 30){
        fprintf(output_file,"%d too long %s\n", token->line, tokenToString(TK_ERROR));
    }
    else if(currLexemeSize > 0){
        fprintf(output_file,"%d ", token->line);
        for(int i = 0; i < currLexemeSize; i++){
            fprintf(output_file,"%c", token->lexeme[i]);
        }
        fprintf(output_file," %s\n", tokenToString(token->tokenName));
    }
    state = 0;
    currLexemeSize = 0;
    free(token);
}

void printError(){
    currToken = createToken(TK_ERROR);
    printToken(currToken);
}

int main(int argc, char const *argv[])
{
    output_file = fopen("lexer_output.txt", "wb");
    initializeLookupTable();
    FILE *file;
    file = fopen(argv[1], "rb");
    char c;
    TOKEN *currToken = (TOKEN*) malloc(sizeof(TOKEN));
    do 
    {
        c = (char)fgetc(file);
        currLexeme[currLexemeSize] = c;
        currLexemeSize++;
        currLexeme[currLexemeSize] = '\0';
        switch(state){
            case 0:
                switch(c){
                    case 32:
                        state = 0;
                        currLexemeSize = 0;
                        break;
                    case '\t':
                        state = 0;
                        currLexemeSize = 0;
                        break;
                    case '\n':
                        state = 0;
                        currLexemeSize = 0;
                        lineNumber++;
                        break;
                    case '[':
                        currToken = createToken(TK_SQL);
                        printToken(currToken);
                        break;
                    case ']':
                        currToken = createToken(TK_SQR);
                        printToken(currToken);
                        break;
                    case ',':
                        currToken = createToken(TK_COMMA);
                        printToken(currToken);
                        break;
                    case ';':
                        currToken = createToken(TK_SEM);
                        printToken(currToken);
                        break;
                    case ':':
                        currToken = createToken(TK_COLON);
                        printToken(currToken);
                        break;
                    case '.':
                        currToken = createToken(TK_DOT);
                        printToken(currToken);
                        break;
                    case '(':
                        currToken = createToken(TK_OP);
                        printToken(currToken);
                        break;
                    case ')':
                        currToken = createToken(TK_CL);
                        printToken(currToken);
                        break;
                    case '+':
                        currToken = createToken(TK_PLUS);
                        printToken(currToken);
                        break;
                    case '-':
                        currToken = createToken(TK_MINUS);
                        printToken(currToken);
                        break;
                    case '*':
                        currToken = createToken(TK_MUL);
                        printToken(currToken);
                        break;
                    case '/':
                        currToken = createToken(TK_DIV);
                        printToken(currToken);
                        break;
                    case '~':
                        currToken = createToken(TK_NOT);
                        printToken(currToken);
                        break;
                    case '=':
                        state = 1;
                        break;
                    case '!':
                        state = 2;
                        break;
                    case '>':
                        state = 3;
                        break;
                    case '<':
                        state = 4;
                        break;
                    case '#':
                        state = 5;
                        break;
                    case '%':
                        state = 6;
                        break;
                    case '@':
                        state = 7;
                        break;
                    case '&':
                        state = 8;
                        break;
                    case '_':
                        state = 9;
                        break;
                    default:
                        if(c >= '0' && c <= '9') {
                            state = 10;
                        }
                        else if(c >= 'a' && c <= 'z') {
                            if(c >= 'b' && c <= 'd') state = 11;
                            else state = 12;
                        }
                        else {
                            printError();
                            currLexemeSize = 0;
                        }
                        break;
                }
                break;
                case 1:
                    if(c == '='){
                        currToken = createToken(TK_EQ);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    else{
                        retract(file, 1);
                        printError();
                        currLexemeSize = 0;
                    }
                    break;
                case 2:
                    if(c == '='){
                        currToken = createToken(TK_NE);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    else{
                        retract(file, 1);
                        printError();
                        currLexemeSize = 0;
                    }
                    break;
                case 3:
                    if(c == '='){
                        currToken = createToken(TK_NE);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    else{
                        retract(file, 1);
                        currToken = createToken(TK_NE);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 4:
                    if(c == '='){
                        currToken = createToken(TK_LE);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    else if(c != '-'){
                        retract(file, 1);
                        printError();
                        currLexemeSize = 0;
                    }
                    else if(c == '-') state = 13;
                    break;
                case 5:
                    if(c >= 'a' && c <= 'z') state = 14;
                    else{
                        retract(file, 1);
                        printError();
                        currLexemeSize = 0;
                    }
                    break;
                case 6:
                    if(c != '\n') state = 6;
                    else if(c == '\n'){
                        currToken = createToken(TK_COMMENT);
                        currLexemeSize--;
                        printToken(currToken);
                        currLexemeSize = 0;
                        state = 0;
                        lineNumber++;
                    }
                    break;
                case 7:
                    if(c != '@'){
                        retract(file, 1);
                        printError();
                        currLexemeSize = 0;
                    }
                    else state = 15;
                    break;
                case 8:
                    if(c != '&'){
                        retract(file, 1);
                        printError();
                        currLexemeSize = 0;
                    }
                    else state = 16;
                    break;
                case 9:
                    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
                        state = 17;
                    }
                    else{
                        retract(file, 1);
                        printError();
                        currLexemeSize = 0;
                    }
                    break;
                case 10:
                    if(c == '.'){
                        state = 19;
                    }
                    else if(c < '0' || c > '9'){
                        retract(file, 1);
                        currToken = createToken(TK_NUM);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 11:
                    if(c >= 'a' && c <= 'z') state = 12;
                    else if(c >= '2' && c <= '7') state = 20;
                    else {
                        retract(file, 1);
                        TOKENS tempToken = TK_FIELDID;
                        if(findLookupTable(currLexeme)){
                            tempToken = getTokenFromString(currLexeme); 
                        }
                        if(tempToken == nf) tempToken = TK_FIELDID;
                        currToken = createToken(tempToken);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 12:
                    if(c < 'a' || c > 'z') {
                        retract(file, 1);
                        TOKENS tempToken = TK_FIELDID;
                        if(findLookupTable(currLexeme)){
                            tempToken = getTokenFromString(currLexeme); 
                        }
                        if(tempToken == nf) tempToken = TK_FIELDID;
                        currToken = createToken(tempToken);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 13:
                    if(c == '-') state = 21;
                    else{
                        retract(file, 2);
                        currToken = createToken(TK_LT);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 14:
                    if(c < 'a' || c > 'z'){
                        retract(file, 2);
                        currToken = createToken(TK_RUID);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 15:
                    if(c != '@'){
                        retract(file, 1);
                        printError();
                        currLexemeSize = 0;
                    }
                    else {
                        currToken = createToken(TK_OR);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 16:
                    if(c != '&'){
                        retract(file, 1);
                        printError();
                        currLexemeSize = 0;
                    }
                    else {
                        currToken = createToken(TK_AND);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 17:
                    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) state = 17;
                    else if(c >= '0' && c <= '9') state = 22;
                    else {
                        retract(file, 1);
                        TOKENS tempToken = TK_FUNID;
                        if(findLookupTable(currLexeme)){
                            tempToken = getTokenFromString(currLexeme); 
                        }
                        if(tempToken == nf) tempToken = TK_FUNID;
                        currToken = createToken(tempToken);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 19:
                    if(c >= '0' && c <= '9') state = 23;
                    else {
                        retract(file, 2);
                        currToken = createToken(TK_NUM);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 20:
                    if(c >= 'b' && c <= 'd') state = 20;
                    else if(c >= '2' && c <= '7') state = 24;
                    else {
                        retract(file, 1);
                        currToken = createToken(TK_ID);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 21:
                    if(c == '-'){
                        currToken = createToken(TK_ASSIGNOP);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    else{
                        retract(file, 3);
                        currToken = createToken(TK_LT);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 22:
                    if(c >= '0' && c <= '9') state = 22;
                    else {
                        retract(file, 1);
                        TOKENS tempToken = TK_FUNID;
                        if(findLookupTable(currLexeme)){
                            tempToken = getTokenFromString(currLexeme); 
                        }
                        if(tempToken == nf) tempToken = TK_FUNID;
                        currToken = createToken(tempToken);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 23:
                    if(c >= '0' && c <= '9') state = 25;
                    else {
                        retract(file, 3);
                        currToken = createToken(TK_NUM);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 24:
                    if(c >= '2' && c <= '7') state = 24;
                    else {
                        retract(file, 1);
                        currToken = createToken(TK_ID);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 25:
                    if(c == 'E') state = 26;
                    else{
                        retract(file, 1);
                        currToken = createToken(TK_RNUM);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 26:
                    if(c == '+' || c == '-') state = 27;
                    else if(c >= '0' && c <= '9') state = 28;
                    else{
                        retract(file, 2);
                        currToken = createToken(TK_RNUM);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 27:
                    if(c >= '0' && c <= '9') state = 28;
                    else{
                        retract(file, 3);
                        currToken = createToken(TK_RNUM);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                case 28:
                    if(c >= '0' && c <= '9'){
                        currToken = createToken(TK_RNUM);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    else{
                        retract(file, 3);
                        currToken = createToken(TK_RNUM);
                        printToken(currToken);
                        currLexemeSize = 0;
                    }
                    break;
                default:
                    break;
        }
        // if(c == '\n') lineNumber++;
    } while(c != EOF);

    fclose(output_file);
    fclose(file);
    return 0;
}
