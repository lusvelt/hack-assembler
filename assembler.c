#include "includes.h"
#include "helper.h"
#include "parser.h"
#include "code.h"
#include "symbolTable.h"

int main(int argc, char** argv) {
    checkAndLoadArgs(argc, argv);    
    initializeParser(inputFileName, outputFileName);

    generateSymbolTable();
    generateHackBinary();

    saveChanges();

    exit(EXIT_SUCCESS);
}