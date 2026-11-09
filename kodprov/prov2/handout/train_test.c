#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "train.h"

int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

void test_track_create(void)
{
  track_t *t = track_create(NULL);
  CU_ASSERT_PTR_NOT_NULL_FATAL(t);
  CU_ASSERT_FALSE(track_has_train(t));
  track_destroy(t);
}

void test_move_straight(void)
{
  // Build a simple line: a -> b -> c -> d
  track_t *d = track_create(NULL);
  track_t *c = track_create(d);
  track_t *b = track_create(c);
  track_t *a = track_create(b);

  CU_ASSERT_PTR_NOT_NULL_FATAL(a);

  // Move a train 2 steps straight
  track_t *pos = track_move(a, 2, false);
  CU_ASSERT_PTR_EQUAL(pos, c);
  CU_ASSERT_TRUE(track_has_train(c));
  CU_ASSERT_FALSE(track_has_train(a));
  CU_ASSERT_FALSE(track_has_train(b));

  track_destroy(a);
}

void test_add_switch(void)
{
  track_t *a = track_create(NULL);
  track_t *b = track_create(NULL);

  CU_ASSERT_PTR_NOT_NULL_FATAL(a);
  CU_ASSERT_PTR_NOT_NULL_FATAL(b);

  CU_ASSERT_TRUE(track_add_switch(a, b));
  CU_ASSERT_FALSE(track_add_switch(a, b)); // cannot add a second switch

  track_destroy(a);
}

void test_move_take_switch(void)
{
  // Build a -> b -> c -> d -> e
  track_t *e = track_create(NULL);
  track_t *d = track_create(e);
  track_t *c = track_create(d);
  track_t *b = track_create(c);
  track_t *a = track_create(b);

  // Create side branch: c -> f -> g
  track_t *g = track_create(NULL);
  track_t *f = track_create(g);
  CU_ASSERT_TRUE(track_add_switch(c, f));

  // Move 4 steps taking the switch (a->b->c->f->g)
  track_t *pos = track_move(a, 4, true);
  CU_ASSERT_PTR_EQUAL(pos, g);
  CU_ASSERT_TRUE(track_has_train(g));

  CU_ASSERT_FALSE(track_has_train(a));
  CU_ASSERT_FALSE(track_has_train(b));
  CU_ASSERT_FALSE(track_has_train(c));
  CU_ASSERT_FALSE(track_has_train(d));
  CU_ASSERT_FALSE(track_has_train(e));
  CU_ASSERT_FALSE(track_has_train(f));

  track_destroy(a);
}

void test_move_ignore_switch(void)
{
  // Build a -> b -> c -> d -> e
  track_t *e = track_create(NULL);
  track_t *d = track_create(e);
  track_t *c = track_create(d);
  track_t *b = track_create(c);
  track_t *a = track_create(b);

  // Create side branch: c -> f -> g
  track_t *g = track_create(NULL);
  track_t *f = track_create(g);
  CU_ASSERT_TRUE(track_add_switch(c, f));

  // Move 3 steps ignoring the switch (a->b->c->d)
  track_t *pos = track_move(a, 3, false);
  CU_ASSERT_PTR_EQUAL(pos, d);
  CU_ASSERT_TRUE(track_has_train(d));

  CU_ASSERT_FALSE(track_has_train(a));
  CU_ASSERT_FALSE(track_has_train(b));
  CU_ASSERT_FALSE(track_has_train(c));
  CU_ASSERT_FALSE(track_has_train(e));
  CU_ASSERT_FALSE(track_has_train(f));
  CU_ASSERT_FALSE(track_has_train(g));

  track_destroy(a);
}

void test_move_collision(void)
{
  // Build a -> b -> c -> d
  track_t *d = track_create(NULL);
  track_t *c = track_create(d);
  track_t *b = track_create(c);
  track_t *a = track_create(b);

  // Place a train on c
  CU_ASSERT_PTR_EQUAL(track_move(c, 0, false), c);
  CU_ASSERT_TRUE(track_has_train(c));

  // Try to move another train from a past c — should stop at b
  track_t *pos = track_move(a, 3, false);
  CU_ASSERT_PTR_EQUAL(pos, b);
  CU_ASSERT_TRUE(track_has_train(b));
  CU_ASSERT_TRUE(track_has_train(c));

  CU_ASSERT_FALSE(track_has_train(a));

  track_destroy(a);
}

void test_move_collision_by_switch(void)
{
  // Build a -> b -> c -> d -> e
  track_t *e = track_create(NULL);
  track_t *d = track_create(e);
  track_t *c = track_create(d);
  track_t *b = track_create(c);
  track_t *a = track_create(b);

  // Create side branch: c -> f -> g
  track_t *g = track_create(NULL);
  track_t *f = track_create(g);

  track_add_switch(c, f);

  // Move 4 steps taking the switch (a->b->c->f)
  track_t *pos = track_move(a, 3, true);
  CU_ASSERT_PTR_EQUAL(pos, f);
  CU_ASSERT_TRUE(track_has_train(f));

  // Move 4 steps stopping before the switch (a->b->c->c)
  pos = track_move(a, 3, true);
  CU_ASSERT_PTR_EQUAL(pos, c);
  CU_ASSERT_TRUE(track_has_train(c));

  track_destroy(a);
}

void test_move_to_end(void)
{
  // Build a -> b -> c
  track_t *c = track_create(NULL);
  track_t *b = track_create(c);
  track_t *a = track_create(b);

  // Move 5 steps — should stop at the end
  track_t *pos = track_move(a, 5, false);
  CU_ASSERT_PTR_EQUAL(pos, c);
  CU_ASSERT_TRUE(track_has_train(c));

  track_destroy(a);
}

void test_move_existing(void)
{
  // Build a -> b -> c
  track_t *c = track_create(NULL);
  track_t *b = track_create(c);
  track_t *a = track_create(b);

  // Add a train to a
  track_t *pos = track_move(a, 0, false);
  CU_ASSERT_PTR_EQUAL(pos, a);
  CU_ASSERT_TRUE(track_has_train(a));

  // Move that piece
  pos = track_move(a, 2, false);
  CU_ASSERT_PTR_EQUAL(pos, c);
  CU_ASSERT_FALSE(track_has_train(a));
  CU_ASSERT_FALSE(track_has_train(b));
  CU_ASSERT_TRUE(track_has_train(c));

  track_destroy(a);
}

void test_move_long(void)
{
  // Build a loooong track
  size_t alot = 1000;
  track_t *tracks[alot];
  track_t *next = NULL;
  for (size_t i = 0; i < alot; i++)
  {
    next = track_create(next);
    tracks[alot - 1 - i] = next;
  }
  CU_ASSERT_PTR_NOT_NULL_FATAL(tracks[0]);

  // No one has a train
  bool has_train = false;
  for (size_t i = 0; i < alot; i++)
  {
    has_train = has_train || track_has_train(tracks[i]);
  }
  CU_ASSERT_FALSE(has_train);

  // Fill up the board with pieces
  for (size_t i = 0; i < alot; i++)
  {
    track_move(tracks[0], alot, false);
  }

  has_train = true;
  // Everyone has a piece
  for (size_t i = 0; i < alot; i++)
  {
    has_train = has_train && track_has_train(tracks[i]);
  }
  CU_ASSERT_TRUE(has_train);

  track_destroy(tracks[0]);
}

int main()
{
  CU_pSuite suite = NULL;
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  suite = CU_add_suite("Train Test Suite", init_suite, clean_suite);
  if (NULL == suite)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (
    (NULL == CU_add_test(suite, "Test creating a track segment", test_track_create)) ||
    (NULL == CU_add_test(suite, "Test moving along a straight track", test_move_straight)) ||
    (NULL == CU_add_test(suite, "Test adding a switch", test_add_switch)) ||
    (NULL == CU_add_test(suite, "Test moving and taking a switch", test_move_take_switch)) ||
    (NULL == CU_add_test(suite, "Test moving and ignoring a switch", test_move_ignore_switch)) ||
    (NULL == CU_add_test(suite, "Test stopping at occupied segment", test_move_collision)) ||
    (NULL == CU_add_test(suite, "Test stopping at occupied segment after switch", test_move_collision_by_switch)) ||
    (NULL == CU_add_test(suite, "Test moving to the end of the track", test_move_to_end)) ||
    (NULL == CU_add_test(suite, "Test moving an existing train", test_move_existing)) ||
    (NULL == CU_add_test(suite, "Test moving on a long track", test_move_long)) ||
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