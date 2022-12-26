#include "book.h"
#include "config.h"
#include "sheet.h"

void
init_book(struct book *b, char *title)
{
    b->n_sheets = 0;
    b->sheets = malloc(sizeof(struct sheet*));
    add_sheet(b);

    (b->sheets)[1] = NULL;

    if (title != NULL) {
        b->title = strndup(title, MAX_STR);
    } else {
        b->title = strdup("");
    }
}

void
delete_book(struct book *b)
{
    if (b == NULL) {
        warn("attempted to delete NULL book");
        return;
    }

    size_t i = 0;
    while ((b->sheets)[i] != NULL) {
        delete_sheet((b->sheets)[i]);
        i++;
    }
}

void
add_sheet(struct book *b)
{
    struct sheet *s;

    b->n_sheets++;

    b->sheets = realloc(b->sheets, sizeof(struct sheet*) * (b->n_sheets + 1));
    b->sheets[b->n_sheets] = NULL;

    s = malloc(sizeof(struct sheet));
    init_sheet(s, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_SHEET_TITLE);

    b->sheets[b->n_sheets-1] = s;
}