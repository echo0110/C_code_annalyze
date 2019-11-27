/*******************************************************************************
*
*  FILE NAME   : x2ap_csc.c
*
*  DESCRIPTION : This file contains the functional definition of event handler
*                of diffrent events from CSC Module.
*
*  DATE 24-June-2010  NAME Ashutosh Gupta  REFERENCE  36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/

#include "commoninc.h"
#include "x2ap_db.h"
#include "rrc_x2apOam_intf.h"
#include "x2ap_utils.h"
#include "sctp_ipr.h"
#include "x2ap_api.h"
#include "sm_entry.h"
#include "x2ap_csc.h"
#include "rrc_x2apCsc_il_parser.h"
#include "rrc_x2apOam_il_parser.h"
#include "rrc_msg_mgmt.h"
#include "rrc_intrl_api.h"
#include "rrc_common_utils.h"

void x2ap_process_csc_cell_add(
        void             *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx);
void x2ap_process_csc_cell_del(
        void             *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx);
void x2ap_process_csc_cell_upd(
        void             *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx);


/*****************************************************************************
 * Function Name  : x2ap_process_csc_messages
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to x2AP global context.
 * Outputs        : None
 * Returns        : None
 * Description    : This function is invoked to handle OAM messages to x2AP.
 ******************************************************************************/
void x2ap_process_csc_messages(
        /* Pointer to input API buffer */
        void            *p_api,         

        /* Pointer to the x2AP global context data */
        x2ap_gb_context_t   *p_x2ap_gb_ctx)
{

    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

    switch (x2ap_get_api_id((U8*)p_api))
    {
        case X2AP_CSC_CELL_ADD_IND:
            x2ap_process_csc_cell_add(p_api, p_x2ap_gb_ctx);
            break;
        case X2AP_CSC_CELL_DEL_IND:
            x2ap_process_csc_cell_del(p_api, p_x2ap_gb_ctx);
            break;
        case X2AP_CSC_CELL_UPD_IND:
            x2ap_process_csc_cell_upd(p_api, p_x2ap_gb_ctx);
            break;
        case X2AP_CSC_CELL_STATUS_RESP:
            x2ap_process_csc_cell_status_resp(p_api, p_x2ap_gb_ctx);
            break;
        default:
            X2AP_TRACE(X2AP_WARNING,
                    "x2ap_process_oam_messages: "
                    "unexpected API from CSC module received");
            break;
    }

}

/*****************************************************************************
 * Function Name  : x2ap_process_csc_cell_add
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the cell addition req from CSC 
 ****************************************************************************/
void x2ap_process_csc_cell_add(
        void             *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx)
{

    x2ap_csc_cell_add_t *p_x2ap_cell_add = X2AP_P_NULL;
    U32 loop = X2AP_NULL;
    x2ap_bool_et is_cell_valid = X2AP_FALSE;
    x2ap_return_et retVal =  X2AP_FAILURE;
    /*NR_DC Changes Start*/
    U8  num_peer_nodeB = X2AP_NULL;
    /*NR_DC Changes Stop*/

    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);

    if(!p_x2ap_gb_ctx->is_prov_done)
    {
        X2AP_TRACE(X2AP_WARNING, 
                "X2AP OAM PROVISIONING is not done so ignoring message.");
        return;
    }

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size((U8*)p_api) != RRC_API_HEADER_SIZE + 
            sizeof(x2ap_csc_cell_add_t))
    {
        X2AP_TRACE(X2AP_WARNING, "Wrong message size - ignore.");
        return;
    }

    /* Decode the received message from OAM */
    p_x2ap_cell_add = (x2ap_csc_cell_add_t*)((U8 *)p_api +
            RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_x2ap_cell_add->cell_index);

    /* Check Valid as well as Active cell status */
    retVal = x2ap_check_active_cell_status(p_x2ap_gb_ctx, 
            p_x2ap_cell_add->cell_index,
            &is_cell_valid);
    /*SPR_16723_START*/
    /* Check and update if some cell specific
     * parameters have been updated in cell
     * configuration after x2ap provisioning */
    x2ap_update_cell_specific_params(p_x2ap_gb_ctx,
            p_x2ap_cell_add);
    /*SPR_16723_END*/

    if(is_cell_valid == X2AP_TRUE)
    {
        /* 
         * check if csc timer has expired or not
         */
        if (X2AP_P_NULL == p_x2ap_gb_ctx->cell_up_guard_tim_id)
        {
            /* 
             * If cell has been already been marked as active 
             * then enbConf will be sent to those enbs who are direct neigh of this cell
             * and whose x2setup is done
             */
            if(retVal == X2AP_SUCCESS)
            {
                X2AP_TRACE(X2AP_INFO,"Send ENB Configuration Update to all eNBs "
                        "who are direct neighbours of this cell "
                        "and whose x2setup is done");
            }
            /*
             * Else 
             * Cell is marked as active in X2AP Global context and x2setup sm would
             * be invoked to send X2setup request message
             */
            else
            {
                /*Mark the cell to be active in X2AP global Context */
                /* X2AP Fix_AG_1 : Start */ 
                if(X2AP_SUCCESS == x2ap_update_active_cell_status(p_x2ap_gb_ctx, 
                            p_x2ap_cell_add->cell_index,
                            X2AP_TRUE))
                    /* X2AP Fix_AG_1 : End */ 
                {
                    /*NR_DC Changes Start*/
#ifdef ENDC_ENABLED
                    num_peer_nodeB = p_x2ap_gb_ctx->num_peer_enb + p_x2ap_gb_ctx->num_peer_gnb;
                    X2AP_TRACE(X2AP_INFO, 
                            "Number of peer eNB[%d] gNB[%d]\n", 
                            p_x2ap_gb_ctx->num_peer_enb,
                            p_x2ap_gb_ctx->num_peer_gnb);
#else
                    num_peer_nodeB = p_x2ap_gb_ctx->num_peer_enb;
                    X2AP_TRACE(X2AP_INFO, 
                            "Number of peer eNB[%d]\n", 
                            p_x2ap_gb_ctx->num_peer_enb);
#endif
                    for (loop = 0; loop < num_peer_nodeB; ++loop)
                        /*NR_DC Changes Stop*/
                    {
                        if (NOT_ASSOCIATED == p_x2ap_gb_ctx->
                                peer_enb_cntxt_arr[loop].op_status)
                        {
                            X2AP_TRACE(X2AP_INFO, 
                                    "peer cntxt no %d is not sctp associated\n", 
                                    loop);

                            /*SPR_18992_START*/
                            // continue;

                            if(X2AP_TRUE == p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].is_enb_conf)
                            {
                                X2AP_TRACE(X2AP_INFO,"Invoking x2ap_sctp_sm_entry ");
                                x2ap_sctp_sm_entry(
                                        X2AP_SCTP_CONNECT_EV,
                                        /*coverity_fix*/
                                        (U8*)p_x2ap_gb_ctx,
                                        /*coverity_fix*/
                                        p_x2ap_gb_ctx,
                                        &(p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop]));

                            }
                            else
                            {
                                X2AP_TRACE(X2AP_INFO,"eNB info at for eNB at "
                                        "index=%u is not configured", loop);
                            }
                        }
                        /*SPR_18992_END*/
                        else
                        {
                            X2AP_TRACE(X2AP_INFO,"trigger fsm entry for X2AP_CSC_CELL_ADD_IND ");
                            x2setup_sm_entry(X2AP_CSC_CELL_ADD_IND,
                                    (U8*)p_x2ap_cell_add,p_x2ap_gb_ctx,
                                    &(p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop]) );
                        }
                    }
                }
            }
        }
        else
        {
            /*
             * Cell Setup Ind Timer has not expired hence just mark the cell 
             * active in X2AP Global Context 
             */
            X2AP_TRACE(X2AP_INFO, 
                    "Waiting for cell setup timer expiry");
            /* X2AP Fix_AG_1 : Start */ 
            x2ap_update_active_cell_status(p_x2ap_gb_ctx, 
                    p_x2ap_cell_add->cell_index,
                    X2AP_TRUE);
            /* X2AP Fix_AG_1 : End */ 
        }
    }
}

/*****************************************************************************
 * Function Name  : x2ap_process_csc_cell_status_resp
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes csc cell status response.
 ****************************************************************************/
void x2ap_process_csc_cell_status_resp(
        void             *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx)
{

    x2ap_csc_cell_status_resp_t *p_x2ap_cell_status_resp = X2AP_P_NULL;
    U32 loop = X2AP_NULL;
    U32 cell_count = X2AP_NULL;
    U16 num_act_cells = X2AP_NULL;
    U16 act_cells_arr[MAX_NUM_CELL_IN_X2AP] = {X2AP_NULL};
    /*NR_DC Changes Start*/
    U8  num_peer_nodeB = X2AP_NULL;
    /*NR_DC Changes Stop*/

    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);


    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size((U8*)p_api) != RRC_API_HEADER_SIZE + 
            sizeof(x2ap_csc_cell_status_resp_t))
    {
        X2AP_TRACE(X2AP_WARNING, "Wrong message size - ignore.");
        return;
    }

    /* Decode the received message from CSC */
    p_x2ap_cell_status_resp = (x2ap_csc_cell_status_resp_t*)((U8 *)p_api +
            RRC_API_HEADER_SIZE);

    p_x2ap_gb_ctx->no_of_uecc_instances = 
        p_x2ap_cell_status_resp->no_of_uecc_instances;

    for(cell_count=0; 
            cell_count < (p_x2ap_cell_status_resp->num_served_cells); 
            cell_count++)
    {
        SET_CELL_INDEX(p_x2ap_cell_status_resp->cell_status_info[cell_count].cell_index);

        if(X2AP_CSC_CELL_UP == 
                p_x2ap_cell_status_resp->cell_status_info[cell_count].status)
        {
            x2ap_update_active_cell_status(p_x2ap_gb_ctx, 
                    p_x2ap_cell_status_resp->cell_status_info[cell_count].cell_index,
                    X2AP_TRUE);
        }
        else
        {
            x2ap_update_active_cell_status(p_x2ap_gb_ctx, 
                    p_x2ap_cell_status_resp->cell_status_info[cell_count].cell_index,
                    X2AP_FALSE);
        }
    }

    x2ap_check_active_cell(&num_act_cells, act_cells_arr, p_x2ap_gb_ctx);

    if (X2AP_P_NULL == p_x2ap_gb_ctx->cell_up_guard_tim_id)
    {
        if(num_act_cells >0)
        {
/*NR_DC Changes Start*/
#ifdef ENDC_ENABLED
            num_peer_nodeB = p_x2ap_gb_ctx->num_peer_enb + p_x2ap_gb_ctx->num_peer_gnb;
            X2AP_TRACE(X2AP_INFO, 
                    "Number of peer eNB[%d] gNB[%d]\n", 
                     p_x2ap_gb_ctx->num_peer_enb,
                     p_x2ap_gb_ctx->num_peer_gnb);
#else
            num_peer_nodeB = p_x2ap_gb_ctx->num_peer_enb;
            X2AP_TRACE(X2AP_INFO, 
                    "Number of peer eNB[%d]\n", 
                     p_x2ap_gb_ctx->num_peer_enb);
#endif

            for (loop = 0; loop < num_peer_nodeB; ++loop)
/*NR_DC Changes Stop*/
            {
                if (NOT_ASSOCIATED == p_x2ap_gb_ctx->
                        peer_enb_cntxt_arr[loop].op_status)
                {
                    X2AP_TRACE(X2AP_INFO, 
                            "peer cntxt no %d is not sctp associated\n", 
                            loop);
                    continue;
                }
                x2setup_sm_entry(X2AP_CSC_CELL_STATUS_RESP, 
                        (U8*)p_x2ap_cell_status_resp,
                        p_x2ap_gb_ctx, 
                        &(p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop]));
            }
        }
    }
    else
    {
        /*
         * Cell Setup Ind Timer has not expired hence just mark the cell 
         * active in X2AP Global Context 
         */
        X2AP_TRACE(X2AP_INFO, 
                "Waiting for cell setup timer expiry.");
    }
}
/* X2AP Fix_AG_1 : Start */ 
/*****************************************************************************
 * Function Name  : x2ap_process_csc_cell_del
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the cell deletion update request
 *                  received from CSC .
 ****************************************************************************/
void x2ap_process_csc_cell_del(
        void             *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx)
{
    
    x2ap_return_et retVal =  X2AP_FAILURE;
    x2ap_csc_cell_del_t *p_x2ap_cell_del = X2AP_P_NULL;
    U16 num_act_cells = X2AP_NULL;
    U16 act_cells_arr[MAX_NUM_CELL_IN_X2AP] = {X2AP_NULL};
    x2ap_bool_et is_cell_valid = X2AP_FALSE;


    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);


    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size((U8*)p_api) != RRC_API_HEADER_SIZE + 
            sizeof(x2ap_csc_cell_del_t))
    {
        X2AP_TRACE(X2AP_WARNING, "Wrong message size - ignore.");
        return;
    }

    /* Decode the received message from OAM */
    p_x2ap_cell_del = (x2ap_csc_cell_del_t*)((U8*)p_api +
        RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_x2ap_cell_del->cell_index);

    /* Check Valid as well as Active cell status */
    retVal = x2ap_check_active_cell_status(p_x2ap_gb_ctx, 
            p_x2ap_cell_del->cell_index,
            &is_cell_valid);

    x2ap_check_active_cell(&num_act_cells, act_cells_arr, p_x2ap_gb_ctx);
    if (0 == num_act_cells)
    {
        X2AP_TRACE(X2AP_WARNING, 
            "No cell was active to mark as inactive for CSC cell del");
    }
    if(is_cell_valid == X2AP_TRUE)
    {
        if(retVal == X2AP_SUCCESS)
        {
            X2AP_TRACE(X2AP_INFO, 
                    "Marking Cell as Inactive");
            x2ap_update_active_cell_status(p_x2ap_gb_ctx, 
                    p_x2ap_cell_del->cell_index,
                    X2AP_FALSE);
            X2AP_TRACE(X2AP_INFO,"Send ENB Configuration Update to all eNBs "
                        "who are direct neighbours of this cell "
                        "and whose x2setup is done");
        }
    }
}
/* X2AP Fix_AG_1 : End */ 


/*****************************************************************************
 * Function Name  : x2ap_update_served_cell_info
 * Inputs         : p_api - Pointer to cell update struct
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the cell modification update request
 *                  received from CSC .
 ****************************************************************************/
static void x2ap_update_served_cell_info(
        x2ap_gb_context_t      *p_x2ap_gb_ctx, 
        x2ap_csc_cell_upd_t    *p_x2ap_cell_upd)
{
    X2AP_TRACE(X2AP_DETAILED,"Unused variable p_x2ap_gb_ctx :%p,p_x2ap_cell_upd:%p",
            p_x2ap_gb_ctx,p_x2ap_cell_upd);
    return;
}

/*****************************************************************************
 * Function Name  : x2ap_process_csc_cell_upd
 * Inputs         : p_api - Pointer to input API buffer
 *                  p_x2ap_gb_ctx - Pointer to the x2AP global context data
 *
 * Outputs        : None
 * Returns        : none
 * Description    : This function processes the cell modification update responce
 *                  received from CSC .
 ****************************************************************************/
void x2ap_process_csc_cell_upd(
        void             *p_api,
        x2ap_gb_context_t   *p_x2ap_gb_ctx)
{
    x2ap_csc_cell_upd_t *p_x2ap_cell_upd = X2AP_P_NULL;
    S32 length_read = X2AP_NULL;
    x2ap_return_et response = X2AP_FAILURE;
    U16 loop = X2AP_NULL;
    U16 num_nbr_cells = X2AP_NULL;
    U16 num_enbid = X2AP_NULL;
    U16 nbr_cell_indexes[MAX_NEIGHBOURS] = {X2AP_NULL};
    x2_gb_enb_id_t   gb_enb_id_arr[MAX_NEIGHBOURS]; 
    sctp_gb_enb_id_search_node_t * p_tree_node = X2AP_P_NULL;

    X2AP_ASSERT(X2AP_P_NULL != p_api);
    X2AP_ASSERT(X2AP_P_NULL != p_x2ap_gb_ctx);


    p_x2ap_cell_upd = (x2ap_csc_cell_upd_t*)
                    x2ap_mem_get(sizeof(x2ap_csc_cell_upd_t));
    
    if (PNULL == p_x2ap_cell_upd)
    {
        X2AP_TRACE(X2AP_WARNING, "Unable to allocate memory.");
        return;
    }

    /* Decode the received message from OAM */

     response = rrc_il_parse_x2ap_csc_cell_upd(
            p_x2ap_cell_upd ,
            (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
            x2ap_get_api_buf_size(p_api) -
            RRC_FULL_INTERFACE_HEADERS_SIZE,
            &length_read);

    SET_CELL_INDEX(p_x2ap_cell_upd->cell_index);

    if (X2AP_SUCCESS == response)
    {
        X2AP_TRACE(X2AP_INFO, "x2ap CSC CELL UPDATE RESP encode success");
        x2ap_get_ngh_cell_index(&num_nbr_cells, nbr_cell_indexes, 
                p_x2ap_cell_upd->cell_index, p_x2ap_gb_ctx, X2AP_TRUE);
        x2ap_get_uniq_enbid_set_from_nbr_cell_indexes(p_x2ap_gb_ctx,
                num_nbr_cells, nbr_cell_indexes, &num_enbid, gb_enb_id_arr);
        x2ap_update_served_cell_info(p_x2ap_gb_ctx, p_x2ap_cell_upd);
        for (loop = 0; (loop < num_enbid) && (loop < MAX_NEIGHBOURS); ++loop)
        {
            p_tree_node = (sctp_gb_enb_id_search_node_t *) 
                x2ap_st_find_tree_node((x2ap_search_tree_t *)
                        &(p_x2ap_gb_ctx->enb_ctxt_trees.
                            enb_id_search_tree), 
                        (void *) &gb_enb_id_arr[loop]);
            if (!p_tree_node)
            {
                X2AP_TRACE(X2AP_WARNING, 
                        "node not found for given gb_enb id\n");
                continue;
            }
            else if (MAX_PEER_ENB > loop)
            {
                if (loop < MAX_PEER_ENB)
                {
                    if(OPERATIONAL ==
                            p_x2ap_gb_ctx->peer_enb_cntxt_arr[loop].op_status)
                    {
                    }
                }
            }
        }

    }
    else
    {
        X2AP_TRACE(X2AP_WARNING, "x2ap CSC CELL UPDATE RESP encode failed");
    }
    /* coverity_fix_63318_start */
    x2ap_mem_free((void *)p_x2ap_cell_upd);
   /* Coverity ID 85916 Fix Start*/
   /* p_x2ap_cell_upd = X2AP_P_NULL; */
    /* Coverity ID 85916 Fix End*/
    /* coverity_fix_63318_stop */
}

/*****************************************************************************
 * Function Name  : x2ap_build_and_send_csc_cell_status_req 
 * Inputs         : x2ap_gb_context_t
 * Outputs        : None
 * Returns        : X2AP_FAILURE/X2AP_SUCCESS 
 * Description    : This function  build and send cell status req msg
 *                  to csc module.
 ****************************************************************************/
x2ap_return_et x2ap_build_and_send_csc_cell_status_req(
        x2ap_gb_context_t *p_x2ap_gb_ctx)
{
    x2ap_csc_cell_status_req_t    *msg_p = PNULL;
    void *p_intrl_msg                     = PNULL;
    U16 count = 0;

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);

    p_intrl_msg = x2ap_alloc_intrl_msg(
            RRC_CSC_MODULE_ID,
            X2AP_CSC_CELL_STATUS_REQ,
            (U16)(sizeof(x2ap_csc_cell_status_req_t)));

    if (PNULL == p_intrl_msg)
    {
        /*out of memory*/
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    msg_p = (x2ap_csc_cell_status_req_t *)((U8 *)p_intrl_msg +
            RRC_API_HEADER_SIZE);

    msg_p->num_served_cells = p_x2ap_gb_ctx->num_served_cells;

    for(count=0; count < (p_x2ap_gb_ctx->num_served_cells); count++)
    {
        msg_p->cell_info[count].cell_index 
            = p_x2ap_gb_ctx->act_cell_info[count].cell_id;
    }

    X2AP_TRACE(X2AP_DETAILED, "Forwarding message to CSC");

    /* Send Request to CSC */
    rrc_send_message(
            p_intrl_msg,
            RRC_CSC_MODULE_ID);

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;

}

