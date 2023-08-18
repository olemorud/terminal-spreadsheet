
#ifndef DRAW_H
#define DRAW_H

#include "book.h"
#include "cell.h"
#include <stddef.h>

void draw_highlight(int const x, int const y);

void draw_init(struct book *book);

void draw_book(struct book *b, size_t tab);

void draw_cell(struct cell const *const cell);

void draw_left_status(const char *const s);

void draw_right_status(char const *const s);

#endif
