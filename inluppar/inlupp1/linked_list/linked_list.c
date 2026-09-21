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
    // Set current to what the sentinel node points to
    list_entry_t *entry = list->sentinel.next;

    // Check if the current entry is empty
    while (entry != NULL)
    {
        // update current to next and free current
        list_entry_t *next = entry->next;
        free(entry); 
        entry = next; 
    }
    free(list); 
}
void ioopm_list_append(ioopm_list_t *list, elem_t value)
{
    // Create the entry to append
    list_entry_t *entry = entry_create(value);
    // let the last element of the list point to entry
    list->last->next = entry;
    list->size++; 
    // Set the entry to be the last of the list
    list->last = entry;
}

void ioopm_list_prepend(ioopm_list_t *list, elem_t value)
{
    // Initialize the new entry
    list_entry_t *entry = entry_create(value); 
    // Set the next of our entry to what is after sentinel
    entry->next = list->sentinel.next;
    // make sentinels next node be our entry
    list->sentinel.next = entry; 

    // Check if list is empty before prepend
    // If so, make our entry the last item of the list
    if (list->size == 0)
    {
        list->last = entry; 
    }

    list->size++;  
}

elem_t ioopm_list_head(const ioopm_list_t *list)
{
    // Retrun -1 if the list is empty
    if (list->sentinel.next == NULL)
    {
        return int_elem(-1);
    }
    
    return list->sentinel.next->value; 
    
}

elem_t ioopm_list_last(const ioopm_list_t *list)
{
    // Return -1 if the list is empty
    if (list->sentinel.next == NULL)
    {
        return int_elem(-1);
    }
    return list->last->value; 
    
}

static list_entry_t *find_previous_entry(ioopm_list_t *list, const size_t index)
{
    list_entry_t *previous = &list->sentinel;

    // Loop over a linked list until weve reached our index
    for (size_t i = 0; i < index; i++)
    {
        previous = previous->next;
    }
    return previous;
}

void ioopm_list_insert(ioopm_list_t *list, size_t index, elem_t value)
{
    // Initialize the entry and previous to entry
    list_entry_t *entry = entry_create(value); 
    list_entry_t *previous = find_previous_entry(list, index); 

    // Make entrys next element previous next element
    entry->next = previous->next;
    // Make previous next element our entry
    previous->next = entry;

    // Look if its the last element of our list
    // If so, update last to our entry
    if (index == list->size)
    {
        list->last = entry;
    }
    
    list->size++; 
    return;
}

elem_t ioopm_list_remove(ioopm_list_t *list, size_t index)
{
    // If the list is empty return -1
    if (ioopm_list_is_empty(list))
    {
        return int_elem(-1); 
    }
    
    // initialize previous and current
    list_entry_t *previous = find_previous_entry(list, index); 
    list_entry_t *current = previous->next;   

    
    // Make previous next element the element current has next
    previous->next = current->next; 
    
    // check if current was the last element in the linked list
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

    // Check if the list is empty, if so return -1
    if (list->size == 0 || index > list->size)
    {
        return int_elem(-1); 
    }
    
    // Iterate through the list until we get to our desired element
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
