CC      = gcc
CFLAGS  = -Wall -Wextra -g -std=c11 -D_GNU_SOURCE
INCLUDE = -Igeneric_data_structs -Igeneric_utils -Ibuissness_logic -Iuser_interface
BIN     = binaries

# Library code, without any file that has its own main
DS_SRC    = $(filter-out %_tests_i2.c, $(wildcard generic_data_structs/*.c))
UTIL_SRC  = $(wildcard generic_utils/*.c)
LOGIC_SRC = $(filter-out %_test.c, $(wildcard buissness_logic/*.c))
UI_SRC    = $(filter-out %_test.c, $(wildcard user_interface/*.c))

BACKEND_SRC = $(DS_SRC) $(UTIL_SRC) $(LOGIC_SRC)
HEADERS     = $(wildcard */*.h)

TESTS = $(BIN)/db_test $(BIN)/cart_test

# Coverage of the backend, measured by running the CUnit tests
COV_DIR   = cov
COV_SRC   = $(BACKEND_SRC)
COV_OBJ   = $(addprefix $(COV_DIR)/, $(notdir $(COV_SRC:.c=.o)))
COV_FLAGS = $(CFLAGS) -O0 --coverage

# Coverage of the whole program, measured by running it on scripted input
COV_FRONT_DIR = cov_frontend
FRONT_SRC     = main.c $(UI_SRC) $(BACKEND_SRC)
FRONT_OBJ     = $(addprefix $(COV_FRONT_DIR)/, $(notdir $(FRONT_SRC:.c=.o)))
UI_INPUT      = ui_test_input.txt

.PHONY: all run test carttest memrun memtest memcarttest clean shop coverage coverage_frontend coverage_all

all: $(BIN)/shop $(TESTS)

$(BIN):
	mkdir -p $(BIN)

# The program: main + UI + backend
$(BIN)/shop: main.c $(BACKEND_SRC) $(UI_SRC) $(HEADERS) | $(BIN)
	$(CC) $(CFLAGS) $(INCLUDE) main.c $(BACKEND_SRC) $(UI_SRC) -o $@

# The tests: backend only, no UI
$(BIN)/db_test: buissness_logic/db_test.c $(BACKEND_SRC) $(HEADERS) | $(BIN)
	$(CC) $(CFLAGS) $(INCLUDE) buissness_logic/db_test.c $(BACKEND_SRC) -o $@ -lcunit

$(BIN)/cart_test: buissness_logic/cart_test.c $(BACKEND_SRC) $(HEADERS) | $(BIN)
	$(CC) $(CFLAGS) $(INCLUDE) buissness_logic/cart_test.c $(BACKEND_SRC) -o $@ -lcunit

shop: $(BIN)/shop

run: $(BIN)/shop
	./$(BIN)/shop

test: $(TESTS)
	@for t in $(TESTS); do ./$$t || exit 1; done

carttest: $(BIN)/cart_test
	./$(BIN)/cart_test

memrun: $(BIN)/shop
	valgrind --leak-check=full --show-leak-kinds=all ./$(BIN)/shop

memtest: $(TESTS)
	@for t in $(TESTS); do valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./$$t || exit 1; done

memcarttest: $(BIN)/cart_test
	valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./$(BIN)/cart_test

coverage:
	rm -rf $(COV_DIR) && mkdir -p $(COV_DIR)
	for f in $(COV_SRC) buissness_logic/db_test.c buissness_logic/cart_test.c; do \
	  $(CC) $(COV_FLAGS) $(INCLUDE) -c $$f -o $(COV_DIR)/$$(basename $$f .c).o || exit 1; done
	$(CC) --coverage $(COV_DIR)/db_test.o   $(COV_OBJ) -lcunit -o $(COV_DIR)/db_test
	$(CC) --coverage $(COV_DIR)/cart_test.o $(COV_OBJ) -lcunit -o $(COV_DIR)/cart_test
	./$(COV_DIR)/db_test
	./$(COV_DIR)/cart_test
	gcov -b -c -o $(COV_DIR) $(COV_SRC) | grep -E "^File|Lines|Taken" > $(COV_DIR)/coverage_report.txt
	mv *.gcov $(COV_DIR)/
	@cat $(COV_DIR)/coverage_report.txt

coverage_frontend:
	rm -rf $(COV_FRONT_DIR) && mkdir -p $(COV_FRONT_DIR)
	for f in $(FRONT_SRC); do \
	  $(CC) $(COV_FLAGS) $(INCLUDE) -c $$f -o $(COV_FRONT_DIR)/$$(basename $$f .c).o || exit 1; done
	$(CC) --coverage $(FRONT_OBJ) -o $(COV_FRONT_DIR)/shop_cov
	./$(COV_FRONT_DIR)/shop_cov < $(UI_INPUT) > /dev/null
	gcov -b -c -o $(COV_FRONT_DIR) $(FRONT_SRC) | grep -E "^File|Lines|Taken" > $(COV_FRONT_DIR)/frontend_coverage_report.txt
	mv *.gcov $(COV_FRONT_DIR)/
	@cat $(COV_FRONT_DIR)/frontend_coverage_report.txt

coverage_all: coverage coverage_frontend

clean:
	rm -rf $(BIN) $(COV_DIR) $(COV_FRONT_DIR)