#include "unity.h"
#include "ledc_led_builder.h"


static ledc_timer_config_t _dummy_timer = {};
static ledc_channel_t _dummy_channel = LEDC_CHANNEL_0;
static int _dummy_pin = 0;
static int _dummy_bool = false;
static char _dummy_string[0] = "";


static struct ledc_led_t _build_dummy_led() {
    return build_ledc_led(_dummy_string, _dummy_timer, _dummy_channel, _dummy_pin, _dummy_bool);
}

static struct ledc_led_t _build_named_led(char name[10]) {
    return build_ledc_led(name, _dummy_timer, _dummy_channel, _dummy_pin, _dummy_bool);
}

void test_build_ledc_led__returns_initialized() {
    
    struct ledc_led_t led = _build_dummy_led();
    TEST_ASSERT_EQUAL(led.is_initialized, true);
}

void test_build_ledc_led__led_receives_new_id() {
    struct ledc_led_t first_led  = _build_dummy_led();
    struct ledc_led_t second_led = _build_dummy_led();
    
    TEST_ASSERT_GREATER_OR_EQUAL(0, first_led.id);
    TEST_ASSERT_GREATER_THAN(first_led.id, second_led.id);
}

void test_build_ledc_led__led_has_name() {
    char expected_name[10] = "testname";
    
    struct ledc_led_t led  = _build_named_led(expected_name);

    TEST_ASSERT_EQUAL_STRING(expected_name, led.name);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_build_ledc_led__returns_initialized);
    RUN_TEST(test_build_ledc_led__led_receives_new_id);
    RUN_TEST(test_build_ledc_led__led_has_name);

    UNITY_END();
}

int app_main() {
    return main();
}