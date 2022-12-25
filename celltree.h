#ifndef CELLTREE_H
#define CELLTREE_H

#include <stdlib.h>
#include "cell.h"

struct cell_tree_node {
    struct cell           *cell;
    struct cell_tree_node *right;
    struct cell_tree_node *left;
};

void delete_cell_tree(struct cell_tree_node *root);


#endif