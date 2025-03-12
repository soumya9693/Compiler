#ifndef PARSER_H
#define PARSER_H

#include "lexerDef.h"
#include <stdbool.h>
#include <stdio.h>
extern FILE *srcFile;
extern char *forward;
extern char *lexemebegin;
extern int activeBuffer;
extern bool exhaustedInput;
extern bool ldfirstBuff;
extern bool ldsecondBuff;
extern TwinBuffer twinBuffer; // Global TwinBuffer instance
extern keyword *kwEntries[KC];
extern Symboltable *table;
/*-----symboltable------*/
void reportSyntaxError(int line, const char *message);
#endif
