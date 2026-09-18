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