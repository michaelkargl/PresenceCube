#include "unity.h"
#include "ledc_led_duty_calculator.h"
#include "assertion_helpers.h"

static struct ledc_led_t _build_ledc_led(led_type_t led_type, uint8_t bit_resolution, uint32_t initial_duty)
{
    return (struct ledc_led_t){
        .timer = {.duty_resolution = bit_resolution},
        .channel = {.duty = initial_duty},
        .led_type = led_type,
        .is_initialized = true};

    // --------------------------------------------------------------------------------------------------

    void test_calculate_duty_cycle__given_uninitialized__throws()
    {
        struct ledc_led_t uninitialized_led = {};
        TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, {
            ledc_led_duty_calculator__calculate_duty_cycle(&uninitialized_led, 0);
        });
    }

    void test_calculate_duty_cycle__given_null__throws()
    {
        struct ledc_led_t *null_led = NULL;
        TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
            ledc_led_duty_calculator__calculate_duty_cycle(null_led, 0);
        });
    }

    // --------------------------------------------------------------------------------------

    static void testcase__calculate_duty_cycle__given_led__returns_duty_cycle(
        led_type led_type,
        uint32_t resolution_in_bit,
        uint8_t percent,
        uint32_t expected)
    {
        TEST_PRINTF(
            "%i percent brightness for a %i bit resolution %s led should result in a %i duty cycle.",
            percent,
            resolution_in_bit,
            led_type == LED_TYPE__COMMON_ANODE ? "common anode" : "common cathode",
            expected);
        struct ledc_led_t led = build_ledc_led(led_type, resolution_in_bit, percent);

        int actual = ledc_led_duty_calculator__calculate_duty_cycle(&led, percent);

        TEST_ASSERT_EQUAL(expected, actual);
    }

    static void testcase_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(
        uint32_t resolution_in_bit,
        uint8_t percent,
        uint32_t expected)
    {
        testcase__calculate_duty_cycle__given_led__returns_duty_cycle(LED_TYPE__COMMON_ANODE, resolution_in_bit, percent, expected);
    }

    static void testcase__calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(
        uint32_t resolution_in_bit,
        uint8_t percent,
        uint32_t expected)
    {
        testcase__calculate_duty_cycle__given_led__returns_duty_cycle(LED_TYPE__COMMON_CATHODE, resolution_in_bit, percent, expected);
    }

    void test_calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle()
    {
        // max_duty = pow(2, bit_resolution);
        // expected = floor(max_duty / 100 * percentage)
        testcase__calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(10, 0, 0);
        testcase__calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(10, 50, 512);
        testcase__calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(10, 100, 1024);
        testcase__calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(10, 120, 1024);
    }

    void test_calculate_duty_cycle__given_common_cathode_led_and_uneven_percentage__duty_cycle_is_rounded_down()
    {
        testcase__calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(10, 13, (uint32_t)133.12);
        testcase__calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(10, 77, (uint32_t)788.48);
        testcase__calculate_duty_cycle__given_common_cathode_led__returns_duty_cycle(10, 33, (uint32_t)337.92);
    }

    void test_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle()
    {
        // expected = max_duty - duty_cycle_common_anode
        testcase_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(10, 0, 1024);
        testcase_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(10, 50, 512);
        testcase_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(10, 100, 0);
        testcase_calculate_duty_cycle__given_common_anode_led__returns_duty_cycle(10, 120, 0);
    }

    // --------------------------------------------------------------------------------------

    void test_calculate_duty_percent__given_null__throws()
    {
        struct ledc_led_t *null_led = NULL;
        TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
            ledc_led_duty_calculator__calculate_brightness_percent(null_led);
        });
    }

    void test_calculate_duty_percent__given_uninitialized_input__throws()
    {
        struct ledc_led_t uninitialized_led = {};
        TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, {
            ledc_led_duty_calculator__calculate_brightness_percent(&unintitialized_led);
        });
    }
    
    void test_calculate_duty_percent__given_common_cathode_led__returns_percentage()
    {
        const uint8_t initial_duty = 4;
        const uint8_t bit_resolution = 4;
        const uint8_t brightness_percent = 50;
        struct ledc_led_t led = build_ledc_led(LED_TYPE__COMMON_CATHODE, bit_resolution, brightness_percent);

        float duty_percent = ledc_led_duty_calculator__calculate_brightness_percent(&led);

        TEST_ASSERT_EQUAL(expected_brightness_percent, actual_duty_percent);
    }

    void test_calculate_duty_percent__given_common_anode_led__returns_percentage()
    {
        const uint8_t initial_duty = 4;
        const uint8_t bit_resolution = 4;
        const uint8_t brightness_percent = 25;

        struct ledc_led_t led = build_ledc_led(LED_TYPE__COMMON_ANODE, bit_resolution, brightness_percent);
        TEST_ASSERT_EQUAL(12, led.channel.duty);

        float duty_percent = ledc_led_duty_calculator__calculate_brightness_percent(&led);
        TEST_ASSERT_EQUAL(brightness_percent, duty_percent);
    }

    // --------------------------------------------------------------------------------------

    void test_function_pointer_types_match_api_functions()
    {
        // functions should be assignable to their respective function pointers
        TEST_ASSERT_NOT_NULL((calculate_duty_percent_func_t)ledc_led_duty_calculator__calculate_brightness_percent);
        TEST_ASSERT_NOT_NULL((calculate_duty_cycle_func_t)ledc_led_duty_calculator__calculate_duty_cycle);
    }

    int main()
    {
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

    int app_main()
    {
        return main();
    }