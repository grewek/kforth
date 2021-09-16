SRC = src/main.c
CFLAGS = -Wall -Wpedantic -Werror -g -O0 -std=c11
LDFLAGS = 


all: main
	cc $(CFLAGS) $(SRC) -o main