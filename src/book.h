#ifndef BOOK_H
#define BOOK_H

#include "sheet.h"
#include <stddef.h>

struct book {
  struct sheet **sheets;
  size_t         n_sheets;
  char          *title;
};

void init_book(struct book *b, char *title);

void delete_book(struct book *b);

void add_sheet(struct book *b);

#endif