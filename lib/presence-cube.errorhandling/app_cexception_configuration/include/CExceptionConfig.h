#pragma once
#include "uncaught_error_handler.h"


// unless CEXCEPTION_USE_CONFIG_FILE compiler flag is undefined,
// this file will be included by the CException automatically


// defines the error code datatype
#define CEXCEPTION_T error_code_t

// set this to a number which will never be an exception id in your system
#define CEXCEPTION_NONE ERROR_CODE_SUCCESS

// If in a multi-tasking environment, this should be set to be a call to the 
// function described in #2 above. It defaults to just return 0 all the time
// (good for single tasking environments, not so good otherwise).
// #define CEXCEPTION_GET_ID xxx

// If in a multi-tasking environment, this should be set to the number of ID's
// required (usually the number of tasks in the system). Defaults to 1 
// (good for single tasking environments or systems where you will only use 
// this from one task).
// # define CEXCEPTION_NUM_ID xxx

// Function call when a Throw is made outside of Try...Catch protection. Consider 
// this the emergency fallback plan for when something has gone terribly wrong.
#define CEXCEPTION_NO_CATCH_HANDLER(id) uncaught_error_handler_handle(id)


// called immediately before the Try block
// #define CEXCEPTION_HOOK_START_TRY

// called immediately after the Try block if no exception was thrown
// #define CEXCEPTION_HOOK_HAPPY_TRY

// called immediately after the Try block OR before an exception is caught
// #define CEXCEPTION_HOOK_AFTER_TRY

// called immediately before the catch
// #define CEXCEPTION_HOOK_START_CATCH