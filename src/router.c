#include "router.h"
#include "auth.h"
#include "rate_limit.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RESPONSE_SIZE 4096
#define JSON_SIZE 4000 // Buffer for OpenAPI JSON

char *route_request(const char *request)
{
    static char response[RESPONSE_SIZE];
    static char json_buffer[JSON_SIZE]; // Separate buffer for JSON

    // **Handle Public Routes First** (No API Key Required)
    if (strstr(request, "GET /docs") != NULL)
    {
        FILE *file = fopen("docs/openapi.json", "r");
        if (file)
        {
            size_t bytes_read = fread(json_buffer, 1, JSON_SIZE - 1, file);
            fclose(file);
            json_buffer[bytes_read] = '\0'; // Null-terminate the JSON string

            snprintf(response, RESPONSE_SIZE,
                     "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %zu\r\n\r\n%s",
                     bytes_read, json_buffer);
        }
        else
        {
            snprintf(response, RESPONSE_SIZE,
                     "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\n\r\nFailed to load documentation.");
        }
        return response;
    }

    // **Check Authentication for Protected Routes**
    if (!validate_api_key(request))
    {
        snprintf(response, RESPONSE_SIZE,
                 "HTTP/1.1 401 Unauthorized\r\nContent-Type: text/plain\r\n\r\nInvalid API key.");
        return response;
    }

    // **Rate Limiting (For Authenticated Users)**
    if (is_rate_limited(request))
    {
        snprintf(response, RESPONSE_SIZE,
                 "HTTP/1.1 429 Too Many Requests\r\nContent-Type: text/plain\r\n\r\nRate limit exceeded.");
        return response;
    }

    // **Handle API Endpoints**
    if (strstr(request, "GET /health") != NULL)
    {
        snprintf(response, RESPONSE_SIZE,
                 "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nServer is running.");
    }
    else
    {
        snprintf(response, RESPONSE_SIZE,
                 "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nAPI Not Found.");
    }

    return response;
}
