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
#define SCTP_MODULE "SM OS_MEM"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_cobuf.h>
#include <s_colst.h>
#include <s_osmem.h>
#include <s_mmmain.h>
#include <s_stats.h>
#include <s_dbg.h>
#include <s_osmem.h>
#include <s_osmutx.h>
#include <s_es.h>
#include <s_red.h>
#include <s_comac.h>
#include <s_clco.h>
#include <s_closmemsm.h>

static sigtran_pool_id_t sctp_cli_sm_mesg_pool_id;

/******************************************************************************
**  Function Name       : sctp_client_sm_get_mesg_buffer
**
**    Description       : This function gets the buffer on the basis of pool_id
**
**    Returns           : void pointer
**
**    Functions called  :
**
******************************************************************************/
sctp_Pvoid_t sctp_client_sm_get_mesg_buffer( sctp_U32bit size )
{
    return sigtran_mesg_get_buffer( SIG_SCTP_CLIENT,
                                    &sctp_cli_sm_mesg_pool_id, size );
}

/******************************************************************************
**  Function Name       : sctp_client_sm_free_mesg_buffer
**
**    Description       : This function frees the buffer passed as parameter.
**
**    Returns           : SCTP_FAILURE
**                        SCTP_SUCCESS
**
**    Functions called  :
**
******************************************************************************/
sctp_return_t sctp_client_sm_free_mesg_buffer( sctp_U8bit **p_p_buffer )
{
    sigtran_return_t ret_val;

    ret_val = sigtran_mesg_free_buffer( SIG_SCTP_CLIENT, p_p_buffer );

    if( SIGTRAN_FAILURE == ret_val )
    {
        return SCTP_FAILURE;
    }

    return SCTP_SUCCESS;
}

/******************************************************************************
**  Function Name       : sctp_client_sm_init_mem_module
**
**    Description       : This function initializes the memory pool
**
**    Returns           : SCTP_SUCCESS
**                        SCTP_FAILURE
**
**    Functions called  :
**
******************************************************************************/
sctp_return_t sctp_client_sm_init_mem_module()
{
    sigtran_return_t        ret_val;
    sigtran_error_t         ecode;
    sigtran_pool_info_st    pool_info;

    pool_info.buf_size = SCTP_MAX_API_SIZE;
    pool_info.num_buf = SCTP_SM_MAX_NUM_APIS; /* One from SM and one for redundancy */

#ifdef SCTP_RED_SUPPORT
    pool_info.buf_size = SCTP_MAX_UDPSIZE;
    pool_info.num_buf = SCTP_SM_MAX_NUM_APIS; /* One from SM and one for redundancy */
#else
    pool_info.buf_size = SCTP_MAX_MTUSIZE;
    pool_info.num_buf = SCTP_SM_MAX_NUM_APIS; /* One from SM and one for redundancy */
#endif

    ret_val = sigtran_mem_init( SIG_SCTP_CLIENT, 0, 0, 0, 1, &pool_info,
                                &sctp_cli_sm_mesg_pool_id, SIGTRAN_TRUE, &ecode );

    if( SIGTRAN_FAILURE == ret_val )
    {
        return SCTP_FAILURE;
    }

    return SCTP_SUCCESS;
}

/******************************************************************************
**  Function Name       : sctp_client_sm_deinit_mem_module
**
**    Description       : This function deinitializes the memory manager
**
**    Returns           : void
**
**    Functions called  :
**
******************************************************************************/
sctp_void_t sctp_client_sm_deinit_mem_module()
{
    sigtran_error_t         ecode;

    sigtran_mem_deinit( SIG_SCTP_CLIENT, 0, 0, 0, SCTP_MAX_MESG_POOL_IDS,
                        SIGTRAN_NULL, &sctp_cli_sm_mesg_pool_id, SIGTRAN_TRUE, &ecode );
}
