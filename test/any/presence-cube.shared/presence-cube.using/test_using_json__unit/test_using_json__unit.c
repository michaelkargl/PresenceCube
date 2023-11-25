#include "unity.h"
#include "using_json.h"
#include "assertion_helpers.h"
#include "fff.h"

extern cJSON_Delete_func_t using_json__cJSON_Delete_func;

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(fff_cJSON_Delete_func, cJSON*);


void setUp() {
    using_json__cJSON_Delete_func = fff_cJSON_Delete_func;
}

void tearDown() {
    RESET_FAKE(fff_cJSON_Delete_func);
}

void test__USING_JSON__frees_object()
{
    cJSON *led_array = cJSON_CreateArray();

    TEST_ASSERT_NOT_NULL(led_array);
    USING_JSON(led_array, {});
    
    TEST_ASSERT_EQUAL(1, fff_cJSON_Delete_func_fake.call_count);
}

void test__USING_JSON__given_throwing_code_block__frees_reference_and_rethrows() {
    cJSON *led_array = cJSON_CreateArray();

    TEST_ASSERT_THROWS(ERROR_CODE_INDEX_OUT_OF_RANGE, {
        USING_JSON(led_array, {
            Throw(ERROR_CODE_INDEX_OUT_OF_RANGE);
        })
    });

    TEST_ASSERT_EQUAL(1, fff_cJSON_Delete_func_fake.call_count);
}

int main()
{
    UNITY_BEGIN();

    TEST_ASSERT_THROWS_NOT({
        RUN_TEST(test__USING_JSON__frees_object);
        RUN_TEST(test__USING_JSON__given_throwing_code_block__frees_reference_and_rethrows);
    });

    return UNITY_END();
}

int app_main()
{
    return main();
}