#include "unity.h"
#include "stdint.h"
#include "string.h"

#include "get_led_query_handler.h"
#include "get_led_query_handler_globals.h"

void setUp(void) { }
void tearDown(void) {
    get_led_query_handler_deinit();
}


static led_domain_bag_t _fake_get_leds_result;

static void _use_fake_get_leds_function(const led_domain_bag_t (*fn_get_leds)())
{
    get_led_query_handler__led_register__get_leds = fn_get_leds;
}

static void _set_fake_get_leds_function_result(led_domain_bag_t led_bag)
{
    _fake_get_leds_result = led_bag;
}

static const led_domain_bag_t _fake_get_leds()
{
    return _fake_get_leds_result;
}

static void _expect_led_bag_equals(const led_domain_bag_t *expected_bag, const led_domain_bag_t *actual_bag)
{
    
    TEST_ASSERT_NOT_NULL(expected_bag);
    TEST_ASSERT_NOT_NULL(actual_bag);

    TEST_ASSERT_NOT_NULL(actual_bag->leds);
    TEST_ASSERT_NOT_NULL(expected_bag->leds);

    TEST_ASSERT_EQUAL(expected_bag->count, actual_bag->count);

    for (uint8_t i = 0; i < actual_bag->count; i++)
    {
        led_domain_t actual_led = actual_bag->leds[i];
        led_domain_t expected_led = expected_bag->leds[i];

        TEST_ASSERT_EQUAL(expected_led.id, actual_led.id);
        TEST_ASSERT_EQUAL(expected_led.initialized, actual_led.initialized);
        TEST_ASSERT_EQUAL(expected_led.brightness_percent, actual_led.brightness_percent);
        TEST_ASSERT_EQUAL_STRING(expected_led.display_name, actual_led.display_name);
    }
}

// This tests the default state of uninitialized modules. To do this properly, this test
// needs to be executed before all others.
void test_get_led_query_handler_initialized__uninitialized__returns_false()
{
    TEST_ASSERT_FALSE(get_led_query_handler_initialized());
}

void test_get_led_query_handler_initialized__initialized__returns_true()
{
    get_led_query_handler_init();
    TEST_ASSERT_TRUE(get_led_query_handler_initialized());
}

void test_get_led_query_handler_initialized__deinitialized__returns_false()
{
    get_led_query_handler_deinit();
    TEST_ASSERT_FALSE(get_led_query_handler_initialized());
}

void test_handle_get_led_query__initialized__returns_a_list_of_leds()
{
    led_domain_t leds[] = {
        {true, 12, 0, "t1"},
        {true, 23, 0, "t2"}};

    const led_domain_bag_t expected_led_bag = {
        .leds = leds,
        .count = sizeof(leds) / sizeof(leds[0])};

    get_led_query_handler_init();
    _use_fake_get_leds_function(_fake_get_leds);
    _set_fake_get_leds_function_result(expected_led_bag);

    const get_led_query_response_t query_result = handle_get_led_query((get_led_query_t){});
    
    _expect_led_bag_equals(&expected_led_bag, &query_result.led_bag);    
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_handle_get_led_query__initialized__returns_a_list_of_leds);
    RUN_TEST(test_get_led_query_handler_initialized__deinitialized__returns_false);
    RUN_TEST(test_get_led_query_handler_initialized__initialized__returns_true);
    RUN_TEST(test_get_led_query_handler_initialized__uninitialized__returns_false);
    return UNITY_END();
}

int app_main()
{
  return main();
}