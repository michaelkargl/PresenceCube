#include "unity.h"
#include "set_led_repository.h"
#include "assertion_helpers.h"
#include "led_store.h"
#include "fff.h"

DEFINE_FFF_GLOBALS
FAKE_VOID_FUNC1(fff_update_led, rgb_led_domain_t);
FAKE_VALUE_FUNC1(const rgb_led_domain_t *, fff_get_led, uint8_t);

extern led_store__update_func_t set_led_repository__led_store__update_led;
extern led_store__get_led_func_t set_led_repository__led_store__get_led;

static const uint8_t _known_led_id = 1;
static const uint8_t _unknown_led_id = 255;
static const uint8_t _dummy_id = 255;
static const uint8_t _dummy_color = 50;

static rgb_led_domain_t valid_led = {
    .is_initialized = true,
    .id = _known_led_id,
    .red = {.id = 10, .brightness_percent = 10},
    .green = {.id = 20, .brightness_percent = 20},
    .blue = {.id = 30, .brightness_percent = 30}};

void setUp()
{
    set_led_repository__led_store__update_led = fff_update_led;
    set_led_repository__led_store__get_led = fff_get_led;
    fff_get_led_fake.return_val = &valid_led;
}

void tearDown()
{
    RESET_FAKE(fff_update_led);
    RESET_FAKE(fff_get_led);
}

static void test_update_led__updates_stored_entity()
{
    set_led_repository__update(_known_led_id, 10, 20, 30);

    TEST_ASSERT_EQUAL(ONCE, fff_update_led_fake.call_count);
    TEST_ASSERT_EQUAL(_known_led_id, fff_update_led_fake.arg0_val.id);
    TEST_ASSERT_EQUAL(10, fff_update_led_fake.arg0_val.red.brightness_percent);
    TEST_ASSERT_EQUAL(20, fff_update_led_fake.arg0_val.green.brightness_percent);
    TEST_ASSERT_EQUAL(30, fff_update_led_fake.arg0_val.blue.brightness_percent);
}

static void test_update_led__given_invalid_id__throws()
{
    fff_get_led_fake.return_val = NULL;
    TEST_ASSERT_THROWS(ERROR_CODE_RESOURCE_NOT_FOUND, {
        set_led_repository__update(_unknown_led_id, _dummy_color, _dummy_color, _dummy_color);
    });
}

static void test_function_pointer_compatibility() {
    (set_led_repository__update_led_func_t)set_led_repository__led_store__update_led;
}

int main()
{
    UNITY_BEGIN();

    TEST_ASSERT_THROWS_NOT({
        RUN_TEST(test_update_led__updates_stored_entity);
        RUN_TEST(test_update_led__given_invalid_id__throws);
        RUN_TEST(test_function_pointer_compatibility);
    });

    return UNITY_END();
}

int app_main()
{
    return main();
}