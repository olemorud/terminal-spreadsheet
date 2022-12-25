#include "book.h"


void
init_book(struct book *b, char *title)
{

    b->sheets = malloc(sizeof(struct sheet*) * 2);

    init_sheet((b->sheets)[0], DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_SHEET_TITLE"1");
    (b->sheets)[1] = NULL;
    
    if ( title ) {
        b->title = strndup(title, MAX_STR);
    } else {
        b->title = strdup("");
    }
}

void
delete_book(struct book* b)
{
    size_t i = 0;
    while ( (b->sheets)[i] != NULL ) {
        delete_sheet((b->sheets)[i]);
        i++;
    }
}
