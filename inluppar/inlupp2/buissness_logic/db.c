#include "db.h"
#include <stdlib.h>
#include "hash_table_i2.h"
#include "hash_table_iterator_i2.h"
#include "linked_list_i2.h"
#include "linked_list_iterator_i2.h"
#include "common_i2.h"
#include <string.h>


struct merch
{
    int id;
    char *name;
    char *desc;
    int price;
    ioopm_list_t *list;
};

struct database
{
    ioopm_hash_table_t *ht_n;       // shelf name -> merch id
    ioopm_hash_table_t *ht_s;       // merch id -> merch_t *
    ioopm_hash_table_t *name_id_ht; // merch name -> merch id
    int next_id;
    ioopm_hash_table_t *carts;     // cart id -> cart table
    size_t next_cart_id;
};

struct shelf
{
    size_t amount;
    elem_t shelf;
};


#pragma region helpers

char *ioopm_db_get_shelf(ioopm_shelf_t *shlf)
{
    return shlf->shelf.s;
}

size_t ioopm_db_get_shelf_amount(ioopm_shelf_t *shlf)
{
    return shlf->amount;
}

size_t ioopm_db_get__shelf_amount(ioopm_shelf_t *shlf)
{
    return ioopm_db_get_shelf_amount(shlf);
}

ioopm_hash_table_t *ioopm_get_carts(ioopm_data_base_t *db)
{
    return db->carts;
}

ioopm_hash_table_t *ioopm_get_name_id_ht(ioopm_data_base_t *db)
{
    return db->name_id_ht;
}

ioopm_hash_table_t *ioopm_get_ht_n(ioopm_data_base_t *db)
{
    return db->ht_n;
}

ioopm_hash_table_t *ioopm_get_ht_s(ioopm_data_base_t *db)
{
    return db->ht_s;
}

char *ioopm_get_merch_name(merch_t *m)
{
    return m->name;
}

char *ioopm_get_merch_desc(merch_t *m)
{
    return m->desc;
}

int ioopm_get_merch_id(merch_t *m)
{
    return m->id;
}

ioopm_list_t *ioopm_get_merch_list(merch_t *m)
{
    return m->list;
}

int ioopm_get_merch_price(merch_t *m)
{
    return m->price;
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
        merch_t *current = ioopm_hash_table_iterator_current_value(it).p;
        names[i] = current->name; 
        ioopm_hash_table_iterator_advance(it);
        i++; 
    }
    ioopm_hash_table_iterator_destroy(it);
    
    return names; 
} 

static int ioopm_db_merch_price(merch_t *m)
{
    return m->price; 
}

/// @brief find the key string the cart stores. 
/// @param cart the cart hashtable
/// @param name the key to look for
/// @return the found key, NULL if not found. 
static char *cart_stored_key(ioopm_hash_table_t *cart, char *name)
{
    char *found = NULL; 
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart);
    while (!ioopm_hash_table_iterator_at_end(it))
    {
        char *key = ioopm_hash_table_iterator_current_key(it).s;
        if (strcmp(key, name) == 0)
        {
            found = key;
            break; 
        }
        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);
    return found;
}

/// @brief replace and rehash a new name to a database. This replaces
///        the old name, making the new name have the same values 
///        as the old one. 
/// @param db the database
/// @param name_old the old name
/// @param name_new the new name
static void carts_update_merch(ioopm_data_base_t *db, char *name_old, char *name_new)
{
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(db->carts);
    while (!ioopm_hash_table_iterator_at_end(it))
    {
        ioopm_hash_table_t *cart = ioopm_hash_table_iterator_current_value(it).p;

        char *key = cart_stored_key(cart, name_old);   
        if (key != NULL)
        {
            elem_t quantity;
            ioopm_hash_table_remove(cart, string_elem(name_old), &quantity);
            free(key);                                

            if (name_new != NULL)
            {
                ioopm_hash_table_insert(cart, string_elem(strdup(name_new)), quantity);  
            }
        }
        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);
}


#pragma endregion

#pragma region backend



#pragma region create

ioopm_data_base_t *ioopm_db_create(ioopm_hash_function hf_s, ioopm_eq_function eq_s, ioopm_hash_function hf_u, ioopm_eq_function eq_u)
{
    ioopm_data_base_t *db = calloc(1, sizeof(ioopm_data_base_t));
    db->ht_n = ioopm_hash_table_create(hf_s, eq_s);
    db->ht_s = ioopm_hash_table_create(hf_u, eq_u);
    db->name_id_ht = ioopm_hash_table_create(hf_s, eq_s);  
    db->next_id = 1;  
    db->carts = ioopm_hash_table_create(hf_u, eq_u); 
    db->next_cart_id = 1; 
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

static void ioopm_db_cart_destroy(ioopm_hash_table_t *ht)
{
    ioopm_hash_table_iterator_t *it_h = ioopm_hash_table_iterator_create(ht);
    while (!ioopm_hash_table_iterator_at_end(it_h))
    {
        char *current = ioopm_hash_table_iterator_current_key(it_h).s;
        ioopm_hash_table_iterator_advance(it_h);
        free(current); 
    }
    ioopm_hash_table_iterator_destroy(it_h);
    ioopm_hash_table_destroy(ht);
}

void ioopm_db_destroy(ioopm_data_base_t *db)
{
    // Destroy ht_s
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(db->ht_s);
    while (!ioopm_hash_table_iterator_at_end(it))
    {
        merch_t *merch = ioopm_hash_table_iterator_current_value(it).p;
        ioopm_hash_table_iterator_advance(it); 
        ioopm_db_item_destroy(merch); // Destroy current merch
    }
    ioopm_hash_table_iterator_destroy(it); 
    // Free empty ht_s
    ioopm_hash_table_destroy(db->ht_s);
    
    // Free filled ht_s
    ioopm_hash_table_destroy(db->ht_n);

    // Free filled name_id_ht
    ioopm_hash_table_destroy(db->name_id_ht); 

    // Destroy carts hashtable
    ioopm_hash_table_iterator_t *it_h = ioopm_hash_table_iterator_create(db->carts);
    while (!ioopm_hash_table_iterator_at_end(it_h))
    {
        ioopm_hash_table_t *current = ioopm_hash_table_iterator_current_value(it_h).p;        
        ioopm_hash_table_iterator_advance(it_h);
        ioopm_db_cart_destroy(current); 
        // ioopm_hash_table_destroy(current);
    }
    ioopm_hash_table_iterator_destroy(it_h);
    ioopm_hash_table_destroy(db->carts); 

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
    
    carts_update_merch(db, name, NULL); 

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
        carts_update_merch(db, name_old, name_new);
        
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
            ioopm_list_iterator_destroy(it); 
            return DB_OK;
        }
        ioopm_list_iterator_advance(it); 
    }
    ioopm_list_iterator_destroy(it); 

    return DB_ERROR; 
}

#pragma endregion



#pragma endregion



#pragma region carts


ioopm_db_result_t ioopm_cart_create(ioopm_data_base_t *db, ioopm_hash_function s_hash, ioopm_eq_function s_eq)
{
    ioopm_hash_table_t *cart = ioopm_hash_table_create(s_hash, s_eq); 
    if (cart == NULL) 
    {
        ioopm_hash_table_destroy(cart); 
        return CART_CREATION_FAILED;
    } 
        
    
    ioopm_hash_table_insert(db->carts, int_elem(db->next_cart_id), ptr_elem(cart));
    db->next_cart_id++; 
    return DB_OK; 
}

ioopm_db_result_t ioopm_cart_destroy(ioopm_data_base_t *db, size_t cart)
{
    elem_t ptr;
    if (!ioopm_hash_table_lookup(db->carts, int_elem(cart), &ptr))
    {
        return CART_DOES_NOT_EXSIST;
    }

    elem_t removed;
    ioopm_hash_table_remove(db->carts, int_elem(cart), &removed);   
    ioopm_db_cart_destroy(ptr.p);                                   
    return DB_OK;
}
 


static size_t accumulate_cart_stock(ioopm_data_base_t *db, char *merch)
{
    size_t result = 0; 
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(db->carts); 
    elem_t add;
    while (!ioopm_hash_table_iterator_at_end(it))
    {
        ioopm_hash_table_t *ht = ioopm_hash_table_iterator_current_value(it).p; 
        
        if (ioopm_hash_table_lookup(ht, string_elem(merch), &add))
        {
            result += add.u; 
        }
         


        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it); 
    return result; 
}

ioopm_db_result_t ioopm_cart_increase_quantity(ioopm_data_base_t *db, size_t cart, char *merch_name, size_t amount)
{
    // Checks
    if (ioopm_hash_table_size(db->name_id_ht) == 0) return DB_EMPTY; 
    if (amount < 1) return DB_INVALID_AMOUNT;
    
    elem_t ptr; 
    if (!ioopm_hash_table_lookup(db->carts, int_elem(cart), &ptr)) return CART_DOES_NOT_EXSIST; 
    
    // Garbage values to fit stock arguments
    ioopm_shelf_t **garbage_shelf = NULL; size_t garbage_size = 0; 

    size_t total = 0; 
    ioopm_db_result_t res = ioopm_db_item_stock(db, 
                                                merch_name, 
                                                &garbage_shelf, 
                                                &garbage_size, 
                                                &total);   
    free(garbage_shelf); 
    if (res != DB_OK) return res; 
 
    size_t reserved = accumulate_cart_stock(db, merch_name); 
    

    if (total - reserved < amount)
    {
        return CART_NOT_ENOUGH_STOCK; 
    }
    ioopm_hash_table_t *current_ht = ptr.p; 
    
    elem_t cart_stock; 
    bool exsists = ioopm_hash_table_lookup(current_ht, string_elem(merch_name), &cart_stock);
    if (!exsists)
    {
        cart_stock.u = 0; 
    } 
    
    cart_stock.u += amount; 
    
    ioopm_hash_table_insert(current_ht, string_elem(merch_name), cart_stock);

    return exsists ? DB_OK_NO_MERCH_ADDED : DB_OK;
}

ioopm_db_result_t ioopm_cart_decrease_quantity(ioopm_data_base_t *db, size_t cart, char *merch_name, size_t amount)
{
    elem_t ptr; 
    if (!ioopm_hash_table_lookup(db->carts, int_elem(cart), &ptr)) 
    {
        return CART_DOES_NOT_EXSIST; 
    }

    elem_t quantity; 
    if (!ioopm_hash_table_lookup((ioopm_hash_table_t *) ptr.p, string_elem(merch_name), &quantity))
    {
        return DB_NO_SUCH_MERCH; 
    }
    
    if (quantity.u < amount)
    {
        return DB_INVALID_AMOUNT; 
    }

    quantity.i -= amount; 
    if (quantity.i == 0)
    {
        char *key = cart_stored_key(ptr.p, merch_name); 
        ioopm_hash_table_remove(ptr.p, string_elem(merch_name), &quantity);
        free(key); 
        return DB_OK;
    }
    
    ioopm_hash_table_insert((ioopm_hash_table_t *)ptr.p, string_elem(merch_name), quantity);

    return DB_OK;
} 

ioopm_db_result_t ioopm_cart_calculate_cost(ioopm_data_base_t *db, size_t cart, size_t *cost)
{
    *cost = 0; 
    elem_t ptr;
    if (!ioopm_hash_table_lookup(db->carts, int_elem(cart), &ptr))
    {
        return CART_DOES_NOT_EXSIST; 
    }
    
    ioopm_hash_table_t *crt = ptr.p; 
    if (ioopm_hash_table_size(crt) == 0)
    {
        return CART_EMPTY; 
    }

    
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(crt);

    while (!ioopm_hash_table_iterator_at_end(it))
    {
        size_t current_quantity = ioopm_hash_table_iterator_current_value(it).u;
        elem_t current_name = ioopm_hash_table_iterator_current_key(it);
        
        elem_t id;
        if (!ioopm_hash_table_lookup(db->name_id_ht, current_name, &id))
        {
            ioopm_hash_table_iterator_destroy(it); 
            return DB_ERROR; 
        }
        elem_t merch_ptr;
        ioopm_hash_table_lookup(db->ht_s, id, &merch_ptr);
        merch_t *m = merch_ptr.p; 
        int price = ioopm_db_merch_price(m); 

        *cost += price * current_quantity; 
        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it); 
    return DB_OK;
}


// Invariant: there is always enough stock to depleat
static ioopm_db_result_t ioopm_db_depleat(ioopm_data_base_t *db, char *merch, size_t amnt)
{
    elem_t id;
    if (!ioopm_hash_table_lookup(db->name_id_ht, string_elem(merch), &id))
    {
        return DB_NO_SUCH_MERCH;
    }
    
    elem_t ptr; 
    if (!ioopm_hash_table_lookup(db->ht_s, id, &ptr))
    {
        return DB_ERROR; // should not be possible considering previous ht lookup
    }
    
    merch_t *m = ptr.p; 

    size_t left = amnt; 
    ioopm_list_iterator_t *l = ioopm_list_iterator_create(m->list); 
    while (!ioopm_list_iterator_at_end(l) && left > 0)
    {
        ioopm_shelf_t *s = ioopm_list_iterator_current(l).p;

        size_t take = s->amount > left ? left : s->amount; 

        s->amount -= take; 

        left -= take; 

        if (s->amount == 0)
        {
            elem_t res;
            ioopm_hash_table_remove(db->ht_n, s->shelf, &res);  
            ioopm_list_iterator_remove(l);
            ioopm_db_shelf_destroy(s); 
        } else
        {
            ioopm_list_iterator_advance(l);
        }
    }
    ioopm_list_iterator_destroy(l); 
    
    return DB_OK; 
}

ioopm_db_result_t ioopm_cart_checkout(ioopm_data_base_t *db, size_t cart)
{
    elem_t ptr;
    if (!ioopm_hash_table_lookup(db->carts, int_elem(cart), &ptr))
    {
        return CART_DOES_NOT_EXSIST; 
    }

    if (ioopm_hash_table_size(ptr.p) == 0)
    {
        return CART_EMPTY; 
    }

    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ptr.p); 
    while (!ioopm_hash_table_iterator_at_end(it))
    {
        elem_t merch = ioopm_hash_table_iterator_current_key(it);
        elem_t amount = ioopm_hash_table_iterator_current_value(it);

        ioopm_db_result_t res = ioopm_db_depleat(db, merch.s, amount.i);
        if (res != DB_OK)
        {
            ioopm_hash_table_iterator_destroy(it); 
            return res; 
        }

        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);

    elem_t result; 
    ioopm_hash_table_remove(db->carts, int_elem(cart), &result); 

    ioopm_db_cart_destroy(ptr.p);
    return DB_OK; 
}



#pragma endregion


