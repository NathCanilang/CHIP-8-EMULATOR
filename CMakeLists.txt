CC = gcc
CFLAGS = -Iinclude -Wall
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
OUT = bin/chip8_emulator

all:
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -rf bin/*