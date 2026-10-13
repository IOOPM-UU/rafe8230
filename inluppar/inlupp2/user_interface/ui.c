
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
           "[C]reate cart : Not yet implemented\n"
           "[R]emove cart : Not yet implemented\n"
           "[+] Add to cart : Not yet implemented\n"
           "[-] Remove from cart : Not yet implemented\n"
           "[=] Calculate cost : Not yet implemented\n"
           "Check[O]ut : Not yet imlemented\n\n"
           "\n--- Other ---\n"
           "[U]ndo : Not yet implemented\n"
           "[Q]uit : Not yet implemented\n");
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

#pragma endregion

#pragma region backend

void ui_add_merch(ioopm_data_base_t *db)
{
    char *name = strdup(ask_question_string("Enter name: "));
    char *desc = strdup(ask_question_string("Enter description: ")); 
    int price  = ask_question_int("Enter price: ");

    ioopm_db_result_t res = ioopm_db_add_item(db, name, desc, price);
    
    switch (res)
    {
        case DB_OK:
            printf("%s added\n", name);
            break;
        case DB_NAME_TAKEN:
            printf("A merch named %s already exists\n", name);  
            break;
        case DB_INVALID_AMOUNT:
            printf("Invalid price entered");
            break; 
        default:
            printf("Could not add %s\n", name);
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
        printf("\n\n\nNo items in database to print\n\n\n");
        break;
    case DB_ALL_LISTED:
        printf("\n\n\nall itesm have been listed\n\n\n"); 
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
            printf("%s removed from database", u); 
            break;
        case DB_NO_SUCH_MERCH:
            printf("%s is not an item within the database", u); 
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
    name_old = ask_question_string("Select item to edit: ");
    
    char *name_new; 
    name_new = ask_question_string("Enter name: ");
    
    char *desc; 
    desc = ask_question_string("Enter description: ");

    int price; 
    price = ask_question_int("Enter price: "); 

    switch (ioopm_db_edit_merch(db, name_old, name_new, desc, price) /* STUB */)
    {
        case DB_OK:
            break;
        case DB_INVALID_AMOUNT:
            break; 
        case DB_NO_SUCH_MERCH:
            break;
        default:
            break;
    }

}

static void ui_print_shelves_arr(ioopm_shelf_t **shelves, size_t size, size_t total)
{
    sort_shelves_alphabetically(shelves, size); 

    ioopm_shelf_t *current = NULL;
    for (size_t i = 0; i < size; i++)
    {
        current = shelves[i];
        printf("%zu: Name: %s, Amount: %zu", i + 1, current->shelf.s, current->amount); 
    }
    
    printf("Total stock: %zu", total); 
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
        printf("No such merch in database");
        break;
    
    default:
        printf("Unable to produce item");
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
        printf("\n\n\n Item replenished \n\n\n");
        break;
    case DB_NO_SUCH_MERCH:
        printf("\n\n\n No such merch in database\n\n\n"); 
        break;
    case DB_INVALID_AMOUNT:
        printf("\n\n\n Invalid amount input\n\n\n"); 
        break;
    default:
        break;
    }

    if (res != DB_OK)
    {
        free(shelf);
        free(name);
    }
}

#pragma endregion 

#pragma region carts

void ui_create_cart(ioopm_data_base_t *db)
{
    (void) db;
}     

void ui_remove_cart(ioopm_data_base_t *db)
{
    (void) db;
    
}

void ui_add_to_cart(ioopm_data_base_t *db)
{
    (void) db;
    
}    

void ui_remove_from_cart(ioopm_data_base_t *db)
{
    (void) db;
    
} 

void ui_calculate_cost(ioopm_data_base_t *db)
{
    (void) db;
    
}

void ui_checkout(ioopm_data_base_t *db)
{
    (void) db;
    
} 

#pragma endregion

#pragma region others
void ui_undo(ioopm_data_base_t *db)
{
    (void) db;
    
}          

bool ui_quit()
{
    return false; 
}

#pragma endregion