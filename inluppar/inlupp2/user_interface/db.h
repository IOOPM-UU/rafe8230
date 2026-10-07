#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#include "linked_list_i2.h"
#include "hash_table_i2.h"
#include "linked_list_private_i2.h"
#include "hash_table_private_i2.h"
#include "common_i2.h"



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
    char *shelf;
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

/// @brief Frees a shelf and its name string.
/// @param slf the shelf to free
void ioopm_db_shelf_destroy(ioopm_shelf_t *slf);

/// @brief Frees every shelf in a merch's shelf list, including their name strings.
///        The list itself is NOT freed; call ioopm_list_destroy afterwards.
/// @param lst a list of ioopm_shelf_t * (in .p)
void ioopm_db_item_list_destroy(ioopm_list_t *lst);

/// @brief Frees a merch completely: its shelves, its shelf list, its name,
///        its description and the merch itself.
///        Do not call this on a merch that is still stored in a database;
///        use it for merch the database rejected, or let
///        ioopm_db_destroy free stored merch.
/// @param item the merch to free
void ioopm_db_item_destroy(merch_t *item);

/// @brief Creates a new, empty database.
/// @param hf hash function for the keys (merch names and shelf names), e.g. string_knr_hash
/// @param eq equality function for the keys, e.g. string_eq
/// @return a pointer to the new database
ioopm_data_base_t *ioopm_db_create(ioopm_hash_function hf, ioopm_eq_function eq);

/// @brief Destroys a database and frees all memory it owns,
///        including every merch, its shelves and all their strings.
/// @param db the database to destroy
void ioopm_db_destroy(ioopm_data_base_t *db);

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
ioopm_shelf_t *ioopm_db_shelf_create(char *slf, size_t amount);

/// @brief Reads a new merch from the user (name, description and price)
///        and adds it to the database.
/// @param db the database to add the merch to
/// @return true if the merch was added, false if a merch with that name already exists
bool ioopm_db_add_item_to_db(ioopm_data_base_t *db);

/// @brief Adds an already created merch to the database, without user input.
///        Shelves with amount 0 are removed from the merch first.
/// @param db the database to add the merch to
/// @param item the merch to add
/// @return true if the merch was added, in which case the database owns it;
///         false if a merch with the same name already exists or one of its
///         shelves is already used by another merch, in which case nothing
///         is changed and the caller still owns item
bool ioopm_db_add_item_to_db_test(ioopm_data_base_t *db, merch_t *item);

/// @brief Checks whether any of a merch's shelves is already registered
///        in the database (i.e. used by some merch).
/// @param db the database
/// @param item the merch whose shelves to check
/// @return true if at least one shelf already exists in the database, otherwise false
bool ioopm_db_any_shelf_occupied(ioopm_data_base_t *db, merch_t *item);

/// @brief Lists the names of all shelves a merch is stored on.
/// @param item the merch
/// @return a new list of shelf names (char * in .s), in the order they were added.
///         The caller must destroy the list with ioopm_list_destroy, but must NOT
///         free the strings, since they still belong to the merch.
ioopm_list_t *ioopm_db_item_all_shelves(merch_t *item);

/// @brief Calculates the total stock of a merch.
/// @param item the merch
/// @return the sum of the amounts on all shelves holding the merch (0 if none)
int ioopm_db_item_stock(merch_t *item);

/// @brief Gets the price of a merch.
/// @param item the merch
/// @return the price, in öre, in the .i field
elem_t ioopm_db_item_price(merch_t *item);

/// @brief Gets the name of a merch.
/// @param item the merch
/// @return the name in the .s field; the string still belongs to the merch
elem_t ioopm_db_item_name(merch_t *item);

/// @brief Gets the description of a merch.
/// @param item the merch
/// @return the description in the .s field; the string still belongs to the merch
elem_t ioopm_db_item_desc(merch_t *item);

/// @brief Prints all merch in the database, 20 at a time, in alphabetical order.
/// @param db the database to list
void ioopm_list_db(ioopm_data_base_t *db);

/// @brief Reads an old name, new name, new description, new price and a
///        confirmation from the user, and updates the merch. The stock is unchanged.
/// @param db the database containing the merch
void ioopm_edit_db(ioopm_data_base_t *db);

/// @brief Reads a merch name and a confirmation from the user, and removes
///        the merch together with all its stock from the database.
/// @param db the database to remove the merch from
void ioopm_remove_item_from_db(ioopm_data_base_t *db);

/// @brief Prints the main menu.
void print_menu(void);

/// @brief Prints the menu and asks for an action until a valid one is entered.
/// @param question the prompt to show
/// @return the chosen action code, in upper case
char ask_question_menu(char *question);

/// @brief Copies a string into a buffer and ends it with a given character.
/// @param buf the buffer to write to
/// @param buf_size the maximum number of characters to copy
/// @param string the string to copy
/// @param character the character to write after the copied characters
/// @return the number of characters copied
int read_string_to_buf(char *buf, const int buf_size, const char *string, const char character);

/// @brief Prints a merch: name, description, price and its shelves.
/// @param item the merch to print
void print_item(merch_t *item);

/// @brief Reads name, description and price for a new merch from the user.
/// @return the new merch, with no shelves
merch_t *ioopm_db_input_item();