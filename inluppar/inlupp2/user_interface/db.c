#include <stdio.h>
#include "utils_i2.h"
#include "db.h"
#include <stdlib.h>
#include <ctype.h>
#include "hash_table_i2.h"
#include "hash_table_private_i2.h"
#include "hash_table_iterator_i2.h"
#include "linked_list_i2.h"
#include "linked_list_iterator_i2.h"
#include "linked_list_private_i2.h"
#include "common_i2.h"
#include <string.h>
void ioopm_db_shelf_destroy(ioopm_shelf_t *slf)
{
    free(slf->shelf); 
    free(slf);
}

void ioopm_db_item_list_destroy(ioopm_list_t *lst)
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
    ioopm_db_item_list_destroy(item->list); 
    ioopm_list_destroy(item->list); 
    free(item->desc);
    free(item->name);
    free(item);
}

ioopm_data_base_t *ioopm_db_create(ioopm_hash_function hf, ioopm_eq_function eq)
{
    ioopm_data_base_t *db = calloc(1, sizeof(ioopm_data_base_t));
    db->ht_n = ioopm_hash_table_create(hf, eq);
    db->ht_s = ioopm_hash_table_create(hf, eq); 
    return db; 
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

void print_menu(void)
{
    printf("[A]dd merchendise\n"
           "[R]emove merchendise\n"
           "[E]dit merchendise\n"
           "[U]ndo\n"
           "[L]ist merchendise\n"
           "[Q]uit\n");
}

char ask_question_menu(char *question)
{
  print_menu();

  char result;
  do
  {
    result = ask_question_char(question);
  
  } while (!is_valid_char(&result));
  
  return toupper(result);
}

int read_string_to_buf(char *buf, const int buf_size, const char *string, const char character)
{
    int i = 0;
    while (!(*(string + i) == '\0'))
    {
        if (i >= buf_size)
        {
            break;
        }
    *(buf + i) = *(string + i);
        i++;
    }
    *(buf + i) = character;
    return i;
}

void print_item(merch_t *item)
{
    (void) item; 
    // printf("Name:  %s\n", item->name);
    // printf("Desc:  %s\n", item->desc);
    // printf("Price: %d.%02d SEK\n", item->price / 100, item->price % 100);
    // int i = 1;

    // if (ioopm_list_is_empty(item->shelfs))
    // {
    //     printf("Out of stock\n");
    //     return;
    // }
    
    // ioopm_list_iterator_t *it = ioopm_list_iterator_create(item->shelfs); 
    // while (!ioopm_list_iterator_at_end(it))
    // {
    //     shelf_t *s = it->current_entry->next;
    //     printf("Shelf_%d: %s\n", i, it->current_entry);
    //     i++;
    // }
    // ioopm_list_iterator_destroy(it); 
    
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

ioopm_shelf_t *ioopm_db_shelf_create(char *slf, size_t amount)
{
    ioopm_shelf_t *shelf = calloc(1, sizeof(ioopm_shelf_t));
    shelf->shelf = slf;
    shelf->amount = amount; 
    return shelf;
}

merch_t *ioopm_db_input_item()
{
    char *name = ask_question_string("Enter the name of the merch: ");
    char *desc = ask_question_string("Enter a description of the merch: ");
    int price = ask_question_int("Enter the price of the merch");

    ioopm_list_t *shelves = ioopm_list_create();
    char *shelf;
    size_t amount;
    char continu;

    // Loop until user has added all new shelves
    do
    {
        shelf = ask_question_shelf("Enter shelf: ");
        amount = ask_question_size_u("Enter amount of items that shelf:"); 
        
        elem_t ptr = ptr_elem(ioopm_db_shelf_create(shelf, amount)); // Cheat
        ioopm_list_append(shelves, ptr); 
        
        continu = ask_question_char("Did you add the item to any other shelf? (y) (n): ");
        
    } while (continu != 'n');
    
    return ioopm_db_item_create(name, desc, price, shelves); 
}

/// @brief Removes and frees all shelves with amount 0 from a merch.
///        Additionally, removes and frees all occupied shelves from a merch.
/// @param item the merch to clean up
/// @return the number of shelves left
static size_t ioopm_remove_empty_shelves(merch_t *item)
{
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(item->list); 
    while (!ioopm_list_iterator_at_end(it))
    {
        ioopm_shelf_t *current = ioopm_list_iterator_current(it).p; 
        if (current->amount < 1)
        {
            ioopm_db_shelf_destroy(current); 
            ioopm_list_iterator_remove(it);
        } else
        {
            ioopm_list_iterator_advance(it); 
        }
    }
    ioopm_list_iterator_destroy(it); 
    return ioopm_list_size(item->list); 
}

/// @brief Inserts a merch into ht_s and registers each of its shelves in ht_n.
///        Assumes that no merch with the same name exists.
/// @param db the database
/// @param item the merch to insert; the database takes ownership
/// @return true
static bool insert_new_item_to_db(ioopm_data_base_t *db, merch_t *item)
{
    // Remove any shelves with quantity 0 from item
    ioopm_remove_empty_shelves(item); // cheat
     
    // Insert to ht_s
    ioopm_hash_table_insert(db->ht_s, string_elem(item->name), ptr_elem(item));

    // Insert to ht_n
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(item->list);
    while (!ioopm_list_iterator_at_end(it))
    {
        ioopm_shelf_t *s = ioopm_list_iterator_current(it).p; 
        ioopm_hash_table_insert(db->ht_n, string_elem(s->shelf), string_elem(item->name));
        ioopm_list_iterator_advance(it); 
    }   
    ioopm_list_iterator_destroy(it); 
    return true; 
}

bool ioopm_db_any_shelf_occupied(ioopm_data_base_t *db, merch_t *item)
{
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(item->list); 
    while (!ioopm_list_iterator_at_end(it))
    {
        ioopm_shelf_t *current = ioopm_list_iterator_current(it).p;
        elem_t result = int_elem(0); 
        if (ioopm_hash_table_lookup(db->ht_n, string_elem(current->shelf), &result))
        {
            ioopm_list_iterator_destroy(it);
            return true; 
        }
        
        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it); 
    return false; 
}

bool ioopm_db_add_item_to_db(ioopm_data_base_t *db)
{
    merch_t *item = ioopm_db_input_item(db); 

    elem_t result = int_elem(0); 
    if (ioopm_hash_table_lookup(db->ht_s, string_elem(item->name), result.p))
    {
        return false; 
    } 
    
    insert_new_item_to_db(db, item); // Cheat
    return true;  
}

bool ioopm_db_add_item_to_db_test(ioopm_data_base_t *db, merch_t *item)
{
    elem_t result; 
    if (ioopm_hash_table_lookup(db->ht_s, string_elem(item->name), &result))
    {
        return false;
    } 
    if (ioopm_db_any_shelf_occupied(db, item))
    {
        return false; 
    }
    return insert_new_item_to_db(db, item); // Cheat
}

ioopm_list_t *ioopm_db_item_all_shelves(merch_t *item)
{
    ioopm_list_t *names = ioopm_list_create();

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(item->list);
    while (!ioopm_list_iterator_at_end(it))
    {
        ioopm_shelf_t *s = ioopm_list_iterator_current(it).p;
        ioopm_list_append(names, string_elem(s->shelf));
        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it);

    return names;
}

int ioopm_db_item_stock(merch_t *item)
{
    size_t result = 0;
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(item->list); 
    while (!ioopm_list_iterator_at_end(it))
    {
        ioopm_shelf_t *s = ioopm_list_iterator_current(it).p;
        result += s->amount;
        ioopm_list_iterator_advance(it); 
    }
    ioopm_list_iterator_destroy(it); 

    return result; 
}

elem_t ioopm_db_item_price(merch_t *item)
{
    return int_elem(item->price); 
}
elem_t ioopm_db_item_name(merch_t *item)
{
    return string_elem(item->name);
}
elem_t ioopm_db_item_desc(merch_t *item)
{
    return string_elem(item->desc);
}