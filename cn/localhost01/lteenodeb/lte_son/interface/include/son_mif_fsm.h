/*! \file son_mif_fsm.h
 *  \brief This file contains the states events and definitions for SONMgmtIfH
 *         FSM.
 *  \date January 13, 2012
 *  \author Ritika Chopra
 */

/*!
 *  \copyright {Copyright (c) 2009, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_MIF_FSM_H_
#define _SON_MIF_FSM_H_

/*
 * Header File Includes
 */

/*! \headerfile cspl.h <> */
#include <cspl.h>
/*! \headerfile stacklayer.h <> */
#include <stacklayer.h>
/*! \headerfile son_mif_msg_handler.h <> */
#include <son_mif_msg_handler.h>
/*! \headerfile son_api_defines.h <> */
#include <son_api_defines.h>

/*
 * Extern definitions
 */

/*! \def  SMIF_FSM_STATES_NAMES
 *  \brief Array of String constants storing the FSM states names
 */
extern const S8 *SMIF_FSM_STATES_NAMES[];

/*! \def  SMIF_FSM_EVENTS_NAMES
 *  \brief Array of String constants storing the FSM events names
 */
extern const S8 *SMIF_FSM_EVENTS_NAMES[];

/*! \def   p_son_mif_facility_name
 *  \brief  Pointer variable storing the SMIF name for logging
 */
extern const son_8 *p_son_mif_facility_name;

/*! \def g_mif_log_on_off
 *  \brief  Global variable storing the status (Enabled/Disabled) of
 *          SMIF logging
 */
extern son_oam_log_on_off_et g_mif_log_on_off;

/*
 * Macro definitions
 */

/* SPR-12708 Fix Starts */
/*! \def   SMIF_OAM_INIT_IND_TIMER_DURATION
 *  \brief Timer duration (in sec) for sending SON_OAM_INIT_IND to OAM.
 */
#define SMIF_OAM_INIT_IND_TIMER_DURATION 5
/* SPR-12708 Fix Ends */

/*! \def   SMIF_OAM_INIT_IND_RETRY_COUNT
 *  \brief No of retry counts for sending SON_OAM_INIT_IND to OAM
 */
#define SMIF_OAM_INIT_IND_RETRY_COUNT    5

#define SMIF_FSM_INIT_TRACE(fsm_name, state_name) \
    SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_BRIEF, \
    "[%s] initialised, initial state [%s]", fsm_name, state_name)

#define SMIF_FSM_SET_STATE_TRACE(fsm_name, state_name) \
    SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO, \
    "[%s] State changed to [%s]", fsm_name, state_name)

#define SMIF_FSM_EVENT_TRACE(fsm_name, event_name, state_name) \
    SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO, \
    "### %s: Processing State %s Event %s ###", fsm_name, state_name, event_name)

/*! \decl smif_fsm_name
 *  \brief  Pointer to array used to specify the MIF fsm name
 */
extern const son_8 *smif_fsm_name;

/*! \def    SMIF_FSM_INIT
 *  \brief  Initialise the FSM state
 */
#define SMIF_FSM_INIT(p_ctx) \
    (p_ctx)->current_state = SMIF_NULL; \
    SMIF_FSM_INIT_TRACE(smif_fsm_name, \
    SMIF_FSM_STATES_NAMES[SMIF_NULL]);

/*! \def    SMIF_GET_FSM_STATE
 *  \brief  Return the current state of FSM
 */
#define SMIF_GET_FSM_STATE(gb_ctx) ((gb_ctx)->current_state)

/*! \def    SMIF_SET_FSM_STATE
 *  \brief  Change the state of FSM to the new state
 */

#define SMIF_SET_FSM_STATE(gb_ctx, new_state) \
    (gb_ctx)->current_state = (new_state); \
    SMIF_FSM_SET_STATE_TRACE(smif_fsm_name, \
    SMIF_FSM_STATES_NAMES[new_state]);

/*
 * Enumeration Definitions
 */

/*
 * Types Definitions
 */

/*! \     smif_fsm_ret
 *  \brief      Function Pointer to return a pointer to the current event's
 *              handler function
 *  \param      p_smif_gb_context
 *  \param      void
 */
typedef void (*smif_fsm_ret)
(
    son_mif_gb_context_t *,
    void *
);

/*
 * Function Prototypes
 */

/*! \fn	void son_mif_reset_context(son_mif_gb_context_t *p_smif_gb_context)
 *  \brief  This function sets default values for SMIF global context
 *  \param  *p_smif_gb_context   SON MgmtIfH Global Context
 */

void
son_mif_reset_context
(
    son_mif_gb_context_t *p_smif_gb_context /*SMIF global context*/
);

/*! \fn void son_mif_fsm_process_event(son_mif_gb_context_t *p_smif_gb_context,
 *                                     smif_event_et event,void *p_msg)
 *  \brief  This is entry point function for SON MgmtIfH FSM. This function
 *          calls appropriate function handler based on state and
 *  \param  p_smif_gb_context   SON MgmtIfH Global Context
 *  \param  event               Event ID
 *  \param  p_msg              Message specific info (payload)
 */
void
son_mif_fsm_process_event
(
    son_mif_gb_context_t *p_smif_gb_context,    /* Global Context */
    smif_event_et event,         /* Event ID */
    void *p_msg                 /* Message specific info (payload) */
);

/*! \fn son_mif_send_internal_msg (son_module_id_et,U16,U16,void *)
 *  \brief To send the message to the internal modules of SON
 *  \param dst_module_id        Destination module identifier
 *  \param api_id	        API Id
 *  \param msg_size		Size of message (payload)
 *  \param p_msg		Message specific info (payload)
 */
son_return_et
son_mif_send_internal_msg
(
    son_module_id_et dst_module_id,     /* Destination module identifier */
    U16 api_id,             /* API Id */
    U16 msg_size,           /* Size of message (payload) */
    void *p_msg          /* Message specific info (payload) */
);
/*SPR_19279_START*/
son_return_et
son_mif_send_internal_cell_info_ind_msg
(
    son_module_id_et dst_module_id,     /* Destination module identifier */
    U16 api_id,             /* API Id */
    U16 msg_size,           /* Size of message (payload) */
    void *p_msg,          /* Message specific info (payload) */
    U8 cell_index           /* Cell Instance of Serving Cell */
);
/*SPR_19279_END*/

#endif /* _SON_MIF_FSM_H_ */
