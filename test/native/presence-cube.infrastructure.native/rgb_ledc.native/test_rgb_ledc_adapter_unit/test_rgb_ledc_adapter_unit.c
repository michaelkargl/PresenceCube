#include "unity.h"
#include "rgb_ledc_adapter.h"
#include "assertion_helpers.h"

static const rgb_led_domain_t _dummy_domain_led = {};
static const uint8_t _dummy_color = 12;

static void test_function_pointer_compatibility()
{
    void *_;
    _ = (rgb_ledc_adapter__set_color_func_t)rgb_ledc_adapter__set_color;
}

static void test_set_color__given_null_input__throws_argument_null_error()
{
    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        rgb_ledc_adapter__set_color(NULL, 0, 0, 0);
    });
}

static void test_set_color__given_uninitialized_module__throws()
{
    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_MODULE_ACCESS, {
        rgb_ledc_adapter__set_color(&_dummy_domain_led, _dummy_color, _dummy_color, _dummy_color);
    });
}

static void test_set_color__initializes_module()
{
    TEST_ASSERT_FALSE(rgb_ledc_adapter__initialized());
    rgb_ledc_adapter__initialize();
    TEST_ASSERT_TRUE(rgb_ledc_adapter__initialized());
}

static void run_order_critical_tests()
{
    test_set_color__given_uninitialized_module__throws();
    // ...
    // this needs to be called last
    test_set_color__initializes_module();
}

int main()
{
    UNITY_BEGIN();

    TEST_ASSERT_THROWS_NOT({
        run_order_critical_tests();
        RUN_TEST(test_set_color__given_null_input__throws_argument_null_error);
    })

    return UNITY_END();
}

int app_main()
{
    return main();
}