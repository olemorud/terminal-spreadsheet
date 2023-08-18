
#include "interface.h"
#include "cell.h"
#include "celltree.h"
#include "draw.h"
#include "keymap.h"
#include <curses.h>
#include <stdlib.h>

int g_display_sel_x = 0;
int g_display_sel_y = 0;
int g_tab           = 0;
// int g_right_scrolled = 0;
// int g_down_scrolled = 0;

struct book *g_book = NULL;
struct cell *g_cur  = NULL;

enum modes mode = Command;

void interface_attach(struct book *b)
{
    g_book = b;
}

void interface_move_right()
{
    g_display_sel_x++;
    draw_highlight(g_display_sel_x, g_display_sel_y);
}

void interface_move_left()
{
    if (g_display_sel_x == 0)
        return;

    g_display_sel_x--;
    draw_highlight(g_display_sel_x, g_display_sel_y);
}

void interface_move_up()
{
    if (g_display_sel_y == 0)
        return;

    g_display_sel_y--;
    draw_highlight(g_display_sel_x, g_display_sel_y);
}

void interface_move_down()
{
    g_display_sel_y++;
    draw_highlight(g_display_sel_x, g_display_sel_y);
}

void interface_next_tab()
{
    g_tab++;
    g_tab %= g_book->n_sheets;
    draw_book(g_book, g_tab);
}

void interface_prev_tab()
{
    g_tab--;
    if (g_tab < 0) {
        g_tab = g_book->n_sheets - 1;
    }
    draw_book(g_book, g_tab);
}

void interface_editor_backspace()
{
    size_t textlen = strlen(g_cur->text);

    if (textlen == 0)
        return;

    int x, y;

    addch(' ');
    getyx(stdscr, y, x);
    move(y, x - 1);

    g_cur->text              = realloc(g_cur->text, textlen);
    g_cur->text[textlen - 1] = '\0';

    refresh();
}

void interface_editor_append(const char c)
{
    int x, y;

    size_t textlen = strlen(g_cur->text);

    g_cur->text              = realloc(g_cur->text, textlen + 2);
    g_cur->text[textlen]     = c;
    g_cur->text[textlen + 1] = '\0';

    getyx(stdscr, y, x);
    draw_cell(g_cur);
    move(y, x);
    refresh();
}

static void
start_edit_cell()
{
    // struct cell* cur
    g_cur = celltree_search(g_book->sheets[g_tab]->root_cell, g_display_sel_x,
                            g_display_sel_y);

    if (g_cur == NULL) {
        g_cur = malloc(sizeof(struct cell));
        cell_init(g_cur, g_display_sel_x, g_display_sel_y, Text);
        celltree_insert(&g_book->sheets[g_tab]->root_cell, g_cur);
    }

    draw_left_status(g_cur->text);
}

void interface_mode_normal()
{
    draw_right_status("Normal");
    mode = Command;
    curs_set(0);
}

void interface_mode_g()
{
    draw_right_status("g");
    mode = G;
}

void interface_mode_edit()
{
    mode = Edit;
    draw_right_status("Insert");
    curs_set(1);
    echo();
    start_edit_cell();
}

void interface_commit_cell_change()
{
    cell_set_type(g_cur);
    draw_cell(g_cur);
}

void interface_handle_resize()
{
    draw_book(g_book, g_tab);
    refresh();
}

void interface_interact()
{
    refresh();

    int x, y;
    getmaxyx(stdscr, y, x);
    move(y - 1, x - 1);

    int c = getch();
    keymap_parse_key(c, mode);
}
