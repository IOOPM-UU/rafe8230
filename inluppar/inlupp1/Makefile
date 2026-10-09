CC      = gcc
CFLAGS  = -Wall -Wextra -g -I.
LDLIBS  = -lcunit

HT      = hash_table
LL      = linked_list
FC      = frec_count
BIN     = binaries
ANA     = analysis_files

O44_DIR     = $(HT)/Achievments/O44
M39_DIR     = $(HT)/Achievments/M39
M39_O44_DIR = $(HT)/Achievments/M39_O44

HT_SRC         = common.c $(HT)/hash_table.c $(HT)/hash_table_iterator.c
HT_SRC_O44     = common.c $(O44_DIR)/hash_table_O44.c $(HT)/hash_table_iterator.c
HT_SRC_M39     = common.c $(M39_DIR)/hash_table_M39.c $(M39_DIR)/hash_table_iterator_M39.c
HT_SRC_M39_O44 = common.c $(M39_O44_DIR)/hash_table_M39_O44.c $(M39_O44_DIR)/hash_table_iterator_M39_O44.c
LL_SRC         = common.c $(LL)/linked_list.c $(LL)/linked_list_iterator.c

TESTS   = $(BIN)/hash_table_test $(BIN)/hash_table_iterator_test \
          $(BIN)/linked_list_test $(BIN)/linked_list_iterator_test
O44_TESTS     = $(BIN)/hash_table_test_O44 $(BIN)/hash_table_iterator_test_O44
M39_TESTS     = $(BIN)/hash_table_test_M39 $(BIN)/hash_table_iterator_test_M39
M39_O44_TESTS = $(BIN)/hash_table_test_M39_O44 $(BIN)/hash_table_iterator_test_M39_O44
ACH_TESTS     = $(O44_TESTS) $(M39_TESTS) $(M39_O44_TESTS)

# Where the test files for each achievement come from.
# O44 keeps the original API, so it reuses the base tests in hash_table/.
# M39_O44 has no test files of its own yet, so it reuses the M39 tests.
# If you add dedicated tests in M39_O44/, point this at $(M39_O44_DIR)
# and rename the files in the two rules below.
O44_TEST_DIR     = $(HT)
M39_O44_TEST_DIR = $(M39_DIR)

# all frec_count variants that the profiling targets run
FC_BINS = $(BIN)/$(FC) $(BIN)/$(FC)_O44 $(BIN)/$(FC)_M39 $(BIN)/$(FC)_M39_O44

BIG_TEXT  = $(HT)/frec_count_tests/big_text.txt
MASSIF_IN = $(BIG_TEXT)
CG_DIR    = $(ANA)/cachegrind

all: $(TESTS) $(ACH_TESTS) $(FC_BINS)

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

# --- achievments ---
$(BIN)/$(FC)_O44: $(HT_SRC_O44) $(O44_DIR)/$(FC)_O44.c | $(BIN)
	$(CC) $(CFLAGS) -I$(HT) $^ -o $@

$(BIN)/$(FC)_M39: $(HT_SRC_M39) $(M39_DIR)/$(FC)_M39.c | $(BIN)
	$(CC) $(CFLAGS) -I$(M39_DIR) $^ -o $@

$(BIN)/$(FC)_M39_O44: $(HT_SRC_M39_O44) $(M39_O44_DIR)/$(FC)_M39_O44.c | $(BIN)
	$(CC) $(CFLAGS) -I$(M39_O44_DIR) $^ -o $@

# --- achievment tests ---
$(BIN)/hash_table_test_O44: $(HT_SRC_O44) $(O44_TEST_DIR)/hash_table_tests.c | $(BIN)
	$(CC) $(CFLAGS) -I$(HT) $^ $(LDLIBS) -o $@

$(BIN)/hash_table_iterator_test_O44: $(HT_SRC_O44) $(O44_TEST_DIR)/hash_table_iterator_tests.c | $(BIN)
	$(CC) $(CFLAGS) -I$(HT) $^ $(LDLIBS) -o $@

$(BIN)/hash_table_test_M39: $(HT_SRC_M39) $(M39_DIR)/hash_table_tests_M39.c | $(BIN)
	$(CC) $(CFLAGS) -I$(M39_DIR) $^ $(LDLIBS) -o $@

$(BIN)/hash_table_iterator_test_M39: $(HT_SRC_M39) $(M39_DIR)/hash_table_iterator_tests_M39.c | $(BIN)
	$(CC) $(CFLAGS) -I$(M39_DIR) $^ $(LDLIBS) -o $@

$(BIN)/hash_table_test_M39_O44: $(HT_SRC_M39_O44) $(M39_O44_TEST_DIR)/hash_table_tests_M39.c | $(BIN)
	$(CC) $(CFLAGS) -I$(M39_O44_DIR) $^ $(LDLIBS) -o $@

$(BIN)/hash_table_iterator_test_M39_O44: $(HT_SRC_M39_O44) $(M39_O44_TEST_DIR)/hash_table_iterator_tests_M39.c | $(BIN)
	$(CC) $(CFLAGS) -I$(M39_O44_DIR) $^ $(LDLIBS) -o $@

# --- linked list ---
$(BIN)/linked_list_test: $(LL_SRC) $(LL)/linked_list_test.c | $(BIN)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

$(BIN)/linked_list_iterator_test: $(LL_SRC) $(LL)/linked_list_iterator_test.c | $(BIN)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

# --- running the tests ---
test: $(TESTS)
	for t in $(TESTS); do ./$$t || exit 1; done

test_o44: $(O44_TESTS)
	for t in $(O44_TESTS); do ./$$t || exit 1; done

test_m39: $(M39_TESTS)
	for t in $(M39_TESTS); do ./$$t || exit 1; done

test_m39_o44: $(M39_O44_TESTS)
	for t in $(M39_O44_TESTS); do ./$$t || exit 1; done

# old name, kept so 'make test_M39' still works
test_M39: test_m39

# base tests + every achievment
test_all: test test_o44 test_m39 test_m39_o44

memtest: $(TESTS)
	for t in $(TESTS); do valgrind --leak-check=full --error-exitcode=1 ./$$t || exit 1; done

# --- profiling ---
PROF_DIR = $(HT)/frec_count_tests

# every .txt in frec_count_tests except frec_1/2/3
PROF_IN = $(filter-out $(PROF_DIR)/frec_1.txt $(PROF_DIR)/frec_2.txt $(PROF_DIR)/frec_3.txt, \
                       $(wildcard $(PROF_DIR)/*.txt))

profile: CFLAGS += -pg
profile: clean_build $(BIN)/$(FC) $(BIN)/$(FC)_O44
	mkdir -p $(ANA)
	@for f in $(PROF_IN); do \
	    name=$$(basename $$f .txt); \
	    echo "profiling $$name ..."; \
	    ./$(BIN)/$(FC) $$f > analysis_files/results/frec_result_$$name.txt; \
	    gprof -b $(BIN)/$(FC) gmon.out > analysis_files/profiles/frec_analysis_$$name.txt; \
	    ./$(BIN)/$(FC)_O44 $$f > analysis_files/results/frec_result_O44_$$name.txt; \
	    gprof -b $(BIN)/$(FC)_O44 gmon.out > analysis_files/profiles/frec_analysis_O44_$$name.txt; \
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

heap_profile: clean_build $(FC_BINS)
	mkdir -p $(ANA)/massif
	valgrind --tool=massif --stacks=yes --massif-out-file=$(ANA)/massif/frec_count.massif ./$(BIN)/$(FC) $(MASSIF_IN) > /dev/null
	valgrind --tool=massif --stacks=yes --massif-out-file=$(ANA)/massif/frec_count_O44.massif ./$(BIN)/$(FC)_O44 $(MASSIF_IN) > /dev/null
	valgrind --tool=massif --stacks=yes --massif-out-file=$(ANA)/massif/frec_count_M39.massif ./$(BIN)/$(FC)_M39 $(MASSIF_IN) > /dev/null
	valgrind --tool=massif --stacks=yes --massif-out-file=$(ANA)/massif/frec_count_M39_O44.massif ./$(BIN)/$(FC)_M39_O44 $(MASSIF_IN) > /dev/null
	ms_print $(ANA)/massif/frec_count.massif > $(ANA)/massif/frec_count.txt
	ms_print $(ANA)/massif/frec_count_O44.massif > $(ANA)/massif/frec_count_O44.txt
	ms_print $(ANA)/massif/frec_count_M39.massif > $(ANA)/massif/frec_count_M39.txt
	ms_print $(ANA)/massif/frec_count_M39_O44.massif > $(ANA)/massif/frec_count_M39_O44.txt

cachegrind_profile: clean_build $(FC_BINS)
	mkdir -p $(CG_DIR)
	valgrind --tool=cachegrind --cache-sim=yes --cachegrind-out-file=$(CG_DIR)/frec_count.cachegrind ./$(BIN)/$(FC) $(MASSIF_IN) > /dev/null
	valgrind --tool=cachegrind --cache-sim=yes --cachegrind-out-file=$(CG_DIR)/frec_count_O44.cachegrind ./$(BIN)/$(FC)_O44 $(MASSIF_IN) > /dev/null
	valgrind --tool=cachegrind --cache-sim=yes --cachegrind-out-file=$(CG_DIR)/frec_count_M39.cachegrind ./$(BIN)/$(FC)_M39 $(MASSIF_IN) > /dev/null
	valgrind --tool=cachegrind --cache-sim=yes --cachegrind-out-file=$(CG_DIR)/frec_count_M39_O44.cachegrind ./$(BIN)/$(FC)_M39_O44 $(MASSIF_IN) > /dev/null
	cg_annotate $(CG_DIR)/frec_count.cachegrind > $(CG_DIR)/frec_count_cachegrind.txt
	cg_annotate $(CG_DIR)/frec_count_O44.cachegrind > $(CG_DIR)/frec_count_O44_cachegrind.txt
	cg_annotate $(CG_DIR)/frec_count_M39.cachegrind > $(CG_DIR)/frec_count_M39_cachegrind.txt
	cg_annotate $(CG_DIR)/frec_count_M39_O44.cachegrind > $(CG_DIR)/frec_count_M39_O44_cachegrind.txt


Assembler:
	$(CC) -S -O0 -I. $(LL)/linked_list.c -o $(LL)/linked_list_O0.s
	$(CC) -S -O2 -I. $(LL)/linked_list.c -o $(LL)/linked_list_O2.s

clean_build:
	rm -f $(TESTS) $(ACH_TESTS) $(FC_BINS)
	rm -f $(BIN)/*.gcda $(BIN)/*.gcno
	rm -f $(HT)/*.o $(LL)/*.o *.o
	rm -f *.gcov */*.gcov *.gcda */*.gcda *.gcno */*.gcno
	rm -f gmon.out a.out */a.out
	rm -f $(HT)/hash_table_test $(HT)/hash_table_iterator_test $(HT)/$(FC)
	rm -f $(LL)/linked_list_test $(LL)/linked_list_iterator_test

clean: clean_build
	rm -f $(ANA)/*.txt $(ANA)/*.gcov
	rm -f $(CG_DIR)/*.cachegrind $(CG_DIR)/*.txt
	rm -f $(ANA)/massif/*.massif $(ANA)/massif/*.txt
	rm -f $(ANA)/profiles/*.txt $(ANA)/results/*.txt


.PHONY: all test test_o44 test_m39 test_m39_o44 test_M39 test_all memtest profile coverage heap_profile cachegrind_profile Assembler clean