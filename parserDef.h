#ifndef PARSERDEF_H
#define PARSERDEF_H

#include "lexerDef.h"
#include <stdio.h>
#include <stdbool.h>

// Non-terminal symbols
typedef enum {
    PROGRAM, MAIN_FUNCTION, OTHER_FUNCTIONS, FUNCTION, INPUT_PAR, OUTPUT_PAR, PARAMETER_LIST, DATA_TYPE,
    PRIMITIVE_DATATYPE, CONSTRUCTED_DATATYPE, A, STMTS, REMAINING_LIST, TYPE_DEFINITIONS, TYPE_DEFINITION,
    FIELD_DEFINITIONS, FIELD_DEFINITION, FIELDTYPE, MORE_FIELDS, DECLARATIONS, DECLARATION,
    GLOBAL_OR_NOT, OTHER_STMTS, STMT, ASSIGNMENT_STMT, SINGLE_OR_REC_ID, OPTION_SINGLE_CONSTRUCTED,
    ONE_EXPANSION, MORE_EXPANSIONS, FUN_CALL_STMT, OUTPUT_PARAMETERS, INPUT_PARAMETERS, ITERATIVE_STMT,
    CONDITIONAL_STMT, ELSE_PART, IO_STMT, ARITHMETIC_EXPRESSION, EXP_PRIME, TERM, TERM_PRIME, FACTOR,
    HIGH_PRECEDENCE_OPERATORS, LOW_PRECEDENCE_OPERATORS, BOOLEAN_EXPRESSION, VAR, LOGICAL_OP, RELATIONAL_OP,
    RETURN_STMT, OPTIONAL_RETURN, ID_LIST, MORE_IDS, DEFINETYPE_STMT
} NonTerminal;

// Production rules
// Define the production rules as a set of functions or structures
// For example, you can use a struct to represent a production rule
typedef struct {
    NonTerminal lhs; // Left-hand side non-terminal
    int rhsSize;     // Size of the right-hand side
    int* rhs;        // Array of right-hand side symbols (terminals and non-terminals)
} ProductionRule;

// Define the number of production rules
#define NUM_PRODUCTION_RULES 50

// Define the production rules array
extern ProductionRule productionRules[NUM_PRODUCTION_RULES];

// Parsing table
// Define a parsing table structure
typedef struct {
    NonTerminal nonTerminal;
    terminals terminal;
    int action; // Action to take (e.g., shift, reduce, accept)
} ParsingTableEntry;

// Define the parsing table
extern ParsingTableEntry parsingTable[NUM_NON_TERMINALS][NUM_TERMINALS];

// Define the number of non-terminals and terminals
#define NUM_NON_TERMINALS 50
#define NUM_TERMINALS N_TC

// Function prototypes
void initializeParsingTable();
void parseProgram();

#endif // PARSERDEF_H
