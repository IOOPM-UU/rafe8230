#include <stdlib.h>
#include "linked_list_i2.h"
#include <assert.h>
#include "linked_list_private_i2.h"
#include "common_i2.h"


typedef struct link ioopm_link_t;

ioopm_list_t *ioopm_list_create(void) {
    
    ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t));

    if (list != NULL) {
        
        ioopm_link_t *dummy = calloc(1, sizeof(ioopm_link_t));
        
        list->first = dummy;
        list->last  = dummy; // Tåget slutar vid dummyn
        list->size  = 0;
    }

    return list;
}

ioopm_link_t *entry_create(elem_t value) // ADDED : might cause memleak
{
    ioopm_link_t *entry = calloc(1, sizeof(ioopm_link_t));
    entry->element = value; 
    entry->next = NULL;
    return entry; 
}

void ioopm_list_destroy(ioopm_list_t *list) {
    ioopm_link_t *current = list->first;
    
    while (current != NULL) {
        ioopm_link_t *next = current->next;
        free(current);
        current = next;
    }
    
    free(list);
}

size_t ioopm_list_size(ioopm_list_t *list) {
    return list->size;
}

void ioopm_list_append(ioopm_list_t *list, elem_t value) {
    ioopm_link_t *new_link = calloc(1, sizeof(ioopm_link_t));

    if (new_link != NULL) {
        new_link->element = value;

        list->last->next = new_link;

        list->last = new_link;
        list->size++;
    }
}

void ioopm_list_prepend(ioopm_list_t *list, elem_t value) {
    ioopm_link_t *new_link = calloc(1, sizeof(ioopm_link_t));
    
    if (new_link != NULL) {
        new_link->element = value;

        new_link->next = list->first->next;

        list->first->next = new_link;

        if (list->size == 0) {
            list->last = new_link;
        }

        list->size++;
    }
}

elem_t ioopm_list_head(ioopm_list_t *list) {
    
    assert(list->size > 0); 
    
    elem_t head = list->first->next->element;
    return head;
}

elem_t ioopm_list_last(ioopm_list_t *list) {
    elem_t last = list->last->element;
    return last;
}

void ioopm_list_insert(ioopm_list_t *list, size_t index, elem_t value) {
    
    assert(/*index >= 0 - REMOVED : size_t always > 0 && */ index <= list->size);

    ioopm_link_t *new_link = calloc(1, sizeof(ioopm_link_t));
    new_link->element = value;

    size_t counter = 0; // EDITED : before; int counter = 0;  
    ioopm_link_t * current = list->first;

    while (counter != index) {
        current = current->next;

        counter++;
    }
    
    new_link->next = current->next;
    current->next = new_link;

    if (index == list->size) {
        list->last = new_link;
    }

    list->size++;
}

elem_t ioopm_list_remove(ioopm_list_t *list, size_t index) {

    assert(/* index >= 0 && - REMOVED : index always > 0 */ index < list->size);

    size_t counter = 0; // EDITED : before; int counter = 0;
    ioopm_link_t *current = list->first;

    while (counter != index) {
        current = current->next;

        counter++;
    }
    ioopm_link_t *removed_link = current->next;

    elem_t removed_value = removed_link->element;

    current->next = current->next->next;

    if (index == list->size - 1) {
        list->last = current;
    }

    free(removed_link);

    list->size--;

    return removed_value;
}

elem_t ioopm_list_get(ioopm_list_t *list, size_t index) {

    assert(/* index >= 0 && - REMOVED : size_t always > 0*/ index < list->size);

    size_t counter = 0; //    int counter = 0;

    ioopm_link_t *current = list->first;

    while (counter != index) {
        current = current->next;

        counter++;
    }

    return current->next->element;

}

bool ioopm_list_is_empty(ioopm_list_t *list) {

    if (ioopm_list_size(list) == 0) {
        return true;
    }
    else
    {
        return false;
    }
}