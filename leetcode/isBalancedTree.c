#include <stdio.h>
//#include <stdlib.h>
#define true 1
#define false 0

struct binaryTreeNode {
	int value;
	struct binaryTreeNode *pleft;
	struct binaryTreeNode *pright;
};

int
isBalanced2Paras(struct binaryTreeNode *pRoot, int *depth)
{
	int rightDepth, leftDepth;
	int diff;

	if(NULL == pRoot) {
		*depth = 0;
		return true;
	}

	if(isBalanced2Paras(pRoot -> pleft, &leftDepth) &&
		isBalanced2Paras(pRoot -> pright, &rightDepth)) {
		diff = leftDepth - rightDepth;
		if(diff >= -1 && diff <= 1) {
			*depth = 1 + (leftDepth > rightDepth ? leftDepth : rightDepth);
			return true;
		}
	}

	return false;
}

int
isBalaced(struct binaryTreeNode *pRoot)
{
	int depth = 0;

	isBalanced2Paras(pRoot, &depth);
}
//
//void
//addBinaryTreeNode(binaryTreeNode** pRoot, int value)
//{
//    if(NULL == pRoot) {
//        BinaryTreeNode *pBTree = new BinaryTreeNode();
//        pBTree -> m_pLeft = NULL;
//        pBTree -> m_pRight = NULL;
//        pBTree -> value = value;
//        pRoot = pBTree;
//    } 
//    else {
//        if(pRoot -> value > value) {
//            addBinaryTreeNode(pRoot -> m_pLeft, value);
//        } else if(pRoot -> value < value) {
//            addBinaryTreeNode(pRoot -> m_pRight, value);
//        } else {
//            printf("Add the same value in tree %d\n", value);
//        }
//    }
//}
//
//ptrTreeNode
//MakeEmpty(ptrTreeNode pRoot)
//{
//    if(pRoot != NULL) {
//        MakeEmpty(pRoot -> m_pLeft);
//        MakeEmpty(pRoot -> m_pRight);
//        free(pRoot);
//    }
//    return NULL;
//}
//
//int
//main(void)
//{
//    struct BinaryTreeNode *pRoot = NULL;
//    addBinaryTreeNode(&pRoot, 10);
//    addBinaryTreeNode(&pRoot, 4);
//    addBinaryTreeNode(&pRoot, 6);
//    addBinaryTreeNode(&pRoot, 21);
//    addBinaryTreeNode(&pRoot, 12);
//    addBinaryTreeNode(&pRoot, 3);
//    addBinaryTreeNode(&pRoot, 11);
//
//    if(IsBalanced(pRoot)) {
//        printf("The Tree is balanced!\n");
//    } else {
//        printf("The Tree is not balanced!\n");
//    }
//
//    MakeEmpty(pRoot);
//
//    return 0;
//}
//
int
main(int argc, char **argv)
{
	return 0;	
}

