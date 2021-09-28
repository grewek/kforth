#include "hash_map.h"

HashMap InitializeHashMap() {
    HashMap result = {0};
    result.size = HASHMAP_DEFAULT_SIZE;
    result._functions = calloc(result.size, sizeof(ForthFunction));

    if(result._functions == NULL) {
        fprintf(stderr, "ERROR: Failed to reserve memory for the Function Hashmap.\n");
        exit(1);
    }

    return result;
}

ForthFunction *EmptyFunctionWithKey(HashMap *map, char *key, u64 size) {    
    u64 index = _GenerateHashValue(key, size) % map->size;
    ForthFunction emptyFunction = InitializeFunction(128);
    memcpy(map->_functions + index, &emptyFunction, sizeof(ForthFunction));

    return map->_functions + index;
}

void InsertKeyValuePair(HashMap *map, char *key, u64 size, ForthFunction *value) {
    
    u64 index = _GenerateHashValue(key, size) % map->size;

    memcpy(map->_functions + index, value, sizeof(ForthFunction));
}

ForthFunction *GetValue(HashMap *map, char *key, u64 size) {
    u64 index = _GenerateHashValue(key, size) % map->size;

    return &map->_functions[index];
}

void FreeHashMap(HashMap *map) {
    for(u64 i = 0; i < map->size; i++) {
        FreeFunction(&map->_functions[i]);
    }

    free(map->_functions);
    map->_functions = NULL;
}

u64 _GenerateHashValue(char *key, u64 size) {
    const u64 prime = 31;
    u64 hashValue = 0;
    for(u64 i = 0; i < size; i++) {
        hashValue = (prime * hashValue + key[i]);
    }

    return hashValue;
}