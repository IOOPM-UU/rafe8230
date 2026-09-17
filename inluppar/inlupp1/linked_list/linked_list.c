#include <stdbool.h>
#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct list_entry list_entry_t; 

struct list_entry
{
    int value; 
    list_entry_t *next;
};

struct list 
{
    list_entry_t sentinel;
    list_entry_t *last;
    int size;
};


bool ioopm_list_is_empty(ioopm_list_t *list)
{
    return list->size == 0; 
}

ioopm_list_t *ioopm_list_create(void)
{
    ioopm_list_t *linked_list = calloc(1, sizeof(ioopm_list_t));
    linked_list->last = &linked_list->sentinel;
    return linked_list;
}

static list_entry_t *entry_create(int value)
{
    list_entry_t *entry = calloc(1, sizeof(list_entry_t));
    entry->value = value; 
    entry->next = NULL;
    return entry; 
}

void ioopm_list_destroy(ioopm_list_t *list)
{ 
    if (list->size < 1)
    {
        free(list); 
        return;
    }
    list_entry_t *entry = &list->sentinel;
    while (true)
    {
        if (!(list->sentinel.next == NULL))
        {
            list->sentinel.next = entry->next;
            free(entry->next);
        }
        
        free(entry);
        return;
    }
}
void ioopm_list_append(ioopm_list_t *list, int value)
{
    list_entry_t *entry = entry_create(value);
    list->last->next = entry;
    list->size++; 
    list->last = entry;
}

void ioopm_list_prepend(ioopm_list_t *list, int value)
{
    list_entry_t *entry = entry_create(value); 
    entry->next = list->sentinel.next;
    list->sentinel.next = entry; 

    if (list->size == 0)
    {
        list->last = entry; 
    }
    

    list->size++;  
}

int ioopm_list_head(ioopm_list_t *list)
{
    if (list->sentinel.next == NULL)
    {
        return -1;
    }
    
    return list->sentinel.next->value; 
    
}

int ioopm_list_last(ioopm_list_t *list)
{
    if (list->sentinel.next == NULL)
    {
        return -1;
    }
    return list->last->value; 
    
}

static list_entry_t *find_previous_entry(ioopm_list_t *list, int index)
{
    list_entry_t *previous = &list->sentinel;

    for (int i = 0; i < index; i++)
    {
        previous = previous->next;
    }
    return previous;
}

void ioopm_list_insert(ioopm_list_t *list, int index, int value)
{
    list_entry_t *entry = entry_create(value); 
    list_entry_t *previous = find_previous_entry(list, index); 
    entry->next = previous->next;
    previous->next = entry;

    if (index == list->size)
    {
        list->last = entry;
    }
    
    list->size++; 
    return;
}

int ioopm_list_remove(ioopm_list_t *list, int index)
{
    if (ioopm_list_is_empty(list))
    {
        return -1; 
    }
    
    list_entry_t *previous = find_previous_entry(list, index); 
    list_entry_t *current = previous->next;   
    previous->next = current->next; 
    
    if (index == list->size - 1)
    {
        list->last = previous; 
    }

    int value = current->value; 
    free(current); 
    list->size--;

    
    return value; 
}

int ioopm_list_get(ioopm_list_t *list, int index)
{
    list_entry_t *current = list->sentinel.next;

    if (list->size == 0)
    {
        return -1; 
    }
    

    for (int i = 0; i < index; i++)
    {
        current = current->next; 
    }
    return current->value;
}

int ioopm_list_size(ioopm_list_t *list)
{
    return list->size;
}
