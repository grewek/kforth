SRC = src/main.c
CFLAGS = -Wall -Wpedantic -Werror -Wextra -g -O0 -std=c11
LDFLAGS = 
EXEC = bin/kforth

all: main
	cc $(CFLAGS) $(SRC) -o $(EXEC)