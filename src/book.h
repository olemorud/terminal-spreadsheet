#ifndef BOOK_H
#define BOOK_H

#include "config.h"
#include "sheet.h"
#include <err.h>
#include <stdlib.h>

struct book {
  struct sheet **sheets;
  char *title;
};

void init_book(struct book *b, char *title);

void delete_book(struct book *b);

#endif