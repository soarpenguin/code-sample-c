#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
typedef struct ListNode {
    int val;
    ListNode *next;
} ListNode;

ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
    // Start typing your C/C++ solution below
    // DO NOT write int main() function
    ListNode *h1 = l1;
    ListNode *h2 = l2;
    ListNode *h1pre = l1;
    ListNode *h2pre = l2;

    int forward = 0;
    int sum = 0;

    if (NULL == l1 && NULL == l2) {
        return l1;
    } else if (NULL == l1) {
        return l2;
    } else if (NULL == l2) {
        return l1;
    }
    
    while (h1 != NULL && h2 != NULL) {
        sum = h1->val + h2->val;

        if (forward > 0) {
            sum += forward;
            forward = 0;
        }

        if (sum >= 10) {
            forward = sum / 10;
            sum %= 10;
        }

        h1->val = sum;
        h1pre = h1;
        h1 = h1->next;
        h2pre = h2;
        h2 = h2->next;
    }

    if (NULL == h1 && NULL == h2) {
        if (forward > 0) {
            h1pre->next = (ListNode *) malloc(sizeof(ListNode));
            h1pre->next->val = forward;
            h1pre->next->next = NULL; 
        }
        goto done;
    } else if (NULL == h1) {
        h1pre->next = h2;
        h2pre->next = NULL;
    } 

    h1pre = h1pre->next;

    while (forward > 0) {
        sum = h1pre->val + forward;
        forward = 0;

        if (sum >= 10) {
            forward = sum / 10;
            sum %= 10;
        }
        h1pre->val = sum;

        if (NULL == h1pre->next) {
            if (forward > 0) {
                h1pre->next = (ListNode *) malloc(sizeof(ListNode));
                h1pre->next->val = forward;
                h1pre->next->next = NULL;
            }
            break;
        } else {
            h1pre = h1pre->next;
        }
    }
done:
    return l1;
}

int
main(int argc, char **argv) {
    return 0;
}
