#include "unity.h"
#include "math_helper.h"


TEST_CASE("Given two different integers, the smaller one is returned", "[min]") {
    // Given two numbers
    int small_number = 12;
    int big_number = 23;

    // When executing
    int result = min(small_number, big_number);

    // Then the lower one is returned
    TEST_ASSERT_EQUAL(small_number, result);
}
