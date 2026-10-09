#include "db.h"
#include <stdlib.h>
#include "hash_table_i2.h"
#include "hash_table_iterator_i2.h"
#include "linked_list_i2.h"
#include "linked_list_iterator_i2.h"
#include "common_i2.h"
#include <string.h>

#pragma region create

ioopm_data_base_t *ioopm_db_create(ioopm_hash_function hf, ioopm_eq_function eq)
{
    ioopm_data_base_t *db = calloc(1, sizeof(ioopm_data_base_t));
    db->ht_n = ioopm_hash_table_create(hf, eq);
    db->ht_s = ioopm_hash_table_create(hf, eq); 
    return db; 
}

merch_t *ioopm_db_item_create(char *name, char *desc, int price, ioopm_list_t *shelfs)
{
    merch_t *merch = calloc(1, sizeof(merch_t)); 
    merch->name = name;
    merch->desc = desc;
    merch->price = price; 
    merch->list = shelfs; 
    return merch; 
}

ioopm_shelf_t *ioopm_db_shelf_create(elem_t slf, size_t amount)
{
    ioopm_shelf_t *shelf = calloc(1, sizeof(ioopm_shelf_t));
    shelf->shelf = slf;
    shelf->amount = amount; 
    return shelf;
}

#pragma endregion

#pragma region destroy

void ioopm_db_shelf_destroy(ioopm_shelf_t *slf)
{
    free(slf->shelf.s); 
    free(slf);
}

void ioopm_db_item_destroy(merch_t *item)
{
    ioopm_db_shelves_destroy(item->list); 
    ioopm_list_destroy(item->list); 
    free(item->desc);
    free(item->name);
    free(item);
}

void ioopm_db_shelves_destroy(ioopm_list_t *lst)
{
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(lst); 
    while (!ioopm_list_iterator_at_end(it))
    {
        ioopm_shelf_t *current = ioopm_list_iterator_current(it).p;
        ioopm_list_iterator_advance(it); 
        ioopm_db_shelf_destroy(current);
    }
    ioopm_list_iterator_destroy(it); 
}

void ioopm_db_destroy(ioopm_data_base_t *db)
{
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(db->ht_s);
    while (!ioopm_hash_table_iterator_at_end(it))
    {
        merch_t *merch = ioopm_hash_table_iterator_current_value(it).p;
        ioopm_hash_table_iterator_advance(it); 
        ioopm_db_item_destroy(merch); 
    }
    ioopm_hash_table_iterator_destroy(it); 

    ioopm_hash_table_destroy(db->ht_n);
    ioopm_hash_table_destroy(db->ht_s);
    free(db); 
}

#pragma endregion

#pragma region add_item

/// @brief Inserts a merch into ht_s and registers each of its shelves in ht_n.
///        Assumes that no merch with the same name exists.
/// @param db the database
/// @param item the merch to insert; the database takes ownership
/// @return true
static bool insert_new_item_to_db(ioopm_data_base_t *db, merch_t *item)
{
    // Insert to ht_s
    ioopm_hash_table_insert(db->ht_s, string_elem(item->name), ptr_elem(item));

    // Insert to ht_n
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(item->list);
    while (!ioopm_list_iterator_at_end(it))
    {
        ioopm_shelf_t *s = ioopm_list_iterator_current(it).p; 
        ioopm_hash_table_insert(db->ht_n, string_elem(s->shelf.s), string_elem(item->name));
        ioopm_list_iterator_advance(it); 
    }   
    ioopm_list_iterator_destroy(it); 
    return true; 
}

ioopm_db_result_t ioopm_db_add_item(ioopm_data_base_t *db, char *name, char *desc, int price)
{
    elem_t result; 
    if (ioopm_hash_table_lookup(db->ht_s, string_elem(name), &result))
    {
        return DB_NAME_TAKEN;
    } 

    if (price < 0)
    {
        return DB_INVALID_AMOUNT;
    }
    
    merch_t *item = ioopm_db_item_create(strdup(name), strdup(desc), price, ioopm_list_create());

    insert_new_item_to_db(db, item);

    return DB_OK; 
}

#pragma endregion 
