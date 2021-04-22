#include <unity.h>

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_equal(void) {
    TEST_ASSERT_EQUAL(1, 2);
}

void app_main() {
    UNITY_BEGIN();

    RUN_TEST(test_equal);

    UNITY_END();
}