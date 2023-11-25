#include "uncaught_error_handler.h"
#include "logger.h"
#include "stdlib.h"
#include "stdbool.h"


static const char * const logger_tag = "error_handling";
static void (*handle_uncaught_error_callback)(error_code_t) = 0;
void (*uncaught_error_handler__exit_fn)(int) = exit;

static bool assert_module_initialized() {
    bool uncaught_error_callback_handler_defined = handle_uncaught_error_callback != 0;    
    return uncaught_error_callback_handler_defined;
}

void uncaught_error_handler_deinit() {
    handle_uncaught_error_callback = 0;
}

void uncaught_error_handler_init(
    void (*uncaught_error_handler_fn)(error_code_t)
) {
    log_information(logger_tag, "Initializing error_handling\n");
    handle_uncaught_error_callback = uncaught_error_handler_fn;
}

void uncaught_error_handler_handle(error_code_t error_code) {
    LOG_ERROR("Uncaught exception caught: error code 0x%04x", error_code);
    
    if (!assert_module_initialized()) {
        LOG_ERROR(
            "=================================================\n"
            "No uncaught exception handler could be found.\n"
            "To prevent memory leaks or error fall-throughs, make sure to\n"
            "- configure an uncaught_error_handler using uncaught_error_handler_init\n"
            "- handle every Throw() in a corresponding Try/Catch()\n"
            "=========================================================================",
            error_code
        );

        uncaught_error_handler__exit_fn(error_code);
        return;
    }

    LOG_DEBUG("Calling configured error handler with error_code: %i", error_code);
    handle_uncaught_error_callback(error_code);
}