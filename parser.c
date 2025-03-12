#include "parserDef.h"
#include "lexerDef.h"
#include "lexer.h" // Include the lexer header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *srcFile;
extern char *forward;
extern char *lexemebegin;
extern int activeBuffer;
extern bool exhaustedInput;
extern bool ldfirstBuff;
extern bool ldsecondBuff;
extern TwinBuffer twinBuffer; 
extern keyword *kwEntries[KC];

tokenInfo getNextToken();

// Error Reporting (Centralized)
void reportError(int lineNo, const char* message) {
    fprintf(stderr, "Error (Line %d): %s\n", lineNo, message);
}

// Error reporting function
void reportSyntaxError(int line, const char *message) {
    fprintf(stderr, "Line %d: Syntax Error: %s\n", line, message);
}

//Parsing
TreeNode* program(FILE* srcFile, Symboltable *table) {

    if(!srcFile) {
        reportError(0, "null file pointer in program");
        exit(1);
    }

    TreeNode* new_node=(TreeNode*)malloc(sizeof(TreeNode));
    if(!new_node) {
        reportError(currentToken.lineNo, "memory allocation failure");
        exit(1);
    }

    currentToken = getNextToken();

    if(currentToken.token == TK_MAIN) {

        strcpy(new_node->symbol,"Main-Function");
        // Implement the parsing logic for the 'program' non-terminal
        // and create relevant AST nodes.

        --------logic is not done yet------------
    }
    else {
        reportSyntaxError(currentToken.lineNo, "expected TK_MAIN");
        exit(1);
    }

    // initializeKeywords(table);
    // closeLexer(srcFile);
    return new_node;
}

// Main parsing function
TreeNode* parseInputSourceCode(FILE* srcFile) {
    
    // initializeLexer(srcFile); // Initialise Lexer
    srcFile = initialise("test.txt");

    if(!srcFile) {
        reportError(0, "could not initialise and will now quit");
        exit(1);
    }
   
    table = (Symboltable *)calloc(1, sizeof(Symboltable));
    initializeSymbolTable(table);

    TreeNode* rootNode = program(srcFile, table);
    return rootNode;
}

