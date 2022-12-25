#include "celltree.h"
#include <stdlib.h>
#include <errno.h>
#include <err.h>

/* returns true if (ax,ay) `greater than` (bx,by) */
static bool greater_than(int x_a, int y_a, int x_b, int y_b)
{
    if (x_a == x_b)
        return y_a > y_b;

    return x_a > x_b;
}

void celltree_delete(struct celltree *root)
{
    if (root == NULL) {
        return;
    }

    cell_delete(root->cell);
    celltree_delete(root->right);
    celltree_delete(root->left);

    free(root);
}

struct cell* celltree_search(struct celltree *root, size_t x, size_t y)
{
    if (root == NULL) {
        return NULL;
    }

    struct cell *c = root->cell;

    if (c->x_pos == x && c->y_pos == y) {
        return root->cell;
    }

    if (greater_than(x, y, c->x_pos, c->y_pos)) {
        return celltree_search(root->right, x, y);
    } else {
        return celltree_search(root->left, x, y);
    }
}

void celltree_insert(struct celltree **root, struct cell *c)
{
    if (*root == NULL) {
        *root = malloc(sizeof **root);
        if (*root == NULL)
            exit(errno);
        (*root)->cell = c;
        (*root)->right = NULL;
        (*root)->left = NULL;
        return;
    }

    if (greater_than(c->x_pos, c->y_pos, (*root)->cell->x_pos,
                     (*root)->cell->y_pos)) {
        celltree_insert(&(*root)->right, c);
    } else {
        celltree_insert(&(*root)->left, c);
    }
}
