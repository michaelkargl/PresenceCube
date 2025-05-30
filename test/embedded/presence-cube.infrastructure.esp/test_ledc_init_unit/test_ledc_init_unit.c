#include "unity.h"
#include "ledc_init.h"
#include "exception_handling.h"
#include "stubs/ledc_channel_config_stub.h"
#include "stubs/ledc_fade_func_install_stub.h"
#include "stubs/ledc_timer_config_stub.h"

#define ONCE 1
#define ZERO_TIMES 0

extern esp_err_t (*ledc_init__ledc_timer_config)(const ledc_timer_config_t* timer_configuration);
extern esp_err_t (*ledc_init__ledc_channel_config)(const ledc_channel_config_t* ledc_configuration);
extern esp_err_t (*led_init__ledc_fade_func_install)(int interrupt_allocation_flags);


void tearDown() { }
void setUp() {
    ledc_init__ledc_timer_config = ledc_timer_config_stub;
    ledc_init__ledc_channel_config = ledc_channel_config_stub;
    led_init__ledc_fade_func_install = ledc_fade_func_install_stub;
    
    ledc_timer_config_stub__reset();
    ledc_channel_config_stub__reset();
    ledc_fade_func_install_stub__reset();
}


static void assert_ledc_functions_called(uint8_t expected_call_count) {
    TEST_ASSERT_EQUAL_MESSAGE(expected_call_count, ledc_timer_config_stub__get_call_count(), "Function ledc_timer_config_stub.");
    TEST_ASSERT_EQUAL_MESSAGE(expected_call_count, ledc_channel_config_stub__get_call_count(), "Function ledc_channel_config_stub.");
    TEST_ASSERT_EQUAL_MESSAGE(expected_call_count, ledc_fade_func_install_stub__get_call_count(), "Function ledc_fade_func_install_stub.");
}


void test__configure_led__given_uninitialized_led__throws_uninitialized_struct_exception() {
    struct ledc_led_t dummy_led = {};
    
    CEXCEPTION_T ex;
    Try {
        configure_led(&dummy_led);
        TEST_FAIL_MESSAGE("Expected an 'uninitialized struct' exception but none was caught.");
    } Catch(ex) { }

    TEST_ASSERT_EQUAL(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, ex);
    assert_ledc_functions_called(ZERO_TIMES);
}

void test__configure_led__given_null_input__throws_argument_null_exception() {
    CEXCEPTION_T ex;
    Try {
        configure_led(NULL);
        TEST_FAIL_MESSAGE("Expected an 'argument null' exception but none was caught.");
    } Catch (ex) { }

    TEST_ASSERT_EQUAL(ERROR_CODE_ARGUMENT_NULL, ex);
    assert_ledc_functions_called(ZERO_TIMES);
}

void test__configure_led__given_initialized_led__calls_ledc_functions() {
    struct ledc_led_t dummy_led = { .is_initialized = true };

    configure_led(&dummy_led);
    
    assert_ledc_functions_called(ONCE);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test__configure_led__given_uninitialized_led__throws_uninitialized_struct_exception);
    RUN_TEST(test__configure_led__given_null_input__throws_argument_null_exception);
    RUN_TEST(test__configure_led__given_initialized_led__calls_ledc_functions);

    return UNITY_END();
}

int app_main() {
    return main();
}