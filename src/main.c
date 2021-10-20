#include <stdio.h>

#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#include "base_types.h"
#include "hash_map.h"
#include "lexer.h"
#include "token_list.h"

#include "forth_vm.h"
#include "forth_vm_backend.h"
#include "forth_parser.h"

int main(int argc, char **argv) 
{
    if(argc < 2) {
        fprintf(stderr, "USAGE: kforth [path]\n");
        exit(1);
    }

    const char *path = argv[1];

    TokenList list = GenerateTokenList(path);

    Parser parser = InitializeParser(list);

    Parse(&parser);

    ForthVM vm = {
        .stack = {0},
        .sp = 0,

        .instructions = RetrieveByteCode(&parser.gen),
        .pc = GetFunctionAddress(&parser, "main", 4),
    };

    while(true) {
        vm.pc += ExecuteByteCode(&vm);
    }
}
