
#include <stdio.h>                    
#include <stdlib.h>                   
#include <ctype.h>                    
#include "ui.h"                       
#include "db.h"                       
#include "utils_i2.h"                 
#include "common_i2.h"                
#include "linked_list_i2.h"           
#include "linked_list_iterator_i2.h"  
#include "hash_table_iterator_i2.h"

#pragma region helpers

void print_menu(void)
{
    printf("\n--- Merchandise ---\n"
           "[A]dd merchandise\n"
           "[L]ist merchandise \n"
           "[D]elete merchandise \n"
           "[E]dit merchandise \n"
           "[S]how stock \n"
           "Re[P]lenish \n\n"
           "\n--- Shopping carts ---\n"
           "[C]reate cart\n"
           "[R]emove cart \n"
           "[+] Add to cart \n"
           "[-] Remove from cart \n"
           "[=] Calculate cost \n"
           "Check[O]ut \n\n"
           "\n--- Other ---\n"
           "[U]ndo : Not yet implemented\n"
           "[Q]uit \n");
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

static void print_names(char **name, size_t strt, size_t end)
{
    for (size_t start = strt; start < end; start++)
    {
        printf("\n%zu: %s", start + 1, name[start]);
    }
}

/// @brief loop over all names in an array, printing them 
///        and asking the user if they would like to continue 
///        printing after every 20th element
/// @param names array of names
/// @param size size of the given array of namse
/// @return false if all items have been listed, false otherwise
static ioopm_db_result_t loop_names(char **names, size_t size)
{
    if (size == 0)
    {
        return DB_EMPTY;
    }

    for (size_t i = 0; i < size; i += 20)
    {
        size_t end = (i + 20 < size) ? i + 20 : size; 
        print_names(names, i, end); 

        if (end < size)
        {
            char answer = tolower(ask_question_char("Continue listing? (to select an item, type 'n') [y]/[n]\n\n"));
            if (answer == 'n')
            {
                return DB_OK; 
            }
        }
    }
    return DB_ALL_LISTED;
}

/// @brief print the ids of all carts
/// @param db the database
/// @return a boolean representing if db is empty of carts
static bool cart_print_start(ioopm_data_base_t *db)
{
    ioopm_hash_table_t *carts = ioopm_get_carts(db); 
    if (ioopm_hash_table_size(carts) == 0)
    {
        printf("\n\nNo carts in database\n\n"); 
        return true; 
    } 
    else 
    {
        ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(carts);
        while (!ioopm_hash_table_iterator_at_end(it))
        {
            size_t current = ioopm_hash_table_iterator_current_key(it).i;
            printf("\n%zu", current); 
            ioopm_hash_table_iterator_advance(it); 
        }
        ioopm_hash_table_iterator_destroy(it);
        printf("\n");
        return false; 
    }
}
#pragma endregion

#pragma region backend

void ui_add_merch(ioopm_data_base_t *db)
{
    char *name = ask_question_string("Enter name: ");
    char *desc = ask_question_string("Enter description: "); 
    int price  = ask_question_int("Enter price: ");

    ioopm_db_result_t res = ioopm_db_add_item(db, name, desc, price);
    
    switch (res)
    {
        case DB_OK:
            printf("\n\n%s added\n\n", name);
            break;
        case DB_NAME_TAKEN:
            printf("\n\nA merch named %s already exists\n\n", name);  
            break;
        case DB_INVALID_AMOUNT:
            printf("\n\nInvalid price entered\n\n");
            break; 
        default:
            printf("\n\nCould not add %s\n", name);
            break;
    }
   if (res != DB_OK)
    {
        free(name);
        free(desc);
    }
}

void ui_list_merch(ioopm_data_base_t *db)
{
    size_t size;
    char **names = ioopm_db_merch_names(db, &size);

    sort_names_alphabetically(names, size); 
    
    switch (loop_names(names, size))
    {
    case DB_EMPTY:
        printf("\n\nNo items in database to print\n\n");
        break;
    case DB_ALL_LISTED:
        printf("\n\nall itesm have been listed\n\n"); 
        break;
    case DB_OK:
        // All went as user expected
        // No print needed for confirmation
        break;
    default:
        break;
    }

    free(names); 
}

void ui_remove_merch(ioopm_data_base_t *db)
{
    char *u;
    u = ask_question_string("Select item to remove: ");

    char answer; 
    do
    {
        answer = ask_question_char("Would you like to delete this item [y]/[n]: "); 
        if (tolower(answer) == 'y' || tolower(answer) == 'n')
        {
            break; 
        }
        
    } while (true);

    
    if (answer == 'y')
    {
        switch (ioopm_db_remove_item(db, u))
        {
        case DB_OK:
            printf("\n\n%s removed from database\n\n", u); 
            break;
        case DB_NO_SUCH_MERCH:
            printf("\n\n%s is not an item within the database\n\n", u); 
            break;
        case DB_ERROR:
            printf("\n\nThis error should be impossible\n\n"); 
            break; 
        default:
            break;
        }
    }
    free(u); 
}

void ui_edit_merch(ioopm_data_base_t *db)
{
    char *name_old; 
    name_old = ask_question_string("\n\nSelect item to edit: ");
    
    char *name_new; 
    name_new = ask_question_string("\n\nEnter name: ");
    
    char *desc; 
    desc = ask_question_string("\n\nEnter description: ");

    int price; 
    price = ask_question_int("\n\nEnter price: "); 

    ioopm_db_result_t res = ioopm_db_edit_merch(db, name_old, name_new, desc, price);
    switch (res)
    {
        case DB_OK:
            break;
        case DB_INVALID_AMOUNT:
            printf("\n\nInvalid amount\n\n"); 
            break; 
        case DB_NO_SUCH_MERCH:
            printf("\n\nMerch does not exsist\n\n"); 
            break;
        case DB_NAME_TAKEN:
            printf("\n\nName already occupied\n\n"); 
            break; 
        default:
            printf("\n\nHow did we get here?\n\n");
            break;
    }
    if (res != DB_OK)
    {
        free(name_new);
        free(desc);
    }
    free(name_old);
    

}

/// @brief Sort and print an array of shelves
/// @param shelves an array of shelves
/// @param size the size of the array
/// @param total amount of the stock for an item
static void ui_print_shelves_arr(ioopm_shelf_t **shelves, size_t size, size_t total)
{
    sort_shelves_alphabetically(shelves, size); 

    ioopm_shelf_t *current = NULL;
    for (size_t i = 0; i < size; i++)
    {
        current = shelves[i];
        printf("\n%zu: Self: %s, Amount: %zu", i + 1, 
                ioopm_db_get_shelf(current), 
                ioopm_db_get__shelf_amount(current)); 
    }
    
    printf("\nTotal stock: %zu\n\n", total); 
} 

void ui_show_stock(ioopm_data_base_t *db)
{
    char *name;
    name = ask_question_string("Name item to show: ");
    ioopm_shelf_t **shelves;
    size_t size; 
    size_t total;
    switch (ioopm_db_item_stock(db, name, &shelves, &size, &total))
    {
    case DB_OK:
        ui_print_shelves_arr(shelves, size, total); 
        free(shelves); 
        break;
    case DB_NO_SUCH_MERCH:
        printf("\n\nNo such merch in database\n\n");
        break;
    case DB_ERROR:
        printf("\n\nShould not be possible to be here\n\n"); 
        break;
    
    default:
        printf("\n\nUnable to produce item\n\n");
        break;
    }
    free(name); 
}

void ui_replenish(ioopm_data_base_t *db)
{
    char *shelf = ask_question_shelf("Shelf name: ");
    char *name = ask_question_string("Merch name: ");
    size_t amount = ask_question_size_u("Amount: ");  

    ioopm_db_result_t res = ioopm_db_replenish(db, name, shelf, amount);
    switch (res)
    {
    case DB_OK:
        printf("\n\n Item replenished \n\n");
        break;
    case DB_NO_SUCH_MERCH:
        printf("\n\n No such merch in database\n\n"); 
        break;
    case DB_INVALID_AMOUNT:
        printf("\n\n Invalid amount input\n\n"); 
        break;
    case DB_ERROR:
        printf("\n\nhow did we get here?\n\n"); 
        break;
    default:
        printf("\n\nWHAT???\n\n");
        break;
    }

    free(shelf);
    free(name);
    
}

#pragma endregion 

#pragma region carts

void ui_create_cart(ioopm_data_base_t *db)
{
    ioopm_db_result_t res = ioopm_cart_create(db, string_knr_hash, string_eq); // STUB

    switch (res)
    {
    case DB_OK:
        break;
    case CART_CREATION_FAILED:
        printf("\n\nCart was not created\n\n");
        break;
    default:
        printf("\n\nWHAT\n\n"); 
        break;
    }
}     

void ui_remove_cart(ioopm_data_base_t *db)
{
    if (cart_print_start(db)) return;

    size_t cart = ask_question_size_u("Select the id of the cart to remove: ");
    
    char answer; 
    do
    {
        answer = ask_question_char("Would you like to delete this cart? [y]/[n]: ");
        if (answer == 'n') return; 
    } while (answer != 'y');

    ioopm_db_result_t res = ioopm_cart_destroy(db, cart); // STUB

    switch (res)
    {
    case DB_OK:
        break;
    case CART_DOES_NOT_EXSIST:
        printf("\n\nINVALID CART ID\n\n"); 
        break;
    default:
        break;
    }
}

void ui_add_to_cart(ioopm_data_base_t *db)
{
    if (cart_print_start(db)) return;
    size_t cart = ask_question_size_u("Select the id of the cart to add items to: ");

    char *merch_name = ask_question_string("Select the merch to add: ");
    size_t amount = ask_question_size_u("Select quantity to add: ");

    ioopm_db_result_t res = ioopm_cart_increase_quantity(db, cart, merch_name, amount); // STUB
    
    switch (res)
    {
    case DB_OK:
        break;
    case DB_OK_NO_MERCH_ADDED: 
        // Same as db_ok just here for me to free merch_name later
        break;
    case CART_DOES_NOT_EXSIST:
        printf("\n\nCart id does not exist\n\n"); 
        break;
    case DB_INVALID_AMOUNT:
        printf("\n\nInvalid amount, stock not added\n\n");
        break;
    case DB_EMPTY:
        printf("\n\nNo items in database\n\n"); 
        break;
    case CART_NOT_ENOUGH_STOCK:
        printf("\n\nNot enough reserved stock\n\n");
        break;
    case DB_ERROR:
        printf("\n\nhow did we get here? \n\n"); 
        break;
    default:
        printf("\n\nwhat did you do??\n\n"); 
        break;
    }

    if (res != DB_OK)
    {
        free(merch_name); 
    }
    
}    

void ui_remove_from_cart(ioopm_data_base_t *db)
{
    if (cart_print_start(db)) return;
    size_t cart = ask_question_size_u("Select the id of the cart to remove items from: ");
    
    char *merch_name = ask_question_string("Select the merch to remove items from: ");
    size_t amount = ask_question_size_u("Select quantity to remove: ");
    
    ioopm_db_result_t res = ioopm_cart_decrease_quantity(db, cart, merch_name, amount); // STUB
    
    switch (res)
    {
    case DB_OK:
        /* code */
        break;
    case CART_DOES_NOT_EXSIST:
        printf("\n\nCart does not exist\n\n"); 
        break;
    case DB_INVALID_AMOUNT:
        printf("\n\nInvalid quantity\n\n"); 
        break;
    case DB_NO_SUCH_MERCH:
        printf("\n\nNo such merch in cart\n\n"); 
        break;
    default:
        printf("\n\nWHAT??\n\n"); 
        break;
    }

    free(merch_name); 
    
} 

void ui_calculate_cost(ioopm_data_base_t *db)
{
    if (cart_print_start(db)) return;
    size_t cart = ask_question_size_u("Select the id of the cart to calculate cost: ");

    size_t cost; 
    ioopm_db_result_t res = ioopm_cart_calculate_cost(db, cart, &cost); // STUB 
    
    switch (res)
    {
    case DB_OK:
        printf("Total: %zu", cost); 
        break;
    case CART_DOES_NOT_EXSIST:
        printf("\n\nNo such cart\n\n"); 
        break;
    case CART_EMPTY:
        printf("\n\nEmpty cart: 0$\n\n"); 
        break;
    case DB_ERROR:
        printf("\n\nNah\n\n"); 
        break;
    default:
        printf("\n\nWHAT THE FUCL\n\n"); 
        break;
    }
    
}

void ui_checkout(ioopm_data_base_t *db)
{
    if (cart_print_start(db)) return;
    size_t cart = ask_question_size_u("Select the id of the cart to checkout: ");

    ioopm_db_result_t res = ioopm_cart_checkout(db, cart); // STUB
    switch (res)
    {
    case DB_OK:
        /* code */
        break;
    case CART_DOES_NOT_EXSIST:
        printf("\n\nInvalid cart id\n\n");
        break;
    case DB_NO_SUCH_MERCH:
        printf("\n\nNo such merch in cart\n\n"); 
        printf("\nThis should not be possible\n\n"); 
    break;
    case DB_ERROR:
        printf("\n\nwhat?\n\n");
        break;
    case CART_EMPTY:
        printf("\n\nNo items in cart\n\n"); 
        break;
    default:
        break;
    } 
} 


#pragma endregion

#pragma region others
void ui_undo(ioopm_data_base_t *db)
{
    (void) db;
    
}          

bool ui_quit()
{
    return true; 
}

#pragma endregion