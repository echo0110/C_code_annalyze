/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpTxQueues.c,v 1.1.1.1.12.2.2.2 2010/10/07 13:24:18 gur23054 Exp $
 *
 ******************************************************************************
 *
 *  File Description : PDCP TX queues implementation. 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpTxQueues.c,v $
 * Revision 1.1.1.1.12.2.2.2  2010/10/07 13:24:18  gur23054
 * Fix for SPR 713
 *
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:18  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:11:47  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.7.2.4  2009/07/20 08:21:56  gur19479
 * Changes done for SRB handling
 *
 * Revision 1.7.2.3  2009/07/13 11:05:24  gur20435
 *
 * PDCP Optimizations
 *
 * Revision 1.7.2.2  2009/07/08 08:42:41  gur19836
 * Tx flow Optimization changes
 *
 * Revision 1.7.2.1  2009/06/09 11:33:51  gur19140
 * *** empty log message ***
 *
 * Revision 1.7  2009/05/22 09:20:52  gur19836
 * ROHC Changes merged
 *
 * Revision 1.6  2009/05/21 07:10:46  gur19140
 * memory leak fixes
 *
 * Revision 1.5  2009/05/20 13:36:24  gur20548
 * header file name changed
 *
 * Revision 1.4  2009/05/15 18:43:24  gur19140
 * fixes
 *
 * Revision 1.3  2009/05/15 07:08:14  gur19836
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

#include "ltePdcpTxQueues.h"
#include "ltePdcpDiscardTimer.h"
#include "ltePdcpErrors.h"
#include "ltePdcpContext.h"
#include "ltePdcpRrcSap.h"
#include "logging.h"
#include "alarm.h"


/******************************************************************************
  Private Definitions
 *****************************************************************************/


/*wrapper macroses definitions for PDCP Control PDU TX queue*/  
#define QINIT_PDCP_TX_CONTROL_PDU_IND_Q()\
    sQueueInit((LTE_SQUEUE*) &pdcpTxControlPduIndQ_g )

#define QCOUNT_PDCP_TX_CONTROL_PDU_IND_Q()\
    sQueueCount((LTE_SQUEUE*) &pdcpTxControlPduIndQ_g )

#define ENQUEUE_PDCP_TX_CONTROL_PDU_IND_Q(txControlPduInd_p)\
    pushNode((LTE_SQUEUE*) &pdcpTxControlPduIndQ_g, &(txControlPduInd_p)->sNode ) 

#define DEQUEUE_PDCP_TX_CONTROL_PDU_IND_Q(txControlPduInd_p){\
    txControlPduInd_p = (PdcpControlPduInd *)popNode((LTE_SQUEUE*) &pdcpTxControlPduIndQ_g );\
}
/* SPR 3608 changes start*/
#define GET_FIRST_PDCP_TX_CONTROL_PDU(txControlPduInd_p){\
    txControlPduInd_p = (PdcpControlPduInd *)getFirstNode((LTE_SQUEUE*) &pdcpTxControlPduIndQ_g);\
}

#define GET_NEXT_PDCP_TX_CONTROL_PDU(txControlPduInd_p){\
    txControlPduInd_p = (PdcpControlPduInd *)getNextNode((LTE_SQUEUE*) &pdcpTxControlPduIndQ_g);\
}


/* SPR 3608 changes end */
/* Start 128 UE: Memory Changes */
/*
 ** This is same as Macro "RLC_PDCP_NUM_DATA_REQ" defined
 ** in lteRlcOamInterface.c. Any change in "RLC_PDCP_NUM_DATA_REQ",
 ** would require change in PDCP_RLC_NUM_DATA_REQ.
 */
#define PDCP_RLC_NUM_DATA_REQ 600
/* End 128 UE: Memory Changes */

/* Memory handling changes start */
#define MAX_SRB_DATA_REQ     10
/* Memory handling changes end */

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static inline void pdcpFreeTxQueues (LP_PdcpEnityContext ctx_p);
static inline void pdcpFreeSrbTxQueues (LP_PdcpEnityContext ctx_p);

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* This queue will be data interface from PDCP user to the PDCP layer. */
volatile LTE_SQUEUE pdcpTxPendingDeletionQ_g;
/* This queue will be data interface from the RLC layer to the PDCP layer 
   for Delivery status indication reception.
 */
/** CA 2 Core Changes Start **/
#ifdef UE_SIM_TESTING
volatile LTE_CIRCQUEUE pdcpTxDeliveryIndQ_g;
#else
volatile LTE_CIRCQUEUE pdcpTxDeliveryIndQ_g[MAX_NUM_CELL];
#endif
/** CA 2 Core Changes End **/

extern PdcpContext pdcpContext;

#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
/*this queue is definbed for dataInd from RLC to PDCP */ 
volatile LTE_CIRCQUEUE pdcpRlcDataIndQ_g; 
volatile LTE_CIRCQUEUE rlcDeliveryStatusIndQ_g; 
#endif

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
/* This queue is data interface between RX and TX data path for PDCP layer 
   control PDU transferring.
 */
volatile LTE_SQUEUE pdcpTxControlPduIndQ_g;

/* SPR 15909 fix start */
extern tickType_t pdcpCurrentTime_g;
/* SPR 15909 fix end */

/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : txQueuesCreateMemPool 
 * Inputs         : maxUeNumber - Maximum number of supported UEs
 *                  txReservationCoef - Number of blocks for TX purposes
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code.
 * Description    : Reserve memory in pools for PDCP TX queuing purposes.
 ****************************************************************************/
 /* +- SPR 17777 */
UInt16 txQueuesCreateMemPool(
)
    /* +- SPR 17777 */
{
    /* Start 128 UE: Memory Changes */
    createMemPool( sizeof(PdcpDataReq), ((pdcpNumOfSupportedUsers_g *
                    pdcpNumOfSupportedLC_g * MAX_PDCP_SDU_QUEUE_SIZE * 
                    pdcpDlMemFac_g)/100));
    createMemPool( sizeof(PdcpControlPduInd), PDCP_RLC_NUM_DATA_REQ );
    createMemPool( sizeof(PdcpDeliveryInd), PDCP_RLC_NUM_DATA_REQ);
    /* End 128 UE: Memory Changes */
    /* Memory handling changes start */
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
    createMemPool( sizeof(RlcDeliveryStatusInd), PDCP_RLC_NUM_DATA_REQ);
    createMemPool( sizeof(PdcpRlcDataInd), ((pdcpNumOfSupportedUsers_g *
                    pdcpNumOfSupportedLC_g * MAX_PDCP_SDU_QUEUE_SIZE * 
                    pdcpUlMemFac_g)/100));
#endif    
    createMemPool( sizeof(PdcpStatusBuffer), PDCP_RLC_NUM_DATA_REQ);
    createMemPool( sizeof(PdcpSrbDataReq), MAX_SRB_DATA_REQ);
    createMemPool((sizeof(TlvHeader) + sizeof(RrcSrbDataInd) + PDCP_TS_SDU_MAX_SIZE), 1);
    /* Memory handling changes end */
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : txQueuesInit 
 * Inputs         :     
 * Outputs        : None.
 * Returns        : None.
 * Description    : It inits the set of PDCP TX queues which are used for 
 *                  data processing on TX path.
 ****************************************************************************/ 
/* + SPR 17439 */
void txQueuesInit(void)
/* - SPR 17439 */    
{
    QINIT_PDCP_TX_PENDING_DELETION_Q();
    /** CA 2 Core Changes Start **/
#ifndef UE_SIM_TESTING
    InternalCellIndex  index = 0;
    for(; index <  pdcpContext.sCellCount;index++)
    {
        QINIT_PDCP_TX_DELIVERY_IND_Q(index);
    }
#else
    QINIT_PDCP_TX_DELIVERY_IND_Q();
#endif

    /** CA 2 Core Changes End **/
    QINIT_PDCP_TX_CONTROL_PDU_IND_Q();
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
    QINIT_PDCP_RLC_DATA_IND_Q();
    QINIT_RLC_DELIVERY_STATUS_IND_Q();
#endif
}

/****************************************************************************
 * Function Name  : txQueuesCleanup 
 * Inputs         :     
 * Outputs        : None.
 * Returns        : None.
 * Description    : It cleanup the set of PDCP TX queues which are used 
 *                  for data processing on TX path.
 ****************************************************************************/ 
/* + SPR 17439 */
void txQueuesCleanup(void)
/* - SPR 17439 */    
{
    LP_PdcpDeliveryInd deliveryInd_p = PNULL;
    LP_PdcpControlPduInd controlPduInd_p = PNULL;
    LP_PdcpTxPendingDeletionReq delReq_p = PNULL;

    LP_PdcpUeContext    ueCtx_p = PNULL;
    LP_PdcpEnityContext ctx_p   = PNULL;
    UInt16              ueIndex = 0;
    UInt8               lcId    = 0;
    /** CA 2 Core Changes Start **/
#if !defined(UE_SIM_TESTING)
    InternalCellIndex   cellIndex = 0;
#endif
    /** CA 2 Core Changes End **/
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
    PdcpRlcDataInd * dataInd_p = PNULL;
    RlcDeliveryStatusInd *statusInd_p = PNULL;
#endif
    /* cleanup PDCP DRB TX queue */
    while ( ueIndex < pdcpContext.numOfActiveUe)
    {
        /*
         ** activeUeInfo consists only of active UEs, hence valid UE context
         ** will always be available. Therefore, no need to check for NULL check
         */
        /* SPR 5485 changes start */
        ueCtx_p = PDCP_GET_VALID_UE_CONTEXT( 
                        pdcpContext.activeUeInfo[ueIndex] );
        /* SPR 5485 changes end */
        /* Clears nodes of SRB, if any*/
        for (lcId = LTE_SRB_LC_ID_LOW_BOUND; lcId <= LTE_SRB_LC_ID_HIGH_BOUND;
                lcId++)
        {           
            /* SPR 5485 changes start */
            ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p, lcId, 
                    PDCP_ENTITY_RB_TX );
            /* SPR 5485 changes end */
            if (PNULL == ctx_p )
            {
                LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_TX,
                        pdcpCurrentTime_g, __LINE__, 
                        pdcpContext.activeUeInfo[ueIndex],
                        lcId, 0,0, 0,0,__func__,"");
            }
            else
            {
                pdcpFreeSrbTxQueues( ctx_p );
            }
        }
        lcId = 0;
        /* Clears nodes of DRB, if any*/
        while ( lcId < pdcpContext.ueContexts[ueIndex]->numOfActiveDrbLc )
        {
            /*
             ** activeLcInfo consists only of active DRBs in an UE, hence
             ** valid PDCP context will always be available. Therefore, 
             ** no need to check for NULL check
             */
            /* SPR 5485 changes start */
            ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p, 
                    ueCtx_p->activeLcInfo[lcId], 
                    PDCP_ENTITY_RB_TX);
            /* SPR 5485 changes end */
            pdcpFreeTxQueues (ctx_p);
            lcId++;
        }
        ueIndex++;
    }

    /* cleanup PDCP Delivery indication TX queue */
    /** CA 2 Core Changes Start **/
#if !defined UE_SIM_TESTING
    for(cellIndex = 0;cellIndex < pdcpContext.sCellCount ;cellIndex++)
    {
#endif
        do 
        {
#if !defined UE_SIM_TESTING
            DEQUEUE_PDCP_TX_DELIVERY_IND_Q(deliveryInd_p,cellIndex);
#else
            DEQUEUE_PDCP_TX_DELIVERY_IND_Q(deliveryInd_p);
#endif

            if ( PNULL != deliveryInd_p )
            {
                /*VALGRIND FIX STARTS*/
                freeMemPool ( deliveryInd_p->deliveryStatusArr );
                deliveryInd_p->deliveryStatusArr = PNULL;
                /*VALGRIND FIX ENDS*/
                freeMemPool( deliveryInd_p );
                deliveryInd_p  = PNULL;
            }
        } while ( PNULL != deliveryInd_p );
#if defined UE_SIM_TESTING
        /*++530-coverity-54621*/
        QDEINIT_PDCP_TX_DELIVERY_IND_Q();
        /*++530-coverity-54621*/
#else
        QDEINIT_PDCP_TX_DELIVERY_IND_Q(cellIndex);
    }
#endif
    /** CA 2 Core Changes End **/


    /* cleanup PDCP Control PDU TX queue */
    do 
    {
        DEQUEUE_PDCP_TX_CONTROL_PDU_IND_Q(controlPduInd_p);

        if ( PNULL != controlPduInd_p )
        {
            pdcpFreeControlPduInd( controlPduInd_p );
        }
    } while ( PNULL != controlPduInd_p );
    do
    {
        DEQUEUE_PDCP_TX_PENDING_DELETION_Q(delReq_p);
        if ( PNULL != delReq_p ) {
            freeMemPool( delReq_p );
            delReq_p = PNULL;
        }

    } while(delReq_p);
    /* clean up RLC_PDCP Data Ind Queue*/
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
    do
    {
        DEQUEUE_PDCP_RLC_DATA_IND_Q(dataInd_p);
        if ( PNULL != dataInd_p ) {
            freeMemPool( dataInd_p );
            dataInd_p = PNULL;
        }

    } while(dataInd_p);

    do
    {
        DEQUEUE_RLC_DELIVERY_STATUS_IND_Q(statusInd_p);
        if ( PNULL != statusInd_p ) {
            freeMemPool( statusInd_p );
            statusInd_p = PNULL;
        }

    } while(statusInd_p);
    QDEINIT_RLC_DELIVERY_STATUS_IND_Q();

#endif
}

/****************************************************************************
 * Function Name  : pdcpFreeSrbTxQueues
 * Inputs         : ctx_p - Pointer to PDCP entity context
 * Outputs        : None.
 * Returns        : None.
 * Description    : It cleanups the PDCP SRB TX queues
 ****************************************************************************/
static inline void pdcpFreeSrbTxQueues (LP_PdcpEnityContext ctx_p)
{
    LP_PdcpSrbDataReq txSrbDataReq_p = PNULL;

    /* Free SRB data Request queue */
    do 
    {
        DEQUEUE_PDCP_SRB_TX_DATA_REQ_Q(ctx_p, txSrbDataReq_p);
        if ( PNULL != txSrbDataReq_p )
        {
            pdcpFreeSrbTxDataReq(txSrbDataReq_p);
        }
    } while ( PNULL != txSrbDataReq_p );

    /* Free data request storage queue for TX*/
    do
    {
        DEQUEUE_PDCP_SRB_TX_STORAGE_Q(ctx_p, txSrbDataReq_p);
        if ( PNULL != txSrbDataReq_p )
        {
            pdcpFreeSrbTxDataReq(txSrbDataReq_p);
        }
    } while ( PNULL != txSrbDataReq_p );
}

/****************************************************************************
 * Function Name  : pdcpFreeTxQueues
 * Inputs         : ctx_p - Pointer to PDCP entity context
 * Outputs        : None.
 * Returns        : None.
 * Description    : It cleanups the PDCP DRB TX queues
 ****************************************************************************/
static inline void pdcpFreeTxQueues (LP_PdcpEnityContext ctx_p)
{
    LP_PdcpDataReq dataReq_p = PNULL;
	
	do
	{
		DEQUEUE_PDCP_TX_DATA_REQ_Q(ctx_p, dataReq_p);
		if ( PNULL != dataReq_p )
		{
			pdcpFreeTxDataReq(dataReq_p);
		}
	} while ( PNULL != dataReq_p );
	do
	{
		DEQUEUE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
		if ( PNULL != dataReq_p )
		{
			pdcpFreeTxDataReq(dataReq_p);
		}
	} while ( PNULL != dataReq_p );
}

/****************************************************************************
 * Function Name  : pdcpFreeTxDataReq 
 * Inputs         : LP_PdcpDataReq *dataReq_p   
 * Outputs        : None.
 * Returns        : None.
 * Description    : To free LP_PdcpDataReq
 ****************************************************************************/ 
void pdcpFreeTxDataReq(LP_PdcpDataReq dataReq_p)
{
    if(PNULL == dataReq_p) 
    {
        LOG_PDCP_MSG (PDCP_DATA_POINTER_NULL, LOGFATAL, PDCP_TX, pdcpCurrentTime_g,
                __LINE__, 0,0,0,0, 0,0, __func__,"DRB dataReq_p is NULL" );
    } 
    else 
    {
        if(PNULL != dataReq_p->data_p) {
            msgFree(dataReq_p->data_p);
            dataReq_p->data_p = PNULL;
        }
        freeMemPool(dataReq_p);
    }
}

/****************************************************************************
 * Function Name  : pdcpFreeSrbTxDataReq 
 * Inputs         : LP_PdcpDataReq *dataReq_p   
 * Outputs        : None.
 * Returns        : None.
 * Description    : To free LP_PdcpSrbDataReq
 ****************************************************************************/ 
void pdcpFreeSrbTxDataReq(LP_PdcpSrbDataReq dataReq_p)
{
    if(PNULL == dataReq_p) 
    {
        LOG_PDCP_MSG (PDCP_DATA_POINTER_NULL, LOGFATAL, PDCP_TX, pdcpCurrentTime_g,
                __LINE__, 0,0,0,0, 0,0, __func__,"SRB dataReq_p is NULL" );
    } 
    else 
    {
        if(PNULL != dataReq_p->data_p) {
            msgFree(dataReq_p->data_p);
            dataReq_p->data_p = PNULL;
        }

        freeMemPool(dataReq_p);
    }
}

/****************************************************************************
 * Function Name  : pdcpAllocateSrbTxDataReq 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  transactionId - Transaction Id
 *                  sn -
 *                  size -
 *                  serviceReqested -
 * Outputs        : None.
 * Returns        : LP_PdcpSrbDataReq pointer on TX data request structure or NULL.
 * Description    : To allocate and init LP_PdcpDataReq
 ****************************************************************************/ 
LP_PdcpSrbDataReq pdcpAllocateSrbTxDataReq( UInt16 ueIndex, UInt16 lcId, 
        UInt32 transactionId, UInt16 sn,UInt16 size, UInt8 serviceReqested )
{
    LP_PdcpSrbDataReq dataReq_p = PNULL;

    /* Allocate data request structure */
    dataReq_p = (LP_PdcpSrbDataReq)getMemFromPool( sizeof(PdcpSrbDataReq), PNULL);

    /*As per SPR 713, ltePanic take care of the returning value, no change done*/
    if ( PNULL == dataReq_p  )
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_TX, 
                pdcpCurrentTime_g,__LINE__, sizeof(PdcpDataReq), ueIndex, lcId,
                0, 0,0, __func__,"dataReq_p is NULL");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s",__FUNCTION__);
        return PNULL;
    }    

    /* Init dataReq_p. */
    dataReq_p->commDataReq.ueIndex = ueIndex;
    dataReq_p->commDataReq.lcId = lcId;
    dataReq_p->data_p = PNULL;
    dataReq_p->tickAssociated = 0;
    dataReq_p->transactionId = transactionId;
    dataReq_p->commDataReq.count = 0;
    dataReq_p->commDataReq.sn = sn;
    dataReq_p->commDataReq.size = size;
    /* SPR 3406 changes start */
#ifdef PDCP_ASYNC_INTERFACE
    dataReq_p->commDataReq.isPacketWithSecEngine = LTE_FALSE;
#endif
    /* SPR 3406 changes end */
    dataReq_p->srbServiceReqested = serviceReqested;

    return dataReq_p;
}

/****************************************************************************
 * Function Name  : pdcpAllocateTxDataReq 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  sn -
 *                  size -
 * Outputs        : None.
 * Returns        : LP_PdcpDataReq pointer on TX data request structure or NULL.
 * Description    : To allocate and init LP_PdcpDataReq
 ****************************************************************************/ 
LP_PdcpDataReq pdcpAllocateTxDataReq( UInt16 ueIndex, UInt16 lcId, 
        UInt16 sn,UInt16 size )
{
    LP_PdcpDataReq dataReq_p = PNULL;

    /* Allocate data request structure */
    dataReq_p = (LP_PdcpDataReq)getMemFromPool( sizeof(PdcpDataReq), PNULL);

    /*As per SPR 713, ltePanic take care of the returning value, no change done*/
    if ( PNULL == dataReq_p  )
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_TX, 
                pdcpCurrentTime_g,__LINE__, sizeof(PdcpDataReq), ueIndex, lcId,
                0, 0,0, __func__,"dataReq_p is NULL");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("getMemFromPool fails %s",__FUNCTION__);
        return PNULL;
    }    

    /* Init dataReq_p. */
    dataReq_p->commDataReq.ueIndex = ueIndex;
    dataReq_p->commDataReq.lcId = lcId;
    dataReq_p->data_p = PNULL;
    dataReq_p->tickAssociated = 0;
    dataReq_p->commDataReq.count = 0;
    dataReq_p->commDataReq.sn = sn;
    dataReq_p->commDataReq.size = size;
    /* SPR 3406 changes start */
#ifdef PDCP_ASYNC_INTERFACE
    dataReq_p->commDataReq.isPacketWithSecEngine = LTE_FALSE;
#endif
    /* SPR 3406 changes end */
    /* SPR 20570 Fix Start */
    dataReq_p->commDataReq.voiceSilenceType = PDCP_VOICE_PKT;
    /* SPR 20570 Fix End */

    return dataReq_p;
}

/****************************************************************************
 * Function Name  : pdcpStoreControlPduInd 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  type - control PDU type (PDCP_TX_CONTROL_PDU, 
 *                         PDCP_RX_CONTROL_PDU)
 *                  controlPdu_p - pointer on Control PDU ZCB buffer
 *                  ctrl_type -
 * Outputs        : None.
 * Returns        : LP_PdcpControlPduInd pointer on PDCP Control PDU 
 *                  indication structure or NULL.
 * Description    : It allocates and stores PdcpControlPduInd structure in
 *                  PDCP Control PDU TX queue.
 ****************************************************************************/ 
/* SRP 3608 changes start */
LP_PdcpControlPduInd pdcpStoreControlPduInd( UInt16 ueIndex, UInt16 lcId, 
        UInt16 type, void*  controlPdu_p, UInt8 ctrl_type)
{
    LP_PdcpControlPduInd controlPduInd_p = PNULL;

    /* Allocate data request structure */
    controlPduInd_p = 
        (LP_PdcpControlPduInd)getMemFromPool(sizeof(PdcpControlPduInd),PNULL);
    if ( PNULL == controlPduInd_p  )
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, (PDCP_TX|PDCP_RX), 
                pdcpCurrentTime_g, __LINE__, sizeof(PdcpControlPduInd), ueIndex,
                lcId, 0, 0,0, __func__, "controlPduInd_p is NULL");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        /* Coverity 25198 Fix Start*/
        msgFree(controlPdu_p);
        controlPdu_p = PNULL;
        /* Coverity 25198 Fix End*/

        return PNULL;
    }    

    /* Init dataReq_p. */
    controlPduInd_p->type = type;
    controlPduInd_p->ueIndex = ueIndex;
    controlPduInd_p->lcId = lcId;
    controlPduInd_p->data_p = controlPdu_p;
    controlPduInd_p->ctrl_type = ctrl_type;
    /* put data to PDCP Control PDU TX queue */

    /* coverity_25463 Fix Start */

    if(ENQUEUE_PDCP_TX_CONTROL_PDU_IND_Q(controlPduInd_p))
    {
        /* Coverity 25198 Fix Start*/
        msgFree(controlPdu_p);
        controlPdu_p = PNULL;
 	controlPduInd_p->data_p = PNULL;
        /* Coverity 25198 Fix End*/
        freeMemPool(controlPduInd_p);
        return PNULL;
    }

    /* coverity_25463 Fix End */

    return controlPduInd_p;
}
/* SPR 3608 changes end */
/****************************************************************************
 * Function Name  : pdcpGetControlPduInd 
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : LP_PdcpControlPduInd pointer on PDCP Control PDU 
 *                  indication structure or NULL.
 * Description    : It dequeues PdcpControlPduInd structure form PDCP Control
 *                  PDU TX queue.
 ****************************************************************************/ 
LP_PdcpControlPduInd pdcpGetControlPduInd( void )
{
    LP_PdcpControlPduInd controlPduInd_p = PNULL;

    /*Get PDCP Control PDU indication structure from PDCP Control PDU TX queue*/
    DEQUEUE_PDCP_TX_CONTROL_PDU_IND_Q(controlPduInd_p);

    return controlPduInd_p;
}

/* SPR 1781 changes start */
/* Removed definition of function pdcpGetControlPduCount() */
/* SPR 1781 changes end */

/****************************************************************************
 * Function Name  : pdcpFreeControlPduInd 
 * Inputs         : controlPduInd_p - pointer on PDCP Control PDU indication
 *                                    structure.
 * Outputs        : None.
 * Returns        : None.
 * Description    : It frees memory allocated for PdcpControlPduInd structure.
 ****************************************************************************/ 
void pdcpFreeControlPduInd( LP_PdcpControlPduInd controlPduInd_p )
{
    if ( PNULL == controlPduInd_p )
    {
        LOG_PDCP_MSG (PDCP_DATA_POINTER_NULL, LOGFATAL, PDCP_TX, pdcpCurrentTime_g,
                __LINE__, 0,0,0,0, 0,0, __func__,"controlPduInd_p is NULL" );
    }
    else
    {
        if ( PNULL != controlPduInd_p->data_p )
        {
            msgFree( controlPduInd_p->data_p );
            controlPduInd_p->data_p = PNULL;
        }

        freeMemPool( controlPduInd_p );
    }
}

/****************************************************************************
 * Function Name  : pdcpAllocateTxDeliveryInd 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  count - 
 *                  deliveryStatus - the delivery status.
 * Outputs        : None.
 * Returns        : LP_PdcpDeliveryInd pointer on TX delivery status 
 *                  indication structure or NULL.
 * Description    : To allocate and init PdcpDeliveryInd
 ****************************************************************************/ 
LP_PdcpDeliveryInd pdcpAllocateTxDeliveryInd( UInt16 ueIndex, UInt16 lcId,
        UInt32 count, PdcpStatusBuffer *deliveryStatus_p)
{
    LP_PdcpDeliveryInd deliveryInd_p = PNULL;
    UInt16 startVar=0;
    /* Allocate data request structure */
    deliveryInd_p = 
        (LP_PdcpDeliveryInd)getMemFromPool( sizeof(PdcpDeliveryInd), PNULL);

    if ( PNULL != deliveryInd_p )
    {
        /* Init dataInd_p. */
        deliveryInd_p->ueIndex = ueIndex;
        deliveryInd_p->lcId = lcId;
        deliveryInd_p->count = count;
        deliveryInd_p->deliveryStatusArr = (PdcpStatusBuffer *)\
                                           getMemFromPool(sizeof(PdcpStatusBuffer)*count, PNULL);
        if (PNULL != deliveryInd_p->deliveryStatusArr)
            /* SPR 925 FIXED -----EMD */
        {        
            for (startVar=0;startVar<count;startVar++) 
            {
                deliveryInd_p->deliveryStatusArr[startVar].transactionId =
                    deliveryStatus_p[startVar].transactionId;
                deliveryInd_p->deliveryStatusArr[startVar].deliveryStatus =
                    deliveryStatus_p[startVar].deliveryStatus;
            }
        }
        else
        {
            LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__, sizeof(PdcpStatusBuffer)*count, 
                    ueIndex, lcId, count, 0,0, __func__,
                    "deliveryInd_p->deliveryStatusArr is NULL");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            /* SPR 925 FIXED --- START*/
            ltePanic ("Memory allocation fail for %s", __FUNCTION__);
            freeMemPool (deliveryInd_p);
            deliveryInd_p = PNULL;
            /* SPR 925 FIXED -----EMD */
        }
    }
    else {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_TX, 
                pdcpCurrentTime_g,__LINE__, sizeof(PdcpDeliveryInd), ueIndex,
                lcId, count, 0,0, __func__,"deliveryInd_p is NULL");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
    }
    return deliveryInd_p;
}
