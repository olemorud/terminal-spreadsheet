#ifndef CELLTREE_H
#define CELLTREE_H

#include <stdlib.h>
#include <stdbool.h>
#include "cell.h"

/* TODO: implement red-black tree */
struct cell_tree_node {
    struct cell           *cell;
    struct cell_tree_node *right;
    struct cell_tree_node *left;
    bool                   isRed;
};

void delete_cell_tree(struct cell_tree_node *root);

struct cell *search_tree(struct cell_tree_node *root, int x, int y);

void insert_node(struct cell_tree_node **root, struct cell *c);

#endif