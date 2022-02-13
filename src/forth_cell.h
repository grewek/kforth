#ifndef _FORTH_CELL_H_
#define _FORTH_CELL_H_

#include <stdio.h>
#include <stdlib.h>

#include "base_types.h"

typedef enum CellType {
    CELL_VOID,
    CELL_INT,
    CELL_STRING,
} CellType;

typedef struct ForthString {
    char *buffer;
    u64 size;
} ForthString;

typedef struct ForthCell {
    CellType ct;

    union {
        i32 integer;
        ForthString string;
    } innerType;
} ForthCell;

/*ForthCell ValueIntoCell(void *value, CellType ct);

ForthCell Plus(ForthCell a, ForthCell b);
ForthCell Minus(ForthCell a, ForthCell b);
ForthCell Multiply(ForthCell a, ForthCell b);
ForthCell Divide(ForthCell a, ForthCell b);

ForthCell Equal(ForthCell a, ForthCell b);
ForthCell LessThan(ForthCell a, ForthCell b);
ForthCell GreaterThan(ForthCell a, ForthCell b);

const char *CellTypeToString(CellType type);*/
#endif
