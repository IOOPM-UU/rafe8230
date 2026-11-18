#include "hash_table.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "hash_table_private.h"
#include "common.h"
#include "hash_table_iterator.h"
// Severity 1: including stdio is not needed

// Severity 2: No checks to see if table is NULL, added asserts where needed

// Severity1: Overall formatting, different indentation, "{" not following the same style
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *h_fn, ioopm_eq_function *eq_fn)
{
    // Severity 1: redundant zeroing of fields because of calloc
    ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t)); // calloc zero-initializes the struct and buckets
    ht->hash_fn = h_fn;
    ht->hash_eq_fn = eq_fn;
    ht->no_buckets = No_Buckets;
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
        ioopm_entry_t *entry = ht->buckets[i].next; // skip the dummy
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

// Severity 1: Removed comment about optimization
static ioopm_entry_t *find_previous_entry(ioopm_hash_table_t *ht, elem_t key)
{
    // find bucket
    size_t bucket = ht->hash_fn(key) % ht->no_buckets;

    ioopm_entry_t *previous = &ht->buckets[bucket];
    ioopm_entry_t *current = previous->next;

    // look for an entry with the key we want then terminate the loop and return prev
    while (current != NULL && !ht->hash_eq_fn(current->key, key)) // Severity 1: double negation was confusing for no reason
    {
        previous = current;
        current = current->next;
    }
    return previous;
}

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
    assert(ht != NULL && result != NULL);

    ioopm_entry_t *previous = find_previous_entry(ht, key);
    ioopm_entry_t *current = previous->next;

    if (current == NULL)
    {
        // Severity 1: Magic number, now ELEM_ERROR defined in common.h
        *result = ELEM_ERROR;
        return false;
    }

    *result = current->value;
    previous->next = entry_destroy(current);
    ht->size--;
    return true;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
    assert(ht != NULL);

    // find previous entry, or the last entry if the key does not exist
    ioopm_entry_t *previous = find_previous_entry(ht, key);

    // if the key exists, update the value, otherwise create a new entry
    if (previous->next != NULL)
    {
        previous->next->value = value;
    }
    else
    {
        previous->next = ioopm_entry_create(key, value, NULL);
        ht->size++;
    }
}

// Severity 1: Removed internal comments about code changes
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
    assert(ht != NULL && result != NULL);

    ioopm_entry_t *previous = find_previous_entry(ht, key);
    ioopm_entry_t *current = previous->next;

    // if the key exists, return the value, otherwise, indicate that the lookup failed
    if (current != NULL)
    {
        *result = current->value;
        return true;
    }
    else
    {
        // Severity 1: Magic number, now ELEM_ERROR defined in common.h
        *result = ELEM_ERROR;
        return false;
    }
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
    // Severity 1: Magic number, now ELEM_ERROR defined in common.h
    elem_t result = ELEM_ERROR;
    return ioopm_hash_table_lookup(ht, key, &result);
}

size_t ioopm_hash_table_size(const ioopm_hash_table_t *ht)
{
    return ht->size;
}

bool ioopm_hash_table_is_empty(const ioopm_hash_table_t *ht)
{
    return ht->size == 0;
}