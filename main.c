#include <stdio.h>
#include <stdlib.h>

typedef struct BinaryTree binary_tree_t;
typedef struct BinaryTree {
    int data;
    binary_tree_t* left;
    binary_tree_t* right;
} binary_tree_t;

binary_tree_t* BinaryTreeCreate() {
    binary_tree_t* bt = (binary_tree_t*) malloc(sizeof(binary_tree_t));
    if (bt == NULL) {
        return (binary_tree_t*) NULL;
    }

    bt->data = 0;
    bt->left = NULL;
    bt->right = NULL;

    return bt;
}

int BinaryTreeInsertLeftOn(binary_tree_t* bt, int value) {
    if (bt == NULL) {
        return 0;
    }

    if (bt->left == NULL) {
        bt->left = (binary_tree_t*) malloc(sizeof(binary_tree_t));    
        bt->left->data = value;
    }

    return 1;
}

int BinaryTreeInsertRightOn(binary_tree_t* bt, int value) {
    if (bt == NULL) {
        return 0;
    }

    if (bt->right == NULL) {
        bt->right = (binary_tree_t*) malloc(sizeof(binary_tree_t));    
        bt->right->data = value;
    }

    return 1;
}

void BinaryTreeDestroy(binary_tree_t* bt) {
    static int depth = 0;
    static bool onLeftBranch = false;

    printf("========================== BinaryTreeDestroy called ==========================\n");
    printf("[[ DEPTH = %d on %s branch ]]\n", depth, (onLeftBranch ? "left" : "right"));
    printf("DATA = %d\n", bt->data);
    printf("BEFORE WHILE: bt->left = 0x%X\n", bt->left);
    printf("BEFORE WHILE: bt->right = 0x%X\n", bt->right);

    if (bt->left == NULL && bt->right == NULL) {
        printf("WHILE WILL NOT BE VISITED (left and right are NULL)\n");
        printf("\n");
    }

    while (bt->left != NULL || bt->right != NULL) {
        printf("WHILE VISITED\n");
        if (bt->left != NULL) {
            printf("Freeing 0x%X->left\n\n", bt);
            ++depth;
            onLeftBranch = true;
            BinaryTreeDestroy(bt->left);
            bt->left = NULL;
        }

        else {
            printf("0x%X->left is NULL\n", bt);
        }

        if (bt->right != NULL) {
            printf("Freeing 0x%X->right\n\n", bt);
            ++depth;
            onLeftBranch = false;
            BinaryTreeDestroy(bt->right);
            bt->right = NULL;
        }

        else {    
            printf("0x%X->right is NULL\n", bt);
        }

        printf("\n");
    }

    printf("[[ DEPTH = %d on %s branch ]]\n", depth, (onLeftBranch ? "left" : "right"));
    printf("Freeing 0x%X\n", bt);
    free(bt);
    bt = NULL;


    --depth;
}

int main() {
    binary_tree_t* root = BinaryTreeCreate();

    if (root == NULL) {
        fprintf(stderr, "Unable to create a binary tree");
    }
    
    /*
        Creates the following tree

              0
             / \
            1   2
           / \
          3   4
         / \   \
        5   6   7
             \
              8
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

    printf("bt->left->data = %d\n", root->left->data);

    root->left->data = 3;
    printf("bt->left->data = %d\n", root->left->data);
    
    printf("\n");

    BinaryTreeDestroy(root);

    return EXIT_SUCCESS;
}