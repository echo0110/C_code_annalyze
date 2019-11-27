/******************************************************************************
*   FILE NAME   : oam_snmp_main.h
*
*   DESCRIPTION : SNMP modules 
*
*       DATE            AUTHOR          REFERENCE       REASON
*   25 July 2013   Purshottam Gupta     ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
/*******************************************************************************
*                        MACROS
*******************************************************************************/
#ifndef OAM_SNMP_MAIN_H
#define OAM_SNMP_MAIN_H


#define  SH_HDRSIZE 16
#define  QCLI_HEADER_SIZE 12
#define  CSPL_HEADER_SIZE 16
#define  INTERFACE_HEADER_SIZE 10
#define  OAM_MODULE_ID 13
#define  SNMP_MODULE_ID 27
#define  INTERFACE_MODULE_ID 28
#define  RRC_MODULE_ID 7
#define  PDCP_MODULE_ID 8
#define  RLC_MODULE_ID 9
#define  MAC_MODULE_ID 10
#define  S1_MODULE_ID 11
#define  OAM_ZERO 0
#define OAM_SNMP_TRACE_FORMAT_SIZE               500
#define OAM_SNMP_TRACE_TEMP_STR_SIZE              60
#define OAM_SNMP_TRACE_YEAR_FMT                 1900
#define OAM_MGMT_IF_MAX_MSG_LEN                 4096
#define MAX_PORT_LENGTH            6
#define MAX_IP_ADD_LEN_STR         64
/* SPR 20584 changes start */
#define SNMP_ID_ADDR_AND_PORT      MAX_IP_ADD_LEN_STR + MAX_PORT_LENGTH + 6  
                                   /* 6 bytes reqd for additinal string tcp::\0 */ 
/* SPR 20584 changes end */


/*******************************************************************************
*                        Function's Prototype
*******************************************************************************/
int agent_check_and_process(int block);

U16 get_word_from_header
(
 U8 *p_header
);

/* + SPR 17439 */
void init_oam_snmp_agent(void);
/* - SPR 17439 */

/*******************************************************************************
*                           Enumrations
*******************************************************************************/
typedef enum
{
        OAM_SUCCESS=1,
        OAM_FAILURE=0
}oam_return_et;

typedef enum
{
        SNMP_GET_REQ,
        SNMP_SET_REQ,
        SNMP_CTRL_CMD_REQ,
        SNMP_TRAP_MSG
}oam_request_et;

/*******************************************************************************
*                            Structures
*******************************************************************************/
typedef struct thread_arg
{
    QDRIVER driver_id;
    QWAIT * p_qwait;
}thread_arg_t;

typedef struct __oam_snmp_comm_info_t {
    char oam_ip_addr[20];
    int oam_port;
    char snmp_ip_addr[20];
    int snmp_port;
} oam_snmp_comm_info_t;


typedef struct tran_ctrl_ptr
{
    UInt8        mutex_flag;
    UInt32   transaction_id;
    UInt8          msg_ptr[OAM_MGMT_IF_MAX_MSG_LEN];
}tran_ctrl_ptr_t;

typedef tran_ctrl_ptr_t trans_cntrl_ptr;

#endif  /* OAM_SNMP_MAIN_H */
