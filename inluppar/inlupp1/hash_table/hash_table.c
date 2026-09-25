
#include <stdio.h>
#include "hash_table.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table_private.h"
#include "common.h"



ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *h_fn, ioopm_eq_function *eq_fn) {
    // NOTE: Calloc initializes all bits to 0.
    // We therefore do not have to create a loop
    // setting all the buckets to NULL
    ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
    ht->ioopm_table_size = 0; 
    ht->hash_fn = h_fn;
    ht->hash_eq_fn = eq_fn;  
    return ht;
}

static ioopm_entry_t *ioopm_entry_create(elem_t key, elem_t value, ioopm_entry_t *next)
{
    ioopm_entry_t *entry = calloc(1, sizeof(ioopm_entry_t));
    entry->key = key;
    entry->value = value;
    entry->next = next;

    return entry; 
}

static ioopm_entry_t *entry_destroy(ioopm_entry_t *entry)
{
    // Return the entry our current entry is pointing to (next) 
    // and free the current entry
  ioopm_entry_t *next = entry->next;
  free(entry);
  return next;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
    
    // Iterate trough each bucket
    for (size_t i = 0; i < No_Buckets; i++)
    {
        // Set the current entry to the entry after the sentinel node
        ioopm_entry_t *current = ht->buckets[i].next;

        // Destroy each entry in the bucket
        while (current != NULL)
        {
            current = entry_destroy(current);
        }
    }
    // Free the hash table
    free(ht);
    return;
}

ioopm_entry_t *ioopm_find_previous_entry(ioopm_hash_table_t *ht, elem_t key)
{
    // find bucket
    size_t bucket = ht->hash_fn(key) % No_Buckets;
    
    ioopm_entry_t *previous = &ht->buckets[bucket];
    ioopm_entry_t *current = previous->next;
    
    // look for an entry with the key we want then terminate the loop and return prev
    while (current != NULL && !ht->hash_eq_fn(current->key, key) != 0)
    {
        previous = current;
        current = current->next;
    }
    return previous; 
}   

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
    ioopm_entry_t *previous = ioopm_find_previous_entry(ht, key);
    ioopm_entry_t *current = previous->next;

    
    while (current != NULL)
    {
        // Is the current key the same as the one sought after 
        if (ht->hash_eq_fn(current->key, key))
        {
            // Yes : Update result
            *result = current->value;
            // Destroy our current entry and make previous point to it 
            // (since destrying current makes it null, previous->next points to null)
            previous->next = entry_destroy(current);
            ht->ioopm_table_size--;
            return true;
        } else 
        {
            // NO : Update previous and current, iterate through loop again
            previous = current; 
            current = current->next; 
        }
    }
    
    // the bucket is empty or key is not in bucket
    *result = int_elem(-1); 
    return false;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  // find previous entry, or the last entry if the key does not exist
  ioopm_entry_t *previous = ioopm_find_previous_entry(ht, key);

  // if the key exists, update the value, otherwise create a new entry
  if (previous->next != NULL)
  {
    previous->next->value = value;
  }
  else
  {
    previous->next = ioopm_entry_create(key, value, NULL);
    ht->ioopm_table_size++; 
  }
}

bool ioopm_hash_table_lookup( ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
    ioopm_entry_t *previous = ioopm_find_previous_entry(ht, key); 
    ioopm_entry_t *current = previous->next; 

    // if the key exists, return the value, otherwise, indicate that the lookup failed
    if (current != NULL)
    {
        *result = current->value;
        return true;
    }
    else
    {
        *result = int_elem(-1); 
        return false;
    }
}


bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
    elem_t result = int_elem(-1); 
    return ioopm_hash_table_lookup(ht, key, &result); 
}

size_t ioopm_hash_table_size(const ioopm_hash_table_t *ht)
{
    
    return ht->ioopm_table_size;
}

bool ioopm_hash_table_is_empty(const ioopm_hash_table_t *ht)
{
    
    return ht->ioopm_table_size == 0; 
}


