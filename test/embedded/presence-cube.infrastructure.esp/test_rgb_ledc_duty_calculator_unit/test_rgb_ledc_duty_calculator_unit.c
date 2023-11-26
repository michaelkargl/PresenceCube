#include "unity.h"
#include "ledc_led_duty_calculator.h"
#include "assertion_helpers.h"


static struct ledc_led_t _build_ledc_led(bool common_anode, uint8_t bit_resolution, uint32_t initial_duty) {
    return (struct ledc_led_t) {
            .timer = {.duty_resolution = bit_resolution},
            .channel = {.duty = initial_duty},
            .is_common_anode = common_anode,
            .is_initialized = true};
}

// --------------------------------------------------------------------------------------------------

void test_calculate_duty_cycle__given_uninitialized__throws() {
    struct ledc_led_t uninitialized_led = {};
    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, {
        calculate_duty_cycle(&uninitialized_led, 0);
    });
}

void test_calculate_duty_cycle__given_null__throws() {
    struct ledc_led_t *null_led = NULL;
    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        calculate_duty_cycle(null_led, 0);
    });
}

// --------------------------------------------------------------------------------------

static void _test_calculate_duty_cycle__given_led__returns_duty_cycle(
        bool is_common_anode,
        const uint32_t resolution_in_bit,
        uint8_t percent,
        uint32_t expected) {
    TEST_PRINTF(
            "%i percent brightness for a %i bit resolution %s led should result in a %i duty cycle.",
            percent,
            resolution_in_bit,
            is_common_anode ? "common anode" : "common cathode",
            expected);

    struct ledc_led_t led = _build_ledc_led(is_common_anode, resolution_in_bit, 0);
    int actual = calculate_duty_cycle(&led, percent);

    TEST_ASSERT_EQUAL(expected, actual);
}

static void _test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(
        uint32_t resolution_in_bit,
        uint8_t percent,
        uint32_t expected) {
    _test_calculate_duty_cycle__given_led__returns_duty_cycle(true, resolution_in_bit, percent, expected);
}

static void _test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(
        uint32_t resolution_in_bit,
        uint8_t percent,
        uint32_t expected) {
    _test_calculate_duty_cycle__given_led__returns_duty_cycle(false, resolution_in_bit, percent, expected);
}

void test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle() {
    // max_duty = pow(2, bit_resolution);
    // expected = floor(max_duty / 100 * percentage)

    // even percentages
    _test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(8, 0, 0);
    _test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(8, 50, 128);
    _test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(8, 100, 256);
    _test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(8, 120, 256);
    // uneven percentages
    _test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(9, 13, (uint32_t) 66.56);
    _test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(9, 33, (uint32_t) 168.96);
    _test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(9, 77, (uint32_t) 394.24);
    _test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(9, 133, 512);
}


void test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle() {
    // max_duty = pow(2, bit_resolution);
    // expected = max_duty - duty_cycle_common_cathode

    // even percentages
    _test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(10, 0, 1024);
    _test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(10, 50, 512);
    _test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(10, 100, 0);
    _test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(10, 120, 0);
    // uneven percentages
    _test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(11, 13, (uint32_t) 1781.76);
    _test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(11, 33, (uint32_t) 1372.16);
    _test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(11, 77, (uint32_t) 471.04);
    _test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(11, 133, 0);
}

// --------------------------------------------------------------------------------------

void test_calculate_duty_percent__given_null__throws() {
    struct ledc_led_t *null_led = NULL;
    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        calculate_brightness_percent(null_led);
    });
}

void test_calculate_duty_percent__given_uninitialized_input__throws() {
    struct ledc_led_t uninitialized_led = {};
    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, {
        calculate_brightness_percent(&uninitialized_led);
    });
}

void test_calculate_duty_percent__given_common_cathode_led__returns_percentage() {
    const uint8_t initial_duty = 4;
    const uint8_t bit_resolution = 4;
    const uint8_t expected_brightness_percent = 25;
    struct ledc_led_t led = _build_ledc_led(false, bit_resolution, initial_duty);

    float actual_duty_percent = calculate_brightness_percent(&led);

    TEST_ASSERT_EQUAL(expected_brightness_percent, actual_duty_percent);
}

void test_calculate_duty_percent__given_common_anode_led__returns_percentage() {
    const uint8_t initial_duty = 4;
    const uint8_t bit_resolution = 4;
    const uint8_t expected_brightness_percent = 75;
    struct ledc_led_t led = _build_ledc_led(true, bit_resolution, initial_duty);

    float actual_duty_percent = calculate_brightness_percent(&led);

    TEST_ASSERT_EQUAL(expected_brightness_percent, actual_duty_percent);
}

// --------------------------------------------------------------------------------------

void test_function_pointer_types_match_api_functions() {
    // sanity test to ensure functions are assignable to their respective function pointers
    TEST_ASSERT_NOT_NULL((calculate_duty_percent_func_t) calculate_brightness_percent);
    TEST_ASSERT_NOT_NULL((calculate_duty_cycle_func_t) calculate_duty_cycle);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_calculate_duty_cycle__given_uninitialized__throws);
    RUN_TEST(test_calculate_duty_cycle__given_null__throws);
    RUN_TEST(test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle);
    RUN_TEST(test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle);

    RUN_TEST(test_calculate_duty_percent__given_null__throws);
    RUN_TEST(test_calculate_duty_percent__given_uninitialized_input__throws);
    RUN_TEST(test_calculate_duty_percent__given_common_cathode_led__returns_percentage);
    RUN_TEST(test_calculate_duty_percent__given_common_anode_led__returns_percentage);

    RUN_TEST(test_function_pointer_types_match_api_functions);

    return UNITY_END();
}

int app_main() {
    return main();
}