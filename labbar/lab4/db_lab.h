#include <stdio.h>

typedef struct item 
{
    char *name; 
    char *desc;
    int price;
    char *shelf;
} item_t;

void print_item(item_t *item);

item_t make_item(char *name, char *desc, int price, char *shelf);

item_t input_item();

void add_item_to_db(item_t *db, int *db_size);