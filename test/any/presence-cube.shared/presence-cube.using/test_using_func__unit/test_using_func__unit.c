#include "unity.h"
#include "using/using_func.h"
#include "assertion_helpers.h"
#include "fff.h"

static const char _dummy_string[] = "0-118999-88199-9919-725-3";

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(fff_free, const char *);

void setUp() {}
void tearDown()
{
    RESET_FAKE(fff_free);
}

void test_USING_FUNC__given_successful_codeblock__calls_func()
{
    USING_FUNC(_dummy_string, {
        TEST_MESSAGE("Calling code_block...");
    }, fff_free);

    TEST_ASSERT_EQUAL(1, fff_free_fake.call_count);
    TEST_ASSERT_EQUAL_PTR(_dummy_string, fff_free_fake.arg0_val);
}

void test__USING_FUNC__given_throwing_codeblock__calls_func_and_rethrows()
{
    TEST_ASSERT_THROWS(ERROR_CODE_INSUFFICIENT_BUFFER, {
        USING_FUNC(_dummy_string, {
            Throw(ERROR_CODE_INSUFFICIENT_BUFFER);
        },fff_free);
    });

    TEST_ASSERT_EQUAL(1, fff_free_fake.call_count);
}

void test__USING_FUNC__given_code_block__runs_in_child_scope() {
    USING_FUNC(_dummy_string, {
        uint8_t number = 123;
        TEST_PRINTF("Number %i is block-scoped.", number);
    }, fff_free);

    uint8_t number = 0;
    TEST_ASSERT_EQUAL(0, number);
}

void test__USING_FUNC_given_code_block__can_access_parent_scope() {
    uint8_t volatile number = 41;
    
    USING_FUNC(_dummy_string, {
        number++;
    }, fff_free);

    TEST_ASSERT_EQUAL(42, number);
}

void test__USING_FUNC__given_early_breaking_codeblock__does_not_call_func() {
    
    USING_FUNC(_dummy_string, {
        /* return; */
    }, fff_free);
    
    // TEST_ASSERT_EQUAL(0, fff_free_fake.call_count);
    TEST_IGNORE_MESSAGE("Prematurely breaking execution of code_blocks is unsupported! Please see the code docs!");
}

int main()
{
    UNITY_BEGIN();

    TEST_ASSERT_THROWS_NOT({
        RUN_TEST(test_USING_FUNC__given_successful_codeblock__calls_func);
        RUN_TEST(test__USING_FUNC__given_throwing_codeblock__calls_func_and_rethrows);
        RUN_TEST(test__USING_FUNC__given_code_block__runs_in_child_scope);
        RUN_TEST(test__USING_FUNC_given_code_block__can_access_parent_scope);
        RUN_TEST(test__USING_FUNC__given_early_breaking_codeblock__does_not_call_func);
    });

    return UNITY_END();
}

int app_main()
{
    return main();
}