#include "Vector.h"
#include <stdlib.h>


Vector *VectorAlloc(VectorElemCpy elem_copy_func, VectorElemCmp elem_cmp_func, VectorElemFree elem_free_func) {
    if(!elem_copy_func || !elem_cmp_func || !elem_free_func){ // check uf the functions are not Null
        return  NULL;
    }
    Vector *new_vector = calloc(1, sizeof(Vector));
    if (!new_vector) {
        return NULL;
    }
    new_vector->data = calloc(VECTOR_INITIAL_CAP, sizeof(void *));
    if (!new_vector->data) {
        return NULL;
    }
    new_vector->capacity = VECTOR_INITIAL_CAP;
    new_vector->size = 0;
    new_vector->elem_copy_func = elem_copy_func;
    new_vector->elem_cmp_func = elem_cmp_func;
    new_vector->elem_free_func = elem_free_func;
    return new_vector;
}

void VectorFree(Vector **p_vector) {
    if (!*p_vector) {
        return;
    }
    for (size_t k_i = 0; k_i < (*p_vector)->size; k_i++) {
        (*p_vector)->elem_free_func(&(*p_vector)->data[k_i]);
        (*p_vector)->data[k_i] = NULL;
    }
    free((*p_vector)->data);
    (*p_vector)->data = NULL;
    free(*p_vector);
    (*p_vector) = NULL;


}
void *VectorAt(Vector *vector, size_t ind) {
    if (!vector || ind >= vector->size || !vector->data ) {
        return NULL;
    }
    int *a = vector->data[ind];
    return a;
}

int VectorFind(Vector *vector, void *value) {
    if (!vector || !value) {
        return -1; // need to think about it
    }
    for (size_t k_i = 0; k_i < vector->size; k_i++) {
        void const *a = vector->data[k_i];
        void const *b = value;
        if (vector->elem_cmp_func(a, b)) {
            return k_i;
        }
    }
    return -1;
}

static void *CheckError(void *ptr, void *return_value) {
    /**
     * check error of the parameters that the function get
     */
    if (!ptr) {
        return return_value;
    }
    return NULL;
}

int VectorIncreaseCapIfNessry(Vector *vector) {
    /**
     * this function check if we need to increase the capacity of the vector depend on the load factor
     * and if it was it change the capacity
     */
    if (VectorGetLoadFactor(vector) >= VECTOR_MAX_LOAD_FACTOR) {
        void **tmp = realloc(vector->data, vector->capacity * VECTOR_GROWTH_FACTOR * sizeof(void *));
        CheckError(tmp, 0);
        vector->data = tmp;
        vector->capacity *= VECTOR_GROWTH_FACTOR;

    }
    return 1;
}

int VectorDecreaseCapIfNessry(Vector *vector) {
    /**
     * this function check if we need to decrease the capacity of the vector depend on the load factor
     * and if it was it change the capacity
     */
    if (VectorGetLoadFactor(vector) != 0 && VectorGetLoadFactor(vector) < VECTOR_MIN_LOAD_FACTOR) {
        void **tmp = realloc(vector->data, (vector->capacity / VECTOR_GROWTH_FACTOR) * sizeof(void *));
        CheckError(tmp, 0);
        vector->data = tmp;
        vector->capacity /= VECTOR_GROWTH_FACTOR;
    }
    return 1;
}


int VectorPushBack(Vector *vector, void *value) {
    CheckError(vector, 0);
    CheckError(value, 0);
    void *cpy_val = vector->elem_copy_func(value);
    vector->data[vector->size++] = cpy_val;
    VectorIncreaseCapIfNessry(vector); // increase the capacity of the vector and change the vector size and the vector cap
    return 1;

}

double VectorGetLoadFactor(Vector *vector) {
    if (!vector) {
        return -1;
    }
    return (double) vector->size / vector->capacity;
}

int VectorErase(Vector *vector, size_t ind) {
    CheckError(vector, 0);
    if (ind >= vector->size) {
        return 0;
    }

    vector->elem_free_func(&vector->data[ind]);
    for (size_t k_i = ind; k_i < vector->size; k_i++) {

        if (k_i != vector->size) { // if the index is not the last value in the vector
            vector->data[k_i] = vector->data[k_i + 1];
        } else {
            vector->data[k_i] = NULL;
        }
    }
    vector->size--;
    VectorDecreaseCapIfNessry(vector);
    return 1;

}
void ChooseCapOfClearVec(Vector *vec, size_t start_size, size_t start_cap) {
    /** this function choose the final capacity of the vector .
     * there are three options to the final capacity depend in what the start size and the start capacity
     * was in the first
     */
    if (start_size == 1 && start_cap == VECTOR_INITIAL_CAP) {
        vec->capacity = VECTOR_INITIAL_CAP/VECTOR_GROWTH_FACTOR;
    } else if (start_size == 2 && start_cap == VECTOR_INITIAL_CAP) {
        vec->capacity = VECTOR_INITIAL_CAP / VECTOR_GROWTH_FACTOR / VECTOR_GROWTH_FACTOR;
    } else {
        vec->capacity = VECTOR_INITIAL_CAP/VECTOR_GROWTH_FACTOR/VECTOR_GROWTH_FACTOR/ \
        VECTOR_GROWTH_FACTOR/VECTOR_GROWTH_FACTOR;
    }
}
void VectorClear(Vector *vector) {
    if (!vector) {
        return;
    }
    size_t start_size = vector->size;
    size_t start_cap = vector->capacity;
    int i = 0;
    while (vector->size){
        vector->elem_free_func(&vector->data[i++]);
        vector->size--;
    }
    ChooseCapOfClearVec(vector,start_size,start_cap);

}
