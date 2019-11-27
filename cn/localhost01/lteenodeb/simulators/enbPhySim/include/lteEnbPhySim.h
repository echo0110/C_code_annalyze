/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteEnbPhySim.h $
 *
 ****************************************************************************
 *
 *  File Description :  This file contains declaration of EnbPhySim
 *
 ****************************************************************************
 *
 * Revision Details :
 *
 ****************************************************************************/

#ifndef LTE_ENB_PHY_SIM_H
#define LTE_ENB_PHY_SIM_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include <stdio.h>
#include "lteTypes.h"
#include "lteSQueue.h"
#include "lteCircQueue.h"


/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

//#define RECV_BUFF_SIZE 64000
#define RECV_BUFF_SIZE 20000
#define DATA_BUFFER_SIZE            19000
#define HOSTNAME_MAX_LEN 64
#ifndef LAYER2_PHYSIM_THREAD
#define MAX_FD_ELEMENTS 4
#else
/* MAX_FD_ELEMENTS will be 3 file descriptor for rxSockFD_g, readSockFD_g, 
 * pdcpSocketRx and 2 for data and control port per cell */
#define MAX_FD_ELEMENTS MAX_NUM_CELLS*2 + 3
#endif
#define MAC_PHY_DELAY 2
#define FAPI_SFN_VALUE_SHIFT 4

#define ENB_PHY_SIM_PORT_FILE_NAME  "../cfg/ENB_PHY_SIM_Configuration.cfg"

#define GET_ENB_PHY_SIM_PORT_CONFIG_FILE(filename) {\
    strcpy(filename,ENB_PHY_SIM_PORT_FILE_NAME);\
}

#if 0
/* This macro is used for declaring array of variable length */
#define FAPI_VAR_SIZE(x) 1


/**
 * FAPI_msgIdType_t definition
 */
typedef uint8_t FAPI_msgIdType_t;


/****************************************************************************
 * Exported Types
 ****************************************************************************/

LTE_SQUEUE macPhyDataQ_g;

typedef struct FAPI_l1ApiMsg_stT
{
     /* Message type ID */
     FAPI_msgIdType_t     msgId;

     /* Length of vendor-specific message body (bytes) */
     uint8_t              lenVendorSpecific;

     /* Length of message body (bytes) */
     uint16_t             msgLen;

     /* Message body.
      * This shall contain one of the below mentioned structure.
      * Each of the below mentioned structure are specific to
      * different message type */
     uint8_t              msgBody[FAPI_VAR_SIZE(msgLen)];

     /* Vendor-specific message body */
     uint8_t              vendorMsgBody[FAPI_VAR_SIZE(lenVendorSpecific)];

}FAPI_l1ApiMsg_st;
typedef struct
{
    LTE_SNODE  anchor;
    SInt32      len;
    void      *msg;
} macPhyDataNode;
#endif

typedef struct
{
    SInt32      len;
    void      *buffer;
} macPhyMsg;

#ifdef LAYER2_PHYSIM_THREAD
extern LTE_CIRCQUEUE dlL2PhysimDataQ_g ;
extern LTE_CIRCQUEUE dlL2PhysimControlQ_g ;
extern LTE_CIRCQUEUE ulL2PhysimDataQ_g ;
extern LTE_CIRCQUEUE ulL2PhysimControlQ_g ;

#define QUEUE_INIT_L2_PHYSIM_Q(physimQ)\
    circQInit(&physimQ,MAX_NODES)
#if 0
#define QUEUE_INIT_L2_PHYSIM_UL_Q()\
    circQInit(&ulL2PhysimQ_g,MAX_NODES)
#endif
/* This utility is applicable for both ul and dl PhysimQ */
#define DEQUEUE_L2_PHYSIM_Q(physimQ, physimDataQ_p){\
    popNodeCircQ(&physimQ,(void *)&(physimDataQ_p));\
    physimDataQ_p = (macPhyMsg *) physimDataQ_p;\
}
#define GETNODE_QUEUE_L2_PHYSIM_Q(physimQ, physimDataQ_p){\
    getNodeCircQ(&physimQ,(void *)&(physimDataQ_p));\
    physimDataQ_p = (macPhyMsg *) physimDataQ_p;\
}

/* This utility is applicable for both ul and dl PhysimQ */
#define ENQUEUE_L2_PHYSIM_Q(physimQ, physimDataQ_p)\
    pushNodeCircQ(&physimQ, physimDataQ_p)

#define QUEUE_COUNT_L2_PHYSIM_Q(physimQ) \
    circQueueCount(&physimQ)

#endif
#endif

