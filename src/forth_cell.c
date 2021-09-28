#include "forth_cell.h"

ForthCell ValueIntoCell(void *value, CellType ct) {
    ForthCell result;

    switch(ct) {
        case CELL_INT:
            result.innerType.integer = *(i32 *)value;
        break;
        case CELL_STRING:
            result.innerType.string = *(ForthString *)value;
        break;
        default:
            fprintf(stderr, "ERROR: Cannot convert into cell of type %s.\n", CellTypeToString(ct));
            exit(1);
        break;
    }

    result.ct = ct;
    return result;
}

ForthCell Plus(ForthCell a, ForthCell b) {
    if(a.ct != b.ct) {
        fprintf(stderr, "ERROR: Mismatching types!\n");
        exit(1);
    }

    switch(a.ct) {
        case CELL_INT: {
            i32 value = PlusInt(a.innerType.integer, b.innerType.integer);
            return ValueIntoCell((void *)&value, CELL_INT);
        } break;
        default:
            fprintf(stderr, "TODO: Impossible to add cells of type %s.\n", CellTypeToString(a.ct));
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
        case CELL_INT: {
            i32 value = MinusInt(a.innerType.integer, b.innerType.integer);
            return ValueIntoCell((void *)&value, CELL_INT);
        } break;
        default:
            fprintf(stderr, "TODO: Impossible to subtract cells of type %s.\n", CellTypeToString(a.ct));
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
        case CELL_INT: {
            i32 value = MultiplyInt(a.innerType.integer, b.innerType.integer);
            return ValueIntoCell((void *)&value, CELL_INT);
        } break;
        default:
            fprintf(stderr, "TODO: Impossible to multiply cells of type %s.\n", CellTypeToString(a.ct));
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
        case CELL_INT: {
            i32 value = DivideInt(a.innerType.integer, b.innerType.integer);
            return ValueIntoCell((void *)&value, CELL_INT);
        } break;
        default:
            fprintf(stderr, "TODO: Impossible to divide cells of type %s.\n", CellTypeToString(a.ct));
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
            i32 value = EqualInt(a.innerType.integer, b.innerType.integer);
            return ValueIntoCell((void *)&value, CELL_INT);
        } break;
        default:
            fprintf(stderr, "Impossible to compare cells of type %s for equality.\n", CellTypeToString(a.ct));
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
            i32 value = LessThanInt(a.innerType.integer, b.innerType.integer);
            return ValueIntoCell((void *)&value, CELL_INT);
        } break;
        default:
            fprintf(stderr, "Impossible to compare cells of type %s.\n", CellTypeToString(a.ct));
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
            i32 value = GreaterThanInt(a.innerType.integer, b.innerType.integer);
            return ValueIntoCell((void *)&value, CELL_INT);
        } break;
        default:
            fprintf(stderr, "Impossible to compare cells of type %s.\n", CellTypeToString(a.ct));
            exit(1);
        break;
    }
}

const char *CellTypeToString(CellType type) {
    switch(type) {
        case CELL_INT:
            return "CELL_INTEGER";
        break;
        case CELL_STRING:
            return "CELL_STRING";
        break;
        case CELL_VOID:
            return "CELL_VOID";
        break;
    }

    return "Unhandled Type in CellTypeToString Function";
}