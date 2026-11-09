#include "linked_list_iterator_i2.h"
#include <stdlib.h>
#include <assert.h>
#include "linked_list_private_i2.h"
#include <stddef.h>

struct list_iterator {
    ioopm_link_t *current;
    ioopm_list_t *list;
    ioopm_link_t *previous; // ADDED :
};

ioopm_list_iterator_t *ioopm_list_iterator_create(ioopm_list_t *l) {
    
    assert(l != NULL);

    ioopm_list_iterator_t *iter = calloc(1, sizeof(ioopm_list_iterator_t));

    iter->list = l;
    iter->previous = l->first; // ADDED : 
    iter->current = l->first->next;

    return iter;
}

void ioopm_list_iterator_destroy(ioopm_list_iterator_t *iter) {
    free(iter);
}

bool ioopm_list_iterator_at_end(ioopm_list_iterator_t *iter) {
    assert(iter != NULL);

    if (iter->current == NULL) {
        return true;
    }
    else {
        return false;
    }
}

void ioopm_list_iterator_advance(ioopm_list_iterator_t *iter) {
    assert(iter != NULL);

    if (iter->current != NULL) {
        iter->previous = iter->current; 
        iter->current = iter->current->next;
    }
}

elem_t ioopm_list_iterator_current(ioopm_list_iterator_t *iter) {
    assert(iter != NULL);
    
    return iter->current != NULL ? iter->current->element : int_elem(0);
}


elem_t ioopm_list_iterator_remove(ioopm_list_iterator_t *it)
{
    // return -1 if we are at null list
    if (ioopm_list_iterator_at_end(it))
    {
        return int_elem(-1); 
    }

    // initialize value and current
    ioopm_link_t *current = it->current;
    elem_t value = current->element; 

    // set previous entry to point at what current points at
    it->previous->next = current->next;

    // check if current was last entry, if so update last in list
    if (it->current == it->list->last)
    {
        it->list->last = it->previous; 
    }

    it->list->size--; 
    it->current = it->current->next; 
    free(current);
    return value; 
}

void ioopm_list_iterator_insert(ioopm_list_iterator_t *it, elem_t element)
{
    // Create an entry
    ioopm_link_t *entry = entry_create(element); 

    // make entrys next point to what previous points to
    entry->next = it->previous->next; 

    // make previous point to entry
    it->previous->next = entry; 

    // Check if we are inserting the last element
    if (entry->next == NULL)
    {
        it->list->last = entry; 
    }

    it->current = entry; 
    it->list->size++; 
}