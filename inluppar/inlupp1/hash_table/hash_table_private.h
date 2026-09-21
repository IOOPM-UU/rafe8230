#pragma once 

#include "hash_table.h"
#include "common.h"

#define No_Buckets 17


struct entry
{
  elem_t key;     // holds the key
  elem_t value;     // holds the value
  ioopm_entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  // DODGE: hard-coding number of buckets as 17.
  // NOTE: addressing this dodge is optional.
  ioopm_entry_t buckets[No_Buckets];
  size_t ioopm_table_size;
  ioopm_hash_function *hash_fn;
  ioopm_eq_function *hash_eq_fn; 
};




