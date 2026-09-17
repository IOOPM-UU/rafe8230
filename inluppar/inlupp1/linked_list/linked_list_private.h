#pragma once
#include "linked_list.h"

struct list_entry
{
    int value; 
    list_entry_t *next;
};

struct list 
{
    list_entry_t sentinel;
    list_entry_t *last;
    int size;
};

list_entry_t *entry_create(int value);