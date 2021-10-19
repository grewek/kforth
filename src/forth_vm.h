#ifndef _FORTH_VM_
#define _FORTH_VM_

#include <stdbool.h>

#include "base_types.h"
#include "forth_cell.h"

typedef enum ByteCode {
    //Stack opcodes
    OPCODE_PUSH,
    OPCODE_POP,

    //Subroutine opcodes
    OPCODE_CALL,
    OPCODE_RET,

    //Relative Jumps opcodes
    OPCODE_GOTO,
    OPCODE_IF_GOTO,

    //Comparisions opcodes
    OPCODE_EQ,
    OPCODE_GT,
    OPCODE_LT,

    //Arithmetic opcodes
    OPCODE_ADD,
    OPCODE_SUB,
    OPCODE_MUL,
    OPCODE_DIV,

    //Intrinsic Opcodes
    OPCODE_DOT,
    OPCODE_SWAP,
    OPCODE_DUP,
    OPCODE_ROTATE,

    OPCODE_NOP,
} ByteCode;

typedef struct ForthVM {
    u32 callStack[256];
    u32 cp;

    i32 stack[256];
    u32 sp;

    u32 *instructions;
    u32 pc;

} ForthVM;

void vm__basic_arithmetic_subprogram_test();
void vm__branches_test();

u32 ExecuteByteCode(ForthVM *vm);

void VM_PushReturn(ForthVM *vm, u32 address);
void VM_Push(ForthVM *vm, u32 value);
void VM_Call(ForthVM *vm, u32 target);
void VM_Ret(ForthVM *vm);
void VM_Goto(ForthVM *vm, u32 offset);
void VM_IfGoto(ForthVM *vm, u32 offset);

i32  VM_Pop(ForthVM *vm);
u32  VM_PopReturn(ForthVM *vm);

void VM_Eq(ForthVM *vm);
void VM_Gt(ForthVM *vm);
void VM_Lt(ForthVM *vm);

void VM_Add(ForthVM *vm);
void VM_Sub(ForthVM *vm);
void VM_Mul(ForthVM *vm);
void VM_Div(ForthVM *vm);

void VM_Dot(ForthVM *vm);
void VM_Swap(ForthVM *vm);
void VM_Dup(ForthVM *vm);
void VM_Rotate(ForthVM *vm);

#endif
