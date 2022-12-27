
#include "draw.h"
#include "cell.h"
#include "celltree.h"
#include "config.h"     // colors, sizes ...
#include "sheet.h"      // struct sheet
#include <ncurses.h>    // ncurses functions
#include <signal.h>     // handle sigterm
#include <string.h>     // strlen

#define _STR(x) #x
#define STR(x) _STR(x)

void highlight(int const row, int const col, struct cell *c);

void
cleanup()
{
    clear();
    echo();
    curs_set(1);
    refresh();
    endwin();
}

void
init_draw(struct book *book)
{

    initscr();
    atexit(cleanup);
    signal(SIGTERM, exit);
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

    init_color(COLOR_HIGHLIGHTED, 800, 600, 600);
    init_pair(COLOR_HIGHLIGHTED, COLOR_BLACK, COLOR_HIGHLIGHTED);

    init_color(COLOR_EDITMODE, 600, 800, 600);
    init_pair(COLOR_EDITMODE, COLOR_BLACK, COLOR_EDITMODE);

    init_pair(COLOR_TITLE, COLOR_BLACK, COLOR_GREEN);

    init_pair(COLOR_BACKGROUND, COLOR_MAGENTA, COLOR_GREEN);

    attrset(COLOR_PAIR(COLOR_BACKGROUND));

    draw_book(book, 0);

    highlight(0, 0, NULL);
    write_right_status("Normal");

    refresh();
}

int
get_cell_color(int const row, int const col)
{
    return (row + col) % 2 ? COLOR_LIGHTER_GRAY : COLOR_LIGHT_GRAY;
}

void
draw_cell(int row, int col, char *content)
{
    int color;

    if (content == NULL) {
        content = "";
    }

    color = get_cell_color(row, col);

    attron(COLOR_PAIR(color));
    printw("%-" STR(CELL_SIZE) "s", content);
    attroff(COLOR_PAIR(color));
}

void
draw_row_num(int n)
{
    attron(COLOR_PAIR(COLOR_GRAY));
    printw("%" STR(Y_AXIS_WIDTH) "d", n);
    attroff(COLOR_PAIR(COLOR_GRAY));
}

void
draw_row_header(int row)
{
    char buf[4] = "   ";
    row++;

    size_t i = 0;

    while (row) {
        i++;
        buf[3 - i] = (row % 26) + 'A' - 1;
        row /= 26;
    }

    attron(COLOR_PAIR(COLOR_GRAY));
    printw("%" STR(CELL_SIZE) "s", buf);
    attroff(COLOR_PAIR(COLOR_GRAY));
}

void
draw_sheet(struct sheet *s)
{
    addch('\n');
    attron(COLOR_PAIR(COLOR_GRAY));
    printw("%" STR(Y_AXIS_WIDTH) "s", "");
    attroff(COLOR_PAIR(COLOR_GRAY));

    int width = getmaxx(stdscr) - Y_AXIS_WIDTH;
    int height = getmaxy(stdscr) - 3;

    int n_cells_wide = width / CELL_SIZE;

    for (int i = 0; i < n_cells_wide; i++) {
        draw_row_header(i);
    }

    for (int row = 0; row < height; row++) {
        addch('\n');
        draw_row_num(row);

        for (int col = 0; col < n_cells_wide; col++) {
            draw_cell(row, col, NULL);
        }
    }
    attroff(COLOR_LIGHT_GRAY);
}

void
draw_input_bar()
{
    int width, height;

    getmaxyx(stdscr, height, width);

    move(height - 1, 0);

    attron(COLOR_PAIR(COLOR_TITLE));
    for (int i = 0; i < width; i++) {
        addch(' ');
    }
}

void
draw_book(struct book *bk, size_t tab)
{
    move(0, 0);
    attron(COLOR_PAIR(COLOR_TITLE));
    printw(" %s ", bk->title);
    attroff(COLOR_PAIR(COLOR_TITLE));

    for (size_t i = 0; i < bk->n_sheets; i++) {
        int attr = tab == i ? COLOR_GRAY : COLOR_LIGHT_GRAY;
        attron(COLOR_PAIR(attr));
        printw("%10s ", bk->sheets[i]->title);
        attroff(COLOR_PAIR(attr));
    }

    attron(COLOR_PAIR(COLOR_BACKGROUND));

    draw_sheet(bk->sheets[tab]);

    draw_input_bar();
}


/* highlights relative to screen */
void
highlight(int const x, int const y, struct cell *c)
{
    static int prev_x;
    static int prev_y;
    static struct cell *prev_c;
    int color;

    color = get_cell_color(prev_y, prev_x);
    attron(COLOR_PAIR(color));
    mvprintw(prev_y + 2, prev_x * CELL_SIZE + Y_AXIS_WIDTH,
             "%" STR(CELL_SIZE) "s", prev_c ? prev_c->text : "");

    attron(COLOR_PAIR(COLOR_HIGHLIGHTED));
    mvprintw(y + 2, x * CELL_SIZE + Y_AXIS_WIDTH, "%" STR(CELL_SIZE) "s",
             c ? c->text : "");
    attroff(COLOR_PAIR(color));

    move(y + 2, x * CELL_SIZE + Y_AXIS_WIDTH + 1);

    prev_x = x;
    prev_y = y;
    prev_c = c;
}

void
handle_resize(struct book* b, int tab)
{
    draw_book(b, tab);
    refresh();
}

void
write_left_command(char const *const s)
{
    int y = getmaxy(stdscr);

    attron(COLOR_PAIR(COLOR_TITLE));
    mvprintw(y - 1, 1, "%s", s);
    attron(COLOR_PAIR(COLOR_TITLE));
}

