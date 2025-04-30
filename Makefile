CC=gcc
CFLAGS=-I. -g

.PHONY: build clean

build: ./src/main.c ./src/operations.c ./src/tasks.c
	$(CC) -o flake ./src/main.c ./src/operations.c ./src/tasks.c -lSDL2 -lm

clean:
	rm -f flake
