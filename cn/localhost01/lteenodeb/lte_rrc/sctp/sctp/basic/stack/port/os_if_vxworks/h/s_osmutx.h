/***************************************************************************
 *  FILE NAME  : s_osmutx.h
 *
 *  DESCRIPTION: Contains the Locking/unlocking functionality.
 *
 *  DATE       NAME         REF#         REASON
 *  ------------------------------------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  07Oct03    Vivek Bansal Rel 5.0      Changes for CSPL
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef __FILE_sctp_mutex_SEEN__
#define __FILE_sctp_mutex_SEEN__

#include <s_os.h>
#include <sig_os.h>

typedef sigtran_mutex_t     sctp_lock_t;
typedef sigtran_thr_cond_t  sctp_cond_t;
typedef sigtran_sem_t       sctp_sema_t;

/* Macros for semaphores */
#define    __SEMAWAIT(P)        sigtran_sem_wait(P)
#define    __SEMAPOST(P)        sigtran_sem_post(P)
#define    __SEMAINIT(P, cnt)   sigtran_sem_init(P)
#define    __SEMADEINIT(P)      sigtran_sem_deinit(P)

/* Macros that access the pthread_cond_t variables :conditions */
#define __COND_WAKEUP(c)         sigtran_thr_cond_signal(&(c))
#define __COND_SLEEP(c, mutex)   sigtran_thr_cond_wait(&(c), &(mutex))
#define __COND_INIT(c)           sigtran_thr_cond_init(&(c))
#define __COND_DESTROY(c)        sigtran_thr_cond_deinit(&(c))

/* Macros for mutex : Just used for mutal exclusion */
#define sctp_mutex_init(a,b)    sigtran_mutex_init(&a)

#define sctp_mutex_destroy(a)    sigtran_mutex_deinit(&(a))

#define __UNLOCK(c)              sigtran_mutex_unlock(&c)
#define __LOCK(c)                sigtran_mutex_lock(&c)
#define __TRYLOCK(c)             sigtran_mutex_trylock(&c)
#define __LOCK_INIT(c)           sigtran_mutex_init(&c)

#define __LOCK_FLUSH(c)          sigtran_mutex_trylock(&c)


/*
 * The SCTP Global Lock -> Locks the main DB database.
 */
#define SCTPLOCK()      {SCTP_TRACE(SCTP_NULL, SCTP_DBG_TRC, ("\n\nLock\n")); __LOCK(sctp_db_main.lock);}
#define SCTPUNLOCK()      {SCTP_TRACE(SCTP_NULL, SCTP_DBG_TRC, ("\n\nUNLock\n")); __UNLOCK(sctp_db_main.lock);}


/*
 * Macros to Lock/Unlock/Flush an Endpoint Lock.
 */
#define SCTP_INIT_LOCK(lock)     __LOCK_INIT(lock)
#define SCTP_DESTROY_LOCK(lock)  sctp_mutex_destroy(lock)
#define SCTP_FLUSH_LOCK(lock)    __LOCK_FLUSH(lock)
#define SCTP_LOCK(lock)          __LOCK(lock)
#define SCTP_UNLOCK(lock)        __UNLOCK(lock)

#define SCTP_INIT_SEMA(sema, cnt) __SEMAINIT(sema, cnt)
#define SCTP_SEMA_WAIT(sema)      __SEMAWAIT(sema)
#define SCTP_SEMA_POST(sema)      __SEMAPOST(sema)

#define SCTP_SEMA_SLEEP(mut, sem) __UNLOCK(mut);__SEMAWAIT(sem);__LOCK(mut)

/* Locking specifically for the endpoint */
#define SCTP_LOCK_EP(ep_id)      {SCTP_TRACE(SCTP_NULL, SCTP_DBG_TRC, ("\n\nLock ep %d\n", (int)ep_id)); __LOCK(sctp_db_main.ep_table[ep_id].lock);}
#define SCTP_UNLOCK_EP(ep_id)    {SCTP_TRACE(SCTP_NULL, SCTP_DBG_TRC, ("\n\nUnlock ep %d\n", (int)ep_id)); __UNLOCK(sctp_db_main.ep_table[ep_id].lock);}
#define SCTP_TRYLOCK_EP(ep_id)   __TRYLOCK(sctp_db_main.ep_table[ep_id].lock)


/*
 * Macros to access the conditional valiables.
 */
#define SCTP_SLEEP(cond, mutex)  __COND_SLEEP(cond, mutex)
#define SCTP_WAKEUP(cond)        __COND_WAKEUP(cond)
#define SCTP_COND_INIT(cond)     __COND_INIT(cond)
#define SCTP_COND_DESTROY(cond)  __COND_DESTROY(cond)
/* Used only by the name resolver */
#define    SCTP_RESOLVER_LOCK()    \
    sigtran_mutex_lock(&sctp_cookie_resolver_mutex)

#define    SCTP_RESOLVER_UNLOCK()  \
    sigtran_mutex_unlock(&sctp_cookie_resolver_mutex)

#endif /* __FILE_sctp_mutex_SEEN__ */

