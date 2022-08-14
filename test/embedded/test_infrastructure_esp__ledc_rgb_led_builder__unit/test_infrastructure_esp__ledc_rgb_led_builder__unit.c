#include "unity.h"
#include "ledc_rgb_led_builder.h"
#include "build_ledc_led_mock.h"

static ledc_timer_config_t _dummy_timer = {};
static struct ledc_rgb_gpio_pins_t _dummy_pins = {};
static struct ledc_rgb_channels_t _dummy_channels = {};
static int _dummy_bool = false;
static char _dummy_string[0] = "";



extern struct ledc_led_t (*_ledc_rgb_led_builder__build_ledc_led)(
    char name[10],
    ledc_timer_config_t timer,
    ledc_channel_t channel,
    uint8_t gpio_pin,
    bool is_common_anode
);


void tearDown() { }
void setUp() {
    // turns this test into a unit test
    _ledc_rgb_led_builder__build_ledc_led = build_ledc_led_mock__build_ledc_led;
    
    build_ledc_led_mock__reset();
}



static struct ledc_rgb_led_t build_named_led(char name[10]) {
    return build_ledc_rgb_led(name, _dummy_timer, _dummy_channels, _dummy_pins, _dummy_bool);
}

static struct ledc_rgb_led_t build_dummy_led() {
    return build_named_led(_dummy_string);
}



void test_build_ledc_rgb_led__returns_initialized() {
    struct ledc_rgb_led_t led =  build_dummy_led();
    TEST_ASSERT_EQUAL(true, led.is_initialized);
}


void test_build_ledc_rgb_led__increases_ids() {
    struct ledc_rgb_led_t first_led = build_dummy_led();
    struct ledc_rgb_led_t second_led = build_dummy_led();

    TEST_ASSERT_GREATER_THAN(first_led.id, second_led.id);
}


void test_build_ledc_rgb_led__builds_single_coloredleds() {
    const uint8_t expected_led_count = 3;

    build_dummy_led();

    TEST_ASSERT_EQUAL_UINT8(expected_led_count, build_ledc_led_mock__get_callcount());    
}


void test_build_ledc_rgb_led__assigns_name() {
    char expected_name[10] = "testname";

    struct ledc_rgb_led_t led = build_named_led(expected_name);

    TEST_ASSERT_EQUAL_STRING(expected_name, led.name);
}


int main() {
    UNITY_BEGIN();

    RUN_TEST(test_build_ledc_rgb_led__returns_initialized);
    RUN_TEST(test_build_ledc_rgb_led__increases_ids);
    RUN_TEST(test_build_ledc_rgb_led__builds_single_coloredleds);
    RUN_TEST(test_build_ledc_rgb_led__assigns_name);
    
    return UNITY_END();
}

int app_main() {
    return main();
}