
#ifndef DRAW_H
#define DRAW_H


#include "book.h"
#include <stddef.h>
#include "cell.h"

void handle_resize();

void highlight(int const x, int const y, struct cell *c);

void init_draw(struct book *book);

void draw_book(struct book *b, size_t tab);

void write_right_status(char const *const s);

int get_cell_color(int const row, int const col);

void handle_resize(struct book* b, int tab);

void write_left_command(const char *const s);

#endif