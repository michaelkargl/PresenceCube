#include "unity.h"
#include "rgb_ledc_adapter.h"
#include "assertion_helpers.h"
#include "led_store_func.h"
#include "map_ledc_led__led_domain.h"
#include "rgb_ledc_func.h"
#include "fff.h"

extern led_store__get_led_func_t rgb_ledc_adapter__led_store__get_led_fn;
extern rgb_ledc__set_led_color_percent_func_t rgb_ledc_adapter__rgb_ledc__set_led_color_percent_fn;

DEFINE_FFF_GLOBALS
FAKE_VALUE_FUNC1(struct ledc_rgb_led_t *, fff_get_led, uint8_t);
FAKE_VOID_FUNC5(
    fff_set_led_color_percent,
    const struct ledc_rgb_led_t *,
    int,
    int,
    int,
    uint32_t);

static const rgb_led_domain_t _dummy_domain_led = {};
static const uint8_t _dummy_color = 12;
static const uint8_t _dummy_id = 32;
static const uint8_t _known_led_id = 33;

static struct ledc_rgb_led_t _known_ledc_led = {
    .is_initialized = true,
    .id = _known_led_id};

void setUp()
{
    rgb_ledc_adapter__rgb_ledc__set_led_color_percent_fn = fff_set_led_color_percent;
    rgb_ledc_adapter__led_store__get_led_fn = fff_get_led;
    fff_get_led_fake.return_val = &_known_ledc_led;
}

void tearDown()
{
    RESET_FAKE(fff_get_led);
}

static void test_set_color__given_null_led__throws()
{
    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        rgb_ledc_adapter__set_color(NULL, _dummy_id, _dummy_id, _dummy_id);
    });
}

static void test_set_color__given_unknown_id__throws()
{
    fff_get_led_fake.return_val = NULL;
    TEST_ASSERT_THROWS(ERROR_CODE_RESOURCE_NOT_FOUND, {
        rgb_ledc_adapter__set_color(&_dummy_domain_led, _dummy_id, _dummy_id, _dummy_id);
    });
}

static void test_set_color__given_known_led__sets_color()
{
    rgb_ledc_adapter__set_color(&_dummy_domain_led, 10, 20, 30);

    TEST_ASSERT_EQUAL(1, fff_set_led_color_percent_fake.call_count);
    TEST_ASSERT_EQUAL_PTR(fff_get_led_fake.return_val, fff_set_led_color_percent_fake.arg0_val);
    TEST_ASSERT_EQUAL(10, fff_set_led_color_percent_fake.arg1_val);
    TEST_ASSERT_EQUAL(20, fff_set_led_color_percent_fake.arg2_val);
    TEST_ASSERT_EQUAL(30, fff_set_led_color_percent_fake.arg3_val);
}

static void test_set_color__given_uninitialized_module__throws()
{
    TEST_ASSERT_THROWS(ERROR_CODE_UNINITIALIZED_MODULE_ACCESS, {
        rgb_ledc_adapter__set_color(&_dummy_domain_led, _dummy_color, _dummy_color, _dummy_color);
    });
}

static void test_set_color__initializes_module()
{
    TEST_ASSERT_FALSE(rgb_ledc_adapter__initialized());
    rgb_ledc_adapter__initialize();
    TEST_ASSERT_TRUE(rgb_ledc_adapter__initialized());
}

static void run_order_critical_tests()
{
    test_set_color__given_uninitialized_module__throws();
    // ...
    // this needs to be called last
    test_set_color__initializes_module();
}

int main()
{
    UNITY_BEGIN();

    TEST_ASSERT_THROWS_NOT({
        run_order_critical_tests();
        RUN_TEST(test_set_color__given_null_led__throws);
        RUN_TEST(test_set_color__given_unknown_id__throws);
        RUN_TEST(test_set_color__given_known_led__sets_color);
    });

    return UNITY_END();
}

int app_main()
{
    return main();
}