CC=gcc
CFLAGS=-I. -g

.PHONY: build clean

build: main.c operations.c tasks.c
	$(CC) -o flake main.c operations.c tasks.c -lSDL2 

clean:
	rm -f flake
