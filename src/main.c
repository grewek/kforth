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
//#include "forth_stack.h"
#include "forth_parser.h"
//#include "forth_function.h"
//#include "forth_evaluator.h"

int main(int argc, char **argv) 
{
    if(argc < 2) {
        fprintf(stderr, "USAGE: kforth [path]\n");
        exit(1);
    }

    const char *path = argv[1];

    u32 instructions[128] = {0};
    u64 addresses[1024] = {0};
    Generator gen = InitializeGenerator(instructions, 32);
    TokenList list = GenerateTokenList(path);

    Parser parser = {0};
    parser.tokens = &list;
    parser.addressBook = addresses;
    parser.gen = &gen;

    Parse(parser);
    RetrieveByteCode(&gen);
    

    ForthVM vm = {
        .stack = {0},
        .sp = 0,

        .instructions = instructions,
        .pc = GetFunctionAddress(parser, "main", 4),
    };

    while(true) {
        vm.pc += ExecuteByteCode(&vm);
    }
}
