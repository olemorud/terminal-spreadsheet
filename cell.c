#include "cell.h"

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