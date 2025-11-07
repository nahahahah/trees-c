#include <stdio.h>
#include <stdlib.h>

typedef struct BinaryTree binary_tree_t;
typedef struct BinaryTree {
    int data;
    binary_tree_t* left;
    binary_tree_t* right;
} binary_tree_t;

binary_tree_t* BinaryTreeCreate() {
    return (binary_tree_t*) calloc(1, sizeof(binary_tree_t));
}

int BinaryTreeInsertLeftOn(binary_tree_t* bt, int value) {
    if (bt == NULL) {
        return 0;
    }

    bt->left = BinaryTreeCreate();
    bt->left->data = value;

    return 1;
}

int BinaryTreeInsertRightOn(binary_tree_t* bt, int value) {
    if (bt == NULL) {
        return 0;
    }

    bt->right = BinaryTreeCreate();    
    bt->right->data = value;

    return 1;
}

void BinaryTreeDestroy(binary_tree_t* bt) {
    static int depth = 0;
    static int onLeftBranch = 0;

    if (bt == NULL) {
        return;
    }

    printf("[[ DEPTH = %d on %s branch ]]\n", depth, (onLeftBranch ? "left" : "right"));
    printf("DATA = %d\n", bt->data);

    if (bt->left != NULL) {
        printf("Freeing 0x%p->left\n\n", bt);
        ++depth;
        onLeftBranch = 1;
        BinaryTreeDestroy(bt->left);
        bt->left = NULL;
    }

    if (bt->right != NULL) {
        printf("Freeing 0x%p->right\n\n", bt);
        ++depth;
        onLeftBranch = 0;
        BinaryTreeDestroy(bt->right);
        bt->right = NULL;
    }


    printf("Freeing 0x%p\n", bt);
    free(bt);

    --depth;
}

int main() {
    binary_tree_t* root = BinaryTreeCreate();

    if (root == NULL) {
        fprintf(stderr, "Unable to create a binary tree");
    }
    
    /*
        Creates the following tree

        -------------
              0       0
        -----/-\-----
            1   2     1
        ---/-\-------
          3   4       2
        -/-\---\-----
        5   6   7     3
        -----\-------
              8       4
        -------------
    */
    {
        // depth 1
        BinaryTreeInsertLeftOn(root, 1);
        BinaryTreeInsertRightOn(root, 2);

        // depth 2
        BinaryTreeInsertLeftOn(root->left, 3);
        BinaryTreeInsertRightOn(root->left, 4);

        // depth 3    
        BinaryTreeInsertLeftOn(root->left->left, 5);
        BinaryTreeInsertRightOn(root->left->left, 6);
        BinaryTreeInsertRightOn(root->left->right, 7);

        // depth 4
        BinaryTreeInsertRightOn(root->left->left->right, 8);
    }

    BinaryTreeDestroy(root);

    return EXIT_SUCCESS;
}