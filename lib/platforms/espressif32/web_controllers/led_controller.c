#include "exception_handling.h"
#include "led_controller.h"
#include "get_led_query_handler.h"
#include "set_led_command_handler.h"
#include "stddef.h"
#include "led_store.h"

// TODO #71: Add integration test for led_controller
// TODO: replace get_led_query_response_t with response dto
// TODO: review if the "led_control" library used by some other libs is still up-to-date / used / available

get_led_query_response_t led_controller__GET_leds_handler()
{
    const get_led_query_response_t response = get_led_query_handler__handle((const get_led_query_t){});

    bool not_found = response.led_bag->leds == NULL;
    if (not_found)
    {
        THROW(ERROR_CODE_RESOURCE_NOT_FOUND, "No registered LEDs could be found");
    }

    return response;
}

void led_controller__POST_led_handler(struct set_led_request_dto_t *set_led_request)
{
    THROW_ARGUMENT_NULL_IF_NULL(set_led_request);

    set_led_command_handler__handle((set_led_command_t){
        .id = set_led_request->id,
        .red = set_led_request->red,
        .green = set_led_request->green,
        .blue = set_led_request->blue});
}