#pragma once

#ifdef PLATFORM_TEST
    extern void (*_uncaught_error_handler_exit_fn)(int);    
#endif

void (*_uncaught_error_handler_exit_fn)(int);


