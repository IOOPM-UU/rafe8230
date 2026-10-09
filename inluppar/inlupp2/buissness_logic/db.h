#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#include "linked_list_i2.h"
#include "hash_table_i2.h"
#include "linked_list_private_i2.h"
#include "hash_table_private_i2.h"
#include "common_i2.h"


/// @brief Result codes for database operations.
// (idea from ai but i will obviously implement it myself)
typedef enum
{
    DB_OK,             
    DB_NAME_TAKEN,     
    DB_SHELF_TAKEN,    
    DB_INVALID_AMOUNT,
    DB_NO_SUCH_MERCH,
    DB_EMPTY,
    DB_ALL_LISTED

} ioopm_db_result_t;


typedef struct merch
{
    char *name; 
    char *desc;
    int price;
    ioopm_list_t *list;
} merch_t;

typedef struct database
{
    ioopm_hash_table_t *ht_n; 
    ioopm_hash_table_t *ht_s; 
} ioopm_data_base_t;

typedef struct shelf

{
    size_t amount; 
    elem_t shelf;
} ioopm_shelf_t;

/**
 * @file db.h
 * @author Rasmus Ferngren
 * @date 7 oct 2026
 * @brief Backend for a simple webstore: merchandise, storage locations and stock.
 *
 * The database keeps two hash tables in sync:
 *  - ht_s maps a merch name to its merch_t
 *  - ht_n maps a storage location (shelf) name to the name of the merch stored there
 *
 * A storage location holds one kind of merch only, and always at least one item.
 *
 * Ownership: strings and lists given to ioopm_db_item_create and ioopm_db_shelf_create are
 * taken over by the merch/shelf. Once a merch has been added to the database,
 * the database owns it and frees it in ioopm_db_destroy.
 * 
 * All function definition was created by AI
 */
/// @brief Creates a new, empty database.
/// @param hf hash function for the keys (merch names and shelf names), e.g. string_knr_hash
/// @param eq equality function for the keys, e.g. string_eq
/// @return a pointer to the new database
ioopm_data_base_t *ioopm_db_create(ioopm_hash_function hf, ioopm_eq_function eq);

/// @brief Creates a merch from already allocated data.
/// @param name the name of the merch (heap allocated; the merch takes ownership)
/// @param desc the description of the merch (heap allocated; the merch takes ownership)
/// @param price the price of the merch, in öre
/// @param shelfs a list of ioopm_shelf_t * (in .p) where the merch is stored;
///               the merch takes ownership of the list and its shelves
/// @return a pointer to the new merch
merch_t *ioopm_db_item_create(char *name, char *desc, int price, ioopm_list_t *shelfs);

/// @brief Creates a storage location entry for a merch.
/// @param slf the name of the shelf, e.g. "A25" (heap allocated; the shelf takes ownership)
/// @param amount the number of items stored on the shelf
/// @return a pointer to the new shelf
ioopm_shelf_t *ioopm_db_shelf_create(elem_t slf, size_t amount);

/// @brief Frees a shelf and its name string.
/// @param slf the shelf to free
void ioopm_db_shelf_destroy(ioopm_shelf_t *slf);

/// @brief Frees every shelf in a merch's shelf list, including their name strings.
///        The list itself is NOT freed; call ioopm_list_destroy afterwards.
/// @param lst a list of ioopm_shelf_t * (in .p)
void ioopm_db_shelves_destroy(ioopm_list_t *lst);

/// @brief Frees a merch completely: its shelves, its shelf list, its name,
///        its description and the merch itself.
///        Do not call this on a merch that is still stored in a database;
///        use it for merch the database rejected, or let
///        ioopm_db_destroy free stored merch.
/// @param item the merch to free
void ioopm_db_item_destroy(merch_t *item);

/// @brief Destroys a database and frees all memory it owns,
///        including every merch, its shelves and all their strings.
/// @param db the database to destroy
void ioopm_db_destroy(ioopm_data_base_t *db);

/// @brief Adds an already created merch to the database, without user input.
///        Shelves with amount 0 are removed from the merch first.
/// @param db the database to add the merch to
/// @param item the merch to add
/// @return true if the merch was added, in which case the database owns it;
///         false if a merch with the same name already exists or one of its
///         shelves is already used by another merch, in which case nothing
///         is changed and the caller still owns item
ioopm_db_result_t ioopm_db_add_item(ioopm_data_base_t *db, char *name, char *desc, int price);


