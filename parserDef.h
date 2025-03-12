//                         GROUP - 30
// ID:  2020A7PS0094P                     Name:  Arya Veer Singh Chauhan
// ID:  2020A7PS0049P                     Name:  Madhav Madhusoodanan
// ID:  2020A7PS0016P                     Name:  Ruchika Sarkar
// ID:  2020A7PS0984P                     Name:  Utsav Goel
// ID:  2020A7PS0102P                     Name:  Hardik Jain

#include<stdio.h>
#ifndef PARSERDEF_H
#define PARSERDEF_H
#define LEXEME_SIZE 32

#include "lexerDef.h"

typedef enum{
    TYPE_ERROR = -2,
    TYPE_UNDEFINED = -1,
    TYPE_INTEGER = 0,
    TYPE_REAL = 1,
    TYPE_BOOLEAN = 2,
}TYPE;

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
    TYPE type;
    int isArray;
    // AST 
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

// typedef struct ASTNode* ASTNODE;
// struct ASTNode {
//     AST left;
//     AST right;
//     AST addr_inh;
//     AST addr_syn;
// };

// typedef union Ast* AST;

// union Ast {
//     ASTNODE astNode;
//     TREENODE treeNode;
// };

#endif
