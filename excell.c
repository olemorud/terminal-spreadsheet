#include "config.h"
#include "book.h"
#include <stdio.h>


struct cell_list_node {
    struct cell           *cell;
    struct cell_list_node *next;
};



int main(int argc, char **argv){
    struct book b;
    init_book(&b, DEFAULT_BOOK_TITLE);

    printf("%s", b.sheets[0]->title);

    return 0;
}
