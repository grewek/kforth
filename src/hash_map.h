#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "base_types.h"

#define HASHMAP_DEFAULT_SIZE 1024;
typedef struct HashMap {
    //TODO: In the case where two values want to map into the same bucket we need to have some kind of linked list
    //      so that we can chain another value into this specific index.
    u32 *_addrBook;
    u64 _size;
} HashMap;

HashMap InitializeHashMap();
void InsertKeyValuePair(HashMap *map, char *key, u64 size, u32 value);
u32 GetAddressFromLable(HashMap *map, char *key, u32 size);
void FreeHashMap(HashMap *map);
u64 _GenerateHashValue(char *key, u64 size);

#endif
