#ifndef BOOK_H
#define BOOK_H

#include "sheet.h"
#include <stdbool.h>
#include <stddef.h>

struct book {
    struct sheet **sheets;
    size_t         n_sheets;
    char          *title;
};

void book_init(struct book *b, char *title);

void book_delete(struct book *b);

bool book_add_sheet(struct book *b);

#endif
