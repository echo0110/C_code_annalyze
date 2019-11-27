/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcRxProcessor.c,v 1.3.12.2.2.2 2010/10/07 13:35:02 gur23054 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file describes the processing of recieved MAC PDU
 *
 *
 * 1. void* rlcRunRxProcessor(void* args)
 * 2. UInt32 rlcRunRxExecutionLeg(void)
 * 3. SInt32 rlcInitRxThread(void* args) 
 * 4. SInt32 rlcDeInitRxThread() 
 * 5. UInt32 rlcHandleComChDataFromMac() 
 * 6. validateAndGetEntity 
 * 7. handleRlcUMRxPacket 
 * 8. handleRlcAMRxPacket
 * 9. rlcHandleDataFromMac
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcRxProcessor.c,v $
 *
****************************************************************************/


/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcRxProcessor.h"
#include "lteRlcRrcInterface.h"
#include "lteRlcUeContext.h"
#include "lteRlcTmdSdu.h"
#include "lteRlcTmRxOperation.h"
#include "lteLayer2CommanTypes.h"
#include "lteRlcAmRxOperation.h"
#include "logging.h"
#include "alarm.h"
#include "lteCommonStatsManager.h"
#include "lteRlcErrorCode.h"
#include "lteRlcEntityDef.h"
/****************************************************************************
  Private Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

extern UInt32 rlcGetRingStatusForUM(RxUMEntity * rxUMEntity_p);
extern UInt32 numOfSDUTxtoPDCP;
extern void userPrintRawBuffer(const UChar8* strHeader, 
        const UInt8* buf, UInt32 len_buf, UInt32 fmt);

/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */
extern UInt16 rlcLayerInitialized_g;
#ifdef RLC_QUEUE_STATS
extern void lteMacUpdateStatsQueue(UInt16 , UInt16 );
#endif
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
extern LTE_CIRCQUEUE pdcpRlcDataIndQ_g; 
extern LTE_CIRCQUEUE rlcDeliveryStatusIndQ_g; 
#endif

#define MULTIPLY_BY_12(x)\
 ( (x << 3) + (x << 2) )
 
#define DIVIDE_BY_8(x)\
 x >> 3

#define MOD_BY_8(x)\
x & 0x7

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
#ifdef RLC_STANDALONE_SETUP
RlcThread rlcRxThread_g = RLC_THREAD_INIT;
#endif
/* This semaphore is using for indicate the RLC is recieving any data in RX
   direction 
*/
LTE_SEM   rlcRxSemlock_g;   

/****************************************************************************
 * Function Name  : rlcRunRxProcessor 
 * Inputs         : args - pointer to argument(s) 
 * Outputs        : None
 * Returns        : args
 * Description    : it is startup routine for Rx Thread.
 ****************************************************************************/

#ifdef RLC_STANDALONE_SETUP
void* rlcRunRxProcessor(void* args) 
{
    unsigned long new_mask =0x01;
    pid_t p = 0;
    THREAD_TO_BIND_GET_CORE_NO(p);
    THREAD_TO_BIND_SET_CORE_NO(p,new_mask);

    //for(;;)
    {
        //semWait(&rlcRxSemlock_g);
        rlcRunRxExecutionLeg();
    }
    return args ;
}
#endif


/****************************************************************************
 * Function Name  : rlcRunRxExecutionLeg 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : Execution Leg for Rx data path.
 ****************************************************************************/

#ifdef RLC_STANDALONE_SETUP
UInt32 rlcRunRxExecutionLeg(void) 
{
    UInt64 cQcount = 0;
    UInt64 sQcount = 0;
    logger_init(1,"RLC_PDCP_REGION",5242880 );

    startLogReader(&l2Input);
    while(1){
	uSleep(200);
        if(RLC_FAILURE != rlcLayerInitialized_g)
	{
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
           cQcount = circQueueCount(&pdcpRlcUeDataReqQ_g);
           if(cQcount)
               rlcHandleUeDataReq(cQcount);
#endif
           sQcount = sQueueCount(&rlcControlPduQ_g);
	  // if(sQcount)
            //   rlcHandleControlPduForRlcAm(sQcount);
           sQcount = COUNT_MAC_UE_DATA_IND_Q();
           if(sQcount)
               rlcHandleDataFromMac(sQcount);
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
        cQcount = circQueueCount(&pdcpRlcDataIndQ_g);
        if(cQcount)
            rlcHandleUeDataInd(cQcount);
        cQcount = circQueueCount(&rlcDeliveryStatusIndQ_g);
        if(cQcount)
            rlcHandleDeliveryStatusInd(cQcount);
#endif
        rlcHdlrPenDelQ(&rlcRxPendingDelQ_g);
        /* SPR 3805 Fix Start*/
        rlcHdlrPenUeDelQ(&rlcRxUePendingDelQ_g);
        /* SPR 3805 Fix End */
        rlcHdlrReEstabQ(&rlcRxReEstabQ_g);
    }
    }
    return RLC_SUCCESS;
}
#endif

/****************************************************************************
 * Function Name  : rlcInitRxThread
 * Inputs         : args - pointer to argument(s)
 * Outputs        : None
 * Returns        : RLC_SUCCESS if rx thread initialized successfully else
 *                  RLC_FAILURE
 * Description    : initializes Rx Thread.
 ****************************************************************************/
#ifdef RLC_STANDALONE_SETUP
SInt32 rlcInitRxThread(void* args)
{
    SInt32 ret = RLC_SUCCESS ;
    semInit(&rlcRxSemlock_g,0);
    rlcRxThread_g.start = rlcRunRxProcessor ;
    rlcRxThread_g.arg = args ;
    
    ret = threadCreate(&rlcRxThread_g.tid,THREAD_CREATE_DETACHED,
            THREAD_SCHED_FIFO,0,
            THREAD_EXPLICIT_SCHED,
            THREAD_SCOPE_SYSTEM,rlcRxThread_g.start,PNULL);

    THREAD_TO_BIND_SET_CORE_NO(rlcRxThread_g.tid,1);
    
    if(ret)
    {
        ltePanic("Unable to initialize the Rx thread\n");
    }    
    return ret;
}
#endif
/****************************************************************************
 * Function Name  : rlcDeInitRxThread
 * Inputs         : None
 * Outputs        : None
 * Returns        : RLC_SUCCESS if rx thread un-initialized  successfully else
 *                  RLC_FAILURE
 * Description    : cancels Rx Thread.
 ****************************************************************************/

#ifdef RLC_STANDALONE_SETUP
SInt32 rlcDeInitRxThread()
{
    SInt32 ret = RLC_SUCCESS;
    if(rlcRxThread_g.tid > 0) 
    {
        ret = threadCancel(rlcRxThread_g.tid);
        rlcRxThread_g.tid = 0;
    }
    semDestroy(&rlcRxSemlock_g);
    return ret;
}
#endif



/****************************************************************************
 * Function Name  : validateAndGetEntity
 * Inputs         : uplinkDataArr_p - Pointer to RLCUplinkDataInfo,
 *                  ueContext_p - Pointer to UEContext,
 *                  entityInfo_p - Pointer to RLCEntityInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE with error code
 * Description    : This function will do following things:-
 *                  1. Validate the dataLen and MsgRecieved.
 *                  2. Validate the LC ID.
 *                  3. Valid the buffer pointer and size of the buffer.
 *                  4. Fetch and validate the Entity.
 *                  5. Fetch and validate the UM Mode Entity. or
 *                  6. Fetch and validate the AM Mode Enttity.
 ****************************************************************************/
inline static UInt32 validateAndGetEntity (  RLCUplinkDataInfo* uplinkDataArr_p,
                                             UEContext *ueContext_p,
                                             RLCEntityInfo** entityInfo_p
                                           )
{
    RlcLcId lcId = uplinkDataArr_p->lcId;
    UInt16 szPdu = uplinkDataArr_p->dataLength;
    UInt8* pPdu = uplinkDataArr_p->rlcdataBuffer;

    if ( RLC_NULL == pPdu || 0 == szPdu )
    {
        if (pPdu )
        {
            LOG_RLC_MSG( LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR, LOGWARNING, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    ueContext_p->ueId, lcId, szPdu, RLC_RX_ZERO_SIZE_PDU, 0, 0, __func__,
                    ""); 
            msgFree(pPdu);
            /* SPR 20985 fix start */
            pPdu = RLC_NULL;
            /* SPR 20985 fix end */
            return RLC_RX_ZERO_SIZE_PDU;
        }

        LOG_RLC_MSG( LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_1, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                ueContext_p->ueId, lcId, szPdu, RLC_RX_NULL_PDU, 0, 0,
                __func__, ""); 

        return RLC_RX_NULL_PDU;
    }

    if ( msgSize(pPdu, PNULL) != szPdu )
    {
        LOG_RLC_MSG(LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_2, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                ueContext_p->ueId, lcId, szPdu, RLC_RX_MSZ_SIZE_NOT_EQUAL, 0, 0, __func__, ""); 

        return RLC_RX_MSZ_SIZE_NOT_EQUAL; 
    }

    if ( lcId >= MAX_LC_ID )
    {
        LOG_RLC_MSG( LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_3, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                ueContext_p->ueId, lcId, szPdu, RLC_RX_INVALID_LCID, 0, 0, __func__, ""); 

        return RLC_RX_INVALID_LCID;
    }
    /* getting ptr of ueContext */
    RLCEntityInfo* eInfo_p =
        DB_FETCH_RLC_ENTITY_INFO(ueContext_p, lcId);

    LOG_RLC_MSG( LTE_RLC_VALIDATE_AND_GET_ENTITY, LOGINFO, 
            (RLC_RX), globalTTITickCount_g, __LINE__,
            ueContext_p->ueId, lcId, szPdu,0, 0, 0, __func__, ""); 

    if ( RLC_NULL == eInfo_p)
    {
        LOG_RLC_MSG( LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_4, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                ueContext_p->ueId, lcId, szPdu, RLC_RX_ENTITY_NOT_EXIST, 0, 0, __func__, ""); 

        return RLC_RX_ENTITY_NOT_EXIST;
    }
    /*SPR 16885 +-*/
#ifdef UE_SIM_TESTING
    if ( RLC_ENTITY_ACTIVE != eInfo_p->rlcEntityState)
    {
        LOG_RLC_MSG( LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_5, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                ueContext_p->ueId, lcId, szPdu,  eInfo_p->rlcEntityState,
                0, 0, __func__, "");

        return RLC_RX_ENTITY_NOT_IN_ACTIVE_MODE;
    }
#endif    

    switch ( eInfo_p->rlcEntityMode )
    {
        case RLC_ENTITY_UM:
            {  
                UMEntity* umEntity_p = eInfo_p->rlcEntity.umEntity_p;
                if ( !umEntity_p || !umEntity_p->rxUMEntity_p )
                {
                    LOG_RLC_MSG( LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_6, LOGWARNING, 
                            (RLC_RX), globalTTITickCount_g, __LINE__,
                            ueContext_p->ueId, lcId, szPdu, eInfo_p->rlcEntityMode,
                            0,0, __func__, "");

                    return RLC_RX_UM_ENTITY_NOT_FOUND;
                }
    /*SPR 16885 +-*/
#ifndef UE_SIM_TESTING
                if ( RLC_ENTITY_ACTIVE != umEntity_p->rxEntityState)
                {
                    LOG_RLC_MSG( LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_5, LOGWARNING, 
                            (RLC_RX), globalTTITickCount_g, __LINE__,
                            ueContext_p->ueId, lcId, szPdu,  umEntity_p->rxEntityState,
                            0, 0, __func__, "");

                    return RLC_RX_ENTITY_NOT_IN_ACTIVE_MODE;
                }
#endif    
            }
            break;
        case RLC_ENTITY_AM :
            {
                AMEntity* amEntity_p = eInfo_p->rlcEntity.amEntity_p;
                if (( !amEntity_p ) ||
                        (!amEntity_p->rxAMEntity_p))

                {
                    LOG_RLC_MSG( LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_7, LOGWARNING, 
                            (RLC_RX), globalTTITickCount_g, __LINE__,
                            ueContext_p->ueId, lcId, szPdu, eInfo_p->rlcEntityMode,
                            0,0, __func__, "");

                    return RLC_RX_AM_ENTITY_NOT_FOUND;
                }
    /*SPR 16885 +-*/
#ifndef UE_SIM_TESTING
                if ( RLC_ENTITY_ACTIVE != amEntity_p->rxEntityState)
                {
                    LOG_RLC_MSG( LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_5, LOGWARNING, 
                            (RLC_RX), globalTTITickCount_g, __LINE__,
                            ueContext_p->ueId, lcId, szPdu,  amEntity_p->rxEntityState,
                            0, 0, __func__, "");

                    return RLC_RX_ENTITY_NOT_IN_ACTIVE_MODE;
                }
#endif    

            }
            break;
        default:

            LOG_RLC_MSG( LTE_RLC_VALIDATE_AND_GET_ENTITY_ERR_8, LOGWARNING, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    ueContext_p->ueId, lcId, szPdu, eInfo_p->rlcEntityMode,
                    0,0, __func__, "");

            break;
    }
    *entityInfo_p = eInfo_p;

    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : handleRlcUMRxPacket
 * Inputs         : uplinkDataArr_p - Pointer to RLCUplinkDataInfo,
 *                  rxUMEntity_p - Pointer to RxUMEntity
 * Outputs        : None
 * Returns        : errorCode
 * Description    : This function will do following things:-
 *                  1. Fill the umdRawPdu with recieved data from MAC Layer
 *                  2. Call the function rlcReceiveUmPduFromLowerLayer
 *                  3. if errorCode != RLC_SUCCESS then free the recieved buffer.
 *                  4. return errorCode.
 ****************************************************************************/
inline UInt32 handleRlcUMRxPacket( RLCUplinkDataInfo* uplinkDataArr_p ,
                                          RxUMEntity* rxUMEntity_p       
                                        )
{
    UmdRawPdu umdRawPdu;
    RlcBuffer *pRawPdu_p = PNULL;
    UInt32 errorCode       = INVALID_ERR_CODE;
#ifndef UE_SIM_TESTING    
    /* SPR 15909 fix start */
    tickType_t pktTimeStamp = 0;
    /* SPR 15909 fix end */
    UInt8 spsState = 0;  /*SPS_IDLE*/
#endif
    /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 15909 fix start */
    tickType_t packetTime = 0;
    /* SPR 15909 fix end */
#endif
    /*32.425 Events code changes end*/

    memSet(&umdRawPdu,0,sizeof(UmdRawPdu));
    pRawPdu_p =  &(umdRawPdu.pRawPdu);

    pRawPdu_p->szUsed = uplinkDataArr_p->dataLength;
    pRawPdu_p->size   = uplinkDataArr_p->dataLength;
    pRawPdu_p->data_p = uplinkDataArr_p->rlcdataBuffer;
#ifndef UE_SIM_TESTING    
    pktTimeStamp = uplinkDataArr_p->pktTimeStamp;
    spsState = uplinkDataArr_p->spsState;
#endif
    /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    packetTime = uplinkDataArr_p->packetTime;
#endif
    /*32.425 Events code changes end*/
    errorCode = rlcReceiveUmPduFromLowerLayer(
            rxUMEntity_p,
            &umdRawPdu
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

    if ( RLC_SUCCESS == errorCode )
    {
        LOG_RLC_MSG(LTE_RLC_HANDLE_UM_RX_PACKET, LOGINFO, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                uplinkDataArr_p->dataLength, 0,
                0,0, __func__, "");
    }
    else
    {
        LOG_RLC_MSG(LTE_RLC_HANDLE_UM_RX_PACKET_ERR, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
                uplinkDataArr_p->dataLength, errorCode,
                0,0, __func__, "");
/*SPR 17053 fix start*/
        msgFree(umdRawPdu.pRawPdu.data_p);
        umdRawPdu.pRawPdu.data_p=RLC_NULL;
/*SPR 17053 fix end*/
        uplinkDataArr_p->rlcdataBuffer = RLC_NULL;
    }
  
    return errorCode;
}


/****************************************************************************
 * Function Name  : handleRlcAMRxPacket
 * Inputs         : uplinkDataArr_p - Pointer to RLCUplinkDataInfo,
 *                  amEntity_p - Pointer to AMEntity
 * Outputs        : None
 * Returns        : errorCode
 * Description    : This function will do following things:-
 *                  1. fill the amdRawPdu with recieved data from MAC layer. 
 *                  2. call the function rlcReceiveAmdPduFromLowerLayer
 *                  3. if errorCode is not equal to success then free the buffer.
 *                  4. return errorCode
 ****************************************************************************/

inline static UInt32 handleRlcAMRxPacket( RLCUplinkDataInfo* uplinkDataArr_p, 
                                          AMEntity* amEntity_p
                                        )
{
    AmdRawPdu amdRawPdu;   
    UInt32 errorCode      = INVALID_ERR_CODE;
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 15909 fix start */
    tickType_t packetTime = 0;
    /* SPR 15909 fix end */
#endif
    memSet(&amdRawPdu,0,sizeof(AmdRawPdu));

    amdRawPdu.pRawPdu.size      = uplinkDataArr_p->dataLength;
    amdRawPdu.pRawPdu.data_p    = uplinkDataArr_p->rlcdataBuffer;
    /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    packetTime = uplinkDataArr_p->packetTime;
#endif
    /*32.425 Events code changes end*/
    errorCode = rlcReceiveAmdPduFromLowerLayer(
            amEntity_p, 
            &amdRawPdu
    /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
            ,packetTime
#endif
    /*32.425 Events code changes end*/
            );
    if( RLC_SUCCESS == errorCode)
    {
        LOG_RLC_MSG(LTE_RLC_HANDLE_AM_RX_PACKET, LOGINFO, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                amEntity_p->rxAMEntity_p->ueIndex,
                amEntity_p->rxAMEntity_p->lcId, 
                uplinkDataArr_p->dataLength,
                0, 0, 0, __func__, "");
    }
    else
    {
        LOG_RLC_MSG(LTE_RLC_HANDLE_AM_RX_PACKET_ERR, LOGWARNING, 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                amEntity_p->rxAMEntity_p->ueIndex,
                amEntity_p->rxAMEntity_p->lcId, 
                uplinkDataArr_p->dataLength,
                errorCode, 0, 0, __func__, "");

        //msgFree(uplinkDataArr_p->rlcdataBuffer);
        msgFree(amdRawPdu.pRawPdu.data_p); //atanwar
        uplinkDataArr_p->rlcdataBuffer = RLC_NULL;
        /* SPR 20985 fix start */
        amdRawPdu.pRawPdu.data_p = RLC_NULL;
        /* SPR 20985 fix end */
    }

    return errorCode;
}


/****************************************************************************
 * Function Name  : rlcHandleDataFromMac
 * Inputs         : count,
 * 		            internalCellIndex
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE/errorCode
 * Description    : This function will do following things:-
 *                  1. Dequeue the node for UL Data.
 *                  2. Fetch & Validate the UE Context using UE Index.
 *                  3. Validate the LcId, UL Buffer and its length.
 *                  4. Fetch & Validate RLC Entity.
 *                  5. Check Whether RLC Entity is UM or AM Mode and 
 *                     call correspoinding for decode ULSCH packets.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
/** CA 2 Core Changes Start **/
extern UInt64 uplinkRlcIngressDataSize;
extern UInt64 uplinkRlcIngressDataNum;
UInt32 rlcHandleDataFromMac(UInt32 count,InternalCellIndex internalCellIndex)
/** CA 2 Core Changes End **/
/* SPR 5599_129119 Fix End */
{
    MacUeDataInd* ueDataInd_p          = RLC_NULL;
    RLCUplinkDataInfo* uplinkDataArr_p = RLC_NULL ; 
    UEContext *ueContext_p             = RLC_NULL ;
    UInt16 lcCount                     = 0  ;
    UInt16 lIdx                        = 0 ;
    RLCEntityInfo* eInfo_p             = RLC_NULL;
    UInt32 errorCode                   = INVALID_ERR_CODE;                         

    do
    {
        /** CA 2 Core Changes Start **/
	DEQUEUE_MAC_UE_DATA_IND_Q(MacUeDataInd,(void **)&ueDataInd_p,internalCellIndex ); 
        /** CA 2 Core Changes End **/
        if ( RLC_NULL == ueDataInd_p )
        {
             LOG_RLC_MSG(LTE_RLC_HANDLE_DATA_FROM_MAC_ERR, LOGWARNING, 
                     (RLC_RX), globalTTITickCount_g, __LINE__,
                     0,0, 0,0, 0,0, __func__, "");

            return RLC_FAILURE;
        }

        lcCount         = ueDataInd_p->dataToRlc_p->numLCInfo;
        uplinkDataArr_p = ueDataInd_p->dataToRlc_p->rlcDataArr ;
        
        /*if ueContext exists*/
/* SPR#11313 Fix changes	*/	
        if ((( ueContext_p = DB_FETCH_UE_CONTEXT(ueDataInd_p->ueIndex)) == RLC_NULL)
                    || ( ueContext_p->rlcUEState != RLC_UECONTEXT_ACTIVE ))
/* SPR#11313 Fix changes	*/
        {
             errorCode = LTE_RLC_UE_CTX_NOT_FOUND; 
             LOG_RLC_MSG(LTE_RLC_HANDLE_DATA_FROM_MAC_ERR_1, LOGWARNING, 
                     (RLC_RX), globalTTITickCount_g, __LINE__,
                     ueDataInd_p->ueIndex, lcCount, errorCode,0, 0,0, __func__,
                     "");

            for ( lIdx = 0;lIdx < lcCount ; lIdx++ )
            {
                msgFree(uplinkDataArr_p[lIdx].rlcdataBuffer);
                /* SPR 21754 Fix + */
                uplinkDataArr_p[lIdx].rlcdataBuffer = RLC_NULL;
                /* SPR 21754 Fix - */
            }
            freeMemPool(ueDataInd_p->dataToRlc_p);
            /*SPR 16979 Fix Start*/
            ueDataInd_p->dataToRlc_p = RLC_NULL;
            /*SPR 16979 Fix Start*/
            FREE_MEM_NODE_MAC_UE_DATA_IND_Q(ueDataInd_p);

            return errorCode;
        }
        
        LOG_RLC_MSG(LTE_RLC_HANDLE_DATA_FROM_MAC, LOGINFO, 
                     (RLC_RX), globalTTITickCount_g, __LINE__,
                     ueDataInd_p->ueIndex, lcCount, 0, 0, 0, 0, __func__,
                     "");
        
        for ( lIdx = 0; lIdx < lcCount ; lIdx++ )
        {
            if ( RLC_SUCCESS !=  validateAndGetEntity(&uplinkDataArr_p[lIdx], ueContext_p, &eInfo_p) )
            {
             LOG_RLC_MSG(LTE_RLC_HANDLE_DATA_FROM_MAC_ERR_3, LOGWARNING, 
                     (RLC_RX), globalTTITickCount_g, __LINE__,
                     ueDataInd_p->ueIndex, lIdx, errorCode,0, 0,0, __func__,
                     "");
                msgFree(uplinkDataArr_p[lIdx].rlcdataBuffer);
                uplinkDataArr_p[lIdx].rlcdataBuffer = RLC_NULL; 
                continue;
            }
            if( uplinkDataArr_p[lIdx].dataLength >= RLC_MAX_PDU_LENGTH )
            {
                msgFree(uplinkDataArr_p[lIdx].rlcdataBuffer);
                uplinkDataArr_p[lIdx].rlcdataBuffer = RLC_NULL; 
                continue;
            }
			
			uplinkRlcIngressDataSize += uplinkDataArr_p[lIdx].dataLength;
			uplinkRlcIngressDataNum++;
            switch ( eInfo_p->rlcEntityMode )
            {
                case RLC_ENTITY_UM:
                   {
                     RxUMEntity * rxUMEntity_p = 
                          eInfo_p->rlcEntity.umEntity_p->rxUMEntity_p; 

                     errorCode = handleRlcUMRxPacket( &uplinkDataArr_p[lIdx],
                                                     rxUMEntity_p
                                                   );
                            
                    }
                    break;
                case RLC_ENTITY_AM:
                {
                    AMEntity* amEntity_p = 
                            (eInfo_p->rlcEntity.amEntity_p); 
                            
                    errorCode = handleRlcAMRxPacket( &uplinkDataArr_p[lIdx], 
                                                     amEntity_p
                                                   );
                            
                 }           
                    break;
                default:
                    LOG_RLC_MSG(LTE_RLC_HANDLE_DATA_FROM_MAC_ERR_2, LOGWARNING, 
                            (RLC_RX), globalTTITickCount_g, __LINE__,
                            ueDataInd_p->ueIndex, lcCount, eInfo_p->rlcEntityMode,
                            0, 0, 0, __func__, "");

                    break;
            }
        }
        freeMemPool(ueDataInd_p->dataToRlc_p);
        /*SPR 16979 Fix Start*/
        ueDataInd_p->dataToRlc_p = RLC_NULL;
        /*SPR 16979 Fix Start*/
        FREE_MEM_NODE_MAC_UE_DATA_IND_Q(ueDataInd_p);
    }while(--count);

    return RLC_SUCCESS ;
}

/*Assume that this function will call for multiple SDU */
#define GET_E_BIT( eliStart, eliOffset)\
    ((eliStart[0] >> ( 7 - eliOffset )) & 0x01 )

#define GET_LI( eliStart, eliOffset )\
 ( (( eliStart[0] << ( 4 + eliOffset )) | (eliStart[1] >> ( 4 - eliOffset) )) & 0x7FF )

/****************************************************************************
 * Function Name  : rlcGetHeaderInfo 
 * Inputs         : data_p - a pointer to data buffer(UMD PDU),
 *                  fixedHdrIdx - a pointer to integer location having index to
 *                                data field(DF) first byte,
 *                  pduHdrInfo_p - a pointer to RlcPduHeaderInfo
 * Outputs        : None
 * Returns        : number of DFE(s)
 * Description    : returns number of DFE(s). call this method if E field in
 *                  UMD PDU's fixed header is 1.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 rlcGetHeaderInfo(  UInt8* data_p,
                                  UInt32 fixedHdrIdx,
                                  RlcPduHeaderInfo *pduHdrInfo_p
                                )
/* SPR 5599_129119 Fix End */
{
    UInt8 ext       = 1;
    UInt8 byteNum   = 0;
    UInt32 dfeCount = 1; /* Fixed Header*/
    UInt32 li       = 0;
    UInt8 *eliStart = PNULL;
    /* Value should be 0 and 4 as corressponding to E bit position*/
    UInt8 eliOffset    = 0;
    UInt8 *base_data_p = RLC_NULL;

    QSEGMENT segment = {0};
    msgSegNext(data_p, 0, &segment);
    base_data_p  = segment.base;
    /*+++530-coverity-55200*/
    if(PNULL == base_data_p)
    {
        return RLC_FAILURE;
    }
    /*+++530-coverity-55200*/
    UInt8 *baseStartWithVH = &base_data_p[fixedHdrIdx];

    pduHdrInfo_p->numDfe = 0;

    do  
    { /*  
          elifOffset = 0   elifOffset = 4 where E bit exist.

          0   1  2   3   4   5  6  7
       *****************************
       E              L               eliStart = 0
       *****************************
       L           E     L         eliStart = 1
       *****************************
       L
       *****************************
       E              L               eliStart = 3
       *****************************
       */
        /* Get the byte number where E bit exist 
           12 = E ( 1 bit) + L ( 11 bit ) 
           byteNum = 
           8 = 1 bytes
         */
        eliStart  = & (baseStartWithVH[ DIVIDE_BY_8( (MULTIPLY_BY_12( byteNum)) )]);
	/* Coverity CID 54513 Start */
        /* Code removed for coverity */
	/* Coverity CID 54513 End */
        /* Get the exact position of E in term of bits. i.e 0 or 4 */
        eliOffset = MOD_BY_8( (MULTIPLY_BY_12( ( byteNum) )) ) /* Mod 8*/;
        /* Get the value of extension i.e 0 or 1 */
        ext = GET_E_BIT( eliStart, eliOffset);
        /* Get the value of Length of the sdu*/
        li  = GET_LI( eliStart, eliOffset);
        /* SPR 5144 Fix start */ 
        /* li = 0 is reserved and if this value is received the PDU should be discard */
        if (0 == li)
        {
            LOG_RLC_MSG(LTE_RLC_GET_HEADER_INFO_ERR, LOGWARNING,
                   (RLC_RX), globalTTITickCount_g, __LINE__,
                    RLC_FAILURE, 0, 0, 0, 0, 0, __func__, "LI = 0");
            return RLC_FAILURE;
        }
        /* SPR 5144 Fix End */ 

        pduHdrInfo_p->dfeLength[pduHdrInfo_p->numDfe] = li;
        pduHdrInfo_p->numDfe++;
        dfeCount++;
        byteNum++;

        LOG_RLC_MSG(LTE_RLC_GET_HEADER_INFO, LOGDEBUG , 
                (RLC_RX), globalTTITickCount_g, __LINE__,
                dfeCount, li, eliOffset,
                ext, 0,0, __func__, ""); 
        /* + SPR 5498 */
        if(pduHdrInfo_p->numDfe >= RLC_MAX_DFE_IN_PDU_UL )
            break;
        /* - SPR 5498 */

    }while ( 0 != ext );

    pduHdrInfo_p->hdrSize = (GET_VAR_HEADER_SIZE( pduHdrInfo_p->numDfe + 1) ) + fixedHdrIdx;
    /* + Coverity 26358, 26360 */
    LOG_RLC_MSG(LTE_RLC_GET_HEADER_INFO_1, LOGDEBUG, 
            (RLC_RX), globalTTITickCount_g, __LINE__,
            pduHdrInfo_p->hdrSize, dfeCount,
            0, 0, 0, 0, __func__, "");
    /* - Coverity 26358, 26360*/

    return dfeCount;
}
