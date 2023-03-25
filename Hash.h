#ifndef HASH_H_
#define HASH_H_

#include <stdlib.h>

/**
 * Integers simple hash func.
 */
size_t HashInt(void *elem){
  size_t hash = (*((int *) elem));
  return hash;
}

/**
 * Chars simple hash func.
 */
size_t HashChar(void *elem){
  size_t hash = (*((char *) elem));
  return hash;
}

/**
 * Doubles simple hash func.
 */
size_t HashDouble(void *elem){
  size_t hash = (*((double *) elem));
  return hash;
}

#endif // HASH_H_
