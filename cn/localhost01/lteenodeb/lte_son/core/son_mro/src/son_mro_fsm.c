/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_mro_fsm.c $
 *
 *******************************************************************************
 *
 * File Description: This file contains the code for MRO global level FSM and 
 * defines functions for handling MRO global level messages.
 *
 *******************************************************************************
 * Revision Details
 *----------------------
 *   DATE            AUTHOR       REFERENCE       REASON
 *   Dec, 2012       Shilpi       Initial         Initial
 *   30-May-2014     Tirtha       SPR-11452       Some warnings is observed 
 *                                                in SON logs
 *   Aug, 2014       Shilpi                       SPR 13251 Fix
 ******************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_mro_ctxt_mgr.h>
#include <son_mro_cell_fsm.h>
#include <son_mro_fsm.h>
#include <son_anr_intf.h>
#include <son_mro_intf.h>
#include <rrm_son_parser.h>
#include <x2ap_api.h>
#include <son_types.h>

/****************************************************************************
 * Global Variables
 ****************************************************************************/
extern mro_global_context_t g_mro_gb_ctxt;
extern const son_8 *p_son_mro_facility_name;
extern const son_s8 *SON_MRO_FSM_EVENT_NAMES[];
extern const son_s8 *SON_MRO_FSM_STATES_NAMES[];

/*****************************************************************************
 * Function Name  : mro_fsm_init_config_req_handler
 * Inputs         : p_buf - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_INIT_CONFIG_REQ
 *                  message received from SONMgmtIfH in MRO_STATE_INIT state.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_init_config_req_handler
(
    son_u8          *p_buf,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    smif_init_config_req_t *p_msg = (smif_init_config_req_t *)p_buf;
    smif_init_config_res_t init_config_res = {0}; 

    SON_UT_TRACE_ENTER();

    SON_LOG(g_mro_gb_ctxt.log_mode,
            p_son_mro_facility_name, SON_BRIEF,
            "SMIF_INIT_CONFIG_REQ received: "
            "trans Id: %u, log enable: %u , log level: %u",
            p_msg->transaction_id,
            (son_u16)p_msg->log_enable,
            (son_u16)p_msg->log_level);
    mro_set_log_mode((son_oam_log_on_off_et)(p_msg->log_enable));
                    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(event_id);
    LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
    if (SON_OAM_LOG_ON == g_mro_gb_ctxt.log_mode)
    {
        /* SPR-13251 Fix Starts */
        mro_set_log_level(p_msg->log_level);
        /* SPR-13251 Fix Ends */
        SET_MODULE_LOG_LEVEL(p_msg->log_level);
    }
    else
    {
        /* SPR-13251 Fix Starts */
        mro_set_log_level(SON_LOG_LEVEL_NONE);
        /* SPR-13251 Fix Ends */
        /* Logging is disabled so set log level to NONE  */
        SET_MODULE_LOG_LEVEL(SON_LOG_LEVEL_NONE);
    }

    init_config_res.transaction_id = p_msg->transaction_id;
    init_config_res.result = SON_SUCCESS;
    init_config_res.error_code = SON_NO_ERROR;
    son_create_send_buffer((son_u8 *)&init_config_res,
            SON_MRO_MODULE_ID, SON_MIF_MODULE_ID, SMIF_INIT_CONFIG_RES,
            sizeof(smif_init_config_res_t));

    /* Set MRO global FSM state to ACTIVE */
    SONMRO_FSM_SET_STATE(g_mro_gb_ctxt.current_fsm_state,
        MRO_STATE_ACTIVE);

    SON_UT_TRACE_EXIT();
    /* Release the buffer memory as it is no longer needed. */
    return RELEASE_SON_BUFFER;
} /* mro_fsm_init_config_req_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_nack_init_config_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function to send negative response 
 *                  for SMIF_INIT_CONFIG_REQ message received from SONMgmtIfH 
 *                  in any state other than MRO_STATE_INIT state.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_nack_init_config_req_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    smif_init_config_res_t smif_init_config_res = {0};
    smif_init_config_req_t *p_smif_init_config_req =
        (smif_init_config_req_t *)p_msg;

    SON_UT_TRACE_ENTER();

                    /*SPR 17777 +-*/
    SON_LOG(g_mro_gb_ctxt.log_mode,
            p_son_mro_facility_name, SON_ERROR,
            "SMIF_INIT_CONFIG_REQ "
            "received in invalid state %s:"
            "transaction Id: %u log enable: %u log level: %u",
            SON_MRO_FSM_STATES_NAMES[g_mro_gb_ctxt.current_fsm_state],
            p_smif_init_config_req->transaction_id,
            (son_u16)p_smif_init_config_req->log_enable,
            (son_u16)p_smif_init_config_req->log_level);
            LTE_GCC_UNUSED_PARAM(event_id);
            LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/

    /* Populate the init config response structure */
    smif_init_config_res.transaction_id =
        p_smif_init_config_req->transaction_id;
    smif_init_config_res.result = SON_FAILURE;
    smif_init_config_res.error_code = SON_ERR_UNEXPECTED_MSG;

    /* Sends SMIF_INIT_CONFIG_RES to SONMgmtIfh */
    son_create_send_buffer((son_u8 *)&smif_init_config_res,
            SON_MRO_MODULE_ID, SON_MIF_MODULE_ID, SMIF_INIT_CONFIG_RES,
            sizeof(smif_init_config_res_t));

    SON_UT_TRACE_EXIT();
    /* Release the buffer memory as it is no longer needed. */
    return RELEASE_SON_BUFFER;
} /* mro_fsm_nack_init_config_req_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_invalid_event_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for the messages received
 *                  in invalid states for which nothing needs to be done.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_invalid_event_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    /* Rel 3.0 Cell delete support changes Start */
    son_rrm_deregistration_res_t *p_res = SON_PNULL;
    /* Rel 3.0 Cell delete support changes Stop */
    SON_UT_TRACE_ENTER();

    /* Rel 3.0 Cell delete support changes Start */
    if (MRO_IDX_RRM_EVENT_DEREGISTER_RESP == event_id)
    {
        p_res = (son_rrm_deregistration_res_t *)p_msg;

        /* Find the transaction Id received in De-register response in
           temporary transaction Id list stored during cell deletion.
           If found remove it from temporary transaction Id list and
           do not process the corresponding De-register response as the
           cell is already deleted */
        if (SON_SUCCESS == 
                son_find_txn_id_in_temp_txn_id_list(p_res->transaction_id))
        {
            SON_LOG(g_mro_gb_ctxt.log_mode,
                    p_son_mro_facility_name, SON_DETAILED,
                "mro_fsm_invalid_event_handler: RRM_SON_DEREGISTER_RESP "
                "received for the cell [Cell Id: 0x%x] already deleted "
                "at MRO due to cell delete request from OAM so ignore it",
                son_convert_char_cell_id_to_int_cell_id(
                    p_res->res.deregistration_resp.id.object_id.
                    cell_id.cell_identity));
            SON_UT_TRACE_EXIT();
            return RELEASE_SON_BUFFER;
        }
    }
    /* Rel 3.0 Cell delete support changes Stop */

    SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, SON_WARNING,
            "mro_fsm_invalid_event_handler: Invalid Event %s occured "
            "in %s state from %u",
            SON_MRO_FSM_EVENT_NAMES[event_id],
            SON_MRO_FSM_STATES_NAMES[g_mro_gb_ctxt.current_fsm_state],
            p_cspl_hdr->from);

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
}

/*****************************************************************************
 * Function Name  : mro_fsm_cell_info_ind_enabling_state_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_CELL_INFO_IND
  *                 message received from SONMgmtIfH in MRO_STATE_ENABLING.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_cell_info_ind_enabling_state_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    SON_UT_TRACE_ENTER();
/*+ SPR 11452 Fix +*/
                    /*SPR 17777 +-*/
    SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, SON_DETAILED,
            "mro_fsm_cell_info_ind_enabling_state_handler: Global enable is "
            "in progress so cell info indication can't be processed.evnt_id=%d p_cspl_hdr=%p,p_msg=%p",
            event_id,p_cspl_hdr,p_msg);
                    /*SPR 17777 +-*/
/*- SPR 11452 Fix -*/
    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
}

/*****************************************************************************
 * Function Name  : mro_fsm_cell_info_ind_disabling_state_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_CELL_INFO_IND
  *                 message received from SONMgmtIfH in MRO_STATE_DISABLING.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_cell_info_ind_disabling_state_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    SON_UT_TRACE_ENTER();

                    /*SPR 17777 +-*/
    SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, SON_WARNING,
            "mro_fsm_cell_info_ind_disabling_state_handler: Global disable is "
            "in progress so cell info indication can't be processed.evnt_id=%d p_cspl_hdr=%p p_msg=%p"
            ,event_id,p_cspl_hdr,p_msg);
                    /*SPR 17777 +-*/

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
}

/*****************************************************************************
 * Function Name  : mro_fsm_shutdown_state_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for the messages received
 *                  in MRO_STATE_SHUTDOWN state.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_shutdown_state_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    SON_UT_TRACE_ENTER();

                    /*SPR 17777 +-*/
    SON_LOG(g_mro_gb_ctxt.log_mode, p_son_mro_facility_name, SON_BRIEF,
            "mro_fsm_shutdown_state_handler: Event %s ignored in "
            "MRO_STATE_SHUTDOWN state received from %u;p_msg=%p", 
            SON_MRO_FSM_EVENT_NAMES[event_id], 
            p_cspl_hdr->from,p_msg);
                    /*SPR 17777 +-*/
    
    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
}

/*****************************************************************************
 * Function Name  : mro_fsm_set_log_level_cmd_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  SMIF_EVENT_SET_LOG_LEVEL_CMD message received from 
 *                  SONMgmtIfH in state other than MRO_STATE_INIT and 
 *                  MRO_STATE_SHUTDOWN state.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_set_log_level_cmd_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    smif_set_log_level_cmd_t *p_cmd =
        (smif_set_log_level_cmd_t *)p_msg;

    SON_UT_TRACE_ENTER();

    if (SON_OAM_LOG_ON == g_mro_gb_ctxt.log_mode)
    {
                    /*SPR 17777 +-*/
        SON_LOG(SON_OAM_LOG_ON, p_son_mro_facility_name,
            SON_BRIEF,
            "Log level "
            "changed to %u through msg: SMIF_SET_LOG_LEVEL_CMD"
            "evnt_id=%d p_cspl_hdr=%p",
            (son_u8)p_cmd->log_level,event_id,p_cspl_hdr);
                    /*SPR 17777 +-*/
        /* SPR-13251 Fix Starts */
        mro_set_log_level(p_cmd->log_level);
        /* SPR-13251 Fix Ends */
        SET_MODULE_LOG_LEVEL(p_cmd->log_level);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mro_fsm_set_log_level_cmd_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_log_enable_disable_cmd_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  SMIF_EVENT_LOG_ENABLE_DISABLE_CMD message received from 
 *                  SONMgmtIfH in state other than MRO_STATE_INIT and 
 *                  MRO_STATE_SHUTDOWN state.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_log_enable_disable_cmd_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    smif_log_enable_disable_cmd_t *p_cmd =
        (smif_log_enable_disable_cmd_t *)p_msg;

    SON_UT_TRACE_ENTER();

    mro_set_log_mode((son_oam_log_on_off_et)(p_cmd->log_enable));

    /* Print the current logging status(enable/disable)
     *  SON_TRUE: always print log, even if it is disabled. */
                    /*SPR 17777 +-*/
    SON_LOG(SON_OAM_LOG_ON, p_son_mro_facility_name,
            SON_BRIEF,
            "Log enable/disable %u",
            p_cmd->log_enable);
    LTE_GCC_UNUSED_PARAM(event_id);
    LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/

    if (SON_OAM_LOG_ON == g_mro_gb_ctxt.log_mode)
    {
        if (SMIF_MODULE_LOG_LEVEL_PRESENT & p_cmd->bitmask)
        {
            /* SPR-13251 Fix Starts */
            mro_set_log_level(p_cmd->log_level);
            /* SPR-13251 Fix Ends */
            SET_MODULE_LOG_LEVEL(p_cmd->log_level);
        }
        else
        {
            /* SPR-13251 Fix Starts */
            mro_set_log_level(SON_OAM_LOG_LEVEL_BRIEF);
            /* SPR-13251 Fix Ends */
            /* Set log level to Default (Brief) value since no log level 
             *  provided in the request message */
            SET_MODULE_LOG_LEVEL(SON_OAM_LOG_LEVEL_BRIEF);
        }
    }
    else
    {
        /* SPR-13251 Fix Starts */
        mro_set_log_level(SON_LOG_LEVEL_NONE);
        /* SPR-13251 Fix Ends */
        /* Set log level to NONE  */
        SET_MODULE_LOG_LEVEL(SON_LOG_LEVEL_NONE);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mro_fsm_log_enable_disable_cmd_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_shutdown_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_SHUTDOWN_REQ
 *                  message received from SONMgmtIfH in any state other than
 *                  MRO_STATE_SHUTDOWN state.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_shutdown_req_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    smif_shutdown_req_t *p_req = (smif_shutdown_req_t *)p_msg;    
    smif_shutdown_res_t shut_down_res = {0};
    /* SPR 20163 Fix Start */
	mro_cell_context_node_t *p_ctxt_node = SON_PNULL;
    /* SPR 20163 Fix End */
    SON_UT_TRACE_ENTER();

    SON_LOG(g_mro_gb_ctxt.log_mode,
            p_son_mro_facility_name, SON_BRIEF,
            "SMIF_SHUTDOWN_REQ received "
            "transaction Id: %u",
            p_req->transaction_id);
    
    /* SPR 20163 Fix Start */
	p_ctxt_node = mro_get_first_cell_ctxt();
	while (SON_PNULL != p_ctxt_node)
	{
        mro_delete_cell_from_context(&p_ctxt_node->data.cell_id);
	    p_ctxt_node = mro_get_next_node(p_ctxt_node);
	}
    /* SPR 20163 Fix End */

    /* Clean up the MRO cell contexts list */
    mro_clear_cell_contexts_list(p_req->transaction_id);

    /* Clean up ENABLE/DISABLE Pending Response list */
    mro_clear_pending_res_list();

    if (SON_TRUE == 
        son_is_timer_running(g_mro_gb_ctxt.mro_pending_res_timer_id))
    {
        son_stop_timer(g_mro_gb_ctxt.mro_pending_res_timer_id);
                    /*SPR 17777 +-*/
        LTE_GCC_UNUSED_PARAM(event_id);
        LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
        g_mro_gb_ctxt.mro_pending_res_timer_id = SON_PNULL;
    }

    /* Set MRO Global FSM state to SHUTDOWN */
    SONMRO_FSM_SET_STATE(g_mro_gb_ctxt.current_fsm_state,
            MRO_STATE_SHUTDOWN);

    /* Set the number of cells configured at MRO to 0 */
    list_init(&g_mro_gb_ctxt.mro_cell_ctxt_list);
    /* Send SMIF_SHUTDOWN_RES to SONMgmtIfH*/
    shut_down_res.transaction_id = p_req->transaction_id;
    shut_down_res.result         = SON_SUCCESS;
    shut_down_res.error_code     = SON_NO_ERROR;

    son_create_send_buffer((son_u8 *)&shut_down_res, SON_MRO_MODULE_ID,
            SON_MIF_MODULE_ID, SMIF_SHUTDOWN_RES, sizeof(smif_shutdown_res_t));

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mro_fsm_shutdown_req_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_nack_enable_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function to send nack for 
 *                  SONMRO_ENABLE_REQ message received from SONMgmtIfH.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_nack_enable_req_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    son_mro_enable_req_t *p_req = (son_mro_enable_req_t *)p_msg;
    son_mro_enable_res_t enable_res;
    son_size_t enable_res_len = sizeof(son_mro_enable_res_t);

    SON_UT_TRACE_ENTER();

    if (SON_MRO_ENABLE_SRV_CGI_PRESENT & p_req->enable_req.bitmask)
    {
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_ERROR,
                "Cell specific "
                "SONMRO_ENABLE_REQ received in invalid state %s:"
                "transaction Id: %u",
                SON_MRO_FSM_STATES_NAMES[g_mro_gb_ctxt.current_fsm_state],
                p_req->transaction_id);
    }
    else
    {
                    /*SPR 17777 +-*/
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_ERROR,
                "Global "
                "SONMRO_ENABLE_REQ received in invalid state %s:"
                "transaction Id: %u",
                SON_MRO_FSM_STATES_NAMES[g_mro_gb_ctxt.current_fsm_state],
                p_req->transaction_id);
        LTE_GCC_UNUSED_PARAM(event_id);
        LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
    }

    SON_MEMSET(&enable_res, 0, enable_res_len);
    enable_res.transaction_id = p_req->transaction_id;
    enable_res.enable_res.result = SON_FAILURE;
    if (MRO_STATE_INIT == g_mro_gb_ctxt.current_fsm_state)
    {
        enable_res.enable_res.error_code = SON_ERR_UNEXPECTED_MSG;
    }
    else if (MRO_STATE_ENABLING == g_mro_gb_ctxt.current_fsm_state)
    {
        enable_res.enable_res.error_code = SON_ERR_ENABLE_IN_PROGRESS;
    }
    else 
    {
        enable_res.enable_res.error_code = SON_ERR_DISABLE_IN_PROGRESS;
    }

    son_create_send_buffer((son_u8 *)&enable_res,
            SON_MRO_MODULE_ID, SON_MIF_MODULE_ID, SONMRO_ENABLE_RES,
            enable_res_len);

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mro_fsm_nack_enable_req_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_nack_disable_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function to send nack for 
 *                  SONMRO_DISABLE_REQ message received from SONMgmtIfH.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_nack_disable_req_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    son_mro_disable_req_t *p_req = (son_mro_disable_req_t *)p_msg;
    son_mro_disable_res_t disable_res;
    son_size_t disable_res_len = sizeof(son_mro_disable_res_t);

    SON_UT_TRACE_ENTER();

    if (SON_MRO_DISABLE_SRV_CGI_PRESENT & p_req->disable_req.bitmask)
    {
                    /*SPR 17777 +-*/
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_ERROR,
                "Cell specific "
                "SONMRO_DISABLE_REQ received in invalid state %s:"
                "transaction Id: %u",
                SON_MRO_FSM_STATES_NAMES[g_mro_gb_ctxt.current_fsm_state],
                p_req->transaction_id);
        LTE_GCC_UNUSED_PARAM(event_id);
        LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
    }
    else
    {
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_ERROR,
                "Global "
                "SONMRO_DISABLE_REQ received in invalid state %s:"
                "transaction Id: %u",
                SON_MRO_FSM_STATES_NAMES[g_mro_gb_ctxt.current_fsm_state],
                p_req->transaction_id);
    }

    SON_MEMSET(&disable_res, 0, disable_res_len);
    disable_res.transaction_id = p_req->transaction_id;
    disable_res.disable_res.result = SON_FAILURE;
    if (MRO_STATE_INIT == g_mro_gb_ctxt.current_fsm_state)
    {
        disable_res.disable_res.error_code = SON_ERR_UNEXPECTED_MSG;
    }
    else if (MRO_STATE_ENABLING == g_mro_gb_ctxt.current_fsm_state)
    {
        disable_res.disable_res.error_code = SON_ERR_ENABLE_IN_PROGRESS;
    }
    else 
    {
        disable_res.disable_res.error_code = SON_ERR_DISABLE_IN_PROGRESS;
    }

    son_create_send_buffer((son_u8 *)&disable_res,
            SON_MRO_MODULE_ID, SON_MIF_MODULE_ID, SONMRO_DISABLE_RES,
            disable_res_len);

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mro_fsm_nack_disable_req_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_nack_modify_config_param_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function to send nack for 
 *                  SONMRO_MODIFY_CONFIG_PARAM_REQ message received from 
 *                  SONMgmtIfH.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_nack_modify_config_param_req_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    son_mro_modify_config_params_req_t *p_req = 
        (son_mro_modify_config_params_req_t *)p_msg;
    son_mro_modify_config_params_res_t config_param_res;
    son_size_t config_param_res_len = 
        sizeof(son_mro_modify_config_params_res_t);

    SON_UT_TRACE_ENTER();

                    /*SPR 17777 +-*/
    SON_LOG(g_mro_gb_ctxt.log_mode,
            p_son_mro_facility_name, SON_ERROR,
            "SONMRO_MODIFY_CONFIG_PARAM_REQ received in invalid state %s:"
            "transaction Id: %u",
            SON_MRO_FSM_STATES_NAMES[g_mro_gb_ctxt.current_fsm_state],
            p_req->transaction_id);
            LTE_GCC_UNUSED_PARAM(event_id);
            LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/

    SON_MEMSET(&config_param_res, 0, config_param_res_len);
    SON_MEMCPY(&config_param_res.config_param_res.srv_cgi,
            &p_req->config_param_req.srv_cgi,
            sizeof(son_intra_rat_global_cell_id_t));
    config_param_res.transaction_id = p_req->transaction_id;
    config_param_res.config_param_res.result = SON_FAILURE;
    config_param_res.config_param_res.error_code = SON_ERR_UNEXPECTED_MSG;
    son_create_send_buffer((son_u8 *)&config_param_res,
            SON_MRO_MODULE_ID, SON_MIF_MODULE_ID, 
            SONMRO_MODIFY_CONFIG_PARAM_RES, config_param_res_len);

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mro_fsm_nack_modify_config_param_req_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_cell_info_ind_active_state_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_CELL_INFO_IND 
 *                  message received from SONMgmtIfH in MRO_STATE_ACTIVE.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_cell_info_ind_active_state_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    son_cell_info_ind_t *p_ind = (son_cell_info_ind_t *)p_msg;
    son_u8 idx = 0;
    mro_cell_context_node_t *p_cell_ctxt = SON_PNULL;
    son_feature_state_change_cause_et cause = SON_STATE_CHANGE_CAUSE_UNDEFINED;
    son_feature_state_et state = SON_STATE_UNDEFINED;
    /*SPR_19279_START */
    son_u8 cell_index = *((son_u8 *)(p_msg + sizeof(son_cell_info_ind_t)));
    /*SPR_19279_END */

    SON_UT_TRACE_ENTER();

    SON_LOG(g_mro_gb_ctxt.log_mode,
            p_son_mro_facility_name, SON_INFO,
            "MIF->MRO:SMIF_CELL_INFO_IND");

    /* Check if any cell for configuration is present in the request */
    if (0 == p_ind->cell_info_size)
    {
                    /*SPR 17777 +-*/
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_WARNING,
                "Cell list is empty");
        LTE_GCC_UNUSED_PARAM(event_id);
                    /*SPR 17777 +-*/
    }
    else
    {
        /* Check MRO Global Mode */
        if (SON_ENABLED == g_mro_gb_ctxt.mro_global_mode)
        {
            mro_init_pending_resp_list(son_generate_txn_id(), 
                    SMIF_CELL_INFO_IND);

            for (idx = 0; idx < p_ind->cell_info_size; idx++)
            {
                /*SPR_19279_START */
                p_cell_ctxt = mro_add_new_cell_in_context(
                        &p_ind->serving_cell[idx], &state, &cause,cell_index);
                /*SPR_19279_END */

                if (SON_PNULL != p_cell_ctxt)
                {
                    son_cell_status_t cell_sts;
                    SON_MEMSET(&cell_sts, 0, sizeof(son_cell_status_t));
                    cell_sts.error_code = SON_ERR_TIMER_EXPIRED;
                    SON_MEMCPY(&cell_sts.cgi,
                            &p_ind->serving_cell[idx].src_cgi,
                            sizeof(son_intra_rat_global_cell_id_t));

                    /* Add Cell ID in MRO_Pending_Response_LIST */ 
                    if (SON_SUCCESS == mro_add_cell_in_pending_res(&cell_sts))
                    {
                        son_mro_enable_req_t enable_req;
                        SON_MEMSET(&enable_req, 0, 
                            sizeof(son_mro_enable_req_t));
                        enable_req.transaction_id = 
                            mro_get_pending_res_transaction_id();
                        enable_req.enable_req.bitmask |= 
                            SON_MRO_ENABLE_SRV_CGI_PRESENT;
                        SON_MEMCPY(&enable_req.enable_req.srv_cgi,
                                &p_ind->serving_cell[idx].src_cgi,
                                sizeof(son_intra_rat_global_cell_id_t));
                        /* Call cell specific FSM handler */
                        mro_cell_process_msg(p_cspl_hdr, &p_cell_ctxt->data, 
                            (son_u8 *)&enable_req);
                    }
                }
                else
                {
                    /* Send SONMRO_FEATURE_STATE_CHANGE_IND */
                    mro_send_feature_state_change_ind(
                            &p_ind->serving_cell[idx].src_cgi, 
                            state, cause);
                }
            }

            if (0 == mro_get_pending_res_expected_count())
            {
                mro_reset_pending_resp_list();
            }
            else
            {
                /* Set MRO global FSM state to MRO_STATE_ENABLING 
                 * as at least 1 registration request has been sent */
                SONMRO_FSM_SET_STATE(g_mro_gb_ctxt.current_fsm_state,
                        MRO_STATE_ENABLING);
                g_mro_gb_ctxt.mro_pending_res_timer_id = son_start_timer(
                        MRO_DEFULT_PENDING_RESPONSE_TIMER, SON_PNULL, SON_NULL, 
                        SON_FALSE);
            }
        }
        else
        {
            /* Add cell in MRO cell configuration list if number of current 
             * configured cells is below maximum number of configured cells
             * allowed at MRO and the cell is not already configured.
             * Cell must be added with MRO mode as disabled by default. */
            for (idx = 0; idx < p_ind->cell_info_size; idx++)
            {
                mro_add_new_cell_in_context(
                        /*SPR_19279_START*/
                        &p_ind->serving_cell[idx], &state, &cause,cell_index);
                         /*SPR_19279_END*/
                /* Send SONMRO_FEATURE_STATE_CHANGE_IND */
                mro_send_feature_state_change_ind(
                        &p_ind->serving_cell[idx].src_cgi, 
                        state, cause);
            }
        }
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mro_fsm_cell_info_ind_active_state_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_enable_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONMRO_ENABLE_REQ 
 *                  message received from SONMgmtIfH.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_enable_req_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    son_mro_enable_req_t *p_req = (son_mro_enable_req_t *)p_msg;
    son_mro_enable_res_t enable_res;
    son_size_t enable_res_len = sizeof(son_mro_enable_res_t);
    mro_cell_context_node_t *p_node = SON_PNULL;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&enable_res, 0, enable_res_len);

    /* Check whether the received SONMRO_ENABLE_REQ is global or cell 
     *  specific */
    if (!(SON_MRO_ENABLE_SRV_CGI_PRESENT & p_req->enable_req.bitmask))
    {
                    /*SPR 17777 +-*/
        son_cell_status_t cell_sts;
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_INFO,
                "Global "
                "SONMRO_ENABLE_REQ received:"
                "transaction Id: %u; event_id=%d",
                p_req->transaction_id,event_id);
                    /*SPR 17777 +-*/

        SON_MEMSET(&cell_sts, 0, sizeof(son_cell_status_t));
        mro_reset_pending_resp_list();
        g_mro_gb_ctxt.mro_global_mode = SON_ENABLED;

        /* Find from the configured cell list whether there is at least one 
         * cell with cell state = MRO_CELL_STATE_DISABLED or
         * MRO_CELL_STATE_DISABLING. */
        if (SON_TRUE == mro_is_any_cell_disabled())
        {
            /* If such cells exists */
            mro_init_pending_resp_list(p_req->transaction_id, 
                    SONMRO_ENABLE_RES);

            p_node = mro_get_first_cell_ctxt();
            while (SON_PNULL != p_node)
            {
                if (MRO_CELL_STATE_DISABLED == 
                        p_node->data.current_cell_fsm_state
                        || MRO_CELL_STATE_DISABLING == 
                        p_node->data.current_cell_fsm_state)
                {
                   cell_sts.error_code = SON_ERR_TIMER_EXPIRED;
                   SON_MEMCPY(&cell_sts.cgi, &p_node->data.cell_id,
                           sizeof(son_intra_rat_global_cell_id_t));

                   if (SON_SUCCESS == mro_add_cell_in_pending_res(&cell_sts))
                   {
                       p_req->enable_req.bitmask |= 
                           SON_MRO_ENABLE_SRV_CGI_PRESENT; 
                       SON_MEMCPY(&p_req->enable_req.srv_cgi,
                               &p_node->data.cell_id,
                               sizeof(son_intra_rat_global_cell_id_t));
                       mro_cell_process_msg(p_cspl_hdr, &p_node->data, 
                               (son_u8 *)p_req);
                   }
                }

                p_node = mro_get_next_node(p_node);
            }

            if (0 == mro_get_pending_res_expected_count())
            {
                /* Construct and send SONMRO_ENABLE_RES with all the cells in
                 *  Pending Response List as per the error status of the
                 *  cell. */
                enable_res.transaction_id = p_req->transaction_id;
                enable_res.enable_res.result = SON_FAILURE;
                enable_res.enable_res.error_code = SON_NO_ERROR;
                mro_populate_enable_disable_res_failure_data_from_pending_list(
                    &enable_res.enable_res.cell_status_list_size, 
                    enable_res.enable_res.cell_status_list);

                son_create_send_buffer((son_u8 *)&enable_res,
                        SON_MRO_MODULE_ID, SON_MIF_MODULE_ID, SONMRO_ENABLE_RES,
                        enable_res_len);

                mro_reset_pending_resp_list();
            }
            else
            {
                /* Set MRO global FSM state to MRO_STATE_ENABLING 
                 *  as at least 1 registration request has been sent */
                SONMRO_FSM_SET_STATE(g_mro_gb_ctxt.current_fsm_state,
                        MRO_STATE_ENABLING);
                g_mro_gb_ctxt.mro_pending_res_timer_id = son_start_timer(
                        MRO_DEFULT_PENDING_RESPONSE_TIMER, SON_PNULL, SON_NULL,
                        SON_FALSE);
            }
        }
        else
        {
            /* Send SONMRO_ENABLE_RES to SONMgmtIfH with result as 
             *  SON_SUCCESS */
            enable_res.transaction_id = p_req->transaction_id;
            enable_res.enable_res.result = SON_SUCCESS;
            enable_res.enable_res.error_code = SON_NO_ERROR;
            son_create_send_buffer((son_u8 *)&enable_res,
                    SON_MRO_MODULE_ID, SON_MIF_MODULE_ID, SONMRO_ENABLE_RES,
                    enable_res_len);
        }
    }
    else
    {
        mro_cell_context_t *p_cell_ctxt = SON_PNULL;
        p_cell_ctxt = mro_get_cell_context_from_global_context(
                &p_req->enable_req.srv_cgi);

        if (SON_PNULL == p_cell_ctxt)
        {
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_BRIEF,
                "Cell specific "
                "SONMRO_ENABLE_REQ received:"
                "trans Id: %u , "
                    "for unconfigured Cell 0x%x",
                p_req->transaction_id,
                son_convert_char_cell_id_to_int_cell_id(p_req->enable_req.srv_cgi.cell_identity));
            /* Send SONMRO_ENABLE_RES to SONMgmtIfH with result SON_FAILURE 
             *  and error code = SON_ERR_CELL_UNCONFIGURED */
            enable_res.transaction_id = p_req->transaction_id;
            enable_res.enable_res.result = SON_FAILURE;
            enable_res.enable_res.error_code = SON_NO_ERROR;
            enable_res.enable_res.cell_status_list_size = 1;
            SON_MEMCPY(&enable_res.enable_res.cell_status_list[0].cgi,
                    &p_req->enable_req.srv_cgi,
                    sizeof(son_intra_rat_global_cell_id_t)); 
            enable_res.enable_res.cell_status_list[0].error_code = 
                SON_ERR_CELL_UNCONFIGURED; 
            son_create_send_buffer((son_u8 *)&enable_res,
                    SON_MRO_MODULE_ID, SON_MIF_MODULE_ID, SONMRO_ENABLE_RES,
                    enable_res_len);
        }
        else
        {
            mro_cell_process_msg(p_cspl_hdr, p_cell_ctxt, (son_u8 *)p_req);
        }
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mro_fsm_enable_req_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_disable_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONMRO_DISABLE_REQ 
 *                  message received from SONMgmtIfH.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_disable_req_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    son_mro_disable_req_t *p_req = (son_mro_disable_req_t *)p_msg;
    son_mro_disable_res_t disable_res;
    son_size_t disable_res_len = sizeof(son_mro_disable_res_t);
    mro_cell_context_node_t *p_node = SON_PNULL;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&disable_res, 0, disable_res_len);
    if (!(SON_MRO_DISABLE_SRV_CGI_PRESENT & p_req->disable_req.bitmask))
    {
        son_cell_status_t cell_sts;
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_INFO,
                "Global "
                "SONMRO_DISABLE_REQ received:"
                "transaction Id: %u",
                p_req->transaction_id);

        SON_MEMSET(&cell_sts, 0, sizeof(son_cell_status_t));
        mro_reset_pending_resp_list();
        g_mro_gb_ctxt.mro_global_mode = SON_DISABLED;

        /* Find from the configure cell list whether there is at least one 
         *  cell with cell state =  MRO_CELL_STATE_ENABLED or
         *  MRO_CELL_STATE_ENABLING state */
        if (SON_TRUE == mro_is_any_cell_enabled())
        {
            /* If such cells exists */
            mro_init_pending_resp_list(p_req->transaction_id, 
                    SONMRO_DISABLE_RES);

            p_node = mro_get_first_cell_ctxt();
            while (SON_PNULL != p_node)
            {
                if (MRO_CELL_STATE_ENABLED == 
                        p_node->data.current_cell_fsm_state
                        || MRO_CELL_STATE_ENABLING == 
                        p_node->data.current_cell_fsm_state)
                {
                   cell_sts.error_code = SON_ERR_TIMER_EXPIRED;
                   SON_MEMCPY(&cell_sts.cgi, &p_node->data.cell_id,
                           sizeof(son_intra_rat_global_cell_id_t));

                   if (SON_SUCCESS == mro_add_cell_in_pending_res(&cell_sts))
                   {
                       p_req->disable_req.bitmask |= 
                           SON_MRO_DISABLE_SRV_CGI_PRESENT;
                       SON_MEMCPY(&p_req->disable_req.srv_cgi,
                               &p_node->data.cell_id,
                               sizeof(son_intra_rat_global_cell_id_t));
                       mro_cell_process_msg(p_cspl_hdr, &p_node->data, 
                               (son_u8 *)p_req);
                   }
                }

                p_node = mro_get_next_node(p_node);
            }

            if (0 == mro_get_pending_res_expected_count())
            {
                /* Construct and send SONMRO_DISABLE_RES with all the cells in
                 *  Pending Response List as per the error status of the
                 *  cell. */
                disable_res.transaction_id = p_req->transaction_id;
                disable_res.disable_res.result = SON_FAILURE;
                disable_res.disable_res.error_code = SON_NO_ERROR;
                mro_populate_enable_disable_res_failure_data_from_pending_list(
                    &disable_res.disable_res.cell_status_list_size,
                    disable_res.disable_res.cell_status_list);

                son_create_send_buffer((son_u8 *)&disable_res,
                        SON_MRO_MODULE_ID, SON_MIF_MODULE_ID, 
                        SONMRO_DISABLE_RES, disable_res_len);

                mro_reset_pending_resp_list();
            }
            else
            {
                /* Set MRO global FSM state to MRO_STATE_DISABLING */
                SONMRO_FSM_SET_STATE(g_mro_gb_ctxt.current_fsm_state,
                        MRO_STATE_DISABLING);
                g_mro_gb_ctxt.mro_pending_res_timer_id = son_start_timer(
                        MRO_DEFULT_PENDING_RESPONSE_TIMER, SON_PNULL, SON_NULL,
                        SON_FALSE);
            }
        }
        else
        {
            /* Send SONMRO_DISABLE_RES to SONMgmtIfH with result as 
             *  SON_SUCCESS */
            disable_res.transaction_id = p_req->transaction_id;
            disable_res.disable_res.result = SON_SUCCESS;
            disable_res.disable_res.error_code = SON_NO_ERROR;
            son_create_send_buffer((son_u8 *)&disable_res,
                    SON_MRO_MODULE_ID, SON_MIF_MODULE_ID, SONMRO_DISABLE_RES,
                    disable_res_len);
        }
    }
    else
    {
        mro_cell_context_t *p_cell_ctxt = SON_PNULL;
        p_cell_ctxt = mro_get_cell_context_from_global_context(
                &p_req->disable_req.srv_cgi);

        if (SON_PNULL == p_cell_ctxt)
        {
            /*SPR 17777 +-*/
            SON_LOG(g_mro_gb_ctxt.log_mode,
                    p_son_mro_facility_name, SON_INFO,
                    "Cell specific "
                    "SONMRO_DISABLE_REQ received:"
                    "transaction Id %u "
                    "for unconfigured Cell 0x%x",
                    p_req->transaction_id,
                    son_convert_char_cell_id_to_int_cell_id(p_req->disable_req.srv_cgi.cell_identity));
            LTE_GCC_UNUSED_PARAM(event_id);
            /*SPR 17777 +-*/
            /* Send SONMRO_DISABLE_RES to SONMgmtIfH with result SON_FAILURE 
             * and error code = SON_ERR_CELL_UNCONFIGURED */
            disable_res.transaction_id = p_req->transaction_id;
            disable_res.disable_res.result = SON_FAILURE;
            disable_res.disable_res.error_code = SON_NO_ERROR;
            disable_res.disable_res.cell_status_list_size = 1;
            SON_MEMCPY(&disable_res.disable_res.cell_status_list[0].cgi,
                    &p_req->disable_req.srv_cgi,
                    sizeof(son_intra_rat_global_cell_id_t)); 
            disable_res.disable_res.cell_status_list[0].error_code = 
                SON_ERR_CELL_UNCONFIGURED; 
            son_create_send_buffer((son_u8 *)&disable_res,
                    SON_MRO_MODULE_ID, SON_MIF_MODULE_ID, SONMRO_DISABLE_RES,
                    disable_res_len);
        }
        else
        {
            mro_cell_process_msg(p_cspl_hdr, p_cell_ctxt, (son_u8 *)p_req);
        }
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mro_fsm_disable_req_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_modify_config_param_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  SONMRO_MODIFY_CONFIG_PARAM_REQ message received from 
 *                  SONMgmtIfH.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_modify_config_param_req_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    son_mro_modify_config_params_req_t *p_req = 
        (son_mro_modify_config_params_req_t *)p_msg;
    mro_cell_context_t *p_cell_ctxt = SON_PNULL;

    SON_UT_TRACE_ENTER();
    
    LTE_GCC_UNUSED_PARAM(event_id);
                    /*SPR 17777 +-*/

    p_cell_ctxt = mro_get_cell_context_from_global_context(
            &p_req->config_param_req.srv_cgi);

    if (SON_PNULL == p_cell_ctxt)
    {
        /*SPR 17777 +-*/
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_INFO,
                "SONMRO_MODIFY_CONFIG_PARAM_REQ received: "
                "transaction Id: %u"
                " for unconfigured Cell 0x%x",
                p_req->transaction_id,
                son_convert_char_cell_id_to_int_cell_id(p_req->config_param_req.srv_cgi.cell_identity));
        son_mro_modify_config_params_res_t config_params_res;
        son_size_t config_params_res_len = 
            sizeof(son_mro_modify_config_params_res_t);
        SON_MEMSET(&config_params_res, 0, config_params_res_len);
        config_params_res.transaction_id = p_req->transaction_id;
        SON_MEMCPY(&config_params_res.config_param_res.srv_cgi,
            &p_req->config_param_req.srv_cgi,
            sizeof(son_intra_rat_global_cell_id_t));
        config_params_res.config_param_res.result = SON_FAILURE;
        config_params_res.config_param_res.error_code = 
            SON_ERR_CELL_UNCONFIGURED;
        son_create_send_buffer((son_u8 *)&config_params_res,
                SON_MRO_MODULE_ID, SON_MIF_MODULE_ID, 
                SONMRO_MODIFY_CONFIG_PARAM_RES, config_params_res_len);
    }
    else
    {
        mro_cell_process_msg(p_cspl_hdr, p_cell_ctxt, (son_u8 *)p_req);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mro_fsm_modify_config_param_req_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_register_res_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  RRM_SON_REGISTER_RESP message received from RRM.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_register_res_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    son_rrm_registration_res_t *p_res = (son_rrm_registration_res_t *)p_msg;
    mro_cell_context_t *p_cell_ctxt = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_cell_ctxt = mro_get_cell_context_from_global_context(
            (son_intra_rat_global_cell_id_t *)(&p_res->res.
            registration_resp.id.object_id.cell_id));

    if (SON_PNULL == p_cell_ctxt)
    {
                    /*SPR 17777 +-*/
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_WARNING,
                "RRM_SON_REGISTER_RESP received "
                "for unconfigured Cell 0x%x",
                son_convert_char_cell_id_to_int_cell_id(p_res->res.registration_resp.id.object_id.cell_id.
                cell_identity));
        LTE_GCC_UNUSED_PARAM(event_id);
                    /*SPR 17777 +-*/
    }
    else
    {
        mro_cell_process_msg(p_cspl_hdr, p_cell_ctxt, (son_u8 *)p_res);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mro_fsm_register_res_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_deregister_res_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  RRM_SON_DEREGISTER_RESP message received from RRM.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_deregister_res_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    son_rrm_deregistration_res_t *p_res = (son_rrm_deregistration_res_t *)p_msg;
    mro_cell_context_t *p_cell_ctxt = SON_PNULL;

    SON_UT_TRACE_ENTER();

    SON_LOG(g_mro_gb_ctxt.log_mode,
            p_son_mro_facility_name, SON_BRIEF,
            "mro_fsm_deregister_res_handler: "
            "RRM_SON_DEREGISTER_RESP received:\n"
            "transaction Id: %u\t"
            "Cell Id: 0x%x",
            p_res->transaction_id,
            son_convert_char_cell_id_to_int_cell_id(p_res->res.deregistration_resp.id.object_id.cell_id.
            cell_identity));

    /* Rel 3.0 Cell delete support changes Start */
    /* Find the transaction Id received in De-register response in
       temporary transaction Id list stored during cell deletion.
       If found remove it from temporary transaction Id list and
       do not process the corresponding De-register response as the
       cell is already deleted */
    if (SON_SUCCESS == 
            son_find_txn_id_in_temp_txn_id_list(p_res->transaction_id))
    {
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_DETAILED,
                "mro_fsm_deregister_res_handler: RRM_SON_DEREGISTER_RESP "
                "received for the cell [Cell Id: 0x%x] already deleted "
                "at MRO due to cell delete request from OAM so ignore it",
                son_convert_char_cell_id_to_int_cell_id(
                    p_res->res.deregistration_resp.id.object_id.
                    cell_id.cell_identity));
        SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
    }
    /* Rel 3.0 Cell delete support changes Stop */

    p_cell_ctxt = mro_get_cell_context_from_global_context(
            (son_intra_rat_global_cell_id_t *)
            (&p_res->res.deregistration_resp.id.object_id.cell_id));

    if (SON_PNULL == p_cell_ctxt)
    {
                    /*SPR 17777 +-*/
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_ERROR,
                "mro_fsm_deregister_res_handler: RRM_SON_DeREGISTER_RESP "
                "received for unconfigured cell [Cell Id: 0x%x] event_id=%d",
                son_convert_char_cell_id_to_int_cell_id(p_res->res.deregistration_resp.id.object_id.
                    cell_id.cell_identity),event_id);
                    /*SPR 17777 +-*/
    }
    else
    {
        mro_cell_process_msg(p_cspl_hdr, p_cell_ctxt, (son_u8 *)p_res);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mro_fsm_deregister_res_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_mlb_nbr_cell_status_ind_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONMLB_NBR_CELL_STATUS_IND
 *                  message received from SON MLB.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_mlb_nbr_cell_status_ind_handler
(
    son_u8              *p_msg,
    STACKAPIHDR         *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    mlb_nbr_cell_status_ind_t *p_ind =
        (mlb_nbr_cell_status_ind_t *)p_msg;
    mro_cell_context_t *p_cell_ctxt = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_cell_ctxt = mro_get_cell_context_from_global_context(
        (son_intra_rat_global_cell_id_t *)(&p_ind->serv_cell_id));

    if (SON_PNULL == p_cell_ctxt)
    {
                    /*SPR 17777 +-*/
        SON_LOG(g_mro_gb_ctxt.log_mode,
            p_son_mro_facility_name, SON_WARNING,
            "SONMLB_NBR_CELL_STATUS_IND received for "
            "unconfigured Cell 0x%x",
            son_convert_char_cell_id_to_int_cell_id(p_ind->serv_cell_id.cell_identity));
        LTE_GCC_UNUSED_PARAM(event_id);
                    /*SPR 17777 +-*/
    }
    else
    {
        mro_cell_process_msg(p_cspl_hdr, p_cell_ctxt, (son_u8 *)p_ind);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mro_fsm_mlb_nbr_cell_status_ind_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_handover_report_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  RRM_SON_HO_FAILURE_REPORT message received from RRM or
 *                  SONX2_HO_REPORT_IND received from L3 X2AP.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_handover_report_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    son_ho_failure_report_t *p_report = 
        (son_ho_failure_report_t *)p_msg;
    mro_cell_context_t *p_cell_ctxt = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_cell_ctxt = mro_get_cell_context_from_global_context(
            (son_intra_rat_global_cell_id_t *)(&p_report->src_cgi));

    if (SON_PNULL == p_cell_ctxt)
    {
        if (MRO_IDX_RRM_HO_FAILURE_REPORT == event_id)
        {
            SON_LOG(g_mro_gb_ctxt.log_mode,
                    p_son_mro_facility_name, SON_INFO,
                    "RRM->MRO:RRM_SON_HO_FAILURE_REPORT "
                    "for unconfigured Cell 0x%x",
                    son_convert_char_cell_id_to_int_cell_id(p_report->src_cgi.cell_identity));
        }
        else
        {
            SON_LOG(g_mro_gb_ctxt.log_mode,
                    p_son_mro_facility_name, SON_INFO,
                    "X2->MRO:SONX2_HO_REPORT_IND "
                    "for unconfigured Cell 0x%x",
                    son_convert_char_cell_id_to_int_cell_id(p_report->src_cgi.cell_identity));
        }
    }
    else
    {
        mro_cell_process_msg(p_cspl_hdr, p_cell_ctxt, (son_u8 *)p_report);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mro_fsm_handover_report_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_handover_attempts_ind_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  RRM_SON_HO_ATTEMPT_IND message received from RRM.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_handover_attempts_ind_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    son_mro_ho_attempts_ind_t *p_ho_attempts_ind = 
        (son_mro_ho_attempts_ind_t *)p_msg;
    mro_cell_context_t *p_cell_ctxt = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_cell_ctxt = mro_get_cell_context_from_global_context(
            (son_intra_rat_global_cell_id_t *)(&p_ho_attempts_ind->src_cgi));

    if (SON_PNULL == p_cell_ctxt)
    {
                    /*SPR 17777 +-*/
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_INFO,
                "RRM->MRO:RRM_SON_HO_ATTEMPT_IND "
                "received for unconfigured Cell 0x%x",
                son_convert_char_cell_id_to_int_cell_id(p_ho_attempts_ind->src_cgi.cell_identity));
        LTE_GCC_UNUSED_PARAM(event_id);
                    /*SPR 17777 +-*/
    }
    else
    {
        mro_cell_process_msg(p_cspl_hdr, p_cell_ctxt, 
                (son_u8 *)p_ho_attempts_ind);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mro_fsm_handover_attempts_ind_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_cell_state_change_ind_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  RRM_SON_CELL_STATE_CHANGE_IND message received from RRM.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_cell_state_change_ind_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    son_cell_state_change_indication_t *p_ind = 
        (son_cell_state_change_indication_t *)p_msg;
    mro_cell_context_t *p_cell_ctxt = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_cell_ctxt = mro_get_cell_context_from_global_context(
            (son_intra_rat_global_cell_id_t *)(&p_ind->cell_id));

    if (SON_PNULL == p_cell_ctxt)
    {
                    /*SPR 17777 +-*/
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_INFO,
                "RRM->MRO:RRM_SON_CELL_STATE_CHANGE_IND for unconfigured "
                "Cell 0x%x",
                son_convert_char_cell_id_to_int_cell_id(p_ind->cell_id.cell_identity));
        LTE_GCC_UNUSED_PARAM(event_id);
                    /*SPR 17777 +-*/
    }
    else
    {
        mro_cell_process_msg(p_cspl_hdr, p_cell_ctxt, (son_u8 *)p_ind);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mro_fsm_cell_state_change_ind_handler */

/*****************************************************************************
 * Function Name  : mro_fsm_cell_anr_mode_ind_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  SONANR_DISABLE_IND message received from SON ANR.
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_cell_anr_mode_ind_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    son_anr_cell_mode_ind_t *p_ind = 
        (son_anr_cell_mode_ind_t *)p_msg;
    mro_cell_context_t *p_cell_ctxt = SON_PNULL;

    SON_UT_TRACE_ENTER();

    p_cell_ctxt = mro_get_cell_context_from_global_context(
            &p_ind->srv_cgi);

    if (SON_PNULL == p_cell_ctxt)
    {
                    /*SPR 17777 +-*/
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_INFO,
                "MIF->MRO:SONANR_DISABLE_IND for unconfigured cell "
                "Cell 0x%x",
                son_convert_char_cell_id_to_int_cell_id(p_ind->srv_cgi.cell_identity));
        LTE_GCC_UNUSED_PARAM(event_id);
                    /*SPR 17777 +-*/
    }
    else
    {
        mro_cell_process_msg(p_cspl_hdr, p_cell_ctxt, (son_u8 *)p_ind);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* mro_fsm_cell_anr_mode_ind_handler */

/* Rel 3.0 Cell delete support changes Start */
/*****************************************************************************
 * Function Name  : mro_fsm_cell_delete_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for
 *                  SMIF_CELL_DELETE_REQ message received from MIF
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_cell_delete_req_handler
(
    son_u8          *p_buf,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    smif_cell_delete_req_t   *p_msg          = (smif_cell_delete_req_t *)p_buf;
    mro_cell_context_t      *p_cell_ctxt    = SON_PNULL;
    son_u8                  idx             = 0;
    smif_cell_delete_resp_t  cell_del_res;
    cell_status_list_data_t *p_list_node = SON_PNULL;
    son_procedure_code_et api = SON_PROCEDURE_UNDEFINED;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&cell_del_res, 0, sizeof(cell_del_res));

    cell_del_res.cell_delete_status_list_size =
        p_msg->cell_delete_list_size;

    /* Traverse the cell delete list */
    for (idx = 0; idx < p_msg->cell_delete_list_size; idx ++)
    {
        /* Populate the response structure */
        SON_MEMCPY(&cell_del_res.cell_delete_status_list[idx].cgi,
                &p_msg->cell_delete_list[idx],
                sizeof(son_intra_rat_global_cell_id_t));

        cell_del_res.cell_delete_status_list[idx].result = SON_FAILURE;

        /* Find the cell in global context */
        p_cell_ctxt = mro_get_cell_context_from_global_context(
                &p_msg->cell_delete_list[idx]);

        if (SON_PNULL != p_cell_ctxt)
        {
            /* Send deregister request to RRM if cell is ENABLING/ENABLED */
            if ((MRO_CELL_STATE_ENABLING == 
                        p_cell_ctxt->current_cell_fsm_state) ||
                    (MRO_CELL_STATE_ENABLED == 
                     p_cell_ctxt->current_cell_fsm_state))
            {
                son_u16 txn_id =  son_generate_txn_id();

                /* Start: Coverity 7127170 */
                if (SON_FAILURE == mro_construct_send_rrm_deregister_req(
                            txn_id,
                            &p_cell_ctxt->cell_id ))
                {
                    cell_del_res.cell_delete_status_list[idx].result = 
                        SON_PARTIAL_SUCCESS;
                }
                else
                {
                    /* Store the transaction Id with which De-register 
                       request is sent to RRM. This shall be used later 
                       when De-register response is received for this request */
                    son_insert_txn_id_in_temp_txn_id_list(txn_id);
                }
                /* Stop: Coverity 7127170 */
            }
            /* Check the pending resp list for this cell */
            if (mro_get_pending_res_expected_count())
            {
                /* Search for the cell Id in pending response structure. */
                p_list_node = mro_find_cell_in_pending_res(&p_cell_ctxt->cell_id);

                if (SON_PNULL != p_list_node)
                {
                    /* Error code SON_ERR_TIMER_EXPIRED indicates that
                       the pending response is not received for this cell 
                       so decrement expected response count since the 
                       cell is being deleted and no response should be
                       expected for this cell. */
                    if (SON_ERR_TIMER_EXPIRED == p_list_node->cell_status.error_code)
                    {
                        mro_decrement_pending_res_expected_count();
                    }
                    api = mro_get_pending_res_api_id();
                    if (SMIF_CELL_INFO_IND == api)
                    {
                        /* Delete the cell as in case of cell deletion
                           no feature state change indication shall be send
                           and only cell delete response shall be send to 
                           SMIF */
                        mro_delete_cell_from_pending_res
                            (&p_msg->cell_delete_list[idx]);
                    }
                    else
                    {
                        /* Update Cell in MRO Pending Response List with error code
                           SON_ERR_CELL_UNCONFIGURED */
                        p_list_node->cell_status.error_code = SON_ERR_CELL_UNCONFIGURED;
                    }
                }

                /* Start Coverity Fix: 60121 */
                if (0 == mro_get_pending_res_expected_count())
                {
                    /* End Coverity Fix: 60121 */
                    mro_pending_res_timer_handler();
                    /* Set MRO global FSM state to ACTIVE */
                    SONMRO_FSM_SET_STATE(g_mro_gb_ctxt.current_fsm_state,
                            MRO_STATE_ACTIVE);
                }
            }

            /* Delete the cell from the context */
            if (SON_TRUE == mro_delete_cell_from_context
                    (&p_msg->cell_delete_list[idx]))
            {
                if (SON_PARTIAL_SUCCESS != 
                        cell_del_res.cell_delete_status_list[idx].result)
                {
                    cell_del_res.cell_delete_status_list[idx].result = 
                        SON_SUCCESS;
                }
        }
        }
    }

    /* Send cell delete resp to mif */
    son_create_send_buffer((son_u8 *)&cell_del_res,
            SON_MRO_MODULE_ID,
            SON_MIF_MODULE_ID,
            SMIF_CELL_DELETE_RESP,
            sizeof(cell_del_res));

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
}
/* Rel 3.0 Cell delete support changes Stop */

/* SPR-13251 Fix Starts */
/*****************************************************************************
 * Function Name  : mro_fsm_get_log_level_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to MRO module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for
 *                  SMIF_GET_LOG_LEVEL_REQ message received from MIF
 ****************************************************************************/
static son_buf_retain_status_et
mro_fsm_get_log_level_req_handler
(
    son_u8          *p_buf,
    STACKAPIHDR     *p_cspl_hdr,
    son_mro_index_et event_id
)
{
    smif_get_log_level_req_t *p_msg = (smif_get_log_level_req_t*)p_buf;
    smif_get_log_level_res_t res;
    SON_UT_TRACE_ENTER();

    SON_MEMSET(&res, 0, sizeof(res));
    res.transaction_id = p_msg->transaction_id;
    res.log_level = mro_get_log_level();
    
    /* Send cell delete resp to mif */
    son_create_send_buffer((son_u8 *)&res,
            SON_MRO_MODULE_ID,
            SON_MIF_MODULE_ID,
            SMIF_GET_LOG_LEVEL_RESP,
            sizeof(res));
                    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(event_id);
    LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
}
/* SPR-13251 Fix Ends */

/*****************************************************************************
 * Function Name  : mro_get_msg_idx_from_rrm_msgs
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 * Outputs        : None
 * Returns        : Index in SONMRO Global FSM
 * Description    : This function computes the index in the MRO Global FSM for 
 *                  the messages received from RRM.
 ****************************************************************************/
static son_mro_index_et
mro_get_msg_idx_from_rrm_msgs
(
    son_u16 from,
    son_u16 api
)
{
    son_mro_index_et ret = MRO_IDX_NOT_AVAILABLE;

    SON_UT_TRACE_ENTER();
    switch (api)
    {
        case RRM_SON_CELL_STATE_CHANGE_IND:
        {
            ret = MRO_IDX_RRM_EVENT_CELL_STATE_CHANGE_IND;
            break;
        }

        case RRM_SON_REGISTER_RESP:
        {
            ret = MRO_IDX_RRM_EVENT_REGISTER_RESP;
            break;
        }

        case RRM_SON_DEREGISTER_RESP:
        {
            ret = MRO_IDX_RRM_EVENT_DEREGISTER_RESP;
            break;
        }

        case RRM_SON_HO_FAILURE_REPORT:
        {
            ret = MRO_IDX_RRM_HO_FAILURE_REPORT;
            break;
        }

        case RRM_SON_HO_ATTEMPT_IND:
        {
            ret = MRO_IDX_RRM_HO_ATTEMPTS;
            break;
        }

        default:
        {
            SON_LOG(g_mro_gb_ctxt.log_mode, 
                    p_son_mro_facility_name, SON_WARNING, 
                    "Invalid API = %u received from %d", api, from);
            break;
    }
    } /* switch */

    SON_UT_TRACE_EXIT();
    return ret;
} /* mro_get_msg_idx_from_rrm_msgs */

/*****************************************************************************
 * Function Name  : mro_get_msg_idx_from_anr_msgs
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 * Outputs        : None
 * Returns        : Index in SONMRO Global FSM
 * Description    : This function computes the index in the MRO Global FSM for 
 *                  the messages received from SON ANR.
 ****************************************************************************/
static son_mro_index_et
mro_get_msg_idx_from_anr_msgs
(
    son_u16                 from,
    son_procedure_code_et   api
)
{
    son_mro_index_et ret = MRO_IDX_NOT_AVAILABLE;

    SON_UT_TRACE_ENTER();

    switch (api)
    {
        case SONANR_DISABLE_IND:
        {
            ret = MRO_IDX_SONANR_CELL_ANR_MODE_IND;
            break;
        }

        default:
        {
            SON_LOG(g_mro_gb_ctxt.log_mode, 
                    p_son_mro_facility_name, SON_WARNING, 
                    "Invalid API = %u received from %u", api, from);
            break;
    }
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* mro_get_msg_idx_from_anr_msgs */

/*****************************************************************************
 * Function Name  : mro_get_msg_idx_from_mif_msgs
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 * Outputs        : None
 * Returns        : Index in SONMRO FSM
 * Description    : This function computes the index in the MRO Global FSM for 
 *                  the messages received from SONMgmtIfH
 ****************************************************************************/
static son_mro_index_et
mro_get_msg_idx_from_mif_msgs
(
    son_u16                 from,
    son_procedure_code_et   api
)
{
    son_mro_index_et ret = MRO_IDX_NOT_AVAILABLE;

    SON_UT_TRACE_ENTER();
    switch (api)
    {
        case SMIF_INIT_CONFIG_REQ:
        {
            ret = MRO_IDX_SMIF_EVENT_INIT_CONFIG_REQ;
            break;
        }

        case SMIF_SET_LOG_LEVEL_CMD:
        {
            ret = MRO_IDX_SMIF_EVENT_SET_LOG_LEVEL_CMD;
            break;
        }

        case SMIF_LOG_ENABLE_DISABLE_CMD:
        {
            ret = MRO_IDX_SMIF_EVENT_LOG_ENABLE_DISABLE_CMD;
            break;
        }

        case SMIF_SHUTDOWN_REQ:
        {
            ret = MRO_IDX_SMIF_EVENT_SHUTDOWN_REQ;
            break;
        }

        case SMIF_CELL_INFO_IND:
        {
            ret = MRO_IDX_SMIF_EVENT_CELL_INFO_IND;
            break;
        }

        case SONMRO_ENABLE_REQ:
        {
            ret = MRO_IDX_EVENT_MRO_ENABLE_REQ;
            break;
        }

        case SONMRO_DISABLE_REQ:
        {
            ret = MRO_IDX_EVENT_MRO_DISABLE_REQ;
            break;
        }

        case SONMRO_MODIFY_CONFIG_PARAM_REQ:
        {
            ret = MRO_IDX_EVENT_MRO_MODIFY_CONFIG_PARAM_REQ;
            break;
        }

        /* Rel 3.0 Cell delete support changes Start */
        case SMIF_CELL_DELETE_REQ:
        {
            ret = MRO_IDX_SMIF_CELL_DELETE_REQ;
            break;
        }
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        case SMIF_GET_LOG_LEVEL_REQ:
        {
            ret = MRO_IDX_SMIF_GET_LOG_LEVEL_REQ;
            break;
        }
        /* SPR-13251 Fix Ends */
        default:
        {
            SON_LOG(g_mro_gb_ctxt.log_mode,
                    p_son_mro_facility_name, SON_WARNING,
                    "Invalid API = %u received from %u", api, from);
            break;
    }
    } /* switch */

    SON_UT_TRACE_EXIT();
    return  ret;
} /* mro_get_msg_idx_from_mif_msgs */

/*****************************************************************************
 * Function Name  : mro_get_msg_idx_from_mlb_msgs
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 * Outputs        : None
 * Returns        : Index in SONMRO Global FSM
 * Description    : This function computes the index in the MRO Global FSM for
 *                  the messages from SON MLB.
 ****************************************************************************/
static son_mro_index_et
mro_get_msg_idx_from_mlb_msgs
(
                    /*SPR 17777 +-*/
    son_procedure_code_et api
)
{
    son_mro_index_et ret = MRO_IDX_NOT_AVAILABLE;

    SON_UT_TRACE_ENTER();

    switch (api)
    {
        case SONMLB_NBR_CELL_STATUS_IND:
        {
            ret = MRO_IDX_MLB_NBR_CELL_STATUS_IND;
            break;
        }

        default:
        {
            SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_WARNING,
                "Invalid API = %u received from SON MLB", api);
            break;
        }
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* mro_get_msg_idx_from_mlb_msgs */

/*****************************************************************************
 * Function Name  : mro_get_msg_idx_from_x2_msgs
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 * Outputs        : None
 * Returns        : Index in SONMRO Global FSM
 * Description    : This function computes the index in the MRO Global FSM for 
 *                  the messages received from SONX2.
 ****************************************************************************/
static son_mro_index_et
mro_get_msg_idx_from_x2_msgs
(
                    /*SPR 17777 +-*/
    son_procedure_code_et   api
)
{
    son_mro_index_et ret = MRO_IDX_NOT_AVAILABLE;

    SON_UT_TRACE_ENTER();

    switch (api)
    {
        case SONX2_HO_REPORT_IND:
        {
            ret = MRO_IDX_RRC_HO_REPORT_IND;
            break;
        }

        default:
        {
            SON_LOG(g_mro_gb_ctxt.log_mode, 
                    p_son_mro_facility_name, SON_WARNING, 
                    "Invalid API = %u received from SON X2", api);
            break;
    }
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* mro_get_msg_idx_from_x2_msgs */

/*****************************************************************************
 * Function Name  : mro_calculate_msg_index
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 * Outputs        : None
 * Returns        : Index in SONMRO Global FSM
 * Description    : This function computes the index in the MRO Global FSM for 
 *                  the given procedure code
 ****************************************************************************/
static son_mro_index_et  
mro_calculate_msg_index
(
    son_u16                 from,
    son_procedure_code_et   api
)
{
    son_mro_index_et ret = MRO_IDX_NOT_AVAILABLE;

    SON_UT_TRACE_ENTER();
    
    if (SON_MIF_MODULE_ID == from)
    {
        ret = mro_get_msg_idx_from_mif_msgs(from, api);
    }
    else if (SON_ANR_MODULE_ID == from)
    {
        ret = mro_get_msg_idx_from_anr_msgs(from, api);
    }
    else if (SON_RRM_MODULE_ID == from)
    {
        ret = mro_get_msg_idx_from_rrm_msgs(from, api);
    }
    else if (SON_X2_MODULE_ID == from)
    {
                    /*SPR 17777 +-*/
        ret = mro_get_msg_idx_from_x2_msgs(api);
    }
    else if (SON_MLB_MODULE_ID == from)
    {
        ret = mro_get_msg_idx_from_mlb_msgs(api);
                    /*SPR 17777 +-*/
    }
    else
    {
        SON_LOG(g_mro_gb_ctxt.log_mode,
            p_son_mro_facility_name, SON_WARNING,
            "Message with API Id = %u received "
            "from unknown module: %u", api, from);
    }

    SON_UT_TRACE_EXIT();
    return ret;     
} /* mro_calculate_msg_index */

/************************* MRO Global State Machine*********************/
typedef son_buf_retain_status_et (*p_mro_fsm_evt_handler_t)
(
    son_u8 *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_mro_index_et event_id
);

/***********************************************************************
 * MRO Global FSM Entry Point Table
 **********************************************************************/
static p_mro_fsm_evt_handler_t 
son_mro_fsm_table[MRO_NUM_OF_STATES][MRO_NUM_OF_EVENTS] = 
{
    /* State INIT */
    {
        mro_fsm_init_config_req_handler,   
        /* SMIF_EVENT_INIT_CONFIG_REQ */
        mro_fsm_invalid_event_handler,   
        /* SMIF_EVENT_SET_LOG_LEVEL_CMD*/
        mro_fsm_invalid_event_handler,   
        /*SMIF_LOG_ENABLE_DISABLE_CMD*/
        mro_fsm_invalid_event_handler,   
        /* SMIF_EVENT_CELL_INFO_IND */
        mro_fsm_shutdown_req_handler,  
        /* SMIF_EVENT_SHUTDOWN_REQ */
        mro_fsm_nack_enable_req_handler,
        /* SONMRO_EVENT_ENABLE_REQ */
        mro_fsm_nack_disable_req_handler,
        /* SONMRO_EVENT_DISABLE_REQ */
        mro_fsm_nack_modify_config_param_req_handler,
        /* SONMRO_EVENT_MODIFY_CONFIG_PARAM_REQ */
        mro_fsm_invalid_event_handler,  
        /* RRM_EVENT_REGISTER_RES */
        mro_fsm_invalid_event_handler,  
        /* RRM_EVENT_DEREGISTER_RES*/
        mro_fsm_invalid_event_handler,  
        /* RRM_EVENT_HANDOVER_REPORT */
        mro_fsm_invalid_event_handler,  
        /* RRM_EVENT_HANDOVER_ATTEMPTS */
        mro_fsm_invalid_event_handler,  
        /* RRM_SON_CELL_STATE_CHANGE_IND */
        mro_fsm_invalid_event_handler,  
        /* SONMRO_EVENT_CELL_ANR_MODE_IND */
        mro_fsm_invalid_event_handler,  
        /* RRC_EVENT_HO_REPORT_IND */
        mro_fsm_invalid_event_handler,
        /* SONMRO_EVENT_MLB_NBR_CELL_STATUS_IND */
        /* Rel 3.0 Cell delete support changes Start */
        mro_fsm_invalid_event_handler,
        /* SMIF_CELL_DELETE_REQ */
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        mro_fsm_invalid_event_handler,
        /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    },
    /* State ACTIVE */
    {
        mro_fsm_nack_init_config_req_handler,   
        /* SMIF_EVENT_INIT_CONFIG_REQ */
        mro_fsm_set_log_level_cmd_handler,   
        /*SMIF_EVENT_SET_LOG_LEVEL_CMD*/
        mro_fsm_log_enable_disable_cmd_handler,   
        /*SMIF_LOG_ENABLE_DISABLE_CMD*/
        mro_fsm_cell_info_ind_active_state_handler,
        /* SMIF_EVENT_CELL_INFO_IND */
        mro_fsm_shutdown_req_handler,  
        /* SMIF_EVENT_SHUTDOWN_REQ */
        mro_fsm_enable_req_handler,
        /* SONMRO_EVENT_ENABLE_REQ */
        mro_fsm_disable_req_handler,
        /* SONMRO_EVENT_DISABLE_REQ */
        mro_fsm_modify_config_param_req_handler,
        /* SONMRO_EVENT_MODIFY_CONFIG_PARAM_REQ */
        mro_fsm_register_res_handler,  
        /* RRM_EVENT_REGISTER_RES */
        mro_fsm_deregister_res_handler,  
        /* RRM_EVENT_DEREGISTER_RES*/
        mro_fsm_handover_report_handler,
        /* RRM_EVENT_HANDOVER_REPORT */
        mro_fsm_handover_attempts_ind_handler,
        /* RRM_EVENT_HANDOVER_ATTEMPTS */
        mro_fsm_cell_state_change_ind_handler,
        /* RRM_SON_CELL_STATE_CHANGE_IND */
        mro_fsm_cell_anr_mode_ind_handler,
        /* SONMRO_EVENT_CELL_ANR_MODE_IND */
        mro_fsm_handover_report_handler,
        /* RRC_EVENT_HO_REPORT_IND */
        mro_fsm_mlb_nbr_cell_status_ind_handler,
        /* SONMRO_EVENT_MLB_NBR_CELL_STATUS_IND */
        /* Rel 3.0 Cell delete support changes Start */
        mro_fsm_cell_delete_req_handler,
        /* SMIF_CELL_DELETE_REQ */
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        mro_fsm_get_log_level_req_handler,
        /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    },
    /* State ENABLING */
    {
        mro_fsm_nack_init_config_req_handler,   
        /* SMIF_EVENT_INIT_CONFIG_REQ */
        mro_fsm_set_log_level_cmd_handler,   
        /*SMIF_EVENT_SET_LOG_LEVEL_CMD*/
        mro_fsm_log_enable_disable_cmd_handler,   
        /*SMIF_LOG_ENABLE_DISABLE_CMD*/
        mro_fsm_cell_info_ind_enabling_state_handler,   
        /* SMIF_EVENT_CELL_INFO_IND */
        mro_fsm_shutdown_req_handler,  
        /* SMIF_EVENT_SHUTDOWN_REQ */
        mro_fsm_nack_enable_req_handler,
        /* SONMRO_EVENT_ENABLE_REQ */
        mro_fsm_nack_disable_req_handler,
        /* SONMRO_EVENT_DISABLE_REQ */
        mro_fsm_modify_config_param_req_handler,
        /* SONMRO_EVENT_MODIFY_CONFIG_PARAM_REQ */
        mro_fsm_register_res_handler,  
        /* RRM_EVENT_REGISTER_RES */
        mro_fsm_invalid_event_handler,   
        /* RRM_EVENT_DEREGISTER_RES*/
        mro_fsm_handover_report_handler,
        /* RRM_EVENT_HANDOVER_REPORT */
        mro_fsm_handover_attempts_ind_handler,
        /* RRM_EVENT_HANDOVER_ATTEMPTS */
        mro_fsm_cell_state_change_ind_handler,
        /* RRM_SON_CELL_STATE_CHANGE_IND */
        mro_fsm_cell_anr_mode_ind_handler,
        /* SONMRO_EVENT_CELL_ANR_MODE_IND */
        mro_fsm_handover_report_handler,
        /* RRC_EVENT_HO_REPORT_IND */
        mro_fsm_mlb_nbr_cell_status_ind_handler,
        /* SONMRO_EVENT_MLB_NBR_CELL_STATUS_IND */
        /* Rel 3.0 Cell delete support changes Start */
        mro_fsm_cell_delete_req_handler,
        /* SMIF_CELL_DELETE_REQ */
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        mro_fsm_get_log_level_req_handler,
        /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    },
    /* State DISABLING */
    {
        mro_fsm_nack_init_config_req_handler,   
        /* SMIF_EVENT_INIT_CONFIG_REQ */
        mro_fsm_set_log_level_cmd_handler,   
        /* SMIF_EVENT_SET_LOG_LEVEL_CMD*/
        mro_fsm_log_enable_disable_cmd_handler,   
        /*SMIF_LOG_ENABLE_DISABLE_CMD*/
        mro_fsm_cell_info_ind_disabling_state_handler,   
        /* SMIF_EVENT_CELL_INFO_IND */
        mro_fsm_shutdown_req_handler,  
        /* SMIF_EVENT_SHUTDOWN_REQ */
        mro_fsm_nack_enable_req_handler,
        /* SONMRO_EVENT_ENABLE_REQ */
        mro_fsm_nack_disable_req_handler,
        /* SONMRO_EVENT_DISABLE_REQ */
        mro_fsm_modify_config_param_req_handler,
        /* SONMRO_EVENT_MODIFY_CONFIG_PARAM_REQ */
        mro_fsm_invalid_event_handler,   
        /* RRM_EVENT_REGISTER_RES */
        mro_fsm_deregister_res_handler,  
        /* RRM_EVENT_DEREGISTER_RES*/
        mro_fsm_handover_report_handler,
        /* RRM_EVENT_HANDOVER_REPORT */
        mro_fsm_handover_attempts_ind_handler,
        /* RRM_EVENT_HANDOVER_ATTEMPTS */
        mro_fsm_cell_state_change_ind_handler,
        /* RRM_SON_CELL_STATE_CHANGE_IND */
        mro_fsm_cell_anr_mode_ind_handler,
        /* SONMRO_EVENT_CELL_ANR_MODE_IND */
        mro_fsm_handover_report_handler,
        /* RRC_EVENT_HO_REPORT_IND */
        mro_fsm_mlb_nbr_cell_status_ind_handler,
        /* SONMRO_EVENT_MLB_NBR_CELL_STATUS_IND */
        /* Rel 3.0 Cell delete support changes Start */
        mro_fsm_cell_delete_req_handler,
        /* SMIF_CELL_DELETE_REQ */
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        mro_fsm_get_log_level_req_handler,
        /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    },
    /* State SHUTDOWN */
    {
        mro_fsm_shutdown_state_handler,   
        /* SMIF_EVENT_INIT_CONFIG_REQ */
        mro_fsm_shutdown_state_handler,   
        /*SMIF_EVENT_SET_LOG_LEVEL_CMD*/
        mro_fsm_shutdown_state_handler,   
        /*SMIF_LOG_ENABLE_DISABLE_CMD*/
        mro_fsm_shutdown_state_handler,   
        /* SMIF_EVENT_CELL_INFO_IND */
        mro_fsm_shutdown_state_handler,   
        /* SMIF_EVENT_SHUTDOWN_REQ */
        mro_fsm_shutdown_state_handler,   
        /* SONMRO_EVENT_ENABLE_REQ */
        mro_fsm_shutdown_state_handler,   
        /* SONMRO_EVENT_DISABLE_REQ */
        mro_fsm_shutdown_state_handler,   
        /* SONMRO_EVENT_MODIFY_CONFIG_PARAM_REQ */
        mro_fsm_shutdown_state_handler,   
        /* RRM_EVENT_REGISTER_RES */
        mro_fsm_shutdown_state_handler,   
        /* RRM_EVENT_DEREGISTER_RES*/
        mro_fsm_shutdown_state_handler,   
        /* RRM_EVENT_HANDOVER_REPORT */
        mro_fsm_shutdown_state_handler,   
        /* RRM_EVENT_HANDOVER_ATTEMPTS */
        mro_fsm_shutdown_state_handler,
        /* RRM_SON_CELL_STATE_CHANGE_IND */
        mro_fsm_shutdown_state_handler,
        /* SONMRO_EVENT_CELL_ANR_MODE_IND */
        mro_fsm_shutdown_state_handler,
        /* RRC_EVENT_HO_REPORT_IND */
        mro_fsm_shutdown_state_handler,
        /* SONMRO_EVENT_MLB_NBR_CELL_STATUS_IND */
        /* Rel 3.0 Cell delete support changes Start */
        /* SPR-13251 Fix Starts */
        mro_fsm_shutdown_state_handler,
        /* SMIF_CELL_DELETE_REQ */
        /* Rel 3.0 Cell delete support changes Stop */
        mro_fsm_shutdown_state_handler
        /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    }
};

/****************************************************************************
 * Function Name  : mro_parse_registration_res
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the register response
 *                  message received from RRM
 ****************************************************************************/
static son_u8 *
mro_parse_registration_res
(
    son_u8  *p_msg,
    son_u16 buf_len
    /*SPR 17777 +-*/
)
{
    son_u8 *p_parsed_msg = SON_PNULL;
    S32 length_read = 0;

    SON_UT_TRACE_ENTER();

    p_parsed_msg = son_mem_get(
            sizeof(son_rrm_registration_res_t));
    if (SON_PNULL != p_parsed_msg)
    {
        if (RRM_FAILURE == rrm_parse_rrm_son_registration_res(
                &((son_rrm_registration_res_t *)p_parsed_msg)->res,
                (p_msg + EXT_MSG_API_HDR_SIZE),
                buf_len - EXT_MSG_API_HDR_SIZE,
                &length_read))
        {
            SON_LOG(g_mro_gb_ctxt.log_mode,
                    p_son_mro_facility_name, SON_ERROR,
                    "RRM->MRO:RRM_SON_REGISTER_RESP parsing failed");
            /* Free the memory when parsing is failed */
            son_mem_free(p_parsed_msg);
            p_parsed_msg = SON_PNULL;
        }
        else 
        {
            *(son_u16 *)p_parsed_msg = son_get_word_from_header(
                    &p_msg[EXT_MSG_TRANSACTION_ID_OFFSET]);
        }
    }

    SON_UT_TRACE_EXIT();
    return p_parsed_msg;
} /* mro_parse_registration_res */

/****************************************************************************
 * Function Name  : mro_parse_deregistration_res
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the deregister response
 *                  message received from RRM
 ****************************************************************************/
static son_u8 *
mro_parse_deregistration_res
(
    son_u8  *p_msg,
    son_u16 buf_len
    /*SPR 17777 +-*/
)
{
    son_u8 *p_parsed_msg = SON_PNULL;
    S32 length_read = 0;

    SON_UT_TRACE_ENTER();

    p_parsed_msg = son_mem_get(
            sizeof(son_rrm_deregistration_res_t));
    if (SON_PNULL != p_parsed_msg)
    {
        if (RRM_FAILURE == rrm_parse_rrm_son_deregistration_res(
                &((son_rrm_deregistration_res_t *)p_parsed_msg)->res,
                (p_msg + EXT_MSG_API_HDR_SIZE),
                buf_len - EXT_MSG_API_HDR_SIZE,
                &length_read))
        {
            SON_LOG(g_mro_gb_ctxt.log_mode,
                    p_son_mro_facility_name, SON_ERROR,
                    "RRM->MRO:RRM_SON_DEREGISTER_RESP parsing failed");
            /* Free the memory when parsing is failed */
            son_mem_free(p_parsed_msg);
            p_parsed_msg = SON_PNULL;
        }
        else 
        {
            *(son_u16 *)p_parsed_msg = son_get_word_from_header(
                    &p_msg[EXT_MSG_TRANSACTION_ID_OFFSET]);
        }
    }

    SON_UT_TRACE_EXIT();
    return p_parsed_msg;
} /* mro_parse_deregistration_res */

/****************************************************************************
 * Function Name  : mro_parse_cell_state_change_ind
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the cell state change indicaiton
 *                  message received from RRM
 ****************************************************************************/
static son_u8 *
mro_parse_cell_state_change_ind
(
    son_u8  *p_msg,
    son_u16 buf_len
    /*SPR 17777 +-*/
)
{
    son_u8 *p_parsed_msg = SON_PNULL;
    S32 length_read = 0;

    SON_UT_TRACE_ENTER();

    p_parsed_msg = son_mem_get(sizeof(son_cell_state_change_indication_t));
    if (SON_PNULL != p_parsed_msg && 
            RRM_FAILURE == rrm_parse_rrm_son_cell_state_change_ind(
                (son_cell_state_change_indication_t *)p_parsed_msg,
                (p_msg + EXT_MSG_API_HDR_SIZE),
                buf_len - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_ERROR,
                "RRM->MRO:RRM_SON_CELL_STATE_CHANGE_IND parsing failed");
        /* Free the memory when parsing is failed */
        son_mem_free(p_parsed_msg);
        p_parsed_msg = SON_PNULL;
    }

    SON_UT_TRACE_EXIT();
    return p_parsed_msg;
} /* mro_parse_cell_state_change_ind */

/****************************************************************************
 * Function Name  : mro_parse_ho_failure_report
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the HO Failure Report
 *                  message received from RRM
 ****************************************************************************/
static son_u8 *
mro_parse_ho_failure_report
(
    son_u8  *p_msg,
    son_u16 buf_len
    /*SPR 17777 +-*/
)
{
    son_u8 *p_parsed_msg = SON_PNULL;
    S32 length_read = 0;

    SON_UT_TRACE_ENTER();

    p_parsed_msg = son_mem_get(sizeof(son_ho_failure_report_t));
    if (SON_PNULL != p_parsed_msg && 
            RRM_FAILURE == rrm_parse_rrm_son_ho_failure_report(
                (son_ho_failure_report_t *)p_parsed_msg,
                (p_msg + EXT_MSG_API_HDR_SIZE),
                buf_len - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_ERROR,
                "RRM->MRO:RRM_SON_HO_FAILURE_REPORT parsing failed");
        /* Free the memory when parsing is failed */
        son_mem_free(p_parsed_msg);
        p_parsed_msg = SON_PNULL;
    }

    SON_UT_TRACE_EXIT();
    return p_parsed_msg;
} /* mro_parse_ho_failure_report */

/****************************************************************************
 * Function Name  : mro_parse_ho_attempts
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the HO Attempts
 *                  message received from RRM
 ****************************************************************************/
static son_u8 *
mro_parse_ho_attempts
(
    son_u8  *p_msg,
    son_u16 buf_len
    /*SPR 17777 +-*/
)
{
    son_u8 *p_parsed_msg = SON_PNULL;
    S32 length_read = 0;

    SON_UT_TRACE_ENTER();

    p_parsed_msg = son_mem_get(sizeof(son_mro_ho_attempts_ind_t));
    if (SON_PNULL != p_parsed_msg && 
            RRM_FAILURE == rrm_parse_rrm_son_ho_attempt_ind(
                (son_mro_ho_attempts_ind_t *)p_parsed_msg,
                (p_msg + EXT_MSG_API_HDR_SIZE),
                buf_len - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_mro_gb_ctxt.log_mode,
                p_son_mro_facility_name, SON_ERROR,
                "RRM->MRO:RRM_SON_HO_ATTEMPT_IND parsing failed");
        /* Free the memory when parsing is failed */
        son_mem_free(p_parsed_msg);
        p_parsed_msg = SON_PNULL;
    }

    SON_UT_TRACE_EXIT();
    return p_parsed_msg;
} /* mro_parse_ho_attempts */

/*****************************************************************************
 * Function Name  : mro_parse_rrm_msgs
 * Inputs         : p_msg  - Pointer to the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the messages received from
 *                  RRM
 ****************************************************************************/
static son_u8 *
mro_parse_rrm_msgs
(
    son_u8 *p_msg,
    son_u16 api_id
)
{
    son_u8 *p_parsed_msg = SON_PNULL;
    son_u16 buf_len = 0;

    SON_UT_TRACE_ENTER();

    buf_len = son_get_word_from_header(&p_msg[EXT_MSG_BUF_LEN_OFFSET]);

    switch (api_id)
    {
        case RRM_SON_CELL_STATE_CHANGE_IND:
            {
                /*SPR 17777 +-*/
                p_parsed_msg = mro_parse_cell_state_change_ind(p_msg, buf_len);
            break;
        }

        case RRM_SON_REGISTER_RESP:
            {
                p_parsed_msg = mro_parse_registration_res(p_msg, buf_len);
            break;
        }

        case RRM_SON_DEREGISTER_RESP:
            {
                p_parsed_msg = mro_parse_deregistration_res(p_msg, buf_len);
            break;
        }

        case RRM_SON_HO_FAILURE_REPORT:
            {
                p_parsed_msg = mro_parse_ho_failure_report(p_msg, buf_len);
            break;
        }

        case RRM_SON_HO_ATTEMPT_IND:
            {
                p_parsed_msg = mro_parse_ho_attempts(p_msg, buf_len);
                /*SPR 17777 +-*/
            break;
        }

        default:
        {
            SON_LOG(g_mro_gb_ctxt.log_mode,
                    p_son_mro_facility_name, SON_ERROR, 
                    "Unexpected Message: %u "
                    "received from RRM", api_id);
            break;
    }
    } /* switch */

    SON_UT_TRACE_EXIT();
    return p_parsed_msg;
} /* mro_parse_rrm_msgs */

/*****************************************************************************
 * Function Name  : son_mro_process_event
 * Inputs         : p_msg - Pointer to the message received
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This function is used to invoke the appropriate event
 *                  handler for the messages received by SON MRO. It
 *                  calculates the API Id, finds current FSM state and based
 *                  on this access the son_mro_fsm_table to call the
 *                  appropriate handler.
 ****************************************************************************/
son_buf_retain_status_et
son_mro_process_event
(
    son_void_t *p_msg
)
{
    son_buf_retain_status_et buf_retain_status = RELEASE_SON_BUFFER;
    son_mro_index_et idx = MRO_IDX_NOT_AVAILABLE;
    STACKAPIHDR cspl_hdr = {0};
    son_u8 *p_parsed_msg = SON_PNULL;

    SON_UT_TRACE_ENTER();
    
    /* Parse the cspl header */
    son_parse_cspl_header(p_msg, &cspl_hdr);

    /* Calculate global FSM index */
    idx = mro_calculate_msg_index(cspl_hdr.from, 
            (son_procedure_code_et)cspl_hdr.api);

    /* Validate the index of son_mro_fsm_table array */
    if (MRO_NUM_OF_EVENTS <= idx)
    {
        /* No need to print the log here for invalid API Id as it is
         * already printed in mro_calculate_msg_index function */
        /* Do nothing */
    }
    else if (MRO_NUM_OF_STATES <=
        g_mro_gb_ctxt.current_fsm_state)
    {
        SON_LOG(g_mro_gb_ctxt.log_mode,
            p_son_mro_facility_name, SON_WARNING,
            "API Id %u received in "
            "invalid state = %u", cspl_hdr.api,
            g_mro_gb_ctxt.current_fsm_state);
    }
    else
    {
        /* Move the pointer such that it points to the message data after the
         * CSPL header */
        p_msg += CV_HDRSIZE;

        /* Parse the incoming message received from RRM */
        if (SON_RRM_MODULE_ID == cspl_hdr.from)
        {
            /* Parse message received from RRM */
            p_parsed_msg = mro_parse_rrm_msgs(p_msg,
                    cspl_hdr.api);

            if (SON_PNULL != p_parsed_msg)
            {
                p_msg = p_parsed_msg;
            }
            else
            {
                SON_UT_TRACE_EXIT();
                return buf_retain_status;
            }
        }
        
        /* Call the appropriate message handler based on the API Id and
         * source module id */
        SON_LOG(g_mro_gb_ctxt.log_mode,
            p_son_mro_facility_name, SON_INFO,
            "### MRO Global FSM Processing State %s Event %u ###",
            SON_MRO_FSM_STATES_NAMES[g_mro_gb_ctxt.current_fsm_state],
            idx);

        buf_retain_status = 
            (*son_mro_fsm_table[g_mro_gb_ctxt.current_fsm_state][idx])
            (p_msg, &cspl_hdr, idx);

        SON_LOG(g_mro_gb_ctxt.log_mode,
            p_son_mro_facility_name, SON_INFO,
            "### MRO Global FSM Exit ###");

        /* Free the memory taken for message received from RRM */
        if (SON_PNULL != p_parsed_msg)
        {
            son_mem_free(p_parsed_msg);
        }
    }

    SON_UT_TRACE_EXIT();
    return buf_retain_status;
} /* son_mro_process_event */
