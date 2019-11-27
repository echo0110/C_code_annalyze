/****************************************************************************
 *    FILE NAME  : s_select.h
 *
 *    DESCRIPTION: This file contains types for the select API.
 *
 *    DATE     NAME         REFERENCE              REASON
 *    ----     ----         ---------              ------
 *    09Spe01  gsheoran         -                  Create Original
 *
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef _S_SELECT_H_SEEN_
#define _S_SELECT_H_SEEN_

#ifdef __cplusplus
extern "C" {
#endif

#include "s_cotyps.h"

#ifdef SCTP_UI_SOCK_API

    /* The Number of bits in the FD_SET */
#ifndef SCTP_FD_SETSIZE
#define SCTP_FD_SETSIZE 1024
#endif

    /* Number of bits ina byte. */
#define SCTP_NBBY          8

    typedef sctp_U32bit  sctp_fds_mask;

#define SCTP_NFDBITS    (sizeof(sctp_fds_mask) * SCTP_NBBY)

#define sctp_fd_total_elems(x, y)  (((x) + ((y) - 1))/ (y))

    typedef struct __sctp_fd_set
    {
        sctp_fds_mask fds_bits[sctp_fd_total_elems( SCTP_FD_SETSIZE, SCTP_NFDBITS )];
    } sctp_fd_set_st;

#define SCTP_FD_SET(__n, __p) (__p)->fds_bits[(__n)/SCTP_NFDBITS] |= \
        ((unsigned)1 << ((__n) % SCTP_NFDBITS))

#define SCTP_FD_CLR(__n, __p) (__p)->fds_bits[(__n)/SCTP_NFDBITS] &= \
        ~((unsigned)1 << ((__n) % SCTP_NFDBITS))

#define SCTP_FD_ISSET(__n, __p) ((__p)->fds_bits[(__n)/SCTP_NFDBITS] & \
                                 ((unsigned)1 << ((__n) % SCTP_NFDBITS)))

#define SCTP_FD_ZERO(__p)    memset((char *)(__p), 0, sizeof (*(__p)))


    /* Easy to use macros. */
#define __S_BIT(i)           (1UL << ((i) & (SCTP_NFDBITS - 1)))

    /* will give us the bit */

    /* Each Node in the select list is made of this. */
    typedef struct __sctp_select
    {
        struct node          __header__;

        /* Received Select flags */
        sctp_fd_set_st    *p_rfds;
        sctp_fd_set_st    *p_wfds;
        sctp_fd_set_st    *p_efds;
        sctp_time_t       *p_timeout;

        /* Updated select flags */
        sctp_fd_set_st    n_rfds;
        sctp_fd_set_st    n_wfds;
        sctp_fd_set_st    n_efds;

        sctp_U32bit       nfds;

        /* The Select condition variable. */
        sctp_cond_t       cond;
        sctp_lock_t       lock;

        /* The Select timer id */
        sctp_timer_t      timer_id;

        sctp_S32bit       retval;

    } sctp_select_st;


    /* The Select list */
    typedef struct __select_db
    {
        /* The List of select blocks */
        LIST         Select_free_list;

        /* The List of select blocks */
        LIST         Select_list;

        /* The Lock to protect the select list */
        sctp_lock_t  Select_list_lock;
    } sctp_select_db_st;

    extern sctp_select_db_st select_db;

#define SCTP_SEL_INIT_NODE(p_select) \
    p_select->p_rfds = SCTP_NULL;\
    p_select->p_wfds = SCTP_NULL;\
    p_select->p_efds = SCTP_NULL;\
    p_select->p_timeout = SCTP_NULL;\
    p_select->nfds   = 0;\
    SCTP_FD_ZERO(&p_select->n_rfds);\
    SCTP_FD_ZERO(&p_select->n_wfds);\
    SCTP_FD_ZERO(&p_select->n_efds);

    /* Get a new node from the free list */
#define SCTP_SEL_GET_NODE(p_node) \
    SCTP_LOCK(select_db.Select_list_lock);\
    p_node = (sctp_select_st*) lstGet(&select_db.Select_free_list);\
    SCTP_UNLOCK(select_db.Select_list_lock);

    /* Free the node back to the free list */
#define SCTP_SEL_FREE_NODE(p_node) \
    SCTP_LOCK(select_db.Select_list_lock);\
    lstAdd(&select_db.Select_free_list, (NODE*)p_node);\
    SCTP_UNLOCK(select_db.Select_list_lock);

    /* Add the node to the Select list */
#define SCTP_SEL_ATTACH_NODE(p_node) \
    SCTP_LOCK(select_db.Select_list_lock);\
    lstAdd(&select_db.Select_list, (NODE*)p_node);\
    SCTP_UNLOCK(select_db.Select_list_lock);

    /* Add the node to the Select list */
#define SCTP_SEL_DETACH_NODE(p_node) \
    SCTP_LOCK(select_db.Select_list_lock);\
    lstDelete(&select_db.Select_list, (NODE*)p_node);\
    SCTP_UNLOCK(select_db.Select_list_lock);

    /* Some Lock Macros */
#define SCTP_SLEEP_SELECT(select, mutex) SCTP_SLEEP(&(select.cond), &(mutex))
#define SCTP_WAKEUP_SELECT(select)       SCTP_WAKEUP(&(select.cond))


    int
    sctp_do_select(
        sctp_select_st      *p_select );

    int
    sctp_select_sleep_timeout(
        sctp_select_st      *p_select );

    void
    sctp_mark_ep_selected(
        sctp_select_st      *p_select,
        sctp_Boolean_t      onoff );

    void
    sctp_wakeup_selected(
        sctp_U32bit         ep_id );

    int
    sctp_select_get_no_fds_ready(
        sctp_select_st      *p_select,
        sctp_Boolean_t      lock );

    void
    sctp_handle_select_timeout(
        sctp_timer_t        timer_id );

    /* Changes for SPR#15820 */
    sctp_void_t
    sctp_lock_cleanup_func(
        sctp_void_t               *p_arg );
    /* Changes for SPR#15820 Ends */

#endif /* SCTP_UI_SOCK_API */

#ifdef __cplusplus
}
#endif


#endif /* _S_SELECT_H_SEEN */
