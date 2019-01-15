#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

typedef struct symbol_t {
    char* symbol;
    int address;
    struct symbol_t* next;
} symbol_t;

#define DEFAULT_SYMBOLS_COUNT 23
struct symbol_t defaultSymbols[DEFAULT_SYMBOLS_COUNT];

void initializeSymbolTable();
void addEntry(const char* symbol, const int address);
int contains(const char* symbol);

// Returns -1 if no corresponding symbol is found
int getAddress(const char* symbol);

#endif