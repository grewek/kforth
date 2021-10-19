#include "forth_vm_backend.h"

void InsertPushOpcode(Generator *gen, u32 value) 
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_PUSH;
    gen->memory[gen->instructionCount + 1] = value;
    gen->instructionCount += 2;
}

void InsertPopOpcode(Generator *gen)
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_POP;
}

void InsertCallOpcode(Generator *gen, u32 address)
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_CALL;
    gen->memory[gen->instructionCount + 1] = address;
    gen->instructionCount += 2;
}

void InsertRetOpcode(Generator *gen)
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_RET;
    gen->instructionCount += 1;
}

void InsertGotoOpcode(Generator *gen, u32 offset)
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_GOTO;
    gen->memory[gen->instructionCount + 1] = offset;

    gen->instructionCount += 2;
}

void InsertIfGotoOpcode(Generator *gen, u32 offset)
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_IF_GOTO;
    gen->memory[gen->instructionCount + 1] = offset;

    gen->instructionCount += 2;
}

void InsertEqOpcode(Generator *gen)
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_EQ;
    gen->instructionCount += 1;
}

void InsertGtOpcode(Generator *gen)
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_GT;
    gen->instructionCount += 1;
}

void InsertLtOpcode(Generator *gen)
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_LT;
    gen->instructionCount += 1;
}

void InsertAddOpcode(Generator *gen) 
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_ADD;
    gen->instructionCount += 1;
}

void InsertSubOpcode(Generator *gen)
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_SUB;
    gen->instructionCount += 1;
}

void InsertMulOpcode(Generator *gen)
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_MUL;
    gen->instructionCount += 1;
}

void InsertDivOpcode(Generator *gen)
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_DIV;
    gen->instructionCount += 1;
}

void InsertNopOpcode(Generator *gen)
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_NOP;
    gen->instructionCount += 1;
}

void InsertDotOpcode(Generator *gen)
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_DOT;
    gen->instructionCount += 1;
}

void InsertSwapOpcode(Generator *gen)
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_SWAP;
    gen->instructionCount += 1;
}

void InsertDupOpcode(Generator *gen)
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_DUP;
    gen->instructionCount += 1;
}

void InsertRotateOpcode(Generator *gen)
{
    gen->memory[gen->instructionCount] = (u32)OPCODE_ROTATE;
    gen->instructionCount += 1;
}

Generator InitializeGenerator(u32 *memory, u32 maxSize) {
    Generator result = {0};

    result.memory = memory;
    result.maxSize = maxSize;

    return result;
}
u32 *RetrieveByteCode(Generator *gen) {
    u32 *instructions = gen->memory;
    gen->memory = NULL;

    return instructions;
}

void vm__backend_test_opcode_generation()
{
    u32 instructions[32] ={0};
    Generator gen = InitializeGenerator(instructions, 32);

    InsertPushOpcode(&gen, 2);
    InsertPushOpcode(&gen, 2);
    InsertCallOpcode(&gen, 14);
    InsertPushOpcode(&gen, 3);
    InsertPushOpcode(&gen, 3);
    InsertCallOpcode(&gen, 14);
    InsertGotoOpcode(&gen, 6);
    InsertAddOpcode(&gen);
    InsertPushOpcode(&gen, 1);
    InsertSubOpcode(&gen);
    InsertRetOpcode(&gen);
    InsertNopOpcode(&gen);
    InsertGotoOpcode(&gen, -1);

    assert(gen.instructionCount < gen.maxSize);

    assert(gen.memory[0] == (u32)OPCODE_PUSH);
    assert(gen.memory[1] == 2);
    assert(gen.memory[2] == (u32)OPCODE_PUSH);
    assert(gen.memory[3] == 2);
    assert(gen.memory[4] == (u32)OPCODE_CALL);
    assert(gen.memory[5] == 14);
    assert(gen.memory[6] == (u32)OPCODE_PUSH);
    assert(gen.memory[7] == 3);
    assert(gen.memory[8] == (u32)OPCODE_PUSH);
    assert(gen.memory[9] == 3);
    assert(gen.memory[10] == (u32)OPCODE_CALL);
    assert(gen.memory[11] == 14);
    assert(gen.memory[12] == (u32)OPCODE_GOTO);
    assert(gen.memory[13] == 7);
    assert(gen.memory[14] == (u32)OPCODE_ADD);
    assert(gen.memory[15] == (u32)OPCODE_PUSH);
    assert(gen.memory[16] == 1);
    assert(gen.memory[17] == (u32)OPCODE_SUB);
    assert(gen.memory[18] == (u32)OPCODE_RET);
    assert(gen.memory[19] == (u32)OPCODE_NOP);
    assert(gen.memory[20] == (u32)OPCODE_GOTO);
    assert((i32)gen.memory[21] == -1);
}
