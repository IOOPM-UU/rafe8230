#pragma once
#include <stdbool.h>
#include "db.h"

/**
 * @file ui.h
 * @author Rasmus Ferngren
 * @brief Text user interface for the webstore.
 *
 * Every ui_ function reads its input in the order given by the assignment
 * spec (one item per line), calls the backend in db.h, and prints the result.
 * Input format errors are handled by asking again; all other errors are
 * reported after all input has been read.
 */

/// @brief Prints the main menu.
void print_menu(void);

/// @brief Asks for an action until a valid action code is entered.
///        Lower and upper case are both accepted.
/// @param question the prompt to show
/// @return the chosen action code, in upper case
char ask_question_menu(char *question);

/// @brief Prints a merch: name, description, price and its storage locations.
/// @param item the merch to print
void print_item(merch_t *item);

// ---------- Merchandise ----------

/// @brief [A] Reads name, description and price, and adds a new merch.
///        A new merch has no stock.
/// @param db the database
void ui_add_merch(ioopm_data_base_t *db);

/// @brief [L] Lists all merch, 20 at a time. Listing stops if the answer
///        to "continue?" begins with N or n.
/// @param db the database
void ui_list_merch(ioopm_data_base_t *db);

/// @brief [D] Reads a merch name and a confirmation, and removes the merch
///        with all its stock if the confirmation begins with Y or y.
/// @param db the database
void ui_remove_merch(ioopm_data_base_t *db);

/// @brief [E] Reads the old name, new name, new description, new price and a
///        confirmation, and updates the merch if the confirmation begins with
///        Y or y. The stock is not changed.
/// @param db the database
void ui_edit_merch(ioopm_data_base_t *db);

/// @brief [S] Reads a merch name and prints its storage locations and the
///        amount on each.
/// @param db the database
void ui_show_stock(ioopm_data_base_t *db);

/// @brief [P] Reads a storage location, a merch name and an amount, and adds
///        that many items of the merch to the location.
/// @param db the database
void ui_replenish(ioopm_data_base_t *db);

// ---------- Shopping carts ----------

/// @brief [C] Creates a new, empty cart and prints its id.
/// @param db the database
void ui_create_cart(ioopm_data_base_t *db);

/// @brief [R] Reads a cart id and a confirmation, and removes the cart if the
///        confirmation begins with Y or y.
/// @param db the database
void ui_remove_cart(ioopm_data_base_t *db);

/// @brief [+] Reads a cart id, a merch name and an amount, and adds the
///        items to the cart.
/// @param db the database
void ui_add_to_cart(ioopm_data_base_t *db);

/// @brief [-] Reads a cart id, a merch name and an amount, and removes the
///        items from the cart.
/// @param db the database
void ui_remove_from_cart(ioopm_data_base_t *db);

/// @brief [=] Reads a cart id and prints the total cost of the cart.
/// @param db the database
void ui_calculate_cost(ioopm_data_base_t *db);

/// @brief [O] Reads a cart id, decreases the stock for everything in the
///        cart, and removes the cart.
/// @param db the database
void ui_checkout(ioopm_data_base_t *db);

// ---------- Other ----------

/// @brief [U] Undoes the last action (optional).
/// @param db the database
void ui_undo(ioopm_data_base_t *db);

/// @brief [Q] Reads a confirmation.
/// @return true if the program should quit (the answer begins with Y or y)
bool ui_quit(void);