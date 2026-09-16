#include "hash_table.h"
#include "hash_table_private.h"
#include "hash_table_iterator.h"
#include <stdio.h>

ioopm_hash_table_iterator_t *ioopm_hash_table_iterator_create(ioopm_hash_table_t *ht)
{
  (void) ht;
  return NULL;
}

void ioopm_hash_table_iterator_destroy(ioopm_hash_table_iterator_t *it)
{
    (void) it;
    return;
}

bool ioopm_hash_table_iterator_at_end(ioopm_hash_table_iterator_t *it)
{
    (void) it;
    return true;
}


void ioopm_hash_table_iterator_advance(ioopm_hash_table_iterator_t *it)
{
    (void) it;
    return;
}


char *ioopm_hash_table_iterator_current_key(ioopm_hash_table_iterator_t *it)
{
    (void) it;
    return "test";
}

int ioopm_hash_table_iterator_current_value(ioopm_hash_table_iterator_t *it)
{
    (void) it;
    return -1;
}