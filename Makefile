SRC = src/main.c src/lexer.c src/token_list.c
CFLAGS = -Wall -Wpedantic -Werror -Wextra -g -O0 -std=c11
LDFLAGS = 
EXEC = bin/kforth

all:
	cc $(CFLAGS) $(SRC) -o $(EXEC)