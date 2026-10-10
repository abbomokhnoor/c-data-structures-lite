#include "test_framework.h"
#include "slist.h"

/* Compare the list with an expected array using only the public API. */
static int list_equals(const SList *l, const int *expected, size_t n)
{
    if (slist_size(l) != n) {
        return 0;
    }
    for (size_t i = 0; i < n; i++) {
        int x = 0;
        if (slist_get(l, i, &x) != DSA_OK || x != expected[i]) {
            return 0;
        }
    }
    return 1;
}

static void test_create_empty(void)
{
    SList *l = slist_create();
    CHECK(l != NULL);
    CHECK(slist_is_empty(l));
    CHECK_EQ_INT(slist_size(l), 0);
    int x;
    CHECK_STATUS(slist_pop_front(l, &x), DSA_ERR_EMPTY);
    CHECK_STATUS(slist_pop_back(l, &x), DSA_ERR_EMPTY);
    slist_destroy(l);
    slist_destroy(NULL);
}

static void test_push_front_back(void)
{
    SList *l = slist_create();
    slist_push_back(l, 2);
    slist_push_back(l, 3);
    slist_push_front(l, 1);
    int expected[] = {1, 2, 3};
    CHECK(list_equals(l, expected, 3));
    CHECK(l->head->value == 1);
    CHECK(l->tail->value == 3);
    slist_destroy(l);
}

static void test_pop_front(void)
{
    SList *l = slist_create();
    slist_push_back(l, 1);
    slist_push_back(l, 2);
    int x = 0;
    CHECK_STATUS(slist_pop_front(l, &x), DSA_OK);
    CHECK_EQ_INT(x, 1);
    CHECK_STATUS(slist_pop_front(l, &x), DSA_OK);
    CHECK_EQ_INT(x, 2);
    CHECK(l->head == NULL && l->tail == NULL);   /* tail reset when emptied */
    slist_push_back(l, 9);                       /* list must still work */
    CHECK_EQ_INT(slist_size(l), 1);
    slist_destroy(l);
}

static void test_pop_back(void)
{
    SList *l = slist_create();
    for (int i = 1; i <= 3; i++) {
        slist_push_back(l, i);
    }
    int x = 0;
    CHECK_STATUS(slist_pop_back(l, &x), DSA_OK);
    CHECK_EQ_INT(x, 3);
    CHECK(l->tail->value == 2);
    CHECK(l->tail->next == NULL);
    slist_pop_back(l, NULL);
    slist_pop_back(l, &x);
    CHECK_EQ_INT(x, 1);
    CHECK(slist_is_empty(l));
    slist_destroy(l);
}

static void test_insert_at(void)
{
    SList *l = slist_create();
    CHECK_STATUS(slist_insert_at(l, 0, 10), DSA_OK);
    CHECK_STATUS(slist_insert_at(l, 1, 30), DSA_OK);
    CHECK_STATUS(slist_insert_at(l, 1, 20), DSA_OK);
    CHECK_STATUS(slist_insert_at(l, 5, 99), DSA_ERR_RANGE);
    int expected[] = {10, 20, 30};
    CHECK(list_equals(l, expected, 3));
    slist_destroy(l);
}

static void test_remove_at(void)
{
    SList *l = slist_create();
    for (int i = 0; i < 5; i++) {
        slist_push_back(l, i);
    }
    int x = -1;
    CHECK_STATUS(slist_remove_at(l, 2, &x), DSA_OK);
    CHECK_EQ_INT(x, 2);
    CHECK_STATUS(slist_remove_at(l, 3, &x), DSA_OK);     /* the tail */
    CHECK_EQ_INT(x, 4);
    CHECK(l->tail->value == 3);
    CHECK_STATUS(slist_remove_at(l, 0, &x), DSA_OK);
    CHECK_STATUS(slist_remove_at(l, 9, &x), DSA_ERR_RANGE);
    int expected[] = {1, 3};
    CHECK(list_equals(l, expected, 2));
    slist_destroy(l);
}

static void test_remove_value_and_find(void)
{
    SList *l = slist_create();
    slist_push_back(l, 5);
    slist_push_back(l, 6);
    slist_push_back(l, 5);
    CHECK_EQ_INT(slist_find(l, 6), 1);
    CHECK_EQ_INT(slist_find(l, 42), -1);
    CHECK_STATUS(slist_remove_value(l, 5), DSA_OK);      /* removes the first 5 */
    int expected[] = {6, 5};
    CHECK(list_equals(l, expected, 2));
    CHECK_STATUS(slist_remove_value(l, 42), DSA_ERR_NOT_FOUND);
    slist_destroy(l);
}

static void test_reverse(void)
{
    SList *l = slist_create();
    slist_reverse(l);                                    /* empty: no crash */
    for (int i = 1; i <= 4; i++) {
        slist_push_back(l, i);
    }
    slist_reverse(l);
    int expected[] = {4, 3, 2, 1};
    CHECK(list_equals(l, expected, 4));
    CHECK(l->tail->value == 1);
    slist_push_back(l, 0);                               /* tail still valid */
    CHECK(l->tail->value == 0);
    slist_destroy(l);
}

static void test_get_errors(void)
{
    SList *l = slist_create();
    int x;
    CHECK_STATUS(slist_get(l, 0, &x), DSA_ERR_RANGE);
    CHECK_STATUS(slist_get(NULL, 0, &x), DSA_ERR_INVALID);
    CHECK_STATUS(slist_push_back(NULL, 1), DSA_ERR_INVALID);
    slist_destroy(l);
}

static void test_many_items(void)
{
    SList *l = slist_create();
    for (int i = 0; i < 5000; i++) {
        slist_push_back(l, i);
    }
    CHECK_EQ_INT(slist_size(l), 5000);
    int x = 0;
    slist_get(l, 4999, &x);
    CHECK_EQ_INT(x, 4999);
    slist_destroy(l);
}

int main(void)
{
    RUN_TEST(test_create_empty);
    RUN_TEST(test_push_front_back);
    RUN_TEST(test_pop_front);
    RUN_TEST(test_pop_back);
    RUN_TEST(test_insert_at);
    RUN_TEST(test_remove_at);
    RUN_TEST(test_remove_value_and_find);
    RUN_TEST(test_reverse);
    RUN_TEST(test_get_errors);
    RUN_TEST(test_many_items);
    return TEST_REPORT("slist");
}
