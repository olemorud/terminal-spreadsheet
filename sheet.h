#ifndef SHEET_H
#define SHEET_H

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"

struct sheet {
    struct cell_tree_node *root_cell;
    size_t          width;
    size_t          height;
    char*           title;
};

void init_sheet(struct sheet *s, size_t width, size_t height, char* title);
void delete_sheet(struct sheet *s);

#endif