#ifndef CELLTREE_H
#define CELLTREE_H

#include <stdbool.h>
#include "cell.h"

/* TODO: implement red-black tree */
struct celltree {
    struct cell     *cell;
    struct celltree *right;
    struct celltree *left;
    bool             isRed;
};

void celltree_delete(struct celltree *root);

struct cell *celltree_search(struct celltree *root, size_t x, size_t y);

void celltree_insert(struct celltree **root, struct cell *c);

#endif