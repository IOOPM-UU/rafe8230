#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "common.h"

/**
* @file hash_table.h
* @author write both your names here
* @date write the date you started working on this
* @brief Simple hash table that maps string keys to integer values.
*
* Here typically goes a more extensive explanation of what the header
* defines. Doxygens tags are words preceeded by either a backslash @\
* or by an at symbol @@.
*
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
/// @return the value mapped to by key (FIXME: what if the key does not exist?)
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
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
