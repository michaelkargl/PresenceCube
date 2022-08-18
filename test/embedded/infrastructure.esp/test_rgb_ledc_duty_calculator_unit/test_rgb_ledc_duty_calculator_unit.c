#include "unity.h"
#include "rgb_ledc_duty_calculator.h"
#include "CException.h"


void test_get_max_duty__given_uninitialized_led__throws_uninitialized_data_access_exception() {
    struct ledc_led_t led = { };
    CEXCEPTION_T ex;
    Try {
        get_max_duty(&led);
        TEST_FAIL_MESSAGE("Expected uninitialized data access exception but none was caught.");
    } Catch(ex) { }

    TEST_ASSERT_EQUAL(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, ex);
}

void test_get_max_duty__given_null_led__throws_uninitialized_data_access_exception() {
    CEXCEPTION_T ex;
    Try {
        get_max_duty(NULL);
        TEST_FAIL_MESSAGE("Expected null reference exception but none was caught.");
    } Catch(ex) { }

    TEST_ASSERT_EQUAL(ERROR_CODE_ARGUMENT_NULL, ex);
}

// --------------------------------------------------------------------------------------

static void _test_get_max_duty__given_initialized_led__returns_maximum_viable_duty_cycle(
    uint32_t resolution_in_bit, 
    uint32_t expected
) {
    struct ledc_led_t led = {
        .is_initialized = true,
        .timer = {
            .duty_resolution = resolution_in_bit
        }
    };
    
    uint32_t actual = get_max_duty(&led);

    TEST_ASSERT_EQUAL(expected, actual);
}


void test_get_max_duty__given_initialized_led__returns_maximum_viable_duty_cycle() {
    _test_get_max_duty__given_initialized_led__returns_maximum_viable_duty_cycle(0, 1);
    _test_get_max_duty__given_initialized_led__returns_maximum_viable_duty_cycle(1, 2);
    _test_get_max_duty__given_initialized_led__returns_maximum_viable_duty_cycle(5, 32);
    _test_get_max_duty__given_initialized_led__returns_maximum_viable_duty_cycle(10, 1024);
    _test_get_max_duty__given_initialized_led__returns_maximum_viable_duty_cycle(LEDC_TIMER_BIT_MAX, pow(2, LEDC_TIMER_BIT_MAX));
}

// --------------------------------------------------------------------------------------

void test_calculate_duty__given_uninitialized_led__throws_uninitialized_data_access_exception() {
    struct ledc_led_t led = { };
    CEXCEPTION_T ex;
    Try {
        calculate_duty(&led, 0);
        TEST_FAIL_MESSAGE("Expected uninitialized data access exception but none was caught.");
    } Catch(ex) { }

    TEST_ASSERT_EQUAL(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, ex);
}

void test_calculate_duty__given_null_led__throws_uninitialized_data_access_exception() {
    CEXCEPTION_T ex;
    Try {
        calculate_duty(NULL, 0);
        TEST_FAIL_MESSAGE("Expected null reference exception but none was caught.");
    } Catch(ex) { }

    TEST_ASSERT_EQUAL(ERROR_CODE_ARGUMENT_NULL, ex);
}

// --------------------------------------------------------------------------------------

static void _test_calculate_duty__given_led__returns_duty_cycle(
    bool is_common_anode,
    uint32_t resolution_in_bit,
    uint8_t percent,
    uint32_t expected
) {
    TEST_PRINTF(
        "%i percent brightness for a %i bit resolution %s led should result in a %i duty cycle.",
        percent,
        resolution_in_bit,
        is_common_anode ? "common anode" : "common cathode",
        expected
    );

    struct ledc_led_t led = {
        .is_initialized = true,
        .is_common_anode = is_common_anode,
        .timer = {
            .duty_resolution = resolution_in_bit
        }
    };
    
    int actual = calculate_duty(&led, percent);

    TEST_ASSERT_EQUAL(expected, actual);
}

static void _test_calculate_duty__given_common_anode_led__returns_duty_cycle(
    uint32_t resolution_in_bit, 
    uint8_t percent,
    uint32_t expected
) {
    _test_calculate_duty__given_led__returns_duty_cycle(true, resolution_in_bit, percent, expected);
}

static void _test_calculate_duty__given_common_cathode_led__returns_duty_cycle(
    uint32_t resolution_in_bit, 
    uint8_t percent,
    uint32_t expected
) {
    _test_calculate_duty__given_led__returns_duty_cycle(false, resolution_in_bit, percent, expected);
}

void test_calculate_duty__given_common_cathode_led__returns_duty_cycle() {
    // expected = floor(max_duty / 100 * percentage)
    _test_calculate_duty__given_common_cathode_led__returns_duty_cycle(10, 0, 0);
    _test_calculate_duty__given_common_cathode_led__returns_duty_cycle(10, 50, 512);
    _test_calculate_duty__given_common_cathode_led__returns_duty_cycle(10, 100, 1024);
    _test_calculate_duty__given_common_cathode_led__returns_duty_cycle(10, 120, 1024);
}


void test_calculate_duty__given_common_cathode_led_and_uneven_percentage__duty_cycle_is_rounded_down() {
    _test_calculate_duty__given_common_cathode_led__returns_duty_cycle(10, 13, (uint32_t)133.12);
    _test_calculate_duty__given_common_cathode_led__returns_duty_cycle(10, 77, (uint32_t)788.48);
    _test_calculate_duty__given_common_cathode_led__returns_duty_cycle(10, 33, (uint32_t)337.92);
}

void test_calculate_duty__given_common_anode_led__returns_duty_cycle() {
    // expected = max_duty - duty_cycle_common_anode
    _test_calculate_duty__given_common_anode_led__returns_duty_cycle(10, 0, 1024);
    _test_calculate_duty__given_common_anode_led__returns_duty_cycle(10, 50, 512);
    _test_calculate_duty__given_common_anode_led__returns_duty_cycle(10, 100, 0);
    _test_calculate_duty__given_common_anode_led__returns_duty_cycle(10, 120, 0);
}

// --------------------------------------------------------------------------------------

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_get_max_duty__given_initialized_led__returns_maximum_viable_duty_cycle);
    RUN_TEST(test_get_max_duty__given_uninitialized_led__throws_uninitialized_data_access_exception);
    RUN_TEST(test_get_max_duty__given_null_led__throws_uninitialized_data_access_exception);
    
    RUN_TEST(test_calculate_duty__given_uninitialized_led__throws_uninitialized_data_access_exception);
    RUN_TEST(test_calculate_duty__given_null_led__throws_uninitialized_data_access_exception);
    RUN_TEST(test_calculate_duty__given_common_anode_led__returns_duty_cycle);
    RUN_TEST(test_calculate_duty__given_common_cathode_led__returns_duty_cycle);
    RUN_TEST(test_calculate_duty__given_common_cathode_led_and_uneven_percentage__duty_cycle_is_rounded_down);

    return UNITY_END();
}

int app_main() {
    return main();
}