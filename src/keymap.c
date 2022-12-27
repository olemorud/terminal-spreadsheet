
#include "keymap.h"
#include "draw.h"
#include "interface.h"
#include <curses.h>



void * const keymap_always[1024] = {
    [KEY_RESIZE] = handle_resize,
    [27] /*ESC*/ = mode_normal,
};

void * const keymap_normal[1024] = {
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

void * const keymap_edit[1024] = {
    [27]            = mode_normal,
    [KEY_BACKSPACE] = editor_backspace,
};

void * const keymap_g[1024] = {
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
parse_key(size_t c, enum modes mode)
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
        if (cmd == NULL){
            editor_append(c);
            return;
        }
        break;
    }

    if(mode != Edit) {
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