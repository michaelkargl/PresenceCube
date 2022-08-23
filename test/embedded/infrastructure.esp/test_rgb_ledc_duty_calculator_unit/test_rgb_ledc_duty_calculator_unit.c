#include "unity.h"
#include "ledc_led_duty_calculator.h"
#include "assertion_helpers.h"

static uint32_t _percent_to_duty_cycle(int bit_resolution, float brightness_percent)
{
    uint32_t max_duty = pow(2, bit_resolution);
    float duty = max_duty / 100.0f * brightness_percent;
    return duty;
}


static struct ledc_led_t _build_ledc_led(uint8_t bit_resolution, float initial_brightness_percent)
{
    const uint32_t initial_duty = _percent_to_duty_cycle(bit_resolution, initial_brightness_percent);

    return (struct ledc_led_t){
        .timer = {.duty_resolution = bit_resolution},
        .channel = {.duty = initial_duty},
        .is_initialized = true};
}

// --------------------------------------------------------------------------------------------------

void test_calculate_duty_cycle__given_uninitialized__throws()
{
    struct ledc_led_t uninitialized_led = { };
    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, {
        calculate_duty_cycle(&uninitialized_led, 0);
    });
}

void test_calculate_duty_cycle__given_null__throws()
{
    struct ledc_led_t *null_led = NULL;
    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        calculate_duty_cycle(null_led, 0);
    });
}

// --------------------------------------------------------------------------------------

static void _test_calculate_duty_cycle__given_led__returns_duty_cycle(
    bool is_common_anode,
    uint32_t resolution_in_bit,
    uint8_t percent,
    uint32_t expected)
{
    TEST_PRINTF(
        "%i percent brightness for a %i bit resolution %s led should result in a %i duty cycle.",
        percent,
        resolution_in_bit,
        is_common_anode ? "common anode" : "common cathode",
        expected);

    struct ledc_led_t led = _build_ledc_led(resolution_in_bit, percent);
    led.is_common_anode = is_common_anode;

    int actual = calculate_duty_cycle(&led, percent);

    TEST_ASSERT_EQUAL(expected, actual);
}

static void _test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(
    uint32_t resolution_in_bit,
    uint8_t percent,
    uint32_t expected)
{
    _test_calculate_duty_cycle__given_led__returns_duty_cycle(true, resolution_in_bit, percent, expected);
}

static void _test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(
    uint32_t resolution_in_bit,
    uint8_t percent,
    uint32_t expected)
{
    _test_calculate_duty_cycle__given_led__returns_duty_cycle(false, resolution_in_bit, percent, expected);
}

void test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle()
{
    // expected = floor(max_duty / 100 * percentage)
    _test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(10, 0, 0);
    _test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(10, 50, 512);
    _test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(10, 100, 1024);
    _test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(10, 120, 1024);
}

void test_calculate_duty_cycle__given_common_cathode_led_and_uneven_percentage__duty_cycle_is_rounded_down()
{
    _test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(10, 13, (uint32_t)133.12);
    _test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(10, 77, (uint32_t)788.48);
    _test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(10, 33, (uint32_t)337.92);
}

void test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle()
{
    // expected = max_duty - duty_cycle_common_anode
    _test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(10, 0, 1024);
    _test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(10, 50, 512);
    _test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(10, 100, 0);
    _test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(10, 120, 0);
}

// --------------------------------------------------------------------------------------

void test_calculate_duty_percent__given_null__throws()
{
    struct ledc_led_t *null_led = NULL;
    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        calculate_duty_percent(null_led);
    });
}

void test_calculate_duty_percent__given_uninitialized_input__throws()
{
    struct ledc_led_t unintitialized_led = { };
    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, {
        calculate_duty_percent(&unintitialized_led);
    });
}

void test_calculate_duty_percent__given_led__returns_percentage()
{
    const uint8_t bit_resolution = 4;
    const uint8_t brightness_percent = 50;
    struct ledc_led_t led = _build_ledc_led(bit_resolution, brightness_percent);

    float duty_percent = calculate_duty_percent(&led);

    TEST_ASSERT_EQUAL(brightness_percent, duty_percent);
}

// --------------------------------------------------------------------------------------

void test_function_pointer_types_match_api() {
    calculate_duty_percent_func_t _calculate_duty_percent = calculate_duty_cycle;
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_calculate_duty_cycle__given_uninitialized__throws);
    RUN_TEST(test_calculate_duty_cycle__given_null__throws);
    RUN_TEST(test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle);
    RUN_TEST(test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle);
    RUN_TEST(test_calculate_duty_cycle__given_common_cathode_led_and_uneven_percentage__duty_cycle_is_rounded_down);

    RUN_TEST(test_calculate_duty_percent__given_null__throws);
    RUN_TEST(test_calculate_duty_percent__given_uninitialized_input__throws);
    RUN_TEST(test_calculate_duty_percent__given_led__returns_percentage);

    RUN_TEST(test_function_pointer_types_match_api);

    return UNITY_END();
}

int app_main()
{
    return main();
}