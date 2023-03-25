#ifndef PAIR_H_
#define PAIR_H_

#include <stdlib.h>

/**
 * @typedef KeyT, ValueT
 * typedef for the key and value elements in the pair.
 */
typedef void *KeyT;
typedef void *ValueT;

/**
 * @typedef PairKeyCpy, PairValueCpy
 * Copy (dynamically) functions for the key and the value.
 */
typedef KeyT (*PairKeyCpy)(KeyT);
typedef ValueT (*PairValueCpy)(ValueT);

/**
 * @typedef PairKeyCmp, PairValueCmp
 * Compare functions for the key and the value.
 */
typedef int (*PairKeyCmp)(KeyT , KeyT);
typedef int (*PairValueCmp)(ValueT , ValueT);

/**
 * @typedef PairKeyFree, PairValueFree
 * Free functions for the key and the value.
 */
typedef void (*PairKeyFree)(KeyT *);
typedef void (*PairValueFree)(ValueT *);

/**
 * @struct Pair - represent a pair '''{key: value}'''.
 * @param key, value - the key and value.
 * @param key_cpy, value_cpy - copy functions for key and value.
 * @param key_cmp, value_cmp - compare functions for key and value.
 * @param key_free, value_free - free functions for key and value.
 */
typedef struct Pair {
  KeyT key;
  ValueT value;
  PairKeyCpy key_cpy;
  PairValueCpy value_cpy;
  PairKeyCmp key_cmp;
  PairValueCmp value_cmp;
  PairKeyFree key_free;
  PairValueFree value_free;
} Pair;

/**
 * Allocates dynamically a new pair.
 * @param key, value - the key and value.
 * @param key_cpy, value_cpy - copy functions for key and value.
 * @param key_cmp, value_cmp - compare functions for key and value.
 * @param key_free, value_free - free functions for key and value.
 * @return dynamically allocated pair.
 */
Pair *PairAlloc(
    KeyT key, ValueT value,
    PairKeyCpy key_cpy, PairValueCpy value_cpy,
    PairKeyCmp key_cmp, PairValueCmp value_cmp,
    PairKeyFree key_free, PairValueFree value_free);

/**
 * Creates a new (dynamically allocated) copy of the given pair.
 * @param pair pair to be copied.
 * @return new dynamically allocated pair if succeeded, NULL otherwise.
 */
Pair *PairCopy(const Pair *pair);

/**
 * This function frees a pair and everything it allocated dynamically.
 * @param p_pair pointer to dynamically allocated pair to be freed.
 */
void PairFree(Pair **p_pair);

#endif //PAIR_H_
