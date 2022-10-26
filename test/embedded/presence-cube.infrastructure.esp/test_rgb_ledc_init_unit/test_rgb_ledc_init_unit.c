#include "unity.h"
#include "rgb_ledc_init.h"
#include "stubs/configure_led_mock.h"
#include "exception_handling.h"

#define RGB_LED_CHANNEL_COUNT 3

extern void (*_rgb_ledc_init__configure_led)(const struct ledc_led_t *);


void tearDown() { }
void setUp() {
    _rgb_ledc_init__configure_led = configure_led_mock;
    configure_led_mock__reset();
}


void test_configure_rgb_led__given_null_input__throws_argument_null_exception() {
    CEXCEPTION_T ex;
    Try {
        configure_rgb_led(NULL);
        TEST_FAIL_MESSAGE("Expected argument null exception but none received.");
    } Catch(ex) { }

    TEST_ASSERT_EQUAL(ERROR_CODE_ARGUMENT_NULL, ex);
}

void test_configure_rgb_led__given_an_uninitialized_led__throws_uninitialized_access_exception() {
    struct ledc_rgb_led_t dummy_led = {};
    CEXCEPTION_T ex;
    Try {
        configure_rgb_led(&dummy_led);
        TEST_FAIL_MESSAGE("Expected argument null exception but none received.");
    } Catch(ex) { }

    TEST_ASSERT_EQUAL(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, ex);
}

void test_configure_rgb_led__given_valid_led__configures_all_rgb_channel_leds() {
    struct ledc_rgb_led_t dummy_led = { .is_initialized = true };
    
    configure_rgb_led(&dummy_led);
    
    TEST_ASSERT_EQUAL(RGB_LED_CHANNEL_COUNT, configure_led_mock__get_callcount());
}


void test_configure_rgb_leds__given_null_array__throws_argument_null_exception() {
    CEXCEPTION_T ex;
    Try {
        configure_rgb_leds(NULL, 0);
        TEST_FAIL_MESSAGE("Expected argument null exception but none received.");
    } Catch(ex) { }

    TEST_ASSERT_EQUAL(ERROR_CODE_ARGUMENT_NULL, ex);
}

void test_configure_rgb_leds__given_initialized_array__configures_all_rgb_channels_of_all_leds() {   
    struct ledc_rgb_led_t leds[] = {
        { .is_initialized = true },
        { .is_initialized = true }
    };

    uint8_t led_count = sizeof(leds)/sizeof(leds[0]);
    configure_rgb_leds(leds, led_count);

    uint8_t expected_channel_led_count = RGB_LED_CHANNEL_COUNT * led_count;
    TEST_ASSERT_EQUAL(expected_channel_led_count, configure_led_mock__get_callcount());
}

void test_configure_rgb_leds__given_array_with_uninitialized_element__uninitialized_access_exception() {
    struct ledc_rgb_led_t leds[] = {
        { .is_initialized = true },
        { .is_initialized = false }
    };

    CEXCEPTION_T ex;
    Try {
        uint8_t led_count = sizeof(leds)/sizeof(leds[0]);
        configure_rgb_leds(leds, led_count);
        TEST_FAIL_MESSAGE("Expected argument null exception but none received.");
    } Catch(ex) { }

    TEST_ASSERT_EQUAL(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, ex);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_configure_rgb_led__given_null_input__throws_argument_null_exception);
    RUN_TEST(test_configure_rgb_led__given_an_uninitialized_led__throws_uninitialized_access_exception);
    RUN_TEST(test_configure_rgb_led__given_valid_led__configures_all_rgb_channel_leds);
    RUN_TEST(test_configure_rgb_leds__given_null_array__throws_argument_null_exception);
    RUN_TEST(test_configure_rgb_leds__given_initialized_array__configures_all_rgb_channels_of_all_leds);
    RUN_TEST(test_configure_rgb_leds__given_array_with_uninitialized_element__uninitialized_access_exception);

    return UNITY_END();
}

int app_main() {
    return main();
}