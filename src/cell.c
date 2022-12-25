#include "cell.h"
#include <stdlib.h>
#include <err.h>
#include <string.h>

void cell_parse_function(struct cell *cell);

void cell_init(struct cell *c, size_t x, size_t y, enum cell_type type)
{
    c->referenced_by = NULL;
    c->x_pos = x;
    c->y_pos = y;
    c->type = type;
    c->text = calloc(1,1);
    c->value.integer = 0;
}

void cell_delete(struct cell *c)
{
    if (c == NULL) {
        warn("attempted to delete NULL cell");
        return;
    }

    free(c->text);
    free(c);
}

void cell_set_type(struct cell *cell)
{
    if (cell->text[0] == '=') {
        //cell_parse_function(cell);
        return;
    }

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
