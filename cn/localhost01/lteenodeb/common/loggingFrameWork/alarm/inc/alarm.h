
/**************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc. All Rights Reserved.
 *
 *************************************************************************
 *
 * $Id: alarm.h 
 *
 *************************************************************************
 *
 * File Description : This file contains the declaration of functions and 
 *                    structure that are used for alarm handling
 *************************************************************************/

#ifndef ALARM_H
#define ALARM_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "socket_utils.h"
#include "shm_utils.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/* API ID */
#define OAM_ALARM_NOTIFICATION 0xFFF0

/* CA changes start */
/* the value should be set as INTERFACE_API_HEADER_LEN */
#define OAM_INTERFACE_HEADER_SIZE 16
/* the value should be set as API_HEADER_CELL_ID_NIL */
#define OAM_API_HEADER_CELL_ID_NIL 0xFF
/* CA changes end */

#define OAM_TRANSACTION_ID 0

#define MAX_STR_SIZE    80

/* Cyclomatic Complexity changes - starts here */
#ifdef LAYER2_ALARM_REPORTING_ENABLED 
#ifdef UE_SIM_TESTING
#define ALARM_MSG(src_module_id, alarm_id, criticality)
#else
#define ALARM_MSG(src_module_id, alarm_id, criticality) do \
{ \
        send_alarm(src_module_id, alarm_id, criticality);\
}while(0)
#endif
#else
#define ALARM_MSG(src_module_id, alarm_id, criticality)
#endif
/* Cyclomatic Complexity changes - ends here */

#ifdef UE_SIM_TESTING
#define ALARM_FMT_MSG(src_module_id, alarm_id, criticality,...)
#else
#define ALARM_FMT_MSG(src_module_id, alarm_id, criticality,...) do \
{ \
        send_fmt_alarm(src_module_id, alarm_id, criticality,##__VA_ARGS__);\
}while(0)
#endif
/****************************************************************************
 * Exported Types
 ****************************************************************************/


/* Enumeration of Alarm IDs */ 
typedef enum
{
    BASE_GENERIC_ALARM_ID = 0,
    MEM_ALLOC_FAILED_ALARM_ID = BASE_GENERIC_ALARM_ID+1,
    TIMER_START_FAILED_ALARM_ID,
    TIMER_STOP_FAILED_ALARM_ID,
    MEM_FREE_FAILED_ALARM_ID,
    SEND_MSG_FAILED_ALARM_ID,
    RECV_MSG_FAILED_ALARM_ID,
#ifdef LTE_EMBMS_SUPPORTED
    SYSTEM_FAILURE,
    MCE_UNREACHABLE_ALARM_ID,
    M2_LINK_FAILURE_ALARM_ID,
#endif
    MAX_GENERIC_ALARM_ID = 100,

    BASE_RRC_ALARM_ID =  MAX_GENERIC_ALARM_ID,
    RRC_PROVISIONING_FAILURE_ALARM_ID,
    S1AP_PROVISIONING_FAILURE_ALARM_ID,
    RRC_SCTP_ASSOCIATION_FAILURE_ALARM_ID,
    RRC_SCTP_ASSOCIATION_RECOVERY_ALARM_ID,
    RRC_S1AP_CONNECTION_FAILURE_ALARM_ID,
    RRC_S1AP_CONNECTION_SUCCESS_ALARM_ID,
    RRC_LICENSE_EXPIRY_ALARM_ID,
    /*CSR 51441 Start*/
    X2AP_RRM_RESET_ALARM_ID,
    X2AP_PEER_ENB_RESET_ALARM_ID,
    X2AP_SELF_RESET_ALARM_ID,
    /*CSR 51441 Stop*/
    /*CSR 51442 Start*/
    S1AP_RESET_ALARM_ID,
    /*CSR 51442 Stop*/
    /*CSR 51443 Start*/
    RRC_X2AP_CONNECTION_FAILURE_ALARM_ID,
    RRC_X2AP_CONNECTION_SUCCESS_ALARM_ID,
    /*CSR 51443 Stop*/
    /*CSR 51447 Start*/
    RRC_S1AP_ERROR_INDICATION_ALARM_ID,
    /*CSR 51447 Stop*/
    /*CSR 51448 Start*/
    RRC_X2AP_ERROR_INDICATION_ALARM_ID,
    /*CSR 51448 Stop*/

    MAX_RRC_ALARM_ID  = BASE_RRC_ALARM_ID + 100,
    
    BASE_PDCP_ALARM_ID = MAX_RRC_ALARM_ID,
    MAX_PDCP_ALARM_ID  = BASE_PDCP_ALARM_ID + 100,

    BASE_RLC_ALARM_ID =  MAX_PDCP_ALARM_ID,
    MAX_RLC_ALARM_ID  = BASE_RLC_ALARM_ID + 100,

    BASE_MAC_ALARM_ID =  MAX_RLC_ALARM_ID,
    MAX_MAC_ALARM_ID  = BASE_MAC_ALARM_ID + 100,

    MAX_ALARM_ID      =  MAX_MAC_ALARM_ID
}alarm_id_et;


/* Enumeration of Alarm SUB IDs */ 

typedef enum
{
    IPC_INVALID_HANDLE_ERR_CAUSE_ID = 1,
    IPC_NULL_POINTER_ERR_CAUSE_ID,
    IPC_NO_BUFF_AVAILABLE_ERR_CAUSE_ID,
    IPC_INVALID_PARAMTER_ERR_CAUSE_ID,
    MEM_ALLOC_NO_BUFF_AVAILABLE_ERR_CAUSE_ID,
    MEM_ALLOC_INVALID_PARAM_ERR_CAUSE_ID,
    MEM_FREE_NULL_POINTER_PASSED_ERR_CAUSE_ID,
    MEM_FREE_POOL_ID_CORRUPTED_ERR_CAUSE_ID,
    MEM_FREE_ALREADY_FREE_ERR_CAUSE_ID,
    MEM_FREE_MEM_CORRUPTED_ERR_CAUSE_ID,
    TIMER_CREATE_NO_BUFF_IN_TIMER_POOL_ERR_CAUSE_ID,
    TIMER_CREATE_INVALID_PARAM_ERR_CAUSE_ID,
    TIMER_START_INVALID_PARAM_ERR_CAUSE_ID,
    TIMER_STOP_INVALID_TIMER_ID_ERR_CAUSE_ID,
    TIMER_STOP_INVALID_PARAM_ERR_CAUSE_ID,
    INVALID_ERR_CAUSE_ID
}alarm_cause_et;
/* Enumeration of Alarm Criticality */ 
typedef enum
{
    CRITICAL_ALARM,
    MAJOR_ALARM,
    MINOR_ALARM,
    NOTIFICATION,
    DEFAULT_ALARM
}alarm_criticality_et;


/******************************************************************************
*   OAM_ALARM_NOTIFICATION
******************************************************************************/
typedef struct _oam_alarm_notification_t
{
    unsigned short int alarmId;
    unsigned short int criticality;
    char almStr[MAX_STR_SIZE];
}oam_alarm_notification_t;

typedef struct
{
    unsigned short int src_module_id;
    unsigned short int dst_module_id;
    char dst_ip_addr[IP_ADDR_MAX_SIZE];
    unsigned int dst_port;
    int sockfd;
    struct sockaddr_in serv_addr;
}alarm_cntxt_t;
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* +- SPR 17777 */
void alarm_init(unsigned short int, char*, unsigned int );
/* +- SPR 17777 */
void send_alarm(unsigned short int, unsigned int, unsigned int);
void send_fmt_alarm(unsigned short int, unsigned int, unsigned int, const char* , ...);

#endif
