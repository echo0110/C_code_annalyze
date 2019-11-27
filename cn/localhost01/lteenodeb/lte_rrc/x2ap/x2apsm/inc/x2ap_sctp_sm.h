/*******************************************************************************
*
*  FILE NAME   : x2ap_sctp_sm.h
*
*  DESCRIPTION : This file contains the sctp state machine and the state machine Entry
*                function declaration.
*
*  DATE 24-June-2010  NAME Ashutosh Gupta  REFERENCE  36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#ifndef _X2_AP_SCTP_FSM_H_
#define _X2_AP_SCTP_FSM_H_

#include "x2ap_types.h"
#include "commoninc.h"




/*****************************************************************************
 * DATA
 *      DESCRIPTION: This enum is used for the enumeration of X2AP States.
 ****************************************************************************/
typedef enum 
{
    X2AP_SCTP_IDLE_ST = 0,  
    /*SPR_16533_START*/
        /* Code Removed */
    /*SPR_16533_END*/
    X2AP_SCTP_W_COMM_UP,
    X2AP_SCTP_ESTABLISHED_ST,  
    X2AP_SCTP_RELEASE_ONGOING_ST,
    X2AP_SCTP_MAX_ST
}x2ap_sctp_state_et;

/******************************************************************************
 * DATA DESCRIPTION : This enum is used for the enumeration of X2AP Events.
 ******************************************************************************/
typedef enum 
{
    X2AP_SCTP_CONNECT_EV = 0,
    X2AP_SCTP_COMM_UP_EV,
    X2AP_SCTP_CONN_LOST_EV,
    X2AP_SCTP_CONN_RSTRT_EV,
    X2AP_SCTP_CLOSED_EV,
    X2AP_SCTP_SHUT_DOWN_EV,
    X2AP_SCTP_CONN_CLOSE_EV,
    X2AP_SCTP_SEND_DATA_EV,
    X2AP_SCTP_RCVD_DATA_EV,
    X2AP_SCTP_CONNECT_TMR_EXP_EV,
    X2AP_SCTP_PENDING_CONN_EV,
    X2AP_SCTP_CLOSE_ALL_X2AP_SCTP_CONN_EV,
    X2AP_SCTP_MAX_EV
}x2ap_sctp_event_et;


#endif

