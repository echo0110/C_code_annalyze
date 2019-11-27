/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: accumulatorThread.h  2017/03/15  04:51:26 
 *
 ****************************************************************************
 *
 *  File Description : PDCP Accumulator Thread context definitions
 *
 ***************************************************************************/
#ifndef _WORKER_THREAD_H_
#define _WORKER_THREAD_H_

#if defined(CRAN_RLC_PDCP_SPLIT) && !defined(UE_SIM_TESTING)

/****************************************************************************
 * Project specific includes
 ***************************************************************************/
#include "ltePdcpStatistic.h"
#include "pdcpInternalEvents.h"


/****************************************************************************
 * Wrapper macros definitions for Accumulator worker Events queue          
 ***************************************************************************/

#define QINIT_WORKER_TO_ACCUMULATOR_EVENT_Q(workerIndex)        \
    circQInit((LTE_CIRCQUEUE*)&accumulatorContext_g.            \
              workerEventsQ[workerIndex],RX_EVENT_Q_SIZE)       \

#define QCOUNT_WORKER_TO_ACCUMULATOR_EVENT_Q(workerIndex)       \
    circQueueCount((LTE_CIRCQUEUE*)&accumulatorContext_g.       \
              workerEventsQ[workerIndex])  

#define ENQUEUE_WORKER_TO_ACCUMULATOR_EVENT_Q(workerIndex,      \
                                              txEvent_p)        \
    pushNodeCircQ((LTE_CIRCQUEUE*)&accumulatorContext_g.        \
              workerEventsQ[workerIndex], txEvent_p)            \

#define DEQUEUE_WORKER_TO_ACCUMULATOR_EVENT_Q(workerIndex,      \
                                              rxEvent_p)        \
{                                                               \
    popNodeCircQ((LTE_CIRCQUEUE*)&accumulatorContext_g.         \
              workerEventsQ[workerIndex], (void **)&rxEvent_p); \
    rxEvent_p = (LP_PdcpInternalEvent)rxEvent_p;                \
}

#define QDEINIT_WORKER_TO_ACCUMULATOR_EVENT_Q(workerIndex)      \
    circQDeInit((LTE_CIRCQUEUE*)&accumulatorContext_g.          \
              workerEventsQ[workerIndex])  


/****************************************************************************
 * Wrapper macros definitions for Accumulator Master Events queue *
 ***************************************************************************/

#define QINIT_MASTER_TO_ACCUMULATOR_EVENT_Q(masterIndex)     \
    circQInit((LTE_CIRCQUEUE*)&accumulatorContext_g.         \
              masterEventsQ[masterIndex], RX_EVENT_Q_SIZE)   \

#define QCOUNT_MASTER_TO_ACCUMULATOR_EVENT_Q(masterIndex)    \
    circQueueCount((LTE_CIRCQUEUE*)&accumulatorContext_g.    \
              masterEventsQ[masterIndex])  

#define ENQUEUE_MASTER_TO_ACCUMULATOR_EVENT_Q(masterIndex,   \
                                              txEvent_p)     \
    pushNodeCircQ((LTE_CIRCQUEUE*)&accumulatorContext_g.     \
              masterEventsQ[masterIndex], txEvent_p)         \

#define DEQUEUE_MASTER_TO_ACCUMULATOR_EVENT_Q(masterIndex,   \
                                              rxEvent_p)     \
{                                                            \
    popNodeCircQ((LTE_CIRCQUEUE*)&accumulatorContext_g.      \
              masterEventsQ[masterIndex],                    \
              (void **)&rxEvent_p);                          \
    rxEvent_p = (LP_PdcpInternalEvent)rxEvent_p;             \
}

#define QDEINIT_MASTER_TO_ACCUMULATOR_EVENT_Q(masterIndex)   \
    circQDeInit((LTE_CIRCQUEUE*)&accumulatorContext_g.       \
              masterEventsQ[masterIndex])  


/****************************************************************************
 * Public Data structures
 ***************************************************************************/
#ifdef KPI_DEF

typedef struct LtePdcpCommonUeKPIStatsT
{
    UInt32              activeIndexForPdcpThpStats;
    LtePdcpKPIThpStats  ltePdcpKPIThpStats[MAXIMUM_KPI_INDEX];
}LtePdcpCommonUeKpiStats;

typedef struct LtePdcpCommonKPIStatsT
{
    /* PDCP throughput Stats */
    UInt32              activeIndexForPdcpThpStats;
    LtePdcpKPIThpStats  ltePdcpKPIThpStats[MAXIMUM_KPI_INDEX];
    
    /* PDCP Stats*/
    UInt32              activeIndexForPdcpStats;
    LtePdcpKPIStats     ltePdcpKPIStats[MAXIMUM_KPI_INDEX];

}LtePdcpCommonKPIStats;


typedef struct LtePdcpKpiStatsT 
{
    LtePdcpCommonUeKpiStats  lteCommonUeKPIStats;
    LtePdcpCommonKPIStats	 lteCommonKPIStats[MAX_CELL_PER_PDCP_INSTANCE];
} LtePdcpKpiStats;


typedef struct PdcpKpiStatsContainerT
{
    KpiStatsType           macOrPdcpStats;

    union
    {
        LtePdcpKPIConfig   ltePdcpKPIConfig;
    }config;

    union
    {
        LtePdcpKPIStats    ltePdcpKPIStats[MAX_CELL_PER_PDCP_INSTANCE];
        LtePdcpKPIThpStats ltePdcpKPIThpStats;
    }stats;

    union
    {
        LteKPIThpStatus    ltePdcpKPIThpStatus;
    }status;

}PdcpKpiStatsContainer;


typedef struct PdcpKpiScannerT
{
    UInt16                module_bitmask;
    UInt64                timeout_tti[MAX_KPI_CONTAINER];
    QTIME                 prevkpi_tick;
    PdcpKpiStatsContainer kpiStatsContainer[MAX_KPI_CONTAINER];
} PdcpkpiScanner;

#endif

/* Runtime information maintained by Accumulator thread */
typedef struct AccumulatorContextT
{
#ifdef KPI_STATS
    /* Storage for Worker thread specific KPI Stats */
    LtePdcpKpiStats  kpiStats[MAX_WORKER_THREADS];

    /* Storage for Worker thread specific PDCP Stats */
    PdcpStats        pdcpStats[MAX_WORKER_THREADS];
#endif
    /* Queue for storing events received from Worker threads */
    LTE_CIRCQUEUE    workerEventsQ[MAX_WORKER_THREADS];

    /* Queue for storing events received from Master threads */
    LTE_CIRCQUEUE    masterEventsQ[MAX_MASTER_THREADS];

    /* Memory pools for internal events exchanged between Master 
     * and accumulator threads. */
    void*            internalEventsPool[MAX_MASTER_THREADS];

} AccumulatorContext;


/* Object of accumulator context */
AccumulatorContext  accumulatorContext_g;


/****************************************************************************
 * Public functions definitions
 ***************************************************************************/


/****************************************************************************
 * Function Name  : accumulatorThreadLoop 
 * Inputs         : arg  
 * Outputs        : None
 * Returns        : None 
 * Description    : Start function of the accumulator thread.
 ***************************************************************************/
void* accumulatorThreadLoop(void* arg);


/****************************************************************************
 * Function Name  : allocateMemoryForMasterToAccumulatorEvent 
 * Inputs         : masterIndex : Index of master thread 
 * Outputs        : None
 * Returns        : Pointer to allocated memory 
 * Description    : This function will allocate memory for internal events 
 *                  from Master to accumulator.
 ***************************************************************************/
void* allocateMemoryForMasterToAccumulatorEvent(UInt8 masterIndex);

#endif


#endif  // _WORKER_THREAD_H_
