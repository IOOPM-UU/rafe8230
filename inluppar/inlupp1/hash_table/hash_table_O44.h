#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "common.h"

typedef bool ioopm_eq_function(elem_t a, elem_t b);
typedef size_t ioopm_hash_function(elem_t key);

/**
* @file hash_table.h
* @author Rasmus Ferngren
* @date 7 sep 2026
* @brief Simple hash table that maps string keys to integer values.
*
* Hash table interface
* A hashtable consists of an array of buckets. Each bucket contains entries. Each entry contains a key, value and pointer to next entry. 
*/

typedef struct hash_table ioopm_hash_table_t;
typedef struct entry ioopm_entry_t;


/// @brief Create a new empty hashtable
/// @param h_fn Hash function
/// @param eq_fn Equal function
/// @return The created hashtable
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *h_fn, ioopm_eq_function *eq_fn);


/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @param result The value looked at
/// @return the value mapped to by key (If key does not exsist, result = 0)
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @param result The value removed
/// @return the value mapped to by key, if key does not exsist, result = 0
bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, elem_t *result);

/// @brief find a given key in a mapping
/// @param ht hash table operated upon
/// @param key key to find
/// @return a boolean representing if key was found or not
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief Is the hash table empty or not
/// @param ht hash table operated upon
/// @return a boolean representing if its empty or not
bool ioopm_hash_table_is_empty(const ioopm_hash_table_t *ht);

/// @brief Counts the size of the hash table
/// @param ht hash table operated upon
/// @return size of hash table
size_t ioopm_hash_table_size(const ioopm_hash_table_t *ht);
