#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

bool
isSameTree(struct TreeNode *p, struct TreeNode *q) {
    // Start typing your C/C++ solution below
    // DO NOT write int main() function
    bool lsame = false;
    bool rsame = false;

    if (NULL == p && NULL == q) {
        return true;
    } else if (NULL == p || NULL == q) {
        return false;
    }
    
    if (p->val != q->val) {
        return false;
    }

    lsame = isSameTree(p->left, q->left);
    rsame = isSameTree(p->right, q->right);

    if (lsame && rsame) {
        return true;
    } else {
        return false;
    }

}

int
main(int argc, char **argv) {
    return 0;
}
