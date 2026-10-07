CC      = gcc
CFLAGS  = -Wall -Wextra -g -std=c11 -D_GNU_SOURCE
INCLUDE = -I. -Igeneric_data_structs -Igeneric_utils -Iuser_interface -Ibuissness_logic
LDLIBS_TEST = -lcunit

BIN = binaries

# Library sources (no main) from each folder.
# Test files contain their own main, so they are filtered out.
DS_SRC    = $(filter-out %_tests_i2.c, $(wildcard generic_data_structs/*.c))
UTIL_SRC  = $(wildcard generic_utils/*.c)
LOGIC_SRC = $(filter-out %_test_i2.c %_tests_i2.c, $(wildcard buissness_logic/*.c))
UI_SRC    = user_interface/db.c

LIB_SRC = $(DS_SRC) $(UTIL_SRC) $(LOGIC_SRC) $(UI_SRC)
HEADERS = $(wildcard */*.h)

.PHONY: all run test memrun memtest clean

all: $(BIN)/shop $(BIN)/db_test

$(BIN):
	mkdir -p $(BIN)

# The program: main.c + everything else
$(BIN)/shop: main.c $(LIB_SRC) $(HEADERS) | $(BIN)
	$(CC) $(CFLAGS) $(INCLUDE) main.c $(LIB_SRC) -o $@

# The tests: db_test.c + everything else (CUnit linked last)
$(BIN)/db_test: user_interface/db_test.c $(LIB_SRC) $(HEADERS) | $(BIN)
	$(CC) $(CFLAGS) $(INCLUDE) user_interface/db_test.c $(LIB_SRC) -o $@ $(LDLIBS_TEST)

run: $(BIN)/shop
	./$(BIN)/shop

test: $(BIN)/db_test
	./$(BIN)/db_test

memrun: $(BIN)/shop
	valgrind --leak-check=full --show-leak-kinds=all ./$(BIN)/shop

memtest: $(BIN)/db_test
	valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./$(BIN)/db_test

clean:
	rm -rf $(BIN)