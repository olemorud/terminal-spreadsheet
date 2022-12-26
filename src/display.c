
#include "display.h"
#include <curses.h>
#include <ncurses.h>
#include <signal.h>
#include "config.h"
#include "keymap.h"
#include <string.h>

#define _STR(x) #x
#define STR(x) _STR(x)


void highlight(int const row, int const col, struct cell *c);


int display_sel_x = 0;
int display_sel_y = 0;
int tab = 0;
int right_scrolled = 0;
int down_scrolled = 0;

struct book *b = NULL;

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
init_display(struct book *book)
{
    b = book;

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

    print_book(b, tab);

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
print_cell(int row, int col, char *content)
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
        buf[3 - i] = (row % 26) + 'A' - 1;
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

    int width = getmaxx(stdscr) - Y_AXIS_WIDTH;
    int height = getmaxy(stdscr) - 3;

    int n_cells_wide = width / CELL_SIZE;

    for (int i = 0; i < n_cells_wide; i++) {
        print_row_header(i);
    }

    for (int row = 0; row < height; row++) {
        addch('\n');
        print_y_axis(row);

        for (int col = 0; col < n_cells_wide; col++) {
            print_cell(row, col, NULL);
        }
    }
    attroff(COLOR_LIGHT_GRAY);
}

void
print_command_bar()
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
print_book(struct book *bk, size_t tab)
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

    print_sheet(bk->sheets[tab]);

    print_command_bar();
}

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
move_right()
{
    display_sel_x++;
    highlight(display_sel_x, display_sel_y, NULL);
}

void
move_left()
{
    if (display_sel_x > 0)
        display_sel_x--;
    highlight(display_sel_x, display_sel_y, NULL);
}

void
move_up()
{
    if (display_sel_y > 0)
        display_sel_y--;
    highlight(display_sel_x, display_sel_y, NULL);
}

void
move_down()
{
    display_sel_y++;
    highlight(display_sel_x, display_sel_y, NULL);
}

void
next_tab()
{
    tab++;
    if (tab >= b->n_sheets) {
        tab = 0;
    }
    print_book(b, tab);
}

void
prev_tab()
{
    tab--;
    if (tab < 0) {
        tab = b->n_sheets - 1;
    }
    print_book(b, tab);
}

void
handle_resize()
{
    print_book(b, tab);
    refresh();
}

void
write_right_status(char const *const s)
{
    int x, y;
    getmaxyx(stdscr, y, x);

    attron(COLOR_PAIR(COLOR_TITLE));
    mvprintw(y - 1, x - 1 - strlen(s), "%s", s);
    attron(COLOR_PAIR(COLOR_TITLE));
}

void
interact(struct book *b)
{
    while (1) {
        refresh();

        int c = getch();
        parse_key(c);
    }
}