
#ifndef DRAW_H
#define DRAW_H


#include "book.h"
#include <stddef.h>
#include "cell.h"

void handle_resize();

void highlight(int const x, int const y);

void init_draw(struct book *book);

void draw_book(struct book *b, size_t tab);

void draw_cell(struct cell const * const cell);

void write_right_status(char const *const s);

int get_cell_color(int const row, int const col);


void write_left_command(const char *const s);

#endif