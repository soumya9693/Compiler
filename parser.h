#ifndef PARSER_H
#define PARSER_h
#include "parserDef.h"

// Function to compute FIRST and FOLLOW sets
void ComputeFirstAndFollowSets(Grammar G, FirstAndFollow *F);

// Function to create a parse table
void createParseTable(FirstAndFollow F, ParseTable *T);

// Function to parse input source code
ParseTree *parseInputSourceCode(char *testcaseFile, ParseTable T);

// Function to print the parse tree
void printParseTree(ParseTree *PT, char *outfile);

#endif // PARSE_H
