/* demo_vector.c - watch a dynamic array grow. */
#include <stdio.h>

#include "vector.h"

static void show(const Vector *v, const char *what)
{
    printf("%-22s size=%-2zu capacity=%-2zu [", what, vector_size(v), vector_capacity(v));
    for (size_t i = 0; i < vector_size(v); i++) {
        int x = 0;
        vector_get(v, i, &x);
        printf("%s%d", i ? ", " : "", x);
    }
    printf("]\n");
}

int main(void)
{
    printf("== Dynamic array (vector) demo ==\n\n");
    Vector v;
    vector_init(&v);
    show(&v, "after init");

    for (int i = 1; i <= 9; i++) {
        if (vector_push_back(&v, i * 10) != DSA_OK) {
            return 1;
        }
        char label[32];
        snprintf(label, sizeof label, "push_back(%d)", i * 10);
        show(&v, label);
    }

    printf("\nCapacity doubled 4 -> 8 -> 16 as needed: pushes are amortized O(1).\n\n");

    vector_insert(&v, 2, 25);
    show(&v, "insert(2, 25)");
    int removed = 0;
    vector_remove(&v, 0, &removed);
    printf("removed %d from the front\n", removed);
    show(&v, "after remove(0)");
    printf("find(60) -> index %td\n", vector_find(&v, 60));

    int out = 0;
    dsa_status st = vector_get(&v, 99, &out);
    printf("get(99) -> %s\n", dsa_strerror(st));

    vector_shrink_to_fit(&v);
    show(&v, "shrink_to_fit");
    vector_free(&v);
    return 0;
}
