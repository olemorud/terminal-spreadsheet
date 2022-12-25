
CC = gcc
CFLAGS = -g -Wall
LIBS=

_OBJ = main.o book.o cell.o celltree.o sheet.o $(shell)
OBJ = $(patsubst %,obj/%,$(_OBJ))

obj/%.o: %.c | objdir
	$(CC) -c -o $@ $< $(CFLAGS)

spreadsheet: $(OBJ) | bindir
	$(CC) -o bin/spreadsheet $^ $(CFLAGS) $(LIBS)

objdir:
	mkdir -p obj

bindir:
	mkdir -p bin
