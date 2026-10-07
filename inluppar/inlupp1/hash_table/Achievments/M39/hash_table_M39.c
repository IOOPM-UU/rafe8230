
#include <stdio.h>
#include "hash_table_M39.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table_private_M39.h"
#include "common.h"
#include "hash_table_iterator_M39.h"



ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *h_fn, ioopm_eq_function *eq_fn) {
    // NOTE: Calloc initializes all bits to 0.
    // We therefore do not have to create a loop
    // setting all the buckets to NULL
    ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
    ht->ioopm_table_size = 0; 
    ht->hash_fn = h_fn;
    ht->hash_eq_fn = eq_fn;  
    ht->no_buckets = No_Buckets;
    ht->prime_index = 0; 
    ht->buckets = calloc(No_Buckets, sizeof(ioopm_entry_t *));
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

// Frees all entries and the bucket array, but NOT the struct itself
static void free_buckets(ioopm_hash_table_t *ht)
{
    for (size_t i = 0; i < ht->no_buckets; i++)
    {
        ioopm_entry_t *entry = ht->buckets[i];
        while (entry)
        {
            entry = entry_destroy(entry); 
        }
    }
    free(ht->buckets);
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
    free_buckets(ht);
    free(ht);
}


ioopm_entry_t **ioopm_find_entry(ioopm_hash_table_t *ht, elem_t key)
{
    // find bucket
    size_t bucket = ht->hash_fn(key) % ht->no_buckets;
    
    ioopm_entry_t **entry = &ht->buckets[bucket];
    
    // look for an entry with the key we want then terminate the loop and return prev
    while (*entry != NULL && !ht->hash_eq_fn((*entry)->key, key) != 0)
    {
        entry = &(*entry)->next;
    }

    return entry; 
}   

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{

    ioopm_entry_t **previous_point = ioopm_find_entry(ht, key);
    if (*previous_point == NULL)
    {
        *result = int_elem(-1); 
        return false;
    }
    
    ioopm_entry_t *tmp = *previous_point;
    
    *result = tmp->value;
    *previous_point = tmp->next; 
    free(tmp); 
    ht->ioopm_table_size--;
    return true; 

}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  // find previous entry, or the last entry if the key does not exist
  ioopm_entry_t **previous_point = ioopm_find_entry(ht, key);

  // if the key exists, update the value, otherwise create a new entry
  if (*previous_point != NULL)
  {
    (*previous_point)->value = value;
  }
  else
  {
    *previous_point = ioopm_entry_create(key, value, NULL);
    ht->ioopm_table_size++; 
  }
}


// Can we replace find previous entry here? 
// How can we do a lookup with an entry as argument?
bool ioopm_hash_table_lookup( ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
    ioopm_entry_t **previous_point = ioopm_find_entry(ht, key); 

    // if the key exists, return the value, otherwise, indicate that the lookup failed
    if (*previous_point != NULL)
    {
        *result = (*previous_point)->value;
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



