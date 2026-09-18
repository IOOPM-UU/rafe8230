#pragma once 

#include "hash_table.h"

#define No_Buckets 17


struct entry
{
  char *key;     // holds the key
  int value;     // holds the value
  ioopm_entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  // DODGE: hard-coding number of buckets as 17.
  // NOTE: addressing this dodge is optional.
  ioopm_entry_t buckets[No_Buckets];
  size_t ioopm_table_size;
};




