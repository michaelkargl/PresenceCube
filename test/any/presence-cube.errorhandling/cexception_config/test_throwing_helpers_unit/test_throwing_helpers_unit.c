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

void test_throw_argument_null__throws()
{
    uint8_t number = 42;

    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        THROW_ARGUMENT_NULL(number);
    });
}

void test_throw_argument_null_given_null_variable__throws()
{
    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        uint8_t *argument = NULL;
        THROW_ARGUMENT_NULL_IF_NULL(argument);
    });
}

void test_throw_argument_null_if_null__given_non_null_reference__does_not_throw()
{
    TEST_ASSERT_THROWS_NOT({
        char argument = 'a';
        THROW_ARGUMENT_NULL_IF_NULL(&argument);
    });
}


void test_throw_uninitialized_access_if_uninitialized_struct__given_initialized_struct__does_not_throw()
{
    TEST_ASSERT_THROWS_NOT({
        THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_STRUCT(_initialized_struct);
    });
}

void test_throw_uninitialized_access_if_uninitialized_struct__given_uninitialized_struct__throws()
{
    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, {
        THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_STRUCT(_uninitialized_struct);
    });
}

void test_throw_uninitialized_access_if_uninitialized_struct_ref__given_uninitialized_struct_reference__throw()
{
    struct initializable_t *uninitialized_struct = &_uninitialized_struct;

    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, {
        THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_STRUCT_REF(uninitialized_struct);
    });
}

void test_throw_uninitialized_access_if_uninitialized_struct_ref__given_initialized_struct_reference__does_not_throw()
{
    const struct initializable_t *struct_ref = &_initialized_struct;

    TEST_ASSERT_THROWS_NOT({
        THROW_UNINITIALIZED_ACCESS_IF_UNINITIALIZED_STRUCT_REF(struct_ref);
    });
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_throw_argument_null__throws);
    
    RUN_TEST(test_throw_argument_null_given_null_variable__throws);
    RUN_TEST(test_throw_argument_null_if_null__given_non_null_reference__does_not_throw);

    RUN_TEST(test_throw_uninitialized_access_if_uninitialized_struct__given_initialized_struct__does_not_throw);
    RUN_TEST(test_throw_uninitialized_access_if_uninitialized_struct__given_uninitialized_struct__throws);

    RUN_TEST(test_throw_uninitialized_access_if_uninitialized_struct_ref__given_initialized_struct_reference__does_not_throw);
    RUN_TEST(test_throw_uninitialized_access_if_uninitialized_struct_ref__given_uninitialized_struct_reference__throw);

    return UNITY_END();
}

int app_main()
{
    return main();
}