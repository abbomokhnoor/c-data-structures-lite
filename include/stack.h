/*
 * stack.h - LIFO stack of int, in two flavours.
 *
 * CONCEPT: "Last In, First Out" - like a pile of plates. You can only
 * touch the top.
 *
 *   push(3)  push(7)  push(9)        pop() -> 9
 *                      | 9 | <- top           | 7 | <- top
 *             | 7 |    | 7 |                  | 3 |
 *   | 3 |     | 3 |    | 3 |                  +---+
 *   +---+     +---+    +---+
 *
 * ArrayStack  - items in a growable array; top is the last slot.
 * LinkedStack - items in linked nodes; top is the head node.
 * Both give O(1) push/pop. Pick the array for speed and cache-friendliness,
 * the list when you never want a big reallocation pause.
 */
#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include "dsa_common.h"

/* ------------------------------------------------------------ ArrayStack */

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} ArrayStack;

/* Allocate a stack. initial_capacity 0 uses a small default. NULL on failure. */
ArrayStack *astack_create(size_t initial_capacity);   /* O(1) */
void astack_destroy(ArrayStack *s);                   /* O(1) */
dsa_status astack_push(ArrayStack *s, int value);     /* amortized O(1) */
dsa_status astack_pop(ArrayStack *s, int *out);       /* O(1), DSA_ERR_EMPTY */
dsa_status astack_peek(const ArrayStack *s, int *out);/* O(1), DSA_ERR_EMPTY */
size_t astack_size(const ArrayStack *s);              /* O(1) */
int astack_is_empty(const ArrayStack *s);             /* O(1) */

/* ----------------------------------------------------------- LinkedStack */

typedef struct SNodeStack {
    int value;
    struct SNodeStack *next;
} StackNode;

typedef struct {
    StackNode *top;
    size_t size;
} LinkedStack;

LinkedStack *lstack_create(void);                     /* O(1) */
void lstack_destroy(LinkedStack *s);                  /* O(n) */
dsa_status lstack_push(LinkedStack *s, int value);    /* O(1) */
dsa_status lstack_pop(LinkedStack *s, int *out);      /* O(1), DSA_ERR_EMPTY */
dsa_status lstack_peek(const LinkedStack *s, int *out);/* O(1), DSA_ERR_EMPTY */
size_t lstack_size(const LinkedStack *s);             /* O(1) */
int lstack_is_empty(const LinkedStack *s);            /* O(1) */

/* ------------------------------------------------------------ Application */

/* Classic stack exercise: are the brackets ()[]{} in `text` balanced and
 * properly nested? Other characters are ignored.
 * Returns DSA_OK if balanced, DSA_ERR_INVALID if not, DSA_ERR_NOMEM if the
 * helper stack could not be allocated. O(n) time, O(n) space. */
dsa_status stack_check_brackets(const char *text);

#endif /* STACK_H */
