#include <stdio.h>
#include <stdlib.h>
#include "http_server.h"

#define PORT 8080

int main() {
    printf("🚀 Starting API Server on port %d...\n", PORT);
    start_server(PORT);
    return 0;
}

