#ifndef PARSERDEF_H
#define PARSERDEF_H

#include "lexerDef.h"

#define LEXEME_SIZE 32

// Define TOKENS and NONTERMINALS based on lexerDef.h
typedef enum {
    // Example: if you have 214 terminals, define like this
    TOKENS_MAX = 214
} TOKENS;

typedef enum {
    // Example: if you have 70 non-terminals, define like this
    NONTERMINALS_MAX = 70
} NONTERMINALS;

/* LINKED LIST STRUCTURES*/
union NodeType {
    TOKENS terminal;
    NONTERMINALS nonterminal;
};

typedef struct ListNode* LISTNODE;
struct ListNode {
    short ruleNum;
    LISTNODE next;
    union NodeType* NODETYPE;
    short isTerminal;
};

typedef struct LinkedList* LINKEDLIST;

struct LinkedList{
    short size;
    LISTNODE head;
};

/*TREENODE STRUCTURE*/
typedef struct TreeNode* TREENODE;

union TreeNodeData{
    LEXEME* terminal;
    NONTERMINALS nonterminal;
};

struct TreeNode{
    TREENODE child;
    TREENODE next;
    TREENODE parent;
    union TreeNodeData* TREENODEDATA;
    short isTerminal;
    short ruleNum;
    TREENODE addr;
    TREENODE addr_syn;
    TREENODE addr_inh;
    TREENODE list_addr_syn;
    TREENODE left_child;
    TREENODE right_child;
    int type; // Use int for type, adjust based on actual type enum
    int isArray;
};

/*STACK STRUCTURE*/
typedef struct StackNode* STACKNODE;
struct StackNode {
    STACKNODE next;
    union NodeType* NODETYPE;
    short isTerminal;
    TREENODE treenode;
};

typedef struct Stack* STACK;

struct Stack {
    STACKNODE top;
    short size;
};

#endif
