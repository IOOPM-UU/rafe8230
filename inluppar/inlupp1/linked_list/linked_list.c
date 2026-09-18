#include <stdbool.h>
#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include "linked_list_private.h"
#include "common.h"




bool ioopm_list_is_empty(const ioopm_list_t *list)
{
    return list->size == 0; 
}

ioopm_list_t *ioopm_list_create(void)
{
    ioopm_list_t *linked_list = calloc(1, sizeof(ioopm_list_t));
    linked_list->last = &linked_list->sentinel;
    return linked_list;
}

list_entry_t *entry_create(elem_t value)
{
    list_entry_t *entry = calloc(1, sizeof(list_entry_t));
    entry->value = value; 
    entry->next = NULL;
    return entry; 
}

void ioopm_list_destroy(ioopm_list_t *list)
{ 
    list_entry_t *entry = list->sentinel.next;
    while (entry != NULL)
    {
        list_entry_t *next = entry->next;
        free(entry); 
        entry = next; 
    }
    free(list); 
}
void ioopm_list_append(ioopm_list_t *list, elem_t value)
{
    list_entry_t *entry = entry_create(value);
    list->last->next = entry;
    list->size++; 
    list->last = entry;
}

void ioopm_list_prepend(ioopm_list_t *list, elem_t value)
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

elem_t ioopm_list_head(const ioopm_list_t *list)
{
    if (list->sentinel.next == NULL)
    {
        return int_elem(-1);
    }
    
    return list->sentinel.next->value; 
    
}

elem_t ioopm_list_last(const ioopm_list_t *list)
{
    if (list->sentinel.next == NULL)
    {
        return int_elem(-1);
    }
    return list->last->value; 
    
}

static list_entry_t *find_previous_entry(ioopm_list_t *list, const size_t index)
{
    list_entry_t *previous = &list->sentinel;

    for (size_t i = 0; i < index; i++)
    {
        previous = previous->next;
    }
    return previous;
}

void ioopm_list_insert(ioopm_list_t *list, size_t index, elem_t value)
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

elem_t ioopm_list_remove(ioopm_list_t *list, size_t index)
{
    if (ioopm_list_is_empty(list))
    {
        return int_elem(-1); 
    }
    
    list_entry_t *previous = find_previous_entry(list, index); 
    list_entry_t *current = previous->next;   
    previous->next = current->next; 
    
    if (index == list->size - 1)
    {
        list->last = previous; 
    }

    elem_t value = current->value; 
    free(current); 
    list->size--;

    
    return value; 
}

elem_t ioopm_list_get(const ioopm_list_t *list, const size_t index)
{
    list_entry_t *current = list->sentinel.next;

    if (list->size == 0)
    {
        return int_elem(-1); 
    }
    
    for (size_t i = 0; i < index; i++)
    {
        current = current->next; 
    }
    return current->value;
}

size_t ioopm_list_size(const ioopm_list_t *list)
{
    return list->size;
}
