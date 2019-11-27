/*******************************************************************************
*
*  FILE NAME   : x2ap_enb_conf_sm.h
*
*  DESCRIPTION : This file contains the state machine and the state machine Entry
*                function declaration.
*
*  DATE           NAME                REFERENCE                  REASON
*  13-July-2010   Vaibhav Singh       36.423_880(3GPP Spec)      Initial
*
*  Copyright 2009, Aricent Inc.
*
*******************************************************************************/

#ifndef _X2AP_ENB_CONF_SM_
#define _X2AP_ENB_CONF_SM_

#include "commoninc.h"
#include "rrc_x2apCommon_intf.h"
#include "rrc_x2apRrm_intf.h"
/*Coverity Fix 29163 Start */
//#include "x2ap_db.h"
/*Coverity Fix 29163 End */
/*****************************************************************************
 * DATA
 *      DESCRIPTION: This enum is used for the enumeration of 
 *       ENB CONFIGURATION UPDATE PROCEDURE States.
 ****************************************************************************/
typedef enum
{
    X2AP_ENB_CONFIG_IDLE_ST = 0,
    X2AP_ENB_CONFIG_ACTIVE_ST,
    X2AP_ENB_CONFIG_W_CONF_RESP_ST,
    X2AP_ENB_CONFIG_TIME_TO_WAIT_RESEND_ST
} x2ap_enb_config_state_et;

/******************************************************************************
 * DATA DESCRIPTION : This enum is used for the enumeration of  
 *                    ENB CONFIGURATION UPDATE PROCEDURE Events.
 ******************************************************************************/

typedef enum
{
	X2AP_ENB_CONFIG_REQ_EV = 0,
	X2AP_ENB_CONFIG_ENB_REQ_EV,
	X2AP_ENB_CONFIG_ACK_EV,
	X2AP_ENB_CONFIG_FAIL_EV,
	X2AP_ENB_CONFIG_TIMER_EXPIRY_EV,
	X2AP_ENB_CONFIG_TIME_TO_WAIT_EXPIRY_EV,
	X2AP_ENB_CONFIG_RESET_REQ_EV,
	X2AP_ENB_CONFIG_SCTP_CONN_DOWN_EV,
    X2AP_ENB_CONFIG_MAX_EV
} x2ap_enb_config_event_et;

typedef struct
{
    x2ap_enb_config_state_et state;
    U8 req_retry_count;
    U8 time_to_wait_retry_count;
    x2ap_timer_t time_to_wait_timer_id;
    x2ap_timer_t req_timer_id;
    x2ap_enb_config_update_req_t enb_config_req;
    U16 transaction_id;
    U16 dst_module_id;
}x2ap_enb_config_sm_context_t;

typedef struct
{
    x2ap_enb_config_state_et state;
    U8 req_retry_count;
    U8 time_to_wait_retry_count;
    x2ap_timer_t time_to_wait_timer_id;
    x2ap_timer_t req_timer_id;
    x2ap_endc_config_update_req_t endc_config_req;
    U16 transaction_id;
    U16 dst_module_id;
}x2ap_endc_config_sm_context_t;

#endif /*_X2AP_ENB_CONF_SM_ */
