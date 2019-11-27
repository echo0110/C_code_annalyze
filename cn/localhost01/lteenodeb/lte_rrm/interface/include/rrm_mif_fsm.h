/*! \file rrm_mif_fsm.h
 *  \brief This file contains the states events and definitions for RRMMgmtIfH FSM.
 *  \date January 23, 2012 
 *  \author Hargovind Sharma
*/
/*!
 *  \copyright {Copyright (c) 2009, Aricent Inc. All Rights Reserved}
 */

#ifndef _RRM_MIF_FSM_H_
#define _RRM_MIF_FSM_H_

/*
 * Header File Includes
 */

/*! \headerfile rrm_defines.h <>
   */
#include "rrm_defines.h"

/*! \headerfile rrm_utils.h <>
   */
#include "rrm_utils.h"

/*! \headerfile rrm_cell_rmim_intf.h <>
   */
#include "rrm_cell_rrmim_intf.h"

/*! \headerfile rrm_oam_intf.h <>
   */
#include "rrm_oam_intf.h"

/*! \headerfile rrm_uem_cellm_intf.h <>
     */
#include "rrm_uem_cellm_intf.h"

/*! \headerfile rrm_uem_rrmim_intf.h <>
     */
#include "rrm_uem_rrmim_intf.h"

/*! \headerfile rrm_oam_composer.h <>
     */
#include "rrm_oam_composer.h"

/*! \headerfile rrm_oam_parser.h <>
     */
#include "rrm_oam_parser.h"


/*
 * Extern definitions
 */

/*! \def   p_g_rrm_mif_facility_name
 *  \brief  Pointer variable storing the RRM MIF name for logging
 */
extern S8 *p_g_rrm_mif_facility_name;


/*! \def g_mif_log_on_off
 *  \brief  Global variable storing the status (Enabled/Disabled) of
 *          RRM MIF logging
 */
extern rrm_oam_log_on_off_et g_mif_log_on_off;


/*
 * Enumeration Definitions
 */


/*! \enum   rmif_super_state_et  
 *  \brief  An enum denoting states for RRM MgmtIfH FSM
 */
typedef enum
{
    RMIF_STATE_IDLE,                                    /*!< Idle state */
    RMIF_STATE_INIT,                                    /*!< Initialized state */
    RMIF_STATE_INIT_CONFIG_ONGOING,                     /*!< Init Config Ongoing */
    RMIF_STATE_ACTIVE,                                  /*!< Active state */
    RMIF_STATE_SHUTDOWN_ONGOING,                        /*!< Shutdown in progress */
    RMIF_STATE_RESUME_ONGOING,                          /*!< Resume in progress */
    RMIF_NUM_OF_STATES                                  /*!< Number of states */
}rmif_super_state_et;


/*! \enum   rmif_event_et
 *  \brief  An enum denoting Events for MIF FSM
 */
typedef enum
{
    MIF_FSM_EVENT_CELL_CONFIG_REQUEST,                  /*!< MIF_FSM_EVENT_CELL_CONFIG_REQUEST */
    MIF_FSM_EVENT_CELL_RECONFIG_REQ,                    /*!< MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
    MIF_FSM_EVENT_CELL_DELETE_REQ,                      /*!< MIF_FSM_EVENT_CELL_DELETE_REQ */
    MIF_FSM_EVENT_CELL_START_REQ,                       /*!< MIF_FSM_EVENT_CELL_START_REQ */
    MIF_FSM_EVENT_CELL_STOP_REQ,                        /*!< MIF_FSM_EVENT_CELL_STOP_REQ */
    MIF_FSM_EVENT_CELL_UPDATE_REQ,                    /*!< MIF_FSM_EVENT_CELL_UPDATE_REQ*/
    MIF_FSM_EVENT_CELL_SHUTDOWN_REQ,                    /*!< MIF_FSM_EVENT_CELL_SHUTDOWN_REQ */
    MIF_FSM_EVENT_CELL_SWITCH_ON_REQ,                   /*!< MIF_FSM_EVENT_CELL_SWITCH_ON_REQ */
    MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ,                  /*!< MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ */
    MIF_FSM_EVENT_CELL_CONFIG_RESP,                     /*!< MIF_FSM_EVENT_CELL_CONFIG_RESP */
    MIF_FSM_EVENT_CELL_RECONFIG_RESP,                   /*!< MIF_FSM_EVENT_CELL_RECONFIG_RESP */
    MIF_FSM_EVENT_CELL_DELETE_RESP,                     /*!< MIF_FSM_EVENT_CELL_DELETE_RESP */
    MIF_FSM_EVENT_CELL_START_RESP,                      /*!< MIF_FSM_EVENT_CELL_START_RESP */
    MIF_FSM_EVENT_CELL_STOP_RESP,                       /*!< MIF_FSM_EVENT_CELL_STOP_RESP */
    MIF_FSM_EVENT_CELL_UPDATE_RESP,                    /*!< MIF_FSM_EVENT_CELL_UPDATE_RESP*/
    MIF_FSM_EVENT_START_ADMISSION_RESP,                 /*!< MIF_FSM_EVENT_START_ADMISSION_RESP */
    MIF_FSM_EVENT_STOP_ADMISSION_RESP,                  /*!< MIF_FSM_EVENT_STOP_ADMISSION_RESP */
    MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP,           /*!< MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP */
    MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY,     /*!< MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY */
    MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ,               /*!< MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ */
    MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP,              /*!< MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP */
    MIF_FSM_EVENT_CELL_RESUME_REQ,                      /*!< MIF_FSM_EVENT_CELL_RESUME_REQ */
    MIF_FSM_EVENT_BLOCK_CELL_REQ,                      /*!<MIF_FSM_EVENT_BLOCK_CELL_REQ */
    MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP,    /*!<MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP */
    MIF_FSM_EVENT_UNBLOCK_CELL_CMD,                     /*!< MIF_FSM_EVENT_UNBLOCK_CELL_CMD */
    MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ,
    MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP,                /*!< MIF_FSM_EVENT_RADIO_USAGE_RESP */
    MIF_FSM_EVENT_NO_ACTIVE_UE_IND,                     /*!< MIF_FSM_EVENT_NO_ACTIVE_UE_IND */
    MIF_FSM_EVENT_CELL_PLATFORM_IND,                    /*!<MIF_FSM_EVENT_CELL_PLATFORM_IND */
    MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ,                  /*!<MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
    MIF_FSM_EVENT_CELL_GET_KPI_REQ,                     /*!<MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
    MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP,                 /*!<MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
    MIF_FSM_EVENT_CELL_GET_KPI_RESP,                    /*!< MIF_FSM_EVENT_CELL_GET_KPI_RESP*/
    MIF_FSM_EVENT_CELL_RESUME_RESP,                     /*!< MIF_FSM_EVENT_CELL_RESUME_RESP */
    MIF_FSM_EVENT_MEAS_CONFIG_REQ,                      /*!< MIF_FSM_EVENT_MEAS_CONFIG_REQ */
    MIF_FSM_NUM_OF_EVENTS,                              /*!< MIF_FSM_NUM_OF_EVENTS */
    MIF_FSM_INVALID_EVENT                               /*!< MIF_FSM_INVALID_EVENT */
} rmif_event_et;

/*! \enum  rmif_state_et 
 *  \brief  An enum denoting Events for MIF FSM
 */
typedef enum
{
    MIF_FSM_STATE_INITIAL,                              /*!< MIF_FSM_STATE_INITIAL*/
    MIF_FSM_STATE_W_FOR_CELL_CONFIG_RESP,               /*!< MIF_FSM_STATE_W_FOR_CELL_CONFIG_RESP */
    MIF_FSM_STATE_W_FOR_CELL_RECONFIG_RESP,             /*!< MIF_FSM_STATE_W_FOR_CELL_RECONFIG_RESP */
    MIF_FSM_STATE_W_FOR_CELL_DELETE_RESP,               /*!< MIF_FSM_STATE_W_FOR_CELL_DELETE_RESP */
    MIF_FSM_STATE_W_FOR_CELL_START_RESP,                /*!< MIF_FSM_STATE_W_FOR_CELL_START_RESP */
    MIF_FSM_STATE_W_FOR_CELL_STOP_RESP,                 /*!< MIF_FSM_STATE_W_FOR_CELL_STOP_RESP */
    MIF_FSM_STATE_W_FOR_CELL_UPDATE_RESP,               /*!< MIF_FSM_STATE_W_FOR_CELL_UPDATE_RESP */
    MIF_FSM_STATE_W_FOR_START_ADMISSION_RESP,           /*!< MIF_FSM_STATE_W_FOR_START_ADMISSION_RESP */
    MIF_FSM_STATE_W_FOR_STOP_ADMISSION_RESP,            /*!< MIF_FSM_STATE_W_FOR_STOP_ADMISSION_RESP */
    MIF_FSM_STATE_W_FOR_SWITCH_ON,                      /*!< MIF_FSM_STATE_W_FOR_SWITCH_ON */
    MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP,    /*!< MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP */
    MIF_FSM_STATE_ACTIVE,                               /*!< MIF_FSM_STATE_ACTIVE */
    MIF_FSM_STATE_W_FOR_SHUTDOWN_TIMER_EXPIRY,          /*!< MIF_FSM_STATE_W_FOR_SHUTDOWN_TIMER_EXPIRY */
    MIF_FSM_STATE_W_FOR_RAC_ENABLE_DISABLE_RESP,        /*!< MIF_FSM_STATE_W_FOR_RAC_ENABLE_DISABLE_RESP */
    MIF_FSM_STATE_CELL_ALREADY_BLOCKED,                   /*!<MIF_FSM_STATE_CELL_ALREADY_BLOCKED */
    MIF_FSM_STATE_W_FOR_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP, /*!<MIF_FSM_STATE_W_FOR_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP */
    MIF_FSM_STATE_W_FOR_CELL_RESUME_RESP,               /*!< MIF_FSM_STATE_W_FOR_CELL_RESUME_RESP */
    MIF_FSM_NUM_OF_STATES,
    MIF_FSM_INVALID_STATE
} rmif_state_et;


/*! \enum  mif_fsm_type_et 
   *  \brief  An enum denoting Timer Types
*/
typedef enum mif_fsm_type_et
{
   NULL_FSM,
   MIF_FSM,
   MAX_FSM_TYPE
}mif_fsm_type;

/*SPR_15766 FIX START*/
typedef enum
{
    RRM_CELL_INIT = 0,
    RRM_CELL_START_ONGOING,
    RRM_CELL_STARTED,
    RRM_CELL_STOP_ONGOING,
    RRM_CELL_STOPPED,
    RRM_CELL_UNKNOWN_STATE
}rrm_cell_state_et;
/*SPR_15766 FIX END*/

/*! \ mif_return_value 
 *  \brief  Structure for return data 
 *  \param node YLNODE
 *  \param src_fsm Variable denoting source FSm
 *  \param fsm_index Index of FSM
 *  \param event_id Denotes event ID
 *  \param p_msg_info Pointer to msg
 */
typedef struct mif_return_value_et
{
    YLNODE      node;
    mif_fsm_type   src_fsm;
    mif_fsm_type   fsm_index;
    U16      event_id;
    U8       *p_msg_info;
}mif_return_value;



/*! \def    RMIF_RRMCM_INIT_RECD
 *  \brief  To set flag on receipt of msg
 */
#define RMIF_RRMCM_INIT_RECD           	0x01   /** 0 0 0 0 0 0 0 1 */

/*! \def    RMIF_RRMUEM_INIT_RECD
 *  \brief  To set flag on receipt of msg
 */
#define RMIF_RRMUEM_INIT_RECD   	0x02   /** 0 0 0 0 0 0 1 0 */

/*! \def   RMIF_RRMCM_INIT_CONFIG_SUCCESS_RESP_RECD 
 *  \brief  To set flag on receipt of msg
 */
#define RMIF_RRMCM_INIT_CONFIG_SUCCESS_RESP_RECD     	0x01   /** 0 0 0 0 0 0 0 1 */

/*! \def   RMIF_RRMCM_INIT_CONFIG_FAILURE_RESP_RECD 
 *  \brief  To set flag on receipt of msg
 */
#define RMIF_RRMCM_INIT_CONFIG_FAILURE_RESP_RECD     	0x02   /** 0 0 0 0 0 0 1 0 */

/*! \def   RMIF_RRMUEM_INIT_CONFIG_SUCCESS_RESP_RECD 
 *  \brief  To set flag on receipt of msg
 */
#define RMIF_RRMUEM_INIT_CONFIG_SUCCESS_RESP_RECD    	0x04   /** 0 0 0 0 0 1 0 0 */

/*! \def   RMIF_RRMUEM_INIT_CONFIG_FAILURE_RESP_RECD 
 *  \brief  To set flag on receipt of msg
 */
#define RMIF_RRMUEM_INIT_CONFIG_FAILURE_RESP_RECD    	0x08   /** 0 0 0 0 1 0 0 0 */

/*! \def   RMIF_RRMCM_GENERIC_SUCCESS_RESP_RECD 
 *  \brief  To set flag on receipt of msg
 */
#define RMIF_RRMCM_GENERIC_SUCCESS_RESP_RECD            0x01   /** 0 0 0 0 0 0 0 1 */

/*! \def   RMIF_RRMCM_GENERIC_FAILURE_RESP_RECD 
 *  \brief  To set flag on receipt of msg
 */
#define RMIF_RRMCM_GENERIC_FAILURE_RESP_RECD            0x02   /** 0 0 0 0 0 0 1 0 */

/*! \def   RMIF_RRMUEM_GENERIC_SUCCESS_RESP_RECD 
 *  \brief  To set flag on receipt of msg
 */
#define RMIF_RRMUEM_GENERIC_SUCCESS_RESP_RECD           0x04   /** 0 0 0 0 0 1 0 0 */

/*! \def   RMIF_RRMUEM_GENERIC_FAILURE_RESP_RECD 
 *  \brief  To set flag on receipt of msg
 */
#define RMIF_RRMUEM_GENERIC_FAILURE_RESP_RECD           0x08   /** 0 0 0 0 1 0 0 0 */

/*! \def   RMIF_GLOBAL_CELL_ID_PRESENT
 *  \brief To test whether the GLOBAL_CELL_ID_PRESENT_BIT is set 
 *         in the bitmask field
 */
#define RMIF_GLOBAL_CELL_ID_PRESENT(X)    \
            (((X)->bitmask) & RRM_OAM_CELL_ID_PRESENT) == 1

/*! \def   RMIF_SET_GLOBAL_CELL_ID_FLAG
 *  \brief To set the GLOBAL_CELL_ID_PRESENT_BIT  
 *         in the bitmask field
 */
#define RMIF_SET_GLOBAL_CELL_ID_FLAG(X)    \
            (X->bitmask |= RRM_OAM_CELL_ID_PRESENT)

/*! \def   RMIF_SET_CELL_REQUEST_SENT
 *  \brief To set the Cell request bit 
 *         in the bs_cellmap
 */
#define RMIF_SET_CELL_REQUEST_SENT(X,Y)    \
            (RRM_BIT_SET(X->bs_cellmap,Y))

/*! \def   RMIF_CLEAR_CELL_REQUEST_SENT
 *  \brief To clear the Cell request bit 
 *         in the bs_cellmap
 */
#define RMIF_CLEAR_CELL_REQUEST_SENT(X,Y)  \
            (RRM_BIT_UNSET(X->bs_cellmap,Y))


/*! \def  RRM_MIF_FSM_GET_FSM_INFO 
 *  \brief To get FSM context related information
 */
#define RRM_MIF_FSM_GET_FSM_INFO(P_MIFcONTEXT)  \
        (rrm_mif_fsm_context_info *)((P_MIFcONTEXT)->fcb[MIF_FSM].p_fsm_specific_info)

/*! \def  RRM_MIF_FSM_GET_STATE 
 *  \brief To get FSM state related information
 */
#define RRM_MIF_FSM_GET_STATE(P_MIFcONTEXT) \
                ((P_MIFcONTEXT)->fcb[MIF_FSM].fsmState)

/*! \def  RRM_MIF_FSM_SET_STATE
 *  \brief To set the state of FSM with current state
 */
#define RRM_MIF_FSM_SET_STATE(P_MIFcONTEXT, STATE)  \
                 (P_MIFcONTEXT)->fcb[MIF_FSM].fsmState = (STATE)
/* BUG_11576_FIX_START */
//code_deleted
/* BUG_11576_FIX_END */
#define RRM_MIF_FSM_ACTION_ROUTINE(funcname)         \
        static void                         \
        funcname                                \
        (                                       \
            rrm_mif_gb_context_t     *p_gl_ctxt,    \
            rrm_mif_context            *p_mif_context,      \
            void                     *p_msg             \
        )


/*! \typedef struct mif_fcb
 *  \brief A Struct denoting Function Control Block of MIF FSM 
 *  \param fsmState Variable for FSM state 
 *  \param p_fsm_specific_info Pointer to FSM specific information
 */
typedef struct mif_fcb_t
{
    U8               fsmState;                 /*Fsm State*/
    /* BUG_11576_FIX_START */
    //code_deleted
    /* BUG_11576_FIX_END */
}mif_fcb;

/*! \typedef struct rrm_mif_fsm_context_info 
 *  \brief  a struct denoting Process Action Routine 
 *  \param p_info Pointer to information
 */
typedef struct rrm_mif_fsm_context_info_t
{
   void     *p_info;
}rrm_mif_fsm_context_info;

/*! \typedef struct mif_tcb
 *  \brief  Transaction Control Block of MIF FSM 
 *  \param trans_id Transaction Identifier
 *  \param api_first_source     Api originating source(SON/OAM)
 *  \param p_next  Pointer pointing to next element
 */
typedef struct mif_tcb_t
{
    U16             trans_id;
    U16             api_first_source;
    U16             fail_cause;
/*SPR 15993 Fix Start*/
    U16             egress_trans_id;
/*SPR 15993 Fix End*/
    struct mif_tcb_t       *p_next;
    /* SPR_19279_START */
    void            *p_data;
    /* SPR_19279_END */
}mif_tcb;

/*! \typedef enum mif_source_adm_stop_et
 *  \brief  An enum denoting Source for Stop admission MIF FSM
 */
typedef enum
{
    MIF_SOURCE_BLOCK_CELL = 0x01,
    MIF_SOURCE_SWITCHOFF_GRACEFUL_CELL = 0x02,
    MIF_SOURCE_SWITCHOFF_FORCEFUL_CELL = 0x04,
    MIF_SOURCE_SWITCHON_CELL = 0x08,
    MIF_SOURCE_SHUTDOWN_CELL = 0x10,
}mif_source_adm_stop_et;

/*! \typedef struct rrm_mif_context
 *  \brief  A struct denoting Process Action Routine 
 *  \param mif_ctxt_id Mif context ID 
 *  \param fcb[MAX_FSM_TYPE] Denotes FSM info
 *  \param cell_index Cell index used across Layer 2 & 3
 *  \param currently_processed_api_trans_d Denotes currently processed api transaction id
 *  \param p_tcb Pointer to TCB
 *  \param id Denotes attr ID
 *  \param source_stop_adm variable of mif_source_adm_stop_et type
 *  \param prev_block_cell_priority Denotes pevious block cell proirity
 *  \param is_block_cell_in_progress Denotes if blocked cell is in progress 
 */ 
typedef struct rrm_mif_context_t
{
   U8                              mif_ctxt_id;
   mif_fcb                         fcb[MAX_FSM_TYPE];/*Fsm Info */ 
   rrm_cell_index_t                cell_index; /* Cell Index used across Layer 2 and 3 */
   mif_tcb                        *p_tcb;
   U16                             currently_processed_api_trans_d;
/*SPR 15993 Fix Start*/
   U16                             timer_expiry_trans_id;
/*SPR 15993 Fix End*/
   rrm_attr_id_et                  id;
   mif_source_adm_stop_et          source_stop_adm;
   U8                              prev_block_cell_priority;
   U8                              is_block_cell_in_progress;
   rrm_bool_et                     block_cell_ongoing;
   /* eICIC_PHASE_1_2_CHANGES_START */
   rrm_oam_eicic_node_type_et      eicic_prov_type;
   /* eICIC_PHASE_1_2_CHANGES_END */
   /* SPR-20656 START */
   rrm_bool_et                     emergcy_call_ongoing;
   /* SPR-20656 END */

}rrm_mif_context;
/*! \fn rrm_return_t rrm_mif_send_internal_msg(rrm_module_id_t, U16, U16, void *)
 *  \brief  This function is entry point for RRM MgmtIfH FSM. This function
 *          calls appropriate function handler based on state and
 */
rrm_return_et rrm_mif_send_internal_msg
(
    rrm_module_id_t,    /*! Destination module identifier */
    U16,                /*! API Id */
    /* SPR 20653 Fix Start */
    U32,                /*! Size of message (payload) */
    /* SPR 20653 Fix End */
    void *              /*! Message specific info (payload) */
);

/*! \fn U16 rrm_get_non_emergengy_active_call (rrm_cell_index_t cell_index)
 *  \brief This function is to get number of active ue in given cell index.
 *  \param cell_index Denotes Global Cell ID
 */
U16 rrm_get_non_emergengy_active_call (rrm_cell_index_t cell_index);


/* Coverity_ID : 54441, 54443, 54444 start */
/*! \fn rrm_si_window_length_et rrm_map_si_window_length (U8 rrm_si_window_length)
 *  \brief This function map si window length.
 *  \param rrm_si_window_length denote si window length
 */
rrm_si_window_length_et
rrm_map_si_window_length (
    U8 rrm_si_window_length
);
/* Coverity_ID : 54441, 54443, 54444 end */


 void rrm_mif_fsm_set_state(rrm_mif_context *p_mif_context,
        U16 prev_ste,
        U16 next_state);

#endif /* _RRM_MIF_FSM_H_ */
