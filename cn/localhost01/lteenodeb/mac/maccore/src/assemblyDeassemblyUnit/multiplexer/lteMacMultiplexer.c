/******************************************************************************
		    *lcCountOfTB_p = 0;
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacMultiplexer.c,v 1.1.1.1.6.1.4.2.2.3 2010/10/11 12:57:02 gur10121 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the function related to the multiplexing 
 *                     of the logical channel.
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacMultiplexer.c,v $
 * 
 * Revision 1.1.1.1.6.1.4.2.2.4  2012/12/03 12:57:02  gur31292
 * Changes done for Performance Stats
 *
 * Revision 1.1.1.1.6.1.4.2.2.3  2010/10/11 12:57:02  gur10121
 * SPR #706 fix
 *
 * Revision 1.1.1.1.6.1.4.2.2.2.4.1  2010/10/08 07:42:46  gur10121
 *  SPR 706 Fix - KlockWork Warnings removed
 *
 * Revision 1.1.1.1.6.1.4.2.2.2  2010/09/29 05:08:47  gur04640
 * scheduled tick passed to RLC
 *
 * Revision 1.1.1.1.6.1.4.2.2.1  2010/09/21 15:46:02  gur20491
 * FAPI changes
 *
 * Revision 1.76.20.3  2010/08/30 13:48:58  gur24420
 * Updated RLC AM fixes
 *
 * Revision 1.76.20.2  2010/08/09 15:04:28  gur23951
 * INITIALISED HARQ FAILURE INFO IN TRANSMISSION RLC DATA
 *
 * Revision 1.76.20.1  2010/07/21 09:15:13  gur24420
 * Aligned changes with respect to eNode Integration activity(based on RMI-NetLogic Code).
 * Overall Changes Cover:-
 * 1. Forced some code to go under ONE_FLOW_ONE_DELAY condition only.
 *
 * Revision 1.76  2010/04/05 12:00:21  gur20439
 * removal of warning in case of ONE_FLOW_ONE_DELAY.
 *
 * Revision 1.75  2009/11/15 00:54:28  gur18550
 * Function Modified for dual TB
 *
 * Revision 1.74  2009/11/14 13:43:26  gur18550
 * Added RESET_CCE
 *
 * Revision 1.73  2009/11/14 08:42:29  gur18550
 * Function Updated for TA DRX
 *
 * Revision 1.72  2009/11/13 14:44:31  gur18550
 * DL Harq being freed for Q Load = 0 case
 *
 * Revision 1.71  2009/11/12 19:55:04  gur18550
 * Bug solved related to Two TB
 *
 * Revision 1.70  2009/11/09 10:09:51  gur18550
 * Bug Solved for ltePanic
 *
 * Revision 1.69  2009/10/07 11:35:43  gur11912
 * optimization changes
 *
 * Revision 1.68  2009/10/07 06:01:49  gur18550
 * Added Flag
 *
 * Revision 1.67  2009/10/06 10:02:55  gur18550
 * Function Updated
 *
 * Revision 1.66  2009/10/06 09:41:41  gur18550
 * Function Optimised
 *
 * Revision 1.65  2009/10/06 06:09:45  gur11912
 * modified processDummyNode
 *
 * Revision 1.64  2009/08/17 15:20:42  gur18550
 * Logs Updated
 *
 * Revision 1.63  2009/08/17 13:05:21  gur18550
 * Function Updated
 *
 * Revision 1.62  2009/08/14 10:36:51  gur18550
 * Log Added
 *
 * Revision 1.61  2009/08/14 09:59:58  gur18550
 * Log Added @ 1731
 *
 * Revision 1.60  2009/08/13 10:21:04  gur18550
 * Warnings removed
 *
 * Revision 1.59  2009/08/12 06:57:33  gur18550
 * Log Updated
 *
 * Revision 1.58  2009/08/11 14:09:05  gur18550
 * Function Updated
 *
 * Revision 1.57  2009/08/03 06:10:08  gur18550
 * Logs Updated
 *
 * Revision 1.56  2009/07/29 14:28:58  gur11912
 * assignedRBQueueLoad updated in Mux
 *
 * Revision 1.55  2009/07/23 14:03:04  gur18569
 * UT fixes in MUX
 *
 * Revision 1.54  2009/07/22 15:54:20  gur18569
 * added UT_TESTING logs
 *
 * Revision 1.53  2009/07/21 12:57:17  gur18550
 * Function Updated
 *
 * Revision 1.52  2009/07/20 13:18:10  gur18569
 * initializing tbTwoLcStart  index in mux's transmissionAndRLCData_p
 *
 * Revision 1.51  2009/07/20 05:55:08  gur18550
 * Function Updated
 *
 * Revision 1.50  2009/07/16 08:12:50  gur18550
 * Logs updated
 *
 * Revision 1.49  2009/07/16 08:10:36  gur18550
 * Removed warning of macUeOpp
 *
 * Revision 1.48  2009/07/15 13:45:14  gur18550
 * Function Updated
 *
 * Revision 1.47  2009/07/15 05:48:43  gur18550
 * FUnctuon Updated
 *
 * Revision 1.46  2009/07/14 09:11:55  gur18550
 * Function Updated
 *
 * Revision 1.45  2009/07/14 08:30:59  gur18550
 * Function Updated
 *
 * Revision 1.44  2009/07/13 07:25:15  gur18569
 * fixed ut bug:setting padding
 *
 * Revision 1.43  2009/07/09 09:52:09  gur18550
 * Changes in Log
 *
 * Revision 1.42  2009/07/09 08:19:21  gur18550
 * Indentation
 *
 * Revision 1.41  2009/07/09 05:49:06  gur18550
 * Comments Inc
 *
 * Revision 1.40  2009/07/08 12:25:49  gur18550
 * Function Updated
 *
 * Revision 1.39  2009/07/08 12:20:29  gur18550
 * Function Updated
 *
 * Revision 1.38  2009/07/08 09:25:13  gur18550
 * Log Updated
 *
 * Revision 1.37  2009/07/08 06:49:05  gur18550
 * Function updated
 *
 * Revision 1.36  2009/07/07 09:41:02  gur18550
 * Comments Inc
 *
 * Revision 1.35  2009/07/07 08:54:24  gur18550
 * Comments Inc
 *
 * Revision 1.34  2009/07/07 08:46:46  gur18550
 * Comments Inc
 *
 * Revision 1.33  2009/07/06 07:36:22  gur18550
 * Comments Inc
 *
 * Revision 1.32  2009/07/01 06:14:07  gur18550
 * Function Updated
 *
 * Revision 1.31  2009/07/01 05:03:15  gur18550
 * Function Updated
 *
 * Revision 1.30  2009/06/29 13:11:06  gur18550
 * Function Updated
 *
 * Revision 1.29  2009/06/29 06:41:44  gur18550
 * Function Updated
 *
 * Revision 1.28  2009/06/26 07:09:07  gur18550
 * Function Introduced
 *
 * Revision 1.26  2009/06/25 07:43:53  gur18550
 * Indentaion resolved
 *
 * Revision 1.25  2009/06/25 07:35:49  gur18550
 * Error Removed
 *
 * Revision 1.24  2009/06/24 12:08:23  gur18550
 * Final Review Comments Inc
 *
 * Revision 1.22  2009/06/18 12:18:02  gur18550
 * Pending Delete related Changes
 *
 * Revision 1.21  2009/06/18 10:03:23  gur18550
 * CCE related Changes
 *
 * Revision 1.20  2009/06/18 07:45:57  gur18569
 * changes in handling dummy node
 *
 * Revision 1.19  2009/06/18 06:17:29  gur18550
 * Function Update
 *
 * Revision 1.18  2009/06/17 14:53:27  gur18550
 * MacUeOpp.....
 *
 * Revision 1.17  2009/06/17 14:42:33  gur18550
 * Added Feature for RLC HEADER
 *
 * Revision 1.16  2009/06/17 05:40:13  gur18550
 * *** empty log message ***
 *
 * Revision 1.15  2009/06/16 14:15:41  gur18569
 * added and argument to processMultiplexerQueue , delay to phy
 *
 * Revision 1.14  2009/06/16 13:57:03  gur18550
 * dualTB changes
 *
 * Revision 1.13  2009/06/16 13:35:46  gur18550
 * AllocTb
 *
 * Revision 1.12  2009/06/16 12:22:15  gur18569
 * allocating memory for transmissionAndRlcDataBuf for dummy node also
 *
 * Revision 1.11  2009/06/15 14:50:11  gur18569
 * changes in handling dummy msg from scheduler
 *
 * Revision 1.10  2009/06/11 10:00:41  gur18550
 * cleanUPMux added
 *
 * Revision 1.9  2009/06/11 09:23:10  gur18550
 * InitMux added
 *
 * Revision 1.8  2009/06/11 06:04:10  gur18569
 * added fourth arg in macUeOpputunityInd
 *
 * Revision 1.7  2009/06/10 04:52:37  gur18550
 * Process Dummy Node Modified
 *
 * Revision 1.6  2009/06/08 11:24:40  gur18550
 * Comments Inc.
 *
 * Revision 1.5  2009/06/08 09:36:48  gur18550
 * Comments Inc.
 *
 * Revision 1.4  2009/06/05 07:06:17  gur19413
 * Removed Compilation Erros
 *
 * Revision 1.3  2009/06/01 10:55:48  gur18550
 * File Updated
 *
 * Revision 1.2  2009/06/01 09:47:13  gur18550
 * Warnings Removed
 *
 * Revision 1.1  2009/04/30 05:11:26  gur18550
 * INITIAL VERSION
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacCellConfMgr.h"
#include "lteMacMultiplexer.h"
/* FDD_Warning_Fix */
#include "lteMacDLHarqMgr.h"
/* FDD_Warning_Fix */
#include "lteMacDLSchedulerStrategy.h"
#include "lteMacDLSpsScheduler.h"
/* Start 128 UE: Memory Changes */
#ifdef TDD_CONFIG
#include "lteMacTddMgr.h"
#endif
/* End 128 UE: Memory Changes */
 
/******************************************************************************
 * Private Definitions
 *****************************************************************************/



/******************************************************************************
 Private Types
 *****************************************************************************/

 /*****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
LTE_SQUEUE multiplexerQueue_g[MAX_NUM_CELL];

/*START_DYN_PDCCH_TDD_CHANGE*/
extern LTE_SEM cellSyncSem_g;
/*END_DYN_PDCCH_TDD_CHANGE*/
/*START : DYN_PDCCH*/
extern UInt8 currentCFIValue_g[MAX_NUM_CELL];/*DYN_PDCCH*/
/*END   : DYN_PDCCH*/
STATIC void macDlRetraverseForPaddingRemove( UInt32 *availableTBSize,
                                                 TransmissionAndRLCData* transmissionAndRLCData_p,
                                                 DLUEContext *ueContext_p,
                                                 UInt32 arrayIndex);

STATIC  MacRetType fillRLCDataBufferMacUEIndForSingleTB(  
            DLUEContext *ueContext_p ,
            DLQosStrategyLCNode *dlQosStrategyLCNode_p,
            UInt32 *availableTBSize_p,
            RLCDataBuffer * rlcDataBufferArr_p,
            UInt8 *lcCount_p,
            UInt8* lcCountOfTB_p,
            UInt32* paddingBytesForTB_p,
            TransmissionAndRLCData * transmissionAndRLCData_p,
            InternalCellIndex internalCellIndex,
            DLHARQProcess* dlHarqProcess_p);

/* + SPR 13605 Fix */
STATIC  void dlQoSResetCCEsAndHarqProcess( DLUEContext *ueContext_p,
                                                  TransmissionAndRLCData * transmissionAndRLCData_p);
/* - SPR 13605 Fix */
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/





/*****************************************************************************
 * Function Name  :  putEntryInMultiplexerQueue 
 * Inputs         :  muxQueueNode_p - A pointer to the node MultiplexerQueueNode
 *                                    which stores the various information about the 
 *                                    ueIndex/harqProcessId etc.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This fucntion will contain create the Nodes and puts them
 *                   in the multipexer Queue.
 *****************************************************************************/

 void putEntryInMultiplexerQueue(MultiplexerQueueNode  *muxQueueNode_p,
        InternalCellIndex internalCellIndex)
{

  LTE_MAC_UT_LOG (LOG_INFO, LOG_MULTIPLEXER, "[%s] Entry", __func__);
  if (pushNode (&multiplexerQueue_g[internalCellIndex],
                                    &(muxQueueNode_p->muxNodeAnchor)))
    {
      lteWarning ("pushNode() failed for " "multiplexerQueue_g queue");
      if(muxQueueNode_p)
      {
          freeMemPool (muxQueueNode_p);
      }
    }
  else
    {
      LTE_MAC_UT_LOG (LOG_INFO, "[%s]ENTRY PUT INTO MUX QUEUE %d\n", __func__,
		      muxQueueIndex);
    }
  LTE_MAC_UT_LOG (LOG_INFO, LOG_MULTIPLEXER, "[%s] Exit", __func__);
}
    
/* SPR 20254 Fix Start */
/*****************************************************************************
 * Function Name  :  handleMuxFailure
 * Inputs         :  muxQueueNode_p - A pointer to the structure Multiplexer
 *                                    QueueNode which stores the information like
 *                                    ueIndex, HarqID etc,
 *                   scheduledTick - This information is simply passed by this
 *                                   function to fetch the dciContainerIndex.
 *                   ueContext_p - The Downlink UE Context Pointer.
 *                   internalCellIndex    
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function will handle the failure scenario when
 *                   DPSCH wihthout MUX is executed and mux is failing.
 *                   Currently only Harq and cce are being freed.
 *                   Retransmission handling will be done in future if reqd.
 *****************************************************************************/
void handleMuxFailure(MultiplexerQueueNode *muxQueueNode_p,
            tickType_t scheduledTick,
            DLUEContext *dlUeContext_p,
            InternalCellIndex internalCellIndex)
{

    UInt8 dciContainerIndex = 0;
    DLHARQProcess* dlHarqProcess_p = PNULL;
    DciCCEInfo* dciCCEInfo_p = PNULL;

    dciContainerIndex = scheduledTick % MAX_CCE_CONTAINER;
    dlHarqProcess_p = &dlUeContext_p->dlHarqContext_p->harqProcess[muxQueueNode_p->harqProcessId];
    if ((PNULL != dlHarqProcess_p) && 
        (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == INVALID_TA_DRX_MSG) &&
        (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == INVALID_TA_DRX_MSG))
    {
        dciCCEInfo_p = &((dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex)->
                dciCCEInfo[muxQueueNode_p->cceIndex]);
        mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                dciCCEInfo_p,internalCellIndex);
        freeDLHarqProcess (dlUeContext_p,dlHarqProcess_p, internalCellIndex);
    }
}
/* SPR 20254 Fix End */

/*****************************************************************************
 * Function Name  :  processMultiplexerQueue
 * Inputs         :  currentGlobalTick - This value of the cuurent global tick,
 *                   arrayIndex - The index of the MUX Queue Array on  which 
 *                                this function has to work,
 *                   untilLastFlag - The flag if set then the loop waits until
 *                                   it receives the dummy node,
 *                   delayToPhy - This is the difference between the cuurent tick
 *                                on which the MUX is working and the scheduled tick
 *                                on which the PHY is to be scheduled
 *                   internalCellIndex    
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will be entry point function for the entire
 *                   when MUX would be executing as a separate entity. This function
 *                   is called by the Execution scheduler.
 *****************************************************************************/
MacRetType processMultiplexerQueue(
                            /* SPR 15909 fix start */
                            tickType_t currentGlobalTick,    
                            /* SPR 15909 fix end */
                            UInt8  untilLastFlag,
                            UInt32 delayToPhy,
                            InternalCellIndex internalCellIndex)
{
    MultiplexerQueueNode *muxQueueNode_p = PNULL;
    LTE_SQUEUE *muxQueue_p = PNULL;
    UInt8 muxNodeCount = 0;
    /* SPR 15909 fix start */
    tickType_t scheduledTick = 0;
    /* SPR 15909 fix end */
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext* dlUeContext_p = PNULL;
     /* + Critical section changes */
    UInt8 isOtherCellStateRunning = 0;
     /* - Critical section changes */
    scheduledTick = currentGlobalTick + delayToPhy;
    /* + SPR 11208 Changes */
    if (IS_CA_ENABLED())
    {
    muxQueue_p = &(multiplexerQueue_g[!internalCellIndex]);
    }
    else
    {
    muxQueue_p = &(multiplexerQueue_g[internalCellIndex]);
    }
    /* - SPR 11208 Changes */
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Entry",__func__);


    if (untilLastFlag)
    {
        while (1)
        {
            while ((muxQueueNode_p = 
                           (MultiplexerQueueNode *)popNode(muxQueue_p)) == PNULL)
            {
            }
        
            if (muxQueueNode_p->msgType == DUMMY_MSG)
            {
                /*
                This is the case when DUMMY Message has been received so MULTIPLEXER
                will stop after sending the information to RLC. 
                Also send SUCCESS indication to Execution Scheduler.
                */
                /* + SPR 11209 Changes */
                processDummyNode(scheduledTick,internalCellIndex, internalCellIndex);
                /* - SPR 11209 Changes */
                freeMemPool((void *)muxQueueNode_p);
                LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
                return MAC_SUCCESS;
            }
        
            dlUeContextInfo_p = &dlUECtxInfoArr_g[muxQueueNode_p->ueIndex];
            if (!dlUeContextInfo_p->pendingDeleteFlag) 
            {
                dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;  
                /* PDCCH_PDSCH_WITHOUT_MUX Start */
                    /*
                     * This function wrapper will call
                     * muxing function based on scheduling
                     */
                /* SPR 20254 Fix Start */
                if (MAC_FAILURE== lteLayer2MacLogicalChannelsMuxing(muxQueueNode_p, 
                            scheduledTick,
                            dlUeContext_p,
                            internalCellIndex))
                {
                    handleMuxFailure(muxQueueNode_p,
                        scheduledTick,
                        dlUeContext_p,
                        internalCellIndex);
                }
                /* SPR 20254 Fix End */
                /* PDCCH_PDSCH_WITHOUT_MUX End*/
            }
        
            freeMemPool((void *)muxQueueNode_p);
            muxQueueNode_p = PNULL;
        }
    }
    else
    {
        muxNodeCount = sQueueCount(muxQueue_p);
        while (muxNodeCount)
        {
            muxQueueNode_p = (MultiplexerQueueNode *)popNode(muxQueue_p);
            if (muxQueueNode_p)
            {
                /* + SPR 11209 Changes */
                if (!IS_CA_ENABLED())
                {
                    if (muxQueueNode_p->msgType == DUMMY_MSG)
                    {
                        /*
                           This is the case when DUMMY Message has been received so MULTIPLEXER
                           will stop after sending the information to RLC. 
                           Also send SUCCESS indication to Execution Scheduler.
                           */
                        processDummyNode(scheduledTick,internalCellIndex, internalCellIndex);
                        freeMemPool((void *)muxQueueNode_p);
                        LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
                        return MAC_SUCCESS;
                    }
                }
                /* - SPR 11209 Changes */
        
                dlUeContextInfo_p = &dlUECtxInfoArr_g[muxQueueNode_p->ueIndex];
                if (!dlUeContextInfo_p->pendingDeleteFlag) 
                {
                    dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;  
                    /* PDCCH_PDSCH_WITHOUT_MUX Start */
                    /*
                     * This function wrapper will call
                     * muxing function based on scheduling
                     */

                    /* SPR 20254 Fix Start */
                    if(MAC_FAILURE == lteLayer2MacLogicalChannelsMuxing(muxQueueNode_p,
                                scheduledTick,
                                dlUeContext_p,
                                internalCellIndex))
                    {
                        handleMuxFailure(muxQueueNode_p,
                          scheduledTick,
                            dlUeContext_p,
                            internalCellIndex);
                    }
                    /* SPR 20254 Fix End */
                    /* PDCCH_PDSCH_WITHOUT_MUX End */

                }
        
                freeMemPool((void *)muxQueueNode_p);
                muxQueueNode_p = PNULL;
                muxNodeCount--;
            }
        }
        /* + SPR 11209 Changes */
        if (IS_CA_ENABLED())
        {
            processDummyNode(scheduledTick,internalCellIndex, internalCellIndex);
             /* + Critical section changes */
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
            semWait(&cellSyncSem_g); 
#endif 
            isOtherCellStateRunning = CHECK_CELL_STATE(!internalCellIndex);

#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
            semPost(&cellSyncSem_g);
#endif 

            if (isOtherCellStateRunning)
            {
                processDummyNode(scheduledTick,!(internalCellIndex), internalCellIndex);
            }
            /* - Critical section changes */
        }
        /* - SPR 11209 Changes */
        LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
        return MAC_FAILURE;
    }
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
    return MAC_FAILURE;
}

/*****************************************************************************
 * Function Name  :  processDummyNode 
 * Inputs         :  scheduledTick - This information is simply passed by this
 *                                  function and is used by the encoder functionality.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function is fills the RLC data buffer with a dummy node
 *                   and forwards it to the RLC, this information is used by the 
 *                   Encoder to stop its processing.
 *****************************************************************************/
 /** SPR 13599 Changes Start **/
            /* SPR 22936 Start */
#ifdef DL_DATA_SEPARATION
TransmissionAndRLCData *transmissionAndRLCData_g[MAX_NUM_CELL][MAX_NUM_CELL];
#else
    TransmissionAndRLCData *transmissionAndRLCData_g[MAX_NUM_CELL];
#endif
            /* SPR 22936 End */
 /** SPR 13599 Changes End **/

/* + SPR 11209 Changes */
                             /* SPR 15909 fix start */
 void processDummyNode( tickType_t scheduledTick,
                             /* SPR 15909 fix end */
                              InternalCellIndex encoderCellIndex,
                              InternalCellIndex rlcCellIndex)
/* - SPR 11209 Changes */
{
    /** SPR 13599 Changes Start **/
            /* SPR 22936 Start */
#ifdef DL_DATA_SEPARATION
    TransmissionAndRLCData *transmissionAndRLCData_p = transmissionAndRLCData_g[rlcCellIndex][encoderCellIndex];
#else
    TransmissionAndRLCData *transmissionAndRLCData_p = transmissionAndRLCData_g[rlcCellIndex];
#endif
            /* SPR 22936 End */
    /** SPR 13599 Changes End **/
    UInt8 zeroLCCount = 0;
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Entry",__func__);
    /** In case of Dummy Node,transmissionAndRLCData_p is globally Allocated,
     * So no need to allocate memory for every dummy Node */ 
   

    /* SPR 5633 changes start */
    if(PNULL != transmissionAndRLCData_p)             
    {
    /* SPR 5633 changes end */
        transmissionAndRLCData_p->scheduledTick = scheduledTick;
        /* CA-Ph2 Integration fix start */
/* + SPR 11209 Changes */
        transmissionAndRLCData_p->encoderCellIndex = encoderCellIndex;
/* - SPR 11209 Changes */
        /* CA-Ph2 Integration fix end */

/* + SPR 11209 Changes */
        macUeOpportunityInd(INVALID_UE_ID, zeroLCCount, 
                (RLCDataBuffer*) transmissionAndRLCData_p,scheduledTick,rlcCellIndex);
/* - SPR 11209 Changes */
    /* SPR 5633 changes start */
    }
    /* SPR 5633 changes end */
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
}


/*****************************************************************************
 * Function Name  :  multiplexLogicalChannelsForUE 
 * Inputs         :  muxQueueNode_p - A pointer to the structure MultiplexerQueueNode
 *                                    which stores the information like ueIndex HarqID etc.
 *                   scheduledTick - This information is simply passed by this
 *                                   function and is used by the encoder functionality.
 *                   ueContext_p - The Downlink UE Context Pointer.
 *                   internalCellIndex 
 * Outputs        :  None 
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function is called when the non dummy nodes are to be 
 *                   sent to the RLC after multiplexing, in fact this function
 *                   will initate the entire logic of multiplexing depending upon
 *                   the MIMO/Non-MIMO case. 
 *****************************************************************************/
/* SPR 4168 */
MacRetType multiplexLogicalChannelsForUE(
                           MultiplexerQueueNode  * muxQueueNode_p,
                           /* SPR 15909 fix start */
                           tickType_t scheduledTick,
                           /* SPR 15909 fix end */
                           DLUEContext* ueContext_p,
                           InternalCellIndex internalCellIndex)
/* SPR 4168 */
{

    DLHARQProcess* dlHarqProcess_p = PNULL;
    DLHARQTBInfo* dlHarqTbOneInfo_p = PNULL;
    DLHARQTBInfo* dlHarqTbTwoInfo_p = PNULL;   
    TransmissionAndRLCData * transmissionAndRLCData_p = PNULL;
    /* SPR 4168 */
    MacRetType retType = MAC_SUCCESS;
    /* SPR 4168 */
    /* _RLC_AM_MODE_ */
#ifdef  OBSELETE_REL_8    
    RLCHarqFailureInfo* tempRlcHarqFailureInfo = PNULL;
    UInt8 ii = 0;
#endif    
    /* SPR 4219 start */
    UInt8   containerTick = 0;
    DciCCEInfo                  *pDciCceInfo_p = PNULL;
    DCIPduInfo                  *pDciPduInfo_p = PNULL;
    /* SPR 20254 Fix +- */
    /* SPR 4219 end */
    GET_MEM_FROM_POOL(TransmissionAndRLCData,transmissionAndRLCData_p,sizeof(TransmissionAndRLCData),PNULL);
    if (PNULL == transmissionAndRLCData_p)
    {
	    return MAC_FAILURE ;
    }
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Entry",__func__);



#ifdef  OBSELETE_REL_8    
    /* _RLC_AM_MODE_ */
    tempRlcHarqFailureInfo = &transmissionAndRLCData_p->rlcHarqFailureInfo[0];
    for (ii=0; ii < ueContext_p->maxLcId; ii++)
    {
        tempRlcHarqFailureInfo[ii].lcId=INVALID_LCID;
        /*  RLC - ReArch changes start */
        tempRlcHarqFailureInfo[ii].newPduSeqNum=INVALID_SEQ_NUM; 
        /*  RLC - ReArch changes end */
        tempRlcHarqFailureInfo[ii].SOStart=INVALID_SOS_START;
        tempRlcHarqFailureInfo[ii].SOEnd=INVALID_SOS_END;
        tempRlcHarqFailureInfo[ii].harqFailureReporting=HARQ_FAILURE_REPORTING_OFF;
    } 
#endif    
    transmissionAndRLCData_p->endIndication = 0;
    transmissionAndRLCData_p->tbTwoLCStartIndex = 0; 
    transmissionAndRLCData_p->numValidTBs = 0;

    dlHarqProcess_p = 
        &ueContext_p->dlHarqContext_p->harqProcess[muxQueueNode_p->harqProcessId];
    dlHarqTbOneInfo_p = &dlHarqProcess_p->dlHARQTBOneInfo;
    dlHarqTbTwoInfo_p = &dlHarqProcess_p->dlHARQTBTwoInfo;

    transmissionAndRLCData_p->cceIndex = muxQueueNode_p->cceIndex;
    transmissionAndRLCData_p->harqProcessId = muxQueueNode_p->harqProcessId;
    transmissionAndRLCData_p->scheduledTick = scheduledTick;

    /* CA-Ph2 Integration fix start */
/* + SPR 11209 Changes */
    transmissionAndRLCData_p->encoderCellIndex = internalCellIndex;
/* - SPR 11209 Changes */

    /* CA-Ph2 Integration fix end */
    /*
       The first two cases checks for both MIMO and Non-MIMO cases as in the
       former it may be possible that the second code word has been disabled
       so we may need to send only one TB, while in the case of latter it is 
       always TB_ONE only.
     */
    if ((dlHarqProcess_p->isTBOneValid) && (!dlHarqProcess_p->isTBTwoValid))    
    {
        /*transmissionStatusFlag = TB_ONE_VALID_AND_TB_TWO_INVALID;*/
        transmissionAndRLCData_p->sizeTBOne = dlHarqProcess_p->dlHARQTBOneInfo.tbSize;
        transmissionAndRLCData_p->sizeTBTwo = 0;
        transmissionAndRLCData_p->paddingBytesForTBTwo = 0; 
        transmissionAndRLCData_p->lcCountTBTwo = 0;
        dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = transmissionAndRLCData_p;

        /* SPR 4168 */
        retType =  singleTBDistribution( 
                muxQueueNode_p,
                transmissionAndRLCData_p,
                ueContext_p ,
                dlHarqProcess_p,
                /* +- SPR 17777 */
                TB_ONE, internalCellIndex);
                /* +- SPR 17777 */
        if (MAC_FAILURE == retType )
        {
            /* SPR 16766 fix start */
            LOG_MAC_MSG(MAC_MUX_ENCODE_FAIL_ID, LOGWARNING, MAC_MUX,
                    getCurrentTick(), ueContext_p->ueIndex,
                    ueContext_p->crnti, 0,0,0, 0,0, __func__,
                    "retFail1SingleTBDistribution");
            /* SPR 16766 fix end */
            /* SPR 20254 Fix +- */
            /* Reset CCE and Retx will be done in processPDSCHFailure() */
            freeMemPool(transmissionAndRLCData_p);
            dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
        }
        /* SPR 4168 */
    }               
    else if ((!dlHarqProcess_p->isTBOneValid) && (dlHarqProcess_p->isTBTwoValid))         
    {
        /*transmissionStatusFlag = TB_ONE_INVALID_AND_TB_TWO_VALID;*/
        transmissionAndRLCData_p->sizeTBOne = 0;
        transmissionAndRLCData_p->paddingBytesForTBOne = 0; 
        transmissionAndRLCData_p->lcCountTBOne = 0;
        transmissionAndRLCData_p->sizeTBTwo = dlHarqProcess_p->dlHARQTBTwoInfo.tbSize;
        dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = transmissionAndRLCData_p;

        /* SPR 4168 */
        retType = singleTBDistribution( 
                muxQueueNode_p,
                transmissionAndRLCData_p,
                ueContext_p ,
                dlHarqProcess_p,
                /* +- SPR 17777 */
                TB_TWO,internalCellIndex);
                /* +- SPR 17777 */
        if (MAC_FAILURE == retType )
        {
            /* SPR 16766 fix start */
            LOG_MAC_MSG(MAC_MUX_ENCODE_FAIL_ID, LOGWARNING, MAC_MUX,
                    getCurrentTick(), ueContext_p->ueIndex,
                    ueContext_p->crnti, 0,0,0, 0,0, __func__,
                    "retFail2SingleTBDistribution");
            /* SPR 16766 fix end */
            /* SPR 20254 Fix +- */
            /* Reset CCE and Retx will be done in processPDSCHFailure() */
            freeMemPool(transmissionAndRLCData_p);
            dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;
        }
        /* SPR 4168 */
    }
    /*
       Now we deal with the MIMO case in which also there can be multiple scenarios
       related to new- and re- transmissions of the TBs.
     */
    else if ((dlHarqProcess_p->isTBOneValid) && (dlHarqProcess_p->isTBTwoValid))
    {
        transmissionAndRLCData_p->sizeTBOne = 0;
        transmissionAndRLCData_p->paddingBytesForTBOne = 0; 
        transmissionAndRLCData_p->lcCountTBOne = 0;
        transmissionAndRLCData_p->sizeTBTwo = 0;
        transmissionAndRLCData_p->paddingBytesForTBTwo = 0; 
        transmissionAndRLCData_p->lcCountTBTwo = 0;

        if (dlHarqTbOneInfo_p->txIndicator == NEW_TX)
        {
            if (dlHarqTbTwoInfo_p->txIndicator == NEW_TX)
            {
                /*transmissionStatusFlag = TB_ONE_VALID_AND_TB_TWO_VALID;*/
                /* SPR 4168 */
                retType =  dualTBDistribution( 
                        muxQueueNode_p, 
                        transmissionAndRLCData_p,
                        dlHarqProcess_p,
                        ueContext_p,
                        internalCellIndex);
                if (MAC_FAILURE == retType )
                {
                    /* SPR 16766 fix start */
                    LOG_MAC_MSG(MAC_MUX_ENCODE_FAIL_ID, LOGWARNING, MAC_MUX,
                            getCurrentTick(), ueContext_p->ueIndex,
                            ueContext_p->crnti, 0,0,0, 0,0, __func__,
                            "retFail1dualTBDistribution");
                    /* SPR 16766 fix end */
                    /* SPR 20254 Fix +- */
                    /* Reset CCE and Retx will be done in processPDSCHFailure() */
                    freeMemPool(transmissionAndRLCData_p);
                    dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
                    dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;
                }
                /* SPR 4168 */
                /* SPR 4219 start */
                if (!dlHarqProcess_p->isTBTwoValid)
                {
                    containerTick = scheduledTick % MAX_PDCCH_CONTAINER;
                    /*++coverity 31265*/
                    pDciCceInfo_p = &((dciCCEContainerInfo_gp[internalCellIndex] + containerTick)->\
                            dciCCEInfo[muxQueueNode_p->cceIndex]);
                    /*--coverity 31265*/
                    pDciPduInfo_p  = &(pDciCceInfo_p->dciPduInfo);                    
                    pDciPduInfo_p->isTBTwoValid = dlHarqProcess_p->isTBTwoValid;
                }
                /* SPR 4219 End*/
            }
            else
            {
                /*transmissionStatusFlag = TB_ONE_VALID_AND_TB_TWO_RETRANS;*/
                transmissionAndRLCData_p->sizeTBOne = dlHarqProcess_p->dlHARQTBOneInfo.tbSize;
                dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = transmissionAndRLCData_p;

                /* SPR 4168 */
                retType = singleTBDistribution( 
                        muxQueueNode_p,
                        transmissionAndRLCData_p,
                        ueContext_p ,
                        dlHarqProcess_p,
                        /* +- SPR 17777 */
                        TB_ONE,
                        /* +- SPR 17777 */
                        internalCellIndex);
                if (MAC_FAILURE == retType )
                {
                    /* SPR 16766 fix start */
                    LOG_MAC_MSG(MAC_MUX_ENCODE_FAIL_ID, LOGWARNING, MAC_MUX,
                            getCurrentTick(), ueContext_p->ueIndex,
                            ueContext_p->crnti, 0,0,0, 0,0, __func__,
                            "retFail3SingleTBDistribution");
                    /* SPR 16766 fix end */
                    /* SPR 20254 Fix +- */
                    /* Reset CCE and Retx will be done in processPDSCHFailure() */
                    freeMemPool(transmissionAndRLCData_p);
                    dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
                }
                /* SPR 4168 */
            }
        }
        else if (dlHarqTbTwoInfo_p->txIndicator == NEW_TX)
        {
            /*transmissionStatusFlag = TB_ONE_RETRANS_AND_TB_TWO_VALID;*/
            transmissionAndRLCData_p->sizeTBTwo = dlHarqProcess_p->dlHARQTBTwoInfo.tbSize;
            dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = transmissionAndRLCData_p;
            /* SPR 4168 */
            retType = singleTBDistribution( 
                    muxQueueNode_p, 
                    transmissionAndRLCData_p,
                    ueContext_p ,
                    dlHarqProcess_p,
                    /* +- SPR 17777 */
                    TB_TWO,
                    /* +- SPR 17777 */
                    internalCellIndex);

            if (MAC_FAILURE == retType )
            {
                /* SPR 16766 fix start */
                LOG_MAC_MSG(MAC_MUX_ENCODE_FAIL_ID, LOGWARNING, MAC_MUX,
                        getCurrentTick(), ueContext_p->ueIndex,
                        ueContext_p->crnti, 0,0,0, 0,0, __func__,
                        "retFail4SingleTBDistribution");
                /* SPR 16766 fix end */
                /* SPR 20254 Fix +- */
                /* Reset CCE and Retx will be done in processPDSCHFailure() */
                freeMemPool(transmissionAndRLCData_p);
                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;
            }

            /* SPR 4168 */
        }
	/*Coverity 25487 Fix Start*/
	else
	{
        /* SPR 20254 Fix Start */
        /* Reset CCE is not required as it is retransmitted */
        LOG_MAC_INFO(MAC_MUX,"Retx Both TB UE Index:[%u] Harq:[%u] \n",
        muxQueueNode_p->ueIndex,muxQueueNode_p->harqProcessId); 
        /* SPR 20254 Fix End */
	    freeMemPool(transmissionAndRLCData_p);
	} 
	/*Coverity 25487 Fix End*/
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_DETAIL, LOG_MULTIPLEXER, \
                "\n The HARQ Process is INVALID NULL POINTER RECEIVED for the "
                "\n UE INDEX = %d, and the Harq Process ID = %d", muxQueueNode_p->ueIndex, 
                muxQueueNode_p->harqProcessId);
        /* SPR 20254 Fix Start */
        /* Reset CCE is not required This case should not hit */
        LOG_MAC_ERROR(MAC_MUX,"INVALID NULL POINTER RECEIVED UE Index:[%u] Harq:[%u] \n",
        muxQueueNode_p->ueIndex,muxQueueNode_p->harqProcessId); 
        /* SPR 20254 Fix End */
        /*Coverity 25487 Fix Start*/
        freeMemPool(transmissionAndRLCData_p);
        /*Coverity 25487 Fix End*/
    }
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
    /* SPR 4168 */
    return retType; 
} /*end of Function*/


/*****************************************************************************
 * Function Name  :  generateSortedListOfLCs 
 * Inputs         :  ueContext_p -  A pointer to DLUEContext,
 *                   numOfLC - number of LC,
 *                   sortedArrayOfLC - An array of type SortedLCsLoadInfo which
 *                                     returns the LC in a sotered fornm on the 
 *                                     basis of priority,
 *                   scheduledTick - The tick which has to be passsed to the encoder.
 *                   muxQueueNode_p - A pointer of the type MultiplexerQueueNode
 *                                    which stores the inforamtion about ueIndex Harq ID etc
 *                   internalCellIndex 
 * Outputs        :  None
 * Returns        :  numOfLC - The number of LC that has been added in the sorted list. 
 * Description    :  This function generates the sorted list of LC's.
 *****************************************************************************/

/* CA Changes start */
UInt8  generateSortedListOfLCs( 
                     DLUEContext *ueContext_p,
                     UInt8 numOfLC,
                     SortedLCsLoadInfo* sortedArrayOfLC,
                     /* SPR 15909 fix start */
                     tickType_t scheduledTick,
                     /* SPR 15909 fix end */
                     MultiplexerQueueNode* muxQueueNode_p
                     ,InternalCellIndex internalCellIndex)

{
    UInt8   localNumOfLC = numOfLC;
    UInt32 count = 0;
    UInt32 sumLCPriorityAndQLoad = 0;
    UInt32 sumLCPriority = 0;
    UInt32 qLoadWeightedMeanofAllLCs = 0;
    UInt32 delayFactor = 0;
    UInt32 percentLCQLoad = 0;
    UInt32 queueLoadFactor = 0;
    UInt8  tempStoreOfLCIds[MAX_NUMBER_OF_LOGICAL_CHANNEL] = {0};
    UInt8  localCount = 0;    
    DLLogicalChannelInfo* logicalChannel_p = PNULL;
    UInt32 lcQueueLoad = 0;
    DLLogicalChannelInfo* logicalChannelArr =  ueContext_p->logicalChannel;
    UInt8 maxLogicalChannel = ueContext_p->maxLcId; 
    DLSchedulerStrategyConfig *dlSchdConfig_p =  &(cellSpecificParams_g.
                    cellConfigAndInitParams_p[internalCellIndex]->initParams_p->dlSchdConfig);

/* CA Changes end */
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Entry",__func__);
   /*KLOCKWORK_CHANGES_START_65 */	
    if ((MAX_NUMBER_OF_LOGICAL_CHANNEL <= maxLogicalChannel))
    {
        LTE_MAC_UT_LOG(LOG_WARNING,LOG_MULTIPLEXER,
            "[%s] VALUE of ueContext_p->maxLcId has"
            " been set to %d for the ueIndex = %d",__func__,maxLogicalChannel,ueContext_p->ueIndex);
        return numOfLC; 
    }  
   /*KLOCKWORK_CHANGES_STOP_65 */	
    /*
        We first calculate the Weighted Mean of all LCs, we start to calculate from the 
        LC Id 3 as the LC id 1 and LC id 2 belong to SRBs and they have been already
        catered to.
    */
    if (!muxQueueNode_p->sumWeightedQueueLoad)
    {
        for(count = 3; count <= maxLogicalChannel; count++)
        { 
            /* SPR 21745 Fix + */
            if (INVALID_LCID == logicalChannelArr[count].logicalChannelId)
            {
                continue;
            }
            /* SPR 21745 Fix - */
            lcQueueLoad = logicalChannelArr[count].queueLoad;
            if(lcQueueLoad)
            {
                sumLCPriorityAndQLoad += (MAX_WEIGHTED_LC_PRIORITY - \
                       logicalChannelArr[count].logicalChannelPriority)* \
                                     (logicalChannelArr[count].queueLoad);

                sumLCPriority +=  (MAX_WEIGHTED_LC_PRIORITY - \
                                    logicalChannelArr[count].logicalChannelPriority);
                tempStoreOfLCIds[localCount] = logicalChannelArr[count].logicalChannelId;
                localCount++;
            }
        }
    }
    else
    {
        sumLCPriority = muxQueueNode_p->sumLCPriority;
        sumLCPriorityAndQLoad = muxQueueNode_p->sumWeightedQueueLoad;
        for (count = 3; count <= maxLogicalChannel; count++)
        {
            /* SPR 21745 Fix + */
            if (INVALID_LCID == logicalChannelArr[count].logicalChannelId)
            {
                continue;
            }
            /* SPR 21745 Fix - */
            lcQueueLoad = logicalChannelArr[count].queueLoad;
            if(lcQueueLoad)
            
           {
                tempStoreOfLCIds[localCount] = logicalChannelArr[count].logicalChannelId;
                localCount++;
            }
        } 
    }
    if (sumLCPriority != 0)
    {
        qLoadWeightedMeanofAllLCs = (sumLCPriorityAndQLoad/sumLCPriority);
    }

    if (!qLoadWeightedMeanofAllLCs)
    {
        return numOfLC;
    }
    
    LTE_MAC_UT_LOG(LOG_DETAIL, LOG_MULTIPLEXER, \
        "[%s] -> weighted Mean of all LC is %d\n",__func__, qLoadWeightedMeanofAllLCs);

    /*
        We now calculate the Priority factor of each of the LC Considered above.
    */

    count = (UInt32)numOfLC; 
    /*
        We update the field numOfLC now as this field be used by the calling function.
    */
    localNumOfLC += localCount;
    /* Review comment fix start LJ7 */
    if (ONE == localCount)
    {
        localCount--;
        logicalChannel_p = &logicalChannelArr[tempStoreOfLCIds[localCount]];
        if( ( DRA_SCHEDULING == ueContext_p->schType )  &&
            ( ( SPS_ACTIVATED == ueContext_p->spsDlInfo.spsState ) || 
              ( SPS_ACTIVATION_INITIATED == ueContext_p->spsDlInfo.spsState ) ) &&
            ( LTE_TRUE == logicalChannel_p->isSpsLc ) )
        {
            /*SPS_TDD_Changes*/
            localNumOfLC-- ;
            /*SPS_TDD_Changes*/
            return localNumOfLC;
        }
        sortedArrayOfLC[count].logicalChannelID = tempStoreOfLCIds[localCount];
        sortedArrayOfLC[count].lcQLoad = logicalChannel_p->queueLoad;
        return localNumOfLC;
    }
    /* Review comment fix end LJ7 */
    for (; localCount > 0;)
    {
        localCount--;
        logicalChannel_p = &logicalChannelArr[tempStoreOfLCIds[localCount]];
        
        if( ( DRA_SCHEDULING == ueContext_p->schType )  &&
            ( ( SPS_ACTIVATED == ueContext_p->spsDlInfo.spsState ) || 
              ( SPS_ACTIVATION_INITIATED == ueContext_p->spsDlInfo.spsState ) ) &&
            ( LTE_TRUE == logicalChannel_p->isSpsLc ) )
        {
            /*SPS_TDD_Changes*/
            localNumOfLC-- ;
            /*SPS_TDD_Changes*/
            continue;
        }
/* CA Changes start */
        delayFactor = (dlSchdConfig_p->dwlkLCDelayWt)*(scheduledTick -
                            (logicalChannel_p->lastScheduledTick));

        percentLCQLoad = (logicalChannel_p->queueLoad * 100)/
                                            qLoadWeightedMeanofAllLCs;
        if (percentLCQLoad > 1)
        {
            queueLoadFactor = (dlSchdConfig_p->dwlkLCQLoadWt)* \
                           (1+DIVIDE_BY_1024(logint(percentLCQLoad)));
        }
        else
        {
            queueLoadFactor = dlSchdConfig_p->dwlkLCQLoadWt;
        }
        /*set the weighted priority of logical channel*/

        sortedArrayOfLC[count].sortKey = ((MAX_WEIGHTED_LC_PRIORITY - \
                                logicalChannel_p->logicalChannelPriority) * \
                 (dlSchdConfig_p->dwlkLCPriorityWt)*(1 + queueLoadFactor + delayFactor));

/* CA Changes end */
        LTE_MAC_UT_LOG(LOG_DETAIL, LOG_MULTIPLEXER, \
            "[%s] calculateWeightedPriorityOfLCID -> LCID = %d weightedPriority = %d\n", \
            __func__, tempStoreOfLCIds[localCount], sortedArrayOfLC[count].sortKey);
    
        sortedArrayOfLC[count].logicalChannelID = tempStoreOfLCIds[localCount];
        sortedArrayOfLC[count].lcQLoad = logicalChannel_p->queueLoad;
        count++;
    }
    /*
        We have have all the logical channels with their sort keys ready 
        we apply a quick sort algorithm (O(nlogn)) to sort this array. The check
        below prevents the call of Quick Sort when there is only one DRB.
    */
    if (count > (UInt32)numOfLC + 1)
    { 
       quickSort(sortedArrayOfLC, numOfLC, count, count + 1);
    }
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
    return localNumOfLC;
}

/*****************************************************************************
 * Function Name  : swapFunc
 * Inputs         : sortedArrayOfLC - scheduler Data 
                        dex1 - element one
                        dex2 - element two
 * Outputs        : Swap of the two Elements
 * Returns        : None
 * Description    : It swaps two elements.
 *****************************************************************************/
 void swapFunc(
        SortedLCsLoadInfo sortedArrayOfLC[],
        UInt32 dex1, 
        UInt32 dex2)
{
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Entry",__func__);
    SortedLCsLoadInfo temp = sortedArrayOfLC[dex1];
    sortedArrayOfLC[dex1]= sortedArrayOfLC[dex2];
    sortedArrayOfLC[dex2]= temp;
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
}

/*****************************************************************************
 * Function Name  : medianOfThree
 * Inputs         : sortedArrayOfLC - array to be sorted,
 *                  left - start index of array to be sorted,
 *                  right - end index of array to be sorted
 * Outputs        : It calculates mean value
 * Returns        : median value
 * Description    : Called by quickSort to select a pivot element.
 *****************************************************************************/
 UInt32 medianOfThree(
        SortedLCsLoadInfo sortedArrayOfLC[],
        UInt32 left, 
        UInt32 right) 
{
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Entry",__func__);
    UInt32 center = DIVIDE_BY_TWO ((left + right));
    /* order left & center*/
    if (sortedArrayOfLC[left].sortKey < sortedArrayOfLC[center].sortKey)
        swapFunc(sortedArrayOfLC,left, center);
    /* order left & right*/
    if (sortedArrayOfLC[left].sortKey < sortedArrayOfLC[right].sortKey)
        swapFunc(sortedArrayOfLC,left, right);
    /* order center & right*/
    if (sortedArrayOfLC[center].sortKey < sortedArrayOfLC[right].sortKey)
        swapFunc(sortedArrayOfLC,center, right);

    /*put pivot on right */
    swapFunc(sortedArrayOfLC,center, right - 1);
    /* return median value*/
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
    return sortedArrayOfLC[right - 1].sortKey;
}


/*****************************************************************************
 * Function Name  : makePartition
 * Inputs         : sortedArrayOfLC - array to be sorted,
 *                  left - start index of array to be sorted,
 *                  right - end index of array to be sorted,
 *                  pivot - pivot position
 * Outputs        : Finding out the Pivot position
 * Returns        : pivot location
 * Description    : Called by quickSort to fix position of one element.
 *****************************************************************************/
 UInt32 makePartition(
        SortedLCsLoadInfo sortedArrayOfLC[],
        UInt32 left, 
        UInt32 right, 
        UInt32 pivot)
{
    /*right of first element*/
    UInt32 leftPtr = left; 
    /*left of pivot */
    UInt32 rightPtr = right - 1;
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Entry",__func__);

    while (1) 
    {
        /* finding  bigger */
        while (leftPtr <= right && sortedArrayOfLC[++leftPtr].sortKey > pivot)
            ;
        /* finding smaller*/
        while (rightPtr >= left && sortedArrayOfLC[--rightPtr].sortKey < pivot)
            ;
        /*if pointers cross, partition done */
        if (leftPtr >= rightPtr) 
        {
            break;    
        }
        else
        {
            /*If NOT Crosses then swap elements */
            swapFunc(sortedArrayOfLC,leftPtr, rightPtr);
        }
    }
    /*restore pivot */
    swapFunc(sortedArrayOfLC,leftPtr, right - 1);
    /*return pivot location */
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
    return leftPtr;
}


/*****************************************************************************
 * Function Name  : mySort
 * Inputs         : sortedArrayOfLC - array to be sorted,
 *                  left - start index of array to be sorted,
 *                  right - end index of array to be sorted
 * Outputs        : Sorts the input array
 * Returns        : None
 * Description    : It does the sorting of the array from the start and and of 
                    array index.
 *****************************************************************************/
void mySort(
        SortedLCsLoadInfo sortedArrayOfLC[],
        UInt32 left, 
        UInt32 right)
{
    /*Calculating the size to be sort*/
    UInt32 size = right - left + 1;
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Entry",__func__);

    /*if Size is equal to or Less than 1, then no sort  is necessary */
    if (size <= 1)
    {
        return;
    }
    /**if Size is equal to 2, then sort left and right*/
    if (size == 2) 
    {
        if (sortedArrayOfLC[left].sortKey < sortedArrayOfLC[right].sortKey)
            swapFunc(sortedArrayOfLC,left, right);
        return;
    }
    /*if Size is equal to 3,then sort left, center, & right */
    else 
    { 
         /*left, center*/
        if (sortedArrayOfLC[left].sortKey < sortedArrayOfLC[right - 1].sortKey)
            swapFunc(sortedArrayOfLC,left, right - 1);
        /* left, right */
        if (sortedArrayOfLC[left].sortKey < sortedArrayOfLC[right].sortKey)
            swapFunc(sortedArrayOfLC,left, right);
        /*center, right */
        if (sortedArrayOfLC[right - 1].sortKey < sortedArrayOfLC[right].sortKey)
            swapFunc(sortedArrayOfLC,right - 1, right);
    }
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
}


/*****************************************************************************
 * Function Name  : quickSort
 * Inputs         : sortedArrayOfLC - array to be sorted,
 *                  left - start index of array to be sorted,
 *                  right - end index of array to be sorted,
 *                  maxValuesToBeSorted - The number of maximum values to be 
                                           sorted.
 * Outputs        : Performs the Quick sorting
 * Returns        : None 
 * Description    : It first see how many number of elements need to Sorting,if 
                    Number of elements are less tham 4 then do the manual sorting
                    otherwise it the Quick sorting is done.
 *****************************************************************************/
void quickSort(
        SortedLCsLoadInfo sortedArrayOfLC[],
        UInt32 left, 
        UInt32 right,
        UInt32 maxValuesToBeSorted) 
{
    UInt32 size = right - left + 1;
    /*if Size is less than 4, then do  manual sort*/
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Entry",__func__);
    if (size <= 3)
    {
        mySort(sortedArrayOfLC,left, right);
    }
    /*if Size is large, then do quicksort */
    else 
    {
        if ( left < right )
        {
            UInt32 median = medianOfThree(sortedArrayOfLC,left, right);
            UInt32 partition = makePartition(sortedArrayOfLC,left, right, median);

            quickSort(sortedArrayOfLC,left, partition - 1,maxValuesToBeSorted);       

            if ( partition < maxValuesToBeSorted)
            {
                quickSort(sortedArrayOfLC,partition + 1, right,maxValuesToBeSorted);
            }
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
}

/*****************************************************************************
 * Function Name  :  invokeMuxLogic
 * Inputs         :  muxQueueNode_p - A pointer to the structure MultiplexerQueueNode
 *                                      which stores the information like ueIndex HarqID etc,
 *                   scheduledTick - This information is simply passed by this
 *                                   function and is used by the encoder functionality.
 *                   internalCellIndex
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function is used when MUX is not being used as a 
 *                   separate entity and is being called from PDSCH directly. 
 *****************************************************************************/

 MacRetType invokeMuxLogic(
                        MultiplexerQueueNode* muxQueueNode_p,
                        /* SPR 15909 fix start */
                        tickType_t scheduledTick,
                        /* SPR 15909 fix end */ 
                        InternalCellIndex internalCellIndex)
{
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext* dlUeContext_p = PNULL;
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Entry",__func__);

    /*
        We first Check whether a DUMMY Message has been received or not 
        if yes then it means now the MULTIPLEXER has to stop after sending 
        the information to RLC.
    */
    if (muxQueueNode_p->msgType == DUMMY_MSG)
    {
/* + SPR 11209 Changes */
        processDummyNode(scheduledTick,internalCellIndex, internalCellIndex);
/* - SPR 11209 Changes */
    }
    else
    {
        dlUeContextInfo_p = &dlUECtxInfoArr_g[muxQueueNode_p->ueIndex];
        if (!dlUeContextInfo_p->pendingDeleteFlag) 
        {
            dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;  
            /* SPR 4168 */
            return multiplexLogicalChannelsForUE(muxQueueNode_p, 
                    scheduledTick,
                    dlUeContext_p,
                    internalCellIndex);
            /* SPR 4168 */
        }
        else
        {
            LTE_MAC_UT_LOG(LOG_DETAIL, LOG_MULTIPLEXER, \
            "\n The Pending Delete Flag has been set for the UE INDEX = %d",
            muxQueueNode_p->ueIndex);
            return MAC_FAILURE;
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  initMUX 
 * Inputs         :  numOfCells
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function will creates the mempool required to be used 
 *                   by put entry into MUX Queue.   
 *****************************************************************************/
 
 void initMUX(UInt8 numOfCells)
{
    UInt8 cellIndex = 0;
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Entry",__func__);
    createMemPool(sizeof(MultiplexerQueueNode), MAX_DL_UE_SCHEDULED);
    /* Start 128 UE: Memory Changes */
    /* Assuming 10% UE could have MAX_HARQ_RETX_ reached */
#ifdef TDD_CONFIG
    createMemPool(sizeof(TransmissionAndRLCData), ((MAX_DL_UE_SCHEDULED *MAX_HARQ_RETRANSMISSION_COUNT * DL_HARQ_RTT_TIMER_13_PLUS_4)/MAC_LTE_MEMORY_HARQ_RTX_FACT) * 4);
#else
    createMemPool(sizeof(TransmissionAndRLCData), (MAX_DL_UE_SCHEDULED *MAX_HARQ_RETRANSMISSION_COUNT * 17)/MAC_LTE_MEMORY_HARQ_RTX_FACT);
#endif
    /* End 128 UE: Memory Changes */
    for(cellIndex = 0; cellIndex <numOfCells; cellIndex++)
    {
        sQueueInit(&multiplexerQueue_g[cellIndex]);
            /* SPR 22936 Start */
#ifdef DL_DATA_SEPARATION
        /** SPR 21373 Changes Start **/
        GET_MEM_FROM_POOL(TransmissionAndRLCData,transmissionAndRLCData_g[cellIndex][cellIndex],sizeof(TransmissionAndRLCData),PNULL);
        if (PNULL == transmissionAndRLCData_g[cellIndex][cellIndex])
        {
            LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGERROR,L2_SYS_FAIL,
                    getCurrentTick(), __LINE__,sizeof(TransmissionAndRLCData),
                    cellIndex,0,0,0,0,__FILE__,__func__);
            return; 
        }
        GET_MEM_FROM_POOL(TransmissionAndRLCData,transmissionAndRLCData_g[cellIndex][!cellIndex],sizeof(TransmissionAndRLCData),PNULL);
        if (PNULL == transmissionAndRLCData_g[cellIndex][!cellIndex])
        {
            LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGERROR,L2_SYS_FAIL,
                    getCurrentTick(), __LINE__,sizeof(TransmissionAndRLCData),
                    cellIndex,0,0,0,0,__FILE__,__func__);
            return; 
        }
        /** SPR 21373 Changes End **/
#endif
            /* SPR 22936 End */
    }

    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
}


/*****************************************************************************
 * Function Name  :  cleanUpMUXQueue
 * Inputs         :  internalCellIndex
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function will return the memory allocated to the POOL
 *                   of all the MUX nodes that may have been created.
 *****************************************************************************/
/* CA Changes start */
 void cleanUpMUXQueue(InternalCellIndex internalCellIndex)
/* CA Changes end */
{
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Entry",__func__);
    UInt8 muxNodeCount = 0;
    MultiplexerQueueNode  * muxQueueNode_p = PNULL;
    LTE_SQUEUE *muxQueue_p = PNULL;
    { 
        muxQueue_p = &(multiplexerQueue_g[internalCellIndex]);
        muxNodeCount = sQueueCount(muxQueue_p);
        
        while(muxNodeCount--)
        {  
            muxQueueNode_p = (MultiplexerQueueNode *)popNode(muxQueue_p);
            freeMemPool((void *)muxQueueNode_p);
        }
    }
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
}

/* Cyclomatic Complexity changes - starts here */
/*****************************************************************************
 * Function Name  :  updateAvailableTBOneSizeForTaDrxMsg 
 * Inputs         :  taDrxFlag,
 *                :  availableTBOneSize
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function will update TBOneSize depending upon TA_MSG/DRX_MSG/TA_DRX_MSG.
 *****************************************************************************/
static  void updateAvailableTBOneSizeForTaDrxMsg(MsgType taDrxFlag,
                                          UInt32* availableTBOneSize)
{                                          
    switch(taDrxFlag)
    {
        /*
           We have to allocate Two Bytes so that TA_MSG can be sent to the UE
           */
        case TA_MSG:
            (*availableTBOneSize) -= TWO_BYTES;
            LTE_MAC_UT_LOG(LOG_INFO, LOG_MULTIPLEXER,
                    "\n The TA FLAG was set and hence One Byte was allocated in the TB\n ");
            break;
            /*
               We have to allocate Two Bytes so that DRX_MSG can be sent to the UE
               */
        case DRX_MSG:
            (*availableTBOneSize) -= ONE_BYTE;
            LTE_MAC_UT_LOG(LOG_INFO, LOG_MULTIPLEXER,
                    "\n The DRX FLAG was set and hence Two Bytes were allocated in the TB\n ");
            break;
            /*
               We have to allocate Three Bytes so that TA_DRX_MSG can be sent to the UE
               */
        case TA_DRX_MSG:
            (*availableTBOneSize) -= THREE_BYTES;
            LTE_MAC_UT_LOG(LOG_INFO, LOG_MULTIPLEXER,
                    "\n The TA and DRX FLAG was set and hence 3 Bytes were allocated in the TB\n ");
            break;
            /*
               We donot need to  allocate any Byte as no TA or DRX has to sent to the UE
               */
        default:
            LTE_MAC_UT_LOG(LOG_INFO, LOG_MULTIPLEXER,
                    "\n No FLAG was for TA/DRX set and hence no allocation in the TB\n ");
    }/* End of switch case */
}

/*****************************************************************************
 * Function Name  :  fillSortedArrayOfLC 
 * Inputs         :  queueLoad1,
 *                   queueLoad2,
 *                   logicalChannelId1,
 *                   logicalChannelId2,
 *                   numOfLC,
 *                   srbTotalQLoad,
 *                   sortedArrayOfLC,
 *                   ueContext_p,
 *                   scheduledTick,
 *                   muxQueueNode_p,
 *                   availableTBOneSize,
 *                   availableTBTwoSize
 *                   internalCellIndex
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function fills the Sorted Array of LC.
 *****************************************************************************/
static  void fillSortedArrayOfLC(UInt32 queueLoad1,
                                       UInt32 queueLoad2,
                                       UInt8 logicalChannelId1,
                                       UInt8 logicalChannelId2,
                                       UInt8* numOfLC,
                                       UInt32* srbTotalQLoad,
                                       SortedLCsLoadInfo* sortedArrayOfLC,
                                       DLUEContext* ueContext_p,
                                       /* SPR 15909 fix start */
                                       tickType_t scheduledTick,
                                       /* SPR 15909 fix end */
                                       MultiplexerQueueNode* muxQueueNode_p,
                                       UInt32 availableTBOneSize,
                                       UInt32 availableTBTwoSize,
                                       InternalCellIndex internalCellIndex)
{
    if (queueLoad1)
    {
        sortedArrayOfLC[*numOfLC].logicalChannelID = logicalChannelId1;
        sortedArrayOfLC[*numOfLC].lcQLoad = queueLoad1;
        (*numOfLC)++;
        *srbTotalQLoad = queueLoad1 + RLC_FIXED_HEADER_SIZE_ONE_BYTE + \
                                      THREE_BYTES;
    }
    if (queueLoad2)
    {
        sortedArrayOfLC[*numOfLC].logicalChannelID = logicalChannelId2;
        sortedArrayOfLC[*numOfLC].lcQLoad = queueLoad2;
        (*numOfLC)++;
        *srbTotalQLoad += queueLoad2 + RLC_FIXED_HEADER_SIZE_ONE_BYTE + \
                                       THREE_BYTES;
    }
    if ((availableTBOneSize + availableTBTwoSize) >  ((*srbTotalQLoad) + MAC_MIN_SDU_SIZE))
    {
        *numOfLC =  generateSortedListOfLCs(
                     ueContext_p, 
                     *numOfLC,
                     sortedArrayOfLC,
                     scheduledTick,
                     muxQueueNode_p
                     ,internalCellIndex);

    }
}        
/* Cyclomatic Complexity changes - ends here */

/*****************************************************************************
 * Function Name  :  dualTBDistribution 
 * Inputs         :  muxQueueNode_p - A pointer to the structure MultiplexerQueueNode
 *                                    which stores the information like ueIndex HarqID etc,
 *                   transmissionAndRLCData_p - A pointer to the type  TransmissionAndRLCData,
 *                   dlHarqProcess_p - A pointer to the Concerned DL Harq of the UE
 *                                     under consideration,
 *                   ueContext_p - The pointer to the UE Context Concerned
 *                   internalCellIndex   
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will be only called on when we are in MIMO
 *                   mode and on both the TB new transmission has been scheduled. 
 *****************************************************************************/
/* SPR 4168 */
MacRetType dualTBDistribution(
        MultiplexerQueueNode* muxQueueNode_p,
        TransmissionAndRLCData * transmissionAndRLCData_p,
        DLHARQProcess* dlHarqProcess_p,
        DLUEContext *ueContext_p,
        InternalCellIndex internalCellIndex)
/* SPR 4168 */
{
    DLHARQTBInfo*   dlHarqTBInfo_p = PNULL;
    DLLogicalChannelInfo* logicalChannelArr = PNULL;
    SortedLCsLoadInfo sortedArrayOfLC[MAX_NUMBER_OF_LOGICAL_CHANNEL] = {{0}};     
    UInt8 numOfLC = 0;
    ScheduledLCInfo  scheduledLCInfo = {0};
    UInt32 availableTBOneSize = 0;
    UInt32 availableTBTwoSize = 0;
    UInt32 srbTotalQLoad = 0;    
    DciCCEInfo* dciCCEInfo_p = PNULL;
    UInt8 dciContainerIndex = 0;
    /*
        We now update the relevent information related to the transmissionAndRlcData
        Buffer which is sent to the RLC layer from MUX
    */
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Entry",__func__);
      
    transmissionAndRLCData_p->cceIndex = muxQueueNode_p->cceIndex;
    transmissionAndRLCData_p->harqProcessId = muxQueueNode_p->harqProcessId;

    /*
        We derive the DL HARQ process and the Logical channel Array pointers.
    */    


    logicalChannelArr = ueContext_p->logicalChannel;            

    LTE_MAC_UT_LOG(LOG_DETAIL, LOG_MULTIPLEXER, "[%s] Entry for ueId %d" 
         "hProcessId %d Scheduled tick %d\n",  __func__, ueContext_p->crnti, 
         muxQueueNode_p->harqProcessId, transmissionAndRLCData_p->scheduledTick);


    /*
        We update the transmissionAndRlcData Buffer here in accordance to the 
        tbIdentifier and preserve pointers of those fields which has to be updated on
        a later account (viz.  lcCountOfTB_p & paddingBytesForTB_p).
    */
    transmissionAndRLCData_p->sizeTBOne = dlHarqProcess_p->dlHARQTBOneInfo.tbSize;
    transmissionAndRLCData_p->sizeTBTwo = dlHarqProcess_p->dlHARQTBTwoInfo.tbSize;    

    /* 
        We now need to initialise the information to be passed to the functions
        which are being called from this functon, the elemnts of this structure 
        will updated as according.
    */

    availableTBOneSize = dlHarqProcess_p->dlHARQTBOneInfo.tbSize;
    availableTBTwoSize = dlHarqProcess_p->dlHARQTBTwoInfo.tbSize;

    /*
        We now update the information that needs to be sent to the TB allocatind functionality.
    */

    scheduledLCInfo.logicalChannelArr_p = &logicalChannelArr[0];
    scheduledLCInfo.dlHarqProcess_p = dlHarqProcess_p;
    scheduledLCInfo.ueContext_p = ueContext_p;
    scheduledLCInfo.sortedArrayOfLC_p = &sortedArrayOfLC[0]; 
    
    /*
        We consider here the extreme error condition in which the 
        one of the TB Size has arrived as Less than Three Bytes
        from the concerned higger layer. 
        In the case only as One TB has been configured with the lesser SIZE,
        size and other TB  still being having sufficent size, 
        hence  we try to allocate the 
        resources from this other TB only and there by calling appropriate 
        functions. 
    */ 
    if (availableTBOneSize >= THREE_BYTES)
    {
        dlHarqTBInfo_p = &dlHarqProcess_p->dlHARQTBOneInfo;
  

        /*
            We now keep aside one byte (for DRX) or two bytes (for TA) from the 
            present TB Size, In case of MIMO when both the TB are on new transmission
            the TA/DRX will be allocated only in the TBOne, but in the case when When
            TBOne is on re-transmission the same could be sent on the TBTwo also
        */

        /* Cyclomatic Complexity changes - starts here */
        updateAvailableTBOneSizeForTaDrxMsg(dlHarqTBInfo_p->taDrxFlag,&availableTBOneSize);
        /* Cyclomatic Complexity changes - ends here */

        /*
            We now consider the case Whether the size of TB Two is
            greater than MAC_MIN_SDU_SIZE or not as if not we have to set 
            it as invalid.
        */
        /* SPR 7188 Changes starts */
        if (availableTBTwoSize >= MAC_MIN_SDU_SIZE)  
        /* SPR 7188 Changes ends */
        {
            LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,
                "[%s] Both TBs have TbSize > Three Bytes",__func__);
            dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = \
                                                    transmissionAndRLCData_p; 
            dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = \
                                                    transmissionAndRLCData_p;

            /* Cyclomatic Complexity changes - starts here */
            fillSortedArrayOfLC(logicalChannelArr[1].queueLoad,
                                logicalChannelArr[2].queueLoad,
                                logicalChannelArr[1].logicalChannelId,
                                logicalChannelArr[2].logicalChannelId,
                                &numOfLC,&srbTotalQLoad,sortedArrayOfLC,
                                ueContext_p,
                                transmissionAndRLCData_p->scheduledTick,
                                muxQueueNode_p,availableTBOneSize,
                                availableTBTwoSize,
                                internalCellIndex);
            /* Cyclomatic Complexity changes - ends here */
            /*
                 In case when the numOfLC is Zero we need to free
                 the transmissionAndRLCData_p and return, this is because
                 in case of Multi-threaded scenario this may arise.
            */
            if((0 == numOfLC) && (dlHarqTBInfo_p->taDrxFlag == INVALID_TA_DRX_MSG))
            {
                dciContainerIndex = transmissionAndRLCData_p->scheduledTick % MAX_CCE_CONTAINER;
                dciCCEInfo_p = &((dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex)->
                                       dciCCEInfo[transmissionAndRLCData_p->cceIndex]);
                /* Cyclomatic Complexity changes */
                mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                        dciCCEInfo_p,internalCellIndex);
                /*pdcch for scell changes start*/
                mac_reset_extra_cce( (dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex),dciCCEInfo_p,ueContext_p,internalCellIndex);
                /*pdcch for scell changes end*/

                freeMemPool(transmissionAndRLCData_p);
                dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;

                freeDLHarqProcess(ueContext_p,
                        dlHarqProcess_p,internalCellIndex);
                return MAC_SUCCESS; /* SPR 4168 */
            }   

            scheduledLCInfo.numOfLc = numOfLC;

            /* SPR 4168 */
            return  distributeTwoTBAmongLCs(
                    availableTBOneSize,
                    availableTBTwoSize,
                    transmissionAndRLCData_p,
                    &scheduledLCInfo,
                    internalCellIndex);
            /* SPR 4168 */
        }
        else
        {
            /*
               We reach here when the TB Two was of lesser size than the 
               prescribed threshold of MAC_MIN_SDU_SIZE and hence we have to 
               sent only the TB ONE in this case.
               */
            LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,
                "[%s] TB TWO had a size lesser than Threshold"
                " hence ONLY TB ONE is being used and TB TWO set to TB_INVALID\n",__func__);

            dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = \
                                                    transmissionAndRLCData_p; 
            dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;

            /* Cyclomatic Complexity changes - starts here */
            fillSortedArrayOfLC(logicalChannelArr[1].queueLoad,
                                logicalChannelArr[2].queueLoad,
                                logicalChannelArr[1].logicalChannelId,
                                logicalChannelArr[2].logicalChannelId,
                                &numOfLC,&srbTotalQLoad,sortedArrayOfLC,
                                ueContext_p,
                                transmissionAndRLCData_p->scheduledTick,
                                muxQueueNode_p,availableTBOneSize,0,
                                internalCellIndex);
            /* Cyclomatic Complexity changes - ends here */

            /*
                As the size of the TB Two was less than the required this Module
                explicitly set the TB Two as INVALID before multiplexing the LCs.
            */ 
            dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;
            transmissionAndRLCData_p->paddingBytesForTBTwo = transmissionAndRLCData_p->sizeTBTwo;
            transmissionAndRLCData_p->lcCountTBTwo = 0;

            /*
                 In case when the numOfLC is Zero we need to free
                 the transmissionAndRLCData_p and return, this is because
                 in case of Multi-threaded scenario this may arise.
            */
            if((0 == numOfLC) && (dlHarqTBInfo_p->taDrxFlag == INVALID_TA_DRX_MSG))
            {
                dciContainerIndex = transmissionAndRLCData_p->scheduledTick % MAX_CCE_CONTAINER;
                dciCCEInfo_p = &((dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex)->
                                       dciCCEInfo[transmissionAndRLCData_p->cceIndex]);
                /* Cyclomatic Complexity changes */
                mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                             dciCCEInfo_p,internalCellIndex);
                /*pdcch for scell changes start*/
                mac_reset_extra_cce( (dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex),dciCCEInfo_p,ueContext_p,internalCellIndex);
                /*pdcch for scell changes end*/
                freeMemPool(transmissionAndRLCData_p);
                dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;
                freeDLHarqProcess(ueContext_p,
                        dlHarqProcess_p,internalCellIndex);
                /* SPR 4168 */
                return MAC_SUCCESS;
                /* SPR 4168 */
            }   

            /* SPR 4168 */
            return distributeSingleTBAmongLCs(
                    ueContext_p,
                    numOfLC, 
                    availableTBOneSize,
                    transmissionAndRLCData_p,
                    logicalChannelArr,
                    sortedArrayOfLC,
                    internalCellIndex);      
            /* SPR 4168 */

        }  
    } 
    else 
    {
        dlHarqTBInfo_p = &dlHarqProcess_p->dlHARQTBTwoInfo;
        if (dlHarqTBInfo_p->taDrxFlag != INVALID_TA_DRX_MSG)
        {
        /* SPR 7188 Changes starts */
            if (availableTBTwoSize >= (THREE_BYTES + MAC_MIN_SDU_SIZE))
        /* SPR 7188 Changes ends */
            {
                /*
                   We reach here when the TB One was of lesser size than the 
                   prescribed threshold of Three Bytes and hence we have to 
                   sent only the TB Two in this case, Also as the encoder has
                   to get the info of at least ONE LC for processing further.
                */
                LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,
                    "[%s] TB One had a size lesser than Threshold"
                    " hence ONLY TB Two is being used and TB One set to TB_INVALID\n",__func__);

                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = \
                                                        transmissionAndRLCData_p; 

                /* Cyclomatic Complexity changes - starts here */
                updateAvailableTBOneSizeForTaDrxMsg(dlHarqTBInfo_p->taDrxFlag,&availableTBOneSize);
                /* Cyclomatic Complexity changes - ends here */
            }
            else
            {
                /*
                     We reach a stage where none of the TB has 
                     the size greater than the prescibed threshold
                     of three bytes and this should a rare scenario.
                */
                LTE_MAC_UT_LOG(LOG_WARNING,LOG_MULTIPLEXER,
                   "[%s] Both the TB has a value less than THREE Bytes ",__func__);
                LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
                /*
                     In case when we can't send ant TB due to size restriction
                     we should free the transmissionAndRLCData_p and return. 
                */
                dciContainerIndex = transmissionAndRLCData_p->scheduledTick % MAX_CCE_CONTAINER;
                dciCCEInfo_p = &((dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex)->
                                       dciCCEInfo[transmissionAndRLCData_p->cceIndex]);
                /* Cyclomatic Complexity changes */
                mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                             dciCCEInfo_p,internalCellIndex);
                /*pdcch for scell changes start*/
                mac_reset_extra_cce( (dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex),dciCCEInfo_p,ueContext_p,internalCellIndex);
                /*pdcch for scell changes end*/
                freeMemPool(transmissionAndRLCData_p);
                dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;
                freeDLHarqProcess(ueContext_p,
                        dlHarqProcess_p,internalCellIndex);
                /* SPR 4168 */
                return MAC_SUCCESS;
                /* SPR 4168 */
            }   
        } 
        else
        {
            /*
                 Since there are no TA/DRX message we need not check 
                 it against the THREE_BYTES extra left over and also
                 no need for the switch case of the above.
            */
            /* SPR 7188 Changes starts */
            if (availableTBTwoSize >= MAC_MIN_SDU_SIZE)
            /* SPR 7188 Changes ends */
            {
                LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,
                    "[%s] TB One had a size lesser than Threshold"
                    " hence ONLY TB Two is being used and TB One set to TB_INVALID\n",__func__);
                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = \
                                                        transmissionAndRLCData_p; 
            }
            else
            {
                /*
                     We reach a stage where the TB Two has 
                     the size greater than the prescibed threshold
                     of three bytes but less than the MAC_MIN_SDU_SIZE
                     and this should a rare scenario.
                */
                LTE_MAC_UT_LOG(LOG_WARNING,LOG_MULTIPLEXER,
                   "[%s] Both the TB has a value less than THREE Bytes ",__func__);
                LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
                /*
                     In case when we can't send ant TB due to size restriction
                     we should free the transmissionAndRLCData_p and return. 
                */
                dciContainerIndex = transmissionAndRLCData_p->scheduledTick % MAX_CCE_CONTAINER;
                dciCCEInfo_p = &((dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex)->
                                       dciCCEInfo[transmissionAndRLCData_p->cceIndex]);
                /* Cyclomatic Complexity changes */
                mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                             dciCCEInfo_p,internalCellIndex);
                /*pdcch for scell changes start*/
                mac_reset_extra_cce( (dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex),dciCCEInfo_p,ueContext_p,internalCellIndex);
                /*pdcch for scell changes end*/
                freeMemPool(transmissionAndRLCData_p);
                /*pdcch for scell changes start*/
                mac_reset_extra_cce( (dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex),dciCCEInfo_p,ueContext_p,internalCellIndex);
                /*pdcch for scell changes end*/
                dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;
                freeDLHarqProcess(ueContext_p,
                                  dlHarqProcess_p,internalCellIndex);
            /* SPR 4168 */
                return MAC_SUCCESS;
            /* SPR 4168 */
            }   
        }

        /* Cyclomatic Complexity changes - starts here */
        fillSortedArrayOfLC(logicalChannelArr[1].queueLoad,
                            logicalChannelArr[2].queueLoad,
                            logicalChannelArr[1].logicalChannelId,
                            logicalChannelArr[2].logicalChannelId,
                            &numOfLC,&srbTotalQLoad,sortedArrayOfLC,
                            ueContext_p,
                            transmissionAndRLCData_p->scheduledTick,
                            muxQueueNode_p,0,availableTBTwoSize,
                            internalCellIndex);
        /* Cyclomatic Complexity changes - ends here */
        /*
            As the size of the TB One was less than the required this Module
            explicitly set the TB One as INVALID before multiplexing the LCs.
        */ 
        dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
        transmissionAndRLCData_p->paddingBytesForTBOne = transmissionAndRLCData_p->sizeTBOne;
        transmissionAndRLCData_p->lcCountTBOne = 0;

        /*
             In case when the numOfLC is Zero we need to free
             the transmissionAndRLCData_p and return, this is because
             in case of Multi-threaded scenario this may arise.
        */
        if(0 == numOfLC && dlHarqTBInfo_p->taDrxFlag == INVALID_TA_DRX_MSG)
        {
            dciContainerIndex = transmissionAndRLCData_p->scheduledTick % MAX_CCE_CONTAINER;
            dciCCEInfo_p = &((dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex)->
                                   dciCCEInfo[transmissionAndRLCData_p->cceIndex]);
            /* Cyclomatic Complexity changes */
            mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                    dciCCEInfo_p,internalCellIndex);
            /*pdcch for scell changes start*/
            mac_reset_extra_cce( (dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex),dciCCEInfo_p,ueContext_p,internalCellIndex);
            /*pdcch for scell changes end*/
            freeMemPool(transmissionAndRLCData_p);
            dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
            dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;
            freeDLHarqProcess(ueContext_p,
                    dlHarqProcess_p,internalCellIndex);
            /* SPR 4168 */
            return MAC_SUCCESS;
            /* SPR 4168 */
        }   

        /* SPR 4168 */
        return  distributeSingleTBAmongLCs(
                ueContext_p,
                numOfLC, 
                availableTBTwoSize,
                transmissionAndRLCData_p,
                logicalChannelArr,
                sortedArrayOfLC,
                internalCellIndex);      
        /* SPR 4168 */
    } 
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
    /* SPR 4168 */
    return MAC_SUCCESS;
}


/*****************************************************************************
 * Function Name  :  distributeTwoTBAmongLCs
 * Inputs         :  availableTBOneSize - A Size of TB One left,
 *                   availableTBTwoSize - A Size of TB Two left,
 *                   transmissionAndRLCData_p - A pointer to the TransmissionAndRLCData
 *                                              header size space,
 *                   scheduledLCInfo_p - A pointer to the type ScheduledLCInfo
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will allocate the required TB space to the
 *                   concerned LC and update the relevant fields of TBDistributionInfo.
 *****************************************************************************/
/* SPR 4168 */
 MacRetType distributeTwoTBAmongLCs( 
        UInt32 availableTBOneSize,
        UInt32 availableTBTwoSize,
        TransmissionAndRLCData* transmissionAndRLCData_p,
        ScheduledLCInfo* scheduledLCInfo_p,
        InternalCellIndex    internalCellIndex)
/* SPR 4168 */
{
    UInt16 lcId = INVALID_LC_ID;
    UInt8* lcCountOfTB_p = PNULL;
    UInt32* paddingBytesForTB_p = PNULL;
    DLLogicalChannelInfo* logicalChannelArr = PNULL;    
    UInt32 loopCount = 0;
    UInt32 reDoForNextTB = 0;
    SortedLCsLoadInfo* sortedArrayOfLC = PNULL;       
    UInt32 localLcQload = 0;
    UInt32 sortedArrayIndex = 0;       
    UInt32 localNumTB = 2;
    UInt32 localMaxLcCount = 0;
    UInt32 bytesAllocated = 0;
    UInt32 availableTBSize = 0;
    RLCDataBuffer * rlcDataBufferArr_p = PNULL;
    UInt32 rlcDataBufferArrayIndex = 0;
    DLUEContext *ueContext_p = PNULL;
    /*GCC_4.7 Warning Fix Start*/
    /*DLHARQProcess* dlHarqProcess_p = PNULL;*/
    /*GCC_4.7 Warning Fix End*/
    UInt32 tbSize = availableTBOneSize + availableTBTwoSize;

    /* SPR 4168 */
    MacRetType    retType = MAC_SUCCESS; 
    /* SPR 4168 */
    rlcDataBufferArr_p = transmissionAndRLCData_p->rlcDataBufferArr;
    /*ueContext_p = dlUECtxInfoArr_g[scheduledLCInfo_p->ueIndex].dlUEContext_p;*/
    ueContext_p = scheduledLCInfo_p->ueContext_p;
    /*GCC_4.7 Warning Fix Start*/
    /*dlHarqProcess_p = scheduledLCInfo_p->dlHarqProcess_p;*/
    /*GCC_4.7 Warning Fix Start*/
    /* SPR 4704 Start */	
    transmissionAndRLCData_p->numValidTBs = 2;
    /* SPR 4704 End */	
	
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Entry",__func__);
    /* SPR 7188 Changes starts */
    if (availableTBOneSize >= MAC_MIN_SDU_SIZE)
    /* SPR 7188 Changes ends */
    {
        availableTBSize = availableTBOneSize;
        lcCountOfTB_p = &transmissionAndRLCData_p->lcCountTBOne;
        paddingBytesForTB_p = &transmissionAndRLCData_p->paddingBytesForTBOne;
    }
    /* SPR 7188 Changes starts */
    else if (availableTBTwoSize >= MAC_MIN_SDU_SIZE) 
    /* SPR 7188 Changes ends */
    {
        LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,
            "[%s] Available TB Size of TB one is %d",__func__,availableTBOneSize);
        availableTBSize = availableTBTwoSize;
        lcCountOfTB_p = &transmissionAndRLCData_p->lcCountTBTwo;
        paddingBytesForTB_p = &transmissionAndRLCData_p->paddingBytesForTBTwo; 
        localNumTB = 1;     
    }
    else
    {
        ltePanic("[%s] For both the TB the TB SIZE is less than MAC_MIN_SDU_SIZE",__func__);
	/*KLOCKWORK_CHANGES_START_60_61*/
	return MAC_FAILURE;
	/*KLOCKWORK_CHANGES_START_60_61*/
    } 

    logicalChannelArr = scheduledLCInfo_p->logicalChannelArr_p;
    sortedArrayOfLC = scheduledLCInfo_p->sortedArrayOfLC_p;
    loopCount = scheduledLCInfo_p->numOfLc;


    while (loopCount) 
    {
        lcId = sortedArrayOfLC[sortedArrayIndex].logicalChannelID;
        localLcQload = sortedArrayOfLC[sortedArrayIndex].lcQLoad;    
        sortedArrayIndex++;
        LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] lcId = %d,localLcQload = %d ",__func__,lcId,localLcQload);

        do 
        {
            reDoForNextTB = 0;
            bytesAllocated = 0;

            if (logicalChannelArr[lcId].rlcMode != RLC_TM_MODE)
            {
                if (logicalChannelArr[lcId].rlcSNFieldLength == RLC_SN_FIELD_LENGTH_SMALL)
                {
                    bytesAllocated = RLC_FIXED_HEADER_SIZE_ONE_BYTE;                      
                }
                else
                {
                    bytesAllocated = RLC_FIXED_HEADER_SIZE_TWO_BYTE;                      
                }
            }
            availableTBSize -= (THREE_BYTES + bytesAllocated );                                       

            if (localLcQload <= availableTBSize)
            {
                bytesAllocated += localLcQload;
                availableTBSize -= localLcQload;  
                localLcQload = 0;                
            }
            else
            {
                bytesAllocated += availableTBSize;
                localLcQload -= availableTBSize;
                availableTBSize = 0;
            }

            if (bytesAllocated < MAC_SDU_SIZE_CHECKPOINT)
            {
                /* 
                   Here we donot update the  
                   Avaliable TB Size as this one Byte can be 
                   allocated to the TB 
                   */
                if (localLcQload)
                {
                    bytesAllocated++;
                    localLcQload--;
                }
                else
                {
                    availableTBSize += ONE_BYTE;                               
                }
            }
            /*  RLC - ReArch changes start */
            rlcDataBufferArr_p[rlcDataBufferArrayIndex].statusPdu_p = PNULL; 
            rlcDataBufferArr_p[rlcDataBufferArrayIndex].statusPduLen = 0;
            rlcDataBufferArr_p[rlcDataBufferArrayIndex].retxRlcPdu_p = PNULL; 
            rlcDataBufferArr_p[rlcDataBufferArrayIndex].retxPduLen = 0;
            rlcDataBufferArr_p[rlcDataBufferArrayIndex].newRlcPdu_p = PNULL; 
            rlcDataBufferArr_p[rlcDataBufferArrayIndex].newPduLen = 0;
            /*  RLC - ReArch changes end */
            rlcDataBufferArr_p[rlcDataBufferArrayIndex].remainingDataSize = 0;
            rlcDataBufferArr_p[rlcDataBufferArrayIndex].requestedDataSize = bytesAllocated;
            rlcDataBufferArr_p[rlcDataBufferArrayIndex].lcID = lcId;
            rlcDataBufferArrayIndex++;
            localMaxLcCount++;
            logicalChannelArr[lcId].lastScheduledTick = transmissionAndRLCData_p->scheduledTick;   
                   
            if (availableTBSize < MAC_MIN_SDU_SIZE)
            {
                localNumTB--;
                *lcCountOfTB_p = localMaxLcCount;
                *paddingBytesForTB_p = availableTBSize;
                 /*transmissionAndRLCData_p->numValidTBs++;*/
                if (localNumTB)
                {
                    if (localLcQload)
                    {
                        reDoForNextTB = 1;
                    }
                    lcCountOfTB_p = &transmissionAndRLCData_p->lcCountTBTwo;
                    paddingBytesForTB_p = &transmissionAndRLCData_p->paddingBytesForTBTwo;
                    availableTBSize = availableTBTwoSize;
                    localMaxLcCount = 0;
                    /*
                       We now cater to a situation when the availableTBSize
                       of the TB TWO is less than MAC_MIN_SDU_SIZE, Under
                       such condition we will sent the entire TB TWO as Padding
                    */
                    if (availableTBSize < MAC_MIN_SDU_SIZE)
                    {
                       /*
                          The next two variables are being reset
                          in order to break the LOOP. 
                       */
                       LTE_MAC_UT_LOG(LOG_DETAIL, LOG_MULTIPLEXER, 
                           "[%s] A rare Phenomenon Occured SIZE of TB Two = %d ", __func__,availableTBSize);
                       reDoForNextTB = 0;
                       loopCount = 1;
                    }         
                }
                else
                {

                    LTE_MAC_UT_LOG(LOG_DETAIL, LOG_MULTIPLEXER, "[%s]REQUESTING" 
                        "RLC DATA for ueindex %d padding bytes for TBOne %d padding Bytes for Tb Two"
                        "Scheduled tick %d sizeof TBOne %d sizeof TBTwo %d \n",  __func__,
                        ueContext_p->ueIndex, 
                        transmissionAndRLCData_p->paddingBytesForTBOne,
                        transmissionAndRLCData_p->paddingBytesForTBTwo, 
                        transmissionAndRLCData_p->sizeTBOne,
                        transmissionAndRLCData_p->sizeTBTwo);
                    
                    /*
                        The following section of code will hit only in the case
                        when TB One had the size less than MAC_MIN_SDU_SIZE.      
                    */
					
                    ueContext_p->assignedRBQueueLoad
                             [ueContext_p->assignedRBQueueLoadSchIndex] = tbSize;
            
                    ueContext_p->assignedRBQueueLoadSchIndex =
                                  (ueContext_p->assignedRBQueueLoadSchIndex + 1)% 
                                                     MAX_TEMP_QUEUELOAD_ARR_SIZE;
                    /** SPR 3657 Changes Start **/
                    INIT_RLC_RETX_Q(transmissionAndRLCData_p,(transmissionAndRLCData_p->lcCountTBOne +
                        transmissionAndRLCData_p->lcCountTBTwo));
                    /** SPR 3657 Changes End **/
                    /* SPR 4168 */
                    /* + coverity 32186 */
                    retType = (MacRetType) macUeOpportunityInd(
                            ueContext_p->ueIndex, 
                            (transmissionAndRLCData_p->lcCountTBOne + \
                             transmissionAndRLCData_p->lcCountTBTwo), 
                            (RLCDataBuffer*) transmissionAndRLCData_p,
                            transmissionAndRLCData_p->scheduledTick,
                            internalCellIndex);
                    /* - coverity 32186 */
                    return retType;
                    /* SPR 4168 */
                }                    
            }
        }while (reDoForNextTB);
        loopCount--;
    }
    /*
       This section of the code should only hit when there is still avaliable TB but
       the Queue Load for the LCs of this UE has been finished, so we must now 
       send the TB to the RLC.
       */
    *lcCountOfTB_p = localMaxLcCount;
    *paddingBytesForTB_p = availableTBSize;

    LTE_MAC_UT_LOG(LOG_DETAIL, LOG_MULTIPLEXER, "[%s]REQUESTING" 
        "RLC DATA for ueindex %d padding bytes for TBOne %d padding Bytes for Tb Two"
        "Scheduled tick %d sizeof TBOne %d sizeof TBTwo %d \n",  __func__,
        ueContext_p->ueIndex, 
        transmissionAndRLCData_p->paddingBytesForTBOne,
        transmissionAndRLCData_p->paddingBytesForTBTwo, 
        transmissionAndRLCData_p->sizeTBOne,
        transmissionAndRLCData_p->sizeTBTwo);



    if (localMaxLcCount)
    {
        ueContext_p->assignedRBQueueLoad
                [ueContext_p->assignedRBQueueLoadSchIndex] = tbSize;
                
        ueContext_p->assignedRBQueueLoadSchIndex =
                 (ueContext_p->assignedRBQueueLoadSchIndex + 1)% 
                                   MAX_TEMP_QUEUELOAD_ARR_SIZE;
    }

     /** SPR 3657 Changes Start **/
     INIT_RLC_RETX_Q(transmissionAndRLCData_p,(transmissionAndRLCData_p->lcCountTBOne +
         transmissionAndRLCData_p->lcCountTBTwo));
     /** SPR 3657 Changes End **/

    /* SPR 4168 */
    /* + coverity 32186 */
    retType = (MacRetType) macUeOpportunityInd(ueContext_p->ueIndex, 
            (transmissionAndRLCData_p->lcCountTBOne + \
             transmissionAndRLCData_p->lcCountTBTwo), 
            (RLCDataBuffer*) transmissionAndRLCData_p,
            transmissionAndRLCData_p->scheduledTick,
                internalCellIndex);
    /* - coverity 32186 */
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
    return retType;
    /* SPR 4168 */
}

/*****************************************************************************
 * Function Name  :  distributeSingleTBAmongLCs 
 * Inputs         :  ueContext_p - pointer to DLUEContext,
 *                   loopCount - The No. of times LC to be multiplexed,
 *                   availableTBSize - the present value of TB Size,
 *                   transmissionAndRLCData_p - A pointer to the TransmissionAndRLCData
 *                                              header size space,
 *                   logicalChannelArr - A pointer to the Logical Channel Array,
 *                   sortedArrayOfLC - The Array of logical Channel After sorting
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will allocate the required TB space to the
 *                   concerned LC and update the relevant fields of TBDistributionInfo.
 *****************************************************************************/
/* SPR 4168 */
 MacRetType distributeSingleTBAmongLCs(
        DLUEContext *ueContext_p,
        UInt8 loopCount,   
        UInt32 availableTBSize,
        TransmissionAndRLCData* transmissionAndRLCData_p,
        DLLogicalChannelInfo* logicalChannelArr,  
        SortedLCsLoadInfo* sortedArrayOfLC,
        InternalCellIndex  internalCellIndex)
/* SPR 4168 */
{
    UInt16 lcId = INVALID_LC_ID;
    UInt8* lcCountOfTB_p = PNULL;
    UInt32* paddingBytesForTB_p = PNULL;
    UInt32 localLcQload = 0;
    UInt32 sortedArrayIndex = 0;       
    UInt32 localMaxLcCount = 0;
    UInt32 bytesAllocated = 0;
    RLCDataBuffer * rlcDataBufferArr_p = PNULL;

    UInt32 tbSize = availableTBSize;
    /* SPR 4168 */
    MacRetType    retType = MAC_SUCCESS; 
    /* SPR 4168 */

    rlcDataBufferArr_p = transmissionAndRLCData_p->rlcDataBufferArr;

    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Entry",__func__);
    if (transmissionAndRLCData_p->sizeTBOne)
    {
        paddingBytesForTB_p = &transmissionAndRLCData_p->paddingBytesForTBOne;
        lcCountOfTB_p = &transmissionAndRLCData_p->lcCountTBOne;
    }
    else if (transmissionAndRLCData_p->sizeTBTwo)
    {
        paddingBytesForTB_p = &transmissionAndRLCData_p->paddingBytesForTBTwo;
        lcCountOfTB_p = &transmissionAndRLCData_p->lcCountTBTwo;    
    }
    else
    {
        ltePanic("[%s] Invalid case the TB has TB Size",__func__);
	/*KLOCKWORK_CHANGES_START_62_63*/
	return MAC_FAILURE; 
	/*KLOCKWORK_CHANGES_STOP_62_63*/
    }


    /** KlocWork Changes Start **/
    if(loopCount > MAX_NUMBER_OF_LOGICAL_CHANNEL)
    {
        loopCount = MAX_NUMBER_OF_LOGICAL_CHANNEL;
    }
    /** KlocWork Changes End **/
    while (loopCount) 
    {
        lcId = sortedArrayOfLC[sortedArrayIndex].logicalChannelID;
        localLcQload = sortedArrayOfLC[sortedArrayIndex].lcQLoad;    
        sortedArrayIndex++;

        LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,
                "[%s] lcId = %d,localLcQload = %d ",__func__,lcId,localLcQload);
        bytesAllocated = 0;
        /* SPR 7188 Changes starts */
        if (availableTBSize >= MAC_MIN_SDU_SIZE)
        {
        /* SPR 7188 Changes ends */
            if (logicalChannelArr[lcId].rlcMode != RLC_TM_MODE)
            {
                if (logicalChannelArr[lcId].rlcSNFieldLength == RLC_SN_FIELD_LENGTH_SMALL)
                {
                    bytesAllocated = RLC_FIXED_HEADER_SIZE_ONE_BYTE;                      
                }
                else
                {
                    bytesAllocated = RLC_FIXED_HEADER_SIZE_TWO_BYTE;                      
                }
            }
            availableTBSize -= (THREE_BYTES + bytesAllocated ) ;                                       
            if (localLcQload <= availableTBSize)
            {
                bytesAllocated += localLcQload;
                availableTBSize -= localLcQload;                  
                localLcQload = 0;
            }
            else
            {
                bytesAllocated += availableTBSize;
                availableTBSize = 0;
            }

            if (bytesAllocated < MAC_SDU_SIZE_CHECKPOINT)
            {
                /* 
                    Here we donot update the TotalBytesAllocated 
                    and  the Avaliable TB Size as this one Byte can be 
                    allocated to the TB 
                */
                if (localLcQload)
                {
                    bytesAllocated++;
                }
                else
                {
                    availableTBSize += ONE_BYTE;                               
                }
            }
            /*  RLC - ReArch changes start */
            rlcDataBufferArr_p[localMaxLcCount].statusPdu_p = PNULL;
            rlcDataBufferArr_p[localMaxLcCount].statusPduLen = 0;
            rlcDataBufferArr_p[localMaxLcCount].retxRlcPdu_p = PNULL;
            rlcDataBufferArr_p[localMaxLcCount].retxPduLen = 0;
            rlcDataBufferArr_p[localMaxLcCount].newRlcPdu_p = PNULL;
            rlcDataBufferArr_p[localMaxLcCount].newPduLen = 0;
            /*  RLC - ReArch changes end */
            rlcDataBufferArr_p[localMaxLcCount].remainingDataSize = 0;
            rlcDataBufferArr_p[localMaxLcCount].requestedDataSize = \
                                  bytesAllocated;
            rlcDataBufferArr_p[localMaxLcCount].lcID = lcId;
            localMaxLcCount++;
            logicalChannelArr[lcId].lastScheduledTick = \
                          transmissionAndRLCData_p->scheduledTick;   
        }
        else 
        {
            *lcCountOfTB_p = localMaxLcCount;
            *paddingBytesForTB_p = availableTBSize;

            LTE_MAC_UT_LOG(LOG_DETAIL, LOG_MULTIPLEXER, 
                    "[%s]#####REQUESTING" 
                "RLC DATA for ueindex %d padding bytes for TBOne %d padding Bytes for Tb Two"
                "Scheduled tick %d sizeof TBOne %d sizeof TBTwo %d \n",  __func__,
                ueContext_p->ueIndex, 
                transmissionAndRLCData_p->paddingBytesForTBOne,
                transmissionAndRLCData_p->paddingBytesForTBTwo, 
                transmissionAndRLCData_p->sizeTBOne,
                transmissionAndRLCData_p->sizeTBTwo);
            transmissionAndRLCData_p->numValidTBs++;
            
            ueContext_p->assignedRBQueueLoad
                [ueContext_p->assignedRBQueueLoadSchIndex] = tbSize;
            
            ueContext_p->assignedRBQueueLoadSchIndex =
                        (ueContext_p->assignedRBQueueLoadSchIndex + 1)% 
                                           MAX_TEMP_QUEUELOAD_ARR_SIZE;
           /** SPR 3657 Changes Start **/
           INIT_RLC_RETX_Q(transmissionAndRLCData_p,localMaxLcCount);
           /** SPR 3657 Changes End **/
 
            /* SPR 4168 */
            /* + coverity 32185 */
            retType = (MacRetType) macUeOpportunityInd(ueContext_p->ueIndex, 
                                   localMaxLcCount, 
                                   (RLCDataBuffer*) transmissionAndRLCData_p,
                                   transmissionAndRLCData_p->scheduledTick,
                                    internalCellIndex);
            /* + coverity 32185 */
            /* SPR 16766 fix start */
            if( MAC_FAILURE == retType )
            {
                LOG_MAC_MSG(MAC_MUX_ENCODE_FAIL_ID, LOGWARNING, MAC_MUX,
                        getCurrentTick(), ueContext_p->ueIndex,
                        ueContext_p->crnti, localMaxLcCount, availableTBSize,
                        0, 0,0, __func__,
                        "oppIndFailed");
            }
            /* SPR 16766 fix end */
            return retType;
            /* SPR 4168 */
        }
        loopCount--;
    }
    /*
       This section of the code should only hit when there is still avaliable TB but
       the Queue Load for the LCs of this UE has been finished, so we must now 
       send the TB to the RLC.
       */
    *lcCountOfTB_p = localMaxLcCount;
    *paddingBytesForTB_p = availableTBSize;
   
    LTE_MAC_UT_LOG(LOG_DETAIL, LOG_MULTIPLEXER, 
            "[%s]REQUESTING" 
        "RLC DATA for ueindex %d padding bytes for TBOne %d padding Bytes for Tb Two"
        "Scheduled tick %d sizeof TBOne %d sizeof TBTwo %d \n",  __func__,
        ueContext_p->ueIndex, 
        transmissionAndRLCData_p->paddingBytesForTBOne,
        transmissionAndRLCData_p->paddingBytesForTBTwo, 
        transmissionAndRLCData_p->sizeTBOne,
        transmissionAndRLCData_p->sizeTBTwo);
    transmissionAndRLCData_p->numValidTBs++;

    if (localMaxLcCount)
    {
        ueContext_p->assignedRBQueueLoad
                [ueContext_p->assignedRBQueueLoadSchIndex] = tbSize;
                
        ueContext_p->assignedRBQueueLoadSchIndex =
                 (ueContext_p->assignedRBQueueLoadSchIndex + 1)% 
                                   MAX_TEMP_QUEUELOAD_ARR_SIZE;
    }
    /** SPR 3657 Changes Start **/
    INIT_RLC_RETX_Q(transmissionAndRLCData_p,localMaxLcCount);
    /** SPR 3657 Changes End **/
    /* SPR 4168 */
    /* + coverity 32186 */
    retType = (MacRetType) macUeOpportunityInd(ueContext_p->ueIndex, 
                           localMaxLcCount, 
                           (RLCDataBuffer*) transmissionAndRLCData_p,
                           transmissionAndRLCData_p->scheduledTick,
                           internalCellIndex);


    /* SPR 16766 fix start */
    if( MAC_FAILURE == retType )
    {
        LOG_MAC_MSG(MAC_MUX_ENCODE_FAIL_ID, LOGWARNING, MAC_MUX,
                getCurrentTick(), ueContext_p->ueIndex,
                localMaxLcCount, availableTBSize, 0,0, 
                0,0, __func__,
                "oppIndFailed");
    }
    /* SPR 16766 fix end */
    /* - coverity 32186 */
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);
    return retType ;
    /* SPR 4168 */
}



/*****************************************************************************
 * Function Name  :  singleTBDistribution  
 * Inputs         :  muxQueueNode_p - A pointer to the structure MultiplexerQueueNode
 *                                  which stores the information like ueIndex HarqID etc,
 *                   transmissionAndRLCData_p - The pointer to the TransmissionAndRLCData.
 *                   ueContext_p - The pointer to the UE Context Concerned,
 *                   dlHarqProcess_p - The pointer to the DLHARQProcess,
 *                   tbIdentifier - The Corresponding TB Value,
 *                   scheduledTick
 *                   internalCellIndex
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This fucntion is called on both MIMO/Non-MIMO modes when only
 *                   a single TB has to be sent. 
 *****************************************************************************/
/* SPR 4168 */
MacRetType singleTBDistribution(
        MultiplexerQueueNode* muxQueueNode_p,
        TransmissionAndRLCData * transmissionAndRLCData_p,
        DLUEContext *ueContext_p ,
        DLHARQProcess* dlHarqProcess_p,
        UInt8 tbIdentifier,
        /* +- SPR 17777 */
/* SPR 4168 */
        InternalCellIndex internalCellIndex)
{

    DLLogicalChannelInfo* logicalChannelArr = PNULL;
    SortedLCsLoadInfo sortedArrayOfLC[MAX_NUMBER_OF_LOGICAL_CHANNEL] = {{0}};     
    UInt8 numOfLC = 0;
    UInt32 availableTBSize = 0;
    UInt32 srbTotalQLoad = 0;    
    DLHARQTBInfo*   dlHarqTBInfo_p = PNULL;
    DLHARQTBInfo*   dlHarqOtherTBInfo_p = PNULL;
    DciCCEInfo* dciCCEInfo_p = PNULL;
    UInt8 dciContainerIndex = 0;
    /*
       We derive the DL HARQ process and the Logical channel Array pointers.
       */    

    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Entry",__func__);

    logicalChannelArr = ueContext_p->logicalChannel;            

    /* SPR#706 Changes Start */
    if (PNULL == dlHarqProcess_p)
    {
        LTE_MAC_UT_LOG(LOG_WARNING, LOG_MULTIPLEXER, 
           "[%s] dlHarqProcess_p is NULL hence returning \n", 
           __func__);
        ltePanic ("dlHarqProcess_p is NULL"); /*SPR 4168 */
    }
    /* SPR#706 Changes End */

    if (tbIdentifier == TB_ONE)
    {
        dlHarqTBInfo_p = &dlHarqProcess_p->dlHARQTBOneInfo;   
        dlHarqOtherTBInfo_p = &dlHarqProcess_p->dlHARQTBTwoInfo;   
    }
    else if (tbIdentifier == TB_TWO)
    {
        dlHarqOtherTBInfo_p = &dlHarqProcess_p->dlHARQTBOneInfo;   
        dlHarqTBInfo_p = &dlHarqProcess_p->dlHARQTBTwoInfo;                  
    }



    /*SPR 4017 PH2 Start*/
    if(dlHarqTBInfo_p)
    {
        LTE_MAC_UT_LOG(LOG_DETAIL, LOG_MULTIPLEXER, 
                "[%s] Entry for ueId %d" 
              "\n hProcessId %d Scheduled tick %d\ttbsize%d\n",  __func__, ueContext_p->crnti, 
              muxQueueNode_p->harqProcessId, transmissionAndRLCData_p->scheduledTick,dlHarqTBInfo_p->tbSize);
        availableTBSize = dlHarqTBInfo_p->tbSize;
    }
    /*SPR 4017 PH2 End*/
    /* COVERITY 10066 */
    else
    {
        /* SPR 16766 fix start */
        LOG_MAC_MSG(MAC_MUX_ENCODE_FAIL_ID, LOGWARNING, MAC_MUX,
                getCurrentTick(), ueContext_p->ueIndex,
                ueContext_p->crnti, 0,0,0, 0,0, __func__,
                "null dlHarqTBInfo_p");
        /* SPR 16766 fix end */
        LTE_MAC_UT_LOG(LOG_WARNING, LOG_MULTIPLEXER, 
           "[%s] dlHarqTBInfo_p is NULL hence returning \n", 
           __func__);
        return MAC_FAILURE;
    }
    /* COVERITY 10066 */

    if (availableTBSize < THREE_BYTES)
    {
        LTE_MAC_UT_LOG(LOG_WARNING, LOG_MULTIPLEXER, 
           "[%s] The TB Size is %d which is less than the threshold hence return", 
           __func__,availableTBSize);
        LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit  on failure ",__func__);
       
       /* COVERITY 10734*/
        if (dlHarqOtherTBInfo_p)
        {
       /* COVERITY 10734*/
          if (dlHarqOtherTBInfo_p->txIndicator)
        
          {
	    {
            dciContainerIndex = transmissionAndRLCData_p->scheduledTick % MAX_CCE_CONTAINER;
            dciCCEInfo_p = &((dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex)->
                                   dciCCEInfo[transmissionAndRLCData_p->cceIndex]);
/*mandeep changes start*/
/*mandeep changes stop*/
            /* Cyclomatic Complexity changes */
            mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                    dciCCEInfo_p,internalCellIndex);
            /*pdcch for scell changes start*/
            mac_reset_extra_cce( (dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex),dciCCEInfo_p,ueContext_p,internalCellIndex);
            /*pdcch for scell changes end*/
	    }
            freeMemPool(transmissionAndRLCData_p);
            dlHarqTBInfo_p->transmissionAndRLCData_p = PNULL;
            freeDLHarqProcessTBSpecific(
                          ueContext_p,
                          dlHarqProcess_p,
                          tbIdentifier,
                          internalCellIndex);
          }
          /* SPR 4168 */
          return MAC_SUCCESS;
          /* SPR 4168 */
       /* COVERITY 10734*/
        }
       /* COVERITY 10734*/
    }

    /*
        We now keep aside one byte (for DRX) or two bytes (for TA) from the 
        present TB Size, In case of MIMO when both the TB are on new transmission
        the TA/DRX will be allocated only in the TBOne, but in the case when When
        TBOne is on re-transmission the same could be sent on the TBTwo also
    */
    /* Cyclomatic Complexity changes - starts here */
    updateAvailableTBOneSizeForTaDrxMsg(dlHarqTBInfo_p->taDrxFlag,
                                        &availableTBSize);
    /* Cyclomatic Complexity changes - ends here */
    /* SPS_CHG */
    /*! \fn static MacRetType singleTBDistribution(
     *                     MultiplexerQueueNode* muxQueueNode_p,
     *                     TransmissionAndRLCData * transmissionAndRLCData_p,
     *                     DLUEContext *ueContext_p ,
     *                     DLHARQProcess* dlHarqProcess_p,
     *                     UInt8 tbIdentifier 
     *                     UInt32 scheduledTick )
     *  \brief Function Modification 
     * */
    if (SPS_ACTIVATED == ueContext_p->spsDlInfo.spsState || 
        SPS_ACTIVATION_INITIATED == ueContext_p->spsDlInfo.spsState ) 
    {
        /*! \code
         * Handling of strictSpsResourceUsageForSpsLc for RR/PFS in Multiplexer
         * If (UE State == SPS_ACTIVATED) and (Current TTI correspond to SPS 
         * Occasion) 
         * \endcode
         * */
        UInt8 dlSpsInterval = 0;
        LP_SpsCommIntervalProfile commSpsIntervalProfile_p = PNULL;
        dlSpsInterval = ueContext_p->spsDlInfo.spsDlSetupInfo_p->\
                        semiPersistentSchedIntervalDL;
        commSpsIntervalProfile_p = &cellSpecificParams_g.\
              cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
              spsCommIntervalProfile[dlSpsInterval];

        /*! \code
         * Check for strictSpsResourceUsageForSpsLc
         * If strictSpsResourceUsageForSpsLc is TRUE
         *     Do not multiplex SPS LC with any other LC
         * Else, do the normal processing
         * \endcode
         * */
        if(commSpsIntervalProfile_p->strictSpsResourceUsageForSpsLc) 
        {
            if( SPS_SCHEDULING == ueContext_p->schType )
            {
                UInt8 lcIndex = ueContext_p->spsDlInfo.lcId;
                /* Commented for Ph -1 for padding TB to be sent on empty occasion and
                 * Activation */
                /*if (logicalChannelArr[lcIndex].queueLoad) */
                {
                    sortedArrayOfLC[numOfLC].logicalChannelID = lcIndex;
                    sortedArrayOfLC[numOfLC].lcQLoad = dlHarqTBInfo_p->tbSize ;
                        /* logicalChannelArr[lcIndex].queueLoad; */
                    numOfLC++;
                }
                return distributeSingleTBAmongLCs(
                        ueContext_p,
                        numOfLC, 
                        availableTBSize,
                        transmissionAndRLCData_p,
                        logicalChannelArr,
                        sortedArrayOfLC,
                        internalCellIndex);      
            }
        }
    }
    /* SPS_CHG */
    if (logicalChannelArr[1].queueLoad)
    {
        sortedArrayOfLC[numOfLC].logicalChannelID = logicalChannelArr[1].logicalChannelId;
        sortedArrayOfLC[numOfLC].lcQLoad = logicalChannelArr[1].queueLoad;
        numOfLC++;
        srbTotalQLoad = logicalChannelArr[1].queueLoad + \
            RLC_FIXED_HEADER_SIZE_ONE_BYTE + THREE_BYTES;
    }
    if (logicalChannelArr[2].queueLoad)
    {
        sortedArrayOfLC[numOfLC].logicalChannelID = logicalChannelArr[2].logicalChannelId;
        sortedArrayOfLC[numOfLC].lcQLoad = logicalChannelArr[2].queueLoad;
        numOfLC++;
        srbTotalQLoad += logicalChannelArr[2].queueLoad + \
            RLC_FIXED_HEADER_SIZE_ONE_BYTE + THREE_BYTES;
    }

    if (availableTBSize >  srbTotalQLoad + MAC_MIN_SDU_SIZE)
    {
          numOfLC =  generateSortedListOfLCs(
                              ueContext_p, 
                              numOfLC,
                              sortedArrayOfLC,
                              transmissionAndRLCData_p->scheduledTick,
                              muxQueueNode_p,
                              internalCellIndex);
          
    }
    /*
         In case when the numOfLC is Zero we need to free
         the transmissionAndRLCData_p and return, this is because
         in case of Multi-threaded scenario this may arise.
    */
    if((0 == numOfLC) && (dlHarqTBInfo_p->taDrxFlag == INVALID_TA_DRX_MSG))
    {
        /* SPR 16766 fix start */
        LOG_MAC_MSG(MAC_MUX_ENCODE_FAIL_ID, LOGWARNING, MAC_MUX,
                getCurrentTick(), ueContext_p->ueIndex,
                ueContext_p->crnti, 0,0,0, 0,0, __func__,
                "retFail2SingleTBDistribution");
        /* SPR 16766 fix end */
        /* SPR 20254 Fix Start  */
	/* Reset CCE and Retx will be done in processPDSCHFailure() */
        return MAC_FAILURE;
        /* SPR 20254 Fix End  */
    }   

    /* SPR 4168 */
    return  distributeSingleTBAmongLCs(
            ueContext_p,
            numOfLC, 
            availableTBSize,
            transmissionAndRLCData_p,
            logicalChannelArr,
            sortedArrayOfLC,
            internalCellIndex);      
    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Exit",__func__);

    /* SPR 4168 */
}

/* QOS MUX Change Start */
/*****************************************************************************
 * Function Name  :  multiplexLCForOneTB 
 * Inputs         :  transmissionAndRLCData_p - pointer to the transmitted RLC
 *                                              data,
 *                   ueContext_p - Pointer to the DLUEContext,
 *                   dlHarqProcess_p - Pointer to the DLHARQProcess,
 *                   tbIdentifier - indicates whether function is called for TB
 *                                  one or TB two
 *                   internalCellIndex  
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE 
 * Description    :  This function fills the RLC data buffer with relevant 
 *                   information and sends it to the RLC through Mac Ue 
 *                   Indication for Single TB.
 *****************************************************************************/
MacRetType multiplexLCForOneTB(                          
            TransmissionAndRLCData * transmissionAndRLCData_p,
            DLUEContext *ueContext_p ,
            DLHARQProcess* dlHarqProcess_p,
            UInt8 tbIdentifier,
            InternalCellIndex internalCellIndex)
{
    DLQosStrategyLCNode *dlQosStrategyLCNode_p = PNULL;
    void *dlQosStrategyLCNodevoid_p = PNULL;
    RLCDataBuffer * rlcDataBufferArr_p = PNULL;
    SInt8 loopCount = 0;
    UInt8 lcCount = 0;
    DLHARQTBInfo*   dlHarqTBInfo_p = PNULL;
    DLHARQTBInfo*   dlHarqOtherTBInfo_p = PNULL;
    DciCCEInfo* dciCCEInfo_p = PNULL;
    UInt8 dciContainerIndex = 0;
    UInt32 availableTBSize = 0;
    UInt8* lcCountOfTB_p = PNULL;
    UInt32* paddingBytesForTB_p = PNULL;
    MacRetType retVal = MAC_FAILURE;

    /* For padding bytes adjusements */
    UInt32 tempLcCount = 0;
/** opt **/
    UInt32 tempQueueLoad = 0;
    UInt32 remainingQueueLoad = 0;
/** opt **/
/*SPR 17561 FIX*/
#ifdef TDD_CONFIG
    UInt32 sCellStateChangeTddTimerOffset = 0;
    sCellStateChangeTddTimerOffset = getSCellStateChangeTddTimerOffset(ueContext_p->internalCellIndex);
    UInt32 scheduledExpiryTick = getCurrentTick() + PHY_DL_DELAY + sCellStateChangeTddTimerOffset;
    UInt8  harqSubFrameNum = (scheduledExpiryTick)%MAX_SUB_FRAME;
#endif
/*SPR 17561 FIX*/

    /* SPR 13492 Changes Start */
#ifdef KPI_STATS
    UInt8 qci = INVALID_QCI;
#endif
    /* SPR 13492 Changes End */

    rlcDataBufferArr_p = transmissionAndRLCData_p->rlcDataBufferArr;
    if (PNULL == dlHarqProcess_p)
    {
        LTE_MAC_UT_LOG(LOG_WARNING, LOG_MULTIPLEXER,
                "[%s] dlHarqProcess_p is NULL hence returning \n",
                __func__);
        return retVal;
    }
    if (tbIdentifier == TB_ONE)
    {
        dlHarqTBInfo_p = &dlHarqProcess_p->dlHARQTBOneInfo;
        dlHarqOtherTBInfo_p = &dlHarqProcess_p->dlHARQTBTwoInfo;
        paddingBytesForTB_p = &transmissionAndRLCData_p->paddingBytesForTBOne;
        lcCountOfTB_p = &transmissionAndRLCData_p->lcCountTBOne;

    }
    else if (tbIdentifier == TB_TWO)
    {
        dlHarqOtherTBInfo_p = &dlHarqProcess_p->dlHARQTBOneInfo;
        dlHarqTBInfo_p = &dlHarqProcess_p->dlHARQTBTwoInfo;
        paddingBytesForTB_p = &transmissionAndRLCData_p->paddingBytesForTBTwo;
        lcCountOfTB_p = &transmissionAndRLCData_p->lcCountTBTwo;
    }
    else
    {
       lteWarning("Invalid tbIdentifier\n");

        dlQoSResetCCEsAndHarqProcess( ueContext_p,
                                      transmissionAndRLCData_p);
        return retVal;
    }
    availableTBSize = dlHarqTBInfo_p->tbSize;

    if (availableTBSize < FIVE_BYTES)
    {
        if (dlHarqOtherTBInfo_p->txIndicator)
        {
            if(SCELL_ACT_DEACT_MSG == dlHarqTBInfo_p->macScellCEFlag)
            {
                putEntryInPendingFailureDlCAMacCEQueue( ueContext_p->ueIndex,
                                                        dlHarqProcess_p,
                                                        internalCellIndex);
            }

            dciContainerIndex = transmissionAndRLCData_p->scheduledTick % MAX_CCE_CONTAINER;
           /* SPR 19625 Start */ 
            dciCCEInfo_p = &((dciCCEContainerInfo_gp[transmissionAndRLCData_p->encoderCellIndex] + dciContainerIndex)->
                    dciCCEInfo[transmissionAndRLCData_p->cceIndex]);
            /* Cyclomatic Complexity changes */
            mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                    dciCCEInfo_p,transmissionAndRLCData_p->encoderCellIndex);
            /*pdcch for scell changes start*/
            mac_reset_extra_cce( (dciCCEContainerInfo_gp[transmissionAndRLCData_p->encoderCellIndex] + dciContainerIndex),dciCCEInfo_p,ueContext_p,transmissionAndRLCData_p->encoderCellIndex);
            /*pdcch for scell changes end*/
            dlHarqTBInfo_p->transmissionAndRLCData_p = PNULL;

            /*SPR 12070 Fix Start */
            /* If Both the TB's are valid we only need to free the TB resources, no need to free
             * the corresponding Harq Process */ 
            if ((dlHarqProcess_p->isTBOneValid) && (dlHarqProcess_p->isTBTwoValid))
            {
                freeDLHarqTB(
                        ueContext_p,
                        dlHarqProcess_p,
                        tbIdentifier,transmissionAndRLCData_p->encoderCellIndex);
            }
            else
            {
                freeDLHarqProcessTBSpecific(
                        ueContext_p,
                        dlHarqProcess_p,
                        tbIdentifier,transmissionAndRLCData_p->encoderCellIndex);
            }
            /* SPR 12070 Fix End */
            freeMemPool(transmissionAndRLCData_p);
        }

        LOG_MAC_MSG(MAC_AVAILABLE_TB_LESS_THAN_THREE_BYTES,LOGDEBUG,
        MAC_MUX,getCurrentTick(), 
        (UInt32)ueContext_p->ueIndex,
        (UInt32)availableTBSize,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
        DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
	"TB1 less than 3 bytes");
        /* SPR# 4906 Changes Starts  */
        return retVal;
    }

    switch(dlHarqTBInfo_p->taDrxFlag)
    {
        case TA_MSG:
        availableTBSize -= TWO_BYTES;
        break;

        case DRX_MSG:
            availableTBSize -= ONE_BYTE;
        /* + SPR 22352 fix */
        break;
        case TA_DRX_MSG:
        availableTBSize -= THREE_BYTES;
        break;
        /* - SPR 22352 fix */
        default:
        LTE_MAC_UT_LOG(LOG_INFO, LOG_MULTIPLEXER,
                "\n No FLAG was set and hence no allocated in the TB\n ");
    }

    if(SCELL_ACT_DEACT_MSG == dlHarqTBInfo_p->macScellCEFlag)
    {
        availableTBSize -= LENGTH_OF_SCELL_ACT_DEACT_CE;
    }

    loopCount = ueContext_p->scheduledLcCount;

    dlQosStrategyLCNodevoid_p = (void *)
                              getFirstListNode(&ueContext_p->dlScheduledLcList);
    
    if ( PNULL == dlQosStrategyLCNodevoid_p )
    {
        /* SPR# 5189 Changes Starts */
        dciContainerIndex = transmissionAndRLCData_p->scheduledTick % MAX_CCE_CONTAINER;
        dciCCEInfo_p = &((dciCCEContainerInfo_gp[transmissionAndRLCData_p->encoderCellIndex] + dciContainerIndex)->
                dciCCEInfo[transmissionAndRLCData_p->cceIndex]);
        /* Cyclomatic Complexity changes */
        mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                dciCCEInfo_p,transmissionAndRLCData_p->encoderCellIndex);
        /*pdcch for scell changes start*/
        mac_reset_extra_cce( (dciCCEContainerInfo_gp[transmissionAndRLCData_p->encoderCellIndex] + dciContainerIndex),dciCCEInfo_p,ueContext_p,transmissionAndRLCData_p->encoderCellIndex);
        /*pdcch for scell changes end*/
        dlHarqTBInfo_p->transmissionAndRLCData_p = PNULL;

        /*SPR 12070 Fix Start */
        /* If Both the TB's are valid we only need to free the TB resources, no need to free
         * the corresponding Harq Process */ 
        if (dlHarqProcess_p->isTBOneValid && dlHarqProcess_p->isTBTwoValid)
        {
            freeDLHarqTB(
                    ueContext_p,
                    dlHarqProcess_p,
                    tbIdentifier,transmissionAndRLCData_p->encoderCellIndex);
        }
        else
        {
            freeDLHarqProcessTBSpecific(
                    ueContext_p,
                    dlHarqProcess_p,
                    tbIdentifier,internalCellIndex);
        }
        /* SPR 12070 Fix End */
        freeMemPool(transmissionAndRLCData_p);
           /* SPR 19625 End */ 


        /* SPR# 5189 Changes Ends */
        LOG_MAC_MSG(MAC_QOS_LC_NODE_NULL,LOGFATAL,
        MAC_MUX,getCurrentTick(), 
        (UInt32)ueContext_p->ueIndex,
        (UInt32)loopCount,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
        DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
	" First LC Node dlQosStrategyLCNode_p is NULL");
        /* SPR# 4906 Changes Starts  */

        return retVal;
    }        

    DL_QOS_GET_BASE_ADDRESS_FROM_SCHEDULED_LC_ANCHOR(
                                           dlQosStrategyLCNodevoid_p,
                                           dlQosStrategyLCNode_p);
    /* Cyclomatic complexity changes start */
    if(MAC_SUCCESS != fillRLCDataBufferMacUEIndForSingleTB(ueContext_p,dlQosStrategyLCNode_p,
                &availableTBSize, rlcDataBufferArr_p,
                &lcCount,lcCountOfTB_p,paddingBytesForTB_p,
                transmissionAndRLCData_p, internalCellIndex,dlHarqProcess_p))
    {
        /* This func will return failure only if Opp is already send to RLC. 
           So returning SUCCESS from here itself. */
        return MAC_SUCCESS;
    }
    /* Cyclomatic complexity changes end */

    transmissionAndRLCData_p->numValidTBs++;

    /* Second traversal for padding removal */
    if ( availableTBSize > MAC_MIN_SDU_SIZE)
    {
        dlQosStrategyLCNodevoid_p = (void *)
                              getFirstListNode(&ueContext_p->dlScheduledLcList);
        if ( PNULL == dlQosStrategyLCNodevoid_p )
        {
            LOG_MAC_MSG(MAC_QOS_LC_NODE_NULL,LOGFATAL,
                    MAC_MUX,getCurrentTick(),
                    (UInt32)ueContext_p->ueIndex,
                    (UInt32)loopCount,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
                    " First LC Node dlQosStrategyLCNode_p is NULL");
            dlQoSResetCCEsAndHarqProcess( ueContext_p,
                                      transmissionAndRLCData_p);
            return MAC_FAILURE;
        }
        DL_QOS_GET_BASE_ADDRESS_FROM_SCHEDULED_LC_ANCHOR(
                dlQosStrategyLCNodevoid_p,
                dlQosStrategyLCNode_p);
    }
    loopCount =  ueContext_p->scheduledLcCount;
    while(availableTBSize > MAC_MIN_SDU_SIZE && PNULL != dlQosStrategyLCNode_p && loopCount > 0)
    {
        /*CA_CHG_START*/
        /* + SPR 12349 Changes */
        tempLcCount = 0;
        while(tempLcCount < lcCount)
        {
            if ( rlcDataBufferArr_p[tempLcCount].lcID == dlQosStrategyLCNode_p->lcId )
            {
                tempQueueLoad = dlQosStrategyLCNode_p->lcInfo_p->queueLoad;
                if ( tempQueueLoad > dlQosStrategyLCNode_p->requestedDataSize )
                {
                    /*SPR 12789 Start */
                    remainingQueueLoad =  (SInt32)(tempQueueLoad - dlQosStrategyLCNode_p->requestedDataSize); 
                    /*SPR 12789 End */
                    if ( remainingQueueLoad > availableTBSize )
                    {
                        rlcDataBufferArr_p[tempLcCount].requestedDataSize += availableTBSize;
                        dlQosStrategyLCNode_p->requestedDataSize += availableTBSize;
                        availableTBSize = 0;
                    }
                    else
                    {
                        rlcDataBufferArr_p[tempLcCount].requestedDataSize += remainingQueueLoad;
                        dlQosStrategyLCNode_p->requestedDataSize += remainingQueueLoad;
                        availableTBSize -= remainingQueueLoad;
                    }
                }
                break;
            }
            tempLcCount++;
        }
        if(tempLcCount >= lcCount)
        {
            tempQueueLoad = (dlQosStrategyLCNode_p->lcInfo_p->queueLoad - dlQosStrategyLCNode_p->requestedDataSize);
            if ((SInt32) tempQueueLoad  > 0)
            {
                tempQueueLoad += dlQosStrategyLCNode_p->lcInfo_p->dlLcQosInfo.rlcFixedHeaderSize;

                rlcDataBufferArr_p[lcCount].requestedDataSize = 0;
                rlcDataBufferArr_p[lcCount].lcID = dlQosStrategyLCNode_p->lcId;
                /* SPR 13492 Changes Start */
#ifdef KPI_STATS
                qci= dlQosStrategyLCNode_p->lcInfo_p->dlLcQosInfo.QCI;
                if(dlQosStrategyLCNode_p->lcId >= MAC_LCID_3) /* SPR 13947 Changes */
                    dlUECtxInfoArr_g[ueContext_p->ueIndex].
                        inSessionTimePerQCIFlag[qci - 1] = TRUE; /* SPR 14267 Changes */
#endif
                /* SPR 13492 Changes End */
                if (tempQueueLoad > availableTBSize )
                {
                    availableTBSize -= THREE_BYTES;
                    rlcDataBufferArr_p[lcCount].requestedDataSize = availableTBSize;
                    dlQosStrategyLCNode_p->requestedDataSize += availableTBSize; 
                    availableTBSize = 0;
                }
                else
                {
                    if (tempQueueLoad + THREE_BYTES <= availableTBSize)
                    {
                        availableTBSize -= (tempQueueLoad + THREE_BYTES); 
                        rlcDataBufferArr_p[lcCount].requestedDataSize = tempQueueLoad;
                        dlQosStrategyLCNode_p->requestedDataSize += tempQueueLoad; 
                    }
                    else
                    {
                        availableTBSize -= THREE_BYTES;
                        rlcDataBufferArr_p[lcCount].requestedDataSize = availableTBSize; 
                        dlQosStrategyLCNode_p->requestedDataSize += availableTBSize; 
                        availableTBSize = 0;
                    }
                }
                lcCount++;
            }
        }
        /* - SPR 12349 Changes */
        /*CA_CHG_END*/
        dlQosStrategyLCNodevoid_p = (void *)
            getNextListNode(&dlQosStrategyLCNode_p->scheduledLcAnchor);
        if( PNULL == dlQosStrategyLCNodevoid_p )
        {
            break;
        }    
        DL_QOS_GET_BASE_ADDRESS_FROM_SCHEDULED_LC_ANCHOR(
                dlQosStrategyLCNodevoid_p,
                dlQosStrategyLCNode_p);
        loopCount--;
    }
    *lcCountOfTB_p = lcCount;
    *paddingBytesForTB_p = availableTBSize;
/*SPR 18861 fix start */
/*Code Removed as the code to fill RB is in encoder in function processEncoderNode*/
/*SPR 18861 fix end */

    /** SPR 3657 Changes Start **/
    INIT_RLC_RETX_Q(transmissionAndRLCData_p,lcCount);
    
    /*ca chngs start*/
    /*check for the bitmask in harq for act/deact state event of scell */
/*SPR 17561 FIX*/
    /*SPR 16026 changes start*/
#ifdef FDD_CONFIG
    checkScellActDeactBitmask(dlHarqProcess_p,ueContext_p);
    /*SPR 16026 changes end*/
#else
    checkScellActDeactBitmask(dlHarqProcess_p,ueContext_p,harqSubFrameNum,scheduledExpiryTick);
#endif
/*SPR 17561 FIX*/
    /*ca chngs end*/  
    /** SPR 3657 Changes End **/
    /* + SPR 13605 Fix */
    if(lcCount == 0 && 
        (!( (dlHarqProcess_p->isTBOneValid && dlHarqProcess_p->dlHARQTBOneInfo.txIndicator == RE_TX)||
          (dlHarqProcess_p->isTBTwoValid && dlHarqProcess_p->dlHARQTBTwoInfo.txIndicator == RE_TX))))
    {
      /* SPR 23616 fix starts */
        /*Not calling this function for eMTC UE was causing HARQ LEAK*/
		/*Changes have been made in the below function to avoid certain actions for eMTC UE
		 * and free the HARQ in case of both eMTC and LTE*/ 

        dlQoSResetCCEsAndHarqProcess( ueContext_p,
                                      transmissionAndRLCData_p);
      /* SPR 23616 fix ends */
        retVal = MAC_FAILURE;

    }
    else
    {
        macUeOpportunityInd(ueContext_p->ueIndex,
                lcCount,
                (RLCDataBuffer*) transmissionAndRLCData_p,
                transmissionAndRLCData_p->scheduledTick,
                internalCellIndex);
        retVal = MAC_SUCCESS;
    }
    /* - SPR 13605 Fix */
    /* CA_Phase3_UE_Inactivity_changes start */
    /*SPR 21133 Changes Start */
    // ueContext_p->lastScheduledTick = getCurrentTick();
    /*SPR 21133 Changes End */
    /* CA_Phase3_UE_Inactivity_changes end */

    return retVal;
}
/*****************************************************************************
 * Function Name  :  fillRLCDataBufferMacUEIndForSingleTB 
 * Inputs         :  ueContext_p - Pointer to the DLUEContext,
 *                   dlQosStrategyLCNode_p,
 *                   availableTBSize_p,
 *                   rlcDataBufferArr_p,
 *                   lcCount_p,
 *                   lcCountOfTB_p,
 *                   paddingBytesForTB_p,
 *                   transmissionAndRLCData_p - pointer to the transmitted RLC
 *                                              data,
 *                   internalCellIndex  
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function fills the RLC data buffer with relevant 
 *                   information and sends it to the RLC through Mac Ue 
 *                   Indication for Single TB.
 *****************************************************************************/
STATIC  MacRetType fillRLCDataBufferMacUEIndForSingleTB(  
            DLUEContext *ueContext_p ,
            DLQosStrategyLCNode *dlQosStrategyLCNode_p,
            UInt32 *availableTBSize_p,
            RLCDataBuffer * rlcDataBufferArr_p,
            UInt8 *lcCount_p,
            UInt8* lcCountOfTB_p,
            UInt32* paddingBytesForTB_p,
            TransmissionAndRLCData * transmissionAndRLCData_p,
            InternalCellIndex internalCellIndex,
            DLHARQProcess* dlHarqProcess_p)
{
    SInt8 loopCount = ueContext_p->scheduledLcCount;
    UInt32          bytesAllocated = 0;
    void *dlQosStrategyLCNodevoid_p = PNULL;  
/*ca-tdd change end*/
/*SPR 17561 FIX*/
#ifdef TDD_CONFIG
    UInt32 sCellStateChangeTddTimerOffset = 0; 
    sCellStateChangeTddTimerOffset = getSCellStateChangeTddTimerOffset(ueContext_p->internalCellIndex);
    UInt32 scheduledExpiryTick = getCurrentTick() + PHY_DL_DELAY + sCellStateChangeTddTimerOffset;
    UInt8  harqSubFrameNum = (scheduledExpiryTick)%MAX_SUB_FRAME;
#endif
/*SPR 17561 FIX*/
    /* SPR 13492 Changes Start */
#ifdef KPI_STATS
    UInt8 qci = INVALID_QCI;
#endif
    /* SPR 13492 Changes End */
    
    while( PNULL != dlQosStrategyLCNode_p && loopCount > 0 )
    {
        /* SPR 7188 Changes Starts */
        /*CA_CHG_START*/
        /* + SPR 12349 Changes */
        if((dlQosStrategyLCNode_p->schedulableBytes) > MAC_MIN_SDU_SIZE)
        {
            /*CA_CHG_END*/

            if ( *availableTBSize_p >= MAC_MIN_SDU_SIZE )
                /* SPR 7188 Changes ends */
            {            
                bytesAllocated = 0;

                if ( *availableTBSize_p >= dlQosStrategyLCNode_p->schedulableBytes )
                {
                    *availableTBSize_p = *availableTBSize_p - dlQosStrategyLCNode_p->schedulableBytes;
                    bytesAllocated = dlQosStrategyLCNode_p->schedulableBytes;
                    dlQosStrategyLCNode_p->schedulableBytes = 0;

                }
                else if (*availableTBSize_p  < dlQosStrategyLCNode_p->schedulableBytes)
                {
                    bytesAllocated = *availableTBSize_p;
                    dlQosStrategyLCNode_p->schedulableBytes -= (*availableTBSize_p);  
                    *availableTBSize_p = 0;
                }
                bytesAllocated -= THREE_BYTES;

                /*  RLC - ReArch changes start */
                rlcDataBufferArr_p[*lcCount_p].statusPdu_p = PNULL; 
                rlcDataBufferArr_p[*lcCount_p].statusPduLen = 0;
                rlcDataBufferArr_p[*lcCount_p].retxRlcPdu_p = PNULL; 
                rlcDataBufferArr_p[*lcCount_p].retxPduLen = 0;
                rlcDataBufferArr_p[*lcCount_p].newRlcPdu_p = PNULL; 
                rlcDataBufferArr_p[*lcCount_p].newPduLen = 0;
                /*  RLC - ReArch changes end */
                rlcDataBufferArr_p[*lcCount_p].remainingDataSize = 0;
                rlcDataBufferArr_p[*lcCount_p].requestedDataSize = 
                    bytesAllocated;
                rlcDataBufferArr_p[*lcCount_p].lcID = 
                    dlQosStrategyLCNode_p->lcId;
                /* SPR 13492 Changes Start */
#ifdef KPI_STATS
                qci= dlQosStrategyLCNode_p->lcInfo_p->dlLcQosInfo.QCI;
                if(dlQosStrategyLCNode_p->lcId >= MAC_LCID_3)  /* SPR 13947 Changes */
                    dlUECtxInfoArr_g[ueContext_p->ueIndex].
                        inSessionTimePerQCIFlag[qci - 1] = TRUE; /* SPR 14267 Changes */
#endif
                /* SPR 13492 Changes End */
                *lcCount_p = *lcCount_p + 1;

                dlQosStrategyLCNode_p->requestedDataSize += bytesAllocated;
            }
            else
            {
                *lcCountOfTB_p = *lcCount_p;
                *paddingBytesForTB_p = *availableTBSize_p;
                transmissionAndRLCData_p->numValidTBs++;
                /** SPR 3657 Changes Start **/
                INIT_RLC_RETX_Q(transmissionAndRLCData_p,*lcCount_p);
                /** SPR 3657 Changes End **/

                /*ca chngs start*/
                /*check for the bitmask in harq for act/deact state event of scell */
/*SPR 17561 FIX*/
#ifdef FDD_CONFIG
                /*SPR 16026 changes start*/
                checkScellActDeactBitmask(dlHarqProcess_p,ueContext_p);
                /*SPR 16026 changes end*/
                /*ca chngs end*/ 
#else
    checkScellActDeactBitmask(dlHarqProcess_p,ueContext_p,harqSubFrameNum,scheduledExpiryTick);
#endif
/*SPR 17561 FIX*/

                macUeOpportunityInd(ueContext_p->ueIndex,
                        *lcCount_p,
                        (RLCDataBuffer*) transmissionAndRLCData_p,
                        transmissionAndRLCData_p->scheduledTick,
                        internalCellIndex);

                return MAC_FAILURE; /* this fail is just introduced for cyclomatic
                                     * complexity change as return type of calling
                                     * func is void */ 
            }
        } 
        /* - SPR 12349 Changes */

        loopCount--;

        dlQosStrategyLCNodevoid_p = (void *)
            getNextListNode(&dlQosStrategyLCNode_p->scheduledLcAnchor);
        if ( PNULL == dlQosStrategyLCNodevoid_p )
        {
            break;
        }
        DL_QOS_GET_BASE_ADDRESS_FROM_SCHEDULED_LC_ANCHOR(
                dlQosStrategyLCNodevoid_p,
                dlQosStrategyLCNode_p);
    }
    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  multiplexLcForTwoTB
 * Inputs         :  availableTBOneSize - TB Size for TB one,
 *                   availableTBTwoSize - TB Size for TB Two,
 *                   transmissionAndRLCData_p - pointer to the structure
 *                                              TransmissionAndRLCData,
 *                   ueContext_p - pointer to DLUEContext
 *                   dlHarqProcess_p -pointer to DLHARQProcess 
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function fills the relevant information for 
 *                   multiplexing and send the Indication to the RLC.
 *****************************************************************************/
MacRetType multiplexLcForTwoTB(
        UInt32 availableTBOneSize,
        UInt32 availableTBTwoSize,
        TransmissionAndRLCData* transmissionAndRLCData_p,
        DLUEContext *ueContext_p,
	/* SPR 10157 changes start */
        DLHARQProcess *dlHarqProcess_p,
	/* SPR 10157 changes end */
        InternalCellIndex internalCellIndex)
{
    UInt32 availableTBSize[MAX_NUM_TB] = {0}; 
    RLCDataBuffer * rlcDataBufferArr_p = PNULL;
    UInt32 loopCount = 0;
    UInt32 localNumTB = MAX_NUM_TB;
    UInt32 lcCount = 0;
    DLQosStrategyLCNode *dlQosStrategyLCNode_p = PNULL;
    /* SPR# 3896 Changes Starts */
    SInt32 bytesAllocated = 0;
    /* SPR# 3896 Changes Ends */
    UInt8 reDoForNextTB = 0;
    UInt8* lcCountOfTB_p = PNULL;
    UInt32* paddingBytesForTB_p = PNULL;
    UInt32  arrayIndex = 0;
    void *dlQosStrategyLCNodevoid_p = PNULL;
/*SPR 17561 FIX*/
#ifdef TDD_CONFIG
    UInt32 sCellStateChangeTddTimerOffset = 0;
    sCellStateChangeTddTimerOffset = getSCellStateChangeTddTimerOffset(ueContext_p->internalCellIndex);
    UInt32 scheduledExpiryTick = getCurrentTick() + PHY_DL_DELAY + sCellStateChangeTddTimerOffset;
    UInt8  harqSubFrameNum = (scheduledExpiryTick)%MAX_SUB_FRAME;
#endif
/*SPR 17561 FIX*/
   /* SPR 4704 Start */    
    transmissionAndRLCData_p->numValidTBs = MAX_NUM_TB;
    /* SPR 4704 End */    
    /* SPR 10157 changes start */
    UInt8 dciContainerIndex = 0;
    DciCCEInfo* dciCCEInfo_p = PNULL;
    /* SPR 10157 changes start */
    MacRetType retVal = MAC_FAILURE;
    /*CA_CHG_START*/
    /* For padding bytes adjusements */
    availableTBSize[1] = availableTBOneSize;
    availableTBSize[0] = availableTBTwoSize;
    rlcDataBufferArr_p = transmissionAndRLCData_p->rlcDataBufferArr;
    transmissionAndRLCData_p->lcCountTBOne = 0;
    transmissionAndRLCData_p->lcCountTBTwo = 0;
    transmissionAndRLCData_p->paddingBytesForTBOne = 0;
    transmissionAndRLCData_p->paddingBytesForTBTwo = 0;
/*CA_CHG_END*/

    /* SPR 13492 Changes Start */
#ifdef KPI_STATS
    UInt8 qci = INVALID_QCI;
#endif
    /* SPR 13492 Changes End */

    /* SPR 7188 Changes starts */
    if (availableTBOneSize >= MAC_MIN_SDU_SIZE)
    /* SPR 7188 Changes ends */
    {
        paddingBytesForTB_p = &transmissionAndRLCData_p->paddingBytesForTBOne;
        lcCountOfTB_p = &transmissionAndRLCData_p->lcCountTBOne;
    }
    /* SPR 7188 Changes starts */
    else if (availableTBTwoSize >= MAC_MIN_SDU_SIZE)
    /* SPR 7188 Changes ends */
    {

        localNumTB = 1;     
        paddingBytesForTB_p = &transmissionAndRLCData_p->paddingBytesForTBTwo;
        lcCountOfTB_p = &transmissionAndRLCData_p->lcCountTBTwo;

    }
    else
    {
        ltePanic("[%s] For both the TB the TB SIZE is "
                 "less than MAC_MIN_SDU_SIZE",__func__);
        return retVal;
    }

    loopCount = ueContext_p->scheduledLcCount;
    dlQosStrategyLCNodevoid_p = (void *)
                              getFirstListNode(&ueContext_p->dlScheduledLcList);
    if ( PNULL == dlQosStrategyLCNodevoid_p )
    {
        /* SPR 10157 changes start */
        dciContainerIndex = transmissionAndRLCData_p->scheduledTick %
            MAX_CCE_CONTAINER;
        
           /* SPR 19625 Start */ 
        dciCCEInfo_p = &((dciCCEContainerInfo_gp[transmissionAndRLCData_p->encoderCellIndex] + dciContainerIndex)->
                dciCCEInfo[transmissionAndRLCData_p->cceIndex]);

        mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                dciCCEInfo_p,transmissionAndRLCData_p->encoderCellIndex);
        /*pdcch for scell changes start*/
        mac_reset_extra_cce( (dciCCEContainerInfo_gp[transmissionAndRLCData_p->encoderCellIndex] + dciContainerIndex),dciCCEInfo_p,ueContext_p,transmissionAndRLCData_p->encoderCellIndex);
        /*pdcch for scell changes end*/
        dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
        dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;
        freeDLHarqProcess(ueContext_p,
                dlHarqProcess_p,transmissionAndRLCData_p->encoderCellIndex);
        freeMemPool(transmissionAndRLCData_p);
           /* SPR 19625 End */ 
        /* SPR 10157 changes end */

        LOG_MAC_MSG(MAC_QOS_LC_NODE_NULL,LOGFATAL,
        MAC_MUX,getCurrentTick(), 
        (UInt32)ueContext_p->ueIndex,
        (UInt32)loopCount,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
        DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
	" First LC Node dlQosStrategyLCNode_p is NULL");
        return retVal;
    }        
    DL_QOS_GET_BASE_ADDRESS_FROM_SCHEDULED_LC_ANCHOR(
                                  dlQosStrategyLCNodevoid_p,
                                  dlQosStrategyLCNode_p);
    while( PNULL != dlQosStrategyLCNode_p && loopCount > 0 ) 
    {
        bytesAllocated = 0;
        /*SPR 4017 PH2 Start*/
        reDoForNextTB = 0;
        /*SPR 4017 PH2 End*/
        if(!dlQosStrategyLCNode_p->requestedDataSize)
        {
        do
        {           
            bytesAllocated = 0;
            /*CA_CHG_START*/
            if(dlQosStrategyLCNode_p->schedulableBytes) 
            {
                LOG_MAC_MSG(MAC_MUX_LC_1,LOGDEBUG,
                    MAC_MUX,getCurrentTick(), 
                    (UInt32)ueContext_p->ueIndex,
		    dlQosStrategyLCNode_p->lcId,
		    dlQosStrategyLCNode_p->schedulableBytes,
		    dlQosStrategyLCNode_p->requestedDataSize,
		    reDoForNextTB,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
                    "");

                LOG_MAC_MSG(MAC_MUX_LC_2,LOGDEBUG,
                    MAC_MUX,getCurrentTick(), 
		    availableTBSize[0],
		    availableTBSize[1],
		    transmissionAndRLCData_p->lcCountTBOne,
		    transmissionAndRLCData_p->lcCountTBTwo,
		    localNumTB,
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
                    "");

            /*CA_CHG_END*/
            if ( availableTBSize[localNumTB-1] >= dlQosStrategyLCNode_p->schedulableBytes )
            {
                /* SPR# 3896 Changes Starts */
                if(reDoForNextTB)
                {
                    if(availableTBSize[localNumTB-1] >= (UInt32)(MAC_MIN_SDU_SIZE + dlQosStrategyLCNode_p->schedulableBytes))
                    {
                        availableTBSize[localNumTB-1] -= (dlQosStrategyLCNode_p->schedulableBytes + MAC_MIN_SDU_SIZE);
                        bytesAllocated = dlQosStrategyLCNode_p->schedulableBytes + MAC_MIN_SDU_SIZE;
                    }
                    else if( ((UInt32)(MAC_MIN_SDU_SIZE + dlQosStrategyLCNode_p->schedulableBytes)) > availableTBSize[localNumTB-1])
                    {
                        bytesAllocated = availableTBSize[localNumTB-1];

                        availableTBSize[localNumTB-1] = 0;
                    }
                    else
                    {
                        /* this case will not occur*/
                        bytesAllocated =0;
                    }
                }
                /* SPR# 3896 Changes Starts */
                else
                {
                    availableTBSize[localNumTB-1] -= dlQosStrategyLCNode_p->schedulableBytes;
                    bytesAllocated = dlQosStrategyLCNode_p->schedulableBytes;
                }
            }
            else if ( availableTBSize[localNumTB-1] < dlQosStrategyLCNode_p->schedulableBytes )
            {
                bytesAllocated = availableTBSize[localNumTB-1];

                availableTBSize[localNumTB-1] = 0;
            }

            /* SPR# 3896 Changes Starts */
            
            reDoForNextTB = 0;
            if(bytesAllocated ) 
            {
                /*SPR 12789 Start */
                dlQosStrategyLCNode_p->schedulableBytes -= bytesAllocated;
                /*SPR 12789 End */
                /* SPR# 3896 Changes Ends */
                bytesAllocated -= THREE_BYTES;
                /*  RLC - ReArch changes start */
                rlcDataBufferArr_p[arrayIndex].statusPdu_p = PNULL; 
                rlcDataBufferArr_p[arrayIndex].statusPduLen = 0;
                rlcDataBufferArr_p[arrayIndex].retxRlcPdu_p = PNULL; 
                rlcDataBufferArr_p[arrayIndex].retxPduLen = 0;
                rlcDataBufferArr_p[arrayIndex].newRlcPdu_p = PNULL; 
                rlcDataBufferArr_p[arrayIndex].newPduLen = 0;
                /*  RLC - ReArch changes end */
                rlcDataBufferArr_p[arrayIndex].remainingDataSize = 0;
                rlcDataBufferArr_p[arrayIndex].requestedDataSize = \
                                                                   bytesAllocated;
                rlcDataBufferArr_p[arrayIndex].lcID =
                    dlQosStrategyLCNode_p->lcId;
                /* SPR 13492 Changes Start */
#ifdef KPI_STATS
                qci = dlQosStrategyLCNode_p->lcInfo_p->dlLcQosInfo.QCI;
                if(dlQosStrategyLCNode_p->lcId >= MAC_LCID_3)  /* SPR 13947 Changes */
                    dlUECtxInfoArr_g[ueContext_p->ueIndex].
                        inSessionTimePerQCIFlag[qci -1] = TRUE; /* SPR 14267 Changes */
#endif
                /* SPR 13492 Changes End */
                dlQosStrategyLCNode_p->requestedDataSize += bytesAllocated;
                lcCount++;
                arrayIndex++;
            }

            LOG_MAC_MSG(MAC_MUX_LC_3,LOGDEBUG,
                    MAC_MUX,getCurrentTick(), 
		    bytesAllocated,
		    dlQosStrategyLCNode_p->requestedDataSize,
		    dlQosStrategyLCNode_p->schedulableBytes,
		    availableTBSize[0],
		    availableTBSize[1],
                    DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
                    "");

            if (availableTBSize[localNumTB-1] <= MAC_MIN_SDU_SIZE )
            {
                *lcCountOfTB_p = lcCount;
                *paddingBytesForTB_p = availableTBSize[localNumTB-1];
                localNumTB--;
                 /* SPR 4704 Start */    
                 /* transmissionAndRLCData_p->numValidTBs++;*/
                 /* SPR 4704 End */    
                if ( localNumTB )
                {   
                    lcCountOfTB_p = &transmissionAndRLCData_p->lcCountTBTwo;
                    paddingBytesForTB_p = 
                            &transmissionAndRLCData_p->paddingBytesForTBTwo;
                    availableTBSize[localNumTB-1] = availableTBTwoSize;
                    lcCount = 0;
                    if ( dlQosStrategyLCNode_p->schedulableBytes)
                    {
                        reDoForNextTB = 1;
                    }
                }
                else
                {
                    /** SPR 3657 Changes Start **/
                    INIT_RLC_RETX_Q(transmissionAndRLCData_p,(transmissionAndRLCData_p->lcCountTBOne +
                        transmissionAndRLCData_p->lcCountTBTwo));
                    /** SPR 3657 Changes End **/
                    /*ca chngs start*/
                    /*check for the bitmask in harq for act/deact state event of scell */
/*SPR 17561 FIX*/
#ifdef FDD_CONFIG
                    /*SPR 16026 changes start*/
                    checkScellActDeactBitmask(dlHarqProcess_p,ueContext_p);
                    /*SPR 16026 changes end*/
#else
    checkScellActDeactBitmask(dlHarqProcess_p,ueContext_p,harqSubFrameNum,scheduledExpiryTick);
#endif
/*SPR 17561 FIX*/
                    /*ca chngs end*/  
		    LOG_MAC_MSG(MAC_MUX_LC_4,LOGDEBUG,
			    MAC_MUX,getCurrentTick(), 
			    (UInt32)ueContext_p->ueIndex,
			    dlQosStrategyLCNode_p->lcId,
			    transmissionAndRLCData_p->lcCountTBOne,
			    transmissionAndRLCData_p->lcCountTBTwo,
			    dlQosStrategyLCNode_p->requestedDataSize,
			    DEFAULT_FLOAT_VALUE,
			    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
			    "");

                    macUeOpportunityInd(
                            ueContext_p->ueIndex,
                            (transmissionAndRLCData_p->lcCountTBOne + \
                             transmissionAndRLCData_p->lcCountTBTwo),
                            (RLCDataBuffer*) transmissionAndRLCData_p,
                            transmissionAndRLCData_p->scheduledTick,
                            internalCellIndex);
                    /* CA_Phase3_UE_Inactivity_changes start */
		    /*SPR 21133 Changes Start */
                    //ueContext_p->lastScheduledTick = getCurrentTick();
		    /*SPR 21133 Changes End */
                    /* CA_Phase3_UE_Inactivity_changes end */
                    return MAC_SUCCESS;
                }                                                            
            }
            }

        }while(reDoForNextTB != 0 && dlQosStrategyLCNode_p != PNULL );          
        } //end of if(!dlQosStrategyLCNode_p->requestedDataSize)

        loopCount--;

        dlQosStrategyLCNodevoid_p = (void *)
                  getNextListNode(&dlQosStrategyLCNode_p->scheduledLcAnchor);
        if(PNULL == dlQosStrategyLCNodevoid_p)
        {
           break;
        }
        DL_QOS_GET_BASE_ADDRESS_FROM_SCHEDULED_LC_ANCHOR(
                                         dlQosStrategyLCNodevoid_p,
                                         dlQosStrategyLCNode_p);   
    }//end of while
    /*update lc count*/  
    *lcCountOfTB_p = lcCount;
    *paddingBytesForTB_p = availableTBSize[localNumTB-1];
    /*CA_CHG_START*/
    /*If availableTBSize is greater than MAC_MIN_SDU_SIZE then it will go for traversal 
      and Zero Index is represent for TB2 */
    
    /*SPR 12789 Start */
    if(availableTBSize[0] > MAC_MIN_SDU_SIZE )
        /*SPR 12789 End */
    {
       /*Function call for Second traversal for padding removal*/
       macDlRetraverseForPaddingRemove(availableTBSize,
                                     transmissionAndRLCData_p,
                                     ueContext_p,
                                     arrayIndex);
    }
    /*CA_CHG_END*/
    INIT_RLC_RETX_Q(transmissionAndRLCData_p,(transmissionAndRLCData_p->lcCountTBOne +
        transmissionAndRLCData_p->lcCountTBTwo));
    /** SPR 3657 Changes End **/

    
    /*ca chngs start*/
    /*check for the bitmask in harq for act/deact state event of scell */
/*SPR 17561 FIX*/
#ifdef FDD_CONFIG
    /*SPR 16026 changes start*/
    checkScellActDeactBitmask(dlHarqProcess_p,ueContext_p);
    /*SPR 16026 changes end*/
#else
    checkScellActDeactBitmask(dlHarqProcess_p,ueContext_p,harqSubFrameNum,scheduledExpiryTick);
#endif
/*SPR 17561 FIX*/
    /*ca chngs end*/  
    /* + SPR 13605 Fix */
    if(((transmissionAndRLCData_p->lcCountTBOne + transmissionAndRLCData_p->lcCountTBTwo) == 0)  &&
        (!( (dlHarqProcess_p->isTBOneValid && dlHarqProcess_p->dlHARQTBOneInfo.txIndicator == RE_TX)||
          (dlHarqProcess_p->isTBTwoValid && dlHarqProcess_p->dlHARQTBTwoInfo.txIndicator == RE_TX))))
    {
        dlQoSResetCCEsAndHarqProcess( ueContext_p,
                                      transmissionAndRLCData_p);
        retVal = MAC_FAILURE;
    }
    else
    {
        macUeOpportunityInd(ueContext_p->ueIndex,
                (transmissionAndRLCData_p->lcCountTBOne + 
                 transmissionAndRLCData_p->lcCountTBTwo),
                (RLCDataBuffer*) transmissionAndRLCData_p,
                transmissionAndRLCData_p->scheduledTick,
                internalCellIndex);
        retVal = MAC_SUCCESS;
    }
    /* - SPR 13605 Fix */
    /* CA_Phase3_UE_Inactivity_changes start */
    /*SPR 21133 Changes Start */
    //ueContext_p->lastScheduledTick = getCurrentTick();
    /*SPR 21133 Changes End */
    /* CA_Phase3_UE_Inactivity_changes end */

    return retVal;
}    
/*****************************************************************************
 * Function Name  :  prepareMultiplexOfLcForTwoTB
 * Inputs         : 
 *                   transmissionAndRLCData_p - pointer to the transmitted RLC data,
 *                   dlHarqProcess_p - Pointer to the DL HarqProcess,
 *                   ueContext_p - Pointer to the DL UeContext
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function prepares the structure with relevant 
 *                   information and calls multiplexLcForTwoTB for multiplexing.
 ******************************************************************************/
MacRetType prepareMultiplexOfLcForTwoTB(
       /* +- SPR 17777 */
       TransmissionAndRLCData * transmissionAndRLCData_p,
       DLHARQProcess* dlHarqProcess_p,
       DLUEContext *ueContext_p,
       InternalCellIndex internalCellIndex)
{
    UInt32 availableTBOneSize = 0;
    UInt32 availableTBTwoSize = 0;
    DLHARQTBInfo*   dlHarqTBInfo_p = PNULL;
    DciCCEInfo* dciCCEInfo_p = PNULL;
    UInt8 dciContainerIndex = 0;
    MacRetType retVal = MAC_FAILURE;

    availableTBOneSize = dlHarqProcess_p->dlHARQTBOneInfo.tbSize;
    availableTBTwoSize = dlHarqProcess_p->dlHARQTBTwoInfo.tbSize;

    dlHarqTBInfo_p = &dlHarqProcess_p->dlHARQTBOneInfo;

    if ( availableTBOneSize > FIVE_BYTES)
    {
        switch(dlHarqTBInfo_p->taDrxFlag)
        {
            case TA_MSG:
            availableTBOneSize -= TWO_BYTES;
            break;

            case DRX_MSG:
            availableTBOneSize -= ONE_BYTE;

            /* + SPR 22352 fix */
            break;
            case TA_DRX_MSG:
            availableTBOneSize -= THREE_BYTES;
            break;
            /* - SPR 22352 fix */
            default:
                LTE_MAC_UT_LOG(LOG_INFO, LOG_MULTIPLEXER,
                "\n No FLAG was for TA/DRX set and "
                "hence no allocation in the TB\n ");

        }

        if(SCELL_ACT_DEACT_MSG == dlHarqTBInfo_p->macScellCEFlag)
        {
            availableTBOneSize -= LENGTH_OF_SCELL_ACT_DEACT_CE;
        }

        if ( availableTBTwoSize >= FIVE_BYTES )
        {    
            dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p =
                                                   transmissionAndRLCData_p;
            dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p =
                                                   transmissionAndRLCData_p;

            retVal = multiplexLcForTwoTB(availableTBOneSize,
                                availableTBTwoSize,
                                transmissionAndRLCData_p,
                                ueContext_p,
				/* SPR 10157 changes start */
                                dlHarqProcess_p,
				/* SPR 10157 changes end */
                                internalCellIndex);

            return retVal;
        }
        else
        {
            dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p =
                                                     transmissionAndRLCData_p;
            dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;
            transmissionAndRLCData_p->paddingBytesForTBTwo = 
                                           transmissionAndRLCData_p->sizeTBTwo;
            transmissionAndRLCData_p->lcCountTBTwo = 0;

            retVal = multiplexLCForOneTB(
                    transmissionAndRLCData_p,
                    ueContext_p ,
                    dlHarqProcess_p,
                    TB_ONE,
                    internalCellIndex);

        }
    }
    else
    {
        dlHarqTBInfo_p = &dlHarqProcess_p->dlHARQTBTwoInfo;
        if (dlHarqTBInfo_p->taDrxFlag != INVALID_TA_DRX_MSG)
        {
            /* SPR 7188 Changes starts */
            if (availableTBTwoSize >= (FIVE_BYTES + MAC_MIN_SDU_SIZE))
            /* SPR 7188 Changes ends */
            {
                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p =
                                                   transmissionAndRLCData_p;

                switch(dlHarqTBInfo_p->taDrxFlag)
                {
                    case TA_MSG:
                    availableTBTwoSize -= TWO_BYTES;
                    break;

                    case DRX_MSG:
                    availableTBTwoSize -= ONE_BYTE; 
                    /* + SPR 22352 fix */
                    break;

                    case TA_DRX_MSG:
                    availableTBTwoSize -= THREE_BYTES;
                    break;
                    /* - SPR 22352 fix */
                    default:
                    LTE_MAC_UT_LOG(LOG_INFO, LOG_MULTIPLEXER,
                            "\n No FLAG was set and hence no allocation in the TB\n ");
                }
                if(SCELL_ACT_DEACT_MSG == dlHarqTBInfo_p->macScellCEFlag)
                {
                    availableTBOneSize -= LENGTH_OF_SCELL_ACT_DEACT_CE;
                }
            }
            else
            {
                if(SCELL_ACT_DEACT_MSG == dlHarqTBInfo_p->macScellCEFlag)
                {
                    putEntryInPendingFailureDlCAMacCEQueue( ueContext_p->ueIndex,
                                                            dlHarqProcess_p,
                                                            internalCellIndex);
                }

                dciContainerIndex = transmissionAndRLCData_p->scheduledTick % 
                                                             MAX_CCE_CONTAINER;
                /* SPR 19625 Start */ 
                dciCCEInfo_p = &((dciCCEContainerInfo_gp[transmissionAndRLCData_p->encoderCellIndex] + dciContainerIndex)->
                        dciCCEInfo[transmissionAndRLCData_p->cceIndex]);
                /* Cyclomatic Complexity changes */
                mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                        dciCCEInfo_p,transmissionAndRLCData_p->encoderCellIndex);
                /*pdcch for scell changes start*/
                mac_reset_extra_cce( (dciCCEContainerInfo_gp[transmissionAndRLCData_p->encoderCellIndex] + dciContainerIndex),dciCCEInfo_p,ueContext_p,transmissionAndRLCData_p->encoderCellIndex);
                /*pdcch for scell changes end*/
                dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;
                freeDLHarqProcess(ueContext_p,
                        dlHarqProcess_p,transmissionAndRLCData_p->encoderCellIndex);
                freeMemPool(transmissionAndRLCData_p);

                LOG_MAC_MSG(MAC_AVAILABLE_TB_LESS_THAN_THREE_BYTES,LOGDEBUG,
                    MAC_MUX,getCurrentTick(), 
                            (UInt32)ueContext_p->ueIndex,
                    (UInt32)availableTBTwoSize,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
                    "TB1 and TB2 less than three bytes");

                return retVal;
            }
        }
        else if(SCELL_ACT_DEACT_MSG == dlHarqTBInfo_p->macScellCEFlag)
        {
            if (availableTBTwoSize >= (TWO_BYTES + MAC_MIN_SDU_SIZE))
            {
                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p =
                                                   transmissionAndRLCData_p;

                if(SCELL_ACT_DEACT_MSG == dlHarqTBInfo_p->macScellCEFlag)
                {
                    availableTBOneSize -= LENGTH_OF_SCELL_ACT_DEACT_CE;
                }
            }
            else
            {
                if(SCELL_ACT_DEACT_MSG == dlHarqTBInfo_p->macScellCEFlag)
                {
                    putEntryInPendingFailureDlCAMacCEQueue( ueContext_p->ueIndex,
                                                            dlHarqProcess_p,
                                                            internalCellIndex);
                }
                dciContainerIndex = transmissionAndRLCData_p->scheduledTick % 
                                                             MAX_CCE_CONTAINER;
                dciCCEInfo_p = &((dciCCEContainerInfo_gp[transmissionAndRLCData_p->encoderCellIndex] + dciContainerIndex)->
                        dciCCEInfo[transmissionAndRLCData_p->cceIndex]);

                mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                        dciCCEInfo_p,transmissionAndRLCData_p->encoderCellIndex);

                /*pdcch for scell changes start*/
                mac_reset_extra_cce( (dciCCEContainerInfo_gp[transmissionAndRLCData_p->encoderCellIndex] + dciContainerIndex),dciCCEInfo_p,ueContext_p,transmissionAndRLCData_p->encoderCellIndex);
                /*pdcch for scell changes end*/
                dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;

                freeDLHarqProcess(ueContext_p,
                        dlHarqProcess_p,transmissionAndRLCData_p->encoderCellIndex);
                freeMemPool(transmissionAndRLCData_p);
                /* SPR 19625 End */ 

                LOG_MAC_MSG(MAC_AVAILABLE_TB_LESS_THAN_THREE_BYTES,LOGDEBUG,
                    MAC_MUX,getCurrentTick(), 
                            (UInt32)ueContext_p->ueIndex,
                    (UInt32)availableTBTwoSize,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
                    "TB1 and TB2 less than three bytes");

                return retVal;
            }
        }
        else
        {   
            /* SPR 7188 Changes starts */
            if (availableTBTwoSize >= MAC_MIN_SDU_SIZE)
            /* SPR 7188 Changes ends */
            {
                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p =
                                                       transmissionAndRLCData_p;
            }
            else
            {
                dciContainerIndex = transmissionAndRLCData_p->scheduledTick % 
                                                             MAX_CCE_CONTAINER;
                /* SPR 19625 Start */ 
                dciCCEInfo_p = &((dciCCEContainerInfo_gp[transmissionAndRLCData_p->encoderCellIndex] + dciContainerIndex)->
                        dciCCEInfo[transmissionAndRLCData_p->cceIndex]);
                /* Cyclomatic Complexity changes */
                mac_reset_cce(dciCCEInfo_p->dciPduInfo.aggregationLvl,
                        dciCCEInfo_p,transmissionAndRLCData_p->encoderCellIndex);
                /*pdcch for scell changes start*/
                mac_reset_extra_cce( (dciCCEContainerInfo_gp[transmissionAndRLCData_p->encoderCellIndex] + dciContainerIndex),dciCCEInfo_p,ueContext_p,transmissionAndRLCData_p->encoderCellIndex);
                /*pdcch for scell changes end*/
                dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;
                freeDLHarqProcess(ueContext_p,
                        dlHarqProcess_p,transmissionAndRLCData_p->encoderCellIndex);
                freeMemPool(transmissionAndRLCData_p);
                /* SPR 19625 End */ 

		        LOG_MAC_MSG(MAC_AVAILABLE_TB_LESS_THAN_THREE_BYTES,LOGDEBUG,
		            MAC_MUX,getCurrentTick(), 
                    (UInt32)ueContext_p->ueIndex,
        		    (UInt32)availableTBTwoSize,
	        	    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
		            DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
		            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
		            "TB1 and TB2 less than three bytes");

                return retVal;
            }

            dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
            transmissionAndRLCData_p->paddingBytesForTBOne = 
                                            transmissionAndRLCData_p->sizeTBOne;
            transmissionAndRLCData_p->lcCountTBOne = 0;

            retVal = multiplexLCForOneTB(
                    transmissionAndRLCData_p,
                    ueContext_p ,
                    dlHarqProcess_p,
                    TB_TWO,
                    internalCellIndex);

        }
    }
    return retVal;
}

/*****************************************************************************
 * Function Name  :  multiplexScheduledLcinQoS
 * Inputs         :  muxQueueNode_p - A pointer to the structure Multiplexer
 *                                    QueueNode which stores the information like
 *                                    ueIndex, HarqID etc,
 *                   scheduledTick - This information is simply passed by this
 *                                   function and is used by the encoder functionality.
 *                   ueContext_p - The Downlink UE Context Pointer.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function is called when the non dummy nodes are to be
 *                   sent to the RLC after multiplexing, in fact this function
 *                   will initate the entire logic of multiplexing depending 
 *                   upon the MIMO/Non-MIMO case.
 ******************************************************************************/
MacRetType multiplexScheduledLcinQoS(
        MultiplexerQueueNode  * muxQueueNode_p,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        DLUEContext* ueContext_p,
        InternalCellIndex internalCellIndex)
{
    DLHARQProcess* dlHarqProcess_p = PNULL;
    DLHARQTBInfo* dlHarqTbOneInfo_p = PNULL;
    DLHARQTBInfo* dlHarqTbTwoInfo_p = PNULL;
    TransmissionAndRLCData * transmissionAndRLCData_p = PNULL;
    /* _RLC_AM_MODE_ */
#ifdef  OBSELETE_REL_8    
    RLCHarqFailureInfo* tempRlcHarqFailureInfo = PNULL;
    UInt8 ii = 0;
#endif    
    MacRetType retVal = MAC_SUCCESS;

    GET_MEM_FROM_POOL(TransmissionAndRLCData,transmissionAndRLCData_p,sizeof(TransmissionAndRLCData),PNULL);

    if (PNULL == transmissionAndRLCData_p)
    {
        return MAC_FAILURE;
    }

    LTE_MAC_UT_LOG(LOG_INFO,LOG_MULTIPLEXER,"[%s] Entry",__func__);
    /* _RLC_AM_MODE_ */
#ifdef  OBSELETE_REL_8    
    tempRlcHarqFailureInfo = &transmissionAndRLCData_p->rlcHarqFailureInfo[0];
    for (ii=0; ii < ueContext_p->maxLcId; ii++)
    {
        tempRlcHarqFailureInfo[ii].lcId=INVALID_LCID;
        /*  RLC - ReArch changes start */
        tempRlcHarqFailureInfo[ii].newPduSeqNum=INVALID_SEQ_NUM;
        /*  RLC - ReArch changes end */
        tempRlcHarqFailureInfo[ii].SOStart=INVALID_SOS_START;
        tempRlcHarqFailureInfo[ii].SOEnd=INVALID_SOS_END;
        tempRlcHarqFailureInfo[ii].harqFailureReporting=HARQ_FAILURE_REPORTING_OFF;
    }
#endif    
    transmissionAndRLCData_p->endIndication = 0;
    transmissionAndRLCData_p->tbTwoLCStartIndex = 0;
    transmissionAndRLCData_p->numValidTBs = 0;
    /* + SPR 11469 Changes */
    if(IS_PCELL_OF_UE(ueContext_p,muxQueueNode_p->internalCellIndex))
    {
        dlHarqProcess_p =
            &ueContext_p->dlHarqContext_p->harqProcess[muxQueueNode_p->harqProcessId];
    }
    else
    {
        dlHarqProcess_p = &(ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p->
                harqProcess[muxQueueNode_p->harqProcessId]);
    }
    /* - SPR 11469 Changes */

    dlHarqTbOneInfo_p = &dlHarqProcess_p->dlHARQTBOneInfo;
    dlHarqTbTwoInfo_p = &dlHarqProcess_p->dlHARQTBTwoInfo;

    transmissionAndRLCData_p->cceIndex = muxQueueNode_p->cceIndex;
    transmissionAndRLCData_p->harqProcessId = muxQueueNode_p->harqProcessId;
    transmissionAndRLCData_p->scheduledTick = scheduledTick;

    transmissionAndRLCData_p->lcCountTBOne = 0;
    transmissionAndRLCData_p->lcCountTBTwo = 0;
    /** CA Phase2 Changes Start **/
/* + SPR 11208 Changes */
    if(IS_PCELL_OF_UE(ueContext_p,muxQueueNode_p->internalCellIndex))
/* - SPR 11208 Changes */
    {
        transmissionAndRLCData_p->encoderCellIndex = internalCellIndex;
    }
    else
    {
        transmissionAndRLCData_p->encoderCellIndex = !(internalCellIndex);
    }
    /** CA Phase2 Changes End **/


    /* Two TB transmission with both new Transmission */
    if ((dlHarqProcess_p->isTBOneValid) && (dlHarqProcess_p->isTBTwoValid)&& 
                                  (dlHarqTbOneInfo_p->txIndicator == NEW_TX) &&
                                     (dlHarqTbTwoInfo_p->txIndicator == NEW_TX))
    {
        transmissionAndRLCData_p->sizeTBOne = 
                                  dlHarqProcess_p->dlHARQTBOneInfo.tbSize;
        transmissionAndRLCData_p->sizeTBTwo = 
                                  dlHarqProcess_p->dlHARQTBTwoInfo.tbSize;

        /* transmissionStatusFlag = TB_ONE_VALID_AND_TB_TWO_VALID*/
        retVal = prepareMultiplexOfLcForTwoTB(
                /* +- SPR 17777 */
            transmissionAndRLCData_p,
            dlHarqProcess_p,
            ueContext_p,
            internalCellIndex);
    }
    else if ( dlHarqProcess_p->isTBOneValid &&
                           dlHarqTbOneInfo_p->txIndicator == NEW_TX)
    {
        /*transmissionStatusFlag = TB_ONE_VALID_AND_TB_TWO_INVALID;*/
        transmissionAndRLCData_p->sizeTBOne = 
                              dlHarqProcess_p->dlHARQTBOneInfo.tbSize;
        dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = 
                              transmissionAndRLCData_p;

        /* calling function for multiplexing with identifier as TB_ONE */
        retVal = multiplexLCForOneTB(
                transmissionAndRLCData_p,
                ueContext_p ,
                dlHarqProcess_p,
                TB_ONE,
                internalCellIndex);

    }
    else if ( dlHarqProcess_p->isTBTwoValid &&
                                dlHarqTbTwoInfo_p->txIndicator == NEW_TX)
    {
        /*transmissionStatusFlag = TB_ONE_INVALID_AND_TB_TWO_VALID;*/
        transmissionAndRLCData_p->sizeTBTwo = 
                               dlHarqProcess_p->dlHARQTBTwoInfo.tbSize;
        dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = 
                                             transmissionAndRLCData_p;

        /* calling function for multiplexing with identifier as TB_TWO */
        retVal = multiplexLCForOneTB(
                transmissionAndRLCData_p,
                ueContext_p ,
                dlHarqProcess_p,
                TB_TWO,
                internalCellIndex);
    }
    /* */
    /*Coverity 25488 Fix Start*/
    else
    {
	    freeMemPool(transmissionAndRLCData_p);
    } 
    /*Coverity 25488 Fix End*/
#ifdef SSI_DEBUG_STATS
    /* CA Stats Changes Start */
    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].qoSUELCStat[
        ueContext_p->ueIndex].dlTBSize[(getCurrentTick() % MAX_TTI_STATS)] = 
        dlHarqProcess_p->dlHARQTBOneInfo.tbSize;
    /* CA Stats Changes End */
#endif
    return retVal;
}
/* QOS MUX Change End */ 
 
/*****************************************************************************
 *  *  * Function Name  :  macMultiplexFillTBwithQload
 *  *  * Inputs         :  rlcDataBufferArr_p -    Pointer to structure
 *  *  *                                           RLCDataBuffer,
 *  *  *                   dlQosStrategyLCNode_p - Pointer to structure
 *  *  *                                           DLQosStrategyLCNode,
 *  *  *                   ueContext_p - pointer to DLUEContext,
 *  *  *                   arrayIndex -  Pointer to var arrayIndex
 *  *  *                   tbIndex - 
 *  *  * Outputs        :  None
 *  *  * Returns        :  It will return the array Index.
 *  *  * Description    :  This function fills TB1 and TB2 with Qload.
 *  *  *****************************************************************************/
static void macMultiplexFillTBwithQload(RLCDataBuffer * rlcDataBufferArr_p,
                                        DLQosStrategyLCNode *dlQosStrategyLCNode_p,
                                        UInt32 *availableTBSize,
                                        UInt32 *arrayIndex,
                                        UInt8 tbIndex,
                                        UInt32 *tempArrCount)
{
   UInt32 remainingQueueLoad = 0;
   while(*tempArrCount < *arrayIndex)
   {
    LOG_MAC_MSG(MAC_MUX_LC_5,LOGDEBUG,
	    MAC_MUX,getCurrentTick(), 
	    dlQosStrategyLCNode_p->lcId,
	    dlQosStrategyLCNode_p->lcInfo_p->queueLoad,
	    dlQosStrategyLCNode_p->requestedDataSize,
	    *(availableTBSize + tbIndex), 
	    *tempArrCount,
	    DEFAULT_FLOAT_VALUE,
	    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
	    "");
       if ( rlcDataBufferArr_p[*tempArrCount].lcID == dlQosStrategyLCNode_p->lcId )
       {
	   remainingQueueLoad = (dlQosStrategyLCNode_p->lcInfo_p->queueLoad -  dlQosStrategyLCNode_p->requestedDataSize); 
           /*SPR 12789 Start */
           if ( (SInt32 )remainingQueueLoad > 0 )  
               /*SPR 12789 End */
           {
               if ( remainingQueueLoad > *(availableTBSize + tbIndex))
               {
                   rlcDataBufferArr_p[*tempArrCount].requestedDataSize += *(availableTBSize + tbIndex);
                   dlQosStrategyLCNode_p->requestedDataSize += *(availableTBSize + tbIndex);
                   *(availableTBSize + tbIndex)= 0;
                   /*TB Two case if tbIndex = 0,it will break from loop*/
                   if(tbIndex == 0)
                   {
                     break;
                   }
               }
               else
               {
                   rlcDataBufferArr_p[*tempArrCount].requestedDataSize += remainingQueueLoad;
                   dlQosStrategyLCNode_p->requestedDataSize += remainingQueueLoad;
                   *(availableTBSize + tbIndex) -= remainingQueueLoad;
               }
           }
           break;
       }
       (*tempArrCount)++;
   }
}
/*****************************************************************************
 *  *  *  * Function Name  :  macMultiplexFillTBForNewEntry
 *  *  *  * Inputs         :  transmissionAndRLCData_p - Pointer to structure
 *  *  *  *                                              TransmissionAndRLCData,
 *  *  *  *                   dlQosStrategyLCNode_p - Pointer to structure
 *  *  *  *                                           DLQosStrategyLCNode,
 *  *  *  *                   availableTBSize  - Pointer to an 
 *  *  *  *                                      Array availableTBSize,
 *  *  *  *                   arrayIndex - Pointer to var arrayIndex,
 *  *  *  *                   tbIndex - var tbIndex,
 *  *  *  * Outputs        :  None
 *  *  *  * Returns        :  MAC_SUCCESS/MAC_FAILURE.
 *  *  *  * Description    :  This function fill TB1 and TB2 when Scheduble
 *  *  *  *                   Lc byte is zero,and new entry in RLC data
 *  *  *  *                   data buffer.
 *  *  *  *****************************************************************************/
static MacRetType macMultiplexFillTBForNewEntry(TransmissionAndRLCData* transmissionAndRLCData_p,
                                                  DLQosStrategyLCNode *dlQosStrategyLCNode_p,
                                                  UInt32 *availableTBSize,
                                                  UInt32 *arrayIndex,
                                                  UInt8 tbIndex) 
{
    MacRetType retVal = MAC_SUCCESS;
    UInt32 remainingQLoad = 0;
    RLCDataBuffer * rlcDataBufferArr_p = PNULL;
    rlcDataBufferArr_p = transmissionAndRLCData_p->rlcDataBufferArr;
    rlcDataBufferArr_p[*arrayIndex].requestedDataSize = 0;

    remainingQLoad = (dlQosStrategyLCNode_p->lcInfo_p->queueLoad -  dlQosStrategyLCNode_p->requestedDataSize); 
    LOG_MAC_MSG(MAC_MUX_LC_6,LOGDEBUG,
	    MAC_MUX,getCurrentTick(), 
	    dlQosStrategyLCNode_p->lcId,
	    dlQosStrategyLCNode_p->lcInfo_p->queueLoad,
	    dlQosStrategyLCNode_p->requestedDataSize,
	    *(availableTBSize + tbIndex), 
	    tbIndex,
	    DEFAULT_FLOAT_VALUE,
	    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
	    "");
                    /*SPR 12789 Start */
    if( (SInt32)remainingQLoad > 0) 
                    /*SPR 12789 End */
    {
        rlcDataBufferArr_p[*arrayIndex].lcID = dlQosStrategyLCNode_p->lcId;
        remainingQLoad += dlQosStrategyLCNode_p->lcInfo_p->dlLcQosInfo.rlcFixedHeaderSize ;
        *(availableTBSize + tbIndex) -= THREE_BYTES;
        /* + SPR 12119 Changes */
        if( remainingQLoad >= *(availableTBSize + tbIndex))
        /* - SPR 12119 Changes */
        {
            rlcDataBufferArr_p[*arrayIndex].requestedDataSize = *(availableTBSize + tbIndex);
            dlQosStrategyLCNode_p->requestedDataSize += *(availableTBSize + tbIndex);
            *(availableTBSize + tbIndex) = 0;
            /*In TB Two case if TB Size is zero update the LcCount and LcIndex and return Failure*/
            if(tbIndex == 0)
            {
               retVal = MAC_FAILURE;
            }
        }
        else
        {
            rlcDataBufferArr_p[*arrayIndex].requestedDataSize = remainingQLoad;
            dlQosStrategyLCNode_p->requestedDataSize += remainingQLoad;
            *(availableTBSize + tbIndex) -= remainingQLoad;
        }
        /*In TB One case update the LcCount and LcIndex and and also update Qload */
        if(tbIndex == 1)
        {
           transmissionAndRLCData_p->lcCountTBOne++;
        }
        /*In TB Two case update the LcCount and LcIndex and and also update Qload */
        else if(tbIndex == 0)
        {
           transmissionAndRLCData_p->lcCountTBTwo++;
        }
        (*arrayIndex)++;
    }
    return retVal;
}
/*****************************************************************************
 *  *  *  * Function Name  :  macMultiplexFillTBTwoAfterTBOne
 *  *  *  * Inputs         :  rlcDataBufferArr_p - Pointer to structure
 *  *  *  *                                        RLCDataBuffer 
 *  *  *  *                   dlQosStrategyLCNode_p - Pointer to structure
 *  *  *  *                                           DLQosStrategyLCNode,
 *  *  *  *                   availableTBSize - Pointer to an Array
 *  *  *  *                                     availableTBSize,
 *  *  *  *                   arrayIndex - Pointer to var arrayIndex
 *  *  *  * Outputs        :  None
 *  *  *  * Returns        :  MAC_SUCCESS/MAC_FAILURE.
 *  *  *  * Description    :  This function fills the TB2 after TB1 
 *  *  *  *                   becomes Full.
 *  *  *  *****************************************************************************/
static MacRetType macMultiplexFillTBTwoAfterTBOne(   /* SPR 11743 Start */
                                                     TransmissionAndRLCData* transmissionAndRLCData_p,
                                                     /* SPR 11743 End */
                                                     RLCDataBuffer * rlcDataBufferArr_p,
                                                     DLQosStrategyLCNode *dlQosStrategyLCNode_p,
                                                     UInt32 *availableTBSize,
                                                     UInt32 *arrayIndex)
{
    /* SPR 11743 Start */
    MacRetType retVal = MAC_FAILURE;
    /* SPR 11743 End */

    UInt32 remainingQueueLoad = (dlQosStrategyLCNode_p->lcInfo_p->queueLoad -
                                    dlQosStrategyLCNode_p->requestedDataSize);

    LOG_MAC_MSG(MAC_MUX_LC_7,LOGDEBUG,
	    MAC_MUX,getCurrentTick(), 
	    dlQosStrategyLCNode_p->lcId,
	    dlQosStrategyLCNode_p->lcInfo_p->queueLoad,
	    dlQosStrategyLCNode_p->requestedDataSize,
	    *(availableTBSize), 
	    *arrayIndex,
	    DEFAULT_FLOAT_VALUE,
	    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
	    "");
                    /*SPR 12789 Start */
    if((SInt32)remainingQueueLoad > 0) 
                    /*SPR 12789 End */
    {
        rlcDataBufferArr_p[*arrayIndex].requestedDataSize = 0;
        rlcDataBufferArr_p[*arrayIndex].lcID = dlQosStrategyLCNode_p->lcId;

        remainingQueueLoad += dlQosStrategyLCNode_p->lcInfo_p->dlLcQosInfo.rlcFixedHeaderSize;
        *availableTBSize -= THREE_BYTES;

        if(remainingQueueLoad > *availableTBSize )
        {
            rlcDataBufferArr_p[*arrayIndex].requestedDataSize = *availableTBSize;
            dlQosStrategyLCNode_p->requestedDataSize += *availableTBSize;
            *availableTBSize  = 0;
            /* SPR 11743 Start */
            retVal = MAC_FAILURE;
            /* SPR 11743 End */
        }
        else
        {
            rlcDataBufferArr_p[*arrayIndex].requestedDataSize = remainingQueueLoad;
            dlQosStrategyLCNode_p->requestedDataSize += remainingQueueLoad;
            *availableTBSize -= remainingQueueLoad;
            /* SPR 11743 Start */
            retVal = MAC_SUCCESS;
            /* SPR 11743 End */

        }
        /* SPR 11743 Start */
        (*arrayIndex)++;
        transmissionAndRLCData_p->lcCountTBTwo++;
    }
    else
    {
        retVal = MAC_SUCCESS;
    }
    return retVal; 
    /* SPR 11743 End */
}

/*****************************************************************************
 *  * Function Name  :  macDlRetraverseForPaddingRemove
 *  * Inputs         :  availableTBOneSize - Pointer to array 
 *  *                                        availableTBOneSize,
 *  *                   transmissionAndRLCData_p - pointer to the structure
 *  *                                              TransmissionAndRLCData,
 *  *                   ueContext_p - pointer to DLUEContext,
 *  *                   arrayIndex - index for fill LC
 *  * Outputs        :  None
 *  * Returns        :  None
 *  * Description    :  This function fills the relevant information 
 *  *                   in retravesing.
 *  *****************************************************************************/
STATIC void macDlRetraverseForPaddingRemove( UInt32 *availableTBSize,
                                    TransmissionAndRLCData* transmissionAndRLCData_p,
                                    DLUEContext *ueContext_p,
                                    UInt32 arrayIndex)
{
    RLCDataBuffer * rlcDataBufferArr_p = PNULL;
    UInt32 loopCount = 0;
    UInt32 localNumTB = MAX_NUM_TB;
    DLQosStrategyLCNode *dlQosStrategyLCNode_p = PNULL;
    void *dlQosStrategyLCNodevoid_p = PNULL;
    transmissionAndRLCData_p->numValidTBs = MAX_NUM_TB;
    UInt32 tempArrCount = 0;
    UInt8  tbIndex = 0;
    MacRetType retVal = MAC_SUCCESS;
    rlcDataBufferArr_p = transmissionAndRLCData_p->rlcDataBufferArr;
    rlcDataBufferArr_p[arrayIndex].statusPdu_p = PNULL;
    rlcDataBufferArr_p[arrayIndex].statusPduLen = 0;
    rlcDataBufferArr_p[arrayIndex].retxRlcPdu_p = PNULL;
    rlcDataBufferArr_p[arrayIndex].retxPduLen = 0;
    rlcDataBufferArr_p[arrayIndex].newRlcPdu_p = PNULL;
    rlcDataBufferArr_p[arrayIndex].newPduLen = 0;
    rlcDataBufferArr_p[arrayIndex].remainingDataSize = 0;
    /*Get the first node from Strategy schedule List*/

    LOG_MAC_MSG(MAC_MUX_LC_8,LOGDEBUG,
	    MAC_MUX,getCurrentTick(), 
	    ueContext_p->ueIndex,
	    availableTBSize[0],
	    availableTBSize[1],
	    ueContext_p->scheduledLcCount,
            DEFAULT_INT_VALUE,
	    DEFAULT_FLOAT_VALUE,
	    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
	    "");

    if (availableTBSize[localNumTB-1] > MAC_MIN_SDU_SIZE || availableTBSize[localNumTB-2] > MAC_MIN_SDU_SIZE)
    {
        dlQosStrategyLCNodevoid_p = (void *)
                              getFirstListNode(&ueContext_p->dlScheduledLcList);
        if ( PNULL == dlQosStrategyLCNodevoid_p )
        {
            LOG_MAC_MSG(MAC_QOS_LC_NODE_NULL,LOGFATAL,
                    MAC_MUX,getCurrentTick(),
                    (UInt32)ueContext_p->ueIndex,
                    (UInt32)loopCount,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,
                    " First LC Node dlQosStrategyLCNode_p is NULL");
             return;
        }
        DL_QOS_GET_BASE_ADDRESS_FROM_SCHEDULED_LC_ANCHOR(
                                           dlQosStrategyLCNodevoid_p,
                                           dlQosStrategyLCNode_p);
    }
    /*Total LC count in Schedule LC list*/
    loopCount =  ueContext_p->scheduledLcCount;
    /*Retravesal loop */
    while(PNULL != dlQosStrategyLCNode_p && loopCount > 0)
    {
                    /*SPR 12789 Start */
        if ((availableTBSize[localNumTB-1] > MAC_MIN_SDU_SIZE)  && (localNumTB == MAX_NUM_TB))
                    /*SPR 12789 End */
        {
           /* If TB1 Size is greater than MAC_MIN_SDU_SIZE,then fill both TB1 and TB2 */
           /*Fill data for TB1 when tbIndex = 1*/
           tbIndex = 1;
           /*Function call for fill TB1*/
           macMultiplexFillTBwithQload(rlcDataBufferArr_p,
                                       dlQosStrategyLCNode_p,
                                       availableTBSize,
                                       &arrayIndex,
                                       tbIndex,
                                       &tempArrCount);
           /*If schedule byte is Zero then LC and Qload  is added in RLC data buffer*/
           if(tempArrCount >= arrayIndex)
           {
               /*Function call to fill TB1 and new entry in Rlc data buffer when schedule byte data is zero*/
               if(availableTBSize[tbIndex] > MAC_MIN_SDU_SIZE)
               {
                   retVal  = macMultiplexFillTBForNewEntry(transmissionAndRLCData_p,
                                                           dlQosStrategyLCNode_p,
                                                           availableTBSize,
                                                           &arrayIndex,
                                                           tbIndex);
                   /*+ Coverity fix 73054*/
                   if(retVal == MAC_FAILURE)
                     break;
                   /*- Coverity fix 73054*/
               }
           }
           /*Fill the TB2 when TB1 size is less than MAC_MIN_SDU_SIZE*/
           if (availableTBSize[localNumTB-1] <= MAC_MIN_SDU_SIZE )
           {
                    /*SPR 12789 Start */
               if( (SInt32)(dlQosStrategyLCNode_p->lcInfo_p->queueLoad - 
                    /*SPR 12789 End */
                           dlQosStrategyLCNode_p->requestedDataSize) > 0 ) 

               {
                   /*Function call for fill TB2*/
                   /* SPR 11743 Start */
                   retVal = macMultiplexFillTBTwoAfterTBOne(transmissionAndRLCData_p,
                   /* SPR 11743 End */
                                                            rlcDataBufferArr_p,
                                                            dlQosStrategyLCNode_p,
                                                            availableTBSize,
                                                            &arrayIndex);
                   if(retVal == MAC_FAILURE)
                     break;
               }
            }
        }
        /*If TB1 is less than MAC_MIN_SDU_SIZE then fill only TB2*/
        else
        {
            tempArrCount = transmissionAndRLCData_p->lcCountTBOne;
            /* TB TWO case */
            localNumTB = 1;
            if (availableTBSize[localNumTB-1] > MAC_MIN_SDU_SIZE )
            {
               /*Fill TB2 when tbIndex = 0*/
               tbIndex = 0;
               /*Function call for fill TB2*/
               macMultiplexFillTBwithQload(rlcDataBufferArr_p,
                                           dlQosStrategyLCNode_p,
                                           availableTBSize,
                                           &arrayIndex,
                                           tbIndex,
                                           &tempArrCount);
               /*If schedule byte is Zero then LC and Qload  is added in RLC data buffer*/
               if(tempArrCount >= arrayIndex)
               {
                   /*Function call to fill TB2 and new entry in Rlc data buffer when schedule byte data is zero*/
                    if(availableTBSize[tbIndex] > MAC_MIN_SDU_SIZE)
                    {
                        retVal  = macMultiplexFillTBForNewEntry(transmissionAndRLCData_p,
                                                                dlQosStrategyLCNode_p,
                                                                availableTBSize,
                                                                &arrayIndex,
                                                                tbIndex);
                        if( retVal== MAC_FAILURE )
                          break;
                    }
               }
            }
        }
        
        /* SES-528 Changes Start */ 
        /* The below check has been added to tackle the situation in case if lc count for cumulative of both TBs exceed maximum LC count */
        if ( (transmissionAndRLCData_p->lcCountTBOne + transmissionAndRLCData_p->lcCountTBTwo) >= (MAX_NUMBER_OF_LOGICAL_CHANNEL - 1))
        {
            break;
        }
        /* SES-528 Changes End */ 
        /*Get the Next node from Schedule LC List*/
        dlQosStrategyLCNodevoid_p = (void *)
                    getNextListNode(&dlQosStrategyLCNode_p->scheduledLcAnchor);
        if(PNULL == dlQosStrategyLCNodevoid_p )
        {
           break;
        } 
        DL_QOS_GET_BASE_ADDRESS_FROM_SCHEDULED_LC_ANCHOR(
                                            dlQosStrategyLCNodevoid_p,
                                                        dlQosStrategyLCNode_p);
        loopCount--;
    }
    /*update padding byte for TB1*/
    if (availableTBSize[1] >= MAC_MIN_SDU_SIZE )
    {
       transmissionAndRLCData_p->paddingBytesForTBOne = availableTBSize[1];
    }
    else
    {
       transmissionAndRLCData_p->paddingBytesForTBOne = 0;
    }
    /*update padding byte for TB2*/
    if(availableTBSize[0] >= MAC_MIN_SDU_SIZE )
    {
       transmissionAndRLCData_p->paddingBytesForTBTwo = availableTBSize[0];
    }
    else
    {
       transmissionAndRLCData_p->paddingBytesForTBTwo = 0;
    }
}
/* PDCCH_PDSCH_WITHOUT_MUX Start */
/*****************************************************************************
 * Function Name  :  lteLayer2MacLogicalChannelsMuxing
 * Inputs         :  muxQueueNode_p - A pointer to the structure Multiplexer
 *                                    QueueNode which stores the information like
 *                                    ueIndex, HarqID etc,
 *                   scheduledTick - This information is simply passed by this
 *                                   function and is used by the encoder functionality.
 *                   ueContext_p - The Downlink UE Context Pointer.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This is function  wrapper called for multiplexing.
 *                   Based on scheduling type , corresponding muxing 
 *                   function will be called
 ******************************************************************************/
MacRetType lteLayer2MacLogicalChannelsMuxing(
        MultiplexerQueueNode  * muxQueueNode_p,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        DLUEContext* ueContext_p,
        InternalCellIndex internalCellIndex)
{
    MacRetType retType = MAC_SUCCESS;
#ifdef FDD_CONFIG
    UInt32 harqSubFrameNum = 0;
    /* SPR 15909 fix start */
    tickType_t scheduledExpiryTick = 0;
    /* SPR 15909 fix end */
    /*SPR 21948 Changes Start */
    /* Changes under spr 21762 have been reverted */
    /*SPR 21948 Changes End */
#endif
    SchType schUsed = DRA_SCHEDULING;

    if( DL_QOS_BASED_SCHEDULING == 
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            initParams_p->dlSchdConfig.dlSchedulerStrategy)
    {
        retType = multiplexScheduledLcinQoS(muxQueueNode_p,
                scheduledTick,
                ueContext_p,
                internalCellIndex);
        /* SPR 13609 Fix Start */
        if(MAC_FAILURE == retType )
        {
            /* SPR 21540 + */
#ifdef FDD_CONFIG
            /*SPR 22044 Change Start*/
            UInt8 dciContainerIndex = scheduledTick % MAX_CCE_CONTAINER;
            /*SPR 22044 Change End*/
            (pdcchULOutputInfo_gp[internalCellIndex] + dciContainerIndex)->directIndexingUEInfo[
                ueContext_p->ueIndex].dlCCEIndex = INVALID_CCE_INDEX;
#endif
            /* SPR 21540 - */

            if(!IS_PCELL_OF_UE(ueContext_p,muxQueueNode_p->internalCellIndex))
            {
                ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.schedulingStatus
                    = NO_TB;
                /** SPR 14277 Changes Start **/
#ifdef TDD_CONFIG
                ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.
                    sCellSchInfo[ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->pucchReqInfo.sCellSubFrame] = NO_TB;
#endif
                /** SPR 14277 Changes End **/
            }
        }
        /* SPR 13609 Fix End */
    }
    else
    {
        retType = multiplexLogicalChannelsForUE(muxQueueNode_p, 
                scheduledTick,
                ueContext_p,
                internalCellIndex);
    }

    if(retType == MAC_SUCCESS && INVALID_HARQ_ID !=  
            muxQueueNode_p->harqProcessId)
    {
        if (SPS_ACTIVATION <= muxQueueNode_p->msgType && 
                SPS_RETX >= muxQueueNode_p->msgType)
        {
            schUsed = SPS_SCHEDULING;
        }
        /* + SPS_TDD */
#ifdef FDD_CONFIG
        harqSubFrameNum = (scheduledTick + FDD_HARQ_OFFSET )%MAX_SUB_FRAME;
        scheduledExpiryTick = scheduledTick  + 8;
        /* - SPS_TDD */
        dlHarqTimerStart(ueContext_p->ueIndex,
                muxQueueNode_p->harqProcessId,harqSubFrameNum,
                /* + SPR 11208 Changes */
                scheduledExpiryTick,schUsed,muxQueueNode_p->internalCellIndex); 
        /* - SPR 11208 Changes */
#elif TDD_CONFIG       
        tddDlHarqTimerStart(ueContext_p->ueIndex,
                muxQueueNode_p->harqProcessId,
                muxQueueNode_p->dlSubframeNum,
                scheduledTick-muxQueueNode_p->phyDelay,
                muxQueueNode_p->sysFrame, schUsed,
                muxQueueNode_p->internalCellIndex
                ); 
#endif
    }

    if( DL_QOS_BASED_SCHEDULING ==
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
            initParams_p->dlSchdConfig.dlSchedulerStrategy)
    {
        return MAC_SUCCESS;
    }
    else
    {
        return retType;
    }
}

/* + SPR 13605 Fix */
/*****************************************************************************
 * Function Name  : dlQoSResetCCEsAndHarqProcess 
 * Inputs         : ueContext_p
                    transmissionAndRLCData_p
 * Outputs        : 
 * Returns        : None
 * Description    : Reset CCEs and free harq process when mux fail 
                    for any cell.
 ******************************************************************************/
STATIC  void dlQoSResetCCEsAndHarqProcess( DLUEContext *ueContext_p,
                                                 TransmissionAndRLCData * transmissionAndRLCData_p)
{

    UInt8 dciContainerIndex = 0;
    DciCCEInfo* dciCCEInfo_p = PNULL;
    DLHARQProcess* dlHarqProcess_p = PNULL;
    InternalCellIndex internalCellIndex = transmissionAndRLCData_p->encoderCellIndex;

    /* SPR 23616 fix starts */
    /*Check has been added to avoid certain actions for eMTC UE*/
        /* SPR 23616 fix ends */
    dciContainerIndex = transmissionAndRLCData_p->scheduledTick % MAX_CCE_CONTAINER;
    dciCCEInfo_p = &((dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex)->
            dciCCEInfo[transmissionAndRLCData_p->cceIndex]);

    mac_reset_cce( dciCCEInfo_p->dciPduInfo.aggregationLvl,
                   dciCCEInfo_p,
                   internalCellIndex);

    mac_reset_extra_cce( (dciCCEContainerInfo_gp[internalCellIndex] + dciContainerIndex),
                         dciCCEInfo_p,
                         ueContext_p,
                         internalCellIndex);
        /* SPR 23616 fix starts */
    /*HARQ Free shall be done for both eMTC and LTE UEs*/
    /* SPR 23616 fix ends */

    if(IS_PCELL_OF_UE(ueContext_p,internalCellIndex))
    {
        dlHarqProcess_p =
            &ueContext_p->dlHarqContext_p->harqProcess[transmissionAndRLCData_p->harqProcessId];
    }
    else
    {
        dlHarqProcess_p = &(ueContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p->
                harqProcess[transmissionAndRLCData_p->harqProcessId]);
    }

    freeDLHarqProcess(ueContext_p,
            dlHarqProcess_p,internalCellIndex);
}
/* - SPR 13605 Fix */
