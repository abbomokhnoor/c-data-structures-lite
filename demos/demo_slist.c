/* demo_slist.c - a singly linked list in action. */
#include <stdio.h>

#include "slist.h"

int main(void)
{
    printf("== Singly linked list demo ==\n\n");
    SList *list = slist_create();
    if (list == NULL) {
        return 1;
    }

    for (int i = 1; i <= 4; i++) {
        slist_push_back(list, i * 10);
    }
    printf("push_back 10..40 : ");
    slist_print(list, stdout);

    slist_push_front(list, 5);
    printf("push_front(5)    : ");
    slist_print(list, stdout);

    slist_insert_at(list, 3, 25);
    printf("insert_at(3, 25) : ");
    slist_print(list, stdout);

    slist_remove_value(list, 20);
    printf("remove_value(20) : ");
    slist_print(list, stdout);

    slist_reverse(list);
    printf("reverse          : ");
    slist_print(list, stdout);

    int x = 0;
    slist_pop_front(list, &x);
    printf("pop_front -> %d   : ", x);
    slist_print(list, stdout);
    slist_pop_back(list, &x);
    printf("pop_back  -> %d    : ", x);
    slist_print(list, stdout);

    printf("\nfind(25) -> index %td, size = %zu\n", slist_find(list, 25), slist_size(list));
    printf("remove_value(999) -> %s\n", dsa_strerror(slist_remove_value(list, 999)));

    slist_destroy(list);
    return 0;
}
