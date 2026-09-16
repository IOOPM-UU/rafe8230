#include <CUnit/Basic.h>
#include "hash_table.h"
#include <stdbool.h>
#include <assert.h>
#include "hash_table_iterator.h"
#include "hash_table_private.h"

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

void test_iterate_empty_table(void)
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(); 
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht); 

    CU_ASSERT_TRUE(ioopm_hash_table_iterator_at_end(it));
    ioopm_hash_table_iterator_destroy(it);
    ioopm_hash_table_destroy(ht); 
}

void test_iterate_table_size_one(void)
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(); 
    char *key = "a"; 
    int value = 1; 
    ioopm_hash_table_insert(ht, key, value);

    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht); 
    
    CU_ASSERT_STRING_EQUAL(ioopm_hash_table_iterator_current_key(it), key);

    CU_ASSERT_EQUAL(ioopm_hash_table_iterator_current_value(it), value);

    ioopm_hash_table_iterator_destroy(it); 
    ioopm_hash_table_destroy(ht); 
}


// Only one entry in each bucket
void test_iterate_table_size_mulitple(void)
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(); 
    
    char keys[No_Buckets][8]; 
    
    for (int i = 0; i < No_Buckets; i++)
    {
        snprintf(keys[i], sizeof(keys[i]), "key%d", i); 
        ioopm_hash_table_insert(ht, keys[i], i);
    }

    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), No_Buckets);


    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht); 
    int counter = 0;
    while (!ioopm_hash_table_iterator_at_end(it))
    {

        counter++;
        ioopm_hash_table_iterator_advance(it); 
    }
     
    CU_ASSERT_EQUAL(counter, No_Buckets);
    ioopm_hash_table_iterator_destroy(it);  
    ioopm_hash_table_destroy(ht);
}

// Max one entry in each bucket - also tests advancing between empty buckets
void test_iterate_table_multiple_checks(void)
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(); 

    // No_Buckets does not work because of our hash function
    char keys[20][8]; 

    // Initialize a large ht
    for (int i = 0; i < 20; i++)
    {
        snprintf(keys[i], sizeof(keys[i]), "key%d", i); 
        ioopm_hash_table_insert(ht, keys[i], i);
    }

    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht); 
    int counter = 0;
    bool seen[20] = { false };
    int value = 0; 
    while (!ioopm_hash_table_iterator_at_end(it))
    {
        // Update value to current and check if its inbounds 
        value = ioopm_hash_table_iterator_current_value(it);
        CU_ASSERT_TRUE(value >= 0 && value < 20);


        CU_ASSERT_STRING_EQUAL(ioopm_hash_table_iterator_current_key(it), keys[value]);
        CU_ASSERT_EQUAL(ioopm_hash_table_iterator_current_value(it), value); 
        
        // Check and update seen value to true
        CU_ASSERT_FALSE(seen[value]); 
        seen[value] = true; 
        
        ioopm_hash_table_iterator_advance(it); 
        counter++;
    }

    CU_ASSERT_EQUAL(counter, 20);
    ioopm_hash_table_iterator_destroy(it);  
    ioopm_hash_table_destroy(ht);
}

#define NO_ENTRIES 40
//Multiple entries in each bucket (or none)
void test_iterate_table_large_checks(void)
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(); 

    // No_Buckets does not work because of our hash function
 
    char keys[NO_ENTRIES][8]; 

    // Initialize a large ht
    for (int i = 0; i < NO_ENTRIES; i++)
    {
        snprintf(keys[i], sizeof(keys[i]), "key%d", i); 
        ioopm_hash_table_insert(ht, keys[i], i);
    }

    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht); 
    int counter = 0;
    bool seen[NO_ENTRIES] = { false };
    while (!ioopm_hash_table_iterator_at_end(it))
    {
        // Update value to current and check if its inbounds 
        int value = ioopm_hash_table_iterator_current_value(it);
        CU_ASSERT_TRUE(value >= 0 && value < NO_ENTRIES);


        CU_ASSERT_STRING_EQUAL(ioopm_hash_table_iterator_current_key(it), keys[value]);
        CU_ASSERT_EQUAL(ioopm_hash_table_iterator_current_value(it), value); 
        
        // Check and update seen value to true
        CU_ASSERT_FALSE(seen[value]); 
        seen[value] = true; 
        
        ioopm_hash_table_iterator_advance(it); 
        counter++;
    }

    CU_ASSERT_EQUAL(counter, NO_ENTRIES);
    ioopm_hash_table_iterator_destroy(it);  
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
    (CU_add_test(my_test_suite, "Iterate: Empty table", test_iterate_empty_table) == NULL) ||
    (CU_add_test(my_test_suite, "Iterate: One entry in table", test_iterate_table_size_one) == NULL) ||
    (CU_add_test(my_test_suite, "Iterate: Multiple entries, one in each bucket", test_iterate_table_size_mulitple) == NULL) ||
    (CU_add_test(my_test_suite, "Iterate: Multiple entries, all checked", test_iterate_table_multiple_checks) == NULL) ||
    (CU_add_test(my_test_suite, "Iterate: Multiple eintreis in each bucket", test_iterate_table_large_checks) == NULL) ||
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
