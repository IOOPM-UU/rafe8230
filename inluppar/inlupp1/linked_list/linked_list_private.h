#pragma once
#include "linked_list.h"
#include "common.h"

/**
 * @file linked_list_private.h //Severity 1: Wrong file name
 * @author Rasmus Ferngren
 * @date 17 sep 2026
 * @brief A linked list struct library
 *
 * Each entry is a list containing a value and a pointer to the next index of said list
 */

struct list_entry
{
    elem_t value;
    ioopm_list_entry_t *next;
};

struct list
{
    ioopm_list_entry_t sentinel;
    ioopm_list_entry_t *last;
    size_t size;
};

//Severity 1: Moved static declaration to c-file


//Severity 2: iterator_insert and iterator_remove updates last and size in the
// same way as insert and remove. Create helper functions for this repeated code.

/// @brief Creates a new entry and links it into the list directly after previous.
///         Updates the list's size and its last pointer if the new entry is the last one
/// @param list the list operated upon
/// @param previous the entry before the new entry is inserted
/// @param value the value to store in the new entry
void list_link_after(ioopm_list_t *list, ioopm_list_entry_t *previous, elem_t value);


/// @brief Unlink the entry directly after previous from the list and free it.
///         Updates the list's size and its last pointer if the removed entry was the last one.
/// @param list the list operated upon
/// @param previous the entry before the entry to remove
/// @return the value of the removed entry
elem_t list_unlink_after(ioopm_list_t *list, ioopm_list_entry_t *previous);