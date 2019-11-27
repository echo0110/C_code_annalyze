/******************************************************************************
 *    FILE NAME:  s_osmem.c
 *
 *    DESCRIPTION: This file implements the timer module funcions.
 *
 *    DATE          NAME           REFERENCE       REASON
 *    -----------------------------------------------------
 *    02June 2000   Sandeep Mahajan  -           .Original Creation
 *    16Aug01       ygahlaut         -            REL 3.0 Update
 *    07Oct03       Vivek Bansal     Rel 5.0      Changes for CSPL
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#define SCTP_MODULE "OS_MEM"

#include <sig_os.h>
#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_cobuf.h>
#include <s_colst.h>
#include <s_osmem.h>
#include <s_mmmain.h>
#include <s_stats.h>
#include <s_dbg.h>
#include <s_osmutx.h>
#include <s_es.h>
#include <s_red.h>
#include <s_comac.h>
#include <s_clco.h>

sctp_pool_id_t   sctp_mesg_pool_ids[SCTP_MAX_MESG_POOL_IDS];

/******************************************************************************
**  Function Name       : sctp_init_mem_module
**
**    Description       :
**      Initializes the memory module
**
**    Returns           :
**      SCTP_FAILURE
**      SCTP_SUCCESS
******************************************************************************/
sctp_return_t sctp_init_mem_module()
{
    sigtran_return_t        ret_val;
    sigtran_error_t         ecode;
    sctp_U32bit             num_mesg_pools = SCTP_MAX_MESG_POOL_IDS;
    sigtran_pool_info_st    pool_info[SCTP_MAX_MESG_POOL_IDS];

    /*
     * Number of message pools required
     * 1. For Redundancy - 1 os size SCTP_MAX_RED_MEM
     * 2. For APIs resp and notification - NUM_APIS of size SCTP_MAX_API_SIZE.
     */

    pool_info[SCTP_PDU_SEND_BUFFER_POOL].buf_size = sizeof( sig_iovec_ipc_buffer_st );
    pool_info[SCTP_PDU_SEND_BUFFER_POOL].num_buf = SCTP_MAX_SEND_BUFFERS;

    pool_info[SCTP_APIS_MESG_POOL].buf_size = SCTP_MAX_API_SIZE;
    pool_info[SCTP_APIS_MESG_POOL].num_buf = SCTP_MAX_NUM_APIS;

#ifdef SCTP_RED_SUPPORT
    pool_info[SCTP_RDNCY_MESG_POOL].buf_size = SCTP_MAX_RED_MEM;
    pool_info[SCTP_RDNCY_MESG_POOL].num_buf = 1;
#endif

    ret_val = sigtran_mem_init( SIG_SCTP_STACK, 0, 0, 0, num_mesg_pools,
                                pool_info, sctp_mesg_pool_ids, SIGTRAN_TRUE, &ecode );

    if( SIGTRAN_FAILURE == ret_val )
    {
        return SCTP_FAILURE;
    }

    return SCTP_SUCCESS;
}

/******************************************************************************
**  Function Name       : sctp_deinit_mem_module
**
**    Description       :
**      Deinitializes the memory module.
**
**    Returns           :
**      Void
******************************************************************************/
sctp_void_t sctp_deinit_mem_module()
{
    sigtran_error_t         ecode;

    sigtran_mem_deinit( SIG_SCTP_STACK, 0, 0, 0, SCTP_MAX_MESG_POOL_IDS,
                        0, sctp_mesg_pool_ids, SIGTRAN_TRUE, &ecode );
}

