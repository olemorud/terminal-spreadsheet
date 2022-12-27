
#include <curses.h>
#include <stdlib.h>
#include "interface.h"
#include "celltree.h"
#include "keymap.h"
#include "draw.h"

int g_display_sel_x = 0;
int g_display_sel_y = 0;
int g_tab = 0;
//int g_right_scrolled = 0;
//int g_down_scrolled = 0;

struct book *g_book = NULL;
struct cell *g_cur = NULL;

enum modes mode = Command;

void
init_interface(struct book *b)
{
    g_book = b;
}

void
move_right()
{
    g_display_sel_x++;
    highlight(g_display_sel_x, g_display_sel_y);
}

void
move_left()
{
    if (g_display_sel_x == 0)
        return;

    g_display_sel_x--;
    highlight(g_display_sel_x, g_display_sel_y);
}

void
move_up()
{
    if (g_display_sel_y == 0)
        return;

    g_display_sel_y--;
    highlight(g_display_sel_x, g_display_sel_y);
}

void
move_down()
{
    g_display_sel_y++;
    highlight(g_display_sel_x, g_display_sel_y);
}

void
next_tab()
{
    g_tab++;
    g_tab %= g_book->n_sheets;
    draw_book(g_book, g_tab);
}

void
prev_tab()
{
    g_tab--;
    if (g_tab < 0) {
        g_tab = g_book->n_sheets - 1;
    }
    draw_book(g_book, g_tab);
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
editor_backspace()
{
    size_t textlen = strlen(g_cur->text);

    if (textlen == 0)
        return;

    int x, y;

    addch(' ');
    getyx(stdscr, y, x);
    move(y, x - 1);

    g_cur->text = realloc(g_cur->text, textlen);
    g_cur->text[textlen-1] = '\0';

    refresh();
}

void
editor_append(const char c)
{
    int x, y;

    size_t textlen = strlen(g_cur->text);

    g_cur->text = realloc(g_cur->text, textlen+2);
    g_cur->text[textlen] = c;
    g_cur->text[textlen+1] = '\0';

    getyx(stdscr, y, x);
    draw_cell(g_cur);
    move(y, x);
    refresh();
}

void
start_edit_cell()
{
    /* struct cell* cur */
    g_cur = search_tree(g_book->sheets[g_tab]->root_cell, g_display_sel_x,
                      g_display_sel_y);

    if (g_cur == NULL) {
        g_cur = malloc(sizeof(struct cell));
        init_cell(g_cur, g_display_sel_x, g_display_sel_y, "", Text);
        insert_node(&g_book->sheets[g_tab]->root_cell, g_cur);
    }

    write_left_command(g_cur->text);
}

void
mode_normal()
{
    write_right_status("Normal");
    mode = Command;
    curs_set(0);
}

void
mode_g()
{
    write_right_status("g");
    mode = G;
}

void
mode_edit()
{
    mode = Edit;

    write_right_status("Insert");

    curs_set(1);
    echo();

    start_edit_cell();
}

void
interact(struct book *b)
{
    while (1) {
        refresh();

        int x, y;
        getmaxyx(stdscr, y, x);

        move(y-1, x-1);

        int c = getch();
        parse_key(c, mode);
    }
}