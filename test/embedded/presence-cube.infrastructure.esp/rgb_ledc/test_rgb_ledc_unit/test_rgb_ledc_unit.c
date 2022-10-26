#include "unity.h"
#include "fff.h"
#include "exception_handling.h"
#include "rgb_ledc.h"
#include "exception_assertion_helper.h"
#include "ledc_facade_funcs.h"
#include "assertion_helpers.h"

extern ledc_set_percent_func_t _rgb_ledc__ledc_facade__set_percent_func;

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(_fff_ledc_set_percent, const struct ledc_led_t *, uint8_t, int32_t);

static const uint8_t _dummy_fade_interval = 0;
static const uint8_t _dummy_color = 42;
static struct ledc_rgb_led_t _uninitialized_led = {};
static struct ledc_rgb_led_t _initialized_led = {
    .is_initialized = true,
    .red = {.id = 1},
    .green = {.id = 2},
    .blue = {.id = 3}};

void setUp()
{
    _rgb_ledc__ledc_facade__set_percent_func = _fff_ledc_set_percent;
}

void tearDown()
{
    RESET_FAKE(_fff_ledc_set_percent);
    FFF_RESET_HISTORY();
}

void test_set_led_color_percent__given_null__throws_null_argument_exception()
{
    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        set_led_color_percent(NULL, 100, 100, 100, 0);
    });

    TEST_ASSERT_EQUAL(0, _fff_ledc_set_percent_fake.call_count);
}

void test_set_led_color_percent__given_uninitialized_led__throws_uninitialized_data_access_exception()
{
    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, {
        set_led_color_percent(&_uninitialized_led, 10, 20, 30, 0);
    });

    TEST_ASSERT_EQUAL(0, _fff_ledc_set_percent_fake.call_count);
}

void test_set_leds_color_percent__given_uninitialized_led__throws()
{

    const struct ledc_rgb_led_t leds[] = {
        _initialized_led,
        _uninitialized_led};

    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, {
        set_leds_color_percent(leds,
                               ARRAY_LENGTH(leds),
                               _dummy_color,
                               _dummy_color,
                               _dummy_color,
                               _dummy_fade_interval);
    });
}

void test_set_leds_color_percent__given_null__throws_null_argument_exception()
{
    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        set_leds_color_percent(NULL, 0, 100, 100, 100, 0);
    });

    TEST_ASSERT_EQUAL(0, _fff_ledc_set_percent_fake.call_count);
}

static void _test_set_leds_color_percent__given_led_array__sets_led_percentages(
    struct ledc_rgb_led_t *leds, uint8_t leds_count, uint8_t expected_callcount)
{
    RESET_FAKE(_fff_ledc_set_percent);

    set_leds_color_percent(leds, leds_count, 100, 100, 100, 0);

    TEST_ASSERT_EQUAL(expected_callcount, _fff_ledc_set_percent_fake.call_count);
}

void test_set_leds_color_percent__given_led_array__sets_led_percentages()
{
    _test_set_leds_color_percent__given_led_array__sets_led_percentages(
        (struct ledc_rgb_led_t[0]){}, 0, 0);

    _test_set_leds_color_percent__given_led_array__sets_led_percentages(
        (struct ledc_rgb_led_t[1]){_initialized_led}, 1, 3);

    _test_set_leds_color_percent__given_led_array__sets_led_percentages(
        (struct ledc_rgb_led_t[2]){_initialized_led, _uninitialized_led}, 2, 3);

    _test_set_leds_color_percent__given_led_array__sets_led_percentages(
        (struct ledc_rgb_led_t[3]){_initialized_led, _initialized_led, _initialized_led}, 3, 9);
}

void test_set_led_color_percent__given_led__sets_all_color_channel_leds()
{
    TEST_ASSERT_THROWS_NOT({
        set_led_color_percent(&_initialized_led, 10, 20, 30, 40);
    });

    TEST_ASSERT_EQUAL(3, _fff_ledc_set_percent_fake.call_count);
    TEST_ASSERT_EQUAL(_initialized_led.red.id, _fff_ledc_set_percent_fake.arg0_history[0]->id);
    TEST_ASSERT_EQUAL(_initialized_led.green.id, _fff_ledc_set_percent_fake.arg0_history[1]->id);
    TEST_ASSERT_EQUAL(_initialized_led.blue.id, _fff_ledc_set_percent_fake.arg0_history[2]->id);
}

void test_set_led_color_8bit__given_uninitialized_led__throws_uninitialized_data_access_exception()
{
    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_DATA_ACCESS, {
        set_led_color_8bit(&_uninitialized_led, 10, 20, 30, 0);
    });

    TEST_ASSERT_EQUAL(0, _fff_ledc_set_percent_fake.call_count);
}

void test_set_led_color_8bit__given_null__throws_null_argument_exception()
{
    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        set_led_color_8bit(NULL, 10, 20, 30, 0);
    });

    TEST_ASSERT_EQUAL(0, _fff_ledc_set_percent_fake.call_count);
}

void test_set_led_color_8bit__given_rgb_values__sets_correct_channel_percentages()
{
    set_led_color_8bit(&_initialized_led, 85, 170, 255, 0);

    TEST_ASSERT_EQUAL(3, _fff_ledc_set_percent_fake.call_count);
    TEST_ASSERT_EQUAL(33, _fff_ledc_set_percent_fake.arg1_history[0]);
    TEST_ASSERT_EQUAL(66, _fff_ledc_set_percent_fake.arg1_history[1]);
    TEST_ASSERT_EQUAL(100, _fff_ledc_set_percent_fake.arg1_history[2]);
}

static void test_function_pointer_compatibility()
{
    void *_ = NULL;
    _ = (rgb_ledc__set_led_color_percent_func_t)set_led_color_percent;
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_set_led_color_percent__given_null__throws_null_argument_exception);
    RUN_TEST(test_set_led_color_percent__given_uninitialized_led__throws_uninitialized_data_access_exception);
    RUN_TEST(test_set_led_color_percent__given_led__sets_all_color_channel_leds);

    RUN_TEST(test_set_leds_color_percent__given_uninitialized_led__throws);
    RUN_TEST(test_set_leds_color_percent__given_null__throws_null_argument_exception);
    RUN_TEST(test_set_leds_color_percent__given_led_array__sets_led_percentages);

    RUN_TEST(test_set_led_color_8bit__given_uninitialized_led__throws_uninitialized_data_access_exception);
    RUN_TEST(test_set_led_color_8bit__given_null__throws_null_argument_exception);
    RUN_TEST(test_set_led_color_8bit__given_rgb_values__sets_correct_channel_percentages);

    RUN_TEST(test_function_pointer_compatibility);

    return UNITY_END();
}

int app_main()
{
    return main();
}