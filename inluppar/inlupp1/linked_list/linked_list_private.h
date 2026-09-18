#pragma once
#include "linked_list.h"
#include "common.h"

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

list_entry_t *entry_create(elem_t value);