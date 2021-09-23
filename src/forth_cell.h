#ifndef _FORTH_CELL_H_
#define _FORTH_CELL_H_

#include "base_types.h"

typedef enum CellType {
    CELL_INT,
    CELL_STRING,
    CELL_VOID,
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

#endif