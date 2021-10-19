#include "hash_map.h"

HashMap InitializeHashMap() 
{
    HashMap result = {0};
    result._size = HASHMAP_DEFAULT_SIZE;
    result._addrBook = calloc(result._size, sizeof(u32));

    if(result._addrBook == NULL) {
        fprintf(stderr, 
            "ERROR: Failed to reserve memory for the Function Hashmap.\n");
        exit(1);
    }

    return result;
}

void InsertKeyValuePair(HashMap *map, char *key, u64 size, u32 value) 
{
    
    u64 index = _GenerateHashValue(key, size) % map->_size;
    map->_addrBook[index] = value;
}

u32 GetAddressFromLable(HashMap *map, char *key, u32 size) 
{
    u64 index = _GenerateHashValue(key, size) % map->_size;

    return map->_addrBook[index];
}

void FreeHashMap(HashMap *map) 
{
    free(map->_addrBook);
    map->_addrBook = NULL;
}

u64 _GenerateHashValue(char *key, u64 size) 
{
    const u64 prime = 31;
    u64 hashValue = 0;
    for(u64 i = 0; i < size; i++) {
        hashValue = (prime * hashValue + key[i]);
    }

    return hashValue;
}
