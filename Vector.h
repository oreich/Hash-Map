#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdlib.h>

/**
 * @def VECTOR_INITIAL_CAP
 * The initial capacity of the vector.
 */
#define VECTOR_INITIAL_CAP 16UL

/**
 * @def VECTOR_GROWTH_FACTOR
 * The growth factor of the vector.
 */
#define VECTOR_GROWTH_FACTOR 2UL

/**
 * @def VECTOR_MAX_LOAD_FACTOR
 * The maximal load factor the vector can be in before
 * extension (vector need to be extended if the load factor is >0.75).
 */
#define VECTOR_MAX_LOAD_FACTOR 0.75

/**
 * @def VECTOR_MIN_LOAD_FACTOR
 * The minimal load factor the vector can be in before
 * size decreasing (vector need to be decreased if the load factor is <0.25).
 */
#define VECTOR_MIN_LOAD_FACTOR 0.25

/**
 * @typedef VectorElemCpy
 * Function which receive an element stored in the vector
 * and returns a dynamically allocated copy of it.
 */
typedef void *(*VectorElemCpy)(const void *);

/**
 * @typedef VectorElemCmp
 * Function which receives two elements stored in the vector
 * and returns 1 if they are equal, 0 otherwise.
 */
typedef int (*VectorElemCmp)(const void *, const void *);

/**
 * @typedef VectorElemFree
 * Function which receives a pointer to element stored in the
 * vector and frees it.
 */
typedef void (*VectorElemFree)(void **);

/**
 * @struct Vector - a generic vector struct.
 * @param capacity - the capacity of the vector.
 * @param size - the current size of the vector.
 * @param data - the values stored inside the vector.
 * @param elem_copy_func - a function which copies (returns
 * a dynamically allocates copy) the elements stored in the vector.
 * @param elem_cmp_func - a function which compares the elements
 * stored in the vector.
 * @param elem_free_func - a function which frees the elements stored
 * in the vector.
 */
typedef struct Vector {
  size_t capacity;
  size_t size;
  void **data;
  VectorElemCpy elem_copy_func;
  VectorElemCmp elem_cmp_func;
  VectorElemFree elem_free_func;
} Vector;

/**
 * Allocates dynamically new vector element.
 * @param elem_copy_func func which copies the element stored in the vector (returns
 * dynamically allocated copy).
 * @param elem_cmp_func func which is used to compare elements stored in the vector.
 * @param elem_free_func func which frees elements stored in the vector.
 * @return pointer to dynamically allocated vector.
 * @if_fail return NULL.
 */
Vector *VectorAlloc(VectorElemCpy elem_copy_func, VectorElemCmp elem_cmp_func, VectorElemFree elem_free_func);

/**
 * Frees a vector and the elements the vector itself allocated.
 * @param p_vector pointer to dynamically allocated pointer to vector.
 */
void VectorFree(Vector **p_vector);

/**
 * Returns the element at the given index.
 * @param vector pointer to a vector.
 * @param ind the index of the element we want to get.
 * @return the element the given index if exists (the element itself, not a copy of it)
 * , NULL otherwise.
 */
void *VectorAt(Vector *vector, size_t ind);

/**
 * Gets a value and checks if the value is in the vector.
 * @param vector a pointer to vector.
 * @param value the value to look for.
 * @return the index of the given value if it is in the
 * vector ([0, vector_size - 1]).
 * Returns -1 if no such value in the vector.
 */
int VectorFind(Vector *vector, void *value);

/**
 * Adds a new value to the back (index vector_size) of the vector.
 * @param vector a pointer to vector.
 * @param value the value to be added to the vector.
 * @return 1 if the adding has been done successfully, 0 otherwise.
 */
int VectorPushBack(Vector *vector, void *value);

/**
 * This function returns the load factor of the vector.
 * @param vector a vector.
 * @return the vector's load factor, -1 if the function failed.
 */
double VectorGetLoadFactor(Vector *vector);

/**
 * Removes the element at the given index from the vector.
 * @param vector a pointer to vector.
 * @param ind the index of the element to be removed.
 * @return 1 if the removing has been done successfully, 0 otherwise.
 */
int VectorErase(Vector *vector, size_t ind);

/**
 * Deletes all the elements in the vector.
 * @param vector vector a pointer to vector.
 */
void VectorClear(Vector *vector);

#endif //VECTOR_H_
