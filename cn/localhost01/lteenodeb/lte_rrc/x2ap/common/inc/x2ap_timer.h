/*******************************************************************************
*
*  FILE NAME   : x2ap_timer.h
*
*  DESCRIPTION : This file contains the structure and function declaration
*                required for x2ap Timer managment.
*
*  DATE 24-June-2010 NAME Ashutosh Gupta REFERENCE 36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#ifndef _X2AP_TIMER_H_
#define _X2AP_TIMER_H_

#include "commoninc.h"

#define MILISECS_1SEC 1000

#define  X2AP_TIMER_OAM_INIT_IND_DEFAULT_DURATION       3000 /* in ms */
#define  X2AP_TIMER_X2_SETUP_RESP_DEFAULT_DURATION      1000 /* in ms */
#define  X2AP_TIMER_RESET_RESP_DEFAULT_DURATION         1000 /* in ms */
#define  X2AP_TIMER_CONN_RECOVERY_DEFAULT_DURATION      7000 /* in ms */
#define  X2AP_TIMER_CONN_DEFAULT_DURATION               7000 /* in ms */

typedef enum 
{
    /* Timer started of OAM Init Ind
     */
    X2AP_OAM_INIT_TIMER, 

    /* Timer started for X2 setup request
     */
    X2AP_X2_SETUP_RSP_TIMER,
   
    /* Time started when recived failure resp
     */
    X2AP_X2_SETUP_TIME_TO_WAIT_TIMER, 

    /* Timer started upon reception of Connection
     * failure indication from SCTP.
     */
    X2AP_SCTP_CONN_REC_TIMER,     

    /* Timer started to open SCTP association
     */
    X2AP_SCTP_CONNECT_TIMER,

    /* Timer started upon reception of time to wait IE in
     * X2 SETUP FAILURE
     */
    X2AP_ENB_CONFIG_TIME_TO_WAIT_TIMER,

    /* 
     * Timer started after receiving oam prov req
     * to guard CSI Up Indication 
     */
    X2AP_CSI_IND_GUARD_TIMER,
    
    /* Timer started for eNB Configuration Update Resp wait 
    */
    X2AP_ENB_CONFIG_UPDATE_RSP_TIMER,
    /* Timer started for X2AP RSU request 
    */
    X2AP_RSU_ENB_GUARD_TIMER,

    X2AP_RSU_RRM_GUARD_TIMER,
    /*Timer started after Sending RESET resquest to peer eNodeB*/
    X2AP_ENB_RESET_TIMER,

    X2AP_CELL_ACTIVATION_TIMER,
    /*Timer started after Sending Mobility Change resquest to peer eNodeB*/
    X2AP_ENB_MOBILITY_CH_TIMER,
    
    /*Timer started after Sending Mobility Change resquest to RRM*/
    X2AP_MOBILITY_CH_RRM_GUARD_TIMER,
    /*Timer started after sending cell_act_req to target RRM/SON*/
    X2AP_CELL_ACT_RRM_GUARD_TIMER,
    /*Timer started health monitor procedure*/
    X2AP_OAM_HEALTH_MONITOR_TIMER
}x2ap_timer_id_en;

typedef struct _x2ap_timer_buffer_t
{
    /* It identifies the type of request for which a timer is
     * started
     */
    x2ap_timer_id_en timer_type;

    /* This can be used to maintain any timer specific data*/
    void * p_timer_specific_data;

    /* If true timer buff is freed by timer handler
     */
    x2ap_bool_et             auto_delete;
    sctp_sd_t               sd;
    /*SPR_16533_DELTA2_START*/
#ifdef LKSCTP
    /*ip address will be used for fetching enb context upon expiry of 
     X2AP_X2_SETUP_RSP_TIMER and X2AP_X2_SETUP_TIME_TO_WAIT_TIMER */
    U8                      ip_addr[MAX_X2AP_IPV6_ADDRESS_LENGTH];
#endif
    /*SPR_16533_DELTA2_END*/
}x2ap_timer_buffer_t;

/* X2AP Timer Management Functions Prototypes */
x2ap_timer_t x2ap_start_timer(
    U32         timer_duration,
    void        *p_timer_data,
    U16         timer_data_size,  
    x2ap_bool_t  is_repeated );

void x2ap_stop_timer (
    x2ap_timer_t timer );

void* x2ap_get_timer_data( x2ap_timer_t timer );

#endif

