#include "includes.h"
#include "helper.h"
#include "parser.h"

FILE *fin, *fout;
char currentCommand[SYMBOL_MAX_LENGTH + 2];
int currentLine, nextAddress;

struct instruction_t instruction;

void initializeParser(const char* inputFileName, const char* outputFileName) {
    fin = fopen(inputFileName, "r");
    fout = fopen(outputFileName, "w");

    if (fin == NULL) {
        printf("The specified input file was not found\n");
        exit(EXIT_FAILURE);
    }

    currentCommand[0] = '\0';
    currentLine = 0;
    nextAddress = 0;
}

int hasMoreCommands() {
    clearRecords();
    return !feof(fin);
}

void scanIgnoreSpaces(char* c, int* comment, const int stopAtNewLine) {
    do {
        do fscanf(fin, "%c", c);
        while (*c == ' ' && !feof(fin));

        if (*c == '\n') {
            currentLine++;
            *comment = 0;
        }
    } while (!stopAtNewLine && *c == '\n' && !feof(fin));
}

int advance() {
    char c;
    int comment, cont, wrong;
    comment = 0;
    cont = 0;

    do {
        scanIgnoreSpaces(&c, &comment, cont > 0);

        if (c == '/') comment++;
        else if (!feof(fin)) {
            if (comment == 1) comment = 0;
            if (c != ' ' && c != '\n' && !comment) {
                currentCommand[cont] = c;
                cont++;
            }
        }
    } while (c != '\n' && cont < SYMBOL_MAX_LENGTH + 2 && !feof(fin));

    if (feof(fin) && cont == 0)
        return 0;
    
    currentCommand[cont] = '\0';

    if (cont == SYMBOL_MAX_LENGTH + 2)
        throwError("The instruction contains too many characters");

    currentCommand[cont] = '\0';

    parseInstruction();
    checkInstruction();
    return 1;
}

char getCommandType() {
    if (currentCommand[0] == '@')
        return 'A';
    else if (currentCommand[0] == '(')
        return 'L';
    else
        return 'C';
}

char* getSymbol() {
    char *symbol = (char*) malloc(sizeof(char[SYMBOL_MAX_LENGTH + 2]));
    int cont = 0;

    while (currentCommand[cont + 1] != '\0' && currentCommand[cont + 1] != ')') {
        symbol[cont] = currentCommand[cont + 1];
        cont++;
    }

    symbol[cont] = '\0';

    return symbol;
}

char* getDestMnemonic() {
    char *dest = (char*) malloc(sizeof(char[5]));
    int cont = 0;

    while (currentCommand[cont] != '=' && currentCommand[cont] != ';' && cont < 5 && currentCommand[cont] != '\0') {
        dest[cont] = currentCommand[cont];
        cont++;
    }

    if (currentCommand[cont] != '=')
        cont = 0;
    
    dest[cont] = '\0';

    return dest;
}

char* getCompMnemonic() {
    char *comp = (char*) malloc(sizeof(char[5]));
    int cont = 0, i = 0;

    while (currentCommand[cont] != ';' && i < 5 && currentCommand[cont] != '\0') {
        if (currentCommand[cont] == '=')
            i = 0;
        else {
            comp[i] = currentCommand[cont];
            i++;
        }
        cont++;
    }

    comp[i] = '\0';

    return comp;
}

char* getJumpMnemonic() {
    char *jump = (char*) malloc(sizeof(char[5]));
    int cont = 0;

    while (currentCommand[cont] != ';' && currentCommand[cont] != '\0')
        cont++;
    
    if (currentCommand[cont] == ';') {
        int start = cont + 1;
        cont = 0;
        while (currentCommand[start + cont] != '\0' && cont < 4) {
            jump[cont] = currentCommand[start + cont];
            cont++;
        }
    } else
        cont = 0;

    jump[cont] = '\0';

    return jump;
}

void checkSymbol() {
    char* symbol = instruction.symbol;

    if (strlen(symbol) > SYMBOL_MAX_LENGTH)
        throwError("Symbol is too long");

    if (!isNaNChar(symbol[0])) {
        int cont = 1;

        while (cont < strlen(symbol)) {
            if (isNaNChar(symbol[cont]))
                throwError("Symbol starts with a number but contains non-numeric characters");
            cont++;
        }

        if (atoi(symbol) >= 32768) throwError("A-value is too high");
    } else if (containsAnyForbiddenChar(symbol))
        throwError("Symbol not allowed");
}

void checkLabel() {
    char* symbol = instruction.symbol;

    if (strlen(symbol) > SYMBOL_MAX_LENGTH)
        throwError("Symbol is too long");

    if (!isNaNChar(symbol[0]))
        throwError("Label must not start with a number");

    if (containsAnyForbiddenChar(symbol))
        throwError("Symbol not allowed");

    if (strsrch(currentCommand, ')') == -1)
        throwError("Missing closing parenthesis");
}

void checkDest() {
    char* dest = instruction.dest;

    int cont = 0;
    char dests[4];
    strcpy(dests, "");

    while (dest[cont] != '\0') {
        if (strsrch("AMD", dest[cont]) == -1)
            throwError("Bad destination in C-instruction");

        if (strsrch(dests, dest[cont]) != -1)
            throwError("Duplicated destination in C-instruction");

        strncat(dests, dest + cont, 1);
        cont++;
    }
}

void checkComp() {
    char* comp = instruction.comp;

    if (strlen(comp) == 0)
        throwError("Comp part not found in C-instruction");
    
    if (strlen(comp) == 1) {
        if (strsrch("01AMD", comp[0]) == -1)
            throwError("Syntax error in comp part of C-instruction");
    }

    if (strlen(comp) == 2) {
        if (strsrch("-!", comp[0]) == -1 || strsrch("1AMD", comp[1]) == -1 || (comp[0] == '-' && comp[1] == '0') || (comp[0] == '!' && strsrch("AMD", comp[1]) == -1))
            throwError("Syntax error in comp part of C-instruction");
    }

    if (strlen(comp) == 3) {
        if (strsrch("AMD", comp[0]) == -1 ||
            strsrch("+-&|", comp[1]) == -1 ||
            strsrch("AMD1", comp[2]) == -1 ||
            ((comp[1] == '&' || comp[1] == '|') && comp[2] == '1') ||
            ((comp[0] == 'A' || comp[0] == 'M') && strsrch("D1", comp[2]) == -1) ||
            ((comp[1] == 'A' || comp[1] == 'M') && comp[0] != 'D'))
            throwError("Syntax error in comp part of C-instruction");
    }

    if (strlen(comp) > 3)
        throwError("Syntax error in comp part of C-instruction");
}

void checkJump() {
    char* jump = instruction.jump;

    if (strlen(jump) > 0) {
        if (strcmp(jump, "JMP") != 0 &&
            strcmp(jump, "JGT") != 0 &&
            strcmp(jump, "JGE") != 0 &&
            strcmp(jump, "JEQ") != 0 &&
            strcmp(jump, "JNE") != 0 &&
            strcmp(jump, "JLE") != 0 &&
            strcmp(jump, "JLT") != 0)
                throwError("Bad jump directive in C-instruction");
    }
}

void checkInstruction() {
    if (instruction.type == 'A')
        checkSymbol();
    else if (instruction.type == 'L')
        checkLabel();
    else {
        checkDest();
        checkComp();
        checkJump();
    }
}

void parseInstruction() {
    freeMemory();

    instruction.type = getCommandType();
    if (instruction.type == 'C') {
        instruction.dest = getDestMnemonic();
        instruction.comp = getCompMnemonic();
        instruction.jump = getJumpMnemonic();
    } else {
        instruction.symbol = getSymbol();
        instruction.isLabel = isNaNChar(instruction.symbol[0]);
    }
}

int containsAnyForbiddenChar(const char* s) {
    int found = 0;
    while (*s != '\0' && !found) {
        if (strsrch("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz_.$:", *s) == -1)
            found = 1;
        else
            s++;
    }

    return found;
}

void freeMemory() {
    if (instruction.type == 'A' || instruction.type == 'L') {
        free(instruction.symbol);
        instruction.symbol = NULL;
    } else {
        free(instruction.dest);
        instruction.dest = NULL;
        free(instruction.comp);
        instruction.comp = NULL;
        free(instruction.jump);
        instruction.jump = NULL;
    }
}

void resetInput() {
    fclose(fin);
    fin = fopen(inputFileName, "r");
    currentLine = 0;
}

void throwError(const char* error) {
    printf("Fatal error at line %d: %s\n", currentLine, error);
    exit(EXIT_FAILURE);
}

void saveChanges() {
    fclose(fin);
    fclose(fout);
}
