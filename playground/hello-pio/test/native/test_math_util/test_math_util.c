#include "unity.h"
#include "math_util.h"

static void test_add()
{
    TEST_ASSERT_EQUAL(3, add_util__add(1, 2));
}

static void test_subtract()
{
    TEST_ASSERT_EQUAL(-1, subtract_util__subtract(1, 2));
}

static void test_multply()
{
    TEST_ASSERT_EQUAL(123 * 431, minmax_util__multiply(123, 431));
}

static void test_circumference()
{
    int r = 123;
    int pi = 3;
    TEST_ASSERT_EQUAL(2 * r * pi, circle_util__circumference(r));
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_add);
    RUN_TEST(test_subtract);
    RUN_TEST(test_multply);

    return UNITY_END();
}

int app_main()
{
    return main();
}