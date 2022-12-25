#include "celltree.h"

void
delete_cell_tree(struct cell_tree_node *root)
{
    if ( root==NULL ) {
        return;
    }

    delete_cell(root->cell);
    delete_cell_tree(root->right);
    delete_cell_tree(root->left);
    
    free(root);
}