#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define QUADTREE_MAX_BRANCHES 4

#define TOP_LEFT_PART     0
#define BOTTOM_LEFT_PART  1
#define BOTTOM_RIGHT_PART 2
#define TOP_RIGHT_PART    3

typedef struct QuadTree quad_tree_t;
typedef struct QuadTree {
    int data;
    quad_tree_t* branches[QUADTREE_MAX_BRANCHES];
} quad_tree_t;

quad_tree_t* QuadTreeCreate() {
    return (quad_tree_t*) calloc(1, sizeof(quad_tree_t));
}

int QuadTreeInsertOn(quad_tree_t* qt, int index, int value) {
    assert(index >= 0 && index < QUADTREE_MAX_BRANCHES && "Index must be within [0;3]");

    if (qt == NULL) {
        return 0;
    }

    if (qt->branches[index] == NULL) {
        qt->branches[index] = QuadTreeCreate();    
        qt->branches[index]->data = value;
    }

    return 1;
}

void QuadTreeDestroy(quad_tree_t** pQt) {
    static int depth = 0;
    static int branchIndex = 0;

    if (pQt == NULL) {
        printf("pQt is NULL. Aborting.");
        return;
    }

    quad_tree_t* qt = *pQt;

    printf("[[ DEPTH = %d on branch #%d ]]\n", depth, branchIndex);
    printf("DATA = %d\n", qt->data);

    for (int i = 0; i < QUADTREE_MAX_BRANCHES; ++i) {
        if (qt->branches[i] != NULL) {
            printf("Freeing 0x%X->branches[%d]\n\n", qt, i);
            ++depth;
            branchIndex = i;
            QuadTreeDestroy(&(qt->branches[i]));
            qt->branches[i] = NULL;
        }

        printf("\n");
    }

    printf("Freeing 0x%X\n", qt);
    free(*pQt);
    *pQt = NULL;

    --depth;
}

int main() {
    quad_tree_t* root = QuadTreeCreate();

    if (root == NULL) {
        fprintf(stderr, "Unable to create a quad tree");
    }
    
    /*
        Create the following tree

        ---------------
               0
            __/\___     0        
        ---/-|---|-\---
          1  2   3  4   1
        -/-\-----------   
        4   5           2
        ---/-----------
          6             3
        ---------------
    */
    {
        QuadTreeInsertOn(root, 0, 1);
        QuadTreeInsertOn(root, 1, 2);
        QuadTreeInsertOn(root, 2, 3);
        QuadTreeInsertOn(root, 3, 4);

        QuadTreeInsertOn(root->branches[1], 2, 5);

        QuadTreeInsertOn(root->branches[1]->branches[0], 1, 6);
        QuadTreeInsertOn(root->branches[1]->branches[2], 1, 7);

        QuadTreeInsertOn(root->branches[1]->branches[2]->branches[0], 0, 8);
    }

    QuadTreeDestroy(&root);

    return EXIT_SUCCESS;
}