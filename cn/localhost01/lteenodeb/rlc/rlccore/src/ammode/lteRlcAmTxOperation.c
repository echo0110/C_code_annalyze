/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcAmTxOperation.c,v 1.1.6.2.2.1 2010/09/21 15:50:03 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *  This file describes funtionality when delivering a new AMD PDU to lower layer
 ****************************************************************************
 * Revision Details
 * ----------------
 * $Log: lteRlcAmTxOperation.c,v $
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcMacInterface.h"
#include "lteRlcPdcpInterface.h"
#include "lteRlcAmTxOperation.h"
#include "lteRlcUeContext.h"
#include "lteRlcAmdArqOperation.h"
#include "lteLayer2MacRlcInterface.h"
#include "logging.h"
#include "lteRlcStat.h"
#include "atomic-ops.h"
#include "lteRlcErrorCode.h"
/****************************************************************************
 * Varibale Definition
 ****************************************************************************/
/** SPR 3657 Chanegs Start **/
#define MAX_RLC_PDU_PER_TICK 100

#define RLC_CONTROL_PDU(pAmTxE, txOpp, pAmRxE,pduCount)\
    ( (pAmTxE->sendStatusInd) && (txOpp) &&  !(pAmRxE->isSPTimerRunning) && \
    (pduCount < MAX_RLC_PDU_PER_TICK)) ? (1) : (0)


#define RLC_RETX_PDU(pAmTxE, txOpp,pduCount) \
    ( ( (pAmTxE->txPduIn.pRawPdu.size) || \
    (  QUEUE_COUNT_AM_NACK_LIST_Q(pAmTxE->nackList)) ) && (txOpp  > 2) && \
     (pduCount < MAX_RLC_PDU_PER_TICK) ) ? 1 : 0\
/* SPR 11571 changes start */
 #define RLC_NEW_PDU(pAmTxE, txOpp ,pduCount) \
    ((pAmTxE->txQueueSize || pAmTxE->txSduIn.rlcAmRawSdu.size) && \
    /* Start 128 UE: 25% NACK Changes */ \
    ( txOpp > 2) &&  (pduCount < MAX_RLC_PDU_PER_TICK)) ? 1 : 0
    /* End 128 UE: 25% NACK Changes */
/* SPR 11571 changes end */
/** SPR 3657 Changes End **/

#define IS_VALID_AM_TX_RX_ENTITY(amEntity_p)\
 ( (!amEntity_p) || !(amEntity_p->txAMEntity_p) \
 || !(amEntity_p->rxAMEntity_p) ) ? 0 : 1


/****************************************************************************
 * External Variable Includes
 ****************************************************************************/
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */
#define MAX_TWO_BYTE_SUBHEADER 127
/**********************************************************************************
 * Function Name  : rlcFetchIncmpPduQload 
 * Inputs         : txAMEntity_p - a pointer to TxAMEntity,
 *                  queueLoad - Pointer to QueueLoad
 * Outputs        : None
 * Returns        : Size of the Incomplete PDU queue load
 * Description    : describes the calculation of the queue load for Incomplete PDU 
 *                  that is recieved for AM Entity @txAMEntity_p.
 **********************************************************************************/
UInt32 rlcFetchIncmpPduQload(TxAMEntity* txAMEntity_p,UInt32 *queueLoad)
{
    UInt32 vrHeaderSize = 0;
    UInt16 index = 0, dfeCnt = 0, sduCount = 0;
    UInt16 dataSize = 0, soStart = 0, soEnd = 0, sduSize = 0;
    RlcSn seqNum = 0; 
    AmReTransmission* amReTxPdu_p = RLC_NULL;

    seqNum = GET_RETX_QUEUE_INDEX_FROM_SN(txAMEntity_p->txPduIn.sn);
    GET_IN_PDU_FROM_RETXQ(txAMEntity_p->reTxQueue, \
            seqNum, amReTxPdu_p);
    if(RLC_NULL == amReTxPdu_p)
    {
        LOG_RLC_MSG(LTE_RLC_NULL_RETX_PDU_POINTER, LOGERROR, (RLC_TX|RLC_AM), 
                globalTTITickCount_g,__LINE__,txAMEntity_p->ueIndex, txAMEntity_p->lcId, 
                seqNum, RLC_ERR_NULL_RETX_PDU_POINTER_FOUND, 0, 0, __func__, "");
        return RLC_ERR_NULL_RETX_PDU_POINTER_FOUND;
    }
    else
    {
        /* Get the data field element count */
        dfeCnt = amReTxPdu_p->dfeInfo.dfeCount;
        soStart = txAMEntity_p->txPduIn.Sostart;
        soEnd = txAMEntity_p->txPduIn.Soend;
        /* Count the number of SDUs in segment occured within 
           sostart and soend */
        while(index < dfeCnt)
        {
            sduSize = amReTxPdu_p->dfeInfo.sizeOfDfe[index];
            dataSize += sduSize;
            if((soStart <= dataSize))
            {
                sduCount++;
            }
            if(dataSize > soEnd) 
            {
                break;
            }

            index++;
        }
        /* calculating data size */
        dataSize = (soEnd + 1) - soStart;
        /* calculating variable header size */
        vrHeaderSize = GET_VAR_HEADER_SIZE( sduCount);
        /* calculating queueLoad */
        *queueLoad +=  (vrHeaderSize + dataSize 
                + AM_PDU_RESEG_FIX_HDR_SIZE);
    }

    LOG_RLC_MSG(LTE_RLC_NULL_RETX_PDU_POINTER_1, LOGINFO, (RLC_TX|RLC_AM), 
            globalTTITickCount_g, seqNum , txAMEntity_p->ueIndex, txAMEntity_p->lcId, 
            soStart , soEnd, 0,0, __func__, "");

    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : rlcFetchReTxPduQload
 * Inputs         : txAMEntity_p - a pointer to TxAMEntity,
 *                  queueLoad - Pointer to QueueLoad
 * Outputs        : Size of the Re-Tx queue load
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : describes the calculation of the Re-Tx queue load that is
 *                  recieved for AM Entity @txAMEntity_p.
 ****************************************************************************/
UInt32 rlcFetchReTxPduQload(TxAMEntity* txAMEntity_p,UInt32 *queueLoad)
{   
    UInt32 reTxQueueLoad = txAMEntity_p->reTxQueueSize;
    /* queuecount in nacklist = 0 && no Incomplete pdu */
    if (!QUEUE_COUNT_AM_NACK_LIST_Q(txAMEntity_p->nackList) && !(txAMEntity_p->txPduIn.pRawPdu.size))//shreya
    {
        atomicSubtractInt(&txAMEntity_p->reTxQueueSize, reTxQueueLoad);

        LOG_RLC_MSG(LTE_RLC_FETCH_RETX_PDU_QLOAD, LOGINFO , (RLC_TX|RLC_AM),
                globalTTITickCount_g, __LINE__,  reTxQueueLoad , txAMEntity_p->reTxQueueSize,
                0, 0, 0, 0, __func__, "");
        return RLC_FAILURE;
    }
    *queueLoad += txAMEntity_p->reTxQueueSize; 
    if( txAMEntity_p->txPduIn.pRawPdu.size)
    {
        *queueLoad += AM_PDU_RESEG_FIX_HDR_SIZE;
    }
    else     
    {
        *queueLoad += AM_PDU_SEG_FIX_HDR_SIZE;
    }

    LOG_RLC_MSG(LTE_RLC_FETCH_RETX_PDU_QLOAD_1, LOGINFO , (RLC_TX|RLC_AM),
            globalTTITickCount_g, __LINE__,  *queueLoad , txAMEntity_p->reTxQueueSize,
            0, 0, 0, 0, __func__, "");
    return RLC_SUCCESS;
}
/****************************************************************************
 * Function Name  : rlcFetchFreshDataQload
 * Inputs         : txAMEntity_p - a pointer to TxAMEntity
 * Outputs        : None
 * Returns        : Size of the Fresh Data queue load
 * Description    : describes the calculation of the fresh data queue load 
 *                  that is recieved for AM Entity @txAMEntity_p.
 ****************************************************************************/
UInt32 rlcFetchFreshDataQload(TxAMEntity* txAMEntity_p)
{
    UInt32 queueLoad = 0;
    UInt32 vrHeaderSize = 0;
    UInt16 sduCount = 0;

    /* SPR 5283 Fix Start */
    RlcQSz txQueueSize = txAMEntity_p->txQueueSize;
    RlcQSz txSduInQSize = txAMEntity_p->txSduIn.rlcAmRawSdu.size;
    if((txQueueSize) || 
    /* SPR 5283 Fix End */
            (txAMEntity_p->txSduIn.rlcAmRawSdu.size))
    {
        sduCount = QUEUE_COUNT_AMD_RAW_SDU_Q(txAMEntity_p->txSDUQueue);
        /* SPR 4804 Fix Start */
        /* SPR 5283 Fix Start */
        if((!sduCount) && (txQueueSize))
        /* SPR 5283 Fix End */
        {
             txAMEntity_p->txQueueSize = 0; 
        }
        /* SPR 4804 Fix End */
	      /* + SPR#5283 */
	      if(txSduInQSize)
        /* - SPR#5283 */
	      {
            sduCount += 1;
	           /* + SPR#5283 */
	          queueLoad += txSduInQSize;
             /* - SPR#5283 */
	      }
	      /* calculating variable header size */
	      /* SPR 4804 Fix Start */
	      /* + SPR#5283 */
	      if((sduCount && txQueueSize) || (sduCount && txSduInQSize))
	      /* - SPR#5283 */
	      {
	          vrHeaderSize = GET_VAR_HEADER_SIZE( sduCount);
	          /* + SPR#5283 */
	          queueLoad += (txQueueSize + vrHeaderSize );
	          /* - SPR#5283 */
	      }
	      /* SPR 4804 Fix End */
    }
       /* + SPR#5283 */
    if (queueLoad)
       queueLoad += AM_PDU_SEG_FIX_HDR_SIZE;
    /* - SPR#5283 */
    LOG_RLC_MSG(LTE_RLC_FETCH_RETX_PDU_QLOAD_2, LOGINFO , (RLC_TX|RLC_AM),
            globalTTITickCount_g, txAMEntity_p->ueIndex, txAMEntity_p->lcId, queueLoad ,
            txAMEntity_p->txSduIn.rlcAmRawSdu.size, vrHeaderSize, 0, 0, __func__, "");

    return (queueLoad);
}

/****************************************************************************
 * Function Name  : rlcFetchQueueLoad
 * Inputs         : txAMEntity_p - Pointer to TxAMEntity,
 *                  lcQueueLoad - Pointer to LCQueueLoad
 * Outputs        : Size of the queue load
 * Returns        : None
 * Description    : describes the calculation of the queue load that is
 *                  recieved for AM Entity @txAMEntity_p.
 ****************************************************************************/

void rlcFetchQueueLoad(TxAMEntity* txAMEntity_p,LCQueueLoad *lcQueueLoad)
{
    UInt32 qLoad = 0;
    UInt32 ret = RLC_SUCCESS;
    /** RLC Fetch qload for Status PDU **/ 
    if (txAMEntity_p->sendStatusInd)
    {    
        lcQueueLoad->ctrlQueueLoad = txAMEntity_p->rxStatusPduSize;
    }
    /** RLC fetch qload for Incomplete PDU **/
    if(txAMEntity_p->txPduIn.pRawPdu.size)
    {
        ret = rlcFetchIncmpPduQload(txAMEntity_p, &qLoad);
        if(ret != RLC_SUCCESS)
            lcQueueLoad->retxnPduSize = 0;
        else
            lcQueueLoad->retxnPduSize = qLoad;
    }
    /** RLC Fetch qload for Re-Tx Pdu **/ 
    else if(txAMEntity_p->reTxQueueSize)
    {
        ret = rlcFetchReTxPduQload(txAMEntity_p, &qLoad);
        if(ret != RLC_SUCCESS)
            lcQueueLoad->retxnPduSize = 0;
        else
            lcQueueLoad->retxnPduSize = qLoad;
    }
    /** RLC fetch qload for fresh data **/
    lcQueueLoad->lcQueueLoad = rlcFetchFreshDataQload(txAMEntity_p);

    LOG_RLC_MSG(LTE_RLC_QLOAD_CALCULATION, LOGDEBUG, (RLC_TX|RLC_AM), globalTTITickCount_g,
            txAMEntity_p->ueIndex, txAMEntity_p->lcId, lcQueueLoad->ctrlQueueLoad,
            lcQueueLoad->retxnPduSize,lcQueueLoad->lcQueueLoad, 0,0, __func__, "");       
}


/****************************************************************************
 * Function Name  : rlcReceiveAmSduFromUpperLayer
 * Inputs         : txAMEntity_p - a pointer to TxAMEntity,
 *                  pAmdSdu - a pointer to RlcUeDataReq
 * Outputs        : None
 * Returns        : RLC_SUCCESS if case success else RLC_FAILURE
 * Description    : This function does following things :-
 *                  1. Check Number of TX SDU Nodes is not equal or greater
 *                     then predefined MAX_NUMBER_OF_SDU.
 *                  2. Step 1 is not true then 
 *                     a) Add the Qload into the RLC Entity.
 *                     b) Enqueue the PDCP Data Buffer Node into the RLC TX 
 *                        SDU Queue.  
 *                     c) Update the RLC TX SDU stats.
 *                  3. Fetch the total QLoad for this RLC entity and fill into
 *                     the LCQueueLoad structure using "rlcFetchQueueLoad"func.
 *                  4. Check for RLC Window Stall.
 *                  5. Update the Qload to MAC Layer by using mac function
 *                       "macUpdateBufferOccupancyReq".
 *
 ****************************************************************************/

UInt32 rlcReceiveAmSduFromUpperLayer ( TxAMEntity * txAMEntity_p, 
                                              RlcUeDataReq *pAmdSdu) 
{
    LCQueueLoad lcQLoad = {0};
    AmdRawSduQ *amdRawSduQ_p = &txAMEntity_p->txSDUQueue ;
    /* SPR 12612 Start */
    UInt16   pAmdSduSize = pAmdSdu->size;
    /* SPR 12612 End */ 
    
    UInt32 amdSDUCount = QUEUE_COUNT_AMD_RAW_SDU_Q(*amdRawSduQ_p);
    InternalCellIndex cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(txAMEntity_p->ueIndex);    

    UInt32 ret = RLC_SUCCESS;
    RlcVr vtA  = txAMEntity_p->vtA;
    RlcVr vtMS = txAMEntity_p->vtMS;
    RlcVr vtS  = txAMEntity_p->vtS;
    LteRlcLCStats* lcStats_p =
        &(gRlcStats.pRlcStats->lteRlcUeStat[txAMEntity_p->ueIndex].
                lcStats[txAMEntity_p->lcId]);


    lcQLoad.lcId = txAMEntity_p->lcId;
    lcQLoad.scheduledDataBytes = 0;
    lcQLoad.ctrlFlag  = RLC_NORMAL_DATA;

#ifdef UE_SIM_TESTING 
    /* + SPR 19808 */
    /* if maximum number of SDu received */ 
    if ( amdSDUCount >= MAX_AM_RAW_SDU_Q_SIZE )
    {
#else
    /** SPR 6961 Changes Start **/
    if ((txAMEntity_p->txQueueSize >= txAMEntity_p->rlcQueueSize) || 
        (amdSDUCount >= MAX_AM_RAW_SDU_Q_SIZE ))
        /* - SPR 19808 */
    {
    /* Uplink ThroughPut Fix End */
#endif
#if defined (RLC_PDCP_IPC_INTF) && !defined UE_SIM_TESTING
        UInt32 rlcStatusBufferCount = 0;
        RlcStatusBuffer *rlcQStatusBuffer_p = PNULL;

        rlcQStatusBuffer_p =   getMemFromPool( sizeof(RlcStatusBuffer) ,RLC_NULL);
        if( RLC_NULL == rlcQStatusBuffer_p)
        {
            ltePanic("%s => Memory Allocation Failure",__FUNCTION__);
            return RLC_FAILURE;
        }

        rlcQStatusBuffer_p[rlcStatusBufferCount].transId = pAmdSdu->transId;
        rlcQStatusBuffer_p[rlcStatusBufferCount].deliveryStatus = RLC_PDCP_SDU_ENQUEUE_FAILURE;
        rlcStatusBufferCount++;
        CLOCK_START_RLC_API(RLC_DLVRY_STATUS_IND);
        rlcDeliveryStatusInd(pAmdSdu->ueIndex, pAmdSdu->lcId, rlcStatusBufferCount,
                rlcQStatusBuffer_p);
        CLOCK_END_RLC_PDCP_API(RLC_DLVRY_STATUS_IND);
#endif

        msgFree(pAmdSdu->data_p);
        pAmdSdu->data_p = RLC_NULL;
        freeMemPool(pAmdSdu);
        /* COVERITY CID 25541 FIX START */
        /* COVERITY CID 25541 FIX END */

        ret = RLC_FAILURE;
		/* + PERF_CA GBL */
        lteRlcStatInfo_g[cellIndex].lteAMStat.amdSduDropped++;
		/* - PERF_CA GBL */
        lcStats_p->numPktsDropped++;
        LOG_UT( LTE_RLC_RECV_SDU_ERR, LOGWARNING, (RLC_TX|RLC_AM),
                 globalTTITickCount_g, __LINE__, 0,
                 0, 0, 0, 0, 0, __func__, "");
        /* SPR 16766 fix start */
        LOG_RLC_MSG (LTE_RLC_RX_AM_SDU_ERR, LOGWARNING, RLC_TX, 
                globalTTITickCount_g,txAMEntity_p->ueIndex,
                txAMEntity_p->lcId,txAMEntity_p->txQueueSize ,\
                txAMEntity_p->rlcQueueSize,
                0,0,0,  
                __func__, "");
        return ret;
       /* - SPR_17858_567_CHANGES */
    }
    else 
    {  
        /*SPR 16936 fix*/
        if ( amdSDUCount == 0)
        {
            txAMEntity_p->headPktTS = pAmdSdu->pktArrTime;
        }
        /*SPR 16936 fix*/
        /* SPR 4804 Fix Start */ 
        /* enqueue the SDU */
        /*** Coverity 25305 Fix : Start ****/
        if (!ENQUEUE_AMD_RAW_SDU_Q(amdRawSduQ_p, pAmdSdu))
        {
#ifdef DEBUG_STATS
            txAMEntity_p->enQFailedSduQ++;
#endif

            msgFree(pAmdSdu->data_p);
            pAmdSdu->data_p = RLC_NULL;
            freeMemPool(pAmdSdu);
            return RLC_FAILURE;
        }
        /*** Coverity 25305 Fix : End ****/
        /* SPR 4804 Fix End */
 
        /* Add QLoad */
        /* SPR 12612 Start */
        atomicAddInt(&txAMEntity_p->txQueueSize,pAmdSduSize);
        /* SPR 12612 End */
       
       /* SPR 17554 start */
        atomicAddInt(&amdRawSduQ_p->enqueueCount, 1);
        /* SPR 17554 End */
        
        /* Update the stats*/
        lteRlcStatInfo_g[cellIndex].lteAMStat.amdSduReceivedFromPdcp++;
        lteRlcStatInfo_g[cellIndex].lteUEStat.rlcUeStats[txAMEntity_p->ueIndex].
            sduReceivedFromPdcp++;
        lcStats_p->sduNumDl++;
        /* SPR 12612 Start */
        lcStats_p->sduBytesDl += pAmdSduSize;
        /* SPR 12612 End */
    } 
    /* + SPR 12349 Changes */

    /*SPR 16936 fix*/
    lcQLoad.headPktTS = txAMEntity_p->headPktTS;
    /*SPR 16936 fix*/
    rlcFetchQueueLoad(txAMEntity_p, &lcQLoad);
    /* - SPR 12349 Changes */
/*****************************************************************/
    if( !isWithInTxWindow( vtA, vtS,vtMS))
    {
        lcQLoad.ctrlFlag = RLC_WAIT_FOR_STATUS_PDU;
        macUpdateBufferOccupancyReq(txAMEntity_p->ueIndex, 
               1 /*LC Count*/, &lcQLoad,RLC_BO_TYPE_NEW,
              rlcUEContextList_g[txAMEntity_p->ueIndex]\
                                          ->cellIndex
                                          );
        return ret;

    }
/*******************************************************************/    

    macUpdateBufferOccupancyReq( txAMEntity_p->ueIndex, 
                                 1 /*LC Count*/, 
                                 &lcQLoad,
                                 RLC_BO_TYPE_NEW,
                                rlcUEContextList_g[txAMEntity_p->ueIndex]\
                                          ->cellIndex
                               );
    /*NEW_LOG_REQ*/
    LOG_RLC_MSG ( LTE_RLC_RX_AM_SDU, LOGINFO, 
            RLC_TX, globalTTITickCount_g,txAMEntity_p->ueIndex,
            txAMEntity_p->lcId,lcQLoad.lcQueueLoad,lcQLoad.retxnPduSize,
            lcQLoad.ctrlQueueLoad, txAMEntity_p->reTxQueueSize,  
            /* SPR 12612 Start */
            pAmdSduSize,
            /* SPR 12612 End */
             __func__, "");
    return ret ;
}

 /****************************************************************************
 * Function Name  : rlcReceiveAmSduDiscardFromUpperLayer 
 * Inputs         : txAMEntity_p - a pointer to TxAMEntity,
 *                  transId - sdu transId
 * Outputs        : None
 * Returns        : RLC_SUCCESS if case success else RLC_FAILURE
 * Description    : describes actions when AMD SDU referd by @pAmdSdu is
 *                  recieved for AM Entity @txAMEntity_p.
 ****************************************************************************/

UInt32 rlcReceiveAmSduDiscardFromUpperLayer( TxAMEntity * txAMEntity_p,
                                                  /* SPR 2812 changes start */
                                                  void *transId
                                                  /* SPR 2812 changes end */
                                                  )
{
    AmdRawSduQ *amdRawSduQ_p = &txAMEntity_p->txSDUQueue;
    LCQueueLoad lcQLoad = {0};
    RlcUeDataReq * amdRawSdu_p = RLC_NULL ;
    UInt16 lcCount = 1;
    UInt32 sduSize = 0;
        /* SPR 14622 fix start */
    /* search and delete the SDU requested by PDCP*/
    sduSize = rlcSearchNdeleteAmdSduFromQ(amdRawSduQ_p,transId);
    if ( sduSize )
    {
#if defined (RLC_PDCP_IPC_INTF) && !defined UE_SIM_TESTING
        UInt32 rlcStatusBufferCount = 0;
        RlcStatusBuffer *rlcQStatusBuffer_p = PNULL;

        rlcQStatusBuffer_p =   getMemFromPool( sizeof(RlcStatusBuffer) ,RLC_NULL);
        if( RLC_NULL == rlcQStatusBuffer_p)
        {
            ltePanic("%s => Memory Allocation Failure",__FUNCTION__);
            return RLC_FAILURE;
        }
        rlcQStatusBuffer_p[rlcStatusBufferCount].transId = transId;
#endif
        /* SPR 14622 fix end */

        atomicSubtractInt(&txAMEntity_p->txQueueSize,sduSize);
        lcQLoad.lcId = txAMEntity_p->lcId;
        rlcFetchQueueLoad(txAMEntity_p,&lcQLoad);
        lcQLoad.scheduledDataBytes = 0;
        gRlcStats.pRlcStats->lteRlcUeStat[txAMEntity_p->ueIndex].
            lcStats[txAMEntity_p->lcId].numPktsDiscarded ++;

/* QOS */
/*******************************************************************/
        /* SPR 12610 Start */
        if( amdRawSduQ_p->pQ->count > 0)
        {
        amdRawSdu_p = (RlcUeDataReq *)(amdRawSduQ_p->pQ->ring[amdRawSduQ_p->pQ->head].data);
            lcQLoad.headPktTS = amdRawSdu_p->pktArrTime;
        }
        else 
        {
            lcQLoad.headPktTS = 0;
        }
        /* SPR 12610 End */
        lcQLoad.ctrlFlag = RLC_NORMAL_DATA; 
/********************************************************************/        
        
        macUpdateBufferOccupancyReq(txAMEntity_p->ueIndex,
                lcCount, &lcQLoad, RLC_BO_TYPE_NEW,
                rlcUEContextList_g[txAMEntity_p->ueIndex]->cellIndex
                );

        /* SPR 3406 changes start */
#if defined (RLC_PDCP_IPC_INTF) && !defined UE_SIM_TESTING
        /* SPR 14622 fix start */
        rlcQStatusBuffer_p[rlcStatusBufferCount].deliveryStatus = RLC_DISCARD_SUCCESS;
        /* SPR 14622 fix end */
        rlcStatusBufferCount++;
        CLOCK_START_RLC_API(RLC_DLVRY_STATUS_IND);
        rlcDeliveryStatusInd(txAMEntity_p->ueIndex, txAMEntity_p->lcId, rlcStatusBufferCount,
                rlcQStatusBuffer_p);
        CLOCK_END_RLC_PDCP_API(RLC_DLVRY_STATUS_IND);
        return RLC_SUCCESS ;
#else
        return RLC_SUCCESS ;
#endif
    }
    return RLC_FAILURE ;
}

/****************************************************************************
 * Function Name  : rlcEncodeControlPdu 
 * Inputs         : pAmRxE - pointer to AM RX Entity,
 *                  rlcDataBuffer_p - pointer to the RLCDataBuffer,
 *                  txOpp_p - TX Opportunity given by MAC Layer
 * Outputs        : None
 * Returns        : It will return the errorCode
 * Description   : This function will do the following things:-
 *                  1. Encode Status PDU 
 *                  2. Assign the SequenceNumber, Size, data_p to rlcDataBuffer_p
 *                  3. Start Status Probhibit Timer. 
 *                  4. Updating the remaing txOpportunity. 
 ****************************************************************************/
static UInt32 rlcEncodeControlPdu(
                                          RxAMEntity* pAmRxE,
                                          RLCDataBuffer* rlcDataBuffer_p,
                                          UInt32 *txOpp_p
                                        )
{
    /*Coverity 108787 Fix Start*/
    RlcPduBuffer pduFormed = {0};  
    /*Coverity 108787 Fix End*/
    UInt32 errorCode       = INVALID_ERR_CODE;
    UInt32 txOpp           = *txOpp_p;
    UInt32 prlcStatusPduSendSts=0;

    /* encode Control PDu */
    errorCode = rlcEncodeStatusPdu( pAmRxE,
                                    &pduFormed, 
                                    txOpp,
                                    &prlcStatusPduSendSts
                                   );
    if( RLC_SUCCESS == errorCode)
    {
        rlcDataBuffer_p->statusPduLen = pduFormed.size;
        rlcDataBuffer_p->statusPdu_p  = pduFormed.data_p;
        /** SPR 3657 Changes Start **/
        rlcDataBuffer_p->totalPduLen +=  pduFormed.size;
        
        /** SPR 3657 Changes End **/
        /** Rel 4.0 changes start **/
#ifndef UE_SIM_TESTING
        rlcDataBuffer_p->sduCount = 0;
        /* SPR 16916 FIX START */
        rlcDataBuffer_p->qci = pAmRxE->qci;
        /* SPR 16916 FIX END */
#endif
        /** Rel 4.0 changes end **/

        /* Start Status Probhibit Timer */
        rlcRxStartStatusProhibitTimer(pAmRxE);
 
        /* updating the remaing txOpportunity */ 
        *txOpp_p = (txOpp - pduFormed.size); 
        LteRlcLCStats* lcStats_p =
                &(gRlcStats.pRlcStats->lteRlcUeStat[pAmRxE->ueIndex].
                          lcStats[pAmRxE->lcId]);

        
        lcStats_p->pduBytesDl += rlcDataBuffer_p->statusPduLen;
        lcStats_p->pduNumDl++;
        lcStats_p->dlThroughput += rlcDataBuffer_p->statusPduLen;
        gRlcStats.pRlcStats->lteRlcUeStat[pAmRxE->ueIndex].
            dlThrput += rlcDataBuffer_p->statusPduLen;
        gRlcStats.pRlcStats->lteSysStat.sysDlThroughput += 
            rlcDataBuffer_p->statusPduLen;
    }
    else
    {
        rlcDataBuffer_p->statusPduLen = 0;
        rlcDataBuffer_p->statusPdu_p  = RLC_NULL;
	    
        LOG_RLC_MSG( LTE_RLC_ENCODE_STATUS_PDU, LOGERROR, (RLC_TX|RLC_AM), 
                 globalTTITickCount_g, __LINE__, pAmRxE->ueIndex,
                 pAmRxE->lcId, errorCode, 0, 0,0, __func__, "Encoding of STATUS PDU Failed");	   
        
    }

    if( prlcStatusPduSendSts == RLC_AM_NACK_SN_PART_IN )
    {
        errorCode = RLC_PARTIAL_SUCCESS;
    }

    return errorCode;
}


/****************************************************************************
 * Function Name  : rlcEncodeRetxPdu 
 * Inputs         : pAmTxE - Its point to the AM Tx Entity,
 *                  rlcDataBuffer_p - Pointer to RLCDataBuffer,
 *                  lcQLoadArr_p - Its point to the LCQueueLoad,
 *                  rlcHarqFailureInfo_p - RLCHarqFailureInfo,
 *                  txOpp_p - TX Opportunity given by MAC Layer 
 * Outputs        : None
 * Returns        : It will return the errorCode
 * Description    : This function will do following things :-
 *                  1. Encode Retx Pdu or Segment PDU
 *                  2. Update the RLCDataBuffer structure for MAC Layer
 *                  3. Update scheduledDataBytes in lcQLoadArr_p
 *                  4. If polling bit is set then start PollingRetransmitTimer.
 ****************************************************************************/

static UInt32 rlcEncodeRetxPdu( 
                                       TxAMEntity* pAmTxE,
                                       RLCDataBuffer* rlcDataBuffer_p,
                                       LCQueueLoad * lcQLoadArr_p,
                                       RLCHarqFailureInfo* rlcHarqFailureInfo_p,
                                       UInt32 *txOpp_p
                                    )

{
    RlcPduBuffer pduFormed;
    /** SPR 3722 Changes Start **/
    pduFormed.data_p = RLC_NULL; 
    /** SPR 3722 Changes End **/
    UInt32 txOpp = *txOpp_p;
    /** SPR 3657 Changes Start **/
    rlcReTxPduNode *rlcReTxPduNode_p = RLC_NULL;
    /** SPR 3657 Changes End **/ 
    UInt32 errorCode = INVALID_ERR_CODE;
    UInt8  pollBit   = 0;
    /* SPR 15909 fix start */
    tickType_t pktArrTime[MAX_RLC_SDU_IN_PDU_FOR_KPI]={0};
    /* SPR 15909 fix end */
#ifndef UE_SIM_TESTING
    UInt8 sduCount   = 0;
#endif

    /*SPR 21723 Fix Start */
    RlcSn seqNum = 0;
    AmReTransmission * amReTxPdu_p = RLC_NULL;
    /*SPR 21723 Fix End */    

    /* encode ReTx PDU */
    errorCode = rlcEncodeAmdReTxPdu( pAmTxE,
            &pduFormed,
            txOpp,
            &pollBit, 
            rlcHarqFailureInfo_p,
            pktArrTime
            ); 

    if ( RLC_SUCCESS == errorCode )
    {
        /** SPR 3657 Changes Start **/
        rlcReTxPduNode_p = (rlcReTxPduNode *)getMemFromPool(sizeof(rlcReTxPduNode), RLC_NULL);
        if(rlcReTxPduNode_p == RLC_NULL)
        {
            /*SPR 21723 Fix Start */
            seqNum = GET_RETX_QUEUE_INDEX_FROM_SN(pAmTxE->txPduIn.sn);
            amReTxPdu_p = pAmTxE->reTxQueue[seqNum];
            if(RLC_NULL != amReTxPdu_p)
            {
                amReTxPdu_p->reTxCount--;
            }
#ifdef PRINT
            fprintf(stderr,"\n%s: %d : Pool Alloc Failed \n",__func__,__LINE__);
#endif
            /*SPR 21723 Fix End */    
            msgFree(pduFormed.data_p);
            /* SPR 20985 fix start */
            pduFormed.data_p = RLC_NULL;
            /* SPR 20985 fix end */
            return LTE_RLC_MEM_ALLOC_FAIL;
        }
        rlcReTxPduNode_p->retxPduSeqNum = pduFormed.seqNum;
        rlcReTxPduNode_p->retxPduLen    = pduFormed.size;
        rlcReTxPduNode_p->retxRlcPdu_p  = pduFormed.data_p;
        rlcDataBuffer_p->totalPduLen += pduFormed.size;
        ENQUEUE_AMD_RETX_PDU_Q(rlcDataBuffer_p->reTxRlcPduQ,rlcReTxPduNode_p); 
        /** SPR 3657 Changes End **/
        /** Rel4.0 changes Start **/
#ifndef UE_SIM_TESTING
        rlcDataBuffer_p->qci = pAmTxE->qci;
        rlcDataBuffer_p->sduCount = pduFormed.sduCount;
        for(sduCount = 0; sduCount < rlcDataBuffer_p->sduCount;sduCount++)
        {
            rlcDataBuffer_p->pktArrTime[sduCount] = pktArrTime[sduCount];
        }
#endif
        /** Rel4.0 changes End **/


        LteRlcLCStats* lcStats_p =
            &(gRlcStats.pRlcStats->lteRlcUeStat[pAmTxE->ueIndex].
                    lcStats[pAmTxE->lcId]);

        /** SPR 3657 Changes Start **/
        lcStats_p->pduBytesDl += pduFormed.size;
        lcStats_p->pduNumDl++;
        lcStats_p->dlThroughput += pduFormed.size;
        gRlcStats.pRlcStats->lteRlcUeStat[pAmTxE->ueIndex].
            dlThrput += pduFormed.size;
        gRlcStats.pRlcStats->lteSysStat.sysDlThroughput += 
            pduFormed.size;
        /** SPR 3657 Changes End **/

        txOpp -= pduFormed.size; 
    }
    else
    {
        /*rlcDataBuffer_p->retxPduSeqNum = INVALID_SEQ_NUM;
        rlcDataBuffer_p->retxPduLen    = 0;
        rlcDataBuffer_p->retxRlcPdu_p  = RLC_NULL; */
        
        LOG_RLC_MSG( LTE_RLC_ENCODE_RETX_PDU, LOGERROR, (RLC_TX|RLC_AM), 
                 globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                 pAmTxE->lcId, errorCode, 0, 0,0, __func__, "Encoding of RETX PDU Failed");	   
    }
    /* QoS perspective Retx case should be zero */
    lcQLoadArr_p->scheduledDataBytes = 0;
    /* if RLC PDU polling bit set 1 then start the 
       poll retransmit timer */
    if ( 1 == pollBit )
    {
        rlcTxStartPollRetransmitTimer(pAmTxE); 
    }

    *txOpp_p = txOpp;

    return errorCode;
}


/*******************************************************************************
 * Function Name  : rlcEncodeNewPdu 
 * Inputs         : pAmTxE - Pointer to TxAMEntity,
 *                  rlcDataBuffer_p - Pointer to RLCDataBuffer,
 *                  lcQLoadArr_p - Pointer to LCQueueLoad,
 *                  rlcHarqFailureInfo_p - Pointer to RLCHarqFailureInfo,
 *                  txOpp - TX Opportunity given by MAC Layer
 * Outputs        : None
 * Returns        : It will return the errorCode
 * Description    : This function will do following things :-
 *                  1. Encode the New RLC PDU.
 *                  2. Update the RLCDataBuffer for MAC Layer.
 *                  3. Update scheduledDataBytes in lcQLoadArr_p
 *                  4. If polling bit is set then start PollingRetransmitTimer.
 *******************************************************************************/
static UInt32 rlcEncodeNewPdu(  
                                      TxAMEntity* pAmTxE,
                                      RLCDataBuffer* rlcDataBuffer_p,
                                      LCQueueLoad * lcQLoadArr_p,
                                      RLCHarqFailureInfo* rlcHarqFailureInfo_p,
                                      UInt32 txOpp
                                    )
{
/* KPI Changes Start */
    /* coverity 13489 starts */
    RlcPduBuffer pduFormed = {0};
    /* coverity 13489 ends */
/* KPI Changes End */
    UInt8 pollBit    = 0;
    UInt32 errorCode = INVALID_ERR_CODE;
    /* SPR 15909 fix start */
    tickType_t pktArrTime[MAX_RLC_SDU_IN_PDU_FOR_KPI]={0};
    /* SPR 15909 fix end */
#ifndef UE_SIM_TESTING
    UInt8 sduCount   = 0;
/* KPI Changes Start */
#if defined(KPI_STATS) || defined(PERF_STATS)
    UInt8 count = 0;
#endif
/* KPI Changes End */
#endif

    errorCode = rlcEncodeAmdDataRawPdu( pAmTxE,
            &pduFormed,
            txOpp, 
            &pollBit,
            pktArrTime
            );

    if (RLC_SUCCESS == errorCode)
    {
        LOG_RLC_MSG( LTE_RLC_ENCODE_NEW_PDU2, LOGINFO, (RLC_TX|RLC_AM), 
                globalTTITickCount_g, pAmTxE->ueIndex,
                pAmTxE->lcId, pduFormed.seqNum, pAmTxE->vtA, 0, 0,0, __func__, "");	   

        rlcDataBuffer_p->newPduSeqNum = pduFormed.seqNum;
        rlcDataBuffer_p->newPduLen    = pduFormed.size;
        rlcDataBuffer_p->newRlcPdu_p  = pduFormed.data_p;
        /** SPR 3657 Changes Start **/
        rlcDataBuffer_p->totalPduLen += pduFormed.size;
        /** SPR 3657 Changes End **/
        /** Rel4.0 changes Start **/
#ifndef UE_SIM_TESTING
        rlcDataBuffer_p->qci = pAmTxE->qci;
        rlcDataBuffer_p->sduCount = pduFormed.sduCount;
        for(sduCount = 0; sduCount < rlcDataBuffer_p->sduCount;sduCount++)
        {
            rlcDataBuffer_p->pktArrTime[sduCount] = pktArrTime[sduCount];
        }
/* KPI Changes Start */
#if defined(KPI_STATS) || defined(PERF_STATS)
        /* Populate individaul PDCP SDU Size info in rlcDataBuffer_p */
        rlcDataBuffer_p->completeEncodedSduCount =  pduFormed.completeEncodedSduCount;
        for(;count < pduFormed.completeEncodedSduCount; count++)
        {
            rlcDataBuffer_p->pdcpSduSize[count] = pduFormed.pdcpSduSize[count];
        }
#endif
/* KPI Changes End */
#endif
        /** Rel4.0 changes End **/

        lcQLoadArr_p->scheduledDataBytes = pduFormed.payLoadSize;
        lcQLoadArr_p->headPktTS          = pduFormed.headPktTS;  
        /* Filling the rlcHarqFailureInfo  structure  */
        rlcHarqFailureInfo_p->newPduSeqNum = pduFormed.seqNum;
        rlcHarqFailureInfo_p->SOStart      = 0;
        rlcHarqFailureInfo_p->SOEnd        = 0;
        rlcHarqFailureInfo_p->harqFailureReporting 
            = REPORTING_TRUE;

        /* Increment the Sequence Number */
        pAmTxE->vtS = (pAmTxE->vtS + 1) & 1023;
        txOpp -= pduFormed.size; 
            LteRlcLCStats* lcStats_p =
            &(gRlcStats.pRlcStats->lteRlcUeStat[pAmTxE->ueIndex].
                    lcStats[pAmTxE->lcId]);
        lcStats_p->pduBytesDl += rlcDataBuffer_p->newPduLen;
        lcStats_p->pduNumDl++;
        lcStats_p->dlThroughput += rlcDataBuffer_p->newPduLen;
        gRlcStats.pRlcStats->lteRlcUeStat[pAmTxE->ueIndex].
            dlThrput += rlcDataBuffer_p->newPduLen;
        gRlcStats.pRlcStats->lteSysStat.sysDlThroughput += 
            rlcDataBuffer_p->newPduLen;

    }
    else
    {
        rlcDataBuffer_p->newPduSeqNum = INVALID_SEQ_NUM;
        rlcDataBuffer_p->newPduLen    = 0;
        rlcDataBuffer_p->newRlcPdu_p  = RLC_NULL;

        lcQLoadArr_p->scheduledDataBytes = 0;
        rlcHarqFailureInfo_p->harqFailureReporting = 0;
        /* SPR 10985 START */
        /* SPR 21075 Start*/
        if(LTE_RLC_VALIDATION_FAILED_SN_OUTSIDE_TX_WINDOW != errorCode)
        {
        /* SPR 21075 End*/
            pollBit = 0;
        /* SPR 21075 Start*/
        }
        /* SPR 21075 End*/
        /* SPR 10985 END */

        /*SPR 15251 Fix*/
        LOG_RLC_MSG( LTE_RLC_ENCODE_NEW_PDU, LOGWARNING, (RLC_TX|RLC_AM), 
                /*SPR 15251 Fix*/
                globalTTITickCount_g, __LINE__,  pAmTxE->ueIndex,
                pAmTxE->lcId, errorCode, 0, 0,0, __func__, "Encoding of NEW PDU Failed");	   

    }
    /* Start 128 UE: 25% NACK Changes */
    /*
    ** Start Poll retransmit timer in case of Tx window stall and 
    ** pollBit is set 0x0F.
    */
    if (pollBit)
    {
    /* End 128 UE: 25% NACK Changes */
        rlcTxStartPollRetransmitTimer(pAmTxE); 
    }
    
    return errorCode;
}
#ifndef 	LINUX_PC_TEST								
/** SPR 3657 Changes Start **/
#define MAC_TWO_BYTE_SUB_HEADER 2
#define MAC_THREE_BYTE_SUB_HEADER 3
#else
#define MAC_TWO_BYTE_SUB_HEADER 0
#define MAC_THREE_BYTE_SUB_HEADER 0
#endif
/** SPR 3657 Changes End **/

/****************************************************************************
 * Function Name  : rlcHandleTxOppForAMEntity 
 * Inputs         : amEntity_p - Pointer to AMEntity,
 *                  rlcDataBuffer_p - Pointer to RLCDataBuffer,
 *                  lcQLoadArr_p - Pointer to LCQueueLoad,
 *                  rlcHarqFailureInfo_p - Pointer to RLCHarqFailureInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS if case success else RLC_FAILURE
 * Description    : This function will do following things:-
 *                  1. Fetch the TX and RX Entity from AM Entity
 *                  2. Check If any Control PDU  
 *                  3. Check if any Retx or Segment 
 *                  4. Check if any New data to transmit.
 ****************************************************************************/
UInt32 rlcHandleTxOppForAMEntity( 
                                         AMEntity* amEntity_p,
                                         RLCDataBuffer* rlcDataBuffer_p,
                                         LCQueueLoad * lcQLoadArr_p,
                                         RLCHarqFailureInfo* rlcHarqFailureInfo_p
                                       )
{
    UInt32 errorCode = INVALID_ERR_CODE;
    
    UInt32 txOpp = rlcDataBuffer_p->requestedDataSize;
    /* Prev function already check for NULL pointer */
    TxAMEntity* pAmTxE = amEntity_p->txAMEntity_p;
    RxAMEntity* pAmRxE = amEntity_p->rxAMEntity_p;
    UInt32 encodedDataSize = 0;
    /** SPR 3657 Changes Start **/
    UInt32 pduCount = 0;
    rlcDataBuffer_p->totalPduLen = 0;
    UInt32 totalDataLen = 0;
    /** SPR 3657 Changes End **/
    /***RLC_CONTROL_PDU - STATUS PDU ****/
    if ( RLC_CONTROL_PDU(pAmTxE, txOpp, pAmRxE,pduCount))
    {
        errorCode = rlcEncodeControlPdu ( pAmRxE, rlcDataBuffer_p,
                &txOpp );
        if ( RLC_SUCCESS == errorCode )
        {
            lcQLoadArr_p->scheduledDataBytes = 0;
            pAmTxE->rxStatusPduSize          = 0;
            pAmTxE->sendStatusInd            = RLC_FALSE;
            rlcHarqFailureInfo_p->harqFailureReporting 
                = REPORTING_FALSE;
            /** SPR 3657 Changes Start **/
            encodedDataSize = totalDataLen = rlcDataBuffer_p->totalPduLen;
            pduCount++;
            /** SPR 3657 Changes End **/
            LOG_RLC_MSG( LTE_RLC_AM_STATUS_PDU, LOGINFO, (RLC_TX|RLC_AM), 
                    globalTTITickCount_g, __LINE__, pAmRxE->ueIndex,
                    pAmRxE->lcId, 0, 0, 0,0, __func__, "");	  

        }
        else if( RLC_PARTIAL_SUCCESS == errorCode )
        {
            encodedDataSize = totalDataLen = rlcDataBuffer_p->totalPduLen;
            pduCount++;
        }
        else
        {
#ifdef DEBUG_STATS
            pAmTxE->encodingFailedStatus++;
#endif
        }
    }
    /****RLC_RETX_DATA_PDU or AMD PDU segment*********/
#ifndef UE_SIM_TESTING
    /** SPR 3657 Changes Start **/
    while ( RLC_RETX_PDU(pAmTxE, txOpp,pduCount ))
    /** SPR 3657 Changes End **/
#else
    /* Uplink Throughput Fix Start */
    if ( RLC_RETX_PDU(pAmTxE, txOpp,pduCount ))
#endif
    {
        /* SPR 21732 Fix + */
        /* SPR 21732 Fix - */
    /* Uplink Throughput Fix End */
        if(encodedDataSize )
         {
             if(encodedDataSize <= MAX_TWO_BYTE_SUBHEADER)
             {
                 /** SPR 3657 Changes Start **/
                 txOpp -= MAC_TWO_BYTE_SUB_HEADER;
             }
             else
             {
                 txOpp -= MAC_THREE_BYTE_SUB_HEADER;
             }
                 /** SPR 3657 Changes End **/
         }


        errorCode = rlcEncodeRetxPdu( pAmTxE, rlcDataBuffer_p, lcQLoadArr_p,
                rlcHarqFailureInfo_p, &txOpp );
        if(errorCode == RLC_SUCCESS)
        {
            /** SPR 3657 Changes Start **/
            encodedDataSize = (rlcDataBuffer_p->totalPduLen - totalDataLen);
            totalDataLen += encodedDataSize;
            pduCount++;
       
            /** SPR 3657 Changes End **/
            LOG_RLC_MSG( LTE_RLC_AM_STATUS_PDU_1, LOGINFO, (RLC_TX|RLC_AM), 
                    globalTTITickCount_g, __LINE__, pAmRxE->ueIndex,
                    pAmRxE->lcId, 0, 0, 0,0, __func__, "");	   
        }
        /* SPR 4443 Fix Start */
        else
        {
#ifdef DEBUG_STATS
            pAmTxE->encodingFailedReTx++;
#endif
            return errorCode;
        }
        /* SPR 4443 Fix End */
    }
    /*** New AMD PDU***/
    /** SPR 3657 Changes Start **/
#ifndef UE_SIM_TESTING
    if( RLC_NEW_PDU(pAmTxE, txOpp,pduCount))
#else
    /* Uplink Throughput Fix Start */
    if( RLC_NEW_PDU(pAmTxE, txOpp,pduCount))
#endif
    {
    /* Uplink Throughput Fix End */
        if(encodedDataSize )
        {
            if(encodedDataSize <= MAX_TWO_BYTE_SUBHEADER)
            {
                txOpp -= MAC_TWO_BYTE_SUB_HEADER;
            }
            else
            {
                txOpp -= MAC_THREE_BYTE_SUB_HEADER;
            }
        }
    /** SPR 3657 Changes End **/

        errorCode = rlcEncodeNewPdu( pAmTxE, rlcDataBuffer_p, lcQLoadArr_p,
                                        rlcHarqFailureInfo_p, txOpp );   
        if(errorCode == RLC_SUCCESS)
        {
            LOG_RLC_MSG( LTE_RLC_AM_STATUS_PDU_2, LOGINFO, (RLC_TX|RLC_AM), 
                    globalTTITickCount_g, __LINE__, pAmRxE->ueIndex,
                    pAmRxE->lcId, 0, 0, 0,0, __func__, "");	   
        }
        else
        {
#ifdef DEBUG_STATS
            pAmTxE->encodingFailedNewTx++;
#endif
        }


    }

    return errorCode;
}
