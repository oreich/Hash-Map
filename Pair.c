#include "Pair.h"

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
    PairKeyFree key_free, PairValueFree value_free) {
  Pair *pair = malloc(sizeof(Pair));
  pair->key = key_cpy(key);
  pair->value = value_cpy(value);
  pair->key_cpy = key_cpy;
  pair->value_cpy = value_cpy;
  pair->key_cmp = key_cmp;
  pair->value_cmp = value_cmp;
  pair->key_free = key_free;
  pair->value_free = value_free;
  return pair;
}

/**
 * Creates a new (dynamically allocated) copy of the given pair.
 * @param pair pair to be copied.
 * @return new dynamically allocated pair if succeeded, NULL otherwise.
 */
Pair *PairCopy(const Pair *pair) {
  if (!pair) {
    return NULL;
  }
  Pair *new_pair = PairAlloc(pair->key, pair->value,
      pair->key_cpy, pair->value_cpy,
      pair->key_cmp, pair->value_cmp,
      pair->key_free, pair->value_free);
  return new_pair;
}

/**
 * This function frees a pair and everything it allocated dynamically.
 * @param p_pair pointer to dynamically allocated pair to be freed.
 */
void PairFree(Pair **p_pair) {
  if (p_pair && (*p_pair)) {
    (*p_pair)->key_free(&(*p_pair)->key);
    (*p_pair)->value_free(&(*p_pair)->value);
    free((*p_pair));
    *p_pair = NULL;
  }
}
