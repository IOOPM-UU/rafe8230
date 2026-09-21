#include <string.h>
#include "common.h"

size_t string_knr_hash(elem_t key)
{
  const char *str = key.s;
  size_t result = 0;
  while (*str != '\0')
  {
    result = result * 31 + (unsigned char) *str;
    str++;
  }
  return result;
}

bool string_eq(elem_t a, elem_t b)
{
  return strcmp(a.s, b.s) == 0;
}

size_t int_hash(elem_t key)
{
  return (size_t) key.i;
}

bool int_eq(elem_t a, elem_t b)
{
  return a.i == b.i;
}