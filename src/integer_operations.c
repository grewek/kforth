#include "integer_operations.h"

//TODO: After everything is debugged and solidified we can inline the result variable into the return statement
//      but right now for debugging purposes it is better to leave it in this state. The compiler will probably
//      do the right thing anyway.
i32 PlusInt(i32 a, i32 b) {
    i32 result = a + b;
    return result;
}

i32 MinusInt(i32 a, i32 b) {
    i32 result = a - b;
    return result;
}

i32 MultiplyInt(i32 a, i32 b) {
    i32 result = a * b;
    return result;
}

i32 DivideInt(i32 a, i32 b) {
    i32 result = a / b;
    return result;
}

i32 EqualInt(i32 a, i32 b) {
    i32 result = a == b ? -1 : 0;
    return result;
}

i32 LessThanInt(i32 a, i32 b) {
    i32 result = a < b ? -1 : 0;
    return result;
}

i32 GreaterThanInt(i32 a, i32 b) {
    i32 result = a > b ? -1 : 0;
    return result;
}