/*! \file son_x2_fsm.h
 *  \brief This file contains the enumerations and function prototypes specific to X2 module global finite state machine.
 *  \date  Aug, 2012
 *  \author Shilpi
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_X2_FSM_H_
#define _SON_X2_FSM_H_

/*! \headerfile son_defines.h <>
 */
#include <son_defines.h>

/*! \headerfile son_utils.h <>
 */
#include <son_utils.h>

/*! \enum son_x2_index_et
 *  \brief An enum that specifies the index value in SONX2 global FSM
 */
typedef enum
{
    X2_IDX_BASE,

    X2_IDX_SMIF_EVENT_INIT_CONFIG_REQ = X2_IDX_BASE,
    X2_IDX_SMIF_EVENT_SET_LOG_LEVEL_CMD,
    X2_IDX_SMIF_EVENT_SHUTDOWN_REQ,

    X2_IDX_SONANR_EVENT_ADD_ENB_REQ,
    X2_IDX_SONANR_EVENT_DELETE_ENB_REQ,
    X2_IDX_SONANR_EVENT_NRT_UPDATE_IND,

    X2_IDX_SONES_EVENT_CELL_SWITCH_ON_OFF_IND,
    X2_IDX_SONES_EVENT_CELL_ACTIVATION_RES,

    X2_IDX_RRC_EVENT_ADD_ENB_RES,
    X2_IDX_RRC_EVENT_LINK_UP_IND,
    X2_IDX_RRC_EVENT_LINK_DOWN_IND,
    X2_IDX_RRC_EVENT_DELETE_ENB_RES,
    X2_IDX_RRC_EVENT_CELL_ACTIVATION_REQ,
    X2_IDX_RRC_EVENT_ENB_CONFIG_UPDATE_RES,
    X2_IDX_RRC_EVENT_ENB_CONFIG_UPDATE_IND,
    X2_IDX_RRC_EVENT_ENB_RESET_RES,
#ifdef SON_MRO_FEATURE
    X2_IDX_RRC_EVENT_HO_REPORT_IND,
#endif
    X2_IDX_SONMLB_EVENT_MOB_SETT_CHANGE_REQ,
    X2_IDX_SONMLB_EVENT_MOB_CHANGE_RESP,
    X2_IDX_SONMLB_EVENT_MOB_CHANGE_REQ,
    X2_IDX_SONMLB_EVENT_MOB_CHANGE_ACK,
    X2_IDX_SONMLB_EVENT_MOB_CHANGE_FAIL,
    /* SPR-10331 Fix Starts */
    X2_IDX_SMIF_EVENT_GU_GROUP_ID_CONFIG_REQ,
    /* SPR-10331 Fix Ends */
/* SPR_19619 start */
    X2_IDX_SONES_EVENT_PEER_CELL_ACTIVATION_REQ,
    X2_IDX_RRC_EVENT_CELL_ACTIVATION_RESP,
/* SPR_19619 stop */
	X2_IDX_RRC_EVENT_ENDC_CONFIG_UPDATE_EN_GNB_IND,

    X2_NUM_OF_EVENTS,

    X2_IDX_NOT_AVAILABLE  = 0xFF
}son_x2_index_et;

/*! \fn son_buf_retain_status_et son_x2_process_event(son_void_t *p_msg)
 *  \brief This function is used to invoke the appropriate event handlers based
 *  on the various messages received by the X2 module in different X2 FSM
 *  states.
 *  \param p_msg            Pointer to the message received by X2 module
 */
son_buf_retain_status_et
son_x2_process_event
(
    son_void_t *p_msg
);

#endif /* _SON_X2_FSM_H_ */
