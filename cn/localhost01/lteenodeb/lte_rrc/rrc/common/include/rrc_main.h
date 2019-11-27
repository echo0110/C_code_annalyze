/******************************************************************************
*
*       FILE NAME:
*               rrc_main.h
*
*       DESCRIPTION:
*               This header file contains all definations for CMES at RRC
*               layer.
*
*       DATE                    AUTHOR          REFERENCE               REASON
*       01 June 2009         Ritika Garg        ---------               Initial
*
*       Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_MAIN_H_
#define _RRC_MAIN_H_

#include "cspl.h"
#include "stacklayer.h"
#include "rrc_defines.h"


/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define RRC_CONFIG_FILE "rrcConfigFile"

#define    INVALID_FD                   -1
#define    MAX_SOCK_FD                  7
#define    CSPL_TIME_MICRO_SEC          10000

#define    CSPL_HDR_GET_SOURCE(b)       ((UInt16)yfGetU16At((b), 1))
#define    CSPL_HDR_GET_DESTINATION(b)  ((UInt16)yfGetU16At((b), 3))
#define    CSPL_HDR_GET_API_ID(b)       ((UInt16)yfGetU16At((b), 5))
#define    CSPL_HDR_GET_API_SIZE(b)     ((UInt16)yfGetU16At((b), 7))
#define    CSPL_HDR_GET_PAYLOAD_SIZE(b) ((UInt32)yfGetU16At((b), 11))
#define    CSPL_HDR_GET_PRIORITY(b)     ((SChar)yfGetU8At((b), 13))

#define    IPC_IF                       8
#define    IPC_SZ                       6

#define    RRM_IP_INDEX                 0
#define    OAM_IP_INDEX                 1
#define    S1AP_IP_INDEX                2
#define    PDCP_IP_INDEX                3
#define    RLC_IP_INDEX                 4
#define    MAC_IP_INDEX                 5
#define    PHY_IP_INDEX                 6
#define    RF_IP_INDEX                  7
#define    RRC_IP_INDEX                 8
#define    IP_INDEX_NONE                9

#define    IPADDR_MAX_LEN               64
#define    EXT_API_HDRSIZE              16
#define    MAX_KEY_SIZE                 128
#define    MAX_BUFF_SIZE                256
#define    IPADDR_PORT_MAX_LEN          300
#define    LABEL_MAX_LEN                75

/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct ListenerTag
{
    S32      sdRrc;
    fd_set   fdSet;
}Listener;

/* gur04564 - in_addr & sockaddr should be a part of socket header file
typedef struct in_addr
{
    U32      s_addr;
};*/


typedef struct sockaddr_Tag
{
    short           sin_family;       /* AF_INET */
    U16             sin_port;         /* Port */
    struct in_addr  sin_addr;         /* IP Addr */
    U16             sock_type;        /* SOCK Stream or DGRAM */
    struct sockaddr *next;
}sockaddr;

/* Structure for External API Header */
typedef struct EXTAPIHDR_Tag
{
    U16      transaction_id;
    U16      source_id;
    U16      dest_id;
    U16      api_id;
    U16      buf_len;
    U8       spare[6];
}EXTAPIHDR;

U8 IpcDataList[IPC_IF][IPC_SZ];
U8 confLabel[LABEL_MAX_LEN];

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

extern const QSHELL     __operations;
extern const QSHELL     *operations;


void rrc_timed_wait (void *p_cspl_listener,
                     const QTIME *p_timer_data);

void rrc_send (void *p_cspl_listener,
               const void *message_p, /* message to be sent */
               QMODULE sender,
               QMODULE receiver,
               void *data_p,
               U32 size);

void *rrc_receive (void      *p_cspl_listener,
                   QMODULE   *sender_p,
                   QMODULE   *receiver_p,
                   S8     *priority_p);

void *rrc_open (void *data_p);

void rrc_close (void *listenerData_p);


#endif /* _RRC_MAIN_H_ */
