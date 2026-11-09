CC=gcc

CFLAGS=-Wall -Wextra -g

PROGRAM_NAME=train
TEST_NAME=$(PROGRAM_NAME)_test

all: $(TEST_NAME)

$(PROGRAM_NAME): $(PROGRAM_NAME).c
	$(CC) $(CFLAGS) $< -o $@

$(TEST_NAME): $(PROGRAM_NAME).c $(TEST_NAME).c
	$(CC) $(CFLAGS) -D TEST $^ -o $@ -lcunit

run: $(PROGRAM_NAME)
	./$<

test: $(TEST_NAME)
	./$<

memrun: $(PROGRAM_NAME)
	valgrind --leak-check=full ./$<

memtest: $(TEST_NAME)
	valgrind --leak-check=full ./$(TEST_NAME)

clean:
	rm -f $(TEST_NAME)
	rm -f $(PROGRAM_NAME)
	rm -f *.o

.PHONY: run memrun test memtest clean all
