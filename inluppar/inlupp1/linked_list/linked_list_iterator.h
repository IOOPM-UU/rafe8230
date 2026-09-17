#pragma once
#include <stdbool.h>

#include "linked_list.h"

typedef struct list_iterator ioopm_list_iterator_t;

struct list_iterator
{
  ioopm_list_t *list;
  int index;
  list_entry_t *current_entry;
  list_entry_t *previous_entry;
};

/// @brief Create a new iterator
/// @param l the list to iterate over
ioopm_list_iterator_t *ioopm_list_iterator_create(ioopm_list_t *l);

/// @brief Destroy the iterator and return its resources
/// @param it the iterator
void ioopm_list_iterator_destroy(ioopm_list_iterator_t *it);

/// @brief Checks if there are more elements to iterate over
/// @param it the iterator
/// @return true if there is at least one more element
bool ioopm_list_iterator_at_end(ioopm_list_iterator_t *it);

/// @brief Step the iterator forward one step
/// @param it the iterator
void ioopm_list_iterator_advance(ioopm_list_iterator_t *it);

/// @brief Return the current element from the underlying list
/// @param it the iterator
/// @return the current element
int ioopm_list_iterator_current(ioopm_list_iterator_t *it);

/// NOTE: REMOVE IS OPTIONAL TO IMPLEMENT
/// @brief Remove the current element from the underlying list
/// @param it the iterator
/// @return the removed element
int ioopm_list_iterator_remove(ioopm_list_iterator_t *it);

/// NOTE: INSERT IS OPTIONAL TO IMPLEMENT
/// @brief Insert a new element into the underlying list making the current element it's next
/// @param it the iterator
/// @param element the element to be inserted
void ioopm_list_iterator_insert(ioopm_list_iterator_t *it, int element);