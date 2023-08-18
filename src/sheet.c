#include "sheet.h"
#include "celltree.h"

void sheet_init(struct sheet *s, size_t width, size_t height, char *title)
{
    s->width     = width;
    s->height    = height;
    s->root_cell = NULL;

    if (title) {
        s->title = strndup(title, MAX_STR);
    } else {
        s->title = strdup("");
    }
}

void sheet_delete(struct sheet *s)
{
    if (s == NULL) {
        warn("attempted to delete NULL sheet");
        return;
    }

    celltree_delete(s->root_cell);
    free(s->title);
    free(s);
}
