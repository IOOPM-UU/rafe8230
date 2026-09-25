CC      = gcc
CFLAGS  = -Wall -Wextra -g -I.
LDLIBS  = -lcunit

HT      = hash_table
LL      = linked_list
FC      = frec_count
BIN     = binaries
ANA     = analysis_files

HT_SRC  = common.c $(HT)/hash_table.c $(HT)/hash_table_iterator.c
LL_SRC  = common.c $(LL)/linked_list.c $(LL)/linked_list_iterator.c

TESTS   = $(BIN)/hash_table_test $(BIN)/hash_table_iterator_test \
          $(BIN)/linked_list_test $(BIN)/linked_list_iterator_test

BIG_TEXT = $(HT)/frec_count_tests/big_text.txt

all: $(TESTS) $(BIN)/$(FC)

# the output directories have to exist before anything is written into them
$(BIN) $(ANA):
	mkdir -p $@

# --- hash table ---
$(BIN)/hash_table_test: $(HT_SRC) $(HT)/hash_table_tests.c | $(BIN)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

$(BIN)/hash_table_iterator_test: $(HT_SRC) $(HT)/hash_table_iterator_tests.c | $(BIN)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

$(BIN)/$(FC): $(HT_SRC) $(HT)/$(FC).c | $(BIN)
	$(CC) $(CFLAGS) $^ -o $@

# --- linked list ---
$(BIN)/linked_list_test: $(LL_SRC) $(LL)/linked_list_test.c | $(BIN)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

$(BIN)/linked_list_iterator_test: $(LL_SRC) $(LL)/linked_list_iterator_test.c | $(BIN)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

# --- running the tests ---
test: $(TESTS)
	for t in $(TESTS); do ./$$t || exit 1; done

memtest: $(TESTS)
	for t in $(TESTS); do valgrind --leak-check=full --error-exitcode=1 ./$$t || exit 1; done

# --- profiling ---
profile: CFLAGS += -pg
profile: clean $(BIN)/$(FC) | $(ANA)
	mkdir -p $(ANA)
	./$(BIN)/$(FC) $(BIG_TEXT) > $(ANA)/frec_result.txt
	gprof ./$(BIN)/$(FC) gmon.out > $(ANA)/frec_analysis.txt
	@echo "wrote $(ANA)/frec_analysis.txt -- run 'make clean' before testing again"

# --- coverage ---
coverage: CFLAGS += --coverage
coverage: clean $(TESTS)
	mkdir -p $(ANA)
	for t in $(TESTS); do ./$$t > /dev/null || exit 1; done
	gcov -b $(BIN)/hash_table_test-hash_table.gcda
	gcov -b $(BIN)/hash_table_iterator_test-hash_table_iterator.gcda
	gcov -b $(BIN)/linked_list_test-linked_list.gcda
	gcov -b $(BIN)/linked_list_iterator_test-linked_list_iterator.gcda
	mv -f *.gcov $(ANA)/
	@echo "wrote $(ANA)/*.c.gcov -- run 'make clean' before testing again"

clean:
clean:
	rm -f $(TESTS) $(BIN)/$(FC)
	rm -f $(BIN)/*.gcda $(BIN)/*.gcno
	rm -f $(HT)/*.o $(LL)/*.o *.o
	rm -f *.gcov */*.gcov *.gcda */*.gcda *.gcno */*.gcno
	rm -f gmon.out a.out */a.out
	rm -f $(HT)/hash_table_test $(HT)/hash_table_iterator_test $(HT)/$(FC)
	rm -f $(LL)/linked_list_test $(LL)/linked_list_iterator_test

.PHONY: all test memtest profile coverage clean