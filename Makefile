CC=gcc
CFLAGS=-I.
DEPS = minesweeper.h
OBJ = test.o


CFLAGS2=-Isrc/include
LIB =-Lsrc/lib
l = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
OBJ2 = minesweeper.o



%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) -Wall

minesweeper.o: minesweeper.c minesweeper.h
	$(CC) -I. -Isrc/include -Lsrc/lib -O -c minesweeper.c $< -I. $(l) -Wall

minesweeper: $(OBJ2)
	$(CC) -o $@ $^ $(CFLAGS2) $(LIB) $(l) -Wall
