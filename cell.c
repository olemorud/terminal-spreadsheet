#include "cell.h"


void
init_cell(struct cell *c, size_t x_pos, size_t y_pos, char* text, enum cell_type type)
{
    c->referenced_by = NULL;
    c->x_pos = x_pos;
    c->y_pos = y_pos;
    c->type  = type;
    c->text  = strdup("");
    c->value.integer = 0;
}


void
delete_cell(struct cell *c)
{
    if(c == NULL){
        warn("attempted to delete NULL cell");
        return;
    }

    free(c->text);
    free(c);
}