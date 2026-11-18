#include "hash_table.h"
#include "hash_table_private.h"
#include "hash_table_iterator.h"
#include <stdlib.h>
#include <assert.h>
#include "common.h"
//Severity 1: including stdio is not needed

struct hash_table_iterator
{
  ioopm_hash_table_t *ht;
  size_t current_bucket;
  ioopm_entry_t *current_entry;
};

static void advance_iterator_state(ioopm_hash_table_iterator_t *it)
{
    // advance to the next entry in the bucket
    it->current_entry = it->current_entry->next;

    // if it was null advance to the next bucket
    if (it->current_entry == NULL)
    {
        it->current_bucket += 1;

        if (it->current_bucket < it->ht->no_buckets)
        {
            it->current_entry = &it->ht->buckets[it->current_bucket];
        }
    }
}

static void skip_sentinel_nodes(ioopm_hash_table_iterator_t *it)
{
    while (it->current_bucket < it->ht->no_buckets &&
           it->current_entry == &it->ht->buckets[it->current_bucket])
    {
        advance_iterator_state(it);
    }
}

ioopm_hash_table_iterator_t *ioopm_hash_table_iterator_create(ioopm_hash_table_t *ht)
{
    //Severity 1: redundant zeroing of fields because of calloc
    ioopm_hash_table_iterator_t *it = calloc(1, sizeof(ioopm_hash_table_iterator_t));
    it->ht = ht;
    it->current_entry = &ht->buckets[0];
    skip_sentinel_nodes(it);
    return it;
}

void ioopm_hash_table_iterator_destroy(ioopm_hash_table_iterator_t *it)
{
    free(it);
}

bool ioopm_hash_table_iterator_at_end(const ioopm_hash_table_iterator_t *it)
{
    return it->current_bucket == it->ht->no_buckets;
}

//Severity 3: Iterator deferences NULL if you go past end of list, use assert
void ioopm_hash_table_iterator_advance(ioopm_hash_table_iterator_t *it)
{
    assert(it != NULL && !ioopm_hash_table_iterator_at_end(it));
    advance_iterator_state(it);
    skip_sentinel_nodes(it);
}

//Severity 3: Iterator deferences NULL if you go past end of list, use assert
elem_t ioopm_hash_table_iterator_current_key(const ioopm_hash_table_iterator_t *it)
{   
    assert(it != NULL && !ioopm_hash_table_iterator_at_end(it));
    return it->current_entry->key;
}

//Severity 3: Iterator deferences NULL if you go past end of list, use assert
elem_t ioopm_hash_table_iterator_current_value(const ioopm_hash_table_iterator_t *it)
{
    assert(it != NULL && !ioopm_hash_table_iterator_at_end(it));
    return it->current_entry->value;
}
