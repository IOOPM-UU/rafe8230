#include <stdio.h>
#include "hash_table.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define No_Buckets 17

static size_t string_knr_hash(const char *str)
{
  size_t result = 0;
  while (*str != '\0')
  {
    result = result * 31 + ((unsigned char) *str);
    str++;
  }
  return result;
}

ioopm_hash_table_t *ioopm_hash_table_create(void) {
    // NOTE: Calloc initializes all bits to 0.
    // We therefore do not have to create a loop
    // setting all the buckets to NULL
    return calloc(1, sizeof(ioopm_hash_table_t));
}

static ioopm_entry_t *ioopm_entry_create(char *key, int value, ioopm_entry_t *next)
{
    ioopm_entry_t *entry = calloc(1, sizeof(ioopm_entry_t));
    entry->key = key;
    entry->value = value;
    entry->next = next;

    return entry; 
}

static ioopm_entry_t *entry_destroy(ioopm_entry_t *entry)
{
  ioopm_entry_t *next = entry->next;
  free(entry);
  return next;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
    
    for (int i = 0; i < No_Buckets; i++)
    {
        ioopm_entry_t *current = ht->buckets[i].next;
        while (current != NULL)
        {
            current = entry_destroy(current);
        }
    }
    
    free(ht);
    return;
}

ioopm_entry_t *ioopm_find_previous_entry(ioopm_hash_table_t *ht, char *key)
{
    // find bucket
    size_t bucket = string_knr_hash(key) % No_Buckets;
    
    // look for an entry with the key we want
    ioopm_entry_t *previous = &ht->buckets[bucket];
    ioopm_entry_t *current = previous->next;
    while (current != NULL && strcmp(current->key, key) != 0)
    {
        previous = current;
        current = current->next;
    }
    return previous; 
}

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, char *key, int *result)
{
    ioopm_entry_t *previous = ioopm_find_previous_entry(ht, key);
    ioopm_entry_t *current = previous->next;

    
    while (current != NULL)
    {

        if (strcmp(key, current->key) == 0)
        {

            // case : no middle element and is last elementx
            *result = current->value;
            previous->next = entry_destroy(current);
            return true;
        } else 
        {
            previous = current; 
            current = current->next; 
        }
    }
    
    // the bucket is empty or key is not in bucket
    *result = 0; 
    return false;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, char *key, int value)
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
  }
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, char *key, int *result)
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
        *result = 0; 
        return false;
    }
}
