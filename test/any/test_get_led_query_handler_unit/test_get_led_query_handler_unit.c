#include "unity.h"
#include "stdint.h"
#include "string.h"
#include "get_led/get_led_query_handler.h"
#include "fff.h"
#include "assertion_helpers.h"

extern const rgb_led_diode_bag_t *(*get_led_query_handler__led_repository__get_leds)();

static const rgb_led_diode_bag_t _valid_led_bag = {
    .count = 2,
    .leds = (rgb_led_diode_t[]){
        {.id = 0, .display_name = "t1"},
        {.id = 1, .display_name = "t2"},
    }};

DEFINE_FFF_GLOBALS;
FAKE_VALUE_FUNC(const rgb_led_diode_bag_t *, fff_get_leds);

void setUp(void)
{
    get_led_query_handler__led_repository__get_leds = fff_get_leds;
}

void tearDown(void)
{
    RESET_FAKE(fff_get_leds);
}

void test_handle_get_led_query__returns_a_list_of_leds()
{
    fff_get_leds_fake.return_val = &_valid_led_bag;

    const get_led_query_response_t query_result = get_led_query_handler__handle((get_led_query_t){});
    
    TEST_ASSERT_NOT_NULL(query_result.led_bag);
    TEST_ASSERT_EQUAL(_valid_led_bag.count, query_result.led_bag->count);
    TEST_ASSERT_EQUAL_PTR(_valid_led_bag.leds, query_result.led_bag->leds);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_handle_get_led_query__returns_a_list_of_leds);
    return UNITY_END();
}

int app_main()
{
    return main();
}