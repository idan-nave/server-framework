CC=gcc
CFLAGS=-Wall -Werror -Iinclude
LDFLAGS=-pthread

all: build

build: bin/server

bin/server: src/main.c src/http_server.c src/router.c src/auth.c src/rate_limit.c
	$(CC) $(CFLAGS) -o bin/server src/main.c src/http_server.c src/router.c src/auth.c src/rate_limit.c

clean:
	rm -rf bin/server tests/bin/

docs:
	mkdir -p docs
	echo '{ "openapi": "3.0.0", "info": { "title": "Sorting API", "version": "1.0.0" } }' > docs/openapi.json

# Ensure test binaries directory exists
test: test_dirs tests/bin/test_server tests/bin/test_auth tests/bin/test_rate_limit
	@echo "✅ Running all tests..."
	@tests/bin/test_server
	@tests/bin/test_auth
	@tests/bin/test_rate_limit

# Ensure test_dirs exists before compiling test binaries
test_dirs:
	mkdir -p tests/bin

# Correct linking order: Place -lcmocka at the end
tests/bin/test_server: tests/test_server.c src/router.c src/auth.c src/rate_limit.c
	$(CC) $(CFLAGS) -o tests/bin/test_server tests/test_server.c src/router.c src/auth.c src/rate_limit.c -lcmocka

tests/bin/test_auth: tests/test_auth.c src/auth.c
	$(CC) $(CFLAGS) -o tests/bin/test_auth tests/test_auth.c src/auth.c -lcmocka

tests/bin/test_rate_limit: tests/test_rate_limit.c src/rate_limit.c
	$(CC) $(CFLAGS) -o tests/bin/test_rate_limit tests/test_rate_limit.c src/rate_limit.c -lcmocka