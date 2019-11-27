/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_fsm_engine.h,v 1.2 2010/01/04 16:10:05 ukr15916 Exp $uecc_fsm_engine.h
 ****************************************************************************
 *
 *  File Description : UECC FSM engine functions declaration
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_fsm_engine.h,v $
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.6.3  2009/12/26 12:09:04  gur21006
 * Warnings removed
 *
 * Revision 1.1.6.2  2009/12/08 11:22:51  ukr16032
 * UECC - inspection log items LG_20, LG_23, LG_24, LG_26, LG_27, LG_28, LG_29, LG_34, LG_40, LG_41 fixed
 *
 * Revision 1.1.6.1  2009/11/26 18:33:48  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.4.7  2009/11/24 12:12:51  ukr15916
 * tabs expanded, lineending updated for Unix format
 *
 * Revision 1.1.4.6  2009/11/24 11:32:34  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.4.4.2.1  2009/11/23 16:43:50  ukr15916
 * beautification
 *
 * Revision 1.1.4.4  2009/11/19 09:52:16  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.4.3  2009/11/14 16:29:55  ukr16032
 * UECC UE Capability transfer implemented
 * UECC ICSP - security capability checking implemented
 *
 * Revision 1.1.4.2  2009/11/03 11:43:37  ukr18880
 * UECC FSM engine was updated
 *
 * Revision 1.1.4.1  2009/10/27 09:50:12  ukr15916
 * UECC added to Rel 1.0
 *
 * Revision 1.1.2.6  2009/10/27 07:59:20  ukr18880
 * UECC FSM failure processing was updated
 *
 * Revision 1.1.2.5  2009/10/26 16:14:46  ukr18880
 * UECC FSM failure processing engine initial creation
 *
 * Revision 1.1.2.4  2009/10/26 14:25:05  ukr18880
 * UECC FSM engine update
 *
 * Revision 1.1.2.3  2009/10/26 13:34:05  ukr18880
 * UECC FSM engine modification
 *
 * Revision 1.1.2.2  2009/10/22 09:43:16  ukr18880
 * bug fixing
 *
 * Revision 1.1.2.1  2009/10/22 07:03:19  ukr18880
 * New UECC FSM initial creation
 *19.10.2009    AndriyT -   initial creation
 *
 ****************************************************************************/

#ifndef UECC_FSM_ENGINE_H
#define UECC_FSM_ENGINE_H

/****************************************************************************
 * Includes
 ****************************************************************************/
#include "uecc_ue_ctx.h"
#include "uecc_event_queue.h"

/****************************************************************************
 * Defines
 ****************************************************************************/
#define UECC_FSM_SET_MAIN_STATE(p_ue_context, p_new_state, p_data) \
{\
    uecc_fsm_set_state(p_ue_context,\
        &(p_ue_context->uecc_fsm.p_current_state),p_new_state,p_data);\
}

#define UECC_FSM_SET_CHILD_STATE(p_ue_context, p_new_state, p_data) \
{\
    uecc_fsm_set_state(p_ue_context,\
        &(p_ue_context->uecc_fsm.proc_descriptor.p_current_state),\
        p_new_state, p_data);\
}

/****************************************************************************
 * Public functions definitions
 ****************************************************************************/
/****************************************************************************
 * Function Name  : uecc_fsm_init_engine
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Inits UECC FSM engine
 ****************************************************************************/
void uecc_fsm_init_engine(uecc_ue_context_t*                p_ue_context);

/****************************************************************************
 * Function Name  : uecc_fsm_start_procedure
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  uecc_fsm_control_procedure_descriptor_t* - pointer to
 *                      procedure for activation
 *                  void* p_api_data - pointer on procedure
 *                                     constructor parameters
 * Outputs        : None
 * Returns        : None
 * Description    : Activates specified procedure
 ****************************************************************************/
void uecc_fsm_start_procedure(uecc_ue_context_t*                p_ue_context,
                              uecc_fsm_procedure_t*       p_procedure,
                              void*                             p_api_data);

/****************************************************************************
 * Function Name  : uecc_fsm_finish_active_procedure
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Finishes current active procedure
 ****************************************************************************/
void uecc_fsm_finish_active_procedure(uecc_ue_context_t*    p_ue_context);

/****************************************************************************
 * Function Name  : uecc_fsm_store_ongoing_active_procedure
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Stores ongoing active procedure and state
 ****************************************************************************/
void uecc_fsm_store_ongoing_active_procedure(uecc_ue_context_t*    p_ue_context);

/****************************************************************************
 * Function Name  : uecc_fsm_start_next_activity
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Starts next activity from procedure activities list
 ****************************************************************************/
rrc_bool_et uecc_fsm_start_next_activity(uecc_ue_context_t* p_ue_context);

/****************************************************************************
 * Function Name  : uecc_fsm_finish_activity_success
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void* - pointer to data
 * Outputs        : None
 * Returns        : None
 * Description    : Finishes current activity of current active procedure
 ****************************************************************************/
void uecc_fsm_finish_activity_success(uecc_ue_context_t*    p_ue_context,
                                      void*                 p_data);

/****************************************************************************
 * Function Name  : uecc_fsm_finish_activity_failure
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void* - pointer to data
 * Outputs        : None
 * Returns        : None
 * Description    : Finishes current activity of current active procedure
 ****************************************************************************/
void uecc_fsm_finish_activity_failure(uecc_ue_context_t*    p_ue_context,
                                      void*                 p_data);

/****************************************************************************
 * Function Name  : uecc_fsm_set_state
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  uecc_fsm_activity_state_t** - double pointer to current
 *                      state
 *                  uecc_fsm_activity_state_t* - pointer to new state
 *                  void* - pointer to data
 * Outputs        : uecc_fsm_activity_state_t** - new state
 * Returns        : None
 * Description    : Unified function for state switching in Main FSM and
 *                      activity FSM
 ****************************************************************************/
void uecc_fsm_set_state(uecc_ue_context_t*          p_ue_context,
                        uecc_fsm_activity_state_t** p_current_state,
                        uecc_fsm_activity_state_t*  p_new_state,
                        void*                       p_data);

/****************************************************************************
 * Function Name  : uecc_fsm_push_init_msg
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void* - pointer to init data
 * Outputs        : None
 * Returns        : None
 * Description    : Pushes initial message to queue
 ****************************************************************************/
void uecc_fsm_push_init_msg(uecc_ue_context_t*          p_ue_context,
                            void*                       p_data);

/****************************************************************************
 * Function Name  : uecc_fsm_process_message
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  void* - pointer to message
 * Outputs        : None
 * Returns        : None
 * Description    : Processes message
 ****************************************************************************/
void uecc_fsm_process_event(uecc_ue_context_t*          p_ue_context,
                            uecc_fsm_event_et           event_id,
                            void*                       p_msg);

/****************************************************************************
 * Function Name  : uecc_fsm_call_handler
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  const uecc_fsm_handler_record_t*  -pointer to handlers table
 *                  void* - pointer to message
 * Outputs        : RRC_TRUE/RRC_FALSE
 * Returns        : rrc_bool_et
 * Description    : Processes message
 ****************************************************************************/
rrc_bool_et uecc_fsm_call_handler(uecc_ue_context_t*               p_ue_context,
                                  uecc_fsm_handler_record_t* p_mht,
                                  uecc_fsm_event_et                event_id,
                                  void*                            p_data);

/****************************************************************************
 * Function Name  : uecc_fsm_start_failure_processing
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  U16 const*  - Pointer on the current failure actions list
 * Outputs        : None
 * Returns        : None
 * Description    : Activates failure processing
 ****************************************************************************/
void uecc_fsm_start_failure_processing(
    uecc_ue_context_t*              p_ue_context,
    const uecc_fsm_fail_handler_f *  p_failure_actions_list
);

/****************************************************************************
 * Function Name  : uecc_fsm_process_failure
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : None
 * Returns        : None
 * Description    : Executes actions from failure actions list
 *                      for proper failure handling
 ****************************************************************************/
void uecc_fsm_process_failure(
    uecc_ue_context_t*          p_ue_context
);


/****************************************************************************
 * Function Name  : rrc_generate_unexpected_rrc_msg_event
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 *                  event_id
 * Outputs        : None
 * Returns        : None
 * Description    : Determines the message_id of a RRC message received unexpectedly
 ****************************************************************************/
void rrc_generate_unexpected_rrc_msg_event(
        uecc_ue_context_t*          p_ue_context,
        uecc_fsm_event_et           event_id
);

/*SPR_18241_START*/
/****************************************************************************
 * Function Name  : uecc_fsm_is_procedure_active
 * Inputs         : uecc_ue_context_t* - pointer to UE context
 * Outputs        : RRC_TRUE/RRC_FALSE
 * Returns        : rrc_bool_et
 * Description    : Checks is any procedure is active in UECC Main FSM
 ****************************************************************************/
rrc_bool_et
    uecc_fsm_is_procedure_active(uecc_ue_context_t* p_ue_context);
/*SPR_18241_END*/
#endif /* UECC_FSM_ENGINE_H */


