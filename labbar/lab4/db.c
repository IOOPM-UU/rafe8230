#include <stdio.h>
#include "utils.h"
#include "db.h"
#include <time.h>
#include <stdlib.h>
#include <ctype.h>


void print_menu(void)
{
    printf("[L]ägga till en vara\n"
           "[T]a bort en vara\n"
           "[R]edigera en vara\n"
           "Ån[g]ra senaste ändringen\n"
           "Lista [h]ela varukatalogen\n"
           "[A]vsluta\n");
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

int read_string_to_buf(char *buf, const int buf_size, const char *string, const char character)
{
    int i = 0;
    while (!(*(string + i) == '\0'))
    {
        if (i >= buf_size)
        {
            break;
        }
    *(buf + i) = *(string + i);
        i++;
    }
    *(buf + i) = character;
    return i;
}

void print_item(item_t *item)
{
    printf("Name:  %s\n", item->name);
    printf("Desc:  %s\n", item->desc);
    printf("Price: %d.%02d SEK\n", item->price / 100, item->price % 100);
    printf("Shelf: %s\n", item->shelf);
}

item_t make_item(char *name, char *desc, int price, char *shelf)
{
    item_t item = {.name = name, .desc = desc, .price = price, .shelf = shelf};

    return item;
}

item_t input_item()
{
    char *name = ask_question_string("Write the name of the item: ");
    char *desc = ask_question_string("Add a description of the item: ");
    int price = ask_question_int("Enter the price of the item: "); 
    char *shelf = ask_question_shelf("Enter the location of the item: ");

    return make_item(name, desc, price, shelf);
}

int random_num(int size)
{
    
    int n = rand() % size;

    return n;
}

char *magick(char *arr1[], char *arr2[], char *arr3[], int size)
{
    int buf_size = 255; 
    char buf[buf_size];
    
    char *string1 = arr1[random_num(size)];
    int index = read_string_to_buf(buf, buf_size, string1, '-') + 1;
    
    
    char *string2 = arr2[random_num(size)];
    index += read_string_to_buf(buf + index, buf_size, string2, ' ') + 1;
    
    char *string3 = arr3[random_num(size)];
    read_string_to_buf(buf + index, buf_size, string3, '\0');
    

    return strdup(buf);    
}

void list_db(item_t *items, int no_items)
{
    for (int i = 0; i < no_items; i++)
    {
        printf("%d. %s\n", i + 1, items[i].name);
    }
    return;
}

void edit_db(item_t *items, const int no_items)
{
    int answer; 
    do
    {
        answer = ask_question_int("What item du you want to edit? \n");
        if (answer > no_items || answer < 1)
        {
            printf("item num %d, does not exist", answer);
            continue;
        } else 
        {
            break; 
        }
        
    } while (true);
    
    print_item(&items[answer - 1]);

    item_t new_item = input_item();
    
    items[answer-1] = new_item; 

    list_db(items, no_items); 

}

void add_item_to_db(item_t *db, int *db_siz)
{
    item_t new_item = input_item();
    (*db_siz)++; 
    db[*db_siz - 1] = new_item;
}

void remove_item_from_db(item_t *db, int *db_siz)
{
    list_db(db, *db_siz);
    int answer = 0;
    do
    {
        answer = ask_question_int("Choose an item from the list: "); 
        if (answer > *db_siz || answer < 1)
        {
            printf("item num %d, does not exist", answer);
            continue;
        } else 
        {
            break; 
        }
    } while (true);
    
    for (int i = answer - 1; i < *db_siz; i++)
    {
        db[i] = db[i + 1];
    }
    (*db_siz)--; 
}

void event_loop(item_t *db, int *db_siz)
{
    char answer;
    do
    {
        answer = ask_question_menu("Enter a valid character (LlTtRrGgHhAa): ");
        if (answer == 'A')
        {
            return;
        } else if (answer == 'L')
        {
            if (*db_siz >= 16)
            {
                printf("Too many items registerd\n");

                continue;
            }
            
            add_item_to_db(db, db_siz);
        } else if (answer == 'T')
        {
            remove_item_from_db(db, db_siz);
        } else if (answer == 'H')
        {
            list_db(db, *db_siz); 
            printf("\n\n\n");
        } else if (answer == 'R')
        {
            edit_db(db, *db_siz);
        } else if (answer == 'G')
        {
            printf("\n\nnot yet implemented\n\n");
        }
        
    } while (answer != 'A');
    return;
}


int main(int argc, char *argv[])
{
    srand(time(NULL));

    char *array1[] = { "from", "hell", "chaos" }; // TODO: Lägg till!
    char *array2[] = { "crit", "krieg", "fortress" }; // TODO: Lägg till!
    char *array3[] = { "Ioopm", "middle", "man" }; // TODO: Lägg till!

  if (argc < 2)
  {
    printf("Usage: %s number\n", argv[0]);
  }
  else
  {
    item_t db[16]; // Array med plats för 16 varor
    int db_siz = 0; // Antalet varor i arrayen just nu

    int items = atoi(argv[1]); // Antalet varor som skall skapas

    if (items > 0 && items <= 16)
    {
      for (int i = 0; i < items; ++i)
      {
        // Läs in en vara, lägg till den i arrayen, öka storleksräknaren
        if (items > 1)
        {
            printf("\nNext item: %d\n", i + 2);
        }
        
        item_t item = input_item();
        db[db_siz] = item;
        ++db_siz;
      }
    }
    else
    {
      puts("Sorry, must have [1-16] items in database.");
      return 1; // Avslutar programmet!
    }
    
        for (int i = db_siz; i < 16; ++i)
          {
            char *name = magick(array1, array2, array3, 3); // TODO: Lägg till storlek
            char *desc = magick(array1, array2, array3, 3); // TODO: Lägg till storlek
            int price = random() % 200000;
            char shelf[] = { random() % ('Z'-'A') + 'A',
                             random() % 10 + '0',
                             random() % 10 + '0',
                             '\0' };
            item_t item = make_item(name, desc, price, strdup(shelf));
    
            db[db_siz] = item;
            ++db_siz;
        }
        
        // Skriv ut innehållet
        for (int i = 0; i < db_siz; ++i)
        {
            print_item(&db[i]);
        }
        
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
         

    event_loop(db, &db_siz); 
    








  }
  return 0;
}