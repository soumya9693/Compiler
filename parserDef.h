#ifndef PARSERDEF_H
#define PARSERDEF_H

#include <stdio.h>
#include <stdlib.h>

#define EPSILON_ID 0
#define END_MARKER_ID -1
#define INITIAL_SET_SIZE 10

// Symbol structure definition
typedef struct {
    int id;
    int isTerminal;
    int isEpsilon;
} Symbol;

// Production Rule structure
typedef struct {
    Symbol lhs;
    Symbol* rhs;
    int numRhs;
} Rule;

// Grammar structure
typedef struct {
    Symbol startSymbol;
    Symbol* nonTerminals;
    int numNonTerminals;
    Rule* rules;
    int numRules;
} Grammar;

// Set implementation for FIRST/FOLLOW
typedef struct {
    Symbol* symbols;
    int count;
    int capacity;
} Set;

// Combined FIRST and FOLLOW sets
typedef struct {
    Set* first;
    Set* follow;
} FirstAndFollow;

// Public interface
FirstAndFollow ComputeFirstAndFollowSets(Grammar G);

// Helper functions
void computeFirstSets(Grammar G, FirstAndFollow* FF);
void computeFollowSets(Grammar G, FirstAndFollow* FF);
Set createEmptySet();
int addToSet(Set* set, Symbol symbol);
int unionSets(Set* dest, Set src);
int containsEpsilon(Set set);
int getNonTerminalIndex(Grammar G, Symbol nonTerminal);
int symbolsEqual(Symbol s1, Symbol s2);
Symbol createEpsilonSymbol();
Symbol createEndMarkerSymbol();

#endif
