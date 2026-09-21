#pragma once
#include <stdbool.h>


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

    #define int_elem(x)   ((elem_t) { .i = (x) })
    #define bool_elem(x)  ((elem_t) { .b = (x) })
    #define ptr_elem(x)   ((elem_t) { .p = (x) })
    #define string_elem(x) ((elem_t) { .s = (x) })


typedef bool ioopm_eq_function(elem_t a, elem_t b);
typedef size_t ioopm_hash_function(elem_t key);

typedef bool ioopm_eq_function(elem_t a, elem_t b);
typedef size_t ioopm_hash_function(elem_t key);

size_t string_knr_hash(elem_t key);
bool string_eq(elem_t a, elem_t b);
size_t int_hash(elem_t key);
bool int_eq(elem_t a, elem_t b);