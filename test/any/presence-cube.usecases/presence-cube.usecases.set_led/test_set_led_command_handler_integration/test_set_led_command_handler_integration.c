#include "unity.h"
#include "set_led/set_led_command_handler.h"
#include "get_led_repository.h"
#include "exception_assertion_helper.h"
#include "rgb_ledc_adapter.h"
#include "led_store.h"

static const uint8_t _known_led_id = 0;
static const uint8_t _unknown_led_id = 255;

void setUp()
{
    rgb_ledc_adapter__initialize();
}

void test_handle__success()
{
    set_led_command_t command = {
        .id = _known_led_id,
        .red = 25,
        .green = 50,
        .blue = 75};

    const rgb_led_diode_t *led = get_led_repository__get_led(_known_led_id);
    TEST_ASSERT_NOT_NULL(led);
    TEST_ASSERT_NOT_EQUAL(command.red, led->red.brightness_percent);
    TEST_ASSERT_NOT_EQUAL(command.green, led->green.brightness_percent);
    TEST_ASSERT_NOT_EQUAL(command.blue, led->blue.brightness_percent);

    set_led_command_response_t response = set_led_command_handler__handle(command);

    led = get_led_repository__get_led(_known_led_id);
    TEST_ASSERT_NOT_NULL(led);
    TEST_ASSERT_EQUAL_MESSAGE(command.red, led->red.brightness_percent, "red");
    TEST_ASSERT_EQUAL_MESSAGE(command.green, led->green.brightness_percent, "green");
    TEST_ASSERT_EQUAL_MESSAGE(command.blue, led->blue.brightness_percent, "blue");
    TEST_ASSERT_EQUAL_UINT8(command.id, response.id);
}

void test_handle__given_unknown_id__throws_not_found_exception()
{
    TEST_ASSERT_THROWS(ERROR_CODE_RESOURCE_NOT_FOUND, {
        set_led_command_handler__handle((set_led_command_t){
            .id = _unknown_led_id});
    });
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_handle__success);
    RUN_TEST(test_handle__given_unknown_id__throws_not_found_exception);

    return UNITY_END();
}

int app_main()
{
    return main();
}