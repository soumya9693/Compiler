#include<stdio.h>
#ifndef PARSERDEF_H
#define PARSERDEF_H

#include "lexerDef.h"

#define LEXEME_SIZE = 256

// Define structures for FIRST and FOLLOW sets
typedef struct {
    char **first;
    char **follow;
} FirstAndFollow;

// Define structure for a grammar rule
typedef struct GrammarRule {
    char *lhs; // Left-hand side non-terminal
    char **rhs; // Right-hand side terminals/non-terminals
    int rhsSize; // Size of the RHS array
} GrammarRule;


// Define structure for the grammar
typedef struct Grammar {
    GrammarRule *rules;
    int numRules; // Number of rules in the grammar
} Grammar;


// Define structure for a parse table entry
typedef struct ParseTableEntry {
    char *lhs; // Left-hand side non-terminal
    char **rhs; // Right-hand side terminals/non-terminals
} ParseTableEntry;

// Define structure for the parse table
typedef struct ParseTable {
    ParseTableEntry **entries; // Array of parse table entries
    int rows; // Number of rows in the table
    int cols; // Number of columns in the table
} ParseTable;


// Define structure for a node in the parse tree
typedef struct ParseTreeNode {
    char *lexeme; // Lexeme of the node (if leaf)
    int lineno; // Line number of the node
    char *tokenName; // Token name of the node
    double value; // Value if the node is a number
    char *parentNodeSymbol; // Parent node symbol
    int isLeafNode; // Whether the node is a leaf node
    char *nodeSymbol; // Symbol of the current node
    struct ParseTreeNode *children; // Array of child nodes
    int numChildren; // Number of child nodes
} ParseTreeNode;




// Define structure for the parse tree
typedef struct ParseTree {
    ParseTreeNode *root; // Root node of the parse tree
} ParseTree;

#endif // PARSERDEF_H
