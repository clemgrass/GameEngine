CC = gcc
CFLAGS = -Wall -Wextra -Werror

COMMON = Shared/print.c

# output final
OUT = app

# FILE se pasa desde la terminal
FILE ?= bmp/main.c

all: run

build:
	$(CC) $(CFLAGS) $(COMMON) $(FILE) -o $(OUT)

run: build
	./$(OUT)

clean:
	rm -f $(OUT)