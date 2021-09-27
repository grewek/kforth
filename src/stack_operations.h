#ifndef _STACK_OPERATIONS_H_
#define _STACK_OPERATIONS_H_

#include "forth_cell.h"

//Unary Operations
void Output(ForthCell a);
ForthCell Duplicate(ForthCell *a);

//Binary Operations
void Swap(ForthCell *a, ForthCell *b);

//Ternary Operations
void Rotate(ForthCell *a, ForthCell *b, ForthCell *c);

#endif