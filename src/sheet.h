#ifndef SHEET_H
#define SHEET_H

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include "config.h"

struct sheet {
    struct celltree *root_cell;
    size_t           width;
    size_t           height;
    char*            title;
};

void sheet_init(struct sheet *s, size_t width, size_t height, char* title);
void sheet_delete(struct sheet *s);

#endif