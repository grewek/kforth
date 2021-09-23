#include "forth_intrinsics.h"



ForthCell Plus(ForthCell a, ForthCell b) {
    if(a.ct != b.ct) {
        fprintf(stderr, "ERROR: Mismatching types!\n");
        exit(1);
    }

    switch(a.ct) {
        //NOTE: The block is there to prevent spillage of values into other blocks of the switch statement.
        case CELL_INT: {
            i32 valueA = a.innerType.integer;
            i32 valueB = b.innerType.integer;

            i32 sum = valueA + valueB;

            ForthCell result = {0};
            result.ct = a.ct;
            result.innerType.integer = sum;
            return result;
        } break;
        default:
            fprintf(stderr, "TODO: Make it possible to add these types !");
            exit(1);
        break;
    }
}

ForthCell Minus(ForthCell a, ForthCell b) {
    if(a.ct != b.ct) {
        fprintf(stderr, "ERROR: Mismatching types!\n");
        exit(1);
    }

    switch(a.ct) {
        //NOTE: The block is there to prevent spillage of values into other blocks of the switch statement.
        case CELL_INT: {
            i32 valueA = a.innerType.integer;
            i32 valueB = b.innerType.integer;

            i32 sum = valueA - valueB;

            ForthCell result = {0};
            result.ct = a.ct;
            result.innerType.integer = sum;
            return result;
        } break;
        default:
            fprintf(stderr, "TODO: Make it possible to add these types !");
            exit(1);
        break;
    }
}

ForthCell Multiply(ForthCell a, ForthCell b) {
    if(a.ct != b.ct) {
        fprintf(stderr, "ERROR: Mismatching types!\n");
        exit(1);
    }

    switch(a.ct) {
        //NOTE: The block is there to prevent spillage of values into other blocks of the switch statement.
        case CELL_INT: {
            i32 valueA = a.innerType.integer;
            i32 valueB = b.innerType.integer;

            i32 sum = valueA * valueB;

            ForthCell result = {0};
            result.ct = a.ct;
            result.innerType.integer = sum;
            return result;
        } break;
        default:
            fprintf(stderr, "TODO: Make it possible to add these types !");
            exit(1);
        break;
    }
}

ForthCell Divide(ForthCell a, ForthCell b) {
    if(a.ct != b.ct) {
        fprintf(stderr, "ERROR: Mismatching types!\n");
        exit(1);
    }

    switch(a.ct) {
        //NOTE: The block is there to prevent spillage of values into other blocks of the switch statement.
        case CELL_INT: {
            i32 valueA = a.innerType.integer;
            i32 valueB = b.innerType.integer;

            i32 sum = valueA / valueB;

            ForthCell result = {0};
            result.ct = a.ct;
            result.innerType.integer = sum;
            return result;
        } break;
        default:
            fprintf(stderr, "TODO: Make it possible to add these types !");
            exit(1);
        break;
    }
}

ForthCell Equal(ForthCell a, ForthCell b) {
    if(a.ct != b.ct) {
        fprintf(stderr, "ERROR: Mismatching types!\n");
        exit(1);
    }

    switch(a.ct) {
        //NOTE: The block is there to prevent spillage of values into other blocks of the switch statement.
        case CELL_INT: {
            i32 valueA = a.innerType.integer;
            i32 valueB = b.innerType.integer;

            i32 eResult = valueA == valueB ? -1 : 0;

            ForthCell result = {0};
            result.ct = a.ct;
            result.innerType.integer = eResult;
            return result;
        } break;
        default:
            fprintf(stderr, "TODO: Make it possible to add these types !");
            exit(1);
        break;
    }
}

ForthCell LessThan(ForthCell a, ForthCell b) {
    if(a.ct != b.ct) {
        fprintf(stderr, "ERROR: Mismatching types!\n");
        exit(1);
    }

    switch(a.ct) {
        //NOTE: The block is there to prevent spillage of values into other blocks of the switch statement.
        case CELL_INT: {
            i32 valueA = a.innerType.integer;
            i32 valueB = b.innerType.integer;

            i32 lResult = valueA < valueB ? -1 : 0;

            ForthCell result;
            result.ct = a.ct;
            result.innerType.integer = lResult;
            return result;
        } break;
        default:
            fprintf(stderr, "TODO: Make it possible to add these types !");
            exit(1);
        break;
    }
}

ForthCell GreaterThan(ForthCell a, ForthCell b) {
    //NOTE: According to the forth specs all values other than 0 are truthy values, but it seems like that -1.0 is the standard
    //      value...
    ;
    if(a.ct != b.ct) {
        fprintf(stderr, "ERROR: Mismatching types!\n");
        exit(1);
    }

    switch(a.ct) {
        //NOTE: The block is there to prevent spillage of values into other blocks of the switch statement.
        case CELL_INT: {
            i32 valueA = a.innerType.integer;
            i32 valueB = b.innerType.integer;

            i32 gResult = valueA > valueB ? -1.0 : 0.0;

            ForthCell result;
            result.ct = a.ct;
            result.innerType.integer = gResult;
            return result;
        } break;
        default:
            fprintf(stderr, "TODO: Make it possible to add these types !");
            exit(1);
        break;
    }
}

ForthCell DuplicateValue(ForthCell *a) {
    return *a;
}

void SwapValues(ForthCell *a, ForthCell *b) {
    ForthCell temp = *a;
    *a = *b;
    *b = temp;
}