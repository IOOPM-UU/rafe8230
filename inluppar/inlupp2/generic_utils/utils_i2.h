#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <db.h>

/**
 * @file utils.h
 * @brief Input helpers for reading and validating user input from stdin,
 *        plus some sorting utilities.
 */

/// A value produced by a convert function. Which field is valid depends
/// on which ask_question_* function produced it.
typedef union {
    int int_value;        ///< Valid after ask_question_int
    float float_value;    ///< Valid after ask_question_float
    char *string_value;   ///< Valid after ask_question_string (heap-allocated)
    char *shelf;          ///< Valid after ask_question_shelf (heap-allocated)
    char chr;             ///< Valid after ask_question_char
    size_t u;             ///< Valid after ask_question_size_u
} answer_t;

/// A predicate that decides whether a string has a valid format.
/// @param arr the string to check
/// @return true if arr has the expected format, otherwise false
typedef bool control_format(char *arr);

/// A function that converts a string (already validated by a
/// control_format) into an answer_t.
/// @param arr the string to convert
/// @return the converted value
typedef answer_t convert(char *arr);

extern char *strdup(const char *);


/// @brief Discards the remaining characters on stdin up to and including
///        the next newline (or until EOF).
void clear_keyboard_buffer(void);

/// @brief Reads one line from stdin into buf.
///
/// Reads at most buf_siz - 1 characters. The newline is not stored and
/// buf is always null-terminated. If the line is longer than the buffer,
/// the rest of the line is discarded.
/// @param buf buffer to read into
/// @param buf_siz size of buf in bytes (must be > 0)
/// @return the number of characters stored in buf (excluding '\0')
int read_string(char *buf, int buf_siz);

/// @brief Checks whether str is a (possibly negative) integer.
/// @param str the string to check
/// @return true if str is non-empty, optionally starts with '-', and
///         otherwise consists only of digits; false otherwise
bool is_number(char *str);

/// @brief Checks whether c is a decimal digit ('0'–'9').
/// @param c the character to check
/// @return true if c is a digit, otherwise false
bool is_digit(char c);

/// @brief Checks whether str is a decimal number, e.g. "3.14" or "-2".
/// @param str the string to check
/// @return true if str is an optional '-', followed by digits with at most
///         one '.', and contains at least one digit; false otherwise
bool is_float(char *str);

/// @brief Converts a string to a float stored in an answer_t.
/// @param str a string that satisfies is_float
/// @return an answer_t whose float_value is the parsed number
answer_t make_float(char *str);

/// @brief Checks that a string is not empty. Helper for ask_question_string.
/// @param str the string to check
/// @return true if str contains at least one character, otherwise false
bool not_empty(char *str);

/// @brief Repeatedly asks a question until the user gives a valid answer.
///
/// Prints question, reads a line from stdin, and re-asks as long as
/// check returns false for the input. The valid input is then passed to
/// convert.
/// @param question the question to print
/// @param check predicate that decides if the input is valid
/// @param convert function that turns valid input into an answer_t
/// @return the converted answer
answer_t ask_question(char *question, control_format *check, convert *convert);

/// @brief Asks a question until the user enters a non-empty string.
/// @param question the question to print
/// @return a heap-allocated copy of the answer; the caller must free it
char *ask_question_string(char *question);

/// @brief Asks a question until the user enters a valid integer.
/// @param question the question to print
/// @return the integer entered
int ask_question_int(char *question);

/// @brief Asks a question until the user enters a valid shelf name
///        (one letter followed by one or more digits, e.g. "A25").
/// @param question the question to print
/// @return a heap-allocated copy of the shelf name; the caller must free it
char *ask_question_shelf(char *question);

/// @brief Asks a question until the user enters a valid decimal number.
/// @param question the question to print
/// @return the number entered
double ask_question_float(char *question);

/// @brief Checks whether str consists of exactly one character.
/// @param str the string to check
/// @return true if strlen(str) == 1, otherwise false
bool is_valid_char(char *str);

/// @brief Converts a one-character string to an answer_t.
/// @param str a string that satisfies is_valid_char
/// @return an answer_t whose chr is str[0]
answer_t make_char(char *str);

/// @brief Asks a question until the user enters exactly one character.
/// @param question the question to print
/// @return the character entered
char ask_question_char(char *question);

/// @brief Checks whether str is a non-negative integer (digits only).
/// @param str the string to check
/// @return true if str is non-empty and consists only of digits,
///         otherwise false
bool is_size_u(char *str);

/// @brief Asks a question until the user enters a non-negative integer.
/// @param question the question to print
/// @return the number entered
int ask_question_size_u(char *question);

/// @brief Copies characters from string into buf, stopping at the first
///        occurrence of character or at the end of string.
///
/// At most buf_size - 1 characters are copied and buf is always
/// null-terminated. The stop character itself is not copied.
/// @param buf destination buffer
/// @param buf_size size of buf in bytes (must be > 0)
/// @param string source string
/// @param character the character to stop at
/// @return the number of characters copied into buf (excluding '\0')
int read_string_to_buf(char *buf, const int buf_size, const char *string, const char character);

/// @brief Sorts an array of strings in place in ascending alphabetical
///        (strcmp) order.
/// @param names array of strings to sort
/// @param size number of elements in names
void sort_names_alphabetically(char *names[], size_t size);

/// @brief Sorts an array of shelves in place in ascending alphabetical
///        order by shelf name.
/// @param slf array of pointers to shelves to sort
/// @param size number of elements in slf
void sort_shelves_alphabetically(ioopm_shelf_t **slf, size_t size);