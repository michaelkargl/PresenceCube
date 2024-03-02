#include "unity.h"
#include "assertion_helpers.h"
#include "led_store.h"

static const uint8_t _known_led_id = 1;
static const uint8_t _unknown_led_id = 255;
static const uint8_t _dummy_id = _unknown_led_id;
static const uint8_t _dummy_color = 50;

static void test_initialize()
{
    TEST_ASSERT_FALSE(led_store__initialized());
    led_store__initialize();
    TEST_ASSERT_TRUE(led_store__initialized());
}

static void test_get_leds__given_uninitialized_module__throws()
{
    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_MODULE_ACCESS, {
        led_store__get_leds();
    });
}

static void test_function_compatibility()
{
    (led_store__get_leds_func_t) led_store__get_leds;
    (led_store__get_led_func_t) led_store__get_led;
    (led_store__update_func_t) led_store__update;
}

static void test_get_leds__returns_non_empty_led_bag()
{
    led_store__initialize();

    const rgb_led_domain_bag_t *led_bag = led_store__get_leds();
    TEST_ASSERT_NOT_NULL(led_bag);
    TEST_ASSERT_GREATER_THAN(0, led_bag->count);
}

static void test_get_led__given_uninitialized_module__throws()
{
    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_MODULE_ACCESS, {
        led_store__get_led(_dummy_id);
    });
}

static void test_get_led__given_unknown_id__returns_null()
{
    TEST_ASSERT_NULL(led_store__get_led(_unknown_led_id));
}

static void test_get_led__given_known_id__returns_led()
{
    const rgb_led_diode_t *led = led_store__get_led(_known_led_id);

    TEST_ASSERT_NOT_NULL(led);
    TEST_ASSERT_TRUE(led->is_initialized);
    TEST_ASSERT_EQUAL(_known_led_id, led->id);
    TEST_ASSERT_NOT_EMPTY(led->display_name);
}

void test_update_led__given_valid_id__mutates_state()
{
    const rgb_led_diode_t mutated_led = {
        .id = _known_led_id,
        .red = {.brightness_percent = 99},
        .green = {.brightness_percent = 89},
        .blue = {.brightness_percent = 79}};

    const rgb_led_diode_t *led = led_store__get_led(_known_led_id);
    TEST_ASSERT_NOT_EQUAL(mutated_led.red.brightness_percent, led->red.brightness_percent);
    TEST_ASSERT_NOT_EQUAL(mutated_led.green.brightness_percent, led->green.brightness_percent);
    TEST_ASSERT_NOT_EQUAL(mutated_led.blue.brightness_percent, led->blue.brightness_percent);

    led_store__update(mutated_led);

    TEST_ASSERT_EQUAL(mutated_led.red.brightness_percent, led->red.brightness_percent);
    TEST_ASSERT_EQUAL(mutated_led.green.brightness_percent, led->green.brightness_percent);
    TEST_ASSERT_EQUAL(mutated_led.blue.brightness_percent, led->blue.brightness_percent);
}

static void run_order_critical_tests()
{
    RUN_TEST(test_get_leds__given_uninitialized_module__throws);
    RUN_TEST(test_get_led__given_uninitialized_module__throws);
    RUN_TEST(test_initialize);
}

int main()
{
    UNITY_BEGIN();
    TEST_ASSERT_THROWS_NOT({
        run_order_critical_tests();
        RUN_TEST(test_get_leds__returns_non_empty_led_bag);
        RUN_TEST(test_function_compatibility);
        RUN_TEST(test_get_led__given_unknown_id__returns_null);
        RUN_TEST(test_get_led__given_known_id__returns_led);
        RUN_TEST(test_update_led__given_valid_id__mutates_state);
    });
    return UNITY_END();
}

int app_main()
{
    return main();
}