#pragma once

#include "hash_table.h"
#include "common.h"

#define No_Buckets 17
//Severity 1: Unused No_Buckets_2

/**
 * @file hash_table_private.h
 * @author Rasmus Ferngren
 * @date 15 sep 2026
 * @brief Private file containing structs for hash table interface and iterator interface
 *
 * A common file for all implementations of hash_table and hash_table_iterator
 *
 */

struct entry
{
  elem_t key;          // holds the key
  elem_t value;        // holds the value
  ioopm_entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  // NOTE: No_buckets is hardcoded to be 17
  size_t no_buckets;
  size_t prime_index;
  ioopm_entry_t *buckets;
  size_t size; //Severity 1: changed name to size from ioopm_table_size
  ioopm_hash_function *hash_fn;
  ioopm_eq_function *hash_eq_fn;
};

//Severity 2: Declarations of hash functions that are already in common.h
