#include "unity.h"
#include "fff.h"
#include "assertion_helpers.h"
#include "map_ledc_led__led_domain.h"
#include "ledc_led_duty_calculator_func.h"
#include "ledc_led_builder.h"

#define ONCE 1

extern calculate_duty_percent_func_t _map_ledc_led__led_domain__calculate_duty_percent;

DEFINE_FFF_GLOBALS;
FAKE_VALUE_FUNC(float, _fff_calculate_duty_percent, const struct ledc_led_t);

void setUp()
{
    _map_ledc_led__led_domain__calculate_duty_percent = _fff_calculate_duty_percent;
}

void tearDown()
{
    RESET_FAKE(_fff_calculate_duty_percent);
}

static struct ledc_led_t _build_2level_led(bool on)
{
    const ledc_timer_config_t twoBitTimer = {.duty_resolution = 1};
    return (struct ledc_led_t){
        .timer = twoBitTimer,
        .channel = {.duty = on},
        .is_initialized = true};
}

void test__map_ledc_led__to__led_domain__given_null__throws()
{
    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        map_ledc_led__to__led_domain(NULL);
    });
}

void test__map_ledc_led_array__to__led_domain_array__given_null_array__throws()
{
    const led_domain_t dummy_source[1];
    const struct ledc_led_t dummy_target[1];

    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        map_ledc_led_array__to__led_domain_array(
            NULL, ARRAY_LENGTH(dummy_source),
            dummy_target, ARRAY_LENGTH(dummy_target)
        );
    });

    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        map_ledc_led_array__to__led_domain_array(
            dummy_source, ARRAY_LENGTH(dummy_source),
            NULL, ARRAY_LENGTH(dummy_target)
        );
    });
}

void test__map_ledc_led_array__to__led_domain_array__given_smaller_target_array__throws() {
    const led_domain_t large_source[2];
    struct ledc_led_t small_target[1];

    TEST_ASSERT_THROWS(ERROR_CODE_INSUFFICIENT_BUFFER, {
        map_ledc_led_array__to__led_domain_array(
            large_source, ARRAY_LENGTH(large_source),
            small_target, ARRAY_LENGTH(small_target)
        );
    });
}

void test__map_ledc_led_array__to__led_domain_array__given_larger_target_array__partly_maps() {
    const led_domain_t large_target[2];
    struct ledc_led_t small_source[1] = {
        _build_2level_led(true)
    };

    TEST_ASSERT_THROWS(ERROR_CODE_INSUFFICIENT_BUFFER, {
        map_ledc_led_array__to__led_domain_array(
            small_source, ARRAY_LENGTH(small_source),
            large_target, ARRAY_LENGTH(large_target)
        );
    });

    TEST_ASSERT_EQUAL(small_source[0].id, large_target[0].id);
    TEST_ASSERT_FALSE(large_target[1].is_initialized);
}

void test_map_ledc_led__to__led_domain()
{
    struct ledc_led_t source = _build_2level_led(true);
    _fff_calculate_duty_percent_fake.return_val = 50;
    
    led_domain_t actual = map_ledc_led__to__led_domain(&source);

    TEST_ASSERT_EQUAL(source.is_initialized, actual.is_initialized);
    TEST_ASSERT_EQUAL(source.id, actual.id);
    TEST_ASSERT_EQUAL_STRING(source.name, actual.display_name);   
    TEST_ASSERT_EQUAL(ONCE, _fff_calculate_duty_percent_fake.call_count);
    TEST_ASSERT_EQUAL(_fff_calculate_duty_percent_fake.return_val, actual.brightness_percent);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test__map_ledc_led__to__led_domain__given_null__throws);
    RUN_TEST(test_map_ledc_led__to__led_domain);

    RUN_TEST(test__map_ledc_led_array__to__led_domain_array__given_null_array__throws);
    RUN_TEST(test__map_ledc_led_array__to__led_domain_array__given_smaller_target_array__throws);

    return UNITY_END();
}

int app_main()
{
    return main();
}