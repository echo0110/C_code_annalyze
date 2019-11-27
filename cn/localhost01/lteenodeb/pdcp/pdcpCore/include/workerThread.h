/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: workerThread.h  2017/03/15  04:51:26 
 *
 ****************************************************************************
 *
 *  File Description : PDCP Worker Threads context definitions
 *
 ***************************************************************************/

#ifndef _WORKER_THREADS_H_
#define _WORKER_THREADS_H_

#if defined(CRAN_RLC_PDCP_SPLIT) && !defined(UE_SIM_TESTING)

/***************************************************************************
 * Project specific includes
 **************************************************************************/
#include "ltePdcpContext.h"
#include "accumulatorThread.h"
#include "pdcpInternalEvents.h"

#define MAX_NUM_THREADS 3

#define MAX_UE_SUPPORTED_PER_THREAD 50

extern __thread UInt8 tssIndex;

/***************************************************************************
 * Wrapper macros definitions for PDCP RX Event queue 
 **************************************************************************/

#define QINIT_MASTER_TO_WORKER_EVENT_Q()                        \
    circQInit((LTE_CIRCQUEUE*)&workerThreadContext_g.           \
                workerThreadContext[tssIndex].masterEventsQ,    \
                RX_EVENT_Q_SIZE)                                \

#define QCOUNT_MASTER_TO_WORKER_EVENT_Q(workerIndex)            \
    circQueueCount((LTE_CIRCQUEUE*)&workerThreadContext_g.      \
                workerThreadContext[workerIndex].masterEventsQ) \

#define ENQUEUE_MASTER_TO_WORKER_EVENT_Q(workerIndex, txEvent_p)\
    pushNodeCircQ((LTE_CIRCQUEUE*)&workerThreadContext_g.       \
                workerThreadContext[workerIndex].masterEventsQ, \
                txEvent_p)

#define DEQUEUE_MASTER_TO_WORKER_EVENT_Q(rxEvent_p)             \
{                                                               \
    popNodeCircQ((LTE_CIRCQUEUE*)&workerThreadContext_g.        \
                  workerThreadContext[tssIndex].masterEventsQ,        \
                  (void **)&rxEvent_p);                         \
    rxEvent_p = (LP_PdcpInternalEvent)rxEvent_p;                \
}

#define QDEINIT_MASTER_TO_WORKER_EVENT_Q()                      \
    circQDeInit((LTE_CIRCQUEUE*)&workerThreadContext_g.         \
                workerThreadContext[tssIndex].masterEventsQ)          \


/***************************************************************************
 * Wrapper macros definition for PDCP Tx Delivery indication Queue 
 **************************************************************************/

#define QINIT_ACCUMULATOR_TO_WORKER_EVENT_Q()                     \
    circQInit((LTE_CIRCQUEUE*)&workerThreadContext_g.             \
               workerThreadContext[tssIndex].accumulatorEventsQ,  \
               RX_EVENT_Q_SIZE)

#define QCOUNT_ACCUMULATOR_TO_WORKER_EVENT_Q(workerIndex)         \
    circQueueCount((LTE_CIRCQUEUE*)&workerThreadContext_g.        \
               workerThreadContext[workerIndex].accumulatorEventsQ)  

#define ENQUEUE_ACCUMULATOR_TO_WORKER_EVENT_Q(workerIndex,        \
                                              txEvent_p)          \
    pushNodeCircQ((LTE_CIRCQUEUE*)&workerThreadContext_g.         \
               workerThreadContext[tssIndex].accumulatorEventsQ,  \
               txEvent_p)

#define DEQUEUE_ACCUMULATOR_TO_WORKER_EVENT_Q(rxEvent_p)         \
{                                                                \
    popNodeCircQ((LTE_CIRCQUEUE*)&workerThreadContext_g.         \
               workerThreadContext[tssIndex].accumulatorEventsQ, \
               (void **)&rxEvent_p);                             \
    rxEvent_p = (LP_PdcpInternalEvent)rxEvent_p;                 \
}

#define QDEINIT_ACCUMULATOR_TO_WORKER_EVENT_Q()                  \
    circQDeInit((LTE_CIRCQUEUE*)&workerThreadContext_g.          \
               workerThreadContext[tssIndex].accumulatorEventsQ)


/****************************************************************************
 * Wrapper macros definition for PDCP Tx Delivery indication Queue 
 ***************************************************************************/

#define QINIT_PDCP_TX_CONTROL_PDU_IND_Q()                        \
{                                                                \
    sQueueInit((LTE_SQUEUE*) &workerThreadContext_g.             \
       workerThreadContext[tssIndex].pdcpTxControlPduIndQ_g);    \
}

#define QCOUNT_PDCP_TX_CONTROL_PDU_IND_Q()                       \
{                                                                \
    sQueueCount((LTE_SQUEUE*) &workerThreadContext_g.            \
       workerThreadContext[tssIndex].pdcpTxControlPduIndQ_g);    \
}

#define ENQUEUE_PDCP_TX_CONTROL_PDU_IND_Q(txControlPduInd_p)     \
    pushNode((LTE_SQUEUE*) &workerThreadContext_g.               \
       workerThreadContext[tssIndex].pdcpTxControlPduIndQ_g,     \
             &(txControlPduInd_p)->sNode)                        \

#define DEQUEUE_PDCP_TX_CONTROL_PDU_IND_Q(txControlPduInd_p)     \
{                                                                \
    txControlPduInd_p = (PdcpControlPduInd *)popNode(            \
       (LTE_SQUEUE*) &workerThreadContext_g.                     \
       workerThreadContext[tssIndex].pdcpTxControlPduIndQ_g);    \
}

#define GET_FIRST_PDCP_TX_CONTROL_PDU(txControlPduInd_p)         \
{                                                                \
    txControlPduInd_p = (PdcpControlPduInd *)getFirstNode(       \
       (LTE_SQUEUE*) &workerThreadContext_g.                     \
       workerThreadContext[tssIndex].pdcpTxControlPduIndQ_g);    \
}

#define GET_NEXT_PDCP_TX_CONTROL_PDU(txControlPduInd_p)          \
{                                                                \
    txControlPduInd_p = (PdcpControlPduInd *)getNextNode(        \
       (LTE_SQUEUE*) &workerThreadContext_g.                     \
       workerThreadContext[tssIndex].pdcpTxControlPduIndQ_g);    \
}


/***************************************************************************
 * Wrapper macros definition for PDCP Tx Delivery indication Queue 
 **************************************************************************/

#define QINIT_PDCP_TX_DELIVERY_IND_Q()                           \
{                                                                \
   circQInit((LTE_CIRCQUEUE*)&workerThreadContext_g.             \
             workerThreadContext[tssIndex].pdcpTxDeliveryIndQ_g, \
             MAX_UE_SUPPORTED_PER_THREAD * MAX_LOGICAL_CHANNEL); \
}

#define QCOUNT_PDCP_TX_DELIVERY_IND_Q()                          \
    circQueueCount((LTE_CIRCQUEUE*)&workerThreadContext_g.       \
             workerThreadContext[tssIndex].pdcpTxDeliveryIndQ_g) \

#define ENQUEUE_PDCP_TX_DELIVERY_IND_Q(txDeliveryInd_p)          \
    pushNodeCircQ((LTE_CIRCQUEUE*)&workerThreadContext_g.        \
             workerThreadContext[tssIndex].pdcpTxDeliveryIndQ_g, \
             txDeliveryInd_p)                                    \

#define DEQUEUE_PDCP_TX_DELIVERY_IND_Q(txDeliveryInd_p)          \
{                                                                \
    popNodeCircQ((LTE_CIRCQUEUE*)&workerThreadContext_g.         \
             workerThreadContext[tssIndex].pdcpTxDeliveryIndQ_g, \
             (void **)&txDeliveryInd_p);                         \
    txDeliveryInd_p = (LP_PdcpDeliveryInd)txDeliveryInd_p;       \
}

#define QDEINIT_PDCP_TX_DELIVERY_IND_Q()                         \
{                                                                \
    circQDeInit((LTE_CIRCQUEUE*)&workerThreadContext_g.          \
             workerThreadContext[tssIndex].pdcpTxDeliveryIndQ_g);\
}


/***************************************************************************
 * Helper Macros for UE context
 **************************************************************************/

#define PDCP_GET_UE_CONTEXT(ueIndex)                            \
   (ueIndex >= pdcpContext.maxUeNumber) ? PNULL :               \
   workerThreadContext_g.workerThreadContext[tssIndex].         \
    ueContexts[ueIndex]                          

#define PDCP_GET_VALID_UE_CONTEXT(ueIndex)                      \
   workerThreadContext_g.workerThreadContext[tssIndex].         \
    ueContexts[ueIndex]                                         \

#define PDCP_GET_CELL_INDEX(ueIndex)                            \
   workerThreadContext_g.workerThreadContext[tssIndex].         \
               ueContexts[ueIndex]->cellIndex                   \


/****************************************************************************
 * Public data types
 ***************************************************************************/

/* This structure encapsulates data specific to a worker thread */
typedef struct WorkerContextT
{
    /* Array Containing ECN Configuration parameters */
    PdcpRrmEcnConfigParam  pdcpEcnConfigParam[MAX_CELL_PER_PDCP_INSTANCE];

    /* Number of active UEs handled by this thread */
    UInt16                 numOfActiveUe; 
    
    /* Following arrays consists of list of Active UEs */
    UInt16                 activeUeInfo[MAX_UE_SUPPORTED];

    /* Array of indexes of active UEs */
    UInt16                 activeUeIndex[MAX_UE_SUPPORTED];

    /* Array of UE contexts handled by this thread */
    LP_PdcpUeContext       ueContexts[MAX_UE_SUPPORTED];   

    /* Queue for storing delivery status indications received from RLC */
    LTE_CIRCQUEUE          pdcpTxDeliveryIndQ_g;

    /* Queue for storing PDCP control PDUs to be sent to RLC */
    LTE_SQUEUE             pdcpTxControlPduIndQ_g;

    /* Queue for storing events received from Master thread */
    LTE_CIRCQUEUE          masterEventsQ;

    /* Queue for storing events received from Accumulator thread */
    LTE_CIRCQUEUE          accumulatorEventsQ;

    /* MemPool for PDCP Internal Events */
    void*                  internalEventsPool;

    /* MemPool for LteMacStats */
    void*                  lteMacStatsPool;

    /* MemPool for PdcpStats */
    void*                  pdcpStatsPool;

    /* MemPool for master to worker events */
    void*                  masterToWorkerEventsPool;

    /* Message pool for allocating messages by this thread */
    MSGPOOL                msgPool;

    /* Timestamp when stats collected by this worker thread were sent last 
     * time to the accumulator thread */
    QTIME                  lastStatsSentTimestamp;

#ifdef KPI_DEF
    /* Object of PDCP KPI stats for this worker thread */
    LtePdcpKpiStats        pdcpKpiStats;
#endif

    /* MemPool for Receiving message from socket interface */
    void*                  pdcpIngressMsgPool;

    /* MemPool for PdcpDataReq */
    void*                  pdcpDataReqPool;

    /* MemPool for PdcpControlPduInd */
    void*                  pdcpControlPduIndPool;

    /* MemPool for PdcpDeliveryInd */
    void*                  pdcpDeliveryIndPool;

    /* MemPool for PdcpStatusBuffer */
    void*                  pdcpStatusBufferPool;

    /* MemPool for PdcpSrbDataReq */
    void*                  pdcpSrbDataReqPool;

    /* MemPool for RrcSrbDataInd */
    void*                  pdcpSrbDataIndPool;

    /* MemPool for PdcpDataInd */
    void*                  pdcpDataIndPool;

    /* MemPool for PDCP Entity contexts */
    void*                  pdcpEntityContextPool;

    /* MemPool for PDCP UE contexts */
    void*                  pdcpUeContextPool;

    /* MemPool for PDCP DRB PDU header buffers pool */
    void*                  pdcpDrbPduHdrPool;

    /* MemPool for PDCP SRB PDU header buffers pool */
    void*                  pdcpSrbPduHdrPool;

} WorkerThreadContext;


typedef struct WorkerThreadsGlobalContextT
{
    /* Periodicity (in ms) for sending stats from worker to 
     * accumulator thread */
    UInt16               statsPeriodicity;

    /* Transaction ID to be used while sending data discard request to RLC */ 
    UInt16               pdcpDiscardReqTransId_g;

    /* Transaction ID to be used while sending data request to RLC */
    UInt16               pdcpDataReqTransId_g;

    /* Array of Objects of each Worker thread context */
    WorkerThreadContext  workerThreadContext[MAX_WORKER_THREADS];

} WorkerThreadsGlobalContext;


/****************************************************************************
 * Public variables
 ***************************************************************************/

/* Object of Worker Threads global context */
WorkerThreadsGlobalContext   workerThreadContext_g;

//typedef workerThreadContext_g.workerThreadContext[tssIndex]  workerContext
#define workerContext workerThreadContext_g.workerThreadContext[tssIndex]
/* Typedef for worker thread specific pdcpKpiStats */
/* Note: This will ensure that we can use existing variable */
//typedef workerThreadContext_g.workerThreadContext[tssIndex].pdcpKpiStats   gMacStats


/****************************************************************************
 * Public Functions
 ***************************************************************************/

/****************************************************************************
 * Function Name  : freeToNamedMemPool 
 * Inputs         : Pointer to be freed
 * Outputs        : None
 * Returns        : None
 * Description    : This function free's the buffer back to pool from 
 *                  which it is allocated. 
 *****************************************************************************/
void freeToNamedMemPool(void *p , void *q);

/****************************************************************************
 * Function Name  : allocateMemoryFromIngressMsgPool 
 * Inputs         : workerIndex : Index of worker thread 
 * Outputs        : None
 * Returns        : Pointer to  allocated memory
 * Description    : This function will allocate memory from Ingress 
 *                  message pool.
 ***************************************************************************/
void* allocateMemoryFromIngressMsgPool(UInt8 workerIndex);


/****************************************************************************
 * Function Name  : allocateMemoryForMasterToWorkerEvent 
 * Inputs         : threadIndex : Index of worker thread 
 * Outputs        : None
 * Returns        : Pointer to allocated memory 
 * Description    : This function will allocate memory from master to worker
 *                  events pool maintained for each worker thread.
 ***************************************************************************/
void* allocateMemoryForMasterToWorkerEvent(UInt8 threadIndex);



/****************************************************************************
 * Function Name  : allocateMemoryFromPdcpDrbPduHdrPool
 * Inputs         : workerIndex : Index of worker thread 
 * Outputs        : None
 * Returns        : Pointer to  allocated memory
 * Description    : This function will allocate memory from PDCP DRB 
 *                  PDU header pool.
 ***************************************************************************/
void* allocateMemoryFromPdcpDrbPduHdrPool(UInt8 workerIndex);


/****************************************************************************
 * Function Name  : allocateMemoryFromPdcpSrbPduHdrPool
 * Inputs         : workerIndex : Index of worker thread 
 * Outputs        : None
 * Returns        : Pointer to  allocated memory
 * Description    : This function will allocate memory from PDCP SRB 
 *                  PDU header pool.
 ***************************************************************************/
void* allocateMemoryFromPdcpSrbPduHdrPool(UInt8 workerIndex);

/****************************************************************************
 * Function Name  : allocateMemoryFromPdcpUeContextPool
 * Inputs         : workerIndex : Index of worker thread 
 * Outputs        : None
 * Returns        : Pointer to  allocated memory
 * Description    : This function will allocate memory from PDCP UE  
 *                  context pool.
 ***************************************************************************/
void* allocateMemoryFromPdcpUeContextPool(UInt8 workerIndex);


/****************************************************************************
 * Function Name  : allocateMemoryFromPdcpEntityContextPool
 * Inputs         : workerIndex : Index of worker thread 
 * Outputs        : None
 * Returns        : Pointer to  allocated memory
 * Description    : This function will allocate memory from PDCP UE 
 *                  Entity context pool.
 ***************************************************************************/
void* allocateMemoryFromPdcpEntityContextPool(UInt8 workerIndex);


/****************************************************************************
 * Function Name  : allocateMemoryFromPdcpControlPduIndPool
 * Inputs         : workerIndex : Index of worker thread 
 * Outputs        : None
 * Returns        : Pointer to  allocated memory
 * Description    : This function will allocate memory from PDCP control 
 *                  PDU indication pool.
 ***************************************************************************/
void* allocateMemoryFromPdcpControlPduIndPool(UInt8 workerIndex);


/****************************************************************************
 * Function Name  : allocateMemoryFromPdcpDataReqPool
 * Inputs         : workerIndex : Index of worker thread 
 * Outputs        : None
 * Returns        : Pointer to  allocated memory
 * Description    : This function will allocate memory from PDCP data 
 *                  request message pool.
 ***************************************************************************/
void* allocateMemoryFromPdcpDataRequestPool(UInt8 workerIndex);


/****************************************************************************
 * Function Name  : allocateMemoryFromPdcpSrbDataReqPool
 * Inputs         : workerIndex : Index of worker thread 
 * Outputs        : None
 * Returns        : Pointer to  allocated memory
 * Description    : This function will allocate memory from PDCP SRB data 
 *                  request pool.
 ***************************************************************************/
void* allocateMemoryFromPdcpSrbDataReqPool(UInt8 workerIndex);


/****************************************************************************
 * Function Name  : allocateMemoryFromPdcpDeliveryIndPool
 * Inputs         : workerIndex : Index of worker thread 
 * Outputs        : None
 * Returns        : Pointer to  allocated memory
 * Description    : This function will allocate memory from PDCP Delivery 
 *                  indication pool.
 ***************************************************************************/
void* allocateMemoryFromPdcpDeliveryIndPool(UInt8 workerIndex);


/****************************************************************************
 * Function Name  : allocateMemoryFromPdcpStatusBufferPool
 * Inputs         : workerIndex : Index of worker thread 
 * Outputs        : None
 * Returns        : Pointer to  allocated memory
 * Description    : This function will allocate memory from PDCP Status 
 *                  buffer pool.
 ***************************************************************************/
void* allocateMemoryFromPdcpStatusBufferPool(UInt8 workerIndex);


/****************************************************************************
 * Function Name  : allocateMemoryFromPdcpSrbDataIndPool
 * Inputs         : workerIndex : Index of worker thread 
 * Outputs        : None
 * Returns        : Pointer to  allocated memory
 * Description    : This function will allocate memory from PDCP SRB data 
 *                  indication pool.
 ***************************************************************************/
void* allocateMemoryFromPdcpSrbDataIndPool(UInt8 workerIndex);


/****************************************************************************
 * Function Name  : allocateMemoryFromMacStatsPool 
 * Inputs         : threadIndex : Index of worker thread 
 * Outputs        : None
 * Returns        : Pointer to allocated memory 
 * Description    : This function will allocate memory from MAC stats pool.
 ***************************************************************************/
void* allocateMemoryFromMacStatsPool(UInt8 threadIndex);


/****************************************************************************
 * Function Name  : allocateMemoryFromPdcpStatsPool 
 * Inputs         : threadIndex : Index of worker thread 
 * Outputs        : None
 * Returns        : Pointer to allocated memory
 * Description    : This function will allocate memory from PDCP stats pool.
 ***************************************************************************/
void* allocateMemoryFromPdcpStatsPool(UInt8 threadIndex);


/****************************************************************************
 * Function Name  : workerThreadLoop 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None 
 * Description    : Start function of worker thread.
 ***************************************************************************/
void* workerThreadLoop(void* arg);

#endif


#endif  // _WORKER_THREADS_H_
