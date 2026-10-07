#pragma once
#include <stdbool.h>

/**
* @file common.h
* @author Rasmus Ferngren
* @date 19 sep 2026
* @brief Common file for linked list and hash table implementation
*
* Defined union elem_t and its respective types int, bool, void ptr and string.
*/

typedef union elem elem_t;

union elem
{
  int i;
  unsigned int u;
  bool b;
  float f;
  void *p;
  char *s;
};

size_t string_knr_hash(elem_t key);
bool string_eq(elem_t a, elem_t b);
size_t int_hash(elem_t key);
bool int_eq(elem_t a, elem_t b);

    #define int_elem(x)   ((elem_t) { .i = (x) })
    #define bool_elem(x)  ((elem_t) { .b = (x) })
    #define ptr_elem(x)   ((elem_t) { .p = (x) })
    #define string_elem(x) ((elem_t) { .s = (x) })
