#include "sheet.h"
#include "celltree.h"

void
init_sheet(struct sheet *s, size_t width, size_t height, char *title)
{
    s->width = width;
    s->height = height;
    s->root_cell = NULL;

    if (title) {
        s->title = strndup(title, MAX_STR);
    } else {
        s->title = strdup("");
    }
}

void
delete_sheet(struct sheet *s)
{
    if (s == NULL) {
        warn("attempted to delete NULL sheet");
        return;
    }

    delete_cell_tree(s->root_cell);
    free(s->title);
    free(s);
}
