#include "includes.h"
#include "helper.h"
#include "symbolTable.h"

symbol_t* symbolTable = NULL;

symbol_t defaultSymbols[DEFAULT_SYMBOLS_COUNT] = {
    {"SP", 0},         {"LCL", 1},     {"ARG", 2},  {"THIS", 3}, {"THAT", 4},
    {"SCREEN", 16384}, {"KBD", 24576}, {"R0", 0},   {"R1", 1},   {"R2", 2},
    {"R3", 3},         {"R4", 4},      {"R5", 5},   {"R6", 6},   {"R7", 7},
    {"R8", 8},         {"R9", 9},      {"R10", 10}, {"R11", 11}, {"R12", 12},
    {"R13", 13},       {"R14", 14},    {"R15", 15}
};

void initializeSymbolTable() {
    for (int i = 0; i < DEFAULT_SYMBOLS_COUNT; i++)
        addEntry(defaultSymbols[i].symbol, defaultSymbols[i].address);
}

void addEntry(const char* symbol, const int address) {
    symbol_t* tmp = (symbol_t*) malloc(sizeof(symbol_t));
    tmp->symbol = (char*) malloc(sizeof(char[SYMBOL_MAX_LENGTH + 1]));
    strcpy(tmp->symbol, symbol);
    tmp->address = address;
    tmp->next = symbolTable;
    symbolTable = tmp;
}

int contains(const char* symbol) {
    symbol_t* tmp = symbolTable;
    int found = 0;

    while (tmp != NULL && !found) {
        if (strcmp(tmp->symbol, symbol) == 0)
            found = 1;
        tmp = tmp->next;
    }

    return found;
}

int getAddress(const char* symbol) {
    symbol_t* tmp = symbolTable;
    int found = 0, foundAddress;

    while (tmp != NULL && !found) {
        if (strcmp(tmp->symbol, symbol) == 0) {
            found = 1;
            foundAddress = tmp->address;
        }
        tmp = tmp->next;
    }

    if (found)
        return foundAddress;
    else return -1;
}
