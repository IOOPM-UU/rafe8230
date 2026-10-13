#include <CUnit/Basic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "db.h"
#include "linked_list_iterator_i2.h"

/*
 * Ownership rules the tests rely on:
 *
 * ioopm_db_add_item takes ownership of name and desc when it returns DB_OK.
 * When it returns anything else, the caller still owns them and frees them.
 * That is why every string passed to it is strdup'd.
 *
 * ioopm_db_replenish never takes ownership: it copies the shelf name when it
 * creates a new shelf, so string literals can be passed directly.
 *
 * Database layout:
 *   name_id_ht : merch name -> id
 *   ht_s       : id         -> merch_t *
 *   ht_n       : shelf name -> id
 *
 * Majority of tests were create by ai but have been checked (i dont trust ai)
 */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

// helpers

// Creates an empty db. The only place that knows the hash functions,
// so a change to ioopm_db_create only needs updating here.
// If your int hash/eq functions have other names, change them here.
static ioopm_data_base_t *new_db(void)
{
    return ioopm_db_create(string_knr_hash, string_eq, int_hash, int_eq);
}

// The merch called name, or NULL if there is none (name -> id -> merch)
static merch_t *lookup_merch(ioopm_data_base_t *db, char *name)
{
    elem_t id;
    if (!ioopm_hash_table_lookup(db->name_id_ht, string_elem(name), &id))
    { 
        return NULL;
    }
    elem_t result;
    if (ioopm_hash_table_lookup(db->ht_s, id, &result))
    {
        return result.p;
    }
    return NULL;
}

// name_id_ht and ht_s must always hold one entry per merch
static void assert_tables_consistent(ioopm_data_base_t *db)
{
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->name_id_ht), ioopm_hash_table_size(db->ht_s));
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
static void assert_new_merch(ioopm_data_base_t *db, char *name, char *desc, int price)
{
    merch_t *m = lookup_merch(db, name);
    CU_ASSERT_PTR_NOT_NULL_FATAL(m);
    CU_ASSERT_STRING_EQUAL(m->name, name);
    CU_ASSERT_STRING_EQUAL(m->desc, desc);
    CU_ASSERT_EQUAL(m->price, price);

    // a new merch is on no shelf, and therefore not in stock
    ioopm_shelf_t **shelves = NULL;
    size_t size = 123;    // garbage on purpose: the function must overwrite it
    size_t total = 123;
    CU_ASSERT_EQUAL(ioopm_db_item_stock(db, name, &shelves, &size, &total), DB_OK);
    CU_ASSERT_EQUAL(size, 0);
    CU_ASSERT_EQUAL(total, 0);
    free(shelves);
}

// The amount of merch name on shelf, or -1 if the merch is not on that shelf
static int amount_on_shelf(ioopm_data_base_t *db, char *name, char *shelf)
{
    ioopm_shelf_t **shelves = NULL;
    size_t size = 0;
    size_t total = 0;
    int amount = -1;

    if (ioopm_db_item_stock(db, name, &shelves, &size, &total) == DB_OK)
    {
        for (size_t i = 0; i < size; ++i)
        {
            if (strcmp(shelves[i]->shelf.s, shelf) == 0)
            {
                amount = shelves[i]->amount;
            }
        }
    }
    free(shelves);
    return amount;
}

// Number of shelves merch name is on; the total amount is stored in *total
static size_t shelf_count(ioopm_data_base_t *db, char *name, size_t *total)
{
    ioopm_shelf_t **shelves = NULL;
    size_t size = 0;
    ioopm_db_item_stock(db, name, &shelves, &size, total);
    free(shelves);
    return size;
}

// True if shelf is registered as used in ht_n
static bool shelf_in_use(ioopm_data_base_t *db, char *shelf)
{
    elem_t ignored;
    return ioopm_hash_table_lookup(db->ht_n, string_elem(shelf), &ignored);
}

// The shelf named location among the first size entries, or NULL
static ioopm_shelf_t *find_shelf(ioopm_shelf_t **shelves, size_t size, char *location)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (strcmp(shelves[i]->shelf.s, location) == 0)
        {
            return shelves[i];
        }
    }
    return NULL;
}

// Edits a merch, copying name_new and desc so the db can take ownership
static ioopm_db_result_t edit(ioopm_data_base_t *db, char *name_old, char *name_new, char *desc, int price)
{
    char *n = strdup(name_new);
    char *d = strdup(desc);
    ioopm_db_result_t res = ioopm_db_edit_merch(db, name_old, n, d, price);
    if (res != DB_OK)
    {
        // rejected: the db did not take the strings
        free(n);
        free(d);
    }
    return res;
}

// create / destroy

void db_create_destroy(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_PTR_NOT_NULL(db);
    ioopm_db_destroy(db);
}

void db_create_is_empty(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_PTR_NOT_NULL_FATAL(db);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_s));
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_n));
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->name_id_ht));
    ioopm_db_destroy(db);
}

// add

void db_add_one(void)
{
    ioopm_data_base_t *db = new_db();

    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 1);
    assert_tables_consistent(db);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_n));   // no shelves used
    assert_new_merch(db, "Apple", "Red fruit", 5);

    ioopm_db_destroy(db);
}

void db_add_three(void)
{
    ioopm_data_base_t *db = new_db();

    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Orange", "Orange fruit", 7), DB_OK);

    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 3);
    assert_tables_consistent(db);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_n));
    assert_new_merch(db, "Apple", "Red fruit", 5);
    assert_new_merch(db, "Pear", "Green fruit", 6);
    assert_new_merch(db, "Orange", "Orange fruit", 7);

    ioopm_db_destroy(db);
}

void db_add_same_name(void)
{
    // Adding a merch with the same name as an existing merch is not allowed
    ioopm_data_base_t *db = new_db();

    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Apple", "Red and Green fruit", 10), DB_NAME_TAKEN);

    // the original is untouched
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 1);
    assert_tables_consistent(db);
    assert_new_merch(db, "Apple", "Red fruit", 5);

    ioopm_db_destroy(db);
}

void db_add_same_name_other_case(void)
{
    // Names are case sensitive: "apple" is a different merch than "Apple"
    ioopm_data_base_t *db = new_db();

    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "apple", "Small red fruit", 4), DB_OK);

    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 2);
    assert_tables_consistent(db);
    assert_new_merch(db, "apple", "Small red fruit", 4);

    ioopm_db_destroy(db);
}

void db_add_lookup_other_buffer(void)
{
    // Lookup must compare the text, not the pointer
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    char key[] = "Apple";   // a different buffer with the same text
    CU_ASSERT_PTR_NOT_NULL(lookup_merch(db, key));

    ioopm_db_destroy(db);
}

void db_add_lookup_missing(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_PTR_NULL(lookup_merch(db, "Apple"));

    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_PTR_NULL(lookup_merch(db, "Pear"));

    ioopm_db_destroy(db);
}

void db_add_many(void)
{
    // Enough items to make the hash tables grow; all must still be found
    ioopm_data_base_t *db = new_db();
    char name[16];

    for (int i = 0; i < 50; ++i)
    {
        snprintf(name, sizeof(name), "Item%d", i);
        CU_ASSERT_EQUAL(add(db, name, "Some item", i), DB_OK);
    }
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 50);
    assert_tables_consistent(db);

    for (int i = 0; i < 50; ++i)
    {
        snprintf(name, sizeof(name), "Item%d", i);
        merch_t *m = lookup_merch(db, name);
        CU_ASSERT_PTR_NOT_NULL_FATAL(m);
        CU_ASSERT_EQUAL(m->price, i);
    }

    ioopm_db_destroy(db);
}

void db_add_negative_price(void)
{
    // A negative price is rejected and nothing is added
    ioopm_data_base_t *db = new_db();

    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", -1), DB_INVALID_AMOUNT);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_s));
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->name_id_ht));

    ioopm_db_destroy(db);
}

// ids

void db_ids_unique(void)
{
    // Every merch gets its own id
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Orange", "Orange fruit", 7), DB_OK);

    merch_t *a = lookup_merch(db, "Apple");
    merch_t *p = lookup_merch(db, "Pear");
    merch_t *o = lookup_merch(db, "Orange");
    CU_ASSERT_PTR_NOT_NULL_FATAL(a);
    CU_ASSERT_PTR_NOT_NULL_FATAL(p);
    CU_ASSERT_PTR_NOT_NULL_FATAL(o);
    CU_ASSERT_NOT_EQUAL(a->id, p->id);
    CU_ASSERT_NOT_EQUAL(a->id, o->id);
    CU_ASSERT_NOT_EQUAL(p->id, o->id);

    ioopm_db_destroy(db);
}

void db_ids_name_maps_to_own_id(void)
{
    // name_id_ht must point each name at its own merch's id
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);

    elem_t id;
    CU_ASSERT_TRUE_FATAL(ioopm_hash_table_lookup(db->name_id_ht, string_elem("Apple"), &id));
    CU_ASSERT_EQUAL(id.i, lookup_merch(db, "Apple")->id);
    CU_ASSERT_TRUE_FATAL(ioopm_hash_table_lookup(db->name_id_ht, string_elem("Pear"), &id));
    CU_ASSERT_EQUAL(id.i, lookup_merch(db, "Pear")->id);

    ioopm_db_destroy(db);
}

void db_ids_not_reused_after_remove(void)
{
    // A re-added name gets a new id, and other merch keep theirs
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);
    int old_apple = lookup_merch(db, "Apple")->id;
    int pear = lookup_merch(db, "Pear")->id;

    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Apple"), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_NOT_EQUAL(lookup_merch(db, "Apple")->id, old_apple);
    CU_ASSERT_EQUAL(lookup_merch(db, "Pear")->id, pear);
    assert_tables_consistent(db);

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
    ioopm_data_base_t *db = new_db();

    size_t size = 123;   // garbage on purpose: the function must overwrite it
    char **names = ioopm_db_merch_names(db, &size);
    CU_ASSERT_EQUAL(size, 0);
    CU_ASSERT_PTR_NULL(names);

    ioopm_db_destroy(db);
}

void db_names_one(void)
{
    ioopm_data_base_t *db = new_db();
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
    ioopm_data_base_t *db = new_db();
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
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    size_t size = 0;
    char **names = ioopm_db_merch_names(db, &size);
    CU_ASSERT_EQUAL_FATAL(size, 1);
    CU_ASSERT_PTR_EQUAL(names[0], lookup_merch(db, "Apple")->name);

    free(names);
    ioopm_db_destroy(db);
}

void db_names_after_rejected_add(void)
{
    // A rejected duplicate must not show up in the list
    ioopm_data_base_t *db = new_db();
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
    // More than 20 items (one listing page) and enough to make the tables grow
    ioopm_data_base_t *db = new_db();
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

// remove

void db_remove_only_item(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Apple"), DB_OK);
    CU_ASSERT_PTR_NULL(lookup_merch(db, "Apple"));
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_s));
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->name_id_ht));

    ioopm_db_destroy(db);
}

void db_remove_missing(void)
{
    // Removing from an empty db, and removing a name that was never added
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Apple"), DB_NO_SUCH_MERCH);

    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Pear"), DB_NO_SUCH_MERCH);

    // nothing was removed
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 1);
    assert_tables_consistent(db);
    assert_new_merch(db, "Apple", "Red fruit", 5);

    ioopm_db_destroy(db);
}

void db_remove_twice(void)
{
    // The second remove must fail: the merch is already gone
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Apple"), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Apple"), DB_NO_SUCH_MERCH);
    assert_tables_consistent(db);

    ioopm_db_destroy(db);
}

void db_remove_case_sensitive(void)
{
    // Removing "apple" must not touch "Apple"
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "apple"), DB_NO_SUCH_MERCH);
    CU_ASSERT_PTR_NOT_NULL(lookup_merch(db, "Apple"));

    ioopm_db_destroy(db);
}

void db_remove_one_of_three(void)
{
    // The other merch must be untouched
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Orange", "Orange fruit", 7), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Pear"), DB_OK);

    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 2);
    assert_tables_consistent(db);
    CU_ASSERT_PTR_NULL(lookup_merch(db, "Pear"));
    assert_new_merch(db, "Apple", "Red fruit", 5);
    assert_new_merch(db, "Orange", "Orange fruit", 7);

    ioopm_db_destroy(db);
}

void db_remove_all(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Apple"), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Pear"), DB_OK);

    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_s));
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->name_id_ht));

    size_t size = 123;
    char **names = ioopm_db_merch_names(db, &size);
    CU_ASSERT_EQUAL(size, 0);
    CU_ASSERT_PTR_NULL(names);

    ioopm_db_destroy(db);
}

void db_remove_then_add_again(void)
{
    // After removal the name is free again
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Apple"), DB_OK);

    CU_ASSERT_EQUAL(add(db, "Apple", "Green fruit", 9), DB_OK);
    assert_new_merch(db, "Apple", "Green fruit", 9);
    assert_tables_consistent(db);

    ioopm_db_destroy(db);
}

void db_remove_not_in_names(void)
{
    // A removed merch must not show up in the name list
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Apple"), DB_OK);

    size_t size = 0;
    char **names = ioopm_db_merch_names(db, &size);
    CU_ASSERT_EQUAL_FATAL(size, 1);
    CU_ASSERT_EQUAL(count_name(names, size, "Apple"), 0);
    CU_ASSERT_EQUAL(count_name(names, size, "Pear"), 1);

    free(names);
    ioopm_db_destroy(db);
}

void db_remove_with_merch_own_name(void)
{
    // The UI will likely pass a name taken from ioopm_db_merch_names, which
    // is the merch's own string. remove must not use it after freeing it.
    // Run with `make memtest`: Valgrind reports the error if the order is wrong.
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    size_t size = 0;
    char **names = ioopm_db_merch_names(db, &size);
    CU_ASSERT_EQUAL_FATAL(size, 1);

    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, names[0]), DB_OK);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_s));
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->name_id_ht));

    free(names);   // names[0] is now dangling, so only free the array
    ioopm_db_destroy(db);
}

void db_remove_many(void)
{
    // Remove every other item from tables big enough to have grown
    ioopm_data_base_t *db = new_db();
    char name[16];
    for (int i = 0; i < 50; ++i)
    {
        snprintf(name, sizeof(name), "Item%d", i);
        CU_ASSERT_EQUAL(add(db, name, "Some item", i), DB_OK);
    }

    for (int i = 0; i < 50; i += 2)
    {
        snprintf(name, sizeof(name), "Item%d", i);
        CU_ASSERT_EQUAL(ioopm_db_remove_item(db, name), DB_OK);
    }

    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 25);
    assert_tables_consistent(db);
    for (int i = 0; i < 50; ++i)
    {
        snprintf(name, sizeof(name), "Item%d", i);
        if (i % 2 == 0)
        {
            CU_ASSERT_PTR_NULL(lookup_merch(db, name));
        }
        else
        {
            CU_ASSERT_PTR_NOT_NULL(lookup_merch(db, name));
        }
    }

    ioopm_db_destroy(db);
}

// replenish

void db_replenish_new_shelf(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);

    size_t total = 0;
    CU_ASSERT_EQUAL(shelf_count(db, "Apple", &total), 1);
    CU_ASSERT_EQUAL(total, 10);
    CU_ASSERT_EQUAL(amount_on_shelf(db, "Apple", "A25"), 10);

    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_n), 1);
    CU_ASSERT_TRUE(shelf_in_use(db, "A25"));

    ioopm_db_destroy(db);
}

void db_replenish_shelf_maps_to_id(void)
{
    // ht_n must point the shelf at the id of the merch stored on it
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Pear", "B12", 3), DB_OK);

    elem_t id;
    CU_ASSERT_TRUE_FATAL(ioopm_hash_table_lookup(db->ht_n, string_elem("B12"), &id));
    CU_ASSERT_EQUAL(id.i, lookup_merch(db, "Pear")->id);

    ioopm_db_destroy(db);
}

void db_replenish_same_shelf_adds(void)
{
    // Replenishing a shelf the merch is already on increases the amount
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 5), DB_OK);

    size_t total = 0;
    CU_ASSERT_EQUAL(shelf_count(db, "Apple", &total), 1);   // still one shelf
    CU_ASSERT_EQUAL(total, 15);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_n), 1);

    ioopm_db_destroy(db);
}

void db_replenish_same_shelf_not_first(void)
{
    // The matching shelf is not the first one in the list
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "B12", 3), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "B12", 4), DB_OK);

    CU_ASSERT_EQUAL(amount_on_shelf(db, "Apple", "A25"), 10);
    CU_ASSERT_EQUAL(amount_on_shelf(db, "Apple", "B12"), 7);

    ioopm_db_destroy(db);
}

void db_replenish_two_shelves(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "B12", 3), DB_OK);

    size_t total = 0;
    CU_ASSERT_EQUAL(shelf_count(db, "Apple", &total), 2);
    CU_ASSERT_EQUAL(total, 13);
    CU_ASSERT_EQUAL(amount_on_shelf(db, "Apple", "A25"), 10);
    CU_ASSERT_EQUAL(amount_on_shelf(db, "Apple", "B12"), 3);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_n), 2);

    ioopm_db_destroy(db);
}

void db_replenish_shelf_taken(void)
{
    // A shelf can only hold one kind of merch
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Pear", "A25", 4), DB_SHELF_TAKEN);

    // Apple is untouched, Pear got nothing
    CU_ASSERT_EQUAL(amount_on_shelf(db, "Apple", "A25"), 10);
    size_t total = 0;
    CU_ASSERT_EQUAL(shelf_count(db, "Pear", &total), 0);
    CU_ASSERT_EQUAL(total, 0);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_n), 1);

    ioopm_db_destroy(db);
}

void db_replenish_no_such_merch(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_NO_SUCH_MERCH);

    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Pear", "A25", 10), DB_NO_SUCH_MERCH);

    // the shelf must not be marked as used
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_n));

    ioopm_db_destroy(db);
}

void db_replenish_case_sensitive(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "apple", "A25", 10), DB_NO_SUCH_MERCH);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_n));

    ioopm_db_destroy(db);
}

void db_replenish_invalid_amount(void)
{
    // Zero and negative amounts are rejected and change nothing
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 0), DB_INVALID_AMOUNT);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", -5), DB_INVALID_AMOUNT);

    size_t total = 0;
    CU_ASSERT_EQUAL(shelf_count(db, "Apple", &total), 0);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_n));

    ioopm_db_destroy(db);
}

void db_replenish_invalid_amount_existing_shelf(void)
{
    // A rejected replenish must not change an existing shelf
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", -3), DB_INVALID_AMOUNT);
    CU_ASSERT_EQUAL(amount_on_shelf(db, "Apple", "A25"), 10);

    ioopm_db_destroy(db);
}

void db_replenish_copies_shelf_name(void)
{
    // The db must keep its own copy: the caller frees or reuses its buffer
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    char buf[] = "A25";
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", buf, 10), DB_OK);
    strcpy(buf, "XXX");   // overwrite the caller's buffer

    CU_ASSERT_EQUAL(amount_on_shelf(db, "Apple", "A25"), 10);
    CU_ASSERT_TRUE(shelf_in_use(db, "A25"));
    CU_ASSERT_FALSE(shelf_in_use(db, "XXX"));

    ioopm_db_destroy(db);
}

void db_replenish_other_merch_untouched(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Pear", "B12", 2), DB_OK);

    CU_ASSERT_EQUAL(amount_on_shelf(db, "Apple", "B12"), -1);
    CU_ASSERT_EQUAL(amount_on_shelf(db, "Pear", "A25"), -1);
    CU_ASSERT_EQUAL(amount_on_shelf(db, "Pear", "B12"), 2);

    ioopm_db_destroy(db);
}

void db_replenish_remove_frees_shelves(void)
{
    // Removing a merch frees its shelves in ht_n
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "B12", 3), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Apple"), DB_OK);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_n));
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_s));
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->name_id_ht));

    ioopm_db_destroy(db);
}

void db_replenish_remove_keeps_other_shelves(void)
{
    // Removing one merch must only free its own shelves
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Pear", "B12", 3), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Apple"), DB_OK);
    CU_ASSERT_FALSE(shelf_in_use(db, "A25"));
    CU_ASSERT_TRUE(shelf_in_use(db, "B12"));
    CU_ASSERT_EQUAL(amount_on_shelf(db, "Pear", "B12"), 3);

    ioopm_db_destroy(db);
}

void db_replenish_shelf_reusable_after_remove(void)
{
    // After removal the shelf can be used by another merch
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Apple"), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Pear", "A25", 4), DB_OK);
    CU_ASSERT_EQUAL(amount_on_shelf(db, "Pear", "A25"), 4);

    ioopm_db_destroy(db);
}

// stock

void db_stock_missing_empty_db(void)
{
    // The out-parameters must be defined even on failure
    ioopm_data_base_t *db = new_db();

    ioopm_shelf_t **shelves = (ioopm_shelf_t **) 0x1;   // garbage on purpose
    size_t size = 123;
    size_t total = 123;
    CU_ASSERT_EQUAL(ioopm_db_item_stock(db, "Apple", &shelves, &size, &total), DB_NO_SUCH_MERCH);
    CU_ASSERT_PTR_NULL(shelves);
    CU_ASSERT_EQUAL(size, 0);
    CU_ASSERT_EQUAL(total, 0);

    ioopm_db_destroy(db);
}

void db_stock_missing_other_name(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);

    ioopm_shelf_t **shelves = (ioopm_shelf_t **) 0x1;
    size_t size = 123;
    size_t total = 123;
    CU_ASSERT_EQUAL(ioopm_db_item_stock(db, "Pear", &shelves, &size, &total), DB_NO_SUCH_MERCH);
    CU_ASSERT_PTR_NULL(shelves);
    CU_ASSERT_EQUAL(size, 0);
    CU_ASSERT_EQUAL(total, 0);

    ioopm_db_destroy(db);
}

void db_stock_case_sensitive(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);

    ioopm_shelf_t **shelves = NULL;
    size_t size = 0;
    size_t total = 0;
    CU_ASSERT_EQUAL(ioopm_db_item_stock(db, "apple", &shelves, &size, &total), DB_NO_SUCH_MERCH);
    CU_ASSERT_PTR_NULL(shelves);

    ioopm_db_destroy(db);
}

void db_stock_no_shelves(void)
{
    // Merch exists but is on no shelf
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    ioopm_shelf_t **shelves = (ioopm_shelf_t **) 0x1;
    size_t size = 123;
    size_t total = 123;
    CU_ASSERT_EQUAL(ioopm_db_item_stock(db, "Apple", &shelves, &size, &total), DB_OK);
    CU_ASSERT_PTR_NULL(shelves);
    CU_ASSERT_EQUAL(size, 0);
    CU_ASSERT_EQUAL(total, 0);

    ioopm_db_destroy(db);
}

void db_stock_one_shelf(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);

    ioopm_shelf_t **shelves = NULL;
    size_t size = 0;
    size_t total = 0;
    CU_ASSERT_EQUAL(ioopm_db_item_stock(db, "Apple", &shelves, &size, &total), DB_OK);
    CU_ASSERT_EQUAL_FATAL(size, 1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(shelves);
    CU_ASSERT_STRING_EQUAL(shelves[0]->shelf.s, "A25");
    CU_ASSERT_EQUAL(shelves[0]->amount, 10);
    CU_ASSERT_EQUAL(total, 10);

    free(shelves);
    ioopm_db_destroy(db);
}

void db_stock_many_shelves(void)
{
    // Every shelf returned exactly once, in any order, and total is the sum
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "B12", 3), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "C01", 7), DB_OK);

    ioopm_shelf_t **shelves = NULL;
    size_t size = 0;
    size_t total = 0;
    CU_ASSERT_EQUAL(ioopm_db_item_stock(db, "Apple", &shelves, &size, &total), DB_OK);
    CU_ASSERT_EQUAL_FATAL(size, 3);
    CU_ASSERT_PTR_NOT_NULL_FATAL(shelves);

    ioopm_shelf_t *a = find_shelf(shelves, size, "A25");
    ioopm_shelf_t *b = find_shelf(shelves, size, "B12");
    ioopm_shelf_t *c = find_shelf(shelves, size, "C01");
    CU_ASSERT_PTR_NOT_NULL_FATAL(a);
    CU_ASSERT_PTR_NOT_NULL_FATAL(b);
    CU_ASSERT_PTR_NOT_NULL_FATAL(c);
    CU_ASSERT_EQUAL(a->amount, 10);
    CU_ASSERT_EQUAL(b->amount, 3);
    CU_ASSERT_EQUAL(c->amount, 7);
    CU_ASSERT_EQUAL(total, 20);

    free(shelves);
    ioopm_db_destroy(db);
}

void db_stock_same_shelf_twice(void)
{
    // Replenishing the same shelf twice shows up as one shelf, not two
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 5), DB_OK);

    ioopm_shelf_t **shelves = NULL;
    size_t size = 0;
    size_t total = 0;
    CU_ASSERT_EQUAL(ioopm_db_item_stock(db, "Apple", &shelves, &size, &total), DB_OK);
    CU_ASSERT_EQUAL_FATAL(size, 1);
    CU_ASSERT_EQUAL(shelves[0]->amount, 15);
    CU_ASSERT_EQUAL(total, 15);

    free(shelves);
    ioopm_db_destroy(db);
}

void db_stock_only_own_shelves(void)
{
    // Another merch's shelves must not be included
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Pear", "P01", 2), DB_OK);

    ioopm_shelf_t **shelves = NULL;
    size_t size = 0;
    size_t total = 0;
    CU_ASSERT_EQUAL(ioopm_db_item_stock(db, "Apple", &shelves, &size, &total), DB_OK);
    CU_ASSERT_EQUAL_FATAL(size, 1);
    CU_ASSERT_PTR_NOT_NULL(find_shelf(shelves, size, "A25"));
    CU_ASSERT_PTR_NULL(find_shelf(shelves, size, "P01"));
    CU_ASSERT_EQUAL(total, 10);

    free(shelves);
    ioopm_db_destroy(db);
}

void db_stock_rejected_replenish_not_shown(void)
{
    // A failed replenish (taken shelf, invalid amount) must not appear in stock
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Pear", "A25", 4), DB_SHELF_TAKEN);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Pear", "B12", 0), DB_INVALID_AMOUNT);

    ioopm_shelf_t **shelves = (ioopm_shelf_t **) 0x1;
    size_t size = 123;
    size_t total = 123;
    CU_ASSERT_EQUAL(ioopm_db_item_stock(db, "Pear", &shelves, &size, &total), DB_OK);
    CU_ASSERT_PTR_NULL(shelves);
    CU_ASSERT_EQUAL(size, 0);
    CU_ASSERT_EQUAL(total, 0);

    ioopm_db_destroy(db);
}

void db_stock_does_not_change_db(void)
{
    // Calling twice gives the same answer, and freeing the array
    // must not destroy the shelves (the db still owns them)
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "B12", 3), DB_OK);

    ioopm_shelf_t **shelves = NULL;
    size_t size = 0;
    size_t total = 0;
    CU_ASSERT_EQUAL(ioopm_db_item_stock(db, "Apple", &shelves, &size, &total), DB_OK);
    free(shelves);

    CU_ASSERT_EQUAL(ioopm_db_item_stock(db, "Apple", &shelves, &size, &total), DB_OK);
    CU_ASSERT_EQUAL(size, 2);
    CU_ASSERT_EQUAL(total, 13);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_n), 2);

    free(shelves);
    ioopm_db_destroy(db);
}

void db_stock_after_remove(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Apple"), DB_OK);

    ioopm_shelf_t **shelves = (ioopm_shelf_t **) 0x1;
    size_t size = 123;
    size_t total = 123;
    CU_ASSERT_EQUAL(ioopm_db_item_stock(db, "Apple", &shelves, &size, &total), DB_NO_SUCH_MERCH);
    CU_ASSERT_PTR_NULL(shelves);
    CU_ASSERT_EQUAL(size, 0);
    CU_ASSERT_EQUAL(total, 0);

    ioopm_db_destroy(db);
}

void db_stock_many_shelves_one_merch(void)
{
    // Enough shelves to catch a fixed-size array or wrong allocation size
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    char shelf[8];
    size_t expected_total = 0;
    for (int i = 1; i <= 30; ++i)
    {
        snprintf(shelf, sizeof(shelf), "A%d", i);
        CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", shelf, i), DB_OK);
        expected_total += i;
    }

    ioopm_shelf_t **shelves = NULL;
    size_t size = 0;
    size_t total = 0;
    CU_ASSERT_EQUAL(ioopm_db_item_stock(db, "Apple", &shelves, &size, &total), DB_OK);
    CU_ASSERT_EQUAL_FATAL(size, 30);
    CU_ASSERT_EQUAL(total, expected_total);

    for (int i = 1; i <= 30; ++i)
    {
        snprintf(shelf, sizeof(shelf), "A%d", i);
        ioopm_shelf_t *s = find_shelf(shelves, size, shelf);
        CU_ASSERT_PTR_NOT_NULL_FATAL(s);
        CU_ASSERT_EQUAL(s->amount, i);
    }

    free(shelves);
    ioopm_db_destroy(db);
}


// edit: desc and price only (name unchanged)

void db_edit_desc_and_price(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_EQUAL(edit(db, "Apple", "Apple", "Green fruit", 9), DB_OK);
    assert_new_merch(db, "Apple", "Green fruit", 9);
    assert_tables_consistent(db);

    ioopm_db_destroy(db);
}

void db_edit_keeps_name_and_id(void)
{
    // Same name: still the same merch, same id
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    merch_t *before = lookup_merch(db, "Apple");
    CU_ASSERT_PTR_NOT_NULL_FATAL(before);
    int id = before->id;

    CU_ASSERT_EQUAL(edit(db, "Apple", "Apple", "Green fruit", 9), DB_OK);

    merch_t *after = lookup_merch(db, "Apple");
    CU_ASSERT_PTR_NOT_NULL_FATAL(after);
    CU_ASSERT_PTR_EQUAL(after, before);
    CU_ASSERT_EQUAL(after->id, id);

    ioopm_db_destroy(db);
}

void db_edit_missing(void)
{
    // Editing in an empty db, and editing a name that was never added
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(edit(db, "Apple", "Apple", "Green fruit", 9), DB_NO_SUCH_MERCH);

    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(edit(db, "Pear", "Pear", "Green fruit", 9), DB_NO_SUCH_MERCH);
    CU_ASSERT_EQUAL(edit(db, "Pear", "Banana", "Yellow fruit", 9), DB_NO_SUCH_MERCH);

    // nothing was created or changed
    CU_ASSERT_PTR_NULL(lookup_merch(db, "Banana"));
    assert_new_merch(db, "Apple", "Red fruit", 5);
    assert_tables_consistent(db);

    ioopm_db_destroy(db);
}

void db_edit_case_sensitive(void)
{
    // Editing "apple" must not touch "Apple"
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_EQUAL(edit(db, "apple", "apple", "Green fruit", 9), DB_NO_SUCH_MERCH);
    assert_new_merch(db, "Apple", "Red fruit", 5);

    ioopm_db_destroy(db);
}

void db_edit_negative_price(void)
{
    // Same rule as add: a negative price is rejected and nothing changes,
    // not even the name
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_EQUAL(edit(db, "Apple", "Apple", "Green fruit", -1), DB_INVALID_AMOUNT);
    CU_ASSERT_EQUAL(edit(db, "Apple", "Pear", "Green fruit", -1), DB_INVALID_AMOUNT);

    CU_ASSERT_PTR_NULL(lookup_merch(db, "Pear"));
    assert_new_merch(db, "Apple", "Red fruit", 5);
    assert_tables_consistent(db);

    ioopm_db_destroy(db);
}

void db_edit_price_zero(void)
{
    // Same rule as add: 0 is a valid price
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_EQUAL(edit(db, "Apple", "Apple", "Free fruit", 0), DB_OK);
    assert_new_merch(db, "Apple", "Free fruit", 0);

    ioopm_db_destroy(db);
}

void db_edit_one_of_three(void)
{
    // The other merch must be untouched
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Orange", "Orange fruit", 7), DB_OK);

    CU_ASSERT_EQUAL(edit(db, "Pear", "Pear", "Yellow fruit", 8), DB_OK);

    assert_new_merch(db, "Apple", "Red fruit", 5);
    assert_new_merch(db, "Pear", "Yellow fruit", 8);
    assert_new_merch(db, "Orange", "Orange fruit", 7);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 3);
    assert_tables_consistent(db);

    ioopm_db_destroy(db);
}

void db_edit_twice(void)
{
    // The last edit wins. With `make memtest`, this also catches the old
    // name or description leaking when they are replaced.
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_EQUAL(edit(db, "Apple", "Apple", "Green fruit", 9), DB_OK);
    CU_ASSERT_EQUAL(edit(db, "Apple", "Apple", "Blue fruit", 12), DB_OK);
    assert_new_merch(db, "Apple", "Blue fruit", 12);

    ioopm_db_destroy(db);
}

void db_edit_keeps_stock(void)
{
    // Editing desc and price must not touch the shelves
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "B12", 3), DB_OK);

    CU_ASSERT_EQUAL(edit(db, "Apple", "Apple", "Green fruit", 9), DB_OK);

    size_t total = 0;
    CU_ASSERT_EQUAL(shelf_count(db, "Apple", &total), 2);
    CU_ASSERT_EQUAL(total, 13);
    CU_ASSERT_EQUAL(amount_on_shelf(db, "Apple", "A25"), 10);
    CU_ASSERT_EQUAL(amount_on_shelf(db, "Apple", "B12"), 3);

    ioopm_db_destroy(db);
}

void db_edit_after_remove(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Apple"), DB_OK);

    CU_ASSERT_EQUAL(edit(db, "Apple", "Apple", "Green fruit", 9), DB_NO_SUCH_MERCH);
    CU_ASSERT_PTR_NULL(lookup_merch(db, "Apple"));

    ioopm_db_destroy(db);
}

// edit: rename

void db_edit_rename(void)
{
    // The old name is gone, the new name finds the same merch with the same id
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    merch_t *before = lookup_merch(db, "Apple");
    CU_ASSERT_PTR_NOT_NULL_FATAL(before);
    int id = before->id;

    CU_ASSERT_EQUAL(edit(db, "Apple", "Pear", "Green fruit", 9), DB_OK);

    CU_ASSERT_PTR_NULL(lookup_merch(db, "Apple"));
    merch_t *after = lookup_merch(db, "Pear");
    CU_ASSERT_PTR_NOT_NULL_FATAL(after);
    CU_ASSERT_PTR_EQUAL(after, before);
    CU_ASSERT_EQUAL(after->id, id);
    assert_new_merch(db, "Pear", "Green fruit", 9);

    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 1);
    assert_tables_consistent(db);

    ioopm_db_destroy(db);
}

void db_edit_rename_name_taken(void)
{
    // Renaming to another merch's name is rejected and both are untouched
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Pear", "Green fruit", 6), DB_OK);

    CU_ASSERT_EQUAL(edit(db, "Apple", "Pear", "Yellow fruit", 9), DB_NAME_TAKEN);

    assert_new_merch(db, "Apple", "Red fruit", 5);
    assert_new_merch(db, "Pear", "Green fruit", 6);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 2);
    assert_tables_consistent(db);

    ioopm_db_destroy(db);
}

void db_edit_rename_case_only(void)
{
    // "Apple" -> "apple" is a real rename: names are case sensitive
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_EQUAL(edit(db, "Apple", "apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_PTR_NULL(lookup_merch(db, "Apple"));
    assert_new_merch(db, "apple", "Red fruit", 5);
    assert_tables_consistent(db);

    ioopm_db_destroy(db);
}

void db_edit_rename_keeps_stock(void)
{
    // Shelves follow the merch: ht_n maps to the id, which does not change
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "B12", 3), DB_OK);

    CU_ASSERT_EQUAL(edit(db, "Apple", "Pear", "Green fruit", 9), DB_OK);

    size_t total = 0;
    CU_ASSERT_EQUAL(shelf_count(db, "Pear", &total), 2);
    CU_ASSERT_EQUAL(total, 13);
    CU_ASSERT_EQUAL(amount_on_shelf(db, "Pear", "A25"), 10);

    elem_t id;
    CU_ASSERT_TRUE_FATAL(ioopm_hash_table_lookup(db->ht_n, string_elem("A25"), &id));
    CU_ASSERT_EQUAL(id.i, lookup_merch(db, "Pear")->id);

    // and the shelves can still be replenished under the new name
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Pear", "A25", 5), DB_OK);
    CU_ASSERT_EQUAL(amount_on_shelf(db, "Pear", "A25"), 15);

    ioopm_db_destroy(db);
}

void db_edit_rename_old_name_free(void)
{
    // After a rename, the old name can be used by a new merch
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(edit(db, "Apple", "Pear", "Green fruit", 9), DB_OK);

    CU_ASSERT_EQUAL(add(db, "Apple", "New red fruit", 4), DB_OK);
    assert_new_merch(db, "Apple", "New red fruit", 4);
    assert_new_merch(db, "Pear", "Green fruit", 9);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 2);
    assert_tables_consistent(db);

    ioopm_db_destroy(db);
}

void db_edit_rename_chain(void)
{
    // Apple -> Pear -> Banana: only the last name works
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    CU_ASSERT_EQUAL(edit(db, "Apple", "Pear", "Green fruit", 6), DB_OK);
    CU_ASSERT_EQUAL(edit(db, "Pear", "Banana", "Yellow fruit", 7), DB_OK);

    CU_ASSERT_PTR_NULL(lookup_merch(db, "Apple"));
    CU_ASSERT_PTR_NULL(lookup_merch(db, "Pear"));
    assert_new_merch(db, "Banana", "Yellow fruit", 7);
    assert_tables_consistent(db);

    ioopm_db_destroy(db);
}

void db_edit_rename_in_names(void)
{
    // The name list shows the new name, not the old
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(add(db, "Orange", "Orange fruit", 7), DB_OK);
    CU_ASSERT_EQUAL(edit(db, "Apple", "Pear", "Green fruit", 6), DB_OK);

    size_t size = 0;
    char **names = ioopm_db_merch_names(db, &size);
    CU_ASSERT_EQUAL_FATAL(size, 2);
    CU_ASSERT_EQUAL(count_name(names, size, "Apple"), 0);
    CU_ASSERT_EQUAL(count_name(names, size, "Pear"), 1);
    CU_ASSERT_EQUAL(count_name(names, size, "Orange"), 1);

    free(names);
    ioopm_db_destroy(db);
}

void db_edit_rename_then_remove(void)
{
    // The renamed merch can be removed under its new name, leaving nothing behind
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Apple", "A25", 10), DB_OK);
    CU_ASSERT_EQUAL(edit(db, "Apple", "Pear", "Green fruit", 6), DB_OK);

    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Apple"), DB_NO_SUCH_MERCH);
    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Pear"), DB_OK);

    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_s));
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->name_id_ht));
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_n));

    ioopm_db_destroy(db);
}

void db_edit_rename_with_merch_own_name(void)
{
    // The UI will likely pass name_old from ioopm_db_merch_names, which is
    // the merch's own string. A rename frees that string, so edit must take
    // the old key out of name_id_ht BEFORE freeing it. `make memtest` reports
    // an invalid read if the order is wrong.
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(add(db, "Apple", "Red fruit", 5), DB_OK);

    size_t size = 0;
    char **names = ioopm_db_merch_names(db, &size);
    CU_ASSERT_EQUAL_FATAL(size, 1);

    CU_ASSERT_EQUAL(edit(db, names[0], "Pear", "Green fruit", 6), DB_OK);
    free(names);   // names[0] is now dangling, so only free the array

    CU_ASSERT_PTR_NULL(lookup_merch(db, "Apple"));
    assert_new_merch(db, "Pear", "Green fruit", 6);
    assert_tables_consistent(db);

    ioopm_db_destroy(db);
}

void db_edit_rename_many(void)
{
    // Rename every item in tables big enough to have grown
    ioopm_data_base_t *db = new_db();
    char name[16];
    char new_name[16];
    for (int i = 0; i < 50; ++i)
    {
        snprintf(name, sizeof(name), "Item%d", i);
        CU_ASSERT_EQUAL(add(db, name, "Some item", i), DB_OK);
    }

    for (int i = 0; i < 50; ++i)
    {
        snprintf(name, sizeof(name), "Item%d", i);
        snprintf(new_name, sizeof(new_name), "Renamed%d", i);
        CU_ASSERT_EQUAL(edit(db, name, new_name, "Edited item", i * 2), DB_OK);
    }

    for (int i = 0; i < 50; ++i)
    {
        snprintf(name, sizeof(name), "Item%d", i);
        snprintf(new_name, sizeof(new_name), "Renamed%d", i);
        CU_ASSERT_PTR_NULL(lookup_merch(db, name));
        merch_t *m = lookup_merch(db, new_name);
        CU_ASSERT_PTR_NOT_NULL_FATAL(m);
        CU_ASSERT_STRING_EQUAL(m->desc, "Edited item");
        CU_ASSERT_EQUAL(m->price, i * 2);
    }
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 50);
    assert_tables_consistent(db);

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
        (CU_add_test(my_test_suite, "add: negative price", db_add_negative_price) == NULL) ||

        (CU_add_test(my_test_suite, "ids: unique", db_ids_unique) == NULL) ||
        (CU_add_test(my_test_suite, "ids: name maps to own id", db_ids_name_maps_to_own_id) == NULL) ||
        (CU_add_test(my_test_suite, "ids: not reused after remove", db_ids_not_reused_after_remove) == NULL) ||

        (CU_add_test(my_test_suite, "names: empty db", db_names_empty) == NULL) ||
        (CU_add_test(my_test_suite, "names: one", db_names_one) == NULL) ||
        (CU_add_test(my_test_suite, "names: three", db_names_three) == NULL) ||
        (CU_add_test(my_test_suite, "names: not copies", db_names_are_not_copies) == NULL) ||
        (CU_add_test(my_test_suite, "names: after rejected add", db_names_after_rejected_add) == NULL) ||
        (CU_add_test(my_test_suite, "names: many", db_names_many) == NULL) ||

        (CU_add_test(my_test_suite, "remove: only item", db_remove_only_item) == NULL) ||
        (CU_add_test(my_test_suite, "remove: missing", db_remove_missing) == NULL) ||
        (CU_add_test(my_test_suite, "remove: twice", db_remove_twice) == NULL) ||
        (CU_add_test(my_test_suite, "remove: case sensitive", db_remove_case_sensitive) == NULL) ||
        (CU_add_test(my_test_suite, "remove: one of three", db_remove_one_of_three) == NULL) ||
        (CU_add_test(my_test_suite, "remove: all", db_remove_all) == NULL) ||
        (CU_add_test(my_test_suite, "remove: then add again", db_remove_then_add_again) == NULL) ||
        (CU_add_test(my_test_suite, "remove: not in names", db_remove_not_in_names) == NULL) ||
        (CU_add_test(my_test_suite, "remove: with merch's own name", db_remove_with_merch_own_name) == NULL) ||
        (CU_add_test(my_test_suite, "remove: many", db_remove_many) == NULL) ||

        (CU_add_test(my_test_suite, "replenish: new shelf", db_replenish_new_shelf) == NULL) ||
        (CU_add_test(my_test_suite, "replenish: shelf maps to id", db_replenish_shelf_maps_to_id) == NULL) ||
        (CU_add_test(my_test_suite, "replenish: same shelf adds", db_replenish_same_shelf_adds) == NULL) ||
        (CU_add_test(my_test_suite, "replenish: same shelf, not first", db_replenish_same_shelf_not_first) == NULL) ||
        (CU_add_test(my_test_suite, "replenish: two shelves", db_replenish_two_shelves) == NULL) ||
        (CU_add_test(my_test_suite, "replenish: shelf taken", db_replenish_shelf_taken) == NULL) ||
        (CU_add_test(my_test_suite, "replenish: no such merch", db_replenish_no_such_merch) == NULL) ||
        (CU_add_test(my_test_suite, "replenish: case sensitive", db_replenish_case_sensitive) == NULL) ||
        (CU_add_test(my_test_suite, "replenish: invalid amount", db_replenish_invalid_amount) == NULL) ||
        (CU_add_test(my_test_suite, "replenish: invalid amount, existing shelf", db_replenish_invalid_amount_existing_shelf) == NULL) ||
        (CU_add_test(my_test_suite, "replenish: copies shelf name", db_replenish_copies_shelf_name) == NULL) ||
        (CU_add_test(my_test_suite, "replenish: other merch untouched", db_replenish_other_merch_untouched) == NULL) ||
        (CU_add_test(my_test_suite, "replenish: remove frees shelves", db_replenish_remove_frees_shelves) == NULL) ||
        (CU_add_test(my_test_suite, "replenish: remove keeps other shelves", db_replenish_remove_keeps_other_shelves) == NULL) ||
        (CU_add_test(my_test_suite, "replenish: shelf reusable after remove", db_replenish_shelf_reusable_after_remove) == NULL) ||

        (CU_add_test(my_test_suite, "stock: missing, empty db", db_stock_missing_empty_db) == NULL) ||
        (CU_add_test(my_test_suite, "stock: missing, other name", db_stock_missing_other_name) == NULL) ||
        (CU_add_test(my_test_suite, "stock: case sensitive", db_stock_case_sensitive) == NULL) ||
        (CU_add_test(my_test_suite, "stock: no shelves", db_stock_no_shelves) == NULL) ||
        (CU_add_test(my_test_suite, "stock: one shelf", db_stock_one_shelf) == NULL) ||
        (CU_add_test(my_test_suite, "stock: many shelves", db_stock_many_shelves) == NULL) ||
        (CU_add_test(my_test_suite, "stock: same shelf twice", db_stock_same_shelf_twice) == NULL) ||
        (CU_add_test(my_test_suite, "stock: only own shelves", db_stock_only_own_shelves) == NULL) ||
        (CU_add_test(my_test_suite, "stock: rejected replenish not shown", db_stock_rejected_replenish_not_shown) == NULL) ||
        (CU_add_test(my_test_suite, "stock: does not change db", db_stock_does_not_change_db) == NULL) ||
        (CU_add_test(my_test_suite, "stock: after remove", db_stock_after_remove) == NULL) ||
        (CU_add_test(my_test_suite, "stock: 30 shelves", db_stock_many_shelves_one_merch) == NULL) ||

        (CU_add_test(my_test_suite, "edit: desc and price", db_edit_desc_and_price) == NULL) ||
        (CU_add_test(my_test_suite, "edit: keeps name and id", db_edit_keeps_name_and_id) == NULL) ||
        (CU_add_test(my_test_suite, "edit: missing", db_edit_missing) == NULL) ||
        (CU_add_test(my_test_suite, "edit: case sensitive", db_edit_case_sensitive) == NULL) ||
        (CU_add_test(my_test_suite, "edit: negative price", db_edit_negative_price) == NULL) ||
        (CU_add_test(my_test_suite, "edit: price zero", db_edit_price_zero) == NULL) ||
        (CU_add_test(my_test_suite, "edit: one of three", db_edit_one_of_three) == NULL) ||
        (CU_add_test(my_test_suite, "edit: twice", db_edit_twice) == NULL) ||
        (CU_add_test(my_test_suite, "edit: keeps stock", db_edit_keeps_stock) == NULL) ||
        (CU_add_test(my_test_suite, "edit: after remove", db_edit_after_remove) == NULL) ||
        (CU_add_test(my_test_suite, "edit: rename", db_edit_rename) == NULL) ||
        (CU_add_test(my_test_suite, "edit: rename to taken name", db_edit_rename_name_taken) == NULL) ||
        (CU_add_test(my_test_suite, "edit: rename case only", db_edit_rename_case_only) == NULL) ||
        (CU_add_test(my_test_suite, "edit: rename keeps stock", db_edit_rename_keeps_stock) == NULL) ||
        (CU_add_test(my_test_suite, "edit: rename frees old name", db_edit_rename_old_name_free) == NULL) ||
        (CU_add_test(my_test_suite, "edit: rename chain", db_edit_rename_chain) == NULL) ||
        (CU_add_test(my_test_suite, "edit: rename in names", db_edit_rename_in_names) == NULL) ||
        (CU_add_test(my_test_suite, "edit: rename then remove", db_edit_rename_then_remove) == NULL) ||
        (CU_add_test(my_test_suite, "edit: rename with merch's own name", db_edit_rename_with_merch_own_name) == NULL) ||
        (CU_add_test(my_test_suite, "edit: rename many", db_edit_rename_many) == NULL) ||
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