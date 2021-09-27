#ifndef _INTEGER_OPERATIONS_H_
#define _INTEGER_OPERATIONS_H_

#include <stdio.h>
#include <stdlib.h>

#include "base_types.h"


//Arithmetic Operations on Integers
i32 PlusInt(i32 a, i32 b);
i32 MinusInt(i32 a, i32 b);
i32 MultiplyInt(i32 a, i32 b);
i32 DivideInt(i32 a, i32 b);

//Loigc Operations on Integers
i32 EqualInt(i32 a, i32 b);
i32 LessThanInt(i32 a, i32 b);
i32 GreaterThanInt(i32 a, i32 b);

#endif