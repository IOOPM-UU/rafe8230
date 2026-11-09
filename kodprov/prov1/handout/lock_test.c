#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "CUnit/CUnit.h"
#include "lock.h"

int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

void test_session_create(void)
{
  session_t *s = session_create();
  CU_ASSERT_PTR_NOT_NULL_FATAL(s);
  CU_ASSERT_PTR_NULL(session_drop(s));
}

void test_session_join(void)
{
  session_t *s = session_create();
  CU_ASSERT_PTR_NOT_NULL_FATAL(s);
  CU_ASSERT_PTR_EQUAL(session_join(s), s); // Refcount is 2

  CU_ASSERT_PTR_EQUAL(session_drop(s), s); // Refcount is 1
  CU_ASSERT_PTR_NULL(session_drop(s));
}

void test_session_join_twice(void)
{
  session_t *s = session_create();
  CU_ASSERT_PTR_NOT_NULL_FATAL(s);
  CU_ASSERT_PTR_EQUAL(session_join(s), s); // Refcount is 2
  CU_ASSERT_PTR_EQUAL(session_join(s), s); // Refcount is 3

  CU_ASSERT_PTR_EQUAL(session_drop(s), s); // Refcount is 2
  CU_ASSERT_PTR_EQUAL(session_drop(s), s); // Refcount is 1
  CU_ASSERT_PTR_NULL(session_drop(s));
}

void test_session_join_many(void)
{
  session_t *s = session_create();
  CU_ASSERT_PTR_NOT_NULL_FATAL(s);
  for (int i = 0; i < 100; i++)
  {
    session_join(s);
  }
  // Refcount is 101
  for (int i = 0; i < 100; i++)
  {
    session_drop(s);
  }
  // Refcount is 1
  CU_ASSERT_PTR_NULL(session_drop(s));
}

void test_lock_basic(void)
{
  lock_t *l = lock_create(42);
  CU_ASSERT_PTR_NOT_NULL_FATAL(l);

  CU_ASSERT_EQUAL(lock_get_id(l), 42);
  CU_ASSERT_PTR_NULL(lock_get_session(l));
  CU_ASSERT_TRUE(lock_is_open(l));

  lock_destroy(l);
}

void test_key_basic(void)
{
  digikey_t *k = key_create();
  CU_ASSERT_PTR_NOT_NULL_FATAL(k);

  CU_ASSERT_PTR_NULL(key_get_session(k));

  key_destroy(k);
}

void test_connect_to_lock_once(void)
{
  digikey_t *k = key_create();
  lock_t *l = lock_create(1);

  CU_ASSERT_PTR_NOT_NULL_FATAL(k);
  CU_ASSERT_PTR_NOT_NULL_FATAL(l);

  CU_ASSERT_TRUE(connect_to_lock(l, k));
  // k and l now share a session whose refcount is 2

  CU_ASSERT_FALSE(lock_is_open(l));

  CU_ASSERT_PTR_NOT_NULL(key_get_session(k));
  CU_ASSERT_PTR_EQUAL(key_get_session(k), lock_get_session(l));

  key_destroy(k); // Refcount of session is 1
  lock_destroy(l); // Session is implicitly destroyed
}

void test_connect_to_lock_once_flipped(void)
{
  digikey_t *k = key_create();
  lock_t *l = lock_create(1);

  CU_ASSERT_PTR_NOT_NULL_FATAL(k);
  CU_ASSERT_PTR_NOT_NULL_FATAL(l);

  CU_ASSERT_TRUE(connect_to_lock(l, k));
  // k and l now share a session whose refcount is 2

  CU_ASSERT_FALSE(lock_is_open(l));

  CU_ASSERT_PTR_NOT_NULL(lock_get_session(l));
  CU_ASSERT_PTR_EQUAL(lock_get_session(l), key_get_session(k));

  lock_destroy(l); // Refcount of session is 1
  key_destroy(k); // Session is implicitly destroyed
}

void test_connect_to_lock_unlock(void)
{
  digikey_t *k = key_create();
  lock_t *l = lock_create(1);

  CU_ASSERT_PTR_NOT_NULL_FATAL(k);
  CU_ASSERT_PTR_NOT_NULL_FATAL(l);

  CU_ASSERT_TRUE(connect_to_lock(l, k));
  // k and l now share a session whose refcount is 2

  CU_ASSERT_FALSE(lock_is_open(l));

  CU_ASSERT_PTR_NOT_NULL(key_get_session(k));
  CU_ASSERT_PTR_EQUAL(key_get_session(k), lock_get_session(l));

  session_t *s = key_get_session(k);

  // Next connection unlocks l (key's session has refcount 1)
  CU_ASSERT_TRUE(connect_to_lock(l, k));

  CU_ASSERT_TRUE(lock_is_open(l));

  // l has no session, k still has the same session
  CU_ASSERT_PTR_NULL(lock_get_session(l));
  CU_ASSERT_PTR_EQUAL(key_get_session(k), s);

  lock_destroy(l); // Session is unaffected
  CU_ASSERT_PTR_EQUAL(key_get_session(k), s);

  key_destroy(k); // Session is implicitly destroyed
}

void test_connect_to_lock_twice(void)
{
  digikey_t *k = key_create();
  lock_t *l = lock_create(1);

  CU_ASSERT_PTR_NOT_NULL_FATAL(k);
  CU_ASSERT_PTR_NOT_NULL_FATAL(l);

  CU_ASSERT_TRUE(connect_to_lock(l, k));
  // k and l now share a session whose refcount is 2

  session_t *s = key_get_session(k);

  CU_ASSERT_FALSE(lock_is_open(l));

  // Next connection unlocks l (key's session has refcount 1)
  CU_ASSERT_TRUE(connect_to_lock(l, k));

  CU_ASSERT_TRUE(lock_is_open(l));

  // Yet another connection locks l again (shared session has refcount 2)
  CU_ASSERT_TRUE(connect_to_lock(l, k));

  CU_ASSERT_FALSE(lock_is_open(l));

  // l and k have the same session
  CU_ASSERT_PTR_EQUAL(lock_get_session(l), s);
  CU_ASSERT_PTR_EQUAL(key_get_session(k), s);

  lock_destroy(l); // Shared session has refcount 1
  key_destroy(k); // Session is implicitly destroyed
}

void test_connect_to_several_locks(void)
{
  digikey_t *k = key_create();
  lock_t *l1 = lock_create(1);
  lock_t *l2 = lock_create(2);
  lock_t *l3 = lock_create(3);

  CU_ASSERT_PTR_NOT_NULL_FATAL(k);
  CU_ASSERT_PTR_NOT_NULL_FATAL(l1);
  CU_ASSERT_PTR_NOT_NULL_FATAL(l2);
  CU_ASSERT_PTR_NOT_NULL_FATAL(l3);

  CU_ASSERT_TRUE(connect_to_lock(l1, k));
  CU_ASSERT_TRUE(connect_to_lock(l2, k));
  CU_ASSERT_TRUE(connect_to_lock(l3, k));
  // k, l1, l2 and l3 now share a session whose refcount is 4

  CU_ASSERT_FALSE(lock_is_open(l1));
  CU_ASSERT_FALSE(lock_is_open(l2));
  CU_ASSERT_FALSE(lock_is_open(l3));

  // Unlock l1 (shared session has refcount 3)
  CU_ASSERT_TRUE(connect_to_lock(l1, k));

  CU_ASSERT_TRUE(lock_is_open(l1));
  CU_ASSERT_FALSE(lock_is_open(l2));
  CU_ASSERT_FALSE(lock_is_open(l3));

  // Unlock l2 (shared session has refcount 2)
  CU_ASSERT_TRUE(connect_to_lock(l2, k));

  CU_ASSERT_TRUE(lock_is_open(l1));
  CU_ASSERT_TRUE(lock_is_open(l2));
  CU_ASSERT_FALSE(lock_is_open(l3));

  // Unlock l3 (shared session has refcount 1)
  CU_ASSERT_TRUE(connect_to_lock(l3, k));

  CU_ASSERT_TRUE(lock_is_open(l1));
  CU_ASSERT_TRUE(lock_is_open(l2));
  CU_ASSERT_TRUE(lock_is_open(l3));

  lock_destroy(l1);
  lock_destroy(l2);
  lock_destroy(l3);
  key_destroy(k); // Session is implicitly destroyed
}

void test_connect_keys_to_locks(void)
{
  digikey_t *k1 = key_create();
  digikey_t *k2 = key_create();
  lock_t *l1 = lock_create(1);
  lock_t *l2 = lock_create(2);
  lock_t *l3 = lock_create(3);

  CU_ASSERT_PTR_NOT_NULL_FATAL(k1);
  CU_ASSERT_PTR_NOT_NULL_FATAL(k2);
  CU_ASSERT_PTR_NOT_NULL_FATAL(l1);
  CU_ASSERT_PTR_NOT_NULL_FATAL(l2);
  CU_ASSERT_PTR_NOT_NULL_FATAL(l3);

  CU_ASSERT_TRUE(connect_to_lock(l1, k1));
  // k1 shares a session with l1 (refcount 2)

  // k2 cannot connect to l1, no new session is created
  CU_ASSERT_FALSE(connect_to_lock(l1, k2));
  CU_ASSERT_PTR_NULL(key_get_session(k2));
  CU_ASSERT_FALSE(lock_is_open(l1));

  CU_ASSERT_TRUE(connect_to_lock(l2, k2));
  CU_ASSERT_TRUE(connect_to_lock(l3, k2));
  // k2 shares a session with l2 and l3 (refcount 3)

  CU_ASSERT_PTR_NOT_EQUAL(key_get_session(k1), key_get_session(k2));

  // k1 cannot unlock l2 or l3
  CU_ASSERT_FALSE(connect_to_lock(l2, k1));
  CU_ASSERT_FALSE(connect_to_lock(l3, k1));

  // k2 cannot unlock l1
  CU_ASSERT_FALSE(connect_to_lock(l1, k2));

  // Unlock l3 (shared session has refcount 2)
  CU_ASSERT_TRUE(connect_to_lock(l3, k2));

  // k1 can now lock l3 (share session gets refcount 3)
  CU_ASSERT_TRUE(connect_to_lock(l3, k1));

  // Destroy k2 and its lock (session is implicitly destroyed)
  key_destroy(k2);
  lock_destroy(l2);

  // Destroy k1 and its locks (session is implicitly destroyed)
  lock_destroy(l1);
  CU_ASSERT_TRUE(connect_to_lock(l3, k1)); // unlock l3 just for fun
  lock_destroy(l3);
  key_destroy(k1);
}

int main()
{
  CU_pSuite suite = NULL;
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  suite = CU_add_suite("Lock Test Suite", init_suite, clean_suite);
  if (NULL == suite)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (
    (NULL == CU_add_test(suite, "Test creating a session", test_session_create)) ||
    (NULL == CU_add_test(suite, "Test joining a session", test_session_join)) ||
    (NULL == CU_add_test(suite, "Test joining a session twice", test_session_join_twice)) ||
    (NULL == CU_add_test(suite, "Test joining a session 100 times", test_session_join_many)) ||
    (NULL == CU_add_test(suite, "Test basic lock functions", test_lock_basic)) ||
    (NULL == CU_add_test(suite, "Test basic key functions", test_key_basic)) ||
    (NULL == CU_add_test(suite, "Test connecting to a lock", test_connect_to_lock_once)) ||
    (NULL == CU_add_test(suite, "Test connecting to a lock, flipping order of operations", test_connect_to_lock_once_flipped)) ||
    (NULL == CU_add_test(suite, "Test connecting to a lock and unlocking", test_connect_to_lock_unlock)) ||
    (NULL == CU_add_test(suite, "Test connecting to a lock twice", test_connect_to_lock_twice)) ||
    (NULL == CU_add_test(suite, "Test connecting to multiple locks", test_connect_to_several_locks)) ||
    (NULL == CU_add_test(suite, "Test connecting to multiple locks with multiple keys", test_connect_keys_to_locks)) ||
    false
  )
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
