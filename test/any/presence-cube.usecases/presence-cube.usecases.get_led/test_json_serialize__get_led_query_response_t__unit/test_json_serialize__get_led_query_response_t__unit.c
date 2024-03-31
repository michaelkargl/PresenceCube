#include "unity.h"
#include "assertion_helpers.h"
#include "get_led/json_serialize__get_led_query_response_t.h"
#include "using/using_json.h"
#include "using/using_free.h"

static cJSON *_json_led_array;

void setUp() {}
void tearDown()
{
    TEST_MESSAGE("Disposing of json objects");
    cJSON_Delete(_json_led_array);
}

void test_json_stringify__get_led_query_response_t__given_null__throws()
{
    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        json_stringify__get_led_query_response_t(NULL);
    });
}

void test_json_stringify__get_led_query_response_t__serializes_to_json()
{
    rgb_led_diode_bag_t led_bag = {
        .is_initialized = true,
        .count = 2,
        .leds = (rgb_led_diode_t[]){
            {.id = 5, .display_name = "test1"},
            {.id = 6, .display_name = "test2"}}};

    get_led_query_response_t response = {.led_bag = &led_bag};
    const char *json_string = json_stringify__get_led_query_response_t(&response);
    TEST_ASSERT_NOT_NULL(json_string);
    TEST_ASSERT_NOT_EMPTY(json_string);

    USING(json_string, {
        _json_led_array = cJSON_Parse(json_string);
    });
    TEST_ASSERT_EQUAL(response.led_bag->count, cJSON_GetArraySize(_json_led_array));
    
    for (uint8_t i = 0; i < response.led_bag->count; i++)
    {
        rgb_led_diode_t expected_led = response.led_bag->leds[i];

        cJSON *actual_led = cJSON_GetArrayItem(_json_led_array, i);
        TEST_ASSERT_NOT_NULL(actual_led);

        cJSON *actual_led_id = cJSON_GetObjectItem(actual_led, "id");
        cJSON *actual_led_name = cJSON_GetObjectItem(actual_led, "name");
        cJSON *actual_led_initialized = cJSON_GetObjectItem(actual_led, "isInitialized");

        TEST_ASSERT_NOT_NULL(actual_led_id);
        TEST_ASSERT_EQUAL(expected_led.id, actual_led_id->valueint);
        TEST_ASSERT_NOT_EMPTY(actual_led_id->string);
        TEST_ASSERT_TRUE(actual_led_id->valueint);
    }
}

int main()
{
    UNITY_BEGIN();

    TEST_ASSERT_THROWS_NOT({
        RUN_TEST(test_json_stringify__get_led_query_response_t__given_null__throws);
        RUN_TEST(test_json_stringify__get_led_query_response_t__serializes_to_json);
    });

    return UNITY_END();
}

int app_main()
{
    return main();
}