#include "unity.h"
#include "ledc_facade.h"
#include "esp_err.h"
#include "fff.h"
#include "assertion_helpers/exceptions.h"

extern esp_err_t (*_ledc_facade__ledc_set_duty)(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t duty);
extern esp_err_t (*_ledc_facade__ledc_update_duty)(ledc_mode_t speed_mode, ledc_channel_t channel);
extern esp_err_t (*_ledc_facade__ledc_set_fade_with_time)(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t target_duty, int max_fade_time_ms);
extern esp_err_t (*_ledc_facade__ledc_fade_start)(ledc_mode_t speed_mode, ledc_channel_t channel, ledc_fade_mode_t fade_mode);

DEFINE_FFF_GLOBALS;
FAKE_VALUE_FUNC(esp_err_t, ledc_set_duty, ledc_mode_t, ledc_channel_t, uint32_t);
FAKE_VALUE_FUNC(esp_err_t, ledc_update_duty, ledc_mode_t, ledc_channel_t);
FAKE_VALUE_FUNC(esp_err_t, ledc_set_fade_with_time, ledc_mode_t, ledc_channel_t, uint32_t, int);
FAKE_VALUE_FUNC(esp_err_t, ledc_fade_start, ledc_mode_t, ledc_channel_t, ledc_fade_mode_t);

static const struct ledc_led_t _initialized_led = {.is_initialized = true};
static const struct ledc_led_t _uninitialized_led = {};

static void reset_fakes()
{
    RESET_FAKE(ledc_set_duty);
    RESET_FAKE(ledc_update_duty);
    RESET_FAKE(ledc_set_fade_with_time);
    RESET_FAKE(ledc_fade_start);
}

static void assign_fakes()
{
    _ledc_facade__ledc_set_duty = ledc_set_duty;
    _ledc_facade__ledc_update_duty = ledc_update_duty;
    _ledc_facade__ledc_set_fade_with_time = ledc_set_fade_with_time;
    _ledc_facade__ledc_fade_start = ledc_fade_start;
}

void setUp()
{
    assign_fakes();
}

void tearDown()
{
    reset_fakes();
}

void test_ledc_set_percent__with_zero_fade_time__sets_duty_directly()
{
    ledc_set_percent(&_initialized_led, 100, 0);

    TEST_ASSERT_EQUAL(0, ledc_fade_start_fake.call_count);
    TEST_ASSERT_EQUAL(1, ledc_update_duty_fake.call_count);
}

void test_ledc_set_percent__with_positive_fade_time__sets_duty_softly()
{
    ledc_set_percent(&_initialized_led, 100, 1000);

    TEST_ASSERT_EQUAL(1, ledc_fade_start_fake.call_count);
    TEST_ASSERT_EQUAL(0, ledc_set_duty_fake.call_count);
}

void test_ledc_set_percent__with_uninitialized_led__throws_uninitialized_access_exception()
{
    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, {
        ledc_set_percent(&_uninitialized_led, 100, 0);
    });
}

void test_ledc_set_percent__with_null_led__throws_argument_null_exception()
{
    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        ledc_set_percent(NULL, 100, 0);
    });
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_ledc_set_percent__with_zero_fade_time__sets_duty_directly);
    RUN_TEST(test_ledc_set_percent__with_positive_fade_time__sets_duty_softly);
    RUN_TEST(test_ledc_set_percent__with_uninitialized_led__throws_uninitialized_access_exception);
    RUN_TEST(test_ledc_set_percent__with_null_led__throws_argument_null_exception);

    return UNITY_END();
}

int app_main()
{
    return main();
}