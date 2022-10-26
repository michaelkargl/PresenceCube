#include "unity.h"
#include "get_led_repository.h"
#include "assertion_helpers.h"
#include "led_store.h"

void setUp() {
    led_store__initialize();
}
void tearDown() {}


void test_get_leds__returned_bag_has_size_of_configured_rgb_led_count()
{
    const rgb_led_domain_bag_t* led_bag = get_led_repository__get_leds();
    TEST_ASSERT_NOT_NULL(led_bag);
    TEST_ASSERT_EQUAL(CONFIG_CUBE_HARDWARE_RGB_LED_COUNT, led_bag->count);
}

void test_get_leds__all_leds_are_initialized()
{
    const rgb_led_domain_bag_t* led_bag = get_led_repository__get_leds();
    TEST_ASSERT_NOT_NULL(led_bag);
    TEST_ASSERT_NOT_NULL(led_bag->leds);
    TEST_ASSERT_GREATER_THAN(0, led_bag->count);
    TEST_ASSERT_TRUE(led_bag->is_initialized);

    for(uint8_t i = 0; i < led_bag->count; i++) {
        rgb_led_domain_t led = led_bag->leds[i];
        TEST_ASSERT_TRUE(led.is_initialized);
        TEST_ASSERT_GREATER_THAN(0, led.id);
        TEST_ASSERT_GREATER_THAN(0, led.id);
        TEST_ASSERT_GREATER_THAN(0, led.red.id);
        TEST_ASSERT_GREATER_THAN(0, led.green.id);
        TEST_ASSERT_GREATER_THAN(0, led.blue.id);
    }
}

int main()
{
    UNITY_BEGIN();

    TEST_ASSERT_THROWS_NOT({
        RUN_TEST(test_get_leds__returned_bag_has_size_of_configured_rgb_led_count);
        RUN_TEST(test_get_leds__all_leds_are_initialized);
    });

    return UNITY_END();
}

int app_main()
{
    return main();
}