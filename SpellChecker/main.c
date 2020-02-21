#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BSTNode
{
    struct BSTNode *left;
    struct BSTNode *right;
    char word[50];
} BSTNode;

BSTNode *NewNode(char newWord[])
{
    BSTNode *newNode = (BSTNode *)malloc(sizeof(BSTNode));
    strcpy(newNode->word,newWord);
    newNode -> left = newNode -> right = NULL;
    return newNode;
}

int size(BSTNode *root)
{
    return (root == NULL) ? 0 : 1+size(root->left)+size(root->right);
}

int MAX (int x, int y)
{
    return x>y? x : y;
}

int height(BSTNode *root)
{
    return (root == NULL) ? -1 : 1+MAX(height(root->left),height(root->right));
}

BSTNode* front(BSTNode *root) //finding the minimum value
{
    if(root == NULL)
        return;
    while (root->left != NULL)
        root = root->left;
    return root;
}

BSTNode* back(BSTNode *root) //finding the maximum value
{
    if(root == NULL)
        return;
    while (root->right != NULL)
        root = root->right;
    return root;
}

int isFound(BSTNode *root, char newWord[])
{
    if (root == NULL)
        return 0;
    if (strcmp(newWord,root->word)==0)
        return 1;
    else if (strcmp(newWord,root->word)>0)
        isFound(root->right,newWord);
    else
        isFound(root->left,newWord);
}

BSTNode* findParent(BSTNode *child, BSTNode *root)
{
    BSTNode* parent = root;
    while(parent!=NULL)
    {
        if((parent->left==child) || (parent->right==child))
            return parent;
        else if (strcmp(parent->word,child->word)>0)
            parent=parent->left;
        else if (strcmp(child->word,parent->word)>0)
            parent=parent->right;

    }
    return;
}

BSTNode* insert(BSTNode *p, char newWord[])
{
    if (p == NULL)
        p = NewNode(newWord);
    else if (strcmp(p->word,newWord)>0)
        p->left = insert(p->left, newWord);
    else if (strcmp(newWord,p->word)>0)
        p->right = insert(p->right, newWord);
    return p;
}

void SpellChecker (char *filename)
{
    FILE * fp = fopen(filename,"r");
    if (fp == NULL)
    {
        printf("File not found!");
        exit(0);
    }
    char newWord[50];
    BSTNode *root = NULL;
    //creating the tree
    while(!feof(fp))
    {
        fscanf(fp,"%s",newWord);
        root = insert(root,newWord);
    }

    printf("Height: %d\n\nSize: %d\n\nWord - CORRECT\nWord - leaf, successor, predecessor.\n~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\n~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\n\n",height(root),size(root));
    while(1)
    {
        char input[200];
        printf("Enter sentance: ");
        gets(input);
        char *token;
        token = strtok(input," ");
        while( token != NULL )
        {
            printf( "%s - ", token );
            if(isFound(root,token))
                printf("CORRECT\n");
            else
                mistakes(root,token);
            token = strtok(NULL," ");
        }
        printf("\n");
    }
}

void mistakes(BSTNode *p, char wrongWord[])
{
    BSTNode *parent;
    BSTNode *root = p;

    //FINDING LEAF
    while(p != NULL)
    {
        if(strcmp(wrongWord,p->word)>0)
        {
            if(p->right!=NULL)
                p=p->right;
            else
                break;
        }

        else
        {
            if(p->left!=NULL)
                p=p->left;
            else
                break;
        }
    }
    printf("%s, ",p->word); //PRINTING LEAF

    //FINDING SUCCESSOR
    BSTNode *successorNode;
    BSTNode *temp1 = p;
    BSTNode *temp2 = p;

    if(temp1 == back(root))
        printf("%s, ",temp1->word);
    else
    {
        parent = findParent(p,root);
        if(temp1->right != NULL)
        {
            successorNode=front(temp1->right);
        }

        else if(parent->left==p)
            successorNode=parent;
        else
        {
            while(parent->right==temp1)
            {
                temp1=parent;
                parent=findParent(temp1,root);
            }
            successorNode=parent;
        }
        printf("%s, ",successorNode->word); //PRINTING SUCCESSOR
    }

    //FINDING PREDECESSOR
    BSTNode *predecessorNode;
    if(temp2 == front(root))
        printf("%s.",temp2->word);
    else
    {
        parent=findParent(temp2,root);
        if(temp2->left != NULL)
        {
            predecessorNode=back(temp2->left);
        }

        else if(parent->right==p)
            predecessorNode=parent;
        else
        {
            while(parent->left==temp2)
            {
                temp2=parent;
                parent=findParent(temp2,root);
            }
            predecessorNode=parent;
        }
        printf("%s.",predecessorNode->word); //PRINTING PREDECESSOR
    }
    printf("\n");
}

int main(int argc, char**argv)
{
    if(argc!=2)
    {
        printf("Usage: %s filename\n", argv[0]);
        exit(1);
    }

    SpellChecker(argv[1]);

    return 0;
}
