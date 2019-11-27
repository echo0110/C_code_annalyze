/****************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 * SPR-612 Coverity Fix Starts
 * $Id: son_apps_fsm.c $
 * SPR-612 Coverity Fix Ends
 *
 ****************************************************************************
 *
 * File Description: Processes the SONApps states and events. It contains
 *                   the definition of various event handlers for SONApps 
 *                   module.
 *
 ****************************************************************************
 * Revision Details
 * ----------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Feb, 2012       Shilpi Jain                 Initial code
 *   May, 2012       Ritika Chopra/Sonal Jain    
 *   Aug, 2014       Shilpi                      SPR 13251 Fix
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_apps_fsm.h>
#include <son_apps_pci.h>
#include <son_apps_acf.h>

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/* SONApps FSM state names */
const son_s8 *SON_APPS_FSM_STATES_NAMES[] =
{
    /* Typecasting strings to const son_s8 pointers to remove klocwork 
     *  warnings. */
    (const son_s8 *)"SONAPP_STATE_INIT",
    (const son_s8 *)"SONAPP_STATE_ACTIVE",
    (const son_s8 *)"SONAPP_STATE_SHUTDOWN",
    (const son_s8 *)"SONAPP_STATE_UNDEFINED"
};

const son_8 *sonapps_fsm_name = "SON APPS FSM";

/*****************************************************************************
 * Function Name  : son_apps_reset_context
 * Inputs         : p_sonapps_gb_context - Pointer to the SONApps global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function resets the SONApps global context
 ****************************************************************************/
son_void_t 
son_apps_reset_context
(
    son_apps_gb_context_t *p_sonapps_gb_context
)
{
    SON_UT_TRACE_ENTER();
    
    if (SON_PNULL == p_sonapps_gb_context)
    {
        SON_LOG(SON_OAM_LOG_ON, p_son_apps_facility_name, 
                SON_ERROR, 
                "Pointer to the Apps global context "
                "is NULL.");
        SON_UT_TRACE_EXIT();
        return;
    }

    /* Default logging mode is Enabled */
    p_sonapps_gb_context->g_sonapps_state = SONAPP_STATE_UNDEFINED;
    p_sonapps_gb_context->apps_log_enable = SON_OAM_LOG_ON;
    p_sonapps_gb_context->pci_log_enable = SON_OAM_LOG_ON;
    p_sonapps_gb_context->acf_log_enable = SON_OAM_LOG_ON;
    p_sonapps_gb_context->meas_bandwidth = SON_NULL;
    SON_MEMSET(p_sonapps_gb_context->transaction_id, 0xFF, 
        sizeof(p_sonapps_gb_context->transaction_id));
    p_sonapps_gb_context->pci_fsm_state = SONAPPS_PCI_ENABLED;
    p_sonapps_gb_context->pci_list_size = SON_NULL;

    if (SON_PNULL != p_sonapps_gb_context->pci_list)
    {
        son_mem_free(p_sonapps_gb_context->pci_list);
        p_sonapps_gb_context->pci_list = SON_PNULL;
    }

    p_sonapps_gb_context->num_nw_scan_req_sent_for_pci = SON_NULL;
    p_sonapps_gb_context->num_nw_scan_rsp_rcvd_for_pci = SON_NULL;
    p_sonapps_gb_context->meas_bw_list_size = SON_NULL;

    if (SON_PNULL != p_sonapps_gb_context->meas_bw_list)
    {
        son_mem_free(p_sonapps_gb_context->meas_bw_list);
        p_sonapps_gb_context->meas_bw_list = SON_PNULL;
    }

    p_sonapps_gb_context->pci_selection_earfcn = SONAPPS_INVALID_VALUE;
    SON_MEMSET(p_sonapps_gb_context->in_use_pci_list, SON_FALSE,
        sizeof(p_sonapps_gb_context->in_use_pci_list));
    SON_MEMSET(p_sonapps_gb_context->earfcn, 0xFF,
        sizeof(p_sonapps_gb_context->earfcn));
    SON_MEMSET(p_sonapps_gb_context->acf_selection_progress_status, SON_FALSE,
        sizeof(p_sonapps_gb_context->acf_selection_progress_status));

    /* SPR-13251 Fix Starts */
    p_sonapps_gb_context->log_level = SON_OAM_LOG_LEVEL_BRIEF;
    /* SPR-13251 Fix Ends */

    /*SPR-12944-Start*/
    p_sonapps_gb_context->nmm_support_present = SON_TRUE;
    /*SPR-12944-End*/

    SON_UT_TRACE_EXIT();
} /* son_apps_reset_context */

/*****************************************************************************
 * Function Name  : sonapps_fsm_init_config_req
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_INIT_CONFIG_REQ 
 *                  message received from SONMgmtIfH in SONAPP_STATE_INIT state.
 ****************************************************************************/
static son_buf_retain_status_et 
sonapps_fsm_init_config_req 
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
    (void)p_cspl_hdr;/*SPR 17777 +-*/
    smif_init_config_res_t smif_init_config_res = {0};
    smif_init_config_req_t *p_smif_init_config_req = 
        (smif_init_config_req_t *)p_msg;


    SON_UT_TRACE_ENTER();

    /* Print the info about the current logging status(enable/disable)
     *  Pass first argument as true such that this info is printed
     *  even if logging is disabled. */
    SON_LOG(SON_OAM_LOG_ON, p_son_apps_facility_name, SON_BRIEF,
        "Log enable/disable = %u "
        "[0-DISABLE, 1-ENABLE] for all SONApps features",
        p_sonapps_gb_context->apps_log_enable);

    /* Enable/Disable the SONApps (NW_SCAN feature) log */ 
    p_sonapps_gb_context->apps_log_enable = p_smif_init_config_req->
        log_enable;
    /* Enable/Disable the PCI feature log */
    p_sonapps_gb_context->pci_log_enable = p_smif_init_config_req->log_enable;
    /* Enable/Disable the ACF feature log */
    p_sonapps_gb_context->acf_log_enable = p_smif_init_config_req->log_enable;

    if ((SON_OAM_LOG_ON == p_sonapps_gb_context->apps_log_enable) ||
        (SON_OAM_LOG_ON == p_sonapps_gb_context->pci_log_enable) ||
        (SON_OAM_LOG_ON == p_sonapps_gb_context->acf_log_enable))
    {
        /* SPR-13251 Fix Starts */
        p_sonapps_gb_context->log_level = p_smif_init_config_req->log_level;
        /* SPR-13251 Fix Ends */
        SET_MODULE_LOG_LEVEL(p_smif_init_config_req->log_level);
    }
    else
    {
        /* Here there is no need to check if logging is disabled for all
         *  SONApps features because for SMIF_INIT_CONFIG_REQ message,
         *  logging is Enabled or Disabled for all SONApps features.
         *  Set module log level to 0 since logging is disabled
         *  for all SONApps features */
        /* SPR-13251 Fix Starts */
        p_sonapps_gb_context->log_level = SON_LOG_LEVEL_NONE;
        /* SPR-13251 Fix Ends */
        SET_MODULE_LOG_LEVEL(SON_LOG_LEVEL_NONE);
    }
    /*SPR-12944-Start*/
    p_sonapps_gb_context->nmm_support_present = 
        p_smif_init_config_req->is_nmm_supported;
    /*SPR-12944-End*/

    /* Populate SMIF_INIT_CONFIG_RES message data */
    smif_init_config_res.transaction_id = 
      p_smif_init_config_req->transaction_id;
    smif_init_config_res.result = SON_SUCCESS;
    smif_init_config_res.error_code = SON_NO_ERROR;

    /* Set the SONApps FSM state to ACTIVE */
    p_sonapps_gb_context->g_sonapps_state = SONAPP_STATE_ACTIVE;

    SON_LOG(p_sonapps_gb_context->apps_log_enable, p_son_apps_facility_name,
        SON_INFO, "### %s State changed to %s ###", sonapps_fsm_name,
        SON_APPS_FSM_STATES_NAMES[SONAPP_STATE_ACTIVE]);

    /* Send SMIF_INIT_CONFIG_RES to SONMgmtIfh */
    son_create_send_buffer((son_u8 *)&smif_init_config_res, SON_APPS_MODULE_ID,
        SON_MIF_MODULE_ID, SMIF_INIT_CONFIG_RES,
        sizeof(smif_init_config_res_t));

    SON_UT_TRACE_EXIT();
    /* Release the buffer memory as it is no longer needed. */
    return RELEASE_SON_BUFFER;
} /* sonapps_fsm_init_config_req */

/*****************************************************************************
 * Function Name  : sonapps_fsm_invalid_scan_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This function is used to handle the 
 *                  SONAPPS_NW_SCAN_REQ message received 
 *                  from SONMgmtIfH in SONAPP_STATE_INIT state. It sends the
 *                  response message with result = SON_FAILURE to SONMgmtIfH 
 *                  as the message is received in invalid state and can't be 
 *                  handled. It also does the clean up of the message memory 
 *                  taken by the SONMgmtIfH.
 ****************************************************************************/
static son_buf_retain_status_et 
sonapps_fsm_invalid_scan_req_handler
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
    sonapps_nw_scan_res_t apps_scan_res = {0};
    /* Get the scan req message data */
    sonapps_nw_scan_req_t *p_sonapps_nw_scan_req = (sonapps_nw_scan_req_t *)
        p_msg;


    SON_UT_TRACE_ENTER();
    if (SON_NULL != p_sonapps_nw_scan_req->eutran_scan_req.transaction_id)
    {    
        /* Populate apps scan response structure */
        apps_scan_res.transaction_id = 
            p_sonapps_nw_scan_req->eutran_scan_req.transaction_id;
        apps_scan_res.result = SON_FAILURE;
        apps_scan_res.error_code = SON_ERR_UNEXPECTED_MSG_IN_INIT_STATE;

        /* Send SONAPPS_EVENT_NW_SCAN_RES to SONMgmtIfh with result = 
         *  SON_FAILURE and error = SON_ERR_UNEXPECTED_MSG_IN_INIT_STATE. */
        son_create_send_buffer((son_u8 *)&apps_scan_res, SON_APPS_MODULE_ID,
            SON_MIF_MODULE_ID, SONAPPS_NW_SCAN_RES,
            sizeof(sonapps_nw_scan_res_t));

        SON_LOG(p_sonapps_gb_context->apps_log_enable,
            p_son_apps_facility_name,
            SON_BRIEF,
            "SONAPPS_NW_SCAN_RES "
            "message sent to [%u] with transaction Id = [%u]",
            p_cspl_hdr->from,
        apps_scan_res.transaction_id);

    /* Free the memory of scan req data pointer received from SMIF */
    son_mem_free(p_sonapps_nw_scan_req->eutran_scan_req.p_start_scan_req);
    p_sonapps_nw_scan_req->eutran_scan_req.p_start_scan_req = SON_PNULL;
    SON_LOG(p_sonapps_gb_context->apps_log_enable, p_son_apps_facility_name, 
        SON_DETAILED, "Memory has been "
        "freed for SONAPPS_NW_SCAN_REQ message with transaction Id = [%u]", 
        apps_scan_res.transaction_id);
    }
    else
    {
        /* Populate apps UTRA scan response structure */
        apps_scan_res.transaction_id = 
            p_sonapps_nw_scan_req->utran_scan_req.transaction_id;
        apps_scan_res.result = SON_FAILURE;
        apps_scan_res.error_code = SON_ERR_UNEXPECTED_MSG_IN_INIT_STATE;

        /* Send SONAPPS_EVENT_NW_SCAN_RES to SONMgmtIfh with result = 
         *  SON_FAILURRE and error = SON_ERR_UNEXPECTED_MSG_IN_INIT_STATE. */
        son_create_send_buffer((son_u8 *)&apps_scan_res, SON_APPS_MODULE_ID,
                SON_MIF_MODULE_ID, SONAPPS_NW_SCAN_RES,
                sizeof(sonapps_nw_scan_res_t));

        SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                p_son_apps_facility_name, SON_BRIEF, 
                "SONAPPS_NW_SCAN_RES "
                "message sent to [%u] with transaction Id = [%u]", p_cspl_hdr->from,
                apps_scan_res.transaction_id);

        /* Free the memory of scan req data pointer received from SMIF */
        son_mem_free(p_sonapps_nw_scan_req->utran_scan_req.p_start_scan_req);
        p_sonapps_nw_scan_req->utran_scan_req.p_start_scan_req = SON_PNULL;
        SON_LOG(p_sonapps_gb_context->apps_log_enable, p_son_apps_facility_name,
                SON_DETAILED, 
                "Memory has been freed"
                " for SONAPPS_NW_SCAN_REQ message with transaction Id = [%u]",
                apps_scan_res.transaction_id);
    }

    SON_UT_TRACE_EXIT();
    /* Release the buffer memory as it is no longer needed. */
    return RELEASE_SON_BUFFER;
} /* sonapps_fsm_invalid_scan_req_handler */

/*****************************************************************************
 * Function Name  : sonapps_fsm_active_state_nack_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_INIT_CONFIG_REQ 
 *                  message received from SONMgmtIfH in 
 *                  SONAPP_STATE_ACTIVE_state.
 ****************************************************************************/
static son_buf_retain_status_et 
sonapps_fsm_active_state_nack_handler 
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
    smif_init_config_res_t smif_init_config_res = {0};
    smif_init_config_req_t *p_smif_init_config_req =
       (smif_init_config_req_t *)p_msg;


    SON_UT_TRACE_ENTER();

    /* Populate the init config response structure */
    smif_init_config_res.transaction_id = 
        p_smif_init_config_req->transaction_id;
    smif_init_config_res.result = SON_FAILURE;
    smif_init_config_res.error_code = SON_ERR_UNEXPECTED_MSG_IN_ACTIVE_STATE;

    /* Sends SMIF_INIT_CONFIG_RES to SONMgmtIfh */
    son_create_send_buffer((son_u8 *)&smif_init_config_res,
        SON_APPS_MODULE_ID, SON_MIF_MODULE_ID, SMIF_INIT_CONFIG_RES,
        sizeof(smif_init_config_res_t));

    SON_LOG(p_sonapps_gb_context->apps_log_enable, p_son_apps_facility_name, 
        SON_BRIEF,
        "SMIF_INIT_CONFIG_RES message sent to [%u] with transaction Id = [%u]", 
        p_cspl_hdr->from, smif_init_config_res.transaction_id);

    SON_UT_TRACE_EXIT();
    /* Release the buffer memory as it is no longer needed. */
    return RELEASE_SON_BUFFER;
} /* sonapps_fsm_active_state_nack_handler */

/*****************************************************************************
 * Function Name  : set_log_enable_disable
 * Inputs         : p_sonapps_gb_context - Pointer to the global context
 *                  structure
 *                  p_enable_disable_cmd - Pointer to enable/disable
 *                  command structure
 * Outputs        : None
 * Returns        : None
 * Description    : This function sets the log mode (Enable or Disable) for 
 *                  SONApps features at runtime
 ****************************************************************************/
static son_void_t
set_log_enable_disable
(
    son_apps_gb_context_t           *p_sonapps_gb_context,
    smif_log_enable_disable_cmd_t   *p_enable_disable_cmd
)
{
   son_oam_log_on_off_et log_enable_disable = (son_oam_log_on_off_et)p_enable_disable_cmd->log_enable; 
    son_u8 log_level = p_enable_disable_cmd->log_level;


    /* Print the info about the current logging status(enable/disable)
     *  Pass first argument as true such that this info is printed
     *  even if logging is disabled. */
    SON_LOG(SON_OAM_LOG_ON, p_son_apps_facility_name, SON_INFO,
        "set_log_enable_disable: Log enable/disable = %u "
        "[0-DISABLE, 1-ENABLE] for feature = %u "
        "[0-ALL_FEATURES, 1-PCI_FEATURE, 2-ACF_FEATURE, 3-NW_SCAN_FEATURE]",
        log_enable_disable, p_enable_disable_cmd->feature_id);

    switch (p_enable_disable_cmd->feature_id)
    {
        case SON_ALL_FEATURE:
        {
            /* If feature id is SON_ALL_FEATURE then set log mode for
             *  all SONApps features - SON_PCI_FEATURE, SON_ACF_FEATURE
             *  and SON_NW_SCAN_FEATURE */
            p_sonapps_gb_context->apps_log_enable = log_enable_disable;
            p_sonapps_gb_context->pci_log_enable = log_enable_disable;
            p_sonapps_gb_context->acf_log_enable = log_enable_disable;
            break;
        }

        case SON_PCI_FEATURE:
        {
            /* If feature id is SON_PCI_FEATURE then set log mode only
             *  for SON_PCI_FEATURE */
            p_sonapps_gb_context->pci_log_enable = log_enable_disable;
            break;
        }

        case SON_ACF_FEATURE:
        {
            /* If feature id is SON_ACF_FEATURE then set log mode only
             *  for SON_ACF_FEATURE */
            p_sonapps_gb_context->acf_log_enable = log_enable_disable;
            break;
        }

        case SON_NW_SCAN_FEATURE:
        {
            /* If feature id is SON_NW_SCAN_FEATURE then set log mode only
             *  for SON_NW_SCAN_FEATURE */
            p_sonapps_gb_context->apps_log_enable = log_enable_disable;
        }

        default:
        {
            /* Do nothing */
            break;
        }
    } /* switch */

    if (log_enable_disable == SON_OAM_LOG_ON)
    {
        if (p_enable_disable_cmd->bitmask & SMIF_MODULE_LOG_LEVEL_PRESENT)
        {
            /* SPR-13251 Fix Starts */
            p_sonapps_gb_context->log_level = log_level;
            /* SPR-13251 Fix Ends */
            /* Set module log level to log_level received */
            SET_MODULE_LOG_LEVEL(log_level);
            SON_LOG(SON_OAM_LOG_ON, p_son_apps_facility_name, SON_DETAILED,
                    "Log Level Set :: %u ", log_level);
        }
    }
} /* set_log_enable_disable */

/*****************************************************************************
 * Function Name  : sonapps_fsm_log_enable_cmd
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_LOG_ENABLE_DISABLE_CMD
 *                  message received from SONMgmtIfH in 
 *                  SONAPP_STATE_ACTIVE_state.
 ****************************************************************************/
static son_buf_retain_status_et 
sonapps_fsm_log_enable_cmd 
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
    (void)p_cspl_hdr;/*SPR 17777 +-*/
    smif_log_enable_disable_cmd_t *p_enable_disable_cmd = 
        (smif_log_enable_disable_cmd_t *)p_msg;


    SON_UT_TRACE_ENTER();
    
    /* Function call to Enable/Disable logging for SONApps features */
    set_log_enable_disable(p_sonapps_gb_context, p_enable_disable_cmd);
       
    if ((SON_OAM_LOG_OFF == p_sonapps_gb_context->apps_log_enable) &&
        (SON_OAM_LOG_OFF == p_sonapps_gb_context->pci_log_enable) &&
        (SON_OAM_LOG_OFF == p_sonapps_gb_context->acf_log_enable))
    {
        SON_LOG(SON_OAM_LOG_ON, p_son_apps_facility_name, SON_DETAILED,
            "Logging disabled for all SONApps features, Set LOG_LEVEL :: %u ",
            SON_LOG_LEVEL_NONE);
         
        /* Set module log level to 0 once logging is disabled for
         *  all SONApps features */
        /* SPR-13251 Fix Starts */
        p_sonapps_gb_context->log_level = SON_LOG_LEVEL_NONE;
        /* SPR-13251 Fix Ends */
        SET_MODULE_LOG_LEVEL(SON_LOG_LEVEL_NONE);
    }

    SON_UT_TRACE_EXIT();
    /* Release the buffer memory as it is no longer needed. */
    return RELEASE_SON_BUFFER;
} /* sonapps_fsm_log_enable_cmd */

/*****************************************************************************
 * Function Name  : sonapps_fsm_set_log_level_cmd
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_SET_LOG_LEVEL_CMD 
 *                  message received from SONMgmtIfH in 
 *                  SONAPP_STATE_ACTIVE_state.
 ****************************************************************************/
static son_buf_retain_status_et 
sonapps_fsm_set_log_level_cmd 
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
    (void)p_cspl_hdr;/*SPR 17777 +-*/
    smif_set_log_level_cmd_t *p_smif_set_log_level_cmd = 
        (smif_set_log_level_cmd_t *)p_msg;


    SON_UT_TRACE_ENTER();

    /* Set module log level when logging is enabled for any of SONApps
     *  features */
    if ((SON_OAM_LOG_ON == p_sonapps_gb_context->apps_log_enable) ||
        (SON_OAM_LOG_ON == p_sonapps_gb_context->pci_log_enable) ||
        (SON_OAM_LOG_ON == p_sonapps_gb_context->acf_log_enable))
    {
        /* SPR-13251 Fix Starts */
        p_sonapps_gb_context->log_level = p_smif_set_log_level_cmd->log_level;
        /* SPR-13251 Fix Ends */
        SET_MODULE_LOG_LEVEL(p_smif_set_log_level_cmd->log_level);
    }        
    
    SON_UT_TRACE_EXIT();
    /* Release the buffer memory as it is no longer needed. */
    return RELEASE_SON_BUFFER;
} /* sonapps_fsm_set_log_level_cmd */

/*****************************************************************************
 * Function Name  : son_buf_retain_status_et
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SMIF_SHUTDOWN_REQ message 
 *                  received from SONMgmtIfh in SONAPP_STATE_ACTIVE_state.
 ****************************************************************************/
static son_buf_retain_status_et
sonapps_fsm_shutdown_req 
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
    (void)p_cspl_hdr;/*SPR 17777 +-*/
    smif_shutdown_res_t smif_shutdown_response = {0};
    smif_shutdown_req_t *p_smif_shutdown_req = 
        (smif_shutdown_req_t *)p_msg;


    SON_UT_TRACE_ENTER();

    son_apps_reset_context(p_sonapps_gb_context);

    /* Populate SMIF_SHUTDOWN_RES message data */
    smif_shutdown_response.transaction_id =
        p_smif_shutdown_req->transaction_id;
    smif_shutdown_response.result = SON_SUCCESS;
    smif_shutdown_response.error_code = SON_NO_ERROR;

    /* Sends SMIF_SHUTDOWN_RES to SONMgmtIfh */
    son_create_send_buffer((son_u8 *)&smif_shutdown_response, 
        SON_APPS_MODULE_ID, SON_MIF_MODULE_ID, SMIF_SHUTDOWN_RES,
        sizeof(smif_shutdown_res_t));

    /* Set the SONApps FSM state to SHUTDOWN */
    p_sonapps_gb_context->g_sonapps_state = SONAPP_STATE_SHUTDOWN;
    
    SON_LOG(p_sonapps_gb_context->apps_log_enable, p_son_apps_facility_name,
        SON_BRIEF, "### %s: State changed to %s ###", sonapps_fsm_name,
        SON_APPS_FSM_STATES_NAMES[SONAPP_STATE_SHUTDOWN]);
       
    SON_UT_TRACE_EXIT();
    /* Release the buffer memory as it is no longer needed. */
    return RELEASE_SON_BUFFER;
} /* sonapps_fsm_shutdown_req */

/*****************************************************************************
 * Function Name  : sonapps_fsm_nw_scan_req
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONAPPS_NW_SCAN_REQ 
 *                  message received from SONMgmtIfH in 
 *                  SONAPP_STATE_ACTIVE_state.
 ****************************************************************************/
static son_buf_retain_status_et
sonapps_fsm_nw_scan_req
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
    sonapps_nw_scan_req_t *p_sonapps_nw_scan_req = (sonapps_nw_scan_req_t *) p_msg;


    SON_UT_TRACE_ENTER();

    if (SON_PNULL != (p_sonapps_nw_scan_req->eutran_scan_req.p_start_scan_req))
    {
        /* Send SONNMM_START_SCAN_REQ to Nmm for EUTRAN*/
        son_create_send_buffer((son_u8 *)&(p_sonapps_nw_scan_req->eutran_scan_req), 
            SON_APPS_MODULE_ID, 
            SON_NMM_MODULE_ID, SONNMM_START_SCAN_REQ, 
            sizeof(son_nmm_start_scan_req_t));
        SON_LOG(p_sonapps_gb_context->apps_log_enable, p_son_apps_facility_name, 
            SON_BRIEF, "SONNMM_START_SCAN_REQ message sent to"
            "Nmm with transaction Id = %u",
            (p_sonapps_nw_scan_req->eutran_scan_req.transaction_id));
    }         
    else if (SON_PNULL != (p_sonapps_nw_scan_req->utran_scan_req.p_start_scan_req))
    {
        /* Send SONNMM_START_SCAN_REQ to Nmm for UTRAN*/
        son_create_send_buffer((son_u8 *)&(p_sonapps_nw_scan_req->utran_scan_req),
                SON_APPS_MODULE_ID,
                SON_NMM_MODULE_ID, SONNMM_START_SCAN_REQ,
                sizeof(son_nmm_start_scan_req_t));
        SON_LOG(p_sonapps_gb_context->apps_log_enable, p_son_apps_facility_name, 
            SON_BRIEF, "SONNMM_START_SCAN_REQ message sent to"
            "Nmm with transaction Id = %u",
            (p_sonapps_nw_scan_req->utran_scan_req.transaction_id));
    }
    else 
    {
        /*SPR 17777 +-*/
        SON_LOG(p_sonapps_gb_context->apps_log_enable, p_son_apps_facility_name,
                SON_ERROR, "Failed to send nw req " 
                "Incorrect scan type");
        LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
        /*SPR 17777 +-*/
    }

    SON_UT_TRACE_EXIT();
    /* Release the buffer memory as it is no longer needed. */
    return RELEASE_SON_BUFFER;
} /* sonapps_fsm_nw_scan_req */

/*****************************************************************************
 * Function Name  : sonapps_fsm_periodic_nw_scan_req
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONAPPS_PERIODIC_NW_SCAN_REQ 
 *                  message received from SONMgmtIfH in 
 *                  SONAPP_STATE_ACTIVE_state.
 ****************************************************************************/
static son_buf_retain_status_et
sonapps_fsm_periodic_nw_scan_req
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
    sonapps_periodic_nw_scan_req_t *p_sonapps_periodic_nw_scan_req 
                                   = (sonapps_periodic_nw_scan_req_t *)p_msg;


    SON_UT_TRACE_ENTER();
        
    /* Send SONNMM_PERIODIC_SCAN_REQ to Nmm */
    son_create_send_buffer((son_u8 *)p_sonapps_periodic_nw_scan_req, 
                           SON_APPS_MODULE_ID,
                           SON_NMM_MODULE_ID, 
                           SONNMM_PERIODIC_SCAN_REQ,
                           sizeof(sonapps_periodic_nw_scan_req_t));
    /*SPR 17777 +-*/
     SON_LOG(p_sonapps_gb_context->apps_log_enable, p_son_apps_facility_name, SON_BRIEF,
             "SONNMM_PERIODIC_SCAN_REQ message sent to "
             "Nmm with transaction Id = %u", p_sonapps_periodic_nw_scan_req->transaction_id);
     LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
    /*SPR 17777 +-*/

    SON_UT_TRACE_EXIT();
    /* Release the buffer memory as it is no longer needed. */
    return RELEASE_SON_BUFFER;
} /* sonapps_fsm_periodic_nw_scan_req */

/*****************************************************************************
 * Function Name  : son_free_cell_info_list
 * Inputs         : p_cell_info_list - Pointer to the cell info list 
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to clean up the memory used by
 *                  cell info list (created by SONApps) in case of  internal 
 *                  failure. 
 ****************************************************************************/
static son_void_t
son_free_cell_info_list
(
    cell_info_list_t        *p_cell_info_list
)
{
    SON_UT_TRACE_ENTER();
    if (SON_PNULL != p_cell_info_list)
    {
        son_u32 count = 0;
        son_u32 no_of_nodes = p_cell_info_list->count;
        cell_info_data_t *p_cell_info_data = SON_PNULL;

        /* Iterate through the cell info list */
        for (count = 0; count < no_of_nodes; count++)
        {
            /* Get data from the cell info list */
            CELL_INFO_LIST_DEQUEUE(p_cell_info_list, p_cell_info_data);
            if (SON_PNULL != p_cell_info_data)
            {
                /* Free the memory of the cell info list pointer */
                son_mem_free(p_cell_info_data);
                p_cell_info_data = SON_PNULL;
            }
        }

        /* Free the memory of the cell info list pointer */
        son_mem_free(p_cell_info_list);
    }

    SON_UT_TRACE_EXIT();
} /* son_free_cell_info_list */

/*****************************************************************************
 * Function Name  : son_free_earfcn_scan_res_list
 * Inputs         : p_earfcn_scan_res_list - Pointer to the earfcn scan res  
 *                  list
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to clean up the memory used by
 *                  earfcn scan res list (created by SONApps) in case of  
 *                  internal failure. 
 ****************************************************************************/
static son_void_t
son_free_earfcn_scan_res_list
(
    earfcn_scan_res_list_t *p_earfcn_scan_res_list
)
{
    SON_UT_TRACE_ENTER();
    if (SON_PNULL != p_earfcn_scan_res_list)
    {
        son_u32 count = 0;
        son_u32 no_of_nodes = p_earfcn_scan_res_list->count;
        earfcn_scan_res_data_t *p_earfcn_data = SON_PNULL;

        /* Iterate through the earfcn scan res list */
        for (count = 0; count < no_of_nodes; count++)
        {
            /* Get data from the earfcn scan res list */
            EARFCN_SCAN_LIST_DEQUEUE(p_earfcn_scan_res_list, 
                p_earfcn_data);
            if (SON_PNULL != p_earfcn_data)
            {
                /* Free the cell info list pointed by this node */
                son_free_cell_info_list(p_earfcn_data->earfcn_data.p_cell_info_list);
                p_earfcn_data->earfcn_data.p_cell_info_list = SON_PNULL;
            }

            /* Free the memory of the earfcn scan res list data pointer */
            son_mem_free(p_earfcn_data);
            p_earfcn_data = SON_PNULL;
        }

        /* Free the memory of the earfcn scan res list pointer */
        son_mem_free(p_earfcn_scan_res_list);
    }

    SON_UT_TRACE_EXIT();
} /* son_free_earfcn_scan_res_list */

/*****************************************************************************
 * Function Name  : sonapps_scan_resp_clean_up
 * Inputs         : p_earfcn_scan_res_list - Pointer to the earfcn scan res
 *                  list
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to clean up the memory used by
 *                  various nmm scan resp lists (created by SONApps/SONNmm)
 *                  in case of  internal failure. 
 ****************************************************************************/
static son_void_t
sonapps_scan_resp_clean_up
(
    earfcn_scan_res_list_t  *p_earfcn_scan_res_list
)
{
    SON_UT_TRACE_ENTER();
    
    /* Free the earfcn scan resp list */
    son_free_earfcn_scan_res_list(p_earfcn_scan_res_list);

    SON_UT_TRACE_EXIT();
}

/* LTE_SON_KLOCWORK_WARN_9JULY_2012_START */

/*****************************************************************************
 * Function Name  : son_apps_populate_eutran_cell_info
 * Inputs         : p_eutran_channel_num - Pointer to the eutran scan 
 *                                         response node data
 *                  p_earfcn_data - Pointer to earfcn data
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : p_earfcn_data - Pointer to the structure containing eutran 
 *                  cells data (It will populte EUTRAN cell infomation in this 
 *                  structure.)
 * Returns        : SON_TRUE - If some error occured 
 *                  SON_FALSE - If the function populted results without error.
 * Description    : This is the function for populating the eutran cell 
 *                  information in scan results.
 ****************************************************************************/
static son_bool_et
son_apps_populate_eutran_cell_info
(
   eutran_channel_num_t     *p_eutran_channel_num, 
   earfcn_data_t            *p_earfcn_data,
   son_apps_gb_context_t    *p_sonapps_gb_context
)
{
    son_u16 cell_index = 0;
    son_bool_et is_error = SON_FALSE;
    son_size_t temp_len = 0;
    cell_info_data_t *p_cell_info_data = SON_PNULL;
    son_float rsrq_in_db = 0.0;
    son_float meas_bw_in_db = 0.0;
    son_u16 dl_sys_bw_array[SON_MAX_RBS]={6,15,25,50,75,100};
    son_u16 final_index = 0;
    son_u16 index = 0;

    SON_UT_TRACE_ENTER();
    SON_LOG(p_sonapps_gb_context->apps_log_enable, 
            p_son_apps_facility_name, SON_DETAILED, 
            "num_of_cells %d", p_eutran_channel_num->num_of_cells);

    /* Initialize the cell info list */

    /* Loop to populate all the cell information for a 
     * particular earfcn value. */
    /* Inner for loop */
    for (cell_index = 0; 
            cell_index < p_eutran_channel_num->num_of_cells; 
            cell_index++)
    {
        /* Cell info list needs to be created only once if the
         * num of cells is atleast 1 */
        if (0 == cell_index)
        {
            p_earfcn_data->p_cell_info_list = (cell_info_list_t *)son_mem_get(
                    sizeof(cell_info_list_t));

            if (SON_PNULL == p_earfcn_data->p_cell_info_list)
            {
                SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                        p_son_apps_facility_name, SON_ERROR, 
                        "Memory allocation failed"
                        " for p_cell_info_list");
                is_error = SON_TRUE;
                break; /* Break from inner for loop */
            }

            /* Initialize the cell info list */
            list_init(p_earfcn_data->p_cell_info_list);
        }

        temp_len = sizeof(cell_info_data_t);
        /* Create the node for cell_info_list_t list */
        p_cell_info_data = (cell_info_data_t *)
            son_mem_get(temp_len);
        if (SON_PNULL == p_cell_info_data)
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_ERROR, 
                    "Memory allocation failed "
                    "for p_cell_info_data");
            is_error = SON_TRUE;
            break; /* Break from inner for loop */
        }

        SON_MEMSET(p_cell_info_data, 0, temp_len);
        SON_MEMCPY(p_cell_info_data->cell_info.cell_Id, 
                p_eutran_channel_num->cell[cell_index].cell_Id, 
                SON_CELL_ID_OCTET_SIZE);

        p_cell_info_data->cell_info.cell_barred = 
            p_eutran_channel_num->cell[cell_index].cell_barred;

        p_cell_info_data->cell_info.csg_indication = 
            p_eutran_channel_num->cell[cell_index].csg_indication;

        if (CELL_ACCESS_INFO_CSG_ID_PRES_FLAG & 
                p_eutran_channel_num->cell[cell_index].presence_bitmask)
        {
            p_cell_info_data->cell_info.bitmask = 
                SON_CSG_ID_PRESENT;
            SON_MEMCPY(p_cell_info_data->cell_info.csg_identity, 
                    p_eutran_channel_num->cell[cell_index].csg_identity, 
                    SON_CSG_ID_OCTET_SIZE);
        }

        p_cell_info_data->cell_info.num_plmn_id =
            p_eutran_channel_num->cell[cell_index].num_plmn_id;

        SON_MEMCPY(p_cell_info_data->cell_info.plmn_identity_info,
                p_eutran_channel_num->cell[cell_index].plmn_identity_info, 
                sizeof(p_cell_info_data->cell_info.plmn_identity_info));

        SON_MEMCPY(p_cell_info_data->cell_info.tac, 
                p_eutran_channel_num->cell[cell_index].tac, 
                SON_TAC_OCTET_SIZE);

        p_cell_info_data->cell_info.rsTxPower = 
            p_eutran_channel_num->cell[cell_index].reference_signal_power;

        p_cell_info_data->cell_info.dl_sys_bw =
            p_eutran_channel_num->cell[cell_index].dl_sys_bw;

        p_cell_info_data->cell_info.ul_sys_bw =
            p_eutran_channel_num->cell[cell_index].ul_sys_bw;

        p_cell_info_data->cell_info.pci_id =
            p_eutran_channel_num->cell[cell_index].pci_id;
        SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                p_son_apps_facility_name, SON_DETAILED, 
                "earfcn %d pci_id %d", 
                p_eutran_channel_num->earfcn, p_cell_info_data->cell_info.pci_id);



        if (INTRA_FREQ_CELL_RESELECT_MEAS_BW_PRES_FLAG &
                p_eutran_channel_num->cell[cell_index].presence_bitmask)
        {
            /* Covert the number of RBs/Meas BW into DB */
            meas_bw_in_db = convert_meas_bw_in_db(
                    p_eutran_channel_num->cell[cell_index].
                    allowed_meas_bandwidth);
        }
        else
        {
            /*SPR NO : 11004 changes start */
            for (index =0;index< SON_MAX_RBS;index ++)
            {
                if ( p_eutran_channel_num->cell[index].dl_sys_bw == dl_sys_bw_array[index])
                {
                    final_index =index;
                    break;
                }
            }
            /*SPR NO : 11004 changes end */

            /* Convert the index of RBs/Meas BW passed  into the corresponding  DB */
            meas_bw_in_db = convert_meas_bw_in_db(
                    final_index);
        }

        /* Find RSRQ using RSRQ = #ofRB in BW * (RSRP/RSSI).
         * The calculated value will be in db
         * Subract 144 from rsrp and rssi to get their db
         * equivalent */
        rsrq_in_db = meas_bw_in_db + 
            (p_eutran_channel_num->cell[cell_index].rsrp - 144) - 
            (p_eutran_channel_num->rssi - 144);

        /* Perform RSRQ measurement report mapping. 
         * Possible range of rsrq is 0 to 34. */
        CONVERT_RSRQ_DB_TO_REPORTED_VALUE(rsrq_in_db, 
                p_cell_info_data->cell_info.rsrq);

        CONVERT_RSRP_TO_3GPP_REPORTED_VALUE(
                p_eutran_channel_num->cell[cell_index].rsrp, 
                p_cell_info_data->cell_info.rsrp);

        /* Add a node in cell_info_list_t list */
        SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                p_son_apps_facility_name, SON_INFO, 
                "CELL_INFO_LIST_ENQUEUE : earfcn %d pci_id %d", 
                p_eutran_channel_num->earfcn, p_cell_info_data->cell_info.pci_id);

        CELL_INFO_LIST_ENQUEUE(p_cell_info_data,
                p_earfcn_data->p_cell_info_list);
    } /* End of Inner for loop */

    SON_UT_TRACE_EXIT();
    return is_error;
} /* son_apps_populate_eutran_cell_info */

/*****************************************************************************
 * Function Name  : son_apps_populate_eutran_scan_results
 * Inputs         : p_eutran_scan_res_list - Pointer to the eutran scan 
 *                                           response results 
 *                  no_of_nodes - Number of nodes in the scan response list 
 *                                received from SONNmm
 *                  p_sonapps_gb_context - Pointer to global context structure
 *                  p_apps_scan_res - Pointer to network scan response
 *                  structure
 * Outputs        : p_apps_scan_res - Pointer to scan response result structure
 *                  (It will populate the eutran scan results in this sturcture)
 * Returns        : SON_TRUE - If some error occured 
 *                  SON_FALSE - If the function populted results without error.
 * Description    : This is the function for populating the eutran scan results.
 ****************************************************************************/
static son_bool_et
son_apps_populate_eutran_scan_results
(
    nmm_scan_res_list_t     *p_eutran_scan_res_list,
    son_u32                 no_of_nodes,
    son_apps_gb_context_t   *p_sonapps_gb_context,
    sonapps_nw_scan_res_t   *p_apps_scan_res
)
{
    son_u32 count = 0;
    start_scan_res_eutran_data_t *p_start_scan_res_data = SON_PNULL;
    son_bool_et is_error = SON_FALSE; 
    son_size_t temp_len = 0;
    earfcn_scan_res_data_t *p_earfcn_data = SON_PNULL;


    SON_UT_TRACE_ENTER();

    /* Iterate through the etran scan response list */
    for (count = 0; count < no_of_nodes; count++) 
    {
        if (0 == count)
        {
            /* Get the memory for earfcn scan response list to be 
             * sent to SMIF. This needs to be done only if
             * atleast 1 node is present in nmm res list */
            p_apps_scan_res->p_earfcn_list = (earfcn_scan_res_list_t *)
                son_mem_get(sizeof(earfcn_scan_res_list_t));

            if (SON_PNULL == p_apps_scan_res->p_earfcn_list)
            {
                SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                        p_son_apps_facility_name, SON_ERROR, 
                        "Memory allocation failed "
                        " for p_earfcn_list");
                is_error = SON_TRUE;
                break; /* Break from the outer for loop */
            }

            /* Initialize the earfcn scan response list */
            list_init(p_apps_scan_res->p_earfcn_list);
        }

        /* Get data from the nmm start scan resp list */
        SCAN_RESP_LIST_DEQUEUE(start_scan_res_eutran_data_t, p_eutran_scan_res_list, 
                p_start_scan_res_data);

        /* Checking p_start_scan_res_data pointer to remove 
         * warning though this pointer will never be null*/
        if (SON_PNULL == p_start_scan_res_data)
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_ERROR, 
                    "Failed to read data from NMM scan response list");
            is_error = SON_TRUE;
            break; /* Break from outer for loop */ 
        }

        /* Enqueue the dequed data as it is needed by NMM */
        SCAN_RESP_LIST_ENQUEUE(p_eutran_scan_res_list, 
                p_start_scan_res_data);

/*+ SPR 18396*/
        if(0 != (p_start_scan_res_data->scan_res_data).num_of_cells)
        {

        temp_len = sizeof(earfcn_scan_res_data_t);
        /* Create the node for earfcn_scan_res_list_t list */
        p_earfcn_data = (earfcn_scan_res_data_t *)son_mem_get(
                temp_len);
        if (SON_PNULL == p_earfcn_data)
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_ERROR, 
                    "Memory allocation failed for p_earfcn_data");
            is_error = SON_TRUE;
            break; /* Break from outer for loop */
        }

        SON_MEMSET(p_earfcn_data, 0, temp_len);
        p_earfcn_data->earfcn_data.earfcn = p_start_scan_res_data->
            scan_res_data.earfcn;
        CONVERT_RSSI_TO_3GPP_REPORTED_VALUE(p_start_scan_res_data->
                scan_res_data.rssi, p_earfcn_data->earfcn_data.rssi);
 
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_INFO, 
                    "earfcn %d scan_res_data.rssi %d  p_earfcn_data->earfcn_data.rssi %d",
                    p_earfcn_data->earfcn_data.earfcn, p_start_scan_res_data->scan_res_data.rssi,
                    p_earfcn_data->earfcn_data.rssi);

        is_error = son_apps_populate_eutran_cell_info(
                &p_start_scan_res_data->scan_res_data,
                &p_earfcn_data->earfcn_data,
                p_sonapps_gb_context);
        
        /*SPR 15935 Fix Start */
        /*Lines Deleted*/
        /*SPR 15935 Fix Stop */

        /* Add a node in the earfcn_scan_res_list_t */ 
        EARFCN_SCAN_LIST_ENQUEUE(p_earfcn_data, 
                p_apps_scan_res->p_earfcn_list);
        }
        else
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable,
                    p_son_apps_facility_name, SON_WARNING, 
                    "earfcn [%d] not added in the scan list"
                    "as number of cell is [%d]",p_start_scan_res_data->scan_res_data.earfcn,
                    p_start_scan_res_data->scan_res_data.num_of_cells);
        }
/*+ SPR 18396*/

        /*SPR 15935 Fix Start */
        if (SON_TRUE == is_error)
        {
            break; /* Break from outer for loop */
        }
        /*SPR 15935 Fix Stop */
    } /* End of outer for loop */

    SON_UT_TRACE_EXIT();
    return is_error;
} /* son_apps_populate_eutran_scan_results */

/*****************************************************************************
 * Function Name  : sonapps_fsm_start_scan_res
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the handler function for SONNMM_START_SCAN_RES
 *                  message received from NMM Controller in 
 *                  SONAPP_STATE_ACTIVE_state.
 ****************************************************************************/
static son_buf_retain_status_et
sonapps_fsm_start_scan_res
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
    sonapps_nw_scan_res_t apps_scan_res = {0};
    son_return_et result = SON_FAILURE;
    son_nmm_start_scan_res_t *p_nmm_start_scan_res = 
        (son_nmm_start_scan_res_t *)p_msg;
    nmm_scan_res_list_t *p_eutran_scan_res_list = SON_PNULL;
    son_bool_et is_error = SON_FALSE;


    SON_UT_TRACE_ENTER();

    /* Check if NMM scan response is for carrier frequency selection
     *  request from OAM */
    if ((SON_MIF_MODULE_ID == p_nmm_start_scan_res->originator_module_id) &&
        (SON_TRUE == p_sonapps_gb_context->
        acf_selection_progress_status[ACF_SELECTION_REQ]) &&
        (p_sonapps_gb_context->transaction_id[SONAPPS_ACF_OAM] ==
        p_nmm_start_scan_res->transaction_id))
    {
        /* Function call to handle network scan response for carrier
         *  frequency selection request message */
        sonapps_fsm_acf_selection_res_for_oam(p_nmm_start_scan_res,
            p_sonapps_gb_context);
    }

    /* Check if NMM scan response is for carrier frequency selection
     *  and DL power parameters request */
    else if ((SON_APPS_MODULE_ID == p_nmm_start_scan_res->
        originator_module_id) &&
        (SON_TRUE == p_sonapps_gb_context->
        acf_selection_progress_status[ACF_AND_POWER_REQ]) &&
        (p_sonapps_gb_context->transaction_id[SONAPPS_ACF_APPS] ==
        p_nmm_start_scan_res->transaction_id))
    {
        sonapps_fsm_acf_selection_res_for_apps(p_nmm_start_scan_res,
            p_sonapps_gb_context);
    }
    /* Check if NMM scan response is for PCI Selection/Reselection */
    else if (p_sonapps_gb_context->transaction_id[SONAPPS_PCI] ==
                  p_nmm_start_scan_res->transaction_id)
    {
        sonapps_pci_scan_res_handler(p_nmm_start_scan_res,
            p_sonapps_gb_context);
    }
    else
    {
        /*do-while loop used to ensure handling of failure at one common place*/
        do 
        {
            /* Populate the transaction Id, result and error code */
            apps_scan_res.transaction_id = p_nmm_start_scan_res->transaction_id;
            apps_scan_res.result = p_nmm_start_scan_res->start_scan_res.
                son_resp;
            apps_scan_res.error_code = p_nmm_start_scan_res->start_scan_res.
                son_error;
         
            result = p_nmm_start_scan_res->start_scan_res.son_resp; 
            /* Get the pointer to the nmm start scan resp list */
            p_eutran_scan_res_list = p_nmm_start_scan_res->start_scan_res.
                p_eutran_scan_res_list;
            if ((SON_SUCCESS == result || SON_PARTIAL_SUCCESS == result) 
                && SON_PNULL != p_eutran_scan_res_list)
            {
                son_u32 no_of_nodes = p_eutran_scan_res_list->count;
                SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_DETAILED, 
                    "Number of nodes in the scan resp "
                    "list received from SONNmm = %u", no_of_nodes);

                is_error = son_apps_populate_eutran_scan_results(
                        p_eutran_scan_res_list,
                        no_of_nodes,
                        p_sonapps_gb_context,
                        &apps_scan_res);

                if (SON_TRUE == is_error)
                {
                    break; /* Break from do-while loop */
                }
            }

            /* Send SONAPPS_NW_SCAN_RES message to MgmtIfH module */
            son_create_send_buffer((son_u8 *)&apps_scan_res, SON_APPS_MODULE_ID,
                SON_MIF_MODULE_ID, SONAPPS_NW_SCAN_RES,
                sizeof(sonapps_nw_scan_res_t));
        } while (0); /* End of do-while loop */
        if (SON_TRUE == is_error)
        {
            /* Free all the memory taken by scan response in case of error */
            SON_LOG(p_sonapps_gb_context->apps_log_enable,
                p_son_apps_facility_name, SON_DETAILED,
                "Memory is freeed for NMM "
                "scan response list");
            LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
                /*SPR 17777 +-*/
            sonapps_scan_resp_clean_up(apps_scan_res.p_earfcn_list);
        }
    }
      
    SON_UT_TRACE_EXIT();
    /* Release the buffer memory as it is no longer needed. */
    return RELEASE_SON_BUFFER;
} /* sonapps_fsm_start_scan_res */

/* LTE_SON_KLOCWORK_WARN_9JULY_2012_END */

/*****************************************************************************
 * Function Name  : sonapps_fsm_invalid_scan_res_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This function is used to handle the
 *                  SONNMM_EVENT_START_SCAN_RES message received
 *                  from SONNmm in SONAPP_STATE_INIT state. It logs the 
 *                  reception of message in invalid state. It also does the 
 *                  clean up of the message memory taken by the SONNmm.
 ****************************************************************************/
static son_buf_retain_status_et
sonapps_fsm_invalid_scan_res_handler
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
    SON_UT_TRACE_ENTER();

    /* Free the scan res list received from Nmm */
    SON_LOG(p_sonapps_gb_context->apps_log_enable, p_son_apps_facility_name,
        SON_DETAILED, "Memory used "
        "by scan resp list received fron SONNmm has been freed.");

    SON_LOG(p_sonapps_gb_context->apps_log_enable, p_son_apps_facility_name, 
        SON_BRIEF, 
        "SONNMM_EVENT_START_SCAN_RES message received by SONApps in "
        "invalid state = [%u]. Memory taken "
        /*SPR 17777 +-*/
        "by scan resp list received from SONNmm is freed", 
        p_sonapps_gb_context->g_sonapps_state);
    LTE_GCC_UNUSED_PARAM(p_msg);
    LTE_GCC_UNUSED_PARAM(p_cspl_hdr);
        /*SPR 17777 +-*/
    
    SON_UT_TRACE_EXIT();
    /* Release the buffer memory as it is no longer needed. */
    return RELEASE_SON_BUFFER;
} /* sonapps_fsm_invalid_scan_res_handler */

/*****************************************************************************
 * Function Name  : sonapps_fsm_invalid_log_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This function is used to handle 
 *                  SMIF_EVENT_LOG_ENABLE_CMD/SMIF_EVENT_SET_LOG_LEVEL_CMD/
 *                  SMIF_EVENT_GET_LOG_LEVEL_REQ message received from 
 *                  SONMgmtIfH in SONAPP_STATE_INIT state. It simply logs 
 *                  the reception of message in invalid state and returns 
 *                  after releasing the message buffer. 
 ****************************************************************************/
static son_buf_retain_status_et
sonapps_fsm_invalid_log_handler
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
    SON_UT_TRACE_ENTER();

    SON_LOG(p_sonapps_gb_context->apps_log_enable, p_son_apps_facility_name, 
        SON_INFO,
        "Message with api id = [%u] received "
        /*SPR 17777 +-*/
        "by SONApps in invalid state = [%u] msg %u", p_cspl_hdr->api, 
        p_sonapps_gb_context->g_sonapps_state,*p_msg);
        /*SPR 17777 +-*/

    SON_UT_TRACE_EXIT();
    /* Release the buffer memory as it is no longer needed. */
    return RELEASE_SON_BUFFER;
}

/* SPR-13251 Fix Starts */
/*****************************************************************************
 * Function Name  : sonapps_fsm_get_log_level_req
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This function is used to handle SMIF_GET_LOG_LEVEL_REQ
 *                  message received from SONMgmtIfH in SONAPP_STATE_ACTIVE 
 *                  state.
 ****************************************************************************/
static son_buf_retain_status_et
sonapps_fsm_get_log_level_req
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
    (void)p_cspl_hdr;/*SPR 17777 +-*/
    smif_get_log_level_req_t *p_req = (smif_get_log_level_req_t*)p_msg;
    smif_get_log_level_res_t res;
    SON_UT_TRACE_ENTER();

    SON_MEMSET(&res, 0, sizeof(res));
    res.transaction_id = p_req->transaction_id;
    res.log_level = p_sonapps_gb_context->log_level;
    
    /* Send cell delete resp to mif */
    son_create_send_buffer((son_u8 *)&res,
            SON_APPS_MODULE_ID,
            SON_MIF_MODULE_ID,
            SMIF_GET_LOG_LEVEL_RESP,
            sizeof(res));

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
}
/* SPR-13251 Fix Ends */

/*************************APPS State Machine ********************************/
typedef son_buf_retain_status_et (*son_apps_fsm_evt_handler_t)
(
    son_u8 *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
);

/***********************************************************************
 * FSM Entry Point Table
 * The first index of array is 1 less than the number of states since in
 * SHUTDOWN state no action will be taken.
 **********************************************************************/
static son_apps_fsm_evt_handler_t 
    son_apps_fsm_table[SONAPPS_NUM_OF_STATES - 1][SONAPPS_NUM_OF_EVENTS] =
{
    /* State INIT */
    {
        sonapps_pci_fsm_enable_disable_req_nack,
        /*SONAPPS_EVENT_PCI_ENABLE_DISABLE_REQ */
        sonapps_pci_fsm_invalid_event_handler,
        /*SONAPPS_EVENT_PCI_REG_DEREG_RES */
        sonapps_pci_fsm_selection_req_nack,
        /*SONAPPS_EVENT_PCI_SELECTION_REQ */
        sonapps_pci_fsm_reselection_req_nack,
        /*SONAPPS_EVENT_PCI_RESELECTION_REQ */
        sonapps_pci_fsm_invalid_event_handler,
        /*SONANR_EVENT_PCI_CONFLICT_IND */
        sonapps_pci_fsm_invalid_event_handler,
        /*SONANR_EVENT_PCI_IN_USE_LIST_RES */ 
        
        sonapps_fsm_init_config_req,
        /* SMIF_EVENT_INIT_CONFIG_REQ */
        sonapps_fsm_invalid_log_handler,        
        /* SMIF_EVENT_LOG_ENABLE_CMD */
        sonapps_fsm_invalid_log_handler,
        /*SMIF_EVENT_SET_LOG_LEVEL_CMD*/
        sonapps_fsm_invalid_scan_req_handler,
        /* SONAPPS_EVENT_NW_SCAN_REQ */
        sonapps_fsm_invalid_scan_req_handler,
        /* SONAPPS_EVENT_PERIODIC_NW_SCAN_REQ */
        sonapps_fsm_invalid_scan_res_handler,
        /* SONNMM_EVENT_START_SCAN_RES*/
        sonapps_fsm_shutdown_req,  
        /* SMIF_EVENT_SHUTDOWN_REQ */
        sonapps_fsm_acf_invalid_state_selection_req_handler,
        /* SONAPPS_EVENT_CARRIER_FREQ_SELECTION_REQ */
        /* SPR-13251 Fix Starts */
        sonapps_fsm_acf_invalid_state_selection_and_power_req_handler,
        /* SONAPPS_EVENT_CARRIER_FREQ_DL_PARAMS_REQ*/
        sonapps_fsm_invalid_log_handler,
        /* SMIF_EVENT_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    },
    /* State ACTIVE */
    {
        sonapps_pci_fsm_event_handler,
        /*SONAPPS_EVENT_PCI_ENABLE_DISABLE_REQ */
        sonapps_pci_fsm_event_handler,
        /*SONAPPS_EVENT_PCI_REG_DEREG_RES */
        sonapps_pci_fsm_event_handler,
        /*SONAPPS_EVENT_PCI_SELECTION_REQ */
        sonapps_pci_fsm_event_handler,
        /*SONAPPS_EVENT_PCI_RESELECTION_REQ */
        sonapps_pci_fsm_event_handler,
        /*SONANR_EVENT_PCI_CONFLICT_IND */
        sonapps_pci_fsm_event_handler,
        /*SONANR_EVENT_PCI_IN_USE_LIST_RES */           
        sonapps_fsm_active_state_nack_handler,
        /* SMIF_EVENT_INIT_CONFIG_REQ */
        sonapps_fsm_log_enable_cmd,             
        /* SMIF_EVENT_LOG_ENABLE_CMD*/
        sonapps_fsm_set_log_level_cmd,          
        /*SMIF_EVENT_SET_LOG_LEVEL_CMD*/
        sonapps_fsm_nw_scan_req,                
        /* SONAPPS_EVENT_NW_SCAN_REQ */
        sonapps_fsm_periodic_nw_scan_req,
        /* SONAPPS_EVENT_PERIODIC_NW_SCAN_REQ */
        sonapps_fsm_start_scan_res,             
        /* SONNMM_EVENT_START_SCAN_RES*/
        sonapps_fsm_shutdown_req,                
        /* SMIF_EVENT_SHUTDOWN_REQ  */
        sonapps_fsm_acf_selection_req,    
        /* SONAPPS_EVENT_CARRIER_FREQ_SELECTION_REQ */
        /* SPR-13251 Fix Starts */
        sonapps_fsm_acf_selection_req,
        /* SONAPPS_EVENT_CARRIER_FREQ_DL_PARAMS_REQ*/
        sonapps_fsm_get_log_level_req
        /* SMIF_EVENT_GET_LOG_LEVEL_REQ */
        /* SPR-13251 Fix Ends */
    }
};

/* LTE_SON_KLOCWORK_WARN_24JULY_2012_START */

/*****************************************************************************
 * Function Name  : get_msg_idx_from_mif_msgs
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : Index in SONApps FSM               
 * Description    : This function computes the index in the Apps FSM for the 
 *                  messages from MIF
 ****************************************************************************/
static son_apps_index_et
get_msg_idx_from_mif_msgs
(
    son_u16                 from,
    son_u16                 api,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
    son_apps_index_et ret = APPS_IDX_NOT_AVAILABLE;


    switch (api)
    {
        case SMIF_INIT_CONFIG_REQ: 
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_INFO, 
                    "MIF->SONAPPS:SMIF_INIT_CONFIG_REQ");
            ret = APPS_IDX_SMIF_EVENT_INIT_CONFIG_REQ;
            break;
        }

        case SMIF_LOG_ENABLE_DISABLE_CMD:
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_INFO, 
                    "MIF->SONAPPS:SMIF_LOG_ENABLE_DISABLE_CMD");
            ret = APPS_IDX_SMIF_EVENT_LOG_ENABLE_CMD;
            break;
        }

        case SMIF_SET_LOG_LEVEL_CMD:
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_INFO, 
                    "MIF->SONAPPS:SMIF_SET_LOG_LEVEL_CMD");
            ret = APPS_IDX_SMIF_EVENT_SET_LOG_LEVEL_CMD;
            break;
        }

        case SONAPPS_NW_SCAN_REQ:
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_INFO, 
                    "MIF->SONAPPS:SONAPPS_NW_SCAN_REQ");
            ret = APPS_IDX_SONAPPS_EVENT_NW_SCAN_REQ;
            break;
        }

        case SONAPPS_PERIODIC_NW_SCAN_REQ:
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_INFO, 
                    "MIF->SONAPPS:SONAPPS_PERIODIC_NW_SCAN_REQ");
            ret = APPS_IDX_SONAPPS_EVENT_PERIODIC_NW_SCAN_REQ;
            break;
        }

        case SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_REQ:
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_INFO, 
                    "MIF->SONAPPS:SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_REQ");
            ret = APPS_IDX_SMIF_EVENT_PCI_ENABLE_DISABLE_REQ;
            break;
        }

        case SONAPPS_PCI_SELECTION_REQ:
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_INFO, 
                    "MIF->SONAPPS:SONAPPS_PCI_SELECTION_REQ");
            ret = APPS_IDX_SMIF_EVENT_PCI_SELECTION_REQ;
            break;
        }

        case SONAPPS_PCI_RESELECTION_REQ:
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_INFO, 
                    "MIF->SONAPPS:SONAPPS_PCI_RESELECTION_REQ");
            ret = APPS_IDX_SMIF_EVENT_PCI_RESELECTION_REQ;
            break;
        }

        case SMIF_SHUTDOWN_REQ:
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_INFO, 
                    "MIF->SONAPPS:SMIF_SHUTDOWN_REQ");
            ret = APPS_IDX_SMIF_EVENT_SHUTDOWN_REQ;
            break;
        }

        case SONAPPS_CARRIER_FREQ_SELECTION_REQ:
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_INFO, 
                    "MIF->SONAPPS:SONAPPS_CARRIER_FREQ_SELECTION_REQ");
            ret = APPS_IDX_SONAPPS_EVENT_CARRIER_FREQ_SELECTION_REQ;
            break;
        }

        case SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_REQ:
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_INFO, 
                    "MIF->SONAPPS:SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_REQ");
            ret = APPS_IDX_SONAPPS_EVENT_CARRIER_FREQ_DL_PARAMS_REQ;
            break;
        }

        /* SPR-13251 Fix Starts */
        case SMIF_GET_LOG_LEVEL_REQ:
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_INFO, 
                    "MIF->SONAPPS:SMIF_GET_LOG_LEVEL_REQ");
            ret = APPS_IDX_SONAPPS_EVENT_GET_LOG_LEVEL_REQ;
            break;
        }
        /* SPR-13251 Fix Ends */

        default:
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_WARNING, 
                    "Invalid API = [%u] received from [%d]", api, from);
            break;
    }
    } /* switch */

    return ret;
} /* get_msg_idx_from_mif_msgs */

/*****************************************************************************
 * Function Name  : get_msg_idx_from_anr_msgs
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : Index in SONApps FSM               
 * Description    : This function computes the index in the Apps FSM for the 
 *                  ANR messages
 ****************************************************************************/
static son_apps_index_et
get_msg_idx_from_anr_msgs
(
    son_u16                 from,
    son_u16                 api,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
   son_apps_index_et ret = APPS_IDX_NOT_AVAILABLE;


    switch (api)
    {
        case SONANR_PCI_REG_DEREG_RES:
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_INFO, 
                    "ANR->SONAPPS:SONANR_PCI_REG_DEREG_RES");
            ret = APPS_IDX_SONANR_EVENT_PCI_REG_DEREG_RES;
            break;
        }

        case SONANR_PCI_IN_USE_LIST_RES:
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_INFO, 
                    "ANR->SONAPPS:SONANR_PCI_IN_USE_LIST_RES");
            ret = APPS_IDX_SONANR_EVENT_PCI_IN_USE_LIST_RES;
            break;
        }

        case SONANR_PCI_CONFLICT_CONFUSION_IND:
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_INFO, 
                    "ANR->SONAPPS:SONANR_PCI_CONFLICT_CONFUSION_IND");
            ret = APPS_IDX_SONANR_EVENT_PCI_CONFLICT_CONFUSION_IND;
            break;
        }

        default:
        {
            SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_WARNING, 
                    "Invalid API = [%u] received from [%d]", api, from);
            break;
    }
    } /* switch */
   
   return ret;
} /* get_msg_idx_from_anr_msgs */

/*****************************************************************************
 * Function Name  : calculate_apps_msg_index
 * Inputs         : from - Source module id
 *                  api  - Procedure code of the message
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : Index in SONApps FSM               
 * Description    : This function computes the index in the Apps FSM for the 
 *                  given procedure code
 ****************************************************************************/
son_u8  
calculate_apps_msg_index
(
    son_u16                 from,
    son_u16                 api,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
    son_apps_index_et ret = APPS_IDX_NOT_AVAILABLE;


    SON_UT_TRACE_ENTER();
    
    if (SON_MIF_MODULE_ID == from)
    {
        ret = get_msg_idx_from_mif_msgs(from, api, p_sonapps_gb_context );
    }
    else if (SON_ANR_MODULE_ID == from)
    {
        ret = get_msg_idx_from_anr_msgs(from, api, p_sonapps_gb_context );
    }
    else if (SON_NMM_MODULE_ID == from)
    {
        switch (api)
        {
            case SONNMM_START_SCAN_RES:
            {
                SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_INFO, 
                    "NMM->SONAPPS:SONNMM_START_SCAN_RES");
                ret = APPS_IDX_SONNMM_EVENT_START_SCAN_RES;
                break;
            }

            default:
            {
                SON_LOG(p_sonapps_gb_context->apps_log_enable, 
                    p_son_apps_facility_name, SON_WARNING, 
                    "Invalid API = [%u] received from [%d]", api, from);
                break;
        }
    }
    }
    else
    {
        SON_LOG(p_sonapps_gb_context->apps_log_enable,
            p_son_apps_facility_name, SON_WARNING,
            "Message with API Id = [%u] received "
            "from unknown entity : [%u]", api, from);
    }

    SON_UT_TRACE_EXIT();
    return (son_u8)ret;     
} /* calculate_apps_msg_index */

/* LTE_SON_KLOCWORK_WARN_24JULY_2012_STOP */


/*****************************************************************************
 * Function Name  : son_apps_process_event
 * Inputs         : p_msg - Pointer to the message received
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This function is used to invoke the appropriate event 
 *                  handler for the messages received by SONApps. It 
 *                  calculates the API Id and current FSM state and based
 *                  on this access the son_apps_fsm_table to call the 
 *                  appropriate handler.
 ****************************************************************************/
son_buf_retain_status_et
son_apps_process_event
(
    son_void_t *p_msg,
    son_apps_gb_context_t *p_sonapps_gb_context
)
{
    son_buf_retain_status_et buf_retain_status = RELEASE_SON_BUFFER;
    son_u8 idx = APPS_IDX_NOT_AVAILABLE;
    STACKAPIHDR cspl_hdr = {0};




    SON_UT_TRACE_ENTER();

    /* Get the procedure code(API Id) */
    son_parse_cspl_header(p_msg, &cspl_hdr);

    /* Call the appropriate message handler based on the API Id and
     * source module id */
    idx = calculate_apps_msg_index(cspl_hdr.from, cspl_hdr.api, 
            p_sonapps_gb_context);
    /* Validate the index of son_apps_fsm_table array*/
    if (SONAPPS_NUM_OF_EVENTS <= idx)
    {
        /* No need to print the log here for invalid API Id as it is 
         *  already printed in calculate_apps_msg_index function */
        /* Do nothing */
    }
    else if ((SONAPPS_NUM_OF_STATES - 1) <= 
        p_sonapps_gb_context->g_sonapps_state)
    {
        SON_LOG(p_sonapps_gb_context->apps_log_enable, 
            p_son_apps_facility_name, SON_WARNING,
            "Message with API Id = [%u] received in "
            "invalid state = [%u]", cspl_hdr.api, 
            p_sonapps_gb_context->g_sonapps_state);
    }
    else
    {
        SON_LOG(p_sonapps_gb_context->apps_log_enable, 
            p_son_apps_facility_name, SON_INFO,
            "### SONAPPS_FSM: Processing state %s Event %u ###", 
            SON_APPS_FSM_STATES_NAMES[p_sonapps_gb_context->g_sonapps_state],
            idx);

        /* Move the pointer such that it points to the message data after the 
         * CSPL header */
        p_msg += CV_HDRSIZE;
        
        buf_retain_status = (*son_apps_fsm_table[p_sonapps_gb_context->
            g_sonapps_state][idx])(p_msg, &cspl_hdr, p_sonapps_gb_context);

        SON_LOG(p_sonapps_gb_context->apps_log_enable, 
            p_son_apps_facility_name, SON_INFO,
            "### SONAPPS_FSM: Processing Completed ###");
    }

    SON_UT_TRACE_EXIT();
    return buf_retain_status;
} /* son_apps_process_event */
