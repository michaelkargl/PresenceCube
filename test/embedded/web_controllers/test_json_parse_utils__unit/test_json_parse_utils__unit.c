#include "unity.h"
#include "json_parse_utils.h"
#include "assertion_helpers.h"

static cJSON *_test_document;
#define ID_PROPERTY_KEY "id"
#define ID_PROPERTY_VALUE 42
#define NAME_PROPERTY_KEY "name"

void setUp()
{
    _test_document = cJSON_CreateObject();
    cJSON_AddNumberToObject(_test_document, ID_PROPERTY_KEY, ID_PROPERTY_VALUE);
    cJSON_AddStringToObject(_test_document, NAME_PROPERTY_KEY, "Answer");
}
void tearDown()
{
    cJSON_Delete(_test_document);
}

void test_get_number_property__given_null__throws()
{
    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        get_number_property(NULL, ID_PROPERTY_KEY);
    });

    TEST_ASSERT_THROWS(ERROR_CODE_ARGUMENT_NULL, {
        get_number_property(_test_document, NULL);
    });
}

void test_get_number_property__given_unknown_property_name__throws()
{
    TEST_ASSERT_THROWS(ERROR_CODE_RESOURCE_NOT_FOUND, {
        get_number_property(_test_document, "unknown property");
    });
}

void test_get_number_property__given_property_name_pointing_to_string__throws()
{
    TEST_ASSERT_THROWS(ERROR_CODE_INVALID_TYPE, {
        get_number_property(_test_document, NAME_PROPERTY_KEY);
    });
}

void test_get_number_property__returns_json_number_property()
{
    cJSON *number = get_number_property(_test_document, ID_PROPERTY_KEY);

    TEST_ASSERT_NOT_NULL(number);
    TEST_ASSERT_EQUAL(ID_PROPERTY_VALUE, number->valueint);
}

int main()
{
    UNITY_BEGIN();

    TEST_ASSERT_THROWS_NOT({
        RUN_TEST(test_get_number_property__given_null__throws);
        RUN_TEST(test_get_number_property__given_unknown_property_name__throws);
        RUN_TEST(test_get_number_property__given_property_name_pointing_to_string__throws);
        RUN_TEST(test_get_number_property__returns_json_number_property);
    });

    return UNITY_END();
}

int app_main()
{
    return main();
}