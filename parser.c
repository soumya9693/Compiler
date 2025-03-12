#include "parserDef.h"

#include <stdio.h>
#include <stdlib.h>

// Helper function prototypes
int addToSet(Set* set, Symbol symbol);
int unionSets(Set* dest, Set src);
int containsEpsilon(Set set);
int getNonTerminalIndex(Grammar G, Symbol nonTerminal);
int symbolsEqual(Symbol s1, Symbol s2);
Symbol createEpsilonSymbol();
Symbol createEndMarkerSymbol();
Set createEmptySet();

// Constants (add these at top)
#define EPSILON_ID 0
#define END_MARKER_ID -1
#define INITIAL_SET_SIZE 10

// Create empty set with initial capacity
Set createEmptySet() {
    Set set;
    set.count = 0;
    set.capacity = INITIAL_SET_SIZE;
    set.symbols = (Symbol*)malloc(set.capacity * sizeof(Symbol));
    if(!set.symbols) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return set;
}

// Add symbol to set (returns 1 if added, 0 if already exists)
int addToSet(Set* set, Symbol symbol) {
    // Check for existing element
    for(int i = 0; i < set->count; i++) {
        if(symbolsEqual(set->symbols[i], symbol)) return 0;
    }
    
    // Resize if needed
    if(set->count >= set->capacity) {
        set->capacity *= 2;
        Symbol* new_mem = realloc(set->symbols, set->capacity * sizeof(Symbol));
        if(!new_mem) {
            fprintf(stderr, "Set expansion failed\n");
            exit(EXIT_FAILURE);
        }
        set->symbols = new_mem;
    }
    
    set->symbols[set->count++] = symbol;
    return 1;
}

// Union of two sets
int unionSets(Set* dest, Set src) {
    int changed = 0;
    for(int i = 0; i < src.count; i++) {
        changed |= addToSet(dest, src.symbols[i]);
    }
    return changed;
}

// Check epsilon presence
int containsEpsilon(Set set) {
    for(int i = 0; i < set.count; i++) {
        if(set->symbols[i].isEpsilon) return 1;
    }
    return 0;
}

// Get index of non-terminal
int getNonTerminalIndex(Grammar G, Symbol nonTerminal) {
    for(int i = 0; i < G.numNonTerminals; i++) {
        if(symbolsEqual(G.nonTerminals[i], nonTerminal)) {
            return i;
        }
    }
    fprintf(stderr, "Non-terminal not found in grammar\n");
    exit(EXIT_FAILURE);
}

// Equality check for symbols
int symbolsEqual(Symbol s1, Symbol s2) {
    return s1.id == s2.id && s1.isTerminal == s2.isTerminal;
}

// Create epsilon symbol
Symbol createEpsilonSymbol() {
    Symbol epsilon;
    epsilon.id = EPSILON_ID;
    epsilon.isTerminal = 1;
    epsilon.isEpsilon = 1;
    return epsilon;
}

// Create end marker symbol
Symbol createEndMarkerSymbol() {
    Symbol endMarker;
    endMarker.id = END_MARKER_ID;
    endMarker.isTerminal = 1;
    endMarker.isEpsilon = 0;
    return endMarker;
}
// Compute FIRST sets for all non-terminals
void computeFirstSets(Grammar G, FirstAndFollow* FF) {
    int changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < G.numRules; i++) {
            Rule rule = G.rules[i];
            int lhsIndex = getNonTerminalIndex(G, rule.lhs);
            Set firstSet = FF->first[lhsIndex];
            Set tempSet = createEmptySet();
            int allEpsilon = 1;

            for (int j = 0; j < rule.numRhs; j++) {
                Symbol rhsSymbol = rule.rhs[j];

                if (rhsSymbol.isTerminal) {
                    changed |= addToSet(&tempSet, rhsSymbol);
                    allEpsilon = 0;
                    break;
                } else {
                    int rhsIndex = getNonTerminalIndex(G, rhsSymbol);
                    changed |= unionSets(&tempSet, FF->first[rhsIndex]);
                    if (!containsEpsilon(FF->first[rhsIndex])) {
                        allEpsilon = 0;
                        break;
                    }
                }
            }

            if (allEpsilon) {
                Symbol epsilon = createEpsilonSymbol();
                changed |= addToSet(&tempSet, epsilon);
            }

            changed |= unionSets(&firstSet, tempSet);
            FF->first[lhsIndex] = firstSet;

            // Free tempSet contents and then the Set itself.
            free(tempSet.symbols);
        }
    }
}

// Compute FOLLOW sets for all non-terminals
void computeFollowSets(Grammar G, FirstAndFollow* FF) {
// Add start symbol to follow set of start symbol
    int startSymbolIndex = getNonTerminalIndex(G, G.startSymbol);
    Symbol endMarker = createEndMarkerSymbol();
    addToSet(&FF->follow[startSymbolIndex], endMarker);

    int changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < G.numRules; i++) {
            Rule rule = G.rules[i];
            int lhsIndex = getNonTerminalIndex(G, rule.lhs);

            for (int j = 0; j < rule.numRhs; j++) {
                Symbol rhsSymbol = rule.rhs[j];
                if (!rhsSymbol.isTerminal) {
                    int rhsIndex = getNonTerminalIndex(G, rhsSymbol);
                    Set tempSet = createEmptySet();
                    int allEpsilon = 1;

                    for (int k = j + 1; k < rule.numRhs; k++) {
                        Symbol nextSymbol = rule.rhs[k];

                        if (nextSymbol.isTerminal) {
                            changed |= addToSet(&tempSet, nextSymbol);
                            allEpsilon = 0;
                            break;
                        } else {
                            int nextIndex = getNonTerminalIndex(G, nextSymbol);
                            changed |= unionSets(&tempSet, FF->first[nextIndex]);
                            if (!containsEpsilon(FF->first[nextIndex])) {
                                allEpsilon = 0;
                                break;
                            }
                        }
                    }

                    if (allEpsilon) {
                        changed |= unionSets(&tempSet, FF->follow[lhsIndex]);
                    }

                    changed |= unionSets(&FF->follow[rhsIndex], tempSet);
                    free(tempSet.symbols);
                }
            }
        }
    }
}

// Main FIRST/FOLLOW computation
FirstAndFollow ComputeFirstAndFollowSets(Grammar G) {
    FirstAndFollow FF;
    FF.first = (Set*)malloc(G.numNonTerminals * sizeof(Set));
    FF.follow = (Set*)malloc(G.numNonTerminals * sizeof(Set));
    
    if(!FF.first || !FF.follow) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    // Initialize sets
    for(int i = 0; i < G.numNonTerminals; i++) {
        FF.first[i] = createEmptySet();
        FF.follow[i] = createEmptySet();
    }
    
    computeFirstSets(G, &FF);
    computeFollowSets(G, &FF);
    
    return FF;
}
