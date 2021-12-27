#include "math_util.h"
#include "stdio.h"
#include <unity.h>

void test_function_min(void) {
    printf("Given two different integers, the smaller one is returned");
    
    // Given two numbers
    int small_number = 12;
    int big_number = 23;

    // When executing
    int result = min(small_number, big_number);

    // Then the lower one is returned
    TEST_ASSERT_EQUAL(small_number, result);
}

void test_function_max(void) {
    printf("Given two integers, the bigger one is returned");
    
    // Given two numbers
    int small_number = 12;
    int big_number = 23;

    // When executing
    int result = max(small_number, big_number);

    // Then the lower one is returned
    TEST_ASSERT_EQUAL(big_number, result);
}

void app_main() {
    UNITY_BEGIN();

    RUN_TEST(test_function_min);
    RUN_TEST(test_function_max);

    UNITY_END();
}