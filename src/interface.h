
#ifndef INTERFACE_H
#define INTERFACE_H

#include "book.h"

void init_interface(struct book *b);
void move_right();
void move_left();
void move_up();
void move_down();
void prev_tab();
void next_tab();
void write_right_status(char const *const s);
void editor_backspace();
void start_edit_cell();
void interact(struct book *b);
void editor_append(const char c);
void editor_backspace();
void mode_g();
void mode_edit();
void mode_normal();
void commit_cell_change();





#endif