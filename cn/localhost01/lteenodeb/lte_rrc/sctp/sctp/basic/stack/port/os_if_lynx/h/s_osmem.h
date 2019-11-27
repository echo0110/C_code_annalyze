/***************************************************************************
 *  FILE NAME  : s_osmem.h
 *
 *  DESCRIPTION: Contains the Memory related macros.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  07Oct03    Vivek Bansal Rel 5.0      Changes for CSPL
 *  27Nov03    Vivek Bansal Rel 5.0      Changes for CSPL
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef __FILE_sctp_os_mem_SEEN__
#define __FILE_sctp_os_mem_SEEN__

#include <sig_os.h>

typedef enum
{
    SCTP_PDU_SEND_BUFFER_POOL,
    SCTP_APIS_MESG_POOL,
#ifdef SCTP_RED_SUPPORT
    SCTP_RDNCY_MESG_POOL,
#endif
    SCTP_MAX_MESG_POOL_IDS

} sctp_mesg_pool_type_et;

sctp_return_t sctp_init_mem_module( sctp_void_t );

sctp_void_t sctp_deinit_mem_module( sctp_void_t );

extern sigtran_pool_id_t   sctp_mesg_pool_ids[];

/* Use this macro to get/free the dynamic memory needed by stack */
#define __MALLOC(s)   sigtran_malloc((sigtran_U32bit)(s));

#define __FREE(s)     sigtran_free((sigtran_buffer_t)(s));

#define __MESG_MALLOC(pool_id,size)\
    sigtran_mesg_get_buffer\
    (SIG_SCTP_STACK, &(sctp_mesg_pool_ids[pool_id]),\
     size);

#define __MESG_FREE(p_buffer)\
    sigtran_mesg_free_buffer\
    ((sigtran_U32bit)SIG_SCTP_STACK,\
     (sigtran_buffer_t*)&p_buffer);


#define __MEMCOPY(t,f,n)  sigtran_memcpy( (sigtran_pvoid_t)(t), \
                                          (sigtran_pvoid_t)(f), (sigtran_U32bit)(n) )

#define __MEMSET(s,v,z)   sigtran_memset(s,v,z)

sctp_void_t  sctp_mesg_mem_free(
    sctp_Pvoid_t  p_data,
    sctp_Pvoid_t  p_free_args );

#endif /* __FILE_sctp_os_mem_SEEN__ */
