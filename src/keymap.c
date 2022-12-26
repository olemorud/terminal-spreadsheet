
#include "keymap.h"
#include "display.h"
#include <curses.h>

void mode_g();

void* keymap_always[1024] = {
    [KEY_RESIZE] = handle_resize,
};

void* keymap_normal[1024] = {
    [KEY_LEFT]  = move_left,
    ['h']       = move_left,
    [KEY_DOWN]  = move_down,
    ['j']       = move_down,
    [KEY_UP]    = move_up,
    ['k']       = move_up,
    [KEY_RIGHT] = move_right,
    ['l']       = move_right,
    ['g']       = mode_g,
};

enum modes mode = command;

void* keymap_g[1024] = {
    ['t'] = next_tab,
    ['T'] = prev_tab,
};

void undefined() {
    int height = getmaxy(stdscr);
    move(height-1, 0);
    printw("key not defined");
}

void mode_g() {
    mode = g;
}

void parse_key(size_t c) {
    void (*cmd)(void);

    switch(mode) {
    case command:
        cmd = keymap_normal[c];
        break;
    case g:
        cmd = keymap_g[c];
        mode = command;
        break;
    case edit:
        cmd = NULL;
        break;
    }

    if(cmd == NULL) {
        cmd = keymap_always[c];

        if (cmd == NULL) {
            cmd = undefined;
        }
    }

    (*cmd)();
}