#include <CUnit/Basic.h>
#include "hash_table.h"
#include <stdbool.h>

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
   ioopm_hash_table_t *ht = ioopm_hash_table_create();
   CU_ASSERT_PTR_NOT_NULL(ht);
   ioopm_hash_table_destroy(ht);
}


void test_insert_once(void)
{
  // create new hash table
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *key = "abc";
  int value = 123;

  // check that key is not in ht
  int result = 0;
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, 0);

  // insert key-value pair and check that the mapping exists
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, value);

  // destroy hash table
  ioopm_hash_table_destroy(ht);
}
void test_insert_twice_same(void)
{
  // create new hash table
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *key = "abc";
  int value = 123;
  char *key_dup = "abc";
  int value_dup = 543;

  // check that key is not in ht
  int result = 0;
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, 0);
  

  // insert key-value pair and check that the mapping exists
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, value);

  ioopm_hash_table_insert(ht, key_dup, value_dup);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key_dup, &result));
  CU_ASSERT_EQUAL(result, value_dup); 

  // destroy hash table
  ioopm_hash_table_destroy(ht);
}

// Remove the first element of a linked list of size two
void test_remove_entry_first(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(); 
  
  char *key1 = "a";
  int value1 = 1; 
  ioopm_hash_table_insert(ht, key1, value1);

  char *key2 = "r";
  int value2 = 2;
  ioopm_hash_table_insert(ht, key2, value2);
  
  // insert key-value linked list and check that the mapping exsits
  int result = 0; 
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result, value1);
  
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result));
  CU_ASSERT_EQUAL(result, value2);


  // remove the value and check if the correct value got removed
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key1, &result));
  CU_ASSERT_EQUAL(result, value1);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key1, &result));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result));
  CU_ASSERT_EQUAL(result, value2); 

  ioopm_hash_table_destroy(ht);
}

// Remove the midlle most element
void test_remove_entry_middle(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(); 
  
  char *key1 = "a";
  int value1 = 1; 
  ioopm_hash_table_insert(ht, key1, value1);

  char *key2 = "r";
  int value2 = 2;
  ioopm_hash_table_insert(ht, key2, value2);

  char *key3 = "at";
  int value3 = 3;
  ioopm_hash_table_insert(ht, key3, value3);

  
  // insert key-value linked list and check that the mapping exsits
  int result = 0; 
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result, value1);
  
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result));
  CU_ASSERT_EQUAL(result, value2);
  
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key3, &result));
  CU_ASSERT_EQUAL(result, value3);

  // remove the value and check if the correct value got removed
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key2, &result));
  CU_ASSERT_EQUAL(result, value2);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key2, &result));

  // check if other entries are still there
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result, value1); 

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key3, &result));
  CU_ASSERT_EQUAL(result, value3); 

  ioopm_hash_table_destroy(ht);
}

// Remove the last element of a linked list of size two
void test_remove_entry_one(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(); 
  
  char *key1 = "a";
  int value1 = 1; 
  ioopm_hash_table_insert(ht, key1, value1);
  
  // insert key-value linked list and check that the mapping exsits
  int result = 0; 
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result, value1);


  // remove the value and check if the correct value got removed
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key1, &result));
  CU_ASSERT_EQUAL(result, value1);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key1, &result));

  ioopm_hash_table_destroy(ht);
}

// Remove the last element of a linked list of size two
void test_remove_entry_last(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(); 
  
  char *key1 = "a";
  int value1 = 1; 
  ioopm_hash_table_insert(ht, key1, value1);

  char *key2 = "r";
  int value2 = 2;
  ioopm_hash_table_insert(ht, key2, value2);
  
  // insert key-value linked list and check that the mapping exsits
  int result = 0; 
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result, value1);
  
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result));
  CU_ASSERT_EQUAL(result, value2);


  // remove the value and check if the correct value got removed
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key2, &result));
  CU_ASSERT_EQUAL(result, value2);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key2, &result));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key1, &result));
  CU_ASSERT_EQUAL(result, value1); 

  ioopm_hash_table_destroy(ht);
}

// Try to remove an element in an empty table
void test_remove_entry_empty_table(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(); 
  char *key = "a";
  
  int result = 0; 

  // Check if key exsists in ht
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
  
  // Try to remove that key
  CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, key, &result));

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
    (CU_add_test(my_test_suite, "Insert once", test_insert_once) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: first entry", test_remove_entry_first) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: last entry", test_remove_entry_last) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: middle entry", test_remove_entry_middle) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: no entries", test_remove_entry_empty_table) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: only one entry", test_remove_entry_one) == NULL) ||
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
