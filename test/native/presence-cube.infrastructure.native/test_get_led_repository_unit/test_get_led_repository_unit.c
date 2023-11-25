#include "unity.h"
#include "get_led_repository.h"
#include "exception_handling.h"
#include "fff.h"
#include "led_store_func.h"

extern led_store__get_led_func_t get_led_repository__get_led_fn;
extern led_store__get_leds_func_t get_led_repository__get_leds_fn;

DEFINE_FFF_GLOBALS
FAKE_VALUE_FUNC1(const rgb_led_domain_t *, fff_get_led, uint8_t);
FAKE_VALUE_FUNC0(const rgb_led_domain_bag_t *, fff_get_leds);

static uint8_t unknown_led_id = 255;
static uint8_t known_led_id = 1;
static const rgb_led_domain_bag_t _known_leds = {
    .count = 2,
    .leds = (rgb_led_domain_t[2]){
        // to keep things simpler: index == id
        {.id = 0, .display_name = "t0"},
        {.id = 1, .display_name = "t1"}}};

static const rgb_led_domain_t *get_known_led()
{
    return &_known_leds.leds[known_led_id];
}

void setUp()
{
    get_led_repository__get_led_fn = fff_get_led;
    fff_get_led_fake.return_val = &_known_leds.leds[1];

    get_led_repository__get_leds_fn = fff_get_leds;
    fff_get_leds_fake.return_val = &_known_leds;
}

void tearDown()
{
    RESET_FAKE(fff_get_leds);
}

void test_function_pointer_compatibility_with_exposed_api()
{
    (get_led_repository__get_led_func_t) get_led_repository__get_led_fn;
    (get_led_repository__get_leds_func_t) get_led_repository__get_leds_fn;
}

void test_get_led__given_invalid_id__returns_null()
{
    fff_get_led_fake.return_val = NULL;
    const rgb_led_domain_t *result = get_led_repository__get_led(unknown_led_id);
    TEST_ASSERT_NULL(result);
}

void test_get_led__given_valid_id__returns_led()
{
    const rgb_led_domain_t *known_led = get_known_led();
    const rgb_led_domain_t *result = get_led_repository__get_led(known_led->id);

    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_PTR(known_led, result);
    TEST_ASSERT_EQUAL(1, result->id);
    TEST_ASSERT_EQUAL_STRING("t1", result->display_name);
}

void test_get_leds__returns_leds()
{
    const rgb_led_domain_bag_t *result = get_led_repository__get_leds();
    TEST_ASSERT_EQUAL_PTR(&_known_leds, result);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_function_pointer_compatibility_with_exposed_api);
    RUN_TEST(test_get_led__given_invalid_id__returns_null);
    RUN_TEST(test_get_led__given_valid_id__returns_led);
    RUN_TEST(test_get_leds__returns_leds);

    return UNITY_END();
}

int app_main()
{
    return main();
}