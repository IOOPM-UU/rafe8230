#include "db.h"
#include "linked_list_iterator.h"
#include "hash_table_iterator_i2.h"
#include "hash_table_i2.h"


/*
Functions i have created but dont want to delete
Im to emotionally invested in these :(

*/


bool ioopm_db_any_shelf_occupied(ioopm_data_base_t *db, merch_t *item)
{
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(item->list); 
    while (!ioopm_list_iterator_at_end(it))
    {
        ioopm_shelf_t *current = ioopm_list_iterator_current(it).p;
        elem_t result = int_elem(0); 
        if (ioopm_hash_table_lookup(db->ht_n, string_elem(current->shelf.s), &result))
        {
            ioopm_list_iterator_destroy(it);
            return true; 
        }
        
        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it); 
    return false; 
}

ioopm_list_t *ioopm_db_item_all_shelves(merch_t *item)
{
    ioopm_list_t *names = ioopm_list_create();
    
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(item->list);
    while (!ioopm_list_iterator_at_end(it))
    {
        ioopm_shelf_t *s = ioopm_list_iterator_current(it).p;
        ioopm_list_append(names, string_elem(s->shelf.s));
        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it);
    
    return names;
}

char **ioopm_db_merch_names(ioopm_data_base_t *db, size_t *size)
{
    *size = ioopm_hash_table_size(db->ht_s); 
    if (*size == 0)
    {
        return NULL; 
    }
    
    char **names = calloc(*size, sizeof(char *)); 
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(db->ht_s);    
    size_t i = 0; 
    while (!ioopm_hash_table_iterator_at_end(it))
    {
        names[i] = ioopm_hash_table_iterator_current_key(it).s;
        ioopm_hash_table_iterator_advance(it);
        i++; 
    }
    ioopm_hash_table_iterator_destroy(it);
    
    return names; 
} 