#include "main_native.h"
#include "logger.h"
#include "get_led_query_handler.h"

static const char* _logger_context = "main.native";


static void _initialize_modules() {
    log_information(_logger_context, "Initializing modules...\n");
    get_led_query_handler_init();
}

static void _deinitialize_modules() {
    log_information(_logger_context, "Deinitializing modules...");
    get_led_query_handler_deinit();
}

int main() {
    log_information("main", "Running %s main %s build\n", "native", BUILD_ENVIRONMENT);    

    _initialize_modules();

    get_led_query_response_t response = handle_get_led_query((get_led_query_t) {});
    
    log_debug(_logger_context, "Queried a list of %i leds\n", response.led_bag.count);
    for (uint8_t i = 0; i < response.led_bag.count; i++) {
        led_domain_t led = response.led_bag.leds[i];
        log_debug(_logger_context, "  Led %i: %s\n", i, led.display_name);
    }
    
    _deinitialize_modules();
    return 0;
}
