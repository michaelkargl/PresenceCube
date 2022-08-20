#include "unity.h"
#include "CException.h"
#include "uncaught_error_handler.h"

static error_code_t _handled_error;
static int exit_error_code = -1;

// statement of trust that this variable is defined somewhere.
// make that declaration available in this compilation unit
extern void (*_uncaught_error_handler_exit_fn)(int);

static void _fake_handle_uncaught_error(error_code_t code);
static void _fake_exit_function(int error_code);
static void _run_order_sensitive_tests();


void setUp() {
    _handled_error = ERROR_CODE_SUCCESS;
}

void test_uncaughterror_uninitializedhandler_succeedswithwarnings() {
    _uncaught_error_handler_exit_fn = _fake_exit_function;
    Throw(ERROR_CODE_UNHANDLED);
    TEST_ASSERT_EQUAL(ERROR_CODE_UNHANDLED, exit_error_code);
}

void test_caughterror_fallbackhandlernotcalled() {
    uncaught_error_handler_init(_fake_handle_uncaught_error);

    volatile CEXCEPTION_T e;
    Try {
        Throw(ERROR_CODE_UNHANDLED);
    } Catch(e) {
        TEST_ASSERT_EQUAL(ERROR_CODE_UNHANDLED, e);
    }

    TEST_ASSERT_EQUAL(ERROR_CODE_SUCCESS, _handled_error);
}

void test_uncaughterror_caughtbyfallbackcallback() {
    uncaught_error_handler_init(_fake_handle_uncaught_error);
    Throw(ERROR_CODE_UNHANDLED);

    // Thw "Throw" does not have a corresponding "Catch" which means 
    // that no jump is performed => the catch falls through.
    // Unless our fake callback aborts of course.
    TEST_ASSERT_EQUAL(ERROR_CODE_UNHANDLED, _handled_error);
}

// native
int main() {
    UNITY_BEGIN();

    _run_order_sensitive_tests();
    RUN_TEST(test_uncaughterror_caughtbyfallbackcallback);
    RUN_TEST(test_caughterror_fallbackhandlernotcalled);
    
    return UNITY_END();
}

static void _run_order_sensitive_tests() {
    // Keep this test first: it requires an uninitialized / unaltered application state to bring the most value
    RUN_TEST(test_uncaughterror_uninitializedhandler_succeedswithwarnings);
}

// esp-idf
int app_main() {
    return main();
}


static void _fake_handle_uncaught_error(error_code_t code) {
    TEST_PRINTF("Mocked handler called for unhandled error: %i", code);
    _handled_error = code;
}

static void _fake_exit_function(int error_code) {
    exit_error_code = error_code;
}