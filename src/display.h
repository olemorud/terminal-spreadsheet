
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

void init_display(struct book *book);

void interact(struct book *b);

void move_down();

void move_left();

void move_right();

void move_up();

void next_tab();

void prev_tab();

void print_book(struct book *b, size_t tab);

void write_right_status(char const *const s);

int get_cell_color(int const row, int const col);

void start_edit_cell();

#endif