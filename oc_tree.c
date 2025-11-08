#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define OCTREE_MAX_BRANCHES 8

#define TOP_LEFT_BACK_PART      0
#define TOP_LEFT_FRONT_PART     1
#define TOP_RIGHT_FRONT_PART    2
#define TOP_RIGHT_BACK_PART     3
#define BOTTOM_LEFT_BACK_PART   4
#define BOTTOM_LEFT_FRONT_PART  5
#define BOTTOM_RIGHT_FRONT_PART 6
#define BOTTOM_RIGHT_BACK_PART  7

typedef struct OcTree octree_t;
typedef struct OcTree {
    int data;
    octree_t* branches[OCTREE_MAX_BRANCHES];
} octree_t;

octree_t* OcTreeCreate() {
    return (octree_t*) calloc(1, sizeof(octree_t));
}

int OcTreeInsertOn(octree_t* ot, int index, int value) {
    assert(index >= 0 && index < OCTREE_MAX_BRANCHES && "Index must be within [0;7]");

    if (ot == NULL) {
        return 0;
    }

    if (ot->branches[index] == NULL) {
        ot->branches[index] = OcTreeCreate();    
        ot->branches[index]->data = value;
    }

    return 1;
}

void OcTreeDestroy(octree_t** pOt) {
    static int depth = 0;
    static int branchIndex = 0;

    octree_t* ot = *pOt;

    printf("[[ DEPTH = %d on branch #%d ]]\n", depth, branchIndex);
    printf("DATA = %d\n", ot->data);

    for (int i = 0; i < OCTREE_MAX_BRANCHES; ++i) {
        if (ot->branches[i] != NULL) {
            printf("Freeing 0x%p->branches[%d]\n\n", ot, i);
            ++depth;
            branchIndex = i;
            OcTreeDestroy(&(ot->branches[i]));
            ot->branches[i] = NULL;
        }
    }

    printf("Freeing 0x%p\n", pOt);
    free(*pOt);
    *pOt = NULL; // setting to NULL to avoid segfault when destroying part of a tree recursively

    --depth;
}

int main() {
    octree_t* root = OcTreeCreate();

    if (root == NULL) {
        fprintf(stderr, "Unable to create an octree tree");
    }
    
    {
        for (int i = 0; i < OCTREE_MAX_BRANCHES; ++i) {
            OcTreeInsertOn(root, i, i + 1);
        }

        for (int i = 0; i < OCTREE_MAX_BRANCHES; ++i) {
            for (int j = 0; j < OCTREE_MAX_BRANCHES; ++j) {
                OcTreeInsertOn(root->branches[i], j, 8 * i + j);
            }
        }

        OcTreeDestroy(&root->branches[3]);
        printf("AFTER FREE: %p\n", root->branches[3]);
    }

    OcTreeDestroy(&root);

    return EXIT_SUCCESS;
}