#pragma once 

#include "hash_table_M39_O44.h"
#include "common.h"

#define No_Buckets 17
#define No_buckets_2 19319

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
  elem_t key;     // holds the key
  elem_t value;     // holds the value
  ioopm_entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  size_t no_buckets;
  size_t prime_index;
  ioopm_entry_t **buckets;
  size_t ioopm_table_size;
  ioopm_hash_function *hash_fn;
  ioopm_eq_function *hash_eq_fn; 
};


/// @brief Hash a string
/// @param key an elem_t whose .s is a null terminated string
/// @return a number refrencing the hash of the string
size_t string_knr_hash(elem_t key);

/// @brief check equality of two stirngs
/// @param a an elem_t whose .s is a null terminated string
/// @param b an elem_t whose .s is a null terminated string
/// @return true if the strings are equal, false otherwise
bool string_eq(elem_t a, elem_t b);

/// @brief Hash an integer
/// @param key an elem_t whose .i member holds an integer
/// @return the integer converted to size_t
size_t int_hash(elem_t key);

/// @brief Check equality of two integers
/// @param a an elem_t whose .i member holds an integer
/// @param b an elem_t whose .i member holds an integer
/// @return true if the integers are equal, false otherwise
bool int_eq(elem_t a, elem_t b);

