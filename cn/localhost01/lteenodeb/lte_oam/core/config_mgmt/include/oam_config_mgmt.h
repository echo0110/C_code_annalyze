/* vim:ts=4:et:ai:ci:sw=4
*/
/*****************************************************************************
 * File Name   : oam_config_mgmt.h
 *
 * Description : This file contains includes required for OAM Configuration 
 *               Manager FSM Handler 
 *
 * Revision History:
 *
 * Date         Author           Reference              Comments
 * -----        ------           ---------              --------
 * April,2012  Nitin Agarwal   OAM Design Document      Initial Version                
 *
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 ****************************************************************************/

#ifndef _OAM_CM_FSM_H_
#define _OAM_CM_FSM_H_

/****************************************************************************
 **  INCLUDES
 ****************************************************************************/

#include <oam_types.h>
#include <oam_proto.h>
#include <oam_defines.h>
#include <cspl.h>

#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
#include <oam_rcm_core.h>
#endif
#endif

/****************************************************************************
 **  MACROS
 ****************************************************************************/

/* EMBMS Changes Start */
#define OAM_LAYER_INIT_TIMEOUT_DATA_SIZE      8
/* EMBMS Changes End */
#define OAM_INIT_TIMER_VALUE 	20000
#define SON_OAM_INIT_CNF 		 0x01
#define RRM_OAM_INIT_CNFM 		 0x02
/* SPR 20908 Start */
#define MAC_OAM_INIT_CELL_CNF 	 0x10
/* SPR 20908 End */
#define RRC_OAM_COMM_INIT_CNF 	 0x40
#define RRC_OAM_PROV_INIT_CNF 	 0x80
#define S1AP_OAM_INIT_CNFM 		0x100
#define X2AP_OAM_INIT_CNFM 		0x200
/* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
/* SPR 22248 MBMS Changes Start */
#define M2AP_OAM_INIT_CNFM 		0x800
#define PDCP_EMBMS_INIT_CNF     0x1000
/* SPR 22248 MBMS Changes Stop */
#endif
/* EMBMS Changes End */
/*OAM_REVIEW_CHANGES*/
#define OAM_CM_ST_MAX   		    4
#define OAM_CM_EV_MAX       	    4
/*OAM_REVIEW_CHANGES*/
/****************************************************************************
 **  ENUMERATIONS / STRUCTURES
 ****************************************************************************/

/*enum: oam_cm_events_et
 *brief: An enum that specifies the OAM_CM_EVENTS.
 */

typedef enum
{
    OAM_CM_EV_LAYER_REQ,
    OAM_CM_EV_LAYER_RES,
    OAM_CM_EV_TIMER_EXPIRY,
    OAM_CM_EV_ABORT,
/*OAM_REVIEW_CHANGES*/
    OAM_CM_MAX_EVENT
}oam_cm_events_et;

/*enum: oam_cm_states_et
 *brief: An enum that specifies the OAM_CM_STATES.
 */

typedef enum
{
    OAM_CM_ST_IDLE,                           
    OAM_CM_ST_LAYER_INIT,                    
    OAM_CM_ST_LAYER_WF_RES,                  
    OAM_CM_ST_ACTIVE,                         
    OAM_CM_MAX_STATE                       
}oam_cm_states_et;

/*struct: oam_cm_cntxt_t
 *brief: A structure that contains parameters required for 
 *       CM FSM.
 */
typedef struct
{
    oam_cm_states_et    state;
    oam_cm_events_et    event;
    oam_timer_t         guard_timer_id;
} oam_cm_cntxt_t;

/****************************************************************************
 **  FUNCTION PROTOTYPES
 ****************************************************************************/

oam_return_et 
oam_stop_timer
(
    oam_timer_t timer
);

oam_timer_t
oam_start_timer
(
    /* Duration (in milliseconds) of the timer to be started */
    UInt32             timer_duration,
    /* Timer data that will be attached to timer */
    void              *p_timer_data,
    UInt16             timer_data_size,    /* Size of timer data */
    oam_bool_et        is_repeated
);

oam_return_et
oam_stop_timer
(
    oam_timer_t timer
);

oam_return_et
oam_cm_fsm_process_event
(
    void              *p_msg, 		/*Pointer to input api buffer*/
    oam_cm_cntxt_t    *p_cm_cntxt,  /*Pointer to CM FSM context*/
    oam_error_code_et *p_err 	    /*Pointer to error code*/
);


oam_return_et
oam_cm_son_req
(
    void 			  *p_msg, 	   /*Pointer to input api buffer*/
    oam_cm_cntxt_t 	  *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err 	   /*Pointer to error code*/
);

oam_return_et
oam_son_msg_handler 
(
    void              *p_msg,      /*Pointer to input api buffer*/
    oam_cm_cntxt_t    *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err       /*Pointer to error code*/
);

oam_return_et
oam_rrm_msg_handler 
(
    void              *p_msg,      /*Pointer to input api buffer*/
    oam_cm_cntxt_t    *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err       /*Pointer to error code*/
);

oam_return_et
oam_mac_msg_handler 
(
    void              *p_msg,      /*Pointer to input api buffer*/
    oam_cm_cntxt_t    *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err       /*Pointer to error code*/
);

oam_return_et
oam_rlc_msg_handler 
(
    void              *p_msg,      /*Pointer to input api buffer*/
    oam_cm_cntxt_t    *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err       /*Pointer to error code*/
);

oam_return_et
oam_pdcp_msg_handler 
(
    void              *p_msg,      /*Pointer to input api buffer*/
    oam_cm_cntxt_t    *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err       /*Pointer to error code*/
);

oam_return_et
oam_egtpu_msg_handler 
(
    void              *p_msg,      /*Pointer to input api buffer*/
    oam_cm_cntxt_t    *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err /*Pointer to error code*/
);

/* + SPR 17439 */
oam_return_et
oam_cm_handle_active_state(void);
/* - SPR 17439 */

oam_return_et
oam_rrc_msg_handler 
(
    void              *p_msg,      /*Pointer to input api buffer*/
    oam_cm_cntxt_t    *p_cm_cntxt /*Pointer to CM FSM context*/
    /*SPR 17777 fix*/
);

oam_return_et
oam_s1ap_msg_handler 
(
    void              *p_msg,      /*Pointer to input api buffer*/
    oam_cm_cntxt_t    *p_cm_cntxt /*Pointer to CM FSM context*/
    /*SPR 17777 fix*/
);

oam_return_et
oam_x2ap_msg_handler 
(
    void              *p_msg,      /*Pointer to input api buffer*/
    oam_cm_cntxt_t    *p_cm_cntxt /*Pointer to CM FSM context*/
    /*SPR 17777 fix*/
);

oam_return_et
oam_cm_layer_handler 
(
    void              *p_msg,      /*Pointer to input api buffer*/
    oam_cm_cntxt_t    *p_cm_cntxt, /*Pointer to CM FSM context*/
    oam_error_code_et *p_err       /*Pointer to error code*/
);

oam_return_et
oam_cm_layer_resp 
(
    void              *p_msg,      /*Pointer to input api buffer*/
    oam_cm_cntxt_t    *p_cm_cntxt, /*Context for CM FSM*/
    oam_error_code_et *p_err       /*error code*/
);

oam_return_et
oam_cm_abort
(
    void              *p_msg,      /*Pointer to input api buffer*/
    oam_cm_cntxt_t    *p_cm_cntxt, /*Context for CM FSM*/
    oam_error_code_et *p_err       /*error code*/
);

oam_return_et
oam_cm_ignore_event
(
    void              *p_msg,      /*Pointer to input api buffer*/
    oam_cm_cntxt_t    *p_cm_cntxt, /*Context for CM FSM*/
    oam_error_code_et *p_err       /*error code*/
);

oam_return_et
oam_cm_timer_expired 
(
    void              *p_msg,      /*Pointer to input api buffer*/
    oam_cm_cntxt_t    *p_cm_cntxt, /*Context for CM FSM*/
    oam_error_code_et *p_err       /*error code*/
);

oam_return_et
oam_cm_fsm_init (void);

/* SPR 20908 Start */
oam_return_et
find_l2_instance_id_from_cell_index(
    UInt8 cell_index,
    UInt8 *instance_id);
/* SPR 20908 End */
/*SPR 21096 Start*/

oam_return_et
oam_check_instance_id_present(
        UInt8 instance_id,
        UInt8 start_idx,
        U8 *cell_index);

oam_return_et
find_l2_instance_id_from_cell_index(
UInt8 cell_index,
UInt8 *instance_id);

/*SPR 21096 End*/
/* spr_21013_fix_start */
oam_return_et
oam_handle_initial_admin_state_unlocked
(
/* SPR 22248 MBMS Changes Start */
    UInt8 *p_is_oam_startup_required,
/* SPR 22248 MBMS Changes Stop */
    oam_rrm_cell_config_req_t  *p_rrm_cell_config,
    UInt16  *p_trans_id
);
/* spr_21013_fix_end */
oam_return_et
rrc_il_send_x2ap_oam_get_sctp_status_req
(
    x2ap_oam_get_sctp_status_req_t  *p_x2ap_oam_get_sctp_status_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 transaction_id,  /* Interface transaction identifier */
    oam_error_code_et      *p_error_code
);

oam_return_et
rrc_il_send_s1ap_oam_get_sctp_status_req
(
    s1ap_oam_get_sctp_status_req_t  *p_s1ap_oam_get_sctp_status_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 transaction_id,  /* Interface transaction identifier */
    oam_error_code_et       *p_error_code
);
#endif /* _OAM_CM_FSM_H_ */
