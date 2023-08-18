#include "book.h"
#include "config.h"
#include <err.h>
#include <stdlib.h>
#include <string.h>

void book_init(struct book *b, char *title)
{
    b->n_sheets = 0;
    b->sheets   = malloc(0);
    if (!book_add_sheet(b))
        exit(1);

    (b->sheets)[1] = NULL;

    if (title != NULL)
        b->title = strndup(title, MAX_STR);
    else
        b->title = strdup("");
}

void book_delete(struct book *b)
{
    if (b == NULL) {
        warn("attempted to delete NULL book");
        return;
    }

    size_t i = 0;
    while ((b->sheets)[i] != NULL) {
        sheet_delete((b->sheets)[i]);
        i++;
    }
}

bool book_add_sheet(struct book *b)
{
    struct sheet *s;

    b->n_sheets++;

    void *tmp = realloc(b->sheets, sizeof(struct sheet *) * (b->n_sheets + 1));
    if (tmp == NULL)
        return false;
    b->sheets = tmp;

    b->sheets[b->n_sheets] = NULL;

    s = malloc(sizeof(struct sheet));
    sheet_init(s, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_SHEET_TITLE);

    b->sheets[b->n_sheets - 1] = s;

    return true;
}
