#include "set_led/set_led_command_handler.h"
#include "get_led_repository.h"
#include "set_led_repository.h"
#include "exception_handling.h"
#include "hal/rgb_ledc_adapter.h"
#include "stddef.h"

// TODO: Create a configuration module to configure this
//       There's no menuconfig for native, therefore, since this is a shared library
//       we can not directly use it without either hacking our way towards it or
//       or creating a better abstraction. A configuration module would also make
//       the resolving of choices more DRY => handle this in a ticket
//       Also clarify the memory impact of this design decision and decide whether
//       we want to use this only for shared libraries or throughout natvie/embedded
#define LED_FADE_MILLISECONDS 1000

get_led_repository__get_led_func_t _set_led_command__get_led_fn = get_led_repository__get_led;
rgb_ledc_adapter__set_color_func_t _set_led_command__set_color_fn = rgb_ledc_adapter__set_color;
set_led_repository__update_led_func_t _set_led_command__set_led_repository__update_led_fn = set_led_repository__update;

set_led_command_response_t set_led_command_handler__handle(const set_led_command_t command)
{
    const rgb_led_diode_t *led = _set_led_command__get_led_fn(command.id);
    THROW_RESOURCE_NOT_FOUND_IF_NULL(led, "A LED with id=%u could not be found.", command.id);

    _set_led_command__set_color_fn(led, command.red, command.green, command.blue);

    _set_led_command__set_led_repository__update_led_fn(led->id, command.red, command.green, command.blue);

    return (set_led_command_response_t){
        .id = command.id};
}