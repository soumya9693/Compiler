#ifndef PARSER_H
#define PARSER_h
#include "parserDef.h"

TREENODE parser(char* grammarFile,char* inputFile,int size);
void testAutomation(char* grammarFile);
TREENODE getChildNonTerminal(int nt,TREENODE tn);
TREENODE getChildTerminal(int nt,TREENODE tn);
TREENODE* getDualNonTerminal(int nt,TREENODE tn);
TREENODE* getDualTerminal(int nt,TREENODE tn);
void inorderTraversal(TREENODE root,short goNext);
int countParseTreeNodes(TREENODE tn, short goNext);
int getSizePT();
void setSizePT();
#endif
