/*
 * slist.h - singly linked list of int.
 *
 * CONCEPT: Every node stores a value and a pointer to the NEXT node. Nodes
 * are scattered in memory, so inserting or removing at the front never
 * shifts anything, but reaching item #k means walking k links.
 *
 *   head                                tail
 *    |                                   |
 *    v                                   v
 *   [10|*]--->[20|*]--->[30|*]--->[40|NULL]
 *
 * This list keeps a `tail` pointer so push_back is O(1) too.
 */
#ifndef SLIST_H
#define SLIST_H

#include <stddef.h>
#include <stdio.h>
#include "dsa_common.h"

typedef struct SNode {
    int value;
    struct SNode *next;
} SNode;

typedef struct {
    SNode *head;
    SNode *tail;
    size_t size;
} SList;

/* Allocate an empty list. Returns NULL if memory ran out. O(1). */
SList *slist_create(void);

/* Free every node and the list itself. NULL is allowed. O(n). */
void slist_destroy(SList *list);

/* Insert at the front. O(1). */
dsa_status slist_push_front(SList *list, int value);

/* Insert at the back (uses the tail pointer). O(1). */
dsa_status slist_push_back(SList *list, int value);

/* Remove the first item into *out (may be NULL). DSA_ERR_EMPTY if empty. O(1). */
dsa_status slist_pop_front(SList *list, int *out);

/* Remove the last item into *out (may be NULL). We must walk to the node
 * BEFORE the tail because nodes only point forward. O(n). */
dsa_status slist_pop_back(SList *list, int *out);

/* Insert so the new item is at `index` (0..size). O(n). */
dsa_status slist_insert_at(SList *list, size_t index, int value);

/* Remove the item at `index` into *out (may be NULL). O(n). */
dsa_status slist_remove_at(SList *list, size_t index, int *out);

/* Remove the first node whose value equals `value`.
 * DSA_ERR_NOT_FOUND if there is none. O(n). */
dsa_status slist_remove_value(SList *list, int value);

/* Read the item at `index` into *out. DSA_ERR_RANGE if out of bounds. O(n). */
dsa_status slist_get(const SList *list, size_t index, int *out);

/* Index of the first match or -1. O(n). */
ptrdiff_t slist_find(const SList *list, int value);

/* Reverse the list in place by re-pointing every `next`. O(n), O(1) memory. */
void slist_reverse(SList *list);

size_t slist_size(const SList *list);     /* O(1) */
int slist_is_empty(const SList *list);    /* O(1) */

/* Print like  10 -> 20 -> 30 -> NULL  followed by a newline. O(n). */
void slist_print(const SList *list, FILE *out);

#endif /* SLIST_H */
