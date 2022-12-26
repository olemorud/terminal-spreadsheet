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

    struct book b;

    init_book(&b, DEFAULT_BOOK_TITLE);

    add_sheet(&b);
    add_sheet(&b);
    add_sheet(&b);

    init_display(&b);

    interact(&b);

    return 0;
}
