
#ifndef INTERFACE_H
#define INTERFACE_H

#include "book.h"

/*  */
void interface_commit_cell_change();

/* terminal resize handler */
void interface_handle_resize();

/* attach book to interface */
void interface_attach(struct book *b);

/* move one cell right */
void interface_move_right();

/* move one cell left */
void interface_move_left();

/* move one cell up */
void interface_move_up();

/* move one cell down */
void interface_move_down();

/* move to prev tab */
void interface_prev_tab();

/* move to next tab */
void interface_next_tab();

/* writes a status message in the bottom
 * right corner
 **/
void interface_write_right_status(char const *const s);

/* event loop */
void interface_interact();

/*
 * Insert Mode commands
 * --------------------
 */

/* Add char to input */
void interface_editor_append(const char c);

/* Remove last char */
void interface_editor_backspace();

/*
 * Mode switching
 * --------------
 */

/* switch to g mode TODO: find better way */
void interface_mode_g();

/* switch to insert mode */
void interface_mode_edit();

/* switch to normal mode */
void interface_mode_normal();

#endif
