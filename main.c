//Equivalent grammar that is parsable by a recursive-descent parser:
//T() => <T>  -> <T><TT>
//TT() => <TT> -> +-<T><TT>|e
//T() => <T>  -> <F><B>
//FT()=> <FT> -> */<F><B>|e
//F() => <F>  -> <N>|(<E>)
//N() => <N>  -> <D><NT>
//NT()=> <NT> -> <N> | e
//D() => 0|...|9

#include <stdio.h>
#include <stdlib.h>

typedef struct NODE *TREE;
TREE parseTree;
char *nextSym;
TREE E(void);
TREE TT(void);
TREE T(void);
TREE FT(void);
TREE F(void);
TREE N(void);
TREE NT(void);
TREE D(void);
TREE makeNode0(char x);
TREE makeNode1(char x, TREE t);
TREE makeNode2(char x, TREE t1, TREE t2);
TREE makeNode3(char x, TREE t1, TREE t2, TREE t3);

struct NODE {
    char label;
    TREE leftmostChild, rightSibling;
};
void displayTree(TREE n, int level);
int main(int argc, const char * argv[]) {
    
    //Test different inputs
    nextSym = "(3-2)*4/(6-1*3*(7-1))";
    
    parseTree = E();
    displayTree(parseTree,0);
    printf("\n");
    return 0;
}


void displayTree(TREE n, int level)
{
    TREE c;
    
    for(int i = 1; i<=level; i++) {
        printf("\t\t");
    }
    switch (n->label) {
        case 'W':
            printf("TT\n");
            break;
        case 'B':
            printf("FT\n");
            break;
        case 'M':
            printf("NT\n");
            break;
            
        default:
            printf("%c\n", n->label);
    }
    c=n->leftmostChild;
    
    while (c != NULL)
    {
        displayTree(c, level+1);
        c = c->rightSibling;
    }
}

int nextTerminal(char c)
{
    return *nextSym == c;
}

int matchTerminal(char c)
{
    if (*nextSym == c) {
        nextSym++;
        return 1;
    } else {
        return 0;
    }
}

TREE makeNode0(char x)
{
    TREE root;
    root = (TREE)malloc(sizeof(struct NODE));
    root->label=x;
    root->leftmostChild=NULL;
    root->rightSibling=NULL;
    return root;
}

TREE makeNode1(char x, TREE t)
{
    TREE root = makeNode0(x);
    root->leftmostChild=t;
    return root;
}

TREE makeNode2(char x, TREE t1, TREE t2)
{
    TREE root = makeNode1(x, t1);
    t1->rightSibling=t2;
    return root;
}

TREE makeNode3(char x, TREE t1, TREE t2, TREE t3)
{
    TREE root = makeNode1(x, t1);
    t1->rightSibling=t2;
    t2->rightSibling=t3;
    return root;
}

TREE E()
{
    TREE firstB,secondB;
    
    firstB = T();
    if (firstB == NULL)
        return NULL;
    
    secondB = TT();
    if (secondB == NULL) {
        return NULL;
    }
    
    return makeNode2('E', firstB, secondB);
}

TREE TT()
{
    TREE firstB, secondB;
    
    if (!nextTerminal('+') && !nextTerminal('-'))
    {
        return makeNode1('W', makeNode0('e'));
    }
    else if (matchTerminal('+'))
    {
        firstB = T();
        if(firstB == NULL)
            return NULL;
        
        secondB = FT();
        if(secondB == NULL)
            return NULL;
        
        return makeNode3('W',makeNode0('+'), firstB, secondB);
    }
    else if (matchTerminal('-'))
    {
        firstB = F();
        if(firstB == NULL)
            return NULL;
        
        secondB = FT();
        if(secondB == NULL)
            return NULL;
        
        return makeNode3('W',makeNode0('-'), firstB, secondB);
    }
    else
        return NULL;
}

TREE T()
{
    
    TREE firstB, secondB;
    
    firstB = F();
    if(firstB == NULL)
        return NULL;
    
    secondB = FT();
    if(secondB == NULL)
        return NULL;
    
    return makeNode2('T', firstB, secondB);
}

TREE FT(){
    
    if (!nextTerminal('*') && !nextTerminal('/'))
    {
        return makeNode1('B', makeNode0('e'));
    }
    else if (matchTerminal('*'))
    {
        TREE firstB, secondB;
        
        firstB = F();
        if(firstB == NULL)
            return NULL;
        
        secondB = FT();
        if(secondB == NULL)
            return NULL;
        
        return makeNode3('B', makeNode0('*'), firstB, secondB);
    }
    else if (matchTerminal('/'))
    {
        TREE firstB = F();
        if(firstB == NULL)
            return NULL;
        TREE secondB = FT();
        if(secondB == NULL)
            return NULL;
        return makeNode3('B', makeNode0('/'), firstB, secondB);
        
    }
    else
        return NULL;
    
}

TREE F(){
    
    TREE firstB, secondB;
    
    if(nextTerminal('(')) {
        
        if(!matchTerminal('('))
            return NULL;
        firstB = E();
        if(firstB == NULL)
            return NULL;
        if(!matchTerminal(')'))
            return NULL;
        return makeNode3('E',makeNode0('('), firstB, makeNode0(')'));
        
    } else {
        
        secondB = N();
        if(secondB == NULL)
            return NULL;
        return makeNode1('N', secondB);
        
    }
}


TREE N(){
    
    TREE firstB, secondB;
    
    firstB = D();
    if(firstB == NULL)
        return NULL;
    
    secondB = NT();
    if(secondB == NULL)
        return NULL;
    
    return makeNode2('N', firstB, secondB);
}

TREE NT()
{
    TREE firstB = N();
    if (firstB == NULL)
        return makeNode1('M', makeNode0('e'));
    return makeNode1('M', firstB);
}

TREE D()
{
    if(matchTerminal('0')){
        return makeNode1('D',makeNode0('0'));
    } else if(matchTerminal('1')){
        return makeNode1('D',makeNode0('1'));
    } else if(matchTerminal('2')){
        return makeNode1('D',makeNode0('2'));
    } else if(matchTerminal('3')){
        return makeNode1('D',makeNode0('3'));
    } else if(matchTerminal('4')){
        return makeNode1('D',makeNode0('4'));
    } else if(matchTerminal('5')){
        return makeNode1('D',makeNode0('5'));
    } else if(matchTerminal('6')){
        return makeNode1('D',makeNode0('6'));
    } else if(matchTerminal('7')){
        return makeNode1('D',makeNode0('7'));
    } else if(matchTerminal('8')){
        return makeNode1('D',makeNode0('8'));
    } else if(matchTerminal('9')){
        return makeNode1('D',makeNode0('9'));
    } else return NULL;
    
}
