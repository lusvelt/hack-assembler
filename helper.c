#include "helper.h"
#include "code.h"
#include "parser.h"
#include "symbolTable.h"

struct instruction16_t instruction16;

void checkAndLoadArgs(const int argc, char** argv) {
    if (argc < 2) {
        printf("You need to specify an input assembly file\n");
        exit(EXIT_FAILURE);
    } else if (argc > 3) {
        printf("Too many arguments. Specify only input and output file names.");
        exit(EXIT_FAILURE);
    } else if (strlen(argv[1]) > FILE_NAMES_MAX_LENGTH) {
        printf("The input file name is too long");
        exit(EXIT_FAILURE);
    } else if (argc == 3) {
        if (strlen(argv[2]) > FILE_NAMES_MAX_LENGTH) {
            printf("The output file name is too long");
            exit(EXIT_FAILURE);
        }
    }

    strcpy(inputFileName, argv[1]);
    if (argc < 3) {
        int cont = 0;
        int slashes = 0;
        
        while (inputFileName[cont] != '\0') {
            if (inputFileName[cont] == '\\' || inputFileName[cont] == '/')
                slashes++;
            cont++;
        }

        cont = 0;
        while (slashes > 0 && inputFileName[cont] != '\0') {
            if (inputFileName[cont] == '\\' || inputFileName[cont] == '/')
                slashes--;
            cont++;
        }

        int start = cont;
        while (inputFileName[cont] != '.' && inputFileName[cont] != '\0' && cont < FILE_NAMES_MAX_LENGTH - 5) {
            outputFileName[cont - start] = inputFileName[cont];
            cont++;
        }
        strcpy(outputFileName + cont - start, ".hack");
    } else
        strcpy(outputFileName, argv[2]);
}

int isNaNChar(const char c) {
    return c < '0' || c > '9';
}

int strsrch(const char* s, const char c) {
    int cont = 0, found = 0;

    while (s[cont] != '\0' && !found) {
        if (s[cont] == c)
            found = 1;
        else cont++;
    }

    if (found)
        return cont;
    else return -1;
}

void clearRecords() {
    free(instruction16.comp);
    instruction16.comp = NULL;
    free(instruction16.dest);
    instruction16.dest = NULL;
    free(instruction16.jump);
    instruction16.jump = NULL;
    free(instruction16.prefix);
    instruction16.prefix = NULL;
    free(instruction16.value);
    instruction16.value = NULL;
}

void assemble() {
    strcpy(instruction16.bin, "");
    strcat(instruction16.bin, instruction16.prefix);

    if (instruction16.prefix[0] == '0')
        strcat(instruction16.bin, instruction16.value);
    else {
        strcat(instruction16.bin, instruction16.comp);
        strcat(instruction16.bin, instruction16.dest);
        strcat(instruction16.bin, instruction16.jump);
    }
}

void generateSymbolTable() {
    initializeSymbolTable();
    while (hasMoreCommands()) {
        if (advance()) {
            if (instruction.type == 'L') {
                if (contains(instruction.symbol))
                    throwError("Duplicate label");
                else addEntry(instruction.symbol, nextAddress);
            } else nextAddress++;
        }
    }

    resetInput();
}

void generateHackBinary() {
    nextAddress = 16;

    while (hasMoreCommands()) {
        if (advance())
            if (instruction.type != 'L') {
                instruction16.prefix = getPrefix(instruction.type);

                if (instruction.type == 'A')
                    if (instruction.isLabel) {
                        if (!contains(instruction.symbol)) {
                            addEntry(instruction.symbol, nextAddress);
                            nextAddress++;
                        }
                        instruction16.value = decToBin(getAddress(instruction.symbol));
                    } else instruction16.value = decToBin(atoi(instruction.symbol));
                else {
                    instruction16.dest = getDestFromMnemonic(instruction.dest);
                    instruction16.comp = getCompFromMnemonic(instruction.comp);
                    instruction16.jump = getJumpFromMnemonic(instruction.jump);
                }

                assemble();
                fprintf(fout, "%s\n", instruction16.bin);
            }
    }
}
