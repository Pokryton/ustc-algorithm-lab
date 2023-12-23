#include <stdio.h>
#include <stdlib.h>

#include "interval_tree.h"

void interval_print(Interval i)
{
    printf("[%d, %d]\n", i.low, i.high);
}

int main(void)
{
    FILE *fp = fopen("insert.txt", "r");
    if (fp == NULL) {
        perror("Failed to load data from insert.txt");
        exit(EXIT_FAILURE);
    }

    size_t n;
    fscanf(fp, "%zu", &n);

    Interval_Tree tree = INTERVAL_TREE_EMPTY;
    Interval i;
    for (size_t k = 0; k < n; ++k) {
        fscanf(fp, "%d%d", &i.low, &i.high);
        interval_tree_insert(&tree, i);
    }

    fclose(fp);

    for (;;) {
        printf("Query: ");
        fflush(stdout);

        if (scanf("%d%d", &i.low, &i.high) != 2) {
            printf("quit\n");
            break;
        }

        if (interval_tree_overlaps(tree, i, interval_print) == 0) {
            printf("nil\n");
        }
    }

    interval_tree_destroy(&tree);
    return 0;
}
