#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <stdio.h>

// Includes parserDef.h, which has token definitions and data structures
#include "parserDef.h"
TreeNode* parseInputSourceCode(FILE* testFile);


// Error reporting function (prototype)
void reportSyntaxError(int line, const char *message);
#endif
