#include "hash_table_M39_O44.h"
#include "hash_table_private_M39_O44.h"
#include "hash_table_iterator_M39_O44.h"
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

bool ioopm_hash_table_iterator_at_end(const ioopm_hash_table_iterator_t *it)
{
    return it->current_entry == NULL;
}

static void advance_iterator_state(ioopm_hash_table_iterator_t *it)
{

  // if it was null advance to the next bucket
  while (it->current_entry == NULL)
  {
    it->current_bucket += 1;

    if (it->current_bucket >= it->ht->no_buckets)
    {
        return; 
    }
    it->current_entry = it->ht->buckets[it->current_bucket];
  }
}

ioopm_hash_table_iterator_t *ioopm_hash_table_iterator_create(ioopm_hash_table_t *ht)
{
    ioopm_hash_table_iterator_t *it = calloc(1, sizeof(ioopm_hash_table_iterator_t)); 
    it->ht = ht;
    it->current_bucket = 0;
    it->current_entry = ht->buckets[0];
    advance_iterator_state(it); 
    return it; 

}

void ioopm_hash_table_iterator_destroy(ioopm_hash_table_iterator_t *it)
{
    free(it);
}



void ioopm_hash_table_iterator_advance(ioopm_hash_table_iterator_t *it)
{
    it->current_entry = it->current_entry->next;  
    advance_iterator_state(it);                        
}


elem_t ioopm_hash_table_iterator_current_key(const ioopm_hash_table_iterator_t *it)
{
    return it->current_entry->key;
}

elem_t ioopm_hash_table_iterator_current_value(const ioopm_hash_table_iterator_t *it)
{
    return it->current_entry->value;
}
