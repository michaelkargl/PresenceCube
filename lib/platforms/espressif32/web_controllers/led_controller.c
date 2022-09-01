#include "CException.h"
#include "led_controller.h"
#include "get_led_query_handler.h"
#include "stddef.h"
#include "malloc.h"
//
#include "rgb_ledc.h"
#include "led_store.h"
//

// TODO: configure using menuconfig
#define LED_FADE_MILLISECONDS CONFIG_PWM_FADE_INTERVAL

// TODO: review naming
// TODO: replace get_led_query_response_t with response dto
get_led_query_response_t led_controller__GET_leds_handler()
{
    // TODO: test
    get_led_query_response_t response = handle_get_led_query((const get_led_query_t){});

    // TODO: test
    bool not_found = response.led_bag.leds == NULL;
    if (not_found)
    {
        THROW(ERROR_CODE_RESOURCE_NOT_FOUND, "No registered LEDs could be found");
    }
    
    // TODO: testt
    return response;
}

void led_controller__POST_led_handler(struct set_led_request_dto_t *set_led_request)
{  
    // TODO: test
    THROW_ARGUMENT_NULL_IF_NULL(set_led_request);

    // TODO: replace with set_led usecase one it exists
    // TODO: test
    struct ledc_rgb_led_t* leds = led_store__get_leds();
    set_led_color_8bit(
        &leds[set_led_request->id],
        set_led_request->red,
        set_led_request->green,
        set_led_request->blue,
        LED_FADE_MILLISECONDS
    );
}