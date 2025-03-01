#include "rate_limit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_REQUESTS 10  // 10 requests per minute
#define WINDOW_SIZE 60   // 60 seconds

typedef struct {
    char ip[64];
    time_t timestamps[MAX_REQUESTS];
    int count;
} RateLimitEntry;

RateLimitEntry clients[100];  // Store up to 100 clients
int client_count = 0;

// Mock function to extract IP from request
char* extract_ip(const char* request) {
    return "127.0.0.1";  // For now, assuming localhost
}

// Check if the IP is rate-limited
int is_rate_limited(const char *request) {
    char *ip = extract_ip(request);
    time_t now = time(NULL);

    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].ip, ip) == 0) {
            int new_count = 0;
            for (int j = 0; j < clients[i].count; j++) {
                if (now - clients[i].timestamps[j] < WINDOW_SIZE) {
                    clients[i].timestamps[new_count++] = clients[i].timestamps[j];
                }
            }
            clients[i].count = new_count;

            if (clients[i].count >= MAX_REQUESTS) {
                return 1;  // Rate limit exceeded
            }

            clients[i].timestamps[clients[i].count++] = now;
            return 0;
        }
    }

    // New client
    strcpy(clients[client_count].ip, ip);
    clients[client_count].timestamps[0] = now;
    clients[client_count].count = 1;
    client_count++;

    return 0;
}

