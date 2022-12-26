
#ifndef DISPLAY_H
#define DISPLAY_H

#include "sheet.h"
#include "book.h"
#include "cell.h"
#include <stdio.h>
#include <stdio.h>

#define CELL_SIZE 12
#define TAB_SIZE 24
#define Y_AXIS_WIDTH 4
#define N_CELLS_WIDTH 5
#define N_CELLS_HEIGHT 5


#define COLOR_TITLE 20
#define COLOR_BACKGROUND 21
#define COLOR_LIGHTER_GRAY 22
#define COLOR_LIGHT_GRAY 23
#define COLOR_GRAY 24
#define COLOR_COMMANDMODE 25
#define COLOR_EDITMODE 26



enum modes {edit, command};


//void print_cell(int n, char *content);
//void print_sheet(struct sheet *s);

void print_book(struct book *b, size_t tab);
void init_display();
void interact(struct book *b, int tab);

#endif