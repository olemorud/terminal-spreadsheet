
#ifndef KEYMAP_H
#define KEYMAP_H

#include <stdlib.h>
#include <err.h>
#include "display.h"
#include <ncurses.h>
#include <stddef.h>

void parse_key(size_t c);

enum modes {Edit, Command, G};

#endif