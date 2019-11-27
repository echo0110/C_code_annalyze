/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacECIDMgr.h,v $
 *
 ****************************************************************************
 *
 *  File Description : This file contains all the data type information that are
 *  used by E-CID manager
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacECIDMgr.h,v $
****************************************************************************/
#ifndef _LTE_MAC_ECID_MANAGER_H_
#define _LTE_MAC_ECID_MANAGER_H_
/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacRRMInterface.h"
#include "lteSQueue.h"
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
#define VALUE_BIT_STRING 7
#define  MAC_UL_GRANT_E_CID_DUMMY_DCI0_LOAD 2
#define MAX_RETRY_FOR_LIST_INSERT   7
#define MIN_VALUE_ECID_MEAS_ID 1
#define MAX_VALUE_ECID_MEAS_ID 15
/* + SPR_8082 */
#define MAX_WAIT_DCI0_TTI 20
#define ENB_RX_TX_TARGET_SFN_MAX_VALUE 1280 
/* - SPR_8082 */
/* SPR_8672_FIX */
#define FLAG_RESET 0
#define PRESENT_IN_LIST 1
#define PRESENT_IN_FAILURE_LIST 2
/* SPR_8672_FIX */
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/*CA Changes start  */
extern LTE_LIST* dummyDCI0SendList_gp[MAX_NUM_CELL];
extern LTE_LIST* dummyDCI0SendFailureList_gp[MAX_NUM_CELL];
extern E_CID_REPORT_SEND_Q* eCidReportsQueue_gp[MAX_NUM_CELL];
/*CA Changes end  */

typedef enum ReportStatusT
{
    INVALID_STATUS_ECID_REPORTS = 0, 
    WAIT_FOR_REPORTS_TA_1_OR_AOA_AFTER_TA_2_RECEIVED,
    WAIT_FOR_REPORTS_TA_1_OR_AOA,
    WAIT_FOR_TA_2_REPORT,
    WAIT_FOR_TA_2_REPORT_ONLY_FOR_RELEASE_8_UE,
    REPORTS_RECEIVED
}ReportStatus;
typedef struct EcidReportNodeT
{
    LTE_SNODE eCidReportAnchor;
    /* +- SPR 18268 */
    UInt16 ueIndex;
    /* +- SPR 18268 */
    UInt8 responseCode;
    UInt8 requiredReports;
}EcidReportNode;
typedef struct DummyDCI0NodeT
{
    LTE_LIST_NODE nodeAnchor;
    UInt32 ueIndex;
    /* SPR 15909 fix start */
    tickType_t dummyDCI0ProcessTTI;
    /* SPR 15909 fix end */
    UInt8 retryCounter;
}DummyDCI0Node;


typedef enum ReportTypesRequiredT
{
    NO_REPORT_TYPE_REQUIRED = 0,
    TYPE_1_TA_REPORT_REQUIRED,
    TYPE_2_TA_REPORT_REQUIRED ,
    TYPE_1_2_TA_REPORTS_REQUIRED,
    AOA_REPORTS_REQUIRED,
    TYPE_1_TA_AOA_REPORTS_REQUIRED,
    TYPE_2_TA_AOA_REPORTS_REQUIRED,
    TYPE_1_2_TA_AOA_REPORTS_REQUIRED
}ReportTypesRequired;

/****************************************************************************
 * Functions declaration
 ****************************************************************************/
/*CA Changes start  */
void initECidMgr (UInt8 numCellsConfigured);
void processEcidReports (UInt8 numCellsConfigured);
/*CA Changes start  */

/* ++coverity--530-54652*/
/*CA Changes start  */
extern void cleanupECidQueue(InternalCellIndex internalCellIndex);
extern void deInitECidQueue(InternalCellIndex internalCellIndex);
extern void cleanAndDeInitECidQueue(InternalCellIndex internalCellIndex);
/*CA Changes end  */
/* ++coverity--530-54652*/
/* +- SPR 18268 */
void getEcidMeasuermentReports(UInt16 ueIndex,UInt8 requiredReports);
/*CA Changes start  */
void processECidMeasReportReq(RrmMacEcidMeasConfigReq *rrmMacEcidMeasConfigReq_p,UInt16 transactionId,InternalCellIndex internalCellIndex);
void putEntryInEcidMeasReportQueue(UInt16 ueIndex,UInt8 requiredReports,UInt8 responseCode, InternalCellIndex internalCellIndex);
/* +- SPR 18268 */

/* SPR_8672_FIX */
#define CHECK_AND_DELETE_NODE_FROM_DUMMY_DCI0_LIST(dummyDCI0Entry_p,dummyDCI0PresentFlag, internalCellIndex)\
{\
    if (dummyDCI0Entry_p != PNULL && (PRESENT_IN_LIST == dummyDCI0PresentFlag))\
    {\
        listDeleteNode(dummyDCI0SendList_gp[internalCellIndex],dummyDCI0Entry_p);\
        freeMemPool(dummyDCI0Entry_p);\
        dummyDCI0Entry_p = PNULL;\
    }\
    else if(dummyDCI0Entry_p != PNULL && (PRESENT_IN_FAILURE_LIST == dummyDCI0PresentFlag))\
    {\
        listDeleteNode(dummyDCI0SendFailureList_gp[internalCellIndex],dummyDCI0Entry_p);\
        freeMemPool(dummyDCI0Entry_p);\
        dummyDCI0Entry_p = PNULL;\
    }\
}
/*CA Changes end  */
/* SPR_8672_FIX */
#endif


