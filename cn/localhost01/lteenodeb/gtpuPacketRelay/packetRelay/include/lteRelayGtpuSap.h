/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRelayGtpuSap.h,v 1.1.1.1 2010/02/11 04:51:36 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains variable declarations for Packet
 *                   : Relay and GTP-U Interface.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRelayGtpuSap.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:36  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.1  2009/11/25 09:57:08  cm_lteenodeb
 * code checkin
 *
 *
 ****************************************************************************/

#ifndef  LTE_RELAY_GTPU_SAP_H
#define  LTE_RELAY_GTPU_SAP_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRelay.h"
#include "lteTypes.h"
/* COVERITY 10302 */
//#include "lteRelayS1Sap.h"
/* COVERITY 10302 */

/* External Entities */
#define EI_GTPU_MODULE_ID    0 
#define EI_UDP_MODULE_ID     1
#define EI_RELAY_MODULE_ID   2 
#define EI_S1AP_MODULE_ID    3 
#define EI_OAM_MODULE_ID     4
/*+SPR #3456*/
#define EI_TS_MODULE_ID      5 
/*-SPR #3456*/

/* API from Control Plane */
#define GTPU_CP_IN_API_BASE           200
#define GTPU_SETUP_SAP_REQ            (GTPU_CP_IN_API_BASE)
#define GTPU_RELEASE_SAP_REQ          (GTPU_CP_IN_API_BASE + 1)
#define GTPU_CP_LAST_IN_API           (GTPU_RELEASE_SAP_REQ)

/* API from Relay Entity */
#define GTPU_RELAY_IN_API_BASE        300
#define GTPU_DATA_REQ                 (GTPU_RELAY_IN_API_BASE)
#define GTPU_RELOC_DATA_REQ           (GTPU_RELAY_IN_API_BASE + 1)
#define GTPU_SEND_EMI_TO_PEER_REQ     (GTPU_RELAY_IN_API_BASE + 2)
#ifdef ENDC_ENABLED
#define GTPU_X2U_DELIVERY_STATUS      (GTPU_RELAY_IN_API_BASE + 3)
#define GTPU_RELAY_LAST_IN_API        (GTPU_X2U_DELIVERY_STATUS)
#else
#define GTPU_RELAY_LAST_IN_API        (GTPU_SEND_EMI_TO_PEER_REQ)
#endif

/* API from UDP */
#define GTPU_UDP_IN_API_BASE         100
#define GTPU_UNITDATA_IND            (GTPU_UDP_IN_API_BASE)
#define GTPU_UDP_LAST_IN_API         (GTPU_UNITDATA_IND)

/* API from Layer Management */
#define GTPU_LM_IN_API_BASE            400
#define GTPU_INITDB_REQ               (GTPU_LM_IN_API_BASE)
#define GTPU_SET_TIMER_REQ            (GTPU_LM_IN_API_BASE + 1)
#define GTPU_GET_TIMER_REQ            (GTPU_LM_IN_API_BASE + 2)
#define GTPU_GET_MAX_RETRIES_REQ      (GTPU_LM_IN_API_BASE + 3)
#define GTPU_SET_MAX_RETRIES_REQ      (GTPU_LM_IN_API_BASE + 4)
#define GTPU_GET_STATS_REQ            (GTPU_LM_IN_API_BASE + 5)
#define GTPU_GET_INIT_STATS_REQ       (GTPU_LM_IN_API_BASE + 6)
#define GTPU_INIT_TRACE_LEVEL_REQ     (GTPU_LM_IN_API_BASE + 7)
#define GTPU_SET_TRACE_LEVEL_REQ      (GTPU_LM_IN_API_BASE + 8)
#define GTPU_GET_TRACE_LEVEL_REQ      (GTPU_LM_IN_API_BASE + 9)
#define GTPU_GET_CTXT_INFO_REQ        (GTPU_LM_IN_API_BASE + 10)
#define GTPU_MODIFY_REORDERING_IE_REQ (GTPU_LM_IN_API_BASE + 11)



/* API to Control PLane */
#define GTPU_CP_OUT_API_BASE         250
#define GTPU_CP_SETUP_SAP_CNF        (GTPU_CP_OUT_API_BASE)
#define GTPU_CP_RELEASE_SAP_CNF      (GTPU_CP_OUT_API_BASE + 1)
#define GTPU_CP_ERROR_IND            (GTPU_CP_OUT_API_BASE + 2)
#define GTPU_CP_PATH_FAILURE_IND     (GTPU_CP_OUT_API_BASE + 3)
#define GTPU_CP_END_MARKER_IND       (GTPU_CP_OUT_API_BASE + 4)
/*+SPR #3456*/
#define GTPU_CP_PATH_SUCCESS_IND     (GTPU_CP_OUT_API_BASE + 5)
#define GTPU_CP_LAST_OUT_API         (GTPU_CP_PATH_SUCCESS_IND)
/*-SPR #3456*/

/* API to Relay */
#define GTPU_RELAY_OUT_API_BASE      350
#define GTPU_RELAY_DATA_IND          (GTPU_RELAY_OUT_API_BASE)
#define GTPU_RELAY_LAST_OUT_API      (GTPU_RELAY_DATA_IND)

/* API to Layer management */
#define GTPU_LM_OUT_API_BASE         450
#define GTPU_LM_INITDB_CNF           (GTPU_LM_OUT_API_BASE)
#define GTPU_LM_SET_TIMER_CNF        (GTPU_LM_OUT_API_BASE + 1)
#define GTPU_LM_GET_TIMER_CNF        (GTPU_LM_OUT_API_BASE + 2)
#define GTPU_LM_GET_MAX_RETRIES_CNF  (GTPU_LM_OUT_API_BASE + 3)
#define GTPU_LM_SET_MAX_RETRIES_CNF  (GTPU_LM_OUT_API_BASE + 4)
#define GTPU_LM_GET_STATS_CNF        (GTPU_LM_OUT_API_BASE + 5)
#define GTPU_LM_GET_INIT_STATS_CNF   (GTPU_LM_OUT_API_BASE + 6)
#define GTPU_LM_INIT_TRACE_LEVEL_CNF (GTPU_LM_OUT_API_BASE + 7)
#define GTPU_LM_SET_TRACE_LEVEL_CNF  (GTPU_LM_OUT_API_BASE + 8)
#define GTPU_LM_GET_TRACE_LEVEL_CNF  (GTPU_LM_OUT_API_BASE + 9)
#define GTPU_LM_GET_CTXT_INFO_CNF    (GTPU_LM_OUT_API_BASE + 10)
#define GTPU_LM_ALARM_NOTIFICATION   (GTPU_LM_OUT_API_BASE + 11)
#define GTPU_LM_MODIFY_REORDERING_IE_CNF (GTPU_LM_OUT_API_BASE + 12)
#define GTPU_LM_LAST_OUT_API         (GTPU_LM_OUT_API_BASE + 13)

#define GTPU_DEF_PRIORITY        0
#define GTPU_API_VERSION_NUMBER  1
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/* structure used for Configure Sap Request for Relay and GTP-U Interface */

typedef struct gtpuSetupSapReqTag
{
    relayGtpuTrasId transactionId;    /* To identify transaction id 
                                         (Conditional will present only first time)*/
    relayGtpuTid teidSelf;            /* To identify self tunnel id 
                                         (Conditional will present after first time)*/
    tvHeader sapFlag;                 /* To indicate Sap Flag (Mandatory)*/
    tlvHeader qosProfile;             /* To identify Qos Profile (Mandatory)*/
    UInt32 qosId;                     /* To identify qos identifier (Mandatory)*/
    tvSNHeader seqNumUplink;          /* To identify UL Seq Number 
                                         (Optional)*/
    tvSNHeader seqNumDnlink;          /* To identify DL Seq Number 
                                         (Conditional, Present if seqNumUplink is present)*/
    relayGtpuTid teidPeer;            /* To identify peer tunnel id 
                                         (Optional Length is fixed that's two bytes*/
    tlvHeader transportAddr;          /* To identify transport Address 
                                         (Conditional, Present if teidPeer is present)*/
    tvHeader seqFlag;                 /* To indicate Seq Flag 
                                         (Optional Length is fixed that's two bytes)*/
    tvHeader reorderingReqd;          /* To identify reordering reqd flag 
                                         (Optional Length is fixed that's two bytes*/
}gtpuSetupSapReq;

/* structure used for Configure Sap Confirmation for Relay and GTP-U Interface */

typedef struct gtpuSetupSapCnfTag
{
    UInt8 lcId;                       /* To indetifiy logical channel */
    tvHeader cause;                   /* To indicate the cause (Mandatory) */
    relayGtpuTid teidSelf;            /* To indicate self tunnel identifier
                                         (Conditional present if cause contains failure) */
    tvSNHeader seqNumUplink;          /* To indicate UL Sequence Number
                                         (Conditional present if cause contains success)*/
    tvSNHeader seqNumDnlink;          /* To indicate DL Sequence Number
                                         (Conditional, present if cause contains success)*/
}gtpuSetupSapCnf;

/* structure used for Release Sap Request for Relay and GTP-U Interface */

typedef struct gtpuRelSapReqTag
{
    relayGtpuTid teidSelf;           /* To indicate self tunnel identifier (Mandatory) */
}gtpuRelSapReq;

/* structure used for Release Sap Confirmation for Relay and GTP-U Interface */

typedef struct gtpuReleaseSapCnfTag
{
    tvHeader cause;                   /* To indicate the cause (Mandatory)*/
    relayGtpuTrasId transactionId;    /* To identify transaction id */ 
    relayGtpuTid teidSelf;            /* To indicate self tunnel identifier
                                         (Conditional present if cause contains failure) */
    tvSNHeader seqNumUplink;          /* To indicate UL Sequence Number 
                                         (Conditional present if cause contains success)*/
    tvSNHeader seqNumDnlink;          /* To indicate DL Sequence Number 
                                         (Conditional present if cause contains success)*/
}gtpuRelSapCnf;

/* structure used for CP Error Ind for Relay and GTP-U Interface */

typedef struct relayGtpuCpErrorIndTag
{
    UInt32 transactionId;  /* To indicate transaction identifier (Mandatory) */
}relayGtpuCpErrorInd;

/* structure used for CP Path Error Ind for Relay and GTP-U Interface */

typedef struct relayGtpuCpPathErrorIndTag
{ 
    tlvHeader transportAddr;          /* To identify transport Address (Mandatory)*/
}relayGtpuCpPathErrorInd;

#endif 

