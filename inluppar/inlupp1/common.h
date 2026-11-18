#pragma once
#include <stdbool.h>
#include <stddef.h> //Severity 2: using size_t without including
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

/// @brief Hash a string
/// @param key an elem_t whose .s is a null terminated string
/// @return a number refrencing the hash of the string
size_t string_knr_hash(elem_t key);

/// @brief check equality of two stirngs
/// @param a an elem_t whose .s is a null terminated string
/// @param b an elem_t whose .s is a null terminated string
/// @return true if the strings are equal, false otherwise
bool string_eq(elem_t a, elem_t b);

/// @brief Hash an integer
/// @param key an elem_t whose .i member holds an integer
/// @return the integer converted to size_t
size_t int_hash(elem_t key);

/// @brief Check equality of two integers
/// @param a an elem_t whose .i member holds an integer
/// @param b an elem_t whose .i member holds an integer
/// @return true if the integers are equal, false otherwise
bool int_eq(elem_t a, elem_t b);

//Severity 1 = fix indentation on definitions
#define int_elem(x) ((elem_t){.i = (x)})
#define bool_elem(x) ((elem_t){.b = (x)})
#define ptr_elem(x) ((elem_t){.p = (x)})
#define string_elem(x) ((elem_t){.s = (x)})

//Severity 3: Change implementation to not use a -1 that is also a valid value. 
// For example, using an size_t as out-parameter instead of int.
#define ELEM_ERROR int_elem(-1);