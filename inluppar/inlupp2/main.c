#include "utils_i2.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "hash_table_i2.h"
#include "db.h"


void event_loop(ioopm_data_base_t *db)
{
    char answer;
    do
    {
        answer = ask_question_menu("Enter a valid character (LlTtRrGgHhAa): ");
        if (answer == 'Q')
        {
            return;
        } else if (answer == 'A') 
        {
            ioopm_add_item_to_db(db);
        } else if (answer == 'R')
        {
            ioopm_remove_item_from_db(db);
        } else if (answer == 'L')
        {
            ioopm_list_db(db); 
            printf("\n\n\n");
        } else if (answer == 'E')
        {
            ioopm_edit_db(db);
        } else if (answer == 'G')
        {
            printf("\n\nnot yet implemented\n\n");
        }
        
    } while (answer != 'Q');
    return;
}


int main()
{
    ioopm_data_base_t *db = ioopm_data_base_create(string_knr_hash, string_eq);
    event_loop(db); 
    
    return 0;
}