#include <stdbool.h>
#include <stdlib.h>

typedef struct lock lock_t;
typedef struct digikey digikey_t;
typedef struct session session_t;

// Session functions //////////

/** Create a new session

    @return a newly created session with refcount 1
    @note this should not be used outside of the library
*/
session_t *session_create();

/** Join an existing session, increasing its refcount

    @param s the session to join
    @return the session s
    @note this should not be used outside of the library
*/
session_t *session_join(session_t *s);

/** Drop an existing session, decreasing its refcount (freeing the session if it reaches 0)

    @param s the session to drop
    @return the session s if its new refcount is larger than 0, NULL if its refcount is 0
    @note this should not be used outside of the library
*/
session_t *session_drop(session_t *s);


// Lock functions //////////

/** Create a new lock without a connected session

    @param id the identifier of the lock
    @return a newly created lock
*/
lock_t *lock_create(size_t id);

/** Destroy a lock, dropping its existing session if any

    @param l the lock to destroy
*/
void lock_destroy(lock_t *l);

/** Get the identifier of a lock

    @param l the lock to be destroyed
*/
size_t lock_get_id(lock_t *l);

/** Get the session of a lock

    @param l a lock
    @return the session of l (NULL if not connected to a session)
*/
session_t *lock_get_session(lock_t *l);

/** Check whether a lock is open

    @param l a lock
    @return true if l is open (i.e. not connected to a session), otherwise false
*/
bool lock_is_open(lock_t *l);


// Key functions //////////

/** Create a new key without a connected session

    @return a newly created key
*/
digikey_t *key_create();

/** Destroy a key, dropping its existing session if any

    @param k the lock to be destroyed
*/
void key_destroy(digikey_t *k);

/** Get the session of a lock

    @param k a lock
    @return the session of k (NULL if not connected to a session)
*/
session_t *key_get_session(digikey_t *k);

/** Connect a lock to a key, either locking the lock by making it
    share session with the key or unlocking it by dropping its
    session. Do nothing if the lock is already locked by another
    key.

    @param l the lock to connect to
    @param k the key to connect
    @return true if l could be locked or unlocked, false if l was
    already locked by another key
*/
bool connect_to_lock(lock_t *l, digikey_t *k);
