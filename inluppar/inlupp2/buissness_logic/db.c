#include "db.h"
#include <stdlib.h>
#include "hash_table_i2.h"
#include "hash_table_iterator_i2.h"
#include "linked_list_i2.h"
#include "linked_list_iterator_i2.h"
#include "common_i2.h"
#include <string.h>

#pragma region create

ioopm_data_base_t *ioopm_db_create(ioopm_hash_function hf_s, ioopm_eq_function eq_s, ioopm_hash_function hf_u, ioopm_eq_function eq_u)
{
    ioopm_data_base_t *db = calloc(1, sizeof(ioopm_data_base_t));
    db->ht_n = ioopm_hash_table_create(hf_s, eq_s);
    db->ht_s = ioopm_hash_table_create(hf_u, eq_u);
    db->name_id_ht = ioopm_hash_table_create(hf_s, eq_s);  
    db->next_id = 1;  
    return db; 
}

merch_t *ioopm_db_item_create(ioopm_data_base_t *db, char *name, char *desc, int price, ioopm_list_t *shelfs)
{
    merch_t *merch = calloc(1, sizeof(merch_t)); 
    merch->name = name;
    merch->desc = desc;
    merch->price = price; 
    merch->list = shelfs;
    merch->id = db->next_id;
    db->next_id++;  
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

void ioopm_db_item_destroy(merch_t *item)
{
    ioopm_db_shelves_destroy(item->list); 
    ioopm_list_destroy(item->list); 
    free(item->desc);
    free(item->name);
    free(item);
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
    ioopm_hash_table_destroy(db->name_id_ht); 
    free(db); 
}

#pragma endregion

#pragma region add_item

/// @brief Inserts a merch into ht_s and registers each of its shelves in ht_n.
///        Assumes that no merch with the same name exists.
/// @param db the database
/// @param item the merch to insert; the database takes ownership
/// @return void
static void insert_item_to_db(ioopm_data_base_t *db, merch_t *item)
{
    // Insert to name_id_ht
    ioopm_hash_table_insert(db->name_id_ht, string_elem(item->name), int_elem(item->id));
    
    // Insert to ht_s
    ioopm_hash_table_insert(db->ht_s, int_elem(item->id), ptr_elem(item));

    // We dont need to insert to ht_n because we dont have shelves
}

ioopm_db_result_t ioopm_db_add_item(ioopm_data_base_t *db, char *name, char *desc, int price)
{
    elem_t result; 
    if (ioopm_hash_table_lookup(db->name_id_ht, string_elem(name), &result))
    {
        return DB_NAME_TAKEN;
    } 

    if (price < 0)
    {
        return DB_INVALID_AMOUNT;
    }
    
    merch_t *item = ioopm_db_item_create(db, name, desc, price, ioopm_list_create());

    insert_item_to_db(db, item);

    return DB_OK; 
}

#pragma endregion 

#pragma region show_stock
ioopm_db_result_t ioopm_db_item_stock(ioopm_data_base_t *db, char *name, ioopm_shelf_t ***shelves, size_t *size, size_t *total)
{
    // set all inputs to 0 or null
    *shelves = NULL; 
    *size = 0; 
    *total = 0;

    elem_t result; 
    if (!ioopm_hash_table_lookup(db->name_id_ht, string_elem(name), &result))
    {
        return DB_NO_SUCH_MERCH; 
    }

    if (!ioopm_hash_table_lookup(db->ht_s, result, &result))
    {
        return DB_ERROR; 
    }
    
    merch_t *merch = result.p; 

    size_t res = ioopm_list_size(merch->list); 
    if (res == 0)
    {
        return DB_OK; 
    }
    
    ioopm_shelf_t **arr = calloc(res, sizeof (ioopm_shelf_t *)); 
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(merch->list); 
    while (!ioopm_list_iterator_at_end(it))
    {
        ioopm_shelf_t *current =  ioopm_list_iterator_current(it).p;
        *total += current->amount; 
        arr[*size] = current; 
        (*size)++; 
        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it);

    *shelves = arr; 

    return DB_OK;
}

#pragma endregion

#pragma region remove_item

ioopm_db_result_t ioopm_db_remove_item(ioopm_data_base_t *db, char *name)
{
    elem_t result_id; 
    
    if (!ioopm_hash_table_lookup(db->name_id_ht, string_elem(name), &result_id))
    {
        return DB_NO_SUCH_MERCH; 
    }

    elem_t result_ptr; 
    if (!ioopm_hash_table_lookup(db->ht_s, result_id, &result_ptr))
    {
        return DB_ERROR; 
    }
    
    merch_t *merch = result_ptr.p; 
    
    // Lookup ht_n for all shelves with the name in it
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(merch->list); 
    elem_t removed; // Garbage, only used to fit function remove
    while (!ioopm_list_iterator_at_end(it))
    {
        ioopm_shelf_t *current = ioopm_list_iterator_current(it).p; 
        ioopm_hash_table_remove(db->ht_n, current->shelf, &removed);    
        ioopm_list_iterator_advance(it);
    }
    ioopm_hash_table_remove(db->name_id_ht, string_elem(name), &removed);    
    ioopm_list_iterator_destroy(it); 

    // Remove merch from ht_s
    ioopm_hash_table_remove(db->ht_s, int_elem(merch->id), &removed);
    ioopm_db_item_destroy(merch); 

    return DB_OK;
}

#pragma endregion

#pragma region edit_merch

ioopm_db_result_t ioopm_db_edit_merch(ioopm_data_base_t *db, char *name_old, char *name_new, char *desc, int price)
{
    if (price < 0)
    {
        return DB_INVALID_AMOUNT; 
    }

    elem_t id;
    
    if (strcmp(name_old, name_new) != 0)
    {
        if (ioopm_hash_table_lookup(db->name_id_ht, string_elem(name_new), &id)) 
        {
            return DB_NAME_TAKEN;
        }
    }
    
    if (!ioopm_hash_table_lookup(db->name_id_ht, string_elem(name_old), &id))
    {
        return DB_NO_SUCH_MERCH; 
    }

    // Find merch for the id
    ioopm_hash_table_lookup(db->ht_s, id, &id); 

    merch_t *merch = id.p; 

    if (strcmp(merch->name, name_new) != 0) 
    {
        elem_t removed; 
        ioopm_hash_table_remove(db->name_id_ht, string_elem(merch->name), &removed);

        free(merch->name);
        free(merch->desc); 
        merch->name = name_new;
        ioopm_hash_table_insert(db->name_id_ht, string_elem(merch->name), int_elem(merch->id)); 
    } else
    {
        free(name_new); 
        free(merch->desc); 

    }
    
    merch->price = price; 
    merch->desc = desc; 

    return DB_OK;
}

#pragma endregion

#pragma region replenish

ioopm_db_result_t ioopm_db_replenish(ioopm_data_base_t *db, char *name, char *shelf, int amount)
{
    if (amount < 1)
    {
        return DB_INVALID_AMOUNT; 
    }

    elem_t result; 
    if (!ioopm_hash_table_lookup(db->name_id_ht, string_elem(name), &result))
    {
        return DB_NO_SUCH_MERCH; 
    }

    elem_t result_ptr; 
    if (!ioopm_hash_table_lookup(db->ht_s, result, &result_ptr))
    {
        return DB_ERROR; 
    }
    
    merch_t *merch = result_ptr.p; 

    // If shelf is not in ht_n - create shelf and append to list
    elem_t garbage; 
    if (!ioopm_hash_table_lookup(db->ht_n, string_elem(shelf), &garbage))
    {

        ioopm_shelf_t *slf = ioopm_db_shelf_create(string_elem(strdup(shelf)), amount);
        ioopm_list_append(merch->list, ptr_elem(slf)); 
        ioopm_hash_table_insert(db->ht_n, slf->shelf, int_elem(merch->id));
        return DB_OK; 
    
    } 
    
    if (garbage.i != merch->id)
    {
        return DB_SHELF_TAKEN; 
    }
    
    
     // find shelf and update value
    
    ioopm_shelf_t *current = NULL; 
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(merch->list);
    while (!ioopm_list_iterator_at_end(it))
    {
        current = ioopm_list_iterator_current(it).p;
        if (strcmp(current->shelf.s, shelf) == 0)
        {
            current->amount += amount; 
            break; // Cut loop short if element is found 
        }
        ioopm_list_iterator_advance(it); 
    }
    ioopm_list_iterator_destroy(it); 

    return DB_OK;
}

#pragma endregion

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
        merch_t *current = ioopm_hash_table_iterator_current_value(it).p;
        names[i] = current->name; 
        ioopm_hash_table_iterator_advance(it);
        i++; 
    }
    ioopm_hash_table_iterator_destroy(it);
    
    return names; 
} 



