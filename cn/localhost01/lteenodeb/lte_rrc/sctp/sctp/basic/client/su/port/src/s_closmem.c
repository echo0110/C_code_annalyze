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
 *    25Oct13       Pinky Sinha      SPR 21157    CSR 56258
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#define SCTP_MODULE "OS_MEM"

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
#include <s_closmem.h>

static sctp_pool_id_t sctp_cli_mesg_pool_id;
static sctp_pool_id_t sctp_cli_mem_pool_id;

/******************************************************************************
**  Function Name       : sctp_client_get_mesg_buffer
**
**    Description       : Gets message buffer of size(IN) from memory pools
**
**    Returns           : Buffer of void pointer type sctp_Pvoid_t
**
**    Functions called  : sigtran_mesg_get_buffer
:
**
******************************************************************************/
sctp_Pvoid_t sctp_client_get_mesg_buffer( sctp_U32bit size )
{
    return sigtran_mesg_get_buffer( SIG_SCTP_CLIENT,
                                    &( sctp_cli_mesg_pool_id ), size );
}

/******************************************************************************
**  Function Name       : sctp_client_free_mesg_buffer
**
**    Description       : Frees the buffer passed as parameter.
**
**    Returns           : SCTP_FAILURE OR SCTP_SUCCESS
**
**    Functions called  : sigtran_mesg_free_buffer()
**
******************************************************************************/
sctp_return_t sctp_client_free_mesg_buffer( sctp_U8bit **p_p_buffer )
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
**  Function Name       : sctp_client_init_mem_module
**
**    Description       : Initializes SCTP client memory module
**
**    Returns           : SCTP_FAILURE OR SCTP_SUCCESS
**
**    Functions called  : sigtran_mem_init
**
******************************************************************************/
sctp_return_t sctp_client_init_mem_module()
{
    sigtran_return_t        ret_val;
    sigtran_error_t         ecode;
    sigtran_pool_info_st    pool_info;

    pool_info.buf_size = SCTP_MAX_API_SIZE;
    pool_info.num_buf = SCTP_MAX_NUM_APIS * 2; /* One from SU/SM and one from the
                                                lower layer */

    ret_val = sigtran_mem_init( SIG_SCTP_CLIENT, 0, 0, 0, 1, &pool_info,
                                &sctp_cli_mesg_pool_id, SIGTRAN_TRUE, &ecode );

    if( SIGTRAN_FAILURE == ret_val )
    {
        return SCTP_FAILURE;
    }

    pool_info.buf_size = SCTP_MAX_API_SIZE;
    pool_info.num_buf = 1;

    ret_val = sigtran_mem_init( SIG_SCTP_CLIENT, 0, 0, 0, 1, &pool_info,
                                &sctp_cli_mem_pool_id, SIGTRAN_FALSE, &ecode );

    if( SIGTRAN_FAILURE == ret_val )
    {
        return SCTP_FAILURE;
    }

    return SCTP_SUCCESS;
}

/******************************************************************************
**  Function Name       : sctp_client_deinit_mem_module
**
**    Description       : De-Initializes SCTP client memory module
**
**    Returns           : void
**
**    Functions called  : sigtran_mem_deinit
**
******************************************************************************/
sctp_void_t sctp_client_deinit_mem_module()
{
    sigtran_error_t         ecode;

    sigtran_mem_deinit( SIG_SCTP_CLIENT, 0, 0, 0, SCTP_MAX_MESG_POOL_IDS,
                        SIGTRAN_NULL, &sctp_cli_mesg_pool_id, SIGTRAN_TRUE, &ecode );

    /* start spr 21157 */
    sigtran_mem_deinit( SIG_SCTP_CLIENT, 0, 0, 0, 1,
                        SIGTRAN_NULL, &sctp_cli_mem_pool_id, SIGTRAN_FALSE, &ecode );
    /* end spr 21157 */
}


/******************************************************************************
**  Function Name       : sctp_client_get_buffer
**
**    Description       : Gets buffer of requested size from memory pools
**
**    Returns           : buffer of void pointer type sctp_Pvoid_t
**
**    Functions called  : sigtran_get_buffer
**
******************************************************************************/
sctp_Pvoid_t sctp_client_get_buffer( sctp_U32bit size )
{
    return sigtran_get_buffer( SIG_SCTP_CLIENT, SIGTRAN_FALSE,
                               &sctp_cli_mem_pool_id, size );
}

/******************************************************************************
**  Function Name       : sctp_client_free_buffer
**
**    Description       : Frees the buffer passed as parameter.
**
**    Returns           : SCTP_FAILURE OR SCTP_SUCCESS
**
**    Functions called  : sigtran_free_buffer()
**
******************************************************************************/
sctp_return_t sctp_client_free_buffer( sctp_Pvoid_t p_buffer )
{
    sigtran_return_t ret_val;

    ret_val = sigtran_free_buffer( SIG_SCTP_CLIENT, SIGTRAN_FALSE,
                                   &sctp_cli_mem_pool_id, ( sigtran_buffer_t * )&p_buffer );

    if( SIGTRAN_FAILURE == ret_val )
    {
        return SCTP_FAILURE;
    }

    return SCTP_SUCCESS;
}

