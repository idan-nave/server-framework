#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../src/router.h"

#define VALID_REQUEST "GET /health HTTP/1.1\r\nHost: localhost\r\nAuthorization: Bearer supersecretkey123\r\n\r\n"
#define INVALID_REQUEST "GET /invalid HTTP/1.1\r\nHost: localhost\r\nAuthorization: Bearer supersecretkey123\r\n\r\n"

// Test API response for valid request
static void test_health_check(void **state)
{
    (void)state;
    char *response = route_request(VALID_REQUEST);

    printf("🔹 Response from /health: %s\n", response); // Debugging

    assert_non_null(strstr(response, "Server is running."));
}

// Test API response for invalid endpoint
static void test_invalid_endpoint(void **state)
{
    (void)state;
    char *response = route_request(INVALID_REQUEST);

    printf("🔹 Response from invalid route: %s\n", response); // Debugging

    assert_non_null(strstr(response, "API Not Found."));
}

// Run all tests
int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_health_check),
        cmocka_unit_test(test_invalid_endpoint),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
