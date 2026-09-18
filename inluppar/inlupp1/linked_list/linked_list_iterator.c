#include "linked_list.h"
#include "linked_list_iterator.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "linked_list_private.h"
#include <assert.h>
#include "common.h"

ioopm_list_iterator_t *ioopm_list_iterator_create(ioopm_list_t *l)
{
    ioopm_list_iterator_t *it = calloc(1, sizeof(ioopm_list_iterator_t));
    it->index = 0;
    it->list = l;
    it->current_entry = it->list->sentinel.next;
    it->previous_entry = &it->list->sentinel; 

    return it; 
}

void ioopm_list_iterator_destroy(ioopm_list_iterator_t *it)
{
    free(it);
}

bool ioopm_list_iterator_at_end(const ioopm_list_iterator_t *it)
{
    return it->index == it->list->size;
}

void ioopm_list_iterator_advance(ioopm_list_iterator_t *it)
{
    assert(!ioopm_list_iterator_at_end(it));
    it->previous_entry = it->current_entry; 
    it->current_entry = it->current_entry->next;
    it->index++; 
}

elem_t ioopm_list_iterator_current(ioopm_list_iterator_t *it)
{
    if (it->current_entry == NULL)
    {
        return int_elem(-1); 
    }
    
    return ioopm_list_get(it->list, it->index);
}

elem_t ioopm_list_iterator_remove(ioopm_list_iterator_t *it)
{
    if (ioopm_list_iterator_at_end(it))
    {
        return int_elem(-1); 
    }

    list_entry_t *current = it->current_entry;
    elem_t value = current->value; 

    it->previous_entry->next = current->next;

    if (it->current_entry == it->list->last)
    {
        it->list->last = it->previous_entry; 
    }

    it->list->size--; 
    it->current_entry = it->current_entry->next; 
    free(current);
    return value; 
}

void ioopm_list_iterator_insert(ioopm_list_iterator_t *it, elem_t element)
{
    list_entry_t *entry = entry_create(element); 

    entry->next = it->previous_entry->next; 
    it->previous_entry->next = entry; 

    if (entry->next == NULL)
    {
        it->list->last = entry; 
    }

    it->current_entry = entry; 
    it->list->size++; 
}