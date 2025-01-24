CC = gcc
FLAGS = -O2 -Wall -Wextra -I include
COMPILE = $(CC) $(FLAGS)
SOURCES = $(wildcard src/*.c)
BUILD_DIR = build
BUILD_OBJECT = $(BUILD_DIR)/vector.o
BUILD_SHARED = $(BUILD_DIR)/vector.so
BUILD_TEST = $(BUILD_DIR)/test

all: object shared

object: build
	$(COMPILE) -c -o $(BUILD_OBJECT) $(SOURCES)

shared: build
	$(COMPILE) -shared -fPIC -o $(BUILD_SHARED) $(SOURCES)

build:
	mkdir build

test:
	$(COMPILE) -fsanitize=address -g -o $(BUILD_TEST) $(wildcard test/*.c) $(SOURCES)
	ASAN_OPTIONS=allocator_may_return_null=1 $(BUILD_TEST)
