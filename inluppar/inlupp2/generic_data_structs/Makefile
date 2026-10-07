CC ?= gcc
CFLAGS ?= -Wall -Wextra -g

COMMON = ../generic_utils/common.c
HT_SOURCES = hash_table.c hash_table_iterator.c $(COMMON)
CPPFLAGS += -I../generic_utils
TEST_BINS = build/hash_table_test build/hash_table_iterator_test

.PHONY: all test clean

all: test

test: $(TEST_BINS)
	@for test in $(TEST_BINS); do ./$$test || exit 1; done

build:
	mkdir -p $@

build/hash_table_test: hash_table_tests.c $(HT_SOURCES) hash_table.h hash_table_private.h | build
	$(CC) $(CPPFLAGS) $(CFLAGS) $(HT_SOURCES) $< -lcunit -o $@

build/hash_table_iterator_test: hash_table_iterator_tests.c $(HT_SOURCES) hash_table.h hash_table_private.h hash_table_iterator.h | build
	$(CC) $(CPPFLAGS) $(CFLAGS) $(HT_SOURCES) $< -lcunit -o $@

clean:
	rm -rf build
