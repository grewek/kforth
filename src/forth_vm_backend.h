#ifndef _FORTH_VM_BACKEND_H_
#define _FORTH_VM_BACKEND_H_

#include <assert.h>

#include "base_types.h"
#include "forth_vm.h"

typedef struct Generator {
    u32 *memory;
    u32 instructionCount;
    u32 maxSize;
    u32 entryPoint;
} Generator;

Generator InitializeGenerator(u32 *memory, u32 maxSize);
u32 *RetrieveByteCode(Generator *gen);

void vm__backend_test_opcode_generation();

void InsertPushOpcode(Generator *gen, u32 value);
void InsertCallOpcode(Generator *gen, u32 address);
void InsertRetOpcode(Generator *gen);

void InsertGotoOpcode(Generator *gen, u32 offset);
void InsertIfGotoOpcode(Generator *gen, u32 offset);

void InsertEqOpcode(Generator *gen);
void InsertGtOpcode(Generator *gen);
void InsertLtOpcode(Generator *gen);

void InsertAddOpcode(Generator *gen);
void InsertSubOpcode(Generator *gen);
void InsertMulOpcode(Generator *gen);
void InsertDivOpcode(Generator *gen);
void InsertNopOpcode(Generator *gen);

void InsertDotOpcode(Generator *gen);
void InsertSwapOpcode(Generator *gen);
void InsertDupOpcode(Generator *gen);
void InsertRotateOpcode(Generator *gen);

#endif
