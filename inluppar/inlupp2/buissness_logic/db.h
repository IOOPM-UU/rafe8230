#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#include "linked_list_i2.h"
#include "hash_table_i2.h"
#include "linked_list_private_i2.h"
#include "hash_table_private_i2.h"
#include "common_i2.h"


/// @brief Result codes for database and cart operations.
typedef enum
{
    DB_OK,
    DB_NAME_TAKEN,
    DB_SHELF_TAKEN,
    DB_INVALID_AMOUNT,
    DB_NO_SUCH_MERCH,
    DB_EMPTY,
    DB_ALL_LISTED,
    DB_ERROR,

    CART_DOES_NOT_EXSIST,
    CART_EMPTY,
    CART_CREATION_FAILED,
    CART_NOT_ENOUGH_STOCK,
    DB_OK_NO_MERCH_ADDED
} ioopm_db_result_t;


typedef struct shelf ioopm_shelf_t;
typedef struct merch merch_t;
typedef struct database ioopm_data_base_t;
/**
 * @file db.h
 * @author Rasmus Ferngren
 * @date 7 oct 2026
 * @brief Backend for a simple webstore: merchandise, storage locations and stock.
 *
 * The database stores several hash tables:
 *  - `name_id_ht` maps a merch name to its unique integer id.
 *  - `ht_s` maps a merch id to the `merch_t *` for that item.
 *  - `ht_n` maps a shelf label (for example "A25") to the owning merch id.
 *  - `carts` maps a cart id to a per-cart hash table whose keys are merch names.
 *
 * A storage location belongs to exactly one merchandise item and may hold zero or more
 * units for that item. The database owns all `merch_t`, every `ioopm_shelf_t` in a merch's
 * list, and the strings stored inside them.
 *
 * Ownership convention for string arguments:
 *  - The caller owns the strings passed to `ioopm_db_add_item`, `ioopm_db_edit_merch`,
 *    and `ioopm_db_replenish` until the function returns `DB_OK`.
 *  - After a successful insertion or update, the database owns the stored strings.
 *  - Callers that pass borrowed strings to lookups/removals must ensure the strings stay
 *    valid for the duration of the call.
 */


// Helper functions for tests: 
char *ioopm_db_get_shelf(ioopm_shelf_t *shlf);
size_t ioopm_db_get_shelf_amount(ioopm_shelf_t *shlf);
size_t ioopm_db_get__shelf_amount(ioopm_shelf_t *shlf);

ioopm_hash_table_t *ioopm_get_carts(ioopm_data_base_t *db);
ioopm_hash_table_t *ioopm_get_name_id_ht(ioopm_data_base_t *db);
ioopm_hash_table_t *ioopm_get_ht_n(ioopm_data_base_t *db);
ioopm_hash_table_t *ioopm_get_ht_s(ioopm_data_base_t *db);

char *ioopm_get_merch_name(merch_t *m);
char *ioopm_get_merch_desc(merch_t *m);
int ioopm_get_merch_id(merch_t *m);
ioopm_list_t *ioopm_get_merch_list(merch_t *m);
int ioopm_get_merch_price(merch_t *m);


/// @brief Creates a new empty database.
/// @param hf_s hash function used for string keys such as merch names and shelf labels
/// @param eq_s equality function used for string keys
/// @param hf_u hash function used for integer/identifier keys in the merch table
/// @param eq_u equality function used for integer/identifier keys
/// @return a pointer to the new database, or NULL on allocation failure
ioopm_data_base_t *ioopm_db_create(ioopm_hash_function hf_s, ioopm_eq_function eq_s, ioopm_hash_function hf_u, ioopm_eq_function eq_u);

/// @brief Creates a merch wrapper from caller-owned data.
/// @param db the database that owns the generated id
/// @param name heap-allocated merch name; the caller keeps ownership until the merch is inserted
/// @param desc heap-allocated merch description; the caller keeps ownership until the merch is inserted
/// @param price the price in öre
/// @param shelfs a list of `ioopm_shelf_t *` entries for the merch; the function stores the pointer and
///              increments the database's next id
/// @return a newly allocated merch with a unique id
merch_t *ioopm_db_item_create(ioopm_data_base_t *db, char *name, char *desc, int price, ioopm_list_t *shelfs);

/// @brief Creates a new shelf entry with a storage label and stock count.
/// @param slf a shelf label stored in `elem_t.s`
/// @param amount the number of units currently stored on the shelf
/// @return a newly allocated shelf object
ioopm_shelf_t *ioopm_db_shelf_create(elem_t slf, size_t amount);

/// @brief Frees the shelf and the string stored in `slf->shelf.s`.
/// @param slf the shelf to destroy
void ioopm_db_shelf_destroy(ioopm_shelf_t *slf);

/// @brief Destroys all shelves in a list and frees their strings.
/// @param lst a list of `ioopm_shelf_t *` values
void ioopm_db_shelves_destroy(ioopm_list_t *lst);

/// @brief Frees a merch and all objects it owns: shelves, list, strings and the item itself.
/// @param item the merch to destroy
void ioopm_db_item_destroy(merch_t *item);

/// @brief Destroys the complete database and frees all owned merch, shelf data and carts.
/// @param db the database to destroy
void ioopm_db_destroy(ioopm_data_base_t *db);

/// @brief Inserts a merch into the database by name and id.
/// @param db the database to modify
/// @param name the merch name; the database takes ownership on success
/// @param desc the merch description; the database takes ownership on success
/// @param price the price in öre
/// @return `DB_OK` on success, `DB_NAME_TAKEN` if the name already exists,
///         or `DB_INVALID_AMOUNT` if price is negative
ioopm_db_result_t ioopm_db_add_item(ioopm_data_base_t *db, char *name, char *desc, int price);

/// @brief Creates an array of merch names currently stored in the database.
/// @param db the database to inspect
/// @param size output parameter holding the number of names returned
/// @return an array of `char *` names, or NULL when the database is empty
char **ioopm_db_merch_names(ioopm_data_base_t *db, size_t *size);

/// @brief Removes a merch item and all of its stock from the database.
/// @param db the database to modify
/// @param name the merch name to remove
/// @return `DB_OK` when removed, `DB_NO_SUCH_MERCH` if absent, or `DB_ERROR` if the
///         internal state is inconsistent
ioopm_db_result_t ioopm_db_remove_item(ioopm_data_base_t *db, char *name);

/// @brief Renames or reprices an existing merch without changing its stock locations.
/// @param db the database to modify
/// @param name_old current merch name
/// @param name_new new merch name; the database takes ownership on success
/// @param desc new description; the database takes ownership on success
/// @param price new price in öre
/// @return `DB_OK`, `DB_NO_SUCH_MERCH`, `DB_NAME_TAKEN`, or `DB_INVALID_AMOUNT`
ioopm_db_result_t ioopm_db_edit_merch(ioopm_data_base_t *db, char *name_old, char *name_new, char *desc, int price);

/// @brief Returns the entire stock record of one merch item.
/// @param db the database to inspect
/// @param name the merch name to look up
/// @param shelves output pointer to an array of `ioopm_shelf_t *` entries; caller frees with `free`
/// @param size output number of shelf entries in the array
/// @param total output total stock amount across all shelves
/// @return `DB_OK` for a valid item, `DB_NO_SUCH_MERCH` if missing, or `DB_ERROR` if the stored state is inconsistent
ioopm_db_result_t ioopm_db_item_stock(ioopm_data_base_t *db, char *name, ioopm_shelf_t ***shelves, size_t *size, size_t *total);

/// @brief Adds stock to a named shelf for a merch.
/// @param db the database to modify
/// @param name merch name
/// @param shelf shelf label
/// @param amount positive number of units to add
/// @return `DB_OK` on success, `DB_NO_SUCH_MERCH` if the item does not exist,
///         `DB_SHELF_TAKEN` if the shelf is owned by another merch,
///         or `DB_INVALID_AMOUNT` if `amount < 1`
ioopm_db_result_t ioopm_db_replenish(ioopm_data_base_t *db, char *name, char *shelf, int amount);

/// @brief Creates a new, empty shopping cart with its own hash table.
/// @param db the database owning the cart container
/// @param s_hash hash function for merch names inside the cart
/// @param s_eq equality function for merch names inside the cart
/// @return `DB_OK` on success, `CART_CREATION_FAILED` if allocation failed
ioopm_db_result_t ioopm_cart_create(ioopm_data_base_t *db, ioopm_hash_function s_hash, ioopm_eq_function s_eq);

/// @brief Removes a cart and frees the per-cart table and all stored item keys.
/// @param db the database to modify
/// @param cart cart id to destroy
/// @return `DB_OK` on success or `CART_DOES_NOT_EXSIST` if the id is invalid
ioopm_db_result_t ioopm_cart_destroy(ioopm_data_base_t *db, size_t cart);

/// @brief Adds a quantity of one merch to the given cart, respecting available stock.
/// @param db the database containing the merch and the cart
/// @param cart cart id
/// @param merch_name merch name to add
/// @param amount positive quantity to add
/// @return `DB_OK` if the cart was changed, `DB_OK_NO_MERCH_ADDED` if it already existed but was updated,
///         `DB_EMPTY` if the database has no items, `DB_INVALID_AMOUNT` if the quantity is invalid,
///         `CART_DOES_NOT_EXSIST` if there is no such cart, `CART_NOT_ENOUGH_STOCK` if not enough stock exists,
///         or `DB_NO_SUCH_MERCH` if the merch does not exist in the database
ioopm_db_result_t ioopm_cart_increase_quantity(ioopm_data_base_t *db, size_t cart, char *merch_name, size_t amount);

/// @brief Removes a quantity of one merch from the given cart.
/// @param db the database containing the cart
/// @param cart cart id
/// @param merch_name merch name to remove
/// @param amount positive quantity to remove
/// @return `DB_OK`, `CART_DOES_NOT_EXSIST`, `DB_INVALID_AMOUNT`, or `DB_NO_SUCH_MERCH`
ioopm_db_result_t ioopm_cart_decrease_quantity(ioopm_data_base_t *db, size_t cart, char *merch_name, size_t amount);

/// @brief Calculates the total monetary cost of the cart in the database's unit.
/// @param db the database containing the cart
/// @param cart cart id
/// @param cost output parameter holding the total cost
/// @return `DB_OK`, `CART_DOES_NOT_EXSIST`, `CART_EMPTY`, or `DB_ERROR`
ioopm_db_result_t ioopm_cart_calculate_cost(ioopm_data_base_t *db, size_t cart, size_t *cost);

/// @brief Checks out a cart by removing the merch from stock and deleting the cart.
/// @param db the database containing the cart
/// @param cart cart id
/// @return `DB_OK`, `CART_DOES_NOT_EXSIST`, `CART_EMPTY`, `DB_NO_SUCH_MERCH`, or `DB_ERROR`
ioopm_db_result_t ioopm_cart_checkout(ioopm_data_base_t *db, size_t cart);
