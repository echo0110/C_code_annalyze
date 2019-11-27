/* REL1.1 CHANGES : STARTS */

/*! \file son_apps_pci.h
 *  \brief This file contains the states events and function definitions
 *         for SONAPPS PCI feature.
 *  \date May 17, 2012
 *  \author gur25767
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_APPS_PCI_H_
#define _SON_APPS_PCI_H_

/*! \headerfile son_apps_common_intf.h <>
 */
#include <son_apps_common_intf.h>

/*! \def SONAPPS_INVALID_PCI
 *  \brief This macro stores the invalid value for PCI
 */
#define SONAPPS_INVALID_PCI       504
/* Spr 17753 Changes Start */
/*! \def SONAPPS_PCI_RESELECTION_NOT_REQUIRED 
 *  \brief This macro stores the pci reselection not reqd value for PCI
 */
#define SONAPPS_PCI_RESELECTION_NOT_REQUIRED     505
/* Spr 17753 Changes End */
/*! \a decl SONAPPS_PCI_FSM_STATES_NAMES
 *  \brief Pointer to array storing PCI FSM state names
 */
extern const S8 *SONAPPS_PCI_FSM_STATES_NAMES[];

/*! \  son_apps_pci_fsm_evt_handler_t
 *  \brief   Pointer to the handler functions of PCI feature FSM
 */
typedef son_buf_retain_status_et (*son_apps_pci_fsm_evt_handler_t)
(
    son_u8 *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
);

/*! \fn     son_buf_retain_status_et sonapps_pci_fsm_invalid_event_handler(
 *          son_u8      *p_msg,
 *          STACKAPIHDR *p_cspl_hdr,
 *          son_apps_gb_context_t   *p_sonapps_gb_context
 *          )
 *  \brief  This is the handler function for the messages received
 *          from SONMgmtIfH for PCI feature in invalid state of PCI
 *          or SONAPPS.
 *  \param  p_msg      - Pointer to the message data received (after
 *                       the CSPL header)
 *  \param  p_cspl_hdr - Pointer to the the CSPL header information
 *  \param  p_sonapps_gb_context - Pointer to the global context of
 *                                 SONAPPS
 */

son_buf_retain_status_et
sonapps_pci_fsm_invalid_event_handler
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
);

/*! \fn     son_buf_retain_status_et sonapps_pci_fsm_enable_disable_req_nack(
 *          son_u8      *p_msg,
 *          STACKAPIHDR *p_cspl_hdr,
 *          son_apps_gb_context_t   *p_sonapps_gb_context
 *          )
 *  \brief  This is the handler function for
 *          SONAPPS_PCI_ENABLE_DISABLE_REQ message received from
 *          SONMgmtIfH if the message is received in an invalid state
 *          of PCI or SONAPPS.
 *  \param  p_msg      - Pointer to the message data received (after
 *                       the CSPL header)
 *  \param  p_cspl_hdr - Pointer to the the CSPL header information
 *  \param  p_sonapps_gb_context - Pointer to the global context of
 *                                 SONAPPS
 */
son_buf_retain_status_et
sonapps_pci_fsm_enable_disable_req_nack
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
);

/*! \fn     son_buf_retain_status_et sonapps_pci_fsm_selection_req_nack(
 *           son_u8      *p_msg,
 *           STACKAPIHDR *p_cspl_hdr,
 *           son_apps_gb_context_t   *p_sonapps_gb_context
 *          )
 *  \brief  This is the handler function for SONAPPS_PCI_RESELECTION_REQ
 *          message received from SONMgmtIfH, if the message is
 *          received in an invalid state of PCI or SONAPPS.
 *  \param  p_msg      - Pointer to the message data received (after
 *                       the CSPL header)
 *  \param  p_cspl_hdr - Pointer to the the CSPL header information
 *  \param  p_sonapps_gb_context - Pointer to the global context of
 *                                 SONAPPS
 */
son_buf_retain_status_et
sonapps_pci_fsm_selection_req_nack
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
);

/*! \fn     son_buf_retain_status_et sonapps_pci_fsm_reselection_req_nack(
 *          son_u8      *p_msg,
 *          STACKAPIHDR *p_cspl_hdr,
 *          son_apps_gb_context_t   *p_sonapps_gb_context
 *          )
 *  \brief  This is the handler function for SONAPPS_PCI_RESELECTION_REQ
 *          message received from SONMgmtIfH, if the message is
 *          received in an invalid state of PCI or SONAPPS.
 *  \param  p_msg      - Pointer to the message data received (after
 *                       the CSPL header)
 *  \param  p_cspl_hdr - Pointer to the the CSPL header information
 *  \param  p_sonapps_gb_context - Pointer to the global context of
 *                                 SONAPPS
 */
son_buf_retain_status_et
sonapps_pci_fsm_reselection_req_nack
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
);

/*! \fn     son_buf_retain_status_et  sonapps_pci_fsm_enable_disable_req(
 *          son_u8      *p_msg,
 *          STACKAPIHDR *p_cspl_hdr,
 *          son_apps_gb_context_t   *p_sonapps_gb_context
 *          )
 *  \brief  This is the handler function for
 *          SONAPPS_PCI_ENABLE_DISABLE_REQ message received from
 *          SONMgmtIfH in SONAPP_STATE_ACTIVE_state.
 *  \param  p_msg      - Pointer to the message data received (after
 *                       the CSPL header)
 *  \param  p_cspl_hdr - Pointer to the the CSPL header information
 *  \param  p_sonapps_gb_context - Pointer to the global context of
 *                                 SONAPPS
 */
son_buf_retain_status_et
sonapps_pci_fsm_enable_disable_req
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
);

/*! \fn     son_buf_retain_status_et sonapps_pci_fsm_reg_dereg_res(
 *          son_u8      *p_msg,
 *          STACKAPIHDR *p_cspl_hdr,
 *          son_apps_gb_context_t   *p_sonapps_gb_context
 *          )
 *  \brief  This is the handler function for SONAPPS_REG_DEREG_RES
 *          message received from SONANR in
 *          SONAPP_STATE_ACTIVE_state.
 *  \param  p_msg      - Pointer to the message data received (after
 *                       the CSPL header)
 *  \param  p_cspl_hdr - Pointer to the the CSPL header information
 *  \param  p_sonapps_gb_context - Pointer to the global context of
 *                                 SONAPPS
 */
son_buf_retain_status_et
sonapps_pci_fsm_reg_dereg_res
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
);

/*! \fn     son_buf_retain_status_et sonapps_pci_fsm_reselection_req(
 *          son_u8      *p_msg,
 *          STACKAPIHDR *p_cspl_hdr,
 *          son_apps_gb_context_t   *p_sonapps_gb_context
 *          )
 *  \brief  This is the handler function for SONAPPS_PCI_RESELECTION_REQ
 *          message received from SONMgmtIfH in
 *          SONAPP_STATE_ACTIVE_state.
 *  \param  p_msg      - Pointer to the message data received (after
 *                       the CSPL header)
 *  \param  p_cspl_hdr - Pointer to the the CSPL header information
 *  \param  p_sonapps_gb_context - Pointer to the global context of
 *                                 SONAPPS
 */
son_buf_retain_status_et
sonapps_pci_fsm_reselection_req
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
);

/*! \fn     son_buf_retain_status_et sonapps_pci_fsm_conflict_confusion_ind(
 *          son_u8      *p_msg,
 *          STACKAPIHDR *p_cspl_hdr,
 *          son_apps_gb_context_t   *p_sonapps_gb_context
 *          )
 *  \brief  This is the handler function for
 *          SONANR_PCI_CONFLICT_CONFUSION_IND message received from
 *          SONANR in SONAPP_STATE_ACTIVE_state.
 *  \param  p_msg      - Pointer to the message data received (after
 *                       the CSPL header)
 *  \param  p_cspl_hdr - Pointer to the the CSPL header information
 *  \param  p_sonapps_gb_context - Pointer to the global context of
 *                                 SONAPPS
 */
son_buf_retain_status_et
sonapps_pci_fsm_conflict_confusion_ind
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
);

/*! \fn     son_buf_retain_status_et sonapps_pci_fsm_in_use_list_res(
 *          son_u8      *p_msg,
 *          STACKAPIHDR *p_cspl_hdr,
 *          son_apps_gb_context_t   *p_sonapps_gb_context
 *          )
 *  \brief  This is the handler function for SONANR_PCI_IN_USE_LIST_RES
 *          message received from SONANR in SONAPP_STATE_ACTIVE_state.
 *  \param  p_msg      - Pointer to the message data received (after
 *                       the CSPL header)
 *  \param  p_cspl_hdr - Pointer to the the CSPL header information
 *  \param  p_sonapps_gb_context - Pointer to the global context of
 *                                 SONAPPS
 */
son_buf_retain_status_et
sonapps_pci_fsm_in_use_list_res
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
);

/*! \fn     son_buf_retain_status_et sonapps_pci_fsm_event_handler(
 *          son_u8      *p_msg,
 *          STACKAPIHDR *p_cspl_hdr,
 *          son_apps_gb_context_t   *p_sonapps_gb_context
 *          )
 *  \brief  This is the handler function for messages received for
 *          PCI feature in SONAPP_STATE_ACTIVE state and call the
 *          respective function handlers for various messages received
 *          depending upon the current state of PCI feature.
 *  \param  p_msg      - Pointer to the message data received (after
 *                       the CSPL header)
 *  \param  p_cspl_hdr - Pointer to the the CSPL header information
 *  \param  p_sonapps_gb_context - Pointer to the global context of
 *                                 SONAPPS
 */
son_buf_retain_status_et
sonapps_pci_fsm_event_handler
(
    son_u8      *p_msg,
    STACKAPIHDR *p_cspl_hdr,
    son_apps_gb_context_t   *p_sonapps_gb_context
);

/*! \fn  son_buf_retain_status_et  sonapps_pci_scan_res_handler(
 *       son_nmm_start_scan_res_t   *p_nmm_start_scan_res,
 *       son_apps_gb_context_t      *p_sonapps_gb_context
 *   )
 *  \brief  This is the handler function for SONNMM_START_SCAN_RES
 *          message received from SONNMM in SONAPP_STATE_ACTIVE_state
 *          for the PCI feature.
 *  \param  p_nmm_start_scan_res - Pointer to the NW scan response
 *                                 received from NMM(after the CSPL
 *                                 header)
 *  \param  p_sonapps_gb_context - Pointer to the global context of
 *                                 SONAPPS
 */
son_buf_retain_status_et
sonapps_pci_scan_res_handler
(
    son_nmm_start_scan_res_t   *p_nmm_start_scan_res,
    son_apps_gb_context_t      *p_sonapps_gb_context
);

#endif /* _SON_APPS_PCI_H_ */

/* REL1.1 CHANGES : ENDS */
