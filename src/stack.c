#include "stack.h"

#include <stdint.h>
#include <stdlib.h>

#define ASTACK_DEFAULT_CAPACITY 8

/* ------------------------------------------------------------ ArrayStack */

ArrayStack *astack_create(size_t initial_capacity)
{
    if (initial_capacity == 0) {
        initial_capacity = ASTACK_DEFAULT_CAPACITY;
    }
    if (initial_capacity > SIZE_MAX / sizeof(int)) {
        return NULL;
    }
    ArrayStack *s = malloc(sizeof *s);
    if (s == NULL) {
        return NULL;
    }
    s->data = malloc(initial_capacity * sizeof(int));
    if (s->data == NULL) {
        free(s);                 /* undo the first allocation before failing */
        return NULL;
    }
    s->size = 0;
    s->capacity = initial_capacity;
    return s;
}

void astack_destroy(ArrayStack *s)
{
    if (s == NULL) {
        return;
    }
    free(s->data);
    free(s);
}

dsa_status astack_push(ArrayStack *s, int value)
{
    if (s == NULL) {
        return DSA_ERR_INVALID;
    }
    if (s->size == s->capacity) {
        if (s->capacity > SIZE_MAX / 2 / sizeof(int)) {
            return DSA_ERR_NOMEM;
        }
        size_t bigger_cap = s->capacity * 2;
        int *bigger = realloc(s->data, bigger_cap * sizeof(int));
        if (bigger == NULL) {
            return DSA_ERR_NOMEM;       /* the stack is unchanged and usable */
        }
        s->data = bigger;
        s->capacity = bigger_cap;
    }
    s->data[s->size++] = value;        /* "top" is simply index size-1 */
    return DSA_OK;
}

dsa_status astack_pop(ArrayStack *s, int *out)
{
    if (s == NULL) {
        return DSA_ERR_INVALID;
    }
    if (s->size == 0) {
        return DSA_ERR_EMPTY;
    }
    s->size--;
    if (out != NULL) {
        *out = s->data[s->size];
    }
    return DSA_OK;
}

dsa_status astack_peek(const ArrayStack *s, int *out)
{
    if (s == NULL || out == NULL) {
        return DSA_ERR_INVALID;
    }
    if (s->size == 0) {
        return DSA_ERR_EMPTY;
    }
    *out = s->data[s->size - 1];
    return DSA_OK;
}

size_t astack_size(const ArrayStack *s)
{
    return s == NULL ? 0 : s->size;
}

int astack_is_empty(const ArrayStack *s)
{
    return s == NULL || s->size == 0;
}

/* ----------------------------------------------------------- LinkedStack */

LinkedStack *lstack_create(void)
{
    LinkedStack *s = malloc(sizeof *s);
    if (s == NULL) {
        return NULL;
    }
    s->top = NULL;
    s->size = 0;
    return s;
}

void lstack_destroy(LinkedStack *s)
{
    if (s == NULL) {
        return;
    }
    StackNode *cur = s->top;
    while (cur != NULL) {
        StackNode *next = cur->next;
        free(cur);
        cur = next;
    }
    free(s);
}

dsa_status lstack_push(LinkedStack *s, int value)
{
    if (s == NULL) {
        return DSA_ERR_INVALID;
    }
    StackNode *n = malloc(sizeof *n);
    if (n == NULL) {
        return DSA_ERR_NOMEM;
    }
    n->value = value;
    n->next = s->top;          /* new node sits on top of the old top */
    s->top = n;
    s->size++;
    return DSA_OK;
}

dsa_status lstack_pop(LinkedStack *s, int *out)
{
    if (s == NULL) {
        return DSA_ERR_INVALID;
    }
    if (s->top == NULL) {
        return DSA_ERR_EMPTY;
    }
    StackNode *old = s->top;
    if (out != NULL) {
        *out = old->value;
    }
    s->top = old->next;
    free(old);
    s->size--;
    return DSA_OK;
}

dsa_status lstack_peek(const LinkedStack *s, int *out)
{
    if (s == NULL || out == NULL) {
        return DSA_ERR_INVALID;
    }
    if (s->top == NULL) {
        return DSA_ERR_EMPTY;
    }
    *out = s->top->value;
    return DSA_OK;
}

size_t lstack_size(const LinkedStack *s)
{
    return s == NULL ? 0 : s->size;
}

int lstack_is_empty(const LinkedStack *s)
{
    return s == NULL || s->size == 0;
}

/* ------------------------------------------------------------ Application */

dsa_status stack_check_brackets(const char *text)
{
    if (text == NULL) {
        return DSA_ERR_INVALID;
    }
    ArrayStack *open = astack_create(0);
    if (open == NULL) {
        return DSA_ERR_NOMEM;
    }
    dsa_status result = DSA_OK;
    for (const char *p = text; *p != '\0' && result == DSA_OK; p++) {
        int expected_open = 0;           /* opener that must be on top */
        switch (*p) {
        case '(': case '[': case '{':
            if (astack_push(open, *p) != DSA_OK) {
                result = DSA_ERR_NOMEM;
            }
            break;
        case ')': expected_open = '('; break;
        case ']': expected_open = '['; break;
        case '}': expected_open = '{'; break;
        default:  break;                 /* not a bracket: ignore */
        }
        if (expected_open != 0) {
            int top = 0;
            if (astack_pop(open, &top) != DSA_OK || top != expected_open) {
                result = DSA_ERR_INVALID;   /* closer with no / wrong opener */
            }
        }
    }
    if (result == DSA_OK && !astack_is_empty(open)) {
        result = DSA_ERR_INVALID;           /* openers left unclosed */
    }
    astack_destroy(open);
    return result;
}
