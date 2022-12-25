#ifndef BOOK_H
#define BOOK_H

#include <stdlib.h>
#include "config.h"
#include "sheet.h"

struct book {
    struct sheet **sheets;
    char         *title;
};


void init_book(struct book *b, char *title);

void delete_book(struct book* b);

#endif