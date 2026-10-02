CC      = gcc
CFLAGS  = -Wall -Wextra -g -I.
LDLIBS  = -lcunit

HT      = hash_table
LL      = linked_list
FC      = frec_count
BIN     = binaries
ANA     = analysis_files

HT_SRC  = common.c $(HT)/hash_table.c $(HT)/hash_table_iterator.c
HT_SRC_O44  = common.c $(HT)/hash_table_O44.c $(HT)/hash_table_iterator.c
LL_SRC  = common.c $(LL)/linked_list.c $(LL)/linked_list_iterator.c

TESTS   = $(BIN)/hash_table_test $(BIN)/hash_table_iterator_test \
          $(BIN)/linked_list_test $(BIN)/linked_list_iterator_test

BIG_TEXT = $(HT)/frec_count_tests/big_text.txt

all: $(TESTS) $(BIN)/$(FC) $(BIN)/$(FC)_O44

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

$(BIN)/$(FC)_O44: $(HT_SRC_O44) $(HT)/$(FC)_O44.c | $(BIN)
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
PROF_DIR = $(HT)/frec_count_tests

# every .txt in frec_count_tests except frec_1/2/3
PROF_IN = $(filter-out $(PROF_DIR)/frec_1.txt $(PROF_DIR)/frec_2.txt $(PROF_DIR)/frec_3.txt, \
                       $(wildcard $(PROF_DIR)/*.txt))

profile: CFLAGS += -pg
profile: clean $(BIN)/$(FC) $(BIN)/$(FC)_O44 
	mkdir -p $(ANA)
	@for f in $(PROF_IN); do \
	    name=$$(basename $$f .txt); \
	    echo "profiling $$name ..."; \
	    ./$(BIN)/$(FC) $$f > analysis_files/frec_result_$$name.txt; \
	    gprof -b $(BIN)/$(FC) gmon.out > analysis_files/frec_analysis_$$name.txt; \
	    ./$(BIN)/$(FC)_O44 $$f > analysis_files/frec_result_O44_$$name.txt; \
	    gprof -b $(BIN)/$(FC)_O44 gmon.out > analysis_files/frec_analysis_O44_$$name.txt; \
	done
	@rm -f gmon.out
	@echo ""
	@echo "wrote $(ANA)/frec_result_*.txt and $(ANA)/frec_analysis_*.txt"
	@echo "run 'make clean' before testing again"

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


Assembler:
	$(CC) -S -O0 -I. $(LL)/linked_list.c -o $(LL)/linked_list_O0.s
	$(CC) -S -O2 -I. $(LL)/linked_list.c -o $(LL)/linked_list_O2.s

clean:
	rm -f $(TESTS) $(BIN)/$(FC) $(BIN)/$(FC)_O44
	rm -f $(BIN)/*.gcda $(BIN)/*.gcno
	rm -f $(HT)/*.o $(LL)/*.o *.o
	rm -f *.gcov */*.gcov *.gcda */*.gcda *.gcno */*.gcno
	rm -f gmon.out a.out */a.out
	rm -f $(HT)/hash_table_test $(HT)/hash_table_iterator_test $(HT)/$(FC)
	rm -f $(LL)/linked_list_test $(LL)/linked_list_iterator_test
	rm -f $(ANA)/*.txt


.PHONY: all test memtest profile coverage clean