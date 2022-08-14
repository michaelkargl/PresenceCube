#include "unity.h"
#include "stdbool.h"
#include "stdio.h"

void setUp(void) { }
void tearDown(void) { }

// A few great unity resoruces:
// https://github.com/ThrowTheSwitch/Unity/tree/master/docs
//    https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsCheatSheetSuitableforPrintingandPossiblyFraming.pdf
//    https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md

void test_ignore()
{
  TEST_IGNORE_MESSAGE("This test would fail if it weren't ignored.");
  TEST_FAIL();
}

void test_message()
{
  TEST_MESSAGE("These messages should be included in the unity output stream");
  TEST_MESSAGE("You might need to use -verbose mode to see them though.");
  TEST_MESSAGE("For instance: pio test --environment native -v");
  TEST_PASS();
}

void test_printf_message()
{
  const char* test_name = "TEST_PRINTF";
  const char* flag_name = "UNITY_INCLUDE_PRINT_FORMATTED";
  const char* message_pattern = "'%s' is only available if built using '%s' build flag.";

#ifdef UNITY_INCLUDE_PRINT_FORMATTED
  TEST_PRINTF(
      message_pattern,
      test_name,
      flag_name);
#else
  char fail_message_buffer[112];
  snprintf(fail_message_buffer, sizeof(fail_message_buffer), message_pattern, test_name, flag_name);
  TEST_FAIL_MESSAGE(fail_message_buffer);
#endif
}

void test_assertTrueFalse()
{
  TEST_ASSERT_TRUE(1 == 1);
  TEST_ASSERT_FALSE(1 == 2);
}

void test_assertIntArrays()
{
  uint8_t left[] = {1, 2, 3};
  uint8_t right[] = {1, 2, 3};
  TEST_ASSERT_EQUAL_UINT8_ARRAY(left, right, sizeof(left));
}

void test_properlyAssertStringArrays()
{
  const char *left[] = {"hello", "my", "sweetheart"};
  const char *right[] = {"hello", "my", "sweetheart"};

  // this is critical, see test_comparingTwoArraysOfDifferentSizes_canResultsInUndefinedBehavior
  const uint8_t left_size = sizeof(left) / sizeof(left[0]);
  const uint8_t right_size = sizeof(right) / sizeof(right[0]);
  TEST_ASSERT_EQUAL(left_size, right_size);

  TEST_ASSERT_EQUAL_STRING_ARRAY(left, right, sizeof(left) / sizeof(left[0]));
}

void test_comparingTwoArraysOfDifferentSizes_canResultsInUndefinedBehavior()
{
  TEST_MESSAGE(
      "BEWARE, comparing two arrays of different sizes yields undefined results."
      "        On native, when 'right' overflows, the first element is returned => test passes."
      "        On esp32s2, the overflow results in address values being interpreted as strings => test fails"
      "        So beware and always check your array lengths manually!!");

  const char *left[] = {"sweetheart", "my", "sweetheart"};
  const char *right[] = {"sweetheart", "my"};

  uint8_t right_length = sizeof(right) / sizeof(right[0]);
  TEST_ASSERT_EQUAL_STRING_ARRAY(left, right, right_length); // true

#ifdef PLATFORM_NATIVE
  // this check results only passes in native
  uint8_t left_length = sizeof(left) / sizeof(left[0]);
  TEST_ASSERT_EQUAL_STRING_ARRAY(left, right, left_length); // undefined
#endif
}


int main()
{
  UNITY_BEGIN();
  RUN_TEST(test_assertTrueFalse);
  RUN_TEST(test_ignore);
  RUN_TEST(test_message);
  RUN_TEST(test_printf_message);
  RUN_TEST(test_assertIntArrays);
  RUN_TEST(test_properlyAssertStringArrays);
  RUN_TEST(test_comparingTwoArraysOfDifferentSizes_canResultsInUndefinedBehavior);
  return UNITY_END();
}

int app_main()
{
  return main();
}