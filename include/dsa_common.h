/*
 * dsa_common.h - shared status codes and helpers used by every module.
 *
 * WHY: Every function that can fail returns a dsa_status instead of
 * crashing or silently doing nothing. DSA_OK (0) always means success, so
 * you can write:   if (vector_push_back(&v, 5) != DSA_OK) { ... }
 * and turn any code into readable text with dsa_strerror().
 */
#ifndef DSA_COMMON_H
#define DSA_COMMON_H

#include <stddef.h>

typedef enum {
    DSA_OK = 0,           /* success                                        */
    DSA_ERR_NOMEM,        /* malloc/realloc returned NULL                   */
    DSA_ERR_INVALID,      /* bad argument, e.g. a NULL pointer              */
    DSA_ERR_RANGE,        /* index / value outside the allowed range        */
    DSA_ERR_EMPTY,        /* operation needs at least one element           */
    DSA_ERR_FULL,         /* fixed-capacity container has no room left      */
    DSA_ERR_NOT_FOUND,    /* the requested key / value is not present       */
    DSA_ERR_DUPLICATE,    /* key already exists and duplicates are rejected */
    DSA_ERR_CYCLE         /* a graph cycle makes the request impossible     */
} dsa_status;

/* Returns a short, human-readable description of a status code.
 * Complexity: O(1). The returned string is static - never free it. */
const char *dsa_strerror(dsa_status status);

/* Portable replacement for strdup() (which is POSIX, not standard C11).
 * Returns a malloc'd copy of s, or NULL if s is NULL or memory ran out.
 * The caller must free() the result.
 * Complexity: O(n) for a string of length n. */
char *dsa_strdup(const char *s);

#endif /* DSA_COMMON_H */
