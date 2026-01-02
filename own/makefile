CC = gcc
CFLAGS = -Wall -Wextra -O2 \
				 $(shell pkg-config --cflags gtk+-3.0 vte-2.91)

LDFLAGS = $(shell pkg-config --libs gtk+-3.0 vte-2.91)

SRC = src/main.c src/terminal.c src/keys.c
OBJ = $(SRC:.c=.o)


BIN = build/own

all: $(BIN)

$(BIN): $(OBJ)
	mkdir -p build
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)


clean:
	rm -rf src/*.o build
