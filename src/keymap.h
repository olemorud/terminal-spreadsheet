
#ifndef KEYMAP_H
#define KEYMAP_H

#include "common.h"
#include <stddef.h>

void parse_key(size_t c);

enum modes {Edit, Command, G};

#endif