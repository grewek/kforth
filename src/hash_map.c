#include "hash_map.h"

HashMap InitializeHashMap() 
{
    HashMap result = {0};
    result._size = HASHMAP_DEFAULT_SIZE;
    result._storage = calloc(result._size, sizeof(KeyValuePair));

    if(result._storage == NULL) {
        fprintf(stderr,
            "ERROR: Failed to reserve memory\n");
        exit(1);
    }

    return result;
}

void InsertKeyValuePair(HashMap *map, char *key, u64 size, u32 value) 
{
    u64 index = _GenerateHashValue(key, size) % map->_size;
    map->_storage[index]._key = duplicateString(key, size); 
    map->_storage[index]._addr = value; 
    //map->_addrBook[index] = value;
}

bool ContainsKey(HashMap *map, char *key, u64 size) 
{
    u64 index = _GenerateHashValue(key, size) % map->_size;

    if(map->_storage[index]._key)
    {
        if(strcmp(key, map->_storage[index]._key) == 0)
        {
            return true;
        }

    }

    return false;
}
u32 GetAddressFromLable(HashMap *map, char *key, u32 size) 
{
    u64 index = _GenerateHashValue(key, size) % map->_size;
    return map->_storage[index]._addr;
}

void FreeHashMap(HashMap *map) 
{
    free(map->_storage);
    map->_storage = NULL;
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

char *duplicateString(char *src, u64 size)
{
    char *dest = calloc(size + 1, sizeof(char));
    assert(dest);
    assert(src);
    assert(size > 0);

    for(u64 i = 0; i < size; i++)
    {
        dest[i] = src[i];
    }

    return dest;
}
