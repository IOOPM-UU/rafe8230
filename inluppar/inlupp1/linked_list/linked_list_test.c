#include <CUnit/Basic.h>
#include "linked_list.h"
#include <stdbool.h>
#include <assert.h>

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

void test_linked_list_create_destroy(void)
{
    ioopm_list_t *l_list = ioopm_list_create();
    CU_ASSERT_PTR_NOT_NULL(l_list); 
    ioopm_list_destroy(l_list); 
}

void test_linked_list_create_destroy_with_one_entry(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value = int_elem(1);

    ioopm_list_append(l_list, value);
    ioopm_list_destroy(l_list); 
    //if no mem-leaks then function is working
}
void test_linked_list_create_destroy_with_multiple_entries(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);
    elem_t value4 = int_elem(4);
    elem_t value5 = int_elem(5);

    ioopm_list_append(l_list, value1);
    ioopm_list_append(l_list, value2);
    ioopm_list_append(l_list, value3);
    ioopm_list_append(l_list, value4);
    ioopm_list_append(l_list, value5);
    
    ioopm_list_destroy(l_list); 
}

void test_linked_list_append_one(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value = int_elem(1);

    ioopm_list_append(l_list, value);

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 1);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 0).i, value.i);
    
    ioopm_list_destroy(l_list); 
}

void test_linked_list_append_three(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);

    ioopm_list_append(l_list, value1);
    ioopm_list_append(l_list, value2);
    ioopm_list_append(l_list, value3);

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 3);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 0).i, value1.i);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 1).i, value2.i);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 2).i, value3.i);
    
    ioopm_list_destroy(l_list); 
}

void test_linked_list_prepend_one(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value = int_elem(1);

    ioopm_list_prepend(l_list, value);
    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 1);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 0).i, value.i);
    
    ioopm_list_destroy(l_list); 

}
void test_linked_list_prepend_three(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);

    ioopm_list_prepend(l_list, value1);
    ioopm_list_prepend(l_list, value2);
    ioopm_list_prepend(l_list, value3);

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 3);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 2).i, value1.i);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 1).i, value2.i);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 0).i, value3.i);
    
    ioopm_list_destroy(l_list); 
}

void test_empty_list_head(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 0);
    CU_ASSERT_EQUAL(ioopm_list_head(l_list).i, -1);
    
    ioopm_list_destroy(l_list); 
}

void test_single_list_head(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value = int_elem(1);
    ioopm_list_append(l_list, value); 

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 1);
    CU_ASSERT_EQUAL(ioopm_list_head(l_list).i, 1);
    
    ioopm_list_destroy(l_list); 
}

void test_sized_list_head_prepend(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);

    ioopm_list_prepend(l_list, value1);
    ioopm_list_prepend(l_list, value2);
    ioopm_list_prepend(l_list, value3);

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 3);
    CU_ASSERT_EQUAL(ioopm_list_head(l_list).i, value3.i);
    CU_ASSERT_EQUAL(ioopm_list_last(l_list).i, value1.i);
    
    ioopm_list_destroy(l_list); 
}

void test_sized_list_head_append(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);

    ioopm_list_append(l_list, value1);
    ioopm_list_append(l_list, value2);
    ioopm_list_append(l_list, value3);

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 3);
    CU_ASSERT_EQUAL(ioopm_list_head(l_list).i, value1.i);
    CU_ASSERT_EQUAL(ioopm_list_last(l_list).i, value3.i);
    
    ioopm_list_destroy(l_list); 
}

void test_empty_list_last(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 0);
    CU_ASSERT_EQUAL(ioopm_list_last(l_list).i, -1);
    
    ioopm_list_destroy(l_list); 
}

void test_single_list_last(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value = int_elem(1);
    ioopm_list_append(l_list, value);

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 1);
    CU_ASSERT_EQUAL(ioopm_list_last(l_list).i, 1);
    
    ioopm_list_destroy(l_list); 
}

void test_sized_list_last_prepend(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);

    ioopm_list_prepend(l_list, value1);
    ioopm_list_prepend(l_list, value2);
    ioopm_list_prepend(l_list, value3);

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 3);
    CU_ASSERT_EQUAL(ioopm_list_last(l_list).i, value1.i);
    
    ioopm_list_destroy(l_list); 
}

void test_sized_list_last_append(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);

    ioopm_list_append(l_list, value1);
    ioopm_list_append(l_list, value2);
    ioopm_list_append(l_list, value3);

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 3);
    CU_ASSERT_EQUAL(ioopm_list_last(l_list).i, value3.i);
    
    ioopm_list_destroy(l_list); 
}

void test_list_insert_empty(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value1 = int_elem(1);

    ioopm_list_insert(l_list, 0, value1);

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 1);
    CU_ASSERT_EQUAL(ioopm_list_last(l_list).i, value1.i); 
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 0).i, value1.i);
    
    ioopm_list_destroy(l_list); 
}
void test_list_insert_middle(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value_middle = int_elem(3); 

    ioopm_list_append(l_list, value1);
    ioopm_list_append(l_list, value2);
    ioopm_list_insert(l_list, 1, value_middle);

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 3);
    CU_ASSERT_EQUAL(ioopm_list_last(l_list).i, value2.i);
    CU_ASSERT_EQUAL(ioopm_list_head(l_list).i, value1.i);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 1).i, value_middle.i);
    
    ioopm_list_destroy(l_list); 
}

void test_list_insert_last(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3); 

    ioopm_list_append(l_list, value1);
    ioopm_list_append(l_list, value2);
    ioopm_list_insert(l_list, 2, value3);

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 3);
    CU_ASSERT_EQUAL(ioopm_list_last(l_list).i, value3.i);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 2).i, value3.i);
    CU_ASSERT_EQUAL(ioopm_list_head(l_list).i, value1.i);
    
    ioopm_list_destroy(l_list); 
}

void test_list_remove_empty(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    ioopm_list_remove(l_list, 0);

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 0);
    
    ioopm_list_destroy(l_list); 
}
void test_list_insert_remove_one(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value1 = int_elem(1);

    ioopm_list_insert(l_list, 0, value1);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 0).i, value1.i);

    ioopm_list_remove(l_list, 0);

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 0);
    
    ioopm_list_destroy(l_list); 

}
void test_list_incert_three_remove_first(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);

    ioopm_list_append(l_list, value1);
    ioopm_list_append(l_list, value2);

    ioopm_list_insert(l_list, 0, value3);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 0).i, value3.i);
    ioopm_list_remove(l_list, 0);

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 2);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 0).i, value1.i); 
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 1).i, value2.i); 
    CU_ASSERT_EQUAL(ioopm_list_last(l_list).i, value2.i); 
    
    ioopm_list_destroy(l_list); 
    
}
void test_list_incert_three_remove_middle(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 
    
    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);
    
    ioopm_list_append(l_list, value1);
    ioopm_list_append(l_list, value2);
    
    ioopm_list_insert(l_list, 1, value3);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 1).i, value3.i);
    ioopm_list_remove(l_list, 1);
    
    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 2);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 0).i, value1.i); 
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 1).i, value2.i); 
    CU_ASSERT_EQUAL(ioopm_list_last(l_list).i, value2.i); 

    
    ioopm_list_destroy(l_list); 
    
}
void test_list_incert_three_remove_last(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 
    
    elem_t value1 = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);
    
    ioopm_list_append(l_list, value1);
    ioopm_list_append(l_list, value2);
    
    ioopm_list_insert(l_list, 2, value3);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 2).i, value3.i);
    ioopm_list_remove(l_list, 2);
    
    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 2);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 0).i, value1.i); 
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 1).i, value2.i); 
    CU_ASSERT_EQUAL(ioopm_list_last(l_list).i, value2.i); 
    
    ioopm_list_destroy(l_list); 
}
void test_list_get_empty(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 0).i, -1);
    
    ioopm_list_destroy(l_list); 
}
void test_list_get_one(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value = int_elem(1);
    ioopm_list_insert(l_list, 0, value);

    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 0).i, 1);
    
    ioopm_list_destroy(l_list); 
    
}
void test_list_get_three(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value = int_elem(1);
    elem_t value2 = int_elem(2);
    elem_t value3 = int_elem(3);
    ioopm_list_insert(l_list, 0, value);
    ioopm_list_insert(l_list, 1, value2);
    ioopm_list_insert(l_list, 2, value3);

    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 0).i, 1);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 1).i, 2);
    CU_ASSERT_EQUAL(ioopm_list_get(l_list, 2).i, 3);  
    
    ioopm_list_destroy(l_list); 
}
void test_list_size_empty(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 0);
    
    ioopm_list_destroy(l_list); 
}
void test_list_size_one(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value = int_elem(1);
    ioopm_list_append(l_list, value);

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 1);
    
    ioopm_list_destroy(l_list); 
}
void test_list_size_three(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value = int_elem(1);
    ioopm_list_append(l_list, value);
    elem_t value2 = int_elem(2);
    ioopm_list_append(l_list, value2);
    elem_t value3 = int_elem(3);
    ioopm_list_append(l_list, value3);

    CU_ASSERT_EQUAL(ioopm_list_size(l_list), 3);
    
    ioopm_list_destroy(l_list); 
}
void test_list_empty_is_empty(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    CU_ASSERT_TRUE(ioopm_list_is_empty(l_list));
    
    ioopm_list_destroy(l_list); 
}
void test_list_empty_not_empty(void)
{
    ioopm_list_t *l_list = ioopm_list_create(); 
    CU_ASSERT_PTR_NOT_NULL(l_list); 

    elem_t value = int_elem(1);
    ioopm_list_append(l_list, value);

    CU_ASSERT_FALSE(ioopm_list_is_empty(l_list));
    
    ioopm_list_destroy(l_list); 
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
    (CU_add_test(my_test_suite, "Create and destroy", test_linked_list_create_destroy) == NULL) ||
    (CU_add_test(my_test_suite, "Create and destroy: one entry", test_linked_list_create_destroy_with_one_entry) == NULL) ||
    (CU_add_test(my_test_suite, "Create and destroy: many entries", test_linked_list_create_destroy_with_multiple_entries) == NULL) ||
    (CU_add_test(my_test_suite, "Append: one", test_linked_list_append_one) == NULL) ||
    (CU_add_test(my_test_suite, "Append: three", test_linked_list_append_three) == NULL) ||
    (CU_add_test(my_test_suite, "Prepend: one", test_linked_list_prepend_one) == NULL) ||
    (CU_add_test(my_test_suite, "Prepend: three", test_linked_list_prepend_three) == NULL) ||
    (CU_add_test(my_test_suite, "Head: empty list", test_empty_list_head) == NULL) ||
    (CU_add_test(my_test_suite, "Head: single entry", test_single_list_head) == NULL) ||
    (CU_add_test(my_test_suite, "Head: after prepends", test_sized_list_head_prepend) == NULL) ||
    (CU_add_test(my_test_suite, "Head: after appends", test_sized_list_head_append) == NULL) ||
    (CU_add_test(my_test_suite, "Last: empty list", test_empty_list_last) == NULL) ||
    (CU_add_test(my_test_suite, "Last: single entry", test_single_list_last) == NULL) ||
    (CU_add_test(my_test_suite, "Last: after prepends", test_sized_list_last_prepend) == NULL) ||
    (CU_add_test(my_test_suite, "Last: after appends", test_sized_list_last_append) == NULL) ||
    (CU_add_test(my_test_suite, "Insert: into empty list", test_list_insert_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Insert: in the middle", test_list_insert_middle) == NULL) ||
    (CU_add_test(my_test_suite, "Insert: at the end", test_list_insert_last) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: from empty list", test_list_remove_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: the only entry", test_list_insert_remove_one) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: first of three", test_list_incert_three_remove_first) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: middle of three", test_list_incert_three_remove_middle) == NULL) ||
    (CU_add_test(my_test_suite, "Remove: last of three", test_list_incert_three_remove_last) == NULL) ||
    (CU_add_test(my_test_suite, "Get: empty list", test_list_get_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Get: one entry", test_list_get_one) == NULL) ||
    (CU_add_test(my_test_suite, "Get: three entries", test_list_get_three) == NULL) ||
    (CU_add_test(my_test_suite, "Size: empty list", test_list_size_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Size: one entry", test_list_size_one) == NULL) ||
    (CU_add_test(my_test_suite, "Size: three entries", test_list_size_three) == NULL) ||
    (CU_add_test(my_test_suite, "Is empty: empty list", test_list_empty_is_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Is empty: non-empty list", test_list_empty_not_empty) == NULL) ||
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