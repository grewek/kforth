#include "forth_function.h"

ForthFunction InitializeFunction(u64 size) {
    ForthFunction result = {0};
    result.count = 0;
    result.size = size;
    result.instructions = calloc(result.size, sizeof(Instruction));

    if(result.instructions == NULL) {
        fprintf(stderr, "ERROR: Could not reserve memory for forthfunction\n");
        exit(1);
    }

    return result;
}

void FreeFunction(ForthFunction *function) {
    if(function->instructions != NULL) {
        free(function->instructions);
        function->instructions = NULL;
    }
}