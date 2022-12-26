
#ifndef DISPLAY_H
#define DISPLAY_H

#include "sheet.h"
#include "book.h"
#include "cell.h"
#include <stdio.h>

#define COLOR_TITLE 20
#define COLOR_BACKGROUND 21
#define COLOR_LIGHTER_GRAY 22
#define COLOR_LIGHT_GRAY 23
#define COLOR_GRAY 24
#define COLOR_HIGHLIGHTED 25
#define COLOR_EDITMODE 26

void handle_resize();

void highlight(int const x, int const y, struct cell *c);

void init_draw(struct book *book);

void draw_book(struct book *b, size_t tab);

void write_right_status(char const *const s);

int get_cell_color(int const row, int const col);

void handle_resize(struct book* b, int tab);

void write_left_command(const char *const s);

#endif