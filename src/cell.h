#ifndef CELL_H
#define CELL_H

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include "common.h"

enum cell_type {
    Text,
    Integer,
    Floating,
    Date
};

struct cell_list_node {
    struct cell           *cell;
    struct cell_list_node *next;
};

struct cell {
    struct cell_list_node *referenced_by;
    size_t                 x_pos;
    size_t                 y_pos;
    char                  *text;
    enum cell_type         type;
    union {
        int64_t integer;
        double  floating;
        time_t  date;
    }                      value;
};

void init_cell(struct cell *c, size_t x_pos, size_t y_pos, char *text,
               enum cell_type type);
void delete_cell(struct cell *c);
void set_type(struct cell *cell);

#endif