/* Minimal stand-in for gnulib's glthread/lock.h -- just the subset
   gettext-runtime/intl's *.c files actually use (grepped for gl_rwlock
   and __libc_lock macros): rwlocks, plain locks, and recursive locks.
   Not vendoring gnulib itself; see the top-level build.zig for context.

   On Windows, CRITICAL_SECTION is used for the plain/recursive locks
   (it's inherently reentrant, so the same implementation covers both),
   lazily initialized via an InterlockedCompareExchange guard since
   CRITICAL_SECTION has no static initializer. Reader/writer locks
   collapse "read" and "write" acquisition to the same exclusive SRWLOCK
   acquisition (real gnulib distinguishes them so unlock knows which
   release call to use) -- correct, just less concurrent than a true
   reader/writer lock; these locks only guard the translation-catalog
   cache, not a hot path. */
#ifndef _GLTHREAD_LOCK_H
#define _GLTHREAD_LOCK_H

#if defined _WIN32 && !defined __CYGWIN__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef SRWLOCK gl_rwlock_t;

#define gl_rwlock_define(STORAGECLASS, NAME) \
  STORAGECLASS gl_rwlock_t NAME;
#define gl_rwlock_define_initialized(STORAGECLASS, NAME) \
  STORAGECLASS gl_rwlock_t NAME = SRWLOCK_INIT;
#define gl_rwlock_init(NAME) InitializeSRWLock (&(NAME))
#define gl_rwlock_rdlock(NAME) AcquireSRWLockExclusive (&(NAME))
#define gl_rwlock_wrlock(NAME) AcquireSRWLockExclusive (&(NAME))
#define gl_rwlock_unlock(NAME) ReleaseSRWLockExclusive (&(NAME))
#define gl_rwlock_destroy(NAME) ((void) (NAME))

typedef struct
{
  CRITICAL_SECTION cs;
  LONG volatile initialized;
}
__libc_lock_t;

static __inline void
__libc_lock_ensure_init (__libc_lock_t *lock)
{
  switch (InterlockedCompareExchange (&lock->initialized, 1, 0))
    {
    case 0:
      InitializeCriticalSection (&lock->cs);
      InterlockedExchange (&lock->initialized, 2);
      break;
    default:
      while (lock->initialized != 2)
        Sleep (0);
      break;
    }
}

#define __libc_lock_define_initialized(CLASS, NAME) \
  CLASS __libc_lock_t NAME;
#define __libc_lock_lock(NAME) \
  (__libc_lock_ensure_init (&(NAME)), EnterCriticalSection (&(NAME).cs))
#define __libc_lock_unlock(NAME) LeaveCriticalSection (&(NAME).cs)

/* CRITICAL_SECTION is already reentrant, so the recursive variants reuse
   the exact same type/macros. */
typedef __libc_lock_t __libc_lock_recursive_t;
#define __libc_lock_define_initialized_recursive(CLASS, NAME) \
  CLASS __libc_lock_recursive_t NAME;
#define __libc_lock_lock_recursive(NAME) __libc_lock_lock (NAME)
#define __libc_lock_unlock_recursive(NAME) __libc_lock_unlock (NAME)

#else

#include <pthread.h>

typedef pthread_rwlock_t gl_rwlock_t;

#define gl_rwlock_define(STORAGECLASS, NAME) \
  STORAGECLASS gl_rwlock_t NAME;
#define gl_rwlock_define_initialized(STORAGECLASS, NAME) \
  STORAGECLASS gl_rwlock_t NAME = PTHREAD_RWLOCK_INITIALIZER;
#define gl_rwlock_init(NAME) pthread_rwlock_init (&(NAME), NULL)
#define gl_rwlock_rdlock(NAME) pthread_rwlock_rdlock (&(NAME))
#define gl_rwlock_wrlock(NAME) pthread_rwlock_wrlock (&(NAME))
#define gl_rwlock_unlock(NAME) pthread_rwlock_unlock (&(NAME))
#define gl_rwlock_destroy(NAME) pthread_rwlock_destroy (&(NAME))

typedef pthread_mutex_t __libc_lock_t;
#define __libc_lock_define_initialized(CLASS, NAME) \
  CLASS __libc_lock_t NAME = PTHREAD_MUTEX_INITIALIZER;
#define __libc_lock_lock(NAME) pthread_mutex_lock (&(NAME))
#define __libc_lock_unlock(NAME) pthread_mutex_unlock (&(NAME))

typedef pthread_mutex_t __libc_lock_recursive_t;
#define __libc_lock_define_initialized_recursive(CLASS, NAME) \
  CLASS __libc_lock_recursive_t NAME = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
#define __libc_lock_lock_recursive(NAME) pthread_mutex_lock (&(NAME))
#define __libc_lock_unlock_recursive(NAME) pthread_mutex_unlock (&(NAME))

#endif

#endif /* _GLTHREAD_LOCK_H */
