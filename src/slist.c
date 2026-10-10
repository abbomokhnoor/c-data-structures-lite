#include "slist.h"

#include <stdlib.h>

/* Allocate one node. Returns NULL on failure. O(1). */
static SNode *node_new(int value)
{
    SNode *n = malloc(sizeof *n);
    if (n == NULL) {
        return NULL;
    }
    n->value = value;
    n->next = NULL;
    return n;
}

SList *slist_create(void)
{
    SList *list = malloc(sizeof *list);
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void slist_destroy(SList *list)
{
    if (list == NULL) {
        return;
    }
    SNode *cur = list->head;
    while (cur != NULL) {
        SNode *next = cur->next;   /* save the link BEFORE freeing the node */
        free(cur);
        cur = next;
    }
    free(list);
}

dsa_status slist_push_front(SList *list, int value)
{
    if (list == NULL) {
        return DSA_ERR_INVALID;
    }
    SNode *n = node_new(value);
    if (n == NULL) {
        return DSA_ERR_NOMEM;
    }
    n->next = list->head;
    list->head = n;
    if (list->tail == NULL) {      /* list was empty: n is also the tail */
        list->tail = n;
    }
    list->size++;
    return DSA_OK;
}

dsa_status slist_push_back(SList *list, int value)
{
    if (list == NULL) {
        return DSA_ERR_INVALID;
    }
    SNode *n = node_new(value);
    if (n == NULL) {
        return DSA_ERR_NOMEM;
    }
    if (list->tail == NULL) {      /* empty list */
        list->head = n;
    } else {
        list->tail->next = n;
    }
    list->tail = n;
    list->size++;
    return DSA_OK;
}

dsa_status slist_pop_front(SList *list, int *out)
{
    if (list == NULL) {
        return DSA_ERR_INVALID;
    }
    if (list->head == NULL) {
        return DSA_ERR_EMPTY;
    }
    SNode *old = list->head;
    if (out != NULL) {
        *out = old->value;
    }
    list->head = old->next;
    if (list->head == NULL) {      /* removed the only node */
        list->tail = NULL;
    }
    free(old);
    list->size--;
    return DSA_OK;
}

dsa_status slist_pop_back(SList *list, int *out)
{
    if (list == NULL) {
        return DSA_ERR_INVALID;
    }
    if (list->head == NULL) {
        return DSA_ERR_EMPTY;
    }
    if (list->head == list->tail) {          /* exactly one node */
        return slist_pop_front(list, out);
    }
    SNode *prev = list->head;
    while (prev->next != list->tail) {       /* find the node before the tail */
        prev = prev->next;
    }
    if (out != NULL) {
        *out = list->tail->value;
    }
    free(list->tail);
    prev->next = NULL;
    list->tail = prev;
    list->size--;
    return DSA_OK;
}

dsa_status slist_insert_at(SList *list, size_t index, int value)
{
    if (list == NULL) {
        return DSA_ERR_INVALID;
    }
    if (index > list->size) {
        return DSA_ERR_RANGE;
    }
    if (index == 0) {
        return slist_push_front(list, value);
    }
    if (index == list->size) {
        return slist_push_back(list, value);
    }
    SNode *prev = list->head;                 /* walk to node index-1 */
    for (size_t i = 0; i < index - 1; i++) {
        prev = prev->next;
    }
    SNode *n = node_new(value);
    if (n == NULL) {
        return DSA_ERR_NOMEM;
    }
    n->next = prev->next;                     /* new node points forward first, */
    prev->next = n;                           /* THEN the previous one points to it */
    list->size++;
    return DSA_OK;
}

dsa_status slist_remove_at(SList *list, size_t index, int *out)
{
    if (list == NULL) {
        return DSA_ERR_INVALID;
    }
    if (index >= list->size) {
        return DSA_ERR_RANGE;
    }
    if (index == 0) {
        return slist_pop_front(list, out);
    }
    SNode *prev = list->head;
    for (size_t i = 0; i < index - 1; i++) {
        prev = prev->next;
    }
    SNode *victim = prev->next;
    if (out != NULL) {
        *out = victim->value;
    }
    prev->next = victim->next;                /* unlink, then free */
    if (victim == list->tail) {
        list->tail = prev;
    }
    free(victim);
    list->size--;
    return DSA_OK;
}

dsa_status slist_remove_value(SList *list, int value)
{
    if (list == NULL) {
        return DSA_ERR_INVALID;
    }
    ptrdiff_t idx = slist_find(list, value);
    if (idx < 0) {
        return DSA_ERR_NOT_FOUND;
    }
    return slist_remove_at(list, (size_t)idx, NULL);
}

dsa_status slist_get(const SList *list, size_t index, int *out)
{
    if (list == NULL || out == NULL) {
        return DSA_ERR_INVALID;
    }
    if (index >= list->size) {
        return DSA_ERR_RANGE;
    }
    const SNode *cur = list->head;
    for (size_t i = 0; i < index; i++) {
        cur = cur->next;
    }
    *out = cur->value;
    return DSA_OK;
}

ptrdiff_t slist_find(const SList *list, int value)
{
    if (list == NULL) {
        return -1;
    }
    ptrdiff_t i = 0;
    for (const SNode *cur = list->head; cur != NULL; cur = cur->next, i++) {
        if (cur->value == value) {
            return i;
        }
    }
    return -1;
}

void slist_reverse(SList *list)
{
    if (list == NULL) {
        return;
    }
    SNode *prev = NULL;
    SNode *cur = list->head;
    list->tail = list->head;      /* the old head becomes the new tail */
    while (cur != NULL) {
        SNode *next = cur->next;  /* 1. remember the rest of the list     */
        cur->next = prev;         /* 2. flip this link backwards          */
        prev = cur;               /* 3. step forward                      */
        cur = next;
    }
    list->head = prev;
}

size_t slist_size(const SList *list)
{
    return list == NULL ? 0 : list->size;
}

int slist_is_empty(const SList *list)
{
    return list == NULL || list->size == 0;
}

void slist_print(const SList *list, FILE *out)
{
    if (list == NULL || out == NULL) {
        return;
    }
    for (const SNode *cur = list->head; cur != NULL; cur = cur->next) {
        fprintf(out, "%d -> ", cur->value);
    }
    fprintf(out, "NULL\n");
}
