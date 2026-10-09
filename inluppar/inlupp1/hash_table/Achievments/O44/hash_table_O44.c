#include <stdio.h>
#include "hash_table_O44.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table_private.h"
#include "common.h"
#include "hash_table_iterator.h"



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
    ht->buckets = calloc(No_Buckets, sizeof(ioopm_entry_t)); 
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
        ioopm_entry_t *entry = ht->buckets[i].next;  // skip the dummy
        while (entry)
        {
            ioopm_entry_t *next = entry->next;
            free(entry);  // use your entry_destroy here if you have one
            entry = next;
        }
    }
    free(ht->buckets);
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
    free_buckets(ht);
    free(ht);
}


// Optimization idea - compair pointer values instead of keys, faster comparison aka, take entry as argument not elem_t
ioopm_entry_t *ioopm_find_previous_entry(ioopm_hash_table_t *ht, elem_t key)
{
    // find bucket
    size_t bucket = ht->hash_fn(key) % ht->no_buckets;
    
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

static size_t prime_at(size_t index)
{
    static const size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191,
    16381, 32771, 65537, 131071, 262147, 524287, 1048573};
    size_t no_primes = sizeof(primes) / sizeof(primes[0]);
    return index < no_primes ? primes[index] : 0;
}

static void ht_realloc(ioopm_hash_table_t *ht, size_t new_prime_index)
{
    // Create a copy of the ht in the stack
    ioopm_hash_table_t ht_copy = *ht;
    
    // Update the real ht to more buckets
    // We set the ht to null when using calloc
    ht->prime_index = new_prime_index; 
    ht->no_buckets = prime_at(ht->prime_index); 
    ht->buckets = calloc(ht->no_buckets, sizeof(ioopm_entry_t));
    ht->ioopm_table_size = 0;
    
    
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(&ht_copy);  
    while (!ioopm_hash_table_iterator_at_end(it))
    {
        ioopm_hash_table_insert(ht, it->current_entry->key, it->current_entry->value);   
        ioopm_hash_table_iterator_advance(it); 
    }
    ioopm_hash_table_iterator_destroy(it);
    
    free_buckets(&ht_copy);    
}


static void ht_grow(ioopm_hash_table_t *ht)
{
    if (prime_at(ht->prime_index + 1) != 0) ht_realloc(ht, ht->prime_index + 1); 
}

static void ht_shrink(ioopm_hash_table_t *ht)
{
    if (ht->prime_index > 0) ht_realloc(ht, ht->prime_index - 1);
}

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
    ioopm_entry_t *previous = ioopm_find_previous_entry(ht, key);
    ioopm_entry_t *current = previous->next;
    
    // If optimization found due to coverage showing if inside the while loop
    // being 0%
    if (current == NULL)
    {
        *result = int_elem(-1); 
        return false;
    }
    
    // the bucket is empty or key is not in bucket
    
    *result = current->value; 
    previous->next = entry_destroy(current); 
    ht->ioopm_table_size--; 
    
    
    if (ht->ioopm_table_size < prime_at(ht->prime_index) * 0.3)
    {
        ht_shrink(ht); 
    }
    
    
    return true;
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
    
    if (ht->ioopm_table_size > prime_at(ht->prime_index) * 0.7)
    {
        ht_grow(ht);
    }
}


// Can we replace find previous entry here? 
// How can we do a lookup with an entry as argument?
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




