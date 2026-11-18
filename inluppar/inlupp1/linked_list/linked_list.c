#include <stdbool.h>
#include "linked_list.h"
#include <stdlib.h>
#include "linked_list_private.h"
#include "common.h"
#include <assert.h>
//Severity 1: including stdio is not needed

//Severity 1: Moved compilation comments to readme

//Severity 2: No NULL-checks, added asserts where needed

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

//Severity 2: private function should be static
//Severity 1: Static declaration should be in c-file 

/// @brief Create a list entry
/// @param value a value of type elem_t to connect with the entry
/// @return an initialized list entry
static ioopm_list_entry_t *entry_create(elem_t value)
{
    //Severity 1: redundant zeroing of fields because of calloc
    ioopm_list_entry_t *entry = calloc(1, sizeof(ioopm_list_entry_t));
    entry->value = value;
    return entry;
}

//Severity 2: Function is never not used?
static void entries_destroy_tail(ioopm_list_entry_t *entry)
{
    if (entry == NULL)
    {
        return;
    }
    ioopm_list_entry_t *next = entry->next;
    free(entry);
    entries_destroy_tail(next);
}

//Severity 2: recursion gave cause stack overflow on large lists
void ioopm_list_destroy(ioopm_list_t *list)
{
    assert(list != NULL);
    // Set current to what the sentinel node points to
    ioopm_list_entry_t *entry = list->sentinel.next;
    // Check if the current entry is empty
    while (entry != NULL)
    {
        // update current to next and free current
        ioopm_list_entry_t *next = entry->next;
        free(entry);
        entry = next;
    }
}
void ioopm_list_append(ioopm_list_t *list, elem_t value)
{
    assert(list != NULL);
    // Create the entry to append
    ioopm_list_entry_t *entry = entry_create(value);
    // let the last element of the list point to entry
    list->last->next = entry;
    list->size++;
    // Set the entry to be the last of the list
    list->last = entry;
}

void ioopm_list_prepend(ioopm_list_t *list, elem_t value)
{
    assert(list != NULL);
    // Initialize the new entry
    ioopm_list_entry_t *entry = entry_create(value);
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
    assert(list != NULL);
    // Retrun -1 if the list is empty
    if (list->sentinel.next == NULL)
    {
        return ELEM_ERROR;
    }

    return list->sentinel.next->value;
}

elem_t ioopm_list_last(const ioopm_list_t *list)
{
    assert(list != NULL);
    // Return -1 if the list is empty
    if (list->sentinel.next == NULL)
    {
        return ELEM_ERROR;
    }
    return list->last->value;
}

//Severity 2: recursion gave cause stack overflow on large lists
static ioopm_list_entry_t *find_previous_entry(ioopm_list_entry_t *entry, size_t index)
{
    for (size_t i = 0; i < index; i++)
    {
        entry = entry->next;
    }
    return entry;
}

//Severity 2: iterator_insert and iterator_remove updates last and size in the
// same way as insert and remove. Create helper functions for this repeated code.

void list_link_after(ioopm_list_t *list, ioopm_list_entry_t * previous, elem_t value)
{
    assert(list != NULL);
    ioopm_list_entry_t *entry = entry_create(value);
    entry->next = previous->next;
    previous->next = entry;
    if (entry->next == NULL) 
    {
        list->last = entry;
    }
    list->size++;
}

elem_t list_unlink_after(ioopm_list_t *list, ioopm_list_entry_t *previous) 
{
    assert(list != NULL);
    ioopm_list_entry_t *current = previous->next;
    elem_t value = current->value;

    previous->next = current->next;
    if (current == list->last) 
    {
        list->last = previous;
    }
    list->size--;
    free(current);
    return value;
}

void ioopm_list_insert(ioopm_list_t *list, size_t index, elem_t value)
{   
    //Severity 3: Insert need to check valid index <= size (was <). if index == size appends at the end
    assert(list != NULL && index <= list->size);
    list_link_after(list, find_previous_entry(&list->sentinel, index), value);
}

elem_t ioopm_list_remove(ioopm_list_t *list, size_t index)
{
    assert(list != NULL);
    //Severity 3: Remove need to check invalid index >= size (was >). if index == size - 1 we are at end of list
    if (index >= list->size)
    {
        return ELEM_ERROR;
    }
    return list_unlink_after(list, find_previous_entry(&list->sentinel, index));
}


//Severity 3: function will crash if index == size.
elem_t ioopm_list_get(const ioopm_list_t *list, const size_t index)
{
    assert(list != NULL);
    // Check if the list is empty, if so return -1
    if (index >= list->size)
    {
        return ELEM_ERROR;
    }

    ioopm_list_entry_t *current = list->sentinel.next;
    // Iterate through the list until we get to our desired element
    for (size_t i = 0; i < index; i++)
    {
        current = current->next;
    }
    return current->value;
}

size_t ioopm_list_size(const ioopm_list_t *list)
{
    assert(list != NULL);
    return list->size;
}
