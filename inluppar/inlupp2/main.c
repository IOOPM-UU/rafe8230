#include <stdio.h>
#include <stdbool.h>
#include "utils_i2.h"
#include "common_i2.h"
#include "db.h"
#include "ui.h"

void event_loop(ioopm_data_base_t *db)
{
    bool quit = false;
    while (!quit)
    {
        char answer = ask_question_menu("Choose an action: ");
        switch (answer)
        {
            // AI generated switch case (the switch case was my idea though
            // I just didnt want to type all this)

            // Merchandise
            case 'A': ui_add_merch(db);       break;   
            case 'L': ui_list_merch(db);      break;   
            case 'D': ui_remove_merch(db);    break;   
            case 'E': ui_edit_merch(db);      break;
            case 'S': ui_show_stock(db);      break;
            case 'P': ui_replenish(db);       break;

            // Shopping carts
            case 'C': ui_create_cart(db);     break;  
            case 'R': ui_remove_cart(db);     break;
            case '+': ui_add_to_cart(db);     break;
            case '-': ui_remove_from_cart(db); break;
            case '=': ui_calculate_cost(db);  break;
            case 'O': ui_checkout(db);        break;

            // Other
            case 'U': ui_undo(db);            break;
            case 'Q': quit = ui_quit();       break;
        }
    }
}

int main()
{
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    event_loop(db);
    ioopm_db_destroy(db);
    return 0;
}