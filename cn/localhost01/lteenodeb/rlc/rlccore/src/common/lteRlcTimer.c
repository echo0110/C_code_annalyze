/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcTimer.c,v 1.1.1.1.12.2.2.2 2010/10/07 13:35:41 gur23054 Exp $
 *
 ****************************************************************************
 *
 *  File Description : this file describes the timer operations.
 *
 *
 *
 * 1. rlcRunTmProcessor
 *
 * 2. startSPTimer 
 * 3. stopSPTimer 
 * 4. expireSPTimer
 * 5. restartSPTimer 
 * 6. startUMROTimer
 * 7. stopUMROTimer 
 * 8. expireUMROTimer
 * 9. restartUMROTimer 
 * 10. startAMROTimer
 * 11. stopAMROTimer 
 * 12. expireAMROTimer
 * 13. restartAMROTimer 
 * 14. startPRTimer
 * 15. isValidForStartPRTimer 
 * 16. isValidTimer 
 * 17. stopPRTimer
 * 18. expirePRTimer
 * 19. restartPRTimer
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcTimer.c,v $
*
*
****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcUeContext.h" 
#include "lteRlcEntityDef.h"
#include "lteRlcTimer.h"
#include "lteRlcTypes.h"
#include "lteLayer2CommanTypes.h"
#include "lteRlcGlobalData.h"
#include "logging.h"
#include "alarm.h"
#include "lteRlcAmRxOperation.h"
#include "lteRlcErrorCode.h"
#include "lteRlcUmRxOperation.h"
/*SPR 21770 Changes start*/
#include "atomic-ops.h"
/*SPR 21770 Changes end*/
#ifdef OVERSHOOT_CORRECTION
#include "lteCommonStatsManager.h"
#endif

/****************************************************************************
 * Varibale Definition
 ****************************************************************************/
/* rlc current time in ms(milliseconds) since initialised Timer module */
#ifdef RLC_STANDALONE_SETUP
/* there is separate thread for timer */
RlcThread rlcTmThread_g = RLC_THREAD_INIT ;

/* This semaphore is used by two function functions 
   1. macFirstSubFrameInd
   2. macSubFrameInd
   This function called by mac layer for indicating the 1 msec tick 
*/
LTE_SEM rlcCurrentTickSemlock_g ;
/* + SPR 9911 Fix */
#endif

#ifndef UE_SIM_TESTING 
RLC_TRIGGER_STATUS_REPORT_Q rlcTriggerStatusReportQueue_g[MAX_NUM_CELL];
#endif
/* - SPR 9911 Fix */
/* SPR 15909 fix start */
tickType_t rlcCurrentTime_g = 0 ;
/* + Layer2 NON CA RVW CMNT */ 
tickType_t globalTTITickCount_g;
/* - Layer2 NON CA RVW CMNT */ 
/* SPR 15909 fix end */
/* + Layer2 NON CA Changes */
/*SPR 18697 fix start*/ 
#ifdef LTE_EMBMS_SUPPORTED
tickType_t embmsPlatformTick_g;
#endif
/*SPR 18697 fix end*/ 
#ifndef UE_SIM_TESTING
extern UInt8 numOfRlcCellConfigured_g;
/* SPR 15909 fix start */
tickType_t rlcGlobalTTITickCount_g[MAX_NUM_CELL];
/* SPR 15909 fix end */
#endif
/*  - Layer2 NON CA Changes*/


/* SPR 5599 Fix Start */
UInt32 rlcActionOnExpiryOfPollRetransmit(TxAMEntity* pAmTxE);
void* rlcRunTmProcessor(void* arg);
/* SPR 5599 Fix End */

/* SPR 5599 Fix Start */
/*  + Layer2 NON CA Changes*/
void expireUMROTimer(
#ifndef UE_SIM_TESTING
     InternalCellIndex internalCellIndex
#endif
     );
inline static void  expireSPTimer(
#ifndef UE_SIM_TESTING
     InternalCellIndex internalCellIndex
#endif
     );
void expireAMROTimer(
#ifndef UE_SIM_TESTING
     InternalCellIndex internalCellIndex
#endif
     );
inline static void expirePRTimer(
#ifndef UE_SIM_TESTING
     InternalCellIndex internalCellIndex
#endif
     );

/** CA Changes End **/
/* SPR 5599 Fix Start */
void rlcCheckTimerExpiry(
#ifndef UE_SIM_TESTING
     InternalCellIndex internalCellIndex
#endif
     );
/*  - Layer2 NON CA Changes*/

/* + Layer2 NON CA RVW CMNT */ 

/*
Reference 36.331 (RRC Specs) 6.3.2 -RLC-Config
T-StatusProhibit and  T-:PollRetransmit ::= ENUMERATED {

                     ms0, ms5, ms10, ms15, ms20, ms25, ms30, ms35,
                     ms40, ms45, ms50, ms55, ms60, ms65, ms70,
                     ms75, ms80, ms85, ms90, ms95, ms100, ms105,
                     ms110, ms115, ms120, ms125, ms130, ms135,
                     ms140, ms145, ms150, ms155, ms160, ms165,
                     ms170, ms175, ms180, ms185, ms190, ms195,
                     ms200, ms205, ms210, ms215, ms220, ms225,
                     ms230, ms235, ms240, ms245, ms250, ms300,
                     ms350, ms400, ms450, ms500, spare8, spare7,
                     spare6, spare5, spare4, spare3, spare2,
                      spare1}
*/

#define MAX_T_STATUS_PROHIBIT_TIMER   512
#define MOD_VAL_STATUS_PROHIBIT_TIMER 511 

#define MAX_T_POLL_RETRANSMIT_TIMER 512
#define MOD_VAL_T_POLL_RETRANSMIT_TIMER 511

typedef struct RlcTStatusProhibitTimerT
{
  LTE_LIST subFrameList;

}RlcTStatusProhibitTimer;

typedef struct RlcTPollRetransmitTimerT
{
  LTE_LIST subFrameList;

}RlcTPollRetransmitTimer;

/*
In UM Mode and AM Mode Reordering Timer
Reference 36.331 (RRC Specs) 6.3.2 -RLC-Config
T-Reordering ::=  ENUMERATED {
                                ms0, ms5, ms10, ms15, ms20, ms25, ms30, ms35,
                                ms40, ms45, ms50, ms55, ms60, ms65, ms70,
                                ms75, ms80, ms85, ms90, ms95, ms100, ms110,
                                ms120, ms130, ms140, ms150, ms160, ms170,
                                ms180, ms190, ms200, spare1
                             }
*/

#define MAX_T_REORDERING_TIMER      256
#define MOD_VAL_REORDERING_TIMER    255

typedef struct RlcTReorderingTimerT
{
  LTE_LIST subFrameList;

}RlcTReorderingTimer;

#ifndef UE_SIM_TESTING
static UInt64 lastScheduleSPTimer_g[MAX_NUM_CELL];
static RlcTStatusProhibitTimer 
        SPTimerMap_g[MAX_NUM_CELL][MAX_T_STATUS_PROHIBIT_TIMER];

static UInt64 lastSchedulePRTimer_g[MAX_NUM_CELL];
static RlcTPollRetransmitTimer 
       RlcTPollRetransmitTimerMap_g[MAX_NUM_CELL][MAX_T_POLL_RETRANSMIT_TIMER];

static UInt64 lastScheduleUMROTimer_g[MAX_NUM_CELL];
RlcTReorderingTimer  
       RlcUMTReorderingTimerMap_g[MAX_NUM_CELL][MAX_T_REORDERING_TIMER];

static UInt64 lastScheduleAMROTimer_g[MAX_NUM_CELL];
RlcTReorderingTimer
       RlcAMTReorderingTimerMap_g[MAX_NUM_CELL][MAX_T_REORDERING_TIMER];

#else

static UInt64 lastSchedulePRTimer_g;
static RlcTPollRetransmitTimer 
       RlcTPollRetransmitTimerMap_g[MAX_T_POLL_RETRANSMIT_TIMER];

static UInt64 lastScheduleSPTimer_g;
static RlcTStatusProhibitTimer 
       SPTimerMap_g[MAX_T_STATUS_PROHIBIT_TIMER];

static UInt64 lastScheduleUMROTimer_g;
RlcTReorderingTimer  
       RlcUMTReorderingTimerMap_g[MAX_T_REORDERING_TIMER];

static UInt64 lastScheduleAMROTimer_g;
RlcTReorderingTimer 
       RlcAMTReorderingTimerMap_g[MAX_T_REORDERING_TIMER];
#endif


/* - Layer2 NON CA RVW CMNT */ 




/* SPR 5599 Fix End */
inline static UInt32 isValidPRStartTimer( RlcPRTm *);
/****************************************************************************
 * Function Name  : rlcInitTmThread
 * Inputs         : arg - pointer to argument(s)
 * Outputs        : None
 * Returns        : RLC_SUCCESS if timer thread created successfully else
 *                  RLC_FAILURE
 * Description    : This function initializes timer Thread.
 ****************************************************************************/
#ifdef RLC_STANDALONE_SETUP
SInt32 rlcInitTmThread(void* arg)
{
    SInt32 ret = RLC_SUCCESS ;
    rlcTmThread_g.start = rlcRunTmProcessor ;
    rlcTmThread_g.arg = arg ;
    ret = threadCreate(&rlcTmThread_g.tid,THREAD_CREATE_DETACHED,
            THREAD_SCHED_FIFO,0,
            THREAD_EXPLICIT_SCHED,
            THREAD_SCOPE_SYSTEM,rlcTmThread_g.start,PNULL);

    THREAD_TO_BIND_SET_CORE_NO(rlcTmThread_g.tid,8);

    if(ret) 
    {
        ltePanic("Unable to initialize Timer thread\n");
    }
    semInit(&rlcCurrentTickSemlock_g,0);
    return ret ;
}

/****************************************************************************
 * Function Name  : rlcDeInitTmThread
 * Inputs         : None
 * Outputs        : None
 * Returns        : RLC_SUCCESS if timer thread created successfully else
 *                  RLC_FAILURE
 * Description    : Un-initializes timer Thread.
 ****************************************************************************/
SInt32 rlcDeInitTmThread()
{
    SInt32 ret = RLC_SUCCESS ;
    if (rlcTmThread_g.tid > 0) 
    {
        ret = threadCancel(rlcTmThread_g.tid);
        rlcTmThread_g.tid = 0;
    }
    semDestroy(&rlcCurrentTickSemlock_g);

    return ret ;
}
/****************************************************************************
 * Function Name  : rlcRunTmProcessor 
 * Inputs         : arg - pointer to argument(s)
 * Outputs        : arg - pointer to argument(s)
 * Returns        : None
 * Description    : This is startup routine for timer Thread.
 ****************************************************************************/
/* SPR 5599_129117 Fix Start */
void* rlcRunTmProcessor(void* arg) 
/* SPR 5599_129117 Fix End */
{
    unsigned long new_mask = 0x01;
    pid_t p = 0;
    THREAD_TO_BIND_GET_CORE_NO(p);
    THREAD_TO_BIND_SET_CORE_NO(p,new_mask);

    for(;;)
    {

/* SPR 3160 Fix Start */
        semWait(&rlcCurrentTickSemlock_g);
/* SPR 3160 Fix End */
        rlcCheckTimerExpiry(); 
    }

    return arg;
}
#endif

/****************************************************************************
 * Function Name  : rlcCheckTimerExpiry
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function check for expiry for all TX RX timers. 
 ****************************************************************************/
/* + Layer2 NON CA RVW CMNT */ 
/* SPR 5599_129117 Fix Start */
void rlcCheckTimerExpiry(
#ifndef UE_SIM_TESTING
     InternalCellIndex internalCellIndex
#endif
     )
/* SPR 5599_129117 Fix End */
{
    //expireUMROTimer();
    expireSPTimer(
#ifndef UE_SIM_TESTING
     internalCellIndex
#endif
     );
#if defined OVERSHOOT_CORRECTION && !defined DL_UL_SPLIT /*SPR 19221 +-*/
   if (noTimeToProcessflag_g == 1)
      return;
#endif
    //expireAMROTimer();
    expirePRTimer(
#ifndef UE_SIM_TESTING
     internalCellIndex
#endif
     );
}
/* - Layer2 NON CA RVW CMNT */ 
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
/****************************************************************************
 * Function Name  : rlcCheckRxTimerExpiry
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function check for expiry for RX timers. 
 ****************************************************************************/
/*  + Layer2 NON CA Changes*/
/* + SPR 17439 */
inline void rlcCheckRxTimerExpiry(void)
/* - SPR 17439 */    
{
#ifndef UE_SIM_TESTING
    InternalCellIndex cellIndex = 0;
    for(;cellIndex < numOfRlcCellConfigured_g;cellIndex++)
    {
        expireUMROTimer(cellIndex);
    /* Moved to rlcCheckTxTimerExpiry as the timer starts in Tx */
    // expireSPTimer();
        expireAMROTimer(cellIndex);
    }
#else
    expireUMROTimer();
    expireAMROTimer();
#endif
}
/****************************************************************************
 * Function Name  : rlcCheckTxTimerExpiry
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function check for expiry for TX timers. 
 ****************************************************************************/
inline void rlcCheckTxTimerExpiry(
#ifndef UE_SIM_TESTING
InternalCellIndex internalCellIndex
#endif
)
{
         expirePRTimer(
#ifndef UE_SIM_TESTING
                 internalCellIndex
#endif         
         );
         expireSPTimer(
#ifndef UE_SIM_TESTING
                 internalCellIndex
#endif         
         );
}
/*  - Layer2 NON CA Changes*/
#endif
/**************** Status Prohibit Timer Implementation ***********************/

/****************************************************************************
 * Function Name  : isValidSPStartTimer
 * Inputs         : spTimerInfo_p - Pointer to RlcSPTm
 * Outputs        : None
 * Returns        : RLC_SUCCESS if Timer is Valid
 * Description    : This function checks for Valid SP start Timer. 
 ****************************************************************************/
inline static UInt32 isValidSPStartTimer(  RlcSPTm *spTimerInfo_p )
{
    if ( spTimerInfo_p->spTimerNode_p )
    {
        return RLC_TIMER_SP_NODE_ALREADY_EXIST;
    }
    if ( INVALID_EXP_IDX != spTimerInfo_p->spExpiryIdx )
    {
        return RLC_TIMER_SP_IDX_ALREADY_EXIST;
    }

    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : startSPTimer
 * Inputs         : rxAMEntity_p - Pointer to RxAMEntity
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_Failure/errorCode
 * Description    : Calling function should check that Status Probhibit timer 
 *                  is not already running.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 startSPTimer (  RxAMEntity* rxAMEntity_p)
/* SPR 5599_129119 Fix End */
{
    RlcSPTm *spTimerInfo_p       = PNULL;
    RlcTimerNode *rlcTimerNode_p = PNULL;
     UInt64 expiryIndex           = 0;
    /* SPR 15909 fix start */
    tickType_t tti                   = 0;
    /* SPR 15909 fix end */
    UInt32 errorCode             = INVALID_ERR_CODE;

    if ( !rxAMEntity_p)
    {
        LOG_RLC_MSG(LTE_RLC_START_SP_TIMER_ERR, LOGWARNING, 
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                0, 0, 0, 0, 0, 0, __func__, "");

        return RLC_TIMER_ERR_RX_AM_ENTITY_NOT_EXIST;
    }

/*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
    InternalCellIndex cellIndex = rlcUEContextList_g[rxAMEntity_p->ueIndex]->cellIndex;
    tti = rlcGlobalTTITickCount_g[cellIndex];
#else
    tti = globalTTITickCount_g;
#endif
/*  - Layer2 NON CA Changes*/
    spTimerInfo_p = &(rxAMEntity_p->spTimerInfo);
    errorCode = isValidSPStartTimer (spTimerInfo_p);
    if ( RLC_SUCCESS != errorCode )
    {
        LOG_RLC_MSG(LTE_RLC_START_SP_TIMER_ERR_1, LOGWARNING, 
                RLC_TIMER, tti, __LINE__,
                rxAMEntity_p->ueIndex, rxAMEntity_p->lcId, errorCode,
                0, 0, 0, __func__, "");

        return errorCode;
    }


    expiryIndex = ( spTimerInfo_p->spDuration + tti) &  (MOD_VAL_STATUS_PROHIBIT_TIMER);

    if ( (rlcTimerNode_p = (RlcTimerNode *)getMemFromPool(sizeof(RlcTimerNode), RLC_NULL))
            == RLC_NULL)
    {
        LOG_RLC_MSG(LTE_RLC_START_SP_TIMER_ERR_2, LOGWARNING, 
                RLC_TIMER, tti, __LINE__,
                rxAMEntity_p->ueIndex, rxAMEntity_p->lcId, errorCode,
                expiryIndex, 0, 0, __func__, "");

        return RLC_FAILURE;
    }

    rlcTimerNode_p->ueIndex = rxAMEntity_p->ueIndex;
    rlcTimerNode_p->lcId    = rxAMEntity_p->lcId;
    rlcTimerNode_p->isValid = RLC_TIMER_NODE_VALID;

    /* Store the reference in the AM RX Entity.*/
    spTimerInfo_p->spTimerNode_p = rlcTimerNode_p;
    spTimerInfo_p->spExpiryIdx   = expiryIndex;

#ifndef UE_SIM_TESTING
    listInsertNode(&SPTimerMap_g[cellIndex][expiryIndex].subFrameList,
            &rlcTimerNode_p->nodeAnchor);
#else
    listInsertNode(&SPTimerMap_g[expiryIndex].subFrameList,
            &rlcTimerNode_p->nodeAnchor);
#endif

    LOG_RLC_MSG(LTE_RLC_START_SP_TIMER, LOGINFO, 
            RLC_TIMER, tti, __LINE__,
            rxAMEntity_p->ueIndex, rxAMEntity_p->lcId, globalTTITickCount_g,
            spTimerInfo_p->spDuration, expiryIndex, 0, __func__, "");

    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : stopSPTimer
 * Inputs         : rxAMEntity_p - Pointer to RxAMEntity
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE with error message
 * Description    : This function Stops status prohibited timer.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 stopSPTimer ( RxAMEntity* rxAMEntity_p)
/* SPR 5599_129119 Fix End */
{
    RlcTimerNode *rlcTimerNode_p = PNULL;
    RlcSPTm *spTimerInfo_p       = PNULL;
    
    
    if ( !rxAMEntity_p )
    {
        LOG_RLC_MSG(LTE_RLC_STOP_SP_TIMER_ERR, LOGWARNING, 
                /* FDD_Warning_Fix */
                RLC_TIMER, globalTTITickCount_g , __LINE__,
                /* FDD_Warning_Fix */
                0, 0, 0, 0, 0, 0, __func__, "");

        return RLC_FAILURE;
    }
    spTimerInfo_p = &(rxAMEntity_p->spTimerInfo);
    
    rlcTimerNode_p = spTimerInfo_p->spTimerNode_p;
    if ( !rlcTimerNode_p )
    {
        LOG_RLC_MSG(LTE_RLC_STOP_SP_TIMER_ERR_1, LOGWARNING, 
                /* FDD_Warning_Fix */
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                /* FDD_Warning_Fix */
                rxAMEntity_p->ueIndex,  rxAMEntity_p->lcId,
                0, 0, 0, 0, __func__, "");

       return RLC_TIMER_ERR_TIMER_NODE_NOT_EXIST;         

    }
    if ( INVALID_EXP_IDX == spTimerInfo_p->spExpiryIdx )
    {
        LOG_RLC_MSG(LTE_RLC_STOP_SP_TIMER_ERR_2, LOGWARNING, 
                /* FDD_Warning_Fix */
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                /* FDD_Warning_Fix */
                rxAMEntity_p->ueIndex,  rxAMEntity_p->lcId,
                 spTimerInfo_p->spDuration, spTimerInfo_p->spExpiryIdx,
                 0, 0, __func__, "" );

        return RLC_TIMER_ERR_INVALID_SP_TIMER_IDX;
    }

    LOG_RLC_MSG(LTE_RLC_STOP_SP_TIMER, LOGINFO, 
            RLC_TIMER, globalTTITickCount_g, __LINE__,
            /* FDD_Warning_Fix */
            rxAMEntity_p->ueIndex, rxAMEntity_p->lcId, globalTTITickCount_g,
            /* FDD_Warning_Fix */
            spTimerInfo_p->spDuration,  spTimerInfo_p->spExpiryIdx,
            0, __func__, "");

    rlcTimerNode_p->isValid      = INVALID_RLC_TIMER_NODE;
    spTimerInfo_p->spExpiryIdx   = INVALID_EXP_IDX;
    spTimerInfo_p->spTimerNode_p = PNULL; 

    return RLC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : expireSPTimer
 * Inputs         : cellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This Function Checks for Expiry of Status Prohibited Timer.
 *******************************************************************************/
/** CA Changes Start **/
/* + Layer2 NON CA RVW CMNT */ 
inline static void  expireSPTimer(
#ifndef UE_SIM_TESTING
        InternalCellIndex cellIndex
#endif
)
/* - Layer2 NON CA RVW CMNT */ 
/** CA Changes End **/
{
    UInt64 expiryIndex = 0;
    UInt16 ueIndex     = 0;
    UInt8  lcId        = 0;
    UInt8  isValid     = 0;
    /* SPR 15909 fix start */
    tickType_t currentTTI  = 0;
    /* SPR 15909 fix end */
    UInt64 prevTick    = 0;

    RLCEntityInfo* eInfo_p       = PNULL;
    UEContext* ueContext_p       = PNULL;
    RlcSPTm   *spTimer_p         = PNULL;
    RlcTimerNode *rlcTimerNode_p = PNULL;
    RlcTimerNode *nextNode_p     = PNULL;
    RxAMEntity   *rxAMEntity_p   = PNULL;

    /*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
    currentTTI  = rlcGlobalTTITickCount_g[cellIndex];
    for ( prevTick = lastScheduleSPTimer_g[cellIndex] + 1;
            /* SPR 15909 fix start*/
            prevTick < (currentTTI+1); prevTick++)
            /* SPR 15909 fix end*/
#else
        currentTTI = globalTTITickCount_g;
    for ( prevTick = lastScheduleSPTimer_g + 1;
            /* SPR 15909 fix start*/
            prevTick < (currentTTI+1); prevTick++)
            /* SPR 15909 fix end*/
#endif
        /*  - Layer2 NON CA Changes*/
    {
        expiryIndex = (prevTick) &  MOD_VAL_STATUS_PROHIBIT_TIMER;

#ifndef UE_SIM_TESTING
        rlcTimerNode_p = ( RlcTimerNode *)
            getListNode (&(SPTimerMap_g[cellIndex][expiryIndex].subFrameList), 0);
#else
        rlcTimerNode_p = ( RlcTimerNode *)
            getListNode (&(SPTimerMap_g[expiryIndex].subFrameList), 0);
#endif

        while ( PNULL != rlcTimerNode_p )
        {
#ifndef UE_SIM_TESTING
#if defined OVERSHOOT_CORRECTION && !defined DL_UL_SPLIT
            UInt64 rem_cycles = 0;
            GET_PROCESSING_CYCLES_LEFT(rem_cycles);
            if ( ( !noTimeToProcessflag_g ) && ( rem_cycles >= PROCESS_NODE_TIME_REQD ) )
            {
#endif                
#endif                
                /* Fetch the parameter from the node */
                ueIndex = rlcTimerNode_p->ueIndex;
                lcId    = rlcTimerNode_p->lcId;
                isValid = rlcTimerNode_p->isValid;
                /* Fetch the next node from the timer list */ 
                nextNode_p = (RlcTimerNode *)getNextListNode((void *)rlcTimerNode_p);
                /* Delete the current Node */
#ifndef UE_SIM_TESTING
                listDeleteNode( &(SPTimerMap_g[cellIndex][expiryIndex].subFrameList),
                        &(rlcTimerNode_p->nodeAnchor)) ;
#else
                listDeleteNode( &(SPTimerMap_g[expiryIndex].subFrameList),
                        &(rlcTimerNode_p->nodeAnchor)) ;
#endif                
                freeMemPool((void *)rlcTimerNode_p);

                rlcTimerNode_p = nextNode_p;

                if ( RLC_TIMER_NODE_VALID != isValid )
                {
                    continue;
                }

                /* Fetch the AM Tx Entity */
				/* SPR#11313 Fix changes	*/	
				if ((( ueContext_p = DB_FETCH_UE_CONTEXT(ueIndex)) == RLC_NULL)
					          || ( ueContext_p->rlcUEState != RLC_UECONTEXT_ACTIVE ))
				/* SPR#11313 Fix changes	*/

                {
                    LOG_RLC_MSG(LTE_RLC_EXPIRE_SP_TIMER_ERR_1, LOGWARNING, 
                            RLC_TIMER, globalTTITickCount_g, __LINE__,
                            ueIndex, lcId, 0, 0, 0, 0, __func__,
                            "");
                    continue;
                }
                eInfo_p = DB_FETCH_RLC_ENTITY_INFO(ueContext_p, lcId);
                if ( eInfo_p == RLC_NULL )                
                {
                    LOG_RLC_MSG(LTE_RLC_EXPIRE_SP_TIMER_ERR_2, LOGWARNING, 
                            RLC_TIMER, globalTTITickCount_g, __LINE__,
                            ueIndex, lcId, 0, 0, 0, 0, __func__,
                            "");
                    continue;

                }
                /*SPR 16855 +-*/
                              /*+ SPR_17858_684_CHANGES*/
                if ( eInfo_p->rlcEntityMode == RLC_ENTITY_AM )
                {
                    rxAMEntity_p = (eInfo_p->rlcEntity.amEntity_p->rxAMEntity_p);
                    spTimer_p    =  &(rxAMEntity_p->spTimerInfo);
                    /** SPR 5604 Start **/
                    /*Action on expire SP Timer*/
                    rxAMEntity_p->isSPTimerRunning = RLC_FALSE;
                    /** SPR 5604 End **/
                    spTimer_p->spTimerNode_p = PNULL;
                    spTimer_p->spExpiryIdx   = INVALID_EXP_IDX;
                }
                /*- SPR_17858_684_CHANGES*/


#ifndef UE_SIM_TESTING
          if(eInfo_p->rlcEntityMode != RLC_ENTITY_AM)
          {
              continue;
          }
          if(eInfo_p->rlcEntity.amEntity_p->rxEntityState != RLC_ENTITY_ACTIVE)
          {
               LOG_RLC_MSG(LTE_RLC_EXPIRE_SP_TIMER_ERR_3, LOGWARNING, 
                       RLC_TIMER, globalTTITickCount_g, __LINE__,
                       ueIndex, lcId, eInfo_p->rlcEntity.amEntity_p->rxEntityState,
                       eInfo_p->rlcEntityMode, 0, 0, __func__,
                       "");
              continue;
          }                
#else
                if ( ( eInfo_p->rlcEntityState != RLC_ENTITY_ACTIVE ) ||
                        ( eInfo_p->rlcEntityMode != RLC_ENTITY_AM )
                   )
                {
                    LOG_RLC_MSG(LTE_RLC_EXPIRE_SP_TIMER_ERR_3, LOGWARNING, 
                            RLC_TIMER, globalTTITickCount_g, __LINE__,
                            ueIndex, lcId, eInfo_p->rlcEntityState,
                            eInfo_p->rlcEntityMode, 0, 0, __func__,
                            "");
                    continue;
                }
#endif
                rxAMEntity_p = (eInfo_p->rlcEntity.amEntity_p->rxAMEntity_p);
                spTimer_p    =  &(rxAMEntity_p->spTimerInfo);


                /** SPR 5604 Start **/
                /*Action on expire SP Timer*/
                rxAMEntity_p->isSPTimerRunning = RLC_FALSE;
                /** SPR 5604 End **/
                /* spr 8637 start */   
                if (rxAMEntity_p->sendStatusInd)
		{
			/* + SPR 9911 Fix */
#ifndef UE_SIM_TESTING 
			putEntryInRlcTrigStatusReportQueue(ueIndex,lcId,cellIndex); 
#else
			rlcTriggerStatusReport(eInfo_p->rlcEntity.amEntity_p);
#endif					
			/* - SPR 9911 Fix */
		}
                /* spr 8637 end */ 
                LOG_RLC_MSG(LTE_RLC_EXPIRE_SP_TIMER, LOGINFO, 
                        RLC_TIMER, globalTTITickCount_g, __LINE__,
                        ueIndex, lcId, globalTTITickCount_g, spTimer_p->spDuration,
                        spTimer_p->spExpiryIdx, 0, __func__,
                        "");

                spTimer_p->spTimerNode_p = PNULL;
                spTimer_p->spExpiryIdx   = INVALID_EXP_IDX;
#ifndef UE_SIM_TESTING
#if defined OVERSHOOT_CORRECTION && !defined DL_UL_SPLIT
            }
            else
            {
                noTimeToProcessflag_g = 1;

                if(currTickOverLoad_g[cellIndex] < OVERLOAD_LEVEL_MEDIUM)
                {
                    currTickOverLoad_g[cellIndex] = OVERLOAD_LEVEL_MEDIUM;
                }
                LOG_RLC_MSG( LTE_RLC_TX_LD_CHANGE, LOGWARNING, RLC_TX, 
                        globalTTITickCount_g, __LINE__,currTickOverLoad_g[cellIndex], macOverLoadLevel_g[cellIndex],
                        /*+ Dyn UEs To Be SchPerTTI +*/
                        /** CA-TDD Changes Start **/
                        dlUeSchdIndxInCurrTick_g, ulUeSchdIndxInCurrTick_g , 0, 0, __func__, "");
                        /** CA-TDD Changes Stop **/
                        /*- Dyn UEs To Be SchPerTTI -*/
                return;
            }
#endif
#endif

        }
    }
    /*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
    lastScheduleSPTimer_g[cellIndex] = currentTTI; 
#else
    lastScheduleSPTimer_g = currentTTI; 
#endif
    /*  - Layer2 NON CA Changes*/
}

/****************************************************************************
 * Function Name  : restartSPTimer
 * Inputs         : rxAMEntity_p - Pointer to RxAMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : This function Restarts Status Prohibited Timer.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
void restartSPTimer ( RxAMEntity* rxAMEntity_p)
/* SPR 5599_129119 Fix End */
{
    UInt32 errorCode = INVALID_ERR_CODE;
    
    if (!rxAMEntity_p)
    {
        LOG_RLC_MSG( LTE_RLC_RESTART_SP_TIMER_ERR, LOGWARNING, 
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                0, 0, 0, 0, 0, 0, __func__, " RX_AM_NOT_EXIST");

        return ;
    }
 
    errorCode = stopSPTimer(rxAMEntity_p);
    /* + SPR#5898 */
    if(INVALID_ERR_CODE != errorCode)
    /* - SPR#5898 */
   { 
    LOG_RLC_MSG(LTE_RLC_RESTART_SP_TIMER, LOGINFO, 
            RLC_TIMER, globalTTITickCount_g, __LINE__,
            rxAMEntity_p->ueIndex, rxAMEntity_p->lcId,
            globalTTITickCount_g, (rxAMEntity_p->spTimerInfo.spDuration),
            (rxAMEntity_p->spTimerInfo.spExpiryIdx), errorCode, __func__,
            "Stop SP TIMER -> Start");
   }
   errorCode = startSPTimer(rxAMEntity_p);
   /* + SPR#5898 */
   if(INVALID_ERR_CODE != errorCode)
   /* - SPR#5898 */       
   { 
     LOG_RLC_MSG(LTE_RLC_RESTART_SP_TIMER_1, LOGINFO, 
            RLC_TIMER, globalTTITickCount_g, __LINE__,
            rxAMEntity_p->ueIndex, rxAMEntity_p->lcId,
            globalTTITickCount_g, (rxAMEntity_p->spTimerInfo.spDuration),
            (rxAMEntity_p->spTimerInfo.spExpiryIdx), errorCode, __func__,
            "Start SP TIMER -> Start");
   }
}


/*********************** End of Status Probihit timer ***************************/



/************************************************************************************************
 * Function Name  : isValidUMROStartTimer
 * Inputs         : umROTimer_p - Pointer to RlcROTm
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_TIMER_SP_NODE_ALREADY_EXIST/RLC_TIMER_SP_IDX_ALREADY_EXIST
 * Description    : This function checks whether UMRO Start Timer is Valid or not. 
 ************************************************************************************************/
inline static UInt32 isValidUMROStartTimer( RlcROTm  *umROTimer_p)
{
    if ( umROTimer_p->roTimerNode_p )
    {
        return RLC_TIMER_SP_NODE_ALREADY_EXIST;
    }
    if ( INVALID_EXP_IDX != umROTimer_p->roExpiryIdx )
    {
        return RLC_TIMER_SP_IDX_ALREADY_EXIST;
    }

    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : startUMROTimer
 * Inputs         : rxUMEntity_p - Pointer to RxUMEntity
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE/errorCode
 * Description    : This API starts UMRO Timer.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 startUMROTimer( RxUMEntity* rxUMEntity_p)
/* SPR 5599_129119 Fix End */
{
    RlcROTm      *umROTimer_p    = PNULL;
    RlcTimerNode *rlcTimerNode_p = PNULL;
     UInt64 expiryIndex           = 0;
    /* SPR 15909 fix start */
    tickType_t tti                   = 0;  
    /* SPR 15909 fix end */
    UInt32 errorCode             = INVALID_ERR_CODE;

    if ( PNULL == rxUMEntity_p )
    {
        LOG_RLC_MSG(LTE_RLC_START_UM_RO_TIMER_ERR, LOGWARNING, 
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                0, 0, 0, 0, 0, 0, __func__, " RX_UM_NOT_EXIST");
        return RLC_FAILURE;
    }
    umROTimer_p = &(rxUMEntity_p->umROTimerInfo);
  
    errorCode = isValidUMROStartTimer ( umROTimer_p );   

    if ( RLC_SUCCESS != errorCode )
    {
        LOG_RLC_MSG(LTE_RLC_START_UM_RO_TIMER_ERR_1, LOGWARNING, 
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                rxUMEntity_p->ueIndex, rxUMEntity_p->lcId, errorCode,
                0, 0, 0, __func__, "UM_RO_TIMER Already Running");

        return errorCode;
    }
    /*Coverity 72885 Fix Start*/
    /*Code Removed*/
    /*Coverity 72885 Fix End*/

/*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
    InternalCellIndex cellIndex = rlcUEContextList_g[rxUMEntity_p->ueIndex]->cellIndex;
    tti = rlcGlobalTTITickCount_g[cellIndex];
#else
    tti = globalTTITickCount_g;
#endif
/*  - Layer2 NON CA Changes*/
    expiryIndex = ( umROTimer_p->roDuration + tti) & ( MOD_VAL_REORDERING_TIMER );
    
    if ( (rlcTimerNode_p = (RlcTimerNode *)getMemFromPool(sizeof(RlcTimerNode), RLC_NULL))
            == RLC_NULL)
    {
        LOG_RLC_MSG( LTE_RLC_START_UM_RO_TIMER_ERR_2, LOGWARNING, 
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                rxUMEntity_p->ueIndex, rxUMEntity_p->lcId, errorCode,
                expiryIndex, 0, 0, __func__, "MEM_FAILED");

        return RLC_FAILURE;
    }
    rlcTimerNode_p->ueIndex     = rxUMEntity_p->ueIndex;
    rlcTimerNode_p->lcId        = rxUMEntity_p->lcId;
    rlcTimerNode_p->isValid     = RLC_TIMER_NODE_VALID;

    /* Store the reference in the AM TX Entity.*/
    umROTimer_p->roTimerNode_p = rlcTimerNode_p;
    umROTimer_p->roExpiryIdx   = expiryIndex;

#ifndef UE_SIM_TESTING
    listInsertNode(&RlcUMTReorderingTimerMap_g[cellIndex][expiryIndex].subFrameList,
                   &rlcTimerNode_p->nodeAnchor);
#else
    listInsertNode(&RlcUMTReorderingTimerMap_g[expiryIndex].subFrameList,
                   &rlcTimerNode_p->nodeAnchor);
#endif

    LOG_RLC_MSG( LTE_RLC_START_UM_RO_TIMER, LOGINFO, 
            RLC_TIMER, globalTTITickCount_g, __LINE__,
            rxUMEntity_p->ueIndex, rxUMEntity_p->lcId, globalTTITickCount_g,
            umROTimer_p->roDuration, expiryIndex, 0, __func__, "UM_RO_TIMER Start");

    return RLC_SUCCESS;
}

/******************************************************************************
 * Function Name  : stopUMROTimer
 * Inputs         : rxUMEntity_p - Pointer to RxUMEntity
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API stops UMRO Timers. This is called from scheduler.
 ******************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 stopUMROTimer(  RxUMEntity* rxUMEntity_p)
/* SPR 5599_129119 Fix End */
{
    RlcTimerNode *rlcTimerNode_p = PNULL;
    RlcROTm      *umROTimer_p    = PNULL; 
 
    if ( !rxUMEntity_p )
    {
        LOG_RLC_MSG(LTE_RLC_STOP_UM_RO_TIMER_ERR, LOGWARNING, 
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                0, 0, 0, 0, 0, 0, __func__, " RX_UM_NOT_EXIST");

         return RLC_FAILURE;
    }
    umROTimer_p = &(rxUMEntity_p->umROTimerInfo);
    
    rlcTimerNode_p = umROTimer_p->roTimerNode_p;
    if ( !rlcTimerNode_p )
    {
        LOG_RLC_MSG(LTE_RLC_STOP_UM_RO_TIMER_ERR_1, LOGWARNING, 
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                rxUMEntity_p->ueIndex,  rxUMEntity_p->lcId,
                umROTimer_p->roDuration, umROTimer_p->roExpiryIdx,
                0, 0, __func__, "RO_TIMER_NODE_NOT_EXIST");

        return RLC_FAILURE;
    }
    if ( INVALID_EXP_IDX == umROTimer_p->roExpiryIdx )
    {
        LOG_RLC_MSG(LTE_RLC_STOP_UM_RO_TIMER_ERR_2, LOGWARNING, 
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                rxUMEntity_p->ueIndex,  rxUMEntity_p->lcId,
                umROTimer_p->roDuration, umROTimer_p->roExpiryIdx,
                0, 0, __func__, " RO_TIMER_IDX_INVALID");

        return RLC_FAILURE;

    }
    LOG_RLC_MSG( LTE_RLC_STOP_UM_RO_TIMER, LOGINFO, 
            RLC_TIMER, globalTTITickCount_g, 
            rxUMEntity_p->ueIndex, rxUMEntity_p->lcId, globalTTITickCount_g,
            umROTimer_p->roDuration, umROTimer_p->roExpiryIdx,
            0,0, __func__, "RO_TIMER Stop");

    rlcTimerNode_p->isValid    = INVALID_RLC_TIMER_NODE;
    umROTimer_p->roTimerNode_p = PNULL;
    umROTimer_p->roExpiryIdx   = INVALID_EXP_IDX;

    return RLC_SUCCESS;
}

/*********************************************************************************
 * Function Name  : expireUMROTimer
 * Inputs         : cellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API handle expire Timers. This is called from scheduler.
 *********************************************************************************/
/* SPR 5599_129117 Fix Start */
/*  + Layer2 NON CA Changes*/
/* + Layer2 NON CA RVW CMNT */ 
void  expireUMROTimer(
#ifndef UE_SIM_TESTING
        InternalCellIndex cellIndex
#endif
        )
/* - Layer2 NON CA RVW CMNT */ 
/*  - Layer2 NON CA Changes*/
/* SPR 5599_129117 Fix End */
{
    UInt64 expiryIndex = 0;
    UInt16 ueIndex     = 0;
    UInt8  lcId        = 0;
    UInt8  isValid     = 0;
    /* SPR 15909 fix start */
    tickType_t currentTTI  = 0;
    /* SPR 15909 fix end */
    UInt64 prevTick    = 0;

    RLCEntityInfo* eInfo_p       = PNULL;
    UEContext* ueContext_p       = PNULL;
    RlcTimerNode *rlcTimerNode_p = PNULL;
    RlcTimerNode *nextNode_p     = PNULL;
    RxUMEntity* rxUMEntity_p     = PNULL;
    RlcROTm *umROTimer_p         = PNULL;


    /*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
    currentTTI  = rlcGlobalTTITickCount_g[cellIndex];
    for ( prevTick = lastScheduleUMROTimer_g[cellIndex] + 1;
            /* SPR 15909 fix start*/
            prevTick < (currentTTI+1); prevTick++)
            /* SPR 15909 fix end*/
#else
        currentTTI = globalTTITickCount_g;
    for ( prevTick = lastScheduleUMROTimer_g + 1;
            /* SPR 15909 fix start*/
            prevTick < (currentTTI+1); prevTick++)
            /* SPR 15909 fix end*/
#endif
    /*  - Layer2 NON CA Changes*/
    {
        expiryIndex = ( prevTick ) & MOD_VAL_REORDERING_TIMER;

#ifndef UE_SIM_TESTING
        rlcTimerNode_p = (RlcTimerNode *)
            getListNode (&(RlcUMTReorderingTimerMap_g[cellIndex][expiryIndex].subFrameList), 0);
#else
        rlcTimerNode_p = (RlcTimerNode *)
            getListNode (&(RlcUMTReorderingTimerMap_g[expiryIndex].subFrameList), 0);
#endif

        while ( PNULL != rlcTimerNode_p )
        {
/* SPR_5948_CHG_START */
/* SPR_5948_CHG_END */
                ueIndex = rlcTimerNode_p->ueIndex;
                lcId    = rlcTimerNode_p->lcId;
                isValid = rlcTimerNode_p->isValid;

                nextNode_p = (RlcTimerNode *)getNextListNode((void *)rlcTimerNode_p);

#ifndef UE_SIM_TESTING
            listDeleteNode(&(RlcUMTReorderingTimerMap_g[cellIndex][expiryIndex].subFrameList),
                    &(rlcTimerNode_p->nodeAnchor)) ;
#else
                listDeleteNode(&(RlcUMTReorderingTimerMap_g[expiryIndex].subFrameList),
                        &(rlcTimerNode_p->nodeAnchor)) ;
#endif               
                freeMemPool((void *)rlcTimerNode_p);

                rlcTimerNode_p = nextNode_p;

                if ( RLC_TIMER_NODE_VALID != isValid )
                {
                    continue;
                }
				/* SPR#11313 Fix changes	*/	
				if ((( ueContext_p = DB_FETCH_UE_CONTEXT(ueIndex)) == RLC_NULL)
				     ||      ( ueContext_p->rlcUEState != RLC_UECONTEXT_ACTIVE ))
				/* SPR#11313 Fix changes	*/

                {
                    LOG_RLC_MSG(LTE_RLC_EXPIRE_UM_RO_TIMER_ERR_1, LOGWARNING, 
                            RLC_TIMER, globalTTITickCount_g, __LINE__,
                            ueIndex, lcId, 0, 0, 0, 0, __func__,
                            " UE_CTX_NOT_EXIST");
                    continue;
                }
                eInfo_p = DB_FETCH_RLC_ENTITY_INFO(ueContext_p, lcId);
                if ( RLC_NULL ==  eInfo_p )                          
                {
                    LOG_RLC_MSG(LTE_RLC_EXPIRE_UM_RO_TIMER_ERR_2, LOGWARNING, 
                            RLC_TIMER, globalTTITickCount_g, __LINE__,
                            ueIndex, lcId, 0, 0, 0, 0, __func__,
                            "RX_UM_ENTITY_NOT_EXIST");
                    continue;

           }
            /*SPR 16855 +-*/
            /*+ SPR_17858_684_CHANGES*/
            if ( eInfo_p->rlcEntityMode == RLC_ENTITY_UM )
            {
                rxUMEntity_p = (eInfo_p->rlcEntity.umEntity_p->rxUMEntity_p);
                umROTimer_p  = &(rxUMEntity_p->umROTimerInfo);
                umROTimer_p->roTimerNode_p = PNULL;
                umROTimer_p->roExpiryIdx   = INVALID_EXP_IDX;
            }
            /*- SPR_17858_684_CHANGES*/
#ifndef UE_SIM_TESTING
          if(eInfo_p->rlcEntityMode != RLC_ENTITY_UM)
          {
              continue;
          }
          if(eInfo_p->rlcEntity.umEntity_p->rxEntityState != RLC_ENTITY_ACTIVE)
          {
               LOG_RLC_MSG(LTE_RLC_EXPIRE_UM_RO_TIMER_ERR_3, LOGWARNING, 
                       RLC_TIMER, globalTTITickCount_g, __LINE__,
                       ueIndex, lcId, eInfo_p->rlcEntity.umEntity_p->rxEntityState,
                       eInfo_p->rlcEntityMode, 0, 0, __func__,
                       "RX_UM_ENTITY_NOT_EXIST");
              continue;
          }                
#endif          

                rxUMEntity_p = (eInfo_p->rlcEntity.umEntity_p->rxUMEntity_p); 
                umROTimer_p  = &(rxUMEntity_p->umROTimerInfo);

                LOG_RLC_MSG(LTE_RLC_EXPIRE_UM_RO_TIMER, LOGINFO, 
                        RLC_TIMER, globalTTITickCount_g, __LINE__,
                        ueIndex, lcId, globalTTITickCount_g, umROTimer_p->roDuration,
                        umROTimer_p->roExpiryIdx, 0, __func__,
                        "UM_RO_TIMER Expired");

                umROTimer_p->roTimerNode_p = PNULL;
                umROTimer_p->roExpiryIdx   = INVALID_EXP_IDX;

                /*Action on expire */
                rlcActionUmTReorderingExpire( rxUMEntity_p);
/* SPR_5948_CHG_START */
/* SPR_5948_CHG_END */
        }
    }
    /*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
    lastScheduleUMROTimer_g[cellIndex] = currentTTI; 
/* + Layer2 NON CA RVW CMNT */ 

#else
    lastScheduleUMROTimer_g = currentTTI;
#endif
    /*  - Layer2 NON CA Changes*/

}

/****************************************************************************
 * Function Name  : restartUMROTimer
 * Inputs         : rxUMEntity_p - Pointer to RxUMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : This function restarts UMRO Timer.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
void restartUMROTimer( RxUMEntity* rxUMEntity_p)
/* SPR 5599_129119 Fix End */
{
    UInt32 errorCode = INVALID_ERR_CODE;
    
    if ( !rxUMEntity_p )
    {
        LOG_RLC_MSG(LTE_RLC_RESTART_UM_RO_TIMER_ERR, LOGWARNING, 
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                0, 0, 0, 0, 0, 0, __func__, " RX_UM_NOT_EXIST");
        return ;        
    }
   
    errorCode = stopUMROTimer ( rxUMEntity_p );
    /* + SPR#5898 */
    if(INVALID_ERR_CODE != errorCode)
    /* - SPR#5898 */
    {
    LOG_RLC_MSG(LTE_RLC_RESTART_UM_RO_TIMER, LOGINFO, 
            RLC_TIMER, globalTTITickCount_g, __LINE__,
            rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
            globalTTITickCount_g, (rxUMEntity_p->umROTimerInfo.roDuration),
            (rxUMEntity_p->umROTimerInfo.roExpiryIdx), errorCode, __func__,
            "Stop UM_RO_TIMER Now Start UM RO Timer");
    }
    errorCode = startUMROTimer( rxUMEntity_p);
   
    /* + SPR#5898 */
    if(INVALID_ERR_CODE != errorCode)
   /* - SPR#5898 */ 
   {
    LOG_RLC_MSG(LTE_RLC_RESTART_UM_RO_TIMER_1, LOGINFO, 
            RLC_TIMER, globalTTITickCount_g, __LINE__,
            rxUMEntity_p->ueIndex, rxUMEntity_p->lcId,
            globalTTITickCount_g, (rxUMEntity_p->umROTimerInfo.roDuration),
            (rxUMEntity_p->umROTimerInfo.roExpiryIdx), errorCode, __func__,
            "UM_RO_TIMER_RE_STARTED");
   }
}

/******************************* End UM T Reordering Timer **********************************/



/*********************************************************************************************
 * Function Name  : isValidAMROStartTimer
 * Inputs         : amROTimer_p - Pointer to RlcROTm
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_TIMER_RO_IDX_ALREADY_EXIST/RLC_TIMER_RO_NODE_ALREADY_EXIST
 * Description    : This function restarts UMRO Timer.
 **********************************************************************************************/
inline static UInt32 isValidAMROStartTimer( RlcROTm  *amROTimer_p)
{
    if ( amROTimer_p->roTimerNode_p )
    {
        return RLC_TIMER_RO_NODE_ALREADY_EXIST;
    }
    if ( INVALID_EXP_IDX != amROTimer_p->roExpiryIdx )
    {
        return RLC_TIMER_RO_IDX_ALREADY_EXIST;
    }

    return RLC_SUCCESS;
}

/********************************************************************************
 * Function Name  : startAMROTimer
 * Inputs         : rxAMEntity_p - Pointer to RxAMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : This function START AMRO Timer.
 ********************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 startAMROTimer( RxAMEntity* rxAMEntity_p)
/* SPR 5599_129119 Fix End */
{
    RlcTimerNode *rlcTimerNode_p = PNULL;
     UInt64 expiryIndex           = 0;
    RlcROTm *amRoTimer_p         = PNULL;
    /* SPR 15909 fix start */
    tickType_t tti                   = 0;
    /* SPR 15909 fix end */
    UInt32 errorCode             = INVALID_ERR_CODE;

    tti = globalTTITickCount_g;
    if ( PNULL == rxAMEntity_p )
    {
        LOG_RLC_MSG(LTE_RLC_START_AM_RO_TIMER_ERR, LOGWARNING, 
                RLC_TIMER, tti, __LINE__,
                0, 0, 0, 0, 0, 0, __func__, " RX_AM_NOT_EXIST");
        return RLC_FAILURE;
    }
    amRoTimer_p = &(rxAMEntity_p->amROTimerInfo);
   
    errorCode = isValidAMROStartTimer (amRoTimer_p );

    if ( RLC_SUCCESS != errorCode )
    {
        LOG_RLC_MSG(LTE_RLC_START_AM_RO_TIMER_ERR_1, LOGWARNING, 
                RLC_TIMER, tti, __LINE__,
                rxAMEntity_p->ueIndex, rxAMEntity_p->lcId, errorCode,
                0, 0, 0, __func__, "AM_RO_TIMER Already Running");

        return errorCode;
    } 

/*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
    InternalCellIndex cellIndex = rlcUEContextList_g[rxAMEntity_p->ueIndex]->cellIndex;
    tti = rlcGlobalTTITickCount_g[cellIndex];
#else
    tti = globalTTITickCount_g;
#endif
/*  - Layer2 NON CA Changes*/

    expiryIndex = ( amRoTimer_p->roDuration + tti) & MOD_VAL_REORDERING_TIMER;

    if ( (rlcTimerNode_p = (RlcTimerNode *)getMemFromPool(sizeof(RlcTimerNode), RLC_NULL))
            == RLC_NULL)
    {
        LOG_RLC_MSG( LTE_RLC_START_AM_RO_TIMER_ERR_2, LOGWARNING, 
                RLC_TIMER, tti, __LINE__,
                rxAMEntity_p->ueIndex, rxAMEntity_p->lcId, errorCode,
                expiryIndex, 0, 0, __func__, "MEM_FAILED");

        return RLC_FAILURE;
    }

    rlcTimerNode_p->ueIndex     = rxAMEntity_p->ueIndex;
    rlcTimerNode_p->lcId        = rxAMEntity_p->lcId;
    rlcTimerNode_p->isValid     = RLC_TIMER_NODE_VALID;

    /* Store the reference in the AM TX Entity.*/
    amRoTimer_p->roTimerNode_p = rlcTimerNode_p;
    amRoTimer_p->roExpiryIdx   = expiryIndex;

/*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
    listInsertNode(&RlcAMTReorderingTimerMap_g[cellIndex][expiryIndex].subFrameList,
            &rlcTimerNode_p->nodeAnchor);
#else
    listInsertNode(&RlcAMTReorderingTimerMap_g[expiryIndex].subFrameList,
            &rlcTimerNode_p->nodeAnchor);
#endif
/*  - Layer2 NON CA Changes*/

    LOG_RLC_MSG( LTE_RLC_START_AM_RO_TIMER, LOGINFO, 
            RLC_TIMER, tti, __LINE__,
            rxAMEntity_p->ueIndex, rxAMEntity_p->lcId, 0,
            amRoTimer_p->roDuration, expiryIndex, 0, __func__, "AM_RO_TIMER Start");
    
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : stopAMROTimer
 * Inputs         : rxAMEntity_p - Pointer to RxAMEntity
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This function stops AMRO Timer.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 stopAMROTimer (  RxAMEntity* rxAMEntity_p )
/* SPR 5599_129119 Fix End */
{
    RlcROTm *amRoTimer_p         = PNULL;
    RlcTimerNode *rlcTimerNode_p = PNULL;

    if ( !rxAMEntity_p )
    {
        LOG_RLC_MSG(LTE_RLC_STOP_AM_RO_TIMER_ERR, LOGWARNING,
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                0, 0, 0, 0, 0, 0, __func__, " RX_UM_NOT_EXIST");

         return RLC_FAILURE;
    }
    amRoTimer_p = &(rxAMEntity_p->amROTimerInfo);
    rlcTimerNode_p = amRoTimer_p->roTimerNode_p;
    if ( !rlcTimerNode_p )
    {
        LOG_RLC_MSG(LTE_RLC_STOP_AM_RO_TIMER_ERR_1, LOGWARNING,
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                rxAMEntity_p->ueIndex, rxAMEntity_p->lcId,
                amRoTimer_p->roDuration, amRoTimer_p->roExpiryIdx,
                0, 0, __func__, "RO_TIMER_NODE_NOT_EXIST");

        return RLC_FAILURE;
    }
   
    if ( INVALID_EXP_IDX == amRoTimer_p->roExpiryIdx)
    {
        LOG_RLC_MSG(LTE_RLC_STOP_AM_RO_TIMER_ERR_2, LOGWARNING, 
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                rxAMEntity_p->ueIndex, rxAMEntity_p->lcId,
                amRoTimer_p->roDuration, amRoTimer_p->roExpiryIdx,
                0, 0, __func__, " RO_TIMER_IDX_INVALID");
        return RLC_FAILURE;
    }
    LOG_RLC_MSG( LTE_RLC_STOP_AM_RO_TIMER, LOGINFO, 
            RLC_TIMER, globalTTITickCount_g, __LINE__,
            rxAMEntity_p->ueIndex, rxAMEntity_p->lcId, 0,
            amRoTimer_p->roDuration, amRoTimer_p->roExpiryIdx,
            0, __func__, "RO_TIMER Stop");

    rlcTimerNode_p->isValid    = INVALID_RLC_TIMER_NODE;
    amRoTimer_p->roTimerNode_p = PNULL;
    amRoTimer_p->roExpiryIdx   = INVALID_EXP_IDX;

    return RLC_SUCCESS;
}

/***************************************************************************************
 * Function Name  : expireAMTReorderingTimer
 * Inputs         : cellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API handle expire AMRO Timers. This is called from scheduler.
 **************************************************************************************/
/* SPR 5599_129117 Fix Start */
/** CA Changes Start **/
/* + Layer2 NON CA RVW CMNT */ 
void expireAMROTimer(
#ifndef UE_SIM_TESTING
        InternalCellIndex cellIndex
#endif
)
/* - Layer2 NON CA RVW CMNT */ 
/** CA Changes End **/
/* SPR 5599_129117 Fix End */
{
    UInt64 expiryIndex = 0;
    UInt16 ueIndex     = 0;
    UInt8  lcId        = 0;
    UInt8  isValid     = 0;
    /* SPR 15909 fix start */
    tickType_t currentTTI  = 0;
    /* SPR 15909 fix end */
    UInt64 prevTick    = 0;
    
    RLCEntityInfo* eInfo_p       = PNULL;
    UEContext* ueContext_p       = PNULL;
    RlcROTm    *amROTimer_p      = PNULL;
    RlcTimerNode *rlcTimerNode_p = PNULL;
    RlcTimerNode *nextNode_p     = PNULL;
    RxAMEntity* rxAMEntity_p     = PNULL;

    /*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
    currentTTI  = rlcGlobalTTITickCount_g[cellIndex];
    /* SPR 15909 fix start*/
	//bug[004]jsm
    for ( prevTick = lastScheduleAMROTimer_g[cellIndex]; prevTick <
            (currentTTI+1); prevTick++)
        /* SPR 15909 fix end*/
#else
        currentTTI = globalTTITickCount_g;
    /* SPR 15909 fix start*/
    for ( prevTick = lastScheduleAMROTimer_g + 1; prevTick < 
                (currentTTI+1); prevTick++)
        /* SPR 15909 fix end*/
#endif
        /*  - Layer2 NON CA Changes*/
    {
        expiryIndex = (prevTick) &  MOD_VAL_REORDERING_TIMER;

#ifndef UE_SIM_TESTING
        rlcTimerNode_p = ( RlcTimerNode *)
            getListNode (&(RlcAMTReorderingTimerMap_g[cellIndex][expiryIndex].subFrameList), 0);
#else
        rlcTimerNode_p = ( RlcTimerNode *)
            getListNode (&(RlcAMTReorderingTimerMap_g[expiryIndex].subFrameList), 0);
#endif

        while ( PNULL != rlcTimerNode_p )
        {
            ueIndex = rlcTimerNode_p->ueIndex;
            lcId    = rlcTimerNode_p->lcId;
            isValid = rlcTimerNode_p->isValid;

            nextNode_p = (RlcTimerNode *)getNextListNode((void *)rlcTimerNode_p);

#ifndef UE_SIM_TESTING
            listDeleteNode(&(RlcAMTReorderingTimerMap_g[cellIndex][expiryIndex].subFrameList),
                    &(rlcTimerNode_p->nodeAnchor)) ;
#else
            listDeleteNode(&(RlcAMTReorderingTimerMap_g[expiryIndex].subFrameList),
                    &(rlcTimerNode_p->nodeAnchor)) ;
#endif
            freeMemPool((void *)rlcTimerNode_p);

            rlcTimerNode_p = nextNode_p;

            if ( RLC_TIMER_NODE_VALID != isValid )
            {

                continue;
            }

            /* Fetch the AM RX Entity */
			/* SPR#11313 Fix changes	*/	
			if ((( ueContext_p = DB_FETCH_UE_CONTEXT(ueIndex)) == RLC_NULL)
			         	|| ( ueContext_p->rlcUEState != RLC_UECONTEXT_ACTIVE ))
			/* SPR#11313 Fix changes	*/

            {
                LOG_RLC_MSG(LTE_RLC_EXPIRE_AM_RO_TIMER_ERR_1, LOGWARNING, 
                        RLC_TIMER, globalTTITickCount_g, __LINE__,
                        ueIndex, lcId, 0, 0, 0, 0, __func__,
                        " UE_CTX_NOT_EXIST");
                continue;
            }
            eInfo_p = DB_FETCH_RLC_ENTITY_INFO(ueContext_p, lcId);
            if ( eInfo_p == RLC_NULL )
            {
                LOG_RLC_MSG(LTE_RLC_EXPIRE_AM_RO_TIMER_ERR_2, LOGWARNING, 
                        RLC_TIMER, globalTTITickCount_g, __LINE__,
                        ueIndex, lcId, 0, 0, 0, 0, __func__,
                        "RX_AM_ENTITY_NOT_EXIST");

                continue;
            }
            /*SPR 16855 +-*/
            /*+ SPR_17858_684_CHANGES*/
            if ( eInfo_p->rlcEntityMode == RLC_ENTITY_AM )
            {
                rxAMEntity_p = (eInfo_p->rlcEntity.amEntity_p->rxAMEntity_p);
                amROTimer_p  = &(rxAMEntity_p->amROTimerInfo);
                amROTimer_p->roTimerNode_p = PNULL;
                amROTimer_p->roExpiryIdx   = INVALID_EXP_IDX;
                rxAMEntity_p->isRTimerRunning = RLC_FALSE ;
            }
            /*- SPR_17858_684_CHANGES*/
#ifndef UE_SIM_TESTING
          if(eInfo_p->rlcEntityMode != RLC_ENTITY_AM)
          {
              continue;
          }
          if(eInfo_p->rlcEntity.amEntity_p->rxEntityState != RLC_ENTITY_ACTIVE)
          {
              LOG_RLC_MSG(LTE_RLC_EXPIRE_AM_RO_TIMER_ERR_3, LOGWARNING, 
                        RLC_TIMER, globalTTITickCount_g, __LINE__,
                        ueIndex, lcId, eInfo_p->rlcEntity.amEntity_p->rxEntityState,
                        eInfo_p->rlcEntityMode, 0, 0, __func__,
                        "RX_AM_ENTITY_NOT_EXIST");
              continue;
          }                
#else
            if ( ( eInfo_p->rlcEntityState != RLC_ENTITY_ACTIVE ) ||
                    ( eInfo_p->rlcEntityMode != RLC_ENTITY_AM )
               )
            {
                LOG_RLC_MSG(LTE_RLC_EXPIRE_AM_RO_TIMER_ERR_3, LOGWARNING, 
                        RLC_TIMER, globalTTITickCount_g, __LINE__,
                        ueIndex, lcId, eInfo_p->rlcEntityState,
                        eInfo_p->rlcEntityMode, 0, 0, __func__,
                        "RX_AM_ENTITY_NOT_EXIST");
                continue;
            }
#endif          
            rxAMEntity_p = (eInfo_p->rlcEntity.amEntity_p->rxAMEntity_p);
            amROTimer_p  = &(rxAMEntity_p->amROTimerInfo);

            LOG_RLC_MSG(LTE_RLC_EXPIRE_AM_RO_TIMER, LOGINFO, 
                    RLC_TIMER, globalTTITickCount_g, __LINE__,
                    ueIndex, lcId, 0, amROTimer_p->roDuration,
                    amROTimer_p->roExpiryIdx, 0, __func__,
                    "AM_RO_TIMER Expired");

            amROTimer_p->roTimerNode_p = PNULL;
            amROTimer_p->roExpiryIdx   = INVALID_EXP_IDX;

            /*Action on expire */
            rlcActionAmTReorderingExpire( rxAMEntity_p); 
        }
    }// for loop 
    /*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
    lastScheduleAMROTimer_g[cellIndex] = currentTTI; 
/* + Layer2 NON CA RVW CMNT */ 
#else
    lastScheduleAMROTimer_g = currentTTI;
#endif
    /*  - Layer2 NON CA Changes*/
}


/****************************************************************************
 * Function Name  : restartAMROTimer
 * Inputs         : rxAMEntity_p - Pointer to RxAMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : This function restarts AMRO Timer.
 ****************************************************************************/

/* SPR 5599_129119 Fix Start */
void restartAMROTimer( RxAMEntity* rxAMEntity_p)
/* SPR 5599_129119 Fix End */
{
    UInt32 errorCode = INVALID_ERR_CODE;

    if ( !rxAMEntity_p )
    {
        LOG_RLC_MSG(LTE_RLC_RESTART_AM_RO_TIMER_ERR, LOGWARNING,
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                0, 0, 0, 0, 0, 0, __func__, " RX_UM_NOT_EXIST");
        return ;
    }

    errorCode = stopAMROTimer( rxAMEntity_p);

  /* + SPR#5898 */
  if(INVALID_ERR_CODE != errorCode)
  /* - SPR#5898 */
  {
    
    LOG_RLC_MSG(LTE_RLC_RESTART_AM_RO_TIMER, LOGINFO, 
            RLC_TIMER, globalTTITickCount_g, __LINE__,
            rxAMEntity_p->ueIndex, rxAMEntity_p->lcId,
            globalTTITickCount_g, (rxAMEntity_p->amROTimerInfo.roDuration),
            (rxAMEntity_p->amROTimerInfo.roExpiryIdx), errorCode, __func__,
            "Stop AM_RO_TIMER Now Start UM RO Timer");
    }
    errorCode = startAMROTimer( rxAMEntity_p );
    /* + SPR#5898 */
    if(INVALID_ERR_CODE != errorCode)
   /* - SPR#5898 */
   {
    LOG_RLC_MSG(LTE_RLC_RESTART_AM_RO_TIMER_1, LOGINFO, 
            RLC_TIMER, globalTTITickCount_g, __LINE__,
            rxAMEntity_p->ueIndex, rxAMEntity_p->lcId,
            globalTTITickCount_g, (rxAMEntity_p->amROTimerInfo.roDuration),
            (rxAMEntity_p->amROTimerInfo.roExpiryIdx), errorCode, __func__,
            "AM_RO_TIMER_RE_STARTED");
   }
}





/************* End AM T Reordering Timer implementation ***********/

/****************************************************************************
 * Function Name  : startPRTimer
 * Inputs         : txAMEntity_p- Pointer to TxAMEntity
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE/errorCode
 * Description    : This functions starts PR Timer.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 startPRTimer(  TxAMEntity* txAMEntity_p) 
/* SPR 5599_129119 Fix End */
{
    RlcPRTm *prTimer_p           = PNULL;
    RlcTimerNode *rlcTimerNode_p = PNULL;
     UInt64 expiryIndex           = 0;
    UInt32 errorCode             = 0;
    /* SPR 15909 fix start */
    tickType_t tti                   = 0;
    /* SPR 15909 fix end */
  
    if ( PNULL == txAMEntity_p )
    {
        LOG_RLC_MSG(LTE_RLC_START_AM_PR_TIMER_ERR, LOGWARNING,
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                0, 0, 0, 0, 0, 0, __func__, " RX_AM_NOT_EXIST");
        return RLC_FAILURE;
    }
    
    prTimer_p = &(txAMEntity_p->prTimerInfo); 
    errorCode = isValidPRStartTimer ( prTimer_p);
 
    if ( RLC_SUCCESS != errorCode )
    {
        LOG_RLC_MSG(LTE_RLC_START_AM_PR_TIMER_ERR_1, LOGWARNING, 
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                txAMEntity_p->ueIndex, txAMEntity_p->lcId, errorCode,
                0, 0, 0, __func__, "AM_PR_TIMER Already Running");
        return errorCode;
    } 

/*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
    InternalCellIndex cellIndex = rlcUEContextList_g[txAMEntity_p->ueIndex]->cellIndex;
    tti = rlcGlobalTTITickCount_g[cellIndex];
#else
    tti = globalTTITickCount_g;
#endif
/*  - Layer2 NON CA Changes*/
    expiryIndex = ( prTimer_p->prDuration + tti) & MOD_VAL_T_POLL_RETRANSMIT_TIMER;
    
    if ( (rlcTimerNode_p =
           (RlcTimerNode *)getMemFromPool(sizeof(RlcTimerNode), RLC_NULL))
            == RLC_NULL)
    {
        LOG_RLC_MSG( LTE_RLC_START_AM_PR_TIMER_ERR_2, LOGWARNING, 
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                txAMEntity_p->ueIndex, txAMEntity_p->lcId, errorCode,
                expiryIndex, 0, 0, __func__, "MEM_FAILED");

        return RLC_FAILURE;
    }
    rlcTimerNode_p->ueIndex     = txAMEntity_p->ueIndex;
    rlcTimerNode_p->lcId        = txAMEntity_p->lcId;
    rlcTimerNode_p->isValid     = RLC_TIMER_NODE_VALID;
    /* SPR 11571 changes start */
    rlcTimerNode_p->pollSN      = txAMEntity_p->pollSN;
    /* SPR 11571 changes end */

    /* Store the reference in the AM TX Entity.*/
    prTimer_p->prTimerNode_p = rlcTimerNode_p;
    prTimer_p->prExpiryIdx   = expiryIndex;
    
/*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
    listInsertNode(&(RlcTPollRetransmitTimerMap_g[cellIndex][expiryIndex].subFrameList), 
                   &rlcTimerNode_p->nodeAnchor);
#else
    listInsertNode(&(RlcTPollRetransmitTimerMap_g[expiryIndex].subFrameList), 
                   &rlcTimerNode_p->nodeAnchor);
#endif
/*  - Layer2 NON CA Changes*/
    
    LOG_RLC_MSG( LTE_RLC_START_AM_PR_TIMER, LOGINFO, 
            RLC_TIMER, globalTTITickCount_g, __LINE__,
            txAMEntity_p->ueIndex, txAMEntity_p->lcId, globalTTITickCount_g,
            prTimer_p->prDuration, expiryIndex, 0, __func__, "AM_PR_TIMER Start");

    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : isValidPRStartTimer
 * Inputs         : prTimer_p - Pointer to RlcPRTm
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API checks whether PR Start Timer is valid or not.
 ****************************************************************************/

inline static UInt32 isValidPRStartTimer( RlcPRTm *prTimer_p)
{
    if (  INVALID_EXP_IDX != prTimer_p->prExpiryIdx )
    {
        return RLC_FAILURE;
    }

    if ( prTimer_p->prTimerNode_p)
    {
        return RLC_FAILURE;
    }

    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : stopPRTimer
 * Inputs         : txAMEntity_p - Pointer to TxAMEntity
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API stops Poll Retransmit Timer.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
UInt32 stopPRTimer( TxAMEntity* txAMEntity_p )
/* SPR 5599_129119 Fix End */
{
    RlcPRTm *prTimer_p           = PNULL;
    RlcTimerNode *rlcTimerNode_p = PNULL;

    if ( !txAMEntity_p )
    {
        LOG_RLC_MSG(LTE_RLC_STOP_AM_PR_TIMER_ERR, LOGWARNING,
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                0, 0, 0, 0, 0, 0, __func__, " RX_UM_NOT_EXIST");
        return RLC_FAILURE;
    }
    prTimer_p = &(txAMEntity_p->prTimerInfo);
    rlcTimerNode_p = prTimer_p->prTimerNode_p; 
    if ( !rlcTimerNode_p )
    {
        LOG_RLC_MSG(LTE_RLC_STOP_AM_PR_TIMER_ERR_1, LOGWARNING,
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                txAMEntity_p->ueIndex, txAMEntity_p->lcId,
                prTimer_p->prDuration, prTimer_p->prExpiryIdx,
                0, 0, __func__, "PR_TIMER_NODE_NOT_EXIST");

        return RLC_FAILURE;
    }

    /** SPR 13991 Changes Start **/
    if ( (INVALID_EXP_IDX == prTimer_p->prExpiryIdx) || (rlcTimerNode_p->isValid == INVALID_RLC_TIMER_NODE))
    /** SPR 13991 Changes End **/
    {
        LOG_RLC_MSG(LTE_RLC_STOP_AM_PR_TIMER_ERR_2, LOGWARNING,
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                txAMEntity_p->ueIndex, txAMEntity_p->lcId,
                prTimer_p->prDuration, prTimer_p->prExpiryIdx,
                0, 0, __func__, " PR_TIMER_IDX_INVALID");
        return RLC_FAILURE;
    }

    LOG_RLC_MSG( LTE_RLC_STOP_AM_PR_TIMER, LOGINFO,
            RLC_TIMER, globalTTITickCount_g, __LINE__,
            txAMEntity_p->ueIndex, txAMEntity_p->lcId, globalTTITickCount_g,
            prTimer_p->prDuration, prTimer_p->prExpiryIdx,
            0, __func__, "PR_TIMER Stop");
    /* SPR 4041 Fix Start */
    rlcTimerNode_p->isValid = INVALID_RLC_TIMER_NODE;
    /* SPR 4041 Fix End */
    prTimer_p->prTimerNode_p     = PNULL;
    prTimer_p->prExpiryIdx       = INVALID_EXP_IDX;

    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : expirePRTimer
 * Inputs         : cellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API handles expired PR Timers.
 ****************************************************************************/
/*  + Layer2 NON CA Changes*/
/* + Layer2 NON CA RVW CMNT */ 
inline static void  expirePRTimer(
#ifndef UE_SIM_TESTING
        InternalCellIndex cellIndex
#endif
)
/* - Layer2 NON CA RVW CMNT */ 
/** CA Changes End **/
{
    UInt64 expiryIndex = 0;
    UInt16 ueIndex     = 0;
    UInt8  lcId        = 0;
    UInt8  isValid     = 0;
    /* SPR 15909 fix start */
    tickType_t currentTTI  = 0;
    /* SPR 15909 fix end */
    UInt64 prevTick    = 0;

    RLCEntityInfo* eInfo_p       = PNULL;
    UEContext* ueContext_p       = PNULL;
    RlcTimerNode *rlcTimerNode_p = PNULL;
    RlcTimerNode *nextNode_p     = PNULL;
    RlcPRTm     *prTimer_p       = PNULL;
    TxAMEntity* txAMEntity_p     = PNULL;
/*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
    currentTTI  = rlcGlobalTTITickCount_g[cellIndex];
    /*SPR 15909 Fix Start*/
    for ( prevTick = lastSchedulePRTimer_g[cellIndex] + 1;
    prevTick < ( currentTTI+1); prevTick++)
    /*SPR 15909 Fix end*/
#else
    currentTTI = globalTTITickCount_g;
    /* SPR 15909 fix start*/
    for ( prevTick = lastSchedulePRTimer_g + 1;
            prevTick < (currentTTI+1); prevTick++)
        /* SPR 15909 fix end*/
#endif
/*  - Layer2 NON CA Changes*/
  {
      expiryIndex = (prevTick) & MOD_VAL_T_POLL_RETRANSMIT_TIMER;

/*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
      rlcTimerNode_p = ( RlcTimerNode *) 
          getListNode (&(RlcTPollRetransmitTimerMap_g[cellIndex][expiryIndex].subFrameList), 0);
#else
      rlcTimerNode_p = ( RlcTimerNode *) 
          getListNode (&(RlcTPollRetransmitTimerMap_g[expiryIndex].subFrameList), 0);
#endif
/*  - Layer2 NON CA Changes*/

      while ( PNULL != rlcTimerNode_p )
      {
          ueIndex = rlcTimerNode_p->ueIndex;
          lcId    = rlcTimerNode_p->lcId;
          isValid = rlcTimerNode_p->isValid;

          nextNode_p = (RlcTimerNode *)getNextListNode((void *)rlcTimerNode_p);

/*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
          listDeleteNode(&(RlcTPollRetransmitTimerMap_g[cellIndex][expiryIndex].subFrameList),
                  &(rlcTimerNode_p->nodeAnchor));
#else          
          listDeleteNode(&(RlcTPollRetransmitTimerMap_g[expiryIndex].subFrameList),
                  &(rlcTimerNode_p->nodeAnchor));
#endif
/*  - Layer2 NON CA Changes*/

          if ( RLC_TIMER_NODE_VALID != isValid)
          {
              /** SPR 13991 Changes Start **/
              freeMemPool((void *)rlcTimerNode_p);
              rlcTimerNode_p = PNULL;
              rlcTimerNode_p = nextNode_p;
              /** SPR 13991 Changes End **/
              continue;
          }
          /** SPR 13991 Changes Start **/
          else
          {
              rlcTimerNode_p->isValid = INVALID_RLC_TIMER_NODE;
          }
          /** SPR 13991 Changes End **/

          /* Fetch the AM Tx Entity */
		  /* SPR#11313 Fix changes	  */  
		  if ((( ueContext_p = DB_FETCH_UE_CONTEXT(ueIndex)) == RLC_NULL)
					|| ( ueContext_p->rlcUEState != RLC_UECONTEXT_ACTIVE ))
		  /* SPR#11313 Fix changes	  */

          {
              LOG_RLC_MSG(LTE_RLC_EXPIRE_AM_PR_TIMER_ERR_1, LOGWARNING, 
                      RLC_TIMER, globalTTITickCount_g, __LINE__,
                      ueIndex, lcId, 0, 0, 0, 0, __func__,
                      " UE_CTX_NOT_EXIST");
              /** SPR 13991 Changes Start **/
              freeMemPool((void *)rlcTimerNode_p);
              rlcTimerNode_p = PNULL;
              rlcTimerNode_p = nextNode_p;
              /** SPR 13991 Changes End **/
              continue;
          }
          eInfo_p = DB_FETCH_RLC_ENTITY_INFO(ueContext_p, lcId);
          if  ( eInfo_p == RLC_NULL )         
          {
              LOG_RLC_MSG(LTE_RLC_EXPIRE_AM_PR_TIMER_ERR_2, LOGWARNING, 
                      RLC_TIMER, globalTTITickCount_g, __LINE__,
                      ueIndex, lcId, 0, 0, 0, 0, __func__,
                      "RX_AM_ENTITY_NOT_EXIST");
              /** SPR 13991 Changes Start **/
              freeMemPool((void *)rlcTimerNode_p);
              rlcTimerNode_p = PNULL;
              rlcTimerNode_p = nextNode_p;
              /** SPR 13991 Changes End **/
              continue;

          }
            /*SPR 16855 +-*/
            /*+ SPR_17858_684_CHANGES*/
            if ( eInfo_p->rlcEntityMode == RLC_ENTITY_AM )
            {
                txAMEntity_p = (eInfo_p->rlcEntity.amEntity_p->txAMEntity_p);
                prTimer_p =  &(txAMEntity_p->prTimerInfo);
                prTimer_p->prTimerNode_p = PNULL;
                prTimer_p->prExpiryIdx = INVALID_EXP_IDX;
                txAMEntity_p->istRetransmitTimerRunning = RLC_FALSE;
            }
            /*- SPR_17858_684_CHANGES*/

#ifndef UE_SIM_TESTING
          if(eInfo_p->rlcEntityMode != RLC_ENTITY_AM)
          {
              /** SPR 13991 Changes Start **/
              freeMemPool((void *)rlcTimerNode_p);
              rlcTimerNode_p = PNULL;
              rlcTimerNode_p = nextNode_p;
              /** SPR 13991 Changes End **/
              continue;
          }
          if(eInfo_p->rlcEntity.amEntity_p->txEntityState != RLC_ENTITY_ACTIVE)
          {
              LOG_RLC_MSG(LTE_RLC_EXPIRE_AM_PR_TIMER_ERR_3, LOGWARNING, 
                      RLC_TIMER, globalTTITickCount_g, __LINE__,
                      ueIndex, lcId, eInfo_p->rlcEntity.amEntity_p->txEntityState,
                      eInfo_p->rlcEntityMode, 0, 0, __func__,
                      "RX_AM_ENTITY_NOT_EXIST");
              /** SPR 13991 Changes Start **/
              freeMemPool((void *)rlcTimerNode_p);
              rlcTimerNode_p = PNULL;
              rlcTimerNode_p = nextNode_p;
              /** SPR 13991 Changes End **/
              continue;
          }                
#else
          if ( ( eInfo_p->rlcEntityState != RLC_ENTITY_ACTIVE ) ||
                  ( eInfo_p->rlcEntityMode != RLC_ENTITY_AM )
             )
          {
              LOG_RLC_MSG(LTE_RLC_EXPIRE_AM_PR_TIMER_ERR_3, LOGWARNING, 
                      RLC_TIMER, globalTTITickCount_g, __LINE__,
                      ueIndex, lcId, eInfo_p->rlcEntityState,
                      eInfo_p->rlcEntityMode, 0, 0, __func__,
                      "RX_AM_ENTITY_NOT_EXIST");
              /** SPR 13991 Changes Start **/
              freeMemPool((void *)rlcTimerNode_p);
              rlcTimerNode_p = PNULL;
              rlcTimerNode_p = nextNode_p;
              /** SPR 13991 Changes End **/
              continue;
          }
#endif          
          txAMEntity_p = (eInfo_p->rlcEntity.amEntity_p->txAMEntity_p);
          prTimer_p =  &(txAMEntity_p->prTimerInfo);

          LOG_RLC_MSG(LTE_RLC_EXPIRE_AM_PR_TIMER, LOGINFO, 
                  RLC_TIMER, globalTTITickCount_g, __LINE__,
                  ueIndex, lcId, globalTTITickCount_g, prTimer_p->prDuration,
                  prTimer_p->prExpiryIdx, 0, __func__,
                  "AM_PR_TIMER Expired");

          prTimer_p->prTimerNode_p = PNULL;
          prTimer_p->prExpiryIdx = INVALID_EXP_IDX;
          /** SPR 13991 Changes Start **/
          freeMemPool((void *)rlcTimerNode_p);
          rlcTimerNode_p = PNULL;
          rlcTimerNode_p = nextNode_p;
          /** SPR 13991 Changes End **/

          rlcActionOnExpiryOfPollRetransmit( txAMEntity_p);
          txAMEntity_p->istRetransmitTimerRunning = RLC_FALSE;
      }
  }
/*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
    lastSchedulePRTimer_g[cellIndex] = currentTTI; 
#else
  lastSchedulePRTimer_g = currentTTI; 
#endif
/*  - Layer2 NON CA Changes*/
}

/****************************************************************************
 * Function Name  : restartPRTimer
 * Inputs         : txAMEntity_p - Pointer to TxAMEntity
 * Outputs        : None
 * Returns        : None
 * Description    : This API restarts Poll Retransmit Timer.
 ****************************************************************************/
/* SPR 5599_129119 Fix Start */
void restartPRTimer(  TxAMEntity* txAMEntity_p)
/* SPR 5599_129119 Fix End */
{
    UInt32 errorCode = INVALID_ERR_CODE;
   
    if ( !txAMEntity_p )
    {
        LOG_RLC_MSG(LTE_RLC_RESTART_AM_PR_TIMER_ERR, LOGWARNING,
                RLC_TIMER, globalTTITickCount_g, __LINE__,
                0, 0, 0, 0, 0, 0, __func__, " RX_UM_NOT_EXIST");

        return ;
    } 
    errorCode = stopPRTimer ( txAMEntity_p);
    /* + SPR#5898 */
    if(INVALID_ERR_CODE != errorCode)
    /* - SPR#5898 */
    {   	
    LOG_RLC_MSG(LTE_RLC_RESTART_AM_PR_TIMER, LOGINFO, 
            RLC_TIMER, globalTTITickCount_g, __LINE__,
            txAMEntity_p->ueIndex, txAMEntity_p->lcId,
            globalTTITickCount_g, (txAMEntity_p->prTimerInfo.prDuration),
            (txAMEntity_p->prTimerInfo.prExpiryIdx), errorCode, __func__,
            "Stop AM_RO_TIMER Now Start UM RO Timer");
    }
    errorCode = startPRTimer( txAMEntity_p);
    /* + SPR#5898 */
    if(INVALID_ERR_CODE != errorCode)
    /* - SPR#5898 */
    {   	
    LOG_RLC_MSG(LTE_RLC_RESTART_AM_PR_TIMER_1, LOGINFO, 
            RLC_TIMER, globalTTITickCount_g, __LINE__,
            txAMEntity_p->ueIndex, txAMEntity_p->lcId,
            globalTTITickCount_g, (txAMEntity_p->prTimerInfo.prDuration),
            (txAMEntity_p->prTimerInfo.prExpiryIdx), errorCode, __func__,
            "AM_RO_TIMER_RE_STARTED");
    }
}


/***********************************************************************************/
/*
Reference 3GPP T.S 36.322 Section 5.2.2.3

Expiry of t-PollRetransmit
Upon expiry of t-PollRetransmit, the transmitting side of an AM RLC entity shall:
-   if both the transmission buffer and the retransmission buffer are empty 
    (excluding transmitted RLC data PDU awaiting for acknowledgements); or
-   if no new RLC data PDU can be transmitted (e.g. due to window stalling):
-   consider the AMD PDU with SN = VT(S)- 1 for retransmission; or
-   consider any AMD PDU which has not been positively acknowledged for retransmission;
-   include a poll in a RLC data PDU as described in section 5.2.2.1.
*/

/****************************************************************************
 * Function Name  : rlcActionOnExpiryOfPollRetransmit 
 * Inputs         : pAmTxE - Tx Am Entity pointer
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This function return the poll bit value for Rx-Tx case.
 ****************************************************************************/

/* SPR 5599_129117 Fix Start */
UInt32 rlcActionOnExpiryOfPollRetransmit(TxAMEntity* pAmTxE)
/* SPR 5599_129117 Fix End */
{
    UInt32 ret   = RLC_SUCCESS;
   
    AmNackListNode *amNackList_p  = RLC_NULL; 
    AmReTransmission *amReTxPdu_p = RLC_NULL;
    RlcUeDataReq * amdRawSdu_p    = RLC_NULL ;
    AmdRawSduQ *amdRawSduQ_p      = &pAmTxE->txSDUQueue;
    
    UInt32 Sostart      = 0;
    UInt32 Soend        = 0;
    LCQueueLoad lcQLoad = {0};

    RlcVr nackSn = 0;
    RlcVr vtA    = pAmTxE->vtA;
    RlcVr vtMS   = pAmTxE->vtMS;
    RlcVr vtS    = pAmTxE->vtS ;

   nackSn = (vtS - 1) & 1023;
/*    
-   if both the transmission buffer and the retransmission buffer are empty 
    (excluding transmitted RLC data PDU awaiting for acknowledgements); or
-   if no new RLC data PDU can be transmitted (e.g. due to window stalling):
*/
    if ( (( 0 == pAmTxE->txQueueSize) && (0 == pAmTxE->reTxQueueSize) ) ||
            (!( isAMLTE( vtA, vtS, vtA) && isAMLT (vtS, vtMS, vtA) )))
    {
        /*-   consider the AMD PDU with SN = VT(S)- 1 for retransmission; */
        nackSn =  nackSn & 511 ;
        amReTxPdu_p = pAmTxE->reTxQueue[nackSn];
        /*consider the AMD PDU with SN = VT(S) - 1 for retransmission; or*/
        if(amReTxPdu_p)
        {
	    /*+COVERITY 5.3.0 - 32643*/
            //QUEUE_COUNT_AM_NACK_LIST_Q(pAmTxE->nackList);
	    /*-COVERITY 5.3.0 - 32643*/
            CREATE_AM_NACK_LIST_NODE(amNackList_p,amReTxPdu_p->seqNum,Sostart,Soend);

            if(amNackList_p == RLC_NULL)
            {
                return RLC_FAILURE;
            }

            /*** Coverity 25578 Fix : Start ****/
            if (!ENQUEUE_AMD_NACK_LIST_Q(pAmTxE->nackList,amNackList_p))
            {
                freeMemPool(amNackList_p);
				/* + Coverity 70894 */
                //amNackList_p = RLC_NULL;
				/* - Coverity 70894 */
                return RLC_FAILURE;
            }
#ifdef DEBUG_STATS
            pAmTxE->prTimerExpiryCount++;
#endif

            /*** Coverity 25578 Fix : End ****/

            /*If this will data size the we will miss the header in tx opp*/
	    /*SPR 21770 Changes start*/
	    atomicAddInt(&pAmTxE->reTxQueueSize,amReTxPdu_p->pdu_size);
	    /*SPR 21770 Changes end*/
            lcQLoad.lcId = pAmTxE->lcId;
            
            rlcFetchQueueLoad(pAmTxE,&lcQLoad);
            lcQLoad.scheduledDataBytes = 0;

            /*******  Qos *****/    
            /* SPR 12610 Start */
            if( amdRawSduQ_p->pQ->count > 0)
            {
            amdRawSdu_p  = (RlcUeDataReq *)(amdRawSduQ_p->pQ->ring[amdRawSduQ_p->pQ->head].data);
                lcQLoad.headPktTS = amdRawSdu_p->pktArrTime;
            }
            else 
            {
                lcQLoad.headPktTS = 0;
            }
            /* SPR 12610 End */
            /***********************************************/    
            lcQLoad.ctrlFlag = RLC_NORMAL_DATA;
            /* notify MAC layer of queue load */
            macUpdateBufferOccupancyReq(pAmTxE->ueIndex, 1 /*lcCount*/, &lcQLoad, RLC_BO_TYPE_NEW,
            rlcUEContextList_g[pAmTxE->ueIndex ]->cellIndex);

        }
        else
        {

        }
    }
    pAmTxE->istRetransmitTimerRunning = RLC_FALSE;
    /*include a poll in a RLC data PDU as described in section 5.2.2.1.*/
    pAmTxE->pollBit = 0x01;
    
    return ret ;
}





/****************************************************************************
 * Function Name  : rlcInitTimerInterface
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : initializes Timer Interface.
 ****************************************************************************/
/* COVERITY CID 62547 FIX START <nesting indent mismatch>*/
/* + SPR 17439 */
void rlcInitTimerInterface(void)
/* - SPR 17439 */    
{
   UInt32 subFrameOffset = 0;
 
    /* Start 128 UE: Memory Changes */ 
    /* Moved to createRLCMemPools */
    /* End 128 UE: Memory Changes */

#ifndef UE_SIM_TESTING
    /*  + Layer2 NON CA Changes*/
    InternalCellIndex cellCntr = 0;
    for(cellCntr  = 0 ;cellCntr < numOfRlcCellConfigured_g; cellCntr++)
        /*  - Layer2 NON CA Changes*/
    {
#endif
    for (subFrameOffset = 0; subFrameOffset < MAX_T_REORDERING_TIMER;
                                                        subFrameOffset++)
    {
            /*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
            listInit(&RlcAMTReorderingTimerMap_g[cellCntr][subFrameOffset].subFrameList);
            listInit(&RlcUMTReorderingTimerMap_g[cellCntr][subFrameOffset].subFrameList);
            listInit(&SPTimerMap_g[cellCntr][subFrameOffset].subFrameList);
            listInit(&RlcTPollRetransmitTimerMap_g[cellCntr][subFrameOffset].subFrameList);
#else
        listInit(&RlcAMTReorderingTimerMap_g[subFrameOffset].subFrameList);
        listInit(&RlcUMTReorderingTimerMap_g[subFrameOffset].subFrameList);
        listInit(&SPTimerMap_g[subFrameOffset].subFrameList);
        listInit(&RlcTPollRetransmitTimerMap_g[subFrameOffset].subFrameList);
#endif
            /*  - Layer2 NON CA Changes*/
    }
    for ( ; subFrameOffset < MAX_T_STATUS_PROHIBIT_TIMER;
                                                        subFrameOffset++)
    {
            /*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
            listInit(&SPTimerMap_g[cellCntr][subFrameOffset].subFrameList);
            listInit(&RlcTPollRetransmitTimerMap_g[cellCntr][subFrameOffset].subFrameList);
#else
            listInit(&SPTimerMap_g[subFrameOffset].subFrameList);
            listInit(&RlcTPollRetransmitTimerMap_g[subFrameOffset].subFrameList);
#endif           

        }
#ifndef UE_SIM_TESTING
    }
    /*  - Layer2 NON CA Changes*/
#endif
    /* + SPR 9911 Fix */
#ifndef UE_SIM_TESTING 
    for(cellCntr  = 0 ;cellCntr < numOfRlcCellConfigured_g; cellCntr++)
    {
        INIT_RLC_TRIGGER_STATUS_REPORT_Q(cellCntr);
    }
#endif
    /* - SPR 9911 Fix */
}
/* COVERITY CID 62547 FIX END*/

/*  + Layer2 NON CA Changes*/
#ifdef UE_SIM_TESTING
/*  - Layer2 NON CA Changes*/
/****************************************************************************
 * Function Name  : rlcDeInitTimerInterface
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : de-initializes Timer Interface.
 ****************************************************************************/
void rlcDeInitTimerInterface()
{
    UInt32 count              = 0;
    UInt32 subFrameOffset     = 0;
    LTE_LIST_NODE *listNode_p = PNULL;
    for ( subFrameOffset = 0; subFrameOffset < MAX_T_REORDERING_TIMER; subFrameOffset++)
    {
        count = listCount(&( RlcAMTReorderingTimerMap_g[subFrameOffset].subFrameList));
        while(count) 
        {
            listNode_p = getListNode(&RlcAMTReorderingTimerMap_g[subFrameOffset].subFrameList, 0);
            if(listNode_p) 
            {
                listDeleteNode(&(RlcAMTReorderingTimerMap_g[subFrameOffset].subFrameList), listNode_p);
                freeMemPool(listNode_p);
                listNode_p = PNULL;
            }
            count--;
        }

        count = listCount(&(RlcUMTReorderingTimerMap_g[subFrameOffset].subFrameList));
        while(count)
        {
            listNode_p = getListNode(&RlcUMTReorderingTimerMap_g[subFrameOffset].subFrameList, 0);
            if(listNode_p)
            {
                listDeleteNode(&(RlcUMTReorderingTimerMap_g[subFrameOffset].subFrameList), listNode_p);
                freeMemPool(listNode_p);
                listNode_p = PNULL;
            }
            count--;
        }

        count = listCount(&(SPTimerMap_g[subFrameOffset].subFrameList));
        while(count)
        {
            listNode_p = getListNode(&SPTimerMap_g[subFrameOffset].subFrameList, 0);
            if(listNode_p)
            {
                listDeleteNode(&(SPTimerMap_g[subFrameOffset].subFrameList), listNode_p);
                freeMemPool(listNode_p);
                listNode_p = PNULL;
            }
            count--;
        }


        count = listCount(&(RlcTPollRetransmitTimerMap_g[subFrameOffset].subFrameList));
        while(count)
        {
            listNode_p = getListNode(& RlcTPollRetransmitTimerMap_g[subFrameOffset].subFrameList, 0);
            if(listNode_p)
            {
                listDeleteNode(&(RlcTPollRetransmitTimerMap_g[subFrameOffset].subFrameList), listNode_p);
                freeMemPool(listNode_p);
                listNode_p = PNULL;
            }
            count--;
        }


    }

    for ( ; subFrameOffset < MAX_T_STATUS_PROHIBIT_TIMER; subFrameOffset++)
    {
        count = listCount(&(SPTimerMap_g[subFrameOffset].subFrameList));
        while(count)
        {
            listNode_p = getListNode(& SPTimerMap_g[subFrameOffset].subFrameList, 0);
            if(listNode_p)
            {
                listDeleteNode(&( SPTimerMap_g[subFrameOffset].subFrameList), listNode_p);
                freeMemPool(listNode_p);
                listNode_p = PNULL;
            }
            count--;
        }

        count = listCount(&(RlcTPollRetransmitTimerMap_g[subFrameOffset].subFrameList));
        while(count)
        {
            listNode_p = getListNode(& RlcTPollRetransmitTimerMap_g[subFrameOffset].subFrameList, 0);
            if(listNode_p)
            {
                listDeleteNode(&(RlcTPollRetransmitTimerMap_g[subFrameOffset].subFrameList), listNode_p);
                freeMemPool(listNode_p);
                listNode_p = PNULL;
            }
            count--;
        }
    }
    /* + SPR 9911 Fix */
#ifndef UE_SIM_TESTING 
    QDEINIT_RLC_TRIGGER_STATUS_REPORT_Q();
#endif
    /* - SPR 9911 Fix */
}

/* + SPR 9911 Fix */
#else
/****************************************************************************
 * Function Name  : rlcDeInitTimerInterface
 * Inputs         : 
 * Outputs        :
 * Returns        : void
 * Description    : de-initializes Timer Interface .
 ****************************************************************************/
/* + SPR 17439 */
void rlcDeInitTimerInterface(void)
/* - SPR 17439 */    
{
    UInt32 count              = 0;
    UInt32 subFrameOffset     = 0;
    LTE_LIST_NODE *listNode_p = PNULL;
    InternalCellIndex cellCntr = 0;

    for(cellCntr  = 0 ;cellCntr < numOfRlcCellConfigured_g; cellCntr++)
    {
        for ( subFrameOffset = 0; subFrameOffset < MAX_T_REORDERING_TIMER; subFrameOffset++)
        {
            count = listCount(&( RlcAMTReorderingTimerMap_g[cellCntr][subFrameOffset].subFrameList));
            while(count) 
            {
                listNode_p = getListNode(&RlcAMTReorderingTimerMap_g[cellCntr][subFrameOffset].subFrameList, 0);
                if(listNode_p) 
                {
                    listDeleteNode(&(RlcAMTReorderingTimerMap_g[cellCntr][subFrameOffset].subFrameList), listNode_p);
                    freeMemPool(listNode_p);
                    listNode_p = PNULL;
                }
                count--;
            }

            count = listCount(&(RlcUMTReorderingTimerMap_g[cellCntr][subFrameOffset].subFrameList));
            while(count)
            {
                listNode_p = getListNode(&RlcUMTReorderingTimerMap_g[cellCntr][subFrameOffset].subFrameList, 0);
                if(listNode_p)
                {
                    listDeleteNode(&(RlcUMTReorderingTimerMap_g[cellCntr][subFrameOffset].subFrameList), listNode_p);
                    freeMemPool(listNode_p);
                    listNode_p = PNULL;
                }
                count--;
            }

            count = listCount(&(SPTimerMap_g[cellCntr][subFrameOffset].subFrameList));
            while(count)
            {
                listNode_p = getListNode(&SPTimerMap_g[cellCntr][subFrameOffset].subFrameList, 0);
                if(listNode_p)
                {
                    listDeleteNode(&(SPTimerMap_g[cellCntr][subFrameOffset].subFrameList), listNode_p);
                    freeMemPool(listNode_p);
                    listNode_p = PNULL;
                }
                count--;
            }


            count = listCount(&(RlcTPollRetransmitTimerMap_g[cellCntr][subFrameOffset].subFrameList));
            while(count)
            {
                listNode_p = getListNode(& RlcTPollRetransmitTimerMap_g[cellCntr][subFrameOffset].subFrameList, 0);
                if(listNode_p)
                {
                    listDeleteNode(&(RlcTPollRetransmitTimerMap_g[cellCntr][subFrameOffset].subFrameList), listNode_p);
                    freeMemPool(listNode_p);
                    listNode_p = PNULL;
                }
                count--;
            }


        }

        for ( ; subFrameOffset < MAX_T_STATUS_PROHIBIT_TIMER; subFrameOffset++)
        {
            count = listCount(&(SPTimerMap_g[cellCntr][subFrameOffset].subFrameList));
            while(count)
            {
                listNode_p = getListNode(& SPTimerMap_g[cellCntr][subFrameOffset].subFrameList, 0);
                if(listNode_p)
                {
                    listDeleteNode(&( SPTimerMap_g[cellCntr][subFrameOffset].subFrameList), listNode_p);
                    freeMemPool(listNode_p);
                    listNode_p = PNULL;
                }
                count--;
            }

            count = listCount(&(RlcTPollRetransmitTimerMap_g[cellCntr][subFrameOffset].subFrameList));
            while(count)
            {
                listNode_p = getListNode(& RlcTPollRetransmitTimerMap_g[cellCntr][subFrameOffset].subFrameList, 0);
                if(listNode_p)
                {
                    listDeleteNode(&(RlcTPollRetransmitTimerMap_g[cellCntr][subFrameOffset].subFrameList), listNode_p);
                    freeMemPool(listNode_p);
                    listNode_p = PNULL;
                }
                count--;
            }
        }
    }
    /* + SPR 9911 Fix */
    for(cellCntr  = 0 ;cellCntr < numOfRlcCellConfigured_g; cellCntr++)
        /*  - Layer2 NON CA Changes*/
    {
        QDEINIT_RLC_TRIGGER_STATUS_REPORT_Q(cellCntr);
    }
}
/*  - Layer2 NON CA Changes*/
/* + SPR 9911 Fix */
/*****************************************************************************
 * Function Name  : putEntryInRlcTrigStatusReportQueue
 * Inputs         : ueIndex - UE ID, 
 *                  lcId - LC ID,
 * Outputs        : None
 * Returns        : None 
 * Description    : This function will put an entry in rlcTriggerStatusReportQueue_g queue.
 *****************************************************************************/
void putEntryInRlcTrigStatusReportQueue(UInt16 ueIndex, UInt8 lcId,InternalCellIndex cellIndex)
{
	RlcTriggerStatusReportNode *rlcTriggerStatusReportNode_p = PNULL;

	GET_MEM_NODE_RLC_TRIGGER_STATUS_REPORT_Q(rlcTriggerStatusReportNode_p,RlcTriggerStatusReportNode);

	if( PNULL != rlcTriggerStatusReportNode_p )
	{
		rlcTriggerStatusReportNode_p->ueIndex = ueIndex;
		rlcTriggerStatusReportNode_p->lcId = lcId;
		if(RLC_FAILURE == ENQUEUE_RLC_TRIGGER_STATUS_REPORT_Q(cellIndex,
                                           &(rlcTriggerStatusReportNode_p->nodeAnchor)))
                {
                     FREE_MEM_NODE_RLC_TRIGGER_STATUS_REPORT_Q(rlcTriggerStatusReportNode_p);
                }
                /* Pls check Enqueue , if Enqueue Fails,free the node */
        }
}

/*****************************************************************************
 * Function Name  : processRlcTriggerStatusReport
 * Inputs         : statusReportsQueueCount
 * Outputs        : None
 * Returns        : None 
 * Description    : This function will fetch the nodes from the global queue 
 *                  rlcTriggerStatusReportQueue_g, and call the function 
 *                  rlcTriggerStatusReport() for each valid node, in order to 
 *                  trigger the status Pdu tx request.
 *****************************************************************************/
void processRlcTriggerStatusReport(UInt32 statusReportsQueueCount,InternalCellIndex cellIndex)
{
	RlcTriggerStatusReportNode *rlcTriggerStatusReportNode_p = PNULL;
	RLCEntityInfo* eInfo_p       = PNULL;
	UEContext* ueContext_p       = PNULL;
        UInt16 ueIndex = 0;
        UInt8 lcId = 0;

	while(statusReportsQueueCount--)
	{
            if(RLC_FAILURE == DEQUEUE_RLC_TRIGGER_STATUS_REPORT_Q(cellIndex,
                                      (void**)&rlcTriggerStatusReportNode_p))
            {
                  if(PNULL != rlcTriggerStatusReportNode_p)
                  FREE_MEM_NODE_RLC_TRIGGER_STATUS_REPORT_Q(rlcTriggerStatusReportNode_p);
                  
                  continue;
            }

	    if(rlcTriggerStatusReportNode_p)
	    {
                ueIndex = rlcTriggerStatusReportNode_p->ueIndex;
                lcId = rlcTriggerStatusReportNode_p->lcId;

		/* SPR#11313 Fix changes	*/	
		if ((( ueContext_p = DB_FETCH_UE_CONTEXT(ueIndex)) == RLC_NULL)
					|| ( ueContext_p->rlcUEState != RLC_UECONTEXT_ACTIVE ))
		/* SPR#11313 Fix changes	*/

		{
                	LOG_RLC_MSG(LTE_RLC_EXPIRE_SP_TIMER_ERR_1, LOGWARNING, 
				RLC_TIMER, globalTTITickCount_g, __LINE__,
				ueIndex, lcId, 0, 0, 0, 0, __func__,
		  		"");
                        FREE_MEM_NODE_RLC_TRIGGER_STATUS_REPORT_Q(rlcTriggerStatusReportNode_p);
                        continue;
		}
                
                eInfo_p = DB_FETCH_RLC_ENTITY_INFO(ueContext_p,lcId);
                if ( eInfo_p == RLC_NULL  )                
		{
	        	LOG_RLC_MSG(LTE_RLC_EXPIRE_SP_TIMER_ERR_2, LOGWARNING, 
				RLC_TIMER, globalTTITickCount_g, __LINE__,
				ueIndex, lcId, 0, 0, 0, 0, __func__,
				"");
                        FREE_MEM_NODE_RLC_TRIGGER_STATUS_REPORT_Q(rlcTriggerStatusReportNode_p);
	                continue;
		}
                if( eInfo_p->rlcEntity.amEntity_p->rxEntityState != RLC_ENTITY_ACTIVE)
                {
       			LOG_RLC_MSG(LTE_RLC_EXPIRE_SP_TIMER_ERR_3, LOGWARNING, 
				RLC_TIMER, globalTTITickCount_g, __LINE__,
				ueIndex, lcId, eInfo_p->rlcEntity.amEntity_p->rxEntityState ,
				eInfo_p->rlcEntityMode, 0, 0, __func__,
				"");

                        FREE_MEM_NODE_RLC_TRIGGER_STATUS_REPORT_Q(rlcTriggerStatusReportNode_p);
                        continue;
                }
                rlcTriggerStatusReport(eInfo_p->rlcEntity.amEntity_p);
                FREE_MEM_NODE_RLC_TRIGGER_STATUS_REPORT_Q(rlcTriggerStatusReportNode_p);
            }
       }
}
#endif
/* - SPR 9911 Fix */


/* SPR 11571 changes start */
/*****************************************************************************
* Function Name  : getPRRunningTimerPollSN
* Inputs         : TxAMEntity* txAMEntity_p
*                  UInt16    * runningPollSN_p
* Outputs        : UInt32
* Returns        : UInt32
* Description    : This function gets the pollSN of the PRRunning timer
*****************************************************************************/

UInt32 getPRRunningTimerPollSN(TxAMEntity* txAMEntity_p, UInt16* runningPollSN_p)
{
    RlcPRTm *prTimer_p           = PNULL;
    RlcTimerNode *rlcTimerNode_p = PNULL;

    if ( !txAMEntity_p )
    {
        return RLC_FAILURE;
    }
    prTimer_p = &(txAMEntity_p->prTimerInfo);
    rlcTimerNode_p = prTimer_p->prTimerNode_p;
    if ( !rlcTimerNode_p || (rlcTimerNode_p->isValid != RLC_TIMER_NODE_VALID) )
    {
        return RLC_FAILURE;
    }

    if ( INVALID_EXP_IDX == prTimer_p->prExpiryIdx)
    {
        return RLC_FAILURE;
    }

    *runningPollSN_p = rlcTimerNode_p->pollSN;

    return RLC_SUCCESS;
}
/* SPR 11571 changes end */
