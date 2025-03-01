#include "auth.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENV_FILE ".env"
#define API_KEY_SIZE 64

// Load API key from .env file
char *get_api_key()
{
    static char api_key[64];
    FILE *file = fopen(".env", "r");

    if (!file)
    {
        fprintf(stderr, "⚠️ Error: Could not load .env file!\n");
        return NULL;
    }

    while (fgets(api_key, sizeof(api_key), file))
    {
        if (strncmp(api_key, "API_KEY=", 8) == 0)
        {
            fclose(file);

            // Remove newline if present
            char *newline = strchr(api_key, '\n');
            if (newline)
                *newline = '\0';

            printf("🔹 Loaded API Key (after processing): '%s'\n", api_key + 8);
            return api_key + 8;
        }
    }

    fclose(file);
    return NULL;
}

// Validate API key from the request
int validate_api_key(const char *request) {
    char *valid_key = get_api_key();
    if (!valid_key) return 0;

    char *auth_header = strstr(request, "Authorization: Bearer ");
    if (!auth_header) return 0;

    char api_key[64];
    sscanf(auth_header, "Authorization: Bearer %63s", api_key);

    return strcmp(api_key, valid_key) == 0;
}

