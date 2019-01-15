#include "includes.h"
#include "helper.h"
#include "code.h"

char* getPrefix(const char instructionType) {
    char* prefix = (char*) malloc(sizeof(char[4]));
    if (instructionType == 'A') {
        prefix = (char*) malloc(sizeof(char[2]));
        strcpy(prefix, "0");
    } else {
        prefix = (char*) malloc(sizeof(char[4]));
        strcpy(prefix, "111");
    }

    return prefix;
}

char* decToBin(int dec) {
    char* bin = (char*) malloc(sizeof(char[16]));
    int i = 14, bit;

    while (dec > 0) {
        bit = dec % 2;
        dec /= 2;
        if (bit)
            bin[i] = '1';
        else
            bin[i] = '0';
        i--;
    }

    while (i >= 0) {
        bin[i] = '0';
        i--;
    }

    bin[15] = '\0';

    return bin;
}

char* getDestFromMnemonic(const char* mnemonic) {
    char* dest = (char*) malloc(sizeof(char[4]));
    strcpy(dest, "");

    if (strsrch(mnemonic, 'A') != -1)
        strcat(dest, "1");
    else strcat(dest, "0");

    if (strsrch(mnemonic, 'D') != -1)
        strcat(dest, "1");
    else strcat(dest, "0");

    if (strsrch(mnemonic, 'M') != -1)
        strcat(dest, "1");
    else strcat(dest, "0");

    return dest;
}

char* getCompFromMnemonic(const char* mnemonic) {
    char* comp = (char*) malloc(sizeof(char[8]));

    if (strcmp(mnemonic, "0") == 0) strcpy(comp, "0101010");
    else if (strcmp(mnemonic, "1") == 0) strcpy(comp, "0111111");
    else if (strcmp(mnemonic, "-1") == 0) strcpy(comp, "0111010");
    else if (strcmp(mnemonic, "D") == 0) strcpy(comp, "0001100");
    else if (strcmp(mnemonic, "A") == 0) strcpy(comp, "0110000");
    else if (strcmp(mnemonic, "!D") == 0) strcpy(comp, "0001101");
    else if (strcmp(mnemonic, "!A") == 0) strcpy(comp, "0110001");
    else if (strcmp(mnemonic, "-D") == 0) strcpy(comp, "0001111");
    else if (strcmp(mnemonic, "-A") == 0) strcpy(comp, "0110011");
    else if (strcmp(mnemonic, "D+1") == 0) strcpy(comp, "0011111");
    else if (strcmp(mnemonic, "A+1") == 0) strcpy(comp, "0110111");
    else if (strcmp(mnemonic, "D-1") == 0) strcpy(comp, "0001110");
    else if (strcmp(mnemonic, "A-1") == 0) strcpy(comp, "0110010");
    else if (strcmp(mnemonic, "D+A") == 0) strcpy(comp, "0000010");
    else if (strcmp(mnemonic, "D-A") == 0) strcpy(comp, "0010011");
    else if (strcmp(mnemonic, "A-D") == 0) strcpy(comp, "0000111");
    else if (strcmp(mnemonic, "D&A") == 0) strcpy(comp, "0000000");
    else if (strcmp(mnemonic, "D|A") == 0) strcpy(comp, "0010101");

    else if (strcmp(mnemonic, "M") == 0) strcpy(comp, "1110000");
    else if (strcmp(mnemonic, "!M") == 0) strcpy(comp, "1110001");
    else if (strcmp(mnemonic, "-M") == 0) strcpy(comp, "1110011");
    else if (strcmp(mnemonic, "M+1") == 0) strcpy(comp, "1110111");
    else if (strcmp(mnemonic, "M-1") == 0) strcpy(comp, "1110010");
    else if (strcmp(mnemonic, "D+M") == 0) strcpy(comp, "1000010");
    else if (strcmp(mnemonic, "D-M") == 0) strcpy(comp, "1010011");
    else if (strcmp(mnemonic, "M-D") == 0) strcpy(comp, "1000111");
    else if (strcmp(mnemonic, "D&M") == 0) strcpy(comp, "1000000");
    else if (strcmp(mnemonic, "D|M") == 0) strcpy(comp, "1010101");

    return comp;
}

char* getJumpFromMnemonic(const char* mnemonic) {
    char* jump = (char*) malloc(sizeof(char[4]));

    if (strcmp(mnemonic, "JGT") == 0) strcpy(jump, "001");
    else if (strcmp(mnemonic, "JEQ") == 0) strcpy(jump, "010");
    else if (strcmp(mnemonic, "JGE") == 0) strcpy(jump, "011");
    else if (strcmp(mnemonic, "JLT") == 0) strcpy(jump, "100");
    else if (strcmp(mnemonic, "JNE") == 0) strcpy(jump, "101");
    else if (strcmp(mnemonic, "JLE") == 0) strcpy(jump, "110");
    else if (strcmp(mnemonic, "JMP") == 0) strcpy(jump, "111");
    else strcpy(jump, "000");

    return jump;
}
