#include "cell.h"
#include <stdlib.h>
#include <err.h>
#include <string.h>

void
init_cell(struct cell *c, size_t x, size_t y, char *text, enum cell_type type)
{
    c->referenced_by = NULL;
    c->x_pos = x;
    c->y_pos = y;
    c->type = type;
    c->text = strdup("");
    c->value.integer = 0;
}

void
delete_cell(struct cell *c)
{
    if (c == NULL) {
        warn("attempted to delete NULL cell");
        return;
    }

    free(c->text);
    free(c);
}

void
set_type(struct cell *cell)
{
    char *endptr;
    int i = strtoll(cell->text, &endptr, 10);

    if (*endptr == '\0') {
        cell->type = Integer;
        cell->value.integer = i;
        return;
    }

    int d = strtold(cell->text, &endptr);

    if (*endptr == '\0') {
        cell->type = Floating;
        cell->value.floating = d;
        return;
    }
}