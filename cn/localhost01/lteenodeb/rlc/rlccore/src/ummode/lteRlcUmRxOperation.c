/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcUmRxOperation.c,v 1.1.1.1.12.2.2.3 2010/10/14 05:56:45 gur19479 Exp $
 *
 ****************************************************************************
 *
 *  File Description : this file describes the UM Entity methods.  
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcUmRxOperation.c,v $
 *
 *
****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcPdcpInterface.h"
#include "lteRlcUmRxOperation.h"
#include "lteRlcUmdDecode.h"
#include "logging.h"
#include "alarm.h"
#include "lteRlcStat.h"
#include "lteRlcErrorCode.h"
#include "lteRlcTimer.h"
/* + PERF_CA GBL */
#include "lteRlcUeContext.h"
#if (defined(LTE_EMBMS_SUPPORTED) && defined(UE_SIM_TESTING))    
#include "lteRlcEmbmsDef.h"
#endif
/* - PERF_CA GBL */
 /****************************************************************************
 * Varibale Definition
 ****************************************************************************/
/****************************************************************************
 * External Variable Includes
 ****************************************************************************/
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */
UInt32 rxOutOfOrderPdu_g= 0;

#ifndef UE_SIM_TESTING
#define MAX_SEG_IND_LIMIT_UL  10
#endif
/* SPR 5599_129124 Fix Start */
UInt32 rlcActionUmPduInReceptionBuffer( RxUMEntity * rxUMEntity_p,
/* SPR 5599_129124 Fix End */
                                        RlcSn seqNum
                                       );
static UInt32 rlcHandleUmPduFromLowerLayer( RxUMEntity *rxUMEntity_p,
                                             UmdRawPdu  *umdRawPdu_p,
                                             RlcSn* seqNum_p
#ifndef UE_SIM_TESTING
                                             /* SPR 15909 fix start */
                                             , tickType_t pktTimeStamp,
                                             /* SPR 15909 fix end */
                                             UInt8 spsState
#endif
                                             /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                                             /* SPR 15909 fix start */
                                             ,tickType_t packetTime
                                             /* SPR 15909 fix end */
#endif
                                             /* 32.425 Events code changes end*/
        );

UInt32 rlcReassembleUmSdu( RxUMEntity * rxUMEntity_p,
                           RlcSnList *snList
                         );
                                                    
static UInt32 isUmEQ   ( UInt16 x,
                                UInt16 y, 
                                SInt32 mBase, 
                                UInt16 modVal);
#ifdef ENDC_ENABLED
extern void pdcpDataInd( UInt16 ueIndex, UInt16 lcId, void* data_p, UInt16 size);
#endif
/****************************************************************************
 * Function Name  : rlcReceiveUmPduFromLowerLayer
 * Inputs         : rxUMEntity_p - pointer to RxUMEntity,
 *                  umdRawPdu_p - pointer to UmdRawPdu,
 *                  pktTimeStamp - Packet Time Stamp,
 *                  spsState - State of SPS,
 *                  packetTime - Packet Time
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE/errorCode 
 * Description    : This method is responsible to take action when a pdu recieved 
 *                  from lower layer.
 ****************************************************************************/
/* SPR 5599_129124 Fix Start */
UInt32 rlcReceiveUmPduFromLowerLayer ( RxUMEntity * rxUMEntity_p,
/* SPR 5599_129124 Fix End */
                                       UmdRawPdu *umdRawPdu_p
                                      
#ifndef UE_SIM_TESTING
                                             /* SPR 15909 fix start */
                                              , tickType_t pktTimeStamp,
                                             /* SPR 15909 fix end */
                                              UInt8 spsState
#endif
                                              /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                                             /* SPR 15909 fix start */
                                              ,tickType_t packetTime
                                             /* SPR 15909 fix end */
#endif
                                              /*32.425 Events code changes end*/
        ) 
{
    UInt32 errorCode = INVALID_ERR_CODE;
    RlcSn seqNum = INVALID_SEQ_NUM ;
    InternalCellIndex cellIndex;    
    LteRlcLCStats* lcStats_p;
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
    if(!rxUMEntity_p->mchEntity)
    {    
#endif    
    lcStats_p =
        &(gRlcStats.pRlcStats->lteRlcUeStat[rxUMEntity_p->ueIndex].
                  lcStats[rxUMEntity_p->lcId]);
	/* + PERF_CA GBL */
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
    /*SPR 22684 Fix3 Start*/ 
    if(!rxUMEntity_p->mchEntity)
    {    
    cellIndex = RLC_FETCH_INTERNAL_AREA_CELL_INDEX(rxUMEntity_p->ueIndex);   
    }
    /*SPR 22684 Fix3 Stop*/
#else    
    cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(rxUMEntity_p->ueIndex);    
#endif    
    lteRlcStatInfo_g[cellIndex].lteUMStat.rxUmdPdu++;
	/* - PERF_CA GBL */
    lcStats_p->pduNumUl++;
    lcStats_p->pduBytesUl += GET_SIZE_UMD_RAW_PDU(umdRawPdu_p);
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
    }    
#endif    
    /* to decided whether this pdu should be kept or discarded */
    errorCode =
        rlcHandleUmPduFromLowerLayer ( rxUMEntity_p,
                                       umdRawPdu_p,
                                       &seqNum
#ifndef UE_SIM_TESTING
                                       , pktTimeStamp,
                                       spsState
#endif
                                       /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                                       ,packetTime
#endif
                                       /*32.425 Events code changes end*/
                );
    if ( RLC_SUCCESS != errorCode ) 
    {
        LOG_RLC_MSG(LTE_RLC_RECEIVE_UM_PDU_ERR, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                seqNum, errorCode, 0, 0, __func__, "");
        /* discard the received UMD PDU */
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
        if(!rxUMEntity_p->mchEntity)
        {    
#endif    
        lteRlcStatInfo_g[cellIndex].lteUMStat.rxUmdPduDropped++;
        lcStats_p->numPktsDroppedUl++;
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
        }    
#endif    
        return errorCode;
    }
    /* keeping the pdu into reception buffer */
    errorCode =  rlcActionUmPduInReceptionBuffer( rxUMEntity_p, seqNum) ;

    return errorCode;
}

/****************************************************************************
 * Function Name  : getAbs 
 * Inputs         : n - number of which absolute value is required 
 * Outputs        : None
 * Returns        : This function will return absoulte value of n
 * Description    : It gets absolute value of passed parameter. 
 ****************************************************************************/
#define CHAR_BT 8
unsigned int getAbs(int n)
{
    int const mask = n >> (sizeof(int) * CHAR_BT - 1);
    return ((n + mask) ^ mask);
}

/****************************************************************************
 * Function Name  : isValidRxUMPduForReceptionBuffer
 * Inputs         : rxUMEntity_p - Pointer to RxUMEntity,
 *                  seqNum - Sequence Number
 * Outputs        : None
 * Returns        : RLC_SUCCESS/errorCode
 * Description    : The function will check the Received UM PDU 
 *                  1. Dupilcate UM PDU
 *                  2. Out of Window.
 ****************************************************************************/
/*
Actions when an UMD PDU is received from lower layer
When an UMD PDU with SN = x is received from lower layer, the receiving UM RLC entity shall:
-   if VR(UR) < x < VR(UH) and the UMD PDU with SN = x has been received before; or
-   if (VR(UH) - UM_Window_Size) <= x < VR(UR):
-   discard the received UMD PDU;
-   else:
-   place the received UMD PDU in the reception buffer.
*/
UInt32 isValidRxUMPduForReceptionBuffer( RxUMEntity *rxUMEntity_p ,
                                                       UInt16 seqNum
                                                      )
{

    RlcVr UR     = rxUMEntity_p->UR ;
    RlcVr UH     = rxUMEntity_p->UH ;
    /*    
      VR(UH) - UM_Window_Size shall be assumed as the modulus base
      at the receiving side of an UM RLC entity. 
     */  
    SInt32 mbase = UH - rxUMEntity_p->wSize;

    UInt32 wSize              = rxUMEntity_p->wSize;
    UInt32 modValForWrapRound = rxUMEntity_p->modValForWrapRound; 
    UmdPduRing *ring_p        = &rxUMEntity_p->rxUmdPduRing ;

    /* SPR 21575 + */
    UInt32 r = (UR - mbase) & modValForWrapRound;
    UInt32 x = (seqNum - mbase) & modValForWrapRound;
    UInt32 h = (UH - mbase) & modValForWrapRound;
    /* SPR 21575 - */

    RingSS snStatus  = RING_SLOT_FREE ;
   /* SPR 3546 - 3547 changes start */ 
    snStatus = GET_RING_SS_UMD_PDU_RN(ring_p, seqNum, modValForWrapRound);
   /* SPR 3546 - 3547 changes end */ 

        LOG_RLC_MSG( LTE_RLC_IS_VALID_RX_UM_PDU, LOGINFO, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                seqNum, rxUMEntity_p->lcId, rxUMEntity_p->ueIndex, snStatus, 0, 0, __func__, "");
    /* Refer 3GPP T.S 36.322  
       if VR(UR) < x < VR(UH) 
       and the UMD PDU with SN = x has been received before;
     */

    if ( ( (r < x) && (x < h) ) && (snStatus != RING_SLOT_FREE) )
    {
        LOG_RLC_MSG( LTE_RLC_IS_VALID_RX_UM_PDU_ERR, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                seqNum, rxUMEntity_p->lcId, rxUMEntity_p->ueIndex, snStatus, 0, 0, __func__, "");

        return RLC_RX_DISCARD_DUPLICATE_PDU;
    }

    h = ( getAbs( (UH - wSize ) -  mbase) ) & modValForWrapRound;

    /* Refer 3GPP T.S 36.322  
       -   if (VR(UH) - UM_Window_Size) <= x < VR(UR):
     */
    if ( (h <= x) && (x < r) )
    {
        LOG_RLC_MSG( LTE_RLC_IS_VALID_RX_UM_PDU_ERR_1, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                 UR, seqNum, UH, RLC_RX_DISCARD_OUT_OF_WINDOW, 
                 0, 0, __func__, "");

        return RLC_RX_DISCARD_OUT_OF_WINDOW;
    }

    return RLC_SUCCESS;

}

/****************************************************************************
 * Function Name  : rlcHandleUmPduFromLowerLayer
 * Inputs         : rxUMEntity_p - pointer to RxUMEntity,
 *                  umdRawPdu_p - pointer to UmdRawPdu,
 *                  seqNum_p - Pointer to RlcSn,
 *                  pktTimeStamp - Packet Time Stamp,
 *                  spsState - State of SPS,
 *                  packetTime
 * Outputs        : x - a pointer to SN and set to SN of the UMD PDU
 * Returns        : RLC_SUCCESS/RLC_FAILURE/errorcode
 * Description    : It describes actions when UMD PDU pointed by @umdRawPdu_p 
 *                  recieved from Lower Layer. 
 ****************************************************************************/
static UInt32 rlcHandleUmPduFromLowerLayer( RxUMEntity *rxUMEntity_p,
                                                   UmdRawPdu  *umdRawPdu_p,
                                                   RlcSn* seqNum_p
#ifndef UE_SIM_TESTING
                                                    /* SPR 15909 fix start */
                                                   , tickType_t pktTimeStamp,
                                                    /* SPR 15909 fix end */
                                                   UInt8 spsState
#endif
                                                   /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                                                    /* SPR 15909 fix start */
                                                   ,tickType_t packetTime
                                                    /* SPR 15909 fix end */
#endif
                                                   /*32.425 Events code changes end*/
        )
{
    UInt32 errorCode = INVALID_ERR_CODE;
    UmdPdu *umdPdu_p = RLC_NULL ;
    UInt32 seqNum    = 0;
    /* + Coverity 32398 */
    UmdH umdHdr ;     
    /* + Coverity 55023, 55022 */
    umdHdr.snLen = invalidSnSize;
    umdHdr.sz    = 0;
    umdHdr.sn    = 0;
    umdHdr.fi    = RLC_DF_FB_LB_MATCH;
    /* - Coverity 55023, 55022 */
    umdHdr.e     = 0;
    /* - Coverity 32398 */
	/* + PERF_CA GBL */
    InternalCellIndex cellIndex=0;    
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
    if(!rxUMEntity_p->mchEntity)
    {    
      cellIndex = RLC_FETCH_INTERNAL_AREA_CELL_INDEX(rxUMEntity_p->ueIndex);   
    }  
#else   
      cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(rxUMEntity_p->ueIndex);    
#endif    
	/* - PERF_CA GBL */

    SnFieldLength snLen = rxUMEntity_p->snFieldLength ;
    UmdPduRing *ring_p  = &rxUMEntity_p->rxUmdPduRing ;
    UInt8  szFHdr       = rxUMEntity_p->szFHdr;

    /* decode partially to get SN from umdRawPdu_p */
    errorCode = rlcDecodeUmdH(umdRawPdu_p, &umdHdr, snLen);
    
    if ( RLC_SUCCESS != errorCode)
    {
        /* Rx pdu ptr is null */
        LOG_RLC_MSG( LTE_RLC_HANDLE_UM_PDU_ERR, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
               umdRawPdu_p->pRawPdu.size , errorCode,
                0, 0, __func__, "");

        return RLC_RX_INVALID_NULL_PDU;
    }

    seqNum = umdHdr.sn;

    errorCode = isValidRxUMPduForReceptionBuffer ( rxUMEntity_p, seqNum);

    if ( errorCode != RLC_SUCCESS )
    {
       /* Packet duplicate or out of window */
        LOG_RLC_MSG(LTE_RLC_RECEIVE_UM_PDU_ERR_1, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                umdRawPdu_p->pRawPdu.size, seqNum, errorCode,
                0, __func__, "");

        return errorCode;
    }
    /* Decode fully umdRawPdu_p buffer to get UmdPdu & 
       place in reception buffer (ring_p)*/
    umdPdu_p  = GET_RING_DATA_UMD_PDU_RN(ring_p, seqNum);

    LOG_RLC_MSG(LTE_RLC_RECEIVE_UM_PDU, LOGDEBUG,
           (RLC_RX), globalTTITickCount_g, __LINE__,
           rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
           seqNum,rxUMEntity_p->UR,rxUMEntity_p->UH, 0, __func__, "");

    errorCode = 
             rlcDecodeUmdRawPdu(umdRawPdu_p, umdPdu_p, &umdHdr, szFHdr
#ifndef UE_SIM_TESTING
             , &(rxUMEntity_p->rlcRxSegCounter)
             , pktTimeStamp,
             spsState
#endif
             /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
             ,packetTime
#endif
             /*32.425 Events code changes end*/
             ) ;


    if ( errorCode != RLC_SUCCESS )
    {
        LOG_RLC_MSG(LTE_RLC_RECEIVE_UM_PDU_ERR_2, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                umdRawPdu_p->pRawPdu.size, seqNum, 
                 errorCode, 0, __func__, "");
                
        EMPTY_UMDDFE_Q((&umdPdu_p->df));
        return errorCode;
    }

#ifndef UE_SIM_TESTING
    /* Send Seg Ind only if seg counter reaches Max limit */
    if( (rxUMEntity_p->isSpsLc) && (rxUMEntity_p->rlcRxSegCounter == MAX_SEG_IND_LIMIT_UL))
    {
        macUeSegPduInd(rxUMEntity_p->ueIndex, rxUMEntity_p->lcId, RLC_PDU_SEG_IND,cellIndex);
        rxUMEntity_p->rlcRxSegCounter = 0;
    }
#endif

    /* Keep in Reception Buffer */
    SET_RING_SS_UMD_PDU_RN(ring_p, seqNum, RING_SLOT_IN_USE) ;

    *seqNum_p = seqNum;

	/* + PERF_CA GBL */
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
    if(!rxUMEntity_p->mchEntity)
    {    
#endif    
    lteRlcStatInfo_g[cellIndex].lteUEStat.rlcUeStats[rxUMEntity_p->ueIndex].pduReceivedFromMac++;
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
    }
#endif    
	/* - PERF_CA GBL */

    return errorCode;
}

/*****************************************************************************************
 * Function Name  : isXfallOutSideReOrderingW
 * Inputs         : UH,
 *                  seqNum - sequence number,
 *                  wSize - window size,
 *                  modValForWrapRound - value after which wrapping required
 * Outputs        : None
 * Returns        : RLC_RX_OUTSIDE_RE_ORDERING_WINDOW/RLC_RX_WITH_IN_RE_ORDERING_WINDOW
 * Description    : The function will check within reOrdering window or not.
 ******************************************************************************************/
static UInt32 isXfallOutSideReOrderingW(
                                                UInt16 UH,
                                                UInt16 seqNum,
                                                UInt16 wSize,
                                                UInt16 modValForWrapRound
                                              )
{
    /*
       Refere 3GPP T.S 36.322 Section 5.1.2.2.1
       a SN falls within the reordering window if (VR(UH) - UM_Window_Size) <= SN < VR(UH);
     */

    SInt32 mbase = (UH - wSize);
    
    /* SPR 7922 Changes Start */
    UInt32 lh  = (((SInt32)UH - wSize) - mbase)  & modValForWrapRound;
    UInt32 x   = ((SInt32)seqNum - mbase) & modValForWrapRound;
    UInt32 h   = ((SInt32)UH - mbase) & modValForWrapRound;
    /* SPR 7922 Changes End */

    /*LOG_RLC_MSG( LTE_RLC_HANDLE_UM_PDU_ERR_3, LOGDEBUG, 
            (RLC_RX), globalTTITickCount_g, __LINE__,
            lh, x, h, mbase,  0, 0, __func__, "");*/

    if ( lh <= x  && x < h )
    {
        return RLC_RX_WITH_IN_RE_ORDERING_WINDOW;
    }
    else
    {
        return RLC_RX_OUTSIDE_RE_ORDERING_WINDOW;
    } 

}

/****************************************************************************
 * Function Name  : isUmEQ
 * Inputs         : x,
 *                  y,
 *                  mBase,
 *                  modVal
 * Outputs        : None
 * Returns        : 0 or 1 
 * Description    : The function will check both state variables are equal.
 ****************************************************************************/

static UInt32 isUmEQ   ( UInt16 x,
                                UInt16 y, 
                                SInt32 mBase, 
                                UInt16 modVal)
{
    return ( ( ( getAbs(x - mBase) ) & modVal ) 
            == ( ( getAbs(y - mBase) ) & modVal ) );
}
/* SPR 4440 Fix Start */
/* SPR 4440 Fix End */

/****************************************************************************
 * Function Name  : isUmGTE
 * Inputs         : x,
 *                  y,
 *                  mBase,
 *                  modVal
 * Outputs        : None
 * Returns        : 0 or 1
 * Description    : The function will check that x is greater than equal to y.
 ****************************************************************************/

static UInt32 isUmGTE( UInt16 x, 
                              UInt16 y,
                              SInt32 mBase,
                              UInt16 modVal
                            )
{
    return ( ((getAbs( x - mBase) ) & modVal)
                >= ((getAbs( y - mBase) ) & modVal) );

}

/****************************************************************************
 * Function Name  : isUmGT
 * Inputs         : x,
 *                  y,
 *                  mBase,
 *                  modVal
 * Outputs        : None
 * Returns        : 0 or 1
 * Description    : The function will check that x is greater than y.
 ****************************************************************************/

static UInt32 isUmGT( UInt16 x,
                              UInt16 y,
                              SInt32 mBase,
                              UInt16 modVal
                            )
{
    return ( ((getAbs( x - mBase) ) & modVal)
                > ((getAbs( y - mBase) ) & modVal) );

}

/****************************************************************************
 * Function Name  : rlcActionUmPduInReceptionBuffer
 * Inputs         : rxUMEntity_p - pointer to RxUMEntity,
 *                  seqNum - Sequence Number
 * Outputs        : None
 * Returns        : RLC_SUCCESS if case is success else RLC_FAILURE
 * Description    : describes actions UMD PDU indexed by @x.
 ****************************************************************************/
/*
Refere 3GPP T.S 36.322 Section 5.1.2.2.1
a SN falls within the reordering window if (VR(UH) - UM_Window_Size) <= SN < VR(UH);

Refere 3GPP T.S 36.322 Section 5.1.2.2.3

Actions when an UMD PDU is placed in the reception buffer
When an UMD PDU with SN = x is placed in the reception buffer, 
the receiving UM RLC entity shall:

-   if x falls outside of the reordering window:
-   update VR(UH) to x + 1;
-   reassemble RLC SDUs from any UMD PDUs with SN 
    that falls outside of the reordering window, 
    remove RLC headers when doing so and deliver the 
    reassembled RLC SDUs to upper layer in ascending order 
    of the RLC SN if not delivered before;
-   if VR(UR) falls outside of the reordering window:
-   set VR(UR) to (VR(UH) - UM_Window_Size);

-   if the reception buffer contains an UMD PDU with SN = VR(UR):
-   update VR(UR) to the SN of the first UMD PDU with SN > current VR(UR) that has not been received;
-   reassemble RLC SDUs from any UMD PDUs with SN < updated VR(UR),
    remove RLC headers when doing so and deliver the reassembled 
    RLC SDUs to upper layer in ascending order of the 
    RLC SN if not delivered before;
    
-   if t-Reordering is running:
-   if VR(UX) <= VR(UR); or
-   if VR(UX) falls outside of the reordering window and VR(UX) is not equal to VR(UH)::
-   stop and reset t-Reordering;

-   if t-Reordering is not running (includes the case when t-Reordering is stopped due to actions above):
-   if VR(UH) > VR(UR):
-   start t-Reordering;
-   set VR(UX) to VR(UH).

*/
/* SPR 5599_129124 Fix Start */
UInt32 rlcActionUmPduInReceptionBuffer( RxUMEntity * rxUMEntity_p,
/* SPR 5599_129124 Fix End */
                                         RlcSn seqNum
                                      )
{
    UInt32 errorCode = INVALID_ERR_CODE;
    UInt16 rCount    = 0 ;
    //UInt16 ringSN    = 0;
    SInt32 mBase     = 0; 
    RlcSnList snList = RLC_SN_LIST_INIT ;

    UmdPduRing *ring_p = &rxUMEntity_p->rxUmdPduRing ;

    RlcVr UR = rxUMEntity_p->UR ;
    RlcVr UH = rxUMEntity_p->UH ;
    RlcVr UX = rxUMEntity_p->UX ;

    UInt16 wSize  = rxUMEntity_p->wSize;
    UInt16 modVal = rxUMEntity_p->modValForWrapRound;
    
    mBase = UH - wSize;

    /* Check that SN falls within the reordering window */ 
    errorCode = isXfallOutSideReOrderingW( UH, seqNum, wSize, modVal);


    if ( RLC_RX_OUTSIDE_RE_ORDERING_WINDOW == errorCode )
    {
        /*
           -   if x falls outside of the reordering window:
           -   update VR(UH) to x + 1;
         */
        rxUMEntity_p->UH  = ( (seqNum + 1) & modVal );
        UH = rxUMEntity_p->UH;
        /* update the value of mBase */
        mBase = UH - wSize;
        /*
           -   reassemble RLC SDUs from any UMD PDUs with SN
           that falls outside of the reordering window,
           remove RLC headers when doing so and deliver the
           reassembled RLC SDUs to upper layer in ascending order
           of the RLC SN if not delivered before;
         */           
        UInt16 tempSN = UR;
        /* For Testing Purpose only*/
        UInt8 flag = 2;
        /*
           Refere 3GPP T.S 36.322 Section 5.1.2.2.1
           a SN falls within the reordering window if (VR(UH) - UM_Window_Size) <= SN < VR(UH);
         */
        while ( RLC_RX_OUTSIDE_RE_ORDERING_WINDOW ==
                isXfallOutSideReOrderingW ( UH , tempSN, wSize, modVal))
        {
                /* SPR 3546 changes start */ 
            if (  RING_SLOT_IN_USE == GET_RING_SS_UMD_PDU_RN(ring_p,tempSN, modVal ) )
                /* SPR 3546 changes end */ 
            {
                SET_RING_SS_UMD_PDU_RN(ring_p, tempSN, RING_SLOT_READY);
                rlcSetSnList(&snList, tempSN);

                LOG_RLC_MSG(LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER, LOGDEBUG,
                        (RLC_RX), globalTTITickCount_g, __LINE__,
                        rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                        tempSN, UH,0,0, __func__,"");
            }
            tempSN = (tempSN + 1 ) & modVal;

            /** As we are preparing Sn list for reassembly, so no need to put below logs **/
/*            LOG_RLC_MSG( LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_1, LOGDEBUG, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                    tempSN, UH, mBase, 0, __func__, 
                    ""); */
            /* Only Testing Purpose */
            flag = 1;
        }
        /*reassemble RLC SDUs */
        if ( snList.szUsed != 0 )
        {
            rlcReassembleUmSdu(rxUMEntity_p, &snList); 
        }

        /* check within the reordering window
           if (VR(UH) - UM_Window_Size) <= VR(UR) < VR(UH);
           if VR(UR) falls outside of the reordering window
           -   set VR(UR) to (VR(UH) - UM_Window_Size);
         */
        if ( RLC_RX_OUTSIDE_RE_ORDERING_WINDOW ==
                isXfallOutSideReOrderingW ( UH, UR, wSize, modVal ))
        {
            UR = (UH - wSize) & modVal;
            rxUMEntity_p->UR = UR;

            LOG_RLC_MSG( LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_2, LOGDEBUG, 
                    (RLC_RX), globalTTITickCount_g, 
                    rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                    UH, UR, wSize, 0,0, __func__, 
                    "");
            /* Only Testing Purpose */
            flag = 2;        
        }

        /* Only Testing Purpose */
        if ( flag != 2 )
        {
            ltePanic("flag is not 2 we have to update UR");
        }

        LOG_RLC_MSG( LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_3, LOGDEBUG, 
                (RLC_RX), globalTTITickCount_g,
                rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                UH, UR, wSize, 0,0, __func__, 
                "");
    }
    /*
       -   if the reception buffer contains an UMD PDU with SN = VR(UR):
       -   update VR(UR) to the SN of the first UMD PDU with SN > current VR(UR) that has not been received;
       -   reassemble RLC SDUs from any UMD PDUs with SN < updated VR(UR),
       remove RLC headers when doing so and deliver the reassembled
       RLC SDUs to upper layer in ascending order of the
       RLC SN if not delivered before;
     */
    /*if the reception buffer contains an UMD PDU with SN = VR(UR)*/
    
    /** SPR 7922 Changes Start **/
    if ( (seqNum == UR)  || 
        ((RING_SLOT_IN_USE == GET_RING_SS_UMD_PDU_RN(ring_p,UR, modVal ))))
    /** SPR 7922 Changes End **/
    {
        /* Only Testing purpose */
            /* SPR 3546 changes start */ 
        if  (GET_RING_SS_UMD_PDU_RN(ring_p, UR,modVal ) != RING_SLOT_IN_USE) 
            /* SPR 3546 changes end */ 
        {
            ltePanic("Some logic error occured SeqNum[%d]  UR [%d]\n",seqNum, UR);
        }

        LOG_RLC_MSG( LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_4, LOGDEBUG, 
                (RLC_RX), globalTTITickCount_g, 
                rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                UH, UR, seqNum, 0,0, __func__, 
                "");
        /*update VR(UR) to the SN of the first UMD PDU 
          with SN > current VR(UR) that has not been received;*/
        rCount = UR;

        for( ; ; rCount = ( (rCount + 1) & (modVal) ) )
        {   
                /* SPR 3546 changes start */ 
            if ( RING_SLOT_IN_USE == GET_RING_SS_UMD_PDU_RN(ring_p, rCount, modVal))
                /* SPR 3546 changes end */ 
            {
                SET_RING_SS_UMD_PDU_RN(ring_p, rCount, RING_SLOT_READY);
                rlcSetSnList(&snList, rCount);

                LOG_RLC_MSG( LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_6, LOGDEBUG,
                        (RLC_RX), globalTTITickCount_g,
                        rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                        UH, UR, seqNum, 0,0, __func__,
                        "");
            }
                /* SPR 3546 changes start */ 
            else if ( RING_SLOT_FREE == GET_RING_SS_UMD_PDU_RN(ring_p, rCount,modVal))
                /* SPR 3546 changes end */ 
            {
                rxUMEntity_p->UR = rCount;  
                UR = rCount;

                LOG_RLC_MSG( LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_5, LOGDEBUG, 
                        (RLC_RX), globalTTITickCount_g, 
                        rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                        UH, UR, seqNum, 0,0,  __func__, 
                        "");
                break ;
            }
            else
            {
                /* SPR 3546 changes start */ 
                ltePanic("Invalid scenario UR [%d] State [%d] \n",
                                    rCount, GET_RING_SS_UMD_PDU_RN(ring_p,
                                    rCount, modVal));      
                /* SPR 3546 changes end */ 
            }
        }

        /*reassemble RLC SDUs */
        if(snList.szUsed != 0)
        {
            rlcReassembleUmSdu(rxUMEntity_p,&snList); 
        }

        LOG_RLC_MSG( LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_7, LOGDEBUG, 
                (RLC_RX), globalTTITickCount_g, 
                rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                UH, UR, seqNum, 0,0, __func__, 
                "");
    }
    else 
    {

        /*Increment stat for out of order*/
        ++rxOutOfOrderPdu_g;

        LOG_RLC_MSG( LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_8, LOGDEBUG, 
                (RLC_RX), globalTTITickCount_g, 
                rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                UH, UR, seqNum, 0,0, __func__, 
                "");
    }

    /*if T_reordering is running */
    if( rxUMEntity_p->isTimerRunning == RLC_TRUE )
    {
        /* if VR(UX) <= VR(UR); or
           if VR(UX) falls outside of the reordering window and 
           VR(UX) is not equal to VR(UH) */

        if ( isUmLTE( UX, UR, mBase, modVal) 
                || (( RLC_RX_OUTSIDE_RE_ORDERING_WINDOW ==
                        isXfallOutSideReOrderingW ( UX, UR, wSize, modVal) ) 
                      && !isUmEQ(UX, UH, mBase, modVal))
           )
        {
            /* stop and reset T_reordering;*/ 
            rxUMEntity_p->isTimerRunning = RLC_FALSE ;
            stopUMROTimer(rxUMEntity_p);
            
            /** SPR 7922 Changes Start **/
            rxUMEntity_p->UX = RLC_UNULL;
            /** SPR 7922 Changes End **/
            LOG_RLC_MSG( LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_9, LOGDEBUG, 
                    (RLC_RX), globalTTITickCount_g,
                    rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                    UH, UR, UX, 0,0, __func__, 
                    "StopROTimer");
        }
    }

    if( rxUMEntity_p->isTimerRunning != RLC_TRUE )
    {
        /*if VR(UH) > VR(UR)*/
        if ( isUmGT ( UH, UR, mBase, modVal))
        {
            /*start T_reordering;
              set VR(UX) to VR(UH). */
            rxUMEntity_p->isTimerRunning = RLC_TRUE ;
            startUMROTimer( rxUMEntity_p );
            rxUMEntity_p->UX = UH;

            LOG_RLC_MSG( LTE_RLC_ACTION_UM_PDU_IN_RECEPTION_BUFFER_10, LOGDEBUG, 
                    (RLC_RX), globalTTITickCount_g, 
                    rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                    UH, UR, UX, 0,0, __func__, "StartROTimer");
        }
    }
    return RLC_SUCCESS ;
}
/****************************************************************************
 * Function Name  : rlcActionUmTReorderingExpire
 * Inputs         : rxUMEntity_p - pointer to RxUMEntity
 * Outputs        : None
 * Returns        : RLC_SUCCESS if case is success else RLC_FAILURE
 * Description    : describes actions when re-ordering timer expires.
 ****************************************************************************/
/* 
Refer 3GPP T.S 36.322 Section 5.1.2.2.4   
Actions when t-Reordering expires

When t-Reordering expires, the receiving UM RLC entity shall:
-   update VR(UR) to the SN of the first UMD PDU with SN >= VR(UX) 
    that has not been received;
-   reassemble RLC SDUs from any UMD PDUs with SN < updated VR(UR), 
    remove RLC headers when doing so and deliver the reassembled RLC SDUs 
    to upper layer in ascending order of the RLC SN if not delivered before;
-   if VR(UH) > VR(UR):
-   start t-Reordering;
-   set VR(UX) to VR(UH).
*/
/* SPR 5599_129124 Fix Start */
UInt32 rlcActionUmTReorderingExpire(RxUMEntity * rxUMEntity_p) 
/* SPR 5599_129124 Fix End */
{
    UInt32 ret = RLC_SUCCESS ;
    UInt16 rCount = 0 ;
    RingSS snStatus  = RING_SLOT_FREE ;    

    UmdPduRing *ring_p = &rxUMEntity_p->rxUmdPduRing ;
    if (!ring_p)
    {
        LOG_RLC_MSG(LTE_RLC_ACTION_UM_RO_TIMER_EXPIRY_ERR, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                0, 0, 0, 0, __func__,"");
                
        return RLC_FAILURE;
    }
    RlcVr UR = rxUMEntity_p->UR ;
    RlcVr UH = rxUMEntity_p->UH ;
    RlcVr UX = rxUMEntity_p->UX ;
    UInt16 modVal = rxUMEntity_p->modValForWrapRound;

    UInt32 wSize = rxUMEntity_p->wSize;
    SInt32 mBase = UH - wSize;

    RlcSnList snList = RLC_SN_LIST_INIT ;

    /* update VR(UR) to the SN of the first UMD PDU 
       with SN >= VR(UX) that has not been received; */
    rCount = UR;
    if ( UX > 1023 )
    {
        ltePanic("Invalid scenario UX [%d] \n", UX); 
      //  return RLC_TIMER_EXPIRY_INVALID;
    }
    
    for(;;)
    {
        /* SPR 3546 changes start */ 
        snStatus = GET_RING_SS_UMD_PDU_RN(ring_p, rCount, modVal);
         /* SPR 3546 changes end */ 
        if ( (isUmGTE( rCount , UX, mBase, modVal) )&& (snStatus == RING_SLOT_FREE) )
        {
            UR = rCount;
            /* Update the UR Value */
            rxUMEntity_p->UR = UR;

            LOG_RLC_MSG(LTE_RLC_ACTION_UM_RO_TIMER_EXPIRY, LOGDEBUG, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                    UR, 0, 0, 0, __func__,"");

            break ;
        }

        switch ( snStatus )
        {
            case RING_SLOT_IN_USE:
                {
                    SET_RING_SS_UMD_PDU_RN(ring_p, rCount, RING_SLOT_READY);
                    rlcSetSnList(&snList, rCount);

                    LOG_RLC_MSG(LTE_RLC_ACTION_UM_RO_TIMER_EXPIRY_1, LOGDEBUG, 
                            (RLC_RX), globalTTITickCount_g, __LINE__,
                            rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                            UR, snList.szUsed, 0, 0,
                            __func__,"");

                }
                break;

            case RING_SLOT_FREE:
                {
                    /* Dropping the SN due to RO Timer Expiry */  
                    LOG_RLC_MSG(LTE_RLC_ACTION_UM_RO_TIMER_EXPIRY_ERR_1, LOGDEBUG, 
                            (RLC_RX), globalTTITickCount_g, __LINE__,
                            rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                            UX, 0, 0, 0, __func__,"");
                }
                break;

            default:
                {
                    /* SPR 3546 changes start */ 
                    ltePanic("Not valid case rCount[%d] Ux [%d] slotStatus [%d] \n",
                            rCount, UX,  GET_RING_SS_UMD_PDU_RN(ring_p, rCount,
                            modVal));
                   /* SPR 3546 changes end */ 
                }
                break;


        }

        rCount = ( rCount + 1 ) & modVal;
    }
     /*reassemble RLC SDUs */
    if(snList.szUsed != 0)
    {
        rlcReassembleUmSdu(rxUMEntity_p, &snList);
    }

    if ( isUmGT(UH, UR, mBase, modVal))
    {
        rxUMEntity_p->isTimerRunning = RLC_TRUE ;
        startUMROTimer ( rxUMEntity_p);
        rxUMEntity_p->UX = UH;

        LOG_RLC_MSG(LTE_RLC_ACTION_UM_RO_TIMER_EXPIRY_2, LOGDEBUG, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                UR, UX, 0, 0, __func__,"");
    }
    else
    {
        rxUMEntity_p->isTimerRunning = RLC_FALSE ;
        
        /** SPR 7922 Changes Start **/
        rxUMEntity_p->UX = RLC_UNULL;
        /** SPR 7922 Changes End **/
    }
    return ret ;
}

/****************************************************************************
 * Function Name  : rlcDeliverUmSduToUpperLayerDirectly
 * Inputs         : rxUMEntity_p - pointer to RxUMEntity, 
 *		            rlc_buffer - Rlc Buffer pointer,
 *                  pktTimeStamp - Packet Time Stamp,
 *                  spsState - State of SPS,
 *                  packetTime
 * Outputs        : None
 * Returns        : None
 * Description    : delivers reassembled SDU(s) to upper layer (PDCP).
 ****************************************************************************/
/* SPR 5599_129124 Fix Start */
void rlcDeliverUmSduToUpperLayerDirectly( RxUMEntity * rxUMEntity_p, 
        /* SPR 5599_129124 Fix End */
        RlcBuffer *rlc_buffer
#ifndef UE_SIM_TESTING
        /* SPR 15909 fix start */
        , tickType_t pktTimeStamp,
        /* SPR 15909 fix end */
        UInt8 spsState
#endif
        /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        /* SPR 15909 fix start */
        , tickType_t packetTime
        /* SPR 15909 fix end */
#endif
        /* 32.425 Events code changes end*/
        )
{
    /* + PERF_CA GBL */
#if (defined(LTE_EMBMS_SUPPORTED) && defined(UE_SIM_TESTING))    
    /*SPR 22684 Fix3 Start*/ 
    if(RLC_EMBMS_MCCH_ENTITY == rxUMEntity_p->mchEntity)
    {
    /*SPR 22684 Fix3 Stop*/
        sendMCCHPduToRRC(rlc_buffer->data_p,rxUMEntity_p->ueIndex,0);
    }    
#else    
    InternalCellIndex cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(rxUMEntity_p->ueIndex);    
    /* - PERF_CA GBL */
    CLOCK_START_RLC_API(RLC_UE_DATA_IND);

#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
	if((SN_TERMINATED_SCG_SPLIT_BEARER ==  rxUMEntity_p->dcBearerType) && (rxUMEntity_p->lcId >= MIN_DRB_LC_ID))
	{
	
		pdcpDataInd( rxUMEntity_p->ueIndex,rxUMEntity_p->lcId,
	        	rlc_buffer->data_p,
		        rlc_buffer->szUsed) ;

	}
	else
#endif
    rlcUeDataInd ( rxUMEntity_p->ueIndex,
            rxUMEntity_p->lcId,
            rlc_buffer->data_p,
            rlc_buffer->szUsed,
            0, 
#ifndef UE_SIM_TESTING
            pktTimeStamp, 
            spsState 
#else
            0, 0
#endif
            /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
            ,packetTime
#endif
            /* 32.425 Events code changes end*/

            );
    CLOCK_END_RLC_PDCP_API(RLC_UE_DATA_IND);

    LOG_RLC_MSG(LTE_RLC_DELIVER_UM_SDU_DIRECTLY, LOGDEBUG, 
            (RLC_RX), globalTTITickCount_g, __LINE__,
            rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
            rlc_buffer->szUsed, 0, 0, 0, __func__, 
            "");

    LteRlcLCStats* lcStats_p =
        &(gRlcStats.pRlcStats->lteRlcUeStat[rxUMEntity_p->ueIndex].
                lcStats[rxUMEntity_p->lcId]);
    /* Update Stats */
	/* + PERF_CA GBL */
    lteRlcStatInfo_g[cellIndex].lteUMStat.rxUmdSduTxUL++;
    lteRlcStatInfo_g[cellIndex].lteUEStat.rlcUeStats[rxUMEntity_p->ueIndex].sduTransmittedToPdcp++;
	/* - PERF_CA GBL */
    lcStats_p->sduNumUl++;
    lcStats_p->sduBytesUl += rlc_buffer->szUsed;
    rlc_buffer->data_p = RLC_NULL;
#endif    
}

/****************************************************************************
 * Function Name  : rlcReassembleInCmpSDU
 * Inputs         : pSduIn - Pointer to UmdRawSduIn,
 *                  rxUMEntity_p - Pointer to RxUMEntity,
 *                  umdDfe_p - Pointer to UmdDFE,
 *                  sn - SN Field
 * Outputs        : None
 * Returns        : None
 * Description    : It delivers reassembled SDU(s) to upper layer (PDCP).
 ****************************************************************************/

static void rlcReassembleInCmpSDU( UmdRawSduIn *pSduIn,
                                          RxUMEntity * rxUMEntity_p,
                                          UmdDFE *umdDfe_p,
                                          UInt16 sn
                                        )
{
    RlcBuffer *first = &(pSduIn->pRawSdu);
    RlcBuffer *second = &(umdDfe_p->pDFE);
    /* + PERF_CA GBL */
    InternalCellIndex cellIndex = 0;    
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
    if(!rxUMEntity_p->mchEntity)
    {    
        cellIndex = RLC_FETCH_INTERNAL_AREA_CELL_INDEX(rxUMEntity_p->ueIndex);    
    }
    else/*SPR 22684 Fix3 Start*/ 
    {
    cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(rxUMEntity_p->ueIndex);    
    }
    /*SPR 22684 Fix3 Stop*/
    
#else
    cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(rxUMEntity_p->ueIndex);    
#endif    
    /* - PERF_CA GBL */

    switch (umdDfe_p->status)
    {
        case PDU_MAP_SDU_END:

            if(PNULL == (msgJoin(first->data_p, second->data_p)))
            {
                LOG_RLC_MSG(LTE_RLC_REASSEMBLE_IN_CMP_SDU_ERR, LOGWARNING, 
                        (RLC_RX), globalTTITickCount_g, 
                        rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                        sn, first->szUsed, second->szUsed,0, 0
                        , __func__, "");
                return;
            }

            LOG_RLC_MSG(LTE_RLC_REASSEMBLE_IN_CMP_SDU, LOGINFO, 
                    (RLC_RX), globalTTITickCount_g,__LINE__,
                    rxUMEntity_p->ueIndex, rxUMEntity_p->lcId, sn,
                    umdDfe_p->status,0,0,__func__, "");

            first->szUsed += second->szUsed;
            rlcDeliverUmSduToUpperLayerDirectly(rxUMEntity_p, first
#ifndef UE_SIM_TESTING
            , umdDfe_p->pktTimeStamp,
            umdDfe_p->spsState
#endif
            /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
            , umdDfe_p->packetTime
#endif
            /* 32.425 Events code changes end*/
            );
            msgFree(umdDfe_p->pDFE.data_p);
            /* SPR 20985 fix start */
            umdDfe_p->pDFE.data_p = PNULL;
            /* SPR 20985 fix end */

            break;

        case PDU_MAP_SDU_MID :

            if ( PNULL == (msgJoin(first->data_p,second->data_p)))
            {
                LOG_RLC_MSG(LTE_RLC_REASSEMBLE_IN_CMP_SDU_ERR_1, LOGWARNING, 
                        (RLC_RX), globalTTITickCount_g, __LINE__,
                        rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                        sn, umdDfe_p->status,0, 0
                        , __func__, "");
                return;
            }

            LOG_RLC_MSG(LTE_RLC_REASSEMBLE_IN_CMP_SDU_1, LOGINFO, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    rxUMEntity_p->ueIndex, rxUMEntity_p->lcId, sn,
                    umdDfe_p->status,0,0, __func__, "");
            
            first->szUsed += second->szUsed;

            pSduIn->pRawSdu.size = first->size;
            pSduIn->pRawSdu.szUsed = first->szUsed;
            pSduIn->pRawSdu.data_p = first->data_p;
            pSduIn->sn = sn ;

            msgFree(umdDfe_p->pDFE.data_p);
            /* SPR 20985 fix start */
            umdDfe_p->pDFE.data_p = PNULL;
            /* SPR 20985 fix end */

            break;

        case PDU_MAP_SDU_FULL:

    		/* + PERF_CA GBL */
            lteRlcStatInfo_g[cellIndex].lteUMStat.rxIncompleteUmdSduDiscarded++;
    		/* - PERF_CA GBL */
            msgFree(pSduIn->pRawSdu.data_p);
            pSduIn->pRawSdu.data_p= RLC_NULL ;
            pSduIn->pRawSdu.size = 0;
            pSduIn->pRawSdu.szUsed = 0;

            LOG_RLC_MSG(LTE_RLC_REASSEMBLE_IN_CMP_SDU_2, LOGDEBUG, 
                    (RLC_RX), globalTTITickCount_g, 
                    rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                    sn, second->szUsed, umdDfe_p->status,0, 0, __func__, "fullSDU");
            
            rlcDeliverUmSduToUpperLayerDirectly( rxUMEntity_p, &umdDfe_p->pDFE
#ifndef UE_SIM_TESTING
            , umdDfe_p->pktTimeStamp,
            umdDfe_p->spsState
#endif
            /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
            , umdDfe_p->packetTime
#endif
            /* 32.425 Events code changes end*/
            );

            break;

        default :

    		/* + PERF_CA GBL */
            lteRlcStatInfo_g[cellIndex].lteUMStat.rxIncompleteUmdSduDiscarded++;
    		/* - PERF_CA GBL */
            msgFree(pSduIn->pRawSdu.data_p);
            /* SPR 20985 fix start */
            pSduIn->pRawSdu.data_p = PNULL;
            /* SPR 20985 fix end */
            MOVE_BUFF_CONTENT(&(pSduIn->pRawSdu),&(umdDfe_p->pDFE));
            pSduIn->sn = sn ;

            LOG_RLC_MSG(LTE_RLC_REASSEMBLE_IN_CMP_SDU_ERR_2, LOGWARNING, 
                    (RLC_RX), globalTTITickCount_g, 
                    rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                    sn, second->szUsed, umdDfe_p->status, 0,0,
                     __func__, "");

            break;

    }

}

/****************************************************************************
 * Function Name  : deliverUmSduToPDCP
 * Inputs         : pSduIn - Pointer to UmdRawSduIn,
 *                  rxUMEntity_p - Pointer to RxUMEntity,
 *                  umdDfe_p - Pointer to UmdDFE,
 *                  sn - SN Field
 * Outputs        : None
 * Returns        : None
 * Description    : IT delivers SDU(s) to upper layer (PDCP).
 ****************************************************************************/

static void deliverUmSduToPDCP( UmdDFE *umdDfe_p,
                                       RxUMEntity * rxUMEntity_p,
                                       UmdRawSduIn *pSduIn,
                                       UInt16 sn
                                     )
{
    switch ( umdDfe_p->status )
    {
        case PDU_MAP_SDU_FULL:

            LOG_RLC_MSG( LTE_RLC_DELIVER_UM_SDU_TO_PDCP, LOGDEBUG, 
                    (RLC_RX), globalTTITickCount_g, 
                    rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                     umdDfe_p->status, sn, umdDfe_p->pDFE.szUsed,
                     0,0, __func__, "");
            
            rlcDeliverUmSduToUpperLayerDirectly(rxUMEntity_p, &umdDfe_p->pDFE
#ifndef UE_SIM_TESTING
                    , umdDfe_p->pktTimeStamp,
                    umdDfe_p->spsState
#endif
                    /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                    , umdDfe_p->packetTime
#endif
                    /* 32.425 Events code changes end*/
                    );

            break;

        case PDU_MAP_SDU_START:

            LOG_RLC_MSG( LTE_RLC_DELIVER_UM_SDU_TO_PDCP_1, LOGDEBUG, 
                    (RLC_RX), globalTTITickCount_g, 
                    rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                     umdDfe_p->status, sn, umdDfe_p->pDFE.szUsed,
                     0,0, __func__, "");
            
            MOVE_BUFF_CONTENT(&(pSduIn->pRawSdu), &(umdDfe_p->pDFE));
            pSduIn->sn = sn ;

            break;

        default:

            LOG_RLC_MSG( LTE_RLC_DELIVER_UM_SDU_TO_PDCP_ERR, LOGWARNING, 
                    (RLC_RX), globalTTITickCount_g,
                    rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                     umdDfe_p->status, sn, umdDfe_p->pDFE.szUsed,
                     0,0, __func__, "");
            
            msgFree(umdDfe_p->pDFE.data_p); 
            /* SPR 20985 fix start */
            umdDfe_p->pDFE.data_p = PNULL;
            /* SPR 20985 fix end */

            break;
    }

}

/****************************************************************************
 * Function Name  : handleRlcReassembleInCmpSDU
 * Inputs         : pSduIn - Pointer to UmdRawSduIn,
 *                  sn - SN Field,
 *                  umdDfe_p - Pointer to UmdDFE,
 *                  rxUMEntity_p - Pointer to RxUMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : This functon handle the Reassembling of SDU 
                    or sends the UM SDU to pdcp
 ****************************************************************************/

static void handleRlcReassembleInCmpSDU (  UmdRawSduIn *pSduIn,
                                                  UInt16 sn,
                                                  UmdDFE *umdDfe_p,
                                                  RxUMEntity * rxUMEntity_p
                                               )
{
    UInt16 wSize  = rxUMEntity_p->wSize;
    UInt16 modVal = rxUMEntity_p->modValForWrapRound;
    UInt16 UH     = rxUMEntity_p->UH;
    SInt32 mBase  = UH - wSize;
    /* + PERF_CA GBL */
    InternalCellIndex cellIndex = 0;    
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
    if(!rxUMEntity_p->mchEntity)
    {    
        cellIndex = RLC_FETCH_INTERNAL_AREA_CELL_INDEX(rxUMEntity_p->ueIndex);    
    }
    else/*SPR 22684 Fix3 Start*/ 
    {
    cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(rxUMEntity_p->ueIndex);    
    }
    /*SPR 22684 Fix3 Stop*/
#else
    cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(rxUMEntity_p->ueIndex);    
#endif    
    /* - PERF_CA GBL */
    /* make it the head of UmdRawSduQ if SN is in sequence */
    if ( !(isUmEQ ( sn, ((pSduIn->sn + 1) & modVal), mBase, modVal) ))
    {
        /* Recv Sequence number is not equal
           to previous incomplete sdu sequence*/

        /* discard this portion of out of sequence PDU.. it is loss of SDU */
		/* + PERF_CA GBL */
        lteRlcStatInfo_g[cellIndex].lteUMStat.rxIncompleteUmdSduDiscarded++;
		/* - PERF_CA GBL */

        msgFree(pSduIn->pRawSdu.data_p);
        pSduIn->pRawSdu.data_p = RLC_NULL ;
        pSduIn->pRawSdu.size   = 0;
        pSduIn->pRawSdu.szUsed = 0;
        
        LOG_RLC_MSG( LTE_RLC_HANDLE_RLC_REASSEMBLE_SDU, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g,
                rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                sn, umdDfe_p->status,  umdDfe_p->pDFE.szUsed,
                0,0, __func__, "");

        deliverUmSduToPDCP(umdDfe_p, rxUMEntity_p, pSduIn, sn);
    }
    else
    {  /* Both Sequence Numbers are equal */
        LOG_RLC_MSG( LTE_RLC_HANDLE_RLC_REASSEMBLE_SDU_1, LOGINFO, 
                (RLC_RX), globalTTITickCount_g, 
                rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                sn, umdDfe_p->status,  umdDfe_p->pDFE.szUsed,
                0, 0,__func__, "");
        
        rlcReassembleInCmpSDU ( pSduIn, rxUMEntity_p, umdDfe_p, sn);

    }

}

/****************************************************************************
 * Function Name  : rlcTraverseAndHandleUmdDfeQ
 * Inputs         : umdPdu_p - Pointer to UmdPdu,
 *                  rxUMEntity_p - Pointer to RxUMEntity,
 *                  seqNum - Sequence Number
 * Outputs        : None
 * Returns        : None
 * Description    : This function handle the  UM DFE queue and reasseble it 
 *                  or deliver it to the PDCP 
 ****************************************************************************/

static void rlcTraverseAndHandleUmdDfeQ(  UmdPdu *umdPdu_p,
                                                 RxUMEntity * rxUMEntity_p,
                                                 UInt16 seqNum
                                              )
{
    UmdDFE *umdDfe_p    = RLC_NULL ;
    UmdRawSduIn *pSduIn = &rxUMEntity_p->rxSduIn ;

    DE_QUEUE_UMDDFE_Q((&umdPdu_p->df), umdDfe_p);

    /* For testing purpose only */
    if ( umdPdu_p->h.sn != seqNum )
    {
        ltePanic("Invalid sequence number");
    }

    while ( umdDfe_p != RLC_NULL )
    {
        if ( RLC_NULL != pSduIn->pRawSdu.data_p)
        {
            handleRlcReassembleInCmpSDU( pSduIn,
                    seqNum, 
                    umdDfe_p,
                    rxUMEntity_p );
        }
        else
        {
            deliverUmSduToPDCP (
                    umdDfe_p,
                    rxUMEntity_p,
                    pSduIn,
                    umdPdu_p->h.sn
                    );
        }
        freeMemPool(umdDfe_p); 
        DE_QUEUE_UMDDFE_Q((&umdPdu_p->df), umdDfe_p);
    }

}

/****************************************************************************
 * Function Name  : rlcReassembleUmSdu
 * Inputs         : rxUMEntity_p - a pointer to RxUMEntity,
 *                  snList - a pointer to RlcSnList and this list contains the
 *                           SN(s) of UMD PDUs to be reassembled
 * Outputs        : None
 * Returns        : RLC_SUCCESS if case is success else RLC_FAILURE
 * Description    : reassembles all UMD PDU(s) with SN in @snList.
 ****************************************************************************/
UInt32 rlcReassembleUmSdu( RxUMEntity * rxUMEntity_p,
                           RlcSnList *snList
                         )
{
    UInt32 ret       = RLC_SUCCESS ;
    UInt32 count     = 0 ;
    UInt16 ringSN    = 0;
    UmdPdu *umdPdu_p = RLC_NULL ;

    UmdPduRing *ring_p = &rxUMEntity_p->rxUmdPduRing ;

    LOG_RLC_MSG( LTE_RLC_HANDLE_RLC_REASSEMBLE_UM_SDU, LOGDEBUG, 
            (RLC_RX), globalTTITickCount_g, __LINE__,
            rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
            snList->szUsed, 0,  0,
            0, __func__, "");

    for(; count < snList->szUsed; count++)
    {
        ringSN   = snList->snList[count];
        umdPdu_p = GET_RING_DATA_UMD_PDU_RN(ring_p, ringSN);

        rlcTraverseAndHandleUmdDfeQ ( umdPdu_p, rxUMEntity_p, ringSN); 


        SET_RING_SS_UMD_PDU_RN(ring_p, ringSN, RING_SLOT_FREE) ;
    } 
    /* method to deliver complete SDU to upper layer */
    RESET_SN_LIST(snList);
    return ret ; 
}





