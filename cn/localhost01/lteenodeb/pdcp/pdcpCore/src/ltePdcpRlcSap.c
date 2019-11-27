/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpRlcSap.c,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:16 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : Interface implementation for interaction beetwen the 
 *                     PDCP layer and RLC layer (RLC SAP).
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpRlcSap.c,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:16  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:11:47  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.9.2.2  2009/07/07 11:37:15  gur19140
 * *** empty log message ***
 *
 * Revision 1.9.2.1  2009/06/09 11:33:51  gur19140
 * *** empty log message ***
 *
 * Revision 1.8  2009/05/21 07:10:46  gur19140
 * memory leak fixes
 *
 * Revision 1.7  2009/05/20 13:38:43  gur20548
 * header file name changed
 *
 * Revision 1.6  2009/05/15 09:10:14  gur19140
 * bug fixes
 *
 * Revision 1.5  2009/05/15 06:44:47  gur19836
 * CR Comments Incorporated
 *
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "ltePdcpLteFramework.h"

#include "ltePdcpLteDefs.h"
#include "ltePdcpRlcSap.h"
#include "ltePdcpRxQueues.h"
#include "ltePdcpTxQueues.h"
#include "ltePdcpContext.h"
#include "logging.h"
#include "alarm.h"
#ifdef L2_FAST_PATH
#include "ltePdcpDataTransferRx.h"
#endif


/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/
/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* + Layer2 NON CA Changes start */
#ifndef UE_SIM_TESTING
/* SPR 15909 fix start */
extern tickType_t pdcpGlobalTTITickCount_g[MAX_NUM_CELL];
/* SPR 15909 fix end */
#endif
/* - Layer2 NON CA Changes */
/* SPR 15909 fix start */
extern tickType_t pdcpCurrentTime_g ;
/* SPR 15909 fix end */

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
 /*PdcpAdapter Thread Id*/
ULong32  pdcpAdapterExecutionSchedulerThId_g;


#ifdef LAYER2_DLTG_THREAD
ULong32  dlTgExecutionSchedulerThId_g;
#endif
#ifdef PDCP_THROUGHPUT_STATS
UInt32 pdcpDataRx = 0;
UInt32 pdcpDataRxPkt = 0;
UInt32 pdcpDataTx = 0;
/* Add Num Pkt Stat Start */
UInt32 pdcpPktTx = 0;
#endif
/* Add Num Pkt Stat End */
/*SPR 21551 Fix start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
extern void lteMacUpdateKpiStatsIPThpUlOnDataArrival( UInt16 ueIndex , UInt16 lcId, UInt32 size, tickType_t pktTimeStamp);
#endif
/*SPR 21551 Fix End*/


/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : rlcProcessUeDataInd 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  data_p  - pointer to PDCP PDU data in Zero-copy buffering 
 *                            format.
 *                  size - size of data
 *                  packetForReEstablishment -
 *                  headPktTS -
 *                  macState -
 *                  packetTime -
 * Outputs        : None.
 * Returns        : UInt32
 * Description    : This function receives data indication (PDCP PDU) from 
 *                  the RLC layer and stores it into the PDCP RX queue.
 *                  It's asynchronous function.
 ****************************************************************************/
UInt32 rlcProcessUeDataInd( UInt16 ueIndex, UInt16 lcId, UInt8* data_p, 
    /* SPR 15909 fix start */
        UInt16 size, UInt8 packetForReEstablishment, tickType_t headPktTS, 
    UInt8 macState
    /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    ,tickType_t packetTime
    /* SPR 15909 fix end */
#endif
    /* 32.425 Events code changes end*/
    )
{
    LP_PdcpDataInd dataInd_p  = PNULL;
    LP_PdcpEnityContext ctx_p = PNULL;
    UInt32  dataSize = 0;

    /* Check data pointer */
    if ( PNULL == data_p )
    {
        LOG_PDCP_MSG (PDCP_DATA_POINTER_NULL, LOGFATAL, PDCP_RX, pdcpCurrentTime_g,
                 __LINE__, ueIndex, lcId,0,0, 0,0, __func__,"RLC gives NULL data" );

        return 0;
    }
    /*SPR 21551 Fix Start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    lteMacUpdateKpiStatsIPThpUlOnDataArrival( ueIndex,(lcId - 3),size,packetTime);
#endif
    /*SPR 21551 Fix End*/

    do {
        /* Check if PDCP is initialized */
        if ( !pdcpContextIsInitialized() )
        {
            LOG_PDCP_MSG( PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RX, 
                     pdcpCurrentTime_g, __LINE__, 0,0,0,0, 0,0, __func__,"");
            break;
        }
        /* check message size */
        dataSize = msgSize( data_p, PNULL );
        if ( dataSize != size )
        {
            LOG_PDCP_MSG( PDCP_INVALID_DATA_SIZE, LOGWARNING, PDCP_RX, 
                     pdcpCurrentTime_g, __LINE__, dataSize,0, ueIndex, lcId,
                     0,0, __func__,"");
            break;
        }

        /* Check ueIndex and lcId parameters */
        /*CSR 40799 chg start*/
        if ( (MAX_UE_SUPPORTED <= ueIndex) || 
        /*CSR 40799 chg end*/
                (LTE_SRB_LC_ID_LOW_BOUND > lcId) ||
                (LTE_DRB_LC_ID_HIGH_BOUND < lcId) ) 
        {
            LOG_PDCP_MSG( PDCP_INVALID_LC, LOGWARNING, PDCP_RX, pdcpCurrentTime_g,
                     __LINE__, lcId, ueIndex, 0,0, 0,0, __func__,"");
            break;
        }

        ctx_p = pdcpGetEntityContext(ueIndex,lcId,PDCP_ENTITY_RB_RX);
        if ( PNULL == ctx_p )
        {
            LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_RX, LOGWARNING, PDCP_RX,
                     pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 0,0, 0,0, 
                     __func__,"");
            break;
        }

        /* Allocate and fill data request structure */
        dataInd_p = pdcpAllocateRxDataInd( ueIndex, lcId, PDCP_INVALID_SN,
                                           dataSize, packetForReEstablishment,
                                           headPktTS, macState
                                           /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                                           ,packetTime
#endif
                                           /* 32.425 Events code changes end*/
                );
        if ( PNULL == dataInd_p )
        {
            LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RX, 
                     pdcpCurrentTime_g,__LINE__, sizeof(PdcpDataInd), ueIndex,
                     lcId, 0,0,0,__func__,"");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            ltePanic("MemAlloc fails for PdcpDataInd\n");
            break;
        }    
        dataInd_p->data_p = data_p;
        /* put data indication to PDCP RX queue */
        LOG_UT( PDCP_PDU_FROM_RLC_TO_PDCP, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
                 __LINE__, dataInd_p->ueIndex, dataInd_p->lcId, 
                 0, 0, 0,0, __func__,"");
        ENQUEUE_PDCP_RX_DATA_IND_Q(ctx_p, dataInd_p);
        return 1;
    } while (0);
    /* abnormal termination */
    if (PNULL != data_p)
    {
        msgFree(data_p);
    }
    return 1;
}

/****************************************************************************
 * Function Name  : rlcUeDataInd 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  data_p  - pointer to PDCP PDU data in Zero-copy buffering 
 *                            format.
 *                  size - size of data
 *                  packetForReEstablishment -
 *                  headPktTS -
 *                  macState -
 *                  packetTime -
 * Outputs        : None.
 * Returns        : UInt32
 * Description    : This function receives data indication (PDCP PDU) from 
 *                  the RLC layer and stores it into the PDCP RX queue.
 *                  It's asynchronous function.
 ****************************************************************************/
extern UInt64 uplinkRlcEgressDataSize;
extern UInt64 uplinkRlcEgressDataNum;
UInt32 rlcUeDataInd( UInt16 ueIndex, UInt16 lcId, UInt8* data_p, UInt16 size, 
                    /* SPR 15909 fix start */             
                    UInt8 packetForReEstablishment, tickType_t headPktTS, 
                     UInt8 macState
                     /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                     ,tickType_t packetTime
                     /* SPR 15909 fix end */
#endif
                     /* 32.425 Events code changes end*/
        )
{
	uplinkRlcEgressDataSize += size;
	uplinkRlcEgressDataNum++;

#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
    LP_PdcpRlcDataInd pdcpRlcDataInd_p = PNULL;
    pdcpRlcDataInd_p =
            (LP_PdcpRlcDataInd)getMemFromPool(sizeof(PdcpRlcDataInd) ,PNULL);

    if ( PNULL == pdcpRlcDataInd_p )
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RX, 
               pdcpCurrentTime_g,__LINE__, sizeof(PdcpDataInd), ueIndex,
               lcId, 0,0,0,__func__,"");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("MemAlloc fails for PdcpDataInd\n");
        return 0;
    }
    pdcpRlcDataInd_p->ueIndex=ueIndex;
    pdcpRlcDataInd_p->lcId = lcId;
    pdcpRlcDataInd_p->data_p = data_p ;   
    pdcpRlcDataInd_p->size = size;
    pdcpRlcDataInd_p->packetForReEstablishment = packetForReEstablishment;
    pdcpRlcDataInd_p->headPktTS = headPktTS;
    pdcpRlcDataInd_p->macState = macState;
    /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    pdcpRlcDataInd_p->pktTimeStamp = packetTime ;
#endif
    /* 32.425 Events code changes end*/
    /* + SPR 9253 */
    if (!ENQUEUE_PDCP_RLC_DATA_IND_Q(pdcpRlcDataInd_p))
    {
        msgFree(data_p);
        data_p = PNULL;
        freeMemPool(pdcpRlcDataInd_p);
    }
    /* - SPR 9253 */

    return 1;
#else
    return(rlcProcessUeDataInd( ueIndex, lcId, data_p, size, 
                packetForReEstablishment, headPktTS, macState
                /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                , packetTime
#endif
                /* 32.425 Events code changes end*/
                ));
#endif

}

#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
/****************************************************************************
 * Function Name  : rlcHandleUeDataInd 
 * Inputs         : cQCount
 * Outputs        : None.
 * Returns        : 0/1
 * Description    : This function receives data indication (PDCP PDU) from 
 *                  the RLC layer and stores it into the PDCP RX queue.
 *                  It's asynchronous function.
 ****************************************************************************/
UInt32 rlcHandleUeDataInd(UInt32 cQCount)
{
    UInt16  ueIndex;
    UInt16  lcId;
    UInt8 * data_p;
    UInt16  size;
    UInt8   packetForReEstablishment;

    LP_PdcpRlcDataInd pdcpDataInd_p = PNULL;
    while(cQCount > 0)
    {
        DEQUEUE_PDCP_RLC_DATA_IND_Q(pdcpDataInd_p);

        if( pdcpDataInd_p == PNULL)
        {
            return 0;
        }
        ueIndex = pdcpDataInd_p->ueIndex;
        lcId = pdcpDataInd_p->lcId;
        data_p = pdcpDataInd_p->data_p;
        size = pdcpDataInd_p->size;
        packetForReEstablishment = pdcpDataInd_p->packetForReEstablishment;
        rlcProcessUeDataInd( ueIndex, lcId, data_p, size, 
                             packetForReEstablishment, 
                             pdcpDataInd_p->headPktTS, 
                             pdcpDataInd_p->macState
/* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                             ,pdcpDataInd_p->pktTimeStamp
#endif
/* 32.425 Events code changes end*/
                );
        freeMemPool(pdcpDataInd_p);
        pdcpDataInd_p = PNULL;
        cQCount --;
    }
    return 1;
}
#endif

/****************************************************************************
 * Function Name  : rlcDeliveryStatusInd 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  count -
 *                  deliveryStatus - the delivery status.
 * Outputs        : None.
 * Returns        : 0/1
 * Description    : This function receives data delivery indication (PDCP PDU)
 *                  from the RLC layer and stores it into the PDCP delivery
 *                  indication TX queue. 
 ****************************************************************************/
UInt32  rlcProcessDeliveryStatusInd(
        UInt16  ueIndex,
        UInt16  lcId,
        UInt32  count,
        RlcStatusBuffer* deliveryStatus_p)
{
    LP_PdcpDeliveryInd deliveryInd = PNULL;
    /* We receive PDU delivery indication from the RLC layer, so 
       store it to the queue for future processing */
    /** CA 2 Core Changes Start **/
#ifndef UE_SIM_TESTING
    LP_PdcpUeContext    ueCtx_p = PNULL;
#endif
    /** CA 2 Core Changes End **/
    /* Check ueIndex and lcId parameters */
    /*CSR 40799 chg start*/
    if ( (MAX_UE_SUPPORTED <= ueIndex) || 
    /*CSR 40799 chg end*/
            (LTE_SRB_LC_ID_LOW_BOUND > lcId) ||
            (LTE_DRB_LC_ID_HIGH_BOUND < lcId) ) 
    {
        LOG_PDCP_MSG( PDCP_INVALID_LC, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                __LINE__, lcId, ueIndex,0,0, 0,0, __func__,"");
        /* SPR 4440 Fix start */
        freeMemPool(deliveryStatus_p);
        /* SPR 4440 Fix End */
        return 0;
    }
    /** CA 2 Core Changes Start **/
#ifndef UE_SIM_TESTING
    /** SPR 11030 Changes Start **/
    ueCtx_p = PDCP_GET_VALID_UE_CONTEXT(ueIndex);
    /** SPR 11030 Changes End **/
    if(!ueCtx_p)
    {
        LOG_PDCP_MSG( PDCP_INVALID_LC, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                __LINE__, lcId, ueIndex,0,0, 0,0, __func__,"");
        /* SPR 4440 Fix start */
	/*Coverity 107175 fix*/ 
        freeMemPool(deliveryStatus_p);
        /* SPR 4440 Fix End */
        return 0;
    }
#endif
    /** CA 2 Core Changes Start **/
    /* Allocate and fill delivery status indication structure */
    deliveryInd = pdcpAllocateTxDeliveryInd( ueIndex, lcId, 
            count,(PdcpStatusBuffer*)deliveryStatus_p);
    if ( PNULL == deliveryInd )
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_TX, 
                pdcpCurrentTime_g,__LINE__, sizeof(PdcpDeliveryInd), 
                ueIndex, lcId, count,0,0,__func__,"");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("MemAlloc fails for PdcpDeliveryInd\n");
        return 0;
    }    
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( PDCP_RLC_STATUS_IND_RECVD, LOGDEBUG, PDCP_TX, 
            pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 
            (ADDR)(deliveryStatus_p->transId),
            deliveryStatus_p->deliveryStatus, 0,0, __func__,"");
/* SPR 20636 Changes End */
    /** CA 2 Core Changes Start **/
    /* + Coverity Id - 32622 */
#ifndef UE_SIM_TESTING
    if(!ENQUEUE_PDCP_TX_DELIVERY_IND_Q(deliveryInd,ueCtx_p->cellIndex))
    {
        freeMemPool (deliveryInd);
    }

#else
    if(!ENQUEUE_PDCP_TX_DELIVERY_IND_Q(deliveryInd))
    {
        freeMemPool (deliveryInd);
        deliveryInd = PNULL;
    }
#endif
    /* - Coverity Id - 32622 */
    /** CA 2 Core Changes End **/

#ifndef LINUX_PC_TEST
    freeMemPool(deliveryStatus_p);
#endif

    /* SPR 4440 Fix start */
    /* SPR 4440 Fix End */
    return 1;
}

/****************************************************************************
 * Function Name  : rlcDeliveryStatusInd 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  count -
 *                  deliveryStatus - the delivery status.
 * Outputs        : None.
 * Returns        : 0/1
 * Description    : This function receives data delivery indication (PDCP PDU)
 *                  from the RLC layer and enqueues it in RLC Delivery Status
 *                  Ind Queue.
 ****************************************************************************/
UInt32  rlcDeliveryStatusInd(
        UInt16  ueIndex,
        UInt16  lcId,
        UInt32  count,
        RlcStatusBuffer* deliveryStatus_p)
{
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
    RlcDeliveryStatusInd * rlcDeliveryStatusInd_p = PNULL;

    rlcDeliveryStatusInd_p = 
        (RlcDeliveryStatusInd *)getMemFromPool(sizeof(RlcDeliveryStatusInd) ,PNULL);
    if(rlcDeliveryStatusInd_p == PNULL)
    {
            LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_TX, 
                     pdcpCurrentTime_g,__LINE__, sizeof(PdcpDeliveryInd), 
                     ueIndex, lcId, count,0,0,__func__,"");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            ltePanic("MemAlloc fails for PdcpDeliveryInd\n");
            return 0;
    }
     rlcDeliveryStatusInd_p->ueIndex = ueIndex;
     rlcDeliveryStatusInd_p->lcId = lcId;
     rlcDeliveryStatusInd_p->count = count;
     rlcDeliveryStatusInd_p->deliveryStatus_p = deliveryStatus_p;


 /* + SPR 9253 */ 
     
     if(!ENQUEUE_RLC_DELIVERY_STATUS_IND_Q(rlcDeliveryStatusInd_p))
     {
         freeMemPool(rlcDeliveryStatusInd_p);
         rlcDeliveryStatusInd_p = PNULL;
     }
/* - SPR 9253 */         
     return 1;
#else
     return rlcProcessDeliveryStatusInd(ueIndex,lcId,count,deliveryStatus_p);
#endif
}

#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
/****************************************************************************
 * Function Name  : rlcHandleDeliveryStatusInd 
 * Inputs         : cQCount
 * Outputs        : None.
 * Returns        : 0/1
 * Description    : This function receives data delivery indication (PDCP PDU)
 *                  from the RLC layer and enqueues it in RLC Delivery Status
 *                  Ind Queue.
 ****************************************************************************/
UInt32  rlcHandleDeliveryStatusInd(UInt32 cQCount)
{
    UInt16  ueIndex;
    UInt16  lcId;
    UInt32  count;
    RlcStatusBuffer* deliveryStatus_p;
        
    RlcDeliveryStatusInd * deliveryStatusInd_p = PNULL;
    while(cQCount >0)
    {
        DEQUEUE_RLC_DELIVERY_STATUS_IND_Q(deliveryStatusInd_p);
        if(deliveryStatusInd_p == PNULL)
        {
            return 0;
        }
        ueIndex = deliveryStatusInd_p->ueIndex;
        lcId = deliveryStatusInd_p->lcId;
        count = deliveryStatusInd_p->count;
        deliveryStatus_p = deliveryStatusInd_p->deliveryStatus_p;
            
        rlcProcessDeliveryStatusInd(ueIndex,lcId,count,deliveryStatus_p);
        freeMemPool(deliveryStatusInd_p);
        deliveryStatusInd_p = PNULL;
        cQCount --;
    }
    return 1;
}
#endif

/* SPR 1535 Changes Start*/
/****************************************************************************
 * Function Name  : rlcFirstSubFrameInd
 * Inputs         : sysFrameNum, UInt32 subFrameNum, InternalCellIndex internalCellIndex
 * Outputs        : None.
 * Returns        : None.
 * Description    : update the pdcp tick 
 ****************************************************************************/
/* + Layer2 NON CA Changes start */

#ifndef UE_SIM_TESTING

void rlcFirstSubFrameInd( UInt32 sysFrameNum, UInt32 subFrameNum,InternalCellIndex internalCellIndex)
#else
void rlcFirstSubFrameInd( UInt32 sysFrameNum, UInt32 subFrameNum)
#endif
{
    /* SPR 2030 fix Start*/
#ifndef UE_SIM_TESTING
    pdcpGlobalTTITickCount_g[internalCellIndex] =  sysFrameNum * 10 + subFrameNum;
    /* this global TTI is updated for Logging and RoHC timers
     * This will be updated only for first cell which is active for
     * tick updation */
    if (internalCellIndex == layer2CommonGetActiveCellTick())
    {
        pdcpCurrentTime_g = sysFrameNum * 10 + subFrameNum;
        /* Relay timers will always using this tick for active cell
         * and no cell wise tick needed */
        pdcpFirstSubFrameInd(sysFrameNum, subFrameNum);
    }
    /* - Layer2 NON CA Changes */
#else
    pdcpCurrentTime_g = sysFrameNum * 10 + subFrameNum;
#endif
    /* SPR 2030 fix End*/
}
/* SPR 1535 Changes End*/

/* FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
/* SPR 1535 Changes Start*/
/****************************************************************************
 * Function Name  : rlcFirstSubFrameIndFrmL2Tick
 * Inputs         : currentTickFrmL2Tick
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function updates the pdcp tick.
 ****************************************************************************/
    /* SPR 15909 fix start */
void rlcFirstSubFrameIndFrmL2Tick(tickType_t currentTickFrmL2Tick)
    /* SPR 15909 fix end */
{
    pdcpCurrentTime_g = currentTickFrmL2Tick;
    
#ifndef UE_SIM_TESTING
    /* SPR 2030 fix Start*/
    pdcpFirstSubFrameIndFrmL2Tick(pdcpCurrentTime_g);
    /* SPR 2030 fix End*/
#endif
}
#endif
/* FAPI L2 AS MASTER END */

/* + Layer2 NON CA Changes start */
#ifndef UE_SIM_TESTING
void rlcSubFrameInd(InternalCellIndex internalCellIndex)
#else
/* + SPR 17439 */    
void rlcSubFrameInd(void)
/* - SPR 17439 */    
#endif
 /* - Layer2 NON CA Changes */
{
#ifdef PDCP_THROUGHPUT_STATS     
#endif
    /* SPR 2030 fix Start*/
#ifndef UE_SIM_TESTING
    pdcpGlobalTTITickCount_g[internalCellIndex]++;
    /* this global TTI is updated for Logging and RoHC timers
     * This will be updated only for first cell which is active for
     * tick updation */
    if (internalCellIndex == layer2CommonGetActiveCellTick())
    {
        pdcpCurrentTime_g++;

#if  !defined(PDCP_GTPU_INTF)
        /* Relay timers will always using this tick for active cell
         * and no cell wise tick needed */
        pdcpSubFrameInd();
#endif
    }
#else
    pdcpCurrentTime_g++;
#endif
    /* SPR 2030 fix End*/
/* - Layer2 NON CA Changes */
#ifdef PDCP_THROUGHPUT_STATS     
     static tickType_t updatedTick = 0;
     if(!(pdcpCurrentTime_g % 5000) && (updatedTick != pdcpCurrentTime_g))
     {
        updatedTick = pdcpCurrentTime_g;
/* SPR 20441 Changes Start*/
        fprintf(stderr, "PDCP THP: TX: %03f bps, RX: %03f bps Num Pkt Tx: %u\n", ((((float)pdcpDataTx)/5)*8), ((((float)pdcpDataRx)/5)*8), pdcpPktTx);
/* SPR 20441 Changes End*/
        pdcpDataTx = 0;
        /* Add Num Pkt Stat Start */
        pdcpPktTx  = 0;
        /* Add Num Pkt Stat End */
        pdcpDataRx = 0;
        pdcpDataRxPkt = 0;
     }
#endif
}


/*LWA_Changes*/
/*LWA_Changes*/

