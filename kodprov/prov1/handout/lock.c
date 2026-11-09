#include "lock.h"


struct session {
  int ref_count;

};

struct lock {
  session_t *sesh;
  size_t id; 
};

struct digikey {
  session_t *sesh;
};

// Session functions //////////
session_t *session_create() {
  session_t *sesh = calloc(1, sizeof(session_t));
  sesh->ref_count = 1; 
  return sesh;
}

session_t *session_join(session_t *s) {
  s->ref_count++; 
  return s;
}

session_t *session_drop(session_t *s) {
  s->ref_count--; 
  if (s->ref_count == 0)
  {
    free(s);
    return NULL;
  }
  return s;
}


// Lock functions //////////
lock_t *lock_create(size_t id) {
  lock_t *lck = calloc(1, sizeof(lock_t)); 
  lck->id = id;
  return lck;
}

void lock_destroy(lock_t *l) {
	if (l->sesh != NULL) session_drop(l->sesh); 
	free(l); 
}

size_t lock_get_id(lock_t *l) {
  return l->id;
}

session_t *lock_get_session(lock_t *l) {
  return l->sesh;
}

bool lock_is_open(lock_t *l) {
  return l->sesh == NULL ? true : false;
}


// Key functions //////////
digikey_t *key_create() {
  digikey_t *k = calloc(1, sizeof(digikey_t)); 
  return k;
}

void key_destroy(digikey_t *k) {
	free(k->sesh); 
	free(k);
}

session_t *key_get_session(digikey_t *k) {
  return k->sesh;
}

bool connect_to_lock(lock_t *l, digikey_t *k) 
{
	if (l->sesh != NULL) // locked:
	{
		if (key_get_session(k) != lock_get_session(l)) // cmp key
		{
			return false; 
		}
	}
	

	if (key_get_session(k) == lock_get_session(l) && k->sesh != NULL)
	{
		session_drop(l->sesh); 
		l->sesh = NULL; 
		return true; 
	} 
	
	if (k->sesh == NULL)
	{
		k->sesh = session_create(); 
	}

	l->sesh = session_join(k->sesh); 
    return true;
}

#ifndef TEST
#include <stdio.h>

int main(int argc, char *argv[]) {
  puts("If you want to you can write test code here and run it using `make run`");
  return 0;
}
#endif