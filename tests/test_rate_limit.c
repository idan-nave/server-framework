#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../src/rate_limit.h"

// Mock request for rate limiting
static void test_rate_limiting(void **state)
{
    (void)state;
    char request[] = "GET /health HTTP/1.1";

    // Simulate 10 valid requests
    for (int i = 0; i < 10; i++)
    {
        assert_false(is_rate_limited(request));
    }

    // The 11th request should be blocked
    assert_true(is_rate_limited(request));
}

// Run all tests
int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_rate_limiting),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
