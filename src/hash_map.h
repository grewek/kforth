#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "base_types.h"
#include "forth_function.h"

#define HASHMAP_DEFAULT_SIZE 1024;
typedef struct HashMap {
    //TODO: In the case where two values want to map into the same bucket we need to have some kind of linked list
    //      so that we can chain another value into this specific index.
    ForthFunction *_functions;
    u64 size;
} HashMap;

HashMap InitializeHashMap();
ForthFunction *EmptyFunctionWithKey(HashMap *map, char *key, u64 size);
void InsertKeyValuePair(HashMap *map, char *key, u64 size, ForthFunction *value);
ForthFunction *GetValue(HashMap *map, char *key, u64 size);
void FreeHashMap(HashMap *map);
u64 _GenerateHashValue(char *key, u64 size);

#endif
