/*! \file son_mro_fsm.h
 *  \brief This file contains the enumerations and function prototypes specific to MRO module global finite state machine.
 *  \date  Dec, 2012
 *  \author Shilpi
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_MRO_FSM_H_
#define _SON_MRO_FSM_H_

/*! \headerfile son_defines.h <>
 */
#include <son_defines.h>

/*! \headerfile son_utils.h <>
 */
#include <son_utils.h>

/*! \enum son_mro_index_et
 *  \brief An enum that specifies the index value in SONMRO global FSM
 */
typedef enum
{
    MRO_IDX_BASE,

    MRO_IDX_SMIF_EVENT_INIT_CONFIG_REQ = MRO_IDX_BASE,
    MRO_IDX_SMIF_EVENT_SET_LOG_LEVEL_CMD,
    MRO_IDX_SMIF_EVENT_LOG_ENABLE_DISABLE_CMD,
    MRO_IDX_SMIF_EVENT_CELL_INFO_IND,
    MRO_IDX_SMIF_EVENT_SHUTDOWN_REQ,

    MRO_IDX_EVENT_MRO_ENABLE_REQ,
    MRO_IDX_EVENT_MRO_DISABLE_REQ,
    MRO_IDX_EVENT_MRO_MODIFY_CONFIG_PARAM_REQ,

    MRO_IDX_RRM_EVENT_REGISTER_RESP,
    MRO_IDX_RRM_EVENT_DEREGISTER_RESP,
    MRO_IDX_RRM_HO_FAILURE_REPORT,
    MRO_IDX_RRM_HO_ATTEMPTS,
    MRO_IDX_RRM_EVENT_CELL_STATE_CHANGE_IND,

    MRO_IDX_SONANR_CELL_ANR_MODE_IND,

    MRO_IDX_RRC_HO_REPORT_IND,

    MRO_IDX_MLB_NBR_CELL_STATUS_IND,
    
    /* Rel 3.0 Cell delete support changes Start */
    MRO_IDX_SMIF_CELL_DELETE_REQ,
    /* Rel 3.0 Cell delete support changes Stop */
    /* SPR-13251 Fix Starts */
    MRO_IDX_SMIF_GET_LOG_LEVEL_REQ,
    /* SPR-13251 Fix Ends */
    MRO_NUM_OF_EVENTS,

    MRO_IDX_NOT_AVAILABLE  = 0xFF
}son_mro_index_et;

/*! \fn son_buf_retain_status_et son_mro_process_event(son_void_t *p_msg)
 *  \brief This function is used to invoke the appropriate event handlers based
 *  on the various messages received by the MRO module in different MRO FSM
 *  states.
 *  \param p_msg            Pointer to the message received by MRO module
 */
son_buf_retain_status_et
son_mro_process_event
(
    son_void_t *p_msg
);

/* Rel 3.0 Cell delete support changes Start */
/*! \fn son_void_t mro_pending_res_timer_handler()
 *  \brief This function cleans up Pending Response Structure and sends pending response to OAM on pending response timer expiry.
 */
son_void_t
mro_pending_res_timer_handler
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);
/* Rel 3.0 Cell delete support changes Stop */

#endif /* _SON_MRO_FSM_H_ */
