/****************************************************************************
*
* ARICENT -
*
*
* Copyright (C) 2012 Aricent Inc. All Rights Reserved.
*
****************************************************************************
*
* SPR-612 Coverity Fix Starts
* $Id: son_apps_msg_handler.c $
* SPR-612 Coverity Fix Ends
*
****************************************************************************
*
* File Description: This file contains the definition manifest structure for
*                   SONApps module. It also defines the init and message
*                   handler functions for the SONApps module which are
*                   used for SONApps initialization and message handling
*                   respectively.
*
****************************************************************************
* Revision Details
* ----------------
*
* SPR-612 Coverity Fix Starts
* $Log: son_apps_msg_handler.c $
* SPR-612 Coverity Fix Ends
* Revision 0.1  2012/20/02 09:00:00  Shilpi Jain
* Initial code
*
****************************************************************************/

/****************************************************************************
* Project Includes
****************************************************************************/
#include <son_apps_fsm.h>
#include <son_apps_acf.h>

/****************************************************************************
* Global Variables
****************************************************************************/
const son_8 *p_son_apps_facility_name = "SON_APPS";
extern son_global_config_params_t g_config_params;

static const unsigned long son_apps_dst_list[] =
{
    SON_MIF_MODULE_ID,
    SON_NMM_MODULE_ID,
    SON_ANR_MODULE_ID
};

const QMANIFEST son_manifest_apps =
{
    "SON_APPS_MODULE_ID",
    SON_APPS_MODULE_ID,
    {0, son_apps_init},
    {son_apps_process_msg, 0},
    {0, 0, 0},
    son_apps_dst_list
};

/*****************************************************************************
 * Function Name  : son_apps_fill_icic_param_default_values
 * Inputs         : p_icic_config_param: Pointer to sonapps_icic_config_params_t
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function initializes the icic global context
 configuration parameters with default values.
 ******************************************************************************/

son_void_t
son_apps_fill_icic_param_default_values
(
  sonapps_icic_config_params_t *p_icic_config_param
 )
{
    SON_UT_TRACE_ENTER();
    p_icic_config_param->dl_tx_alpha = DL_TX_ALPHA;
    p_icic_config_param->dl_tx_beta = DL_TX_BETA;
    p_icic_config_param->ul_tx_alpha = UL_TX_ALPHA;
    p_icic_config_param->ul_offset_k = UL_OFFSET_K;
    SON_UT_TRACE_EXIT();
}



/*****************************************************************************
 * Function Name  : son_apps_init
 * Inputs         : p_son_apps_init_data - Pointer to SONApps stack
 *                  initialization data (Unused)
 * Outputs        : None
 * Returns        : Pointer to Apps specific context data (Unused)
 * Description    : This is the initialization function of SONApps module.
 *                  It sets the SONApps FSM state to init and send the INIT IND
 *                  to SONMgmtIfH.
 ****************************************************************************/
son_void_t *
son_apps_init
(
    /* Pointer to SONApps stack initialization data. Unused */
    son_void_t *p_son_apps_init_data
)
{
    son_apps_gb_context_t *p_sonapps_gb_context = SON_PNULL;

    SON_UT_TRACE_ENTER();

    /* Init advance logger with default log file */
    SON_INIT_LOG(SON_PNULL);

    /* Set the SONApps module log level to BRIEF till the time
     *  log level is not configured by OAM */
    SET_MODULE_LOG_LEVEL(SON_OAM_LOG_LEVEL_BRIEF);

    /* REL1.1 CHANGES : STARTS */
    p_sonapps_gb_context = son_mem_get(sizeof(son_apps_gb_context_t));

    if (SON_PNULL == p_sonapps_gb_context)
    {
        SON_LOG(SON_OAM_LOG_ON,
            p_son_apps_facility_name,
            SON_ERROR,
            /*SPR 17777 +-*/
            "Memory allocation failed for SONAPPS global context");
            LTE_GCC_UNUSED_PARAM(p_son_apps_init_data);
    }
    else
    {
        SON_MEMSET(p_sonapps_gb_context, 0, sizeof(son_apps_gb_context_t));
        son_apps_reset_context(p_sonapps_gb_context);

        /* REL1.1 CHANGES : ENDS */

        /* Set the SONApps FSM state to INIT */
        p_sonapps_gb_context->g_sonapps_state = SONAPP_STATE_INIT;

        SON_LOG(p_sonapps_gb_context->apps_log_enable, p_son_apps_facility_name,
            SON_INFO, "### SONAPPS_FSM: State change %s->%s ###", sonapps_fsm_name,
            SON_APPS_FSM_STATES_NAMES[SONAPP_STATE_INIT]);
       
        son_apps_fill_icic_param_default_values(&p_sonapps_gb_context->icic_params);
              
        /* Copy ICIC params read from config file */
        SON_MEMCPY(&p_sonapps_gb_context->icic_params, 
                     &g_config_params.icic_config_param,      
                     sizeof (sonapps_icic_config_params_t)); 

        /* Send SMIF_INIT_IND to SONMgmtIfh */
        son_create_send_buffer(SON_PNULL, SON_APPS_MODULE_ID,
            SON_MIF_MODULE_ID, SMIF_INIT_IND, 0);
    }

    SON_UT_TRACE_EXIT();

    /* Returning pointer to the global context of SONApps */
    return p_sonapps_gb_context;
} /* son_apps_init */

/*****************************************************************************
 * Function Name  : son_apps_process_msg
 * Inputs         : p_msg - The message received
 *                  p_cxt - Pointer to the SONApps global context data (Unused)
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This is the main entry point of SONApps module - CSPL
 *                  callback. It receives all the external APIs received
 *                  from external entities and invokes appropriate
 *                  handlers based on the API ID and current state.
 ****************************************************************************/
S32
son_apps_process_msg
(
    son_void_t *p_msg,
    son_void_t *p_cxt
)
{
    son_apps_gb_context_t *p_sonapps_gb_context = SON_PNULL;
    son_buf_retain_status_et buf_retain_status = RELEASE_SON_BUFFER;

    p_sonapps_gb_context = (son_apps_gb_context_t *)p_cxt;
    SON_UT_TRACE_ENTER();

    /* Precondition check */
    if (SON_PNULL == p_sonapps_gb_context)
    {
        SON_LOG(SON_OAM_LOG_ON, p_son_apps_facility_name, SON_ERROR,
            "SONAPPS global context is NULL!");
    }
    else if (SON_PNULL == p_msg)
    {
        SON_LOG(p_sonapps_gb_context->apps_log_enable,
            p_son_apps_facility_name, SON_ERROR,
            "API pointer received is NULL");
    }
    else if (SONAPP_STATE_SHUTDOWN == p_sonapps_gb_context->g_sonapps_state)
    {
        STACKAPIHDR cspl_hdr = {0};

        /* Get the procedure code(API Id) */
        son_parse_cspl_header(p_msg, &cspl_hdr);

        SON_LOG(p_sonapps_gb_context->apps_log_enable,
            p_son_apps_facility_name, SON_WARNING,
            "Message with Id = %u ignored in "
            "SONAPP_STATE_SHUTDOWN state.", cspl_hdr.api);

        /* Clean up the memory taken by the received messages */
        if (SONAPPS_NW_SCAN_REQ == cspl_hdr.api)
        {
            /* Free the memory of scan req data pointer received from SMIF */
            if (SON_NULL != ((sonapps_nw_scan_req_t *)(p_msg + CV_HDRSIZE))->eutran_scan_req.p_start_scan_req)
            {
                son_mem_free(((sonapps_nw_scan_req_t *)(p_msg + CV_HDRSIZE))->eutran_scan_req.p_start_scan_req);
                ((sonapps_nw_scan_req_t *)(p_msg + CV_HDRSIZE))->eutran_scan_req.p_start_scan_req = SON_PNULL;
            }
            else
            {
                son_mem_free(((sonapps_nw_scan_req_t *)(p_msg + CV_HDRSIZE))->utran_scan_req.p_start_scan_req);
                ((sonapps_nw_scan_req_t *)(p_msg + CV_HDRSIZE))->utran_scan_req.p_start_scan_req = SON_PNULL;
            }

            SON_LOG(p_sonapps_gb_context->apps_log_enable,
                p_son_apps_facility_name, SON_INFO,
                "Clean up done for SONAPPS_NW_SCAN_REQ "
                "message with Id = %u received in SONAPP_STATE_SHUTDOWN state",
                cspl_hdr.api);
        }
        /* Clean up the memory taken by the received messages */
        else if (SONAPPS_PERIODIC_NW_SCAN_REQ == cspl_hdr.api)
        {
            /* Free the memory of scan req data pointer received from SMIF */
            son_mem_free(((sonapps_periodic_nw_scan_req_t *)(p_msg + CV_HDRSIZE))->
                p_start_scan_req);
            ((sonapps_periodic_nw_scan_req_t *)(p_msg + CV_HDRSIZE))->
            p_start_scan_req = SON_PNULL;
            SON_LOG(p_sonapps_gb_context->apps_log_enable, p_son_apps_facility_name, SON_INFO,
                "Clean up done for SONAPPS_PERIODIC_NW_SCAN_REQ "
                "message with Id = %u received in SONAPP_STATE_SHUTDOWN state.",
                cspl_hdr.api);
        }
        else
        {
            /* No clean up is required for other messages
             *  so do nothing*/
        }
    }
    else
    {
        buf_retain_status = son_apps_process_event(p_msg, p_sonapps_gb_context);
    }

    SON_UT_TRACE_EXIT();
    return buf_retain_status;
} /* son_apps_process_msg */
