CC=gcc
CFLAGS=-I. -g
SRC=./src/main.c ./src/operations.c ./src/tasks.c ./src/view.c ./src/theme.c
BIN=flake
PREFIX=package/flake-amd64/usr/bin

.PHONY: build clean install

build:
	$(CC) $(CFLAGS) -o $(BIN) $(SRC) -lSDL2 -lm

install: build
	mkdir -p $(PREFIX)
	cp $(BIN) $(PREFIX)/
	dpkg-deb --build package/flake-amd64

clean:
	rm -f $(BIN)