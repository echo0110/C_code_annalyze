/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcEntityDef.c,v 1.1.1.1.12.2.2.2 2010/10/07 13:35:13 gur23054 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file describes data structure operations for 
 *                      entities.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcEntityDef.c,v $
 *
****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcEntityDef.h"
#include "alarm.h"
#include "logging.h"
#include "lteCommonStatsManager.h"
#include "lteRlcAmdArqOperation.h"
#include "lteRlcTimer.h"
/*SPR 22684 Fix3 Start*/
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
#include "lteRlcEmbmsDef.h"
#endif
/*SPR 22684 Fix3 Stop*/
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */





/****************************************************************************
 * Function Name  : rlcInitTxUMEntity
 * Inputs         : txUMEntity_p - pointer to TxUMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : initializes UM Entity referenced by @txUMEntity_p.
 ****************************************************************************/
void rlcInitTxUMEntity(TxUMEntity* txUMEntity_p)
{
    txUMEntity_p->lcId                     = 0;
    txUMEntity_p->txSduIn.sn               = 0;
    /* Rel 5.3: Coverity 32379 Fix Start */
    txUMEntity_p->txSduIn.pRawSdu.copyType = RLC_ZERO_COPY_BUFFER;
    /* Rel 5.3: Coverity 32379 Fix End */
    txUMEntity_p->txSduIn.pRawSdu.size     = 0;
    txUMEntity_p->txSduIn.pRawSdu.szUsed   = 0;
    txUMEntity_p->txSduIn.pRawSdu.data_p   = NULL;
    txUMEntity_p->txQueueSize              = 0;
    /* Rel 5.3: Coverity 32378 Fix Start */
    txUMEntity_p->snFieldLength            = invalidSnSize;
    /* Rel 5.3: Coverity 32378 Fix End */
    txUMEntity_p->US                       = 0;
    /** Rel 4. 0changes Start **/
#ifndef UE_SIM_TESTING
    txUMEntity_p->rlcQueueSize = RLC_QUEUE_SIZE;
    txUMEntity_p->qci = 0;
    txUMEntity_p->bitRateTx = 0;
    /* Initialize SPS variables */
    txUMEntity_p->isSpsLc = LTE_FALSE;
    txUMEntity_p->rlcTxSegCounter = 0;
    txUMEntity_p->prevAvgRunningSduSize = 0;
#endif
    /* + SPR 20548 */
    /* - SPR 20548 */
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
	txUMEntity_p->dcBearerType = INVALID_DC_BEARER;
#endif

    /** Rel 4. 0changes end **/
    QUEUE_INIT_UMD_RAW_SDU_Q(txUMEntity_p->txSDUQueue);
}

/****************************************************************************
 * Function Name  : rlcInitRxUMEntity
 * Inputs         : rxUMEntity_p - pointer to RxUMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : initializes UM Entity referenced by @rxUMEntity_p.
 ****************************************************************************/
void rlcInitRxUMEntity(RxUMEntity* rxUMEntity_p)
{
    RlcROTm *umROTimer_p = RLC_NULL;

    rxUMEntity_p->lcId = 0 ;
    if((rxUMEntity_p->rxUmdPduRing.pR = 
                (Ring * )getMemFromPool(sizeof(Ring) ,RLC_NULL)) == RLC_NULL)
    {
        ALARM_MSG (RLC_MODULE_ID,MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        return;
    }
    rlcRingInitUmdPduR(&rxUMEntity_p->rxUmdPduRing, 0);
    QUEUE_INIT_UMD_RAW_SDU_Q(rxUMEntity_p->rxReSduQ);
    rxUMEntity_p->rxSduIn.sn = 0;
    /* Rel 5.3: Coverity 32373 Fix Start */
    rxUMEntity_p->rxSduIn.pRawSdu.copyType = RLC_ZERO_COPY_BUFFER;
    /* Rel 5.3: Coverity 32373 Fix End */
    rxUMEntity_p->rxSduIn.pRawSdu.size = 0;
    rxUMEntity_p->rxSduIn.pRawSdu.szUsed = 0;
    rxUMEntity_p->rxSduIn.pRawSdu.data_p = NULL;
    rxUMEntity_p->UR = 0;
    rxUMEntity_p->UX = RLC_UNULL;
    rxUMEntity_p->UH = 0;
    /* Rel 5.3: Coverity 32374 Fix Start */
    rxUMEntity_p->snFieldLength = invalidSnSize;
    /* Rel 5.3: Coverity 32374 Fix End */
    /** Rel 4.0 changes Start **/
#ifndef UE_SIM_TESTING
    rxUMEntity_p->qci = 0;
    rxUMEntity_p->bitRateRx = 0;
    rxUMEntity_p->isSpsLc = LTE_FALSE;
    rxUMEntity_p->rlcRxSegCounter = 0;
#endif
    /** Rel 4.0 chanegs End **/
    /* Initialize the UM Reordering Timer values */
    umROTimer_p = &(rxUMEntity_p->umROTimerInfo);
    umROTimer_p->roDuration    = 0;
    umROTimer_p->roTimerNode_p = PNULL;
    umROTimer_p->roExpiryIdx   = INVALID_EXP_IDX;

    rxUMEntity_p->isTimerRunning = RLC_FALSE;
/*SPR 22684 Fix3 Start*/
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
    rxUMEntity_p->mchEntity = RLC_EMBMS_INVALID_ENTITY;
#endif
/*SPR 22684 Fix3 Stop*/
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
	rxUMEntity_p->dcBearerType = INVALID_DC_BEARER;
#endif
}

/*RLC Rel 2.0 Changes Start*/
/****************************************************************************
 * Function Name  : rlcInitTxAMEntity
 * Inputs         : txAMEntity_p - pointer to TxAMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : initializes AM Entity referenced by @txAMEntity_p.
 ****************************************************************************/
void rlcInitTxAMEntity(TxAMEntity* txAMEntity_p)
{
    RlcPRTm *prTimer_p = PNULL;
    UInt32 cnt = 0;

    txAMEntity_p->lcId = 0;
    QUEUE_INIT_AMD_RAW_SDU_Q(txAMEntity_p->txSDUQueue);
    txAMEntity_p->txSduIn.sn = 0;
    txAMEntity_p->txSduIn.rlcAmRawSdu.size = 0;
    txAMEntity_p->txSduIn.rlcAmRawSdu.data_p = NULL;
    txAMEntity_p->txQueueSize = 0;
    /* Initialize the AM Poll Retransmit values */
    prTimer_p = &(txAMEntity_p->prTimerInfo);
    prTimer_p->prDuration    = 0;
    prTimer_p->prExpiryIdx   = INVALID_EXP_IDX;
    prTimer_p->prTimerNode_p = PNULL;
    
    txAMEntity_p->istRetransmitTimerRunning = RLC_FALSE;

    txAMEntity_p->vtA = 0;
    txAMEntity_p->vtMS = txAMEntity_p->vtA + AM_WINDOW_SIZE;
    txAMEntity_p->vtS              = 0;
    txAMEntity_p->pollSN           = 0;
    txAMEntity_p->pollBit          = 0;
    txAMEntity_p->pduWithoutPoll   = 0;
    txAMEntity_p->byteWithoutPoll  = 0;
    txAMEntity_p->maxRetxThreshold = t0;
    txAMEntity_p->pollPDU          = p0;
    txAMEntity_p->pollByte         = kb0;

    /* SPR 17554 Start */
    txAMEntity_p->txSDUQueue.enqueueCount = 0;
    /* SPR 17554 End */
    
    QUEUE_INIT_AM_NACK_LIST_Q(txAMEntity_p->nackList);
    txAMEntity_p->statusReportAck = 0;
    for ( cnt = 0; cnt < AM_WINDOW_SIZE; cnt++) 
    {
        txAMEntity_p->reTxQueue[cnt] = RLC_NULL;
    }

#ifndef UE_SIM_TESTING
    txAMEntity_p->rlcQueueSize = RLC_QUEUE_SIZE;
    txAMEntity_p->qci = 0;
    txAMEntity_p->bitRateTx = 0;
#endif
    txAMEntity_p->txPduIn.sn      = 0 ;
    txAMEntity_p->txPduIn.Sostart = 0;
    txAMEntity_p->txPduIn.Soend   = 0;
    txAMEntity_p->txPduIn.pRawPdu.data_p = RLC_NULL;
    txAMEntity_p->txPduIn.pRawPdu.size   = 0;
    txAMEntity_p->reTxQueueSize          = 0;
    txAMEntity_p->sendStatusInd          = RLC_FALSE;
    txAMEntity_p->rxStatusPduSize        = 0;
    txAMEntity_p->txPduIn.pTbInfo.tbCounter = 0;
    QUEUE_INIT_AM_SDU_TID_Q(txAMEntity_p->tidQueue);
#ifdef DEBUG_STATS    
    txAMEntity_p->prTimerExpiryCount = 0;
    txAMEntity_p->statusCount = 0;
    memSet(txAMEntity_p->rlcRxStatus, 0, sizeof(RlcRxStatus) *MAX_RLC_RX_STATUS);
    txAMEntity_p->invalidCptInHdr = 0;
    txAMEntity_p->invalidAckRecvd = 0;
    txAMEntity_p->oowAckRecvd = 0;    
    txAMEntity_p->enQFailedSduQ = 0;
    txAMEntity_p->InValidInEntity = 0;
    txAMEntity_p->encodingFailedStatus = 0;
    txAMEntity_p->encodingFailedReTx = 0;
    txAMEntity_p->encodingFailedNewTx= 0;
#endif
    /* + SPR 20548 */
    /* - SPR 20548 */
}

/****************************************************************************
 * Function Name  : rlcInitRxAMEntity
 * Inputs         : rxAMEntity_p - pointer to RxAMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : initializes AM Entity referenced by @rxAMEntity_p.
 ****************************************************************************/
void rlcInitRxAMEntity(RxAMEntity* rxAMEntity_p)
{
    RlcROTm *amROTimer_p = PNULL;
    RlcSPTm *spTimer_p   = PNULL;

    rxAMEntity_p->lcId = 0;
    rxAMEntity_p->rxAmdPduRing.pR = (Ring * )getMemFromPool(sizeof(Ring) ,RLC_NULL);

    if ( RLC_NULL == rxAMEntity_p->rxAmdPduRing.pR )
    {
        ALARM_MSG (RLC_MODULE_ID,MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        return;
    }
    rlcRingInitAmdPduR(&rxAMEntity_p->rxAmdPduRing, 0);

    rxAMEntity_p->rxSduIn.sn = 0;
    rxAMEntity_p->rxSduIn.rlcAmRawSdu.size   = 0;
    rxAMEntity_p->rxSduIn.rlcAmRawSdu.data_p = NULL;

    rxAMEntity_p->sendStatusInd = 0;
	rxAMEntity_p->pollDelayInd = 0;//bug[003]jsm
	rxAMEntity_p->pollDelaySN = 65535;//bug[003]jsm
    rxAMEntity_p->vrR  = 0;
    rxAMEntity_p->vrMR = AM_WINDOW_SIZE;
    rxAMEntity_p->vrX  = RLC_UNULL;
    rxAMEntity_p->vrMS = 0;
    rxAMEntity_p->vrH  = 0;
    rxAMEntity_p->RLCRESETFLAG    = RLC_FALSE;
   /** Rel 4.0 changes Start **/
#ifndef UE_SIM_TESTING
    rxAMEntity_p->qci = 0;
    rxAMEntity_p->bitRateRx = 0;
#endif
    /** Rel 4.0 changes End **/

    /* Initialize the AM Reordering Timer values */
    amROTimer_p = &(rxAMEntity_p->amROTimerInfo);
    amROTimer_p->roDuration    = 0;
    amROTimer_p->roExpiryIdx   = INVALID_EXP_IDX;
    amROTimer_p->roTimerNode_p =  PNULL;
    rxAMEntity_p->isRTimerRunning = RLC_FALSE;
    /* Initialize the AM Status Prohibit Timer values */
    spTimer_p = &(rxAMEntity_p->spTimerInfo);
    spTimer_p->spDuration    = 0;
    spTimer_p->spExpiryIdx   = INVALID_EXP_IDX;
    spTimer_p->spTimerNode_p = PNULL;
    rxAMEntity_p->isSPTimerRunning = RLC_FALSE;

    
    QUEUE_INIT_AMD_RX_NACK_LIST_Q(rxAMEntity_p->amRxNackList);
#ifdef DEBUG_STATS
   memSet(rxAMEntity_p->rxRlcStatus, 0, sizeof(NackListArr)*MAX_RLC_RX_STATUS);
   memSet(rxAMEntity_p->ttiForStatus, 0, sizeof(UInt32)*MAX_RLC_RX_STATUS);
   rxAMEntity_p->statusCount = 0;
   rxAMEntity_p->numPktDropOOW = 0;
   rxAMEntity_p->numHdrDecFailed = 0;
   rxAMEntity_p->numPktDropDuplicate = 0;
   rxAMEntity_p->segPktHandlingFailed = 0;
   rxAMEntity_p->segPktHandlingFailed_2 = 0;
   rxAMEntity_p->reSegPktHandlingFailed = 0;
   rxAMEntity_p->firstReSegPktHandlingFailed = 0;
   rxAMEntity_p->enQFailNackList= 0;
   rxAMEntity_p->RO_timerExpired= 0;
#endif
}



/****************************************************************************
 * Function Name  : rlcDeInitTxUMEntity
 * Inputs         : txUMEntity_p - pointer to TxUMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : deinitializes UM Entity referenced by @txUMEntity_p.
 ****************************************************************************/
void rlcDeInitTxUMEntity(TxUMEntity* txUMEntity_p)
{
#ifdef RLC_QUEUE_STATS
    UInt16 qCount = 0;
    UInt16 ueId = 0;
    UInt8 lcId = 0;
#endif
    /*+ SPR_17858_532_CHANGES*/
    //EMPTY_TX_UMD_RAW_SDU_Q(txUMEntity_p->txSDUQueue);
    /*- SPR_17858_532_CHANGES*/
#ifdef RLC_QUEUE_STATS
    qCount = QUEUE_COUNT_UMD_RAW_SDU_Q(txUMEntity_p->txSDUQueue);
    lcId = txUMEntity_p->lcId;
    ueId = txUMEntity_p->ueIndex;
    lteMacUpdateStatsQueuePerUePerLc(UMD_RAW_SDU_Q,ueId,lcId,qCount);
#endif
    /**  CSR 9788 changes Start **/
    QUEUE_DE_INIT_UMD_RAW_SDU_Q(txUMEntity_p->txSDUQueue); 
    /**  CSR 9788 changes End **/
    msgFree(txUMEntity_p->txSduIn.pRawSdu.data_p);
    txUMEntity_p->txSduIn.pRawSdu.data_p = RLC_NULL;
    txUMEntity_p->txSduIn.pRawSdu.szUsed = 0;
}

/****************************************************************************
 * Function Name  : rlcDeInitRxUMEntity
 * Inputs         : rxUMEntity_p - pointer to RxUMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : deinitializes UM Entity referenced by @rxUMEntity_p.
 ****************************************************************************/
void rlcDeInitRxUMEntity(RxUMEntity* rxUMEntity_p)
{
    if (RLC_TRUE == rxUMEntity_p->isTimerRunning)                           
    {
        stopUMROTimer( rxUMEntity_p );
        rxUMEntity_p->isTimerRunning = RLC_FALSE;
    }

    rlcChangeSizeUmdPduR(&rxUMEntity_p->rxUmdPduRing,0);
    RING_DE_INIT(rxUMEntity_p->rxUmdPduRing.pR);
    /** CSR 9788 changes Start **/
    freeMemPool(rxUMEntity_p->rxUmdPduRing.pR); 
    /** CSR 9788 changes End **/
    rxUMEntity_p->rxUmdPduRing.pR = RLC_NULL;
    /** CSR 9788 changes Start **/
    QUEUE_DE_INIT_UMD_RAW_SDU_Q(rxUMEntity_p->rxReSduQ);  
    /** CSR 9788 changes End **/
    msgFree(rxUMEntity_p->rxSduIn.pRawSdu.data_p);
    rxUMEntity_p->rxSduIn.pRawSdu.data_p = RLC_NULL;
    rxUMEntity_p->rxSduIn.pRawSdu.szUsed = 0;
    rxUMEntity_p->rxSduIn.pRawSdu.size = 0;
}



/*RLC Rel 2.0 Changes Start*/
/****************************************************************************
 * Function Name  : rlcDeInitTxAMEntity
 * Inputs         : txAMEntity_p - pointer to TxAMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : deinitializes AM Entity referenced by @txAMEntity_p.
 ****************************************************************************/
void rlcDeInitTxAMEntity(TxAMEntity* txAMEntity_p)
{
    if (RLC_TRUE == txAMEntity_p->istRetransmitTimerRunning)                           
    {
        stopPRTimer( txAMEntity_p);
        txAMEntity_p->istRetransmitTimerRunning = RLC_FALSE;
    }
    /** CSR 9788 changes Start **/
    QUEUE_DE_INIT_AMD_RAW_SDU_Q(txAMEntity_p->txSDUQueue); 
    /** CSR 9788 changes End **/
    msgFree(txAMEntity_p->txSduIn.rlcAmRawSdu.data_p);
    txAMEntity_p->txSduIn.rlcAmRawSdu.data_p = RLC_NULL;
    txAMEntity_p->txSduIn.rlcAmRawSdu.size = 0;

    QUEUE_DE_INIT_TX_AM_NACK_LIST(txAMEntity_p->nackList);
    QUEUE_DE_INIT_TX_AM_SDU_TID_Q(txAMEntity_p->tidQueue);

    EMPTY_TX_AM_RETX_Q(txAMEntity_p->reTxQueue);
    /** CSR 9788 changes End **/
}

/****************************************************************************
 * Function Name  : rlcDeInitRxAMEntity
 * Inputs         : rxAMEntity_p - pointer to RxAMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : deinitializes AM Entity referenced by @rxAMEntity_p.
 ****************************************************************************/
void rlcDeInitRxAMEntity(RxAMEntity* rxAMEntity_p)
{
    if (RLC_TRUE == rxAMEntity_p->isSPTimerRunning)                           
    {
        stopSPTimer(rxAMEntity_p);
        rxAMEntity_p->isSPTimerRunning = RLC_FALSE;
    }
    if (RLC_TRUE == rxAMEntity_p->isRTimerRunning)                           
    {
        stopAMROTimer(rxAMEntity_p);
        rxAMEntity_p->isRTimerRunning = RLC_FALSE;
    }
    
    if(rxAMEntity_p->rxAmdPduRing.pR) 
    {
        /* Coverity 5.0.1 ID:25776 fix start*/
	rlcChangeSizeAmdPduR(&rxAMEntity_p->rxAmdPduRing, 0);
        /* Coverity 5.0.1 ID:25776 fix end*/
        RING_DE_INIT(rxAMEntity_p->rxAmdPduRing.pR);
        /** CSR 9788 changes Start **/
        freeMemPool(rxAMEntity_p->rxAmdPduRing.pR); 
        /** CSR 9788 changes End **/
        rxAMEntity_p->rxAmdPduRing.pR = RLC_NULL;
    }
    msgFree(rxAMEntity_p->rxSduIn.rlcAmRawSdu.data_p);
    rxAMEntity_p->rxSduIn.rlcAmRawSdu.data_p = RLC_NULL;
    rxAMEntity_p->rxSduIn.rlcAmRawSdu.size = 0;

    QUEUE_DEINIT_AMD_RX_NACK_LIST_Q(rxAMEntity_p->amRxNackList);
}

/****************************************************************************
 * Function Name  : rlcCreateRLCEntityInfo
 * Inputs         : eM - mode of Rlc Entity,
 *                  eT - type of Rlc Entity
 * Outputs        : None
 * Returns        : an object of type RLCEntityInfo
 * Description    : creates an object of type RLCEntityInfo based on 
 *                  @rlcEntityMode and @eT.
 ****************************************************************************/

RLCEntityInfo * rlcCreateRLCEntityInfo ( 
                                         RlcEntityMode eM, 
                                         RlcEntityType eT
                                       ) 
{
    RLCEntityInfo *eInfo_p = RLC_NULL;

    if (( eInfo_p =(RLCEntityInfo*)
                getMemFromPool(sizeof(RLCEntityInfo) ,RLC_NULL))== RLC_NULL)
    {

        ALARM_MSG (RLC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        return RLC_FAILURE;
    }    
    eInfo_p->rlcEntityMode  = eM;
    eInfo_p->entityType     = eT;
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    eInfo_p->rlcEntityState = RLC_ENTITY_IDLE;
#endif    

    switch ( eM )
    {

        case RLC_ENTITY_UM :
            {
                eInfo_p->rlcEntity.umEntity_p = rlcCreateUMEntity(eT);
                if (! eInfo_p->rlcEntity.umEntity_p )
                {
                   freeMemPool(eInfo_p);  
                   eInfo_p = RLC_NULL;
                }
            }
            break;
        case RLC_ENTITY_AM :
            {
                eInfo_p->rlcEntity.amEntity_p = rlcCreateAMEntity(eT);
                if ( !eInfo_p->rlcEntity.amEntity_p )
                {
                   freeMemPool(eInfo_p);
                   eInfo_p = RLC_NULL;
                }
            }
            break;
        default:
            {
                   freeMemPool(eInfo_p);
                   eInfo_p = RLC_NULL;
                  
            }
           break; 
    }
   
    return eInfo_p;
}

/****************************************************************************
 * Function Name  : rlcFreeRLCEntityInfo
 * Inputs         : eInfo_p - pointer to RLCEntityInfo,
 *                  entityType - RLC Entity Type
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : frees an object of type RLCEntityInfo.
 ****************************************************************************/
UInt32 rlcFreeRLCEntityInfo(RLCEntityInfo *eInfo_p, RlcEntityType entityType) 
{
    if(eInfo_p == RLC_NULL)  
    {
        return RLC_FAILURE;
    }    
    switch(eInfo_p->rlcEntityMode)
    {
        case RLC_ENTITY_UM :
            {
                rlcFreeUMEntity(eInfo_p->rlcEntity.umEntity_p, entityType);
                if((eInfo_p->rlcEntity.umEntity_p)  
                        && (!eInfo_p->rlcEntity.umEntity_p->txUMEntity_p)
                        && (!eInfo_p->rlcEntity.umEntity_p->rxUMEntity_p)) 
                {
                    freeMemPool(eInfo_p->rlcEntity.umEntity_p);
                    eInfo_p->rlcEntity.umEntity_p = RLC_NULL;
                    freeMemPool(eInfo_p);
                }    
            }
            break;
        case RLC_ENTITY_AM :
            {
                rlcFreeAMEntity(eInfo_p->rlcEntity.amEntity_p, entityType);
                if((eInfo_p->rlcEntity.amEntity_p)
                        && (!eInfo_p->rlcEntity.amEntity_p->txAMEntity_p)
                        && (!eInfo_p->rlcEntity.amEntity_p->rxAMEntity_p)) 
                {
                    freeMemPool(eInfo_p->rlcEntity.amEntity_p);
                    eInfo_p->rlcEntity.amEntity_p = RLC_NULL;
                    freeMemPool(eInfo_p);
                }
                 /*+ SPR_17858_530_CHANGES*/
                else
                {
                    return RLC_FAILURE;
                }
                /*- SPR_17858_530_CHANGES*/
            }
            break ;
        default:
            {
                return RLC_FAILURE;
            }
    }
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : rlcCreateUMEntity
 * Inputs         : eT - Rlc Entity Type
 * Outputs        : None
 * Returns        : an object of type UMEntity
 * Description    : creates an object of type UM Entity based on @eT.
 ****************************************************************************/
 
UMEntity * rlcCreateUMEntity(RlcEntityType eT)
{
    UMEntity* umEntity_p = RLC_NULL;
    
    if (( umEntity_p = (UMEntity * )
                getMemFromPool(sizeof(UMEntity) ,RLC_NULL)) == RLC_NULL)
    {
        ALARM_MSG (RLC_MODULE_ID,MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        return RLC_FAILURE;
    }    
    umEntity_p->rxUMEntity_p = RLC_NULL;
    umEntity_p->txUMEntity_p = RLC_NULL;
    
    switch(eT)
    {
        case RLC_TX_ENTITY :
            {
                if((umEntity_p->txUMEntity_p = (TxUMEntity * )getMemFromPool
                            (sizeof(TxUMEntity) ,RLC_NULL)) == RLC_NULL )
                {
                    ALARM_MSG (RLC_MODULE_ID,MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
                    /* SPR 5292_6023 Changes Start */
                    /* msgFree(umEntity_p); */
                    freeMemPool(umEntity_p);
                    /* SPR 5292_6023 Changes End */
                    return RLC_FAILURE;
                }    
                rlcInitTxUMEntity(umEntity_p->txUMEntity_p);
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                umEntity_p->txEntityState = RLC_ENTITY_ACTIVE;
#endif    
            }
            break;
        case RLC_RX_ENTITY:
            {
                if((umEntity_p->rxUMEntity_p = (RxUMEntity * )getMemFromPool
                            (sizeof(RxUMEntity) ,RLC_NULL)) == RLC_NULL)
                {
                    ALARM_MSG (RLC_MODULE_ID,MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
                    /* SPR 5292_6023 Changes Start */
                    /* msgFree(umEntity_p); */
                    freeMemPool(umEntity_p);
                    /* SPR 5292_6023 Changes End */
                    return RLC_FAILURE;
                }    
                rlcInitRxUMEntity(umEntity_p->rxUMEntity_p);
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                umEntity_p->rxEntityState = RLC_ENTITY_ACTIVE;
#endif    
            }
            break;
        case RLC_TX_RX_ENTITY:
            {
                if (( umEntity_p->txUMEntity_p = (TxUMEntity * )getMemFromPool
                            (sizeof(TxUMEntity) ,RLC_NULL)) == RLC_NULL)
                {
                    ALARM_MSG (RLC_MODULE_ID,MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
                    /* SPR 5292_6023 Changes Start */
                    /* msgFree(umEntity_p); */
                    freeMemPool(umEntity_p);
                    /* SPR 5292_6023 Changes End */
                    return RLC_FAILURE;
                }
                if (( umEntity_p->rxUMEntity_p = (RxUMEntity * )getMemFromPool
                            (sizeof(RxUMEntity) ,RLC_NULL)) == RLC_NULL)
                {
                    ALARM_MSG (RLC_MODULE_ID,MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
                    /* SPR 5292_6023 Changes Start */
                    /* msgFree(umEntity_p); */
                   freeMemPool(umEntity_p->txUMEntity_p);
                   umEntity_p->txUMEntity_p = RLC_NULL;
                    freeMemPool(umEntity_p);
                    /* SPR 5292_6023 Changes End */
                    return RLC_FAILURE;
                }
                rlcInitTxUMEntity(umEntity_p->txUMEntity_p);
                rlcInitRxUMEntity(umEntity_p->rxUMEntity_p);
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                umEntity_p->txEntityState = RLC_ENTITY_ACTIVE;
                umEntity_p->rxEntityState = RLC_ENTITY_ACTIVE;
#endif    
            }
            break;
        default: 
            {
            
            }
    }
    return umEntity_p ;
}

/****************************************************************************
 * Function Name  : rlcFreeUMEntity
 * Inputs         : umEntity_p - a pointer to UMEntity,
 *                  entityType - RLC Entity Type
 * Outputs        : None
 * Returns        : None
 * Description    : frees an object of type UM Entity refered by @umEntity_p.
 ****************************************************************************/
void rlcFreeUMEntity(UMEntity *umEntity_p, RlcEntityType entityType) 
{
    if(umEntity_p == RLC_NULL)
    {
        return ;
    }
    /** SPR 2712 Changes Start **/
    if(RLC_TX_RX_ENTITY == entityType)
    { 
        if (umEntity_p->txUMEntity_p != RLC_NULL) 
        {
            rlcDeInitTxUMEntity(umEntity_p->txUMEntity_p);
            freeMemPool(umEntity_p->txUMEntity_p);
            umEntity_p->txUMEntity_p = RLC_NULL;
        }
        if(umEntity_p->rxUMEntity_p != RLC_NULL)
        {
            rlcDeInitRxUMEntity(umEntity_p->rxUMEntity_p);
            freeMemPool(umEntity_p->rxUMEntity_p);
            umEntity_p->rxUMEntity_p = RLC_NULL;
        }
    }
    else if((RLC_TX_ENTITY == entityType) && (umEntity_p->txUMEntity_p != RLC_NULL))
    /** SPR 2712 Changes End **/
    {
        rlcDeInitTxUMEntity(umEntity_p->txUMEntity_p);
        freeMemPool(umEntity_p->txUMEntity_p);
        umEntity_p->txUMEntity_p = RLC_NULL;
    }
    else if((RLC_RX_ENTITY == entityType) && 
            (umEntity_p->rxUMEntity_p != RLC_NULL))
    {
        rlcDeInitRxUMEntity(umEntity_p->rxUMEntity_p);
        freeMemPool(umEntity_p->rxUMEntity_p);
        umEntity_p->rxUMEntity_p = RLC_NULL;
    }
}



/*RLC Rel 2.0 Changes Start*/
/****************************************************************************
 * Function Name  : rlcCreateAMEntity
 * Inputs         : eT - Rlc Entity Type
 * Outputs        : None
 * Returns        : an object of type AMEntity
 * Description    : creates an object of type AM Entity based on @eT.
 ****************************************************************************/
AMEntity * rlcCreateAMEntity(RlcEntityType eT) 
{
    AMEntity*   amEntity_p = RLC_NULL;
    if((amEntity_p = (AMEntity * )getMemFromPool
                (sizeof(AMEntity) , RLC_NULL)) == RLC_NULL ) 
    {
        ALARM_MSG (RLC_MODULE_ID,MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        return RLC_FAILURE;
    }
    amEntity_p->rxAMEntity_p = RLC_NULL;
    amEntity_p->txAMEntity_p = RLC_NULL;
    switch(eT) 
    {
        case RLC_TX_RX_ENTITY:
            {
                if((amEntity_p->txAMEntity_p = (TxAMEntity * )getMemFromPool
                            (sizeof(TxAMEntity), RLC_NULL)) == RLC_NULL) 
                {
                    ALARM_MSG (RLC_MODULE_ID,MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
                    /* SPR 5292_6023 Changes Start */
                    /* msgFree(amEntity_p); */
                    freeMemPool(amEntity_p);
                    /* SPR 5292_6023 Changes End */
                    return RLC_FAILURE;
                }
                if((amEntity_p->rxAMEntity_p = (RxAMEntity * )getMemFromPool
                            (sizeof(RxAMEntity), RLC_NULL)) == RLC_NULL) 
                {
                    ALARM_MSG (RLC_MODULE_ID,MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
                    /* SPR 5292_6023 Changes Start */
                    /* msgFree(amEntity_p); */
                    freeMemPool(amEntity_p->txAMEntity_p);
                    amEntity_p->txAMEntity_p = RLC_NULL;
                    freeMemPool(amEntity_p);
                    /* SPR 5292_6023 Changes End */
                    return RLC_FAILURE;
                }    
                rlcInitTxAMEntity(amEntity_p->txAMEntity_p);
                rlcInitRxAMEntity(amEntity_p->rxAMEntity_p);
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                amEntity_p->txEntityState = RLC_ENTITY_ACTIVE;
                amEntity_p->rxEntityState = RLC_ENTITY_ACTIVE;
#endif    
            }
            break ;
        default:
            {
            }
    }
    return  amEntity_p;
}

/****************************************************************************
 * Function Name  : rlcFreeAMEntity
 * Inputs         : amEntity_p - a pointer to AMEntity,
 *                  entityType - RLC Entity Type
 * Outputs        : None
 * Returns        : None
 * Description    : frees an object of type AM Entity refered by @amEntity_p.
 ****************************************************************************/
void rlcFreeAMEntity(AMEntity *amEntity_p, RlcEntityType entityType) 
{
    if(amEntity_p == RLC_NULL)
    {
        return;
    }    
    /** SPR 2712 Changes Start **/
    if(RLC_TX_RX_ENTITY == entityType)
    {
        if(amEntity_p->txAMEntity_p != RLC_NULL) 
        { 
             rlcDeInitTxAMEntity(amEntity_p->txAMEntity_p);
             freeMemPool(amEntity_p->txAMEntity_p);
             amEntity_p->txAMEntity_p = RLC_NULL;
        }
        if(amEntity_p->rxAMEntity_p != RLC_NULL)
        {
             rlcDeInitRxAMEntity(amEntity_p->rxAMEntity_p);
             freeMemPool(amEntity_p->rxAMEntity_p);
             amEntity_p->rxAMEntity_p = RLC_NULL;
        }
    }
    else if((RLC_TX_ENTITY == entityType) && (amEntity_p->txAMEntity_p != RLC_NULL))
    /** SPR 2712 Changes End **/
    {
        rlcDeInitTxAMEntity(amEntity_p->txAMEntity_p);
        freeMemPool(amEntity_p->txAMEntity_p);
        amEntity_p->txAMEntity_p = RLC_NULL;
    } 
    else if((RLC_RX_ENTITY == entityType) && 
            (amEntity_p->rxAMEntity_p != RLC_NULL))
    {
        rlcDeInitRxAMEntity(amEntity_p->rxAMEntity_p);
        freeMemPool(amEntity_p->rxAMEntity_p);
        amEntity_p->rxAMEntity_p = RLC_NULL;
    }
    else 
    {
        return;
    }    
}
/*RLC Rel 2.0 Changes End*/

