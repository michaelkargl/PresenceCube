#include "unity.h"
#include "stdint.h"
#include "string.h"
#include "get_led_query_handler.h"
#include "fff.h"
#include "assertion_helpers.h"

// statement of trust that this variable has been defined in another unit.
// make this declaration available in this compilation unit
extern const rgb_led_domain_bag_t* (*get_led_query_handler__led_repository__get_leds)();

DEFINE_FFF_GLOBALS;
FAKE_VALUE_FUNC(const rgb_led_domain_bag_t*, fff_get_leds);

void setUp(void) {
    get_led_query_handler__led_repository__get_leds = fff_get_leds;
}

void tearDown(void) {
    RESET_FAKE(fff_get_leds);
}

static void _expect_led_bag_equals(
    const rgb_led_domain_bag_t *expected_bag,
    const rgb_led_domain_bag_t *actual_bag
)
{   
    TEST_ASSERT_NOT_NULL(expected_bag);
    TEST_ASSERT_NOT_NULL(actual_bag);

    TEST_ASSERT_NOT_NULL(actual_bag->leds);
    TEST_ASSERT_NOT_NULL(expected_bag->leds);

    TEST_ASSERT_EQUAL(expected_bag->count, actual_bag->count);

    for (uint8_t i = 0; i < actual_bag->count; i++)
    {
        rgb_led_domain_t actual_led = actual_bag->leds[i];
        rgb_led_domain_t expected_led = expected_bag->leds[i];
        TEST_ASSERT_EQUAL(expected_led.id, actual_led.id);
    }
}


void test_handle_get_led_query__returns_a_list_of_leds()
{
    rgb_led_domain_t leds[2];
    const rgb_led_domain_bag_t expected_led_bag = {
        .leds = leds,
        .count = ARRAY_LENGTH(leds)
    };

    fff_get_leds_fake.return_val = &expected_led_bag;

    const get_led_query_response_t query_result = handle_get_led_query((get_led_query_t){});
    
    TEST_ASSERT_EQUAL_PTR_ARRAY(&expected_led_bag, &query_result.led_bag, ARRAY_LENGTH(leds));
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_handle_get_led_query__returns_a_list_of_leds);
    return UNITY_END();
}

int app_main()
{
  return main();
}