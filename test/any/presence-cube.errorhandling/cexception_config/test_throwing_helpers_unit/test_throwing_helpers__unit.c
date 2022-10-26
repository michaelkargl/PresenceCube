#include "unity.h"
#include "assertion_helpers.h"
#include "stdbool.h"

static struct initializable_t
{
    bool is_initialized;
} _initialized_struct = {.is_initialized = true},
  _uninitialized_struct;

void setUp() {}
void tearDown() {}

void test_throw_argument_null_if_null__given_null__throws()
{
    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        THROW_ARGUMENT_NULL_IF_NULL(NULL);
    });
}

void test_throw_resource_not_found_if_null__given_null__throws() {
    TEST_ASSERT_THROWS(ERROR_CODE_RESOURCE_NOT_FOUND, {
        THROW_RESOURCE_NOT_FOUND_IF_NULL(NULL, "Resource no found");
    });
}

void test_throw_resource_not_found_if_null__given_non_null__does_not_throw() {
    TEST_ASSERT_THROWS_NOT({
        const char* argument = "a";
        THROW_RESOURCE_NOT_FOUND_IF_NULL(argument, "Resource no found");
    });
}

void test_throw_argument_null_if_null__given_non_null__does_not_throw()
{
    TEST_ASSERT_THROWS_NOT({
        char argument = 'a';
        THROW_ARGUMENT_NULL_IF_NULL(&argument);
    });
}

void test_throw_uninitialized_access_if_uninitialized_struct__given_initialized__does_not_throw()
{
    TEST_ASSERT_THROWS_NOT({
        THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_STRUCT(_initialized_struct);
    });
}

void test_throw_uninitialized_access_if_uninitialized_struct__given_uninitialized__throws()
{
    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, {
        THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_STRUCT(_uninitialized_struct);
    });
}

void test_throw_uninitialized_access_if_uninitialized_struct_ref__given_uninitialized__throws()
{
    struct initializable_t *uninitialized_struct = &_uninitialized_struct;

    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, {
        THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_STRUCT_REF(uninitialized_struct);
    });
}

void test_throw_uninitialized_access_if_uninitialized_struct_ref__given_null__throws()
{
    struct initializable_t *null_struct = NULL;

    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, {
        THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_STRUCT_REF(null_struct);
    });
}

void test_throw_uninitialized_access_if_uninitialized_struct_ref__given_initialized__does_not_throw()
{
    const struct initializable_t *struct_ref = &_initialized_struct;

    TEST_ASSERT_THROWS_NOT({
        THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_STRUCT_REF(struct_ref);
    });
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_throw_resource_not_found_if_null__given_null__throws);
    RUN_TEST(test_throw_resource_not_found_if_null__given_non_null__does_not_throw);

    RUN_TEST(test_throw_argument_null_if_null__given_null__throws);
    RUN_TEST(test_throw_argument_null_if_null__given_non_null__does_not_throw);

    RUN_TEST(test_throw_uninitialized_access_if_uninitialized_struct__given_initialized__does_not_throw);
    RUN_TEST(test_throw_uninitialized_access_if_uninitialized_struct__given_uninitialized__throws);

    RUN_TEST(test_throw_uninitialized_access_if_uninitialized_struct_ref__given_initialized__does_not_throw);
    RUN_TEST(test_throw_uninitialized_access_if_uninitialized_struct_ref__given_uninitialized__throws);
    RUN_TEST(test_throw_uninitialized_access_if_uninitialized_struct_ref__given_null__throws);

    return UNITY_END();
}

int app_main()
{
    return main();
}