CC = gcc
FLAGS = -O2 -Wall -I include
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

test: build
	$(COMPILE) -g -o $(BUILD_TEST) $(wildcard test/*.c) $(SOURCES)
	$(BUILD_TEST)
