#include "test_framework.h"
#include "vector.h"

static void test_init_is_empty(void)
{
    Vector v;
    CHECK_STATUS(vector_init(&v), DSA_OK);
    CHECK(vector_is_empty(&v));
    CHECK_EQ_INT(vector_size(&v), 0);
    CHECK_EQ_INT(vector_capacity(&v), 0);
    vector_free(&v);
    CHECK_STATUS(vector_init(NULL), DSA_ERR_INVALID);
}

static void test_push_and_get(void)
{
    Vector v;
    vector_init(&v);
    for (int i = 0; i < 100; i++) {
        CHECK_STATUS(vector_push_back(&v, i * 3), DSA_OK);
    }
    CHECK_EQ_INT(vector_size(&v), 100);
    int x = -1;
    CHECK_STATUS(vector_get(&v, 0, &x), DSA_OK);
    CHECK_EQ_INT(x, 0);
    CHECK_STATUS(vector_get(&v, 99, &x), DSA_OK);
    CHECK_EQ_INT(x, 297);
    CHECK(vector_capacity(&v) >= 100);
    vector_free(&v);
}

static void test_growth_is_geometric(void)
{
    Vector v;
    vector_init(&v);
    size_t reallocations = 0;
    size_t last_cap = 0;
    for (int i = 0; i < 1000; i++) {
        vector_push_back(&v, i);
        if (vector_capacity(&v) != last_cap) {
            reallocations++;
            last_cap = vector_capacity(&v);
        }
    }
    CHECK(reallocations <= 10);        /* doubling: ~log2(1000), not 1000 */
    vector_free(&v);
}

static void test_get_set_out_of_range(void)
{
    Vector v;
    vector_init(&v);
    vector_push_back(&v, 7);
    int x = 0;
    CHECK_STATUS(vector_get(&v, 1, &x), DSA_ERR_RANGE);
    CHECK_STATUS(vector_set(&v, 1, 9), DSA_ERR_RANGE);
    CHECK_STATUS(vector_set(&v, 0, 9), DSA_OK);
    vector_get(&v, 0, &x);
    CHECK_EQ_INT(x, 9);
    CHECK_STATUS(vector_get(&v, 0, NULL), DSA_ERR_INVALID);
    vector_free(&v);
}

static void test_pop_back(void)
{
    Vector v;
    vector_init(&v);
    int x = 0;
    CHECK_STATUS(vector_pop_back(&v, &x), DSA_ERR_EMPTY);
    vector_push_back(&v, 1);
    vector_push_back(&v, 2);
    CHECK_STATUS(vector_pop_back(&v, &x), DSA_OK);
    CHECK_EQ_INT(x, 2);
    CHECK_STATUS(vector_pop_back(&v, NULL), DSA_OK);
    CHECK(vector_is_empty(&v));
    vector_free(&v);
}

static void test_insert(void)
{
    Vector v;
    vector_init(&v);
    CHECK_STATUS(vector_insert(&v, 0, 20), DSA_OK);   /* into empty */
    CHECK_STATUS(vector_insert(&v, 0, 10), DSA_OK);   /* at front   */
    CHECK_STATUS(vector_insert(&v, 2, 40), DSA_OK);   /* at end     */
    CHECK_STATUS(vector_insert(&v, 2, 30), DSA_OK);   /* middle     */
    CHECK_STATUS(vector_insert(&v, 9, 99), DSA_ERR_RANGE);
    int expected[] = {10, 20, 30, 40};
    CHECK_EQ_INT(vector_size(&v), 4);
    CHECK_ARRAY_EQ(v.data, expected, 4);
    vector_free(&v);
}

static void test_remove(void)
{
    Vector v;
    vector_init(&v);
    for (int i = 1; i <= 5; i++) {
        vector_push_back(&v, i);
    }
    int x = 0;
    CHECK_STATUS(vector_remove(&v, 0, &x), DSA_OK);
    CHECK_EQ_INT(x, 1);
    CHECK_STATUS(vector_remove(&v, 3, &x), DSA_OK);
    CHECK_EQ_INT(x, 5);
    CHECK_STATUS(vector_remove(&v, 7, &x), DSA_ERR_RANGE);
    int expected[] = {2, 3, 4};
    CHECK_EQ_INT(vector_size(&v), 3);
    CHECK_ARRAY_EQ(v.data, expected, 3);
    vector_free(&v);
}

static void test_find(void)
{
    Vector v;
    vector_init(&v);
    CHECK_EQ_INT(vector_find(&v, 1), -1);
    vector_push_back(&v, 5);
    vector_push_back(&v, 6);
    vector_push_back(&v, 5);
    CHECK_EQ_INT(vector_find(&v, 5), 0);      /* first match */
    CHECK_EQ_INT(vector_find(&v, 6), 1);
    CHECK_EQ_INT(vector_find(&v, 7), -1);
    vector_free(&v);
}

static void test_reserve_clear_shrink(void)
{
    Vector v;
    vector_init(&v);
    CHECK_STATUS(vector_reserve(&v, 50), DSA_OK);
    CHECK_EQ_INT(vector_capacity(&v), 50);
    CHECK_EQ_INT(vector_size(&v), 0);
    vector_push_back(&v, 1);
    vector_push_back(&v, 2);
    CHECK_STATUS(vector_shrink_to_fit(&v), DSA_OK);
    CHECK_EQ_INT(vector_capacity(&v), 2);
    vector_clear(&v);
    CHECK(vector_is_empty(&v));
    CHECK(vector_capacity(&v) == 2);          /* memory kept */
    CHECK_STATUS(vector_shrink_to_fit(&v), DSA_OK);
    CHECK_EQ_INT(vector_capacity(&v), 0);
    CHECK_STATUS(vector_reserve(&v, (size_t)-1), DSA_ERR_NOMEM);   /* overflow guard */
    vector_free(&v);
}

static void test_free_is_idempotent(void)
{
    Vector v;
    vector_init(&v);
    vector_push_back(&v, 1);
    vector_free(&v);
    vector_free(&v);                          /* second call must be harmless */
    CHECK(v.data == NULL);
    vector_free(NULL);
    CHECK(vector_is_empty(NULL));
}

int main(void)
{
    RUN_TEST(test_init_is_empty);
    RUN_TEST(test_push_and_get);
    RUN_TEST(test_growth_is_geometric);
    RUN_TEST(test_get_set_out_of_range);
    RUN_TEST(test_pop_back);
    RUN_TEST(test_insert);
    RUN_TEST(test_remove);
    RUN_TEST(test_find);
    RUN_TEST(test_reserve_clear_shrink);
    RUN_TEST(test_free_is_idempotent);
    return TEST_REPORT("vector");
}
