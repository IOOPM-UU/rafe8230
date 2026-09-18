#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef struct list ioopm_list_t; /// Meta: struct definition goes in C file

typedef struct list_entry list_entry_t; 

/// @brief Creates a new empty list
/// @return an empty linked list
ioopm_list_t *ioopm_list_create(void);

/// @brief Tear down the linked list and return all its memory (but not the memory of the elements)
/// @param list the list to be destroyed
void ioopm_list_destroy(ioopm_list_t *list);

/// @brief Insert at the end of a linked list in O(1) time
/// @param list the linked list that will be appended
/// @param value the value to be appended
void ioopm_list_append(ioopm_list_t *list, int value);

/// @brief Insert at the front of a linked list in O(1) time
/// @param list the linked list that will be prepended to
/// @param value the value to be prepended
void ioopm_list_prepend(ioopm_list_t *list, int value);

/// @brief Return the first element of a linked list in O(1) time
/// @pre the list is non-empty
/// @param list the linked list to take the head of
int ioopm_list_head(const ioopm_list_t *list);

/// @brief Return the last element of a linked list in O(1) time
/// @pre the list is non-empty
/// @param list the linked list to take the last element of
int ioopm_list_last(const ioopm_list_t *list);

/// @brief Insert an element into a linked list in O(n) time.
/// The valid values of index are [0,n] for a list of n elements,
/// where 0 means before the first element and n means after
/// the last element.
/// @pre 0 <= index <= length(list)
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @param value the value to be inserted
void ioopm_list_insert(ioopm_list_t *list, size_t index, int value);

/// @brief Remove an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @pre 0 <= index < length(list)
/// @param list the linked list
/// @param index the position in the list
/// @return the value removed
int ioopm_list_remove(ioopm_list_t *list, size_t index);

/// @brief Retrieve an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @pre 0 <= index < length(list)
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @return the value at the given position
int ioopm_list_get(const ioopm_list_t *list, const size_t index);

/// @brief Lookup the number of elements in the linked list in O(1) time
/// @param list the linked list
/// @return the number of elements in the list
size_t ioopm_list_size(const ioopm_list_t *list);

/// @brief Test whether a list is empty or not
/// @param list the linked list
/// @return true if the number of elements int the list is 0, else false
bool ioopm_list_is_empty(const ioopm_list_t *list);