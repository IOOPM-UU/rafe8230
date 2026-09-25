# Inlupp 1 — Hash table and linked list

Rasmus Ferngren - (rafe8230)

A generic hash table and a generic linked list in C, both with iterators, plus a
word frequency counter (`frec_count`) that uses the hash table.

All commands below are run from `inluppar/inlupp1/`.

## Requirements

* `gcc`
* CUnit (`-lcunit`)
* `valgrind`
* `gprof`

## Repository layout

```
inlupp1/
├── Makefile
├── common.c / common.h        elem_t, function pointer types,
│                              and the hash/equality functions for
│                              strings and integers
├── hash_table/
│   ├── hash_table.c / .h      the hash table
│   ├── hash_table_private.h   struct definitions, shared between
│   │                          hash_table.c and hash_table_iterator.c
│   ├── hash_table_iterator.c / .h
│   ├── hash_table_tests.c     CUnit tests for the hash table
│   ├── hash_table_iterator_tests.c
│   ├── frec_count.c           word frequency program
│   ├── big_text.txt           large test input for word frequency
│   └── frec_*.txt             small test input for word frequency 
└── linked_list/
    ├── linked_list.c / .h     The linked list
    ├── linked_list_private.h  
    ├── linked_list_iterator.c / .h 
    ├── linked_list_test.c
    └── linked_list_iterator_test.c
```

## Build

```bash
make              # compiles all files including tests and frec_count
make clean        # removes all binaries and generated files
```

Individual targets:

```bash
make hash_table/hash_table_test
make hash_table/hash_table_iterator_test
make linked_list/linked_list_test
make linked_list/linked_list_iterator_test
make hash_table/frec_count
```

## Run the tests

```bash
make test         # builds and runs all four CUnit suites
```

or run one suite at a time:

```bash
./hash_table/hash_table_test
./hash_table/hash_table_iterator_test
./linked_list/linked_list_test
./linked_list/linked_list_iterator_test
```

### Memory tests

Several functions have no return value to assert on.
The correctness is verified with valgrind instead of with CUnit assertions.

```bash
make memtest      # runs all test suites under valgrind
```

or manually:

```bash
valgrind --leak-check=full ./hash_table/hash_table_test
valgrind --leak-check=full ./hash_table/hash_table_iterator_test
valgrind --leak-check=full ./linked_list/linked_list_test
valgrind --leak-check=full ./linked_list/linked_list_iterator_test
```

Expected output: `All heap blocks were freed -- no leaks are possible`,
matching alloc/free counts, and `ERROR SUMMARY: 0 errors`.

## Run frec_count

The program counts how often each word occurs in one or more text files and
prints the result in descending frequency order.

```bash
./hash_table/frec_count hash_table/frec_1.txt
./hash_table/frec_count hash_table/frec_1.txt hash_table/frec_2.txt hash_table/frec_3.txt
./hash_table/frec_count hash_table/big_text.txt
```

Output format is `<word>: <count>`

Check it for leaks with:

```bash
valgrind --leak-check=full ./hash_table/frec_count hash_table/frec_1.txt
```

## Profiling

To profile `frec_count`, `-pg` has to be added to `CFLAGS` in the Makefile.
Then:

```bash
make clean && make
./hash_table/frec_count <a large text file> > /dev/null
gprof ./hash_table/frec_count gmon.out > frec_analysis.txt
```

`gmon.out` is written to the current directory when the program exits normally.

**Note:** `-pg` should be removed from `CFLAGS` again afterwards, since it slows
the program down considerably.

## Design decisions

* **Generic keys and values via `elem_t`** (a union in `common.h`). Since C has
  no run-time type information, `ioopm_hash_table_create` takes two function
  pointers — one hash function and one equality function — which are stored in
  the table and used internally wherever keys are hashed or compared.
* **Collisions are handled by separate chaining.** Each bucket holds a *sentinel
  node* stored by value in the table struct. Traversal always starts at
  `buckets[i].next`, which means `insert` and `remove` never need a special case
  for "first in the bucket". The sentinels are part of the table's own
  allocation and must never be freed individually.
* **The number of buckets is hard-coded to 17** (`No_Buckets` in
  `hash_table_private.h`). This is a known limitation: profiling with gprof
  shows that with around 12,000 distinct keys, about 88% of the runtime is spent walking
  chains. Increasing the bucket count is the obvious optimisation.
* **The size is stored in a counter field** in both the hash table and the
  linked list, so `size` and `is_empty` are O(1). `insert`/`append`/`remove`
  keep the counter updated.
* **The linked list stores a `last` pointer** so `append` is O(1). Note that
  `get` is O(n) — the list must be traversed. Every operation that can change
  the end of the list (`append`, `prepend`, `insert`, `remove`) maintains
  `last`.
* **Error handling:** `lookup`, `remove` and `has_key` return `bool` and write
  the result through an out-parameter, since no `elem_t` value can be reserved
  as an error code. On failure, the out-parameter is set to `int_elem(-1)` and
  the function returns `false`. The list functions (`get`, `head`, `last`,
  `remove`) and `iterator_current` return `-1` for invalid indices or an empty
  list, which is ambiguous when `-1` is a legitimate stored value — callers
  should use `size`/`is_empty`/`at_end` to distinguish.
* **Iterators must not outlive modifications to the structure.** Modifying a
  list or a hash table while an iterator is active invalidates the iterator.
  This is not checked at run time.
* **Encapsulation:** the struct definitions live in `*_private.h`, which is
  included by the implementation files (and by the test files, which are
  deliberately white-box). Client code such as `frec_count.c` includes only the
  public headers and therefore cannot access the internals.
* **`size_t` is used for all sizes and indices**, which means negative indices
  are impossible by construction. A single `index >= size` check covers both
  ends of the range.