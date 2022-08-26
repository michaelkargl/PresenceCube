#include "led_controller.h"
#include "webserver.h"
#include "delay_service.h"
#include "cJSON.h"
#include "stdbool.h"
#include "CException.h"
#include "get_led_query_handler.h"
#include "set_led_response_dto_t.h"
#include "using.h"

// TODO: move configuration into a separate configuration header
#define REQUEST_BODY_BUFFER_SIZE 50
#define REQUEST_ERROR_MESSAGE_BUFFER_SIZE 50
#define LED_FADE_MILLISECONDS 4000
#define MODULE_NAME "led_controller"

// TODO: extract static functions into tested parsers and response_builders (naming tbd)


static esp_err_t POST_led_handler(httpd_req_t *req);
static esp_err_t GET_leds_handler(httpd_req_t *req);
static esp_err_t OPTIONS_handler(httpd_req_t *req);

static const httpd_uri_t _home_controller_endpoints[] = {
    {.uri = "/led",
     .method = HTTP_POST,
     .handler = POST_led_handler},
    {.uri = "/leds",
     .method = HTTP_GET,
     .handler = GET_leds_handler},
    {.uri = "/led",
     .method = HTTP_OPTIONS,
     .handler = OPTIONS_handler}};

const httpd_uri_t *get_led_controller_endpoints()
{
    return _home_controller_endpoints;
}

uint8_t get_led_controller_endpoint_count()
{
    return (uint8_t)(sizeof(_home_controller_endpoints) / sizeof(_home_controller_endpoints[0]));
}

static void _send_response(httpd_req_t *request, char *status_code, const char *payload)
{
    ESP_ERROR_CHECK(httpd_resp_set_hdr(request, "Access-Control-Allow-Origin", "*"));
    ESP_ERROR_CHECK(httpd_resp_set_hdr(request, "Access-Control-Max-Age", "600"));
    ESP_ERROR_CHECK(httpd_resp_set_hdr(request, "Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS"));
    ESP_ERROR_CHECK(httpd_resp_set_hdr(request, "Access-Control-Allow-Headers", "*"));
    ESP_ERROR_CHECK(httpd_resp_set_status(request, status_code));

    if (payload != NULL)
    {
        ESP_ERROR_CHECK(httpd_resp_send(request, payload, HTTPD_RESP_USE_STRLEN));
    }
}

static esp_err_t OPTIONS_handler(httpd_req_t *request)
{
    _send_response(request, HTTPD_204, "Cors approved ;)");
    return ESP_OK;
}

/**
 * @brief
 *
 * @param response
 * @return cJSON* a reference to a json array object. The caller is responsible for
 *         it's disposal (by defualt cJSON_Delete).
 */
static cJSON *_map_led_bag_to_json_document(get_led_query_response_t *response)
{
    // https://github.com/DaveGamble/cJSON#working-with-the-data-structure
    // For every value type there is a cJSON_Create... function that can be used to
    // create an item of that type. All of these will allocate a cJSON struct that
    // can later be deleted with cJSON_Delete. Note that you have to delete them at
    // some point, otherwise you will get a memory leak.
    //
    // Important: If you have added an item to an array or an object already, you
    // mustn't delete it with __cJSON_Delete__. Adding it to an array or object transfers
    // its ownership so that when that array or object is deleted, it gets deleted as
    // well. You also could use cJSON_SetValuestring to change a cJSON_String's //
    /// valuestring, and you needn't to free the previous valuestring manually.

    cJSON *led_array = cJSON_CreateArray();

    // TODO: error handling
    // cJSON delete array on error
    // test out if json objects can be deleted twice without error

    for (int i = 0; i < response->led_bag.count; i++)
    {
        log_information(MODULE_NAME, "Processing led bag led %i", i);
        rgb_led_domain_t led = response->led_bag.leds[i];

        cJSON *led_item = cJSON_CreateObject();
        cJSON_AddNumberToObject(led_item, "id", led.id);
        cJSON_AddStringToObject(led_item, "name", led.display_name);
        cJSON_AddBoolToObject(led_item, "isInitialized", led.is_initialized);

        cJSON_AddItemToArray(led_array, led_item);
    }

    return led_array;
}

static char *_map_led_bag_to_json(get_led_query_response_t *response)
{
    // volatile int *var tells the compiler that the data at that address is volatile
    // int *volatile var makes the pointer itself volatile
    // remember the clockwise/spiral rule
    char *volatile json = NULL;

    cJSON *json_object = _map_led_bag_to_json_document(response);
    USING_JSON(json_object, {
        // make sure to deallocate
        json = cJSON_Print(json_object);
        log_information(MODULE_NAME, "Produced json %s", json);
    });

    return json;
}

// TODO: integration test
static esp_err_t GET_leds_handler(httpd_req_t *request)
{
    get_led_query_response_t response = handle_get_led_query((const get_led_query_t){});

    bool not_found = response.led_bag.leds == NULL || response.led_bag.count == 0;
    if (not_found)
    {
        _send_response(request, HTTPD_404, "No/NULL LEDs were found.");
    }

    char *response_string = _map_led_bag_to_json(&response);
    if (response_string == NULL)
    {
        _send_response(request, HTTPD_500, "Received a NULL payload from response string builder.");
    }

    USING(response_string, {
        _send_response(request, HTTPD_200, response_string);
    });

    // heap_caps_print_heap_info(MALLOC_CAP_DEFAULT);
    return ESP_OK;
}

// TODO: with THROW statements flying around, make sure to dispose of everythign in a catch statement
static cJSON *_get_number_property(cJSON *object, const char *const property_name)
{
    cJSON *property = cJSON_GetObjectItemCaseSensitive(object, property_name);
    if (!cJSON_IsNumber(property))
    {
        THROW(ERROR_CODE_UNHANDLED, "JSON property '%s' is not a number.", property_name);
    }

    return property;
}

static struct set_led_response_dto_t _parse_to_request_dto(cJSON *json)
{
    return (struct set_led_response_dto_t){
        .id = _get_number_property(json, "id")->valueint,
        .red = _get_number_property(json, "r")->valueint,
        .green = _get_number_property(json, "g")->valueint,
        .blue = _get_number_property(json, "b")->valueint};
}

static esp_err_t POST_led_handler(httpd_req_t *request)
{
    char body_buffer[REQUEST_BODY_BUFFER_SIZE];
    char error_buffer[REQUEST_ERROR_MESSAGE_BUFFER_SIZE];
    volatile esp_err_t status_code = ESP_FAIL;

    cJSON *json = webserver_try_get_json_request(
        request,
        body_buffer, sizeof(body_buffer) / sizeof(body_buffer[0]),
        error_buffer, sizeof(error_buffer) / sizeof(error_buffer[0]));

    volatile CEXCEPTION_T ex;
    Try
    {
        struct set_led_response_dto_t dto;
        USING_JSON(json, {
            dto = _parse_to_request_dto(json);
        });

        heap_caps_print_heap_info(MALLOC_CAP_DEFAULT);
        _send_response(request, "501 METHOD_NOT_IMPLEMENTED", "Not implemented");

        // set_led_color_8bit(
        //     NULL, //&_leds[id->valueint],
        //     dto.red,
        //     dto.green,
        //     dto.blue,
        //     LED_FADE_MILLISECONDS
        // );

        // _send_response(request, HTTPD_200, "OK");
        status_code = ESP_OK;
    }
    Catch(ex)
    {
        // TODO: improve message once exceptions can carry strings.
        _send_response(request, HTTPD_500, "An error occured.");
    }

    return status_code;
}
