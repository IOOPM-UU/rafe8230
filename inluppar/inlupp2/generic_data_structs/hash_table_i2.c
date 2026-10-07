#include "hash_table_i2.h"
#include <stdlib.h>
#include "hash_table_private_i2.h"



ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *h_fn, ioopm_eq_function *eq_fn)
{
    ioopm_hash_table_t *ht = calloc(1, sizeof(*ht));
    ht->hash_fn = h_fn;
    ht->hash_eq_fn = eq_fn;
    ht->no_buckets = No_Buckets;
    ht->buckets = calloc(ht->no_buckets, sizeof(*ht->buckets));
    ht->prime_index = 0; 
    ht->ioopm_table_size = 0; 
    return ht;
}

static ioopm_entry_t *ioopm_entry_create(elem_t key, elem_t value)
{
    ioopm_entry_t *entry = calloc(1, sizeof(*entry));
    entry->key = key;
    entry->value = value;
    return entry;
}

static ioopm_entry_t *entry_destroy(ioopm_entry_t *entry)
{

    ioopm_entry_t *next = entry->next;
    free(entry);
    return next;
}

// Frees all entries and the bucket array, but NOT the struct itself
static void free_buckets(ioopm_hash_table_t *ht)
{
    for (size_t i = 0; i < ht->no_buckets; ++i)
    {
        ioopm_entry_t *entry = ht->buckets[i];
        while (entry != NULL)
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

static ioopm_entry_t **ioopm_find_entry(ioopm_hash_table_t *ht, elem_t key)
{
    size_t bucket = ht->hash_fn(key) % ht->no_buckets;

    ioopm_entry_t **entry = &ht->buckets[bucket];

    while (*entry != NULL && !ht->hash_eq_fn((*entry)->key, key))
    {
        entry = &(*entry)->next;
    }
    return entry;
}

static size_t prime_at(size_t index)
{
    static const size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381};
    size_t no_primes = sizeof(primes) / sizeof(primes[0]);
    return index < no_primes ? primes[index] : 0;
}

static void ht_realloc(ioopm_hash_table_t *ht, size_t new_prime_index)
{
    size_t new_bucket_count = prime_at(new_prime_index);
    ioopm_entry_t **new_buckets = calloc(new_bucket_count, sizeof(*new_buckets));

    ioopm_entry_t **old_buckets = ht->buckets;
    size_t old_bucket_count = ht->no_buckets;
    ht->buckets = new_buckets;
    ht->no_buckets = new_bucket_count;
    ht->prime_index = new_prime_index;

    for (size_t i = 0; i < old_bucket_count; ++i)
    {
        ioopm_entry_t *entry = old_buckets[i];
        while (entry != NULL)
        {
            ioopm_entry_t *next = entry->next;
            size_t bucket = ht->hash_fn(entry->key) % ht->no_buckets;
            entry->next = ht->buckets[bucket];
            ht->buckets[bucket] = entry;
            entry = next;
        }
    }
    free(old_buckets);
}


static void ht_grow(ioopm_hash_table_t *ht)
{
    if (prime_at(ht->prime_index + 1) != 0)
    {
        ht_realloc(ht, ht->prime_index + 1);
    }
}

static void ht_shrink(ioopm_hash_table_t *ht)
{
    if (ht->prime_index > 0)
    {
        ht_realloc(ht, ht->prime_index - 1);
    }
}

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
    ioopm_entry_t **entry = ioopm_find_entry(ht, key);
    if (*entry == NULL)
    {
        *result = int_elem(-1); 
        return false;
    }
    
    ioopm_entry_t *removed = *entry;
    *result = removed->value;
    *entry = removed->next;
    free(removed);
    ht->ioopm_table_size--;

    if (ht->prime_index > 0 && ht->ioopm_table_size < ht->no_buckets * 0.3)
    {
        ht_shrink(ht);
    }
    return true; 

}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
    ioopm_entry_t **entry = ioopm_find_entry(ht, key);
    if (*entry != NULL)
    {
            (*entry)->value = value;
    }
    else
    {
        *entry = ioopm_entry_create(key, value);
        if (*entry == NULL)
        {
                return;
        }
        ht->ioopm_table_size++;
    }

    if (ht->ioopm_table_size > ht->no_buckets * 0.7)
    {
        ht_grow(ht);
    }
}


// Can we replace find previous entry here? 
// How can we do a lookup with an entry as argument?
bool ioopm_hash_table_lookup( ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
    ioopm_entry_t **entry = ioopm_find_entry(ht, key);

    // if the key exists, return the value, otherwise, indicate that the lookup failed
    if (*entry != NULL)
    {
        *result = (*entry)->value;
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




