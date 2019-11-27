/*! \file son_rach_opt_fsm.h
 *  \brief This file contains the enumerations and function prototypes specific to RACH OPT module global finite state machine.
 *  \date  Aug, 2013
 *  \author SON Team
 */
/*!
 *  \copyright {Copyright (c) 2013, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_RACH_OPT_FSM_H_
#define _SON_RACH_OPT_FSM_H_

/*! \headerfile son_defines.h <>
 */
#include <son_defines.h>

/*! \headerfile son_utils.h <>
 */
#include <son_utils.h>

/*! \enum son_rach_opt_index_et
 *  \brief An enum that specifies the index value in SON RACH OPT global FSM
 */
typedef enum
{
    RACH_OPT_IDX_BASE,

    RACH_OPT_IDX_SMIF_EVENT_INIT_CONFIG_REQ = RACH_OPT_IDX_BASE,
    RACH_OPT_IDX_SMIF_EVENT_SET_LOG_LEVEL_CMD,
    RACH_OPT_IDX_SMIF_EVENT_LOG_ENABLE_DISABLE_CMD,
    RACH_OPT_IDX_SMIF_EVENT_CELL_INFO_IND,
    RACH_OPT_IDX_SMIF_EVENT_SHUTDOWN_REQ,

    RACH_OPT_IDX_SMIF_EVENT_RACH_OPT_ENABLE_REQ,
    RACH_OPT_IDX_SMIF_EVENT_RACH_OPT_DISABLE_REQ,
    RACH_OPT_IDX_SMIF_EVENT_RACH_OPT_MODIFY_CONFIG_PARAM_REQ,
    RACH_OPT_IDX_SMIF_EVENT_RACH_OPT_MODIFY_RACH_OPTMODE_REQ,

    RACH_OPT_IDX_RRM_EVENT_RACH_CONFIG_RESP,
    RACH_OPT_IDX_RRM_EVENT_UE_INFO_IND,
    RACH_OPT_IDX_RRM_EVENT_L2_IND,
    RACH_OPT_IDX_RRM_EVENT_CELL_STATE_CHANGE_IND,

    RACH_OPT_IDX_SONANR_CELL_ANR_MODE_IND,
    /* Rel 3.0 Cell delete support changes Start */
    RACH_OPT_IDX_SMIF_EVENT_CELL_DELETE_REQ,
    /* Rel 3.0 Cell delete support changes Stop */
    /* SPR-13251 Fix Starts */
    RACH_OPT_IDX_SMIF_EVENT_GET_LOG_LEVEL_REQ,
    /* SPR-13251 Fix Ends */
    RACH_OPT_NUM_OF_EVENTS,

    RACH_OPT_IDX_NOT_AVAILABLE  = 0xFF
}son_rach_opt_index_et;

/*! \fn son_buf_retain_status_et son_rach_opt_process_event(son_void_t *p_msg)
 *  \brief This function is used to invoke the appropriate event handlers based
    on the various messages received by the SON RACH OPT module in different FSM
    states.
 *  \param p_msg            Pointer to the message received by RACH OPT module
 */
son_buf_retain_status_et
son_rach_opt_process_event
(
    son_void_t *p_msg
);

/* Rel 3.0 Cell delete support changes Start */
/*! \fn son_void_t rach_opt_pending_res_timer_handler()
 *  \brief This function cleans up Pending Response Structure and sends pending response to OAM on pending response timer expiry.
 */
son_void_t
rach_opt_pending_res_timer_handler
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);
/* Rel 3.0 Cell delete support changes Stop */

#endif
