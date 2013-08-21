#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

int
goDownAndSum(TreeNode *root, int sum) {
    int left = 0;
    int right = 0;
    
    if (NULL == root) {
        return 0;
    }

    if (NULL == root->left && NULL == root->right) {
        return sum;
    }

    if (root->left != NULL) {
        left = goDownAndSum(root->left, 10 * sum + root->left->val);
    }
    
    if (root->right != NULL) {
        right = goDownAndSum(root->right, 10 * sum + root->right->val);
    }

    return left + right;
}

int
sumNumbers(TreeNode *root) {
    if (NULL == root) {
        return 0;
    }

    return goDownAndSum(root, root->val);
}

int
main(int argc, char **argv) {
    return 0;
}
