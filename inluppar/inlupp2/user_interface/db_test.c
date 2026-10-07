#include <CUnit/Basic.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"
#include "linked_list_iterator_i2.h"

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

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

// insert

void db_insert_item_one(void)
{
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    ioopm_list_t *l = ioopm_list_create();

    ioopm_shelf_t *slf = ioopm_db_shelf_create(strdup("A10"), 10);
    ioopm_list_append(l, ptr_elem(slf));
    
    merch_t *item = ioopm_db_item_create(strdup("Apple"), strdup("Red fruit"), 5, l);
    CU_ASSERT_PTR_NOT_NULL_FATAL(item);
    
    CU_ASSERT_TRUE(ioopm_db_add_item_to_db_test(db, item));

    elem_t result = int_elem(0);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_s, string_elem("Apple"), &result));
    merch_t *m = result.p;
    CU_ASSERT_PTR_NOT_NULL_FATAL(m);

    CU_ASSERT_STRING_EQUAL(ioopm_db_item_name(m).s, "Apple");
    CU_ASSERT_STRING_EQUAL(ioopm_db_item_desc(m).s, "Red fruit");
    CU_ASSERT_EQUAL(ioopm_db_item_price(m).i, 5);
    CU_ASSERT_EQUAL(ioopm_db_item_stock(m), 10);

    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_n), 1);
    elem_t owner = int_elem(0);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_n, string_elem("A10"), &owner));
    CU_ASSERT_STRING_EQUAL(owner.s, "Apple");

    // Only one item in shelves
    ioopm_list_t *shelves = ioopm_db_item_all_shelves(m);
    CU_ASSERT_EQUAL(ioopm_list_size(shelves), 1);

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(shelves);
    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "A10");
    
    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(shelves);   
    ioopm_db_destroy(db);
}
void db_insert_item_three_shelves(void)
{
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    ioopm_list_t *l = ioopm_list_create();

    ioopm_shelf_t *slf1 = ioopm_db_shelf_create(strdup("A10"), 10);
    ioopm_shelf_t *slf2 = ioopm_db_shelf_create(strdup("A20"), 20);
    ioopm_shelf_t *slf3 = ioopm_db_shelf_create(strdup("A30"), 30);
    ioopm_list_append(l, ptr_elem(slf1));
    ioopm_list_append(l, ptr_elem(slf2));
    ioopm_list_append(l, ptr_elem(slf3));
    
    merch_t *item = ioopm_db_item_create(strdup("Apple"), strdup("Red fruit"), 5, l);
    CU_ASSERT_PTR_NOT_NULL_FATAL(item);
    
    CU_ASSERT_TRUE(ioopm_db_add_item_to_db_test(db, item));

    elem_t result = int_elem(0);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_s, string_elem("Apple"), &result));
    merch_t *m = result.p;
    CU_ASSERT_PTR_NOT_NULL_FATAL(m);

    CU_ASSERT_STRING_EQUAL(ioopm_db_item_name(m).s, "Apple");
    CU_ASSERT_STRING_EQUAL(ioopm_db_item_desc(m).s, "Red fruit");
    CU_ASSERT_EQUAL(ioopm_db_item_price(m).i, 5);
    CU_ASSERT_EQUAL(ioopm_db_item_stock(m), 60);

    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_n), 3);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 1);
    
    elem_t owner = int_elem(0);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_n, string_elem("A10"), &owner));
    CU_ASSERT_STRING_EQUAL(owner.s, "Apple");

    // Only one item in shelves
    ioopm_list_t *shelves = ioopm_db_item_all_shelves(m);
    CU_ASSERT_EQUAL(ioopm_list_size(shelves), 3);

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(shelves);
    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "A10");
    ioopm_list_iterator_advance(it); 

    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "A20");
    ioopm_list_iterator_advance(it); 

    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "A30");
    ioopm_list_iterator_advance(it); 

    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it));
    
    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(shelves);   
    ioopm_db_destroy(db);
}
void db_insert_three_items_different_shelves(void)
{
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    ioopm_list_t *l1 = ioopm_list_create();
    ioopm_list_t *l2 = ioopm_list_create();
    ioopm_list_t *l3 = ioopm_list_create();

    ioopm_shelf_t *slf1 = ioopm_db_shelf_create(strdup("A10"), 10);
    ioopm_shelf_t *slf2 = ioopm_db_shelf_create(strdup("A20"), 20);
    ioopm_shelf_t *slf3 = ioopm_db_shelf_create(strdup("A30"), 30);
    ioopm_list_append(l1, ptr_elem(slf1));
    ioopm_list_append(l2, ptr_elem(slf2));
    ioopm_list_append(l3, ptr_elem(slf3));
    
    merch_t *item1 = ioopm_db_item_create(strdup("Apple"), strdup("Red fruit"), 5, l1);
    merch_t *item2 = ioopm_db_item_create(strdup("Pear"), strdup("Green fruit"), 6, l2);
    merch_t *item3 = ioopm_db_item_create(strdup("Orange"), strdup("Orange fruit"), 7, l3);

    CU_ASSERT_PTR_NOT_NULL_FATAL(item1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(item2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(item3);
    
    CU_ASSERT_TRUE(ioopm_db_add_item_to_db_test(db, item1));
    CU_ASSERT_TRUE(ioopm_db_add_item_to_db_test(db, item2));
    CU_ASSERT_TRUE(ioopm_db_add_item_to_db_test(db, item3));

    elem_t result = int_elem(0);

    // Apple
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_s, string_elem("Apple"), &result));
    merch_t *m1 = result.p;
    CU_ASSERT_PTR_NOT_NULL_FATAL(m1);

    CU_ASSERT_STRING_EQUAL(ioopm_db_item_name(m1).s, "Apple");
    CU_ASSERT_STRING_EQUAL(ioopm_db_item_desc(m1).s, "Red fruit");
    CU_ASSERT_EQUAL(ioopm_db_item_price(m1).i, 5);
    CU_ASSERT_EQUAL(ioopm_db_item_stock(m1), 10);

    ioopm_list_t *shelves1 = ioopm_db_item_all_shelves(m1);
    CU_ASSERT_EQUAL(ioopm_list_size(shelves1), 1);

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(shelves1);
    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "A10");
    ioopm_list_iterator_advance(it); 
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it)); 

    ioopm_list_iterator_destroy(it);

    // Pear
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_s, string_elem("Pear"), &result));
    merch_t *m2 = result.p;
    CU_ASSERT_PTR_NOT_NULL_FATAL(m2);

    CU_ASSERT_STRING_EQUAL(ioopm_db_item_name(m2).s, "Pear");
    CU_ASSERT_STRING_EQUAL(ioopm_db_item_desc(m2).s, "Green fruit");
    CU_ASSERT_EQUAL(ioopm_db_item_price(m2).i, 6);
    CU_ASSERT_EQUAL(ioopm_db_item_stock(m2), 20);

    ioopm_list_t *shelves2 = ioopm_db_item_all_shelves(m2);
    CU_ASSERT_EQUAL(ioopm_list_size(shelves2), 1);

    it = ioopm_list_iterator_create(shelves2);
    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "A20");
    ioopm_list_iterator_advance(it); 
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it)); 

    ioopm_list_iterator_destroy(it);


    // Orange
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_s, string_elem("Orange"), &result));
    merch_t *m3 = result.p;
    CU_ASSERT_PTR_NOT_NULL_FATAL(m3);

    CU_ASSERT_STRING_EQUAL(ioopm_db_item_name(m3).s, "Orange");
    CU_ASSERT_STRING_EQUAL(ioopm_db_item_desc(m3).s, "Orange fruit");
    CU_ASSERT_EQUAL(ioopm_db_item_price(m3).i, 7);
    CU_ASSERT_EQUAL(ioopm_db_item_stock(m3), 30);

    ioopm_list_t *shelves3 = ioopm_db_item_all_shelves(m3);
    CU_ASSERT_EQUAL(ioopm_list_size(shelves3), 1);

    it = ioopm_list_iterator_create(shelves3);
    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "A30");
    ioopm_list_iterator_advance(it); 
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it)); 

    ioopm_list_iterator_destroy(it);



    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_n), 3);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 3);

    elem_t owner = int_elem(0);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_n, string_elem("A10"), &owner));
    CU_ASSERT_STRING_EQUAL(owner.s, "Apple");
    
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_n, string_elem("A20"), &owner));
    CU_ASSERT_STRING_EQUAL(owner.s, "Pear");

    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_n, string_elem("A30"), &owner));
    CU_ASSERT_STRING_EQUAL(owner.s, "Orange");





    
    ioopm_list_destroy(shelves1);   
    ioopm_list_destroy(shelves2);   
    ioopm_list_destroy(shelves3);   
    ioopm_db_destroy(db);
}
void db_insert_three_items_three_shelves_each(void)
{
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    ioopm_list_t *l1 = ioopm_list_create();
    ioopm_list_t *l2 = ioopm_list_create();
    ioopm_list_t *l3 = ioopm_list_create();

    ioopm_shelf_t *slf1 = ioopm_db_shelf_create(strdup("A10"), 10);
    ioopm_shelf_t *slf2 = ioopm_db_shelf_create(strdup("A20"), 20);
    ioopm_shelf_t *slf3 = ioopm_db_shelf_create(strdup("A30"), 30);
    ioopm_list_append(l1, ptr_elem(slf1));
    ioopm_list_append(l1, ptr_elem(slf2));
    ioopm_list_append(l1, ptr_elem(slf3));

    ioopm_shelf_t *slf4 = ioopm_db_shelf_create(strdup("B10"), 40);
    ioopm_shelf_t *slf5 = ioopm_db_shelf_create(strdup("B20"), 50);
    ioopm_shelf_t *slf6 = ioopm_db_shelf_create(strdup("B30"), 60);
    ioopm_list_append(l2, ptr_elem(slf4));
    ioopm_list_append(l2, ptr_elem(slf5));
    ioopm_list_append(l2, ptr_elem(slf6));

    ioopm_shelf_t *slf7 = ioopm_db_shelf_create(strdup("C10"), 70);
    ioopm_shelf_t *slf8 = ioopm_db_shelf_create(strdup("C20"), 80);
    ioopm_shelf_t *slf9 = ioopm_db_shelf_create(strdup("C30"), 90);
    ioopm_list_append(l3, ptr_elem(slf7));
    ioopm_list_append(l3, ptr_elem(slf8));
    ioopm_list_append(l3, ptr_elem(slf9));
    
    merch_t *item1 = ioopm_db_item_create(strdup("Apple"), strdup("Red fruit"), 5, l1);
    merch_t *item2 = ioopm_db_item_create(strdup("Pear"), strdup("Green fruit"), 6, l2);
    merch_t *item3 = ioopm_db_item_create(strdup("Orange"), strdup("Orange fruit"), 7, l3);

    CU_ASSERT_PTR_NOT_NULL_FATAL(item1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(item2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(item3);
    
    CU_ASSERT_TRUE(ioopm_db_add_item_to_db_test(db, item1));
    CU_ASSERT_TRUE(ioopm_db_add_item_to_db_test(db, item2));
    CU_ASSERT_TRUE(ioopm_db_add_item_to_db_test(db, item3));

    elem_t result = int_elem(0);

    // Apple
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_s, string_elem("Apple"), &result));
    merch_t *m = result.p;
    CU_ASSERT_PTR_NOT_NULL_FATAL(m);

    CU_ASSERT_STRING_EQUAL(ioopm_db_item_name(m).s, "Apple");
    CU_ASSERT_STRING_EQUAL(ioopm_db_item_desc(m).s, "Red fruit");
    CU_ASSERT_EQUAL(ioopm_db_item_price(m).i, 5);
    CU_ASSERT_EQUAL(ioopm_db_item_stock(m), 60);

    ioopm_list_t *shelves = ioopm_db_item_all_shelves(m);
    CU_ASSERT_EQUAL(ioopm_list_size(shelves), 3);

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(shelves);
    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "A10");
    ioopm_list_iterator_advance(it); 
    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "A20");
    ioopm_list_iterator_advance(it); 
    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "A30");
    ioopm_list_iterator_advance(it); 


    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it)); 

    ioopm_list_iterator_destroy(it);

    // Pear
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_s, string_elem("Pear"), &result));
    merch_t *m2 = result.p;
    CU_ASSERT_PTR_NOT_NULL_FATAL(m2);

    CU_ASSERT_STRING_EQUAL(ioopm_db_item_name(m2).s, "Pear");
    CU_ASSERT_STRING_EQUAL(ioopm_db_item_desc(m2).s, "Green fruit");
    CU_ASSERT_EQUAL(ioopm_db_item_price(m2).i, 6);
    CU_ASSERT_EQUAL(ioopm_db_item_stock(m2), 150);

    ioopm_list_t *shelves2 = ioopm_db_item_all_shelves(m2);
    CU_ASSERT_EQUAL(ioopm_list_size(shelves2), 3);

    it = ioopm_list_iterator_create(shelves2);
    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "B10");
    ioopm_list_iterator_advance(it); 
    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "B20");
    ioopm_list_iterator_advance(it); 
    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "B30");
    ioopm_list_iterator_advance(it); 

    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it)); 

    ioopm_list_iterator_destroy(it);


    // Orange
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_s, string_elem("Orange"), &result));
    merch_t *m3 = result.p;
    CU_ASSERT_PTR_NOT_NULL_FATAL(m3);

    CU_ASSERT_STRING_EQUAL(ioopm_db_item_name(m3).s, "Orange");
    CU_ASSERT_STRING_EQUAL(ioopm_db_item_desc(m3).s, "Orange fruit");
    CU_ASSERT_EQUAL(ioopm_db_item_price(m3).i, 7);
    CU_ASSERT_EQUAL(ioopm_db_item_stock(m3), 240);

    ioopm_list_t *shelves3 = ioopm_db_item_all_shelves(m3);
    CU_ASSERT_EQUAL(ioopm_list_size(shelves3), 3);

    it = ioopm_list_iterator_create(shelves3);
    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "C10");
    ioopm_list_iterator_advance(it); 
    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "C20");
    ioopm_list_iterator_advance(it); 
    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "C30");
    ioopm_list_iterator_advance(it); 
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(it)); 

    ioopm_list_iterator_destroy(it);



    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_n), 9);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 3);

    elem_t owner = int_elem(0);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_n, string_elem("A10"), &owner));
    CU_ASSERT_STRING_EQUAL(owner.s, "Apple");
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_n, string_elem("A20"), &owner));
    CU_ASSERT_STRING_EQUAL(owner.s, "Apple");
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_n, string_elem("A30"), &owner));
    CU_ASSERT_STRING_EQUAL(owner.s, "Apple");
    
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_n, string_elem("B10"), &owner));
    CU_ASSERT_STRING_EQUAL(owner.s, "Pear");
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_n, string_elem("B20"), &owner));
    CU_ASSERT_STRING_EQUAL(owner.s, "Pear");
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_n, string_elem("B30"), &owner));
    CU_ASSERT_STRING_EQUAL(owner.s, "Pear");

    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_n, string_elem("C10"), &owner));
    CU_ASSERT_STRING_EQUAL(owner.s, "Orange");
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_n, string_elem("C20"), &owner));
    CU_ASSERT_STRING_EQUAL(owner.s, "Orange");
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_n, string_elem("C30"), &owner));
    CU_ASSERT_STRING_EQUAL(owner.s, "Orange");
    
    ioopm_list_destroy(shelves);   
    ioopm_list_destroy(shelves2);   
    ioopm_list_destroy(shelves3);   
    ioopm_db_destroy(db);
}
void db_insert_two_items_same_name(void)
{
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    ioopm_list_t *l = ioopm_list_create();

    ioopm_shelf_t *slf = ioopm_db_shelf_create(strdup("A10"), 10);
    ioopm_list_append(l, ptr_elem(slf));
    
    merch_t *item = ioopm_db_item_create(strdup("Apple"), strdup("Red fruit"), 5, l);
    CU_ASSERT_PTR_NOT_NULL_FATAL(item);
    
    CU_ASSERT_TRUE(ioopm_db_add_item_to_db_test(db, item));
    
    elem_t result = int_elem(0);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_s, string_elem("Apple"), &result));
    merch_t *m = result.p;
    CU_ASSERT_PTR_NOT_NULL_FATAL(m);
    
    // check item creation
    CU_ASSERT_STRING_EQUAL(ioopm_db_item_name(m).s, "Apple");
    CU_ASSERT_STRING_EQUAL(ioopm_db_item_desc(m).s, "Red fruit");
    CU_ASSERT_EQUAL(ioopm_db_item_price(m).i, 5);
    CU_ASSERT_EQUAL(ioopm_db_item_stock(m), 10);
    
    // Check the ht_s
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_n), 1);
    elem_t owner = int_elem(0);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_n, string_elem("A10"), &owner));
    CU_ASSERT_STRING_EQUAL(owner.s, "Apple");
    
    // Only one item in shelves
    ioopm_list_t *shelves = ioopm_db_item_all_shelves(m);
    CU_ASSERT_EQUAL(ioopm_list_size(shelves), 1);
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(shelves);
    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "A10");
    
    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(shelves);   
    
    
    ioopm_list_t *l2 = ioopm_list_create();
    ioopm_shelf_t *slf2 = ioopm_db_shelf_create(strdup("B10"), 10);
    ioopm_list_append(l2, ptr_elem(slf2));

    merch_t *item_new = ioopm_db_item_create(strdup("Apple"), strdup("Red and Green fruit"), 10, l2);
    elem_t result2 = int_elem(0);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_s, string_elem("Apple"), &result2));
    merch_t *m2 = result2.p;
    CU_ASSERT_PTR_NOT_NULL_FATAL(m2);
    
    // check item creation
    CU_ASSERT_STRING_EQUAL(ioopm_db_item_name(m2).s, "Apple");
    CU_ASSERT_STRING_EQUAL(ioopm_db_item_desc(m2).s, "Red fruit");
    CU_ASSERT_EQUAL(ioopm_db_item_price(m2).i, 5);
    CU_ASSERT_EQUAL(ioopm_db_item_stock(m2), 10);

    
    CU_ASSERT_FALSE(ioopm_db_add_item_to_db_test(db, item_new));
    ioopm_db_item_destroy(item_new);

    // Check the ht_s
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_n), 1);
    elem_t owner2 = int_elem(0);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_n, string_elem("A10"), &owner2));
    CU_ASSERT_STRING_EQUAL(owner2.s, "Apple");
    
    // Only one item in shelves
    ioopm_list_t *shelves2 = ioopm_db_item_all_shelves(m2);
    CU_ASSERT_EQUAL(ioopm_list_size(shelves2), 1);
    ioopm_list_iterator_t *it2 = ioopm_list_iterator_create(shelves2);
    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it2));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it2).s, "A10");
    
    ioopm_list_iterator_destroy(it2);
    ioopm_list_destroy(shelves2);   
    
    
    
    
    ioopm_db_destroy(db);

} 
void db_insert_shelf_amount_zero(void)
{
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    ioopm_list_t *l = ioopm_list_create();

    ioopm_shelf_t *slf = ioopm_db_shelf_create(strdup("A10"), 0);
    ioopm_list_append(l, ptr_elem(slf));
    merch_t *item = ioopm_db_item_create(strdup("Apple"), strdup("Red fruit"), 5, l);
    CU_ASSERT_PTR_NOT_NULL_FATAL(item);
    
    CU_ASSERT_TRUE(ioopm_db_add_item_to_db_test(db, item));
    CU_ASSERT_EQUAL(ioopm_db_item_stock(item), 0);
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(db->ht_n));
    ioopm_db_destroy(db);
}
void db_insert_shelf_one_empty_two_not_empty(void)
{
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    ioopm_list_t *l = ioopm_list_create();

    ioopm_shelf_t *slf = ioopm_db_shelf_create(strdup("A10"), 0);
    ioopm_list_append(l, ptr_elem(slf));
    ioopm_shelf_t *slf2 = ioopm_db_shelf_create(strdup("A20"), 10);
    ioopm_list_append(l, ptr_elem(slf2));
    ioopm_shelf_t *slf3 = ioopm_db_shelf_create(strdup("A30"), 20);
    ioopm_list_append(l, ptr_elem(slf3));
    
    merch_t *item = ioopm_db_item_create(strdup("Apple"), strdup("Red fruit"), 5, l);
    CU_ASSERT_PTR_NOT_NULL_FATAL(item);
    
    CU_ASSERT_TRUE(ioopm_db_add_item_to_db_test(db, item));

    CU_ASSERT_EQUAL(ioopm_list_size(item->list), 2);
    CU_ASSERT_EQUAL(ioopm_db_item_stock(item), 30);
    elem_t owner;
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(db->ht_n, string_elem("A10"), &owner));
        
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_n), 2);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db->ht_s), 1);

    ioopm_db_destroy(db);
}
void db_insert_shelf_already_take_by_other_item(void)
{
    ioopm_data_base_t *db = ioopm_db_create(string_knr_hash, string_eq);
    ioopm_list_t *l = ioopm_list_create();

    ioopm_shelf_t *slf = ioopm_db_shelf_create(strdup("A10"), 10);
    ioopm_list_append(l, ptr_elem(slf));
    
    merch_t *item = ioopm_db_item_create(strdup("Apple"), strdup("Red fruit"), 5, l);
    CU_ASSERT_PTR_NOT_NULL_FATAL(item);
    
    CU_ASSERT_TRUE(ioopm_db_add_item_to_db_test(db, item));
    

    ioopm_list_t *l2 = ioopm_list_create();

    ioopm_shelf_t *slf2 = ioopm_db_shelf_create(strdup("A10"), 200);
    ioopm_list_append(l2, ptr_elem(slf2));
    merch_t *item2 = ioopm_db_item_create(strdup("Pear"), strdup("Green fruit"), 10, l2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(item2);
    
    CU_ASSERT_FALSE(ioopm_db_add_item_to_db_test(db, item2));
    ioopm_db_item_destroy(item2); 
    
    elem_t result2 = int_elem(0);
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(db->ht_s, string_elem("Pear"), &result2));

    elem_t result = int_elem(0);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(db->ht_s, string_elem("Apple"), &result));
    merch_t *m = result.p;
    CU_ASSERT_PTR_NOT_NULL_FATAL(m);

    // Only one item in shelves
    ioopm_list_t *shelves = ioopm_db_item_all_shelves(m);
    CU_ASSERT_EQUAL(ioopm_list_size(shelves), 1);

    ioopm_list_iterator_t *it = ioopm_list_iterator_create(shelves);
    CU_ASSERT_FALSE_FATAL(ioopm_list_iterator_at_end(it));
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, "A10");
    
    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(shelves);   
    ioopm_db_destroy(db);
}

// 

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
 
        (CU_add_test(my_test_suite, "insert: one", db_insert_item_one) == NULL) ||
        (CU_add_test(my_test_suite, "insert: one with three shelves", db_insert_item_three_shelves) == NULL) ||
        (CU_add_test(my_test_suite, "insert: three with one shelf", db_insert_three_items_different_shelves) == NULL) ||
        (CU_add_test(my_test_suite, "insert: three with three shelf each", db_insert_three_items_three_shelves_each) == NULL) ||
        (CU_add_test(my_test_suite, "Insert: two items with the same name", db_insert_two_items_same_name) == NULL) ||
        (CU_add_test(my_test_suite, "Insert: An item with a shelf quantity 0", db_insert_shelf_amount_zero) == NULL) ||
        (CU_add_test(my_test_suite, "Insert: insert one item with one empty and two filled shelves", db_insert_shelf_one_empty_two_not_empty) == NULL) ||
        (CU_add_test(my_test_suite, "Insert: Insert different items on the same shelf", db_insert_shelf_already_take_by_other_item) == NULL) ||
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