#include <CUnit/Basic.h>
#include "linked_list.h"
#include <stdbool.h>
#include <assert.h>
#include "linked_list_private.h"
#include "linked_list_iterator.h"

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

void test_iterator_create_destroy(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list); 

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list); 

}
void test_iterator_at_end_of_empty(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list); 

    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));     

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list); 

}

void test_iterator_advance_single(void)
{
    ioopm_list_t *list = ioopm_list_create();

    elem_t value = int_elem(1); 
    ioopm_list_append(list, value); 

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list); 

    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value.i);
    ioopm_list_iterator_advance(it);

    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, -1); 

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list); 
}
void test_iterator_at_end_of_filled(void)
{
    ioopm_list_t *list = ioopm_list_create();
    
    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);
    
    ioopm_list_append(list, value1);
    ioopm_list_append(list, value2);
    ioopm_list_append(list, value3);
    
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list); 

    ioopm_list_iterator_advance(it);
    
    ioopm_list_iterator_advance(it);
    ioopm_list_iterator_advance(it);

    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list); 

}
void test_iterator_current_empty(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list); 

    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, -1); 
    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list); 
    
}
void test_iterator_current_filled(void)
{
    ioopm_list_t *list = ioopm_list_create();
    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);
    
    ioopm_list_append(list, value1);
    ioopm_list_append(list, value2);
    ioopm_list_append(list, value3);
    
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list); 

    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value1.i);
    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value2.i);
    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value3.i);
    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, -1);

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list); 

}
void test_iterator_remove_current_empty(void)
{
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list); 

    
    CU_ASSERT_EQUAL(ioopm_list_size(list), 0);
    CU_ASSERT_EQUAL(ioopm_list_iterator_remove(it).i, -1); 
    CU_ASSERT_EQUAL(ioopm_list_size(list), 0);

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list); 
    
}
void test_iterator_remove_current_first(void)
{
    ioopm_list_t *list = ioopm_list_create();
    
    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);
    
    ioopm_list_append(list, value1);
    ioopm_list_append(list, value2);
    ioopm_list_append(list, value3);
    
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list); 

    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value1.i);
    
    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
    CU_ASSERT_EQUAL(ioopm_list_iterator_remove(it).i, value1.i); 
    CU_ASSERT_EQUAL(ioopm_list_size(list), 2);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value2.i);
    
    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value3.i);
    
    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list); 

}
void test_iterator_remove_current_last(void)
{
    ioopm_list_t *list = ioopm_list_create();
    
    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);
    
    ioopm_list_append(list, value1);
    ioopm_list_append(list, value2);
    ioopm_list_append(list, value3);

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);

    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value1.i);
    
    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value2.i);
    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value3.i);
    
    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
    CU_ASSERT_EQUAL(ioopm_list_iterator_remove(it).i, value3.i); 
    CU_ASSERT_EQUAL(ioopm_list_size(list), 2);
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it)); 
    
    
    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list); 

}
void test_iterator_remove_current_middle(void)
{
    ioopm_list_t *list = ioopm_list_create();
    
    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);
    
    ioopm_list_append(list, value1);
    ioopm_list_append(list, value2);
    ioopm_list_append(list, value3);
    
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list);  

    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value1.i);
    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value2.i);
    
    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
    CU_ASSERT_EQUAL(ioopm_list_iterator_remove(it).i, value2.i); 
    CU_ASSERT_EQUAL(ioopm_list_size(list), 2);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value3.i);
    
    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list); 

}
void test_iterator_insert_current_first(void)
{
    ioopm_list_t *list = ioopm_list_create();
    
    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);
    
    ioopm_list_append(list, value1);
    ioopm_list_append(list, value2);
    
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list); 
    
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value1.i);

    CU_ASSERT_EQUAL(ioopm_list_size(list), 2);
    ioopm_list_iterator_insert(it, value3);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value3.i);

    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value1.i);

    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value2.i);

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list); 

}
void test_iterator_insert_current_middle(void)
{
    ioopm_list_t *list = ioopm_list_create();
    
    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);
    
    ioopm_list_append(list, value1);
    ioopm_list_append(list, value2);
    
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list); 
    
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value1.i);
    ioopm_list_iterator_advance(it);
    
    CU_ASSERT_EQUAL(ioopm_list_size(list), 2);
    ioopm_list_iterator_insert(it, value3);
    CU_ASSERT_EQUAL(ioopm_list_size(list), 3);
    
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value3.i);
    ioopm_list_iterator_advance(it);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value2.i);

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list); 

}
void test_iterator_insert_current_last(void)
{
    ioopm_list_t *list = ioopm_list_create();
    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);
    
    ioopm_list_append(list, value1);
    ioopm_list_append(list, value2);
    
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(list); 

    ioopm_list_iterator_advance(it);
    ioopm_list_iterator_advance(it);
    ioopm_list_iterator_insert(it, value3);
    

    CU_ASSERT_EQUAL(ioopm_list_iterator_current(it).i, value3.i);

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(list); 

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
    CU_pSuite my_test_suite = CU_add_suite("Linked list iterator tests: ", init_suite, clean_suite);
    if (my_test_suite == NULL) {
        // If the test suite could not be added, tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
    (CU_add_test(my_test_suite, "Create and destroy", test_iterator_create_destroy) == NULL) ||
    (CU_add_test(my_test_suite, "At end: empty list", test_iterator_at_end_of_empty) == NULL) ||
    (CU_add_test(my_test_suite, "At end: after full traversal", test_iterator_at_end_of_filled) == NULL) ||
    (CU_add_test(my_test_suite, "Advance: single entry", test_iterator_advance_single) == NULL) ||
    (CU_add_test(my_test_suite, "Current: empty list", test_iterator_current_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Current: three entries", test_iterator_current_filled) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: empty list", test_iterator_remove_current_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: first entry", test_iterator_remove_current_first) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: middle entry", test_iterator_remove_current_middle) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: last entry", test_iterator_remove_current_last) == NULL) ||
    (CU_add_test(my_test_suite, "Insert: at first position", test_iterator_insert_current_first) == NULL) ||
    (CU_add_test(my_test_suite, "Insert: in the middle", test_iterator_insert_current_middle) == NULL) ||
    (CU_add_test(my_test_suite, "Insert: at the end", test_iterator_insert_current_last) == NULL) ||
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