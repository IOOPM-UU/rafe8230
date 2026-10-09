CC      = gcc
CFLAGS  = -Wall -Wextra -g -std=c11 -D_GNU_SOURCE
INCLUDE = -Igeneric_data_structs -Igeneric_utils -Ibuissness_logic -Iuser_interface
BIN     = binaries

# Library code, without any file that has its own main
DS_SRC    = $(filter-out %_tests_i2.c, $(wildcard generic_data_structs/*.c))
UTIL_SRC  = $(wildcard generic_utils/*.c)
LOGIC_SRC = $(filter-out %_test.c, $(wildcard buissness_logic/*.c))
UI_SRC    = $(wildcard user_interface/*.c)

BACKEND_SRC = $(DS_SRC) $(UTIL_SRC) $(LOGIC_SRC)
HEADERS     = $(wildcard */*.h)

.PHONY: all run test memrun memtest clean

all: $(BIN)/shop $(BIN)/db_test

$(BIN):
	mkdir -p $(BIN)

# The program: main + UI + backend
$(BIN)/shop: main.c $(BACKEND_SRC) $(UI_SRC) $(HEADERS) | $(BIN)
	$(CC) $(CFLAGS) $(INCLUDE) main.c $(BACKEND_SRC) $(UI_SRC) -o $@

# The tests: backend only, no UI
$(BIN)/db_test: buissness_logic/db_test.c $(BACKEND_SRC) $(HEADERS) | $(BIN)
	$(CC) $(CFLAGS) $(INCLUDE) buissness_logic/db_test.c $(BACKEND_SRC) -o $@ -lcunit

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