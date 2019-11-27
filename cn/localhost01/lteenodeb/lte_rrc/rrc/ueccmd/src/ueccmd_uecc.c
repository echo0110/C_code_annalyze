/******************************************************************************
 *
 *   FILE NAME:
 *       ueccmd_uecc.c
 *
 *   DESCRIPTION:
 *       This is the source file of the UECC MD UECC .
 *       UECC MD UECC interracts with RRC UECC module.
 *
 *   Copyright (c) 2012, Aricent Inc. All Rights Reserved
 *
 ******************************************************************************/
#include "rrc_defines.h"
#include "ueccmd_oamh.h"
#include "rrc_intrl_api.h"
#include "rrc_oamh_intf.h"
#include "rrc_msg_mgmt.h"
#include "ueccmd_utils.h"
#include "ueccmd_init.h"
#include "ueccmd_logger.h"
#include "rrc_common_utils.h"
#include "rrc_handlers.h"
#include "ueccmd_uecc.h"
#include "rrc_uecc_llim_intf.h"
#include "rrc_csc_uecc_intf.h"
#include "rrc_s1ap_uecc_intf.h"
#include "rrc_x2ap_uecc_intf.h"

/******************************************************************************
 *   FUNCTION NAME: ueccmd_uecc_oamh_get_cell_status_resp_process_msg
 *   INPUT        : void            *p_api
 *                  ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
 *   OUTPUT       : none
 *   RETURNS      : none
 *   DESCRIPTION:    This function processes UECC_OAMH_GET_CELL_STATUS_RESP message
 ******************************************************************************/
void ueccmd_uecc_oamh_get_cell_status_resp_process_msg
(
 void            *p_api,          /* Pointer to input API buffer */
 ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECC MD global context */
 )
{
    rrc_uecc_oamh_get_cell_status_resp_t   *p_cell_status_resp = PNULL;
    void                                   *p_intrl_msg  = PNULL;
    U8                                     uecc_index = RRC_NULL;
    rrc_return_et                          result = RRC_SUCCESS;
    rrc_module_id_t                        src_module_id = RRC_NULL;
    rrc_uecc_oamh_get_cell_status_resp_t   *p_cell_status_resp_for_oamh = PNULL;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_uecc_oamh_get_cell_status_resp_t))
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }

    /* Get message payload */
    p_cell_status_resp = (rrc_uecc_oamh_get_cell_status_resp_t *)((U8* )p_api + RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_cell_status_resp->cell_index);

    /* If Received Message not consistent with running Procedure */
    if ((p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_cell_status_resp->
            cell_index].bitmask & UECCMD_GET_CELL_STATUS_PROC_RUNNING) != 
            UECCMD_GET_CELL_STATUS_PROC_RUNNING)
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Ignore Cell Status Response message As "
                "GET_CELL_STATUS_RESP Proc is not running");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }
    else
    {
        p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_cell_status_resp->cell_index].
            uecc_resp_per_cell[UECCMD_GET_CELL_STATUS].num_resp_recv++;
        /*If First Cell status response received */
        if(p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_cell_status_resp->
                cell_index].uecc_resp_per_cell[UECCMD_GET_CELL_STATUS].
                num_resp_recv == 1)
        {
            l3_memcpy_wrapper(&(p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_cell_status_resp->cell_index].
                        uecc_resp_per_cell[UECCMD_GET_CELL_STATUS].
                        cell_status_resp), p_cell_status_resp,
                    sizeof(rrc_uecc_oamh_get_cell_status_resp_t));
        }
        /* Now add the status received from all UECC isntances */
        else
        {
            /*If Status received from any of the UECC instances then it will 
             * be send to OAMH */
            p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_cell_status_resp->cell_index].
                uecc_resp_per_cell[UECCMD_GET_CELL_STATUS].
                cell_status_resp.bitmask |= p_cell_status_resp->bitmask;

            p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_cell_status_resp->cell_index].
                uecc_resp_per_cell[UECCMD_GET_CELL_STATUS].
                cell_status_resp.cell_status.num_ue += 
                p_cell_status_resp->cell_status.num_ue;
            p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_cell_status_resp->cell_index].
                uecc_resp_per_cell[UECCMD_GET_CELL_STATUS].
                cell_status_resp.cell_status.num_srb1 += 
                p_cell_status_resp->cell_status.num_srb1;
            p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_cell_status_resp->cell_index].
                uecc_resp_per_cell[UECCMD_GET_CELL_STATUS].
                cell_status_resp.cell_status.num_srb2 += 
                p_cell_status_resp->cell_status.num_srb2;
            p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_cell_status_resp->cell_index].
                uecc_resp_per_cell[UECCMD_GET_CELL_STATUS].
                cell_status_resp.cell_status.num_drb += 
                p_cell_status_resp->cell_status.num_drb;
        }

        src_module_id = rrc_get_src_module_id(p_api);

        if ((src_module_id <= RRC_UECC_8_MODULE_ID) &&
                (src_module_id >= RRC_UECC_1_MODULE_ID))
        {
            /* Store Response received at proper index*/ 
            p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_cell_status_resp->cell_index].
                uecc_resp_per_cell[UECCMD_GET_CELL_STATUS].
                uecc_resp[src_module_id - RRC_UECC_1_MODULE_ID] = p_cell_status_resp->response;
        }

        /* If all responses received from UECC */
        if (p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_cell_status_resp->cell_index].
                uecc_resp_per_cell[UECCMD_GET_CELL_STATUS].
                num_resp_recv == p_ueccmd_gl_ctx->num_uecc_instances)
        {

            /* Prepare GET CELL STATUS RESP message */
            p_intrl_msg = (void *)ueccmd_alloc_intrl_msg (RRC_UECC_MODULE_ID,
                    RRC_OAMH_MODULE_ID,
                    UECC_OAMH_GET_CELL_STATUS_RESP,
                    sizeof(rrc_uecc_oamh_get_cell_status_resp_t));

            if (PNULL != p_intrl_msg)
            {
                p_cell_status_resp_for_oamh = (rrc_uecc_oamh_get_cell_status_resp_t *)((U8 *)
                        p_intrl_msg + RRC_API_HEADER_SIZE);
                /*If Failure received from any UECC instance then send Failure
                 * to OAMH */
                for ( uecc_index=0; uecc_index<p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_cell_status_resp->
                        cell_index].uecc_resp_per_cell[UECCMD_GET_CELL_STATUS].
                        num_resp_recv; uecc_index++)
                {
                    if ( p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_cell_status_resp->
                            cell_index].uecc_resp_per_cell[UECCMD_GET_CELL_STATUS].
                            uecc_resp[uecc_index] == RRC_FAILURE)
                    {
                        result = RRC_FAILURE;
                        break;
                    }
                }
                l3_memcpy_wrapper(p_cell_status_resp_for_oamh,
                        &(p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_cell_status_resp->cell_index].
                            uecc_resp_per_cell[UECCMD_GET_CELL_STATUS].
                            cell_status_resp), sizeof(rrc_uecc_oamh_get_cell_status_resp_t));

                p_cell_status_resp_for_oamh->response = result;

                /* Send UECC_OAMH_GET_CELL_STATUS_RESP message  */
                rrc_send_message(p_intrl_msg, RRC_OAMH_MODULE_ID);
            }

            /*Reset num_resp_recv to ZERO*/
            p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_cell_status_resp->cell_index].
                uecc_resp_per_cell[UECCMD_GET_CELL_STATUS].
                num_resp_recv = RRC_NULL;

            memset_wrapper(p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_cell_status_resp->
                    cell_index].uecc_resp_per_cell[UECCMD_GET_CELL_STATUS].
                    uecc_resp, RRC_NULL,
                    sizeof(U8)*MAX_UECC_INSTANCES);

            p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_cell_status_resp->
                cell_index].bitmask &= (U8)(~(UECCMD_GET_CELL_STATUS_PROC_RUNNING));
        }

        /*If all the response not received then wait for responses */
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: ueccmd_uecc_oamh_prov_resp_process_msg
 *   INPUT        : void            *p_api
 *                  ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
 *   OUTPUT       : none
 *   RETURNS      : none
 *   DESCRIPTION  : This function processes UECC_OAMH_PROV_RESP message
 ******************************************************************************/
void ueccmd_uecc_oamh_prov_resp_process_msg
(
 void            *p_api,          /* Pointer to input API buffer */
 ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECC MD global context */
 )
{
    rrc_uecc_oamh_prov_resp_t   *p_prov_resp = PNULL;
    void                        *p_intrl_msg  = PNULL;
    U8                          uecc_index = RRC_NULL;
    rrc_return_et               result = RRC_SUCCESS;
    rrc_ueccmd_oamh_prov_resp_t *p_ueccmd_oamh_prov_resp = PNULL;
    rrc_module_id_t             src_module_id = RRC_NULL;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_uecc_oamh_prov_resp_t))
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }

    /* Get message payload */
    p_prov_resp = (rrc_uecc_oamh_prov_resp_t *)((U8* )p_api + RRC_API_HEADER_SIZE);

    /* If Received Message not consistent with running Procedure */
    if (p_ueccmd_gl_ctx->current_proc != UECCMD_PROVISION )
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Ignore Provision Response message As - Current Proc: %u",
                p_ueccmd_gl_ctx->current_proc);
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }
    else
    {
        p_ueccmd_gl_ctx->num_resp_recv++;

        src_module_id = rrc_get_src_module_id (p_api);
        
        if ((src_module_id <= RRC_UECC_8_MODULE_ID) &&
                (src_module_id >= RRC_UECC_1_MODULE_ID))
        {
            /* Store Response received at proper index*/ 
            p_ueccmd_gl_ctx->uecc_resp[src_module_id - RRC_UECC_1_MODULE_ID] = p_prov_resp->result;
        }

        /* If all Provision response received from UECC */
        if (p_ueccmd_gl_ctx->num_resp_recv == p_ueccmd_gl_ctx->num_uecc_instances)
        {

            /* Prepare UECCMD_OAMH_PROV_RESP message */
            p_intrl_msg = (void *)ueccmd_alloc_intrl_msg (RRC_UECCMD_MODULE_ID,
                    RRC_OAMH_MODULE_ID,
                    UECCMD_OAMH_PROV_RESP,
                    sizeof(rrc_ueccmd_oamh_prov_resp_t));

            if (PNULL != p_intrl_msg)
            {
                p_ueccmd_oamh_prov_resp = (rrc_ueccmd_oamh_prov_resp_t *)((U8 *)
                        p_intrl_msg + RRC_API_HEADER_SIZE);

                /*If Failure received from any UECC instance then send Failure
                 * to OAMH */
                for ( uecc_index=0; uecc_index < p_ueccmd_gl_ctx->num_resp_recv &&
                        uecc_index < MAX_UECC_INSTANCES; uecc_index++)
                {
                    if ( p_ueccmd_gl_ctx->uecc_resp[uecc_index] == RRC_FAILURE)
                    {
                        result = RRC_FAILURE;
                        break;
                    }
                }
                p_ueccmd_oamh_prov_resp->result = result;
                /* Send UECC_OAMH_PROV_RESP message  */
                rrc_send_message(p_intrl_msg, RRC_OAMH_MODULE_ID);
            }

            /*Reset num_resp_recv to ZERO*/
            p_ueccmd_gl_ctx->num_resp_recv = RRC_NULL;

            memset_wrapper(p_ueccmd_gl_ctx->uecc_resp, RRC_NULL,
                    sizeof(p_ueccmd_gl_ctx->uecc_resp));

            p_ueccmd_gl_ctx->current_proc = UECCMD_LAST_PROC;
        }

        /*If all the response not received then wait for responses */
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}

/*SPR_17727_START*/
/******************************************************************************
 *   FUNCTION NAME: ueccmd_uecc_ue_con_re_establish_process_msg
 *   INPUT        : void            *p_api
 *                  ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
 *   OUTPUT       : none
 *   RETURNS      : none
 *   DESCRIPTION  : This function processes 
 *                  UECCMD_UE_CON_RE_ESTABLISH_ONGOING message
 ******************************************************************************/
void ueccmd_uecc_ue_con_re_establish_process_msg
(
 void            *p_api,          /* Pointer to input API buffer */
 ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECC MD global context */
 )
{
    rrc_uecc_ueccmd_ue_con_re_establish_ongoing_t *p_re_estb_msg = PNULL;
    ueccmd_ue_conn_reestab_node_t   *p_ue_conn_reestab_node = PNULL;
    U32                             rnti = RRC_NULL;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_uecc_ueccmd_ue_con_re_establish_ongoing_t))
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }

    /* Get message payload */
    p_re_estb_msg = 
        (rrc_uecc_ueccmd_ue_con_re_establish_ongoing_t *)((U8* )p_api
        + RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_re_estb_msg->cell_index);
    rnti = ((((U32)(p_re_estb_msg->cell_index)) << 16) & 0x00FF0000) | p_re_estb_msg->rnti;

    /*Check whether ue connection reestablishment already ongoing for received UE */
    p_ue_conn_reestab_node = (ueccmd_ue_conn_reestab_node_t* )(st_find_tree_node( (const search_tree_t *)
                &(p_ueccmd_gl_ctx->ueccmd_ue_conn_reestab_search_tree),
                (const void*)&rnti));

    if ( p_ue_conn_reestab_node == PNULL )
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Ignore RE-ESTABLISHMENT received from "
                "UECC message as - Re_establishment not running for "
                "the requested cell_index: %u and RNTI: %u .. dropping ", 
                p_re_estb_msg->cell_index, p_re_estb_msg->rnti);
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }
    else
    {
        /* UE connection reestablishment procedure is complete for UECCMD module,
         * delete node from search tree */

        st_delete_node (&(p_ueccmd_gl_ctx->ueccmd_ue_conn_reestab_search_tree),
                &(p_ue_conn_reestab_node->anchor));

        RRC_UECCMD_TRACE(RRC_INFO, "Deleted reestb node for cell_index: %u and rnti: %u", 
                p_re_estb_msg->cell_index, p_re_estb_msg->rnti);

        rrc_mem_free((void *)p_ue_conn_reestab_node);
        p_ue_conn_reestab_node = PNULL;

        RRC_UECCMD_TRACE(RRC_INFO, "Deleted RE-ESTABLISHMENT node "
                "for RNTI: %d from UECCMD", rnti);
    }
    RRC_UECCMD_UT_TRACE_EXIT();
}
/*SPR_17727_END*/

/******************************************************************************
 *   FUNCTION NAME: ueccmd_uecc_llim_ue_con_re_establish_rej_req_process_msg
 *   INPUT        : void            *p_api
 *                  ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
 *   OUTPUT       : none
 *   RETURNS      : none
 *   DESCRIPTION:    This function processes UECC_LLIM_UE_CON_RE_ESTABLISH_REJ_REQ message
 ******************************************************************************/
void ueccmd_uecc_llim_ue_con_re_establish_rej_req_process_msg
(
 void            *p_api,          /* Pointer to input API buffer */
 ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECC MD global context */
 )
{
    rrc_uecc_llim_con_re_establish_rej_req_t   *p_data_req = PNULL;
    void                                       *p_intrl_msg  = PNULL;
    rrc_module_id_t                            src_module_id = RRC_NULL;
    ueccmd_ue_conn_reestab_node_t              *p_ue_conn_reestab_node = PNULL;
/* Reestablishment_Fix start */   
    /*SPR_17727_START*/
    U32                                        rnti = RRC_NULL;
    /*SPR_17727_END*/
/* Reestablishment_Fix stop */   
    x2ap_ue_reestab_conn_setup_reconfig_t                      *p_reestab_conn_setup_reconfig = PNULL;
    void                                       *p_msg = PNULL;
    /* SPR 16729 Fix Start */
    U16                                        msg_size = RRC_NULL;
    /* SPR 16729 Fix Stop */

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    /* Get message payload */
    p_data_req = (rrc_uecc_llim_con_re_establish_rej_req_t *)((U8* )p_api + RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_data_req->cell_index);

/* Reestablishment_Fix start */   
    /*SPR_17727_START*/
    rnti = ((((U32)(p_data_req->cell_index)) << 16) & 0x00FF0000) | p_data_req->crnti;
    /*SPR_17727_END*/
/* Reestablishment_Fix stop */   

    /*Check whether ue connection reestablishment already ongoing for received UE */
    p_ue_conn_reestab_node = (ueccmd_ue_conn_reestab_node_t* )(st_find_tree_node( (const search_tree_t *)
                &(p_ueccmd_gl_ctx->ueccmd_ue_conn_reestab_search_tree),
                (const void*)&rnti));

    if ( p_ue_conn_reestab_node == PNULL )
    {
        /*SPR_17727_START*/
        RRC_UECCMD_TRACE(RRC_WARNING, "Ignore rrc_uecc_llim_con_re_establish_rej_req message as "
                "- Re_establishment not running for the requested "
                "cell_index: %u and RNTI: %d .. dropping ", p_data_req->cell_index, p_data_req->crnti);
        /*SPR_17727_END*/
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }
    else
    {
        p_ue_conn_reestab_node->num_resp_recv++;

        src_module_id = rrc_get_src_module_id (p_api);

        if ((src_module_id <= RRC_UECC_8_MODULE_ID) &&
                (src_module_id >= RRC_UECC_1_MODULE_ID))
        {
            /* Store Response received at proper index*/ 
            p_ue_conn_reestab_node->uecc_resp[src_module_id - RRC_UECC_1_MODULE_ID] =
                RRC_FAILURE;
        }

        /* If all Response received from UECC */
        if (p_ue_conn_reestab_node->num_resp_recv == p_ueccmd_gl_ctx->num_uecc_instances)
        {
            p_msg = (void *)ueccmd_alloc_intrl_msg (RRC_UECC_MODULE_ID,
                    RRC_X2AP_MODULE_ID,
                    X2AP_UE_REESTABLISHMENT_CONN_SETUP_RECONFIGURE,
                    sizeof(x2ap_ue_reestab_conn_setup_reconfig_t));

            if (PNULL != p_msg)
            {
                p_reestab_conn_setup_reconfig = (x2ap_ue_reestab_conn_setup_reconfig_t*)((U8 *)p_msg + RRC_API_HEADER_SIZE);

                memset_wrapper(p_reestab_conn_setup_reconfig, 0, sizeof(x2ap_ue_reestab_conn_setup_reconfig_t));

                /* Send Reestablishment_ind to X2AP module */
                p_reestab_conn_setup_reconfig->bitmask    = 0;
                p_reestab_conn_setup_reconfig->cell_index = 
                    p_ue_conn_reestab_node->ue_reestab_info.cell_index;
                p_reestab_conn_setup_reconfig->crnti      = 
                    p_ue_conn_reestab_node->ue_reestab_info.crnti;
                p_reestab_conn_setup_reconfig->pci        = 
                    p_ue_conn_reestab_node->ue_reestab_info.pci;
                p_reestab_conn_setup_reconfig->short_mac_i = 
                    p_ue_conn_reestab_node->ue_reestab_info.short_mac_i;

                /* Send X2AP_UE_REESTABLISHMENT_CONN_SETUP_RECONFIGURE message  */
                rrc_send_message(p_msg, RRC_X2AP_MODULE_ID);
            }
            /* UE connection reestablishment procedure is complete,
             * delete node from search tree */
            st_delete_node (&(p_ueccmd_gl_ctx->ueccmd_ue_conn_reestab_search_tree),
                    &(p_ue_conn_reestab_node->anchor));

            /*SPR_17727_START*/
            RRC_UECCMD_TRACE(RRC_INFO, "Deleted reestb node for cell_index: %u and rnti: %u", 
                p_data_req->cell_index, p_data_req->crnti);
            /*SPR_17727_END*/

            rrc_mem_free((void *)p_ue_conn_reestab_node);
            p_ue_conn_reestab_node = PNULL;

            /* Prepare RRC_REESTAB_CON_REJ message */
	    /* Coverity Fix 83413 Start */

            /* SPR 16729 Fix Start */
            msg_size = (sizeof(rrc_uecc_llim_con_re_establish_rej_req_t) +
                       p_data_req->data_buffer_length);

            p_intrl_msg = (void *)ueccmd_alloc_intrl_msg (RRC_UECC_MODULE_ID,
    	       RRC_LLIM_MODULE_ID,
	    	   UECC_LLIM_UE_CON_RE_ESTABLISH_REJ_REQ,
		       msg_size);
            /* SPR 16729 Fix Stop */
            if (PNULL != p_intrl_msg)
            {
                /* SPR 16729 Fix Start */
                /*Forward the message RRC_REESTAB_CON_REJ received from UECC to LLIM */
                 l3_memcpy_wrapper(((U8*)p_intrl_msg + RRC_API_HEADER_SIZE),
                    p_data_req, msg_size);
                /* SPR 16729 Fix Stop */
		/* Coverity Fix 83413 Stop */

                /* Send UECC_LLIM_CCCH_DATA_REQ  */
                rrc_send_message(p_intrl_msg, RRC_LLIM_MODULE_ID);
            }
        }

        /*If all the response not received then wait for responses */
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: ueccmd_oamh_cleanup_resp_process_msg
 *   INPUT        : void            *p_api
 *                  ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
 *   OUTPUT       : none
 *   RETURNS      : none
 *   DESCRIPTION:    This function processes UECCMD_OAMH_CLEANUP_RESP message
 ******************************************************************************/
void ueccmd_oamh_cleanup_resp_process_msg
(
 void            *p_api,          /* Pointer to input API buffer */
 ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECC MD global context */
 )
{
    void                           *p_intrl_msg  = PNULL;
    rrc_return_et                  result = RRC_SUCCESS;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE)
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }

    /* If Received Message not consistent with running Procedure */
    if (p_ueccmd_gl_ctx->current_proc != UECCMD_CLEANUP)
    {
        RRC_UECCMD_TRACE(RRC_WARNING, 
                "Ignore cleanup Response message As - Current Proc: %u",
                p_ueccmd_gl_ctx->current_proc);
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }
    else
    {
        p_ueccmd_gl_ctx->num_resp_recv++;

        /* If all responses received from UECC */
        if (p_ueccmd_gl_ctx->num_resp_recv == 
                p_ueccmd_gl_ctx->num_uecc_instances)
        {
            /*Reset num_resp_recv to ZERO*/
            p_ueccmd_gl_ctx->num_resp_recv = RRC_NULL;

            memset_wrapper(p_ueccmd_gl_ctx->uecc_resp, RRC_NULL,
                    sizeof(p_ueccmd_gl_ctx->uecc_resp));

            p_ueccmd_gl_ctx->current_proc = UECCMD_LAST_PROC;
            /* Destry the threads created for UECC entity */
            result = rrc_destroy_uecc_instances (p_ueccmd_gl_ctx->num_uecc_instances);

            if (RRC_FAILURE == result)
            {
                RRC_UECCMD_TRACE(RRC_WARNING, "Unable to destoy Threads.");
                RRC_UECCMD_UT_TRACE_EXIT();
                return;
            }
            
            /* Prepare CLEANUP RESP message */
            p_intrl_msg = (void *)ueccmd_alloc_intrl_msg (RRC_UECCMD_MODULE_ID,
                    RRC_OAMH_MODULE_ID,
                    UECCMD_OAMH_CLEANUP_RESP,
                    RRC_NULL);

            if (PNULL != p_intrl_msg)
            {
                /* Send UECC_OAMH_CLEANUP_RESP message  */
                rrc_send_message(p_intrl_msg, RRC_OAMH_MODULE_ID);
            }

            rrc_ueccmd_init_gl_ctx (p_ueccmd_gl_ctx);
        }

        /*If all the response not received then wait for responses */
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME:ueccmd_uecc_csc_delete_all_ue_resp_process_msg
 *   INPUT        : void            *p_api
 *                  ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
 *   OUTPUT       : none
 *   RETURNS      : none
 *   DESCRIPTION:    This function processes UECC_CSC_DELETE_ALL_UE_RESP message
 ******************************************************************************/
void ueccmd_uecc_csc_delete_all_ue_resp_process_msg
(
 void            *p_api,          /* Pointer to input API buffer */
 ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECC MD global context */
 )
{
    rrc_csc_uecc_delete_all_ue_resp_t   *p_delete_all_ue_resp = PNULL;
    void                                *p_intrl_msg  = PNULL;
    U8                                   uecc_index = RRC_NULL;
    rrc_return_et                        result = RRC_SUCCESS;
    rrc_module_id_t                      src_module_id = RRC_NULL;
    rrc_csc_uecc_delete_all_ue_resp_t   *p_delete_all_ue_resp_for_csc = PNULL;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_csc_uecc_delete_all_ue_resp_t))
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }

    /* Get message payload */
    p_delete_all_ue_resp = (rrc_csc_uecc_delete_all_ue_resp_t *)((U8* )p_api + RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_delete_all_ue_resp->cell_index);

    /* If Received Message not consistent with running Procedure */
    if ((p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_delete_all_ue_resp->cell_index].
            bitmask & UECCMD_DELETE_ALL_UE_PROC_RUNNING) != UECCMD_DELETE_ALL_UE_PROC_RUNNING )
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Ignore Delete all UE Response message As "
          "DELETE_ALL_UE Proc is not running");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }
    else
    {
        p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_delete_all_ue_resp->cell_index].
            uecc_resp_per_cell[UECCMD_DELETE_ALL_UE].num_resp_recv++;

        src_module_id = rrc_get_src_module_id(p_api);

        if ((src_module_id <= RRC_UECC_8_MODULE_ID) &&
                (src_module_id >= RRC_UECC_1_MODULE_ID))
        {
            /* Store Response received at proper index*/ 
            p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_delete_all_ue_resp->cell_index].
                uecc_resp_per_cell[UECCMD_DELETE_ALL_UE].
                uecc_resp[src_module_id - RRC_UECC_1_MODULE_ID] = p_delete_all_ue_resp->result;
        }

        /* If all responses received from UECC */
        if (p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_delete_all_ue_resp->cell_index].
                uecc_resp_per_cell[UECCMD_DELETE_ALL_UE].
                num_resp_recv == p_ueccmd_gl_ctx->num_uecc_instances)
        {

            /* Prepare DELETE ALL UE RESP message */
            p_intrl_msg = (void *)ueccmd_alloc_intrl_msg (RRC_UECC_MODULE_ID,
                    RRC_CSC_MODULE_ID,
                    UECC_CSC_DELETE_ALL_UE_RESP,
                    sizeof(rrc_csc_uecc_delete_all_ue_resp_t));

            if (PNULL != p_intrl_msg)
            {
                p_delete_all_ue_resp_for_csc = (rrc_csc_uecc_delete_all_ue_resp_t *)((U8 *)
                        p_intrl_msg + RRC_API_HEADER_SIZE);
                /*If Failure received from any UECC instance then send Failure
                 * to OAMH */
                for ( uecc_index=0; uecc_index < p_ueccmd_gl_ctx->num_uecc_instances
                        && uecc_index < MAX_UECC_INSTANCES; uecc_index++)
                {
                    if ( p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_delete_all_ue_resp->cell_index].
                             uecc_resp_per_cell[UECCMD_DELETE_ALL_UE].
                             uecc_resp[uecc_index] == RRC_FAILURE)
                    {
                        result = RRC_FAILURE;
                        break;
                    }
                }
                p_delete_all_ue_resp_for_csc->result = result;
                p_delete_all_ue_resp_for_csc->cell_index = p_delete_all_ue_resp->cell_index;


                /* Send UECC_DELETE_ALL_UE_RESP message  */
                rrc_send_message(p_intrl_msg, RRC_CSC_MODULE_ID);
            }
            
            /* BUG 11182 Fix Start*/
            memset_wrapper(&p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_delete_all_ue_resp->cell_index], RRC_NULL, 
                    sizeof(ueccmd_cell_ctx_t));
            /* BUG 11182 Fix Stop*/
        }

        /*If all the response not received then wait for responses */
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME:ueccmd_oamh_cell_traffic_trace_start_error_ind_process_msg
 *   INPUT        : void            *p_api
 *                  ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
 *   OUTPUT       : none
 *   RETURNS      : none
 *   DESCRIPTION:    This function processes UECC_OAMH_CELL_TRAFFIC_TRACE_START_ERROR_IND message
 ******************************************************************************/
void ueccmd_oamh_cell_traffic_trace_start_error_ind_process_msg
(
 void            *p_api,          /* Pointer to input API buffer */
 ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECC MD global context */
 )
{
    void                                   *p_intrl_msg = PNULL;
    rrc_uecc_oamh_cell_traffic_trace_start_error_ind_t  *trace_start_ind = PNULL;


    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_uecc_oamh_cell_traffic_trace_start_error_ind_t))
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }

    /* Get message payload */
    trace_start_ind = (rrc_uecc_oamh_cell_traffic_trace_start_error_ind_t *)((U8* )p_api + 
            RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(trace_start_ind->cell_index);

    /* If Received Message not consistent with running Procedure */
    if ((p_ueccmd_gl_ctx->ueccmd_cell_ctx[trace_start_ind->cell_index].
                bitmask & UECCMD_TRAFFIC_TRACE_START_PROC_RUNNING) !=
            UECCMD_TRAFFIC_TRACE_START_PROC_RUNNING )
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Ignore Traffic Trace Start Error Ind message As - Current Proc: %u",
                p_ueccmd_gl_ctx->current_proc);
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }
    else
    {
        p_ueccmd_gl_ctx->ueccmd_cell_ctx[trace_start_ind->cell_index].
            uecc_resp_per_cell[UECCMD_TRAFFIC_TRACE_START].num_resp_recv++;


        /* If all responses received from UECC */
        if (p_ueccmd_gl_ctx->ueccmd_cell_ctx[trace_start_ind->cell_index].
                uecc_resp_per_cell[UECCMD_TRAFFIC_TRACE_START].
                num_resp_recv == p_ueccmd_gl_ctx->num_uecc_instances)
        {
            /*Reset num_resp_recv to ZERO*/
            p_ueccmd_gl_ctx->ueccmd_cell_ctx[trace_start_ind->cell_index].
                uecc_resp_per_cell[UECCMD_TRAFFIC_TRACE_START].
                num_resp_recv = RRC_NULL;

            memset_wrapper(p_ueccmd_gl_ctx->ueccmd_cell_ctx[trace_start_ind->cell_index].
                   uecc_resp_per_cell[UECCMD_TRAFFIC_TRACE_START].
                   uecc_resp, RRC_NULL, sizeof(U8)*MAX_UECC_INSTANCES);

            p_ueccmd_gl_ctx->ueccmd_cell_ctx[trace_start_ind->cell_index].
                bitmask &= (U8)(~(UECCMD_TRAFFIC_TRACE_START_PROC_RUNNING));

	    /* Coverity Fix 83409 Start */
	    p_intrl_msg = (void *)ueccmd_alloc_intrl_msg(RRC_UECC_MODULE_ID,
		    RRC_OAMH_MODULE_ID,
		    UECC_OAMH_CELL_TRAFFIC_TRACE_START_ERROR_IND,
		    sizeof(rrc_uecc_oamh_cell_traffic_trace_start_error_ind_t));
	    if (PNULL != p_intrl_msg)
	    {
		l3_memcpy_wrapper(((U8*)p_intrl_msg + RRC_API_HEADER_SIZE),
			trace_start_ind,
			sizeof(rrc_uecc_oamh_cell_traffic_trace_start_error_ind_t));
		/* Coverity Fix 83409 Stop */

                /* Send TRAFFIC_TRACE_START_ERROR_IND  message  */
                rrc_send_message(p_intrl_msg, RRC_OAMH_MODULE_ID );
            }
        }

        /*If all the response not received then wait for responses */
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}
/******************************************************************************
 *   FUNCTION NAME: ueccmd_oamh_get_ue_status_resp_process_msg
 *   INPUT        : void            *p_api
 *                  ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
 *   OUTPUT       : none
 *   RETURNS      : none
 *   DESCRIPTION:    This function processes UECC_OAMH_GET_UE_STATUS_RESP message
 ******************************************************************************/
void ueccmd_oamh_get_ue_status_resp_process_msg
(
 void            *p_api,          /* Pointer to input API buffer */
 ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECC MD global context */
 )
{
    rrc_uecc_oamh_get_ue_status_resp_t   *p_ue_status_resp = PNULL;
    void                                 *p_intrl_msg  = PNULL;
    U8                                    uecc_index = RRC_NULL;
    rrc_return_et                         result = RRC_SUCCESS;
    rrc_module_id_t                       src_module_id = RRC_NULL;
    U16                                   index = RRC_NULL;
    rrc_uecc_oamh_get_ue_status_resp_t   *p_ue_status_resp_for_oamh = PNULL;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_uecc_oamh_get_ue_status_resp_t))
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }

    /* Get message payload */
    p_ue_status_resp = (rrc_uecc_oamh_get_ue_status_resp_t *)((U8* )p_api + 
            RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_ue_status_resp->cell_index);

    /* If Received Message not consistent with running Procedure */
    if ((p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
            bitmask & UECCMD_GET_UE_STATUS_PROC_RUNNING) !=
            UECCMD_GET_UE_STATUS_PROC_RUNNING )
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Ignore UE Status Response message As "
                "GET_UE_STATUS Proc is not running");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }
    else
    {
        p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
            uecc_resp_per_cell[UECCMD_GET_UE_STATUS].num_resp_recv++;
        /*If First Cell status response received */
        if(p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                uecc_resp_per_cell[UECCMD_GET_UE_STATUS].num_resp_recv == 1)
        {
            l3_memcpy_wrapper(&(p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                        uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                        ue_status_resp), p_ue_status_resp,
                    sizeof(rrc_uecc_oamh_get_ue_status_resp_t));


            if(p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                    uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                    ue_status_resp.ue_status_list.num_ue_status == 
                    RRC_MAX_NUM_UE_STATUS)
            {
                p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                    uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                    num_resp_recv = RRC_NULL;

                memset_wrapper(p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                        uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                        uecc_resp, RRC_NULL, sizeof(U8)*MAX_UECC_INSTANCES);

                p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                    bitmask &= (U8)(~(UECCMD_GET_UE_STATUS_PROC_RUNNING));
            }


        }
        /* Now add the status received from all UECC isntances */
        else
        {
            /*If Status received from any of the UECC instances then it will 
             * be send to OAMH */
            p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                ue_status_resp.bitmask |= p_ue_status_resp->bitmask;

            index = p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                ue_status_resp.ue_status_list.num_ue_status;

            for(uecc_index = 0;
                    (p_ue_status_resp->ue_status_list.num_ue_status < RRC_MAX_NUM_UE_STATUS) 
                    && (uecc_index <= p_ue_status_resp->ue_status_list.num_ue_status) 
                    && ((p_ue_status_resp->ue_status_list.num_ue_status) + 
                        (p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                         uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                         ue_status_resp.ue_status_list.num_ue_status)<= 
                        RRC_MAX_NUM_UE_STATUS);uecc_index++,index++)
            {    
                l3_memcpy_wrapper(&(p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                            uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                            ue_status_resp.ue_status_list.ue_status[index]), 
                        &(p_ue_status_resp->ue_status_list.ue_status[uecc_index]),
                        sizeof(rrc_oam_ue_status_t));
            }

            p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                ue_status_resp.ue_status_list.num_ue_status =
                (U16)(p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                        uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                        ue_status_resp.ue_status_list.num_ue_status + 
                        p_ue_status_resp->ue_status_list.num_ue_status);

            if(p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                    uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                    ue_status_resp.ue_status_list.num_ue_status > RRC_MAX_NUM_UE_STATUS)
            {
                p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                    uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                    ue_status_resp.ue_status_list.num_ue_status = RRC_MAX_NUM_UE_STATUS;
            }


            if(p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                    uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                    ue_status_resp.ue_status_list.num_ue_status == RRC_MAX_NUM_UE_STATUS)
            {
                p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                    uecc_resp_per_cell[UECCMD_GET_UE_STATUS].num_resp_recv = RRC_NULL;

                memset_wrapper(p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                        uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                        uecc_resp, RRC_NULL,  sizeof(U8)*MAX_UECC_INSTANCES);

                p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                    bitmask &= (U8)(~(UECCMD_GET_UE_STATUS_PROC_RUNNING));

            }

        }

        src_module_id = rrc_get_src_module_id(p_api);

        if ((src_module_id <= RRC_UECC_8_MODULE_ID) &&
                (src_module_id >= RRC_UECC_1_MODULE_ID))
        {
            /* Store Response received at proper index*/ 
            p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                uecc_resp[src_module_id - RRC_UECC_1_MODULE_ID] = 
                p_ue_status_resp->response;
        }

        if((p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->
                    cell_index].bitmask & UECCMD_GET_UE_STATUS_PROC_RUNNING))
        {    
            /* If all responses received from UECC */
            if (p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                    uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                    num_resp_recv == p_ueccmd_gl_ctx->num_uecc_instances)
            {

                p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                    bitmask &= (U8)(~(UECCMD_GET_UE_STATUS_PROC_RUNNING));
            }
        }
        if(!((p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->
                    cell_index].bitmask) & UECCMD_GET_UE_STATUS_PROC_RUNNING))
        {
            /* Prepare GET UE STATUS RESP message */
            p_intrl_msg = (void *)ueccmd_alloc_intrl_msg (RRC_UECC_MODULE_ID,
                    RRC_OAMH_MODULE_ID,
                    UECC_OAMH_GET_UE_STATUS_RESP,
                    sizeof(rrc_uecc_oamh_get_ue_status_resp_t));

            if (PNULL != p_intrl_msg)
            {
                p_ue_status_resp_for_oamh = (rrc_uecc_oamh_get_ue_status_resp_t *)
                    ((U8 *)p_intrl_msg + RRC_API_HEADER_SIZE);
                /*If Failure received from any UECC instance then send Failure
                 * to OAMH */
                for ( uecc_index=0; uecc_index < p_ueccmd_gl_ctx->num_uecc_instances &&
                        uecc_index < MAX_UECC_INSTANCES; uecc_index++)
                {
                    if ( p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                            uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                            uecc_resp[uecc_index] == RRC_FAILURE)
                    {
                        result = RRC_FAILURE;
                        break;
                    }
                }
                l3_memcpy_wrapper(p_ue_status_resp_for_oamh,
                        &(p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                            uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                            ue_status_resp), sizeof(rrc_uecc_oamh_get_ue_status_resp_t));

                p_ue_status_resp_for_oamh->response = result;
                /* Send UECC_OAMH_GET_UE_STATUS_RESP message  */
                rrc_send_message(p_intrl_msg, RRC_OAMH_MODULE_ID);
            }

            /*Reset num_resp_recv to ZERO*/
            p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                num_resp_recv = RRC_NULL;

            memset_wrapper(p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_ue_status_resp->cell_index].
                    uecc_resp_per_cell[UECCMD_GET_UE_STATUS].
                    uecc_resp, RRC_NULL, sizeof(U8)*MAX_UECC_INSTANCES);

        }
        /*If all the response not received then wait for responses */
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: ueccmd_s1ap_ue_associated_sig_msg_req
 *   INPUT        : void            *p_api
 *                  ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
 *   OUTPUT       : none
 *   RETURNS      : none
 *   DESCRIPTION:    This function processes S1AP_UE_ASSOCIATED_SIG_MSG_REQ message
 ******************************************************************************/
void ueccmd_s1ap_ue_associated_sig_msg_req_process_msg
(
 void            *p_api,          /* Pointer to input API buffer */
 ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECC MD global context */
 )
{
    s1ap_ue_associated_sig_msg_req_t   *p_sig_msg_req = PNULL;
    rrc_module_id_t                    src_module_id = RRC_NULL;
    U8                                 result = RRC_SUCCESS;
    U8                                 uecc_index = RRC_NULL;
    void                               *p_intrl_msg = PNULL;
    ueccmd_ue_ctx_release_node_t       *ue_ctx_release_node = PNULL;
    U32                                mme_ue_s1apid = RRC_NULL;


    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    /* Get message payload */
    p_sig_msg_req = (s1ap_ue_associated_sig_msg_req_t *)((U8* )p_api + RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_sig_msg_req->cell_index, p_sig_msg_req->ue_index);

    if ( (p_sig_msg_req->bitmask & UE_ASSOC_SIG_MSG_REQ_MME_UE_S1AP_ID_PRESENT) != 
            UE_ASSOC_SIG_MSG_REQ_MME_UE_S1AP_ID_PRESENT)
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Ignore SIG_MSG_REQ message As -  "
                "mme_ue_s1apId not present .. dropping ") ;
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }
    mme_ue_s1apid = p_sig_msg_req->mme_ue_s1ap_id;

    /*Check whether ue context release already ongoing for received UE */
    ue_ctx_release_node = (ueccmd_ue_ctx_release_node_t* )(st_find_tree_node( (const search_tree_t *)
                &(p_ueccmd_gl_ctx->ueccmd_ue_ctx_search_tree),
                (const void*)&mme_ue_s1apid));


    if ( ue_ctx_release_node == PNULL )
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Ignore SIG_MSG_REQ message as - "
                "ue context release not running for "
                "the requested UE %d .. dropping ", mme_ue_s1apid);
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }
    else
    {
        ue_ctx_release_node->num_resp_recv++;

        src_module_id = rrc_get_src_module_id (p_api);

        if ((src_module_id <= RRC_UECC_8_MODULE_ID) &&
                (src_module_id >= RRC_UECC_1_MODULE_ID))
        {
            /* Store Response received at proper index*/ 
            ue_ctx_release_node->uecc_resp[src_module_id - RRC_UECC_1_MODULE_ID] = RRC_FAILURE;
        }

        /* If all Response received from UECC */
        if (ue_ctx_release_node->num_resp_recv == p_ueccmd_gl_ctx->num_uecc_instances)
        {

            /*If Failure received from any UECC instance then send Failure
             * to OAMH */
            for ( uecc_index=0; uecc_index < p_ueccmd_gl_ctx->num_uecc_instances &&
                    uecc_index < MAX_UECC_INSTANCES; uecc_index++)
            {
                if ( ue_ctx_release_node->uecc_resp[uecc_index] == RRC_SUCCESS)
                {
                    result = RRC_FAILURE;
                    break;
                }
            }
            /*send SIG_MSG_REQ message  */
	    if( result == RRC_SUCCESS )
	    {
		/* Coverage Fix 83392 Start */
		p_intrl_msg = (void *)ueccmd_alloc_intrl_msg(RRC_UECC_MODULE_ID,
			RRC_S1AP_MODULE_ID,
			S1AP_UE_ASSOCIATED_SIG_MSG_REQ,
			sizeof(s1ap_ue_associated_sig_msg_req_t));

                if (PNULL != p_intrl_msg)
                {

		    l3_memcpy_wrapper(((U8*)p_intrl_msg + RRC_API_HEADER_SIZE),
			    p_sig_msg_req, sizeof(s1ap_ue_associated_sig_msg_req_t));	
		    /* Coverage Fix 83392 Start */

		    rrc_send_message(p_intrl_msg, RRC_S1AP_MODULE_ID);
		}
            }
            
            /* UE context release procedure is complete,
             * delete node from search tree */

            st_delete_node (&(p_ueccmd_gl_ctx->ueccmd_ue_ctx_search_tree),
                    &(ue_ctx_release_node->anchor));
            rrc_mem_free((void *)ue_ctx_release_node);
/* Coverity ID 81541  Fix Start*/
           /* ue_ctx_release_node = PNULL; */
/* Coverity ID 81541 Fix End*/
        }


        /*If all the response not received then wait for responses */
    }

    RRC_UECCMD_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: ueccmd_ue_context_release_ind_process_msg
 *   INPUT        : void            *p_api
 *                  ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
 *   OUTPUT       : none
 *   RETURNS      : none
 *   DESCRIPTION:    This function processes UECCMD_UE_CONTEXT_RELEASE_ONGOING_IND message
 ******************************************************************************/
void ueccmd_ue_context_release_ind_process_msg
(
 void            *p_api,          /* Pointer to input API buffer */
 ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECC MD global context */
 )
{
    ueccmd_ue_context_release_ongoing_ind_t   *ue_context_release = PNULL;
    rrc_module_id_t                    src_module_id = RRC_NULL;
    U32                                mme_ue_s1ap_id;
    ueccmd_ue_ctx_release_node_t       *ue_ctx_release_node = PNULL;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + 
            sizeof(ueccmd_ue_context_release_ongoing_ind_t))
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }

    /* Get message payload */
    ue_context_release = (ueccmd_ue_context_release_ongoing_ind_t *)
        ((U8* )p_api + RRC_API_HEADER_SIZE);


    mme_ue_s1ap_id = ue_context_release->mme_ue_s1ap_id;

    /*Check whether ue context release already ongoing for received UE */
    ue_ctx_release_node = (ueccmd_ue_ctx_release_node_t* )(st_find_tree_node( (const search_tree_t *)
                &(p_ueccmd_gl_ctx->ueccmd_ue_ctx_search_tree),
                (const void*)&mme_ue_s1ap_id));


    if ( ue_ctx_release_node == PNULL )
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Ignore UECCMD_UE_CONTEXT_RELEASE_ONGOING_IND message as -  "
                "ue context release not running for the requested UE %d .. dropping ", 
                mme_ue_s1ap_id);
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }
    else
    {
        if(ue_context_release->result == RRC_SUCCESS ||
                ue_ctx_release_node->num_resp_recv ==
                p_ueccmd_gl_ctx->num_uecc_instances)
        {
            /* If UE Context Released successfully on UECC then procedure is
             * complete then any further Error Ind messages should be discarded */

            /* OR If All the responses received from UECC but UE Context not
             * released successfully on UECC even then procedure is complete but 
             * no need of sending error_ind to S1AP as cause invalid */


            /* UE context release procedure is complete,
             * delete node from search tree */

            st_delete_node (&(p_ueccmd_gl_ctx->ueccmd_ue_ctx_search_tree),
                    &(ue_ctx_release_node->anchor));

            rrc_mem_free((void *)ue_ctx_release_node);
        }
        else
        {
            ue_ctx_release_node->num_resp_recv++;

            src_module_id = rrc_get_src_module_id (p_api);

            if ((src_module_id <= RRC_UECC_8_MODULE_ID) &&
                    (src_module_id >= RRC_UECC_1_MODULE_ID))
            {
                /* Store Response received at proper index*/ 
                ue_ctx_release_node->uecc_resp[src_module_id - RRC_UECC_1_MODULE_ID]
                    = RRC_SUCCESS;
            }

        }

    }
    RRC_UECCMD_UT_TRACE_EXIT();
}


/******************************************************************************
 *   FUNCTION NAME: ueccmd_uecc_proc_status_ind_process_msg
 *   INPUT        : void            *p_api
 *                  ueccmd_gl_ctx_t *p_ueccmd_gl_ctx
 *   OUTPUT       : none
 *   RETURNS      : none
 *   DESCRIPTION:    This function processes UECCMD_UECC_PROC_STATUS_IND message
 ******************************************************************************/
void ueccmd_uecc_proc_status_ind_process_msg
(
 void            *p_api,          /* Pointer to input API buffer */
 ueccmd_gl_ctx_t *p_ueccmd_gl_ctx /* Pointer to the UECC MD global context */
 )
{
    rrc_uecc_proc_status_ind_t            *p_status_ind = PNULL;

    RRC_UECCMD_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_ueccmd_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_uecc_proc_status_ind_t))
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }

    /* Get message payload */
    p_status_ind = (rrc_uecc_proc_status_ind_t *)((U8* )p_api + 
            RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_status_ind->cell_index);

    /* If Received Message not consistent with running Procedure */
    if ((p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_status_ind->cell_index].
                bitmask & UECCMD_TRAFFIC_TRACE_START_PROC_RUNNING) !=
            UECCMD_TRAFFIC_TRACE_START_PROC_RUNNING)
    {
        RRC_UECCMD_TRACE(RRC_WARNING, "Ignore UECC_PROC_STATUS_IND message As - Current Proc: %u",
                p_ueccmd_gl_ctx->current_proc);
        RRC_UECCMD_UT_TRACE_EXIT();
        return;
    }
    else
    {
        p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_status_ind->cell_index].
            uecc_resp_per_cell[UECCMD_TRAFFIC_TRACE_START].num_resp_recv++;

        /* If all Response received from UECC */
        if (p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_status_ind->cell_index].
                uecc_resp_per_cell[UECCMD_TRAFFIC_TRACE_START].num_resp_recv ==
                p_ueccmd_gl_ctx->num_uecc_instances)
        {
            /*Reset num_resp_recv to ZERO*/
            p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_status_ind->cell_index].
                uecc_resp_per_cell[UECCMD_TRAFFIC_TRACE_START].num_resp_recv = RRC_NULL;

            memset_wrapper(p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_status_ind->cell_index]
                    .uecc_resp_per_cell[UECCMD_TRAFFIC_TRACE_START].
                    uecc_resp, RRC_NULL, sizeof(U8)*MAX_UECC_INSTANCES);

            p_ueccmd_gl_ctx->ueccmd_cell_ctx[p_status_ind->cell_index].
                bitmask &= (U8)(~(UECCMD_TRAFFIC_TRACE_START_PROC_RUNNING));

        }

    }

    RRC_UECCMD_UT_TRACE_EXIT();
}
