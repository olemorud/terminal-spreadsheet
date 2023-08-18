#ifndef CELL_H
#define CELL_H

#include "common.h"
#include <stddef.h>
#include <stdint.h>
#include <time.h>

enum cell_type { Text,
                 Integer,
                 Floating,
                 Date };

struct cell_list {
    struct cell      *cell;
    struct cell_list *next;
};

struct cell {
    struct cell_list *referenced_by;
    size_t            x_pos;
    size_t            y_pos;
    char             *text;
    enum cell_type    type;
    union {
        int64_t integer;
        double  floating;
        time_t  date;
    } value;
};

void cell_init(struct cell *c, size_t x_pos, size_t y_pos, enum cell_type type);
void cell_delete(struct cell *c);
void cell_set_type(struct cell *cell);

#endif
