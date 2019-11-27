/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcRrcInterface.c,v 1.2.2.1.4.2.2.4 2010/10/27 10:42:08 gur20056 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *  This file contains the implementation of interface APIs to communicate 
 *  with RRC upper layer.
 *  
 *  1. Initialize/DeInitialize the Thread for recieving RRC and OAM Msg. 
 *       (Valid for only Standalone RLC Setup.
 *     a) initRRCInterfaceThread -> runRRCInterfaceProcessor -> rlcHandleSocketMsgs
 *     b) deInitRRCInterfaceThread
 *
 *  2. Recieve Msg from Socket (RRC/OAM Layer)    
 *     c) rlcHandleSocketMsgs
 *
 *  3. BroadCast Message and Paging Msg recieved from RRC Layer
 *     d) intRrcDataReq 
 *     e) intRrcDataInd
 *
 *  4. Send Msg or Indication to RRC layer for Max retransmission in AM mode.
 *     f) rlcSendMaxReTxThresholdIndToRrc
 *
 *  5. 
 *     g) rlcInitRrcInterface
 *     h) rlcDeInitRrcInterface
 *     i) rlcDeleteUEContext
 *     j) rlcCreateComChEntity
 *     k) rlcCreateUEEntity
       l) validateEntityTypeAndCtx
       m) rlcReconfigUEEntity
       n) rlcDeleteUEEntity
       o) deleteComChEntity
 *     p) rlcDeleteComChEntity
 *     r) rlcReEstablishComChEntity 
 *     s) reEstablishComChEntity
 *     t) rlcReEstabUeEntity
 *     u) reEstablishUeEntity
 *     v) rlcReEstabReassRxAMEntity 
 *     x) rlcReEstabReassRxUMEntity
 *     y) rlcConfigEntityParam
 *     z) rlcConfigAmRxEntityParam
 *     1) rlcConfigAmTxEntityParam 
 *     2) rlcConfigUmRxEntityParam
 *     3) rlcConfigUmRxEntityParam
 *     4) rlcConfigUmTxEntityParam
 *
 *  x. Delete/Reestablish the RLC Entity 
 *     called from execution Scheduler.
 *
 *     5) rlcHdlrPenDelQ
 *     6) rlcHdlrReEstabQ
 *     7) rlcHdlrPenUeDelQ
 *
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcRrcInterface.c,v $
*
*
****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcRrcInterface.h"
#include "lteRlcRrcParseUtil.h"
#include "lteRlcTmTxOperation.h"
#include "lteRlcOamIntf.h"
#include "lteLayer2CommanTypes.h"
#include "lteRlcAmArithmetic.h"
#include "lteRlcAmRxOperation.h"
#include "lteRlcAmdSdu.h"
#include "logging.h"
#include "alarm.h"
#include "lteRlcStat.h"
#include "lteCommonStatsManager.h"
#include "lteRlcErrorCode.h"
#include "lteRlcTimer.h"
#include "lteRlcTxProcessor.h" 

/* SPR 5178 changes starts here */
#if defined(ENDC_ENABLED)
#include "lteRlcPdcpInterface.h"
#endif
/* SPR 5178 changes ends here */
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* used to configure parameters of entity */
static void rlcConfigEntityParam ( RLCEntityInfo *outRLCEntityInfo_p,
                                          RlcEntityCfgInfo* rlcEntityCfgInfo, 
                                          UInt16 ueIndex
                                        );

/* used to configure parameters of Rx entity of UM Mode*/
static void rlcConfigUmRxEntityParam( RxUMEntity *rxUMEntity_p,
                                             RlcConfigParam rlcConfigParam,
                                             /* SPR 4272 Fix Start */
                                             RlcEntityType eT
                                             /* SPR 4272 Fix End */
                                           );

/* used to configure parameters of Tx entity of UM Mode*/
static void rlcConfigUmTxEntityParam( TxUMEntity *txUMEntity_p,
                                             RlcConfigParam rlcConfigParam 
                                           );

/* used to configure parameters of Rx entity of AM Mode*/
static void rlcConfigAmRxEntityParam( RxAMEntity *rxAMEntity_p,
                                             RlcConfigParam rlcConfigParam
                                           );
/* used to configure parameters of Tx entity of AM Mode*/
static void rlcConfigAmTxEntityParam ( TxAMEntity *txAMEntity_p,
                                              RlcConfigParam rlcConfigParam 
                                            );

/*used to send max Re-transmit indication to RRC */
/* SPR 5599_129123 Fix Start */
UInt32 rlcSendMaxReTxThresholdIndToRrc(TxAMEntity* pAmTxE);
/* SPR 5599_129123 Fix End */
/*RLC Rel 2.0 Changes End*/

/* SPR 5599_129123 Fix Start */
void  reportErrorToRRCLayer ( UInt8 *inMsg_p, 
/* SPR 5599_129123 Fix End */
                              UInt16 resp
                            );

/* SPR 5178 changes starts here */
/* SPR 5178 changes ends here */

static UInt32 rlcHandleReceivedMsg (
                                               UInt8 *recvMsgBuf_p,
                                               SInt32 bytesRead
                                             );
                                                                                                                                           

static void reEstablishUMEntity(  RlcEntityType entityType,
                                         RLCEntityInfo *rlcEntityInfo_p 
                                      );
static UInt32 reEstablishAMEntity( RLCEntityInfo *rlcEntityInfo_p);

#ifndef UE_SIM_TESTING
/* +- SPR 17777 */
STATIC void rlcEntitySetsSpsLc(UEContext *ueCtx_p,
        UInt16 rbDirection );
/* +- SPR 17777 */
#endif

/* SPR 4172 Fix Start */
/*used to stop all running AM Timers */
/* SPR 5599_129123 Fix Start */
void rlcStopAllRunningAmTimers(RLCEntityInfo *entityInfo_p);
/* SPR 5599_129123 Fix End */
/*used to stop all running UM Timers */
/* SPR 5599_129123 Fix Start */
void rlcStopAllRunningUmTimers(RLCEntityInfo *entityInfo_p);
/* SPR 5599_129123 Fix End */
/* SPR 4172 Fix End */
/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
/****************************************************************************
 * Queue to store the Rlc Entities to delete.
 * RlcGenericQ data structure.
 ****************************************************************************/
/*SPR 3805 Fix Start*/
LTE_CIRCQUEUE rlcRxUePendingDelQ_g;
LTE_CIRCQUEUE rlcTxUePendingDelQ_g;
/*SPR 3805 Fix End */

LTE_SQUEUE rlcRxPendingDelQ_g;
LTE_SQUEUE rlcTxPendingDelQ_g;

/****************************************************************************
 * Queue to store the ReEstablished Rlc Entities .
 * RlcGenericQ data structure.
 ****************************************************************************/
LTE_SQUEUE rlcRxReEstabQ_g;
LTE_SQUEUE rlcTxReEstabQ_g;

#else
/** CA Changes Start **/
LTE_CIRCQUEUE rlcRxPendingDelQ_g;
LTE_CIRCQUEUE rlcTxPendingDelQ_g[MAX_NUM_CELL];
LTE_CIRCQUEUE rlcRxReEstabQ_g;
LTE_CIRCQUEUE rlcTxReEstabQ_g[MAX_NUM_CELL];
LTE_CIRCQUEUE rlcReconfigTxEntityQ_g;
LTE_CIRCQUEUE rlcReconfigRxEntityQ_g;
/*+ SPR 21121*/
LTE_CIRCQUEUE rlcUlSchedToLPCircQ_g;
LTE_CIRCQUEUE rlcLPtoRlcUlSchedCircQ_g;
/*- SPR 21121*/
/** CA Changes End **/
#endif

/****************************************************************************
 * Semaphore used in the processing of pending deletion queue.
 ****************************************************************************/
LTE_SEM processDelSem_g;

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern UInt32 activeUEs_g;
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */
/* SPR 5599_129123 Fix Start */
UInt32 rlcHandleSocketMsgs( void *, SInt32);
UInt8 * rlcReceiveSocketMsgs(SInt32* bytesRead);
/* SPR 5599_129123 Fix End */

#ifdef RLC_STANDALONE_SETUP
/* initializing the RRC thread */
RlcThread rlcRRCThread_g = RLC_THREAD_INIT;
#endif
volatile UInt16 rlcLayerInitialized_g = 0;

/** Rel 4.0 changes Start **/
#ifndef UE_SIM_TESTING
extern UInt32 qSizeFactorGBR_g;
extern UInt32 qSizeFactorNGBR_g;
RlcUeBitRateInfo rlcUeBitRateInfo_g[MAX_UE_SUPPORTED];
UDouble32 qSizeFactor[] = {0.5,1,2,3,4,5};

/* + Layer2 NON CA Changes */
extern RlcSecondaryCellInfo secondaryCellInfo_g[MAX_NUM_CELL];
extern UInt8 numOfRlcCellConfigured_g;
/* - Layer2 NON CA Changes */

#endif
/** Rel 4.0 changes End **/
/** SPR 3986 Fix Start **/
#define MIN_DRB_LC_ID 3
/** SPR 3986 Fix End **/
extern void  rlcSendMsgToRrc(UInt8 *ui_CnfMsgBuf_p,
        UInt16 ui16_msgLen);

extern SInt32 sendMsgWithIccQ(SInt32 mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio);
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
extern SInt32 rlcRrcCnfInputQId_g;
/*+ SPR 21121*/
extern LTE_CIRCQUEUE rlcLPtoRlcUlSchedCircQ_g;
/*- SPR 21121*/
#endif

/****************************************************************************
 * Function Name  : runRRCInterfaceProcessor 
 * Inputs         : args - pointer to argument(s) 
 * Outputs        : None
 * Returns        : None
 * Description    : The runRRCInterfaceProcessor thread call the 
 *                  rlcHandleSocketMsgs function for OAM
 *                  and RRC layer msg on the sockets.
 ****************************************************************************/

#ifdef RLC_STANDALONE_SETUP  
void* runRRCInterfaceProcessor(void* args) 
{

    fd_Set readfds;
    unsigned long new_mask = 0x1;
    pid_t p =0;
    THREAD_TO_BIND_GET_CORE_NO(p);
    THREAD_TO_BIND_SET_CORE_NO(p,new_mask);
    UInt32 highestFD = 0;
    timeVal timeout;
    logger_init(0, "MAC_RLC_REGION", 5242880);
    while (1)
    {
        timeout.tv_sec = 0;
        timeout.tv_usec =200;
        //check that rlc layer is initialized
        //otherwise continue;
        lteFD_SET(readSockFD_g, &readfds);
        highestFD = readSockFD_g;
  
        if( 0 != select_wrapper(highestFD + 1, &readfds, PNULL, PNULL, &timeout))
		{
			UInt8 *recvMsgBuf_p  = PNULL;
			SInt32 bytesRead     = 0;
			recvMsgBuf_p = (UInt8 *)getMemFromPool( MAX_REQ_DATASIZE, RLC_NULL);
			SockAddr rrc_addr;

			if (PNULL == recvMsgBuf_p)
			{
				ltePanic("[%s]Error in getMemFromPool",__func__);
			}

			UInt32 addr_len_rrc = sizeof(rrc_addr);
			memSet(&rrc_addr, 0x0, sizeof(rrc_addr));
			addr_len_rrc = addr_len_rrc; //remove warning

			bytesRead = READ_FROM_SOCKET( recvMsgBuf_p, rrc_addr, addr_len_rrc);
			rlcHandleSocketMsgs(recvMsgBuf_p,bytesRead);
        }   
        if(rlcLayerInitialized_g == RLC_TRUE)
             rlcRunTxExecutionLeg();  
    }
    return args;
}
#endif

/* SPR 5178 changes starts here */
/* SPR 5178 changes ends here */

/****************************************************************************
 * Function Name  : reportErrorToRRCLayer 
 * Inputs         : inMsg_p,
 *                  resp
 * Outputs        : None
 * Returns        : None
 * Description    : This function send failure response to RRC.  
 ****************************************************************************/
/* SPR 5599_129123 Fix Start */
void  reportErrorToRRCLayer( UInt8 *inMsg_p, 
/* SPR 5599_129123 Fix End */
                             UInt16 resp
                           )
{
    UInt16 current = 0;
    UInt16 apiId   = 0;
    UInt16 val     = 0;
    UInt8 respBuf[20] = {0};
    UInt8 *resp_p = PNULL;

    resp_p = respBuf;
    resp_p[current] = (inMsg_p[current] & 0x00FF);
    current += 1;
    resp_p[current] = (inMsg_p[current] & 0x00FF);
    current += 1;
    val = LTE_GET_U16BIT(inMsg_p + (current + 2));
    LTE_SET_U16BIT((resp_p + current),val); 
    current += 2;

    val = LTE_GET_U16BIT(inMsg_p + (current - 2));
    LTE_SET_U16BIT((resp_p + current),val); 
    current += 2;

    apiId = LTE_GET_U16BIT(inMsg_p + current);
    apiId += 1;
    LTE_SET_U16BIT((resp_p + current),apiId); 
    current += 4;
    val = LTE_GET_U16BIT(inMsg_p + (current));
    LTE_SET_U16BIT((resp_p + current),val); 
    current += 2;

    LTE_SET_U16BIT((resp_p + current),resp); 
    current += 2;

    LTE_SET_U16BIT((resp_p + (current -6)),current); 
    rlcSendMsgToRrc(respBuf,
            current);
    
    LOG_RLC_MSG ( LTE_RLC_REPORT_ERROR_TO_RRC, LOGWARNING, 
            RLC_RRC, globalTTITickCount_g, __LINE__, apiId, current, resp, 
            0, 0, 0, __func__, "");
} 
/****************************************************************************
 * Function Name  : rlcReceiveSocketMsgs
 * Inputs         : bytesRead
 * Outputs        : None
 * Returns        : None
 * Description    : This function receive RLC Socket messages.  
 ****************************************************************************/
UInt8 * rlcReceiveSocketMsgs(SInt32* bytesRead)
{
	UInt8 *recvMsgBuf_p  = PNULL;
	SockAddr rrc_addr;
	recvMsgBuf_p = (UInt8 *)getMemFromPool( MAX_REQ_DATASIZE, PNULL);

	if (PNULL == recvMsgBuf_p)
	{
	    lteWarning("[%s]Error in getMemFromPool",__func__);
        /*+COVERITY 5.3.0 - 29783*/
        //    return RLC_FAILURE;
        return PNULL;
        /*-COVERITY 5.3.0 - 29783*/
	}
    /* + Coverity : 32126 */
	memSet(&rrc_addr, 0x0, sizeof(rrc_addr));
	*bytesRead = READ_FROM_SOCKET( recvMsgBuf_p, rrc_addr, sizeof(rrc_addr));
    /* - Coverity : 32126 */
	return recvMsgBuf_p;
}

/****************************************************************************
 * Function Name  : rlcHandleSocketMsgs 
 * Inputs         : recvBuf_p,
 *                  bytesRead
 * Outputs        : None
 * Returns        : RLC SUCCESS/ErrorCode 
 * Description    : This function receive message on socket from OAM and RRC  
 *                  and send a response after processing the message.
 ****************************************************************************/
/* SPR 5599_129123 Fix Start */
UInt32 rlcHandleSocketMsgs(void *recvBuf_p, SInt32 bytesRead)
/* SPR 5599_129123 Fix End */
{
    /* SPR 20469 Fix +- */
    UInt8 *recvMsgBuf_p = (UInt8 *)recvBuf_p;
    UInt32 errorCode     = INVALID_ERR_CODE;

    /* method to get the data from socket */
    if (bytesRead  == -1 )
    {
        LOG_RLC_MSG ( LTE_RLC_HANDLE_SOCKET_MSG_ERR_1, LOGWARNING,
                RLC_RRC, globalTTITickCount_g, __LINE__, RLC_SOCKET_ERROR, 0, 0, 0, 0, 0,
                __func__, "");
        /* SPR 15590 fix start */
        freeMemPool(recvMsgBuf_p);
        recvMsgBuf_p = PNULL;
        /* SPR 15590 fix end */
        return RLC_SOCKET_ERROR;
    }
    /* SPR 20469 Fix +- */
    if ( bytesRead >= RLC_API_HEADER_LEN )
    {
        errorCode = rlcHandleReceivedMsg( recvMsgBuf_p, bytesRead);
    }
    else
    {
        LOG_RLC_MSG ( LTE_RLC_HANDLE_SOCKET_MSG_ERR_2, LOGWARNING,
                RLC_RRC, globalTTITickCount_g, __LINE__, bytesRead,
                RLC_SOCKET_RX_INVALID_MSG_LEN, 0, 0, 0, 0, __func__, "");

        errorCode = RLC_SOCKET_RX_INVALID_MSG_LEN;
    }
    /* SPR 7587 Changes Start */
    freeMemPool(recvMsgBuf_p); 
    /* SPR 7587 Changes End */

    return errorCode;
}


/****************************************************************************
 * Function Name  : rlcHandleReceivedMsg 
 * Inputs         : recvMsgBuf_p,
 *                  bytesRead
 * Outputs        : None
 * Returns        : RLC SUCCESS/ErrorCode
 * Description    : This function receive message on socket from OAM and RRC
 *                  and send a response after processing the message.
 ****************************************************************************/

static UInt32 rlcHandleReceivedMsg ( 
                                               UInt8 *recvMsgBuf_p, 
                                               SInt32 bytesRead
                                             )
{
    UInt16 srcModId     = 0;
    UInt16 destModId    = 0;
    UInt16 apiId        = 0;
    UInt32 errorCode    = 0;


    /* Parse the Header of recieved buffer */
    /*
       Transaction ID   -  2 bytes
       Src Module ID    -  2 bytes
       Dest. Module ID  -  2 bytes
       API - Identifier -  2 bytes
       Length           -  2 bytes
     */
        srcModId = LTE_GET_U16BIT(recvMsgBuf_p + 2);
        destModId = LTE_GET_U16BIT(recvMsgBuf_p + 4);
        apiId = LTE_GET_U16BIT(recvMsgBuf_p + 6);

    if ( RLC_MODULE_ID !=  destModId ) 
    {
        LOG_RLC_MSG ( LTE_RLC_HANDLE_RECV_SOCKET_MSG_ERR, LOGWARNING,
                RLC_RRC, globalTTITickCount_g, __LINE__, srcModId, destModId,
                apiId, RLC_SOCKET_INVALID_DEST_ID, 0, 0, __func__, "");

        return RLC_SOCKET_INVALID_DEST_ID;
    }

    switch ( srcModId )
    {
        case OAM_MODULE_ID:
            {
                /* For Integration Setup- RLC Cleanup from OUTSIDE L2 function*/ 
                if ( CLEANUP_LAYER2 == apiId ) 
                {
                    return CLEANUP_LAYER2;
                }    
                /* +- SPR 17777 */
                errorCode = handleOamRequests( recvMsgBuf_p);               
                /* +- SPR 17777 */
            }
            break;

        case RRC_MODULE_ID:
            {
                if ( RLC_SUCCESS == rlcLayerInitialized_g ) 
                {
                    errorCode = parseRRCMsg ( recvMsgBuf_p, bytesRead);
                }
                else 
                {
                    LOG_RLC_MSG ( LTE_RLC_HANDLE_RECV_SOCKET_MSG_ERR_1, LOGWARNING,
                            RLC_RRC, globalTTITickCount_g, __LINE__, srcModId, destModId,
                            apiId, RLC_SOCKET_LAYER_NOT_INITIALIZED, 0, 0, __func__, "");

                    reportErrorToRRCLayer( recvMsgBuf_p, RLC_LAYER_NOT_INITIALIZED);

                    return RLC_SOCKET_LAYER_NOT_INITIALIZED ;
                }
            }
            break;

        default:
            {
                LOG_RLC_MSG(LTE_RLC_HANDLE_RECV_SOCKET_MSG_ERR_2, LOGWARNING,
                RLC_RRC, globalTTITickCount_g, __LINE__, srcModId,
                destModId, apiId, RLC_SOCKET_INVALID_SRC_MODULE_ID, 0, 0, __func__, "Src Id");

                return RLC_SOCKET_INVALID_SRC_MODULE_ID;
            }
    }   
    return errorCode;
}

/****************************************************************************
 * Function Name  : initRRCInterfaceThread 
 * Inputs         : args - pointer to argument(s)
 * Outputs        : None
 * Returns        : RLC_SUCCESS if RRC thread created successfully else
 *                  RLC_FAILURE
 * Description    : initializes RRC Thread.
 ****************************************************************************/
#ifdef RLC_STANDALONE_SETUP
/* SPR 5599_129123 Fix Start */
SInt32 initRRCInterfaceThread(void* args)
/* SPR 5599_129123 Fix End */
{
    SInt32 ret = RLC_SUCCESS ;

    rlcRRCThread_g.start = runRRCInterfaceProcessor ;
    rlcRRCThread_g.arg = args ;
    
    ret = threadCreate(&rlcRRCThread_g.tid,THREAD_CREATE_DETACHED,
            THREAD_SCHED_FIFO,0,
            THREAD_EXPLICIT_SCHED,
            THREAD_SCOPE_SYSTEM,rlcRRCThread_g.start,PNULL);

    THREAD_TO_BIND_SET_CORE_NO(rlcRRCThread_g.tid,1);
    
    if(ret) 
    {
        ltePanic("Unable to initialize RLC-RRC Interface thread\n");
    }    
    return ret ;
}
#endif
/****************************************************************************
 * Function Name  : deInitRRCInterfaceThread 
 * Inputs         : None
 * Outputs        : None
 * Returns        : RLC_SUCCESS if RRC thread destroyed successfully
 * Description    : cancel RRC Thread.
 ****************************************************************************/
#ifdef RLC_STANDALONE_SETUP
/* SPR 5599_129123 Fix Start */
SInt32 deInitRRCInterfaceThread()
/* SPR 5599_129123 Fix End */
{
    SInt32 ret = RLC_SUCCESS ;
    if(rlcRRCThread_g.tid > 0) 
    {
        ret = threadCancel(rlcRRCThread_g.tid);
        rlcRRCThread_g.tid = 0;
    }
    return ret ;                                        
}
#endif

/* Rlc Rel 2.0 change start */
 /*******************************************************************************************
 * Function Name  : rlcSendMaxReTxThresholdIndToRrc
 * Inputs         : pAmTxE - pointer to TxAMEntity
 * Outputs        : None
 * Returns        : RLC_SUCCESS if successful else RLC_FAILURE
 * Description    : This internal function will be called by RLC
 *                  to send MaxReTxThreshold indication to RRC Layer at external interface.
 ********************************************************************************************/
/* SPR 5599_129123 Fix Start */
UInt32 rlcSendMaxReTxThresholdIndToRrc( TxAMEntity* pAmTxE )
/* SPR 5599_129123 Fix End */
{
    UInt32 ret = RLC_SUCCESS ;
    UInt8 rrcDataIndBuf[MAX_DATASIZE] = {0};
    UInt8 *rrcDataIndBuf_p = PNULL;
   /*As per SPR 698-4737 change start*/
    UInt16 rrcDataIdx = 0;
   /*As per SPR 698-4737 change end*/

    /* sending transaction id oxFFFF as dummy */
    UInt16 transactionId = 0xFFFF;

    /**  SPR 1842 Start **/
    /*SPR 6836 Fix start*/
    rrcDataIdx = RRC_RLC_API_HEADER_LEN;
    /*SPR 6836 Fix end*/
    /* + Layer2 NON CA Changes */
    rrcDataIndBuf_p = rrcDataIndBuf;

    RrcCellIndex cellId = 0;

#ifndef UE_SIM_TESTING
    cellId = RLC_GET_RRC_CELL_INDEX( 
            rlcUEContextList_g[pAmTxE->ueIndex]->cellIndex);
    /* - Layer2 NON CA Changes */

    UInt16 rrcUeIndex = rlcUEContextList_g[pAmTxE->ueIndex]->rrcUeIndex; 
    LTE_SET_U16BIT(&rrcDataIndBuf_p[rrcDataIdx],rrcUeIndex);
#else
    /* copying ueIdex */
    LTE_SET_U16BIT(&rrcDataIndBuf_p[rrcDataIdx],pAmTxE->ueIndex);
#endif
    /* - Layer2 NON CA Changes */
    rrcDataIdx += 2;
    /* copying lcId */
    rrcDataIndBuf_p[rrcDataIdx++] = pAmTxE->lcId;

    /* preparing the header */
    prepareCnfBufferHeader(rrcDataIndBuf_p
            , RLC_UE_ENTITY_ERROR_IND ,transactionId, cellId, rrcDataIdx);

    /** SPR 1842 End **/
    /* writting data to socket */
#ifdef LINUX_PC_TEST
    extern void rlcSetRetErrorMsgFlag(UInt8 Errorstatus);
    rlcSetRetErrorMsgFlag(RLC_TRUE);
    return ret;
#endif
    rlcSendMsgToRrc(rrcDataIndBuf, rrcDataIdx);
    return ret;
}
/* Rlc Rel 2.0 change end */


/****************************************************************************
 * Function Name  : rlcInitRrcInterface 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to initialize RRC interface queues
 *                  1. rlcRxPendingDelQ_g
 *                  2. rlcTxPendingDelQ_g
 *                  3. rlcRxReEstabQ_g
 *                  4. rlcTxReEstabQ_g
 *                  5. Initialize the Semaphore- processDelSem_g.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
/* + SPR 17439 */
void rlcInitRrcInterface(void)
/* - SPR 17439 */    
/* SPR 5599_129119 Fix End */
{
#ifndef UE_SIM_TESTING
    InternalCellIndex cellIndex = 0;
#endif 

/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    /* SPR 3805 Fix Start */
    QINIT_RLC_DEL_UE_ENTITY(rlcRxUePendingDelQ_g);
    QINIT_RLC_DEL_UE_ENTITY(rlcTxUePendingDelQ_g);
    /* SPR 3805 Fix End */
#endif
    /* initializing the pending del queue for rx entity*/
    QINIT_RLC_DEL_RE_ESTABQ(&rlcRxPendingDelQ_g);
    QINIT_RLC_DEL_RE_ESTABQ(&rlcRxReEstabQ_g);

#ifndef UE_SIM_TESTING
    /* initializing the pending del queue for rx entity*/
    for(;cellIndex < numOfRlcCellConfigured_g ; cellIndex++)
    {
	    /* initializing the pending del queue for tx entity*/
	    QINIT_RLC_DEL_RE_ESTABQ(&rlcTxPendingDelQ_g[cellIndex]);
	    /* initializing the ReEstablished queue for tx entity*/
	    QINIT_RLC_DEL_RE_ESTABQ(&rlcTxReEstabQ_g[cellIndex]);
    }
    /**Coverity 54591 start**/
	    /*SPR 16855 +-*/
    /*CID 110660 fix start*/
    QINIT_RLC_RECONFIG_ENTITY_Q(&rlcReconfigTxEntityQ_g); /**It need to be add cell index to the variable else this statement to be moved outside for loop. Coverity : 54591**/
    /*CID 110660 fix End*/
	    /*SPR 16855 +-*/
    /**Coverity 54591 end**/
#else
    /* initializing the pending del queue for tx entity*/
    QINIT_RLC_DEL_RE_ESTABQ(&rlcTxPendingDelQ_g);
    /* initializing the ReEstablished queue for rx entity*/
    QINIT_RLC_DEL_RE_ESTABQ(&rlcTxReEstabQ_g);
#endif


/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
    QINIT_RLC_RECONFIG_ENTITY_Q(&rlcReconfigRxEntityQ_g);
    /*+ SPR 21121*/
    QINIT_RLC_LP_TO_RLC_UL_SCH_Q(&rlcUlSchedToLPCircQ_g);
    QINIT_RLC_UL_SCH_TO_LP_Q(&rlcLPtoRlcUlSchedCircQ_g);
    /*- SPR 21121*/
#endif    

#ifdef RLC_STANDALONE_SETUP
    /* initialinzing the semaphore */
    semInit(&processDelSem_g,1);
#endif

/* SPR 3160 Fix End */
}

/****************************************************************************
 * Function Name  : rlcDeInitRrcInterface 
 * Inputs         : None
 * Outputs        : None
 * Returns        : RLC_SUCCESS if successful else RLC_FAILURE
 * Description    : This API is used to deinitialize RRC interface
 *                  1. rlcRxPendingDelQ_g
 *                  2. rlcTxPendingDelQ_g
 *                  3. rlcRxReEstabQ_g
 *                  4. rlcTxReEstabQ_g
 *                  5. DeInitialize the Semaphore- processDelSem_g.
 ****************************************************************************/

/* SPR 5599_129123 Fix Start */
/* + SPR 17439 */
UInt32 rlcDeInitRrcInterface(void)
/* - SPR 17439 */    
/* SPR 5599_129123 Fix End */
{
    UInt32 ret = RLC_SUCCESS ;
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    RlcUeDelQNode* ueEntity_p = RLC_NULL;
    /* SPR 3805 Fix Start */
    QDEINIT_RLC_DEL_UE_ENTITY(ueEntity_p,rlcRxUePendingDelQ_g);
    QDEINIT_RLC_DEL_UE_ENTITY(ueEntity_p,rlcTxUePendingDelQ_g);
    /* SPR 3805 Fix End */
#endif

    return ret ;
}

/****************************************************************************
 * Function Name  : rlcDeleteUEContext 
 * Inputs         : ueIndex - to identify the UE Context,
 *                  transactionId - identify the transaction,
 *                  cellId - RRC Cell Index
 *                  nodeCount - Count of Nodes
 * Outputs        : None
 * Returns        : RLC_SUCCESS if deleted successful else error message
 * Description    : This API will make an entry to of UE Context into pending
 *                  deletion queue.
 ****************************************************************************/

/* SPR 5599_129123 Fix Start */
UInt32 rlcDeleteUEContext( UInt16 ueIndex, 
/* SPR 5599_129123 Fix End */
                           UInt16 transactionId,
                           /* + Layer2 NON CA Changes */
                           RrcCellIndex cellId
                           /* - Layer2 NON CA Changes */
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                                  ,UInt8 *nodeCount
#endif
                                )
{
    UInt32 ret = RLC_SUCCESS;
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    /*SPR 3805 Fix Start */
    RlcUeDelQNode *rlcUeDelQNode_p = RLC_NULL;
    /*SPR 3805 Fix End */
    /* SPR 4172 Fix Start */
    UInt32 idx = 0;
    /* SPR 4172 Fix End */
#else
    RlcDelReEstabQNode *rlcDelReEstabQNode_p = RLC_NULL;
#endif
    /** CA Changes Start **/
#ifndef UE_SIM_TESTING
    InternalCellIndex cellIndex = RLC_GET_INTERNAL_CELL_INDEX(cellId);
    if( MAX_INTERNAL_CELL_INDEX < cellIndex )
    {
        LOG_RLC_MSG(LTE_HANDLE_RLC_DELETE_ENTITY, LOGWARNING,
            RLC_RRC, globalTTITickCount_g, __LINE__,
            ueIndex, transactionId, RLC_UE_NOT_EXISTS,
             0, 0, 0, __func__, "Invalid UE Index");
        return 0;
    }
#endif
    /** CA Changes End **/
    UEContext *ueContext_p = DB_FETCH_UE_CONTEXT(ueIndex);

    if(RLC_NULL == ueContext_p )
    {

        LOG_RLC_MSG(LTE_HANDLE_RLC_DELETE_ENTITY, LOGWARNING,
            RLC_RRC, globalTTITickCount_g, __LINE__,
            ueIndex, transactionId, RLC_UE_NOT_EXISTS,
             0, 0, 0, __func__, "Invalid UE Index");

        return RLC_UE_NOT_EXISTS;
    }
    
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    ueContext_p->rlcUEState    = RLC_UECONTEXT_PENDING_DELETION;
    ueContext_p->delProcessCnt = 0;

    /* SPR 4172 Fix Start */
    for ( ;(idx < MAX_LC_ID); idx++)
    {
        if(ueContext_p->rlcEntityInfoList[idx] != RLC_NULL)
        {
            switch (ueContext_p->rlcEntityInfoList[idx]->rlcEntityMode)
            {
                case RLC_ENTITY_AM:
                     rlcStopAllRunningAmTimers(ueContext_p->rlcEntityInfoList[idx]);
                break;
                case RLC_ENTITY_UM:
                     rlcStopAllRunningUmTimers(ueContext_p->rlcEntityInfoList[idx]);
                break;
                default:
                break;
            }
        }
    }
    /* SPR 4172 Fix End */

    /* making an entry to pending deletion queue */
    /*As per SPR 698, ltePanic already takes care of returning value,
     *no change done*/
    /* SPR 3805 Fix Start */
       CREATE_RLC_UE_DEL_Q_NODE(rlcUeDelQNode_p);
       /*coverity 25321 +*/
       if(PNULL == rlcUeDelQNode_p){
           return RLC_FAILURE;
       }
       /*coverity 25321 -*/
       rlcUeDelQNode_p->ueIndex       = ueIndex ;
       rlcUeDelQNode_p->transactionId = transactionId;
       rlcUeDelQNode_p->rlcNodeType   = RLC_UE_CONTEXT;
       /* enqueue the node in both pending deletion queue */
       ENQUEUE_DEL_UE_ENTITY_Q(rlcUeDelQNode_p,
                               rlcRxUePendingDelQ_g);
    /* SPR 3805 Fix End */

#ifdef RLC_STANDALONE_SETUP
    /*To make Rx thread Active*/
    semPost(&rlcRxSemlock_g);
#endif
    /* SPR 3805 Fix Start */
       rlcUeDelQNode_p = RLC_NULL;   
       CREATE_RLC_UE_DEL_Q_NODE(rlcUeDelQNode_p);
        /*coverity 25321 +*/
       if(PNULL == rlcUeDelQNode_p){
           return RLC_FAILURE;
       }
       /*coverity 25321 -*/
       rlcUeDelQNode_p->ueIndex       = ueIndex ;
       rlcUeDelQNode_p->transactionId = transactionId;
       rlcUeDelQNode_p->rlcNodeType   = RLC_UE_CONTEXT;

       ENQUEUE_DEL_UE_ENTITY_Q(rlcUeDelQNode_p,
                               rlcTxUePendingDelQ_g);       
    /* SPR 3805 Fix End */
#else
/* making an entry to pending deletion queue */
    /*As per SPR 698, ltePanic already takes care of returning value,
     *no change done*/
    CREATE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
    /* + Coverity_24569 */
    if (RLC_NULL == rlcDelReEstabQNode_p)
    {
        return RLC_FAILURE;    
    }    
    /* - Coverity_24569 */
    rlcDelReEstabQNode_p->ueIndex       = ueIndex ;
    /* SPR_10206_Fix Start */
    rlcDelReEstabQNode_p->cellId = cellId;
    /* SPR_10206_Fix End */
    rlcDelReEstabQNode_p->transactionId = transactionId;
    rlcDelReEstabQNode_p->rlcNodeType   = RLC_UE_CONTEXT;
    rlcDelReEstabQNode_p->entityType   = RLC_RX_ENTITY;
    /* SPR 21778 Fix */
    rlcDelReEstabQNode_p->reEstablishProcessTick   = 0;
    /* SPR 21778 Fix */
/* enqueue the node in both pending deletion queue */
    /*** Coverity 25581 Fix : Start ****/
    /** SPR 11010 Changes Start **/
    if (ENQUEUE_RLC_DEL_RE_ESTABQ(&rlcRxPendingDelQ_g, rlcDelReEstabQNode_p))
    {
        (*nodeCount)++;
    }
    else
    {
        LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGFATAL,
               			RLC_RRC, globalTTITickCount_g, __LINE__,
                     0, ueIndex, 0, 0, 0, 0, __func__, "ENQUEUE_RLC_DEL_RE_ESTABQ Failed");
        FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
        return RLC_FAILURE;
    }
/*+ SPR 21121*/
        RlcRrcOamHiPrioQNode *node_p = RLC_NULL;
        
        node_p = (RlcRrcOamHiPrioQNode *)getMemFromPool(sizeof(RlcRrcOamHiPrioQNode),RLC_NULL);
        if(PNULL == node_p)
        {    
            lteWarning("Memory Allocation fail %d",sizeof(node_p));
            return RLC_FAILURE;
        }    
        node_p->ueIndex =  ueIndex;
        node_p->cellId  =  cellId;
        node_p->transactionId =  transactionId;
        node_p->processRrcOamMsgEvent = RLC_ENTITY_PENDING_DELETION;
        node_p->rlcState = RLC_ENTITY_PENDING_DELETION;
        
        if(ENQUEUE_RLC_LP_TO_RLC_UL_SCH_Q(RlcRrcOamHiPrioQNode, node_p))
        {
            (*nodeCount)++;
        }
/*- SPR 21121*/

    /* SPR#11313 Fix changes */
    /** SPR 13293 Changes Start **/
    /* ueContext_p->rlcUEState = RLC_UECONTEXT_PENDING_DELETION;*/
    /** SPR 13293 Changes End **/
    /* SPR#11313 Fix changes */

#ifdef RLC_STANDALONE_SETUP
    /*To make Rx thread Active*/
    semPost(&rlcRxSemlock_g);
#endif

    rlcDelReEstabQNode_p = RLC_NULL;    
    CREATE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
    /* + Coverity_24569 */
    if (RLC_NULL == rlcDelReEstabQNode_p)
    {
        return RLC_FAILURE;
    }    
    /* - Coverity_24569 */
    rlcDelReEstabQNode_p->ueIndex       = ueIndex ;
    rlcDelReEstabQNode_p->cellId = cellId;
    rlcDelReEstabQNode_p->transactionId = transactionId;
    rlcDelReEstabQNode_p->rlcNodeType   = RLC_UE_CONTEXT;
    rlcDelReEstabQNode_p->entityType   = RLC_TX_ENTITY;
    /* SPR 21778 Fix */
    rlcDelReEstabQNode_p->reEstablishProcessTick   = 0;
    /* SPR 21778 Fix */
    /** CA Changes Start **/
    /*** Coverity 25581 Fix : Start ****/
    /** SPR 11010 Changes Start **/
    if (ENQUEUE_RLC_DEL_RE_ESTABQ(&rlcTxPendingDelQ_g[cellIndex], rlcDelReEstabQNode_p))
    {
        (*nodeCount)++;
    }
    else
    {
        LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGFATAL,
               			RLC_RRC, globalTTITickCount_g, __LINE__,
                     0, ueIndex, 0, 0, 0, 0, __func__, "ENQUEUE_RLC_DEL_RE_ESTABQ Failed");
        FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
        return RLC_FAILURE;
    }
    /** SPR 11010 Changes End **/
    /*** Coverity 25581 Fix : End ****/
    /** CA Changes End **/

    /* SPR#11313 Fix changes */
    /** SPR 13293 Changes Start **/
    /* ueContext_p->rlcUEState = RLC_UECONTEXT_PENDING_DELETION;*/
    /** SPR 13293 Changes End **/
    /* SPR#11313 Fix changes */
#endif


#ifdef RLC_STANDALONE_SETUP
    /*To make Tx thread Active*/
    semPost(&rlcTxSemlock_g);
#endif
    return ret;
}

/* SPR 4172 Fix Start */
/****************************************************************************
 * Function Name  : rlcStopAllRunningAmTimers
 * Inputs         : entityInfo_p - a pointer to RLCEntityInfo.
 * Outputs        : None
 * Returns        : None
 * Description    : This Function is used to stop all AM running timers.
 ******************************************************************************/
/* SPR 5599_129123 Fix Start */
void rlcStopAllRunningAmTimers(RLCEntityInfo *entityInfo_p)
/* SPR 5599_129123 Fix End */
{
    TxAMEntity *txAMEntity_p = RLC_NULL;
    RxAMEntity *rxAMEntity_p = RLC_NULL;

    switch(entityInfo_p->entityType)
    {
        case RLC_TX_RX_ENTITY:
             rxAMEntity_p = entityInfo_p->rlcEntity.amEntity_p->rxAMEntity_p;
             /* Stopping Status Prohibited Timer */
             if (RLC_TRUE == rxAMEntity_p->isSPTimerRunning)
             {
                 stopSPTimer(rxAMEntity_p);
                 rxAMEntity_p->isSPTimerRunning = RLC_FALSE;
             }
             /* Stopping Re-Odering Timer */
             if (RLC_TRUE == rxAMEntity_p->isRTimerRunning)
             {
                 stopAMROTimer(rxAMEntity_p);
                 rxAMEntity_p->isRTimerRunning = RLC_FALSE;
             }
             txAMEntity_p = entityInfo_p->rlcEntity.amEntity_p->txAMEntity_p;
             /*Stopping Poll Timer */
             if (RLC_TRUE == txAMEntity_p->istRetransmitTimerRunning)
             {
                 stopPRTimer( txAMEntity_p);
                 txAMEntity_p->istRetransmitTimerRunning = RLC_FALSE;
             }
        break;
        case RLC_TX_ENTITY:
             txAMEntity_p = entityInfo_p->rlcEntity.amEntity_p->txAMEntity_p;
             /*Stopping Poll Timer */
             if (RLC_TRUE == txAMEntity_p->istRetransmitTimerRunning)
             {
                 stopPRTimer( txAMEntity_p);
                 txAMEntity_p->istRetransmitTimerRunning = RLC_FALSE;
             }
        break;
        case RLC_RX_ENTITY:
             rxAMEntity_p = entityInfo_p->rlcEntity.amEntity_p->rxAMEntity_p;
             /* Stopping Status Prohibited Timer */
             if (RLC_TRUE == rxAMEntity_p->isSPTimerRunning)
             {
                 stopSPTimer(rxAMEntity_p);
                 rxAMEntity_p->isSPTimerRunning = RLC_FALSE;
             }
             /* Stopping Re-Odering Timer */
             if (RLC_TRUE == rxAMEntity_p->isRTimerRunning)
             {
                 stopAMROTimer(rxAMEntity_p);
                 rxAMEntity_p->isRTimerRunning = RLC_FALSE;
             }
        break;
        default:
        break;
    }
}

/****************************************************************************
 * Function Name  : rlcStopAllRunningUmTimers
 * Inputs         : entityInfo_p - a pointer to RLCEntityInfo.
 * Outputs        : None
 * Returns        : None
 * Description    : This Function is used to stop all UM running timers.
 *******************************************************************************/
/* SPR 5599_129123 Fix Start */
void rlcStopAllRunningUmTimers(RLCEntityInfo *entityInfo_p)
/* SPR 5599_129123 Fix End */
{
    RxUMEntity *rxUMEntity_p = RLC_NULL;

    switch(entityInfo_p->entityType)
    {
        case RLC_TX_RX_ENTITY:
        case RLC_RX_ENTITY:
             /* Stopping Re-Odering Timer */
             rxUMEntity_p = entityInfo_p->rlcEntity.umEntity_p->rxUMEntity_p;

             if (RLC_TRUE == rxUMEntity_p->isTimerRunning)
             {
                 stopUMROTimer(rxUMEntity_p);
                 rxUMEntity_p->isTimerRunning = RLC_FALSE;
             }
        break;
        default:
        break;
    }
}
/* SPR 4172 Fix End */


/****************************************************************************
 * Function Name  : rlcCreateUEEntity
 * Inputs         : ueIndex - Identifies the UE Context,
 *                  rnti - identifies UE for which Context is to be created,
 *                  rlcEntityCfgInfo_p - pointer to RlcEntityCfgInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS if created successfully error msg otherwise
 * Description    : This API is used to create & configure an RLC entity.
 ****************************************************************************/

/* SPR 5599_129123 Fix Start */
UInt16 rlcCreateUEEntity ( UInt16 ueIndex, 
/* SPR 5599_129123 Fix End */
                           UInt16 rnti, 
                           RlcEntityCfgInfo* rlcEntityCfgInfo_p
                         ) 
{
    RlcLcId lcId = rlcEntityCfgInfo_p->lcId;

    UEContext* outUEContext_p = DB_FETCH_UE_CONTEXT(ueIndex);

    if ( RLC_NULL == outUEContext_p )
    {
        LOG_RLC_MSG(LTE_RLC_CREATE_UE_CTX_ERR, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                ueIndex, rnti, lcId,
                RLC_UE_NOT_EXISTS, 0,0, __func__, "");

        return RLC_UE_NOT_EXISTS;
    }

    if ( outUEContext_p->rlcEntityInfoList[lcId] != RLC_NULL )
    {
        LOG_RLC_MSG(LTE_RLC_CREATE_UE_CTX_ERR_1, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                ueIndex, rnti, lcId,
                RLC_LCID_EXISTS, 0,0, __func__, "");
        return RLC_LCID_EXISTS;        
    }
    if ( rnti )
    {
        outUEContext_p->ueId = rnti ;
    }
    RlcEntityMode eM = rlcEntityCfgInfo_p->entityMode;
    RlcEntityType eT = rlcEntityCfgInfo_p->entityType;

    /* creating the entity */
    outUEContext_p->rlcEntityInfoList[lcId] = rlcCreateRLCEntityInfo( eM, eT);

    if ( RLC_FAILURE == outUEContext_p->rlcEntityInfoList[lcId])
    {
        LOG_RLC_MSG(LTE_RLC_CREATE_UE_CTX_ERR_2, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                ueIndex, rnti, lcId,
                eM, 0,0, __func__, "Mem allocation failed For Entity");
       return RLC_FAILURE;
    }
    /* configuring the entity */
    rlcConfigEntityParam ( outUEContext_p->rlcEntityInfoList[lcId],
            rlcEntityCfgInfo_p, ueIndex);

/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    /* setting initial state of rlc entity to ACTIVE */ 
    outUEContext_p->rlcEntityInfoList[lcId]->rlcEntityState = RLC_ENTITY_ACTIVE;                                            
#endif    
    gRlcStats.pRlcStats->lteRlcUeStat[ueIndex].numActiveLC ++;

    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : validateEntityTypeAndCtx
 * Inputs         : rlcEntityCfgInfo_p - pointer to RlcEntityCfgInfo,
 *                  outUEContext_p - pointer to UEContext
 * Outputs        : None
 * Returns        : RLC_SUCCESS if created successfully error msg otherwise
 * Description    : This API is used to validate the entityType and Ctx .
 ****************************************************************************/

static int validateEntityTypeAndCtx( 
                                              RlcEntityCfgInfo* rlcEntityCfgInfo_p,
                                              UEContext* outUEContext_p
                                           )
{
    UInt32 ret = RLC_SUCCESS;
    RlcLcId lcId = rlcEntityCfgInfo_p->lcId; 
    UMEntity *umEntity_p =  
        outUEContext_p->rlcEntityInfoList[lcId]->rlcEntity.umEntity_p;
    if (RLC_NULL == umEntity_p)
    {
        LOG_RLC_MSG ( LTE_RLC_INVALID_ENTITY, LOGWARNING,
                RLC_RRC, globalTTITickCount_g, __LINE__,
                lcId, RLC_UEENTITY_NOT_EXIST, 0, 0, 0, 0, __func__, "");
        return RLC_UEENTITY_NOT_EXIST;
    }
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
    if(umEntity_p->txEntityState == RLC_ENTITY_PENDING_DELETION || 
        umEntity_p->rxEntityState == RLC_ENTITY_PENDING_DELETION)
    {
        LOG_RLC_MSG(LTE_RLC_HANDLE_RECONFIG_ERR_3, LOGWARNING,
                RLC_RRC, globalTTITickCount_g, __LINE__,
                0, 0, 0, 0, 0, 0, __func__, "RLC_ENTITY_IN_PENDING_DEL_Q");
        return RLC_ENTITY_IN_PENDING_DEL_Q; 
    }
#endif

    if (( RLC_TX_RX_ENTITY == rlcEntityCfgInfo_p->entityType) ||
            (  RLC_RX_ENTITY  == rlcEntityCfgInfo_p->entityType) ||
            ( RLC_TX_ENTITY   == rlcEntityCfgInfo_p->entityType ))
    {
        switch (rlcEntityCfgInfo_p->entityType)
        {
            case RLC_TX_RX_ENTITY:
                if ((RLC_NULL == umEntity_p->rxUMEntity_p)
                        || (RLC_NULL == umEntity_p->txUMEntity_p ))
                    ret = RLC_TXRX_ENTITY_NOT_EXIST;
                break;
            case RLC_RX_ENTITY:
                if(RLC_NULL == umEntity_p->rxUMEntity_p)
                    ret = RLC_RX_ENTITY_NOT_EXIST;
                break;
            case RLC_TX_ENTITY:
                if(RLC_NULL == umEntity_p->txUMEntity_p)
                    ret = RLC_TX_ENTITY_NOT_EXIST;
                break;
                default:
                break;
        }
        return ret;
    }
    else
    {
        ret = INVALID_ENTITY_TYPE;
        LOG_RLC_MSG ( LTE_RLC_INVALID_ENTITY_ERR, LOGWARNING,
                RLC_RRC, globalTTITickCount_g, __LINE__,
                lcId, ret, 0, 0, 0, 0, __func__, "");
        return ret;
    }  

}

/* Cyclomatic Complexity changes - starts here */

/****************************************************************************
 * Function Name  : rlcReconfigUmTxRxUeEntity
 * Inputs         : rlcEntityInfoList- pointer to rlc Entity,
 *                : rlcEntityCfgInfo_p - pointer to RlcEntityCfgInfo,
 *                : ueIndex - Identifies the UE Context
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to Reconfigure an RLC entity.
 ****************************************************************************/
static void rlcReconfigUmTxRxUeEntity(
        RLCEntityInfo* rlcEntityInfoList,
        RlcEntityCfgInfo* rlcEntityCfgInfo_p,
        UInt16 ueIndex)
{
#ifndef UE_SIM_TESTING
    UInt8 tempQci = 0;
    UInt32 rateTx = 0; 
#endif
    rlcEntityInfoList->rlcEntity.umEntity_p->
        rxUMEntity_p->umROTimerInfo.roDuration = rlcEntityCfgInfo_p->
        configParam.umBiDirectional.rxUmRlc.tReordering;
                /** Rel 4.0 changes Start **/
#ifndef UE_SIM_TESTING
    if(rlcEntityCfgInfo_p->rlcQciInfo.qci)
    {
        tempQci = rlcEntityInfoList->rlcEntity.
                      umEntity_p->rxUMEntity_p->qci = 
                      rlcEntityInfoList->rlcEntity.
                      umEntity_p->txUMEntity_p->qci = 
                      rlcEntityCfgInfo_p->rlcQciInfo.qci;

        rlcEntityInfoList->rlcEntity.umEntity_p->
            rxUMEntity_p->bitRateRx = rlcEntityCfgInfo_p->configParam.
            umBiDirectional.rxUmRlc.bitRateRx;

        rateTx = rlcEntityInfoList->rlcEntity.
                 umEntity_p->txUMEntity_p->bitRateTx = 
                 rlcEntityCfgInfo_p->configParam.
                 umBiDirectional.txUmRlc.bitRateTx;
        if(rateTx == 0)
        {
            rateTx = rlcEntityInfoList->rlcEntity.
                    umEntity_p->txUMEntity_p->bitRateTx = 
                    rlcUeBitRateInfo_g[ueIndex].
                    bitRateTx; 

        }
        if(rlcEntityCfgInfo_p->configParam.umBiDirectional.txUmRlc.rlcQueueSize)
        {
            rlcEntityInfoList->rlcEntity.umEntity_p->
               txUMEntity_p->rlcQueueSize = rlcEntityCfgInfo_p->configParam.
               umBiDirectional.txUmRlc.rlcQueueSize;
        }
        else
        {
            if(rlcQosQciInfo_g[tempQci -1].eResType == RLC_GBR_LC)
            {
                rlcEntityInfoList->rlcEntity.umEntity_p->
                   txUMEntity_p->rlcQueueSize = ((rateTx *
                   rlcQosQciInfo_g[tempQci -1].pktDlyBdgt * 
                   qSizeFactor[qSizeFactorGBR_g - 1])/(1000 * 8));
            }
            else
            {
                rlcEntityInfoList->rlcEntity.umEntity_p->
                   txUMEntity_p->rlcQueueSize = ((rateTx *
                   rlcQosQciInfo_g[tempQci -1].pktDlyBdgt *
                   qSizeFactor[qSizeFactorNGBR_g -1])/(1000 * 8));
            }
        }    
    }   
#endif
   
}

#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : rlcReconfigUmTxUeEntity
 * Inputs         : rlcEntityInfoList- pointer to rlc Entity,
 *                : rlcEntityCfgInfo_p - pointer to RlcEntityCfgInfo,
 *                : ueIndex - Identifies the UE Context
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to Reconfigure an RLC entity.
 ****************************************************************************/
static void rlcReconfigUmTxUeEntity(
        RLCEntityInfo* rlcEntityInfoList,
        RlcEntityCfgInfo* rlcEntityCfgInfo_p,
        UInt16 ueIndex)
{
    UInt8 tempQci = 0;
    UInt32 rateTx = 0; 
    if(rlcEntityCfgInfo_p->rlcQciInfo.qci)
    {
        tempQci = rlcEntityInfoList->rlcEntity.
                  umEntity_p->txUMEntity_p->qci = 
                  rlcEntityCfgInfo_p->rlcQciInfo.qci;

        rateTx = rlcEntityInfoList->rlcEntity.
                 umEntity_p->txUMEntity_p->bitRateTx = 
                 rlcEntityCfgInfo_p->configParam.umBiDirectional.txUmRlc.bitRateTx;
       if(rateTx == 0)
       {
           rateTx = rlcEntityInfoList->rlcEntity.
                    umEntity_p->txUMEntity_p->bitRateTx = 
                    rlcUeBitRateInfo_g[ueIndex].
                    bitRateTx; 

       }
       if(rlcEntityCfgInfo_p->configParam.
              umBiDirectional.txUmRlc.rlcQueueSize)
       {
           rlcEntityInfoList->rlcEntity.umEntity_p->
               txUMEntity_p->rlcQueueSize = 
               rlcEntityCfgInfo_p->configParam.
               umBiDirectional.txUmRlc.rlcQueueSize;
       }
       else
       {
           if(rlcQosQciInfo_g[tempQci -1].eResType == RLC_GBR_LC)
           {
               rlcEntityInfoList->rlcEntity.umEntity_p->
                   txUMEntity_p->rlcQueueSize = 
                   ((rateTx *rlcQosQciInfo_g[tempQci -1].pktDlyBdgt * 
                   qSizeFactor[qSizeFactorGBR_g - 1])/(1000 * 8));
           }
           else
           {
               rlcEntityInfoList->rlcEntity.umEntity_p->
                   txUMEntity_p->rlcQueueSize = 
                   ((rateTx *rlcQosQciInfo_g[tempQci -1].pktDlyBdgt *
                   qSizeFactor[qSizeFactorNGBR_g -1])/(1000 * 8));
           }
       }
   }
}

#endif

/****************************************************************************
 * Function Name  : rlcReconfigUmRxUeEntity
 * Inputs         : rlcEntityInfoList- pointer to rlc Entity,
 *                : rlcEntityCfgInfo_p - pointer to RlcEntityCfgInfo
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to Reconfigure an RLC entity.
 ****************************************************************************/
static void rlcReconfigUmRxUeEntity(
        RLCEntityInfo* rlcEntityInfoList,
        RlcEntityCfgInfo* rlcEntityCfgInfo_p)
{
    rlcEntityInfoList->rlcEntity.umEntity_p->
        rxUMEntity_p->umROTimerInfo.roDuration = rlcEntityCfgInfo_p->
        configParam.umUniDirectionalRx.rxUmRlc.tReordering;
    /** Rel 4.0 changes Start **/
#ifndef UE_SIM_TESTING
    if(rlcEntityCfgInfo_p->rlcQciInfo.qci)
    {
        rlcEntityInfoList->rlcEntity.
            umEntity_p->rxUMEntity_p->qci = 
            rlcEntityCfgInfo_p->rlcQciInfo.qci;

        rlcEntityInfoList->rlcEntity.umEntity_p->
            rxUMEntity_p->bitRateRx = 
            rlcEntityCfgInfo_p->configParam.
            umUniDirectionalRx.rxUmRlc.bitRateRx;
    }
#endif
    /** Rel 4.0 changes End **/
}

/****************************************************************************
 * Function Name  : rlcReconfigAmTxRxUeEntity
 * Inputs         : rlcEntityInfoList- pointer to rlc Entity,
 *                : rlcEntityCfgInfo_p - pointer to RlcEntityCfgInfo,
 *                : amEntity_p - pointer to AmEntity,
 *                : ueIndex - Identifies the UE Context
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to Reconfigure an RLC entity.
 ****************************************************************************/
static void rlcReconfigAmTxRxUeEntity(
        RLCEntityInfo* rlcEntityInfoList,
        RlcEntityCfgInfo* rlcEntityCfgInfo_p,
        AMEntity* amEntity_p,
        UInt16 ueIndex)
{
#ifndef UE_SIM_TESTING
    UInt8 tempQci = 0;
    UInt32 rateTx = 0; 
#endif
    TxAMEntity* txAMEntity_p = (amEntity_p->txAMEntity_p);
    TxAmRlc *   txAmRlc_p    = &(rlcEntityCfgInfo_p->configParam.am.txAmRlc);
    RxAMEntity * rxAMEntity_p = amEntity_p-> rxAMEntity_p;
    RxAmRlc *rxAmRlc_p        =  &(rlcEntityCfgInfo_p->configParam. am.rxAmRlc); 
    rxAMEntity_p->amROTimerInfo.roDuration  = rxAmRlc_p->tReordering;
    rxAMEntity_p->spTimerInfo.spDuration    = rxAmRlc_p->tStatusProhibit;
    txAMEntity_p->prTimerInfo.prDuration    = txAmRlc_p->tPollRetransmit;

    txAMEntity_p->pollPDU          = txAmRlc_p->pollPDU;
    txAMEntity_p->pollByte         = txAmRlc_p->pollByte;
    txAMEntity_p->maxRetxThreshold = txAmRlc_p->maxRetxThreshold;
    /** Rel 4.0 changes Start **/
#ifndef UE_SIM_TESTING
    if(rlcEntityCfgInfo_p->rlcQciInfo.qci)
    {
        tempQci = rlcEntityInfoList->rlcEntity.
                  amEntity_p->txAMEntity_p->qci = 
                  rlcEntityCfgInfo_p->rlcQciInfo.qci;

        rateTx =  rlcEntityInfoList->rlcEntity.
                  amEntity_p->txAMEntity_p->bitRateTx = 
                  rlcEntityCfgInfo_p->configParam.
                  am.txAmRlc.bitRateTx;
        if(rateTx == 0)
        {
            rateTx = rlcEntityInfoList->rlcEntity.
                     amEntity_p->txAMEntity_p->bitRateTx = 
                     rlcUeBitRateInfo_g[ueIndex].
                     bitRateTx;
        }
        rlcEntityInfoList->rlcEntity.amEntity_p->
                rxAMEntity_p->bitRateRx = rlcEntityCfgInfo_p->configParam.
                am.rxAmRlc.bitRateRx;
        if((rlcEntityCfgInfo_p->configParam.am.txAmRlc.rlcQueueSize))
        {
            rlcEntityInfoList->rlcEntity.amEntity_p->
                txAMEntity_p->rlcQueueSize = 
                rlcEntityCfgInfo_p->configParam.
                am.txAmRlc.rlcQueueSize;
        }
        else
        {
            if(rlcQosQciInfo_g[tempQci -1].eResType == RLC_GBR_LC )
            {
                rlcEntityInfoList->rlcEntity.
                    amEntity_p->txAMEntity_p->rlcQueueSize = 
                    (rateTx)/(1000 * 8) *
                    (rlcQosQciInfo_g[tempQci -1].pktDlyBdgt *
                     qSizeFactor[qSizeFactorGBR_g - 1]);
            }
            else
            {
                rlcEntityInfoList->rlcEntity.
                   amEntity_p->txAMEntity_p->rlcQueueSize = 
                   (rateTx)/(1000 * 8) *
                   (rlcQosQciInfo_g[tempQci -1].pktDlyBdgt *
                   qSizeFactor[qSizeFactorNGBR_g - 1]);
            } 
        }

    }
#endif
    /** Rel 4.0 changes end **/
}
/* Cyclomatic Complexity changes - ends here */

/*RLC Rel 2.0 Changes Start*/
/****************************************************************************
 * Function Name  : rlcReconfigUEEntity
 * Inputs         : ueIndex - Identifies the UE Context,
 *                  rlcEntityCfgInfo_p - pointer to RlcEntityCfgInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS if created successfully error msg otherwise
 * Description    : This API is used to create & configure an RLC entity.
 ****************************************************************************/

/* SPR 5599_129123 Fix Start */
UInt16 rlcReconfigUEEntity( UInt16 ueIndex,
/* SPR 5599_129123 Fix End */
                            RlcEntityCfgInfo* rlcEntityCfgInfo_p
                          )
{
    UInt16 ret   = RLC_SUCCESS ;
    RlcLcId lcId = rlcEntityCfgInfo_p->lcId;
    UEContext* outUEContext_p = DB_FETCH_UE_CONTEXT(ueIndex);

    if ( RLC_NULL == outUEContext_p )
    {
        LOG_RLC_MSG ( LTE_RLC_INVALID_UECONTEXT, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                ueIndex, lcId,RLC_UE_NOT_EXISTS , 0, 0,0, __func__, "");

        return RLC_UE_NOT_EXISTS;
    }

    if ( RLC_NULL == outUEContext_p->rlcEntityInfoList[lcId])
    {
        LOG_RLC_MSG ( LTE_RLC_INVALID_ENTITY_1, LOGWARNING, RLC_RRC,
                globalTTITickCount_g, __LINE__, ueIndex, lcId, RLC_LCID_NOT_EXISTS, 0,
                0,0, __func__, "");

        return RLC_LCID_NOT_EXISTS;        
    }

    if((rlcEntityCfgInfo_p->entityMode !=
         outUEContext_p->rlcEntityInfoList[lcId]->rlcEntityMode)||
        (rlcEntityCfgInfo_p->entityType !=
          outUEContext_p->rlcEntityInfoList[lcId]->entityType))
    {

        return RLC_ENTITY_NOT_EXIST;

    }

    if ( RLC_ENTITY_UM == rlcEntityCfgInfo_p->entityMode)
    {
        ret = 
            validateEntityTypeAndCtx( rlcEntityCfgInfo_p, outUEContext_p );

        if ( RLC_SUCCESS == ret)
        {
            switch(rlcEntityCfgInfo_p->entityType)
            {
                case RLC_TX_RX_ENTITY:
                    /* Cyclomatic Complexity changes - starts here */
                    rlcReconfigUmTxRxUeEntity(outUEContext_p->rlcEntityInfoList[lcId],
                            rlcEntityCfgInfo_p,ueIndex);
                    /* Cyclomatic Complexity changes - ends here */                    
                break;

                case RLC_RX_ENTITY:
                    /* Cyclomatic Complexity changes - starts here */
                    rlcReconfigUmRxUeEntity(outUEContext_p->rlcEntityInfoList[lcId],
                            rlcEntityCfgInfo_p);
                    /* Cyclomatic Complexity changes - ends here */                    
                break;

                case RLC_TX_ENTITY:
                    /* Cyclomatic Complexity changes - starts here */
#ifndef UE_SIM_TESTING
                    rlcReconfigUmTxUeEntity(outUEContext_p->rlcEntityInfoList[lcId],
                            rlcEntityCfgInfo_p,ueIndex);
#endif
                    /* Cyclomatic Complexity changes - ends here */                    
                break;

                default:
                LOG_RLC_MSG ( LTE_RLC_INVALID_ENTITY_ERR_1, LOGWARNING, 
                        RLC_RRC, globalTTITickCount_g, __LINE__,
                        ueIndex, lcId, INVALID_ENTITY_TYPE , 0, 0,0, __func__, "");

                return INVALID_ENTITY_TYPE;
            }

        }
        else
        {        
            LOG_RLC_MSG ( LTE_RLC_INVALID_ENTITY_ERR_2, LOGWARNING, 
                    RLC_RRC, globalTTITickCount_g, __LINE__,
                    ueIndex, lcId, ret, 0, 0,0, __func__, "");
            return ret;
        }
    }
    else if (RLC_ENTITY_AM == rlcEntityCfgInfo_p->entityMode)
    {
        if ( RLC_TX_RX_ENTITY == rlcEntityCfgInfo_p->entityType)
        {
            AMEntity* amEntity_p = 
                outUEContext_p->rlcEntityInfoList[lcId]->
                rlcEntity.amEntity_p; 

            if (( RLC_NULL == amEntity_p) ||
                    ( RLC_NULL == amEntity_p->rxAMEntity_p) ||
                    ( RLC_NULL == amEntity_p->txAMEntity_p))
            {
                LOG_RLC_MSG ( LTE_RLC_INVALID_ENTITY_2, LOGWARNING, 
                        RLC_RRC, globalTTITickCount_g, __LINE__,
                        ueIndex, lcId, RLC_UEENTITY_NOT_EXIST, 0, 0, 0, __func__, "");

                return RLC_UEENTITY_NOT_EXIST;
            }
            /* Cyclomatic Complexity changes - starts here */
            rlcReconfigAmTxRxUeEntity(outUEContext_p->rlcEntityInfoList[lcId],
                rlcEntityCfgInfo_p,amEntity_p,ueIndex);
            /* Cyclomatic Complexity changes - ends here */                   

            LOG_RLC_MSG(LTE_RLC_RECONFIG_UE_ENTITY_1, LOGINFO, RLC_RRC, globalTTITickCount_g, 
                    ueIndex,lcId, rlcEntityCfgInfo_p->configParam.am.rxAmRlc.tReordering,
                    rlcEntityCfgInfo_p->configParam.am.rxAmRlc.tStatusProhibit,
                    rlcEntityCfgInfo_p->configParam.am.txAmRlc.tPollRetransmit,0,0, __func__, "");

            LOG_RLC_MSG(LTE_RLC_RECONFIG_UE_ENTITY_2, LOGINFO, RLC_RRC, globalTTITickCount_g,
                    ueIndex,lcId, rlcEntityCfgInfo_p->configParam.am.txAmRlc.pollPDU,
                    rlcEntityCfgInfo_p->configParam.am.txAmRlc.pollByte,
                    rlcEntityCfgInfo_p->configParam.am.txAmRlc.maxRetxThreshold,0,0, __func__, "");
        }
    }
    else 
    {
        LOG_RLC_MSG ( LTE_RLC_INVALID_ENTITY_ERR_3, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                ueIndex, lcId,RLC_SYNTAX_ERROR , 0, 0,0, __func__, "");
        return RLC_SYNTAX_ERROR;
    }

    return ret;
}
/*RLC Rel 2.0 Changes End*/

/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING

/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : rlcUpdateUmTxRxQciInfo
 * Inputs         : rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo,
 *                  rlcTxEntityCfgInfo_p - Pointer to RlcTxEntityCfgInfo,
 *                  rlcRxEntityCfgInfo_p - Pointer to RlcRxEntityCfgInfo
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to Update UM TX RX QCI info in RLC.
 ****************************************************************************/
static void rlcUpdateUmTxRxQciInfo
(
     RlcEntityCfgInfo* rlcEntityCfgInfo_p,
     RlcTxEntityCfgInfo *rlcTxEntityCfgInfo_p,
     RlcRxEntityCfgInfo *rlcRxEntityCfgInfo_p,
     UInt16 ueIndex
)
{
    UInt8  tempQci = 0;
    UInt32 rateTx = 0;
    if(rlcEntityCfgInfo_p->rlcQciInfo.qci)
    {
        tempQci = rlcTxEntityCfgInfo_p->rlcQciInfo.qci =
                  rlcRxEntityCfgInfo_p->rlcQciInfo.qci= 
                  rlcEntityCfgInfo_p->rlcQciInfo.qci;
        
        rlcRxEntityCfgInfo_p->rxRlc.rxUmRlc.bitRateRx =
                  rlcEntityCfgInfo_p->configParam.umBiDirectional.rxUmRlc.bitRateRx;

        rateTx =  rlcTxEntityCfgInfo_p->txRlc.txUmRlc.bitRateTx = 
                  rlcEntityCfgInfo_p->configParam.umBiDirectional.txUmRlc.bitRateTx;
        if(rateTx == 0)
        {
            rateTx = rlcTxEntityCfgInfo_p->txRlc.txUmRlc.bitRateTx =
                     rlcUeBitRateInfo_g[ueIndex].bitRateTx;
        }

        if(rlcEntityCfgInfo_p->configParam.umBiDirectional.txUmRlc.rlcQueueSize)
        {
            rlcTxEntityCfgInfo_p->txRlc.txUmRlc.rlcQueueSize = 
                rlcEntityCfgInfo_p->configParam.umBiDirectional.txUmRlc.rlcQueueSize;
        }
        else
        {
            if(rlcQosQciInfo_g[tempQci -1].eResType == RLC_GBR_LC)
            {
                rlcTxEntityCfgInfo_p->txRlc.txUmRlc.rlcQueueSize = ((rateTx *
                   rlcQosQciInfo_g[tempQci -1].pktDlyBdgt * 
                   qSizeFactor[qSizeFactorGBR_g - 1])/(1000 * 8));
            }
            else
            {
                rlcTxEntityCfgInfo_p->txRlc.txUmRlc.rlcQueueSize = ((rateTx *
                   rlcQosQciInfo_g[tempQci -1].pktDlyBdgt *
                   qSizeFactor[qSizeFactorNGBR_g -1])/(1000 * 8));
            }
        }
    }
    else
    {
        rlcTxEntityCfgInfo_p->rlcQciInfo.qci =
            rlcRxEntityCfgInfo_p->rlcQciInfo.qci = 0; 

    }
}


/****************************************************************************
 * Function Name  : rlcUpdateUmTxQciInfo
 * Inputs         : rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo,
 *                  rlcTxEntityCfgInfo_p - Pointer to RlcTxEntityCfgInfo,
 *                  ueIndex - Index of UE,
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to Update UM TX QCI info in RLC.
 ****************************************************************************/
static inline void rlcUpdateUmTxQciInfo
(
     RlcEntityCfgInfo* rlcEntityCfgInfo_p,
     RlcTxEntityCfgInfo *rlcTxEntityCfgInfo_p,
     UInt16 ueIndex
)
{
    UInt8  tempQci = 0;
    UInt32 rateTx = 0;
    if(rlcEntityCfgInfo_p->rlcQciInfo.qci)
    {
        tempQci = rlcTxEntityCfgInfo_p->rlcQciInfo.qci =
                  rlcEntityCfgInfo_p->rlcQciInfo.qci;
       
        rateTx = rlcTxEntityCfgInfo_p->txRlc.txUmRlc.bitRateTx =
                 rlcEntityCfgInfo_p->configParam.umUniDirectionalTX.txUmRlc.bitRateTx;
        
        if(rateTx == 0)
        {
            rateTx = rlcTxEntityCfgInfo_p->txRlc.txUmRlc.bitRateTx =
                     rlcUeBitRateInfo_g[ueIndex].bitRateTx;
        }

        if(rlcEntityCfgInfo_p->configParam.umUniDirectionalTX.txUmRlc.rlcQueueSize)
        {
            rlcTxEntityCfgInfo_p->txRlc.txUmRlc.rlcQueueSize = 
               rlcEntityCfgInfo_p->configParam.umUniDirectionalTX.txUmRlc.rlcQueueSize;
        }
        else
        {
            if(rlcQosQciInfo_g[tempQci -1].eResType == RLC_GBR_LC)
            {
                rlcTxEntityCfgInfo_p->txRlc.txUmRlc.rlcQueueSize = ((rateTx *
                   rlcQosQciInfo_g[tempQci -1].pktDlyBdgt * 
                   qSizeFactor[qSizeFactorGBR_g - 1])/(1000 * 8));
            }
            else
            {
                rlcTxEntityCfgInfo_p->txRlc.txUmRlc.rlcQueueSize = ((rateTx *
                   rlcQosQciInfo_g[tempQci -1].pktDlyBdgt *
                   qSizeFactor[qSizeFactorNGBR_g -1])/(1000 * 8));
            }
        }
    }
    else
    {
        rlcTxEntityCfgInfo_p->rlcQciInfo.qci = 0;

    }
}

/****************************************************************************
 * Function Name  : rlcUpdateUmRxQciInfo
 * Inputs         : rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo,
 *                  rlcRxEntityCfgInfo_p - Pointer to RlcRxEntityCfgInfo,
 *                  ueIndex - Index of UE
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to Update UM RX QCI info in RLC.
 ****************************************************************************/
static inline void rlcUpdateUmRxQciInfo
(
     RlcEntityCfgInfo* rlcEntityCfgInfo_p,
     RlcRxEntityCfgInfo *rlcRxEntityCfgInfo_p
     /* +- SPR 17777 */
)
{
    if(rlcEntityCfgInfo_p->rlcQciInfo.qci)
    {
        rlcRxEntityCfgInfo_p->rlcQciInfo.qci = 
            rlcEntityCfgInfo_p->rlcQciInfo.qci;

        rlcRxEntityCfgInfo_p->rxRlc.rxUmRlc.bitRateRx =
            rlcEntityCfgInfo_p->configParam.umUniDirectionalRx.rxUmRlc.bitRateRx;
    }
    else
    {
        rlcRxEntityCfgInfo_p->rlcQciInfo.qci = 0;
    }
}

/****************************************************************************
 * Function Name  : rlcUpdateAmQciInfo
 * Inputs         : rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo,
 *                  rlcTxEntityCfgInfo_p - Pointer to RlcTxEntityCfgInfo,
 *                  rlcRxEntityCfgInfo_p - Pointer to RlcRxEntityCfgInfo,
 *                  ueIndex - Index of UE
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to Update AM QCI info in RLC.
 ****************************************************************************/
static inline void rlcUpdateAmQciInfo
(
     RlcEntityCfgInfo* rlcEntityCfgInfo_p,
     RlcTxEntityCfgInfo *rlcTxEntityCfgInfo_p,
     RlcRxEntityCfgInfo *rlcRxEntityCfgInfo_p,
     UInt16 ueIndex
)
{
    UInt8  tempQci = 0;
    UInt32 rateTx = 0;
    if(rlcEntityCfgInfo_p->rlcQciInfo.qci)
    {
        tempQci = rlcTxEntityCfgInfo_p->rlcQciInfo.qci = 
                  rlcRxEntityCfgInfo_p->rlcQciInfo.qci =
                  rlcEntityCfgInfo_p->rlcQciInfo.qci;

        rateTx = rlcTxEntityCfgInfo_p->txRlc.txAmRlc.bitRateTx = 
                 rlcEntityCfgInfo_p->configParam.am.txAmRlc.bitRateTx;
        if(rateTx == 0)
        {
            rateTx = rlcTxEntityCfgInfo_p->txRlc.txAmRlc.bitRateTx =
                     rlcUeBitRateInfo_g[ueIndex].bitRateTx;
        }
        rlcRxEntityCfgInfo_p->rxRlc.rxAmRlc.bitRateRx = 
               rlcEntityCfgInfo_p->configParam.am.rxAmRlc.bitRateRx;

        if((rlcEntityCfgInfo_p->configParam.am.txAmRlc.rlcQueueSize))
        {                    
            rlcTxEntityCfgInfo_p->txRlc.txAmRlc.rlcQueueSize = 
              rlcEntityCfgInfo_p->configParam.am.txAmRlc.rlcQueueSize;
        }
        else
        {
            if(rlcQosQciInfo_g[tempQci -1].eResType == RLC_GBR_LC )
            {
                 rlcTxEntityCfgInfo_p->txRlc.txAmRlc.rlcQueueSize = (rateTx)/(1000 * 8) *
                    (rlcQosQciInfo_g[tempQci -1].pktDlyBdgt *
                    qSizeFactor[qSizeFactorGBR_g - 1]);
            }  
            else
            {
                rlcTxEntityCfgInfo_p->txRlc.txAmRlc.rlcQueueSize = (rateTx)/(1000 * 8) *
                    (rlcQosQciInfo_g[tempQci -1].pktDlyBdgt *
                    qSizeFactor[qSizeFactorNGBR_g - 1]);
            } 
         }
    }
    else
    {
         rlcTxEntityCfgInfo_p->rlcQciInfo.qci = 
         rlcRxEntityCfgInfo_p->rlcQciInfo.qci = 0;
    }
}


/****************************************************************************
 * Function Name  : rlcReconfigUmTxRxEntity
 * Inputs         : rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo,
 *                  ueIndex - Index of UE,
 *                  lcId - Logical channel ID,
 *                  nodeCount - Node count
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to Re-configure UM TX RX Entity in RLC.
 ****************************************************************************/
static inline void rlcReconfigUmTxRxEntity(RlcEntityCfgInfo* rlcEntityCfgInfo_p,
             UInt16 ueIndex,
             UInt16 lcId,
             UInt8 *nodeCount,
             /* Layer 2 NON CA Changes */
             UInt16 transactionId,
             RrcCellIndex cellId
             /* Layer 2 NON CA Changes */
)
{
    RlcTxEntityCfgInfo *rlcTxEntityCfgInfo_p = RLC_NULL;
    RlcRxEntityCfgInfo *rlcRxEntityCfgInfo_p = RLC_NULL;
    /** CA Changes Start **/
    InternalCellIndex cellIndex = RLC_GET_INTERNAL_CELL_INDEX(cellId);

    if( MAX_INTERNAL_CELL_INDEX < cellIndex )
    {
        LOG_RLC_MSG(LTE_HANDLE_RLC_DELETE_ENTITY, LOGWARNING,
            RLC_RRC, globalTTITickCount_g, __LINE__,
            ueIndex, transactionId, RLC_UE_NOT_EXISTS,
             0, 0, 0, __func__, "Invalid cell ID");
         return;
    }
    /** CA Changes End **/
    
    rlcTxEntityCfgInfo_p = getMemFromPool( sizeof(RlcTxEntityCfgInfo) ,RLC_NULL);
    if( RLC_NULL == rlcTxEntityCfgInfo_p)
    {
        ltePanic("%s => Memory Allocation Failure",__FUNCTION__);
        /* coverity_53493 Fix Start */
        return ;
        /* coverity_53493 Fix End */
    }
    rlcRxEntityCfgInfo_p = getMemFromPool( sizeof(RlcRxEntityCfgInfo) ,RLC_NULL);
    if( RLC_NULL == rlcRxEntityCfgInfo_p)
    {
        ltePanic("%s => Memory Allocation Failure",__FUNCTION__);
        /* coverity_53492 Fix Start */
        freeMemPool(rlcTxEntityCfgInfo_p);
        return ;
        /* coverity_53492 Fix End */
    }
    rlcTxEntityCfgInfo_p->ueIndex = rlcRxEntityCfgInfo_p->ueIndex = ueIndex;
    rlcTxEntityCfgInfo_p->lcId = rlcRxEntityCfgInfo_p->lcId = lcId;
    rlcTxEntityCfgInfo_p->entityMode = rlcRxEntityCfgInfo_p->entityMode = RLC_ENTITY_UM;
    rlcRxEntityCfgInfo_p->rxRlc.rxUmRlc.tReordering = 
         rlcEntityCfgInfo_p->configParam.umBiDirectional.rxUmRlc.tReordering;
    /* + Layer 2 NON CA Changes */
    rlcTxEntityCfgInfo_p->transactionId = transactionId;
    rlcTxEntityCfgInfo_p->cellId = cellId;
    rlcRxEntityCfgInfo_p->transactionId = transactionId;
    rlcRxEntityCfgInfo_p->cellId = cellId;
    /* - Layer 2 NON CA Changes */
    rlcUpdateUmTxRxQciInfo(rlcEntityCfgInfo_p,rlcTxEntityCfgInfo_p,
         rlcRxEntityCfgInfo_p,ueIndex);
    /** CA Changes Start **/
    /*** Coverity 63527 Fix : Start ****/
    if (!ENQUEUE_RLC_RECONFIG_ENTITY_Q(&rlcReconfigTxEntityQ_g, rlcTxEntityCfgInfo_p))
    {
	LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGFATAL,
               			RLC_RRC, globalTTITickCount_g, __LINE__,
             			0,ueIndex, 0, 0, 0, 0, __func__, "ENQUEUE_RLC_RECONFIG_ENTITY_Q Failed");
        freeMemPool(rlcTxEntityCfgInfo_p);
        freeMemPool(rlcRxEntityCfgInfo_p);
        return ;
    }
    else
    {
        (*nodeCount)++;
    }
    /** CA Changes End **/
    if (!ENQUEUE_RLC_RECONFIG_ENTITY_Q((&(rlcReconfigRxEntityQ_g)), rlcRxEntityCfgInfo_p))
    {
	LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGFATAL,
               			RLC_RRC, globalTTITickCount_g, __LINE__,
             			0,ueIndex, 0, 0, 0, 0, __func__, "ENQUEUE_RLC_RECONFIG_ENTITY_Q Failed");
        freeMemPool(rlcRxEntityCfgInfo_p);
        return ;
    }
    else
    {
        (*nodeCount)++;
    }
    /*** Coverity 63527 Fix : End ****/
    return;
}

/****************************************************************************
 * Function Name  : rlcReconfigUmRxEntity
 * Inputs         : rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo,
 *                  ueIndex - Index of UE,
 *                  lcId - Logical channel ID,
 *                  nodeCount - Node count,
 *                  transactionId,
 *                  cellId - RRC Cell Index
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to Re-configure UM RX Entity in RLC.
 ****************************************************************************/
static inline void rlcReconfigUmRxEntity(RlcEntityCfgInfo* rlcEntityCfgInfo_p,
             UInt16 ueIndex,
             UInt16 lcId,
             UInt8 *nodeCount,
             /* Layer 2 NON CA Changes */
             UInt16 transactionId,
             RrcCellIndex cellId
             /* Layer 2 NON CA Changes */
             )
{
    RlcRxEntityCfgInfo *rlcRxEntityCfgInfo_p = RLC_NULL;
    rlcRxEntityCfgInfo_p = getMemFromPool( sizeof(RlcRxEntityCfgInfo) ,RLC_NULL);
    if( RLC_NULL == rlcRxEntityCfgInfo_p)
    {
        ltePanic("%s => Memory Allocation Failure",__FUNCTION__);
    /* coverity_53490 Fix Start */
         return ;
    /* coverity_53490 Fix End */
    }
    rlcRxEntityCfgInfo_p->ueIndex = ueIndex;
    rlcRxEntityCfgInfo_p->lcId = lcId;
    rlcRxEntityCfgInfo_p->entityMode = RLC_ENTITY_UM;
    /* + Layer 2 NON CA Changes */
    rlcRxEntityCfgInfo_p->transactionId = transactionId;
    rlcRxEntityCfgInfo_p->cellId = cellId;
    /* - Layer 2 NON CA Changes */

    rlcRxEntityCfgInfo_p->rxRlc.rxUmRlc.tReordering = 
        rlcEntityCfgInfo_p->configParam.umUniDirectionalRx.rxUmRlc.tReordering;

    /* +- SPR 17777 */
    rlcUpdateUmRxQciInfo(rlcEntityCfgInfo_p,rlcRxEntityCfgInfo_p);
    /* +- SPR 17777 */
    /* send req to UL RLC threads*/
    /*** Coverity 63525 Fix : Start ****/
    if (!ENQUEUE_RLC_RECONFIG_ENTITY_Q((&(rlcReconfigRxEntityQ_g)), rlcRxEntityCfgInfo_p))
    {
	LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGFATAL,
               			RLC_RRC, globalTTITickCount_g, __LINE__,
             			0,ueIndex, 0, 0, 0, 0, __func__, "ENQUEUE_RLC_RECONFIG_ENTITY_Q Failed");       
 	freeMemPool(rlcRxEntityCfgInfo_p);
    }
    else
    {
        (*nodeCount)++;
    }
    /*** Coverity 63525 Fix : End ****/
}

/****************************************************************************
 * Function Name  : rlcReconfigUmTxEntity
 * Inputs         : rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo,
 *                  ueIndex - Index of UE,
 *                  lcId - Logical channel ID,
 *                  nodeCount - Node count,
 *                  transactionId,
 *                  cellId - RRC Cell Index
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to Re-configure UM TX Entity in RLC.
 ****************************************************************************/
static inline void rlcReconfigUmTxEntity(RlcEntityCfgInfo* rlcEntityCfgInfo_p,
             UInt16 ueIndex,
             UInt16 lcId,
             UInt8 *nodeCount,
             /* Layer 2 NON CA Changes */
             UInt16 transactionId,
             RrcCellIndex cellId
             /* Layer 2 NON CA Changes */
)
{
    RlcTxEntityCfgInfo *rlcTxEntityCfgInfo_p = RLC_NULL;
    /** CA Changes Start **/
    InternalCellIndex cellIndex = RLC_GET_INTERNAL_CELL_INDEX(cellId);
    if( MAX_INTERNAL_CELL_INDEX < cellIndex )
    {
        LOG_RLC_MSG(LTE_HANDLE_RLC_DELETE_ENTITY, LOGWARNING,
            RLC_RRC, globalTTITickCount_g, __LINE__,
            ueIndex, transactionId, RLC_UE_NOT_EXISTS,
             0, 0, 0, __func__, "Invalid cell ID");
         return;
    }
    /** CA Changes End **/
    rlcTxEntityCfgInfo_p = getMemFromPool( sizeof(RlcTxEntityCfgInfo) ,RLC_NULL);
    if( RLC_NULL == rlcTxEntityCfgInfo_p)
    {
        ltePanic("%s => Memory Allocation Failure",__FUNCTION__);
        /* coverity_53491 Fix Start */
        return ;
        /* coverity_53491 Fix End */
    }
    rlcTxEntityCfgInfo_p->ueIndex = ueIndex;
    rlcTxEntityCfgInfo_p->lcId = lcId;
    rlcTxEntityCfgInfo_p->entityMode = RLC_ENTITY_UM;
    /* + Layer 2 NON CA Changes */
    rlcTxEntityCfgInfo_p->transactionId = transactionId;
    rlcTxEntityCfgInfo_p->cellId = cellId;
    /* - Layer 2 NON CA Changes */

    rlcUpdateUmTxQciInfo(rlcEntityCfgInfo_p,rlcTxEntityCfgInfo_p,
           ueIndex);
    /* send req to DL RLC threads*/
    /** CA Changes Start **/
    /*** Coverity 63526 Fix : Start ****/
    if (!ENQUEUE_RLC_RECONFIG_ENTITY_Q(&rlcReconfigTxEntityQ_g, rlcTxEntityCfgInfo_p))
    {
	LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGFATAL,
               			RLC_RRC, globalTTITickCount_g, __LINE__,
             			0,ueIndex, 0, 0, 0, 0, __func__, "ENQUEUE_RLC_RECONFIG_ENTITY_Q Failed");
        freeMemPool(rlcTxEntityCfgInfo_p);
        return ;
    }
    else
    {
        (*nodeCount)++;
    }
    /*** Coverity 63526 Fix : End ****/
    /** CA Changes End **/
}


/****************************************************************************
 * Function Name  : rlcReconfigAmTxRxEntity
 * Inputs         : rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo,
 *                  ueIndex - Index of UE,
 *                  lcId - Logical channel ID,
 *                  nodeCount - Node count,
 *                  transactionId,
 *                  cellId - RRC Cell Index
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to Re-configure AM TX RX Entity in RLC.
 ****************************************************************************/
static inline void rlcReconfigAmTxRxEntity(RlcEntityCfgInfo* rlcEntityCfgInfo_p,
             UInt16 ueIndex,
             UInt16 lcId,
             UInt8 *nodeCount,
             /* Layer 2 NON CA Changes */
             UInt16 transactionId,
             RrcCellIndex cellId
             /* Layer 2 NON CA Changes */
)
{
    RlcTxEntityCfgInfo *rlcTxEntityCfgInfo_p = RLC_NULL;
    RlcRxEntityCfgInfo *rlcRxEntityCfgInfo_p = RLC_NULL;
    /** CA Changes Start **/
    InternalCellIndex cellIndex = RLC_GET_INTERNAL_CELL_INDEX(cellId);
    if( MAX_INTERNAL_CELL_INDEX < cellIndex )
    {
        LOG_RLC_MSG(LTE_HANDLE_RLC_DELETE_ENTITY, LOGWARNING,
            RLC_RRC, globalTTITickCount_g, __LINE__,
            ueIndex, transactionId, RLC_UE_NOT_EXISTS,
             0, 0, 0, __func__, "Invalid cell ID");
         return;
    }
    /** CA Changes End **/
	/*Coverity 25510,25511 Fix Start*/ 
    rlcTxEntityCfgInfo_p = getMemFromPool( sizeof(RlcTxEntityCfgInfo) ,RLC_NULL);
    if( RLC_NULL == rlcTxEntityCfgInfo_p)
    {
        ltePanic("%s => Memory Allocation Failure",__FUNCTION__);

    /* coverity_53489 Fix Start */
        return;
    /* coverity_53489 Fix End */
    }
    rlcRxEntityCfgInfo_p = getMemFromPool( sizeof(RlcRxEntityCfgInfo) ,RLC_NULL);
    if( RLC_NULL == rlcRxEntityCfgInfo_p)
    {
        ltePanic("%s => Memory Allocation Failure",__FUNCTION__);
        
    /* coverity_53488 Fix Start */
        freeMemPool(rlcTxEntityCfgInfo_p);
        return;
    /* coverity_53488 Fix End */
    }

	/*Coverity 25510,25511 Fix End*/ 
    rlcTxEntityCfgInfo_p->ueIndex = rlcRxEntityCfgInfo_p->ueIndex = ueIndex;
    rlcTxEntityCfgInfo_p->lcId = rlcRxEntityCfgInfo_p->lcId = lcId;
    /* + Layer 2 NON CA Changes */
    rlcTxEntityCfgInfo_p->transactionId = transactionId;
    rlcTxEntityCfgInfo_p->cellId = cellId;
    /*SPR 17489 Fix Start*/
    rlcRxEntityCfgInfo_p->transactionId = transactionId;
    /*SPR 17489 Fix End*/
    /* SPR_10206_Fix Start */
    rlcRxEntityCfgInfo_p->cellId = cellId;
    /* SPR_10206_Fix End */
    /* - Layer 2 NON CA Changes */
    rlcTxEntityCfgInfo_p->entityMode = rlcRxEntityCfgInfo_p->entityMode = RLC_ENTITY_AM;

    rlcRxEntityCfgInfo_p->rxRlc.rxAmRlc.tReordering = rlcEntityCfgInfo_p->configParam.am.rxAmRlc.tReordering;
    rlcRxEntityCfgInfo_p->rxRlc.rxAmRlc.tStatusProhibit = rlcEntityCfgInfo_p->configParam.am.rxAmRlc.tStatusProhibit;

    rlcTxEntityCfgInfo_p->txRlc.txAmRlc.tPollRetransmit = rlcEntityCfgInfo_p->configParam.am.txAmRlc.tPollRetransmit;
    rlcTxEntityCfgInfo_p->txRlc.txAmRlc.pollPDU = rlcEntityCfgInfo_p->configParam.am.txAmRlc.pollPDU;
    rlcTxEntityCfgInfo_p->txRlc.txAmRlc.pollByte = rlcEntityCfgInfo_p->configParam.am.txAmRlc.pollByte;
    rlcTxEntityCfgInfo_p->txRlc.txAmRlc.maxRetxThreshold = rlcEntityCfgInfo_p->configParam.am.txAmRlc.maxRetxThreshold;

    /* Cyclomatic Complexity changes - starts here */
    rlcUpdateAmQciInfo(rlcEntityCfgInfo_p,rlcTxEntityCfgInfo_p,
       rlcRxEntityCfgInfo_p,ueIndex);
    /* Cyclomatic Complexity changes - ends here */

    /* send req to DL/UL RLC threads*/
    /** CA Changes Start **/
    /*** Coverity 63524 Fix : Start ****/
    if (!ENQUEUE_RLC_RECONFIG_ENTITY_Q(&rlcReconfigTxEntityQ_g, rlcTxEntityCfgInfo_p))
    {
	LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGFATAL,
               			RLC_RRC, globalTTITickCount_g, __LINE__,
             			0,ueIndex, 0, 0, 0, 0, __func__, "ENQUEUE_RLC_RECONFIG_ENTITY_Q Failed");
        freeMemPool(rlcTxEntityCfgInfo_p);
        freeMemPool(rlcRxEntityCfgInfo_p);
        return;
    }
    else
    {
        (*nodeCount)++;
    }
    /** CA Changes End **/
    if (!ENQUEUE_RLC_RECONFIG_ENTITY_Q((&(rlcReconfigRxEntityQ_g)), rlcRxEntityCfgInfo_p))
    {
	LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGFATAL,
               			RLC_RRC, globalTTITickCount_g, __LINE__,
             			0,ueIndex, 0, 0, 0, 0, __func__, "ENQUEUE_RLC_RECONFIG_ENTITY_Q Failed");
        freeMemPool(rlcRxEntityCfgInfo_p);
        return;
    }
    else
    {
        (*nodeCount)++;
    }
    /*** Coverity 63524 Fix : End ****/
}
/* Cyclomatic Complexity changes - ends here */



/****************************************************************************
 * Function Name  : rlcReconfigTxRxEntity
 * Inputs         : rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo,
 *                  ueIndex - Index of UE,
 *                  lcId - Logical channel ID,
 *                  nodeCount - Node count,
 *                  transactionId,
 *                  cellId - RRC Cell Index
 * Outputs        : None
 * Returns        : RLC_SUCCESS/error code
 * Description    : This API is used to Re-configure TX RX Entity in RLC.
 ****************************************************************************/
inline UInt16 rlcReconfigTxRxEntity( UInt16 ueIndex,
                                     /* + Layer2 NON CA Changes */
                                     UInt16 transactionId,
                                     RrcCellIndex cellId,
                                     /* - Layer2 NON CA Changes */
                                     RlcEntityCfgInfo* rlcEntityCfgInfo_p,
                                     UInt8 *nodeCount)
{
    UInt16 ret   = RLC_SUCCESS ;
    RlcLcId lcId = rlcEntityCfgInfo_p->lcId;
    UEContext* outUEContext_p = DB_FETCH_UE_CONTEXT(ueIndex);
/* Cyclomatic Complexity changes - starts here */
    UInt8 nodeNum = *nodeCount;
/* Cyclomatic Complexity changes - ends here */

    if ( RLC_NULL == outUEContext_p )
    {
        LOG_RLC_MSG ( LTE_RLC_INVALID_UECONTEXT, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                ueIndex, lcId,RLC_UE_NOT_EXISTS , 0, 0,0, __func__, "");

        return RLC_UE_NOT_EXISTS;
    }

    if ( RLC_NULL == outUEContext_p->rlcEntityInfoList[lcId])
    {
        LOG_RLC_MSG ( LTE_RLC_INVALID_ENTITY_1, LOGWARNING, RLC_RRC,
                globalTTITickCount_g, __LINE__, ueIndex, lcId, RLC_LCID_NOT_EXISTS, 0,
                0,0, __func__, "");

        return RLC_LCID_NOT_EXISTS;        
    }
    
    if((rlcEntityCfgInfo_p->entityMode !=
         outUEContext_p->rlcEntityInfoList[lcId]->rlcEntityMode)||
        (rlcEntityCfgInfo_p->entityType !=
          outUEContext_p->rlcEntityInfoList[lcId]->entityType))
    {
        return RLC_ENTITY_NOT_EXIST;

    }

    if ( RLC_ENTITY_UM == rlcEntityCfgInfo_p->entityMode)
    {
        ret = validateEntityTypeAndCtx( rlcEntityCfgInfo_p, outUEContext_p );

        if ( RLC_SUCCESS == ret)
        {
            switch(rlcEntityCfgInfo_p->entityType)
            {
                /* Cyclomatic Complexity changes - starts here */
                case RLC_TX_RX_ENTITY:
                {
                    rlcReconfigUmTxRxEntity(rlcEntityCfgInfo_p,ueIndex,lcId,&nodeNum,transactionId,cellId);
                    *nodeCount = nodeNum;
                }
                break;

                case RLC_RX_ENTITY:
                {
                    rlcReconfigUmRxEntity(rlcEntityCfgInfo_p,ueIndex,lcId,&nodeNum, transactionId,cellId);
                    *nodeCount = nodeNum;
                }
                break;

                case RLC_TX_ENTITY:
                {
                    rlcReconfigUmTxEntity(rlcEntityCfgInfo_p,ueIndex,lcId,&nodeNum,transactionId,cellId);
                    *nodeCount = nodeNum;
                }
                break;

                default:
                LOG_RLC_MSG ( LTE_RLC_INVALID_ENTITY_ERR_1, LOGWARNING, 
                        RLC_RRC, globalTTITickCount_g, __LINE__,
                        ueIndex, lcId, INVALID_ENTITY_TYPE , 0, 0,0, __func__, "");

                return INVALID_ENTITY_TYPE;
            }

        }
        else
        {        
            LOG_RLC_MSG ( LTE_RLC_INVALID_ENTITY_ERR_2, LOGWARNING, 
                    RLC_RRC, globalTTITickCount_g, __LINE__,
                    ueIndex, lcId, ret, 0, 0,0, __func__, "");
            return ret;
        }
    }
    else if (RLC_ENTITY_AM == rlcEntityCfgInfo_p->entityMode)
    {
        if ( RLC_TX_RX_ENTITY == rlcEntityCfgInfo_p->entityType)
        {
            AMEntity* amEntity_p = 
                outUEContext_p->rlcEntityInfoList[lcId]->
                rlcEntity.amEntity_p; 


            if (( RLC_NULL == amEntity_p))
            {
                LOG_RLC_MSG ( LTE_RLC_INVALID_ENTITY_2, LOGWARNING, 
                        RLC_RRC, globalTTITickCount_g, __LINE__,
                        ueIndex, lcId, RLC_UEENTITY_NOT_EXIST, 0, 0, 0, __func__, "");

                return RLC_UEENTITY_NOT_EXIST;
            }
            if(amEntity_p->txEntityState == RLC_ENTITY_PENDING_DELETION || 
                    amEntity_p->rxEntityState == RLC_ENTITY_PENDING_DELETION)
            {
                LOG_RLC_MSG(LTE_RLC_HANDLE_RECONFIG_ERR_2, LOGWARNING,
                        RLC_RRC, globalTTITickCount_g, __LINE__,
                        ueIndex, 0, 0, 0, 0, 0, __func__, "RLC_ENTITY_IN_PENDING_DEL_Q");
                return RLC_ENTITY_IN_PENDING_DEL_Q;
            }
            
            rlcReconfigAmTxRxEntity(rlcEntityCfgInfo_p,ueIndex,lcId,&nodeNum,transactionId,cellId);
            *nodeCount = nodeNum;
        }
        /* Cyclomatic Complexity changes - ends here */
    }
    else 
    {
        LOG_RLC_MSG ( LTE_RLC_INVALID_ENTITY_ERR_3, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                ueIndex, lcId,RLC_SYNTAX_ERROR , 0, 0,0, __func__, "");
        return RLC_SYNTAX_ERROR;
    }

    return ret;
}
#endif

/****************************************************************************
 * Function Name  : rlcDeleteUEEntity 
 * Inputs         : ueIndex - Identifier for UE Context,
 *                  lcId - Identifier for logical channel,
 *                  transactionId - identifies transaction,
 *                  entityType - RLC Entity type,
 *                  entityMode - RLC Entity mode,
 *                  nodeCount
 * Outputs        : None
 * Returns        : RLC_SUCCESS if deleted successfully error msg otherwise.
 * Description    : This API is used to delete the UE entity and send the cnf 
 *                  msg only if entity is not in use otherwise it make an entry
 *                  to pending deletion queue.
 ****************************************************************************/
/* SPR 5599_129123 Fix Start */
UInt16 rlcDeleteUEEntity ( UInt16 ueIndex, 
/* SPR 5599_129123 Fix End */
                                  RlcLcId lcId, 
                                  UInt16 transactionId, 
                                  /* + Layer2 NON CA Changes */
                                  RrcCellIndex cellId,
                                  /* - Layer2 NON CA Changes */
                                  RlcEntityType entityType,
                                  RlcEntityMode entityMode
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                                  , UInt8 *nodeCount
#endif
                                )
{
    RlcDelReEstabQNode *rlcDelReEstabQNode_p = RLC_NULL;
    UEContext* outUEContext_p = DB_FETCH_UE_CONTEXT(ueIndex);
    if ( RLC_NULL == outUEContext_p )
    {
        LOG_RLC_MSG( LTE_RLC_INVALID_UECONTEXT_1, LOGWARNING, RLC_RRC, 
                globalTTITickCount_g, __LINE__, ueIndex, lcId, RLC_UE_NOT_EXISTS 
                , 0, 0, 0, __func__, "");

        return RLC_UE_NOT_EXISTS;
    }    
    RLCEntityInfo* rlcEntityInfo_p = RLC_NULL;
    if ( RLC_NULL == 
            (rlcEntityInfo_p = DB_FETCH_RLC_ENTITY_INFO(outUEContext_p,lcId)) )
    {
        LOG_RLC_MSG( LTE_RLC_INVALID_UECONTEXT_3, LOGWARNING, RLC_RRC, 
                globalTTITickCount_g, __LINE__, ueIndex, lcId, RLC_LCID_NOT_EXISTS 
                , 0, 0, 0, __func__, "");
        return RLC_LCID_NOT_EXISTS;
    }
    
    if((entityMode != rlcEntityInfo_p->rlcEntityMode)||
            (entityType != rlcEntityInfo_p->entityType))
    {

        return RLC_ENTITY_NOT_EXIST;

    }
    /** CA Changes Start **/
#ifndef UE_SIM_TESTING
    InternalCellIndex cellIndex = RLC_GET_INTERNAL_CELL_INDEX(cellId);
    if( MAX_INTERNAL_CELL_INDEX < cellIndex )
    {
        LOG_RLC_MSG(LTE_HANDLE_RLC_DELETE_ENTITY, LOGWARNING,
            RLC_RRC, globalTTITickCount_g, __LINE__,
            ueIndex, transactionId, RLC_UE_NOT_EXISTS,
             0, 0, 0, __func__, "Invalid cell ID");
        return RLC_FAILURE;
    }
#endif
    /** CA Changes End **/
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    /* setting the pending deletion of Rlc Entity */                                                 
    if ( RLC_ENTITY_PENDING_DELETION == rlcEntityInfo_p->rlcEntityState)
    {
        LOG_RLC_MSG ( LTE_RLC_ENTITY_MARKED_IN_PENDING_DEL_QUEUE, 
                LOGWARNING, RLC_RRC, globalTTITickCount_g, __LINE__,
                RLC_ENTITY_ALREADY_IN_PENDING_DEL_Q, 0, 0, 0, 0, 0, __func__, "");
        return RLC_ENTITY_ALREADY_IN_PENDING_DEL_Q;
    }    

    rlcEntityInfo_p->rlcEntityState = RLC_ENTITY_PENDING_DELETION;
#endif    
    /*As per SPR 698, ltePanic already takes care of returning value,
     *no change done*/
    CREATE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
    /* coverity 24570 fix start */
    if(RLC_NULL == rlcDelReEstabQNode_p)
    {
        return RLC_FAILURE;
    }
    /* coverity 24570 fix end */
    rlcDelReEstabQNode_p->ueIndex = ueIndex ;
    rlcDelReEstabQNode_p->lcId    = lcId;
    rlcDelReEstabQNode_p->transactionId = transactionId;
    rlcDelReEstabQNode_p->cellId = cellId;
    rlcDelReEstabQNode_p->rlcNodeType   = RLC_UE_ENTITY;
    /* SPR 21778 Fix */
    rlcDelReEstabQNode_p->reEstablishProcessTick   = 0;
    /* SPR 21778 Fix */

    switch ( entityType )
    {
        case RLC_TX_ENTITY : 
            {
                rlcDelReEstabQNode_p->entityType = RLC_TX_ENTITY;
                rlcDelReEstabQNode_p->cnf        = RLC_TX_ENTITY;
                 /** CA Changes Start **/
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                /*** Coverity 25582 Fix : Start ****/
                /** SPR 11010 Changes Start **/
                if (ENQUEUE_RLC_DEL_RE_ESTABQ(&rlcTxPendingDelQ_g[cellIndex], rlcDelReEstabQNode_p))
                /** SPR 11010 Changes End **/
                {
                    (*nodeCount)++;
                }
                else
#else
                /*SPR 13840 Fix, corrected if condition*/    
                if (ENQUEUE_RLC_DEL_RE_ESTABQ( &rlcTxPendingDelQ_g, rlcDelReEstabQNode_p))
#endif                
                {
	 	    LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGFATAL,
               			RLC_RRC, globalTTITickCount_g, __LINE__,
             			0,ueIndex, 0, 0, 0, 0, __func__, "ENQUEUE_RLC_DEL_RE_ESTABQ Failed");
             	    FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
                    return RLC_FAILURE;
                }
                /*** Coverity 25582 Fix : End ****/
                /** CA Changes End **/
#ifdef RLC_STANDALONE_SETUP    
                /*To make Tx thread active*/
                semPost(&rlcTxSemlock_g);
#endif
            }
            break;
        case RLC_TX_RX_ENTITY:
            {
                /* SPR 4392 Fix Start */
                rlcDelReEstabQNode_p->entityType = RLC_TX_RX_ENTITY;
                /* SPR 4392 Fix End */
                rlcDelReEstabQNode_p->cnf        = RLC_TX_ENTITY;
                 /** CA Changes Start **/
                /*** Coverity 25582 Fix : Start ****/
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                /** SPR 11010 Changes Start **/
                if (ENQUEUE_RLC_DEL_RE_ESTABQ(&rlcTxPendingDelQ_g[cellIndex], rlcDelReEstabQNode_p))
                /** SPR 11010 Changes End **/
                {
                    (*nodeCount)++;
                }
                else
#else
                /*SPR 13840 Fix, corrected if condition*/    
                if (ENQUEUE_RLC_DEL_RE_ESTABQ( &rlcTxPendingDelQ_g, rlcDelReEstabQNode_p))
#endif                
                {
		    LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGFATAL,
               			RLC_RRC, globalTTITickCount_g, __LINE__,
             			0, ueIndex, 0, 0, 0, 0, __func__, "ENQUEUE_RLC_DEL_RE_ESTABQ Failed");
             	    FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p); 

                    return RLC_FAILURE;
                }
                /*** Coverity 25582 Fix : End ****/
                 /** CA Changes End **/

#ifdef RLC_STANDALONE_SETUP    
                /*To make Tx thread active*/
                semPost(&rlcTxSemlock_g);
#endif

                rlcDelReEstabQNode_p = RLC_NULL;
                CREATE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
                /* coverity_24570 Fix Start */
                if(PNULL == rlcDelReEstabQNode_p)
                {
                    return RLC_FAILURE;
                }
                /* coverity_24570 Fix End */
                rlcDelReEstabQNode_p->ueIndex = ueIndex ;
                rlcDelReEstabQNode_p->lcId = lcId;
                rlcDelReEstabQNode_p->transactionId = transactionId;
                rlcDelReEstabQNode_p->rlcNodeType = RLC_UE_ENTITY;
                /* SPR_10206 Fix Start */ 
                rlcDelReEstabQNode_p->cellId = cellId;
                /* SPR_10206 Fix End */ 
                /* SPR 4392 Fix Start */
                rlcDelReEstabQNode_p->entityType = RLC_TX_RX_ENTITY;
                /* SPR 4392 Fix End */
                rlcDelReEstabQNode_p->cnf = RLC_RX_ENTITY;
                /*** Coverity 25582 Fix : Start ****/
                /** SPR 11010 Changes Start **/
		/* SPR 21778 Fix */
                rlcDelReEstabQNode_p->reEstablishProcessTick   = 0;
		/* SPR 21778 Fix */
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                if (ENQUEUE_RLC_DEL_RE_ESTABQ( &rlcRxPendingDelQ_g, rlcDelReEstabQNode_p))
                {
                    (*nodeCount)++;
                }
                else
#else
                /*SPR 13840 Fix, corrected if condition*/    
                if (ENQUEUE_RLC_DEL_RE_ESTABQ( &rlcRxPendingDelQ_g, rlcDelReEstabQNode_p))
#endif
                {
		    LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGFATAL,
               			RLC_RRC, globalTTITickCount_g, __LINE__,
             			0,ueIndex, 0, 0, 0, 0, __func__, "ENQUEUE_RLC_DEL_RE_ESTABQ Failed");
             	    FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
                    return RLC_FAILURE;
                }
                /** SPR 11010 Changes End **/
                /*** Coverity 25582 Fix : End ****/

#ifdef RLC_STANDALONE_SETUP        
                /*To make Rx thread active*/
                semPost(&rlcRxSemlock_g);
#endif        
            }
            break;
        case RLC_RX_ENTITY:
            {
                rlcDelReEstabQNode_p->entityType = RLC_RX_ENTITY;
                rlcDelReEstabQNode_p->cnf        = RLC_RX_ENTITY;
                /*** Coverity 25582 Fix : Start ****/
                /** SPR 11010 Changes Start **/
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                if (ENQUEUE_RLC_DEL_RE_ESTABQ( &rlcRxPendingDelQ_g, rlcDelReEstabQNode_p))
                {
                    (*nodeCount)++;
                }
                else
#else
                /*SPR 13840 Fix, corrected if condition*/    
                if (ENQUEUE_RLC_DEL_RE_ESTABQ( &rlcRxPendingDelQ_g, rlcDelReEstabQNode_p))
#endif
                {
		    LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGFATAL,
               			RLC_RRC, globalTTITickCount_g, __LINE__,
             			0,ueIndex, 0, 0, 0, 0, __func__, "ENQUEUE_RLC_DEL_RE_ESTABQ Failed");
             	    FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
                    return RLC_FAILURE;
                }
                /*** Coverity 25582 Fix : End ****/
                /** SPR 11010 Changes End **/

#ifdef RLC_STANDALONE_SETUP        
                /*To make Rx thread active*/
                semPost(&rlcRxSemlock_g);
#endif        
            }
            break;

        default:
            { 
                LOG_RLC_MSG( LTE_RLC_INVALID_ENTITY_TYPE, LOGWARNING, RLC_RRC, 
                        globalTTITickCount_g, __LINE__, ueIndex, lcId, INVALID_ENTITY_TYPE 
                        , 0, 0, 0, __func__, "");
		/* Coverity 25509 Fix Start */
		FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
		/* Coverity 25509 Fix End */
                return INVALID_ENTITY_TYPE;//need to discuss with shreya mam
            }
    }
    gRlcStats.pRlcStats->lteRlcUeStat[ueIndex].numActiveLC --;
    memSet(&(gRlcStats.pRlcStats->lteRlcUeStat[ueIndex].lcStats[lcId]), 
            0, sizeof(LteRlcLCStats));
    /* SPR 2712 Fix Start */
    LOG_RLC_MSG( LTE_RLC_ENTITY_IN_PENDING_DEL_QUEUE_1, LOGINFO, RLC_RRC, 
            globalTTITickCount_g, __LINE__, ueIndex, lcId, RLC_SUCCESS, 
            0, 0, 0, __func__, "");
    return RLC_SUCCESS;
   // return RLC_ENTITY_IN_PENDING_DEL_Q;
    /* SPR 2712 Fix End */
}


/****************************************************************************
 * Function Name  : rlcReEstabUeEntity 
 * Inputs         : ueIndex - Identifies the UE Context,
 *                  lcId - logical channel identifier,
 *                  rlcState - RLC State,
 *                  transactionId - transaction id,
 *                  nodeCount - Node Count,
 *                  cellId - RRC Cell Index
 * Outputs        : None
 * Returns        : RLC_SUCCESS if successfull error message otherwise
 * Description    : This API is used to make an entry into re-establish queue 
 *                  for an UE Entity. 
 ****************************************************************************/

/* SPR 5599_129123 Fix Start */
UInt16 rlcReEstabUeEntity ( UInt16 ueIndex, 
/* SPR 5599_129123 Fix End */
                                   RlcLcId lcId,
                                   /* SPR 4994 Change Start */
                                   RlcState   rlcState, 
                                   /* SPR 4994 Change End */ 
                                   UInt16 transactionId
    /* Layer 2 NON CA Changes */
                                   , RrcCellIndex cellId
    /* Layer 2 NON CA Changes */
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                                   , UInt8 *nodeCount
#endif                                   
                                 ) 
{
    UInt16 ret = RLC_SUCCESS;
    RlcDelReEstabQNode *rlcDelReEstabQNode_p = RLC_NULL;
    /* SPR 4994 Chnages Start */
    /* Rel 5.3: Coverity 32382 and 32381 Fix Start */
    RlcEntityState rlcEntityTxState = RLC_ENTITY_INACTIVE;
    /* Rel 5.3: Coverity 32382 and 32381 Fix Start */
    /* Rel 5.3: Coverity 32380 Fix Start */
    RlcEntityState rlcEntityRxState = RLC_ENTITY_INACTIVE;
    /* Rel 5.3: Coverity 32380 Fix End */
    /* SPR 4994 Chnages End */
    /* mark the entity inactive */
    UEContext* ueContext_p = DB_FETCH_UE_CONTEXT(ueIndex);
    if ( RLC_NULL == ueContext_p )
    {
        LOG_RLC_MSG(LTE_RLC_INVALID_UECONTEXT_2, LOGWARNING, RLC_RRC, globalTTITickCount_g, __LINE__,
                ueIndex, lcId, RLC_UE_NOT_EXISTS, 0, 0, 0, __func__, "");
        return RLC_UE_NOT_EXISTS;
    }
    RLCEntityInfo* rlcEntityInfo_p = RLC_NULL; 
    if ( RLC_NULL == 
            (rlcEntityInfo_p = DB_FETCH_RLC_ENTITY_INFO(ueContext_p,lcId)) )
    {
        return RLC_LCID_NOT_EXISTS;
    }
    /** CA Changes Start **/
#ifndef UE_SIM_TESTING
    InternalCellIndex cellIndex = RLC_GET_INTERNAL_CELL_INDEX(cellId);
    if( MAX_INTERNAL_CELL_INDEX < cellIndex )
    {
        LOG_RLC_MSG(LTE_HANDLE_RLC_DELETE_ENTITY, LOGWARNING,
            RLC_RRC, globalTTITickCount_g, __LINE__,
            ueIndex, transactionId, RLC_UE_NOT_EXISTS,
             0, 0, 0, __func__, "Invalid cell ID");
         return 0;
    }
#endif
    /** CA Changes End **/
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    if ( RLC_ENTITY_PENDING_DELETION == rlcEntityInfo_p->rlcEntityState )
    {
        return RLC_ENTITY_IN_PENDING_DEL_Q;
    }
    /* SPR 4994 Chnages Start */
    rlcEntityTxState = rlcEntityInfo_p->rlcEntityState;
    rlcEntityRxState = rlcEntityInfo_p->rlcEntityState;
    /* SPR 4994 Chnages End */
    rlcEntityInfo_p->rlcEntityState = RLC_ENTITY_INACTIVE;
    
#endif    
    /* enqueue the entity into queue to be deleted */
    /*As per SPR 698, ltePanic already takes care of returning value, 
     *no change done*/
    CREATE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
    /* Coverity 24571 Fix Start */
    if(RLC_NULL == rlcDelReEstabQNode_p)
    {
        return RLC_FAILURE;
    }
    /* Coverity 24571 Fix End */
    rlcDelReEstabQNode_p->rlcNodeType = RLC_UE_ENTITY;
    rlcDelReEstabQNode_p->ueIndex = ueIndex;
    rlcDelReEstabQNode_p->lcId = lcId;
    rlcDelReEstabQNode_p->transactionId = transactionId;
    /* Layer 2 NON CA Changes */
    rlcDelReEstabQNode_p->cellId = cellId;
    /* Layer 2 NON CA Changes */

    if ( rlcEntityInfo_p->entityType == RLC_TX_ENTITY ) 
    {
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
        if(rlcEntityInfo_p->rlcEntityMode == RLC_ENTITY_UM)
        {
            /* SPR 4994 Chnages Start */
            rlcEntityTxState = rlcEntityInfo_p->rlcEntity.umEntity_p->txEntityState;
            /* SPR 4994 Chnages End */
            rlcEntityInfo_p->rlcEntity.umEntity_p->txEntityState = RLC_ENTITY_INACTIVE;
        }
        else if(rlcEntityInfo_p->rlcEntityMode == RLC_ENTITY_AM)
        {
            /* SPR 4994 Chnages Start */
            rlcEntityTxState = rlcEntityInfo_p->rlcEntity.amEntity_p->txEntityState;
            /* SPR 4994 Chnages End */
            rlcEntityInfo_p->rlcEntity.amEntity_p->txEntityState = RLC_ENTITY_INACTIVE;
        }
#endif                                   
        /*RLC Rel 2.0 Changes Start*/
        rlcDelReEstabQNode_p->entityType = RLC_TX_ENTITY;
        rlcDelReEstabQNode_p->cnf = RLC_TX_ENTITY;
        /* SPR 4994 Changes start */
        if (RLC_FREEZE == rlcState)
        rlcDelReEstabQNode_p->rlcState = RLC_ENTITY_INACTIVE;
        else
        rlcDelReEstabQNode_p->rlcState = rlcEntityTxState; 
        /* SPR 4994 Changes End */
        /*RLC Rel 2.0 Changes End*/
       /** CA Changes Start **/
       /* SPR 21778 Fix */
       rlcDelReEstabQNode_p->reEstablishProcessTick   = 0;
       /* SPR 21778 Fix */
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
        /*** Coverity 25583 Fix : Start ****/
        /** SPR 11010 Changes Start **/
        if (ENQUEUE_RLC_DEL_RE_ESTABQ(&rlcTxReEstabQ_g[cellIndex], rlcDelReEstabQNode_p))
        /** SPR 11010 Changes End **/
        {
            (*nodeCount)++;
        }
        else
#else
        /*SPR 13840 Fix, corrected if condition*/    
        if (ENQUEUE_RLC_DEL_RE_ESTABQ( &rlcTxReEstabQ_g, rlcDelReEstabQNode_p)) 
#endif                                   
        {
	    LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGFATAL,
               			RLC_RRC, globalTTITickCount_g, __LINE__,
             			0,ueIndex, 0, 0, 0, 0, __func__, "ENQUEUE_RLC_DEL_RE_ESTABQ Failed");
            FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
            return RLC_FAILURE;
        }
        /*** Coverity 25583 Fix : End ****/
        /** CA Changes End **/
#ifdef RLC_STANDALONE_SETUP    
        /*To made Tx thread active*/
        semPost(&rlcTxSemlock_g);
#endif
    }
    else if(rlcEntityInfo_p->entityType == RLC_TX_RX_ENTITY ) 
    {
        /*RLC Rel 2.0 Changes Start*/
            /*RLC Rel 3.0 Changes Start*/
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
        if(rlcEntityInfo_p->rlcEntityMode == RLC_ENTITY_UM)
        {
            /* SPR 4994 Chnages Start */
            rlcEntityTxState = rlcEntityInfo_p->rlcEntity.umEntity_p->txEntityState;
            rlcEntityRxState = rlcEntityInfo_p->rlcEntity.umEntity_p->rxEntityState;
            /* SPR 4994 Chnages End */
            rlcEntityInfo_p->rlcEntity.umEntity_p->txEntityState =
                                                   RLC_ENTITY_INACTIVE;
            rlcEntityInfo_p->rlcEntity.umEntity_p->rxEntityState =
                                                   RLC_ENTITY_INACTIVE;
        }
        else if(rlcEntityInfo_p->rlcEntityMode == RLC_ENTITY_AM)
        {
            /* SPR 4994 Chnages Start */
            rlcEntityTxState = rlcEntityInfo_p->rlcEntity.amEntity_p->txEntityState;
            rlcEntityRxState = rlcEntityInfo_p->rlcEntity.amEntity_p->rxEntityState;
            /* SPR 4994 Chnages End */
            rlcEntityInfo_p->rlcEntity.amEntity_p->txEntityState =
                                                   RLC_ENTITY_INACTIVE;
            rlcEntityInfo_p->rlcEntity.amEntity_p->rxEntityState =
                                                   RLC_ENTITY_INACTIVE;
        }
        rlcDelReEstabQNode_p->entityType = RLC_TX_ENTITY;
#else
        rlcDelReEstabQNode_p->entityType = RLC_TX_RX_ENTITY;
#endif
        rlcDelReEstabQNode_p->cnf = RLC_TX_RX_ENTITY;
            /*RLC Rel 3.0 Changes End*/
        /*RLC Rel 2.0 Changes End*/
        /* SPR 4994 Changes start */
        if (RLC_FREEZE == rlcState)
        rlcDelReEstabQNode_p->rlcState = RLC_ENTITY_INACTIVE;
        else
        rlcDelReEstabQNode_p->rlcState = rlcEntityTxState;
        /* SPR 4994 Changes End */
        /** CA Changes Start **/
       /* SPR 21778 Fix */
       rlcDelReEstabQNode_p->reEstablishProcessTick   = 0;
       /* SPR 21778 Fix */
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
        /*** Coverity 25583 Fix : Start ****/
        /** SPR 11010 Changes Start **/
        if (ENQUEUE_RLC_DEL_RE_ESTABQ(&rlcTxReEstabQ_g[cellIndex], rlcDelReEstabQNode_p))
        /** SPR 11010 Changes End **/
        {
            (*nodeCount)++;
        }
        else
#else
        /*SPR 13840 Fix, corrected if condition*/    
        if (ENQUEUE_RLC_DEL_RE_ESTABQ( &rlcTxReEstabQ_g, rlcDelReEstabQNode_p))
#endif
        {
	    LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGFATAL,
               			RLC_RRC, globalTTITickCount_g, __LINE__,
             			0,ueIndex, 0, 0, 0, 0, __func__, "ENQUEUE_RLC_DEL_RE_ESTABQ Failed");
        
            FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
            return RLC_FAILURE;
        }
        /*** Coverity 25583 Fix : End ****/
        /** CA Changes End **/
#ifdef RLC_STANDALONE_SETUP    
        /*To made Tx thread active*/
        semPost(&rlcTxSemlock_g);
#endif
        /* Review comment fix start REEST_3 */
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
        rlcDelReEstabQNode_p = RLC_NULL;
        CREATE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
        /* coverity_24571 Fix Start */
        if(PNULL == rlcDelReEstabQNode_p)
        {
            return RLC_FAILURE;
        }
        /* coverity_24571 Fix End*/
        rlcDelReEstabQNode_p->rlcNodeType = RLC_UE_ENTITY;
        rlcDelReEstabQNode_p->ueIndex = ueIndex;
        rlcDelReEstabQNode_p->lcId = lcId;
        rlcDelReEstabQNode_p->transactionId = transactionId;
        /* Layer 2 NON CA Changes */
        rlcDelReEstabQNode_p->cellId = cellId;
        /* Layer 2 NON CA Changes */
        /*RLC Rel 2.0 Changes Start*/
        rlcDelReEstabQNode_p->entityType = RLC_RX_ENTITY;
        rlcDelReEstabQNode_p->cnf = RLC_TX_RX_ENTITY;
        /*RLC Rel 2.0 Changes End*/
        /* SPR 4994 Changes start */
        if (RLC_FREEZE == rlcState)
        rlcDelReEstabQNode_p->rlcState = RLC_ENTITY_INACTIVE;
        else
        rlcDelReEstabQNode_p->rlcState = rlcEntityRxState;
        /* SPR 4994 Changes End */
	/* SPR 21778 Fix */
	rlcDelReEstabQNode_p->reEstablishProcessTick   = 0;
	/* SPR 21778 Fix */
        /*** Coverity 25583 Fix : Start ****/
        /** SPR 11010 Changes Start **/
        if (ENQUEUE_RLC_DEL_RE_ESTABQ(&rlcRxReEstabQ_g, rlcDelReEstabQNode_p))
        {
            (*nodeCount)++;
        }
        else
        {
	    LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGFATAL,
               			RLC_RRC, globalTTITickCount_g, __LINE__,
             			0,ueIndex, 0, 0, 0, 0, __func__, "ENQUEUE_RLC_DEL_RE_ESTABQ Failed");
            FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p); 
            return RLC_FAILURE;
        }
        /** SPR 11010 Changes End **/
        /*** Coverity 25583 Fix : End ****/
#ifdef RLC_STANDALONE_SETUP    
    	/*To made Rx thread active*/
        semPost(&rlcRxSemlock_g);
#endif        
#endif        
        /* Review comment fix end REEST_3 */
    }
    else 
    {
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
        if(rlcEntityInfo_p->rlcEntityMode == RLC_ENTITY_UM)
        {
            /* SPR 4994 Changes start */
            rlcEntityRxState = rlcEntityInfo_p->rlcEntity.umEntity_p->rxEntityState;
            /* SPR 4994 Changes End */
            rlcEntityInfo_p->rlcEntity.umEntity_p->rxEntityState = RLC_ENTITY_INACTIVE;
        }
        else if(rlcEntityInfo_p->rlcEntityMode == RLC_ENTITY_AM)
        {
            /* SPR 4994 Changes start */
            rlcEntityRxState = rlcEntityInfo_p->rlcEntity.amEntity_p->rxEntityState;
            /* SPR 4994 Changes End */
            rlcEntityInfo_p->rlcEntity.amEntity_p->rxEntityState = RLC_ENTITY_INACTIVE;
        }
#endif                                   
        /*RLC Rel 2.0 Changes Start*/
        rlcDelReEstabQNode_p->entityType = RLC_RX_ENTITY;
        rlcDelReEstabQNode_p->cnf = RLC_RX_ENTITY;
        /*RLC Rel 2.0 Changes End*/
        /* SPR 4994 Changes start */
        if (RLC_FREEZE == rlcState)
        rlcDelReEstabQNode_p->rlcState = RLC_ENTITY_INACTIVE;
        else
        rlcDelReEstabQNode_p->rlcState = rlcEntityRxState;
        /* SPR 4994 Changes End */
        /*** Coverity 25583 Fix : Start ****/
	/* SPR 21778 Fix */
	rlcDelReEstabQNode_p->reEstablishProcessTick   = 0;
	/* SPR 21778 Fix */
        /** SPR 11010 Changes Start **/
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
        if (ENQUEUE_RLC_DEL_RE_ESTABQ(&rlcRxReEstabQ_g, rlcDelReEstabQNode_p))
        {
            (*nodeCount)++;
        }
#else
        /*SPR 13840 Fix, corrected if condition*/    
        if (ENQUEUE_RLC_DEL_RE_ESTABQ(&rlcRxReEstabQ_g, rlcDelReEstabQNode_p))
#endif
        {
	    LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGFATAL,
               			RLC_RRC, globalTTITickCount_g, __LINE__,
             			0,ueIndex, 0, 0, 0, 0, __func__, "ENQUEUE_RLC_DEL_RE_ESTABQ Failed");
            FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
            return RLC_FAILURE;
        }
        /** SPR 11010 Changes End **/
        /*** Coverity 25583 Fix : End ****/
#ifdef RLC_STANDALONE_SETUP    
    	/*To made Rx thread active*/
        semPost(&rlcRxSemlock_g);
#endif        
    }    
    memSet(&(gRlcStats.pRlcStats->lteRlcUeStat[ueIndex].lcStats[lcId]), 
           0, sizeof(LteRlcLCStats));
    return ret;
}

/****************************************************************************
 * Function Name  : reEstablishUeEntity
 * Inputs         : ueIndex - UE index,
 *                  lcId - Logical channel ID,
 *                  entityType - Entity type
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to configure the rxAmEntity using requested
 *                  parameter.
 ****************************************************************************/
/* SPR 5599_129123 Fix Start */
void reEstablishUeEntity ( UInt16 ueIndex,
/* SPR 5599_129123 Fix End */
                           RlcLcId lcId,
                           RlcEntityType entityType
                         )
{
    /* fetching rlc Entity */
    UEContext* outUEContext_p = DB_FETCH_UE_CONTEXT(ueIndex);
    if ( RLC_NULL == outUEContext_p )
    {
        LOG_RLC_MSG(LTE_RLC_REST_UE_ENTITY_ERR, LOGWARNING, RLC_RRC, globalTTITickCount_g,
                __LINE__, ueIndex, lcId, 0,
                0, 0, 0, __func__, "");
        return ;
    }
    RLCEntityInfo *rlcEntityInfo_p = outUEContext_p->rlcEntityInfoList[lcId];
    if ( RLC_NULL == rlcEntityInfo_p)
    {
        LOG_RLC_MSG(LTE_RLC_REST_UE_ENTITY_ERR_1, LOGWARNING, RLC_RRC, globalTTITickCount_g,
                __LINE__, ueIndex, lcId, 0,
                0, 0, 0, __func__, "");
        return ;
    }

    switch(rlcEntityInfo_p->rlcEntityMode)
    {
        case RLC_ENTITY_UM:
            reEstablishUMEntity ( entityType, rlcEntityInfo_p);
            break;    

        case RLC_ENTITY_AM: 
            reEstablishAMEntity (rlcEntityInfo_p);
            break;

        default:
            LOG_RLC_MSG(LTE_RLC_REST_UE_ENTITY_ERR_2, LOGWARNING, RLC_RRC, globalTTITickCount_g,
                    __LINE__, ueIndex, lcId, 0,
                    0, 0, 0, __func__, "");

            break; 

    }
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    rlcEntityInfo_p -> rlcEntityState = RLC_ENTITY_ACTIVE;
#endif    

}
/****************************************************************************
 * Function Name  : reEstablishTxUMEntity 
 * Inputs         : rlcEntityInfo_p - pointer to the RLC entity info 
 * Outputs        : None 
 * Returns        : void
 * Description    : This API is used to re-establish UE entity. This API shall 
 *                  be called by the handler of reEstablish Queue.
 ****************************************************************************/
inline static void reEstablishTxUMEntity(  RLCEntityInfo *rlcEntityInfo_p)
{
    /* SPR 21778 Fix code deleted */
    /* checking the type of entity either tx or rx */
    TxUMEntity *txUMEntity_p =
        rlcEntityInfo_p->rlcEntity.umEntity_p->txUMEntity_p;
    
    /*UM Transmitting Entity Re-establishment*/
    /* SPR 4555 Fix Start */
    /* if((RLC_TX_ENTITY == rlcEntityInfo_p->entityType) && */
    if (RLC_NULL != txUMEntity_p)
    /* SPR 4555 Fix End */
    {
        /* empty the Sdu Q */
/* SPR 4497 Changes Start */
        RlcStatusBuffer *rlcStatusBuffer = PNULL;
        UInt32 rlcStatusBufferCount = 0;

        UInt16 nodeCount = QUEUE_COUNT_AMD_RAW_SDU_Q(txUMEntity_p->txSDUQueue);
        if(txUMEntity_p->txSduIn.pRawSdu.data_p)
        {
            nodeCount+=1;
        }
        /*+COVERITY 5.3.0 - 32096*/
	    /* SPR 8760 fix start */
        if( 0 < nodeCount)
        /*-COVERITY 5.3.0 - 32096*/
        {
            /* SPR 8760 fix end */
            rlcStatusBuffer = getMemFromPool((sizeof(RlcStatusBuffer) * nodeCount) + 1 ,RLC_NULL);
            if( RLC_NULL == rlcStatusBuffer)
            {
                ltePanic("%s => Memory Allocation Failure",__FUNCTION__);
                return;
            }

            if(txUMEntity_p->txSduIn.pRawSdu.data_p)
            {
                rlcStatusBuffer[rlcStatusBufferCount].transId = txUMEntity_p->txSduIn.transId;
                rlcStatusBuffer[rlcStatusBufferCount].deliveryStatus = RLC_FAILURE;
                rlcStatusBufferCount++;
            }
            EMPTY_TX_UMD_RAW_SDU_Q_SET_STATUS_BUFFER(txUMEntity_p->txSDUQueue ,
                    rlcStatusBuffer, rlcStatusBufferCount);

            LteRlcLCStats* lcStats_p =
                &(gRlcStats.pRlcStats->lteRlcUeStat[txUMEntity_p->ueIndex].
                        lcStats[txUMEntity_p->lcId]);

            if(rlcStatusBufferCount)
            {
                CLOCK_START_RLC_API(RLC_DLVRY_STATUS_IND);
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
                if((SN_TERMINATED_SCG_SPLIT_BEARER ==  txUMEntity_p->dcBearerType) && (txUMEntity_p->lcId >= MIN_DRB_LC_ID))
                    sendRlcDeliveryStatusToX2U(txUMEntity_p->ueIndex,txUMEntity_p->lcId,
                    rlcStatusBufferCount,rlcStatusBuffer);
                else
#endif
                rlcDeliveryStatusInd(txUMEntity_p->ueIndex, txUMEntity_p->lcId, rlcStatusBufferCount,
                        rlcStatusBuffer);
                CLOCK_END_RLC_PDCP_API(RLC_DLVRY_STATUS_IND);
                lcStats_p->numDlvryInd += rlcStatusBufferCount;
            }
            else
            {
                freeMemPool(rlcStatusBuffer);
            }
        }
#ifdef RLC_QUEUE_STATS
        UInt32 qCount = QUEUE_COUNT_UMD_RAW_SDU_Q(txUMEntity_p->txSDUQueue);
        UInt32 LcId = txUMEntity_p->lcId;
        UInt32 ueId = txUMEntity_p->ueIndex;
        lteMacUpdateStatsQueuePerUePerLc(UMD_RAW_SDU_Q,ueId,LcId,qCount);
#endif
        msgFree(txUMEntity_p->txSduIn.pRawSdu.data_p);
        txUMEntity_p->txSduIn.pRawSdu.data_p = RLC_NULL;
        txUMEntity_p->txSduIn.pRawSdu.szUsed = 0;
        /* Start 128 UE: 25% NACK Changes */
        txUMEntity_p->txSduIn.pRawSdu.size = 0;//v_g
        /* End 128 UE: 25% NACK Changes */
        /* Reset state variables at TX entity */
        txUMEntity_p->txSduIn.sn = 0;
        txUMEntity_p->txQueueSize = 0;
        txUMEntity_p->US = 0;
        /** Rel 4.0 Changes Start **/
#ifndef UE_SIM_TESTING
        /*SPR 4596 Fix +*/
        //txUMEntity_p->rlcQueueSize = RLC_QUEUE_SIZE;
        /*SPR 4596 Fix -*/
        txUMEntity_p->qci = 0;
        txUMEntity_p->bitRateTx = 0;
#endif
        /** Rel 4.0 Changes End **/

	/* SPR 21778 Fix code deleted */
        LOG_RLC_MSG(LTE_RLC_REESTAB_UM_TX_ENTITY, LOGINFO, RLC_RRC, globalTTITickCount_g, __LINE__,
                txUMEntity_p->ueIndex, txUMEntity_p->lcId, txUMEntity_p->txQueueSize ,
                txUMEntity_p->US, 0,0, __func__, "");
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
        rlcEntityInfo_p->rlcEntity.umEntity_p->txEntityState = RLC_ENTITY_ACTIVE;
#endif        
    }
}

/****************************************************************************
 * Function Name  : reEstablishRxUMEntity
 * Inputs         : rlcEntityInfo_p - Pointer to RLCEntityInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS
 * Description    : This API is used to re-establish UE entity. This API shall
 *                  be called by the handler of reEstablish Queue.
 ****************************************************************************/

inline static UInt32 reEstablishRxUMEntity(  RLCEntityInfo *rlcEntityInfo_p)
{
    RxUMEntity *rxUMEntity_p =
        rlcEntityInfo_p->rlcEntity.umEntity_p->rxUMEntity_p;
    /* empty the Sdu Q */
    rlcReEstabReassRxUMEntity(rxUMEntity_p);
    EMPTY_UMD_RAW_SDU_Q(rxUMEntity_p->rxReSduQ);
    msgFree(rxUMEntity_p->rxSduIn.pRawSdu.data_p);
    rxUMEntity_p->rxSduIn.pRawSdu.data_p = RLC_NULL;
    rxUMEntity_p->rxSduIn.pRawSdu.szUsed = 0;
    rxUMEntity_p->rxSduIn.pRawSdu.size = 0;
    rxUMEntity_p->rxSduIn.sn = 0;
    rxUMEntity_p->UR = 0 ;
    rxUMEntity_p->UX = RLC_UNULL ;
    rxUMEntity_p->UH = 0 ;
    
    /** Rel 4.0 Changes Start **/
#ifndef UE_SIM_TESTING
    rxUMEntity_p->qci = 0;
    rxUMEntity_p->bitRateRx = 0;
#endif
    /** Rel 4.0 Changes End **/

    /* stop UM Reordering timer */
    stopUMROTimer(rxUMEntity_p);
    /* SPR 4663 Fix Start */
    rxUMEntity_p->isTimerRunning = RLC_FALSE;
    /* SPR 4663 Fix End */
    LOG_RLC_MSG(LTE_RLC_REESTAB_UM_RX_ENTITY, LOGINFO, RLC_RRC, globalTTITickCount_g,__LINE__,
            rxUMEntity_p->ueIndex, rxUMEntity_p->lcId, rxUMEntity_p->rxSduIn.pRawSdu.szUsed ,
            rxUMEntity_p->UR, rxUMEntity_p->UH, 0, __func__, "");
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
        rlcEntityInfo_p->rlcEntity.umEntity_p->rxEntityState = RLC_ENTITY_ACTIVE;
#endif        
   return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : reEstablishTxRxUMEntity 
 * Inputs         : rlcEntityInfo_p - Pointer to RLCEntityInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to re-establish UE entity. This API shall
 *                  be called by the handler of reEstablish Queue.
 ****************************************************************************/

inline static UInt32 reEstablishTxRxUMEntity(  RLCEntityInfo *rlcEntityInfo_p)
{
    /* Review comment fix start REEST_4 */
    UInt32 retVal = RLC_SUCCESS;
    reEstablishTxUMEntity(rlcEntityInfo_p);
    /*Klockworks Warning Fix Start*/
    if (RLC_SUCCESS == reEstablishRxUMEntity(rlcEntityInfo_p))
    {
        /* This function will always return RLC_SUCCESS*/
        retVal = RLC_SUCCESS;        
    }
    /*Klockworks Warning Fix End*/
    return retVal;        
    /* Review comment fix end REEST_4 */
}

/****************************************************************************
 * Function Name  : reEstablishUMEntity
 * Inputs         : entityType - Entity Type,
 *                  rlcEntityInfo_p - Pointer to RLCEntityInfo
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to re-establish UE entity. This API shall
 *                  be called by the handler of reEstablish Queue.
 ****************************************************************************/
inline static void reEstablishUMEntity(  RlcEntityType entityType,
                                         RLCEntityInfo *rlcEntityInfo_p 
                                      )
{

    switch (entityType)
    {
        case RLC_TX_ENTITY:
            reEstablishTxUMEntity (rlcEntityInfo_p );
            break;

        case RLC_RX_ENTITY:
            reEstablishRxUMEntity (rlcEntityInfo_p );
            break;

        case RLC_TX_RX_ENTITY:
            reEstablishTxRxUMEntity (rlcEntityInfo_p ); 
            break;

        default:
            break; 
    }
}
/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : rlcReEstbAmSduQueueAndSendDelInd
 * Inputs         : txAMEntity_p - pointer to TxAmEntity
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to re-establish rlc entity SDU Queue and
 *                  send delivery indication to pdcp.
 ****************************************************************************/
static inline UInt32 rlcReEstbAmSduQueueAndSendDelInd(
        TxAMEntity *txAMEntity_p)
{
    RlcStatusBuffer *rlcStatusBuffer = PNULL;
    UInt32 rlcStatusBufferCount = 0;      
    UInt16 nodeCount = QUEUE_COUNT_AMD_RAW_SDU_Q(txAMEntity_p->txSDUQueue);
    /*+COVERITY 5.3.0 - 32723*/
    if (nodeCount)
    /*-COVERITY 5.3.0 - 32723*/
    {
        rlcStatusBuffer = getMemFromPool((sizeof(RlcStatusBuffer) * nodeCount) + 1,RLC_NULL);
        if( RLC_NULL == rlcStatusBuffer)
        {
            ltePanic("%s => Memory Allocation Failure",__FUNCTION__);
            return RLC_FAILURE;
        }

        EMPTY_TX_AMD_RAW_SDU_Q_SET_STATUS_BUFFER(txAMEntity_p->txSDUQueue ,
                rlcStatusBuffer, rlcStatusBufferCount);
    }
    else 
    {
        /* Coverity 32723 fix */
        return RLC_SUCCESS;
    }
    LteRlcLCStats* lcStats_p =
        &(gRlcStats.pRlcStats->lteRlcUeStat[txAMEntity_p->ueIndex].
                lcStats[txAMEntity_p->lcId]);
    /* Send rlc delivery status indication to upper layer */
    if(rlcStatusBufferCount && txAMEntity_p->lcId >= MIN_DRB_LC_ID)
    {
        LOG_RLC_MSG(LTE_RLC_DELIVERY_STATUS_IND, LOGINFO, RLC_AM, globalTTITickCount_g, __LINE__,
                txAMEntity_p->ueIndex, txAMEntity_p->lcId, rlcStatusBufferCount,0, 0,0, __func__, "");
        CLOCK_START_RLC_API(RLC_DLVRY_STATUS_IND);
        rlcDeliveryStatusInd(txAMEntity_p->ueIndex, txAMEntity_p->lcId, rlcStatusBufferCount,
                rlcStatusBuffer);

        CLOCK_END_RLC_PDCP_API(RLC_DLVRY_STATUS_IND);
        lcStats_p->numDlvryInd += rlcStatusBufferCount;
    }
    else
    {
        freeMemPool(rlcStatusBuffer);
    }
    return RLC_SUCCESS;
}
/****************************************************************************
 * Function Name  : rlcReEstbAmReTxQueueAndSendDelInd
 * Inputs         : txAMEntity_p - pointer to TxAmEntity
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to re-establish rlc entity ReTx Queue and
 *                  send delivery indication to pdcp.
 ****************************************************************************/
static inline UInt32 rlcReEstbAmReTxQueueAndSendDelInd(
        TxAMEntity *txAMEntity_p)
{
    RlcStatusBuffer *rlcStatusBuffer = PNULL;
    UInt32 rlcStatusBufferCount = 0;    
    UInt16 Idx = 0;
    UInt16 nodeCount = 0;
    LteRlcLCStats* lcStats_p =
        &(gRlcStats.pRlcStats->lteRlcUeStat[txAMEntity_p->ueIndex].
                lcStats[txAMEntity_p->lcId]);
    for (; Idx < AM_WINDOW_SIZE; Idx++)
    {
        /* SPR 5034 Fix Start */
        if (txAMEntity_p->reTxQueue[Idx])
        {
            nodeCount = nodeCount + txAMEntity_p->reTxQueue[Idx]->dfeInfo.dfeCount;
        }
        /* SPR 5034 Fix End */
    }
    rlcStatusBuffer = getMemFromPool((sizeof(RlcStatusBuffer) * nodeCount) +1 ,RLC_NULL);
    /* SPR 4440 Fix End */
    if( RLC_NULL == rlcStatusBuffer)
    {
        ltePanic("%s => Memory Allocation Failure",__FUNCTION__);
        return RLC_FAILURE;
    }

    EMPTY_TX_AM_RETX_Q_SET_STATUS_BUFFER(txAMEntity_p->reTxQueue ,
            txAMEntity_p->tidQueue, txAMEntity_p->ueIndex, rlcStatusBuffer,
            rlcStatusBufferCount);

    /* Send rlc delivery status indication to upper layer */
    /** SPR 3986 Fix Start **/
    if(rlcStatusBufferCount && txAMEntity_p->lcId >= MIN_DRB_LC_ID)
    /** SPR 3986 Fix End **/
    {
        /* Change for SPR 2203 Start */
        LOG_RLC_MSG(LTE_RLC_DELIVERY_STATUS_IND_1, LOGINFO, 
                RLC_AM, globalTTITickCount_g, __LINE__,
                txAMEntity_p->ueIndex, txAMEntity_p->lcId, rlcStatusBufferCount,0, 0,0, __func__, "");
        /* Change for SPR 2203 End */
        CLOCK_START_RLC_API(RLC_DLVRY_STATUS_IND);

        rlcDeliveryStatusInd(txAMEntity_p->ueIndex, txAMEntity_p->lcId, rlcStatusBufferCount,
                rlcStatusBuffer);
        CLOCK_END_RLC_PDCP_API(RLC_DLVRY_STATUS_IND);
        lcStats_p->numDlvryInd += rlcStatusBufferCount;
    }
    else
    {
        freeMemPool(rlcStatusBuffer);
    }
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : rlcCheckAndStopAmTimers
 * Inputs         : txAMEntity_p - pointer to TxAmEntity,
 *                : rxAMEntity_p - pointer to RxAmEntity
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to stop rlc Am entity timers.
 ****************************************************************************/
static inline void rlcCheckAndStopAmTimers(
        TxAMEntity *txAMEntity_p,
        RxAMEntity *rxAMEntity_p)
{
    if(txAMEntity_p->istRetransmitTimerRunning)
    {
        stopPRTimer(txAMEntity_p);
        /* SPR 4663 Fix Start */
        txAMEntity_p->istRetransmitTimerRunning = RLC_FALSE;
        /* SPR 4663 Fix End */
    }
    if(rxAMEntity_p->isRTimerRunning)
    {
        stopAMROTimer(rxAMEntity_p);
        /* SPR 4663 Fix Start */
        rxAMEntity_p->isRTimerRunning = RLC_FALSE;
        /* SPR 4663 Fix End */
    }
    if(rxAMEntity_p->isSPTimerRunning)
    {
        stopSPTimer(rxAMEntity_p);
        /* SPR 4663 Fix Start */
        rxAMEntity_p->isSPTimerRunning = RLC_FALSE;
        /* SPR 4663 Fix End */
    }
}
/* Cyclomatic Complexity changes - ends here */

/****************************************************************************
 * Function Name  : reEstablishAMEntity
 * Inputs         : rlcEntityInfo_p - Pointer to RLCEntityInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to re-establish UE entity. This API shall
 *                  be called by the handler of reEstablish Queue.
 ****************************************************************************/

inline static UInt32 reEstablishAMEntity( RLCEntityInfo *rlcEntityInfo_p)
{
    /* SPR 3822 Changes Start */
    UInt8 lcCount = 1;
    /* SPR 3822 Changes End */
    UInt8 idx = 0;
    LCQueueLoad lcQLoadArr_p[MAX_LC_ID];
    /* checking the type of entity either tx or rx */
    TxAMEntity *txAMEntity_p =
        rlcEntityInfo_p->rlcEntity.amEntity_p->txAMEntity_p;
    RxAMEntity *rxAMEntity_p =
        rlcEntityInfo_p->rlcEntity.amEntity_p->rxAMEntity_p;
    /* Cyclomatic Complexity changes - starts here */
    UInt32 ret = RLC_SUCCESS;
    memSet(&lcQLoadArr_p,0,sizeof(LCQueueLoad));
    ret = rlcReEstbAmSduQueueAndSendDelInd(txAMEntity_p);
    if(ret != RLC_SUCCESS)
    {
        return ret;
    }
    /* Cyclomatic Complexity changes - ends here */

    msgFree(txAMEntity_p->txSduIn.rlcAmRawSdu.data_p);
    txAMEntity_p->txSduIn.rlcAmRawSdu.data_p = RLC_NULL;
    /* Start 128 UE: 25% NACK Changes */
    txAMEntity_p->txSduIn.rlcAmRawSdu.size = 0;//v_g
    /* End 128 UE: 25% NACK Changes */
    txAMEntity_p->txSduIn.sn = 0;
    txAMEntity_p->txQueueSize = 0;

    txAMEntity_p->vtA = 0;
    txAMEntity_p->vtMS = txAMEntity_p->vtA + AM_WINDOW_SIZE;
    txAMEntity_p->vtS = 0;
    txAMEntity_p->pduWithoutPoll = 0;
    txAMEntity_p->pollSN = 0;
    txAMEntity_p->pollBit = 0;
    txAMEntity_p->byteWithoutPoll = 0;
    /** Rel 4.0 Changes Start **/
#ifndef UE_SIM_TESTING
    /*SPR 4596 Fix +*/
    //txAMEntity_p->rlcQueueSize = RLC_QUEUE_SIZE;
    /*SPR 4596 Fix -*/
    /* SPR 4010 Fix Start */
    txAMEntity_p->qci = 0;
    rxAMEntity_p->qci = 0;
    /* SPR 4010 Fix End */
    txAMEntity_p->bitRateTx = 0;
#endif
    /** Rel 4.0 Changes End **/

    EMPTY_TX_AM_NACK_LIST(txAMEntity_p->nackList);
    txAMEntity_p->statusReportAck = 0;
   
    /* Cyclomatic Complexity changes - starts here */
    ret = rlcReEstbAmReTxQueueAndSendDelInd(txAMEntity_p);
    if(ret != RLC_SUCCESS)
    {
        return ret;
    }
    /* Cyclomatic Complexity changes - ends here */

    txAMEntity_p->txPduIn.sn = 0;
    txAMEntity_p->txPduIn.Sostart = 0;
    txAMEntity_p->txPduIn.Soend = 0;
    msgFree(txAMEntity_p->txPduIn.pRawPdu.data_p);
    txAMEntity_p->txPduIn.pRawPdu.data_p = RLC_NULL;
    txAMEntity_p->txPduIn.pRawPdu.size = 0;
    txAMEntity_p->reTxQueueSize = 0;
    txAMEntity_p->rxStatusPduSize = 0;
    txAMEntity_p->sendStatusInd = RLC_FALSE;
    EMPTY_TX_AM_SDU_TID_Q(txAMEntity_p->tidQueue);
    /* Cyclomatic Complexity changes - starts here */
    rlcCheckAndStopAmTimers(txAMEntity_p,rxAMEntity_p);
    /* Cyclomatic Complexity changes - ends here */

    EMPTY_RLC_CONTROL_PDU_Q();
    lcQLoadArr_p[idx].lcId = txAMEntity_p->lcId ;
    lcQLoadArr_p[idx].lcQueueLoad = 0;

    lcQLoadArr_p[idx].scheduledDataBytes = 0;
    lcQLoadArr_p[idx].headPktTS = 0;
    lcQLoadArr_p[idx].retxnPduSize = 0;
    lcQLoadArr_p[idx].ctrlQueueLoad = 0;
    lcQLoadArr_p[idx].ctrlFlag = RLC_NORMAL_DATA;

    macUpdateBufferOccupancyReq( txAMEntity_p->ueIndex,
            lcCount,lcQLoadArr_p,
            RLC_BO_TYPE_NEW,
            rlcUEContextList_g[ txAMEntity_p->ueIndex]->cellIndex
            );

    rlcReEstabReassRxAMEntity(rxAMEntity_p);
    rxAMEntity_p->RLCRESETFLAG = RLC_FALSE;
    /* rxAM Entity */
    rxAMEntity_p->sendStatusInd = 0;
	rxAMEntity_p->pollDelayInd = 0;//bug[003]jsm
	rxAMEntity_p->pollDelaySN = 65535;//bug[003]jsm
    rxAMEntity_p->vrR = 0;
    rxAMEntity_p->vrMR = AM_WINDOW_SIZE;
    /* Review comment fix start MAC_AT_4 */
    rxAMEntity_p->vrX = RLC_UNULL;
    /* Review comment fix end MAC_AT_4 */
    rxAMEntity_p->vrMS = 0;
    rxAMEntity_p->vrH = 0;
    /** Rel 4.0 Changes Start **/
#ifndef UE_SIM_TESTING
    /* +COVERITY 10058 */
    rxAMEntity_p->qci = 0;
    /* rxAMEntity_p->qci = rxAMEntity_p->qci = 0; */
    /* -COVERITY 10058 */
    rxAMEntity_p->bitRateRx = 0;
#endif
    /** Rel 4.0 Changes End **/


    msgFree(rxAMEntity_p->rxSduIn.rlcAmRawSdu.data_p);
    rxAMEntity_p->rxSduIn.rlcAmRawSdu.data_p = RLC_NULL;
    rxAMEntity_p->rxSduIn.rlcAmRawSdu.size = 0;
    rxAMEntity_p->rxSduIn.sn = 0;

    LOG_RLC_MSG(LTE_RLC_REESTAB_AM_RX_ENTITY_1, LOGINFO, RLC_RRC, globalTTITickCount_g,
            rxAMEntity_p->ueIndex, rxAMEntity_p->lcId, rxAMEntity_p->vrR ,
            rxAMEntity_p->vrH, rxAMEntity_p->vrMR,0,0, __func__, "");

    return RLC_SUCCESS;        
}
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING

/*RLC Rel 2.0 Changes Start*/
/****************************************************************************
 * Function Name  : reEstablishRxAMEntity
 * Inputs         : rlcEntityInfo_p - Pointer to RLCEntityInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS
 * Description    : This API is used to re-establish UE Rx entity. This API shall
 *                  be called by the handler of reEstablish Queue.
 ****************************************************************************/
inline static UInt32 reEstablishRxAMEntity( RLCEntityInfo *rlcEntityInfo_p)
{
    RxAMEntity *rxAMEntity_p = rlcEntityInfo_p->rlcEntity.amEntity_p->rxAMEntity_p;
    rlcReEstabReassRxAMEntity(rxAMEntity_p);
    rxAMEntity_p->RLCRESETFLAG = RLC_FALSE;
    if(rxAMEntity_p->isRTimerRunning)
    {
        stopAMROTimer(rxAMEntity_p);
        /* SPR 4663 Fix Start */
        rxAMEntity_p->isRTimerRunning = RLC_FALSE;
        /* SPR 4663 Fix End */
    }
    if(rxAMEntity_p->isSPTimerRunning)
    {
        stopSPTimer(rxAMEntity_p);
        /* SPR 4663 Fix Start */
        rxAMEntity_p->isSPTimerRunning = RLC_FALSE;
        /* SPR 4663 Fix End */
    }
    rxAMEntity_p->sendStatusInd = 0;
	rxAMEntity_p->pollDelayInd = 0;//bug[003]jsm
	rxAMEntity_p->pollDelaySN = 65535;//bug[003]jsm
    rxAMEntity_p->vrR = 0;
    rxAMEntity_p->vrMR = AM_WINDOW_SIZE;
    /* Review comment fix start MAC_AT_4 */
    rxAMEntity_p->vrX = RLC_UNULL;
    /* Review comment fix end MAC_AT_4 */
    rxAMEntity_p->vrMS = 0;
    rxAMEntity_p->vrH = 0;
    rxAMEntity_p->qci = 0;
    rxAMEntity_p->bitRateRx = 0;

    msgFree(rxAMEntity_p->rxSduIn.rlcAmRawSdu.data_p);
    rxAMEntity_p->rxSduIn.rlcAmRawSdu.data_p = RLC_NULL;
    rxAMEntity_p->rxSduIn.rlcAmRawSdu.size = 0;
    rxAMEntity_p->rxSduIn.sn = 0;

    LOG_RLC_MSG(LTE_RLC_REESTAB_AM_RX_ENTITY_1, LOGINFO, RLC_RRC, globalTTITickCount_g,
            rxAMEntity_p->ueIndex, rxAMEntity_p->lcId, rxAMEntity_p->vrR ,
            rxAMEntity_p->vrH, rxAMEntity_p->vrMR,0,0, __func__, "");

    return RLC_SUCCESS;    
}


/****************************************************************************
 * Function Name  : reEstablishTxAMEntity
 * Inputs         : rlcEntityInfo_p - Pointer to RLCEntityInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to re-establish UE Tx entity. This API shall
 *                  be called by the handler of reEstablish Queue.
 ****************************************************************************/
inline static UInt32 reEstablishTxAMEntity( RLCEntityInfo *rlcEntityInfo_p)
{
    /* SPR 21778 Fix code deleted */
    /* checking the type of entity either tx or rx */
    TxAMEntity *txAMEntity_p =
        rlcEntityInfo_p->rlcEntity.amEntity_p->txAMEntity_p;
    /* SPR 4440 Fix Start */
    /* Cyclomatic Complexity changes - starts here */
    UInt32 ret = RLC_SUCCESS;
    ret = rlcReEstbAmSduQueueAndSendDelInd(txAMEntity_p);
    if(ret != RLC_SUCCESS)
    {
        return ret;
    }
    /* Cyclomatic Complexity changes - ends here */

    msgFree(txAMEntity_p->txSduIn.rlcAmRawSdu.data_p);
    txAMEntity_p->txSduIn.rlcAmRawSdu.data_p = RLC_NULL;
    txAMEntity_p->txSduIn.sn = 0;
    txAMEntity_p->txQueueSize = 0;
    /* Start 128 UE: 25% NACK Changes */
    txAMEntity_p->txSduIn.rlcAmRawSdu.size = 0; //v_g
    /* End 128 UE: 25% NACK Changes */

    txAMEntity_p->vtA = 0;
    txAMEntity_p->vtMS = txAMEntity_p->vtA + AM_WINDOW_SIZE;
    txAMEntity_p->vtS = 0;
    txAMEntity_p->pduWithoutPoll = 0;
    txAMEntity_p->pollSN = 0;
    txAMEntity_p->pollBit = 0;
    txAMEntity_p->byteWithoutPoll = 0;

    /*SPR 4596 Fix +*/
    //txAMEntity_p->rlcQueueSize = RLC_QUEUE_SIZE;
    /*SPR 4596 Fix +*/
    txAMEntity_p->qci = 0;
    txAMEntity_p->bitRateTx = 0;

    EMPTY_TX_AM_NACK_LIST(txAMEntity_p->nackList);
    txAMEntity_p->statusReportAck = 0;

    /* Cyclomatic Complexity changes - starts here */
    ret = rlcReEstbAmReTxQueueAndSendDelInd(txAMEntity_p);
    if(ret != RLC_SUCCESS)
    {
        return ret;
    }
    /* Cyclomatic Complexity changes - ends here */

    txAMEntity_p->txPduIn.sn = 0;
    txAMEntity_p->txPduIn.Sostart = 0;
    txAMEntity_p->txPduIn.Soend = 0;
    msgFree(txAMEntity_p->txPduIn.pRawPdu.data_p);
    txAMEntity_p->txPduIn.pRawPdu.data_p = RLC_NULL;
    txAMEntity_p->txPduIn.pRawPdu.size = 0;
    txAMEntity_p->reTxQueueSize = 0;
    txAMEntity_p->rxStatusPduSize = 0;
    txAMEntity_p->sendStatusInd = RLC_FALSE;
    EMPTY_TX_AM_SDU_TID_Q(txAMEntity_p->tidQueue);
    if(txAMEntity_p->istRetransmitTimerRunning)
    {
        stopPRTimer(txAMEntity_p);
        /* SPR 4663 Fix Start */
        txAMEntity_p->istRetransmitTimerRunning = RLC_FALSE;
        /* SPR 4663 Fix End */
    }
    EMPTY_RLC_CONTROL_PDU_Q();
    /* SPR 21778 Fix code deleted */
    return RLC_SUCCESS;
}

#endif

/*RLC Rel 2.0 Changes Start*/
/****************************************************************************
 * Function Name  : rlcReEstabReassRxAMEntity 
 * Inputs         : rxAMEntity_p - a pointer to RxAMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : This API used to reassemble the data in AmPdu queue of 
 *                  AM Rx type of entity.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
void rlcReEstabReassRxAMEntity (
/* SPR 5599_129119 Fix Start */
                                        RxAMEntity* rxAMEntity_p
                                      )
{
/* SPR 4440 Fix Start */   
    AmdPduRing *ring = &(rxAMEntity_p->rxAmdPduRing);
    RlcVr vrH     = rxAMEntity_p->vrH;
    UInt32 rCount = rxAMEntity_p->vrR;
    SInt32 mBase  = vrH - AM_WINDOW_SIZE;

    /* initializing the SN List */
    RlcSnList snList = RLC_SN_LIST_INIT ;
    /* for( ;!(AM_EQ(rCount,vrH ,mBase)) ;
            rCount = AM_ASSIGN(rCount+1)) */
    /** SPR 6388 Changes Start **/
    for ( ; isAMLT( rCount, vrH, mBase ); rCount = (rCount + 1 ) & 1023)
    /** SPR 6388 Changes End **/
    {
        if(GET_RING_SS_AMD_PDU_RN(ring,rCount & 511 ) == RING_SLOT_IN_USE)
        {
            SET_RING_SS_AMD_PDU_RN(ring,rCount & 511,RING_SLOT_READY);
            rlcSetSnList(&snList,rCount & 511);
        }
    }
    rxAMEntity_p->RLCRESETFLAG = RLC_TRUE;
    /*reassemble RLC SDUs */
    if(snList.szUsed != 0)
    {
        //rlcReassembleAmdSdu(rxAMEntity_p,&snList);
        rlcReassembleAmdSduForReestablish(rxAMEntity_p,&snList);
    }

/* SPR 4440 Fix End */   
}
/*RLC Rel 2.0 Changes End*/

/****************************************************************************
 * Function Name  : rlcReEstabReassRxUMEntity 
 * Inputs         : rxUMEntity_p - a pointer to RxUMEntity.
 * Outputs        : None
 * Returns        : None
 * Description    : This API used to reassemble the data in UmPdu queue of 
 *                  UM Rx type of entity.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
void rlcReEstabReassRxUMEntity (
/* SPR 5599_129119 Fix End */
                                        RxUMEntity* rxUMEntity_p
                                      )
{
/* SPR 4440 Fix Start */
    UmdPduRing *ring = &(rxUMEntity_p->rxUmdPduRing);
    RlcVr UH = rxUMEntity_p->UH ;
    UInt16 modValue = rxUMEntity_p->modValForWrapRound;
    SInt32 mBase = (UH - (rxUMEntity_p->wSize));
    /* initializing the SN List */
    RlcSnList snList = RLC_SN_LIST_INIT ;
   // UInt32 rCount = UM_ASSIGN(UH - wSize,snLen);
    UInt16 rCount = rxUMEntity_p->UR;
   /* for( ;!(UM_EQ(rCount,UH ,mBase,snLen)) ;
            rCount = UM_ASSIGN(rCount+1 ,snLen)) */

    /** SPR 6388 Changes Start **/
    for ( ; isUmLT(rCount, UH, mBase, modValue); rCount = (rCount + 1 ) & modValue)
    /** SPR 6388 Changes End **/
    {
        if(GET_RING_SS_UMD_PDU_RN(ring, rCount, modValue) == RING_SLOT_IN_USE){
            SET_RING_SS_UMD_PDU_RN(ring, (rCount & modValue),RING_SLOT_READY);
            rlcSetSnList(&snList,(rCount & modValue));
        }
    }
    /*reassemble RLC SDUs */
    if(snList.szUsed != 0)
    {
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
        if(rxUMEntity_p->mchEntity)
        {
            rlcCleanUmSdu(rxUMEntity_p,&snList);
        }
        else
#endif
        {
        rlcReassembleUmSdu(rxUMEntity_p,&snList);
    }
    }
/* SPR 4440 Fix End */
}


/* Cyclomatic Complexity changes - starts here */
#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : rlcConfigUmRxQciParam 
 * Inputs         : rxUMEntity_p - a pointer to RLCRxUmEntity
 *                  rlcEntityCfgInfo_p - a pointer to RlcEntityCfgInfo,
 *                  ueIndex - ueIndex value
 * Outputs        : None
 * Returns        : None
 * Description    : This API used to configure Qci info of RLC Entity.
 ****************************************************************************/
static inline void rlcConfigUmRxQciParam(
        RxUMEntity* rxUMEntity_p,
        RlcEntityCfgInfo* rlcEntityCfgInfo_p,
        UInt16 ueIndex
        )
{
    rxUMEntity_p->qci = rlcEntityCfgInfo_p->rlcQciInfo.qci;
    rxUMEntity_p->bitRateRx = rlcEntityCfgInfo_p->configParam.umUniDirectionalRx.
        rxUmRlc.bitRateRx;
    if(!(rxUMEntity_p->bitRateRx))
    { 
        rxUMEntity_p->bitRateRx = rlcUeBitRateInfo_g[ueIndex].bitRateRx; 
    }
}
/****************************************************************************
 * Function Name  : rlcConfigUmTxQciParam 
 * Inputs         : txUMEntity_p - a pointer to RLCTxUmEntity,
 *                  rlcEntityCfgInfo_p - a pointer to RlcEntityCfgInfo,
 *                  ueIndex - ueIndex value
 * Outputs        : None
 * Returns        : None
 * Description    : This API used to configure Qci info of RLC Entity.
 ****************************************************************************/
static inline void rlcConfigUmTxQciParam(
        TxUMEntity* txUMEntity_p,
        RlcEntityCfgInfo* rlcEntityCfgInfo_p,
        UInt16 ueIndex
        )
{
    txUMEntity_p->qci = rlcEntityCfgInfo_p->rlcQciInfo.qci;
    txUMEntity_p->bitRateTx = 
        rlcEntityCfgInfo_p->configParam.umUniDirectionalTX.
        txUmRlc.bitRateTx;
    if(rlcEntityCfgInfo_p->configParam.umUniDirectionalTX.
        txUmRlc.rlcQueueSize)
    {
        txUMEntity_p->rlcQueueSize = rlcEntityCfgInfo_p->configParam.
            umUniDirectionalTX.txUmRlc.rlcQueueSize;
    }
    if(!(txUMEntity_p->bitRateTx))
    {
        txUMEntity_p->bitRateTx = rlcUeBitRateInfo_g[ueIndex].bitRateTx;
    }
    if((txUMEntity_p->qci) && (txUMEntity_p->rlcQueueSize == RLC_QUEUE_SIZE))
    {
        if(rlcQosQciInfo_g[txUMEntity_p->qci -1].eResType == RLC_GBR_LC)
        {
            txUMEntity_p->rlcQueueSize = (txUMEntity_p->bitRateTx)/(1000*8) * 
               (rlcQosQciInfo_g[txUMEntity_p->qci -1].pktDlyBdgt * 
               qSizeFactor[qSizeFactorGBR_g -1]); 
        }
        else
        {
            txUMEntity_p->rlcQueueSize = (txUMEntity_p->bitRateTx)/(1000*8) *
              (rlcQosQciInfo_g[txUMEntity_p->qci -1].pktDlyBdgt * 
               qSizeFactor[qSizeFactorNGBR_g -1]);
        }
    }
}
/****************************************************************************
 * Function Name  : rlcConfigUmTxRxQciParam 
 * Inputs         : txUMEntity_p - a pointer to RLCTxUmEntity,
 *                : rxUMEntity_p - a pointer to RLCRxUmEntity,
 *                : rlcEntityCfgInfo_p - a pointer to RlcEntityCfgInfo,
 *                : ueIndex - ueIndex value
 * Outputs        : None
 * Returns        : None
 * Description    : This API used to configure Qci info of RLC Entity.
 ****************************************************************************/
static inline void rlcConfigUmTxRxQciParam(
        TxUMEntity* txUMEntity_p,
        RxUMEntity* rxUMEntity_p,
        RlcEntityCfgInfo* rlcEntityCfgInfo_p,
        UInt16 ueIndex
        )
{
    rxUMEntity_p->qci = txUMEntity_p->qci  = 
        rlcEntityCfgInfo_p->rlcQciInfo.qci;
    rxUMEntity_p->bitRateRx = rlcEntityCfgInfo_p->configParam.umBiDirectional.
        rxUmRlc.bitRateRx;
    txUMEntity_p->bitRateTx = rlcEntityCfgInfo_p->configParam.umBiDirectional.
        txUmRlc.bitRateTx;
    if(!(rxUMEntity_p->bitRateRx))
    {
        rxUMEntity_p->bitRateRx = rlcUeBitRateInfo_g[ueIndex].bitRateRx; 
    }
    if(!(txUMEntity_p->bitRateTx))
    {
        txUMEntity_p->bitRateTx = rlcUeBitRateInfo_g[ueIndex].bitRateTx;
    }
    if(rlcEntityCfgInfo_p->configParam.umBiDirectional.
        txUmRlc.rlcQueueSize)
    {
        txUMEntity_p->rlcQueueSize = rlcEntityCfgInfo_p->configParam.
            umBiDirectional.txUmRlc.rlcQueueSize;
    }
    if((txUMEntity_p->qci) && (txUMEntity_p->rlcQueueSize == RLC_QUEUE_SIZE))
    {
        if(rlcQosQciInfo_g[txUMEntity_p->qci -1].eResType == RLC_GBR_LC)
        {
            txUMEntity_p->rlcQueueSize = (txUMEntity_p->bitRateTx)/(1000 * 8) *
                (rlcQosQciInfo_g[txUMEntity_p->qci -1].pktDlyBdgt * 
                 qSizeFactor[qSizeFactorGBR_g -1]);
        }
        else
        {
            txUMEntity_p->rlcQueueSize = (txUMEntity_p->bitRateTx)/(1000 * 8) *
                (rlcQosQciInfo_g[txUMEntity_p->qci -1].pktDlyBdgt * 
                 qSizeFactor[qSizeFactorNGBR_g - 1]);
        }
    }
}
/****************************************************************************
 * Function Name  : rlcConfigAmTxRxQciParam 
 * Inputs         : txAMEntity_p - a pointer to RLCTxAmEntity, 
 *                : rxAMEntity_p - a pointer to RLCRxAmEntity,
 *                : rlcEntityCfgInfo_p - a pointer to RlcEntityCfgInfo,
 *                : ueIndex - ueIndex value
 * Outputs        : None
 * Returns        : None
 * Description    : This API used to configure Qci info of RLC Entity.
 ****************************************************************************/
static inline void rlcConfigAmTxRxQciParam(
        TxAMEntity* txAMEntity_p,
        RxAMEntity* rxAMEntity_p,
        RlcEntityCfgInfo* rlcEntityCfgInfo_p,
        UInt16 ueIndex
        )
{
    txAMEntity_p->qci = rxAMEntity_p->qci = 
        rlcEntityCfgInfo_p->rlcQciInfo.qci;
    txAMEntity_p->bitRateTx = rlcEntityCfgInfo_p->configParam.am.txAmRlc.
        bitRateTx;
    if(rlcEntityCfgInfo_p->configParam.am.txAmRlc.
        rlcQueueSize)
    {
        txAMEntity_p->rlcQueueSize = rlcEntityCfgInfo_p->configParam.
            am.txAmRlc.rlcQueueSize; 
    }
    rxAMEntity_p->bitRateRx = rlcEntityCfgInfo_p->configParam.am.rxAmRlc.
        bitRateRx;
              
    if(!(txAMEntity_p->bitRateTx))
    {
        txAMEntity_p->bitRateTx = rlcUeBitRateInfo_g[ueIndex].bitRateTx;
    }
    if(!(rxAMEntity_p->bitRateRx))
    {
        rxAMEntity_p->bitRateRx = rlcUeBitRateInfo_g[ueIndex].bitRateRx;
    }
    if((txAMEntity_p->qci) && (txAMEntity_p->rlcQueueSize == RLC_QUEUE_SIZE))
    {
        if(rlcQosQciInfo_g[txAMEntity_p->qci -1].eResType == RLC_GBR_LC)
        {
            txAMEntity_p->rlcQueueSize = 
               (txAMEntity_p->bitRateTx)/(1000 * 8) *
                   (rlcQosQciInfo_g[txAMEntity_p->qci -1].pktDlyBdgt * 
                   qSizeFactor[qSizeFactorGBR_g - 1]);
        }
        else
        {
            txAMEntity_p->rlcQueueSize = 
               (txAMEntity_p->bitRateTx)/(1000 * 8) *
               (rlcQosQciInfo_g[txAMEntity_p->qci -1].pktDlyBdgt * 
                   qSizeFactor[qSizeFactorNGBR_g - 1]);
        }
     }
}
#endif

/* Cyclomatic Complexity changes - ends here */

/*******************************************************************************
 * Function Name  : rlcConfigEntityParam 
 * Inputs         : outRLCEntityInfo_p - a pointer to RLCEntityInfo,
 *                  rlcEntityCfgInfo_p - a pointer to RlcEntityCfgInfo,
 *                  ueIndex - ueIndex value
 * Outputs        : None
 * Returns        : None
 * Description    : This API used to configure Entity using requested parameter.
 *******************************************************************************/

inline static void rlcConfigEntityParam ( RLCEntityInfo *outRLCEntityInfo_p, 
                                          RlcEntityCfgInfo* rlcEntityCfgInfo_p, 
                                          UInt16 ueIndex
                                        )
{
    RlcEntityMode eM = rlcEntityCfgInfo_p->entityMode;
    RlcEntityType eT = rlcEntityCfgInfo_p->entityType;

    switch( eM )
    {
        case RLC_ENTITY_UM:
            {
                switch(eT)
                {
                    case RLC_RX_ENTITY:
                        {

                            RxUMEntity* rxUMEntity_p = outRLCEntityInfo_p->
                                rlcEntity.umEntity_p->rxUMEntity_p;
                            rxUMEntity_p->ueIndex = ueIndex;
                            rxUMEntity_p->lcId = rlcEntityCfgInfo_p->lcId;
                            /* config the param for Um Rx Entity */
                            rlcConfigUmRxEntityParam(rxUMEntity_p,
                                    rlcEntityCfgInfo_p->configParam,
                                    /* SPR 4272 Fix Start */
                                    eT );
                                    /* SPR 4272 Fix End */

                            /** Rel 4.0 changes Start **/
#ifndef UE_SIM_TESTING
                            /* Cyclomatic Complexity changes - starts here */
                            rlcConfigUmRxQciParam(rxUMEntity_p,rlcEntityCfgInfo_p,ueIndex);
                            /* Cyclomatic Complexity changes - ends here */
#endif
                            /** Rel 4.0 changes End **/ 
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                            outRLCEntityInfo_p->rlcEntity.umEntity_p->rxEntityState = RLC_ENTITY_ACTIVE;
#endif                            

                        }
                        break;
                    case RLC_TX_ENTITY:
                        {
                            TxUMEntity* txUMEntity_p = outRLCEntityInfo_p->
                                rlcEntity.umEntity_p->txUMEntity_p;
                            txUMEntity_p->ueIndex = ueIndex;
                            txUMEntity_p->lcId = rlcEntityCfgInfo_p->lcId;

                            /* config the param for Um Tx Entity */
                            rlcConfigUmTxEntityParam(txUMEntity_p,
                                    rlcEntityCfgInfo_p->configParam );

                            /** Rel 4.0 changes Start **/
#ifndef UE_SIM_TESTING
                            /* Cyclomatic Complexity changes - starts here */
                            rlcConfigUmTxQciParam(txUMEntity_p,rlcEntityCfgInfo_p,ueIndex);
                            /* Cyclomatic Complexity changes - ends here */
#endif
                            /** Rel 4.0 changes End **/ 
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                            outRLCEntityInfo_p->rlcEntity.umEntity_p->txEntityState = RLC_ENTITY_ACTIVE;
#endif                            
                        }
                        break;
                    case RLC_TX_RX_ENTITY:
                        {
                            RxUMEntity* rxUMEntity_p = outRLCEntityInfo_p->
                                rlcEntity.umEntity_p->rxUMEntity_p;
                            rxUMEntity_p->ueIndex = ueIndex;
                            rxUMEntity_p->lcId = rlcEntityCfgInfo_p->lcId;
                            /* defined both pointers as it is bi-directional */
                            TxUMEntity* txUMEntity_p = outRLCEntityInfo_p->
                                rlcEntity.umEntity_p->txUMEntity_p;
                            txUMEntity_p->ueIndex = ueIndex;
                            txUMEntity_p->lcId = rlcEntityCfgInfo_p->lcId;

                            /* config the param for Um Rx Entity */
                            rlcConfigUmRxEntityParam(rxUMEntity_p,
                                    rlcEntityCfgInfo_p->configParam,
                                    /* SPR 4272 Fix Start */
                                    eT );
                                    /* SPR 4272 Fix End */
                            /* config the param for Um Tx Entity */
                            rlcConfigUmTxEntityParam(txUMEntity_p,
                                    rlcEntityCfgInfo_p->configParam);
                            /** Rel 4.0 changes Start **/
#ifndef UE_SIM_TESTING
                            /* Cyclomatic Complexity changes - starts here */
                            rlcConfigUmTxRxQciParam(txUMEntity_p,rxUMEntity_p,
                                    rlcEntityCfgInfo_p,ueIndex);
                            /* Cyclomatic Complexity changes - ends here */
#endif
                        /** Rel 4. 0changes End **/
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                            outRLCEntityInfo_p->rlcEntity.umEntity_p->txEntityState = RLC_ENTITY_ACTIVE;
                            outRLCEntityInfo_p->rlcEntity.umEntity_p->rxEntityState = RLC_ENTITY_ACTIVE;
#endif                            
                        }
                        break;
                    default:
                        {
                            RLC_LOG(LOG_WARNING,"Invalid Entity Type");
                        }
                }/* end of switch for eT */
            } /* end of UM mode */
            break;
            /*RLC Rel 2.0 Changes Start*/
        case RLC_ENTITY_AM:
            {
                switch(eT)
                {
                    case RLC_TX_RX_ENTITY:
                        {
                            RxAMEntity* rxAMEntity_p = outRLCEntityInfo_p->
                                rlcEntity.amEntity_p->rxAMEntity_p;
                            rxAMEntity_p->ueIndex = ueIndex;
                            rxAMEntity_p->lcId = rlcEntityCfgInfo_p->lcId;

                            /* defined both pointers as it is bi-directional */
                            TxAMEntity* txAMEntity_p = outRLCEntityInfo_p->
                                rlcEntity.amEntity_p->txAMEntity_p;
                            txAMEntity_p->ueIndex = ueIndex;
                            txAMEntity_p->lcId = rlcEntityCfgInfo_p->lcId;

                            /* config the param for Am Rx Entity */
                            rlcConfigAmRxEntityParam(rxAMEntity_p,
                                    rlcEntityCfgInfo_p->configParam );
                            /* config the param for Am Tx Entity */
                            rlcConfigAmTxEntityParam(txAMEntity_p,
                                    rlcEntityCfgInfo_p->configParam);
                            /** Rel 4.0 changes Start **/
#ifndef UE_SIM_TESTING
                            /* Cyclomatic Complexity changes - starts here */
                            rlcConfigAmTxRxQciParam(txAMEntity_p,rxAMEntity_p,
                                    rlcEntityCfgInfo_p,ueIndex);
                            /* Cyclomatic Complexity changes - ends here */

#endif
                            /** Rel 4.0 changes End **/
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                            outRLCEntityInfo_p->rlcEntity.amEntity_p->txEntityState = RLC_ENTITY_ACTIVE;
                            outRLCEntityInfo_p->rlcEntity.amEntity_p->rxEntityState = RLC_ENTITY_ACTIVE;
#endif                            
                        }
                        break;
                    default:
                        {
                            RLC_LOG(LOG_WARNING,"Invalid Entity Type");
                        }
                }
            }
            break;
            /*RLC Rel 2.0 Changes End*/
        default:
            {
                RLC_LOG(LOG_WARNING,"Invalid Entity Mode");
            }
    } /* end of switch for eM */        
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    outRLCEntityInfo_p->rlcEntityState = RLC_ENTITY_ACTIVE;
#endif    
}

/*RLC Rel 2.0 Changes Start*/
/****************************************************************************
 * Function Name  : rlcConfigAmRxEntityParam 
 * Inputs         : rxUMEntity_p - pointer to RxAMEntity,
 *                  rlcConfigParam - pointer to RlcConfigParam
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to configure the rxAmEntity using requested
 *                  parameter.
 ****************************************************************************/

inline static void rlcConfigAmRxEntityParam ( RxAMEntity *rxAMEntity_p, 
                                              RlcConfigParam rlcConfigParam
                                            )
{
    rxAMEntity_p->amROTimerInfo.roDuration = rlcConfigParam.am.rxAmRlc.tReordering; 

    rlcChangeSizeAmdPduR(&(rxAMEntity_p->rxAmdPduRing),AM_WINDOW_SIZE);
    
    rxAMEntity_p->spTimerInfo.spDuration = rlcConfigParam.am.rxAmRlc.tStatusProhibit; 
#ifdef UE_SIM_TESTING    
    if(rxAMEntity_p->spTimerInfo.spDuration == 0)
    {
        rxAMEntity_p->spTimerInfo.spDuration = 60;
    }
#endif  

#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
        rxAMEntity_p->dcBearerType = rlcConfigParam.am.rxAmRlc.dcBearerType;
#endif

}

/****************************************************************************
 * Function Name  : rlcConfigAmTxEntityParam
 * Inputs         : txAMEntity_p - pointer to TxAMEntity API,
 *                  rlcConfigParam - pointer to RlcConfigParam
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to configure the txAmEntity using requested
 *                  parameter.
 ****************************************************************************/

inline static void rlcConfigAmTxEntityParam ( TxAMEntity *txAMEntity_p, 
                                              RlcConfigParam rlcConfigParam 
                                            )
{
    txAMEntity_p->pollPDU  = rlcConfigParam.am.txAmRlc.pollPDU;
    txAMEntity_p->pollByte = rlcConfigParam.am.txAmRlc.pollByte;
    txAMEntity_p->maxRetxThreshold = rlcConfigParam.am.txAmRlc.maxRetxThreshold;

    txAMEntity_p->prTimerInfo.prDuration = rlcConfigParam.am.txAmRlc.tPollRetransmit;

#ifdef UE_SIM_TESTING   
    /* Coverity Fix 64222 Start */ 
    if(txAMEntity_p->maxRetxThreshold != t4)
    {
        txAMEntity_p->maxRetxThreshold = t4;
    }
    /* Coverity Fix 64222 End */
#endif
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
        txAMEntity_p->dcBearerType = rlcConfigParam.am.txAmRlc.dcBearerType;
#endif

}

/****************************************************************************
 * Function Name  : rlcConfigUmRxEntityParam 
 * Inputs         : rxUMEntity_p - pointer to RxUMEntity,
 *                  rlcConfigParam - pointer to RlcConfigParam,
 *                  eT - Rlc Entity Type
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to configure the rxUmEntity using requested
 *                  parameter.
 ****************************************************************************/

inline static void rlcConfigUmRxEntityParam( RxUMEntity *rxUMEntity_p, 
                                             RlcConfigParam rlcConfigParam,
                                             /* SPR 4272 Fix Start */
                                             RlcEntityType eT
                                             /* SPR 4272 Fix End */
                                           )
{
    /* changing ring size as per the snFieldLen*/
    RingSz ringSz = 0;
    UInt16 seqRange  = 0x01;
    /* SPR 4272 Fix Start */
    RxUmRlc *rxUmRlc_p = RLC_NULL;
    /* SPR 4272 Fix End */

    /* SPR 4272 Fix Start */
    if (eT == RLC_TX_RX_ENTITY)
    {
        rxUmRlc_p = &( rlcConfigParam.umBiDirectional.rxUmRlc);
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
    	rxUMEntity_p->dcBearerType = rlcConfigParam.umBiDirectional.rxUmRlc.dcBearerType; 
#endif
	}
    else
    {
        rxUmRlc_p = &( rlcConfigParam.umUniDirectionalRx.rxUmRlc);
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
    	rxUMEntity_p->dcBearerType = rlcConfigParam.umUniDirectionalRx.rxUmRlc.dcBearerType; 
#endif
    }

    rxUMEntity_p->snFieldLength = rxUmRlc_p->snFieldLength; 
    seqRange = seqRange << (rxUMEntity_p->snFieldLength );
    rxUMEntity_p->modValForWrapRound = seqRange - 1;
    /* SPR 4272 Fix End */
     
    if ( rxUMEntity_p->snFieldLength == size5 )
    {
        rxUMEntity_p->szFHdr = 1 /* ONE_BYTE */;
        rxUMEntity_p->wSize = UM_WINDOW_SN_5;
    }
    else
    {
        rxUMEntity_p->szFHdr = 2 /*TWO_BYTE*/;
        rxUMEntity_p->wSize = UM_WINDOW_SN_10;
    }

    rxUMEntity_p->umROTimerInfo.roDuration = rxUmRlc_p->tReordering;
#ifndef UE_SIM_TESTING
    /* SPR 4272 Fix Start */
    rxUMEntity_p->bitRateRx = rxUmRlc_p->bitRateRx;
    /* SPR 4272 Fix End */
#endif
    ringSz = seqRange; 

    rlcChangeSizeUmdPduR(&(rxUMEntity_p->rxUmdPduRing),ringSz);
}

/****************************************************************************
 * Function Name  : rlcConfigUmTxEntityParam
 * Inputs         : txUMEntity_p - pointer to TxUMEntity API,
 *                  rlcConfigParam - pointer to RlcConfigParam
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to configure the txUmEntity using requested
 *                  parameter.
 ****************************************************************************/

inline static void rlcConfigUmTxEntityParam( TxUMEntity *txUMEntity_p, 
                                             RlcConfigParam rlcConfigParam 
                                           )
{
    UInt8 szFHdr = 0;
    UInt16 seqRange  = 0x01;
    txUMEntity_p->snFieldLength = rlcConfigParam.umUniDirectionalRx
        .rxUmRlc.snFieldLength;

    (txUMEntity_p->snFieldLength == size5) ? (szFHdr = 1) : (szFHdr = 2);
    txUMEntity_p->szFHdr = szFHdr;
    seqRange = seqRange << (txUMEntity_p->snFieldLength );
    txUMEntity_p->modValForWrapRound = seqRange - 1;
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
    txUMEntity_p->dcBearerType = rlcConfigParam.umUniDirectionalTX.txUmRlc.dcBearerType; 
#endif
}

/****************************************************************************
 * Function Name  : rlcConfigTmRxEntityParam
 * Inputs         : *rxTMEntity_p - pointer to RxTMEntity API.
 *                  * - pointer to config request.
 * Outputs        :
 * Returns        : void
 * Variables      :
 * Description    : This API is used to configure the rxTmEntity using requested
 *                  parameter.
 ****************************************************************************/
/*static void rlcConfigTmRxEntityParam(RxTMEntity *rxUMEntity_p, 
  RlcEntityCfgInfo* rlcEntityCfgInfo)
  {
  rxUMEntity_p->comChType = rlcEntityCfgInfo->comChType;
  }*/
/****************************************************************************
 * Function Name  : rlcConfigTmTxEntityParam
 * Inputs         : *txTMEntity_p - pointer to TxTMEntity API.
 *                  * - pointer to config request.
 * Outputs        :
 * Returns        : void
 * Variables      :
 * Description    : This API is used to configure the txTmEntity using requested
 *                  parameter.
 ****************************************************************************/
/*static void rlcConfigTmTxEntityParam(TxTMEntity *txUMEntity_p, 
  RlcEntityCfgInfo* rlcEntityCfgInfo)
  {
  txUMEntity_p->comChType = rlcEntityCfgInfo->comChType;
  }*/
/* SPR 3805 Fix Start */
/****************************************************************************
 * Function Name  : rlcHdlrPenUeDelQ
 * Inputs         : rlcUePendingDelQ_p - Pointer to LTE_CIRCQUEUE
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used handle the Pending UE deletion queue.
 *                  It deletes the UE Context and send the confirmation.
 ****************************************************************************/
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
 inline UInt32 rlcHdlrPenUeDelQ(LTE_CIRCQUEUE *rlcUePendingDelQ_p)
 {
     UInt32 ueCount = GET_UE_DEL_ENTITY_Q_COUNT(*rlcUePendingDelQ_p);
     RlcUeDelQNode *RlcUeDelQNode_p = RLC_NULL;
     UInt16 resp = RLC_SUCCESS;

     /* Loop to remove the all pending delete ue request */
     while(ueCount--)
     {
        DEQUEUE_DEL_UE_ENTITY_Q(RlcUeDelQNode_p,*rlcUePendingDelQ_p);
        if (RLC_NULL == RlcUeDelQNode_p)
        {
            continue;
        }
        else
        {
            UInt16 transactionId = RlcUeDelQNode_p->transactionId;
            UInt32 ueIndex = RlcUeDelQNode_p->ueIndex;
            UEContext* ueContext_p = DB_FETCH_UE_CONTEXT(ueIndex);

            if(ueContext_p == RLC_NULL)
            {
                freeMemPool(RlcUeDelQNode_p);
                RlcUeDelQNode_p = RLC_NULL;
                continue;
            }
            if( 0 == ueContext_p->delProcessCnt )
            {
                ueContext_p->delProcessCnt++;
                LOG_RLC_MSG(LTE_RLC_DEL_UE_CONTEXT_ERR, LOGWARNING, RLC_RRC,
                        globalTTITickCount_g,__LINE__, ueIndex,
                        transactionId, RLC_FAILURE,0, 0, 0, __func__, "");
                freeMemPool(RlcUeDelQNode_p);
                RlcUeDelQNode_p = RLC_NULL;
            }
            else
            {
                UInt8 cnfMsgBuf[MAX_RES_DATASIZE];
                UInt8 *resMsgBuf_p = PNULL;
                /* SPR 4737 Fix Start */
                UInt16 cnfMsgIdx = RRC_RLC_API_HEADER_LEN;
                /* SPR 4737 Fix End */
                resMsgBuf_p = cnfMsgBuf;
#ifndef UE_SIM_TESTING
                RrcCellIndex cellId = RLC_GET_RRC_CELL_INDEX( 
                                    rlcUEContextList_g[ueIndex]->cellIndex );

                UInt16 rrcUeIndex = rlcUEContextList_g[ueIndex]->rrcUeIndex;
                LTE_SET_U16BIT((resMsgBuf_p + cnfMsgIdx),rrcUeIndex);
#else
                RrcCellIndex cellId = 0;
                LTE_SET_U16BIT((resMsgBuf_p + cnfMsgIdx),ueIndex);
#endif
                cnfMsgIdx += 2;

                resp = rlcDbDeleteUEContext(ueIndex);

                LTE_SET_U16BIT((resMsgBuf_p + cnfMsgIdx),resp);
                cnfMsgIdx += 2;

                /* preparing header for confirmation */
                prepareCnfBufferHeader(resMsgBuf_p
                        , RLC_DELETE_UE_ENTITY_CNF,transactionId, cellId, cnfMsgIdx);

                /* writting to socket */
                rlcSendMsgToRrc(cnfMsgBuf,
                        cnfMsgIdx);
                activeUEs_g--;

                LOG_RLC_MSG(LTE_RLC_DEL_UE_CONTEXT, LOGINFO, RLC_RRC, globalTTITickCount_g,
                         __LINE__, ueIndex, transactionId, 0,
                        0, 0, 0, __func__, "");
                freeMemPool(RlcUeDelQNode_p);
                RlcUeDelQNode_p = RLC_NULL;
            }
         }
     }
     return resp;
 }
#endif
/*SPR 3805 Fix End */


/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
/*****************************************************************************
 * Function Name  : putEntryInRlcRrcCnfQUsingICC
 * Inputs         : resp - response,
 *                  ueIndex - Index of UE,
 *                  respEvent - Response Event,
 *                  transactionId,
 *                  cellId - RRC Cell Index
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to send CNF from RLC DL/UL threads for 
 *                  delete/reconfig/reEsablish UE req recived from RRC
 *****************************************************************************/
void putEntryInRlcRrcCnfQUsingICC(UInt8 resp,UInt16 ueIndex, UInt8 respEvent
/* + Layer2 NON CA Changes */
, UInt16 transactionId, RrcCellIndex cellId
/* - Layer2 NON CA Changes */
)
{
    RlcRrcOamHiPrioQNode * node_p = RLC_NULL;

    node_p = (RlcRrcOamHiPrioQNode *)getMemFromPool(sizeof(RlcRrcOamHiPrioQNode),RLC_NULL);

    /* + COVERITY 5.0 */
    if(PNULL == node_p)
    {
        lteWarning("Memory Allocation fail %d",sizeof(node_p));
        return;
    }
    node_p->ueIndex =  ueIndex;
    node_p->processRrcOamMsgEvent = respEvent;
    node_p->rlcState = resp;
    node_p->transactionId = transactionId;
    node_p->cellId = cellId;

    sendMsgWithIccQ(rlcRrcCnfInputQId_g, (const char *)&node_p , sizeof(node_p), 1);
    /* Coverity_31231 Fix */
    //freeMemPool(node_p);
    /* Coverity_31231 Fix */
}

#endif


/****************************************************************************
 * Function Name  : rlcHdlrPenDelQ 
 * Inputs         : pendDelQ_p - Pointer to LTE_SQUEUE
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to handle the Pending deletion queue. 
 *                  It delete the requested entity and send the confirmation.
 ****************************************************************************/
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
inline UInt32 rlcHdlrPenDelQ(LTE_SQUEUE *pendDelQ_p)
{
    RlcDelReEstabQNode *rlcDelReEstabQNode_p = RLC_NULL; 
    UInt16 resp = RLC_FAILURE;
    UInt32 ueIndex =0;
    RlcLcId lcId = 0;
    UInt32 nCount = sQueueCount(pendDelQ_p);

    /* handle the nCount number of node of queue */
    while ( nCount--)
    {
        DEQUEUE_RLC_DEL_RE_ESTABQ(pendDelQ_p, rlcDelReEstabQNode_p);

        if(rlcDelReEstabQNode_p == RLC_NULL)
        {
            continue;
        }
        if (rlcDelReEstabQNode_p->rlcNodeType == RLC_UE_ENTITY)
        {
            ueIndex = rlcDelReEstabQNode_p->ueIndex;
            lcId = rlcDelReEstabQNode_p->lcId;
            resp = rlcDbDeleteRLCEntityInfo(ueIndex,lcId, rlcDelReEstabQNode_p->entityType);
            /** SPR 2712 Changes Start **/
            if((resp == RLC_SUCCESS) && 
                    (rlcDelReEstabQNode_p->entityType == rlcDelReEstabQNode_p->cnf))
            {
                /* intializing the lndex with 0 for cnf msg */
                /* As Response is send , when RLC parse the request. No need to send it again */
            /** SPR 2712 Changes End **/
            LOG_RLC_MSG(LTE_RLC_DEL_UE_ENTITY, LOGINFO, RLC_RRC, globalTTITickCount_g,
                    /* FDD_Warning_Fix */
                    __LINE__,ueIndex, lcId, 0,
                    /* FDD_Warning_Fix */
                    0, 0, 0, __func__, "");
            }
            else
            {
                LOG_RLC_MSG(LTE_RLC_DEL_UE_ENTITY_ERR, LOGWARNING, RLC_RRC, globalTTITickCount_g,
                     /* FDD_Warning_Fix */
                   __LINE__, ueIndex, lcId, 0,
                     /* FDD_Warning_Fix */
                    RLC_FAILURE, 0, 0, __func__, "");
                FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
                rlcDelReEstabQNode_p = RLC_NULL;

                return RLC_FAILURE;
            }
        }

        FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
        rlcDelReEstabQNode_p = RLC_NULL;

    } /* end of while */
    return RLC_SUCCESS;
}

#else

/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : rlcSetEntityState
 * Inputs         : rlcDelReEstabQNode_p - Pointer to RlcDelReEstabQNode,
 *                  lcId - Logical channel ID,
 *                  ueContext_p - Pointer to UEContext,
 *                  rlcEntityInfoList - Pointer to RLCEntityInfo,
 *                  respEvent - Pointer to response event
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to set the RLC state event 
 ****************************************************************************/
static inline void rlcSetEntityState(
        RlcDelReEstabQNode *rlcDelReEstabQNode_p,
        UInt16         lcId,
        UEContext* ueContext_p,
        RLCEntityInfo  *rlcEntityInfoList,
        UInt8 *respEvent)
{
    /** SPR 13293 Changes Start **/
    if(rlcEntityInfoList->rlcEntityMode == RLC_ENTITY_UM )
    {
        rlcStopAllRunningUmTimers(ueContext_p->rlcEntityInfoList[lcId]);

    }
    else if(rlcEntityInfoList->rlcEntityMode == RLC_ENTITY_AM)
    {
        rlcStopAllRunningAmTimers(ueContext_p->rlcEntityInfoList[lcId]);
    }
    /** SPR 13293 Changes End **/

    if(rlcDelReEstabQNode_p->entityType == RLC_TX_ENTITY)
    {
        if(rlcEntityInfoList->rlcEntityMode == RLC_ENTITY_UM )
        {
            rlcEntityInfoList->rlcEntity.umEntity_p->txEntityState = 
                            RLC_ENTITY_PENDING_DELETION;
        }
        else if( rlcEntityInfoList->rlcEntityMode == RLC_ENTITY_AM)
        {
           rlcEntityInfoList->rlcEntity.amEntity_p->txEntityState = 
                            RLC_ENTITY_PENDING_DELETION;                
        }
        *respEvent = RLC_DELETE_TX_ENTITY_EV;
    }
    else if(rlcDelReEstabQNode_p->entityType == RLC_RX_ENTITY)
    {
        if(rlcEntityInfoList->rlcEntityMode == RLC_ENTITY_UM )
        {
            rlcEntityInfoList->rlcEntity.umEntity_p->rxEntityState = 
                           RLC_ENTITY_PENDING_DELETION;
        }
        else if( rlcEntityInfoList->rlcEntityMode == RLC_ENTITY_AM)
        {
            rlcEntityInfoList->rlcEntity.amEntity_p->rxEntityState = 
                            RLC_ENTITY_PENDING_DELETION;                
        }
        *respEvent = RLC_DELETE_RX_ENTITY_EV;
    }
    /*SPR 6631 Start*/

    else if(rlcDelReEstabQNode_p->entityType == RLC_TX_RX_ENTITY)
    {
        if(ueContext_p->rlcEntityInfoList[lcId]->rlcEntityMode == RLC_ENTITY_UM )
        {
            ueContext_p->rlcEntityInfoList[lcId]->rlcEntity.umEntity_p->txEntityState = 
                RLC_ENTITY_PENDING_DELETION;
        }
        else if( ueContext_p->rlcEntityInfoList[lcId]->rlcEntityMode == RLC_ENTITY_AM)
        {
            ueContext_p->rlcEntityInfoList[lcId]->rlcEntity.amEntity_p->txEntityState = 
                RLC_ENTITY_PENDING_DELETION;            
        }


        if(ueContext_p->rlcEntityInfoList[lcId]->rlcEntityMode == RLC_ENTITY_UM )
        {
            ueContext_p->rlcEntityInfoList[lcId]->rlcEntity.umEntity_p->rxEntityState = 
                RLC_ENTITY_PENDING_DELETION;
        }
        else if( ueContext_p->rlcEntityInfoList[lcId]->rlcEntityMode == RLC_ENTITY_AM)
        {
            ueContext_p->rlcEntityInfoList[lcId]->rlcEntity.amEntity_p->rxEntityState = 
                RLC_ENTITY_PENDING_DELETION;            
        }
        *respEvent = RLC_DELETE_TX_RX_ENTITY_EV;
    }

    /*SPR 6631 End*/
}
/* Cyclomatic Complexity changes - ends here */

/****************************************************************************
 * Function Name  : rlcProcessDelMsg
 * Inputs         : rlcPendingDelQ_p - Pointer to LTE_CIRCQUEUE
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to process the Delete messages.
 ****************************************************************************/
UInt32 rlcProcessDelMsg(LTE_CIRCQUEUE *rlcPendingDelQ_p)
{
    RlcDelReEstabQNode *rlcDelReEstabQNode_p = RLC_NULL;
    UInt32 ueIndex = 0;
    UInt16 lcId = 0;
    UInt32 idx = 0;
    UInt8 respEvent = RLC_UE_CONTEXT_NOT_EXIST_EV;
    if(!circQueueCount(rlcPendingDelQ_p))
    {
        return RLC_SUCCESS;
    }
    DEQUEUE_RLC_DEL_RE_ESTABQ(rlcPendingDelQ_p, rlcDelReEstabQNode_p);
    if(rlcDelReEstabQNode_p == RLC_NULL)
    {        
        LOG_RLC_MSG(LTE_RLC_DELETE_ENTITY_ERR_1, LOGWARNING,
                RLC_RRC, globalTTITickCount_g, __LINE__,
                0, 0,0, 0, 0, 0, __func__, "Node not exist");
        return RLC_FAILURE;
    }
    ueIndex = rlcDelReEstabQNode_p->ueIndex;
    UEContext* ueContext_p = DB_FETCH_UE_CONTEXT(ueIndex);
    if(ueContext_p == RLC_NULL)
    {
        LOG_RLC_MSG(LTE_RLC_DELETE_ENTITY_ERR_2, LOGWARNING,
                RLC_RRC, globalTTITickCount_g, __LINE__,
                rlcDelReEstabQNode_p->ueIndex, 0,0, 0, 0, 0, __func__, "UE context not exist");

        /* + Layer2 NON CA Changes */
        putEntryInRlcRrcCnfQUsingICC(RLC_ENTITY_INACTIVE,ueIndex,respEvent,
                        rlcDelReEstabQNode_p->transactionId, rlcDelReEstabQNode_p->cellId);
        /* - Layer2 NON CA Changes */
        FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
        return RLC_FAILURE;
    }
    if(rlcDelReEstabQNode_p->rlcNodeType == RLC_UE_CONTEXT)
    {
        for( ;(idx < MAX_LC_ID); idx++)
        {
            if(ueContext_p->rlcEntityInfoList[idx] != RLC_NULL)
            {
                /* Cyclomatic Complexity changes - starts here */
                /** SPR 13293 Changes Start **/
                rlcSetEntityState(rlcDelReEstabQNode_p, idx, ueContext_p,
                        ueContext_p->rlcEntityInfoList[idx],&respEvent);
                /** SPR 13293 Changes End **/
            }
        }
        respEvent = RLC_DELETE_UE_CONTEXT_EV;

       /** SPR 13293 Changes Start **/
       ueContext_p->rlcUEState = RLC_UECONTEXT_PENDING_DELETION;
       /** SPR 13293 Changes End **/
        /* Cyclomatic Complexity changes - ends here */
    }
    else if(rlcDelReEstabQNode_p->rlcNodeType == RLC_UE_ENTITY)
    {
        lcId = rlcDelReEstabQNode_p->lcId;
        /* Cyclomatic Complexity changes - starts here */
        rlcSetEntityState(rlcDelReEstabQNode_p, lcId, ueContext_p,
                        ueContext_p->rlcEntityInfoList[lcId],&respEvent);
        /* Cyclomatic Complexity changes - ends here */
    }
    else
    {
        LOG_RLC_MSG(LTE_RLC_DELETE_ENTITY_ERR_3, LOGWARNING,
                RLC_RRC, globalTTITickCount_g, __LINE__,
                rlcDelReEstabQNode_p->ueIndex, 0,0, 0, 0, 0, __func__, "Invalid node type");

        /* + Layer2 NON CA Changes */
        putEntryInRlcRrcCnfQUsingICC(RLC_ENTITY_INACTIVE,ueIndex,respEvent,
                        rlcDelReEstabQNode_p->transactionId, rlcDelReEstabQNode_p->cellId);
        /* - Layer2 NON CA Changes */

        return RLC_FAILURE;
    }
    /* Send Confirmation to RRC Thread */   
    /* + Layer2 NON CA Changes */
    putEntryInRlcRrcCnfQUsingICC(RLC_ENTITY_PENDING_DELETION,ueIndex,respEvent,
                        rlcDelReEstabQNode_p->transactionId, rlcDelReEstabQNode_p->cellId);
    /* - Layer2 NON CA Changes */
    FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
    LOG_RLC_MSG(LTE_RLC_DELETE_ENTITY, LOGINFO,RLC_RRC, globalTTITickCount_g, 
            __LINE__,ueIndex, 0,0, 0, 0, 0, __func__, "");
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : rlcProcessRxReconfigMsg
 * Inputs         : rlcReconfigRxEntityQ_p - Pointer to LTE_CIRCQUEUE
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE with error message
 * Description    : This API is used to process the reconfiguration messages 
 ****************************************************************************/
UInt32 rlcProcessRxReconfigMsg(LTE_CIRCQUEUE *rlcReconfigRxEntityQ_p)
{
    RlcRxEntityCfgInfo *rlcRxEntityCfg_p = RLC_NULL;
    UInt8 respEvent = RLC_UE_CONTEXT_NOT_EXIST_EV;

    DEQUEUE_RLC_RECONFIG_ENTITY_Q(rlcReconfigRxEntityQ_p, rlcRxEntityCfg_p);
    rlcRxEntityCfg_p = (RlcRxEntityCfgInfo *)rlcRxEntityCfg_p;
    if(rlcRxEntityCfg_p != RLC_NULL)
    {
        UInt16 ueIndex = rlcRxEntityCfg_p->ueIndex;
        UInt16 lcId = rlcRxEntityCfg_p->lcId;
        RlcEntityMode entityMode = rlcRxEntityCfg_p->entityMode;
        UEContext* outUEContext_p = DB_FETCH_UE_CONTEXT(ueIndex);

        if ( RLC_NULL == outUEContext_p )
        {
            LOG_RLC_MSG(LTE_RLC_HANDLE_RECONFIG_ERR_4, LOGWARNING,
                    RLC_RRC, globalTTITickCount_g, __LINE__,
                    ueIndex, 0, 0, 0, 0, 0, __func__, "RLC_UE_NOT_EXISTS");
            /* Send confirmation to RRC thread */
            /* + Layer2 NON CA Changes */
            putEntryInRlcRrcCnfQUsingICC(RLC_FAILURE,ueIndex,respEvent,
                        rlcRxEntityCfg_p->transactionId, rlcRxEntityCfg_p->cellId);
            /* - Layer2 NON CA Changes */
            freeMemPool(rlcRxEntityCfg_p);
            rlcRxEntityCfg_p = RLC_NULL;
            return RLC_UE_NOT_EXISTS;
        }
        if ( RLC_NULL == outUEContext_p->rlcEntityInfoList[lcId])
        {
            LOG_RLC_MSG(LTE_RLC_HANDLE_RECONFIG_ERR_5, LOGWARNING,
                    RLC_RRC, globalTTITickCount_g, __LINE__,
                    ueIndex,lcId, 0, 0, 0, 0, __func__, "RLC_LCID_NOT_EXISTS");
            /* Send confirmation to RRC thread */
            /* + Layer2 NON CA Changes */
            putEntryInRlcRrcCnfQUsingICC(RLC_FAILURE,ueIndex,respEvent,
                        rlcRxEntityCfg_p->transactionId, rlcRxEntityCfg_p->cellId);
            /* - Layer2 NON CA Changes */
            freeMemPool(rlcRxEntityCfg_p);
            rlcRxEntityCfg_p = RLC_NULL;
            return RLC_LCID_NOT_EXISTS;
        }        
        if(entityMode ==  RLC_ENTITY_UM)
        {

            respEvent = RLC_RECONFIG_RX_ENTITY_EV;
            UMEntity *umEntity_p = outUEContext_p->rlcEntityInfoList[lcId]->rlcEntity.umEntity_p;
            if(umEntity_p->rxEntityState == RLC_ENTITY_PENDING_DELETION)
            {
                LOG_RLC_MSG(LTE_RLC_HANDLE_RECONFIG_ERR_6, LOGWARNING,
                        RLC_RRC, globalTTITickCount_g, __LINE__,
                        ueIndex, lcId, 0, 0, 0, 0, __func__, "RLC_ENTITY_IN_PENDING_DEL_Q");
                /* Send confirmation to RRC thread */
                /* + Layer2 NON CA Changes */
                putEntryInRlcRrcCnfQUsingICC(RLC_FAILURE,ueIndex,respEvent,
                        rlcRxEntityCfg_p->transactionId, rlcRxEntityCfg_p->cellId);
                /* - Layer2 NON CA Changes */
                freeMemPool(rlcRxEntityCfg_p);
                rlcRxEntityCfg_p = RLC_NULL;
                return RLC_ENTITY_IN_PENDING_DEL_Q;
            }
            if(!umEntity_p->rxUMEntity_p)
            {
                LOG_RLC_MSG(LTE_RLC_HANDLE_RECONFIG_ERR_7, LOGWARNING,
                        RLC_RRC, globalTTITickCount_g, __LINE__,
                        ueIndex, lcId, 0, 0, 0, 0, __func__, "RLC_UEENTITY_NOT_EXIST");
                /* Send confirmation to RRC thread */
                /* + Layer2 NON CA Changes */
                putEntryInRlcRrcCnfQUsingICC(RLC_FAILURE,ueIndex,respEvent,
                        rlcRxEntityCfg_p->transactionId, rlcRxEntityCfg_p->cellId);
                /* - Layer2 NON CA Changes */
                freeMemPool(rlcRxEntityCfg_p);
                rlcRxEntityCfg_p = RLC_NULL;
                return RLC_UEENTITY_NOT_EXIST; 
            }

            umEntity_p->rxUMEntity_p->umROTimerInfo.roDuration = rlcRxEntityCfg_p->rxRlc.rxUmRlc.tReordering;
            if(rlcRxEntityCfg_p->rlcQciInfo.qci)
            {
                umEntity_p->rxUMEntity_p->qci = rlcRxEntityCfg_p->rlcQciInfo.qci;
                umEntity_p->rxUMEntity_p->bitRateRx = rlcRxEntityCfg_p->rxRlc.rxUmRlc.bitRateRx;
            }
            /* + Layer2 NON CA Changes */
            putEntryInRlcRrcCnfQUsingICC(RLC_SUCCESS,ueIndex,respEvent,
                        rlcRxEntityCfg_p->transactionId, rlcRxEntityCfg_p->cellId);
            /* - Layer2 NON CA Changes */
        }
        else if(entityMode == RLC_ENTITY_AM)
        {
            respEvent = RLC_RECONFIG_RX_ENTITY_EV;
            AMEntity *amEntity_p = outUEContext_p->rlcEntityInfoList[lcId]->rlcEntity.amEntity_p;
            if(amEntity_p->rxEntityState == RLC_ENTITY_PENDING_DELETION)
            {
                LOG_RLC_MSG(LTE_RLC_HANDLE_RECONFIG_ERR_6, LOGWARNING,
                        RLC_RRC, globalTTITickCount_g, __LINE__,
                        ueIndex, lcId, 0, 0, 0, 0, __func__, "RLC_ENTITY_IN_PENDING_DEL_Q");
                /* Send confirmation to RRC thread */
                /* + Layer2 NON CA Changes */
                putEntryInRlcRrcCnfQUsingICC(RLC_FAILURE,ueIndex,respEvent,
                        rlcRxEntityCfg_p->transactionId, rlcRxEntityCfg_p->cellId);
                /* - Layer2 NON CA Changes */
                freeMemPool(rlcRxEntityCfg_p);
                rlcRxEntityCfg_p = RLC_NULL;
                return RLC_ENTITY_IN_PENDING_DEL_Q; 
            }
            if(!amEntity_p->rxAMEntity_p)
            {
                LOG_RLC_MSG(LTE_RLC_HANDLE_RECONFIG_ERR_7, LOGWARNING,
                        RLC_RRC, globalTTITickCount_g, __LINE__,
                        ueIndex, lcId, 0, 0, 0, 0, __func__, "RLC_UEENTITY_NOT_EXIST");
                /* Send confirmation to RRC thread */
                /* + Layer2 NON CA Changes */
                putEntryInRlcRrcCnfQUsingICC(RLC_FAILURE,ueIndex,respEvent,
                        rlcRxEntityCfg_p->transactionId, rlcRxEntityCfg_p->cellId);
                /* - Layer2 NON CA Changes */
                freeMemPool(rlcRxEntityCfg_p);
                rlcRxEntityCfg_p = RLC_NULL;
                return RLC_UEENTITY_NOT_EXIST; 
            }

            amEntity_p->rxAMEntity_p->amROTimerInfo.roDuration = rlcRxEntityCfg_p->rxRlc.rxAmRlc.tReordering;
            amEntity_p->rxAMEntity_p->spTimerInfo.spDuration = rlcRxEntityCfg_p->rxRlc.rxAmRlc.tStatusProhibit;

            if(rlcRxEntityCfg_p->rlcQciInfo.qci)
            {
                amEntity_p->rxAMEntity_p->qci = rlcRxEntityCfg_p->rlcQciInfo.qci;
                amEntity_p->rxAMEntity_p->bitRateRx = rlcRxEntityCfg_p->rxRlc.rxAmRlc.bitRateRx;
            }
            /* + Layer2 NON CA Changes */
            putEntryInRlcRrcCnfQUsingICC(RLC_SUCCESS,ueIndex,respEvent,
                        rlcRxEntityCfg_p->transactionId, rlcRxEntityCfg_p->cellId);
            /* - Layer2 NON CA Changes */
        }
        else
        {
            LOG_RLC_MSG(LTE_RLC_HANDLE_RECONFIG_ERR_8, LOGWARNING,
                    RLC_RRC, globalTTITickCount_g, __LINE__,
                    ueIndex, 0, 0, 0, 0, 0, __func__, "Invalid Entity Mode");
        }
        freeMemPool(rlcRxEntityCfg_p);
        rlcRxEntityCfg_p = RLC_NULL;
    }
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : rlcProcessTxReconfigMsg
 * Inputs         : rlcReconfigTxEntityQ_p - Pointer to LTE_CIRCQUEUE
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE with error message
 * Description    : This API is used to process the reconfiguration messages 
 ****************************************************************************/
UInt32 rlcProcessTxReconfigMsg(LTE_CIRCQUEUE *rlcReconfigTxEntityQ_p)
{
    RlcTxEntityCfgInfo *rlcTxEntityCfg_p = RLC_NULL;

    UInt8 respEvent = RLC_UE_CONTEXT_NOT_EXIST_EV;

    DEQUEUE_RLC_RECONFIG_ENTITY_Q(rlcReconfigTxEntityQ_p, rlcTxEntityCfg_p);
    rlcTxEntityCfg_p = (RlcTxEntityCfgInfo *)rlcTxEntityCfg_p;

    if(rlcTxEntityCfg_p != RLC_NULL)
    {
        UInt16 ueIndex = rlcTxEntityCfg_p->ueIndex;
        UInt16 lcId = rlcTxEntityCfg_p->lcId;
        RlcEntityMode entityMode = rlcTxEntityCfg_p->entityMode;
        UEContext* outUEContext_p = DB_FETCH_UE_CONTEXT(ueIndex);

        if ( RLC_NULL == outUEContext_p )
        {
            LOG_RLC_MSG(LTE_RLC_HANDLE_RECONFIG_ERR_4, LOGWARNING,
                    RLC_RRC, globalTTITickCount_g, __LINE__,
                    ueIndex, 0, 0, 0, 0, 0, __func__, "RLC_UE_NOT_EXISTS");
            /* Send confirmation to RRC thread */

            /* + Layer2 NON CA Changes */
            putEntryInRlcRrcCnfQUsingICC(RLC_FAILURE,ueIndex,respEvent, 
                                     rlcTxEntityCfg_p->transactionId, rlcTxEntityCfg_p->cellId);
            /* - Layer2 NON CA Changes */

            freeMemPool(rlcTxEntityCfg_p);
            rlcTxEntityCfg_p = RLC_NULL;
            return RLC_UE_NOT_EXISTS;
        }
        if ( RLC_NULL == outUEContext_p->rlcEntityInfoList[lcId])
        {
            LOG_RLC_MSG(LTE_RLC_HANDLE_RECONFIG_ERR_5, LOGWARNING,
                    RLC_RRC, globalTTITickCount_g, __LINE__,
                    ueIndex, 0, 0, 0, 0, 0, __func__, "RLC_LCID_NOT_EXISTS");
            /* Send confirmation to RRC thread */
            /* + Layer2 NON CA Changes */
            putEntryInRlcRrcCnfQUsingICC(RLC_FAILURE,ueIndex,respEvent, 
                                     rlcTxEntityCfg_p->transactionId, rlcTxEntityCfg_p->cellId);
            /* - Layer2 NON CA Changes */

            freeMemPool(rlcTxEntityCfg_p);
            rlcTxEntityCfg_p = RLC_NULL;
            return RLC_LCID_NOT_EXISTS;
        }        

        if(entityMode ==  RLC_ENTITY_UM)
        {
            respEvent = RLC_RECONFIG_TX_ENTITY_EV;
            UMEntity *umEntity_p = outUEContext_p->rlcEntityInfoList[lcId]->rlcEntity.umEntity_p;
            if(umEntity_p->txEntityState == RLC_ENTITY_PENDING_DELETION)
            {
                LOG_RLC_MSG(LTE_RLC_HANDLE_RECONFIG_ERR_6, LOGWARNING,
                        RLC_RRC, globalTTITickCount_g, __LINE__,
                        ueIndex, 0, 0, 0, 0, 0, __func__, "RLC_ENTITY_IN_PENDING_DEL_Q");
                /* Send confirmation to RRC thread */
                /* + Layer2 NON CA Changes */
                putEntryInRlcRrcCnfQUsingICC(RLC_FAILURE,ueIndex,respEvent, 
                                     rlcTxEntityCfg_p->transactionId, rlcTxEntityCfg_p->cellId);
                /* - Layer2 NON CA Changes */

                freeMemPool(rlcTxEntityCfg_p);
                rlcTxEntityCfg_p = RLC_NULL;
                return RLC_ENTITY_IN_PENDING_DEL_Q;
            }
            if(!umEntity_p->txUMEntity_p)
            {
                LOG_RLC_MSG(LTE_RLC_HANDLE_RECONFIG_ERR_7, LOGWARNING,
                        RLC_RRC, globalTTITickCount_g, __LINE__,
                        ueIndex, 0, 0, 0, 0, 0, __func__, "RLC_UEENTITY_NOT_EXIST");
                /* Send confirmation to RRC thread */

                /* + Layer2 NON CA Changes */
                putEntryInRlcRrcCnfQUsingICC(RLC_FAILURE,ueIndex,respEvent, 
                                     rlcTxEntityCfg_p->transactionId, rlcTxEntityCfg_p->cellId);
                /* - Layer2 NON CA Changes */

                freeMemPool(rlcTxEntityCfg_p);
                rlcTxEntityCfg_p = RLC_NULL;
                return RLC_UEENTITY_NOT_EXIST;
            }
            if(rlcTxEntityCfg_p->rlcQciInfo.qci)
            {
                umEntity_p->txUMEntity_p->qci = rlcTxEntityCfg_p->rlcQciInfo.qci;
                umEntity_p->txUMEntity_p->bitRateTx = rlcTxEntityCfg_p->txRlc.txUmRlc.bitRateTx;
                umEntity_p->txUMEntity_p->rlcQueueSize = rlcTxEntityCfg_p->txRlc.txUmRlc.rlcQueueSize;
            }
            /* Send confirmation to RRC thread */
            /* + Layer2 NON CA Changes */
            putEntryInRlcRrcCnfQUsingICC(RLC_SUCCESS,ueIndex,respEvent, 
                                     rlcTxEntityCfg_p->transactionId, rlcTxEntityCfg_p->cellId);
            /* - Layer2 NON CA Changes */
        }
        else if(entityMode == RLC_ENTITY_AM)
        {
            respEvent = RLC_RECONFIG_TX_ENTITY_EV;
            AMEntity *amEntity_p = outUEContext_p->rlcEntityInfoList[lcId]->rlcEntity.amEntity_p;
            if(amEntity_p->txEntityState == RLC_ENTITY_PENDING_DELETION)
            {
                LOG_RLC_MSG(LTE_RLC_HANDLE_RECONFIG_ERR_6, LOGWARNING,
                        RLC_RRC, globalTTITickCount_g, __LINE__,
                        ueIndex, 0, 0, 0, 0, 0, __func__, "RLC_ENTITY_IN_PENDING_DEL_Q");
                /* Send confirmation to RRC thread */
                /* + Layer2 NON CA Changes */
                putEntryInRlcRrcCnfQUsingICC(RLC_FAILURE,ueIndex,respEvent, 
                                     rlcTxEntityCfg_p->transactionId, rlcTxEntityCfg_p->cellId);
                /* - Layer2 NON CA Changes */

                freeMemPool(rlcTxEntityCfg_p);
                rlcTxEntityCfg_p = RLC_NULL;
                return RLC_ENTITY_IN_PENDING_DEL_Q;
            }
            if(!amEntity_p->txAMEntity_p)
            {
                LOG_RLC_MSG(LTE_RLC_HANDLE_RECONFIG_ERR_7, LOGWARNING,
                        RLC_RRC, globalTTITickCount_g, __LINE__,
                        ueIndex, 0, 0, 0, 0, 0, __func__, "RLC_UEENTITY_NOT_EXIST");
                /* Send confirmation to RRC thread */
                /* + Layer2 NON CA Changes */
                putEntryInRlcRrcCnfQUsingICC(RLC_FAILURE,ueIndex,respEvent, 
                                     rlcTxEntityCfg_p->transactionId, rlcTxEntityCfg_p->cellId);
                /* - Layer2 NON CA Changes */

                freeMemPool(rlcTxEntityCfg_p);
                rlcTxEntityCfg_p = RLC_NULL;
                return RLC_UEENTITY_NOT_EXIST;
            }
            
            amEntity_p->txAMEntity_p->prTimerInfo.prDuration = rlcTxEntityCfg_p->txRlc.txAmRlc.tPollRetransmit;
            amEntity_p->txAMEntity_p->pollPDU = rlcTxEntityCfg_p->txRlc.txAmRlc.pollPDU;
            amEntity_p->txAMEntity_p->pollByte = rlcTxEntityCfg_p->txRlc.txAmRlc.pollByte;
            amEntity_p->txAMEntity_p->maxRetxThreshold = rlcTxEntityCfg_p->txRlc.txAmRlc.maxRetxThreshold;
            
            if(rlcTxEntityCfg_p->rlcQciInfo.qci)
            {
                amEntity_p->txAMEntity_p->qci = rlcTxEntityCfg_p->rlcQciInfo.qci;
                amEntity_p->txAMEntity_p->bitRateTx = rlcTxEntityCfg_p->txRlc.txAmRlc.bitRateTx;
                amEntity_p->txAMEntity_p->rlcQueueSize = rlcTxEntityCfg_p->txRlc.txAmRlc.rlcQueueSize;
            }
            /* Send confirmation to RRC thread */
            /* + Layer2 NON CA Changes */
            putEntryInRlcRrcCnfQUsingICC(RLC_SUCCESS,ueIndex,respEvent, 
                                     rlcTxEntityCfg_p->transactionId, rlcTxEntityCfg_p->cellId);
            /* - Layer2 NON CA Changes */
        }
        else
        {
            LOG_RLC_MSG(LTE_RLC_HANDLE_RECONFIG_ERR_8, LOGWARNING,
                    RLC_RRC, globalTTITickCount_g, __LINE__,
                    ueIndex, 0, 0, 0, 0, 0, __func__, "Invalid Entity Mode");
        }
        freeMemPool(rlcTxEntityCfg_p);
        rlcTxEntityCfg_p = RLC_NULL;
    }
    return RLC_SUCCESS;
}

#endif

/****************************************************************************
 * Function Name  : rlcHdlrReEstabQ 
 * Inputs         : reEstabQ_p - Pointer to LTE_SQUEUE
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE with error message
 * Description    : This API is used handle the reEstablishment Queue. 
 ****************************************************************************/
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
UInt32 rlcHdlrReEstabQ(LTE_SQUEUE *reEstabQ_p)
{
    RlcDelReEstabQNode *rlcDelReEstabQNode_p = RLC_NULL;
    /** SPR 3015 changes Start **/
    /** SPR 3015 changes End **/
    /*As per SPR 698 change start*/
    UInt32 ueIndex =0;
    RlcLcId lcId = 0;
    UInt64 nCount = sQueueCount(reEstabQ_p);
    while(nCount)
    {
            /* SPR 16164 fix start */
            DEQUEUE_RLC_DEL_RE_ESTABQ(reEstabQ_p, rlcDelReEstabQNode_p);
            if(RLC_NULL == rlcDelReEstabQNode_p){
                nCount--;
                continue;
            }    
            UEContext* ueContext_p 
                = DB_FETCH_UE_CONTEXT(rlcDelReEstabQNode_p->ueIndex);
            if(ueContext_p == RLC_NULL)
            {
                nCount --;
                /* SPR 4253-4333 Fix Start */
                FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
                rlcDelReEstabQNode_p = RLC_NULL;
                /* SPR 4253-4333 Fix End */
                continue;
            }
            /* SPR 16164 fix end */
#ifndef UE_SIM_TESTING
#if defined OVERSHOOT_CORRECTION && !defined DL_UL_SPLIT
        UInt64 rem_cycles = 0;
        GET_PROCESSING_CYCLES_LEFT(rem_cycles);
        if ( ( !noTimeToProcessflag_g ) && ( rem_cycles >= PROCESS_NODE_TIME_REQD ) )
        {
#endif
#endif
            /* SPR 16164 fix start */
            /*** source lines moved above ***/
            /* SPR 16164 fix end */

            /* SPR 4253-4333 Fix Start */
            if (ueContext_p->delProcessCnt)
            {
                //ueContext_p->delProcessCnt ++;
                nCount --;
                FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
                rlcDelReEstabQNode_p = RLC_NULL;
                continue;
            }
            /* SPR 4253-4333 Fix End */
            if(rlcDelReEstabQNode_p->rlcNodeType == RLC_UE_ENTITY)
            {
                ueIndex = rlcDelReEstabQNode_p->ueIndex;
                lcId = rlcDelReEstabQNode_p->lcId;
                /*RLC Rel 2.0 Changes Start*/
                reEstablishUeEntity(ueIndex, lcId, rlcDelReEstabQNode_p->entityType);

                if(rlcDelReEstabQNode_p->entityType == rlcDelReEstabQNode_p->cnf)
                {
                    RLCEntityInfo *rlcEntityInfo_p 
                        = ueContext_p->rlcEntityInfoList[lcId];
                    if(rlcEntityInfo_p)
                    {
                        /* SPR 4994 Change Start */
                        /*rlcEntityInfo_p->rlcEntityState = RLC_ENTITY_ACTIVE;*/
                        rlcEntityInfo_p->rlcEntityState =  
                                               rlcDelReEstabQNode_p->rlcState;
                        /* SPR 4994 Change End */
                    }
                    /** SPR 3015 Changes Start **/
                    /** SPR 3015 Changes End **/         
                    /* SPR 4253-4333 Fix Start */
                    // ueContext_p->delProcessCnt = 0;
                    /* SPR 4253-4333 Fix End */
                    LOG_RLC_MSG(LTE_RLC_REST_UE_ENTITY_ERR_3, LOGINFO, RLC_RRC, globalTTITickCount_g,
                            /* FDD_Warning_Fix */
                            __LINE__, ueIndex, lcId, 0,
                            /* FDD_Warning_Fix */
                            0, 0, 0, __func__, "");
                }    
                /*RLC Rel 2.0 Changes End*/
            }

            FREE_RLC_DEL_RE_ESTABQNODE(rlcDelReEstabQNode_p);
            rlcDelReEstabQNode_p = RLC_NULL;
            nCount--;
#ifndef UE_SIM_TESTING
#if defined OVERSHOOT_CORRECTION && !defined DL_UL_SPLIT
        }
        else
        {
            /* SPR 16164 fix start */
            InternalCellIndex internalCellIndex = ueContext_p->cellIndex;

            noTimeToProcessflag_g = 1;

            if(currTickOverLoad_g[internalCellIndex] < OVERLOAD_LEVEL_MEDIUM)
            {
                currTickOverLoad_g[internalCellIndex] = OVERLOAD_LEVEL_MEDIUM;
            }
            LOG_RLC_MSG( LTE_RLC_TX_LD_CHANGE, LOGWARNING, RLC_TX, 
                    /** CA-TDD Changes Start **/
                    globalTTITickCount_g, __LINE__,currTickOverLoad_g[internalCellIndex], macOverLoadLevel_g[internalCellIndex],
                    /*+ Dyn UEs To Be SchPerTTI +*/
                    dlUeSchdIndxInCurrTick_g, ulUeSchdIndxInCurrTick_g, 0, 0, __func__, "");
                    /** CA-TDD Changes Stop **/
                    /*+ Dyn UEs To Be SchPerTTI +*/
            /* SPR 16164 fix end */
            return RLC_SUCCESS;
        }
#endif
#endif
    } /* end of while */
    return RLC_SUCCESS;
}

#else
/* SPR 21778 Fix */
/****************************************************************************
 * Function Name  : resetQLoadAndState 
 * Inputs         : ueIndex - UeId to reset QlOad
 *                  lcid - LCID of the reestablished UE
 * Outputs        : None
 * Returns        : NONe 
 * Description    : This function is used to set the QLoad and delay the
 *  process of reestablishment to avoid * Race conditons of DL and UL
 ****************************************************************************/
void resetQLoad(UInt16 ueIndex,UInt16 lcid)
{
    LCQueueLoad lcQLoadArr_p[MAX_LC_ID];
    UInt8 idx =0;
    UInt8 lcCount =1;
    memSet(&lcQLoadArr_p,0,sizeof(LCQueueLoad));
 
    lcQLoadArr_p[idx].lcId = lcid; 
    lcQLoadArr_p[idx].lcQueueLoad = 0;

    lcQLoadArr_p[idx].scheduledDataBytes = 0;
    lcQLoadArr_p[idx].headPktTS = 0;
    lcQLoadArr_p[idx].retxnPduSize = 0;
    lcQLoadArr_p[idx].ctrlQueueLoad = 0;
    lcQLoadArr_p[idx].ctrlFlag = RLC_NORMAL_DATA;

    macUpdateBufferOccupancyReq( ueIndex,
            lcCount,lcQLoadArr_p,
            RLC_BO_TYPE_NEW,
            rlcUEContextList_g[ueIndex]->cellIndex
            );
}
/* SPR 21778 Fix */
/****************************************************************************
 * Function Name  : rlcProcessReEstabMsg
 * Inputs         : reEstabQ_p - Pointer to LTE_CIRCQUEUE
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE with error message
 * Description    : This API is used process the reEstablishment Message. 
 ****************************************************************************/
UInt32 rlcProcessReEstabMsg(LTE_CIRCQUEUE *reEstabQ_p)
{
    RlcDelReEstabQNode *rlcDelReEstabQNode_p = RLC_NULL;

    UInt8 respEvent = RLC_UE_CONTEXT_NOT_EXIST_EV;

    RlcLcId lcId = 0;
    UInt16 ueIndex = 0;
    DEQUEUE_RLC_DEL_RE_ESTABQ(reEstabQ_p,rlcDelReEstabQNode_p);
    if(RLC_NULL == rlcDelReEstabQNode_p)
    {
        /* SPR 4390 change start */
        LOG_UT(LTE_RLC_HANDLE_RE_ESTABLISH_ERR_2, LOGWARNING,
                RLC_RRC, globalTTITickCount_g, __LINE__,
                0, 0, 0, 0, 0, 0, __func__, "Node not found");
        /* SPR 4390 change end */
        return RLC_FAILURE;
    }

    ueIndex = rlcDelReEstabQNode_p->ueIndex;
    UEContext* ueContext_p = DB_FETCH_UE_CONTEXT(rlcDelReEstabQNode_p->ueIndex);
	/* SPR#11313 Fix changes	*/	
	if (( ueContext_p == RLC_NULL)
			|| ( ueContext_p->rlcUEState != RLC_UECONTEXT_ACTIVE ))
	/* SPR#11313 Fix changes	*/
    {
        LOG_RLC_MSG(LTE_RLC_HANDLE_RE_ESTABLISH_ERR_3, LOGWARNING,
                RLC_RRC, globalTTITickCount_g, __LINE__,
                rlcDelReEstabQNode_p->ueIndex, 0, 0, 0, 0, 0, __func__, "RLC_UE_NOT_EXISTS");
        /* Send confirmation to RRC thread */
        /* + Layer2 NON CA Changes */
        putEntryInRlcRrcCnfQUsingICC(RLC_FAILURE,ueIndex,respEvent, 
                     rlcDelReEstabQNode_p->transactionId, rlcDelReEstabQNode_p->cellId);
        /* - Layer2 NON CA Changes */
        freeMemPool(rlcDelReEstabQNode_p);
        rlcDelReEstabQNode_p = RLC_NULL;
        return RLC_UE_NOT_EXISTS;
    }
    /* SPR 21778 Fix */
    if(!rlcDelReEstabQNode_p->reEstablishProcessTick)
    {
        rlcDelReEstabQNode_p->reEstablishProcessTick = globalTTITickCount_g + REESTABLISH_PROCESS_DELAY;
        resetQLoad(rlcDelReEstabQNode_p->ueIndex,rlcDelReEstabQNode_p->lcId);
        /*Coverity 114738 Start*/
	/* SPR 22340 Fix Start */
#ifndef UE_SIM_TESTING
        if(ENQUEUE_RLC_DEL_RE_ESTABQ(reEstabQ_p,rlcDelReEstabQNode_p))
#else
        if(!ENQUEUE_RLC_DEL_RE_ESTABQ(reEstabQ_p,rlcDelReEstabQNode_p))
#endif
		/* SPR 22340 Fix End */
        {  
            return RLC_SUCCESS;
        }
        /*Coverity 114738 End*/
    }
    if(rlcDelReEstabQNode_p->reEstablishProcessTick >= globalTTITickCount_g)
    {
        /*Coverity 114738 Start*/
	/* SPR 22340 Fix Start */
#ifndef UE_SIM_TESTING
        if(ENQUEUE_RLC_DEL_RE_ESTABQ(reEstabQ_p,rlcDelReEstabQNode_p))
#else
        if(!ENQUEUE_RLC_DEL_RE_ESTABQ(reEstabQ_p,rlcDelReEstabQNode_p))
#endif
		/* SPR 22340 Fix End */
        {
            return RLC_SUCCESS;
        }
        /*Coverity 114738 End*/
    }
    /* SPR 21778 Fix */
    lcId = rlcDelReEstabQNode_p->lcId;
    RLCEntityInfo *rlcEntityInfo_p = ueContext_p->rlcEntityInfoList[lcId];
    respEvent = RLC_REESTAB_ENTITY_EV;
    if ( RLC_NULL == rlcEntityInfo_p)
    {
        LOG_RLC_MSG(LTE_RLC_HANDLE_RE_ESTABLISH_ERR_4, LOGWARNING,
                RLC_RRC, globalTTITickCount_g, __LINE__,
                rlcDelReEstabQNode_p->ueIndex, 0, 0, 0, 0, 0, __func__, "RLC_LCID_NOT_EXISTS");
        /* Send confirmation to RRC thread */
        /* + Layer2 NON CA Changes */
        putEntryInRlcRrcCnfQUsingICC(RLC_FAILURE,ueIndex,respEvent, 
                     rlcDelReEstabQNode_p->transactionId, rlcDelReEstabQNode_p->cellId);
        /* - Layer2 NON CA Changes */
        freeMemPool(rlcDelReEstabQNode_p);
        rlcDelReEstabQNode_p = RLC_NULL;
        return RLC_LCID_NOT_EXISTS;
    }
    if(rlcDelReEstabQNode_p->entityType == RLC_TX_ENTITY)
    {
        if(rlcEntityInfo_p->rlcEntityMode == RLC_ENTITY_UM)
        {
            reEstablishTxUMEntity(rlcEntityInfo_p); 
            /* SPR 4949 Change Start */               
            rlcEntityInfo_p->rlcEntity.umEntity_p->txEntityState = 
                rlcDelReEstabQNode_p->rlcState;
            /* SPR 4949 Change End */               
            //put success entry in RRC msgQ                    
            /* + Layer2 NON CA Changes */
            putEntryInRlcRrcCnfQUsingICC(RLC_SUCCESS,ueIndex,respEvent, 
                     rlcDelReEstabQNode_p->transactionId, rlcDelReEstabQNode_p->cellId);
            /* - Layer2 NON CA Changes */

        }
        else if(rlcEntityInfo_p->rlcEntityMode == RLC_ENTITY_AM)
        {
            reEstablishTxAMEntity(rlcEntityInfo_p);
            /* SPR 4949 Change Start */               
            /* SPR 5330 Fix Start */
            rlcEntityInfo_p->rlcEntity.amEntity_p->txEntityState =
                                    rlcDelReEstabQNode_p->rlcState;
            /* SPR 5330 Fix End */
            /* SPR 4949 Change End */               
            //put success entry in RRC msgQ
            /* + Layer2 NON CA Changes */
            putEntryInRlcRrcCnfQUsingICC(RLC_SUCCESS,ueIndex,respEvent, 
                     rlcDelReEstabQNode_p->transactionId, rlcDelReEstabQNode_p->cellId);
            /* - Layer2 NON CA Changes */
        }
    }
    else if(rlcDelReEstabQNode_p->entityType == RLC_RX_ENTITY)
    {
        if(rlcEntityInfo_p->rlcEntityMode == RLC_ENTITY_UM)
        {
            reEstablishRxUMEntity(rlcEntityInfo_p);                
            /* SPR 4949 Change Start */               
            /* SPR 5330 Fix Start */
            rlcEntityInfo_p->rlcEntity.umEntity_p->rxEntityState =
                                    rlcDelReEstabQNode_p->rlcState;
            /* SPR 5330 Fix End */
            /* SPR 4949 Change End */               
            //put success entry in RRC msgQ                    
            /* + Layer2 NON CA Changes */
            putEntryInRlcRrcCnfQUsingICC(RLC_SUCCESS,ueIndex,respEvent, 
                     rlcDelReEstabQNode_p->transactionId, rlcDelReEstabQNode_p->cellId);
            /* - Layer2 NON CA Changes */
        }
        else if(rlcEntityInfo_p->rlcEntityMode == RLC_ENTITY_AM)
        {
            reEstablishRxAMEntity(rlcEntityInfo_p);
            /* SPR 4949 Change Start */               
            /* SPR 5330 Fix Start */
            rlcEntityInfo_p->rlcEntity.amEntity_p->rxEntityState =
                                    rlcDelReEstabQNode_p->rlcState;
            /* SPR 5330 Fix End */
            /* SPR 4949 Change End */               
            //put success entry in RRC msgQ
            /* + Layer2 NON CA Changes */
            putEntryInRlcRrcCnfQUsingICC(RLC_SUCCESS,ueIndex,respEvent, 
                     rlcDelReEstabQNode_p->transactionId, rlcDelReEstabQNode_p->cellId);
            /* - Layer2 NON CA Changes */
        }
    }
    freeMemPool(rlcDelReEstabQNode_p);
    rlcDelReEstabQNode_p = RLC_NULL;
    return RLC_SUCCESS;
}
#endif

#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : rlcConfigSpsInfo
 * Inputs         : ueindex - Index of UE,
 *                  rlcConfigReq_p - Pointer to RlcConfigReq
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE with error message
 * Description    : This API is used to configure SPS information in the UE
 *                  and for each eligible LC.
 ****************************************************************************/
UInt32 rlcConfigSpsInfo( UInt16 ueIndex, RlcConfigReq *rlcConfigReq_p )
{
    UInt32     retVal  = RLC_SUCCESS;
    UEContext* ueCtx_p = RLC_NULL;

    do
    {
        ueCtx_p = DB_FETCH_UE_CONTEXT( ueIndex );
        if ( RLC_NULL == ueCtx_p )
        {
            LOG_RLC_MSG( LTE_RLC_CREATE_UE_CTX_ERR, LOGWARNING, RLC_RRC, 
                    globalTTITickCount_g, __LINE__, ueIndex, 0, 0, 
                    RLC_UE_NOT_EXISTS, 0,0, __func__, "");
                
            retVal = RLC_UE_NOT_EXISTS;
            break;
        }
     
        /* Check if DL SPS config request is received */
        if( 0x01 == (rlcConfigReq_p->spsBitmask & 0x01) )
        {
            if( LTE_TRUE == rlcConfigReq_p->rlcUeSpsInfo.dlRequestType && 
                LTE_TRUE == ueCtx_p->dlSpsReqType )
            {
                LOG_RLC_MSG( LTE_RLC_MULTIPLE_SPS_SETUP_NOT_VALID, LOGWARNING, 
                       RLC_RRC, globalTTITickCount_g, __LINE__, ueIndex,
                       rlcConfigReq_p->rlcUeSpsInfo.dlRequestType, 
                       ueCtx_p->dlSpsReqType, 0, 0,0, __func__,"");
                /* Ignore the request as setup is alreday received */
                break;
            }
        }
        
        /* Check if UL SPS config request is received */
        if( 0x02 == (rlcConfigReq_p->spsBitmask & 0x02) )
        {
            if( LTE_TRUE == rlcConfigReq_p->rlcUeSpsInfo.ulRequestType && 
                LTE_TRUE == ueCtx_p->ulSpsReqType )
            {
                LOG_RLC_MSG( LTE_RLC_MULTIPLE_SPS_SETUP_NOT_VALID, LOGWARNING, 
                       RLC_RRC, globalTTITickCount_g, __LINE__, ueIndex,
                       rlcConfigReq_p->rlcUeSpsInfo.ulRequestType, 
                       ueCtx_p->ulSpsReqType, 0, 0,0, __func__,"");
                /* Ignore the request as setup is alreday received */
                break;
            }
        }
        
        if( 0x01 == (rlcConfigReq_p->spsBitmask & 0x01) )
        {
            ueCtx_p->dlSpsReqType = rlcConfigReq_p->rlcUeSpsInfo.dlRequestType;
            ueCtx_p->semiPersistentSchedIntervalDl = 
                rlcConfigReq_p->rlcUeSpsInfo.semiPersistentSchedIntervalDl;
            /* +- SPR 17777 */
            rlcEntitySetsSpsLc(ueCtx_p, RLC_TX_ENTITY );
            /* +- SPR 17777 */
        }
        else if(ueCtx_p->dlSpsReqType)
        {
            /* +- SPR 17777 */
            rlcEntitySetsSpsLc(ueCtx_p, RLC_TX_ENTITY );
            /* +- SPR 17777 */
        }
        
        if( 0x02 == (rlcConfigReq_p->spsBitmask & 0x02) )
        {
            ueCtx_p->ulSpsReqType = rlcConfigReq_p->rlcUeSpsInfo.ulRequestType;
            ueCtx_p->semiPersistentSchedIntervalUl = 
                rlcConfigReq_p->rlcUeSpsInfo.semiPersistentSchedIntervalUl;
            /* +- SPR 17777 */
            rlcEntitySetsSpsLc(ueCtx_p, RLC_RX_ENTITY );
            /* +- SPR 17777 */
        }
        else if(ueCtx_p->ulSpsReqType)
        {
            /* +- SPR 17777 */
            rlcEntitySetsSpsLc(ueCtx_p, RLC_RX_ENTITY );
            /* +- SPR 17777 */
        }
    } while( 0 );
    
    return retVal;
}

/****************************************************************************
 * Function Name  : rlcEntitySetsSpsLc
 * Inputs         : ueIndex - UE ID,
 *                  ueCtx_p - Pointer to UE context,
 *                  rbDirection - Direction
 * Outputs        : None
 * Returns        : None
 * Description    : It sets or resets Logical channel is for SPS or not 
 ****************************************************************************/
/* +- SPR 17777 */
STATIC void rlcEntitySetsSpsLc( UEContext *ueCtx_p,
        UInt16 rbDirection )
/* +- SPR 17777 */
{
    UInt8           lcId          = 0;
    RLCEntityInfo   *entityInfo_p = PNULL;
    /* Rel 5.3: Coverity 32383 Fix Start */
    RlcEntityMode   eM            = RLC_ENTITY_NONE;
    /* Rel 5.3: Coverity 32383 Fix End */
    UInt8           qci           = 0;
    TxUMEntity*     txUMEntity_p  = PNULL; 
    RxUMEntity*     rxUMEntity_p  = PNULL;
    /* + Layer2 NON CA Changes */
    InternalCellIndex cellIndex = INVALID_CELL_INDEX;
    /* - Layer2 NON CA Changes */
    
    for( lcId = MIN_DRB_LC_ID; lcId < MAX_LC_ID; lcId++ )
    {
        entityInfo_p = ueCtx_p->rlcEntityInfoList[lcId];
        if ( RLC_NULL == entityInfo_p )
        {
            continue;
        }
        eM = entityInfo_p->rlcEntityMode;
        
        switch( eM )
        {
            case RLC_ENTITY_UM:
            {
                switch( rbDirection )
                {
                    case RLC_TX_ENTITY:
                    {
                        txUMEntity_p = entityInfo_p->rlcEntity.umEntity_p->\
                                                                 txUMEntity_p;
                        if( PNULL == txUMEntity_p )
                        {
                            continue;
                        }
                        qci = txUMEntity_p->qci;

                        /* Sets if its SPS LC if SPS is enabled by OAM and QCI
                         * to SPS mapping list has enabled value for SPS for 
                         * the QCI */
                        /* + Layer2 NON CA Changes */
                        cellIndex = ueCtx_p->cellIndex;
                        if ( (LTE_TRUE == ueCtx_p->dlSpsReqType) &&
                             (LTE_TRUE == secondaryCellInfo_g[cellIndex].qciSpslist[qci-1] ) )
                        /* - Layer2 NON CA Changes */
                        {
                            txUMEntity_p->isSpsLc = LTE_TRUE;
                        }
                        else
                        {
                            txUMEntity_p->isSpsLc = LTE_FALSE;
                        }
                        break;
                    }
                    case RLC_RX_ENTITY:
                    { 
                        rxUMEntity_p = entityInfo_p->rlcEntity.umEntity_p->\
                                                                 rxUMEntity_p;
                        if( PNULL == rxUMEntity_p )
                        {
                            continue;
                        }
                        qci = rxUMEntity_p->qci;
                        /* Sets if its SPS LC if SPS is enabled by OAM and QCI
                         * to SPS mapping list has enabled value for SPS for 
                         * the QCI */
                        /* + Layer2 NON CA Changes */
                        cellIndex = ueCtx_p->cellIndex;
                        if ( (LTE_TRUE == ueCtx_p->ulSpsReqType) &&
                             (LTE_TRUE == secondaryCellInfo_g[cellIndex].qciSpslist[qci-1] ) )
                        /* - Layer2 NON CA Changes */
                        {
                            rxUMEntity_p->isSpsLc = LTE_TRUE;
                        }
                        else
                        {
                            rxUMEntity_p->isSpsLc = LTE_FALSE;
                        }
                        break;
                    }
                    default:
                    {
                        RLC_LOG(LOG_WARNING,"Invalid Entity Type");
                    }
                }
            }
            default:
            {
                 RLC_LOG(LOG_WARNING,"Invalid Entity Type");
            }
        }   
    }
}
#endif

#ifdef MAC_AUT_TEST
/****************************************************************************
 * Function Name  : local_rlcConfigEntityParam
 * Inputs         : outRLCEntityInfo_p  - Pointer to RLCEntityInfo,
 *                  RlcEntityCfgInfo - Pointer to rlcEntityCfgInfo_p,
 *                  ueIndex - Index of UE
 * Outputs        : None
 * Returns        : None
 * Description    : This function used for automation to check RLC configuration 
 ****************************************************************************/
void local_rlcConfigEntityParam ( RLCEntityInfo *outRLCEntityInfo_p,
                                    RlcEntityCfgInfo* rlcEntityCfgInfo_p,
                                                                UInt16 ueIndex)
{
        rlcConfigEntityParam(outRLCEntityInfo_p, rlcEntityCfgInfo_p, ueIndex);
}

/****************************************************************************
 * Function Name  : local_rlcConfigAmRxEntityParam
 * Inputs         : rxAMEntity_p  - Pointer to RxAMEntity,
 *                  rlcConfigParam  - Pointer to RlcConfigParam
 * Outputs        : None
 * Returns        : None
 * Description    : This function used for automation to check RLC configuration 
 ****************************************************************************/
void local_rlcConfigAmRxEntityParam ( RxAMEntity *rxAMEntity_p,
                                              RlcConfigParam rlcConfigParam)
{
        rlcConfigAmRxEntityParam(rxAMEntity_p, rlcConfigParam);
}

/****************************************************************************
 * Function Name  : local_rlcConfigAmTxEntityParam
 * Inputs         : txAMEntity_p  - Pointer to TxAMEntity,
 *                  rlcConfigParam  - Pointer to RlcConfigParam
 * Outputs        : None
 * Returns        : None
 * Description    : This function used for automation to check RLC configuration 
 ****************************************************************************/
void local_rlcConfigAmTxEntityParam ( TxAMEntity *txAMEntity_p,
                                              RlcConfigParam rlcConfigParam)
{
        rlcConfigAmTxEntityParam(txAMEntity_p, rlcConfigParam);
}

/****************************************************************************
 * Function Name  : local_rlcConfigUmRxEntityParam
 * Inputs         : rxUMEntity_p  - Pointer to RxUMEntity,
 *                  rlcConfigParam  - Pointer to RlcConfigParam,
 *                  eT - RLC Entity Type
 * Outputs        : None
 * Returns        : None
 * Description    : This function used for automation to check RLC configuration 
 ****************************************************************************/
void local_rlcConfigUmRxEntityParam ( RxUMEntity *rxUMEntity_p,
                                              RlcConfigParam rlcConfigParam,
                                                                                    RlcEntityType eT)
{
        rlcConfigUmRxEntityParam(rxUMEntity_p, rlcConfigParam, eT);
}

/****************************************************************************
 * Function Name  : local_rlcConfigUmTxEntityParam
 * Inputs         : txUMEntity_p  - Pointer to TxUMEntity,
 *                  rlcConfigParam  - Pointer to RlcConfigParam
 * Outputs        : None
 * Returns        : None
 * Description    : This function used for automation to check RLC configuration 
 ****************************************************************************/
void local_rlcConfigUmTxEntityParam ( TxUMEntity *txUMEntity_p,
                                              RlcConfigParam rlcConfigParam)
{
        rlcConfigUmTxEntityParam(txUMEntity_p, rlcConfigParam);
}

/****************************************************************************
 * Function Name  : local_validateEntityTypeAndCtx
 * Inputs         : rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo,
 *                  outUEContext_p - Pointer to UEContext
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE 
 * Description    : This function used for automation to validate entry type 
 *                  and RLC context 
 ****************************************************************************/
int local_validateEntityTypeAndCtx (RlcEntityCfgInfo* rlcEntityCfgInfo_p, UEContext* outUEContext_p)
{
        return validateEntityTypeAndCtx(rlcEntityCfgInfo_p, outUEContext_p);
}

/****************************************************************************
 * Function Name  : local_reEstablishTxUMEntity
 * Inputs         : rlcEntityInfo_p  - Pointer to RLCEntityInfo
 * Outputs        : None
 * Returns        : None
 * Description    : This function used for automation to check RLC 
 *                  Reestablishment procedure 
 ****************************************************************************/
void local_reEstablishTxUMEntity(RLCEntityInfo *rlcEntityInfo_p)
{
    reEstablishTxUMEntity(rlcEntityInfo_p);
}

/****************************************************************************
 * Function Name  : local_reEstablishRxUMEntity
 * Inputs         : rlcEntityInfo_p  - Pointer to RLCEntityInfo
 * Outputs        : None
 * Returns        : None
 * Description    : This function used for automation to check RLC 
 *                  Reestablishment procedure 
 ****************************************************************************/
void local_reEstablishRxUMEntity(RLCEntityInfo *rlcEntityInfo_p)
{
    reEstablishRxUMEntity(rlcEntityInfo_p);
}

/****************************************************************************
 * Function Name  : local_reEstablishTxRxUMEntity
 * Inputs         : rlcEntityInfo_p  - Pointer to RLCEntityInfo
 * Outputs        : None
 * Returns        : None
 * Description    : This function used for automation to check RLC 
 *                  Reestablishment procedure 
 ****************************************************************************/
void local_reEstablishTxRxUMEntity(RLCEntityInfo *rlcEntityInfo_p)
{
    reEstablishTxRxUMEntity(rlcEntityInfo_p);
}

/****************************************************************************
 * Function Name  : local_reEstablishUMEntity
 * Inputs         : entityType - RlcEntityType,
 *                  rlcEntityInfo_p  - Pointer to RLCEntityInfo
 * Outputs        : None
 * Returns        : None
 * Description    : This function used for automation to check RLC 
 *                  Reestablishment procedure 
 ****************************************************************************/
void local_reEstablishUMEntity(RlcEntityType entityType,
                               RLCEntityInfo *rlcEntityInfo_p)
{
    reEstablishUMEntity(entityType, rlcEntityInfo_p);
}

/****************************************************************************
 * Function Name  : local_reEstablishAMEntity
 * Inputs         : rlcEntityInfo_p  - Pointer to RLCEntityInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE 
 * Description    : This function used for automation to check RLC 
 *                  Reestablishment procedure 
 ****************************************************************************/
UInt32 local_reEstablishAMEntity(RLCEntityInfo *rlcEntityInfo_p)
{
    return reEstablishAMEntity(rlcEntityInfo_p);
}

/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : local_reEstablishRxAMEntity
 * Inputs         : rlcEntityInfo_p  - Pointer to RLCEntityInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE 
 * Description    : This function used for automation to check RLC 
 *                  Reestablishment procedure 
 ****************************************************************************/
UInt32 local_reEstablishRxAMEntity(RLCEntityInfo *rlcEntityInfo_p)
{
    return reEstablishRxAMEntity(rlcEntityInfo_p);
}
#endif

#endif

