SRC = src/
CFLAGS = -Wall -Wpedantic -Werror -Wextra -g -O0 -std=c11
OBJDEST = objs/
EXEC = bin/kforth

hash_map.o: $(SRC)hash_map.c
	cc -c $(CFLAGS) $< -o $(OBJDEST)$@

forth_function.o: $(SRC)forth_function.c
	cc -c $(CFLAGS) $< -o $(OBJDEST)$@

forth_intrinsics.o: $(SRC)forth_intrinsics.c
	cc -c $(CFLAGS) $< -o $(OBJDEST)$@

forth_stack.o: $(SRC)forth_stack.c
	cc -c $(CFLAGS) $< -o $(OBJDEST)$@

forth_evaluator.o: $(SRC)forth_evaluator.c
	cc -c $(CFLAGS) $< -o $(OBJDEST)$@

token_list.o: $(SRC)token_list.c
	cc -c $(CFLAGS) $< -o $(OBJDEST)$@

lexer.o: $(SRC)lexer.c
	cc -c $(CFLAGS) $< -o $(OBJDEST)$@

main.o: $(SRC)main.c
	cc -c $(CFLAGS) $< -o $(OBJDEST)$@

kforth: main.o lexer.o forth_function.o forth_intrinsics.o forth_stack.o forth_evaluator.o token_list.o hash_map.o
	cc objs/main.o objs/lexer.o objs/forth_function.o objs/forth_intrinsics.o objs/forth_stack.o objs/forth_evaluator.o objs/token_list.o objs/hash_map.o -o bin/kforth

clean:
	rm bin/*
	rm objs/*