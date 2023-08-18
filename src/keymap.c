
#include "keymap.h"
#include "draw.h"
#include "interface.h"
#include <curses.h>

#define KEY_ESC 27

static void *const keymap_always[1024] = {
    [KEY_RESIZE] = interface_handle_resize,
    [27] /*ESC*/ = interface_mode_normal,
};

static void *const keymap_normal[1024] = {
    ['i']       = interface_mode_edit,
    ['a']       = interface_mode_edit,
    ['g']       = interface_mode_g,
    [KEY_LEFT]  = interface_move_left,
    ['h']       = interface_move_left,
    [KEY_DOWN]  = interface_move_down,
    ['j']       = interface_move_down,
    [KEY_UP]    = interface_move_up,
    ['k']       = interface_move_up,
    [KEY_RIGHT] = interface_move_right,
    ['l']       = interface_move_right,
};

static void *const keymap_edit[1024] = {
    [KEY_ESC]       = interface_mode_normal,
    [KEY_BACKSPACE] = interface_editor_backspace,
};

static void *const keymap_g[1024] = {
    ['t'] = interface_next_tab,
    ['T'] = interface_prev_tab,
};

static void
undefined()
{
    int height = getmaxy(stdscr);
    move(height - 1, 0);
    printw("key not defined");
}

void keymap_parse_key(size_t c, enum modes mode)
{
    void (*cmd)(void) = NULL;

    switch (mode) {
    case Command:
        cmd = keymap_normal[c];
        break;
    case G:
        cmd = keymap_g[c];
        break;
    case Edit:
        cmd = keymap_edit[c];
        if (cmd == NULL) {
            interface_editor_append(c);
            return;
        }
        if (c == KEY_ESC) {
            interface_commit_cell_change();
        }
        break;
    }

    if (mode != Edit) {
        mode = Command;
    }

    if (cmd == NULL) {
        cmd = keymap_always[c];

        if (cmd == NULL) {
            cmd = undefined;
        }
    }

    (*cmd)();
}
