/* demo_stack.c - LIFO behaviour, both stack flavours, bracket checking. */
#include <stdio.h>

#include "stack.h"

static void check(const char *text)
{
    dsa_status st = stack_check_brackets(text);
    printf("  %-22s -> %s\n", text, st == DSA_OK ? "balanced" : "NOT balanced");
}

int main(void)
{
    printf("== Stack demo ==\n\n");

    ArrayStack *a = astack_create(0);
    LinkedStack *l = lstack_create();
    if (a == NULL || l == NULL) {
        astack_destroy(a);
        lstack_destroy(l);
        return 1;
    }

    printf("Pushing 1 2 3 4 onto both stacks...\n");
    for (int i = 1; i <= 4; i++) {
        astack_push(a, i);
        lstack_push(l, i);
    }

    int top = 0;
    astack_peek(a, &top);
    printf("peek -> %d (size %zu)\n", top, astack_size(a));

    printf("array  stack pops: ");
    while (astack_pop(a, &top) == DSA_OK) {
        printf("%d ", top);
    }
    printf("\nlinked stack pops: ");
    while (lstack_pop(l, &top) == DSA_OK) {
        printf("%d ", top);
    }
    printf("\n(last in, first out!)\n");
    printf("pop on empty -> %s\n\n", dsa_strerror(astack_pop(a, &top)));

    printf("Bracket checker (a classic stack exercise):\n");
    check("{[()]}");
    check("int main() { return a[0]; }");
    check("(]");
    check("((())");

    astack_destroy(a);
    lstack_destroy(l);
    return 0;
}
