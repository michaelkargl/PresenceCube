#include "unity.h"
#include "fff.h"
#include "assertion_helpers.h"
#include "map_ledc_led__led_domain.h"
#include "ledc_led_duty_calculator_func.h"
#include "ledc_led_builder.h"

#define ONCE 1

extern calculate_duty_percent_func_t _map_ledc_led__led_domain__calculate_duty_percent;


DEFINE_FFF_GLOBALS;
FAKE_VALUE_FUNC(float, _fff_calculate_duty_percent, const struct ledc_led_t*);

void setUp()
{
    _map_ledc_led__led_domain__calculate_duty_percent = _fff_calculate_duty_percent;
}

void tearDown()
{
    RESET_FAKE(_fff_calculate_duty_percent);
}

static uint8_t _id_counter = 1;
static uint8_t _get_id() {
    if( _id_counter == 0 ) {
        Throw(ERROR_CODE_DATATYPE_OVERFLOW);
    }

    return _id_counter++;
}

static struct ledc_led_t _build_2level_led(bool on)
{
    const ledc_timer_config_t twoBitTimer = {.duty_resolution = 1};
    return (struct ledc_led_t){
        .id = _get_id(),
        .timer = twoBitTimer,
        .channel = {.duty = on},
        .is_initialized = true};
}

// ---------------------------------------------------------------------------

void test__map_ledc_led__to__led_domain__given_null__throws()
{
    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        map_ledc_led__to__led_domain(NULL);
    });
}

void test_map_ledc_led__to__led_domain()
{
    struct ledc_led_t source = _build_2level_led(true);
    _fff_calculate_duty_percent_fake.return_val = 50.0f;
    
    led_domain_t actual = map_ledc_led__to__led_domain(&source);

    TEST_ASSERT_EQUAL(source.is_initialized, actual.is_initialized);
    TEST_ASSERT_EQUAL(source.id, actual.id);
    TEST_ASSERT_EQUAL_STRING(source.name, actual.display_name);   
    TEST_ASSERT_EQUAL(ONCE, _fff_calculate_duty_percent_fake.call_count);
    TEST_ASSERT_EQUAL(_fff_calculate_duty_percent_fake.return_val, actual.brightness_percent);
}

// ---------------------------------------------------------------------------

void test__map_ledc_led_array__to__led_domain_bag__given_null_array__throws()
{
    const struct ledc_led_t dummy_source[1];
    led_domain_bag_t target_bag = {
        .leds = (led_domain_t[1]) {},
        .count = 1
    };

    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        map_ledc_led_array__to__led_domain_bag(
            NULL, ARRAY_LENGTH(dummy_source),
            &target_bag
        );
    });

    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        map_ledc_led_array__to__led_domain_bag(
            dummy_source, ARRAY_LENGTH(dummy_source),
            NULL
        );
    });

    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        target_bag.leds = NULL;
        map_ledc_led_array__to__led_domain_bag(
            dummy_source, ARRAY_LENGTH(dummy_source),
            NULL
        );
    });
}

void test__map_ledc_led_array__to__led_domain_bag__given_smaller_target_array__throws() {
    const struct ledc_led_t large_source[2];
    led_domain_bag_t smaller_bag = {
        .leds = (led_domain_t[1]) {},
        .count = 1
    };

    TEST_ASSERT_THROWS(ERROR_CODE_INSUFFICIENT_BUFFER, {
        map_ledc_led_array__to__led_domain_bag(
            large_source, ARRAY_LENGTH(large_source),
            &smaller_bag
        );
    });
}

void test__map_ledc_led_array__to__led_domain_bag__given_larger_target_array__partly_maps() {
    struct ledc_led_t small_source[] = { _build_2level_led(true) };
    led_domain_bag_t larger_bag = {
        .leds = (led_domain_t[2]) {},
        .count = 2
    };

    TEST_ASSERT_THROWS(ERROR_CODE_INSUFFICIENT_BUFFER, {
        map_ledc_led_array__to__led_domain_bag(
            small_source, ARRAY_LENGTH(small_source),
            &larger_bag
        );
    });

    TEST_ASSERT_EQUAL(ARRAY_LENGTH(small_source), larger_bag.count);
    TEST_ASSERT_EQUAL(small_source[0].id, larger_bag.leds[0].id);
    TEST_ASSERT_FALSE(larger_bag.leds[1].is_initialized);
}

// ---------------------------------------------------------------------------

void test__map_rgb_ledc_led__to__rgb_led_domain__given_null_inputs__throws() {
    const struct ledc_rgb_led_t dummy_source_led = {};
    rgb_led_domain_t dummy_target_led = {};

    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        map_rgb_ledc_led__to__rgb_led_domain(NULL, &dummy_target_led);
    });

    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        map_rgb_ledc_led__to__rgb_led_domain(&dummy_source_led, NULL);
    });
}

void test__map_rgb_ledc_led__to__rgb_led_domain() {
    rgb_led_domain_t target_led = { };
    const struct ledc_rgb_led_t source_led = {
        .id = 99,
        .red = _build_2level_led(true),
        .green = _build_2level_led(true),
        .blue = _build_2level_led(true),
        .is_initialized = true,
        .name = "testrgbled"
    };

    map_rgb_ledc_led__to__rgb_led_domain(&source_led, &target_led);

    TEST_ASSERT_EQUAL(source_led.id, target_led.id);
    TEST_ASSERT_EQUAL(source_led.red.id, target_led.red.id);
    TEST_ASSERT_EQUAL(source_led.green.id, target_led.green.id);
    TEST_ASSERT_EQUAL(source_led.blue.id, target_led.blue.id);
}

// ------------------------------------------------------------------------

void test_map_rgb_ledc_led_array__to__rgb_led_domain_bag__given_null_input_throws() {
    struct ledc_rgb_led_t dummy_source_leds[1];
    rgb_led_domain_t dummy_domain_leds[1];
    rgb_led_domain_bag_t dummy_bag = {
        .count = ARRAY_LENGTH(dummy_domain_leds),
        .leds = dummy_domain_leds
    };
    
    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        map_rgb_ledc_led_array__to__rgb_led_domain_bag(NULL, 0, &dummy_bag);
    });

    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        map_rgb_ledc_led_array__to__rgb_led_domain_bag(
            dummy_source_leds,
            ARRAY_LENGTH(dummy_source_leds),
            NULL
        );
    });

    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        dummy_bag.leds = NULL;
        map_rgb_ledc_led_array__to__rgb_led_domain_bag(
            dummy_source_leds,
            ARRAY_LENGTH(dummy_source_leds),
            &dummy_bag
        );
    });
}

void test_map_rgb_ledc_led_array__to__rgb_led_domain_bag__given_too_small_target_throws() {
    struct ledc_rgb_led_t dummy_source_leds[2];
    rgb_led_domain_bag_t dummy_bag = {
        .is_initialized = true,
        .leds = (rgb_led_domain_t[1]){},
        .count = 1
    };

    TEST_ASSERT_THROWS(ERROR_CODE_INSUFFICIENT_BUFFER, {
        map_rgb_ledc_led_array__to__rgb_led_domain_bag(
            dummy_source_leds,
            ARRAY_LENGTH(dummy_source_leds),
            &dummy_bag
        );
    });
}

static struct ledc_rgb_led_t _build_dummy_ledc_rgb_led() {
    return (struct ledc_rgb_led_t){
        .id = _get_id(),
        .red = { .id = _get_id() },
        .green = { .id = _get_id() },
        .blue = {.id = _get_id() },
        .name = "test",
        .is_initialized = true
    };
}

void test_map_rgb_ledc_led_array__to__rgb_led_domain_bag() {
    struct ledc_rgb_led_t source_leds[2] = {
        _build_dummy_ledc_rgb_led(),
        _build_dummy_ledc_rgb_led()
    };

    rgb_led_domain_bag_t target_bag = {
        .is_initialized = true,
        .leds = (rgb_led_domain_t[2]){},
        .count = 2
    };
    
     map_rgb_ledc_led_array__to__rgb_led_domain_bag(
        source_leds,
        ARRAY_LENGTH(source_leds),
        &target_bag
    );

    TEST_ASSERT_NOT_NULL(target_bag.leds);
    for(uint8_t i = 0; i < target_bag.count; i++) {
        TEST_ASSERT_EQUAL(source_leds[i].id, target_bag.leds[i].id);
        TEST_ASSERT_EQUAL(source_leds[i].red.id, target_bag.leds[i].red.id);
        TEST_ASSERT_EQUAL(source_leds[i].green.id, target_bag.leds[i].green.id);
        TEST_ASSERT_EQUAL(source_leds[i].blue.id, target_bag.leds[i].blue.id);
        TEST_ASSERT_EQUAL_STRING(source_leds[i].name, target_bag.leds[i].display_name);
    }
}


int main()
{
    UNITY_BEGIN();

    TEST_ASSERT_THROWS_NOT({
        RUN_TEST(test__map_ledc_led__to__led_domain__given_null__throws);
        RUN_TEST(test_map_ledc_led__to__led_domain);
        //
        RUN_TEST(test__map_ledc_led_array__to__led_domain_bag__given_null_array__throws);
        RUN_TEST(test__map_ledc_led_array__to__led_domain_bag__given_smaller_target_array__throws);
        //
        RUN_TEST(test__map_rgb_ledc_led__to__rgb_led_domain__given_null_inputs__throws);
        RUN_TEST(test__map_rgb_ledc_led__to__rgb_led_domain);
        //
        RUN_TEST(test_map_rgb_ledc_led_array__to__rgb_led_domain_bag__given_null_input_throws);
        RUN_TEST(test_map_rgb_ledc_led_array__to__rgb_led_domain_bag__given_too_small_target_throws);
        RUN_TEST(test_map_rgb_ledc_led_array__to__rgb_led_domain_bag);
    });

    return UNITY_END();
}

int app_main()
{
    return main();
}