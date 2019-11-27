/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: son_rach_opt_fsm.c $
 *
 *******************************************************************************
 *
 * File Description: This file contains the functions for handling RACH OPT global
 *      level messages and functions for manifest structure of this module.
 *
 *******************************************************************************
 * Revision Details
 *----------------------
 *   DATE            AUTHOR       REFERENCE       REASON
 *   Aug, 2013       SON Team     Initial         Initial
 *   Aug, 2014       Shilpi                       SPR 13251 Fix
 ******************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_rach_opt_ctxt_mgr.h>
#include <son_rach_opt_cell_fsm.h>
#include <son_rach_opt_fsm.h>
#include <son_rach_opt_intf.h>
#include <son_anr_intf.h>
#include <rrm_son_composer.h>
#include <rrm_son_parser.h>
#include <son_anr_ctxt_mgr.h>

/****************************************************************************
 * Global Variables
 ****************************************************************************/
extern rach_opt_global_context_t g_rach_opt_gb_ctxt;
extern const son_8 *p_son_rach_opt_facility_name;
extern const son_s8 *SON_RACH_OPT_FSM_EVENT_NAMES[];
extern const son_s8 *SON_RACH_OPT_FSM_STATES_NAMES[];
/* Rel 3.0 Cell delete support changes Start */
extern pending_res_t g_rach_opt_pending_response;
/* Rel 3.0 Cell delete support changes Stop */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_invalid_event_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT Module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for the messages received
 *                  in invalid states for which nothing needs to be done.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_invalid_event_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    SON_UT_TRACE_ENTER();

    SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
            SON_WARNING,
            "Invalid Event %s occured "
                    /*SPR 17777 +-*/
            "in %s state from %u msg %u",
            SON_RACH_OPT_FSM_EVENT_NAMES[event_id],
            SON_RACH_OPT_FSM_STATES_NAMES[g_rach_opt_gb_ctxt.current_fsm_state],
            p_cspl_hdr->from,*p_msg);
                    /*SPR 17777 +-*/

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_invalid_event_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_init_config_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH 
                    OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_INIT_CONFIG_REQ
 *                  message received from SONMgmtIfH in RACH_OPT_STATE_INIT 
 *                  state.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_init_config_req_handler
(
    son_u8                  *p_buf,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    smif_init_config_req_t *p_msg = (smif_init_config_req_t *)p_buf;
    smif_init_config_res_t init_config_res = {0};
    SON_UT_TRACE_ENTER();

    SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
            SON_BRIEF,
            "SMIF_INIT_CONFIG_REQ "
                    /*SPR 17777 +-*/
            "received: trans Id: %u , log enable: %u , log level: %u"
            "event %u %u",
            p_msg->transaction_id,
            (son_u16)p_msg->log_enable,
            (son_u16)p_msg->log_level,event_id,sizeof(p_cspl_hdr));
                    /*SPR 17777 +-*/
    rach_opt_set_log_mode((son_oam_log_on_off_et)(p_msg->log_enable));

    if (SON_OAM_LOG_ON == g_rach_opt_gb_ctxt.log_mode)
    {
        /* SPR-13251 Fix Starts */
        rach_opt_set_log_level(p_msg->log_level);
        /* SPR-13251 Fix Ends */
        SET_MODULE_LOG_LEVEL(p_msg->log_level);
    }
    else
    {
        /* SPR-13251 Fix Starts */
        rach_opt_set_log_level(SON_LOG_LEVEL_NONE);
        /* SPR-13251 Fix Ends */
        /* Logging is disabled so set log level to NONE  */
        SET_MODULE_LOG_LEVEL(SON_LOG_LEVEL_NONE);
    }

    init_config_res.transaction_id = p_msg->transaction_id;
    init_config_res.result = SON_SUCCESS;
    init_config_res.error_code = SON_NO_ERROR;

    son_create_send_buffer((son_u8*)&init_config_res,
            SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, SMIF_INIT_CONFIG_RES,
            sizeof(smif_init_config_res_t));

    /* Set RACH OPT global FSM state to ACTIVE */
    SONRACH_OPT_FSM_SET_STATE(g_rach_opt_gb_ctxt.current_fsm_state,
        RACH_OPT_STATE_ACTIVE);

    SON_UT_TRACE_EXIT();
    /* Release the buffer memory as it is no longer needed. */
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_init_config_req_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_nack_init_config_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to 
                    RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function to send negative response
 *                  for SMIF_INIT_CONFIG_REQ message received from SONMgmtIfH
 *                  in any state other than RACH_OPT_STATE_INIT state.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_nack_init_config_req_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_rach_opt_index_et event_id
)
{
    smif_init_config_res_t smif_init_config_res = {0};
    smif_init_config_req_t *p_smif_init_config_req =
        (smif_init_config_req_t *)p_msg;
    SON_UT_TRACE_ENTER();

    SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
            SON_ERROR,
            "SMIF_INIT_CONFIG_REQ "
            "received in invalid state %s:"
                    /*SPR 17777 +-*/
            "transaction Id: %u log enable: %u log level: %u event %u %u",
            SON_RACH_OPT_FSM_STATES_NAMES[g_rach_opt_gb_ctxt.current_fsm_state],
            p_smif_init_config_req->transaction_id,
            (son_u16)p_smif_init_config_req->log_enable,
            (son_u16)p_smif_init_config_req->log_level,
            event_id,sizeof(p_cspl_hdr));
                    /*SPR 17777 +-*/

    /* Populate the init config response structure */
    smif_init_config_res.transaction_id =
        p_smif_init_config_req->transaction_id;
    smif_init_config_res.result = SON_FAILURE;
    smif_init_config_res.error_code = SON_ERR_UNEXPECTED_MSG;

    /* Sends SMIF_INIT_CONFIG_RES to SONMgmtIfh */
    son_create_send_buffer((son_u8 *)&smif_init_config_res,
            SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, SMIF_INIT_CONFIG_RES,
            sizeof(smif_init_config_res_t));

    SON_UT_TRACE_EXIT();
    /* Release the buffer memory as it is no longer needed. */
    return RELEASE_SON_BUFFER;
}

/*****************************************************************************
 * Function Name  : rach_opt_fsm_nack_enable_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT
 *                  module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function to send nack for 
 *                  SONRACH_OPT_ENABLE_REQ message received from SONMgmtIfH.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_nack_enable_req_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    son_rach_opt_enable_req_t *p_req = (son_rach_opt_enable_req_t *)p_msg;
    son_rach_opt_enable_res_t enable_res;
    son_size_t enable_res_len = sizeof(son_rach_opt_enable_res_t);

    SON_UT_TRACE_ENTER();

    if (SON_RACH_OPT_ENABLE_SRV_CGI_PRESENT & p_req->enable_req.bitmask)
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                SON_ERROR,
                "Cell specific "
                "SONRACH_OPT_ENABLE_REQ received in invalid state %s:"
                "transaction Id: %u",
                SON_RACH_OPT_FSM_STATES_NAMES[
                g_rach_opt_gb_ctxt.current_fsm_state],
                p_req->transaction_id);
    }
    else
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                SON_ERROR,
                "Global "
                "SONRACH_OPT_ENABLE_REQ received in invalid state %s:"
                    /*SPR 17777 +-*/
                "transaction Id: %u event %u %u",
                SON_RACH_OPT_FSM_STATES_NAMES[
                g_rach_opt_gb_ctxt.current_fsm_state],
                p_req->transaction_id,event_id,sizeof(p_cspl_hdr));
                    /*SPR 17777 +-*/
    }

    SON_MEMSET(&enable_res, 0, enable_res_len);
    enable_res.transaction_id = p_req->transaction_id;
    enable_res.enable_res.result = SON_FAILURE;
    if (RACH_OPT_STATE_INIT == g_rach_opt_gb_ctxt.current_fsm_state)
    {
        enable_res.enable_res.error_code = SON_ERR_UNEXPECTED_MSG;
    }
    else if (RACH_OPT_STATE_ENABLING == g_rach_opt_gb_ctxt.current_fsm_state)
    {
        enable_res.enable_res.error_code = SON_ERR_ENABLE_IN_PROGRESS;
    }
    else 
    {
        enable_res.enable_res.error_code = SON_ERR_DISABLE_IN_PROGRESS;
    }

    son_create_send_buffer((son_u8 *)&enable_res,
            SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, SONRACH_OPT_ENABLE_RES,
            enable_res_len);

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_nack_enable_req_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_nack_disable_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT 
 *                  module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function to send nack for 
 *                  SONRACH_OPT_DISABLE_REQ message received from SONMgmtIfH.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_nack_disable_req_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    son_rach_opt_disable_req_t *p_req = (son_rach_opt_disable_req_t *)p_msg;
    son_rach_opt_disable_res_t disable_res;
    son_size_t disable_res_len = sizeof(son_rach_opt_disable_res_t);

    SON_UT_TRACE_ENTER();

    if (SON_RACH_OPT_DISABLE_SRV_CGI_PRESENT & p_req->disable_req.bitmask)
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                SON_ERROR,
                "Cell specific "
                "SONRACH_OPT_DISABLE_REQ received in invalid state %s:"
                "transaction Id: %u",
                SON_RACH_OPT_FSM_STATES_NAMES[
                g_rach_opt_gb_ctxt.current_fsm_state],
                p_req->transaction_id);
    }
    else
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                SON_ERROR,
                "Global "
                "SONRACH_OPT_DISABLE_REQ received in invalid state %s:"
                    /*SPR 17777 +-*/
                "transaction Id: %u event %u %u",
                SON_RACH_OPT_FSM_STATES_NAMES[
                g_rach_opt_gb_ctxt.current_fsm_state],
                p_req->transaction_id,event_id,sizeof(p_cspl_hdr));
                    /*SPR 17777 +-*/
    }

    SON_MEMSET(&disable_res, 0, disable_res_len);
    disable_res.transaction_id = p_req->transaction_id;
    disable_res.disable_res.result = SON_FAILURE;
    if (RACH_OPT_STATE_INIT == g_rach_opt_gb_ctxt.current_fsm_state)
    {
        disable_res.disable_res.error_code = SON_ERR_UNEXPECTED_MSG;
    }
    else if (RACH_OPT_STATE_ENABLING == g_rach_opt_gb_ctxt.current_fsm_state)
    {
        disable_res.disable_res.error_code = SON_ERR_ENABLE_IN_PROGRESS;
    }
    else 
    {
        disable_res.disable_res.error_code = SON_ERR_DISABLE_IN_PROGRESS;
    }

    son_create_send_buffer((son_u8 *)&disable_res,
            SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, SONRACH_OPT_DISABLE_RES,
            disable_res_len);

    SON_UT_TRACE_EXIT();
        return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_nack_disable_req_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_nack_modify_config_param_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function to send nack for 
 *                  SONRACH_OPT_MODIFY_CONFIG_PARAMS_REQ message received from 
 *                  SONMgmtIfH.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_nack_modify_config_param_req_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    son_rach_opt_modify_config_params_req_t *p_req = 
        (son_rach_opt_modify_config_params_req_t *)p_msg;
    son_rach_opt_modify_config_params_res_t config_param_res;
    son_size_t config_param_res_len = 
        sizeof(son_rach_opt_modify_config_params_res_t);

    SON_UT_TRACE_ENTER();

    SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
            SON_ERROR,
            "SONRACH_OPT_MODIFY_CONFIG_PARAMS_REQ received in invalid state"
                    /*SPR 17777 +-*/
            " %s:transaction Id: %u event%u %u",
            SON_RACH_OPT_FSM_STATES_NAMES[
            g_rach_opt_gb_ctxt.current_fsm_state],
            p_req->transaction_id,event_id,sizeof(p_cspl_hdr));
                    /*SPR 17777 +-*/

    SON_MEMSET(&config_param_res, 0, config_param_res_len);
    SON_MEMCPY(&config_param_res.modify_res.srv_cgi,
            &p_req->modify_req.srv_cgi,
            sizeof(son_intra_rat_global_cell_id_t));
    config_param_res.transaction_id = p_req->transaction_id;
    config_param_res.modify_res.result = SON_FAILURE;
    config_param_res.modify_res.error_code = SON_ERR_UNEXPECTED_MSG;
    config_param_res.modify_res.failed_param_bitmask = 
        p_req->modify_req.rach_config_params.bitmask;

    son_create_send_buffer((son_u8 *)&config_param_res,
            SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, 
            SONRACH_OPT_MODIFY_CONFIG_PARAMS_RES, config_param_res_len);

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_nack_modify_config_param_req_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_nack_modify_rach_optmode_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function to send nack for 
 *                  SONRACH_OPT_MODIFY_RACH_OPTMODE_REQ message received from 
 *                  SONMgmtIfH.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_nack_modify_rach_optmode_req_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    son_rach_opt_modify_rach_optmode_req_t *p_req = 
        (son_rach_opt_modify_rach_optmode_req_t *)p_msg;
    son_rach_opt_modify_rach_optmode_res_t res;
    son_size_t res_len = 
        sizeof(son_rach_opt_modify_rach_optmode_res_t);

    SON_UT_TRACE_ENTER();

    SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
            SON_ERROR,
            "SONRACH_OPT_MODIFY_RACH_OPTMODE_REQ received in invalid state"
                    /*SPR 17777 +-*/
            " %s:transaction Id: %u event %u %u",
            SON_RACH_OPT_FSM_STATES_NAMES[
            g_rach_opt_gb_ctxt.current_fsm_state],
            p_req->transaction_id,event_id,sizeof(p_cspl_hdr));
                    /*SPR 17777 +-*/

    SON_MEMSET(&res, 0, res_len);
    res.transaction_id = p_req->transaction_id;
    res.modify_mode_res.result = SON_FAILURE;
    res.modify_mode_res.error_code = SON_ERR_UNEXPECTED_MSG;
    son_create_send_buffer((son_u8 *)&res,
            SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, 
            SONRACH_OPT_MODIFY_RACH_OPTMODE_RES, res_len);

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_nack_modify_rach_optmode_req_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_set_log_level_cmd_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for
 *                  SMIF_EVENT_SET_LOG_LEVEL_CMD message received from
 *                  SONMgmtIfH in state other than RACH_OPT_STATE_INIT and
 *                  RACH_OPT_STATE_SHUTDOWN state.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_set_log_level_cmd_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    smif_set_log_level_cmd_t *p_cmd =
        (smif_set_log_level_cmd_t *)p_msg;
    SON_UT_TRACE_ENTER();

    if (SON_OAM_LOG_ON == g_rach_opt_gb_ctxt.log_mode)
    {
        SON_LOG(SON_OAM_LOG_ON, p_son_rach_opt_facility_name,
            SON_BRIEF,
            "Log level "
                    /*SPR 17777 +-*/
            "changed to %u through msg: SMIF_SET_LOG_LEVEL_CMD"
            "event %u %u",
            (son_u8)p_cmd->log_level,event_id,sizeof(p_cspl_hdr));
                    /*SPR 17777 +-*/
        /* SPR-13251 Fix Starts */
        rach_opt_set_log_level(p_cmd->log_level);
        /* SPR-13251 Fix Ends */
        SET_MODULE_LOG_LEVEL(p_cmd->log_level);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_set_log_level_cmd_handler */


/*****************************************************************************
 * Function Name  : rach_opt_fsm_log_enable_disable_cmd_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for
 *                  SMIF_EVENT_LOG_ENABLE_DISABLE_CMD message received from
 *                  SONMgmtIfH in state other than RACH_OPT_STATE_INIT and
 *                  RACH_OPT_STATE_SHUTDOWN state.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_log_enable_disable_cmd_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    smif_log_enable_disable_cmd_t *p_cmd =
        (smif_log_enable_disable_cmd_t *)p_msg;
    SON_UT_TRACE_ENTER();

    rach_opt_set_log_mode((son_oam_log_on_off_et)(p_cmd->log_enable));
    /* Print the current logging status(enable/disable)
        SON_TRUE: always print log, even if it is disabled. */
    SON_LOG(SON_OAM_LOG_ON, p_son_rach_opt_facility_name,
            SON_BRIEF,
            "Log enable/disable"
            " %u", p_cmd->log_enable);

    if (SON_OAM_LOG_ON == g_rach_opt_gb_ctxt.log_mode)
    {
        if (SMIF_MODULE_LOG_LEVEL_PRESENT & p_cmd->bitmask)
        {
            /* SPR-13251 Fix Starts */
            rach_opt_set_log_level(p_cmd->log_level);
            /* SPR-13251 Fix Ends */
            SET_MODULE_LOG_LEVEL(p_cmd->log_level);
        }
        else
        {
            SON_LOG(SON_OAM_LOG_ON, p_son_rach_opt_facility_name,
                    SON_BRIEF, 
                    "Log level not provided in the SMIF_LOG_ENABLE_DISABLE_CMD,"
                    /*SPR 17777 +-*/
                    " so log level is set to SON_OAM_LOG_LEVEL_BRIEF");
                    LTE_GCC_UNUSED_PARAM(event_id);
                    LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
            /* SPR-13251 Fix Starts */
            rach_opt_set_log_level(SON_OAM_LOG_LEVEL_BRIEF);
            /* SPR-13251 Fix Ends */
            /* Set log level to Default (Brief) value since no log level
                provided in the request message */
            SET_MODULE_LOG_LEVEL(SON_OAM_LOG_LEVEL_BRIEF);
        }
    }
    else
    {
        /* SPR-13251 Fix Starts */
        rach_opt_set_log_level(SON_LOG_LEVEL_NONE);
        /* SPR-13251 Fix Ends */
        /* Set log level to NONE  */
        SET_MODULE_LOG_LEVEL(SON_LOG_LEVEL_NONE);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_log_enable_disable_cmd_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_cell_info_ind_active_state_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT
                               Module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_CELL_INFO_IND
 *                  message received from SONMgmtIfH in RACH_OPT_STATE_ACTIVE.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_cell_info_ind_active_state_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    son_cell_info_ind_t *p_ind = (son_cell_info_ind_t *)p_msg;
    son_u8 idx = 0;
    rach_opt_cell_context_node_t *p_cell_ctxt = SON_PNULL;
    son_feature_state_change_cause_et cause = SON_STATE_CHANGE_CAUSE_UNDEFINED;
    son_feature_state_et state = SON_STATE_UNDEFINED;
    /*SPR_19279_START */
    son_u8 cell_index = *((son_u8 *)(p_msg + sizeof(son_cell_info_ind_t)));
    /*SPR_19279_END */
    SON_UT_TRACE_ENTER();

    SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
            SON_BRIEF,
            "SMIF_CELL_INFO_IND received");

    /* Check if any cell for configuration is present in the request */
    if (0 == p_ind->cell_info_size)
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name,
                SON_WARNING,
                "Cell list is empty");
        LTE_GCC_UNUSED_PARAM(event_id);
    }
    else
    {
        /* Check RACH OPT Global Mode */
        if (SON_ENABLED == g_rach_opt_gb_ctxt.rach_opt_global_mode)
        {
            rach_opt_init_pending_resp_list(son_generate_txn_id(), 
                    SMIF_CELL_INFO_IND);

            for (idx = 0; idx < p_ind->cell_info_size; idx++)
            {
                /*SPR_19279_START */
                p_cell_ctxt = rach_opt_add_mod_cell_context(
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

                    /* Add Cell ID in RACH OPT pending response list */ 
                    if (SON_SUCCESS == rach_opt_add_cell_in_pending_res(
                                &cell_sts))
                    {
                        son_rach_opt_enable_req_t enable_req;
                        SON_MEMSET(&enable_req, 0, 
                            sizeof(son_rach_opt_enable_req_t));
                        enable_req.transaction_id = 
                            rach_opt_get_pending_res_transaction_id();
                        enable_req.enable_req.bitmask |= 
                            SON_RACH_OPT_ENABLE_SRV_CGI_PRESENT;
                        SON_MEMCPY(&enable_req.enable_req.srv_cgi,
                                &p_ind->serving_cell[idx].src_cgi,
                                sizeof(son_intra_rat_global_cell_id_t));
                        /* Call cell specific FSM handler */
                        rach_opt_cell_process_msg(p_cspl_hdr, 
                                &p_cell_ctxt->data, 
                                (son_u8 *)&enable_req);
                    }
                }
                /* If addition of new node failed */
                else if (SON_STATE_UNDEFINED != state)
                {
                    /* Send SONRACH_OPT_FEATURE_STATE_CHANGE_IND */
                    rach_opt_send_feature_state_change_ind(
                            &p_ind->serving_cell[idx].src_cgi, 
                            state, cause);
                }
            }

            if (0 == rach_opt_get_pending_res_expected_count())
            {
                rach_opt_reset_pending_resp_list();
            }
            else
            {
                /* Set RACH OPT global FSM state to RACH_OPT_STATE_ENABLING 
                 * as at least 1 registration request has been sent */
                SONRACH_OPT_FSM_SET_STATE(g_rach_opt_gb_ctxt.current_fsm_state,
                        RACH_OPT_STATE_ENABLING);
                g_rach_opt_gb_ctxt.rach_opt_pending_res_timer_id = 
                    son_start_timer(
                            RACH_OPT_DEFULT_PENDING_RESPONSE_TIMER, SON_PNULL, 
                            SON_NULL, SON_FALSE);
            }
        }
        else
        {
            /* In case of a new cell Id, add cell in RACH OPT cell 
               configuration list if number of current configured cells is 
               below maximum number of configured cells allowed at RACH OPT 
               and the cell is not already configured. Cell must be added 
               with RACH OPT mode as disabled by default. 
               In case of already existing cell, update the cell context */
            for (idx = 0; idx < p_ind->cell_info_size; idx++)
            {
                rach_opt_add_mod_cell_context(
                /*SPR_19279_START */
                        &p_ind->serving_cell[idx], &state, &cause,cell_index);
                /*SPR_19279_END */
                if (SON_STATE_UNDEFINED != state)
                {
                    /* Send SONRACH_OPT_FEATURE_STATE_CHANGE_IND */
                    rach_opt_send_feature_state_change_ind(
                            &p_ind->serving_cell[idx].src_cgi, 
                            state, cause);
                }
            }
        }
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_cell_info_ind_active_state_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_enable_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONRACH_OPT_ENABLE_REQ 
 *                  message received from SONMgmtIfH.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_enable_req_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    son_rach_opt_enable_req_t *p_req = (son_rach_opt_enable_req_t *)p_msg;
    son_rach_opt_enable_res_t enable_res;
    son_size_t enable_res_len = sizeof(son_rach_opt_enable_res_t);
    rach_opt_cell_context_node_t *p_node = SON_PNULL;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&enable_res, 0, enable_res_len);

    /* Check whether the received SONRACH_OPT_ENABLE_REQ is global or cell 
     *  specific */
    if (!(SON_RACH_OPT_ENABLE_SRV_CGI_PRESENT & p_req->enable_req.bitmask))
    {
        son_cell_status_t cell_sts;
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_BRIEF,
                "Global "
                "SONRACH_OPT_ENABLE_REQ received:"
                    /*SPR 17777 +-*/
                "transaction Id: %u event %u",
                p_req->transaction_id,event_id);
                    /*SPR 17777 +-*/

        SON_MEMSET(&cell_sts, 0, sizeof(son_cell_status_t));
        rach_opt_reset_pending_resp_list();
        g_rach_opt_gb_ctxt.rach_opt_global_mode = SON_ENABLED;

        /* Find from the configured cell list whether there is at least one 
         * cell with cell state = RACH_OPT_CELL_STATE_DISABLED or
         * RACH_OPT_CELL_STATE_DISABLING. */
        if (SON_TRUE == rach_opt_is_any_cell_disabled())
        {
            /* If such cells exists */
            rach_opt_init_pending_resp_list(p_req->transaction_id, 
                    SONRACH_OPT_ENABLE_RES);

            p_node = rach_opt_get_first_cell_ctxt();
            while (SON_PNULL != p_node)
            {
                if (RACH_OPT_CELL_STATE_DISABLED == 
                        p_node->data.current_cell_fsm_state
                        || RACH_OPT_CELL_STATE_DISABLING == 
                        p_node->data.current_cell_fsm_state)
                {
                    cell_sts.error_code = SON_ERR_TIMER_EXPIRED;
                    SON_MEMCPY(&cell_sts.cgi, &p_node->data.cell_id,
                            sizeof(son_intra_rat_global_cell_id_t));

                    if (SON_SUCCESS == rach_opt_add_cell_in_pending_res(
                                &cell_sts))
                    {
                        p_req->enable_req.bitmask |= 
                           SON_RACH_OPT_ENABLE_SRV_CGI_PRESENT; 
                       SON_MEMCPY(&p_req->enable_req.srv_cgi,
                               &p_node->data.cell_id,
                               sizeof(son_intra_rat_global_cell_id_t));
                       rach_opt_cell_process_msg(p_cspl_hdr, &p_node->data, 
                               (son_u8 *)p_req);
                   }
                }

                p_node = rach_opt_get_next_node(p_node);
            }

            if (0 == rach_opt_get_pending_res_expected_count())
            {
                /* Construct and send SONRACH_OPT_ENABLE_RES with all the 
                   cells in Pending Response List as per the error status 
                   of the cell. */
                enable_res.transaction_id = p_req->transaction_id;
                enable_res.enable_res.result = SON_FAILURE;
                enable_res.enable_res.error_code = SON_NO_ERROR;
                rach_opt_populate_enable_disable_res_data_from_pending_list(
                    &enable_res.enable_res.cell_status_list_size, 
                    enable_res.enable_res.cell_status_list);

                son_create_send_buffer((son_u8 *)&enable_res,
                        SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, 
                        SONRACH_OPT_ENABLE_RES, enable_res_len);

                rach_opt_reset_pending_resp_list();
            }
            else
            {
                /* Set RACH OPT global FSM state to RACH_OPT_STATE_ENABLING 
                 *  as at least 1 registration request has been sent */
                SONRACH_OPT_FSM_SET_STATE(g_rach_opt_gb_ctxt.current_fsm_state,
                        RACH_OPT_STATE_ENABLING);
                g_rach_opt_gb_ctxt.rach_opt_pending_res_timer_id = 
                    son_start_timer(RACH_OPT_DEFULT_PENDING_RESPONSE_TIMER, 
                            SON_PNULL, SON_NULL, SON_FALSE);
            }
        }
        else
        {
            SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                    p_son_rach_opt_facility_name, SON_INFO,
                    "All cells "
                    "are already enabled or are enabling.");
            /* Send SONRACH_OPT_ENABLE_RES to SONMgmtIfH with result as 
             *  SON_SUCCESS */
            enable_res.transaction_id = p_req->transaction_id;
            enable_res.enable_res.result = SON_SUCCESS;
            enable_res.enable_res.error_code = SON_NO_ERROR;
            son_create_send_buffer((son_u8 *)&enable_res,
                    SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, 
                    SONRACH_OPT_ENABLE_RES, enable_res_len);
        }
    }
    else
    {
        rach_opt_cell_context_t *p_cell_ctxt = SON_PNULL;
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_BRIEF,
                "Cell specific "
                "SONRACH_OPT_ENABLE_REQ received:"
                "transaction Id: %u"
                "Cell Id: 0x%x",
                p_req->transaction_id,
                son_convert_char_cell_id_to_int_cell_id(
                p_req->enable_req.srv_cgi.cell_identity));
        p_cell_ctxt = rach_opt_get_cell_context_from_global_context(
                &p_req->enable_req.srv_cgi);

        if (SON_PNULL == p_cell_ctxt)
        {
            /* Send SONRACH_OPT_ENABLE_RES to SONMgmtIfH with result 
                SON_FAILURE and error code = SON_ERR_CELL_UNCONFIGURED */
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
                    SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, 
                    SONRACH_OPT_ENABLE_RES, enable_res_len);
        }
        else
        {
            rach_opt_cell_process_msg(p_cspl_hdr, p_cell_ctxt, (son_u8 *)p_req);
        }
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_enable_req_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_disable_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONRACH_OPT_DISABLE_REQ 
 *                  message received from SONMgmtIfH.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_disable_req_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    son_rach_opt_disable_req_t *p_req = (son_rach_opt_disable_req_t *)p_msg;
    son_rach_opt_disable_res_t disable_res;
    son_size_t disable_res_len = sizeof(son_rach_opt_disable_res_t);
    rach_opt_cell_context_node_t *p_node = SON_PNULL;

    SON_UT_TRACE_ENTER();

    SON_MEMSET(&disable_res, 0, disable_res_len);
    if (!(SON_RACH_OPT_DISABLE_SRV_CGI_PRESENT & p_req->disable_req.bitmask))
    {
        son_cell_status_t cell_sts;
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_BRIEF,
                "Global "
                "SONRACH_OPT_DISABLE_REQ received:"
                    /*SPR 17777 +-*/
                "transaction Id: %u event [%u]",
                p_req->transaction_id,event_id);
                    /*SPR 17777 +-*/

        SON_MEMSET(&cell_sts, 0, sizeof(son_cell_status_t));
        rach_opt_reset_pending_resp_list();
        g_rach_opt_gb_ctxt.rach_opt_global_mode = SON_DISABLED;

        /* Find from the configure cell list whether there is at least one 
         *  cell with cell state =  RACH_OPT_CELL_STATE_ENABLED or
         *  RACH_OPT_CELL_STATE_ENABLING state */
        if (SON_TRUE == rach_opt_is_any_cell_enabled())
        {
            /* If such cells exists */
            rach_opt_init_pending_resp_list(p_req->transaction_id, 
                    SONRACH_OPT_DISABLE_RES);

            p_node = rach_opt_get_first_cell_ctxt();
            while (SON_PNULL != p_node)
            {
                if (RACH_OPT_CELL_STATE_ENABLED == 
                        p_node->data.current_cell_fsm_state
                        || RACH_OPT_CELL_STATE_ENABLING == 
                        p_node->data.current_cell_fsm_state)
                {
                   cell_sts.error_code = SON_ERR_TIMER_EXPIRED;
                   SON_MEMCPY(&cell_sts.cgi, &p_node->data.cell_id,
                           sizeof(son_intra_rat_global_cell_id_t));

                   if (SON_SUCCESS == rach_opt_add_cell_in_pending_res(
                               &cell_sts))
                   {
                       p_req->disable_req.bitmask |= 
                           SON_RACH_OPT_DISABLE_SRV_CGI_PRESENT;
                       SON_MEMCPY(&p_req->disable_req.srv_cgi,
                               &p_node->data.cell_id,
                               sizeof(son_intra_rat_global_cell_id_t));
                       rach_opt_cell_process_msg(p_cspl_hdr, &p_node->data, 
                               (son_u8 *)p_req);
                   }
                   else
                   {
                       disable_res.transaction_id = p_req->transaction_id;
                       disable_res.disable_res.result = SON_FAILURE;
                       disable_res.disable_res.error_code = SON_NO_ERROR;
                       SON_MEMCPY(&disable_res.disable_res.cell_status_list[
                               disable_res.disable_res.cell_status_list_size].cgi,
                               &p_node->data.cell_id,
                               sizeof(son_intra_rat_global_cell_id_t));
                       disable_res.disable_res.cell_status_list[
                           disable_res.disable_res.cell_status_list_size].error_code = 
                           SON_ERR_SYS_MEM_ALLOC_FAILURE;
                       disable_res.disable_res.cell_status_list_size++;
                   }
                }

                p_node = rach_opt_get_next_node(p_node);
            }

            if (0 == rach_opt_get_pending_res_expected_count())
            {
                if (0 == disable_res.disable_res.cell_status_list_size)
                {
                    /* Construct and send SONRACH_OPT_DISABLE_RES with all the 
                       cells in Pending Response List as per the error status of 
                       the cell. */
                    disable_res.transaction_id = p_req->transaction_id;
                    disable_res.disable_res.result = SON_FAILURE;
                    disable_res.disable_res.error_code = SON_NO_ERROR;
                    rach_opt_populate_enable_disable_res_data_from_pending_list(
                            &disable_res.disable_res.cell_status_list_size,
                            disable_res.disable_res.cell_status_list);
                }   
                son_create_send_buffer((son_u8 *)&disable_res,
                        SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, 
                        SONRACH_OPT_DISABLE_RES, disable_res_len);

                rach_opt_reset_pending_resp_list();
            }
            else
            {
                /* Set RACH_OPT global FSM state to RACH_OPT_STATE_DISABLING */
                SONRACH_OPT_FSM_SET_STATE(g_rach_opt_gb_ctxt.current_fsm_state,
                        RACH_OPT_STATE_DISABLING);
                g_rach_opt_gb_ctxt.rach_opt_pending_res_timer_id = 
                    son_start_timer(RACH_OPT_DEFULT_PENDING_RESPONSE_TIMER, 
                            SON_PNULL, SON_NULL, SON_FALSE);
            }
        }
        else
        {
            SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                    p_son_rach_opt_facility_name, SON_INFO,
                    "All cells "
                    "are already disabled or are disabling.");
            /* Send SONRACH_OPT_DISABLE_RES to SONMgmtIfH with result as 
             *  SON_SUCCESS */
            disable_res.transaction_id = p_req->transaction_id;
            disable_res.disable_res.result = SON_SUCCESS;
            disable_res.disable_res.error_code = SON_NO_ERROR;
            son_create_send_buffer((son_u8 *)&disable_res,
                    SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, 
                    SONRACH_OPT_DISABLE_RES, disable_res_len);
        }
    }
    else
    {
        rach_opt_cell_context_t *p_cell_ctxt = SON_PNULL;
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_BRIEF,
                "Cell specific "
                "SONRACH_OPT_DISABLE_REQ received:"
                "trans Id: %u"
                "Cell Id: 0x%x",
                p_req->transaction_id,
                son_convert_char_cell_id_to_int_cell_id(
                p_req->disable_req.srv_cgi.cell_identity));
        p_cell_ctxt = rach_opt_get_cell_context_from_global_context(
                &p_req->disable_req.srv_cgi);

        if (SON_PNULL == p_cell_ctxt)
        {
            /* Send SONRACH_OPT_DISABLE_RES to SONMgmtIfH with result 
               SON_FAILURE and error code = SON_ERR_CELL_UNCONFIGURED */
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
                    SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, 
                    SONRACH_OPT_DISABLE_RES, disable_res_len);
        }
        else
        {
            rach_opt_cell_process_msg(p_cspl_hdr, p_cell_ctxt, (son_u8 *)p_req);
        }
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_disable_req_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_modify_config_param_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  SONRACH_OPT_MODIFY_CONFIG_PARAMS_REQ message received from 
 *                  SONMgmtIfH.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_modify_config_param_req_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    son_rach_opt_modify_config_params_req_t *p_req = 
        (son_rach_opt_modify_config_params_req_t *)p_msg;
    rach_opt_cell_context_t *p_cell_ctxt = SON_PNULL;

    SON_UT_TRACE_ENTER();
    
    SON_LOG(g_rach_opt_gb_ctxt.log_mode,
            p_son_rach_opt_facility_name, SON_BRIEF,
            "SONRACH_OPT_MODIFY_CONFIG_PARAMS_REQ received:"
            "transaction Id: %u"
                    /*SPR 17777 +-*/
            "Cell Id: 0x%x event[%u]",
            p_req->transaction_id,
            son_convert_char_cell_id_to_int_cell_id(
            p_req->modify_req.srv_cgi.cell_identity),event_id);
                    /*SPR 17777 +-*/

    p_cell_ctxt = rach_opt_get_cell_context_from_global_context(
            &p_req->modify_req.srv_cgi);

    if (SON_PNULL == p_cell_ctxt)
    {
        son_rach_opt_modify_config_params_res_t config_params_res;
        son_size_t config_params_res_len = 
            sizeof(son_rach_opt_modify_config_params_res_t);
        SON_MEMSET(&config_params_res, 0, config_params_res_len);
        config_params_res.transaction_id = p_req->transaction_id;
        SON_MEMCPY(&config_params_res.modify_res.srv_cgi,
            &p_req->modify_req.srv_cgi,
            sizeof(son_intra_rat_global_cell_id_t));
        config_params_res.modify_res.result = SON_FAILURE;
        config_params_res.modify_res.error_code = 
            SON_ERR_CELL_UNCONFIGURED;
        config_params_res.modify_res.failed_param_bitmask = 
            p_req->modify_req.rach_config_params.bitmask;
        son_create_send_buffer((son_u8 *)&config_params_res,
                SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, 
                SONRACH_OPT_MODIFY_CONFIG_PARAMS_RES, config_params_res_len);
    }
    else
    {
        rach_opt_cell_process_msg(p_cspl_hdr, p_cell_ctxt, (son_u8 *)p_req);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_modify_config_param_req_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_modify_rach_optmode_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  SONRACH_OPT_MODIFY_RACH_OPTMODE_REQ message received from 
 *                  SONMgmtIfH.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_modify_rach_optmode_req_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    son_rach_opt_modify_rach_optmode_req_t *p_req = 
        (son_rach_opt_modify_rach_optmode_req_t *)p_msg;
    son_rach_opt_modify_rach_optmode_res_t rach_optmode_res;
    son_size_t rach_optmode_res_len = 
        sizeof(son_rach_opt_modify_rach_optmode_res_t);
    rach_opt_cell_context_node_t *p_node = SON_PNULL;

    SON_UT_TRACE_ENTER();
    
    SON_LOG(g_rach_opt_gb_ctxt.log_mode,
            p_son_rach_opt_facility_name, SON_BRIEF,
            "SONRACH_OPT_MODIFY_RACH_OPTMODE_REQ received:"
            "transaction Id: %u"
                    /*SPR 17777 +-*/
            "RACH OPT mode: %s",
            p_req->transaction_id,
            SON_MODE_NAMES[p_req->modify_mode_req.rachopt_mode]);
    LTE_GCC_UNUSED_PARAM(event_id);
    LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/


    if (p_req->modify_mode_req.rachopt_mode != g_rach_opt_gb_ctxt.rachopt_mode)
    {
        if (SON_MODE_DISTRIBUTED == g_rach_opt_gb_ctxt.rachopt_mode)
        {
            /* Stop the algo for each cell */
            p_node = rach_opt_get_first_cell_ctxt();
            while (SON_PNULL != p_node)
            {
                rach_opt_init_rach_counters(&p_node->data);
                p_node = rach_opt_get_next_node(p_node);
            }
        }
        g_rach_opt_gb_ctxt.rachopt_mode = p_req->modify_mode_req.rachopt_mode;
    }

    /* Send SONRACH_OPT_MODIFY_RACH_OPTMODE_RES to SONMgmtIfH. */ 
    SON_MEMSET(&rach_optmode_res, 0, rach_optmode_res_len);
    rach_optmode_res.transaction_id = p_req->transaction_id;
    rach_optmode_res.modify_mode_res.result = SON_SUCCESS;
    rach_optmode_res.modify_mode_res.error_code = SON_NO_ERROR;
    son_create_send_buffer((son_u8 *)&rach_optmode_res,
            SON_RACH_OPT_MODULE_ID, SON_MIF_MODULE_ID, 
            SONRACH_OPT_MODIFY_RACH_OPTMODE_RES, rach_optmode_res_len);

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_modify_rach_optmode_req_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_rach_config_res_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  RRM_SON_RACH_CONFIG_RESP message received from RRM.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_rach_config_res_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    son_rrm_rach_config_resp_t *p_res = (son_rrm_rach_config_resp_t *)p_msg;
    rach_opt_cell_context_t *p_cell_ctxt = SON_PNULL;

    SON_UT_TRACE_ENTER();

    SON_LOG(g_rach_opt_gb_ctxt.log_mode,
            p_son_rach_opt_facility_name, SON_BRIEF,
            "RRM_SON_RACH_CONFIG_RESP received:"
            "transaction Id: %u"
                    /*SPR 17777 +-*/
            "Cell Id: 0x%x event[%u]",
            p_res->transaction_id,
            son_convert_char_cell_id_to_int_cell_id(
            p_res->res.cell_id.cell_identity),event_id);
                    /*SPR 17777 +-*/


    /* Rel 3.0 Cell delete support changes Start */
    /* Find the transaction Id received in De-register response in
       temporary transaction Id list stored during cell deletion.
       If found remove it from temporary transaction Id list and
       do not process the corresponding De-register response as the
       cell is already deleted */
    if (RRM_SON_RACH_CONFIG_RESP == p_cspl_hdr->api)
    {
        son_rrm_deregistration_res_t *p_res = (son_rrm_deregistration_res_t *)p_msg;
        if (SON_SUCCESS == 
                son_find_txn_id_in_temp_txn_id_list(p_res->transaction_id))
        {
            SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                    p_son_rach_opt_facility_name, SON_INFO,
                    "RRM_SON_RACH_CONFIG_RESP received for the cell "
                    "Cell 0x%x already deleted at RACH-OPT due to "
                    "cell delete request from OAM so ignore it",
                    son_convert_char_cell_id_to_int_cell_id(
                        p_res->res.deregistration_resp.id.object_id.
                        cell_id.cell_identity));
            SON_UT_TRACE_EXIT();
            return RELEASE_SON_BUFFER;
        }
    }
    /* Rel 3.0 Cell delete support changes Stop */

    p_cell_ctxt = rach_opt_get_cell_context_from_global_context(
            (son_intra_rat_global_cell_id_t *)(&p_res->res.cell_id));

    if (SON_PNULL == p_cell_ctxt)
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_WARNING,
                "RRM_SON_RACH_CONFIG_RESP received "
                "for unconfigured Cell 0x%x",
                son_convert_char_cell_id_to_int_cell_id(
                p_res->res.cell_id.cell_identity));
    }
    else
    {
        rach_opt_cell_process_msg(p_cspl_hdr, p_cell_ctxt, (son_u8 *)p_res);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_rach_config_res_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_rach_ue_ind_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  RRM_SON_RACH_UE_INFO_IND message received from RRM.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_rach_ue_ind_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    son_rrm_rach_ue_info_ind_t *p_ind = (son_rrm_rach_ue_info_ind_t *)p_msg;
    rach_opt_cell_context_t *p_cell_ctxt = SON_PNULL;

    SON_UT_TRACE_ENTER();
    
    SON_LOG(g_rach_opt_gb_ctxt.log_mode,
            p_son_rach_opt_facility_name, SON_BRIEF,
            "RRM_SON_RACH_UE_INFO_IND received:"
                    /*SPR 17777 +-*/
            "Cell Id: 0x%x event [%u]",
            son_convert_char_cell_id_to_int_cell_id(
            p_ind->cell_id.cell_identity),event_id);
                    /*SPR 17777 +-*/

    p_cell_ctxt = rach_opt_get_cell_context_from_global_context(
            (son_intra_rat_global_cell_id_t *)(&p_ind->cell_id));

    if (SON_PNULL == p_cell_ctxt)
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_WARNING,
                "RRM_SON_RACH_UE_INFO_IND received "
                "for unconfigured Cell 0x%x",
                son_convert_char_cell_id_to_int_cell_id(
                p_ind->cell_id.cell_identity));
    }
    else
    {
        rach_opt_cell_process_msg(p_cspl_hdr, p_cell_ctxt, (son_u8 *)p_ind);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_rach_ue_ind_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_rach_l2_ind_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  RRM_SON_L2_RACH_PERF_REPORT message received from RRM.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_rach_l2_ind_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    son_rrm_l2_rach_perf_report_t *p_report = 
        (son_rrm_l2_rach_perf_report_t *)p_msg;
    rach_opt_cell_context_t *p_cell_ctxt = SON_PNULL;

    SON_UT_TRACE_ENTER();
    
    SON_LOG(g_rach_opt_gb_ctxt.log_mode,
            p_son_rach_opt_facility_name, SON_BRIEF,
            "RRM_SON_L2_RACH_PERF_REPORT received:"
                    /*SPR 17777 +-*/
            "Cell 0x%x",
            son_convert_char_cell_id_to_int_cell_id(
            p_report->cell_id.cell_identity));
    LTE_GCC_UNUSED_PARAM(event_id);
                    /*SPR 17777 +-*/

    p_cell_ctxt = rach_opt_get_cell_context_from_global_context(
            (son_intra_rat_global_cell_id_t *)(&p_report->cell_id));

    if (SON_PNULL == p_cell_ctxt)
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_WARNING,
                "RRM_SON_L2_RACH_PERF_REPORT received "
                "for unconfigured Cell 0x%x",
                son_convert_char_cell_id_to_int_cell_id(
                p_report->cell_id.cell_identity));
    }
    else
    {
        rach_opt_cell_process_msg(p_cspl_hdr, p_cell_ctxt, (son_u8 *)p_report);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_rach_l2_ind_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_cell_state_change_ind_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  RRM_SON_CELL_STATE_CHANGE_IND message received from RRM.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_cell_state_change_ind_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    son_cell_state_change_indication_t *p_ind = 
        (son_cell_state_change_indication_t *)p_msg;
    rach_opt_cell_context_t *p_cell_ctxt = SON_PNULL;

    SON_UT_TRACE_ENTER();

    SON_LOG(g_rach_opt_gb_ctxt.log_mode,
            p_son_rach_opt_facility_name, SON_BRIEF,
            "RRM_SON_CELL_STATE_CHANGE_IND received: "
                    /*SPR 17777 +-*/
            "Cell 0x%x event[%u]",
            son_convert_char_cell_id_to_int_cell_id(
            p_ind->cell_id.cell_identity),event_id);
                    /*SPR 17777 +-*/

    p_cell_ctxt = rach_opt_get_cell_context_from_global_context(
            (son_intra_rat_global_cell_id_t *)(&p_ind->cell_id));

    if (SON_PNULL == p_cell_ctxt)
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_WARNING,
                "RRM_SON_CELL_STATE_CHANGE_IND received for unconfigured cell "
                "Cell 0x%x",
                son_convert_char_cell_id_to_int_cell_id(
                p_ind->cell_id.cell_identity));
    }
    else
    {
        rach_opt_cell_process_msg(p_cspl_hdr, p_cell_ctxt, (son_u8 *)p_ind);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_cell_state_change_ind_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_cell_anr_mode_ind_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for 
 *                  SONANR_DISABLE_IND message received from SON ANR.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_cell_anr_mode_ind_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    son_anr_cell_mode_ind_t *p_ind = 
        (son_anr_cell_mode_ind_t *)p_msg;
    rach_opt_cell_context_t *p_cell_ctxt = SON_PNULL;

    SON_UT_TRACE_ENTER();

    SON_LOG(g_rach_opt_gb_ctxt.log_mode,
            p_son_rach_opt_facility_name, SON_BRIEF,
            "SONANR_DISABLE_IND received:"
            "Cell 0x%x",
            son_convert_char_cell_id_to_int_cell_id(
            p_ind->srv_cgi.cell_identity));

    p_cell_ctxt = rach_opt_get_cell_context_from_global_context(
            &p_ind->srv_cgi);

    if (SON_PNULL == p_cell_ctxt)
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_WARNING,
                "SONANR_DISABLE_IND received for unconfigured cell "
                    /*SPR 17777 +-*/
                "Cell 0x%x event[%u]",
                son_convert_char_cell_id_to_int_cell_id(
                p_ind->srv_cgi.cell_identity),event_id);
                    /*SPR 17777 +-*/
    }
    else
    {
        rach_opt_cell_process_msg(p_cspl_hdr, p_cell_ctxt, (son_u8 *)p_ind);
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_cell_anr_mode_ind_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_cell_info_ind_disabling_state_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_CELL_INFO_IND
  *                 message received from SONMgmtIfH in RACH_OPT_STATE_DISABLING.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_cell_info_ind_disabling_state_handler
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_rach_opt_index_et event_id
)
{
    SON_UT_TRACE_ENTER();

    SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
            SON_WARNING,
            "Global "
            "disable is in progress so cell info indication can't be "
            "processed msg");
    LTE_GCC_UNUSED_PARAM(p_msg);
    LTE_GCC_UNUSED_PARAM(event_id);
    LTE_GCC_UNUSED_PARAM(p_cspl_hdr);

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_cell_info_ind_disabling_state_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_cell_info_ind_enabling_state_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_CELL_INFO_IND
  *                 message received from SONMgmtIfH in RACH_OPT_STATE_ENABLING.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_cell_info_ind_enabling_state_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    SON_UT_TRACE_ENTER();

    SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
    SON_WARNING,
    "Global enable"
    " is in progress so cell info indication can't be processed.");
    LTE_GCC_UNUSED_PARAM(p_msg);
    LTE_GCC_UNUSED_PARAM(event_id);
    LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
}

/*****************************************************************************
 * Function Name  : rach_opt_fsm_shutdown_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_SHUTDOWN_REQ
 *                  message received from SONMgmtIfH in any state other than
 *                  RACH_OPT_STATE_SHUTDOWN state.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_shutdown_req_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    smif_shutdown_req_t *p_req = (smif_shutdown_req_t *)p_msg;
    smif_shutdown_res_t shut_down_res = {0};
    /* SPR 20263 Fix Start */
	rach_opt_cell_context_node_t *p_ctxt_node = SON_PNULL;
    /* SPR 20263 Fix End*/
    SON_UT_TRACE_ENTER();

    SON_LOG(g_rach_opt_gb_ctxt.log_mode,
            p_son_rach_opt_facility_name, SON_BRIEF,
            "SMIF_SHUTDOWN_REQ received:"
            "transaction Id: %u",
            p_req->transaction_id);

    /* SPR 20263 Fix Start */
	p_ctxt_node = rach_opt_get_first_cell_ctxt();
    while (SON_PNULL != p_ctxt_node)
    {
        rach_opt_delete_cell_from_context(&p_ctxt_node->data.cell_id);
        p_ctxt_node = rach_opt_get_next_node(p_ctxt_node);
    }
    /* SPR 20263 Fix End */

    /* Clean up the RACH OPT cell contexts list */
                    /*SPR 17777 +-*/
    rach_opt_clear_cell_contexts_list();

    /* Clean up ENABLE/DISABLE Pending Response list */
    rach_opt_reset_pending_resp_list();

    if (SON_TRUE ==
        son_is_timer_running(g_rach_opt_gb_ctxt.rach_opt_pending_res_timer_id))
    {
        son_stop_timer(g_rach_opt_gb_ctxt.rach_opt_pending_res_timer_id);
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_DETAILED,
                "Pending response timer "
                    /*SPR 17777 +-*/
                "with Timer Id [%p] is stopped. event[%u][%u]",
                g_rach_opt_gb_ctxt.rach_opt_pending_res_timer_id,
                event_id,sizeof(p_cspl_hdr));
                    /*SPR 17777 +-*/
        g_rach_opt_gb_ctxt.rach_opt_pending_res_timer_id = SON_PNULL;
    }

    /* Set the number of cells configured at RACH OPT to 0 */
    list_init(&g_rach_opt_gb_ctxt.rach_opt_cell_ctxt_list);

    /* Set RACH OPT Global FSM state to SHUTDOWN */
    SONRACH_OPT_FSM_SET_STATE(g_rach_opt_gb_ctxt.current_fsm_state,
            RACH_OPT_STATE_SHUTDOWN);

    /* Send SMIF_SHUTDOWN_RES to SONMgmtIfH*/
    shut_down_res.transaction_id = p_req->transaction_id;
    shut_down_res.result         = SON_SUCCESS;
    shut_down_res.error_code     = SON_NO_ERROR;

    son_create_send_buffer((son_u8 *)&shut_down_res, SON_RACH_OPT_MODULE_ID,
            SON_MIF_MODULE_ID, SMIF_SHUTDOWN_RES, sizeof(smif_shutdown_res_t));

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_shutdown_req_handler */

/*****************************************************************************
 * Function Name  : rach_opt_fsm_shutdown_state_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT Module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for the messages received
 *                  in RACH_OPT_STATE_SHUTDOWN state.
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_shutdown_state_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    SON_UT_TRACE_ENTER();

    SON_LOG(g_rach_opt_gb_ctxt.log_mode, p_son_rach_opt_facility_name, 
            SON_INFO,
            "Event %s ignored in "
                    /*SPR 17777 +-*/
            "RACH_OPT_STATE_SHUTDOWN state received from %u",
            SON_RACH_OPT_FSM_EVENT_NAMES[event_id],
            p_cspl_hdr->from);
    LTE_GCC_UNUSED_PARAM(p_msg);
                    /*SPR 17777 +-*/

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* rach_opt_fsm_shutdown_state_handler */

/* Rel 3.0 Cell delete support changes Start */
/*****************************************************************************
 * Function Name  : rach_opt_fsm_cell_delete_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for
 *                  SMIF_CELL_DELETE_REQ message received from MIF
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_cell_delete_req_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    smif_cell_delete_req_t  *p_req          = (smif_cell_delete_req_t *)p_msg;
    rach_opt_cell_context_t *p_cell_ctxt = SON_PNULL;
    son_u8                  idx             = 0;
    smif_cell_delete_resp_t  cell_del_res;
    son_rrm_rach_config_req_t req = {0};
    son_procedure_code_et api = SON_PROCEDURE_UNDEFINED;
    cell_status_list_data_t *p_node = SON_PNULL;
    SON_UT_TRACE_ENTER();

    SON_MEMSET(&cell_del_res, 0, sizeof(cell_del_res));

    cell_del_res.cell_delete_status_list_size =
        p_req->cell_delete_list_size;

    /* Traverse the cell delete list */
    for (idx = 0; idx < p_req->cell_delete_list_size; idx++)
    {
        /* Populate the response structure */
        SON_MEMCPY(&cell_del_res.cell_delete_status_list[idx].cgi,
                &p_req->cell_delete_list[idx],
                sizeof(son_intra_rat_global_cell_id_t));

        cell_del_res.cell_delete_status_list[idx].result = SON_FAILURE;

        /* Find the cell in global context */
        p_cell_ctxt = rach_opt_get_cell_context_from_global_context(
                &p_req->cell_delete_list[idx]);

        if (SON_PNULL != p_cell_ctxt)
        {
            /* Send deregister request to RRM if cell is ENABLING/ENABLED */
            if ((RACH_OPT_CELL_STATE_ENABLING == 
                        p_cell_ctxt->current_cell_fsm_state) ||
                    (RACH_OPT_CELL_STATE_ENABLED == 
                     p_cell_ctxt->current_cell_fsm_state))
            {
                req.transaction_id = son_generate_txn_id();;
                SON_MEMCPY(&(req.req.cell_id), &p_cell_ctxt->cell_id, sizeof(
                            son_intra_rat_global_cell_id_t));
                req.req.bitmask = (L2_REPORT_PERIODICITY_PRESENT |
                        UE_REPORT_PERIODICITY_PRESENT);
                req.req.l2_reports_periodicity = 0;
                req.req.ue_reports_periodicity = 0;

                /* Invoke PUP tool to send RACH config request message to 
                   stop receiving RACH related reports from RRM */
                if (RRM_FAILURE == rrm_son_send_rrm_son_rach_config_req(
                            &req.req, SON_RACH_OPT_MODULE_ID, SON_RRM_MODULE_ID,
                            req.transaction_id, SON_NULL))
                {
                    cell_del_res.cell_delete_status_list[idx].result = 
                        SON_PARTIAL_SUCCESS;
                    SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                            p_son_rach_opt_facility_name, SON_ERROR,
                            "Failed to send RACH config request message to "
                            "stop receiving RACH related reports from RRM for "
                            "Cell 0x%x",
                            son_convert_char_cell_id_to_int_cell_id
                            (p_cell_ctxt->cell_id.cell_identity));
                }
                else
                {
                    /* Store the transaction Id with which RACH config 
                       request to de-register services is sent to RRM. 
                       This shall be used later when response is received 
                       for this request */
                    son_insert_txn_id_in_temp_txn_id_list(req.transaction_id);
                }
            }
            /* Check the pending resp list for this cell */
            if (rach_opt_get_pending_res_expected_count())
            {
                p_node = rach_opt_find_cell_in_pending_res(&p_cell_ctxt->cell_id);
                if (SON_PNULL != p_node)
                {
                    /* Error code SON_ERR_TIMER_EXPIRED indicates that
                       the pending response is not received for this cell 
                       so decrement expected response count since the 
                       cell is being deleted and no response should be
                       expected for this cell. */
                    if (SON_ERR_TIMER_EXPIRED == p_node->cell_status.error_code)
                    {
                        rach_opt_decrement_pending_res_expected_count();
                    }

                    api = rach_opt_get_pending_res_api_id();
                    if (SMIF_CELL_INFO_IND == api)
                    {
                        /* Delete the cell as in case of cell deletion
                           no feature state change indication shall be send
                           and only cell delete response shall be send to
                           SMIF */
                        list_delete_node(&g_rach_opt_pending_response.cell_sts_list,
                                &p_node->cell_status_node);
                        son_mem_free(p_node);
                        p_node = SON_PNULL;
                        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                                p_son_rach_opt_facility_name, SON_BRIEF,
                                "Deleted Cell 0x%x from Pending Response List",
                                son_convert_char_cell_id_to_int_cell_id(p_cell_ctxt->cell_id.cell_identity));
                    }
                    else
                    {
                        /* Update Cell in Pending Response List with error code
                           SON_ERR_CELL_UNCONFIGURED */
                           p_node->cell_status.error_code = SON_ERR_CELL_UNCONFIGURED;
                    }
                }
                if (0 == rach_opt_get_pending_res_expected_count())
                {
                    if (MIN_PENDING_RES_COUNT == rach_opt_get_pending_res_cell_count())
                    {
                        /* Set RACH OPT global FSM state to ACTIVE */
                        SONRACH_OPT_FSM_SET_STATE(g_rach_opt_gb_ctxt.current_fsm_state,
                                RACH_OPT_STATE_ACTIVE);

                        /* Clear/Reset Pending Response ListRACH_OPT_Pending_Response_LIST. */
                        rach_opt_reset_pending_resp_list();
                    }
                    else
                    {
                        rach_opt_pending_res_timer_handler();
                    }
                }
            }

            /* Delete the cell from the context */
            if (SON_TRUE == rach_opt_delete_cell_from_context(
                        &p_req->cell_delete_list[idx]))
            {
                if (SON_PARTIAL_SUCCESS != 
                        cell_del_res.cell_delete_status_list[idx].result)
                {
                    cell_del_res.cell_delete_status_list[idx].result = 
                        SON_SUCCESS;
                }
                SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                        p_son_rach_opt_facility_name, SON_INFO,
                        "Cell 0x%x deleted successfully",
                        son_convert_char_cell_id_to_int_cell_id
                        (p_req->cell_delete_list[idx].cell_identity));
            }
        }
        else
        {
            SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                    p_son_rach_opt_facility_name, SON_ERROR,
                    /*SPR 17777 +-*/
                    "Cell 0x%x does not exists",
                    son_convert_char_cell_id_to_int_cell_id
                    (p_req->cell_delete_list[idx].cell_identity));
                    LTE_GCC_UNUSED_PARAM(event_id);
                    LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                    /*SPR 17777 +-*/
        }
    }

    /* Send cell delete resp to mif */
    son_create_send_buffer((son_u8 *)&cell_del_res,
            SON_RACH_OPT_MODULE_ID,
            SON_MIF_MODULE_ID,
            SMIF_CELL_DELETE_RESP,
            sizeof(cell_del_res));

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
}
/* Rel 3.0 Cell delete support changes Stop */

/* SPR-13251 Fix Starts */
/*****************************************************************************
 * Function Name  : rach_opt_fsm_get_log_level_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  event_id - Value of the event Id internal to RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for
 *                  SMIF_GET_LOG_LEVEL_REQ message received from MIF
 ****************************************************************************/
static son_buf_retain_status_et
rach_opt_fsm_get_log_level_req_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_rach_opt_index_et   event_id
)
{
    smif_get_log_level_req_t *p_req = (smif_get_log_level_req_t*)p_msg;
    smif_get_log_level_res_t res;
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    (void)event_id;
    (void)p_cspl_hdr;
     /*SPR 17777 +-*/
    SON_MEMSET(&res, 0, sizeof(res));
    res.transaction_id = p_req->transaction_id;
    res.log_level = rach_opt_get_log_level();
    
    /* Send cell delete resp to mif */
    son_create_send_buffer((son_u8 *)&res,
            SON_RACH_OPT_MODULE_ID,
            SON_MIF_MODULE_ID,
            SMIF_GET_LOG_LEVEL_RESP,
            sizeof(res));

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
}
/* SPR-13251 Fix Ends */

/****************************************************************************
 * Function Name  : rach_opt_parse_cell_state_change_ind
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the cell state change 
 *                  indicaiton message received from RRM
 ****************************************************************************/
static son_u8 *
rach_opt_parse_cell_state_change_ind
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
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_ERROR,
                "RRM->SONRACH:RRM_SON_CELL_STATE_CHANGE_IND parsing failed");
        /* Free the memory when parsing is failed */
        son_mem_free(p_parsed_msg);
        p_parsed_msg = SON_PNULL;
    }
    else
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_INFO,
                "RRM->SONRACH:RRM_SON_CELL_STATE_CHANGE_IND");
    }

    SON_UT_TRACE_EXIT();
    return p_parsed_msg;
} /* rach_opt_parse_cell_state_change_ind */

/****************************************************************************
 * Function Name  : rach_opt_parse_rach_config_resp
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the RRM_SON_RACH_CONFIG_RESP
 *                  indicaiton message received from RRM
 ****************************************************************************/
static son_u8 *
rach_opt_parse_rach_config_resp
(
    son_u8  *p_msg,
    son_u16 buf_len
    /*SPR 17777 +-*/
)
{
    son_u8 *p_parsed_msg = SON_PNULL;
    S32 length_read = 0;
    SON_UT_TRACE_ENTER();

    p_parsed_msg = son_mem_get(sizeof(son_rrm_rach_config_resp_t));
    if (SON_PNULL != p_parsed_msg)
    {
        if (RRM_FAILURE == rrm_parse_rrm_son_rach_config_resp(
                    &((son_rrm_rach_config_resp_t *)p_parsed_msg)->res,
                    (p_msg + EXT_MSG_API_HDR_SIZE),
                    buf_len - EXT_MSG_API_HDR_SIZE,
                    &length_read))
        {
            SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                    p_son_rach_opt_facility_name, SON_ERROR,
                    "RRM->SONRACH:RRM_SON_RACH_CONFIG_RESP parsing failed");
            /* Free the memory when parsing is failed */
            son_mem_free(p_parsed_msg);
            p_parsed_msg = SON_PNULL;
        }
        else
        {
            SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                    p_son_rach_opt_facility_name, SON_INFO,
                    "RRM->SONRACH:RRM_SON_RACH_CONFIG_RESP response %u error %u",
                    ((son_rrm_rach_config_resp_t *)p_parsed_msg)->res.result,
                    ((son_rrm_rach_config_resp_t *)p_parsed_msg)->res.error_code);
            *(son_u16 *)p_parsed_msg = son_get_word_from_header(
                    &p_msg[EXT_MSG_TRANSACTION_ID_OFFSET]);
        }
    }

    SON_UT_TRACE_EXIT();
    return p_parsed_msg;
} /* rach_opt_parse_rach_config_resp */

/****************************************************************************
 * Function Name  : rach_opt_parse_rach_ue_info_ind
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the RRM_SON_RACH_UE_INFO_IND
 *                  message received from RRM
 ****************************************************************************/
static son_u8 *
rach_opt_parse_rach_ue_info_ind
(
    son_u8  *p_msg,
    son_u16 buf_len
    /*SPR 17777 +-*/
)
{
    son_u8 *p_parsed_msg = SON_PNULL;
    S32 length_read = 0;
    SON_UT_TRACE_ENTER();

    p_parsed_msg = son_mem_get(sizeof(son_rrm_rach_ue_info_ind_t));
    if (SON_PNULL != p_parsed_msg && 
            RRM_FAILURE == rrm_parse_rrm_son_rach_ue_info_ind(
                (son_rrm_rach_ue_info_ind_t *)p_parsed_msg,
                (p_msg + EXT_MSG_API_HDR_SIZE),
                buf_len - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_ERROR,
                "RRM->SONRACH:RRM_SON_RACH_UE_INFO_IND parsing failed");
        /* Free the memory when parsing is failed */
        son_mem_free(p_parsed_msg);
        p_parsed_msg = SON_PNULL;
    }
    else
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_INFO,
                "RRM->SONRACH:RRM_SON_RACH_UE_INFO_IND");
    }

    SON_UT_TRACE_EXIT();
    return p_parsed_msg;
} /* rach_opt_parse_rach_ue_info_ind */

/****************************************************************************
 * Function Name  : rach_opt_parse_l2_rach_perf_report
 * Inputs         : p_msg  - Pointer to the message received
 *                  buf_len - Length of the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the 
 *                  RRM_SON_L2_RACH_PERF_REPORT message received from RRM
 ****************************************************************************/
static son_u8 *
rach_opt_parse_l2_rach_perf_report
(
    son_u8  *p_msg,
    son_u16 buf_len
    /*SPR 17777 +-*/
)
{
    son_u8 *p_parsed_msg = SON_PNULL;
    S32 length_read = 0;
    SON_UT_TRACE_ENTER();
    
    p_parsed_msg = son_mem_get(sizeof(son_rrm_l2_rach_perf_report_t));
    if (SON_PNULL != p_parsed_msg && 
            RRM_FAILURE == rrm_parse_rrm_son_l2_rach_perf_report(
                (son_rrm_l2_rach_perf_report_t *)p_parsed_msg,
                (p_msg + EXT_MSG_API_HDR_SIZE),
                buf_len - EXT_MSG_API_HDR_SIZE,
                &length_read))
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_ERROR,
                "RRM->SONRACH:RRM_SON_L2_RACH_PERF_REPORT parsing failed");
        /* Free the memory when parsing is failed */
        son_mem_free(p_parsed_msg);
        p_parsed_msg = SON_PNULL;
    }
    else
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_INFO,
                "RRM->SONRACH:RRM_SON_L2_RACH_PERF_REPORT");
    }

    SON_UT_TRACE_EXIT();
    return p_parsed_msg;
} /* rach_opt_parse_l2_rach_perf_report */

/*****************************************************************************
 * Function Name  : rach_opt_parse_rrm_msgs
 * Inputs         : p_msg  - Pointer to the message received
 *                  api_id - Procedure code of the message
 * Outputs        : None
 * Returns        : Pointer to the parsed message
 * Description    : This function is used to parse the messages received from
 *                  RRM
 ****************************************************************************/
static son_u8 *
rach_opt_parse_rrm_msgs
(
    son_u8  *p_msg,
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
                p_parsed_msg = rach_opt_parse_cell_state_change_ind(p_msg, 
                        buf_len);
                break;
            }
        case RRM_SON_RACH_CONFIG_RESP:
            {
                p_parsed_msg = rach_opt_parse_rach_config_resp(p_msg,
                        buf_len);
                break;
            }
        case RRM_SON_RACH_UE_INFO_IND:
            {
                p_parsed_msg = rach_opt_parse_rach_ue_info_ind(p_msg,
                        buf_len);
                break;
            }
        case RRM_SON_L2_RACH_PERF_REPORT:
            {
                p_parsed_msg = rach_opt_parse_l2_rach_perf_report(p_msg,
                        buf_len);
                /*SPR 17777 +-*/
                break;
            }
        default:
            {
                SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                        p_son_rach_opt_facility_name, SON_ERROR, 
                        "Unexpected Message: %u "
                        "received from RRM", api_id);
                break;
            }
    } /* switch */

    SON_UT_TRACE_EXIT();
    return p_parsed_msg;
} /* rach_opt_parse_rrm_msgs */

/*****************************************************************************
 * Function Name  : rach_opt_get_msg_idx_from_mif_msgs
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 * Outputs        : None
 * Returns        : Index in SON RACH OPT FSM
 * Description    : This function computes the index in the RACH OPT Global
 *                  FSM for the messages received from SONMgmtIfH
 ****************************************************************************/
static son_rach_opt_index_et
rach_opt_get_msg_idx_from_mif_msgs
(
    son_u16                 from,
    son_procedure_code_et   api
)
{
    son_rach_opt_index_et ret = RACH_OPT_IDX_NOT_AVAILABLE;
    SON_UT_TRACE_ENTER();
    
    switch (api)
    {
        case SMIF_INIT_CONFIG_REQ:
            {
                ret = RACH_OPT_IDX_SMIF_EVENT_INIT_CONFIG_REQ;
                break;
            }
        case SMIF_SET_LOG_LEVEL_CMD:
            {
                ret = RACH_OPT_IDX_SMIF_EVENT_SET_LOG_LEVEL_CMD;
                break;
            }
        case SMIF_LOG_ENABLE_DISABLE_CMD:
            {
                ret = RACH_OPT_IDX_SMIF_EVENT_LOG_ENABLE_DISABLE_CMD;
                break;
            }
        case SMIF_SHUTDOWN_REQ:
            {
                ret = RACH_OPT_IDX_SMIF_EVENT_SHUTDOWN_REQ;
                break;
            }
        case SMIF_CELL_INFO_IND:
            {
                ret = RACH_OPT_IDX_SMIF_EVENT_CELL_INFO_IND;
                break;
            }
        case SONRACH_OPT_ENABLE_REQ:
            {
                ret = RACH_OPT_IDX_SMIF_EVENT_RACH_OPT_ENABLE_REQ;
                break;
            }
        case SONRACH_OPT_DISABLE_REQ:
            {
                ret = RACH_OPT_IDX_SMIF_EVENT_RACH_OPT_DISABLE_REQ;
                break;
            }
        case SONRACH_OPT_MODIFY_CONFIG_PARAMS_REQ:
            {
                ret = RACH_OPT_IDX_SMIF_EVENT_RACH_OPT_MODIFY_CONFIG_PARAM_REQ;
                break;
            }
        case  SONRACH_OPT_MODIFY_RACH_OPTMODE_REQ:
            {
                ret = RACH_OPT_IDX_SMIF_EVENT_RACH_OPT_MODIFY_RACH_OPTMODE_REQ;
                break;
            }

            /* Rel 3.0 Cell delete support changes Start */
        case SMIF_CELL_DELETE_REQ:
            {
                ret = RACH_OPT_IDX_SMIF_EVENT_CELL_DELETE_REQ;
                break;
            }
            /* Rel 3.0 Cell delete support changes Stop */
            /* SPR-13251 Fix Starts */
        case SMIF_GET_LOG_LEVEL_REQ:
            {
                ret = RACH_OPT_IDX_SMIF_EVENT_GET_LOG_LEVEL_REQ;
                break;
            }
            /* SPR-13251 Fix Ends */
        default:
            {
                SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                        p_son_rach_opt_facility_name, SON_WARNING,
                        "Invalid API = %u received from %u", api, from);
                break;
            }
    }

    SON_UT_TRACE_EXIT();
    return  ret;
} /* rach_opt_get_msg_idx_from_mif_msgs */

/*****************************************************************************
 * Function Name  : rach_opt_get_msg_idx_from_rrm_msgs
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 * Outputs        : None
 * Returns        : Index in RACH OPT Global FSM
 * Description    : This function computes the index in the RACH OPT Global 
 *                  FSM for the messages received from RRM.
 ****************************************************************************/
static son_rach_opt_index_et
rach_opt_get_msg_idx_from_rrm_msgs
(
    son_u16 from,
    son_u16 api
)
{
    son_rach_opt_index_et ret = RACH_OPT_IDX_NOT_AVAILABLE;
    SON_UT_TRACE_ENTER();

    switch (api)
    {
        case RRM_SON_CELL_STATE_CHANGE_IND:
            {
                ret = RACH_OPT_IDX_RRM_EVENT_CELL_STATE_CHANGE_IND;
                break;
            }
        case RRM_SON_RACH_CONFIG_RESP:
            {
                ret = RACH_OPT_IDX_RRM_EVENT_RACH_CONFIG_RESP;
                break;
            }
        case RRM_SON_RACH_UE_INFO_IND:
            {
                ret = RACH_OPT_IDX_RRM_EVENT_UE_INFO_IND;
                break;
            }
        case RRM_SON_L2_RACH_PERF_REPORT:
            {
                ret = RACH_OPT_IDX_RRM_EVENT_L2_IND;
                break;
            }
        default:
            {
                SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                        p_son_rach_opt_facility_name, SON_WARNING,
                        "Invalid API = %u received from %d", api, from);
                break;
            }
    } /* switch */

    SON_UT_TRACE_EXIT();
    return ret;
} /* rach_opt_get_msg_idx_from_rrm_msgs */

/*****************************************************************************
 * Function Name  : rach_opt_get_msg_idx_from_anr_msgs
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 * Outputs        : None
 * Returns        : Index in SON RACH OPT FSM
 * Description    : This function computes the index in the RACH OPT Global
 *                  FSM for the messages received from SON ANR
 ****************************************************************************/
static son_rach_opt_index_et
rach_opt_get_msg_idx_from_anr_msgs
(
    son_u16                 from,
    son_procedure_code_et   api
)
{
    son_rach_opt_index_et ret = RACH_OPT_IDX_NOT_AVAILABLE;
    SON_UT_TRACE_ENTER();
    
    switch (api)
    {
        case SONANR_DISABLE_IND:
            {
                ret = RACH_OPT_IDX_SONANR_CELL_ANR_MODE_IND;
                break;
            }
        default:
            {
                SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                        p_son_rach_opt_facility_name, SON_WARNING,
                        "Invalid API = %u received from %u", api, from);
                break;
            }
    }

    SON_UT_TRACE_EXIT();
    return  ret;
} /* rach_opt_get_msg_idx_from_anr_msgs */

/*****************************************************************************
 * Function Name  : rach_opt_calculate_msg_index
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 * Outputs        : None
 * Returns        : Index in SON RACH OPT Global FSM
 * Description    : This function computes the index in the RACH OPT 
 *                  Global FSM for the given procedure code
 ****************************************************************************/
static son_rach_opt_index_et
rach_opt_calculate_msg_index
(
    son_u16                 from,
    son_procedure_code_et   api
)
{
    son_rach_opt_index_et ret = RACH_OPT_IDX_NOT_AVAILABLE;
    SON_UT_TRACE_ENTER();

    if (SON_MIF_MODULE_ID == from)
    {
        ret = rach_opt_get_msg_idx_from_mif_msgs(from, api);
    }
    else if (SON_RRM_MODULE_ID == from)
    {
        ret = rach_opt_get_msg_idx_from_rrm_msgs(from, api);
    }
    else if (SON_ANR_MODULE_ID == from)
    {
        ret = rach_opt_get_msg_idx_from_anr_msgs(from, api);
    }
    else
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_WARNING,
                "Message with API Id = %u "
                "received from unknown entity: %u", api, from);
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* rach_opt_calculate_msg_index */

/************************* RACH OPT Global State Machine*********************/
typedef son_buf_retain_status_et (* p_rach_opt_fsm_evt_handler_t)
    (son_u8* p_msg, STACKAPIHDR *p_cspl_hdr, son_rach_opt_index_et event_id);

/***********************************************************************
 * RACH OPT Global FSM Entry Point Table
 **********************************************************************/
static p_rach_opt_fsm_evt_handler_t
son_rach_opt_fsm_table[RACH_OPT_NUM_OF_STATES][RACH_OPT_NUM_OF_EVENTS] = 
{
    /* State INIT */
    {
        rach_opt_fsm_init_config_req_handler,
        /* SMIF_EVENT_INIT_CONFIG_REQ */
        rach_opt_fsm_invalid_event_handler,
        /* SMIF_EVENT_SET_LOG_LEVEL_CMD*/
        rach_opt_fsm_invalid_event_handler,
        /*SMIF_LOG_ENABLE_DISABLE_CMD*/
        rach_opt_fsm_invalid_event_handler,
        /* SMIF_EVENT_CELL_INFO_IND */
        rach_opt_fsm_shutdown_req_handler,
        /* SMIF_EVENT_SHUTDOWN_REQ */
        rach_opt_fsm_nack_enable_req_handler,
        /* SONRACH_OPT_EVENT_ENABLE_REQ */
        rach_opt_fsm_nack_disable_req_handler,
        /* SONRACH_OPT_EVENT_DISABLE_REQ */
        rach_opt_fsm_nack_modify_config_param_req_handler,
        /* SONRACH_OPT_EVENT_MODIFY_CONFIG_PARAM_REQ */
        rach_opt_fsm_nack_modify_rach_optmode_req_handler,
        /* SONRACH_OPT_EVENT_MODIFY_RACH_OPTMODE_REQ */
        rach_opt_fsm_invalid_event_handler,  
        /* RRM_EVENT_RACH_CONFIG_RES */
        rach_opt_fsm_invalid_event_handler,  
        /* RRM_EVENT_RACH_UE_INFO_IND */
        rach_opt_fsm_invalid_event_handler,  
        /* RRM_EVENT_RACH_L2_IND*/
        rach_opt_fsm_invalid_event_handler,  
        /* RRM_EVENT_CELL_STATE_CHANGE_IND */
        rach_opt_fsm_invalid_event_handler,  
        /* SONANR_EVENT_DISABLE_IND */
        /* Rel 3.0 Cell delete support changes Start */
        rach_opt_fsm_invalid_event_handler,  
        /* SMIF_CELL_DELETE_REQ */
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        rach_opt_fsm_invalid_event_handler,  
        /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    },
    /* State ACTIVE */
    {
        rach_opt_fsm_nack_init_config_req_handler,
        /* SMIF_EVENT_INIT_CONFIG_REQ */
        rach_opt_fsm_set_log_level_cmd_handler,
        /*SMIF_EVENT_SET_LOG_LEVEL_CMD*/
        rach_opt_fsm_log_enable_disable_cmd_handler,
        /*SMIF_LOG_ENABLE_DISABLE_CMD*/
        rach_opt_fsm_cell_info_ind_active_state_handler,
        /* SMIF_EVENT_CELL_INFO_IND */
        rach_opt_fsm_shutdown_req_handler,
        /* SMIF_EVENT_SHUTDOWN_REQ */
        rach_opt_fsm_enable_req_handler,
        /* SONRACH_OPT_EVENT_ENABLE_REQ */
        rach_opt_fsm_disable_req_handler,
        /* SONRACH_OPT_EVENT_DISABLE_REQ */
        rach_opt_fsm_modify_config_param_req_handler,
        /* SONRACH_OPT_EVENT_MODIFY_CONFIG_PARAM_REQ */
        rach_opt_fsm_modify_rach_optmode_req_handler,
        /* SONRACH_OPT_EVENT_MODIFY_RACH_OPTMODE_REQ */
        rach_opt_fsm_rach_config_res_handler,  
        /* RRM_EVENT_RACH_CONFIG_RES */
        rach_opt_fsm_rach_ue_ind_handler,  
        /* RRM_EVENT_RACH_UE_INFO_IND */
        rach_opt_fsm_rach_l2_ind_handler,  
        /* RRM_EVENT_RACH_L2_IND*/
        rach_opt_fsm_cell_state_change_ind_handler,  
        /* RRM_EVENT_CELL_STATE_CHANGE_IND */
        rach_opt_fsm_cell_anr_mode_ind_handler,
        /* SONANR_EVENT_DISABLE_IND */
        /* Rel 3.0 Cell delete support changes Start */
        rach_opt_fsm_cell_delete_req_handler,
        /* SMIF_CELL_DELETE_REQ */
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        rach_opt_fsm_get_log_level_req_handler,
        /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    },
    /* State ENABLING */
    {
        rach_opt_fsm_nack_init_config_req_handler,
        /* SMIF_EVENT_INIT_CONFIG_REQ */
        rach_opt_fsm_set_log_level_cmd_handler,
        /*SMIF_EVENT_SET_LOG_LEVEL_CMD*/
        rach_opt_fsm_log_enable_disable_cmd_handler,
        /*SMIF_LOG_ENABLE_DISABLE_CMD*/
        rach_opt_fsm_cell_info_ind_enabling_state_handler,
        /* SMIF_EVENT_CELL_INFO_IND */
        rach_opt_fsm_shutdown_req_handler,
        /* SMIF_EVENT_SHUTDOWN_REQ */
        rach_opt_fsm_nack_enable_req_handler,
        /* SONRACH_OPT_EVENT_ENABLE_REQ */
        rach_opt_fsm_nack_disable_req_handler,
        /* SONRACH_OPT_EVENT_DISABLE_REQ */
        rach_opt_fsm_modify_config_param_req_handler,
        /* SONRACH_OPT_EVENT_MODIFY_CONFIG_PARAM_REQ */
        rach_opt_fsm_modify_rach_optmode_req_handler,
        /* SONRACH_OPT_EVENT_MODIFY_RACH_OPTMODE_REQ */
        rach_opt_fsm_rach_config_res_handler,  
        /* RRM_EVENT_RACH_CONFIG_RES */
        rach_opt_fsm_rach_ue_ind_handler,  
        /* RRM_EVENT_RACH_UE_INFO_IND */
        rach_opt_fsm_rach_l2_ind_handler,  
        /* RRM_EVENT_RACH_L2_IND*/
        rach_opt_fsm_cell_state_change_ind_handler,  
        /* RRM_EVENT_CELL_STATE_CHANGE_IND */
        rach_opt_fsm_cell_anr_mode_ind_handler,
        /* SONANR_EVENT_DISABLE_IND */
        /* Rel 3.0 Cell delete support changes Start */
        rach_opt_fsm_cell_delete_req_handler,
        /* SMIF_CELL_DELETE_REQ */
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        rach_opt_fsm_get_log_level_req_handler,
        /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    },
    /* State DISABLING */
    {
        rach_opt_fsm_nack_init_config_req_handler,
        /* SMIF_EVENT_INIT_CONFIG_REQ */
        rach_opt_fsm_set_log_level_cmd_handler,
        /* SMIF_EVENT_SET_LOG_LEVEL_CMD*/
        rach_opt_fsm_log_enable_disable_cmd_handler,
        /*SMIF_LOG_ENABLE_DISABLE_CMD*/
        rach_opt_fsm_cell_info_ind_disabling_state_handler,
        /* SMIF_EVENT_CELL_INFO_IND */
        rach_opt_fsm_shutdown_req_handler,
        /* SMIF_EVENT_SHUTDOWN_REQ */
        rach_opt_fsm_nack_enable_req_handler,
        /* SONRACH_OPT_EVENT_ENABLE_REQ */
        rach_opt_fsm_nack_disable_req_handler,
        /* SONRACH_OPT_EVENT_DISABLE_REQ */
        rach_opt_fsm_modify_config_param_req_handler,
        /* SONRACH_OPT_EVENT_MODIFY_CONFIG_PARAM_REQ */
        rach_opt_fsm_modify_rach_optmode_req_handler,
        /* SONRACH_OPT_EVENT_MODIFY_RACH_OPTMODE_REQ */
        rach_opt_fsm_rach_config_res_handler,  
        /* RRM_EVENT_RACH_CONFIG_RES */
        rach_opt_fsm_rach_ue_ind_handler,  
        /* RRM_EVENT_RACH_UE_INFO_IND */
        rach_opt_fsm_rach_l2_ind_handler,  
        /* RRM_EVENT_RACH_L2_IND*/
        rach_opt_fsm_cell_state_change_ind_handler,  
        /* RRM_EVENT_CELL_STATE_CHANGE_IND */
        rach_opt_fsm_cell_anr_mode_ind_handler,
        /* SONANR_EVENT_DISABLE_IND */
        /* Rel 3.0 Cell delete support changes Start */
        rach_opt_fsm_cell_delete_req_handler,
        /* SMIF_CELL_DELETE_REQ */
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        rach_opt_fsm_get_log_level_req_handler,
        /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    },
    /* State SHUTDOWN */
    {
        rach_opt_fsm_shutdown_state_handler,
        /* SMIF_EVENT_INIT_CONFIG_REQ */
        rach_opt_fsm_shutdown_state_handler,
        /*SMIF_EVENT_SET_LOG_LEVEL_CMD*/
        rach_opt_fsm_shutdown_state_handler,
        /*SMIF_LOG_ENABLE_DISABLE_CMD*/
        rach_opt_fsm_shutdown_state_handler,
        /* SMIF_EVENT_CELL_INFO_IND */
        rach_opt_fsm_shutdown_state_handler,
        /* SMIF_EVENT_SHUTDOWN_REQ */
        rach_opt_fsm_shutdown_state_handler,
        /* SONRACH_OPT_EVENT_ENABLE_REQ */
        rach_opt_fsm_shutdown_state_handler,
        /* SONRACH_OPT_EVENT_DISABLE_REQ */
        rach_opt_fsm_shutdown_state_handler,
        /* SONRACH_OPT_EVENT_MODIFY_CONFIG_PARAM_REQ */
        rach_opt_fsm_shutdown_state_handler,
        /* SONRACH_OPT_EVENT_MODIFY_RACH_OPTMODE_REQ */
        rach_opt_fsm_shutdown_state_handler,
        /* RRM_EVENT_RACH_CONFIG_RES */
        rach_opt_fsm_shutdown_state_handler,
        /* RRM_EVENT_RACH_UE_INFO_IND */
        rach_opt_fsm_shutdown_state_handler,
        /* RRM_EVENT_RACH_L2_IND*/
        rach_opt_fsm_shutdown_state_handler,
        /* RRM_EVENT_CELL_STATE_CHANGE_IND */
        rach_opt_fsm_shutdown_state_handler,
        /* SONANR_EVENT_DISABLE_IND */
        /* Rel 3.0 Cell delete support changes Start */
        rach_opt_fsm_shutdown_state_handler,
        /* SMIF_CELL_DELETE_REQ */
        /* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        rach_opt_fsm_shutdown_state_handler,
        /* SMIF_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    }
};

/*****************************************************************************
 * Function Name  : son_rach_opt_process_event
 * Inputs         : p_msg - Pointer to the message received by RACH OPT module
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This function is used to invoke the appropriate event
 *                  handler for the messages received by SON RACH OPT Module. It
 *                  calculates the API Id, finds current FSM state and based
 *                  on this access the son_rach_opt_fsm_table to call the
 *                  appropriate handler.
 ****************************************************************************/
son_buf_retain_status_et
son_rach_opt_process_event
(
    son_void_t *p_msg
)
{
    son_rach_opt_index_et  idx = RACH_OPT_IDX_NOT_AVAILABLE;
    STACKAPIHDR cspl_hdr = {0};
    son_u8 *p_parsed_msg = SON_PNULL;
    SON_UT_TRACE_ENTER();

    /* Parse the cspl header */
    son_parse_cspl_header(p_msg, &cspl_hdr);

    /* Calculate global FSM index */
    idx = rach_opt_calculate_msg_index(cspl_hdr.from,
            (son_procedure_code_et)cspl_hdr.api);

    /* Validate the index of son_rach_opt_fsm_table array*/
    if (RACH_OPT_NUM_OF_EVENTS <= idx)
    {
        /* No need to print the log here for invalid API Id as it is
           already printed in rach_opt_calculate_msg_index function */
        ; /* Do nothing */
    }
    else if (RACH_OPT_NUM_OF_STATES <=
            g_rach_opt_gb_ctxt.current_fsm_state)
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_WARNING,
                "Message with API Id = %u received"
                " in invalid state = %u", cspl_hdr.api,
                g_rach_opt_gb_ctxt.current_fsm_state);
    }
    else
    {
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_BRIEF,
                "Message with API Id = %u received"
                " by SON RACH OPT in state %s", cspl_hdr.api,
                SON_RACH_OPT_FSM_STATES_NAMES[
                g_rach_opt_gb_ctxt.current_fsm_state]);

        /* Move the pointer such that it points to the message data after the
           CSPL header */
        p_msg += CV_HDRSIZE;

        /* Parse the incoming message received from RRM */
        if (SON_RRM_MODULE_ID == cspl_hdr.from)
        {
            /* Parse message received from RRM */
            p_parsed_msg = rach_opt_parse_rrm_msgs(p_msg,
                    cspl_hdr.api);

            if (SON_PNULL != p_parsed_msg)
            {
                p_msg = p_parsed_msg;
            }
            else
            {
                SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                        p_son_rach_opt_facility_name, SON_ERROR,
                        "Parsing failed for "
                        "received message: %u in state %s", cspl_hdr.api,
                        SON_RACH_OPT_FSM_STATES_NAMES[
                        g_rach_opt_gb_ctxt.current_fsm_state]);

                SON_UT_TRACE_EXIT();
                return RELEASE_SON_BUFFER;
            }
        }

        /* Call the appropriate message handler based on the API Id and
           source module id */
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_INFO,
                "### RACH_FSM: Processing state %s Event %s ###",
               SON_RACH_OPT_FSM_STATES_NAMES[g_rach_opt_gb_ctxt.current_fsm_state],
               SON_RACH_OPT_FSM_EVENT_NAMES[idx]);
        (*son_rach_opt_fsm_table[g_rach_opt_gb_ctxt.current_fsm_state][idx])
            (p_msg, &cspl_hdr, idx);
        SON_LOG(g_rach_opt_gb_ctxt.log_mode,
                p_son_rach_opt_facility_name, SON_INFO,
                "### RACH_FSM: Processing complete ###");

        /* Free the memory taken for message received from RRM */
        if (SON_PNULL != p_parsed_msg)
        {
            son_mem_free(p_parsed_msg);
        }
    }

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* son_rach_opt_process_event */
