#include <CUnit/Basic.h>
#include "hash_table.h"
#include <stdbool.h>
#include <assert.h>
#include "common.h"

int init_suite(void) {
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void) {
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}

void test_create_destroy(void)
{
   ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq);
   CU_ASSERT_PTR_NOT_NULL(ht);
   ioopm_hash_table_destroy(ht);
}

void test_create_destroy_destroy_with_entries(void)
{
   ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq);

     
  elem_t key1 = string_elem("a");
  elem_t value1 = int_elem(1); 
  ioopm_hash_table_insert(ht, key1, value1);

  elem_t key2 = string_elem("r");
  elem_t value2 = int_elem(2);
  ioopm_hash_table_insert(ht, key2, value2);

  elem_t key3 = string_elem("at");
  elem_t value3 = int_elem(3);
  ioopm_hash_table_insert(ht, key3, value3);

  ioopm_hash_table_destroy(ht); 
  // If no mem leaks, then this function is working
}

void test_lookup_entry_exsist(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq);
  elem_t key = string_elem("a");
  elem_t value = int_elem(123);

  elem_t result = int_elem(0); 
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, 0);
  
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, 123); 

  ioopm_hash_table_destroy(ht);
}

void test_lookup_entry_no_exsist(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq);
  elem_t key_dummy = string_elem("a");
  elem_t value_dummy = int_elem(1);
  elem_t key = string_elem("b");
  
  elem_t result = int_elem(0); 
  ioopm_hash_table_insert(ht, key_dummy, value_dummy);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key_dummy, &result));
  CU_ASSERT_EQUAL(result.i, value_dummy.i);

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, 0); 

  ioopm_hash_table_destroy(ht);
}

void test_insert_once(void)
{
  // create new hash table
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq);

  elem_t key = string_elem("abc");
  elem_t value = int_elem(123);

  // check that key is not in ht
  elem_t result = int_elem(0);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, 0);

  // insert key-value pair and check that the mapping exists
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, value.i);

  // destroy hash table
  ioopm_hash_table_destroy(ht);
}

void test_insert_twice_same(void)
{
  // create new hash table
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq);

  elem_t key = string_elem("abc");
  elem_t value = int_elem(123);
  elem_t key_dup = string_elem("abc");
  elem_t value_dup = int_elem(543);

  // check that key is not in ht
  elem_t result = int_elem(0);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, 0);
  

  // insert key-value pair and check that the mapping exists
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, value.i);

  ioopm_hash_table_insert(ht, key_dup, value_dup);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key_dup, &result));
  CU_ASSERT_EQUAL(result.i, value_dup.i); 

  // destroy hash table
  ioopm_hash_table_destroy(ht);
}

// Remove the first element of a linked list of size two
void test_remove_entry_first(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq); 
  
  elem_t key1 = string_elem("a");
  elem_t value1 = int_elem(1); 
  ioopm_hash_table_insert(ht, key1, value1);

  elem_t key2 = string_elem("r");
  elem_t value2 = int_elem(2);
  ioopm_hash_table_insert(ht, key2, value2);
  
  // insert key-value linked list and check that the mapping exsits
  elem_t result = int_elem(0); 
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result.i, value1.i);
  
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result));
  CU_ASSERT_EQUAL(result.i, value2.i);


  // remove the value and check if the correct value got removed
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key1, &result));
  CU_ASSERT_EQUAL(result.i, value1.i);

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result.i, 0); 

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result));
  CU_ASSERT_EQUAL(result.i, value2.i); 

  ioopm_hash_table_destroy(ht);
}

// Remove the midlle most element
void test_remove_entry_middle(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq); 
  
  elem_t key1 = string_elem("a");
  elem_t value1 = int_elem(1); 
  ioopm_hash_table_insert(ht, key1, value1);

  elem_t key2 = string_elem("r");
  elem_t value2 = int_elem(2);
  ioopm_hash_table_insert(ht, key2, value2);

  elem_t key3 = string_elem("at");
  elem_t value3 = int_elem(3);
  ioopm_hash_table_insert(ht, key3, value3);

  
  // insert key-value linked list and check that the mapping exsits
  elem_t result = int_elem(0); 
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result.i, value1.i);
  
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result));
  CU_ASSERT_EQUAL(result.i, value2.i);
  
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key3, &result));
  CU_ASSERT_EQUAL(result.i, value3.i);

  // remove the value and check if the correct value got removed
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key2, &result));
  CU_ASSERT_EQUAL(result.i, value2.i);

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key2, &result));
  CU_ASSERT_EQUAL(result.i, 0); 

  // check if other entries are still there
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result.i, value1.i); 

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key3, &result));
  CU_ASSERT_EQUAL(result.i, value3.i); 

  ioopm_hash_table_destroy(ht);
}

// Remove the last element of a linked list of size two
void test_remove_entry_one(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq); 
  
  elem_t key1 = string_elem("a");
  elem_t value1 = int_elem(1); 
  ioopm_hash_table_insert(ht, key1, value1);
  
  // insert key-value linked list and check that the mapping exsits
  elem_t result = int_elem(0); 
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result.i, value1.i);


  // remove the value and check if the correct value got removed
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key1, &result));
  CU_ASSERT_EQUAL(result.i, value1.i);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result.i, 0);

  ioopm_hash_table_destroy(ht);
}


// Remove the last element of a linked list of size two
void test_remove_entry_last(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq); 

  elem_t key1 = string_elem("a");
  elem_t value1 = int_elem(1); 
  ioopm_hash_table_insert(ht, key1, value1);

  elem_t key2 = string_elem("r");
  elem_t value2 = int_elem(2);
  ioopm_hash_table_insert(ht, key2, value2);
  
  // insert key-value linked list and check that the mapping exsits
  elem_t result = int_elem(0); 
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result.i, value1.i);
  
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result));
  CU_ASSERT_EQUAL(result.i, value2.i);


  // remove the value and check if the correct value got removed
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key2, &result));
  CU_ASSERT_EQUAL(result.i, value2.i);

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key2, &result));
  CU_ASSERT_EQUAL(result.i, 0);

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result.i, value1.i); 

  ioopm_hash_table_destroy(ht);
}

// Try to remove an element in an empty table
void test_remove_entry_empty_table(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq); 
  elem_t key = string_elem("a");
  
  elem_t result = int_elem(0); 

  // Check if key exsists in ht
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, 0);
  
  // Try to remove that key
  CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, 0);

  ioopm_hash_table_destroy(ht);
}

void test_remove_missing_key_filled_table(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq); 
  
  elem_t key_missing = string_elem("a");

  elem_t key2 = string_elem("r");
  elem_t value2 = int_elem(2);
  ioopm_hash_table_insert(ht, key2, value2);

  elem_t key3 = string_elem("at");
  elem_t value3 = int_elem(3);
  ioopm_hash_table_insert(ht, key3, value3);

  // Try remove with missing key
  elem_t result = int_elem(0); 
  CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, key_missing, &result));
  CU_ASSERT_EQUAL(result.i, 0); 

  // Check for survivors
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result));
  CU_ASSERT_EQUAL(result.i, value2.i);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key3, &result));
  CU_ASSERT_EQUAL(result.i, value3.i);

  ioopm_hash_table_destroy(ht); 
}

// if we dont find key, set result to 0
// if we find key, set result to the keys value

void test_has_key_empty_table(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq);

  elem_t key = string_elem("a"); 


  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, key));


  ioopm_hash_table_destroy(ht);
}

void test_has_key(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq);

  elem_t key = string_elem("a"); 
  elem_t value = int_elem(1);
  ioopm_hash_table_insert(ht, key, value);


  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key));

  ioopm_hash_table_destroy(ht);
}

void test_has_multiple_keys(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq);

  elem_t key1 = string_elem("a"); 
  elem_t value1 = int_elem(1);
  ioopm_hash_table_insert(ht, key1, value1);


  elem_t key2 = string_elem("b"); 
  elem_t value2 = int_elem(2);
  ioopm_hash_table_insert(ht, key2, value2);


  elem_t key3 = string_elem("c"); 
  elem_t value3 = int_elem(3);
  ioopm_hash_table_insert(ht, key3, value3);
  
  
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key1));


  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key2));


  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key3));


  ioopm_hash_table_destroy(ht);
}

void test_insert_remove_check_key(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq);

  elem_t key = string_elem("a"); 
  elem_t value = int_elem(1);
  ioopm_hash_table_insert(ht, key, value);

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key));
  
  elem_t result = int_elem(0);
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, value.i); 

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, key)); 

  ioopm_hash_table_destroy(ht);
}

void test_key_insert_three_remove_one(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq);

  elem_t key1 = string_elem("a"); 
  elem_t value1 = int_elem(1);
  ioopm_hash_table_insert(ht, key1, value1);


  elem_t key2 = string_elem("b"); 
  elem_t value2 = int_elem(2);
  ioopm_hash_table_insert(ht, key2, value2);


  elem_t key3 = string_elem("c"); 
  elem_t value3 = int_elem(3);
  ioopm_hash_table_insert(ht, key3, value3);
  
  
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key1));


  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key2));


  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key3));
  
  elem_t result = int_elem(0); 
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key2, &result));
  CU_ASSERT_EQUAL(result.i, value2.i);
  
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, key2));
  
  
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key1));
  
  
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, key3));

  ioopm_hash_table_destroy(ht);
}

void test_empty_table_size(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq);

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0); 

  ioopm_hash_table_destroy(ht);
}

void test_table_size_one(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq);

  elem_t key = string_elem("a"); 
  elem_t value = int_elem(0); 
  ioopm_hash_table_insert(ht, key, value); 

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);

  ioopm_hash_table_destroy(ht); 
}

void test_table_size_large(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq);

  char keys[50][8]; 
  size_t i = 0;
  for (; i < 50; i++)
  {
    snprintf(keys[i], sizeof(keys[i]), "key%zu", i); 
    ioopm_hash_table_insert(ht, string_elem(keys[i]), int_elem(i));
  }
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), i); 

  ioopm_hash_table_destroy(ht); 
}

void test_table_size_remove(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, string_eq); 

  elem_t key1 = string_elem("a"); 
  elem_t key2 = string_elem("b");

  elem_t value1 = int_elem(1);
  elem_t value2 = int_elem(2);

  ioopm_hash_table_insert(ht, key1, value1);
  ioopm_hash_table_insert(ht, key2, value2); 

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 2); 
  elem_t result = int_elem(0);
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key1, &result));
  CU_ASSERT_EQUAL(result.i, value1.i); 

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1); 

  ioopm_hash_table_destroy(ht); 
}


int main(void)
{
    // First we try to set up CUnit, and exit if we fail
    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        return CU_get_error();
    }

    // We then create an empty test suite and specify the name and
    // the init and cleanup functions
    CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
    if (my_test_suite == NULL) {
        // If the test suite could not be added, tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

     if (
    (CU_add_test(my_test_suite, "create and destroy", test_create_destroy) == NULL) ||
    (CU_add_test(my_test_suite, "Insert: one key", test_insert_once) == NULL) ||
    (CU_add_test(my_test_suite, "Insert: Same key twice", test_insert_twice_same) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: first entry", test_remove_entry_first) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: last entry", test_remove_entry_last) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: middle entry", test_remove_entry_middle) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: no entries", test_remove_entry_empty_table) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: only one entry", test_remove_entry_one) == NULL) ||
    (CU_add_test(my_test_suite, "Lookup: Entry exits", test_lookup_entry_exsist) == NULL) ||
    (CU_add_test(my_test_suite, "Lookup: Entry does not exist", test_lookup_entry_no_exsist) == NULL) ||
    (CU_add_test(my_test_suite, "Has key: Key exist", test_has_key) == NULL) ||
    (CU_add_test(my_test_suite, "Has key: Table is empty", test_has_key_empty_table) == NULL) ||
    (CU_add_test(my_test_suite, "Has key: Multiple keys in table", test_has_multiple_keys) == NULL) ||
    (CU_add_test(my_test_suite, "Has key: Insert multiple - remove one", test_key_insert_three_remove_one) == NULL) ||
    (CU_add_test(my_test_suite, "Has key: Insert and remove same key", test_insert_remove_check_key) == NULL) ||
    (CU_add_test(my_test_suite, "Size: empty table", test_empty_table_size) == NULL) ||
    (CU_add_test(my_test_suite, "Size: one entry", test_table_size_one) == NULL) ||
    (CU_add_test(my_test_suite, "Size: Large table", test_table_size_large) == NULL) ||
    (CU_add_test(my_test_suite, "Size: size after removing element", test_table_size_remove) == NULL) ||
    0
  )
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

      // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_VERBOSE);

  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
        
    return CU_get_error();
}
