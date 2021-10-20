#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "base_types.h"

#define HASHMAP_DEFAULT_SIZE 1024;
typedef struct KeyValuePair {
    char *_key;
    u32 _addr;
    
} KeyValuePair;
typedef struct HashMap {
    //TODO: Linkedlist storage for multiple values with the same key value.
    KeyValuePair *_storage;
    u64 _size;
} HashMap;

HashMap InitializeHashMap();
void InsertKeyValuePair(HashMap *map, char *key, u64 size, u32 value);
bool ContainsKey(HashMap *map, char *key, u64 size);
u32 GetAddressFromLable(HashMap *map, char *key, u32 size);
void FreeHashMap(HashMap *map);
u64 _GenerateHashValue(char *key, u64 size);

char *duplicateString(char *src, u64 size);
#endif
