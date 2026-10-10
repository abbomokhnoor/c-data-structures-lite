/*
 * vector.h - dynamic array of int (a growable array, like C++ std::vector).
 *
 * CONCEPT: Items live side by side in one block of memory. When the block
 * is full we allocate a bigger one (double the size), copy, and free the
 * old block. Doubling makes push_back O(1) *amortized*.
 *
 *   data -> [ 4 | 8 | 15 | 16 |   |   |   |   ]
 *             0   1   2    3   <- size = 4
 *             capacity = 8 -------------------^
 *
 * USAGE:
 *   Vector v;
 *   vector_init(&v);
 *   vector_push_back(&v, 42);
 *   ...
 *   vector_free(&v);          // always pair init with free
 */
#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include "dsa_common.h"

typedef struct {
    int *data;        /* heap block holding the items (NULL while empty)  */
    size_t size;      /* how many items are stored                        */
    size_t capacity;  /* how many items fit before we must grow           */
} Vector;

/* Prepare an empty vector. No memory is allocated yet. O(1). */
dsa_status vector_init(Vector *v);

/* Release the memory and reset to the empty state. Safe to call twice. O(1). */
void vector_free(Vector *v);

/* Make sure at least `min_capacity` items fit without another reallocation.
 * Never shrinks. O(n) when it has to grow, otherwise O(1). */
dsa_status vector_reserve(Vector *v, size_t min_capacity);

/* Append `value` at the end, growing if needed. Amortized O(1). */
dsa_status vector_push_back(Vector *v, int value);

/* Remove the last item and store it in *out (out may be NULL).
 * DSA_ERR_EMPTY if there is nothing to remove. O(1). */
dsa_status vector_pop_back(Vector *v, int *out);

/* Read the item at `index` into *out. DSA_ERR_RANGE if index >= size. O(1). */
dsa_status vector_get(const Vector *v, size_t index, int *out);

/* Overwrite the item at `index`. DSA_ERR_RANGE if index >= size. O(1). */
dsa_status vector_set(Vector *v, size_t index, int value);

/* Insert `value` so it ends up at `index`, shifting later items right.
 * index may equal size (append). DSA_ERR_RANGE if index > size. O(n). */
dsa_status vector_insert(Vector *v, size_t index, int value);

/* Remove the item at `index`, shifting later items left. The removed value
 * is stored in *out (may be NULL). DSA_ERR_RANGE if index >= size. O(n). */
dsa_status vector_remove(Vector *v, size_t index, int *out);

/* Index of the first item equal to `value`, or -1 if absent. O(n). */
ptrdiff_t vector_find(const Vector *v, int value);

/* Drop all items but keep the allocated memory for reuse. O(1). */
void vector_clear(Vector *v);

/* Give back unused capacity so capacity == size. O(n). */
dsa_status vector_shrink_to_fit(Vector *v);

size_t vector_size(const Vector *v);      /* O(1) */
size_t vector_capacity(const Vector *v);  /* O(1) */
int vector_is_empty(const Vector *v);     /* O(1), 1 = empty */

#endif /* VECTOR_H */
