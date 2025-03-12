#ifndef PARSERDEF_H
#define PARSERDEF_H

#include <stdbool.h>
#include <stdio.h>

// Token definition
typedef enum {
    TK_GT, TK_LT, TK_ASSIGNOP, TK_COMMENT, TK_EQ, TK_PLUS, TK_NE, TK_LE, TK_GE, 
    TK_SQR, TK_SQL, TK_OR, TK_NOT, TK_AND, TK_ID, TK_FIELDID, TK_DIV, TK_MUL, 
    TK_MINUS, TK_FUNID, TK_DOT, TK_CL, TK_OP, TK_COLON, TK_SEM, TK_RUID, 
    TK_COMMA, TK_NUM, TK_RNUM, TK_MAIN, TK_AS, TK_CALL, TK_DEFINETYPE, 
    TK_ELSE, TK_END, TK_ENDIF, TK_ENDWHILE, TK_ENDRECORD, TK_ENDUNION, 
    TK_GLOBAL, TK_IF, TK_INPUT, TK_OUTPUT, TK_INT, TK_REAL, TK_LIST, 
    TK_PARAMETERS, TK_PARAMETER, TK_READ, TK_WRITE, TK_RECORD, TK_UNION, 
    TK_RETURN, TK_THEN, TK_TYPE, TK_WHILE, TK_WITH, EPS, END_OF_INPUT
} terminals;

// Structure to hold token information 
typedef struct {
    terminals token;
    int lineNo;
    union {
        char* str;   
        int intVal;   
        float floatVal;
    } attribute;
} tokenInfo;

// Forward declaration of Symboltable 
typedef struct Symboltable Symboltable;

// Structure for SymbolTableitem
typedef struct SymbolTableitem {
    char *lexeme;
    terminals token;
    char *type;
    union {
        int intVal;
        float floatVal;
    } value;
    int lineCount;
    int scopeLevel;
    int startPos;
    int endPos;
    char **parameter;
    char *returnType;
    struct Field *fields;
    struct SymbolTableitem *next;
} SymbolTableitem;

// Forward declaration of Field (used in SymbolTableitem)
typedef struct Field Field;

// Structure for Field (needed to complete SymbolTableitem definition)
struct Field {
    char *fieldname;
    char *fieldtype;
    struct Field *next;
};

// Grammar-related structures 
typedef struct TreeNode {
    char* symbol;          
    tokenInfo token;       
    struct TreeNode* children[10];  
    int numChildren;
} TreeNode;

// Function prototypes (to be implemented in parser.c)
TreeNode* program(FILE* srcFile, Symboltable *table); 
void initializeSymbolTable(Symboltable *table);
int CalHash(char *lexeme);
bool lookup(char *lexeme, Symboltable *table);
void insert(char *lexeme, terminals token, Symboltable *table);

// Global Error
void reportError(int lineNo, const char* message);

// Error reporting function (prototype)
void reportSyntaxError(int line, const char *message);

#endif /* PARSERDEF_H */
