#include "unity.h"
#include "led_store.h"
#include "build_ledc_rgb_led_stub.h"
#include "assertion_helpers.h"

#define TEST_ASSERT_EACH_TRUE_LEDC_RGB_LED(led_count, leds, condition) \
    do                                                            \
    {                                                             \
        for (uint8_t i = 0; i < led_count; i++)                   \
        {                                                         \
            struct ledc_rgb_led_t led = leds[i];                  \
            TEST_ASSERT_TRUE(led.condition);                      \
        }                                                         \
    } while (0);


extern struct ledc_rgb_led_t (*_led_store__build_ledc_rgb_led)(
    const char name[10],
    const ledc_timer_config_t timer,
    const struct ledc_rgb_channels_t channels,
    const struct ledc_rgb_gpio_pins_t gpio_pins,
    bool is_common_anode
);


void tearDown() {}
void setUp() {
    _led_store__build_ledc_rgb_led = stub__build_ledc_rgb_led;
}


void test_store_led__led_count__returns_configured_rgb_led_count()
{
    uint8_t led_count = led_store__get_led_count();
    TEST_ASSERT_EQUAL(CONFIG_CUBE_HARDWARE_RGB_LED_COUNT, led_count);
}

void test_store_led__uninitialized__every_led_uninitialized()
{
    uint8_t led_count = led_store__get_led_count();
    struct ledc_rgb_led_t *leds = led_store__get_leds();

    TEST_ASSERT_EACH_TRUE_LEDC_RGB_LED(led_count, leds, is_initialized == false);
}

void test_store_led__initialized__every_led_initialized()
{
    led_store_initialize();

    uint8_t led_count = led_store__get_led_count();
    struct ledc_rgb_led_t *leds = led_store__get_leds();

    TEST_ASSERT_EACH_TRUE_LEDC_RGB_LED(led_count, leds, is_initialized == true);
}

int main()
{
    UNITY_BEGIN();

    TEST_ASSERT_THROWS_NOT({
        RUN_TEST(test_store_led__led_count__returns_configured_rgb_led_count);
        RUN_TEST(test_store_led__uninitialized__every_led_uninitialized);
        RUN_TEST(test_store_led__initialized__every_led_initialized);
    });

    return UNITY_END();
}

int app_main()
{
    return main();
}