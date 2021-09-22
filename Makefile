SRC = src/main.c src/lexer.c src/token_list.c src/forth_function.c src/hash_map.c src/forth_intrinsics.c src/forth_stack.c
CFLAGS = -Wall -Wpedantic -Werror -Wextra -g -O0 -std=c11
LDFLAGS = 
EXEC = bin/kforth

all:
	cc $(CFLAGS) $(SRC) -o $(EXEC)