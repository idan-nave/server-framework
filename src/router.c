#include "router.h"
#include "auth.h"
#include "rate_limit.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RESPONSE_SIZE 4096

char *route_request(const char *request)
{
    static char response[RESPONSE_SIZE];

    // Check rate limiting
    if (is_rate_limited(request))
    {
        snprintf(response, RESPONSE_SIZE,
                 "HTTP/1.1 429 Too Many Requests\r\nContent-Type: text/plain\r\n\r\nRate limit exceeded.");
        return response;
    }

    // Validate API key
    if (!validate_api_key(request))
    {
        snprintf(response, RESPONSE_SIZE,
                 "HTTP/1.1 401 Unauthorized\r\nContent-Type: text/plain\r\n\r\nInvalid API key.");
        return response;
    }

    // Handle requests
    if (strstr(request, "GET /health") != NULL)
    {
        snprintf(response, RESPONSE_SIZE,
                 "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nServer is running.");
    }
    else if (strstr(request, "GET /docs") != NULL)
    {
        FILE *file = fopen("docs/openapi.json", "r");
        if (file)
        {
            fread(response, 1, RESPONSE_SIZE - 1, file);
            fclose(file);
            snprintf(response, RESPONSE_SIZE,
                     "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n%s", response);
        }
        else
        {
            snprintf(response, RESPONSE_SIZE,
                     "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\n\r\nFailed to load documentation.");
        }
    }
    else
    {
        snprintf(response, RESPONSE_SIZE,
                 "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nAPI Not Found.");
    }

    return response;
}
