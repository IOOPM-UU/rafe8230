## Generic hash table

The generic hash table uses one combined implementation: a dynamically resized array of bucket pointers, with linked chains for collisions. It grows and shrinks as entries are inserted and removed. From this directory, run `make -f MakeFile test` to build and run its tests.

The public API is declared in `generic_data_structs/hash_table.h`.