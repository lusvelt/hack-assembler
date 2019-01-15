#ifndef _HELPER_H_
#define _HELPER_H_
#include "includes.h"

typedef struct instruction16_t {
    char* prefix;
    char* value;
    char* memory;
    char* comp;
    char* dest;
    char* jump;
    char bin[17];
} instruction16_t;

struct instruction16_t instruction16;

void checkAndLoadArgs(const int argc, char** argv);
int isNaNChar(const char c);

// Returns the index of a char in a string if it occurs, -1 otherwise
int strsrch(const char* s, const char c);

void clearRecords();
void assemble();

void generateSymbolTable();
void generateHackBinary();

#endif