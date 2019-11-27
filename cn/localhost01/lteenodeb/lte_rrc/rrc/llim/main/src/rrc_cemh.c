/******************************************************************************
 *
 *   FILE NAME:
 *       rrc_cemh.c
 *
 *   DESCRIPTION:
 *       This header file contains in points of CEMH submodule
 *
 *   DATE            AUTHOR      REFERENCE       REASON
 *   14 May 2009     VasylS      ---------       Initial
 *
 *   Copyright (c) 2009, Aricent Inc. All Rights Reserved
 *
 ******************************************************************************/
#include "rrc_cemh.h"
#include "rrc_defines.h"
#include "rrc_llim_logger.h"
#include "rrc_common_utils.h"
#include "rrc_llim_cell_ctx.h"
#include "rrc_cemh_timers.h"
#include "rrc_cemh_fsm.h"
#include "rrc_msg_mgmt.h"
#include "rrc_llim_utils.h"
#include "rrc_intrl_api.h"
#include "rrc_csc_llim_intf.h"
#include "rrc_mac_intf.h"
#include "rrc_mac_il_parser.h"
#include "rrc_mac_il_composer.h"
#include "rrc_phy_intf.h"
#include "rrc_phy_il_parser.h"
#include "rrc_ext_api.h"
#include "rrc_llim_ue_ctx.h"
#include "rrc_common_md.h"
#include "rrc_uemh.h"
#include "rrc_rlc_intf.h"
#include "rrc_rlc_il_parser.h"


/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_csc_llim_config_cell_req_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process CSC_LLIM_CONFIG_CELL_REQ message
 *                  and call FSM.
 ******************************************************************************/
void rrc_cemh_csc_llim_config_cell_req_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_csc_llim_config_cell_req_t *p_config_cell_req = PNULL;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_csc_llim_config_cell_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_config_cell_req = (rrc_csc_llim_config_cell_req_t *)((U8* )p_api +
            RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_config_cell_req->cell_index);

    if((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL !=  p_rrc_llim_gl_ctx->p_p_cell_ctx[p_config_cell_req->cell_index]))
    {
        /* Already exist */
        rrc_llim_send_csc_llim_config_cell_resp_failure
            (RRC_LLIM_CELL_ALREADY_EXIST,
             p_config_cell_req->transaction_id,
             p_config_cell_req->cell_index);
    }
    else
    {
        /* Create and add to DB */
        p_cell_ctx = (rrc_llim_cell_ctx_t *)rrc_mem_get(sizeof
                (rrc_llim_cell_ctx_t));
        if (PNULL == p_cell_ctx)
        {
            rrc_llim_send_csc_llim_config_cell_resp_failure
                (RRC_LLIM_CELL_NOT_ENOUGH_MEMORY,
                 p_config_cell_req->transaction_id,
                 p_config_cell_req->cell_index);
        }
        else
        {
            memset_wrapper(p_cell_ctx, 0, sizeof(rrc_llim_cell_ctx_t));

            /*Cell Context stored at base address+cell_index */
            p_rrc_llim_gl_ctx->p_p_cell_ctx[p_config_cell_req->cell_index] 
                = p_cell_ctx;
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/
            /* Init main IEs of cell context */
            p_cell_ctx->cell_index = p_config_cell_req->cell_index;
            p_cell_ctx->mac_retries_left = RRC_LLIM_CELL_NUM_MAX_MAC_TRIES;

            CEMH_FSM_INIT(p_cell_ctx);

            /* Normal message processing */
            cemh_fsm_process_event( CEMH_EVENT_CSC_CONFIG_CELL_REQ,
                    p_api,
                    p_config_cell_req,
                    p_cell_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_csc_llim_delete_cell_req_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process CSC_LLIM_DELETE_CELL_REQ message
 *                  and call FSM.
 ******************************************************************************/
void rrc_cemh_csc_llim_delete_cell_req_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_csc_llim_delete_cell_req_t *p_delete_cell_req = PNULL;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_csc_llim_delete_cell_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_delete_cell_req = (rrc_csc_llim_delete_cell_req_t *)((U8* )p_api +
            RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_delete_cell_req->cell_index);

    /* Search in DB cell with p_delete_cell_req->cell_index -
     *  should be present */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[p_delete_cell_req->cell_index])) ||
            ((p_rrc_llim_gl_ctx->p_p_cell_ctx[p_delete_cell_req->cell_index]->
              cell_index)!= p_delete_cell_req->cell_index) )
    {
        rrc_csc_llim_delete_cell_resp_t delete_cell_resp;
        delete_cell_resp.transaction_id = p_delete_cell_req->transaction_id;
        delete_cell_resp.cell_index     = p_delete_cell_req->cell_index;
        delete_cell_resp.response       = RRC_LLIM_CELL_IS_ABSENT;

        if(RRC_SUCCESS!=  rrc_llim_send_internal_msg(RRC_CSC_MODULE_ID, CSC_LLIM_DELETE_CELL_RESP,
                    sizeof(delete_cell_resp), &delete_cell_resp))
        {
            RRC_LLIM_TRACE(RRC_WARNING, "Unable to send meesage CSC_LLIM_DELETE_CELL_RESP");

        }

    }
    /*SPR 21369 Start*/
    else if(RRC_ONE==p_delete_cell_req->l2_instance_reset_flag)
    {
        rrc_csc_llim_delete_cell_resp_t delete_cell_resp;
        delete_cell_resp.transaction_id = p_delete_cell_req->transaction_id;
        delete_cell_resp.cell_index     = p_delete_cell_req->cell_index;
        delete_cell_resp.response       = RRC_LLIM_CSC_SUCCESS;
        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[p_delete_cell_req->cell_index];
        if(RRC_SUCCESS!=  rrc_llim_send_internal_msg(RRC_CSC_MODULE_ID, CSC_LLIM_DELETE_CELL_RESP,
                    sizeof(delete_cell_resp), &delete_cell_resp))
        {
            RRC_LLIM_TRACE(RRC_WARNING, "Unable to send meesage CSC_LLIM_DELETE_CELL_RESP");

        }
        /* Delete from Cell DB and free */
        p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_ctx->cell_index] = PNULL;
        rrc_mem_free(p_cell_ctx);



    }
    /*SPR 21369 End*/
    else
    {
        /* Bug 4450 Start */
        /* Cleanup UEs before deleting the cells*/
        /* Now UE Context will be cleaned per uecc basis */
        rrc_uemh_cleanup_ues(p_api, p_rrc_llim_gl_ctx); 
        /* Bug #4450 Stop */

        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[p_delete_cell_req->cell_index];

        /* Normal message processing */
        cemh_fsm_process_event( CEMH_EVENT_CSC_DELETE_CELL_REQ,
                p_api,
                p_delete_cell_req,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_csc_llim_pcch_data_req_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function fully processes CSC_LLIM_PCCH_DATA_REQ message.
 ******************************************************************************/
void rrc_cemh_csc_llim_pcch_data_req_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_csc_llim_pcch_data_req_t *p_pcch_data_req = PNULL;
    U8 *p_curr_pos = PNULL;
    void *p_msg = PNULL;

    rrc_mac_pcch_msg_req_t mac_pcch_msg_req;
    memset_wrapper(&mac_pcch_msg_req,0,sizeof(rrc_mac_pcch_msg_req_t));

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    p_pcch_data_req = (rrc_csc_llim_pcch_data_req_t *)((U8* )p_api +
            RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_pcch_data_req->cell_index);

    if (    (rrc_get_api_buf_size(p_api) < RRC_API_HEADER_SIZE + sizeof
                (rrc_csc_llim_pcch_data_req_t)) ||
            /* We have enough length for mandatory params, check data buffer size */
            rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_csc_llim_pcch_data_req_t) + p_pcch_data_req->
            paging_msg_buffer_length)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    /* Cell should exist */
    if ((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[p_pcch_data_req->cell_index])))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "Cell doesn't exist - ignore.");
    }
    else
    {

        /* Fill RRC_MAC_PCCH_MSG_REQ message */
        mac_pcch_msg_req.paging_frame       = p_pcch_data_req->paging_frame;
        mac_pcch_msg_req.paging_sub_frame   = p_pcch_data_req->paging_sub_frame;
        mac_pcch_msg_req.num_of_sfn         = p_pcch_data_req->num_of_sfn;


        /* Allocate buffer for storing RRC_MAC_PCCH_MSG_REQ message */
        p_msg = rrc_llim_alloc_external_msg(p_pcch_data_req->transaction_id,
                RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID, p_pcch_data_req->cell_index),
                RRC_MAC_MODULE_ID,
                RRC_MAC_PCCH_MSG_REQ,
                (U16)(rrc_il_get_rrc_mac_pcch_msg_req_len(
                        &mac_pcch_msg_req) + p_pcch_data_req
                    ->paging_msg_buffer_length),p_pcch_data_req->cell_index);

        if (PNULL != p_msg)
        {
            p_curr_pos = (U8*)p_msg + RRC_FULL_INTERFACE_HEADERS_SIZE;
            /* Compose RRC_MAC_PCCH_MSG_REQ */
            rrc_il_compose_rrc_mac_pcch_msg_req(&p_curr_pos, &mac_pcch_msg_req);

            l3_memcpy_wrapper( p_curr_pos,
                    p_pcch_data_req->paging_msg_buffer,
                    p_pcch_data_req->paging_msg_buffer_length);

            rrc_send_message(p_msg,
			RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID,p_pcch_data_req->cell_index));
                     
        }
    }

    RRC_LLIM_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_phy_config_cell_cnf_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process RRC_PHY_CONFIG_CELL_CNF message
 *                  and call FSM.
 ******************************************************************************/
void rrc_cemh_phy_config_cell_cnf_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_phy_config_cell_cnf_t phy_config_cell_cnf;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;
    S32 length_read = RRC_NULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_phy_config_cell_cnf(
                &phy_config_cell_cnf,
                (U8* )p_api +
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) -
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    /* Search in DB cell with phy_config_cell_cnf.cell_index -
     *  should be present */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == (p_rrc_llim_gl_ctx-> p_p_cell_ctx[phy_config_cell_cnf.
            cell_index])) || (p_rrc_llim_gl_ctx->
            p_p_cell_ctx[phy_config_cell_cnf.cell_index]->cell_index !=
                phy_config_cell_cnf.cell_index) )
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "Cell context not found - ignore.");
    }
    else
    {
        SET_CELL_INDEX(phy_config_cell_cnf.cell_index);

        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[phy_config_cell_cnf.cell_index];

        /* Normal message processing */
        cemh_fsm_process_event( CEMH_EVENT_PHY_CONFIG_CELL_CNF,
                p_api,
                &phy_config_cell_cnf,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_mac_config_cell_cnf_process_msg
 *
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process RRC_MAC_CONFIG_CELL_CNF message
 *                  and call FSM.
 ******************************************************************************/
void rrc_cemh_mac_config_cell_cnf_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_mac_config_cell_cnf_t mac_config_cell_cnf;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_mac_config_cell_cnf(
                &mac_config_cell_cnf,
                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) -
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
     
    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api,
            &cell_index);

    SET_CELL_INDEX(cell_index);

    if (RRC_INVALID_CELL_INDEX == cell_index)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Invalid Cell Index");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    
    /* Search in DB cell with ... - should be present */
    if ((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == (p_rrc_llim_gl_ctx->
                p_p_cell_ctx[cell_index])))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "Cell doesn't exist - ignore.");
    }
    else
    {
        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[cell_index];

        /* Normal message processing */
        cemh_fsm_process_event( CEMH_EVENT_MAC_CONFIG_CELL_CNF,
                p_api,
                &mac_config_cell_cnf,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_mac_cell_start_cnf_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process RRC_MAC_CELL_START_CNF message
 *                  and call FSM.
 ******************************************************************************/
void rrc_cemh_mac_cell_start_cnf_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_mac_cell_start_cnf_t mac_cell_start_cnf;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;
    S32 length_read = RRC_NULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_mac_cell_start_cnf(
                &mac_cell_start_cnf,
                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) -
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api,
            &cell_index);

    SET_CELL_INDEX(cell_index);

    if (RRC_INVALID_CELL_INDEX == cell_index)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Invalid Cell Index");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    
    /* Search in DB cell with ... - should be present */
    if ((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[cell_index])))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "Cell doesn't exist - ignore.");
    }
    else
    {
        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[cell_index];

        /* Normal message processing */
        cemh_fsm_process_event( CEMH_EVENT_MAC_CELL_START_CNF,
                p_api,
                &mac_cell_start_cnf,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_mac_cell_stop_cnf_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process RRC_MAC_CELL_STOP_CNF message
 *                  and call FSM.
 ******************************************************************************/
void rrc_cemh_mac_cell_stop_cnf_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_llim_cell_ctx_t        *p_cell_ctx = PNULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* spr 10203 fix start */

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api,
            &cell_index);

    SET_CELL_INDEX(cell_index);

    if (RRC_INVALID_CELL_INDEX == cell_index)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Invalid Cell Index");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    /* Search in DB cell with ... - should be present */
    if ((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[cell_index])))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "Cell doesn't exist - ignore.");
    }
    else
    {
        p_cell_ctx = p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index];

        /* Normal message processing */
        cemh_fsm_process_event( CEMH_EVENT_MAC_CELL_STOP_CNF,
                p_api,/*coverity fix start*/
                p_api,/*coverity fix stop*/
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }
    /* spr 10203 fix stop */

    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_mac_reconfig_cell_cnf_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process RRC_MAC_CONFIG_CELL_CNF message
 *                  and call FSM.
 ******************************************************************************/
void rrc_cemh_mac_reconfig_cell_cnf_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_mac_reconfig_cell_cnf_t mac_reconfig_cell_cnf;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_mac_reconfig_cell_cnf(
                &mac_reconfig_cell_cnf,
                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) -
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api,
             &cell_index);
    
    SET_CELL_INDEX(cell_index);

    if (RRC_INVALID_CELL_INDEX == cell_index)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Invalid Cell Index");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    

    /* Search in DB cell with ... - should be present */
    if ((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[cell_index])))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "Cell doesn't exist - ignore.");
    }
    else
    {
        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[cell_index];

        /* Normal message processing */
        cemh_fsm_process_event( CEMH_EVENT_MAC_RECONFIG_CELL_CNF,
                p_api,
                &mac_reconfig_cell_cnf,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}



/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_mac_sfn_cnf_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process RRC_MAC_SFN_CNF message
 *                  and call FSM.
 ******************************************************************************/
void rrc_cemh_mac_sfn_cnf_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_mac_sfn_cnf_t mac_sfn_cnf;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_mac_sfn_cnf( &mac_sfn_cnf,
                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api,
             &cell_index);
    
    SET_CELL_INDEX(cell_index);

    if (RRC_INVALID_CELL_INDEX == cell_index)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Invalid Cell Index");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    
    /* Search in DB cell with ... - should be present */
    if ((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[cell_index])))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "Cell doesn't exist - ignore.");
    }
    else
    {
        p_cell_ctx = p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index];

        /* Normal message processing */
        cemh_fsm_process_event( CEMH_EVENT_MAC_SFN_CNF,
                p_api,
                &mac_sfn_cnf,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_mac_sfn_ind_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function process RRC_MAC_SFN_IND message.
 ******************************************************************************/
void rrc_cemh_mac_sfn_ind_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_mac_sfn_ind_t mac_sfn_ind;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_mac_sfn_ind( &mac_sfn_ind,
                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api,
             &cell_index);
    
    SET_CELL_INDEX(cell_index);

    if (RRC_INVALID_CELL_INDEX == cell_index)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Invalid Cell Index");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    
    /* Search in DB cell with ... - should be present */
    if ((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[cell_index])))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "Cell doesn't exist - ignore.");
    }
    else
    {
        rrc_csc_llim_sfn_ind_t sfn_ind;

        p_cell_ctx = p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index];

        sfn_ind.transaction_id = rrc_get_transaction_id(p_api);
        sfn_ind.cell_index = p_cell_ctx->cell_index;
        /* Cell Reconfiguration Phase 2 : sf has been removed 
           and sfn stored properly */
        sfn_ind.sfn = mac_sfn_ind.sfn;
        rrc_llim_send_internal_msg( RRC_CSC_MODULE_ID, CSC_LLIM_SFN_IND,
                sizeof(sfn_ind), &sfn_ind);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_oamh_cleanup_req_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function, for each cell, partially process cleanup request,
 *                  call FSM and cleanup Cell DB.
 ******************************************************************************/
void rrc_cemh_oamh_cleanup_req_process_msg
(
 void                *p_api,                 /* PNULL */
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;
    U8                  cell_index = RRC_NULL;

    RRC_LLIM_UT_TRACE_ENTER();

    /*coverity fix start*/
    /*coverity fix stop*/
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /*coverity fix start*/
    /*coverity fix stop*/

    /* Clean Cell Context for all existing cell */
    if (p_rrc_llim_gl_ctx->p_p_cell_ctx != PNULL)
    {
        for (cell_index=0; cell_index<MAX_NUM_CELLS; cell_index++)
        {
            if (PNULL != (p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
            {
                p_cell_ctx = p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index];

                /* Process this special message */
                cemh_fsm_process_event( CEMH_EVENT_OAMH_CLEANUP_REQ,/*coverity fix start*/
                        p_api,
                        p_api,/*coverity fix stop*/
                        p_cell_ctx,
                        p_rrc_llim_gl_ctx);
            }
        }

        rrc_mem_free(p_rrc_llim_gl_ctx->p_p_cell_ctx);
        p_rrc_llim_gl_ctx->p_p_cell_ctx = PNULL;
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/
    }
    RRC_LLIM_UT_TRACE_EXIT();

}
/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_process_timer_msg
 *   INPUT        : rrc_timer_t             timer_id
 *                  rrc_llim_timer_buf_t    *p_rrc_llim_timer_buf
 *                  rrc_llim_gl_ctx_t       *p_rrc_llim_gl_ctx
 *   OUTOUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process CEMH timer message
 *                  and call FSM.
 ******************************************************************************/
void rrc_cemh_process_timer_msg
(
 /* Identity of the expired timer */
 rrc_timer_t             timer_id,
 /* Pointer to buffer associated with timer msg */
 rrc_llim_timer_buf_t    *p_rrc_llim_timer_buf,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t       *p_rrc_llim_gl_ctx
 )
{
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;
    rrc_cemh_timer_buf_t *p_cemh_timer_buf = (rrc_cemh_timer_buf_t
            *)p_rrc_llim_timer_buf;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_rrc_llim_timer_buf);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    RRC_ASSERT(sizeof(rrc_cemh_timer_buf_t) == sizeof(rrc_llim_timer_buf_t) +
            sizeof(rrc_cell_index_t) + sizeof(cemh_fsm_event_et));
    RRC_ASSERT(LLIM_SUBMODULE_CEMH == p_cemh_timer_buf->target_submodule);
/*SPR 20636 Changes Start*/
    RRC_LLIM_TRACE(RRC_INFO, "rrc_cemh_process_timer_msg:  timer id:"
            "%u expired event id: %u,submodule id: %u ",
            (U32_64_Bit)timer_id,p_cemh_timer_buf->cemh_event,
            p_cemh_timer_buf->target_submodule);
/*SPR 20636 Changes End*/

    /* We have only cell specific timer event */
    /* Search in DB cell with p_cemh_timer_buf->cell_index -
     *  should be present */
    if ((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[p_cemh_timer_buf->cell_index])) ||
            ((p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cemh_timer_buf->cell_index])->
             cell_index != p_cemh_timer_buf->
             cell_index) )
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_ERROR, "Cell context not found - ignore.");
    }
    else
    {
        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[p_cemh_timer_buf->cell_index];

        /* Normal message processing */
        cemh_fsm_process_event( p_cemh_timer_buf->cemh_event,
                timer_id,
                p_cemh_timer_buf,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_csc_llim_sfn_req_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process CSC_LLIM_SFN_REQ message
 *                  and call FSM.
 ******************************************************************************/
void rrc_cemh_csc_llim_sfn_req_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_csc_llim_sfn_req_t *p_sfn_req = PNULL;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_csc_llim_sfn_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING,
                "[CSC_LLIM_SFN_REQ] Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_sfn_req = (rrc_csc_llim_sfn_req_t *)((U8* )p_api +
            RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_sfn_req->cell_index);

    /* Search in DB cell with p_sfn_req->cell_index -
     *  should be present */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[p_sfn_req->cell_index])) ||
            ((p_rrc_llim_gl_ctx->p_p_cell_ctx[p_sfn_req->cell_index])->
             cell_index != p_sfn_req->
             cell_index) )
    {
        RRC_LLIM_TRACE(RRC_WARNING,
                "[CSC_LLIM_SFN_REQ] Wrong cell id [%u] - ignore.",
                p_sfn_req->cell_index);
    }
    else
    {
        p_cell_ctx = p_rrc_llim_gl_ctx->p_p_cell_ctx[p_sfn_req->cell_index];

        /* Normal message processing */
        cemh_fsm_process_event( CEMH_EVENT_CSC_LLIM_SFN_SYNC_REQ,
                p_api,
                p_sfn_req,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_csc_llim_reconfig_cell_req_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process CSC_LLIM_RECONFIG_CELL_REQ message
 *                  and call FSM.
 *
 ******************************************************************************/
void rrc_cemh_csc_llim_reconfig_cell_req_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_csc_llim_reconfig_cell_req_t *p_reconfig_cell_req = PNULL;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_csc_llim_reconfig_cell_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_reconfig_cell_req = (rrc_csc_llim_reconfig_cell_req_t *)((U8* )p_api +
            RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_reconfig_cell_req->cell_index);

    if ((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (p_rrc_llim_gl_ctx->p_p_cell_ctx[p_reconfig_cell_req->cell_index] == PNULL)
            || (p_reconfig_cell_req->cell_index!= (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[p_reconfig_cell_req->cell_index]->cell_index)))
    {
        /* LLIM CELL Doest not exist */
        if (RRC_SUCCESS != rrc_llim_send_csc_llim_reconfig_cell_resp_failure
                (RRC_LLIM_INVALID_CELL_INDEX,
                 p_reconfig_cell_req->transaction_id,
                 p_reconfig_cell_req->cell_index))
        {
            RRC_LLIM_TRACE(RRC_WARNING, "UNABLE TO SEND RESPONSE TO CSC");
            RRC_LLIM_UT_TRACE_EXIT();
            return;
        }

    }
    else
    {
        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[p_reconfig_cell_req->cell_index];
        /* Normal message processing */
        cemh_fsm_process_event( CEMH_EVENT_CSC_RECONFIG_CELL_REQ,
                p_api,
                p_reconfig_cell_req,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_csc_llim_write_replace_warning_req_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process CSC_LLIM_WRITE_REPLACE_WARNING_REQ
 *                  message and call FSM.
 ******************************************************************************/
void rrc_cemh_csc_llim_write_replace_warning_req_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_csc_llim_pws_req_t *p_wrw_req = PNULL;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_csc_llim_pws_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING,
                "[CSC_LLIM_WRITE_REPLACE_WARNING_REQ] "\
                "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_wrw_req = (rrc_csc_llim_pws_req_t *)((U8* )p_api +
            RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_wrw_req->cell_index);

    /* Search in DB cell with p_sfn_req->cell_index -
     *  should be present */
    if ((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[p_wrw_req->cell_index])) ||
            ((p_rrc_llim_gl_ctx->p_p_cell_ctx[p_wrw_req->cell_index])->
             cell_index != p_wrw_req->cell_index) )
    {
        RRC_LLIM_TRACE(RRC_WARNING,
                "[CSC_LLIM_WRITE_REPLACE_WARNING_REQ]"\
                " Wrong cell id [%u] - ignore.",
                p_wrw_req->cell_index);
    }
    else
    {
        p_cell_ctx = p_rrc_llim_gl_ctx->p_p_cell_ctx[p_wrw_req->cell_index];

        /* Normal message processing */
        cemh_fsm_process_event( CEMH_EVENT_CSC_WRITE_REPLACE_WARNING_REQ,
                p_api,
                p_wrw_req,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_csc_llim_si_without_etws_req_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process CSC_LLIM_SCHEDULING_INFO_WITHOUT_ETWS_REQ
 *                  message and call FSM.
 ******************************************************************************/
void rrc_cemh_csc_llim_si_without_etws_req_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_csc_llim_updated_pws_si_req_t 
        *p_updated_pws_si_req = PNULL;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_csc_llim_updated_pws_si_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING,
                "[CSC_LLIM_UPDATED_PWS_SI_REQ] "\
                "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_updated_pws_si_req = 
        (rrc_csc_llim_updated_pws_si_req_t*)
        ((U8* )p_api +
         RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_updated_pws_si_req->cell_index);

    /* Search in DB cell with p_sfn_req->cell_index -
     *  should be present */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) ||(PNULL == (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[p_updated_pws_si_req->cell_index])) ||
            ((p_rrc_llim_gl_ctx->
              p_p_cell_ctx[p_updated_pws_si_req->cell_index])->cell_index != 
             p_updated_pws_si_req->cell_index) )
    {
        RRC_LLIM_TRACE(RRC_WARNING,
                "[CSC_LLIM_UPDATED_PWS_SI_REQ]"\
                " Wrong cell id [%u] - ignore.",
                p_updated_pws_si_req->cell_index);
    }
    else
    {
        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[p_updated_pws_si_req->cell_index];

        /* Normal message processing */
        cemh_fsm_process_event( CEMH_EVENT_CSC_SI_WITHOUT_ETWS_REQ,
                p_api,
                p_updated_pws_si_req,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}





/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_phy_reconfig_cell_cnf_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process RRC_PHY_CONFIG_CELL_CNF message
 *                  and call FSM.
 ******************************************************************************/
void rrc_cemh_phy_reconfig_cell_cnf_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_phy_reconfig_cell_cnf_t phy_reconfig_cell_cnf;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;
    S32 length_read = RRC_NULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_phy_reconfig_cell_cnf(
                &phy_reconfig_cell_cnf,
                (U8* )p_api +
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) -
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_CELL_INDEX(phy_reconfig_cell_cnf.cell_index);

    /* Search in DB cell with phy_config_cell_cnf.cell_index -
     *  should be present */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[phy_reconfig_cell_cnf.cell_index]))
            || ((p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[phy_reconfig_cell_cnf.cell_index])->cell_index 
                != phy_reconfig_cell_cnf.cell_index) )
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "Cell context not found - ignore.");
    }
    else
    {
        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[phy_reconfig_cell_cnf.cell_index];

        /* Normal message processing */
        cemh_fsm_process_event( CEMH_EVENT_PHY_RECONFIG_CELL_CNF,
                p_api,
                &phy_reconfig_cell_cnf,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_phy_delete_cell_cnf_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process RRC_PHY_DELETE_CELL_CNF message
 *                  and call FSM.
 ******************************************************************************/
void rrc_cemh_phy_delete_cell_cnf_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_phy_delete_cell_cnf_t phy_delete_cell_cnf;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;
    S32 length_read = RRC_NULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS !=  rrc_il_parse_rrc_phy_delete_cell_cnf(
                &phy_delete_cell_cnf,
                (U8* )p_api +
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) -
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    /* Search in DB cell with phy_delete_cell_cnf.cell_index -
     *  should be present */
    if ((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[phy_delete_cell_cnf.cell_index])) ||
            ((p_rrc_llim_gl_ctx->
              p_p_cell_ctx[phy_delete_cell_cnf.cell_index])->cell_index 
             != phy_delete_cell_cnf.cell_index) )
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "Cell context not found - ignore.");
    }
    else
    {
        SET_CELL_INDEX(phy_delete_cell_cnf.cell_index);

        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[phy_delete_cell_cnf.cell_index];

        /* Normal message processing */
        cemh_fsm_process_event( CEMH_EVENT_PHY_DELETE_CELL_CNF,
                p_api,
                &phy_delete_cell_cnf,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_mac_delete_cell_cnf_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process RRC_MAC_DELETE_CELL_CNF message
 *                  and call FSM.
 ******************************************************************************/
void rrc_cemh_mac_delete_cell_cnf_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    
    /* spr 10203 fix start */
     
    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api,
            &cell_index);

    SET_CELL_INDEX(cell_index);

    if (RRC_INVALID_CELL_INDEX == cell_index)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Invalid Cell Index");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    

    /* Search in DB cell with mac_delete_cell_cnf.cell_index -
     *  should be present */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[cell_index])) )
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "Cell context not found - ignore.");
    }
    else
    {
        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[cell_index];

        /* Normal message processing */
        cemh_fsm_process_event( CEMH_EVENT_MAC_DELETE_CELL_CNF,
                p_api,/*coverity fix start*/
                p_api,/*coverity fix stop*/
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
        
    }
    
    /* spr 10203 fix stop */
     

    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_phy_cell_start_cnf_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process RRC_PHY_CELL_START_CNF message
 *                  and call FSM.
 ******************************************************************************/
void rrc_cemh_phy_cell_start_cnf_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_phy_cell_start_cnf_t phy_cell_start_cnf;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;
    S32 length_read = RRC_NULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_phy_cell_start_cnf(
                &phy_cell_start_cnf,
                (U8* )p_api +
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) -
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_CELL_INDEX(phy_cell_start_cnf.cell_index);

    /* Search in DB cell with phy_cell_start_cnf.cell_index -
     *  should be present */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[phy_cell_start_cnf.cell_index])) ||
            ((p_rrc_llim_gl_ctx->
              p_p_cell_ctx[phy_cell_start_cnf.cell_index])->
             cell_index != phy_cell_start_cnf.cell_index) )
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "Cell context not found - ignore.");
    }
    else
    {
        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[phy_cell_start_cnf.cell_index];

        /* Normal message processing */
        cemh_fsm_process_event( CEMH_EVENT_PHY_CELL_START_CNF,
                p_api,
                &phy_cell_start_cnf,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();

}    
/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_phy_cell_stop_cnf_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process RRC_PHY_CELL_STOP_CNF message
 *                  and call FSM.
 ******************************************************************************/
void rrc_cemh_phy_cell_stop_cnf_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_phy_cell_stop_cnf_t phy_cell_stop_cnf;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;
    S32 length_read = RRC_NULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_phy_cell_stop_cnf(
                &phy_cell_stop_cnf,
                (U8* )p_api +
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) -
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_CELL_INDEX(phy_cell_stop_cnf.cell_index);

    /* Search in DB cell with phy_cell_stop_cnf.cell_index -
     *  should be present */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == p_rrc_llim_gl_ctx->
                p_p_cell_ctx[phy_cell_stop_cnf.cell_index]) ||
            (p_rrc_llim_gl_ctx->p_p_cell_ctx[phy_cell_stop_cnf.cell_index]->
             cell_index != phy_cell_stop_cnf.cell_index) )
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "Cell context not found - ignore.");
    }
    else
    {
        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[phy_cell_stop_cnf.cell_index];

        /* Normal message processing */
        cemh_fsm_process_event( CEMH_EVENT_PHY_CELL_STOP_CNF,
                p_api,
                &phy_cell_stop_cnf,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}
/********************************************************************************
 *   FUNCTION NAME: rrc_cemh_csc_llim_cell_start_req_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function process CSC_LLIM_CELL_START_REQ 
 *                  message and call FSM.
 *******************************************************************************/
void rrc_cemh_csc_llim_cell_start_req_process_msg(
        /* Pointer to input API buffer */
        void                *p_api,
        /* Pointer to the LLIM global context data */
        rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
        )
{
    rrc_csc_llim_cell_start_req_t *p_cell_start_req = PNULL;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;
    /*cdma_fix bug 10186 start */
    rrc_timestamp_t ts = {0};
    /*cdma_fix bug 10186 stop */


    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_csc_llim_cell_start_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_cell_start_req = (rrc_csc_llim_cell_start_req_t *)((U8* )p_api +
            RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_cell_start_req->cell_index);

    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_start_req->cell_index]) || 
            (p_cell_start_req->cell_index!= p_rrc_llim_gl_ctx->
             p_p_cell_ctx[p_cell_start_req->cell_index]->cell_index))
    {
        /* LLIM CELL Doest not exist */
        if (RRC_SUCCESS != rrc_llim_send_csc_llim_cell_start_resp(
                    p_cell_start_req->transaction_id,
                    p_cell_start_req->cell_index,
                    RRC_LLIM_INVALID_CELL_INDEX,
                    RRC_NULL,
                    /*cdma_fix bug 10186 start*/
                    RRC_NULL,
                    ts
                    /*cdma_fix bug 10186 stop*/
                    ))
        {
            RRC_LLIM_TRACE(RRC_WARNING, "UNABLE TO SEND RESPONSE TO CSC");
            RRC_LLIM_UT_TRACE_EXIT();
            return;
        }

    }
    else
    {
        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[p_cell_start_req->cell_index];
        /* Normal message processing */
        cemh_fsm_process_event (CEMH_EVENT_PHY_CELL_START_REQ,
                p_api,
                p_cell_start_req,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_csc_llim_cell_stop_req_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function process CSC_LLIM_CELL_STOP_REQ
 *                  message and call FSM.
 *****************************************************************************/
void rrc_cemh_csc_llim_cell_stop_req_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_csc_llim_cell_stop_req_t *p_cell_stop_req;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_csc_llim_cell_stop_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_cell_stop_req = (rrc_csc_llim_cell_stop_req_t *)((U8* )p_api +
            RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_cell_stop_req->cell_index);

    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[p_cell_stop_req->cell_index]) || 
            ( p_cell_stop_req->cell_index!= p_rrc_llim_gl_ctx->
              p_p_cell_ctx[p_cell_stop_req->cell_index]->cell_index))
    {
        /* LLIM CELL Doest not exist */
        if (RRC_SUCCESS != rrc_llim_send_csc_llim_cell_stop_resp(
                    p_cell_stop_req->transaction_id,
                    p_cell_stop_req->cell_index,
                    RRC_LLIM_INVALID_CELL_INDEX))
        {
            RRC_LLIM_TRACE(RRC_WARNING, "UNABLE TO SEND RESPONSE TO CSC");
            RRC_LLIM_UT_TRACE_EXIT();
            return;
        }

    }
    else
    {
        /* Bug 4450 Start */
        /* Cleanup UEs before stoping the cells*/
        /* Now UE Context will be cleaned per uecc basis */
        rrc_uemh_cleanup_ues(p_api, p_rrc_llim_gl_ctx); 
        /* Bug 4450 Stop */

        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[p_cell_stop_req->cell_index];
        /* Normal message processing */
        cemh_fsm_process_event (CEMH_EVENT_PHY_CELL_STOP_REQ,
                p_api,
                p_cell_stop_req,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();

}    
#ifdef LTE_EMBMS_SUPPORTED
/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_rlc_create_mbms_area_cnf_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process RRC_RLC_CREATE_AREA_CNF message
 *                  and call FSM.
 ******************************************************************************/
void rrc_cemh_rlc_create_mbms_area_cnf_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_rlc_create_mbms_area_entity_cnf_t   rlc_create_mbms_area_entity_cnf;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_rlc_create_mbms_area_entity_cnf(
                &rlc_create_mbms_area_entity_cnf,
                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) -
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api,
             &cell_index);
    
    if (RRC_INVALID_CELL_INDEX == cell_index)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Invalid Cell Index");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    /* Search in DB cell with ... - should be present */
    if ((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[cell_index])))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "Cell doesn't exist - ignore.");
    }
    else
    {
        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[cell_index];

        /* Normal message processing */
        cemh_fsm_process_event(CEMH_EVENT_RLC_CREATE_AREA_ENTITY_CNF,
                p_api,
                &rlc_create_mbms_area_entity_cnf,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: rrc_cemh_rlc_delete_mbms_area_cnf_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function partially process RRC_RLC_DELETE_AREA_CNF message
 *                  and call FSM.
 ******************************************************************************/
void rrc_cemh_rlc_delete_mbms_area_cnf_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_rlc_delete_mbms_area_entity_cnf_t   rlc_delete_mbms_area_entity_cnf;
    rrc_llim_cell_ctx_t *p_cell_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_rlc_delete_mbms_area_entity_cnf(
                &rlc_delete_mbms_area_entity_cnf,
                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                rrc_get_api_buf_size(p_api) -
                RRC_FULL_INTERFACE_HEADERS_SIZE,
                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api,
            &cell_index);

    if (RRC_INVALID_CELL_INDEX == cell_index)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Invalid Cell Index");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    /* Search in DB cell with ... - should be present */
    if ((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == (p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[cell_index])))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "Cell doesn't exist - ignore.");
    }
    else
    {
        p_cell_ctx = p_rrc_llim_gl_ctx->
            p_p_cell_ctx[cell_index];

        /* Normal message processing */
        cemh_fsm_process_event( CEMH_EVENT_RLC_DELETE_AREA_ENTITY_CNF,
                p_api,
                &rlc_delete_mbms_area_entity_cnf,
                p_cell_ctx,
                p_rrc_llim_gl_ctx);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}
#endif
