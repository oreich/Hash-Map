#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <stdlib.h>
#include "Vector.h"
#include "Pair.h"

/**
 * @def HASH_MAP_INITIAL_CAP
 * The initial capacity of the hash map.
 * It means, the initial number of <b> vectors </b> the hash map has.
 */
#define HASH_MAP_INITIAL_CAP 16UL

/**
 * @def HASH_MAP_GROWTH_FACTOR
 * The growth factor of the hash map.
 * Example: hash_map(size=16),
 *          hash_map.add([16 elements]) ->
 *          hash_map(size=32)
 */
#define HASH_MAP_GROWTH_FACTOR 2UL

/**
 * @def HASH_MAP_MIN_LOAD_FACTOR
 * The minimal load factor the hash map can be in.
 * Example: if the hash_map capacity is 16,
 * and it has 4 elements in it (size is 4),
 * if an element is erased, the load factor drops below 0.25,
 * so the hash map should be minimized (to 8 vectors).
 */
#define HASH_MAP_MIN_LOAD_FACTOR 0.25

/**
 * @def HASH_MAP_MAX_LOAD_FACTOR
 * The maximal load factor the hash map can be in.
 * Example: if the hash_map capacity is 16,
 * and it has 12 elements in it (size is 12),
 * if another element is added, the load factor goes above 0.75,
 * so the hash map should be extended (to 32 vectors).
 */
#define HASH_MAP_MAX_LOAD_FACTOR 0.75

/**
 * @typedef HashFunc
 * This type of function receives a KeyT and returns
 * a representational number of it.
 * Example: lets say we have a pair ('Joe', 78) that we want to store in the hash map,
 * the key is 'Joe' so it determines the bucket in the hash map,
 * his index would be:  size_t ind = HashFunc('Joe') & (capacity - 1);
 */
typedef size_t (*HashFunc)(KeyT);

/**
 * @typedef HashMapPairCpy
 * A copy function for the pairs stored in the vectors (which are stored in the hash map).
 */
typedef void *(*HashMapPairCpy)(const void *);

/**
 * @typedef HashMapPairCmp
 * A compare function for the pairs stored in the vectors (which are stored in the hash map).
 */
typedef int (*HashMapPairCmp)(const void *, const void *);

/**
 * @typedef HashMapPairFree
 * A free function for the pairs stored in vectors (which are stored in the hash map).
 */
typedef void (*HashMapPairFree)(void **);

/**
 * @struct HashMap
 * @param buckets dynamic array of vectors which stores the values.
 * @param size the number of elements (pairs) stored in the hash map.
 * @param capacity the number of buckets in the hash map.
 * @param hash_func a function which "hashes" keys.
 * @param pair_cpy a function which copies pairs.
 * @param pair_cmp a function which compares pairs.
 * @param pair_free a function which frees pairs.
 */
typedef struct HashMap {
  Vector **buckets;
  size_t size;
  size_t capacity; // num of buckets.
  HashFunc hash_func;
  HashMapPairCpy pair_cpy;
  HashMapPairCmp pair_cmp;
  HashMapPairFree pair_free;
} HashMap;

/**
 * Allocates dynamically new hash map element.
 * @param hash_func a function which "hashes" keys.
 * @param pair_cpy a function which copies pairs.
 * @param pair_cmp a function which compares pairs.
 * @param pair_free a function which frees pairs.
 * @return pointer to dynamically allocated HashMap.
 * @if_fail return NULL.
 */
HashMap *HashMapAlloc(
    HashFunc hash_func, HashMapPairCpy pair_cpy,
    HashMapPairCmp pair_cmp, HashMapPairFree pair_free);

/**
 * Frees a vector and the elements the vector itself allocated.
 * @param p_hash_map pointer to dynamically allocated pointer to hash_map.
 */
void HashMapFree(HashMap **p_hash_map);

/**
 * Inserts a new pair to the hash map.
 * The function inserts *new*, *copied*, *dynamically allocated* pair,
 * NOT the pair it receives as a parameter.
 * @param hash_map the hash map to be inserted with new element.
 * @param pair a pair the hash map would contain.
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int HashMapInsert(HashMap *hash_map, Pair *pair);

/**
 * The function checks if the given key exists in the hash map.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return 1 if the key is in the hash map, 0 otherwise.
 */
int HashMapContainsKey(HashMap *hash_map, KeyT key);

/**
 * The function checks if the given value exists in the hash map.
 * @param hash_map a hash map.
 * @param value the value to be checked.
 * @return 1 if the value is in the hash map, 0 otherwise.
 */
int HashMapContainsValue(HashMap *hash_map, ValueT value);

/**
 * The function returns the value associated with the given key.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return the value associated with key if exists, NULL otherwise.
 */
ValueT HashMapAt(HashMap *hash_map, KeyT key);

/**
 * The function erases the pair associated with key.
 * @param hash_map a hash map.
 * @param key a key of the pair to be erased.
 * @return 1 if the erasing was done successfully, 0 otherwise.
 */
int HashMapErase(HashMap *hash_map, KeyT key);

/**
 * This function returns the load factor of the hash map.
 * @param hash_map a hash map.
 * @return the hash map's load factor, -1 if the function failed.
 */
double HashMapGetLoadFactor(HashMap *hash_map);

/**
 * This function deletes all the elements in the hash map.
 * @param hash_map a hash map to be cleared.
 */
void HashMapClear(HashMap *hash_map);

#endif //HASHMAP_H_
