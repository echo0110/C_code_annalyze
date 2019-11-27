/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcAmdArqOperation.c,v 1.1.6.2.2.2 2010/10/07 13:34:20 gur23054 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file having RLC AM ARQ functionality
 *
 ****************************************************************************
 * Revision Details
 * ----------------
 * $Log: lteRlcAmdArqOperation.c,v $
 *
 *
****************************************************************************/


/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcAmArithmetic.h"
#include "lteRlcAmdEncode.h"
#include "lteRlcPdcpInterface.h"
#include "lteRlcAmdArqOperation.h"
#include "lteRlcUeContext.h"
#include "logging.h"
#include "alarm.h"
#include "lteRlcTimer.h"
#include "ltePerfStats.h"
/*SPR 21770 Changes start*/
#include "atomic-ops.h"
/*SPR 21770 Changes end*/
/* SPR 16843 start */
#if (defined KPI_STATS) && !(defined UE_SIM_TESTING)
#include "lteCommonStatsManager.h"
#endif
/* SPR 16843 end */
/****************************************************************************
 * Varibale Definition
 ****************************************************************************/
LTE_SQUEUE rlcControlPduQ_g ;
extern void *zeroPool_g;
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */

#define RLC_AM_NACK_SN_PAIR_SIZE 12
#define RLC_AM_NACK_SO_PAIR_SIZE 30
/****************************************************************************
 * Function Name  : isAmTxWindowStall
 * Inputs         : pAmTxE - Tx Am Entity pointer
 * Outputs        : None
 * Returns        : RLC_WINDOW_STALL/RLC_WINDOW_IS_NOT_STALL
 * Description    : This function returns the poll bit value.
 ****************************************************************************/
#define RLC_WINDOW_IS_NOT_STALL    0
#define RLC_WINDOW_STALL           1

static int isAmTxWindowStall( TxAMEntity* pAmTxE )
{
   /* Check for Window Stalling */
    RlcVr vtA  = pAmTxE->vtA;
    RlcVr vtMS = pAmTxE->vtMS;
    RlcVr vtS  = (pAmTxE->vtS+1)&1023;//bug[002]

    if ( isWithInTxWindow(  vtA, vtS, vtMS) )
    {
        return RLC_WINDOW_IS_NOT_STALL;
    }

    return RLC_WINDOW_STALL;

}


/* Reference 3GPP T.S 36.322 Section 5.2.2 Polling
 
 Upon assembly of a new AMD PDU, the transmitting side of an AM RLC entity shall:

-   increment PDU_WITHOUT_POLL by one;
-   increment BYTE_WITHOUT_POLL by every new byte of Data field element 
    that it maps to the Data field of the RLC data PDU;
-   if PDU_WITHOUT_POLL >= pollPDU; or
-   if BYTE_WITHOUT_POLL >= pollByte;
-   include a poll in the RLC data PDU as described below.
    Upon assembly of an AMD PDU or AMD PDU segment, 
    the transmitting side of an AM RLC entity shall:
    
-   if both the transmission buffer and the retransmission buffer becomes empty 
    (excluding transmitted RLC data PDU awaiting for acknowledgements)
    after the transmission of the RLC data PDU; or
-   if no new RLC data PDU can be transmitted after the transmission 
    of the RLC data PDU (e.g. due to window stalling);

-   include a poll in the RLC data PDU as described below.
    To include a poll in a RLC data PDU, the transmitting side 
    of an AM RLC entity shall:
-   set the P field of the RLC data PDU to "1";
-   set PDU_WITHOUT_POLL to 0;
-   set BYTE_WITHOUT_POLL to 0;
*/
/****************************************************************************
 * Function Name  : rlcPollBitForNewAmdPdu 
 * Inputs         : pAmTxE - Tx Am Entity pointer,
 *                  byteWithoutPoll - bytes without poll 
 * Outputs        : None
 * Returns        : pollBit
 * Description    : This function returns the poll bit value.
 ****************************************************************************/

UInt8 rlcPollBitForNewAmdPdu ( TxAMEntity* pAmTxE,
                                       UInt32 byteWithoutPoll
                                      )
{
    UInt8  pollBit  = 0;
    UInt32 pollPDU  = pAmTxE->pollPDU;
    UInt32 PollByte = pAmTxE->pollByte;

/*    
-   increment PDU_WITHOUT_POLL by one;
-   increment BYTE_WITHOUT_POLL by every new byte of Data field element 
    that it maps to the Data field of the RLC data PDU;
*/    
    if ( pInfinity != pollPDU)
    {
        pAmTxE->pduWithoutPoll++;
    }
    
   // if (( kbinfinity*1024) != PollByte)
    if (( kbinfinity << 10) != PollByte)
    {
        pAmTxE->byteWithoutPoll = pAmTxE->byteWithoutPoll + byteWithoutPoll;
    }
/*    
-   if PDU_WITHOUT_POLL >= pollPDU; or
-   if BYTE_WITHOUT_POLL >= pollByte;
-   include a poll in the RLC data PDU as described below.
    Upon assembly of an AMD PDU or AMD PDU segment, 
    the transmitting side of an AM RLC entity shall:
-   if both the transmission buffer and the retransmission buffer becomes empty 
    (excluding transmitted RLC data PDU awaiting for acknowledgements)
    after the transmission of the RLC data PDU; or
-   if no new RLC data PDU can be transmitted after the transmission 
    of the RLC data PDU (e.g. due to window stalling);
-   include a poll in the RLC data PDU as described below.
    To include a poll in a RLC data PDU, the transmitting side 
    of an AM RLC entity shall:
-   set the P field of the RLC data PDU to "1";
-   set PDU_WITHOUT_POLL to 0;
-   set BYTE_WITHOUT_POLL to 0;
*/   

     /* doubt why first condition is here pAmTxE->pollBit ??? **/
    /* Condition to set poll bit */
    if ( ( pAmTxE->pollBit)                        ||
         ( pAmTxE->pduWithoutPoll >= pollPDU)      ||
         ( pAmTxE->byteWithoutPoll >= PollByte)    ||
	 ((0 == pAmTxE->txQueueSize)            &&
         (0 == pAmTxE->reTxQueueSize)         && 
           (0 == pAmTxE->txPduIn.pRawPdu.size)  &&
           (0 == pAmTxE->txSduIn.rlcAmRawSdu.size) 
          )                                        ||
          ( isAmTxWindowStall(pAmTxE) ) 
       )
    {
        pAmTxE->pduWithoutPoll  = 0;
        pAmTxE->byteWithoutPoll = 0;
        pollBit                 = 0x01 ;
        pAmTxE->pollBit         = 0x00;
    }
    return pollBit;
}

/****************************************************************************
 * Function Name  : rlcPollBitForReTxAmdPdu 
 * Inputs         : pAmTxE - Tx Am Entity pointer
 * Outputs        : None
 * Returns        : pollBit value SET/RESET
 * Description    : This function returns the poll bit value for Rx-Tx case.
 ****************************************************************************/

UInt8 rlcPollBitForReTxAmdPdu (TxAMEntity* pAmTxE)
{
    UInt8 pollBit = 0;
    RlcVr vtA = pAmTxE->vtA;
    RlcVr vtMS = pAmTxE->vtMS;
    RlcVr vtS  = pAmTxE->vtS;
    /* Check condition to set poll bit */
    /*if((pAmTxE->pollBit) || ((0 == pAmTxE->txQueueSize) && (0 == pAmTxE->reTxQueueSize) 
            && (0 == pAmTxE->txPduIn.pRawPdu.size)) ||
            (!( isAMLTE(vtA, vtS, vtA) && isAMLT (vtS, vtMS, vtA ) )))*/
    if((pAmTxE->pollBit) || 
		((0 == pAmTxE->txQueueSize) 
		&& (!QUEUE_COUNT_AM_NACK_LIST_Q(pAmTxE->nackList)) 
            && (0 == pAmTxE->txPduIn.pRawPdu.size)) ||
            (!( isAMLTE(vtA, vtS, vtA) && isAMLT (vtS, vtMS, vtA ) )))
   {
        pAmTxE->pduWithoutPoll = 0;
        pAmTxE->byteWithoutPoll = 0;
        pollBit = 0x01 ;
        pAmTxE->pollBit = 0x00;
    }
    return pollBit;
}

/****************************************************************************
 * Function Name  : rlcTxStartPollRetransmitTimer 
 * Inputs         : pAmTxE - Tx Am Entity pointer
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This function start the poll bit timer when ever sending 
 *                  PDU with poll bit.
 ****************************************************************************/

UInt32 rlcTxStartPollRetransmitTimer(TxAMEntity* pAmTxE)
{
    UInt32 ret = RLC_SUCCESS;
    pAmTxE->pollSN = (pAmTxE->vtS -1); 
    pAmTxE->pollSN = (pAmTxE->pollSN) & 1023;
    /* SPR 11571 changes start */
    UInt16 runningPollSN = 0xffff;
    /* SPR 11571 changes end */
    /* If RetransmittingTimerRunning is FALSE */ 
    if ( RLC_FALSE == pAmTxE->istRetransmitTimerRunning)
    {
        ret = startPRTimer(pAmTxE);
        pAmTxE->istRetransmitTimerRunning = RLC_TRUE;
    }
    /* SPR 11571 changes start */
    else
    {
        if (RLC_FAILURE == getPRRunningTimerPollSN(pAmTxE, &runningPollSN))
        {
            restartPRTimer(pAmTxE);
            pAmTxE->istRetransmitTimerRunning = RLC_TRUE;
        }
        else
        {
            if (runningPollSN != pAmTxE->pollSN)
             {
                 restartPRTimer(pAmTxE);
                 pAmTxE->istRetransmitTimerRunning = RLC_TRUE;
             }
        }
    }

    /* SPR 11571 changes end */
    return ret ;
}

/****************************************************************************
 * Function Name  : rlcRxStartStatusProhibitTimer 
 * Inputs         : pAmRxE - Rx Am Entity pointer
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This function start the Status Prohibit Timer when 
 *                : ever sending Control PDU to Peer RLC.
 ****************************************************************************/

UInt32 rlcRxStartStatusProhibitTimer(RxAMEntity* pAmRxE)
{
    if(RLC_FALSE == pAmRxE->isSPTimerRunning) 
    {
        if( (pAmRxE->spTimerInfo.spDuration) )
        {
            startSPTimer(pAmRxE);
            pAmRxE->isSPTimerRunning = RLC_TRUE;
        }    
        pAmRxE->sendStatusInd = RLC_FALSE;
        return RLC_SUCCESS;
    }
    return RLC_FAILURE;
}

/****************************************************************************
 * Function Name  : rlcActionOnNackReceiveStatusReport 
 * Inputs         : pAmTxE - Tx Am Entity pointer,
 *                  controlPdu_p - RAW pointer of Status PDU,
 *                  bitPos - bit position counter,
 *                  leastNackRcv
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This function decode the Status PDU.
 ****************************************************************************/

UInt32 rlcActionOnNackReceiveStatusReport( TxAMEntity* pAmTxE,
                                           UInt8 *controlPdu_p,
                                           UInt32 bitPos,
                                           RlcVr *leastNackRcv
                                         )
{
    UInt32 ret = RLC_SUCCESS;
    UInt32 e1  = 0;
    UInt32 e2  = 0;
    UInt32 Idx = 1;
    UInt32 nackSn   = 0;
    UInt32 Sostart  = 0;
    UInt32 Soend    = 0;

    RlcVr vtA    = pAmTxE->vtA;
    RlcVr vtMS   = pAmTxE->vtMS;

    AmNackListNode   *amNackList_p = RLC_NULL; 
    AmReTransmission *amReTxPdu_p  = RLC_NULL;
    RlcVr reTxIndex   = 0;
    UInt16 lcCount    = 1;
    LCQueueLoad lcQLoadArr = {0};

    
    AmdRawSduQ *amdRawSduQ_p = &pAmTxE->txSDUQueue;
    RlcUeDataReq * amdRawSdu_p = RLC_NULL ;

    RLC_GET_E1_AND_UPDATE_BIT_POS_IDX(controlPdu_p, e1, Idx, bitPos);

#ifdef DEBUG_STATS
    pAmTxE->rlcRxStatus[pAmTxE->statusCount].statusRecvd.count = 0;
	/*SPR 21769 Changes Start*/
    UInt16 nackCount = 0;
	/*SPR 21769 Changes End*/
#endif
    while(e1)
    {

        /* Get nack_sn and update the bitPos */
        RLC_GET_NACK_SN_AND_UPDATE_BIT_POS_IDX(controlPdu_p, nackSn, Idx, bitPos);
#ifdef DEBUG_STATS
    if(nackCount < MAX_RLC_RX_STATUS)
    {
        pAmTxE->rlcRxStatus[pAmTxE->statusCount].statusRecvd.nack_sn[nackCount] = nackSn;
    }
    nackCount++;
    pAmTxE->rlcRxStatus[pAmTxE->statusCount].statusRecvd.count = nackCount;
#endif
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
        /*EVENT: EVENT_PARAM_PER_UE_RLC_NACK_UL , The total number of unsuccessful 
         * RLC PDU and RLC PDU segment transmissions (NACKs) in the uplink direction.*/
        gRlcUePerfStats_p[pAmTxE->ueIndex].lteUeRlcULPerfStats.totalULRLCNacks++;
#endif

        /* Get e1 and update the bitPos */
        RLC_GET_E1_AND_UPDATE_BIT_POS_IDX(controlPdu_p, e1, Idx, bitPos);

        /* Get e2 and update the bitPos */
        RLC_GET_E2_AND_UPDATE_BIT_POS_IDX(controlPdu_p, e2, Idx, bitPos);
        if(e2)
        {
            /* Get so_start and update the bitPos */
            RLC_GET_SO_START_AND_UPDATE_BIT_POS_IDX(controlPdu_p, Sostart, Idx, bitPos);

            /* Get so_end and update the bitPos */
            RLC_GET_SO_END_AND_UPDATE_BIT_POS_IDX(controlPdu_p, Soend, Idx, bitPos);

        }
    
        LOG_RLC_MSG(LTE_RLC_NACK_RECEIVE_STATUS_4, LOGINFO , (RLC_TX|RLC_AM),
                globalTTITickCount_g, e1 , e2, nackSn, Sostart,
                Soend , 0, 0, __func__, "");

        if ( isAMLTE ( vtA, nackSn, vtA) && isAMLT ( nackSn, vtMS, vtA) )
        {
            /*a SN falls within of the transmitting window */
            reTxIndex =  nackSn & 511; 
            amReTxPdu_p = pAmTxE->reTxQueue[reTxIndex];
            /*If this will data size the we will miss the header in tx opp*/
            if ( amReTxPdu_p ) 
            {
                /* To check wehther Soend indicate missing portion of AMD PDU includes /
                   all bytes to the last byte of the AMD PDU  */
                if(!(Soend ^ SO_END_SPC_MASK))
                {
                    Soend = amReTxPdu_p->data_size - 1;
                }
                /*As per SPR 698 , ltePanic takes care of returning value, no change done*/
                CREATE_AM_NACK_LIST_NODE(amNackList_p, nackSn, Sostart, Soend);

                if(amNackList_p != RLC_NULL)
                {
                    /*** Coverity 25577 Fix : Start ****/
                    if (!ENQUEUE_AMD_NACK_LIST_Q(pAmTxE->nackList, amNackList_p))
                    {
                        freeMemPool(amNackList_p);
                        return RLC_FAILURE;
                    }
                    /*** Coverity 25577 Fix : End ****/
                    /* SPR 4315 - 4949 Fix Start */
		/*SPR 21770 changes start*/
	            atomicAddInt(&pAmTxE->reTxQueueSize,(amReTxPdu_p->pdu_size - AM_PDU_SEG_FIX_HDR_SIZE));
		/*SPR 21770 changes end*/
                    /* SPR 4315 - 4949 Fix End */
                }

                if (*leastNackRcv == 1025)
                {
                    *leastNackRcv = nackSn;
                }
                else
                {
                    *leastNackRcv = GET_LEAST_NACK_SN(*leastNackRcv , nackSn, vtA, vtMS); 
                }

                LOG_RLC_MSG(LTE_RLC_NACK_RECEIVE_STATUS_3, LOGINFO , (RLC_TX|RLC_AM),
                        globalTTITickCount_g, pAmTxE->ueIndex , pAmTxE->lcId, vtA ,
                        vtMS, *leastNackRcv, 0, 0, __func__, "");
            }
            else 
            {
                LOG_RLC_MSG(LTE_RLC_NACK_RECEIVE_STATUS_1, LOGWARNING , (RLC_TX|RLC_AM),
                        globalTTITickCount_g, pAmTxE->ueIndex , pAmTxE->lcId, reTxIndex ,
                        vtA, nackSn, vtMS, 0, __func__, "");
            }
        }
        nackSn = 0;
        Sostart = 0;
        Soend = 0;
        e2 = 0;
    }
    lcQLoadArr.lcId = pAmTxE->lcId;
    /*****************************************************/    
    rlcFetchQueueLoad( pAmTxE, &lcQLoadArr);
    lcQLoadArr.scheduledDataBytes = 0;
   
    /* SPR 12610 Start */   
    if( amdRawSduQ_p->pQ->count > 0)
    {        
    amdRawSdu_p  = (RlcUeDataReq *)(amdRawSduQ_p->pQ->ring[amdRawSduQ_p->pQ->head].data);
        lcQLoadArr.headPktTS = amdRawSdu_p->pktArrTime;
    }
    else 
    {
        lcQLoadArr.headPktTS = 0;
    }
    /* SPR 12610 End */   
    lcQLoadArr.ctrlFlag = RLC_NORMAL_DATA;
    /**********************************************************/

    /* notify MAC layer of queue load */
    macUpdateBufferOccupancyReq( pAmTxE->ueIndex, lcCount, &lcQLoadArr, RLC_BO_TYPE_NEW,
             rlcUEContextList_g[pAmTxE->ueIndex]->cellIndex
                               );

    LOG_RLC_MSG(LTE_RLC_NACK_RECEIVE_STATUS, LOGINFO , (RLC_TX|RLC_AM),
            globalTTITickCount_g, pAmTxE->ueIndex , pAmTxE->lcId, vtA ,
            vtMS, 0, 0, 0, __func__, "");
    return ret ;
}

/****************************************************************************
 * Function Name  : rlcActionOnReceiveStatusReport 
 * Inputs         : pAmTxE - Tx Am Entity pointer,
 *                  amdRawPdu_p - Pointer to AmdRawPdu
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This function return the poll bit value for Rx-Tx case.
 ****************************************************************************/

UInt32 rlcActionOnReceiveStatusReport(TxAMEntity* pAmTxE, AmdRawPdu *amdRawPdu_p)
{
    UInt32 ret         = RLC_SUCCESS;
    UInt8 *base_data_p = RLC_NULL;
    UInt8 dc           = 0;
    UInt8 cpt          = 0;
    UInt32 Idx         = 0;
    UInt32 bitPos      = RLC_BYTE;
    RlcVr lastAckReceived = 0;
    RlcVr expectedAck     = 0;   
    RlcVr vtA  = pAmTxE->vtA;
    RlcVr vtMS = pAmTxE->vtMS;
    RlcVr vtS  = pAmTxE->vtS;
    RlcVr leastNackRcv = RLC_INVALID_SN;


    QSEGMENT segment  = {0};
    msgSegNext(amdRawPdu_p->pRawPdu.data_p, 0, &segment);
    base_data_p  = segment.base;
    if(base_data_p == RLC_NULL)
    {
        LOG_RLC_MSG(LTE_RLC_RECV_STATUS_PDU_ERR, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                pAmTxE->ueIndex, pAmTxE->lcId , RLC_FAILURE, 0, 0, 0, __func__, "");
        return RLC_FAILURE;
    }
    EMPTY_TX_AM_NACK_LIST(pAmTxE->nackList);


    pAmTxE->reTxQueueSize = 0;

    /* decode D/C bit and update bitPos */
    RLC_GET_DC_AND_UPDATE_BIT_POS_IDX(base_data_p, dc, Idx, bitPos);
    /* decode CPT  and update bitPos* */
    RLC_GET_CPT_AND_UPDATE_BIT_POS_IDX(base_data_p, cpt, Idx, bitPos);
    
    /* Check for First bytes is Control PDU and CPT should be zero */
    if( (0 != dc) || (0 != cpt) )
    {
#ifdef DEBUG_STATS
        pAmTxE->invalidCptInHdr++;
#endif
        LOG_RLC_MSG(LTE_RLC_NON_ZERO_DC_CPT_IN_CONTROL_PDU, LOGERROR, (RLC_TX|RLC_AM), 
                globalTTITickCount_g,  __LINE__,pAmTxE->ueIndex, pAmTxE->lcId, 
                RLC_ERR_NON_ZERO_DC_CPT_IN_CONTROL_PDU,0, 0,0, __func__, "");
        return RLC_ERR_NON_ZERO_DC_CPT_IN_CONTROL_PDU;
    }

    /*ACK SN*/
    RLC_GET_ACK_SN_AND_UPDATE_BIT_POS_IDX(base_data_p, lastAckReceived, Idx, bitPos);
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
    /*EVENT: EVENT_PARAM_PER_UE_RLC_ACK_UL, The total number of successful RLC PDU 
     * transmissions (ACKs) in the uplink direction*/
    gRlcUePerfStats_p[pAmTxE->ueIndex].lteUeRlcULPerfStats.totalULRLCAcks++;
#endif
   
    if ( ! (isAMGTE ( lastAckReceived, vtA, vtA ) && isAMLTE ( lastAckReceived, vtS, vtA )) ) 
    {
        LOG_RLC_MSG(LTE_RLC_OUT_OF_RANGE_ACK_RECVD_1, LOGERROR, (RLC_TX|RLC_AM),
                globalTTITickCount_g, __LINE__,pAmTxE->ueIndex, pAmTxE->lcId,
                lastAckReceived,RLC_ERR_INVALID_ACK_RECVD, (UDouble32)vtA, (UDouble32)vtS,
                __func__, "");
#ifdef DEBUG_STATS
        pAmTxE->invalidAckRecvd++;
#endif

        return RLC_ERR_INVALID_ACK_RECVD;
    }
    
    if ( !(isAMLTE ( vtA, lastAckReceived, vtA) && isAMLTE ( lastAckReceived, vtMS, vtA) ) )   
    {
        /*check last Ack  SN falls outside of the transmitting window */
        LOG_RLC_MSG(LTE_RLC_ACK_OUT_OF_WINDOW_1, LOGERROR, (RLC_TX|RLC_AM),
                globalTTITickCount_g, __LINE__,pAmTxE->ueIndex, pAmTxE->lcId,
                lastAckReceived,RLC_ERR_OUT_OF_WINDOW_ACK_RECVD, (UDouble32)vtA, (UDouble32)vtMS,
                __func__, "");
#ifdef DEBUG_STATS
        pAmTxE->oowAckRecvd++;
#endif

        return RLC_ERR_OUT_OF_WINDOW_ACK_RECVD;
    }
    
    /* SPR 20443 Changes Start */
    if((isAMGT( lastAckReceived, pAmTxE->pollSN, vtA))&& 
    /* SPR 20443 Changes End */
            pAmTxE->istRetransmitTimerRunning)
    {
        LOG_RLC_MSG(LTE_RLC_ACK_OUT_OF_WINDOW_3, LOGINFO , (RLC_TX|RLC_AM),
                globalTTITickCount_g, __LINE__,pAmTxE->ueIndex, pAmTxE->lcId,
                lastAckReceived ,  pAmTxE->pollSN , 0,0, __func__, "");
        stopPRTimer(pAmTxE); 
        pAmTxE->istRetransmitTimerRunning = RLC_FALSE;
    }
    /*Check whether NACK present or not*/
    if(2 < msgSize(amdRawPdu_p->pRawPdu.data_p, RLC_NULL))
    {
        rlcActionOnNackReceiveStatusReport(pAmTxE, base_data_p, bitPos,&leastNackRcv);
    }
    else 
    {
    }
    if (leastNackRcv == RLC_INVALID_SN )
    {
        expectedAck = lastAckReceived;
    }
    else
    {
        expectedAck = leastNackRcv;
    }
    /* SPR 8402 Fix Start */
#ifdef DEBUG_STATS
    pAmTxE->rlcRxStatus[pAmTxE->statusCount].statusRecvd.ack_Sn = lastAckReceived;
    pAmTxE->rlcRxStatus[pAmTxE->statusCount].statusTti = globalTTITickCount_g;
    pAmTxE->rlcRxStatus[pAmTxE->statusCount].statusRecvd.size = msgSize(amdRawPdu_p->pRawPdu.data_p, RLC_NULL);
    pAmTxE->statusCount = (pAmTxE->statusCount +1) % MAX_RLC_RX_STATUS;
#endif
    if (pAmTxE->vtA != expectedAck )
    {
        ret = rlcSendSuccessfulDeliveryInd(pAmTxE,expectedAck);
    }
    /* SPR 8402 Fix End */
    if ( ret !=  RLC_SUCCESS)
    {
        LOG_RLC_MSG(LTE_RLC_SEND_DEL_IND_FAILED, LOGERROR, (RLC_TX|RLC_AM), 
                globalTTITickCount_g,  __LINE__,pAmTxE->ueIndex, 
                pAmTxE->lcId,  pAmTxE->vtA ,pAmTxE->vtMS, 0,0, __func__, "");
    }

    LOG_RLC_MSG(LTE_RLC_SEND_DEL_IND_INFO, LOGINFO, (RLC_TX|RLC_AM), 
            globalTTITickCount_g,  __LINE__,pAmTxE->ueIndex, 
            pAmTxE->lcId,  pAmTxE->vtA ,pAmTxE->vtMS, 0,0, __func__, "");
    return ret ;
}

/*******************************************************************************
 * Function Name  : rlcRemoveSduTidInfoForDelInd
 * Inputs         : sduTidInfoQ_p - Pointer to SduTidInfoQ,
 *                  amReTxPdu_p - reTx pdu pointer,
 *		            rlcStatusBufferCount_p - counter for no. of TiDs,
 *                  rlcStatusBuffer_p - Pointer to RlcStatusBuffer
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_ERROR
 * Description    : This function remove TiD info from ReTx pdu to send Del Ind.
 *******************************************************************************/
UInt32 rlcRemoveSduTidInfoForDelInd( SduTidInfoQ *sduTidInfoQ_p,
                                            AmReTransmission *amReTxPdu_p,
                                            UInt16* rlcStatusBufferCount_p,
                                            RlcStatusBuffer * rlcStatusBuffer_p
                                          ) 
{
    UInt32 ret                    =   RLC_SUCCESS;
    UInt16 deliveryStatus         =   RLC_TRUE;
    SduTidInfo *temp_p            =   RLC_NULL;
    UInt16 dfeCount               = amReTxPdu_p->dfeInfo.dfeCount;
    do
    {
         if ( dfeCount == 1 && !((amReTxPdu_p->dfeInfo.fi) & 0x01 ) )
         {
             DEQUEUE_AMD_SDU_TID_Q(temp_p, sduTidInfoQ_p);
             if ( !temp_p)
                 return RLC_SUCCESS;

             rlcStatusBuffer_p[*rlcStatusBufferCount_p].transId = temp_p->tId;
             rlcStatusBuffer_p[*rlcStatusBufferCount_p].deliveryStatus = deliveryStatus;
             (*rlcStatusBufferCount_p)++;

             freeMemPool(temp_p);
             temp_p = PNULL;
         }
         else if ( dfeCount > 1 )
         {
             DEQUEUE_AMD_SDU_TID_Q(temp_p, sduTidInfoQ_p);
             if ( !temp_p)
                 return RLC_SUCCESS;

             rlcStatusBuffer_p[*rlcStatusBufferCount_p].transId = temp_p->tId;
             rlcStatusBuffer_p[*rlcStatusBufferCount_p].deliveryStatus = deliveryStatus;
             (*rlcStatusBufferCount_p)++;

             freeMemPool(temp_p);
             temp_p = PNULL;
         }
    }while ( --dfeCount );

    return ret;
}

/******************************************************************************
 * Function Name  : rlcFreeReTxBufferDuringDelInd
 * Inputs         : pAmTxE - Tx Am Entity pointer,
 *                  leastNackRcv - least Nack received in Status PDU,
 *                  rlcStatusBufferCount_p - counter for no. of TiDs,
 *                  rlcStatusBuffer_p - Pointer to RlcStatusBuffer
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_ERROR
 * Description    : This function free ReTx buffer for which Del Ind can be sent.
 *******************************************************************************/
UInt32 rlcFreeReTxBufferDuringDelInd( TxAMEntity* pAmTxE, 
                				      RlcVr leastNackRcv,
                                      UInt16* rlcStatusBufferCount_p,
                                      RlcStatusBuffer * rlcStatusBuffer_p
                				    )
{
    UInt32 ret                      =   RLC_FAILURE;
    RlcVr vtA                      =   pAmTxE->vtA;
    RlcVr vtS                       =   pAmTxE->vtS;
    AmReTransmission *amReTxPdu_p   =   RLC_NULL;
    UInt16 rCount                   =   vtA;
    UInt16 ringSN                   = 0;
    /* Start 128 UE: 25% NACK Changes */
    /*
    ** Check whether a duplicate Status Ind is received.
    */
    if (!isAMLT( rCount, leastNackRcv, vtA))
    {
       LOG_RLC_MSG(LTE_RLC_DUPLICATE_STATUS_IND, LOGINFO, (RLC_TX|RLC_AM), 
               globalTTITickCount_g,  __LINE__,pAmTxE->ueIndex, pAmTxE->lcId,
               vtA,leastNackRcv, 0, 0, __func__, "");
       return ret;
    }
    /* End 128 UE: 25% NACK Changes */
    for ( ; isAMLT( rCount, leastNackRcv, vtA); rCount = (rCount + 1 ) & 1023)  
    {
        if(   isAMLTE (vtA, rCount, vtA) && isAMLT ( rCount, vtS, vtA) ) 
        {
            ringSN = rCount & 511;
            amReTxPdu_p  =  pAmTxE->reTxQueue[ ringSN];

            if ( amReTxPdu_p )
            {
                pAmTxE->reTxQueue[ringSN]  =  RLC_NULL;
                ret = rlcRemoveSduTidInfoForDelInd( &pAmTxE->tidQueue, 
                        amReTxPdu_p, 
                        rlcStatusBufferCount_p,                         
                        rlcStatusBuffer_p);
                if(ret != RLC_SUCCESS)
                {
                    LOG_RLC_MSG(LTE_RLC_FREE_SDU_TID_FAILED, LOGERROR, (RLC_TX|RLC_AM), 
                            globalTTITickCount_g,  __LINE__,pAmTxE->ueIndex, pAmTxE->lcId, 
                            rCount,RLC_ERR_FREE_SDU_TID_FAILED, 0,0, __func__, "");
                    return RLC_ERR_FREE_SDU_TID_FAILED;
                }

                if(rCount == pAmTxE->txPduIn.sn)
                {
                    if(pAmTxE->txPduIn.pRawPdu.data_p)
                    {
                        msgFree(pAmTxE->txPduIn.pRawPdu.data_p);
                        pAmTxE->txPduIn.pRawPdu.data_p =   RLC_NULL;
                        pAmTxE->txPduIn.pRawPdu.size   =   0;
                        pAmTxE->txPduIn.sn             =   0;
                        pAmTxE->txPduIn.Sostart        =   0;
                        pAmTxE->txPduIn.Soend          =   0;
                    }
                }
                FREE_AM_RETX_PDU(amReTxPdu_p);
                ret = RLC_SUCCESS;
            }
            else
            {
                LOG_RLC_MSG(LTE_RLC_NULL_RETX_PDU_IN_DEL_IND, LOGWARNING, (RLC_TX|RLC_AM), 
                        globalTTITickCount_g,  __LINE__,pAmTxE->ueIndex, pAmTxE->lcId, 
                        rCount,0, 0,0, __func__, "");
            }
        }

        else
        {
            LOG_RLC_MSG(LTE_RLC_OUT_OF_RANGE_SN_IN_DEL_IND_1, LOGWARNING, (RLC_TX|RLC_AM), 
                    globalTTITickCount_g,pAmTxE->ueIndex, pAmTxE->lcId, rCount,vtA, vtS, 
                    0,0, __func__, "");
        }
        LOG_RLC_MSG(LTE_RLC_OUT_OF_RANGE_SN_IN_DEL_IND_2, LOGINFO, (RLC_TX|RLC_AM), 
                globalTTITickCount_g,pAmTxE->ueIndex, pAmTxE->lcId, rCount,vtA, vtS, 
                0,0, __func__, "");
    }
    return ret;
}

/****************************************************************************
 * Function Name  : rlcSendSuccessfulDeliveryInd 
 * Inputs         : pAmTxE - Tx Am Entity pointer,
 *                : leastNackRcv - least Nack received in Status PDU  
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This function send Successful delivery to upper layer.
 ****************************************************************************/
UInt32 rlcSendSuccessfulDeliveryInd( TxAMEntity* pAmTxE, 
				     RlcVr leastNackRcv 
				   )
{
    UInt32 ret                     =   RLC_SUCCESS;
    UInt16 rlcStatusBufferCount    =   0;
    RlcStatusBuffer *rlcStatusBuffer_p = PNULL;
    /* SPR 16843 start */
#if (defined KPI_STATS) && !(defined UE_SIM_TESTING)
/*spr 19296 fix start*/
    UInt8 qci = 0;
    InternalCellIndex cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(pAmTxE->ueIndex);
    UInt32 activeIndexForMacStats = (gMacStats.
            pStats->lteCommonKPIStats[cellIndex].activeIndexForMacStats);
    LteMacKPIStats * pLteMacKPIStats = &(gMacStats.
            pStats->lteCommonKPIStats[cellIndex].lteMacKPIStats[activeIndexForMacStats]);
/*spr 19296 fix end*/
#endif
    /* SPR 16843 end */

    if (!(QUEUE_COUNT_AM_SDU_TID_Q(pAmTxE->tidQueue)))
    {
        LOG_RLC_MSG(LTE_RLC_NO_TID_PRESENT_FOR_DEL_IND, LOGERROR, (RLC_TX|RLC_AM), 
                globalTTITickCount_g,  __LINE__,pAmTxE->ueIndex, pAmTxE->lcId, 
                RLC_ERR_NO_TID_PRESENT_FOR_DEL_IND,0, 0,0, __func__, "");

        return RLC_ERR_NO_TID_PRESENT_FOR_DEL_IND;
    }

    /** This buffer should be freed at PDCP once read **/
    rlcStatusBuffer_p = getMemFromPool( (sizeof(RlcStatusBuffer) * RLC_MAX_STATUS_BUFFER ) ,RLC_NULL);
    if( RLC_NULL == rlcStatusBuffer_p)
    {
        ltePanic("%s => Memory Allocation Failure",__FUNCTION__);
        return RLC_FAILURE;
    }

    ret = rlcFreeReTxBufferDuringDelInd(pAmTxE, leastNackRcv, &rlcStatusBufferCount, rlcStatusBuffer_p);
    if (ret != RLC_SUCCESS)
    {
        LOG_RLC_MSG(LTE_RLC_FREE_RETX_BUFFER_FAILED, LOGERROR, (RLC_TX|RLC_AM), 
                globalTTITickCount_g,  __LINE__,pAmTxE->ueIndex, pAmTxE->lcId, 
                RLC_ERR_FREE_RETX_BUFFER_FAILED,0, 0,0, __func__, "");

        freeMemPool(rlcStatusBuffer_p);
        return RLC_ERR_FREE_RETX_BUFFER_FAILED;
    }

    if(rlcStatusBufferCount)
    {
        /* SPR 16843 start */
#if (defined KPI_STATS) && !(defined UE_SIM_TESTING)
/*spr 19296 fix start*/
        qci = pAmTxE->qci;
        if (0 < qci && MAX_QCI >= qci )
        {
            pLteMacKPIStats->totalSduAckRcvdDlperQci[--qci] +=
                rlcStatusBufferCount;
        }
/*spr 19296 fix end*/
#endif
        /* SPR 16843 end */
        LOG_RLC_MSG(LTE_RLC_SEND_DEL_IND, LOGDEBUG, (RLC_TX|RLC_AM), globalTTITickCount_g,
                __LINE__,pAmTxE->ueIndex, pAmTxE->lcId,rlcStatusBufferCount,
                0, 0,0, __func__, "");

        CLOCK_START_RLC_API(RLC_DLVRY_STATUS_IND);
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
		/*Dual Connectivity SeNB changes +*/
		if((SN_TERMINATED_SCG_SPLIT_BEARER ==  pAmTxE->dcBearerType) && (pAmTxE->lcId >= MIN_DRB_LC_ID))
		    sendRlcDeliveryStatusToX2U(
		            pAmTxE->ueIndex,
		            pAmTxE->lcId,
		            rlcStatusBufferCount,
		            rlcStatusBuffer_p);
		else
		    /*Dual Connectivity SeNB changes -*/
#endif
        rlcDeliveryStatusInd(pAmTxE->ueIndex, pAmTxE->lcId, rlcStatusBufferCount,
                rlcStatusBuffer_p);
        CLOCK_END_RLC_PDCP_API(RLC_DLVRY_STATUS_IND);
        gRlcStats.pRlcStats->lteRlcUeStat[pAmTxE->ueIndex].
            lcStats[pAmTxE->lcId].numDlvryInd += rlcStatusBufferCount;
    }
    else
    {
        /// Case when status buffer count is 0
        freeMemPool(rlcStatusBuffer_p);
    }

    pAmTxE->vtA    =   leastNackRcv & 1023;            
    pAmTxE->vtMS   =   (pAmTxE->vtA + AM_WINDOW_SIZE) & 1023;

    LOG_RLC_MSG(LTE_RLC_STATE_VARS_ON_DEL_IND, LOGDEBUG, (RLC_TX|RLC_AM), globalTTITickCount_g, 
            pAmTxE->ueIndex, pAmTxE->lcId,pAmTxE->vtA, pAmTxE->vtS,pAmTxE->vtMS, 
            0,0, __func__, "");

    /* If Ack SN is greater than InComplete ReTransmitted SN */
    UInt32 isWithInWindowFlag =  isWithInTxWindow ( pAmTxE->vtA, pAmTxE->txPduIn.sn , pAmTxE->vtMS );

    if ( !isWithInWindowFlag )    
    {
        if(pAmTxE->txPduIn.pRawPdu.size)
        {
            msgFree(pAmTxE->txPduIn.pRawPdu.data_p);
            pAmTxE->txPduIn.pRawPdu.size     =   0;
            pAmTxE->txPduIn.pRawPdu.data_p   =   PNULL;
        }
        pAmTxE->txPduIn.sn       =   0;
        pAmTxE->txPduIn.Sostart  =   0;
        pAmTxE->txPduIn.Soend    =   0;
    }
    return ret;
}
/****************************************************************************
 * Function Name  : rlcEncodeStatusPdu 
 * Inputs         : amRxE_p - Rx Am Entity pointer,
 *                  pduFormed_p - Pointer of RlcAmBuffer,
 *                  requestedDataSize - Request size provided by MAC
 * Outputs        : None 
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This function handles the Status PDU encoding.
 ****************************************************************************/

UInt32 rlcEncodeStatusPdu(
     RxAMEntity* amRxE_p ,
     RlcPduBuffer* pduFormed_p,
     UInt32 requestedDataSize,
     UInt32 *prlcStatusPduSendSts)
{
    /* Local variables for computations */
    AmRxNackListNode *amRxNackListNode_p = RLC_NULL;
    DEQUEUE_AMD_RX_NACK_LIST_Q(amRxNackListNode_p, amRxE_p->amRxNackList);
    if(RLC_NULL == amRxNackListNode_p)
    {
#ifdef DEBUG_STATS
        amRxE_p->deQFailNackList++;
#endif
        LOG_RLC_MSG(LTE_RLC_EMPTY_NACK_Q_IN_ENCODING, LOGERROR, (RLC_RX|RLC_AM), 
	    globalTTITickCount_g,__LINE__,amRxE_p->ueIndex, amRxE_p->lcId, 
            RLC_ERR_EMPTY_NACK_Q_IN_ENCODING,0, 0,0, __func__, "");
	return RLC_ERR_EMPTY_NACK_Q_IN_ENCODING;
    }
    NackListArr *nackListArr_p = &(amRxNackListNode_p->nackListArr);
    /* SPR 4010 Fix Start */
    // NackListArr nackList = {0} ;
    UInt32 ack_Sn = 0;
    /* SPR 4010 Fix End */
    UInt32 eStatusPduLenInBits = RLC_TWOBYTE;
    UInt32 requestedDataSizeInBits = requestedDataSize << 3;
    
    if(eStatusPduLenInBits == requestedDataSizeInBits)
    {
        pduFormed_p->size = requestedDataSize;
        /* SPR 4010 Fix Start */
        //nackList.count = 0;
        if(nackListArr_p->count)
        {
            ack_Sn = nackListArr_p->nackListArray[0].nack_sn;
        }
        else 
        {
            ack_Sn = nackListArr_p->ack_Sn;
        }
        /* SPR 4010 Fix End */
    }
    /* SPR 3973 Fix Start */
    else if(nackListArr_p->size > requestedDataSize)
    /* SPR 3973 Fix End */
    {
        pduFormed_p->size = requestedDataSize;
    }
    else
    {
        pduFormed_p->size = nackListArr_p->size;
        /* SPR 4010 Fix Start */
        ack_Sn = nackListArr_p->ack_Sn;
        /* SPR 4010 Fix End */
    }
   
    LOG_RLC_MSG(LTE_RLC_STATUS_PDU_SIZE_CALCULATION, LOGDEBUG, (RLC_RX|RLC_AM), 
	globalTTITickCount_g, __LINE__,amRxE_p->ueIndex, amRxE_p->lcId,
	pduFormed_p->size,requestedDataSize, 0, 0, __func__, "");
    /* Creating Zero Copy Buffer for Status PDU Encode */
    pduFormed_p->data_p = (UInt8*)msgAlloc(zeroPool_g,0,0,0);
    if (!(pduFormed_p->data_p))
    {
        LOG_RLC_MSG(LTE_RLC_MSG_ALLOC_FAIL, LOGERROR, (L2_SYS_FAIL),
            globalTTITickCount_g, __LINE__,amRxE_p->ueIndex,amRxE_p->lcId,0,0,
            0,0,__func__,"Making Status PDU");
        ALARM_MSG (RLC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("%s => Buffer Allocation Failure",__FUNCTION__);
        /* SPR 4190 Fix start */
        return RLC_FAILURE;
        /* SPR 4190 Fix End */
    }
    UInt8 *status_pdu_p= msgReserve(pduFormed_p->data_p, 0,pduFormed_p->size);
    if (!status_pdu_p)
    {
        msgFree(pduFormed_p->data_p);
        /* SPR 20985 fix start */
        pduFormed_p->data_p = RLC_NULL;
        /* SPR 20985 fix end */
        ALARM_MSG (RLC_MODULE_ID,MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("%s => Buffer Reserve Failure",__FUNCTION__);
        return RLC_FAILURE; 
    }
    memSet(status_pdu_p, 0, pduFormed_p->size);
    /* Call rlcBuildStatusPdu function to build the status pdu */
    /* SPR 4010 Fix Start */
    pduFormed_p->size = rlcBuildStatusPdu(ack_Sn,&status_pdu_p,nackListArr_p, pduFormed_p->size);

    /* SPR 4010 Fix End */
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
    /*EVENT: EVENT_PARAM_PER_UE_RLC_ACK_DL, The total number of successful RLC PDU 
     * transmissions (ACKs) in the downlink direction*/
    gRlcUePerfStats_p[amRxE_p->ueIndex].lteUeRlcDLPerfStats.totalDLRLCAcks++;
#endif

    if(pduFormed_p->size >= nackListArr_p->size )
    {
        *prlcStatusPduSendSts = RLC_AM_NACK_SN_ALL_IN;

        freeMemPool(amRxNackListNode_p);
        amRxNackListNode_p = RLC_NULL;
    }
    else
    {
        if(!(ENQUEUE_AMD_RX_NACK_LIST_Q(amRxE_p->amRxNackList,amRxNackListNode_p)))
        {
            freeMemPool(amRxNackListNode_p);
            amRxNackListNode_p = RLC_NULL;
        }
    }
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : canFillNackSn
 * Inputs         : eIndx,
 *                  statusPduSize,
 *                  idx,
 *                  bitPos,
 *                  nackList_p - Pointer to NackListArr
 * Outputs        : None
 * Returns        : RLC_TRUE/RLC_FALSE
 * Description    : This function checks if next NACK_Sn can be filled.
 ****************************************************************************/
static UInt32 canFillNackSn(UInt32 eIndx, UInt32 statusPduSize, UInt32 idx,
                                   UInt32 bitPos, NackListArr *nackList_p)
{
    /* SPR 5493 Fix start */
    SInt32 availableBits = ( (statusPduSize - idx) << 3 ) - (8 - bitPos);
    SInt32 numBitsReq = 0;
    /* SPR 5493 Fix end */

    if (nackList_p->nackListArray[eIndx].e1)
    {
        if (nackList_p->nackListArray[eIndx].noOfSeg)
        {
            /* SPR 7517 changes start */
            numBitsReq = (RLC_AM_NACK_SN_PAIR_SIZE + RLC_AM_NACK_SO_PAIR_SIZE)
                 * nackList_p->nackListArray[eIndx].noOfSeg;
            /* SPR 7517 changes end */
        }
        else
        {
            numBitsReq = RLC_AM_NACK_SN_PAIR_SIZE;
        }
        if (availableBits >= numBitsReq)
        {
            return RLC_TRUE;
        }
        else
        {
            return RLC_FALSE;
        }
    }
    else
    {
        return RLC_FALSE;
    }
}

/****************************************************************************
 * Function Name  : canFillSegNack
 * Inputs         : statusPduSize,
 *                  idx,
 *                  bitPos
 * Outputs        : None
 * Returns        : RLC_TRUE/RLC_FALSE
 * Description    : This function checks if next NACK_sn with soStart and SoEnd
 *                  can be filled.
 ****************************************************************************/
static UInt32 canFillSegNack(UInt32 statusPduSize, UInt32 idx, UInt32 bitPos)
{
    /* SPR 5493 Fix start */
    SInt32 availableBits = ( (statusPduSize - idx) << 3) - (8 - bitPos);
    /* SPR 5493 Fix end */

    if (availableBits >= RLC_AM_NACK_SN_PAIR_SIZE + RLC_AM_NACK_SO_PAIR_SIZE)
    {
        return RLC_TRUE;
    }
    else
    {
        return RLC_FALSE;
    }
}

/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : rlcFillNackInStatusPdu
 * Inputs         : data_p - Pointer to status pdu,
 *                  nackList_p - Pointer to nack array,
 *                  eIndx,
 *                  statusPduSize,
 *                  index - pointer to index in status pdu,
 *                  bitPosition - pointer to bit position,
 *                  filledAckSn - Filled Ack sn
 * Outputs        : None
 * Returns        : eBit
 * Description    : This function fills nack in  the status pdu.
 ****************************************************************************/
static UInt32 rlcFillNackInStatusPdu (
    UInt8 **data_p,
    NackListArr *nackList_p,
    UInt32  eIndx,
    UInt32  statusPduSize,
    UInt32  *index,
    UInt32  *bitPosition,
    UInt32  *filledAckSn )
{
    UInt32 eBit = 1;
    UInt32 idx = *index;
    UInt32 bitPos = *bitPosition;
    UInt8 *status_pdu_p = *data_p;
    UInt32 e1_Index = 0, e1_bitPos = 0;
    
    /* Fill Nack SN */
    /* Encode NACK SN in status pdu */
    RLC_FILL_NACK_SN_AND_UPDATE_BIT_POS_IDX(status_pdu_p,\
        nackList_p->nackListArray[eIndx].nack_sn,idx,bitPos);

    e1_Index = idx;
    e1_bitPos = bitPos;
    /* pre fill e1 to set idx and bitPos correctly */
    RLC_FILL_E1_AND_UPDATE_BIT_POS_IDX (status_pdu_p, 0,idx,bitPos);
    /* fill e2 to 0*/
    RLC_FILL_E2_AND_UPDATE_BIT_POS_IDX(status_pdu_p, 0, idx,bitPos);

    if (canFillNackSn(eIndx + 1, statusPduSize, idx, bitPos, nackList_p))
    {
        RLC_FILL_E1_AND_UPDATE_BIT_POS_IDX (status_pdu_p, 1,e1_Index, e1_bitPos);
        eBit = 1;
    }
    else
    {
        RLC_FILL_E1_AND_UPDATE_BIT_POS_IDX (status_pdu_p, 0,e1_Index, e1_bitPos);
        eBit = 0;
        *filledAckSn = nackList_p->nackListArray[eIndx + 1].nack_sn;
    }

    *index = idx;
    *bitPosition = bitPos;
    return eBit;
}

/****************************************************************************
 * Function Name  : rlcFillSegmentNackInStatusPdu
 * Inputs         : data_p - Pointer to status pdu,
 *                  nackList_p - Pointer to nack list,
 *                  eIndx,
 *                  statusPduSize,
 *                  index - pointer to index in status pdu,
 *                  bitPosition - pointer to bit position,
 *                  filledAckSn - Filled Acknowledge Sequence Number
 * Outputs        : None
 * Returns        : eBit
 * Description    : This function fills segment nack in  the status pdu.
 ****************************************************************************/
static UInt32 rlcFillSegmentNackInStatusPdu (
    UInt8 **data_p,
    NackListArr *nackList_p,
    UInt32  eIndx,
    UInt32  statusPduSize,
    UInt32  *index,
    UInt32  *bitPosition,
    UInt32  *filledAckSn )
{
    UInt32 eBit = 1;
    UInt32 idx = *index;
    UInt32 bitPos = *bitPosition;
    UInt8 *status_pdu_p = *data_p;
    UInt32 segNum = 0;

    /* SPR 7517 changes start */
    UInt32 e1BitPos = 0;
    UInt32 e1Idx = 0;
    /* SPR 7517 changes end */

    /* SPR 7517 changes start */
    for (segNum = 0; (eBit ==1) &&
            (segNum < nackList_p->nackListArray[eIndx].noOfSeg); segNum++)
        /* SPR 7517 changes end */
    {
        /* Fill Nack SN */
        /* Encode NACK SN in status pdu */
        RLC_FILL_NACK_SN_AND_UPDATE_BIT_POS_IDX(status_pdu_p,\
                nackList_p->nackListArray[eIndx].nack_sn,idx,bitPos);

        /* SPR 7517 changes start */
        e1BitPos = bitPos;
        e1Idx = idx;
        /* temp fill e1 to set idx and bitPos correctly */
        RLC_FILL_E1_AND_UPDATE_BIT_POS_IDX (status_pdu_p, 0,idx,bitPos);
        RLC_FILL_E2_AND_UPDATE_BIT_POS_IDX(status_pdu_p,1,idx,bitPos);
        /* Encode SO_START in status pdu */
        RLC_FILL_SO_START_AND_UPDATE_BIT_POS_IDX(status_pdu_p,\
                nackList_p->nackListArray[eIndx].rlcSeg[segNum].soStart,\
                idx,bitPos);

        /* Encode SO_END in status pdu */
        RLC_FILL_SO_END_AND_UPDATE_BIT_POS_IDX(status_pdu_p,
                nackList_p->nackListArray[eIndx].rlcSeg[segNum].soEnd,\
                idx,bitPos);
        if ( segNum + 1 < nackList_p->nackListArray[eIndx].noOfSeg)
        {
            if (canFillSegNack(statusPduSize, idx, bitPos))
            {
                RLC_FILL_E1_AND_UPDATE_BIT_POS_IDX (status_pdu_p, 1,e1Idx,e1BitPos);
                eBit = 1;
            }
            else
            {
                RLC_FILL_E1_AND_UPDATE_BIT_POS_IDX (status_pdu_p, 0,e1Idx,e1BitPos);
                eBit = 0;
                *filledAckSn = nackList_p->nackListArray[eIndx].nack_sn;
            }
        }
        else if (canFillNackSn(eIndx + 1, statusPduSize, idx, bitPos, nackList_p))
        {
            RLC_FILL_E1_AND_UPDATE_BIT_POS_IDX (status_pdu_p, 1,e1Idx,e1BitPos);
            eBit = 1;
        }
        else
        {
            RLC_FILL_E1_AND_UPDATE_BIT_POS_IDX (status_pdu_p, 0,e1Idx,e1BitPos);
            eBit = 0;
            *filledAckSn = nackList_p->nackListArray[eIndx + 1].nack_sn;
        }

        /* SPR 7517 changes end */
    }

    *index = idx;
    *bitPosition = bitPos;
    return eBit;
}
/* Cyclomatic Complexity changes - ends here */

/****************************************************************************
 * Function Name  : rlcBuildStatusPdu
 * Inputs         : ackSn - ACK SN to be sent,
 *                  pStatus_pdu - Pointer to status pdu,
 *                  nackList_p - Pointer to nack array,
 *                  statusPduSize - Size of Status PDU
 * Outputs        : None
 * Returns        : None
 * Description    : This function builds the status pdu.
 ****************************************************************************/

UInt32 rlcBuildStatusPdu(
    UInt32 ackSn,
    UInt8 **pStatus_pdu,
    NackListArr *nackList_p,
    UInt32 statusPduSize)
{
    UInt32 bitPos = RLC_BYTE;
    UInt32 idx = 0;
    UInt32 eIndx = 0;
    UInt8 *status_pdu_p = *pStatus_pdu;
    UInt32 ack_snIndex = 0, ack_bitPos = 0;
    UInt32 e1_Index = 0, e1_bitPos=0;
    UInt32 filledAckSn = ackSn;
    UInt32 eBit = 1;
    UInt32 totalLen = 0;

    /* Encode D/C field */
    RLC_FILL_DC_AND_UPDATE_BIT_POS_IDX
        (status_pdu_p,STATUS_PDU_DC_BIT,idx,bitPos);

    /* Encode CPT field */
    RLC_FILL_CPT_AND_UPDATE_BIT_POS_IDX
        (status_pdu_p,STATUS_PDU_CPT_BIT,idx,bitPos);

    /** storing index and bitPos for ack_sn **/
    ack_snIndex = idx;
    ack_bitPos = bitPos;
    /* Encode ACK_SN field */
    RLC_FILL_ACK_SN_AND_UPDATE_BIT_POS_IDX(status_pdu_p,ackSn,idx,bitPos);
    /* pre fill e1 to set idx and bitPos correctly */
    e1_Index = idx;
    e1_bitPos = bitPos;
    RLC_FILL_E1_AND_UPDATE_BIT_POS_IDX (status_pdu_p, 0,idx,bitPos);

    if (RLC_TRUE == canFillNackSn(eIndx, statusPduSize, idx, bitPos, nackList_p))
    {
        RLC_FILL_E1_AND_UPDATE_BIT_POS_IDX (status_pdu_p, 1, e1_Index, e1_bitPos);
        eBit = 1;
    }
    else
    {
        RLC_FILL_E1_AND_UPDATE_BIT_POS_IDX (status_pdu_p, 0, e1_Index, e1_bitPos);
        eBit = 0;
        /* SPR 7607 changes start */
        if (nackList_p->nackListArray[eIndx].e1)
        {
            filledAckSn = nackList_p->nackListArray[eIndx].nack_sn;            
        }
        /* SPR 7607 changes end */
    }

    while ((eBit == 1) && (eIndx < nackList_p->count))
    {
        if (nackList_p->nackListArray[eIndx].noOfSeg)
        {
            /* Cyclomatic Complexity changes - starts here */
            eBit = rlcFillSegmentNackInStatusPdu(&status_pdu_p,nackList_p,eIndx,
                      statusPduSize,&idx,&bitPos,&filledAckSn);
            /* Cyclomatic Complexity changes - ends here */
        }
        else
        {
            /* Cyclomatic Complexity changes - starts here */
            eBit = rlcFillNackInStatusPdu(&status_pdu_p,nackList_p,eIndx,
                      statusPduSize,&idx,&bitPos,&filledAckSn);
            /* Cyclomatic Complexity changes - ends here */
        }
        eIndx++;
    }
    if (eIndx < nackList_p->count)
    {
        /* reset ack sn */
        RLC_FILL_ACK_SN_AND_UPDATE_BIT_POS_IDX(status_pdu_p,filledAckSn,ack_snIndex,ack_bitPos);
    }
    RLC_CHECK_AND_ADDPEND_PADDING(status_pdu_p,idx,bitPos);
    totalLen = idx+1;
    return totalLen;
}
