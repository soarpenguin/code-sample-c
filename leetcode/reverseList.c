#include <stdio.h>
#include <stdlib.h>
#define type int

struct listNode {
	type data;
	struct listNode *next;
};

typedef struct listNode listNode;

/////////////////////////////////////////////////
/*
 * main algorithm for reverse the list
 */
/////////////////////////////////////////////////
listNode *
reverseList(listNode *pHead)
{
	listNode *pNode;
	listNode *pPrev = NULL;
	listNode *pNext = NULL;
	listNode *pReverse = NULL;

	if(NULL == pHead)
		return NULL;
	
	pNode = pHead;
	while(pNode != NULL) {
		// get the next node, and store it at pNext
		pNext = pNode->next;

		// if pNext is null, get the end of the list.
		// store the reverse list head in pReverse for return
		if(NULL == pNext)
			pReverse = pNode;

		// reverse the linkage between nodes
		pNode->next = pPrev;

		// move forward on the list
		pPrev = pNode;
		pNode = pNext;
	}

	return pReverse;
}
/////////////////////////////////////////////////

// build a list for test
listNode *
insertList(listNode *head, type data)
{
	listNode *tmp = NULL;

	tmp = (listNode *)malloc(sizeof(listNode));
	if(NULL == tmp) {
		printf("memory out error!\n");
		return head;
	} else {
		tmp->next = NULL;
		tmp->data = data;
	}

	if(NULL == head) {
		head = tmp;
	} else {
		tmp->next = head;
		head = tmp;
	}

	return head;
}

// free the list
void
freeList(listNode *head)
{
	listNode *pcur;

	if(NULL == head)
		return;

	pcur = head->next;
	while(pcur != NULL){
		free(head);
		head = pcur;
		pcur = head->next;
	}
	free(head);
	head = NULL;
}

// display the list
void
printList(listNode *head)
{
	listNode *tmp;
	
	if(NULL == head) {
		printf("The list is NULL!\n");
		return;
	}

	tmp = head;
	while(tmp != NULL) {
		printf("%d  ", tmp->data);
		tmp = tmp->next;
	}
	printf("\n");
}

/*
 * main 
 */
int
main(int argc, char **argv)
{
	int index;
	listNode *head = NULL;

	head = reverseList(head);
	if(NULL == head)
		printf("The list is NULL!\n");

	for(index = 0; index < 10; index++) {
		head = insertList(head, index);
	}
	// display the orignal list
	printList(head);
	// reverse the list
	head = reverseList(head);
	// display the reversed list
	printList(head);
	// free the list
	freeList(head);
	return 0;
}

