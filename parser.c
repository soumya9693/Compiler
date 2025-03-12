#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parserDef.h"
#include "constants.h"

#define MAX 100

int error = 0;
int PT_SIZE = 0;

// Linked List Functions
LINKEDLIST createLinkedList() {
    LINKEDLIST ll = (LINKEDLIST)malloc(sizeof(struct LinkedList));
    ll->head = NULL;
    ll->size = 0;
    return ll;
}

void insertionInLinkedList(LINKEDLIST ll, short isTerminal, short ndtype, short ruleNum) {
    if (ndtype == -1) isTerminal = -1;
    LISTNODE lstNode = (LISTNODE)malloc(sizeof(struct ListNode));
    lstNode->NODETYPE = (union NodeType*)malloc(sizeof(union NodeType));
    ll->size++;
    lstNode->next = NULL;
    LISTNODE curr = ll->head;
    if (curr == NULL) {
        ll->head = lstNode;
    } else {
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = lstNode;
    }
    lstNode->ruleNum = ruleNum;
    lstNode->isTerminal = isTerminal;
    if (isTerminal == 1) {
        lstNode->NODETYPE->terminal = (TOKENS)ndtype;
    } else {
        lstNode->NODETYPE->nonterminal = (NONTERMINALS)ndtype;
    }
}


/*READING RULES FROM CSV*/

short countCols(FILE *fp){
    char character = ' ';
    short count = 0;
    while(character != '\n' && character != EOF && character != '\0'){
        character = (char)fgetc(fp);
        if(character == ',') ++count;
    }
    fclose(fp);
    return count;
}

short countRows(FILE *fp){
    char character = ' ';
    short count = 1;
    while(character != EOF && character != '\0'){
        character = (char)fgetc(fp);
        if(character == '\n') ++count;
    }
    fclose(fp);
    return count;
}

/* Reads a line of csv file that is passed as fp */
char **csvReader(FILE *fp) {
    char line[MAX];
    char *token;
    char **csv_data;
    short col = 0;

    csv_data = (char**)malloc(MAX * sizeof(char*));

    if (fp == NULL) {
        printf("The file cannot be opened!.\n");
        exit(1);
    }

    for(short i = 0; i < MAX; i++){
        csv_data[i] = NULL;
    }

    if (fgets(line, MAX, fp) != NULL) {
        col = 0;
        token = strtok(line, ",");
        while (token != NULL) {
            csv_data[col] = malloc(strlen(token) + 1);
            strcpy(csv_data[col], token);
            csv_data[col][strcspn(csv_data[col], "\n")] = '\0';
            col++;
            token = strtok(NULL, ",");
        }
    }

    return csv_data;
}

/*GRAMMAR RULES LOGIC*/


short int checkTerminal(char* string){
    for(short int i = 0;i<214;i++){
        if(strcmp(string,TERMINALS_STRINGS[i]) == 0){
            return 1;
        }
    }
    return 0;
}


short int getType(char* string,short int type){
    if(type == 1){
        for(short int i = 0;i<214;i++){
            if(strcmp(string,TERMINALS_STRINGS[i]) == 0){
                return i;
            }
        }
    }
    for(short int i = 0;i<70;i++){
        if(strcmp(string,NONTERMINALS_STRINGS[i]) == 0){
            return i;
        }
    }
    return -1;
}

/*
@params
ruleNum:int => Number of rule as per the grammar
strings[]:char* => The rule is form of array
num:int => Number of strings in the rule
*/
void addRule(short int ruleNum,char** strings,short int num,LISTNODE* RULES){
    LINKEDLIST ll = createLinkedList();
    for(short int i = 0;i < num;i++){
        if(strings[i] == (char*)NULL) break;
        short int isTerminal = checkTerminal(strings[i]);
        short int ndtype = getType(strings[i],isTerminal);
        insertionInLinkedList(ll,isTerminal,ndtype,ruleNum);
    }
    RULES[ruleNum] = ll->head;
    return;
}


void printRules(short int numRules, LISTNODE* RULES){
    for(short int i = 0;i < numRules;i++){
        LISTNODE curr = RULES[i];
        while(curr != NULL){
            short int rule = curr->ruleNum;
            short int isTerminal = curr->isTerminal;
            short int nodeType = isTerminal ? curr->NODETYPE->terminal : curr->NODETYPE->nonterminal;
            if(isTerminal == 1){
                printf("Rule no. %d , Terminal %s\n",rule,TERMINALS_STRINGS[nodeType]);
            }
            else{
                printf("Rule no. %d , NonTerminal %s\n",rule,NONTERMINALS_STRINGS[nodeType]);
            }
            curr=curr->next;
        }
    }
}


LISTNODE* addRules(char* fname){
    short int ruleNum = 0;
    FILE* fp = fopen(fname,"r");
    LISTNODE* RULES = (LISTNODE*) malloc(129*sizeof(LISTNODE));
    while(1){
        char** row = csvReader(fp);
        if(row[0] == NULL) break;
        addRule(ruleNum,row,129,RULES);
        ruleNum++;
    }
    fclose(fp);
    return RULES;
}




/*PARSE TREE LOGIC*/


TREENODE createRootNode(LISTNODE ln) {
    TREENODE root = (TREENODE)malloc(sizeof(struct TreeNode));
    PT_SIZE += sizeof(struct TreeNode);
    root->child = NULL;
    root->next = NULL;
    root->isTerminal = 0;
    root->TREENODEDATA = (union TreeNodeData*)malloc(sizeof(union TreeNodeData));
    PT_SIZE += sizeof(union TreeNodeData);
    root->parent = NULL;
    if (ln->isTerminal == 0) {
        root->TREENODEDATA->nonterminal = ln->NODETYPE->nonterminal;
    }
    return root;
}

TREENODE insertChildTree(TREENODE tn, LISTNODE ln){
    TREENODE childHead = (TREENODE) malloc(sizeof(struct TreeNode));
    PT_SIZE+=sizeof(struct TreeNode);
    tn->child = childHead;
    childHead->next = NULL;
    childHead->child = NULL;
    childHead->ruleNum = ln->ruleNum;
    childHead->isTerminal = ln->isTerminal;
    childHead->TREENODEDATA = (union TreeNodeData*) malloc(sizeof(union TreeNodeData));
    PT_SIZE+=sizeof(union TreeNodeData);
    childHead->parent = tn;
    childHead->addr = NULL;
    childHead->addr_inh = NULL;
    childHead->addr_syn = NULL;
    childHead->list_addr_syn=NULL;
    childHead->isArray = 0;
    childHead->type = -1;
    if(ln->isTerminal == 0){
        childHead->TREENODEDATA->nonterminal = ln->NODETYPE->nonterminal;
    }
    else{
        childHead->TREENODEDATA->terminal = malloc(sizeof(LEXEME));
        PT_SIZE+=sizeof(LEXEME);
        childHead->TREENODEDATA->terminal->lexemedata = (union lexemeData*) malloc(sizeof(union lexemeData));
        PT_SIZE += sizeof(union lexemeData);

    }
    return childHead;
}

TREENODE insertNextTree(TREENODE tn, LISTNODE ln){
    TREENODE nextNode = (TREENODE) malloc(sizeof(struct TreeNode));
    PT_SIZE += sizeof(struct TreeNode);
    tn->next = nextNode;
    nextNode->next = NULL;
    nextNode->child = NULL;
    nextNode->ruleNum = ln->ruleNum;
    nextNode->isTerminal = ln->isTerminal;
    nextNode->addr = NULL;
    nextNode->addr_inh = NULL;
    nextNode->addr_syn = NULL;
    nextNode->list_addr_syn=NULL;
    nextNode->isArray = 0;
    nextNode->type = -1;
    nextNode->TREENODEDATA = (union TreeNodeData*) malloc(sizeof(union TreeNodeData));
    PT_SIZE += sizeof(union TreeNodeData);
    nextNode->parent = tn->parent;
    if(ln->isTerminal == 0){
        nextNode->TREENODEDATA->nonterminal = ln->NODETYPE->nonterminal;
    }
    else{
        nextNode->TREENODEDATA->terminal = (LEXEME*) malloc(sizeof(LEXEME));
        nextNode->TREENODEDATA->terminal->lexemedata = (union lexemeData*) malloc(sizeof(union lexemeData));
        PT_SIZE = PT_SIZE + sizeof(LEXEME) + sizeof(union lexemeData);
    }
    return nextNode;
}

void treeNodePrint(TREENODE node){
    if(node == NULL) printf("NODE IS NULL\n\n");
    else if(node->TREENODEDATA->terminal->token == NUM_TOKEN) printf("%d  ",node->TREENODEDATA->terminal->lexemedata->intData);
    else if(node->TREENODEDATA->terminal->token == RNUM_TOKEN) printf("%f  ",node->TREENODEDATA->terminal->lexemedata->floatData);
    else if(node->TREENODEDATA->terminal->token == IDENTIFIER_TOKEN) printf("%s  ",node->TREENODEDATA->terminal->lexemedata->data);
    else printf("%s  ",node->TREENODEDATA->terminal->lexemedata->data);
}

void inorderTraversal(TREENODE tn, short goNext){
    if(tn == NULL) return;
    inorderTraversal(tn->child,0);
    if(tn->isTerminal == 1){
        // printf("T\n");
        if(tn->TREENODEDATA->terminal != NULL){ 
            if(tn->TREENODEDATA->terminal->token == EOF_TOKEN) return;
            treeNodePrint(tn);
            printf( "       %d     ",tn->TREENODEDATA->terminal->lineNo);
            printf( "       %s     ",TERMINALS_STRINGS[tn->TREENODEDATA->terminal->token]);
            if(tn->TREENODEDATA->terminal->token == NUM_TOKEN){
                printf( "       %d     ",tn->TREENODEDATA->terminal->lexemedata->intData);
            }
            else if(tn->TREENODEDATA->terminal->token == RNUM_TOKEN){
                printf( "       %lf     ",tn->TREENODEDATA->terminal->lexemedata->floatData);
            }
            if(tn->parent) printf( "%s      ",NONTERMINALS_STRINGS[tn->parent->TREENODEDATA->nonterminal]);
            else printf( "%s    ","ROOT");
            printf("YES\n\n");
        }
        else printf("Some Terminal Node with Error\n");
    }
    else if(tn->isTerminal == 0){
        // printf("NT\n");
        printf( "       ----------     ");
        printf( "       %s     ",NONTERMINALS_STRINGS[tn->TREENODEDATA->nonterminal]);
        if(tn->parent) printf( "        %s      ",NONTERMINALS_STRINGS[tn->parent->TREENODEDATA->nonterminal]);
        else printf( "      %s\t      ","ROOT");
        printf("NO\n\n");
    }
    else{
        printf("       epsilon     epsilon");
        if(tn->parent) printf( "        %s      ",NONTERMINALS_STRINGS[tn->parent->TREENODEDATA->nonterminal]);
        else printf( "      %s\t      ","ROOT");
        printf("YES\n\n");
    }
    if(tn->child != NULL){
        // printf("GOING RIGHT OF CHILD\n");
        inorderTraversal(tn->child->next,1);
        // printf("GOING LEFT TO CHILD\n");
    }
    if(goNext == 1){
        inorderTraversal(tn->next, 1);
    }
    
}

int countParseTreeNodes(TREENODE tn, short goNext){
    int sum = 0;
    if(tn == NULL) return 0 ;
    sum += countParseTreeNodes(tn->child,0);
    sum +=1;
    if(tn->child != NULL){
        sum+=countParseTreeNodes(tn->child->next,1);
    }
    if(goNext == 1){
        sum+=countParseTreeNodes(tn->next, 1);
    }
    return sum;
    
}


/*STACK ADT LOGIC*/

STACK createStack() {
    STACK st = (STACK)malloc(sizeof(struct Stack));
    st->top = NULL;
    st->size = 0;
    return st;
}

void pushInStack(STACK st, LISTNODE node, TREENODE tn, short pushChild) {
    if (node == NULL) return;

    STACKNODE newTop = (STACKNODE)malloc(sizeof(struct StackNode));

    TREENODE treenode;
    if (pushChild == 1) {
        treenode = insertChildTree(tn, node);
    } else {
        treenode = insertNextTree(tn, node);
    }

    newTop->next = NULL;
    newTop->NODETYPE = node->NODETYPE;
    newTop->isTerminal = node->isTerminal;
    pushInStack(st, node->next, treenode, 0);
    newTop->treenode = treenode;
    newTop->next = st->top;
    st->top = newTop;
    ++st->size;
}

short isStackEmpty(STACK st){
    return st->size == 0;
}

STACKNODE popFromStack(STACK st){
    if(isStackEmpty(st)){
        printf("Stack Underflow\n");
        return NULL;
    }
    STACKNODE currTop = st->top;
    st->top = currTop->next;
    currTop->next = NULL;
    st->size --;
    return currTop;
}




/*FIRST AND FOLLOW AUTOMATION LOGIC*/



int FIRSTANDFOLLOWSETS[129][20] = {0};
int FOLLOWSETS[129][20] = {0};
int SYNCSET[70][20] = {0};
int vis[129] = {0};

int check1(int i){
    for(int j = 1;j<=FOLLOWSETS[i][0];j++){
        if(FOLLOWSETS[i][j] == SEMICOL_OPERATOR){
            return 0;
        }
    }
    return 1;
}


int check(int i, int val)
{
    for (int j = 1; j <= FOLLOWSETS[i][0]; j++)
    {
        if (FOLLOWSETS[i][j] == val)
            return 0;
    }
    return 1;
}

void follow(int numRules, LISTNODE *RULES, int i, int *vis1)
{
    // if (vis1[i] == 1)
    //     return;
    // printf("follow for %d \n", i);
    // vis1[i] = 1;

    for (int j = 0; j < numRules; j++)
    {
        // printf("(i,j) trying to match for %d , in rule %d \n",i,j);
        LISTNODE temp = RULES[j];
        temp = temp->next;
        while (temp)
        {
            if (temp->isTerminal == 0 && temp->NODETYPE->nonterminal == RULES[i]->NODETYPE->nonterminal)
            {
                // vis1[j] = 1;

            god:

                if (temp->next)
                {
                    LISTNODE help = temp->next;
                    if (help->isTerminal == 1)
                    {
                        // printf("(TERMINAL) in rule number %d ,my %d was terminal \n",j+1,i+1);
                        if (check(i, (int)help->NODETYPE->terminal))
                        {
                            FOLLOWSETS[i][0]++;
                            FOLLOWSETS[i][FOLLOWSETS[i][0]] = (int)help->NODETYPE->terminal;
                        }
                    }

                    else
                    {
                        for (int z = 0; z < numRules; z++)
                        {
                            // printf("(j,z) trying to match for %d , in rule %d \n",j,z);
                            if (help->isTerminal == 0 && help->NODETYPE->nonterminal == RULES[z]->NODETYPE->nonterminal)
                            {
                                LISTNODE help1 = RULES[z];
                                help1 = help1->next;

                                if (help1->isTerminal != -1)
                                {
                                    // printf("(NON-TERMINAL) in rule number %d , called by %d  ,working for %d\n ",z+1,j+1,i+1);

                                    vis1[z] = 1;
                                    for (int k = 1; k <= FIRSTANDFOLLOWSETS[z][0]; k++)
                                    {

                                        if (check(i, FIRSTANDFOLLOWSETS[z][k]))
                                        {
                                            FOLLOWSETS[i][0]++;
                                            FOLLOWSETS[i][FOLLOWSETS[i][0]] = FIRSTANDFOLLOWSETS[z][k];
                                        }
                                    }
                                }

                                else
                                {
                                    if (temp->next)
                                    {
                                        temp = temp->next;
                                        goto god;
                                    }
                                    else
                                    {
                                        follow(numRules, RULES, z, vis1);
                                        for (int k = 1; k <= FOLLOWSETS[z][0]; k++)
                                        {
                                            if (check(i, FOLLOWSETS[z][k]))
                                            {
                                                FOLLOWSETS[i][0]++;
                                                FOLLOWSETS[i][FOLLOWSETS[i][0]] = FOLLOWSETS[z][k];
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                else
                {
                king:
                    if (FOLLOWSETS[j][0] == 0)
                        follow(numRules, RULES, j, vis1);

                    for (int k = 1; k <= FOLLOWSETS[j][0]; k++)
                    {
                        if (check(i, FOLLOWSETS[j][k]))
                        {
                            FOLLOWSETS[i][0]++;
                            FOLLOWSETS[i][FOLLOWSETS[i][0]] = FOLLOWSETS[j][k];
                        }
                    }
                }
            }
            temp = temp->next;
        }
    }
}

void find(int *vis, LISTNODE *RULES, int i, int prev_i, LISTNODE prev_curr, int *follow_arr)
{
    // printf("i = %d, prev_i = %d\n",i,prev_i);
    if (vis[i] == 1)
    {
        return;
    }

    int numRules = 129;
    LISTNODE curr = RULES[i];
    // curr = curr->next;

label:
    if (curr->next)
        curr = curr->next;

    if (curr->isTerminal == 1)
    {
        // printf("THIS IS TERMINAL\n");
        int terminal = (int)curr->NODETYPE->terminal;
        vis[i] = 1;
        FIRSTANDFOLLOWSETS[i][0]++;
        FIRSTANDFOLLOWSETS[i][FIRSTANDFOLLOWSETS[i][0]] = terminal;
        // printf("RETURNED\n");
        return;
    }

    if (curr->isTerminal == 0)
    {
        vis[i] = 1;
        for (int j = 0; j < numRules; j++)
        {
            if (RULES[j]->NODETYPE->nonterminal == curr->NODETYPE->nonterminal)
            {

                if (vis[j] != 1 && vis[j] != 2)
                {
                    find(vis, RULES, j, i, curr, follow_arr);
                }

                if (vis[j] == 2)
                {
                    LISTNODE helper = curr;
                    while (helper->NODETYPE->nonterminal != RULES[j]->NODETYPE->nonterminal)
                    {
                        helper = helper->next;
                    }
                    curr = helper;
                    goto label;
                }

                for (int k = FIRSTANDFOLLOWSETS[i][0] + 1; k <= FIRSTANDFOLLOWSETS[i][0] + FIRSTANDFOLLOWSETS[j][0]; k++)
                {
                    FIRSTANDFOLLOWSETS[i][k] = FIRSTANDFOLLOWSETS[j][k - FIRSTANDFOLLOWSETS[i][0]];
                }
                FIRSTANDFOLLOWSETS[i][0] += FIRSTANDFOLLOWSETS[j][0];
            }
        }
    }

    else
    {
        if (vis[i] != 2)
        {
            vis[i] = 2;
            follow_arr[0]++;
            follow_arr[follow_arr[0]] = i;
            // printf("WILL NEED FOLLOW\n");
            return;
        }
    }
}

void automateFirstandFollow(LISTNODE *RULES)
{

    int numRules = 129;
    int follow_arr[50] = {0};

    for (int i = 0; i < numRules; i++)
    {
        find(vis, RULES, i, -1, NULL, follow_arr);
        // printf("%d\n",i);
    }

    for (int i = 1; i <= follow_arr[0]; i++)
    {
        int vis1[129] = {0};
        FOLLOWSETS[0][0] = 1;
        FOLLOWSETS[0][1] = EOF_TOKEN;
        vis1[0] = 1;
        follow(numRules, RULES, follow_arr[i], vis1);
        // printf(" %d \n", follow_arr[i]);

        for (int j = 0; j <= FOLLOWSETS[follow_arr[i]][0]; j++)
        {

            FIRSTANDFOLLOWSETS[follow_arr[i]][j] = FOLLOWSETS[follow_arr[i]][j];
        }
        // printf("-----------------------------\n");
    }
    printf("\n                      \x1B[32m FIRST AND FOLLOW AUTOMATED \x1B[0m\n");
    for(int i = 0 ; i < numRules ; i++){
        int vis1[129] = {0};
        FOLLOWSETS[0][0] = 1;
        FOLLOWSETS[0][1] = EOF_TOKEN;
        vis1[0] = 1;
        follow(numRules , RULES , i , vis1);
        if(check1(i)){
            FOLLOWSETS[i][0]++;
            FOLLOWSETS[i][FOLLOWSETS[i][0]]=SEMICOL_OPERATOR;
        }

    }
    for (int i = 0; i < 129; i++)
    { 
        for (int j = 0; j <= FOLLOWSETS[i][0]; j++)
        {   
            SYNCSET[RULES[i]->NODETYPE->nonterminal][j] = FOLLOWSETS[i][j];
            // printf("%s, ", TERMINALS_STRINGS[FOLLOWSETS[i][j]]);
        }
        // printf("\n");
    }
}


int inSyncSet(int terminal,int nonterminal){
    int n = SYNCSET[nonterminal][0];
    for(int j = 1;j<=n;j++){
        if(terminal == SYNCSET[nonterminal][j]){
            return 1;
        }
    }
    return 0;
}


/*PARSE TABLE LOGIC*/

void fillParseTable() {
    for (short int i = 0; i < 70; i++) {
        for (short int j = 0; j < 214; j++) {
            PARSETABLE[i][j] = -1;
        }
    }
}

void populateParseTable(LISTNODE* RULES) {
    fillParseTable();
    automateFirstandFollow(RULES);

    short int numRules = 129;
    for (short int i = 0; i < numRules; i++) {
        LISTNODE head = RULES[i];
        short int row = head->NODETYPE->nonterminal;
        short int size = FIRSTANDFOLLOWSETS[i][0];
        for (short int j = 1; j <= size; j++) {
            short int col = FIRSTANDFOLLOWSETS[i][j];
            PARSETABLE[row][col] = i;
        }
    }
}

/*ERROR HANDLING IS PARSER*/


LEXEME* errorHandling(STACK st, LEXEME* lex, short type, STACKNODE stNode, TwinBuffer* TB) {
    printf("\n \x1B[1m\033[31m PARSING ERROR At line %d, ", lex->lineNo);
    error = 1;
    if (type == 1) {
        printf("PARSER GOT INVALID TOKEN \"%s\" , EXPECTED \"%s\"", TERMINALS_STRINGS[lex->token], TERMINALS_STRINGS[stNode->NODETYPE->terminal]);
    }
    if (type == 2) {
        printf("PARSER GOT INVALID TOKEN \"%s\" , CAN NOT BE DERIVED USING %s", TERMINALS_STRINGS[lex->token], NONTERMINALS_STRINGS[stNode->NODETYPE->nonterminal]);
    }
    if (type == 4) {
        printf("INPUT IS YET TO BE PROCESSED \"%s\" ", TERMINALS_STRINGS[lex->token]);
    }
    printf("\033[0m\033[0m\n\n");
    while (!isStackEmpty(st) && st->top->isTerminal == 1) {
        popFromStack(st);
    }
    if (isStackEmpty(st) == 1) {
        return lex;
    }
    stNode = st->top;
    while (!inSyncSet(lex->token, stNode->NODETYPE->nonterminal)) {
        lex = simulateDFA(TB, 0);
        if (lex->token == EOF_TOKEN) return lex;
    }
    if (lex->token == SEMICOL_OPERATOR) {
        lex = simulateDFA(TB, 0);
    }
    if (PARSETABLE[stNode->NODETYPE->nonterminal][lex->token] == -1) {
        popFromStack(st);
    }
    return lex;
}

/*PARSER CODE*/
REENODE parser(char* grammarFile, char* inputFile, int size) {
    LISTNODE* RULES = addRules(grammarFile);
    populateParseTable(RULES);
    TwinBuffer* TB = initializeTwinBuffer(inputFile, size);
    STACK st = createStack();
    TREENODE root = createRootNode(RULES[0]);
    pushInStack(st, RULES[0]->next, root, 1);
    LEXEME* lex = simulateDFA(TB, 0);
    STACKNODE stNode;
    short canContinue = 0;
    while (st->size > 0) {
        stNode = popFromStack(st);

        /* Checking for the terminal */

        if(stNode->isTerminal == 1){
            // printf("Popped Terminal %s\n",TERMINALS_STRINGS[stNode->NODETYPE->terminal]);
            if(lex->token == stNode->NODETYPE->terminal){
                // printf("COPYING EVERYTHING\n");
                stNode->treenode->TREENODEDATA->terminal->lineNo = lex->lineNo;
                stNode->treenode->TREENODEDATA->terminal->token = lex->token;
                if(lex->token == NUM_TOKEN){
                    stNode->treenode->TREENODEDATA->terminal->lexemedata->intData = lex->lexemedata->intData;
                }
                else if(lex->token == RNUM_TOKEN){
                    stNode->treenode->TREENODEDATA->terminal->lexemedata->floatData = lex->lexemedata->floatData;
                }
                else{
                    if(lex->token !=  EOF_TOKEN){
                        // printf("STRING COPY\n");
                        stNode->treenode->TREENODEDATA->terminal->lexemedata->data = (char*) malloc(strlen(lex->lexemedata->data)+1);
                        // printf("%s\n\n\n\n",lex->lexemedata->data);
                        strcpy(stNode->treenode->TREENODEDATA->terminal->lexemedata->data,lex->lexemedata->data);
                        // stNode->treenode->TREENODEDATA->terminal->lexemedata->data =lex->lexemedata->data;
                    }
                }
                // printf("COPYIED EVERYTHING\n");
                free(lex->lexemedata);
                free(lex);
                // if(stNode->treenode->isTerminal == 1 && stNode->treenode->TREENODEDATA->terminal->token == IDENTIFIER_TOKEN)printf("%s\n",stNode->treenode->TREENODEDATA->terminal->lexemedata->data);
                lex = simulateDFA(TB,0);
                // if(stNode->treenode->isTerminal == 1 && stNode->treenode->TREENODEDATA->terminal->token == IDENTIFIER_TOKEN)printf("%s\n",stNode->treenode->TREENODEDATA->terminal->lexemedata->data);
                // printf("MATCHED\n");
            }
            else{
                lex = errorHandling(st,lex,1,stNode,TB);
            }
        }
        else if(stNode->isTerminal == 0){
            // printf("Popped Non Terminal %s\n",NONTERMINALS_STRINGS[stNode->NODETYPE->terminal]);
            if(PARSETABLE[stNode->NODETYPE->nonterminal][lex->token] != -1){

                pushInStack(st,RULES[PARSETABLE[stNode->NODETYPE->nonterminal][lex->token]]->next,stNode->treenode,1);
                
                if(RULES[PARSETABLE[stNode->NODETYPE->nonterminal][lex->token]]->next->isTerminal == -1) stNode = popFromStack(st);

            }
            else{
                lex = errorHandling(st,lex,2,stNode,TB);
            }
        }
        free(stNode);
    }

    if(lex->token != EOF_TOKEN) errorHandling(st,lex,4,stNode,TB);
    if(error!=1){ 
        printf("\nGIVEN SOURCE CODE IS SYNTACTICALLY CORRECT\n\n\n");
    }
    else{
        return NULL;
    }
    fclose(TB->fp);
    free(RULES);
    cleanTwinBuffer(TB);
    return root;
}

/*
FOLLOWING ARE THE FUNCTIONS OF PARSE TREE REQUIRED FOR AST CREATION
*/


TREENODE getChildNonTerminal(int nt,TREENODE tn){
    // printf("33434\n");
    if(tn->child == NULL){ 
        // printf("33434\n");

        // printf("NODE = %s\n",NONTERMINALS_STRINGS[tn->TREENODEDATA->nonterminal]);
        return NULL;
    }
    TREENODE ch1 = tn->child;
    while(ch1!=NULL){
        if(ch1->isTerminal == 0 && ch1->TREENODEDATA->nonterminal == nt){
            // printf("NODE = %s\n",NONTERMINALS_STRINGS[ch1->TREENODEDATA->nonterminal]);
            return ch1;
        }
        ch1 = ch1->next;
    }
    return NULL;
}

TREENODE getChildTerminal(int nt,TREENODE tn){
    // printf("TERMINAL LENE AAYA HU\n");
    if(tn->child == NULL) return NULL;
    TREENODE ch1 = tn->child;
    // printf("%d\n",ch1->ruleNum);
    // printf("%s\n",NONTERMINALS_STRINGS[tn->TREENODEDATA->nonterminal]);
    while(ch1!=NULL){
        // if(ch1->isTerminal == 1) printf("%s\n",TERMINALS_STRINGS[ch1->TREENODEDATA->terminal->token]);
        // if(ch1->isTerminal == 0) printf("%s\n",NONTERMINALS_STRINGS[ch1->TREENODEDATA->nonterminal]);
        if(ch1->isTerminal == 1 && ch1->TREENODEDATA->terminal->token == nt){
            // printf("CLEARING %s IN LINE %d\n",TERMINALS_STRINGS[ch1->TREENODEDATA->terminal->token],ch1->TREENODEDATA->terminal->lineNo);
            return ch1;
        }
        // printf("NEXT PE JA RHA HU\n");
        ch1 = ch1->next;
    }
    return NULL;
}

TREENODE* getDualNonTerminal(int nt,TREENODE tn){
    // printf("DUAL LENE AAYA HU\n");
    TREENODE* dual = (TREENODE*) malloc(sizeof(TREENODE)*2);
    TREENODE ch1 = tn->child;
    while(ch1!=NULL){
        // if(ch1->isTerminal == 1) printf("%s\n",TERMINALS_STRINGS[ch1->TREENODEDATA->terminal->token]);
        // if(ch1->isTerminal == 0) printf("%s\n",NONTERMINALS_STRINGS[ch1->TREENODEDATA->nonterminal]);
        if(ch1->isTerminal == 0 && ch1->TREENODEDATA->nonterminal == nt){
            dual[0] = ch1;
            break;
        }
        ch1 = ch1->next;
    }
    ch1 = ch1->next;
    while(ch1!=NULL){
        // if(ch1->isTerminal == 1) printf("%s\n",TERMINALS_STRINGS[ch1->TREENODEDATA->terminal->token]);
        // if(ch1->isTerminal == 0) printf("%s\n",NONTERMINALS_STRINGS[ch1->TREENODEDATA->nonterminal]);
        if(ch1->isTerminal == 0 && ch1->TREENODEDATA->nonterminal == nt){
            dual[1] = ch1;
            return dual;
        }
        ch1 = ch1->next;
    }
    return NULL;
}

TREENODE* getDualTerminal(int nt,TREENODE tn){
    TREENODE* dual = (TREENODE*) malloc(sizeof(TREENODE)*2);
    TREENODE ch1 = tn->child;
    while(ch1!=NULL){
        // if(ch1->isTerminal == 1) printf("%s\n",TERMINALS_STRINGS[ch1->TREENODEDATA->terminal->token]);
        // if(ch1->isTerminal == 0) printf("%s\n",NONTERMINALS_STRINGS[ch1->TREENODEDATA->nonterminal]);
        if(ch1->isTerminal == 1 && ch1->TREENODEDATA->terminal->token == nt){
            dual[0] = ch1;
            break;
        }
        ch1 = ch1->next;
    }
    ch1 = ch1->next;
    while(ch1!=NULL){
        // if(ch1->isTerminal == 1) printf("%s\n",TERMINALS_STRINGS[ch1->TREENODEDATA->terminal->token]);
        // if(ch1->isTerminal == 0) printf("%s\n",NONTERMINALS_STRINGS[ch1->TREENODEDATA->nonterminal]);
        if(ch1->isTerminal == 1 && ch1->TREENODEDATA->terminal->token == nt){
            dual[1] = ch1;
            return dual;
        }
        ch1 = ch1->next;
    }
    return NULL;
}
