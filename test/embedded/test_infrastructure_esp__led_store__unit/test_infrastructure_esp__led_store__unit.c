#include "unity.h"
#include "led_store.h"
#include "build_ledc_rgb_led_stub.h"
#include "assertion_helpers.h"

#define TEST_ASSERT_EACH_TRUE_LEDC_RGB_LED(led_count, leds, condition) \
    do                                                                 \
    {                                                                  \
        for (uint8_t i = 0; i < led_count; i++)                        \
        {                                                              \
            struct ledc_rgb_led_t led = leds[i];                       \
            TEST_ASSERT_TRUE(led.condition);                           \
        }                                                              \
    } while (0);

extern struct ledc_rgb_led_t (*led_store__build_ledc_rgb_led)(
    const char name[10],
    const ledc_timer_config_t timer,
    const struct ledc_rgb_channels_t channels,
    const struct ledc_rgb_gpio_pins_t gpio_pins,
    led_type_t led_type);

static const uint8_t _dummy_led_id = 23;
static const uint8_t _unknown_led_id = 255;

void tearDown() {}
void setUp()
{
    led_store__build_ledc_rgb_led = stub__build_ledc_rgb_led;
}

static void test_store_led__led_count__returns_configured_rgb_led_count()
{
    uint8_t led_count = led_store__get_led_count();
    TEST_ASSERT_EQUAL(CONFIG_CUBE_HARDWARE_RGB_LED_COUNT, led_count);
}

static void test_store_led__uninitialized__every_led_uninitialized()
{
    uint8_t led_count = led_store__get_led_count();
    struct ledc_rgb_led_t *leds = led_store__get_leds();

    TEST_ASSERT_EACH_TRUE_LEDC_RGB_LED(led_count, leds, is_initialized == false);
}

static void test__get_led__given_uninitialized_module__throws() {
    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_MODULE_ACCESS, {
        led_store__get_led(_dummy_led_id);
    });
}

static void test_store_led__initialized__every_led_initialized()
{
    led_store__initialize();

    uint8_t led_count = led_store__get_led_count();
    struct ledc_rgb_led_t *leds = led_store__get_leds();

    TEST_ASSERT_EACH_TRUE_LEDC_RGB_LED(led_count, leds, is_initialized == true);
}

static void test_get_led__given_unknown_id__returns_null()
{
    TEST_ASSERT_NULL(led_store__get_led(_unknown_led_id));
}

static void test_get_led__given_known_id__returns_led_reference()
{
    struct ledc_rgb_led_t *expected_leds = led_store__get_leds();
    TEST_ASSERT_NOT_NULL(expected_leds);
    TEST_ASSERT_GREATER_THAN(1, led_store__get_led_count());

    struct ledc_rgb_led_t *expected_led = expected_leds + 1;
    struct ledc_rgb_led_t *actual_led = led_store__get_led(expected_led->id);

    TEST_ASSERT_EQUAL_PTR(expected_led, actual_led);
}

static void test_update__given_uninitialized_module__throws()
{
    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_MODULE_ACCESS, {
        struct ledc_rgb_led_t dummy_led = {};
        led_store__update(dummy_led);
    });
}

static void test_initialize__initializes_module()
{
    TEST_ASSERT_FALSE(led_store__initialized());
    led_store__initialize();
    TEST_ASSERT_TRUE(led_store__initialized());
}

static void test_function_pointer_compatibility()
{
    void *_ = NULL;
    _ = (led_store__get_led_func_t)led_store__get_led;
}

static void run_order_sensitive_tests()
{
    RUN_TEST(test_update__given_uninitialized_module__throws);
    RUN_TEST(test__get_led__given_uninitialized_module__throws);
    RUN_TEST(test_store_led__uninitialized__every_led_uninitialized);
    // this needs to be called last
    RUN_TEST(test_initialize__initializes_module);
}

int main()
{
    UNITY_BEGIN();

    TEST_ASSERT_THROWS_NOT({
        run_order_sensitive_tests();
        RUN_TEST(test_store_led__led_count__returns_configured_rgb_led_count);
        RUN_TEST(test_store_led__initialized__every_led_initialized);
        RUN_TEST(test_get_led__given_unknown_id__returns_null);
        RUN_TEST(test_get_led__given_known_id__returns_led_reference);
        RUN_TEST(test_function_pointer_compatibility);
    });

    return UNITY_END();
}

int app_main()
{
    return main();
}