/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacRandomAccess.c,v 1.2.6.1.4.2.2.2.10.1 2010/11/09 13:32:55 gur10121 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the function defination for Random
 *                     access procedure
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacRandomAccess.c,v $
 * Revision 1.2.6.1.4.2.2.2.10.1  2010/11/09 13:32:55  gur10121
 * processAdvancePUCCHAndPRACHReports second parameter changed now introduced the delay
 *
 * Revision 1.2.6.1.4.2.2.2  2010/10/11 12:57:05  gur10121
 * SPR #706 fix
 *
 * Revision 1.2.6.1.4.2.2.1.4.1  2010/10/08 07:42:49  gur10121
 *  SPR 706 Fix - KlockWork Warnings removed
 *
 * Revision 1.2.6.1.4.2.2.1  2010/09/21 15:46:18  gur20491
 * FAPI changes
 *
 * Revision 1.2.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.51  2009/12/31 09:11:29  gur19413
 * warnings removed
 *
 * Revision 1.50  2009/12/30 16:07:30  gur15697
 * comments incorporated
 *
 * Revision 1.49  2009/08/26 11:54:22  gur12140
 * Optimzed For DCI CCE Array
 *
 * Revision 1.48  2009/08/21 05:52:28  gur20439
 * SIT bug fix
 *
 * Revision 1.47  2009/08/17 14:57:31  gur20439
 * backoff indicator bug fix.
 *
 * Revision 1.46  2009/08/06 14:41:52  gur12140
 * MemSet the UE Specific Search Space
 *
 * Revision 1.45  2009/08/03 08:20:24  gur20052
 * Fix for UT_LOGS
 *
 * Revision 1.44  2009/07/31 12:01:05  gur15697
 * splint warnings
 *
 * Revision 1.43  2009/07/29 09:14:33  gur15697
 * UT bugfix
 *
 * Revision 1.42  2009/07/28 09:47:23  gur15697
 * UT logs added, splint warnings removed
 *
 * Revision 1.41  2009/07/26 08:16:15  gur15697
 * compilation warnings removed
 *
 * Revision 1.40  2009/07/21 15:16:18  gur15697
 * UT bugfix
 *
* Revision 1.39  2009/07/20 15:35:19  gur15697
* UT bugfix
*
* Revision 1.38  2009/07/19 12:34:02  gur15697
* rarMsgInfo_p->cceIndex  = cceAllocated added
*
* Revision 1.37  2009/07/17 05:45:15  gur15697
* UT bugfix
*
* Revision 1.36  2009/07/13 12:07:23  gur15697
* second argument to LTE_MAC_UT_LOG modifed
*
* Revision 1.35  2009/07/09 03:50:28  gur15697
* maxNumOfUeSupported_g replaced with MAX_UE_SUPPORTED
*
* Revision 1.34  2009/07/08 15:15:42  gur15697
* extern UInt16 maxNumOfUeSupported_g added
*
* Revision 1.33  2009/07/06 04:22:03  gur15697
* UT bug fixes
*
* Revision 1.32  2009/07/04 10:52:26  gur15697
* UT bug fixes
*
* Revision 1.31  2009/07/04 08:40:47  gur15697
* UT bug fixes
*
* Revision 1.30  2009/07/01 06:42:51  gur15697
* processAdvancePUCCHAndPRACHReports() arguments modifed
*
* Revision 1.29  2009/07/01 06:19:52  gur15697
* LTE_LOG changed to LTE_MAC_UT_LOG
*
* Revision 1.28  2009/07/01 04:25:21  gur15697
* review comments incorporated
*
* Revision 1.27  2009/06/29 06:14:53  gur15697
* UT Bug fix
*
* Revision 1.26  2009/06/25 07:41:26  gur15697
* UT bug fixes
*
* Revision 1.25  2009/06/23 05:13:43  gur15697
* review comments incorporated
*
* Revision 1.24  2009/06/23 04:55:06  gur15697
* review comments incorporated
*
* Revision 1.23  2009/06/22 10:02:47  gur15697
* allocateUplinkResources() arguments updated as per the latest changes
*
* Revision 1.22  2009/06/22 04:52:12  gur15697
* review comments incorporated
*
* Revision 1.21  2009/06/19 13:19:32  gur15697
* review comments incorporated
*
* Revision 1.20  2009/06/19 05:36:55  gur15697
* review comments incorporated
*
* Revision 1.19  2009/06/18 14:20:19  gur12140
* initRNTIPoolTbl() modified
*
* Revision 1.18  2009/06/18 09:15:23  gur15697
* lowerCRNTIBound_g and upperCRNTIBound_g modified to  lowerRNTIBound_g and upperRNTIBound_g
*
* Revision 1.17  2009/06/17 09:37:51  gur15697
* getUeIdxFromRNTIMap() modified
*
* Revision 1.16  2009/06/16 14:38:56  gur15697
* compilation errors removed
*
* Revision 1.15  2009/06/15 09:06:37  gur18569
* calling initUESecificSearchSpaceOfAllUEs from initRNTIPoolTbl
*
* Revision 1.14  2009/06/10 15:15:00  gur18569
* removed linking and compilation errors
*
* Revision 1.13  2009/06/10 06:01:47  gur15697
* initRNTIPool() updated to handle the RARNTI range configured
*
* Revision 1.12  2009/06/09 13:31:27  gur15697
* initRNTITbl updated
*
* Revision 1.11  2009/06/08 08:51:31  gur15697
* PRACH code moved to lteMacPRACH.c
*
    * Revision 1.10  2009/06/08 06:25:11  gur15697
* getPUSCHResources() changed to getFreePUSCHResources()
    *
    * Revision 1.9  2009/06/05 08:03:00  gur15697
    * init and cleanup functions added
    *
    * Revision 1.8  2009/06/05 04:13:53  gur15697
    * updated the file w.r.t PDCCH manager changes
    *
    * Revision 1.7  2009/06/04 12:57:27  gur15697
    * review comments incorporated
    *
    * Revision 1.6  2009/05/28 06:18:45  gur15697
    * compilation errors removed
    *
    * Revision 1.5  2009/05/28 05:52:05  gur15697
    * comments encorporated
    *
    * Revision 1.4  2009/05/27 10:00:54  gur15697
    * Random access code added
    *
    * Revision 1.3  2009/05/10 09:01:45  gur15697
    * Removed compilation errors
    *
    * Revision 1.2  2009/05/05 14:15:26  gur15697
    * new functions added
    *
    * Revision 1.1  2009/04/30 06:47:26  gur15697
    * Initial vertion
    *
    *
    *****************************************************************************/


    /******************************************************************************
     * Standard Library Includes
     *****************************************************************************/



    /******************************************************************************
     * Project Includes
     *****************************************************************************/
#include "lteMacRandomAccess.h"
#include "lteMacCellConfMgr.h"
#include "lteMacPUCCH.h"
#include "lteMacComPorting.h"
#include "lteMacPRACH.h"
#include "lteFapiMacPhyInterface.h"
#include "lteMacPUSCH.h"
#include "lteMacUEConfMgr.h"
#include "lteMacULPowerControlData.h"
    /* 5492 changes start */
    /* SPR 4063 Start */
#include "lteMacULSchedulerStrategy.h"
    /* SPR 4063 End */
    /* 5492 changes end */
#include "lteCircQueue.h"
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
#endif    
    /* ICIC changes start */
#include "lteMacULResourceInfoMgr.h"
#include "lteMacULResourceAllocator.h"
#include "lteMacRRMPeriodicMgr.h"
#include "lteMacDLPowerControlData.h"
    /* ICIC changes end */
#include "lteMacSpsPUSCHmgr.h"
    /* Perf stats change start*/
#include "lteMacReceiver.h"
#include "ltePerfStats.h"
    /*  + E_CID_5.3  */
#include "lteMacECIDMgr.h"
    /*   - E_CID_5.3  */
#include "lteMacPRSMgr.h"


    /******************************************************************************
      Private Definitions
     *****************************************************************************/



    /******************************************************************************
      Private Types
     *****************************************************************************/
    /*SPR 16855 +-*/
    extern RRC_OAM_HPTOLP_THDH_PRIO_TYPE rrcOamHPtoLPThdHPrioQ_g[MAX_NUM_CELL][MAX_THD];
    /*SPR 16855 +-*/
    /*****************************************************************************
     * Private Function Prototypes
     ****************************************************************************/

    /******************************************************************************
     * Private Constants
     *****************************************************************************/

    /******************************************************************************
     * Exported Variables
     *****************************************************************************/

    /*DYN_PDCCH_TDD_FIX*/
    /*CA Changes start  */
    extern UInt8 changeCFIInThisTTI_g[MAX_NUM_CELL];
    /*DYN_PDCCH_TDD_FIX*/



    extern MacRetType deleteUEContext( UInt32 ueIndex, InternalCellIndex internalCellIndex);
    /* CA changes End*/

    /* + CQI_5.0 */
    extern void storeCqiPmiRiDataAperiodic(UInt8 transMode, 
            ULUEContextInfo *ulUEContextInfo_p,
            ULSchUciRachPoolInfo *ulschUciRachInfo_p,
            /*TDD SB_CQI*/
#ifdef TDD_CONFIG
            UInt8 sysFrameNumber, 
#endif
            /*TDD SB_CQI*/
            UInt8 subFrameNumber,UInt8 ulschUciInfoIndex,
            /*CA Changes start */
            InternalCellIndex internalCellIndex);
    /*CA Changes end */
    /* - CQI_5.0 */

    /*START : DYN_PDCCH*/
    extern UInt8 currentCFIValue_g[MAX_NUM_CELL];/*DYN_PDCCH*/
    /*END   : DYN_PDCCH*/
    /*SPR 7086 Fix Start*/
    extern SRSULBandwidthTableEntry *srsFddUplinkSRSBandwidthTable_p[MAX_NUM_CELL];
    /*SPR 7086 Fix End*/
    /*  SPR_9402_FIX_START */
#ifdef FDD_CONFIG
    extern UInt8  maxCCEForCFI_g[MAX_NUM_CELL][MAX_POSSIBLE_CFI_VALUE];
#else
    extern UInt8 maxCCEForCFI_g[MAX_NUM_CELL][MAX_POSSIBLE_CFI_VALUE][MAX_VALUE_M_I];
#endif
    extern UInt8 valueCFI_g[MAX_NUM_CELL] ;
    /* SPR_9402_FIX_END */

    /*****************************************************************************
     * Global variables for maitaining the RNTI pool
     ****************************************************************************/
    /*Range configured for CRNTI, lower boundary & upper boundary*/
    /* Considering Pluggable Scheduler to be 
     * running seperate thread this data sturcture needs to shared with pluggable
     * Scheduler  
     */
    /*CA Changes start  */
    RNTIInfo *crntiToIdxTbl_g[MAX_NUM_CELL] = {PNULL};

/*Queues which maintains free RNTIs */
LTE_SQUEUE* freeRNTIQueue_gp[MAX_NUM_CELL] = {PNULL};
LTE_SQUEUE* freeRNTIQueueAfterUEDeletion_gp[MAX_NUM_CELL] = {PNULL};;

/*****************************************************************************
 * Global variables for Random access procedure
 ****************************************************************************/
/*Contains the Temporary UE context created*/
TempCRNTICtxInfo* tempCrntiCtx_gp[MAX_NUM_CELL] = {PNULL};


RACHGlobals *rachGlobals_gp[MAX_NUM_CELL] = {PNULL};


/* number of UEs who get C-rnti */
UInt16 macCrntiUeNum_g[MAX_NUM_CELL] = {0};
/* number of UEs who get TC-rnti */
UInt16 macTCrntiUeNum_g[MAX_NUM_CELL] = {0};


/*CA Change end*/
/*****************************************************************************
 * Global variables for Contention Based RACH for PDCCH Order 
 ****************************************************************************/

extern UInt8 maxPdcchOrderperTTI_g[MAX_NUM_CELL];

/* msg3 ulconfig fix start */
#ifdef TDD_CONFIG
/* List for storing RAR UL CONFIG till SF for DCI0 (corresponding to the UL SF 
 * for Msg3) reaches */
/** CA Changes Start **/
STATIC LTE_SQUEUE ulschUciInfoForULConfigForRach_g[MAX_NUM_CELL];
/** CA Changes End **/
#endif
/* msg3 ulconfig fix end */

/************ Timer Lists and queues for Random access procedure ************/
/* CA Changes start  */
/*SPR 22322 Start*/
/* Removing TempCRNTI timer , due to this functionality the msg3 retransmission was not happening 
 * as the expTTI was getting reached in the same tick the retransmission was getting scheduled.
 * Hence this timer is of no use as message 3 retransmission is already supported */
/*SPR 22322 End*/

/* SPR 16773 fix start */
LTE_LIST*   createUeContextTimerList_gp[MAX_NUM_CELL] = {PNULL};
/* SPR 16773 fix end */
LTE_LIST*   conFreePreambleTimerList_gp[MAX_NUM_CELL] = {PNULL};
LTE_LIST*   conBasedPDCCHOrderTimerList_gp[MAX_NUM_CELL] = {PNULL};
ContentionFreeRACHPool*   contentionFreeRACHPool_gp[MAX_NUM_CELL] = {PNULL};
LTE_SQUEUE*   contentionTimerQueue_gp[MAX_NUM_CELL] = {PNULL};
/* SPR 3854 Start*/
LTE_SQUEUE*   contentionFreeRAReqQueue_gp[MAX_NUM_CELL] = {PNULL};
/* SPR 3854 End*/
/* + E_CID_5.3 */
extern TempTimingAdvandeR9  tempTimingAdvanceR9_g[MAX_NUM_CELL][MAC_MAX_NUM_OF_RA_PREAMBLES];
/* - E_CID_5.3 */
/****************************************************************************/

/**************** Queues for Random access procedure ************************/
/*Contains the received RA requests*/

/* SPR 3854 Start*/
LTE_SQUEUE* raReqQueue_gp[MAX_NUM_CELL] = {PNULL};
/* SPR 3854 End*/

LTE_SQUEUE* ulRARspNegAckQueue_gp[MAX_NUM_CELL] = {PNULL};
CONTENTION_RESOLUTION_MSG_Q* contentionResolutionMsgQ_gp[MAX_NUM_CELL] = {PNULL};
LTE_SQUEUE* contentionResolutionMsgAckQ_gp[MAX_NUM_CELL] = {PNULL};
DL_RA_RSP_QUEUE_TYPE* dlRARspQueue_gp[MAX_NUM_CELL] = {PNULL};
/*ContentionResULGrantQueue Used by Scheduler */

/*New queues added as per the DL-UL Split */
DEMUX_RACH_MANAGER_INTERFACE_TYPE* demuxRachManagerInterfaceQueue_gp[MAX_NUM_CELL] = {PNULL};
RACH_DL_STRATEGY_INTERFACE_TYPE *rachDLStrategyInterfaceQueue_gp[MAX_NUM_CELL] = {PNULL};
PDCCH_ORDER_TIMER_TRIGGER_TYPE* pdcchOrderTimerTriggerQueue_gp[MAX_NUM_CELL] = {PNULL};
PDCCH_RACH_INTERFACE_TYPE* pdcchRachInterfaceQueue_gp[MAX_NUM_CELL] = {PNULL};
/* CA Changes end  */


/* Counter/Flag used for support MAC_DELETE_CELL_REQ */
/****************************************************************************/

/*Cyclomatic Complexity changes - starts here */
STATIC  void cleanupContentionQueues(InternalCellIndex internalCellIndex);
/*Cyclomatic Complexity changes - ends here */
/* Rel 5.3.1: Memory related changes start*/
 void cleanpContentionFreeRACHPool(InternalCellIndex internalCellIndex);
/* Rel 5.3.1: Memory related changes end*/
/* SPR 16773 fix start */
STATIC LP_CreateUeContextTimerNode searchCreateUeTimer( UInt16 rnti,InternalCellIndex internalCellIndex );
STATIC void stopCreateUeTimer( 
        LP_CreateUeContextTimerNode createUeListNode_p,InternalCellIndex internalCellIndex );
STATIC void processCreateUETimerList( UInt32 currentTick, UInt32 count,InternalCellIndex internalCellIndex );
/* SPR 16773 fix end */

/* + SPR 17733 */
#ifdef FDD_CONFIG
STATIC void processMsg4RetransmissionFailureQueue( 
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt32 *availableCommonCCE_p, UInt32 *availableCCE_p,
        SInt32 *availableDLRBCount_p, UInt32 subFrameNum,
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        InternalCellIndex internalCellIndex);
STATIC void processMsg4RetransmissionQueue( 
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt32 *availableCommonCCE_p, UInt32 *availableCCE_p, 
        SInt32 *availableDLRBCount_p, UInt32 subFrameNum, 
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        InternalCellIndex internalCellIndex);
#endif
/* - SPR 17733 */

/*****************************************************************************/
/*EICIC*/
extern UInt8 absPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN]; /* Aggressor*/
extern UInt8 usableAbsPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN]; /* Victim*/
extern UInt8 absPatternIndex_g[MAX_NUM_CELL]  ;
extern UInt8 usableAbsPatternIndex_g[MAX_NUM_CELL] ;
/*EICIC*/
/* Cyclomatic_complexity_changes_start */
RAReqTobeScheduledInPdu *getRarPduToSchedule(RAReqQueueNode *raReqQNode_p,
        UInt32 *scheduledRARNTIBitMap1_p,
        UInt32 *scheduledRARNTIBitMap2_p,
        RarPduTobeScheduledInTTI *raResponsesInTTI_p,
        UInt8  *processRntiFlag_p);

void validateRarCntInContFreeQueueProcessing(
        UInt8  numRARCnt,
        UInt32 *maxRAR_p,
        UInt8 processRntiFlag,
        UInt16 raRNTI,
        UInt32 *scheduledRARNTIBitMap1_p,
        UInt32 *scheduledRARNTIBitMap2_p,
        RarPduTobeScheduledInTTI *raResponsesInTTI_p,
        RAReqTobeScheduledInPdu *rarPduTobeScheduled_p);
/* Cyclomatic_complexity_changes_end */

/* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
#ifdef MAC_AUT_TEST
TestPrepareAndSendRAR testPrepareAndSendRAR_g = {0};
TestPrepareAndSendRAR testProcessContFreeRAReqQueue_g = {0};
TestPrepareAndSendRAR testProcessRAResInNegRARPDU_g = {0};
#endif
#endif
/*TDD Config 0 Changes End */

/* EICIC +*/
UInt8	isCurrentSfAbs_g[MAX_NUM_CELL] =  {LTE_FALSE};
UInt8      isCurrentSfAbsV_g[MAX_NUM_CELL]= {LTE_FALSE};
/* EICIC -*/
/* Cyclomatic_complexity_changes_end */
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/*****************************************************************************
 * Function Name  : initRACHMgr
 * Inputs         : numCellsConfigured - Number of cells configured
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes the Global queues and variables required by 
 *                  Random Access manager. This is called at MAC_INIT_LAYER_REQ handling.
 *****************************************************************************/
/*CA Changes start  */
void initRACHMgr(UInt8 numCellsConfigured)
    /*CA Changes end  */
{
    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    UInt8 j = 0;    
    UInt8 cellCount = 0;
    /* CA Changes start */
    UInt16  macMaxNumOfUeSupported   = 0;

    CONSTRUCT_AND_INIT_DL_RA_RSP_QUEUE_Q(numCellsConfigured);
    CONSTRUCT_AND_INIT_CONTENTION_RESOLUTION_SQ(numCellsConfigured);

    createMemPool(sizeof(RACHGlobals), numCellsConfigured);
    createMemPool(sizeof(LTE_LIST), numCellsConfigured * 3);
    createMemPool(sizeof(ContentionFreeRACHPool), numCellsConfigured);
    createMemPool(sizeof(LTE_SQUEUE), numCellsConfigured * 5);

    createMemPool(sizeof(ConFreePreambleTimerNode), 
            (MAC_MAX_NUM_OF_RA_PREAMBLES - 
             MAC_MIN_COUNT_OF_CONTENTION_BASED_RA_PREAMBLES));

    createMemPool(sizeof(ContentionFreePreamble), 
            (MAC_MAX_NUM_OF_RA_PREAMBLES - 
             MAC_MIN_COUNT_OF_CONTENTION_BASED_RA_PREAMBLES));

    createMemPool(sizeof(ContentionQueueNode), MAX_RAR_PDU_PER_TTI);
    createMemPool(sizeof(ULGrantRequestQueueNode), MAX_RAR_PDU_PER_TTI);
    createMemPool(sizeof(ULRARspNegAckQueueNode), MAX_RAR_PDU_PER_TTI);
    /*CA Changes start  : Construct and initialize the contentionResolutionMsgAckQ_g queues */
    createMemPool(sizeof(LTE_SQUEUE), numCellsConfigured);

    CONSTRUCT_AND_INIT_DEMUX_RACH_MANAGER_INTERFACE_Q(numCellsConfigured);
    CONSTRUCT_AND_INIT_RACH_DL_STRATEGY_INTERFACE_Q(numCellsConfigured);
    CONSTRUCT_AND_INIT_PDCCH_ORDER_TIMER_TRIGGER_Q(numCellsConfigured);
    CONSTRUCT_AND_INIT_PDCCH_RACH_INTERFACE_Q(numCellsConfigured);

    for (cellCount = 0; cellCount < numCellsConfigured; cellCount++)
    {
        macMaxNumOfUeSupported  = cellSpecificParams_g.\
                                  cellConfigAndInitParams_p[cellCount]->initParams_p->macMaxNumOfUeSupported;
#ifdef TDD_CONFIG
        /** CA Changes Start **/
        sQueueInit(&ulschUciInfoForULConfigForRach_g[cellCount]);
        /** CA Changes End **/
        /* Start 128 UE: Memory Changes */
        createMemPool(sizeof(UlschUciInfoForULConfigForRach), (macMaxNumOfUeSupported));
#endif   

        GET_MEM_FROM_POOL(RACHGlobals, rachGlobals_gp[cellCount],sizeof(RACHGlobals), PNULL);


#ifdef FDD_CONFIG
        for (j = 0; j < MAX_SUB_FRAME; j++)
#elif TDD_CONFIG
            for (j = 0; j < MAX_NUMBER_OF_CONTAINER; j++)
#endif 
            {
                rachGlobals_gp[cellCount]->msg3ScheduledCount[j] = 0;
            }

        /* +- SPR 22322 */
        GET_MEM_FROM_POOL(LTE_LIST, conFreePreambleTimerList_gp[cellCount],sizeof(LTE_LIST), PNULL);
        GET_MEM_FROM_POOL(LTE_LIST, conBasedPDCCHOrderTimerList_gp[cellCount],sizeof(LTE_LIST), PNULL);
        GET_MEM_FROM_POOL(ContentionFreeRACHPool, contentionFreeRACHPool_gp[cellCount],sizeof(ContentionFreeRACHPool), PNULL);
        /*SPR 16773 Fix Start*/
        GET_MEM_FROM_POOL(LTE_LIST, createUeContextTimerList_gp[cellCount],sizeof(LTE_LIST), PNULL);
        listInit(createUeContextTimerList_gp[cellCount]);
        /*SPR 16773 Fix End*/
        /* +- SPR 22322 */
        listInit(conFreePreambleTimerList_gp[cellCount]);
        listInit(conBasedPDCCHOrderTimerList_gp[cellCount]);

        GET_MEM_FROM_POOL(LTE_SQUEUE, contentionTimerQueue_gp[cellCount],sizeof(LTE_SQUEUE), PNULL);
        GET_MEM_FROM_POOL(LTE_SQUEUE, raReqQueue_gp[cellCount],sizeof(LTE_SQUEUE), PNULL);
        GET_MEM_FROM_POOL(LTE_SQUEUE, contentionFreeRAReqQueue_gp[cellCount],sizeof(LTE_SQUEUE), PNULL);
        GET_MEM_FROM_POOL(LTE_SQUEUE, ulRARspNegAckQueue_gp[cellCount],sizeof(LTE_SQUEUE), PNULL);
        GET_MEM_FROM_POOL(LTE_SQUEUE, contentionResolutionMsgAckQ_gp[cellCount],sizeof(LTE_SQUEUE), PNULL);
        sQueueInit(contentionTimerQueue_gp[cellCount]);
        sQueueInit(raReqQueue_gp[cellCount]);
        sQueueInit(contentionFreeRAReqQueue_gp[cellCount]);
        sQueueInit(ulRARspNegAckQueue_gp[cellCount]);
        sQueueInit(contentionResolutionMsgAckQ_gp[cellCount]);

        /*Temporary UE Context*/
        createMemPool(sizeof(TempCRNTICtx), (macMaxNumOfUeSupported));

        /*Timers*/
        /*Timernode for TCRNTI timer and Contextion resolution timer*/
        createMemPool(sizeof(TempContextTimerNode), (macMaxNumOfUeSupported * 2));

        createMemPool(sizeof(RAReqQueueNode), (macMaxNumOfUeSupported));
        createMemPool(sizeof(DLRARspQueueNode), (macMaxNumOfUeSupported));
        createMemPool(sizeof(RachDemuxQueueNode), (macMaxNumOfUeSupported));
        createMemPool(sizeof(RachDLStrategyQueueNode), (macMaxNumOfUeSupported));
        createMemPool(sizeof(PdcchOrderTimerTriggerQueueNode), (macMaxNumOfUeSupported));
        createMemPool(sizeof(RachInterfaceQueueNode), (macMaxNumOfUeSupported));
        /* End 128 UE: Memory Changes */
        /* SPR 16773 fix start */
        createMemPool( sizeof(CreateUeContextTimerNode), 
                (macMaxNumOfUeSupported * 2));
        /* SPR 16773 fix end */
    }
    /*CA Changes end  */

    initTempUECtxFreeIndexPool(numCellsConfigured);

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);

    return;
}



/*****************************************************************************
 * Function Name  : initRNTIPoolTbl
 * Inputs         : maxUE - Maximum number of UEs configured from OAM
 *                  internalCellIndex - Cell-Index at MAC                                            
 * Outputs        : None
 * Returns        : SUCCESS/FAILURE/ERROR
 * Description    : Initializes the crntiToIdxTbl_g table with default values 
 *                  and initializes the free RNTI pool. This is called at cell config.
 *****************************************************************************/
/* + Coverity  24508 */
MacRetType  initRNTIPoolTbl(UInt32 maxUE, InternalCellIndex internalCellIndex)
    /* - Coverity  24508 */
{
    UInt32 i = 0;
    UInt32 rnti = 0;
    UInt32 maxBufPdcchOrderTimerMemPool = 0;
    UInt16 totalRnti  = 0;
    /* CA Changes start */
    InitParams *initParams_p = cellSpecificParams_g.\
                               cellConfigAndInitParams_p[internalCellIndex]->initParams_p;

    CellConfigParams *cellParams_p = cellSpecificParams_g.\
                                     cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /* CA Changes end */
    RNTIInfo *rntiInfo_p = PNULL;
    FreeRNTIPoolNode *freeIdxNode_p = PNULL;

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    /* Contention Based PDCCH Order Timer pool */
    /*SPR_3064_3069_FIX*/
    rachGlobals_gp[internalCellIndex]->conBasedPDCCHOrderTimerValue = 
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->contnFreeRachTimer;
    /*SPR_3064_3069_FIX*/

    maxBufPdcchOrderTimerMemPool = 
        (rachGlobals_gp[internalCellIndex]->conBasedPDCCHOrderTimerValue * MAX_PDCCH_ORDER_RETRIES * maxPdcchOrderperTTI_g[internalCellIndex]);  

    rachGlobals_gp[internalCellIndex]->numBufConBasedPDCCHOrderTimerMemPool =
        ((maxBufPdcchOrderTimerMemPool > maxUE) ? maxUE : maxBufPdcchOrderTimerMemPool);

    if (initParams_p->lowerRNTIBound <= cellParams_p->endRARNTI)
    {
        initParams_p->lowerRNTIBound = cellParams_p->endRARNTI + 1;
    }

    /*CRNTI range = (total RNTI range configured) */
    initParams_p->crntiRange = (initParams_p->upperRNTIBound - initParams_p->lowerRNTIBound + 1);

    /* SPS_CHG */
    totalRnti = initParams_p->crntiRange;

    if (TRUE == initParams_p->spsSysWideParam.enableSps)
    {
        initParams_p->spsCrntiRange = cellParams_p->spsCommonCellInfo.endSpsCRnti - 
            cellParams_p->spsCommonCellInfo.startSpsCRnti + 1;
        totalRnti += initParams_p->spsCrntiRange; 
    }
    /* SPS_CHG */

    /*Initialize the freeTempCRNTIQueue_g queues*/
    /*CA Changes start  */
    /* The mem pool will be created only first time when cell was configured */ 
    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))
    {
        createMemPool(sizeof(LTE_SQUEUE), 2);
        /*Free RNTI pool*/
        createMemPool(sizeof(FreeRNTIPoolNode),(initParams_p->macMaxNumOfUeSupported));
        createMemPool(sizeof(ConBasedPdcchOrderTimerNode), 
                rachGlobals_gp[internalCellIndex]->numBufConBasedPDCCHOrderTimerMemPool);
        /*UE specific search space*/
#ifndef __x86_64__
/*
 * In 64 bit system, mempool usage goes beyond 90% and hence system doesn't work.
 * For a workaroung, creating additional 600 mempool to take care of. Need debug why
 * this issue doesn't come with 32 bit system
 * */
/* SPR 23260 + */
        createMemPool(sizeof(UESpecificSearchSpaceInfo) * MAX_SUB_FRAME , totalRnti);
        /* SPR 21381 Fix Start */
        createMemPool(sizeof(DLHARQEntity), totalRnti);
#else
        createMemPool(sizeof(UESpecificSearchSpaceInfo) * MAX_SUB_FRAME , (totalRnti+600));
        /* SPR 21381 Fix Start */
        createMemPool(sizeof(DLHARQEntity), (totalRnti+600));
/* SPR 23260 - */
#endif

		createMemPool((sizeof(RNTIInfo) * totalRnti), 1);

        /* SPR 21381 Fix End */
        createMemPoolForCell(internalCellIndex);
    }
    GET_MEM_FROM_POOL(LTE_SQUEUE, freeRNTIQueue_gp[internalCellIndex],sizeof(LTE_SQUEUE), PNULL);
    GET_MEM_FROM_POOL(LTE_SQUEUE, freeRNTIQueueAfterUEDeletion_gp[internalCellIndex],sizeof(LTE_SQUEUE), PNULL);
    sQueueInit(freeRNTIQueue_gp[internalCellIndex]);
    sQueueInit(freeRNTIQueueAfterUEDeletion_gp[internalCellIndex]);
    /* CA Changes end */

    GET_MEM_FROM_POOL(RNTIInfo , crntiToIdxTbl_g[internalCellIndex] , (sizeof(RNTIInfo) * totalRnti) , PNULL);

    /* SPR 11044 Fix start */
    memSet(crntiToIdxTbl_g[internalCellIndex], 0 ,(sizeof(RNTIInfo) * totalRnti));
    /* SPR 11044 Fix end */

    /* Coverity CID 25631 start */
    if(PNULL == crntiToIdxTbl_g[internalCellIndex])
    {
        ltePanic("getMemFromPool fails %s[%d]",__FUNCTION__, __LINE__);
        return MAC_FAILURE;
    }
    /* Coverity CID 25631 end */

    /*Initialize the crntiToIdxTbl_g table with default values*/
    /*Insert the crntiRange_g number of queue nodes in freeTempCRNTIQueue_g
     *queue (during MAC init time all RNTIs are free)
     */
    rntiInfo_p = (RNTIInfo *)crntiToIdxTbl_g[internalCellIndex];

    for (i = 0,rnti = initParams_p->lowerRNTIBound; 
            rnti <= initParams_p->upperRNTIBound;
            rnti++)
    {
        /*Initialize CRNTI Pool*/
        rntiInfo_p[i].rntiFlag = FREERNTI;
        rntiInfo_p[i].index    = MAX_UE_SUPPORTED;
        /*pdcch changes start*/
        GET_MEM_FROM_POOL(UESpecificSearchSpaceInfo , rntiInfo_p[i].ueSearchSpaceInfo_p[internalCellIndex],
                MAX_SUB_FRAME * sizeof(UESpecificSearchSpaceInfo), PNULL);
        /* Coverity CID 25631 start */
        if(PNULL == rntiInfo_p[i].ueSearchSpaceInfo_p[internalCellIndex])
        {
            ltePanic("getMemFromPool fails %s[%d]",__FUNCTION__, __LINE__);
            /*Klockwork warning changes start */
            return MAC_FAILURE;
            /*Klockwork warning changes end */
        }
        /* Coverity CID 25631 end */
        memSet(rntiInfo_p[i].ueSearchSpaceInfo_p[internalCellIndex], 0, MAX_SUB_FRAME * sizeof(UESpecificSearchSpaceInfo));

        /*pdcch changes end*/
        /*Free RNTI pool. From this pool TC-RNTI is allocated during RA procedure */
        GET_MEM_FROM_POOL(FreeRNTIPoolNode , freeIdxNode_p , sizeof(FreeRNTIPoolNode) , PNULL);

        /* + Coverity 24508 */
        if( PNULL == freeIdxNode_p )
        {
            LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGERROR,MAC_RACH,
                    getCurrentTick(), __LINE__,
                    sizeof(FreeRNTIPoolNode), 0,0,0,
                    0.0,0.0,__FILE__,__func__);
            return MAC_FAILURE ;
        }
        /* - Coverity 24508 */

        freeIdxNode_p->rnti = (UInt16)rnti;

        /*CA Changes start  */
        if (pushNode(freeRNTIQueue_gp[internalCellIndex], &freeIdxNode_p->nodeAnchor))
            /*CA Changes end  */
        {
            ltePanic("ERROR: pushNode() failed for freeRNTIQueue_g queue\n");
        }
        i++;
        freeIdxNode_p = PNULL;
    }

    macCrntiUeNum_g[internalCellIndex] = 0;
    macTCrntiUeNum_g[internalCellIndex] = 0;

    /* SPS_CHG */
    /*pdcch changes start*/
    if (TRUE == initParams_p->spsSysWideParam.enableSps)
    { 
        for (  i = initParams_p->crntiRange; i < totalRnti ; i++ )
        {
            rntiInfo_p[i].rntiFlag = FREERNTI;
            rntiInfo_p[i].index    = MAX_UE_SUPPORTED;
            rntiInfo_p[i].ueSearchSpaceInfo_p[internalCellIndex] = 
                (UESpecificSearchSpaceInfo *)getMemFromPool( 
                        MAX_SUB_FRAME * sizeof(UESpecificSearchSpaceInfo), PNULL);
            if ( PNULL == rntiInfo_p[i].ueSearchSpaceInfo_p[internalCellIndex] )
            {
                LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
                        getCurrentTick(), __LINE__, 
                        MAX_SUB_FRAME * sizeof(UESpecificSearchSpaceInfo), 0,0,0,
                        0.0,0.0,__FILE__,__func__);
                ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                        CRITICAL_ALARM);
                ltePanic("getMemFromPool fails %s",__FUNCTION__);
            }    
            memSet(rntiInfo_p[i].ueSearchSpaceInfo_p[internalCellIndex], 0, MAX_SUB_FRAME * sizeof(UESpecificSearchSpaceInfo));
        }
    }
    /*pdcch changes end*/
    /* SPS_CHG */
    /*Funtion for Initializing UE Specific Search Space*/
    /* SPR_9402_FIX_START */
#ifdef FDD_CONFIG
    initUESecificSearchSpaceOfAllUEs((UInt8)maxCCEForCFI_g[internalCellIndex][valueCFI_g[internalCellIndex] -1], 
            internalCellIndex,internalCellIndex);

#else
    initUESecificSearchSpaceOfAllUEs(
            /* +- SPR 17777 */
            internalCellIndex, internalCellIndex );
#endif 
    /* + SPR 14000 Changes */
    initUESecificSearchSpaceForCell(internalCellIndex);
    /* - SPR 14000 Changes */
    /* SPR_9402_FIX_END */
    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);

    /* + Coverity  24508 */
    return MAC_SUCCESS; 
    /* - Coverity  24508 */
}


/* SPR 12283 Fix Start */
/****************************************************************************
 * Function Name  : cleanUpUEContext
 * Inputs        : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to cleanup UE Context
 ****************************************************************************/
void cleanUpUEContext(UInt16 msgId, InternalCellIndex internalCellIndex )
{
    UInt16 i = 0;
    DLUEContext *ueDLContext_p = PNULL;
    /*
     * for cell stop/delete following possiblities are possible
     * STOP
     *      -pcell
     *          -delete scell
     *          -delete pcell
     *      -scell
     *          -do nothing
     * DEL
     *      -pcell
     *          -delete scell
     *          -delete pcell
     *      -scell
     *          -delete scell
     * done
     */

    for (i = 0; i < MAX_UE_SUPPORTED; i++)
    {
        ueDLContext_p = dlUECtxInfoArr_g[i].dlUEContext_p;
        if(ueDLContext_p != PNULL)
        { 
            if(ueDLContext_p->internalCellIndex == internalCellIndex)
            { 
                macDeleteScellCtxt(ueDLContext_p, internalCellIndex);
                if (MAC_FAILURE == deleteUEContext(i, internalCellIndex ))
                {
                    LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                            "[%s]: deleteUEContext(%d) returned failure\n",
                            __func__,i);
                }
            }
            else if (msgId == MAC_DELETE_CELL_REQ)
            {
                macDeleteScellCtxt(ueDLContext_p, internalCellIndex);
            }
        }
    }
}
/* SPR 12283 Fix End */
/****************************************************************************
 * Function Name  : cleanupRACHMgr
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API cleanup the cleanup rachManager. This is called at cell stop.
 ****************************************************************************/
/*CA Changes start  */
void cleanupRACHMgr(InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{
    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);
    /*Cyclomatic Complexity changes - starts here */
    cleanupContentionQueues(internalCellIndex);
    /*Cyclomatic Complexity changes - ends here */

    /* Cyclomatic_complexity_changes_start */ 
    /* +- SPR 22322 */
    cleanupRAReqQueue(internalCellIndex);
    cleanupContentionFreeRAReqQueue(internalCellIndex);
    cleanupulRARspNegAckQueue(internalCellIndex);
    cleanupDLRARspQueue(internalCellIndex);
    cleanupRachDemuxQueue(internalCellIndex);
    cleanupRachDLStrategyQueue(internalCellIndex);
    cleanupPdcchOrderTimerTriggerQueue(internalCellIndex);
    cleanupRachInterfaceQueue(internalCellIndex);   
    /* Cyclomatic_complexity_changes_end */
    /* SPR 16773 fix start */
    cleanupUeTimerList(internalCellIndex);
    /* SPR 16773 fix end */


    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);
}

/*Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : cleanupContentionQueues 
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API cleanup the Contention Queues,Free the pool
 ****************************************************************************/
STATIC  void cleanupContentionQueues(InternalCellIndex internalCellIndex)
{ 
    UInt32 i = 0;
    UInt32 count = 0;
    ContentionTimerNode  *contentionTimerNode_p = PNULL;
    ConFreePreambleTimerNode* contentionFreePreambleTimerNode_p = PNULL;    
    ULGrantRequestQueueNode *ulGrantRequestQueueNode_p = PNULL;
    ConBasedPdcchOrderTimerNode* conBasedPdcchOrderTimerNode_p = PNULL;    
    /* SPR 1839 fix Start */ 
    ContentionResAckNode *contentionAckQueueNode_p = PNULL;
    /* SPR 1839 fix End */
    ContentionQueueNode  *contentionQueueNode_p = PNULL; 

    count = listCount(conBasedPDCCHOrderTimerList_gp[internalCellIndex]);

    for (i = 0; i < count; i++)
    {
        conBasedPdcchOrderTimerNode_p = 
            (ConBasedPdcchOrderTimerNode *)getFirstListNode(conBasedPDCCHOrderTimerList_gp[internalCellIndex]);

        listDeleteNode(conBasedPDCCHOrderTimerList_gp[internalCellIndex],
                &conBasedPdcchOrderTimerNode_p->nodeAnchor);
        if (conBasedPdcchOrderTimerNode_p)
        {
            freeMemPool(conBasedPdcchOrderTimerNode_p);
            conBasedPdcchOrderTimerNode_p = PNULL;
        }
    }

    /*Contention timer list*/    
    /*CA Changes start  */
    count = sQueueCount(contentionTimerQueue_gp[internalCellIndex]);
    /*CA Changes end  */
    for (i = 0; i < count; i++)
    {
        contentionTimerNode_p = 
            /*CA Changes start  */
            (ContentionTimerNode *)popNode(contentionTimerQueue_gp[internalCellIndex]);
        /*CA Changes end  */

        if (contentionTimerNode_p)
        {
            freeMemPool(contentionTimerNode_p);
            contentionTimerNode_p = PNULL;
        }
    }
    /*Contention Free Preamble Timer Node*/
    count = listCount(conFreePreambleTimerList_gp[internalCellIndex]);
    for (i = 0; i < count; i++)
    {
        contentionFreePreambleTimerNode_p = 
            (ConFreePreambleTimerNode *)getFirstListNode(conFreePreambleTimerList_gp[internalCellIndex]);

        listDeleteNode(conFreePreambleTimerList_gp[internalCellIndex],
                &contentionFreePreambleTimerNode_p->nodeAnchor);
        if (contentionFreePreambleTimerNode_p)
        {
            freeMemPool(contentionFreePreambleTimerNode_p);
            contentionFreePreambleTimerNode_p = PNULL;
        }
    }
    /*Contention resolution queue*/
    count = COUNT_CONTENTION_RESOLUTION_SQ(internalCellIndex);
    for (i = 0; i < count; i++)
    {
        DEQUEUE_CONTENTION_RESOLUTION_SQ(ContentionQueueNode, contentionQueueNode_p, internalCellIndex);

        if (contentionQueueNode_p)
        {
            if (contentionQueueNode_p->msg_p)
            {
                msgFree(contentionQueueNode_p->msg_p);
            }
            FREE_MEM_NODE_CONTENTION_RESOLUTION_SQ(contentionQueueNode_p);
        }
    }
    /*UL grant Contention resolution queue*/
    /*CA Changes start  */
    count = sQueueCount(ulGrantContentionResQueue_gp[internalCellIndex]);
    /*CA Changes end  */
    for (i = 0; i < count; i++)
    {
        ulGrantRequestQueueNode_p = 
            /*CA Changes start  */
            (ULGrantRequestQueueNode *)popNode(ulGrantContentionResQueue_gp[internalCellIndex]);
        /*CA Changes end  */

        if (ulGrantRequestQueueNode_p)
        {
            freeMemPool(ulGrantRequestQueueNode_p);
            ulGrantRequestQueueNode_p = PNULL;
        }
    }
    /* SPR 1839 fix Start */
    count = sQueueCount(contentionResolutionMsgAckQ_gp[internalCellIndex]);
    for (i = 0; i < count; i++)
    {
        contentionAckQueueNode_p =
            (ContentionResAckNode *)popNode(contentionResolutionMsgAckQ_gp[internalCellIndex]);

        if (contentionAckQueueNode_p)
        {
            freeMemPool(contentionAckQueueNode_p);
        }
    }
    /* SPR 1839 fix End  */
}
/* Rel 5.3.1: Memory related changes start*/
/****************************************************************************
 * Function Name  : cleanupContentionF 
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API cleanup the memory taken for Contention Free Rach
 *                  This memory should be deleted at the time of cell delete
 ****************************************************************************/
 void cleanpContentionFreeRACHPool(InternalCellIndex internalCellIndex)
{ 
    UInt8 loopIndex=0;
    ContentionFreePreamble *contFreePreamble_p = PNULL;
    while(loopIndex < MAC_MAX_NUM_OF_RA_PREAMBLES)
    {
        contFreePreamble_p =
            contentionFreeRACHPool_gp[internalCellIndex]->
            contentionFreePreambleArray
            [loopIndex];

        if( PNULL != contFreePreamble_p )
        {
            freeMemPool(contFreePreamble_p);

            contentionFreeRACHPool_gp[internalCellIndex]->
                contentionFreePreambleArray
                [loopIndex] = PNULL;
        }
        loopIndex++;
    }
}
/* Rel 5.3.1: Memory related changes end*/
/*Cyclomatic Complexity changes - ends here */
/****************************************************************************
 * Function Name  : cleanupRNTIPoolTbl
 * Inputs         : Internal cell index
 * Outputs        : None
 * Returns        : None
 * Description    : This API cleanup the cleanup RNTI pool table, Free RNTI pool
 *                : this is called at cell stop.
 ****************************************************************************/
/* CA Changes start */
void cleanupRNTIPoolTbl(InternalCellIndex internalCellIndex)
    /* CA Changes end */
{
    UInt32 i = 0;
    TempCRNTICtx     *tempCRNTICtx_p = PNULL;

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    /*Temp UE context*/    
    for (i = 0; i < MAX_TEMP_CRNTI_CTX; i++)
    {
        tempCRNTICtx_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[i].tempCRNTICtx_p;

        if (tempCRNTICtx_p)
        {
            if (tempCRNTICtx_p->msg3BuffInfo.msg_p)
            {
                msgFree(tempCRNTICtx_p->msg3BuffInfo.msg_p);
                tempCRNTICtx_p->msg3BuffInfo.msg_p = PNULL;
            }
            freeMemPool(tempCRNTICtx_p);
            tempCRNTICtx_p = PNULL;
            /* SPR 10189 changes start */
            tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[i].tempCRNTICtx_p = PNULL;
            /* SPR 10189 changes end */
        }
        /*SPR 11109 fix start */
        tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[i].nextFreeIndex = (i + 1);
    }
    tempCrntiCtx_gp[internalCellIndex]->freeIndex = 0;
    /*mark nextFreeIndex as invalid for last node*/
    tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[i - 1].nextFreeIndex = INVALID_FREE_INDEX;
    /*SPR 11109 fix end */
/* SPR 21468 Fix Start */
   //Code Removed 
/* SPR 21468 Fix Stop */ 
    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);
}

/*****************************************************************************
 * Function Name  : initTempUECtxFreeIndexPool
 * Inputs         : numCellsConfigured - Number of cells configured
 * Outputs        : None
 * Returns        : None
 * Description    : This will initialize the temporary UE context free index pool 
 *****************************************************************************/
void initTempUECtxFreeIndexPool(UInt8 numCellsConfigured)
{
    UInt32 i = 0;
    UInt8  cellCount = 0;

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    createMemPool(sizeof(TempCRNTICtxInfo), numCellsConfigured);
    for (cellCount = 0; cellCount < numCellsConfigured; cellCount++)
    {
        GET_MEM_FROM_POOL(TempCRNTICtxInfo, tempCrntiCtx_gp[cellCount],sizeof(TempCRNTICtxInfo), PNULL);

        tempCrntiCtx_gp[cellCount]->freeIndex = 0;

        for (i = 0; i < MAX_TEMP_CRNTI_CTX; i++)
        {
            /* SPR 4319 */
            tempCrntiCtx_gp[cellCount]->tempCRNTICtxArray[i].tempCRNTICtx_p = PNULL;
            /* SPR 4319 */
            tempCrntiCtx_gp[cellCount]->tempCRNTICtxArray[i].nextFreeIndex = (i + 1);
        }

        /*mark nextFreeIndex as invalid for last node*/
        tempCrntiCtx_gp[cellCount]->tempCRNTICtxArray[i - 1].nextFreeIndex = INVALID_FREE_INDEX;
    }    

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);

    return;
}

/*****************************************************************************
 * Function Name  : putEntryInRAReqQueue
 * Inputs         : raReqQNode_p - pointer to the RAReqQueueNode struct contains 
 *                  the received RA Request info
 *                  internalCellIndex - Cell-Index at MAC                                            
 * Outputs        : None
 * Returns        : SUCCESS/FAILURE/ERROR
 * Description    : This will push RA req queue node to raReqQueue_g
 *****************************************************************************/
/*CA Changes start  */
MacRetType putEntryInRAReqQueue(RAReqQueueNode *raReqQNode_p, InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{
    RACHConfiguration *rachConfig_p=PNULL;
    UInt32 preambleIndex = INVALID_PREAMBLE_IDX; 

    /*CA Changes start  */
    rachConfig_p = &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
            rachConfigInfo.rachConfiguration);
    /*CA Changes end  */
#ifdef KPI_STATS
    /* + KPI_CA */
    UInt32 activeIndexForMacStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
#endif

    if(rachConfig_p->countOfContentionBasedRAPreambles  > 
            raReqQNode_p->raPreambleInfo.preamble)
    { 
        /* KPI_CHANGES */
#ifdef  KPI_STATS
        if(rachConfig_p->numOfPreamblesInGroupA > 
                raReqQNode_p->raPreambleInfo.preamble)
        {
            gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats].
                totalRcvdRandomAccessPreamblesGroupA ++;
        }
        else
        {
            gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats].
                totalRcvdRandomAccessPreamblesGroupB ++;
            /* - KPI_CA */
        }
#endif
        /* KPI_CHANGES */

        /* SPR 3854 Start*/
        /*CA Changes start  */
        if (pushNode(raReqQueue_gp[internalCellIndex], &raReqQNode_p->raNodeAnchor))
            /*CA Changes end  */
            /* SPR 3854 End*/
            /* SPR 1868 changes end */

        {
            LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,
                    LOGWARNING,
                    L2_SYS_FAIL,
                    getCurrentTick(),
                    LINE_NUMBER,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    __FILE__,
                    FUNCTION_NAME);


            freeMemPool(raReqQNode_p);

            return MAC_FAILURE;
        }

        LOG_MAC_MSG(MAC_RACH_IND_RCVD_ID,
                LOGINFO,
                MAC_RACH,
                getCurrentTick(),
                raReqQNode_p->raRNTI,
                raReqQNode_p->ttiRAReqReceived,
                raReqQNode_p->raPreambleInfo.preamble,
                raReqQNode_p->raPreambleInfo.timingAdvance,
                raReqQNode_p->raPreambleInfo.tmcs,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,
                "CBR_RA_REQ_RECVD");

    }
    else 
    {
        if(raReqQNode_p->raPreambleInfo.preamble < MAC_MAX_NUM_OF_RA_PREAMBLES)
        {

            /* KPI_CHANGES */            
#ifdef  KPI_STATS
            /* + KPI_CA */
            gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats].
                totalRcvdRandomAccessPreamblesContentionFree ++;
            /* - KPI_CA */
#endif
            /* KPI_CHANGES */           
            preambleIndex = raReqQNode_p->raPreambleInfo.preamble;

            if ( PREAMBLE_BUSY !=  
                    /*CA Changes start  */
                    getPreambleStatusInfo(preambleIndex, internalCellIndex))
                /*CA Changes end  */
            {
#ifdef PERF_STATS
                /* + PERF_CA */
                gMacCellPerfStats_p[internalCellIndex]->lteCellMacULPerfStats.totalUnassignedFalseCFRA++;
                /* - PERF_CA */
#endif
                LOG_MAC_MSG(CFR_RA_REQ_FREE_ID,
                        LOGERROR,
                        MAC_RACH,
                        getCurrentTick(),
                        raReqQNode_p->raRNTI,
                        raReqQNode_p->ttiRAReqReceived,
                        raReqQNode_p->raPreambleInfo.preamble,
                        raReqQNode_p->raPreambleInfo.tmcs,
                        LINE_NUMBER,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"RECVD_CFR_PREAMBLE_NOT_BUSY");

                freeMemPool(raReqQNode_p);

            }
            else
            {
                /*Error Removing Start*/
                ContentionFreePreamble *localContentionFreePreamble_p  =
                    /*CA Changes start  */
                    getContentionFreePreambleInfo(preambleIndex, internalCellIndex);
                /*CA Changes end  */
                /*Error Removing Start*/

				/* ysl add 20190912: fix ho crash - if another CFRA preamble comes and previous msg3 is in retransmission, ul harq process maintenance has some problem.
									So drop this new CFRA preamble */
				if (1 == localContentionFreePreamble_p->contentionFreeRACHRecvd)
				{
					return MAC_FAILURE;
				}

                /* SPR 3854 Start*/
                /*CA Changes start  */
                if (pushNode(contentionFreeRAReqQueue_gp[internalCellIndex], 
                            &raReqQNode_p->raNodeAnchor))
                    /*CA Changes end  */
                    /* SPR 3854 End*/

                {
                    LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,
                            LOGWARNING,
                            L2_SYS_FAIL,
                            getCurrentTick(),
                            LINE_NUMBER,DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            __FILE__,
                            __func__);

                    freeMemPool(raReqQNode_p);

                    return MAC_FAILURE;
                }


                /* SPR_4262_FIX Start */ 
                localContentionFreePreamble_p->contentionFreeRACHRecvd = 1;
                /* SPR_4262_FIX End */ 


                LOG_MAC_MSG(MAC_RACH_IND_RCVD_ID,
                        LOGINFO,
                        MAC_RACH,
                        getCurrentTick(),
                        raReqQNode_p->raRNTI,
                        raReqQNode_p->ttiRAReqReceived,
                        raReqQNode_p->raPreambleInfo.preamble,
                        raReqQNode_p->raPreambleInfo.timingAdvance,
                        raReqQNode_p->raPreambleInfo.tmcs,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,
                        "CFR_RA_REQ_RECVD");
            }
        }
        else
        {
            LOG_MAC_MSG(RECVD_INVALID_PREAMBLE,
                    LOGERROR,
                    MAC_RACH,
                    getCurrentTick(),
                    raReqQNode_p->raPreambleInfo.preamble,
                    raReqQNode_p->raRNTI,
                    raReqQNode_p->ttiRAReqReceived,
                    DEFAULT_INT_VALUE,
                    LINE_NUMBER,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"RECVD_INVALID_PREAMBLE");

            freeMemPool(raReqQNode_p);
            return MAC_FAILURE;
        }

    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : putEntryInULNegAckRARspQueue
 * Inputs         : raRspQnode_p - Pointer to the DLRARspQueueNode structure
 *                  internalCellIndex - Cell-Index at MAC                                            
 * Outputs        : None
 * Returns        : None
 * Description    : This will creates and Inserts the Negative RAR ACK node in  
 *                  rarNegativeToULSchedulerQ_g
 *****************************************************************************/
/*CA Changes start  */
void putEntryInULNegAckRARspQueue(DLRARspQueueNode *raRspQnode_p, InternalCellIndex internalCellIndex)
{
    ULRARspNegAckQueueNode *raRspNegAckQNode_p = PNULL;

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    GET_MEM_FROM_POOL(ULRARspNegAckQueueNode , 
            raRspNegAckQNode_p , 
            sizeof(ULRARspNegAckQueueNode), 
            PNULL);

    /*Coverity-5.3.0 CID-24515 */
    if( PNULL != raRspNegAckQNode_p )
    {
        raRspNegAckQNode_p->raRspQnode_p = raRspQnode_p;
        /*CA Changes start  */
        if (pushNode(ulRARspNegAckQueue_gp[internalCellIndex], &(raRspNegAckQNode_p->rarNodeAnchor)))
            /*CA Changes end  */
        {
            LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                    getCurrentTick(),\
                    __LINE__,0,0,0,0,\
                    0.0,0.0,__FILE__,__func__);
            freeMemPool(raRspNegAckQNode_p);
        }  

        LTE_MAC_UT_LOG(LOG_DETAIL,RACH_MGR,
                "[%s]: Negative Acknowledgement sent to UL\n"
                "           for RA-RNTI [%d]\n",
                __func__,raRspQnode_p->rarToDl.raRNTI);
    }
    else        
    {
        LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
                getCurrentTick(), __LINE__, 
                sizeof(ULRARspNegAckQueueNode), 0,0,0,
                0.0,0.0,__FILE__,__func__);
    }
    /*Coverity-5.3.0 CID-24515 */
    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);
    return;
}
/*CA Changes end  */


/*****************************************************************************
 * Function Name  : putEntryInRachDemuxQueue
 * Inputs         : msg - Type of msg received
 *                  tcrnti - Temporary CRNTI
 *                  crnti - CRNTI received
 *                  receivedCCCH_Msg - ccch message received from UE
 *                  length - Message Length
 *                  receivedTTI - TTI in which data from UE is received
 *                  dlAdvance - Downlink advance 
 *                  internalCellIndex - Cell-Index at MAC                                            
 * Outputs        : None
 * Returns        : MacRetType
 * Description    : This will push DemuxRACH queue node to demuxRachManagerInterfaceQueue_g
 *****************************************************************************/
MacRetType putEntryInRachDemuxQueue(RachDemuxMsgType msg,
        UInt16 tcrnti,
        UInt16 crnti,
        UInt8  *receivedCCCH_Msg,
        UInt32 length,
        /* SPR 15909 fix start */
        tickType_t receivedTTI,
        /* SPR 15909 fix end */
        UInt32 dlAdvance,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    RachDemuxQueueNode *rachDemuxQueueNode_p = PNULL;

    GET_MEM_NODE_DEMUX_RACH_MANAGER_INTERFACE_Q(rachDemuxQueueNode_p,
            RachDemuxQueueNode);

    /* + Coverity 24514 */
    if( PNULL == rachDemuxQueueNode_p )
    {
        return MAC_FAILURE ;
    }

    /* - Coverity 24514 */

    rachDemuxQueueNode_p->msg = msg;
    rachDemuxQueueNode_p->tcrnti = tcrnti;
    rachDemuxQueueNode_p->crnti = crnti;
    rachDemuxQueueNode_p->receivedCCCH_Msg = receivedCCCH_Msg;
    rachDemuxQueueNode_p->length = length;
    rachDemuxQueueNode_p->receivedTTI = receivedTTI;
    rachDemuxQueueNode_p->dlAdvance = dlAdvance;
    if(ENQUEUE_DEMUX_RACH_MANAGER_INTERFACE_Q(RachDemuxQueueNode, 
                /*CA Changes start  */
                (void*)&(rachDemuxQueueNode_p->rachDemuxNodeAnchor), internalCellIndex))
        /*CA Changes end  */
    {
        return MAC_SUCCESS;
    }

    LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
            getCurrentTick(),\
            __LINE__,0,0,0,0,\
            0.0,0.0,__FILE__,__func__);

    FREE_MEM_NODE_DEMUX_RACH_MANAGER_INTERFACE_Q(rachDemuxQueueNode_p);
    return MAC_FAILURE;
}
/*****************************************************************************
 * Function Name  : putEntryInRachDLStrategyQueue 
 * Inputs         : ueIndex- The Id of UE which needs to be pushed into queue
 *                  syncLossDetectedTTI 
 *                  internalCellIndex - Cell-Index at MAC                                            
 * Outputs        : None
 * Returns        : MacRetType 
 * Description    : This will push RACHDLStrategy queue node to rachDLStrategyInterfaceQueue_g
 ******************************************************************************/
/* SPR 15909 fix start */
/* +- SPR 18268 */
MacRetType putEntryInRachDLStrategyQueue(UInt16 ueIndex, tickType_t syncLossDetectedTTI,InternalCellIndex internalCellIndex)
    /* +- SPR 18268 */
    /* SPR 15909 fix end */
{
    /* + Coverity 24513 */
    RachDLStrategyQueueNode *rachDLStrategyQueueNode_p = PNULL ;

    GET_MEM_NODE_RACH_DL_STRATEGY_INTERFACE_Q(rachDLStrategyQueueNode_p,RachDLStrategyQueueNode);

    if( PNULL == rachDLStrategyQueueNode_p )
    {
        return MAC_FAILURE ;
    }
    /* - Coverity 24513 */

    rachDLStrategyQueueNode_p->ueIndex = ueIndex;
    rachDLStrategyQueueNode_p->syncLossDetectedTTI = syncLossDetectedTTI;
    if(ENQUEUE_RACH_DL_STRATEGY_INTERFACE_Q(RachDLStrategyQueueNode, 
                /*CA Changes start  */
                (void *)&(rachDLStrategyQueueNode_p->rachDLStrategyNodeAnchor), internalCellIndex))
        /*CA Changes end  */
    {
        return MAC_SUCCESS;
    }

    LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
            getCurrentTick(),\
            __LINE__,0,0,0,0,\
            0.0,0.0,__FILE__,__func__);

    FREE_MEM_NODE_RACH_DL_STRATEGY_INTERFACE_Q(rachDLStrategyQueueNode_p);
    return MAC_FAILURE;
}


/*****************************************************************************
 * Function Name  : putEntryInPdcchOrderTimerTriggerQueue
 * Inputs         : ueIndex- The Id of UE which needs to be pushed into queue
 *                  preambleIndex-preamble ID for the UE 
 *                  pdcchOrderTriggerTTI 
 *                  internalCellIndex - Cell-Index at MAC                                            
 * Outputs        : None
 * Returns        : MacRetType 
 * Description    : This will push RACHPdcch queue node to pdcchRachInterfaceQueue_g
 ******************************************************************************/
/* SPR 15909 fix start */
/* +- SPR 18268 */
MacRetType putEntryInPdcchOrderTimerTriggerQueue(UInt16 ueIndex, UInt8
        /* +- SPR 18268 */
        preambleIndex, tickType_t pdcchOrderTriggerTTI,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/* SPR 15909 fix end */
/*CA Changes end  */
{
    /* + Coverity 24511 */
    PdcchOrderTimerTriggerQueueNode *pdcchOrderTimerTriggerQueueNode_p = PNULL;

    GET_MEM_NODE_PDCCH_ORDER_TIMER_TRIGGER_Q(pdcchOrderTimerTriggerQueueNode_p,PdcchOrderTimerTriggerQueueNode); 

    if( PNULL == pdcchOrderTimerTriggerQueueNode_p )
    {
        return MAC_FAILURE ;
    }
    /* - Coverity 24511 */

    pdcchOrderTimerTriggerQueueNode_p->ueIndex = ueIndex;
    pdcchOrderTimerTriggerQueueNode_p->preambleIndex = preambleIndex;
    pdcchOrderTimerTriggerQueueNode_p->pdcchOrderTriggerTTI = pdcchOrderTriggerTTI;

    /*CA Changes start  */
    if(ENQUEUE_PDCCH_ORDER_TIMER_TRIGGER_Q(PdcchOrderTimerTriggerQueueNode, 
                (void *)&(pdcchOrderTimerTriggerQueueNode_p->pdcchTimeTriggerNodeAnchor), internalCellIndex))
        /*CA Changes end  */
    {
        return MAC_SUCCESS;
    }

    LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
            getCurrentTick(),\
            __LINE__,0,0,0,0,\
            0.0,0.0,__FILE__,__func__);
    FREE_MEM_NODE_PDCCH_ORDER_TIMER_TRIGGER_Q(pdcchOrderTimerTriggerQueueNode_p);
    return MAC_FAILURE;
}
/*****************************************************************************
 * Function Name  : putEntryInPdcchRachInterfaceQueue
 * Inputs         : msgType - Type of RACH 
 *                  rachInterface_p 
 *                  internalCellIndex - Cell-Index at MAC                                            
 * Outputs        : None
 * Returns        : MacRetType 
 * Description    : This will push RachInterfaceQueueNode into the queue
 ******************************************************************************/
MacRetType putEntryInPdcchRachInterfaceQueue(RachType msgType, void *rachInterface_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex )
/*CA Changes end  */
{
    /* + Coverity 24512 */
    RachInterfaceQueueNode *rachInterfaceQueueNode_p = PNULL ;
    GET_MEM_NODE_PDCCH_RACH_INTERFACE_Q(rachInterfaceQueueNode_p,RachInterfaceQueueNode );

    if( PNULL == rachInterfaceQueueNode_p )
    {
        return MAC_FAILURE ;
    }
    /* - Coverity 24512 */

    rachInterfaceQueueNode_p->msgType = msgType;
    rachInterfaceQueueNode_p->rachInterface_p = rachInterface_p;

    /*CA Changes start  */
    if(ENQUEUE_PDCCH_RACH_INTERFACE_Q(RachInterfaceQueueNode, 
                rachInterfaceQueueNode_p, internalCellIndex))
        /*CA Changes end  */
    {
        return MAC_SUCCESS;
    }

    LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
            getCurrentTick(),\
            __LINE__,0,0,0,0,\
            0.0,0.0,__FILE__,__func__);

    FREE_MEM_NODE_PDCCH_RACH_INTERFACE_Q(rachInterfaceQueueNode_p);
    return MAC_FAILURE;
}

/*****************************************************************************
 * Function Name  : processRAReqAndNegRARspQueue
 * Inputs         : currentGlobalTTI - Current global TTI
 *                  PDCCHDelay       - delay from PDCCH to Phy
 *                  sfn - system frame number
 *                  sf - sub frame number
 *                  sfType - sf type in TDD weather its Dl/UL
 *                  internalCellIndex - Cell-Index at MAC                                            
 * Outputs        : None
 * Returns        : None
 * Description    : This will calculates the number of RAR to be schedulted in
 *                  this TTI based on the available RBs and calls the 
 *                  processRaReqQueue() and processNegRaRspQueue()
 *****************************************************************************/
/* SPR 15909 fix start */
void processRAReqAndNegRARspQueue(tickType_t currentGlobalTTI, 
        /* SPR 15909 fix end */
        UInt32 PDCCHDelay,
        UInt32 sfn,
        UInt32 sf
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT_TDD)
#ifdef TDD_CONFIG
        ,ULDLSPFrame sfType
#endif
#endif
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */
        ,ExecutionLegFunctionArg* execArgs)
/*TDD Tick overshoot changes end*/
{
    /* + Coverity 54529 */
    UInt32 rbMapNum  = 0;
    /*Maximum number of RAR to schedule in this TTI*/
    UInt32 maxRAR    = 0;
    UInt32 freeULRB  = 0;
    UInt32 freeULRBForRAR  = 0;
    UInt32 rarPduCount = 0;
    UInt32 negAckQueueCount = 0;
    UInt32 raReqQueueCount = 0;
    UInt32 conFreeRaQueueCount = 0;
    /*SPR 16773 fix start */
    UInt32 count =0;
    /*SPR 16773 fix end */
    /* msg3 ulconfig fix start */
#ifdef TDD_CONFIG
    UInt8 subFrameForRAR = MAX_SUB_FRAME;
    UInt8 ulSubFrame = MAX_SUB_FRAME;
#endif    
    /* msg3 ulconfig fix end */

    RarPduTobeScheduledInTTI raResponsesInTTI = {0,};
    RAReqTobeScheduledInPdu *rarPduTobeScheduled_p = PNULL;

    /*This is bit map to represent RARNTI (0 - 31) scheduled*/
    UInt32 scheduledRARNTIBitMap1 = 0;
    UInt8 numBitsAbsPattern = 0;

    /*This is bit map to represent RARNTI (32 - 59) scheduled*/
    UInt32 scheduledRARNTIBitMap2 = 0;
    /*CA Changes start  */
    InitParams       *initParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /*CA Changes end  */

    /* SPR 15909 fix start */
    tickType_t globalTickForPucchCal = 0;
    /* SPR 15909 fix end */
    /* 5492 changes start */
    /* SPR 4063 Start */
    UInt32 maxUEsToBeScheduledUL  
        /*CA Changes start  */
        = initParams_p->ulSchdConfig.maxUEsToBeScheduledUL;
    /*CA Changes end  */
    /* SPR 4063 End */
    /* 5492 changes end */
    /* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
    UInt8 spsArrayTti = MAX_SPS_INTERVAL;
#elif TDD_CONFIG
    UInt8 spsArrayTti = MAX_UL_SPS_INTERVAL;

#endif
    /* - SPS_TDD_Changes */

    RACHConfiguration *rachConfig_p = 
        /*CA Changes start  */
        &(cellParams_p->rachConfigInfo.rachConfiguration);
    /*CA Changes end  */


    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    raResponsesInTTI.numRarPdu = 0;

    /*Update the RAR Window size*/
    if (10 == (sf + PDCCHDelay))
    {
        sfn = sfn +1;

        if (MAX_SFN_VALUE == sfn)
        {
            sfn = 0;
        }
    }

    /*Update the RAR window size value if new configuration received from RRC*/
    /*CA Changes start  */
    UPDATE_RAR_WINDOW_SIZE_ACTIVE_INDEX(cellParams_p->rachConfigInfo, sfn);

    /* SPR 16773 fix start */
    /* Process create UE timer list if list not empty */
    count = listCount(createUeContextTimerList_gp[internalCellIndex]);
    if( count )
    {
        processCreateUETimerList( currentGlobalTTI, count,internalCellIndex );
    }
    /* SPR 16773 fix end */
    /* Cyclomatic_complexity_changes_start */
    processQueuesBeforeRARspQueue(currentGlobalTTI,internalCellIndex, execArgs);
    /* Cyclomatic_complexity_changes_end */

    /* SPR 1868 changes start */

    /*TDD Tick overshoot changes start*/
#ifdef TDD_CONFIG
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT_TDD)
    if(DL == sfType)
#endif
#endif
    {
        /*CA Changes start  */
        raReqQueueCount = sQueueCount(raReqQueue_gp[internalCellIndex]);
        /*CA Changes end  */
        /* SPR 1868 changes end */

        conFreeRaQueueCount = 
            /* SPR 3854 Start*/
            /*CA Changes start  */
            sQueueCount(contentionFreeRAReqQueue_gp[internalCellIndex]);
        /* SPR 3854 End*/
        negAckQueueCount = sQueueCount(ulRARspNegAckQueue_gp[internalCellIndex]);
        /*CA Changes end  */

        if ((raReqQueueCount) ||
                (negAckQueueCount) ||
                (conFreeRaQueueCount))
        {
            /* msg3 ulconfig fix start */
#ifdef TDD_CONFIG    
            subFrameForRAR = 
                ((currentGlobalTTI + PDCCHDelay + PHY_DL_DELAY) % MAX_SUB_FRAME);
            rbMapNum = (currentGlobalTTI + PDCCHDelay + PHY_DL_DELAY + 
                    getULSfForMsg3(subFrameForRAR, internalCellIndex)) % getMaxULRBMaps(internalCellIndex);
            globalTickForPucchCal = (currentGlobalTTI + PDCCHDelay + PHY_DL_DELAY + 
                    getTickForPUCCHCalForMsg3(subFrameForRAR, internalCellIndex)) ;
            ulSubFrame = (subFrameForRAR + getULSfForMsg3(subFrameForRAR, internalCellIndex)) % MAX_SUB_FRAME;
            /* msg3 ulconfig fix end */
            /*UL RB map is maintained for 16 sub frames*/
            /* SPR 1600 changes start */
#elif FDD_CONFIG
            rbMapNum = 
                MODULO_SIXTEEN((currentGlobalTTI + PDCCHDelay + PHY_DL_DELAY + UE_RA_ADV_TTI));

            globalTickForPucchCal = currentGlobalTTI + PDCCHDelay + PHY_DL_DELAY + UE_RA_ADV_TTI ;
#endif        

            processAdvancePUCCHAndPRACHReports(rbMapNum, globalTickForPucchCal
#ifdef TDD_CONFIG
                    , ulSubFrame
#endif
                    /*CA Changes start  */
                    ,internalCellIndex                    
                    /*CA Changes end  */
                    );
            /*Call UL resource allocator API to find the free RBs*/
            /*CA Changes start  */
            freeULRB = getFreePUSCHResources(rbMapNum, internalCellIndex);
            /*CA Changes end  */

            /*Based on the free UL RB available calculated the maximum number of 
             *RARs to be scheduled
             */
            freeULRBForRAR = (UInt32)(freeULRB * PERCENTAGE_OF_UL_RB_FOR_RAR);
            maxRAR = freeULRBForRAR/rachConfig_p->minimumRBReqdForSingleRAR;

            /* + SPS_TDD_Changes */
            if(TRUE == initParams_p->spsSysWideParam.enableSps)
            {
#ifdef FDD_CONFIG
                /*SPS CHANGES */
                spsArrayTti = (currentGlobalTTI + PDCCHDelay + PHY_DL_DELAY + UE_RA_ADV_TTI - FDD_HARQ_OFFSET)%MAX_SPS_INTERVAL;
#elif TDD_CONFIG
                spsArrayTti = (currentGlobalTTI + PDCCHDelay + PHY_DL_DELAY + getULSfForMsg3(subFrameForRAR,internalCellIndex)) % MAX_UL_SPS_INTERVAL;
#endif
                /*CA Changes start  */
                markULSpsRBsInRBMap(rbMapNum, spsArrayTti, RAR_LOCATION_UPDATE
#ifdef FDD_CONFIG
                        ,currentGlobalTTI, PDCCHDelay 
#endif
#ifdef TDD_CONFIG
                        ,subFrameForRAR
#endif
                        ,internalCellIndex);
                /*CA Changes end  */
                /*SPS CHANGES */
            }
            /* - SPS_TDD_Changes */

            /*If maxRAR is 0 Send atleast one RAR*/
            if (!maxRAR)
            {
                maxRAR = 1;
                if (freeULRB >= rachConfig_p->maxRBReqdForAnyRAR)
                {
                    freeULRBForRAR = rachConfig_p->maxRBReqdForAnyRAR;
                }
                else  
                {
                    freeULRBForRAR = freeULRB; 
                } 
            }
            /* Spr 23393 Changes Start */
            else if (maxRAR > macParams_g.rarPduPerTTI)
            {
                maxRAR = macParams_g.rarPduPerTTI ;
            }
            /* Spr 23393 Changes End */
            /* 5492 changes start */
            /* SPR 4063 Start */
            if(maxRAR > maxUEsToBeScheduledUL)
            {
                maxRAR = maxUEsToBeScheduledUL;
            }
            /* SPR 4063 End */
            /* 5492 changes end */

            /*L2_Fix_start*/
            /* EICIC Change Phase1 */
            UInt8 absSubFrameNum = 0;
            UInt32 sysFrameNum = 0;
            UInt32 subFrameNum = 0;
            getSFAndSFN(&sysFrameNum, &subFrameNum,internalCellIndex);


#ifdef FDD_CONFIG
            numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
#else
            if (UL_DL_CONFIG_0 == cellSpecificParams_g.cellConfigAndInitParams_p\
                    [internalCellIndex]->cellParams_p->subFrameAssign)
            {
                numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
            }
            else if (UL_DL_CONFIG_6 == cellSpecificParams_g.cellConfigAndInitParams_p\
                    [internalCellIndex]->cellParams_p->subFrameAssign)
            {
                numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
            }
            else
            {
                numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
            }
#endif


            absSubFrameNum = ((((sysFrameNum )*MAX_SUB_FRAME) +(subFrameNum + PDCCHDelay + PHY_DL_DELAY )) %numBitsAbsPattern) ;

            if (LTE_TRUE == absPattern_g[internalCellIndex][absPatternIndex_g[internalCellIndex]][absSubFrameNum])
            {
                isCurrentSfAbs_g[internalCellIndex] =  LTE_TRUE; 
            }
            else
            {
                isCurrentSfAbs_g[internalCellIndex] = LTE_FALSE;
            }

            if (LTE_TRUE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum])
            {
                isCurrentSfAbsV_g[internalCellIndex] =  LTE_TRUE; 
            }
            else
            {
                isCurrentSfAbsV_g[internalCellIndex] = LTE_FALSE;
            }   

            if((LTE_TRUE == isCurrentSfAbs_g[internalCellIndex]))
            {
                maxRAR=0; /* Max rar is set to zero to restrict code flow in case of Aggressor cell ,RaR  queues
                             are processed if maxrar is non-zero */
            }
            /* EICIC Change Phase1 */   
            LOG_MAC_MSG(MAC_MAX_RB_FOR_RAR_ID,
                    LOGDEBUG,
                    MAC_RACH,
                    getCurrentTick(),
                    maxRAR,
                    rbMapNum,
                    freeULRB,
                    freeULRBForRAR,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,
                    "MAC_MAX_RB_FOR_RAR_ID");
            /*L2_Fix_end*/



            if (negAckQueueCount)
            {
                /* SPR 2446 Fix Begins*/
                processNegRaRspQueue(currentGlobalTTI,
                        &freeULRBForRAR,
                        &maxRAR,
                        negAckQueueCount,
                        &scheduledRARNTIBitMap1,
                        &scheduledRARNTIBitMap2,
                        &raResponsesInTTI,
                        /*CA Changes start  */
                        internalCellIndex);
                /*CA Changes end  */
                /* SPR 2446 Fix Ends*/
            }


            if(conFreeRaQueueCount)        
            {   
                if (maxRAR) 
                {
                    if ((freeULRBForRAR >= 
                                rachConfig_p->numRBReqdForConFreePDCCHOrder) ||
                            (freeULRBForRAR >=
                             rachConfig_p->numRBReqdForConFreeHandOver))
                    {
                        processContFreeRAReqQueue(
                                &freeULRBForRAR,
                                &maxRAR,
                                currentGlobalTTI,
                                PDCCHDelay,
                                &scheduledRARNTIBitMap1,
                                &scheduledRARNTIBitMap2,
                                &raResponsesInTTI,
                                /*CA Changes start  */
                                internalCellIndex);
                        /*CA Changes end  */
                    }
                }
            }

            if(raReqQueueCount)
            {    
                if (maxRAR) 
                {    
                    if(freeULRBForRAR >= 
                            rachConfig_p->numRBReqdForMSG3GroupARACHRequest)
                    {
                        processRaReqQueue(
                                &freeULRBForRAR,
                                &maxRAR,
                                currentGlobalTTI,
                                PDCCHDelay,
                                &scheduledRARNTIBitMap1,
                                &scheduledRARNTIBitMap2,
                                &raResponsesInTTI,
                                /*CA Changes start  */
                                internalCellIndex);
                        /*CA Changes end  */

                    }
                }
            }    
            if(raResponsesInTTI.numRarPdu)
            {
                for(rarPduCount = 0; rarPduCount < raResponsesInTTI.numRarPdu; 
                        rarPduCount++)
                {

                    rarPduTobeScheduled_p =  
                        &raResponsesInTTI.rarPduTobeScheduled[rarPduCount];  

                    prepareAndSendRARToDlScheduler(rbMapNum,
                            PDCCHDelay,
                            currentGlobalTTI,
                            rarPduTobeScheduled_p,
                            internalCellIndex,
                            execArgs);
                }

            }
        }
        /*TDD Tick overshoot changes start*/
    }
    /*TDD Tick overshoot changes end*/
    /* + Coverity 54529 */
}
/*****************************************************************************
 * Function Name  : processNegRaRspQueue
 * Inputs         : currentGlobalTTI - Curret TTI
 *                  freeULRBForRAR_p - Pointer to Number of RB free in UL for RAR
 *                  queueCount - ulRARspNegAckQueue_g queue count
 *                  internalCellIndex - Cell-Index at MAC                                            
 * Outputs        : maxRAR_p - Maximu no.of RAR to schedule for this TTI
 *                  scheduledRARNTIBitMap1_p - RARNTIs scheduled for this TTI
 *                                             bit map (RARNTI range 0-31)
 *                  scheduledRARNTIBitMap2_p - RARNTIs scheduled for this TTI
 *                                             bit map (RARNTI range 32-59)
 * Returns        : None
 * Description    : This will process the negative RAR queue, free the UL
 *                  resources and reschedules the RAR message if the RAR window
 *                  is not expired
 *****************************************************************************/
/* SPR 15909 fix start */
void processNegRaRspQueue(tickType_t currentGlobalTTI,
        /* SPR 15909 fix end */
        UInt32* freeULRBForRAR_p,
        UInt32 *maxRAR_p,
        UInt32 queueCount,
        UInt32 *scheduledRARNTIBitMap1_p,
        UInt32 *scheduledRARNTIBitMap2_p,
        RarPduTobeScheduledInTTI *raResponsesInTTI_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    UInt32 i = 0;
    DLRARspQueueNode       *raRspQnode_p = PNULL;
    ULRARspNegAckQueueNode *raRspNegAckQNode_p = PNULL;
    UInt32 flag = 1;
    DLRARsp *dlRARsp_p = PNULL;
    RACHConfiguration *rachConfig_p = 
        /*CA Changes start  */
        &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->rachConfigInfo.rachConfiguration);
    /*CA Changes end  */


    for (i = 0; ((i < queueCount) 
                && (*maxRAR_p)
                ) ; i++)
    {
        raRspNegAckQNode_p = 
            /*CA Changes start  */
            (ULRARspNegAckQueueNode *)popNode(ulRARspNegAckQueue_gp[internalCellIndex]);
        /*CA Changes end  */

        if (raRspNegAckQNode_p)
        {
            raRspQnode_p = raRspNegAckQNode_p->raRspQnode_p;

            /*(end of rar window) - DL delay - Phy delay*/
            if (currentGlobalTTI > raRspQnode_p->rarToDl.exptti)
            {
                LOG_MAC_MSG(MAC_NEG_RAR_TTI_EXPIRED_ID,LOGWARNING,MAC_RACH,
                        getCurrentTick(),
                        raRspQnode_p->rarToDl.raRNTI,
                        raRspQnode_p->rarToDl.numRB,
                        raRspQnode_p->rarToDl.mcsIndex,
                        raRspQnode_p->rarToDl.exptti,
                        raRspQnode_p->rarToDl.tti,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,
                        "NEG_RAR_TTI_EXPIRED");

                dlRARsp_p = &raRspQnode_p->rarToDl;

                freeRARBeforeULRBAllocation(flag,
                        PNULL,
                        currentGlobalTTI,
                        dlRARsp_p,
                        /*CA Changes start  */
                        internalCellIndex);
                /*CA Changes end  */

                UInt32 x = 0;

                /*Free the existing UL RBs allocated*/
                FREE_UL_RAR_RB(raRspNegAckQNode_p->raRspQnode_p->rarToDl,
                        x, raRspNegAckQNode_p->raRspQnode_p->rarToDl.numRAR,
                        /*CA Changes start  */
                        internalCellIndex);
                /*CA Changes end  */
                msgFree(raRspQnode_p->rarToDl.macPduData_p);
                freeMemPool(raRspQnode_p);
                freeMemPool(raRspNegAckQNode_p);
            }
            else
            {


                if ((*freeULRBForRAR_p) >= 
                        rachConfig_p->minimumRBReqdForSingleRAR)  
                {
                    processRAResInNegRARPDU(
                            freeULRBForRAR_p,
                            raRspNegAckQNode_p,
                            maxRAR_p,
                            scheduledRARNTIBitMap1_p,
                            scheduledRARNTIBitMap2_p,
                            raResponsesInTTI_p,
                            /*CA Changes start  */
                            /* +- SPR 17777 */
                            internalCellIndex
#ifdef TDD_CONFIG
                            ,currentGlobalTTI
#endif
                            );
                            /* +- SPR 17777 */
                }
                else
                {

                    LOG_MAC_MSG(MAC_NEG_LESS_ULRB_ID,
                            LOGWARNING,
                            MAC_RACH,
                            getCurrentTick(),
                            (*freeULRBForRAR_p),
                            rachConfig_p->minimumRBReqdForSingleRAR,
                            (*maxRAR_p),
                            raRspQnode_p->rarToDl.raRNTI,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"NEG_RB_ALLOC_FAIL");

                    /*CA Changes start  */
                    putEntryInULNegAckRARspQueue(raRspQnode_p, internalCellIndex);
                    /*CA Changes end  */
                    break;

                }
            }
        }
        else
        {
            break;
        }
    }
}

/*****************************************************************************
 * Function Name  : setProcessRntiFlag
 * Inputs         : raRNTI
 *                  scheduledRARNTIBitMap1_p
 *                  scheduledRARNTIBitMap2_p
 *                  processRntiFlag_p
 * Outputs        : None 
 * Returns        : None
 * Description    : Set the flag if RNTI is in range  
 *****************************************************************************/
STATIC  void setProcessRntiFlag(UInt16 raRNTI, 
        UInt32* scheduledRARNTIBitMap1_p,
        UInt32* scheduledRARNTIBitMap2_p,
        UInt8* processRntiFlag_p)
{
    if (raRNTI > 31)
    {
        if ((*scheduledRARNTIBitMap2_p) & 
                (1 << (raRNTI - 31)))
        {
            *processRntiFlag_p = 1;
        }
    }
    else if ((*scheduledRARNTIBitMap1_p) & 
            (1 << raRNTI))
    {
        *processRntiFlag_p = 1;
    }
}

/*****************************************************************************
 * Function Name  : fillRBAndMCS
 * Inputs         : rachConfig_p
 *                  raReqQNode_p
 *                  freeULRBForRAR_p
 * Outputs        : numRBAllocated_p -pointer to fill number of RBs
 *                  mcsIndex_p - pointer to fill MCS
 * Returns        : MacRetType 
 * Description    : fills the Rsource block and MCS 
 *****************************************************************************/
STATIC  MacRetType  fillRBAndMCS(RACHConfiguration *rachConfig_p,
        RAReqQueueNode* raReqQNode_p,
        UInt32* freeULRBForRAR_p,
        UInt8* numRBAllocated_p,
        UInt8* mcsIndex_p)
{             
    if(rachConfig_p->preambleGroupBConfigPresent)
    {
        if(rachConfig_p->numOfPreamblesInGroupA >
                raReqQNode_p->raPreambleInfo.preamble)
        {    
            if((*freeULRBForRAR_p) < 
                    rachConfig_p->numRBReqdForMSG3GroupARACHRequest) 
            {
                return MAC_FAILURE;
            }
            else
            {
                *numRBAllocated_p = 
                    rachConfig_p->numRBReqdForMSG3GroupARACHRequest;
                *mcsIndex_p = 
                    rachConfig_p->mcsIndexForMSG3GroupARACHRequest;
            }
        }
        else
        {
            if((*freeULRBForRAR_p) < 
                    rachConfig_p->numRBReqdForMSG3GroupBRACHRequest) 
            {
                return MAC_FAILURE;
            }
            else
            {
                *numRBAllocated_p = 
                    rachConfig_p->numRBReqdForMSG3GroupBRACHRequest;
                *mcsIndex_p = 
                    rachConfig_p->mcsIndexForMSG3GroupBRACHRequest;
            }
        }
    }
    else
    {
        if((*freeULRBForRAR_p) < 
                rachConfig_p->numRBReqdForMSG3GroupARACHRequest) 
        {
            return MAC_FAILURE;
        }
        else
        {
            *numRBAllocated_p = 
                rachConfig_p->numRBReqdForMSG3GroupARACHRequest;
            *mcsIndex_p = 
                rachConfig_p->mcsIndexForMSG3GroupARACHRequest;
        }
    }
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : checkMaxRNTIReachedFlag
 * Inputs         : maxRNTIReachedFlag
 *                  tempRaRnti
 *                  lteCellMacULPerfStats
 *                  internalCellIndex - Cell-Index at MAC                                            
 * Outputs        : None 
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : Check the MAX RA RNTI were used.If not return FAILURE
 *                  else free the nodes if same RNTI were used.
 *****************************************************************************/
/* + Coverity_25667 */
STATIC  MacRetType checkMaxRNTIReachedFlag(UInt8 maxRNTIReachedFlag,
        UInt16 tempRaRnti
#ifdef PERF_STATS
        ,LteCellMacULPerfStats* lteCellMacULPerfStats
#endif                          
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex                                                
        /*CA Changes end  */
        )
{                             
    RAReqQueueNode *raReqQNode_p = PNULL; 
    /* - Coverity_25667 */
    if(maxRNTIReachedFlag)
        /*SPR 3429 */
    {   
        /*Check still some nodes left with same RA-RNTI*/
        /*SPR 3854 Start*/
        /*CA Changes start  */
        raReqQNode_p = 
            (RAReqQueueNode*)
            getFirstNode(raReqQueue_gp[internalCellIndex]);
        /*CA Changes end  */
        /*SPR 3854 End*/

        if ((raReqQNode_p) && 
                (tempRaRnti == raReqQNode_p->raRNTI))
        {
            /*Free the rest of the messages with same RARNTI*/
            do 
            {
                /*SPR 3854 Start*/
                /*CA Changes start  */
                raReqQNode_p =
                    (RAReqQueueNode *)
                    popNode(raReqQueue_gp[internalCellIndex]);
                /*CA Changes end  */
                /*SPR 4017 PH2 Start*/
                if(raReqQNode_p)
                {
                    /*SPR 3854 End*/
                    LOG_MAC_MSG(RA_REQ_FREE_ID,
                            LOGDEBUG,
                            MAC_RACH,
                            getCurrentTick(),
                            raReqQNode_p->raRNTI,
                            raReqQNode_p->ttiRAReqReceived,
                            raReqQNode_p->raPreambleInfo.preamble,
                            LINE_NUMBER,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"CBR_FREE_RA_REQ");


                    freeMemPool(raReqQNode_p);
                    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                    lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CBRA++;
#endif
                    /* - PERF_STATS_PH1D1 */
                }

                /*SPR 3854 Start*/
                /*CA Changes start  */
                raReqQNode_p = 
                    (RAReqQueueNode*)
                    getFirstNode(raReqQueue_gp[internalCellIndex]);
                /*CA Changes end  */
                /*SPR 3854 End*/
            }while ((raReqQNode_p) && 
                    (tempRaRnti == raReqQNode_p->raRNTI));
        }
        return MAC_SUCCESS;
    }
    return MAC_FAILURE;
}

/*****************************************************************************
 * Function Name  : fillScheduledRARNTIBitMap
 * Inputs         : raRNTI
 * Outputs        : scheduledRARNTIBitMap1_p or scheduledRARNTIBitMap2_p
 * Returns        : None 
 * Description    : Updates the RA RNTI Bitmap base on the RNTI value
 *****************************************************************************/
STATIC  void fillScheduledRARNTIBitMap(UInt16 raRNTI, 
        UInt32* scheduledRARNTIBitMap1_p,
        UInt32* scheduledRARNTIBitMap2_p)
{
    if (raRNTI > 31)
    {
        *scheduledRARNTIBitMap2_p |= 1 << (raRNTI - 31);
    }
    else
    {
        *scheduledRARNTIBitMap1_p |= 1 << raRNTI;
    }
}

/*****************************************************************************
 * Function Name  : checkPreambleBit 
 * Inputs         : raReqQNode_p - Pointer to RA request node
 *                  duplicatePreambleMap1,duplicatePreambleMap2 
 *                  internalCellIndex - Cell-Index at MAC                                            
 * Outputs        : isContinueNeeded_p 
 * Returns        : None
 * Description    : Checks the Preamble bit if it is duplicate free the memory 
 *                  and update the flag to continue next node 
 *****************************************************************************/
STATIC  void checkPreambleBit(RAReqQueueNode* raReqQNode_p,
        UInt32 duplicatePreambleMap1,
        UInt32 duplicatePreambleMap2,
        UInt8 *isContinueNeeded_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{                                         
    if (CHECK_PREAMBLE_BIT(raReqQNode_p, 
                duplicatePreambleMap1,
                duplicatePreambleMap2))
    {
        /*SPR 3854 Start*/
        /*CA Changes start  */
        popNode(raReqQueue_gp[internalCellIndex]);
        /*CA Changes end  */
        /*SPR 3854 End*/
        /* + Coverity_31966 */
        LOG_MAC_MSG(RA_REQ_FREE_ID,
                LOGDEBUG,
                MAC_RACH,
                getCurrentTick(),
                raReqQNode_p->raRNTI,
                raReqQNode_p->ttiRAReqReceived,
                raReqQNode_p->raPreambleInfo.preamble,
                LINE_NUMBER,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"CBR_FREE_DUP_PREAMBLE");

        freeMemPool(raReqQNode_p);

        /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
        /* + PERF_CA */
        gMacCellPerfStats_p[internalCellIndex]->lteCellMacULPerfStats.totalFailedRandomAccessPreamblesMsg2CBRA++;
        /* - PERF_CA */
#endif
        /* - PERF_STATS_PH1D1 */
        /* - Coverity_31966 */
        *isContinueNeeded_p = TRUE;
    }
}

/*****************************************************************************
 * Function Name  : processRaReqQueue
 * Inputs         : freeULRBForRAR_p - 
 *                  currentGlobalTTI - Curret TTI
 *                  dlDelay - DL delay
 *                  maxRAR_p - Max num of RAR that 
 *                             can be scheduled in this TTI
 *                  scheduledRARNTIBitMap1_p - RARNTIs already scheduled 
 *                                             in this TTI 
 *                                             bit map (RARNTI range 0-31)
 *                  scheduledRARNTIBitMap2_p - RARNTIs already scheduled
 *                                             in this TTI 
 *                                             bit map (RARNTI range 32-59)
 *                  raResponsesInTTI_p       - RAR PDU INFO to be scheduled 
 *                                             in this TTI 
 *                  internalCellIndex - Cell-Index at MAC                                            
 *                                             
 * Outputs        : maxRAR_p - updated Max num of RAR to schedule for this TTI
 *                  scheduledRARNTIBitMap1_p - RARNTIs scheduled for this TTI
 *                                             bit map (RARNTI range 0-31)
 *                  scheduledRARNTIBitMap2_p - RARNTIs scheduled for this TTI
 *                                             bit map (RARNTI range 32-59)
 *                  
 * Returns        : None
 * Description    : This function will 
 *                                  - Process Contention Based RA Queue
 *                                  - Add entry in raResponsesInTTI_p
 *                                    for each RAR PDU
 *                                  - Set the RARRNTI bit map 
 *                                    for each RAR PDU
 *                                  - Decrement Available resource blocks
 *                                  - Decrement MAX RA Responses 
 *****************************************************************************/
void processRaReqQueue(
        UInt32* freeULRBForRAR_p,
        UInt32* maxRAR_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTI,
        /* SPR 15909 fix end */
        UInt32 dlDelay,
        UInt32 *scheduledRARNTIBitMap1_p,
        UInt32 *scheduledRARNTIBitMap2_p,
        RarPduTobeScheduledInTTI *raResponsesInTTI_p,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex)
/*CA Changes end  */
{
    UInt8  maxRNTIReachedFlag = FALSE,processRntiFlag = 0;
    UInt8  *noOfRAResponses_p = PNULL;
    UInt16 raRNTI           = 0;
    UInt16 tcRNTI           = INVALID_RNTI;
    UInt16 tempRaRnti    = 0;
    UInt32 raRspWindowSize  = 0;
    /* SPR 15909 fix start */
    tickType_t dlTxTTI          = 0;
    tickType_t rarWindowEndTTI  = 0;
    /* SPR 15909 fix end */
    UInt32 duplicatePreambleMap1 = 0;
    UInt32 duplicatePreambleMap2 = 0;
    RAReqQueueNode *raReqQNode_p  = PNULL;
    RAReqTobeScheduledInPdu* rarPduTobeScheduled_p = PNULL;
    RAReqTobeScheduled *raReqInPDU_p = PNULL;

    /*CA Changes start  */
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /*CA Changes end  */

    RACHConfiguration *rachConfig_p = 
        /*CA Changes start  */
        &(cellParams_p->rachConfigInfo.rachConfiguration);
    /*CA Changes end  */
    UInt8  numRARCnt     = 0;
    UInt8 numRBAllocated = 0;
    UInt8 mcsIndex = 0;
    UInt32 retValue = MAC_FAILURE;
    UInt8 isContinueNeeded = FALSE;

    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
    /* + PERF_CA */
    LteCellMacULPerfStats *lteCellMacULPerfStats = &(gMacCellPerfStats_p[internalCellIndex]->\
            /* - PERF_CA */
            lteCellMacULPerfStats);
#endif
    /* - PERF_STATS_PH1D1 */

    /*CA Changes start  */
    raReqQNode_p = (RAReqQueueNode *) getFirstNode(raReqQueue_gp[internalCellIndex]);
    /*CA Changes end  */

    if (PNULL == raReqQNode_p)
    { 
        /* SPR 15487 fix start */
        LOG_MAC_MSG(RA_RNTI_PROC_RAR_NOT_FOUND, LOGWARNING, MAC_RACH,
                getCurrentTick(), raResponsesInTTI_p->numRarPdu,
                0, __LINE__, 0,0, 0,0, __func__, "raReqQueue_g" );
        /* SPR 15487 fix end */
        return ;
    }

    raRspWindowSize = 
        /*CA Changes start  */
        cellParams_p->rachConfigInfo.rachConfiguration.
        raRspWindowSize[cellParams_p->rachConfigInfo.rachConfiguration.activeIdxRARspWindow];
    /*CA Changes end  */


    /*save the RARNTI*/
    tempRaRnti = raReqQNode_p->raRNTI;

    processRntiFlag = 0x0;

    numRARCnt = *maxRAR_p;

    /*SPR 3854 Start*/
    /* SPR 15909 fix start */
    tickType_t currentTickWithDelay = 0;
    /* SPR 15909 fix end */
    currentTickWithDelay =  currentGlobalTTI + dlDelay;
    if ((raReqQNode_p->ttiRAReqReceived + MIN_DIFF_RA_RAR) > currentTickWithDelay)
    {
        return; 
    } 
    /*SPR 3854 End*/
    setProcessRntiFlag(raReqQNode_p->raRNTI,scheduledRARNTIBitMap1_p,
            scheduledRARNTIBitMap2_p,&processRntiFlag);
    if(1 == processRntiFlag)
    {

        rarPduTobeScheduled_p = 
            getRARPduInfoforRARNTI(raReqQNode_p->raRNTI,
                    raResponsesInTTI_p);

        if(PNULL == rarPduTobeScheduled_p)
        {
            LOG_MAC_MSG(RA_RNTI_PROC_RAR_NOT_FOUND,
                    LOGWARNING,
                    MAC_RACH,
                    getCurrentTick(),
                    raResponsesInTTI_p->numRarPdu,
                    raReqQNode_p->raRNTI,
                    LINE_NUMBER,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"RA_RNTI_PROC_RAR_NOT_FOUND");

            rarPduTobeScheduled_p = 
                getNextRARPduTobeScheduled(raResponsesInTTI_p);
        }
    }
    else
    {
        rarPduTobeScheduled_p = 
            getNextRARPduTobeScheduled(raResponsesInTTI_p);
    }

    duplicatePreambleMap1 = 
        rarPduTobeScheduled_p->preambleMap1;

    duplicatePreambleMap2 = 
        rarPduTobeScheduled_p->preambleMap2;
    /* SPR 3426 */
    while(
            (*maxRAR_p) && 
            (FALSE == maxRNTIReachedFlag)
         )
        /* SPR 3426 */
    {

        /*SPR 3854 Start*/
        /*CA Changes start  */
        raReqQNode_p = (RAReqQueueNode *) getFirstNode(raReqQueue_gp[internalCellIndex]);
        /*CA Changes end  */
        /*SPR 3854 End*/

        if (raReqQNode_p)
        {            
            /*SPR 3854 Start*/
            if ((raReqQNode_p->ttiRAReqReceived + MIN_DIFF_RA_RAR) > currentTickWithDelay)
            {
                break; 
            } 
            /*SPR 3854 End*/
            retValue = fillRBAndMCS(rachConfig_p,raReqQNode_p,freeULRBForRAR_p,
                    &numRBAllocated,&mcsIndex);
            if(MAC_FAILURE == retValue)
            {
                break;
            }

            if (tempRaRnti != raReqQNode_p->raRNTI)  
            {
                /*SPR 3429 */
                if((
                            (numRARCnt != *maxRAR_p) ||
                            (TRUE == rarPduTobeScheduled_p->BIFlag)) &&
                        (!(processRntiFlag))) 
                    /*SPR 3429 */
                {
                    fillScheduledRARNTIBitMap(raRNTI,scheduledRARNTIBitMap2_p,
                            scheduledRARNTIBitMap1_p); 

                    raResponsesInTTI_p->numRarPdu++;

                    rarPduTobeScheduled_p->preambleMap1 = 
                        duplicatePreambleMap1;

                    rarPduTobeScheduled_p->preambleMap2 = 
                        duplicatePreambleMap2;

                    LOG_MAC_MSG(RAR_PDU_PROC_ID,
                            LOGDEBUG,
                            MAC_RACH,
                            getCurrentTick(),
                            raResponsesInTTI_p->numRarPdu,
                            rarPduTobeScheduled_p->noOfRAResponses,
                            rarPduTobeScheduled_p->raRnti,
                            rarPduTobeScheduled_p->BIFlag,
                            LINE_NUMBER,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"CBR_RAR_PDU_PROC_ID");

                    LOG_MAC_MSG(RAR_RARNTI_MAP_ID,
                            LOGDEBUG,
                            MAC_RACH,
                            getCurrentTick(),
                            rarPduTobeScheduled_p->raRnti,
                            (*scheduledRARNTIBitMap1_p),
                            (*scheduledRARNTIBitMap2_p),
                            duplicatePreambleMap1,
                            duplicatePreambleMap2,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"CBR_RAR_RARNTI_MAP_ID");


                }
                processRntiFlag = 0x0;

                tempRaRnti = raReqQNode_p->raRNTI;

                numRARCnt = *maxRAR_p;
                setProcessRntiFlag(raReqQNode_p->raRNTI,scheduledRARNTIBitMap1_p,
                        scheduledRARNTIBitMap2_p,&processRntiFlag);

                if(1 == processRntiFlag)
                {

                    rarPduTobeScheduled_p = 
                        getRARPduInfoforRARNTI(raReqQNode_p->raRNTI,
                                raResponsesInTTI_p);

                    if(PNULL == rarPduTobeScheduled_p)
                    {
                        LOG_MAC_MSG(RA_RNTI_PROC_RAR_NOT_FOUND,
                                LOGWARNING,
                                MAC_RACH,
                                getCurrentTick(),
                                raResponsesInTTI_p->numRarPdu,
                                raReqQNode_p->raRNTI,
                                LINE_NUMBER,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"RA_RNTI_PROC_RAR_NOT_FOUND");

                        rarPduTobeScheduled_p = 
                            getNextRARPduTobeScheduled(raResponsesInTTI_p);
                    }

                }
                else
                {
                    rarPduTobeScheduled_p = 
                        getNextRARPduTobeScheduled(raResponsesInTTI_p);

                }

                duplicatePreambleMap1 = 
                    rarPduTobeScheduled_p->preambleMap1;

                duplicatePreambleMap2 = 
                    rarPduTobeScheduled_p->preambleMap2;
            }
            else
            {
                /*Check if the poped node is duplicate node i.e same RARNTI 
                 *and same preamble
                 */
                checkPreambleBit(raReqQNode_p,duplicatePreambleMap1,
                        duplicatePreambleMap2,&isContinueNeeded,
                        /*CA Changes start  */
                        internalCellIndex);
                /*CA Changes end  */
                if(isContinueNeeded == TRUE)
                { 
                    continue;
                }
            }

            /*Check are we able to schedule the message with in RAR window*/
            /*TTI in which RAR message should be sent from PHY*/
            dlTxTTI = currentGlobalTTI;

            /*TTI in which RAR window will expire minus pdcch delay and 
             *phy dl delay
             */
            /* + SPR_17858_649_CHANGES */
            rarWindowEndTTI = (raReqQNode_p->ttiRAReqReceived + 
                    RAR_WINDOW_START_DELAY + raRspWindowSize) - 
                (dlDelay + PHY_DL_DELAY) -1;
            /* - SPR_17858_649_CHANGES */

            if (dlTxTTI > rarWindowEndTTI)
            {
                /*SPR 3854 Start*/
                /*CA Changes start  */
                raReqQNode_p = (RAReqQueueNode *)
                    popNode(raReqQueue_gp[internalCellIndex]);
                /*CA Changes end  */
                /*SPR 3854 End*/
                if (raReqQNode_p)
                {
                    LOG_MAC_MSG(RA_REQ_FREE_ID,
                            LOGDEBUG,
                            MAC_RACH,
                            getCurrentTick(),
                            raReqQNode_p->raRNTI,
                            raReqQNode_p->ttiRAReqReceived,
                            raReqQNode_p->raPreambleInfo.preamble,
                            LINE_NUMBER,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"CBR_RAREQ_TTI_EXPIRY");

                    freeMemPool(raReqQNode_p);

                    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                    lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CBRA++;
#endif
                    /* - PERF_STATS_PH1D1 */
                }

                continue;
            }
            else 
            {
                /*Allocate the free temporary CRNTT*/
                /*CA Changes start  */
                tcRNTI = getFreeTempRNTI(internalCellIndex);
                /*CA Changes end  */

                if (INVALID_RNTI != tcRNTI)
                {
                        /*SPR 3854 Start*/
                        raReqQNode_p = 
                            (RAReqQueueNode *)
                            /*CA Changes start  */
                            popNode(raReqQueue_gp[internalCellIndex]);
                        /*CA Changes end  */
                        /*SPR 3854 End*/

                        /* SPR#706 Changes Start */
                        if (PNULL == raReqQNode_p)
                        { 
                            /*SPR_7421_FIX_START */
                            if (MAC_FAILURE == 
                                    /*CA Changes start  */
                                    freeRNTIBeforeUETempContextCreation(tcRNTI, internalCellIndex))
                                /*CA Changes end  */
                            {
                                LOG_MAC_MSG(MAC_TCRNTI_FAIL_ID,
                                        LOGWARNING,
                                        MAC_RACH,
                                        getCurrentTick(),
                                        tcRNTI,
                                        LINE_NUMBER,
                                        DEFAULT_INT_VALUE,
                                        DEFAULT_INT_VALUE,
                                        DEFAULT_INT_VALUE,
                                        DEFAULT_FLOAT_VALUE,
                                        DEFAULT_FLOAT_VALUE,
                                        FUNCTION_NAME,
                                        "RAR_FREE_TCRNTI_FAIL");

                            }
                            /*SPR_7421_FIX_END */
                            continue;
                        }
                        /* SPR#706 Changes End */

                        noOfRAResponses_p = 
                            &rarPduTobeScheduled_p->noOfRAResponses;

                        rarPduTobeScheduled_p->raRnti = raReqQNode_p->raRNTI;

                        rarPduTobeScheduled_p->exptti = rarWindowEndTTI;

                        rarPduTobeScheduled_p->tti = raReqQNode_p->ttiRAReqReceived;

                        raReqInPDU_p =  &(rarPduTobeScheduled_p->
                                raReqTobeScheduled[*noOfRAResponses_p]);

                        raReqInPDU_p->raReqQNode_p = raReqQNode_p;

                        raReqInPDU_p->tcRNTI = tcRNTI;

                        raRNTI = raReqQNode_p->raRNTI;

                        (*freeULRBForRAR_p) -= numRBAllocated;

                        raReqInPDU_p->numRBAllocated = numRBAllocated;

                        raReqInPDU_p->mcsIndex = mcsIndex;

                        (*maxRAR_p)--;

                        (*noOfRAResponses_p)++;


                        SET_BIT_FOR_PREAMBLE(raReqQNode_p, 
                                duplicatePreambleMap1, 
                                duplicatePreambleMap2);

                        LOG_MAC_MSG(RA_REQ_PROC_ID,
                                LOGBRIEF,
                                MAC_RACH,
                                getCurrentTick(),
                                raReqQNode_p->raRNTI,
                                raReqQNode_p->ttiRAReqReceived,
                                raReqQNode_p->raPreambleInfo.preamble,
                                (*maxRAR_p),
                                (*freeULRBForRAR_p),
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"CBR_RA_REQ_PROC_ID");

                        LOG_MAC_MSG(CBR_RA_REQ_PREAMBLE_MAP_ID,
                                LOGDEBUG,
                                MAC_RACH,
                                getCurrentTick(),
                                tcRNTI,
                                raReqQNode_p->ttiRAReqReceived,
                                raReqQNode_p->raPreambleInfo.preamble,
                                duplicatePreambleMap1,
                                duplicatePreambleMap2,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"CBR_RA_REQ_PREAMBLE_MAP");

                }
                else
                {
                    /*<!-  MODIFIED POSSIBLE LOSS -!>*/
                    /* SPR 3207 START*/
                    /*SPR 3854 Start*/
                    raReqQNode_p =
                        (RAReqQueueNode *)
                        /*CA Changes start  */
                        popNode(raReqQueue_gp[internalCellIndex]);
                    /*CA Changes end  */
                    /*SPR 3854 End*/
                    /*SPR 4670 Start*/
                    if (raReqQNode_p)
                    {
                        /*SPR 4670 End*/
                        maxRNTIReachedFlag = TRUE;
                        /* SPR 3426 */
                        (*maxRAR_p)--;
                        /* SPR 2903 Fix End */
                        /* SPR 3426 */

                        /*SPR 3429 */
                        /*Set BI indicator*/
                        rarPduTobeScheduled_p->BIFlag = TRUE;

                        rarPduTobeScheduled_p->raRnti = raReqQNode_p->raRNTI;

                        rarPduTobeScheduled_p->exptti = rarWindowEndTTI;
                        /* SPR 3429 */
                        /*SPR 4670 Start --> moved freeMemPool below*/
                        /* If there are no RNTI available then need to pop the node to avoid an infinite loop*/
                        freeMemPool(raReqQNode_p);
                        raReqQNode_p = PNULL;
                        /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                        lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CBRA++;
#endif
                        /* - PERF_STATS_PH1D1 */

                        /* SPR 3207 END*/
                        /*<!-  MODIFIED POSSIBLE LOSS -!>*/
                        /*SPR 4670 End*/


                        LOG_MAC_MSG(MAC_TCRNTI_FAIL_ID,
                                LOGWARNING,
                                MAC_RACH,
                                getCurrentTick(),
                                tcRNTI,
                                LINE_NUMBER,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,
                                "RAR_TCRNTI_ALLOC_FAIL");
                        /*SPR 4670 Start*/
                    }
                    /*SPR 4670 End*/
                }
                retValue = checkMaxRNTIReachedFlag(maxRNTIReachedFlag,
                        tempRaRnti
#ifdef PERF_STATS
                        ,lteCellMacULPerfStats  
#endif
                        /*CA Changes start  */
                        ,internalCellIndex
                        /*CA Changes end  */
                        );
                if(MAC_SUCCESS == retValue)
                {
                    break;
                }

            }
        }
        else
        {
            break;
        }
    }
    /*SPR 3429 */
    if(
            ((numRARCnt != *maxRAR_p) ||
             (TRUE == rarPduTobeScheduled_p->BIFlag)) &&
            (!processRntiFlag) 
      )
        /*SPR 3429 */
    {
        fillScheduledRARNTIBitMap(raRNTI,scheduledRARNTIBitMap2_p,
                scheduledRARNTIBitMap1_p); 

        raResponsesInTTI_p->numRarPdu++;

        rarPduTobeScheduled_p->preambleMap1 = 
            duplicatePreambleMap1;

        rarPduTobeScheduled_p->preambleMap2 = 
            duplicatePreambleMap2;

        LOG_MAC_MSG(RAR_PDU_PROC_ID,
                LOGDEBUG,
                MAC_RACH,
                getCurrentTick(),
                raResponsesInTTI_p->numRarPdu,
                rarPduTobeScheduled_p->noOfRAResponses,
                rarPduTobeScheduled_p->raRnti,
                rarPduTobeScheduled_p->BIFlag,
                LINE_NUMBER,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"CBR_RAR_PDU_PROC_ID");

        LOG_MAC_MSG(RAR_RARNTI_MAP_ID,
                LOGDEBUG,
                MAC_RACH,
                getCurrentTick(),
                rarPduTobeScheduled_p->raRnti,
                (*scheduledRARNTIBitMap1_p),
                (*scheduledRARNTIBitMap2_p),
                duplicatePreambleMap1,
                duplicatePreambleMap2,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"CBR_RAR_RARNTI_MAP_ID");
    }
}

/*****************************************************************************
 * Function Name  : processdemuxRachManagerInterfaceQueue
 * Inputs         : rachDemuxQueueCount 
 *                  internalCellIndex - Cell-Index at MAC                                            
 * Outputs        : None
 * Returns        : None
 * Description    : This will pop the node from RACH DEMUX queue and 
 *                  based on the msg type,call  handleCRNTIMACCEReceived 
 *                  if msg3 is received with 
 *                  CRNTI ,handleCCCHSDUReceived if CCCH msg is received.
 *****************************************************************************/

/*CA Changes start  */
void processdemuxRachManagerInterfaceQueue(UInt32 rachDemuxQueueCount, InternalCellIndex internalCellIndex,
        ExecutionLegFunctionArg* execArgs)
/*CA Changes end  */
{

    /* Variable to contol the processing of MSG3 such that
     * only maximum 4 MSG3 
     * can be processed in current tick */
    UInt32 msg3PerTTICount=0;
    TempCRNTICtx *tempUEContext_p = PNULL;
    RachDemuxQueueNode *rachDemuxQueueNode_p = PNULL;
    UInt8 maxMsg3PerTTI = MAX_MSG3_PER_TTI;

    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
    /* + PERF_CA */
    LteCellMacULPerfStats *lteCellMacULPerfStats = &(gMacCellPerfStats_p[internalCellIndex]-> \
            /* - PERF_CA */
            lteCellMacULPerfStats);
#endif
    /* - PERF_STATS_PH1D1 */

    while(rachDemuxQueueCount > 0 && msg3PerTTICount < maxMsg3PerTTI)
    {
        /*CA Changes start  */
        DEQUEUE_DEMUX_RACH_MANAGER_INTERFACE_Q(RachDemuxQueueNode, (void *)&rachDemuxQueueNode_p, internalCellIndex);
        /*CA Changes end  */

        if(PNULL == rachDemuxQueueNode_p)
        {
            return ;
        }

        switch(rachDemuxQueueNode_p->msg)
        {
            case MSG3WITHCRNTI:
            {
                RNTIInfo  *rntiInfo_p = PNULL;
                DLUEContext *ueDLContext_p  = PNULL;
                /*CA Changes start  */
                rntiInfo_p = getUeIdxFromRNTIMap(rachDemuxQueueNode_p->crnti, internalCellIndex);
                /*CA Changes end  */
                /*SPR_7421_5757_FIX_START*/
                if((rntiInfo_p != PNULL) && (rntiInfo_p->rntiFlag!=FREERNTI)
                        && (PNULL != dlUECtxInfoArr_g[rntiInfo_p->index].dlUEContext_p )) //INCORPORATES COVERTY+SPR5757
                {
                    ueDLContext_p =
                        dlUECtxInfoArr_g[rntiInfo_p->index].dlUEContext_p;

                    /*SPR_7421_5757_FIX_END*/
                    /* Function to extract temporary UE context from tcrnti */
                    {
                    tempUEContext_p = 
                        getTempCRNTIContextPointer(
                                rachDemuxQueueNode_p->tcrnti, internalCellIndex);
                    /* COVERITY 10769 */
                    if(tempUEContext_p)
                    {
                        /* COVERITY 10769 */
                        /* SPR 15682 fix start */
                        /* Refer Spec 36.213 Section 4.2.3 for the calculations
                         * NTA = TA ×16 , where TA = 0, 1, 2, ..., 1282*/
                        ueDLContext_p->timingAdvanceAbsValue = (tempUEContext_p->timingAdvanceAbs << 4); 
                        /* SPR 15682 fix end */

                        tempUEContext_p->msg3BuffInfo.crnti = rachDemuxQueueNode_p->crnti;

                        /* Message Three is recieved with CRNTI */

                        handleCRNTIMACCEReceived(tempUEContext_p, 
                                rachDemuxQueueNode_p->tcrnti, 
                                rachDemuxQueueNode_p->dlAdvance,
                                rachDemuxQueueNode_p->receivedTTI,
                                internalCellIndex,
                                execArgs);
                        /*SPR_7421_5757_FIX_START*/
                    }
                }
                }
                /*SPR_7421_5757_FIX_END*/
                /* Free the temporary context for the new temporaray crnti 
                 * The temporaray context is not needed because 
                 * UE has already permanent context 
                 * corresponding to the crnti 
                 * received in Message3*/

                /*CA Changes start  */
                deleteTempCRNTICtx(rachDemuxQueueNode_p->tcrnti, internalCellIndex);
                /*CA Changes end  */

                /* Free the rnti */
                /*CA Changes start  */
                if(MAC_FAILURE == freeRNTI(rachDemuxQueueNode_p->tcrnti, internalCellIndex))
                    /*CA Changes end  */
                {
                    LOG_MAC_MSG(MAC_TCRNTI_FAIL_ID,
                            LOGWARNING,
                            MAC_RACH,
                            getCurrentTick(),
                            rachDemuxQueueNode_p->tcrnti,
                            LINE_NUMBER,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            __func__,"RAR_FREE_TCRNTI_FAIL");

                }

                LOG_MAC_MSG(MSG3_CRNTI_TEMPCTXT_DEL_ID,
                        LOGWARNING,
                        MAC_RACH,
                        getCurrentTick(),
                        rachDemuxQueueNode_p->tcrnti,
                        LINE_NUMBER,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        __func__,"MSG3_CRNTI_TEMPCTXT_DEL_ID");
            }
            break;

            case MSG3WITHCCCH:
            {
                {
                tempUEContext_p = getTempCRNTIContextPointer(
                        rachDemuxQueueNode_p->tcrnti, internalCellIndex);
                /* +Coverity 10769 */
                if(tempUEContext_p)
                {
                    /* -Coverity 10769 */
                    tempUEContext_p->msg3BuffInfo.msg_p = rachDemuxQueueNode_p->receivedCCCH_Msg; 
                    /*CCCH garbage len fix */
                    tempUEContext_p->msg3BuffInfo.msgLen = rachDemuxQueueNode_p->length;
                    /*CCCH garbage len fix */

                    /* Message Three is recieved with CCCH */
                    handleCCCHSDUReceived(tempUEContext_p, 
                            rachDemuxQueueNode_p->tcrnti, 
                            rachDemuxQueueNode_p->dlAdvance, 
                            rachDemuxQueueNode_p->receivedTTI,
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */
                    /* +Coverity 10769 */
                }
                }
                /* -Coverity 10769 */
            }
            break;

            /* After the receipt of First Uplink data,
             * we need to stop the timer, send a trigger that sync is achieved
             * and free the preamble which was allocated */
            case FIRST_UPLINK_DATA_AFTER_CFR:
            {
                UInt32 n=0;
                DLUEContext *ueDLContext_p  = PNULL;
                ULUEContext *ulUEContext_p  = PNULL;
                RNTIInfo  *rntiInfo_p = PNULL;
                UInt8 preambleIndex;
                ContentionFreePreamble *contFreePreamble_p = PNULL;

                /*CA Changes start  */
                rntiInfo_p = getUeIdxFromRNTIMap(rachDemuxQueueNode_p->crnti, internalCellIndex);
                /*CA Changes end  */

                /* + COVERITY 5.0 */
                if(rntiInfo_p == PNULL)
                {
                    break;
                }
                /* - COVERITY 5.0 */
                /*+ SPR 8564*/
                if((TRUE == dlUECtxInfoArr_g[rntiInfo_p->index].pendingDeleteFlag) ||
                        (TRUE == ulUECtxInfoArr_g[rntiInfo_p->index].pendingDeleteFlag) ||
                        TRUE == ulUECtxInfoArr_g[rntiInfo_p->index].ulUEContext_p->waitForMsg3BeforeResetting)
                    /*- SPR 8564*/
                {
                    LOG_UT(PENDING_DELETE_UE,
                            LOGERROR,
                            MAC_RACH,
                            getCurrentTick(),
                            rntiInfo_p->index,
                            rachDemuxQueueNode_p->crnti,
                            dlUECtxInfoArr_g[rntiInfo_p->index].pendingDeleteFlag,
                            LINE_NUMBER,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"Pending To Del UE");
                    break;
                }

                ueDLContext_p =
                    dlUECtxInfoArr_g[rntiInfo_p->index].dlUEContext_p;
                ulUEContext_p =
                    ulUECtxInfoArr_g[rntiInfo_p->index].ulUEContext_p;

                preambleIndex = ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx;

                if((INVALID_PREAMBLE_IDX == preambleIndex) ||
                        /*CA Changes start  */
                        (PREAMBLE_BUSY != getPreambleStatusInfo(preambleIndex, internalCellIndex)))
                    /*CA Changes end  */
                {
                    LOG_UT(PREAMBLE_NOT_BUSY_ID,
                            LOGERROR,
                            MAC_RACH,
                            getCurrentTick(),
                            rntiInfo_p->index,
                            rachDemuxQueueNode_p->crnti,
                            preambleIndex,
                            LINE_NUMBER,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"PREAMBLE_NOT_BUSY_ID");
                    break;
                }

                /*CA Changes start  */
                contFreePreamble_p = getContentionFreePreambleInfo(preambleIndex, internalCellIndex);
                /*CA Changes end  */


                if(PDCCH_ORDER == 
                        contFreePreamble_p->pdcchOrderHandOverFlag)
                {
                    /* Now we stop the timer after the receipt of First uplink data */
                    stopConFreePreambleTimer(
                            ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderTimerNode,
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */
                    ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderTimerNode = PNULL;

                    /* CA_pahse2_ulSync_code Start */
                    if (IS_PCELL_OF_UE(ulUEContext_p , internalCellIndex))
                    {
                        /* CA_pahse2_ulSync_code End */

                        /* Trigger to strategy that sync is achieved */
                        /*CA Changes start  */
                        putEntryInUlSyncLossOnOffTriggerQ(
                                ulUEContext_p->ueIndex, UL_SYNC_LOSS_OFF, internalCellIndex);
                        /*CA Changes end  */

                        ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx = INVALID_PREAMBLE_IDX;
                        ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState = UL_SYNC_LOSS_OFF;
                        ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderSentCount = 0;
                        ueDLContext_p->lastTASentTick = rachDemuxQueueNode_p->receivedTTI - UE_RA_ADV_TTI;

                        /*SPR_3061_3068_FIX*/
                        ueDLContext_p->lastSyncDetectedTick = execArgs->globalTick;
                        /*SPR_3061_3068_FIX*/

                        LOG_MAC_MSG(MAC_UL_SYNC_ON,
                                LOGINFO,
                                MAC_PDCCH,
                                getCurrentTick(),
                                ueDLContext_p->lastSyncDetectedTick,
                                ueDLContext_p->lastTASentTick,
                                ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState,
                                ulUEContext_p->ueIndex,
                                LINE_NUMBER,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"CFR RAR UL_SYNC_ON UE");
                        /*SPR_3065_FIX*/ 
                        sendSyncIndToRRC(ulUEContext_p->ueIndex,
                                rachDemuxQueueNode_p->crnti, IN_SYNC_AFTER_OUT_OF_SYNC,
                                /*CA Changes start  */
                                internalCellIndex);
                        /*CA Changes end  */
                        /*SPR_3065_FIX*/

                        /* CA_pahse2_ulSync_code Start */
                    }
                    /* CA_pahse2_ulSync_code End*/

                    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                    lteCellMacULPerfStats->totalRcvdMsg3RandomAccessPreamblesCFRA++;
#endif
                    /* - PERF_STATS_PH1D1 */
                }
                /*SPR_3065_FIX*/
                else if ((CONNECTION_HANDOVER ==
                            contFreePreamble_p->pdcchOrderHandOverFlag))
                {
                    sendSyncIndToRRC(ulUEContext_p->ueIndex,
                            rachDemuxQueueNode_p->crnti, IN_SYNC,
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */

                    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                    lteCellMacULPerfStats->totalRcvdMsg3RandomAccessPreamblesCFRA++;
#endif
                    /* - PERF_STATS_PH1D1 */
                }
                /*SPR_3065_FIX*/
                ueDLContext_p->ulSyncLossOrHOInfoInDL.preambleStatus 
                    = PREAMBLE_INVALID; 
                /* SPR 16125 Fix Start */
                ueDLContext_p->ulSyncLossOrHOInfoInDL.preambleIdx = 
                    INVALID_PREAMBLE_IDX;
                ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx = 
                    INVALID_PREAMBLE_IDX;
                /* SPR 16125 Fix End */

                ulUEContext_p->currPowerCorrectionForPusch = 0;
                ulUEContext_p->prevPowerCorrectionForPusch = 0;

                ulUEContext_p->powerControlPuschTPCValue = 0;

                /*CA Changes start  */
                ulUEContext_p->pathLoss = (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])-> \
                                          alphaBasedPathLossToTargetSINRMap.defaultPathLoss;
                /*CA Changes end  */
                /* ICIC changes start */
                /*CA Changes start  */
                updateULPathLoss(ulUEContext_p->ueIndex,ulUEContext_p->pathLoss, internalCellIndex);
                /*CA Changes end  */
                /* ICIC changes ends */
                for(n = 0; n < MAX_SUB_FRAME; n++)
                {
                    ulUEContext_p->powerControlDeltaTFAndLogFactor[n] = 0;
                }
                ulUEContext_p->mcsIndex = ulUEContext_p->initialMcs;
                /* Cyclomatic_complexity_changes_start */
                getUlModSchemeFromMcsIndex(ulUEContext_p->mcsIndex,\
                        /*CA Changes start  */
                        &(ulUEContext_p->modulationScheme),\
                        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->macEnable64QAM,\
                        /*CA Changes end  */
                        ulUEContext_p->ueCategory);
                /* Cyclomatic_complexity_changes_end */
                /*CA Changes start  */
                initUEULCSIContext(ulUEContext_p,ulUEContext_p->initialMcs, internalCellIndex);
                    /* +- SPR 17777 */
                    resetTpcPdcchConfigInfoForPusch (ulUEContext_p, internalCellIndex);
                    /* +- SPR 17777 */
                /*CA Changes end  */

                ueDLContext_p->currPowerCorrectionForPucch = 0;
                ueDLContext_p->powerControlPucchTpcValue = 0;
                ueDLContext_p->averageSinrForPucch = \
                                                     /*CA Changes start  */
                                                     (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])-> \
                                                     deltaSinrToTPCMapForPUCCH.pucchSINRTarget;
                /*CA Changes end  */
                ueDLContext_p->lastSINRCoolOffCounter = 0;
                /*CA Changes start  */
                initPucchBlerContext(ueDLContext_p, internalCellIndex);
                /*CA Changes end  */

                    /* +- SPR 17777 */
                    resetTpcPdcchConfigInfoForPucch (ueDLContext_p, internalCellIndex);
                    /* +- SPR 17777 */

                /* + CQI_5.0 */
                resetCQIInfo(ueDLContext_p,
                        &ueDLContext_p->dlCQIInfo,
                        internalCellIndex);
                /* + CQI_5.0 */

                LOG_MAC_MSG(MAC_UL_SYNC_ON,
                        LOGINFO,
                        MAC_PDCCH,
                        getCurrentTick(),
                        ueDLContext_p->lastSyncDetectedTick,
                        ueDLContext_p->lastTASentTick,
                        ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState,
                        ulUEContext_p->ueIndex,
                        LINE_NUMBER,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"First Uplink data");


                /*CA Changes start  */
                freePreambleIndex(preambleIndex, internalCellIndex);
                /*CA Changes end  */

            }
            break;

            default:

            break;
        }

        rachDemuxQueueCount--;
        msg3PerTTICount++;

        FREE_MEM_NODE_DEMUX_RACH_MANAGER_INTERFACE_Q(rachDemuxQueueNode_p);

        tempUEContext_p = PNULL;
    }

}

/*****************************************************************************
 * Function Name  : processRachDLStrategyQueue
 * Inputs         : currentGlobalTTI - current global Tick
 *                  internalCellIndex - Cell-Index at MAC                                            
 * Outputs        : None
 * Returns        : None
 * Description    : This will pop the node from RACH DL-SCH queue, allocate the preamble
 *                  and put the entry in pdcch order queue
 *****************************************************************************/
/*CA Changes start  */
/* SPR 15909 fix start */
void processRachDLStrategyQueue(tickType_t currentGlobalTTI, InternalCellIndex internalCellIndex)
    /* SPR 15909 fix end */
    /*CA Changes end  */
{

    RachDLStrategyQueueNode *rachDLStrategyQueueNode_p = PNULL;
    /* Tick in which sync loss is detected by DL strategy */
    /* SPR 15909 fix start */
    tickType_t syncLossDetectedTTI;
    /* SPR 15909 fix end */
    ULUEContext *ulUEContext_p  = PNULL;
    DLUEContext *ueDLContext_p  = PNULL;
    ConFreeRACHResourcesInfo rachResourcesInfo = {0};

    /* Function to 
     * get the first node from rachDLStrategyQueue*/
    /*CA Changes start  */
    GET_NODE_RACH_DL_STRATEGY_INTERFACE_Q(RachDLStrategyQueueNode,
            (void *)&rachDLStrategyQueueNode_p, internalCellIndex);
    /*CA Changes end  */

    syncLossDetectedTTI = rachDLStrategyQueueNode_p->syncLossDetectedTTI;
    while(1)
    {
        /*CA Changes start  */
        GET_NODE_RACH_DL_STRATEGY_INTERFACE_Q(RachDLStrategyQueueNode,
                (void *)&rachDLStrategyQueueNode_p, internalCellIndex);
        /*CA Changes end  */
        /* Only Process nodes from the queue whose TTI is same as 
         * syncloss detected TTI for the first node */ 
        if(rachDLStrategyQueueNode_p != PNULL && 
                rachDLStrategyQueueNode_p->syncLossDetectedTTI == syncLossDetectedTTI)
        {
            /* Function to pop the node 
             * from rachDLStrategyQueue*/
            /*CA Changes start  */
            DEQUEUE_RACH_DL_STRATEGY_INTERFACE_Q(RachDLStrategyQueueNode, 
                    (void *)&rachDLStrategyQueueNode_p, internalCellIndex);
            /*CA Changes end  */

            if((TRUE == dlUECtxInfoArr_g[rachDLStrategyQueueNode_p->ueIndex].pendingDeleteFlag) ||
                    (TRUE == ulUECtxInfoArr_g[rachDLStrategyQueueNode_p->ueIndex].pendingDeleteFlag))
            {
                FREE_MEM_NODE_RACH_DL_STRATEGY_INTERFACE_Q(rachDLStrategyQueueNode_p);
                continue;
            }

            ueDLContext_p =
                dlUECtxInfoArr_g[rachDLStrategyQueueNode_p->ueIndex].dlUEContext_p;
            ulUEContext_p =
                ulUECtxInfoArr_g[rachDLStrategyQueueNode_p->ueIndex].ulUEContext_p;

            if(MAC_SUCCESS != allocateDedicatedPreamble(
                        rachDLStrategyQueueNode_p->ueIndex,
                        PDCCH_ORDER, 
                        &rachResourcesInfo,
                        /*CA Changes start  */
                        internalCellIndex))
                /*CA Changes end  */
            {

                LOG_MAC_MSG(MAC_PDCCH_ORDER_TRIGGERED,
                        LOGWARNING,
                        MAC_PDCCH,
                        getCurrentTick(),
                        ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        ulUEContext_p->ueIndex,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,
                        "PreambleIdxAlloc failed");
            }
            else
            {
                ueDLContext_p->ulSyncLossOrHOInfoInDL.preambleStatus = 
                    PREAMBLE_BUSY;

                LOG_MAC_MSG(MAC_PDCCH_ORDER_TRIGGERED,
                        LOGWARNING,
                        MAC_PDCCH,
                        getCurrentTick(),
                        ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        ulUEContext_p->ueIndex,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,
                        "PreambleIdxAlloc success");

            }

            ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx = 
                rachResourcesInfo.preambleIndex;
            /* Put an entry in PDCCH Order Queue to send a PDCCH Order to UL Out of sync UE */
            putEntryInPdcchOrderQ(rachResourcesInfo.preambleIndex, 
                    rachDLStrategyQueueNode_p->ueIndex, 
                    /*CA Changes start  */
                    currentGlobalTTI, internalCellIndex);
            /*CA Changes end  */

            FREE_MEM_NODE_RACH_DL_STRATEGY_INTERFACE_Q(rachDLStrategyQueueNode_p);

        }

        else
        {
            break;
        }
    }
}

/*****************************************************************************
 * Function Name  : processPdcchOrderTimerTriggerQueue
 * Inputs         : internalCellIndex - Cell-Index at MAC                                            
 * Outputs        : None
 * Returns        : None
 * Description    : This will pop the node from PDCCH order timer trigger queue, 
 *                  start the corresponding timer based on the preamble
 *****************************************************************************/
/*CA Changes start  */
/* SPR 15909 fix start */
/* +- SPR 17777 */
void processPdcchOrderTimerTriggerQueue(InternalCellIndex internalCellIndex)
    /* +- SPR 17777 */
    /* SPR 15909 fix end */
    /*CA Changes end  */
{
    /* Tick in which Pdcch order is sent for the out of sync UE */ 
    /* SPR 15909 fix start */
    tickType_t pdcchOrderTriggerTTI;
    /* SPR 15909 fix end */
    UInt32 raRspWindowSize  = 0;
    /* Tick in which timer will expire for the pdcch order */
    /* SPR 15909 fix start */
    tickType_t pdcchOrderExpiryTTI;
    /* SPR 15909 fix end */
    /*CA Changes start  */
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /*CA Changes end  */

    ULUEContext *ulUEContext_p  = PNULL;
    PdcchOrderTimerTriggerQueueNode *pdcchOrderTimerTriggerQueueNode_p = PNULL;
    /*CA Changes start  */
    GET_NODE_PDCCH_ORDER_TIMER_TRIGGER_Q(PdcchOrderTimerTriggerQueueNode, 
            (void *)&pdcchOrderTimerTriggerQueueNode_p, internalCellIndex);
    /*CA Changes end  */
    /* SPR 10927 fix start */
    if( PNULL == pdcchOrderTimerTriggerQueueNode_p)
    {
        return;
    }  
    /* SPR 10927 fix end */

    pdcchOrderTriggerTTI = pdcchOrderTimerTriggerQueueNode_p->pdcchOrderTriggerTTI;

    /*CA Changes start  */
    raRspWindowSize = 
        cellParams_p->rachConfigInfo.rachConfiguration.
        raRspWindowSize[cellParams_p->rachConfigInfo.rachConfiguration.activeIdxRARspWindow];
    /*CA Changes end  */


    while(1)
    {
        /*CA Changes start  */
        GET_NODE_PDCCH_ORDER_TIMER_TRIGGER_Q(PdcchOrderTimerTriggerQueueNode, 
                (void *)&pdcchOrderTimerTriggerQueueNode_p, internalCellIndex); 
        /*CA Changes end  */
        /* Only process the nodes for which pdcch order is sent 
         * in the same tick */
        if(pdcchOrderTimerTriggerQueueNode_p != PNULL && 
                pdcchOrderTimerTriggerQueueNode_p->pdcchOrderTriggerTTI == 
                pdcchOrderTriggerTTI)
        {
            /*CA Changes start  */
            DEQUEUE_PDCCH_ORDER_TIMER_TRIGGER_Q(PdcchOrderTimerTriggerQueueNode,
                    (void *)&pdcchOrderTimerTriggerQueueNode_p, internalCellIndex);
            /*CA Changes end  */

            ulUEContext_p =
                ulUECtxInfoArr_g[pdcchOrderTimerTriggerQueueNode_p->ueIndex].ulUEContext_p;
            /* Based on the preamble index, we will decide whether to start the
             * contention based pdcch order timer or contention free */
            if(pdcchOrderTimerTriggerQueueNode_p->preambleIndex == INVALID_PREAMBLE_IDX)
            {
                pdcchOrderExpiryTTI = 
                    (pdcchOrderTimerTriggerQueueNode_p->pdcchOrderTriggerTTI
                     /*CA Changes start  */
                     + rachGlobals_gp[internalCellIndex]->conBasedPDCCHOrderTimerValue
                     /*CA Changes end  */
                     + raRspWindowSize
                     + (18 * (ulUEContext_p->maxUlHARQTx -1))
                     + PHY_UL_DATA_DELAY);
                ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderTimerNode =  
                    startConBasedPdcchOrderTimer(
                            pdcchOrderExpiryTTI,
                            pdcchOrderTimerTriggerQueueNode_p->ueIndex,
                            /*CA Changes start  */
                            internalCellIndex);
                /*CA Changes end  */
                /* Increment the pdcch order sent count in the UL context */
                ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderSentCount++;

                LOG_MAC_MSG(MAC_PDCCH_ORDER_TRIGGERED,
                        LOGWARNING,
                        MAC_PDCCH,
                        getCurrentTick(),
                        ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx,
                        ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderSentCount,
                        DEFAULT_INT_VALUE,
                        pdcchOrderExpiryTTI,
                        ulUEContext_p->ueIndex,
                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"CON_BASED_PDCCH_ORDER_TRIGGERED");

            }
            else
            {
                /* TTI in which contention free pdcch order timer will expire */
#ifdef FDD_CONFIG
                pdcchOrderExpiryTTI =
                    (pdcchOrderTimerTriggerQueueNode_p->pdcchOrderTriggerTTI 
                     /*CA Changes start  */
                     + cellParams_p->contnFreeRachTimer 
                     /*CA Changes end  */
                     + raRspWindowSize
                     + ((FDD_HARQ_OFFSET + FDD_DATA_OFFSET) * (ulUEContext_p->maxUlHARQTx -1))
                     + PHY_UL_DATA_DELAY);
#elif TDD_CONFIG
                pdcchOrderExpiryTTI = (pdcchOrderTimerTriggerQueueNode_p->pdcchOrderTriggerTTI
                        /*CA Changes start  */
                        + cellParams_p->contnFreeRachTimer
                        /*CA Changes end  */
                        + raRspWindowSize
                        + (18 * (ulUEContext_p->maxUlHARQTx -1))
                        + PHY_UL_DATA_DELAY);
#endif

                ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderTimerNode 
                    = startConFreePreambleTimer(
                            pdcchOrderExpiryTTI,
                            pdcchOrderTimerTriggerQueueNode_p->preambleIndex,
                            /*CA Changes start  */
                            internalCellIndex);
                /*CA Changes end  */

                LOG_MAC_MSG(MAC_PDCCH_ORDER_TRIGGERED,
                        LOGWARNING,
                        MAC_PDCCH,
                        getCurrentTick(),
                        ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx,
                        ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderSentCount,
                        DEFAULT_INT_VALUE,
                        pdcchOrderExpiryTTI,
                        ulUEContext_p->ueIndex,
                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"CON_FREE_PDCCH_ORDER_TRIGGERED");


                ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderSentCount++;
            }

            FREE_MEM_NODE_PDCCH_ORDER_TIMER_TRIGGER_Q(pdcchOrderTimerTriggerQueueNode_p);

        }
        else
        {
            break;
        }

    }
}
/*****************************************************************************
 * Function Name  : processPdcchRachInterfaceQueue
 * Inputs         : internalCellIndex - Cell-Index at MAC                                            
 * Outputs        : None
 * Returns        : None
 * Description    : This will pop the node from PDCCH rach interface queue and
 *                  delete the temporary context and free the RNTI in the case of 
 *                  RRC connection re-establishment and failure
 *****************************************************************************/
/*CA Changes start  */
/* SPR 15909 fix start */
/* +- SPR 17777 */
void processPdcchRachInterfaceQueue(InternalCellIndex internalCellIndex)
/* +- SPR 17777 */
    /* SPR 15909 fix end */
    /*CA Changes end  */
{
    RachInterfaceQueueNode *rachInterfaceQueueNode_p = PNULL;
    UInt16 rnti;
    /* L2_FIX_start*/

    /*CA Changes start  */
    UInt32 queueCount = COUNT_PDCCH_RACH_INTERFACE_Q(internalCellIndex);
    /*CA Changes end  */
    while(queueCount--)
    {
        /* L2_FIX_end*/
        /*CA Changes start  */
        DEQUEUE_PDCCH_RACH_INTERFACE_Q(RachInterfaceQueueNode, 
                (void *)&rachInterfaceQueueNode_p, internalCellIndex);
        /*CA Changes end  */

        if(PNULL == rachInterfaceQueueNode_p)
        {
            return ;
        }
        /* SPR 5235 : Start*/
        if(PNULL == rachInterfaceQueueNode_p->rachInterface_p)
        {
            FREE_MEM_NODE_PDCCH_RACH_INTERFACE_Q(rachInterfaceQueueNode_p);
            return ;
        }
        /* SPR 5235 : End*/
        switch(rachInterfaceQueueNode_p->msgType)
        {
            /* Case when the RRC connection re-establishment or failure 
             * trigger has come from PDCCH */
            case FREE_TCRNTI: 
            {
                rnti = ((PDCCHRachNode *)(rachInterfaceQueueNode_p->rachInterface_p))->rnti;
                freeMemPool(rachInterfaceQueueNode_p->rachInterface_p);
                /*CA Changes start  */
                deleteTempCRNTICtx(rnti, internalCellIndex);
                if(MAC_FAILURE == freeRNTI(rnti, internalCellIndex))
                    /*CA Changes end  */
                {
                    LOG_MAC_MSG(MAC_TCRNTI_FAIL_ID,
                            LOGWARNING,
                            MAC_RACH,
                            getCurrentTick(),
                            rnti,
                            LINE_NUMBER,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            __func__,"RAR_FREE_TCRNTI_FAIL");
                }

                LOG_MAC_MSG(MSG3_CRNTI_TEMPCTXT_DEL_ID,
                        LOGWARNING,
                        MAC_RACH,
                        getCurrentTick(),
                        rnti,
                        LINE_NUMBER,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        __func__,"MSG3_CRNTI_TEMPCTXT_DEL_ID");
            }
            break;

            case CREATE_UE:
            {
                RNTIInfo *rntiInfo_p = PNULL;
                rnti = ((RRCRachInterfaceNode *)(rachInterfaceQueueNode_p->rachInterface_p))->rnti;
                /* +- SPR 18268 */
                UInt16 ueIndex = ((RRCRachInterfaceNode *)(rachInterfaceQueueNode_p->rachInterface_p))->ueIndex;
                /* +- SPR 18268 */
                freeMemPool(rachInterfaceQueueNode_p->rachInterface_p);
                /*CA Changes start  */
                rntiInfo_p = getUeIdxFromRNTIMap(rnti, internalCellIndex);
                /*CA Changes end  */

                /* +COVERITY 5.0 */
                if (rntiInfo_p)
                {

                    rntiInfo_p->rntiFlag = CRNTI;

                    macCrntiUeNum_g[internalCellIndex]++;

                    /*CA Changes start  */
                    deleteTempCRNTICtx(rnti, internalCellIndex);
                    /*CA Changes end  */
                    rntiInfo_p->index = ueIndex;
                }
                /* +COVERITY 5.0 */
            }
            break;
            /*SPR 16855 +-*/
            case HO_RACH_REQ:
            {
                UInt16 tcRNTI;
                /*coverity 55801*/
                HORachInfo hoRachInfo = {0};
                /*coverity 55801*/
                RrcOamHiPrioQNode *dlRrcOamQNode_p = PNULL;
                ConFreeRACHResourcesInfo rachResourceInfo = {0};
                UInt16 ueIndex = ((RRCRachHOInterfaceNode *)(rachInterfaceQueueNode_p->rachInterface_p))->ueIndex;
                freeMemPool(rachInterfaceQueueNode_p->rachInterface_p);
                /*CA Changes start  */
                allocateDedicatedPreamble(ueIndex, CONNECTION_HANDOVER, &rachResourceInfo,internalCellIndex);
                tcRNTI = getFreeTempRNTI(internalCellIndex);
                /*CA Changes end  */
                hoRachInfo.preambleIndex = rachResourceInfo.preambleIndex;
                hoRachInfo.prachMaskIndex = rachResourceInfo.prachMaskIndex;
                hoRachInfo.tcRNTI = tcRNTI;
                GET_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(dlRrcOamQNode_p,RrcOamHiPrioQNode);
                /* + Coverity 24509 */
                if( PNULL == dlRrcOamQNode_p )
                {
                    ltePanic("allocation fail for dlRrcOamQNode_p ");
                }
                else
                {
                    /* + coverity 55801 */
                    memSet(dlRrcOamQNode_p,0,sizeof(RrcOamHiPrioQNode));
                    /* - coverity 55801 */
                    /* - Coverity 24509 */
                    dlRrcOamQNode_p->rrcMsgId = MAC_HO_RACH_RESOURCE_REQ;
                    dlRrcOamQNode_p->ueIndex = ueIndex;
                    dlRrcOamQNode_p->respCode = MAC_SUCCESS;
                    dlRrcOamQNode_p->rrcOamReqResp.hoRachInfo = hoRachInfo;
                    dlRrcOamQNode_p->eventId = rachInterfaceQueueNode_p->eventId; 
                    /* CA changes Start */
                    /* + Coverity Id - 32480 */
                    if(!ENQUEUE_RRC_OAM_HPTOLP_THDH_PRIO_Q(&(rrcOamHPtoLPThdHPrioQ_g[internalCellIndex][UL_THD]),
                                RrcOamHiPrioQNode, dlRrcOamQNode_p))
                    {
                        freeMemPool(dlRrcOamQNode_p);

                    }
                    /* - Coverity Id - 32480 */
                    /* CA changes End */
                    /* Send to the LP thread to process it */
                    lteMessageQSend( recvMsgFrmHPThdFd_g, (UInt8 *)&internalCellIndex, sizeof(InternalCellIndex));
                }
            }
            break;
            /* SPR 8379 changes start */
            case INTRA_HO_RACH_REQ:
            {
                HORachInfo hoRachInfo;
                RrcOamHiPrioQNode *ulRrcOamQNode_p = PNULL;
                ConFreeRACHResourcesInfo rachResourceInfo = {0};
                UInt16 ueIndex = ((RRCRachHOInterfaceNode *)(rachInterfaceQueueNode_p->rachInterface_p))->ueIndex;
                freeMemPool(rachInterfaceQueueNode_p->rachInterface_p);
                hoRachInfo.resp = allocateDedicatedPreamble(ueIndex, 
                        CONNECTION_HANDOVER, &rachResourceInfo,
                        /*CA Changes start  */
                        internalCellIndex);
                /*CA Changes end  */
                /* + Coverity 55801 */
                /* Initialized tcRNTI with 0 as it is not used in this case */
                hoRachInfo.tcRNTI = 0;
                /* - Coverity 55801 */
                if( MAC_SUCCESS == hoRachInfo.resp )
                {
                    hoRachInfo.preambleIndex = rachResourceInfo.preambleIndex;
                    hoRachInfo.prachMaskIndex = rachResourceInfo.prachMaskIndex;
                }
                else
                {
                    hoRachInfo.preambleIndex = 0;
                    hoRachInfo.prachMaskIndex = 0;
                }
                GET_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(ulRrcOamQNode_p,RrcOamHiPrioQNode);
                if( PNULL == ulRrcOamQNode_p )
                {
                    ltePanic("allocation fail for ulRrcOamQNode_p ");
                }
                else
                {
                    /* + coverity 55801 */
                    memSet(ulRrcOamQNode_p,0,sizeof(RrcOamHiPrioQNode));
                    /* - coverity 55801 */
                    ulRrcOamQNode_p->rrcMsgId = MAC_HO_RACH_RESOURCE_REQ;
                    ulRrcOamQNode_p->ueIndex = ueIndex;
                    ulRrcOamQNode_p->respCode = MAC_SUCCESS;
                    ulRrcOamQNode_p->rrcOamReqResp.hoRachInfo = hoRachInfo;
                    ulRrcOamQNode_p->eventId = rachInterfaceQueueNode_p->eventId; 
                    /* CA changes Start */
                    /*Cov_fix_30April_25573_start*/
                    if(!ENQUEUE_RRC_OAM_HPTOLP_THDH_PRIO_Q(&(rrcOamHPtoLPThdHPrioQ_g[internalCellIndex][UL_THD]),
                                RrcOamHiPrioQNode, ulRrcOamQNode_p))
                        /* CA changes End */
                    {
                        FREE_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(ulRrcOamQNode_p);
                    }
                    /*Cov_fix_30April_25573_stop*/
                    /* Send to the LP thread to process it */
                    lteMessageQSend( recvMsgFrmHPThdFd_g, (UInt8 *)&internalCellIndex, sizeof(InternalCellIndex));
                }
            }
            break;
            /* SPR 8379 changes end */
            case HO_RACH_REL_IND:
            {
                /* SPR 16583 fix start */
                RrcOamHiPrioQNode *ulRrcOamQNode_p = PNULL;
                /* SPR 16583 fix end */
                UInt16 ueIndex = ((RRCRachInterfaceNode *)(rachInterfaceQueueNode_p->rachInterface_p))->ueIndex;
                UInt16 crnti = ((RRCRachInterfaceNode *)(rachInterfaceQueueNode_p->rachInterface_p))->rnti;
                freeMemPool(rachInterfaceQueueNode_p->rachInterface_p);
                /*CA Changes start  */
                handOverRelResources(ueIndex,crnti, internalCellIndex) ;
                /*CA Changes end  */
                /* SPR 16583 fix start */
                GET_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(ulRrcOamQNode_p,RrcOamHiPrioQNode);
                if( PNULL == ulRrcOamQNode_p )
                {
                    ltePanic("allocation fail for ulRrcOamQNode_p ");
                }
                else
                {
                    memSet(ulRrcOamQNode_p,0,sizeof(RrcOamHiPrioQNode));
                    ulRrcOamQNode_p->rrcMsgId = MAC_HO_REL_RACH_RESOURCE_IND;
                    ulRrcOamQNode_p->ueIndex = ueIndex;
                    ulRrcOamQNode_p->respCode = MAC_SUCCESS;
                    ulRrcOamQNode_p->eventId = rachInterfaceQueueNode_p->eventId; 
                    /*Coverity 25573 Fix Start*/
                    /*SPR 21944 Changes Start*/
                    if(!ENQUEUE_RRC_OAM_HPTOLP_THDH_PRIO_Q(&(rrcOamHPtoLPThdHPrioQ_g[internalCellIndex][UL_THD]),
                                RrcOamHiPrioQNode, ulRrcOamQNode_p))
                    {
                    /*SPR 21944 Changes End*/
                        FREE_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(ulRrcOamQNode_p);
                    }
                    /*Coverity 25573 Fix End*/

                    /* Send to the LP thread to process it */
                    lteMessageQSend( recvMsgFrmHPThdFd_g, (UInt8 *)&internalCellIndex, sizeof(InternalCellIndex));
                }
                /* SPR 16583 fix end */
            }
            break;
            case HO_RACH_CONN_HO_ENTITY_FAILURE:
            {
                UInt8 preambleIndex = ((ConnHOEntityFailureInterfaceNode *)(rachInterfaceQueueNode_p->rachInterface_p))->preambleIndex;
                /* SPR 14117 fix start */
                UInt16 tcRnti = ((ConnHOEntityFailureInterfaceNode *)(rachInterfaceQueueNode_p->rachInterface_p))->tcRnti;
                /* coverity fix- 70267,70311 start */
                if(!freeRNTIBeforeUETempContextCreation(tcRnti, internalCellIndex))
                {
                    LOG_MAC_MSG (MAC_RECV_INVALID_RNTI_ID, LOGERROR, MAC_PDCCH,\
                            getCurrentTick(),\
                            tcRnti,\
                            0,\
                            0,\
                            0,\
                            0,\
                            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                            __func__, "INVALID_RNTI_RECV");
                }
                /* coverity fix- 70267,70311 end */
                /* SPR 14117 fix end */

                freeMemPool(rachInterfaceQueueNode_p->rachInterface_p);
                /*CA Changes start  */
                freePreambleIndex(preambleIndex, internalCellIndex);
                /*CA Changes end  */
            }
            break;
            case RACH_DELETE_UE_HO:
            {
                UInt8 preambleIndex = ((RRCRachDeleteUeHoInterface *)(rachInterfaceQueueNode_p->rachInterface_p))->preambleIndex;
                freeMemPool(rachInterfaceQueueNode_p->rachInterface_p);
                /*CA Changes start  */
                if(INVALID_PREAMBLE_IDX != preambleIndex && PREAMBLE_BUSY == getPreambleStatusInfo(preambleIndex, internalCellIndex))
                {
                    freePreambleIndex(preambleIndex, internalCellIndex);
                }
                /*CA Changes end  */
            }
            break;
            case RACH_DELETE_UE:
            {
                UInt8 preambleIndex = ((RRCRachDeleteUeInterface *)(rachInterfaceQueueNode_p->rachInterface_p))->preambleIndex;
                UInt16 rnti = ((RRCRachDeleteUeInterface *)(rachInterfaceQueueNode_p->rachInterface_p))->rnti;
                freeMemPool(rachInterfaceQueueNode_p->rachInterface_p);
                /*CA Changes start  */
                if(INVALID_PREAMBLE_IDX != preambleIndex && PREAMBLE_BUSY == getPreambleStatusInfo(preambleIndex, internalCellIndex))
                {
                    freePreambleIndex(preambleIndex, internalCellIndex);
                }
                /*CA Changes start  */
                if(MAC_FAILURE == freeRNTIAfterUEDeletion(rnti, internalCellIndex))
                    /*CA Changes end  */
                {
                    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
                            "Free RNTI After UE Deletion Failed\n",__func__);

                }
            }
            break;
            case CHANGE_CRNTI:
            {
                ChangeCrntiInfo   changeCrntiInfo = {0};
                RrcOamHiPrioQNode *ulRrcOamQNode_p = PNULL;
                /* SPR 16049 Fix Start */
                DLUEContext *ueDLContext_p  = PNULL;
                UInt8 preambleIndex = 0;
                /* SPR 16049 Fix End */
                UInt16 ueIndex =((RRCRachChangeCRNTIInterface *)(rachInterfaceQueueNode_p->rachInterface_p))->ueIndex;
                UInt16 oldCrnti = ((RRCRachChangeCRNTIInterface *)(rachInterfaceQueueNode_p->rachInterface_p))->oldCrnti;
                UInt16 newCrnti = ((RRCRachChangeCRNTIInterface *)(rachInterfaceQueueNode_p->rachInterface_p))->newCrnti;
                freeMemPool(rachInterfaceQueueNode_p->rachInterface_p);
                changeCrntiInfo.oldCrnti = oldCrnti;
                changeCrntiInfo.newCrnti = newCrnti;
                changeCrntiInfo.ueIndex = ueIndex;
                /* SPR 16049 Fix Start */
                ueDLContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
                preambleIndex = ueDLContext_p->ulSyncLossOrHOInfoInDL.preambleIdx;
                /* SPR 16049 Fix End */
                /*CA Changes start  */
                updateCrntiToIndexTable(changeCrntiInfo, internalCellIndex);
                /*CA Changes end  */
                /* SPR 16049 Fix Start */
                if((INVALID_PREAMBLE_IDX != preambleIndex) && 
                        (PREAMBLE_BUSY == getPreambleStatusInfo(preambleIndex,internalCellIndex)))
                {
                    freePreambleIndex(preambleIndex,internalCellIndex);
                }
                /* SPR 16049 Fix End */

                GET_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(
                        ulRrcOamQNode_p, RrcOamHiPrioQNode);
                if(!ulRrcOamQNode_p)
                {
                    ltePanic("allocation fail for ulRrcOamQNode_p");
                }
                /* + coverity 35238 */
                else
                {
                    /* - coverity 35238 */
                    ulRrcOamQNode_p->rrcMsgId = MAC_CHANGE_CRNTI_REQ;
                    ulRrcOamQNode_p->ueIndex = ueIndex;
                    ulRrcOamQNode_p->respCode = MAC_SUCCESS;
                    ulRrcOamQNode_p->eventId = rachInterfaceQueueNode_p->eventId; 
                    /* CA changes Start */
                    /*Cov_fix_30April_25573_start*/
                    if(!ENQUEUE_RRC_OAM_HPTOLP_THDH_PRIO_Q(
                                &(rrcOamHPtoLPThdHPrioQ_g[internalCellIndex][UL_THD]),
                                RrcOamHiPrioQNode, ulRrcOamQNode_p))
                        /* CA changes End */
                    {
                        FREE_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(ulRrcOamQNode_p);
                    }
                    /*Cov_fix_30April_25573_stop*/
                    /* Send to the LP thread to process it */
                    lteMessageQSend( recvMsgFrmHPThdFd_g, (UInt8 *)&internalCellIndex, sizeof(InternalCellIndex));
                }
            }
            break;
            /*SPR 16855 +-*/
            default:
            break;
        }

        FREE_MEM_NODE_PDCCH_RACH_INTERFACE_Q(rachInterfaceQueueNode_p);
    }
    return;

}

/*****************************************************************************
 * Function Name  : scheduleRACHMsg
 * Inputs         : subFrameNum - System frame number w.r.t DL delay
 *                  dciCCEContainerInfo_p
 *                  pdcchULOutputInfo_p
 *                  currentGlobalTick
 *                  maxUeScheduledInDownlink_p
 *                  internalCellIndex - Cell-Index at MAC                                            
 * Outputs        : pdcchDLOutputInfo_p - Pointer to the pdcch container
 *                  dciCCEInfo_p - Pointer to the DCICCE container
 *                  availableCCE_p - Available CCE
 *                  availableCommonCCE_p - Available CCE in common space
 *                  availableDLRBCount_p - DL RB available
 * Returns        : None
 * Description    : This will call the functions processRARspQueue() and 
 *                  processContentionMsgQ() to schedule the messages
 *****************************************************************************/
void scheduleRACHMsg(PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        DciCCEInfo *dciCCEInfo_p,
        UInt32 *availableCCE_p,
        UInt32 *availableCommonCCE_p, 
        SInt32 *availableDLRBCount_p,
        UInt32 subFrameNum,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        PdcchULOutputInfo *pdcchULOutputInfo_p,
        /* SPR 15909 fix start */
                     /* +- SPR 17777 */
#ifdef TDD_CONFIG
                   tickType_t currentGlobalTick,
#endif
        /* SPR 15909 fix end */
        /* 5492 changes start */
                     UInt32 *maxUeScheduledInDownlink_p
                     /* +- SPR 17777 */
        /* 5492 changes end */
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */
        )
{
    UInt32 queueCount = 0;

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);
    /* + SPR 17733 */
#ifdef FDD_CONFIG
    /* Process MSG4 retransmission failure queue */
    if(sQueueCount(&Msg4RetransmissionFailureQueue_gp[internalCellIndex]))
    {
        processMsg4RetransmissionFailureQueue(pdcchDLOutputInfo_p, 
                dciCCEContainerInfo_p, 
                availableCommonCCE_p, availableCCE_p, 
                availableDLRBCount_p, subFrameNum, 
                pdcchULOutputInfo_p,
                internalCellIndex );
    }

    /* Process MSG4 retransmission queue */
    if(sQueueCount(&Msg4RetransmissionQueue_gp[internalCellIndex]))
    {
        processMsg4RetransmissionQueue(pdcchDLOutputInfo_p, 
                dciCCEContainerInfo_p, 
                availableCommonCCE_p, availableCCE_p, 
                availableDLRBCount_p, subFrameNum, 
                pdcchULOutputInfo_p,
                internalCellIndex);
    }

#endif
    /* - SPR 17733 */


    /*CA Changes start  */
    queueCount = COUNT_DL_RA_RSP_QUEUE_Q(internalCellIndex);
    /*CA Changes end  */

    if (queueCount)
    {
        processRARspQueue(pdcchDLOutputInfo_p,
                dciCCEInfo_p,
                queueCount,
                availableCCE_p,
                availableCommonCCE_p,
                availableDLRBCount_p,
                dciCCEContainerInfo_p,
                          /* +- SPR 17777 */
#ifdef TDD_CONFIG
                currentGlobalTick,
#endif
                          /* +- SPR 17777 */
                /*CA Changes start  */
                internalCellIndex);
        /*CA Changes end  */
    }

    queueCount = 0;
    queueCount = COUNT_CONTENTION_RESOLUTION_SQ(internalCellIndex);

    /* 5492 changes start */
    if ( queueCount 
            && (*maxUeScheduledInDownlink_p)
       )
        /* 5492 changes end */
    {
        processContentionMsgQ(pdcchDLOutputInfo_p,
                dciCCEInfo_p,
                availableCCE_p,
                availableCommonCCE_p,
                availableDLRBCount_p,
                queueCount,
                subFrameNum,
                dciCCEContainerInfo_p,
                /*SPR 1101 changes start*/    
                pdcchULOutputInfo_p
                /*SPR 1101 changes end*/
                /* 5492 changes start */
                ,maxUeScheduledInDownlink_p
                /* 5492 changes end */
                ,internalCellIndex
                );
    }

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);

    return; 
}


/*****************************************************************************
 * Function Name  : processRARspQueue
 * Inputs         : rarQueueCount - Number of node in RARsp queue
 *                  dciCCEContainerInfo_p -
 *                  currentGlobalTick -
 *                  subFrameNum -
 *                  internalCellIndex - Cell-Index at MAC                                            
 * Outputs        : pdcchDLOutputInfo_p - Pointer to the pdcch container
 *                  dciCCEInfo_p - Pointer to the DCICCE container 
 *                  availableCCE_p - Available CCE in UE specific space
 *                  availableCommonCCE_p - Available CCE in UE common space
 *                  availableDLRBCount_p - Available DL RB
 * Returns        : None
 * Description    : This will process the RA Response queue and schedules 
 *                  the RARs in DL
 *****************************************************************************/




void processRARspQueue(PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        DciCCEInfo        *dciCCEInfo_p,
        UInt32 rarQueueCount,
        UInt32 *availableCCE_p,
        UInt32 *availableCommonCCE_p,
        SInt32 *availableDLRBCount_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        /* +- SPR 17777 */
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
#endif
        /* +- SPR 17777 */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
        )
{
    UInt16 tcrntiCnt = 0;
    UInt32 retVal           = MAC_FAILURE;
    /* SPR 15909 fix start */
    tickType_t currentGlobalTTI = 0;
    /* SPR 15909 fix end */
    UInt32 agregationLvl    = AGGREGATION_LEVEL_FOR_COMMON_SPACE;
    DLRARsp          *raRsp_p      = PNULL;
    DLRARspQueueNode *raRspQnode_p = PNULL;
    /*Start Changes TDD DL UL SPLIT*/
#ifdef FDD_CONFIG
    /* + TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
    UInt8         tempHarqProcessId = 0;
    UInt32        ueId = 0;
    ULUEContext  *ulUEContext_p = PNULL;
    ULHarqInfo*   tempULHarqInfo_p = PNULL;
    /* - TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
#endif
#ifdef DL_UL_SPLIT
    RAReqQueueNode *raQNode_p = PNULL;

    UInt8 relResCnt = 0;
#endif
    /*End Changes TDD DL UL SPLIT*/
    /*533-GCC-warning-fix*/
#if defined(LOG_PRINT_ENABLED) || defined(DL_UL_SPLIT)
    UInt8 numRAResponse = 0;
    DLRARsp *dlRARsp_p = PNULL;
#endif    
    /*533-GCC-warning-fix*/
    /*SPR 7086 Fix Start*/
    UInt8 rarStartRB = 0;
    UInt8 rarEndRB = 0;
    /*SPR 7086 Fix End*/
    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    currentGlobalTTI = getCurrentTick();
    /* msg3 ulconfig fix start */
#ifdef TDD_CONFIG    
    /* TDD Config 0 Changes Start */
    UInt8 currentSubFrame = (currentGlobalTick % MAX_SUB_FRAME);
    /*SPR 15909 fix start*/
    tickType_t ulSfTti = currentGlobalTick + getULSfForMsg3(currentSubFrame,internalCellIndex);
    /*SPR 15909 fix end*/
    UInt8 ulSubframe = (ulSfTti % MAX_SUB_FRAME);
    UInt32 ulSFN = (MODULO_ONEZEROTWOFOUR(ulSfTti/MAX_SUB_FRAME));
    UInt8 ulSFNIndex = MODULO_TWO(ulSFN); 
    /* TDD Config 0 Changes End */
    /* msg3 ulconfig fix end */

#elif FDD_CONFIG    
    /* SPR 1600 changes start */
    ULSchUciInfo   * ulSchUciInfo_p = PNULL;
    UInt8 containerTick = (currentGlobalTTI + PHY_DL_DELAY + RAR_MSG3_FDD_OFFSET) % MAX_PDCCH_CONTAINER; 
    ULSchUciRachPoolInfo* ulschUciRachInfo_p
        /*CA Changes start  */
        = ulSchUciRachContainer_gp[internalCellIndex] + containerTick;
    /*CA Changes end  */
    UInt16 numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
    /* SPR 5502 changes start */
    UInt32 ueIndex = INVALID_UE_INDEX;
    /* SPR 5502 changes end */
    /* SPR 10136 fix start*/
    UInt8 ulSubframe = (currentGlobalTTI + PHY_DL_DELAY + FDD_DATA_OFFSET) % MAX_SUB_FRAME;
    /* SPR 10138 fix end*/

#endif    
    /* SPR 1600 changes end */

    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
    /* + PERF_CA */
    LteCellMacULPerfStats *lteCellMacULPerfStats = &(gMacCellPerfStats_p[internalCellIndex]-> \
            lteCellMacULPerfStats);
    /* - PERF_CA */
#endif
    /* - PERF_STATS_PH1D1 */

    while (rarQueueCount)
    {
        /*CA Changes start  */
        DEQUEUE_DL_RA_RSP_QUEUE_Q(DLRARspQueueNode, (void**)&raRspQnode_p, internalCellIndex);
        /*CA Changes end  */
        if (PNULL == raRspQnode_p)
        {
            LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
                    "[%s]: Node poped from dlRARspQueue_g Queue is NULL\n",
                    __func__);
            break;
        }

        raRsp_p = &raRspQnode_p->rarToDl;
        /* SPR 3429 Start */
        if (currentGlobalTTI > (raRsp_p->exptti + PHY_DL_DELAY))
            /* SPR 3429 End */

        {
            LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                    "[%s]: ERROR currentGlobalTTI [%d] > expiryTTI [%d]\n",
                    __func__, currentGlobalTTI, raRsp_p->exptti);


            /*533-GCC-warning-fix*/
#if defined(LOG_PRINT_ENABLED) || defined(DL_UL_SPLIT)
            dlRARsp_p = &raRspQnode_p->rarToDl;
            numRAResponse = dlRARsp_p->numRAR;
#endif
            /*533-GCC-warning-fix*/
            LOG_MAC_MSG(MAC_FREE_RAR_ID,LOGWARNING,MAC_RACH,
                    currentGlobalTTI,
                    raRspQnode_p->rarToDl.raRNTI,
                    raRspQnode_p->rarToDl.mcsIndex,
                    raRspQnode_p->rarToDl.exptti,
                    numRAResponse,
                    LINE_NUMBER,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"RAR_TTI_EXPIRED");

            LOG_MAC_MSG(MAC_FREE_RAR_MSG3_ID,LOGWARNING,MAC_RACH,
                    currentGlobalTTI,
                    raRspQnode_p->rarToDl.raRNTI,
                    (raRspQnode_p->rarToDl.tti % NUM_OF_RB_MAP),
                    raRspQnode_p->rarToDl.tti,
                    LINE_NUMBER,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"RAR_TTI_EXPIRED");

            freeRARAfterULRBAllocation(raRspQnode_p, internalCellIndex);

            rarQueueCount--;

            /* SPR 3429 Start */
            if (raRspQnode_p)
            {
                /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CBRA += raRsp_p->contBasedPreambleCount;
                lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CFRA += raRsp_p->contFreePreambleCount;
#endif
                /* - PERF_STATS_PH1D1 */
                /* SPR 19427 fix start */
                UInt32 x = 0;
                /*Free the existing UL RBs allocated*/
                FREE_UL_RAR_RB(raRspQnode_p->rarToDl,
                        x, raRspQnode_p->rarToDl.numRAR,
                        internalCellIndex);
                msgFree(raRspQnode_p->rarToDl.macPduData_p);
                raRspQnode_p->rarToDl.macPduData_p = PNULL;
                /* SPR 19427 fix end */
                freeMemPool(raRspQnode_p);
                raRspQnode_p = PNULL;

            }
            /* SPR 3429 End */

            continue;
        }
        if ((agregationLvl > *availableCommonCCE_p) ||
                (raRspQnode_p->rarToDl.numRB > *availableDLRBCount_p))
        {

            LOG_MAC_MSG(MAC_RAR_AGGLEVEL_AVAILRB_LESS_ID,LOGWARNING,MAC_RACH,\
                    currentGlobalTTI,\
                    agregationLvl,\
                    *availableCommonCCE_p,\
                    raRspQnode_p->rarToDl.raRNTI,\
                    raRspQnode_p->rarToDl.numRB,\
                    raRspQnode_p->rarToDl.mcsIndex,\
                    0.0,0.0,\
                    __func__,"RAR_AGGLEVEL_AVAILRB_LESS");

#ifdef DL_UL_SPLIT
            while (relResCnt < numRAResponse)
            {
                raQNode_p = 
                    dlRARsp_p->resourceInfo[relResCnt].raReqQNode_p;

                if(PNULL != raQNode_p)
                {
                    freeMemPool(raQNode_p);
                }
                relResCnt++;

            }

            rarQueueCount--;

            if (raRspQnode_p)
            {
                /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CBRA += raRsp_p->contBasedPreambleCount;
                lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CFRA += raRsp_p->contFreePreambleCount;
#endif
                /* - PERF_STATS_PH1D1 */
                freeMemPool(raRspQnode_p);
                raRspQnode_p = PNULL;

            }

            continue;

#else             
            /* + TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
#ifdef FDD_CONFIG            

        /* SPR 21870 Fix End */
            freeAllTTIBRARResources(raRsp_p, 0, raRsp_p->numRAR,internalCellIndex);
        /* SPR 21870 Fix End */
#endif
            /* - TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
            /*CA Changes start  */
            putEntryInULNegAckRARspQueue(raRspQnode_p, internalCellIndex);
            /*CA Changes end  */

#ifdef TDD_CONFIG
            /* + E_CID_5.3 */
            UInt8 cnt = raRspQnode_p->rarToDl.numRAR;
            while (cnt--)
            {
                if ((cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                            rachConfigInfo.rachConfiguration.countOfContentionBasedRAPreambles - 1) < 
                        raRspQnode_p->rarToDl.resourceInfo[cnt].raReqQNode_p->raPreambleInfo.preamble ) 
                {
                    continue;
                }
                /* + Bandwidth Fix */
                /*TCRNTI timer list*/                           

                /* SPR 12580 Fix Start */
                /* Timer node was getting deleted while processing the queue ulRARspNegAckQueue_g twice.
                 * Removed the handling from here. Its getting freed at the time of RAR Nack Procesing */
                /* SPR 12580 Fix End */

                if(raRspQnode_p->index)
                {
                    raRspQnode_p->index--;
                }
                /* - Bandwidth Fix */
            }
            /* - E_CID_5.3 */
#endif
            rarQueueCount--;
            continue;
#endif            
        }

        retVal = allocateCCEInCommonSpaceForRARMsg(
                pdcchDLOutputInfo_p,
                dciCCEInfo_p,
                raRspQnode_p,
                availableCCE_p,
                dciCCEContainerInfo_p,
                /*CA Changes start  */
                internalCellIndex);
        /*CA Changes end  */
        if ( MAC_FAILURE == retVal )
        {
#ifdef DL_UL_SPLIT
            while (relResCnt < numRAResponse)
            {
                raQNode_p = 
                    dlRARsp_p->resourceInfo[relResCnt].raReqQNode_p;

                if(PNULL != raQNode_p)
                {
                    freeMemPool(raQNode_p);
                }
                relResCnt++;

            }

            rarQueueCount--;

            if (raRspQnode_p)
            {
                /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CBRA += raRsp_p->contBasedPreambleCount;
                lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CFRA += raRsp_p->contFreePreambleCount;
#endif
                /* - PERF_STATS_PH1D1 */
                freeMemPool(raRspQnode_p);
                raRspQnode_p = PNULL;
            }

            continue;

#else    
#ifdef FDD_CONFIG
            /* + SPR_17858_587_CHANGES */
            LOG_MAC_MSG(MAC_RAR_AGGLEVEL_AVAILRB_LESS_ID,LOGWARNING,MAC_RACH,\
                    currentGlobalTTI,\
                    agregationLvl,\
                    *availableCommonCCE_p,\
                    raRspQnode_p->rarToDl.raRNTI,\
                    raRspQnode_p->rarToDl.numRB,\
                    raRspQnode_p->rarToDl.mcsIndex,\
                    0.0,0.0,\
                    __func__,"CCE_NOT_Allocated");
            /* - SPR_17858_587_CHANGES */
            /* + TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
        /* SPR 21870 Fix End */
            freeAllTTIBRARResources(raRsp_p, 0, raRsp_p->numRAR,internalCellIndex);
        /* SPR 21870 Fix End */
#endif
            /* - TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
            /*CA Changes start  */
            putEntryInULNegAckRARspQueue(raRspQnode_p, internalCellIndex);
            /*CA Changes end  */


            rarQueueCount--;


            continue;
#endif            
        }
        /* SPR 1600 changes start */
        else
        {

            for (tcrntiCnt = 0; tcrntiCnt < raRspQnode_p->rarToDl.numRAR; tcrntiCnt++)
            {
                /* msg3 ulconfig fix start */
#ifdef TDD_CONFIG        
                /* Insert the node into the ULConfigForRach according to the
                 * DCI Grant SF for the uplink sub frame where corresponding 
                 * msg3 needs to be recieved 
                 */
                UlschUciInfoForULConfigForRach 
                    *ulschUciInfoForULConfigForRach_p = PNULL;

                GET_MEM_FROM_POOL(UlschUciInfoForULConfigForRach,\
                        ulschUciInfoForULConfigForRach_p,\
                        sizeof(UlschUciInfoForULConfigForRach),PNULL);
                /*coverity-530-25317*/
                if(PNULL == ulschUciInfoForULConfigForRach_p)
                {
                    continue;
                }
                /*coverity-530-25317*/
                ulschUciInfoForULConfigForRach_p->tcRNTI = 
                    raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].tcRNTI;
                /* TDD Config 0 Chnages Start */
                ulschUciInfoForULConfigForRach_p->ulSubframe = 
                    ulSubframe;
                ulschUciInfoForULConfigForRach_p->containerIndex = ulSFNIndex;
                /* TDD Config 0 Chnages End */
                ulschUciInfoForULConfigForRach_p->ulHarqInfo_p = 
                    raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].ulHarq;
                /* FIX DMRS TDD START */
                /* SPR 3123 CHANGES START */
                ulschUciInfoForULConfigForRach_p->ulHarqInfo_p->nDmrs = 0;
                /* SPR 3123 CHANGES END */
                /* FIX DMRS TDD END */
                /* SPR 4300 Chg */
                ulschUciInfoForULConfigForRach_p->harqProcessId = 
                    raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].harqProcessId;
                /* SPR 4300 Chg */
                /* SPR 3211, 6478 Fix Start */
                ulschUciInfoForULConfigForRach_p->ulHarqInfo_p->freqEnabledFlag = 0;
                ulschUciInfoForULConfigForRach_p->ulHarqInfo_p->freqHoppingBits = 0;
                /* SPR 3211, 6478 Fix End */
                /*TDD SB_CQI*/
                ulschUciInfoForULConfigForRach_p->ueIndex =
                    raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].ueIndex;
                ulschUciInfoForULConfigForRach_p->isCqiWithCFRPresent =
                    raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].isCqiWithCFRPresent;
                /*TDD SB_CQI*/
                /*SPR 7086 Fix Start*/
                /*Check Collision with RAR RB's*/
                rarStartRB = raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].riStart;
                rarEndRB = (UInt8)(raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].riStart +
                        raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].riLength);
                /* SPR 10136 fix start*/
                if(LTE_TRUE == *(srsCellConfigMap_gp[internalCellIndex] + ulSubframe))
                {
                    if((rarEndRB < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                                srsFreqDomainStartRB) ||
                            (rarStartRB > cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                             srsFreqDomainEndRB))
                    {
                        ulschUciInfoForULConfigForRach_p->nSRS = SRS_ULSCH_NO_OVERLAP;
                    }
                    else
                    {
                        ulschUciInfoForULConfigForRach_p->nSRS = SRS_ULSCH_OVERLAP;

                    }
                }
                else
                {
                    ulschUciInfoForULConfigForRach_p->nSRS = SRS_ULSCH_NO_OVERLAP;
                }
                /* SPR 10136 fix end*/
                /*SPR 7086 Fix End*/

                /** CA-TDD Changes Start **/
                if( pushNode(&ulschUciInfoForULConfigForRach_g[internalCellIndex],
                            &ulschUciInfoForULConfigForRach_p->nodeAnchor))
                    /** CA-TDD Changes End **/
                {
                    freeMemPool(ulschUciInfoForULConfigForRach_p);
                    lteWarning("Push Node failed for ULConfigForRach\n");
                }

                /* msg3 ulconfig fix end */
#elif FDD_CONFIG

                /*Fill rnti value */
                ulSchUciInfo_p = &(ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent]);

                ulSchUciInfo_p->rnti = raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].tcRNTI;

                /* Fill UL Harq Info*/
                ulSchUciInfo_p->ulHarqInfo_p = 
                    raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].ulHarq;

                ulSchUciInfo_p->ulHarqProcessId = 
                    raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].harqProcessId;
                /* SPR 3123 CHANGES START */
                ulSchUciInfo_p->ulHarqInfo_p->nDmrs = 0;
                /* SPR 3123 CHANGES END */
                /* SPR 3211 CHANGES START */
                ulSchUciInfo_p->ulHarqInfo_p->freqEnabledFlag = 0;
                ulSchUciInfo_p->ulHarqInfo_p->freqHoppingBits = 0;
                /* SPR 3211 CHANGES END */
                ulSchUciInfo_p->pduType = MAC_SS_PDU_ULSCH;
                /* + CQI_5.0 */
                /*SPR 5502 changes start */
                ueIndex = raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].ueIndex;
                /*SPR 7086 Fix Start*/
                /*Check Collision with RAR RB's*/
                rarStartRB = raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].riStart;
                rarEndRB = (UInt8)(raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].riStart + 
                        raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].riLength);

                /* SPR 10136 fix start*/
                if(LTE_TRUE == *(srsCellConfigMap_gp[internalCellIndex] 
                            + ulSubframe))
                {
                    if((rarEndRB < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                cellParams_p->srsFreqDomainStartRB) ||
                            (rarStartRB > cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                             cellParams_p->srsFreqDomainEndRB))
                    {
                        ulSchUciInfo_p->nSRS = SRS_ULSCH_NO_OVERLAP;
                    }
                    else
                    {
                        ulSchUciInfo_p->nSRS = SRS_ULSCH_OVERLAP;
                    }
                }
                else
                {
                    ulSchUciInfo_p->nSRS = SRS_ULSCH_NO_OVERLAP;
                }
                /* SPR 10136 fix end*/
                /*SPR 7086 Fix End*/
                /* + TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
                ueId = raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].ueIndex;
                if( ueId != INVALID_UE_INDEX )
                {
                    ulUEContext_p = ulUECtxInfoArr_g[ueId].ulUEContext_p;
                    /*SPR 19625 Fix start */
                    if( (FALSE == ulUECtxInfoArr_g[ueId].pendingDeleteFlag) &&
                            (PNULL != ulUEContext_p) && (ulUEContext_p->ttiBundlingEnabled))
                    {
                        /*SPR 19625 Fix End*/
                        tempHarqProcessId = raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].harqProcessId;
                        tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[tempHarqProcessId]);
                        if(tempULHarqInfo_p->isTTIBundled)
                        {
                            UInt32 enqueueTick = raRspQnode_p->rarToDl.tti - FDD_DATA_OFFSET;
                            enqueueNodesInTTIBScheduleQ(enqueueTick,ueId,tempHarqProcessId,1,FALSE,
                                    internalCellIndex);
                        }
                    }
                    /* - TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */

                    if( TRUE == raRspQnode_p->rarToDl.resourceInfo[tcrntiCnt].isCqiWithCFRPresent)
                    {
                        DLUEContext *dlUEContext_p = PNULL;
                        ULUEContextInfo *ulUEContextInfo_p = PNULL;
                        ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
                        dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
                        ulSchUciInfo_p->pduType = MAC_SS_PDU_ULSCH_CQI_RI;
                        ulUEContextInfo_p->ulUEContext_p->aperiodicCQIToBeRequested
                            = FALSE;
                        storeCqiPmiRiDataAperiodic(
                                dlUEContext_p->dlMIMOInfo.transmissionMode, 
                                ulUEContextInfo_p, ulschUciRachInfo_p, ulSubframe, 
                                numberOfInfoPresent,internalCellIndex);
                    }    
                }
                /*SPR 5502 changes end */
                /* - CQI_5.0 */

                numberOfInfoPresent++;   

                LOG_MAC_MSG(MAC_MSG3_ULSCH_ID,LOGBRIEF,MAC_RACH,
                        getCurrentTick(),
                        ulSchUciInfo_p->rnti,
                        ulSchUciInfo_p->ulHarqInfo_p->ulHarqProcessId,
                        ulSchUciInfo_p->ulHarqInfo_p->riStart,
                        ulSchUciInfo_p->ulHarqInfo_p->riLength,
                        (currentGlobalTTI + PHY_DL_DELAY) ,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        __func__,"ULSCH_MSG3");

                LOG_MAC_MSG(MAC_MSG3_ULSCH_CONTAINER_ID,LOGBRIEF,MAC_RACH,
                        getCurrentTick(),
                        ulSchUciInfo_p->rnti,
                        ulSchUciInfo_p->ulHarqInfo_p->ulHarqProcessId,
                        (currentGlobalTTI + PHY_DL_DELAY) ,
                        containerTick,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        __func__,"ULSCH_MSG3");

#endif       

            }
        }
        /* SPR 1600 changes end */

        *availableCommonCCE_p -= agregationLvl;
        *availableDLRBCount_p -= raRspQnode_p->rarToDl.numRB;

        /* SPR 1756 changes start */
        if (raRspQnode_p)
        {
            freeMemPool(raRspQnode_p);
            raRspQnode_p = PNULL;
        }
        /* SPR 1756 changes end */


        rarQueueCount--;
    }
    /* SPR 1600 changes start */
#ifdef FDD_CONFIG
    /*Update Number of numberOfInfoPresent in the global context */ 
    ulschUciRachInfo_p->numberOfInfoPresent =  numberOfInfoPresent;
#endif    
    /* SPR 1600 changes end */

    return;
}

/*****************************************************************************
 * Function Name  : freeRARAfterULRBAllocation
 * Inputs         : raRspQnode_p
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Description    : Only for testing purpose to see the logs
 ******************************************************************************/

void freeRARAfterULRBAllocation(DLRARspQueueNode *raRspQnode_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    UInt8 relResCnt = 0, preambleIndex = 0;
    UInt8 numRAResponse = 0;
    DLRARsp *dlRARsp_p = PNULL;
    RAReqQueueNode *raQNode_p = PNULL;
            /* + SPR_17858_587_CHANGES */
#ifdef FDD_CONFIG
    UInt16 tcRnti  = INVALID_RNTI;
#endif
            /* - SPR_17858_587_CHANGES */
    UInt16 ueIndex = INVALID_UE_ID;
    ContentionFreePreamble *contFreePreamble_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
            /* + SPR_17858_587_CHANGES */
#ifdef FDD_CONFIG
    TempCRNTICtx     *tempCRNTICtx_p = PNULL;
    ULUEContext* ulUEContext_p=PNULL;
#endif
            /* - SPR_17858_587_CHANGES */


    /*CA Changes start  */
    RACHConfiguration *rachConfig_p =
        &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                rachConfigInfo.rachConfiguration);
    /*CA Changes end  */

    dlRARsp_p = &raRspQnode_p->rarToDl;
    numRAResponse = dlRARsp_p->numRAR;

    while (relResCnt < numRAResponse)
    {
        raQNode_p =
            dlRARsp_p->resourceInfo[relResCnt].raReqQNode_p;
            /* + SPR_17858_587_CHANGES */
#ifdef FDD_CONFIG
        tcRnti =
            dlRARsp_p->resourceInfo[relResCnt].tcRNTI;
#endif
            /* - SPR_17858_587_CHANGES */
        if(PNULL != raQNode_p)
        {
            preambleIndex = raQNode_p->raPreambleInfo.preamble;

            if(rachConfig_p->countOfContentionBasedRAPreambles >
                    preambleIndex)
            {
#ifdef FDD_CONFIG
            /* + SPR_17858_587_CHANGES */
                tempCRNTICtx_p = getTempCRNTIContextPointer(tcRnti,internalCellIndex);
                if (tempCRNTICtx_p)
                {
                    ulHarqTimerForMsg3Stop(tempCRNTICtx_p,\
                            tempCRNTICtx_p->ulHarq.harqTimerExpirySF,\
                            tempCRNTICtx_p->ulHarq.harqTimerExpirySystemFrame,internalCellIndex);
#endif
                    LOG_MAC_MSG(RA_REQ_FREE_ID,
                            LOGDEBUG,
                            MAC_RACH,
                            getCurrentTick(),
                            raQNode_p->raRNTI,
                            raQNode_p->ttiRAReqReceived,
                            raQNode_p->raPreambleInfo.preamble,
                            LINE_NUMBER,
#ifdef FDD_CONFIG
                            tcRnti,
#else
                            0,
#endif
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"CBR_RAR_TTI_EXPIRED");
#ifdef FDD_CONFIG
                }
            /* - SPR_17858_587_CHANGES */
#endif
            }
            else
            {

                /*CA Changes start  */
                if(PREAMBLE_BUSY != getPreambleStatusInfo(preambleIndex, internalCellIndex))
                    /*CA Changes end  */
                {

                    LOG_MAC_MSG(CFR_RA_REQ_FREE_ID,
                            LOGERROR,
                            MAC_RACH,
                            getCurrentTick(),
                            raQNode_p->raRNTI,
                            raQNode_p->ttiRAReqReceived,
                            raQNode_p->raPreambleInfo.preamble,
                            raQNode_p->raPreambleInfo.tmcs,
                            LINE_NUMBER,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"CFR_RAR_TTI_EXPIRY");

                    freeMemPool(raQNode_p);
                    relResCnt++;
                    continue;


                }

                contFreePreamble_p =
                    /*CA Changes start  */
                    getContentionFreePreambleInfo(preambleIndex, internalCellIndex);
                /*CA Changes end  */


                if(( PDCCH_ORDER !=
                            contFreePreamble_p->pdcchOrderHandOverFlag ) && 
                        (CONNECTION_HANDOVER != contFreePreamble_p->pdcchOrderHandOverFlag))
                {
                    LOG_MAC_MSG(CFR_RA_REQ_FREE_ID,
                            LOGERROR,
                            MAC_RACH,
                            getCurrentTick(),
                            raQNode_p->raRNTI,
                            raQNode_p->ttiRAReqReceived,
                            raQNode_p->raPreambleInfo.preamble,
                            raQNode_p->raPreambleInfo.tmcs,
                            LINE_NUMBER,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"CFR_RAR_TTI_EXPIRY");

                    freeMemPool(raQNode_p);
                    relResCnt++;
                    continue;
                }

                ueIndex = contFreePreamble_p->ueIdentifier;

                dlUEContext_p =
                    dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
            /* + SPR_17858_587_CHANGES */
#ifdef FDD_CONFIG
                ulUEContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
                if((PNULL == dlUEContext_p)||(PNULL == ulUEContext_p))
#else 
                if(PNULL == dlUEContext_p)
#endif
            /* - SPR_17858_587_CHANGES */
                {

                    LOG_MAC_MSG(PREAMBLE_NO_UECONTEXT_ID,
                            LOGERROR,
                            MAC_RACH,
                            getCurrentTick(),
                            preambleIndex,
                            contFreePreamble_p->pdcchOrderHandOverFlag,
                            contFreePreamble_p->ueIdentifier,
                            LINE_NUMBER,
#ifdef FDD_CONFIG
/* SPR 20636 Changes Start */
                            (ADDR)ulUEContext_p, /*SPR_17858_587_CHANGES*/
/* SPR 20636 Changes End */
#else
                            DEFAULT_INT_VALUE,
#endif
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"PREAMBLE_NO_DLUECONTEXT_ID");

                    LOG_MAC_MSG(CFR_RA_REQ_FREE_ID,
                            LOGERROR,
                            MAC_RACH,
                            getCurrentTick(),
                            raQNode_p->raRNTI,
                            raQNode_p->ttiRAReqReceived,
                            raQNode_p->raPreambleInfo.preamble,
                            raQNode_p->raPreambleInfo.tmcs,
                            LINE_NUMBER,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"CFR_FREE_NO_DLUECONTEXT");

                    freeMemPool(raQNode_p);
                    relResCnt++;
                    continue;

                }
#ifdef FDD_CONFIG
            /* + SPR_17858_587_CHANGES */
                /*Here rcvTTI is passed as 0 as it is no used in
                 *                    * function
                 *                                       */
                /* +- SPR 17777 */

               if(MAC_FAILURE == (ulHarqTimerStop(ulUEContext_p,\
                        ulUEContext_p->ulresInfo[dlRARsp_p->resourceInfo[relResCnt].harqProcessId].harqTimerExpirySF,\
                        ulUEContext_p->ulresInfo[dlRARsp_p->resourceInfo[relResCnt].harqProcessId].harqTimerExpirySystemFrame,internalCellIndex)))
		{
			LOG_MAC_MSG( MAC_UL_HARQ_TIMER_STOP_FAIL, LOGWARNING, MAC_UL_HARQ,
                                getCurrentTick(), __LINE__, ulUEContext_p->ueIndex, raQNode_p->raRNTI,
                                ulUEContext_p->ulresInfo[dlRARsp_p->resourceInfo[relResCnt].harqProcessId].harqTimerExpirySF, raQNode_p->raPreambleInfo.preamble, 0,0, __func__, "");	
		}
            /* - SPR_17858_587_CHANGES */
#endif


                LOG_MAC_MSG(CFR_RA_REQ_FREE_ID,
                        LOGERROR,
                        MAC_RACH,
                        getCurrentTick(),
                        raQNode_p->raRNTI,
                        raQNode_p->ttiRAReqReceived,
                        raQNode_p->raPreambleInfo.preamble,
                        raQNode_p->raPreambleInfo.tmcs,
                        LINE_NUMBER,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"CFR_RAR_TTI_EXPIRY");

            }
            freeMemPool(raQNode_p);
        }
        relResCnt++;
    }

}


/*****************************************************************************
 * Function Name  : processContentionMsgQ
 * Inputs         : contentionQueueCount - No.of nodes in contention queue
 *                  subFrameNum - Sub frame number w.r.t DL delay
 *                  internalCellIndex - Cell-Index at MAC                                             
 *                  pdcchULOutputInfo_p -
 *                  maxUeScheduledInDownlink_p -
 * Outputs        : pdcchDLOutputInfo_p - Pointer to the pdcch container
 *                  dciCCEInfo_p - Pointer to the DCICCE container 
 *                  availableCCE_p - Available CCE
 *                  availableCommonCCE_p - Available CCE in UE common space
 *                  availableDLRBCount_p - Available DL RB
 *                  dciCCEContainerInfo_p - Pointer to DCI CCE Container info
 * Returns        : None
 * Description    : This will process the contention resolution message queue 
 *                  populated by UL scheduler
 *****************************************************************************/
void processContentionMsgQ(PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        DciCCEInfo *dciCCEInfo_p,
        UInt32 *availableCCE_p,
        UInt32 *availableCommonCCE_p,
        SInt32 *availableDLRBCount_p,
        UInt32 contentionQueueCount,
        UInt32 subFrameNum,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        PdcchULOutputInfo *pdcchULOutputInfo_p
        /* 5492 changes start */
        ,UInt32 *maxUeScheduledInDownlink_p
        /* 5492 changes end */
        ,InternalCellIndex internalCellIndex
        )
{
    UInt32 retVal = MAC_FAILURE;
    /* SPR 15909 fix start */
    tickType_t currentTTI = 0;
    /* SPR 15909 fix end */
    UInt32 aggregationLevelForUespecific = AGGREGATION_LEVEL_FOUR;
    UInt32 aggregationLevelForCommon     = AGGREGATION_LEVEL_FOR_COMMON_SPACE;
    UInt32 allocFromCommonSpace = FALSE;
    /* 5492 changes start */
    /* 5492 changes end */

    RNTIInfo  *rntiInfo_p = PNULL;
    TempCRNTICtx *tempUEContext_p = PNULL;
    ContentionQueueNode *contentionQueueNode_p = PNULL;
    /* SPR 3628 chg */
#ifdef FDD_CONFIG
   /* + SPR 17733 */
    DLHARQProcess *harqProcess_p = PNULL;
    /* - SPR 17733 */
#endif
#ifdef TDD_CONFIG
    UInt8 SSFFlag = FALSE;
#endif
    /* SPR 3628 chg */

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    currentTTI = getCurrentTick();
    /* 5492 changes start */
    /* 5492 changes end */
    /* SPR 3628 chg */
#ifdef TDD_CONFIG
    if(checkForSpecialSubframe(subFrameNum, internalCellIndex))
    {
        SSFFlag = TRUE;
    }
#endif
    /* SPR 3628 chg */

    /* 5492 changes start */
    while (
            ( *maxUeScheduledInDownlink_p ) &&
            contentionQueueCount)
    {
        /* SPR 4063 End */
        allocFromCommonSpace = FALSE;
        /* 5492 changes end */
        GET_NODE_CONTENTION_RESOLUTION_SQ(ContentionQueueNode, contentionQueueNode_p, internalCellIndex);

        /* SPR#706 Changes Start */
        if (PNULL != contentionQueueNode_p)
            /* SPR#706 Changes End */
        {
            tempUEContext_p = 
                getTempCRNTIContextPointer(contentionQueueNode_p->tcRNTI, internalCellIndex);

            if ((PNULL == tempUEContext_p) ||
                    (currentTTI > contentionQueueNode_p->contentionResExp))
            {
                DEQUEUE_CONTENTION_RESOLUTION_SQ(ContentionQueueNode,contentionQueueNode_p, internalCellIndex);

                if (PNULL != contentionQueueNode_p)
                {
                    /* + coverity 32685 */
                    if (contentionQueueNode_p->msg_p)
                    {
                        msgFree(contentionQueueNode_p->msg_p);
                    }
                    /* - coverity 32685 */
                    FREE_MEM_NODE_CONTENTION_RESOLUTION_SQ(contentionQueueNode_p);
                }

                if (tempUEContext_p)
                {
                    /* Fix SPR 21492 start */ 
                    ((ContentionTimerNode *)tempUEContext_p->timerNode_p)->\
                        deleteFlag = 255;
                    /* Fix SPR 21492 End */ 
                }
                contentionQueueCount--;
                continue;
            }
        }
        else
        {
            break;
        }
        /* SPR 3628 chg */
#ifdef TDD_CONFIG
        if(SSFFlag)
        {
            if (contentionQueueNode_p->numRBForSSF > *availableDLRBCount_p)
            {
                LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                        "[%s]: WARNING: RBs not available breaking the loop\n",
                        __func__);
                break;
            }
        }
#endif
        /* SPR 3628 chg */

        if (contentionQueueNode_p->numRB > *availableDLRBCount_p)
        {
            LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                    "[%s]: WARNING: RBs not available breaking the loop\n",
                    __func__);
            break;
        }

        /* + SPR 17733 */
#ifdef FDD_CONFIG
        /* coverity fix 96906 */
        harqProcess_p = checkForFreeHarqProcessForTcrnti(
                tempUEContext_p->dlHarqCtx_p);
        if(PNULL == harqProcess_p)
        {
            contentionQueueCount--;
            continue ;
        }
#endif
        /* - SPR 17733 */
        /*If CCE are available in UE specific space allocate from
         *UE specific only otherwise allocate from Common
         *search space
         */
        if (aggregationLevelForUespecific <= *availableCCE_p)
        {
            /*CA Changes start  */
            rntiInfo_p = getUeIdxFromRNTIMap(contentionQueueNode_p->tcRNTI, internalCellIndex);
            /*CA Changes end  */
            /* SPR#706 Changes Start */
            if ( PNULL == rntiInfo_p )
            {
                LOG_MAC_MSG(MAC_TCRNTI_RNTIINFO_NULL_ID,LOGERROR,MAC_RACH,\
                        getCurrentTick(),\
                        contentionQueueNode_p->tcRNTI,\
                        LINE_NUMBER,\
                        DEFAULT_INT_VALUE,\
                        DEFAULT_INT_VALUE,\
                        DEFAULT_INT_VALUE,\
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,\
                        FUNCTION_NAME,"TCRNTI_RNTIINFO_NULL");


            /* + SPR_17858_558_CHANGES */
                contentionQueueNode_p = 
                    (ContentionQueueNode *)popNode(contentionResolutionMsgQ_gp[internalCellIndex]);

                if (contentionQueueNode_p)
                {

                    msgFree(contentionQueueNode_p->msg_p);
                    contentionQueueNode_p->msg_p = PNULL; 
                    freeMemPool(contentionQueueNode_p);
                }
                /* Fix SPR 21492 start */ 
                ((ContentionTimerNode *)tempUEContext_p->timerNode_p)->\
                    deleteFlag = 255;
                /* Fix SPR 21492 end */ 
                contentionQueueCount--;
            /* - SPR_17858_558_CHANGES */
                continue ;
            }
            /* SPR#706 Changes End */

            /*Allocate CCE*/
            retVal = allocateCCEInUESpecificSpaceForContentionResMsg(
                    (rntiInfo_p->ueSearchSpaceInfo_p[internalCellIndex] + subFrameNum),
                    pdcchDLOutputInfo_p,
                    dciCCEInfo_p,
                    contentionQueueNode_p,
                    availableCCE_p,
                    aggregationLevelForUespecific,
                    dciCCEContainerInfo_p,
                    pdcchULOutputInfo_p
#ifdef FDD_CONFIG
                    /* + SPR 17733 */
                    ,rntiInfo_p
                    ,harqProcess_p
                    /* - SPR 17733 */
#endif                    
#ifdef TDD_CONFIG
                    ,
                    subFrameNum
#endif
                    ,internalCellIndex);
            if ( MAC_SUCCESS == retVal)
            {
                DEQUEUE_CONTENTION_RESOLUTION_SQ(ContentionQueueNode, contentionQueueNode_p, internalCellIndex);

                if (contentionQueueNode_p)
                {
#ifdef FDD_CONFIG
                    /* + SPR 17733 */
                    UInt8 numRB = contentionQueueNode_p->numRB;
                    UInt8 mcsIndex = contentionQueueNode_p->mcsIndex;
                    UInt32 tbSizeFound = 
                        determineTBSize(mcsIndex,numRB,TB_MAPPED_TO_ONE_LAYER);
                    FILL_HARQ_INFO_FOR_MSG4(tempUEContext_p, harqProcess_p, contentionQueueNode_p,
                            tbSizeFound,numRB,mcsIndex);
                    /* - SPR 17733 */
#endif
                    /* SPR 3628 chg */
#ifdef TDD_CONFIG
                    if(SSFFlag)
                    {
                        *availableDLRBCount_p -= contentionQueueNode_p->numRBForSSF;
                    }
                    else
                    {
                        *availableDLRBCount_p -= contentionQueueNode_p->numRB;
                    }
#elif FDD_CONFIG
                    *availableDLRBCount_p -= contentionQueueNode_p->numRB;
#endif
                    /* SPR 3628 chg */
                    FREE_MEM_NODE_CONTENTION_RESOLUTION_SQ(contentionQueueNode_p);
                }

                tempUEContext_p->status = CONTENTION_SENT;


                /* SPR 9881 changes start */
                /*Commented the code, because available CCE is already
                  subtracted in function allocateCCEInUESpecificSpaceForContentionResMsg()
                  according to aggregation level*/
                /*Decrement the number of CCE available*/
                /**availableCCE_p -= aggregationLevelForUespecific;*/
                /* SPR 9881 changes end */

                /* 5492 changes start */
                (*maxUeScheduledInDownlink_p)--;
                /* 5492 changes end */
                /*Decrement the number of DL RBs available*/
                contentionQueueCount--;
                continue;

            }
            else
            {
                LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                        "[%s]: WARNING: CCE allocation failed from "
                        "UE specific search space\n",
                        __func__);

                allocFromCommonSpace = TRUE;
            }
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                    "[%s]: WARNING: CCEs not available in UE specific space\n",
                    __func__);

            allocFromCommonSpace = TRUE;
        }

        /*Allocate the from common search space*/
        if ((TRUE == allocFromCommonSpace) &&
                (aggregationLevelForCommon <= *availableCommonCCE_p))
        {
            /*Allocate CCE*/
            retVal = allocateCCEInCommonSpaceForContentionResMsg(
                    pdcchDLOutputInfo_p,
                    dciCCEInfo_p,
                    contentionQueueNode_p,
                    availableCCE_p,
                    dciCCEContainerInfo_p,
                    pdcchULOutputInfo_p
                    /* SPR 21487 Fix Start(CSR 130474) */
#ifdef FDD_CONFIG
                    ,rntiInfo_p
                    ,harqProcess_p
#endif                    
                    /* SPR 21487 Fix End(CSR 130474) */
#ifdef TDD_CONFIG
                    ,
                    subFrameNum
#endif     
                    ,internalCellIndex);
            if ( MAC_SUCCESS == retVal )
            {
                DEQUEUE_CONTENTION_RESOLUTION_SQ(ContentionQueueNode , contentionQueueNode_p, internalCellIndex);

                if (contentionQueueNode_p)
                {
                    /*+ Klockwork warning removal*/ 
                    /*Decrement the number of DL RBs available*/
                    /* SPR 3628 chg */
#ifdef TDD_CONFIG
                    if(SSFFlag)
                    {
                        *availableDLRBCount_p -= contentionQueueNode_p->numRBForSSF;
                    }
                    else
                    {
                        *availableDLRBCount_p -= contentionQueueNode_p->numRB;
                    }
#elif FDD_CONFIG
                    *availableDLRBCount_p -= contentionQueueNode_p->numRB;
                    /* SPR 3628 chg */
                    /* + SPR 17733 */
                    UInt8 numRB = contentionQueueNode_p->numRB;
                    UInt8 mcsIndex = contentionQueueNode_p->mcsIndex;
                    UInt32 tbSizeFound = 
                        determineTBSize(mcsIndex,numRB,TB_MAPPED_TO_ONE_LAYER);

                    FILL_HARQ_INFO_FOR_MSG4( tempUEContext_p, harqProcess_p,
                            contentionQueueNode_p, tbSizeFound, numRB, 
                            mcsIndex );
                    /* - SPR 17733 */
#endif
                    FREE_MEM_NODE_CONTENTION_RESOLUTION_SQ(contentionQueueNode_p);
                }

                tempUEContext_p->status = CONTENTION_SENT;


                /* 5492 changes start */
                ( *maxUeScheduledInDownlink_p )--;
                /* 5492 changes end */
                /*Decrement the available CCE from Common space*/
                *availableCommonCCE_p -= aggregationLevelForCommon;
                contentionQueueCount--;
                continue;
            }
            else
            {
#ifdef FDD_CONFIG
		    /* SPR 20105 Changes Start */
		    freeDLHarqProcessForMsg4(harqProcess_p);
		    /* SPR 20105 Changes End */
#endif

                LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                        "[%s]: WARNING: allocateCCEInCommonSpaceForContentionResMsg()"
                        "failed\n",__func__,*availableCommonCCE_p,
                        aggregationLevelForCommon);

                break;
            }
        }
        else
        {
#ifdef FDD_CONFIG
		/* SPR 20105 Changes Start */
		freeDLHarqProcessForMsg4(harqProcess_p);
		/* SPR 20105 Changes End */
#endif

            LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                    "[%s]: WARNING: CCE in Common space [%d] for Contention message are"
                    "less than required CCE [%d]\n",
                    __func__,*availableCommonCCE_p,
                    aggregationLevelForCommon);

            break;
        }
    }

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);

    return; 
}

/*****************************************************************************
 * Function Name  : allocateCCEInCommonSpaceForRARMsg
 * Inputs         : raRspQnode_p - Pointer to the DLRARspQueueNode
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : pdcchDLOutputInfo_p - Pointer to the pdcch container
 *                  dciCCEInfo_p - Pointer to the DCICCE  Info
 *                  availableCCE_p - Available CCE
 *                  dciCCEContainerInfo_p - Pointer to DCI CCE Container info
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This will allocate the CCE in common space for RAR messages 
 *****************************************************************************/
MacRetType allocateCCEInCommonSpaceForRARMsg(
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        DciCCEInfo        *dciCCEInfo_p,
        DLRARspQueueNode  *raRspQnode_p,
        UInt32            *availableCCE_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    UInt32 cceAllocated = INVALID_CCE_INDEX;
    /*coverity-530 CID-32304*/
    MacRetType  macRetType   = MAC_FAILURE;
    /*coverity-530 CID-32304*/
    UInt32 count        = 0;
    DLRACHMsgInfo    *rarMsgInfo_p = PNULL;
    RAReqQueueNode *raQNode_p = PNULL;

    UInt8 relResCnt = 0;
    UInt8 numRAResponse = 0;
    DLRARsp *dlRARsp_p = PNULL;
    /* Rel 5.3.1: Memory related changes start*/
#ifdef PERF_STATS
    UInt32 index = 0;
    UInt16 taRangeValue = 0;
    /* + PERF_CA */
    LteCellMacULPerfStats *lteCellMacULPerfStats = &(gMacCellPerfStats_p[internalCellIndex]-> \
            lteCellMacULPerfStats);
    /* - PERF_CA */
#endif
    /* Rel 5.3.1: Memory related changes end*/
    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    LTE_MAC_UT_LOG(LOG_INFO ,RACH_MGR,
            "[%s]: Allocating CCE at Aggr Level = %d ",
            __func__,AGGREGATION_LEVEL_FOR_COMMON_SPACE);

    macRetType = isCCEAvailableAggrLvlFourCommonSpace(
            &cceAllocated,availableCCE_p,dciCCEInfo_p, internalCellIndex);

    if (MAC_SUCCESS == macRetType)
    {
        LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
                "[%s]: CCE allocated successfully cceAllocated [%d]\n",
                __func__,cceAllocated);

        count = pdcchDLOutputInfo_p->countCommonChannelMsg;

        /* ICIC changes start */
        pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].msgType = RAR_MSG;

        rarMsgInfo_p = 
            &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].dlMsgInfo.dlRACHMsgInfo;
        /* ICIC changes end */

        rarMsgInfo_p->dciFormat = raRspQnode_p->rarToDl.dciFormat;
        rarMsgInfo_p->dataLen   = (UInt8)raRspQnode_p->rarToDl.dataLen;
        rarMsgInfo_p->raRNTI    = raRspQnode_p->rarToDl.raRNTI;
        rarMsgInfo_p->data_p    = raRspQnode_p->rarToDl.macPduData_p;
        rarMsgInfo_p->resourceAllocInput.requiredRB = raRspQnode_p->rarToDl.numRB;
        rarMsgInfo_p->resourceAllocInput.preferredRATPolicy = 
            RA_POLICY_2_DISTRIBUTED;
        rarMsgInfo_p->cceIndex  = cceAllocated;
        rarMsgInfo_p->resourceAllocInput.mcsIndexForCommonChannel = 
            raRspQnode_p->rarToDl.mcsIndex;

        /* SPR_10231_FIX_START*/
        rarMsgInfo_p->tpc = raRspQnode_p->rarToDl.tpc;
        /* SPR_10231_FIX_END*/

        dciCCEInfo_p[cceAllocated].dciPduInfo.aggregationLvl = 
            AGGREGATION_LEVEL_FOR_COMMON_SPACE;
        dciCCEInfo_p[cceAllocated].dciPduInfo.cceIndex       = cceAllocated;
        dciCCEInfo_p[cceAllocated].dciPduInfo.rnti = raRspQnode_p->rarToDl.raRNTI;

        dciCCEInfo_p[cceAllocated].dciPduInfo.harqProcessId = INVALID_HARQ_ID;

        /* CLPC_CHG */
        dciCCEInfo_p[cceAllocated].dciPduInfo.txPower = 
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->commonDLPowerControlInfo.pdcchPowerOffset;
        /* CLPC_CHG END */

        /* SPR 17112 Fix Start */
        /* SPR 1685 Fix start */
        dciCCEInfo_p[cceAllocated].dciPduInfo.ueCategory = MAC_MIN_UE_CATEGORY;
        /* SPR 1685 Fix end */
        /* SPR 17112 Fix End */

        /* SPR 1488 + SPR 1589 + SPR 1686 Starts */
        /*CA Changes start  */
        if ( GAP_VALUE_ONE == cellSpecificParams_g.\
                cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->gapValue)
            /*CA Changes end  */
        {
            dciCCEInfo_p[cceAllocated].dciPduInfo.tb1Info.txIndicator = 0;
        }
        else
        {
            dciCCEInfo_p[cceAllocated].dciPduInfo.tb1Info.txIndicator = 1;
        }
        /* SPR 1488 + SPR 1589 + SPR 1686 Ends	*/

        pdcchDLOutputInfo_p->countCommonChannelMsg++;
        dciCCEContainerInfo_p->arrayCCEAllocated[dciCCEContainerInfo_p->countOfCCEAllocated]
            = cceAllocated;
        dciCCEContainerInfo_p->countOfCCEAllocated++;

        dlRARsp_p = &raRspQnode_p->rarToDl;

        numRAResponse = dlRARsp_p->numRAR;

        while (relResCnt < numRAResponse)
        {
            raQNode_p = 
                dlRARsp_p->resourceInfo[relResCnt].raReqQNode_p;

            /* Rel 5.3.1: Coverity Fix 58281 Start */    
            if(PNULL != raQNode_p)
            {
                /* Rel 5.3.1: Coverity Fix 58281 End */    
                /* Rel 5.3.1: Memory related changes start*/
#ifdef PERF_STATS
                /*EVENT:EVENT_PARAM_RA_SUCC_TA_0..11, number of detected successful 
                 * random access with a TA value in the range*/
                taRangeValue = raQNode_p->raPreambleInfo.timingAdvance;

                /*tagRangeValue is right shifted to find the index where TA range lies*/
                index = taRangeValue >> RSHIFT_NUM;

                if (index < MAX_TA_INDEX_VAL)
                {
                    if (index > MAX_TA_ACT_INDEX_VAL)
                    {
                        index = MAX_TA_ACT_INDEX_VAL;
                    }
                    lteCellMacULPerfStats->totalSuccRandomAccessTA[index]++;
                }
#endif
                /* Rel 5.3.1: Memory related changes end*/

                LOG_UT(MAC_INFO_IN_RAR,
                        LOGDEBUG,
                        MAC_RACH,
                        getCurrentTick(),
                        raQNode_p->raPreambleInfo.preamble,
                        raQNode_p->raRNTI,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"Info in RAR for each Rach Req");
                freeMemPool(raQNode_p);
            }
            relResCnt++;

        }
        LOG_MAC_MSG(MAC_RAR_DCI_ALLOC_SUCC_ID,LOGDEBUG,MAC_RACH,\
                getCurrentTick(),\
                raRspQnode_p->rarToDl.raRNTI,\
                raRspQnode_p->rarToDl.numRB,\
                raRspQnode_p->rarToDl.mcsIndex,\
                raRspQnode_p->rarToDl.exptti,\
                raRspQnode_p->rarToDl.tti,\
                DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,\
                __func__,"RAR_DCI_ALLOC_SUCC");

    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
                "[%s]: ERROR CCE allocation failed aggregationLevel [%d]\n",
                __func__,AGGREGATION_LEVEL_FOR_COMMON_SPACE);

        LOG_MAC_MSG(MAC_RAR_DCI_ALLOC_FAIL_ID,LOGDEBUG,MAC_RACH,\
                getCurrentTick(),\
                raRspQnode_p->rarToDl.raRNTI,\
                raRspQnode_p->rarToDl.numRB,\
                raRspQnode_p->rarToDl.mcsIndex,\
                raRspQnode_p->rarToDl.exptti,\
                raRspQnode_p->rarToDl.tti,\
                (UDouble32)cceAllocated,\
                (UDouble32)raRspQnode_p->rarToDl.dciFormat,\
                __func__,"RAR_DCI_ALLOC_FAIL");

    }

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);

    return macRetType;
}

/*****************************************************************************
 * Function Name  : allocateCCEInUESpecificSpaceForContentionResMsg
 * Inputs         : ueSearchSpaceInfo_p - Pointer to the UE specific search space 
 *                  info
 *                  contentionQueueNode_p - Pointer to ContentionQueueNode
 *                  aggregationLevel - aggregation level
 *                  dciCCEContainerInfo_p -
 *                  pdcchULOutputInfo_p - 
 *                  subFrame (TDD only) -
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : pdcchDLOutputInfo_p - Pointer to the pdcch container
 *                  dciCCEInfo_p - Pointer to the DCICCE container
 *                  availableCCE_p - Available CCE
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : This will allocate the CCE in UE specific space for 
 *                  contention resolutin messages
 *****************************************************************************/
MacRetType allocateCCEInUESpecificSpaceForContentionResMsg(
        UESpecificSearchSpaceInfo  *ueSearchSpaceInfo_p,
        PdcchDLOutputInfo   *pdcchDLOutputInfo_p,
        DciCCEInfo          *dciCCEInfo_p,
        ContentionQueueNode *contentionQueueNode_p,
        UInt32 *availableCCE_p,
        UInt32 aggregationLevel,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        PdcchULOutputInfo *pdcchULOutputInfo_p
#ifdef FDD_CONFIG
        /* + SPR 17733 */
        ,RNTIInfo *rntiInfo_p,
        DLHARQProcess *harqProcess_p
        /* - SPR 17733 */
#endif
#ifdef TDD_CONFIG
        ,UInt8 subFrame
#endif
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    UInt32     count = 0;
    MacRetType macRetType       = MAC_FAILURE;
    UInt32     cceAllocated     = INVALID_CCE_INDEX;
    DLContentionReslMsgInfo *contReslMsgInfo_p = PNULL;
#ifdef FDD_CONFIG    
    TcrntiCCEIndexInfo *tcrntiCCEIndexInfo_p = PNULL;
    /* SPR 4301 chg */
#elif TDD_CONFIG 
    TcrntiCCEIndexInfo *tcrntiCCEIndexInfoTDD_p = PNULL;
#endif
    /* SPR 4301 chg */
    UInt8 contentionUECount = 0;
    LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
            "Entered in function = %s \n",__func__);
    /*pdcch allocation for scell changes start*/

    /*pdcch allocation for scell changes end*/
    switch(aggregationLevel)
    {
        case AGGREGATION_LEVEL_ONE:
        {
            /*Comes here when the Aggregagation Level of dciFormat is One*/
            LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
                    "Trying to Allocate CCE at Aggr Level = %d",
                    aggregationLevel);   

            /*Calling funtion isCCEAvailableAggrLvlOneUESpace which will
             *check if the CCEs are free at aggregation level one.If free
             *it will return Success else Failure
             */
            /*START : DYN_PDCCH*/
            macRetType =  isCCEAvailableAggrLvlOneUESpace(
                    ueSearchSpaceInfo_p->cceAggrLvlOneInfo[currentCFIValue_g[internalCellIndex]-1],//DYN_PDCCH
                    &cceAllocated,availableCCE_p,dciCCEInfo_p);
            /*END   : DYN_PDCCH*/
        }
        break;
        case AGGREGATION_LEVEL_TWO:
        {
            /*Comes here when the Aggregagation Level of dciFormat is Two*/
            LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
                    "Trying to Allocate CCE at Aggr Level = %d",
                    aggregationLevel);

            /*Calling funtion isCCEAvailableAggrLvlTwoUESpace which will
             *check if the CCEs are free at aggregation level Two.If free
             *it will return Success else Failure
             */
            /*START : DYN_PDCCH*/
            macRetType = isCCEAvailableAggrLvlTwoUESpace(
                    ueSearchSpaceInfo_p->cceAggrLvlTwoInfo[currentCFIValue_g[internalCellIndex]-1],//DYN_PDCCH
                    &cceAllocated,availableCCE_p,dciCCEInfo_p);
            /*END   : DYN_PDCCH*/
        }
        break;
        case AGGREGATION_LEVEL_FOUR:
        {
            /*Comes here when the Aggregagation Level of dciFormat is Four*/
            LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
                    "Trying to Allocate CCE at Aggr Level = %d",
                    aggregationLevel);

            /*Calling funtion isCCEAvailableAggrLvlFourUESpace which will
             *check if the CCEs are free at aggregation level Four.If free
             *it will return Success else Failure
             */
            /*START : DYN_PDCCH*/
            macRetType = isCCEAvailableAggrLvlFourUESpace(
                    ueSearchSpaceInfo_p->cceAggrLvlFourInfo[currentCFIValue_g[internalCellIndex]-1],//DYN_PDCCH
                    &cceAllocated,availableCCE_p,dciCCEInfo_p);
            /*END   : DYN_PDCCH*/
        }
        break;
        case AGGREGATION_LEVEL_EIGHT:
        {   
            /*Comes here when the Aggregagation Level of dciFormat is Eight*/
            LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
                    "Trying to Allocate CCE at Aggr Level = %d",
                    aggregationLevel);

            /*Calling funtion isCCEAvailableAggrLvlEightUESpace which will
             *check if the CCEs are free at aggregation level Eight.If free
             *it will return Success else Failure
             */
            /*START : DYN_PDCCH*/
            macRetType = isCCEAvailableAggrLvlEightUESpace(
                    ueSearchSpaceInfo_p->cceAggrLvlEightInfo[currentCFIValue_g[internalCellIndex]-1],//DYN_PDCCH
                    &cceAllocated,availableCCE_p,dciCCEInfo_p);
            /*END   : DYN_PDCCH*/
        }
        break;
        default:
        {
            LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                    "InValid Aggrgation Level\n");
        }
    }/*End of Switch Statement */

    if (MAC_SUCCESS == macRetType)
    {
        LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
                "[%s]: CCE allocated successfully cceAllocated [%d]\n",
                __func__,cceAllocated);
        /*DYN_PDCCH_TDD_FIX*/
        changeCFIInThisTTI_g[internalCellIndex] = TRUE;
        /*DYN_PDCCH_TDD_FIX*/
        count = pdcchDLOutputInfo_p->countCommonChannelMsg;

        /* ICIC changes start */
        contReslMsgInfo_p = 
            &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].dlMsgInfo.\
            dlContentionReslMsgInfo;
        /* ICIC changes end */

        contReslMsgInfo_p->resourceAllocInput.preferredRATPolicy = 
            RA_POLICY_2_LOCALIZED;
        contReslMsgInfo_p->cceIndex   = cceAllocated;
        contReslMsgInfo_p->resourceAllocInput.requiredRB = 
            contentionQueueNode_p->numRB;
        contReslMsgInfo_p->resourceAllocInput.mcsIndexForCommonChannel = 
            contentionQueueNode_p->mcsIndex;
        dciCCEInfo_p[cceAllocated].dciPduInfo.aggregationLvl = aggregationLevel;
        dciCCEInfo_p[cceAllocated].dciPduInfo.cceIndex       = cceAllocated;
        dciCCEInfo_p[cceAllocated].dciPduInfo.rnti           = 
            contentionQueueNode_p->tcRNTI;
#ifdef TDD_CONFIG
        dciCCEInfo_p[cceAllocated].dciPduInfo.harqProcessId = INVALID_HARQ_ID;
#elif FDD_CONFIG
        /* + SPR 17733 */
        dciCCEInfo_p[cceAllocated].dciPduInfo.harqProcessId = harqProcess_p->harqProcessId;
        dciCCEInfo_p[cceAllocated].dciPduInfo.ueIndex = rntiInfo_p->index;
        /* - SPR 17733 */
#endif
        dciCCEInfo_p[cceAllocated].dciPduInfo.tb1Info.txIndicator = NEW_TX;
        /* CLPC_CHG */
        dciCCEInfo_p[cceAllocated].dciPduInfo.txPower =  
            /*CA Changes start  */
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->commonDLPowerControlInfo.pdcchPowerOffset;
        /*CA Changes end  */
        dciCCEInfo_p[cceAllocated].dciPduInfo.pA = DEFAULT_PA_VALUE;
        /* CLPC_CHG END */

        contReslMsgInfo_p->dciFormat = contentionQueueNode_p->dciFormat;
        contReslMsgInfo_p->resourceAllocInput.requiredRB = 
            contentionQueueNode_p->numRB; 
        contReslMsgInfo_p->data_p = contentionQueueNode_p->msg_p;
        contReslMsgInfo_p->dataLen = contentionQueueNode_p->msgLen;
        /* ICIC changes start */
        pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].msgType = 
            CONTENTION_RESL_MSG;
        /* ICIC changes end */

        pdcchDLOutputInfo_p->countCommonChannelMsg++;
        /* 5492 changes start */
        /* 5492 changes end */
        /* SPR 3628 chg */
#ifdef TDD_CONFIG
        if(checkForSpecialSubframe(subFrame, internalCellIndex))
        {
            contReslMsgInfo_p->resourceAllocInput.requiredRB = 
                contentionQueueNode_p->numRBForSSF;
            contReslMsgInfo_p->resourceAllocInput.mcsIndexForCommonChannel = 
                contentionQueueNode_p->mcsIndexForSSF;
            contReslMsgInfo_p->dataLen = contentionQueueNode_p->msgLenForSSF;
        }
#endif
        /* SPR 3628 chg */

        dciCCEContainerInfo_p->arrayCCEAllocated[dciCCEContainerInfo_p->\
            countOfCCEAllocated] = cceAllocated;
        dciCCEContainerInfo_p->countOfCCEAllocated++;

        /* SPR 4301 chg */ 
#ifdef FDD_CONFIG    
        tcrntiCCEIndexInfo_p = 
            pdcchULOutputInfo_p->pdcchTcrntiUEInfo.pdcchTempUEArray; 

        contentionUECount = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count;
        tcrntiCCEIndexInfo_p[contentionUECount].cceIndex = cceAllocated;
        tcrntiCCEIndexInfo_p[contentionUECount].tcrnti = 
            contentionQueueNode_p->tcRNTI;
        pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count++;
#elif TDD_CONFIG
        tcrntiCCEIndexInfoTDD_p = 
            pdcchULOutputInfo_p->pdcchTcrntiUEInfo. pdcchULUEArrayForTempUlGroup; 
        contentionUECount = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.cumulativeUeCountForTempUlGroup;
        tcrntiCCEIndexInfoTDD_p[contentionUECount].dlSubFrame = subFrame;
        tcrntiCCEIndexInfoTDD_p[contentionUECount].cceIndex = cceAllocated;
        tcrntiCCEIndexInfoTDD_p[contentionUECount].tcrnti = 
            contentionQueueNode_p->tcRNTI;
        pdcchULOutputInfo_p->pdcchTcrntiUEInfo.cumulativeUeCountForTempUlGroup++;
#endif
        /* SPR 4301 chg */
    }

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);

    return macRetType;
}

/*****************************************************************************
 * Function Name  : allocateCCEInCommonSpaceForContentionResMsg
 * Inputs         : contentionQueueNode_p - Pointer to ContentionQueueNode
 *                  dciCCEContainerInfo_p -
 *                  pdcchULOutputInfo_p - 
 *                  subFrame (TDD only) -
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : pdcchDLOutputInfo_p - Pointer to the pdcch container
 *                  dciCCEInfo_p - Pointer to the DCICCE container
 *                  availableCCE_p - Available CCE
 * Returns        : MAC_SUCESS/MAC_FAILURE
 * Description    : This will allocate the CCE from common search space
 *****************************************************************************/
MacRetType allocateCCEInCommonSpaceForContentionResMsg(
        PdcchDLOutputInfo   *pdcchDLOutputInfo_p,
        DciCCEInfo          *dciCCEInfo_p,
        ContentionQueueNode *contentionQueueNode_p,
        UInt32 *availableCCE_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        PdcchULOutputInfo *pdcchULOutputInfo_p
        /* SPR 21487 Fix Start(CSR 130474) */
#ifdef FDD_CONFIG
        ,RNTIInfo *rntiInfo_p,
        DLHARQProcess *harqProcess_p
#endif
        /* SPR 21487 Fix End(CSR 130474) */
#ifdef TDD_CONFIG
        ,
        UInt8 subFrame
#endif                                     
        , InternalCellIndex internalCellIndex)
{
    UInt32 cceAllocated = INVALID_CCE_INDEX;

    /*coverity-530 CID-32304*/
    MacRetType macRetType   = MAC_FAILURE;
    /*coverity-530 CID-32304*/
    UInt32 count        = 0;
    DLContentionReslMsgInfo *contReslMsgInfo_p = PNULL;
#ifdef FDD_CONFIG    
    TcrntiCCEIndexInfo *tcrntiCCEIndexInfo_p = PNULL;
#endif
    UInt8 contentionUECount = 0;
    /* SPR 4301 chg */
#ifdef TDD_CONFIG 
    TcrntiCCEIndexInfo *tcrntiCCEIndexInfoTDD_p = PNULL;
#endif
    /* SPR 4301 chg */

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    LTE_MAC_UT_LOG(LOG_INFO ,RACH_MGR,
            "[%s]: Allocating CCE at Aggr Level = %d ",
            __func__, AGGREGATION_LEVEL_FOR_COMMON_SPACE);

    macRetType = isCCEAvailableAggrLvlFourCommonSpace(
            &cceAllocated,availableCCE_p,dciCCEInfo_p, internalCellIndex);

    if (MAC_SUCCESS == macRetType)
    {
        LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
                "[%s]: CCE allocated successfully cceAllocated [%d]\n",
                __func__,cceAllocated);

        count = pdcchDLOutputInfo_p->countCommonChannelMsg;
        /* ICIC changes start */
        contReslMsgInfo_p = &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].\
                            dlMsgInfo.dlContentionReslMsgInfo;
        /* ICIC changes  end */
        contReslMsgInfo_p->resourceAllocInput.preferredRATPolicy = 
            RA_POLICY_2_LOCALIZED;

        dciCCEInfo_p[cceAllocated].dciPduInfo.aggregationLvl = \
                                                               AGGREGATION_LEVEL_FOR_COMMON_SPACE;
        dciCCEInfo_p[cceAllocated].dciPduInfo.cceIndex       = cceAllocated;
        dciCCEInfo_p[cceAllocated].dciPduInfo.rnti           = 
            contentionQueueNode_p->tcRNTI;
        /* SPR 21487 Fix Start(CSR 130474) */
#ifdef TDD_CONFIG
        dciCCEInfo_p[cceAllocated].dciPduInfo.harqProcessId = INVALID_HARQ_ID;
#elif FDD_CONFIG
        dciCCEInfo_p[cceAllocated].dciPduInfo.harqProcessId = harqProcess_p->harqProcessId;
        dciCCEInfo_p[cceAllocated].dciPduInfo.ueIndex = rntiInfo_p->index;
#endif
        /* SPR 21487 Fix End(CSR 130474) */
        dciCCEInfo_p[cceAllocated].dciPduInfo.tb1Info.txIndicator = NEW_TX;
        /* CLPC_CHG */
        dciCCEInfo_p[cceAllocated].dciPduInfo.txPower = 
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            cellParams_p->commonDLPowerControlInfo.pdcchPowerOffset;
        /* CLPC_CHG END */

        /* ICIC changes start */
        pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].msgType = CONTENTION_RESL_MSG;
        /* ICIC changes end */

        contReslMsgInfo_p->cceIndex  = cceAllocated;
        contReslMsgInfo_p->dciFormat = contentionQueueNode_p->dciFormat;
        contReslMsgInfo_p->resourceAllocInput.requiredRB = 
            contentionQueueNode_p->numRB; 
        contReslMsgInfo_p->resourceAllocInput.mcsIndexForCommonChannel= 
            contentionQueueNode_p->mcsIndex;
        contReslMsgInfo_p->data_p = contentionQueueNode_p->msg_p;
        contReslMsgInfo_p->dataLen = contentionQueueNode_p->msgLen;
        dciCCEContainerInfo_p->arrayCCEAllocated[dciCCEContainerInfo_p->countOfCCEAllocated] =
            cceAllocated;

        /* SPR 3628 chg */
#ifdef TDD_CONFIG
        if(checkForSpecialSubframe(subFrame, internalCellIndex))
        {
            contReslMsgInfo_p->resourceAllocInput.requiredRB = 
                contentionQueueNode_p->numRBForSSF;
            contReslMsgInfo_p->resourceAllocInput.mcsIndexForCommonChannel = 
                contentionQueueNode_p->mcsIndexForSSF;
            contReslMsgInfo_p->dataLen = contentionQueueNode_p->msgLenForSSF;
        }
#endif
        /* SPR 3628 chg */

        dciCCEContainerInfo_p->countOfCCEAllocated++;    
        pdcchDLOutputInfo_p->countCommonChannelMsg++;
        /* 5492 changes start */
        /* 5492 changes end */

        /* SPR 4301 chg */ 
#ifdef FDD_CONFIG    
        tcrntiCCEIndexInfo_p = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.pdcchTempUEArray; 

        contentionUECount = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count;
        tcrntiCCEIndexInfo_p[contentionUECount].cceIndex = cceAllocated;
        tcrntiCCEIndexInfo_p[contentionUECount].tcrnti = contentionQueueNode_p->tcRNTI;
        pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count++;
#elif TDD_CONFIG
        tcrntiCCEIndexInfoTDD_p = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.pdcchULUEArrayForTempUlGroup; 
        contentionUECount = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.cumulativeUeCountForTempUlGroup;
        tcrntiCCEIndexInfoTDD_p[contentionUECount].dlSubFrame = subFrame;
        tcrntiCCEIndexInfoTDD_p[contentionUECount].cceIndex = cceAllocated;
        tcrntiCCEIndexInfoTDD_p[contentionUECount].tcrnti = contentionQueueNode_p->tcRNTI;
        pdcchULOutputInfo_p->pdcchTcrntiUEInfo.cumulativeUeCountForTempUlGroup++;
#endif
        /* SPR 4301 chg */
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
                "[%s]: ERROR CCE allocation failed aggregationLevel [%d]\n",
                __func__,AGGREGATION_LEVEL_FOR_COMMON_SPACE);
    }

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);

    return macRetType;
}

/*****************************************************************************
 * Function Name  : updateTimerExpirySubFrame
 * Inputs         : timerExpirySubFrameNum
 *                  ulHarqSysFrameNum  
 * Outputs        : None 
 * Returns        : None
 * Description    : This function updates the sub frame and UL Harq system frame number
 *****************************************************************************/
STATIC  void updateTimerExpirySubFrame(UInt32* timerExpirySubFrameNum, UInt32* ulHarqSysFrameNum)
{
    if(*timerExpirySubFrameNum >= MAX_SUB_FRAME)
    {
        (*ulHarqSysFrameNum) += (*timerExpirySubFrameNum)/MAX_SUB_FRAME;
        *timerExpirySubFrameNum = *timerExpirySubFrameNum % MAX_SUB_FRAME;
        if (*ulHarqSysFrameNum >= MAX_SFN_VALUE)
        {
            *ulHarqSysFrameNum = 0;
        }
    }
}

/*****************************************************************************
 * Function Name  : addBackoffIndicatorSubheader
 * Inputs         : BIFlag
 *                  numRAR
 *                  subHeader_p 
 * Outputs        : None 
 * Returns        : None
 * Description    : This function updates the BacK of indicater in the sub header
 *****************************************************************************/
STATIC  void addBackoffIndicatorSubheader(UInt8 BIFlag, UInt8 numRAR, UInt8* subHeader_p)
{
    if (BIFlag)
    {
        if (!numRAR)
        {
            /*This is the last subheader*/
            *subHeader_p = ((BACKOFF_INDEX)&(0x0F));
        }
        else
        {
            /*This is not the last subheader*/
            *subHeader_p = ((BACKOFF_INDEX)|(0x80));
        }
        subHeader_p++;
    }
}   

/*****************************************************************************
 * Function Name  : checkAperiodicCQITobeTrigger
 * Inputs         : cqiTypeIndicator
 *                  aperiodicCQITobeTriggerWithCFR
 *                  isCqiWithCFRPresent
 *                  ueIndex
 *                  tempUeIndex
 * Outputs        : None 
 * Returns        : None
 * Description    : This function updates the trigger if CQI Aperioidic in ON
 *****************************************************************************/
STATIC  void checkAperiodicCQITobeTrigger(UInt8 cqiTypeIndicator,
        UInt8* aperiodicCQITobeTriggerWithCFR,UInt8* isCqiWithCFRPresent,
        UInt32* ueIndex, UInt32 tempUeIndex)
{             
    if((APERIODIC_CQI_REPORTING_MODE == cqiTypeIndicator) ||
            (PERIODIC_APERIODIC_MODE == cqiTypeIndicator))
    {
        *aperiodicCQITobeTriggerWithCFR = 1;
        *isCqiWithCFRPresent = TRUE;
        *ueIndex = tempUeIndex;
    }      
}

/*****************************************************************************
 * Function Name  : fillRaRspQueueNode
 * Inputs         : count
 *                  tempCRNTICtx_p
 *                  raRspQueueNode_p
 *                  resourceInfo
 *                  rachConfig_p
 *                  preambleIndex
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None 
 * Returns        : None
 * Description    : This function fills the RA Response Queue node 
 *****************************************************************************/
STATIC  void fillRaRspQueueNode(UInt32 count,TempCRNTICtx* tempCRNTICtx_p,
        DLRARspQueueNode *raRspQueueNode_p,
        RARspResourceInfo *resourceInfo,
        RACHConfiguration *rachConfig_p,
        UInt8* preambleIndex,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    RAReqQueueNode    *raQNode_p      = PNULL;
    UInt32 i = 0;
    for (i = 0; i < count; i++)
    {
        tempCRNTICtx_p = PNULL;

        raRspQueueNode_p->rarToDl.resourceInfo[i].riLength = 
            resourceInfo[i].riLength;

        raRspQueueNode_p->rarToDl.resourceInfo[i].riStart = 
            resourceInfo[i].riStart;

        raRspQueueNode_p->rarToDl.resourceInfo[i].tcRNTI  = 
            resourceInfo[i].tcRNTI;

        raRspQueueNode_p->rarToDl.resourceInfo[i].raReqQNode_p = 
            resourceInfo[i].raReqQNode_p;

        raRspQueueNode_p->rarToDl.resourceInfo[i].ulHarq = 
            resourceInfo[i].ulHarq;

        raRspQueueNode_p->rarToDl.resourceInfo[i].harqProcessId =
            resourceInfo[i].harqProcessId;

        /* SPR 4483 changes start */
        raRspQueueNode_p->rarToDl.resourceInfo[i].isCqiWithCFRPresent =
            resourceInfo[i].isCqiWithCFRPresent;
        /* SPR 4483 changes end */

        /* + TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
        raRspQueueNode_p->rarToDl.resourceInfo[i].ueIndex =
            resourceInfo[i].ueIndex;
        /* - TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */

        raQNode_p = resourceInfo[i].raReqQNode_p;
        *preambleIndex = raQNode_p->raPreambleInfo.preamble;

        if(rachConfig_p->countOfContentionBasedRAPreambles > *preambleIndex)
        { 
            /*CA Changes start  */
            tempCRNTICtx_p = getTempCRNTIContextPointer(
                    resourceInfo[i].tcRNTI, internalCellIndex);
            /*CA Changes end  */ 
            if (tempCRNTICtx_p)
            {
                tempCRNTICtx_p->status = RAR_SENT;
            }
        }
        LOG_MAC_MSG(RA_REQ_RB_ALLOC_ID, LOGDEBUG, MAC_RACH,
                getCurrentTick(), raQNode_p->raRNTI,
                raQNode_p->raPreambleInfo.preamble,
                resourceInfo[i].tcRNTI,
                resourceInfo[i].riStart,
                resourceInfo[i].riLength,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,
                "RA_REQ_UE_STATUS_UPDATE");
    }
}

/*****************************************************************************
 * Function Name  : prepareAndSendRARToDlScheduler
 * Inputs         : rbMapNum - TTI in which UL RBs to allocate
 *                  dlDelay - DL delay
 *                  currentGlobalTTI - Current global TTI
 *                  rarPduTobeScheduled_p - pointer to the RAR PDU
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *                  creates the TempCRNTI context and constructs the RAR PDU by 
 *                  allocating the UL resources
 *****************************************************************************/
void prepareAndSendRARToDlScheduler(UInt8 rbMapNum,
        UInt32 dlDelay,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTI,
        /* SPR 15909 fix end */
        RAReqTobeScheduledInPdu* rarPduTobeScheduled_p,
        InternalCellIndex internalCellIndex,
        ExecutionLegFunctionArg* execArgs)
{
    UInt32 buffLen = 0;
    UInt8  numRB   = 0;
    UInt8 count   = 0;
    /* SPR 13250 fix start */
    UInt16  tbSize  = 0;
    /* SPR 13250 fix end */
    UInt8  mcsIndex = 0;
    UInt16 tcRnti  = INVALID_RNTI;
    UInt8  *msg_p         = PNULL;
    UInt8  *subHeader_p   = PNULL;
    UInt8  *payLoad_p     = PNULL;
    UInt32  hdrLen = 0; 
    TempCRNTICtx      *tempCRNTICtx_p = PNULL;
    RAReqQueueNode    *raQNode_p      = PNULL;
    RARspResourceInfo resourceInfo[MAX_RAR_PER_PDU];
    AllocatedResInfo  tempAllocatedResInfo = {0};
    DLRARspQueueNode *raRspQueueNode_p = PNULL;
    ULUEContext *ueULContext_p = PNULL;
    DLUEContext *ueDLContext_p  = PNULL;
    /* SPR 15487 fix start */
    UInt32  flag = 0; 
    /* SPR 15487 fix end */
/* + TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
#ifdef FDD_CONFIG
    UInt8 ttiMapId1 = TTI_MAP_FREE;
    UInt8 ttiMapId2= TTI_MAP_FREE;
    UInt8 retValue = MAC_FAILURE;
#endif
/* - TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
    void  *msg_struct_p         = PNULL;
    /* SPR  3444 changes start */
    QSEGMENT segment = {0};
    /* SPR  3444 changes end */
#ifdef TDD_CONFIG    
    UInt8 subFrameForRAR = 
        ((currentGlobalTTI + dlDelay + PHY_DL_DELAY) % MAX_SUB_FRAME);
    /* SPR 5798 Start */
    /* SPR 15909 fix start */
    tickType_t msg3TTI = currentGlobalTTI + dlDelay + PHY_DL_DELAY + getULSfForMsg3(subFrameForRAR, internalCellIndex);
    /* SPR 15909 fix end */
    UInt8 msg3CountContainer = (msg3TTI)%MAX_NUMBER_OF_CONTAINER;
    /* SPR 5798 End */
    /* SPR 8416 changes start */
    UInt16 sysFrameNumForRAR = 
        ((currentGlobalTTI + dlDelay + PHY_DL_DELAY) / MAX_SUB_FRAME) % 
        MAX_SFN_VALUE;
    UInt16 sysFrameNumForMsg3 = 0;
    /* TDD Config 0,6 Changes Start */
    UInt16 dci0SysFrameNumForMsg3 = 0;
    UInt8 dci0SubFrameForMsg3 = MAX_SUB_FRAME;
    UInt8  phichSubFrameNum = 0;
    UInt8 iPhich                                = 0;
    UInt8 nDmrs      = MAX_DMRS_VALUE;
    /* SPR 11257 Changes Start*/
    /*SPR 15909 fix start*/
    tickType_t dci0Tick = currentGlobalTTI + dlDelay + PHY_DL_DELAY;
    /*SPR 15909 fix end*/
    dci0Tick = dci0Tick + getTickForPUCCHCalForMsg3(dci0Tick % MAX_SUB_FRAME,internalCellIndex);
    /* SPR 11257 Changes End*/
    /* TDD Config 0,6 Changes End */
    /* SPR 8416 changes end */
#endif    


    UInt8 preambleIndex = 0;
    UInt8 numRAR = rarPduTobeScheduled_p->noOfRAResponses;
    UInt8 BIFlag = rarPduTobeScheduled_p->BIFlag;
#ifdef FDD_CONFIG
    UInt8 tempHarqProcessId = INVALID_HARQ_ID; 
#elif TDD_CONFIG
    UInt8 tempHarqProcessId = INVALID_HARQ_ID; 
    /*L2_Fix_start*/
    UInt8 subFrameForMsg3 = 0;
    /*SPR 3114 End*/
    /* SPR 3114 Start*/
    dci0SubFrameForMsg3 = getTickForPUCCHCalForMsg3(subFrameForRAR,internalCellIndex) + subFrameForRAR;

    /* TDD Config 0,6 Changes Start */
    if (MAX_SUB_FRAME <= dci0SubFrameForMsg3)
    {    
        dci0SubFrameForMsg3 %= MAX_SUB_FRAME;
        dci0SysFrameNumForMsg3 = (sysFrameNumForRAR + 1) % MAX_SFN_VALUE;
    }
    else
    {
        dci0SysFrameNumForMsg3 = sysFrameNumForRAR;
    } 

    /*
     ** Calculate Msg3 system frame Number from
     ** dci0SysFrameNumForMsg3.
     */
    subFrameForMsg3 =
        (subFrameForRAR + getULSfForMsg3(subFrameForRAR,internalCellIndex)) % MAX_SUB_FRAME;
    if( dci0SubFrameForMsg3 > subFrameForMsg3 )
    {
        sysFrameNumForMsg3 = (dci0SysFrameNumForMsg3 + 1) % MAX_SFN_VALUE; 
    }
    else
    {
        sysFrameNumForMsg3 = dci0SysFrameNumForMsg3;
    }
    phichSubFrameNum = ( subFrameForMsg3 + getHiPduSubframeNum(subFrameForMsg3,internalCellIndex) )
        % MAX_SUB_FRAME;
    iPhich = calculateIPhichValue(subFrameForMsg3,internalCellIndex);
#ifdef MAC_AUT_TEST
    testPrepareAndSendRAR_g.subFrameForRAR = subFrameForRAR;
    testPrepareAndSendRAR_g.dci0SubFrameForMsg3 = dci0SubFrameForMsg3 ;
    testPrepareAndSendRAR_g.subFrameForMsg3 = subFrameForMsg3;
    testPrepareAndSendRAR_g.sysFrameNumForRAR = sysFrameNumForRAR;
    testPrepareAndSendRAR_g.dci0SysFrameNumForMsg3 = dci0SysFrameNumForMsg3;
    testPrepareAndSendRAR_g.sysFrameNumForMsg3 = sysFrameNumForMsg3;
#endif
    /* TDD Config 0,6 Changes End */
#endif

    UInt16 raRNTI = rarPduTobeScheduled_p->raRnti;
    UInt16 ueIndex = INVALID_UE_ID;
    /* SPR 15909 fix start */
    tickType_t expTTI = rarPduTobeScheduled_p->exptti;
    /* SPR 15909 fix end */

    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
    UInt32 contBasedPreambleCount = 0;
    UInt32 contFreePreambleCount = 0;
#endif
    /* - PERF_STATS_PH1D1 */

    RAReqTobeScheduled *raReqToscheduled_p = 
        &rarPduTobeScheduled_p->raReqTobeScheduled[0];

    /*CA Changes start  */
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    RACHConfiguration *rachConfig_p = 
        &(cellParams_p->rachConfigInfo.rachConfiguration);
    /*CA Changes end  */

    /* +- SPR 17777 */
    ULHarqInfo*   tempULHarqInfo_p = PNULL; 
    ULUEContextInfo *ulUEContextInfo_p = PNULL;
    ContentionFreePreamble *contFreePreamble_p = PNULL;

    UInt8  numRBAllocated = 0;
    UInt8  mcsIndexForMsg3 = 0;

    /* ICIC changes start */
    /*CA Changes start  */
    RbMapNode *rbMapNode_p = rbMapNodeArr_gp[internalCellIndex] + rbMapNum;
    /*CA Changes end  */
    /* ICIC changes end */
    /* + CQI_5.0 */
    UInt8  aperiodicCQITobeTriggerWithCFR = 0;
    /* + CQI_5.0 */

    /* SPR 5798 Start */
    UInt8 msg3ScheduledCount = 0;
    /* SPR 5798 End */

    UInt32 ulHarqSysFrameNum = 0;
    UInt32 timerExpirySubFrameNum = 0;
    /*SPR 22322 Start*/
#ifdef FDD_CONFIG
    UInt32 ulHarqSubFrameNum = 0; 
    /* + Bandwidth Fix */
#elif TDD_CONFIG    
    UInt32 index = 0;
#endif
    /*SPR 22322 End*/
    /* - Bandwidth Fix */

    /*DCI_1C_Comment Start */
    UInt8 dciFormat =  0;
    /*DCI_1C_Comment End */
    UInt8 retVal = 0;
    /* Cyclomatic Complexity changes */
    UInt8 tpc = 0;
    /*SPR 22322 Start*/
    /* Earlier we had the handling common for both FDD and TDD i.e we are getting the ulHarqSysFrameNum using function getSFAndSFN
     * But in case of boundary condition for eg the current SF id 8 or 9 then in that case the SFN was not getting incremented to 
     * the next value as in the function updateTimerExpirySubFrame the value of timerExpirySubFrameNum was coming out to be less 
     * than max value.This was caused because in case of FDD we are correctly calculating timerExpirySubFrameNum as ulHarqSubFrameNum
     * is the current SF and doesn't have any PHY delay and DL delay added whereas in case of TDD subFrameForRAR already has
     * PHY_DL_DELAY and dldelay added because of which while calculating ulHarqSysFrameNum in case of TDD using timerExpirySubFrameNum
     * value of SFN was coming wrong and later on timer node was getting pushed at wrong index.
     */
#ifdef FDD_CONFIG
    getSFAndSFN(&ulHarqSysFrameNum, &ulHarqSubFrameNum, internalCellIndex);

    timerExpirySubFrameNum  = (ulHarqSubFrameNum + PHY_DL_DELAY + dlDelay +
            UE_RA_ADV_TTI);
#elif TDD_CONFIG
    /* msg3_retx_tdd_support_start */
    ulHarqSysFrameNum = sysFrameNumForRAR;
    timerExpirySubFrameNum  = subFrameForRAR + getULSfForMsg3(subFrameForRAR,internalCellIndex);
#endif
    /* msg3_retx_tdd_support_end */
    /*SPR 22322 End*/

    updateTimerExpirySubFrame(&timerExpirySubFrameNum, &ulHarqSysFrameNum);

    /*SPR_3305_FIX_START*/
#ifdef FDD_CONFIG
    UInt8 hiIndex = (currentGlobalTTI + PHY_DL_DELAY + dlDelay + 
            UE_RA_ADV_TTI + FDD_HARQ_OFFSET) % MAX_PHICH_CONTAINER;
    /*SPR 15909 fix start*/
    tickType_t timerExpiredTTI = currentGlobalTTI + PHY_DL_DELAY + dlDelay + 
        UE_RA_ADV_TTI;
    /* msg3_retx_tdd_support_start */
#elif TDD_CONFIG
    tickType_t timerExpiredTTI = msg3TTI;
    /*SPR 15909 fix end*/
    /* msg3_retx_tdd_support_end */
#endif
    /*SPR_3305_FIX_END*/


    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);


    /* + SPR 20176 - code removed */
    buffLen = (numRAR * (RAR_SUB_HEADER_LEN + RAR_LEN)) + 
        (BIFlag * RAR_SUB_HEADER_LEN);

    /*DCI_1C_Comment Start */

    /*CA Changes start  */
    dciFormat = cellParams_p->dciFormatForCommonChnlMsg;
    /*CA Changes end  */

    /*KLOCKWORK_CHANGES_START_2*/
    if(DCI_FORMAT_1A ==  dciFormat )
    {
        /* Cyclomatic Complexity changes - starts here */
        determineMCSFromTransmissionTypeAndMessageLengthForDCIFormate1A
            (buffLen,
             &tbSize,
             &numRB,
             &mcsIndex,
             &tpc,
             /*CA Changes start  */
             cellParams_p);
        /*CA Changes end  */
        /* Cyclomatic Complexity changes - ends here */
    }
    else if(DCI_FORMAT_1C ==  dciFormat )
    {
        /* Cyclomatic Complexity changes - starts here */
        determineMCSFromTransmissionTypeAndMessageLengthForDCIFormate1C
            (buffLen,
             &tbSize,
             &numRB,
             &mcsIndex,
             &tpc,
             /*CA Changes start  */
             cellParams_p);
        /*CA Changes end  */
        /* Cyclomatic Complexity changes - ends here */
    }
    /*KLOCKWORK_CHANGES_STOP_2*/

    hdrLen = DATA_MSG_API_HEADER_LEN;

    msg_struct_p = msgAlloc(PNULL,0,0,0);
    msgReserve(msg_struct_p,0, (tbSize + hdrLen));
    msg_p = msgSegNext(msg_struct_p,0,&segment);
    msg_p = segment.base;
    /* +COVERITY 10077*/
    if (!msg_p)
    {
        /*L2_Fix_start*/
        msgFree(msg_struct_p);
        LOG_MAC_MSG(L2_MSG_SEGNEXT_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
                /*--coverity--530--54308*/
                getCurrentTick(),__LINE__,0,0,0,0,0.0,0.0,__FILE__,__func__);
        /*--coverity--530--54308*/
        /*L2_Fix_end*/
        /* SPR 15487 fix start */
        freeRARBeforeULRBAllocation(flag, 
                rarPduTobeScheduled_p,
                currentGlobalTTI,
                PNULL,
                internalCellIndex);
        /* SPR 15487 fix end */
        lteWarning("msg_p is NULL");
        return;

    }
    /* -COVERITY 10077*/
    memSet(msg_p, 0, (tbSize + hdrLen));

    subHeader_p = msg_p + hdrLen;
    payLoad_p = (subHeader_p + ((numRAR + BIFlag) * RAR_SUB_HEADER_LEN));

    /*Backoff Indicator subheader should be the first subheader*/
    /*If BIFalg set first add the BI subheader*/
    addBackoffIndicatorSubheader(BIFlag,numRAR,subHeader_p);

    while ((count < numRAR) && 
            (PNULL != raReqToscheduled_p[count].raReqQNode_p))
    {
        raQNode_p = raReqToscheduled_p[count].raReqQNode_p;

        /*Allocate the free temporary CRNTT*/
        tcRnti = raReqToscheduled_p[count].tcRNTI;
        numRBAllocated = raReqToscheduled_p[count].numRBAllocated;
        mcsIndexForMsg3 = raReqToscheduled_p[count].mcsIndex;

        preambleIndex = raQNode_p->raPreambleInfo.preamble;

        LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
                "[%s]: T-CRNTI assigned [%d]\n",
                __func__,tcRnti);

        if(rachConfig_p->countOfContentionBasedRAPreambles > preambleIndex)
        {   
            /* + SPR 20176 */
#ifdef FDD_CONFIG
            /* SPR 10877 fix start */
            tempHarqProcessId = 
            MODULO_EIGHT((currentGlobalTTI + PHY_DL_DELAY /* PDCCHDelay */ 
                        + dlDelay + UE_RA_ADV_TTI));
            /* SPR 10877 fix end */
#endif
            /* - SPR 20176 */

            tempCRNTICtx_p = getTempCRNTIContextPointer(tcRnti, internalCellIndex);

            if(PNULL == tempCRNTICtx_p)
            {    
                tempCRNTICtx_p = createTempCRNTICtx(tcRnti, raQNode_p, internalCellIndex,execArgs);
                if(PNULL == tempCRNTICtx_p)
                {
                    /* + E_CID_5.3 */
                    allocateUplinkFailure(rarPduTobeScheduled_p, internalCellIndex);

                    /*Free the memory allocated for RAR*/
                    msgFree(msg_struct_p);
                    return;
                    /* - E_CID_5.3 */
                }
                /* + E_CID_5.3 */
                /*CA Changes start  */
                if(INVALID_UE_INDEX != tempTimingAdvanceR9_g[internalCellIndex][preambleIndex].ueIndex)
                {
                    tempCRNTICtx_p->exsistingUeIdx = 
                        tempTimingAdvanceR9_g[internalCellIndex][preambleIndex].ueIndex;
                }
                else
                {
                    tempCRNTICtx_p->exsistingUeIdx = INVALID_UE_INDEX;
                }
                tempCRNTICtx_p->type2TAValue = 
                    tempTimingAdvanceR9_g[internalCellIndex][preambleIndex].type2TAValue;
                tempCRNTICtx_p->type2TAreceivedTTI = 
                    tempTimingAdvanceR9_g[internalCellIndex][preambleIndex].type2TAreceivedTTI;
                tempTimingAdvanceR9_g[internalCellIndex][preambleIndex].type2TAValue = 
                    INVALID_TIMING_ADVANCE_R9_VALUE;
                tempTimingAdvanceR9_g[internalCellIndex][preambleIndex].type2TAreceivedTTI = 0;
                /*CA Changes end  */
                /* - E_CID_5.3 */
            }
            /* + E_CID_5.3 */
            /* SPR#6915 Fix Start */
            /*            if(PNULL == tempCRNTICtx_p)
                          {
                          freeMemPool(raQNode_p);
                          raQNode_p = PNULL;
                          count++;
                          continue;
                          }*/
            /* - E_CID_5.3 */
            /* msg3_retx_tdd_support_start */
#if TDD_CONFIG
            /* SPR 11257 Changes Start*/
            tempHarqProcessId = getULHarqIDForMsg3Tdd(dci0SubFrameForMsg3,
                    /* TDD Config 0 Changes Start */
                    subFrameForMsg3,
                    dci0Tick,
                    internalCellIndex); 
            /* TDD Config 0 Changes End */
            /* SPR 11257 Changes End*/
            /* msg3_retx_tdd_support_end */
#endif
            tempULHarqInfo_p = &(tempCRNTICtx_p->ulHarq);
#ifdef FDD_CONFIG
            if ((MAC_SUCCESS == 
                        allocateUplinkResources(
                            numRBAllocated,
                            numRBAllocated,
                            &tempAllocatedResInfo, 1,
                            /* ICIC changes start */
                            &rbMapNode_p->availableResourceInfo[CC_CE_REGION],
                            /* SPR 21958 PUSCH RAT1 Support Start */
                            rbMapNode_p,
                            internalCellIndex
                            ,RAT_1_NOT_SUPPORTED)) &&
                            /* SPR 21958 PUSCH RAT1 Support End */
                    (MAC_SUCCESS ==  
                     getPhichGrpAndPhichSeqForMsg3(tempAllocatedResInfo.allocatedIndex,
                         &tempULHarqInfo_p->nPhichGrp,
                         &tempULHarqInfo_p->nPhichSeq,
                         &tempULHarqInfo_p->sfUsed,
                         &tempULHarqInfo_p->nDmrs,
                         /*CA Changes start  */
                         hiIndex, internalCellIndex)))
                /*CA Changes end  */
#elif TDD_CONFIG
                if (MAC_SUCCESS == 
                        allocateUplinkResources(
                            numRBAllocated,
                            numRBAllocated,
                            &tempAllocatedResInfo, 1,
                            /* ICIC changes start */
                            &rbMapNode_p->availableResourceInfo[CC_CE_REGION],
                            /* SPR 21958 PUSCH RAT1 Support Start */
                            rbMapNode_p,
                            /* SPR 21958 PUSCH RAT1 Support End */
                            /* ICIC changes end  */
                            internalCellIndex,
                            /* TDD Config 0 Changes Start */
                            &nDmrs,
                            phichSubFrameNum,
                            iPhich)) 
#endif

                {
#ifdef TDD_CONFIG
                    reserveHiAndUpdateHarqInfo(iPhich,
                            tempAllocatedResInfo.allocatedIndex,
                            nDmrs,
                            phichSubFrameNum,
                            tempULHarqInfo_p,
                            internalCellIndex);
                    /* SPR 11257 Changes Start*/
                    tempULHarqInfo_p->ulHarqProcessId = tempHarqProcessId;
                    /* SPR 11257 Changes End*/
#endif
                    /* TDD Config 0 Changes End */
                    /* msg_retx_tdd_support_start */
#ifdef PERF_STATS
                    /* EVENT: EVENT_PARAM_DCCH_CCCH_UL_RES , UL PRB usage for SRB*/
                    /* + PERF_CA */
                    gMacCellPerfStats_p[internalCellIndex]->lteCellMacULPerfStats.totalULPRBUsageSRB[SRB_0_LCID] += \
                                                                                                                    tempAllocatedResInfo.allocatedLength;
                    /* - PERF_CA */
#endif

                    /* ICIC changes start */
                    updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                            tempAllocatedResInfo.allocatedIndex,
                            tempAllocatedResInfo.allocatedLength,
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */
                    /* ICIC changes end */
                    /*Start the timer and save the timer node in the temporary 
                     *context
                     */
                    /* msg3 ulconfig fix start */
                    /* +- SPR 22322 */
                    /* SPR 11438 fix end */

                    /*SPR_3305_FIX_START*/
                    /* +- SPR 17777 */
                            /* 4x4 DL MIMO CHG END */
                    allocateULHarqProcess(tempULHarqInfo_p,
                            /* +- SPR 17777 */
                            &tempAllocatedResInfo
                    /* 4x4 DL MIMO CHG END */
#ifdef TDD_CONFIG
                            /* SPR 11331 Fix Start */
                            ,msg3TTI
                            /* SPR 11331 Fix End */
#endif
                            );

                    LOG_UT(MAC_MSG3_RESOURCE_ALLOCATED,
                            LOGDEBUG,
                            MAC_RACH,
                            getCurrentTick(),
                            mcsIndexForMsg3,
                            tempULHarqInfo_p->nPhichGrp,
                            tempULHarqInfo_p->sfUsed,
                            tempULHarqInfo_p->riStart,
                            tempULHarqInfo_p->nPhichSeq,
                            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"Msg3 res alloc succ");
                    /* msg3 ulconfig fix start */
                    /*UL Config for Msg3 */
                    /* SPR 1600 changes start */
                    tempULHarqInfo_p->mcsIndex = mcsIndexForMsg3;
                    tempULHarqInfo_p->freqEnabledFlag = 0;
                    tempULHarqInfo_p->freqHoppingBits = 0;
                    tempULHarqInfo_p->ulTxMode = MAC_UL_TX_MODE_SISO;
                    /*start the UL Harq timer for Msg3*/
                    /*TDD Config 0 Changes Start*/
                    ulHarqTimerStartForMsg3(tcRnti,
                            tempHarqProcessId,
                            timerExpirySubFrameNum,
                            timerExpiredTTI,
                            ulHarqSysFrameNum,
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */
                    /* SPR 1744 changes end */
                    /* SPR 1600 changes end */
                    /* msg3 ulconfig fix end */
                    /*TDD Config 0 Changes End*/
                    /* SPR 5798 Start */
#ifdef FDD_CONFIG
                    /*CA Changes start  */
                    msg3ScheduledCount = rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[timerExpirySubFrameNum];
                    msg3ScheduledCount++;
                    rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[timerExpirySubFrameNum] = msg3ScheduledCount;
#elif TDD_CONFIG
                    msg3ScheduledCount = rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainer];
                    msg3ScheduledCount++;
                    rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainer] = msg3ScheduledCount;
                    /*CA Changes end  */
#endif                
                    /* SPR 5798 End */

                    resourceInfo[count].riStart  = tempAllocatedResInfo.allocatedIndex;
                    resourceInfo[count].riLength = tempAllocatedResInfo.allocatedLength;
                    resourceInfo[count].tcRNTI   = tcRnti;
                    resourceInfo[count].raReqQNode_p   = raQNode_p;
                    resourceInfo[count].ulHarq   = tempULHarqInfo_p;
                    resourceInfo[count].harqProcessId = tempHarqProcessId;
                    /*SPR 5502 changes start */
                    resourceInfo[count].isCqiWithCFRPresent = FALSE;
                    resourceInfo[count].ueIndex = INVALID_UE_INDEX;
                    /*SPR 5502 changes end */
                    /*SPR_3305_FIX_END*/

                    /* Klockwork warning Changes start */
                    /*This is not the last sub header*/
                    FILL_RAR_SUB_HEADER(subHeader_p, 
                            raQNode_p->raPreambleInfo.preamble, 
                            (count < (numRAR-1) ) /*Extention flag*/);
                    /* Klockwork warning Changes end */
                    subHeader_p++;

                    raQNode_p->raPreambleInfo.tmcs = mcsIndexForMsg3;

                    /*Prepare RAR*/
                    payLoad_p = fillRAR(payLoad_p,
                            tempAllocatedResInfo.allocatedRiv,
                            tcRnti,
                            /* + CQI_5.0 */
                            raQNode_p,aperiodicCQITobeTriggerWithCFR,
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */
                    /* - CQI_5.0 */

                    count++;

                    /* + PERF_STATS PH1D1 */
#ifdef PERF_STATS
                    contBasedPreambleCount++;
#endif
                    /* - PERF_STATS PH1D1 */
                    LOG_MAC_MSG(RA_REQ_RB_ALLOC_ID,
                            LOGDEBUG,
                            MAC_RACH,
                            getCurrentTick(),
                            raQNode_p->raRNTI,
                            raQNode_p->raPreambleInfo.preamble,
                            tcRnti,
                            tempAllocatedResInfo.allocatedIndex,
                            tempAllocatedResInfo.allocatedLength,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,
                            "CBR_RA_REQ_RB_ALLOC");

                }
                else
                {
                    /*RBs available already checked before. So this should not 
                     *happen. 
                     */
                    LOG_MAC_MSG(MAC_RAR_RB_ALLOC_FAIL_ID,LOGWARNING,MAC_RACH,
                            getCurrentTick(),
                            tcRnti,
                            numRBAllocated,
                            rbMapNum,
                            numRAR,LINE_NUMBER,
                            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"CBR_RAR_RB_ALLOC_FAIL");

                    LOG_MAC_MSG(RAR_PDU_FREE_ID,
                            LOGDEBUG,
                            MAC_RACH,
                            getCurrentTick(),
                            rarPduTobeScheduled_p->raRnti,
                            rarPduTobeScheduled_p->noOfRAResponses,
                            rarPduTobeScheduled_p->BIFlag,
                            rarPduTobeScheduled_p->exptti,
                            rarPduTobeScheduled_p->tti,
                            (UDouble32)LINE_NUMBER,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,
                            "CBR_RAR_RB_ALLOC_FAIL");

                    /*CA Changes start  */
                    allocateUplinkFailure(rarPduTobeScheduled_p, internalCellIndex);
                    /*CA Changes end  */

                    /*Free the memory allocated for RAR*/
                    msgFree(msg_struct_p);
                    return;
                }
        }
        else
        {
            contFreePreamble_p = 
                getContentionFreePreambleInfo(preambleIndex, internalCellIndex);

            ueIndex = 
                contFreePreamble_p->ueIdentifier;

            ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
            ueULContext_p = ulUEContextInfo_p->ulUEContext_p;
            /* + CQI_5.0 */
            ueDLContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
            /* - CQI_5.0 */
#ifdef TDD_CONFIG
            /* msg3_retx_tdd_support_start */
            /* SPR 11257 Changes Start*/
            tempHarqProcessId = getULHarqIDForMsg3Tdd(dci0SubFrameForMsg3,
                    /* TDD Config 0 Changes Start */
                    subFrameForMsg3,
                    dci0Tick,
                    internalCellIndex); 
            /* TDD Config 0 Changes End */
            /* SPR 11257 Changes End*/
            /* msg3_retx_tdd_support_end */
            /* SPR 3114 End*/
            /* + COVERITY 5.0 */
            if (BUSY_ALL_HARQ == tempHarqProcessId)
            {

                /* + E_CID_5.3 */
                allocateUplinkFailure(rarPduTobeScheduled_p, internalCellIndex);
                /*Free the memory allocated for RAR*/
                msgFree(msg_struct_p);
                return;

                /* - E_CID_5.3 */
            }
            /* - COVERITY 5.0 */
            /* + SPR 20176 */
#elif FDD_CONFIG
            if(ueULContext_p->ttiBundlingEnabled)
            {
                tempHarqProcessId = 
                GET_TTIB_HARQ_ID((currentGlobalTTI + PHY_DL_DELAY /* PDCCHDelay */ 
                            + dlDelay + UE_RA_ADV_TTI));

            }
            else
            {
                tempHarqProcessId = 
                MODULO_EIGHT((currentGlobalTTI + PHY_DL_DELAY /* PDCCHDelay */ 
                            + dlDelay + UE_RA_ADV_TTI));
            }
#endif
            /* + SPR 20176 */
            tempULHarqInfo_p = &(ueULContext_p->ulresInfo[tempHarqProcessId]);
#ifdef FDD_CONFIG
            /* + TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
            if(ueULContext_p->ttiBundlingEnabled)
            {
                hiIndex = (hiIndex + TTIB_FIRST_TO_LAST_TICK_OFFSET) % MAX_PHICH_CONTAINER;
                timerExpirySubFrameNum  = (timerExpirySubFrameNum + TTIB_FIRST_TO_LAST_TICK_OFFSET);
                updateTimerExpirySubFrame(&timerExpirySubFrameNum, &ulHarqSysFrameNum);
                timerExpiredTTI = timerExpiredTTI + TTIB_FIRST_TO_LAST_TICK_OFFSET;

                UInt32 tickToReserve = (currentGlobalTTI + PHY_DL_DELAY + dlDelay + UE_RA_ADV_TTI) - FDD_HARQ_OFFSET;

                 ttiMapId1 = MODULO_SIXTEEN(currentGlobalTTI + PHY_DL_DELAY + dlDelay + UE_RA_ADV_TTI);
                 ttiMapId2 = MODULO_SIXTEEN(currentGlobalTTI + PHY_DL_DELAY + dlDelay + UE_RA_ADV_TTI + MAX_HARQ_PROCESS_NB );
                 if((HARQ_PROCESS_FREE != ueULContext_p->ulresInfo[tempHarqProcessId].harqStatus)||
                    (MAC_FAILURE == processTtiBHarqCheck(tickToReserve,0,ueULContext_p))||
                    (TTI_MAP_FREE != ueULContext_p->ttiHarqMap[ttiMapId1]) ||
                    (TTI_MAP_FREE != ueULContext_p->ttiHarqMap[ttiMapId2]))
                {
                    retValue = MAC_FAILURE;
                }
                else
                {
                    retValue = allocateUplinkResourcesForTTIBundle(
                        numRBAllocated,
                        numRBAllocated,
                        &tempAllocatedResInfo,
                        1,
                        &rbMapNode_p->availableResourceInfo[CC_CE_REGION],
                        rbMapNode_p->rbMap, 
                        0,
                        tickToReserve,
                        ueULContext_p,
                        internalCellIndex);
                    if (MAC_SUCCESS == retValue)
                    {
                        retValue = getPhichGrpAndPhichSeqForMsg3(tempAllocatedResInfo.allocatedIndex,
                         &tempULHarqInfo_p->nPhichGrp,
                         &tempULHarqInfo_p->nPhichSeq,
                         &tempULHarqInfo_p->sfUsed,
                         &tempULHarqInfo_p->nDmrs,
                         hiIndex,
                         internalCellIndex);
                        if(MAC_SUCCESS != retValue)
                        {
                            UInt8 bundleTick = 0;
                            for(bundleTick = 0; bundleTick < TTI_BUNDLE_SIZE; bundleTick++)
                            {
                                freeResourceUsingRBIndex(tempAllocatedResInfo.allocatedLength,
                                                       tempAllocatedResInfo.allocatedIndex, 
                                                       MODULO_SIXTEEN(tickToReserve + FDD_HARQ_OFFSET + bundleTick),
                                                       internalCellIndex);
                            }
                        }
                    }
                }
               
            }
            else
            {
                retValue =    allocateUplinkResources(
                            numRBAllocated,
                            numRBAllocated,
                            &tempAllocatedResInfo, 1,
                            /* ICIC changes start */
                            &rbMapNode_p->availableResourceInfo[CC_CE_REGION],
                            /* SPR 21958 PUSCH RAT1 Support Start */
                            rbMapNode_p,
                            internalCellIndex
                            ,RAT_1_NOT_SUPPORTED
                            /* SPR 21958 PUSCH RAT1 Support End */);
                if (MAC_SUCCESS == retValue)
                {
                   retValue = getPhichGrpAndPhichSeqForMsg3(tempAllocatedResInfo.allocatedIndex,
                         &tempULHarqInfo_p->nPhichGrp,
                         &tempULHarqInfo_p->nPhichSeq,
                         &tempULHarqInfo_p->sfUsed,
                         &tempULHarqInfo_p->nDmrs,
                         hiIndex,
                         internalCellIndex);
                }
            }
            if (MAC_SUCCESS == retValue)
            /* - TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */

#elif TDD_CONFIG
                if (MAC_SUCCESS == 
                        allocateUplinkResources(
                            numRBAllocated,
                            numRBAllocated,
                            &tempAllocatedResInfo, 1,
                            /* ICIC changes start */
                            &rbMapNode_p->availableResourceInfo[CC_CE_REGION],
                            /* SPR 21958 PUSCH RAT1 Support Start */
                            rbMapNode_p,
                            /* SPR 21958 PUSCH RAT1 Support End */
                            /* ICIC changes end */
                            internalCellIndex, 
                            /* TDD Config 0 Changes Start */
                            &nDmrs,
                            phichSubFrameNum,
                            iPhich
                            /* TDD Config 0 Changes End */
                            )) 
#endif
                {
#ifdef TDD_CONFIG
                    /* TDD Config 0 Changes Start */
                    reserveHiAndUpdateHarqInfo(iPhich,
                            tempAllocatedResInfo.allocatedIndex,
                            nDmrs,
                            phichSubFrameNum,
                            tempULHarqInfo_p,
                            internalCellIndex);
#endif
                    /* TDD Config 0 Changes End */

#ifdef PERF_STATS
                    /* EVENT: EVENT_PARAM_DCCH_CCCH_UL_RES , UL PRB usage for SRB*/
                    /* + PERF_CA */
                    gMacCellPerfStats_p[internalCellIndex]->lteCellMacULPerfStats.totalULPRBUsageSRB[SRB_0_LCID] += \
                                                                                                                    tempAllocatedResInfo.allocatedLength;
                    /* - PERF_CA */

#endif

                    /* ICIC changes start */
                    updateCEAvailRes(&rbMapNode_p->availableResourceInfo[CE_REGION],
                            tempAllocatedResInfo.allocatedIndex,
                            tempAllocatedResInfo.allocatedLength,
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */
                    /* ICIC changes end */
                    resourceInfo[count].riStart  = tempAllocatedResInfo.allocatedIndex;
                    resourceInfo[count].riLength = tempAllocatedResInfo.allocatedLength;
                    resourceInfo[count].tcRNTI   = tcRnti;
                    resourceInfo[count].raReqQNode_p   = raQNode_p;
                    /* + TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
                    resourceInfo[count].ueIndex   = ueIndex;
                    /* - TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */

                    /* +- SPR 17777 */

                    allocateULHarqProcess(tempULHarqInfo_p,
                            /* +- SPR 17777 */
                            &tempAllocatedResInfo
                    /* 4x4 DL MIMO CHG END */
#ifdef TDD_CONFIG
                            /* SPR 11331 Fix Start */
                            ,msg3TTI
                            /* SPR 11331 Fix End */
#endif
                            );

                  /* + TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
#ifdef FDD_CONFIG            
                  if(ueULContext_p->ttiBundlingEnabled)
                  {
                      tempULHarqInfo_p->isTTIBundled = TRUE;
                      tempULHarqInfo_p->isTTIDataRcvd = FALSE;
                      tempULHarqInfo_p->ttiBundleFirstTick = (currentGlobalTTI + PHY_DL_DELAY + dlDelay + UE_RA_ADV_TTI);
                  }
#endif                  
                  /* - TTIB_Code : For Contention free PDCCH ORDER Rach - SPR 18703 */
                    /* SPR_8672_FIX */
#ifdef FDD_CONFIG
                    updateTTIHarqMap( tempULHarqInfo_p->ulHarqProcessId, 
                            tempULHarqInfo_p, ueULContext_p->ttiHarqMap);
#endif
                    /* SPR_8672_FIX */
                    tempULHarqInfo_p->freqEnabledFlag = 0;
                    tempULHarqInfo_p->freqHoppingBits = 0;
                    tempULHarqInfo_p->ulTxMode = MAC_UL_TX_MODE_SISO;
                    tempULHarqInfo_p->mcsIndex = mcsIndexForMsg3;

                    /* SPR 8416 changes start */
                    /*start the UL Harq timer for Msg3*/
#ifdef FDD_CONFIG
#ifdef ULHARQ_TIMER_PROC
                    ulHarqTimerStart(ueIndex,
                            tempHarqProcessId,
                            timerExpirySubFrameNum,
                            /* + SPS_TDD_Changes */
                            DRA_SCHEDULING,
                            /* - SPS_TDD_Changes */
                            timerExpiredTTI,
                            ulHarqSysFrameNum,
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */
#endif
#elif TDD_CONFIG
                    /* TDD Config 0,6 Changes Start */
                    ulHarqTimerStart( dci0SubFrameForMsg3, 
                            dci0SysFrameNumForMsg3,
                            ueIndex, 
                            tempHarqProcessId,
                            /* + SPS_TDD_Changes */
                            DRA_SCHEDULING,
                            /* - SPS_TDD_Changes */
                            subFrameForMsg3,
                            internalCellIndex
                            );
                    /* TDD Config 0,6 Changes End */
#endif
                    /* SPR 8416 changes end */

                    /* SPR 5798 Start */
#ifdef FDD_CONFIG
                    /*CA Changes start  */
                    msg3ScheduledCount = rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[timerExpirySubFrameNum];
                    msg3ScheduledCount++;
                    rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[timerExpirySubFrameNum] = msg3ScheduledCount;
#elif TDD_CONFIG
                    msg3ScheduledCount = rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainer];
                    msg3ScheduledCount++;
                    rachGlobals_gp[internalCellIndex]->msg3ScheduledCount[msg3CountContainer] = msg3ScheduledCount;
                    /*CA Changes end  */
#endif                
                    /* SPR 5798 End */

#ifdef TDD_CONFIG
                    setHarqBusy( subFrameForMsg3, /* UL SF on which data needs to come */
                            tempHarqProcessId,
                            ueULContext_p,
                            internalCellIndex,
                            /* TDD Config 6 Changes Start */
                            sysFrameNumForMsg3
                            /* TDD Config 6 Changes End */
                            );

#endif 
                    resourceInfo[count].ulHarq   = tempULHarqInfo_p;
                    resourceInfo[count].harqProcessId   = tempHarqProcessId;

                    /* Klockwork warning Changes start */
                    /*This is not the last sub header*/
                    FILL_RAR_SUB_HEADER(subHeader_p, 
                            raQNode_p->raPreambleInfo.preamble, 
                            (count < (numRAR-1) ) /*Extention flag*/);
                    /* Klockwork warning Changes end */
                    subHeader_p++;

                    raQNode_p->raPreambleInfo.tmcs = mcsIndexForMsg3;

                    /* + CQI_5.0 */
                    /* SPR 5502 changes start */
                    resourceInfo[count].isCqiWithCFRPresent = FALSE;
                    checkAperiodicCQITobeTrigger(ueDLContext_p->dlCQIInfo.cqiTypeIndicator,
                            &aperiodicCQITobeTriggerWithCFR,&resourceInfo[count].isCqiWithCFRPresent,
                            &resourceInfo[count].ueIndex,ueIndex);
                    /* SPR 5502 changes end. */
                    /* - CQI_5.0 */

                    /*Prepare RAR*/
                    payLoad_p = fillRAR(payLoad_p,
                            tempAllocatedResInfo.allocatedRiv,
                            tcRnti,
                            /* + CQI_5.0 */
                            raQNode_p,aperiodicCQITobeTriggerWithCFR,
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */
                    /* - CQI_5.0 */

                    /* SPR 15682 fix start */
                    /* Refer Spec 36.213 Section 4.2.3 for the calculations
                     * NTA = TA ×16 , where TA = 0, 1, 2, ..., 1282*/
                    ueDLContext_p->timingAdvanceAbsValue =
                        (raQNode_p->raPreambleInfo.timingAdvance << 4);
                    /* SPR 15682 fix end */
                    /* SPR 16000 Fix Start */
                    ueDLContext_p->lastTASentTick = getCurrentTick();
                    /* SPR 16000 Fix End */
                    /*+ SPR 8564*/
                    ulUEContextInfo_p->ulUEContext_p->waitForMsg3BeforeResetting = FALSE;
                    /*- SPR 8564*/

                    count++;

                    /* + PERF_STATS PH1D1 */
#ifdef PERF_STATS
                    contFreePreambleCount++;
#endif
                    /* - PERF_STATS PH1D1 */

                    LOG_MAC_MSG(RA_REQ_RB_ALLOC_ID,
                            LOGDEBUG,
                            MAC_RACH,
                            getCurrentTick(),
                            raQNode_p->raRNTI,
                            raQNode_p->raPreambleInfo.preamble,
                            tcRnti,
                            tempAllocatedResInfo.allocatedIndex,
                            tempAllocatedResInfo.allocatedLength,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,
                            "CFR_RA_REQ_RB_ALLOC");

                }
                else
                {
                    /*RBs available already checked before. So this should not 
                     *happen. 
                     */

                    LOG_MAC_MSG(MAC_RAR_RB_ALLOC_FAIL_ID,LOGWARNING,MAC_RACH,
                            getCurrentTick(),
                            tcRnti,
                            numRBAllocated,
                            rbMapNum,
                            numRAR,LINE_NUMBER,
                            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"CFR_RAR_RB_ALLOC_FAIL");

                    LOG_MAC_MSG(RAR_PDU_FREE_ID,
                            LOGDEBUG,
                            MAC_RACH,
                            getCurrentTick(),
                            rarPduTobeScheduled_p->raRnti,
                            rarPduTobeScheduled_p->noOfRAResponses,
                            rarPduTobeScheduled_p->BIFlag,
                            rarPduTobeScheduled_p->exptti,
                            rarPduTobeScheduled_p->tti,
                            (UDouble32)LINE_NUMBER,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,
                            "CFR_RAR_RB_ALLOC_FAIL");

                    allocateUplinkFailure(rarPduTobeScheduled_p, internalCellIndex);
                    /*Free the memory allocated for RAR*/
                    msgFree(msg_struct_p);
                    return;
                }
        }
    }

    /*Prepaire and push RA Rsp queue node to dlRARspQueue_g queue*/
    /* SPR 21870 Fix Start */
    if(0 == count)
    {
        fprintf(stderr,"\n noOfRAResponses in rarPduTobeScheduled_p [%d]\n",rarPduTobeScheduled_p->noOfRAResponses); 
        /*Free the memory allocated for RAR*/
        msgFree(msg_struct_p);
        return;
    }
    /* SPR 21870 Fix End */

    /*Sending RAR to DL Scheduler*/
    GET_MEM_FROM_POOL(DLRARspQueueNode , raRspQueueNode_p , sizeof(DLRARspQueueNode) , PNULL);
    if(PNULL == raRspQueueNode_p)
    {
        msgFree(msg_struct_p);
        return;
    }

    raRspQueueNode_p->rarToDl.numRB        = numRB;
    raRspQueueNode_p->rarToDl.mcsIndex     = mcsIndex;
    raRspQueueNode_p->rarToDl.dciFormat    = cellParams_p->dciFormatForCommonChnlMsg;
    raRspQueueNode_p->rarToDl.dataLen      = tbSize;
    raRspQueueNode_p->rarToDl.raRNTI       = raRNTI;
    raRspQueueNode_p->rarToDl.numRAR       = count;
    raRspQueueNode_p->rarToDl.exptti       = expTTI;
    raRspQueueNode_p->rarToDl.macPduData_p = msg_struct_p;
#ifdef TDD_CONFIG    
    raRspQueueNode_p->rarToDl.tti = 
        (currentGlobalTTI + dlDelay + PHY_DL_DELAY + 
         getULSfForMsg3(subFrameForRAR, internalCellIndex));
    raRspQueueNode_p->index = index;
#else
    raRspQueueNode_p->rarToDl.tti = 
        (currentGlobalTTI + dlDelay + PHY_DL_DELAY + UE_RA_ADV_TTI);
#endif        

    /*Points to the first byte after the last sub header*/
    raRspQueueNode_p->rarToDl.payLoadPos   = 
        (RAR_SUB_HEADER_LEN * (BIFlag + count));

#ifdef PERF_STATS
    raRspQueueNode_p->rarToDl.contBasedPreambleCount = contBasedPreambleCount;
    raRspQueueNode_p->rarToDl.contFreePreambleCount = contFreePreambleCount;
#endif

    fillRaRspQueueNode(count,tempCRNTICtx_p,raRspQueueNode_p,
            resourceInfo,rachConfig_p,&preambleIndex,
            internalCellIndex );

    raRspQueueNode_p->rarToDl.BIFlag = rarPduTobeScheduled_p->BIFlag;
    raRspQueueNode_p->rarToDl.tpc = tpc;

    LOG_MAC_MSG(MAC_RAR_RB_ALLOC_SUCC_ID,LOGDEBUG,MAC_RACH,
            getCurrentTick(),
            raRspQueueNode_p->rarToDl.raRNTI,
            raRspQueueNode_p->rarToDl.numRB,
            raRspQueueNode_p->rarToDl.mcsIndex,
            raRspQueueNode_p->rarToDl.exptti,
            raRspQueueNode_p->rarToDl.BIFlag,
            DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,
            "MAC_RAR_RB_ALLOC_SUCC_ID");

    LOG_MAC_MSG(MAC_RAR_RB_ALLOC_MSG3_ID,LOGDEBUG,MAC_RACH,
            getCurrentTick(),
            raRspQueueNode_p->rarToDl.raRNTI,
            raRspQueueNode_p->rarToDl.tti,
            rbMapNum,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,
            "MAC_RAR_RB_ALLOC_MSG3_ID");

    /*CA Changes start  */
    retVal = ENQUEUE_DL_RA_RSP_QUEUE_Q(DLRARspQueueNode,&(raRspQueueNode_p->rarNodeAnchor), internalCellIndex);
    /*CA Changes end  */
    if(!retVal)
    {
        LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,
                LOGFATAL,
                L2_SYS_FAIL,
                getCurrentTick(),
                LINE_NUMBER,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FILE_NAME,
                FUNCTION_NAME);

        ltePanic("pushNode() for dlRARspQueue_g failed");
    }

    LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
            "[%s]: RAR node pushed to dlRARspQueue_g\n"
            "      RA-RNTI [%d] expTTI [%d]",
            __func__,raRspQueueNode_p->rarToDl.raRNTI,
            raRspQueueNode_p->rarToDl.exptti);

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);

}

/*****************************************************************************
 * Function Name  : freeRARBeforeULRBAllocation
 * Inputs         : flag - flag to check free to be done on or before temperory context
 *                  rarPduTobeScheduled_p - Pointer for RAR to be scheduled
 *                  currentGlobalTTI
 *                  dlRARsp_p - pointer  DL RAR sepsonse
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Description    : Free the RAR it will call in  case of DCI failure 
 ******************************************************************************/
void freeRARBeforeULRBAllocation(UInt32 flag,
        RAReqTobeScheduledInPdu* rarPduTobeScheduled_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTI,
        /* SPR 15909 fix end */
        DLRARsp *dlRARsp_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    UInt8 preambleIndex = 0;
    UInt32 index = 0;
    UInt16 tcRnti  = INVALID_RNTI;
    RAReqQueueNode *raQNode_p = PNULL;
    /*CA Changes start  */
    RACHConfiguration *rachConfig_p =
        &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                rachConfigInfo.rachConfiguration);
    /*CA Changes end  */
    /*Coverity 96890 fix +*/
    RAReqTobeScheduled *raReqToscheduled_p = PNULL;
    if(PNULL != rarPduTobeScheduled_p)
    {
         raReqToscheduled_p =
            &rarPduTobeScheduled_p->raReqTobeScheduled[0];
    }
    /*Coverity 96890 fix -*/
    ContentionFreePreamble *contFreePreamble_p = PNULL;
    UInt32 ueIndex = INVALID_UE_ID;
    ULUEContext *ueULContext_p = PNULL;
    UInt8 numRAR = 0;
    /*SPR 22322 Start*/
    /* + SPR_17858_587_CHANGES */
    TempCRNTICtx  *tempCRNTICtx_p = PNULL;
    /* - SPR_17858_587_CHANGES */
    /*SPR 22322 End*/

    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
    /* + PERF_CA */
    LteCellMacULPerfStats *lteCellMacULPerfStats = &( gMacCellPerfStats_p[internalCellIndex]->\
            lteCellMacULPerfStats);
    /* - PERF_CA */
#endif
    /* - PERF_STATS_PH1D1 */
    /* Coverity 96705 Fix Start */
    if((flag == 1) && (PNULL != dlRARsp_p))
    /* Coverity 96705 Fix End */
    {
        numRAR = dlRARsp_p->numRAR;
    }
    else if(PNULL != rarPduTobeScheduled_p) /*Coverity 96890 fix -*/
    {
        numRAR = rarPduTobeScheduled_p->noOfRAResponses;
    }
    /*Free the RA Req queue nodes*/
    while (index < numRAR)
    {
        if((flag == 1) && (PNULL != dlRARsp_p))/*Coverity 96888 fix*/
        {
            raQNode_p = dlRARsp_p->resourceInfo[index].raReqQNode_p;
            tcRnti = dlRARsp_p->resourceInfo[index].tcRNTI;
        }
        else if(PNULL != raReqToscheduled_p)/*Coverity 96890 fix +-*/
        {
            raQNode_p =
                raReqToscheduled_p[index].raReqQNode_p;
            tcRnti =
                raReqToscheduled_p[index].tcRNTI;
        }
        if(PNULL != raQNode_p)
        {
            preambleIndex = raQNode_p->raPreambleInfo.preamble;

            if(rachConfig_p->countOfContentionBasedRAPreambles > 
                    preambleIndex)
            {
            /* + SPR_17858_587_CHANGES */
                /*MONZA_102_Changes Start*/
                if(flag == 1)
                {
                    /*SPR 22322 Start*/
                    /*MONZA_102_Changes End*/
                    tempCRNTICtx_p = getTempCRNTIContextPointer(tcRnti,internalCellIndex);
                    if (tempCRNTICtx_p)
                    {
                        ulHarqTimerForMsg3Stop(tempCRNTICtx_p,\
                                tempCRNTICtx_p->ulHarq.harqTimerExpirySF, 
                                tempCRNTICtx_p->ulHarq.harqTimerExpirySystemFrame,internalCellIndex);
                    }
                    else
                    {
                        LOG_MAC_MSG(MAC_TCRNTI_FAIL_ID,
                                LOGWARNING,
                                MAC_RACH,
                                getCurrentTick(),
                                tcRnti,
                                LINE_NUMBER,
                                preambleIndex,
                                rachConfig_p->countOfContentionBasedRAPreambles,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,
                                "NullTempContext");
                    }
                    /*SPR 22322 End*/
	            /* - SPR_17858_587_CHANGES */

                    /*CA Changes start  */
                    deleteTempCRNTICtx(tcRnti, internalCellIndex);
                    if (MAC_FAILURE == freeRNTI(tcRnti, internalCellIndex))
                        /*CA Changes end  */

                    {
                        LOG_MAC_MSG(MAC_TCRNTI_FAIL_ID,
                                LOGWARNING,
                                MAC_RACH,
                                getCurrentTick(),
                                tcRnti,
                                LINE_NUMBER,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,
                                "RAR_FREE_TCRNTI_FAIL");
                    }

                }
                else
                {
                    if (MAC_FAILURE == 
                            /*CA Changes start  */
                            freeRNTIBeforeUETempContextCreation(tcRnti, internalCellIndex))
                        /*CA Changes end  */
                    {
                        LOG_MAC_MSG(MAC_TCRNTI_FAIL_ID,
                                LOGWARNING,
                                MAC_RACH,
                                getCurrentTick(),
                                tcRnti,
                                LINE_NUMBER,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,
                                "RAR_FREE_TCRNTI_FAIL");

                    }
                    LOG_MAC_MSG(RA_REQ_FREE_ID,
                            LOGDEBUG,
                            MAC_RACH,
                            getCurrentTick(),
                            raQNode_p->raRNTI,
                            raQNode_p->ttiRAReqReceived,
                            raQNode_p->raPreambleInfo.preamble,
                            LINE_NUMBER,
                            tcRnti,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"PREP_DETRB_FAIL_FREE_RA_REQ");

                }
                /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CBRA++;
#endif
                /* - PERF_STATS_PH1D1 */
            }
            else
            {
                /*CA Changes start  */
                if(PREAMBLE_BUSY != getPreambleStatusInfo(preambleIndex, internalCellIndex))
                    /*CA Changes end  */
                {

                    LOG_MAC_MSG(CFR_RA_REQ_FREE_ID,
                            LOGERROR,
                            MAC_RACH,
                            getCurrentTick(),
                            raQNode_p->raRNTI,
                            raQNode_p->ttiRAReqReceived,
                            raQNode_p->raPreambleInfo.preamble,
                            raQNode_p->raPreambleInfo.tmcs,
                            LINE_NUMBER,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"CFR_PREAMBLE_NOT_BUSY");

                    freeMemPool(raQNode_p);

                    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                    lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CFRA++;
#endif
                    /* - PERF_STATS_PH1D1 */

                    index++;
                    continue;
                } 

                contFreePreamble_p = 
                    /*CA Changes start  */
                    getContentionFreePreambleInfo(preambleIndex, internalCellIndex);
                /*CA Changes end  */


                if((PDCCH_ORDER != 
                            contFreePreamble_p->pdcchOrderHandOverFlag) &&
                        (CONNECTION_HANDOVER != contFreePreamble_p->pdcchOrderHandOverFlag))
                {
                    LOG_MAC_MSG(CFR_RA_REQ_FREE_ID,
                            LOGERROR,
                            MAC_RACH,
                            getCurrentTick(),
                            raQNode_p->raRNTI,
                            raQNode_p->ttiRAReqReceived,
                            raQNode_p->raPreambleInfo.preamble,
                            raQNode_p->raPreambleInfo.tmcs,
                            LINE_NUMBER,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"CFR_RAREQ_TTI_EXPIRY");

                    freeMemPool(raQNode_p);

                    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                    lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CFRA++;
#endif
                    /* - PERF_STATS_PH1D1 */

                    index++;
                    continue;
                }
                ueIndex = contFreePreamble_p->ueIdentifier;

                ueULContext_p =
                    ulUECtxInfoArr_g[ueIndex].ulUEContext_p;

                if(PNULL == ueULContext_p)
                {

                    LOG_MAC_MSG(PREAMBLE_NO_UECONTEXT_ID,
                            LOGERROR,
                            MAC_RACH,
                            getCurrentTick(),
                            preambleIndex,
                            contFreePreamble_p->pdcchOrderHandOverFlag,
                            contFreePreamble_p->ueIdentifier,
                            LINE_NUMBER,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"PREAMBLE_NO_DLUECONTEXT_ID");

                    LOG_MAC_MSG(CFR_RA_REQ_FREE_ID,
                            LOGERROR,
                            MAC_RACH,
                            getCurrentTick(),
                            raQNode_p->raRNTI,
                            raQNode_p->ttiRAReqReceived,
                            raQNode_p->raPreambleInfo.preamble,
                            raQNode_p->raPreambleInfo.tmcs,
                            LINE_NUMBER,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"CFR_FREE_NO_DLUECONTEXT");

                    freeMemPool(raQNode_p);
                    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                    lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CFRA++;
#endif
                    /* - PERF_STATS_PH1D1 */
                    index++;
                    continue;

                }
            /* + SPR_17858_587_CHANGES */
#ifdef FDD_CONFIG
                /*Here rcvTTI is passed as 0 as it is no used in
                 *                       * function
                 *                                             */
                /* +- SPR 17777 */
                /*Coverity 96705 fix*/
                if(PNULL != dlRARsp_p)
                {
                    if(MAC_FAILURE == (ulHarqTimerStop(ueULContext_p,\
                            ueULContext_p->ulresInfo[dlRARsp_p->resourceInfo[index].harqProcessId].harqTimerExpirySF,\
                            ueULContext_p->ulresInfo[dlRARsp_p->resourceInfo[index].harqProcessId].harqTimerExpirySystemFrame,internalCellIndex)))
		    {
			LOG_MAC_MSG( MAC_UL_HARQ_TIMER_STOP_FAIL, LOGWARNING, MAC_UL_HARQ,
                                getCurrentTick(), __LINE__, ueULContext_p->ueIndex,  contFreePreamble_p->pdcchOrderHandOverFlag,
                                ueULContext_p->ulresInfo[dlRARsp_p->resourceInfo[index].harqProcessId].harqTimerExpirySF, contFreePreamble_p->ueIdentifier, 0,0, __func__, "");
	            }
                }
                else
                {
                    LOG_MAC_MSG(PREAMBLE_NO_UECONTEXT_ID,
                            LOGERROR,
                            MAC_RACH,
                            getCurrentTick(),
                            preambleIndex,
                            contFreePreamble_p->pdcchOrderHandOverFlag,
                            contFreePreamble_p->ueIdentifier,
                            LINE_NUMBER,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"DL_RA_REP_NODE_NULL");

                }
                /*Coverity 96705 fix*/
            /* - SPR_17858_587_CHANGES */
#endif

                if(ueULContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderSentCount 
                        < MAX_PDCCH_ORDER_RETRIES)
                {
                    putEntryInPdcchOrderQ(
                            ueULContext_p->ulSyncLossOrHOInfoInUL.preambleIdx, 
                            ueULContext_p->ueIndex, 
                            /*CA Changes start  */
                            currentGlobalTTI, internalCellIndex);
                    /*CA Changes end  */
                }
                else
                {
                    /* + E_CID_5.3 */
                    /* If RRM sent request for TA2 and TA1type/AOA reports then
                     * simply send response as MAC_FAILURE
                     */
                    if (NO_REPORT_TYPE_REQUIRED != 
                            ueULContext_p->eCidReportParams.typeofReportsRequired)
                    {
                        putEntryInEcidMeasReportQueue(ueIndex,\
                                NO_REPORT_TYPE_REQUIRED,\
                                MAC_FAILURE, \
                                /*CA Changes start  */
                                internalCellIndex);
                        /*CA Changes end  */
                        ueULContext_p->eCidReportParams.typeofReportsRequired = 
                            NO_REPORT_TYPE_REQUIRED;
                    }
                    else
                    {
                        /* - E_CID_5.3 */
                        /*SPR_3065_FIX*/
                        sendSyncIndToRRC(ueIndex, ueULContext_p->crnti,
                                /*CA Changes start  */
                                OUT_OF_SYNC_MAX_RETRIES, internalCellIndex);
                        /*CA Changes end  */
                        /*SPR_3065_FIX*/
                    }
                    /* Free the preamble */
                    /*CA Changes start  */
                    freePreambleIndex(
                            raQNode_p->raPreambleInfo.preamble, internalCellIndex);
                    /*CA Changes end  */
                    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                    lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CFRA++;
#endif
                    /* - PERF_STATS_PH1D1 */
                }
                LOG_MAC_MSG(CFR_RA_REQ_FREE_ID,
                        LOGERROR,
                        MAC_RACH,
                        getCurrentTick(),
                        raQNode_p->raRNTI,
                        raQNode_p->ttiRAReqReceived,
                        raQNode_p->raPreambleInfo.preamble,
                        raQNode_p->raPreambleInfo.tmcs,
                        LINE_NUMBER,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"PREP_DETRB_FAIL_FREE_RA_REQ");


            }               
            freeMemPool(raQNode_p);          
        } 
        index++;
    }
}

/*****************************************************************************
 * Function Name  : allocateUplinkFailure
 * Inputs         : rarPduTobeScheduled_p - Pointer to RAR to be scheduled 
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : None
 * Description    : DCI allocation failure handling
 ******************************************************************************/
/*CA Changes start  */
void allocateUplinkFailure(RAReqTobeScheduledInPdu* rarPduTobeScheduled_p,
        InternalCellIndex internalCellIndex)
{
    UInt8 preambleIndex = 0;
    UInt32 index = 0;
    UInt16 tcRnti  = INVALID_RNTI;
    RAReqQueueNode *raQNode_p = PNULL;
    RACHConfiguration *rachConfig_p =
        /*CA Changes start  */
        &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                cellParams_p->rachConfigInfo.rachConfiguration);
    /*CA Changes end  */
    RAReqTobeScheduled *raReqToscheduled_p =
        &rarPduTobeScheduled_p->raReqTobeScheduled[0];
    TempCRNTICtx      *tempCRNTICtx_p = PNULL;
    UInt8 numRAR = rarPduTobeScheduled_p->noOfRAResponses;
    /*Free the RA Req queue nodes*/
    while (index < numRAR)
    {
        raQNode_p =
            raReqToscheduled_p[index].raReqQNode_p;

        /*+SPR #3444*/ 
        if(PNULL != raQNode_p)
        {
            /*+SPR #3444*/ 

            preambleIndex = raQNode_p->raPreambleInfo.preamble;

            if(rachConfig_p->countOfContentionBasedRAPreambles > 
                    preambleIndex)
            {

                tcRnti =
                    raReqToscheduled_p[index].tcRNTI;

                tempCRNTICtx_p = getTempCRNTIContextPointer(tcRnti, internalCellIndex);

                if(tempCRNTICtx_p)
                {
                    /*CA Changes start  */
                    deleteTempCRNTICtx(tcRnti, internalCellIndex);

                    if (MAC_FAILURE == freeRNTI(tcRnti, internalCellIndex))
                        /*CA Changes end  */
                    {
                        LOG_MAC_MSG(MAC_TCRNTI_FAIL_ID,
                                LOGWARNING,
                                MAC_RACH,
                                getCurrentTick(),
                                tcRnti,
                                LINE_NUMBER,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,
                                "RAR_FREE_TCRNTI_FAIL");
                    }
                }
                else
                {
                    if (MAC_FAILURE == 
                            /*CA Changes start  */
                            freeRNTIBeforeUETempContextCreation(tcRnti, internalCellIndex))
                        /*CA Changes end  */
                    {
                        LOG_MAC_MSG(MAC_TCRNTI_FAIL_ID,
                                LOGWARNING,
                                MAC_RACH,
                                getCurrentTick(),
                                tcRnti,
                                LINE_NUMBER,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,
                                "RAR_FREE_TCRNTI_FAIL");
                    }
                }

                LOG_MAC_MSG(RA_REQ_FREE_ID,
                        LOGDEBUG,
                        MAC_RACH,
                        getCurrentTick(),
                        raQNode_p->raRNTI,
                        raQNode_p->ttiRAReqReceived,
                        raQNode_p->raPreambleInfo.preamble,
                        LINE_NUMBER,
                        tcRnti,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"CBR_RAR_RB_ALLOC_FAIL");

            }
            else
            {
                LOG_MAC_MSG(RA_REQ_FREE_ID,
                        LOGDEBUG,
                        MAC_RACH,
                        getCurrentTick(),
                        raQNode_p->raRNTI,
                        raQNode_p->ttiRAReqReceived,
                        raQNode_p->raPreambleInfo.preamble,
                        LINE_NUMBER,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,
                        "CFR_RAR_RB_ALLOC_FAIL");
                /* SPR 15487 fix start */
                freePreambleIndex(preambleIndex,internalCellIndex);
                tcRnti = raReqToscheduled_p[index].tcRNTI;
                if (MAC_FAILURE == 
                        freeRNTIBeforeUETempContextCreation(tcRnti,internalCellIndex))
                {
                    LOG_MAC_MSG( MAC_TCRNTI_FAIL_ID, LOGWARNING, MAC_RACH,
                            getCurrentTick(), tcRnti, __LINE__, 0,0,0,
                            0, 0, __func__, "RAR_FREE_TCRNTI_FAIL");
                }
                /* SPR 15487 fix end */
            }
            /*+ SPR #3444*/
        }
        /*- SPR #3444*/

        if(PNULL != raQNode_p)
        {
            freeMemPool(raQNode_p);          
        } 
        index++;
    }

}
/*CA Changes end  */

/*****************************************************************************
 * Function Name  : fillRAR
 * Inputs         : allocatedRiv - Value returned from the UL resources allocator
 *                  tempCRNTI    - Temporary RNTI value
 *                  raQNode_p    - Pointer to the RAReqQueueNode
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : payLoad_p - Points to the RAR message
 * Returns        : modified payLoad_p pointer
 * Description    : This will prepaires the RAR
 *****************************************************************************/
UInt8* fillRAR(UInt8 *payLoad_p,
        /* SPR 1786 changes start */
        UInt16  allocatedRiv,
        /* SPR 1786 changes end */
        UInt16 tempCRNTI,
        RAReqQueueNode *raQNode_p,
        /* + CQI_5.0 */
        UInt8 cqiWithCFR,
        /* - CQI_5.0 */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    UInt16 leastSignificantBits  = 0;
    UInt16 fixedSizeRBAssign     =0;
    UInt16 tempfixedSizeRBAssign = 0;
    UInt16 tempVal = 0;

    /*CA Changes start  */
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /*CA Changes end  */

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    /*Byte_2: LSB 4 bits of timingAdvance */
    *payLoad_p++ = (raQNode_p->raPreambleInfo.timingAdvance >> 4) & 0x7F;
    *payLoad_p   = ((raQNode_p->raPreambleInfo.timingAdvance & 0x0F) << 4);

    /*CA Changes start  */
    if (cellParams_p->ulAvailableRBs <= 44)
        /*CA Changes end  */
    {   
        /* Hopping flag 1 bit (MSB)*/
        *payLoad_p |= (((raQNode_p->raPreambleInfo.hoppingBit)& 0x01) << 3);
        /** opt **/
        /*CA Changes start  */
        leastSignificantBits = cellParams_p->resAllocInitInfo.lsbForFillRar;
        /*CA Changes end  */
        /** opt **/

        fixedSizeRBAssign =  (0xFFFFFFFF >> (32 - leastSignificantBits)) & allocatedRiv;

        /*Fixed size RBs assignment 10 bits*/
        /*MSB 3 bits of RBs assignment*/
        tempVal = fixedSizeRBAssign >> 7;
        *payLoad_p |= (tempVal )&(0x07);

        payLoad_p++;
        /*7LSB bits RBs assignmt in 7MSB of RAR*/
        *payLoad_p  = ( fixedSizeRBAssign << 1 )&(0xFE );
        /*Truncated MCS scheme 4bits;tmcs's MSB 1bit in */
        *payLoad_p |= (((raQNode_p->raPreambleInfo.tmcs) >> 3) &( 0x01));
        payLoad_p++;
        *payLoad_p  = (((raQNode_p->raPreambleInfo.tmcs) << 5 ) & (0xE0));
        /*TPC command 4 scheduled PUSCH 3bits*/
        *payLoad_p |= (((raQNode_p->raPreambleInfo.tpc) << 2) & (0x1C));
        /*UL delay 1 bit */
        *payLoad_p |= (((raQNode_p->raPreambleInfo.ulDelay) <<1 ) & (0x02));
        /*CQI request 1 bit  */
        /*payload_p |=  0x00;*/
    }
    /*CA Changes start  */
    else if ((cellParams_p->ulAvailableRBs > 44) &&
            /* +COVERITY 10714 */
            (cellParams_p->ulAvailableRBs <= 100))
        /* -COVERITY 10714 */
        /*CA Changes end  */
    {   
        /*Hopping flag 1 bit (MSB)shifting 3 bits to set at 4th bit of 2nd Byte*/
        *payLoad_p |= (((raQNode_p->raPreambleInfo.hoppingBit) & 0x01) << 3);
        /*Calculating MSB for fixSizeRBAssign*/

        /**            (ceil_wrapper((log10_wrapper(((((UDouble32 )cellConfig_g.ulAvailableRBs) *
          (cellConfig_g.ulAvailableRBs + 1.0000))/2.0000))/0.30103))-10);
         **/        
        /*10 bit Fixed RB - No.of MSB Zero bits*/
        /*there will be (10-b MSB)of RB Assignement*/

        /* spr 8623 fix start */
        fixedSizeRBAssign =  (0xFFFFFFFF >> (32 - (0x0A - 0))) & allocatedRiv;
        /* spr 8623 fix end */        

        tempfixedSizeRBAssign = fixedSizeRBAssign >> 7;

        *payLoad_p |= (tempfixedSizeRBAssign & 0x07);
        payLoad_p++;

        /*Setting the lower 7bits of fixedSizeRBAssign
         *by shifting right one bit
         */
        *payLoad_p  = ((fixedSizeRBAssign << 1 )& 0xFE);
        /*Truncated MCS scheme 4 bits */

        *payLoad_p |=(((raQNode_p->raPreambleInfo.tmcs) >> 3)&( 0x01));
        payLoad_p++;
        *payLoad_p  = (((raQNode_p->raPreambleInfo.tmcs) <<5 )&(0xE0));
        /*TPC command 4 scheduled PUSCH 3bits*/
        *payLoad_p |= (((raQNode_p->raPreambleInfo.tpc) <<2)&(0x1C));
        /*UL delay 1 bit */
        *payLoad_p |= (((raQNode_p->raPreambleInfo.ulDelay)<<1)&(0x02));
    }
    /* + CQI_5.0 */
    /*CQI request 1 bit  */
    if(TRUE == cqiWithCFR)
    {
        *payLoad_p |=  0x01;
    }   
    /* - CQI_5.0 */
    payLoad_p++;

    /*16 bits temp CRNTI */
    /* SPR 1824 changes start */            
    /* SPR 1565 changes start */            
    /* SPR 2140 changes start */            
    tempCRNTI = htons_wrapper(tempCRNTI);
    /* SPR 2140 changes end */            
    memCpy(payLoad_p,&tempCRNTI, SIZEOF_UINT16);
    /* SPR 1565 changes end */            
    /* SPR 1824 changes end */            

    payLoad_p += SIZEOF_UINT16;

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    return payLoad_p;
}


/*****************************************************************************
 * Function Name  : processContentionResAckQueue
 * Inputs         : count - number of ACKS to be processed 
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : None
 * Description    : This will process the contentionResolutionMsgAckQ_g queue 
 *                  and sets the deleteFlag in contention resolution timer node 
 *                  and calls processContentionACK() to send CCCH SDU to RRC.
 *****************************************************************************/
/*CA Changes start  */
void processContentionResAckQueue(UInt32 count, InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{
    ContentionResAckNode *contentionAckQueueNode_p = PNULL;
    TempCRNTICtx         *tempCRNTICtx_p = PNULL;

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    while(count)
    {
        contentionAckQueueNode_p = 
            /*CA Changes start  */
            (ContentionResAckNode *)popNode(contentionResolutionMsgAckQ_gp[internalCellIndex]);
        /*CA Changes end  */

        if (PNULL == contentionAckQueueNode_p)
        {
            count--;
            continue;
        }

        tempCRNTICtx_p = 
            tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[contentionAckQueueNode_p->ueIndex].tempCRNTICtx_p;
        if (tempCRNTICtx_p)
        {
            ((ContentionTimerNode *)tempCRNTICtx_p->timerNode_p)->deleteFlag = TRUE;

            /*CA Changes start  */
            processContentionACK(tempCRNTICtx_p, internalCellIndex);
            /*CA Changes end  */
        }
        tempCRNTICtx_p = PNULL;

        /* SPR 1839 fix Start */
        /* Free the contentionAckQueueNode */
        freeMemPool(contentionAckQueueNode_p); 
        /* SPR 1839 fix End  */

        count--;
    }

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);
}

/*****************************************************************************
 * Function Name  : processContentionACK
 * Inputs         : tempCRNTICtx_p - Temporary CRNTI context pointer
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : None
 * Description    : This will stop the contention resolution timer in 
 *                  contention resolution timer list and sends CCCH SDU to RRC
 *****************************************************************************/
/*CA Changes start  */
void processContentionACK(TempCRNTICtx *tempCRNTICtx_p, InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    ((ContentionTimerNode *)tempCRNTICtx_p->timerNode_p)->deleteFlag = TRUE;

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[%s]: Contention ACK received for T-CRNTI [%d]\n",
            __func__,tempCRNTICtx_p->tcRNTI);

    /*CLPC Change*/
    /* UL SAP QUEUE */   
    sendCCCHMsgToRRC(tempCRNTICtx_p->msg3BuffInfo.msg_p,
            tempCRNTICtx_p->msg3BuffInfo.msgLen,
            tempCRNTICtx_p->tcRNTI,
            tempCRNTICtx_p->timingAdvanceAbs,
            tempCRNTICtx_p->msg3BuffInfo.sinr,
            /*CA Changes start  */
            internalCellIndex);
    /*CA Changes end  */
    /*CLPC Change*/

    /*Free the Temp Context, don't return the RNTI to the free pool*/

    /* TA_CHG */
    /* The temp context shall now be deleted when the TA value received 
     * in RACH Indication is copied in the UE context */
    /* TA_CHG */

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);

    return; 
}

/*****************************************************************************
 * Function Name  : processContentionMsgTimerQueue
 * Inputs         : currentTTI - Current global tick
 *                  count -
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : None
 * Description    : This will check contention resolution timer deletes the  
 *                  temporary context if contention resolution timer expired
 *****************************************************************************/
/*CA Changes start  */
/* SPR 15909 fix start */
void processContentionMsgTimerQueue(tickType_t currentTTI, UInt32 count, InternalCellIndex internalCellIndex)
    /* SPR 15909 fix end */    
    /*CA Changes end  */
{
    ContentionTimerNode *timerQueueNode_p = PNULL;
    TempCRNTICtx         *tempCRNTICtx_p  = PNULL;     


    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    if (count)
    {
        timerQueueNode_p = 
            /*CA Changes start  */
            (ContentionTimerNode *) getFirstNode(contentionTimerQueue_gp[internalCellIndex]);
        /*CA Changes end  */

        if (PNULL == timerQueueNode_p)
        {
            return;
        }

        do
        {
            if (TRUE == timerQueueNode_p->deleteFlag)
            {
                timerQueueNode_p = 
                    /*CA Changes start  */
                    (ContentionTimerNode *)popNode(contentionTimerQueue_gp[internalCellIndex]);
                /*CA Changes end  */

                if (timerQueueNode_p != PNULL)
                {
                    LOG_MAC_MSG(MAC_CONTENT_RES_TCRNTI_DELETE_ID,LOGBRIEF,MAC_RACH,\
                            getCurrentTick(),\
                            timerQueueNode_p->expTTI,\
                            timerQueueNode_p->tcRNTI,\
                            timerQueueNode_p->deleteFlag,\
                            0,0,\
                            0.0,0.0,\
                            __func__,"CONTENT_RES_TCRNTI_DELETE");


                    freeMemPool(timerQueueNode_p);

                    timerQueueNode_p = 
                        /*CA Changes start  */
                        (ContentionTimerNode *) getFirstNode(contentionTimerQueue_gp[internalCellIndex]);
                    /*CA Changes end  */
                }
                count--;
            }
            /* Fix SPR 21492 start */ 
            else if (timerQueueNode_p->expTTI <= currentTTI || 255 == timerQueueNode_p->deleteFlag)
            /* Fix SPR 21492 end */ 
            {
                /*Timer expired*/
                /*Free the Temporary UE context and return RNTI to free pool*/
                timerQueueNode_p = 
                    /*CA Changes start  */
                    (ContentionTimerNode *)popNode(contentionTimerQueue_gp[internalCellIndex]);
                /*CA Changes end  */
                /* SPR#706 Changes Start */
                if (PNULL == timerQueueNode_p)
                {
                    LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                            "[%s]: timerQueueNode_p is Null so returning \n",
                            __func__);
                }
                else
                {
                    LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                            "[%s]: Contention timer expired TCRNTI [%d]\n",
                            __func__,timerQueueNode_p->tcRNTI);
                    {

                    tempCRNTICtx_p = getTempCRNTIContextPointer(
                            timerQueueNode_p->tcRNTI, internalCellIndex);
                    }
                    if (PNULL == tempCRNTICtx_p)
                    {
                        LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                                "[%s]: Temp Context not present for TCRNTI [%d]\n",
                                __func__,timerQueueNode_p->tcRNTI);
                    }
                    else
                    {
                        LOG_MAC_MSG(MAC_CONTENT_RES_TIMER_EXPIRY_ID,LOGINFO,MAC_RACH,\
                                getCurrentTick(),\
                                timerQueueNode_p->expTTI,\
                                timerQueueNode_p->tcRNTI,\
                                timerQueueNode_p->deleteFlag,\
                                tempCRNTICtx_p->status,0,\
                                0.0,0.0,\
                                __func__,"CONTENT_RES_TIMER_EXPIRY");

                        /*Delete the temporary context*/
                        /*CA Changes start  */
                        deleteTempCRNTICtx(timerQueueNode_p->tcRNTI, internalCellIndex);
                        if (MAC_FAILURE == freeRNTI(timerQueueNode_p->tcRNTI, internalCellIndex))
                            /*CA Changes end  */
                        {
                            LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                                    "[%s]: ERROR: Failed to return TCRNTI [%d] to free pool\n",
                                    __func__,timerQueueNode_p->tcRNTI);
                        }

                        LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                                "[%s]: Temp Context deleted and RNTI returned to\n"
                                "free pool for TCRNTI [%d]\n",
                                __func__,timerQueueNode_p->tcRNTI);
                    }

                    freeMemPool(timerQueueNode_p);
                }
                /* SPR#706 Changes End */
                timerQueueNode_p = 
                    /*CA Changes start  */
                    (ContentionTimerNode *) getFirstNode(contentionTimerQueue_gp[internalCellIndex]);
                /*CA Changes end  */

                count--;
            }
            else
            {
                break;
            }

        }while ((timerQueueNode_p) && (count));
    }

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);

    return;
}

/*****************************************************************************
 * Function Name  : startContentionResTimer
 * Inputs         : expTTI - Expiry tti
 *                  tcRNTI - TCRNTI
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : Pointer to TempContextTimerNode inserted
 * Description    : This will start the contention resolution timer
 *****************************************************************************/
/*CA Changes start  */
/* SPR 15909 fix start */
ContentionTimerNode* startContentionResTimer(tickType_t  expTTI, UInt16  tcRNTI, InternalCellIndex internalCellIndex)
    /* SPR 15909 fix end */
{
    ContentionTimerNode *timerQueueNode_p = PNULL;

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);
    GET_MEM_FROM_POOL(ContentionTimerNode, timerQueueNode_p , sizeof(ContentionTimerNode) ,PNULL);

    /* + Coverity 24518 */
    if( PNULL == timerQueueNode_p )
    {
        return PNULL ;
    }
    /* - Coverity 24518 */

    timerQueueNode_p->expTTI = expTTI;
    timerQueueNode_p->tcRNTI = tcRNTI;
    timerQueueNode_p->deleteFlag = FALSE;

    LOG_MAC_MSG(MAC_CONTENT_RES_START_TIMER_ID,LOGBRIEF,MAC_RACH,\
            getCurrentTick(),\
            timerQueueNode_p->expTTI,\
            timerQueueNode_p->tcRNTI,\
            timerQueueNode_p->deleteFlag,\
            0,0,\
            0.0,0.0,\
            __func__,"CONTENT_RES_START_TIMER");


    if (pushNode(contentionTimerQueue_gp[internalCellIndex], &timerQueueNode_p->nodeAnchor))
    {
        LOG_MAC_MSG(L2_PUSH_QUEUE_NODE_FAIL_ID,LOGFATAL,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);

        ltePanic("pushNode failed for contentionTimerQueue_g");
    }
    LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
            "[%s]: Timer node successfully inserted into contentionTimerQueue_g list\n"
            "expTTI [%d] tcRNTI [%d]\n",
            __func__, timerQueueNode_p->expTTI, timerQueueNode_p->tcRNTI);

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);

    return timerQueueNode_p;
}
/*CA Changes end  */

/* +- SPR 22322 */

/*CA Changes end  */

/*****************************************************************************
 * Function Name  : createTempCRNTICtx
 * Inputs         : tcRnti - TCRNTI value
 *                  raQNode_p - Pointer to the RAReqQueueNode
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : Pointer to the TempCRNTICtx
 * Description    : This will creates the temporary context, sets the rntiFlag 
 *                  to TCRNTI and returnes the pointer to the created context
 *****************************************************************************/
TempCRNTICtx *createTempCRNTICtx(UInt16 tcRnti, RAReqQueueNode *raQNode_p,
        InternalCellIndex internalCellIndex,
        ExecutionLegFunctionArg* execArgs)
{
    RNTIInfo      *rnti_p     = PNULL;
    TempCRNTICtx  *tempNode_p = PNULL;
#ifdef TDD_CONFIG
    TempCRNTICtx *tempUEContext_p = PNULL;
#endif
    ULHarqInfo  *ulHarqInfo_p    = PNULL; 

    /* + coverity 111602 */
    UInt32 freeIndex  = tempCrntiCtx_gp[internalCellIndex]->freeIndex;
    /* - coverity 111602 */
    if (INVALID_FREE_INDEX == freeIndex)
    {
        LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                "[%s]: ERROR Maximum number of Temporary context reached\n",
                __func__);
        return PNULL;
    }

    rnti_p = getUeIdxFromRNTIMap(tcRnti, internalCellIndex);
    if ( PNULL == rnti_p )
    {
        LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                "[%s]: getUeIdxFromRNTIMap returned for tcrnti = %d \n",
                __func__, tcRnti);
        return PNULL;
    }

    /*Set free index to next free index*/
    tempCrntiCtx_gp[internalCellIndex]->freeIndex = 
        tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[freeIndex].nextFreeIndex;

    GET_MEM_FROM_POOL(TempCRNTICtx , tempNode_p , sizeof(TempCRNTICtx),PNULL);

    /* + Coverity 24501 */
    if( PNULL == tempNode_p )
    {
        return PNULL ;
    }
    /* - Coverity 24501 */

    tempNode_p->raRNTI = raQNode_p->raRNTI;
    tempNode_p->tcRNTI = tcRnti;
    tempNode_p->ttiRAReqReceived = raQNode_p->ttiRAReqReceived;
    tempNode_p->msg3BuffInfo.msg_p = PNULL;
    tempNode_p->timerNode_p = PNULL;
    tempNode_p->timingAdvanceAbs = raQNode_p->raPreambleInfo.timingAdvance;
    tempNode_p->lastTASentTick = execArgs->globalTick + PHY_UL_DATA_DELAY;
    tempNode_p->lastSyncDetectedTick = 0x0;
    /* SPR 23393 Changes Start */
    tempNode_p->status = RAR_SENT;
    /* SPR 23393 Changes End */

    tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[freeIndex].tempCRNTICtx_p = tempNode_p;

    rnti_p->rntiFlag = TCRNTI;
    rnti_p->index    = freeIndex;

	macTCrntiUeNum_g[internalCellIndex]++;

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[%s]: Temp UE context created successfully.\n"
            "      T-CRNTI [%d] RA-RNTI [%d]",
            __func__,tcRnti,tempNode_p->raRNTI);

    ulHarqInfo_p = &(tempNode_p->ulHarq); 
    ulHarqInfo_p->riStart  = 0; /*indicates the starting index of the resorces*/
    ulHarqInfo_p->riLength = 0;    /*indicates number of contigiously allocated RBs*/
    ulHarqInfo_p->riv      = 0;
    ulHarqInfo_p->ackNack  = 0;    /*indicates ACK or NACK (0 for NACK /1 for ACK) */
    ulHarqInfo_p->currentRetransCount = 0;/*indicates the retrans count for current TTI/HARQ*/
    ulHarqInfo_p->newDataIndicator    = 0;
    ulHarqInfo_p->txIndicator    =  0x0;
    ulHarqInfo_p->deltabsrLoad         = 0; /*UL_BSR_FIX*/
    ulHarqInfo_p->nextReTransmissionType = 0;
    ulHarqInfo_p->strictRBAllocation     = 0;
    ulHarqInfo_p->currentIrv = 0;
#ifdef FDD_CONFIG
    ulHarqInfo_p->harqTimerExpirySF = MAX_SUB_FRAME;
    ulHarqInfo_p->harqTimerExpirySystemFrame = MAX_SYS_FRAME_NUM;
    /* SPR 21958 PUSCH RAT1 Support Start */
    ulHarqInfo_p->startRbgIndexforSet1 = 0;
    ulHarqInfo_p->endRbgIndexforSet1 = 0;
    ulHarqInfo_p->startRbgIndexforSet2 = 0;
    ulHarqInfo_p->endRbgIndexforSet2 = 0; 
    ulHarqInfo_p->RAT1Allocation = 0;/* TRUE/FALSE */			 
    /* SPR 21958 PUSCH RAT1 Support End */
#endif
#ifdef ULHARQ_TIMER_PROC
    ulHarqInfo_p->ulHarqProcessId = 0;
#endif
#ifdef TDD_CONFIG
    tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[freeIndex].tempCRNTICtx_p;

    tempUEContext_p->ulUeScheduledInfoPointerArray.flag = UNSET_FLAG;
    tempUEContext_p->ulUeScheduledInfoPointerArray.ulschUciInfoIndex =
        ULSCH_UCI_INVALID_INDEX;
    tempUEContext_p->ulUeScheduledInfoPointerArray.ulCCEIndex = INVALID_CCE_INDEX;
    tempUEContext_p->ulUeScheduledInfoPointerArray.ulHarqProcessId = INVALID_HARQ_ID;
#endif
#ifdef FDD_CONFIG
    /* Initialize harq context for temporary UE context */
    createMACDLHarqEntity( &tempNode_p->dlHarqCtx_p, 1);
#endif
    return(tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[freeIndex].tempCRNTICtx_p);
}

/*****************************************************************************
 * Function Name  : deleteTempCRNTICtx
 * Inputs         : tcRNTI - Temporary CRNTI
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : None
 * Description    : This will free the Temporary UE context
 *****************************************************************************/
/*CA Changes start  */
void deleteTempCRNTICtx(UInt16 tcRNTI, InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{
    {
    RNTIInfo     *rntiInfo_p     = PNULL;
    TempCRNTICtx *tempCRNTICtx_p = PNULL;

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    /* SPR 16773 fix start */
    /* Search the create UE timer node in the timer list. This is done 
     * intentionally at entry level since this timer needs to stopped in both 
     * cases when Temp context is availabel or not. */
    LP_CreateUeContextTimerNode createUETimerNode_p =
        searchCreateUeTimer( tcRNTI ,internalCellIndex);
    /* Stop the create UE timer */
    stopCreateUeTimer( createUETimerNode_p,internalCellIndex );
    /* SPR 16773 fix end */
    /*CA Changes start  */
    rntiInfo_p =  getUeIdxFromRNTIMap(tcRNTI, internalCellIndex);
    /*CA Changes end  */
    if ((PNULL == rntiInfo_p) || 
            (FREERNTI == rntiInfo_p->rntiFlag) ||
            (MAX_UE_SUPPORTED == rntiInfo_p->index))
    {
        LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                "[%s]: WARNING: Temporary context not present\n",
                __func__);
        return;
    }
    else
    {
        tempCRNTICtx_p = 
            tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[rntiInfo_p->index].tempCRNTICtx_p;
        tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[rntiInfo_p->index].nextFreeIndex = 
            tempCrntiCtx_gp[internalCellIndex]->freeIndex;

        /*Set the main free index to the index*/
        tempCrntiCtx_gp[internalCellIndex]->freeIndex = rntiInfo_p->index;
    }

    if (tempCRNTICtx_p)
    {
        LTE_MAC_UT_LOG(LOG_INFO ,RACH_MGR,
                "[%s]: Deleting the Temp UE context. T-CRNTI [%d]\n",
                __func__,tcRNTI);


        /*Free the memory*/
        if (tempCRNTICtx_p->msg3BuffInfo.msg_p)
        {
            msgFree(tempCRNTICtx_p->msg3BuffInfo.msg_p);
            tempCRNTICtx_p->msg3BuffInfo.msg_p = PNULL;
        }
        /* SPR_4263_FIX Start */ 
        //suppose that crnti 0 is invalid.	
        tempCRNTICtx_p->msg3BuffInfo.crnti = 0;
        /* SPR_4263_FIX End */ 
#ifdef FDD_CONFIG
        /* + SPR 18279 */
        /* Free the DL HARQ memory for temporary context */
        freeMemPool( tempCRNTICtx_p->dlHarqCtx_p );
        /* - SPR 18279 */
#endif
        freeMemPool(tempCRNTICtx_p);
        tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[rntiInfo_p->index].tempCRNTICtx_p = PNULL;

        macTCrntiUeNum_g[internalCellIndex]--;
    }

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);

    return;
    }
}

/*****************************************************************************
 * Function Name  : getTempCRNTIContextPointer
 * Inputs         : tcRNTI - Temporary RNTI value
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : pointer to the structure TempCRNTICtx
 * Description    : This function will returns the Temporary UE context pointer
 *****************************************************************************/
TempCRNTICtx* getTempCRNTIContextPointer(UInt16 tcRNTI, InternalCellIndex internalCellIndex)
{
    RNTIInfo     *rntiInfo_p = PNULL;

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    /*CA Changes start  */
    rntiInfo_p =  getUeIdxFromRNTIMap(tcRNTI, internalCellIndex);
    /*CA Changes end  */
    if ((PNULL == rntiInfo_p) || 
            (TCRNTI != rntiInfo_p->rntiFlag) || 
            (MAX_UE_SUPPORTED == rntiInfo_p->index))
    {
        LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
                "[Exit] %s\n",__func__);
        return PNULL;
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
                "[Exit] %s\n",__func__);
        return(tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[rntiInfo_p->index].tempCRNTICtx_p);
    }
}

/*****************************************************************************
 * Function Name  : getUeIdxFromRNTIMap
 * Inputs         : rnti - RNTI value
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : RNTIInfo structure pointer
 * Description    : This API will return pointer to the RNTIInfo structure for 
 *                  the received rnti
 *****************************************************************************/
/*CA Changes start  */
RNTIInfo* getUeIdxFromRNTIMap(UInt16 rnti, InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{
    {
    UInt8 isSpsCrnti = FALSE;
    /*CA Changes start  */
    InitParams       *initParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
  /*CA Changes end  */
    /*If RNTI is not in the range initialized by OAM*/
    /*SPS RNTI FIX Start*/
    /*CA Changes start  */
    if((TRUE == initParams_p->spsSysWideParam.enableSps) &&
            ( rnti >= cellParams_p->spsCommonCellInfo.startSpsCRnti &&
              rnti <= cellParams_p->spsCommonCellInfo.endSpsCRnti))
        isSpsCrnti = TRUE;
    /*SPS RNTI FIX End*/

    if ((FALSE == isSpsCrnti)&&(((rnti < initParams_p->lowerRNTIBound) || 
                    (rnti > initParams_p->upperRNTIBound)) ||
                ((rnti >= cellParams_p->startRARNTI) && 
                 (rnti <= cellParams_p->endRARNTI))))
    {
        return  PNULL;
    }
    else if ((TRUE == isSpsCrnti)&&
            (((rnti < cellParams_p->spsCommonCellInfo.startSpsCRnti) ||
              (rnti > cellParams_p->spsCommonCellInfo.endSpsCRnti)) || 
             ((rnti >= cellParams_p->startRARNTI) &&
              (rnti <= cellParams_p->endRARNTI))))
    {
        LOG_MAC_MSG (MAC_RECV_INVALID_RNTI_ID, LOGERROR, MAC_PDCCH,\
                getCurrentTick(),\
                rnti,\
                0,\
                0,\
                0,\
                0,\
                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                __func__, "INVALID_RNTI_RECV");

        return  PNULL;
    }
    else
    {
        if (isSpsCrnti == TRUE)
        {
            return(&crntiToIdxTbl_g[internalCellIndex][(rnti - 
                        cellParams_p->spsCommonCellInfo.startSpsCRnti + 
                        initParams_p->crntiRange)]);
        }
        return(&crntiToIdxTbl_g[internalCellIndex][(rnti - initParams_p->lowerRNTIBound)]);
        /*CA Changes end  */
    }
}
}

/* SPR 16773 fix start */
/*****************************************************************************
 * Function Name  : startCreateUeTimer
 * Inputs         : expTTI - Expiry tti
 *                  tcRNTI - TCRNTI
 * Outputs        : None
 * Returns        : void
 * Description    : This will start the timer for Create UE context
 *****************************************************************************/
void startCreateUeTimer( UInt32 expTTI, UInt16 tcRNTI,InternalCellIndex internalCellIndex )
{
    LP_CreateUeContextTimerNode createUeListNode_p = PNULL;

    /* Allocate the memory for Create UE timer node */
    createUeListNode_p = (LP_CreateUeContextTimerNode)getMemFromPool(
            sizeof(CreateUeContextTimerNode), PNULL );
    if( PNULL == createUeListNode_p )
    {
        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                getCurrentTick(), __LINE__, sizeof(CreateUeContextTimerNode), 
                0,0,0, 0,0, __FILE__, __func__);
        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
    }
    else
    {
        createUeListNode_p->expTTI = expTTI;
        createUeListNode_p->tcRNTI = tcRNTI;

        LOG_MAC_MSG( MAC_TCRNTI_START_TIMER_ID, LOGBRIEF, MAC_RACH,
                getCurrentTick(), createUeListNode_p->expTTI,
                createUeListNode_p->tcRNTI, 0, 0,0,
                0, 0, __func__, "createUeCtxTimerList_g" );

        /* Insert the node in list */
        listInsertNode(createUeContextTimerList_gp[internalCellIndex], &createUeListNode_p->nodeAnchor);
    }
}

/*****************************************************************************
 * Function Name  : searchCreateUeTimer
 * Inputs         : CRNTI
 * Outputs        : None
 * Returns        : createUeListNode_p - Pointer to CreateUeContextTimerNode
 * Description    : This will search the timer node based on CRNTI for Create 
 *                  UE timer on receiving MAC_CREATE_UE_ENTITY_REQ
 *****************************************************************************/
LP_CreateUeContextTimerNode searchCreateUeTimer( UInt16 rnti,InternalCellIndex internalCellIndex )
{
    UInt8 nodeFound = LTE_FALSE;
    LP_CreateUeContextTimerNode createUeListNode_p = PNULL;
    UInt32 count = listCount(createUeContextTimerList_gp[internalCellIndex]);

    createUeListNode_p = (LP_CreateUeContextTimerNode)getFirstListNode(
            createUeContextTimerList_gp[internalCellIndex]);

    while( createUeListNode_p && count-- )
    {
        if( createUeListNode_p->tcRNTI == rnti )
        {
            nodeFound = LTE_TRUE;
            break;
        }
        else
        {
            createUeListNode_p = (LP_CreateUeContextTimerNode) getNextListNode(
                    &createUeListNode_p->nodeAnchor);
        }
    }
    if( LTE_FALSE == nodeFound )
    {
        createUeListNode_p = PNULL;
    }
    return createUeListNode_p;
}

/*****************************************************************************
 * Function Name  : stopCreateUeTimer
 * Inputs         : createUeListNode_p - Pointer to CreateUeContextTimerNode
 * Outputs        : None
 * Returns        : Nonue
 * Description    : This will stop the timer for Create UE timer on receiving
 *                  MAC_CREATE_UE_ENTITY_REQ
 *****************************************************************************/
void stopCreateUeTimer( LP_CreateUeContextTimerNode createUeListNode_p,InternalCellIndex internalCellIndex )
{
    if( PNULL != createUeListNode_p )
    {    
        LOG_MAC_MSG( PREAMBLE_TIMER_STOP_ID, LOGDEBUG, MAC_RACH,
                getCurrentTick(), createUeListNode_p->tcRNTI,
                createUeListNode_p->expTTI, __LINE__, 0,0, 0,0,
                __func__, "CreateUe");

        listDeleteNode( createUeContextTimerList_gp[internalCellIndex],
                &createUeListNode_p->nodeAnchor);

        freeMemPool( createUeListNode_p );
        createUeListNode_p = PNULL;
    }
    else
    {
        LOG_MAC_MSG( PREAMBLE_TIMER_STOP_ID, LOGDEBUG, MAC_RACH,
                getCurrentTick(), 0,0, __LINE__, 0,0, 0,0,
                __func__, "CreateUeNull");
    }
}

/*****************************************************************************
 * Function Name  : processCreateUETimerList
 * Inputs         : currentTick
 *                  count - number of nodes in queue
 * Outputs        : None
 * Returns        : None
 * Description    : This will delete TCRNTI on timer expiry
 *****************************************************************************/
void processCreateUETimerList( UInt32 currentTick, UInt32 count,InternalCellIndex internalCellIndex )
{
    LP_CreateUeContextTimerNode createUeListNode_p = PNULL;
    /* SPR 20829 Changes Start */
#ifdef LOG_PRINT_ENABLED
    LP_CreateUeContextTimerNode prevUeListNode_p = PNULL;
#endif
    /* SPR 20829 Changes End */

    RNTIInfo *rntiInfo_p = PNULL;
    UInt16 tcrnti = INVALID_RNTI;

    createUeListNode_p = (LP_CreateUeContextTimerNode)getFirstListNode(
            createUeContextTimerList_gp[internalCellIndex]);

    while( createUeListNode_p && count-- )
    {
        /* check if the timer has expired */
        if( createUeListNode_p->expTTI <= currentTick)
        {
            tcrnti = createUeListNode_p->tcRNTI; 
            LOG_MAC_ERROR(MAC_RRC_INF,"Should Not Happen Expiry of CreateUETimer tcrnti %u expTTI %u Tick %llu\n",
                    tcrnti,createUeListNode_p->expTTI,getCurrentTick());

            /* SPR 20829 Changes Start */
#ifdef LOG_PRINT_ENABLED
            prevUeListNode_p = createUeListNode_p;
#endif
            createUeListNode_p = (LP_CreateUeContextTimerNode) getNextListNode(
                    &createUeListNode_p->nodeAnchor);
            /* SPR 20829 Changes End */

            /* delete the TCRNTI context and release the Timer context */
            deleteTempCRNTICtx(tcrnti,internalCellIndex);

            rntiInfo_p = getUeIdxFromRNTIMap( tcrnti,internalCellIndex );
            if ( PNULL == rntiInfo_p ) 
            {
                LOG_MAC_MSG( MAC_TCRNTI_RNTIINFO_NULL_ID, LOGERROR, MAC_RACH,
                        getCurrentTick(), tcrnti, __LINE__, 0,0,0,
                        0,0, __func__, "UeCreateTimer");
            }
            else
            {
                /* SPR 20829 Changes Start */
                LOG_MAC_MSG( MAC_TCRNTI_TIMER_EXPIRY_ID, LOGINFO, MAC_RACH,
                        getCurrentTick(), prevUeListNode_p->expTTI, tcrnti, 
                        rntiInfo_p->rntiFlag, 0,0, 
                        0,0, __func__, "UeCreateTimer");
                /* SPR 20829 Changes End */

                if( TCRNTI == rntiInfo_p->rntiFlag )
                {
                    /* Free the rnti */
                    if(MAC_FAILURE == freeRNTI(tcrnti,internalCellIndex))
                    {
                        LOG_MAC_MSG( MAC_TCRNTI_FAIL_ID, LOGWARNING, MAC_RACH,
                                getCurrentTick(), tcrnti, __LINE__, 0,0,0,
                                0,0, __func__,"FreeRntiFail");
                    }
                }
            }
        
            /* SPR 20829 Changes Start */
            continue;
            /* SPR 20829 Changes End */
        }
        createUeListNode_p = (LP_CreateUeContextTimerNode) getNextListNode(
                &createUeListNode_p->nodeAnchor);
    }
}
/* SPR 16773 fix end */

/*****************************************************************************
 * Function Name  : getFreeTempRNTI
 * Inputs         : internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : RNTI value from the poped queue node
 * Description    : This will pop the first queue node from the 
 *                  free RNTI pool and returns the RNTI value
 *****************************************************************************/
/*CA Changes start  */
UInt16 getFreeTempRNTI(InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{
    UInt16 rnti = INVALID_RNTI;
    FreeRNTIPoolNode *freeRNTIPoolNode_p = PNULL;
    UInt64 macCreateUeTimerExpiry = MAC_CREATE_UE_TIMER_EXPIRY;


    LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
            "Entry [%s]\n",__func__);

    /* Get free temp CRNTI from freeTempCRNTIQueue */ 
    if(PNULL != 
            /*CA Changes start  */
            (freeRNTIPoolNode_p = (FreeRNTIPoolNode *)popNode(freeRNTIQueue_gp[internalCellIndex])))
        /*CA Changes end  */
    {
        rnti = freeRNTIPoolNode_p->rnti;

        freeMemPool(freeRNTIPoolNode_p);
    }
    else if (PNULL != 
            (freeRNTIPoolNode_p = 
             /*CA Changes start  */
             (FreeRNTIPoolNode *)popNode(freeRNTIQueueAfterUEDeletion_gp[internalCellIndex])))
        /*CA Changes end  */
    {
        rnti = freeRNTIPoolNode_p->rnti;

        freeMemPool(freeRNTIPoolNode_p);
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
                "[%s]: Free RNTI not available\n",__func__);
    }
    /* SPR 16766 fix start */
    LOG_MAC_MSG( MAC_TCRNTI_ALLOC, LOGINFO, MAC_RACH,
            getCurrentTick(), __LINE__, rnti, 0,0,0, 0,0,
            __func__, "" );
    /* SPR 16766 fix end */
    /* SPR 16773 fix start */
    /* Start the timer for deleting the UE context only if valid RNTI is 
     * returned */
    if( INVALID_RNTI != rnti )
    {
        startCreateUeTimer( ( getCurrentTick() + macCreateUeTimerExpiry ), 
                rnti,internalCellIndex );
    }
    /* SPR 16773 fix end */

    LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
            "Exit [%s]\n",__func__);
    return rnti;
}

/*****************************************************************************
 * Function Name  : freeRNTI
 * Inputs         : rnti - RNTI value
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This will return rnti to the free RNTI pool.
 *****************************************************************************/
/*CA Changes start  */
MacRetType freeRNTI(UInt16 rnti, InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{
    RNTIInfo         *rnti_p = PNULL;
    FreeRNTIPoolNode *freeIdxNode_p = PNULL;
    /* SPR 15487 fix start */
    InitParams *initParams_p =
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;         

    LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
            "Entry [%s]\n",__func__);

    UInt16 lowerRNTIBound = 0;
    UInt16 upperRNTIBound = 0;
    UInt16 startRARNTI    = 0;
    UInt16 endRARNTI      = 0;
    UInt16 index          = 0;

    {
        lowerRNTIBound = initParams_p->lowerRNTIBound;
        upperRNTIBound = initParams_p->upperRNTIBound;
        startRARNTI    = cellParams_p->startRARNTI;
        endRARNTI      = cellParams_p->endRARNTI;
        index          = MAX_UE_SUPPORTED;
    }

    if (((rnti < lowerRNTIBound) || (rnti > upperRNTIBound)) ||
            ((rnti >= startRARNTI) && (rnti <= endRARNTI)))
    {
        LOG_MAC_MSG (MAC_RECV_INVALID_RNTI_ID, LOGERROR, MAC_PDCCH,
                getCurrentTick(), rnti, 0, 0, 0, 0, 0, 0,
                __func__, "INVALID_RNTI_RECV");

        return  MAC_FAILURE;
    }
    /* SPR 15487 fix end */

    /*CA Changes start  */
    rnti_p = getUeIdxFromRNTIMap(rnti, internalCellIndex);
    /*CA Changes end  */

    if ((PNULL == rnti_p) || 
            (MAX_UE_SUPPORTED == rnti_p->index) ||
            (FREERNTI == rnti_p->rntiFlag))
    {
        /* SPR 16766 fix start */
        LOG_MAC_MSG (MAC_RECV_INVALID_RNTI_ID, LOGERROR, MAC_PDCCH,
                getCurrentTick(), rnti, 0, 0, 0, 0, 0, 0,
                __func__, "FreeRntiFlag");
        /* SPR 16766 fix end */

        return MAC_FAILURE;
    }
			
	if (CRNTI == rnti_p->rntiFlag)
	{
		macCrntiUeNum_g[internalCellIndex]--;
	}

    rnti_p->rntiFlag = FREERNTI;
    rnti_p->index    = index;

    /* Put temp CRNTI to freeRNTIQueue_g */
    GET_MEM_FROM_POOL(FreeRNTIPoolNode , freeIdxNode_p , sizeof(FreeRNTIPoolNode) ,PNULL);

    /* + Coverity 24502 */
    if( PNULL == freeIdxNode_p )
    {
        return MAC_FAILURE ;
    }
    /* - Coverity 24502 */

    freeIdxNode_p->rnti = rnti;

    /*CA Changes start  */
    if (pushNode(freeRNTIQueue_gp[internalCellIndex], &(freeIdxNode_p->nodeAnchor)))
        /*CA Changes end  */
    {
        /* SPR 16766 fix start */
        LOG_MAC_MSG (MAC_RECV_INVALID_RNTI_ID, LOGERROR, MAC_PDCCH,
                getCurrentTick(), rnti, 0, 0, 0, 0, 0, 0,
                __func__, "PushFail");
        /* SPR 16766 fix end */
        return MAC_FAILURE;
    }

    /* SPR 16766 fix start */
    LOG_MAC_MSG( MAC_TCRNTI_DELETE_ID, LOGINFO, MAC_RACH,
            getCurrentTick(), 0, rnti, 0,0,0, 0,0,
            __func__, "FreeRnti" );
    /* SPR 16766 fix end */

    LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
            "Exit [%s]\n",__func__);


    return MAC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : freeRNTIAfterUEDeletion
 * Inputs         : rnti - RNTI value
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This will return rnti to the free RNTI pool.
 *****************************************************************************/
/*CA Changes start  */
MacRetType freeRNTIAfterUEDeletion(UInt16 rnti, InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{
    RNTIInfo         *rnti_p = PNULL;
    FreeRNTIPoolNode *freeIdxNode_p = PNULL;
    /* SPR 15487 fix start */
    InitParams *initParams_p =
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;         

    LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
            "Entry [%s]\n",__func__);

    UInt16 lowerRNTIBound = 0;
    UInt16 upperRNTIBound = 0;
    UInt16 startRARNTI    = 0;
    UInt16 endRARNTI      = 0;
    UInt16 index          = 0;

    {
        lowerRNTIBound = initParams_p->lowerRNTIBound;
        upperRNTIBound = initParams_p->upperRNTIBound;
        startRARNTI    = cellParams_p->startRARNTI;
        endRARNTI      = cellParams_p->endRARNTI;
        index          = MAX_UE_SUPPORTED;
    }

    if (((rnti < lowerRNTIBound) || (rnti > upperRNTIBound)) ||
            ((rnti >= startRARNTI) && (rnti <= endRARNTI)))
    {
        LOG_MAC_MSG (MAC_RECV_INVALID_RNTI_ID, LOGERROR, MAC_PDCCH,
                getCurrentTick(), rnti, 0, 0, 0, 0, 0, 0,
                __func__, "INVALID_RNTI_RECV");

        return  MAC_FAILURE;
    }
    /* SPR 15487 fix end */

    /*CA Changes start  */
    rnti_p = getUeIdxFromRNTIMap(rnti, internalCellIndex);
    /*CA Changes end  */

    if ((PNULL == rnti_p) || 
            (MAX_UE_SUPPORTED == rnti_p->index) ||
            (FREERNTI == rnti_p->rntiFlag))
    {
        /* SPR 16766 fix start */
        LOG_MAC_MSG (MAC_RECV_INVALID_RNTI_ID, LOGERROR, MAC_PDCCH,
                getCurrentTick(), rnti, 0, 0, 0, 0, 0, 0,
                __func__, "FreeRntiFlag");
        /* SPR 16766 fix end */

        return MAC_FAILURE;
    }

	if (CRNTI == rnti_p->rntiFlag)
	{
		macCrntiUeNum_g[internalCellIndex]--;
	}

    rnti_p->rntiFlag = FREERNTI;
    rnti_p->index    = index;

    /* Put temp CRNTI to freeRNTIQueueAfterUEDeletion_g */
    GET_MEM_FROM_POOL(FreeRNTIPoolNode , freeIdxNode_p , sizeof(FreeRNTIPoolNode),PNULL);

    /* + Coverity 24503 */
    if( PNULL == freeIdxNode_p )
    {
        return MAC_FAILURE ;
    }
    /* - Coverity 24503 */

    freeIdxNode_p->rnti = rnti;

    /*CA Changes start  */
    if (pushNode(freeRNTIQueueAfterUEDeletion_gp[internalCellIndex], 
                /*CA Changes end  */
                &(freeIdxNode_p->nodeAnchor)))
    {
        /* SPR 16766 fix start */
        LOG_MAC_MSG (MAC_RECV_INVALID_RNTI_ID, LOGERROR, MAC_PDCCH,
                getCurrentTick(), rnti, 0, 0, 0, 0, 0, 0,
                __func__, "PushFail");
        /* SPR 16766 fix end */
        /* + Coverity 32476 */ 
        freeMemPool(freeIdxNode_p);
        /* - Coverity 32476 */ 
        return MAC_FAILURE;
    }
    /* SPR 16766 fix start */
    LOG_MAC_MSG( MAC_TCRNTI_DELETE_ID, LOGWARNING, MAC_RACH,
            getCurrentTick(), 0, rnti, 0,0,0, 0,0,
            __func__, "FreeRnti After UE deletion");
    /* SPR 16766 fix end */

    LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
            "Exit [%s]\n",__func__);

    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : freeRNTIBeforeUETempContextCreation
 * Inputs         : rnti - RNTI value
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This will return rnti to the free RNTI pool.
 *****************************************************************************/
/*CA Changes start  */
MacRetType freeRNTIBeforeUETempContextCreation(UInt16 rnti, InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{
    FreeRNTIPoolNode *freeIdxNode_p = PNULL;
    InitParams *initParams_p =
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    CellConfigParams *cellParams_p = 
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    UInt16 lowerRNTIBound = 0;
    UInt16 upperRNTIBound = 0;
    UInt16 startRARNTI    = 0;
    UInt16 endRARNTI      = 0;

    {
        lowerRNTIBound = initParams_p->lowerRNTIBound;
        upperRNTIBound = initParams_p->upperRNTIBound;
        startRARNTI    = cellParams_p->startRARNTI;
        endRARNTI      = cellParams_p->endRARNTI;
    }
    /* SPR 14117 fix start */
    if (((rnti < lowerRNTIBound) || (rnti > upperRNTIBound)) ||
            ((rnti >= startRARNTI) && (rnti <= endRARNTI)))
    {
        LOG_MAC_MSG (MAC_RECV_INVALID_RNTI_ID, LOGERROR, MAC_PDCCH,\
                getCurrentTick(),\
                rnti,\
                0,\
                0,\
                0,\
                0,\
                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                __func__, "INVALID_RNTI_RECV");

        return  MAC_FAILURE;
    }
    /* SPR 14117 fix end */


    /* Put free RNTI to freeRNTIQueue_g */
    GET_MEM_FROM_POOL(FreeRNTIPoolNode , freeIdxNode_p , sizeof(FreeRNTIPoolNode),PNULL);

    /* + Coverity 24504 */
    if( PNULL == freeIdxNode_p )
    {
        return MAC_FAILURE ;
    }
    /* - Coverity 24504 */

    freeIdxNode_p->rnti = rnti;
    /* + Coverity 32477 */
    /*CA Changes start  */
    if (pushNode(freeRNTIQueue_gp[internalCellIndex], &(freeIdxNode_p->nodeAnchor)))
        /*CA Changes end  */
    {
        /* SPR 16766 fix start */
        LOG_MAC_MSG (MAC_RECV_INVALID_RNTI_ID, LOGERROR, MAC_PDCCH,
                getCurrentTick(), rnti, 0, 0, 0, 0, 0, 0,
                __func__, "PushFail");
        /* SPR 16766 fix end */
        freeMemPool(freeIdxNode_p);
        return MAC_FAILURE;
    }
    /* - Coverity 32477 */
    /* SPR 16766 fix start */
    LOG_MAC_MSG( MAC_TCRNTI_DELETE_ID, LOGINFO, MAC_RACH,
            getCurrentTick(), 0, rnti, 0,0,0, 0,0,
            __func__, "freeRNTIBeforeUETempContextCreation");
    /* SPR 16766 fix end */

    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : handleCRNTIMACCEReceived
 * Inputs         : tempUEContext_p - Pointer to the TempCRNTICtx
 *                  tcRNTI - Temporary CRNTI
 *                  dlDelay - DL delay
 *                  receivedTTI -
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This will check contention resolution timer and push the 
 *                  contention message for retransmission if the contention 
 *                  resolution timer expired 
 *****************************************************************************/
/*CA Changes start  */
MacRetType handleCRNTIMACCEReceived(TempCRNTICtx *tempUEContext_p,
        UInt16 tcRNTI,
        UInt32 dlDelay,
        /* SPR 15909 fix start */
        tickType_t receivedTTI,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex,
        ExecutionLegFunctionArg* execArgs)
{
    ULGrantRequestQueueNode *ulGrantRequestQueueNode_p = PNULL;
    UInt32 sfn = 0;
    /* SPR 15909 fix start */
    tickType_t tti = 0;
    /* SPR 15909 fix end */
    UInt32 contentionResTimerVal = 0;
    UInt32 activeIndex    = 0;
    RNTIInfo *rntiInfo_p = PNULL;
    UInt16 ueIndex = 0;
    UInt16 preambleIndex = INVALID_PREAMBLE_IDX;
    ContentionFreePreamble *contFreePreamble_p = PNULL;
    DLUEContext *ueDLContext_p  = PNULL;
    ULUEContext *ulUEContext_p= PNULL; /*Power Control*/
    /* SPR 3271 changes start */
    UInt32 n = 0;
    /* SPR 3271 changes end */
    /* + E_CID_5.3 */
    UInt8 numOfAntenna = 0;
    /* - E_CID_5.3 */
    /* + PERF_STATS_PH1D1 */

    /*CA Changes start  */
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /*CA Changes end  */

#ifdef PERF_STATS
    /* + PERF_CA */
    LteCellMacULPerfStats *lteCellMacULPerfStats = &(gMacCellPerfStats_p[internalCellIndex]-> \
            lteCellMacULPerfStats);
    /* - PERF_CA */
#endif
    /* - PERF_STATS_PH1D1 */

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    /*CA Changes start  */
    rntiInfo_p = getUeIdxFromRNTIMap(tempUEContext_p->msg3BuffInfo.crnti, internalCellIndex);
    /*CA Changes end  */
    if ((PNULL == rntiInfo_p)||
            (CRNTI != rntiInfo_p->rntiFlag) ||
            ((TRUE == dlUECtxInfoArr_g[rntiInfo_p->index].pendingDeleteFlag)&&
             /* SPR 3043 start */
             /*SPR 20665 +*/
             ((!(dlUECtxInfoArr_g[rntiInfo_p->index].dlUEContext_p)) || 
              /*SPR 20665 -*/
              (TRUE != dlUECtxInfoArr_g[rntiInfo_p->index].dlUEContext_p->tcrntiFlag))))
            /* SPR 3043 end */
    {
        LTE_MAC_UT_LOG(LOG_WARNING, RACH_MGR,
                "[%s]: WARNING: UE Context not present RNTI [%d]\n",
                __func__,tempUEContext_p->msg3BuffInfo.crnti);

        return MAC_FAILURE;
    }

    ueIndex = rntiInfo_p->index;

    ueDLContext_p = 
        dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

    if(PNULL == ueDLContext_p)
    {
        LOG_MAC_MSG(NO_HO_DL_UE_CONTEXT_ID,
                LOGERROR,
                MAC_RACH,
                getCurrentTick(),
                ueIndex,
                tcRNTI,
                LINE_NUMBER,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"NO_HO_DL_UE_CONTEXT_ID");

        return MAC_FAILURE;

    }


    /*For Power Control*/
    ulUEContext_p =  ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
    if(PNULL == ulUEContext_p)
    {
        lteWarning("ulUEContext_p is NULL\n");
        return MAC_FAILURE;
    }
    /* + E_CID_5.3 */
    if(NO_REPORT_TYPE_REQUIRED != ulUEContext_p->eCidReportParams.typeofReportsRequired)
    {
        ulUEContext_p->eCidReportParams.type1TAValue = tempUEContext_p->type1TAValue;
        ulUEContext_p->eCidReportParams.type1TaAOAReceivedTTI = 
            tempUEContext_p->type1TaAOAReceivedTTI;
        ulUEContext_p->eCidReportParams.type2TAValue = tempUEContext_p->type2TAValue;
        ulUEContext_p->eCidReportParams.type2TAreceivedTTI = 
            tempUEContext_p->type2TAreceivedTTI;
        for (numOfAntenna  = 0; numOfAntenna < MAX_NUM_ANTENNA; numOfAntenna ++)
            ulUEContext_p->eCidReportParams.angleOfArrival[numOfAntenna] = 
                tempUEContext_p->angleOfArrival[numOfAntenna];

        /*Check all reports stored in are within valid range or not
         * If all reports requested by RRM are valid send reports with SUCCESS
         * else if some of the reports are valid send reports with PARTIAL SUCCESS
         * else send FAILURE response if all reports are invalid */
        tempUEContext_p->validReports |= ((MAX_TIMING_ADVANCE_R9_VALUE >= tempUEContext_p->type2TAValue )? TRUE:FALSE) << 1;
        tempUEContext_p->validReports &= ulUEContext_p->eCidReportParams.typeofReportsRequired;
        /* if Type1 TA is requested by RRM along with Type2
         * and/or AoA then L2 has to wait for 120ms to fetch
         * TA1 report even if Msg3 not received 
         *
         * If Type1 is not requested by RRM then here L2 send
         * available reports to RRM */
        if(!(ulUEContext_p->eCidReportParams.typeofReportsRequired & TYPE_1_TA_REPORT_REQUIRED)) 
        {
            if( ulUEContext_p->eCidReportParams.typeofReportsRequired == tempUEContext_p->validReports)
            {
                if(WAIT_FOR_TA_2_REPORT_ONLY_FOR_RELEASE_8_UE == \
                        ulUEContext_p->eCidReportParams.reportStatus)
                {
                    ulUEContext_p->eCidReportParams.type1TAValue = INVALID_TIMING_ADVANCE_R9_VALUE;
                    for (numOfAntenna  = 0; numOfAntenna < MAX_NUM_ANTENNA; numOfAntenna ++)
                        ulUEContext_p->eCidReportParams.angleOfArrival[numOfAntenna] = INVALID_ANGLE_OF_ARRIVAL_VALUE;
                    putEntryInEcidMeasReportQueue(ueIndex,\
                            ulUEContext_p->eCidReportParams.typeofReportsRequired, \
                            MAC_PARTIAL_SUCCESS, \
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */
                }
                else
                {
                    putEntryInEcidMeasReportQueue(ueIndex, \
                            ulUEContext_p->eCidReportParams.typeofReportsRequired, \
                            MAC_SUCCESS, \
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */
                }
            }
            else if(tempUEContext_p->validReports)
            {
                putEntryInEcidMeasReportQueue(ueIndex, \
                        ulUEContext_p->eCidReportParams.typeofReportsRequired, \
                        MAC_PARTIAL_SUCCESS, \
                        /*CA Changes start  */
                        internalCellIndex);
                /*CA Changes end  */
            }
            else
            {
                putEntryInEcidMeasReportQueue(ueIndex, NO_REPORT_TYPE_REQUIRED, \
                        /*CA Changes start  */
                        MAC_FAILURE, internalCellIndex);
                /*CA Changes end  */
            }
            ulUEContext_p->eCidReportParams.reportStatus = REPORTS_RECEIVED;
            ulUEContext_p->eCidReportParams.typeofReportsRequired = 
                NO_REPORT_TYPE_REQUIRED;
        }
        else
        {
            ulUEContext_p->eCidReportParams.reportStatus = 
                WAIT_FOR_REPORTS_TA_1_OR_AOA_AFTER_TA_2_RECEIVED;

        }
    }
    /* - E_CID_5.3 */
    sfn = getSysFrameNumer(internalCellIndex);

    /*CA Changes start  */
    UPDATE_CONTENTION_RES_TIMER_ACTIVE_INDEX(cellParams_p->rachConfigInfo, sfn);

    activeIndex = cellParams_p->rachConfigInfo.rachConfiguration.activeIdxContentionRes;

    contentionResTimerVal = 
        cellParams_p->rachConfigInfo.rachConfiguration.macContentionResTimer[activeIndex];
    /*CA Changes end  */

    /*Prepare UL grant for Contention Resolution and push to queue*/
    GET_MEM_FROM_POOL(ULGrantRequestQueueNode , ulGrantRequestQueueNode_p,sizeof(ULGrantRequestQueueNode),PNULL);

    /* + Coverity 24506 */
    if( PNULL == ulGrantRequestQueueNode_p )
    {
        return MAC_FAILURE ;
    }
    /* - Coverity 24506 */

    tti = execArgs->globalTick;

    ulGrantRequestQueueNode_p->ueIndex = ueIndex;
    ulGrantRequestQueueNode_p->ttiCounter = (tti + contentionResTimerVal - 
            (PHY_UL_DATA_DELAY + dlDelay + PHY_DL_DELAY));
    ulGrantRequestQueueNode_p->ulGrantRequestType = CR_ULGRANT_REQ_MSG;

    /*CA Changes start  */
    if (pushNode(ulGrantContentionResQueue_gp[internalCellIndex], 
                &ulGrantRequestQueueNode_p->ulGrantRequestNodeAnchor))
        /*CA Changes end  */
    {
        LTE_MAC_UT_LOG(LOG_WARNING, RACH_MGR,
                "[%s]: ERROR: pushNode() Failed\n",
                __func__);

        freeMemPool(ulGrantRequestQueueNode_p);

        LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
                "[Exit] %s\n",__func__);

        return MAC_FAILURE;
    }
    /* SPR 4871 Fix Start */
    /*SPR_3065_FIX*/
    /* sendSyncIndToRRC(ueIndex, ueDLContext_p->crnti, IN_SYNC_AFTER_OUT_OF_SYNC); */
    /*SPR_3065_FIX*/
    /* SPR 4871 Fix End */
    tempUEContext_p->status = MSG3_RECEIVED;

    /* Temporary Context timer is removed for FDD*/
    /* As Temporary Context timer is removed for TDD also in SPR 22322, so no need to update
     * deleteFlag here, as TCRNTI timer was not started while sending grant for Msg3*/
    /* SES-514 fix +- */
    
    preambleIndex = 
        ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx;

    if(INVALID_PREAMBLE_IDX != preambleIndex)
    {    
        if(PREAMBLE_BUSY == 
                /*CA Changes start  */
                getPreambleStatusInfo(preambleIndex, internalCellIndex))
            /*CA Changes end  */
        {
            contFreePreamble_p = 
                /*CA Changes start  */
                getContentionFreePreambleInfo(preambleIndex, internalCellIndex);
            /*CA Changes end  */

            if(PDCCH_ORDER == 
                    contFreePreamble_p->pdcchOrderHandOverFlag)
            {
                /* CA_pahse2_ulSync_code Start */
                if(IS_PCELL_OF_UE(ulUEContext_p ,internalCellIndex))
                {
                    /* CA_pahse2_ulSync_code End */
                    /*CA Changes start  */
                    putEntryInUlSyncLossOnOffTriggerQ(
                            ueIndex,UL_SYNC_LOSS_OFF, internalCellIndex);
                    /*CA Changes end  */
                    ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState = UL_SYNC_LOSS_OFF;
                    ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx = INVALID_PREAMBLE_IDX;
                    ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderSentCount = 0;

                    /*CA Changes start  */
                    stopConFreePreambleTimer(ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderTimerNode, internalCellIndex);
                    /*CA Changes end  */
                    ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderTimerNode = PNULL; 
                    /*SPR_3065_FIX*/
                    sendSyncIndToRRC(ueIndex, ueDLContext_p->crnti,
                            /*CA Changes start  */
                            IN_SYNC_AFTER_OUT_OF_SYNC, internalCellIndex);
                    /*CA Changes end  */
                    /*SPR_3065_FIX*/

                    /* CA_pahse2_ulSync_code Start */
                }
                /* CA_pahse2_ulSync_code End */

                /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                lteCellMacULPerfStats->totalRcvdMsg3RandomAccessPreamblesCBRA++;
#endif
                /* - PERF_STATS_PH1D1 */

                LOG_MAC_MSG(MAC_UL_SYNC_ON,LOGINFO,MAC_PDCCH,
                        getCurrentTick(),
                        ueDLContext_p->timeAlignmentTimer,
                        ueDLContext_p->lastTASentTick,
                        ueDLContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,
                        ueDLContext_p->ueIndex,LINE_NUMBER,
                        DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"CFR UL_SYNC_ON UE");


            }
            /*CA Changes start  */
            freePreambleIndex(preambleIndex, internalCellIndex);
            /*CA Changes end  */
        }
    }

    /* For PDCCH Order in case of failure of allocation of free preamble,preambleIndex 
     * zero is sent to UE and UE DL Context is updated with Invalid preamble
     * Index. Now if msg3 is recieved with CRNTI CE and Uplink sync
     * loss is on then it implies Contention Based RACH was triggered for PDCCH Order */


    else if(UL_SYNC_LOSS_ON ==
            ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState) 
    {
        /* CA_pahse2_ulSync_code Start */
        if(IS_PCELL_OF_UE(ulUEContext_p ,internalCellIndex))
        {
            /* CA_pahse2_ulSync_code End */
            /*CA Changes start  */
            putEntryInUlSyncLossOnOffTriggerQ(
                    ueIndex,UL_SYNC_LOSS_OFF, internalCellIndex);
            /*CA Changes end  */

            ulUEContext_p->ulSyncLossOrHOInfoInUL.ulSyncLossState = UL_SYNC_LOSS_OFF;
            ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx = INVALID_PREAMBLE_IDX;
            ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderSentCount = 0;

            /*CA Changes start  */
            stopConBasedPdcchOrderTimer(ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderTimerNode, internalCellIndex);
            /*CA Changes end  */
            ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderTimerNode = PNULL;
            /*SPR_3065_FIX*/
            sendSyncIndToRRC(ueIndex, ueDLContext_p->crnti,
                    /*CA Changes start  */
                    IN_SYNC_AFTER_OUT_OF_SYNC, internalCellIndex);
            /*CA Changes end  */


            /* CA_pahse2_ulSync_code Start */
        }
        /* CA_pahse2_ulSync_code End */


        /*SPR_3065_FIX*/
        /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
        lteCellMacULPerfStats->totalRcvdMsg3RandomAccessPreamblesCBRA++;
#endif
        /* - PERF_STATS_PH1D1 */

        LOG_MAC_MSG(MAC_UL_SYNC_ON,LOGINFO,MAC_PDCCH,
                getCurrentTick(),
                ueDLContext_p->timeAlignmentTimer,
                getCurrentTick(),
                ueDLContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState,
                ueDLContext_p->ueIndex,LINE_NUMBER,
                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"CBR UL_SYNC_ON UE");

    }
    else
    {
        /* In case of handover or when UE detect SyncLoss IN_SYNC will go to RRC*/
        /*SPR_3065_FIX*/
        /*SPR : 9310 FIX start*/
        sendSyncIndToRRC(ueIndex, ueDLContext_p->crnti, IN_SYNC_AFTER_OUT_OF_SYNC,internalCellIndex);
        /*SPR : 9310 FIX end*/

        /*SPR_3065_FIX*/

        /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
        lteCellMacULPerfStats->totalRcvdMsg3RandomAccessPreamblesCBRA++;
#endif
        /* - PERF_STATS_PH1D1 */

        /* SPR 3043 start */
        if(ueDLContext_p->tcrntiFlag)
        {
            ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag = FALSE;
            dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag = FALSE;
            ueDLContext_p->tcrntiFlag = FALSE;
        }
        /* SPR 3043 end */
    }


    /* SPR 3271 changes start */
    ueDLContext_p->currPowerCorrectionForPucch = 0;
    ueDLContext_p->powerControlPucchTpcValue = 0;
    ueDLContext_p->averageSinrForPucch = \
                                         /*CA Changes start  */
                                         (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])-> \
                                         deltaSinrToTPCMapForPUCCH.pucchSINRTarget;
    /*CA Changes end  */
    ueDLContext_p->lastSINRCoolOffCounter = 0;
    /*CA Changes start  */
    initPucchBlerContext(ueDLContext_p, internalCellIndex);
    /*CA Changes end  */
    /* +- SPR 17777 */
    resetTpcPdcchConfigInfoForPucch (ueDLContext_p, internalCellIndex);
    /* +- SPR 17777 */
    /*CA Changes end  */
    /* SPR 3271 changes end */
    /* + CQI_5.0 */
    resetCQIInfo(ueDLContext_p,
            &ueDLContext_p->dlCQIInfo,
            internalCellIndex);
    /* + CQI_5.0 */
    /*reset cuurPowerCorrection* LJA*/
    ulUEContext_p->currPowerCorrectionForPusch = 0;
    ulUEContext_p->prevPowerCorrectionForPusch = 0;
    /* SPR 3271 changes start */
    ulUEContext_p->powerControlPuschTPCValue = 0;
    /*CA Changes start  */
    ulUEContext_p->pathLoss = (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])-> \
                              alphaBasedPathLossToTargetSINRMap.defaultPathLoss;
    /*CA Changes end  */
    /* ICIC changes start */
    /*CA Changes start  */
    updateULPathLoss(ueIndex,ulUEContext_p->pathLoss, internalCellIndex);
    /*CA Changes end  */
    /* ICIC changes ends */
    for(n = 0; n < MAX_SUB_FRAME; n++)
    {
        ulUEContext_p->powerControlDeltaTFAndLogFactor[n] = 0;
    }
    ulUEContext_p->mcsIndex = ulUEContext_p->initialMcs;
    /* Cyclomatic_complexity_changes_start */
    getUlModSchemeFromMcsIndex(ulUEContext_p->mcsIndex,\
            /*CA Changes start  */
            &(ulUEContext_p->modulationScheme),cellParams_p->macEnable64QAM,ulUEContext_p->ueCategory);
    /*CA Changes end  */
    /* Cyclomatic_complexity_changes_end */

    /*CA Changes start  */
    initUEULCSIContext(ulUEContext_p,ulUEContext_p->initialMcs, internalCellIndex);
    /* +- SPR 17777 */
    resetTpcPdcchConfigInfoForPusch (ulUEContext_p, internalCellIndex);
    /* +- SPR 17777 */
    /*CA Changes end  */    
    /* SPR 3271 changes end */


    /* SPR 2363 start */
    ueDLContext_p->lastTASentTick = receivedTTI - UE_RA_ADV_TTI;
    /* SPR 2363 end */
    /* SPR 3608 changes start */
    checkAndSetHighPriorityScheduling(ueIndex);
    /* SPR 3608 changes end */


    /*SPR_3061_3068_FIX*/
    ueDLContext_p->lastSyncDetectedTick = getCurrentTick();
    /*SPR_3061_3068_FIX*/
    /* SPR 15682 fix start */
    /* Refer Spec 36.213 Section 4.2.3 for the calculations
     * NTA = TA ×16 , where TA = 0, 1, 2, ..., 1282*/
    ueDLContext_p->timingAdvanceAbsValue = (tempUEContext_p->timingAdvanceAbs << 4);
    /* SPR 15682 fix end */

    LOG_MAC_MSG(MSG3_CRNTI_RECVD_ID,
            LOGWARNING,
            MAC_RACH,
            getCurrentTick(),
            ueDLContext_p->crnti,
            ueIndex,
            tcRNTI,
            ueDLContext_p->timingAdvanceAbsValue,
            ueDLContext_p->lastTASentTick,
            DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,
            "");

    return MAC_SUCCESS;
}
/*CA Changes end  */

/*****************************************************************************
 * Function Name  : handleCCCHSDUReceived
 * Inputs         : tempUEContext_p - Pointer to the TempCRNTICtx
 *                  tcRNTI - TCRNTI value
 *                  dlDelay - DL delay
 *                  ttiReceived - TTI in which message received 
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : None
 * Description    : This will prepaires the contention resolution message and 
 *                  inserts in contentionResolutionMsgQ_g queue
 *****************************************************************************/
void handleCCCHSDUReceived(TempCRNTICtx *tempUEContext_p,
        UInt16  tcRNTI,
        UInt32  dlDelay,
        /* SPR 15909 fix start */
        tickType_t  ttiReceived,
        /* SPR 15909 fix end */
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    UInt32 sfn            = 0;
    UInt32 activeIndex    = 0;
    UInt32 contentionResTimerVal = 0;
    UInt8  numRB          = 0;
    UInt8  mcsIndex       = 0;
    UInt8  tbSizeFound    = 0;
    UInt32 buffLenInBytes = 0;
    UInt32 hdrLen         = 0;
    /* SPR 3628 chg */
#ifdef TDD_CONFIG
    UInt8  tempRbSSF;
    UInt8  temptbsizeSSF;
    UInt8  tempmcsIndexSSF;
#endif
    /* SPR 3628 chg */
    UInt8 *contentionMsg_p = PNULL;
    UInt8 *ccchData_p      = PNULL;
    ContentionQueueNode *contentionQueueNode_p = PNULL;
    UInt8 *msg_p = PNULL;
    UInt8 *msg_struct_p = PNULL;

    /*CA Changes start  */
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /*CA Changes end  */

    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
    /* + PERF_CA */
    LteCellMacULPerfStats *lteCellMacULPerfStats = &(gMacCellPerfStats_p[internalCellIndex]-> \
            lteCellMacULPerfStats);
    /* - PERF_CA */
#endif
    /* - PERF_STATS_PH1D1 */

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Entry] %s\n",__func__);

    sfn = getSysFrameNumer(internalCellIndex);

    /*CA Changes start  */
    UPDATE_CONTENTION_RES_TIMER_ACTIVE_INDEX(cellParams_p->rachConfigInfo, sfn);

    activeIndex =cellParams_p->rachConfigInfo.rachConfiguration.activeIdxContentionRes;

    contentionResTimerVal = 
        cellParams_p->rachConfigInfo.rachConfiguration.macContentionResTimer[activeIndex];
    /*CA Changes end  */

    /*Prepare Contention Resolution message and push to Contention Queue*/
    GET_MEM_NODE_CONTENTION_RESOLUTION_SQ(contentionQueueNode_p, ContentionQueueNode);
    /* + coverity 24505 */
    if( PNULL == contentionQueueNode_p )
    {
        return;
    }
    /* - coverity 24505 */
    buffLenInBytes = CONTENTION_RES_MAC_CE_SIZE + CONTENTION_RES_SUB_HEADER_LEN;


    /*Length of the Contention Resolution message is 6 bytes and 1 byte 
     *subheader, total length is 7 bytes i.e. 56 bits.
     *Based on the TB size table no padding needed and requires 1RB
     *mcs Index is 4
     */
    if (MAC_FAILURE == determineRBFromTBSizeForDCIFormate1A(buffLenInBytes,
                &tbSizeFound,
                &numRB, 
                &mcsIndex
                /* SPR 3628 chg */
#ifdef TDD_CONFIG
                , &temptbsizeSSF
                , &tempRbSSF
                , &tempmcsIndexSSF
#endif
                /* SPR 3628 chg */
                ))
    {
        LTE_MAC_UT_LOG(LOG_WARNING, RACH_MGR,
                "[%s]: ERROR: determineRBFromTBSizeForDCIFormate1A() Failed\n",
                __func__);

        FREE_MEM_NODE_CONTENTION_RESOLUTION_SQ(contentionQueueNode_p);

        LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
                "[Exit] %s\n",__func__);

        LOG_UT(MAC_DETERMINE_RB_FROM_TB_SIZE,
                LOGDEBUG,
                MAC_RACH,
                getCurrentTick(),
                tcRNTI,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"determineRBFromTBSizeForMsg Failed");


        return;
    }

    hdrLen = DATA_MSG_API_HEADER_LEN;
    /* + coverity 32685 */
    if( PNULL == (msg_struct_p = msgAlloc(PNULL,0,0,0)))
    {
        FREE_MEM_NODE_CONTENTION_RESOLUTION_SQ(contentionQueueNode_p);
        return;
    }
    msg_p = msgReserve(msg_struct_p,0, (tbSizeFound + hdrLen));
    /* - coverity 32685 */
    /*SPR 4017 PH2 Start*/
    if(msg_p)
    {
        memSet(msg_p, 0, (tbSizeFound + hdrLen));
        contentionQueueNode_p->msg_p = msg_struct_p;
        contentionMsg_p = msg_p + hdrLen;
    }
    /*Coverity 10803 Fix Start*/
    else
    {
        msgFree( msg_struct_p );
        FREE_MEM_NODE_CONTENTION_RESOLUTION_SQ(contentionQueueNode_p);
        ltePanic("msg_p is NULL");
        /* Klockwork warning Changes start */
        return;
        /* Klockwork warning Changes end */
    }
    /*Coverity 10803 Fix End*/

    /*SPR 4017 PH2 End*/
    contentionQueueNode_p->tcRNTI = tcRNTI;
    contentionQueueNode_p->dciFormat = DCI_FORMAT_FOR_CONTENTION_MSG;
    contentionQueueNode_p->numRB = numRB;
    contentionQueueNode_p->msgLen = tbSizeFound;
    contentionQueueNode_p->mcsIndex = mcsIndex;
    /* SPR 2578 Fix Start */
    contentionQueueNode_p->contentionResExp =
#ifdef FDD_CONFIG
        /* + SPR 17733 */
        tempUEContext_p->contentionResExp =
        /* - SPR 17733 */
#endif
        ttiReceived + contentionResTimerVal - dlDelay;
    /* SPR 2578 Fix End */

    /* SPR 3628 chg */
#ifdef TDD_CONFIG
    contentionQueueNode_p->numRBForSSF = tempRbSSF;
    contentionQueueNode_p->msgLenForSSF = temptbsizeSSF;
    contentionQueueNode_p->mcsIndexForSSF = tempmcsIndexSSF;
#endif
    /* SPR 3628 chg */

    /*One byte sub header*/
    MAC_SUBHEADER_ONE_BYTE(contentionMsg_p, 0, 
            UE_CONTENTION_RESOLUTION_IDENTITY_LCH);

    void  *ptr        = NULL;
    QSEGMENT segment;

    ptr = msgSegNext(tempUEContext_p->msg3BuffInfo.msg_p , 0, &segment);
    if (PNULL == ptr)
    {
        /* + coverity 32685 */
        if( contentionQueueNode_p->msg_p )
        {
            msgFree(contentionQueueNode_p->msg_p);
        }
        FREE_MEM_NODE_CONTENTION_RESOLUTION_SQ(contentionQueueNode_p);
        /* - coverity 32685 */
        ltePanic("msgSegNext() returned NULL");
        /* Klockwork warning Changes start */
        return;
        /* Klockwork warning Changes end */
    }
    /*SPR 1768 - Start*/
    else
    {
        /*SPR 1768 - End*/
        ccchData_p = segment.base;
    }

    memCpy(contentionMsg_p,
            ccchData_p,
            CONTENTION_RES_MAC_CE_SIZE);

    if( ENQUEUE_CONTENTION_RESOLUTION_SQ( ContentionQueueNode,
                /*CA Changes start  */
                &contentionQueueNode_p->nodeAnchor, internalCellIndex) )
        /*CA Changes end  */
    {
        LTE_MAC_UT_LOG(LOG_WARNING, RACH_MGR,
                "[%s]: ERROR: Failed to push contentionQueueNode to\n"
                "contentionResolutionMsgQ_g queue TCRNIT [%d]\n",
                __func__,tcRNTI);

        /* + coverity 32685 */
        if( contentionQueueNode_p->msg_p )
        {
            msgFree(contentionQueueNode_p->msg_p);
        }
        /* - coverity 32685 */
        FREE_MEM_NODE_CONTENTION_RESOLUTION_SQ(contentionQueueNode_p);

        LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
                "[Exit] %s\n",__func__);

        return;
    }

    tempUEContext_p->status = MSG3_RECEIVED;
    /* SPR 2578 Fix Start */
    /* The below timer is active till ACK/NACK for contention resolution 
     * message is received. Therefore expiry time will be ttiReceived(TTI when
     * msg3 is received) + contentionResTimerVal + PHY_UL_DATA_DELAY + 4(Harq 
     * ACK./NACK will be recevied after 4 TTI from TTI when CR was transmitted.*/

    tempUEContext_p->timerNode_p = 
        startContentionResTimer((ttiReceived + contentionResTimerVal +  
                    /*CA Changes start  */
                    PHY_UL_DATA_DELAY + 4), tcRNTI, internalCellIndex);
    /*CA Changes end  */
    /* SPR 2578 Fix End */
    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
    lteCellMacULPerfStats->totalRcvdMsg3RandomAccessPreamblesCBRA++;
#endif
    /* - PERF_STATS_PH1D1 */

    LTE_MAC_UT_LOG(LOG_INFO, RACH_MGR,
            "[%s]: contentionQueueNode pushed to contentionResolutionMsgQ_g\n"
            " queue successfully for TCRNIT [%d]\n",
            __func__,tcRNTI);

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
            "[Exit] %s\n",__func__);

    LOG_MAC_MSG(MAC_CONTENTION_RESOLUTION,
            LOGDEBUG,
            MAC_RACH,
            getCurrentTick(),
            tcRNTI,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"Pushed to contentionResolutionMsgQ_g");


    return; 
}

/**************************************************************************************
 * Function Name  : getRARPduInfoforRARNTI
 *
 * Inputs         : RARNTI - RA RNTI
 *                  raResponsesInTTI_p - Pointer for array of RAR PDU In a TTI
 * Outputs        : NONE 
 *
 * Returns        : RAR PDU Pointer or PNULL
 * Description    : Search for the RAR PDU for RARNTI
 *                  If the RAR PDU for RARNTI is found 
 *                  return Pointer to RAR PDU or PNULL
 ****************************************************************************************/
RAReqTobeScheduledInPdu* getRARPduInfoforRARNTI(
        UInt16 raRnti,
        RarPduTobeScheduledInTTI *raResponsesInTTI_p)
{
    UInt8 numberOfRARPDU = raResponsesInTTI_p->numRarPdu;
    RAReqTobeScheduledInPdu* rarPduTobeScheduled_p = PNULL;
    UInt8 rarPDUFoundFlag = 0,rarPduIndex=0;

    for(rarPduIndex=0;rarPduIndex<numberOfRARPDU;rarPduIndex++)
    {
        rarPduTobeScheduled_p = &raResponsesInTTI_p->rarPduTobeScheduled[rarPduIndex];

        if(rarPduTobeScheduled_p->raRnti == raRnti)
        {
            rarPDUFoundFlag = 1;
            break;
        }
    }
    if(1 == rarPDUFoundFlag)
    {
        return rarPduTobeScheduled_p;
    }
    else
    {
        return PNULL; 
    }

}    
/**************************************************************************************
 * Function Name  : getNextRARPduTobeScheduled
 * Inputs         : raResponsesInTTI_p - Pointer for array of RAR PDU In a TTI
 * Outputs        : NONE
 * Returns        : RAR PDU Pointer
 * Description    : Return Next RAR PDU Pointer in raResponsesInTTI_p 
 ****************************************************************************************/

RAReqTobeScheduledInPdu* getNextRARPduTobeScheduled(
        RarPduTobeScheduledInTTI *raResponsesInTTI_p)
{
    UInt8 rarPDUCnt =  raResponsesInTTI_p->numRarPdu;
    return &raResponsesInTTI_p->rarPduTobeScheduled[rarPDUCnt];
}

/**************************************************************************************
 * Function Name  : getPreambleStatusInfo
 * Inputs         : preambleIndex- index of the preamble
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : Return Preamble Status 
 * Description    : Return Preamble Status
 ****************************************************************************************/
/*CA Changes start  */
ContentionFreePreambleStatus  getPreambleStatusInfo(UInt8 preambleIndex, InternalCellIndex internalCellIndex)
{
    ContentionFreePreamble *contFreePreamble_p = PNULL;

    contFreePreamble_p = 
        contentionFreeRACHPool_gp[internalCellIndex]->contentionFreePreambleArray[preambleIndex];
    if(contFreePreamble_p)
    {
        return contFreePreamble_p->currStatus;
    }
    else
    {
        return PREAMBLE_INVALID; 
    }
}

/**************************************************************************************
 * Function Name  : getContentionFreePreambleInfo
 * Inputs         : preambleIndex- index of the preamble
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : return Preamble Info
 * Description    : return Preamble Info for the preambleIndex
 ****************************************************************************************/
ContentionFreePreamble*  getContentionFreePreambleInfo(UInt8 preambleIndex, InternalCellIndex internalCellIndex)
{
    return contentionFreeRACHPool_gp[internalCellIndex]->
        contentionFreePreambleArray[preambleIndex];
}
/*****************************************************************************
 * Function Name  : initContentionFreePreamblePool
 * Inputs         : firstFreePreambleIndex- index of the first free preamble 
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : RACH contention free preamble pool initialized
 * Returns        : This function returns MAC_SUCCESS if pool is successfully
 *                       initialized, else MAC_FAILURE is returned
 * Description    : Initializes the contention free RACH pool
 *****************************************************************************/
MacRetType initContentionFreePreamblePool(UInt8 firstFreePreambleIndex,
        InternalCellIndex internalCellIndex)
{

    UInt8 localFirstFreePreambleIndex = firstFreePreambleIndex;

    UInt8 localCurrNumOfFreePreambles = 0; 

    UInt8 initConBasedPreambles = 0; 

    ContentionFreePreamble *contFreePreamble_p = PNULL;

    if(MAC_MAX_NUM_OF_RA_PREAMBLES == localFirstFreePreambleIndex)
    {
        contentionFreeRACHPool_gp[internalCellIndex]->firstFreePreambleIndex 
            = INVALID_PREAMBLE_IDX;

        contentionFreeRACHPool_gp[internalCellIndex]->lastFreePreambleIndex 
            = INVALID_PREAMBLE_IDX;

        contentionFreeRACHPool_gp[internalCellIndex]->currNumOfFreePreambles 
            = 0;

        LOG_MAC_MSG(INIT_PREAMBLE_POOL_FAIL_ID,
                LOGWARNING,
                MAC_RACH,
                getCurrentTick(),
                firstFreePreambleIndex,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"MAX_NUM_OF_RA_PREAMBLES");

        return MAC_SUCCESS;
    }


    while(initConBasedPreambles < localFirstFreePreambleIndex)
    {
        contentionFreeRACHPool_gp[internalCellIndex]->
            contentionFreePreambleArray
            [initConBasedPreambles] = PNULL;

        initConBasedPreambles++;

    }
    while(localFirstFreePreambleIndex < MAC_MAX_NUM_OF_RA_PREAMBLES)
    {
        GET_MEM_FROM_POOL(ContentionFreePreamble, 
                contFreePreamble_p, 
                sizeof(ContentionFreePreamble),
                PNULL);

        /* + Coverity 24507 */
        if( PNULL == contFreePreamble_p )
        {
            LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGERROR,MAC_RACH,
                    getCurrentTick(), __LINE__,
                    sizeof(ContentionFreePreamble), 0,0,0,
                    0.0,0.0,__FILE__,__func__);
            return MAC_FAILURE ;
        }
        /* - Coverity 24507 */

        contentionFreeRACHPool_gp[internalCellIndex]->
            contentionFreePreambleArray
            [localFirstFreePreambleIndex] = contFreePreamble_p;


        contFreePreamble_p->currStatus= PREAMBLE_FREE;
        contFreePreamble_p->pdcchOrderHandOverFlag= INIT_TRIGGER_VALUE;
        contFreePreamble_p->ueIdentifier = INVALID_UE_ID;
        contFreePreamble_p->nextFreePreambleIndex =
            ++localFirstFreePreambleIndex;

        /* SPR_4262_FIX Start */ 
        contFreePreamble_p->contentionFreeRACHRecvd = 0;
        /* SPR_4262_FIX End  */ 


        localCurrNumOfFreePreambles++;
    }

    contentionFreeRACHPool_gp[internalCellIndex]->currNumOfFreePreambles 
        = localCurrNumOfFreePreambles;

    contentionFreeRACHPool_gp[internalCellIndex]->firstFreePreambleIndex 
        = firstFreePreambleIndex;

    contentionFreeRACHPool_gp[internalCellIndex]->lastFreePreambleIndex 
        = localFirstFreePreambleIndex - 1;



    LOG_MAC_MSG(INIT_PREAMBLE_POOL_ID,
            LOGDEBUG,
            MAC_RACH,
            getCurrentTick(),
            /*CA Changes start  */
            getCurrNumOfFreePreambles(internalCellIndex),
            getFirstFreePreambleIndex(internalCellIndex),
            getLastFreePreambleIndex(internalCellIndex),
            /*CA Changes end  */
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"INIT_PREAMBLE_POOL_ID");

    return MAC_SUCCESS;
}
/*CA Changes end  */

/*****************************************************************************
 * Function Name  : allocateDedicatedPreamble
 * Inputs         : ueIndex: Index of the UE
 *                  triggerFlag: Flag for PDDCH order or connection HandOver
 *                  rachResourceInfo: Information of RACH resources 
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : Preamble is allocated for Contention Free RACH request
 * Returns        : The scenario of success or failure or partial Success as 
 * 		                 the case may be. 	
 * Description    : Allocates the Preamble Index
 ***************************************************************************/
MacRetType allocateDedicatedPreamble(UInt16 ueIndex, 
        ConFreeRACHTrigger triggerFlag,
        ConFreeRACHResourcesInfo *rachResourceInfo_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex)
/*CA Changes end  */
{

    UInt8 localFirstFreePreambleIndex;

    /*533-GCC-warning-fix*/
#ifdef LOG_PRINT_ENABLED
    UInt8 localLastFreePreambleIndex;
    UInt8 localCurrNumOfFreePreambles;
#endif
    /*533-GCC-warning-fix*/

    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
    /* + PERF_CA */
    LteCellMacULPerfStats *lteCellMacULPerfStats = &(gMacCellPerfStats_p[internalCellIndex]-> \
            lteCellMacULPerfStats);
    /* - PERF_CA */
#endif

    localFirstFreePreambleIndex =
        getFirstFreePreambleIndex(internalCellIndex);

#ifdef LOG_PRINT_ENABLED
    localCurrNumOfFreePreambles =
        getCurrNumOfFreePreambles( internalCellIndex );

    localLastFreePreambleIndex =
        getLastFreePreambleIndex( internalCellIndex );
#endif

    /* SPR 18434 Start */
    if((0 != getCurrNumOfFreePreambles(internalCellIndex)) && (localFirstFreePreambleIndex < MAC_MAX_NUM_OF_RA_PREAMBLES))
    /* SPR 18434 End */
    {    
        ContentionFreePreamble *localContentionFreePreamble_p  =
            getContentionFreePreambleInfo(localFirstFreePreambleIndex, internalCellIndex);
        /*CA Changes end  */

        localContentionFreePreamble_p->currStatus = PREAMBLE_BUSY;
        localContentionFreePreamble_p->ueIdentifier = ueIndex;
        localContentionFreePreamble_p->pdcchOrderHandOverFlag = triggerFlag;

        /* SPR_4262_FIX Start */ 
        localContentionFreePreamble_p->contentionFreeRACHRecvd = 0;
        /* SPR_4262_FIX End */ 

        /*CA Changes start  */
        updateFirstFreePreambleIndex(
                localContentionFreePreamble_p->nextFreePreambleIndex, internalCellIndex);
        /*CA Changes end  */

        localContentionFreePreamble_p->nextFreePreambleIndex = 
            INVALID_PREAMBLE_IDX;

        /*CA Changes start  */
        decrementNumberOfFreePrambles(internalCellIndex);
        /*CA Changes end  */
        /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
        lteCellMacULPerfStats->totalAllocatedRandomAccessPreambles++;
#endif
        /* - PERF_STATS_PH1D1 */

        /*CA Changes start  */
        if(!(getCurrNumOfFreePreambles(internalCellIndex)))
        {
            updateLastFreePreambleIndex(INVALID_PREAMBLE_IDX, internalCellIndex);
        }
        /*CA Changes end  */

        rachResourceInfo_p->preambleIndex= localFirstFreePreambleIndex;
        rachResourceInfo_p->prachMaskIndex =
            MAC_PRACH_MASK_INDEX_FOR_CONTENTION_FREE_RACH;



        LOG_MAC_MSG(ALLOC_PREAMBLE_UE_ID,
                LOGWARNING,
                MAC_RACH,
                getCurrentTick(),
                ueIndex,
                triggerFlag,
                localFirstFreePreambleIndex,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"ALLOC_PREAMBLE_UE_ID");

        LOG_MAC_MSG(BEFORE_ALLOC_PREAMBLE_ID, 
                LOGBRIEF,
                MAC_RACH,
                getCurrentTick(),
                localCurrNumOfFreePreambles,
                localFirstFreePreambleIndex,
                localLastFreePreambleIndex,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"BEFORE_ALLOC_PREAMBLE_ID");

        LOG_MAC_MSG(AFTER_ALLOC_PREAMBLE_ID, 
                LOGBRIEF,
                MAC_RACH,
                getCurrentTick(),
                getCurrNumOfFreePreambles(internalCellIndex),
                getFirstFreePreambleIndex(internalCellIndex),
                getLastFreePreambleIndex(internalCellIndex),
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"AFTER_ALLOC_PREAMBLE_ID");



    }
    else
    {
        LOG_MAC_MSG(ALLOC_PREAMBLE_FAIL_ID,
                LOGWARNING,
                MAC_RACH,
                getCurrentTick(),
                /*CA Changes start  */
                getCurrNumOfFreePreambles(internalCellIndex),
                getFirstFreePreambleIndex(internalCellIndex),
                getLastFreePreambleIndex(internalCellIndex),
                /*CA Changes end  */
                ueIndex,
                triggerFlag,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"NO_FREE_PREAMBLE");

        rachResourceInfo_p->preambleIndex = INVALID_PREAMBLE_IDX;
        rachResourceInfo_p->prachMaskIndex = 0;
        /* SPR 18434 Start */
        if(localFirstFreePreambleIndex == MAC_MAX_NUM_OF_RA_PREAMBLES)
        {
            decrementNumberOfFreePrambles(internalCellIndex);
        }
        /* SPR 18434 End */
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : freePreambleIndex
 * Inputs         : preambleIndex - Index of the UE Preamble to be freed
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : frees the preamble
 * Returns        : Returns MAC_SUCCESS in case of success scenario, 
 *                                                     else MAC_FAILURE
 * Description    : This function Frees the Preamble Index
 ***************************************************************************/
/*CA Changes start  */
MacRetType freePreambleIndex(UInt8 preambleIndex, InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{

    UInt8 localLastFreePreambleIndex;

    /*533-GCC-warning-fix*/
#ifdef LOG_PRINT_ENABLED
    UInt8 localFirstFreePreambleIndex;
    ConFreeRACHTrigger triggerFlag = INIT_TRIGGER_VALUE; 
#endif
    /*533-GCC-warning-fix*/
    UInt8 localCurrNumOfFreePreambles; 


    ContentionFreePreamble *localLastContentionFreePreamble_p = PNULL;

    ContentionFreePreamble *localContentionFreePreamble_p = PNULL;

    /*CA Changes start  */
    localLastFreePreambleIndex = getLastFreePreambleIndex(internalCellIndex);
#ifdef LOG_PRINT_ENABLED
    localFirstFreePreambleIndex = getFirstFreePreambleIndex(internalCellIndex);
#endif
    localCurrNumOfFreePreambles = 
        getCurrNumOfFreePreambles(internalCellIndex);
    /*CA Changes end  */

    /*+SPR #3444*/
    if(preambleIndex >= MAC_MAX_NUM_OF_RA_PREAMBLES)
        /*-SPR #3444*/
    {
        LOG_MAC_MSG(FREE_PREAMBLE_FAIL_ID,
                LOGWARNING,
                MAC_RACH,
                getCurrentTick(),
                /*CA Changes start  */
                getCurrNumOfFreePreambles(internalCellIndex),
                getFirstFreePreambleIndex(internalCellIndex),
                getLastFreePreambleIndex(internalCellIndex),
                /*CA Changes end  */
                LINE_NUMBER,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"FREE_PREAMBLE_FAIL_ID");

        return MAC_FAILURE;

    } 
    if(0 == localCurrNumOfFreePreambles)
    {
        /*CA Changes start  */
        updateFirstFreePreambleIndex(preambleIndex, internalCellIndex);
        /*CA Changes end  */
    }
    else
    {
        /*CA Changes start  */
        localLastContentionFreePreamble_p = 
            getContentionFreePreambleInfo(localLastFreePreambleIndex, internalCellIndex);

        localLastContentionFreePreamble_p->nextFreePreambleIndex = 
            preambleIndex;
    }
    localContentionFreePreamble_p = 
        /*CA Changes end  */
        getContentionFreePreambleInfo(preambleIndex, internalCellIndex);
    /*533-GCC-warning-fix*/
#ifdef LOG_PRINT_ENABLED
    triggerFlag = 
        localContentionFreePreamble_p->pdcchOrderHandOverFlag;
#endif
    /*533-GCC-warning-fix*/
    localContentionFreePreamble_p->currStatus= PREAMBLE_FREE;

    localContentionFreePreamble_p->pdcchOrderHandOverFlag = 
        INIT_TRIGGER_VALUE;

    localContentionFreePreamble_p->nextFreePreambleIndex = 
        INVALID_PREAMBLE_IDX;

    /*CA Changes start  */
    incrementNumberOfFreePrambles(internalCellIndex);

    updateLastFreePreambleIndex(preambleIndex, internalCellIndex);
    /*CA Changes end  */

    LOG_MAC_MSG(FREE_PREAMBLE_UE_ID,
            LOGWARNING,
            MAC_RACH,
            getCurrentTick(),
            localContentionFreePreamble_p->ueIdentifier,
            triggerFlag,
            preambleIndex,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"FREE_PREAMBLE_UE_ID");

    LOG_MAC_MSG(BEFORE_FREE_PREAMBLE_INFO_ID, 
            LOGBRIEF,
            MAC_RACH,
            getCurrentTick(),
            localCurrNumOfFreePreambles,
            localFirstFreePreambleIndex,
            localLastFreePreambleIndex,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"BEFORE_FREE_PREAMBLE");

    LOG_MAC_MSG(AFTER_FREE_PREAMBLE_INFO_ID, 
            LOGBRIEF,
            MAC_RACH,
            getCurrentTick(),
            /*CA Changes start  */
            getCurrNumOfFreePreambles(internalCellIndex),
            getFirstFreePreambleIndex(internalCellIndex),
            getLastFreePreambleIndex(internalCellIndex),
            /*CA Changes end  */
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"AFTER_FREE_PREAMBLE");


    return MAC_SUCCESS;

}
/*CA Changes start  */
/*****************************************************************************
 * Function Name  : getFirstFreePreambleIndex
 * Inputs         : internalCellIndex
 * Outputs        : This function gets the first free preamble Index
 * Returns        : contentionFreeRACHPool_gp[internalCellIndex]->firstFreePreambleIndex
 * Description    : This function gets the first free preamble 
 ***************************************************************************/
UInt8 getFirstFreePreambleIndex(InternalCellIndex internalCellIndex)
{
    return	contentionFreeRACHPool_gp[internalCellIndex]->firstFreePreambleIndex;
}
/*****************************************************************************
 * Function Name  : updateFirstFreePreambleIndex
 * Inputs         : preambleIndex- Index of thye preamble
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        :
 * Returns        : 
 * Description    : updates contentionFreeRACHPool_gp[internalCellIndex]->firstFreePreambleIndex
 ***************************************************************************/
void updateFirstFreePreambleIndex(UInt8 preambleIndex, InternalCellIndex internalCellIndex)
{
    contentionFreeRACHPool_gp[internalCellIndex]->firstFreePreambleIndex = preambleIndex;
}
/*****************************************************************************
 * Function Name  : getLastFreePreambleIndex
 * Inputs         : internalCellIndex
 * Outputs        : gets the last free preamble Index
 * Returns        : contentionFreeRACHPool_gp[internalCellIndex]->lastFreePreambleIndex
 * Description    : THis function gets the last free preamble 
 ***************************************************************************/
UInt8 getLastFreePreambleIndex(InternalCellIndex internalCellIndex)
{
    return	contentionFreeRACHPool_gp[internalCellIndex]->lastFreePreambleIndex;
}
/*****************************************************************************
 * Function Name  : updateLastFreePreambleIndex
 * Inputs         : preambleIndex- index of the preamble
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : updates last free preamble index
 * Returns        : None 
 * Description    : this function updates last free preamble index
 ***************************************************************************/
void updateLastFreePreambleIndex(UInt8 preambleIndex, InternalCellIndex internalCellIndex)
{
    contentionFreeRACHPool_gp[internalCellIndex]->lastFreePreambleIndex = preambleIndex;
}

/*****************************************************************************
 * Function Name  : incrementNumberOfFreePrambles
 * Inputs         :  internalCellIndex
 * Outputs        :  Current Number Of Free Preambles incremented
 * Returns        :  None
 * Description    :  increments the number of free preambles
 ***************************************************************************/
void incrementNumberOfFreePrambles(InternalCellIndex internalCellIndex)
{
    contentionFreeRACHPool_gp[internalCellIndex]->currNumOfFreePreambles++;
}


/*****************************************************************************
 * Function Name  : decrementNumberOfFreePrambles
 * Inputs         : internalCellIndex
 * Outputs        : Current Number Of Free Preambles are decremented
 * Returns        : None
 * Description    : decrements the number of free preambles
 ***************************************************************************/
void decrementNumberOfFreePrambles(InternalCellIndex internalCellIndex)
{
    /* SPR 18434 Start */
    if(contentionFreeRACHPool_gp[internalCellIndex]->currNumOfFreePreambles == 0)
    {
        return;
    }
    /* SPR 18434 End */
    contentionFreeRACHPool_gp[internalCellIndex]->currNumOfFreePreambles--;
}

/*****************************************************************************
 * Function Name  : getCurrNumOfFreePreambles
 * Inputs         : internalCellIndex
 * Outputs        : gets the current number of free preambles 
 * Returns        : contentionFreeRACHPool_gp[internalCellIndex]->currNumOfFreePreambles
 * Description    : THis function gets the current number of free preambles  
 ***************************************************************************/
UInt8 getCurrNumOfFreePreambles(InternalCellIndex internalCellIndex)
{
    return  contentionFreeRACHPool_gp[internalCellIndex]->currNumOfFreePreambles;
}
/*CA Changes end  */

/*****************************************************************************
 * Function Name  : startConFreePreambleTimer
 * Inputs         : expTTI - Expiry tti
 *                  preambleIndex
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : Pointer to the ConFreePreambleTimerNode
 * Description    : This will start the timer for Contention Free Preamble node
 *****************************************************************************/
/*CA Changes start  */
/* SPR 15909 fix start */
ConFreePreambleTimerNode *startConFreePreambleTimer(tickType_t expTTI, UInt8 preambleIndex, InternalCellIndex internalCellIndex)
    /* SPR 15909 fix end */
    /*CA Changes end  */
{
    ConFreePreambleTimerNode *preambleTimerNode_p = PNULL;
    /*533-GCC-warning-fix*/
#ifdef LOG_PRINT_ENABLED
    ContentionFreePreamble *localContentionFreePreamble_p = PNULL;
#endif
    /*533-GCC-warning-fix*/

    if(INVALID_PREAMBLE_IDX != preambleIndex)
    {    
        /*533-GCC-warning-fix*/
#ifdef LOG_PRINT_ENABLED
        localContentionFreePreamble_p = 
            getContentionFreePreambleInfo(preambleIndex, internalCellIndex);
#endif
        /*533-GCC-warning-fix*/

        GET_MEM_FROM_POOL(ConFreePreambleTimerNode, 
                preambleTimerNode_p, 
                sizeof(ConFreePreambleTimerNode),
                PNULL);

        /* + Coverity 24517 */
        if( PNULL == preambleTimerNode_p )
        {
            return PNULL ;
        }
        /* - Coverity 24517 */

        preambleTimerNode_p->expTTI = expTTI;
        preambleTimerNode_p->preambleIndex = preambleIndex;

        /*CA Changes start  */
        listInsertNode(conFreePreambleTimerList_gp[internalCellIndex], 
                &preambleTimerNode_p->nodeAnchor);
        /*CA Changes end  */


        LOG_MAC_MSG(PREAMBLE_TIMER_START_ID,
                LOGDEBUG,
                MAC_RACH,
                getCurrentTick(),
                preambleIndex,
                localContentionFreePreamble_p->pdcchOrderHandOverFlag,
                localContentionFreePreamble_p->ueIdentifier,
                LINE_NUMBER,
                expTTI,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"CON_FREE_PREAMBLE_TIMER_START");

        return preambleTimerNode_p;
    }
    else
    {
        LOG_MAC_MSG(PREAMBLE_POINTER_NULL,
                LOGWARNING,
                MAC_RACH,
                getCurrentTick(),
                preambleIndex,
                /*CA Changes start  */
                getFirstFreePreambleIndex(internalCellIndex),
                getLastFreePreambleIndex(internalCellIndex),
                getCurrNumOfFreePreambles(internalCellIndex),
                /*CA Changes end  */
                LINE_NUMBER,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,
                "PREAMBLE_POINTER_NULL");
        return PNULL;
    }

}

/*****************************************************************************
 * Function Name  : stopConFreePreambleTimer
 * Inputs         : preambleTimerNode_p - Pointer to the PreambleTimerNode
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : None
 * Description    : This will stop the timer for Preamble
 *****************************************************************************/
/*CA Changes start  */
void stopConFreePreambleTimer(ConFreePreambleTimerNode*  preambleTimerNode_p, InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{
    if(PNULL != preambleTimerNode_p)
    {    
        LOG_MAC_MSG(PREAMBLE_TIMER_STOP_ID,
                LOGDEBUG,
                MAC_RACH,
                getCurrentTick(),
                preambleTimerNode_p->preambleIndex,
                preambleTimerNode_p->expTTI,
                LINE_NUMBER,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"PREAMBLE_TIMER_STOP_ID");


        listDeleteNode(conFreePreambleTimerList_gp[internalCellIndex],
                &preambleTimerNode_p->nodeAnchor);

        freeMemPool(preambleTimerNode_p);

    }
    else
    {
        LOG_MAC_MSG(PREAMBLE_TIMER_STOP_ID,
                LOGDEBUG,
                MAC_RACH,
                getCurrentTick(),
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                LINE_NUMBER,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"TIMER_NODE_NULL_ID");

    }
}

/*****************************************************************************
 * Function Name  : checkContentionFreePreambleExpiry
 *
 * Inputs         : currentTTI - Current Global TTI
 *                  listNoneCount - Timer List Count
 *                  internalCellIndex - Cell-Index at MAC                                             
 *
 * Outputs        : Process the expired timers in conFreePreambleTimerList_g
 * Returns        : None
 * Description    : If the Timers had expired Free the preamble
 *                  If the timer is for PDCCH ORDER
 *                      If the PDCCH Order Sent count is less than threshold
 *                          Put entry in PDCCH ORDER Queue
 *                      Else
 *                          Send Radio Link Failure To RRC
 *****************************************************************************/
/*CA Changes start  */
/* SPR 15909 fix start */
void checkContentionFreePreambleExpiry(tickType_t currentTTI, UInt32 listNoneCount, InternalCellIndex internalCellIndex)
    /* SPR 15909 fix end */
    /*CA Changes end  */
{
    ConFreePreambleTimerNode *timerListNode_p = PNULL;
    ConFreePreambleTimerNode *prevTimerListNode_p = PNULL;
    ContentionFreePreamble *localContentionFreePreamble_p = PNULL;
    ULUEContext *ulUEContext_p = PNULL;
    UInt16 ueIndex;
    timerListNode_p =
        (ConFreePreambleTimerNode *) 
        /*CA Changes start  */
        getFirstListNode(conFreePreambleTimerList_gp[internalCellIndex]);
    /*CA Changes end  */

    if (PNULL == timerListNode_p)
    {
        return;
    }

    do
    {
        if (timerListNode_p->expTTI <= currentTTI)
        {
            /*CA Changes start  */
            listDeleteNode(conFreePreambleTimerList_gp[internalCellIndex],
                    &timerListNode_p->nodeAnchor);
            /*CA Changes end  */

            localContentionFreePreamble_p = 
                getContentionFreePreambleInfo(
                        /*CA Changes start  */
                        timerListNode_p->preambleIndex,internalCellIndex);               
            /*CA Changes end  */

            ueIndex = localContentionFreePreamble_p->ueIdentifier;

            ulUEContext_p =  ulUECtxInfoArr_g[ueIndex].ulUEContext_p;


            if(PNULL == ulUEContext_p)
            {
                LOG_MAC_MSG(PREAMBLE_TIMER_NOCONTEXT_ID,
                        LOGWARNING,
                        MAC_RACH,
                        getCurrentTick(),
                        timerListNode_p->preambleIndex,
                        localContentionFreePreamble_p->pdcchOrderHandOverFlag,
                        localContentionFreePreamble_p->ueIdentifier,
                        timerListNode_p->expTTI,
                        LINE_NUMBER,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"PREAMBLE_TIMER_NOCONTEXT_ID");

                /* SPR 13753 fix start */
                prevTimerListNode_p = timerListNode_p;
                timerListNode_p = (ConFreePreambleTimerNode *) getNextListNode(
                        &prevTimerListNode_p->nodeAnchor);
                freeMemPool(prevTimerListNode_p);

                listNoneCount--;
                /* SPR 13753 fix end */
                continue;

            }

            LOG_MAC_MSG(PREAMBLE_TIMER_EXPIRY_ID,
                    LOGINFO,
                    MAC_RACH,
                    getCurrentTick(),
                    timerListNode_p->preambleIndex,
                    localContentionFreePreamble_p->pdcchOrderHandOverFlag,
                    localContentionFreePreamble_p->ueIdentifier,
                    ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderSentCount,
                    timerListNode_p->expTTI,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"PREAMBLE_TIMER_EXPIRY_ID");

            if(PDCCH_ORDER ==
                    localContentionFreePreamble_p->pdcchOrderHandOverFlag)
            {    
                if(ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderSentCount 
                        < MAX_PDCCH_ORDER_RETRIES)
                {
                    putEntryInPdcchOrderQ(ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx, 
                            /*CA Changes start  */
                            ueIndex, currentTTI, internalCellIndex);
                    /*CA Changes end  */
                }
                else
                {
                    /* + E_CID_5.3 */
                    /* If RRM sent request for TA 2 and/or TA1type/AOA reports then
                     * simply send response as MAC_FAILURE
                     */
                    if (NO_REPORT_TYPE_REQUIRED != 
                            ulUEContext_p->eCidReportParams.typeofReportsRequired)
                    {
                        putEntryInEcidMeasReportQueue(ueIndex,\
                                NO_REPORT_TYPE_REQUIRED,\
                                MAC_FAILURE, \
                                /*CA Changes start  */
                                internalCellIndex);
                        /*CA Changes end  */
                        ulUEContext_p->eCidReportParams.typeofReportsRequired = 
                            NO_REPORT_TYPE_REQUIRED;
                    }
                    else
                    {
                        /* - E_CID_5.3 */
                        /*SPR_3065_FIX*/
                        sendSyncIndToRRC(ueIndex, ulUEContext_p->crnti,
                                /*CA Changes start  */
                                OUT_OF_SYNC_MAX_RETRIES, internalCellIndex);
                        /*CA Changes end  */
                        /*SPR_3065_FIX*/
                    }
                    /* Free the preamble */
                    /*CA Changes start  */
                    freePreambleIndex(
                            timerListNode_p->preambleIndex, internalCellIndex);
                    /*CA Changes end  */
                }
            }
            prevTimerListNode_p = timerListNode_p;

            timerListNode_p = (ConFreePreambleTimerNode *) getNextListNode(
                    &prevTimerListNode_p->nodeAnchor);
            freeMemPool(prevTimerListNode_p);

            ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderTimerNode = PNULL;
            listNoneCount--;
        }
        else
        {
            break;
        }
    }while ((timerListNode_p) && (listNoneCount));

    return;
}
/*****************************************************************************
 * Function Name  : startConBasedPdcchOrderTimer
 * Inputs         : expTTI - Expiry tti
 *                  ueIndex
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : None
 * Description    : This will start the timer for Contention Based PDCCH Order 
 *****************************************************************************/
/*CA Changes start  */
/* SPR 15909 fix start */
ConBasedPdcchOrderTimerNode* startConBasedPdcchOrderTimer(tickType_t expTTI, UInt16 ueIndex, InternalCellIndex internalCellIndex)
    /* SPR 15909 fix end */
    /*CA Changes end  */
{
    ConBasedPdcchOrderTimerNode *pdcchOrderTimerNode_p = PNULL;

    GET_MEM_FROM_POOL(ConBasedPdcchOrderTimerNode, 
            pdcchOrderTimerNode_p, 
            sizeof(ConBasedPdcchOrderTimerNode),
            PNULL);

    /* + Coverity 24516 */
    if( PNULL == pdcchOrderTimerNode_p )
    {
        return PNULL ;
    }
    /* - Coverity 24516 */

    pdcchOrderTimerNode_p->expTTI = expTTI;
    pdcchOrderTimerNode_p->ueIndex = ueIndex;

    /*CA Changes start  */
    listInsertNode(conBasedPDCCHOrderTimerList_gp[internalCellIndex], 
            &pdcchOrderTimerNode_p->nodeAnchor);
    /*CA Changes end  */

    LOG_MAC_MSG(PDCCH_ORDER_TIMER_START_ID,
            LOGBRIEF,
            MAC_RACH,
            getCurrentTick(),
            ueIndex,
            expTTI,
            LINE_NUMBER,
            DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"CON_BASED_PDCCHORDER_TIMER_START");

    return pdcchOrderTimerNode_p;
}

/*****************************************************************************
 * Function Name  : stopConBasedPdcchOrderTimer
 * Inputs         : pdcchOrderTimerNode_p - Pointer to the PreambleTimerNode
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : None
 * Returns        : None
 * Description    : This will stop the timer for Preamble
 *****************************************************************************/
/*CA Changes start  */
void stopConBasedPdcchOrderTimer(ConBasedPdcchOrderTimerNode*  pdcchOrderTimerNode_p, InternalCellIndex internalCellIndex)
{
    if(PNULL != pdcchOrderTimerNode_p)
    {    
        LOG_MAC_MSG(PDCCH_ORDER_TIMER_STOP_ID,
                LOGBRIEF,
                MAC_RACH,
                getCurrentTick(),
                pdcchOrderTimerNode_p->ueIndex,
                pdcchOrderTimerNode_p->expTTI,
                LINE_NUMBER,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"PDCCH_ORDER_TIMER_STOP_ID");


        listDeleteNode(conBasedPDCCHOrderTimerList_gp[internalCellIndex],
                &pdcchOrderTimerNode_p->nodeAnchor);

        freeMemPool(pdcchOrderTimerNode_p);

    }
    else
    {
        LOG_MAC_MSG(PDCCH_ORDER_TIMER_STOP_ID,
                LOGBRIEF,
                MAC_RACH,
                getCurrentTick(),
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                LINE_NUMBER,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"PDCCH_ORDER_TIMER_NODE_NULL");

    }
}
/*CA Changes end  */

/*****************************************************************************
 * Function Name  : checkConBasedPdcchOrderTimerExpiry
 *
 * Inputs         : currentTTI - Current Global TTI
 *                  listNoneCount - Timer List Count
 *                  internalCellIndex - Cell-Index at MAC                                             
 *
 * Outputs        : Process the expired timers in conBasedPDCCHOrderTimerList_g
 * Returns        : None
 * Description    : On timer expiry if the PDCCH Order Sent count 
 *                  is less than threshold
 *                       Put entry in PDCCH ORDER Queue
 *                  Else
 *                       Send Radio Link Failure To RRC
 *****************************************************************************/
/*CA Changes start  */
/* SPR 15909 fix start */
void checkConBasedPdcchOrderTimerExpiry(tickType_t currentTTI, UInt32 listNoneCount, InternalCellIndex internalCellIndex)
    /* SPR 15909 fix end */
    /*CA Changes end  */
{
    ConBasedPdcchOrderTimerNode *timerListNode_p = PNULL;
    ConBasedPdcchOrderTimerNode *prevTimerListNode_p = PNULL;
    ULUEContext *ulUEContext_p = PNULL;
    DLUEContext *ueDLContext_p  = PNULL;
    UInt16 ueIndex;
    ConFreeRACHResourcesInfo rachResourcesInfo = {0};

    timerListNode_p =
        (ConBasedPdcchOrderTimerNode *) 
        /*CA Changes start  */
        getFirstListNode(conBasedPDCCHOrderTimerList_gp[internalCellIndex]);
    /*CA Changes end  */

    if (PNULL == timerListNode_p)
    {
        return;
    }

    do
    {
        if (timerListNode_p->expTTI <= currentTTI)
        {
            /*CA Changes start  */
            listDeleteNode(conBasedPDCCHOrderTimerList_gp[internalCellIndex],
                    &timerListNode_p->nodeAnchor);
            /*CA Changes end  */

            ueIndex = timerListNode_p->ueIndex;

            if((TRUE == dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag) ||
                    (TRUE == ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag))
            {
                /* SPR 13753 fix start */
                prevTimerListNode_p = timerListNode_p;

                timerListNode_p = (ConBasedPdcchOrderTimerNode *) getNextListNode(
                        &prevTimerListNode_p->nodeAnchor);
                freeMemPool(prevTimerListNode_p);

                listNoneCount--;
                /* SPR 13753 fix end */

                continue;
            }


            ulUEContext_p =  ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
            ueDLContext_p =
                dlUECtxInfoArr_g[ueIndex].dlUEContext_p;


            LOG_MAC_MSG(PDCCH_ORDER_TIMER_EXPIRY_ID,
                    LOGINFO,
                    MAC_RACH,
                    getCurrentTick(),
                    timerListNode_p->ueIndex,
                    timerListNode_p->expTTI,
                    LINE_NUMBER,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"PDCCH_ORDER_TIMER_EXPIRY_ID");

            if(ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderSentCount 
                    < MAX_PDCCH_ORDER_RETRIES)
            {

                if(ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx == INVALID_PREAMBLE_IDX)
                {
                    /*CA Changes start  */
                    if(MAC_SUCCESS == allocateDedicatedPreamble(ueIndex,PDCCH_ORDER, &rachResourcesInfo,internalCellIndex))
                        /*CA Changes end  */
                    {
                        ueDLContext_p->ulSyncLossOrHOInfoInDL.preambleStatus = 
                            PREAMBLE_BUSY;

                        ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx = rachResourcesInfo.preambleIndex;
                        /* L2_FIX_start */
                        LOG_MAC_MSG(MAC_PDCCH_ORDER_TRIGGERED,
                                LOGWARNING,
                                MAC_PDCCH,
                                getCurrentTick(),
                                ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx,
                                ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderSentCount,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                ulUEContext_p->ueIndex,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,
                                "PreambleIdxAlloc success");

                    }
                    else
                    {
                        ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx = rachResourcesInfo.preambleIndex;

                        LOG_MAC_MSG(MAC_PDCCH_ORDER_TRIGGERED,
                                LOGWARNING,
                                MAC_PDCCH,
                                getCurrentTick(),
                                ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx,
                                ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderSentCount,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                ulUEContext_p->ueIndex,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,
                                "PreambleIdxAlloc failed");
                    }
                    /* L2_FIX_End */
                }

                putEntryInPdcchOrderQ(ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx, 
                        /*CA Changes start  */
                        ueIndex, currentTTI, internalCellIndex);
                /*CA Changes end  */
            }
            else
            {
                /* + E_CID_5.3 */
                /* If RRM sent request for TA2 and/or TA1type/AOA reports then
                 * simply send response as MAC_FAILURE
                 */
                if (NO_REPORT_TYPE_REQUIRED != 
                        ulUEContext_p->eCidReportParams.typeofReportsRequired)
                {
                    putEntryInEcidMeasReportQueue(ueIndex,\
                            NO_REPORT_TYPE_REQUIRED,\
                            MAC_FAILURE, \
                            /*CA Changes start  */
                            internalCellIndex);
                    /*CA Changes end  */
                    ulUEContext_p->eCidReportParams.typeofReportsRequired = 
                        NO_REPORT_TYPE_REQUIRED;
                }
                else
                {
                    /* - E_CID_5.3 */
                    /*SPR_3065_FIX*/
                    sendSyncIndToRRC(ueIndex, ulUEContext_p->crnti,
                            /*CA Changes start  */
                            OUT_OF_SYNC_MAX_RETRIES, internalCellIndex);
                    /*CA Changes end  */
                    /*SPR_3065_FIX*/
                }
            }

            prevTimerListNode_p = timerListNode_p;

            timerListNode_p = (ConBasedPdcchOrderTimerNode *) getNextListNode(
                    &prevTimerListNode_p->nodeAnchor);
            freeMemPool(prevTimerListNode_p);

            ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderTimerNode = PNULL;

            listNoneCount--;
        }
        else
        {
            break;
        }

    }while ((timerListNode_p) && (listNoneCount));

    return;
}

/*****************************************************************************
 * Function Name  : processRAResInNegRARPDU
 * Inputs         : freeULRBForRAR_p - Pointer to available Resources Blocks 
 *                  raRspNegAckQNode_p - Pointer to ULRARspNegAckQueueNode
 *                  maxRAR_p - Max num of RAR that 
 *                             can be scheduled in this TTI
 *                  scheduledRARNTIBitMap1_p - RARNTIs already scheduled 
 *                                             in this TTI 
 *                                             bit map (RARNTI range 0-31)
 *                  scheduledRARNTIBitMap2_p - RARNTIs already scheduled
 *                                             in this TTI 
 *                                             bit map (RARNTI range 32-59)
 *                  raResponsesInTTI_p       - RAR PDU INFO to be scheduled 
 *                                             in this TTI 
 *                  internalCellIndex - Cell-Index at MAC                                             
 *                                             
 * Outputs        : maxRAR_p - updated Max num of RAR to schedule for this TTI
 *                  scheduledRARNTIBitMap1_p - RARNTIs scheduled for this TTI
 *                                             bit map (RARNTI range 0-31)
 *                  scheduledRARNTIBitMap2_p - RARNTIs scheduled for this TTI
 *                                             bit map (RARNTI range 32-59)
 * Returns        : PREPARE_SEND or NO_RAR 
 * Description    : This function will
 *                                  - Free the previous UL grant in RAR
 *                                  - Add entry in raResponsesInTTI_p
 *                                  - Set the RARRNTI bit map 
 *                                  - Decrement Available resource blocks
 *                                  - Decrement MAX RA Responses 
 *****************************************************************************/
void processRAResInNegRARPDU(
        UInt32* freeULRBForRAR_p, 
        ULRARspNegAckQueueNode *raRspNegAckQNode_p,
        UInt32 *maxRAR_p,
        UInt32 *scheduledRARNTIBitMap1_p,
        UInt32 *scheduledRARNTIBitMap2_p,
        RarPduTobeScheduledInTTI *raResponsesInTTI_p,
        /*CA Changes start  */
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex
        /*CA Changes end  */
        /*SPR 15909 fix start*/
#ifdef TDD_CONFIG
                                ,tickType_t currentGlobalTTI
#endif
                                )
/* +- SPR 17777 */
                               /*SPR 15909 fix start*/
{
    UInt32 i = 0;
    UInt32 victimIndex = 0;
    UInt32 ueindex;
    UInt8 preambleIndex = 0,raResponseIndex=0;
    UInt8  negQueueRARCount = *maxRAR_p;
    UInt8 pushtoNegQueue = 0, count = 0;
    UInt16 tcRNTI    = INVALID_RNTI;
    UInt32 duplicatePreambleMap1 = 0;
    UInt32 duplicatePreambleMap2 = 0;

    DLRARspQueueNode  *raRspQnode_p   = PNULL;
    TempCRNTICtx      *tempCRNTICtx_p = PNULL;
    RAReqTobeScheduled *raReqInPDU_p = PNULL;
    RARspResourceInfo *raRspResourceInfo_p = PNULL;
    RAReqQueueNode* raReqQueueNode_p = PNULL;
    RAReqTobeScheduledInPdu* rarPduTobeScheduled_p = PNULL;
    RACHConfiguration *rachConfig_p = 
        /*CA Changes start  */
        &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]-> \
                cellParams_p->rachConfigInfo.rachConfiguration);
    /*CA Changes end  */
#ifdef FDD_CONFIG
            /* + SPR_17858_587_CHANGES */
    ULUEContext *ulUEContext_p = PNULL;
            /* - SPR_17858_587_CHANGES */
#endif
    ContentionFreePreamble *contFreePreamble_p = PNULL;

    rarPduTobeScheduled_p = getNextRARPduTobeScheduled(raResponsesInTTI_p);

    raRspQnode_p = raRspNegAckQNode_p->raRspQnode_p;

    for(raResponseIndex=0;raResponseIndex < raRspQnode_p->rarToDl.numRAR;
            raResponseIndex++)
    {

        raRspResourceInfo_p = 
            &raRspQnode_p->rarToDl.resourceInfo[raResponseIndex];

        /* SPR 10609 start :: tcRNTI should be a valid one */
        tcRNTI = raRspResourceInfo_p->tcRNTI;
        /* SPR 10609 end */

        raReqQueueNode_p = raRspResourceInfo_p->raReqQNode_p;

        raReqInPDU_p = 
            &rarPduTobeScheduled_p->raReqTobeScheduled[raResponseIndex];

        preambleIndex = raReqQueueNode_p->raPreambleInfo.preamble;

        if(rachConfig_p->countOfContentionBasedRAPreambles > preambleIndex)
        {
            if(rachConfig_p->preambleGroupBConfigPresent)
            {
                if(rachConfig_p->numOfPreamblesInGroupA > preambleIndex)
                {
                    if(((*freeULRBForRAR_p) <  
                                rachConfig_p->numRBReqdForMSG3GroupARACHRequest) || 
                            (!(*maxRAR_p))) 
                    {
                        pushtoNegQueue = 1;
                        break;
                    }
                    else
                    {
                        (*freeULRBForRAR_p) -= 
                            rachConfig_p->numRBReqdForMSG3GroupARACHRequest; 

                        (*maxRAR_p)--;

                        raReqInPDU_p->numRBAllocated = 
                            rachConfig_p->numRBReqdForMSG3GroupARACHRequest;

                        raReqInPDU_p->mcsIndex = 
                            rachConfig_p->mcsIndexForMSG3GroupARACHRequest;


                    }
                }
                else
                {
                    if(((*freeULRBForRAR_p) <  
                                rachConfig_p->numRBReqdForMSG3GroupBRACHRequest) || 
                            (!(*maxRAR_p))) 
                    {
                        pushtoNegQueue = 1;
                        break;
                    }
                    else
                    {
                        (*freeULRBForRAR_p) -= 
                            rachConfig_p->numRBReqdForMSG3GroupBRACHRequest; 

                        (*maxRAR_p)--;

                        raReqInPDU_p->numRBAllocated = 
                            rachConfig_p->numRBReqdForMSG3GroupBRACHRequest;

                        raReqInPDU_p->mcsIndex = 
                            rachConfig_p->mcsIndexForMSG3GroupBRACHRequest;

                    }
                }
            }
            else
            {
                if(((*freeULRBForRAR_p) <  
                            rachConfig_p->numRBReqdForMSG3GroupARACHRequest) || 
                        (!(*maxRAR_p))) 
                {
                    pushtoNegQueue = 1;
                    break;
                }
                else
                {
                    (*freeULRBForRAR_p) -= 
                        rachConfig_p->numRBReqdForMSG3GroupARACHRequest; 

                    (*maxRAR_p)--;

                    raReqInPDU_p->numRBAllocated = 
                        rachConfig_p->numRBReqdForMSG3GroupARACHRequest;

                    raReqInPDU_p->mcsIndex = 
                        rachConfig_p->mcsIndexForMSG3GroupARACHRequest;


                }
            }

            /*CA Changes start  */
            tempCRNTICtx_p = getTempCRNTIContextPointer(tcRNTI, internalCellIndex);
            /*CA Changes end  */

            if (tempCRNTICtx_p)
            {
                /* + SPR_14496 */
                /* SPR 19036 start*/
                ulHarqTimerForMsg3Stop(tempCRNTICtx_p,(raRspQnode_p->rarToDl.tti % MAX_SUB_FRAME),
                        (raRspQnode_p->rarToDl.tti / MAX_SUB_FRAME) % MAX_SFN_VALUE, internalCellIndex);
                /* SPR 19036 end*/
                /* - SPR_14496 */
                /* +- SPR 22322 */

            }
        }
        else
        {
            /* EICIC Change Phase1 */	
            contFreePreamble_p = 
                /*CA Changes start  */
                getContentionFreePreambleInfo(preambleIndex, internalCellIndex);
            /*CA Changes end  */
            ueindex=contFreePreamble_p->ueIdentifier;
            /* SPR 15914 Fix Start */
            if(INVALID_UE_INDEX !=ueindex)
                /* SPR 15914 Fix End */
            {	                   	
                if((VICTIM_UE == ulUECtxInfoArr_g[ueindex].ulUEContext_p->userType) && (LTE_FALSE == isCurrentSfAbsV_g[internalCellIndex]))
                {    

                    pushtoNegQueue=1;
                    raRspQnode_p->rarToDl.resourceInfo[victimIndex].tcRNTI  = 
                        raRspQnode_p->rarToDl.
                        resourceInfo[raResponseIndex].tcRNTI;
                    raRspQnode_p->rarToDl.resourceInfo[victimIndex].ueIndex= 
                        raRspQnode_p->rarToDl.
                        resourceInfo[raResponseIndex].ueIndex;

                    raRspQnode_p->rarToDl.resourceInfo[victimIndex].isCqiWithCFRPresent= 
                        raRspQnode_p->rarToDl.
                        resourceInfo[raResponseIndex].isCqiWithCFRPresent;

                    raRspQnode_p->rarToDl.resourceInfo[victimIndex].riStart= 
                        raRspQnode_p->rarToDl.
                        resourceInfo[raResponseIndex].riStart;

                    raRspQnode_p->rarToDl.resourceInfo[victimIndex].riLength= 
                        raRspQnode_p->rarToDl.
                        resourceInfo[raResponseIndex].riLength;

                    raRspQnode_p->rarToDl.resourceInfo[victimIndex].harqProcessId= 
                        raRspQnode_p->rarToDl.
                        resourceInfo[raResponseIndex].harqProcessId;

                    raRspQnode_p->rarToDl.resourceInfo[victimIndex].raReqQNode_p = 
                        raRspQnode_p->rarToDl.
                        resourceInfo[raResponseIndex].raReqQNode_p;
                    victimIndex++;
                    continue;
                }		 
            }
            /* EICIC Change Phase1 */	

            /* + E_CID_5.3 */
#ifdef TDD_CONFIG
            UInt8 subFrameForMsg3 = currentGlobalTTI % MAX_SUB_FRAME ;
            /* TDD Config 0,6 Changes Start */
            UInt16 sysFrameForMsg3 = (currentGlobalTTI / MAX_SUB_FRAME) % MAX_SFN_VALUE;
            UInt8 dci0SubFrameForMsg3 = 0;
            UInt8 ulSubFrameForMsg3 = 0;
            UInt8 ulSysFrameForMsg3 = 0;

            /* Calculate DCI0 Subframe number and system frame number for Msg3 */
            dci0SubFrameForMsg3 = getTickForPUCCHCalForMsg3(subFrameForMsg3,internalCellIndex) + subFrameForMsg3;

            if (MAX_SUB_FRAME <= dci0SubFrameForMsg3)
            {
                dci0SubFrameForMsg3 %= MAX_SUB_FRAME;
                sysFrameForMsg3 = (sysFrameForMsg3 + 1) % MAX_SFN_VALUE;
            }
            /* Calculate Msg3 Subframe number and system frame number */
            ulSubFrameForMsg3 = (subFrameForMsg3 + getULSfForMsg3(subFrameForMsg3,internalCellIndex)) % MAX_SUB_FRAME;
            if (dci0SubFrameForMsg3 > ulSubFrameForMsg3)
            {
                ulSysFrameForMsg3 = (sysFrameForMsg3 + 1) % MAX_SFN_VALUE;
            }
#ifdef MAC_AUT_TEST
            testProcessRAResInNegRARPDU_g.subFrameForRAR = subFrameForMsg3;
            testProcessRAResInNegRARPDU_g.dci0SubFrameForMsg3 = dci0SubFrameForMsg3;
            testProcessRAResInNegRARPDU_g.subFrameForMsg3 = ulSubFrameForMsg3;
            testProcessRAResInNegRARPDU_g.sysFrameNumForRAR = sysFrameForMsg3;
            testProcessRAResInNegRARPDU_g.sysFrameNumForMsg3 = ulSysFrameForMsg3;
#endif
            /* TDD Config 0,6 Changes End */
#endif            

            /* + E_CID_5.3 */
            if(PDCCH_ORDER == 
                    contFreePreamble_p->pdcchOrderHandOverFlag)
            {     
                if(((*freeULRBForRAR_p) < 
                            rachConfig_p->numRBReqdForConFreePDCCHOrder) || (!(*maxRAR_p)))
                {
                    pushtoNegQueue = 1;
                    break;
                }
                else
                {
                    (*freeULRBForRAR_p) -= 
                        rachConfig_p->numRBReqdForConFreePDCCHOrder;
                    (*maxRAR_p)--;

                    raReqInPDU_p->numRBAllocated = 
                        rachConfig_p->numRBReqdForConFreePDCCHOrder;

                    raReqInPDU_p->mcsIndex = 
                        rachConfig_p->mcsIndexForConFreePDCCHOrder;
                    /* + E_CID_5.3 */
#ifdef TDD_CONFIG
                    /* TDD Config 0,6 Changes Start */
                    resetHarqId( ulSubFrameForMsg3, ulUECtxInfoArr_g[contFreePreamble_p->ueIdentifier].ulUEContext_p,internalCellIndex,ulSysFrameForMsg3);
                    /* TDD Config 0,6 Changes End */
#endif                    
                    /* - E_CID_5.3 */
                }
            }
            else if(CONNECTION_HANDOVER == 
                    contFreePreamble_p->pdcchOrderHandOverFlag)
            {
                if(((*freeULRBForRAR_p) < 
                            rachConfig_p->numRBReqdForConFreeHandOver) || (!(*maxRAR_p)))
                {
                    pushtoNegQueue = 1;
                    break;
                }
                else
                {
                    (*freeULRBForRAR_p) -= 
                        rachConfig_p->numRBReqdForConFreeHandOver;
                    (*maxRAR_p)--;

                    raReqInPDU_p->numRBAllocated = 
                        rachConfig_p->numRBReqdForConFreeHandOver;

                    raReqInPDU_p->mcsIndex = 
                        rachConfig_p->mcsIndexForConFreeHandOver;

                }

            }
#ifdef FDD_CONFIG
            /* + SPR_17858_587_CHANGES */
            ulUEContext_p = ulUECtxInfoArr_g[\
                            contFreePreamble_p->ueIdentifier].ulUEContext_p;
            /*Here rcvTTI is passed as 0 as it is no used in
             *           * function
             *                     */
            if (ulUEContext_p)
            {
                /* +- SPR 17777 */
		
               if(MAC_FAILURE == (ulHarqTimerStop(ulUEContext_p,\
                        ulUEContext_p->ulresInfo[raRspResourceInfo_p->harqProcessId].harqTimerExpirySF,
                        ulUEContext_p->ulresInfo[raRspResourceInfo_p->harqProcessId].harqTimerExpirySystemFrame
                        ,internalCellIndex)))
		{
			LOG_MAC_MSG( MAC_UL_HARQ_TIMER_STOP_FAIL, LOGWARNING, MAC_UL_HARQ,
                                getCurrentTick(), __LINE__, ulUEContext_p->ueIndex, preambleIndex,
                                ulUEContext_p->ulresInfo[raRspResourceInfo_p->harqProcessId].harqTimerExpirySF, contFreePreamble_p->ueIdentifier, 0,0, __func__, "");
		}
            }
            else
            {
/* SPR 20636 Changes Start */
                LOG_MAC_MSG(PREAMBLE_NO_UECONTEXT_ID,
                        LOGERROR,
                        MAC_RACH,
                        getCurrentTick(),
                        preambleIndex,
                        contFreePreamble_p->pdcchOrderHandOverFlag,
                        contFreePreamble_p->ueIdentifier,
                        LINE_NUMBER,
                        (ADDR)ulUEContext_p/* SPR_17858_587_CHANGES*/,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"NO_ULUECONTEXT_ID");
/* SPR 20636 Changes End */
            }
           /* -SPR_17858_587_CHANGES */
#endif

        }

        raReqInPDU_p->raReqQNode_p = 
            raRspResourceInfo_p->raReqQNode_p;

        raReqInPDU_p->tcRNTI = tcRNTI;
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        freeULHarqProcess(raRspResourceInfo_p->ulHarq,INVALID_UE_INDEX,internalCellIndex);
#else
        freeULHarqProcess(raRspResourceInfo_p->ulHarq);
#endif
        /* SPR 16916 FIX END */


        rarPduTobeScheduled_p->noOfRAResponses++;


        SET_BIT_FOR_PREAMBLE(raReqQueueNode_p, 
                duplicatePreambleMap1, 
                duplicatePreambleMap2);


        LOG_MAC_MSG(RA_REQ_PROC_ID,
                LOGBRIEF,
                MAC_RACH,
                getCurrentTick(),
                raReqQueueNode_p->raRNTI,
                raReqQueueNode_p->ttiRAReqReceived,
                preambleIndex,
                (*maxRAR_p),
                (*freeULRBForRAR_p),
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"NEG_RA_REQ_PROC_ID");

    }

    if(negQueueRARCount != *maxRAR_p) 
    {
        if (raRspQnode_p->rarToDl.raRNTI > 31)
        {
            *scheduledRARNTIBitMap2_p |= 1 << (raRspQnode_p->rarToDl.raRNTI - 31);
        }
        else
        {
            *scheduledRARNTIBitMap1_p |= 1 << raRspQnode_p->rarToDl.raRNTI;
        }

        /*Free the existing UL RBs allocated*/
        FREE_UL_RAR_RB(raRspQnode_p->rarToDl, 
                i, raRspQnode_p->rarToDl.numRAR,
                /*CA Changes start  */
                internalCellIndex);
        /*CA Changes end  */


        rarPduTobeScheduled_p->raRnti = raRspQnode_p->rarToDl.raRNTI;

        rarPduTobeScheduled_p->BIFlag = raRspQnode_p->rarToDl.BIFlag;

        rarPduTobeScheduled_p->exptti = raRspQnode_p->rarToDl.exptti;

        raResponsesInTTI_p->numRarPdu++;

        rarPduTobeScheduled_p->preambleMap1 = duplicatePreambleMap1;

        rarPduTobeScheduled_p->preambleMap2 = duplicatePreambleMap2;

        LOG_MAC_MSG(RAR_PDU_PROC_ID,
                LOGDEBUG,
                MAC_RACH,
                getCurrentTick(),
                raResponsesInTTI_p->numRarPdu,
                rarPduTobeScheduled_p->noOfRAResponses,
                rarPduTobeScheduled_p->raRnti,
                rarPduTobeScheduled_p->BIFlag,
                LINE_NUMBER,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"NEG_RAR_PDU_PROC_ID");

        LOG_MAC_MSG(RAR_RARNTI_MAP_ID,
                LOGDEBUG,
                MAC_RACH,
                getCurrentTick(),
                rarPduTobeScheduled_p->raRnti,
                (*scheduledRARNTIBitMap1_p),
                (*scheduledRARNTIBitMap2_p),
                duplicatePreambleMap1,
                duplicatePreambleMap2,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"NEG_RAR_RARNTI_MAP_ID");


    }
    if(pushtoNegQueue)
    {
        count = raRspQnode_p->rarToDl.numRAR;

        /* EICIC Change Phase1 */	
        for (i = victimIndex; (i + raResponseIndex)  < count+victimIndex; i++)
        {
            /* EICIC Change Phase1 */	
            raRspQnode_p->rarToDl.resourceInfo[i].tcRNTI  = 
                raRspQnode_p->rarToDl.
                resourceInfo[i + raResponseIndex].tcRNTI;

            raRspQnode_p->rarToDl.resourceInfo[i].raReqQNode_p = 
                raRspQnode_p->rarToDl.
                resourceInfo[i + raResponseIndex].raReqQNode_p;

            raRspQnode_p->rarToDl.resourceInfo[i].ueIndex= 
                raRspQnode_p->rarToDl.
                resourceInfo[i + raResponseIndex].ueIndex;

            raRspQnode_p->rarToDl.resourceInfo[i].isCqiWithCFRPresent= 
                raRspQnode_p->rarToDl.
                resourceInfo[i + raResponseIndex].isCqiWithCFRPresent;

            raRspQnode_p->rarToDl.resourceInfo[i].riStart= 
                raRspQnode_p->rarToDl.
                resourceInfo[i + raResponseIndex].riStart;

            raRspQnode_p->rarToDl.resourceInfo[i].riLength= 
                raRspQnode_p->rarToDl.
                resourceInfo[i + raResponseIndex].riLength;

            raRspQnode_p->rarToDl.resourceInfo[i].harqProcessId= 
                raRspQnode_p->rarToDl.
                resourceInfo[i + raResponseIndex].harqProcessId;

        }

        raRspQnode_p->rarToDl.numRAR -= (raResponseIndex-victimIndex);
        freeMemPool(raRspNegAckQNode_p);
        /*CA Changes start  */
        putEntryInULNegAckRARspQueue(raRspQnode_p, internalCellIndex);
        /*CA Changes end  */
    }
    else
    {    
        LOG_MAC_MSG(MAC_FREE_RAR_ID,LOGWARNING,MAC_RACH,
                getCurrentTick(),
                raRspQnode_p->rarToDl.raRNTI,
                raRspQnode_p->rarToDl.mcsIndex,
                raRspQnode_p->rarToDl.exptti,
                raRspQnode_p->rarToDl.numRAR,
                LINE_NUMBER,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"NEG_RAR_FREE");


        msgFree(raRspQnode_p->rarToDl.macPduData_p);
        freeMemPool(raRspQnode_p);
        freeMemPool(raRspNegAckQNode_p);
    }
}


/*******************************************************************************
 * Function Name  : processContFreeRAReqQueue
 * Inputs         : freeULRBForRAR_p - Pointer to available Resources Blocks 
 *                  maxRAR_p - Max num of RAR that 
 *                             can be scheduled in this TTI
 *                  currentGlobalTTI - Curret TTI
 *                  dlDelay - DL delay
 *                  scheduledRARNTIBitMap1_p - RARNTIs already scheduled 
 *                                             in this TTI 
 *                                             bit map (RARNTI range 0-31)
 *                  scheduledRARNTIBitMap2_p - RARNTIs already scheduled
 *                                             in this TTI 
 *                                             bit map (RARNTI range 32-59)
 *                  raResponsesInTTI_p       - RAR PDU INFO to be scheduled 
 *                                             in this TTI 
 *                  internalCellIndex - Cell-Index at MAC                                             
 * Outputs        : maxRAR_p - updated Max num of RAR to schedule for this TTI
 *                  scheduledRARNTIBitMap1_p - RARNTIs scheduled for this TTI
 *                                             bit map (RARNTI range 0-31)
 *                  scheduledRARNTIBitMap2_p - RARNTIs scheduled for this TTI
 *                                             bit map (RARNTI range 32-59)
 * Returns        : PREPARE_SEND or NO_RAR 
 * Description    : This function will 
 *                                  - Process Contention Free RA Queue
 *                                  - Add entry in raResponsesInTTI_p
 *                                    for each RAR PDU
 *                                  - Set the RARRNTI bit map 
 *                                    for each RAR PDU
 *                                  - Decrement Available resource blocks
 *                                  - Decrement MAX RA Responses 
 ******************************************************************************/
void processContFreeRAReqQueue(
        UInt32* freeULRBForRAR_p,
        UInt32 *maxRAR_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTI,
        /* SPR 15909 fix end */
        UInt32 dlDelay,
        UInt32 *scheduledRARNTIBitMap1_p,
        UInt32 *scheduledRARNTIBitMap2_p,
        RarPduTobeScheduledInTTI *raResponsesInTTI_p,
        /*CA Changes start  */
        InternalCellIndex  internalCellIndex)
/*CA Changes end  */
{
    UInt8 *noOfRAResponses_p = PNULL;
    /*EICIC*/
    UInt8 victimStatus=0;
    UInt8 queueCount=0;
    /*EICIC*/
    UInt8  processRntiFlag = 0;
    UInt8  preambleIndex = 0;
    /*CA Changes start  */
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /*CA Changes end  */
#ifdef FDD_CONFIG
    UInt8 tempHarqProcessId = INVALID_HARQ_ID; 
#elif TDD_CONFIG
    UInt8 tempHarqProcessId = INVALID_HARQ_ID;
    UInt8 subFrameForRAR = 
        ((currentGlobalTTI + dlDelay + PHY_DL_DELAY) % MAX_SUB_FRAME);
    /*TDD Config 0 Changes Start*/
    /*UL SF on which data needs to come*/
    UInt8 subFrameForMsg3 = 0;
    /*TDD Config 0 changes End*/
    /*SPR 3114 Start*/
    UInt8 dci0SubFrameForMsg3 = MAX_SUB_FRAME;
    /*SPR 3114 End*/
    /* TDD Config 6 Changes Start */
    UInt16 sysFrameNumForRAR = 
        ((currentGlobalTTI + dlDelay + PHY_DL_DELAY) / MAX_SUB_FRAME) % 
        MAX_SFN_VALUE;
    UInt16 dci0SysFrameNumForMsg3 = MAX_SFN_VALUE;
    /* SPR 11257 Changes Start*/
    /*SPR 15909 fix start*/
    tickType_t dci0Tick = currentGlobalTTI + dlDelay + PHY_DL_DELAY;
    /*SPR 15909 fix end*/
    dci0Tick = dci0Tick + getTickForPUCCHCalForMsg3(dci0Tick % MAX_SUB_FRAME,internalCellIndex);
    /* SPR 11257 Changes End*/
    /* TDD Config 6 Changes End */
#endif
    UInt16 tempRaRnti    = 0;
    UInt16 raRNTI           = 0;
    UInt16 ueIndex           = 0;
    UInt32 raRspWindowSize  = 0;
    /* SPR 15909 fix start */
    tickType_t dlTxTTI          = 0;
    tickType_t rarWindowEndTTI  = 0;
    /* SPR 15909 fix end */
    RAReqQueueNode *raReqQNode_p  = PNULL;
    RAReqTobeScheduledInPdu* rarPduTobeScheduled_p = PNULL;
    ContentionFreePreamble *contFreePreamble_p = PNULL;
    LTE_SQUEUE  *contentionFreeRAReqQueue_p = PNULL;
    ULHarqInfo*   tempULHarqInfo_p = PNULL; 
    ULUEContext* ulUEContext_p=PNULL;
    RAReqTobeScheduled *raReqInPDU_p = PNULL;
    UInt8  numRARCnt     = 0;
    RACHConfiguration *rachConfig_p = 
        /*CA Changes start  */
        &(cellParams_p->rachConfigInfo.rachConfiguration);
    /*CA Changes end  */
    /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
    /* + PERF_CA */
    LteCellMacULPerfStats *lteCellMacULPerfStats = &(gMacCellPerfStats_p[internalCellIndex]-> \
            lteCellMacULPerfStats);
    /* - PERF_CA */
#endif
    /* - PERF_STATS_PH1D1 */

    /* SPR 3854 Start*/
    /*CA Changes start  */
    contentionFreeRAReqQueue_p = contentionFreeRAReqQueue_gp[internalCellIndex];
    /*CA Changes end  */
    /* SPR 3854 End*/
    /*EICIC*/
    queueCount=sQueueCount(contentionFreeRAReqQueue_gp[internalCellIndex]);
    /*EICIC*/
    raReqQNode_p = (RAReqQueueNode *) 
        getFirstNode(contentionFreeRAReqQueue_p);

    if (PNULL == raReqQNode_p)
    { 
        return;
    }

    /*SPR 3854 Start*/
    if ((raReqQNode_p->ttiRAReqReceived + RAR_WINDOW_START_DELAY) > 
            currentGlobalTTI + dlDelay + PHY_DL_DELAY)
    {
        return; 
    } 
    /*SPR 3854 End*/
    /*CA Changes start  */
    raRspWindowSize = 
        cellParams_p->rachConfigInfo.rachConfiguration.
        raRspWindowSize[
        cellParams_p->rachConfigInfo.rachConfiguration.activeIdxRARspWindow];
    /*CA Changes end  */

    /*save the RARNTI*/
    tempRaRnti = raReqQNode_p->raRNTI;

    processRntiFlag = 0x0;

    /*Maximum nuber of RARs to be in a sigle RAR PDU*/

    numRARCnt = *maxRAR_p;
    /* Cyclomatic_complexity_changes_start */
    rarPduTobeScheduled_p = getRarPduToSchedule(raReqQNode_p,
            scheduledRARNTIBitMap1_p,
            scheduledRARNTIBitMap2_p,
            raResponsesInTTI_p,
            &processRntiFlag);
    /* Cyclomatic_complexity_changes_end */

    /*EICIC*/
    while(*maxRAR_p && queueCount)
        /*EICIC*/
        {
            if(((*freeULRBForRAR_p) < 
                        rachConfig_p->numRBReqdForConFreeHandOver) ||
                    (*freeULRBForRAR_p) <   
                    rachConfig_p->numRBReqdForConFreePDCCHOrder)

            {
                break;
            }
            raReqQNode_p = (RAReqQueueNode *) 
                getFirstNode(contentionFreeRAReqQueue_p);

            if (raReqQNode_p)
            {
                /*SPR 3854 Start*/
                if ((raReqQNode_p->ttiRAReqReceived + RAR_WINDOW_START_DELAY) > 
                        currentGlobalTTI + dlDelay + PHY_DL_DELAY)
                {
                    break; 
                } 
                /*SPR 3854 End*/
                if (tempRaRnti != raReqQNode_p->raRNTI)  
                {
                    if(numRARCnt != *maxRAR_p) 
                    {
                        if(!(processRntiFlag))
                        {    
                            if (raRNTI > 31)
                            {
                                *scheduledRARNTIBitMap2_p |= 1 << (raRNTI - 31);
                            }
                            else
                            {
                                *scheduledRARNTIBitMap1_p |= 1 << raRNTI;
                            }

                            raResponsesInTTI_p->numRarPdu++;

                            LOG_MAC_MSG(RAR_PDU_PROC_ID,
                                    LOGDEBUG,
                                    MAC_RACH,
                                    getCurrentTick(),
                                    raResponsesInTTI_p->numRarPdu,
                                    rarPduTobeScheduled_p->noOfRAResponses,
                                    rarPduTobeScheduled_p->raRnti,
                                    rarPduTobeScheduled_p->BIFlag,
                                    LINE_NUMBER,
                                    DEFAULT_FLOAT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"CFR_RAR_PDU_PROC_ID");

                            LOG_MAC_MSG(RAR_RARNTI_MAP_ID,
                                    LOGDEBUG,
                                    MAC_RACH,
                                    getCurrentTick(),
                                    rarPduTobeScheduled_p->raRnti,
                                    (*scheduledRARNTIBitMap1_p),
                                    (*scheduledRARNTIBitMap2_p),
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"CFR_RAR_RARNTI_MAP_ID");

                        }
                        else
                        {
                            processRntiFlag = 0;
                        }

                        numRARCnt = *maxRAR_p;

                        rarPduTobeScheduled_p = 
                            getNextRARPduTobeScheduled(raResponsesInTTI_p);

                    }

                    tempRaRnti = raReqQNode_p->raRNTI;
                }

                /*Check are we able to schedule the message with in RAR window*/
                /*TTI in which RAR message should be sent from PHY*/
                dlTxTTI = currentGlobalTTI;

                /*TTI in which RAR window will expire minus pdcch delay and 
                 *phy dl delay
                 */
                /* + SPR_17858_649_CHANGES */
                rarWindowEndTTI = 
                    (raReqQNode_p->ttiRAReqReceived + 
                     RAR_WINDOW_START_DELAY + raRspWindowSize) - 
                    (dlDelay + PHY_DL_DELAY) -1;
                /* - SPR_17858_649_CHANGES */
                if (dlTxTTI > rarWindowEndTTI)
                {
                    raReqQNode_p = (RAReqQueueNode *) 
                        popNode(contentionFreeRAReqQueue_p);

                    /* EICIC Change Phase1 */
                    queueCount--;
                    if (raReqQNode_p)
                    {
                        LOG_MAC_MSG(RA_REQ_FREE_ID,
                                LOGDEBUG,
                                MAC_RACH,
                                getCurrentTick(),
                                raReqQNode_p->raRNTI,
                                raReqQNode_p->ttiRAReqReceived,
                                raReqQNode_p->raPreambleInfo.preamble,
                                LINE_NUMBER,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"CFR_FREE_TTI_EXPIRY");

                        freeMemPool(raReqQNode_p);

                        /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                        lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CFRA++;
#endif
                        /* - PERF_STATS_PH1D1 */
                    }
                    continue;
                }
                else 
                {
                    raReqQNode_p = (RAReqQueueNode *)  
                        popNode(contentionFreeRAReqQueue_p);
                    /* EICIC Change Phase1 */
                    queueCount--;

                    if (PNULL == raReqQNode_p)
                    { 
                        continue;
                    }

                    noOfRAResponses_p = 
                        &rarPduTobeScheduled_p->noOfRAResponses;

                    rarPduTobeScheduled_p->raRnti = raReqQNode_p->raRNTI;

                    rarPduTobeScheduled_p->exptti = rarWindowEndTTI;

                    preambleIndex =
                        raReqQNode_p->raPreambleInfo.preamble;


                    /*CA Changes start  */
                    if(PREAMBLE_BUSY != getPreambleStatusInfo(preambleIndex, internalCellIndex))
                        /*CA Changes end  */
                    {
                        LOG_MAC_MSG(CFR_RA_REQ_FREE_ID,
                                LOGERROR,
                                MAC_RACH,
                                getCurrentTick(),
                                raReqQNode_p->raRNTI,
                                raReqQNode_p->ttiRAReqReceived,
                                raReqQNode_p->raPreambleInfo.preamble,
                                raReqQNode_p->raPreambleInfo.tmcs,
                                LINE_NUMBER,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"CFR_FREE_PREAMBLE_NOT_BUSY");

                        freeMemPool(raReqQNode_p);

                        /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                        lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CFRA++;
#endif
                        /* - PERF_STATS_PH1D1 */

                        continue;
                    }

                    contFreePreamble_p = 
                        /*CA Changes start  */
                        getContentionFreePreambleInfo(preambleIndex, internalCellIndex);
                    /*CA Changes end  */

                    ueIndex = contFreePreamble_p->ueIdentifier;

                    ulUEContext_p =
                        ulUECtxInfoArr_g[ueIndex].ulUEContext_p;

                    if(PNULL == ulUEContext_p)
                    {

                        LOG_MAC_MSG(PREAMBLE_NO_UECONTEXT_ID,
                                LOGERROR,
                                MAC_RACH,
                                getCurrentTick(),
                                preambleIndex,
                                contFreePreamble_p->pdcchOrderHandOverFlag,
                                contFreePreamble_p->ueIdentifier,
                                LINE_NUMBER,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"PREAMBLE_NO_DLUECONTEXT_ID");

                        /*L2_FIX_start*/
                        LOG_MAC_MSG(CFR_RA_REQ_FREE_ID,
                                LOGWARNING,
                                MAC_RACH,
                                getCurrentTick(),
                                raReqQNode_p->raRNTI,
                                raReqQNode_p->ttiRAReqReceived,
                                raReqQNode_p->raPreambleInfo.preamble,
                                raReqQNode_p->raPreambleInfo.tmcs,
                                LINE_NUMBER,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"CFR_FREE_NO_DLUECONTEXT");
                        /*L2_FIX_end*/

                        freeMemPool(raReqQNode_p);

                        /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                        lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CFRA++;
#endif
                        /* - PERF_STATS_PH1D1 */

                        continue;

                    }
                    /*EICIC*/
                    victimStatus=ulUEContext_p->userType;

                    if((VICTIM_UE == victimStatus) && (LTE_FALSE == isCurrentSfAbsV_g[internalCellIndex]))
                    {
                        if(pushNode (contentionFreeRAReqQueue_gp[internalCellIndex], &(raReqQNode_p->raNodeAnchor))) 
                        {
                            /* Failure condition : 1 - Failure, 0-Success */
                            freeMemPool(raReqQNode_p);
                            raReqQNode_p = PNULL; 
                        }
                        continue;
                    }
                    /*EICIC*/

#ifdef TDD_CONFIG
                    /* TDD Config 0 Changes Start */
                    subFrameForMsg3 = (subFrameForRAR + getULSfForMsg3(subFrameForRAR,internalCellIndex)) % MAX_SUB_FRAME;
                    /* TDD Config 0 Changes End */
                    /* SPR 3114 Start */
                    dci0SubFrameForMsg3 = getTickForPUCCHCalForMsg3(subFrameForRAR, internalCellIndex) + subFrameForRAR;
                    if (MAX_SUB_FRAME <= dci0SubFrameForMsg3)
                    {    
                        dci0SubFrameForMsg3 %= MAX_SUB_FRAME;
                    }    

                    /* TDD Config 6 Changes Start */
                    if( MAX_SUB_FRAME <= ( subFrameForRAR +
                                getTickForPUCCHCalForMsg3(subFrameForRAR,internalCellIndex)) )
                    {
                        dci0SysFrameNumForMsg3 = (sysFrameNumForRAR + 1) %
                            MAX_SFN_VALUE;
                    }
                    else
                    {
                        dci0SysFrameNumForMsg3 = sysFrameNumForRAR;
                    }

                    /** CA-TDD Changes Start:05 **/

                    getULHarqProcessIdForCurrentSubframeForMsg3(dci0SubFrameForMsg3,
                            &tempHarqProcessId,
                            ulUEContext_p,internalCellIndex,
                            dci0SysFrameNumForMsg3,
                            /* TDD Config 0 Changes Start */
                            /* SPR 11257 Changes Start*/
                            subFrameForMsg3,
                            dci0Tick 
                            /* SPR 11257 Changes End*/
                            /* TDD Config 0 Changes End */
                            );
                    /* TDD Config 6 Changes End */
                    /*TDD Config 0 Changes Start*/
#ifdef MAC_AUT_TEST
                    testProcessContFreeRAReqQueue_g.subFrameForRAR = subFrameForRAR; 
                    testProcessContFreeRAReqQueue_g.dci0SubFrameForMsg3 = dci0SubFrameForMsg3;
                    testProcessContFreeRAReqQueue_g.subFrameForMsg3 = subFrameForMsg3;
                    testProcessContFreeRAReqQueue_g.sysFrameNumForRAR = sysFrameNumForRAR;
                    testProcessContFreeRAReqQueue_g.dci0SysFrameNumForMsg3 = dci0SysFrameNumForMsg3;
                    testProcessContFreeRAReqQueue_g.harqProcessId = tempHarqProcessId;
#endif
                    /*TDD Config 0 Changes End*/
                    /** CA-TDD Changes End:05 **/
                    /* SPR 3114 End */
                    /* + COVERITY 5.0 */

                    /*L2_FIX_start*/
                    if (BUSY_ALL_HARQ == tempHarqProcessId)
                    {
                        LOG_MAC_MSG(MAC_ALL_HARQ_ARE_BUSY,
                                LOGFATAL,
                                MAC_UL_HARQ,
                                getCurrentTick(),
                                LINE_NUMBER,
                                tempHarqProcessId,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                __FILE__,
                                FUNCTION_NAME);
                        continue;
                    }
                    /*L2_FIX_end*/

                    /* - COVERITY 5.0 */
#elif FDD_CONFIG
                    tempHarqProcessId = 
                        MODULO_EIGHT((currentGlobalTTI + PHY_DL_DELAY  /* PDCCHDelay */ + 
                                    PHY_DL_DELAY + UE_RA_ADV_TTI));
#endif

                    tempULHarqInfo_p = &(ulUEContext_p->ulresInfo[tempHarqProcessId]);

                    if (tempULHarqInfo_p->riLength != 0 )
                    {

                        LOG_MAC_MSG(CFR_ULHARQ_NOT_FREE,
                                LOGDEBUG,
                                MAC_RACH,
                                getCurrentTick(),
                                ueIndex,
                                tempHarqProcessId,
                                tempULHarqInfo_p->riStart, 
                                tempULHarqInfo_p->riLength,
                                tempULHarqInfo_p->riv,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"HARQ_NOT_FREE");

                        LOG_MAC_MSG(CFR_RA_REQ_FREE_ID,
                                LOGERROR,
                                MAC_RACH,
                                getCurrentTick(),
                                raReqQNode_p->raRNTI,
                                raReqQNode_p->ttiRAReqReceived,
                                raReqQNode_p->raPreambleInfo.preamble,
                                raReqQNode_p->raPreambleInfo.tmcs,
                                LINE_NUMBER,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"CFR_FREE_NO_ULUECONTEXT");

                        if(PDCCH_ORDER == 
                                contFreePreamble_p->pdcchOrderHandOverFlag)
                        {
                            if(ulUEContext_p->ulSyncLossOrHOInfoInUL.pdcchOrderSentCount 
                                    < MAX_PDCCH_ORDER_RETRIES)
                            {
                                putEntryInPdcchOrderQ(ulUEContext_p->ulSyncLossOrHOInfoInUL.preambleIdx,
                                        ulUEContext_p->ueIndex,
                                        /*CA Changes start  */
                                        currentGlobalTTI, internalCellIndex);
                                /*CA Changes end  */
                            }
                            else
                            {
                                /* + E_CID_5.3 */
                                /* If RRM sent request for TA2 and/or TA1type/AOA reports then
                                 * simply send response as MAC_FAILURE
                                 */
                                if (NO_REPORT_TYPE_REQUIRED != 
                                        ulUEContext_p->eCidReportParams.typeofReportsRequired)
                                {
                                    putEntryInEcidMeasReportQueue(ueIndex,\
                                            NO_REPORT_TYPE_REQUIRED,\
                                            MAC_FAILURE, \
                                            /*CA Changes start  */
                                            internalCellIndex);
                                    /*CA Changes end  */
                                    ulUEContext_p->eCidReportParams.typeofReportsRequired = 
                                        NO_REPORT_TYPE_REQUIRED;
                                }
                                else
                                {
                                    /* - E_CID_5.3 */
                                    /*SPR_3065_FIX*/
                                    sendSyncIndToRRC(ueIndex, ulUEContext_p->crnti,
                                            /*CA Changes start  */
                                            OUT_OF_SYNC_MAX_RETRIES, internalCellIndex);
                                    /*CA Changes end  */
                                    /*SPR_3065_FIX*/
                                }
                                /* Free the preamble */
                                /*CA Changes start  */
                                freePreambleIndex(
                                        raReqQNode_p->raPreambleInfo.preamble, internalCellIndex);
                                /*CA Changes end  */

                                /* + PERF_STATS_PH1D1 */
#ifdef PERF_STATS
                                lteCellMacULPerfStats->totalFailedRandomAccessPreamblesMsg2CFRA++;
#endif
                                /* - PERF_STATS_PH1D1 */
                            }
                        }
                        freeMemPool(raReqQNode_p);
                        continue;

                    }

                    raReqInPDU_p =  &(rarPduTobeScheduled_p->
                            raReqTobeScheduled[*noOfRAResponses_p]);

                    raReqInPDU_p->raReqQNode_p = raReqQNode_p;

                    raReqInPDU_p->tcRNTI = ulUEContext_p->crnti;

                    raRNTI = raReqQNode_p->raRNTI;

                    if(PDCCH_ORDER == 
                            contFreePreamble_p->pdcchOrderHandOverFlag)
                    {
                        (*freeULRBForRAR_p) -= 
                            rachConfig_p->numRBReqdForConFreePDCCHOrder;

                        raReqInPDU_p->numRBAllocated = 
                            rachConfig_p->numRBReqdForConFreePDCCHOrder;

                        raReqInPDU_p->mcsIndex = 
                            rachConfig_p->mcsIndexForConFreePDCCHOrder;

                    }
                    else if(CONNECTION_HANDOVER ==
                            contFreePreamble_p->pdcchOrderHandOverFlag)
                    {
                        (*freeULRBForRAR_p) -= 
                            rachConfig_p->numRBReqdForConFreeHandOver;

                        raReqInPDU_p->numRBAllocated = 
                            rachConfig_p->numRBReqdForConFreeHandOver;

                        raReqInPDU_p->mcsIndex = 
                            rachConfig_p->mcsIndexForConFreeHandOver;

                        /* SPR 3608 changes start */
                        checkAndSetHighPriorityScheduling(ueIndex);    
                        /* SPR 3608 changes end */

                    }

                    (*maxRAR_p)--;               
                    (*noOfRAResponses_p)++;


                    LOG_MAC_MSG(RA_REQ_PROC_ID,
                            LOGINFO,
                            MAC_RACH,
                            getCurrentTick(),
                            raReqQNode_p->raRNTI,
                            raReqQNode_p->ttiRAReqReceived,
                            raReqQNode_p->raPreambleInfo.preamble,
                            (*maxRAR_p),
                            (*freeULRBForRAR_p),
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"CFR_RA_REQ_PROC_ID");
                }

            }
            else
            {
                break;
            }
        }

    /* Cyclomatic_complexity_changes_start */
    validateRarCntInContFreeQueueProcessing(
            numRARCnt,
            maxRAR_p,
            processRntiFlag,
            raRNTI,
            scheduledRARNTIBitMap2_p,
            scheduledRARNTIBitMap2_p,
            raResponsesInTTI_p,
            rarPduTobeScheduled_p);
    /* Cyclomatic_complexity_changes_end */
    /*EICIC*/
    /*Complexity Start*/
    eicicFreeRaReqQNode2(queueCount,
            contentionFreeRAReqQueue_p,
            internalCellIndex);
    /*Complexity End*/
    /*EICIC*/

}

/*Complexity Start*/
/*****************************************************************************
 * Function Name  : eicicFreeRaReqQNode2
 * Inputs         : queueCount - no of nodes in queue
 *                  contentionFreeRAReqQueue_p  - 
 *                  internalCellIndex -  Cell-Index at MAC 
 * Outputs        : None
 * Returns        : none
 * Description    : Pops node from contentionFreeRAReqQueue_p and pushes it in
 *                  contentionFreeRAReqQueue_gp
 *                                    
 *****************************************************************************/
void eicicFreeRaReqQNode2(
        UInt8 queueCount,
        LTE_SQUEUE  *contentionFreeRAReqQueue_p,
        InternalCellIndex internalCellIndex)
{
    RAReqQueueNode *raReqQNode2_p = PNULL;
    while(queueCount)
    {
        raReqQNode2_p = (RAReqQueueNode *) 
            popNode(contentionFreeRAReqQueue_p);
        if(PNULL!=raReqQNode2_p)
        {
            if(pushNode (contentionFreeRAReqQueue_gp[internalCellIndex], &(raReqQNode2_p->raNodeAnchor))) 
            {
                /* Failure condition : 1 - Failure, 0-Success */
                freeMemPool(raReqQNode2_p);
                raReqQNode2_p = PNULL; 
            }
            queueCount--;
        }
    }

}
/*Complexity End*/

/*****************************************************************************
 * Function Name  : handOverRelResources
 * Inputs         : ueIndex - Index of the Connection HO UE
 *                  tcrnti  - crnti of the Connection HO UE
 *                  internalCellIndex -  Cell-Index at MAC 
 *                  
 * Outputs        : release dedicated preamble 
 *                  
 * Returns        : MAC_SUCCESS OR MAC_FAILURE 
 * Description    : Release dedicated preamble allocated to  
 *                  Connection HO UE  
 *                  Stop timers assciated with the preamble 
 *****************************************************************************/
/*CA Changes start  */
void handOverRelResources(UInt16 ueIndex,UInt16 tcRNTI, InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{
    DLUEContext *ueDLContext_p  = PNULL;

    ueDLContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

    if(PNULL != ueDLContext_p)
    {
        /* SPR 16583 fix start */
        /* SPR 12338 changes start */
        if(INVALID_CRNTI == tcRNTI)
        {
            tcRNTI = ueDLContext_p->crnti;
        }
        /* SPR 12338 changes end */
        else if( tcRNTI != ueDLContext_p->crnti )
        {
            LOG_MAC_MSG( MAC_RNTI_NOT_SAME, LOGWARNING, MAC_RRC_INF,
                    getCurrentTick(), ueIndex, tcRNTI, 
                    ueDLContext_p->crnti, 0,0, 0,0, __func__, 
                    "HoRelRachInd" );
            tcRNTI = ueDLContext_p->crnti;
        }
        if( ueDLContext_p->tcrntiFlag )
        {
            if(MAC_SUCCESS ==  deleteMACDLUEConnHandOverEntity(ueIndex, internalCellIndex))
            {
                /* SPR 16583 fix end */
                /* SPR 16773 fix start */
                /* Search the create UE timer node in the timer list */
                LP_CreateUeContextTimerNode createUETimerNode_p =
                    searchCreateUeTimer( tcRNTI,internalCellIndex );
                /* Stop the create UE timer */
                stopCreateUeTimer( createUETimerNode_p ,internalCellIndex);
                /* SPR 16773 fix end */
                if (MAC_FAILURE == 
                        /*CA Changes start  */
                        freeRNTIBeforeUETempContextCreation(tcRNTI, internalCellIndex))
                    /*CA Changes end  */
                {

                    LOG_MAC_MSG(MAC_TCRNTI_FAIL_ID,
                            LOGWARNING,
                            MAC_RACH,
                            getCurrentTick(),
                            tcRNTI,
                            LINE_NUMBER,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,
                            "HOREL_FREE_TCRNTI_FAIL");
                }
            }
        }
        else
        {
            LOG_MAC_MSG(DLUECONTEXT_VALUE_MSIMATCH_ID,
                    LOGERROR,
                    MAC_RACH,
                    getCurrentTick(),
                    ueIndex,
                    tcRNTI,
                    ueDLContext_p->crnti,
                    ueDLContext_p->tcrntiFlag,
                    LINE_NUMBER,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"CONTEXT_VALUE_MSIMATCH");
        }

    }
    else
    {
        LOG_MAC_MSG(NO_HO_DL_UE_CONTEXT_ID,
                LOGERROR,
                MAC_RACH,
                getCurrentTick(),
                ueIndex,
                tcRNTI,
                LINE_NUMBER,
                DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"NO_HO_DL_UE_CONTEXT_ID");

    }
}
/* msg3 ulconfig fix start */
#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : processULConfigForRachQueue
 * Inputs         : containerIndex - index for ulSchUciRachContainer_g
 *                  currentSubFrame - ulSubframe Num for which UL Config
 *                                    needs to be prepared.
 *                                      
 *                  currentSFN - ulSysFrameNum for which UL Config 
 *                               needs to be prepared.
 *                   internalCellIndex -  Cell-Index at MAC 
 * Outputs        : None
 * Returns        : None
 * Description    : This will prepares the UL CONFIG Message 
 *****************************************************************************/
void processULConfigForRachQueue( UInt8 currentSubFrame,
        UInt32 currentSFN,
        InternalCellIndex internalCellIndex)
{
    UInt8 currentSfnIndex = MODULO_TWO(currentSFN);
    UlschUciInfoForULConfigForRach *node_p = PNULL;
    /* TDD Config 0 Changes Start */
    UInt8 containerIndex = currentSubFrame % MAX_ULSCH_UCI_CONTAINER;
    /* TDD Config 0 Changes End */
    ULSchUciRachPoolInfo* ulschUciRachInfo_p
        /*CA Changes start  */
        = ulSchUciRachContainer_gp[internalCellIndex] + containerIndex;
    /*CA Changes end  */
    /** CA-TDD Changes Start **/
    UInt8 count = sQueueCount(&ulschUciInfoForULConfigForRach_g[internalCellIndex]);
    UInt16 numberOfInfoPresent  = ulschUciRachInfo_p->numberOfInfoPresent;
    ULHarqInfo  *ulHarqInfo_p    = PNULL; 
    /*SPR 10031 FIX START */ 
    UInt8 loopCount = 0;
    UInt8 isPduUpdated = FALSE;
    /*SPR 10031 FIX END */

    /* if current sub frame is valid SF dor ul grant then get the nodes
     * from the ulschUciInfoForULConfigForRach_g queue and put into 
     * ulschUciRachInfo_p */
    while (count--)
    {
        node_p = (UlschUciInfoForULConfigForRach*)\
                 getFirstNode(&ulschUciInfoForULConfigForRach_g[internalCellIndex]);
        if( node_p)
        {
            /* TDD Config 0 Changes Start */
            if ((currentSubFrame == node_p->ulSubframe) &&
                    /* TDD Config 0 Changes End */
                    (currentSfnIndex == node_p->containerIndex))
            {
                node_p = (UlschUciInfoForULConfigForRach*)
                    popNode(&ulschUciInfoForULConfigForRach_g[internalCellIndex]);
                /** CA-TDD Changes End **/

                if (NULL == node_p)
                {
                    break;
                }

                ulHarqInfo_p = node_p->ulHarqInfo_p;
                if (PNULL != ulHarqInfo_p)
                {
                    /*SPR 10031 FIX START */
                    for(loopCount=0; loopCount < numberOfInfoPresent; loopCount++)
                    {
                        if(ulschUciRachInfo_p->ulschUciInfo[loopCount].rnti == node_p->tcRNTI)
                        {
                            isPduUpdated = TRUE;
                            /* Fill UL Harq Info*/
                            ulschUciRachInfo_p->
                                ulschUciInfo[loopCount].ulHarqInfo_p =
                                node_p->ulHarqInfo_p;

                            ulschUciRachInfo_p->
                                ulschUciInfo[loopCount].ulHarqProcessId = node_p->harqProcessId;
                            /* SS_S2 */
                            /*SPR 7086 Fix Start*/
                            ulschUciRachInfo_p->
                                ulschUciInfo[loopCount].nSRS = node_p->nSRS;
                            /*SPR 7086 Fix End*/
                            if(TRUE == node_p->isCqiWithCFRPresent)                       
                            {
                                DLUEContext *dlUEContext_p = PNULL;
                                ULUEContextInfo *ulUEContextInfo_p = PNULL;
                                ulUEContextInfo_p = &ulUECtxInfoArr_g[node_p->ueIndex];
                                dlUEContext_p = dlUECtxInfoArr_g[node_p->ueIndex].dlUEContext_p;
                                ulschUciRachInfo_p->ulschUciInfo[loopCount].pduType = MAC_SS_PDU_ULSCH_CQI_RI;
                                ulUEContextInfo_p->ulUEContext_p->aperiodicCQIToBeRequested = FALSE;
                                storeCqiPmiRiDataAperiodic(dlUEContext_p->dlMIMOInfo.transmissionMode,
                                        ulUEContextInfo_p, ulschUciRachInfo_p,
                                        /* TDD Config 0 Changes Start */
                                        currentSFN,
                                        currentSubFrame,
                                        /* TDD Config 0 Changes End */
                                        loopCount, internalCellIndex);
                            }
                            else
                            {
                                switch (ulschUciRachInfo_p->ulschUciInfo[loopCount].pduType)
                                {
                                    case MAC_SS_PDU_UCI_SR:
                                    {
                                        ulschUciRachInfo_p->ulschUciInfo[loopCount].pduType = MAC_SS_PDU_ULSCH;
                                        break;
                                    }
                                    case MAC_SS_PDU_UCI_CQI:
                                    case MAC_SS_PDU_UCI_CQI_SR:
                                    {
                                        ulschUciRachInfo_p->ulschUciInfo[loopCount].pduType = MAC_SS_PDU_ULSCH_CQI_RI;
                                        if (ulschUciRachInfo_p->ulschUciInfo[loopCount].cqiRiInfo.dlCqiPmiSizeRank_1)
                                        {
                                            ulschUciRachInfo_p->ulschUciInfo[loopCount].cqiRiInfo.dlCqiPmiSizeRankGT_1 = 
                                                ulschUciRachInfo_p->ulschUciInfo[loopCount].cqiRiInfo.dlCqiPmiSizeRank_1;
                                        }
                                        else
                                        {
                                            ulschUciRachInfo_p->ulschUciInfo[loopCount].cqiRiInfo.dlCqiPmiSizeRank_1 = 
                                                ulschUciRachInfo_p->ulschUciInfo[loopCount].cqiRiInfo.dlCqiPmiSizeRankGT_1;
                                        }
                                        break;
                                    }
                                    default:
                                    {
                                        lteWarning("Invalid PDU Type [%d] received in func [%s]\n",
                                                ulschUciRachInfo_p->ulschUciInfo[loopCount].pduType, __func__);
                                        break;
                                    }	
                                }
                            }
                        }/* End of if */
                    }/* End Of For Loop */
                    if(FALSE == isPduUpdated)
                    {
                        /*SPR 10031 FIX END*/
                        /*Fill rnti value */
                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                            rnti = node_p->tcRNTI;

                        /* Fill UL Harq Info*/
                        ulschUciRachInfo_p->
                            ulschUciInfo[numberOfInfoPresent].ulHarqInfo_p =
                            node_p->ulHarqInfo_p;

                        ulschUciRachInfo_p->
                            ulschUciInfo[numberOfInfoPresent].ulHarqProcessId = node_p->harqProcessId;
                        /* SS_S2 */
                        /*SPR 7086 Fix Start*/
                        ulschUciRachInfo_p->
                            ulschUciInfo[numberOfInfoPresent].nSRS = node_p->nSRS;
                        /*SPR 7086 Fix End*/

                        ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                            pduType = MAC_SS_PDU_ULSCH;
                        /*TDD SB_CQI*/
                        if(TRUE == node_p->isCqiWithCFRPresent)                       
                        {
                            DLUEContext *dlUEContext_p = PNULL;
                            ULUEContextInfo *ulUEContextInfo_p = PNULL;
                            ulUEContextInfo_p = &ulUECtxInfoArr_g[node_p->ueIndex];
                            dlUEContext_p = dlUECtxInfoArr_g[node_p->ueIndex].dlUEContext_p;
                            ulschUciRachInfo_p->ulschUciInfo[numberOfInfoPresent].
                                pduType = MAC_SS_PDU_ULSCH_CQI_RI;
                            ulUEContextInfo_p->ulUEContext_p->aperiodicCQIToBeRequested
                                = FALSE;
                            storeCqiPmiRiDataAperiodic(
                                    dlUEContext_p->dlMIMOInfo.transmissionMode,
                                    ulUEContextInfo_p, ulschUciRachInfo_p,
                                    /* TDD Config 0 Changes Start */
                                    currentSFN,
                                    currentSubFrame,
                                    /* TDD Config 0 Changes End */
                                    numberOfInfoPresent, internalCellIndex);
                            /** CA-TDD Changes End:05 **/
                        }
                        /*TDD SB_CQI*/

                        numberOfInfoPresent++;    
                    }
                    /*SPR 10031 FIX START*/
                }
                /*SPR 10031 FIX END*/
                else
                {
                }
                freeMemPool(node_p);
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
        node_p = PNULL;
    }

    /*Update Number of numberOfInfoPresent in the global context */ 
    ulschUciRachInfo_p->numberOfInfoPresent =  numberOfInfoPresent;
    return ;
}    

#endif

/* CA Changes start */
/****************************************************************************
 * Function Name  : deInitRACHMgr
 * Inputs         : internalCellIndex -  Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This API cleanups various queues initialized in 
 *                  initRachMgr. This is called at MAC_CLEANUP_LAYER_REQ
 ****************************************************************************/
void deInitRACHMgr(InternalCellIndex internalCellIndex)
{



    if (CELL_PHY_CONFIG <
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellState)
    {
        cleanupRNTIPoolTbl(internalCellIndex);
        freeMemPool(tempCrntiCtx_gp[internalCellIndex]);
        tempCrntiCtx_gp[internalCellIndex] = PNULL;
    }

    QDEINIT_DEMUX_RACH_MANAGER_INTERFACE_Q(internalCellIndex);
    QDEINIT_DL_RA_RSP_QUEUE_Q(internalCellIndex);
    QDEINIT_PDCCH_ORDER_TIMER_TRIGGER_Q(internalCellIndex);
    QDEINIT_PDCCH_RACH_INTERFACE_Q(internalCellIndex);
    QDEINIT_RACH_DL_STRATEGY_INTERFACE_Q(internalCellIndex);

    if(dlRARspQueue_gp[internalCellIndex])
    {
        freeMemPool(dlRARspQueue_gp[internalCellIndex]);
        dlRARspQueue_gp[internalCellIndex] = PNULL;
    }

    if(contentionResolutionMsgQ_gp[internalCellIndex])
    {
        freeMemPool(contentionResolutionMsgQ_gp[internalCellIndex]);
        contentionResolutionMsgQ_gp[internalCellIndex] = PNULL;
    }

    if(demuxRachManagerInterfaceQueue_gp[internalCellIndex])
    {
        freeMemPool(demuxRachManagerInterfaceQueue_gp[internalCellIndex]);
        demuxRachManagerInterfaceQueue_gp[internalCellIndex] = PNULL;
    }

    if(rachDLStrategyInterfaceQueue_gp[internalCellIndex])
    {
        freeMemPool(rachDLStrategyInterfaceQueue_gp[internalCellIndex]);
        rachDLStrategyInterfaceQueue_gp[internalCellIndex] = PNULL;
    }

    if(pdcchOrderTimerTriggerQueue_gp[internalCellIndex])
    {
        freeMemPool(pdcchOrderTimerTriggerQueue_gp[internalCellIndex]);
        pdcchOrderTimerTriggerQueue_gp[internalCellIndex] = PNULL;
    }

    if(pdcchRachInterfaceQueue_gp[internalCellIndex])
    {
        freeMemPool(pdcchRachInterfaceQueue_gp[internalCellIndex]);
        pdcchRachInterfaceQueue_gp[internalCellIndex] = PNULL;
    }

    if(rachGlobals_gp[internalCellIndex])
    {
        freeMemPool(rachGlobals_gp[internalCellIndex]);
        rachGlobals_gp[internalCellIndex] = PNULL;
    }
    /* +- SPR 22322 */

    if(conFreePreambleTimerList_gp[internalCellIndex])
    {
        freeMemPool(conFreePreambleTimerList_gp[internalCellIndex]);
        conFreePreambleTimerList_gp[internalCellIndex] = PNULL;
    }

    if(conBasedPDCCHOrderTimerList_gp[internalCellIndex])
    {
        freeMemPool(conBasedPDCCHOrderTimerList_gp[internalCellIndex]);
        conBasedPDCCHOrderTimerList_gp[internalCellIndex] = PNULL;
    }

    if(contentionTimerQueue_gp[internalCellIndex])
    {
        freeMemPool(contentionTimerQueue_gp[internalCellIndex]);
        contentionTimerQueue_gp[internalCellIndex] = PNULL;
    }

    if(raReqQueue_gp[internalCellIndex])
    {
        freeMemPool(raReqQueue_gp[internalCellIndex]);
        raReqQueue_gp[internalCellIndex] = PNULL;
    }

    if(contentionFreeRAReqQueue_gp[internalCellIndex])
    {
        freeMemPool(contentionFreeRAReqQueue_gp[internalCellIndex]);
        contentionFreeRAReqQueue_gp[internalCellIndex] = PNULL;
    }

    if(ulRARspNegAckQueue_gp[internalCellIndex])
    {
        freeMemPool(ulRARspNegAckQueue_gp[internalCellIndex]);
        ulRARspNegAckQueue_gp[internalCellIndex] = PNULL;
    }

    if(contentionResolutionMsgAckQ_gp[internalCellIndex])
    {
        freeMemPool(contentionResolutionMsgAckQ_gp[internalCellIndex]);
        contentionResolutionMsgAckQ_gp[internalCellIndex] = PNULL;
    }

    if(contentionFreeRACHPool_gp[internalCellIndex])
    {
        freeMemPool(contentionFreeRACHPool_gp[internalCellIndex]);
        contentionFreeRACHPool_gp[internalCellIndex] = PNULL;
    }


    return;
}

/*****************************************************************************
 * Function Name  : createMemPoolForCell
 * Inputs         : internalCellIndex 
 * Outputs        : None
 * Returns        : None
 * Description    :Create Memory pool for ue specific searc space in
 *                 case CA is enabled 
 *                  
 *****************************************************************************/
void createMemPoolForCell(InternalCellIndex internalCellIndex)
{
    InitParams *initParams_p = 
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p;
    InitParams *initParamsForOtherCell_p = PNULL;
    InternalCellIndex cellIndex=0;

    if(IS_CA_ENABLED())
    {
        if(internalCellIndex)
        {
            cellIndex=internalCellIndex -1;
        }
        else
        {
            cellIndex=internalCellIndex + 1;
        }
        initParamsForOtherCell_p=cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]->initParams_p;
        if(crntiToIdxTbl_g[cellIndex])
        {
            createMemPool(sizeof(UESpecificSearchSpaceInfo) * MAX_SUB_FRAME , initParams_p->crntiRange);
            createMemPool(sizeof(UESpecificSearchSpaceInfo) * MAX_SUB_FRAME , initParamsForOtherCell_p->crntiRange) ;
        }
    }
}

/*****************************************************************************
 * Function Name  : initUESecificSearchSpaceForCell
 * Inputs         : internalCellIndex 
 * Outputs        : None
 * Returns        : 
 * Description    :Calculates  Ue Specific search space for cell which
 *                  is already existing 
 *                  
 *****************************************************************************/
void initUESecificSearchSpaceForCell(InternalCellIndex internalCellIndex)
{
    if(IS_CA_ENABLED())
    {
        if((internalCellIndex))
        {
            /*In case second cell is configured calculated ue specific search space for other cell*/
            if(crntiToIdxTbl_g[internalCellIndex -1])
            {
                /*allocate memory for uespecific search space[0] in cell 1 rnti range */
                allocateUESpecificSearchSpace(internalCellIndex -1);
                /*ue specific search space for cell 0 considering rnti range of cell 1*/
                /* SPR 11044 Fix start */
                /* + SPR 14000 Changes */
#ifdef FDD_CONFIG
                initUESecificSearchSpaceOfAllUEs(maxCCEForCFI_g[internalCellIndex-1][valueCFI_g[internalCellIndex-1] -1 ], 
                        internalCellIndex-1,internalCellIndex);
#else
                initUESecificSearchSpaceOfAllUEs(
                        /* +- SPR 17777 */
                        internalCellIndex -1, internalCellIndex );
#endif 

                if(crntiToIdxTbl_g[internalCellIndex])
                {
                    allocateUESpecificSearchSpace(internalCellIndex);
                }

                /*ue specific search space for cell 1 considering rnti range of cell 0*/
#ifdef FDD_CONFIG
                initUESecificSearchSpaceOfAllUEs(maxCCEForCFI_g[internalCellIndex][valueCFI_g[internalCellIndex] -1 ], 
                        internalCellIndex,internalCellIndex-1);
#else
                initUESecificSearchSpaceOfAllUEs(
                        /* +- SPR 17777 */
                        internalCellIndex , internalCellIndex -1 );
#endif 
                /* SPR 11044 Fix end */
            }
        }
        else  
        {
            /* We would probably never be hitting this code, and thus it has to be done in the above if cond */
            if(crntiToIdxTbl_g[internalCellIndex  + 1])
            {
                /*allocate memory for uespecific search space[1] in cell 0 rnti range */
                allocateUESpecificSearchSpace(internalCellIndex + 1);
                /*ue specific search space for cell 1 considering rnti range of cell 0*/
                /* SPR 11044 Fix start */
#ifdef FDD_CONFIG
                initUESecificSearchSpaceOfAllUEs(maxCCEForCFI_g[internalCellIndex + 1][valueCFI_g[internalCellIndex + 1] -1 ], 
                        internalCellIndex + 1,internalCellIndex);
#else
                initUESecificSearchSpaceOfAllUEs(
                        /* +- SPR 17777 */
                        internalCellIndex +1, internalCellIndex  );

#endif

                /*ue specific search space for cell 0 considering rnti range of cell 1*/
                if(crntiToIdxTbl_g[internalCellIndex])
                {
                    allocateUESpecificSearchSpace(internalCellIndex);
                }
#ifdef FDD_CONFIG
                initUESecificSearchSpaceOfAllUEs(maxCCEForCFI_g[internalCellIndex][valueCFI_g[internalCellIndex] -1 ], 
                        internalCellIndex,internalCellIndex + 1);
#else
                initUESecificSearchSpaceOfAllUEs(
                        /* +- SPR 17777 */
                        internalCellIndex , internalCellIndex +1 );

#endif
                /* - SPR 14000 Changes */
                /* SPR 11044 Fix end */
            }
        }
    }
}

/*****************************************************************************
 * Function Name  : allocateUESpecificSearchSpace
 * Inputs         : internalCellIndex 
 * Outputs        : None
 * Returns        : 
 * Description    :Allocate Memory for UE Specific SearchSpace                  
 *****************************************************************************/
void allocateUESpecificSearchSpace(InternalCellIndex internalCellIndex)
{
    InitParams *initParams_p =PNULL;  
    RNTIInfo *rntiInfo_p=PNULL;
    UInt32 i = 0;
    UInt8 cellIndex=0;
    UInt32 rnticount= 0;
    if(internalCellIndex)
    {
        /*Allocate memory in ueSearchSpaceInfo_p[1] for all rnti 's on cell 0*/
        cellIndex=internalCellIndex - 1;
    }
    else
    {

        /*Allocate memory in ueSearchSpaceInfo_p[0] for all rnti 's on cell 1*/
        cellIndex=internalCellIndex + 1;
    }
    initParams_p=cellSpecificParams_g.\
                 cellConfigAndInitParams_p[cellIndex]->initParams_p;

    rntiInfo_p = (RNTIInfo *)crntiToIdxTbl_g[cellIndex];
    for (i=0,rnticount = initParams_p->lowerRNTIBound; 
            rnticount <= initParams_p->upperRNTIBound;
            rnticount++)
    {

        if(rntiInfo_p[i].ueSearchSpaceInfo_p[internalCellIndex]==PNULL)
        {
            GET_MEM_FROM_POOL(UESpecificSearchSpaceInfo , rntiInfo_p[i].ueSearchSpaceInfo_p[internalCellIndex],
                    MAX_SUB_FRAME * sizeof(UESpecificSearchSpaceInfo), PNULL);

            if(PNULL == rntiInfo_p[i].ueSearchSpaceInfo_p[internalCellIndex])
            {
                return ;
            }
            memSet(rntiInfo_p[i].ueSearchSpaceInfo_p[internalCellIndex], 0, MAX_SUB_FRAME * sizeof(UESpecificSearchSpaceInfo));

        }

        i++;
    }
}

/****************************************************************************
 * Function Name  : deInitRNTIPoolTbl
 * Inputs         : Internal cell Index
 * Outputs        : None
 * Returns        : None
 * Description    : This API cleanups memory allocated 
 *                  This is called at cell delete.
 ****************************************************************************/
void deInitRNTIPoolTbl(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    UInt32 queueCount = 0;
    FreeRNTIPoolNode *freeRNTIPoolNode_p = PNULL;

    /*Free RNTI queue */
    queueCount = sQueueCount (freeRNTIQueue_gp[internalCellIndex]);
    for (i = 0; i < queueCount; i++)
    {
        freeRNTIPoolNode_p = (FreeRNTIPoolNode *) popNode (freeRNTIQueue_gp[internalCellIndex]);

        if (freeRNTIPoolNode_p)
        {
            freeMemPool (freeRNTIPoolNode_p);
            freeRNTIPoolNode_p = PNULL;
        }
    }
    if (freeRNTIQueue_gp[internalCellIndex])
    {
        freeMemPool (freeRNTIQueue_gp[internalCellIndex]);
        freeRNTIQueue_gp[internalCellIndex] = PNULL;
    }
/* SPR 21468 Fix Start */ 
    queueCount = sQueueCount(freeRNTIQueueAfterUEDeletion_gp[internalCellIndex]);
    for (i = 0; i < queueCount; i++)
    {
        freeRNTIPoolNode_p =
            (FreeRNTIPoolNode *) popNode(freeRNTIQueueAfterUEDeletion_gp[internalCellIndex]);

        if (freeRNTIPoolNode_p)
        {
            freeMemPool(freeRNTIPoolNode_p);
            freeRNTIPoolNode_p = PNULL;
        }
    }
/* SPR 21468 Fix End */ 

    if (freeRNTIQueueAfterUEDeletion_gp[internalCellIndex])
    {
        freeMemPool (freeRNTIQueueAfterUEDeletion_gp[internalCellIndex]);
        freeRNTIQueueAfterUEDeletion_gp[internalCellIndex] = PNULL;
    }

    cleanupUeSpecificSearchSpace(internalCellIndex);

    if(crntiToIdxTbl_g[internalCellIndex])
    {
        freeMemPool(crntiToIdxTbl_g[internalCellIndex]);
        crntiToIdxTbl_g[internalCellIndex] = PNULL;
    }

    return;
}
/* CA Changes end */
/*****************************************************************************
 * Function Name  : cleanupUeSearchSpace
 * Inputs         : internalCellIndex -  Cell-Index at MAC 
 * Outputs        : None
 * Returns        : None
 * Description    : This will cleanup the uespecific search space on cell 
 *                 specified by Internal cell Index
 *****************************************************************************/
void cleanupUeSpecificSearchSpace( InternalCellIndex  internalCellIndex)
{


    RNTIInfo *rntiInfo_p = PNULL;
    UInt32 i = 0;
    UInt8 cellcount=0;
    InitParams *initParams_p = cellSpecificParams_g.\
                               cellConfigAndInitParams_p[internalCellIndex]->initParams_p;


    if(crntiToIdxTbl_g[internalCellIndex])
    {
        rntiInfo_p = (RNTIInfo *)crntiToIdxTbl_g[internalCellIndex];
        for (i = 0; i < initParams_p->crntiRange + initParams_p->spsCrntiRange ; i++)
        {
            /*pdcch changes start*/
            /*In case of cell deletion for each rnti ueSearchSpaceInfo_p  is set to NULL for all 
              the configured  cells*/
            for (cellcount = 0; cellcount < cellSpecificParams_g.numOfCells ; cellcount++)
            {
                if(rntiInfo_p[i].ueSearchSpaceInfo_p[cellcount])
                {
                    freeMemPool(rntiInfo_p[i].ueSearchSpaceInfo_p[cellcount]);
                    rntiInfo_p[i].ueSearchSpaceInfo_p[cellcount] = PNULL;
                }

            }
        }
        if(IS_CA_ENABLED())
        {
            if(internalCellIndex)
            {
                /*cleanup  UeSearchSpac[1] on cell 0 if delete req comes for cell 1*/
                cleanupUeSearchSpaceForCell(internalCellIndex -1 ,internalCellIndex );
            }
            else
            {

                /*cleanup  UeSearchSpac[0] on cell 1 if delete req comes for cell 0*/
                cleanupUeSearchSpaceForCell(internalCellIndex + 1 ,internalCellIndex);
            }
        }

    }
    /*pdcch changes end*/

}
/*****************************************************************************
 * Function Name  : cleanupUeSearchSpaceForCell
 * Inputs         : cellIndex-internal cell index for the cell where ue 
 *                  specific search space needs to be considered
 *                  ueSearchSpaceIndex-Ue specific search space index 
 *                  on cell specifed by cellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This will cleanup the uespecific search space on cell 
 *                 specified by cell index on index specified by ueSearchSpaceIndex
 *****************************************************************************/
void cleanupUeSearchSpaceForCell(UInt8 cellIndex,UInt8 ueSearchSpaceIndex )
{
    /* + SPR_11556 */ 
    UInt16 i=0;
    /* - SPR_11556 */ 
    RNTIInfo *rntiInfo_p = PNULL;
    UInt16   crntiRange = cellSpecificParams_g.\
                          cellConfigAndInitParams_p[cellIndex]->\
                          initParams_p->crntiRange;

    if(crntiToIdxTbl_g[cellIndex])
    {
        rntiInfo_p = (RNTIInfo *)crntiToIdxTbl_g[cellIndex];
        for (i = 0; i < crntiRange; i++)
        {
            if(  rntiInfo_p[i].ueSearchSpaceInfo_p[ueSearchSpaceIndex])
            {
                freeMemPool(rntiInfo_p[i].ueSearchSpaceInfo_p[ueSearchSpaceIndex]);
                rntiInfo_p[i].ueSearchSpaceInfo_p[ueSearchSpaceIndex] = PNULL;
            }
        }


    }

}

/* +- SPR 22322 */
/* SPR 16773 fix start */
/*****************************************************************************
 * Function Name  : cleanupTempCRNTITimerList
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This will cleanup tempCRNTITimerList_gp
 ******************************************************************************/

void cleanupUeTimerList(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    UInt32 count = 0;
    LP_CreateUeContextTimerNode createUeTimerNode_p = PNULL;

    /* Clean the Create UE timer List */
    count = listCount(createUeContextTimerList_gp[internalCellIndex]);
    for (i = 0; i < count; i++)
    {
        createUeTimerNode_p = 
            (LP_CreateUeContextTimerNode)getFirstListNode(
                    createUeContextTimerList_gp[internalCellIndex]);

        listDeleteNode(createUeContextTimerList_gp[internalCellIndex],
                &createUeTimerNode_p->nodeAnchor);
        if (createUeTimerNode_p)
        {
            freeMemPool(createUeTimerNode_p);
            createUeTimerNode_p = PNULL;
        }
    }
    return;
}
/* SPR 16773 fix end */
/*****************************************************************************
 * Function Name  : cleanupRAReqQueue
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This will cleanup RAReqQueue
 * *******************************************************************************/

void cleanupRAReqQueue(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    UInt32 count = 0;
    RAReqQueueNode *raReqQueueNode_p = PNULL;
    /* SPR 3854 Start*/
    /*RA Request queue*/
    /*CA Changes start  */
    count = sQueueCount(raReqQueue_gp[internalCellIndex]);
    /*CA Changes end  */
    for (i = 0; i < count; i++)
    {
        /*CA Changes start  */
        raReqQueueNode_p = (RAReqQueueNode *)popNode(raReqQueue_gp[internalCellIndex]);
        /*CA Changes end  */
        /* SPR 1868 changes end */

        if (raReqQueueNode_p)
        {
            freeMemPool(raReqQueueNode_p);
            raReqQueueNode_p = PNULL;
        }
        /* SPR 1868 changes start */
    } 
    /* SPR 1868 changes end */

    /* SPR 3854 End*/
    return;
}
/*****************************************************************************
 * Function Name  : cleanupContentionFreeRAReqQueue
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This will cleanup ContentionFreeRAReqQueueGp
 ********************************************************************************/

void cleanupContentionFreeRAReqQueue(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    UInt32 count = 0;
    RAReqQueueNode *raReqQueueNode_p = PNULL;
    /* SPR 3854 Start*/
    count = sQueueCount(contentionFreeRAReqQueue_gp[internalCellIndex]);

    for (i = 0; i < count; i++)
    {
        raReqQueueNode_p = 
            (RAReqQueueNode *)popNode(contentionFreeRAReqQueue_gp[internalCellIndex]);
        if (raReqQueueNode_p)
        {
            freeMemPool(raReqQueueNode_p);
            raReqQueueNode_p = PNULL;
        }
    } 

    /* SPR 3854 End*/
    return;
}
/*****************************************************************************
 * Function Name  : cleanupulRARspNegAckQueue
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This will cleanup ulRARspNegAckQueue
 ********************************************************************************/

void cleanupulRARspNegAckQueue(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    UInt32 count = 0;
    ULRARspNegAckQueueNode *ulRARspNegAckQueueNode_p = PNULL;
    /*RA Rsp negetive ack queue*/
    /*CA Changes start  */
    count = sQueueCount(ulRARspNegAckQueue_gp[internalCellIndex]);
    /*CA Changes end  */
    for (i = 0; i < count; i++)
    {
        ulRARspNegAckQueueNode_p = 
            /*CA Changes start  */
            (ULRARspNegAckQueueNode *)popNode(ulRARspNegAckQueue_gp[internalCellIndex]);
        /*CA Changes end  */

        if (ulRARspNegAckQueueNode_p)
        {
            freeMemPool(ulRARspNegAckQueueNode_p);
            ulRARspNegAckQueueNode_p = PNULL;
        }
    }
    return;
}
/*****************************************************************************
 * Function Name  : cleanupDLRARspQueue
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This will cleanup DLRARspQueue
 *********************************************************************************/

void cleanupDLRARspQueue(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    UInt32 count = 0;
    DLRARspQueueNode *dlRARspQueueNode_p = PNULL;
    /*DL RA Responce queue*/
    /*CA Changes start  */
    count = COUNT_DL_RA_RSP_QUEUE_Q(internalCellIndex);
    /*CA Changes end  */
    for (i = 0; i < count; i++)
    { 
        /*CA Changes start  */
        DEQUEUE_DL_RA_RSP_QUEUE_Q(DLRARspQueueNode, (void**)&dlRARspQueueNode_p, internalCellIndex);
        /*CA Changes end  */
        if (dlRARspQueueNode_p)
        {
            /* SPR 1756 changes start */
            msgFree( dlRARspQueueNode_p->rarToDl.macPduData_p);
            /* SPR 1786 changes start */
            dlRARspQueueNode_p->rarToDl.macPduData_p = PNULL;
            /* SPR 1786 changes end */
            /* SPR 1756 changes end */
            FREE_MEM_NODE_DL_RA_RSP_QUEUE_Q(dlRARspQueueNode_p);
        }
    }
    return;
}
/*****************************************************************************
 * Function Name  : cleanupRachDemuxQueue
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This will cleanup Rach Demux Queue.
 **********************************************************************************/

void cleanupRachDemuxQueue(InternalCellIndex internalCellIndex)
{  
    UInt32 i = 0;
    UInt32 count = 0;
    RachDemuxQueueNode *rachDemuxQueueNode_p = PNULL;
    /*CA Changes start  */
    count = COUNT_DEMUX_RACH_MANAGER_INTERFACE_Q(internalCellIndex);
    /*CA Changes end  */
    for (i = 0; i < count; i++)
    {
        /*CA Changes start  */
        DEQUEUE_DEMUX_RACH_MANAGER_INTERFACE_Q(RachDemuxQueueNode, (void *)&rachDemuxQueueNode_p, internalCellIndex);
        /*CA Changes end  */

        if (rachDemuxQueueNode_p)
        {
            FREE_MEM_NODE_DEMUX_RACH_MANAGER_INTERFACE_Q(rachDemuxQueueNode_p);
        }
    }
    return;
}
/*****************************************************************************
 * Function Name  : cleanupRachDLStrategyQueue
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This will cleanup RachDLStrategyQueue
 ***********************************************************************************/

void cleanupRachDLStrategyQueue(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    UInt32 count = 0;
    RachDLStrategyQueueNode *rachDLStrategyQueueNode_p = PNULL;
    /*CA Changes start  */
    count = COUNT_RACH_DL_STRATEGY_INTERFACE_Q(internalCellIndex);
    /*CA Changes end  */
    for (i = 0; i < count; i++)
    {
        /*CA Changes start  */
        DEQUEUE_RACH_DL_STRATEGY_INTERFACE_Q(RachDLStrategyQueueNode, (void *)&rachDLStrategyQueueNode_p, internalCellIndex);
        /*CA Changes end  */

        if (rachDLStrategyQueueNode_p)
        {
            FREE_MEM_NODE_RACH_DL_STRATEGY_INTERFACE_Q(rachDLStrategyQueueNode_p);
        }
    }
    return;
}

/*****************************************************************************
 * Function Name  : cleanupPdcchOrderTimerTriggerQueue
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This will cleanup PdcchOrderTimerTriggerQueue
 ************************************************************************************/

void cleanupPdcchOrderTimerTriggerQueue(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    UInt32 count = 0;
    PdcchOrderTimerTriggerQueueNode *pdcchOrderTimerTriggerQueueNode_p = PNULL;
    /*CA Changes start  */
    count = COUNT_PDCCH_ORDER_TIMER_TRIGGER_Q(internalCellIndex);
    /*CA Changes end  */
    for (i = 0; i < count; i++)
    {
        /*CA Changes start  */
        DEQUEUE_PDCCH_ORDER_TIMER_TRIGGER_Q(PdcchOrderTimerTriggerQueueNode, (void *)&pdcchOrderTimerTriggerQueueNode_p, internalCellIndex);
        /*CA Changes end  */

        if (pdcchOrderTimerTriggerQueueNode_p)
        {
            FREE_MEM_NODE_PDCCH_ORDER_TIMER_TRIGGER_Q(pdcchOrderTimerTriggerQueueNode_p);
        }
    }
    return;
}

/*****************************************************************************
 * Function Name  : cleanupRachInterfaceQueue
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This will cleanup RachInterfaceQueue
 *************************************************************************************/

void cleanupRachInterfaceQueue(InternalCellIndex internalCellIndex)
{
    UInt32 i = 0;
    UInt32 count = 0;
    RachInterfaceQueueNode *rachInterfaceQueueNode_p = PNULL;
    /*CA Changes start  */
    count = COUNT_PDCCH_RACH_INTERFACE_Q(internalCellIndex);
    /*CA Changes end  */
    for (i = 0; i < count; i++)
    {
        /*CA Changes start  */
        DEQUEUE_PDCCH_RACH_INTERFACE_Q(RachInterfaceQueueNode, (void *)&rachInterfaceQueueNode_p, internalCellIndex);
        /*CA Changes end  */
        if(rachInterfaceQueueNode_p)
        {
            /* SPR 5235 : Start*/
            if(rachInterfaceQueueNode_p->rachInterface_p)
            {
                FREE_MEM_NODE_PDCCH_RACH_INTERFACE_Q(rachInterfaceQueueNode_p->rachInterface_p);
            }
            /* SPR 5235 : End*/
            FREE_MEM_NODE_PDCCH_RACH_INTERFACE_Q(rachInterfaceQueueNode_p);
        }
    }
    return;
}

/*****************************************************************************
 * Function Name  : processQueuesBeforeRARspQueue
 * Inputs         : currentGlobalTTI
 *                  internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function are used to process rachDLStrategyQueue,
 *                  pdcchOrderTimerTriggerQueue, pdcchRachInterfaceQueue,
 *                  rachDemuxQueue, TempCRNTITimerList, contentionResolutionMsgAck,
 *                  conFreePreambleTimerList, conBasedPDCCHOrderTimerList and
 *                  contentionTimerQueue.
 *************************************************************************************/
/*SPR 15909 fix start*/
void processQueuesBeforeRARspQueue(tickType_t currentGlobalTTI,
        /*SPR 15909 fix end*/        
        InternalCellIndex internalCellIndex, ExecutionLegFunctionArg* execArgs)
{
    UInt32 rachDLStrategyQueueCount = 0;
    UInt32 pdcchOrderTimerTriggerQueueCount = 0;
    UInt32 pdcchRachInterfaceQueueCount = 0;
    UInt32 rachDemuxQueueCount = 0;
    UInt32 listNoneCount   = 0;
    UInt32 count  = 0;

    rachDLStrategyQueueCount = COUNT_RACH_DL_STRATEGY_INTERFACE_Q(internalCellIndex);
    /*Process the queue between rach manager and DL Strategy*/
    if(rachDLStrategyQueueCount)
    {
        processRachDLStrategyQueue(currentGlobalTTI, internalCellIndex);
    }

    /*Process the PdcchOrderTimerTriggerQueue */
    pdcchOrderTimerTriggerQueueCount = COUNT_PDCCH_ORDER_TIMER_TRIGGER_Q(internalCellIndex);
    if(pdcchOrderTimerTriggerQueueCount)
    {
        /* +- SPR 17777 */
        processPdcchOrderTimerTriggerQueue(internalCellIndex);
        /* +- SPR 17777 */
    }

    /*Process the queue between rach manager and PDCCH*/
    pdcchRachInterfaceQueueCount = COUNT_PDCCH_RACH_INTERFACE_Q(internalCellIndex);
    if(pdcchRachInterfaceQueueCount)
    {
        /*CA Changes Start */
        /* +- SPR 17777 */
        processPdcchRachInterfaceQueue(internalCellIndex);
        /* +- SPR 17777 */
        /*CA Changes End */
    }
    /*Process the queue between demux and rach manager*/
    /*CA Changes start  */
    rachDemuxQueueCount = COUNT_DEMUX_RACH_MANAGER_INTERFACE_Q(internalCellIndex);
    /*CA Changes end  */
    if(rachDemuxQueueCount)
    {
        processdemuxRachManagerInterfaceQueue(rachDemuxQueueCount, internalCellIndex, execArgs);
    }
    /* +- SPR 22322 */
    count = sQueueCount(contentionResolutionMsgAckQ_gp[internalCellIndex]);

    if (count)
    {
        processContentionResAckQueue(count, internalCellIndex);
    }

    /*Process the conFreePreambleTimerList_g */
    if(conFreePreambleTimerList_gp[internalCellIndex])  
    {
    listNoneCount = listCount(conFreePreambleTimerList_gp[internalCellIndex]);

    if (listNoneCount)
    {
        /* + SPR 17317 */
        checkContentionFreePreambleExpiry(currentGlobalTTI, listNoneCount, internalCellIndex);
        /* - SPR 17317 */
    }
    }

    if(conBasedPDCCHOrderTimerList_gp[internalCellIndex]) 
    {

    /*Process the conBasedPDCCHOrderTimerList_g */
    listNoneCount = listCount(conBasedPDCCHOrderTimerList_gp[internalCellIndex]);

    if (listNoneCount)
    {
        /* + SPR 17317 */
        checkConBasedPdcchOrderTimerExpiry(currentGlobalTTI, listNoneCount, internalCellIndex);
        /* - SPR 17317 */
    }
    }

    count = sQueueCount(contentionTimerQueue_gp[internalCellIndex]);

    if (count)
    {
        processContentionMsgTimerQueue(currentGlobalTTI, count, internalCellIndex);
    }
    /*CA Changes end  */
    return;
}

/*****************************************************************************
 * Function Name  : getRarPduToSchedule
 * Inputs         : raReqQNode_p
 *                  scheduledRARNTIBitMap1_p
 *                  scheduledRARNTIBitMap2_p
 *                  raResponsesInTTI_p
 *                  processRntiFlag_p
 * Outputs        : None
 * Returns        : RAReqTobeScheduledInPdu
 * Description    : This function checks RNTI and sends RAR pdu for processing.
 *************************************************************************************/
RAReqTobeScheduledInPdu *getRarPduToSchedule(RAReqQueueNode *raReqQNode_p,
        UInt32 *scheduledRARNTIBitMap1_p,
        UInt32 *scheduledRARNTIBitMap2_p,
        RarPduTobeScheduledInTTI *raResponsesInTTI_p,
        UInt8  *processRntiFlag_p)
{
    RAReqTobeScheduledInPdu *rarPduTobeScheduled_p = PNULL;

    if (raReqQNode_p->raRNTI > 31)
    {
        if ((*scheduledRARNTIBitMap2_p) & 
                (1 << (raReqQNode_p->raRNTI - 31)))
        {
            *processRntiFlag_p = 1;
        }
    }
    else if ((*scheduledRARNTIBitMap1_p) & 
            (1 << raReqQNode_p->raRNTI))
    {
        *processRntiFlag_p = 1;
    }
    if(1 == *processRntiFlag_p)
    {

        rarPduTobeScheduled_p = 
            getRARPduInfoforRARNTI(raReqQNode_p->raRNTI,
                    raResponsesInTTI_p);

        if(PNULL == rarPduTobeScheduled_p)
        {
            LOG_MAC_MSG(RA_RNTI_PROC_RAR_NOT_FOUND,
                    LOGWARNING,
                    MAC_RACH,
                    getCurrentTick(),
                    raResponsesInTTI_p->numRarPdu,
                    raReqQNode_p->raRNTI,
                    LINE_NUMBER,
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"RA_RNTI_PROC_RAR_NOT_FOUND");

            rarPduTobeScheduled_p = 
                getNextRARPduTobeScheduled(raResponsesInTTI_p);

        }

    }
    else
    {
        rarPduTobeScheduled_p = 
            getNextRARPduTobeScheduled(raResponsesInTTI_p);
    }

    return rarPduTobeScheduled_p;
}

/*****************************************************************************
 * Function Name  : validateRarCntInContFreeQueueProcessing
 * Inputs         : numRARCnt
 *                  maxRAR_p
 *                  processRntiFlag
 *                  raRNTI
 *                  scheduledRARNTIBitMap1_p
 *                  scheduledRARNTIBitMap2_p
 *                  raResponsesInTTI_p
 *                  rarPduTobeScheduled_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function validates the numRARCnt with maxRarCnt after processing
 *                  contention free rach request queue.
 *************************************************************************************/
void validateRarCntInContFreeQueueProcessing(
        UInt8  numRARCnt,
        UInt32 *maxRAR_p,
        UInt8 processRntiFlag,
        UInt16 raRNTI,
        UInt32 *scheduledRARNTIBitMap1_p,
        UInt32 *scheduledRARNTIBitMap2_p,
        RarPduTobeScheduledInTTI *raResponsesInTTI_p,
        RAReqTobeScheduledInPdu *rarPduTobeScheduled_p)
{

    if(numRARCnt != *maxRAR_p) 
    {
        if(!(processRntiFlag))
        {    
            if (raRNTI > 31)
            {
                *scheduledRARNTIBitMap2_p |= 1 << (raRNTI - 31);
            }
            else
            {
                *scheduledRARNTIBitMap1_p |= 1 << raRNTI;
            }

            raResponsesInTTI_p->numRarPdu++;

            LOG_MAC_MSG(RAR_PDU_PROC_ID,
                    LOGDEBUG,
                    MAC_RACH,
                    getCurrentTick(),
                    raResponsesInTTI_p->numRarPdu,
                    rarPduTobeScheduled_p->noOfRAResponses,
                    rarPduTobeScheduled_p->raRnti,
                    rarPduTobeScheduled_p->BIFlag,
                    LINE_NUMBER,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"CFR_RAR_PDU_PROC_ID");

            LOG_MAC_MSG(RAR_RARNTI_MAP_ID,
                    LOGDEBUG,
                    MAC_RACH,
                    getCurrentTick(),
                    rarPduTobeScheduled_p->raRnti,
                    (*scheduledRARNTIBitMap1_p),
                    (*scheduledRARNTIBitMap2_p),
                    DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"CFR_RAR_RARNTI_MAP_ID");

        }
    }

    return;
}

/* + SPR 17733 */
#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : allocateCCEInUESpecificSpaceForContentionResMsgReTx
 * Inputs         : ueSearchSpaceInfo_p - Pointer to the UE specific search space 
 *                  info
 *                  contentionQueueNode_p - Pointer to ContentionQueueNode
 *                  aggregationLevel - aggregation level
 *                  pdcchDLOutputInfo_p - Pointer to the pdcch container
 *                  dciCCEInfo_p - Pointer to the DCICCE container
 *                  availableCCE_p - Available CCE
 * Outputs        :                   
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : This will allocate the CCE in UE specific space for 
 *                  contention resolutin messages
 *****************************************************************************/
MacRetType allocateCCEInUESpecificSpaceForContentionResMsgReTx(
                            UESpecificSearchSpaceInfo  *ueSearchSpaceInfo_p,
                            PdcchDLOutputInfo   *pdcchDLOutputInfo_p,
                            DciCCEInfo          *dciCCEInfo_p,
                            DLHARQProcess       *dlHarqProcess_p ,
                            UInt32 *availableCCE_p,
                            UInt32 aggregationLevel,
                            DciCCEContainerInfo *dciCCEContainerInfo_p,
                            PdcchULOutputInfo *pdcchULOutputInfo_p,
                            TempCRNTICtx *tempUEContext_p,
                            InternalCellIndex internalCellIndex)
{
    UInt32     count = 0;
    MacRetType macRetType       = MAC_FAILURE;
    UInt32     cceAllocated     = INVALID_CCE_INDEX;
    DLContentionReslMsgInfo *contReslMsgInfo_p = PNULL;
    TcrntiCCEIndexInfo *tcrntiCCEIndexInfo_p = PNULL;
    UInt8 contentionUECount = 0;
    LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
                   "Entered in function = %s \n",__func__);

    switch(aggregationLevel)
    {
        case AGGREGATION_LEVEL_ONE:
        {
            /*Comes here when the Aggregagation Level of dciFormat is One*/
            LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
                          "Trying to Allocate CCE at Aggr Level = %d",
                          aggregationLevel);   
            
            /*Calling funtion isCCEAvailableAggrLvlOneUESpace which will
             *check if the CCEs are free at aggregation level one.If free
             *it will return Success else Failure
             */
            macRetType =  isCCEAvailableAggrLvlOneUESpace(
                            ueSearchSpaceInfo_p->cceAggrLvlOneInfo[currentCFIValue_g[internalCellIndex]-1],//DYN_PDCCH
                            &cceAllocated,availableCCE_p,dciCCEInfo_p);
        }
        break;
        case AGGREGATION_LEVEL_TWO:
        {
            /*Comes here when the Aggregagation Level of dciFormat is Two*/
            LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
            "Trying to Allocate CCE at Aggr Level = %d",
            aggregationLevel);
            
             /*Calling funtion isCCEAvailableAggrLvlTwoUESpace which will
              *check if the CCEs are free at aggregation level Two.If free
              *it will return Success else Failure
              */
            macRetType = isCCEAvailableAggrLvlTwoUESpace(
                            ueSearchSpaceInfo_p->cceAggrLvlTwoInfo[currentCFIValue_g[internalCellIndex]-1],//DYN_PDCCH
                            &cceAllocated,availableCCE_p,dciCCEInfo_p);
        }
        break;
        case AGGREGATION_LEVEL_FOUR:
        {
            /*Comes here when the Aggregagation Level of dciFormat is Four*/
            LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
             "Trying to Allocate CCE at Aggr Level = %d",
             aggregationLevel);
            
            /*Calling funtion isCCEAvailableAggrLvlFourUESpace which will
             *check if the CCEs are free at aggregation level Four.If free
             *it will return Success else Failure
             */
            macRetType = isCCEAvailableAggrLvlFourUESpace(
                            ueSearchSpaceInfo_p->cceAggrLvlFourInfo[currentCFIValue_g[internalCellIndex]-1],//DYN_PDCCH
                            &cceAllocated,availableCCE_p,dciCCEInfo_p);
        }
        break;
        case AGGREGATION_LEVEL_EIGHT:
        {   
            /*Comes here when the Aggregagation Level of dciFormat is Eight*/
            LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
               "Trying to Allocate CCE at Aggr Level = %d",
               aggregationLevel);

            /*Calling funtion isCCEAvailableAggrLvlEightUESpace which will
             *check if the CCEs are free at aggregation level Eight.If free
             *it will return Success else Failure
             */
            macRetType = isCCEAvailableAggrLvlEightUESpace(
                            ueSearchSpaceInfo_p->cceAggrLvlEightInfo[currentCFIValue_g[internalCellIndex]-1],//DYN_PDCCH
                            &cceAllocated,availableCCE_p,dciCCEInfo_p);
        }
        break;
        default:
        {
            LTE_MAC_UT_LOG(LOG_WARNING,RACH_MGR,
                "InValid Aggrgation Level\n");
        }
    }/*End of Switch Statement */

    if (MAC_SUCCESS == macRetType)
    {
        LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
            "[%s]: CCE allocated successfully cceAllocated [%d]\n",
            __func__,cceAllocated);
        changeCFIInThisTTI_g[internalCellIndex] = TRUE;
        count = pdcchDLOutputInfo_p->countCommonChannelMsg;

        contReslMsgInfo_p = 
            &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].dlMsgInfo.\
              dlContentionReslMsgInfo;

        contReslMsgInfo_p->resourceAllocInput.preferredRATPolicy = 
                                                          RA_POLICY_2_LOCALIZED;
        contReslMsgInfo_p->cceIndex   = cceAllocated;
        contReslMsgInfo_p->resourceAllocInput.requiredRB = 
                                             dlHarqProcess_p->assignedResourcesNB;
        contReslMsgInfo_p->resourceAllocInput.mcsIndexForCommonChannel = 
                                             dlHarqProcess_p->dlHARQTBOneInfo.mcsIndex;
        dciCCEInfo_p[cceAllocated].dciPduInfo.aggregationLvl = aggregationLevel;
        dciCCEInfo_p[cceAllocated].dciPduInfo.cceIndex       = cceAllocated;
        dciCCEInfo_p[cceAllocated].dciPduInfo.rnti           = 
                                             tempUEContext_p->tcRNTI;

        dciCCEInfo_p[cceAllocated].dciPduInfo.harqProcessId = dlHarqProcess_p->harqProcessId;
        dciCCEInfo_p[cceAllocated].dciPduInfo.tb1Info.txIndicator = NEW_TX;
        dciCCEInfo_p[cceAllocated].dciPduInfo.txPower =  
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                       cellParams_p->commonDLPowerControlInfo.pdcchPowerOffset ;
        dciCCEInfo_p[cceAllocated].dciPduInfo.pA = DEFAULT_PA_VALUE;

        contReslMsgInfo_p->dciFormat = dlHarqProcess_p->dciFormat;
        contReslMsgInfo_p->resourceAllocInput.requiredRB = 
                                       dlHarqProcess_p->assignedResourcesNB; 
        contReslMsgInfo_p->data_p = dlHarqProcess_p->transmissionBuffer_p;
        contReslMsgInfo_p->dataLen = dlHarqProcess_p->dlHARQTBOneInfo.tbSize;
        pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].msgType = 
                                                       CONTENTION_RESL_MSG;

        pdcchDLOutputInfo_p->countCommonChannelMsg++;

        dciCCEContainerInfo_p->arrayCCEAllocated[dciCCEContainerInfo_p->\
            countOfCCEAllocated] = cceAllocated;
        dciCCEContainerInfo_p->countOfCCEAllocated++;
            
        tcrntiCCEIndexInfo_p = 
            pdcchULOutputInfo_p->pdcchTcrntiUEInfo.pdcchTempUEArray; 

        contentionUECount = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count;
        tcrntiCCEIndexInfo_p[contentionUECount].cceIndex = cceAllocated;
        tcrntiCCEIndexInfo_p[contentionUECount].tcrnti = 
            tempUEContext_p->tcRNTI;
        pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count++;

    }

    LTE_MAC_UT_LOG(LOG_DETAIL ,RACH_MGR,
           "[Exit] %s\n",__func__);

    return macRetType;
}

/*****************************************************************************
 * Function Name  : allocateCCEInCommonSpaceForContentionResMsgReTx
 * Inputs         : contentionQueueNode_p - Pointer to ContentionQueueNode
 *                  aggregationLevel - aggregation level
 *                : pdcchDLOutputInfo_p - Pointer to the pdcch container
 *                  dciCCEInfo_p - Pointer to the DCICCE container
 *                  availableCCE_p - Available CCE
 * Outputs        :                   
 * Returns        : MAC_SUCESS/MAC_FAILURE
 * Description    : This will allocate the CCE from common search space
 *****************************************************************************/
MacRetType allocateCCEInCommonSpaceForContentionResMsgReTx(
                                     PdcchDLOutputInfo   *pdcchDLOutputInfo_p,
                                     DciCCEInfo          *dciCCEInfo_p,
                                     DLHARQProcess       *dlHarqProcess_p ,
                                     UInt32 *availableCCE_p,
                                     DciCCEContainerInfo *dciCCEContainerInfo_p,
                                     PdcchULOutputInfo *pdcchULOutputInfo_p,
                                     TempCRNTICtx *tempUEContext_p,
                                     InternalCellIndex internalCellIndex)
{
    UInt32 cceAllocated = INVALID_CCE_INDEX;
    MacRetType macRetType   = MAC_FAILURE;
    UInt32 count        = 0;
    DLContentionReslMsgInfo *contReslMsgInfo_p = PNULL;
    TcrntiCCEIndexInfo *tcrntiCCEIndexInfo_p = PNULL;
    UInt8 contentionUECount = 0;

    LTE_MAC_UT_LOG(LOG_INFO ,RACH_MGR,
            "[%s]: Allocating CCE at Aggr Level = %d ",
            __func__, AGGREGATION_LEVEL_FOR_COMMON_SPACE);
    
    macRetType = isCCEAvailableAggrLvlFourCommonSpace(
                   &cceAllocated,availableCCE_p,dciCCEInfo_p,internalCellIndex);
     
    if (MAC_SUCCESS == macRetType)
    {
        LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
            "[%s]: CCE allocated successfully cceAllocated [%d]\n",
            __func__,cceAllocated);
        
        count = pdcchDLOutputInfo_p->countCommonChannelMsg;
        contReslMsgInfo_p = &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].\
                             dlMsgInfo.dlContentionReslMsgInfo;
        contReslMsgInfo_p->resourceAllocInput.preferredRATPolicy = 
                                                         RA_POLICY_2_LOCALIZED;
        
        dciCCEInfo_p[cceAllocated].dciPduInfo.aggregationLvl = \
                                            AGGREGATION_LEVEL_FOR_COMMON_SPACE;
        dciCCEInfo_p[cceAllocated].dciPduInfo.cceIndex       = cceAllocated;
        dciCCEInfo_p[cceAllocated].dciPduInfo.rnti           = 
                                          tempUEContext_p->tcRNTI;
        dciCCEInfo_p[cceAllocated].dciPduInfo.harqProcessId = dlHarqProcess_p->harqProcessId;
        dciCCEInfo_p[cceAllocated].dciPduInfo.tb1Info.txIndicator = RE_TX;
        dciCCEInfo_p[cceAllocated].dciPduInfo.txPower = 
                       cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                                   cellParams_p->commonDLPowerControlInfo.pdcchPowerOffset; 
        pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].msgType = CONTENTION_RESL_MSG;

        contReslMsgInfo_p->cceIndex  = cceAllocated;
        contReslMsgInfo_p->dciFormat = dlHarqProcess_p->dciFormat;
        contReslMsgInfo_p->resourceAllocInput.requiredRB = 
                                                dlHarqProcess_p->assignedResourcesNB; 
        contReslMsgInfo_p->resourceAllocInput.mcsIndexForCommonChannel= 
                                                dlHarqProcess_p->dlHARQTBOneInfo.mcsIndex;
        contReslMsgInfo_p->data_p = dlHarqProcess_p->transmissionBuffer_p;
        contReslMsgInfo_p->dataLen = dlHarqProcess_p->dlHARQTBOneInfo.tbSize;
        dciCCEContainerInfo_p->arrayCCEAllocated[dciCCEContainerInfo_p->countOfCCEAllocated] =
            cceAllocated;

        dciCCEContainerInfo_p->countOfCCEAllocated++;    
        pdcchDLOutputInfo_p->countCommonChannelMsg++;

        tcrntiCCEIndexInfo_p = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.pdcchTempUEArray; 

        contentionUECount = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count;
        tcrntiCCEIndexInfo_p[contentionUECount].cceIndex = cceAllocated;
        tcrntiCCEIndexInfo_p[contentionUECount].tcrnti = tempUEContext_p->tcRNTI;
        pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count++;
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,RACH_MGR,
                "[%s]: ERROR CCE allocation failed aggregationLevel [%d]\n",
                __func__,AGGREGATION_LEVEL_FOR_COMMON_SPACE);
    }

    return macRetType;
}

/*****************************************************************************
 * Function Name  : deleteOrSkipRetxFailureNode
 * Inputs         :
 * Outputs        : 
 * Returns        : 
 * Description    : This function deletes the nodes from the retx failure queue
 *                  if firstRetxNode is TRUE else skips the nodes
 *****************************************************************************/
void deleteOrSkipRetxFailureNode(DLRetxQueueNode **dlReTxNode_pp, UInt8 firstRetxNode,
                                  InternalCellIndex internalCellIndex)
{
    DLRetxQueueNode *dlReTxNode_p = PNULL;
    DLRetxQueueNode *dlReTxNodeTmp_p = PNULL;
    if(firstRetxNode)
    {
        dlReTxNode_p = (DLRetxQueueNode *) popNode
            (&Msg4RetransmissionFailureQueue_gp[internalCellIndex]);

        if ( dlReTxNode_p )
        {
            dlReTxNodeTmp_p = dlReTxNode_p;

            dlReTxNode_p = (DLRetxQueueNode *) getNextNode
                ((const LTE_SNODE *) (dlReTxNodeTmp_p));

            freeMemPool (dlReTxNodeTmp_p);
        }
    }
    else
    {
        (*dlReTxNode_pp )->isNodeInvalid = TRUE;
        (*dlReTxNode_pp ) = (DLRetxQueueNode *) getNextNode
              ((const LTE_SNODE *) (*dlReTxNode_pp ));
        dlReTxNode_p = *dlReTxNode_pp;
    }
    *dlReTxNode_pp = dlReTxNode_p;
}

/*****************************************************************************
 * Function Name  : deleteOrSkipRetxNode
 * Inputs         :
 * Outputs        : 
 * Returns        : 
 * Description    : This function deletes the nodes from the retx queue
 *                  if firstRetxNode is TRUE else skips the nodes
 *****************************************************************************/
void deleteOrSkipRetxNode(DLRetxQueueNode **dlReTxNode_pp, UInt8 firstRetxNode
                            ,InternalCellIndex internalCellIndex)
{
    DLRetxQueueNode *dlReTxNode_p = PNULL;
    DLRetxQueueNode *dlReTxNodeTmp_p = PNULL;
    if(firstRetxNode)
    {
        dlReTxNode_p = (DLRetxQueueNode *) popNode
            (&Msg4RetransmissionQueue_gp[internalCellIndex]);
        if ( dlReTxNode_p )
        {
            dlReTxNodeTmp_p = dlReTxNode_p;

            dlReTxNode_p = (DLRetxQueueNode *) getNextNode
                ((const LTE_SNODE *) (dlReTxNodeTmp_p));

            freeMemPool (dlReTxNodeTmp_p);
        }
    }
    else
    {
        (*dlReTxNode_pp )->isNodeInvalid = TRUE;
        (*dlReTxNode_pp ) = (DLRetxQueueNode *) getNextNode
              ((const LTE_SNODE *) (*dlReTxNode_pp ));
        dlReTxNode_p = *dlReTxNode_pp;
    }
    *dlReTxNode_pp = dlReTxNode_p;
}

/*****************************************************************************
 * Function Name  : putEntryInMsg4ReTxFailureQueue
 * Inputs         : ueContext_p- A pointer to TempCRNTICtx
 *                  hProcessId - The harq process id whose NACK has been
 *                               received.
 *                  harqRTTExpiryTTI - Expiry TTI of HARQ RTT timer
 * Outputs        :
 * Returns        : None
 * Description    : When handling NACK or timer expiry ,timer thread
 *                  calls this function to put the harq process id in 
 *                  the global Msg4 retransmission failure queue.
 *****************************************************************************/
void putEntryInMsg4ReTxFailureQueue(
        TempCRNTICtx *tempUeCtx_p, UInt8 harqProcessId,
        InternalCellIndex internalCellIndex   )
{
    DLRetxQueueNode *retxFailureNode_p = PNULL;
    DLRetxQueueNode *retxMsg4Node_p = PNULL;
    DLHARQProcess* dlHarqProcess_p = PNULL;

    GET_MEM_FROM_POOL (DLRetxQueueNode, retxFailureNode_p,
            sizeof (DLRetxQueueNode), PNULL);
    if( PNULL == retxFailureNode_p )
    {
        return; 
    }
    RNTIInfo *rntiInfo_p = getUeIdxFromRNTIMap( tempUeCtx_p->tcRNTI, internalCellIndex );
    /* coverity fix 96909 */
    if(PNULL == rntiInfo_p)
    {
        freeMemPool(retxFailureNode_p);
        return;
    }
    retxFailureNode_p->harqProcessId = harqProcessId;
    retxFailureNode_p->ueIndex = rntiInfo_p->index;
    retxFailureNode_p->isNodeInvalid = FALSE;
    dlHarqProcess_p = 
        &( tempUeCtx_p->dlHarqCtx_p->harqProcess[harqProcessId] );
    dlHarqProcess_p->dlHARQTBOneInfo.txIndicator = RE_TX;
    if( pushNode( &Msg4RetransmissionFailureQueue_gp[internalCellIndex],
                &(retxFailureNode_p->anchorNode) ) )
    {
       freeMemPool(retxFailureNode_p);
    }
    else
    {
       retxMsg4Node_p = (DLRetxQueueNode *)getFirstNode( 
               &Msg4RetransmissionQueue_gp[internalCellIndex] );
       deleteOrSkipRetxNode(&retxMsg4Node_p, TRUE, internalCellIndex);
    }
}

/*****************************************************************************
 * Function Name  : processMsg4RetransmissionFailureQueue
 * Inputs         : pdcchDLOutputInfo_p
 *                  dciCCEContainerInfo_p
 *                  availableCommonCCE_p
 *                  availableCCE_p
 *                  availableDLRBCount_p
 *                  subFrameNum
 *                  pdcchULOutputInfo_p
 * Outputs        : 
 * Returns        : Void
 * Description    : This function processes the global Msg4 Retransmission 
 *                  failure queue, and schedules the UE first whose data is 
 *                  queued for retransmission
 *****************************************************************************/
void processMsg4RetransmissionFailureQueue( 
        PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt32 *availableCommonCCE_p, UInt32 *availableCCE_p, 
        SInt32 *availableDLRBCount_p, UInt32 subFrameNum,
        PdcchULOutputInfo *pdcchULOutputInfo_p,
       InternalCellIndex internalCellIndex)
{
    UInt32 allocFromCommon      = FALSE;
    UInt8  numRB            = 0;
    DLMsgInfo     *dlMsgInfo_p   = PNULL;
    DciCCEInfo    *dciCCEInfo_p      = PNULL;
    UInt32 retVal           = MAC_FAILURE;
    UInt32 aggregationLevelForUespecific = AGGREGATION_LEVEL_FOUR;
    UInt32 aggregationLevelForCommon     = AGGREGATION_LEVEL_FOR_COMMON_SPACE;
    UInt8 firstRetxNode = TRUE;
    UInt16 ueIndex = 0;
    UInt32 dlHarqRTTExpiryTTI = 0;
    DLRetxQueueNode *dlReTxNode_p = PNULL;
    TempCRNTICtx *tempUEContext_p = PNULL;
    DLHARQProcess *dlHarqProcess_p = PNULL;
    RNTIInfo      *rntiInfo_p        = PNULL;
    UInt8 dlDelay = PHY_DL_DELAY;
    UInt32 globalTick = getCurrentTick() + dlDelay;
    UInt32 maxUEsToBeScheduledDL = cellSpecificParams_g.cellConfigAndInitParams_p\
                          [internalCellIndex]->initParams_p->dlSchdConfig.maxUEsToBeScheduledDL; 
    TcrntiCCEIndexInfo *tcrntiCCEIndexInfo_p = PNULL;
    UInt8 tempUECount = 0;

    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];
    dlReTxNode_p = (DLRetxQueueNode *) getFirstNode (&Msg4RetransmissionFailureQueue_gp[internalCellIndex]);

    while (dlReTxNode_p && *availableDLRBCount_p && maxUEsToBeScheduledDL)
    { 
        retVal = MAC_FAILURE;
        if (TRUE == dlReTxNode_p->isNodeInvalid)
        {
            deleteOrSkipRetxFailureNode(&dlReTxNode_p, firstRetxNode,internalCellIndex);
            continue;
        }

        ueIndex = dlReTxNode_p->ueIndex;
        tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;

        if (PNULL!=tempUEContext_p)  
        {
            dlHarqProcess_p = &(tempUEContext_p->dlHarqCtx_p->
                    harqProcess[dlReTxNode_p->harqProcessId]);

            dlHarqRTTExpiryTTI = dlHarqProcess_p->dlharqRttExpiryTick;
        }
        else
        {
            deleteOrSkipRetxFailureNode(&dlReTxNode_p, firstRetxNode,internalCellIndex);
            continue;
        }

        if ((dlReTxNode_p->isNodeInvalid) ||
                (HARQ_PROCESS_IN_USE != dlHarqProcess_p->harqStatus))
        {
            deleteOrSkipRetxFailureNode(&dlReTxNode_p, firstRetxNode,internalCellIndex);

            if(HARQ_PROCESS_RESET == dlHarqProcess_p->harqStatus)
            {
                freeDLHarqProcessForMsg4(dlHarqProcess_p);
            }
            continue;
        }

        /* Check for Contention Resolution Tick expiry*/
        if (((globalTick > tempUEContext_p->contentionResExp)
                    || (dlHarqRTTExpiryTTI >= tempUEContext_p->contentionResExp) ) )
        {

            LOG_MSG(PDCCH_ORDER_TIMER_NOCONTEXT_ID,
                    LOGWARNING,
                    MAC_RACH,
                    getCurrentTick(),
                    globalTick,
                    tempUEContext_p->contentionResExp,
                    dlHarqRTTExpiryTTI,
                    LINE_NUMBER,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"RetxExpire");

            freeDLHarqProcessForMsg4( dlHarqProcess_p);
            deleteOrSkipRetxFailureNode(&dlReTxNode_p, firstRetxNode,internalCellIndex);
            continue;
        }

        /* If Harq TTI is not expired, Retransmission cant be done 
           so go to the next node */
        if ((globalTick < dlHarqRTTExpiryTTI ) ||
                (*availableDLRBCount_p < dlHarqProcess_p->
                 assignedResourcesNB))
        {
            LOG_MSG (DRX_HARQ_RETX_TIMER_EXPIRE_LOG_ID, LOGWARNING,
                    MAC_DL_Strategy, getCurrentTick (),\
                    ueIndex,dlHarqProcess_p->harqProcessId,\
                    dlHarqRTTExpiryTTI,\
                    tempUEContext_p->tcRNTI,\
                    dlHarqProcess_p->assignedResourcesNB,\
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME, "");
            dlReTxNode_p =
                (DLRetxQueueNode *)
                getNextNode ((const LTE_SNODE *) (dlReTxNode_p));
            firstRetxNode = FALSE;
            continue;
        }

        numRB = dlHarqProcess_p->assignedResourcesNB;
        rntiInfo_p = getUeIdxFromRNTIMap(tempUEContext_p->tcRNTI,internalCellIndex);
        /* coverity fix 96908 */
        if ((PNULL != rntiInfo_p)&&(*availableCCE_p))
        {
            if (aggregationLevelForUespecific <= *availableCCE_p)
            {
                if ((numRB) && (numRB <= *availableDLRBCount_p))
                {
                    UInt32        count = 0;

                    count = pdcchDLOutputInfo_p->countCommonChannelMsg;
                    dlMsgInfo_p = 
                        &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].dlMsgInfo;

                    retVal = allocateCCEInUESpecificSpaceForContentionResMsgReTx(
                            (rntiInfo_p->ueSearchSpaceInfo_p[internalCellIndex] + subFrameNum),
                            pdcchDLOutputInfo_p,
                            dciCCEInfo_p,
                            dlHarqProcess_p,
                            availableCCE_p,
                            aggregationLevelForUespecific,
                            dciCCEContainerInfo_p,
                            pdcchULOutputInfo_p,
                            tempUEContext_p,
                            internalCellIndex);

                    if ( MAC_SUCCESS == retVal)
                    {
                        tempUEContext_p->status = CONTENTION_SENT;
                        tempUEContext_p->isPdcchAllocated = TRUE;
                        dciCCEContainerInfo_p->\
                            dciCCEInfo[dlMsgInfo_p->dlContentionReslMsgInfo.cceIndex].\
                            dciPduInfo.ueIndex = ueIndex;
                        dciCCEContainerInfo_p->arrayCCEAllocated[
                            dciCCEContainerInfo_p->countOfCCEAllocated] =
                            dlMsgInfo_p->dlContentionReslMsgInfo.cceIndex;

                        *availableDLRBCount_p -= numRB;
                        *availableCommonCCE_p -= aggregationLevelForUespecific;
                    }
                    else
                    {
                        LOG_MSG(PDCCH_ORDER_TIMER_NOCONTEXT_ID,
                                LOGWARNING,
                                MAC_RACH,
                                getCurrentTick(),
                                0,
                                0,
                                0,
                                LINE_NUMBER,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"CCCHRetx CCE Failure");

                        allocFromCommon = TRUE;
                    }
                }
            }
            else
            {
                LOG_MSG(PDCCH_ORDER_TIMER_NOCONTEXT_ID,
                        LOGWARNING,
                        MAC_RACH,
                        getCurrentTick(),
                        *availableCCE_p,
                        aggregationLevelForUespecific,
                        0,
                        LINE_NUMBER,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"CCEs Failure");

                allocFromCommon = TRUE;
            }
        }
        else
        {
            return;
        }

        if(TRUE == allocFromCommon)
        {
            if (*availableCommonCCE_p >= AGGREGATION_LEVEL_FOR_COMMON_SPACE)
            {
                if ((numRB) && (numRB <= *availableDLRBCount_p))
                {
                    UInt32        count = 0;

                    count = pdcchDLOutputInfo_p->countCommonChannelMsg;
                    dlMsgInfo_p = 
                        &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].dlMsgInfo;

                    /*Allocate CCE*/
                    retVal = allocateCCEInCommonSpaceForContentionResMsgReTx(
                            pdcchDLOutputInfo_p,
                            dciCCEInfo_p,
                            dlHarqProcess_p,
                            availableCCE_p,
                            dciCCEContainerInfo_p,
                            pdcchULOutputInfo_p,
                            tempUEContext_p,
                            internalCellIndex); 
                    if ( MAC_SUCCESS == retVal )
                    {
                        tempUEContext_p->isPdcchAllocated = TRUE;
                        dciCCEContainerInfo_p->\
                            dciCCEInfo[dlMsgInfo_p->dlContentionReslMsgInfo.cceIndex].\
                            dciPduInfo.ueIndex = ueIndex;
                        dciCCEContainerInfo_p->arrayCCEAllocated[
                            dciCCEContainerInfo_p->countOfCCEAllocated] =
                            dlMsgInfo_p->dlContentionReslMsgInfo.cceIndex;

                        *availableDLRBCount_p -= numRB;
                        *availableCommonCCE_p -= aggregationLevelForCommon;

                        LOG_MSG(PDCCH_ORDER_TIMER_NOCONTEXT_ID,
                                LOGWARNING,
                                MAC_RACH,
                                getCurrentTick(),
                                ueIndex,
                                dlMsgInfo_p->dlContentionReslMsgInfo.cceIndex,
                                dlHarqProcess_p->harqProcessId,
                                LINE_NUMBER,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"CCCH RETX CommonSpace");
                    }                       
                    else
                    {
                        LOG_MSG(PDCCH_ORDER_TIMER_NOCONTEXT_ID,
                                LOGWARNING,
                                MAC_RACH,
                                getCurrentTick(),
                                *availableCCE_p,
                                aggregationLevelForCommon,
                                0,
                                LINE_NUMBER,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"CCEs Failure");
                        return;
                    }
                }
                else
                {
                    LOG_MSG(PDCCH_ORDER_TIMER_NOCONTEXT_ID,
                            LOGWARNING,
                            MAC_RACH,
                            getCurrentTick(),
                            numRB,
                            *availableDLRBCount_p,
                            0,
                            LINE_NUMBER,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"Rbs Failure");
                    return;
                }
            }
            else
            {
                LOG_MSG(PDCCH_ORDER_TIMER_NOCONTEXT_ID,
                        LOGWARNING,
                        MAC_RACH,
                        getCurrentTick(),
                        *availableCCE_p,
                        aggregationLevelForCommon,
                        0,
                        LINE_NUMBER,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"CCEs Failure");
                return;
            }
        }
        if (MAC_SUCCESS == retVal)
        {
            /* To Send UCI HARQ in UL Config for UE with 
             * temporary context */
            tcrntiCCEIndexInfo_p = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.pdcchTempUEArray; 

            tempUECount = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count;
            tcrntiCCEIndexInfo_p[tempUECount].cceIndex = 
                dlMsgInfo_p->dlContentionReslMsgInfo.cceIndex;
            tcrntiCCEIndexInfo_p[tempUECount].tcrnti = tempUEContext_p->tcRNTI;
            maxUEsToBeScheduledDL--;
        }

        if (MAC_SUCCESS == retVal)
        {
            deleteOrSkipRetxFailureNode(&dlReTxNode_p, firstRetxNode,internalCellIndex);
        }
        else
        {
            dlReTxNode_p =
                (DLRetxQueueNode *)
                getNextNode ((const LTE_SNODE *) (dlReTxNode_p));
            LOG_MSG (DRX_HARQ_RETX_TIMER_EXPIRE_LOG_ID, LOGWARNING,
                    MAC_DL_Strategy, getCurrentTick (),\
                    ueIndex,0,\
                    0,\
                    tempUEContext_p->tcRNTI,\
                    firstRetxNode,\
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME, "");
            firstRetxNode = FALSE;
        }
    }
}

/*****************************************************************************
 * Function Name  : processMsg4RetransmissionQueue
 * Inputs         : pdcchDLOutputInfo_p,
 *                  dciCCEContainerInfo_p,
 *                  availableCommonCCE_p,
 *                  availableCCE_p,
 *                  availableDLRBCount_p,
 *                  subFrameNum,
 *                  pdcchULOutputInfo_p
 * Outputs        : 
 * Returns        : Void
 * Description    : This function processes the global Retransmission queue,and
 *                  schedules the UE first whose data is queued for
 *                  retransmissios
 *****************************************************************************/
void processMsg4RetransmissionQueue( PdcchDLOutputInfo *pdcchDLOutputInfo_p,
        DciCCEContainerInfo *dciCCEContainerInfo_p,
        UInt32 *availableCommonCCE_p, UInt32 *availableCCE_p, 
        SInt32 *availableDLRBCount_p, UInt32 subFrameNum, 
        PdcchULOutputInfo *pdcchULOutputInfo_p 
        ,InternalCellIndex internalCellIndex)
{
    UInt32 allocFromCommon      = FALSE;
    UInt8  numRB            = 0;
    DLMsgInfo     *dlMsgInfo_p   = PNULL;
    DciCCEInfo    *dciCCEInfo_p      = PNULL;
    UInt32 retVal           = MAC_FAILURE;
    UInt32 aggregationLevelForUespecific = AGGREGATION_LEVEL_FOUR;
    UInt32 aggregationLevelForCommon     = AGGREGATION_LEVEL_FOR_COMMON_SPACE;
    UInt8 firstRetxNode = TRUE;
    UInt16 ueIndex = 0;
    UInt32 dlHarqRTTExpiryTTI = 0;
    DLRetxQueueNode *dlReTxNode_p = PNULL;
    TempCRNTICtx *tempUEContext_p = PNULL;
    DLHARQProcess *dlHarqProcess_p = PNULL;
    RNTIInfo      *rntiInfo_p        = PNULL;
    UInt8 dlDelay = PHY_DL_DELAY;
    UInt32 globalTick = getCurrentTick() + dlDelay;
    UInt32 maxUEsToBeScheduledDL = cellSpecificParams_g.cellConfigAndInitParams_p\
                           [internalCellIndex]->initParams_p->dlSchdConfig.maxUEsToBeScheduledDL; 
    TcrntiCCEIndexInfo *tcrntiCCEIndexInfo_p = PNULL;
    UInt8 tempUECount = 0;

    dciCCEInfo_p = &dciCCEContainerInfo_p->dciCCEInfo[0];
    dlReTxNode_p = (DLRetxQueueNode *) getFirstNode (&Msg4RetransmissionQueue_gp[internalCellIndex]);

    while (dlReTxNode_p && *availableDLRBCount_p && maxUEsToBeScheduledDL)
    { 
        retVal = MAC_FAILURE;
        if (TRUE == dlReTxNode_p->isNodeInvalid)
        {
            deleteOrSkipRetxNode(&dlReTxNode_p, firstRetxNode,internalCellIndex);
            continue;
        }

        ueIndex = dlReTxNode_p->ueIndex;
        tempUEContext_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;

        if (PNULL!=tempUEContext_p)  
        {
            dlHarqProcess_p = &(tempUEContext_p->dlHarqCtx_p->
                    harqProcess[dlReTxNode_p->harqProcessId]);

            dlHarqRTTExpiryTTI = dlHarqProcess_p->dlharqRttExpiryTick;
        }
        else
        {
            deleteOrSkipRetxNode(&dlReTxNode_p, firstRetxNode,internalCellIndex);
            continue;
        }

        if ((dlReTxNode_p->isNodeInvalid) ||
                (HARQ_PROCESS_IN_USE != dlHarqProcess_p->harqStatus))
        {
            deleteOrSkipRetxNode(&dlReTxNode_p, firstRetxNode,internalCellIndex);

            if(HARQ_PROCESS_RESET == dlHarqProcess_p->harqStatus)
            {
                freeDLHarqProcessForMsg4(dlHarqProcess_p);
            }
            continue;
        }

        /* Check for Contention Resolution Tick expiry*/
        if (((globalTick > tempUEContext_p->contentionResExp)
                    || (dlHarqRTTExpiryTTI >= tempUEContext_p->contentionResExp) ) )
        {

            LOG_MSG(PDCCH_ORDER_TIMER_NOCONTEXT_ID,
                    LOGWARNING,
                    MAC_RACH,
                    getCurrentTick(),
                    globalTick,
                    tempUEContext_p->contentionResExp,
                    dlHarqRTTExpiryTTI,
                    LINE_NUMBER,
                    DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"RetxExpire");


            freeDLHarqProcessForMsg4(dlHarqProcess_p);
            deleteOrSkipRetxNode(&dlReTxNode_p, firstRetxNode,internalCellIndex);
            continue;
        }

        /* If Harq TTI is not expired, Retransmission cant be done 
           so go to the next node */
        if ((globalTick < dlHarqRTTExpiryTTI ) ||
                (*availableDLRBCount_p < dlHarqProcess_p->
                 assignedResourcesNB))
        {
            LOG_MSG (DRX_HARQ_RETX_TIMER_EXPIRE_LOG_ID, LOGWARNING,
                    MAC_DL_Strategy, getCurrentTick (),\
                    ueIndex,dlHarqProcess_p->harqProcessId,\
                    dlHarqRTTExpiryTTI,\
                    tempUEContext_p->tcRNTI,\
                    dlHarqProcess_p->assignedResourcesNB,\
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME, "");
            dlReTxNode_p =
                (DLRetxQueueNode *)
                getNextNode ((const LTE_SNODE *) (dlReTxNode_p));
            firstRetxNode = FALSE;
            continue;
        }

        numRB = dlHarqProcess_p->assignedResourcesNB;
        rntiInfo_p = getUeIdxFromRNTIMap(tempUEContext_p->tcRNTI,internalCellIndex);
        /* coverity fix 96904 */
        if ((PNULL != rntiInfo_p)&&(*availableCCE_p))
        {
            if (aggregationLevelForUespecific <= *availableCCE_p)
            {
                if ((numRB) && (numRB <= *availableDLRBCount_p))
                {
                    UInt32        count = 0;

                    count = pdcchDLOutputInfo_p->countCommonChannelMsg;
                    dlMsgInfo_p = 
                        &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].dlMsgInfo;

                    retVal = allocateCCEInUESpecificSpaceForContentionResMsgReTx(
                            (rntiInfo_p->ueSearchSpaceInfo_p[internalCellIndex] + subFrameNum),
                            pdcchDLOutputInfo_p,
                            dciCCEInfo_p,
                            dlHarqProcess_p,
                            availableCCE_p,
                            aggregationLevelForUespecific,
                            dciCCEContainerInfo_p,
                            pdcchULOutputInfo_p,
                            tempUEContext_p,
                            internalCellIndex);

                    if ( MAC_SUCCESS == retVal)
                    {
                        tempUEContext_p->status = CONTENTION_SENT;
                        tempUEContext_p->isPdcchAllocated = TRUE;
                        dciCCEContainerInfo_p->\
                            dciCCEInfo[dlMsgInfo_p->dlContentionReslMsgInfo.cceIndex].\
                            dciPduInfo.ueIndex = ueIndex;
                        dciCCEContainerInfo_p->arrayCCEAllocated[
                            dciCCEContainerInfo_p->countOfCCEAllocated] =
                            dlMsgInfo_p->dlContentionReslMsgInfo.cceIndex;

                        *availableDLRBCount_p -= numRB;
                        *availableCommonCCE_p -= aggregationLevelForUespecific;
                    }
                    else
                    {
                        LOG_MSG(PDCCH_ORDER_TIMER_NOCONTEXT_ID,
                                LOGWARNING,
                                MAC_RACH,
                                getCurrentTick(),
                                0,
                                0,
                                0,
                                LINE_NUMBER,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"CCCHRetx CCE Failure");

                        allocFromCommon = TRUE;
                    }
                }
            }
            else
            {
                LOG_MSG(PDCCH_ORDER_TIMER_NOCONTEXT_ID,
                        LOGWARNING,
                        MAC_RACH,
                        getCurrentTick(),
                        *availableCCE_p,
                        aggregationLevelForUespecific,
                        0,
                        LINE_NUMBER,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"CCEs Failure");

                allocFromCommon = TRUE;
            }
        }
        else
        {
            return;
        }

        if(TRUE == allocFromCommon)
        {
            if (*availableCommonCCE_p >= AGGREGATION_LEVEL_FOR_COMMON_SPACE)
            {
                if ((numRB) && (numRB <= *availableDLRBCount_p))
                {
                    UInt32        count = 0;

                    count = pdcchDLOutputInfo_p->countCommonChannelMsg;
                    dlMsgInfo_p = 
                        &pdcchDLOutputInfo_p->dlMsgTypeCommonMsgInfo[count].dlMsgInfo;

                    /*Allocate CCE*/
                    retVal = allocateCCEInCommonSpaceForContentionResMsgReTx(
                            pdcchDLOutputInfo_p,
                            dciCCEInfo_p,
                            dlHarqProcess_p,
                            availableCCE_p,
                            dciCCEContainerInfo_p,
                            pdcchULOutputInfo_p,
                            tempUEContext_p,
                            internalCellIndex ); 
                    if ( MAC_SUCCESS == retVal )
                    {
                        tempUEContext_p->isPdcchAllocated = TRUE;
                        dciCCEContainerInfo_p->\
                            dciCCEInfo[dlMsgInfo_p->dlContentionReslMsgInfo.cceIndex].\
                            dciPduInfo.ueIndex = ueIndex;
                        dciCCEContainerInfo_p->arrayCCEAllocated[
                            dciCCEContainerInfo_p->countOfCCEAllocated] =
                            dlMsgInfo_p->dlContentionReslMsgInfo.cceIndex;

                        *availableDLRBCount_p -= numRB;
                        *availableCommonCCE_p -= aggregationLevelForCommon;

                        LOG_MSG(PDCCH_ORDER_TIMER_NOCONTEXT_ID,
                                LOGWARNING,
                                MAC_RACH,
                                getCurrentTick(),
                                ueIndex,
                                dlMsgInfo_p->dlContentionReslMsgInfo.cceIndex,
                                dlHarqProcess_p->harqProcessId,
                                LINE_NUMBER,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"CCCH RETX CommonSpace");
                    }                       
                    else
                    {
                        LOG_MSG( PDCCH_ORDER_TIMER_NOCONTEXT_ID, LOGWARNING,
                                MAC_RACH, getCurrentTick(), *availableCCE_p,
                                aggregationLevelForCommon, 0, __LINE__, 0,
                                0, 0, __func__, "CCEs Failure");

                        putEntryInMsg4ReTxFailureQueue(tempUEContext_p,
                                dlHarqProcess_p->harqProcessId,
                                internalCellIndex);

                        return;
                    }
                }
                else
                {
                    LOG_MSG(PDCCH_ORDER_TIMER_NOCONTEXT_ID,
                            LOGWARNING,
                            MAC_RACH,
                            getCurrentTick(),
                            numRB,
                            *availableDLRBCount_p,
                            0,
                            LINE_NUMBER,
                            DEFAULT_INT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,
                            FUNCTION_NAME,"Rbs Failure");

                    putEntryInMsg4ReTxFailureQueue(tempUEContext_p,
                            dlHarqProcess_p->harqProcessId,
                            internalCellIndex);

                    return;
                }
            }
            else
            {
                LOG_MSG(PDCCH_ORDER_TIMER_NOCONTEXT_ID,
                        LOGWARNING,
                        MAC_RACH,
                        getCurrentTick(),
                        *availableCCE_p,
                        aggregationLevelForCommon,
                        0,
                        LINE_NUMBER,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"CCEs Failure");

                putEntryInMsg4ReTxFailureQueue(tempUEContext_p,
                        dlHarqProcess_p->harqProcessId,
                        internalCellIndex);

                return;
            }
        }
        if (MAC_SUCCESS == retVal)
        {
            /* To Send UCI HARQ in UL Config for UE with 
             * temporary context */
            tcrntiCCEIndexInfo_p = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.pdcchTempUEArray; 

            tempUECount = pdcchULOutputInfo_p->pdcchTcrntiUEInfo.count;
            tcrntiCCEIndexInfo_p[tempUECount].cceIndex = 
                dlMsgInfo_p->dlContentionReslMsgInfo.cceIndex;
            tcrntiCCEIndexInfo_p[tempUECount].tcrnti = tempUEContext_p->tcRNTI;
            maxUEsToBeScheduledDL--;
        }

        if (MAC_SUCCESS == retVal)
        {
            deleteOrSkipRetxNode(&dlReTxNode_p, firstRetxNode,internalCellIndex);
        }
        else
        {
            dlReTxNode_p =
                (DLRetxQueueNode *)
                getNextNode ((const LTE_SNODE *) (dlReTxNode_p));
            LOG_MSG (DRX_HARQ_RETX_TIMER_EXPIRE_LOG_ID, LOGWARNING,
                    MAC_DL_Strategy, getCurrentTick (),\
                    ueIndex,0,\
                    0,\
                    tempUEContext_p->tcRNTI,\
                    firstRetxNode,\
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME, "");
            firstRetxNode = FALSE;
        }
    }
}
#endif


