#include "unity.h"
#include "using_free.h"
#include "assertion_helpers.h"
#include "fff.h"

extern free_func_t _using_free__free_func;
static const char _dummy_string[] = "0-118999-88199-9919-725-3";

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(fff_free_func, void*);


void setUp() {
    _using_free__free_func = fff_free_func;
}

void tearDown() {
    RESET_FAKE(fff_free_func);
}

void test__USING_FREE__frees_object()
{
    USING(_dummy_string, {});
    TEST_ASSERT_EQUAL(1, fff_free_func_fake.call_count);
}

void test__USING_FREE__given_throwing_code_block__frees_reference_and_rethrows() {
    TEST_ASSERT_THROWS(ERROR_CODE_INDEX_OUT_OF_RANGE, {
        USING(_dummy_string, {
            Throw(ERROR_CODE_INDEX_OUT_OF_RANGE);
        });
    });

    TEST_ASSERT_EQUAL(1, fff_free_func_fake.call_count);
}

int main()
{
    UNITY_BEGIN();

    TEST_ASSERT_THROWS_NOT({
        RUN_TEST(test__USING_FREE__frees_object);
        RUN_TEST(test__USING_FREE__given_throwing_code_block__frees_reference_and_rethrows);
    });

    return UNITY_END();
}

int app_main()
{
    return main();
}