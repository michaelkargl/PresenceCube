#include "get_led/json_serialize__get_led_query_response_t.h"
#include "logger.h"
#include "exception_handling.h"
#include "using/using_json.h"

/**
 * @brief
 *
 * @param response
 * @return cJSON* a reference to a json array object. The caller is responsible for
 *         it's disposal (by defualt cJSON_Delete).
 */
static cJSON *_serialize_get_led_query_response(get_led_query_response_t *response)
{
    // https://github.com/DaveGamble/cJSON#working-with-the-data-structure
    // For every value type there is a cJSON_CreateXYZ function that can be used to
    // create an item of that type. All of these will allocate a cJSON struct that
    // can later be deleted with cJSON_Delete. Note that you have to delete them at
    // some point, otherwise you will get memory leaks.

    cJSON *led_array = cJSON_CreateArray();

    for (int i = 0; i < response->led_bag->count; i++)
    {
        LOG_INFORMATION("Processing led bag led %i", i);
        rgb_led_diode_t led = response->led_bag->leds[i];

        cJSON *led_item = cJSON_CreateObject();
        cJSON_AddNumberToObject(led_item, "id", led.id);
        cJSON_AddStringToObject(led_item, "name", led.display_name);
        cJSON_AddBoolToObject(led_item, "isInitialized", led.is_initialized);

        // Important: Once an item is attached to an array or an object, ownership
        // is transfered to that object or array. Ripping out elements by deleting
        // manually through __cJSON_Delete__ will end in memory leaks.
        cJSON_AddItemToArray(led_array, led_item);
    }

    return led_array;
}

const char *json_stringify__get_led_query_response_t(get_led_query_response_t *response)
{
    THROW_ARGUMENT_NULL_IF_NULL(response);

    // we are making the pointer volatile so we can use it within the USING_JSON macro
    // (which uses Try/Catch error handling)
    // volatile int *var tells the compiler that the data at that address is volatile
    // int *volatile var makes the pointer itself volatile
    // remember the clockwise/spiral rule
    char *volatile json = NULL;

    cJSON *json_object = _serialize_get_led_query_response(response);
    USING_JSON(json_object, {
        // make sure to deallocate
        json = cJSON_Print(json_object);
        LOG_INFORMATION("Produced json %s", json);
    });

    return json;
}
