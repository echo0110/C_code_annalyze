/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacDemux.c,v 1.2.6.1.4.2.2.2 2010/10/22 12:44:10 gur04640 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the function definitions for 
 *                     Demultiplexing functionality
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacDemux.c,v $
 * Revision 1.2.6.1.4.2.2.2  2010/10/22 12:44:10  gur04640
 * Statistics code merged from branch BRFramework_RRC1_0_RLC_AM_MAC_8_8_FAPI_optimizations
 *
 * Revision 1.2.6.1.4.2.2.1.2.1  2010/10/13 10:53:20  gur04640
 * Changes done for implementing Stats using shared memory
 *
 * Revision 1.2.6.1.4.2.2.1.2.1  2010/10/12 02:46:54  gur04640
 * Changes done for implementing Stats using shared memory
 *
 * Revision 1.2.6.1.4.2.2.1  2010/09/21 15:45:59  gur20491
 * FAPI changes
 *
 * Revision 1.2.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.45.20.1  2010/07/21 09:09:18  gur24420
 * Aligned changes with respect to eNode Integration activity(based on RMI-NetLogic Code).
 * Overall Changes Cover:-
 * 1. Changes taken from netlogic code to avoid Integration problems.
 *
 * Revision 1.45  2010/04/05 12:00:03  gur20439
 * removal of compilation warning
 *
 * Revision 1.44  2010/03/30 10:19:28  gur18550
 * Uplink Throughput Calc
 *
 * Revision 1.43  2010/03/26 10:46:21  gur15697
 * Demultiplexing of UL data when multiple logical channels are there in case of zero buffer copy
 *
 * Revision 1.42  2009/12/11 08:14:00  gur11912
 * modified for end padding length under ZERO_BUFFER_COPY
 *
 * Revision 1.41  2009/12/02 07:26:07  gur19413
 * modify condition if (numNodesToProcess < nodesToProcess)
 *
 * Revision 1.40  2009/11/29 09:21:10  gur11912
 * modified functionality when number of Demux nodes to process is called
 *
 * Revision 1.39  2009/11/06 11:10:43  gur11912
 * corrected value of dataToRlc_p->numLCInfo
 *
 * Revision 1.38  2009/10/30 12:33:14  gur11912
 * fixed for length of LCID
 *
 * Revision 1.37  2009/09/16 09:49:44  gur11912
 * Modified for padding handling
 *
 * Revision 1.36  2009/09/09 13:57:31  gur11912
 * removed msgAlloc
 *
 * Revision 1.35  2009/07/31 12:01:45  gur15697
 * splint warnings
 *
 * Revision 1.34  2009/07/28 14:47:37  gur15697
 * UT bugfix
 *
 * Revision 1.33  2009/07/27 15:09:15  gur15697
 * splint warnings
 *
 * Revision 1.32  2009/07/21 12:26:33  gur15697
 * UT bugfix
 *
 * Revision 1.31  2009/07/17 05:44:40  gur15697
 * splin warnings removed
 *
 * Revision 1.30  2009/07/13 12:11:53  gur15697
 * second argument to LTE_MAC_UT_LOG modifed
 *
 * Revision 1.29  2009/07/13 10:14:40  gur18569
 * setting freed node to null
 *
 * Revision 1.28  2009/07/04 10:53:10  gur15697
 * UT bug fixes
 *
 * Revision 1.27  2009/07/01 06:21:10  gur15697
 * LTE_LOG changed to LTE_MAC_UT_LOG
 *
 * Revision 1.26  2009/06/27 05:43:55  gur15697
 * tempUEContext_p->status = MSG3_RECEIVED removed
 *
 * Revision 1.25  2009/06/26 03:55:32  gur15697
 * numOfDemuxThreads argument removed from putDataInDemuxQueue()
 *
 * Revision 1.24  2009/06/22 08:47:57  gur15697
 * review comments incorporated
 *
 * Revision 1.23  2009/06/19 13:23:35  gur15697
 * modified sendCCCHMsgToRRC()
 *
 * Revision 1.22  2009/06/15 05:30:53  gur15697
 * UT bug fix
 *
 * Revision 1.21  2009/06/15 05:18:21  gur15697
 * UT bug fix
 *
 * Revision 1.20  2009/06/09 14:16:22  gur15697
 * decodeAndSendCCCHMsgToRRC() modified
 *
 * Revision 1.19  2009/06/05 08:04:25  gur15697
 * init and cleanup functions added
 *
 * Revision 1.18  2009/06/04 15:13:55  gur19413
 * compilation errors removed
 *
 * Revision 1.17  2009/06/04 12:58:31  gur15697
 * review comments incorporated
 *
 * Revision 1.16  2009/06/03 12:06:45  gur15697
 * initDeMuxMgr() added
 *
 * Revision 1.15  2009/06/03 10:05:27  gur15697
 * bug fixes in UT cycle-1
 *
 * Revision 1.14  2009/06/02 07:38:02  gur15697
 * comments encorporated
 *
 * Revision 1.13  2009/06/01 07:20:13  gur18569
 * removed compilation errors
 *
 * Revision 1.12  2009/05/28 10:12:38  gur15697
 * macros NON_ZERO_TO_ZERO_BSR and  ZERO_TO_NON_ZERO_BSR removed
 *
 * Revision 1.11  2009/05/28 07:40:25  gur15697
 * updated as per the modified DL and UL context
 *
 * Revision 1.10  2009/05/27 09:33:30  gur15697
 * RACH handling added
 *
 * Revision 1.9  2009/05/22 06:11:42  gur15697
 * as per the modified UL and DL context code modified
 *
 * Revision 1.8  2009/05/21 04:24:30  gur15697
 * sendMsgToRRC() arguments modified
 *
 * Revision 1.7  2009/05/20 05:35:22  gur15697
 * sendPowerHeadroomToRRC() funtion added
 *
 * Revision 1.6  2009/05/19 05:53:48  gur15697
 * Compilation errors removed
 *
 * Revision 1.5  2009/05/18 05:55:17  gur15697
 * Truncated BSR handling added, Code for Zero buffer copy added and Incorporated the comments
 *
 * Revision 1.4  2009/05/11 10:35:08  gur15697
 * short BSR handling added
 *
 * Revision 1.3  2009/05/08 14:07:14  gur15697
 * Short BSR handling added and compilation errors removed
 *
 * Revision 1.2  2009/05/05 11:42:05  gur15697
 * modified to add logic in the functions
 *
 * Revision 1.1  2009/04/30 06:49:26  gur15697
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
#include "lteMacDemux.h"
#include "lteMacExecutionLegs.h"
#include "lteMacCellConfMgr.h"
#include "lteMacOAMInterface.h"
#include "lteMacPuschPowerControl.h"
#include "lteMacULPowerControlData.h"
#ifdef OVERSHOOT_CORRECTION
#include "lteCommonStatsManager.h"
#endif
#include "lteMacRRMPeriodicMgr.h"
/* SPS CHANGES */
#include "lteMacULSpsScheduler.h"
#include "lteMacPdcpInterface.h"
/* SPS CHANGES */
#ifdef DL_UL_SPLIT_TDD
#include "lteMacTddMgr.h"  
#endif
#ifdef TDD_CONFIG
 /* +GCC 4.7 */
#if defined(KPI_STATS) || defined(PERF_STATS)
 /* -GCC 4.7 */
#include "lteMacTDDConfig.h"
#endif
#endif
#include "ltePerfStats.h"
/******************************************************************************
 Private Definitions
 *****************************************************************************/



/******************************************************************************
 Private Types
 *****************************************************************************/
/*array of de-mux queues*/
/*CA Changes start  */
DeMuxThreadInfo* deMuxThreadPool_gp[MAX_NUM_CELL] = {PNULL};
/*CA Changes end  */
#if (defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT) || defined (DL_UL_SPLIT_TDD))
static UInt32 dlQosMappingForTokenAdjust[MAX_UE_SUPPORTED][MAX_HARQ_PROCESS_NB][MAX_LOGICAL_CHANNEL] ;
DemuxProcessMap* demuxProcessMap_gp[MAX_NUM_CELL] = {PNULL};
#endif
#ifdef DEBUG_STATS
pduUlDebugInfo debugULPDUData_g[MAX_NUM_UE_DEBUG]= {{0}};
#endif

/* SPR 23402 Changes Start */
/* MOCN Changes Start */
extern mocnOperatorParams mocnOperatorParams_g;
/* MOCN Changes End */
/* SPR 23402 Changes End */

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static void dataDeMultiplexAndSend(DemuxDataBase *demuxData_p,
                                   UInt8   *origDataPtr_p,
                                   UInt8   *dataPtr_p,
                                   UInt16  ueIndex,
                                   UInt16  recvdRNTI,
                                   UInt32  pos,
                                   UInt32  dataPtrPos,
                                   /* SPR 15909 fix start */
                                   tickType_t  ttiCounter,
                                   /* SPR 15909 fix end */
                                   UInt32  thdIndex,
                                   UInt32  dlDelay,
                                   /* +CLPC_CHG_LJA*/
                                   SInt8   powerControlDeltaTFAndLogFactor,
                                   /* -CLPC_CHG_LJA*/ /* SPR 3486 - Start/END */
/* SPR 4751 changes */
                                   UInt8 harqProcessId,
/* SPR 4751 changes */
/*CA Changes start  */
                                   InternalCellIndex internalCellIndex
/*CA Changes end  */
                                   );

static UInt8* calculateMACSubHeaderLength(UInt8 *headerPtr_p,
                                        UInt32 *len_p,
                                        UInt32 *dataPtrPos_p);

static MacRetType deMultiplexTCRNTIMessage(DemuxDataBase *demuxData_p,
                                    void    *origDataPtr_p,
                                    UInt8   *dataPtr_p,
                                    UInt16  tcRNTI,
                                    UInt32  pos,
                                    UInt32  dataPtrPos,
                                    TempCRNTICtx *tempUEContext_p,
                                    /* SPR 15909 fix start */
                                    tickType_t  ttiCounter,
                                    /* SPR 15909 fix end */
                                    UInt32 dlDelay,
                                    /* SPR 15093 fix start */   
                                    UInt32  thdIndex,
                                    /* SPR 15093 fix end */   
                                    /*CA Changes start  */
                                    InternalCellIndex internalCellIndex);
                                    /*CA Changes end  */

#ifdef PERF_STATS
extern UInt32 perfStatsULDelTrigger_g[PERF_MAX_UE_SUPPORTED];
#endif

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* SPR 609 */
/*SPR 21179 Start*/
#ifdef SSI_DEBUG_STATS
/*SPR 21179 End*/
/* +- SPR 17777 */
extern void lteMacUpdateStatsRlcTx(RLCUplinkData * pRlcBuffer);
/*SPR 21179 Start*/
#endif 
/*SPR 21179 End*/
/* +- SPR 17777 */
/* SPR 609 */
UInt8 checkSpsBsrOnlyReceived(ULUEContext* ulCntxt_p);
/*Cyclomatic Complexity changes - starts here*/
static  void processMacUlCcchLch(UInt32 startPaddingFlag,
        UInt32 extnFlag,
        UInt32 *dataPtrPos,
        SInt32 *remainingPayloadLength,
        UInt32 *sucessDecodeFlag,
        UInt32 *localLength,
        UInt8  **dataPtr_p,
        DemuxDataBase *demuxData_p,
        UInt32 *pos,
        UInt16  rcvdRNTI);
static  void processMacLcid(UInt32 startPaddingFlag,
        UInt32 extnFlag,
        UInt32 *dataPtrPos,
        SInt32 *remainingPayloadLength,
        UInt32 *sucessDecodeFlag,
        UInt8  **dataPtr_p,
        DemuxDataBase *demuxData_p,
        UInt32 *pos,
        UInt32 lcId,
        UInt32 *localLength,
        UInt16  ueIndex,
        UInt32  dataLen,
        /* SPR 15909 fix start */
        /* +- SPR 17777 */
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD)
#ifdef TDD_CONFIG
        tickType_t  rcvdTTI,
#endif
#endif
        /* +- SPR 17777 */
        /* SPR 15909 fix end */
        UInt16  rcvdRNTI
        /* SPR 11160 Changes Start */
        /** +- SPR 12364 **/
        ,UInt8  harqProcessId
        /* SPR 11160 Changes End */
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex        
        /*CA Changes end  */
        );        
static  void processMacPowerHeadLch(SInt32 *remainingPayloadLength,
        UInt32 *sucessDecodeFlag,
        DemuxDataBase *demuxData_p,
        UInt32 *pos,
        UInt16 ueIndex,
        UInt32 *dataPtrPos,
        /* SPR 9202 Fix Starts */
        UInt16  rcvdRNTI,
        /* SPR 9202 Fix Ends */
        InternalCellIndex internalCellIndex);
static  void processMacCrntiLch(SInt32 *remainingPayloadLength,
        UInt32 *sucessDecodeFlag,
        UInt32 *pos,
        DemuxDataBase *demuxData_p,
        UInt32 *dataPtrPos);
static  void processShortOrTruncatedBsr(SInt32 *remainingPayloadLength,
        UInt32 *sucessDecodeFlag,
        UInt32 *pos,
        DemuxDataBase *demuxData_p,
        UInt32 *dataPtrPos,
        UInt32 lcId);
static  void processMacLongBsrLch(SInt32 *remainingPayloadLength,
        UInt32 *sucessDecodeFlag,
        UInt32 *pos,
        DemuxDataBase *demuxData_p,
        UInt32 *dataPtrPos);
static  void processMacPaddingLch(UInt32 extnFlag,
        UInt32 *dataPtrPos,
        SInt32 *remainingPayloadLength, 
        UInt32 *localLength, 
        UInt32 *sucessDecodeFlag,
        UInt32 pos,
        DemuxDataBase *demuxData_p,
        UInt32 *startPaddingFlag);
static  void updateTempPreviousBsrDuringSPS(ULUEContext *ulCntxt_p,
        UInt32 *tempPreviousBsr);
static  void demuxAndSendMacUlCcchLchMsg(DLUEContext *dlCntxt_p,
        ULUEContext *ulCntxt_p,
        UInt16 recvdRNTI,
        /* SPR 15909 fix start */
        tickType_t ttiCounter,
        /* SPR 15909 fix end */
        UInt32 dlDelay,
        UInt32 *dataPtrPos,
        /* +- SPR 17777 */
        void **org_p,
        UInt32 *length,
        RLCUplinkData **dataToRlc_p,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex);	
        /*CA Changes end  */        
static  void demuxMacOneToTenLchMsg(UInt32 lchId,
        UInt32 *lcIdx,
        ULUEContext *ulCntxt_p,
        DLUEContext *dlCntxt_p,
        /* + SPS_TDD_CHANGES */
        UInt16 recvdRNTI,
        /* - SPS_TDD_CHANGES */
        UInt32 *rlcFlag,
        void **org_p,
        UInt8 **rlcData_p,
        UInt32 *dataPtrPos,
        UInt32 length,
        UInt32 *invalidLCDataFlag,
        UInt32 *lcgIdVal,
        UInt32 longBSRFlag,
        UInt32 shortBSRFlag,
        UInt32 *bsrReport,
        UInt8 ulHarqProcessId,
        UInt32 shortBSRLcgId,
        UInt16  ueIndex,
        UInt8 **dataPtr_p,
        RLCUplinkData **dataToRlc_p,
    /*UL Force Grant Changes Start */
        UInt8 *ulDataReceivedFlag,  
        InternalCellIndex internalCellIndex	
    /*UL Force Grant Changes End */
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD)
#ifdef TDD_CONFIG
        /* SPR 15909 fix start */
       ,tickType_t ttiCounter
       /* SPR 15909 fix end */
#endif
#endif
       /* +- SPR 17777 */
       /* SPR 11331 Fix End */
       );
STATIC  void validatePHRForPLCalculation(SInt8 pwrHeadRoomInDb,
        CellConfigParams *cellParams_p,
        ULUEContext *ulCntxt_p,
        SInt8 powerControlDeltaTFAndLogFactor,
        UInt16  ueIndex,
        UInt8 harqProcessId,
        /* SPR 15909 fix start */
        tickType_t  ttiCounter,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex);
STATIC  void demuxMacPowerHeadLchMsg(UInt8 **dataPtr_p,
        /* +- SPR 17777 */
        /* +- SPR 17777 */
        ULUEContext *ulCntxt_p,
        UInt32 *dataPtrPos,
        UInt32 length,
        SInt8  powerControlDeltaTFAndLogFactor,
       /* SPR 15909 fix start */
        tickType_t ttiCounter,
        /* SPR 15909 fix end */
        UInt8 harqProcessId,
        UInt16 ueIndex,
        /*CA Changes start  */
        InternalCellIndex internalCellIndex);	
        /*CA Changes end  */        
static  void freeEndPaddingZeroCopyBuffer(UInt32 rlcFlag,
        UInt32 invalidLCDataFlag,
        void   **org_p,
        UInt32 *dataPtrPos);

        

/*Cyclomatic Complexity changes - ends here*/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/*****************************************************************************
 * Function Name  : initDeMuxMgr
 * Inputs         : maxDemuxQueues - Number of Demux queues to initialize,
 *                  numCellsConfigured - Number of Cells Configured
 * Outputs        : None
 * Returns        : None
 * Description    : This will initializes the global queues and variables 
 *                  required by the Demux manager.
 *****************************************************************************/
/*CA Changes start  */
void initDeMuxMgr(UInt32 maxDemuxQueues, UInt8 numCellsConfigured)
/*CA Changes end  */
{
    UInt32 i = 0;
    UInt8 cellCount = 0;
    UInt16 macMaxNumOfUeSupported = 0;
    LTE_MAC_UT_LOG(LOG_DETAIL ,DEMUX,
           "[%s]: initDeMuxMgr()\n",__func__);

    /* Start 128 UE: Memory Changes */
    createMemPool(sizeof(RLCUplinkData), MAX_UL_UE_SCHEDULED);
    /* End 128 UE: Memory Changes */

    /*CA Changes start  */
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT)
    createMemPool(sizeof(DemuxProcessMap), numCellsConfigured);
#endif
    createMemPool(sizeof(DeMuxThreadInfo) * maxDemuxQueues, numCellsConfigured);
    for (cellCount = 0; cellCount < numCellsConfigured; cellCount++)
    {
        macMaxNumOfUeSupported = cellSpecificParams_g.cellConfigAndInitParams_p[cellCount]->initParams_p->macMaxNumOfUeSupported;
        createMemPool((UInt32)sizeof(DataQueueInfoNode), macMaxNumOfUeSupported);
        GET_MEM_FROM_POOL(DeMuxThreadInfo, deMuxThreadPool_gp[cellCount],
                          sizeof(DeMuxThreadInfo) * maxDemuxQueues, PNULL);
        for(i = 0; i < maxDemuxQueues; i++)
        {
            sQueueInit(&(deMuxThreadPool_gp[cellCount] + i)->demuxQueue);
        }

#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT)
        GET_MEM_FROM_POOL(DemuxProcessMap, demuxProcessMap_gp[cellCount],
                          sizeof(DemuxProcessMap), PNULL);

         /** SPR 14605 Changes Start **/
         memSet(demuxProcessMap_gp[cellCount],0,sizeof(DemuxProcessMap));
         /** SPR 14605 Changes End **/

#endif
    }
    /*CA Changes end  */

}

/****************************************************************************
* Function Name  : cleanupDeMuxMgr
* Inputs         : maxDemuxQueues - Number of Demux queues
*                  internalCellIndex - Cell-Index at MAC
* Outputs        : None
* Returns        : None
* Description    : This API cleanup the cleanup Demux Manager.
****************************************************************************/
/*CA Changes start  */
void cleanupDeMuxMgr(UInt32 maxDemuxQueues, InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    UInt32 i = 0, j = 0;
    UInt32 queueCount = 0;
    DataQueueInfoNode *dataQueueInfoNode_p = PNULL;
    
    for (i = 0; i < maxDemuxQueues; i++)
    {
        /*CA Changes start  */
        queueCount = sQueueCount(&(deMuxThreadPool_gp[internalCellIndex] + i)->demuxQueue);
        /*CA Changes end  */
        for (j = 0; j < queueCount; j++)
        {
            /*CA Changes start  */
            dataQueueInfoNode_p = 
                (DataQueueInfoNode *) popNode(&(deMuxThreadPool_gp[internalCellIndex] + i)->demuxQueue);
            /*CA Changes end  */
            
            if (dataQueueInfoNode_p)
            {
                if (dataQueueInfoNode_p->recvdUlData_p)
                {
                    msgFree(dataQueueInfoNode_p->recvdUlData_p);
                    dataQueueInfoNode_p->recvdUlData_p = PNULL;
                }
                freeMemPool(dataQueueInfoNode_p);
                dataQueueInfoNode_p = PNULL;
            }
        }
    }
}

/*****************************************************************************
 * Function Name  : processDemuxQueue
 * Inputs         : thdIndex - represents the deMuxThreadPoolArr_g[] array index,
 *                  numNodesToProcess - Number of queue nodes to process,
 *                  dlDelay  - DL delay
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This will wait on semaphore corresponding to 
 *                  De-Mux thread , pops the node from the demux queue and 
 *                  calls the processDemuxData() with queue node information as 
 *                  arguments.
 *****************************************************************************/
void  processDemuxQueue(UInt32 thdIndex, 
                        UInt32 numNodesToProcess,
                        UInt32 dlDelay,
                        /*CA Changes start  */
                        InternalCellIndex  internalCellIndex
                        /*CA Changes end  */
                        ,ExecutionLegFunctionArg* execArgs
                        )
{
    DeMuxThreadInfo   *demuxThread_p     = PNULL;
    DataQueueInfoNode *demuxNode_p       = PNULL;
    DLUEContext       *dlUEContext_p     = PNULL;
    DLUEContextInfo   *dlUEContextInfo_p = PNULL;
    ULUEContext       *ulUEContext_p     = PNULL;
    ULUEContextInfo   *ulUEContextInfo_p = PNULL;
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD)
    UInt8 startHpId = 0;
    UInt8 startLcId = 0; 
    /* CA Changes start */
    UInt8  ulSchedulerStrategy =  cellSpecificParams_g.\
                 cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
                  ulSchdConfig.ulSchedulerStrategy;
    /* CA Changes end */
#endif
    RNTIInfo          *rntiInfo_p        = PNULL;  

    UInt32  nodesToProcess = 0;
    UInt32  msgFreeFlag = FALSE;
    UInt32  retVal      = MAC_FAILURE;   
#if (defined OVERSHOOT_CORRECTION && (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD)))
    /** CA2 Changes Start **/
    OverLoadLevel *currTickOverLoad_p = &currTickOverLoad_g[internalCellIndex];
    /** CA2 Changes End **/
#endif/*OVERSHOOT_CORRECTION*/    

    /* +GCC 4.7 */
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD)
#ifdef KPI_STATS
	/* + KPI_CA */
    UInt32 activeIndexForMacStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
	/* - KPI_CA */
#endif
#endif
    /* -GCC 4.7 */
    LTE_MAC_UT_LOG(LOG_DETAIL ,DEMUX,
            "Entry [%s]: Function\n",__func__);

    /*CA Changes start  */
    demuxThread_p = deMuxThreadPool_gp[internalCellIndex] + thdIndex;
    /*CA Changes end  */

    if (numNodesToProcess)
    {
        nodesToProcess = sQueueCount(&demuxThread_p->demuxQueue);

        if (nodesToProcess)
        {    
            if (numNodesToProcess < nodesToProcess)
            {
                nodesToProcess = numNodesToProcess;
            }
        }
    }
    else
    {
        /*If the nodesToProcess is zero process the queue count 
         *number of nodes
         */
        nodesToProcess = sQueueCount(&demuxThread_p->demuxQueue);
    }

    while (nodesToProcess)
    {
#if (defined OVERSHOOT_CORRECTION && (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD)))
        UInt64 rem_cycles = 0;
        GET_PROCESSING_CYCLES_LEFT(rem_cycles);
        if ( ( !noTimeToProcessflag_g ) && ( rem_cycles >= PROCESS_NODE_TIME_REQD ) )
        {
#endif
            do
            {
                demuxNode_p = (DataQueueInfoNode *)popNode(&(demuxThread_p->demuxQueue));
                /* SPR 6954 Changes starts  */
            }while((--nodesToProcess) &&(PNULL == demuxNode_p));

            if(PNULL == demuxNode_p)
            {
                LTE_MAC_UT_LOG(LOG_WARNING,DEMUX,
                        "dataDeMultiplexAndSend: demuxNode_p is NULL\n");
                /* + SPR 16766 */
                /* +- SPR 17777 */
                LOG_MAC_MSG( MAC_QUEUE_EMPTY, LOGWARNING, MAC_RRC_INF,
                        getCurrentTick(), __LINE__,internalCellIndex,nodesToProcess,execArgs->delayToPhy,0,
                        0, 0, __func__,"demuxQueue");

                /* - SPR 16766 */
                return;
            }
            /* SPR 6954 Changes ends */



            /*Check this is TCRNTI or CRNTI*/
            /*CA Changes start  */
            rntiInfo_p = getUeIdxFromRNTIMap(demuxNode_p->rcvdRNTI, internalCellIndex);
            /*CA Changes end  */
            if (PNULL == rntiInfo_p  ||
                 /*SPR 7100 Fix start*/
                 (FREERNTI == rntiInfo_p->rntiFlag))
                 /*SPR 7100 Fix end*/
            {
                LTE_MAC_UT_LOG(LOG_WARNING,DEMUX,
                        "dataDeMultiplexAndSend: Out of bound RNTI value received\n");
                /* + Coverity 93533 */
                /* + SPR 16766 */
                LOG_MAC_MSG( MAC_UL_PDU_RESERVE_BIT_SET_ID, LOGERROR, MAC_L1_INF,
                        getCurrentTick(), 0,0, demuxNode_p->rcvdRNTI,
                        (rntiInfo_p) ? rntiInfo_p->rntiFlag : 0,
                        0,0,0, __func__, "InvalidRnti" );
                /* - SPR 16766 */
                /* - Coverity 93533 */

                 /*SPR 7100 Fix start*/
                if (demuxNode_p->recvdUlData_p)
                {
                    msgFree(demuxNode_p->recvdUlData_p);
                    demuxNode_p->recvdUlData_p = PNULL;
                }
                freeMemPool((void *)demuxNode_p);
                 /*SPR 7100 Fix end*/
                /* SPR 22376 fix start */
                continue;
                /* SPR 22376 fix end */
            }

            if (TCRNTI != rntiInfo_p->rntiFlag)
            {
                dlUEContextInfo_p = &dlUECtxInfoArr_g[demuxNode_p->ueIdx];
                dlUEContext_p = dlUEContextInfo_p->dlUEContext_p;

                ulUEContextInfo_p = &ulUECtxInfoArr_g[demuxNode_p->ueIdx];
                ulUEContext_p = ulUEContextInfo_p->ulUEContext_p;
            }

            if ((TCRNTI != rntiInfo_p->rntiFlag) &&
                    /* SPR 3043 start */
                    (((dlUEContextInfo_p->pendingDeleteFlag) ||
                      (ulUEContextInfo_p->pendingDeleteFlag))&&
            /*SPR 7100 Fix start*/
                   ((PNULL != dlUEContext_p)&&(dlUEContext_p->tcrntiFlag != TRUE))))
            /*SPR 7100 Fix End*/
                /* SPR 3043 end */
            {
                /* + SPR 16766 */
                LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                        getCurrentTick(), demuxNode_p->ueIdx, demuxNode_p->rcvdRNTI,
                        dlUEContextInfo_p->pendingDeleteFlag,
                        dlUEContext_p->tcrntiFlag, rntiInfo_p->rntiFlag, 0,0,
                        __func__, "msgFreeFlagSet");
                /* - SPR 16766 */
                LTE_MAC_UT_LOG(LOG_WARNING,DEMUX,
                        "[%s]: ERROR pendingDeleteFlag set DL [%d] UL [%d]\n",
                        __func__,dlUEContextInfo_p->pendingDeleteFlag,
                        ulUEContextInfo_p->pendingDeleteFlag);
                msgFreeFlag = TRUE;
            }
            else if ((TCRNTI == rntiInfo_p->rntiFlag) ||
                    ((PNULL != ulUEContext_p) && (PNULL != dlUEContext_p) && 
                     (PNULL != demuxNode_p->recvdUlData_p)))
            {
                retVal = processDemuxData(demuxNode_p->ueIdx,
                        demuxNode_p->rcvdRNTI,
                        demuxNode_p->rcvdTTI,
                        demuxNode_p->recvdUlData_p,
                        demuxNode_p->dataLen,
                        /* +CLPC_CHG_LJA */
                        demuxNode_p->powerCorrectionInfo,
                        /* -CLPC_CHG_LJA */
                        thdIndex,
                        dlDelay,
                        /* +- SPR 17777 */
                        /* SPR 4751 changes */
                        demuxNode_p->harqProcessId,
                        /*CA Changes start  */
                        internalCellIndex);
                        /*CA Changes end  */
                /* SPR 4751 changes */

                if (MAC_FAILURE == retVal)
                {
                    LTE_MAC_UT_LOG(LOG_WARNING,DEMUX,
                            "processDemuxQueue(): ERROR: returned failure\n");
                    /* + SPR 16766 */
                    /* + Coverity 93530 */
                    LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                            getCurrentTick(), demuxNode_p->ueIdx, demuxNode_p->rcvdRNTI,
                            (dlUEContextInfo_p) ? dlUEContextInfo_p->pendingDeleteFlag : 0,
                            (dlUEContext_p) ? dlUEContext_p->tcrntiFlag : 0, rntiInfo_p->rntiFlag, 0,0,
                            __func__, "DemuxQFailed");
                    /* + Coverity 93530 */
                    /* - SPR 16766 */
                }

                if (demuxNode_p->recvdUlData_p)
                {
                    msgFree(demuxNode_p->recvdUlData_p);
                    demuxNode_p->recvdUlData_p = PNULL;
                }

                /*Free the memory*/
                freeMemPool((void *)demuxNode_p);

                demuxNode_p = PNULL;

                continue;
            }
            else
            {
                /* + SPR 16766 */
                /* + Coverity 93532 */
                LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                        getCurrentTick(), demuxNode_p->ueIdx, demuxNode_p->rcvdRNTI,
                        dlUEContextInfo_p->pendingDeleteFlag,
                        (dlUEContext_p) ? dlUEContext_p->tcrntiFlag : 0, rntiInfo_p->rntiFlag, 0,0,
                        __func__, "UECtxNull");
                /* + Coverity 93532 */
                /* - SPR 16766 */
                LTE_MAC_UT_LOG(LOG_WARNING,DEMUX,
                        "[%s]: ERROR UE context is NULL for UE Index [%d] OR\n"
                        "      demuxNode_p->recvdUlData_p is NULL\n",
                        __func__,demuxNode_p->ueIdx);
                msgFreeFlag = TRUE;
            }

            if (TRUE == msgFreeFlag)
            {
                /*Free the memory*/
                if (demuxNode_p->recvdUlData_p)
                {
                    msgFree(demuxNode_p->recvdUlData_p);
                    demuxNode_p->recvdUlData_p = PNULL;
                }

                freeMemPool((void *)demuxNode_p);

                demuxNode_p = PNULL;

            }
#if (defined OVERSHOOT_CORRECTION && (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD)))
        }
        else
        {
            noTimeToProcessflag_g = 1;

            if(*currTickOverLoad_p < OVERLOAD_LEVEL_MEDIUM)
            {
                *currTickOverLoad_p = OVERLOAD_LEVEL_MEDIUM;
            }
            LOG_MAC_MSG(MAC_TICK_LD_CHANGE,LOGWARNING,
                    L2_SYS_FAIL,
                    getCurrentTick(),
                    __LINE__,
                    *currTickOverLoad_p,
                    /** CA2 Changes Start **/
                    macOverLoadLevel_g[internalCellIndex],
                    /** CA2 Changes End **/
                    /*+ Dyn UEs To Be SchPerTTI +*/
                    /** CA-TDD Changes Start **/
                    dlUeSchdIndxInCurrTick_g, ulUeSchdIndxInCurrTick_g, 0, 0 ,__func__,
                    /** CA-TDD Changes Stop **/
                    /*- Dyn UEs To Be SchPerTTI -*/
                    "Overload level changed.");
            return;
        }
#endif
    }

#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD)
    /* + SPR_5711 */ 
    if(!sQueueCount(&demuxThread_p->demuxQueue))
    {
        while(demuxProcessMap_gp[internalCellIndex]->demuxIndex)
        {
            ULUEContextInfo *ulUEContextInfo_p = PNULL;
            ULUEContext     *ulUEContext_p     = PNULL;

/* SPR 11331 Fix Start */
#ifdef FDD_CONFIG
            UInt16 ueIndex                      = demuxProcessMap_gp[internalCellIndex]->\
                                                    ueIdToBeProcessed[demuxProcessMap_gp[internalCellIndex]->demuxIndex - 1];
#else
            UInt16 ueIndex  = demuxProcessMap_gp[internalCellIndex]->ueIdToBeProcessed[demuxProcessMap_gp[internalCellIndex]->demuxIndex - 1].ueIndex;
#if defined(KPI_STATS) ||  defined(PERF_STATS)
            /* SPR 15909 fix start */
            tickType_t rcvdTTI  = demuxProcessMap_gp[internalCellIndex]->ueIdToBeProcessed[demuxProcessMap_gp[internalCellIndex]->demuxIndex - 1].rcvdTTI;
            /* SPR 15909 fix end */
            UInt8 rbForKpiStatsIdx = rcvdTTI % MAX_UL_SF_CONTAINER;
#endif
#endif
/* SPR 11331 Fix End */

            ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];

            if(!ulUEContextInfo_p->pendingDeleteFlag)         
            {
                ulUEContext_p = ulUEContextInfo_p->ulUEContext_p; 
                if((PNULL != ulUEContext_p)&&(TRUE == ulUEContext_p->isDemuxIndexPresent ))
                {

                    for(startHpId = 0; startHpId < MAX_HARQ_PROCESS_NB; startHpId ++)
                    {
                        /* SPR 5127 Start */
                        for(startLcId = 0; startLcId <= ulUEContext_p->maxConfiguredLcId ; startLcId ++)
                        {
                            /* SPR 5127 End */
                            /* CA Changes start */
                            if (UL_QOS_SCHEDULING == ulSchedulerStrategy) 
                            /* CA Changes end */
                            {
                                /* +- SPR 17777 */
                                adjustTokensForLcId(ulUEContext_p, startLcId,
                                        dlQosMappingForTokenAdjust[ueIndex][startHpId][startLcId]
                                        ,startHpId
#if (defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
                                        ,execArgs->globalTick
#endif
                                        );
                                /* +- SPR 17777 */
                                dlQosMappingForTokenAdjust[ueIndex][startHpId][startLcId] = 0;
                            }

#if defined(KPI_STATS) ||  defined(PERF_STATS)
                            /* L2_PERFORMANCE_OPT_CHANGES_START */
                            UInt32           qciSizeInBits     = 0;
                            UInt32           tbSizeInBits      = 0;
                            /* L2_PERFORMANCE_OPT_CHANGES_END */
                            UInt32           numRBs            = 0;
                            UInt32           qciSizePerTB      = 0;
                            if (ulUEContext_p->ulresInfo[startHpId].netlength[startLcId] && 
                                    ulUEContext_p->ulresInfo[startHpId].tbSizeNet)
                            {
                                qciSizeInBits = (ulUEContext_p->ulresInfo[startHpId].netlength[startLcId])<<3;
                                tbSizeInBits  = (ulUEContext_p->ulresInfo[startHpId].tbSizeNet)<<3;

/* SPR 11331 Fix Start */
#ifdef FDD_CONFIG
                                numRBs = ulUEContext_p->ulresInfo[startHpId].rbForKpiStats;
#else
                                numRBs = ulUEContext_p->ulresInfo[startHpId].rbForKpiStats[rbForKpiStatsIdx];
#endif
/* SPR 11331 Fix End */

                                qciSizePerTB = ((qciSizeInBits<<SCALE_UP_MULTIPLYING_FACTOR)/tbSizeInBits);


#ifdef KPI_STATS
                                /* +GCC 4.7 */
                                UInt8 qci = 0;
                                /* -GCC 4.7 */
                                qci = ulUEContext_p->lcInfo[startLcId].ulQosInfo.QCI;
                                /* SPR_8524_FIX_START */
                                if (qci)
                                {     
                                /* SPR_8524_FIX_END */    
									/* + KPI_CA */
                                    gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats].
                                        totalULPRBUsagePerQCI[qci - 1] += (qciSizePerTB*numRBs); /* SPR 14267 Change */
									/* - KPI_CA */
                                    /* SPR 23402 Changes Start */
                                    /* MOCN Changes Start */
                                    if(mocnOperatorParams_g.isMocnEnabled && (ulUEContext_p->operatorId < mocnOperatorParams_g.numOfOperator))
                                    {    
                                        gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats].
                                        totalULPRBUsagePerOperatorPerQCI[ulUEContext_p->operatorId][qci - 1] += (qciSizePerTB*numRBs);
                                    }
                                    /* MOCN Changes End */
                                    /* SPR 23402 Changes End */
                                }
#endif
#ifdef PERF_STATS
                                /* EVENT: EVENT_PARAM_DCCH_CCCH_UL_RES , UL PRB usage for SRB*/
                                if (SRB_1_LCID == startLcId)
                                {
									/* + PERF_CA */
                                    gMacCellPerfStats_p[internalCellIndex]->lteCellMacULPerfStats.totalULPRBUsageSRB[SRB_1_LCID] += 
                                        (qciSizePerTB*numRBs);
                                }
                                if (SRB_2_LCID == startLcId)
                                {
                                    gMacCellPerfStats_p[internalCellIndex]->lteCellMacULPerfStats.totalULPRBUsageSRB[SRB_2_LCID] +=
                                        (qciSizePerTB*numRBs);
									/* - PERF_CA */
                                }
#endif
                                ulUEContext_p->ulresInfo[startHpId].netlength[startLcId] = 0;

                            }
#endif
                            /* - SPR_5711 */ 
                        }
                        /* SPR 9105 */
#if defined(KPI_STATS) ||  defined(PERF_STATS)
                         /* SPR 10411 changes start */
                         if(ulUEContext_p->ulresInfo[startHpId].tbSizeNet)
			 {
                         /* SPR 10411 changes end */
                                ulUEContext_p->ulresInfo[startHpId].tbSizeNet = 0;

/* SPR 11331 Fix Start */
#ifdef FDD_CONFIG
                                ulUEContext_p->ulresInfo[startHpId].rbForKpiStats = 0;
#else
                                ulUEContext_p->ulresInfo[startHpId].rbForKpiStats[rbForKpiStatsIdx] = 0;
#endif
/* SPR 11331 Fix End */

			 }
#endif
                        /* SPR 9105 */
                    }
                    ulUEContext_p->isDemuxIndexPresent = FALSE;
                }
            }
            demuxProcessMap_gp[internalCellIndex]->demuxIndex--;
        }
    }
#endif


    LTE_MAC_UT_LOG(LOG_DETAIL,DEMUX,
            "Exit [%s]: Function\n",__func__);
}
/*****************************************************************************
 * Function Name  : processDemuxData
 * Inputs         : ueIndex  - Index of the UE contect array,
 *                  rcvdRNTI - RNTI received in the UL message,
 *                  rcvdTTI  - In which TTI UL pdu message received,
 *                  recvdUlData_p - Pointer to the PDU received,
 *                  dataLen  - Length of the PDU received,
 *                  powerControlDeltaTFAndLogFactor - deltaTF and 10LogM factor
 *                                                     for this transmission.
 *                  thdIndex - Index of thd,
 *                  dlDelay  - DL delay,
 *                  riLength - length of RI,
 *                  harqProcessId - Process ID of HARQ
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function segregates the data based on LC id and 
 *                  call dataDeMultiplexAndSend() to break data and send to
 *                  RLC/RRC.
 *****************************************************************************/
MacRetType processDemuxData( UInt16  ueIndex,
                             UInt16  rcvdRNTI,
                            /* SPR 15909 fix start */
                             tickType_t  rcvdTTI,
                             /* SPR 15909 fix end */
                             void    *recvdUlData_p,
                             UInt32  dataLen,
                             /* +CLPC_CHG_LJA*/
                             SInt8   powerControlDeltaTFAndLogFactor,
                             /* -CLPC_CHG_LJA*/
                             UInt32  thdIndex,
                             UInt32  dlDelay,
                             /* +- SPR 17777 */
                             /* SPR 4751 changes */
                             UInt8 harqProcessId,
                             /* SPR 4751 changes */
                             /*CA Changes start  */
                             InternalCellIndex internalCellIndex
                             /*CA Changes end  */
                             )
{
    UInt8  *dataPtr_p       = PNULL;
    UInt32 extnFlag         = FALSE;
    UInt32 startPaddingFlag = FALSE;
    UInt32 sucessDecodeFlag = TRUE;
    UInt32 lcId        = 0;
    UInt32 pos         = 0;
    UInt32 dataPtrPos  = 0;
    /* SPR SPR 1401  Start*/
    SInt32 remainingPayloadLength = 0;
    /* SPR  SPR 1401 End */
    /*SPR 2499 Start*/
    UInt32 localLength = 0;
    /*SPR 2499 End*/
    /* SPR 20834 Changes Start */
    DemuxDataBase demuxData[MAX_NUM_OF_SUBHEADER_IN_UL_PDU] = {{0}};
    /* SPR 20834 Changes End */
    DemuxDataBase *demuxData_p = PNULL;
    /* Review comment fix start LJ6 */
    RNTIInfo *rntiInfo_p = PNULL;
    /* Review comment fix end LJ6 */
    /* SPR 11474 changes start */
    UInt16 tempTcRNTI = 0;
    /* SPR 11474 changes end */

#ifdef PERF_STATS
/** CA-TDD Changes Start **/
    /* SPR 15909 fix start */
    static tickType_t localTTI[MAX_NUM_CELL][MAX_UE_SUPPORTED] = {{0}};
    /* SPR 15909 fix end */
    static UInt32 addDataLengthFlag[MAX_NUM_CELL][MAX_UE_SUPPORTED] = {{0}};
/** CA-TDD Changes Stop **/
#endif

#ifdef LOG_PRINT_ENABLED    
#ifdef LOG_UT_ENABLED /* TDD Warning Fix */
    UInt32 loopPosition = 0;
#endif
#ifdef LOG_UT_ENABLED
    /* SPR 15909 fix start */
    tickType_t currentTTI = getCurrentTick();
    /* SPR 15909 fix end */
#endif
#endif  

    void *ptr = PNULL;
    QSEGMENT segment = {0};
    LTE_MAC_UT_LOG(LOG_DETAIL,DEMUX,
            "[Entry] : Function [%s]\n",__func__);

    /*for zero buffer copy*/
    ptr = msgSegNext(recvdUlData_p, 0, &segment);

    if (PNULL == ptr)
    {
        LOG_MAC_MSG(L2_MSG_SEGNEXT_FAIL_ID,LOGFATAL,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);
        /*SPR 16980 Fix Start*/
        msgFree(recvdUlData_p);
        recvdUlData_p = PNULL;
        /*SPR 16980 Fix End*/
        return MAC_FAILURE;
    }
    /*SPR 1768 -Start*/
    else
    {
        /*SPR 1768 -End*/

        dataPtr_p   = segment.base;
    }


    demuxData_p            = &demuxData[0];
    remainingPayloadLength = dataLen;
    /* SPR 3444 changes start */
    if((!dataLen) || (PNULL == dataPtr_p))
    {
        /* + SPR 16766 */
        LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                getCurrentTick(), ueIndex, rcvdRNTI, dataLen,
                0, 0, 0,0, __func__, "");
        /* - SPR 16766 */

        return MAC_FAILURE;
    }
    /* SPR 3444 changes end */

    do 
    {
        /*Reffer section 6.1.2 and 6.2 of 36321-850.doc*/
        /* <------ 1 Octet ------>
         * +++++++++++++++++++++++
         * |R |R |E |   LCID     |
         * +++++++++++++++++++++++
         * R    - Reserved (1 bit)
         * E    - Extention bit (1 bit)
         * LCID - Logical channel id (5 bits)
         */
        /*Subheader with variable length*/
        /*Reffer section 6.1.2 and 6.2 of 36321-850.doc*/
        /* ++++++++++++++++++++++
         * |R|R|E|   LCID       | Oct 1
         * ++++++++++++++++++++++
         * |F|       L          | Oct 2 
         * ++++++++++++++++++++++
         * R    - Reserved (1 bit)
         * E    - Extention bit (1 bit)
         * LCID - Logical channel id (5 bits)
         * L    - Length 
         */
        /* ++++++++++++++++++++++
         * |R|R|E|   LCID       | Oct 1
         * ++++++++++++++++++++++
         * |F|       L          | Oct 2 
         * ++++++++++++++++++++++
         * |         L          | Oct 3
         * ++++++++++++++++++++++
         * R    - Reserved (1 bit)
         * E    - Extention bit (1 bit)
         * LCID - Logical channel id (5 bits)
         * L    - Length 
         */
        /* Review comment fix start LJ6 */
        if(*dataPtr_p & MAC_UL_PDU_RESERVE_BIT_FLAG)
        {
            /*CA Changes start  */
            rntiInfo_p = getUeIdxFromRNTIMap(rcvdRNTI, internalCellIndex);
            /*CA Changes end  */
            if (PNULL == rntiInfo_p)
            {
                LOG_MAC_MSG(MAC_INCORRECT_CRNTI_RECVD_ID,
                        LOGERROR,
                        MAC_L1_INF,
                        getCurrentTick(),
                        (*dataPtr_p & MAC_UL_PDU_RESERVE_BIT1_FLAG),
                        (*dataPtr_p & MAC_UL_PDU_RESERVE_BIT2_FLAG),
                        rcvdRNTI,
                        DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"Out of bound RNTI value received");
                return MAC_FAILURE;
            }
            if (CRNTI == rntiInfo_p->rntiFlag || 
                SPS_RNTI == rntiInfo_p->rntiFlag)
            {
                LOG_MAC_MSG(MAC_UL_PDU_RESERVE_BIT_SET_ID,
                        LOGERROR,
                        MAC_L1_INF,
                        getCurrentTick(),
                        (*dataPtr_p & MAC_UL_PDU_RESERVE_BIT1_FLAG),
                        (*dataPtr_p & MAC_UL_PDU_RESERVE_BIT2_FLAG),
                        rcvdRNTI,
                        rntiInfo_p->rntiFlag,
                        DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"MAC_UL_PDU_RESERVE_BIT_SET");
                return MAC_FAILURE;
            }
        }     
        /* Review comment fix end LJ6 */
        lcId = (*dataPtr_p & 0x1F);

        if (*dataPtr_p & MAC_UL_PDU_EXTN_FLAG)
        {
            extnFlag = TRUE;
        }
        else
        {
            extnFlag = FALSE;
        }


#ifdef LOG_PRINT_ENABLED 
#ifdef LOG_UT_ENABLED
        loopPosition = pos;
#endif
#endif        
        /*SPR 2499 Start*/
        localLength = 0 ;
        /*SPR 2499 End*/
        switch(lcId)
        {
            case MAC_UL_CCCH_LCH:
            {
                /*Cyclomatic Complexity changes - starts here*/
                processMacUlCcchLch(startPaddingFlag,
                        extnFlag,
                        &dataPtrPos,
                        &remainingPayloadLength,
                        &sucessDecodeFlag,
                        &localLength,
                        &dataPtr_p,
                        demuxData_p,
                        &pos,
                        rcvdRNTI);
                /*Cyclomatic Complexity changes - ends here*/
            }
            break;

            case MAC_LCID_1:
            case MAC_LCID_2:
            case MAC_LCID_3:
            case MAC_LCID_4:
            case MAC_LCID_5:
            case MAC_LCID_6:
            case MAC_LCID_7:
            case MAC_LCID_8:
            case MAC_LCID_9:  
            case MAC_LCID_10:
            {
                /*Cyclomatic Complexity changes - starts here*/
                processMacLcid(startPaddingFlag,
                        extnFlag,
                        &dataPtrPos,
                        &remainingPayloadLength,
                        &sucessDecodeFlag,
                        &dataPtr_p,
                        demuxData_p,
                        &pos,
                        lcId,
                        &localLength,
                        ueIndex,
                        dataLen,
                        /* +- SPR 17777 */
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD)
#ifdef TDD_CONFIG
                        rcvdTTI,
#endif
#endif
                        /* +- SPR 17777 */
                        rcvdRNTI
                        /* SPR 11160 Changes Start */
                        /** +- SPR 12364 **/
                        ,harqProcessId
                        /* SPR 11160 Changes End */
                        /*CA Changes start  */
                        ,internalCellIndex);
                        /*CA Changes end  */
                /*Cyclomatic Complexity changes - ends here*/
            }

            break;

            case MAC_POWER_HEAD_LCH:
            {
                /*Cyclomatic Complexity changes - starts here*/
                processMacPowerHeadLch(&remainingPayloadLength,
                        &sucessDecodeFlag,
                        demuxData_p,
                        &pos,
                        ueIndex,
                        &dataPtrPos,
                        /* SPR 9202 Fix Starts */
                        rcvdRNTI,
                        /* SPR 9202 Fix Ends */
                        internalCellIndex);
                /*Cyclomatic Complexity changes - ends here*/
            }
            break;

            case MAC_CRNTI_LCH:
            {
                /*Cyclomatic Complexity changes - starts here*/
                processMacCrntiLch(&remainingPayloadLength,
                        &sucessDecodeFlag,
                        &pos,
                        demuxData_p,
                        &dataPtrPos);
                /*Cyclomatic Complexity changes - ends here*/
            }
            break;

            case MAC_TRUNCATED_BSR:
            case MAC_S_BSR_LCH:
            {
                /*Cyclomatic Complexity changes - starts here*/
                processShortOrTruncatedBsr(&remainingPayloadLength,
                        &sucessDecodeFlag,
                        &pos,
                        demuxData_p,
                        &dataPtrPos,
                        lcId);
                /*Cyclomatic Complexity changes - ends here*/
            }
            break;

            case MAC_L_BSR_LCH:
            {
                /*Cyclomatic Complexity changes - starts here*/
                processMacLongBsrLch(&remainingPayloadLength,
                        &sucessDecodeFlag,
                        &pos,
                        demuxData_p,
                        &dataPtrPos);
                /*Cyclomatic Complexity changes - ends here*/
            } 
            break;

            case MAC_PADDING_LCH:
            {
                /*Cyclomatic Complexity changes - starts here*/
                processMacPaddingLch(extnFlag,
                        &dataPtrPos,
                        &remainingPayloadLength, 
                        &localLength, 
                        &sucessDecodeFlag,
                        pos,
                        demuxData_p,
                        &startPaddingFlag);
                /*Cyclomatic Complexity changes - ends here*/
            }
            break;

            default:
            {
                /* + SPR 16766 */
                LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                        getCurrentTick(), ueIndex, rcvdRNTI, lcId,
                        0, 0, 0,0, __func__, "1");
                /* - SPR 16766 */

                lteWarning("processDemuxData: Invalid lcId=%d\n",lcId);    
                sucessDecodeFlag = FALSE;        
            }
            break;     
        }

        /* SPR SPR 1401  Start*/
        if(remainingPayloadLength < 0)
        {
            sucessDecodeFlag = FALSE;
            /* + SPR 16766 */
            LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                    getCurrentTick(), ueIndex, rcvdRNTI, lcId,
                    remainingPayloadLength, 0, 0,0, __func__, "2");
            /* - SPR 16766 */
            lteWarning("[%s] Invalid Case Remaining payload length [%d]\n", remainingPayloadLength);
        }
        /* SPR SPR 1401  End*/
        dataPtr_p++;

        LOG_UT(MAC_DEMUX_LC_ID,LOGDEBUG,MAC_L1_INF,\
                currentTTI,\
                lcId,\
                (demuxData_p + loopPosition)->length,
                remainingPayloadLength,\
                sucessDecodeFlag,extnFlag,\
                0.0,0.0,__func__,"DEMUX_LC");

    }while ((TRUE == extnFlag) && (TRUE == sucessDecodeFlag));



    /* SPR SPR 1401  Start*/
    if (sucessDecodeFlag)
    {
        /* SPR 4751 changes */
        dataDeMultiplexAndSend(demuxData, recvdUlData_p, dataPtr_p, ueIndex,
                rcvdRNTI, pos, dataPtrPos, rcvdTTI, thdIndex, dlDelay,powerControlDeltaTFAndLogFactor,
                harqProcessId,/* SPR 3486 - Start/END */
                /*CA Changes start  */
                internalCellIndex);
                /*CA Changes end  */
#ifdef PERF_STATS
        UInt32 count = 0;
        if (perfStatsULDelTrigger_g[ueIndex])
        {
            /** CA-TDD Changes Start **/
            localTTI[internalCellIndex][ueIndex] = 0;
            addDataLengthFlag[internalCellIndex][ueIndex] = 0;
            /** CA-TDD Changes Stop **/
            perfStatsULDelTrigger_g[ueIndex] = 0;
        }
        else
        {
            /** CA-TDD Changes Start **/
            localTTI[internalCellIndex][ueIndex]++;
            if(localTTI[internalCellIndex][ueIndex] > 4)
            {
                addDataLengthFlag[internalCellIndex][ueIndex] = 1;
            }
            if (addDataLengthFlag[internalCellIndex][ueIndex])
            /** CA-TDD Changes Stop **/
            {
                for (count = 0; count < pos; count++)
                {
                    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalUlThpPerUeTrunk += demuxData[count].length;
                }
            }
        }
#endif

        /* SPR 4751 changes */
    }
    /* SPR 11474 changes start */
    else
    {
        RNTIInfo *rntiInfo_p = getUeIdxFromRNTIMap(rcvdRNTI, internalCellIndex);
        if (PNULL == rntiInfo_p)
        {
            /* + Coverity 93534 */
            LOG_MAC_MSG( MAC_UL_PDU_RESERVE_BIT_SET_ID, LOGERROR, MAC_L1_INF,
                    getCurrentTick(), 0,0, rcvdRNTI,  0,
                    0,0,0, __func__, "InvalidRnti" );
            /* - Coverity 93534 */
            return MAC_FAILURE;
        } 

        if (TCRNTI == rntiInfo_p->rntiFlag)
        {
            TempCRNTICtx *tempUEContext_p = PNULL;
            {
            tempUEContext_p =
                tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[rntiInfo_p->index].tempCRNTICtx_p;
            }

            if (PNULL == tempUEContext_p)
            {
                LTE_MAC_UT_LOG(LOG_WARNING,DEMUX,
                        "[%s]: Temporary UE context not present\n"\
                        "for the received UL message. Message Ignored\n",__func__);
                /* + SPR 16766 */
                LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                        getCurrentTick(), rntiInfo_p->index, 0, 0, 0,
                        rntiInfo_p->rntiFlag, 0,0, __func__, "tempCtxNull");
                /* - SPR 16766 */
                return MAC_FAILURE;
            }
            tempTcRNTI = tempUEContext_p->tcRNTI;
            deleteTempCRNTICtx(tempTcRNTI, internalCellIndex); 
            /*Coverity 65585 Start*/
            if(MAC_FAILURE == freeRNTI(tempTcRNTI, internalCellIndex))
            {
                lteWarning("Failed to return TCRNTI [%d] to free pool\n", tempTcRNTI);
            }
            /*Coverity 65585 End*/
        }
    }
    /* SPR 11474 changes end */

    /* SPR SPR 1401  End*/
    LTE_MAC_UT_LOG(LOG_DETAIL,DEMUX,
            "[Exit] : Function [%s]\n",__func__);
    return MAC_SUCCESS;
}

/*Cyclomatic Complexity changes - starts here*/
/*****************************************************************************
 * Function Name  : processMacUlCcchLch 
 * Inputs         : startPaddingFlag,
 * 		            extnFlag,
 * 		            dataPtrPos,
 * 		            remainingPayloadLength,
 * 		            sucessDecodeFlag,
 * 		            localLength,
 * 		            dataPtr_p,
 * 		            demuxData_p,
 * 		            pos,
 * 		            rcvdRNTI 
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the MAC_UL_CCCH_LCH req.
 *****************************************************************************/
static  void processMacUlCcchLch(UInt32 startPaddingFlag,
                                       UInt32 extnFlag,
                                       UInt32 *dataPtrPos,
                                       SInt32 *remainingPayloadLength,
                                       UInt32 *sucessDecodeFlag,
                                       UInt32 *localLength,
                                       UInt8  **dataPtr_p,
                                       DemuxDataBase *demuxData_p,
                                       UInt32 *pos,
                                       UInt16  rcvdRNTI)
{
    /* SPR 2446 Start*/
    if (((TRUE == startPaddingFlag) && (FALSE == extnFlag)) ||
            (FALSE == extnFlag))
    {
        /*This is the case with 1/2 byte padding at the start of 
         *PDU or no padding. So the last subheader is of 1byte
         *i.e no length field.
         */
        /* SPR 1564 changes start */ 
        *dataPtrPos += 1;
        /*SPR 2499 Start*/

        if (*remainingPayloadLength <= (SInt64)*dataPtrPos )
        {
            /* + SPR 16766 */
            LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                    getCurrentTick(), 0, rcvdRNTI, 
                    *remainingPayloadLength, *dataPtrPos,
                    0, 0,0, __func__, "55");
            /* - SPR 16766 */

            *sucessDecodeFlag = FALSE;
            return;
        }
        /* SPR 1564 changes end */ 
        *localLength = *remainingPayloadLength - *dataPtrPos;
        /*SPR 2499 End*/
    }
    else if (TRUE == extnFlag)
    {
        /*Increment the pointer to point to length field*/
        (*dataPtr_p)++;
        /*SPR 2499 Start*/
        *dataPtr_p = calculateMACSubHeaderLength(*dataPtr_p, 
                localLength,
                dataPtrPos);
    }
    /* SPR 2446 End*/

    /* SPR 4029 Fix Start */
    if ( (0 == *localLength) || 
            (*remainingPayloadLength < (SInt64)*localLength)|| (*localLength>100) )
    {
        /* + SPR 16766 */
        LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                getCurrentTick(), 0, rcvdRNTI, 
                *remainingPayloadLength, *dataPtrPos,
                0, 0,0, __func__, "66");
        /* - SPR 16766 */
        *sucessDecodeFlag = FALSE;
        return;
    }
    /* SPR 6262 fix end */

    if(*localLength != 0)
    {
        /**SPR_4954_Fix Start **/
        /* SPR 20834 Changes Start */
        if(*pos < MAX_NUM_OF_SUBHEADER_IN_UL_PDU)
        /* SPR 20834 Changes End */
        {
            (demuxData_p + *pos)->lchId = MAC_UL_CCCH_LCH;
            (demuxData_p + *pos)->length =  *localLength;
            /* SPR 5487 Start */
            *pos += 1;
            /* SPR 5487 End */
        }
        *remainingPayloadLength = 
            *remainingPayloadLength -*localLength;
        /**SPR_4954_Fix End **/
    }
    /*SPR 2499 End*/
}  

/*****************************************************************************
 * Function Name  : processMacLcid 
 * Inputs         : startPaddingFlag,
 *                  extnFlag,
 *                  dataPtrPos,
 *                  remainingPayloadLength_p,
 *                  sucessDecodeFlag_p,
 *                  dataPtr_p,
 *                  demuxData_p,
 *                  pos,
 *                  lcId,
 *                  localLength_p,
 *                  ueIndex,
 *                  dataLen,
 *                  rcvdTTI,
 *                  rcvdRNTI,
 *                  harqProcessId,
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the MAC_LCID_1,MAC_LCID_2,MAC_LCID_3,MAC_LCID_4,
 *                  MAC_LCID_5,MAC_LCID_6,MAC_LCID_7,MAC_LCID_8,MAC_LCID_9,MAC_LCID_10.
 *****************************************************************************/
static  void processMacLcid(UInt32 startPaddingFlag,
        UInt32 extnFlag,
        UInt32 *dataPtrPos,
        SInt32 *remainingPayloadLength,
        UInt32 *sucessDecodeFlag,
        UInt8  **dataPtr_p,
        DemuxDataBase *demuxData_p,
        UInt32 *pos,
        UInt32 lcId,
        UInt32 *localLength,
        UInt16  ueIndex,
        UInt32  dataLen,
        /* SPR 15909 fix start */
        /* +- SPR 17777 */
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD)
#ifdef TDD_CONFIG
        tickType_t  rcvdTTI,
#endif
#endif
        /* +- SPR 17777 */
        /* SPR 15909 fix end */
        UInt16  rcvdRNTI
        /* SPR 11160 Changes Start */
        /** +- SPR 12364 **/
        ,UInt8 harqProcessId
        /* SPR 11160 Changes End */
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex) 
        /*CA Changes end  */
{
#if defined( KPI_STATS) || defined(PERF_STATS)
    ULUEContextInfo *ueULContextInfo_p = PNULL;
    ULUEContext     *ueULContext_p     = PNULL;
    UInt8            ulHarqProcessId   = 0;
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD)
    /* L2_PERFORMANCE_OPT_CHANGES_START */
    UInt32           qciSizeInBits     = 0;
    UInt32           tbSizeInBits      = 0;
    /* L2_PERFORMANCE_OPT_CHANGES_END */
    UInt32           numRBs            = 0; 
    UInt32           qciSizePerTB      = 0;
    /* + SPR_5711 */
#ifdef KPI_STATS
    /* +GCC 4.7 */
    UInt8 qci = 0;
    /* -GCC 4.7 */
	/* + KPI_CA */
    UInt32 activeIndexForMacStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
	/* - KPI_CA */
    /* - SPR_5711 */
#endif
#endif 
#endif

    /*<!- SPR_7532_FIX_START -!>*/
    RNTIInfo *rntiInfo_p = NULL;
    UInt8     ctxAccessValidity   = FALSE;

    /*Check this is TCRNTI or CRNTI*/
    /*CA Changes start  */
    rntiInfo_p = getUeIdxFromRNTIMap(rcvdRNTI, internalCellIndex);
    /*CA Changes end  */
    if (PNULL == rntiInfo_p)
    {
        LTE_MAC_UT_LOG(LOG_WARNING,DEMUX,
                "dataDeMultiplexAndSend: Out of bound RNTI value received\n");
        /* + SPR 16766 */
        LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                getCurrentTick(), 0, rcvdRNTI, 
                0, 0, 0, 0,0, __func__, "77");
        /* - SPR 16766 */
        return ;
    }
    if((rntiInfo_p->rntiFlag == CRNTI)||(rntiInfo_p->rntiFlag == SPS_RNTI))
    {
        ctxAccessValidity = TRUE;
    }
    /*<!- SPR_7532_FIX_END -!>*/
#ifdef DEBUG_STATS
    UInt8 loop=0;
    if (ueIndex < MAX_NUM_UE_DEBUG)
    {
        for(loop=0 ; loop < dataLen && loop < MAX_DATA_BYTES;loop++)
        {
            if(MAC_LCID_1 == lcId)
                debugULPDUData_g[ueIndex].strorePduDataLC1[debugULPDUData_g[ueIndex].lc1Idex][loop]= *((*dataPtr_p)+loop);
            else if (MAC_LCID_2 == lcId)
                debugULPDUData_g[ueIndex].strorePduDataLC2[debugULPDUData_g[ueIndex].lc2Idex][loop]= *((*dataPtr_p)+loop);
            else if(MAC_LCID_3 == lcId)
                debugULPDUData_g[ueIndex].strorePduDataLC3[debugULPDUData_g[ueIndex].lc3Idex][loop]= *((*dataPtr_p)+loop);
        }
        if (MAC_LCID_1 == lcId)
        {
            debugULPDUData_g[ueIndex].tick1[debugULPDUData_g[ueIndex].lc1Idex] = getCurrentTick();
            debugULPDUData_g[ueIndex].lc1Idex = (debugULPDUData_g[ueIndex].lc1Idex + 1) % LC_1_MAX_DATA ;
        }
        else if (MAC_LCID_2 == lcId)
        {
            debugULPDUData_g[ueIndex].tick2[debugULPDUData_g[ueIndex].lc2Idex] = getCurrentTick();
            debugULPDUData_g[ueIndex].lc2Idex = (debugULPDUData_g[ueIndex].lc2Idex + 1) % LC_2_MAX_DATA ;
        }
        else if (MAC_LCID_3 == lcId) 
        {
            debugULPDUData_g[ueIndex].tick3[debugULPDUData_g[ueIndex].lc3Idex] = getCurrentTick();
            debugULPDUData_g[ueIndex].lc3Idex = (debugULPDUData_g[ueIndex].lc3Idex + 1) % LC_3_MAX_DATA ;
        }
    }
#endif

    /* SPR 2446 Start*/
    if (((TRUE == startPaddingFlag) && (FALSE == extnFlag)) ||
            (FALSE == extnFlag))
    {
        /*This is the case with 1/2 byte padding at the start of 
         *PDU or no padding. So the last subheader is of 1byte
         *i.e no length field.
         */
        *dataPtrPos += 1;
        /*SPR 2499 Start*/
        if (*remainingPayloadLength <= (SInt64)*dataPtrPos )
        {
            *sucessDecodeFlag = FALSE;
            /* + SPR 16766 */
            LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                    getCurrentTick(), 0, rcvdRNTI, 
                    *remainingPayloadLength, *dataPtrPos, 0, 
                    0,0, __func__, "88");
            /* - SPR 16766 */
            return;
        }

        /* SPR 1564 changes end */
        *localLength = *remainingPayloadLength - *dataPtrPos;
        /*SPR 2499 End*/
    }
    else if (TRUE == extnFlag)
    {
        /*Increment the pointer to point to length field*/
        (*dataPtr_p)++;
        /*SPR 2499 Start*/
        *dataPtr_p = calculateMACSubHeaderLength(*dataPtr_p, 
                localLength, 
                dataPtrPos);
    }
    /* SPR 2446 End*/
    if ((0 == *localLength) ||
            (*remainingPayloadLength < (SInt64)*localLength))
    {
        *sucessDecodeFlag = FALSE;
        /* + SPR 16766 */
        LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                getCurrentTick(), 0, rcvdRNTI, 
                *remainingPayloadLength, *dataPtrPos, *localLength, 
                0,0, __func__, "99");
        /* - SPR 16766 */
        return;
    }

    if(*localLength != 0)
    {
        /* SPR 20834 Changes Start */
        if(*pos < MAX_NUM_OF_SUBHEADER_IN_UL_PDU)
        /* SPR 20834 Changes End */
        {
            (demuxData_p + *pos)->lchId = lcId;
            (demuxData_p + *pos)->length =  *localLength;
            *pos += 1;
            /*SPR 2499 End*/
            /* SPR 5487 Start */
            /*<!- SPR_7532_FIX_START -!>*/
            if(ctxAccessValidity == TRUE)/*Context accessible only for CRNTI and UE which are not in deletion phase*/
            {
            /*<!- SPR_7532_FIX_END -!>*/
#if defined(KPI_STATS) || defined(PERF_STATS)
                ueULContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
                ueULContext_p     = ueULContextInfo_p->ulUEContext_p;
                /*<!- SPR_7532_FIX_START -!>*/
                if(ueULContextInfo_p->pendingDeleteFlag != TRUE)
                {
                /*<!- SPR_7532_FIX_END -!>*/
                    /* SPR 7973 changes start */
                    if( PNULL == ueULContext_p )
                    {
                        *sucessDecodeFlag = FALSE;
                        return;
                    }
                    /* SPR 7973 changes end */

                    /** SPR 12364 fix Starts**/
            /* SPR 11160 Changes Start */
            /*
            ** Use the harq process Id which is present in 
            ** demux node rather than get from the getFreeULHarqId() for next
            ** ulsubframe.
            */
            ulHarqProcessId = harqProcessId;
            /* SPR 11160 Changes End */
                    /** SPR 12364 fix Ends **/
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD)
            /* SPR 5487 Start */
            /* + SPR_5711 */
            qciSizeInBits = *localLength<<3;
            /* SPR 5487 End */
            tbSizeInBits =  dataLen<<3;

/* SPR 11331 Fix Start */
#ifdef FDD_CONFIG
                    /** SPR 12364 fix Starts**/
                    numRBs = ueULContext_p->ulresInfo[ulHarqProcessId].ulSchReqInfo.rbForKpiStats;
                    /** SPR 12364 fix Ends **/
#else
            UInt8 rbForKpiStatsIdx = rcvdTTI % MAX_UL_SF_CONTAINER;
            numRBs = ueULContext_p->ulresInfo[ulHarqProcessId].rbForKpiStats[rbForKpiStatsIdx];
#endif
/* SPR 11331 Fix End */

            qciSizePerTB = ((qciSizeInBits<<SCALE_UP_MULTIPLYING_FACTOR)/tbSizeInBits);
#ifdef KPI_STATS
            qci = ueULContext_p->lcInfo[lcId].ulQosInfo.QCI;
            if (qci)
            {
				/* + KPI_CA */
                gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats].
                    totalULPRBUsagePerQCI[qci - 1] += (qciSizePerTB*numRBs); /* SPR 14267 Change */
				/* + KPI_CA */
                /* SPR 23402 Changes Start */
                /* MOCN Changes Start */
                if(mocnOperatorParams_g.isMocnEnabled && (ueULContext_p->operatorId < mocnOperatorParams_g.numOfOperator))
                {    
                    gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats].
                        totalULPRBUsagePerOperatorPerQCI[ueULContext_p->operatorId][qci - 1] += (qciSizePerTB*numRBs);
                }
                /* MOCN Changes End */
                /* SPR 23402 Changes End */
            }
#endif
#ifdef PERF_STATS
            /* EVENT: EVENT_PARAM_DCCH_CCCH_UL_RES , UL PRB usage for SRB*/
            if (SRB_1_LCID == lcId)
            {
				/* + PERF_CA */
                gMacCellPerfStats_p[internalCellIndex]->lteCellMacULPerfStats.totalULPRBUsageSRB[SRB_1_LCID] +=
                    (qciSizePerTB*numRBs);
            }
            if (SRB_2_LCID == lcId)
            {
                gMacCellPerfStats_p[internalCellIndex]->lteCellMacULPerfStats.totalULPRBUsageSRB[SRB_2_LCID] +=
                    (qciSizePerTB*numRBs);
				/* - PERF_CA */
            }
            /* - SPR_5711 */
#endif
            /* SPR 5487 End */
#else
            /* SPR 5675 fix starts*/
            ueULContext_p->ulresInfo[ulHarqProcessId].netlength[lcId] += *localLength;
            /* SPR 5675 fix ends*/
            /* SPR 11331 Fix Start */
            ueULContext_p->ulresInfo[ulHarqProcessId].tbSizeNet = dataLen;
            /* SPR 11331 Fix End */
#endif
                    /*<!- SPR_7532_FIX_START -!>*/
                }
#endif
            }
            /*<!- SPR_7532_FIX_END -!>*/
        }
        *remainingPayloadLength =
            *remainingPayloadLength - *localLength;
        /**SPR_4954_Fix End **/
        /* SPR 5487 End */
    }
}

/*****************************************************************************
 * Function Name  : processMacPowerHeadLch 
 * Inputs         : remainingPayloadLength,
 *                  sucessDecodeFlag,
 *                  demuxData_p,
 *                  pos,
 *                  ueIndex,
 *                  dataPtrPos,
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the MAC_POWER_HEAD_LCH.
 *****************************************************************************/
static  void processMacPowerHeadLch(SInt32 *remainingPayloadLength,
                                          UInt32 *sucessDecodeFlag,
                                          DemuxDataBase *demuxData_p,
                                          UInt32 *pos,
                                          UInt16 ueIndex,
                                          UInt32 *dataPtrPos,
                                          /* SPR 9202 Fix Starts */
                                          UInt16  rcvdRNTI,
                                          /* SPR 9202 Fix Ends */
                                          InternalCellIndex internalCellIndex)
{
/*<!- SPR_7532_FIX_START -!>*/
    /* SPR 9202 Fix Starts*/
    ULUEContextInfo *ueULContextInfo_p = PNULL;
    ULUEContext     *ueULContext_p     = PNULL;
    /* SPR 9202 Fix Starts*/
    /*SPR 2499 Start*/
    if(*remainingPayloadLength < POWER_HEADROOM_MAC_CE_LENGTH)
    {
        *sucessDecodeFlag = FALSE;
        /* + SPR 16766 */
        LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                getCurrentTick(), ueIndex, rcvdRNTI, 
                *remainingPayloadLength, 0, 0, 
                0,0, __func__, "100");
        /* - SPR 16766 */
        return;
    }
    /*SPR 2499 End*/
    /* SPR 5487 Start */
    /* SPR 20834 Changes Start */
    if(*pos < MAX_NUM_OF_SUBHEADER_IN_UL_PDU)
    {
    /* SPR 20834 Changes End */
        (demuxData_p + *pos)->lchId   = MAC_POWER_HEAD_LCH;
        (demuxData_p + *pos)->length  = POWER_HEADROOM_MAC_CE_LENGTH;
        *pos += 1;
    }
    /* SPR 5487 End */
    *remainingPayloadLength = 
        *remainingPayloadLength - POWER_HEADROOM_MAC_CE_LENGTH;
    /* SPR 9202 Fix Starts*/
    RNTIInfo          *rntiInfo_p        = PNULL;  
    rntiInfo_p = getUeIdxFromRNTIMap(rcvdRNTI, internalCellIndex);
    if (PNULL == rntiInfo_p )
    {
        /* No RNTI context rnti */
        *dataPtrPos += 1;
        /* + SPR 16766 */
        LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                getCurrentTick(), ueIndex, rcvdRNTI, 
                0, 0, 0, 0,0, __func__, "101");
        /* - SPR 16766 */
        return;

    }
    else if(TCRNTI == rntiInfo_p->rntiFlag)
    {
        *dataPtrPos += 1;
        /* + SPR 16766 */
        LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                getCurrentTick(), ueIndex, rcvdRNTI, 
                0, 0, 0, 0,0, __func__, "102");
        /* - SPR 16766 */
        return;
    } 
    ueULContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
    ueULContext_p     = ueULContextInfo_p->ulUEContext_p;
    if( (ueULContextInfo_p->pendingDeleteFlag) || (ueULContext_p ==PNULL) )
    {
        *dataPtrPos += 1;
        /* + SPR 16766 */
        LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                getCurrentTick(), ueIndex, rcvdRNTI, 
                0, 0, 0, 0,0, __func__, "103");
        /* - SPR 16766 */
        return;

    }		

    /* SPR 9202 Fix Ends*/
    *dataPtrPos += 1;
}

/*****************************************************************************
 * Function Name  : processMacCrntiLch 
 * Inputs           remainingPayloadLength,
 *                  sucessDecodeFlag,
 *                  pos,
 *                  demuxData_p,
 *                  dataPtrPos,
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the MAC_CRNTI_LCH.
 *****************************************************************************/
static  void processMacCrntiLch(SInt32 *remainingPayloadLength,
                                      UInt32 *sucessDecodeFlag,
                                      UInt32 *pos,
                                      DemuxDataBase *demuxData_p,
                                      UInt32 *dataPtrPos)
{
    /*SPR 2499 Start*/
    if(*remainingPayloadLength < CRNTI_MAC_CE_LENGTH)
    {
        *sucessDecodeFlag = FALSE;
        /* + SPR 16766 */
        LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                getCurrentTick(), 0, 0, 
                0, 0, 0, 0,0, __func__, "104");
        /* - SPR 16766 */
        return;
    }
    /*SPR 2499 End*/
    /* SPR 5487 Start */
    /* SPR 20834 Changes Start */
    if(*pos < MAX_NUM_OF_SUBHEADER_IN_UL_PDU)
    /* SPR 20834 Changes End */
    {
        (demuxData_p + *pos)->lchId = MAC_CRNTI_LCH;
        (demuxData_p + *pos)->length = CRNTI_MAC_CE_LENGTH;
        *pos += 1;
    }
    /* SPR 5487 End */
    *remainingPayloadLength = 
        *remainingPayloadLength - CRNTI_MAC_CE_LENGTH;
    *dataPtrPos += 1;
}

/*****************************************************************************
 * Function Name  : processShortOrTruncatedBsr 
 * Inputs         : remainingPayloadLength,
 *                  sucessDecodeFlag,
 *                  pos,
 *                  demuxData_p,
 *                  dataPtrPos,
 *                  lcId 
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the MAC_TRUNCATED_BSR and MAC_S_BSR_LC.
 *****************************************************************************/
static  void processShortOrTruncatedBsr(SInt32 *remainingPayloadLength,
                                              UInt32 *sucessDecodeFlag,
                                              UInt32 *pos,
                                              DemuxDataBase *demuxData_p,
                                              UInt32 *dataPtrPos,
                                              UInt32 lcId)
{
    /*SPR 2499 Start*/
    if(*remainingPayloadLength < SHORT_BSR_MAC_CE_LENGTH)
    {
        *sucessDecodeFlag = FALSE;
        /* + SPR 16766 */
        LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                getCurrentTick(), 0, 0, 
                0, 0, 0, 0,0, __func__, "105");
        /* - SPR 16766 */
        return;
    }
    /*SPR 2499 End*/
    /* SPR 5487 Start */
    /* SPR 20834 Changes Start */
    if(*pos < MAX_NUM_OF_SUBHEADER_IN_UL_PDU)
    /* SPR 20834 Changes End */
    {
        (demuxData_p + *pos)->lchId  = lcId;
        (demuxData_p + *pos)->length = SHORT_BSR_MAC_CE_LENGTH;
        *pos += 1;                    
    }
    /* SPR 5487 End */
    *remainingPayloadLength = 
        *remainingPayloadLength - SHORT_BSR_MAC_CE_LENGTH;
    *dataPtrPos += 1;
}

/*****************************************************************************
 * Function Name  : processMacLongBsrLch 
 * Inputs         : remainingPayloadLength,
 *                  sucessDecodeFlag,
 *                  pos,
 *                  demuxData_p,
 *                  dataPtrPos 
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the MAC_L_BSR_LCH.
 *****************************************************************************/
static  void processMacLongBsrLch(SInt32 *remainingPayloadLength,
                                        UInt32 *sucessDecodeFlag,
                                        UInt32 *pos,
                                        DemuxDataBase *demuxData_p,
                                        UInt32 *dataPtrPos)
{
    /*SPR 2499 Start*/
    if(*remainingPayloadLength < LONG_BSR_MAC_CE_LENGTH)
    {
        /* + SPR 16766 */
        LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                getCurrentTick(), 0, 0, 
                0, 0, 0, 0,0, __func__, "106");
        /* - SPR 16766 */
        *sucessDecodeFlag = FALSE;
        return;
    }
    /*SPR 2499 End*/
    /* SPR 5487 Start */
    /* SPR 20834 Changes Start */
    if(*pos < MAX_NUM_OF_SUBHEADER_IN_UL_PDU)
    /* SPR 20834 Changes End */
    {
        (demuxData_p + *pos)->lchId = MAC_L_BSR_LCH;
        /*Large BSR is of 3 bytes*/
        (demuxData_p + *pos)->length = LONG_BSR_MAC_CE_LENGTH;
        *pos += 1;                    
    }
    /* SPR 5487 End */
    *remainingPayloadLength = 
        *remainingPayloadLength - LONG_BSR_MAC_CE_LENGTH;
    *dataPtrPos += 1;
}

/*****************************************************************************
 * Function Name  : processMacPaddingLch 
 * Inputs         : extnFlag
 *                  dataPtrPos,
 *                  remainingPayloadLength
 *                  localLength,
 *                  sucessDecodeFlag,
 *                  pos,
 *                  demuxData_p,
 *                  startPaddingFlag
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the MAC_PADDING_LCH.
 *****************************************************************************/
static  void processMacPaddingLch(UInt32 extnFlag,
                                        UInt32 *dataPtrPos,
                                        SInt32 *remainingPayloadLength, 
                                        UInt32 *localLength, 
                                        UInt32 *sucessDecodeFlag,
                                        UInt32 pos,
                                        DemuxDataBase *demuxData_p,
                                        UInt32 *startPaddingFlag)
{
    if (FALSE == extnFlag)
    {
        *dataPtrPos += 1;
        /*case where padding is of 1 byte and present 
         *after the last MAC PDU subheader
         */
        if(*remainingPayloadLength > (SInt64)*dataPtrPos)
        {
            /*SPR 2499 Start*/
            *localLength = *remainingPayloadLength - *dataPtrPos ;
        }
        else
        {
            *localLength = 
                *dataPtrPos - *remainingPayloadLength;
        }
        if(*remainingPayloadLength >= (SInt64)*localLength)
        {
            *remainingPayloadLength = 
                *remainingPayloadLength - *localLength;

        }
        else
        {
            *sucessDecodeFlag = FALSE;
            /* + SPR 16766 */
            LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                    getCurrentTick(), 0, 0, 
                    0, 0, 0, 0,0, __func__, "107");
            /* - SPR 16766 */
            return;
        }
        /*SPR 2499 End*/
        /**SPR_4954_Fix Start **/
        /* SPR 20834 Changes Start */
        if(pos < MAX_NUM_OF_SUBHEADER_IN_UL_PDU)
        /* SPR 20834 Changes End */
        {
            (demuxData_p + pos)->length = 0;
        }
        /**SPR_4954_Fix End **/
    }
    else
    {
        *startPaddingFlag = TRUE;
        *dataPtrPos += 1;
    }
}
/*Cyclomatic Complexity changes - ends here*/

/*****************************************************************************
 * Function Name  : putDataInDemuxQueue
 * Inputs         : ueIdx - Index corresponds to array of UE context,
 *                  rcvdRNTI - received RNTI value in UL message,
 *                  rcvdTTI  - TTI in which UL message received,
 *                  recvdUlData_p - Pointer to the receved UL data,
 *                  dataLen - Length of the received data,
 *                  powerControlDeltaTFAndLogFactor,
 *                  riLength,
 *                  harqProcessId
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This function will distribute received data in round robin 
 *                  fashion among the De-multiplexer thread queue pool.
 *****************************************************************************/
MacRetType putDataInDemuxQueue(UInt16  ueIdx,
                               UInt16  rcvdRNTI,
                               /* SPR 15909 fix start */ 
                               tickType_t  rcvdTTI,
                               /* SPR 15909 fix end */
                               UInt8   *recvdUlData_p,
                               UInt32  dataLen,
                               /* +CLPC_CHG_LJA*/
                               SInt8   powerControlDeltaTFAndLogFactor,
                               /* -CLPC_CHG_LJA*/
                               /*CLPC_CHG_RILEN*/
                               UInt8 riLength,
                               /*CLPC_CHG_RILEN*/
/* SPR 4751 changes */
                               UInt8 harqProcessId,
/* SPR 4751 changes */
/*CA Changes start  */
                               InternalCellIndex internalCellIndex
/*CA Changes end  */
                               )
{
    DataQueueInfoNode *dataQueueNode_p = PNULL;
    /** CA-TDD Changes Start **/
    static UInt32 demuxIndex[MAX_NUM_CELL] = {0};
    /** CA-TDD Changes Stop **/
    UInt32        demuxId    = 0;
    
    LTE_MAC_UT_LOG(LOG_DETAIL,DEMUX,
            "[Entry] : Function [%s]\n",__func__);

    /** CA-TDD Changes Start **/
    demuxId    = demuxIndex[internalCellIndex];
    /*Increment by one to point to next thread*/
    demuxIndex[internalCellIndex] = (demuxIndex[internalCellIndex] + 1) % NUM_OF_DEMUX_THREADS;
    /** CA-TDD Changes Stop **/
    GET_MEM_FROM_POOL(DataQueueInfoNode,dataQueueNode_p,sizeof(DataQueueInfoNode), PNULL);

    /* Rel 5.3: Coverity 24419 Fix Start */
    if (PNULL == dataQueueNode_p)
    {
        /* Not able to allocate Memory */
        return MAC_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */

    dataQueueNode_p->ueIdx         = ueIdx;
    dataQueueNode_p->rcvdRNTI      = rcvdRNTI;
    dataQueueNode_p->rcvdTTI       = rcvdTTI;
    dataQueueNode_p->dataLen       = dataLen;
    /* +CLPC_CHG_LJA*/
    dataQueueNode_p->powerCorrectionInfo  = powerControlDeltaTFAndLogFactor;
    /* -CLPC_CHG_LJA*/
    /*CLPC_CHG_RILEN*/
    dataQueueNode_p->riLength = riLength;
    /*CLPC_CHG_RILEN*/
/* SPR 4751 changes */
    dataQueueNode_p->harqProcessId = harqProcessId;
/* SPR 4751 changes */
    dataQueueNode_p->recvdUlData_p = recvdUlData_p;
    /*push node into demux thread queue*/
    /*CA Changes start  */
    /* Coverity CID 25404 FIX start */
    if ( pushNode(&(deMuxThreadPool_gp[internalCellIndex] + demuxId)->demuxQueue,
             &(dataQueueNode_p->dataQueueInfoNodeAnchor)) )
    {
    /* + SPR 16766 */
        LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                getCurrentTick(), ueIdx, rcvdRNTI, 
                0, 0, 0, 0,0, __func__, "PushFail");
    /* - SPR 16766 */
        lteWarning ("pushNode() failed for deMuxThreadPool");
        freeMemPool(dataQueueNode_p);
        return MAC_FAILURE;
    }
    /* Coverity CID 25404 FIX end */
    /*CA Changes end  */

    LOG_UT(MAC_DEMUX_QUEUE_INS_ID,LOGDEBUG,MAC_L1_INF,\
            getCurrentTick(),\
            ueIdx,rcvdRNTI,\
            rcvdTTI,dataLen,demuxId,\
            0.0,0.0,__func__,"DEMUX_QUEUE_INS");

    return MAC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : adjustTokensForLcId
 * Inputs         : ulCntxt_p,
 *                  lchId,
 *                  length,
 *                  ulHarqProcessId
 * Outputs        : None
 * Returns        : None
 * Description    : If we receive more data than we scheduled for a LC, it will
 *                  be added to its credit tokens.
 *                  If we receive data for a LC that we hadnt scheduled,it will
 *                  be added to its credit tokens.
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129112) */
void adjustTokensForLcId(
/* SPR 5599 changes end (ZIP ID 129112) */
        ULUEContext * ulCntxt_p, 
        UInt8   lchId, 
        UInt32  length,
        /* +- SPR 17777 */
        UInt8   ulHarqProcessId
        /* SPR 15909 fix start */
#if (defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
        ,tickType_t currentGlobalTick
#endif
        )
        /* +- SPR 17777 */
        /* SPR 15909 fix end */
{

    ULLogicalChannelInfo *lcInfo_p = PNULL;
    
    UInt8            harqLc            = 0;
    SInt32           *availToken_p      = PNULL;
    SInt32           *creditToken_p      = PNULL;
    lcInfo_p = &ulCntxt_p->lcInfo[lchId];
    /** SPR 12364 fix Starts**/
    ULTokenInfo * ulTokenInfo_p = &(ulCntxt_p->ulresInfo[ulHarqProcessId].ulSchReqInfo.ulTokenInfo[0]);   
    /** SPR 12364 fix Ends **/
    SInt32 extraData = 0;
    SInt32 extraScheduled = 0;

    if (NON_GBR == lcInfo_p->ulQosInfo.channelType)
    {
        /* This is because all info for non gbr lcs are kept 
         * at the same location/index in ulTokenInfo within ulHarqInfo
         */
        harqLc = ulCntxt_p->ulAmbrInfo.lcId;
        availToken_p = &ulCntxt_p->ulAmbrInfo.availAmbrToken;
        creditToken_p = &ulCntxt_p->ulAmbrInfo.creditToken;
    }
    else
    {
        harqLc = lchId;
        /* QOS_CHG*/
        availToken_p  =(SInt32 *)&lcInfo_p->ulQosInfo.availLcToken;
        creditToken_p =(SInt32 *)&lcInfo_p->ulQosInfo.creditToken;
        /* QOS_CHG*/
    }

    if(ulTokenInfo_p[harqLc].isLCScheduled)
    {
        /*TDD Tick overshoot changes start*/
#if (defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
/*TDD Tick overshoot changes start*/       
#ifdef FDD_CONFIG
        if((1 == ulTokenInfo_p[harqLc].isLCScheduled) && 
                currentGlobalTick >=  (ulCntxt_p->ulresInfo[ulHarqProcessId].lastScheduledTTI + PHY_DELAY + FDD_HARQ_OFFSET + 1))
        {
#else
        if((1 == ulTokenInfo_p[harqLc].isLCScheduled) && 
		/* SPR 11115 Fix Start */
                (currentGlobalTick) >=  (ulCntxt_p->ulresInfo[ulHarqProcessId].ulSchTTI + PHY_DELAY))
		/* SPR 11115 Fix End */
        {
#endif
/*TDD Tick overshoot changes end*/       
            extraData = (length - 
                    ulTokenInfo_p[harqLc].scheduledToken );
            ulTokenInfo_p[harqLc].isLCScheduled = 0;

        }
        else if(ulTokenInfo_p[harqLc].isLCScheduled > 1)
        {
/*TDD Tick overshoot changes start*/       
#ifdef FDD_CONFIG
            if(currentGlobalTick >=  (ulCntxt_p->ulresInfo[ulHarqProcessId].lastScheduledTTI + PHY_DELAY + FDD_HARQ_OFFSET + 1))
#else
            if(currentGlobalTick >=  (ulCntxt_p->ulresInfo[ulHarqProcessId].ulSchTTI + 1))
#endif
/*TDD Tick overshoot changes end*/       
            {
                extraData = (length -
                        ulTokenInfo_p[harqLc].scheduledToken );
                ulTokenInfo_p[harqLc].isLCScheduled = 0;
            }
            else
            {
                extraData = (length -  
                        (ulTokenInfo_p[harqLc].scheduledToken - ulTokenInfo_p[harqLc].lastScheduledToken) );
                ulTokenInfo_p[harqLc].isLCScheduled = 1;
            }
        }
        else
        {
            return;
        }

#else
        extraData = (length - 
                ulTokenInfo_p[harqLc].scheduledToken );

#endif
        extraScheduled = -extraData;
        /* Since multiple NON GBR LCs map to the same lchid
         * hence we cannot set the flag to false until all 
         * non gbr lcs are received
         */
        if (NON_GBR != lcInfo_p->ulQosInfo.channelType)
        {
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD)
            ulTokenInfo_p[harqLc].isLCScheduled = FALSE;

#endif

            /* 
             * First adjust available tokens by the difference 
             * between scheduled and length received
             * Note: available tokens had already been reduced 
             * by the amt of scheduled tokens at strategy/pusch
             */
            if (extraScheduled > 0)
            {
                (*availToken_p) += extraScheduled; 
            }
            /* 
             * in case extraScheduled < 0, credit tokens 
             * will have been added.
             * These will then be adjusted in available tokens 
             * during token updation, hence not doing here
             */
        }

/* SPR 5127 Start */
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD)
        if ( NON_GBR == lcInfo_p->ulQosInfo.channelType )
        {
            if (extraScheduled > 0)
            {
                (*availToken_p) += extraScheduled;
            }
        }

#endif
/* SPR 5127 End */


        /* 
         * Now adjust scheduled tokens and credit tokens
         * 
         * Note: adjusting scheduled tokens needs to
         * be done for NON GBR LCs, 
         * but it doesnt hurt if we do it for GBR LCs also
         */
        if (extraData <= 0)//When data received is <= scheduled
        {
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD) 
            ulTokenInfo_p[harqLc].scheduledToken = abs_wrapper(extraData);
#else
            if(ulTokenInfo_p[harqLc].isLCScheduled)
            {
               /* SPR 4626 fix start */
                ulTokenInfo_p[harqLc].scheduledToken =  ulTokenInfo_p[harqLc].lastScheduledToken;
               /* SPR 4626 fix end */
            }
            else
            {
                ulTokenInfo_p[harqLc].scheduledToken = 0;
            }
#endif
        }
        else
        {
            (*creditToken_p) += extraData;
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD) 
            ulTokenInfo_p[harqLc].scheduledToken = 0;
#else
            if(ulTokenInfo_p[harqLc].isLCScheduled)
            {            
                ulTokenInfo_p[harqLc].scheduledToken =  ulTokenInfo_p[harqLc].lastScheduledToken;
            }
            else
            {
                ulTokenInfo_p[harqLc].scheduledToken = 0;
            }
#endif
        }
    }
    else /* When we hadn't scheduled the LC any recvd data is extra data*/
    { 
        (*creditToken_p) += length;
    }

}


/*****************************************************************************
 * Function Name  : restoreTokensOfNonTransmittedLCs
 * Inputs         : ulCntxt_p,
 *                  ulHarqProcessId
 * Outputs        : None
 * Returns        : None
 * Description    : This function will distribute received data in round robin 
 *                  fashion among the De-multiplexer thread queue pool
 *****************************************************************************/
/* SPR 5599 changes start (ZIP ID 129112) */
void restoreTokensOfNonTransmittedLCs(
/* SPR 5599 changes end (ZIP ID 129112) */
        ULUEContext * ulCntxt_p, 
        UInt8   ulHarqProcessId)
{
    UInt8   counter = 0; 
    ULLogicalChannelInfo *lcInfo_p = PNULL;
    UInt8 alreadyConsideredAMBR = FALSE;
    /** SPR 12364 fix Starts**/
    ULTokenInfo * ulTokenInfo_p = &(ulCntxt_p->ulresInfo[ulHarqProcessId].ulSchReqInfo.ulTokenInfo[0]);  
    /** SPR 12364 fix Ends **/
    for(counter = MAC_LCID_3; counter < MAX_LOGICAL_CHANNEL;  ++counter)
    {
        /* In case of NON GBR LCs
         * even if we have received data for non gbr LC in ulsch
         * the isLCScheduled will not have been set to false in 
         * the adjustTokensForLcId() func.
         * 
         * Since scheduled tokens for transmitted non gbr lcs 
         * would have been adjusted, any remaining scheduled tokens 
         * and corresponding credit tokens will be updated here.
         */
        if(ulTokenInfo_p[counter].isLCScheduled)
        {
            ulTokenInfo_p[counter].isLCScheduled = FALSE;

            lcInfo_p = &ulCntxt_p->lcInfo[counter];

            if (NON_GBR == lcInfo_p->ulQosInfo.channelType && !alreadyConsideredAMBR)
            {
                /* We need the alreadyConsideredAMBR check coz
                 * multiple non gbr LC data is stored at the same index
                 * in the harqContext's ulTokenInfo array
                 */
                alreadyConsideredAMBR = TRUE;
                ulCntxt_p->ulAmbrInfo.availAmbrToken +=  
                    ulTokenInfo_p[ulCntxt_p->ulAmbrInfo.lcId].scheduledToken;
                ulTokenInfo_p[ulCntxt_p->ulAmbrInfo.lcId].scheduledToken = 0;
            }
            else if (NON_GBR != lcInfo_p->ulQosInfo.channelType)
            {
                lcInfo_p->ulQosInfo.availLcToken +=  
                    ulTokenInfo_p[counter].scheduledToken;
                ulTokenInfo_p[counter].scheduledToken = 0;
            }

        }
    }
}

/*Qload adjustment changes start*/
/*****************************************************************************
 * Function Name  : ulQosAdjustQload
 * Inputs         : ulCntxt_p,
 *                  bsrReport_p,
 *                  shortBSRFlag,
 *                  shortBSRLcgId,
 *                  ulHarqProcessId,
 *                  internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function will distribute received data in round robin 
 *                  fashion among the De-multiplexer thread queue pool.
 *****************************************************************************/

void ulQosAdjustQload(ULUEContext *ulCntxt_p,
                      UInt32      * bsrReport_p,
                      UInt32        shortBSRFlag,
                      UInt32        shortBSRLcgId,
                      UInt8         ulHarqProcessId,
                      /*CA Changes start  */
                      UInt8         internalCellIndex)
                      /*CA Changes end  */
{
    UInt8  lcgIdVal = 0;
    UInt32 prevLcgBsr = 0;
    /** SPR 12364 fix Starts**/
    reqHarqInfo* ulHarqInfo_p = &(ulCntxt_p->ulresInfo[ulHarqProcessId].ulSchReqInfo);  
    /** SPR 12364 fix Ends **/
    for (lcgIdVal = 0; lcgIdVal < NUM_OF_LCG; lcgIdVal++) 
    {
        prevLcgBsr = 0;
        if ((shortBSRFlag) && (lcgIdVal == shortBSRLcgId))
        {
            /** SPR 12364 fix Starts**/
            /* Need to check current harq status because if it was it's last UL
             * allocation, then we need to set grantPerLcg to Zero. */
            if (ulCntxt_p->ulresInfo[ulHarqProcessId].harqStatus != HARQ_PROCESS_IN_USE)
            {
                ulCntxt_p->ulresInfo[ulHarqProcessId].grantPerLcg[lcgIdVal] = 0;
            } 
            /** SPR 12364 fix Ends **/
            continue;
        }

        if ((*(bsrReport_p + lcgIdVal)) ||(ulHarqInfo_p->grantPerLcg[lcgIdVal]))
        {
            prevLcgBsr = ulCntxt_p->bsrNet[lcgIdVal];
            if (ulHarqInfo_p->grantPerLcg[lcgIdVal] > *(bsrReport_p + lcgIdVal))
            {
                ulCntxt_p->bsrNet[lcgIdVal] +=
                    (ulHarqInfo_p->grantPerLcg[lcgIdVal] - *(bsrReport_p + lcgIdVal));
                /** SPR 12364 fix Starts**/
                if (ulCntxt_p->ulresInfo[ulHarqProcessId].harqStatus != HARQ_PROCESS_IN_USE)
                {
                    ulCntxt_p->ulresInfo[ulHarqProcessId].grantPerLcg[lcgIdVal] = 0;
                }  
                /** SPR 12364 fix Ends **/
            }

            if((0 == prevLcgBsr) && 0 != ulCntxt_p->bsrNet[lcgIdVal] )
            {
                /*CA Changes start  */
                putEntryInBSRQueue(ulCntxt_p->ueIndex,lcgIdVal,0,ADD_TRIGGER, internalCellIndex);
                /*CA Changes end  */
                /* +- SPR 17404 */
            }
        }

    }
} 
/*Qload adjustment changes end*/

/*****************************************************************************
 * Function Name  : dataDeMultiplexAndSend
 * Inputs         : demuxData_p - Pointer contains the LCID and length of data 
 *                                received per lcid, in the sequence received in UL pdu.
 *                  origDataPtr_p - Points to UL pdu received (including header)
 *                  dataPtr_p - Points to data section in UL pdu (no header)
 *                  ueIndex - Index corresponds to array of UE context
 *                  rcvdRNTI - received RNTI value in UL message
 *                  pos - contains the number of lcid data received in UL pdu
 *                  dataPtrPos - Position of the dataPtr_p position
 *                  ttiCounter - TTI in which UL message received
 *                  thdIndex - Thread index
 *                  dlDelay - DL delay
 *                  powerControlDeltaTFAndLogFactor,
 *                  harqProcessId
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function demultiplex the data received from PHY and
 *                  send to RLC or RRC based on the LC ID data. It also fill
 *                  container's BSR queue and update the UL context based 
 *                  on the received BSR.
 *****************************************************************************/
static void dataDeMultiplexAndSend(DemuxDataBase *demuxData_p,
                                   UInt8   *origDataPtr_p,
                                   UInt8   *dataPtr_p,
                                   UInt16  ueIndex,
                                   UInt16  recvdRNTI,
                                   UInt32  pos,
                                   UInt32  dataPtrPos,
                                   /* SPR 15909 fix start */
                                   tickType_t  ttiCounter,
                                   /* SPR 15909 fix end */
                                   UInt32  thdIndex,
                                   UInt32  dlDelay,
                                   /* +CLPC_CHG_LJA*/
                                   SInt8   powerControlDeltaTFAndLogFactor,
                                   /* -CLPC_CHG_LJA*/ /* SPR 3486 - Start/END */
/* SPR 4751 changes */
                                   UInt8 harqProcessId,
/* SPR 4751 changes */
/*CA Changes start  */
                                   InternalCellIndex internalCellIndex
/*CA Changes end  */
                                   )
{
    /*UL Force Grant Changes Start */
    CellConfigParams *cellParams_p = cellSpecificParams_g.
        cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /*UL Force Grant Changes End */
    RNTIInfo *rntiInfo_p = NULL;
    UInt8   ulHarqProcessId   = 0;

    LTE_MAC_UT_LOG(LOG_DETAIL,DEMUX,
        "[Entry] : Function [%s]\n",__func__);

    /*Check this is TCRNTI or CRNTI*/
    /*CA Changes start  */
    rntiInfo_p = getUeIdxFromRNTIMap(recvdRNTI, internalCellIndex);
    /*CA Changes end  */
    if (PNULL == rntiInfo_p)
    {
        LTE_MAC_UT_LOG(LOG_WARNING,DEMUX,
                "dataDeMultiplexAndSend: Out of bound RNTI value received\n");
        /* + SPR 16766 */
        LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                getCurrentTick(), ueIndex, recvdRNTI, 
                0, 0, 0, 0,0, __func__, "InvalidRnti");
        /* - SPR 16766 */
        return;
    }

    if (TCRNTI == rntiInfo_p->rntiFlag)
    {
        UInt32 retVal = MAC_FAILURE;
        {
        TempCRNTICtx *tempUEContext_p = PNULL;
        tempUEContext_p = 
            tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[rntiInfo_p->index].tempCRNTICtx_p;
        
        if (PNULL == tempUEContext_p)
        {
            LTE_MAC_UT_LOG(LOG_WARNING,DEMUX,
                    "[%s]: Temporary UE context not present\n"\
                    "for the received UL message. Message Ignored\n",__func__);
            /* + SPR 16766 */
            LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                    getCurrentTick(), ueIndex, recvdRNTI, 
                    0, 0, 0, 0,0, __func__, "TempCtxNull");
            /* - SPR 16766 */
            return;
        }
        
        retVal =  deMultiplexTCRNTIMessage(demuxData_p,
                                     origDataPtr_p,
                                     dataPtr_p,
                                     recvdRNTI,
                                     pos,
                                     dataPtrPos,
                                     tempUEContext_p,
                                     ttiCounter,
                                     dlDelay,
                                    /* SPR 15093 fix start */   
                                     thdIndex, 
                                    /* SPR 15093 fix end */   
                                     /*CA Changes start  */
                                     internalCellIndex);
                                     /*CA Changes end  */

        if (MAC_FAILURE == retVal)
        {
            /* + SPR 16766 */
            LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                    getCurrentTick(), ueIndex, recvdRNTI, 
                    0, 0, 0, 0,0, __func__, "demuxTCrntiFail");
            /* - SPR 16766 */
            LTE_MAC_UT_LOG(LOG_WARNING,DEMUX,
               "[%s]: deMultiplexTCRNTIMessage() returned failure\n",__func__);
        }
    }
    }
    else /*If message received on CRNTI*/
    {
        UInt32 rlcFlag      = FALSE;
        UInt32 invalidLCDataFlag = FALSE;
        UInt32 idx          = 0;        
        UInt32 tempPreviousBsr = 0;
        /*Qload adjustment changes start*/
        UInt32 longBSRFlag    = FALSE;
        UInt32 shortBSRFlag   = FALSE;
        UInt32 lcgIdVal       = LCGID_INVALID_VALUE;        
        UInt32 bsrReport[NUM_OF_LCG] = {0};
        /*Qload adjustment changes end*/
        UInt32 shortBSRLcgId  = LCGID_INVALID_VALUE;        
        UInt32 lcIdx     = 0;
        UInt32 lchId     = 0;
        UInt32 length    = 0;
        RLCUplinkData         *dataToRlc_p = PNULL;        
        ULUEContext           *ulCntxt_p   = PNULL;
        DLUEContext           *dlCntxt_p   = PNULL;
        UInt8 *rlcData_p = PNULL;
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        UInt8 lcgTrigIndexForKpi               = 0;
        UInt8 addTrigLCGNodeForKpi[NUM_OF_LCG] = {0};
#endif
        /*UL Force Grant Changes Start */
        /* ulDataReceivedFlag flag added to mark true when data arrived
         * from UL */
        UInt8 ulDataReceivedFlag = FALSE;
         /*UL Force Grant Changes End */
        void *org_p = (void *) origDataPtr_p;
        ulCntxt_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
        dlCntxt_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

        /* Token adjustment */
#ifdef FDD_CONFIG
        /** SPR 12364 fix Starts**/
        ulHarqProcessId = harqProcessId;
        /** SPR 12364 fix Ends **/    
#elif TDD_CONFIG
        /* SPR 4751 changes */
        ulHarqProcessId = harqProcessId;
        /* SPR 4751 changes */
#endif          
        /* Token adjustment */

        /*Initializing tempPreviousBsr with the value of bsrNetTotal before
         * updating BSR recieved from UE.
         */
        tempPreviousBsr = ulCntxt_p->bsrNetTotal;
         
        /* SPS CHANGES  start */       
        /* Modify the previous bsr by subtracting the SPS LCG BSR in case sps is activated to send the 
           ADD_TRIGGER correctly to RR strategy */ 
        /*Cyclomatic Complexity changes - starts here*/
        updateTempPreviousBsrDuringSPS(ulCntxt_p,
                                       &tempPreviousBsr);
        /*Cyclomatic Complexity changes - ends here*/
        /* SPS CHANGES  end */       

            ulCntxt_p->ulSyncLossOrHOInfoInUL.preambleIdx = dlCntxt_p->ulSyncLossOrHOInfoInDL.preambleIdx;
        /* SPR 5487 Start */
        /**SPR_4954_Fix Start **/
        /* SPR 20834 Changes Start */
        for (idx=0; idx < pos && (pos <= MAX_NUM_OF_SUBHEADER_IN_UL_PDU); idx++)
        /* SPR 20834 Changes End */
        {
        /**SPR_4954_Fix End **/
        /* SPR 5487 End */
            lchId  = (demuxData_p + idx)->lchId;
            length = (demuxData_p + idx)->length;

            /*find logical channel ID*/
            LTE_MAC_UT_LOG(LOG_DETAIL,DEMUX,
                    "[%s]:Logical Channel ID [%d] and Length [%d]\n",__func__,
                    lchId,length);
            
            LOG_UT(MAC_DEMUX_DATA_LC_ID,LOGDEBUG,MAC_L1_INF,\
                    getCurrentTick(),\
                    lchId,\
                    length,
                    0,0,0,\
                    0.0,0.0,__func__,"DEMUX_DATA_LC");

            switch (lchId)
            {
                case MAC_UL_CCCH_LCH:
                {
                    /*Cyclomatic Complexity changes - starts here*/
                    demuxAndSendMacUlCcchLchMsg(dlCntxt_p,
                            ulCntxt_p,
                            recvdRNTI,
                            ttiCounter,
                            dlDelay,
                            &dataPtrPos,
                            /* +- SPR 17777 */
                            &org_p,
                            &length,
                            &dataToRlc_p,
                            /*CA Changes start  */
                            internalCellIndex);	
                            /*CA Changes end  */        
                    /*Cyclomatic Complexity changes - ends here*/
                    return;
                }
                break; 
                      
                case MAC_LCID_1:
                case MAC_LCID_2:
                case MAC_LCID_3:
                case MAC_LCID_4:
                case MAC_LCID_5:
                case MAC_LCID_6:
                case MAC_LCID_7:
                case MAC_LCID_8:
                case MAC_LCID_9:
                case MAC_LCID_10:
                {
		    /*Cyclomatic Complexity changes - starts here*/
                    demuxMacOneToTenLchMsg(lchId,
                                           &lcIdx,
                                           ulCntxt_p,
                                           dlCntxt_p,
                                           /* + SPS_TDD_CHANGES */
                                           recvdRNTI,
                                           /* - SPS_TDD_CHANGES */
                                           &rlcFlag,
                                           &org_p,
                                           &rlcData_p,
                                           &dataPtrPos,
                                           length,
                                           &invalidLCDataFlag,
                                           &lcgIdVal,
                                           longBSRFlag,
                                           shortBSRFlag,
                                           bsrReport,
                                           ulHarqProcessId,
                                           shortBSRLcgId,
                                           ueIndex,
                                           &dataPtr_p,
                                           &dataToRlc_p,
                                           /*UL Force Grant Changes Start */
                                           &ulDataReceivedFlag,
                                           internalCellIndex	
                                               /*UL Force Grant Changes End */
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD)
#ifdef TDD_CONFIG
                                           ,ttiCounter
#endif
#endif
                                               /* +- SPR 17777 */
                                           /* SPR 11331 Fix End */
                                           );

		    /*Cyclomatic Complexity changes - ends here*/
                }    
                break;
                
                case MAC_POWER_HEAD_LCH:
                {
                    /*Cyclomatic Complexity changes - starts here*/
                    demuxMacPowerHeadLchMsg(&dataPtr_p,
                            /* +- SPR 17777 */
                            /* +- SPR 17777 */
                                            ulCntxt_p,
                                            &dataPtrPos,
                                            length,
                                            powerControlDeltaTFAndLogFactor,
                                            ttiCounter,
                                            harqProcessId,
                                            ueIndex,
                                            /*CA Changes start  */
                                            internalCellIndex);	
                                            /*CA Changes end  */        
		    /*Cyclomatic Complexity changes - ends here*/
                }
                break;

                case MAC_TRUNCATED_BSR:
                case MAC_S_BSR_LCH:
                {
                    /*short BSR*/
                    shortBSRLcgId = updateShortOrTruncatedBSR(ulCntxt_p,
                                                              dataPtr_p,
                                                              thdIndex,
                                                              /*CA Changes start  */
                                                              internalCellIndex
                                                              /*CA Changes end  */ 
                                                              /* SPR 16916 FIX START */
#ifdef KPI_STATS
                                                              ,&lcgTrigIndexForKpi,
                                                              addTrigLCGNodeForKpi
#endif
                                                              /* SPR 16916 FIX START */
                                                              );

                    if (LCGID_INVALID_VALUE == shortBSRLcgId)
                    {
                        LTE_MAC_UT_LOG(LOG_DETAIL,DEMUX,
                                "[%s]: ERROR: Invalid short BSR LCGID receivd\n",
                                __func__);
                    }
                    else
                    {
                        shortBSRFlag = TRUE;
                    }

                    dataPtrPos = dataPtrPos + length;
                    /*increment data pointer to point to next SDU*/
                    dataPtr_p++;
                }
                break;
                
                case MAC_L_BSR_LCH:
                {
                    /* it's a Long BSR*/
                    dataPtr_p = updateLongBSR(ulCntxt_p,
                                         dataPtr_p,
                                         thdIndex,
                                         /*CA Changes start  */
                                         internalCellIndex
                                         /* SPR 16916 FIX START */
#ifdef KPI_STATS
                                         ,&lcgTrigIndexForKpi,
                                         addTrigLCGNodeForKpi
#endif
                                         /* SPR 16916 FIX END */
                                         );
                                         /*CA Changes end  */

                    /*increment data pointer to point to next SDU*/
                    dataPtr_p++;
                    dataPtrPos = dataPtrPos + length;
                    longBSRFlag = TRUE;
                }    
                break;
                
                default:
                {
                    /*increment data pointer to point to next SDU*/
                    dataPtr_p+=length;
                    dataPtrPos = dataPtrPos + length;
                }    
                break;
            }
        }

        /*UL Force Grant Changes Start */
        /* Extended grant to be applied only if both num and size EUL is 
         * non-zero and is a DRB */
        if ((cellParams_p->numEUL) && (cellParams_p->sizeEUL) && 
                ( lchId >= MAC_LCID_1 && lchId <= MAC_LCID_10 ))
        {
            if(TRUE == ulCntxt_p->firstBSRReceived)
            {
                /* UL data is received for any SRB 1, SRB2 or DRBs */
                if((TRUE == ulDataReceivedFlag))
                {
                    /* SPR 21777 fix start */
                    ulCntxt_p->numOfForceGrant = cellParams_p->numEUL;
                }
                /* SPR 21777 fix end */
            }
        }
        /*UL Force Grant Changes End */
        if(PREAMBLE_BUSY == dlCntxt_p->ulSyncLossOrHOInfoInDL.preambleStatus)
        {
            ulCntxt_p->ulSyncLossOrHOInfoInUL.preambleIdx = dlCntxt_p->ulSyncLossOrHOInfoInDL.preambleIdx;
            putEntryInRachDemuxQueue(FIRST_UPLINK_DATA_AFTER_CFR,
                    INVALID_RNTI,
                    recvdRNTI,
                    PNULL,
                    0,
                    ttiCounter,
                    dlDelay,
                    /*CA Changes start  */
                    internalCellIndex);
                    /*CA Changes end  */
        }
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        if (lcgTrigIndexForKpi)
        {
            macUpdateActiveUlUePerQciInfo(lcgTrigIndexForKpi,ulCntxt_p,addTrigLCGNodeForKpi,internalCellIndex);
        }
#endif
        /* SPR 16916 FIX END */
        /*CA Changes start  */
        if(
            UL_QOS_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                initParams_p->ulSchdConfig.ulSchedulerStrategy )
        /*CA Changes end  */
        {
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD) 
            /* Token adjustment */
            restoreTokensOfNonTransmittedLCs(ulCntxt_p, ulHarqProcessId);
            /* Token adjustment */
#endif
            /*Qload adjustment changes start*/ 
            if(longBSRFlag == FALSE)
            {
                /*CA Changes start  */
                ulQosAdjustQload(ulCntxt_p,&bsrReport[0],shortBSRFlag,shortBSRLcgId,ulHarqProcessId, internalCellIndex); 
                /*CA Changes end  */
            }
            /*Qload adjustment changes end*/
            

        } 
        else
        {
            if ((!tempPreviousBsr) && 
                    (ulCntxt_p->bsrNetTotal))
            {
                /*BSR from zero to non zero.
                 *Set the
                 zeroToNonZeroFlag as
                 ADD_TRIGGER
                 */
                /*SPR 3608 Changes Start*/
                if(ulCntxt_p->prioritySchedulingReqUL)
                {
                    /*CA Changes start  */
                    putBSREntryInPriorityGrantQueue(ueIndex, internalCellIndex);
                    /*CA Changes end  */
                }
                else
                {
                    if(!checkSpsBsrOnlyReceived(ulCntxt_p))
                    {
                        /*CA Changes start  */
                        putEntryInBSRQueue(ueIndex, 0,
                                thdIndex, ADD_TRIGGER,
                                internalCellIndex);
                        /*CA Changes end  */

                        /* +- SPR 17404 */
                    }
                }
                /*SPR 3608 Changes End*/
            }
        }

        /*Free the end padding zero copy buffer*/
        /*Cyclomatic Complexity changes - starts here*/
        freeEndPaddingZeroCopyBuffer(rlcFlag,
                                     invalidLCDataFlag,
                                     &org_p,
                                     &dataPtrPos);
        /*Cyclomatic Complexity changes - ends here*/
        if (lcIdx)
        {
            /* + Coverity_31473 */
            /* +- SPR 17777 */
/*SPR 21179 Start*/
#ifdef SSI_DEBUG_STATS
/*SPR 21179 End*/
            lteMacUpdateStatsRlcTx(dataToRlc_p);
            /* +- SPR 17777 */
/*SPR 21179 Start*/
#endif 
/*SPR 21179 End*/
            /* - Coverity_31473 */
            if (!(macUeDataInd(ueIndex, dataToRlc_p)))
            {
                LTE_MAC_UT_LOG(LOG_WARNING,DEMUX,
                    "[%s]: WARNING: macUeDataInd() returned failure\n",
                    __func__);
                /* + SPR 16766 */
                LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                        getCurrentTick(), ueIndex, recvdRNTI, 
                        lcIdx, 0, 0, 0,0, __func__, "macUeDataIndFailed");
                /* - SPR 16766 */
            }
        }
        /* + SPS_TDD_Changes */
        /*CA Changes start  */
        checkIsSpsDataReceivedOnOccasion(ulCntxt_p, ulHarqProcessId,recvdRNTI, internalCellIndex);
        /*CA Changes end  */
	/* - SPS_TDD_Changes */
    }
    
    LTE_MAC_UT_LOG(LOG_DETAIL,DEMUX,
            "Exit [%s]: Function\n",__func__);
}
/*Cyclomatic Complexity changes - starts here*/
/*****************************************************************************
 * Function Name  : updateTempPreviousBsrDuringSPS 
 * Inputs         : ulCntxt_p,
 *                  tempPreviousBsr
 * Outputs        : None
 * Returns        : None
 * Description    : Modify the previous bsr by subtracting the SPS LCG BSR in case sps is activated to send the
 *                  ADD_TRIGGER correctly to RR strategy.
 *****************************************************************************/
static  void updateTempPreviousBsrDuringSPS(ULUEContext *ulCntxt_p,
                                                  UInt32 *tempPreviousBsr)
{
    if ((ulCntxt_p->spsUlInfo.spsState == SPS_ACTIVATED) && (0 != *tempPreviousBsr))
    {
        if (*tempPreviousBsr > ulCntxt_p->bsrNet[ulCntxt_p->lcInfo[ulCntxt_p->spsUlInfo.lcId].lcGId])
        {
            *tempPreviousBsr -=ulCntxt_p->bsrNet[ulCntxt_p->lcInfo[ulCntxt_p->spsUlInfo.lcId].lcGId];
        }
        else
        {
            *tempPreviousBsr = 0;
        }
    }
}
/*****************************************************************************
 * Function Name  : demuxAndSendMacUlCcchLchMsg 
 * Inputs         : dlCntxt_p,
 *                  ulCntxt_p,
 *                  recvdRNTI,
 *                  ttiCounter,
 *                  dlDelay,
 *                  dataPtrPos,
 *                  dataPtr_p,
 *                  org_p,
 *                  length,
 *                  dataToRlc_p,
 *                  internalCellIndex 
 * Outputs        : None 
 * Returns        : None 
 * Description    : This func will process MAC_UL_CCCH_LCH msg.
 *****************************************************************************/
static  void demuxAndSendMacUlCcchLchMsg(DLUEContext *dlCntxt_p,
				                ULUEContext *ulCntxt_p,
				                UInt16 recvdRNTI,
		                                /* SPR 15909 fix start */
						tickType_t ttiCounter,
                       			        /* SPR 15909 fix end */
						UInt32 dlDelay,
						UInt32 *dataPtrPos,
                        /* +- SPR 17777 */
						void **org_p,
						UInt32 *length,
						RLCUplinkData **dataToRlc_p,
                        /*CA Changes start  */
                        InternalCellIndex internalCellIndex)	
                        /*CA Changes end  */        
{
    LOG_MAC_MSG(MAC_UL_CCCH_MSG_ID, LOGINFO, MAC_CCCH,
            getCurrentTick(),
            recvdRNTI,
            dlCntxt_p->ueIndex,
            *length, 
            internalCellIndex, 
            dlCntxt_p->ulSyncLossOrHOInfoInDL.preambleStatus,
            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME, "" );

    if(PREAMBLE_BUSY == dlCntxt_p->ulSyncLossOrHOInfoInDL.preambleStatus)
    {
        ulCntxt_p->ulSyncLossOrHOInfoInUL.preambleIdx = dlCntxt_p->ulSyncLossOrHOInfoInDL.preambleIdx;
        putEntryInRachDemuxQueue(FIRST_UPLINK_DATA_AFTER_CFR,
                                 INVALID_RNTI,
                                 recvdRNTI,
                                 PNULL,
                                 0,
                                 ttiCounter,
                                 dlDelay,
                                 /*CA Changes start  */
                                 internalCellIndex);
                                 /*CA Changes end  */
    }
    /* +- SPR 17777 */
    decodeAndSendCCCHMsgToRRC(dataPtrPos,org_p,
                              length,recvdRNTI,dlCntxt_p->ueIndex,
                              internalCellIndex);
    /*data will not present if CCCH SDU present so return 
     *from here
     */
    /*Coverity 25485 Start Fix*/
    if(PNULL != *dataToRlc_p)
    {
        freeMemPool(*dataToRlc_p);
        /*SPR 16979 Fix Start*/
        *dataToRlc_p = PNULL;
        /*SPR 16979 Fix End*/
    } 
    /*Coverity 25485 Start End*/
}
/*****************************************************************************
 * Function Name  : demuxMacOneToTenLchMsg 
 * Inputs         : lchId,
 *                  lcIdx,
 *                  ulCntxt_p,
 *                  dlCntxt_p,
 *                  rlcFlag,
 *                  org_p,
 *                  rlcData_p,
 *                  dataPtrPos,
 *                  length,
 *                  invalidLCDataFlag,
 *                  lcgIdVal,
 *                  longBSRFlag,
 *                  shortBSRFlag,
 *                  bsrReport,
 *                  ulHarqProcessId,
 *                  shortBSRLcgId,
 *                  ueIndex,
 *                  dataPtr_p,
 *                  dataToRlc_p
 *                  internalCellIndex 
 *                  ttiCounter
 * Outputs        : updates few input parameters 
 * Returns        : None
 * Description    : This fucntion will demultiplex the msg coming on LCH 1 to 10.
 *****************************************************************************/
static  void demuxMacOneToTenLchMsg(UInt32 lchId,
                                          UInt32 *lcIdx,
                                          ULUEContext *ulCntxt_p,
                                          DLUEContext *dlCntxt_p,
                                          /* + SPS_TDD_CHANGES */
                                          UInt16 recvdRNTI,
                                          /* - SPS_TDD_CHANGES */
                                          UInt32 *rlcFlag,
                                          void **org_p,
                                          UInt8 **rlcData_p,
                                          UInt32 *dataPtrPos,
                                          UInt32 length,
                                          UInt32 *invalidLCDataFlag,
                                          UInt32 *lcgIdVal,
                                          UInt32 longBSRFlag,
                                          UInt32 shortBSRFlag,
                                          UInt32 *bsrReport,
                                          UInt8 ulHarqProcessId,
                                          UInt32 shortBSRLcgId,
                                          UInt16  ueIndex,
                                          UInt8 **dataPtr_p,
                                          RLCUplinkData **dataToRlc_p,
                                          /*UL Force Grant Changes Start */
                                          UInt8 *ulDataReceivedFlag,
                                          InternalCellIndex internalCellIndex
                                           /*UL Force Grant Changes End */
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD)
#ifdef TDD_CONFIG
                                          /* SPR 15909 fix start */                                         
                                            ,tickType_t ttiCounter
                                          /* SPR 15909 fix end */
#endif
#endif
                                            /* +- SPR 17777 */
                                          /* SPR 11331 Fix Start */
                                          )
{
    LP_SpsUlInfo   spsUlInfo_p   = PNULL;
    RLCUplinkDataInfo *rlcLCIdData_p = PNULL;
    /* SPR 13492 Changes Start */
#ifdef KPI_STATS
    UInt8 qci = INVALID_QCI;
#endif
    /* SPR 13492 Changes End */
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
    /* SPR 5487 Start */
    if ((*lcIdx >= MAX_NUM_UL_PKTS) || (INVALID_LCID == \
         ulCntxt_p->lcInfo[lchId].lchId))
#else
    if ((*lcIdx >= MAX_NUM_UL_PKTS) || (INVALID_LCID == \
         dlCntxt_p->logicalChannel[lchId].logicalChannelId))
    /* SPR 5487 End */
#endif
    {
        LTE_MAC_UT_LOG(LOG_WARNING,DEMUX,
                       "[%s]: Invalid logical channel Id %d\n",
                        __func__,lchId);
        /* + SPR 16766 */
        LOG_MAC_MSG( PENDING_DELETE_UE, LOGERROR, MAC_RACH,
                getCurrentTick(), ueIndex, 
                dlCntxt_p->logicalChannel[lchId].logicalChannelId, 
                *lcIdx, 0, 0, 0,0, __func__, "108");
        /* - SPR 16766 */
        if (*rlcFlag)
        {
            SPLIT_ZERO_BUFFER_COPY(*org_p, *rlcData_p, *dataPtrPos);
            *org_p = (void *)(*rlcData_p);
            *dataPtrPos = length;
            *invalidLCDataFlag = TRUE;
            *rlcFlag = FALSE;	
        }
        else
        {
            *dataPtrPos = *dataPtrPos + length;
        }

        LOG_MAC_MSG(MAC_DEMUX_DATA_INVALID_LC_ID,LOGWARNING,MAC_L1_INF,\
                getCurrentTick(),\
                lchId,\
                length,
                0,0,0,\
                0.0,0.0,__func__,"DEMUX_DATA_INVALID_LC_ID");
    }
    else
    {
        /* SPS CHNAGES */
#ifndef FDD_CONFIG
        /* + SPS_TDD_Changes */
        if ((ulCntxt_p->spsUlInfo.hpId[0].harqId == ulHarqProcessId) && \
                (ulCntxt_p->spsUlInfo.SpsCrnti== recvdRNTI))
#else                               
            /** SPR 12364 fix Starts**/    
            if((((ulCntxt_p->ulresInfo[ulHarqProcessId].ulSchReqInfo.isTTIBundled) &&
                            (ulCntxt_p->ulresInfo[ulHarqProcessId].ulSchReqInfo.isSpsEnabled) ) ||
                        (!(ulCntxt_p->ulresInfo[ulHarqProcessId].ulSchReqInfo.isTTIBundled) &&
                         (ulCntxt_p->spsUlInfo.hpId[0].harqId == ulHarqProcessId))) &&
                    (ulCntxt_p->spsUlInfo.SpsCrnti== recvdRNTI))
                /* - SPS_TDD_Changes */
                /** SPR 12364 fix Ends **/
#endif                               
            {
                ulCntxt_p->spsUlInfo.hpId[0].isDataReceived = TRUE;
                ulCntxt_p->spsUlInfo.numOfSureSpsEmptyTx = 0;
                ulCntxt_p->spsUlInfo.numOfUnsureSpsEmptyTx = 0;
            }
#ifndef FDD_CONFIG
        /* + SPS_TDD_Changes */
            else if ((ulCntxt_p->spsUlInfo.hpId[1].harqId == ulHarqProcessId)&&\
                    (ulCntxt_p->spsUlInfo.SpsCrnti== recvdRNTI))
#else                               
                /** SPR 12364 fix Starts**/
            else if((((ulCntxt_p->ulresInfo[ulHarqProcessId].ulSchReqInfo.isTTIBundled) &&
                            (ulCntxt_p->ulresInfo[ulHarqProcessId].ulSchReqInfo.isSpsEnabled) ) ||
                        (!(ulCntxt_p->ulresInfo[ulHarqProcessId].ulSchReqInfo.isTTIBundled) &&
                         (ulCntxt_p->spsUlInfo.hpId[1].harqId == ulHarqProcessId)))  &&
                    ( ulCntxt_p->spsUlInfo.SpsCrnti== recvdRNTI)) 
                /* - SPS_TDD_Changes */

                /** SPR 12364 fix Ends **/
#endif                               
            {
                ulCntxt_p->spsUlInfo.hpId[1].isDataReceived = TRUE;
                ulCntxt_p->spsUlInfo.numOfSureSpsEmptyTx = 0;
                ulCntxt_p->spsUlInfo.numOfUnsureSpsEmptyTx = 0;
            }
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD)
        /* + SPR_5711 */ 
        if(FALSE == ulCntxt_p->isDemuxIndexPresent)
        { 
            ulCntxt_p->isDemuxIndexPresent = TRUE;

/* SPR 11331 Fix Start */
#ifdef FDD_CONFIG  
            demuxProcessMap_gp[internalCellIndex]->ueIdToBeProcessed[demuxProcessMap_gp[internalCellIndex]->demuxIndex] = ulCntxt_p->ueIndex;
#else
            demuxProcessMap_gp[internalCellIndex]->ueIdToBeProcessed[demuxProcessMap_gp[internalCellIndex]->demuxIndex].ueIndex = ulCntxt_p->ueIndex;
            demuxProcessMap_gp[internalCellIndex]->ueIdToBeProcessed[demuxProcessMap_gp[internalCellIndex]->demuxIndex].rcvdTTI = ttiCounter;
#endif
            demuxProcessMap_gp[internalCellIndex]->demuxIndex++; 
        }
#endif
        /* + Coverity_31959_31958 */  
        if(lchId >= MAC_LCID_3)
        /* - Coverity_31959_31958 */  
        { 
            ulCntxt_p->lastScheduledTick = getCurrentTickUl();
            /* SPR 13492 Changes Start */
#ifdef KPI_STATS
            qci = ulCntxt_p->lcInfo[lchId].ulQosInfo.QCI;
            ulUECtxInfoArr_g[ulCntxt_p->ueIndex].inSessionTimePerQCIFlag[qci - 1] = TRUE; /* SPR 14267 Change */
#endif
            /* SPR 13492 Changes End */
        }
        if(dlCntxt_p->tcrntiFlag)
        {
            ulUECtxInfoArr_g[ueIndex].pendingDeleteFlag = FALSE;
            dlUECtxInfoArr_g[ueIndex].pendingDeleteFlag = FALSE;
            dlCntxt_p->tcrntiFlag = FALSE;
        }
        *ulDataReceivedFlag = TRUE;
        ulCntxt_p->firstBSRReceived = TRUE;
        /*data arrrived for RLC*/
        if (PNULL == *dataToRlc_p)
        {
            GET_MEM_FROM_POOL(RLCUplinkData,*dataToRlc_p,sizeof(RLCUplinkData),PNULL);
            /* SPR 1753 changes start */
        }    
        /* SPR 1753 changes end */    
        SPLIT_ZERO_BUFFER_COPY(*org_p, *rlcData_p, 
                                    *dataPtrPos);
        if (*invalidLCDataFlag)
        {
            msgFree(*org_p);
            *invalidLCDataFlag = FALSE;
        }

        *org_p = (void *)(*rlcData_p);
        *dataPtrPos = length;
        *lcgIdVal = ulCntxt_p->lcInfo[lchId].lcGId;
        if (FALSE == longBSRFlag)
        {
            /*update bsrReport only if no bsr has arrived . 
             if bsr arrived then this has to be updated 
             by bsr only*/
            /*increment it only by number of bytes arrived*/
            if (TRUE == shortBSRFlag)
            {
                if (shortBSRLcgId != *lcgIdVal)
                {
                    bsrReport[*lcgIdVal] += length; 
                }
            }
            else
            {
                bsrReport[*lcgIdVal] += length; 
            }
        }

        rlcLCIdData_p = &(*dataToRlc_p)->rlcDataArr[*lcIdx];

        /* fill the struct for sending data to RLC*/
        rlcLCIdData_p->rlcdataBuffer = *rlcData_p;
        rlcLCIdData_p->dataLength    = length;
        rlcLCIdData_p->lcId          = lchId;
        spsUlInfo_p = &(ulCntxt_p->spsUlInfo);

#ifdef KPI_STATS
        rlcLCIdData_p->packetTime = getCurrentTickUl();
#endif
        if(ulCntxt_p->lcInfo[lchId].isSpsLc)
        {
            /* Append Pkt TS for SPS LC for an UE */
            rlcLCIdData_p->pktTimeStamp  = getCurrentTickUl(); 
            if( SPS_ACTIVATED == spsUlInfo_p->spsState )
            {
                rlcLCIdData_p->spsState = PDCP_MAC_SPS_ACTIVATED;
            }
            else 
            {
                rlcLCIdData_p->spsState = PDCP_MAC_SPS_DEACTIVATED;
            }
        }
        *lcIdx += 1;
        (*dataToRlc_p)->numLCInfo = *lcIdx;
        *rlcFlag = TRUE;

        /* SPR 3608 changes start */
        if(ulCntxt_p->prioritySchedulingReqUL)
        {
            if(MAC_LCID_1 < lchId)
            {
                ulCntxt_p->recvdUePsrCount++;
                if (ulCntxt_p->recvdUePsrCount >=
                                 ulCntxt_p->uePsrCount)
                {
                    ulCntxt_p->prioritySchedulingReqUL = FALSE;
                }
            }
        }
        /* SPR 3608 changes end */

        if(ulCntxt_p->lcInfo[lchId].isSpsLc)
        {
            ulCntxt_p->spsUlInfo.pktArrivalTs = getCurrentTickUl();
        }
        /*CA Changes start  */
        {
        if( UL_QOS_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig.ulSchedulerStrategy )
        /*CA Changes end  */
        {
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD)
            /*Tokens adjustment*/
            /* +- SPR 17777 */
            adjustTokensForLcId(ulCntxt_p, lchId, length, ulHarqProcessId);
            /* +- SPR 17777 */
            /*Tokens adjustment*/
#else
            /* SPR 5127 Start */
            if ( ulCntxt_p->lcInfo[lchId].ulQosInfo.channelType == NON_GBR )
            {
                 dlQosMappingForTokenAdjust[ueIndex][ulHarqProcessId][ulCntxt_p->ulAmbrInfo.lcId] += length;
            }
            else
            {
                dlQosMappingForTokenAdjust[ueIndex][ulHarqProcessId][lchId] += length;
            }
            /* SPR 5127 End */
#endif
        }
        }
        /*increment dataPtr_p by len so that dataPtr_p reaches 
        *to start of next RLC SDU.
        */
        (*dataPtr_p) += length;
    }
}
/*cyclomatic fix start*/
/*****************************************************************************
 * Function Name  : validatePHRForPLCalculation 
 * Inputs         : pwrHeadRoomInDb,
 *                  cellParams_p,
 *                  ulCntxt_p,
 *                  powerControlDeltaTFAndLogFactor,
 *                  ueIndex,
 *                  harqProcessId,
 *                  internalCellIndex 
 * Outputs        : None
 * Returns        : None 
 * Description    : This function will validate PHR and calculate the PathLoss.
 ******************************************************************************/
/* SPR 15909 fix start */
STATIC  void validatePHRForPLCalculation(SInt8 pwrHeadRoomInDb,CellConfigParams *cellParams_p,ULUEContext *ulCntxt_p,
                                               SInt8 powerControlDeltaTFAndLogFactor,UInt16  ueIndex,
                                               UInt8 harqProcessId,tickType_t ttiCounter,InternalCellIndex internalCellIndex)
/* SPR 15909 fix end */
{
    if ((pwrHeadRoomInDb <= MAX_PHR_IN_DB)
      && (pwrHeadRoomInDb >= MIN_PHR_IN_DB))
    /*SPR_10754_Changes End*/ 
    {
        /* SPR 4609_4610 End */
        /*CA Changes start  */
        if(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->commonULPowerControlInfo.alpha)
        /*CA Changes end  */
        {
            if(ulCntxt_p->tpcInfoForPusch.lastAppliedTick > ttiCounter)
            {
#ifdef FDD_CONFIG
                /** SPR 12364 fix Starts**/
                if( TRUE == checkSpsOccasionTtiFrmDemux(ulCntxt_p, harqProcessId) )
                    /** SPR 12364 fix Ends**/
#else                                
                if((SPS_ACTIVATED == ulCntxt_p->spsUlInfo.spsState) &&
                   ((harqProcessId == ulCntxt_p->spsUlInfo.hpId[0].harqId) || (harqProcessId == ulCntxt_p->spsUlInfo.hpId[1].harqId)))
#endif                   
                {
                    ulCntxt_p->pathLoss = calculatePathlossFromPHR (ulCntxt_p->pcMax,
                                          cellParams_p->commonULPowerControlInfo.alpha,
                                          (ulCntxt_p->spsUlInfo.spsUlSetupInfo_p->p0PersistentInfo.p0NominalPuschPersistent + ulCntxt_p->spsUlInfo.spsUlSetupInfo_p->p0PersistentInfo.p0UePuschPersistent),
                                          powerControlDeltaTFAndLogFactor,
                                          pwrHeadRoomInDb,/*SPR_PH_MAPPING*/
                                          ulCntxt_p->prevPowerCorrectionForPusch);
                }
                else
                {
                    ulCntxt_p->pathLoss = calculatePathlossFromPHR (ulCntxt_p->pcMax, 
                                          cellParams_p->commonULPowerControlInfo.alpha,
                                          cellParams_p->commonULPowerControlInfo.p0NominalPUSCH + ulCntxt_p->dedicatedULPowerControlInfo.p0UePusch,
                                          powerControlDeltaTFAndLogFactor, 
                                          pwrHeadRoomInDb,/*SPR_PH_MAPPING*/ 
                                          ulCntxt_p->prevPowerCorrectionForPusch);
                }
            }
            else
            {
#ifdef FDD_CONFIG
                /** SPR 12364 fix Starts**/
                if( TRUE == checkSpsOccasionTtiFrmDemux(ulCntxt_p, harqProcessId) )
                    /** SPR 12364 fix Ends**/
#else                                
                if((SPS_ACTIVATED == ulCntxt_p->spsUlInfo.spsState) &&
                   ((harqProcessId == ulCntxt_p->spsUlInfo.hpId[0].harqId) || (harqProcessId == ulCntxt_p->spsUlInfo.hpId[1].harqId)))
#endif                   
                {
                    ulCntxt_p->pathLoss =  calculatePathlossFromPHR(ulCntxt_p->pcMax, 
                                           cellParams_p->commonULPowerControlInfo.alpha,
                                           (ulCntxt_p->spsUlInfo.spsUlSetupInfo_p->p0PersistentInfo.p0NominalPuschPersistent + ulCntxt_p->spsUlInfo.spsUlSetupInfo_p->p0PersistentInfo.p0UePuschPersistent),
                                           powerControlDeltaTFAndLogFactor, 
                                           pwrHeadRoomInDb,/*SPR_PH_MAPPING*/ 
                                           ulCntxt_p->currPowerCorrectionForPusch);
                }
                else
                {
                    ulCntxt_p->pathLoss =  calculatePathlossFromPHR(ulCntxt_p->pcMax, 
                                           cellParams_p->commonULPowerControlInfo.alpha,
                                           cellParams_p->commonULPowerControlInfo.p0NominalPUSCH + ulCntxt_p->dedicatedULPowerControlInfo.p0UePusch,
                                           powerControlDeltaTFAndLogFactor, 
                                           pwrHeadRoomInDb,/*SPR_PH_MAPPING*/ 
                                           ulCntxt_p->currPowerCorrectionForPusch);
                }

            }
            if(ulCntxt_p->pathLoss > MAX_PATHLOSS)
            {
                ulCntxt_p->pathLoss = MAX_PATHLOSS;
            }
            else if(ulCntxt_p->pathLoss < MIN_PATHLOSS)
            {
                ulCntxt_p->pathLoss = MIN_PATHLOSS;
            }
            /* ICIC changes start */
            /*CA Changes start  */
            updateULPathLoss(ueIndex,ulCntxt_p->pathLoss, internalCellIndex);
            /*CA Changes end  */
            /* ICIC changes end */
        }
    }
}
/*cyclomatic fix end*/
/*****************************************************************************
 * Function Name  : demuxMacPowerHeadLchMsg 
 * Inputs         : dataPtr_p,
 *                  recvdRNTI,
 *                  ulCntxt_p,
 *                  dataPtrPos,
 *                  length,
 *                  powerControlDeltaTFAndLogFactor,
 *                  ttiCounter,
 *                  harqProcessId,
 *                  ueIndex,
 *                  internalCellIndex 
 * Outputs        : None
 * Returns        : None 
 * Description    : This function will process the MAC_POWER_HEAD_LCH msg.
 *****************************************************************************/
STATIC  void demuxMacPowerHeadLchMsg(UInt8 **dataPtr_p,
                                           /* +- SPR 17777 */
                                           /* +- SPR 17777 */
                                           ULUEContext *ulCntxt_p,
                                           UInt32  *dataPtrPos,
                                           UInt32 length,
                                           SInt8 powerControlDeltaTFAndLogFactor,
                                          /* SPR 15909 fix start */
                                           tickType_t ttiCounter,
                                           /* SPR 15909 fix end */
                                           UInt8 harqProcessId,
                                           UInt16  ueIndex,
                                           /*CA Changes start  */
                                           InternalCellIndex internalCellIndex)
                                           /*CA Changes end  */        
{
    UInt8 pwrHeadRoom = 0;
    /*SPR_PH_MAPPING_START*/
    SInt8 pwrHeadRoomInDb = 0;
    /*SPR_PH_MAPPING_END*/
    /*Power headroom report*/
    pwrHeadRoom = **dataPtr_p & 0x3F; /*PH is 6 bits*/
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;

    LOG_MAC_MSG(MAC_UL_PHR_LCH_MSG_ID, LOGBRIEF, MAC_PUSCH,
            getCurrentTick(),
            ulCntxt_p->crnti,
            ueIndex,
            pwrHeadRoom, 
            internalCellIndex, 
            DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME, "" );

/*<!- SPR_7532_FIX_START -!>*/
    /* +DYNAMIC_ICIC */
    if(INVALID_PHR_GRANT != ulCntxt_p->phrGrant)
    {
        ulCntxt_p->phrProhibitTimerExpiryTick =
            ulCntxt_p->phrTimerCfg.prohibitPHRTimer + getCurrentTick();
    }
    /* -DYNAMIC_ICIC */
/*<!- SPR_7532_FIX_END -!>*/

    /* +CLPC_CHG_LJA */
    /* SPR 3486 - Start */
    /*SPR_PH_MAPPING_START*/
    pwrHeadRoomInDb = pwrHeadRoom - DIFF_PH_REPORT_PH_DB;
    /*SPR_PH_MAPPING_END*/
    /* SPR 3486 - END */

    /* SPR 4609_4610 Start */
    if ( pwrHeadRoomInDb <= MAX_POWER_REACHED_PH_VAL  )
    {
        ulCntxt_p->tpcInfoForPusch.maxPowerReachedFlag = TRUE;
        ulCntxt_p->tpcInfoForPusch.minPowerReachedFlag = FALSE;
    }
    else if ( pwrHeadRoomInDb >= MIN_POWER_REACHED_PH_VAL )
    {
        if ( ( ( ulCntxt_p->dedicatedULPowerControlInfo.deltaMcsEnabled) &&
               ( powerControlDeltaTFAndLogFactor >= RB_PLUS_MCS_MIN_PHR_COND_THRESHOLD) ) ||
               ( ( !ulCntxt_p->dedicatedULPowerControlInfo.deltaMcsEnabled )&&
               ( powerControlDeltaTFAndLogFactor >= RB_MIN_PHR_COND_THRESHOLD ) ) )
        {
            /* SPR 13196 fix start */
            /* even if UE is reporting max headroom, if avgSINR > target
             *                then we need to send a power down to the UE*/
            if (ulCntxt_p->csiContext.averageSINR <= *(macPathlossToTargetSINRMapForPUSCH_p[internalCellIndex]
                        + ulCntxt_p->pathLoss))
            {
                ulCntxt_p->tpcInfoForPusch.minPowerReachedFlag = TRUE;
                ulCntxt_p->tpcInfoForPusch.maxPowerReachedFlag = FALSE;
            }
            else
            {
                ulCntxt_p->tpcInfoForPusch.minPowerReachedFlag = FALSE;
            }
            /* SPR 13196 fix end */
        }
        else
        {
            ulCntxt_p->tpcInfoForPusch.minPowerReachedFlag = FALSE;
            ulCntxt_p->tpcInfoForPusch.maxPowerReachedFlag = FALSE;
        }
    }
    else
    {
        ulCntxt_p->tpcInfoForPusch.maxPowerReachedFlag = FALSE;
        ulCntxt_p->tpcInfoForPusch.minPowerReachedFlag = FALSE;
    }
    /* SPR 4609_4610 End */

    // Calculate PL from PHR only if the PHR received is more that 0 and less than 63
    /* SPR 4609_4610 Start */
    /*SPR_10754_Changes Start*/
    /*cyclomatic fix start*/
    validatePHRForPLCalculation(pwrHeadRoomInDb,cellParams_p,ulCntxt_p,
                                powerControlDeltaTFAndLogFactor,ueIndex,
                                harqProcessId,ttiCounter,internalCellIndex);
    /*cyclomatic fix end*/ 
    /* Added for TTI Bundling: Start */
    /* SPR 8104 */
    //CODE_COMMENT_L7_FIXED                             
    /*CA Changes start  */
    /* SPR 18641_18843 start */
    // updateULPHR(ueIndex,pwrHeadRoom, internalCellIndex);
    /* SPR 18641_18843 end */
   /*CA Changes end  */
    /* SPR 8104 */
    /* Added for TTI Bundling: End */
    /* -CLPC_CHG_LJA */
    /* +DYNAMIC_ICIC */
    if(INVALID_PHR_GRANT != ulCntxt_p->phrGrant)
    {
        ulCntxt_p->phrProhibitTimerExpiryTick =
        ulCntxt_p->phrTimerCfg.prohibitPHRTimer + getCurrentTickUl();
    }
    /* -DYNAMIC_ICIC */

    *dataPtrPos = *dataPtrPos +length;
                  
    LTE_MAC_UT_LOG(LOG_DETAIL,DEMUX,
                   "[%s]:Power headroom value is : %d\n",
                   __func__,pwrHeadRoom);

    (*dataPtr_p)++;
}
/*****************************************************************************
 * Function Name  : freeEndPaddingZeroCopyBuffer 
 * Inputs         : rlcFlag,
 *                  invalidLCDataFlag,
 *                  org_p,
 *                  dataPtrPos 
 * Outputs        : None
 * Returns        : None 
 * Description    : Free the end padding zero copy buffer.
 *****************************************************************************/
static  void freeEndPaddingZeroCopyBuffer(UInt32 rlcFlag,
                                                UInt32 invalidLCDataFlag,
                                                void   **org_p,
                                                UInt32 *dataPtrPos)
{
    if ( rlcFlag )
    {
        UInt8 *afterRlcBuff_p =  PNULL;
        SPLIT_ZERO_BUFFER_COPY(*org_p, afterRlcBuff_p,
                               *dataPtrPos);
        msgFree(afterRlcBuff_p);
    }
    else
    {
        if (invalidLCDataFlag )
        {
            msgFree (*org_p);	
        }
    }
}
/*Cyclomatic Complexity changes - ends here*/
/*****************************************************************************
 * Function Name  : calculateMACSubHeaderLength
 * Inputs         : headerPtr_p - pointer of the header,
 *                  len_p - pointer to length,
 *                  dataPtrPos_p - Data pointer position
 * Outputs        : None
 * Returns        : header pointer
 * Description    : Calculates the length from MAC sub header.
 *****************************************************************************/
UInt8* calculateMACSubHeaderLength(UInt8 *headerPtr_p,
                                        UInt32 *len_p,
                                        UInt32 *dataPtrPos_p)
{
    /*check whether length is greater than 128 bytes 
     *or less than 128 bytes
     */
    LTE_MAC_UT_LOG(LOG_INFO,DEMUX,
                   "Entry [%s]: Function\n",__func__);

    *len_p = 0;
    
    /*Reffer section 6.1.2 in 36321-850.doc*/
    if (*headerPtr_p & 0x80)
    {
        /*length is greater than 128 bytes*/
        *len_p = *headerPtr_p++ & 0x7F;
        *len_p <<= 8;

        *len_p |= *headerPtr_p;
        *dataPtrPos_p = *dataPtrPos_p+3;
    }
    else
    {
        /*length is less than 128 bytes*/
        *len_p = *headerPtr_p & 0x7F;
        *dataPtrPos_p = *dataPtrPos_p + 2;
    }

    LTE_MAC_UT_LOG(LOG_INFO,DEMUX,
            "Exit [%s]: Function\n",__func__);
    return headerPtr_p;
}

/*****************************************************************************
 * Function Name  : deMultiplexTCRNTIMessage
 * Inputs         : demuxData_p - Pointer contains the LCID and length of data 
 *                                received per lcid, in the sequence received in UL pdu.
 *                  origDataPtr_p - Points to UL pdu received (including header)
 *                  dataPtr_p - Points to data section in UL pdu (no header)
 *                  tcRNTI - received RNTI value in UL message
 *                  pos - contains the number of lcid data received in UL pdu
 *                  dataPtrPos - Position of the dataPtr_p position
 *                  ttiCounter - TTI inwhich message received
 *                  dlDelay - DL delay
 *                  internalCellIndex - cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_FAILURE/MAC_SUCCESS
 * Description    : This will demultiplex the UL messages received on TCRNTI
 *                  (CCCH/CMAC MAC CE) and sends to appropriate module.
 *****************************************************************************/
static MacRetType deMultiplexTCRNTIMessage(DemuxDataBase *demuxData_p,
                                    void    *origDataPtr_p,
                                    UInt8   *dataPtr_p,
                                    UInt16  tcRNTI,
                                    UInt32  pos,
                                    UInt32  dataPtrPos,
                                    TempCRNTICtx *tempUEContext_p,
                                    /* SPR 15909 fix start */
                                    tickType_t  ttiCounter,
                                    /* SPR 15909 fix end */
                                    UInt32 dlDelay,
                                    /* SPR 15093 fix start */   
                                    UInt32  thdIndex, 
                                    /* SPR 15093 fix end */   
                                    /*CA Changes start  */
                                    InternalCellIndex internalCellIndex)
                                    /*CA Changes end  */
{
    UInt32  crntiPresentFlag = FALSE; 
    UInt32  ccchPresentFlag  = FALSE;
    UInt32  lchId  = 0;
    UInt32  length = 0;
    UInt32 rlcFlag      = FALSE;
    UInt32 lcIdx     = 0;
    UInt16 ueIndex = MAX_UE_SUPPORTED;
    RNTIInfo *rntiInfo_p = PNULL; 
    UInt16 msg3Crnti = INVALID_RNTI;
/*533-GCC-warning-fix*/
#ifdef LOG_PRINT_ENABLED
    UInt16 pendingDeleteFlag = 0;
    RNTI   msg3RntiFlag = FREERNTI;
#endif
/*533-GCC-warning-fix*/    
    RLCUplinkData         *dataToRlc_p = PNULL;
    UInt8 *receivedCCCH_Msg = PNULL;
    UInt8 msgLength = 0;
    /* +coverity-5.3.0-CID 32183*/
    RachDemuxMsgType msg = MSG3WITHCRNTI;  /*TDD Warning Fix*/
    /* +coverity-5.3.0-CID 32183*/

    void *org_p = PNULL;
    org_p = origDataPtr_p;
    /* SPR 15093 fix start */   
    ULUEContext           *ulCntxt_p   = PNULL;
    DLUEContext           *dlCntxt_p   = PNULL;
    UInt32 idx    = 0;
    UInt32 shortBSRLcgId  = LCGID_INVALID_VALUE;        
    /* SPR 15093 fix end */   
    /* SPR 16916 FIX START */
#ifdef KPI_STATS
    UInt8 lcgTrigIndexForKpi               = 0;
    UInt8 addTrigLCGNodeForKpi[NUM_OF_LCG] = {0};
#endif
    /* SPR 16916 FIX END */

    /*Check the Message3 is already reveived*/
    if (tempUEContext_p->status == (MSG3_RECEIVED | CONTENTION_SENT))
    {
        LTE_MAC_UT_LOG(LOG_WARNING,DEMUX,
            "dataDeMultiplexAndSend: Msg3 alredy received. Message Ignored\n");
        return MAC_SUCCESS;
    }
    else
    {
        /* SPR 20834 Changes Start */
        for (idx = 0; idx < pos && (pos <= MAX_NUM_OF_SUBHEADER_IN_UL_PDU); idx++)
        {
        /* SPR 20834 Changes End */
        /**SPR_4954_Fix End **/
        /* SPR 5487 End */
            length = (demuxData_p + idx)->length;
            lchId  = (demuxData_p + idx)->lchId;
			
			LOG_MAC_MSG(DEMUX_TCRNTI_MSG_ID,
					LOGINFO,
					MAC_RACH,
					getCurrentTick(),
					tcRNTI,
					tempUEContext_p->status,
					length,
					lchId,
					DEFAULT_INT_VALUE,
					DEFAULT_FLOAT_VALUE,
					DEFAULT_FLOAT_VALUE,
					FUNCTION_NAME,"");

            /*find logical channel ID*/
            switch (lchId)
            {
                case MAC_UL_CCCH_LCH:
                {
                    /*Decode and save the received message in temp UE context*/
                    SPLIT_ZERO_BUFFER_COPY(org_p,
                                           receivedCCCH_Msg,
                                           dataPtrPos);
                    org_p = receivedCCCH_Msg;
                    dataPtrPos = length;
                    msgLength = length;

                    /*increment dataPtr_p by tempLen so that dataPtr_p reaches 
                     *to start of next RLC SDU.
                     */
                    dataPtr_p += length;
                    ccchPresentFlag = TRUE;
                }    
                break;
    
                case MAC_CRNTI_LCH:
                {
                    /*CRNTI MAC control elememt*/
                    msg3Crnti = (((*dataPtr_p) << 8 )   | *(dataPtr_p + 1));
                    dataPtr_p += 2;
                    dataPtrPos = dataPtrPos + length;

                    crntiPresentFlag = TRUE;
                }
                break;

                case MAC_LCID_1:
                case MAC_LCID_2:
                case MAC_LCID_3:
                case MAC_LCID_4:
                case MAC_LCID_5:
                case MAC_LCID_6:
                case MAC_LCID_7:
                case MAC_LCID_8:
                case MAC_LCID_9:
                case MAC_LCID_10:
                {
                    RLCUplinkDataInfo     *rlcLCIdData_p      = PNULL;
                    UInt8 *rlcData_p = PNULL;
                    
                    if(TRUE != crntiPresentFlag)
                    {
                        dataPtr_p += length;
                        dataPtrPos = dataPtrPos + length;
                        
                        LOG_MAC_MSG(MSG3_DATA_MSG_NOCRNTI_CE_ID,
                                LOGERROR,
                                MAC_RACH,
                                getCurrentTick(),
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"MSG3_DATA_MSG_NOCRNTI_CE_ID");
                       
                        break;   
                    }
                    
                    /*CA Changes start  */
                    rntiInfo_p = getUeIdxFromRNTIMap(msg3Crnti, internalCellIndex);
                    /*CA Changes end  */

                    if ((PNULL == rntiInfo_p)||
                            (CRNTI != rntiInfo_p->rntiFlag) ||
                            ((TRUE == dlUECtxInfoArr_g[rntiInfo_p->index].pendingDeleteFlag)&&
                         /* SPR 10614 changes start */
                         /* SPR 3043 start */
                         ((!(dlUECtxInfoArr_g[rntiInfo_p->index].dlUEContext_p)) || 
                          (TRUE != dlUECtxInfoArr_g[rntiInfo_p->index].dlUEContext_p->tcrntiFlag))))
                         /* SPR 3043 end */
                         /* SPR 10614 changes end */
                    {
                        dataPtr_p += length;
                        dataPtrPos = dataPtrPos + length;
                        if(PNULL != rntiInfo_p)
                        {
/*533-GCC-warning-fix*/
#ifdef LOG_PRINT_ENABLED
                            msg3RntiFlag = rntiInfo_p->rntiFlag;
                            pendingDeleteFlag = 
                                dlUECtxInfoArr_g[rntiInfo_p->index].pendingDeleteFlag;
#endif
/*533-GCC-warning-fix*/
                            LOG_MAC_MSG(MSG3_CRNTI_INCORRECT_STATE_ID,
                                    LOGERROR,
                                    MAC_RACH,
                                    getCurrentTick(),
                                    msg3Crnti,
                                    msg3RntiFlag,
                                    pendingDeleteFlag,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"RNTI_STATE_INCORRECT");
                           
                        }
                        else
                        {    
                            LOG_MAC_MSG(MSG3_CRNTI_INCORRECT_STATE_ID,
                                    LOGERROR,
                                    MAC_RACH,
                                    getCurrentTick(),
                                    msg3Crnti,
                                    msg3RntiFlag,
                                    pendingDeleteFlag,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"NO_RNTI_INFO");
                        }
                       
                        break;   
                    }

                    ueIndex = rntiInfo_p->index;

                    ulCntxt_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
                    dlCntxt_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
                   
                    if((PNULL == dlCntxt_p) || (PNULL == ulCntxt_p)) 
                    {

                        dataPtr_p += length;
                        dataPtrPos = dataPtrPos + length;
                        if(PNULL == dlCntxt_p)
                        {    
                            LOG_MAC_MSG(NO_HO_DL_UE_CONTEXT_ID,
                                    LOGERROR,
                                    MAC_RACH,
                                    getCurrentTick(),
                                    ueIndex,
                                    msg3Crnti,
                                    LINE_NUMBER,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"NO_DL_UE_CONTEXT_ID");
                        }
                        else
                        {
                            LOG_MAC_MSG(NO_HO_DL_UE_CONTEXT_ID,
                                    LOGERROR,
                                    MAC_RACH,
                                    getCurrentTick(),
                                    ueIndex,
                                    msg3Crnti,
                                    LINE_NUMBER,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"NO_UL_UE_CONTEXT_ID");
                        }
                       
                        break;   
                       
                    }
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
                    /* SPR 5487 Start */
                    if ((lcIdx >= MAX_NUM_UL_PKTS) || (INVALID_LCID == \
                        ulCntxt_p->lcInfo[lchId].lchId))
#else
                    if ((lcIdx >= MAX_NUM_UL_PKTS) || (INVALID_LCID == \
                        dlCntxt_p->logicalChannel[lchId].logicalChannelId))
                    /* SPR 5487 End */
#endif
                    {
                        if (rlcFlag)
                        {
                            SPLIT_ZERO_BUFFER_COPY(org_p, rlcData_p, dataPtrPos);
                            msgFree(org_p);
                            org_p = (void *)rlcData_p;
                            dataPtrPos = length;
                        }
                        else
                        {
                            dataPtrPos = dataPtrPos + length;
                        }

                        LOG_MAC_MSG(MSG3_CRNTI_INCORRECT_STATE_ID,
                                LOGERROR,
                                MAC_RACH,
                                getCurrentTick(),
                                msg3Crnti,
                                msg3RntiFlag,
                                pendingDeleteFlag,
                                dlCntxt_p->logicalChannel[lchId].logicalChannelId,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"LCID_NOT_CONFIGURED");
                       
                        break;   
                    }
                    else
                    {
                        /*data arrrived for RLC*/
                        if (PNULL == dataToRlc_p)
                        {
                            GET_MEM_FROM_POOL(RLCUplinkData,dataToRlc_p,sizeof(RLCUplinkData),PNULL);
                            /* SPR 1753 changes start */
                        }    
                        /* SPR 1753 changes end */    
                        SPLIT_ZERO_BUFFER_COPY(org_p, rlcData_p, 
                                dataPtrPos);
                        org_p = (void *)rlcData_p;
                        dataPtrPos = length;

                        rlcLCIdData_p = &dataToRlc_p->rlcDataArr[lcIdx];

                        /* fill the struct for sending data to RLC*/
                        rlcLCIdData_p->rlcdataBuffer = rlcData_p;
                        rlcLCIdData_p->dataLength    = length;
                        rlcLCIdData_p->lcId          = lchId;
                        lcIdx++;
                        dataToRlc_p->numLCInfo = lcIdx;
                        rlcFlag = TRUE;

                        LOG_MAC_MSG(MSG3_CRNTI_DATA_RECVD_ID,
                                LOGBRIEF,
                                MAC_RACH,
                                getCurrentTick(),
                                msg3Crnti,
                                ueIndex,
                                lchId,
                                length,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"MSG3_CRNTI_DATA_RECVD_ID");
                       
                    }
                    /*increment dataPtr_p by len so that dataPtr_p reaches 
                     *to start of next RLC SDU.
                     */
                    dataPtr_p += length;
                }    
                break;
               /* Review comment fix start LJ4 */
                case MAC_POWER_HEAD_LCH:
                {
                    /* Warning fix - The variable 'pwrHeadRoom' is not 
                     * being used */
                    /*UInt8 pwrHeadRoom     = 0;
                    pwrHeadRoom = *dataPtr_p & 0x3F;*/ /*PH is 6 bits*/
                    dataPtrPos = dataPtrPos +length;
                    dataPtr_p += length;

                }
                break;
               /* Review comment fix end LJ4 */
                /* SPR 15093 fix start */   
                case MAC_TRUNCATED_BSR:
                case MAC_S_BSR_LCH:
                    {
                         if(TRUE != crntiPresentFlag)
                         {
                             dataPtr_p += length;
                             dataPtrPos = dataPtrPos + length;

                             LOG_MAC_MSG(MSG3_DATA_MSG_NOCRNTI_CE_ID,
                                     LOGERROR,
                                     MAC_RACH,
                                     getCurrentTick(),
                                     DEFAULT_INT_VALUE,
                                     DEFAULT_INT_VALUE,
                                     DEFAULT_INT_VALUE,
                                     DEFAULT_INT_VALUE,
                                     DEFAULT_INT_VALUE,
                                     DEFAULT_FLOAT_VALUE,
                                     DEFAULT_FLOAT_VALUE,
                                     FUNCTION_NAME,"MSG3_S_BSR_NOCRNTI_CE_ID");

                             break;   
                         }
        
                        rntiInfo_p = getUeIdxFromRNTIMap(msg3Crnti, internalCellIndex);

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
                            dataPtr_p += length;
                            dataPtrPos = dataPtrPos + length;
                            if(PNULL != rntiInfo_p)
                            {
                                /*533-GCC-warning-fix*/
#ifdef LOG_PRINT_ENABLED
                                msg3RntiFlag = rntiInfo_p->rntiFlag;
                                pendingDeleteFlag = 
                                    dlUECtxInfoArr_g[rntiInfo_p->index].pendingDeleteFlag;
#endif
                                /*533-GCC-warning-fix*/
                                LOG_MAC_MSG(MSG3_CRNTI_INCORRECT_STATE_ID,
                                        LOGERROR,
                                        MAC_RACH,
                                        getCurrentTick(),
                                        msg3Crnti,
                                        msg3RntiFlag,
                                        pendingDeleteFlag,
                                        DEFAULT_INT_VALUE,
                                        DEFAULT_INT_VALUE,
                                        DEFAULT_FLOAT_VALUE,
                                        DEFAULT_FLOAT_VALUE,
                                        FUNCTION_NAME,"RNTI_STATE_INCORRECT");

                            }
                            else
                            {    
                                LOG_MAC_MSG(MSG3_CRNTI_INCORRECT_STATE_ID,
                                        LOGERROR,
                                        MAC_RACH,
                                        getCurrentTick(),
                                        msg3Crnti,
                                        msg3RntiFlag,
                                        pendingDeleteFlag,
                                        DEFAULT_INT_VALUE,
                                        DEFAULT_INT_VALUE,
                                        DEFAULT_FLOAT_VALUE,
                                        DEFAULT_FLOAT_VALUE,
                                        FUNCTION_NAME,"NO_RNTI_INFO");
                            }

                            break;   
                        }

                        ueIndex = rntiInfo_p->index;

                        ulCntxt_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
                        dlCntxt_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
                        /*short BSR*/
                        shortBSRLcgId = updateShortOrTruncatedBSR(ulCntxt_p,
                                dataPtr_p,
                                thdIndex,
                                internalCellIndex
                                /* SPR 16916 FIX START */
#ifdef KPI_STATS
                                ,&lcgTrigIndexForKpi,
                                addTrigLCGNodeForKpi 
#endif
                                /* SPR 16916 FIX END */
                                );
                        if (LCGID_INVALID_VALUE == shortBSRLcgId)
                        {
                            LTE_MAC_UT_LOG(LOG_DETAIL,DEMUX,
                                    "[%s]: ERROR: Invalid short BSR LCGID receivd\n",
                                    __func__);
                        }
                        dataPtrPos = dataPtrPos + length;
                        /*increment data pointer to point to next SDU*/
                        dataPtr_p += length;
 
                    }
                    break;

                case MAC_L_BSR_LCH:
                    {
                        if(TRUE != crntiPresentFlag)
                        {
                            dataPtr_p += length;
                            dataPtrPos = dataPtrPos + length;

                            LOG_MAC_MSG(MSG3_DATA_MSG_NOCRNTI_CE_ID,
                                    LOGERROR,
                                    MAC_RACH,
                                    getCurrentTick(),
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_INT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    DEFAULT_FLOAT_VALUE,
                                    FUNCTION_NAME,"MSG3_L_BSR_NOCRNTI_CE_ID");

                            break;   
                        }

                        rntiInfo_p = getUeIdxFromRNTIMap(msg3Crnti, internalCellIndex);

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
                            dataPtr_p += length;
                            dataPtrPos = dataPtrPos + length;
                            if(PNULL != rntiInfo_p)
                            {
                                /*533-GCC-warning-fix*/
#ifdef LOG_PRINT_ENABLED
                                msg3RntiFlag = rntiInfo_p->rntiFlag;
                                pendingDeleteFlag = 
                                    dlUECtxInfoArr_g[rntiInfo_p->index].pendingDeleteFlag;
#endif
                                /*533-GCC-warning-fix*/
                                LOG_MAC_MSG(MSG3_CRNTI_INCORRECT_STATE_ID,
                                        LOGERROR,
                                        MAC_RACH,
                                        getCurrentTick(),
                                        msg3Crnti,
                                        msg3RntiFlag,
                                        pendingDeleteFlag,
                                        DEFAULT_INT_VALUE,
                                        DEFAULT_INT_VALUE,
                                        DEFAULT_FLOAT_VALUE,
                                        DEFAULT_FLOAT_VALUE,
                                        FUNCTION_NAME,"RNTI_STATE_INCORRECT");

                            }
                            else
                            {    
                                LOG_MAC_MSG(MSG3_CRNTI_INCORRECT_STATE_ID,
                                        LOGERROR,
                                        MAC_RACH,
                                        getCurrentTick(),
                                        msg3Crnti,
                                        msg3RntiFlag,
                                        pendingDeleteFlag,
                                        DEFAULT_INT_VALUE,
                                        DEFAULT_INT_VALUE,
                                        DEFAULT_FLOAT_VALUE,
                                        DEFAULT_FLOAT_VALUE,
                                        FUNCTION_NAME,"NO_RNTI_INFO");
                            }

                            break;   
                        }

                        ueIndex = rntiInfo_p->index;

                        ulCntxt_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
                        dlCntxt_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
                        /* it's a Long BSR*/
                        dataPtr_p = updateLongBSR(ulCntxt_p,
                                dataPtr_p,
                                thdIndex,
                                internalCellIndex
                                /* SPR 16916 FIX START */
#ifdef KPI_STATS
                                ,&lcgTrigIndexForKpi,
                                addTrigLCGNodeForKpi 
#endif
                                /* SPR 16916 FIX END */
                                );
                        /*increment data pointer to point to next SDU*/
                        //dataPtr_p++;
                        dataPtr_p += length;
                        dataPtrPos = dataPtrPos + length;
                    }    
                    break;
                /* SPR 15093 fix end */   

                default:
                {
                    /*increment data pointer to point to next SDU*/
                    dataPtr_p+=length;
                    dataPtrPos = dataPtrPos + length;
                }    
            }
        }
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        if (lcgTrigIndexForKpi)
        {
            macUpdateActiveUlUePerQciInfo(lcgTrigIndexForKpi,ulCntxt_p,addTrigLCGNodeForKpi,internalCellIndex);
        }
#endif
        /* SPR 16916 FIX END */

        /*Free the end padding zero copy buffer*/
        UInt8 *afterRlcBuff_p =  PNULL;
        SPLIT_ZERO_BUFFER_COPY(org_p, afterRlcBuff_p,
                               dataPtrPos);
        msgFree(afterRlcBuff_p);

        if (crntiPresentFlag)
        {
                if (TRUE == rlcFlag)
                {
                    /* + Coverity_31438 */ 
                    /* +- SPR 17777 */
/*SPR 21179 Start*/
#ifdef SSI_DEBUG_STATS
/*SPR 21179 End*/
                    lteMacUpdateStatsRlcTx(dataToRlc_p);
                    /* +- SPR 17777 */
/*SPR 21179 Start*/
#endif 
/*SPR 21179 End*/
                    /* - Coverity_31438 */ 
                    if (!(macUeDataInd(ueIndex, dataToRlc_p)))
                    {
                        LOG_MAC_MSG(MAC_UE_DATA_IND_ERR_ID,
                                LOGERROR,
                                MAC_RACH,
                                getCurrentTick(),
                                msg3Crnti,
                                ueIndex,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                DEFAULT_FLOAT_VALUE,
                                FUNCTION_NAME,"MAC_UE_DATA_IND_ERR_ID");
                       
                    }
                }

          msg = MSG3WITHCRNTI; 
        }
        else if (ccchPresentFlag)
        {
            msg = MSG3WITHCCCH;
        }

        /*CA Changes start  */
        putEntryInRachDemuxQueue(msg, tcRNTI, msg3Crnti, receivedCCCH_Msg, msgLength, ttiCounter, dlDelay, internalCellIndex);
        /*CA Changes end  */
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : decodeAndSendCCCHMsgToRRC
 * Inputs         : dataPtrPos_p - Position of the pointer,
 *                  org_p        -
 *                  length_p     - Length of the CCCH SDU,
 *                  rnti      - UE Id,
 *                  ueIndex - Index of the UE
 *                  internalCellIndex - cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This will decode and send the ccch message to RRC.
 *****************************************************************************/
void decodeAndSendCCCHMsgToRRC( UInt32 *dataPtrPos_p,
        void   **org_p,
        UInt32 *length_p,
        UInt16 rnti,
        UInt16  ueIndex, 
        InternalCellIndex internalCellIndex)
{
    /*Its a control PDU*/
    UInt8 *msg_p = PNULL;
    UInt16 timingAdvance;
    UInt8 sinr;
    ULUEContext *ulCntxt_p   = PNULL;
    DLUEContext *dlCntxt_p   = PNULL;

    ulCntxt_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
    dlCntxt_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

    sinr = ulCntxt_p->csiContext.averageSINR;
    timingAdvance = dlCntxt_p->timingAdvanceValue;

    LTE_MAC_UT_LOG(LOG_DETAIL,DEMUX,
            "[%s]:(CCCH) Control PDU present\n",__func__);

    void *ccchData_p = PNULL;

    LTE_MAC_UT_LOG(LOG_DETAIL,DEMUX,
            "[%s]:DATA PTR CURR POS IS : %d\n",
            __func__,*dataPtrPos_p);

    ccchData_p = msgAlloc(PNULL, 0, 0, 0);
    /** SPR 14758 Changes Start **/
    if( 0 == msgSplit(*org_p, *dataPtrPos_p, (void *)ccchData_p))
    {
        /* + SPR 16766 */
        LOG_MAC_MSG( LTE_RLC_ADD_DFE_TO_AM_PDU_2, LOGWARNING, MAC_L1_INF,
                getCurrentTick(), __LINE__, ueIndex, rnti, 0,0,
                0, 0, FUNCTION_NAME,"");
        /* - SPR 16766 */
        msgFree(ccchData_p);
        ccchData_p = PNULL;
        return;
    }
    /** SPR 14758 Changes End **/

    *org_p = (void *)ccchData_p;
    *dataPtrPos_p = *length_p;
    msg_p = ccchData_p;

    {
        sendCCCHMsgToRRC(msg_p, *length_p, rnti, timingAdvance, sinr, internalCellIndex);
    }
    msgFree(msg_p);
}



