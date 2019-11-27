/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacPowerControl.h
 *
 ****************************************************************************
 *
 *  File Description : This header file contains declarations required for 
 *                     Power Control feature in LTE MAC.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * Revision 1.1.1  2012/07/20 14:51:27  gur31292
 * Changes related to DCI Format3A
 ****************************************************************************/

#ifndef LTE_MAC_POWER_CONTROL_H
#define LTE_MAC_POWER_CONTROL_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/* Indicate the default value with which tpcBitmap for DCI Format 3 can be 
   populated */
#define DEFAULT_VAL_FOR_DCI3 0x55555555

/* Indicate the default value with which tpcBitmap for DCI Format 3A can be 
   populated */
#define DEFAULT_VAL_FOR_DCI3A 0x0

#define INVALID_TPC_RNTI    0

/* No change in TPC value*/
#define TPC_NO_CHANGE   0

#define MAX_UE_SUPPORTED_FOR_DCI_3_OR_3A 32

/* Step size fror PUCCH BLER */
#define PUCCH_BLER_SURE_STEP_SIZE(internalCellIndex)    pucchBlerSureWeightage_g[internalCellIndex]
#define PUCCH_BLER_UNSURE_STEP_SIZE(internalCellIndex)   pucchBlerUnsureWeightage_g[internalCellIndex]

/* Timer value for TPC_TRIGGER_QUEUE_FOR_PUSCH expiry */

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/* DCI Format3A change start*/
/* Specify the last sent tcpValue was real or fake*/
typedef enum fakeTcpValueT
{
    FAKE_TPC_POWER_MINUS_1 = -1,
    FAKE_TPC_NO_VALUE,
    FAKE_TPC_POWER_PLUS_1
}FakeTcpValue;
/* DCI Format3A change end*/

/* Specify the current tpcScheduledState for any TPC RNTI */
typedef enum ScheduledStateT
{
    UNSET_SCHEDULED_STATE,
    SCHEDULED_FORMAT3,
    SCHEDULED_FORMAT3A

}ScheduledState;

/* Describe the node information that is stored in tpcTriggerQueueForPusch and 
 * tpcTriggerQueueForPucch */
typedef struct TpcTriggerQueueNodeT
{
    /* data type of anchor */
    LTE_SNODE nodeAnchor;

    /* Indicate the UE ID that is inserted into the tpcTriggerQueue. */
    UInt16 ueIndex;

    /* Indicate the expiry tick after which node can be processed */
    /* SPR 15909 fix start */
    tickType_t expiryTick;
    /* SPR 15909 fix end */

}TpcTriggerQueueNode;

/* This structure shall describe the node information that is stored in 
 * scheduledFormat3Or3AQueueForPusch and scheduledFormat3Or3AQueueForPucch*/
typedef struct ScheduledFormat3Or3AQueueNodeT
{
    LTE_SNODE nodeAnchor;

    /* Indicate the TPC RNTI that is inserted in Scheduled3/3A Queue */
    UInt16 tpcRntiIndex;

}ScheduledFormat3Or3AQueueNode;

/*structure for the node of TriggeredUEList queue */
typedef struct TriggeredUEListNodeT
{
    /*data type of anchor*/
    LTE_LIST_NODE triggeredUeAnchor;
    /*Index of UE */
    UInt16 ueIndex;
    /* HD FDD Changes Start */
    /* variable to indicate whether its is present in Fail queue or not */
    UInt8 isPresentInFailList;        
    /* HD FDD Changes End */
}TriggeredUEListNode;

/*structure for the node of ConfiguredUEList queue */
typedef struct ConfiguredUEListNodeT
{
    /*data type of anchor*/
    LTE_LIST_NODE configuredUeAnchor;
    /*Index of UE */
    UInt16 ueIndex;
}ConfiguredUEListNode;

/* Store the information related to TPC RNTI configured in the system.*/
typedef struct TpcRntiContextT
{
    /* Maintained during TPC RNTI configuration to represent whether this 
     * TPC-RNTI is configured for DCI Format 3 or 3A */
    /*coverity-530 CID-32300*/
    DCIFormat isConfiguredForDciFormat3Or3A;
    /*coverity-530 CID-32300*/

    /* Indicate the count of the UEs that are configured using this TPC RNTI. 
     * If TPC-RNTI is configured for DCI Format 3 then it can be maximum 16. 
     * Otherwise it can be maximu 32 */
    UInt8 configuredCount;

    /* Indicate the count of the UEs for which TPC is scheduled */
    UInt8 triggeredCount;

    /*HD FDD Changes Start*/
    UInt8 failTriggerCount;
    /*HD FDD Changes End*/

    /* Indicate the scheduled state of the tpcRnti */
    ScheduledState scheduledState;

    /* Indicate the tpcRntiIndex */
    UInt16 tpcRntiIndex;

    /* Inidcate the tpcRnti */
    UInt16 tpcRnti;
    
    /* The tick corresponding to the sub frame in which power control command 
     * shall be applicable. This tick shall be set whenever the TPC command is
     * sent to UE either in DCI 3/3A */
    /* SPR 15909 fix start */
    tickType_t appliedUlTick;
    /* SPR 15909 fix end */

    /* Indicate the UEs for which power control command is triggered to be sent
     * using DCI Format 3/3A. */
    LTE_LIST triggeredUeList;

    /* Indicate the UEs for which power control command is configured to be sent
     * using DCI Format 3A. */
    LTE_LIST configuredUeList;

    /*HD FDD Changes Start*/
    /* Indicate the UEs for which power control command is triggered but failed to sent
     * using DCI Format 3/3A. */
    LTE_LIST failTriggerUeList;
    /* +- SPR 22354 */
}TpcRntiContext;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* This function will update the tpcSentMap_g for PUCCH/PUSCH */
        /* SPR 15909 fix start */
void putEntryInTpcSentMap( tickType_t currentTick, UInt8 mapIdx,
        /* SPR 15909 fix end */
        UInt16 rnti, SInt8 powerCorrectionVal, 
        ErrIndPowerControlType powerCtrlType,
        InternalCellIndex internalCellIndex);

/* This function will reset the tpcSentMap_g for PUCCH/PUSCH */
/* SPR 23393 Changes Start */
 void resetEntryInTpcSentMap( UInt8 mapIdx ,InternalCellIndex internalCellIndex);
/* SPR 23393 Changes End */

#endif /*LTE_MAC_POWER_CONTROL_H*/
