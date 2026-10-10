#include "dsa_common.h"

#include <stdlib.h>
#include <string.h>

const char *dsa_strerror(dsa_status status)
{
    switch (status) {
    case DSA_OK:            return "success";
    case DSA_ERR_NOMEM:     return "out of memory";
    case DSA_ERR_INVALID:   return "invalid argument";
    case DSA_ERR_RANGE:     return "index or value out of range";
    case DSA_ERR_EMPTY:     return "container is empty";
    case DSA_ERR_FULL:      return "container is full";
    case DSA_ERR_NOT_FOUND: return "not found";
    case DSA_ERR_DUPLICATE: return "duplicate key";
    case DSA_ERR_CYCLE:     return "graph contains a cycle";
    }
    return "unknown status";
}

char *dsa_strdup(const char *s)
{
    if (s == NULL) {
        return NULL;
    }
    size_t len = strlen(s) + 1;          /* +1 for the '\0' terminator */
    char *copy = malloc(len);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, s, len);
    return copy;
}
