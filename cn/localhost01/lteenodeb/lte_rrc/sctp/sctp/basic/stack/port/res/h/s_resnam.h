/***************************************************************************
 *  FILE NAME  : s_resnam.h
 *
 *  DESCRIPTION: Contains the Name Resolution Module.
 *
 *  DATE     NAME          REF#          REASON
 *  -----------------------------------------
 *  4Apr00   sigtran team   -            Original
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef __FILE_sctp_res_SEEN__
#define __FILE_sctp_res_SEEN__

#ifdef SCTP_NAME_RESOLV_THREAD
#include <s_osmutx.h>

extern sctp_lock_t          sctp_cookie_resolver_mutex;
extern sctp_sema_t          cookie_queue_count;
extern sigtran_thread_t     name_res_thread;

extern LIST                 recv_to_res_queue;
extern LIST                 res_to_recv_queue;

extern sctp_return_t        sctp_hostname_resolver_init( void );
extern sctp_return_t        sctp_hostname_resolver_deinit( void );

#endif /* SCTP_NAME_RESOLV_THREAD */

#endif /* __FILE_sctp_res_SEEN__ */
