#pragma once
#include "db.h"
#include <stdbool.h>

/// @brief Prints the main menu.
void print_menu(void);

/// @brief Prints the menu and asks for an action until a valid one is entered.
/// @param question the prompt to show
/// @return the chosen action code, in upper 
char ask_question_menu(char *question);

/// @brief Prints a merch: name, description, price and its shelves.
/// @param item the merch to print
void print_item(merch_t *item);

/// @brief Reads name, description and price for a new merch from the user.
/// @return the new merch


// Owner actions
void ui_add_merch(ioopm_data_base_t *db);
void ui_list_merch(ioopm_data_base_t *db);         
void ui_remove_merch(ioopm_data_base_t *db);       
void ui_edit_merch(ioopm_data_base_t *db);      
void ui_show_stock(ioopm_data_base_t *db);      
void ui_replenish(ioopm_data_base_t *db);       

// Shopping carts
void ui_create_cart(ioopm_data_base_t *db);       
void ui_remove_cart(ioopm_data_base_t *db);     
void ui_add_to_cart(ioopm_data_base_t *db);     
void ui_remove_from_cart(ioopm_data_base_t *db); 
void ui_calculate_cost(ioopm_data_base_t *db);  
void ui_checkout(ioopm_data_base_t *db);        

// Other
void ui_undo(ioopm_data_base_t *db);            
bool ui_quit();       