#include "book.h"
#include "display.h"
#include "config.h"
#include "sheet.h"
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>


int
main(int argc, char **argv)
{
    init_display();

    struct book b;

    init_book(&b, DEFAULT_BOOK_TITLE);

    b.sheets = realloc(b.sheets, 4 * sizeof(struct sheet *));

    b.sheets[1] = malloc(sizeof(struct sheet));
    init_sheet(b.sheets[1], DEFAULT_WIDTH, DEFAULT_HEIGHT,
               DEFAULT_SHEET_TITLE "2");

    b.sheets[2] = malloc(sizeof(struct sheet));
    init_sheet(b.sheets[2], DEFAULT_WIDTH, DEFAULT_HEIGHT,
               DEFAULT_SHEET_TITLE "3");

    b.sheets[3] = NULL;

    //printf("%s", b.sheets[0]->title);

    print_book(&b, 0);
    
    interact();

    endwin();

    return 0;
}
