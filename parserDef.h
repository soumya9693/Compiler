#include<stdio.h>
#ifndef PARSERDEF_H
#define PARSERDEF_H

#include "lexerDef.h"

#define LEXEME_SIZE =

typedef enum{
    TYPE_ERROR = ,
    TYPE_UNDEFINED = ,
    TYPE_INTEGER = ,
    TYPE_REAL = ,
    TYPE_BOOLEAN = ,
}TYPE;

// Define TOKENS and NONTERMINALS based on lexerDef.h
typedef enum {
 
} TOKENS;

typedef enum {
    
    NONTERMINALS_MAX = 
} NONTERMINALS;

/* LINKED LIST STRUCTURES*/
union NodeType {
    TOKENS terminal;
    NONTERMINALS nonterminal;
};

typedef struct ListNode* NodePointer;
struct ListNode {
    short ruleNumber;
    NodePointer nextNode;
    union NodeType* nodeType;
    short isTerminalNode;
};

typedef struct LinkedList* NodeList;

struct LinkedList{
    short listSize;
    LISTNODE listHead;
};

/*TREENODE STRUCTURE*/
typedef struct TreeNode* Node;

union TreeNodeData{
    LEXEME* terminal;
    NONTERMINALS nonterminal;
};

struct TreeNode{
    Node childNode;
    Node nextNode;
    Node parentNode;
    union TreeNodeData* nodeData;
    short isTerminalNode;
    short ruleNumber;
    Node address;
    Node synthesizedAddress;
    Node inheritedAddress;
    Node synthesizedListAddress;
    Node leftChild;
    Node rightChild;
    int nodeType; 
    int isNodeArray;
};

/*STACK STRUCTURE*/
typedef struct StackNode* StackNodePointer;
struct StackNode {
    StackNodePointer nextNode;
    union NodeType* nodeType;
    short isTerminalNode;
    Node treeNode;
};

typedef struct Stack* StackPointer;

struct Stack {
    STACKNODE StackTop;
    short stackSize;
};

#endif
