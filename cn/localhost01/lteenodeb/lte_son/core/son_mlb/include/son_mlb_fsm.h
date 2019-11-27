/*! \file son_mlb_fsm.h
 *  \brief This file contains the functions prototype for handling MLB Module
 *         State Machine
 *  \date  Dec, 2012
 *  \author Sajida
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_MLB_FSM_H_
#define _SON_MLB_FSM_H_

/*! \enum mlb_fsm_state_et
 *  \brief An enum that specifies the SON MLB FSM states
 */

#include <son_mlb_ctxt_mgr.h>

#include <rrm_son_intf.h>

/*! \enum mlb_fsm_event_et
 *  \brief An enum that specifies the SON MLB FSM events
 */

typedef enum _mlb_fsm_event_e
{
    SONMLB_INIT_CONFIG_REQ = 0,
    SONMLB_ENABLE_REQ_HAND,
    SONMLB_DISABLE_REQ_HAND,
    SMIF_CELL_INFO_IND_HAND,
    SMIF_LOG_ENABLE_DISABLE_CMD_HAND,
    SMIF_SET_LOG_LEVEL_CMD_HAND,
    RRM_SON_REGISTER_RES_HAND,
    RRM_SON_DEREGISTER_RES_HAND,
    RRM_SON_HO_REPORT_HAND,
    RRM_SON_LOAD_REPORT_HAND,
    RRM_SON_CELL_STATE_CHANGE,
    SONANR_DISABLE_IND_HAND,
    SONMLB_MODIFY_REQ,
    SMIF_SHUTDOWN_REQ_HAND,
    SON_MOBILITY_CHANGE_REQ,
    SON_MOBILITY_CHANGE_FAILURE,
    SON_MOBILITY_CHANGE_ACK,
 /*Rel 3.0 Multi Sector code changes Start*/
    SON_INTERNAL_MOBILITY_CHANGE_REQ,
    SON_INTERNAL_MOBILITY_CHANGE_FAILURE,
    SON_INTERNAL_MOBILITY_CHANGE_ACK,
 /*Rel 3.0 Multi Sector code changes Stop*/
    SONANR_NBR_UPDATE_INFO_HAND,
    SONMLB_OFFSET_CHANGE_REQ_HAND,
    SONMLB_PEER_OFFSET_CHANGE_RESP_HAND,
    /* Rel 3.0 Cell delete support changes Start */
    SMIF_CELL_DELETE_REQ_HAND,
    /* Rel 3.0 Cell delete support changes Stop */
    /* SPR-13251 Fix Starts */
    SMIF_GET_LOG_LEVEL_REQ_HAND,
    /* SPR-13251 Fix Ends */
/* eICIC feature code Starts */
    RRM_SON_EICIC_ENABLE_DISABLE,
/* eICIC feature code Ends */
    MLB_MAX_FSM_EVENT,
    MLB_EVENT_UNDEFINED = 0xFF,
}mlb_fsm_event_et;

typedef son_buf_retain_status_et (*mlb_fsm_fn_ptr_tbl)
(
    son_u8 *p_msg,
    STACKAPIHDR *p_cspl_hdr
);

/*! \fn son_buf_retain_status_et mlb_shutdown_handler (son_u8 *p_msg,STACKAPIHDR *p_cspl_info)
 *  \brief This is the message handler for MLB module and is called
 * when SMIF_SHUTDOWN_REQ message is received.
 *  \param p_msg       Pointer to the message received
 *  \param p_cspl_info Pointer to  the STACKAPIHDR structure
 */
son_buf_retain_status_et
mlb_shutdown_handler
(
    son_u8 *p_msg,
    STACKAPIHDR *p_cspl_hdr
);

/*! \fn son_buf_retain_status_et mlb_init_state_handler (son_u8 *p_msg,STACKAPIHDR *p_cspl_hdr)
 *  \brief This is the message handler for MLb module and is called for
 * any message received in INIT State
 *  \param p_msg       Pointer to the message received
 *  \param p_cspl_hdr  Pointer to  the STACKAPIHDR structure
 */
son_buf_retain_status_et
mlb_init_state_handler
(
    son_u8 *p_msg,
    STACKAPIHDR *p_cspl_hdr
);

son_buf_retain_status_et
mlb_global_enable_req_handler
(
    STACKAPIHDR        *p_cspl_hdr,
    son_u8             *p_msg,
    son_mlb_index_et event_id
);

son_buf_retain_status_et
mlb_global_disable_req_handler
(
    STACKAPIHDR        *p_cspl_hdr,
    son_u8                *p_msg,
    son_mlb_index_et event_id
);

son_void_t
mlb_create_send_generic_nack
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_error_et error_code
);

son_void_t
mlb_create_send_generic_nack_to_mif
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_error_et error_code
);

son_buf_retain_status_et
mlb_sonmlb_modify_req
(
    son_u8         *p_buf,
    STACKAPIHDR    *p_cspl_hdr
);

son_buf_retain_status_et
mlb_global_sonanr_disable_ind_handler
(
    son_u8         *p_buf,
    STACKAPIHDR    *p_cspl_hdr
);

son_void_t
mlb_stop_timer
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

son_u8 *
mlb_create_enable_disable_req_nack
(
    son_u8                  *p_msg,
    son_procedure_code_et api,
    son_u32 length,
    son_error_et error_code
);

son_void_t
mlb_create_send_generic_nack
(
    son_u8          *p_msg,
    STACKAPIHDR     *p_cspl_hdr,
    son_error_et error_code
);

son_intra_rat_global_cell_id_t *
mlb_get_cell_id_from_mif_msgs
(
    son_u16 from,
    son_u16 api,
    son_u8 *p_msg
);

#endif /* ifndef _SON_MLB_FSM_H_ */
