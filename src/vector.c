#include "vector.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_MIN_CAPACITY 4

dsa_status vector_init(Vector *v)
{
    if (v == NULL) {
        return DSA_ERR_INVALID;
    }
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
    return DSA_OK;
}

void vector_free(Vector *v)
{
    if (v == NULL) {
        return;
    }
    free(v->data);          /* free(NULL) is allowed and does nothing */
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

dsa_status vector_reserve(Vector *v, size_t min_capacity)
{
    if (v == NULL) {
        return DSA_ERR_INVALID;
    }
    if (min_capacity <= v->capacity) {
        return DSA_OK;                       /* already big enough */
    }
    if (min_capacity > SIZE_MAX / sizeof(int)) {
        return DSA_ERR_NOMEM;                /* size computation would overflow */
    }
    /* Use a temporary pointer: if realloc fails it returns NULL but the old
     * block is still valid. Writing straight into v->data would leak it. */
    int *bigger = realloc(v->data, min_capacity * sizeof(int));
    if (bigger == NULL) {
        return DSA_ERR_NOMEM;
    }
    v->data = bigger;
    v->capacity = min_capacity;
    return DSA_OK;
}

dsa_status vector_push_back(Vector *v, int value)
{
    if (v == NULL) {
        return DSA_ERR_INVALID;
    }
    if (v->size == v->capacity) {
        /* Grow geometrically (x2). Growing by a fixed amount would make a
         * long series of pushes O(n^2) instead of O(n). */
        size_t wanted = v->capacity == 0 ? VECTOR_MIN_CAPACITY : v->capacity * 2;
        dsa_status st = vector_reserve(v, wanted);
        if (st != DSA_OK) {
            return st;
        }
    }
    v->data[v->size++] = value;
    return DSA_OK;
}

dsa_status vector_pop_back(Vector *v, int *out)
{
    if (v == NULL) {
        return DSA_ERR_INVALID;
    }
    if (v->size == 0) {
        return DSA_ERR_EMPTY;
    }
    v->size--;
    if (out != NULL) {
        *out = v->data[v->size];
    }
    return DSA_OK;
}

dsa_status vector_get(const Vector *v, size_t index, int *out)
{
    if (v == NULL || out == NULL) {
        return DSA_ERR_INVALID;
    }
    if (index >= v->size) {
        return DSA_ERR_RANGE;
    }
    *out = v->data[index];
    return DSA_OK;
}

dsa_status vector_set(Vector *v, size_t index, int value)
{
    if (v == NULL) {
        return DSA_ERR_INVALID;
    }
    if (index >= v->size) {
        return DSA_ERR_RANGE;
    }
    v->data[index] = value;
    return DSA_OK;
}

dsa_status vector_insert(Vector *v, size_t index, int value)
{
    if (v == NULL) {
        return DSA_ERR_INVALID;
    }
    if (index > v->size) {
        return DSA_ERR_RANGE;
    }
    if (v->size == v->capacity) {
        size_t wanted = v->capacity == 0 ? VECTOR_MIN_CAPACITY : v->capacity * 2;
        dsa_status st = vector_reserve(v, wanted);
        if (st != DSA_OK) {
            return st;
        }
    }
    /* memmove (not memcpy!) because source and destination overlap. */
    memmove(&v->data[index + 1], &v->data[index], (v->size - index) * sizeof(int));
    v->data[index] = value;
    v->size++;
    return DSA_OK;
}

dsa_status vector_remove(Vector *v, size_t index, int *out)
{
    if (v == NULL) {
        return DSA_ERR_INVALID;
    }
    if (index >= v->size) {
        return DSA_ERR_RANGE;
    }
    if (out != NULL) {
        *out = v->data[index];
    }
    memmove(&v->data[index], &v->data[index + 1], (v->size - index - 1) * sizeof(int));
    v->size--;
    return DSA_OK;
}

ptrdiff_t vector_find(const Vector *v, int value)
{
    if (v == NULL) {
        return -1;
    }
    for (size_t i = 0; i < v->size; i++) {
        if (v->data[i] == value) {
            return (ptrdiff_t)i;
        }
    }
    return -1;
}

void vector_clear(Vector *v)
{
    if (v != NULL) {
        v->size = 0;
    }
}

dsa_status vector_shrink_to_fit(Vector *v)
{
    if (v == NULL) {
        return DSA_ERR_INVALID;
    }
    if (v->size == v->capacity) {
        return DSA_OK;
    }
    if (v->size == 0) {
        free(v->data);
        v->data = NULL;
        v->capacity = 0;
        return DSA_OK;
    }
    int *smaller = realloc(v->data, v->size * sizeof(int));
    if (smaller == NULL) {
        return DSA_ERR_NOMEM;   /* old block is still valid and usable */
    }
    v->data = smaller;
    v->capacity = v->size;
    return DSA_OK;
}

size_t vector_size(const Vector *v)
{
    return v == NULL ? 0 : v->size;
}

size_t vector_capacity(const Vector *v)
{
    return v == NULL ? 0 : v->capacity;
}

int vector_is_empty(const Vector *v)
{
    return v == NULL || v->size == 0;
}
