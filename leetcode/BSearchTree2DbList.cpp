#include <stdio.h>
#include <typeinfo>

typedef struct BSTreeNode {
    int value;
    BSTreeNode *left;
    BSTreeNode *right;
} DoubleList;

DoubleList *pHead;
DoubleList *pListIndex;

void
addBSTreeNode(BSTreeNode *&pCurrent, int value)
{
    if(NULL == pCurrent) {
        BSTreeNode *pBSTree = new BSTreeNode();
        pBSTree->left = NULL;
        pBSTree->right = NULL;
        pBSTree->value = value;
        pCurrent = pBSTree;
    } else {
        if(pCurrent->value > value)
            addBSTreeNode(pCurrent->right, value);
        else if(pCurrent->value < value)
            addBSTreeNode(pCurrent->left, value);
        else
            printf("Insert same number\n");
    }
}

void
convert2DbList(BSTreeNode *pCurrent)
{
    pCurrent->left = pListIndex;
    if(NULL != pListIndex)
        pListIndex->right = pCurrent;
    else
        pHead = pCurrent;

    pListIndex = pCurrent;
    printf("%d\n", pCurrent->value);
}

void
ergodicBSTree(BSTreeNode *pCurrent)
{
    if(NULL == pCurrent)
        return;

    if(NULL != (pCurrent->left))
        ergodicBSTree(pCurrent->left);

    convert2DbList(pCurrent);

    if(NULL != (pCurrent->right))
        ergodicBSTree(pCurrent->right);
}

int
main(int argc, char **argv)
{
    printf("%s\n",typeid(BSTreeNode).name());
    printf("%s\n", typeid(pHead).name());
    return 0;
}

