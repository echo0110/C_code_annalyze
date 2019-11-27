/******************************************************************************
*
*   FILE NAME:
*       rrc_llim_utils.c
*
*   DESCRIPTION:
*       This header file contains implementation of LLIM specific utilities
*
*   DATE            AUTHOR      REFERENCE       REASON
*   14 May 2009     VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "rrc_llim_utils.h"
#include "rrc_llim_logger.h"
#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"
#include "rrc_intrl_api.h"
#include "rrc_ext_api.h"
#include "rrc_phy_intf.h"
#include "rrc_common_md.h"
#include "rrc_cp_common.h"
/******************************************************************************
*   FUNCTION NAME: rrc_llim_send_internal_msg
*
*   DESCRIPTION:
*       Send message only with RRC API Header from LLIM.
*       Is used for sending messages to other RRC submodules
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_t rrc_llim_send_internal_msg
(
    /* Destination module identifier */
    rrc_module_id_t             dst_module_id,
    U16                         api_id,         /* API Id */
    U16                         msg_size,       /* Size of message (payload) */
    /* Message specific info (payload) */
    void                        *p_msg
)
{
    U8 *p_api = PNULL;
    U16 msg_api_length = RRC_NULL;
    
    msg_api_length = (U16)(msg_size + RRC_API_HEADER_SIZE);

    RRC_LLIM_UT_TRACE_ENTER();

    /* Allocate buffer */
    p_api = rrc_msg_mem_get(msg_api_length);

    if (PNULL == p_api)
    {
        return RRC_FAILURE;
    }

    memset_wrapper(p_api, 0, msg_api_length);

    rrc_construct_api_header(   p_api, RRC_VERSION_ID, RRC_LLIM_MODULE_ID,
                                dst_module_id, api_id, msg_api_length);

    if((0 != msg_size) &&  (PNULL != p_msg))
    {
        l3_memcpy_wrapper(p_api + RRC_API_HEADER_SIZE, p_msg, msg_size);
    }

    /* Send message to */
    rrc_send_message(p_api, dst_module_id);

    RRC_LLIM_UT_TRACE_EXIT();

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: rrc_llim_alloc_internal_msg
*
*   DESCRIPTION:
*       Allocate memory and fill RRC API Header for internal message
*
*   RETURNS:
*       PNULL in failure case
*       pointer on the very begining of message
*
******************************************************************************/
void* rrc_llim_alloc_internal_msg
(
    /* Destination module identifier */
    rrc_module_id_t             dst_module_id,
    U16                         api_id,             /* API Id */
    /* Size of message (payload) */
    U16                         api_msg_size
)
{
    void *p_api = PNULL;
    U16 msg_api_length = RRC_NULL;
    
    msg_api_length =(U16) (api_msg_size + RRC_API_HEADER_SIZE);

    RRC_LLIM_UT_TRACE_ENTER();

    /* Allocate buffer */
    p_api = rrc_msg_mem_get(msg_api_length);

    if (PNULL != p_api)
    {
        memset_wrapper(p_api, 0, msg_api_length);

        /* Fill RRC header */
        rrc_construct_api_header(   p_api, RRC_VERSION_ID, RRC_LLIM_MODULE_ID,
                                    dst_module_id, api_id, msg_api_length);
    }

    RRC_LLIM_UT_TRACE_EXIT();
    return p_api;
}

/******************************************************************************
*   FUNCTION NAME: rrc_llim_alloc_external_msg
*
*   DESCRIPTION:
*       Allocate memory and fill RRC API Header and L1/L2 Header for external
*       message
*
*   RETURNS:
*       PNULL in failure case
*       pointer on the very begining of message
*
******************************************************************************/
void* rrc_llim_alloc_external_msg
(
    U16                         transaction_id,     /* Transaction identifier */
    /* Internal Destination module identifier */
    rrc_module_id_t             int_dst_module_id,
    /* External Destination module identifier */
    rrc_module_id_t             ext_dst_module_id,
    U16                         api_id,             /* API Id */
    /* Size of message (payload) */
    U16                         api_msg_size,
    /* cell index */
    U8                          cell_index
)
{
    void *p_api = PNULL;
    U16 msg_api_length = RRC_NULL;
    
    msg_api_length = (U16)(api_msg_size + RRC_FULL_INTERFACE_HEADERS_SIZE);

    RRC_LLIM_UT_TRACE_ENTER();

    /* Allocate buffer */
    p_api = rrc_msg_mem_get(msg_api_length);

    if (PNULL != p_api)
    {
        memset_wrapper(p_api, 0, msg_api_length);

        /* Fill RRC header */
        /* Internal module ID will vary according to Cell Id */
        rrc_construct_api_header(   p_api, RRC_VERSION_ID, RRC_LLIM_MODULE_ID,
                                   int_dst_module_id, api_id, msg_api_length);

        /* Fill interface header */
        rrc_construct_interface_api_header( (U8 *)p_api + RRC_API_HEADER_SIZE,
            transaction_id,
            RRC_LLIM_MODULE_ID, 
            ext_dst_module_id,
            api_id,
            api_msg_size, 
            cell_index);
    }

    RRC_LLIM_UT_TRACE_EXIT();
    return p_api;
}

/******************************************************************************
*   FUNCTION NAME: rrc_llim_send_csc_llim_config_cell_resp_failure
*
*   DESCRIPTION:
*       This function sends CSC_LLIM_CONFIG_CELL_RESP (FAILURE)
*           with given error code
*
*   RETURNS:
*       None
*
******************************************************************************/
void rrc_llim_send_csc_llim_config_cell_resp_failure
(
    /* Response code to send */
    rrc_llim_csc_response_et        response,
    /* Transaction identifier */
    rrc_transaction_id_t            transaction_id,
    rrc_cell_index_t                cell_index          /* Cell identifier */
)
{
    rrc_csc_llim_config_cell_resp_t config_cell_resp;

    memset_wrapper(&config_cell_resp, 0, sizeof(config_cell_resp));

    config_cell_resp.transaction_id = transaction_id;
    config_cell_resp.bitmask        = 0;
    config_cell_resp.cell_index     = cell_index;
    config_cell_resp.response       = response;

    rrc_llim_send_internal_msg( RRC_CSC_MODULE_ID, CSC_LLIM_CONFIG_CELL_RESP,
                                sizeof(config_cell_resp), &config_cell_resp);
}
/******************************************************************************
*   FUNCTION NAME: rrc_llim_send_csc_llim_reconfig_cell_resp_failure
*
*   DESCRIPTION:
*       This function sends CSC_LLIM_RECONFIG_CELL_RESP (FAILURE)
*           with given error code
*
*   RETURNS:
*       None
*
******************************************************************************/
rrc_return_et rrc_llim_send_csc_llim_reconfig_cell_resp_failure
(
    /* Response code to send */
    rrc_llim_csc_response_et        response,
    /* Transaction identifier */
    rrc_transaction_id_t            transaction_id,
    rrc_cell_index_t                cell_index          /* Cell identifier */
)
{
    rrc_csc_llim_reconfig_cell_resp_t reconfig_cell_resp;

    memset_wrapper(&reconfig_cell_resp, 0, sizeof(reconfig_cell_resp));

    reconfig_cell_resp.transaction_id = transaction_id;
    reconfig_cell_resp.cell_index     = cell_index;
    reconfig_cell_resp.response       = response;

    return (rrc_return_et)rrc_llim_send_internal_msg( RRC_CSC_MODULE_ID, CSC_LLIM_RECONFIG_CELL_RESP,
                                sizeof(reconfig_cell_resp), &reconfig_cell_resp);
}
/******************************************************************************
*   FUNCTION NAME: rrc_llim_send_csc_llim_cell_start_resp
*
*   DESCRIPTION:
*       This function sends CSC_LLIM_CELL_START_RESP
*
*   RETURNS:
*       None
*
******************************************************************************/
rrc_return_et rrc_llim_send_csc_llim_cell_start_resp
(
    /* Transaction identifier */
    rrc_transaction_id_t            transaction_id,
    rrc_cell_index_t                cell_index,          /* Cell identifier */
    rrc_llim_csc_response_et        response,
    rrc_sfn_t                       sfn,
    /*cdma_fix bug 10186 start*/
    rrc_sf_t                        sf,
    rrc_timestamp_t                 timestamp
   /*cdma_fix bug 10186 stop*/
)
{
    rrc_csc_llim_cell_start_resp_t cell_start_resp;

    RRC_LLIM_UT_TRACE_ENTER();
    memset_wrapper(&cell_start_resp, 0, sizeof(cell_start_resp));

    cell_start_resp.transaction_id = transaction_id;
    cell_start_resp.cell_index     = cell_index;
    cell_start_resp.response       = response;
    cell_start_resp.sfn            = sfn;
    /*cdma_fix bug 10186 start*/    
    cell_start_resp.sf             = sf;
    cell_start_resp.timestamp.ts_sec      = timestamp.ts_sec;
    cell_start_resp.timestamp.ts_nsec     = timestamp.ts_nsec;
   /*cdma_fix bug 10186 stop*/
    RRC_LLIM_UT_TRACE_EXIT();

    return (rrc_return_et)rrc_llim_send_internal_msg( RRC_CSC_MODULE_ID, CSC_LLIM_CELL_START_RESP,
            sizeof(cell_start_resp), &cell_start_resp);
}
/******************************************************************************
*   FUNCTION NAME: rrc_llim_send_csc_llim_cell_stop_resp
*
*   DESCRIPTION:
*       This function sends CSC_LLIM_CELL_STOP_RESP
*
*   RETURNS:
*       None
*
******************************************************************************/
rrc_return_et rrc_llim_send_csc_llim_cell_stop_resp
(
    /* Transaction identifier */
    rrc_transaction_id_t            transaction_id,
    rrc_cell_index_t                cell_index,          /* Cell identifier */
    /* Response code to send */
    rrc_llim_csc_response_et        response
)
{
    rrc_csc_llim_cell_stop_resp_t cell_stop_resp;

    memset_wrapper(&cell_stop_resp, 0, sizeof(rrc_csc_llim_cell_stop_resp_t));

    cell_stop_resp.transaction_id = transaction_id;
    cell_stop_resp.cell_index     = cell_index;
    cell_stop_resp.response       = response;

    return (rrc_return_et)rrc_llim_send_internal_msg( RRC_CSC_MODULE_ID, CSC_LLIM_CELL_STOP_RESP,
            sizeof(cell_stop_resp), &cell_stop_resp);
}

/******************************************************************************
*   FUNCTION NAME: rrc_llim_get_ue_context
*
*   DESCRIPTION:
*       This function gives ue context
*
*   RETURNS:
*       None
*
******************************************************************************/
void rrc_llim_get_ue_context
(
     rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx,  /* Pointer to the LLIM global context data */
     rrc_ue_index_t      ue_id,               /* UE INDEX */
     rrc_llim_ue_ctx_t   **p_p_ue_ctx         /* Pointer to the address of LLIM UE Context */
)
{
    rrc_module_id_t      uecc_module_id = RRC_GET_UECC_MODULE_INDEX(ue_id);
    rrc_ue_index_t       ue_index = RRC_GET_UE_INDEX(ue_id);
    
    RRC_ASSERT (PNULL != p_rrc_llim_gl_ctx);
    RRC_ASSERT (PNULL != p_rrc_llim_gl_ctx->p_p_uecc_ctx);

    if ( uecc_module_id >= p_rrc_llim_gl_ctx->no_of_uecc_instances)
    {
        return;
    }

    if(PNULL != p_rrc_llim_gl_ctx->p_p_uecc_ctx[uecc_module_id])
    {
        if (PNULL != p_rrc_llim_gl_ctx->p_p_uecc_ctx[uecc_module_id]->
                p_p_ue_ctx)
        {
            *p_p_ue_ctx = (rrc_llim_ue_ctx_t *)p_rrc_llim_gl_ctx->
                p_p_uecc_ctx[uecc_module_id]->
                p_p_ue_ctx[ue_index];
        }
    }

}

/******************************************************************************
*   FUNCTION NAME: rrc_llim_set_ue_context
*
*   DESCRIPTION:
*       This function assigns ue context in UE DB
*
*   RETURNS:
*       None
*
******************************************************************************/
void rrc_llim_set_ue_context
(
     rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx,  /* Pointer to the LLIM global context data */
     rrc_ue_index_t      ue_id,               /* UE INDEX */
     rrc_llim_ue_ctx_t   *p_p_ue_ctx         /* Pointer to the address of LLIM UE Context */
)
{
    rrc_module_id_t      uecc_module_id = RRC_GET_UECC_MODULE_INDEX(ue_id);
    rrc_ue_index_t       ue_index = RRC_GET_UE_INDEX(ue_id);

    RRC_ASSERT (PNULL != p_rrc_llim_gl_ctx);
    RRC_ASSERT (PNULL != p_rrc_llim_gl_ctx->p_p_uecc_ctx);

    if(PNULL != p_rrc_llim_gl_ctx->p_p_uecc_ctx[uecc_module_id])
    {
        if (PNULL != p_rrc_llim_gl_ctx->p_p_uecc_ctx[uecc_module_id]->p_p_ue_ctx)
        {
            p_rrc_llim_gl_ctx->p_p_uecc_ctx[uecc_module_id]->
                p_p_ue_ctx[ue_index] = (rrc_llim_ue_ctx_t *)p_p_ue_ctx;
        }
    }

}


/******************************************************************************
*   FUNCTION NAME: rrc_llim_get_cell_index
*
*   DESCRIPTION:
*       This function extracts the cell index from the API
*
*   RETURNS:
*       None
*
******************************************************************************/
void rrc_llim_get_cell_index(
   rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx,  /* Pointer to the LLIM global context data */
   void                *p_api,              /* Ponter to Received Message */
   rrc_cell_index_t    *p_cell_index          /* Pointer to Cell Index */
)
{
/*SPR 21653 changes start*/
    U8 length = 0;
/*SPR 21653 changes end*/

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT (PNULL != p_rrc_llim_gl_ctx); 
    RRC_ASSERT (PNULL != p_api);
    RRC_ASSERT (PNULL != p_cell_index);
/*SPR 21653 changes start*/
/*ifdef MULTISECTOR_ENABLE flag removed as that is always defined*/
/* cell index changes start */
    switch(rrc_get_src_module_id(p_api))
    {
        case RRC_MAC_MODULE_ID:
        case RRC_RLC_MODULE_ID:
        case RRC_S1U_MODULE_ID:
        case RRC_PDCP_MODULE_ID:
        case RRC_PHY_MODULE_ID:
            length = RRC_API_HEADER_SIZE + RRC_API_INTERFACE_HEADER_CELL_INDEX;
            rrc_cp_unpack_U8(p_cell_index, (U8 *)p_api + length, "cell_index");
            break;

        default:
            RRC_LLIM_TRACE(RRC_WARNING, "Msg received from Unexpected"
                    " Module[%d]", rrc_get_src_module_id(p_api));
            break;
    }
/*SPR 21653 changes end*/
    /*SPR 17777 +-*/
    /* SPR 19111 Fix Start */
    /* SPR 19221 fix start */
    (void )p_rrc_llim_gl_ctx;
    /* SPR 19221 fix stop*/
    /* SPR 19111 Fix Stop */   
    /*SPR 17777 +-*/         
    if( *p_cell_index > MAX_CELL_INDEX)
    {
        *p_cell_index = RRC_INVALID_CELL_INDEX;
    }

    /* cell index changes stop */
    RRC_LLIM_UT_TRACE_EXIT();
}


/******************************************************************************
*   FUNCTION NAME:  llim_find_index_by_id
*
*   DESCRIPTION:    Function finds the row in table which corresponds
*                   specified id.
*                   Each table element should correspond to the following
*                   conventions:
*                   - each table element is structure;
*                   - the first element field has type llim_id_t.
*
*   RETURNS:        Table row index or LLIM_LAST_ID if row with such id
*                   isn't found.
******************************************************************************/
U16 llim_find_index_by_id
(
    U16         id,
    llim_id_t*  p_table,
    U16         element_size
)
{
    U16 result = LLIM_LAST_ID;
    U16 index = 0;
    while ( LLIM_LAST_ID != p_table->id )
    {
        if ( p_table->id == id )
        {
            result = index;
            break;
        }
        index++;

        p_table= (llim_id_t*) ((U8*)p_table + element_size);
    }
    return result;
}

