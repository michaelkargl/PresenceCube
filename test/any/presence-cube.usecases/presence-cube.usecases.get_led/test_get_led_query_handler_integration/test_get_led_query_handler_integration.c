#include "unity.h"
#include "get_led_repository.h"
#include "get_led_query_handler.h"
#include "led_store.h"

void setUp()
{
    led_store__initialize();
}

void test_handle()
{
    const rgb_led_diode_bag_t *stored_leds = get_led_repository__get_leds();
    TEST_ASSERT_NOT_NULL(stored_leds);

    const get_led_query_response_t response = get_led_query_handler__handle((get_led_query_t){});
    
    TEST_ASSERT_NOT_NULL(response.led_bag);
    TEST_ASSERT_TRUE(response.led_bag->is_initialized);
    TEST_ASSERT_EQUAL(stored_leds->count, response.led_bag->count);
    TEST_ASSERT_NOT_NULL(response.led_bag->leds);
    TEST_ASSERT_EQUAL(0, stored_leds->leds[0].id);
    TEST_ASSERT_EQUAL(1, stored_leds->leds[1].id);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_handle);

    return UNITY_END();
}

int app_main()
{
    return main();
}