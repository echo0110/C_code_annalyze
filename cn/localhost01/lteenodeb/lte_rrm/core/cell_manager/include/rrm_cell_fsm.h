/*! \file rrm_cell_fsm.h
 *  \brief This file contains the states and events of a cell.
 *  \date Feburary 29, 2012
 *  \author gur21481
 */
 /*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */


#ifndef _RRM_CELL_FSM_H_
#define _RRM_CELL_FSM_H_

/*! \headerfile rrm_defines.h <>
 */
#include "rrm_defines.h"
/*! \enum rrm_cell_events_et
 *  \brief An enum that specifies the CELL_EVENTS.
 */

typedef enum
{
   RRMCM_RMIF_CONFIG_REQ_EVENT,
   RRC_RRM_CELL_SETUP_RESP_EVENT,
   RRMCM_RMIF_CELL_START_REQ_EVENT,
   RRC_RRM_CELL_START_RESP_EVENT,
   RRMCM_RMIF_RECONFIG_REQ_EVENT,
   RRC_RRM_CELL_RECONFIG_RESP_EVENT,
   RRMCM_RMIF_CELL_STOP_REQ_EVENT,
   RRC_RRM_CELL_STOP_RESP_EVENT,
   RRMCM_RMIF_UPDATE_REQ_EVENT,
   RRC_RRM_CELL_UPDATE_RESP_EVENT,
   RRMCM_RMIF_CELL_DELETE_REQ_EVENT,
   RRC_RRM_CELL_DELETE_RESP_EVENT,
   RRMUEM_CM_DELETE_ALL_UE_CONTEXT_RESP_EVENT,
   RRMCM_RMIF_STOP_ADMISSION_REQ_EVENT,
   RRMCM_RMIF_START_ADMISSION_REQ_EVENT,
   RRMCM_RMIF_CELL_SON_REGISTER_REQ_EVENT,
   RRMCM_RMIF_CELL_SON_DEREGISTER_REQ_EVENT,
   RRMCM_RMIF_SON_CELL_SET_ATTR_REQ_EVENT,
   CELL_PERIODIC_TIMER_EXPIRY,
   RRMUEM_RMIF_RAC_ENABLE_DISABLE_REQ_EVENT,
   RRMCM_RMIF_MEAS_CONFIG_REQ_EVENT,
   RRMCM_RMIF_UPDATED_NRT_INFO_EVENT,
   RRC_RRM_PWS_REQ_EVENT,                     /*ETWS CMAS FEAT START*/
   RRC_RRM_PWS_CNF_EVENT,
   RRC_RRM_KILL_REQ_EVENT,
   RRC_RRM_KILL_CNF_EVENT,
   RRC_RRM_UPDATE_PWS_LIST_REQ_EVENT,
   RRC_RRM_UPDATE_PWS_LIST_CNF_EVENT,        /*ETWS CMAS FEAT END*/
   //RRMCM_RMIF_RADIO_USAGE_THRESHOLD_REQ_EVENT,
   RRM_PERIODIC_RESRC_UPDATE_EVENT,
   RRMCM_RMIF_CHNG_SELF_RPT_TMR_EVENT,
   RRMCM_RMIF_CHNG_NBR_RPT_TMR_EVENT,
   RRMCM_RMIF_CHNG_BOTH_RPT_TMR_EVENT,
   RRC_RRM_MLB_CELL_RECONFIG_REQ_EVENT,
   RRC_RRM_MLB_CELL_RECONFIG_RESP_EVENT,
   RRMCM_RMIF_TTT_UPDATE_IND_EVENT,
   RRC_RRM_RIM_INFO_EVENT,
   //RRC_RRM_RIM_INFO_APP_ERR_EVENT,
   /* DYNAMIC ICIC CHANGES START */
   RRM_PERIODIC_ICIC_L1_L2_REPORT_INFO_EVENT,
   RRM_X2AP_ICIC_LOAD_INFO_IND_EVENT,
   RRM_UEM_OR_L2_RESPONSE_EVENT,
   RRM_CELL_RECONFIG_FOR_ICIC_EVENT ,
   /* DYNAMIC ICIC CHANGES END */
   RRMCM_RMIF_STOP_ADMISSION_REQ_FOR_CELL_BLOCK_EVENT,
   RRC_RRM_BLOCK_CELL_RECONFIG_RESP_EVENT,
   /* RRM ES Start */
   /* bug_13882_start */  
   RRMCM_RMIF_START_ADMISSION_REQ_FOR_CELL_UNBLOCK_EVENT,
   /* bug_13882_end */  
   RRM_ES_PERIODIC_RESRC_UPDATE_EVENT,
   /* RRM ES end */
   RRMCM_RMIF_MEAS_CONFIG_OBJ_REMOVE_REQ_EVENT,
   /* RACH_OPTIMIZATION_CHANGES_START */
   RRMCM_RMIF_RACH_INFO_IND_EVENT,
   /* RACH_OPTIMIZATION_CHANGES_END */
   /* CA_Stage3_Change: Start */
   RRMUEM_CM_CELL_INFO_CHANGE_RESP_EVENT,
   /* CA_Stage3_Change: End */
   /* BUG_11648_FIX_START */
   RRMUEM_CM_CELL_STOP_BROADCAST_RESP_EVENT,
   /* BUG_11648_FIX_END */
   /* eICIC_PHASE_1_2_CHANGES_START */
   CELL_PERIODIC_TIMER_FOR_AGGRESSOR_SEL_EVENT,
   /* bug_13882_start */  
   CELL_GUARD_TIMER_FOR_ABS_PATTERN_EVENT,
   CELL_ABS_PATTERN_UPDATE_COLLATION_TIMER_EVENT,
   RRM_UEM_GET_AGGRESSOR_RESP_EVENT,
   /* bug_13882_end */  
   RRM_MAC_EICIC_ABS_RECONFIG_REQ_EVENT,
   /** eICIC_PHASE_1_2_CHANGES_END */
#ifdef LTE_EMBMS_SUPPORTED
   RRMCM_M2AP_SF_INFO_REQ_EVENT,
#endif
   RRMCM_CELL_MAX_EVENTS

}rrm_cell_events_et;


/*! \enum rrm_cell_states_et
 *  \brief An enum that specifies the CELL_STATES.
 */

typedef enum
{
   CELL_STATE_IDLE,                                 /*!< CELL_EVENTS event 0 */
   CELL_STATE_W_FOR_CONFIG_RESP,                    /*!< CELL_EVENTS event 1 */
   CELL_STATE_W_FOR_CELL_START,                     /*!< CELL_EVENTS event 2 */
   CELL_STATE_W_FOR_CELL_START_RESP,                /*!< CELL_EVENTS event 3 */
   CELL_STATE_ACTIVE,                               /*!< CELL_EVENTS event 4 */
   CELL_STATE_W_FOR_CELL_RECONFIG_ONGOING_STOP_RESP,/*!< CELL_EVENTS event 5 */
   CELL_STATE_W_FOR_CELL_RECONFIG_RESP,             /*!< CELL_EVENTS event 6 */
   CELL_STATE_W_FOR_CELL_RECONFIG_ONGOING_RECONFIG_RESP,    /*!< CELL_EVENTS event 7 */
   CELL_STATE_W_FOR_CELL_RECONFIG_ONGOING_START_RESP,      /*!< CELL_EVENTS event 8 */
   CELL_STATE_W_FOR_CELL_DEL,                               /*!< CELL_EVENTS event 9 */
   CELL_STATE_W_FOR_CELL_STOP_RESP,                        /*!< CELL_EVENTS event 10 */
   CELL_STATE_W_FOR_CELL_UPDATE_ONGOING_STOP_RESP,      /*!< CELL_EVENTS event 11 */
   CELL_STATE_W_FOR_CELL_UPDATE_ONGOING_UPDATE_RESP,    /*!< CELL_EVENTS event 12 */
   CELL_STATE_W_FOR_CELL_UPDATE_ONGOING_START_RESP,      /*!< CELL_EVENTS event 13 */
   CELL_STATE_OUT_OF_SERVICE,                              /*!< CELL_EVENTS event 14 */
   CELL_STATE_DELETE_IN_PROGRESS,                           /*!< CELL_EVENTS event 15 */
   CELL_STATE_UEM_DEL,                                      /*!< CELL_EVENTS event 16 */
   CELL_STATE_INACTIVE,                                     /*!< CELL_EVENTS event 17 */
   CELL_STATE_W_FOR_CELL_DEL_RECONFIG_FAILED,              /*!< CELL_EVENTS event 18 */
   CELL_STATE_W_FOR_CELL_RECONFIG_RESP_FOR_NRT,            /*!< CELL_EVENTS event 19 */
   CELL_STATE_PWS_ONGOING,                                 /*!< CELL_EVENTS event 20 *//*ETWS CMAS FEAT*/
   CELL_STATE_MLB_W_FOR_CELL_RECONFIG_RESP,                /*!< CELL_EVENTS event 21 */
   /*  DYNAMIC ICIC CHANGES START  */
   CELL_STATE_UEM_W_FOR_ICIC_ACTION_FRM_UEM_OR_L2,         /*!< CELL_EVENTS event 22 */
   /*  DYNAMIC ICIC CHANGES END  */
   CELL_STATE_BLOCK_W_FOR_CELL_RECONFIG_RESP,              /*!< CELL_EVENTS event 23 */
   CELL_STATE_ALREADY_BLOCKED,                             /*!< CELL_EVENTS event 24 */
   /*Spr-9631 fix Start */
   CELL_STATE_W_FOR_CELL_RECONFIG_RESP_DURING_CELL_STOP,   /*!< CELL_EVENTS event 25 */
   /*Spr-9631 fix Stop */
   /* CA_Stage3_Change: Start */
   CELL_STATE_W_FOR_CELL_INFO_CHANGE_RESP,                 /*!< CELL_EVENTS event 26 */
   /* CA_Stage3_Change: End */
   /* BUG_11648_FIX_START */
   CELL_STATE_W_FOR_CELL_STOP_BROADCAST_RESP,              /*!< CELL_EVENTS event 27 */
   /* BUG_11648_FIX_END */
#ifdef LTE_EMBMS_SUPPORTED
   CELL_STATE_W_FOR_CELL_RECONFIG_RESP_FOR_MBMS,
#endif
   /*SPR 21625 fix start*/
   CELL_STATE_W_FOR_CELL_RECONFIG_RESP_DURING_CELL_BLOCK,   /*!< CELL_EVENTS event 29 */
   /*SPR 21625 fix end*/
   CELL_MAX_STATES                                          /*!< CELL_EVENTS event 30 */
}rrm_cell_states_et;

/*! \enum cell_ongoing_procedure_et 
 *  \brief An enum that specifies the trigger of procedure invoked.
 */
typedef enum
{
   CELLM_CELL_DEL_TRIGGERRED_EXTERNALLY,
   CELLM_CELL_STOP_TRIGGERRED_EXTERNALLY,
   CELLM_PROC_TRIGGERRED_EXTERNALLY_IN_ACTIVE_STATE,
   CELLM_PROC_TRIGGERRED_EXTERNALLY_IN_INACTIVE_STATE,
   CELLM_PROC_TRIGGERRED_EXTERNALLY_IN_OOS_STATE,
   CELLM_PROC_TRIGGERRED_INTERNALLY,
   CELLM_PROC_TRIGGERRED_SET_TO_NONE

}cell_ongoing_procedure_et;

/*! \fn rrm_get_first_list_node(const YLIST *p_list)
 *  \brief get the first linkedlist node 
 *  \param p_list
*/
YLNODE *
rrm_get_first_list_node
(
    const YLIST *p_list
);
/*! \fn rrm_get_next_list_node(const YLNODE *p_node)
 *  \brief get the next node from linkedlist
 *  \param p_node
*/
YLNODE *
rrm_get_next_list_node
(
    const YLNODE *p_node
);

/*! \fn rrm_find_best_match_tx_table(rrm_cell_context_t *p_cell_context)
 *  \brief this function fill the another tx table on the basis of tx matrix and supported tx table
 *  \param p_cell_context
*/
/*rrm_void_t
rrm_find_best_match_tx_table(rrm_cell_context_t    *p_cell_context);*/


/* LTE_RRM_TM_MODE_FEATURE_END*/
#endif /* _RRM_CELL_FSM_H_ */


