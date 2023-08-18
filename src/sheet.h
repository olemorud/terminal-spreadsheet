#ifndef SHEET_H
#define SHEET_H

#include "config.h"
#include <err.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct sheet {
    struct celltree *root_cell;
    size_t           width;
    size_t           height;
    char            *title;
};

void sheet_init(struct sheet *s, size_t width, size_t height, char *title);
void sheet_delete(struct sheet *s);

#endif