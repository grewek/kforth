#include "forth_vm.h"

u32 ExecuteByteCode(ForthVM *vm) 
{
    //Fetch the next instruction.
    u32 instruction = vm->instructions[vm->pc];
    u32 argA = vm->instructions[vm->pc + 1];
    //u32 undecodedArgB = vm->instructions[vm->pc + 2];

    ByteCode instructionType = (ByteCode)instruction;

    u32 opcodeLength = 1;
    switch(instructionType) 
    {
        case OPCODE_PUSH:
            VM_Push(vm, argA);
            opcodeLength = 2;
        break;
        case OPCODE_POP:
            VM_Pop(vm);
            opcodeLength = 1;
        break;
        case OPCODE_CALL:
            assert(argA != UINT_MAX);
            VM_Call(vm, argA);
            opcodeLength = 0;
        break;
        case OPCODE_RET:
            VM_Ret(vm);
            opcodeLength = 0;
        break;
        case OPCODE_GOTO:
            VM_Goto(vm, argA);
            opcodeLength = 0;
        break;
        case OPCODE_IF_GOTO:
            VM_IfGoto(vm, argA);
            opcodeLength = 2;
        break;
        case OPCODE_EQ:
            VM_Eq(vm);
            opcodeLength = 1;
        break;
        case OPCODE_GT:
            VM_Gt(vm);
            opcodeLength = 1;
        break;
        case OPCODE_LT:
            VM_Lt(vm);
            opcodeLength = 1;
        break;
        case OPCODE_ADD:
            VM_Add(vm);
            opcodeLength = 1;
        break;
        case OPCODE_SUB:
            VM_Sub(vm);
            opcodeLength = 1;
        break;
        case OPCODE_MUL:
            VM_Mul(vm);
            opcodeLength = 1;
        break;
        case OPCODE_DIV:
            VM_Div(vm);
            opcodeLength = 1;
        break;
        case OPCODE_NOP:
            opcodeLength = 1;
        break;
        case OPCODE_DOT:
            VM_Dot(vm);
            opcodeLength = 1;
        break;
        case OPCODE_SWAP:
        break;
        case OPCODE_DUP:
        break;
        case OPCODE_ROTATE:
        break;
    }

    return opcodeLength;
}

void VM_Push(ForthVM *vm, u32 value) 
{
    vm->stack[vm->sp] = value;
    vm->sp += 1;
}

void VM_PushReturn(ForthVM *vm, u32 address)
{
    vm->callStack[vm->cp] = address;
    vm->cp += 1;
}

i32 VM_Pop(ForthVM *vm)
{
    vm->sp -= 1;
    return vm->stack[vm->sp];
}

u32 VM_PopReturn(ForthVM *vm)
{
    vm->cp -= 1;
    if(vm->cp > 256)
    {
        //NOTE: If the call stack pointer overflow it's maximum value it means
        //      that we reached the end of the main function and we should
        //      return control to the operating system...
        exit(0);
    }
    return vm->callStack[vm->cp];
}

void VM_Call(ForthVM *vm, u32 target)
{
    VM_PushReturn(vm, vm->pc + 2);
    vm->pc = target;
}

void VM_Ret(ForthVM *vm)
{
    u32 returnAddress = VM_PopReturn(vm);
    vm->pc = returnAddress;
}

void VM_Goto(ForthVM *vm, u32 offset)
{
    vm->pc = (u32)((i32)vm->pc  + (i32)offset);
}

void VM_IfNotEqualGoto(ForthVM *vm, u32 offset)
{
    i32 booleanValue = VM_Pop(vm);

    if(booleanValue != -1)
    {
        vm->pc = (u32)((i32)vm->pc + (i32)offset);
    }
}

void VM_IfGoto(ForthVM *vm, u32 offset)
{
    i32 booleanValue = VM_Pop(vm);

    if(booleanValue != -1)
    {
        vm->pc = (u32)((i32)vm->pc + (i32)offset);
    }
}

void VM_Eq(ForthVM *vm)
{
    i32 valueA = VM_Pop(vm);
    i32 valueB = VM_Pop(vm);

    i32 result = valueA == valueB ? -1 : 0;
    VM_Push(vm, result);
}

void VM_Gt(ForthVM *vm)
{
    i32 valueA = VM_Pop(vm);
    i32 valueB = VM_Pop(vm);

    i32 result = valueA > valueB ? -1 : 0;
    VM_Push(vm, result);
}

void VM_Lt(ForthVM *vm) 
{
    i32 valueA = VM_Pop(vm);
    i32 valueB = VM_Pop(vm);

    i32 result = valueA < valueB ? -1 : 0;
    VM_Push(vm, result);
}

void VM_Add(ForthVM *vm)
{
    i32 valueA = VM_Pop(vm);
    i32 valueB = VM_Pop(vm);

    i32 result = valueA + valueB;

    VM_Push(vm, result);
}

void VM_Sub(ForthVM *vm)
{
    i32 valueA = VM_Pop(vm);
    i32 valueB = VM_Pop(vm);

    i32 result = valueA - valueB;

    VM_Push(vm, result);
}

void VM_Mul(ForthVM *vm)
{
    i32 valueA = VM_Pop(vm);
    i32 valueB = VM_Pop(vm);

    i32 result = valueA * valueB;

    VM_Push(vm, result);
}

void VM_Div(ForthVM *vm)
{
    i32 valueA = VM_Pop(vm);
    i32 valueB = VM_Pop(vm);
    
    i32 result = valueA * valueB;

    VM_Push(vm, result);
}

void VM_Dot(ForthVM *vm)
{
    i32 value = VM_Pop(vm);

    fprintf(stdout, "%d\n", value);
}

void VM_Swap(ForthVM *vm)
{
    i32 *valueA = &vm->stack[vm->sp];
    i32 *valueB = &vm->stack[vm->sp - 1];

    i32 temp = *valueA;
    *valueA = *valueB;
    *valueB = temp;
}

void VM_Dup(ForthVM *vm)
{
    i32 topValue = vm->stack[vm->sp];
    VM_Push(vm, topValue);
}

void VM_Rotate(ForthVM *vm)
{
    i32 *valueA = &vm->stack[vm->sp];
    i32 *valueB = &vm->stack[vm->sp - 1];
    i32 *valueC = &vm->stack[vm->sp - 2];

    i32 temp = *valueB;
    *valueB = *valueA;
    *valueA = *valueC;
    *valueC = temp;
}

void vm__branches_test()
{
    u32 instructions[] = 
    {
        (u32)OPCODE_PUSH, 0,
        (u32)OPCODE_PUSH, 1,
        (u32)OPCODE_EQ,
        (u32)OPCODE_IF_GOTO, 2,
        (u32)OPCODE_GOTO, 2,
        (u32)OPCODE_PUSH, 50,
        (u32)OPCODE_GOTO, 2,
        (u32)OPCODE_PUSH, 100,
        (u32)OPCODE_NOP,
        (u32)OPCODE_GOTO, -1,
    };    

    ForthVM vm = {
        .stack = {0},
        .sp = 0,

        .instructions = instructions,
        .pc = 0,
    };

    while(true) {
        vm.pc += ExecuteByteCode(&vm);
    }
}
void vm__basic_arithmetic_subprogram_test()
{
    u32 instructions[] = 
    {
        (u32)OPCODE_PUSH, 2,
        (u32)OPCODE_PUSH, 2,
        (u32)OPCODE_CALL, 14,
        (u32)OPCODE_PUSH, 3,
        (u32)OPCODE_PUSH, 3,
        (u32)OPCODE_CALL, 14,
        (u32)OPCODE_GOTO, 6,
        (u32)OPCODE_ADD,
        (u32)OPCODE_PUSH, 1,
        (u32)OPCODE_SUB,
        (u32)OPCODE_RET,
        (u32)OPCODE_NOP,
        (u32)OPCODE_GOTO, -1,
    };

    ForthVM vm = {
        .stack = {0},
        .sp = 0,

        .instructions = instructions,
        .pc = 0,
    };

    while(true) {
        vm.pc += ExecuteByteCode(&vm);
    }
}
