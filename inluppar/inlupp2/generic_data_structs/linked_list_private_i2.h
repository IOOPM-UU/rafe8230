#pragma once
#include "linked_list_i2.h"
#include <stddef.h>
#include "common_i2.h"

typedef struct link ioopm_link_t;

struct link {
    elem_t element;
    struct link *next;
};

struct list {
    ioopm_link_t *first;
    ioopm_link_t *last;
    size_t size;
};