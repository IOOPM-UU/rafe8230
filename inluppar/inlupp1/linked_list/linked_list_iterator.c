#include "linked_list.h"
#include "linked_list_iterator.h"
#include <stdbool.h>
#include <stdlib.h>
#include "linked_list_private.h"
#include <assert.h>
#include "common.h"
//Severity 1: including stdio is not needed

//Severity 2: List iterator should be in its c-file, not in header.
 struct list_iterator
{
  ioopm_list_t *list;
  size_t index;
  ioopm_list_entry_t *current_entry;
  ioopm_list_entry_t *previous_entry;
};

ioopm_list_iterator_t *ioopm_list_iterator_create(ioopm_list_t *l)
{
    //Severity 1: redundant zeroing of fields because of calloc
    ioopm_list_iterator_t *it = calloc(1, sizeof(ioopm_list_iterator_t));
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
    // If the list is at the end and we try to advance
    // Crash the program
    assert(!ioopm_list_iterator_at_end(it));
    it->previous_entry = it->current_entry;
    it->current_entry = it->current_entry->next;
    it->index++;
}

elem_t ioopm_list_iterator_current(ioopm_list_iterator_t *it)
{
    // if the iterator is at a null list, return -1
    if (it->current_entry == NULL)
    {
        return int_elem(-1);
    }

    return ioopm_list_get(it->list, it->index);
}

//Uses the new helper function
elem_t ioopm_list_iterator_remove(ioopm_list_iterator_t *it)
{
    assert(it != NULL);
    if (ioopm_list_iterator_at_end(it)) 
    {
        return ELEM_ERROR;
    }

    elem_t value = list_unlink_after(it->list, it->previous_entry);
    it->current_entry = it->previous_entry->next;
    return value;
}

//Uses the new helper function
void ioopm_list_iterator_insert(ioopm_list_iterator_t *it, elem_t element)
{
    assert(it != NULL);
    list_link_after(it->list, it->previous_entry, element);
    it->current_entry = it->previous_entry->next;
}