SRC = src/
CFLAGS = -DLINUX_BUILD -Wall -Wpedantic -Werror -Wextra -g -O0 -std=c11
OBJDEST = objs/
EXEC = bin/kforth

hash_map.o: $(SRC)hash_map.c
	cc -c $(CFLAGS) $< -o $(OBJDEST)$@

forth_vm.o: $(SRC)forth_vm.c
	cc -c $(CFLAGS) $< -o $(OBJDEST)$@

forth_vm_backend.o: $(SRC)forth_vm_backend.c
	cc -c $(CFLAGS) $< -o $(OBJDEST)$@

forth_parser.o: $(SRC)forth_parser.c
	cc -c $(CFLAGS) $< -o $(OBJDEST)$@

token_list.o: $(SRC)token_list.c
	cc -c $(CFLAGS) $< -o $(OBJDEST)$@

lexer.o: $(SRC)lexer.c
	cc -c $(CFLAGS) $< -o $(OBJDEST)$@

main.o: $(SRC)main.c
	cc -c $(CFLAGS) $< -o $(OBJDEST)$@

kforth: main.o lexer.o forth_parser.o forth_vm.o token_list.o hash_map.o forth_vm_backend.o
	cc objs/main.o objs/lexer.o objs/forth_parser.o objs/forth_vm.o objs/token_list.o objs/hash_map.o objs/forth_vm_backend.o -o bin/kforth

clean:
	rm bin/*
	rm objs/*
