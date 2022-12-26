
#include "keymap.h"
#include "display.h"
#include <curses.h>

void mode_g();
void mode_edit();
void mode_normal();

void *keymap_always[1024] = {
    [KEY_RESIZE] = handle_resize,
    [27] /*ESC*/ = mode_normal,
};

void *keymap_normal[1024] = {
    ['i']       = mode_edit,
    ['a']       = mode_edit,
    ['g']       = mode_g,
    [KEY_LEFT]  = move_left,
    ['h']       = move_left,
    [KEY_DOWN]  = move_down,
    ['j']       = move_down,
    [KEY_UP]    = move_up,
    ['k']       = move_up,
    [KEY_RIGHT] = move_right,
    ['l']       = move_right,
};

enum modes mode = Command;

void *keymap_g[1024] = {
    ['t'] = next_tab,
    ['T'] = prev_tab,
};

void
undefined()
{
    int height = getmaxy(stdscr);
    move(height - 1, 0);
    printw("key not defined");
}

void
mode_normal()
{
    write_status("Normal");
    mode = Command;
    curs_set(0);

}

void
mode_g()
{
    write_status("g");
    mode = G;
}

void
mode_edit()
{
    int x, y;
    getyx(stdscr, y, x);

    mode = Edit;

    write_status("Insert");

    curs_set(1);
    echo();
    move(y, x - 2);
}

void
parse_key(size_t c)
{
    void (*cmd)(void);

    switch (mode) {
    case Command:
        cmd = keymap_normal[c];
        break;
    case G:
        cmd = keymap_g[c];
        mode = Command;
        break;
    case Edit:
        cmd = NULL;
        break;
    }

    if (cmd == NULL) {
        cmd = keymap_always[c];

        if (cmd == NULL) {
            cmd = undefined;
        }
    }

    (*cmd)();
}