#include "unity.h"
#include "math_util.h"

static void test_add()
{
    TEST_ASSERT_EQUAL(3, add(1, 2));
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_add);

    return UNITY_END();
}

int app_main()
{
    return main();
}