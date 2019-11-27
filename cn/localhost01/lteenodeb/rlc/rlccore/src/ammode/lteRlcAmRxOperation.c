/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcAmRxOperation.c,v 1.1.6.2.2.3 2010/10/25 07:20:05 gur21010 Exp $
 *
 ****************************************************************************
 *
 *  File Description : this file describes the AM Entity methods.  
 *
 *
 *
 * 1. rlcReceiveAmdPduFromLowerLayer
 * 2. rlcHandleAmdPduFromLowerLayer
 * 3. rlcActionAmdPduInReceptionBuffer
 * 4. rlcActionAmTReorderingExpire 
 * 5. rlcDeliverAmSduToUpperLayer
 * 6. rlcReassembleAmdSdu
 * 7. rlcTriggerStatusReport
 * 8. rlcActionOnExpireStatusProhibitTimer
 * 9. rlcGetRingStatusForAM 
 *
 *
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcAmRxOperation.c,v $
 *
 *
 *
 ****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcPdcpInterface.h"
#include "lteRlcAmRxOperation.h"
#include "lteRlcAmdDecode.h"
#include "logging.h"
//#include "socket_utils.h"
#include "alarm.h"
#include "lteRlcStat.h"
#include "lteRlcUeContext.h"
#include "lteRlcEntityDef.h"
#include "lteRlcTimer.h"
#include "lteRlcErrorCode.h"

/****************************************************************************
 * Varibale Definition
 ****************************************************************************/
STATIC inline UInt32 rlcHandleAmdPduFromLowerLayer
(
    AMEntity * amEntity,
    AmdRawPdu *amdRawPdu_p,
    RlcSn* seqNum_p,
    UInt8 *triggerStatus_p
        /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 15909 fix start */
    ,tickType_t packetTime
    /* SPR 15909 fix end */
#endif
        /*32.425 Events code changes end*/
    );

/* Cyclomatic Complexity changes - starts here */
STATIC inline UInt32 rlcFillAmdPduSegmentInfo(
    RxAMEntity *amRxE_p,
    AmdPdu *amdPdu_p,
    NackList *nackList_p,
    UInt32 *eStatusPduLenInBits,
    UInt16 rCount);
/* Cyclomatic Complexity changes - ends here */

/****************************************************************************
 * External Variable Includes
 ****************************************************************************/
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */
#ifdef ENDC_ENABLED
extern void pdcpDataInd( UInt16 ueIndex, UInt16 lcId, void* data_p, UInt16 size);
#endif


/****************************************************************************
 * Function Name  : rlcReceiveAmdPduFromLowerLayer
 * Inputs         : amEntity - pointer to AMEntity,
 *                  amdRawPdu_p - pointer to AmdRawPdu
 * Outputs        : None
 * Returns        : RLC_SUCCESS if case success else RLC_FAILURE
 * Description    : This method is responsible to take action when a pdu 
 *                  recieved from lower layer.
 ****************************************************************************/

inline UInt32 rlcReceiveAmdPduFromLowerLayer (
        AMEntity * amEntity,
        AmdRawPdu *amdRawPdu_p
        /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        /* SPR 15909 fix start */
        ,tickType_t packetTime
        /* SPR 15909 fix end */
#endif
        /*32.425 Events code changes end*/
        ) 
{
    UInt32 ret          = RLC_SUCCESS;

    UInt8 triggerStatus = RLC_SUCCESS;
    RlcSn seqNum        = RLC_INVALID_SN;

    /* Stats */
	/* + PERF_CA GBL */
    InternalCellIndex cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(amEntity->rxAMEntity_p->ueIndex);    
    lteRlcStatInfo_g[cellIndex].lteAMStat.amdPduReceived++;
	/* - PERF_CA GBL */
    LteRlcLCStats* lcStats_p =
        &(gRlcStats.pRlcStats->lteRlcUeStat[amEntity->rxAMEntity_p->ueIndex].
                lcStats[amEntity->rxAMEntity_p->lcId]);
    lcStats_p->pduNumUl++;
    lcStats_p->pduBytesUl += amdRawPdu_p->pRawPdu.size;

    /* to decided whether this pdu should be kept or discarded */
    ret = rlcHandleAmdPduFromLowerLayer( amEntity,
            amdRawPdu_p,
            &seqNum,
            &triggerStatus
    /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
            ,packetTime
#endif
    /*32.425 Events code changes end*/
            );
    if( ret != RLC_SUCCESS) 
    {
        LOG_RLC_MSG(LTE_RLC_RECV_DATA_ERR, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                /* + SPR 16766 */
                ret, amEntity->rxAMEntity_p->ueIndex, 
                amEntity->rxAMEntity_p->lcId,0, 0,0, __func__, "");
                /* - SPR 16766 */
        /* discard the received AM PDU */
		/* + PERF_CA GBL */
        lteRlcStatInfo_g[cellIndex].lteAMStat.amdPduDropped++;
		/* - PERF_CA GBL */
        lcStats_p->numPktsDroppedUl++;
        return ret ;
    }
    
    if ((RLC_INVALID_SN != seqNum)) 
    {
        /* keeping the pdu into reception buffer */
        ret =
            rlcActionAmdPduInReceptionBuffer(amEntity,seqNum,triggerStatus) ;
    }
    LOG_RLC_MSG(LTE_RLC_RECV_DATA_INFO, LOGWARNING, 
            (RLC_RX), globalTTITickCount_g, __LINE__,
            seqNum, ret, 0,0, 0,0, __func__, "");
    return ret ;
}



/****************************************************************************
 * Function Name  : rlcHandleAmdPduFromLowerLayer
 * Inputs         : amEntity - pointer to AMEntity,
 *                  amdRawPdu_p - pointer to AmdRawPdu,
 *                  seqNum_p - Pointer to Sequence number,
 *                  triggerStatus_p - Pointer to Trigger Status,
 *                  packetTime
 * Outputs        : x- a pointer to SN and set to SN of the AMD PDU
 * Returns        : RLC_SUCCESS if case success else RLC_FAILURE
 * Description    : describes actions when AM PDU pointed by @amdRawPdu_p 
 *                  recieved from Lower Layer. 
 ****************************************************************************/

STATIC inline UInt32 rlcHandleAmdPduFromLowerLayer
(
    AMEntity *amEntity,
    AmdRawPdu *amdRawPdu_p,
    RlcSn* seqNum_p,
    UInt8 *triggerStatus_p
    /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 15909 fix start */
    ,tickType_t packetTime
    /* SPR 15909 fix end */
#endif
    /*32.425 Events code changes end*/
    )
{
    UInt32 ret       = RLC_FAILURE ;
    AmdPdu *amdPdu_p = RLC_NULL ;
    RingSS snStatus  = RING_SLOT_FREE ;
    UInt16 ringSeqNum    = 0;
    AmdH amdHdr      = {0};

    RxAMEntity *rxAMEntity_p = amEntity->rxAMEntity_p;
    TxAMEntity *txAMEntity_p = amEntity->txAMEntity_p;
    AmdPduRing *ring_p  = &rxAMEntity_p->rxAmdPduRing ;
	/* + PERF_CA GBL */
    InternalCellIndex cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(txAMEntity_p->ueIndex);    
	/* - PERF_CA GBL */

    /*Receive state variable*/
    RlcVr vrR  = rxAMEntity_p->vrR;
    RlcVr vrMR = rxAMEntity_p->vrMR;

    /* decode partially to get SN from amdRawPdu_p */
    ret = rlcDecodeAmdHeader(amdRawPdu_p, &amdHdr); 
    *triggerStatus_p = amdHdr.p;

    /****************************************************/
    if(ret == RLC_FAILURE)
    {
        LOG_RLC_MSG(LTE_RLC_HDL_PDU_ERR, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                /* + SPR 16766 */
                ret, txAMEntity_p->ueIndex, txAMEntity_p->lcId,0, 0,0,
                /* - SPR 16766 */
                __func__, "");
#ifdef DEBUG_STATS
        rxAMEntity_p->numHdrDecFailed++;
#endif
        return ret ;
    }
    /*****************************************************/

    if ( !amdHdr.dc ) /*Check for Status PDU */
    {
        ret = rlcActionOnReceiveStatusReport(txAMEntity_p,amdRawPdu_p);
        msgFree(amdRawPdu_p->pRawPdu.data_p);
        amdRawPdu_p->pRawPdu.data_p = RLC_NULL;

        return ret;
    }
    ringSeqNum = amdHdr.sn;

    if ( ! isWithInRxWindow (vrR, ringSeqNum, vrMR) )
    {
        /*outside window discard the received RLC data PDU */
        LOG_RLC_MSG(LTE_RLC_HDL_PDU_ERR_1, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                vrR, ringSeqNum , vrMR, RLC_FAILURE, 0, 0, __func__, "");
        msgFree(amdRawPdu_p->pRawPdu.data_p);
        amdRawPdu_p->pRawPdu.data_p = RLC_NULL;
#ifdef DEBUG_STATS
        rxAMEntity_p->numPktDropOOW++;
#endif
        /*SPR 10197 Fix Start*/
        if(amdHdr.p)
        {    
            rlcTriggerStatusReport(amEntity);
        }
        /*SPR 10197 Fix End*/
        return RLC_RX_DISCARD_OUT_OF_WINDOW ;

    }
    /*

       Refere 3GPP T.S 36.322

       Section 5.1.3.2.2   Actions when a RLC data PDU is received from lower layer

       When a RLC data PDU is received from lower layer, 
       where the RLC data PDU contains byte segment numbers y to z of an AMD PDU with SN = x, 
       the receiving side of an AM RLC entity shall:

       -   if x falls outside of the receiving window; or
       -   if byte segment numbers y to z of the AMD PDU with SN = x have been received before:
       -   discard the received RLC data PDU;
       -   else:
       -   place the received RLC data PDU in the reception buffer;
       -   if some byte segments of the AMD PDU contained in the RLC data PDU have been received before:
       -   discard the duplicate byte segments.
     */       
    ringSeqNum = ringSeqNum & 511;
    snStatus = GET_RING_SS_AMD_PDU_RN(ring_p, ringSeqNum);
    amdPdu_p = GET_RING_DATA_AMD_PDU_RN(ring_p, ringSeqNum);

    if ( RING_SLOT_FREE != snStatus )
    {
        if ( PDU_AM_COMPLETE == amdPdu_p->status) 
        { 
            LOG_RLC_MSG(LTE_RLC_HDL_PDU_ERR_2, LOGWARNING, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    snStatus,  amdPdu_p->status , RLC_RX_DISCARD_DUPLICATE_PDU, 0, 0, 0, __func__, "");
            /*Duplicate received discards the Rlc data PDU*/
            msgFree(amdRawPdu_p->pRawPdu.data_p);
            amdRawPdu_p->pRawPdu.data_p = RLC_NULL;
            /*SPR 10197 Fix Start*/
            if(amdHdr.p)
            {    
                rlcTriggerStatusReport(amEntity);
            }
            /*SPR 10197 Fix End*/
#ifdef DEBUG_STATS
        rxAMEntity_p->numPktDropDuplicate++;
#endif
            return  RLC_RX_DISCARD_DUPLICATE_PDU ;
        }
        else 
        {
            if (amdHdr.rf) 
            {
                ret = handleRlcAmdPduSegment(amdRawPdu_p, amdPdu_p, &amdHdr
                        /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                        ,packetTime
#endif
                        /*32.425 Events code changes end*/
                        );
                /* + SPR 16766 */
                if(ret != RLC_SUCCESS)
                {
                    LOG_RLC_MSG(LTE_RLC_HDL_PDU_ERR, LOGWARNING, 
                            (RLC_RX), globalTTITickCount_g, __LINE__,
                            ret, txAMEntity_p->ueIndex, txAMEntity_p->lcId ,
                            0 ,0,0, __func__, "2");
                }
                /* - SPR 16766 */
            }
            /* SPR 4190 Fix Start */
            else
            {
                /** SPR-6232 Changes Start **/

                ret = rlcDecodeAmdRawPdu(amdRawPdu_p, amdPdu_p, &amdHdr
                        /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                        ,packetTime
#endif
                        /*32.425 Events code changes end*/
                        );


                if(ret == RLC_SUCCESS)
                {
                    amdPdu_p->status = PDU_AM_COMPLETE;
                }
                else
                {
#ifdef DEBUG_STATS
                    rxAMEntity_p->segPktHandlingFailed++;
#endif
                    /* + SPR 16766 */
                    LOG_RLC_MSG(LTE_RLC_HDL_PDU_ERR, LOGWARNING, 
                            (RLC_RX), globalTTITickCount_g, __LINE__,
                            ret, txAMEntity_p->ueIndex, txAMEntity_p->lcId ,0,
                            0,0, __func__, "3");
                    /* - SPR 16766 */
                    msgFree(amdRawPdu_p->pRawPdu.data_p);
                    amdRawPdu_p->pRawPdu.data_p = RLC_NULL;
                    rlcTriggerStatusReport(amEntity);
                    /* SPR 4546 Changes Start */
                    *seqNum_p = amdHdr.sn;
                    return RLC_FAILURE;
                    //ret = RLC_FAILURE;
                    /* SPR 4546 Changes End */
                }
                /** SPR-6232 Changes Start **/
            }
            /* SPR 4190 Fix End */

        }
    }
    else 
    {
        /* Decode fully amdRawPdu_p buffer to get amdPdu & 
           place in reception buffer (ring_p)*/
        if (amdHdr.rf) 
        {
            ret = handleRlcAmdPduSegment(amdRawPdu_p, amdPdu_p, &amdHdr
                    /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                    ,packetTime
#endif
                    /*32.425 Events code changes end*/
                    );
            /* + SPR 16766 */
            if(ret != RLC_SUCCESS)
            {
#ifdef DEBUG_STATS
                rxAMEntity_p->firstReSegPktHandlingFailed++;
#endif
                LOG_RLC_MSG(LTE_RLC_HDL_PDU_ERR, LOGWARNING, 
                        (RLC_RX), globalTTITickCount_g, __LINE__,
                        ret, txAMEntity_p->ueIndex, txAMEntity_p->lcId ,0, 0,0, __func__, "4");
            }
            /* - SPR 16766 */
        }
        else 
        {
            ret = rlcDecodeAmdRawPdu(amdRawPdu_p, amdPdu_p, &amdHdr
                    /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                    ,packetTime
#endif
                    /*32.425 Events code changes end*/
                    );
            /* SPR 4560 Fix Start */
            if (ret == RLC_SUCCESS)
            {
                amdPdu_p->status = PDU_AM_COMPLETE;
            }
            /* + SPR 16766 */
            if(ret != RLC_SUCCESS)
            {
#ifdef DEBUG_STATS
                rxAMEntity_p->segPktHandlingFailed_2++;
#endif
                LOG_RLC_MSG(LTE_RLC_HDL_PDU_ERR, LOGWARNING, 
                        (RLC_RX), globalTTITickCount_g, __LINE__,
                        ret, txAMEntity_p->ueIndex, txAMEntity_p->lcId ,0, 0,0, __func__, "5");
            }
            /* - SPR 16766 */
            /* SPR 4560 Fix End */
			/* + PERF_CA GBL */
            lteRlcStatInfo_g[cellIndex].lteUEStat.rlcUeStats[rxAMEntity_p->ueIndex].pduReceivedFromMac++;
			/* - PERF_CA GBL */
        }

        if( ret == RLC_SUCCESS)
        {
            SET_RING_SS_AMD_PDU_RN(ring_p, ringSeqNum, RING_SLOT_IN_USE);
        }
    }
    /*When a RLC data PDU with SN = x and the P field set to .1. is received
      from lower layer, the receiving side of an AM RLC entity shall*/
    if ( amdHdr.p)//bug[003]jsm
    {
        /*if the PDU is to be discarded as specified in subclause 5.1.3.2.2; */
        /*if x < VR(MS) or x >= VR(MR):*/
                /* SPR 4546 Changes Start */
        if (RLC_FAILURE == ret) 
                /* SPR 4546 Changes End */
        {

            rlcTriggerStatusReport(amEntity);
        }
    }
    *seqNum_p = amdHdr.sn;

    LOG_RLC_MSG(LTE_RLC_HDL_PDU_ERR_4, LOGINFO, 
            (RLC_RX), globalTTITickCount_g, __LINE__,
           ret , vrR, vrMR , ringSeqNum, 0, 0, __func__, "");
    return ret ;
}


/****************************************************************************
 * Function Name  : checkRxSnEqualToMS
 * Inputs         : ring_p - Pointer to AmdPduRing,
 *                  seqNum - Sequence Number,
 *                  vrMS,
 *                  vrR
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This function will check the AMD PDU of all byte 
 *                  segments Received  
 ****************************************************************************/



inline static UInt32 checkRxSnEqualToMS(  AmdPduRing *ring_p,
                                          UInt16 seqNum,
                                          UInt16 vrMS,
                                          UInt16 vrR
                                       )
{
    RingSS snStatus   = RING_SLOT_FREE ;
    AmdPdu *amdPdu_p = RLC_NULL ;
    UInt16 ringSN    = seqNum & 511;
    
    snStatus = GET_RING_SS_AMD_PDU_RN(ring_p, ringSN);

    if ( RING_SLOT_FREE == snStatus )
    {
        LOG_RLC_MSG(LTE_RLC_CHECK_RX_SN_ERR, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                vrMS, vrR, seqNum,snStatus, 0,0, __func__, "");
        return RLC_FAILURE;
    }
    amdPdu_p = GET_RING_DATA_AMD_PDU_RN(ring_p, ringSN);

    /*if all byte segments of the AMD PDU with SN = VR(MS) are received*/
    if(((PDU_AM_COMPLETE == amdPdu_p->status) || (PDU_AM_SEGMENT_COMPLETE == amdPdu_p->status))
            && (isAMEqual(seqNum, vrMS, vrR)))
    {
             LOG_RLC_MSG(LTE_RLC_CHECK_RX_SN_MS_ERR, LOGINFO, 
                     (RLC_RX), globalTTITickCount_g, __LINE__,
                      seqNum, vrR, vrMS, snStatus, 0,0, __func__, "");
        return RLC_SUCCESS;

    }

    LOG_RLC_MSG(LTE_RLC_CHECK_RX_SN_ERR_1, LOGWARNING, 
            (RLC_RX), globalTTITickCount_g, __LINE__,
            vrMS, vrR, ringSN,snStatus, 0,0, __func__, "");
    return RLC_FAILURE;
}



/****************************************************************************
 * Function Name  : getNextFirstFreeSN
 * Inputs         : seqNum - Sequence Number,
 *                  UH,
 *                  UR,
 *                  rxAMEntity_p - Pointer to RxAMEntity
 * Outputs        : None
 * Returns        : rCount
 * Description    : This function will get the next first ring slot for AMD PDU 
 ****************************************************************************/
inline static UInt32  getNextFirstFreeSN( UInt16 seqNum,
                                          UInt16 UH,
                                          UInt16 UR, 
                                          RxAMEntity *rxAMEntity_p
                                        )
{

    AmdPduRing *ring_p = &rxAMEntity_p->rxAmdPduRing ;
    AmdPdu *amdPdu_p   = RLC_NULL ;
    UInt16 rCount      = 0;
    UInt16 ringSN      = 0;

    rCount = (seqNum + 1) & 1023;
    for( ;!( isAMEqual(rCount, UH, UR));    rCount = (rCount + 1) & 1023  )
    {
        ringSN = rCount & 511;
        if(GET_RING_SS_AMD_PDU_RN(ring_p, ringSN) == RING_SLOT_IN_USE)
        {
            amdPdu_p = GET_RING_DATA_AMD_PDU_RN(ring_p, ringSN);
            if((PDU_AM_COMPLETE != amdPdu_p->status) &&
                    (PDU_AM_SEGMENT_COMPLETE != amdPdu_p->status))
            {
                /*Update the vrMS*/
                /* + Review Comments : vrMS getting updated on return of func */
                /* rxAMEntity_p->vrMS = rCount; */
                /* + Review Comments : vrMS getting updated on return of func */
                break;
            }

        }
        else
        {
            /*Update the vrMS*/
            /* + Review Comments : vrMS getting updated on return of func */
            /* rxAMEntity_p->vrMS = rCount; */
            /* - Review Comments : vrMS getting updated on return of func */

            break;
        }
    }

    LOG_RLC_MSG(LTE_RLC_IN_RECP_BUFFER_INFO_1, LOGINFO, 
            (RLC_RX), globalTTITickCount_g, seqNum,
            UH, UR, rCount, ringSN, 0,0, __func__, "");
    return rCount;
}

/****************************************************************************
 * Function Name  : UpdateURAndReassembleRLCSdu
 * Inputs         : UR,
 *                  rxAMEntity_p - Pointer to RxAMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : This function will update the UR  and reassemeble
 *                  info in the RLC SDU 
 ****************************************************************************/

inline static void UpdateURAndReassembleRLCSdu (
                                                 UInt16 UR,
                                                 RxAMEntity *rxAMEntity_p
                                               )
{
    RlcSnList snList   = RLC_SN_LIST_INIT ;
    UInt16 rCount      = UR;
    AmdPduRing *ring_p = &rxAMEntity_p->rxAmdPduRing ;
    AmdPdu *amdPdu_p   = RLC_NULL ;
    UInt16 ringSN      = 0;

    while ( 1 )
    {
        ringSN = rCount & 511;
        /*if the reception buffer contains an AMD PDU with SN = VR(UR)*/
        if ( GET_RING_SS_AMD_PDU_RN(ring_p, ringSN) == RING_SLOT_IN_USE)
        {
            amdPdu_p = GET_RING_DATA_AMD_PDU_RN(ring_p, ringSN);
            /*if all byte segments of the AMD PDU with SN = VR(R) are received*/
            if((PDU_AM_COMPLETE == amdPdu_p->status)
                    || (PDU_AM_SEGMENT_COMPLETE == amdPdu_p->status))
            {
                SET_RING_SS_AMD_PDU_RN(ring_p, ringSN, RING_SLOT_READY);
                rlcSetSnList(&snList, rCount);

                rCount = (rCount+1) & 1023;

                /*update VR(MR) to the updated VR(R) + AM_Window_Size;*/
                rxAMEntity_p->vrMR = ( rCount + AM_WINDOW_SIZE) & 1023;
                /* update the value UR */
                rxAMEntity_p->vrR = rCount;

                LOG_RLC_MSG(LTE_RLC_IN_RECP_BUFFER_INFO_2, LOGINFO, 
                        (RLC_RX), globalTTITickCount_g, rCount,
                        ringSN, amdPdu_p->status, rxAMEntity_p->vrR, rxAMEntity_p->vrMR, 0,0, __func__, "");
                continue;
            }
        }

        break;
    }
    if ( snList.szUsed != 0)
    {
        rlcReassembleAmdSdu ( rxAMEntity_p, &snList);
    }

    LOG_RLC_MSG(LTE_RLC_IN_RECP_BUFFER_INFO_4, LOGINFO, 
            (RLC_RX), globalTTITickCount_g,__LINE__, rCount,
            ringSN, rxAMEntity_p->vrR, rxAMEntity_p->vrMR, 0,0, __func__, "");
} 

/****************************************************************************
 * Function Name  : checkRlcTriggerStatusReport
 * Inputs         : triggerStatFlag,
 *                  amEntity_p - Pointer to AMEntity,
 *                  seqNum - Sequence number
 * Outputs        : None
 * Returns        : None
 * Description    : This function Checks RLC Trigger Status Report.
 ****************************************************************************/


inline static void checkRlcTriggerStatusReport( UInt32 triggerStatFlag,
                                                AMEntity *amEntity_p,
                                                UInt16 seqNum
                                               )
{
	//bug[003]jsm
	UInt8 triggered = RLC_FALSE;
	if((amEntity_p->rxAMEntity_p->pollDelayInd) && 
            (( isAMLT(amEntity_p->rxAMEntity_p->pollDelaySN, amEntity_p->rxAMEntity_p->vrMS, amEntity_p->rxAMEntity_p->vrR)) 
             ||( isAMGTE(amEntity_p->rxAMEntity_p->pollDelaySN, amEntity_p->rxAMEntity_p->vrMR, amEntity_p->rxAMEntity_p->vrR))))
	{
		rlcTriggerStatusReport(amEntity_p);
		triggered = RLC_TRUE;
	}
    /* SPR 4546 Changes Start */
    /*SPR 10197 Fix Start*/
	if (triggerStatFlag)
    {
    	if (( isAMLT(seqNum, amEntity_p->rxAMEntity_p->vrMS, amEntity_p->rxAMEntity_p->vrR)) 
             ||( isAMGTE(seqNum, amEntity_p->rxAMEntity_p->vrMR, amEntity_p->rxAMEntity_p->vrR)))
		{
			if (RLC_FALSE == triggered)
			{
				rlcTriggerStatusReport(amEntity_p);
			}
    	}
		else
		{
            /*delay triggering the STATUS report until x < VR(MS) or x >= VR(MR).*/
            amEntity_p->rxAMEntity_p->pollDelayInd = RLC_TRUE;
			amEntity_p->rxAMEntity_p->pollDelaySN = seqNum;
		}
    }
    /*SPR 10197 Fix End*/
    /* SPR 4546 Changes End */
}

/****************************************************************************
 * Function Name  : checkReorderingTimer 
 * Inputs         : rxAMEntity_p - Pointer to RxAMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : This function checks Re-ordering timer.
 ****************************************************************************/

inline static void checkReorderingTimer( RxAMEntity *rxAMEntity_p)
{

    UInt16 UH = rxAMEntity_p->vrH;
    UInt16 UR = rxAMEntity_p->vrR;
    UInt16 UX = rxAMEntity_p->vrX;
    UInt16 MR = rxAMEntity_p->vrMR;
    /*
       -   if t-Reordering is running:
       -   if VR(X) = VR(R); or
       -   if VR(X) falls outside of the receiving window and VR(X) is not equal to VR(MR):
       -   stop and reset t-Reordering;

       -   if t-Reordering is not running (includes the case t-Reordering is stopped due to actions above):
       -   if VR (H) > VR(R):
       -   start t-Reordering;
       -   set VR(X) to VR(H).
     */
    /*if T_reordering is running */
    if ( RLC_TRUE == rxAMEntity_p->isRTimerRunning)
    {
        /*if VR(X) = VR(R); or*/
        /*if VR(X) falls outside of the receiving window and VR(X) is not equal to VR(MR)*/
        if(   isAMEqual( UX, UR, UR)
                || ((!( isWithInRxWindow( UR, UX, MR))) &&
                    (! isAMEqual( UX, MR, UR))))
        {
            stopAMROTimer(rxAMEntity_p);
            rxAMEntity_p->isRTimerRunning = RLC_FALSE;
            rxAMEntity_p->vrX = RLC_UNULL;
        }
    }
    /*coverity 54518*/
    //else
    //{
    /*coverity 54518*/
    /*if t-Reordering is not running (includes the case t-Reordering is stopped due to actions above)*/
    if ( RLC_TRUE != rxAMEntity_p->isRTimerRunning)//bug[001]jsm
    {
        if( isAMGT( UH, UR, UR))
        {
            rxAMEntity_p->vrX = UH;
            startAMROTimer(rxAMEntity_p);
            rxAMEntity_p->isRTimerRunning = RLC_TRUE;
        }
    }
    LOG_RLC_MSG(LTE_RLC_IN_RECP_BUFFER_INFO_6, LOGINFO, 
            (RLC_RX), globalTTITickCount_g,  rxAMEntity_p->vrR,
            UH, UX, MR, rxAMEntity_p->isRTimerRunning, 0,0, __func__, "");
}
/****************************************************************************
 * Function Name  : rlcActionAmdPduInReceptionBuffer
 * Inputs         : rxAMEntity_p - pointer to RxAMEntity
 *                  x-  SN of the AM PDU in Reception Buffer
 * Outputs        : 
 * Returns        : RLC_SUCCESS if case success else RLC_FAILURE
 * Variables      :
 * Description    : describes actions AMD PDU indexed by @x.
 ****************************************************************************/
/*
Refer 3GPP T.S 36.322
Section  5.1.3.2.3   Actions when a RLC data PDU is placed in the reception buffer
        
        When a RLC data PDU with SN = x is placed in the reception buffer, 
        the receiving side of an AM RLC entity shall:
        
-   if x >= VR(H)
-   update VR(H) to x+ 1;
-   if all byte segments of the AMD PDU with SN = VR(MS) are received:
-   update VR(MS) to the SN of the first AMD PDU with SN > current VR(MS)
    for which not all byte segments have been received;
    
-   if x = VR(R):
-   if all byte segments of the AMD PDU with SN = VR(R) are received:
-   update VR(R) to the SN of the first AMD PDU with SN > current VR(R) 
    for which not all byte segments have been received;
-   update VR(MR) to the updated VR(R) + AM_Window_Size;
-   reassemble RLC SDUs from any byte segments of AMD PDUs with SN that falls 
    outside of the receiving window and in-sequence byte segments of the AMD PDU
    with SN = VR(R), remove RLC headers when doing so and deliver the reassembled
    RLC SDUs to upper layer in sequence if not delivered before;

-   if t-Reordering is running:
-   if VR(X) = VR(R); or
-   if VR(X) falls outside of the receiving window and VR(X) is not equal to VR(MR):
-   stop and reset t-Reordering;

-   if t-Reordering is not running (includes the case t-Reordering is stopped due to actions above):
-   if VR (H) > VR(R):
-   start t-Reordering;
-   set VR(X) to VR(H).


*/


/****************************************************************************
 * Function Name  : rlcActionAmdPduInReceptionBuffer
 * Inputs         : amEntity - Pointer to AMEntity,
 *                  seqNum - Sequence number,
 *                  triggerStatFlag
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This function process the Received buffer 
 ****************************************************************************/
UInt32 rlcActionAmdPduInReceptionBuffer
(
     AMEntity *amEntity,
     RlcSn seqNum,
     UInt8 triggerStatFlag
)
{
    UInt32 ret = RLC_SUCCESS ;
    RxAMEntity *rxAMEntity_p = amEntity->rxAMEntity_p;
    AmdPduRing *ring_p = &rxAMEntity_p->rxAmdPduRing ;
    /*Receive state variable*/
    RlcVr vrR  = rxAMEntity_p->vrR;
    RlcVr vrMS = rxAMEntity_p->vrMS;
    RlcVr vrH  = rxAMEntity_p->vrH;
    

/*
-   if x >= VR(H)
-   update VR(H) to x+ 1;
*/
    if (isAMGTE(seqNum,  vrH, vrR))
    {
        vrH = (seqNum + 1) & 1023;
        rxAMEntity_p->vrH = vrH;
    }
/*
-   if all byte segments of the AMD PDU with SN = VR(MS) are received:
-   update VR(MS) to the SN of the first AMD PDU with SN > current VR(MS)
    for which not all byte segments have been received;
*/
    if (checkRxSnEqualToMS( ring_p, seqNum, vrMS, vrR ) )
    {
        /*update VR(MS) to the SN of the first AMD PDU with SN > current VR(MS)
          for which not all byte segments have been received*/
        vrMS = getNextFirstFreeSN( seqNum, vrH, vrR, rxAMEntity_p);
        rxAMEntity_p->vrMS = vrMS; 
    }   
    /*if x = VR(R)*/
    if ( isAMEqual( seqNum, vrR,vrR))
    {
        UpdateURAndReassembleRLCSdu( vrR, rxAMEntity_p);
    }
    /* If poll bit is set then trigger Status PDU */
    checkRlcTriggerStatusReport( triggerStatFlag, amEntity, seqNum);
    /* Check RLC AM Reordering timer */
    checkReorderingTimer( rxAMEntity_p);

        LOG_RLC_MSG(LTE_RLC_IN_RECP_BUFFER_INFO, LOGINFO, 
                (RLC_RX), globalTTITickCount_g, vrR,
                vrMS, vrH, ret, 0, 0, 0, __func__, "");
    return ret ;
}

/****************************************************************************
 * Function Name  : rlcActionAmTReorderingExpire
 * Inputs         : rxAMEntity_p - pointer to RxAMEntity
 * Outputs        : None
 * Returns        : RLC_SUCCESS if case success else RLC_FAILURE
 * Description    : It describes actions when re-ordering timer expires.
 ****************************************************************************/
/*
Refer 3GPP T.S 36.322 

Section 5.1.3.2.4   Actions when t-Reordering expires
When t-Reordering expires, the receiving side of an AM RLC entity shall:
-   update VR(MS) to the SN of the first AMD PDU with SN >= VR(X) 
    for which not all byte segments have been received;
-   if VR(H) > VR(MS):
-   start t-Reordering;
-   set VR(X) to VR(H).



*/
/* SPR 5599_129119 Fix Start */
UInt32 rlcActionAmTReorderingExpire(RxAMEntity * rxAMEntity_p) 
/* SPR 5599_129119 Fix End */
{
    UInt32 ret = RLC_SUCCESS ;
    UInt16 rCount = 0 ;

    AmdPdu *amdPdu_p   = RLC_NULL ;
    AmdPduRing *ring_p = &rxAMEntity_p->rxAmdPduRing ;
    
    RlcVr vrR  = rxAMEntity_p->vrR;
    RlcVr vrX  = rxAMEntity_p->vrX;
    RlcVr vrH  = rxAMEntity_p->vrH;

    AMEntity *amEntity_p = RLC_NULL;
    UEContext *ueContext_p = DB_FETCH_UE_CONTEXT(rxAMEntity_p->ueIndex);

    if (ueContext_p)
    {
        RLCEntityInfo* eInfo_p =
                DB_FETCH_RLC_ENTITY_INFO(ueContext_p,rxAMEntity_p->lcId);
        if(eInfo_p && eInfo_p->rlcEntityMode == RLC_ENTITY_AM)
        {
            amEntity_p = eInfo_p->rlcEntity.amEntity_p;
            if(amEntity_p == RLC_NULL)
                return RLC_FAILURE;
        }
        else 
        {
            return RLC_FAILURE;
        }
    }
    else
    {
        return RLC_FAILURE;
    }   

    /*update VR(MS) to the SN of the first AMD PDU with SN >= VR(X) 
      for which not all byte segments have been received*/
    rCount = vrX;

    while (1) 
    {
        if ( GET_RING_SS_AMD_PDU_RN(ring_p, rCount & 511) != RING_SLOT_FREE)
        {
            amdPdu_p = GET_RING_DATA_AMD_PDU_RN(ring_p,rCount & 511);
            if((PDU_AM_COMPLETE != amdPdu_p->status)&&
                    (PDU_AM_SEGMENT_COMPLETE != amdPdu_p->status))
            {
                rxAMEntity_p->vrMS = rCount;
                break ;
            }
        }
        else 
        {
            rxAMEntity_p->vrMS = rCount;
            break ;
        }
        rCount = ((rCount + 1) & 1023 );
    }

    /*if VR(H) > VR(MS)*/
    if( isAMGT( vrH, rxAMEntity_p->vrMS, vrR))
    {
        startAMROTimer(rxAMEntity_p);
        rxAMEntity_p->isRTimerRunning = RLC_TRUE;
        rxAMEntity_p->vrX = vrH;
    }
    else
    {
        rxAMEntity_p->isRTimerRunning = RLC_FALSE ;
        rxAMEntity_p->vrX = RLC_UNULL ;
    }
#ifdef DEBUG_STATS
    rxAMEntity_p->RO_timerExpired++;
#endif
    /* trigger status report*/
    rlcTriggerStatusReport(amEntity_p);

    return ret ;
}

/****************************************************************************
 * Function Name  : rlcDeliverAmSduToUpperLayer
 * Inputs         : rxAMEntity_p - pointer to RxAMEntity, 
 *		    rlcAMBuffer - pointer to RlcAmBuffer
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : delivers reassembled SDU(s) to upper layer (PDCP).
 ****************************************************************************/

UInt32 rlcDeliverAmSduToUpperLayer( RxAMEntity * rxAMEntity_p, 
        RlcAmBuffer *rlcAMBuffer

        /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        /* SPR 15909 fix start */
        ,tickType_t  packetTime
        /* SPR 15909 fix end */
#endif
        /* 32.425 Events code changes end*/
        )
{
#ifdef LINUX_PC_TEST
extern void adapter_rxrlc_sdu_ind(UInt16 ueIndex,UInt16 lcId,UInt8* data_p,UInt16 size);
adapter_rxrlc_sdu_ind(rxAMEntity_p->ueIndex,rxAMEntity_p->lcId,rlcAMBuffer->data_p,rlcAMBuffer->size);
return 1;
#endif
    /* + PERF_CA GBL */
    InternalCellIndex cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(rxAMEntity_p->ueIndex);    
    /* - PERF_CA GBL */
    CLOCK_START_RLC_API(RLC_UE_DATA_IND);
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
	if((SN_TERMINATED_SCG_SPLIT_BEARER ==  rxAMEntity_p->dcBearerType) && (rxAMEntity_p->lcId >= MIN_DRB_LC_ID))
	{
	
		pdcpDataInd( rxAMEntity_p->ueIndex,rxAMEntity_p->lcId,
	        	rlcAMBuffer->data_p,
		        rlcAMBuffer->size) ;

	}
	else
#endif
    /*SPR 21660 changes end_Kishore*/
    sendRlcUeDataIndToPdcp ( rxAMEntity_p->ueIndex,
            rxAMEntity_p->lcId, 
            rlcAMBuffer->data_p,
            rlcAMBuffer->size, 
            /* SPR 15075 fix start */
            rxAMEntity_p->RLCRESETFLAG, 0, 0
            /* SPR 15075 fix end */
            /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
            ,packetTime
#endif
            /* 32.425 Events code changes end*/
            ) ;
    CLOCK_END_RLC_PDCP_API(RLC_UE_DATA_IND);

    /* Update Stats */
    /* + PERF_CA GBL */
    lteRlcStatInfo_g[cellIndex].lteAMStat.amdSduTransmittedToPdcp++;

    lteRlcStatInfo_g[cellIndex].lteUEStat.rlcUeStats[rxAMEntity_p->ueIndex].sduTransmittedToPdcp++;
	/* - PERF_CA GBL */
    LteRlcLCStats* lcStats_p =
        &(gRlcStats.pRlcStats->lteRlcUeStat[rxAMEntity_p->ueIndex].
                lcStats[rxAMEntity_p->lcId]);
    lcStats_p->sduNumUl++;
    lcStats_p->sduBytesUl += rlcAMBuffer->size;
    rlcAMBuffer->data_p = RLC_NULL;
    rlcAMBuffer->size = 0;
    rlcAMBuffer->RLCRESTBFLAG = RLC_FALSE;
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : rlcReassembleInCmpAMSDU 
 * Inputs         : pSduIn - Pointer to AmdRawSduIn,
 *                  rxAMEntity_p - Pointer to RxAMEntity,
 *                  amdDfe_p - Pointer to AmdDFE,
 *                  sn - Sequence number
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the Reassembling of AM SDU 
 ****************************************************************************/

inline static void rlcReassembleInCmpAMSDU( AmdRawSduIn *pSduIn,
                                            RxAMEntity * rxAMEntity_p,
                                            AmdDFE *amdDfe_p,
                                            UInt16 sn
                                         )
{
    RlcAmBuffer *first = &(pSduIn->rlcAmRawSdu);
    RlcAmBuffer *second = &(amdDfe_p->pDFE);
	/* + PERF_CA GBL */
    InternalCellIndex cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(rxAMEntity_p->ueIndex);    
	/* - PERF_CA GBL */

    first->RLCRESTBFLAG = ((rxAMEntity_p->RLCRESETFLAG == RLC_TRUE) ?
            RLC_TRUE : RLC_FALSE);
    switch (amdDfe_p->status)
    {
        case AM_PDU_MAP_SDU_END:

            if(PNULL == (msgJoinAndFree(first->data_p,second->data_p)))
            {
                LOG_RLC_MSG(LTE_RLC_REASSEMBLE_ERR, LOGWARNING, 
                        (RLC_RX), globalTTITickCount_g, __LINE__,
                        0, 0, 0, 0, 0,0, __func__, "");
                return;                
            }

            LOG_RLC_MSG(LTE_RLC_REASSEMBLE_IN_CMP_SDU, LOGINFO,
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    rxAMEntity_p->ueIndex, rxAMEntity_p->lcId, sn,
                    first->size, second->size, amdDfe_p->status,
                    __func__, "joinIncmpAndNew");

            first->size += second->size;
            if(pSduIn->rlcAmRawSdu.size > 8190)
            {
                lteRlcStatInfo_g[cellIndex].lteAMStat.amdIncompSduDiscard++;
                msgFree(pSduIn->rlcAmRawSdu.data_p);
                pSduIn->rlcAmRawSdu.data_p= RLC_NULL ;
                pSduIn->rlcAmRawSdu.size = 0;
                pSduIn->rlcAmRawSdu.RLCRESTBFLAG = RLC_FALSE;
                msgFree(amdDfe_p->pDFE.data_p);
                amdDfe_p->pDFE.data_p = RLC_NULL;
                amdDfe_p->pDFE.size = 0;
                break;
            }
            rlcDeliverAmSduToUpperLayer(rxAMEntity_p,first
                    /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                    ,amdDfe_p->packetTime
#endif
                    /* 32.425 Events code changes end*/
                    );
            first->data_p = PNULL;
            first->size   = 0;

            break;

        case AM_PDU_MAP_SDU_MID:

            if ( PNULL == (msgJoinAndFree(first->data_p, second->data_p)))
            {
                LOG_RLC_MSG(LTE_RLC_REASSEMBLE_IN_CMP_SDU_ERR, LOGINFO,
                        (RLC_RX), globalTTITickCount_g, __LINE__,
                        rxAMEntity_p->ueIndex, rxAMEntity_p->lcId,
                        sn, first->size, second->size, 0
                        , __func__, "FailMsgJoin");
                return;
            }

            LOG_RLC_MSG(LTE_RLC_REASSEMBLE_IN_CMP_SDU_1, LOGINFO,
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    rxAMEntity_p->ueIndex, rxAMEntity_p->lcId, sn,
                    first->size, second->size, amdDfe_p->status,
                    __func__, "joinIncmpAndNew");

            first->size += second->size;
            pSduIn->rlcAmRawSdu.size   = first->size;
            pSduIn->rlcAmRawSdu.data_p = first->data_p;
            pSduIn->sn = sn ;

            break;

        case AM_PDU_MAP_SDU_FULL:
			/* + PERF_CA GBL */
            lteRlcStatInfo_g[cellIndex].lteAMStat.amdIncompSduDiscard++;
			/* - PERF_CA GBL */
            msgFree(pSduIn->rlcAmRawSdu.data_p);
            pSduIn->rlcAmRawSdu.data_p= RLC_NULL ;
            pSduIn->rlcAmRawSdu.size = 0;

            LOG_RLC_MSG(LTE_RLC_REASSEMBLE_IN_CMP_SDU_2, LOGINFO,
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    rxAMEntity_p->ueIndex, rxAMEntity_p->lcId,
                    sn, 0, second->size, amdDfe_p->status, __func__, "fullSDU");

            rlcDeliverAmSduToUpperLayer(rxAMEntity_p, &amdDfe_p->pDFE
                    /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                    ,amdDfe_p->packetTime
#endif
                    /* 32.425 Events code changes end*/
                    );

            break;

        default :
			/* + PERF_CA GBL */
            lteRlcStatInfo_g[cellIndex].lteAMStat.amdIncompSduDiscard++;
			/* - PERF_CA GBL */
            msgFree(pSduIn->rlcAmRawSdu.data_p);
            pSduIn->rlcAmRawSdu.data_p = amdDfe_p->pDFE.data_p;
            pSduIn->rlcAmRawSdu.size =  amdDfe_p->pDFE.size ;
            amdDfe_p->pDFE.data_p = RLC_NULL;
            amdDfe_p->pDFE.size = 0;
            pSduIn->sn = sn ;

            LOG_RLC_MSG(LTE_RLC_REASSEMBLE_IN_CMP_SDU_ERR_2, LOGINFO,
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    rxAMEntity_p->ueIndex, rxAMEntity_p->lcId,
                    sn, second->size, 0, amdDfe_p->status,
                    __func__, "IncmpSdu");


            break;

    }

}

/****************************************************************************
 * Function Name  : deliverAmSduToPDCP
 * Inputs         : amdDfe_p - Pointer to AmdDFE,
 *                  rxAMEntity_p - Pointer to RxAMEntity,
 *                  pSduIn - Pointer to AmdRawSduIn,
 *                  sn - sequence number
 * Outputs        : None
 * Returns        : None
 * Description    : This function will send the AM SDU to PDCP 
 ****************************************************************************/

inline static void deliverAmSduToPDCP( AmdDFE *amdDfe_p,
                                       RxAMEntity * rxAMEntity_p,
                                       AmdRawSduIn *pSduIn,
                                       UInt16 sn
                                     )
{
    InternalCellIndex cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(rxAMEntity_p->ueIndex);

    switch ( amdDfe_p->status )
    {
        case AM_PDU_MAP_SDU_FULL:

            LOG_RLC_MSG( LTE_RLC_DELIVER_AM_SDU_TO_PDCP, LOGINFO,
                    (RLC_RX), globalTTITickCount_g,
                    rxAMEntity_p->ueIndex, rxAMEntity_p->lcId,
                    amdDfe_p->status, sn,  amdDfe_p->pDFE.size,
                    0,0, __func__, "fullSdu reassembled");
            if(amdDfe_p->pDFE.size > 8190)
            {
                lteRlcStatInfo_g[cellIndex].lteAMStat.amdIncompSduDiscard++;
                msgFree(amdDfe_p->pDFE.data_p);
                amdDfe_p->pDFE.data_p = RLC_NULL;
                amdDfe_p->pDFE.size = 0;
                amdDfe_p->pDFE.RLCRESTBFLAG = RLC_FALSE;
                break;
            }

            rlcDeliverAmSduToUpperLayer(rxAMEntity_p, &amdDfe_p->pDFE
                    /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                    ,amdDfe_p->packetTime
#endif
                    /* 32.425 Events code changes end*/
                    );

            break;

        case AM_PDU_MAP_SDU_START:

            LOG_RLC_MSG( LTE_RLC_DELIVER_AM_SDU_TO_PDCP, LOGINFO, 
                    (RLC_RX), globalTTITickCount_g,
                    rxAMEntity_p->ueIndex, rxAMEntity_p->lcId,
                    amdDfe_p->status, sn, amdDfe_p->pDFE.size,
                    0,0, __func__, "Sdu start");
            pSduIn->rlcAmRawSdu.data_p = amdDfe_p->pDFE.data_p ;
            pSduIn->rlcAmRawSdu.size = amdDfe_p->pDFE.size;

            amdDfe_p->pDFE.data_p = RLC_NULL;
            amdDfe_p->pDFE.size = 0;
            pSduIn->sn = sn;



            break;

        default:

            LOG_RLC_MSG( LTE_RLC_DELIVER_AM_SDU_TO_PDCP, LOGINFO,
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    rxAMEntity_p->ueIndex, rxAMEntity_p->lcId,
                    amdDfe_p->status, sn, amdDfe_p->pDFE.size,
                    0, __func__, "InvalidUmdStatus");

            msgFree(amdDfe_p->pDFE.data_p) ;
            /* SPR 20985 fix start */
            amdDfe_p->pDFE.data_p = RLC_NULL;
            /* SPR 20985 fix end */

            break;
    }
}

/****************************************************************************
 * Function Name  : handleRlcReassembleInCmpAmSDU
 * Inputs         : pSduIn - Pointer to AmdRawSduIn,
 *                  sn - Sequence number
 *                  amdDfe_p - Pointer to AmdDFE,
 *                  rxAMEntity_p - Pointer to RxAMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : This function will handles the RLC Reassemble in AM SDU 
 ****************************************************************************/
inline static void handleRlcReassembleInCmpAmSDU (  AmdRawSduIn *pSduIn,
                                                    UInt16 sn,
                                                    AmdDFE *amdDfe_p,
                                                    RxAMEntity * rxAMEntity_p
                                                 )
{
    UInt16 UR     = rxAMEntity_p->vrR;
	/* + PERF_CA GBL */
    InternalCellIndex cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(rxAMEntity_p->ueIndex);    
	/* - PERF_CA GBL */
    /* make it the head of UmdRawSduQ if SN is in sequence */
    if ( (isAMEqual(sn, (pSduIn->sn + 1) & 1023 , UR)) ||
            (isAMEqual(sn, (pSduIn->sn) & 1023 , UR)))

    {  /* Both Sequence Numbers are equal */

        rlcReassembleInCmpAMSDU ( pSduIn, rxAMEntity_p, amdDfe_p, sn);

    }
    else
    {
        /* Recv Sequence number is not equal
           to previous incomplete sdu sequence*/

        /* discard this portion of out of sequence PDU.. it is loss of SDU */
		/* + PERF_CA GBL */
        lteRlcStatInfo_g[cellIndex].lteAMStat.amdIncompSduDiscard++;
		/* - PERF_CA GBL */
        msgFree(pSduIn->rlcAmRawSdu.data_p);
        pSduIn->rlcAmRawSdu.data_p = RLC_NULL ;
        pSduIn->rlcAmRawSdu.size   = 0;

        LOG_RLC_MSG( LTE_RLC_HANDLE_RLC_REASSEMBLE_SDU, LOGINFO,
                (RLC_RX), globalTTITickCount_g,
                rxAMEntity_p->ueIndex, rxAMEntity_p->lcId,
                sn, amdDfe_p->status,  amdDfe_p->pDFE.size,
                0,0, __func__, "deliverUmSduToPDCP");

        deliverAmSduToPDCP( amdDfe_p, rxAMEntity_p, pSduIn, sn);
    }

}



/****************************************************************************
 * Function Name  : rlcTraverseAndHandleAmdDfeQ
 * Inputs         : amdPduSegment_p - Pointer to AmdPduSegment,
 *                  rxAMEntity_p - Pointer to RxAMEntity,
 *                  seqNum - Sequence number
 * Outputs        : None
 * Returns        : None
 * Description    : 
 ****************************************************************************/

inline static void rlcTraverseAndHandleAmdDfeQ(  AmdPduSegment * amdPduSegment_p,
                                                  RxAMEntity * rxAMEntity_p,
                                                  UInt16 seqNum
                                              )
{
    AmdDFE *amdDfe_p     = RLC_NULL ;
    AmdRawSduIn *pSduIn = &rxAMEntity_p->rxSduIn ;

    DEQUEUE_AMD_PDU_DFE_Q(amdPduSegment_p->dfeQ,amdDfe_p);

    while ( amdDfe_p != RLC_NULL && amdDfe_p->pDFE.data_p != RLC_NULL ) 
    {
        if ( RLC_NULL != pSduIn->rlcAmRawSdu.data_p)
        {
            handleRlcReassembleInCmpAmSDU( pSduIn,
                    seqNum,
                    amdDfe_p,
                    rxAMEntity_p );
        }
        else
        {
            deliverAmSduToPDCP (
                    amdDfe_p,
                    rxAMEntity_p,
                    pSduIn,
                    seqNum /*umdPdu_p->h.sn*/
                    );
        }
        freeMemPool(amdDfe_p);
        DEQUEUE_AMD_PDU_DFE_Q(amdPduSegment_p->dfeQ, amdDfe_p); 
    }

}

/****************************************************************************
 * Function Name  : rlcReassembleAmdSdu
 * Inputs         : rxAMEntity_p - a pointer to RxAMEntity,
 *                  snList - a pointer to RlcSnList and this list contains the
 *                           SN(s) of AMD PDUs to be reassembled
 * Outputs        : None
 * Returns        : RLC_SUCCESS if case success else RLC_FAILURE
 * Description    : reassembles all AM PDU(s) with SN in @snList.
 ****************************************************************************/
UInt32 rlcReassembleAmdSdu ( RxAMEntity * rxAMEntity_p,
                             RlcSnList *snList
                           )
{
    UInt32 ret       = RLC_SUCCESS;
    UInt16 rCount    = 0 ;
    AmdPdu *amdPdu_p = RLC_NULL ;
    AmdPduSegment * amdPduSegment_p = RLC_NULL;
    AmdPduRing *ring_p  = &rxAMEntity_p->rxAmdPduRing ;
    UInt16 ringSN       = 0;

    for(; rCount < snList->szUsed; rCount++)
    {
        RlcSn sn = snList->snList[rCount];
        ringSN   = sn & 511;
        amdPdu_p = GET_RING_DATA_AMD_PDU_RN(ring_p, ringSN);

        /*TI_TICKOVERSHOOT Start*/
        DEQUEUE_AMD_PDU_SEGMENT_Q((amdPdu_p),amdPduSegment_p);
        //DEQUEUE_AMD_PDU_SEGMENT_Q((&amdPdu_p->pduSegmentQ),amdPduSegment_p);
        /*TI_TICKOVERSHOOT End*/



        while( amdPduSegment_p != RLC_NULL )
        {
            rlcTraverseAndHandleAmdDfeQ( amdPduSegment_p, rxAMEntity_p, sn);
            /* CSR 51542 fix start */
            semDestroy((LTE_SEM *)&amdPduSegment_p->dfeQ.pQ->sem_lock);
            /* CSR 51542 fix end */
            freeMemPool(amdPduSegment_p->dfeQ.pQ);
            /*SPR 16979 Fix start*/
            amdPduSegment_p->dfeQ.pQ = RLC_NULL;
            /*SPR 16979 Fix End*/
            freeMemPool(amdPduSegment_p);
            amdPduSegment_p = RLC_NULL;
            /*TI_TICKOVERSHOOT Start*/
            DEQUEUE_AMD_PDU_SEGMENT_Q((amdPdu_p),amdPduSegment_p);
            //DEQUEUE_AMD_PDU_SEGMENT_Q((&amdPdu_p->pduSegmentQ),amdPduSegment_p);
            /*TI_TICKOVERSHOOT End*/
        }
        SET_RING_SS_AMD_PDU_RN(ring_p, ringSN, RING_SLOT_FREE) ;

        LOG_RLC_MSG(LTE_RLC_IN_RECP_BUFFER_INFO_3, LOGINFO, 
                (RLC_RX), globalTTITickCount_g, rCount,
                sn, rxAMEntity_p->ueIndex, rxAMEntity_p->lcId, ret, 0,0, __func__, "");
    }
    return ret;
}
/**********************************************************************************************
* Function Name   : rlcReassembleAmdSduForReestablish
* Inputs         : rxAMEntity_p - a pointer to RxAMEntity,
*                  snList - a pointer to RlcSnList and this list contains the
*                           SN(s) of AMD PDUs to be reassembled
* Outputs        : None
* Returns        : RLC_SUCCESS if case success else RLC_FAILURE
* Description    : reassembles all AM PDU(s) with SN in @snList When Reestablishment
**********************************************************************************************/
UInt32 rlcReassembleAmdSduForReestablish ( RxAMEntity * rxAMEntity_p, RlcSnList *snList )
{
    UInt32 ret       = RLC_SUCCESS;
    UInt16 rCount    = 0 ;
    AmdPdu *amdPdu_p = RLC_NULL ;
    AmdPduSegment * amdPduSegment_p = RLC_NULL;
    AmdPduRing *ring_p  = &rxAMEntity_p->rxAmdPduRing ;
    UInt16 ringSN       = 0;
    UInt32 soStartTmp = 0;

    for(; rCount < snList->szUsed; rCount++)
    {
        RlcSn sn = snList->snList[rCount];
        ringSN   = sn & 511;
        amdPdu_p = GET_RING_DATA_AMD_PDU_RN(ring_p, ringSN);
        DEQUEUE_AMD_PDU_SEGMENT_Q((amdPdu_p),amdPduSegment_p);

        if( (amdPduSegment_p->soStart != 0) && (rxAMEntity_p->RLCRESETFLAG == RLC_TRUE) )
        {
            if( RLC_NULL != rxAMEntity_p->rxSduIn.rlcAmRawSdu.data_p)
            {
                msgFree(rxAMEntity_p->rxSduIn.rlcAmRawSdu.data_p);
                rxAMEntity_p->rxSduIn.rlcAmRawSdu.data_p = RLC_NULL ;
                rxAMEntity_p->rxSduIn.rlcAmRawSdu.size = 0;
            }
        }

        while( amdPduSegment_p != RLC_NULL )
        {
            rlcTraverseAndHandleAmdDfeQ( amdPduSegment_p, rxAMEntity_p, sn);
            soStartTmp = amdPduSegment_p->soEnd + 1;

            semDestroy((LTE_SEM *)&amdPduSegment_p->dfeQ.pQ->sem_lock);
            freeMemPool(amdPduSegment_p->dfeQ.pQ);
            amdPduSegment_p->dfeQ.pQ = RLC_NULL;

            freeMemPool(amdPduSegment_p);
            amdPduSegment_p = RLC_NULL;

            DEQUEUE_AMD_PDU_SEGMENT_Q((amdPdu_p),amdPduSegment_p);
            if( (rxAMEntity_p->RLCRESETFLAG == RLC_TRUE) && (amdPdu_p->status != PDU_AM_COMPLETE) )//AM PDU Segment
            {
                if( (RLC_NULL !=amdPduSegment_p) && (soStartTmp < amdPduSegment_p->soStart)) //so discontinuous
                {
                    msgFree(rxAMEntity_p->rxSduIn.rlcAmRawSdu.data_p);
                    rxAMEntity_p->rxSduIn.rlcAmRawSdu.data_p = RLC_NULL ;
                    rxAMEntity_p->rxSduIn.rlcAmRawSdu.size = 0;
                }
            }
       }

       SET_RING_SS_AMD_PDU_RN(ring_p, ringSN, RING_SLOT_FREE) ;

       LOG_RLC_MSG(LTE_RLC_IN_RECP_BUFFER_INFO_3, LOGINFO,
               (RLC_RX), globalTTITickCount_g, rCount, sn, rxAMEntity_p->ueIndex, rxAMEntity_p->lcId, ret, 0,0, __func__, "");
    }
    return ret;
}
/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : rlcFillAmdPduSegmentInfo
 * Inputs         : amRxE_p - pointer to RxAm Entity,
 *                  amdPdu_p - a pointer to amdPdu,
 *                  nackList_p - pointer to nacklist,
 *                  eStatusPduLenInBits - pointer to size of status pdu,
 *                  rCount
 * Outputs        : None
 * Returns        : RLC_FAILURE/RLC_SUCCESS
 * Description    : It fills segment info in nack list.
 ****************************************************************************/
STATIC inline UInt32 rlcFillAmdPduSegmentInfo(
    RxAMEntity *amRxE_p,
    AmdPdu *amdPdu_p,
    NackList *nackList_p,
    UInt32 *eStatusPduLenInBits,
    UInt16 rCount)
{
    AmdPduSegment *amdSeg_p = PNULL;
    UInt32 idx   = 0;
    UInt16 lastSoStart = 0;
    UInt16 lastSoEnd   = 0;
    UInt16 prevSoEnd   = 0;
    UInt32 queueCntVar      = 0;
                
    /*TI_TICKOVERSHOOT Start*/
    /* Count the number of segemnts of the amdPdu */
    queueCntVar = queueCount(&(amdPdu_p->LQ));
    /*TI_TICKOVERSHOOT End*/
    /* SPR 3881 Fix Start */
    /*Calculating status pdu length in bits */
    /*coverity_530_54585*/
    while ( ( queueCntVar--) && (idx < (MAX_SEG_PDU -1)) )
    /*coverity_530_54585*/
    /* SPR 3881 Fix End */
    {
    /* if amdSeg is not the first node */
        if (amdSeg_p) 
        {
            /*TI_TICKOVERSHOOT Start*/
            amdSeg_p = (AmdPduSegment*)getNextQueueNode(&(amdPdu_p->LQ),&(amdSeg_p->qN));
            /* + coverity 24594 */
            if ( PNULL == amdSeg_p )
            {
                LOG_RLC_MSG(LTE_RLC_CHECK_STATUS_REP_BUFF_ERR_1, LOGWARNING,
                        (RLC_RX), globalTTITickCount_g, __LINE__,
                        RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "");
                return RLC_FAILURE;
            }
            /* - coverity 24594 */
            /*TI_TICKOVERSHOOT Start*/
            if (prevSoEnd == amdSeg_p->soStart)
            {
                lastSoStart = prevSoEnd = amdSeg_p->soEnd+1;
                /* SPR 3993 Fix Start */
                if(!(amdSeg_p->lsf == 0x01))   
                {
                    lastSoEnd = 0;
                }
                /* SPR 3993 Fix End */
                if(queueCntVar) 
                {
                    continue;
                }
                else 
                {
                    /* SPR 3993 Fix Start */
                    if (amdSeg_p->lsf==0x01)
                    /* SPR 3993 Fix End */
                    continue;
                }
            }
            else 
            {
                lastSoStart = prevSoEnd;
                lastSoEnd = amdSeg_p->soStart;
                prevSoEnd =  amdSeg_p->soEnd+1;
                /* SPR 3993 Fix Start */
                queueCntVar?:(amdSeg_p->lsf?:(lastSoEnd =0));
                /* SPR 3993 Fix End */
                /** SPR 4000 Fix Start **/
                if ((amdSeg_p->lsf == 0) && (!queueCntVar))
                {
                    nackList_p->noOfSeg+=1;
                    nackList_p->nack_sn = rCount;
                    nackList_p->rlcSeg[idx].e2=0x01;
                    nackList_p->rlcSeg[idx].soStart
                        =lastSoStart;
                    nackList_p->rlcSeg[idx++].soEnd
                        = lastSoEnd?lastSoEnd-1:RLC_AM_SO_MASK;
                    *eStatusPduLenInBits==RLC_TWOBYTE?\
                         ((*eStatusPduLenInBits)+=(RLC_E_SIZE+RLC_AM_SN_SIZE\
                             +(2*RLC_AM_SO_SIZE))):
                         ((*eStatusPduLenInBits)+=((2*RLC_E_SIZE)+RLC_AM_SN_SIZE\
                             +(2*RLC_AM_SO_SIZE))); 
                   lastSoStart =  amdSeg_p->soEnd+1;
                   lastSoEnd = 0;
               }
               /** SPR 4000 Fix End **/
           }
       }
       else 
       {
           /*TI_TICKOVERSHOOT Start*/
           amdSeg_p = (AmdPduSegment*)
                      getFirstQueueNode(&(amdPdu_p->LQ));
           /*TI_TICKOVERSHOOT Start*/
           if(RLC_NULL == amdSeg_p)
           {
               break;
           }
           if (amdSeg_p->soStart)
           {
               lastSoEnd = amdSeg_p->soStart;
               prevSoEnd = amdSeg_p->soEnd+1;
               /** SPR 4000 Fix Start **/
               if ((amdSeg_p->lsf == 0) && (!queueCntVar))
               {
                   nackList_p->noOfSeg+=1;
                   nackList_p->nack_sn = rCount;
                   nackList_p->rlcSeg[idx].e2=0x01;
                   nackList_p->rlcSeg[idx].soStart =lastSoStart;
                   nackList_p->rlcSeg[idx++].soEnd
                       = lastSoEnd?lastSoEnd-1:RLC_AM_SO_MASK;
                   *eStatusPduLenInBits==RLC_TWOBYTE?\
                         ((*eStatusPduLenInBits)+=(RLC_E_SIZE+RLC_AM_SN_SIZE\
                          +(2*RLC_AM_SO_SIZE))):
                         ((*eStatusPduLenInBits)+=((2*RLC_E_SIZE)+RLC_AM_SN_SIZE\
                          +(2*RLC_AM_SO_SIZE))); 
                   lastSoStart =  amdSeg_p->soEnd+1;
                   lastSoEnd = 0;
               }
               /** SPR 4000 Fix End **/
           }
           else 
           {
               lastSoStart = prevSoEnd = amdSeg_p->soEnd+1;
               if (queueCntVar)
                   continue;
               lastSoEnd = 0;
           }
       }
       nackList_p->noOfSeg+=1;
       nackList_p->nack_sn = rCount;
       nackList_p->rlcSeg[idx].e2=0x01;
       nackList_p->rlcSeg[idx].soStart =lastSoStart;
       /* Coverity Fix for RLC[SPR 4477] */
       nackList_p->rlcSeg[idx].soEnd = lastSoEnd?lastSoEnd-1:RLC_AM_SO_MASK;
       /* Coverity Fix End [SPR 4477]*/
       *eStatusPduLenInBits==RLC_TWOBYTE?\
          ((*eStatusPduLenInBits)+=(RLC_E_SIZE+RLC_AM_SN_SIZE\
           +(2*RLC_AM_SO_SIZE))):
          ((*eStatusPduLenInBits)+=((2*RLC_E_SIZE)+RLC_AM_SN_SIZE\
           +(2*RLC_AM_SO_SIZE)));
       LOG_RLC_MSG(LTE_RLC_RING_SLOT_NOT_FREE_1, LOGDEBUG,(RLC_TX|RLC_AM),
          globalTTITickCount_g,amRxE_p->ueIndex, amRxE_p->lcId,rCount,
          nackList_p->rlcSeg[idx].soStart,
          nackList_p->rlcSeg[idx].soEnd,0,0, __func__, "");
       /* Coverity Fix for RLC[SPR 4477] */
       idx++;
       /* Coverity Fix End [SPR 4477]*/
   }
    return RLC_SUCCESS;
}
/* Cyclomatic Complexity changes - ends here */

/****************************************************************************
 * Function Name  : rlcTriggerStatusReport
 * Inputs         : amEntity - a pointer to AMEntity
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : It triggers the status Pdu tx request.
 ****************************************************************************/

UInt32 rlcTriggerStatusReport(AMEntity *amEntity)
{
    RxAMEntity *amRxE_p = amEntity->rxAMEntity_p;
    TxAMEntity *amTxE_p = amEntity->txAMEntity_p;
    UInt32 eStatusPduLenInBits = RLC_TWOBYTE;
    
    if(RLC_TRUE == amRxE_p->isSPTimerRunning)
    {
        LOG_RLC_MSG(LTE_RLC_STATUS_REPORT_DELAY, LOGDEBUG, (RLC_RX|RLC_AM), globalTTITickCount_g, __LINE__,
                amRxE_p->ueIndex,amRxE_p->lcId,0,0, 0,0, __func__, "");
        amRxE_p->sendStatusInd = RLC_TRUE;
        return RLC_FAILURE; 
    }
	amRxE_p->sendStatusInd = RLC_FALSE;//bug[003]jsm
	
    RlcVr vrMS_t  = amRxE_p->vrMS;
    AmdPduRing *ring_p = &amRxE_p->rxAmdPduRing ;

    UInt32 statusPduLen     = 0;
    AmdPdu *amdPdu_p        = RLC_NULL;
    UInt32 eIndx = 0;
    LCQueueLoad lcQLoad = {0};

    AmRxNackListNode *amRxNackListNode_p = RLC_NULL;
    AmRxNackListNode *tempNackListNode_p = RLC_NULL;
    /** RLC Nack List Changes Start **/
    UInt16 nackListSize = 0;
    nackListSize = getNackListSize(amRxE_p->vrR,amRxE_p->vrMS);
    /** Creating Nack List node **/
    CREATE_AMD_RX_NACK_LIST_NODE(amRxNackListNode_p,(nackListSize+1));
    /** RLC Nack List Changes End **/
    if(amRxNackListNode_p == RLC_NULL)
    {
        return RLC_FAILURE;
    }

    NackListArr *nackList = &(amRxNackListNode_p->nackListArr);

    nackList->count = 0;
    nackList->nackListArray[0].e1 = 0;


    UInt16 rCount = amRxE_p->vrR;
    /* Calculate the length of status pdu*/
    /* SPR 3881 Fix start */
    /* SPR 4010 Fix start */
    for(;(isAMLTE (amRxE_p->vrR, rCount, amRxE_p->vrR) && isAMLT (rCount, vrMS_t, amRxE_p->vrR) && (eIndx < (AM_WINDOW_SIZE - 1)));
          rCount = (rCount+1) & 1023)
    /* SPR 4010 Fix End */
    /* SPR 3881 Fix End */
    {
        nackList->nackListArray[eIndx].noOfSeg = 0;

        /* If ring slot has AMD PDU */
        if(GET_RING_SS_AMD_PDU_RN(ring_p,rCount & 511) != RING_SLOT_FREE)
        {
            /* Get AMD PDU from ring */
            amdPdu_p = GET_RING_DATA_AMD_PDU_RN(ring_p,rCount & 511);

            /* if amdPdu is segemnted */
            if( PDU_AM_SEGMENT == amdPdu_p->status)
            {
                /* Cyclomatic Complexity changes - starts here */
                if (RLC_SUCCESS != rlcFillAmdPduSegmentInfo( amRxE_p,amdPdu_p, 
                            &nackList->nackListArray[eIndx],
                            &eStatusPduLenInBits,rCount))
                {
                    freeMemPool( amRxNackListNode_p );
                    return RLC_FAILURE;
                }
                /* Cyclomatic Complexity changes - ends here */
                nackList->nackListArray[eIndx].e1 = 0x01;
                eIndx++;
            }
        }
        /* If ring slot is free */
        else
        {
            eStatusPduLenInBits==RLC_TWOBYTE?\
                                (eStatusPduLenInBits+=(RLC_E_SIZE+RLC_AM_SN_SIZE)):
                                    (eStatusPduLenInBits+=((2*RLC_E_SIZE)+RLC_AM_SN_SIZE));
            nackList->nackListArray[eIndx].e1 = 0x01;
            nackList->nackListArray[eIndx].nack_sn = rCount;
#ifdef DEBUG_STATS            
            if(eIndx < MAX_RLC_RX_STATUS)
            {
                amRxE_p->rxRlcStatus[amRxE_p->statusCount].nack_sn[eIndx] = rCount;
            }
#endif
            nackList->nackListArray[eIndx].rlcSeg[0].e2=0x00;
            eIndx++;
            LOG_RLC_MSG(LTE_RLC_RING_SLOT_FREE, LOGDEBUG,(RLC_TX|RLC_AM),  globalTTITickCount_g, __LINE__,
                    amRxE_p->ueIndex, amRxE_p->lcId,rCount, amRxE_p->vrMS, 0,0, __func__, "");
        }
    }
    nackList->count = eIndx;
    nackList->ack_Sn = amRxE_p->vrMS;
    nackList->nackListArray[eIndx].e1 = 0x00;
    statusPduLen = eStatusPduLenInBits >> 3;
    //statusPduLen = eStatusPduLenInBits/8;
    //if(eStatusPduLenInBits%8){
    if(eStatusPduLenInBits & 7){
        statusPduLen++;
    }
    nackList->size = statusPduLen; 
#ifdef DEBUG_STATS            
        amRxE_p->rxRlcStatus[amRxE_p->statusCount].ack_Sn  =  nackList->ack_Sn;
        amRxE_p->rxRlcStatus[amRxE_p->statusCount].count =  nackList->count;
        amRxE_p->rxRlcStatus[amRxE_p->statusCount].size = nackList->size;
        amRxE_p->ttiForStatus[amRxE_p->statusCount] = globalTTITickCount_g; 
        amRxE_p->statusCount = ((amRxE_p->statusCount + 1)% MAX_RLC_RX_STATUS);
#endif

    LOG_RLC_MSG(LTE_RLC_TRIGGER_STATUS_PDU, LOGDEBUG, (RLC_RX|RLC_AM), globalTTITickCount_g, __LINE__,
            amRxE_p->ueIndex,amRxE_p->lcId, statusPduLen,
            nackList->ack_Sn, 0,0, __func__, "");
    /** SPR 5604 Start **/
    /** Enqueue the Rx Nack List node **/
    
    /* + Coverity 32529 */
    if(!(ENQUEUE_AMD_RX_NACK_LIST_Q(amRxE_p->amRxNackList,amRxNackListNode_p)))
    {
#ifdef DEBUG_STATS
        amRxE_p->enQFailNackList++;
#endif

       freeMemPool(amRxNackListNode_p);
       return RLC_FAILURE ;
    }
    /* - Coverity 32529 */

   /** Enqueue the Rx Nack List node **/
    while (circQueueCount(amRxE_p->amRxNackList.pQ) > 1)
    {
    /** SPR 5604 End **/
        DEQUEUE_AMD_RX_NACK_LIST_Q(tempNackListNode_p, amRxE_p->amRxNackList);
        if (tempNackListNode_p != RLC_NULL) 
        {
            freeMemPool(tempNackListNode_p);
            /* + SPR 5584 (COVERITY CID 25540) FIX */
            tempNackListNode_p = PNULL;
            /* - COVERITY CID 25540 FIX */
        }
    }
            
    amTxE_p->sendStatusInd = RLC_TRUE; 

    amTxE_p->rxStatusPduSize = statusPduLen;
    lcQLoad.lcId = amTxE_p->lcId; 
    /** Updating Qload to MAC **/
    rlcFetchQueueLoad(amTxE_p,&lcQLoad);
    macUpdateBufferOccupancyReq(amTxE_p->ueIndex, 1 /*lcCount*/, &lcQLoad,RLC_BO_TYPE_NEW,
     rlcUEContextList_g[amTxE_p->ueIndex]->cellIndex
    );

    LOG_RLC_MSG(LTE_RLC_STATUS_REPORT_INFO, LOGINFO, 
            (RLC_RX), globalTTITickCount_g, __LINE__,
            amRxE_p->vrMS, amRxE_p->vrR , amTxE_p->rxStatusPduSize, amTxE_p->lcId, 0, 0, __func__, "");

    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : rlcActionOnExpireStatusProhibitTimer
 * Inputs         : pAmRxE - a pointer to RxAMEntity
 * Outputs        : None
 * Returns        : RLC_TRUE
 * Description    : trigger the status Pdu tx request.
 ****************************************************************************/

inline UInt32 rlcActionOnExpireStatusProhibitTimer(RxAMEntity * pAmRxE)
{
    pAmRxE->isSPTimerRunning = RLC_FALSE;
    return RLC_TRUE;
}

/****************************************************************************
 * Function Name  : rlcGetRingStatusForAM
 * Inputs         : rxAMEntity_p - a pointer to RxAMEntity
 * Outputs        : The number of active rings
 * Returns        : activeRing
 * Description    : This API is used to get ring status for AM.
 ****************************************************************************/

UInt32 rlcGetRingStatusForAM(RxAMEntity * rxAMEntity_p)
{
    RingSS snStatus = RING_SLOT_FREE ;
    UInt32 seqNum = 0,activeRing = 0;
    AmdPduRing *ring_p = &rxAMEntity_p->rxAmdPduRing ;
    for(seqNum = 0 ;seqNum< AM_WINDOW_SIZE;seqNum++)
    {
        snStatus = GET_RING_SS_AMD_PDU_RN(ring_p,seqNum);
        if(snStatus != RING_SLOT_FREE) 
        {
            activeRing++;
        }
    }
    return activeRing;
}

