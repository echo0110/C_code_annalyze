/*! \file son_apps_acf.h
 *  \brief This file contains events and definitions for SONApps Carrier
 *         Frequency Selection feature.
 *  \date May 15, 2012
 *  \author Sonal Jain
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_APPS_ACF_H_
#define _SON_APPS_ACF_H_

/*! \headerfile son_apps_common_intf.h <>
 */
#include <son_apps_common_intf.h>

/*Rel 1.4 Start*/
#define DL_TX_ALPHA   1
#define DL_TX_BETA    20
#define UL_TX_ALPHA   1
#define UL_OFFSET_K   3

/*Rel 1.4 Stop*/

/*! \fn son_buf_retain_status_et sonapps_fsm_acf_invalid_state_selection_req_handler(
 *      son_u8 *p_msg, STACKAPIHDR *p_cspl_hdr, son_apps_gb_context_t
 * p_sonapps_gb_context)
 *  \brief This function is used to send negative acknowledgement
 *         of SONAPPS_CARRIER_FREQ_SELECTION_REQ message
 *  \param p_msg - Pointer to the message data received
 *  \param p_cspl_hdr - Pointer to the the CSPL header information
 *  \param p_sonapps_gb_context - Pointer to SONApps global context
 *                                structure
 */
son_buf_retain_status_et
sonapps_fsm_acf_invalid_state_selection_req_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
);

/*! \fn son_buf_retain_status_et sonapps_fsm_acf_selection_req(
 *     son_u8 *p_msg, STACKAPIHDR *p_cspl_hdr, son_apps_gb_context_t
 * p_sonapps_gb_context)
 *  \brief This function is used to handle the SONAPPS_ACF_SELECTION_REQ
 *         message received from SONMgmtIfH
 *  \param p_msg - Pointer to the message data received
 *  \param p_cspl_hdr - Pointer to the the CSPL header information
 *  \param p_sonapps_gb_context - Pointer to SONApps global context
 *                                structure
 */
son_buf_retain_status_et
sonapps_fsm_acf_selection_req
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
);

/*! \fn son_void_t sonapps_fsm_acf_selection_res_for_oam(
 *     son_nmm_start_scan_res_t *p_msg,
 *     son_apps_gb_context_t *p_sonapps_gb_context)
 *  \brief This function is used to handle the SONNMM_START_SCAN_RES
 *         message received from NMM for carrier frequency selection
 *  \param p_msg - Pointer to the NMM scan response message data
 *  \param p_sonapps_gb_context - Pointer to SONApps global context
 *                                structure
 */
son_void_t
sonapps_fsm_acf_selection_res_for_oam
(
    son_nmm_start_scan_res_t    *p_msg,
    son_apps_gb_context_t       *p_sonapps_gb_context
);

/*! \fn son_void_t sonapps_fsm_acf_selection_res_for_apps(
 *     son_nmm_start_scan_res_t *p_msg,
 *     son_apps_gb_context_t *p_sonapps_gb_context)
 *  \brief This function is used to handle the
 *         SON_APPS_RRM_CARRIER_FREQ_AND_DL_POWER_PARAMS_REQ message
 *         received from RRM for carrier frequency and DL power params
 *  \param p_msg - Pointer to the NMM scan response message data
 *  \param p_sonapps_gb_context - Pointer to SONApps global context
 *                                structure
 */
son_void_t
sonapps_fsm_acf_selection_res_for_apps
(
    son_nmm_start_scan_res_t    *p_msg,
    son_apps_gb_context_t       *p_sonapps_gb_context
);

/*! \fn sonapps_fsm_acf_invalid_state_selection_and_power_req_handler(
 *  son_u8                  *p_msg,
 *  STACKAPIHDR             *p_cspl_hdr,
 *  son_apps_gb_context_t   *p_sonapps_gb_context)
 *  \brief This function is used to send negative acknowledgement
 *         for the carrier frequency selection request message
 *         received in SONAPP_STATE_INIT state
 *  \param p_msg - Pointer to the message data received (after the
 *                  CSPL header)
 *         p_cspl_hdr - Pointer to the the CSPL header information
 *         p_sonapps_gb_context - Pointer to SONApps global context
 *                  structure
 */
son_buf_retain_status_et
sonapps_fsm_acf_invalid_state_selection_and_power_req_handler
(
    son_u8                  *p_msg,
    STACKAPIHDR             *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
);

#endif  /*_SON_APPS_ACF_H_ */
