#include "uncaught_error_handler.h"
#include "uncaught_error_handler_globals.h"
#include "logger.h"
#include "stdlib.h"
#include "stdbool.h"


static const char * const _logger_tag = "error_handling";
static void (*_handle_uncaught_error_callback)(error_code_t) = 0;


bool _assert_module_initialized() {
    bool uncaught_error_callback_handler_defined = _handle_uncaught_error_callback != 0;
    bool exit_function_defined = _uncaught_error_handler_exit_fn != 0;
    
    bool initialized = uncaught_error_callback_handler_defined && exit_function_defined;
    return initialized;
}

void _exit_application(error_code_t error) {
    bool exit_function_defined = _uncaught_error_handler_exit_fn != 0;
    if (!exit_function_defined) {
        log_warning(_logger_tag, "No exit function configured. Using stdlib.exit as fallback exit strategy.\n");
        exit(error);
    }

    _uncaught_error_handler_exit_fn(error);
}

void uncaught_error_handler_deinit() {
    _handle_uncaught_error_callback = 0;
    _uncaught_error_handler_exit_fn = 0;
}

void uncaught_error_handler_init(
    void (*handle_uncaught_error_callback)(error_code_t)
) {
    log_information(_logger_tag, "Initializing error_handling\n");
    _handle_uncaught_error_callback = handle_uncaught_error_callback;
    _uncaught_error_handler_exit_fn = exit;
}

void uncaught_error_handler_handle(error_code_t error_code) {
    
    if (!_assert_module_initialized()) {
        log_error(
            _logger_tag,
            "=================================================\n"
            "Uncaught exception caught but module has not been initialized!\n"
            "Continuing with fallback strategies, but to prevent memory leaks\n"
            "or error fall-through, make sure to\n"
            "- initialize the uncaught_error_handler using uncaught_error_handler_init\n"
            "- handle every Throw() in a corresponding Try/Catch()\n"
            "=========================================================================\n",            
            error_code
        );
    }

    bool error_handler_defined = _handle_uncaught_error_callback != 0;
    if(error_handler_defined) {
        log_debug(_logger_tag, "Calling configured error handler with error_code: %i", error_code);
        _handle_uncaught_error_callback(error_code);
        return;
    }

    _exit_application(error_code);
}