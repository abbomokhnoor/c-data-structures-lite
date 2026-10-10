#include "test_framework.h"
#include "stack.h"

static void test_array_stack_lifo(void)
{
    ArrayStack *s = astack_create(2);          /* tiny, to force growth */
    CHECK(s != NULL);
    CHECK(astack_is_empty(s));
    for (int i = 1; i <= 20; i++) {
        CHECK_STATUS(astack_push(s, i), DSA_OK);
    }
    CHECK_EQ_INT(astack_size(s), 20);
    int x = 0;
    CHECK_STATUS(astack_peek(s, &x), DSA_OK);
    CHECK_EQ_INT(x, 20);
    for (int i = 20; i >= 1; i--) {
        CHECK_STATUS(astack_pop(s, &x), DSA_OK);
        CHECK_EQ_INT(x, i);
    }
    CHECK(astack_is_empty(s));
    astack_destroy(s);
}

static void test_array_stack_errors(void)
{
    ArrayStack *s = astack_create(0);          /* 0 = default capacity */
    CHECK(s != NULL);
    int x;
    CHECK_STATUS(astack_pop(s, &x), DSA_ERR_EMPTY);
    CHECK_STATUS(astack_peek(s, &x), DSA_ERR_EMPTY);
    CHECK_STATUS(astack_push(NULL, 1), DSA_ERR_INVALID);
    CHECK_STATUS(astack_peek(s, NULL), DSA_ERR_INVALID);
    CHECK(astack_create((size_t)-1) == NULL);  /* overflow guard */
    astack_destroy(s);
    astack_destroy(NULL);
}

static void test_linked_stack_lifo(void)
{
    LinkedStack *s = lstack_create();
    CHECK(s != NULL);
    CHECK(lstack_is_empty(s));
    for (int i = 1; i <= 50; i++) {
        CHECK_STATUS(lstack_push(s, i * 2), DSA_OK);
    }
    CHECK_EQ_INT(lstack_size(s), 50);
    int x = 0;
    CHECK_STATUS(lstack_peek(s, &x), DSA_OK);
    CHECK_EQ_INT(x, 100);
    CHECK_STATUS(lstack_pop(s, &x), DSA_OK);
    CHECK_EQ_INT(x, 100);
    CHECK_STATUS(lstack_pop(s, NULL), DSA_OK);
    CHECK_EQ_INT(lstack_size(s), 48);
    lstack_destroy(s);                         /* frees the 48 left-over nodes */
}

static void test_linked_stack_errors(void)
{
    LinkedStack *s = lstack_create();
    int x;
    CHECK_STATUS(lstack_pop(s, &x), DSA_ERR_EMPTY);
    CHECK_STATUS(lstack_peek(s, &x), DSA_ERR_EMPTY);
    CHECK_STATUS(lstack_push(NULL, 1), DSA_ERR_INVALID);
    CHECK_EQ_INT(lstack_size(NULL), 0);
    lstack_destroy(s);
    lstack_destroy(NULL);
}

static void test_both_stacks_agree(void)
{
    ArrayStack *a = astack_create(0);
    LinkedStack *l = lstack_create();
    for (int i = 0; i < 100; i++) {
        astack_push(a, i * i);
        lstack_push(l, i * i);
    }
    int same = 1;
    for (int i = 0; i < 100; i++) {
        int xa = 0;
        int xl = 1;
        astack_pop(a, &xa);
        lstack_pop(l, &xl);
        if (xa != xl) {
            same = 0;
        }
    }
    CHECK(same);
    astack_destroy(a);
    lstack_destroy(l);
}

static void test_brackets_balanced(void)
{
    CHECK_STATUS(stack_check_brackets(""), DSA_OK);
    CHECK_STATUS(stack_check_brackets("()"), DSA_OK);
    CHECK_STATUS(stack_check_brackets("{[()]}"), DSA_OK);
    CHECK_STATUS(stack_check_brackets("int main() { return a[0]; }"), DSA_OK);
    CHECK_STATUS(stack_check_brackets("()[]{}"), DSA_OK);
}

static void test_brackets_unbalanced(void)
{
    CHECK_STATUS(stack_check_brackets("("), DSA_ERR_INVALID);
    CHECK_STATUS(stack_check_brackets(")"), DSA_ERR_INVALID);
    CHECK_STATUS(stack_check_brackets("(]"), DSA_ERR_INVALID);
    CHECK_STATUS(stack_check_brackets("([)]"), DSA_ERR_INVALID);
    CHECK_STATUS(stack_check_brackets("{{}"), DSA_ERR_INVALID);
    CHECK_STATUS(stack_check_brackets(NULL), DSA_ERR_INVALID);
}

int main(void)
{
    RUN_TEST(test_array_stack_lifo);
    RUN_TEST(test_array_stack_errors);
    RUN_TEST(test_linked_stack_lifo);
    RUN_TEST(test_linked_stack_errors);
    RUN_TEST(test_both_stacks_agree);
    RUN_TEST(test_brackets_balanced);
    RUN_TEST(test_brackets_unbalanced);
    return TEST_REPORT("stack");
}
