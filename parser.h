#ifndef PARSER_H
#define PARSER_H

#include "grammar.h"  // Contains Grammar/Symbol definitions

typedef struct {
    Set* first;
    Set* follow;
} FirstAndFollow;

// Public interface
FirstAndFollow computeFirstAndFollow(Grammar G);
ParseTree parseInputSource(FILE* source);

#endif
