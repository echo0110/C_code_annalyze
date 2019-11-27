/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcUmTxOperation.c,v 1.2.12.2.2.1 2010/09/21 15:50:29 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *  This file describes funtionality when delivering a new UMD PDU to lower layer
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcUmTxOperation.c,v $
 *
 ****************************************************************************/


/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcMacInterface.h"
#include "lteRlcPdcpInterface.h"
#include "lteRlcUmTxOperation.h"
#include "lteRlcUeContext.h"
#include "logging.h"
#include "lteRlcStat.h"
#include "lteCommonStatsManager.h"
#include "atomic-ops.h"
#include "lteRlcErrorCode.h"
#include "lteRlcOamInterface.h"

#include "ltePerfStats.h"
/****************************************************************************
 * Varibale Definition
 ****************************************************************************/
/****************************************************************************
 * External Variable Includes
 ****************************************************************************/
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */

/****************************************************************************
 * Function Name  : rlcReceiveUmSduFromUpperLayer
 * Inputs         : txUMEntity_p - a pointer to TxUMEntity,
 *                  pUmdSdu - a pointer to  UmdRawSdu
 * Outputs        : None
 * Returns        : RLC_SUCCESS if case of success else RLC_FAILURE
 * Description    : This function describes actions when UMD SDU referd by
 *                  @pUmdSdu is recieved for UM Entity @txUMEntity_p.
 ****************************************************************************/
inline UInt32 rlcReceiveUmSduFromUpperLayer( TxUMEntity * txUMEntity_p, 
                                             RlcUeDataReq *pUmdSdu
                                           ) 
{
    UInt32 coutSDU      = 0;
    UInt32 ret          = RLC_SUCCESS;
/* + SPR 20690 */
    LCQueueLoad lcQLoad = {0};
    /*SPR 19433 +*/
#ifndef UE_SIM_TESTING    
    RlcUeDataReq * UmdRawSdu_p = RLC_NULL ;
#endif    
/* - SPR 20690 */
    /*SPR 19433 -*/
    /* SPR 12611 Start */
    UInt16   pUmdSduSize = pUmdSdu->size;
    /* SPR 12611 End */
    UmdRawSduQ *umdRawSduQ_p = &txUMEntity_p->txSDUQueue ;
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
    LteUeRlcDLPerfStats *lteUeRlcDLPerfStats = (LteUeRlcDLPerfStats*)\
	    &((gRlcUePerfStats_p[txUMEntity_p->ueIndex]).lteUeRlcDLPerfStats);
#endif
    InternalCellIndex cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(txUMEntity_p->ueIndex);    
    
    LteRlcLCStats* lcStats_p =
            &(gRlcStats.pRlcStats->lteRlcUeStat[txUMEntity_p->ueIndex].
                      lcStats[txUMEntity_p->lcId]);
    /** SPR 6961 Changes Start **/
    coutSDU = QUEUE_COUNT_UMD_RAW_SDU_Q(*umdRawSduQ_p);
    /** SPR 6961 Changes End **/
#ifdef UE_SIM_TESTING   
    /* + SPR 19808 */
    if( coutSDU >= MAX_UM_RAW_SDU_Q_SIZE ) 
    {
#else
    if( (coutSDU >= MAX_UM_RAW_SDU_Q_SIZE ) || 
        (txUMEntity_p->txQueueSize >= txUMEntity_p->rlcQueueSize) )
            /* - SPR 19808 */
    {
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

        rlcQStatusBuffer_p[rlcStatusBufferCount].transId = pUmdSdu->transId;
        /* SPR 7914 changes start */
        rlcQStatusBuffer_p[rlcStatusBufferCount].deliveryStatus = RLC_PDCP_SDU_ENQUEUE_FAILURE;
        /* SPR 7914 changes end */
        rlcStatusBufferCount++;
        CLOCK_START_RLC_API(RLC_DLVRY_STATUS_IND);
        rlcDeliveryStatusInd(pUmdSdu->ueIndex, pUmdSdu->lcId, rlcStatusBufferCount,
                rlcQStatusBuffer_p);
        CLOCK_END_RLC_PDCP_API(RLC_DLVRY_STATUS_IND);
#endif
        /* SPR 4497 Changes End */


        msgFree(pUmdSdu->data_p);
        pUmdSdu->data_p = PNULL;
        freeMemPool(pUmdSdu);

        lteRlcStatInfo_g[cellIndex].lteUMStat.rxUmdSduDiscarded++;
        lcStats_p->numPktsDropped++;
        LOG_RLC_MSG ( LTE_RLC_RX_UM_SDU_ERR, LOGWARNING, 
                RLC_TX, globalTTITickCount_g, __LINE__,txUMEntity_p->ueIndex , 
                txUMEntity_p->lcId, coutSDU, 0, 0, 0, __func__, "");
        ret =  LTE_RLC_UM_MAX_SDU_REVD;
        return ret;
    }
    else 
    {
        /*SPR 16936 fix*/
        if ( coutSDU == 0)
        {
            txUMEntity_p->headPktTS = pUmdSdu->pktArrTime;
        }
        /*SPR 16936 fix*/
        /* + SPR 12349 Changes */
        /*Enqueue in the RLC SDU Q*/
        if (!EN_QUEUE_UMD_SDU_Q(umdRawSduQ_p,pUmdSdu))
        {
            msgFree(pUmdSdu->data_p);
            pUmdSdu->data_p = PNULL;
            freeMemPool(pUmdSdu);
            return RLC_FAILURE; 
        }
        atomicAddInt(&txUMEntity_p->txQueueSize, pUmdSdu->size);

        coutSDU += 1;

        /* Update the stats */
        lteRlcStatInfo_g[cellIndex].lteUMStat.umdSduReceivedFromPdcp++;
        lteRlcStatInfo_g[cellIndex].lteUEStat.rlcUeStats[txUMEntity_p->ueIndex].
            sduReceivedFromPdcp++;
        lcStats_p->sduNumDl++;
        /* SPR 12611 Start */
        lcStats_p->sduBytesDl += pUmdSduSize;
        /* SPR 12611 End */

#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
        /*Update KPI Counter for total SDU recieved from PDCP for UM Mode */
        lteUeRlcDLPerfStats->totalDlRLCUMSduTransmitted[txUMEntity_p->lcId - 3]++;
#endif
    }

    {
        rlcUMFetchQLoad( txUMEntity_p, &lcQLoad);
        /*SPR 16936 fix*/
        lcQLoad.headPktTS = txUMEntity_p->headPktTS;
        /*SPR 16936 fix*/

        lcQLoad.lcId = txUMEntity_p->lcId ;
        /*******************************QoS ********************************/    
        lcQLoad.scheduledDataBytes = 0;
        lcQLoad.retxnPduSize       = 0;
        lcQLoad.ctrlQueueLoad      = 0;
        lcQLoad.ctrlFlag           = RLC_NORMAL_DATA;

        /*******************************QoS ********************************/   
#ifndef UE_SIM_TESTING 
        if( LTE_TRUE == txUMEntity_p->isSpsLc )
        {
            UmdRawSdu_p = (RlcUeDataReq *)(umdRawSduQ_p->pQ->ring[umdRawSduQ_p->pQ->head].data);
            if(UmdRawSdu_p)
            {
                lcQLoad.voiceSilenceType = UmdRawSdu_p->voiceSilenceType;
                lcQLoad.ipPktType = UmdRawSdu_p->ipPktType;
                lcQLoad.headPktQueueLoad = UmdRawSdu_p->size;
                lcQLoad.avgRunningSduSize = UmdRawSdu_p->avgSduSize;
            }

            lcQLoad.numOfSdu = QUEUE_COUNT_UMD_RAW_SDU_Q(*umdRawSduQ_p);
            lcQLoad.RLCPduInd = RLC_NO_IND;

            txUMEntity_p->prevAvgRunningSduSize = lcQLoad.avgRunningSduSize;
        }
#endif
        macUpdateBufferOccupancyReq( txUMEntity_p->ueIndex,
                1 /*lcCount*/, 
                &lcQLoad, 
                RLC_BO_TYPE_NEW,
                rlcUEContextList_g[ txUMEntity_p->ueIndex]->cellIndex
                );

        LOG_RLC_MSG ( LTE_RLC_RX_UM_SDU, LOGINFO, 
                RLC_TX, globalTTITickCount_g, __LINE__,
                txUMEntity_p->ueIndex, txUMEntity_p->lcId, lcQLoad.lcQueueLoad,
                coutSDU + 1, txUMEntity_p->txQueueSize,  
                pUmdSduSize,
                __func__, "");
    }
    return ret;
}

/****************************************************************************
 * Function Name  : rlcReceiveUmSduDiscardFromUpperLayer 
 * Inputs         : txUMEntity_p - a pointer to TxUMEntity,
 *                  transId - sdu tranId
 * Outputs        : None
 * Returns        : RLC_SUCCESS if case of success else RLC_FAILURE
 * Description    : It describes actions when UMD SDU referd by @pUmdSdu is
 *                  recieved for UM Entity @txUMEntity_p.
 ****************************************************************************/
inline UInt32 rlcReceiveUmSduDiscardFromUpperLayer( TxUMEntity * txUMEntity_p,
                                                  void * transId
                                                  )
{
    UInt32 ret     = RLC_SUCCESS ;
    UInt32 sduSize = 0;
    
    LCQueueLoad lcQLoad = {0};
    RlcUeDataReq * UmdRawSdu_p = RLC_NULL ;
    UmdRawSduQ *umdRawSduQ_p   = &txUMEntity_p->txSDUQueue ;

    sduSize = rlcSearchNdeleteUmdSduFromQ(umdRawSduQ_p, transId);

    if ( !sduSize)
    {
        /* SPR 14622 fix start */
        /* code removed */
        /* SPR 14622 fix end */
        return RLC_RX_UM_NOT_FOUND_SDU;
    }
    atomicSubtractInt(&txUMEntity_p->txQueueSize, sduSize);

    gRlcStats.pRlcStats->lteRlcUeStat[txUMEntity_p->ueIndex].
        lcStats[txUMEntity_p->lcId].numPktsDiscarded ++;

    rlcUMFetchQLoad( txUMEntity_p, &lcQLoad);

    lcQLoad.lcId               = txUMEntity_p->lcId ;
/*******************************QoS ********************************/    
    lcQLoad.scheduledDataBytes = 0;
    lcQLoad.retxnPduSize       = 0;
    lcQLoad.ctrlQueueLoad      = 0;
    lcQLoad.ctrlFlag           = RLC_NORMAL_DATA;

    UmdRawSdu_p = (RlcUeDataReq *)(umdRawSduQ_p->pQ->ring[umdRawSduQ_p->pQ->head].data);
    if(UmdRawSdu_p)
    {
        lcQLoad.headPktTS = UmdRawSdu_p->pktArrTime;
    }
    else
    {
        lcQLoad.headPktTS = 0;  
    }
/*****************************************************************************/

    /* notify MAC layer of queue load */
    macUpdateBufferOccupancyReq( txUMEntity_p->ueIndex,
            1 /*lcCount*/, 
            &lcQLoad, 
            /* SPR 14622 fix start */
            RLC_BO_TYPE_NEW, 
            /* SPR 14622 fix end */
            rlcUEContextList_g[ txUMEntity_p->ueIndex]->cellIndex
            );

/* SPR 20636 Changes Start */
    LOG_RLC_MSG ( LTE_RLC_RX_UM_SDU_DISCARD, LOGINFO, 
            RLC_TX, globalTTITickCount_g,  txUMEntity_p->ueIndex,
            txUMEntity_p->lcId, (ADDR)transId, sduSize, lcQLoad.lcQueueLoad,
             0,0, __func__, "");
/* SPR 20636 Changes End */

/* SPR 4497 Changes Start */
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
    /* SPR 14622 fix start */
    rlcQStatusBuffer_p[rlcStatusBufferCount].deliveryStatus = RLC_DISCARD_SUCCESS;
    /* SPR 14622 fix end */
    rlcStatusBufferCount++;
    CLOCK_START_RLC_API(RLC_DLVRY_STATUS_IND);
    rlcDeliveryStatusInd(txUMEntity_p->ueIndex, txUMEntity_p->lcId, rlcStatusBufferCount,
          rlcQStatusBuffer_p);
    CLOCK_END_RLC_PDCP_API(RLC_DLVRY_STATUS_IND);
#endif



    return ret ;
}

