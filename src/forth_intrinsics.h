#ifndef _FORTH_INTRINSICS_H_
#define _FORTH_INTRINSICS_H_

#include <stdio.h>
#include <stdlib.h>

#include "forth_cell.h"
#include "forth_stack.h"
#include "base_types.h"

//Arithmetic Operations
ForthCell Plus(ForthCell a, ForthCell b);
ForthCell Minus(ForthCell a, ForthCell b);
ForthCell Multiply(ForthCell a, ForthCell b);
ForthCell Divide(ForthCell a, ForthCell b);

//Logic Operations
ForthCell Equal(ForthCell a, ForthCell b);
ForthCell LessThan(ForthCell a, ForthCell b);
ForthCell GreaterThan(ForthCell a, ForthCell b);

//Reference Based Stack Operations
ForthCell DuplicateValue(ForthCell *a);
void SwapValues(ForthCell *a, ForthCell *b);

#endif