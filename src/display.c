
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

    set_escdelay(0);
    curs_set(0);

    start_color();
    init_color(COLOR_LIGHTER_GRAY, 900, 900, 900);
    init_pair(COLOR_LIGHTER_GRAY, COLOR_BLACK, COLOR_LIGHTER_GRAY);

    init_color(COLOR_LIGHT_GRAY, 800, 800, 800);
    init_pair(COLOR_LIGHT_GRAY, COLOR_BLACK, COLOR_LIGHT_GRAY);

    init_color(COLOR_GRAY, 650, 650, 650);
    init_pair(COLOR_GRAY, COLOR_BLACK, COLOR_GRAY);

    init_color(COLOR_COMMANDMODE, 800, 600, 600);
    init_pair(COLOR_COMMANDMODE, COLOR_BLACK, COLOR_COMMANDMODE);

    init_color(COLOR_EDITMODE, 600, 800, 600);
    init_pair(COLOR_EDITMODE, COLOR_BLACK, COLOR_EDITMODE);

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
    int n_cells_tall = height - 3;

    for (int i = 0; i < n_cells_wide; i++) {
        print_row_header(i);
    }

    for (int i = 0; i < n_cells_tall; i++) {
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
highlight(int x, int y, struct cell *c, enum modes mode)
{
    static int prev_x;
    static int prev_y;
    static struct cell *prev_c;
    int attr;

    attr = (prev_x + prev_y) % 2 ? COLOR_LIGHTER_GRAY : COLOR_LIGHT_GRAY;
    attron(COLOR_PAIR(attr));
    mvprintw(prev_y+2, prev_x * CELL_SIZE + Y_AXIS_WIDTH, "%" STR(CELL_SIZE) "s", prev_c ? prev_c->text : "");
    attroff(COLOR_PAIR(attr));

    if (mode == command) {
        attr = COLOR_COMMANDMODE;
    } else if (mode == edit) {
        attr = COLOR_EDITMODE;
    }

    attron(COLOR_PAIR(attr));
    mvprintw(y + 2, x * CELL_SIZE + Y_AXIS_WIDTH, "%" STR(CELL_SIZE) "s", c ? c->text : "");
    attroff(COLOR_PAIR(attr));

    prev_x = x;
    prev_y = y;
    prev_c = c;
}

void
interact()
{
    static int sel_x = 0;
    static int sel_y = 0;
    enum modes mode = command;

    while (1) {
        highlight(sel_x, sel_y, NULL, mode);
        refresh();

        int c = getch();

        if (mode == edit) {
            set_escdelay(0);

            if (c == 27) {
                mode = command;
            }
        }

        if(mode == command) {
            switch (c) {
            case KEY_RIGHT:
                sel_x++;
                break;

            case KEY_LEFT:
                if(sel_x > 0)
                    sel_x--;
                break;

            case KEY_UP:
                if(sel_y > 0)
                    sel_y--;
                break;

            case KEY_DOWN:
                sel_y++;
                break;
            
            case 'i':
                mode = edit;
                break;
            }
        }
    }
}