/****************************************************************************
*
* ARICENT -
*
* Copyright (C) 2012 Aricent Inc. All Rights Reserved.
*
****************************************************************************
*
* SPR-612 Coverity Fix Starts
* $Id: son_apps_acf.c $
* SPR-612 Coverity Fix Ends
*
****************************************************************************
*
* File Description: This file contains definitions of various event handlers
*                   for adaptive carrier frequency.
*
****************************************************************************
* Revision Details
* ----------------
*
* SPR-612 Coverity Fix Starts
* $Log: son_apps_acf.c $
* SPR-612 Coverity Fix Ends
* Revision 0.1  2012/15/05 14:50:00  Sonal Jain
*
****************************************************************************/

/****************************************************************************
* Project Includes
****************************************************************************/
#include <son_apps_acf.h>

/****************************************************************************
* Global Variables
****************************************************************************/
extern const son_8 *p_son_apps_facility_name;

/*****************************************************************************
 * Function Name  : send_acf_nack_to_oam_for_acf_sel_req
 * Inputs         : transaction_id - Transaction identifier
 *                  error_code     -  Error code
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send negative acknowledgement
 *                  of SONAPPS_ACF_SELECTION_REQ message received from
 *                  SONMgmtIfH
 ****************************************************************************/
static son_void_t
send_acf_nack_to_oam_for_acf_sel_req
(
    son_u16 transaction_id,
    son_error_et error_code,
    son_apps_gb_context_t  *p_sonapps_gb_context
)
{
    sonapps_carrier_freq_selection_res_t acf_selection_res = {0};

    acf_selection_res.transaction_id = transaction_id;
    acf_selection_res.carrier_freq_selection_res.selected_carrier_freq =
        SONAPPS_INVALID_VALUE;
    acf_selection_res.carrier_freq_selection_res.result = SON_FAILURE;
    acf_selection_res.carrier_freq_selection_res.error_code = error_code;

    SON_LOG(p_sonapps_gb_context->acf_log_enable, p_son_apps_facility_name,
        SON_INFO, "[ACF] Sending negative "
                      "acknowledgement for SONAPPS_ACF_SELECTION_REQ message");

    /* Send SONAPPS_CARRIER_FREQ_SELECTION_RES to SONMgmtIfh with
     *  FAILURE result */
    son_create_send_buffer((son_u8 *)&acf_selection_res, SON_APPS_MODULE_ID,
        SON_MIF_MODULE_ID, SONAPPS_CARRIER_FREQ_SELECTION_RES,
        sizeof(sonapps_carrier_freq_selection_res_t));
} /* send_acf_nack_to_oam_for_acf_sel_req */

/*****************************************************************************
 * Function Name  : send_acf_nack_to_oam_for_acf_power_req
 * Inputs         : transaction_id  -  Transaction identifer
 *                  error_code      -  Error code
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send negative acknowledgement
 *                  of SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_REQ
 *                  message received from MIF
 ****************************************************************************/
static son_void_t
send_acf_nack_to_oam_for_acf_power_req
(
    son_u16 transaction_id,
    son_error_et error_code,
    son_apps_gb_context_t  *p_sonapps_gb_context
)
{
    sonapps_carrier_freq_and_power_selection_res_t acf_power_selection_res = {0};

    acf_power_selection_res.transaction_id = transaction_id;
    acf_power_selection_res.carrier_freq_dl_pwr_selection_resp.result = SON_FAILURE;
    acf_power_selection_res.carrier_freq_dl_pwr_selection_resp.error_code = error_code;

    SON_LOG(p_sonapps_gb_context->acf_log_enable, p_son_apps_facility_name,
        SON_INFO, "[ACF] Sending negative "
                      "acknowledgement for SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_REQ message");

    /* Send SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_RESP to SONMgmtIfh with
     *  FAILURE result */
    son_create_send_buffer((son_u8 *)&acf_power_selection_res, SON_APPS_MODULE_ID,
        SON_MIF_MODULE_ID, SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_RESP,
        sizeof(sonapps_carrier_freq_and_power_selection_res_t));
}

/*****************************************************************************
 * Function Name  : sonapps_fsm_acf_nack_handler
 * Inputs         : transaction_id - Transaction identifier
 *                  error_code     - Error code
 *                  api_id         - API ID name
 *                  p_sonapps_gb_context - Pointer to global context structure
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send negative acknowledgement
 *                  of SONAPPS_ACF_SELECTION_REQ message received from
 *                  SONMgmtIfH in SONAPP_STATE_ACTIVE state.
 ****************************************************************************/
static son_void_t
sonapps_fsm_acf_nack_handler
(
    son_u16 transaction_id,
    son_error_et error_code,
    son_u16 api_id,
    son_apps_gb_context_t  *p_sonapps_gb_context
)
{
    /* Send negative acknowledgement to OAM */

    if (SONAPPS_CARRIER_FREQ_SELECTION_REQ == api_id)
    {
        send_acf_nack_to_oam_for_acf_sel_req(transaction_id, error_code, p_sonapps_gb_context);
    }
    else if (SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_REQ == api_id)
    {
        send_acf_nack_to_oam_for_acf_power_req(transaction_id, error_code, p_sonapps_gb_context);
    }
}

/*****************************************************************************
 * Function Name  : sonapps_fsm_acf_invalid_state_selection_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to SONApps global context
 *                  structure
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This function is used to send negative acknowledgement
 *                  for the carrier frequency selection request message
 *                  received in SONAPP_STATE_INIT state
 ****************************************************************************/
son_buf_retain_status_et
sonapps_fsm_acf_invalid_state_selection_req_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
    sonapps_carrier_freq_selection_req_t *p_carrier_freq_selection_req =
        (sonapps_carrier_freq_selection_req_t *)p_msg;

    SON_UT_TRACE_ENTER();

    SON_LOG(p_sonapps_gb_context->acf_log_enable, p_son_apps_facility_name,
        SON_WARNING,
        "[ACF] Carrier frequency selection request message with transaction id "
        "[%u] received in unexpected INIT state",
        p_carrier_freq_selection_req->transaction_id);

    /* Send negative acknowledgement to OAM */
    sonapps_fsm_acf_nack_handler(p_carrier_freq_selection_req->transaction_id,
        SON_ERR_UNEXPECTED_MSG_IN_INIT_STATE, p_cspl_hdr->api,
        p_sonapps_gb_context);

    SON_UT_TRACE_EXIT();
    /* Release the buffer memory as it is no longer needed. */
    return RELEASE_SON_BUFFER;
} /* sonapps_fsm_acf_invalid_state_selection_req_handler */

/*****************************************************************************
 * Function Name  : sonapps_fsm_acf_invalid_state_selection_and_power_req_handler
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to SONApps global context
 *                  structure
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This function is used to send negative acknowledgement
 *                  for the carrier frequency selection request message
 *                  received in SONAPP_STATE_INIT state
 ****************************************************************************/
son_buf_retain_status_et
sonapps_fsm_acf_invalid_state_selection_and_power_req_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
    sonapps_carrier_freq_and_power_selection_req_t *p_carrier_freq_selection_req =
        (sonapps_carrier_freq_and_power_selection_req_t *)p_msg;

    SON_UT_TRACE_ENTER();

    SON_LOG(p_sonapps_gb_context->acf_log_enable, p_son_apps_facility_name,
        SON_WARNING,
        "[ACF] Carrier frequency selection request message with transaction id "
        "[%u] received in unexpected INIT state",
        p_carrier_freq_selection_req->transaction_id);

    /* Send negative acknowledgement to OAM */
    sonapps_fsm_acf_nack_handler(p_carrier_freq_selection_req->transaction_id,
        SON_ERR_UNEXPECTED_MSG_IN_INIT_STATE, p_cspl_hdr->api,
        p_sonapps_gb_context);

    SON_UT_TRACE_EXIT();
    /* Release the buffer memory as it is no longer needed. */
    return RELEASE_SON_BUFFER;
} /* sonapps_fsm_acf_invalid_state_selection_and_power_req_handler */

/*****************************************************************************
 * Function Name  : set_global_context_structure
 * Inputs         : p_sonapps_gb_context - Pointer to global context structure
 *                  p_carrier_freq_selection_req - Pointer to carrier
 *                  frequency selection request structure
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  structure
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to populate the global context
 *                  structure for ACF specific parameters
 ****************************************************************************/
son_void_t
set_global_context_structure
(
    son_apps_gb_context_t                *p_sonapps_gb_context,
    sonapps_carrier_freq_selection_req_t *p_carrier_freq_selection_req,
    STACKAPIHDR                          *p_cspl_hdr
)
{
    if (SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_REQ == p_cspl_hdr->api)
    {
        /* Set acf_selection_progress_status to SON_TRUE to signify
         *  that ACF selection procedure is in progress */
        p_sonapps_gb_context->acf_selection_progress_status[ACF_AND_POWER_REQ] =
            SON_TRUE;

        /* Save transaction id of carrier frequency selection request
         * in global context structure */
        p_sonapps_gb_context->transaction_id[SONAPPS_ACF_APPS] =
            p_carrier_freq_selection_req->transaction_id;

        /* Save first earfcn of carrier frequency selection request
         * in global context structure. When Network scan response
         * from NMM is received with SON_FAILURE result and error
         * code is SON_ERR_NMM_PHY_NULL_DATA then first EARFCN from
         * the input list is sent back in response */
        p_sonapps_gb_context->earfcn[ACF_AND_POWER_REQ] =
            p_carrier_freq_selection_req->carrier_freq_selection_req.earfcn[0];

        /* Store the measurement bandwidth received from RRM in the global
        *  context structure only when it is not being assigned by valid
        *  value from OAM initiated carrier frequency selection request */
        if (SON_NULL == p_sonapps_gb_context->meas_bandwidth)
        {
            p_sonapps_gb_context->meas_bandwidth =
                p_carrier_freq_selection_req->carrier_freq_selection_req.
                meas_bandwidth;
        }
    }
    else
    {
        /* Set acf_selection_progress_status to SON_TRUE to signify
         *  that ACF selection procedure is in progress */
        p_sonapps_gb_context->acf_selection_progress_status[ACF_SELECTION_REQ] =
            SON_TRUE;

        /* Save transaction id of carrier frequency selection request
         *  in global context structure */
        p_sonapps_gb_context->transaction_id[SONAPPS_ACF_OAM] =
            p_carrier_freq_selection_req->transaction_id;

        /* Save first earfcn of carrier frequency selection request
         *  in global context structure. When Network scan response
         *  from NMM is received with SON_FAILURE result and error
         *  code is SON_ERR_NMM_PHY_NULL_DATA then first EARFCN from
         *  OAM provided list is sent to OAM in
         *  SONAPPS_CARRIER_FREQ_SELECTION_RES */
        p_sonapps_gb_context->earfcn[ACF_SELECTION_REQ] = p_carrier_freq_selection_req->
                                                          carrier_freq_selection_req.earfcn[0];

        /* Save measurement bandwidhth of carrier frequency selection
         * request in global context structure */
        p_sonapps_gb_context->meas_bandwidth =
            p_carrier_freq_selection_req->carrier_freq_selection_req.
            meas_bandwidth;
    }
} /* set_global_context_structure */

/*****************************************************************************
 * Function Name  : sonapps_fsm_acf_selection_req
 * Inputs         : p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *                  p_cspl_hdr - Pointer to the the CSPL header information
 *                  p_sonapps_gb_context - Pointer to SONApps global context
 *                  structure
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - Encapsulating shell should release
 *                  the buffer or
 *                  RETAIN_SON_BUFFER - Encapsulating shell should retain
 *                  the buffer
 * Description    : This function is used to handle the carrier frequency
 *                  selection request message or DL power received from OAM
 *                  in SONAPP_STATE_ACTIVE state. It initiates
 *                  network scan through NMM.
 ****************************************************************************/
son_buf_retain_status_et
sonapps_fsm_acf_selection_req
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
)
{
    sonapps_carrier_freq_selection_req_t *p_carrier_freq_selection_req = SON_PNULL;
    sonapps_carrier_freq_selection_req_t carrier_freq_selection_msg;
    son_error_et status  = SON_NO_ERROR;

    SON_UT_TRACE_ENTER();
    /* SPR 15935 Fix Start */
    SON_MEMSET(&carrier_freq_selection_msg, 0, sizeof(sonapps_carrier_freq_selection_req_t));
    /* SPR 15935 Fix Stop */
    if (SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_REQ == p_cspl_hdr->api)
    {
        sonapps_carrier_freq_and_power_selection_req_t *p_input_buf  =
            (sonapps_carrier_freq_and_power_selection_req_t *)p_msg;
        /* SPR 15935 Fix Start */
        /*Lines Deleted*/
        /* SPR 15935 Fix Stop */
        carrier_freq_selection_msg.transaction_id = p_input_buf->transaction_id;
        carrier_freq_selection_msg.carrier_freq_selection_req.meas_bandwidth =
            p_input_buf->carrier_freq_dl_pwr_selection_req.meas_bandwidth;
        carrier_freq_selection_msg.carrier_freq_selection_req.num_earfcn =
            p_input_buf->carrier_freq_dl_pwr_selection_req.earfcn_list_size;

        SON_MEMCPY( carrier_freq_selection_msg.carrier_freq_selection_req.earfcn,
            p_input_buf->carrier_freq_dl_pwr_selection_req.earfcn_list,
            sizeof(carrier_freq_selection_msg.carrier_freq_selection_req.earfcn));
        p_carrier_freq_selection_req = &carrier_freq_selection_msg;

        /*Rel 1.4 Start */
        p_sonapps_gb_context->tx_power_params_from_oam.p_tx_upp =
            p_input_buf->carrier_freq_dl_pwr_selection_req.p_tx_upp;
        p_sonapps_gb_context->tx_power_params_from_oam.p_tx_low =
            p_input_buf->carrier_freq_dl_pwr_selection_req.p_tx_low;
        p_sonapps_gb_context->tx_power_params_from_oam.p_ue_max =
            p_input_buf->carrier_freq_dl_pwr_selection_req.p_ue_max;
        p_sonapps_gb_context->tx_power_params_from_oam.p_ue_min =
            p_input_buf->carrier_freq_dl_pwr_selection_req.p_ue_min;
        /*Rel 1.4 Stop */
    }
    else
    {
        p_carrier_freq_selection_req = (sonapps_carrier_freq_selection_req_t *)p_msg;
    }

    if (SON_NULL == p_carrier_freq_selection_req->
        carrier_freq_selection_req.num_earfcn)
    {
        /* Send nack when empty earfcn list is received */
        SON_LOG(p_sonapps_gb_context->acf_log_enable, p_son_apps_facility_name,
            SON_WARNING, "[ACF] empty earfcn "
                         "is received for carrier frequency selection.");

        sonapps_fsm_acf_nack_handler(p_carrier_freq_selection_req->
            transaction_id, SON_ERR_EMPTY_CARRIER_FREQ_LIST,
            p_cspl_hdr->api, p_sonapps_gb_context);
    }
    else if ((SONAPPS_CARRIER_FREQ_SELECTION_REQ == p_cspl_hdr->api) &&
             (SON_TRUE == p_sonapps_gb_context->
              acf_selection_progress_status[ACF_SELECTION_REQ]))
    {
        /* Send nack to OAM when ACF selection is in progress */
        SON_LOG(p_sonapps_gb_context->acf_log_enable, p_son_apps_facility_name,
            SON_WARNING, "[ACF] Processing "
                          "of carrier frequency selection request from OAM is in progress");

        sonapps_fsm_acf_nack_handler(p_carrier_freq_selection_req->
            transaction_id, SON_ERR_ACF_SELECTION_IN_PROGRESS,
            p_cspl_hdr->api, p_sonapps_gb_context);
    }
    else if ((SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_REQ == p_cspl_hdr->api) &&
             (SON_TRUE == p_sonapps_gb_context->
              acf_selection_progress_status[ACF_AND_POWER_REQ]))
    {
        /* Send nack to OAM when ACF  selection is in progress */
        SON_LOG(p_sonapps_gb_context->acf_log_enable, p_son_apps_facility_name,
            SON_WARNING, "[ACF] Processing "
                          "of carrier frequency selection and power request from OAM is in progress");

        sonapps_fsm_acf_nack_handler(p_carrier_freq_selection_req->
            transaction_id, SON_ERR_ACF_SELECTION_IN_PROGRESS,
            p_cspl_hdr->api, p_sonapps_gb_context);
    }
    else
    {
        /* Function call to prepare network scan request message
         * and send it to NMM */
        SON_LOG(p_sonapps_gb_context->acf_log_enable, p_son_apps_facility_name,
            SON_DETAILED, "[ACF] Function "
                          "call to prepare network scan request messsage and send it to NMM");

        status = sonapps_prepare_and_send_nw_scan_req(p_sonapps_gb_context,
            p_carrier_freq_selection_req, SON_ACF_FEATURE, p_cspl_hdr);

        /* Update global context structure only if network scan request
         * message is successfully sent to NMM */
        if (SON_NO_ERROR == status)
        {
            set_global_context_structure(p_sonapps_gb_context,
                p_carrier_freq_selection_req, p_cspl_hdr);
        }
        else if (SON_ERR_INVALID_EARFCN == status)
        {
            /* Send nack to OAM when invalid earfcn list is received from OAM */
            SON_LOG(p_sonapps_gb_context->acf_log_enable,
                p_son_apps_facility_name, SON_WARNING,
                "[ACF] Invalid EARFCN "
                "is received for carrier frequency selection ");

            sonapps_fsm_acf_nack_handler(p_carrier_freq_selection_req->
                transaction_id, SON_ERR_INVALID_EARFCN,
                p_cspl_hdr->api, p_sonapps_gb_context);
        }
        else
        {
            /* Memory failure */
            SON_LOG(p_sonapps_gb_context->acf_log_enable,
                p_son_apps_facility_name, SON_ERROR,
                "[ACF] Memory allocation failed "
                "for network scan request structure");
        }
    }

    /* Print global context structure */
    sonapps_print_global_context(p_sonapps_gb_context,
        p_sonapps_gb_context->acf_log_enable);

    SON_UT_TRACE_EXIT();
    return RELEASE_SON_BUFFER;
} /* sonapps_fsm_acf_selection_req */

/*****************************************************************************
 * Function Name  : sonapps_fsm_acf_selection_res_for_oam
 * Inputs         : p_msg - Pointer to the NMM scan response message data
 *                  p_sonapps_gb_context - Pointer to SONApps global context
 *                  structure
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to handle the
 *                  SONNMM_START_SCAN_RES message received from NMM
 *                  for carrier frequency selection
 ****************************************************************************/
son_void_t
sonapps_fsm_acf_selection_res_for_oam
(
    son_nmm_start_scan_res_t    *p_msg,
    son_apps_gb_context_t       *p_sonapps_gb_context
)
{
    start_scan_res_eutran_data_t *p_start_scan_res_data = SON_PNULL;
    nmm_scan_res_list_t *p_eutran_scan_res_list = SON_PNULL;
    sonapps_carrier_freq_selection_res_t acf_selection_res = {0};
    son_u32 count = 0;
    son_u8 min_rssi = 0xFF;
    son_u16 selected_carrier_freq = SONAPPS_INVALID_VALUE;
    son_return_et result = SON_FAILURE;

    SON_LOG(p_sonapps_gb_context->acf_log_enable, p_son_apps_facility_name,
        SON_DETAILED, "[ACF] Network "
                      "scan response message is received from NMM for carrier frequency "
                      "selection with transaction id = %u", p_msg->transaction_id);

    /* Initializing carrier frequency selection response structure */
    acf_selection_res.transaction_id = p_msg->transaction_id;
    acf_selection_res.carrier_freq_selection_res.selected_carrier_freq =
        selected_carrier_freq;
    acf_selection_res.carrier_freq_selection_res.result = SON_SUCCESS;
    acf_selection_res.carrier_freq_selection_res.error_code = SON_NO_ERROR;

    result = p_msg->start_scan_res.son_resp;

    /* Get the pointer to the NMM start scan resp list */
    p_eutran_scan_res_list = p_msg->start_scan_res.p_eutran_scan_res_list;

    /* Check if network scan response result is SUCCESS and pointer to
     *  p_eutran_scan_res_list (NMM scan response list) is not NULL */
    if ((SON_SUCCESS == result) && (SON_PNULL != p_eutran_scan_res_list))
    {
        son_u32 no_of_nodes = p_eutran_scan_res_list->count;
        /* Iterate through the response list */
        for (count = 0; count < no_of_nodes; count++)
        {
            /* Get data from the nmm start scan resp list */
            SCAN_RESP_LIST_DEQUEUE(start_scan_res_eutran_data_t, p_eutran_scan_res_list,
                p_start_scan_res_data);

            /* Checking p_start_scan_res_data pointer to remove 
             *  warning though this pointer will never be null*/
            if (SON_PNULL == p_start_scan_res_data)
            {
                acf_selection_res.carrier_freq_selection_res.result =
                    SON_FAILURE;
                acf_selection_res.carrier_freq_selection_res.error_code =
                    SON_ERR_ACF_INTERNAL_FAILURE;

                SON_LOG(p_sonapps_gb_context->acf_log_enable,
                    p_son_apps_facility_name, SON_ERROR,
                    "[ACF] Failed "
                    "to read data from NMM scan response list");
                break; /* Break out from for loop */
            }

            /* Enqueue the dequed data as it is needed by NMM */
            SCAN_RESP_LIST_ENQUEUE(p_eutran_scan_res_list, p_start_scan_res_data);

            /* If the rssi in NMM scan response list is less than the
             *  selected minimum rssi then update the selected rssi value
             *  and also update the earfcn corresponding to the minimum rssi */
            if (p_start_scan_res_data->scan_res_data.rssi < min_rssi)
            {
                min_rssi = p_start_scan_res_data->scan_res_data.rssi;
                selected_carrier_freq = p_start_scan_res_data->
                                        scan_res_data.earfcn;
            }
        }

        /* EARFCN corresponding to minimum rssi is selected */
        acf_selection_res.carrier_freq_selection_res.selected_carrier_freq
            = selected_carrier_freq;
    }
    else /* FAILURE result is received from NMM */
    {
        if (SON_ERR_NMM_PHY_NULL_DATA == p_msg->start_scan_res.son_error)
        {
            /* Select first earfcn from OAM provided list when PHY_NULL_DATA
             *  error code is received from NMM */
            SON_LOG(p_sonapps_gb_context->acf_log_enable,
                p_son_apps_facility_name, SON_WARNING,
                "[ACF] Network scan "
                "response for carrier frequency selection is received from "
                "NMM with result = FAILURE and "
                "error code = SON_ERR_NMM_PHY_NULL_DATA");

            acf_selection_res.carrier_freq_selection_res.selected_carrier_freq
                = p_sonapps_gb_context->earfcn[ACF_SELECTION_REQ];
        }
        else
        {
            /* When an error occured in network scan response with error code
             *  other than PHY_NULL_DATA, send an invalid carrier frequency in
             *  carrier frequency selection response with FAILURE result and
             *  error code set to SON_ERR_ACF_INTERNAL_FAILURE*/
            SON_LOG(p_sonapps_gb_context->acf_log_enable,
                p_son_apps_facility_name, SON_WARNING,
                "[ACF] Network scan "
                "response for carrier frequency selection is received from "
                "NMM with result = FAILURE and error code = %d",
                p_msg->start_scan_res.son_error);

            acf_selection_res.carrier_freq_selection_res.result = SON_FAILURE;
            acf_selection_res.carrier_freq_selection_res.error_code =
                p_msg->start_scan_res.son_error;
        }
    }

    SON_LOG(p_sonapps_gb_context->acf_log_enable, p_son_apps_facility_name,
        SON_DETAILED, "[ACF] Carrier "
                      "frequency selection response - Transaction id = [%u], "
                      "EARFCN = [%u], Result = [%d] "
                      "and Error code = [%d]",
        acf_selection_res.transaction_id,
        acf_selection_res.carrier_freq_selection_res.selected_carrier_freq,
        acf_selection_res.carrier_freq_selection_res.result,
        acf_selection_res.carrier_freq_selection_res.error_code);

    /* Send SONAPPS_CARRIER_FREQ_SELECTION_RES to SONMgmtIfh */
    son_create_send_buffer((son_u8 *)&acf_selection_res, SON_APPS_MODULE_ID,
        SON_MIF_MODULE_ID, SONAPPS_CARRIER_FREQ_SELECTION_RES,
        sizeof(sonapps_carrier_freq_selection_res_t));

    SON_LOG(p_sonapps_gb_context->acf_log_enable, p_son_apps_facility_name,
        SON_DETAILED, "[ACF] Carrier "
                      "frequency selection response successfully sent from SONApps to SMIF");

    /* Set carrier frequency selection progress status to FALSE */
    p_sonapps_gb_context->acf_selection_progress_status[ACF_SELECTION_REQ] = SON_FALSE;
    /* Reset ACF transaction id to invalid value */
    p_sonapps_gb_context->transaction_id[SONAPPS_ACF_OAM] =
        SONAPPS_INVALID_VALUE;
    /* Reset EARFCN to invalid value */
    p_sonapps_gb_context->earfcn[ACF_SELECTION_REQ] = SONAPPS_INVALID_VALUE;

    /* Print global context structure */
    sonapps_print_global_context(p_sonapps_gb_context,
        p_sonapps_gb_context->acf_log_enable);
} /* sonapps_fsm_acf_selection_res_for_oam */

/* LTE_SON_KLOCWORK_WARN_24JULY_2012_START */

/*****************************************************************************
 * Function Name  : print_carrier_freq_selection_resp
 * Inputs         : p_in_msg - Pointer to the NMM scan response message data
 *                  p_sonapps_gb_context - Pointer to SONApps global context
 *                  structure
 *                : p_out_msg - Pointer to
 *                  son_apps_rrm_carrier_freq_and_dl_power_params_resp_t
 *                  structure
 * Outputs        : None
 * Returns        : None
 * Description    : This function prints the structure pointed by p_out_msg
 ****************************************************************************/
static son_void_t
print_carrier_freq_selection_resp
(
    son_nmm_start_scan_res_t    *p_in_msg,
    son_apps_gb_context_t       *p_sonapps_gb_context,
    son_apps_carrier_freq_and_dl_power_params_resp_t *p_out_msg
)
{
    SON_LOG(p_sonapps_gb_context->acf_log_enable, p_son_apps_facility_name,
        SON_DETAILED, "[ACF] Carrier "
                      "frequency selection response - Transaction id = [%u], "
                      "Result = [%d], Error code = [%d], "
                      "EARFCN = [%u], RSSI = [%u], "
                      "Number of neighboring cells = [%u]",
        p_in_msg->transaction_id,
        p_out_msg->result, p_out_msg->error_code,
        p_out_msg->carrier_freq_and_dl_power_params_resp.earfcn,
        p_out_msg->carrier_freq_and_dl_power_params_resp.rssi,
        p_out_msg->carrier_freq_and_dl_power_params_resp.
        neighbor_cell_list_size);
}

/*****************************************************************************
 * Function Name  : son_apps_get_min_rssi
 * Inputs         : p_eutran_scan_res_list - Pointer to nmm scan resp list
 *                : p_start_scan_res_data - Pointer to start_scan_res_eutran_data_t
 *                  structure
 *                : p_sonapps_gb_context - Pointer to SONApps global context
 * Outputs        : None
 * Returns        : Pointer to the start_scan_res_eutran_data_t with the least
 *                : rssi value
 * Description    : This function is used to identify the earfcn with the
 *                : least rssi value
 ****************************************************************************/
static start_scan_res_eutran_data_t *
son_apps_get_min_rssi
(
    nmm_scan_res_list_t *p_eutran_scan_res_list,
    son_apps_gb_context_t       *p_sonapps_gb_context
)
{
    son_u8 min_rssi = 0xFF;
    start_scan_res_eutran_data_t *p_temp_data = SON_PNULL;
    start_scan_res_eutran_data_t *p_start_scan_res_data = SON_PNULL;
    son_u32 no_of_nodes = p_eutran_scan_res_list->count;
    son_u32 count = SON_NULL;

    /* Iterate through the response list */
    for (count = 0; count < no_of_nodes; count++)
    {
        /* Get data from the nmm start scan resp list */
        SCAN_RESP_LIST_DEQUEUE(start_scan_res_eutran_data_t, p_eutran_scan_res_list,
            p_start_scan_res_data);

        /* Checking p_start_scan_res_data pointer to remove klocwork
         * warning though this pointer will never be null*/
        if (SON_PNULL == p_start_scan_res_data)
        {
            SON_LOG(p_sonapps_gb_context->acf_log_enable,
                p_son_apps_facility_name, SON_ERROR,
                "[ACF] Failed "
                "to read data from NMM scan response list");
            break; /* Break out from for loop */
        }

        /* If the rssi in NMM scan response list is less than the
         * selected minimum rssi then update the selected rssi value
         * and also update the earfcn corresponding to the minimum rssi */
        if (p_start_scan_res_data->scan_res_data.rssi < min_rssi)
        {
            min_rssi = p_start_scan_res_data->scan_res_data.rssi;
            p_temp_data = p_start_scan_res_data;
        }
    }

    return p_temp_data;
} /* son_apps_get_min_rssi */

/*****************************************************************************
 * Function Name  : son_get_max
 * Inputs         : val1 - first value
 *                : val2 - second value
 * Outputs        : None
 * Returns        : Larger value out of val1 and val2
 * Description    : This function is used to find the larger value out of the
 *                  two son_u16 values
 ****************************************************************************/
static son_s16
son_get_max
(
    son_s16 val1,
    son_s16 val2
)
{
    son_s16 max_val = val1;

    if (val2 > val1)
    {
        max_val = val2;
    }

    return max_val;
}

/*****************************************************************************
 * Function Name  : son_get_min
 * Inputs         : val1 - first value
 *                : val2 - second value
 * Outputs        : None
 * Returns        : Smaller value out of val1 and val2
 * Description    : This function is used to find the smaller value out of the
 *                  two son_u16 values
 ****************************************************************************/
static son_s16
son_get_min
(
    son_s16 val1,
    son_s16 val2
)
{
    son_s16 min_val = val1;

    if (val2 < val1)
    {
        min_val = val2;
    }

    return min_val;
}

/*****************************************************************************
 * Function Name  : son_apps_calculate_tx_power
 * Inputs         : max_ref_signal_power - maximum reference signal power
 *                : p_gl_ctxt - pointer to son apps global context
 * Outputs        : None
 * Returns        : tx_power_val - calculated tx power
 * Description    : This function is used to calculate tx power
 ****************************************************************************/
static son_s8
son_apps_calculate_tx_power
(
    son_s16 max_ref_signal_power,
    son_apps_gb_context_t *p_gl_ctxt
)
{
    son_s8 tx_power_val = 0;
    /*STATIC ICIC CHANGE START */
    son_s16 tuned_ref_signal_power = 0;

    /*STATIC ICIC CHANGE END */
    SON_UT_TRACE_ENTER();

    /*STATIC ICIC CHANGE START */

    /*
     * update the refrence signal power value recieved from son as per
     * the tunable/configurable parameters alpha and beta.
     */
    tuned_ref_signal_power =
        (((p_gl_ctxt->icic_params).dl_tx_alpha) * max_ref_signal_power)
        + ((p_gl_ctxt->icic_params).dl_tx_beta);
    SON_LOG(p_gl_ctxt->apps_log_enable, p_son_apps_facility_name, SON_INFO,
        "tuned_ref_signal_power [%d]", tuned_ref_signal_power);

    /*calculating tx power from the various power level */
    tx_power_val = son_get_max(son_get_min(tuned_ref_signal_power,
            p_gl_ctxt->tx_power_params_from_oam.p_tx_upp),
        p_gl_ctxt->tx_power_params_from_oam.p_tx_low);
    /*STATIC ICIC CHANGE END */

    SON_UT_TRACE_EXIT();

    return tx_power_val;
} /* son_apps_calculate_tx_power */

/*****************************************************************************
 * Function Name  : son_apps_calculate_ue_tx_power
 * Inputs         : dl_path_loss - dl path loss value
 *                : p_gl_ctxt - pointer to son apps global context
 * Outputs        : None
 * Returns        : ue_tx_power_val - calculated ue tx power
 * Description    : This function is used to calculate ue tx power
 ****************************************************************************/
static son_s8
son_apps_calculate_ue_tx_power
(
    son_s16 dl_path_loss,
    son_apps_gb_context_t *p_gl_ctxt
)
{
    son_s8 ue_tx_power_val = 0;
    /* uplink path loss is same as downlink path loss */
    son_s16 ul_path_loss = dl_path_loss;
    son_s16 tuned_ue_tx_power = 0;

    SON_UT_TRACE_ENTER();

    /*calculating ue tx power from the various power level */
    tuned_ue_tx_power = p_gl_ctxt->tx_power_params_from_oam.p_ue_max -
                        ((p_gl_ctxt->icic_params).ul_tx_alpha) *
                        (((p_gl_ctxt->icic_params).ul_offset_k) - ul_path_loss);
    SON_LOG(p_gl_ctxt->apps_log_enable, p_son_apps_facility_name, SON_INFO,
        "tuned_ue_tx_power [%d]", tuned_ue_tx_power);

    ue_tx_power_val =  son_get_min(p_gl_ctxt->tx_power_params_from_oam.p_ue_max,
        son_get_max(tuned_ue_tx_power,
            p_gl_ctxt->tx_power_params_from_oam.p_ue_min));

    SON_UT_TRACE_EXIT();
    return ue_tx_power_val;
} /* son_apps_calculate_ue_tx_power */

/*****************************************************************************
 * Function Name  : son_apps_process_son_carrier_freq_dl_power_resp
 * Inputs         : p_msg - pointer to son_apps_carrier_freq_and_dl_power_params_resp_t
 *                : tr_id - value of tr id
 *                : p_gl_ctxt - pointer to son apps global context
 * Outputs        : None
 * Returns        : ue_tx_power_val - calculated ue tx power
 * Description    : This function is used to calculate ue tx power
 ****************************************************************************/
static son_void_t
son_apps_process_son_carrier_freq_dl_power_resp
(
    son_apps_carrier_freq_and_dl_power_params_resp_t    *p_msg,
    son_u16 tr_id,
    son_apps_gb_context_t  *p_gl_ctxt
)
{
    son_s8 tx_power = 0;
    son_s16 rrm_max_ref_signal_power = 0;
    son_u8 neighbr_cell_count = 0;
    son_s16 rrm_rsrp_val = 0;
    sonapps_carrier_freq_and_power_selection_res_t carrier_freq_and_power_res;

    /*STATIC ICIC UL START*/
    son_s16 dl_path_loss = 0;
    son_s8 ue_tx_power  = 0;

    /*STATIC ICIC UL END*/
    SON_UT_TRACE_ENTER();

    SON_MEMSET(&carrier_freq_and_power_res, 0, sizeof(sonapps_carrier_freq_and_power_selection_res_t));
    carrier_freq_and_power_res.transaction_id = tr_id;
    if (SON_FAILURE ==  p_msg->result)
    {
        carrier_freq_and_power_res.carrier_freq_dl_pwr_selection_resp.result = p_msg->result;
        carrier_freq_and_power_res.carrier_freq_dl_pwr_selection_resp.error_code = p_msg->error_code;
    }
    else
    {
        rrm_max_ref_signal_power =  p_msg->carrier_freq_and_dl_power_params_resp.
                                   neighbor_cell_signal_power[0].reference_signal_power;

        rrm_rsrp_val = p_msg->carrier_freq_and_dl_power_params_resp.neighbor_cell_signal_power[0].rsrp;

        for (neighbr_cell_count = 1;
             neighbr_cell_count < p_msg->carrier_freq_and_dl_power_params_resp.
             neighbor_cell_list_size;
             neighbr_cell_count++)
        {
            if ((p_msg->carrier_freq_and_dl_power_params_resp.
                 neighbor_cell_signal_power[neighbr_cell_count].reference_signal_power)  > rrm_max_ref_signal_power)
            {
                rrm_max_ref_signal_power = p_msg->carrier_freq_and_dl_power_params_resp.
                                           neighbor_cell_signal_power[neighbr_cell_count].reference_signal_power;
                rrm_rsrp_val = p_msg->carrier_freq_and_dl_power_params_resp.
                               neighbor_cell_signal_power[neighbr_cell_count].rsrp;
            }
            else if ((p_msg->carrier_freq_and_dl_power_params_resp.
                      neighbor_cell_signal_power[neighbr_cell_count].reference_signal_power) ==
                     rrm_max_ref_signal_power)
            {
                /*To minimize the Path Loss ,get the minimun RSRP value */
                if ((p_msg->carrier_freq_and_dl_power_params_resp.
                     neighbor_cell_signal_power[neighbr_cell_count].rsrp) < rrm_rsrp_val)
                {
                    rrm_rsrp_val = p_msg->carrier_freq_and_dl_power_params_resp.
                                   neighbor_cell_signal_power[neighbr_cell_count].rsrp;
                }
            }
        } /*for loop ends*/

        SON_LOG(p_gl_ctxt->apps_log_enable, p_son_apps_facility_name, SON_DETAILED,
            "max_ref_signal_power[%d]  rsrp_val[%d]", rrm_max_ref_signal_power, rrm_rsrp_val);

        /*Calculate transmisson power*/
        tx_power = son_apps_calculate_tx_power(rrm_max_ref_signal_power, p_gl_ctxt);

        /* Calculate DL path loss */
        dl_path_loss = rrm_rsrp_val - rrm_max_ref_signal_power;

        /* Calculate UE transmit power */
        ue_tx_power  = son_apps_calculate_ue_tx_power(dl_path_loss,
            p_gl_ctxt);

        carrier_freq_and_power_res.carrier_freq_dl_pwr_selection_resp.bitmask |=
            SON_OAM_CARR_FREQ_DL_TX_PWR_DL_EARFCN_PRESENT |
            SON_OAM_CARR_FREQ_DL_TX_PWR_REF_SIGNAL_POWER_PRESENT |
            SON_OAM_CARR_FREQ_DL_TX_PWR_P_MAX_PRESENT;

        carrier_freq_and_power_res.carrier_freq_dl_pwr_selection_resp.dl_earfcn =
            p_msg->carrier_freq_and_dl_power_params_resp.earfcn;

        carrier_freq_and_power_res.carrier_freq_dl_pwr_selection_resp.reference_signal_power =
            tx_power;
        carrier_freq_and_power_res.carrier_freq_dl_pwr_selection_resp.p_max = ue_tx_power;

        carrier_freq_and_power_res.carrier_freq_dl_pwr_selection_resp.result = p_msg->result;
    }

    son_create_send_buffer((son_u8 *)&carrier_freq_and_power_res, SON_APPS_MODULE_ID,
        SON_MIF_MODULE_ID, SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_RESP,
        sizeof(sonapps_carrier_freq_and_power_selection_res_t));
} /* son_apps_process_son_carrier_freq_dl_power_resp */

/*****************************************************************************
 * Function Name  : sonapps_fsm_acf_selection_res_for_apps
 * Inputs         : p_msg - Pointer to the NMM scan response message data
 *                  p_sonapps_gb_context - Pointer to SONApps global context
 *                  structure
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to handle the
 *                  SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_REQ message
 *                  received from RRM for carrier frequency and DL power params
 ****************************************************************************/
son_void_t
sonapps_fsm_acf_selection_res_for_apps
(
    son_nmm_start_scan_res_t    *p_msg,
    son_apps_gb_context_t       *p_sonapps_gb_context
)
{
    start_scan_res_eutran_data_t *p_temp_data = SON_PNULL;
    nmm_scan_res_list_t *p_eutran_scan_res_list = SON_PNULL;
    /* ACF response to be sent to RRM */
    son_apps_carrier_freq_and_dl_power_params_resp_t carrier_freq_res;
    son_u8 min_rssi = 0xFF;
    son_u16 cell_index = SON_NULL;
    son_float rsrq_in_db = 0.0;
    son_float meas_bw_in_db = 0.0;
    son_u16 selected_carrier_freq = SONAPPS_INVALID_VALUE;
    son_return_et result = SON_FAILURE;

    SON_LOG(p_sonapps_gb_context->acf_log_enable, p_son_apps_facility_name,
        SON_DETAILED, "[ACF] Network "
                      "scan response message is received from NMM with "
                      "transaction id = %u", p_msg->transaction_id);

    /* Initializing carrier frequency selection response structure */
    SON_MEMSET(&carrier_freq_res, 0,
        sizeof(son_apps_carrier_freq_and_dl_power_params_resp_t));
    carrier_freq_res.result = SON_SUCCESS;
    carrier_freq_res.error_code = SON_NO_ERROR;
    carrier_freq_res.carrier_freq_and_dl_power_params_resp.earfcn =
        selected_carrier_freq;

    result = p_msg->start_scan_res.son_resp;

    /* Get the pointer to the NMM start scan resp list */
    p_eutran_scan_res_list = p_msg->start_scan_res.p_eutran_scan_res_list;

    /* Check if network scan response result is SUCCESS and pointer to
     *  p_eutran_scan_res_list (NMM scan response list) is not NULL */
    if ((SON_SUCCESS == result) && (SON_PNULL != p_eutran_scan_res_list))
    {
        p_temp_data = son_apps_get_min_rssi(p_eutran_scan_res_list,
            p_sonapps_gb_context );

        if (SON_PNULL != p_temp_data)
        {
            min_rssi = p_temp_data->scan_res_data.rssi;
            /* EARFCN corresponding to minimum rssi is selected */
            carrier_freq_res.carrier_freq_and_dl_power_params_resp.earfcn =
                p_temp_data->scan_res_data.earfcn;

            /* Minimum rssi value corresponding to selected EARFCN */
            CONVERT_RSSI_TO_3GPP_REPORTED_VALUE(min_rssi,
                carrier_freq_res.carrier_freq_and_dl_power_params_resp.rssi);

            /* Neighbour cell list size */
            carrier_freq_res.carrier_freq_and_dl_power_params_resp.
            neighbor_cell_list_size = p_temp_data->scan_res_data.
                                      num_of_cells;

            /* Neighboring PCI corresponding to selected EARFCN */
            for (cell_index = 0;
                 cell_index < p_temp_data->scan_res_data.num_of_cells;
                 cell_index++)
            {
                /* Copy neighboring PCI id */
                carrier_freq_res.carrier_freq_and_dl_power_params_resp.
                neighbor_cell_signal_power[cell_index].pci_id =
                    p_temp_data->scan_res_data.cell[cell_index].pci_id;

                /* RSRP */
                CONVERT_RSRP_TO_3GPP_REPORTED_VALUE(p_temp_data->
                    scan_res_data.cell[cell_index].rsrp,
                    carrier_freq_res.carrier_freq_and_dl_power_params_resp.
                    neighbor_cell_signal_power[cell_index].rsrp);

                /* RSRQ calculation
                 * Covert the number of RBs/Meas BW into DB */
                meas_bw_in_db = convert_meas_bw_in_db(p_temp_data->scan_res_data.
                    cell[cell_index].allowed_meas_bandwidth);

                /* Find RSRQ using RSRQ = #ofRB in BW * (RSRP/RSSI).
                 *  The calculated value will be in db.
                 *  Subract 144 from rsrp and rssi to get their db equivalent */
                rsrq_in_db = meas_bw_in_db + (p_temp_data->scan_res_data.
                                              cell[cell_index].rsrp - 144) - (p_temp_data->scan_res_data.
                                                                              rssi - 144);

                /* Perform RSRQ measurement report mapping.
                 *  Possible range of rsrq is 0 to 34. */
                CONVERT_RSRQ_DB_TO_REPORTED_VALUE(rsrq_in_db,
                    carrier_freq_res.carrier_freq_and_dl_power_params_resp.
                    neighbor_cell_signal_power[cell_index].rsrq);

                /* Reference Signal Power */
                carrier_freq_res.carrier_freq_and_dl_power_params_resp.
                neighbor_cell_signal_power[cell_index].
                reference_signal_power = p_temp_data->scan_res_data.
                                         cell[cell_index].reference_signal_power;
            }
        }
        else
        {
            carrier_freq_res.result = SON_FAILURE;
            carrier_freq_res.error_code = SON_ERR_ACF_INTERNAL_FAILURE;
        }
    }
    else /* FAILURE result is received from NMM */
    {
        if (SON_ERR_NMM_PHY_NULL_DATA == p_msg->start_scan_res.son_error)
        {
            /* Select first earfcn from OAM provided list when PHY_NULL_DATA
             *  error code is received from NMM */
            SON_LOG(p_sonapps_gb_context->acf_log_enable,
                p_son_apps_facility_name, SON_WARNING,
                "[ACF] Network scan "
                "response is received with result = FAILURE and error code = "
                "SON_ERR_ACF_NO_NEIGHBOR_FOUND");

            carrier_freq_res.result = SON_FAILURE;
            carrier_freq_res.error_code = SON_ERR_ACF_NO_NEIGHBOR_FOUND;
            carrier_freq_res.carrier_freq_and_dl_power_params_resp.earfcn
                = p_sonapps_gb_context->earfcn[ACF_AND_POWER_REQ];
        }
        else
        {
            /* When an error occured in network scan response with error code
             *  other than PHY_NULL_DATA, send an invalid carrier frequency in
             *  carrier frequency selection response with FAILURE result and
             *  error code set to SON_ERR_ACF_INTERNAL_FAILURE*/
            SON_LOG(p_sonapps_gb_context->acf_log_enable,
                p_son_apps_facility_name, SON_WARNING,
                "[ACF] Network scan "
                "response is received with result = FAILURE and "
                "error code = %d", p_msg->start_scan_res.son_error);

            carrier_freq_res.result = SON_FAILURE;
            carrier_freq_res.error_code = SON_ERR_ACF_INTERNAL_FAILURE;
        }
    }

    /* Print Carrier frequency selection response to be sent to RRM */
    print_carrier_freq_selection_resp(p_msg, p_sonapps_gb_context, &carrier_freq_res );

    /* Rel 1.4 Starts*/

    son_apps_process_son_carrier_freq_dl_power_resp(&carrier_freq_res, p_msg->transaction_id, p_sonapps_gb_context);

    /* Rel 1.4 Stop*/

    /* Set carrier frequency selection progress status to FALSE */
    p_sonapps_gb_context->acf_selection_progress_status[ACF_AND_POWER_REQ] = SON_FALSE;
    /* Reset ACF transaction id to invalid value */
    p_sonapps_gb_context->transaction_id[SONAPPS_ACF_APPS] =
        SONAPPS_INVALID_VALUE;
    /* Reset EARFCN to invalid value */
    p_sonapps_gb_context->earfcn[ACF_AND_POWER_REQ] = SONAPPS_INVALID_VALUE;

    /* Print global context structure */
    sonapps_print_global_context(p_sonapps_gb_context,
        p_sonapps_gb_context->acf_log_enable);
} /* sonapps_fsm_acf_selection_res_for_apps */

/* LTE_SON_KLOCWORK_WARN_13JULY_2012_STOP */
