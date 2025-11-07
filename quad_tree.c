#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define QUADTREE_MAX_BRANCHES 4

typedef struct QuadTree quad_tree_t;
typedef struct QuadTree {
    int data;
    quad_tree_t* branches[QUADTREE_MAX_BRANCHES];
} quad_tree_t;

quad_tree_t* QuadTreeCreate() {
    quad_tree_t* qt = (quad_tree_t*) malloc(sizeof(quad_tree_t));
    if (qt == NULL) {
        return (quad_tree_t*) NULL;
    }

    qt->data = 0;
    for (int i = 0; i < QUADTREE_MAX_BRANCHES; ++i) {
        qt->branches[i] = NULL;
    }

    return qt;
}

int QuadTreeInsertOn(quad_tree_t* qt, int index, int value) {
    assert(index >= 0 && index < QUADTREE_MAX_BRANCHES && "Index must be within [0;3]");

    if (qt == NULL) {
        return 0;
    }

    if (qt->branches[index] == NULL) {
        qt->branches[index] = (quad_tree_t*) malloc(sizeof(quad_tree_t));    
        qt->branches[index]->data = value;
    }

    return 1;
}

void QuadTreeDestroy(quad_tree_t* qt) {
    static int depth = 0;
    static int branchIndex = 0;

    printf("========================== QuadTreeDestroy called ==========================\n");
    printf("[[ DEPTH = %d on branch #%d ]]\n", depth, branchIndex);
    printf("DATA = %d\n", qt->data);

    for (int i = 0; i < QUADTREE_MAX_BRANCHES; ++i) {
        printf("BEFORE WHILE: qt->branches[%d] = 0x%X\n", i, qt->branches[i]);
    }

    if (qt->branches[0] != NULL ||
        qt->branches[1] != NULL ||
        qt->branches[2] != NULL ||
        qt->branches[3] != NULL
    ) {
        printf("WHILE WILL NOT BE VISITED (all %d branches are NULL)\n", QUADTREE_MAX_BRANCHES);
        printf("\n");
    }

    while (qt->branches[0] != NULL ||
           qt->branches[1] != NULL ||
           qt->branches[2] != NULL ||
           qt->branches[3] != NULL
    ) {
        printf("WHILE VISITED\n");

        for (int i = 0; i < QUADTREE_MAX_BRANCHES; ++i) {
            if (qt->branches[i] != NULL) {
                printf("Freeing 0x%X->branches[%d]\n\n", qt, i);
                ++depth;
                branchIndex = i;
                QuadTreeDestroy(qt->branches[i]);
                qt->branches[i] = NULL;
            }

            else {
                printf("0x%X->branches[%d] is NULL\n", qt, i);
            }

            printf("\n");
        }
    }

    printf("[[ DEPTH = %d on branch #%d ]]\n", depth, branchIndex);
    printf("Freeing 0x%X\n", qt);
    free(qt);
    qt = NULL;


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

    QuadTreeDestroy(root);

    return EXIT_SUCCESS;
}