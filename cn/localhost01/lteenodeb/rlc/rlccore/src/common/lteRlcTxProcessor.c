/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcTxProcessor.c,
 *
 ****************************************************************************
 *
 *  File Description : this file describes the processing of messages (SDUs)
 *                     from upper layers(PDCP) 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcTxProcessor.c,v $
 *
****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcPdcpInterface.h"
#include "lteRlcMacInterface.h"
#include "lteRlcRrcInterface.h"
#include "lteRlcTxProcessor.h"
#include "lteRlcUmTxOperation.h"
/*RLC Rel 2.0 Changes Start*/
#include "lteRlcAmTxOperation.h"
#include "lteRlcAmdArqOperation.h"
/*RLC Rel 2.0 Changes End*/
#include "lteRlcUeContext.h"
#include "lteLayer2CommanTypes.h"
#include "logging.h"
#include "lteRlcStat.h"
#include "lteCommonStatsManager.h"
#include "atomic-ops.h"
#include "lteRlcErrorCode.h"
#include "lteCommonStatsManager.h"
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
#ifdef RLC_STANDALONE_SETUP 
RlcThread rlcTxThread_g = RLC_THREAD_INIT;
RlcThread rlcTxPdcpThread_g = RLC_THREAD_INIT;
#endif

/* CA Changes start */
extern SEM_T waitForMacUeOpporInd_g[MAX_NUM_CELL];
/* CA Changes end */
/* SPR 8712 changes */
#if defined (DL_DATA_SEPARATION) || defined (UE_SIM_TESTING)
extern MAC_UE_OPPOR_IND_CIRC_TYPE* macUeOpporIndCircQ_gp[MAX_NUM_CELL];
#elif defined(DL_UL_SPLIT) && !defined(CIRC_QUEUE_IMPL) 
extern SInt32 macUeOppnIndQIdUL_g[MAX_NUM_CELL];
#else
extern MAC_UE_OPPOR_IND_CIRC_TYPE* macUeOpporIndCircQ_gp[MAX_NUM_CELL];
#endif
/* SPR 8712 changes */

/* warning removed*/
extern SInt32 recvMsgWithIccQ(SInt32 mqdes, const char *msg_ptr);


/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */
extern UInt16 rlcLayerInitialized_g;

extern void userPrintRawBuffer(const UChar8* strHeader, const UInt8* buf, 
                                  UInt32 len_buf, UInt32 fmt);

/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */
#ifdef RLC_QUEUE_STATS
extern void lteMacUpdateStatsQueuePerUePerLc( UInt16, UInt16, UInt8, UInt16 );
#endif

#ifdef RLC_STANDALONE_SETUP
extern  void rlcCheckTimerExpiry();
/* SPR 15909 fix start */
extern tickType_t rlcCurrentTime_g ;
/* SPR 15909 fix end */
#endif
#ifndef UE_SIM_TESTING
SInt32 semWaitDefault(SEM_T *pSem);

#define MAX_SEG_IND_LIMIT_DL  10
#endif

 /** SPR 13599 Changes Start **/
 extern RLCHarqFailureInfo  *rlcHarqFailureInfo_g ;
 /** SPR 13599 Changes End **/

/****************************************************************************
 * Function Name  : rlcRunTxProcessor
 * Inputs         : args - pointer to argument(s)
 * Outputs        : None
 * Returns        : args
 * Description    : this is startup routine for Tx Thread.
 ****************************************************************************/

#ifdef RLC_STANDALONE_SETUP
void* rlcRunTxProcessor(void* args) 
{
    unsigned long new_mask = 0x02;
    pid_t p = 0;
    THREAD_TO_BIND_GET_CORE_NO(p);
    THREAD_TO_BIND_SET_CORE_NO(p,new_mask);

    //for ( ; ; )
    {
        /*Wait till there is some thing to dequeue */
      //  semWait(&rlcTxSemlock_g);
        rlcRunTxExecutionLeg();
    }
    return args ;
}
#endif

/****************************************************************************
 * Function Name  : rlcRunTxExecutionLeg
 * Inputs         : None
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This thread is used to run Tx Execution Leg.
 ****************************************************************************/

#ifdef RLC_STANDALONE_SETUP
UInt32 rlcRunTxExecutionLeg(void)
{
    UInt64 count = 0;

        count = COUNT_MAC_UE_OPPOR_IND_CIRC_Q();
        while(count--)
        {
            rlcHandleTxOppurtunity();
        }
        
/*coverity-530-32868 */
#ifdef OBSELETE_REL_8
/*coverity-530-32868 */
        count = sQueueCount(&macUeHarqFailureIndQ_g);
        if(count){
            rlcHandleHarqFailureInd(count);
        }
#endif        
        uSleep(1);
        rlcCurrentTime_g++;
        rlcCheckTimerExpiry();
        rlcHdlrPenDelQ(&rlcTxPendingDelQ_g) ;
        /*SPR 3805 Fix Start */
        rlcHdlrPenUeDelQ(&rlcTxUePendingDelQ_g);
        /* SPR 3805 Fix End */
        rlcHdlrReEstabQ(&rlcTxReEstabQ_g) ;

    return RLC_SUCCESS;
}
#endif
/****************************************************************************
 * Function Name  : rlcInitTxThread
 * Inputs         : args - pointer to argument(s)
 * Outputs        : None
 * Returns        : RLC_SUCCESS if Tx thread created successfully else
 *                  RLC_FAILURE
 * Description    : Initializes Tx Thread.
 ****************************************************************************/
#ifdef RLC_STANDALONE_SETUP 
SInt32 rlcInitTxThread(void* args)
{
    SInt32 ret = RLC_SUCCESS ;
    semInit(&rlcTxSemlock_g,0);
    rlcTxThread_g.start = rlcRunTxProcessor ;
    rlcTxThread_g.arg = args ;

    ret = threadCreate(&rlcTxThread_g.tid,THREAD_CREATE_DETACHED,
            THREAD_SCHED_FIFO,1,
            THREAD_EXPLICIT_SCHED,
            THREAD_SCOPE_SYSTEM,rlcTxThread_g.start,PNULL);

    THREAD_TO_BIND_SET_CORE_NO(rlcTxThread_g.tid, 12);

    if(ret) 
    {
        ltePanic("Unable to initialize Tx Thread\n");
    }
    return ret ;
}
#endif
/****************************************************************************
 * Function Name  : rlcDeInitTxThread
 * Inputs         : None
 * Outputs        : None
 * Returns        : RLC_SUCCESS if Tx thread created successfully else
 *                  RLC_FAILURE
 * Description    : Cancel Tx Thread.
 ****************************************************************************/
#ifdef RLC_STANDALONE_SETUP 
SInt32 rlcDeInitTxThread()
{
    SInt32 ret = RLC_SUCCESS ;
    if(rlcTxThread_g.tid > 0) 
    {
        ret = threadCancel(rlcTxThread_g.tid);
        rlcTxThread_g.tid = 0;
    }
    semDestroy(&rlcTxSemlock_g);
    return ret ;
}
#endif



/****************************************************************************
 * Function Name  : isValidUeDataReq
 * Inputs         : ueDataReq_p - Pointer to RlcUeDataReq,
 *                  ueContext_p - Pointer to UEContext
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_UE_CTX_NOT_EXIST/RLC_NULL_PTR_RECVD
 * Description    : It Validates UE Data Requests.
 ****************************************************************************/
static inline UInt8 isValidUeDataReq( RlcUeDataReq* ueDataReq_p, 
                                      UEContext** ueContext_p 
                                    )
{
    if ( RLC_NULL == ueDataReq_p)
    {
        LOG_RLC_MSG(LTE_RLC_IS_VALID_UE_DATA_REQ_ERR, LOGWARNING, RLC_TX, 
                globalTTITickCount_g, __LINE__, 0, 0,
                0, 0, 0, 0, __func__, "");

        return RLC_NULL_PTR_RECVD;
    }
    /*fetch ueContext*/
    *ueContext_p = DB_FETCH_UE_CONTEXT(ueDataReq_p->ueIndex) ;
    if ( RLC_NULL == *ueContext_p || RLC_UECONTEXT_ACTIVE != (*ueContext_p)->rlcUEState)
    {
        LOG_RLC_MSG( LTE_RLC_IS_VALID_UE_DATA_REQ_ERR_1, LOGWARNING, RLC_TX, 
                globalTTITickCount_g, __LINE__, ueDataReq_p->ueIndex, 
                ueDataReq_p->lcId, 0, 0, 0, 0, __func__, "");

        return RLC_UE_CTX_NOT_EXIST;
    }
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : getRlcEntity
 * Inputs         : ueDataReq_p - Pointer to RlcUeDataReq,
 *                  entityInfo_p - Pointer to RLCEntityInfo,
 *                  txUMEntity_p - Pointer to TxUMEntity,
 *                  txAMEntity_p - Pointer to TxAMEntity
 * Outputs        : None
 * Returns        : RLC_SUCCESS/errorCode
 * Description    : It gets RLC Entity.
 ****************************************************************************/

inline static UInt32  getRlcEntity( RlcUeDataReq* ueDataReq_p,
                                    RLCEntityInfo** entityInfo_p,
                                    TxUMEntity** txUMEntity_p,
                                    TxAMEntity** txAMEntity_p
                                  )
{
    UEContext* ueContext_p = RLC_NULL;
    UInt32 errorCode       = INVALID_ERR_CODE;

    errorCode = isValidUeDataReq (ueDataReq_p, &ueContext_p);
    if ( RLC_SUCCESS != errorCode  )
    {
        return errorCode;
    }

    /*fetch RLCEntityInfo*/
    RLCEntityInfo* eInfo_p = *entityInfo_p =
        DB_FETCH_RLC_ENTITY_INFO(ueContext_p, ueDataReq_p->lcId);

    if ( RLC_NULL == eInfo_p ) 
    {
        LOG_RLC_MSG( LTE_RLC_GET_ENTITY_ERR, LOGERROR, RLC_TX, 
                globalTTITickCount_g, __LINE__, ueDataReq_p->ueIndex,
                ueDataReq_p->lcId, 0, 0, 0, 0, __func__, "");

        return RLC_ENTITY_NOT_EXIST;
    }
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    if ( eInfo_p->rlcEntityState != RLC_ENTITY_ACTIVE )
    {
        LOG_RLC_MSG( LTE_RLC_GET_ENTITY_ERR_1, LOGERROR, RLC_TX, 
                globalTTITickCount_g, __LINE__, ueDataReq_p->ueIndex,
                ueDataReq_p->lcId, eInfo_p->rlcEntityState,
                0, 0, 0, __func__, "");

        return RLC_ENTITY_NOT_ACTIVE_MODE;
    }
#endif
    switch ( eInfo_p->rlcEntityMode )
    {
        case RLC_ENTITY_AM:
            {
                AMEntity* amEntity_p = eInfo_p->rlcEntity.amEntity_p;
                if((amEntity_p == RLC_NULL )||
                        (amEntity_p->txAMEntity_p == RLC_NULL))
                {
                    LOG_RLC_MSG( LTE_RLC_GET_ENTITY_ERR_2, LOGERROR, RLC_TX, 
                            globalTTITickCount_g, __LINE__,ueDataReq_p->ueIndex ,
                            ueDataReq_p->lcId , 0, 0, 0, 0, __func__, 
                            "");

                    return RLC_AM_ENTITY_NOT_EXIST;
                }
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                if ( amEntity_p->txEntityState != RLC_ENTITY_ACTIVE )
                {
                    LOG_RLC_MSG( LTE_RLC_GET_ENTITY_ERR_1, LOGERROR, RLC_TX, 
                            globalTTITickCount_g, __LINE__, ueDataReq_p->ueIndex,
                            ueDataReq_p->lcId, amEntity_p->txEntityState,
                            0, 0, 0, __func__, "");

                    return RLC_ENTITY_NOT_ACTIVE_MODE;
                }
#endif
                *txAMEntity_p = amEntity_p->txAMEntity_p;
            }    
            break;

        case RLC_ENTITY_UM:
            {
                UMEntity* umEntity_p = eInfo_p->rlcEntity.umEntity_p ;
                if((umEntity_p == RLC_NULL )||
                        (umEntity_p->txUMEntity_p == RLC_NULL))
                {
                    LOG_RLC_MSG( LTE_RLC_GET_ENTITY_ERR_3, LOGERROR, RLC_TX, 
                            globalTTITickCount_g, __LINE__, ueDataReq_p->ueIndex,
                            ueDataReq_p->lcId, 0, 0, 0, 0, __func__, "");

                    return RLC_UM_ENTITY_NOT_EXIST;
                }
/* SPR 5330 Fix Start */
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                if ( umEntity_p->txEntityState != RLC_ENTITY_ACTIVE )
                {
                    LOG_RLC_MSG( LTE_RLC_GET_ENTITY_ERR_1, LOGERROR, RLC_TX, 
                            globalTTITickCount_g, __LINE__, ueDataReq_p->ueIndex,
                            ueDataReq_p->lcId, umEntity_p->txEntityState,
                            0, 0, 0, __func__, "");

                    return RLC_ENTITY_NOT_ACTIVE_MODE;
                }
#endif
/* SPR 5330 Fix End */
                *txUMEntity_p = umEntity_p->txUMEntity_p;
            }    
            break;

        default:
            LOG_RLC_MSG( LTE_RLC_GET_ENTITY_ERR_4, LOGERROR, RLC_TX, 
                    globalTTITickCount_g, __LINE__, ueDataReq_p->ueIndex,
                    ueDataReq_p->lcId, eInfo_p->rlcEntityMode,
                    0, 0, 0, __func__, "");

            break;
    }
    return RLC_SUCCESS;
}


/****************************************************************************
 * Function Name  : rlcHandleReceiveAmSdu
 * Inputs         : txAMEntity_p - Pointer to TxAMEntity,
 *                  ueDataReq_p - Pointer to RlcUeDataReq
 * Outputs        : None
 * Returns        : errorCode
 * Description    : It handles received AM SDU(s) from PDCP layer.
 ****************************************************************************/
inline static UInt32 rlcHandleReceiveAmSdu(  TxAMEntity* txAMEntity_p,
                                             RlcUeDataReq* ueDataReq_p
                                          )
{
    UInt32 errorCode = INVALID_ERR_CODE;
    /* 24072014 - SPR 12734 changes start */
#ifndef RLC_PDCP_IPC_INTF
    UInt32 amdSDUCount = 0;
    AmdRawSduQ *amdRawSduQ_p = &txAMEntity_p->txSDUQueue ;
#endif
    /* 24072014 - SPR 12734 changes end */

    if ( ueDataReq_p->size && ueDataReq_p->data_p )
    {
        /*receive SDU from upper layer*/
        errorCode = rlcReceiveAmSduFromUpperLayer( 
                txAMEntity_p,
                ueDataReq_p
                ) ;
    }
    /*SPR 8544 FIX START*/ 
#ifndef RLC_PDCP_IPC_INTF
    else if ( ueDataReq_p->transId  == 0)
    {
        amdSDUCount = QUEUE_COUNT_AMD_RAW_SDU_Q(*amdRawSduQ_p);

        /* + SPR 19808 */
        if ((txAMEntity_p->txQueueSize >= txAMEntity_p->rlcQueueSize) ||
                (amdSDUCount >= MAX_AM_RAW_SDU_Q_SIZE ))
            /* - SPR 19808 */
        {
            freeMemPool(ueDataReq_p);
            return 1;    
        }
        else
        {
            freeMemPool(ueDataReq_p);
            return 0;
        }
    }
#endif
    /*SPR 8544 FIX END*/
    else
    {
        /*receive SDU discard from upper layer*/
        errorCode = rlcReceiveAmSduDiscardFromUpperLayer(
                txAMEntity_p
                ,ueDataReq_p->transId
                );
        freeMemPool(ueDataReq_p);
    }
    return errorCode;
}


/****************************************************************************
 * Function Name  : rlcHandleReceiveUmSdu
 * Inputs         : txUMEntity_p - Pointer to TxUMEntity,
 *                  ueDataReq_p - Pointer to RlcUeDataReq
 * Outputs        : None
 * Returns        : errorCode
 * Description    : It handles received UM SDU(s) from PDCP layer.
 ****************************************************************************/
inline static UInt32 rlcHandleReceiveUmSdu ( TxUMEntity* txUMEntity_p,
                                             RlcUeDataReq* ueDataReq_p
                                           )
{
    UInt32 errorCode = INVALID_ERR_CODE;
	/* 24072014 - SPR 12734 changes start */
#ifndef RLC_PDCP_IPC_INTF
    UInt32 coutSDU      = 0;
	UmdRawSduQ *umdRawSduQ_p = &txUMEntity_p->txSDUQueue ;
#endif
	/* 24072014 - SPR 12734 changes end */
    
    if ( ueDataReq_p->size && ueDataReq_p->data_p)
    {
        /*receive SDU from upper layer*/
        errorCode = rlcReceiveUmSduFromUpperLayer(txUMEntity_p,
                                                  ueDataReq_p
                                                 ) ;
    }
    /*SPR 8544 FIX START*/
#ifndef RLC_PDCP_IPC_INTF
    else if ( ueDataReq_p->transId  == 0)
    {
        coutSDU = QUEUE_COUNT_UMD_RAW_SDU_Q(*umdRawSduQ_p);
        /* + SPR 19808 */
        if((txUMEntity_p->txQueueSize >= txUMEntity_p->rlcQueueSize) ||
                (coutSDU >= MAX_UM_RAW_SDU_Q_SIZE ))
            /* - SPR 19808 */
        {
            freeMemPool(ueDataReq_p);
            return 1;    
        }
        else
        {
            freeMemPool(ueDataReq_p);
            return 0;
        }
    }
#endif
    /*SPR 8544 FIX END*/
    else
    {
        /*receive SDU discard from upper layer*/
        errorCode = rlcReceiveUmSduDiscardFromUpperLayer(
                                                    txUMEntity_p
                                                     ,ueDataReq_p->transId
     );
   freeMemPool(ueDataReq_p);
    }
    return errorCode;
}

/****************************************************************************
 * Function Name  : rlcHandleDataFromPdcp
 * Inputs         : ueDataReq_p - Pointer to RlcUeDataReq
 * Outputs        : None
 * Returns        : errorCode
 * Description    : It handles SDU(s) from PDCP layer.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 rlcHandleDataFromPdcp(RlcUeDataReq* ueDataReq_p)
/* SPR 5599_129119 Fix End */
{
    UInt32 errorCode          = INVALID_ERR_CODE;
    RLCEntityInfo* eInfo_p    = RLC_NULL;
    TxUMEntity* txUMEntity_p  = RLC_NULL;
    TxAMEntity* txAMEntity_p = RLC_NULL;

    errorCode = getRlcEntity( ueDataReq_p,
            &eInfo_p,
            &txUMEntity_p, 
            &txAMEntity_p);

    if ( RLC_SUCCESS != errorCode )
    {
        if ( ueDataReq_p && ueDataReq_p->data_p )
        {
            msgFree(ueDataReq_p->data_p);
            /* SPR 20985 fix start */
            ueDataReq_p->data_p = RLC_NULL;
            /* SPR 20985 fix end */
        }
        /* SPR 4010 Fix Start */
        if (ueDataReq_p)
        {
            freeMemPool(ueDataReq_p);
        }
        /* SPR 4010 Fix End */
        LOG_RLC_MSG( LTE_RLC_HANDLE_DATA_FROM_PDCP_ERR_1, LOGWARNING, RLC_TX,
                 globalTTITickCount_g, __LINE__, 0,
                 0, 0, 0, 0, 0, __func__, "");
        
        return errorCode;
    }

    switch ( eInfo_p->rlcEntityMode)
    {
        case RLC_ENTITY_UM:
            errorCode = rlcHandleReceiveUmSdu( txUMEntity_p , ueDataReq_p );
            break;

        case RLC_ENTITY_AM:
            errorCode = rlcHandleReceiveAmSdu ( txAMEntity_p , ueDataReq_p );
            break;

        default:
            LOG_RLC_MSG( LTE_RLC_HANDLE_DATA_FROM_PDCP_ERR, LOGERROR, RLC_TX, 
                    globalTTITickCount_g, __LINE__, ueDataReq_p->ueIndex,
                    ueDataReq_p->lcId, eInfo_p->rlcEntityMode, 0, 0, 0, __func__, "");

            break;
    }
    return errorCode;
}


/*coverity-530-32868 */
#ifdef OBSELETE_REL_8
/*coverity-530-32868 */
/****************************************************************************
 * Function Name  : rlcHandleHarqFailureInd 
 * Inputs         : count - Number of nodes this Execution Leg has to process
 * Outputs        : None
 * Returns        : RLC_SUCCESS 
 * Description    : It handles harq failure indication and put an entry in 
 *                  nack_list.
 ****************************************************************************/

UInt32 rlcHandleHarqFailureInd(UInt32 count) 
{
    UEContext* ueContext_p = RLC_NULL;
    RLCEntityInfo* eInfo_p = RLC_NULL;
    MacUeHarqFailureInd *harqFailureInd_p = RLC_NULL;
    LCQueueLoad lcQLoad = {0}; 
    RlcUeDataReq * amdRawSdu_p = RLC_NULL ;
    /* SPR  8366 change start */
     UInt32         retrnsPduCount = 0; 
    /* SPR  8366 change end */
    while(count--)
    {
        bzero_wrapper( &lcQLoad, sizeof(lcQLoad));

        DEQUEUE_MAC_UE_HARQ_FAILURE_IND_Q(harqFailureInd_p);
        if ( !harqFailureInd_p ) 
        {
            LOG_RLC_MSG( LTE_RLC_HANDLE_HARQ_FAILURE_IND_ERR, LOGWARNING, RLC_TX, 
                    globalTTITickCount_g, __LINE__, 0,
                    0, 0, 0, 0, 0, __func__, "");
            continue;
        }    
        /** SPR 3657 Changes Start **/
        /** SPR 3657 Changes End **/
        if((ueContext_p = DB_FETCH_UE_CONTEXT(harqFailureInd_p->ueIndex)) 
                == RLC_NULL || RLC_UECONTEXT_ACTIVE != ueContext_p->rlcUEState)
        {
            LOG_RLC_MSG( LTE_RLC_HANDLE_HARQ_FAILURE_IND_ERR_2, LOGWARNING, RLC_TX, 
                    globalTTITickCount_g, __LINE__, harqFailureInd_p->ueIndex,
                    harqFailureInd_p->lcId, 0, 0, 0, 0, __func__, "");
            freeMemPool(harqFailureInd_p);
            harqFailureInd_p = RLC_NULL;
            continue;
        } 
        else 
        {
            eInfo_p = DB_FETCH_RLC_ENTITY_INFO(ueContext_p,
                    harqFailureInd_p->lcId);
            if((eInfo_p == RLC_NULL) ||
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
                    (eInfo_p->rlcEntityState != RLC_ENTITY_ACTIVE) ||
#endif
                    (eInfo_p->rlcEntityMode != RLC_ENTITY_AM))
            {

                LOG_RLC_MSG( LTE_RLC_HANDLE_HARQ_FAILURE_IND_ERR_3, LOGWARNING, RLC_TX, 
                        globalTTITickCount_g, __LINE__, harqFailureInd_p->ueIndex,
                        harqFailureInd_p->lcId, 0, 0, 0, 0, __func__, "");
                freeMemPool(harqFailureInd_p);
                harqFailureInd_p = RLC_NULL;

                continue;
            }


            AMEntity* amEntity_p = eInfo_p->rlcEntity.amEntity_p;

            if ( !amEntity_p || !amEntity_p->txAMEntity_p)
            {
                LOG_RLC_MSG( LTE_RLC_HANDLE_HARQ_FAILURE_IND_ERR_4, LOGWARNING, RLC_TX, 
                        globalTTITickCount_g, __LINE__, harqFailureInd_p->ueIndex,
                        harqFailureInd_p->lcId, 0, 0, 0, 0, __func__, "");
                freeMemPool(harqFailureInd_p);
                continue;
            }
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
            if(amEntity_p->txEntityState != RLC_ENTITY_ACTIVE)
            {
                LOG_RLC_MSG( LTE_RLC_HANDLE_HARQ_FAILURE_IND_ERR_3, LOGWARNING, RLC_TX,
                    globalTTITickCount_g, __LINE__, harqFailureInd_p->ueIndex,
                    harqFailureInd_p->lcId, 0, 0, 0, 0, __func__, "");
                freeMemPool(harqFailureInd_p);
                harqFailureInd_p = RLC_NULL;
                continue;
            }
#endif
            TxAMEntity* pAmTxE = amEntity_p->txAMEntity_p;

            AmReTransmission *amReTxPdu_p = RLC_NULL;

            RlcVr reTxIndex = 0;
            /* FDD_Warning_Fix */
            /* FDD_Warning_Fix */
            /** SPR 3657 Changes Start **/
            /* +COVERITY 10783 */
            /* SPR  8366 change start */
            retrnsPduCount = 0;
            /* SPR  8366 change END */
            /* -COVERITY 10783 */
            /* AmNackListNode *amNackListNode_p = RLC_NULL; */
            /** SPR 3657 Changes ENd **/
            UInt16 newPduSeqNum  = harqFailureInd_p->newPduSeqNum;
          
            do 
            {
                if ( INVALID_SEQ_NUM != newPduSeqNum)
                {
                    /*Update ReTx Queue Size with the pdu size for this seqNum*/
                    reTxIndex = newPduSeqNum & 511; 
                    amReTxPdu_p = pAmTxE->reTxQueue[reTxIndex];
                    if ( !amReTxPdu_p )
                    {
                        LOG_RLC_MSG( LTE_RLC_HANDLE_HARQ_FAILURE_IND_ERR_5, LOGWARNING, RLC_TX, 
                                globalTTITickCount_g, __LINE__, harqFailureInd_p->ueIndex,
                                harqFailureInd_p->lcId, newPduSeqNum, reTxIndex, 0, 0, __func__, "");
                         break;

                    }
                    /* This commented to remove the retensmission functionality from RLC 
                    ** for Harq failure indication
                     ** SPR 3657 Changes Start **
                    CREATE_AM_NACK_LIST_NODE(amNackListNode_p, newPduSeqNum, Sostart , Soend );
                    ENQUEUE_AMD_NACK_LIST_Q(pAmTxE->nackList, amNackListNode_p);
                    pAmTxE->reTxQueueSize += (amReTxPdu_p->pdu_size - AM_PDU_SEG_FIX_HDR_SIZE);
                    ** SPR 3657 Changes Start **
                    */

                }
            }while (0);
            
            /** SPR 3657 Changes Start **/ 
            while(harqFailureInd_p->reTxPduCount) 
            {
                /* SPR  8366 change start */
                UInt16 retxPduSeqNum = harqFailureInd_p->harqFailReTxInfo[retrnsPduCount].retxPduSeqNum;
                /* SPR  8366 change END */

                if (  INVALID_SEQ_NUM != retxPduSeqNum )
                {
                    /* FDD_Warning_Fix */
                    /* FDD_Warning_Fix */
                    /** SPR 3657 Changes End **/
                    /*Update ReTx Queue Size with the pdu size for this seqNum*/
                    reTxIndex = retxPduSeqNum & 511; 
                    amReTxPdu_p = pAmTxE->reTxQueue[reTxIndex];

                    if (!amReTxPdu_p) 
                    {
                        LOG_RLC_MSG( LTE_RLC_HANDLE_HARQ_FAILURE_IND_ERR_5, LOGWARNING, RLC_TX, 
                                globalTTITickCount_g, __LINE__, harqFailureInd_p->ueIndex,
                                harqFailureInd_p->lcId, retxPduSeqNum, reTxIndex, 0, 0, __func__, "");
                        break;

                    }
                    
                    /* This commented to remove the retensmission functionality from RLC 
                    ** for Harq failure indication
                    CREATE_AM_NACK_LIST_NODE(amNackListNode_p, retxPduSeqNum, Sostart, Soend);
                    ENQUEUE_AMD_NACK_LIST_Q(pAmTxE->nackList, amNackListNode_p);
                    if((0 == Sostart) && (0 == Soend))
                        pAmTxE->reTxQueueSize += (amReTxPdu_p->pdu_size - AM_PDU_SEG_FIX_HDR_SIZE) ;
                    else
                        pAmTxE->reTxQueueSize += (amReTxPdu_p->pdu_size - AM_PDU_RESEG_FIX_HDR_SIZE);
                    */                   
                }
                /** SPR 3657 Changes Start **/
                harqFailureInd_p->reTxPduCount--; 
                 /* SPR  8366 change start */
                 retrnsPduCount++;
                /* SPR  8366 change END */
                /** SPR 3657 Changes End **/
            }
            freeMemPool(harqFailureInd_p);
            harqFailureInd_p = RLC_NULL;


            lcQLoad.lcId = pAmTxE->lcId;

            /******************** QoS ****************************************/            
            AmdRawSduQ *amdRawSduQ_p = &pAmTxE->txSDUQueue;
            rlcFetchQueueLoad(pAmTxE,&lcQLoad);
            lcQLoad.scheduledDataBytes = 0;
            /* SPR 12610 Start */
            if( amdRawSduQ_p->pQ->count > 0)
            {
            amdRawSdu_p   =  (RlcUeDataReq *)(amdRawSduQ_p->pQ->ring[amdRawSduQ_p->pQ->head].data);
                lcQLoad.headPktTS = amdRawSdu_p->pktArrTime;
            }
            else
            {
                lcQLoad.headPktTS = 0;
            }
            /* SPR 12610 End */
            /******************** QoS ****************************************/            
            lcQLoad.ctrlFlag = RLC_NORMAL_DATA;
            /**************************************************************/           

            macUpdateBufferOccupancyReq( pAmTxE->ueIndex,
                    1/*lcCount*/,
                    &lcQLoad,
                    RLC_BO_TYPE_NEW,
                    /* SPR 20785 fix start */
                    internalCellIndex
                    /* SPR 20785 fix end */
                    );

            LOG_RLC_MSG( LTE_RLC_HANDLE_HARQ_FAILURE_IND, LOGINFO, RLC_TX, 
                    globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                    pAmTxE->lcId, newPduSeqNum,  lcQLoad.retxnPduSize, 
                    lcQLoad.ctrlQueueLoad,  lcQLoad.lcQueueLoad, __func__, "");

        }   
    }    
    return RLC_SUCCESS;
}
#endif
/****************************************************************************
 * Function Name  : rlcUMFetchQLoad
 * Inputs         : pE - Pointer to TxUMEntity,
 *                  lcQLoadArr_p - Pointer to LCQueueLoad
 * Outputs        : None
 * Returns        : LC Queue Load
 * Description    : This function Fetch RLC UM Queue Load.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 rlcUMFetchQLoad( TxUMEntity* pE, 
                               LCQueueLoad *lcQLoadArr_p
/* SPR 5599_129119 Fix End */
                             )
{
    UmdRawSduIn *umdRawSduIn_p = RLC_NULL;
    UInt32 coutSDU = 0;
    UInt32 qLoad   = 0;
    UInt32 szFHdr  = pE->szFHdr;
    
    umdRawSduIn_p  = &pE->txSduIn;
    coutSDU = QUEUE_COUNT_UMD_RAW_SDU_Q(pE->txSDUQueue);
    qLoad   = pE->txQueueSize;
    /*Klockworks Warning Fix Start*/
    if (RLC_NULL != umdRawSduIn_p)
    {
    if(umdRawSduIn_p && umdRawSduIn_p->pRawSdu.szUsed)
    {
        coutSDU += 1;
    }
    }
    /*Klockworks Warning Fix End*/
    if ( coutSDU > 1 )
    {  
        qLoad += GET_VAR_HEADER_SIZE(coutSDU);
    }
    if ( qLoad )
    {    
        lcQLoadArr_p->lcQueueLoad = (qLoad + szFHdr);
    }
    else
    {
    lcQLoadArr_p->lcQueueLoad = 0;

    }

    return (lcQLoadArr_p->lcQueueLoad);
}


/****************************************************************************
 * Function Name  : txOppForUM
 * Inputs         : pE - Pointer to TxUMEntity,
 *                  rlcDataBuffer_p - Pointer to RLCDataBuffer,
 *                  lcQLoadArr_p - Pointer to LCQueueLoad,
 *                  rlcHarqFailureInfo_p - Pointer to RLCHarqFailureInfo
 * Outputs        : None
 * Returns        : errorCode 
 * Description    : This function will do following things:- 
 *                  1. Fetch the TX UM Entity.
 *                  2. Encode the RLC UM PDU.
 *                  3. if encode is successful then update
 *                      a) Update the RLCDataBuffer for MAC Layer
 *                      b) Update the lcQueueLoad structure for MAC Layer
 *                      c) Update the RLCHarqFailureInfo structure
 *                      d) Increment the Sequence Number for TX UM Entity
 *                  4. else in case of failure reset these structure with 0. 
 ****************************************************************************/

static inline UInt32 txOppForUM ( TxUMEntity* pE,
                                  RLCDataBuffer* rlcDataBuffer_p,
                                  LCQueueLoad *lcQLoadArr_p,
                                  RLCHarqFailureInfo *rlcHarqFailureInfo_p
                                )
{
    UInt32 errorCode           = 0;
    UInt32 txOppPduSize        = 0;
    UmdRawPdu pRawPdu; 
    RlcBuffer *rlcBuffer_p     = PNULL;
    UInt32 dfeCount            = 0;
    /* SPR 15909 fix start */
    tickType_t pktArrTime[MAX_RLC_SDU_IN_PDU_FOR_KPI]={0};
    /* SPR 15909 fix end */
#ifndef UE_SIM_TESTING
    UInt8  sduCount            = 0;
/* KPI Changes Start */
#if defined(KPI_STATS) || defined(PERF_STATS)
    UInt8 count = 0;
/* KPI Changes End */
#endif
#endif
    memSet(&pRawPdu,0,sizeof(UmdRawPdu));
    rlcBuffer_p    = &(pRawPdu.pRawPdu);
    RlcQSz szQ     = pE->txQueueSize;
    RlcQSz txQueueSize = szQ;

	/* + PERF_CA GBL */
    InternalCellIndex cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(pE->ueIndex);    
	/* - PERF_CA GBL */
    txOppPduSize = rlcDataBuffer_p->requestedDataSize;

    errorCode = rlcEncodeUmdRawPdu(
                                     pE,
                                     &txOppPduSize,
                                     &szQ,
                                     &pRawPdu,
                                     &dfeCount,
                                     pktArrTime
                                  );
    if ( RLC_SUCCESS == errorCode)
    {
        /* Substract the Qload from TX Entity */
        atomicSubtractInt(&pE->txQueueSize, (txQueueSize - szQ) );
        /* SPR 15447 fix start */
        /* TxQueueSize negative condition handling */
        if (pE->txQueueSize > (pE->rlcQueueSize + 20000))
        {
            pE->txQueueSize = 0;
        }
        /* SPR 15447 fix end */

        UInt32 remQload = rlcUMFetchQLoad(pE, lcQLoadArr_p);
        /* Filling the RLC Data Buffer Structure for RLC PDU to MAC layer */
        rlcDataBuffer_p->newPduSeqNum      = pE->US;
        rlcDataBuffer_p->newPduLen         = txOppPduSize;
        rlcDataBuffer_p->totalPduLen       = txOppPduSize; 
        rlcDataBuffer_p->newRlcPdu_p       = rlcBuffer_p->data_p;
        rlcDataBuffer_p->remainingDataSize = remQload;
        /* Filling the LC Queueload structure for informing the remaining Qload*/
        lcQLoadArr_p->lcQueueLoad          = remQload;
        lcQLoadArr_p->scheduledDataBytes   = rlcBuffer_p->payloadSize;
        lcQLoadArr_p->retxnPduSize         = 0;
        lcQLoadArr_p->ctrlQueueLoad        = 0;
        lcQLoadArr_p->ctrlFlag             = RLC_NORMAL_DATA;
#ifndef UE_SIM_TESTING
        /*RLC sends this info for KPI
         * * 1. packet Delay &
         * * 2. Uu Loss Rate for UM
         * * to MAC in case of UM Mode*/
        rlcDataBuffer_p->sduCount = dfeCount;
        rlcDataBuffer_p->qci = pE->qci;
        for(sduCount = 0; sduCount < dfeCount; sduCount++)
        {
            rlcDataBuffer_p->pktArrTime[sduCount] = pktArrTime[sduCount];
        }
/* KPI Changes Start */
#if defined(KPI_STATS) || defined(PERF_STATS)
        /* Populate individaul PDCP SDU Size info in rlcDataBuffer_p */
        rlcDataBuffer_p->completeEncodedSduCount =  rlcBuffer_p->completeEncodedSduCount;
        for(;count < rlcDataBuffer_p->completeEncodedSduCount; count++)
        {
            rlcDataBuffer_p->pdcpSduSize[count] = rlcBuffer_p->pdcpSduSize[count];
        }
#endif
#endif
        /* Updating Harq failure Information */
        rlcHarqFailureInfo_p->harqFailureReporting = 0;

        /* Increment the Sequence Number */
        pE->US = ( pE->US + 1) & ( pE->modValForWrapRound) ;

        /* Stats updated for the TX UMD PDU to MAC Layer */
		/* + PERF_CA GBL */
        lteRlcStatInfo_g[cellIndex].lteUMStat.txUmdPdu++;
        lteRlcStatInfo_g[cellIndex].lteUEStat.rlcUeStats[pE->ueIndex].
            pduTransmittedToMac++;
		/* - PERF_CA GBL */
        LteRlcLCStats* lcStats_p =
            &(gRlcStats.pRlcStats->lteRlcUeStat[pE->ueIndex].
                    lcStats[pE->lcId]);
        lcStats_p->pduBytesDl += txOppPduSize;
        lcStats_p->pduNumDl++;
        lcStats_p->dlThroughput += txOppPduSize;
        gRlcStats.pRlcStats->lteRlcUeStat[pE->ueIndex].dlThrput += txOppPduSize;
        gRlcStats.pRlcStats->lteSysStat.sysDlThroughput += txOppPduSize;

        /* 16Feb - MPSD T3300 changes for RLC queue check start */
        /* 16Feb - changes for RLC queue check start */
        LOG_RLC_MSG( LTE_RLC_UM_TX_OPP_ERR, LOGINFO, RLC_TX, 
                globalTTITickCount_g, __LINE__, pE->ueIndex,
                pE->lcId, pE->txQueueSize,  pE->US -1, txOppPduSize,
                0, __func__, "");
    }
    else
    {
        /* Filling the RLC Data Buffer Structure for RLC PDU to MAC layer */
        rlcDataBuffer_p->newRlcPdu_p    = RLC_NULL ;
        rlcDataBuffer_p->newPduLen      = 0 ;

        /* Filling the LC Queueload structure for informing the remaining Qload*/
        lcQLoadArr_p->lcQueueLoad        = 0;
        lcQLoadArr_p->scheduledDataBytes = 0;
        lcQLoadArr_p->retxnPduSize       = 0;
        lcQLoadArr_p->ctrlQueueLoad      = 0;
        lcQLoadArr_p->ctrlFlag           = RLC_NORMAL_DATA;
        lcQLoadArr_p->headPktTS          = 0;

        /* Updating Harq failure Information */
        rlcHarqFailureInfo_p->lcId = pE->lcId;
        rlcHarqFailureInfo_p->harqFailureReporting = 0;

        msgFree(rlcBuffer_p->data_p);
        /* SPR 20985 fix start */
        rlcBuffer_p->data_p = RLC_NULL;
        /* SPR 20985 fix end */
        LOG_RLC_MSG( LTE_RLC_UM_TX_OPP_ERR_1, LOGWARNING , RLC_TX, 
                globalTTITickCount_g, __LINE__, pE->ueIndex,
                pE->lcId, pE->txQueueSize, txOppPduSize, 0,
                0, __func__, "");
    }
    /* + SPR 20690 */
#ifndef UE_SIM_TESTING
    /* Fill SPS information in BO Request if SPS LC is TRUE */
    if( LTE_TRUE == pE->isSpsLc )
    {
        /* In case of Empty RAW SDU Queue or UmdRawSdu_p is NULL
         * then reset the below parameters */
        lcQLoadArr_p->ipPktType        = 0;
        lcQLoadArr_p->voiceSilenceType = 0;
        lcQLoadArr_p->headPktQueueLoad = 0;
        lcQLoadArr_p->headPktTS        = 0;
        lcQLoadArr_p->avgRunningSduSize = pE->prevAvgRunningSduSize;

        /* Check if any node present in SDU queue */
        sduCount = QUEUE_COUNT_UMD_RAW_SDU_Q(pE->txSDUQueue); 
        if(sduCount > 0)
        {
            RlcUeDataReq *UmdRawSdu_p = PNULL;
            /* get the first node from SDU Queue to fill the SPS info */
		    getNodeCircQ( pE->txSDUQueue.pQ , (void **)&UmdRawSdu_p );
            if(UmdRawSdu_p)
            {
                lcQLoadArr_p->ipPktType = UmdRawSdu_p->ipPktType;
                lcQLoadArr_p->voiceSilenceType = UmdRawSdu_p->voiceSilenceType;
                lcQLoadArr_p->headPktQueueLoad  = UmdRawSdu_p->size;
                lcQLoadArr_p->headPktTS         = UmdRawSdu_p->pktArrTime;
                lcQLoadArr_p->avgRunningSduSize = UmdRawSdu_p->avgSduSize;

                /* Store the Avg SDU received in RLC context */
                pE->prevAvgRunningSduSize = lcQLoadArr_p->avgRunningSduSize;
            }
        }
        /* Fill RlcPduInd flag to MAC for SPS scheduler */
        lcQLoadArr_p->numOfSdu = QUEUE_COUNT_UMD_RAW_SDU_Q(pE->txSDUQueue);
        lcQLoadArr_p->RLCPduInd = RLC_NO_IND;
    }
#endif
    /* - SPR 20690 */

    return errorCode;
}

/****************************************************************************
 * Function Name  : txOppForAM
 * Inputs         : amEntity_p - Pointer to AMEntity,
 *                  rlcDataBuffer_p - Pointer to RLCDataBuffer,
 *                  lcQLoadArr_p - Pointer to LCQueueLoad,
 *                  rlcHarqFailureInfo_p - Pointer to RLCHarqFailureInfo
 * Outputs        : None
 * Returns        : errorCode
 * Description    : This function will do following things :-
 *                  1. Call Function for Handle TX Opp. for AM Mode.
 *                  2. If above function is successful then update
 *                     a) Update the parametr ctrlFlag, remainingDataSize
 *                        of LCQueueLoad and RLCDataBuffer
 *                     b) update the rlc stats.
 *
 ****************************************************************************/

static inline UInt32 txOppForAM ( AMEntity* amEntity_p,
                                  RLCDataBuffer* rlcDataBuffer_p,
                                  LCQueueLoad *lcQLoadArr_p,
                                  RLCHarqFailureInfo *rlcHarqFailureInfo_p
                                )
{
    UInt32 errorCode   = INVALID_ERR_CODE;
    TxAMEntity* pAmTxE = amEntity_p->txAMEntity_p;
	/* + PERF_CA GBL */
    InternalCellIndex cellIndex = RLC_FETCH_INTERNAL_CELL_INDEX(pAmTxE->ueIndex);    
	/* - PERF_CA GBL */

    errorCode =  rlcHandleTxOppForAMEntity( amEntity_p,
            rlcDataBuffer_p,
            lcQLoadArr_p,
            rlcHarqFailureInfo_p
            );

    
    if ( RLC_SUCCESS == errorCode )
    {
        lcQLoadArr_p->ctrlFlag = RLC_NORMAL_DATA;
        rlcFetchQueueLoad ( pAmTxE, lcQLoadArr_p);
        rlcDataBuffer_p->remainingDataSize =    lcQLoadArr_p->lcQueueLoad 
            + lcQLoadArr_p->retxnPduSize 
            + lcQLoadArr_p->ctrlQueueLoad;
        /* Updating the stats */
        lteRlcStatInfo_g[cellIndex].lteAMStat.amdPduTransmitted++;
        lteRlcStatInfo_g[cellIndex].lteUEStat.rlcUeStats[pAmTxE->ueIndex].
            pduTransmittedToMac++;
    //    rlcDataBuffer_p->totalPduLen = (rlcDataBuffer_p->statusPduLen + rlcDataBuffer_p->retxPduLen 
      //                                 + rlcDataBuffer_p->newPduLen);
    }
    else
    {
        /*
        ** In case of validation failed due to SN lies outside
        ** window for new PDU encoding, then send
        ** RLC_WAIT_FOR_STATUS_PDU status to MAC in BO update.
        */
        if (LTE_RLC_VALIDATION_FAILED_SN_OUTSIDE_TX_WINDOW == errorCode)
        {
            lcQLoadArr_p->ctrlFlag           = RLC_WAIT_FOR_STATUS_PDU;
        }
        else
        {
            lcQLoadArr_p->ctrlFlag           = RLC_NORMAL_DATA;
        }
        /* End 128 UE: 25% NACK Changes */

        if( OUT_OF_WINDOW != errorCode )
        {
            rlcFetchQueueLoad(pAmTxE, lcQLoadArr_p);
        }
        else
        {
            lcQLoadArr_p->lcQueueLoad        = 0;
            lcQLoadArr_p->retxnPduSize       = 0;
            lcQLoadArr_p->ctrlQueueLoad      = 0;
        }

        rlcDataBuffer_p->remainingDataSize = (  lcQLoadArr_p->lcQueueLoad 
                                                + lcQLoadArr_p->retxnPduSize
                                                + lcQLoadArr_p->ctrlQueueLoad
                                             );
        LOG_RLC_MSG( LTE_RLC_AM_TX_OPP_ERR, LOGWARNING, RLC_TX, 
                globalTTITickCount_g, __LINE__, pAmTxE->ueIndex,
                pAmTxE->lcId,errorCode,0, 0, 0, __func__, "");
    }

    return errorCode;
}

/****************************************************************************
 * Function Name  : validateRLCEntity 
 * Inputs         : ueContext_p - Pointer to UEContext,
 *                  lcId  - Logical channel Id,
 *                  szPdu -  Size of PDU,
 *                  entityInfo_p - Pointer to RLCEntityInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/errorcode
 * Description    : This function will do following validations :-
 *                  1. Check for lcId is valid
 *                  2. Validate the Entity exist and in Active State
 *                  3. Validate that txOpp should be greater than 1
 *                  4. Validate UM or AM Entity Exist.
 ****************************************************************************/

static inline  UInt32 validateRLCEntity (  UEContext* ueContext_p,
                                           UInt8  lcId,
                                           UInt32 szPdu,
                                           RLCEntityInfo **entityInfo_p
                                         )
{
    RLCEntityInfo* eInfo_p = RLC_NULL;
    /*set the value with NULL */
    *entityInfo_p  = RLC_NULL;   

    if ( lcId >= MAX_LC_ID )
    {
        LOG_RLC_MSG( LTE_RLC_TX_OPP_VALIDATE_1, LOGWARNING, RLC_TX, 
                globalTTITickCount_g, __LINE__, ueContext_p->ueId,
                lcId, szPdu, 0, 0, 0, __func__, "");
                
        return LTE_RLC_ERR_INVALID_LCID;
    }
    /*fetch entity information*/
    eInfo_p = DB_FETCH_RLC_ENTITY_INFO(ueContext_p, lcId);
    if((eInfo_p == RLC_NULL)
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
            || (eInfo_p->rlcEntityState != RLC_ENTITY_ACTIVE)
#endif    
       )
    {
        LOG_RLC_MSG( LTE_RLC_TX_OPP_VALIDATE_2, LOGWARNING, RLC_TX, 
                globalTTITickCount_g, __LINE__, ueContext_p->ueId, 
                lcId, szPdu, 0, 0, 0, __func__, "");
                
        return LTE_RLC_ERR_INVALID_ENTITY;
    }
    
    if ( 1 >= szPdu )
    {
        LOG_RLC_MSG( LTE_RLC_TX_OPP_VALIDATE_3, LOGWARNING, RLC_TX, 
                globalTTITickCount_g, __LINE__, ueContext_p->ueId,
                lcId, szPdu, 0, 0, 0, __func__, "");
                
        return LTE_RLC_ERR_INVALID_MAC_OPP;
    }
    
    if ( RLC_ENTITY_UM == eInfo_p->rlcEntityMode)
    {
       UMEntity* umEntity_p = eInfo_p->rlcEntity.umEntity_p;
       if (( umEntity_p == RLC_NULL ) ||
                   (umEntity_p->txUMEntity_p == RLC_NULL))
       {
           LOG_RLC_MSG( LTE_RLC_TX_OPP_VALIDATE_4, LOGWARNING, RLC_TX, 
                   globalTTITickCount_g, __LINE__, ueContext_p->ueId,
                   lcId, szPdu, 0, 0, 0, __func__, "");
                   
           return LTE_RLC_ERR_INVALID_TX_ENTITY;
       }
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
       if(umEntity_p->txEntityState != RLC_ENTITY_ACTIVE)
       {
           LOG_RLC_MSG( LTE_RLC_TX_OPP_VALIDATE_2, LOGWARNING, RLC_TX,
                globalTTITickCount_g, __LINE__, ueContext_p->ueId,
                lcId, szPdu, 0, 0, 0, __func__, "");
           return LTE_RLC_ERR_INVALID_ENTITY;
       }
#endif       
    }
    else if ( RLC_ENTITY_AM == eInfo_p->rlcEntityMode )
    {
        AMEntity* amEntity_p = eInfo_p->rlcEntity.amEntity_p;
        if ( (!amEntity_p) || !(amEntity_p->txAMEntity_p) 
                || !(amEntity_p->rxAMEntity_p) )
        {
           LOG_RLC_MSG( LTE_RLC_TX_OPP_VALIDATE_5, LOGWARNING, RLC_TX, 
                   globalTTITickCount_g, __LINE__, ueContext_p->ueId,
                   lcId, szPdu, 0, 0, 0, __func__, "");
                   
            return LTE_RLC_ERR_INVALID_RX_ENTITY;
        } 
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
       if(amEntity_p->txEntityState != RLC_ENTITY_ACTIVE)
       {
#ifdef DEBUG_STATS
           amEntity_p->txAMEntity_p->InValidInEntity++;
#endif
           LOG_RLC_MSG( LTE_RLC_TX_OPP_VALIDATE_2, LOGWARNING, RLC_TX,
                globalTTITickCount_g, __LINE__, ueContext_p->ueId,
                lcId, szPdu, 0, 0, 0, __func__, "");
           return LTE_RLC_ERR_INVALID_ENTITY;
       }
#endif       
    }
    *entityInfo_p = eInfo_p;
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : resetRlcDataBuffer 
 * Inputs         : rlcDataBuffer_p - Pointer to RLCDataBuffer,
 *                  lcCount - Count of LC's
 * Outputs        : None
 * Returns        : None
 * Description    : This function will reset the RLCDataBuffer and free the
 *                  memory of rlcDataBuffer_p.
 ****************************************************************************/

inline static void resetRlcDataBuffer( 
                                       RLCDataBuffer* rlcDataBuffer_p,
                                       UInt8 lcCount
                                     )
{
    UInt32 lIdx = 0;
    UInt32 reTxPduCount = 0;

    for( lIdx = 0; lIdx < lcCount ; lIdx++ )
    {
        msgFree(rlcDataBuffer_p[lIdx].statusPdu_p);
        msgFree(rlcDataBuffer_p[lIdx].newRlcPdu_p);
        rlcDataBuffer_p[lIdx].statusPdu_p  = RLC_NULL;
        rlcDataBuffer_p[lIdx].newRlcPdu_p  = RLC_NULL;
        rlcDataBuffer_p[lIdx].statusPduLen = 0;
        rlcDataBuffer_p[lIdx].newPduLen   = 0;
        rlcDataBuffer_p[lIdx].newPduSeqNum = INVALID_SEQ_NUM;
        /** SPR 3657 Changes Start **/
        reTxPduCount = QUEUE_COUNT_AMD_RETX_PDU_Q(rlcDataBuffer_p[lIdx].reTxRlcPduQ);
        if(reTxPduCount)
        {
            rlcReTxPduNode *rlcReTxPduNode_p = PNULL;
            DEQUEUE_AMD_RETX_PDU_Q(rlcReTxPduNode_p,rlcDataBuffer_p[lIdx].reTxRlcPduQ);
            while( rlcReTxPduNode_p && reTxPduCount--)
            {
                msgFree(rlcReTxPduNode_p->retxRlcPdu_p);
                rlcReTxPduNode_p->retxRlcPdu_p = RLC_NULL;
                freeMemPool(rlcReTxPduNode_p);
                rlcReTxPduNode_p = RLC_NULL;
                DEQUEUE_AMD_RETX_PDU_Q(rlcReTxPduNode_p,rlcDataBuffer_p[lIdx].reTxRlcPduQ);
            }
        }
        /*SPR 51542 Start*/
        semDestroy(&rlcDataBuffer_p[lIdx].reTxRlcPduQ.sem_lock);
        /*SPR 51542 End*/
        /*  msgFree(rlcDataBuffer_p[lIdx].retxRlcPdu_p);
            rlcDataBuffer_p[lIdx].retxRlcPdu_p = RLC_NULL;
            rlcDataBuffer_p[lIdx].retxPduLen   = 0;
            rlcDataBuffer_p[lIdx].retxPduSeqNum = INVALID_SEQ_NUM; */
        /** SPR 3657 Changes End **/
       
    }
    freeMemPool(rlcDataBuffer_p);

    return;
}

/****************************************************************************
 * Function Name  : resetLcQLoad
 * Inputs         : lcQLoadArr_p - Pointer to LCQueueLoad
 * Outputs        : None
 * Returns        : None
 * Description    : This function will reset the LcQueueLoad structure.
 ****************************************************************************/

inline static void resetLcQLoad ( LCQueueLoad *lcQLoadArr_p )
{
    lcQLoadArr_p->lcQueueLoad          = 0;
    lcQLoadArr_p->scheduledDataBytes   = 0;
    lcQLoadArr_p->retxnPduSize         = 0;
    lcQLoadArr_p->ctrlQueueLoad        = 0;
    lcQLoadArr_p->ctrlFlag             = RLC_NORMAL_DATA;


}

/****************************************************************************
 * Function Name  : rlcHandleTxOppurtunity
 * Inputs         : currentTick - current TTI,
 *                  internalCellIndex
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE/errorCode
 * Description    : This function will do following things:-
 *                  1. Dequeue the node for tx Opp given by MAC Layer
 *                  2. Check if its Dummy Node then send the data to
 *                     MAC Layer as dummy
 *                  3. Fetch & Validate the UE Context
 *                  4. Fetch & Validate the RLC Entity
 *                  5. Check if RLC EntityMode is UM or AM and call the 
 *                     corresponding functions
 *                  6. After encode (using previous step 5) then given the
 *                     data to MAC Layer by calling "macUeDataReq" func
 *                  7. Update the remaing Qload (for above RLC Entities) to 
 *                     MAC Layer by calling "macUpdateBufferOccupancyReq" func.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
/** CA Changes Start **/
#if !defined (DL_DATA_SEPARATION) || defined (UE_SIM_TESTING)
UInt32 rlcHandleTxOppurtunity(InternalCellIndex internalCellIndex) 
#else
/* SPR 15909 fix start */
UInt32 rlcHandleTxOppurtunity
         (tickType_t currentTick,InternalCellIndex internalCellIndex) 
/* SPR 15909 fix end */
#endif
/** CA Changes End **/
/* SPR 5599_129119 Fix End */
/* + coverity 55316 */
{

    MacUeOpporInd *ueOppInd_p      = RLC_NULL;
    RLCDataBuffer* rlcDataBuffer_p = RLC_NULL;
    UEContext* ueContext_p         = RLC_NULL;
    RLCEntityInfo* eInfo_p         = RLC_NULL;
    UInt8 lcCount                  = 0;

    UInt8 requestFailure           = 0;
    UInt8 lIdx                     = 0;
    UInt32 errorCode               = INVALID_ERR_CODE;
    LCQueueLoad lcQLoadArr_p[MAX_LC_ID];
    /* SPR 18192 fix - start*/
    memSet(lcQLoadArr_p,0,(sizeof(LCQueueLoad) * MAX_LC_ID));
    /* SPR 18192 fix - end*/
    /* + Coverity_Stack Fix */
    //RLCHarqFailureInfo rlcHarqFailureInfo[MAX_LC_ID] = {{0,0}};
    RLCHarqFailureInfo  *rlcHarqFailureInfo = RLC_NULL;

#if defined (OVERSHOOT_CORRECTION)&& !defined (UE_SIM_TESTING)
    /*TDD Tick overshoot changes start*/
#ifdef FDD_CONFIG
    UInt64 rem_cycles = 0;
    UInt64 cyclesReqd = 0;
#endif
    /*TDD Tick overshoot changes end*/
    /* FDD_Warning_Fix */
#endif

/* SPR 8712 changes */
#if (defined(DL_DATA_SEPARATION) || defined(UE_SIM_TESTING)) 
    DEQUEUE_MAC_UE_OPPOR_IND_CIRC_Q(MacUeOpporInd, (void *)&ueOppInd_p, internalCellIndex);
#else
#ifdef DL_UL_SPLIT 
    while(1)
    {
#ifdef CIRC_QUEUE_IMPL
        /* CA Changes start */
        semWaitDefault(&waitForMacUeOpporInd_g[internalCellIndex]);
        DEQUEUE_MAC_UE_OPPOR_IND_CIRC_Q(MacUeOpporInd, (void *)&ueOppInd_p, internalCellIndex);
#else
        recvMsgWithIccQ(macUeOppnIndQIdUL_g[internalCellIndex], (const Char8*)&ueOppInd_p);/*SPR 17777 fix +-*/
#endif
#else
        /* Dequeue the MAC TX OPPORTUNITY Node */
        /*coverity 55316 - Indentation corrected*/
        DEQUEUE_MAC_UE_OPPOR_IND_CIRC_Q(MacUeOpporInd, (void *)&ueOppInd_p, internalCellIndex);
        /*coverity 55316*/
        /* CA Changes end */
#endif
#endif
/* SPR 8712 changes */

        if ( ueOppInd_p == RLC_NULL ) 
        {

#ifndef LINUX_PC_TEST
            LOG_RLC_MSG( LTE_RLC_TX_OPP_1,LOGWARNING , RLC_TX, 
                    globalTTITickCount_g, __LINE__, 0, 0, 0, 0,
                    0, 0, __func__, "");
#endif
/* SPR 8712 changes */
#if (defined (DL_UL_SPLIT) && !defined(UE_SIM_TESTING) && !defined(DL_DATA_SEPARATION))
/* SPR 8712 changes */
            continue;
#else
            return LTE_RLC_UE_OPP_IND_NULL;
#endif
        }

        /* Fetch the information from the Node. */
        lcCount         = ueOppInd_p->lcCount ;
        rlcDataBuffer_p = ueOppInd_p->rlcBuffer_p ;

        /* Dummy Node handling for Encoder in MAC layer*/
        /* SPR 4010 Fix Start */
        if ((MAX_UE_ID == ueOppInd_p->ueIndex) || 
                ( 0 == lcCount))
            /* SPR 4010 Fix End */
        {
            macUeDataReq ( ueOppInd_p->ueIndex,
                    ueOppInd_p->lcCount,
                    rlcDataBuffer_p,
                    requestFailure,
            /** SPR 13599 Changes Start **/
                    rlcHarqFailureInfo_g
            /** SPR 13599 Changes End **/
#ifndef UE_SIM_TESTING
#ifdef DL_DATA_SEPARATION
                    ,currentTick
#endif
#endif
                    ,internalCellIndex     
                    ); 
            FREE_MEM_NODE_MAC_UE_OPPOR_IND_CIRC_Q(ueOppInd_p);

            return LTE_RLC_PUSH_DUMMY_NODE;
        }
        rlcHarqFailureInfo = (RLCHarqFailureInfo *)getMemFromPool( 
            (MAX_LC_ID * sizeof(RLCHarqFailureInfo)), PNULL);

        if(PNULL == rlcHarqFailureInfo)
        {
            lteWarning("In %s, MEMORY ALLOC FAIL\n",__func__);
            return RLC_FAILURE;
        }
#if defined (OVERSHOOT_CORRECTION)&& !defined (UE_SIM_TESTING)
        /*TDD Tick overshoot changes start*/
#ifdef FDD_CONFIG
        GET_PROCESSING_CYCLES_LEFT(rem_cycles);
        /** CA2 Changes Start **/
        /* compilation 10-JUN-2014 fix start */
#ifdef DL_UL_SPLIT
        MIN_CC_REQD_TO_PROCEED(cyclesReqd);
#else
        MIN_CC_REQD_TO_PROCEED(cyclesReqd,internalCellIndex);
#endif
        /* compilation 10-JUN-2014 fix end */
        /** CA2 Changes End **/

        /* Dummy Node handling for Encoder in MAC layer*/
        if (rem_cycles < cyclesReqd)     
        {
            for ( lIdx = 0; lIdx < lcCount; lIdx++)
            {
                /** SPR 13599 Changes Start **/
                rlcHarqFailureInfo[lIdx].lcId = 0;
                rlcHarqFailureInfo[lIdx].newPduSeqNum = 0;
                rlcHarqFailureInfo[lIdx].reTxPduCount = 0;
                /** SPR 13599 Changes End **/
                rlcDataBuffer_p[lIdx].statusPdu_p  = RLC_NULL;
                rlcDataBuffer_p[lIdx].retxRlcPdu_p = RLC_NULL;
                rlcDataBuffer_p[lIdx].newRlcPdu_p  = RLC_NULL;
                /*SPR 7289 Fix Start*/
                queueInit(&rlcDataBuffer_p[lIdx].reTxRlcPduQ);
                /* + SPR 9646 Fix */
                /* No need of semaphore due to single thread operation*/
                semDestroy((LTE_SEM*)&(rlcDataBuffer_p[lIdx].reTxRlcPduQ.sem_lock));
                /*SPR 7289 Fix End*/
                /* - SPR 9646 Fix */
                rlcDataBuffer_p[lIdx].statusPduLen = 0;
                rlcDataBuffer_p[lIdx].retxPduLen   = 0;
                rlcDataBuffer_p[lIdx].newPduLen    = 0;
                rlcDataBuffer_p[lIdx].totalPduLen  = 0;
                rlcDataBuffer_p[lIdx].retxPduSeqNum = INVALID_SEQ_NUM;
                rlcDataBuffer_p[lIdx].newPduSeqNum  = INVALID_SEQ_NUM;
#ifndef UE_SIM_TESTING
                rlcDataBuffer_p[lIdx].qci          = 0;
                rlcDataBuffer_p[lIdx].sduCount     = 0;
#endif
                /* Rel 5.3.1: Memory related changes start*/
#if defined(KPI_STATS) || defined(PERF_STATS)
                rlcDataBuffer_p[lIdx].completeEncodedSduCount  = 0;
#endif                
                /* Rel 5.3.1: Memory related changes End*/
            }


            macUeDataReq ( ueOppInd_p->ueIndex,
                    ueOppInd_p->lcCount,
                    rlcDataBuffer_p,
                    requestFailure,
                    rlcHarqFailureInfo
#ifndef  UE_SIM_TESTING
#ifdef DL_DATA_SEPARATION
                    ,currentTick
#endif               
#endif               
                    ,internalCellIndex 
                    );

            macUpdateBufferOccupancyReq( ueOppInd_p->ueIndex,
                    ueOppInd_p->lcCount,
                    lcQLoadArr_p,
                    RLC_BO_TYPE_UPDATE,
                    /* SPR 20785 fix start */
                    internalCellIndex
                    /* SPR 20785 fix end */
                    );

            noTimeToProcessflag_g = 1;
            FREE_MEM_NODE_MAC_UE_OPPOR_IND_CIRC_Q(ueOppInd_p);

/* SPR 8712 changes */
#if (defined (DL_UL_SPLIT) && !defined(DL_DATA_SEPARATION))
/* SPR 8712 changes */
            currTickOverLoadUL_g = OVERLOAD_LEVEL_CRITICAL ;
            /** CA-TDD Changes Stop **/
            /*- Dyn UEs To Be SchPerTTI -*/
            continue;

#else
            currTickOverLoad_g[internalCellIndex] = OVERLOAD_LEVEL_CRITICAL ;
            LOG_RLC_MSG( LTE_RLC_TX_LD_CHANGE, LOGWARNING, RLC_TX, 
                    /** CA-TDD Changes Start **/
                    globalTTITickCount_g, __LINE__,currTickOverLoad_g[internalCellIndex], macOverLoadLevel_g[internalCellIndex],
                    /*+ Dyn UEs To Be SchPerTTI +*/
                    dlUeSchdIndxInCurrTick_g, ulUeSchdIndxInCurrTick_g , 0, 0, __func__, "");
            /** CA-TDD Changes Stop **/
                    /*- Dyn UEs To Be SchPerTTI -*/

            /* + Coverity_Stack Fix */
            freeMemPool(rlcHarqFailureInfo);
            /* - Coverity_Stack Fix */
            return LTE_RLC_PUSH_DUMMY_NODE; 

#endif

        }
#endif
#endif
        /*TDD Tick overshoot changes end*/


        /*fetch ueContext*/
        if((ueContext_p = 
                    DB_FETCH_UE_CONTEXT(ueOppInd_p->ueIndex)) == RLC_NULL 
                || RLC_UECONTEXT_ACTIVE != ueContext_p->rlcUEState)
        {
            /*SPR 16550 Fix Start */
            for ( lIdx = 0; lIdx < lcCount; lIdx++)
            {
                lcQLoadArr_p[lIdx].lcId       = rlcDataBuffer_p[lIdx].lcID ;                          
                rlcDataBuffer_p[lIdx].statusPdu_p  = RLC_NULL;
                rlcDataBuffer_p[lIdx].retxRlcPdu_p = RLC_NULL;
                rlcDataBuffer_p[lIdx].newRlcPdu_p  = RLC_NULL;
                /*SPR 7289 Fix Start*/
                queueInit(&rlcDataBuffer_p[lIdx].reTxRlcPduQ);
                /* + SPR 9646 Fix */
                /* No need of semaphore due to single thread operation*/
                semDestroy((LTE_SEM*)&(rlcDataBuffer_p[lIdx].reTxRlcPduQ.sem_lock));
                /*SPR 7289 Fix End*/
                /* - SPR 9646 Fix */
                rlcDataBuffer_p[lIdx].statusPduLen = 0;
                rlcDataBuffer_p[lIdx].retxPduLen   = 0;
                rlcDataBuffer_p[lIdx].newPduLen    = 0;
                rlcDataBuffer_p[lIdx].totalPduLen  = 0;
                rlcDataBuffer_p[lIdx].retxPduSeqNum = INVALID_SEQ_NUM;
                rlcDataBuffer_p[lIdx].newPduSeqNum  = INVALID_SEQ_NUM;
#ifndef UE_SIM_TESTING
                rlcDataBuffer_p[lIdx].qci          = 0;
                rlcDataBuffer_p[lIdx].sduCount     = 0;
#endif
                /* Rel 5.3.1: Memory related changes start*/
#if defined(KPI_STATS) || defined(PERF_STATS)
                rlcDataBuffer_p[lIdx].completeEncodedSduCount  = 0;
#endif                
                /* Rel 5.3.1: Memory related changes End*/
            }
            macUeDataReq ( ueOppInd_p->ueIndex,
                    ueOppInd_p->lcCount,
                    rlcDataBuffer_p,
                    requestFailure,
                    rlcHarqFailureInfo
#if (defined(DL_DATA_SEPARATION) && !(defined(UE_SIM_TESTING)))
                    ,currentTick
#endif             
                    ,internalCellIndex 
                    );

            macUpdateBufferOccupancyReq( ueOppInd_p->ueIndex,
                    ueOppInd_p->lcCount,
                    lcQLoadArr_p,
                    RLC_BO_TYPE_UPDATE,
                    internalCellIndex
                    );
            /*SPR 16550 Fix End */

            LOG_RLC_MSG( LTE_RLC_TX_OPP_2, LOGWARNING, RLC_TX, 
                    globalTTITickCount_g, __LINE__, ueOppInd_p->ueIndex,
                    0, 0, 0, 0, 0, __func__, "");
            FREE_MEM_NODE_MAC_UE_OPPOR_IND_CIRC_Q(ueOppInd_p);

/* SPR 8712 changes */
#if (defined (DL_UL_SPLIT) && !defined(UE_SIM_TESTING) && !defined(DL_DATA_SEPARATION))
/* SPR 8712 changes */
            continue;
#else
            /* + Coverity_Stack Fix */
            freeMemPool(rlcHarqFailureInfo);
            /* - Coverity_Stack Fix */
            return RLC_TX_CTX_NOT_FOUND;
#endif
        }

        /* Fetch Entity context using LCID and UE Idx. */
        for ( lIdx = 0; lIdx < lcCount; lIdx++)
        {
            /** SPR 13599 Changes Start **/
            rlcHarqFailureInfo[lIdx].lcId = 0;
            rlcHarqFailureInfo[lIdx].newPduSeqNum = 0;
            rlcHarqFailureInfo[lIdx].reTxPduCount = 0;
            /** SPR 13599 Changes End **/
            RlcLcId lcId  = rlcDataBuffer_p[lIdx].lcID ;                          
            UInt32  txOpp = rlcDataBuffer_p[lIdx].requestedDataSize;
            rlcDataBuffer_p[lIdx].statusPdu_p  = RLC_NULL;
            rlcDataBuffer_p[lIdx].newRlcPdu_p  = RLC_NULL;
            /*SPR 7289 Fix Start*/
            queueInit(&rlcDataBuffer_p[lIdx].reTxRlcPduQ);
            /* + SPR 9646 Fix */
            /* No need of sem due to single thread operation*/
            semDestroy((LTE_SEM*)&(rlcDataBuffer_p[lIdx].reTxRlcPduQ.sem_lock));
            /* - SPR 9646 Fix */
            /*SPR 7289 Fix End*/
            /** SPR 3657 Changes Start **/
            rlcDataBuffer_p[lIdx].retxRlcPdu_p = RLC_NULL;
            rlcDataBuffer_p[lIdx].retxPduSeqNum = INVALID_SEQ_NUM;
            rlcDataBuffer_p[lIdx].retxPduLen   = 0;
            /** SPR 3657 Changes End **/

            rlcDataBuffer_p[lIdx].statusPduLen = 0;
            rlcDataBuffer_p[lIdx].newPduLen    = 0;
            rlcDataBuffer_p[lIdx].totalPduLen  = 0;
            rlcDataBuffer_p[lIdx].newPduSeqNum  = INVALID_SEQ_NUM;
#ifndef UE_SIM_TESTING
            rlcDataBuffer_p[lIdx].qci          = 0;
            rlcDataBuffer_p[lIdx].sduCount     = 0;
            //rlcDataBuffer_p[lIdx].pktArrTime[MAX_RLC_SDU_IN_PDU_FOR_KPI] = {0};
#endif
            /* Rel 5.3.1: Memory related changes start*/
#if defined(KPI_STATS) || defined(PERF_STATS)
            rlcDataBuffer_p[lIdx].completeEncodedSduCount  = 0;
#endif                
            /* Rel 5.3.1: Memory related changes End*/
            lcQLoadArr_p[lIdx].lcId       = lcId;
#ifndef UE_SIM_TESTING
#ifdef SSI_DEBUG_STATS
            /* Update SHM stats for opportunity indication */
            /* CA Stats Changes Start */
            gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                qoSUELCStat[ueOppInd_p->ueIndex].qoSLCPerTTIStat[lcId].\
                STX[WRITE_TTI_INDEX(internalCellIndex)].OppGranted = txOpp;
            /* CA Stats Changes End */
#endif        
#endif
            /*verify the lcid & txOPP and return Entity */
            errorCode = validateRLCEntity(ueContext_p, lcId, txOpp, &eInfo_p);
            if ( PNULL == eInfo_p || txOpp > 19000)
            {
                LOG_RLC_MSG( LTE_RLC_TX_OPP_5, LOGWARNING , RLC_TX, 
                        globalTTITickCount_g, __LINE__, ueContext_p->ueId,
                        lcId, txOpp , errorCode, 0, 0, __func__, "");
                resetLcQLoad ( &lcQLoadArr_p[lIdx]);
            #ifdef LINUX_PC_TEST
            extern void rlcSetRetErrorMsgFlag(UInt8 Errorstatus);
            rlcSetRetErrorMsgFlag(RLC_TRUE);
            extern void adapter_rrc_max_retx_msg(UInt16 ueIdx, UInt16 lcId);
            adapter_rrc_max_retx_msg(ueOppInd_p->ueIndex, lcId);
            #endif
                continue;
            }

            rlcHarqFailureInfo[lIdx].lcId = lcId;
            /* Check whether Entity Mode is UM Mode or AM Mode */
            switch ( eInfo_p->rlcEntityMode )
            {
                case RLC_ENTITY_UM:

                errorCode = txOppForUM( (eInfo_p->rlcEntity.umEntity_p->txUMEntity_p), 
                        &rlcDataBuffer_p[lIdx], 
                        &lcQLoadArr_p[lIdx], 
                        &rlcHarqFailureInfo[lIdx]
                        );
                break;

                case RLC_ENTITY_AM:

                errorCode = txOppForAM( (eInfo_p->rlcEntity.amEntity_p),
                        &rlcDataBuffer_p[lIdx], 
                        &lcQLoadArr_p[lIdx], 
                        &rlcHarqFailureInfo[lIdx]
                        );
                break;

                default:
                LOG_RLC_MSG( LTE_RLC_TX_OPP_3, LOGWARNING, RLC_TX, 
                        globalTTITickCount_g, __LINE__, ueContext_p->ueId, lcId, txOpp,
                        0, 0, 0, __func__, "");

                break;
            }
            /*coverity_32090*/
            if(errorCode == RLC_SUCCESS)
            {
                /* SPR 4391 changes start*/
                LOG_RLC_MSG( LTE_RLC_AM_STATUS_PDU_3, LOGINFO, RLC_TX,
                        globalTTITickCount_g, __LINE__, errorCode,
                        0, 0, 0, 0,0, __func__, "");	   
                /* SPR 4391 changes end*/
            }
            /*coverity_32090*/
        }    


        if( RLC_SUCCESS != ( macUeDataReq ( ueOppInd_p->ueIndex, 
                        ueOppInd_p->lcCount,
                        rlcDataBuffer_p,
                        requestFailure,
                        rlcHarqFailureInfo
#ifndef UE_SIM_TESTING 
#ifdef DL_DATA_SEPARATION
                        ,currentTick
#endif               
#endif
                        ,internalCellIndex 
                        )))
        {
            resetRlcDataBuffer( rlcDataBuffer_p, lcCount);
            LOG_RLC_MSG( LTE_RLC_TX_OPP_4, LOGWARNING, RLC_TX, 
                    globalTTITickCount_g, __LINE__, ueContext_p->ueId, 
                    ueOppInd_p->lcCount, 0, 0, 0, 0, __func__, "");

        }
        /*notify mac for remaing QueueLoad*/
        macUpdateBufferOccupancyReq( ueOppInd_p->ueIndex,
                ueOppInd_p->lcCount,
                lcQLoadArr_p,
                /** SPR 13599 Changes Start **/
                RLC_BO_TYPE_UPDATE,
                /* SPR 20785 fix start */
                internalCellIndex
                /* SPR 20785 fix end */
                /** SPR 13599 Changes End **/
                );

        FREE_MEM_NODE_MAC_UE_OPPOR_IND_CIRC_Q(ueOppInd_p);

        if(errorCode == RLC_SUCCESS)
        {
            /* SPR 4391 changes start*/
            LOG_RLC_MSG( LTE_RLC_AM_STATUS_PDU_3, LOGINFO, RLC_TX,
                    globalTTITickCount_g, __LINE__, 0,
                    0, 0, 0, 0,0, __func__, "");	   
            /* SPR 4391 changes end*/
        }
/* SPR 8712 changes */
#if (defined(DL_UL_SPLIT) && !defined(UE_SIM_TESTING) && !defined(DL_DATA_SEPARATION))
/* SPR 8712 changes */
    }// While(1)
#endif
    /* + Coverity_Stack Fix */
    freeMemPool(rlcHarqFailureInfo);
    /* - Coverity_Stack Fix */
    return errorCode;
}
/* - coverity 55316 */

