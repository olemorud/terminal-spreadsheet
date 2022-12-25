
#include "display.h"
#include <curses.h>

#define _STR(x) #x
#define STR(x) _STR(x)

void
display_init()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);

    curs_set(0);

    start_color();
    init_color(COLOR_LIGHTER_GRAY, 900, 900, 900);
    init_pair(COLOR_LIGHTER_GRAY, COLOR_BLACK, COLOR_LIGHTER_GRAY);

    init_color(COLOR_LIGHT_GRAY, 800, 800, 800);
    init_pair(COLOR_LIGHT_GRAY, COLOR_BLACK, COLOR_LIGHT_GRAY);

    init_color(COLOR_GRAY, 650, 650, 650);
    init_pair(COLOR_GRAY, COLOR_BLACK, COLOR_GRAY);

    init_color(COLOR_HIGHLIGHT, 800, 600, 600);
    init_pair(COLOR_HIGHLIGHT, COLOR_BLACK, COLOR_HIGHLIGHT);

    init_pair(COLOR_GREEN, COLOR_BLACK, COLOR_GREEN);

}

void
print_cell(int n, char *content)
{
    if (content == NULL) {
        content = "";
    }

    int attr = n % 2 ? COLOR_LIGHTER_GRAY : COLOR_LIGHT_GRAY;
    attron(COLOR_PAIR(attr));
    printw("%-" STR(CELL_SIZE) "s", content);
    attroff(COLOR_PAIR(attr));
}

void
print_y_axis(int n)
{
    attron(COLOR_PAIR(COLOR_GRAY));
    printw("%" STR(Y_AXIS_WIDTH) "d", n);
    attroff(COLOR_PAIR(COLOR_GRAY));
}

void
print_row_header(int row)
{
    char buf[4] = "   ";
    row++;

    size_t i = 0;

    while (row) {
        i++;
        buf[3 - i] = (row % 26) + L'A' - 1;
        row /= 26;
    }

    attron(COLOR_PAIR(COLOR_GRAY));
    printw("%" STR(CELL_SIZE) "s", buf);
    attroff(COLOR_PAIR(COLOR_GRAY));
}

void
print_sheet(struct sheet *s)
{
    addch('\n');
    attron(COLOR_PAIR(COLOR_GRAY));
    printw("%" STR(Y_AXIS_WIDTH) "s", "");
    attroff(COLOR_PAIR(COLOR_GRAY));

    int width = getmaxx(stdscr);
    int height = getmaxy(stdscr);

    int n_cells_wide = width / CELL_SIZE;

    for (int i = 0; i < n_cells_wide; i++) {
        print_row_header(i);
    }

    for (int i = 0; i < height; i++) {
        addch('\n');
        print_y_axis(i);

        for (int j = 0; j < n_cells_wide; j++) {
            print_cell(i + j, NULL);
        }
    }
    attroff(COLOR_LIGHT_GRAY);
}

void
print_book(struct book *b, size_t tab)
{
    attron(COLOR_PAIR(COLOR_GREEN));
    printw("%s", b->title);
    attroff(COLOR_PAIR(COLOR_GREEN));

    for (size_t i = 0; b->sheets[i] != NULL; i++) {
        int attr = tab == i ? COLOR_LIGHT_GRAY : COLOR_GRAY;
        attron(COLOR_PAIR(attr));
        printw("%10s ", b->sheets[i]->title);
        attroff(COLOR_PAIR(attr));
    }

    print_sheet(b->sheets[tab]);
}

void
highlight(int x, int y, struct cell *c)
{
    char *text;

    if (c == NULL) {
        text = "";
    } else {
        text = c->text;
    }

    attron(COLOR_PAIR(COLOR_HIGHLIGHT));
    mvprintw(y + 2, x * CELL_SIZE + Y_AXIS_WIDTH, "%" STR(CELL_SIZE) "s", text);
    attroff(COLOR_PAIR(COLOR_HIGHLIGHT));
}

void
unhighlight(int x, int y, struct cell *c)
{
    char *text;

    if (c == NULL) {
        text = "";
    } else {
        text = c->text;
    }

    int attr = (x + y) % 2 ? COLOR_LIGHTER_GRAY : COLOR_LIGHT_GRAY;
    attron(COLOR_PAIR(attr));
    mvprintw(y+2, x * CELL_SIZE + Y_AXIS_WIDTH, "%" STR(CELL_SIZE) "s", text);
    attroff(COLOR_PAIR(attr));
}

void
interact()
{
    static int sel_x = 0;
    static int sel_y = 0;

    while (1) {
        highlight(sel_x, sel_y, NULL);
        refresh();

        int c = getch();

        unhighlight(sel_x, sel_y, NULL);

        if (c == KEY_RIGHT) {
            sel_x++;
        }

        if (c == KEY_LEFT && sel_x > 0) {
            sel_x--;
        }

        if (c == KEY_UP && sel_y > 0) {
            sel_y--;
        }

        if (c == KEY_DOWN) {
            sel_y++;
        }
    }
}