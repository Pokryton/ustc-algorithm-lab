#include "interval_tree.h"
#include <stdlib.h>

#define COLOR_RED   0
#define COLOR_BLACK 1

#define get_color(node) ((node)->parent_color & 1)
#define get_parent(node) (Interval_Node *) ((node)->parent_color & ~1)

#define set_color(node, color) set_parent_color(node, get_parent(node), color)
#define set_parent(node, parent) set_parent_color(node, parent, get_color(node))
#define set_parent_color(node, parent, color) (node)->parent_color = (uintptr_t) (parent) + (color)

inline static Interval_Node *interval_node_alloc(Interval interval, Interval_Node *parent)
{
    Interval_Node *node = malloc(sizeof(Interval_Node));

    node->interval = interval;
    node->max = interval.high;

    node->left = NULL;
    node->right = NULL;
    set_parent_color(node, parent, COLOR_RED);

    return node;
}

inline static int compute_max(Interval_Node *node)
{
    int max = node->interval.high;
    if (node->left && node->left->max > max)
        max = node->left->max;

    if (node->right && node->right->max > max)
        max = node->right->max;

    return max;
}

inline static void rotate_left(Interval_Tree *tree, Interval_Node *p)
{
    Interval_Node *q = p->right;
    Interval_Node *parent = get_parent(p);

    if (!parent) {
        tree->root = q;
    } else if (p == parent->left) {
        parent->left = q;
    } else {
        parent->right = q;
    }

    set_parent(q, parent);
    set_parent(p, q);
    p->right = q->left;
    if (p->right)
        set_parent(p->right, p);
    q->left = p;

    q->max = p->max;
    p->max = compute_max(p);
}

inline static void rotate_right(Interval_Tree *tree, Interval_Node *p)
{
    Interval_Node *q = p->left;
    Interval_Node *parent = get_parent(p);

    if (!parent) {
        tree->root = q;
    } else if (p == parent->left) {
        parent->left = q;
    } else {
        parent->right = q;
    }

    set_parent(q, parent);
    set_parent(p, q);
    p->left = q->right;
    if (p->left)
        set_parent(p->left, p);
    q->right = p;

    q->max = p->max;
    p->max = compute_max(p);
}

inline static void insert_fixup(Interval_Tree *tree, Interval_Node *node)
{
    for (;;) {
        Interval_Node *parent = get_parent(node);

        if (!parent) {
            set_color(node, COLOR_BLACK);
            break;
        }

        if (get_color(parent) == COLOR_BLACK)
            break;

        Interval_Node *gparent = get_parent(parent);
        if (parent == gparent->left) {
            Interval_Node *uncle = gparent->right;
            if (uncle && get_color(uncle) == COLOR_RED) {
                set_color(parent, COLOR_BLACK);
                set_color(uncle, COLOR_BLACK);
                set_color(gparent, COLOR_RED);
                node = gparent;
                continue;
            }

            if (node == parent->right) {
                rotate_left(tree, parent);
                parent = node;
            }
            set_color(parent, COLOR_BLACK);
            set_color(gparent, COLOR_RED);
            rotate_right(tree, gparent);
        } else {
            Interval_Node* uncle = parent->left;
            if (uncle && get_color(uncle) == COLOR_RED) {
                set_color(parent, COLOR_BLACK);
                set_color(uncle, COLOR_BLACK);
                set_color(gparent, COLOR_RED);
                node = gparent;
                continue;
            }

            if (node == parent->left) {
                rotate_right(tree, parent);
                parent = node;
            }
            set_color(parent, COLOR_BLACK);
            set_color(gparent, COLOR_RED);
            rotate_left(tree, gparent);
        }
        break;
    }
}

void interval_tree_insert(Interval_Tree *tree, Interval new)
{
    Interval_Node **indirect = &(tree->root), *parent = NULL;

    while (*indirect) {
        parent = *indirect;
        if (new.high > parent->max)
            parent->max = new.high;

        if (new.low < parent->interval.low) {
            indirect = &parent->left;
        } else {
            indirect = &parent->right;
        }
    }

    *indirect = interval_node_alloc(new, parent);
    insert_fixup(tree, *indirect);
}

static void free_nodes(Interval_Node *node)
{
    Interval_Node *left = node->left, *right = node->right;
    free(node);

    if (left)
        free_nodes(left);

    if (right)
        free_nodes(right);
}

void interval_tree_destroy(Interval_Tree *tree)
{
    if (tree->root) {
        free_nodes(tree->root);
        *tree = INTERVAL_TREE_EMPTY;
    }
}

static size_t foreach_overlaps(Interval_Node *node, Interval query, void (*visit)(Interval))
{
    size_t count = 0;

    Interval i = node->interval;
    if (interval_overlap(i, query)) {
        count += 1;
        if (visit)
            visit(i);
    }

    if (node->left && node->left->max >= query.low)
        count += foreach_overlaps(node->left, query, visit);

    if (node->right && i.low <= query.high && node->right->max >= query.low)
        count += foreach_overlaps(node->right, query, visit);

    return count;
}

size_t interval_tree_overlaps(Interval_Tree tree, Interval query, void (*visit)(Interval))
{
    if (!tree.root)
        return 0;

    return foreach_overlaps(tree.root, query, visit);
}
