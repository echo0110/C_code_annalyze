/******************************************************************************
 *
 *   ARICENT -
 *
 *   Copyright (C) 2015 Aricent Inc. All Rights Reserved.
 * 
 ******************************************************************************
 * 
 *   File Name   : lteX2uRelay.c
 * 
 *   Description : This file contains implementation of  X2U Relay module 
 *                 initialization related APIs.
 *
 *****************************************************************************/
/******************************************************************************
 * Project Includes
 *****************************************************************************/
#ifdef ENDC_ENABLED
#include "lteX2uRelay.h"
#include "logging.h"
#include "lteMisc.h"
#include "lteRelayOamInterface.h"
#include "lteRelay.h"
#include "lteRelayGtpuSap.h"
#include "lteRelayS1Sap.h"
#include "lteLayer2RlcPdcpInterface.h"
#include "lteRelayAdapter.h"
#include "lteRelayCommon.h"


/* Global X2U Relay context */
LTE_CIRCQUEUE     rlcTxDeliveryIndQ_g;
x2uRelayContext   x2uRelayContext_g;
extern UInt8 * dlTempMsgPtr;
extern tickType_t pdcpCurrentTime_g;
extern void lteFreeEgtpuBuff(void * ptr1, void * ptr2);
#define X2U_DELETE_UE_TIMER 200
/******************************************************************************
 * Private Definitions
 *****************************************************************************/
static void updateUeInActiveList(
        UInt16 ueIndex,
        UInt16 *activeUeIdx_p,
        UInt16 *activeUePos_p);
/****************************************************************************
 * FUNCTION NAME   : initX2uRelayContext
 * DESCRIPTION     : This funtion initializes the X2U Relay internal context
 * RETURNS         : None
 ****************************************************************************/
void initX2uRelayContext()
{
    LOG_GTPUPR_INFO(X2U_RELAY,
                   "X2U Relay Context Initialization \n");

    /* Set the flag to indicate that X2U Relay module is initialized */
    x2uRelayContext_g.isInitialized = LTE_TRUE;

    /* Create Memory pool for X2U UE context*/
    createMemPool(sizeof(X2uUeContext), MAX_UE_SUPPORTED);
    /* Create Memory pool for X2U DRB entity context */
    createMemPool(sizeof(X2uDrbEntityContext), MAX_UE_SUPPORTED*(LTE_DRB_LC_ID_HIGH_BOUND - LTE_DRB_LC_ID_LOW_BOUND + 1));
 
    /* Create Memory pool for lost SN storage context*/
    createMemPool(sizeof(X2uLostSN), MAX_UE_SUPPORTED*MAX_NUM_OF_LOST_X2U_PKT_RANGES);

    /* Create Memory pool for RLC status storage context*/
    createMemPool(sizeof(RlcStatusBuffer), MAX_UE_SUPPORTED);

    /* Create Memory pool for RLC Delivery indication queue*/
    createMemPool(sizeof(RlcDeliveryInd), MAX_UE_SUPPORTED);

    circQInit((LTE_CIRCQUEUE*)&rlcTxDeliveryIndQ_g, MAX_UE_SUPPORTED * MAX_LOGICAL_CHANNEL );

    /* Reset the count of active UEs in global context */
    x2uRelayContext_g.activeUeCount = 0;
}
/****************************************************************************
 * FUNCTION NAME   : x2urelaycreatebearercontext
 * DESCRIPTION     : this funtion creates x2u internal bearer context
 * RETURNS         : x2u_success/x2u_failure
 ****************************************************************************/
UInt8 x2uRelayUpdateDataFwdTimerAndThresholds(
      x2uDeliveryStatusTriggerThreshold deliveryStatusTimer,
      UInt16 dataForwardingGuardTimerInterval, UInt8 isGtpuNRHeaderEnabled)
{
    if((MIN_DELIVERY_STATUS_THRESHOLD > deliveryStatusTimer.x2uDeliveryStatusTriggerTimer) ||
       (MIN_DELIVERY_STATUS_THRESHOLD > deliveryStatusTimer.x2uDeliveryStatusTriggerPktCnt) ||
       (MIN_DELIVERY_STATUS_THRESHOLD > deliveryStatusTimer.x2uDeliveryStatusTriggerPktSize) ||
       (MAX_DELIVERY_STATUS_THRESHOLD < deliveryStatusTimer.x2uDeliveryStatusTriggerTimer) ||
       (MAX_DELIVERY_STATUS_THRESHOLD < deliveryStatusTimer.x2uDeliveryStatusTriggerPktCnt) ||
       (MAX_DELIVERY_STATUS_THRESHOLD < deliveryStatusTimer.x2uDeliveryStatusTriggerPktSize))
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,"Invalid Delivery Status Timer" 
        "Configuration TriggerTimer:%u PktCount:%u PktSize:%u",
        deliveryStatusTimer.x2uDeliveryStatusTriggerTimer,
        deliveryStatusTimer.x2uDeliveryStatusTriggerPktCnt,
        deliveryStatusTimer.x2uDeliveryStatusTriggerPktSize);
        return X2U_FAILURE;
    }
    x2uRelayContext_g.deliveryStatusTriggerTimer = 
    deliveryStatusTimer.x2uDeliveryStatusTriggerTimer;

    x2uRelayContext_g.deliveryStatusTriggerPktCnt = 
    deliveryStatusTimer.x2uDeliveryStatusTriggerPktCnt;

    x2uRelayContext_g.deliveryStatusTriggerPktSize = 
    deliveryStatusTimer.x2uDeliveryStatusTriggerPktSize;

    x2uRelayContext_g.dataForwardingGuardTimerInterval = 
    dataForwardingGuardTimerInterval;

    GET_X2U_CURRENT_TICK(x2uRelayContext_g.timerStartTTI);
    x2uRelayContext_g.timerEndTTI = x2uRelayContext_g.timerStartTTI +
                                    x2uRelayContext_g.deliveryStatusTriggerTimer;

    x2uRelayContext_g.isGtpuNRHeaderEnabled = isGtpuNRHeaderEnabled;                                 
    return X2U_SUCCESS;
}
/****************************************************************************
 * FUNCTION NAME   : x2urelaycreatebearercontext
 * DESCRIPTION     : this funtion creates x2u internal bearer context
 * RETURNS         : x2u_success/x2u_failure
 ****************************************************************************/
UInt8 x2uRelayCreateBearerContext(
      UInt16 ueIndex, 
      UInt8  lcId, 
      UInt8  snSize, 
      UInt8  rlcMode) 
{
    X2uUeContext *x2uUeContext_p = PNULL;
    LP_X2uDrbEntityContext x2uDrbEntity_p = PNULL;

    /*Validate UE Index*/
    if (ueIndex >= MAX_UE_SUPPORTED)
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,"Invalid UeIndex:%u",ueIndex);
        return X2U_FAILURE;
    }
    /*Validate LC ID*/
    if ((LTE_DRB_LC_ID_HIGH_BOUND < lcId ) || (LTE_DRB_LC_ID_LOW_BOUND > lcId))
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,"Invalid lcId:%u",lcId);
        return X2U_FAILURE;
    }
    /*Check the UE Context for the given ueIndex*/
    if(PNULL == x2uRelayContext_g.x2uRelayUeInfo[ueIndex].x2uUeContext_p)
    {
        x2uUeContext_p = (X2uUeContext*)getMemFromPool(sizeof(X2uUeContext),NULL);
        if(PNULL == x2uUeContext_p)
        {
            LOG_GTPUPR_WARNING(X2U_RELAY,
                    "Memory Allocation failed for X2U Relay ueContext"
                    "ueIndex [%d] lcId [%d] \n",ueIndex,lcId);

            return X2U_FAILURE;
        }
        memSet(x2uUeContext_p,0,sizeof(X2uUeContext));
        /* Store UE index */
        x2uUeContext_p->ueIndex = ueIndex;
        x2uRelayContext_g.x2uRelayUeInfo[ueIndex].x2uUeContext_p = x2uUeContext_p;

        /* Increase the count of active UE contexts */
        x2uRelayContext_g.activeUeCount += 1;

        updateUeInActiveList(ueIndex,x2uRelayContext_g.activeUeIdx,
                x2uRelayContext_g.activeUePos);
    }
    x2uUeContext_p = GET_X2U_UE_CONTEXT(ueIndex);
    x2uDrbEntity_p = (LP_X2uDrbEntityContext)getMemFromPool(sizeof(X2uDrbEntityContext),NULL);

    if(PNULL == x2uDrbEntity_p)
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,
                "Memory Allocation failed for X2U Relay DRB entity"
                "ueIndex [%d] drbId [%d] \n",ueIndex,lcId);

        return X2U_FAILURE;
    }
    memSet(x2uDrbEntity_p,0,sizeof(X2uDrbEntityContext));

    /* Store LcId in list of active LcId
     * maintained in UE context */
    x2uUeContext_p->activeLc[x2uUeContext_p->activeLcCount] = lcId;

    /* Increase the count of active LCs for this UE */
    x2uUeContext_p->activeLcCount += 1;

    /*Set SN size as 12 bit*/
    x2uDrbEntity_p->snSize = snSize; 
    x2uDrbEntity_p->rlcMode = rlcMode; 
    /* Set Maximum buffer size in bytes*/
    x2uUeContext_p->dataFwdGuardTimerExpiryTTI = 0;
    x2uUeContext_p->tickAss = 0;
    x2uUeContext_p->ueState = X2U_UE_CONNECTED;

    x2uDrbEntity_p->drbDesiredBufferSize    = MAX_X2U_BUFFER_SIZE;
    x2uDrbEntity_p->isFirstPktFromMeNB      = X2U_TRUE;
    x2uDrbEntity_p->discardTimer            = DISCARD_TIMER_1500MS;
    x2uDrbEntity_p->finalFrameInd           = X2U_FALSE;
    x2uDrbEntity_p->desiredBufferResetTimer           = DESIRED_BUFFER_SIZE_RESET_TIMER;
    x2uRelayContext_g.x2uRelayUeInfo[ueIndex].drbEntityContexts[GET_LC_INDEX(lcId)] = x2uDrbEntity_p;
    queueInit( &(x2uDrbEntity_p->x2uTxStorageQ) );
    listInit(&(x2uDrbEntity_p->x2uSLostSnRangeList_g));

    LOG_GTPUPR_INFO(X2U_RELAY,"X2U Bearer Creation Successful for UE:%u LcId:%u",
            ueIndex,lcId);
    return X2U_SUCCESS;
}
/****************************************************************************
 * FUNCTION NAME   : x2urelayDeletebearercontext
 * DESCRIPTION     : this funtion deletes x2u internal UE bearer 
 * RETURNS         : x2u_success/x2u_failure
 ****************************************************************************/
UInt8 x2uRelayDeleteBearerContext(
       UInt16 ueIndex, 
       UInt8  lcId) 
{
    UInt8     lcIndex = GET_LC_INDEX(lcId);
    UInt16    index   = 0;
    X2uUeContext *ueContext_p = PNULL;
    LP_X2uDrbEntityContext drbEntity_p = PNULL;

    /*Validate UE Index*/
    if (ueIndex >= MAX_UE_SUPPORTED)
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,"Invalid UeIndex:%u",ueIndex);
        return X2U_FAILURE;
    }
    /*Validate LC ID*/
    if ((LTE_DRB_LC_ID_HIGH_BOUND < lcId ) || (LTE_DRB_LC_ID_LOW_BOUND > lcId))
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,"Invalid lcId:%u",lcId);
        return X2U_FAILURE;
    }
    ueContext_p = GET_X2U_UE_CONTEXT(ueIndex);;
    if(PNULL == ueContext_p)
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,
                "X2U Relay ueContext doesn't exists"
                "ueIndex [%d] lcId [%d] \n",ueIndex,lcId);

        return X2U_FAILURE;

    }
    /* Traverse the list of active LCs for this UE and delete the
     *      * LC from the list */
    for (index = 0; index < ueContext_p->activeLcCount; index++)
    {
        if (ueContext_p->activeLc[index] == lcId)
        {
            /* Copy the lcId from the last array index to this index */
            ueContext_p->activeLc[index]
                = ueContext_p->activeLc[ueContext_p->activeLcCount-1];
            break;
        }
    }
    /* Decrease the count of active LCs for this UE */
    ueContext_p->activeLcCount -= 1;
    drbEntity_p = GET_UE_DRB_CONTEXT(ueIndex,lcId);
    if(PNULL == drbEntity_p)
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,
                "X2U Relay DRB context doesn't exists"
                "ueIndex [%d] lcId [%d] \n",ueIndex,lcId);
        return X2U_FAILURE;
    }
    /*Free the TX and RX queue of this LC*/
    freeDrbEntityTxRxQ(drbEntity_p);

    /* Update the total available UE buffer size */
    freeMemPool(drbEntity_p);
    x2uRelayContext_g.x2uRelayUeInfo[ueIndex].drbEntityContexts[lcIndex] = PNULL;

    if(!ueContext_p->activeLcCount)
    {
        updateUeInActiveList(ueIndex,x2uRelayContext_g.activeUeIdx,
        x2uRelayContext_g.activeUePos);

        freeMemPool(ueContext_p);
        x2uRelayContext_g.x2uRelayUeInfo[ueIndex].x2uUeContext_p = PNULL;
        x2uRelayContext_g.activeUeCount -= 1;
    }
    return X2U_SUCCESS;
}
/****************************************************************************
 * FUNCTION NAME   : updateUeInActiveList
 * DESCRIPTION     : this funtion add the UE index and its position to active
 * RETURNS         : None
 ****************************************************************************/
static void updateUeInActiveList(
     UInt16 ueIndex, 
     UInt16 *activeUeIdx_p, 
     UInt16 *activeUePos_p)
{
    UInt16 uePosition = INVALID_UE_ID;
    UInt16 lastUeIdx  = INVALID_UE_ID;
    if(X2U_FALSE == activeUePos_p[ueIndex])
    {
        activeUeIdx_p[x2uRelayContext_g.activeUeCount - 1] = ueIndex;
        activeUePos_p[ueIndex] = x2uRelayContext_g.activeUeCount;
    }
    else
    {
        uePosition = activeUePos_p[ueIndex];

        if(uePosition == x2uRelayContext_g.activeUeCount)
        {
            activeUeIdx_p[uePosition - 1] = 0;
            activeUePos_p[ueIndex] = 0;
        }
        else
        {
        lastUeIdx = activeUeIdx_p[x2uRelayContext_g.activeUeCount - 1];
        activeUeIdx_p[uePosition - 1] = lastUeIdx;
        activeUeIdx_p[x2uRelayContext_g.activeUeCount - 1] = 0;
        
        activeUePos_p[lastUeIdx] = uePosition;
        activeUePos_p[ueIndex] = 0;
        }
    }
}
/****************************************************************************
 * FUNCTION NAME   : allocateAndCreateDataReq
 * DESCRIPTION     : this funtion allocate the memory for new data req and fill
 *                   DRB info
 * RETURNS         : LP_X2uDataReq
 ****************************************************************************/
LP_X2uDataReq allocateAndCreateDataReq(
                     UInt16 ueIndex, 
                     UInt16 lcId, 
                     UInt16 size)
{
    LP_X2uDataReq dataReq_p = PNULL;

    dataReq_p = (LP_X2uDataReq)getMemFromPool( sizeof(X2uDataReq), PNULL);
    if(PNULL == dataReq_p)
    {
        LOG_GTPUPR_ERROR(X2U_RELAY,"Memory Allocation Failed for UE:%u Size:%u",
        ueIndex,sizeof(X2uDataReq));
        return PNULL;
    }
    dataReq_p->commDataReq.ueIndex = ueIndex;
    dataReq_p->commDataReq.lcId = lcId;
    dataReq_p->commDataReq.size = size;
    dataReq_p->data_p = PNULL;
    GET_X2U_CURRENT_TICK(dataReq_p->tickAssociated);
    dataReq_p->commDataReq.count = 0;

    return dataReq_p;
}
/****************************************************************************
 * FUNCTION NAME   : initX2uRelayContext
 * DESCRIPTION     : this funtion will check for lost packets over X2U and store
 *                   the start and end of lost packet ranges if any found
 * RETURNS         : None
 ****************************************************************************/
void updateAndInsterInList(
     LP_X2uDrbEntityContext drbEntity_p , 
     UInt16                 receivedX2U_SN)
{
        LP_X2uLostSN lostSN_p = PNULL;
        LP_X2uLostSN tempLostSN_p = PNULL;
        LP_X2uLostSN prevNode_p   = PNULL;

        tempLostSN_p = (LP_X2uLostSN)getFirstListNode(&(drbEntity_p->x2uSLostSnRangeList_g));
        while(tempLostSN_p)
        {
             /*When SN received in out of order that was already being considered
              * as lost packet, the stored lost range list need to updated and
              * mark that packet as received  */
             if((tempLostSN_p->x2uLostSN_Start < receivedX2U_SN) &&
                 (tempLostSN_p->x2uLostSN_End > receivedX2U_SN))
             {
                 lostSN_p = (LP_X2uLostSN)getMemFromPool( sizeof(X2uLostSN), PNULL);
                 if(PNULL == lostSN_p)
                 {
                     LOG_GTPUPR_ERROR(X2U_RELAY,"Memory Allocation Failed for x2uLostSnStorageQ");
                     return;
                 }
                 /*update start and end lost range*/
                 tempLostSN_p->x2uLostSN_End = receivedX2U_SN - 1;
                 lostSN_p->x2uLostSN_Start = receivedX2U_SN + 1;
                 lostSN_p->x2uLostSN_End = tempLostSN_p->x2uLostSN_End;
                 listInsertNode(&(drbEntity_p->x2uSLostSnRangeList_g), &(lostSN_p->nodeAnchor));
             }
             else if((tempLostSN_p->x2uLostSN_Start == receivedX2U_SN) || 
                 (tempLostSN_p->x2uLostSN_End == receivedX2U_SN))
             {
                 /*Delete the node from lost range list if start and end SN is 
                  * same as received SN as out of order*/
                 if(tempLostSN_p->x2uLostSN_Start == tempLostSN_p->x2uLostSN_End)
                 {
                     prevNode_p = tempLostSN_p;
             		 tempLostSN_p = (LP_X2uLostSN)getNextListNode(&tempLostSN_p->nodeAnchor);
                 	 listDeleteNode(&(drbEntity_p->x2uSLostSnRangeList_g),&(prevNode_p->nodeAnchor));
                 	 freeMemPool((void *)prevNode_p);
                 	 continue;

                 }
                 /*SN received in out of order, so adjust the startSN of lost
                  * range*/
                 else if(tempLostSN_p->x2uLostSN_Start == receivedX2U_SN)
                 {
                     tempLostSN_p->x2uLostSN_Start = receivedX2U_SN + 1;
                 }
                 /*SN received in out of order, so adjust the endSN of lost
                  * range*/
                 else if(tempLostSN_p->x2uLostSN_End == receivedX2U_SN)
                 {
                     tempLostSN_p->x2uLostSN_End = receivedX2U_SN - 1;
                 }
             }
             else
             {
                 /*received SN is not fall in to any of the lost range 
                  * stored in list*/
             }

             tempLostSN_p = (LP_X2uLostSN)getNextListNode(&tempLostSN_p->nodeAnchor);
        }
}
/****************************************************************************
 * FUNCTION NAME   : checkForLostPacketRange
 * DESCRIPTION     : this funtion will check for lost packets over X2U and store
 *                   the start and end of lost packet ranges if any found
 * RETURNS         : None
 ****************************************************************************/
void checkForLostPacketRange(
            LP_X2uDrbEntityContext drbEntity_p, 
            UInt32                 receivedX2U_SN)
{
    LP_X2uLostSN lostSN_p = PNULL;

    if(drbEntity_p->isFirstPktFromMeNB)
    {
        drbEntity_p->nextExpected_SN = receivedX2U_SN + 1;
        drbEntity_p->isFirstPktFromMeNB = X2U_FALSE;
        drbEntity_p->snWindowStartIndex = (ceil_wrapper(receivedX2U_SN/NRU_SN_WINDOW_SIZE))*NRU_SN_WINDOW_SIZE;
        drbEntity_p->snWindowEndIndex = drbEntity_p->snWindowStartIndex + NRU_SN_WINDOW_SIZE;

        return;
    }
    if(receivedX2U_SN == drbEntity_p->nextExpected_SN)
    {
    	return;
    }
    if(receivedX2U_SN > drbEntity_p->snWindowEndIndex)
    {
        /*update the SN window */
        drbEntity_p->snWindowStartIndex += NRU_SN_WINDOW_SIZE;
        drbEntity_p->snWindowEndIndex = drbEntity_p->snWindowStartIndex + NRU_SN_WINDOW_SIZE;
    }
    else if (receivedX2U_SN < drbEntity_p->snWindowStartIndex)
    {
        /*this is in case of SN wrapAround condition, update the start and end
         * index of SN window*/
        if(receivedX2U_SN < NRU_SN_WINDOW_SIZE)
        {
            drbEntity_p->snWindowStartIndex = 0;
            drbEntity_p->snWindowEndIndex = NRU_SN_WINDOW_SIZE;

        }
        else
        {
            updateAndInsterInList(drbEntity_p,receivedX2U_SN);           
            drbEntity_p->lostX2uRangeReported++;

        }
    }
    if((receivedX2U_SN > drbEntity_p->snWindowStartIndex) && 
        (receivedX2U_SN < drbEntity_p->snWindowEndIndex))
    {
        /*SN received is less than expected SN the it means
         * SN received in out of order so update the lost 
         * range accordingly*/
        if(receivedX2U_SN < drbEntity_p->nextExpected_SN)
        {
           updateAndInsterInList(drbEntity_p,receivedX2U_SN);           
        }
        else
        {
            /*Here mark the packets as lost between expected SN and received SN*/
            lostSN_p = (LP_X2uLostSN)getMemFromPool( sizeof(X2uLostSN), PNULL);
            if(PNULL == lostSN_p)
            {
                LOG_GTPUPR_ERROR(X2U_RELAY,"Memory Allocation Failed for x2uLostSnStorageQ");
                return;
            }
            lostSN_p->x2uLostSN_Start = drbEntity_p->nextExpected_SN;
            lostSN_p->x2uLostSN_End = receivedX2U_SN - 1;
            listInsertNode(&drbEntity_p->x2uSLostSnRangeList_g, &lostSN_p->nodeAnchor);
        }
        drbEntity_p->lostX2uRangeReported++;

    }
    if(HIGHEST_X2U_SN == receivedX2U_SN)
    {
        drbEntity_p->nextExpected_SN = 0;
        return;
    }
    drbEntity_p->nextExpected_SN = receivedX2U_SN + 1;
}

/****************************************************************************
 * FUNCTION NAME   : prX2UdlDataReq
 * DESCRIPTION     : this funtion extract the PDCP DL data received from PR
 * RETURNS         : None
 ****************************************************************************/
UInt8 prX2UdlDataReq(
      UInt16 ueIndex, 
      UInt8  lcId, 
      UInt8  *data_p, 
      UInt32 size)
{
    UInt8  pos = 0;
    UInt8  pduType = 0;
    UInt8 reportPolling = 0;
    UInt32 receivedX2U_SN = 0;
    UInt16 dataLen = 0;
    UInt16 ranContainerLen = 0;
    X2uUeContext *ueContext_p = PNULL;
    LP_X2uDataReq dataReq_p = PNULL;
    LP_X2uDrbEntityContext drbEntity_p = PNULL;

    /************************************************************************************************
     * 3GPP TS 38.425 V15.4.0 - Sec 5.5.2.1  DL USER DATA                                           *
     * (PDU Type 0)                                                                                 *
     ************************************************************************************************
     *   7   *   6   *   5   *   4   *   3        *   2         *   1          *   0         * Byte *
     ************************************************************************************************
     * PDU Type (=0)                 * spare         DL            DL           Report       *      *
     *                               *             Discard       Flush          Polling      *  1   *
     *                               *              block         SN                         *      *
     ************************************************************************************************
	 * Spare                         * Report     User data    Assistance     retransmission *      *
     *                               * Delivered  Existence    info report       on flag     *  1   *
     *                               *             flag        polling flag                  *      *
     ************************************************************************************************
     *                         NR-U Sequence Number                                          *  3   *
     ************************************************************************************************
     *                       DL discard NR PDCP PDU SN                                       *0 or 3*
     ************************************************************************************************
     *                      DL discard Number of blocks                                      *0 or 1*
     ************************************************************************************************
     *               DL discard NR PDCP PDU SN start (first block)                           *0 or 3*
     ************************************************************************************************
     *                   Discarded Block size (first block)                                  *0 or 1*
     ************************************************************************************************
     *              DL discard NR PDCP PDU SN start (last block)                             *0 or 3*
     ************************************************************************************************
     *                      Discarded Block size (block)                                     *0 or 1*
     ************************************************************************************************
     *                        DL report NR PDCP PDU SN                                       *0 or 3*
     ************************************************************************************************
     *                                Padding                                                *0-3   *
     ************************************************************************************************
     *                               PDCP PDU                                               *Payload*
     ************************************************************************************************/

    /*In current release only UM mode is suppored so above header having only following parameters
      1.Report polling 
      2.NR-U Sequence Number 
      3.Padding
      4.PDCP PDU*/

    if(x2uRelayContext_g.isGtpuNRHeaderEnabled)
    {
    	if((size < MIN_RAN_CONTAINER_EXTN_LEN) || (PNULL == data_p))
    	{
        	LOG_GTPUPR_WARNING(X2U_RELAY,"Either Invalid MsgLen:%u or Msg_p:%p",size,data_p);
        	return X2U_FAILURE;
    	}
    	/*Received container is in multiple of 4 bytes*/
    	ranContainerLen = data_p[pos++]*4;

    	dataLen = size - ranContainerLen;
    	if(!dataLen)
    	{
        	LOG_GTPUPR_WARNING(X2U_RELAY,"Invalid DataLen:%u",dataLen);
        	return X2U_FAILURE;
    	}
        /*Get pdu type and validate*/
    	pduType = data_p[pos] >> 4;
    	if(pduType != DL_USER_DATA_PDU_TYPE)
    	{
        	LOG_GTPUPR_WARNING(X2U_RELAY,"Invalid PDU Type:%u",pduType);
        	return X2U_FAILURE;
    	}
    	/*Get report polling flag*/
    	reportPolling = data_p[pos] & 1;
    	pos +=2;
    	receivedX2U_SN = LTE_GET_U24BIT(data_p + pos);
    	pos = ranContainerLen;
    }
    else
    {
    	dataLen = size;
    }
    ueContext_p = x2uRelayContext_g.x2uRelayUeInfo[ueIndex].x2uUeContext_p;
    if(PNULL == ueContext_p)
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,"UE Context Doesn't exists for UeId:%u",ueIndex);
        return X2U_FAILURE;
    }
    drbEntity_p = x2uRelayContext_g.x2uRelayUeInfo[ueIndex].drbEntityContexts[GET_LC_INDEX(lcId)];
    if(PNULL == drbEntity_p)
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,"RAB Entity Doesn't exists for" 
                "UeId:%u lcId:%u",ueIndex,lcId);
        return X2U_FAILURE;
    }
    if(drbEntity_p->drbDesiredBufferSize > size)
    {
    	drbEntity_p->drbDesiredBufferSize -= size;
    }
    else
    {
    	drbEntity_p->drbDesiredBufferSize = 0;
        LOG_GTPUPR_WARNING(X2U_RELAY,"Desired buffer size met for UeId:%u",ueIndex);
    	return X2U_FAILURE;
    }
    if(PNULL != (dataReq_p = allocateAndCreateDataReq(ueIndex,lcId,dataLen)))
    {
        /* Now Filling the ueIndex and LC-ID field to packet */
        dataReq_p->data_p = (UInt8 *)msgAlloc(PNULL, 0, 0, 0);
        if ( PNULL == dataReq_p->data_p )
        {
            LOG_GTPU_MSG(PR_DL_DATA_MSG_ALLOC_FAIL, LOGWARNING, PR_GTPU_DL_DATA,
                    relayGtpuCurrentTick_g, __LINE__,
                    0,0,0,0, 0,0, __func__,"");
            return RELAY_GTPU_FAILURE;
        }

        if(x2uRelayContext_g.isGtpuNRHeaderEnabled)
        {
        	qvMsgInsertExternal( dataReq_p->data_p, 0, (data_p + pos), dataLen,
                	lteFreeEgtpuBuff, dlTempMsgPtr
#ifdef CSPL_LEAK_DEBUG_LITE
                	, __func__, __LINE__
#endif
                	);
    	}
        else
        {
        	qvMsgInsertExternal( dataReq_p->data_p, 0, data_p, dataLen,
                	lteFreeEgtpuBuff, dlTempMsgPtr
#ifdef CSPL_LEAK_DEBUG_LITE
                	, __func__, __LINE__
#endif
                	);
        }
    	if(x2uRelayContext_g.isGtpuNRHeaderEnabled)
    	{
       		if(drbEntity_p->snSize == PDCP_SN_12_BIT)
       		{

           		dataReq_p->commDataReq.sn = ( ( (UInt32) data_p[pos] & NR_PDCP_TS_PDU_DRB_SN_SMALL_MASK) <<8)
         	   		| data_p[pos+1];

       		}
       		else if(drbEntity_p->snSize == PDCP_SN_18_BIT)
       		{

        		dataReq_p->commDataReq.sn = ( ( (UInt32) data_p[pos] & NR_PDCP_TS_PDU_DRB_SN_BIG_MASK_HI)<<16)
         	       	| (UInt32)data_p[pos+1]<<8 | data_p[pos+2];

       		}
    		checkForLostPacketRange(drbEntity_p,receivedX2U_SN);
    		ENQUEUE_X2U_TX_STORAGE_Q(drbEntity_p,dataReq_p);
    	}

    	if(X2U_UE_CONNECTED == ueContext_p->ueState)
    	{
#ifdef PDCP_GTPU_INTF
        	sendDatatoRlc(
                	lcId,
                	ueIndex,
                	dataLen,
                	sizeof(X2uDataReq),
                	X2U_RLC_REQ_ACK_TRUE,
                	X2U_RLC_TRANSACTION_ID_VALID,
                	dataReq_p->data_p,
                	dataReq_p,
                	dataReq_p->tickAssociated);
#else
        	rlcUeDataReq(ueIndex,
                	lcId, 
                	dataReq_p->data_p, 
                	dataLen,
                	X2U_RLC_TRANSACTION_ID_VALID, 
                	dataReq_p, 
                	drbEntity_p->rlcMode, 
                	dataReq_p->tickAssociated,
                	X2U_FALSE, 
                	X2U_FALSE,
                	X2U_FALSE);
#endif
    	}
        /* coverity_<142773>_fix_start */
        drbEntity_p->highestTransmittedSN = dataReq_p->commDataReq.sn;
        /* coverity_<142773>_fix_end */
    }
    else
    {

    	LOG_GTPUPR_WARNING(X2U_RELAY, "Alloc and Create Data Req Failed ");
    }
    if(reportPolling)
    {
    	sendDeliveryStatusInd(ueIndex, lcId,drbEntity_p);
    }
    x2uRelayContext_g.totalPacketCount++;
    x2uRelayContext_g.totalPacketSize += dataLen;
    return X2U_SUCCESS;
}
/****************************************************************************
 * Function Name  : pdcpFreeTxDataReq
 * Inputs         : LP_PdcpDataReq *dataReq_p   
 * Outputs        : None.
 * Returns        : None.
 * Description    : To free LP_PdcpDataReq
 ****************************************************************************/ 
void pdcpFreeTxX2UDataReq(LP_X2uDataReq dataReq_p)
{
    if(PNULL == dataReq_p)
    {
    	LOG_GTPUPR_WARNING(X2U_RELAY, "PDCP TX data req NULL ");

    }
    else
    {
        if(PNULL != dataReq_p->data_p)
        {
          //  msgFree(dataReq_p->data_p);
            dataReq_p->data_p = PNULL;
        }

        freeMemPool(dataReq_p);
    }
}

/****************************************************************************
 * FUNCTION NAME   : triggerDeliveryStatusReport
 * DESCRIPTION     : this funtion trigger the status report
 * RETURNS         : None
 ****************************************************************************/
void triggerDeliveryStatusReport()
{
    tickType_t currentTick = 0;
    
    processRlcDeliveryStatusReport();

    GET_X2U_CURRENT_TICK(currentTick);

    if((x2uRelayContext_g.totalPacketCount >= 
                x2uRelayContext_g.deliveryStatusTriggerPktCnt) || 
            (x2uRelayContext_g.totalPacketSize >=
             x2uRelayContext_g.deliveryStatusTriggerPktSize))
    {
        prepareAndSendDeliveryStatus(currentTick);
        UPDATE_AND_RESET_TRIGGER_PARAMS(currentTick);
    }
    else if(currentTick == x2uRelayContext_g.timerEndTTI)
    {
        prepareAndSendDeliveryStatus(currentTick);
        UPDATE_AND_RESET_TRIGGER_PARAMS(currentTick);
    }
    resetDesiredBufferSize(currentTick);
    processDiscardPdcpSdu(currentTick);
    return;
}
/****************************************************************************
 * FUNCTION NAME   : prepareAndSendDeliveryStatus
 * DESCRIPTION     : this funtion prepare and send delivery status report + UL
 *                   data to packetRelay
 * RETURNS         : None
 ****************************************************************************/
void prepareAndSendDeliveryStatus(tickType_t currentTick)
{
    UInt8           ueCount = 0;
    UInt16          ueIndex = 0;
    UInt16          lcId = 0;
    UInt16          lcCount = 0;
    LP_X2uUeContext ueContext_p = PNULL;
    LP_X2uDrbEntityContext drbEntity_p = PNULL;
    
        ueCount = x2uRelayContext_g.activeUeCount;
    while(ueCount)
    {
    	ueIndex = x2uRelayContext_g.activeUeIdx[ueCount -  1];
        ueContext_p = GET_X2U_UE_CONTEXT(ueIndex);
        if((PNULL != ueContext_p) && (X2U_UE_CONNECTED == ueContext_p->ueState))
        {
            for(lcCount = 0; lcCount < ueContext_p->activeLcCount; lcCount++)
            {
            	lcId = ueContext_p->activeLc[lcCount];
                drbEntity_p = GET_UE_DRB_CONTEXT(ueIndex,lcId);
                if((PNULL != drbEntity_p) && (X2U_TRUE == drbEntity_p->isStatusReportAvailable) &&
                    (X2U_FALSE == drbEntity_p->finalFrameInd))
                {
                	sendDeliveryStatusInd(ueIndex,lcId, drbEntity_p);

                }
                lcId++;
            }
        }
        ueCount--;
    }
}
void sendDeliveryStatusInd(UInt16 ueIndex, UInt8 lcId, LP_X2uDrbEntityContext drbEntity_p)
{
	UInt8           count = 0;
    UInt8           reportCount = 0;;
    UInt8           *gtpuMsg_p = PNULL;
    UInt8           lostReportedCount = 0;
	UInt8 			lcIndex = lcId - LTE_DRB_LC_ID_LOW_BOUND;
    UInt16          msgLen = 0;
    UInt16          ranContainerLen = 0;
    UInt32          current = CSPL_HDR;
    UInt32          tunnelId = 0;
    LP_X2uLostSN    x2uLostSN_p  = PNULL;
	/************************************************************************************************
     * 3GPP TS 38.425 V15.4.0 - Sec 5.5.2.2 DL DATA DELIVERY STATUS                                 * 
     * (PDU Type 1)                                                                                 *
     ************************************************************************************************
     *   7   *   6   *   5   *   4   *   3        *   2         *   1          *   0         * Byte *
     ************************************************************************************************
     * PDU Type (=1)                 * Highest       Highest      Final           lost       *      *
     *                               * Trans NR      Delivered    Frame          Packet      *  1   *
     *                               * PDCP SN ind   PDCP SN ind   ind            report     *      *
     ************************************************************************************************
	 * Spare                         * data rate     Retrans     delivered       casuse      *      *
     *                               * ind           PDPC SN     retrans PDCP     report     *  1   *
     *                               *                ind        SN ind                      *      *
     ************************************************************************************************
     *                  Desired buffer size for the data radio bearer                        *  4   *
     ************************************************************************************************
     *                           Desired Data Rate                                           *0 or 4*
     ************************************************************************************************
     *               Number of lost NR-U Sequence Number ranges reported                     *0 or 1*
     ************************************************************************************************
     *                     Start of lost NR-U Sequence Number range                    *0 or (6*
     ***********************************************************************************Number of reported  
     *                       End of lost NR-U Sequence Number range                     lost NR-U SN ranges)   
     ************************************************************************************************
     *             Highest successfully delivered NR PDCP Sequence Number                    *0 or 3*
     ************************************************************************************************
     *                  Highest transmitted NR PDCP Sequence Number                          *0 or 3*
     ************************************************************************************************
     *                           Cause Value                                                 *0 or 1*
     ************************************************************************************************
     *          Successfully delivered retransmitted NR PDCP Sequence Number                 *0 or 3*
     *
     *                   Retransmitted NR PDCP Sequence Number                               *0 or 3*
     ************************************************************************************************
     *                         Padding                                                        *0 -3*
     ************************************************************************************************
     *                        UL  PDCP PDU                                                  *Payload*
     ************************************************************************************************/

     /*In current release only UM mode is suppored so above header only following parameters are suppored
       1.Desired buffer size for the data radio bearer 
       2.Desired Data Rate
       3.Number of lost NR-U Sequence Number ranges reported
       4.Start of lost NR-U Sequence Number range
       5.End of lost NR-U Sequence Number range
       6.Highest transmitted NR PDCP Sequence Number
       7.UL PDCP PDU*/

 	lostReportedCount = ceil_wrapper(drbEntity_p->lostX2uRangeReported/161);
    do
    {
        gtpuMsg_p = (UInt8 *)getMemFromPool(1000, PNULL);
        if(PNULL == gtpuMsg_p)
        {
            LOG_GTPUPR_WARNING(X2U_RELAY,"!! Memory Allocation failed for GTPU Msg !!");
            continue;
        }
        memSet(gtpuMsg_p,0,1000);
        if(relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].dcTunnelId != INVALID_TUNNEL_ID)
        {
            tunnelId = relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].dcTunnelId;
        }
        else
        {
            tunnelId = relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId;
        }
        gtpuMsg_p[current++] = RELAY_GTPU_IE_TEID_DATA_I;
        RELAY_GTPU_SET_U32BIT(&gtpuMsg_p[current],tunnelId);

        current = START_PAYLOAD +1 ;

        /*Fill PDU Type 1 in RAN container*/
        gtpuMsg_p[current] = 1 << 4;
        current +=2;

        /*Fill desired buffer size*/
        RELAY_GTPU_SET_U32BIT(&gtpuMsg_p[current],drbEntity_p->drbDesiredBufferSize);
        current+= 4;


        /*Fill lost SN ranges */
        if(drbEntity_p->lostX2uRangeReported)
        {
        	UInt8 pos = current;

            gtpuMsg_p[START_PAYLOAD + 1] |= (drbEntity_p->lostX2uRangeReported);



            if(0 != (count = listCount(&(drbEntity_p->x2uSLostSnRangeList_g))))
            {
                reportCount = 0;
                while(count)
                {
                    x2uLostSN_p = (LP_X2uLostSN)listPopNode(&(drbEntity_p->x2uSLostSnRangeList_g));
                    if(PNULL == x2uLostSN_p)
                    {
                        LOG_GTPUPR_WARNING(X2U_RELAY,"!! LostSN Node NULL, Deqeue next node !!");
                        break;
                    }

                    RELAY_GTPU_SET_U24BIT(&gtpuMsg_p[current],(x2uLostSN_p->x2uLostSN_Start));
                    current += 3;
                    RELAY_GTPU_SET_U24BIT(&gtpuMsg_p[current],x2uLostSN_p->x2uLostSN_End);
                    current += 3;
                    freeMemPool(x2uLostSN_p);
                    if(MAX_NUM_OF_LOST_X2U_PKT_RANGES == (++reportCount))
                    {
                        break;
                    }
                    count--;
                }
            	gtpuMsg_p[pos] = reportCount;
                drbEntity_p->lostX2uRangeReported = count;
            }
            else
            {
            	LOG_GTPUPR_WARNING(X2U_RELAY,"!! LostSN List count zero UE[%d] LC[%d] !!",ueIndex,lcId);

            }
        }
		/*Fill highest sucessfuly delivered NR PDCP SN*/
		if(RLC_AM_MODE == drbEntity_p->rlcMode)
		{
        	gtpuMsg_p[START_PAYLOAD+1] |= (1 << 2);
        	RELAY_GTPU_SET_U24BIT(&gtpuMsg_p[current],drbEntity_p->highestSuccessfullyDeliveredSN);
        	current += 3;
        }

        /*Fill highest transmitted NR PDCP SN*/
        gtpuMsg_p[START_PAYLOAD+1] |= (1 << 3);
            RELAY_GTPU_SET_U24BIT(&gtpuMsg_p[current],drbEntity_p->highestTransmittedSN);
        current += 3;

        current -= START_PAYLOAD + 1;
        
        ranContainerLen = (!current%4) ? (current/4): ((current/4) + 1);

        current = ranContainerLen*4;

        RELAY_GTPU_SET_U16BIT(&gtpuMsg_p[current],0);
        
        gtpuMsg_p[START_PAYLOAD] = ranContainerLen;

        msgLen = START_PAYLOAD + current;
		 /*Append CSPL header to message*/
        appendCsplHeader(EI_RELAY_MODULE_ID,GTPU_X2U_DELIVERY_STATUS,msgLen,gtpuMsg_p);

        egtpu_process_msg(p_global, gtpuMsg_p);
        freeMemPool(gtpuMsg_p);
    }while(lostReportedCount--);
    drbEntity_p->isStatusReportAvailable =  X2U_FALSE;

}
void resetDesiredBufferSize(tickType_t currentTick)
{
    UInt16          ueIndex = 0;
    UInt16          lcId = 0;
    UInt16          lcCount = 0;
    LP_X2uUeContext ueContext_p = PNULL;
    LP_X2uDrbEntityContext drbEntity_p = PNULL;


 	UInt16 ueCount = x2uRelayContext_g.activeUeCount;
    while(ueCount)
    {
    	ueIndex = x2uRelayContext_g.activeUeIdx[ueCount -  1];
        ueContext_p = GET_X2U_UE_CONTEXT(ueIndex);
        if((PNULL != ueContext_p) && (X2U_UE_CONNECTED == ueContext_p->ueState))
        {
        	for(lcCount = 0; lcCount < ueContext_p->activeLcCount; lcCount++)
            {
            	lcId = ueContext_p->activeLc[lcCount];

                drbEntity_p = GET_UE_DRB_CONTEXT(ueIndex,lcId);
                if((PNULL != drbEntity_p) && (currentTick >= drbEntity_p->desiredBufferResetTimer))
                {
                  drbEntity_p->drbDesiredBufferSize    = MAX_X2U_BUFFER_SIZE;
                  drbEntity_p->desiredBufferResetTimer = currentTick + 1000;
                }
                lcId++;
            }
        }
         ueCount--;
    }

}
/****************************************************************************
 * FUNCTION NAME   : rlcX2uDlDataDeliveryStatus
 * DESCRIPTION     : this funtion put the received status report in queue
 * RETURNS         : None
 ****************************************************************************/
UInt32 rlcX2uDlDataDeliveryStatus(
        UInt16   ueIndex,
        UInt8  lcId,
        UInt32  count,
        RlcStatusBuffer *deliveryStatus_p
        )
{
    UInt16 startVar=0;
    LP_RlcDeliveryInd rlcDeliveryInd_p = PNULL;
    LP_X2uUeContext ueContext_p = PNULL;


    if ( (MAX_UE_SUPPORTED <= ueIndex) ||
            (LTE_DRB_LC_ID_LOW_BOUND > lcId) ||
            (LTE_DRB_LC_ID_HIGH_BOUND < lcId) )
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,"!! Invalid LC in delivery Status" 
        "ueIndex :%u lcId: %u!!",ueIndex,lcId);
        return X2U_FAILURE;
    }
    ueContext_p = GET_X2U_UE_CONTEXT(ueIndex);

    if(PNULL == ueContext_p)
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,"!! Invalid UE Context ueIndex :%u lcId: %u!!",ueIndex,lcId);
        return X2U_FAILURE;
    }

    /* Allocate data request structure */
    rlcDeliveryInd_p = 
        (LP_RlcDeliveryInd)getMemFromPool( sizeof(RlcDeliveryInd), PNULL);

    if ( PNULL != rlcDeliveryInd_p )
    {
        /* Init dataInd_p. */
        rlcDeliveryInd_p->ueIndex = ueIndex;
        rlcDeliveryInd_p->lcId = (UInt16)lcId;
        rlcDeliveryInd_p->count = count;
        rlcDeliveryInd_p->deliveryStatusArr = (RlcStatusBuffer *)\
                                           getMemFromPool(sizeof(RlcStatusBuffer)*count, PNULL);
        if (PNULL != rlcDeliveryInd_p->deliveryStatusArr)
        {        
            for (startVar=0;startVar<count;startVar++) 
            {
                rlcDeliveryInd_p->deliveryStatusArr[startVar].transId =
                    deliveryStatus_p[startVar].transId;
                rlcDeliveryInd_p->deliveryStatusArr[startVar].deliveryStatus =
                    deliveryStatus_p[startVar].deliveryStatus;
            }
        }
        else
        {
            LOG_GTPUPR_ERROR(X2U_RELAY,"!! Memory allocation fail for RlcStatusBuffer" 
            "ueIndex :%u lcId: %u!!",ueIndex,lcId);

            ltePanic ("Memory allocation fail for %s", __FUNCTION__);
            freeMemPool (rlcDeliveryInd_p);
            rlcDeliveryInd_p = PNULL;
        }
    }
    else {
            LOG_GTPUPR_ERROR(X2U_RELAY,"!! Memory allocation fail for RlcDeliveryInd"
            "ueIndex :%u lcId: %u!!",ueIndex,lcId);
    }
    if(!ENQUEUE_RLC_TX_DELIVERY_IND_Q(rlcDeliveryInd_p))
    {
       freeMemPool(rlcDeliveryInd_p); 
    }
    return X2U_SUCCESS;
    
}
/****************************************************************************
 * FUNCTION NAME   : rlcX2uUlDataInd
 * DESCRIPTION     : this funtion sends the UL PDCP PDUs to PacketRelay
 * RETURNS         : None
 ****************************************************************************/
#if defined (ENDC_ENABLED) && defined(PDCP_GTPU_INTF)
UInt32 rlcX2uUlDataInd(UInt16 ueIndex, UInt8 lcId,
        UInt8  packetForReEstablishment,
        UInt16 size,
        tickType_t headPktTS,
        tickType_t pktTimeStamp,
        void*  data_p
        )
#elif ENDC_ENABLED
UInt32 rlcX2uUlDataInd(UInt16 ueIndex, UInt8 lcId,
        void*  data_p, UInt16 size)
#endif
{
	UInt8 *gtpuMsg_p = PNULL;
	UInt8 lcIndex = lcId - LTE_DRB_LC_ID_LOW_BOUND;
	UInt16 msgLen = 0;
	UInt32          current = CSPL_HDR;
	UInt32 tunnelId = 0;
    LP_X2uDrbEntityContext drbEntity_p = PNULL;


    if ( (MAX_UE_SUPPORTED <= ueIndex) ||
            (LTE_DRB_LC_ID_LOW_BOUND > lcId) ||
            (LTE_DRB_LC_ID_HIGH_BOUND < lcId) )
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,"!! Invalid LC in delivery Status" 
                "ueIndex :%u lcId: %u!!",ueIndex,lcId);
        return X2U_FAILURE;
    }

    drbEntity_p = GET_UE_DRB_CONTEXT(ueIndex,lcId);
    if(PNULL == drbEntity_p)
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,"RAB Entity Doesn't exists for" 
                "UeId:%u lcId:%u",ueIndex,lcId);
        return X2U_FAILURE;
    }

    if(!size || !data_p)
    {
        LOG_GTPUPR_WARNING(X2U_RELAY,"!! Invalid data and Len"
                "ueIndex :%u lcId: %u  size :%u!!",ueIndex,lcId, size);

    }
    msgLen = size+ START_PAYLOAD + X2U_RAN_CONTAINER_PDU_TYPE_1_MIN_LEN;
    gtpuMsg_p = (UInt8 *)getMemFromPool(msgLen, PNULL);
    if(PNULL == gtpuMsg_p)
    {
    	LOG_GTPU_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
    	        relayGtpuCurrentTick_g, __LINE__,
    	        msgLen, 0,0,0,0,0, __func__,
    	        "ADAPTER" );
    	return X2U_FAILURE;
    }
    /*Append CSPL header to message*/
    appendCsplHeader(EI_RELAY_MODULE_ID,GTPU_X2U_DELIVERY_STATUS,msgLen,gtpuMsg_p);
    if(relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].dcTunnelId != INVALID_TUNNEL_ID)
    {
        tunnelId = relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].dcTunnelId;
    }
    else
    {
        tunnelId = relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId;
    }
    gtpuMsg_p[current++] = RELAY_GTPU_IE_TEID_DATA_I;
    RELAY_GTPU_SET_U32BIT(&gtpuMsg_p[current],tunnelId);

    current = START_PAYLOAD;
    gtpuMsg_p[current++] = X2U_RAN_CONTAINER_PDU_TYPE_1_MIN_LEN + size;

    /*Fill PDU Type 1 in RAN container*/
    gtpuMsg_p[current] = 1 << 4;

    /*Fill desired buffer size*/
    RELAY_GTPU_SET_U32BIT(&gtpuMsg_p[current],drbEntity_p->drbDesiredBufferSize);
    current+= 4;


    /*Append RLC UL data*/
    msgExtract(data_p, 0, gtpuMsg_p+START_PAYLOAD + X2U_RAN_CONTAINER_PDU_TYPE_1_MIN_LEN, size);


    egtpu_process_msg(p_global, gtpuMsg_p);
    freeMemPool(gtpuMsg_p);

    msgFree(data_p);
    data_p = PNULL;

    return X2U_SUCCESS;
}
/****************************************************************************
 * FUNCTION NAME   : triggerDeliveryStatusReport
 * DESCRIPTION     : this funtion check and delete node from X2U storage queue
 *                   if ACK received for concern UE
 * RETURNS         : None
 ****************************************************************************/
void processRlcDeliveryStatusReport()
{
    UInt16 startVar  = 0;
    UInt32 sQCount   = QCOUNT_RLC_TX_DELIVERY_IND_Q();
    LP_X2uDataReq          dataReq_p = PNULL;
    LP_RlcDeliveryInd      deliveryInd_p = PNULL;
    LP_X2uDrbEntityContext drbEntity_p = PNULL;

    while(sQCount--)
    {
        DEQUEUE_RLC_TX_DELIVERY_IND_Q(deliveryInd_p);
        if ( PNULL == deliveryInd_p )
        {
            LOG_GTPUPR_WARNING(X2U_RELAY,"DeliveryInd Node is NULL");
            break;
        }

        drbEntity_p = GET_UE_DRB_CONTEXT(deliveryInd_p->ueIndex,deliveryInd_p->lcId);
        if(PNULL != drbEntity_p)
        {
            for (startVar=0;startVar<deliveryInd_p->count;startVar++)
            {
                switch( deliveryInd_p->deliveryStatusArr[startVar].deliveryStatus )
                {
                    case X2U_RLC_ACK :
                    {
                        dataReq_p = (LP_X2uDataReq)( deliveryInd_p->
                                deliveryStatusArr[startVar].transId );
                        if( PNULL == dataReq_p )
                        {
                            LOG_GTPUPR_WARNING(X2U_RELAY,"DataReq node is NULL for UeIndex :%u LcId: %u",
                            deliveryInd_p->ueIndex, deliveryInd_p->lcId);
                            continue;
                        }
                        drbEntity_p->highestSuccessfullyDeliveredSN = dataReq_p->commDataReq.sn;
                        drbEntity_p->isStatusReportAvailable = X2U_TRUE;

                        DELETE_NODE_X2U_TX_STORAGE_Q(drbEntity_p,dataReq_p);
                        pdcpFreeTxX2UDataReq(dataReq_p);
                        break;
                    }
                    case X2U_RLC_NACK:
                    {
                        /* RLC NACK is received */
                        /* No need to search and delete the node from queue as it has 
                         * to be forwarded to target PDCP or needs to be re-transmitted
                         * in case of re-establishment */                

                        LOG_GTPUPR_WARNING(X2U_RELAY,"RLC NACK Received for UeIndex :%u lcId:%u",
                        deliveryInd_p->ueIndex, deliveryInd_p->lcId);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
        }
        if(PNULL != deliveryInd_p)
        {
            freeMemPool ( deliveryInd_p->deliveryStatusArr );
            deliveryInd_p->deliveryStatusArr = PNULL;
            freeMemPool( deliveryInd_p );
            deliveryInd_p = PNULL;
        }
    }
}
/****************************************************************************
 * FUNCTION NAME   : suspendSplitBearers
 * DESCRIPTION     : this funtion sets the UE state as suspend
 * RETURNS         : None
 ****************************************************************************/
void suspendSplitBearers(UInt16 ueIndex)
{
    LP_X2uUeContext ueContext_p = PNULL;
    ueContext_p = GET_X2U_UE_CONTEXT(ueIndex);
    if(PNULL == ueContext_p)
    {
       LOG_GTPUPR_WARNING(X2U_RELAY,"UE CONTEXT is NULL for received UEId: %u",ueIndex);
       return;
    }
    /*Set the UE state to suspend*/
    ueContext_p->ueState = X2U_UE_SUSPENDED;

}
/****************************************************************************
 * FUNCTION NAME   : startGuardTimerAndReleaseBearers
 * DESCRIPTION     : this funtion start guard timer for data forwarding and 
 *                   release bearer upon expiry
 * RETURNS         : None
 ****************************************************************************/
void startGuardTimerAndReleaseBearers(UInt16 ueIndex)
{
    UInt8 lcCount = 0;
    tickType_t      currentTick = 0;
    LP_X2uUeContext ueContext_p = PNULL;
    UInt16  lcId = 0;
    
    ueContext_p = GET_X2U_UE_CONTEXT(ueIndex);
    if(PNULL == ueContext_p)
    {
       LOG_GTPUPR_WARNING(X2U_RELAY,"UE CONTEXT is NULL for received UEId: %u",ueIndex);
       return;
    }
    if(X2U_DATA_FWDED == ueContext_p->dataFwdState)
    {
        for (lcCount = 0; lcCount < ueContext_p->activeLcCount; lcCount++)
        {
        	lcId = ueContext_p->activeLc[lcCount];
            if(X2U_SUCCESS != x2uRelayDeleteBearerContext(ueIndex,lcId))
            {
       			LOG_GTPUPR_WARNING(X2U_RELAY," Bearer Delete fail for  UEId: %u LcId[%d]",ueIndex,lcId);
            }
        }
        /*No need to start guard timer*/
        return;
    }
    GET_X2U_CURRENT_TICK(currentTick);
    ueContext_p->dataFwdGuardTimerExpiryTTI = currentTick + 
    x2uRelayContext_g.dataForwardingGuardTimerInterval; 
}
/****************************************************************************
 * FUNCTION NAME   : freeAllDrbEntityTxRxQ
 * DESCRIPTION     : This function free the TX RX Queue of bearer
 * RETURNS         : None
 ****************************************************************************/
void freeDrbEntityTxRxQ(LP_X2uDrbEntityContext drbEntity_p)
{
    LP_X2uDataReq         dataReq_p = PNULL;

    do 
    {
        DEQUEUE_X2U_TX_STORAGE_Q(drbEntity_p,dataReq_p);
        if(PNULL != dataReq_p)
        {
            if(PNULL != dataReq_p->data_p)
            {
                msgFree(dataReq_p->data_p);
                dataReq_p->data_p = PNULL;
            }
            freeMemPool(dataReq_p);
        }
    }while(PNULL != dataReq_p);
}
/*S1U_DELETE_FIX*/
void updateTickAssAtX2U(UInt16 ueIndex)
{
    LP_X2uUeContext ueContext_p = PNULL;
    ueContext_p = GET_X2U_UE_CONTEXT(ueIndex);
    if(ueContext_p)
    {
        GET_X2U_CURRENT_TICK(ueContext_p->tickAss);
    }
}
/*S1U_DELETE_FIX*/


/****************************************************************************
 * FUNCTION NAME   : processDiscardPdcpSdu
 * DESCRIPTION     : This function free the PDCP SDUs upon expiry of discard 
 *                   Timer
 * RETURNS         : None
 ****************************************************************************/
void processDiscardPdcpSdu(tickType_t currentTick)
{
    UInt16          ueIndex = 0;
    UInt16          lcId = 0;
    UInt16          retVal = 0;
    UInt16          ueCount = 0;
    UInt16          lcCount =0;
    /*X2uUeDataInd          *ueDataInd_p = PNULL;
    X2uUeDataInd          *tempUeDataInd_p = PNULL;*/
    LP_X2uDataReq         dataReq_p = PNULL;
    LP_X2uDataReq         tempDataReq_p = PNULL;
    LP_X2uUeContext ueContext_p = PNULL;
    LP_X2uDrbEntityContext drbEntity_p = PNULL;
    ueCount = x2uRelayContext_g.activeUeCount; 
    while(ueCount)
    {
    	ueIndex = x2uRelayContext_g.activeUeIdx[ueCount -  1];
        ueContext_p = GET_X2U_UE_CONTEXT(ueIndex);
        if((PNULL != ueContext_p) && (X2U_UE_CONNECTED == ueContext_p->ueState))
        {
        	for(lcCount = 0; lcCount < ueContext_p->activeLcCount; lcCount++)
            {
            	lcId = ueContext_p->activeLc[lcCount];

                drbEntity_p = GET_UE_DRB_CONTEXT(ueIndex,lcId);
                if(PNULL != drbEntity_p && 0 != drbEntity_p->discardTimer)
                {
                    /*Discard PDCP Tx SDUs*/
                    GET_FIRST_X2U_TX_STORAGE_Q(drbEntity_p,dataReq_p);
                    do
                    {
                        if((PNULL == dataReq_p) || (currentTick < 
                                	(dataReq_p->tickAssociated + drbEntity_p->discardTimer)))
                        {
                            break;
                        }
#ifdef PDCP_GTPU_INTF
                        retVal = sendRlcDiscardReq(ueIndex, (UInt16)lcId, (void *)dataReq_p, sizeof(X2uDataReq));
#else
                        retVal = rlcDataDiscardReq(ueIndex, lcId, (void *)dataReq_p);
#endif
                        tempDataReq_p = dataReq_p;
                        GET_NEXT_X2U_TX_STORAGE_Q(drbEntity_p,dataReq_p);

                        if(X2U_SUCCESS == retVal)
                        {
                            DELETE_NODE_X2U_TX_STORAGE_Q(drbEntity_p, tempDataReq_p);
                            pdcpFreeTxX2UDataReq(tempDataReq_p);

                        }

                    }while(PNULL != dataReq_p);
                }
  				if((PNULL != drbEntity_p) && (!QCOUNT_X2U_TX_STORAGE_Q(drbEntity_p)))
                {
                    drbEntity_p->isFirstPktFromMeNB = X2U_FALSE;
                }
                lcId++;

#if 0
                /*discard PDCP RX SDUs*/
                GET_FIRST_X2U_RX_STORAGE_Q(drbEntity_p,ueDataInd_p);
                do
                {
                    if((PNULL == ueDataInd_p) || (currentTick < 
                          		(ueDataInd_p->tickAssociated + drbEntity_p->discardTimer)))
                    {
                        break;
                    }
                    tempUeDataInd_p = ueDataInd_p;
                    GET_NEXT_X2U_RX_STORAGE_Q(drbEntity_p,ueDataInd_p);

                    DELETE_NODE_X2U_TX_STORAGE_Q(drbEntity_p, tempUeDataInd_p);
                    if(PNULL != tempUeDataInd_p)
                    {
                        if(PNULL != tempUeDataInd_p->data_p)
                        {
                            msgFree(tempUeDataInd_p->data_p);
                            tempUeDataInd_p->data_p = PNULL;
                        }
                        freeMemPool(tempUeDataInd_p);
                        tempUeDataInd_p = PNULL;
                    }

                }while(PNULL != ueDataInd_p);
                UInt8 lcIndex = 0;
                UInt8 deleteUe = X2U_TRUE;
                if((0 != ueContext_p->tickAss) && (currentTick >= ueContext_p->tickAss +  X2U_DELETE_UE_TIMER))
                {
                    if(PNULL != drbEntity_p && drbEntity_p->isFwdingEnabled)
                    {
                        for (lcIndex = 0; lcIndex < MAX_RAB_ID; lcIndex++)
                        {
                            if((INVALID_TUNNEL_ID !=                                                                                                                                                      relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelDlFwdId) && 
                                	SN_TERMINATED_SCG_SPLIT_BEARER != relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].dcBearerType)
                                deleteUe = X2U_FALSE;
                        }
                        if(deleteUe)
                        {
                            triggerDeleteUeEntityReq(ueIndex);

                        }
                    }
                }
                if((X2U_DATA_FWDING == ueContext_p->dataFwdState || 
                            X2U_DATA_FWDED == ueContext_p->dataFwdState) && 
                        (X2U_TRUE == isDataFwdGuardTimerExpired(ueIndex)))
                {
                    x2uRelayDeleteBearerContext(
                            ueIndex, 
                            (lcId + LTE_DRB_LC_ID_LOW_BOUND)); 

                }
#endif 
            }
        }
        ueCount--;
    }
}
#if 0
/****************************************************************************
 * FUNCTION NAME   : isDataFwdGuardTimerExpired
 * DESCRIPTION     : This fuction checks the expiry of guard timer
 * RETURNS         : TRUE/FALSE
 ****************************************************************************/
UInt8 isDataFwdGuardTimerExpired(UInt16 ueIndex)
{
    tickType_t      currentTick = 0;
    LP_X2uUeContext ueContext_p = PNULL;
    ueContext_p = GET_X2U_UE_CONTEXT(ueIndex);
    if(PNULL == ueContext_p)
    {
       LOG_GTPUPR_WARNING(X2U_RELAY,"UE CONTEXT is NULL for received UEId: %u",ueIndex);
       return X2U_FAILURE;
    }
    GET_X2U_CURRENT_TICK(currentTick);
    return ((ueContext_p->dataFwdGuardTimerExpiryTTI >= currentTick) ? X2U_TRUE:X2U_FALSE);
}


/****************************************************************************
 * FUNCTION NAME   : X2uSourceDataFwdReq
 * DESCRIPTION     : This function forwards the sournce PDCP DL/UL data to PR
 * RETURNS         : None
 ****************************************************************************/
void x2uSourceDataFwdReq(void* data_p, UInt16 size)
{
    UInt8                 lcCount = 0;
    UInt8                 lcId = 0;
    UInt32                qCount = 0;
    X2uUeDataInd          *ueDataInd_p = PNULL;
    LP_X2uDataReq         dataReq_p = PNULL;
    LP_X2uUeContext       ueContext_p = PNULL;
    LP_X2uDataForwardReq  dataFwdReq_p = PNULL;
    LP_X2uDrbEntityContext drbEntity_p = PNULL;

    dataFwdReq_p = (LP_X2uDataForwardReq)data_p;


    if(X2U_FALSE == x2uRelayContext_g.isInitialized)
    {
       LOG_GTPUPR_WARNING(X2U_RELAY,"X2U Relay not initialized");
       return;
    }
    if ( sizeof( X2uDataForwardReq ) != size ) 
    {
       LOG_GTPUPR_WARNING(X2U_RELAY,"Invalid Length of data forwarding req msg");
       return;
    }
    ueContext_p = GET_X2U_UE_CONTEXT(dataFwdReq_p->ueIndex);
    if(PNULL == ueContext_p)
    {
       LOG_GTPUPR_WARNING(X2U_RELAY,"UE CONTEXT is NULL for received UEId: %u",dataFwdReq_p->ueIndex);
       return;
    }
    ueContext_p->dataFwdState = X2U_DATA_FWDING;
    /*PDCP DL data forwarding*/
    for (lcCount = 0; lcCount < dataFwdReq_p->count; lcCount++)
    {
        lcId = dataFwdReq_p->dataFwdInfo[lcCount].lcId;
        drbEntity_p = GET_UE_DRB_CONTEXT(dataFwdReq_p->ueIndex,lcId);
        if(PNULL != drbEntity_p)
        {
            drbEntity_p->isFwdingEnabled = X2U_TRUE;
            qCount = QCOUNT_X2U_TX_STORAGE_Q(drbEntity_p);
            if(qCount)
            {
                GET_FIRST_X2U_TX_STORAGE_Q(drbEntity_p,dataReq_p);
                while(PNULL != dataReq_p)
                {
                    if(X2U_FAILURE == pdcpSourceDlDataRelocInd(
                                dataReq_p->commDataReq.ueIndex,
                                dataReq_p->commDataReq.lcId,
                                dataReq_p->commDataReq.sn,
                                dataReq_p->data_p,
                                dataReq_p->commDataReq.size))
                    {
                        LOG_GTPUPR_WARNING(X2U_RELAY,"PDCP DL Data Forwarding failed");
                    }
                    GET_NEXT_X2U_TX_STORAGE_Q(drbEntity_p,dataReq_p);
                }
            }
        }
        else
        {
            LOG_GTPUPR_WARNING(X2U_RELAY,"DRB ENTITY is NULL for received UEId: %u"
            "lcid :%u",dataFwdReq_p->ueIndex, lcId);
        }
    }

    /*PDCP UL data forwarding*/
    for (lcCount = 0; lcCount < dataFwdReq_p->count; lcCount++)
    {
        lcId = dataFwdReq_p->dataFwdInfo[lcCount].lcId;
        drbEntity_p = GET_UE_DRB_CONTEXT(dataFwdReq_p->ueIndex,lcId);
        if(PNULL != drbEntity_p)
        {
            qCount = QCOUNT_X2U_RX_STORAGE_Q(drbEntity_p);
            if(qCount)
            {
                GET_FIRST_X2U_RX_STORAGE_Q(drbEntity_p,ueDataInd_p);
                while(PNULL != ueDataInd_p)
                {
                    if(dataFwdReq_p->dataFwdInfo[lcCount].ulDataFwdFlag)
                    {
                        if(X2U_FAILURE == pdcpSourceUlDataRelocInd(
                                    ueDataInd_p->ueIndex,
                                    ueDataInd_p->lcId,
                                    ueDataInd_p->sn,
                                    ueDataInd_p->data_p,
                                    ueDataInd_p->dataLen
#ifdef KPI_STATS
                                    ,INVALID_CELL_INDEX
#endif
                                    ))
                        {
                            LOG_GTPUPR_WARNING(X2U_RELAY,"PDCP UL Data Forwarding failed");
                        }
                    }
                    GET_NEXT_X2U_RX_STORAGE_Q(drbEntity_p,ueDataInd_p);
                }
            }
        }
        else
        {
            LOG_GTPUPR_WARNING(X2U_RELAY,"DRB ENTITY is NULL for received UEId: %u"
            "lcid :%u",dataFwdReq_p->ueIndex, lcId);
        }

    }
    ueContext_p->dataFwdState = X2U_DATA_FWDED;
}
#endif
#endif
