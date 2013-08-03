#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// pass the test of: 
// http://leetcode.com/onlinejudge  

typedef struct ListNode {
    char   content;
    struct ListNode *next;
} ListNode;

ListNode *
removeNthFromEnd(ListNode *head, int n) {
    ListNode *fast = head;
    ListNode *slow = head;

    if (NULL == head || n <= 0) {
        return head;
    }

    while (fast != NULL && n > 0) {
        fast = fast->next;
        n--;
    }

    if (n > 0) {
        return head;
    }

    while (NULL != fast && NULL != fast->next) {
        fast = fast->next;
        slow = slow->next;
    }

    // XXX: just fast eq NULL and head eq slow then remove head.
    //      else remove the slow->next
    if (head == slow && NULL == fast) {
        head = head->next;
        free(slow);
    } else {
        fast = slow->next;
        slow->next = slow->next->next;
        free(fast);
    }

    return head;
}

int
main(int argc, char **argv) {
    return 0;
}
