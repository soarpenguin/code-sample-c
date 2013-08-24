#ifndef _AVLBINTREE_H
#define _AVLBINTREE_H

#define ElementType int
struct AvlTreeNode {
    ElementType value;
    int height;
    struct AvlTreeNode *pleft;
    struct AvlTreeNode *pright;
};

typedef struct AvlTreeNode *pAvlTree;
typedef struct AvlTreeNode *pPosition;

pAvlTree MakeEmpty(pAvlTree root);
pPosition Find(ElementType X, pAvlTree root);
pPosition FindMax(pAvlTree root);
pPosition FindMin(pAvlTree root);
pAvlTree Insert(ElementType X, pAvlTree root);
pAvlTree Delete(ElementType X, pAvlTree root);
ElementType Retrieve(pPosition P);

#define type pAvlTree
// Define simple stack operation
// TODO attention is: stack's start index from 1
#define max 101
static type stack[max];
static int sp = 0;
#define pop stack[sp--]
#define push(x) stack[++sp]=(x)
#define empty (sp==0)
#define full  (sp==(max-1))
#define top (stack[sp])
#define initStack sp=0

#endif

#include <stdio.h>
#include <stdlib.h>

pPosition
FindMax(pAvlTree root)
{
    if(root != NULL)
     while(root->pright != NULL)
        root = root->pright;

    return root;
}

pPosition
FindMin(pAvlTree root)
{
    if(root != NULL)
     while(root->pleft != NULL)
        root = root->pleft;

    return root;
}

static inline int
Height(pPosition P)
{
    if(NULL == P)
        return -1;
    else
        return P->height;
}

static inline int
Max(int n1, int n2)
{
    return ((n1 > n2) ? n1 : n2);
}

// single right Rotate
static pPosition
singleRotateWithLeft(pPosition k2)
{
    pPosition k1;

    k1 = k2->pleft;
    k2->pleft = k1->pright;
    k1->pright = k2;

    k2->height = Max(Height(k2->pleft), Height(k2->pright)) + 1;
    k1->height = Max(Height(k1->pleft), k2->height) + 1;

    return k1;
}

// single left Rotate
static pPosition
singleRotateWithRight(pPosition k4)
{
    pPosition k3;

    k3 = k4->pright;
    k4->pright = k3->pleft;
    k3->pleft = k4;

    k4->height = Max(Height(k4->pleft), Height(k4->pright)) + 1;
    k3->height = Max(Height(k3->pright), k4->height) + 1;

    return k3;
}

// double right Rotate
static pPosition
doubleRotateWithLeft(pPosition k2)
{
    k2->pleft = singleRotateWithRight(k2->pleft);

    return singleRotateWithLeft(k2);
}

// double left Rotate
static pPosition
doubleRotateWithRight(pPosition k4)
{
    k4->pright = singleRotateWithLeft(k4->pright);

    return singleRotateWithRight(k4);
}

// insert a element into the avlBinTree
pAvlTree
Insert(ElementType X, pAvlTree T)
{
    if(NULL == T) {
        T = malloc(sizeof(struct AvlTreeNode));
        if(NULL == T) {
            printf("Out of space.\n");
            return NULL;
        } else {
            T->value = X;
            T->height = 0;
            T->pleft = T->pright = NULL;
        }
    } else if(X < T->value) {
        T->pleft = Insert(X, T->pleft);
        if(Height(T->pleft) - Height(T->pright) == 2) {
            if(X < T->pleft->value) {
                T = singleRotateWithLeft(T);
            } else {
                T = doubleRotateWithLeft(T);
            }
        }
    } else if(X > T->value) {
        T->pright = Insert(X, T->pright);
        if(Height(T->pright) - Height(T->pleft) == 2) {
            if(X > T->pright->value) {
                T = singleRotateWithRight(T);
            } else {
                T = doubleRotateWithRight(T);
            }
        }
    }

    T->height = Max(Height(T->pleft), Height(T->pright)) + 1;
    return T;
}

// empty the AvlBinTree
pAvlTree
MakeEmpty(pAvlTree root)
{
    if(root != NULL) {
        MakeEmpty(root->pleft);
        MakeEmpty(root->pright);
        free(root);
    }
    return NULL;
}

pPosition
Find(ElementType X, pAvlTree root)
{
    while(root != NULL) {
        if(X < root->value)
            root = root->pleft;
        else if(X > root->value)
            root = root->pright;
        else
            break;
    }

    return root;
}

// pre traversal the avlBinTree
int
preorderTraversal(pAvlTree root)
{
	return 0;
}

int
inOrderTraversal(pAvlTree root, int (*visit)(ElementType))
{
	pAvlTree tmp;
	initStack;
	push(root);

	while(!empty) {
		while(top) { 
			// TODO why will happen segmentation fault use next code
			//		may be the use of "static int sp"
			//push(((pAvlTree)top)->pleft);
			tmp = top;
			push((tmp)->pleft);
		}

		pop;
		if(!empty) {
			tmp = pop;
			if(visit(tmp->value))
				return -1;
			push(tmp->pright);
		}
	}

	return 0;
}

int
preOrderTraversal(pAvlTree root, int (*visit)(ElementType))
{
	pAvlTree tmp;
	initStack;
	push(root);

	while(!empty) {
		while(top) { 
			// TODO why will happen segmentation fault use next code
			//		may be the use of "static int sp"
			//push(((pAvlTree)top)->pleft);
			tmp = top;
			visit(tmp->value);
			push((tmp)->pleft);
		}

		pop;
		if(!empty) {
			tmp = pop;
			push(tmp->pright);
		}
	}

	return 0;
}

static inline int
printEle(ElementType data)
{
	printf("%d\t", data);
	return 0;
}

// inorder Traversal the avlBinTree
void
inorderTraversal(pAvlTree root)
{
    if(NULL == root)
        return;

    inorderTraversal(root->pleft);
    printf("%d\t", root->value);
    inorderTraversal(root->pright);
}

int
main(int argc, char **argv)
{
    int array[] = {21, 23, 7, 9, 24, 78};
    pPosition temp = NULL;
    int index = 0;
    int k = 6;
    // XXX: must initialize the root with NULL when declare 
    pAvlTree root = NULL;

    for(index = 0; index < k; ++index) {
        root = Insert(array[index], root);
    }

    //inorderTraversal(root);
	inOrderTraversal(root, printEle);
    printf("\n");
	preOrderTraversal(root, printEle);
    printf("\n");

    temp = Find(24, root);
    if(temp != NULL)
        printf("The result of Find is: %d\n", temp->value);
    else
        printf("Find no result in this AVLTree!\n");

    if(root)
        MakeEmpty(root);

    return 0;
}

