#include "book.h"
#include "config.h"
#include "draw.h"
#include "interface.h"
#include "sheet.h"
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    (void)argv;
    (void)argc;

    struct book b;

    book_init(&b, DEFAULT_BOOK_TITLE);

    book_add_sheet(&b);
    book_add_sheet(&b);
    book_add_sheet(&b);

    draw_init(&b);

    interface_attach(&b);

    while (1) {
        interface_interact();
    }

    return 0;
}
