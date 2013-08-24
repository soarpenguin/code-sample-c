#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ElementType char

struct treeNode {
	ElementType data;
	struct treeNode *pleft;
	struct treeNode *pright;
};

typedef struct treeNode *pTreeNode;

pTreeNode
createTreeNode(ElementType X)
{
	pTreeNode tmp = NULL;

	tmp = malloc(sizeof(struct treeNode));
	if(NULL == tmp) {
		printf("Error in malloc %s-%d", __FILE__, __LINE__);
		return NULL;
	}
	
	tmp->data = X;
	//printf("%c\n", X);
	tmp->pleft = NULL;
	tmp->pright = NULL;

	return tmp;
}

void
destroyTree(pTreeNode root)
{
	if(!root)
		return;
	destroyTree(root->pleft);
	destroyTree(root->pright);
	
	free(root);
	root = NULL;
}

int
findChar(const ElementType *str, int s1, int s2, const ElementType c)
{
	int index = 0;

	if(!str || s1 > s2 || s1 < 0 || s2 >= strlen(str))
		return -1;

	for(index = s1; index <= s2; index++) {
		if(str[index] == c)
			return index;
	}

	return -1;
}

pTreeNode
getRoot(const ElementType *pre, int p1, int p2, const ElementType *in, int i1, int i2)
{
	ElementType rootCh;
	int index = 0;
	pTreeNode root = NULL;

	// XXX: use the wrong !i1
	//if(!pre || p1 < 0 || p1 > p2 || p2 >= strlen(pre) || !i1 || i1 > i2 || i1 < 0 || i2 >= strlen(in)) {
	if(!pre || !in || p1 < 0 || i1 < 0 || p1 > p2 || i1 > i2 || p2 > strlen(pre) || i2 >= strlen(in)) 
		return NULL;
	
	
	rootCh = pre[p1];
	
	index = findChar(in, i1, i2, rootCh);
	if(index < 0)
		return NULL;

	root = createTreeNode(rootCh);
	root->pleft = getRoot(pre, p1+1, p1+index-i1, in, i1, index-1);
	root->pright = getRoot(pre, p1+index-i1+1, p2, in, index+1, i2);
	
	return root;
}

pTreeNode
createTree(const ElementType *pre, int lenpre, const ElementType *in, int lenin)
{
	if(!pre || !in)
		return NULL;
	
	//printf("%d, %d\n", strlen(pre), strlen(in));
	return getRoot(pre, 0, lenpre-1, in, 0, lenin-1); 
}

void
printPostOrder(pTreeNode root)
{
	if(!root)
		return;
	printPostOrder(root->pleft);
	printPostOrder(root->pright);
	printf("%c  ", root->data);
}

int
countTreeNode(pTreeNode root)
{
	if(!root)
		return 0;

	return countTreeNode(root->pleft) + countTreeNode(root->pright) + 1;
}

int
main(int argc, char **argv)
{
	ElementType pre[] = "ABDGCEFH";
	ElementType in[] = "DGBAECHF";
	pTreeNode root = NULL;
	int count = 0;

	root = createTree(pre, strlen(pre), in, strlen(in));

	count = countTreeNode(root);
	if(count != strlen(pre))
		printf("NO such tree!\n");
	//else {
	//}
	printPostOrder(root);
	printf("\n");

	destroyTree(root);
	return 0;
}

