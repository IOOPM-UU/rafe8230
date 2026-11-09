#pragma once
#include <stdbool.h>
#include <stddef.h>

/**
 * @file common_i2.h
 * @author Rasmus Ferngren
 * @date 19 sep 2026
 * @brief Shared element type and hash/equality functions for the generic
 *        hash table and linked list.
 *
 * elem_t lets the data structures store any kind of key or value. The caller
 * decides which field is in use and must read the same field it wrote.
 */

typedef union elem elem_t;

/// A generic element: an integer, a size, a bool, a float, a pointer or a string.
union elem
{
    int i;          ///< signed integer
    unsigned int u; ///< unsigned integer
    size_t z;       ///< size or count
    bool b;         ///< boolean
    float f;        ///< decimal number
    void *p;        ///< any pointer
    char *s;        ///< string
};

#define int_elem(x)      ((elem_t) { .i = (x) })
#define unsigned_elem(x) ((elem_t) { .u = (x) })
#define size_elem(x)     ((elem_t) { .z = (x) })
#define bool_elem(x)     ((elem_t) { .b = (x) })
#define float_elem(x)    ((elem_t) { .f = (x) })
#define ptr_elem(x)      ((elem_t) { .p = (x) })
#define string_elem(x)   ((elem_t) { .s = (x) })

/// @brief Hash function for string keys (K&R).
/// @param key an element whose s field is a string
/// @return the hash of the string
size_t string_knr_hash(elem_t key);

/// @brief Equality function for string keys.
/// @return true if both strings have the same content (strcmp)
bool string_eq(elem_t a, elem_t b);

/// @brief Hash function for int keys.
/// @param key an element whose i field is used
/// @return the hash of the integer
size_t int_hash(elem_t key);

/// @brief Equality function for int keys.
/// @return true if both i fields are equal
bool int_eq(elem_t a, elem_t b);