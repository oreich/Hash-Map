#include "HashMap.h"


HashMap *HashMapAlloc(
        HashFunc hash_func, HashMapPairCpy pair_cpy,
        HashMapPairCmp pair_cmp, HashMapPairFree pair_free) {
    if (!hash_func || !pair_cpy || !pair_cmp || !pair_free) { // check if the functions are not null,
        return NULL;
    }
    HashMap *new_hash = calloc(1, sizeof(HashMap));
    if (!new_hash ) {
        return NULL;
    }
    new_hash->buckets = calloc(HASH_MAP_INITIAL_CAP, sizeof(Vector ));
    if (!new_hash->buckets) {
        return NULL;
    }
    new_hash->capacity = HASH_MAP_INITIAL_CAP;
    new_hash->hash_func = hash_func;
    new_hash->pair_cpy = pair_cpy;
    new_hash->pair_cmp = pair_cmp;
    new_hash->pair_free = pair_free;
    for (unsigned long i = 0; i < HASH_MAP_INITIAL_CAP; ++i) {
        new_hash->buckets[i] = VectorAlloc(new_hash->pair_cpy, new_hash->pair_cmp, new_hash->pair_free);
    }
    return new_hash;
}

void HashMapFree(HashMap **p_hash_map) {
    if (!*p_hash_map) {
        return;
    }
    HashMap *map = (*(p_hash_map));
    for (size_t k_i = 0; k_i < map->capacity; k_i++) {
        VectorFree(&map->buckets[k_i]);
    }
    free(map->buckets);
    map->buckets = NULL;
    free(*p_hash_map);
    *p_hash_map = NULL;
}

void CreatArrOfVector(HashMap *hash_map, Pair *arr[]) {
    /**
     * get the hash map and empty array and insert all the pairs that was in the hash map into the array
     */
    int k = 0;
    for (size_t j = 0; j < hash_map->capacity; ++j) {
        if (hash_map->buckets[j]) {
            for (size_t i = 0; i < hash_map->buckets[j]->size; ++i) {
                Pair *pair = VectorAt(hash_map->buckets[j], i);
                arr[k++] = hash_map->buckets[j]->elem_copy_func(
                        pair);

            }
        }
    }
}

void CheckCapAndIncreaseIfNessr(HashMap *hash_map) {
    /**
     * check if we need to increase the capacity of the hash map depend on the load factor .
     * and if we need to change the capacity we rehash the hash map : we creat buffer - an array - and put all
     * the elements of the hash map into it .
     * after that we change the capacity and insert all the elements to th hash map
     */
    double loaf = HashMapGetLoadFactor(hash_map);
    if (loaf != -1 && loaf > HASH_MAP_MAX_LOAD_FACTOR) {
        size_t arr_size = hash_map->size;
        size_t num_of_vectors = hash_map->capacity;
        Pair** arr = malloc(hash_map->size*sizeof(Pair*));
        CreatArrOfVector(hash_map, arr); // i save all the pairs in an array for the rehashing
        size_t new_cap = hash_map->capacity * HASH_MAP_GROWTH_FACTOR;
        HashMapClear(hash_map);
        for (size_t i = 0; i < num_of_vectors ; ++i) {
            VectorFree(&hash_map->buckets[i]);
        }
        hash_map->capacity = new_cap;
        Vector **tmp = realloc(hash_map->buckets, new_cap * sizeof(Vector ));
        if (tmp) {
            hash_map->buckets = tmp;
        }
        for (size_t i = 0; i < new_cap ; ++i) {
            hash_map->buckets[i] = VectorAlloc(hash_map->pair_cpy, hash_map->pair_cmp, hash_map->pair_free);
        }
        for (size_t i = 0; i < arr_size; ++i) {
            HashMapInsert(hash_map, arr[i]);
        }
        for (size_t i = 0; i < arr_size; ++i) {
            hash_map->pair_free((void**)&arr[i]);
        }
        free(arr);
    }
}

void VectorFindByKey(Vector *vec, Pair *pair) {
    /**
     *     find the vector that the current key is in it and change the old pair to the new pair
     */
    for (size_t i = 0; i < vec->size; ++i) {
        Pair *old_pair = (Pair *) vec->data[i];
        if (pair->key_cmp(pair->key, old_pair->key)) {
            vec->elem_free_func((void**)&old_pair);
            vec->data[i] = pair;

        }
    }
}

void FindVectorAndChangePair(HashMap *hash_map, Pair *pair) {
    for (size_t i = 0; i < hash_map->capacity; ++i) {
        Vector * vec = hash_map->buckets[i];
        if (vec && vec->size) { // the currnet bucket is with vector of at list one pair
            VectorFindByKey(vec, pair);
        }
    }
}

int HashMapInsert(HashMap *hash_map, Pair *pair) {
    if (hash_map && pair) {
        Pair *new_pair = hash_map->pair_cpy(pair);
        if (HashMapContainsKey(hash_map,
                               new_pair->key)) {
            /** if the key of the new pair is in the hash map
            * we need to find the vector that the new key is in it
            * and change the old pair to the new pair
            * we dont need to check the capacity because the size still the same
            */
            FindVectorAndChangePair(hash_map, new_pair);
            return 1;
        }
        size_t hash_ind = hash_map->hash_func(new_pair->key) & (hash_map->capacity - 1);
        if (!hash_map->buckets[hash_ind]->size ) {
            Vector * vec = hash_map->buckets[hash_ind];
            VectorPushBack(vec, new_pair);
            hash_map->pair_free((void**)(&new_pair));
            hash_map->buckets[hash_ind] = vec;
            hash_map->size++;
            CheckCapAndIncreaseIfNessr(hash_map);
            return 1;
        } else { // if there is at list one pair in the vector
            Vector *cur_vec = hash_map->buckets[hash_ind];
            VectorPushBack(cur_vec, new_pair);
            hash_map->pair_free((void**)(&new_pair));
            hash_map->size++;
            CheckCapAndIncreaseIfNessr(hash_map);
            return 1;
        }


    }

    return 0;
}

int HashMapContainsKey(HashMap *hash_map, KeyT key) {
    if (!hash_map) {
        return 0;
    }

    for (size_t i = 0; i < hash_map->capacity  ; ++i) {
        if (hash_map->buckets[i]) {
            Vector *vec = hash_map->buckets[i];
            for (size_t j = 0; j < vec->size; ++j) {
                Pair *cur_pair = (Pair *) vec->data[j];
                KeyT cur_key = cur_pair->key;
                if (cur_pair->key_cmp(cur_key, key)) {
                    return 1;
                }
            }
        }
    }
    return 0;

}

int HashMapContainsValue(HashMap *hash_map, ValueT value) {
    if (!hash_map) {
        return 0;
    }
    for (size_t i = 0; i < hash_map->capacity; ++i) {
        if (hash_map->buckets[i]) { // if there is vector in the the bucket (it means that there is at list one pair
            Vector *vec = hash_map->buckets[i];
            for (size_t j = 0; j < vec->size; ++j) {
                Pair *cur_pair = (Pair *) vec->data[j];
                ValueT cur_val = cur_pair->value;
                if (cur_pair->value_cmp(cur_val, value)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

ValueT HashMapAt(HashMap *hash_map, KeyT key) {
    if (!hash_map) {
        return NULL;
    }
    for (size_t i = 0; i < hash_map->capacity; ++i) {
        if (hash_map->buckets[i]) {
            Vector *vec = hash_map->buckets[i];
            for (size_t j = 0; j < vec->size; ++j) {
                Pair *pair = vec->data[j];
                if (pair->key_cmp(pair->key, key)) {
                    return pair->value;
                }
            }
        }
    }
    return NULL;
}


void CheckCapAndDecreaseIfNessr(HashMap *hash_map) {
    /** check if the current min load factor is lower then the MIN_LOAD_FACTOR and if it is the function send the
    * hash map and an empty array and fil in the array with all the pairs that was in the hash map.
    * after that it realloc the buckets of the hash map , clear the hash map ,and rehash the pairs into the the clear
    * hash map.
     */
    double loaf = HashMapGetLoadFactor(hash_map);
    if (loaf != -1 && loaf < HASH_MAP_MIN_LOAD_FACTOR) {
        size_t arr_size = hash_map->size;
        Pair** arr = malloc(hash_map->size*sizeof(Pair*));
        CreatArrOfVector(hash_map, arr);          // save all the pairs in an array for the rehashing.
        size_t new_cap = hash_map->capacity / HASH_MAP_GROWTH_FACTOR;
        for (size_t i = 0; i < hash_map->capacity; ++i) {
            VectorFree(&hash_map->buckets[i]);
        }
        HashMapClear(hash_map);  // clear the hash map before insert  the pairs into it
        hash_map->capacity = new_cap;
        Vector **tmp = realloc(hash_map->buckets, new_cap * sizeof(Vector *));
        if (tmp) {
            hash_map->buckets = tmp;
        }
        for (size_t i = 0; i < new_cap ; ++i) { // insert vector to all the buckets
            hash_map->buckets[i] = VectorAlloc(hash_map->pair_cpy, hash_map->pair_cmp, hash_map->pair_free);
        }
        for (size_t i = 0; i < arr_size; ++i) { // insert the pairs into the hashmap
            HashMapInsert(hash_map, arr[i]);
        }
        for (size_t i = 0; i < arr_size; ++i) {
            hash_map->pair_free((void**)&arr[i]);
        }
        free(arr);
    }
}

int HashMapErase(HashMap *hash_map, KeyT key) {
    if (!hash_map) {
        return 0;
    }
    if (!HashMapContainsKey(hash_map, key)) {
        return 0;
    }
    for (size_t i = 0; i < hash_map->capacity; ++i) {
        Vector *vec = hash_map->buckets[i];
        if (vec) {
            for (size_t j = 0; j < vec->size; ++j) {
                Pair *pair = vec->data[j];
                if (pair->key_cmp(key, pair->key)) {
                    int ind = VectorFind(vec, pair);
                    if (ind != -1) {
                        VectorErase(vec, ind);
                        hash_map->size--;
                        if (!vec->data[j]) {// the bucket is now empty and dont contain any pairs
                            CheckCapAndDecreaseIfNessr(hash_map);
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

double HashMapGetLoadFactor(HashMap *hash_map) {
    if (hash_map) {
        double size = hash_map->size;
        double cap = hash_map->capacity;
        return size / cap;
    }
    return -1;
}

void ChooseCapOfClearMap(HashMap *hash_map, size_t start_size, size_t start_cap) {
    /** this function choose the final capacity of the hash map .
   * there are three options to the final capacity depend in what the start size and the start capacity
   * was in the first
     *
   */
    if (start_size == 1 && start_cap == HASH_MAP_INITIAL_CAP) {
        hash_map->capacity = HASH_MAP_INITIAL_CAP/HASH_MAP_GROWTH_FACTOR;
    } else if (start_size == 2 && start_cap == HASH_MAP_INITIAL_CAP) {
        hash_map->capacity = HASH_MAP_INITIAL_CAP / HASH_MAP_GROWTH_FACTOR / HASH_MAP_GROWTH_FACTOR;
    } else {
        hash_map->capacity = HASH_MAP_INITIAL_CAP/HASH_MAP_GROWTH_FACTOR/HASH_MAP_GROWTH_FACTOR/HASH_MAP_GROWTH_FACTOR;
    }
}

void HashMapClear(HashMap *hash_map) {
    if (!hash_map) {
        return;
    }
    size_t start_size = hash_map->size;
    size_t start_cap = hash_map->capacity;
    for (size_t i = 0; i < hash_map->capacity ; ++i) {
        if (hash_map->buckets[i]) {
            Vector *vec = hash_map->buckets[i];
            VectorClear(vec);
        }
    }
    ChooseCapOfClearMap(hash_map, start_size, start_cap);
    for (size_t i = hash_map->capacity ; i < start_cap; ++i) {
        VectorFree(&hash_map->buckets[i]);
    }
    hash_map->size = 0;
}
