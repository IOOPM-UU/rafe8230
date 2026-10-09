
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

void print_menu(void)
{
    printf("\n--- Merchandise ---\n"
           "[A]dd merchandise\n"
           "[L]ist merchandise : Not yet implemented\n"
           "[D]elete merchandise : Not yet implemented\n"
           "[E]dit merchandise : Not yet implemented\n"
           "[S]how stock : Not yet impelmented\n"
           "Re[P]lenish : Not yet implemented\n\n"
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

/*
void print_item(merch_t *item)
{
    printf("Name:  %s\n", item->name);
    printf("Desc:  %s\n", item->desc);
    printf("Price: %d.%02d SEK\n", item->price / 100, item->price % 100);
    
    // TODO: print all shelves and their quantities
}
*/

static void print_names(char **name, size_t start, size_t end)
{
    for (size_t start = 0; start < end; start++)
    {
        printf("%zu: %s", start, name[start]);
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
        printf("No items in database");
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

void ui_add_merch(ioopm_data_base_t *db)
{
    char *name = ask_question_string("Enter name ");
    char *desc = ask_question_string("Enter description: ");
    int price  = ask_question_int("Enter price: ");

    switch (ioopm_db_add_item(db, name, desc, price))
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
    free(desc); 
    free(name);
}

void ui_list_merch(ioopm_data_base_t *db)
{
    size_t size;
    char **names = ioopm_db_merch_names(db, &size);

    sort_names_alphabetically(names, size); 
    
    switch (loop_names(names, size))
    {
    case DB_EMPTY:
        printf("No items in database to print");
        break;
    case DB_ALL_LISTED:
        printf("all itesm have been listed"); 
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
        answer = tolower(ask_question_char("Would you like to delete this item [y]/[n]")); 
    } while (answer != 'y' && answer != 'n');

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
    char *u; 
    u = ask_question_string("Select item to edit: ");

    char answer; 
    do
    {
        answer = tolower(ask_question_char("Would you like to edit this item [y]/[n]")); 
    } while (answer != 'y' && answer != 'n');

    if (answer == 'y')
    {
        switch (ioopm_db_edit_merch(db, u) /* STUB */)
        {
        case DB_OK:
            /* code */
            break;
        default:
            break;
        }
    }
    free(u); 
}

void ui_show_stock(ioopm_data_base_t *db)
{

    char *name;
    name = ask_question_string("Name item to show: ");

    ioopm_shelf_t **shelves;
    size_t size; 
    
    switch (ioopm_db_item_stock(db, name, &shelves, &size))
    {
    case DB_OK:
        ui_print_shelves_arr(&shelves, &size); // STUB
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

    switch (ioopm_db_replenish(db, name, shelf, amount) /* STUB */)
    {
    case DB_OK:
        printf("\n\n\n Item replenished \n\n\n");
        break;
    case DB_SHELF_TAKEN:
        printf("\n\n\n Shelf occupied\n\n\n"); 
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
    free(shelf);
    free(name);
}