#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../include/auth.h"

// Mock request with correct API key
static void test_valid_api_key(void **state)
{
    (void)state;
    char request[] = "GET /health HTTP/1.1\r\nAuthorization: Bearer supersecretkey123";
    assert_true(validate_api_key(request));
}

// Mock request with incorrect API key
static void test_invalid_api_key(void **state)
{
    (void)state;
    char request[] = "GET /health HTTP/1.1\r\nAuthorization: Bearer wrongkey";
    assert_false(validate_api_key(request));
}

// Mock request with no API key
static void test_missing_api_key(void **state)
{
    (void)state;
    char request[] = "GET /health HTTP/1.1";
    assert_false(validate_api_key(request));
}

// Run all tests
int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_valid_api_key),
        cmocka_unit_test(test_invalid_api_key),
        cmocka_unit_test(test_missing_api_key),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
