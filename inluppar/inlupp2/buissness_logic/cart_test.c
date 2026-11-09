#include <CUnit/Basic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"

#define DB_NAME_ID(db) ioopm_get_name_id_ht(db)
#define DB_HT_S(db) ioopm_get_ht_s(db)
#define DB_HT_N(db) ioopm_get_ht_n(db)
#define DB_CARTS(db) ioopm_get_carts(db)
#define SHELF_NAME(s) ioopm_db_get_shelf(s)
#define SHELF_AMOUNT(s) ioopm_db_get_shelf_amount(s)

/*
 * Tests for the shopping cart part of the backend.
 *
 * CONTRACT these tests check. Put the same rules in the doc comments in db.h,
 * and change a test here if you decide differently.
 *
 * ioopm_cart_create                                            (spec 2.1.7)
 *   DB_OK. "The number of the i'th shopping cart created is i, regardless of
 *   how many shopping carts have been removed": ids start at 1 and are never
 *   reused (use next_cart_id, not the size of DB_CARTS(db)).
 *
 * ioopm_cart_destroy                                           (spec 2.1.8)
 *   DB_OK, or CART_DOES_NOT_EXSIST. What the cart held can be added to other
 *   carts again.
 *
 * ioopm_cart_increase_quantity                                 (spec 2.1.9)
 *   DB_OK                  merch was not in the cart; the cart now owns merch_name
 *   DB_OK_NO_MERCH_ADDED   merch already in the cart; quantity raised; caller frees merch_name
 *   CART_DOES_NOT_EXSIST
 *   DB_INVALID_AMOUNT      amount == 0
 *   DB_EMPTY               the store has no merch at all
 *   DB_NO_SUCH_MERCH       no merch with that name in the store
 *   CART_NOT_ENOUGH_STOCK  (quantity in ALL carts, this one included) + amount > stock
 *   On every result except DB_OK the caller still owns merch_name.
 *   "All possible orders in the system must be fulfillable ... if all users go
 *   to checkout at the same time, they should all succeed." Adding to a cart
 *   does not change the stock itself; only checkout does (2.1.7).
 *
 * ioopm_cart_decrease_quantity   (never keeps merch_name)     (spec 2.1.10)
 *   DB_OK                  quantity lowered; at 0 the merch leaves the cart
 *                          ("a mapping from names of items to quantities
 *                          greater than zero", 3.2). Amount 0 is allowed
 *                          ("removes zero or more items") and changes nothing.
 *   CART_DOES_NOT_EXSIST
 *   DB_EMPTY               the cart is empty
 *   DB_NO_SUCH_MERCH       the merch is not in this cart
 *   DB_INVALID_AMOUNT      more than the cart holds
 *
 * ioopm_cart_calculate_cost                                    (spec 2.1.11)
 *   DB_OK and *cost = sum of price * quantity
 *   CART_DOES_NOT_EXSIST
 *   CART_EMPTY
 *
 * ioopm_cart_checkout                                          (spec 2.1.12)
 *   DB_OK                  stock lowered, emptied shelves removed (from the
 *                          merch's list AND ht_n), cart removed
 *   CART_DOES_NOT_EXSIST
 *   CART_EMPTY             nothing happens, the cart still exists
 *
 * Other merch functions, as seen from a cart
 *   ioopm_db_edit_merch    a rename is followed by every cart holding the merch;
 *                          a new price is used by the next calculate_cost (2.1.4)
 *   ioopm_db_remove_item   removes the merch "including all its stock" (2.1.3),
 *                          so it must disappear from every cart too
 *
 * ASSUMPTIONS about the rest of db.h (adjust the helpers if they are wrong)
 *   - ioopm_db_add_item and ioopm_db_edit_merch take ownership of name/desc
 *     on DB_OK; on any other result the caller still owns them.
 *   - ioopm_db_replenish and ioopm_db_remove_item copy or only read their strings.
 *   - ioopm_db_item_stock returns DB_OK or DB_NO_SUCH_MERCH, and *shelves is a
 *     heap array of *size shelf pointers. The caller frees the array, not the
 *     shelves.
 */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/* ---------- helpers ---------- */

static ioopm_data_base_t *new_db(void)
{
    return ioopm_db_create(string_knr_hash, string_eq, int_hash, int_eq);
}

// Adds a merch with no stock. The db owns name/desc on DB_OK.
static void add_merch(ioopm_data_base_t *db, char *name, int price)
{
    char *n = strdup(name);
    char *d = strdup("desc");
    ioopm_db_result_t res = ioopm_db_add_item(db, n, d, price);
    if (res != DB_OK)
    {
        free(n);
        free(d);
    }
    CU_ASSERT_EQUAL_FATAL(res, DB_OK);
}

// Puts amount of name on shelf. The backend copies the strings.
static void restock(ioopm_data_base_t *db, char *name, char *shelf, int amount)
{
    CU_ASSERT_EQUAL_FATAL(ioopm_db_replenish(db, name, shelf, amount), DB_OK);
}

// Renames a merch and sets its price. Same ownership rule as add_merch.
static ioopm_db_result_t edit_merch(ioopm_data_base_t *db, char *old_name, char *new_name, int price)
{
    char *n = strdup(new_name);
    char *d = strdup("desc");
    ioopm_db_result_t res = ioopm_db_edit_merch(db, old_name, n, d, price);
    if (res != DB_OK)
    {
        free(n);
        free(d);
    }
    return res;
}

static bool merch_exists(ioopm_data_base_t *db, char *name)
{
    ioopm_shelf_t **shelves = NULL;
    size_t size = 0;
    size_t total = 0;
    ioopm_db_result_t res = ioopm_db_item_stock(db, name, &shelves, &size, &total);
    free(shelves);
    return res == DB_OK;
}

// Total stock of a merch over all its shelves
static size_t stock_of(ioopm_data_base_t *db, char *name)
{
    ioopm_shelf_t **shelves = NULL;
    size_t size = 0;
    size_t total = 0;
    CU_ASSERT_EQUAL_FATAL(ioopm_db_item_stock(db, name, &shelves, &size, &total), DB_OK);
    free(shelves);
    return total;
}

// Number of shelves a merch is stored on
static size_t shelf_count(ioopm_data_base_t *db, char *name)
{
    ioopm_shelf_t **shelves = NULL;
    size_t size = 0;
    size_t total = 0;
    CU_ASSERT_EQUAL_FATAL(ioopm_db_item_stock(db, name, &shelves, &size, &total), DB_OK);
    free(shelves);
    return size;
}

// How many items of name are on shelf (0 if the merch is not on that shelf)
static size_t amount_on_shelf(ioopm_data_base_t *db, char *name, char *shelf)
{
    ioopm_shelf_t **shelves = NULL;
    size_t size = 0;
    size_t total = 0;
    CU_ASSERT_EQUAL_FATAL(ioopm_db_item_stock(db, name, &shelves, &size, &total), DB_OK);
    size_t amount = 0;
    for (size_t i = 0; i < size; ++i)
    {
        if (strcmp(SHELF_NAME(shelves[i]), shelf) == 0)
        {
            amount = SHELF_AMOUNT(shelves[i]);
        }
    }
    free(shelves);
    return amount;
}

// Whether ht_n still has the shelf. Checks that ht_n stays in sync with the merch lists.
static bool shelf_registered(ioopm_data_base_t *db, char *shelf)
{
    elem_t result = int_elem(0);
    return ioopm_hash_table_lookup(DB_HT_N(db), string_elem(shelf), &result);
}

/*
 * Store used by most tests:
 *   Apple  price 10  A10: 5            stock 5
 *   Pear   price 20  B10: 3, C10: 4    stock 7
 *   Banana price 3   no shelves        stock 0
 */
static ioopm_data_base_t *make_store(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_PTR_NOT_NULL_FATAL(db);
    add_merch(db, "Apple", 10);
    add_merch(db, "Pear", 20);
    add_merch(db, "Banana", 3);
    restock(db, "Apple", "A10", 5);
    restock(db, "Pear", "B10", 3);
    restock(db, "Pear", "C10", 4);
    return db;
}

static void create_carts(ioopm_data_base_t *db, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        CU_ASSERT_EQUAL_FATAL(ioopm_cart_create(db, string_knr_hash, string_eq), DB_OK);
    }
}

// Same ownership rule as ui_add_to_cart: the cart keeps the string only on DB_OK.
// strdup also means every call uses a new buffer, so the cart must compare
// names with strcmp and not by pointer.
static ioopm_db_result_t cart_add(ioopm_data_base_t *db, size_t cart, char *name, size_t amount)
{
    char *n = strdup(name);
    ioopm_db_result_t res = ioopm_cart_increase_quantity(db, cart, n, amount);
    if (res != DB_OK)
    {
        free(n);
    }
    return res;
}

// Decrease never keeps the string. A stack buffer checks that it is compared by content.
static ioopm_db_result_t cart_remove(ioopm_data_base_t *db, size_t cart, char *name, size_t amount)
{
    char buf[64];
    strncpy(buf, name, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    return ioopm_cart_decrease_quantity(db, cart, buf, amount);
}

static bool cart_exists(ioopm_data_base_t *db, size_t cart)
{
    size_t cost = 0;
    return ioopm_cart_calculate_cost(db, cart, &cost) != CART_DOES_NOT_EXSIST;
}

// Asserts DB_OK and the expected cost
static void assert_cost(ioopm_data_base_t *db, size_t cart, size_t expected)
{
    size_t cost = 0;
    CU_ASSERT_EQUAL(ioopm_cart_calculate_cost(db, cart, &cost), DB_OK);
    CU_ASSERT_EQUAL(cost, expected);
}

static void assert_cart_empty(ioopm_data_base_t *db, size_t cart)
{
    size_t cost = 0;
    CU_ASSERT_EQUAL(ioopm_cart_calculate_cost(db, cart, &cost), CART_EMPTY);
}

/* ---------- create ---------- */

void cart_create_first_is_empty(void)
{
    ioopm_data_base_t *db = make_store();
    CU_ASSERT_EQUAL(ioopm_cart_create(db, string_knr_hash, string_eq), DB_OK);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(DB_CARTS(db)), 1);
    assert_cart_empty(db, 1);
    CU_ASSERT_FALSE(cart_exists(db, 0));   // ids start at 1
    CU_ASSERT_FALSE(cart_exists(db, 2));
    ioopm_db_destroy(db);
}

void cart_create_multiple(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 3);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(DB_CARTS(db)), 3);
    CU_ASSERT_TRUE(cart_exists(db, 1));
    CU_ASSERT_TRUE(cart_exists(db, 2));
    CU_ASSERT_TRUE(cart_exists(db, 3));
    CU_ASSERT_FALSE(cart_exists(db, 4));
    ioopm_db_destroy(db);
}

void cart_create_in_empty_store(void)
{
    ioopm_data_base_t *db = new_db();
    CU_ASSERT_EQUAL(ioopm_cart_create(db, string_knr_hash, string_eq), DB_OK);
    assert_cart_empty(db, 1);
    ioopm_db_destroy(db);
}

// The size+1 bug: 1, 2, remove 2, create -> must be 3, not 2 again
void cart_create_id_not_reused_after_removing_last(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 2);
    CU_ASSERT_EQUAL(ioopm_cart_destroy(db, 2), DB_OK);
    create_carts(db, 1);
    CU_ASSERT_FALSE(cart_exists(db, 2));
    CU_ASSERT_TRUE(cart_exists(db, 3));
    printf("TEST");
    ioopm_db_destroy(db);
}

// The size+1 bug, overwrite version: 1, 2, 3, remove 2, create -> size+1 = 3 would overwrite cart 3
void cart_create_id_not_reused_after_removing_middle(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 3);
    CU_ASSERT_EQUAL(cart_add(db, 3, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(ioopm_cart_destroy(db, 2), DB_OK);
    create_carts(db, 1);

    CU_ASSERT_EQUAL(ioopm_hash_table_size(DB_CARTS(db)), 3);
    CU_ASSERT_FALSE(cart_exists(db, 2));
    assert_cost(db, 3, 20);       // cart 3 kept its contents
    assert_cart_empty(db, 4);     // the new cart got id 4
    ioopm_db_destroy(db);
}

void cart_create_id_not_reused_after_checkout(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), DB_OK);
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 1), DB_OK);
    create_carts(db, 1);
    CU_ASSERT_FALSE(cart_exists(db, 1));
    assert_cart_empty(db, 2);
    ioopm_db_destroy(db);
}

/* ---------- destroy ---------- */

void cart_destroy_empty_cart(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(ioopm_cart_destroy(db, 1), DB_OK);
    CU_ASSERT_FALSE(cart_exists(db, 1));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(DB_CARTS(db)), 0);
    ioopm_db_destroy(db);
}

void cart_destroy_nonexistent(void)
{
    ioopm_data_base_t *db = make_store();
    CU_ASSERT_EQUAL(ioopm_cart_destroy(db, 1), CART_DOES_NOT_EXSIST);  // no carts at all
    create_carts(db, 1);
    CU_ASSERT_EQUAL(ioopm_cart_destroy(db, 0), CART_DOES_NOT_EXSIST);
    CU_ASSERT_EQUAL(ioopm_cart_destroy(db, 2), CART_DOES_NOT_EXSIST);
    CU_ASSERT_EQUAL(ioopm_cart_destroy(db, 999), CART_DOES_NOT_EXSIST);
    CU_ASSERT_TRUE(cart_exists(db, 1));   // failed destroys touched nothing
    ioopm_db_destroy(db);
}

void cart_destroy_twice(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(ioopm_cart_destroy(db, 1), DB_OK);
    CU_ASSERT_EQUAL(ioopm_cart_destroy(db, 1), CART_DOES_NOT_EXSIST);
    ioopm_db_destroy(db);
}

// Run under valgrind: the cart's names and table must be freed
void cart_destroy_with_items(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Pear", 3), DB_OK);
    CU_ASSERT_EQUAL(ioopm_cart_destroy(db, 1), DB_OK);
    CU_ASSERT_FALSE(cart_exists(db, 1));
    CU_ASSERT_EQUAL(stock_of(db, "Apple"), 5);   // removing a cart sells nothing
    CU_ASSERT_EQUAL(stock_of(db, "Pear"), 7);
    ioopm_db_destroy(db);
}

void cart_destroy_releases_reservation(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 2);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 5), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Apple", 1), CART_NOT_ENOUGH_STOCK);
    CU_ASSERT_EQUAL(ioopm_cart_destroy(db, 1), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Apple", 5), DB_OK);
    ioopm_db_destroy(db);
}

void cart_destroy_leaves_other_carts(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 2);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Pear", 2), DB_OK);
    CU_ASSERT_EQUAL(ioopm_cart_destroy(db, 1), DB_OK);
    assert_cost(db, 2, 40);
    ioopm_db_destroy(db);
}

/* ---------- increase quantity ---------- */

void cart_add_one(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), DB_OK);
    assert_cost(db, 1, 10);
    ioopm_db_destroy(db);
}

void cart_add_nonexistent_cart(void)
{
    ioopm_data_base_t *db = make_store();
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), CART_DOES_NOT_EXSIST);
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 0, "Apple", 1), CART_DOES_NOT_EXSIST);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Apple", 1), CART_DOES_NOT_EXSIST);
    assert_cart_empty(db, 1);
    ioopm_db_destroy(db);
}

void cart_add_nonexistent_merch(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Kiwi", 1), DB_NO_SUCH_MERCH);
    CU_ASSERT_EQUAL(cart_add(db, 1, "", 1), DB_NO_SUCH_MERCH);
    assert_cart_empty(db, 1);
    ioopm_db_destroy(db);
}

void cart_add_case_sensitive(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "apple", 1), DB_NO_SUCH_MERCH);
    CU_ASSERT_EQUAL(cart_add(db, 1, "APPLE", 1), DB_NO_SUCH_MERCH);
    assert_cart_empty(db, 1);
    ioopm_db_destroy(db);
}

void cart_add_empty_store(void)
{
    ioopm_data_base_t *db = new_db();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), DB_EMPTY);
    assert_cart_empty(db, 1);
    ioopm_db_destroy(db);
}

void cart_add_zero_amount(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 0), DB_INVALID_AMOUNT);
    assert_cart_empty(db, 1);
    ioopm_db_destroy(db);
}

void cart_add_merch_without_stock(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);    
    CU_ASSERT_EQUAL(cart_add(db, 1, "Banana", 1), CART_NOT_ENOUGH_STOCK);
    assert_cart_empty(db, 1);
    ioopm_db_destroy(db);
}

void cart_add_exactly_all_stock(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 5), DB_OK);   // one shelf
    CU_ASSERT_EQUAL(cart_add(db, 1, "Pear", 7), DB_OK);    // stock spread over two shelves
    assert_cost(db, 1, 5 * 10 + 7 * 20);
    ioopm_db_destroy(db);
}

void cart_add_more_than_stock(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 6), CART_NOT_ENOUGH_STOCK);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Pear", 8), CART_NOT_ENOUGH_STOCK);
    assert_cart_empty(db, 1);
    ioopm_db_destroy(db);
}

void cart_add_huge_amount(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", (size_t) -1), CART_NOT_ENOUGH_STOCK);
    assert_cart_empty(db, 1);
    ioopm_db_destroy(db);
}

// Also checks the names are compared with strcmp: every cart_add uses a new buffer
void cart_add_same_merch_twice(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), DB_OK_NO_MERCH_ADDED);
    assert_cost(db, 1, 30);
    ioopm_db_destroy(db);
}

void cart_add_same_merch_twice_over_stock(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 3), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 3), CART_NOT_ENOUGH_STOCK);
    assert_cost(db, 1, 30);   // first add kept, second ignored
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK_NO_MERCH_ADDED);
    assert_cost(db, 1, 50);
    ioopm_db_destroy(db);
}

// "All orders must be fulfillable": reservations count across all carts
void cart_add_carts_share_stock(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 2);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 3), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Apple", 1), CART_NOT_ENOUGH_STOCK);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), CART_NOT_ENOUGH_STOCK);
    assert_cost(db, 1, 30);
    assert_cost(db, 2, 20);
    ioopm_db_destroy(db);
}

// The spec's own example: carts may hold 12 of X in total only if the stock is at least 12
void cart_add_spec_example_twelve(void)
{
    ioopm_data_base_t *db = make_store();
    restock(db, "Banana", "D10", 12);
    create_carts(db, 3);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Banana", 5), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Banana", 7), DB_OK);                // 12 in carts, 12 in stock
    CU_ASSERT_EQUAL(cart_add(db, 3, "Banana", 1), CART_NOT_ENOUGH_STOCK); // 13 would be too many
    CU_ASSERT_EQUAL(cart_add(db, 1, "Banana", 1), CART_NOT_ENOUGH_STOCK);
    assert_cart_empty(db, 3);
    ioopm_db_destroy(db);
}

void cart_add_does_not_change_stock(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 4), DB_OK);
    CU_ASSERT_EQUAL(stock_of(db, "Apple"), 5);
    CU_ASSERT_EQUAL(shelf_count(db, "Apple"), 1);
    CU_ASSERT_EQUAL(amount_on_shelf(db, "Apple", "A10"), 5);
    CU_ASSERT_TRUE(shelf_registered(db, "A10"));
    ioopm_db_destroy(db);
}

void cart_add_multiple_merch(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Pear", 2), DB_OK);
    assert_cost(db, 1, 10 + 40);
    ioopm_db_destroy(db);
}

void cart_add_after_replenish(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 5), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), CART_NOT_ENOUGH_STOCK);
    restock(db, "Apple", "A10", 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), DB_OK_NO_MERCH_ADDED);
    restock(db, "Banana", "D10", 2);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Banana", 2), DB_OK);
    assert_cost(db, 1, 60 + 6);
    ioopm_db_destroy(db);
}

/* ---------- decrease quantity ---------- */

void cart_remove_partial(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 3), DB_OK);
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Apple", 1), DB_OK);
    assert_cost(db, 1, 20);
    ioopm_db_destroy(db);
}

void cart_remove_all_of_one_merch(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 3), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Pear", 1), DB_OK);
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Apple", 3), DB_OK);
    assert_cost(db, 1, 20);
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Apple", 1), DB_NO_SUCH_MERCH);   // the entry is gone, not left at 0
    ioopm_db_destroy(db);
}

void cart_remove_last_merch_makes_cart_empty(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Apple", 2), DB_OK);
    assert_cart_empty(db, 1);            // empty, but still exists
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Apple", 1), DB_NO_SUCH_MERCH)
    ioopm_db_destroy(db);
}

// If a 0-quantity entry were left behind, the add would return DB_OK_NO_MERCH_ADDED
void cart_remove_all_then_add_again(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), DB_OK);
    assert_cost(db, 1, 10);
    ioopm_db_destroy(db);
}

void cart_remove_more_than_in_cart(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Apple", 3), DB_INVALID_AMOUNT);
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Apple", (size_t) -1), DB_INVALID_AMOUNT);
    assert_cost(db, 1, 20);   // unchanged
    ioopm_db_destroy(db);
}

// "Removes zero or more items": 0 is a valid amount that changes nothing
void cart_remove_zero(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Apple", 0), DB_OK);
    assert_cost(db, 1, 20);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), DB_OK_NO_MERCH_ADDED);   // still in the cart
    ioopm_db_destroy(db);
}

void cart_remove_nonexistent_cart(void)
{
    ioopm_data_base_t *db = make_store();
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Apple", 1), CART_DOES_NOT_EXSIST);
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_remove(db, 0, "Apple", 1), CART_DOES_NOT_EXSIST);
    CU_ASSERT_EQUAL(cart_remove(db, 2, "Apple", 1), CART_DOES_NOT_EXSIST);
    ioopm_db_destroy(db);
}

void cart_remove_merch_not_in_cart(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Pear", 1), DB_NO_SUCH_MERCH);   // in store, not in cart
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Kiwi", 1), DB_NO_SUCH_MERCH);   // not in store at all
    CU_ASSERT_EQUAL(cart_remove(db, 1, "apple", 1), DB_NO_SUCH_MERCH);  // case sensitive
    assert_cost(db, 1, 20);
    ioopm_db_destroy(db);
}

void cart_remove_only_from_given_cart(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 2);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Apple", 1), DB_OK);
    assert_cost(db, 1, 10);
    assert_cost(db, 2, 20);
    ioopm_db_destroy(db);
}

void cart_remove_from_empty_cart(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Apple", 1), DB_NO_SUCH_MERCH);
    ioopm_db_destroy(db);
}

void cart_remove_releases_reservation(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 2);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 5), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Apple", 2), CART_NOT_ENOUGH_STOCK);
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Apple", 1), CART_NOT_ENOUGH_STOCK);
    ioopm_db_destroy(db);
}

void cart_remove_does_not_change_stock(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Pear", 5), DB_OK);
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Pear", 5), DB_OK);
    CU_ASSERT_EQUAL(stock_of(db, "Pear"), 7);
    CU_ASSERT_EQUAL(shelf_count(db, "Pear"), 2);
    ioopm_db_destroy(db);
}

/* ---------- calculate cost ---------- */

void cart_cost_nonexistent(void)
{
    ioopm_data_base_t *db = make_store();
    size_t cost = 0;
    CU_ASSERT_EQUAL(ioopm_cart_calculate_cost(db, 1, &cost), CART_DOES_NOT_EXSIST);
    create_carts(db, 1);
    CU_ASSERT_EQUAL(ioopm_cart_calculate_cost(db, 0, &cost), CART_DOES_NOT_EXSIST);
    CU_ASSERT_EQUAL(ioopm_cart_calculate_cost(db, 2, &cost), CART_DOES_NOT_EXSIST);
    ioopm_db_destroy(db);
}

void cart_cost_empty(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    assert_cart_empty(db, 1);
    ioopm_db_destroy(db);
}

void cart_cost_one_merch_many(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Pear", 7), DB_OK);
    assert_cost(db, 1, 140);
    ioopm_db_destroy(db);
}

void cart_cost_multiple_merch(void)
{
    ioopm_data_base_t *db = make_store();
    restock(db, "Banana", "D10", 10);
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Pear", 3), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Banana", 10), DB_OK);
    assert_cost(db, 1, 2 * 10 + 3 * 20 + 10 * 3);
    ioopm_db_destroy(db);
}

// The spec's example: 2 x 50 + 8 x 3 = 124
void cart_cost_spec_example(void)
{
    ioopm_data_base_t *db = make_store();
    add_merch(db, "Melon", 50);
    restock(db, "Melon", "E10", 2);
    restock(db, "Banana", "D10", 8);
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Melon", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Banana", 8), DB_OK);
    assert_cost(db, 1, 124);
    ioopm_db_destroy(db);
}

void cart_cost_carts_independent(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 3);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Pear", 1), DB_OK);
    assert_cost(db, 1, 10);
    assert_cost(db, 2, 20);
    assert_cart_empty(db, 3);
    ioopm_db_destroy(db);
}

void cart_cost_changes_nothing(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK);
    assert_cost(db, 1, 20);
    assert_cost(db, 1, 20);
    CU_ASSERT_EQUAL(stock_of(db, "Apple"), 5);
    CU_ASSERT_TRUE(cart_exists(db, 1));
    ioopm_db_destroy(db);
}

/* ---------- checkout ---------- */

void cart_checkout_nonexistent(void)
{
    ioopm_data_base_t *db = make_store();
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 1), CART_DOES_NOT_EXSIST);
    create_carts(db, 1);
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 0), CART_DOES_NOT_EXSIST);
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 2), CART_DOES_NOT_EXSIST);
    ioopm_db_destroy(db);
}

void cart_checkout_empty_cart(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 1), CART_EMPTY);
    CU_ASSERT_TRUE(cart_exists(db, 1));
    ioopm_db_destroy(db);
}

void cart_checkout_lowers_stock(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 1), DB_OK);
    CU_ASSERT_EQUAL(stock_of(db, "Apple"), 3);
    CU_ASSERT_EQUAL(shelf_count(db, "Apple"), 1);
    CU_ASSERT_EQUAL(amount_on_shelf(db, "Apple", "A10"), 3);
    CU_ASSERT_TRUE(shelf_registered(db, "A10"));
    CU_ASSERT_EQUAL(stock_of(db, "Pear"), 7);   // other merch untouched
    ioopm_db_destroy(db);
}

void cart_checkout_removes_cart(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 2);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), DB_OK);
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 1), DB_OK);
    CU_ASSERT_FALSE(cart_exists(db, 1));
    CU_ASSERT_TRUE(cart_exists(db, 2));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(DB_CARTS(db)), 1);
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 1), CART_DOES_NOT_EXSIST);
    ioopm_db_destroy(db);
}

// "We do not keep track of empty storage locations"
void cart_checkout_empties_shelf(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 1), DB_OK); // segfault
    CU_ASSERT_TRUE(merch_exists(db, "Apple"));   // merch stays, only the shelf goes
    CU_ASSERT_EQUAL(stock_of(db, "Apple"), 0);
    CU_ASSERT_EQUAL(shelf_count(db, "Apple"), 0);
    CU_ASSERT_FALSE(shelf_registered(db, "A10"));
    ioopm_db_destroy(db);
}

void cart_checkout_across_shelves(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Pear", 6), DB_OK);   // B10: 3, C10: 4
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 1), DB_OK);
    CU_ASSERT_EQUAL(stock_of(db, "Pear"), 1);
    CU_ASSERT_EQUAL(shelf_count(db, "Pear"), 1);
    // Exactly one shelf is left with 1 item; which one depends on the order you take from
    CU_ASSERT_EQUAL(amount_on_shelf(db, "Pear", "B10") + amount_on_shelf(db, "Pear", "C10"), 1);
    CU_ASSERT_TRUE(shelf_registered(db, "B10") != shelf_registered(db, "C10"));
    ioopm_db_destroy(db);
}

void cart_checkout_all_across_shelves(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Pear", 7), DB_OK);
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 1), DB_OK);
    CU_ASSERT_EQUAL(stock_of(db, "Pear"), 0);
    CU_ASSERT_EQUAL(shelf_count(db, "Pear"), 0);
    CU_ASSERT_FALSE(shelf_registered(db, "B10"));
    CU_ASSERT_FALSE(shelf_registered(db, "C10"));
    ioopm_db_destroy(db);
}

void cart_checkout_multiple_merch(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Pear", 2), DB_OK);
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 1), DB_OK);
    CU_ASSERT_EQUAL(stock_of(db, "Apple"), 4);
    CU_ASSERT_EQUAL(stock_of(db, "Pear"), 5);
    CU_ASSERT_EQUAL(stock_of(db, "Banana"), 0);
    ioopm_db_destroy(db);
}

// Checkout must lower stock AND the reservation, so the other cart's claim still holds
void cart_checkout_keeps_other_reservations(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 3);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Apple", 3), DB_OK);
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 1), DB_OK);

    CU_ASSERT_EQUAL(stock_of(db, "Apple"), 3);
    assert_cost(db, 2, 30);
    CU_ASSERT_EQUAL(cart_add(db, 3, "Apple", 1), CART_NOT_ENOUGH_STOCK);  // 3 left, 3 reserved

    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 2), DB_OK);
    CU_ASSERT_EQUAL(stock_of(db, "Apple"), 0);
    CU_ASSERT_FALSE(shelf_registered(db, "A10"));
    ioopm_db_destroy(db);
}
// "If all users go to checkout at the same time, they should all succeed"
void cart_checkout_all_carts_succeed(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 3);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Pear", 3), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Pear", 4), DB_OK);   // all 7 pears are in carts
    CU_ASSERT_EQUAL(cart_add(db, 3, "Apple", 5), DB_OK);  // all 5 apples are in carts
    CU_ASSERT_EQUAL(cart_add(db, 3, "Pear", 1), CART_NOT_ENOUGH_STOCK);

    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 2), DB_OK);
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 1), DB_OK);
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 3), DB_OK);
    CU_ASSERT_EQUAL(stock_of(db, "Pear"), 0);
    CU_ASSERT_EQUAL(stock_of(db, "Apple"), 0);
    CU_ASSERT_FALSE(shelf_registered(db, "A10"));
    CU_ASSERT_FALSE(shelf_registered(db, "B10"));
    CU_ASSERT_FALSE(shelf_registered(db, "C10"));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(DB_CARTS(db)), 0);
    ioopm_db_destroy(db);
}

// A shelf emptied by checkout is free, so another merch may use it
void cart_checkout_frees_shelf_for_other_merch(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 1), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_replenish(db, "Banana", "A10", 4), DB_OK);
    CU_ASSERT_EQUAL(stock_of(db, "Banana"), 4);
    ioopm_db_destroy(db);
}   

void cart_checkout_sold_out_then_restock(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 2);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 5), DB_OK);
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 1), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Apple", 1), CART_NOT_ENOUGH_STOCK);
    restock(db, "Apple", "A10", 2);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 2), DB_OK);
    CU_ASSERT_EQUAL(stock_of(db, "Apple"), 0);
    ioopm_db_destroy(db);
}

/* ---------- other merch functions, seen from a cart ---------- */

void cart_merch_renamed(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(edit_merch(db, "Apple", "Green apple", 10), DB_OK);

    assert_cost(db, 1, 20);                                                 // still in the cart
    CU_ASSERT_EQUAL(cart_add(db, 1, "Green apple", 1), DB_OK_NO_MERCH_ADDED); // under the new name
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), DB_NO_SUCH_MERCH);         // old name is gone
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Green apple", 3), DB_OK);
    assert_cart_empty(db, 1);
    ioopm_db_destroy(db);
}

void cart_merch_price_changed(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(edit_merch(db, "Apple", "Apple", 15), DB_OK);
    assert_cost(db, 1, 30);   // the cart uses the current price
    ioopm_db_destroy(db);
}

void cart_merch_removed_from_store(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 2);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Pear", 1), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Apple", 1), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Apple"), DB_OK);

    assert_cost(db, 1, 20);                                           // only the pear is left
    assert_cart_empty(db, 2);                                         // apple was all it had
    CU_ASSERT_EQUAL(cart_remove(db, 1, "Apple", 1), DB_NO_SUCH_MERCH);
    CU_ASSERT_EQUAL(ioopm_cart_checkout(db, 1), DB_OK);               // must not touch the freed merch
    CU_ASSERT_EQUAL(stock_of(db, "Pear"), 6);
    ioopm_db_destroy(db);
}

// Edit "does not affect its stock", so what carts hold still counts after a rename
void cart_merch_renamed_keeps_limit(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 2);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 4), DB_OK);
    CU_ASSERT_EQUAL(edit_merch(db, "Apple", "Green apple", 10), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Green apple", 2), CART_NOT_ENOUGH_STOCK);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Green apple", 1), DB_OK);
    ioopm_db_destroy(db);
}

// A new merch with the old name is a different merch, not the one in the cart
void cart_merch_removed_and_readded(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(ioopm_db_remove_item(db, "Apple"), DB_OK);
    add_merch(db, "Apple", 99);
    assert_cart_empty(db, 1);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), CART_NOT_ENOUGH_STOCK);   // new apple has no stock
    ioopm_db_destroy(db);
}

/* ---------- whole database ---------- */

// Only meaningful under valgrind: db_destroy must free carts and their contents
void cart_db_destroy_with_full_carts(void)
{
    ioopm_data_base_t *db = make_store();
    create_carts(db, 3);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Pear", 2), DB_OK);
    CU_ASSERT_EQUAL(cart_add(db, 1, "Apple", 1), DB_OK_NO_MERCH_ADDED);
    CU_ASSERT_EQUAL(cart_add(db, 2, "Pear", 1), DB_OK);
    ioopm_db_destroy(db);
}

void cart_many_carts(void)
{
    ioopm_data_base_t *db = make_store();
    restock(db, "Banana", "D10", 100);
    create_carts(db, 50);   // enough to make the carts table resize
    for (size_t i = 1; i <= 50; ++i)
    {
        CU_ASSERT_EQUAL(cart_add(db, i, "Banana", 2), DB_OK);
    }
    CU_ASSERT_EQUAL(cart_add(db, 1, "Banana", 1), CART_NOT_ENOUGH_STOCK);
    for (size_t i = 1; i <= 50; ++i)
    {
        assert_cost(db, i, 6);
    }
    ioopm_db_destroy(db);
}

/* ---------- main ---------- */

int main(void)
{
    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("cart test suite", init_suite, clean_suite);
    if (suite == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
        (CU_add_test(suite, "create: first cart is empty", cart_create_first_is_empty) == NULL) ||
        (CU_add_test(suite, "create: multiple", cart_create_multiple) == NULL) ||
        (CU_add_test(suite, "create: in empty store", cart_create_in_empty_store) == NULL) ||
        (CU_add_test(suite, "create: id not reused (last removed)", cart_create_id_not_reused_after_removing_last) == NULL) ||
        (CU_add_test(suite, "create: id not reused (middle removed)", cart_create_id_not_reused_after_removing_middle) == NULL) ||
        (CU_add_test(suite, "create: id not reused after checkout", cart_create_id_not_reused_after_checkout) == NULL) ||

        (CU_add_test(suite, "destroy: empty cart", cart_destroy_empty_cart) == NULL) ||
        (CU_add_test(suite, "destroy: nonexistent", cart_destroy_nonexistent) == NULL) ||
        (CU_add_test(suite, "destroy: twice", cart_destroy_twice) == NULL) ||
        (CU_add_test(suite, "destroy: with items", cart_destroy_with_items) == NULL) ||
        (CU_add_test(suite, "destroy: releases reservation", cart_destroy_releases_reservation) == NULL) ||
        (CU_add_test(suite, "destroy: leaves other carts", cart_destroy_leaves_other_carts) == NULL) ||

        (CU_add_test(suite, "add: one", cart_add_one) == NULL) ||
        (CU_add_test(suite, "add: nonexistent cart", cart_add_nonexistent_cart) == NULL) ||
        (CU_add_test(suite, "add: nonexistent merch", cart_add_nonexistent_merch) == NULL) ||
        (CU_add_test(suite, "add: case sensitive", cart_add_case_sensitive) == NULL) ||
        (CU_add_test(suite, "add: empty store", cart_add_empty_store) == NULL) ||
        (CU_add_test(suite, "add: zero amount", cart_add_zero_amount) == NULL) ||
        (CU_add_test(suite, "add: merch without stock", cart_add_merch_without_stock) == NULL) ||
        (CU_add_test(suite, "add: exactly all stock", cart_add_exactly_all_stock) == NULL) ||
        (CU_add_test(suite, "add: more than stock", cart_add_more_than_stock) == NULL) ||
        (CU_add_test(suite, "add: huge amount", cart_add_huge_amount) == NULL) ||
        (CU_add_test(suite, "add: same merch twice", cart_add_same_merch_twice) == NULL) ||
        (CU_add_test(suite, "add: same merch twice over stock", cart_add_same_merch_twice_over_stock) == NULL) ||
        (CU_add_test(suite, "add: carts share stock", cart_add_carts_share_stock) == NULL) ||
        (CU_add_test(suite, "add: spec example (12 items)", cart_add_spec_example_twelve) == NULL) ||
        (CU_add_test(suite, "add: does not change stock", cart_add_does_not_change_stock) == NULL) ||
        (CU_add_test(suite, "add: multiple merch", cart_add_multiple_merch) == NULL) ||
        (CU_add_test(suite, "add: after replenish", cart_add_after_replenish) == NULL) ||

        (CU_add_test(suite, "remove: partial", cart_remove_partial) == NULL) ||
        (CU_add_test(suite, "remove: all of one merch", cart_remove_all_of_one_merch) == NULL) ||
        (CU_add_test(suite, "remove: last merch empties cart", cart_remove_last_merch_makes_cart_empty) == NULL) ||
        (CU_add_test(suite, "remove: all then add again", cart_remove_all_then_add_again) == NULL) ||
        (CU_add_test(suite, "remove: more than in cart", cart_remove_more_than_in_cart) == NULL) ||
        (CU_add_test(suite, "remove: zero", cart_remove_zero) == NULL) ||
        (CU_add_test(suite, "remove: nonexistent cart", cart_remove_nonexistent_cart) == NULL) ||
        (CU_add_test(suite, "remove: merch not in cart", cart_remove_merch_not_in_cart) == NULL) ||
        (CU_add_test(suite, "remove: only from given cart", cart_remove_only_from_given_cart) == NULL) ||
        (CU_add_test(suite, "remove: from empty cart", cart_remove_from_empty_cart) == NULL) ||
        (CU_add_test(suite, "remove: releases reservation", cart_remove_releases_reservation) == NULL) ||
        (CU_add_test(suite, "remove: does not change stock", cart_remove_does_not_change_stock) == NULL) ||

        (CU_add_test(suite, "cost: nonexistent", cart_cost_nonexistent) == NULL) ||
        (CU_add_test(suite, "cost: empty", cart_cost_empty) == NULL) ||
        (CU_add_test(suite, "cost: one merch many", cart_cost_one_merch_many) == NULL) ||
        (CU_add_test(suite, "cost: multiple merch", cart_cost_multiple_merch) == NULL) ||
        (CU_add_test(suite, "cost: spec example (124)", cart_cost_spec_example) == NULL) ||
        (CU_add_test(suite, "cost: carts independent", cart_cost_carts_independent) == NULL) ||
        (CU_add_test(suite, "cost: changes nothing", cart_cost_changes_nothing) == NULL) ||

        (CU_add_test(suite, "checkout: nonexistent", cart_checkout_nonexistent) == NULL) ||
        (CU_add_test(suite, "checkout: empty cart", cart_checkout_empty_cart) == NULL) ||
        (CU_add_test(suite, "checkout: lowers stock", cart_checkout_lowers_stock) == NULL) ||
        (CU_add_test(suite, "checkout: removes cart", cart_checkout_removes_cart) == NULL) ||
        (CU_add_test(suite, "checkout: empties shelf", cart_checkout_empties_shelf) == NULL) ||
        (CU_add_test(suite, "checkout: across shelves", cart_checkout_across_shelves) == NULL) ||
        (CU_add_test(suite, "checkout: all across shelves", cart_checkout_all_across_shelves) == NULL) ||
        (CU_add_test(suite, "checkout: multiple merch", cart_checkout_multiple_merch) == NULL) ||
        (CU_add_test(suite, "checkout: keeps other reservations", cart_checkout_keeps_other_reservations) == NULL) ||
        (CU_add_test(suite, "checkout: all carts succeed", cart_checkout_all_carts_succeed) == NULL) ||
        (CU_add_test(suite, "checkout: frees shelf for other merch", cart_checkout_frees_shelf_for_other_merch) == NULL) ||
        (CU_add_test(suite, "checkout: sold out then restock", cart_checkout_sold_out_then_restock) == NULL) ||

        (CU_add_test(suite, "merch renamed", cart_merch_renamed) == NULL) ||
        (CU_add_test(suite, "merch renamed keeps limit", cart_merch_renamed_keeps_limit) == NULL) ||
        (CU_add_test(suite, "merch price changed", cart_merch_price_changed) == NULL) ||
        (CU_add_test(suite, "merch removed from store", cart_merch_removed_from_store) == NULL) ||
        (CU_add_test(suite, "merch removed and re-added", cart_merch_removed_and_readded) == NULL) ||

        (CU_add_test(suite, "db destroy with full carts", cart_db_destroy_with_full_carts) == NULL) ||
        // (CU_add_test(suite, "many carts", cart_many_carts) == NULL) ||
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