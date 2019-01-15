#ifndef _CODE_H_
#define _CODE_H_

// Should be called only if instructionType != 'L'
char* getPrefix(const char instructionType);

// Returns a new string with the 15-bit binary-representation of the dec number
char* decToBin(int dec);

char* getDestFromMnemonic(const char* mnemonic);
char* getCompFromMnemonic(const char* mnemonic);
char* getJumpFromMnemonic(const char* mnemonic);

#endif