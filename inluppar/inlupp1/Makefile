CC      = gcc
CFLAGS  = -Wall -Wextra -g -I. -pg
LDLIBS  = -lcunit

HT      = hash_table
LL      = linked_list

HT_SRC  = common.c $(HT)/hash_table.c $(HT)/hash_table_iterator.c
LL_SRC  = common.c $(LL)/linked_list.c $(LL)/linked_list_iterator.c

TESTS   = $(HT)/hash_table_test $(HT)/hash_table_iterator_test \
          $(LL)/linked_list_test $(LL)/linked_list_iterator_test

all: $(TESTS) $(HT)/frec_count

# --- hash table ---
$(HT)/hash_table_test: $(HT_SRC) $(HT)/hash_table_tests.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

$(HT)/hash_table_iterator_test: $(HT_SRC) $(HT)/hash_table_iterator_tests.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

$(HT)/frec_count: $(HT_SRC) $(HT)/frec_count.c
	$(CC) $(CFLAGS) $^ -o $@

# --- linked list ---
$(LL)/linked_list_test: $(LL_SRC) $(LL)/linked_list_test.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

$(LL)/linked_list_iterator_test: $(LL_SRC) $(LL)/linked_list_iterator_test.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

# --- running ---
test: $(TESTS)
	for t in $(TESTS); do ./$$t || exit 1; done

memtest: $(TESTS)
	for t in $(TESTS); do valgrind --leak-check=full --error-exitcode=1 ./$$t || exit 1; done

clean:
	rm -f $(TESTS) $(HT)/frec_count $(HT)/*.o $(LL)/*.o $(LL)/linked_list_iterator

.PHONY: all test memtest clean

gprof_frec_analysis1:
	./hash_table/frec_count ./hash_table/big_text.txt > ./hash_table/frec_result.txt

gprof_frec_analysis2:
	gprof ./hash_table/frec_count gmon.out > frec_analysis.txt