#include <CUnit/Basic.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "db.h"
#include "linked_list_iterator_i2.h"

/*
 * ioopm_db_add_item takes ownership of name and desc when it returns DB_OK.
 * When it returns anything else, the caller still owns them and frees them.
 * That is why every string passed to it is strdup'd.
 * 
 * Majority of tests were create by ai but have been checked (i dont trust ai)
 */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

// helpers

// The merch stored under name, or NULL if there is none
static merch_t *lookup_merch(ioopm_data_base_t *db, char *name)
{
    elem_t result = int_elem(0);
    if (ioopm_hash_table_lookup(db->ht_s, string_elem(name), &result))
    {
        return result.p;
    }
    return NULL;
}

// Adds a merch, copying the strings so the db can take ownership
static ioopm_db_result_t add(ioopm_data_base_t *db, char *name, char *desc, int price)
{
    char *n = strdup(name);
    char *d = strdup(desc);
    ioopm_db_result_t res = ioopm_db_add_item(db, n, d, price);
    if (res != DB_OK)
    {
        // rejected: the db did not take the strings
        free(n);
        free(d);
    }
    return res;
}

// Checks every field of a freshly added merch
static void assert_new_merch(merch_t *m, char *name, char *desc, int price)
{
    CU_ASSERT_PTR_NOT_NULL_FATAL(m);
    CU_ASSERT_STRING_EQUAL(ioopm_db_item_name(m).s, name);
    CU_ASSERT_STRING_EQUAL(ioopm_db_item_desc(m).s, desc);
    CU_ASSERT_EQUAL(ioopm_db_item_price(m).i, price);
    CU_ASSERT_EQUAL(ioopm_db_item_stock(m), 0);   // a new merch is not in stock

    ioopm_list_t *shelves = ioopm_db_item_all_shelves(m);
    CU_ASSERT_EQUAL(ioopm_list_size(shelves), 0); // and is on no shelf
    ioopm_list_destroy(shelves);
}

// create / destroy

void db_create_destroy(void)
{
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    CU_ASSERT_PTR_NOT_NULL(db);
    ioopm_db_destroy(db);
}

void db_create_is_empty(void)
{
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    CU_ASSERT_PTR_NOT_NULL_FATAL(db);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_s));
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_n));
    ioopm_db_destroy(db);
}

// add

void db_add_one(void)
{
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);

    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 1);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_n));   // no shelves used
    assert_new_merch(lookup_merch(db, "Apple"), "Apple", "Red fruit", 5);

    ioopm_db_destroy(db);
}

void db_add_three(void)
{
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);

    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Orange", "Orange fruit", 7), DB_OK);

    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 3);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_n));
    assert_new_merch(lookup_merch(db, "Apple"), "Apple", "Red fruit", 5);
    assert_new_merch(lookup_merch(db, "Pear"), "Pear", "Green fruit", 6);
    assert_new_merch(lookup_merch(db, "Orange"), "Orange", "Orange fruit", 7);

    ioopm_db_destroy(db);
}

void db_add_same_name(void)
{
    // Adding a merch with the same name as an existing merch is not allowed
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);

    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Apple", "Red and Green fruit", 10), DB_NAME_TAKEN);

    // the original is untouched
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 1);
    assert_new_merch(lookup_merch(db, "Apple"), "Apple", "Red fruit", 5);

    ioopm_db_destroy(db);
}

void db_add_same_name_other_case(void)
{
    // Names are case sensitive: "apple" is a different merch than "Apple"
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);

    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "apple", "Small red fruit", 4), DB_OK);

    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 2);
    assert_new_merch(lookup_merch(db, "apple"), "apple", "Small red fruit", 4);

    ioopm_db_destroy(db);
}

void db_add_lookup_other_buffer(void)
{
    // Lookup must compare the text, not the pointer
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    char key[] = "Apple";   // a different buffer with the same text
    CU_ASSERT_PTR_NOT_NULL(lookup_merch(db, key));

    ioopm_db_destroy(db);
}

void db_add_lookup_missing(void)
{
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    CU_ASSERT_PTR_NULL(lookup_merch(db, "Apple"));

    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_PTR_NULL(lookup_merch(db, "Pear"));

    ioopm_db_destroy(db);
}

void db_add_many(void)
{
    // Enough items to make the hash table grow; all must still be found
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    char name[16];

    for (int i = 0; i < 50; ++i)
    {
        snprintf(name, sizeof(name), "Item%d", i);
        CU_ASSERT_EQUAL(add(db, name, "Some item", i), DB_OK);
    }
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 50);

    for (int i = 0; i < 50; ++i)
    {
        snprintf(name, sizeof(name), "Item%d", i);
        merch_t *m = lookup_merch(db, name);
        CU_ASSERT_PTR_NOT_NULL_FATAL(m);
        CU_ASSERT_EQUAL(ioopm_db_item_price(m).i, i);
    }

    ioopm_db_destroy(db);
}

// merch names

// How many times name occurs among the first size entries of names
static size_t count_name(char **names, size_t size, char *name)
{
    size_t count = 0;
    for (size_t i = 0; i < size; ++i)
    {
        if (strcmp(names[i], name) == 0)
        {
            count++;
        }
    }
    return count;
}

void db_names_empty(void)
{
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);

    size_t size = 123;   // garbage on purpose: the function must overwrite it
    char **names = ioopm_db_merch_names(db, &size);
    CU_ASSERT_EQUAL(size, 0);
    CU_ASSERT_PTR_NULL(names);

    ioopm_db_destroy(db);
}

void db_names_one(void)
{
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    size_t size = 0;
    char **names = ioopm_db_merch_names(db, &size);
    CU_ASSERT_EQUAL_FATAL(size, 1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(names);
    CU_ASSERT_STRING_EQUAL(names[0], "Apple");

    free(names);  
    ioopm_db_destroy(db);
}

void db_names_three(void)
{
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Banana", "Yellow fruit", 8), DB_OK);

    size_t size = 0;
    char **names = ioopm_db_merch_names(db, &size);
    CU_ASSERT_EQUAL_FATAL(size, 3);
    CU_ASSERT_PTR_NOT_NULL_FATAL(names);

    // every name exactly once, in any order
    CU_ASSERT_EQUAL(count_name(names, size, "Apple"), 1);
    CU_ASSERT_EQUAL(count_name(names, size, "Banana"), 1);
    CU_ASSERT_EQUAL(count_name(names, size, "Pear"), 1);

    free(names);
    ioopm_db_destroy(db);
}

void db_names_are_not_copies(void)
{
    // The array points to the merch's own strings, so the caller must not free them
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    size_t size = 0;
    char **names = ioopm_db_merch_names(db, &size);
    CU_ASSERT_EQUAL_FATAL(size, 1);
    CU_ASSERT_PTR_EQUAL(names[0], ioopm_db_item_name(lookup_merch(db, "Apple")).s);

    free(names);
    ioopm_db_destroy(db);
}

void db_names_after_rejected_add(void)
{
    // A rejected duplicate must not show up in the list
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Apple", "Other fruit", 9), DB_NAME_TAKEN);

    size_t size = 0;
    char **names = ioopm_db_merch_names(db, &size);
    CU_ASSERT_EQUAL_FATAL(size, 1);
    CU_ASSERT_EQUAL(count_name(names, size, "Apple"), 1);

    free(names);
    ioopm_db_destroy(db);
}

void db_names_many(void)
{
    // More than 20 items (one listing page) and enough to make the table grow
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    char name[16];
    for (int i = 0; i < 50; ++i)
    {
        snprintf(name, sizeof(name), "Item%d", i);
        CU_ASSERT_EQUAL(add(db, name, "Some item", i), DB_OK);
    }

    size_t size = 0;
    char **names = ioopm_db_merch_names(db, &size);
    CU_ASSERT_EQUAL_FATAL(size, 50);
    CU_ASSERT_PTR_NOT_NULL_FATAL(names);

    for (int i = 0; i < 50; ++i)
    {
        snprintf(name, sizeof(name), "Item%d", i);
        CU_ASSERT_EQUAL(count_name(names, size, name), 1);
    }

    free(names);
    ioopm_db_destroy(db);
}

// main

int main(void)
{
    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        return CU_get_error();
    }

    CU_pSuite my_test_suite = CU_add_suite("data base test suite:", init_suite, clean_suite);
    if (my_test_suite == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
        (CU_add_test(my_test_suite, "create and destroy", db_create_destroy) == NULL) ||
        (CU_add_test(my_test_suite, "new db is empty", db_create_is_empty) == NULL) ||

        (CU_add_test(my_test_suite, "add: one", db_add_one) == NULL) ||
        (CU_add_test(my_test_suite, "add: three", db_add_three) == NULL) ||
        (CU_add_test(my_test_suite, "add: same name is rejected", db_add_same_name) == NULL) ||
        (CU_add_test(my_test_suite, "add: names are case sensitive", db_add_same_name_other_case) == NULL) ||
        (CU_add_test(my_test_suite, "add: lookup with other buffer", db_add_lookup_other_buffer) == NULL) ||
        (CU_add_test(my_test_suite, "add: lookup missing", db_add_lookup_missing) == NULL) ||
        (CU_add_test(my_test_suite, "add: many items", db_add_many) == NULL) ||

        (CU_add_test(my_test_suite, "names: empty db", db_names_empty) == NULL) ||
        (CU_add_test(my_test_suite, "names: one", db_names_one) == NULL) ||
        (CU_add_test(my_test_suite, "names: three", db_names_three) == NULL) ||
        (CU_add_test(my_test_suite, "names: not copies", db_names_are_not_copies) == NULL) ||
        (CU_add_test(my_test_suite, "names: after rejected add", db_names_after_rejected_add) == NULL) ||
        (CU_add_test(my_test_suite, "names: many", db_names_many) == NULL) ||
        0)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}