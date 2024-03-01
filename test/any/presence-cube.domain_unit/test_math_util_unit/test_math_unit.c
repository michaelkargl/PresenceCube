#include "unity.h"
#include "math_util.h"
#include "exception_handling.h"


static void testcase__min_returns_min_value(int left, int right, int expected)
{
    TEST_PRINTF("Asserting min(%i, %i) = %i.", left, right, expected);

    int result = math_util__min(left, right);

    TEST_ASSERT_EQUAL_INT(expected, result);
}

void test_min__returns_min_value()
{
    testcase__min_returns_min_value(0, 1, 0);
    testcase__min_returns_min_value(1, 0, 0);
    testcase__min_returns_min_value(0, 0, 0);
    testcase__min_returns_min_value(-1, 0, -1);
    testcase__min_returns_min_value(0, -1, -1);
}

// -----------------------------------------------------------------------

static void testcase__max__returns_max_value(int left, int right, int expected)
{
    TEST_PRINTF("Asserting max(%i, %i) = %i.", left, right, expected);

    int result = math_util__max(left, right);

    TEST_ASSERT_EQUAL_INT(expected, result);
}

void test_max__returns_max_value()
{
    testcase__max__returns_max_value(0, 1, 1);
    testcase__max__returns_max_value(1, 0, 1);
    testcase__max__returns_max_value(0, 0, 0);
    testcase__max__returns_max_value(-1, 0, 0);
    testcase__max__returns_max_value(0, -1, 0);
}

// -----------------------------------------------------------------------

static void testcase__assert_clamp__limits_value(int value, int lower_bounds, int upper_bounds, int expected) {
    TEST_PRINTF("Asserting clamp(%i, %i, %i) = %i.", value, lower_bounds, upper_bounds, expected);

    int actual = math_util__clamp(value, lower_bounds, upper_bounds);

    TEST_ASSERT_EQUAL_INT(expected, actual);
}

void test_clamp__limits_value() { 
    testcase__assert_clamp__limits_value(-1, -1, -1, -1);
    testcase__assert_clamp__limits_value(0, 0, 0, 0);
    testcase__assert_clamp__limits_value(0, 0, 100, 0);
    testcase__assert_clamp__limits_value(50, 0, 100, 50);
    testcase__assert_clamp__limits_value(100, 0, 100, 100);
    testcase__assert_clamp__limits_value(120, 0, 100, 100);
    testcase__assert_clamp__limits_value(-120, 0, 100, 0);
}

// -----------------------------------------------------------------------

static void testcase__clamp__given_swapped_boundaries__throws(int lower_bound, int upper_bound) {
    TEST_PRINTF("Asserting that clamp(%i, %i) throws.", lower_bound, upper_bound);
    CEXCEPTION_T ex;
    Try {
        math_util__clamp(0, lower_bound, upper_bound);
        TEST_FAIL_MESSAGE("Expected exception to be thrown, but none received!");
    } Catch(ex) {}

    TEST_ASSERT_EQUAL(ex, ERROR_CODE_LOWER_BOUNDS_BIGGER_THAN_UPPER_BOUNDS);
}

void test_clamp__given_swapped_boundaries__throws() {
    testcase__clamp__given_swapped_boundaries__throws(0, -1);
    testcase__clamp__given_swapped_boundaries__throws(1, 0);
}

// -----------------------------------------------------------------------

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_min__returns_min_value);
    RUN_TEST(test_max__returns_max_value);
    RUN_TEST(test_clamp__limits_value);
    RUN_TEST(test_clamp__given_swapped_boundaries__throws);

    return UNITY_END();
}

int app_main()
{
    return main();
}