#ifndef _PARSER_H_
#define _PARSER_H_

#include "includes.h"

FILE *fin, *fout;
char currentCommand[SYMBOL_MAX_LENGTH + 2];
char inputFileName[FILE_NAMES_MAX_LENGTH + 1], outputFileName[FILE_NAMES_MAX_LENGTH + 1];
int currentLine, nextAddress;

typedef struct instruction_t {
    char type;
    char* symbol;
    char* dest;
    char* comp;
    char* jump;
    int isLabel;
} instruction_t;

struct instruction_t instruction;

void initializeParser(const char* inputFileName, const char* outputFileName);
int hasMoreCommands();

void scanIgnoreSpaces(char* c, int* comment, const int stopAtNewLine);

// Should be called only if hasMoreCommands() == true
// Returns 1 if everything is correct 0 otherwise
int advance();

// Returns 'A' for an A-instruction, 'C' for a C-instruction and 'L' for labels
char getCommandType();

// Should be called only if commandType() == 'A' || commandType() == 'L'
// Returns the string corresponding to the symbol or decimal of the current command.
char* getSymbol();

// Should be called only when commandType() == 'C'
// Returns the dest mnemonic in the current command.
char* getDestMnemonic();

// Should be called only when commandType() == 'C'
// Returns the comp mnemonic in the current command.
char* getCompMnemonic();

// Should be called only when commandType() == 'C'
// Returns the jump mnemonic in the current command.
char* getJumpMnemonic();

// Returns 1 if the instruction is correct, 0 otherwise
void checkInstruction();
void checkSymbol();
void checkLabel();
void checkDest();
void checkComp();
void checkJump();

void throwError(const char* error);
int containsAnyForbiddenChar(const char* s);

void freeMemory();

// Parses the current instructions and initialises the struct instruction
void parseInstruction();

void resetInput();
void saveChanges();

#endif