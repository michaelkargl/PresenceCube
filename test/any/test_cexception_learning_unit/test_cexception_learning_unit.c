#include "unity.h"
#include "stdio.h"
#include "exception_handling.h"

#define FAKE_DIVIDE_BY_ZERO_ERROR_CODE 100
#define FAKE_UNHANDLED_ERROR_CODE 999
#define FAKE_NULL_REFERENCE_ERROR_CODE 123


double divide_numbers(int number, int divisor) {
    if (divisor == 0) {
        Throw(FAKE_DIVIDE_BY_ZERO_ERROR_CODE);
    }

    return number / divisor;
}

void test_throw_outside_try_catch_passes() {
    TEST_MESSAGE(
        "A Throw clause must reside in at least 1 "
        "Try block to be affective / safe to use."
        " This test only demonstrates this undefined behavior."
    );
    Throw(FAKE_UNHANDLED_ERROR_CODE);
}

void test_divide_numbers_validinputs_passes() {
    int result = divide_numbers(10, 2);
    TEST_ASSERT_EQUAL(5, result);
}

void test_divice_numbers_variablemutationsareapplied() {

    // TLDR: if counter wouldn't be volatile, the compiler would warn:
    //    warning: variable 'counter' might be clobbered by 'longjmp' or 'vfork' [-Wclobbered]
    //
    // We need to tell the compiler that it should leave our variable alone by marking it as `volatile`.  
    // Quoting https://github.com/ThrowTheSwitch/CException#local-variables:
    //     If (a) you change local (stack) variables within your Try block, and 
    //        (b) wish to make use of the updated values after an exception is thrown,
    //     those variables should be made volatile.
    //
    //     Note that this is ONLY for locals and ONLY when you need access to them after a Throw.
    //     Compilers optimize (and thank goodness they do). There is no way to guarantee that the
    //      actual memory location was updated and not just a register unless the variable is 
    //      marked volatile.
    //    
    volatile int counter = 0;

    CEXCEPTION_T e;
    Try {
        counter = 2;
        Throw(FAKE_NULL_REFERENCE_ERROR_CODE);
    } Catch(e) {
        counter += 2;
    }

    TEST_ASSERT_EQUAL_MESSAGE(4, counter, "Equals due to the volatile nature of 'counter'");
}

void test_nested_trycatch_statements() {
    volatile CEXCEPTION_T outer_ex, inner_ex;

    Try {        
        Try {
            Throw(FAKE_UNHANDLED_ERROR_CODE);
        } Catch(outer_ex) {
            TEST_ASSERT_EQUAL(FAKE_UNHANDLED_ERROR_CODE, inner_ex);
            Throw(FAKE_NULL_REFERENCE_ERROR_CODE);
            TEST_FAIL_MESSAGE("Should not be reachable due to Throw statement above.");
        }
    } Catch(outer_ex) {
        TEST_ASSERT_EQUAL(FAKE_NULL_REFERENCE_ERROR_CODE, outer_ex);
    }
}

void test_divide_numbers_zerodivisor_throwsdividebyzeroexception() {
   volatile CEXCEPTION_T e;
    Try {
        divide_numbers(1, 0);
        TEST_FAIL_MESSAGE("Should Have Thrown An Error");
    } Catch(e) {
        TEST_ASSERT_EQUAL(e, FAKE_DIVIDE_BY_ZERO_ERROR_CODE);
    }

    // the error code e is retained after the catch
    TEST_ASSERT_EQUAL(e, FAKE_DIVIDE_BY_ZERO_ERROR_CODE);
}

// native
int main() {
    UNITY_BEGIN();
    
    volatile CEXCEPTION_T e;
    Try {
        RUN_TEST(test_divice_numbers_variablemutationsareapplied);
        RUN_TEST(test_divide_numbers_validinputs_passes);
        RUN_TEST(test_throw_outside_try_catch_passes);
        RUN_TEST(test_divide_numbers_zerodivisor_throwsdividebyzeroexception);
        RUN_TEST(test_nested_trycatch_statements);
        
    } Catch(e) {
        TEST_PRINTF("Unhandled exception caught: %i", e);
        TEST_MESSAGE(
            "Internal state tracked by Try-Blocks is released in Catch-Blocks "
            "=> throwing without catching results in memory leaks."
        );
    }

    return UNITY_END();
}

// esp32
int app_main() {
    return main();
}