#pragma once
#include "linked_list.h"
#include "common.h"

/**
* @file linked_list.h
* @author Rasmus Ferngren
* @date 17 sep 2026
* @brief A linked list struct library
*
* Each entry is a list containing a value and a pointer to the next index of said list
*/



struct list_entry
{
    elem_t value; 
    list_entry_t *next;
};

struct list 
{
    list_entry_t sentinel;
    list_entry_t *last;
    size_t size;
};


/// @brief Create a list entry
/// @param value a value of type elem_t to connect with the entry
/// @return an initialized list entry
list_entry_t *entry_create(elem_t value);