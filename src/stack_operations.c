#include "stack_operations.h"

//Unary Operations
void Output(ForthCell a) {
    switch(a.ct) {
        case CELL_INT:
            fprintf(stdout, "%d ok\n", a.innerType.integer);
        break;
        case CELL_STRING:
            fprintf(stdout, "%s ok\n", a.innerType.string.buffer);
        break;
        case CELL_VOID:
            fprintf(stdout, "NIL\n");
        break;
    }
}

ForthCell Duplicate(ForthCell *a) {
    ForthCell result;
    result.ct = a->ct;
    result.innerType = a->innerType;
    return result;
}

//Binary Operations
void Swap(ForthCell *a, ForthCell *b) {
    ForthCell temp = *a;
    *a = *b;
    *b = temp;
}

//Ternary Operations
void Rotate(ForthCell *a, ForthCell *b, ForthCell *c) {
    ForthCell temp = *b;
    *b = *a;
    *a = *c;
    *c = temp;
}