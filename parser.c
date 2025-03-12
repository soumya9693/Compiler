#include "parserDef.h"
#include "lexerDef.h"
#include <stdio.h>
#include <stdlib.h>

// Global variables
Token* currentToken;

// Function to initialize the parsing table
void initializeParsingTable() {
    // Populate the parsing table based on the grammar rules
    // This step is crucial for a top-down parser
    // For simplicity, assume a predefined parsing table structure
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        for (int j = 0; j < NUM_TERMINALS; j++) {
            // Assign actions based on the grammar rules
            // For example:
            if (i == PROGRAM && j == TK_MAIN) {
                parsingTable[i][j].action = SHIFT;
            }
            // Add more rules as needed
        }
    }
}

// Function to parse the program
void parseProgram() {
    // Start parsing from the program non-terminal
    parseNonTerminal(PROGRAM);
}

// Function to parse a non-terminal symbol
void parseNonTerminal(NonTerminal nonTerminal) {
    switch (nonTerminal) {
        case PROGRAM:
            // Example rule: program -> mainFunction otherFunctions
            parseMainFunction();
            parseOtherFunctions();
            break;
        case MAIN_FUNCTION:
            // Example rule: mainFunction -> _main input_par output_par stmts return_stmt
            if (currentToken->type == TK_MAIN) {
                consumeToken(TK_MAIN);
                parseInputPar();
                parseOutputPar();
                parseStmts();
                parseReturnStmt();
            } else {
                // Handle error
                printf("Error: Expected main function\n");
            }
            break;
        // Add more cases for other non-terminals
        default:
            printf("Error: Unknown non-terminal\n");
    }
}

// Function to consume a token
void consumeToken(terminals expectedType) {
    if (currentToken->type == expectedType) {
        currentToken = getNextToken();
    } else {
        // Handle error
        printf("Error: Expected token type %d, got %d\n", expectedType, currentToken->type);
    }
}

// Function to get the next token
Token* getNextToken() {
    // Call the lexer's getNextToken function
    // This function should be implemented in lexer.c
    // For simplicity, assume it's already implemented
    return lexerGetNextToken();
}

int main() {
    // Initialize the parsing table
    initializeParsingTable();

    // Start parsing
    parseProgram();

    return 0;
}
