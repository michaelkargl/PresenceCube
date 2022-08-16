#include "unity.h"
#include "math_util.h"
#include "CException.h"


static void _test_min_returns_min_value(int left, int right, int expected)
{
    TEST_PRINTF("Asserting min(%i, %i) = %i.", left, right, expected);

    int result = min(left, right);

    TEST_ASSERT_EQUAL_INT(expected, result);
}

void test_min__returns_min_value()
{
    _test_min_returns_min_value(0, 1, 0);
    _test_min_returns_min_value(1, 0, 0);
    _test_min_returns_min_value(0, 0, 0);
    _test_min_returns_min_value(-1, 0, -1);
    _test_min_returns_min_value(0, -1, -1);
}

// -----------------------------------------------------------------------

static void _test_max_returns_max_value(int left, int right, int expected)
{
    TEST_PRINTF("Asserting max(%i, %i) = %i.", left, right, expected);

    int result = max(left, right);

    TEST_ASSERT_EQUAL_INT(expected, result);
}

void test_max__returns_max_value()
{
    _test_max_returns_max_value(0, 1, 1);
    _test_max_returns_max_value(1, 0, 1);
    _test_max_returns_max_value(0, 0, 0);
    _test_max_returns_max_value(-1, 0, 0);
    _test_max_returns_max_value(0, -1, 0);
}

// -----------------------------------------------------------------------

static void _test_ranged_value__limits_value(int value, int lower_bounds, int upper_bounds, int expected) {
    TEST_PRINTF("Asserting ranged_value(%i, %i, %i) = %i.", value, lower_bounds, upper_bounds, expected);

    int actual = ranged_value(value, lower_bounds, upper_bounds);

    TEST_ASSERT_EQUAL_INT(expected, actual);
}

void test_ranged_value__limits_value() { 
    _test_ranged_value__limits_value(-1, -1, -1, -1);
    _test_ranged_value__limits_value(0, 0, 0, 0);
    _test_ranged_value__limits_value(0, 0, 100, 0);
    _test_ranged_value__limits_value(50, 0, 100, 50);
    _test_ranged_value__limits_value(100, 0, 100, 100);
    _test_ranged_value__limits_value(120, 0, 100, 100);
    _test_ranged_value__limits_value(-120, 0, 100, 0);
}

// -----------------------------------------------------------------------

static void _test_ranged_value__given_swapped_boundaries__throws(int lower_bound, int upper_bound) {
    TEST_PRINTF("Asserting that ranged_value(%i, %i) throws.", lower_bound, upper_bound);
    CEXCEPTION_T ex;
    Try {
        ranged_value(0, lower_bound, upper_bound);
        TEST_FAIL_MESSAGE("Expected exception to be thrown, but none received!");
    } Catch(ex) {}

    TEST_ASSERT_EQUAL(ex, LOWER_BOUNDS_BIGGER_THAN_UPPER_BOUNDS);
}

void test_ranged_value__given_swapped_boundaries__throws() {
    _test_ranged_value__given_swapped_boundaries__throws(0, -1);
    _test_ranged_value__given_swapped_boundaries__throws(1, 0);
}

// -----------------------------------------------------------------------

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_min__returns_min_value);
    RUN_TEST(test_max__returns_max_value);
    RUN_TEST(test_ranged_value__limits_value);
    RUN_TEST(test_ranged_value__given_swapped_boundaries__throws);

    return UNITY_END();
}

int app_main()
{
    return main();
}