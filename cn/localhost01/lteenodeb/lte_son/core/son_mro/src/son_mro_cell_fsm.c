/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_mro_cell_fsm.c $
 *
 *******************************************************************************
 *
 * File Description: This file contains MRO cell FSM functions
 *
 *******************************************************************************
 * Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Dec, 2012       Ravi/Shilpi Initial         Initial
 *   July, 2014      Shilpi                      SPR 12708 Fix 
 ******************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_mro_cell_fsm.h>
#include <son_anr_ctxt_mgr.h>
#include <son_mro_intf.h>
#include <son_anr_table.h>
#include <rrm_son_composer.h>
#include <son_anr_cell_fsm.h>
#include <x2ap_api.h>

/****************************************************************************
 * Global Variables
 ****************************************************************************/
extern mro_global_context_t g_mro_gb_ctxt;
extern const son_8 *p_son_mro_facility_name;
extern const son_s8 *SON_MRO_CELL_FSM_EVENT_NAMES[];

/*****************************************************************************
 * Function Name  : mro_construct_send_rrm_register_req
 * Inputs         : transaction_id  transaction identifier 
 *                : p_cell_id       Pointer to global Cell Id
 * Outputs        : None
 * Returns        : SUCCESS - If message is sent successfully
 *                  FAILURE - Otherwise
 * Description    : Constructs and sends RRM_REGISTER_REQ to RRM
 ****************************************************************************/
static son_return_et
mro_construct_send_rrm_register_req
(
    son_u16                         transaction_id,
    son_intra_rat_global_cell_id_t  *p_cell_id
)
{
    son_rrm_register_req_t req = {0};
    son_return_et ret_val = SON_SUCCESS;

    SON_UT_TRACE_ENTER();

    req.transaction_id = transaction_id;
    req.req.register_req.object.type = RRM_OBJ_CELL;

    SON_MEMCPY(&(req.req.register_req.object.id.object_id.cell_id), p_cell_id,
        sizeof(son_intra_rat_global_cell_id_t));

    req.req.register_req.object.attr_list_size = 3;

    /* Cell State Change Indication Registeration */
    req.req.register_req.object.p_attr_lst[0].id = RRM_ATTR_CELL_STATE;
    req.req.register_req.object.p_attr_lst[0].trgr_list_size = 1;
    req.req.register_req.object.p_attr_lst[0].trgr_list[0].id =
        RRM_TRGR_EVENT;
    req.req.register_req.object.p_attr_lst[0].trgr_list[0].bitmask |=
        RRM_TRIGGER_TYPE_EVENT;
    req.req.register_req.object.p_attr_lst[0].trgr_list[0].
        event.enable = SON_TRUE;

    /* HO Attempt indication Registration */
    req.req.register_req.object.p_attr_lst[1].id = RRM_ATTR_HO_ATTEMPT_IND;
    req.req.register_req.object.p_attr_lst[1].trgr_list_size = 1;
    req.req.register_req.object.p_attr_lst[1].trgr_list[0].id =
        RRM_TRGR_EVENT;
    req.req.register_req.object.p_attr_lst[1].trgr_list[0].bitmask |=
        RRM_TRIGGER_TYPE_EVENT;
    req.req.register_req.object.p_attr_lst[1].trgr_list[0].
        event.enable = SON_TRUE;

    /* HO Failure Report Registration */
    req.req.register_req.object.p_attr_lst[2].id = RRM_ATTR_HO_FAILURE_REPORT;
    req.req.register_req.object.p_attr_lst[2].trgr_list_size = 1;
    req.req.register_req.object.p_attr_lst[2].trgr_list[0].id =
        RRM_TRGR_EVENT;
    req.req.register_req.object.p_attr_lst[2].trgr_list[0].bitmask |=
        RRM_TRIGGER_TYPE_EVENT;
    req.req.register_req.object.p_attr_lst[2].trgr_list[0].
        event.enable = SON_TRUE;

    /* Invoke PUP tool to send message to RRM */
    if (RRM_FAILURE == rrm_son_send_rrm_son_register_req(&req.req,
                SON_MRO_MODULE_ID, SON_RRM_MODULE_ID,
                transaction_id, SON_NULL))
    {
        ret_val = SON_FAILURE;
        SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                SON_BRIEF,
                "mro_construct_send_rrm_register_req: Failed to send "
                "RRM_SON_REGISTER_REQ from MRO to RRM for cell [Cell Id: 0x%x]",
                son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
    }
    else
    {
        SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                SON_BRIEF,
                "Message "
                "RRM_SON_REGISTER_REQ send from MRO to"
                " RRM for cell [Cell Id: 0x%x]",
                son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));
    }

    SON_UT_TRACE_EXIT();
    return ret_val;
} /* mro_construct_send_rrm_register_req */

/*****************************************************************************
 * Function Name  : son_mro_cell_fsm_enable_req_handler
 * Inputs         : evnt_id - Value of the event Id internal to MRO module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_mro_msg - Pointer to the message data received (after the
 *                  CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for SONMRO_ENABLE_REQ
 *                  message received from SONMgmtIfH.
 ****************************************************************************/
static son_void_t
son_mro_cell_fsm_enable_req_handler
(
 son_mro_cell_index_et  evnt_id,
 STACKAPIHDR            *p_cspl_hdr,
 mro_cell_context_t     *p_cell_ctxt,
 son_u8                 *p_mro_msg
 )
{
    son_mro_enable_req_t *p_req = (son_mro_enable_req_t *)p_mro_msg;
    anr_cell_context_t *p_anr_cell_ctxt = SON_PNULL;
    son_error_et error_code = SON_NO_ERROR;
    son_feature_state_et state = SON_STATE_UNDEFINED;
    son_feature_state_change_cause_et cause = SON_STATE_CHANGE_CAUSE_UNDEFINED;

    SON_UT_TRACE_ENTER();

    /* Check from LTE SON/ANR context whether the Cell Id is present 
     * in ANR module cell context and ANR mode is ENABLED or not
     * for the cell. */
    p_anr_cell_ctxt = anr_get_cell_context_from_global_context(
        &p_cell_ctxt->cell_id);
    if (SON_PNULL != p_anr_cell_ctxt && 
            (ANR_CELL_STATE_ENABLED == ( 
            anr_get_anr_mode_for_cell(p_anr_cell_ctxt))))
    {
        /* Send register request to RRM */
        if (SON_SUCCESS == mro_construct_send_rrm_register_req(
                    p_req->transaction_id,
                    &p_cell_ctxt->cell_id))
        {
            /* Change Cell FSM state to MRO_CELL_STATE_ENABLING */
            SONMRO_CELL_FSM_SET_STATE(p_cell_ctxt,
                    p_cell_ctxt->previous_cell_fsm_state,
                    p_cell_ctxt->current_cell_fsm_state,
                    MRO_CELL_STATE_ENABLING);
        }
        else
        {
            error_code = SON_ERR_COMPOSER_ERROR; 
        }
    }
    else if (SON_PNULL == p_anr_cell_ctxt)
    {
        error_code = SON_ERR_CELL_UNCONFIGURED_AT_ANR;      
        cause = SON_ANR_CELL_UNCONFIGURED;
        state = SON_STATE_DISABLED;
    }
    else
    {
        error_code = SON_ERR_ANR_MODE_IS_DISABLED;    
        cause = SON_ANR_MODE_DISABLED;
        state = SON_STATE_DISABLED;
    }

    if (SON_NO_ERROR != error_code)
    {
        cell_status_list_data_t *p_list_node = SON_PNULL;
        /* Search for the cell Id in pending response structure. */
        p_list_node = mro_find_cell_in_pending_res(&(p_cell_ctxt->cell_id));

        /* Presence indicates that this event is triggered due to Global 
         * Enable Request or Cell Info Indication and no immediate Enable
         * Response must be sent from cell FSM */
        if (SON_PNULL != p_list_node)
        {
            /* Update Cell in MRO Pending Response List with error 
             *  code determined above */
            p_list_node->cell_status.error_code = error_code;
            /* Decrement expected_response_count by 1 */
            mro_decrement_pending_res_expected_count();
            if (SMIF_CELL_INFO_IND == p_cspl_hdr->api)
            {
                mro_send_feature_state_change_ind(&p_cell_ctxt->cell_id,
                        state, cause);
            }
        }
        /* Cell specific Enable Request is being processed */
        else
        {
            son_mro_enable_res_t enable_res;
            son_size_t enable_res_len = sizeof(son_mro_enable_res_t);
            /* Send SONMRO_ENABLE_RES to SONMgmtIfH with faliure */
            SON_MEMSET(&enable_res, 0, enable_res_len);
            enable_res.transaction_id = p_req->transaction_id;
            enable_res.enable_res.result = SON_FAILURE;
            enable_res.enable_res.error_code = SON_NO_ERROR;
            enable_res.enable_res.cell_status_list_size = 1;
            SON_MEMCPY(&enable_res.enable_res.cell_status_list[0].cgi,
                &p_cell_ctxt->cell_id,
                sizeof(son_intra_rat_global_cell_id_t));
            enable_res.enable_res.cell_status_list[0].error_code = error_code;
            son_create_send_buffer((son_u8 *)&enable_res,
                    SON_MRO_MODULE_ID, SON_MIF_MODULE_ID, SONMRO_ENABLE_RES,
                    enable_res_len);
        }
    }
                    /*SPR 17777 +-*/
    SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,SON_DETAILEDALL,"Unused variable evnt_id=%d",evnt_id);
                    /*SPR 17777 +-*/
    SON_UT_TRACE_EXIT();
} /* son_mro_cell_fsm_enable_req_handler */

/******************************************************************************
 * Function Name  : mro_construct_send_rrm_deregister_req
 * Inputs         : transaction_id  Transaction Id
 *                  p_cell_id   Pointer to global Cell_Id
 * Outputs        : None
 * Returns        : SUCCESS - If message is sent successfully
 *                  FAILURE - Otherwise
 * Description    : Constructs and sends RRM_DEREGISTER_REQ to RRM
 *****************************************************************************/
son_return_et
mro_construct_send_rrm_deregister_req
(
    son_u16                         transaction_id,
    son_intra_rat_global_cell_id_t  *p_cell_id
)
{
    son_rrm_deregister_req_t    dereg_req = {0};
    son_return_et ret_val = SON_FAILURE;

    SON_UT_TRACE_ENTER();

    dereg_req.transaction_id = transaction_id;
    dereg_req.req.deregister_req.object.type = RRM_OBJ_CELL;
    SON_MEMCPY(&(dereg_req.req.deregister_req.object.id.object_id.cell_id),
            p_cell_id,
            sizeof(son_intra_rat_global_cell_id_t));
    dereg_req.req.deregister_req.object.attr_list_size = 3;

    /* Cell State Change Indication De-Registeration */
    dereg_req.req.deregister_req.object.p_attr_lst[0].id = RRM_ATTR_CELL_STATE;
    dereg_req.req.deregister_req.object.p_attr_lst[0].trgr_list_size = 0;

    /* HO Attempt indication De-Registration */
    dereg_req.req.deregister_req.object.p_attr_lst[1].id =
        RRM_ATTR_HO_ATTEMPT_IND;
    dereg_req.req.deregister_req.object.p_attr_lst[1].trgr_list_size = 0;

    /* HO Failure Report De-Registration */
    dereg_req.req.deregister_req.object.p_attr_lst[2].id =
        RRM_ATTR_HO_FAILURE_REPORT;
    dereg_req.req.deregister_req.object.p_attr_lst[2].trgr_list_size = 0;

    /* PUP Tool call to create and send DE-REGISTER Message to RRM */
    if (RRM_SUCCESS == rrm_son_send_rrm_son_deregister_req(
                &dereg_req.req,
                SON_MRO_MODULE_ID, SON_RRM_MODULE_ID,
                transaction_id, SON_NULL))
    {
        SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                SON_BRIEF,
                "mro_construct_send_rrm_deregister_req: "
                "Message %u send from %u to %u for Cell: 0x%x\n",
                RRM_SON_DEREGISTER_REQ,
                SON_MRO_MODULE_ID,
                SON_RRM_MODULE_ID,
                son_convert_char_cell_id_to_int_cell_id(p_cell_id->cell_identity));

        ret_val = SON_SUCCESS;
    }
    else
    {
        SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, SON_ERROR,
                "mro_construct_send_rrm_deregister_req:Message sending"
                " failure to RRM \n");
    }

    SON_UT_TRACE_EXIT();
    return ret_val;
} /* mro_construct_send_rrm_deregister_req */

/*****************************************************************************
 * Function Name  : son_mro_cell_fsm_disable_req_handler
 * Inputs         : evnt_id - Value of the event Id internal to MRO module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_mro_msg - Pointer to the message data received (after the
 *                  CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for SONMRO_DISABLE_REQ
 *                  message received from SONMgmtIfH.
 ****************************************************************************/
static son_void_t
son_mro_cell_fsm_disable_req_handler
(
 son_mro_cell_index_et  evnt_id,
 STACKAPIHDR            *p_cspl_hdr,
 mro_cell_context_t     *p_cell_ctxt,
 son_u8                 *p_mro_msg
)
{
    son_mro_disable_req_t *p_req = (son_mro_disable_req_t *)p_mro_msg;

    SON_UT_TRACE_ENTER();
    /* Send Deregister request to RRM */
    if (SON_SUCCESS == mro_construct_send_rrm_deregister_req(
                p_req->transaction_id, 
                &p_cell_ctxt->cell_id))
    {
        p_cell_ctxt->dereg_invoking_api = SON_OAM_MRO_DISABLE_REQ;
        SONMRO_CELL_FSM_SET_STATE(p_cell_ctxt,
                p_cell_ctxt->previous_cell_fsm_state,
                p_cell_ctxt->current_cell_fsm_state,
                MRO_CELL_STATE_DISABLING);
    }
    else
    {
        /* Send failure to SONMgmtIfH */
        son_mro_disable_res_t disable_res;
        son_size_t disable_res_len = sizeof(son_mro_disable_res_t);

        SON_MEMSET(&disable_res, 0, disable_res_len);
        disable_res.transaction_id = p_req->transaction_id;
        disable_res.disable_res.result = SON_FAILURE;
        disable_res.disable_res.error_code = SON_NO_ERROR;
        disable_res.disable_res.cell_status_list_size = 1;
        SON_MEMCPY(&disable_res.disable_res.cell_status_list[0].cgi,
                &p_cell_ctxt->cell_id,
                sizeof(son_intra_rat_global_cell_id_t));
        disable_res.disable_res.cell_status_list[0].error_code =
                SON_ERR_COMPOSER_ERROR;

        son_create_send_buffer((son_u8 *)&disable_res,
                SON_MRO_MODULE_ID, SON_MIF_MODULE_ID, SONMRO_DISABLE_RES,
                disable_res_len);
    }

                    /*SPR 17777 +-*/
    SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,SON_DETAILEDALL,"Unused variable evnt_id=%d p_cspl_hdr=%p",evnt_id,p_cspl_hdr);
                    /*SPR 17777 +-*/
    SON_UT_TRACE_EXIT();
} /* son_mro_cell_fsm_disable_req_handler */

/*****************************************************************************
 * Function Name  : mro_stop_algo
 * Inputs         : p_cell_ctxt - Pointer to the cell context
 * Outputs        : None
 * Returns        : None
 * Description    : This is the helper function to Stop MRO algo when the
 *                  request is received to modify either the mro mode or
 *                  mro param.
 ****************************************************************************/
static son_void_t
mro_stop_algo
(
    mro_cell_context_t     *p_cell_ctxt
)
{
    anr_cell_context_t *p_anr_cell_ctxt = SON_PNULL;
    SON_HASH *p_hash = SON_PNULL;
    anr_intra_rat_hash_nr_tuple_t *p_intra_tpl = SON_PNULL;

    SON_UT_TRACE_ENTER();

    /* Clean up the algo specific parameters and HO counters */
    p_anr_cell_ctxt = anr_get_cell_context_from_global_context(
        &p_cell_ctxt->cell_id);
    if (SON_PNULL != p_anr_cell_ctxt)
    {
        p_hash = &p_anr_cell_ctxt->intra_rat_neighbor_table;
        p_intra_tpl  = anr_get_first_nr_from_intra_rat_hash_table(
            p_hash);

        while (SON_PNULL != p_intra_tpl)
        {
            mro_reset_cio_algo_params(&p_intra_tpl->data);
            mro_reset_ho_counters(&p_intra_tpl->data);
            p_intra_tpl = anr_get_next_nr_from_intra_rat_hash_table(
                p_hash, &p_intra_tpl->anchor);
        }

        mro_reset_ttt_algo_params(p_cell_ctxt);
    }

    /* Re-start the algo timer */
    if (SON_TRUE == son_is_timer_running(
            p_cell_ctxt->ho_failure_accumulation_duration_timer_id))
    {
        son_stop_timer(p_cell_ctxt->
            ho_failure_accumulation_duration_timer_id);
        p_cell_ctxt->ho_failure_accumulation_duration_timer_id =
            /* PR 862 Fix Start */
            /* SPR-12708 Fix Starts */
            son_start_timer(
                    (p_cell_ctxt->mro_config_param.
                     mro_ho_failure_accumulation_duration
                     * SON_SECONDS_IN_A_MIN),
                    &p_cell_ctxt->cell_id, sizeof(son_intra_rat_global_cell_id_t), 
                    SON_TRUE);
        /* SPR-12708 Fix Ends */
        /* PR 862 Fix End */
    }

    SON_UT_TRACE_EXIT();
} /* mro_stop_algo */

/*****************************************************************************
 * Function Name  : son_mro_cell_fsm_modify_config_req_handler
 * Inputs         : evnt_id - Value of the event Id internal to MRO module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_mro_msg - Pointer to the message data received (after the
 *                  CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for
 *                  SONMRO_MODIFY_CONFIG_PARAM_REQ message received from 
 *                  SONMgmtIfH.
 ****************************************************************************/
static son_void_t
son_mro_cell_fsm_modify_config_req_handler
(
 son_mro_cell_index_et  evnt_id,
 STACKAPIHDR            *p_cspl_hdr,
 mro_cell_context_t     *p_cell_ctxt,
 son_u8                 *p_mro_msg
 )
{
    son_mro_modify_config_params_req_t *p_req = 
                (son_mro_modify_config_params_req_t *)p_mro_msg;
    son_mro_modify_config_params_res_t modify_res;
    son_size_t modify_res_len = sizeof(son_mro_modify_config_params_res_t);
    son_return_et result = SON_SUCCESS;
    son_error_et error_code = SON_NO_ERROR;
    SON_UT_TRACE_ENTER();

    SON_MEMSET(&modify_res, 0, modify_res_len);
    modify_res.transaction_id = p_req->transaction_id;
    SON_MEMCPY(&modify_res.config_param_res.srv_cgi,
            &p_cell_ctxt->cell_id,
            sizeof(son_intra_rat_global_cell_id_t));

    /* Check if both start and stop threshold are to be modified, If yes then 
        validate both of them. */
    if ((SON_MRO_HO_OPTIMIZATION_START_THRESHOLD_PRESENT | 
                SON_MRO_HO_OPTIMIZATION_STOP_THRESHOLD_PRESENT)&
            p_req->config_param_req.eutran_config_params.bitmask)
    {
        if (p_req->config_param_req.eutran_config_params.
                mro_ho_optimization_start_threshold >
                p_req->config_param_req.eutran_config_params.
                mro_ho_optimization_stop_threshold)
        {
            /* PR 861 Fix Start */
            p_cell_ctxt->mro_config_param.mro_ho_optimization_start_threshold =
                (son_float)p_req->config_param_req.eutran_config_params.
                mro_ho_optimization_start_threshold / MRO_SCALE_FACTOR;
            p_cell_ctxt->mro_config_param.mro_ho_optimization_stop_threshold =
                (son_float)p_req->config_param_req.eutran_config_params.
                mro_ho_optimization_stop_threshold / MRO_SCALE_FACTOR;
        }
        else
        {
            SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, 
                    SON_ERROR,
                    "son_mro_cell_fsm_modify_config_req_handler: MRO HO "
                    "Optimization Stop threshold %f received in modify "
                    "request is greater than or equal to start threshold %f"
                    " received in modify request so start and stop thresholds"
                    " are not modified for serving Cell Id: 0x%x",
                    (son_float)p_req->config_param_req.eutran_config_params.
                    mro_ho_optimization_stop_threshold / MRO_SCALE_FACTOR,
                    (son_float)p_req->config_param_req.eutran_config_params.
                    mro_ho_optimization_start_threshold / MRO_SCALE_FACTOR,
                    son_convert_char_cell_id_to_int_cell_id(
                    p_cell_ctxt->cell_id.cell_identity));
            /* PR 861 Fix End */
            result = SON_PARTIAL_SUCCESS;
            error_code = SON_ERR_INVALID_PARAMS;
        }
    }
    /* If only start threshold is to modified */
    else if (SON_MRO_HO_OPTIMIZATION_START_THRESHOLD_PRESENT &
            p_req->config_param_req.eutran_config_params.bitmask)
    {
        if (p_req->config_param_req.eutran_config_params.
                mro_ho_optimization_start_threshold > 
                p_cell_ctxt->mro_config_param.
                mro_ho_optimization_stop_threshold)
        {
            /* PR 861 Fix Start */
            p_cell_ctxt->mro_config_param.mro_ho_optimization_start_threshold =
                (son_float)p_req->config_param_req.eutran_config_params.
                mro_ho_optimization_start_threshold / MRO_SCALE_FACTOR;
        }
        else
        {
            SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, 
                    SON_ERROR,
                    "son_mro_cell_fsm_modify_config_req_handler: MRO HO "
                    "Optimization Start threshold %f received in modify "
                    "request is less than or equal to current stop threshold"
                    "%f stored in cell context so start threshold is not "
                    "modified for serving Cell Id: 0x%x",
                    (son_float)p_req->config_param_req.eutran_config_params.
                    mro_ho_optimization_start_threshold / MRO_SCALE_FACTOR,
                    p_cell_ctxt->mro_config_param.
                    mro_ho_optimization_stop_threshold,
                    son_convert_char_cell_id_to_int_cell_id(
                    p_cell_ctxt->cell_id.cell_identity));
            /* PR 861 Fix End */
            result = SON_PARTIAL_SUCCESS;
            error_code = SON_ERR_INVALID_PARAMS;
        }
    }
    /* If only stop threshold is to modified */
    else if (SON_MRO_HO_OPTIMIZATION_STOP_THRESHOLD_PRESENT &
        p_req->config_param_req.eutran_config_params.bitmask)
    {
        if (p_req->config_param_req.eutran_config_params.
                mro_ho_optimization_stop_threshold <
                p_cell_ctxt->mro_config_param.
                mro_ho_optimization_start_threshold)
        {
            /* PR 861 Fix Start */
            p_cell_ctxt->mro_config_param.mro_ho_optimization_stop_threshold =
                (son_float)p_req->config_param_req.eutran_config_params.
                mro_ho_optimization_stop_threshold / MRO_SCALE_FACTOR;
        }
        else
        {
            SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, 
                    SON_ERROR,
                    "son_mro_cell_fsm_modify_config_req_handler: MRO HO "
                    "Optimization Stop threshold %f received in modify "
                    "request is greater than or equal to current start "
                    "threshold %f stored in cell context so stop threshold"
                    /*SPR 17777 +-*/
                    "is not modified for serving Cell Id: 0x%x;evnt_id=%d p_cspl_hdr=%p",
                    (son_float)p_req->config_param_req.eutran_config_params.
                    mro_ho_optimization_stop_threshold / MRO_SCALE_FACTOR,
                    p_cell_ctxt->mro_config_param.
                    mro_ho_optimization_start_threshold,
                    son_convert_char_cell_id_to_int_cell_id(
                    p_cell_ctxt->cell_id.cell_identity),
                    evnt_id,p_cspl_hdr);
                    /*SPR 17777 +-*/
            /* PR 861 Fix End */
            result = SON_PARTIAL_SUCCESS;
            error_code = SON_ERR_INVALID_PARAMS;
        }
    }

    if (SON_MRO_MODE_PRESENT &
        p_req->config_param_req.eutran_config_params.bitmask)
    {
        if (p_req->config_param_req.eutran_config_params.mro_mode !=
            p_cell_ctxt->mro_config_param.mro_mode)
        {
            p_cell_ctxt->mro_config_param.mro_mode =
                p_req->config_param_req.eutran_config_params.mro_mode;
            mro_stop_algo(p_cell_ctxt);
        }
    }

    if (SON_MRO_PARAM_PRESENT & 
            p_req->config_param_req.eutran_config_params.bitmask)
    {
        if (p_req->config_param_req.eutran_config_params.mro_param
            != p_cell_ctxt->mro_config_param.mro_param)
        {
            mro_stop_algo(p_cell_ctxt);
        p_cell_ctxt->mro_config_param.mro_param = 
            p_req->config_param_req.eutran_config_params.mro_param;
    }
    }

    if (SON_MRO_HO_FAILURE_ACCUMULATION_DURATION_PRESENT & 
        p_req->config_param_req.eutran_config_params.bitmask)
    {
        /* If the HO accumulation duration timer is running, stop the timer,
         *  perform HO optimization, store the new timer value in context
         *  and start the timer again with the new timer value */
        if (SON_TRUE == son_is_timer_running(
                p_cell_ctxt->ho_failure_accumulation_duration_timer_id))
        {
            son_stop_timer(p_cell_ctxt->
                ho_failure_accumulation_duration_timer_id);
            p_cell_ctxt->ho_failure_accumulation_duration_timer_id = SON_PNULL;
            mro_execute_optimization_algo(p_cell_ctxt);
            p_cell_ctxt->mro_config_param.mro_ho_failure_accumulation_duration =
                p_req->config_param_req.eutran_config_params.
                mro_ho_failure_accumulation_duration;
            /* PR 862 Fix Start */
            p_cell_ctxt->ho_failure_accumulation_duration_timer_id =
                /* SPR-12708 Fix Starts */
                son_start_timer(
                        (p_cell_ctxt->mro_config_param.
                         mro_ho_failure_accumulation_duration
                         * SON_SECONDS_IN_A_MIN),
                        &p_cell_ctxt->cell_id, sizeof(
                            son_intra_rat_global_cell_id_t), SON_TRUE);
            /* SPR-12708 Fix Ends */
            /* PR 862 Fix End */
        }

        /* Otherwise store the new timer value in context and start the timer 
         *  with the new timer value */
        else
        {
            p_cell_ctxt->mro_config_param.mro_ho_failure_accumulation_duration =
                p_req->config_param_req.eutran_config_params.
                mro_ho_failure_accumulation_duration;
            /* PR 862 Fix Start */
            p_cell_ctxt->ho_failure_accumulation_duration_timer_id =
                /* SPR-12708 Fix Starts */
                son_start_timer(
                        (p_cell_ctxt->mro_config_param.
                         mro_ho_failure_accumulation_duration
                         * SON_SECONDS_IN_A_MIN),
                        &p_cell_ctxt->cell_id, sizeof(
                            son_intra_rat_global_cell_id_t), SON_TRUE);
            /* SPR-12708 Fix Ends */
            /* PR 862 Fix End */
        }
    }

    if (SON_MRO_DELTA_CIO_PRESENT & p_req->config_param_req.
            eutran_config_params.bitmask)
    {
        p_cell_ctxt->mro_config_param.delta_cio = 
            p_req->config_param_req.eutran_config_params.delta_cio;
    }

    if (SON_MRO_HO_ATTEMPTS_THRESHOLD_PRESENT &
        p_req->config_param_req.eutran_config_params.bitmask)
    {
        p_cell_ctxt->mro_config_param.mro_ho_attempts_threshold =
            p_req->config_param_req.eutran_config_params.
            mro_ho_attempts_threshold;
    }

    if (SON_MRO_HO_FAIL_RATE_FOR_OPTIMIZATION_PRESENT &
        p_req->config_param_req.eutran_config_params.bitmask)
    {
        /* PR 861 Fix Start */
        p_cell_ctxt->mro_config_param.mro_ho_fail_rate_for_optimization =
            (son_float)p_req->config_param_req.eutran_config_params.
            mro_ho_fail_rate_for_optimization / MRO_SCALE_FACTOR;
        /* PR 861 Fix End */
    }

    if (SON_MRO_TTT_PRESENT & p_req->config_param_req.
        eutran_config_params.bitmask)
    {
        p_cell_ctxt->mro_config_param.ttt =
            p_req->config_param_req.eutran_config_params.ttt;
    }

    if (SON_MRO_DELTA_TTT_PRESENT & p_req->config_param_req.
            eutran_config_params.bitmask)
    {
        p_cell_ctxt->mro_config_param.delta_ttt = 
            p_req->config_param_req.eutran_config_params.delta_ttt;
    }

    /* PR 860 Fix Start */
    mro_print_global_context("son_mro_cell_fsm_modify_config_req_handler");
    /* PR 860 Fix End */
    modify_res.config_param_res.result = result;
    modify_res.config_param_res.error_code = error_code;
    son_create_send_buffer((son_u8 *)&modify_res,
            SON_MRO_MODULE_ID,
            SON_MIF_MODULE_ID, SONMRO_MODIFY_CONFIG_PARAM_RES,
            modify_res_len);

    SON_UT_TRACE_EXIT();
} /* son_mro_cell_fsm_modify_config_req_handler */

/*****************************************************************************
 * Function Name  : son_mro_cell_fsm_nack_enable_req_handler
 * Inputs         : evnt_id - Value of the event Id internal to MRO module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_mro_msg - Pointer to the message data received (after the
 *                  CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function to send nack for
 *                  SONMRO_ENABLE_REQ message received from SONMgmtIfH.
 ****************************************************************************/
static son_void_t
son_mro_cell_fsm_nack_enable_req_handler
(
 son_mro_cell_index_et  evnt_id,
 STACKAPIHDR            *p_cspl_hdr,
 mro_cell_context_t     *p_cell_ctxt,
 son_u8                 *p_mro_msg
 )
{
    son_mro_enable_req_t *p_req = (son_mro_enable_req_t *)p_mro_msg;
    son_mro_enable_res_t enable_res;
    son_size_t enable_res_len = sizeof(son_mro_enable_res_t);

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&enable_res, 0, enable_res_len);
    enable_res.transaction_id = p_req->transaction_id;
    enable_res.enable_res.result = SON_FAILURE;
    enable_res.enable_res.error_code = SON_NO_ERROR;
    enable_res.enable_res.cell_status_list_size = 1;
    SON_MEMCPY(&enable_res.enable_res.cell_status_list[0].cgi,
            &p_cell_ctxt->cell_id,
            sizeof(son_intra_rat_global_cell_id_t));
    if (MRO_CELL_STATE_ENABLING == p_cell_ctxt->current_cell_fsm_state)
    {
        enable_res.enable_res.cell_status_list[0].error_code = 
            SON_ERR_ENABLE_IN_PROGRESS;
    }

    if (MRO_CELL_STATE_ENABLED == p_cell_ctxt->current_cell_fsm_state)
    {
        enable_res.enable_res.cell_status_list[0].error_code = 
            SON_ERR_ALREADY_ENABLED;
    }

    son_create_send_buffer((son_u8 *)&enable_res,
            SON_MRO_MODULE_ID,
            SON_MIF_MODULE_ID, SONMRO_ENABLE_RES,
            enable_res_len);

                    /*SPR 17777 +-*/
    SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,SON_DETAILEDALL,"Unused variable evnt_id=%d p_cspl_hdr=%p",evnt_id,p_cspl_hdr);
                    /*SPR 17777 +-*/
    SON_UT_TRACE_EXIT();
} /* son_mro_cell_fsm_nack_enable_req_handler */

/*****************************************************************************
 * Function Name  : son_mro_cell_fsm_nack_disable_req_handler
 * Inputs         : evnt_id - Value of the event Id internal to MRO module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_mro_msg - Pointer to the message data received (after the
 *                  CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function to send nack for
 *                  SONMRO_DISABLE_REQ message received from SONMgmtIfH.
 ****************************************************************************/
static son_void_t
son_mro_cell_fsm_nack_disable_req_handler
(
 son_mro_cell_index_et  evnt_id,
 STACKAPIHDR            *p_cspl_hdr,
 mro_cell_context_t     *p_cell_ctxt,
 son_u8                 *p_mro_msg
 )
{
    son_mro_disable_req_t *p_req = (son_mro_disable_req_t *)p_mro_msg;
    son_mro_disable_res_t disable_res;
    son_size_t disable_res_len = sizeof(son_mro_disable_res_t);

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&disable_res, 0, disable_res_len);
    disable_res.transaction_id = p_req->transaction_id;
    disable_res.disable_res.result = SON_FAILURE;
    disable_res.disable_res.error_code = SON_NO_ERROR;
    disable_res.disable_res.cell_status_list_size = 1;
    SON_MEMCPY(&disable_res.disable_res.cell_status_list[0].cgi,
            &p_cell_ctxt->cell_id,
            sizeof(son_intra_rat_global_cell_id_t));
    if (MRO_CELL_STATE_DISABLING == p_cell_ctxt->current_cell_fsm_state)
    {
        disable_res.disable_res.cell_status_list[0].error_code = 
                   SON_ERR_DISABLE_IN_PROGRESS;
    }

    if (MRO_CELL_STATE_DISABLED == p_cell_ctxt->current_cell_fsm_state)
    {
        disable_res.disable_res.cell_status_list[0].error_code =
                    SON_ERR_ALREADY_DISABLED;
    }

    son_create_send_buffer((son_u8 *)&disable_res,
            SON_MRO_MODULE_ID,
            SON_MIF_MODULE_ID, SONMRO_DISABLE_RES,
            disable_res_len);
                    /*SPR 17777 +-*/
    SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,SON_DETAILEDALL,"Unused variable evnt_id=%d p_cspl_hdr=%p",evnt_id,p_cspl_hdr);
                    /*SPR 17777 +-*/
    SON_UT_TRACE_EXIT();
} /* son_mro_cell_fsm_nack_disable_req_handler */

/*****************************************************************************
 * Function Name  : son_mro_cell_fsm_invalid_event_handler
 * Inputs         : evnt_id - Value of the event Id internal to MRO module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_mro_msg - Pointer to the message data received (after the
 *                  CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for the messages received
 *                  in invalid states for which nothing needs to be done.
 ****************************************************************************/
static son_void_t
son_mro_cell_fsm_invalid_event_handler
(
    son_mro_cell_index_et  evnt_id,
    STACKAPIHDR            *p_cspl_hdr,
    mro_cell_context_t     *p_cell_ctxt,
    son_u8                 *p_mro_msg
 )
{
    SON_UT_TRACE_ENTER();

                    /*SPR 17777 +-*/
    SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, SON_WARNING,
            "son_mro_cell_fsm_invalid_event_handler: Invalid Event %s occured "
            "in %s state. evnt_id=%d p_cspl_hdr=%p p_mro_msg=%p",
            SON_MRO_CELL_FSM_EVENT_NAMES[evnt_id],
            SON_MRO_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
            evnt_id,p_cspl_hdr,p_mro_msg);
                    /*SPR 17777 +-*/

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mro_cell_fsm_state_change_ind_handler
 * Inputs         : evnt_id - Value of the event Id internal to MRO module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_mro_msg - Pointer to the message data received (after the
 *                  CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for the cell state change ind
 *                  message received from RRM.
 ****************************************************************************/
static son_void_t
son_mro_cell_fsm_state_change_ind_handler
(
    son_mro_cell_index_et  evnt_id,
    STACKAPIHDR            *p_cspl_hdr,
    mro_cell_context_t     *p_cell_ctxt,
    son_u8                 *p_mro_msg
)
{
    son_cell_state_change_indication_t *p_state_ind = 
        (son_cell_state_change_indication_t *)p_mro_msg;
    son_feature_state_et                state = SON_STATE_UNDEFINED;
    son_feature_state_change_cause_et   cause = 
        SON_STATE_CHANGE_CAUSE_UNDEFINED;

    SON_UT_TRACE_ENTER();
    SON_LOG(g_mro_gb_ctxt.log_mode,
            p_son_mro_facility_name, SON_BRIEF,
            "son_mro_cell_fsm_cell_state_change_ind_handler: "
            "RRM_SON_CELL_STATE_CHANGE_IND received:\n"
            "Cell Id: 0x%x",
            son_convert_char_cell_id_to_int_cell_id(p_state_ind->cell_id.cell_identity));
    if (RRM_CELL_DELETED == p_state_ind->cell_state)
    {
        state = SON_STATE_DELETED;
        cause = SON_RRM_CELL_UNCONFIGURED;

        mro_delete_cell_from_context((son_intra_rat_global_cell_id_t *)
                &p_state_ind->cell_id);
    }
    else if (RRM_CELL_UNOPERATIONAL == p_state_ind->cell_state)
    {
        /* PR 551 Fix Start */
        state = SON_STATE_ENABLED;
        /* PR 551 Fix End */
        cause = SON_RRM_CELL_UNOPERATIONAL;
        /* PR 551 Fix Start */
        SONMRO_CELL_FSM_SET_STATE(p_cell_ctxt,
                p_cell_ctxt->previous_cell_fsm_state,
                p_cell_ctxt->current_cell_fsm_state,
                MRO_CELL_STATE_ENABLED);
        /* PR 551 Fix End */
    }
    /* PR 551 Fix Start */
    else if (RRM_CELL_OPERATIONAL == p_state_ind->cell_state)
    {
        state = SON_STATE_ENABLED;
        cause = SON_RRM_CELL_OPERATIONAL;
    }
    /* PR 551 Fix End */
    else
    {
                    /*SPR 17777 +-*/
        SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                SON_ERROR,
                "son_mro_cell_fsm_state_change_ind_handler: Invalid Cell state[%d]"
                "received from RRM; evnt_id=%d p_cspl_hdr=%p"
                , p_state_ind->cell_state,evnt_id,p_cspl_hdr);
                    /*SPR 17777 +-*/
    }   

    if (SON_STATE_UNDEFINED != state)
    {
        if (mro_get_pending_res_expected_count())
        {
            mro_delete_cell_from_pending_res
                ((son_intra_rat_global_cell_id_t *)&p_state_ind->cell_id);

            /* Decrement expected_response_count by 1 */
            mro_decrement_pending_res_expected_count();

            if (0 == mro_get_pending_res_expected_count())
            {
                son_procedure_code_et api = SON_PROCEDURE_UNDEFINED;
                api = mro_get_pending_res_api_id();
                g_mro_gb_ctxt.current_fsm_state = MRO_STATE_ACTIVE;
                if (SONMRO_ENABLE_RES == api)
                {
                    son_mro_enable_res_t res;
                    son_size_t enable_res_len = sizeof(son_mro_enable_res_t);
                    SON_MEMSET(&res, 0, enable_res_len);
                    res.transaction_id = mro_get_pending_res_transaction_id();
                    res.enable_res.error_code = SON_NO_ERROR;

                    /* Remove Successful cells from the
                     * MRO_Pending_Response_LIST and create cell array with
                     * entry for each cell for which MRO could not be
                     * enabled. Last argument is not required
                     * as the cell FSM state is already set above */
                    mro_construct_cell_arr_from_pending_res_list(
                            &res.enable_res.cell_status_list_size,
                            res.enable_res.cell_status_list,
                            MRO_CELL_NUM_OF_STATES);
                    if (0 != res.enable_res.cell_status_list_size)
                    {
                        res.enable_res.result = SON_FAILURE;
                    }
                    else
                    {
                        res.enable_res.result = SON_SUCCESS;
                    }

                    son_create_send_buffer((son_u8 *)&res,
                            SON_MRO_MODULE_ID,
                            SON_MIF_MODULE_ID,
                            SONMRO_ENABLE_RES,
                            enable_res_len);
                }
                else if (SONMRO_DISABLE_RES == api)
                {
                    son_mro_disable_res_t res;
                    son_size_t disable_res_len = sizeof(son_mro_disable_res_t);
                    SON_MEMSET(&res, 0, disable_res_len);
                    res.transaction_id = mro_get_pending_res_transaction_id();
                    res.disable_res.error_code = SON_NO_ERROR;

                    /* Remove Successful cells from the
                     * MRO_Pending_Response_LIST and create cell array with
                     * entry for each cell for which MRO could not be
                     * disabled. Last argument is not required
                     * as the cell FSM state is already set above */
                    mro_construct_cell_arr_from_pending_res_list(
                            &res.disable_res.cell_status_list_size,
                            res.disable_res.cell_status_list,
                            MRO_CELL_NUM_OF_STATES);
                    if (0 != res.disable_res.cell_status_list_size)
                    {
                        res.disable_res.result = SON_FAILURE;
                    }
                    else
                    {
                        res.disable_res.result = SON_SUCCESS;
                    }

                    son_create_send_buffer((son_u8 *)&res,
                            SON_MRO_MODULE_ID,
                            SON_MIF_MODULE_ID,
                            SONMRO_DISABLE_RES,
                            disable_res_len);
                  }

                  mro_reset_pending_resp_list();
            }
        }

        mro_send_feature_state_change_ind(
            (son_intra_rat_global_cell_id_t *)&p_state_ind->cell_id,
            state, cause);
    }

    SON_UT_TRACE_EXIT();
} /* son_mro_cell_fsm_state_change_ind_handler */

/*****************************************************************************
 * Function Name  : son_mro_cell_fsm_anr_mode_ind_handler
 * Inputs         : evnt_id - Value of the event Id internal to MRO module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_mro_msg - Pointer to the message data received (after the
 *                  CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for the ANR mode change 
 *                  indication message received from ANR. ANR sends this
 *                  indication whenever ANR mode is disabled for a cell.
 ****************************************************************************/
static son_void_t
son_mro_cell_fsm_anr_mode_ind_handler
(
    son_mro_cell_index_et  evnt_id,
    STACKAPIHDR            *p_cspl_hdr,
    mro_cell_context_t     *p_cell_ctxt,
    son_u8                 *p_mro_msg
)    
{
    son_anr_cell_mode_ind_t *p_anr_mode_ind = 
        (son_anr_cell_mode_ind_t *)p_mro_msg;

    SON_UT_TRACE_ENTER();

    SON_LOG(g_mro_gb_ctxt.log_mode,
            p_son_mro_facility_name, SON_BRIEF,
            "son_mro_cell_fsm_anr_mode_ind_handler: "
            "SONANR_DISABLE_IND received:\n"
            "Cell Id: 0x%x",
            son_convert_char_cell_id_to_int_cell_id(p_anr_mode_ind->srv_cgi.cell_identity));

    if (ANR_MODE_DISABLED == p_anr_mode_ind->cell_mode)
    {
        mro_send_feature_state_change_ind(&p_anr_mode_ind->srv_cgi,
                SON_STATE_DISABLED, SON_ANR_MODE_DISABLED);

        if (SON_SUCCESS != mro_construct_send_rrm_deregister_req(
                    son_generate_txn_id(),
                    &p_anr_mode_ind->srv_cgi))
        {
            SON_LOG(g_mro_gb_ctxt.log_mode,
                    p_son_mro_facility_name, SON_ERROR,
                    "son_mro_cell_fsm_anr_mode_ind_handler: "
                    "Failed to send rrm_deregister_req on receiving "
                    "SONANR_DISABLE_IND from ANR for cell [Cell Id: 0x%x]",
                    son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
            SONMRO_CELL_FSM_SET_STATE(p_cell_ctxt,
                    p_cell_ctxt->previous_cell_fsm_state,
                    p_cell_ctxt->current_cell_fsm_state,
                    MRO_CELL_STATE_DISABLED);
        }
        else
        {    
            p_cell_ctxt->dereg_invoking_api = SONANR_DISABLE_IND;
            SONMRO_CELL_FSM_SET_STATE(p_cell_ctxt,
                    p_cell_ctxt->previous_cell_fsm_state,
                    p_cell_ctxt->current_cell_fsm_state,
                    MRO_CELL_STATE_DISABLING);
        }
    } 
    else
    {
                    /*SPR 17777 +-*/
        SON_LOG(g_mro_gb_ctxt.log_mode,
            p_son_mro_facility_name, SON_ERROR, 
            "son_mro_cell_fsm_anr_mode_ind_handler: "
            "SONANR_DISABLE_IND received with invalid cell mode[%d] for "
            "Cell Id: 0x%x; evnt_id=%d p_cspl_hdr=%p",
            p_anr_mode_ind->cell_mode,
            son_convert_char_cell_id_to_int_cell_id(p_anr_mode_ind->srv_cgi.cell_identity),
            evnt_id,p_cspl_hdr);
                    /*SPR 17777 +-*/
    }        

    SON_UT_TRACE_EXIT();
} /* son_mro_cell_fsm_anr_mode_ind_handler */

/*****************************************************************************
 * Function Name  : son_mro_cell_fsm_deregister_resp_handler
 * Inputs         : evnt_id - Value of the event Id internal to MRO module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_mro_msg - Pointer to the message data received (after the
 *                  CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for the deregistraion resp
 *                  message received from RRM.
 ****************************************************************************/
static son_void_t
son_mro_cell_fsm_deregister_resp_handler
(
 son_mro_cell_index_et  evnt_id,
 STACKAPIHDR            *p_cspl_hdr,
 mro_cell_context_t     *p_cell_ctxt,
 son_u8                 *p_mro_msg
 )
{
    son_return_et result = SON_FAILURE;
    son_rrm_deregistration_res_t *p_res = 
        (son_rrm_deregistration_res_t *)p_mro_msg;
    son_mro_disable_res_t res;
    son_error_et error_code = SON_NO_ERROR;
    son_size_t disable_res_len = sizeof(son_mro_disable_res_t);

    SON_UT_TRACE_ENTER();

    result = ((p_res->res.deregistration_resp.result == RRM_SUCCESS) ?
            SON_SUCCESS : SON_FAILURE);
    if (SONANR_DISABLE_IND == p_cell_ctxt->dereg_invoking_api)
    {
        p_cell_ctxt->dereg_invoking_api = SON_PROCEDURE_UNDEFINED;
                    /*SPR 17777 +-*/
        SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                SON_BRIEF,
                "son_mro_cell_fsm_deregister_resp_handler: Deregister "
                "response received for MRO generated RRM_SON_REGISTER_REQ "
                "with result = %u for cell [Cell Id: 0x%x] evnt_id=%d p_cspl_hdr=%p",
                result, son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
                evnt_id,p_cspl_hdr);
                    /*SPR 17777 +-*/

        /* Change cell FSM state to MRO_CELL_STATE_DISABLED */
        SONMRO_CELL_FSM_SET_STATE(p_cell_ctxt,
                p_cell_ctxt->previous_cell_fsm_state,
                p_cell_ctxt->current_cell_fsm_state,
                MRO_CELL_STATE_DISABLED);

        SON_UT_TRACE_EXIT();
        return;
    }

    if (SON_FAILURE == result)
    {
        SONMRO_CELL_FSM_SET_STATE(p_cell_ctxt,
                p_cell_ctxt->previous_cell_fsm_state,
                p_cell_ctxt->current_cell_fsm_state,
                MRO_CELL_STATE_ENABLED);
        error_code = anr_map_rrm_error_codes(
                __func__,
                result, p_res->res.deregistration_resp.error_code,
                RRM_SON_DEREGISTER_RESP);
    }
    else
    {
        /* Change cell FSM state to MRO_CELL_STATE_DISABLED */
        SONMRO_CELL_FSM_SET_STATE(p_cell_ctxt,
                p_cell_ctxt->previous_cell_fsm_state,
                p_cell_ctxt->current_cell_fsm_state,
                MRO_CELL_STATE_DISABLED);
        /* Stop HO Failure Analysis timer */
        if (SON_TRUE == son_is_timer_running(
                p_cell_ctxt->ho_failure_accumulation_duration_timer_id))
        {
            son_stop_timer(p_cell_ctxt->ho_failure_accumulation_duration_timer_id);
            p_cell_ctxt->ho_failure_accumulation_duration_timer_id = SON_PNULL;
        }
    }

    if (p_res->transaction_id == mro_get_pending_res_transaction_id())
    {
        cell_status_list_data_t *p_list_node = SON_PNULL;
        /* Search for the cell Id in pending response structure. */
        p_list_node = mro_find_cell_in_pending_res(&p_cell_ctxt->cell_id);

        /* Presence indicates that cell state is disabling due to Global
         * Disable Request */
        if (SON_PNULL != p_list_node)
        {
            /* Update Cell in MRO Pending Response List with error code
             * determined above */
            p_list_node->cell_status.error_code = error_code;
            /* Decrement expected_response_count by 1 */
            mro_decrement_pending_res_expected_count();
            /* Check if expected response count == 0 */
            if (0 == mro_get_pending_res_expected_count())
            {
                SON_MEMSET(&res, 0, disable_res_len);
                res.transaction_id = p_res->transaction_id;
                res.disable_res.error_code = SON_NO_ERROR;

                /* Remove Successful cells from the
                 * MRO_Pending_Response_LIST and create cell array with
                 * entry for each cell for which MRO could not be
                 * enabled/disabled. Last argument is not required
                 * as the cell FSM state is already set above */
                mro_construct_cell_arr_from_pending_res_list(
                        &res.disable_res.cell_status_list_size,
                        res.disable_res.cell_status_list,
                        MRO_CELL_NUM_OF_STATES);                        
                if (0 != res.disable_res.cell_status_list_size)
                {
                    res.disable_res.result = SON_FAILURE;
                }
                else
                {
                    res.disable_res.result = SON_SUCCESS;
                }

                son_create_send_buffer((son_u8 *)&res,
                        SON_MRO_MODULE_ID,
                        SON_MIF_MODULE_ID,
                        SONMRO_DISABLE_RES,
                        disable_res_len);
                /* Set MRO global FSM state to ACTIVE */
                SONMRO_FSM_SET_STATE(g_mro_gb_ctxt.current_fsm_state,
                        MRO_STATE_ACTIVE);
                /* Reset MRO_Pending_Response_LIST */
                mro_reset_pending_resp_list();
                /* Stop the MRO_PENDING_RESPONSE_TIMER if running */
                if (SON_TRUE == son_is_timer_running(
                            g_mro_gb_ctxt.mro_pending_res_timer_id))
                {
                    son_stop_timer(g_mro_gb_ctxt.mro_pending_res_timer_id);
                    g_mro_gb_ctxt.mro_pending_res_timer_id = SON_PNULL;
                }
            }
        }
    }
    else
    {
        /* Send SONMRO_DISABLE_RES to SONMgmtIfH */
        SON_MEMSET(&res, 0, disable_res_len);
        res.transaction_id = p_res->transaction_id;
        res.disable_res.result = result;
        res.disable_res.error_code = SON_NO_ERROR;
        if (SON_FAILURE == result)
        {
            res.disable_res.cell_status_list_size = 1;
            SON_MEMCPY(&res.disable_res.cell_status_list[0].cgi,
                    &p_cell_ctxt->cell_id,
                    sizeof(son_intra_rat_global_cell_id_t));
            res.disable_res.cell_status_list[0].error_code = error_code;
        }

        son_create_send_buffer((son_u8 *)&res,
                SON_MRO_MODULE_ID,
                SON_MIF_MODULE_ID, SONMRO_DISABLE_RES,
                disable_res_len);
    }

    if ((RRM_ERR_OBJECT_NON_EXISTENT == 
                p_res->res.deregistration_resp.error_code) ||
            (RRM_ERR_CELL_UNCONFIGURED == \
             p_res->res.deregistration_resp.error_code))
    {
        mro_delete_cell_from_context(&p_cell_ctxt->cell_id);
    }

    SON_UT_TRACE_EXIT();
} /* son_mro_cell_fsm_deregister_resp_handler */

/*****************************************************************************
 * Function Name  : son_mro_cell_fsm_mlb_nbr_cell_status_ind
 * Inputs         : p_mro_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  evnt_id - Value of the event Id internal to MRO module
 *                  p_cell_ctxt - Pointer to the cell context
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the MLB Neighbor cell
 *                  status indication message received from SON MLB.
 ****************************************************************************/
static son_void_t
son_mro_cell_fsm_mlb_nbr_cell_status_ind
(
    son_mro_cell_index_et evnt_id,
    STACKAPIHDR            *p_cspl_hdr,
    mro_cell_context_t     *p_cell_ctxt,
    son_u8                 *p_mro_msg
)
{
    mlb_nbr_cell_status_ind_t *p_ind =
        (mlb_nbr_cell_status_ind_t *)p_mro_msg;
    anr_cell_context_t *p_anr_cell_ctxt = SON_PNULL;
    son_error_et error_code = SON_NO_ERROR;
    son_rat_type_et rat_type = SON_EUTRAN_CELL;
    anr_intra_rat_hash_nr_tuple_t *p_eutran_nbr = SON_PNULL;
    son_global_cell_id_t trgt_glb_cell_id;

    SON_UT_TRACE_ENTER();

    p_anr_cell_ctxt = anr_get_cell_context_from_global_context(
        &p_cell_ctxt->cell_id);
    if (SON_PNULL != p_anr_cell_ctxt)
    {
        SON_MEMSET(&trgt_glb_cell_id, 0, sizeof(son_global_cell_id_t));
        trgt_glb_cell_id.bitmask = SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;
        SON_MEMCPY(&trgt_glb_cell_id.intra_rat_global_cell_id,
            &p_ind->nbr_cell_id,
            sizeof(son_intra_rat_global_cell_id_t));
        p_eutran_nbr = (anr_intra_rat_hash_nr_tuple_t *)anr_find_nr(
            p_anr_cell_ctxt, &trgt_glb_cell_id, &error_code, &rat_type);
        if (SON_PNULL != p_eutran_nbr)
        {
            if (MLB_NBR_STATE_ENABLED == p_ind->nbr_cell_state)
            {
                p_eutran_nbr->data.mro_data.in_use_by_mlb = SON_TRUE;
            }
            else if (MLB_NBR_STATE_DISABLED == p_ind->nbr_cell_state)
            {
                p_eutran_nbr->data.mro_data.in_use_by_mlb = SON_FALSE;
            }
        }
    }
                    /*SPR 17777 +-*/
    SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,SON_DETAILEDALL,"Unused variable evnt_id=%d p_cspl_hdr=%p",evnt_id,p_cspl_hdr);
                    /*SPR 17777 +-*/
    SON_UT_TRACE_EXIT();
} /* son_mro_cell_fsm_mlb_nbr_cell_status_ind */

/*****************************************************************************
 * Function Name  : son_mro_cell_fsm_ho_report_handler
 * Inputs         : evnt_id - Value of the event Id internal to MRO module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_mro_msg - Pointer to the message data received (after the
 *                  CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for the HO report
 *                  message received from RRM.
 ****************************************************************************/
static son_void_t
son_mro_cell_fsm_ho_report_handler
(
 son_mro_cell_index_et  evnt_id,
 STACKAPIHDR            *p_cspl_hdr,
 mro_cell_context_t     *p_cell_ctxt,
 son_u8                 *p_mro_msg
 )
{
    son_ho_failure_report_t *p_ho_report = 
        (son_ho_failure_report_t *)p_mro_msg;
    anr_cell_context_t *p_anr_cell_ctxt = SON_PNULL;
    son_error_et error_code = SON_NO_ERROR;
    son_rat_type_et rat_type = SON_EUTRAN_CELL;
    anr_intra_rat_hash_nr_tuple_t *p_eutran_nbr = SON_PNULL;
    son_global_cell_id_t trgt_glb_cell_id;

    SON_UT_TRACE_ENTER();

    p_anr_cell_ctxt = anr_get_cell_context_from_global_context(
            &p_cell_ctxt->cell_id);
    if (SON_PNULL != p_anr_cell_ctxt)
    {
        SON_MEMSET(&trgt_glb_cell_id, 0, sizeof(son_global_cell_id_t));
        trgt_glb_cell_id.bitmask = SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;
        SON_MEMCPY(&trgt_glb_cell_id.intra_rat_global_cell_id,
                &p_ho_report->trgt_cgi,
                sizeof(son_intra_rat_global_cell_id_t));
        p_eutran_nbr = (anr_intra_rat_hash_nr_tuple_t *)anr_find_nr(
                p_anr_cell_ctxt, &trgt_glb_cell_id, &error_code, &rat_type);
        if (SON_PNULL != p_eutran_nbr)
        {    
            if (RRM_HO_TOO_EARLY == p_ho_report->ho_cause)
            {
                p_eutran_nbr->data.mro_data.mro_ho_counter.num_too_early_ho++;
            }
            else if (RRM_HO_TOO_LATE == p_ho_report->ho_cause)
            {
                p_eutran_nbr->data.mro_data.mro_ho_counter.num_too_late_ho++;
            }
            else if (HO_FAILURE_REPORT_RE_EST_CGI_PRESENT &
                p_ho_report->bitmask)
            {
                p_eutran_nbr->data.mro_data.mro_ho_counter.
                    num_ho_to_wrong_cell_for_failure_cell++;
            }

            anr_print_eutran_nbr_mro_info(&p_eutran_nbr->data);
        }
        else if (SON_PNULL == p_eutran_nbr)
        {
            SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                    SON_ERROR,
                    "son_mro_cell_fsm_ho_report_handler: "
                    "Neighbour Not Found with failure cell [Cell Id: 0x%x]",
                    son_convert_char_cell_id_to_int_cell_id(p_ho_report->trgt_cgi.cell_identity));
        }
        else
        {
            SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                SON_BRIEF,
                "son_mro_cell_fsm_ho_report_handler: "
                "Neighbour with failure cell [Cell Id: 0x%x] is in "
                "use by MLB",
                son_convert_char_cell_id_to_int_cell_id(p_ho_report->trgt_cgi.cell_identity));
        }

        if (HO_FAILURE_REPORT_RE_EST_CGI_PRESENT &
                p_ho_report->bitmask)
        {
            /* Find neighbor with re-establishment cell Id */
            SON_MEMCPY(&trgt_glb_cell_id.intra_rat_global_cell_id,
                    &p_ho_report->restablishment_cgi,
                    sizeof(son_intra_rat_global_cell_id_t));
            p_eutran_nbr = (anr_intra_rat_hash_nr_tuple_t *)anr_find_nr(
                    p_anr_cell_ctxt, &trgt_glb_cell_id, &error_code, 
                    &rat_type);
            if (SON_PNULL != p_eutran_nbr)
            {
                p_eutran_nbr->data.mro_data.mro_ho_counter.
                    num_ho_to_wrong_cell_for_reestablishment_cell++;
                p_eutran_nbr->data.mro_data.mro_ho_counter.total_ho_attempted++;
                anr_print_eutran_nbr_mro_info(&p_eutran_nbr->data);
            }
            else if (SON_PNULL == p_eutran_nbr)
            {
                SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                        SON_ERROR,
                        "son_mro_cell_fsm_ho_report_handler: "
                        "Neighbour Not Found with re-establishment cell "
                        "[Cell Id: 0x%x]",
                        son_convert_char_cell_id_to_int_cell_id(p_ho_report->restablishment_cgi.
                            cell_identity));
            }
    else
    {
        SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                    SON_BRIEF,
                    "son_mro_cell_fsm_ho_report_handler: "
                    "Neighbour with re-establishment cell "
                    "[Cell Id: 0x%x] is in use by MLB",
                    son_convert_char_cell_id_to_int_cell_id(p_ho_report->restablishment_cgi.
                        cell_identity));
            }
        }
    }
    else
    {
                    /*SPR 17777 +-*/
        SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
            SON_ERROR,
            "son_mro_cell_fsm_ho_report_handler: "
            "ANR context not found for source cell [Cell Id: 0x%x] evnt_id=%d p_cspl_hdr=%p",
            son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),evnt_id,p_cspl_hdr);
                    /*SPR 17777 +-*/
    }

    SON_UT_TRACE_EXIT();
} /* son_mro_cell_fsm_ho_report_handler */

/*****************************************************************************
 * Function Name  : son_mro_cell_fsm_ho_attempts_ind_handler
 * Inputs         : evnt_id - Value of the event Id internal to MRO module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_mro_msg - Pointer to the message data received (after the
 *                  CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for the HO attempts
 *                  message received from RRM.
 ****************************************************************************/
static son_void_t
son_mro_cell_fsm_ho_attempts_ind_handler
(
 son_mro_cell_index_et  evnt_id,
 STACKAPIHDR            *p_cspl_hdr,
 mro_cell_context_t     *p_cell_ctxt,
 son_u8                 *p_mro_msg
 )
{
    son_mro_ho_attempts_ind_t *p_ho_attempts_ind = 
        (son_mro_ho_attempts_ind_t *)p_mro_msg;
    anr_cell_context_t *p_anr_cell_ctxt = SON_PNULL;
    son_error_et error_code = SON_NO_ERROR;
    son_rat_type_et rat_type = SON_EUTRAN_CELL;
    anr_intra_rat_hash_nr_tuple_t *p_eutran_nbr = SON_PNULL;
    son_global_cell_id_t trgt_glb_cell_id;

    SON_UT_TRACE_ENTER();
    
    /* Note: This count will be set to 1 in present release by RRM */
    if (1 != p_ho_attempts_ind->attempt_count)        
    {
        SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                SON_ERROR,
                "son_mro_cell_fsm_ho_attempts_ind_handler: "
                "RRM_SON_HO_ATTEMPT_IND message received from RRM for "
                "neighbour cell [Cell Id: 0x%x] with invalid attempt count "
                "= %u",
                son_convert_char_cell_id_to_int_cell_id(p_ho_attempts_ind->trgt_cgi.cell_identity),
                p_ho_attempts_ind->attempt_count);
        SON_UT_TRACE_EXIT();
        return;
    }

    p_anr_cell_ctxt = anr_get_cell_context_from_global_context(
            &p_cell_ctxt->cell_id);
    if (SON_PNULL != p_anr_cell_ctxt)
    {
        SON_MEMSET(&trgt_glb_cell_id, 0, sizeof(son_global_cell_id_t));
        trgt_glb_cell_id.bitmask = SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;
        SON_MEMCPY(&trgt_glb_cell_id.intra_rat_global_cell_id,
                &p_ho_attempts_ind->trgt_cgi,
                sizeof(son_intra_rat_global_cell_id_t));
        p_eutran_nbr = (anr_intra_rat_hash_nr_tuple_t *)anr_find_nr(
                p_anr_cell_ctxt, &trgt_glb_cell_id, &error_code, &rat_type);
        if (SON_PNULL != p_eutran_nbr)
        {    
            p_eutran_nbr->data.mro_data.mro_ho_counter.total_ho_attempted +=
                p_ho_attempts_ind->attempt_count;
            anr_print_eutran_nbr_mro_info(&p_eutran_nbr->data);
        }
        else if (SON_PNULL == p_eutran_nbr)
        {
            SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                    SON_ERROR,
                    "son_mro_cell_fsm_ho_attempts_ind_handler: "
                    "Neighbour Not Found with cell [Cell Id: 0x%x]",
                    son_convert_char_cell_id_to_int_cell_id(p_ho_attempts_ind->trgt_cgi.cell_identity));
        }    
    else
    {
        SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                SON_BRIEF,
                "son_mro_cell_fsm_ho_attempts_ind_handler: "
                "Neighbour with cell [Cell Id: 0x%x] is in use by MLB",
                son_convert_char_cell_id_to_int_cell_id(p_ho_attempts_ind->trgt_cgi.cell_identity));
    }   
    }
    else
    {
                    /*SPR 17777 +-*/
        SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
            SON_ERROR,
            "son_mro_cell_fsm_ho_attempts_ind_handler: "
            "ANR context not found for source cell [Cell Id: 0x%x] evnt_id=%d p_cspl_hdr=%p",
            son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
            evnt_id,p_cspl_hdr);
                    /*SPR 17777 +-*/
    }

    SON_UT_TRACE_EXIT();
} /* son_mro_cell_fsm_ho_attempts_ind_handler */

/*****************************************************************************
 * Function Name  : son_mro_cell_fsm_register_resp_handler
 * Inputs         : evnt_id - Value of the event Id internal to MRO module
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_cell_ctxt - Pointer to the cell context
 *                  p_mro_msg - Pointer to the message data received (after the
 *                  CSPL header)
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the handler function for RRM_SON_REGISTER_RESP
 *                  message received from RRM.
 ****************************************************************************/
static son_void_t
son_mro_cell_fsm_register_resp_handler
(
    son_mro_cell_index_et  evnt_id,
    STACKAPIHDR            *p_cspl_hdr,
    mro_cell_context_t     *p_cell_ctxt,
    son_u8                 *p_mro_msg
)    
{
    son_procedure_code_et api = SON_PROCEDURE_UNDEFINED;
    son_return_et result = SON_FAILURE;
    son_rrm_registration_res_t *p_res = (son_rrm_registration_res_t *)p_mro_msg;
    son_mro_enable_res_t res;
    son_error_et error_code = SON_NO_ERROR;
    son_size_t enable_res_len = sizeof(son_mro_enable_res_t);
    son_feature_state_et state = SON_STATE_DISABLED;
    son_feature_state_change_cause_et cause = SON_RRM_REGISTRATION_UNSUCCESSFUL;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&res, 0, enable_res_len);
    result = ((p_res->res.registration_resp.result == RRM_SUCCESS) ? 
            SON_SUCCESS : SON_FAILURE);
    if (SON_FAILURE == result)
    {
        SONMRO_CELL_FSM_SET_STATE(p_cell_ctxt,
                p_cell_ctxt->previous_cell_fsm_state,
                p_cell_ctxt->current_cell_fsm_state,
                MRO_CELL_STATE_DISABLED);
        error_code = anr_map_rrm_error_codes(
            __func__, 
            result, p_res->res.registration_resp.error_code, 
            RRM_SON_REGISTER_RESP);
    }
    else
    {
        anr_cell_context_t *p_anr_cell_ctxt = SON_PNULL;
        p_anr_cell_ctxt = anr_get_cell_context_from_global_context(
                &p_cell_ctxt->cell_id);
        if (SON_PNULL != p_anr_cell_ctxt && 
                (ANR_CELL_STATE_ENABLED == (
                anr_get_anr_mode_for_cell(p_anr_cell_ctxt))))
        {
            /* Change cell FSM state to MRO_CELL_STATE_ENABLED */
            SONMRO_CELL_FSM_SET_STATE(p_cell_ctxt,
                    p_cell_ctxt->previous_cell_fsm_state,
                    p_cell_ctxt->current_cell_fsm_state,
                    MRO_CELL_STATE_ENABLED);
            /* Start HO Failure Analysis timer */
            p_cell_ctxt->ho_failure_accumulation_duration_timer_id = 
                /* SPR-12708 Fix Starts */
                son_start_timer(
                        (p_cell_ctxt->mro_config_param.
                         mro_ho_failure_accumulation_duration * 
                         SON_SECONDS_IN_A_MIN),
                        &p_cell_ctxt->cell_id, sizeof(
                            son_intra_rat_global_cell_id_t), SON_TRUE);
            /* SPR-12708 Fix Ends */
            state = SON_STATE_ENABLED;
            cause = SON_RRM_REGISTRATION_SUCCESSFUL;
        }
        else
        {
            if (SON_PNULL == p_anr_cell_ctxt)
            {
                error_code = SON_ERR_CELL_UNCONFIGURED_AT_ANR; 
                cause = SON_ANR_CELL_UNCONFIGURED;
            }
            else
            {
                error_code = SON_ERR_ANR_MODE_IS_DISABLED; 
                cause = SON_ANR_MODE_DISABLED;
            }

            SONMRO_CELL_FSM_SET_STATE(p_cell_ctxt,
                    p_cell_ctxt->previous_cell_fsm_state,
                    p_cell_ctxt->current_cell_fsm_state,
                    MRO_CELL_STATE_DISABLING);
            if (SON_SUCCESS == mro_construct_send_rrm_deregister_req(
                        p_res->transaction_id, 
                        &p_cell_ctxt->cell_id))
            {
                p_cell_ctxt->dereg_invoking_api = SONANR_DISABLE_IND;
            }
        }
    }

    if (p_res->transaction_id == mro_get_pending_res_transaction_id())
    {
        cell_status_list_data_t *p_list_node = SON_PNULL;
        /* Search for the cell Id in pending response structure. */
        p_list_node = mro_find_cell_in_pending_res(&p_cell_ctxt->cell_id);

        /* Presence indicates that cell state is enabling due to Global 
         * Enable Request or Cell Info Indication and no immediate Enable
         * Response must be sent from cell FSM */
        if (SON_PNULL != p_list_node)
        {
            api = mro_get_pending_res_api_id();

            /* Update Cell in MRO Pending Response List with error code
             *  determined above */
            p_list_node->cell_status.error_code = error_code;
            /* Decrement expected_response_count by 1 */
            mro_decrement_pending_res_expected_count();

            /* Check if expected response count == 0 */
            if (0 == mro_get_pending_res_expected_count())
            {
                if (SONMRO_ENABLE_RES == api)
                {
                    res.transaction_id = p_res->transaction_id;
                    res.enable_res.error_code = SON_NO_ERROR;

                    /* Remove Successful cells from the 
                     *  MRO_Pending_Response_LIST and create cell array with
                     *  entry for each cell for which MRO could not be
                     *  enabled. Last argument is not required
                     *  as the cell FSM state is already set above */
                    mro_construct_cell_arr_from_pending_res_list(
                            &res.enable_res.cell_status_list_size,
                            res.enable_res.cell_status_list, 
                            MRO_CELL_NUM_OF_STATES);
                    if (0 != res.enable_res.cell_status_list_size) 
                    {
                        res.enable_res.result = SON_FAILURE;
                    }
                    else
                    {
                        res.enable_res.result = SON_SUCCESS;
                    }

                    son_create_send_buffer((son_u8 *)&res,
                            SON_MRO_MODULE_ID,
                            SON_MIF_MODULE_ID,
                            SONMRO_ENABLE_RES,
                            enable_res_len);
                }
                else
                {
                    /* Remove cell from the MRO_Pending_Response_LIST */
                    mro_delete_cell_from_pending_res(&p_cell_ctxt->cell_id);
                }

                /* Set MRO global FSM state to ACTIVE */
                SONMRO_FSM_SET_STATE(g_mro_gb_ctxt.current_fsm_state,
                        MRO_STATE_ACTIVE);
                /* Reset MRO_Pending_Response_LIST */
                mro_reset_pending_resp_list();
                /* Stop the MRO_PENDING_RESPONSE_TIMER if running */
                if (SON_TRUE == son_is_timer_running(
                    g_mro_gb_ctxt.mro_pending_res_timer_id))
                {
                    son_stop_timer(g_mro_gb_ctxt.mro_pending_res_timer_id);
                    g_mro_gb_ctxt.mro_pending_res_timer_id = SON_PNULL;
                }
            }
        }
    }
    else
    {
        /* Send SONMRO_ENABLE_RES to SONMgmtIfH */
        res.transaction_id = p_res->transaction_id;
        res.enable_res.result = result;
        res.enable_res.error_code = SON_NO_ERROR;
        if (SON_NO_ERROR != error_code)
        {
            res.enable_res.result = SON_FAILURE;
            res.enable_res.cell_status_list_size = 1;
            SON_MEMCPY(&res.enable_res.cell_status_list[0].cgi, 
                &p_cell_ctxt->cell_id,
                sizeof(son_intra_rat_global_cell_id_t));
            res.enable_res.cell_status_list[0].error_code = error_code;
        }

        son_create_send_buffer((son_u8 *)&res,
                SON_MRO_MODULE_ID,
                SON_MIF_MODULE_ID, SONMRO_ENABLE_RES,
                enable_res_len);
    }

    if ((RRM_ERR_OBJECT_NON_EXISTENT == 
                p_res->res.registration_resp.error_code) ||
            (RRM_ERR_CELL_UNCONFIGURED == 
             p_res->res.registration_resp.error_code))
    {
        state = SON_STATE_DELETED;
        cause = SON_RRM_CELL_UNCONFIGURED;
    }

    if (SMIF_CELL_INFO_IND == api)
    {
        /* Send feature state change indication if received 
         * registration response is for the request triggered due
         * to Cell Info Indication request received by MRO */
        mro_send_feature_state_change_ind(&p_cell_ctxt->cell_id,
                state, cause);
        mro_delete_cell_from_pending_res(&p_cell_ctxt->cell_id);         
    }

    if ((RRM_ERR_OBJECT_NON_EXISTENT == 
                p_res->res.registration_resp.error_code) ||
            (RRM_ERR_CELL_UNCONFIGURED == 
             p_res->res.registration_resp.error_code))
    {
        mro_delete_cell_from_context(&p_cell_ctxt->cell_id);
    }
                    /*SPR 17777 +-*/
    SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,SON_DETAILEDALL,"Unused variable evnt_id=%d p_cspl_hdr=%p",evnt_id,p_cspl_hdr);
                    /*SPR 17777 +-*/
    SON_UT_TRACE_EXIT();
} /* son_mro_cell_fsm_register_resp_handler */

/************************* MRO Cell State Machine*********************/
typedef son_void_t (*mro_fsm_evt_handler_t)
(
    son_mro_cell_index_et evnt_id,
    STACKAPIHDR *p_cspl_hdr,
    mro_cell_context_t *p_cell_ctxt,
    son_u8 *p_mro_msg
);

/***********************************************************************
 * MRO Cell FSM Entry Point Table
 **********************************************************************/
static mro_fsm_evt_handler_t
son_mro_cell_fsm_table[MRO_CELL_NUM_OF_STATES][MRO_CELL_NUM_OF_EVENTS] =
{
    /* State MRO_CELL_STATE_DISABLED */
    {
        son_mro_cell_fsm_enable_req_handler,
        /* MRO_CELL_IDX_SONMRO_EVENT_ENABLE_REQ */
        son_mro_cell_fsm_nack_disable_req_handler,
        /* MRO_CELL_IDX_SONRMO_EVENT_DISABLE_REQ */
        son_mro_cell_fsm_modify_config_req_handler,
        /* MRO_CELL_IDX_SONMRO_EVENT_MODIFY_CONFIG_REQ */
        son_mro_cell_fsm_invalid_event_handler,
        /* MRO_CELL_IDX_RRM_EVENT_REGISTER_RES */
        son_mro_cell_fsm_invalid_event_handler,
         /* MRO_CELL_IDX_RRM_EVENT_DEREGISTER_RES */
        son_mro_cell_fsm_invalid_event_handler,
        /* MRO_CELL_IDX_RRM_EVENT_HO_REPORT */
        son_mro_cell_fsm_invalid_event_handler,
        /* MRO_CELL_IDX_RRM_EVENT_HO_ATTEMPTS */
        son_mro_cell_fsm_invalid_event_handler,
        /* MRO_CELL_IDX_RRM_EVENT_CELL_STATE_CHANGE_IND */
        son_mro_cell_fsm_invalid_event_handler,
        /* MRO_CELL_IDX_SONANR_CELL_ANR_MODE_IND */
        son_mro_cell_fsm_invalid_event_handler,
        /* MRO_CELL_IDX_RRC_HO_REPORT_IND */
        son_mro_cell_fsm_mlb_nbr_cell_status_ind,
        /* MRO_CELL_IDX_MLB_NBR_CELL_STATUS_IND */
    },
    /* State MRO_CELL_STATE_ENABLING */
    {
        son_mro_cell_fsm_nack_enable_req_handler,
        /* MRO_CELL_IDX_SONMRO_EVENT_ENABLE_REQ */
        son_mro_cell_fsm_disable_req_handler,
        /* MRO_CELL_IDX_SONRMO_EVENT_DISABLE_REQ */
        son_mro_cell_fsm_modify_config_req_handler,
        /* MRO_CELL_IDX_SONMRO_EVENT_MODIFY_CONFIG_REQ */
        son_mro_cell_fsm_register_resp_handler,
        /* MRO_CELL_IDX_RRM_EVENT_REGISTER_RES */
        son_mro_cell_fsm_invalid_event_handler,
        /* MRO_CELL_IDX_RRM_EVENT_DEREGISTER_RES */
        son_mro_cell_fsm_invalid_event_handler,
        /* MRO_CELL_IDX_RRM_EVENT_HO_REPORT */
        son_mro_cell_fsm_invalid_event_handler,
        /* MRO_CELL_IDX_RRM_EVENT_HO_ATTEMPTS */
        son_mro_cell_fsm_invalid_event_handler,
        /* MRO_CELL_IDX_RRM_EVENT_CELL_STATE_CHANGE_IND */
        son_mro_cell_fsm_invalid_event_handler,
        /* MRO_CELL_IDX_SONANR_CELL_ANR_MODE_IND */
        son_mro_cell_fsm_invalid_event_handler,
        /* MRO_CELL_IDX_RRC_HO_REPORT_IND */
        son_mro_cell_fsm_mlb_nbr_cell_status_ind,
        /* MRO_CELL_IDX_MLB_NBR_CELL_STATUS_IND */
    },
    /* State MRO_CELL_STATE_ENABLED */
    {
        son_mro_cell_fsm_nack_enable_req_handler,
        /* MRO_CELL_IDX_SONMRO_EVENT_ENABLE_REQ */
        son_mro_cell_fsm_disable_req_handler,
        /* MRO_CELL_IDX_SONRMO_EVENT_DISABLE_REQ */
        son_mro_cell_fsm_modify_config_req_handler,
        /* MRO_CELL_IDX_SONMRO_EVENT_MODIFY_CONFIG_REQ */
        son_mro_cell_fsm_invalid_event_handler,
        /* MRO_CELL_IDX_RRM_EVENT_REGISTER_RES */
        son_mro_cell_fsm_invalid_event_handler,
        /* MRO_CELL_IDX_RRM_EVENT_DEREGISTER_RES */
        son_mro_cell_fsm_ho_report_handler,
        /* MRO_CELL_IDX_RRM_EVENT_HO_REPORT */
        son_mro_cell_fsm_ho_attempts_ind_handler,
        /* MRO_CELL_IDX_RRM_EVENT_HO_ATTEMPTS */
        son_mro_cell_fsm_state_change_ind_handler,
        /* MRO_CELL_IDX_RRM_EVENT_CELL_STATE_CHANGE_IND */
        son_mro_cell_fsm_anr_mode_ind_handler,
        /* MRO_CELL_IDX_SONANR_CELL_ANR_MODE_IND */
        son_mro_cell_fsm_ho_report_handler,
        /* MRO_CELL_IDX_RRC_HO_REPORT_IND */
        son_mro_cell_fsm_mlb_nbr_cell_status_ind,
        /* MRO_CELL_IDX_MLB_NBR_CELL_STATUS_IND */
    },
    /* State MRO_CELL_STATE_DISABLING */
    {
        son_mro_cell_fsm_enable_req_handler,
        /* MRO_CELL_IDX_SONMRO_EVENT_ENABLE_REQ */
        son_mro_cell_fsm_nack_disable_req_handler,
        /* MRO_CELL_IDX_SONRMO_EVENT_DISABLE_REQ */
        son_mro_cell_fsm_modify_config_req_handler,
        /* MRO_CELL_IDX_SONMRO_EVENT_MODIFY_CONFIG_REQ */
        son_mro_cell_fsm_invalid_event_handler,
        /* MRO_CELL_IDX_RRM_EVENT_REGISTER_RES */
        son_mro_cell_fsm_deregister_resp_handler,
        /* MRO_CELL_IDX_RRM_EVENT_DEREGISTER_RES */
        son_mro_cell_fsm_ho_report_handler,
        /* MRO_CELL_IDX_RRM_EVENT_HO_REPORT */
        son_mro_cell_fsm_ho_attempts_ind_handler,
        /* MRO_CELL_IDX_RRM_EVENT_HO_ATTEMPTS */
        son_mro_cell_fsm_state_change_ind_handler,
        /* MRO_CELL_IDX_RRM_EVENT_CELL_STATE_CHANGE_IND */
        son_mro_cell_fsm_invalid_event_handler,
        /* MRO_CELL_IDX_SONANR_CELL_ANR_MODE_IND */
        son_mro_cell_fsm_ho_report_handler,
        /* MRO_CELL_IDX_RRC_HO_REPORT_IND */
        son_mro_cell_fsm_mlb_nbr_cell_status_ind
        /* MRO_CELL_IDX_MLB_NBR_CELL_STATUS_IND */
    }
};    

/*****************************************************************************
 * Function Name  : mro_cell_calculate_msg_index
 * Inputs         : p_cspl_hdr         Pointer to CSPL Header
 * Outputs        : None
 * Returns        : Index in SONMRO cell FSM
 * Description    : This function computes the index in the MRO cell FSM for the
 *                  given procedure code
 ****************************************************************************/
static son_mro_cell_index_et
mro_cell_calculate_msg_index
(
    STACKAPIHDR         *p_cspl_hdr
)
{
    son_mro_cell_index_et ret = MRO_CELL_IDX_NOT_AVAILABLE;

    SON_UT_TRACE_ENTER();

    if (SON_MIF_MODULE_ID == p_cspl_hdr->from)
    {
        switch (p_cspl_hdr->api)
        {
            case SMIF_CELL_INFO_IND:

                /* break left intentionally as both cell info ind and enable 
             * request will be handled using same handler funciton */
            case SONMRO_ENABLE_REQ:
            {
                ret = MRO_CELL_IDX_SONMRO_EVENT_ENABLE_REQ;
                break;
            }

            case SONMRO_DISABLE_REQ:
            {
                ret = MRO_CELL_IDX_SONRMO_EVENT_DISABLE_REQ;
                break;
            }

            case SONMRO_MODIFY_CONFIG_PARAM_REQ:
            {
                ret = MRO_CELL_IDX_SONMRO_EVENT_MODIFY_CONFIG_REQ;
                break;
            }

            default:
            {
                SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                        SON_WARNING,
                        "mro_cell_calculate_msg_index: "
                        "Invalid API = %u received from SMIF", p_cspl_hdr->api);
                break;
        }
        } /* switch */
    }
    else if (SON_RRM_MODULE_ID == p_cspl_hdr->from)
    {
        switch (p_cspl_hdr->api)
        {
            case RRM_SON_REGISTER_RESP:
            {
                ret = MRO_CELL_IDX_RRM_EVENT_REGISTER_RES;
                break;
            }

            case RRM_SON_DEREGISTER_RESP:
            {
                ret = MRO_CELL_IDX_RRM_EVENT_DEREGISTER_RES;
                break;
            }

            case RRM_SON_HO_FAILURE_REPORT:
            {
                ret = MRO_CELL_IDX_RRM_EVENT_HO_REPORT;
                break;
            }

            case RRM_SON_HO_ATTEMPT_IND:
            {
                ret = MRO_CELL_IDX_RRM_EVENT_HO_ATTEMPTS;
                break;
            }

            case RRM_SON_CELL_STATE_CHANGE_IND:
            {
                ret = MRO_CELL_IDX_RRM_EVENT_CELL_STATE_CHANGE_IND;
                break;
            }

            default:
            {
                SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                        SON_WARNING,
                        "mro_cell_calculate_msg_index: "
                        "Invalid API = %u received from RRM", p_cspl_hdr->api);
                break;
        }
        } /* switch */
    }
    else if (SON_ANR_MODULE_ID == p_cspl_hdr->from)
    {
        switch (p_cspl_hdr->api)
        {
            case SONANR_DISABLE_IND:    
            {
                ret = MRO_CELL_IDX_SONANR_CELL_ANR_MODE_IND;
                break;
            }

            default:
            {
                SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                        SON_WARNING,
                        "mro_cell_calculate_msg_index: "
                        "Invalid API = %u received from ANR", p_cspl_hdr->api);
                break;
        }
    }
    }
    else if (SON_X2_MODULE_ID == p_cspl_hdr->from)
    {
        switch (p_cspl_hdr->api)
        {
            case SONX2_HO_REPORT_IND:
            {
                ret = MRO_CELL_IDX_RRC_HO_REPORT_IND;
                break;
            }

            default:
            {
                SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                        SON_WARNING,
                        "mro_cell_calculate_msg_index: "
                        "Invalid API = %u received from X2", p_cspl_hdr->api);
                break;
        }
    }
    }
    else if (SON_MLB_MODULE_ID == p_cspl_hdr->from)
    {
        switch (p_cspl_hdr->api)
        {
            case SONMLB_NBR_CELL_STATUS_IND:
            {
                ret = MRO_CELL_IDX_MLB_NBR_CELL_STATUS_IND;
                break;
            }

            default:
            {
                SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                    SON_WARNING,
                    "mro_cell_calculate_msg_index: "
                    "Invalid API = %u received from MLB", p_cspl_hdr->api);
                break;
            }
        }
    }
    else
    {
        SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                SON_WARNING,
                "mro_cell_calculate_msg_index: "
                "invalid api = %u received from %u", p_cspl_hdr->api, 
                p_cspl_hdr->from);
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* mro_cell_calculate_msg_index */

/*****************************************************************************
 * Function Name  : mro_cell_process_msg
 * Inputs         : p_cspl_hdr         Pointer to CSPL Header
 *                  p_cell_ctxt        Pointer to cell context
 *                  p_mro_msg          Pointer to incoming message
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This is the entry point to the MRO Cell FSM.
 *                  This function takes the API message received, CSPL header,
 *                  cell context and calls the required event handler function.
 ****************************************************************************/
son_void_t
mro_cell_process_msg
(
    STACKAPIHDR         *p_cspl_hdr,
    mro_cell_context_t  *p_cell_ctxt,
    son_u8              *p_mro_msg
)
{   
    son_mro_cell_index_et event_id = MRO_CELL_IDX_NOT_AVAILABLE;

    SON_UT_TRACE_ENTER();

    /* Calculate Cell FSM index */
    event_id = mro_cell_calculate_msg_index(p_cspl_hdr);

    /* Validate the index of mro_cell_fsm_table array*/
    if (MRO_CELL_NUM_OF_EVENTS <= event_id)
    {
        /* No need to print the log here for invalid API Id as it is
         * already printed in mro_cell_calculate_msg_index function */
        /* Do nothing */
    }
    /* Validate the cell FSM state */
    else if (MRO_CELL_NUM_OF_STATES <= p_cell_ctxt->current_cell_fsm_state)
    {
        SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name,
                SON_WARNING,
                "Message with API Id = %u received in "
                "invalid state = %u", p_cspl_hdr->api,
                p_cell_ctxt->current_cell_fsm_state);
    }
    else 
    {
/* SPR_18169 Fix Start */
        SON_LOG(anr_get_log_mode(), p_son_mro_facility_name, SON_INFO,
              "### MRO Cell FSM: Cell_id: 0x%x Processing State %s Event %s ###",
              son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity),
              SON_MRO_CELL_FSM_STATES_NAMES[p_cell_ctxt->current_cell_fsm_state],
              SON_MRO_CELL_FSM_EVENT_NAMES[event_id]);

        /* Call the appropriate message handler based on the API Id */ 
         (*son_mro_cell_fsm_table[p_cell_ctxt->current_cell_fsm_state]
            [event_id])(event_id, p_cspl_hdr, p_cell_ctxt, p_mro_msg);
	 SON_LOG(anr_get_log_mode(), p_son_mro_facility_name, SON_INFO,
			 "### MRO Cell FSM Exit ###");
/* SPR_18169 Fix End */
    }

    SON_UT_TRACE_EXIT();
} /* mro_cell_process_msg */
