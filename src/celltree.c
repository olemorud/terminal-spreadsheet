#include "celltree.h"

void
delete_cell_tree(struct cell_tree_node *root)
{
    if (root == NULL) {
        return;
    }

    delete_cell(root->cell);
    delete_cell_tree(root->right);
    delete_cell_tree(root->left);

    free(root);
}

/* returns true if a 'greater than' b */
bool
greater_than(int x_a, int y_a, int x_b, int y_b)
{
    if (x_a == x_b) {
        return y_a > y_b;
    }

    return x_a > x_b;
}

struct cell*
search_tree(struct cell_tree_node *root, int x, int y) {
    if (root == NULL) {
        return NULL;
    }

    struct cell *c = root->cell;

    if (c->x_pos == x && c->y_pos == y) {
        return root->cell;
    }

    if(greater_than(x, y, c->x_pos, c->y_pos)) {
        return search_tree(root->right, x, y);
    } else {
        return search_tree(root->left, x, y);
    }
}

void
insert_node(struct cell_tree_node **root, struct cell *c)
{
    if (*root == NULL) {
        *root = malloc(sizeof(struct cell_tree_node));
        (*root)->cell = c;
        (*root)->right = NULL;
        (*root)->left = NULL;
        return;
    }

    if (greater_than(c->x_pos, c->y_pos, (*root)->cell->x_pos, (*root)->cell->y_pos)) {
        insert_node(&(*root)->right, c);
    } else {
        insert_node(&(*root)->left, c);
    }
}

