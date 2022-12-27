
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

void highlight(int const row, int const col);

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

    bkgdset(COLOR_PAIR(COLOR_BACKGROUND));

    draw_book(book, 0);

    highlight(0, 0);
    write_right_status("Normal");

    refresh();
}

int
cell_x_pos(int x)
{
    return x * CELL_SIZE + Y_AXIS_WIDTH;
}

int
cell_y_pos(int y)
{
    return y + 2;
}

int
get_cell_color(int const row, int const col)
{
    return (row + col) % 2 ? COLOR_LIGHTER_GRAY : COLOR_LIGHT_GRAY;
}

void
draw_cell(struct cell const *const cell)
{
    int x = cell_x_pos(cell->x_pos);
    int y = cell_y_pos(cell->y_pos);

    int color = get_cell_color(y, x);

    move(y, x);

    attron(COLOR_PAIR(color));

    switch(cell->type){
        case Integer:
            printw("%" STR(CELL_SIZE) "li", cell->value.integer);
            break;
        case Floating:
            printw("%" STR(CELL_SIZE) "lf", cell->value.floating);
            break;
        default:
            printw("%-" STR(CELL_SIZE) "s", cell->text);
            break;
    }

    attroff(COLOR_PAIR(color));
}

void
draw_cells_in_tree(struct cell_tree_node const *const root)
{
    if (root == NULL) {
        return;
    }

    draw_cell(root->cell);
    draw_cells_in_tree(root->right);
    draw_cells_in_tree(root->left);
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

    int width = getmaxx(stdscr) - Y_AXIS_WIDTH;
    int n_cells_wide = width / CELL_SIZE;
    int n_cells_tall = getmaxy(stdscr) - 3;

    // print row headers
    chgat(Y_AXIS_WIDTH, 0, COLOR_GRAY, NULL);
    for (int i = 0; i < n_cells_wide; i++) {
        draw_row_header(i);
    }

    // clear sheet before drawing on top
    for (int row = 0; row < n_cells_tall; row++) {
        int color;
        addch('\n');
        draw_row_num(row);

        for (int col = 0; col < n_cells_wide; col++) {
            color = get_cell_color(row, col);

            attron(COLOR_PAIR(color));

            mvprintw(cell_y_pos(row),
                    cell_x_pos(col),
                    "%" STR(CELL_SIZE) "s", " ");
        }
        attroff(COLOR_PAIR(color));
    }
    
    // draw on top of empty sheet
    draw_cells_in_tree(s->root_cell);
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

    draw_sheet(bk->sheets[tab]);

    draw_input_bar();
}

/* highlights relative to screen */
void
highlight(int const x, int const y)
{
    static int prev_x;
    static int prev_y;
    int color;

    mvchgat(cell_y_pos(y), cell_x_pos(x), CELL_SIZE, 0, COLOR_HIGHLIGHTED,
            NULL);

    color = get_cell_color(prev_y, prev_x);

    mvchgat(cell_y_pos(prev_y), cell_x_pos(prev_x), CELL_SIZE, 0, color, NULL);

    prev_x = x;
    prev_y = y;
}

void
write_left_command(char const *const s)
{
    int y = getmaxy(stdscr);

    attron(COLOR_PAIR(COLOR_TITLE));
    mvprintw(y - 1, 1, "%s", s);
    attron(COLOR_PAIR(COLOR_TITLE));
}
