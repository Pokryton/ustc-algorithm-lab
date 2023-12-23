#ifndef _INTERVAL_TREE_H_
#define _INTERVAL_TREE_H_

#include <stdint.h>
#include <stddef.h>

typedef struct interval {
    int high;
    int low;
} Interval;

#define INTERVAL(low, high) (Interval) { .low = (low), .high = (high) }
#define interval_overlap(i1, i2) ((i1).low <= (i2).high && (i1).high >= (i2).low)

typedef struct interval_node {
    Interval interval;
    int max;
    uintptr_t parent_color;
    struct interval_node *left;
    struct interval_node *right;
} Interval_Node;

typedef struct interval_tree {
   Interval_Node *root;
} Interval_Tree;

#define INTERVAL_TREE_EMPTY (Interval_Tree) { .root = NULL }

void interval_tree_insert(Interval_Tree *tree, Interval interval);
void interval_tree_destroy(Interval_Tree *tree);
size_t interval_tree_overlaps(Interval_Tree tree, Interval query, void (*visit)(Interval));

#endif // _INTERVAL_TREE_H_
