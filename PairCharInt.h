/**
 * Pairs like { char: int }
 * The key type is char *.
 * The value type is int *.
 */

#ifndef PAIRCHARINT_H_
#define PAIRCHARINT_H_

#include <stdlib.h>
#include "Pair.h"

/**
 * Copies the char key of the pair.
 */
void *CharKeyCpy(void *key) {
  char *new_char = malloc(sizeof(char));
  *new_char = *((char *) key);
  return new_char;
}

/**
 * Copies the int value of the pair.
 */
void *IntValueCpy(void *value) {
  int *new_int = malloc(sizeof(int));
  *new_int = *((char *) value);
  return new_int;
}

/**
 * Compares the char key of the pair.
 */
int CharKeyCmp(void *val_1, void *val_2) {
  return *(char *) val_1 == *(char *) val_2;
};

/**
 * Compares the int value of the pair.
 */
int IntValueCmp(void *val_1, void *val_2) {
    return *(int *) val_1 == *(int *) val_2;
};

/**
 * Frees the char key of the pair.
 */
void CharKeyFree(void **val) {
  if (val && *val) {
    free(*val);
    *val = NULL;
  }
}

/**
 * Frees the int value of the pair.
 */
void IntValueFree(void **val) {
  if (val && *val) {
    free(*val);
    *val = NULL;
  }
}

/**
 * Copy func for the pair.
 */
void *PairCharIntCpy(const void *p) {
  const Pair *pair = (const Pair *) p;
  Pair *new_pair = PairCopy(pair);
  return new_pair;
}

/**
 * Compare func for the pair.
 */
int PairCharIntCmp(const void *p_1, const void *p_2) {
  if (!p_1 || !p_2) {
    return 0;
  }

  const Pair *pair_1 = (const Pair *) p_1;
  const Pair *pair_2 = (const Pair *) p_2;

  int key_cmp = pair_1->key_cmp(pair_1->key, pair_2->key);
  int val_cmp = pair_1->value_cmp(pair_1->value, pair_2->value);
  return key_cmp && val_cmp;
}

/**
 * Free func for the pair.
 */
void PairCharIntFree(void **p_p) {
  if (!p_p || !(*p_p)) {
    return;
  }

  Pair **p_pair = (Pair **) p_p;
  (*p_pair)->key_free(&(*p_pair)->key);
  (*p_pair)->value_free(&(*p_pair)->value);
  free(*p_pair);
  *p_pair = NULL;
}

#endif //PAIRCHARINT_H_
