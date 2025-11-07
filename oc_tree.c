#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define OCTREE_MAX_BRANCHES 8

typedef struct OcTree octree_t;
typedef struct OcTree {
    int data;
    octree_t* branches[OCTREE_MAX_BRANCHES];
} octree_t;

octree_t* OcTreeCreate() {
    octree_t* ot = (octree_t*) malloc(sizeof(octree_t));
    if (ot == NULL) {
        return (octree_t*) NULL;
    }

    ot->data = 0;
    for (int i = 0; i < 4; ++i) {
        ot->branches[i] = NULL;
    }

    return ot;
}

int OcTreeInsertOn(octree_t* ot, int index, int value) {
    assert(index >= 0 && index < OCTREE_MAX_BRANCHES && "Index must be within [0;8]");

    if (ot == NULL) {
        return 0;
    }

    if (ot->branches[index] == NULL) {
        ot->branches[index] = (octree_t*) malloc(sizeof(octree_t));    
        ot->branches[index]->data = value;
    }

    return 1;
}

void OcTreeDestroy(octree_t* ot) {
    static int depth = 0;
    static int branchIndex = 0;

    printf("========================== OcTreeDestroy called ==========================\n");
    printf("[[ DEPTH = %d on branch #%d ]]\n", depth, branchIndex);
    printf("DATA = %d\n", ot->data);
    for (int i = 0; i < OCTREE_MAX_BRANCHES; ++i) {
        printf("BEFORE WHILE: ot->branches[%d] = 0x%X\n", i, ot->branches[i]);
    }

    if (
        ot->branches[0] != NULL ||
        ot->branches[1] != NULL ||
        ot->branches[2] != NULL ||
        ot->branches[3] != NULL ||
        ot->branches[4] != NULL ||
        ot->branches[5] != NULL ||
        ot->branches[6] != NULL ||
        ot->branches[7] != NULL
    ) {
        printf("WHILE WILL NOT BE VISITED (all 8 branches are NULL)\n");
        printf("\n");
    }

    while (
        ot->branches[0] != NULL ||
        ot->branches[1] != NULL ||
        ot->branches[2] != NULL ||
        ot->branches[3] != NULL ||
        ot->branches[4] != NULL ||
        ot->branches[5] != NULL ||
        ot->branches[6] != NULL ||
        ot->branches[7] != NULL
    ) {
        printf("WHILE VISITED\n");

        for (int i = 0; i < OCTREE_MAX_BRANCHES; ++i) {
            if (ot->branches[i] != NULL) {
                printf("Freeing 0x%X->branches[%d]\n\n", ot, i);
                ++depth;
                branchIndex = i;
                OcTreeDestroy(ot->branches[i]);
                ot->branches[i] = NULL;
            }

            else {
                printf("0x%X->branches[%d] is NULL\n", ot, i);
            }

            printf("\n");
        }
    }

    printf("[[ DEPTH = %d on branch #%d ]]\n", depth, branchIndex);
    printf("Freeing 0x%X\n", ot);
    free(ot);
    ot = NULL;


    --depth;
}

int main() {
    octree_t* root = OcTreeCreate();

    if (root == NULL) {
        fprintf(stderr, "Unable to create an octree tree");
    }
    
    {
        OcTreeInsertOn(root, 0, 1);
        OcTreeInsertOn(root, 1, 2);
        OcTreeInsertOn(root, 2, 3);
        OcTreeInsertOn(root, 7, 4);

        OcTreeInsertOn(root->branches[1], 2, 5);

        OcTreeInsertOn(root->branches[1]->branches[0], 1, 6);
        OcTreeInsertOn(root->branches[1]->branches[2], 1, 7);

        OcTreeInsertOn(root->branches[1]->branches[2]->branches[0], 0, 8);
    }

    OcTreeDestroy(root);

    return EXIT_SUCCESS;
}