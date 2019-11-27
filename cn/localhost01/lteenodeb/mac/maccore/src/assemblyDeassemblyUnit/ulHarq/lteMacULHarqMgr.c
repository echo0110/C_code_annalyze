/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacULHarqMgr.c,v 1.2.6.1.4.2.2.1 2010/09/21 15:46:03 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This	file contains the function related to ul harq 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacULHarqMgr.h"
#include "lteMacCellConfMgr.h"
#include "lteMacExecutionLegs.h"
/*Start Changes TDD DL UL SPLIT*/
#include "lteMacPHICH.h"
/*End Changes TDD DL UL SPLIT*/
#ifdef ULHARQ_TIMER_PROC
#include "lteMacPRACH.h"
#ifdef FDD_CONFIG
#include "lteMacULSpsScheduler.h"
#endif
/*CA Changes start  */
ULHarqTimerEntityList ulHarqTimerEntityList_g[MAX_NUM_CELL][MAX_UL_HARQ_TIMER][MAX_SUB_FRAME];
/*CA Changes end  */
#endif
/* msg3_retx_tdd_support_start */
ULHarqTimerEntityForMsg3List ulHarqTimerEntityForMsg3List_g[MAX_NUM_CELL][MAX_UL_HARQ_TIMER][MAX_SUB_FRAME];
/* msg3_retx_tdd_support_end */


#include "lteMacECIDMgr.h"
/******************************************************************************
 Private Definitions
 *****************************************************************************/



/******************************************************************************
 Private Types
 *****************************************************************************/

 /*****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/
/*CA Changes start */
 void initULHarqTimerArrayForMsg3(UInt8 numOfCells);
/*CA Changes end */
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/* SPR 1660 + SPR 1668 Fix starts */

    /* Index to the nextIrvSequenceArray_g is the currentIrv
     * Next Irv of 0 =>2
     * Next Irv of 1 =>0
     * Next Irv of 2 =>3
     * Next Irv of 3 =>1
     */
    UInt8 nextIrvSequenceArray_g[] = {2,0,3,1};

/* SPR 1660 + SPR 1668 Fix ends */

#ifdef DL_UL_SPLIT
extern MacRetType putEntryInULContainerNackQueueUsingICC( SInt32 ulContainerNackQIdUL , ContainerNackQueueNode *nackNode_p);

#endif

#ifdef CIRC_QUEUE_IMPL
/*CA Changes start  */
extern sem_t waitForULNackQueue_g[MAX_NUM_CELL];
extern sem_t waitForULNackForTempUE_g[MAX_NUM_CELL];
/*CA Changes end  */
#endif
/* SPR 16916 FIX START */
#ifdef KPI_STATS
void  updateNumOfActiveUePerQciUl(UInt16 ueIndex,InternalCellIndex internalCellIndex);
#endif
/* SPR 16916 FIX END */


/* TTIB_Code Start */

#ifdef FDD_CONFIG
/********************************************************************************
* Function Name     : updateTTIHarqMap
* Inputs            : harqID,
*                     tempULHarqInfo_p,
*                     ttiHarqMap
* Outputs           : updated tiHarqMap in ulUeContext \
* Returns           : None
* Description       : This function will update ttiHarqMap in ulUEContext based on inputs.
*********************************************************************************/

 void updateTTIHarqMap(UInt8 harqID,
                             ULHarqInfo* tempULHarqInfo_p,
                             UInt8 *ttiHarqMap)

{
     UInt8 counter = 0;
/*SPR 21769 Changes Start*/
     tickType_t harqMapId = 0;
/*SPR 21769 Changes End*/
     UInt8 harqStatusFlag = 0;

     harqStatusFlag  = (HARQ_PROCESS_FREE == tempULHarqInfo_p->harqStatus)?(TTI_MAP_FREE):(harqID);

     if(TRUE == tempULHarqInfo_p->isTTIBundled)
     {
       harqMapId = tempULHarqInfo_p->ttiBundleFirstTick;

       for(counter = 0 ; counter < TTI_BUNDLE_SIZE ; counter++)
       {
           ttiHarqMap[MODULO_SIXTEEN(harqMapId+counter)] = harqStatusFlag;
       }
//CODE_COMMENT_L19_FIXED
       if(TTI_MAP_FREE == harqStatusFlag)
       {
           tempULHarqInfo_p->isTTIBundled = 0;
           tempULHarqInfo_p->isTTIDataRcvd = 0;
           tempULHarqInfo_p->ttiBundleFirstTick = 0;
       }
     }
     else
     {
        /** harqID cant be more than 7 **/
         ttiHarqMap[harqID + MAC_DEFAULT_NUM_HARQ_PROCESSES] = ttiHarqMap[harqID] = harqStatusFlag;
     }

}

#endif

/* TTIB_Code End */

/********************************************************************************
* Function Name     : freeULHarqProcess
* Inputs            : tempULHarqInfo - The pointer to the concerned UL HARQ Process
*                                      for which ack has been received and now has to freed.
* Outputs           : None
* Returns           : None
* Description:        This function will release the UL HARQ Process When the data
*                     is successfully received.
*********************************************************************************/
/* SPR 16916 FIX START */
#ifdef KPI_STATS
void freeULHarqProcess(ULHarqInfo* tempULHarqInfo, UInt16 ueIndex,
        InternalCellIndex internalCellIndex)
#else
void freeULHarqProcess(ULHarqInfo* tempULHarqInfo)
#endif
{
/* SPR 16916 FIX END */

    LTE_MAC_UT_LOG(LOG_INFO, LOG_UL_HARQ, "[%s] Entry......\n",__func__);
    /* SPR 14235 Changes Starts */
    UInt8 lcgIndex = 0; 
    /* SPR 14235 Changes Ends */
    tempULHarqInfo->riStart = 0;
    tempULHarqInfo->riLength = 0;
    tempULHarqInfo->riv = 0;
    tempULHarqInfo->currentRetransCount = 0;
/* SPR 1660 + SPR 1668 Fix starts */	
	tempULHarqInfo->currentIrv = 0;
/* SPR 1660 + SPR 1668 Fix ends */	
    tempULHarqInfo->deltabsrLoad =  0;
    tempULHarqInfo->newDataIndicator = 0;
    tempULHarqInfo->sfUsed = 0;
    tempULHarqInfo->nPhichSeq = 0;
    tempULHarqInfo->nPhichGrp = 0;
    tempULHarqInfo->isCqiRequested = 0;
    tempULHarqInfo->nextReTransmissionType = ADAPTIVE_RE_TX;
    /* + SPR_17858_587_CHANGES */
    tempULHarqInfo->harqTimerExpirySF = MAX_SUB_FRAME;
    tempULHarqInfo->harqTimerExpirySystemFrame = MAX_SYS_FRAME_NUM;
    /* - SPR_17858_587_CHANGES */

    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    tempULHarqInfo->hdFddUlNackHandlingFlag = FALSE;
#endif
/* TTIB_Code Start */
#ifdef FDD_CONFIG
    tempULHarqInfo->isSpsEnabled = FALSE;
#endif
/* TTIB_Code End */
    /* SPR 14235 Changes Starts */
     for(lcgIndex = 0; lcgIndex < NUM_OF_LCG; lcgIndex++)
     {
       tempULHarqInfo->grantPerLcg[lcgIndex] = 0;
     }
    /* SPR 14235 Changes Ends */
    /*HD FDD Changes End*/

    /* SPR 16916 FIX START */
#ifdef KPI_STATS
    updateNumOfActiveUePerQciUl(ueIndex,internalCellIndex);
#endif
    /* SPR 16916 FIX END */
    /* Moved line as part of DL/UL Split */
    /* Free harq process after resetting all parameters*/
/* SPR 19288 change start */
/* SPR 19288 change end */
    tempULHarqInfo->harqStatus = HARQ_PROCESS_FREE;
    /*SPR 7086 Fix Start*/
    /*Reset the nSrsInitial in HARQ Context*/
    tempULHarqInfo->nSrsInitial = 0;
    /*SPR 7086 Fix End*/
    /* SPR 10027 FIX START */
    tempULHarqInfo->isDrbPresent = FALSE;
    /* SPR 10027 FIX END */
    /* SPR 10753 Changes */
    tempULHarqInfo->forceAckNextRetxType = NON_ADAPTIVE_RETRANS;
    /* SPR 10753 Changes */
    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    tempULHarqInfo->RAT1Allocation = 0;
    tempULHarqInfo->startRbgIndexforSet1 = 0;
    tempULHarqInfo->startRbgIndexforSet2 = 0;
    tempULHarqInfo->endRbgIndexforSet1 = 0;
    tempULHarqInfo->endRbgIndexforSet2 = 0;
#endif
    /* SPR 21958 PUSCH RAT1 Support End */
    LTE_MAC_UT_LOG(LOG_INFO, LOG_UL_HARQ, "[%s] Exit......\n",__func__);
}

/********************************************************************************
* Function Name     : updateULHarqProcess
* Inputs            : tempULHarqInfo - The pointer to the concerned UL HARQ Process
*                                       which is to be updated,
*                     resourceReserved - A pointer to the AllocatedResInfo which
*                                        contains the information about allocated 
*                                        length, index, riv
*                     updateFlag
* Outputs           : None
* Returns           : None
* Description       : This function will update the UL HARQ Process when the re-
*                     transmission is going to take place.
*********************************************************************************/

 void updateULHarqProcess(ULHarqInfo*   tempULHarqInfo,
                         AllocatedResInfo* resourceReserved,
                         UInt8 updateFlag)
{
    LTE_MAC_UT_LOG(LOG_INFO, LOG_UL_HARQ, "[%s] Entry......\n",__func__);
    if (updateFlag)
    {  
        /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
        if(!resourceReserved->RAT1Allocation)
        {
            tempULHarqInfo->RAT1Allocation = resourceReserved->RAT1Allocation;
#endif
            tempULHarqInfo->riv = resourceReserved->allocatedRiv;
#ifdef FDD_CONFIG
        }
        else
        {
            tempULHarqInfo->startRbgIndexforSet1 = resourceReserved->startRbgIndexforSet1;
            tempULHarqInfo->startRbgIndexforSet2 = resourceReserved->startRbgIndexforSet2;
            tempULHarqInfo->endRbgIndexforSet1 = resourceReserved->endRbgIndexforSet1;
            tempULHarqInfo->endRbgIndexforSet2 = resourceReserved->endRbgIndexforSet2;
            tempULHarqInfo->RAT1Allocation = resourceReserved->RAT1Allocation;
        }
#endif
        /* SPR 21958 PUSCH RAT1 Support End */
        tempULHarqInfo->riStart = resourceReserved->allocatedIndex;
        tempULHarqInfo->riLength = resourceReserved->allocatedLength;
        /* SPR 21958 PUSCH RAT1 Support Start */
        /* Code moved above */
        /* SPR 21958 PUSCH RAT1 Support End */
    }
    tempULHarqInfo->currentRetransCount++;
/* SPR 1660 + SPR 1668 Fix starts */	
	tempULHarqInfo->currentIrv = nextIrvSequenceArray_g[tempULHarqInfo->currentIrv];
/* SPR 1660 + SPR 1668 Fix ends */

    LTE_MAC_UT_LOG(LOG_INFO, LOG_UL_HARQ,
       "[%s] tempULHarqInfo->currentRetransCount = %d \n"
       ,__func__,tempULHarqInfo->currentRetransCount);
 
#ifdef ULHARQ_TIMER_PROC                
    LOG_MAC_MSG(MAC_UPDATE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                getCurrentTick(),
                tempULHarqInfo->ulHarqProcessId,
                tempULHarqInfo->currentRetransCount,
                tempULHarqInfo->nextReTransmissionType,
                tempULHarqInfo->riStart, \
                tempULHarqInfo->riLength,\
                (UDouble32)tempULHarqInfo->riv,\
                0.0,__func__,"ULHARQ_UPDATE");
#else
     LOG_MAC_MSG(MAC_UPDATE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                getCurrentTick(),
                0,
                tempULHarqInfo->currentRetransCount,
                tempULHarqInfo->nextReTransmissionType,
                tempULHarqInfo->riStart, \
                tempULHarqInfo->riLength,\
                (UDouble32)tempULHarqInfo->riv,\
                0.0,__func__,"ULHARQ_UPDATE");
   
#endif                
   
    LTE_MAC_UT_LOG(LOG_INFO, LOG_UL_HARQ, "[%s] Exit......\n",__func__);
}



/********************************************************************************
* Function Name     : allocateULHarqProcess
* Inputs            : tempULHarqInfo - The pointer to the concerned UL HARQ Process.
*                     resourceReserved - A pointer to the AllocatedResInfo which
*                                        contains the information about allocated 
*                                        length, index, riv
*                     ulSfTick - used or only TDD ulSF TTI
* Outputs           : None  
* Returns           : None
* Description       : This function will create a new UL HARQ Process for any new
*                     scheduled UE.
*********************************************************************************/

 void allocateULHarqProcess(ULHarqInfo*   tempULHarqInfo,
                           AllocatedResInfo* resourceReserved
                           /* +- SPR 17777 */
                           /* SPR 11331 Fix Start */
#ifdef TDD_CONFIG
                            /* SPR 15909 fix start */
                           ,tickType_t ulSfTick
                            /* SPR 15909 fix end */
#endif
                           /* SPR 11331 Fix End */
                           )
{
    LTE_MAC_UT_LOG(LOG_INFO, LOG_UL_HARQ, "[%s] Entry......\n",__func__);

    tempULHarqInfo->riStart = resourceReserved->allocatedIndex;
    tempULHarqInfo->riLength = resourceReserved->allocatedLength;
#if defined(KPI_STATS) || defined(PERF_STATS)
    /* SPR 11331 Fix Start */
#ifdef TDD_CONFIG
    UInt8 rbForKpiStatsIdx = ulSfTick % MAX_UL_SF_CONTAINER;
#endif
    /* SPR 11331 Fix End */
#if !defined (OVERSHOOT_CORRECTION) && !defined(DL_UL_SPLIT)

    /* SPR 11331 Fix Start */
#ifdef FDD_CONFIG
    tempULHarqInfo->rbForKpiStats = resourceReserved->allocatedLength;
#else
    tempULHarqInfo->rbForKpiStats[rbForKpiStatsIdx] = resourceReserved->allocatedLength;
#endif
    /* SPR 11331 Fix End */

#else
    /* SPR 11331 Fix Start */
#ifdef FDD_CONFIG
    tempULHarqInfo->rbForKpiStats += resourceReserved->allocatedLength;
#else
    tempULHarqInfo->rbForKpiStats[rbForKpiStatsIdx] += resourceReserved->allocatedLength;
#endif
    /* SPR 11331 Fix End */
    tempULHarqInfo->tbSizeNet = 0;
#endif
#endif 
    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    if(TRUE == resourceReserved->RAT1Allocation)
    {
        tempULHarqInfo->startRbgIndexforSet1 = resourceReserved->startRbgIndexforSet1;
        tempULHarqInfo->endRbgIndexforSet1 = resourceReserved->endRbgIndexforSet1;
        tempULHarqInfo->startRbgIndexforSet2= resourceReserved->startRbgIndexforSet2;
        tempULHarqInfo->endRbgIndexforSet2 = resourceReserved->endRbgIndexforSet2;
        tempULHarqInfo->RAT1Allocation = TRUE;
    }
    else
    {
        tempULHarqInfo->RAT1Allocation = FALSE;
#endif
        tempULHarqInfo->riv = resourceReserved->allocatedRiv;
#ifdef FDD_CONFIG
    }
#endif
    /* RIV is now updated in the above check */
    /* SPR 21958 PUSCH RAT1 Support End */
    tempULHarqInfo->currentRetransCount = 0;
    /* SPR 1660 + SPR 1668 Fix starts */	
    tempULHarqInfo->currentIrv = 0;
    /* SPR 1660 + SPR 1668 Fix ends */	
    /*BSR Changes*/
    /* tempULHarqInfo->deltabsrLoad =  tbSize;*/
    tempULHarqInfo->newDataIndicator = 1;
    tempULHarqInfo->nextReTransmissionType = ADAPTIVE_RE_TX;
    tempULHarqInfo->strictRBAllocation = TRUE;
    tempULHarqInfo->harqStatus = HARQ_PROCESS_IN_USE;
    tempULHarqInfo->isCqiRequested = 0;
/* SPR 19288 change start */
/* SPR 19288 change end */
    /* SPR 10753 Changes */
    tempULHarqInfo->forceAckNextRetxType = NON_ADAPTIVE_RETRANS;
    /* SPR 10753 Changes */
    /*PHICH_SPR Fix 12758 start :Initialize */
    tempULHarqInfo->forcedAckFlag = FORCED_ACK_RESET;
    /*SPR Fix 12758 end */
    LTE_MAC_UT_LOG(LOG_INFO, LOG_UL_HARQ, \
            "[%s] tempULHarqInfo->riStart = %d\n"
            "tempULHarqInfo->riLength = %d\n"
            "tempULHarqInfo->riv = %d\n"
            ,__func__,tempULHarqInfo->riStart,tempULHarqInfo->riLength,tempULHarqInfo->riv);

#ifdef ULHARQ_TIMER_PROC                
    LOG_UT(MAC_ALLOCATE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
            getCurrentTick(),
            tempULHarqInfo->ulHarqProcessId,
            tempULHarqInfo->deltabsrLoad,\
            tempULHarqInfo->riStart, \
            tempULHarqInfo->riLength,0,\
            (UDouble32)tempULHarqInfo->riv,\
            0.0,__func__,"ALLOC_ULHARQ");

#else
    LOG_UT(MAC_ALLOCATE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
            getCurrentTick(),
            0,
            tempULHarqInfo->deltabsrLoad,\
            tempULHarqInfo->riStart, \
            tempULHarqInfo->riLength,0,\
            (UDouble32)tempULHarqInfo->riv,\
            0.0,__func__,"ALLOC_ULHARQ");

#endif                
    LTE_MAC_UT_LOG(LOG_INFO, LOG_UL_HARQ, "[%s] Exit......\n",__func__);
}
#ifdef ULHARQ_TIMER_PROC
/*****************************************************************************
 * Function Name  :   ulHarqTimerStart
 * Inputs         :   ueIndex - The UE index of the UE for which a HARQ Timer  
 *                              has to be started. 
 *                    harqProcessId - The the harq process ID for which the timer 
 *                                    has to  be started.
 *                    frameNumber - The sub frame number for which the UE data
 *                                  will be on the PHYSICAL layer.
 *                    timerExpiredTTI,
 *                    expirySysFrameNumber
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs        :   None
 * Returns        :   MAC_FAILURE/MAC_SUCCESS
 * Description    :   This function will start the HARQ Timer functionality.
 *****************************************************************************/


 MacRetType ulHarqTimerStart( 
                             UInt16  ueIndex,
                             UInt8   harqProcessId,
                             UInt8  frameNumber,
                             /* + SPS_TDD_Changes */
                             UInt8 schUsed,
                             /* - SPS_TDD_Changes */
                             /* SPR 15909 fix start */
                             tickType_t  timerExpiredTTI,
                             /* SPR 15909 fix end */
                             UInt32  expirySysFrameNumber,
                             /*CA Changes start  */
                             InternalCellIndex internalCellIndex)
                             /*CA Changes end  */
{
    UInt8 ulHarqEntityTimerArrIndex = 0;
    ULHarqTimerEntityList* ulHarqEntityTimerList_p = PNULL;
    ULHarqTimerNode* ulharqTimerNode_p = PNULL;
    ULUEContextInfo* ulUEContextInfo_p = PNULL;
    ULUEContext*  ueContext_p = PNULL;       
    ULHarqInfo* ulHarqProcess_p = PNULL;
    UInt32  ulHarqTimerSysFrameIndex = 0;
    
    
    LTE_MAC_UT_LOG(LOG_INFO,LOG_UL_HARQ,"[%s] Entry CTTI %d\n",__func__,getCurrentTick());
    
    ulUEContextInfo_p = &ulUECtxInfoArr_g[ueIndex];

    ueContext_p = ulUEContextInfo_p->ulUEContext_p;
    
 	if(harqProcessId >= MAX_HARQ_PROCESS_NB)
	{
		LTE_MAC_UT_LOG(LOG_WARNING,LOG_UL_HARQ,"[%s] MAC_FAILURE"
        "INVALID harqProcessId %d CTTI %d\n",__func__,harqProcessId,getCurrentTick());

        LOG_MAC_MSG(MAC_INVALID_HID_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                0,ueIndex,\
                frameNumber,0,0,0,\
                0.0,0.0,__func__,"INVALID_HID");
        return MAC_FAILURE;
	}

    ulHarqTimerSysFrameIndex = MODULO_TWO(expirySysFrameNumber);
    /*CA Changes start  */
    ulHarqEntityTimerList_p = &ulHarqTimerEntityList_g[internalCellIndex][ulHarqTimerSysFrameIndex][frameNumber];
    /*CA Changes end  */
    /*
        NOTE:- The value of ulHarqEntityTimerList_p->count was initialised to 0
        (invalid value) and hence we have to increase it after it is used as 
        array index, on the first occasion on the other occasion it just gives
        the proper array index.
    */ 

    ulHarqEntityTimerArrIndex = ulHarqEntityTimerList_p->count;

    LTE_MAC_UT_LOG(LOG_INFO,LOG_UL_HARQ,"[%s] ulHarqEntityTimerList_p->count %d"\
    "CTTI %d\n",__func__,ulHarqEntityTimerList_p->count,getCurrentTick());

    ulharqTimerNode_p = \
       &(ulHarqEntityTimerList_p->ulHarqTimerNodeArr[ulHarqEntityTimerArrIndex]);

    ulHarqProcess_p = \
                      &(ueContext_p->ulresInfo[harqProcessId]);

    if(( 0 != ulHarqProcess_p->riLength) && (HARQ_TIMER_NODE_INVALID == ulharqTimerNode_p->isValid))
    {
        ulharqTimerNode_p->ueIndex = ueIndex;
        ulharqTimerNode_p->harqProcessId = harqProcessId;
        ulharqTimerNode_p->isValid = HARQ_TIMER_NODE_VALID;
        ulharqTimerNode_p->ulHarqExpiredSubframe = frameNumber;
        ulharqTimerNode_p->ulHarqExpiredSysframe = expirySysFrameNumber;
        ulharqTimerNode_p->ulHarqExpiredTTI = timerExpiredTTI;
        /* + SPS_TDD_Changes */
        ulharqTimerNode_p->schUsed = schUsed ;
        /* - SPS_TDD_Changes */
        ueContext_p->ulHarqTimerNodeArrrayIndex[ulHarqTimerSysFrameIndex][frameNumber] = \
                                                               ulHarqEntityTimerArrIndex;
        ulHarqEntityTimerList_p->count++;
        /* + SPR_17858_587_CHANGES */
        ulHarqProcess_p->harqTimerExpirySF = frameNumber;
        ulHarqProcess_p->harqTimerExpirySystemFrame = expirySysFrameNumber;
        /* - SPR_17858_587_CHANGES */


        LTE_MAC_UT_LOG(LOG_INFO,LOG_UL_HARQ,
                "[%s] The ulharqTimer is started for UE ID %d"\
                " Harq ID %d current Tick %d expiry frameNumber %d"\
                " ulHarqEntityTimerArrIndex %d"\
                " ulharqTimerNode_p->isValid%d\n",
                __func__,ueIndex, harqProcessId,getCurrentTick(),frameNumber,
                ulHarqEntityTimerArrIndex,ulharqTimerNode_p->isValid );
        
        LOG_UT(MAC_TIMER_START_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                0,ueIndex,harqProcessId, \
                frameNumber,\
                timerExpiredTTI,\
                ulHarqEntityTimerList_p->count,\
                0.0,0.0,__func__,"TIMER_START");
    }
    else if(HARQ_PROCESS_FREE == ulHarqProcess_p->riLength )
    {
        LTE_MAC_UT_LOG(LOG_INFO,LOG_UL_HARQ,
                "[%s] The ulharqTimer is  not started for UE ID %d"\
                " Harq ID %d current Tick %d" 
                " because the corresponding HARQ Process is free"\
                " ulHarqProcess_p->riLength %d\n",
                __func__,ueIndex,harqProcessId,getCurrentTick(),
                ulHarqProcess_p->riLength);
        
        LOG_MAC_MSG(MAC_HARQ_PROC_FREE_ULHARQ_ID,LOGERROR,MAC_UL_HARQ,\
                0,ueIndex,harqProcessId,\
                frameNumber,0,0,\
                0.0,0.0,__func__,"HID_FREE");

    }
    else if(HARQ_TIMER_NODE_VALID == ulharqTimerNode_p->isValid)
    {
         LTE_MAC_UT_LOG(LOG_INFO,LOG_UL_HARQ,
                "[%s] The ulharqTimer is  not started for UE ID %d"\
                " Harq ID %d current Tick %d"\
                "ulHarqEntityTimerArrIndex %d"\
                " because the ULHARQ Timer Node is busy"\
                " ulharqTimerNode_p->isValid %d\n",
                __func__,ueIndex,harqProcessId,getCurrentTick(),
                ulHarqEntityTimerArrIndex,ulharqTimerNode_p->isValid);
         
         LOG_MAC_MSG(MAC_TIMER_NODE_BUSY_ULHARQ_ID,LOGERROR,MAC_UL_HARQ,\
                 0,ueIndex,harqProcessId, \
                 frameNumber,0,0, 
                 0.0,0.0,__func__,"NODE_BUSY");
    }
    LTE_MAC_UT_LOG(LOG_INFO,LOG_UL_HARQ,"[%s] Exit CTTI %d\n",__func__,getCurrentTick());
    return MAC_SUCCESS;
 
}
/* msg3_retx_tdd_support_start */
#endif
/* msg3_retx_tdd_support_end */
/*SPR_3305_FIX_START*/
/*****************************************************************************
 * Function Name  :   ulHarqTimerStartForMsg3
 * Inputs             tcRnti - temporary context  
 *                    harqProcessId - The the harq process ID for which the timer 
 *                                    has to  be started.
 *                    frameNumber - The sub frame number for which the UE data
 *                                  will be on the PHYSICAL layer.
 *                    timerExpiredTTI,
 *                    expirySysFrameNumber
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs        :   None
 * Returns        :   MAC_FAILURE/MAC_SUCCESS
 * Description    :   This function will start the HARQ Timer functionality.
 *****************************************************************************/


 MacRetType ulHarqTimerStartForMsg3(UInt16  tcRnti,
                                          UInt8   harqProcessId,
                                          UInt8   frameNumber,
                                          /* SPR 15909 fix start */
                                          tickType_t  timerExpiredTTI,
                                          /* SPR 15909 fix end */
                                          UInt32  expirySysFrameNumber,
                                          /*CA Changes start  */
                                          InternalCellIndex internalCellIndex)
                                          /*CA Changes end  */
{
    UInt8 ulHarqEntityTimerArrIndex = 0;
    ULHarqTimerEntityForMsg3List* ulHarqEntityForMsg3TimerList_p = PNULL;
    ULHarqTimerForMsg3Node* ulharqTimerForMsg3Node_p = PNULL;
    ULHarqInfo* ulHarqProcess_p = PNULL;
    TempCRNTICtx *tempCRNTICtx_p = PNULL;
    RNTIInfo      *rntiInfo_p        = PNULL;
    UInt32  ulHarqTimerSysFrameIndex = 0;
    
 	if(harqProcessId >= MAX_HARQ_PROCESS_NB)
	{
        LOG_MAC_MSG(MAC_INVALID_HID_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                0,tcRnti,\
                frameNumber,0,0,0,\
                0.0,0.0,__func__,"INVALID_HID For MSG3");
        return MAC_FAILURE;
	}
    rntiInfo_p = getUeIdxFromRNTIMap(tcRnti, internalCellIndex);

    if(PNULL == rntiInfo_p)
    {
      return MAC_FAILURE;
    }
    /*++klockwork warning */
    tempCRNTICtx_p = getTempCRNTIContextPointer(tcRnti, internalCellIndex);
    if(PNULL == tempCRNTICtx_p)
    {
        return MAC_FAILURE;
    }
    /*--klockwork warning */
    ulHarqTimerSysFrameIndex = MODULO_TWO(expirySysFrameNumber);

    /*CA Changes start  */
    ulHarqEntityForMsg3TimerList_p = 
                          &ulHarqTimerEntityForMsg3List_g[internalCellIndex][ulHarqTimerSysFrameIndex][frameNumber];
    /*CA Changes end  */
    /*
        NOTE:- The value of ulHarqEntityTimerList_p->count was initialised to 0
        (invalid value) and hence we have to increase it after it is used as 
        array index, on the first occasion on the other occasion it just gives
        the proper array index.
    */ 

    ulHarqEntityTimerArrIndex = ulHarqEntityForMsg3TimerList_p->count;

    ulharqTimerForMsg3Node_p = \
       &(ulHarqEntityForMsg3TimerList_p->ulHarqForMsg3TimerNodeArr[ulHarqEntityTimerArrIndex]);

    ulHarqProcess_p = &(tempCRNTICtx_p->ulHarq);
    if(( 0 != ulHarqProcess_p->riLength) && (HARQ_TIMER_NODE_INVALID ==\
                                           ulharqTimerForMsg3Node_p->isValid))
    {
        ulharqTimerForMsg3Node_p->harqProcessId = harqProcessId;
        ulharqTimerForMsg3Node_p->isValid = HARQ_TIMER_NODE_VALID;
        ulharqTimerForMsg3Node_p->ulHarqExpiredSubframe = frameNumber;
        ulharqTimerForMsg3Node_p->ulHarqExpiredSysframe = expirySysFrameNumber;
        ulharqTimerForMsg3Node_p->ulHarqExpiredTTI = timerExpiredTTI;
        ulharqTimerForMsg3Node_p->ueIndex = rntiInfo_p->index; 
        tempCRNTICtx_p->ulHarqTimerNodeArrrayIndex = ulHarqEntityTimerArrIndex;
        ulHarqEntityForMsg3TimerList_p->count++;
        /* + SPR_17858_587_CHANGES */
        ulHarqProcess_p->harqTimerExpirySF = frameNumber;
        ulHarqProcess_p->harqTimerExpirySystemFrame = expirySysFrameNumber;
        /* - SPR_17858_587_CHANGES */

    }
    else if(HARQ_PROCESS_FREE == ulHarqProcess_p->riLength )
    {
        LOG_MAC_MSG(MAC_HARQ_PROC_FREE_ULHARQ_ID,LOGERROR,MAC_UL_HARQ,\
                0,tcRnti,harqProcessId,\
                frameNumber,0,0,\
                0.0,0.0,__func__,"HID_FREE For MSG3");
    }
    else if(HARQ_TIMER_NODE_VALID == ulharqTimerForMsg3Node_p->isValid)
    {
        LOG_MAC_MSG(MAC_TIMER_NODE_BUSY_ULHARQ_ID,LOGERROR,MAC_UL_HARQ,\
                0,tcRnti,harqProcessId, \
                frameNumber,0,0, 
                0.0,0.0,__func__,"NODE_BUSY For MSG3");
    }
    return MAC_SUCCESS;
 
}
/************************************************************************
 * Function Name  :   ulHarqTimerForMsg3Stop 
 * Inputs         :   tempCRNTICtx_p - pointer to TempCRNTICtx,
 *                    frameNumber - The sub frame number for which the UE data
 *                                 was sent on the PHYSICAL layer,
 *                    currentSysFrameNumber
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs        :   None
 * Returns        :   MAC_SUCESS/MAC_FAILURE.
 * Description    :   This functon will stop the HARQ Timer functionality and
 *                    update the relevent fields of the DL UE Context.
 ************************************************************************/


 MacRetType ulHarqTimerForMsg3Stop( TempCRNTICtx *tempCRNTICtx_p,
                                          UInt32 frameNumber,
                                          UInt32 currentSysFrameNumber,
                                          /*CA Changes start  */
                                          InternalCellIndex internalCellIndex)
                                          /*CA Changes end  */
{
    ULHarqTimerEntityForMsg3List* ulHarqEntityForMsg3TimerList_p = PNULL;
    ULHarqTimerForMsg3Node* ulharqTimerForMsg3Node_p = PNULL;
    UInt8 ulHarqEntityTimerArrIndex = -1;
    UInt32  ulHarqTimerSysFrameIndex = 0;

    ulHarqTimerSysFrameIndex = MODULO_TWO(currentSysFrameNumber);

    /*CA Changes start  */
    ulHarqEntityForMsg3TimerList_p = 
                          &ulHarqTimerEntityForMsg3List_g[internalCellIndex][ulHarqTimerSysFrameIndex][frameNumber];
    /*CA Changes end  */
    if (!(ulHarqEntityForMsg3TimerList_p->count))
    {
        /*
            The count is Zero symbolysing that there is no nodes avaliable in the 
            UL HARQ TIMER List Array.
        */
        LOG_MAC_MSG(MAC_TIMER_CNT_ZERO_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                0,frameNumber,0,0,0,0,\
                0.0,0.0,__func__,"CNT_ZERO For MSG3");
        return MAC_FAILURE;
    }
    ulHarqEntityTimerArrIndex = tempCRNTICtx_p->ulHarqTimerNodeArrrayIndex;
    if (ulHarqEntityTimerArrIndex == INVALID_COUNT)
    {
        LOG_MAC_MSG(MAC_TIMER_CNT_INVALID_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                0,tempCRNTICtx_p->tcRNTI,\
                frameNumber,0,0,0,\
                0.0,0.0,__func__,"CNT_INVALID For MSG3");
        return MAC_FAILURE;
    }
     ulharqTimerForMsg3Node_p = \
       &(ulHarqEntityForMsg3TimerList_p->ulHarqForMsg3TimerNodeArr[ulHarqEntityTimerArrIndex]);
   
    if (ulharqTimerForMsg3Node_p->isValid) 
    {
        LOG_UT(MAC_TIMER_STOP_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                0,\
                tempCRNTICtx_p->tcRNTI,\
                ulharqTimerForMsg3Node_p->harqProcessId,\
                frameNumber,\
                ulharqTimerForMsg3Node_p->ulHarqExpiredTTI,\
                ulHarqEntityForMsg3TimerList_p->count,\
                0.0,0.0,__func__,"TIMER_STOP For MGS3");

        ulharqTimerForMsg3Node_p->isValid = HARQ_TIMER_NODE_INVALID;
        /*
            We now fill the array ulHarqTimerNodeIndex with an invalid entry.
        */
        tempCRNTICtx_p->ulHarqTimerNodeArrrayIndex = INVALID_COUNT;

        ulharqTimerForMsg3Node_p->ulHarqExpiredSubframe = MAX_SUB_FRAME;
        
        ulharqTimerForMsg3Node_p->ulHarqExpiredSysframe = MAX_SFN_VALUE;
        
        ulharqTimerForMsg3Node_p->harqProcessId = MAX_HARQ_PROCESS_NB;
        /* + SPR_17858_587_CHANGES */
        tempCRNTICtx_p->ulHarq.harqTimerExpirySF = MAX_SUB_FRAME;
        tempCRNTICtx_p->ulHarq.harqTimerExpirySystemFrame = MAX_SYS_FRAME_NUM;
        /* - SPR_17858_587_CHANGES */

        /* SPR 19036 start */
        /* Decrement count which was increased while doing ul harq timer start*/
        ulHarqEntityForMsg3TimerList_p->count--;
        /* SPR 19036 end*/
    }
    else
    {
        LOG_MAC_MSG(MAC_TIMER_NODE_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                    0,\
                    tempCRNTICtx_p->tcRNTI,\
                    frameNumber,\
                    ulHarqEntityForMsg3TimerList_p->count,0,0,\
                    0.0,0.0,__func__,"NODE_FREE For MSG3");
    } 
    return MAC_SUCCESS;
}
/*SPR_3305_FIX_END*/
/************************************************************************
 * Function Name  :   ulHarqTimerStop 
 * Inputs         :   ueContext_p - A pointer to the DLUEContext,
 *                    frameNumber - The sub frame number for which the UE data
 *                                  was sent on the PHYSICAL layer,
 *                    currentSysFrameNumber
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs        :   None
 * Returns        :   MAC_SUCESS/MAC_FAILURE
 * Description    :   This functon will stop the HARQ Timer functionality and
 *                    update the relevent fields of the DL UE Context.
 ************************************************************************/


/* msg3_retx_tdd_support_start */
#ifdef ULHARQ_TIMER_PROC
/* msg3_retx_tdd_support_end */
 MacRetType ulHarqTimerStop(
                                  ULUEContext* ueContext_p,
                                  /* +- SPR 17777 */
                                  UInt8     frameNumber,
                                  UInt32 currentSysFrameNumber,
                                  /*CA Changes start  */
                                  InternalCellIndex internalCellIndex)
                                  /*CA Changes end  */
{
    ULHarqTimerEntityList* ulHarqEntityTimerList_p = PNULL;
    ULHarqTimerNode* ulharqTimerNode_p = PNULL;
    UInt8 ulHarqEntityTimerArrIndex = -1;
    LTE_MAC_UT_LOG(LOG_INFO,LOG_UL_HARQ,"[%s] Entry CTTI %d\n",__func__,getCurrentTick());
    UInt32  ulHarqTimerSysFrameIndex = 0;

    ulHarqTimerSysFrameIndex = MODULO_TWO(currentSysFrameNumber);

    /*CA Changes start  */
    ulHarqEntityTimerList_p = &ulHarqTimerEntityList_g[internalCellIndex][ulHarqTimerSysFrameIndex][frameNumber];
    /*CA Changes end  */

    if (!(ulHarqEntityTimerList_p->count))
    {
        /*
            The count is Zero symbolysing that there is no nodes avaliable in the 
            UL HARQ TIMER List Array.
        */
        LTE_MAC_UT_LOG(LOG_DETAIL, LOG_UL_HARQ, \
            "The value of count in ULHarqEntityTimerList is equal to" \
            "Zero the sub frame no is %d CTTI %d\n", \
             frameNumber,getCurrentTick());              
        LTE_MAC_UT_LOG(LOG_INFO,LOG_UL_HARQ,"[%s] Exit Return MAC_FAILURE",__func__);
        
        LOG_MAC_MSG(MAC_TIMER_CNT_ZERO_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                0,frameNumber,0,0,0,0,\
                0.0,0.0,__func__,"CNT_ZERO");
        return MAC_FAILURE;
    }
    ulHarqEntityTimerArrIndex = ueContext_p->ulHarqTimerNodeArrrayIndex[ulHarqTimerSysFrameIndex][frameNumber];

    if (ulHarqEntityTimerArrIndex == INVALID_COUNT)
    {
        LTE_MAC_UT_LOG(LOG_DETAIL, LOG_UL_HARQ, \
            "The value of count in ulHarqTimerNodeIndex is equal to" \
            "INVALID COUNT the sub frame no is %d CTTI %d",frameNumber,getCurrentTick());              
        LTE_MAC_UT_LOG(LOG_INFO,LOG_UL_HARQ,"[%s] Exit Return MAC_FAILURE",__func__);
        
        LOG_MAC_MSG(MAC_TIMER_CNT_INVALID_ULHARQ_ID,LOGWARNING,MAC_UL_HARQ,\
                0,ueContext_p->ueIndex,\
                frameNumber,0,0,0,\
                0.0,0.0,__func__,"CNT_INVALID");
        return MAC_FAILURE;
    }
     ulharqTimerNode_p = \
       &(ulHarqEntityTimerList_p->ulHarqTimerNodeArr[ulHarqEntityTimerArrIndex]);
   
    if (ulharqTimerNode_p->isValid) 
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,LOG_UL_HARQ,
        "STOP TIMER" 
        "HARQ ID %d EXPIRED SUB FRAME %d" 
        " ulHarqEntityTimerArrIndex %d" 
        " ulHarqEntityTimerList_p->count %d CTTI %d\n",
        ulharqTimerNode_p->harqProcessId,frameNumber,
        ulHarqEntityTimerArrIndex,ulHarqEntityTimerList_p->count,getCurrentTick());

        LOG_UT(MAC_TIMER_STOP_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                0,\
                ueContext_p->ueIndex,\
                ulharqTimerNode_p->harqProcessId,\
                frameNumber,\
                ulharqTimerNode_p->ulHarqExpiredTTI,\
                ulHarqEntityTimerList_p->count,\
                0.0,0.0,__func__,"TIMER_STOP");
   
        /*
            We now fill the array ulHarqTimerNodeIndex with an invalid entry.
        */
        ulharqTimerNode_p->isValid = HARQ_TIMER_NODE_INVALID;
        ulharqTimerNode_p->harqProcessId = MAX_HARQ_PROCESS_NB;
        ulharqTimerNode_p->ulHarqExpiredSubframe = MAX_SUB_FRAME;
        ulharqTimerNode_p->ulHarqExpiredSysframe = MAX_SFN_VALUE;
        ulharqTimerNode_p->ueIndex = INVALID_UE_ID;
        /* + SPS_TDD_Changes */
        ulharqTimerNode_p->schUsed = DRA_SCHEDULING;
        /* - SPS_TDD_Changes */
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_DETAIL, LOG_UL_HARQ, \
            "The HARQ TIME Node is already marked INVALID" \
            "corresponding to Harq Process Id %d ", \
            ulharqTimerNode_p->harqProcessId);
     
        LOG_MAC_MSG(MAC_TIMER_NODE_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                    0,\
                    ueContext_p->ueIndex,\
                    frameNumber,\
                    ulHarqEntityTimerList_p->count,0,0,\
                    0.0,0.0,__func__,"NODE_FREE");
    } 
    LTE_MAC_UT_LOG(LOG_INFO,LOG_UL_HARQ,"[%s] Exit",__func__);
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  :  expireAllNodeFromULHarqTimerList 
 * Inputs         :  loopOfNumOfSubFrames - Represent the number of previous
 *                                          subframes including the current tick 
 *                                          one for which the entry corresponding to 
 *                                          HARQ TIMER ARRAY has to be deleted,
 *                   currentSubFrame - the present sub frame,
 *                   delaySubFrame,
 *                   currentGlobalTick - the present global tick,
 *                   sysFrameNumber
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None 
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer 
 *                   corresponding to particular sub frame, this function puts 
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/

MacRetType expireAllNodeFromULHarqTimerList (
                                           UInt32 loopOfNumOfSubFrames,
                                           UInt8 currentSubFrame,
                                           UInt8 delaySubFrame,
                                           /* SPR 15909 fix start */
                                           tickType_t currentGlobalTick,
                                           /* SPR 15909 fix end */
                                           UInt32 sysFrameNumber,
                                           /*CA Changes start  */
                                           InternalCellIndex  internalCellIndex)
                                           /*CA Changes end  */
{
#ifdef FDD_CONFIG
    UInt8 ulHarqEntityNewTimerArrIndex = 0;
    UInt8 newphichReceiveIndex = 0;
    UInt32  reTxnSysFrameUpdate = 0;
    UInt32  reTxnSysFrameNumber = 0;
    UInt32  reTxnUlHarqTimerSysFrameIndex = 0;
    /* SPR 15909 fix start */
    tickType_t  timerExpiredTTI;
    /* SPR 15909 fix end */
    ULHarqTimerNode* ulharqNewTimerNode_p = PNULL;
    ULHarqTimerEntityList* ulHarqEntityNewTimerList_p = PNULL;
    UInt32 ulContainerIndex   = 0;
    UInt32 reTxnCntUpdate;
    UInt32 reTxnTickUpdate;
    UplinkContainer *currentContainer_p = PNULL;
    ContainerNackQueueNode *nackNode_p = PNULL;
    UInt8 maxUlHARQTx= 0;
    ULUEContextInfo* ulUeContextInfoArrayAccess_p = PNULL;
    ulUeContextInfoArrayAccess_p = &ulUECtxInfoArr_g[0];

    UInt16 ulHarqEntityTimerArrCount = 0;        
    ULHarqTimerEntityList* ulHarqEntityTimerList_p = PNULL;
    ULHarqTimerNode* ulharqTimerNode_p = PNULL;
    ULUEContextInfo* ulUEContextInfo_p = PNULL;
    ULUEContext*  ulUeContext_p = PNULL;        
    ULHarqInfo* ulHarqProcess_p = PNULL;
    UInt16 ueIndex = INVALID_UE_ID;
    UInt8 frameNumber = 0;
    UInt32 tickDiff;

    /* For  TTIB_Code */
    UInt8 harqMapId = 0;
    UInt8 counter = 0;

    /*++ SPR 19148*/
    LP_MacULSpsStatus macUlSpsStatus_p = PNULL;
    /*-- SPR 19148*/
    /*coverity 97027 +-*/
/* + FDD_COMPLIATION_WARNING */
#ifdef LOG_UT_ENABLED
    UInt32 expiredTimerCnt=0x0;
#endif
/* - FDD_COMPLIATION_WARNING */
    UInt32  ulHarqTimerSysFrameIndex = 0;
    UInt32 expirySysFrameNumber = 0;
/* SPR 19288 change start */
/* SPR 19288 change end */
    if (0 == currentSubFrame)
    {
        frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME - delaySubFrame;
        
        if(0 == sysFrameNumber)
        {
            expirySysFrameNumber = MAX_SYS_FRAME_NUM;
            ulHarqTimerSysFrameIndex = MODULO_TWO(expirySysFrameNumber);
        }
        else 
        {
            ulHarqTimerSysFrameIndex = MODULO_TWO(sysFrameNumber - 1);
        }

    }
    else if (currentSubFrame >= delaySubFrame)
    {
        frameNumber = currentSubFrame - delaySubFrame;
        ulHarqTimerSysFrameIndex = MODULO_TWO(sysFrameNumber);
    }
    else 
    {
        delaySubFrame -= currentSubFrame;
        frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME - delaySubFrame;

        if(0 == sysFrameNumber)
        {
            expirySysFrameNumber = MAX_SYS_FRAME_NUM;
            ulHarqTimerSysFrameIndex = MODULO_TWO(expirySysFrameNumber);
        }
        else 
        {
            ulHarqTimerSysFrameIndex = MODULO_TWO(sysFrameNumber - 1);
        }
       
	}
    if (frameNumber >= NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME)
    {
        frameNumber = frameNumber % NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
    }
    if(loopOfNumOfSubFrames >= MAX_NUM_OF_TICK_MISSED)
    {   
        /* SPR# 3860 Changes Starts */
	    loopOfNumOfSubFrames = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
        /* SPR# 3860 Changes Ends */
	    expireULHarqTimerNodeForTickMiss(loopOfNumOfSubFrames,
                                         /* CA Changes Start*/
                                         internalCellIndex);
                                         /* CA Changes End*/
#ifdef DL_UL_SPLIT
        UInt32 nackQContainerTick = MODULO_EIGHT(currentGlobalTick + PHY_DL_DELAY - FDD_HARQ_OFFSET);
        /*CA Changes start  */
        UplinkContainer* ulContainerQueue_p = ulContainerPool_gp[internalCellIndex] + nackQContainerTick;
        /*CA Changes end  */
        ContainerNackQueueNode *delimiterNackNode_p ;
        GET_MEM_FROM_POOL(ContainerNackQueueNode, delimiterNackNode_p ,sizeof(ContainerNackQueueNode), PNULL);

        delimiterNackNode_p->ueIdx = INVALID_UE_ID; 
        delimiterNackNode_p->ttiCounter = 0;

        putEntryInULContainerNackQueueUsingICC(ulContainerQueue_p->ulContainerQIdUL ,delimiterNackNode_p);
#endif
	    return MAC_SUCCESS;
    }
   
    while (loopOfNumOfSubFrames--)
    {
        /*CA Changes start  */
		ulHarqEntityTimerList_p = &ulHarqTimerEntityList_g[internalCellIndex][ulHarqTimerSysFrameIndex][frameNumber];    
        /*CA Changes end  */
        ulHarqEntityTimerArrCount = ulHarqEntityTimerList_p->count;

                  
        if (!ulHarqEntityTimerArrCount)
        {
            /*
               As there is no node in this Index of Array hence nothing is to be deleted
               remember the valid value of an array starts form zero and the above
               variable ulHarqEntityTimerList_p->count represents the array index.
             */          
            LOG_UT(MAC_EXPIRY_CNT_ZERO_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                    /*coverity 97027 +-*/0,frameNumber,0,0,0,0,\
                    0.0,0.0,__func__,"EXPIRY_CNT_ZERO");
            if(!frameNumber)
            {
                frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME -1;
            }
            else
            {
                frameNumber--;
            } 
            continue;
        }
/* + FDD_COMPLIATION_WARNING */
#ifdef LOG_UT_ENABLED
       expiredTimerCnt=ulHarqEntityTimerArrCount; 
#endif        
/* - FDD_COMPLIATION_WARNING */
        /** SPR 14759 Changes Strat **/
        while ( (ulHarqEntityTimerArrCount <= MAX_UL_UE_SCHEDULED )&&( ulHarqEntityTimerArrCount--))
        /** SPR 14759 Changes End **/
        {
            ulharqTimerNode_p = \
                                &(ulHarqEntityTimerList_p->ulHarqTimerNodeArr[ulHarqEntityTimerArrCount]);    
            
            if (ulharqTimerNode_p->isValid)
            {
                ulharqTimerNode_p->isValid = HARQ_TIMER_NODE_INVALID; 
                ueIndex = ulharqTimerNode_p->ueIndex;
                ulUEContextInfo_p = &ulUeContextInfoArrayAccess_p[ueIndex];
              
                if (!ulUEContextInfo_p->pendingDeleteFlag)
                {
                    ulUeContext_p = ulUEContextInfo_p->ulUEContext_p;
                    
                    if(ulharqTimerNode_p->ulHarqExpiredSubframe != frameNumber)
                    {
                                    
                        LOG_MAC_MSG(MAC_EXPIRY_SFMISMATCH_ULHARQ_ID,LOGERROR,MAC_UL_HARQ,\
                                /*coverity 97027 +-*/0,\
                                ulharqTimerNode_p->ulHarqExpiredSubframe,\
                                frameNumber,\
                                ulharqTimerNode_p->ueIndex,\
                                ulharqTimerNode_p->harqProcessId,\
                                ulharqTimerNode_p->ulHarqExpiredTTI,\
                                0.0,0.0,__func__,"EXPIRY_SF_MISMATCH");
                        
                        continue;
                    }

                    ulHarqProcess_p = 
                        &(ulUeContext_p->ulresInfo[ulharqTimerNode_p->harqProcessId]);
                    /* Perform adaptive re-transmission in case of timer expiry
                     * to again send the DCi PDU */
                    ulHarqProcess_p->nextReTransmissionType = ADAPTIVE_RE_TX;

                    /* Get the Tick diff to know about Tick Jump */
                    /* 32 bit overlap should be take care */
                    tickDiff = currentGlobalTick - ulharqTimerNode_p->ulHarqExpiredTTI;
                    if(tickDiff == DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY)
                    {
                        if( HARQ_PROCESS_FREE != ulHarqProcess_p->riLength)
                        {
                            
                            ulContainerIndex = ulharqTimerNode_p->ulHarqExpiredTTI  % MAX_CONTAINER_NUM;			
                            /*CA Changes start  */
                            currentContainer_p = ulContainerPool_gp[internalCellIndex] + ulContainerIndex;
                            /*CA Changes end  */
/* TTIB_Code Start */
                         if(TRUE == ulHarqProcess_p->isTTIBundled)
                         {
                            LTE_MAC_UT_LOG(LOG_DETAIL,LOG_UL_HARQ,"\n [%d] harqTimerExp , ueIndex[%d] "\
                                    ", HarqId_%d ,TickExpired: %d ,data[%d] \n"
                                    ,getCurrentTick(),ueIndex,ulharqTimerNode_p->harqProcessId,ulharqTimerNode_p->ulHarqExpiredTTI,
                                    ulHarqProcess_p->isTTIDataRcvd);

                             if(ulHarqProcess_p->isTTIDataRcvd)
                             {
                                 ContainerAckQueueNode *ackNode_p = PNULL;

                                 GET_MEM_FROM_POOL(ContainerAckQueueNode,ackNode_p,sizeof(ContainerAckQueueNode), PNULL); 
//Coverity_36214 fixed
                                if(PNULL == ackNode_p)
                                {
                                   continue;
                                }
//Coverity_36214 fixed
                                ulHarqProcess_p->harqStatus = HARQ_PROCESS_FREE;  
                                /*TTI Bundling Code Start*/

                                harqMapId = ulHarqProcess_p->ttiBundleFirstTick;
                                for(counter = 0 ; counter < TTI_BUNDLE_SIZE ; counter++)
                                {
                                    ulUeContext_p->ttiHarqMap[MODULO_SIXTEEN(harqMapId+counter)] = TTI_MAP_FREE;
                                }


                                 ackNode_p->ueIdx = ueIndex;
                                 ackNode_p->ttiCounter = ulharqTimerNode_p->ulHarqExpiredTTI;
                                 ackNode_p->crcValid  = 1;
                                 ackNode_p->ulHarqProcessId = ulharqTimerNode_p->harqProcessId;
                                #ifdef FDD_CONFIG
                                 ackNode_p->tcRntiFlag = FALSE;
                                 #endif
                                 /* + SPS_TDD_Changes */
                                 ackNode_p->schUsed = ulharqTimerNode_p->schUsed;
                                 /* - SPS_TDD_Changes */
                                 /* SPR 20154 Start */
                                 ackNode_p->isLastRetx = FALSE;
                                 /* SPR 20154 End */

                                 /* COVERITY CID 25409 fixes start */
                                 if( pushNode(&(currentContainer_p->ackQueue),
                                         &(ackNode_p->ackNodeAnchor)) )
                                 {
                                     lteWarning ("pushNode() failed for ulContainerPool");
                                     freeMemPool(ackNode_p);
                                     continue;
                                 }
                                 /* COVERITY CID 25409 fixes end */

                                 LTE_MAC_UT_LOG(LOG_DETAIL,LOG_UL_HARQ,"[%s]:[%d] Node pushed in nackQueueForTtiBundledUe.\n"
                                         ,__func__,__LINE__);
                                    /*++ SPR 19148*/
                                 if ( TRUE == checkSpsActTti(ulUeContext_p, ulharqTimerNode_p->harqProcessId)) 
                                 {
                                     GET_MEM_FROM_POOL(MacULSpsStatus, macUlSpsStatus_p,
                                             sizeof(MacULSpsStatus), PNULL);
                                     if( PNULL == macUlSpsStatus_p )
                                     {
                                         ltePanic("Memory is exhausted\n");
                                         /* Klockwork warning Changes Start */
                                         return MAC_FAILURE;
                                         /* Klockwork warning Changes End */
                                     }
                                     macUlSpsStatus_p->ueIndex = ueIndex;
                                     macUlSpsStatus_p->response = FIRST_ACK_NACK;
                                     /*CA Changes start  */
                                     /* + Coverity Id - 32583 */
                                     if(!ENQUEUE_MAC_UL_SPS_Q(ulSpsStatusQ_gp[internalCellIndex],
                                                 macUlSpsStatus_p))
                                     {
                                         freeMemPool(macUlSpsStatus_p);
                                         macUlSpsStatus_p = PNULL;	
                                     }
                                     /* - Coverity Id - 32583 */
                                     /* CA Stats Changes Start */
                                     /* Update Q Stats */
                                     LTE_MAC_QUEUE_USED( UL_SPS_STATUS_Q,
                                             QCOUNT_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex] ),
                                             internalCellIndex);
                                     /*CA Changes end  */
                                     /* CA Stats Changes End */
                                 }
                                    /*-- SPR 19148*/
                             }
                             else
                             {

                                 GET_MEM_NODE_NACK_QUEUE_FOR_TTIB_UE_Q(nackNode_p,ContainerNackQueueNode) 
                                 /* coverity 530 CID 25183 */
                                 if(PNULL == nackNode_p)
                                 {
                                    continue;   
                                 }
                                 /* coverity 530 CID 25183 */
                                 nackNode_p->ueIdx = ueIndex;
                                 nackNode_p->ttiCounter = ulharqTimerNode_p->ulHarqExpiredTTI;
                                 nackNode_p->ulHarqProcessId = ulharqTimerNode_p->harqProcessId;
                                 /* + SPS_TDD_Changes */
                                 nackNode_p->schUsed = ulharqTimerNode_p->schUsed;
                                 /* - SPS_TDD_Changes */
                                 /* Coverity 26384 Fix Start */
                                 if(!ENQUEUE_NACK_QUEUE_FOR_TTIB_UE_Q(&(currentContainer_p->nackQueueForTtiBundledUe),
                                         ContainerNackQueueNode,&(nackNode_p->nackNodeAnchor)))
                                 {
                                     FREE_MEM_NODE_NACK_QUEUE_FOR_TTIB_UE_Q(nackNode_p);
                                     continue;
                                 }
                                 /* Coverity 26384 Fix End */

                                  currentContainer_p->nackQueueCount++;
                                 LTE_MAC_UT_LOG(LOG_DETAIL,LOG_UL_HARQ,"[%s]:[%d] Node pushed in nackQueueForTtiBundledUe.\n"
                                         ,__func__,__LINE__);
                             }
                         }
                         else
                         {
                            GET_MEM_NODE_NACK_CIRC_Q(nackNode_p, ContainerNackQueueNode);
                            /* coverity 530 CID 25183 */
                            if(PNULL == nackNode_p)
                            {
                                continue;   
                            }
                            /* coverity 530 CID 25183 */
                            nackNode_p->ueIdx = ueIndex;
                            nackNode_p->ttiCounter = ulharqTimerNode_p->ulHarqExpiredTTI;
                            nackNode_p->ulHarqProcessId = ulharqTimerNode_p->harqProcessId;
                            /* + SPS_TDD_Changes */
                            nackNode_p->schUsed = ulharqTimerNode_p->schUsed;
                            /* - SPS_TDD_Changes */
                            /* SPR 5798 Fix Start */
                            currentContainer_p->nackQueueCount++;
                            /* SPR 5798 Fix End */
#ifndef DL_UL_SPLIT
                            /*SPR 7944 START*/
                            /* + SPS_TDD_Changes */
                            if(nackNode_p->schUsed)
                                /* - SPS_TDD_Changes */
                            {
                                /*coverity fix 25409*/
                                /* + SPS_TDD */
                                maxUlHARQTx = ulUeContext_p->spsUlInfo.maxSpsUlHARQTx -1;
                                /* - SPS_TDD */
                                if( pushNode(&(currentContainer_p->nackQueueForSps),
                                            &(nackNode_p->nackNodeAnchor)))
                                {
                                    FREE_MEM_NODE_NACK_CIRC_Q(nackNode_p);
                                    nackNode_p=PNULL;
                                    continue;   
                                }
                                /*coverity fix 25409*/
                            }
                            else
                            {
                               /*coverity fix 25409*/
                                /* + SPS_TDD */
                                maxUlHARQTx = ulUeContext_p->maxUlHARQTx -1;
                                /* - SPS_TDD */
                                if(pushNode(&(currentContainer_p->nackQueue),
                                    &(nackNode_p->nackNodeAnchor)))
                                {
                                    FREE_MEM_NODE_NACK_CIRC_Q(nackNode_p);
                                    nackNode_p=PNULL;
                                    continue;   
                                }

                                /*coverity fix 25409*/
                            }
                            /*SPR 7944 END*/
#else
#ifdef CIRC_QUEUE_IMPL
                            ENQUEUE_NACK_CIRC_Q(&(currentContainer_p->nackCircQueue),
                                    ContainerNackQueueNode,
                                    &(nackNode_p->nackNodeAnchor));
                            /*CA Changes start  */
                            semPostDefault(&waitForULNackQueue_g[internalCellIndex]);
                            /*CA Changes end  */
#else
                            putEntryInULContainerNackQueueUsingICC(currentContainer_p->ulContainerQIdUL, (nackNode_p));
#endif
#endif
                            LTE_MAC_UT_LOG(LOG_DETAIL,LOG_UL_HARQ,"[%s]:[%d] Node pushed in nackQueue.\n",__func__,__LINE__);
                         }
/* TTIB_Code End */
                            /* + SPS_TDD_Changes */
                            if (ulHarqProcess_p->currentRetransCount 
                                    < (maxUlHARQTx))
                            {
                                /* - SPS_TDD_Changes */
                            
                                UInt8 dci0Index = (ulharqTimerNode_p->ulHarqExpiredTTI + 4) %
                                    NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
                                /* IiCIC changes start */
                                /*CA Changes start  */
                                totalRetxRb_g[internalCellIndex][ulUeContext_p->userLocationType][dci0Index] +=
                                    ulHarqProcess_p->riLength;
                                /*CA Changes end  */
                                if (ulUeContext_p->userLocationType == CE_USER)
                                {
                                    /*CA Changes start  */
                                    totalRetxRb_g[internalCellIndex][CC_USER][dci0Index] +=
                                        ulHarqProcess_p->riLength;
                                    /*CA Changes end  */
                                }
                                /* ICIC changes end */
                            }

                            LOG_UT(MAC_EXPIRY_NACK_INS_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                                    /*coverity 97027 +-*/0,\
                                    ulharqTimerNode_p->ueIndex,\
                                    ulharqTimerNode_p->harqProcessId,\
                                    ulharqTimerNode_p->ulHarqExpiredSubframe,\
                                    ulharqTimerNode_p->ulHarqExpiredTTI,\
                                    ulHarqProcess_p->currentRetransCount,\
                                    (UDouble32)ulHarqProcess_p->nextReTransmissionType,\
                                    0.0,__func__,"EXPIRY_NACK_INS");
                        }
                        else
                        {
/* TTIB_Code Start */
                            /*SPS + TTIB Fix*/
                            /* SPR 13563 Changes start */
                            /* Freeing Harq process and Updating relevant HARQ TTI Map */
                                /* SPR 16916 FIX START */
#ifdef KPI_STATS
                                freeULHarqProcess(ulHarqProcess_p,ulUeContext_p->ueIndex,internalCellIndex);
#else
                                freeULHarqProcess(ulHarqProcess_p);
#endif
                                /* SPR 16916 FIX END */
#ifdef FDD_CONFIG
                                updateTTIHarqMap(ulharqTimerNode_p->harqProcessId,
                                                  ulHarqProcess_p,ulUeContext_p->ttiHarqMap);
#endif
                            /* SPR 13563 Changes end */
                            /* TTIB_Code End */
                    
                            LOG_UT(MAC_EXPIRY_HARQ_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                                    /*coverity 97027 +-*/0,\
                                    ulharqTimerNode_p->ueIndex,\
                                    ulharqTimerNode_p->harqProcessId,\
                                    ulharqTimerNode_p->ulHarqExpiredSubframe,\
                                    ulharqTimerNode_p->ulHarqExpiredTTI,0,\
                                    0.0,0.0,__func__,"EXPIRY_HARQ_FREE");
                        }
                    }
                    else
                    {


                        /* TIMER RESTART */                        

                        /* Update UL Harq Time node for re-transmission 
                         * 1. Update the Subframe for which timer to be restarted
                         * 2. update the expiry global tick
                         *
                         * 3. Increament the retransmiaaion count for the HARQ process
                         * 4. Check for Max retransmission Count
                         * 5. if Re-Txn Cnt < Max retransmission Count
                         * 		a. Add the node in Timer array
                         * 6. Else Free the Harq Process
                         */


                        /* 1. Update the Subframe for which timer to be restarted */
                        reTxnCntUpdate = ((tickDiff - 1) / RESTART_HARQ_TIMER_WINDOW) + 1;


                        reTxnTickUpdate = reTxnCntUpdate * RESTART_HARQ_TIMER_WINDOW;
						
						/*  Update the Sysframe for which timer to be restarted */
                        reTxnSysFrameUpdate = ((reTxnTickUpdate - 1) / NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME ) + 1;
						
                        newphichReceiveIndex  = (ulharqTimerNode_p->ulHarqExpiredSubframe \
                                +  reTxnTickUpdate) % MAX_SUB_FRAME;

                        reTxnSysFrameNumber = (ulharqTimerNode_p->ulHarqExpiredSysframe \
								+  reTxnSysFrameUpdate) % MAX_SFN_VALUE;

                        reTxnUlHarqTimerSysFrameIndex = MODULO_TWO(reTxnSysFrameNumber);

                        /* 2. update the expiry global tick */
                        timerExpiredTTI = ulharqTimerNode_p->ulHarqExpiredTTI + reTxnTickUpdate;

                        /* 3. Increament the retransmiaaion count for the HARQ process */
                        ulHarqProcess_p->currentRetransCount += reTxnCntUpdate;

                        /* SPS CHANGES */
/* TTIB_Code Start */
                        /* + SPS_TDD_Changes */
                        if(SPS_SCHEDULING == ulharqTimerNode_p->schUsed)
                        /* - SPS_TDD_Changes */
                        {
                            maxUlHARQTx = ulUeContext_p->spsUlInfo.maxSpsUlHARQTx -1;
                        }
                        else
                        {
                            maxUlHARQTx = ulUeContext_p->maxUlHARQTx -1;
                        }
                        /* - SPS_TDD_Changes */
/* TTIB_Code End */

                         /* 4. Check for Max retransmission Count */
                          /* SPR_1987 Start */
                        /* SPR 13563 Changes start */
                        if ((ulHarqProcess_p->currentRetransCount < maxUlHARQTx) && (HARQ_PROCESS_FREE !=ulHarqProcess_p->harqStatus))
                            /* SPR 13563 Changes end */
                        {                            
                        /* SPS CHANGES */
                            /* a. Add the node in Timer array */
                            /*CA Changes start  */
                            ulHarqEntityNewTimerList_p = 
                                &ulHarqTimerEntityList_g[internalCellIndex][reTxnUlHarqTimerSysFrameIndex][newphichReceiveIndex];
                            /*CA Changes end  */
                            ulHarqEntityNewTimerArrIndex = ulHarqEntityNewTimerList_p->count;


                            ulharqNewTimerNode_p = \
                                &(ulHarqEntityNewTimerList_p->
                                  ulHarqTimerNodeArr[ulHarqEntityNewTimerArrIndex]);

                            /* TIMER START */                        
                            if(HARQ_TIMER_NODE_INVALID == ulharqNewTimerNode_p->isValid)
                            {
                                ulharqNewTimerNode_p->ueIndex = ulharqTimerNode_p->ueIndex;
                                ulharqNewTimerNode_p->harqProcessId = ulharqTimerNode_p->harqProcessId;
                                ulharqNewTimerNode_p->isValid = HARQ_TIMER_NODE_VALID;
                                /* + SPS_TDD_Changes */
                                ulharqNewTimerNode_p->schUsed = ulharqTimerNode_p->schUsed;
                                /* - SPS_TDD_Changes */
                                ulharqNewTimerNode_p->ulHarqExpiredSubframe = newphichReceiveIndex;
                                ulharqNewTimerNode_p->ulHarqExpiredTTI  = timerExpiredTTI;
                                ulharqNewTimerNode_p->ulHarqExpiredSysframe = reTxnUlHarqTimerSysFrameIndex;
                                ulUeContext_p->ulHarqTimerNodeArrrayIndex[reTxnUlHarqTimerSysFrameIndex][newphichReceiveIndex] = \
                                                                                       ulHarqEntityNewTimerArrIndex;
                                ulHarqEntityNewTimerList_p->count++;


                                LOG_UT(MAC_TICKDIFF_TIMER_START_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                                        /*coverity 97027 +-*/0,\
                                        ulharqTimerNode_p->ueIndex,\
                                        ulharqTimerNode_p->harqProcessId,\
                                        newphichReceiveIndex,\
                                        timerExpiredTTI,\
                                        ulHarqEntityNewTimerList_p->count,\
                                        (UDouble32)ulHarqProcess_p->currentRetransCount,\
                                        (UDouble32)ulHarqProcess_p->nextReTransmissionType, 
                                        __func__,"TICKDIFF_TIMER_START");


                            }
                            else
                            {
                                    LOG_UT(MAC_TICKDIFF_NODE_BUSY_ULHARQ_ID,LOGERROR,MAC_UL_HARQ,\
                                            /*coverity 97027 +-*/0,\
                                            ulharqTimerNode_p->ueIndex,\
                                            ulharqTimerNode_p->harqProcessId,\
                                            newphichReceiveIndex,\
                                            timerExpiredTTI,\
                                            ulHarqEntityNewTimerList_p->count,\
                                            (UDouble32)ulHarqProcess_p->currentRetransCount,\
                                            (UDouble32)ulHarqProcess_p->nextReTransmissionType,\
                                            __func__,"TICKDIFF_NODE_BUSY");
                            }
                        }
                        else
                        {
                            /* Else Free the Harq Process */

                            /* Put the entry in nack queue
                             * In Nack Queue the node will be checked for
                             * Max retransmission and the Harq will be freed
                             */

/* SPR 19288 change start */
/* SPR 19288 change end */							
                            ulContainerIndex = timerExpiredTTI % MAX_CONTAINER_NUM;
                            /*CA Changes start  */
                            currentContainer_p = ulContainerPool_gp[internalCellIndex] + ulContainerIndex;
                            /*CA Changes end  */

/* TTIB_Code Start */
                         if(TRUE == ulHarqProcess_p->isTTIBundled)
                         {
                            LTE_MAC_UT_LOG(LOG_DETAIL,LOG_UL_HARQ,"\n [%d] harqTimerExp , ueIndex[%d] ,"\
                                    "HarqId_%d ,TickExpired: %d ,data[%d] \n"
                                    ,getCurrentTick(),ueIndex,ulharqTimerNode_p->harqProcessId,ulharqTimerNode_p->ulHarqExpiredTTI,
                                    ulHarqProcess_p->isTTIDataRcvd);

                             if(ulHarqProcess_p->isTTIDataRcvd)
                             {

                                 harqMapId = ulHarqProcess_p->ttiBundleFirstTick;

                                 for(counter = 0 ; counter < TTI_BUNDLE_SIZE ; counter++)
                                 {
                                     ulUeContext_p->ttiHarqMap[MODULO_SIXTEEN(harqMapId+counter)] = TTI_MAP_FREE;
                                 }
                                 ContainerAckQueueNode *ackNode_p = PNULL;

                                 GET_MEM_FROM_POOL(ContainerAckQueueNode,ackNode_p,sizeof(ContainerAckQueueNode), PNULL); 

//Coverity_36214 fixed
                                if(PNULL == ackNode_p)
                                {
                                   continue;
                                }
//Coverity_36214 fixed

                                 ackNode_p->ueIdx = ueIndex;
                                 ackNode_p->ttiCounter = ulharqTimerNode_p->ulHarqExpiredTTI;
                                 ackNode_p->crcValid  = 1;
                                 /* + SPS_TDD_Changes */
                                 ackNode_p->schUsed = ulharqTimerNode_p->schUsed;
                                 /* - SPS_TDD_Changes */
                                #ifdef FDD_CONFIG
                                 ackNode_p->tcRntiFlag = FALSE;
                                 #endif
                                 ackNode_p->ulHarqProcessId = ulharqTimerNode_p->harqProcessId;
                                 /* + coverity 36236 */
                                 if (pushNode(&(currentContainer_p->ackQueue),
                                         &(ackNode_p->ackNodeAnchor)))
                                 {
                                     freeMemPool(ackNode_p);
                                     continue;

                                 }
                                 else
                                 {
                                     LTE_MAC_UT_LOG(LOG_DETAIL,LOG_UL_HARQ,"[%s]:[%d] Node pushed in nackQueueForTtiBundledUe.\n"
                                             ,__func__,__LINE__);
                                 }
                                 /*++ SPR 19148*/
                                 if ( TRUE == checkSpsActTti(ulUeContext_p, ulharqTimerNode_p->harqProcessId)) 
                                 {
                                     GET_MEM_FROM_POOL(MacULSpsStatus, macUlSpsStatus_p,
                                             sizeof(MacULSpsStatus), PNULL);
                                     if( PNULL == macUlSpsStatus_p )
                                     {
                                         ltePanic("Memory isexhausted\n");
                                         /* Klockwork warning Changes Start */
                                         return MAC_FAILURE;
                                         /* Klockwork warning Changes End */
                                     }
                                     macUlSpsStatus_p->ueIndex = ueIndex;
                                     macUlSpsStatus_p->response = FIRST_ACK_NACK;
                                     /*CA Changes start  */
                                     /* + Coverity Id - 32583 */
                                     if(!ENQUEUE_MAC_UL_SPS_Q(ulSpsStatusQ_gp[internalCellIndex],
                                                 macUlSpsStatus_p))
                                     {
                                         freeMemPool(macUlSpsStatus_p);
                                         macUlSpsStatus_p = PNULL;	
                                     }
                                     /* - Coverity Id - 32583 */
                                     /* CA Stats Changes Start */
                                     /* Update Q Stats */
                                     LTE_MAC_QUEUE_USED( UL_SPS_STATUS_Q,
                                             QCOUNT_MAC_UL_SPS_Q( ulSpsStatusQ_gp[internalCellIndex] ),
                                             internalCellIndex);
                                     /*CA Changes end  */
                                     /* CA Stats Changes End */
                                 }
                                    /*-- SPR 19148*/

                             }
                             else
                             {
                                 ContainerNackQueueNode *nackNode_p = PNULL;

                                 GET_MEM_NODE_NACK_QUEUE_FOR_TTIB_UE_Q(nackNode_p,ContainerNackQueueNode)
                                 /* coverity 530 CID 25183 */
                                 if(PNULL == nackNode_p)
                                 {
                                    continue;   
                                 }
                                 /* coverity 530 CID 25183 */
                                 nackNode_p->ueIdx = ueIndex;
                                 nackNode_p->ttiCounter = ulharqTimerNode_p->ulHarqExpiredTTI;
                                 nackNode_p->ulHarqProcessId = ulharqTimerNode_p->harqProcessId;
                                 /* + SPS_TDD_Changes */
                                 nackNode_p->schUsed = ulharqTimerNode_p->schUsed;
                                 /* - SPS_TDD_Changes */
                                 /* Coverity 26384 Fix Start */
                                 if(!ENQUEUE_NACK_QUEUE_FOR_TTIB_UE_Q(&(currentContainer_p->nackQueueForTtiBundledUe),
                                         ContainerNackQueueNode,&(nackNode_p->nackNodeAnchor)))
                                 {
                                     FREE_MEM_NODE_NACK_QUEUE_FOR_TTIB_UE_Q(nackNode_p);
                                     continue;
                                 }
                                 /* Coverity 26384 Fix End */

                                  currentContainer_p->nackQueueCount++;
                                 LTE_MAC_UT_LOG(LOG_DETAIL,LOG_UL_HARQ,"[%s]:[%d] Node pushed in nackQueueForTtiBundledUe.\n"
                                         ,__func__,__LINE__);
                             }

                         }
                         else
                         {
                                /* SPR 13563 Changes start */
                                /*
                                   if ((HARQ_PROCESS_FREE !=ulHarqProcess_p->harqStatus))
                                   {
                                   continue;
                                   } */
                                /* SPR 13563 Changes end */
                             GET_MEM_NODE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(nackNode_p,
                                     ContainerNackQueueNode);
                                 /* coverity 530 CID 25183 */
                                 if(PNULL == nackNode_p)
                                 {
                                    continue;   
                                 }
                                 /* coverity 530 CID 25183 */
                             nackNode_p->ueIdx = ueIndex;
                             nackNode_p->ttiCounter = timerExpiredTTI;
                             nackNode_p->ulHarqProcessId = ulharqTimerNode_p->harqProcessId;
                             /* + SPS_TDD_Changes */
                             nackNode_p->schUsed = ulharqTimerNode_p->schUsed;
#ifndef DL_UL_SPLIT
                            /*SPR 7944 START*/
                             if(SPS_SCHEDULING == nackNode_p->schUsed)
                                 /* - SPS_TDD_Changes */
                            {
                                /*coverity fix 25409*/
                                if(pushNode(&(currentContainer_p->nackQueueForSps),
                                            &(nackNode_p->nackNodeAnchor)))
                                {
                                    FREE_MEM_NODE_NACK_QUEUE_FOR_TTIB_UE_Q(nackNode_p);
                                    nackNode_p=PNULL;
                                    continue;

                                }
                                /*coverity fix 25409*/
                            }
                            else
                            {
                                /*coverity fix 25409*/
                                if(pushNode(&(currentContainer_p->nackQueue),
                                            &(nackNode_p->nackNodeAnchor)))
                                {
                                    FREE_MEM_NODE_NACK_QUEUE_FOR_TTIB_UE_Q(nackNode_p);
                                    nackNode_p=PNULL;
                                    continue;
                                }
                                /*coverity fix 25409*/
                            }
                            /*SPR 7944 END*/
#else
#ifdef CIRC_QUEUE_IMPL
                            ENQUEUE_NACK_CIRC_Q(&(currentContainer_p->nackCircQueue),
                                    ContainerNackQueueNode,
                                    &(nackNode_p->nackNodeAnchor));
                            /*CA Changes start  */
                            semPostDefault(&waitForULNackQueue_g[internalCellIndex]);
                            /*CA Changes end  */
#else
                            putEntryInULContainerNackQueueUsingICC(currentContainer_p->ulContainerQIdUL, (nackNode_p));
#endif
#endif
                             LTE_MAC_UT_LOG(LOG_DETAIL,LOG_UL_HARQ,"[%s]:[%d] Node pushed in nackQueue.\n",__func__,__LINE__);
                         } 
/* TTIB_Code End */
                            LOG_MAC_MSG(MAC_TICKDIFF_EXPIRY_NACK_INS_ULHARQ_ID,\
                                    LOGDEBUG,MAC_UL_HARQ,\
                                    /*coverity 97027 +-*/0,\
                                    ulharqTimerNode_p->ueIndex,\
                                    ulharqTimerNode_p->harqProcessId,\
                                    ulharqTimerNode_p->ulHarqExpiredSubframe,\
                                    ulharqTimerNode_p->ulHarqExpiredTTI,\
                                    timerExpiredTTI,\
                                    (UDouble32)ulHarqProcess_p->currentRetransCount,\
                                    (UDouble32)ulHarqProcess_p->nextReTransmissionType,\
                                    __func__,"TICKDIFF_EXPIRY_NACK_INS");
                           /* + E_CID_5.3 */
                           if( ulUeContext_p->eCidReportParams.dummyDCI0Entry_p == PNULL &&
                                 ulUeContext_p->eCidReportParams.reportStatus == WAIT_FOR_REPORTS_TA_1_OR_AOA)
                           {

                                putEntryInEcidMeasReportQueue(ulUeContext_p->ueIndex,\
                                             /*CA Changes start  */
                                             NO_REPORT_TYPE_REQUIRED,MAC_FAILURE, internalCellIndex);
                                             /*CA Changes end  */
                           }
                         
                           /* - E_CID_5.3 */
                        }
                    }
                    /* TIMER STOP */
                   LOG_UT(MAC_EXPIRY_STOP_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                                /*coverity 97027 +-*/0,\
                                ulUeContext_p->ueIndex,\
                                ulharqTimerNode_p->harqProcessId,\
                                frameNumber,\
                                ulharqTimerNode_p->ulHarqExpiredTTI,\
                                ulHarqEntityTimerList_p->count,\
                                0.0,0.0,__func__,"EXPIRY_TIMER_STOP");
                    

                    /*
                       We now fill the array ulHarqTimerNodeIndex with an invalid entry.
                     */
                    ulUeContext_p->ulHarqTimerNodeArrrayIndex[ulHarqTimerSysFrameIndex][frameNumber] = \
                                                                             INVALID_COUNT;

                    ulharqTimerNode_p->ulHarqExpiredSubframe = MAX_SUB_FRAME;

                    ulharqTimerNode_p->ueIndex = INVALID_UE_ID;

                    ulharqTimerNode_p->harqProcessId = MAX_HARQ_PROCESS_NB;
                }
                else
                {
                
                    LOG_UT(MAC_EXPIRY_UEDELETE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                            /*coverity 97027 +-*/0,\
                            ueIndex,\
                            frameNumber,\
                            0,0,0,\
                            0.0,0.0,__func__,"EXPIRY_UEDELETE");
                    continue;
                }
            }
            else
            {
          
                  LOG_UT(MAC_EXPIRY_NODE_FREE_ULHARQ_ID,LOGDEBUG,MAC_UL_HARQ,\
                          /*coverity 97027 +-*/0,\
                          frameNumber,\
                          expiredTimerCnt,\
                          ulHarqEntityTimerArrCount,0,0,\
                          0.0,0.0,__func__,"EXPIRY_NODE_FREE");
            }
        }
        ulHarqEntityTimerList_p->count = 0;
        if(!frameNumber)
        {
            frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME -1;
        }
        else
        {
            frameNumber--;
        }
    }

#ifdef DL_UL_SPLIT
        UInt32 nackQContainerTick = MODULO_EIGHT(currentGlobalTick + PHY_DL_DELAY - FDD_HARQ_OFFSET);
        /*CA Changes start  */
        UplinkContainer* ulContainerQueue_p = ulContainerPool_gp[internalCellIndex] + nackQContainerTick;
        /*CA Changes end  */
        ContainerNackQueueNode *delimiterNackNode_p ;
        GET_MEM_NODE_NACK_CIRC_Q(delimiterNackNode_p ,ContainerNackQueueNode);

        /* COVERITY CID 26406 Fix start */ 
        if (!delimiterNackNode_p)
        {
            ltePanic("Memory Allocation failure for deliminator node\n");
            return MAC_FAILURE;
        }
        /* COVERITY CID 26406 Fix end */ 

        delimiterNackNode_p->ueIdx = INVALID_UE_ID; 
        delimiterNackNode_p->ttiCounter = 0;
#ifdef CIRC_QUEUE_IMPL
        ENQUEUE_NACK_CIRC_Q(&(ulContainerQueue_p->nackCircQueue),
                ContainerNackQueueNode,
                &(delimiterNackNode_p->nackNodeAnchor));
        /*CA Changes start  */
        semPostDefault(&waitForULNackQueue_g[internalCellIndex]);
        /*CA Changes end  */
#else
        putEntryInULContainerNackQueueUsingICC(ulContainerQueue_p->ulContainerQIdUL ,delimiterNackNode_p);
#endif

#endif

#endif

    return MAC_SUCCESS;
}
/*SPR_3305_FIX_START*/
/*****************************************************************************
 * Function Name  :  expireAllNodeFromULHarqTimerForMsg3List 
 * Inputs         :  loopOfNumOfSubFrames - Represent the number of previous
 *                                          subframes including the current tick
 *                                          one for which the entry corresponding 
 *                                          to HARQ TIMER ARRAY has to be deleted,
 *                   currentSubFrame - present sub frame,
 *                   delaySubFrame,
 *                   currentGlobalTick - the present global tick,
 *                   sysFrameNumber
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE. 
 * Description    :  This function will remove all the nodes in the HARQ Timer 
 *                   corresponding to particular sub frame, this function puts 
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/

MacRetType expireAllNodeFromULHarqTimerForMsg3List (
                                           UInt32 loopOfNumOfSubFrames,
                                           UInt8 currentSubFrame,
                                           UInt8 delaySubFrame,
                                           /* SPR 15909 fix start */ 
                                           tickType_t currentGlobalTick,
                                           /* SPR 15909 fix end */
                                           UInt32 sysFrameNumber,
                                           /*CA Changes start  */
                                           InternalCellIndex  internalCellIndex)
                                           /*CA Changes end  */
{
#ifdef FDD_CONFIG
	UInt8 ulHarqEntityNewTimerArrIndex = 0;
	UInt8 newphichReceiveIndex = 0;
	UInt16 ulHarqEntityTimerArrCount = 0;        
	ULHarqTimerEntityForMsg3List* ulHarqEntityTimerList_p = PNULL;
	ULHarqTimerEntityForMsg3List* ulHarqEntityNewTimerList_p = PNULL;
	ULHarqTimerForMsg3Node* ulharqTimerNode_p = PNULL;
	ULHarqTimerForMsg3Node* ulharqNewTimerNode_p = PNULL;
	UInt8 frameNumber = 0;
    UInt32 ulContainerIndex   = 0;
    UInt32 reTxnCntUpdate;
    UInt32 reTxnTickUpdate;
    /* SPR 15909 fix start */
    tickType_t  timerExpiredTTI;
    /* SPR 15909 fix end */
    UInt16 ueIndex = INVALID_UE_ID;
    ULHarqInfo* ulHarqProcess_p = PNULL;
    UInt32 tickDiff;
    TempCRNTICtx * tempCRNTICtx_p = PNULL;
    ContainerNackQueueNode *nackNode_p = PNULL;
    UplinkContainer *currentContainer_p = PNULL;
    UInt32  ulHarqTimerSysFrameIndex = 0;
    UInt32  reTxnSysFrameUpdate = 0;
    UInt32  reTxnSysFrameNumber = 0;
    UInt32  reTxnUlHarqTimerSysFrameIndex = 0;
    UInt32 expirySysFrameNumber = 0;

	if (0 == currentSubFrame)
	{
		frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME - delaySubFrame;
        
        if(0 == sysFrameNumber)
        {
            expirySysFrameNumber = MAX_SYS_FRAME_NUM;
            ulHarqTimerSysFrameIndex = MODULO_TWO(expirySysFrameNumber);
        }
        else 
        {
            ulHarqTimerSysFrameIndex = MODULO_TWO(sysFrameNumber - 1);
        }

	}
	else if (currentSubFrame >= delaySubFrame)
	{
		frameNumber = currentSubFrame - delaySubFrame;
        ulHarqTimerSysFrameIndex = MODULO_TWO(sysFrameNumber);
	}
	else 
	{
		delaySubFrame -= currentSubFrame;
		frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME - delaySubFrame;

        if(0 == sysFrameNumber)
        {
            expirySysFrameNumber = MAX_SYS_FRAME_NUM;
            ulHarqTimerSysFrameIndex = MODULO_TWO(expirySysFrameNumber);
        }
        else 
        {
            ulHarqTimerSysFrameIndex = MODULO_TWO(sysFrameNumber - 1);
        }
       
	}
    if (frameNumber >= NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME)
    {
        frameNumber = frameNumber % NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;
    }

	while (loopOfNumOfSubFrames--)
	{
        /*CA Changes start  */
		ulHarqEntityTimerList_p = &ulHarqTimerEntityForMsg3List_g[internalCellIndex][ulHarqTimerSysFrameIndex][frameNumber];    
        /*CA Changes end  */
		ulHarqEntityTimerArrCount = ulHarqEntityTimerList_p->count;


		if (!ulHarqEntityTimerArrCount)
		{
			/*
			   As there is no node in this Index of Array hence nothing is to be deleted
			   remember the valid value of an array starts form zero and the above
			   variable ulHarqEntityTimerList_p->count represents the array index.
			 */

			frameNumber++;
			if (frameNumber == NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME)
			{
				frameNumber = 0;
                ulHarqTimerSysFrameIndex = MODULO_TWO(sysFrameNumber);
			}
			continue;
		}

        while (ulHarqEntityTimerArrCount--)
        {
            ulharqTimerNode_p = \
                                &(ulHarqEntityTimerList_p->ulHarqForMsg3TimerNodeArr[ulHarqEntityTimerArrCount]);    

            if (ulharqTimerNode_p->isValid)
            {
                ulharqTimerNode_p->isValid = HARQ_TIMER_NODE_INVALID; 
                ueIndex = ulharqTimerNode_p->ueIndex;
                tempCRNTICtx_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;
                if (PNULL != tempCRNTICtx_p)
                {
					ulHarqProcess_p = &(tempCRNTICtx_p->ulHarq);
					if(ulharqTimerNode_p->ulHarqExpiredSubframe != frameNumber)
					{

						LOG_MAC_MSG(MAC_EXPIRY_SFMISMATCH_ULHARQ_ID,LOGERROR,MAC_UL_HARQ,\
								currentGlobalTick,\
								ulharqTimerNode_p->ulHarqExpiredSubframe,\
								frameNumber,\
								ulharqTimerNode_p->ueIndex,\
								ulharqTimerNode_p->harqProcessId,\
								ulharqTimerNode_p->ulHarqExpiredTTI,\
								0.0,0.0,__func__,"EXPIRY_SF_MISMATCH For MSG3");

						continue;
					}

					/* Get the Tick diff to know about Tick Jump */
					tickDiff = currentGlobalTick - ulharqTimerNode_p->ulHarqExpiredTTI;
					if(tickDiff == DELAY_IN_SUBFRAMES_FOR_HARQ_EXPIRY)
					{
						if( HARQ_PROCESS_FREE != ulHarqProcess_p->riLength)
						{
							ulContainerIndex = ulharqTimerNode_p->ulHarqExpiredTTI  % MAX_CONTAINER_NUM;			
                            /*CA Changes start  */
                            currentContainer_p = ulContainerPool_gp[internalCellIndex] + ulContainerIndex;
                            /*CA Changes end  */
                            GET_MEM_NODE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(nackNode_p,
                                    ContainerNackQueueNode);
                            /*+coverity-530-CID 25164 */
                            if(PNULL == nackNode_p)
                            {
                                continue;
                            }
                            /*+coverity-530-CID 25164 */
							nackNode_p->ueIdx = ueIndex;
							nackNode_p->ttiCounter = ulharqTimerNode_p->ulHarqExpiredTTI;

#ifndef DL_UL_SPLIT
                            if(pushNode(&(currentContainer_p->nackQueueForTempUE),
                                        &(nackNode_p->nackNodeAnchor)))
                            {
                                /*coverity fix 25408*/
                                FREE_MEM_NODE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(nackNode_p);
                                continue;
                                /*coverity fix 25408*/

                            }
#else
#ifdef CIRC_QUEUE_IMPL
                            ENQUEUE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(&(currentContainer_p->nackCircQueueForTempUE),
                                    ContainerNackQueueNode,
                                    &(nackNode_p->nackNodeAnchor));
                            /*CA Changes start  */
                            semPostDefault(&waitForULNackForTempUE_g[internalCellIndex]);
                            /*CA Changes end  */
#else
                            putEntryInULContainerNackQueueUsingICC(currentContainer_p->ulContainerNackQueueForTempUEIdUL, (nackNode_p));
#endif
#endif
                            /* SPR 5798 Fix Start */
                            currentContainer_p->nackQueueCount++;
                            /* SPR 5798 Fix End */
                           
						}
						else
						{
						}
					}
					else
					{


						/* TIMER RESTART */                        

						/* Update UL Harq Time node for re-transmission 
						 * 1. Update the Subframe for which timer to be restarted
						 * 2. update the expiry global tick
						 *
						 * 3. Increament the retransmiaaion count for the HARQ process
						 * 4. Check for Max retransmission Count
						 * 5. if Re-Txn Cnt < Max retransmission Count
						 * 		a. Add the node in Timer array
						 * 6. Else Free the Harq Process
						 */


						/* 1. Update the Subframe for which timer to be restarted */
						reTxnCntUpdate = ((tickDiff - 1) / RESTART_HARQ_TIMER_WINDOW) + 1;


						reTxnTickUpdate = reTxnCntUpdate * RESTART_HARQ_TIMER_WINDOW;
						
						/*  Update the Sysframe for which timer to be restarted */
                        reTxnSysFrameUpdate = ((reTxnTickUpdate - 1) / NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME ) + 1;
						
                        newphichReceiveIndex  = (ulharqTimerNode_p->ulHarqExpiredSubframe \
								+  reTxnTickUpdate) % MAX_SUB_FRAME;

                        reTxnSysFrameNumber = (ulharqTimerNode_p->ulHarqExpiredSysframe \
								+  reTxnSysFrameUpdate) % MAX_SFN_VALUE;

                        reTxnUlHarqTimerSysFrameIndex = MODULO_TWO(reTxnSysFrameNumber);

						/* 2. update the expiry global tick */
						timerExpiredTTI = ulharqTimerNode_p->ulHarqExpiredTTI + reTxnTickUpdate;

						/* 3. Increament the retransmiaaion count for the HARQ process */
						ulHarqProcess_p->currentRetransCount += reTxnCntUpdate;

						/* 4. Check for Max retransmission Count */
						/* SPR_1987 Start */
						if(ulHarqProcess_p->currentRetransCount < 
                            /*CA Changes start  */
                                (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                                 rachConfigInfo.rachConfiguration.maxHarqMsg3Tx - 1))
							/* SPR_1987 End */
						{
							/* a. Add the node in Timer array */
							ulHarqEntityNewTimerList_p = 
                                &ulHarqTimerEntityForMsg3List_g[internalCellIndex][reTxnUlHarqTimerSysFrameIndex][newphichReceiveIndex];
                            /*CA Changes end  */
							ulHarqEntityNewTimerArrIndex = ulHarqEntityNewTimerList_p->count;


							ulharqNewTimerNode_p = \
						       &(ulHarqEntityNewTimerList_p->
                                   ulHarqForMsg3TimerNodeArr[ulHarqEntityNewTimerArrIndex]);

							/* TIMER START */                        
							if(HARQ_TIMER_NODE_INVALID == ulharqNewTimerNode_p->isValid)
							{
								ulharqNewTimerNode_p->ueIndex = ulharqTimerNode_p->ueIndex;
								ulharqNewTimerNode_p->harqProcessId = ulharqTimerNode_p->harqProcessId;
								ulharqNewTimerNode_p->isValid = HARQ_TIMER_NODE_VALID;
								ulharqNewTimerNode_p->ulHarqExpiredSubframe = newphichReceiveIndex;
								ulharqNewTimerNode_p->ulHarqExpiredTTI  = timerExpiredTTI;
								tempCRNTICtx_p->ulHarqTimerNodeArrrayIndex = \
														 ulHarqEntityNewTimerArrIndex;
								ulHarqEntityNewTimerList_p->count++;


							}
							else
							{
							}
						}
						else
						{
							/* Else Free the Harq Process */

							/* Put the entry in nack queue
							 * In Nack Queue the node will be checked for
							 * Max retransmission and the Harq will be freed
							 */

							ulContainerIndex = timerExpiredTTI % MAX_CONTAINER_NUM;
                            /*CA Changes start  */
                            currentContainer_p = ulContainerPool_gp[internalCellIndex] + ulContainerIndex;
                            /*CA Changes end  */
                            GET_MEM_NODE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(nackNode_p,
                                    ContainerNackQueueNode);
                            /*coverity-530 CID 25164 */
                            if(PNULL == nackNode_p)
                            {
                                continue;
                            }
                            /*coverity-530 CID 25164 */
							nackNode_p->ueIdx = ueIndex;
							nackNode_p->ttiCounter = timerExpiredTTI;
                            /* SPR 5798 Fix Start */
                            currentContainer_p->nackQueueCount++;
                            /* SPR 5798 Fix End */

#ifndef DL_UL_SPLIT
                            if(pushNode(&(currentContainer_p->nackQueueForTempUE),
                                    &(nackNode_p->nackNodeAnchor)))
                            {
                                /*coverity fix 25408*/
                                FREE_MEM_NODE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(nackNode_p);
                                continue;
                                /*coverity fix 25408*/
                            }
#else
#ifdef CIRC_QUEUE_IMPL
                            ENQUEUE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(&(currentContainer_p->nackCircQueueForTempUE),
                                    ContainerNackQueueNode,
                                    &(nackNode_p->nackNodeAnchor));
                            /*CA Changes start  */
                            semPostDefault(&waitForULNackForTempUE_g[internalCellIndex]);
                            /*CA Changes end  */
#else
                            putEntryInULContainerNackQueueUsingICC(currentContainer_p->ulContainerNackQueueForTempUEIdUL, (nackNode_p));
#endif
#endif
							LOG_MAC_MSG(MAC_TICKDIFF_EXPIRY_NACK_INS_ULHARQ_ID,\
									LOGDEBUG,MAC_UL_HARQ,\
									currentGlobalTick,\
									ulharqTimerNode_p->ueIndex,\
									ulharqTimerNode_p->harqProcessId,\
									ulharqTimerNode_p->ulHarqExpiredSubframe,\
									ulharqTimerNode_p->ulHarqExpiredTTI,\
									timerExpiredTTI,\
									(UDouble32)ulHarqProcess_p->currentRetransCount,\
									(UDouble32)ulHarqProcess_p->nextReTransmissionType,\
									__func__,"TICKDIFF_EXPIRY_NACK_INS For MSG3");

						}
					}


					/*
					   We now fill the array ulHarqTimerNodeIndex with an invalid entry.
					 */
					tempCRNTICtx_p->ulHarqTimerNodeArrrayIndex = \
											 INVALID_COUNT;
					ulharqTimerNode_p->ulHarqExpiredSubframe = MAX_SUB_FRAME;

					ulharqTimerNode_p->ueIndex = INVALID_UE_ID;

					ulharqTimerNode_p->harqProcessId = MAX_HARQ_PROCESS_NB;
				}
				else
				{

					continue;

				}
			}
			else
			{

			}
		}
		ulHarqEntityTimerList_p->count = 0;

		frameNumber++;
		if (frameNumber == NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME)
		{
			frameNumber = 0;
            ulHarqTimerSysFrameIndex = MODULO_TWO(sysFrameNumber);
		}
	}
#ifdef DL_UL_SPLIT
    UInt32 nackQContainerTick = MODULO_EIGHT(currentGlobalTick + PHY_DL_DELAY - FDD_HARQ_OFFSET);
    /*CA Changes start  */
    UplinkContainer* ulContainerQueue_p = ulContainerPool_gp[internalCellIndex] + nackQContainerTick;
    /*CA Changes end  */
    ContainerNackQueueNode *delimiterNackNode_p ;
    GET_MEM_NODE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(delimiterNackNode_p ,ContainerNackQueueNode);

    /* COVERITY CID 26405 fix start */
    if (!delimiterNackNode_p)
    {
        ltePanic("Memory Allocation failure for deliminator node\n");
        return MAC_FAILURE;
    }
    /* COVERITY CID 26405 fix end */

    delimiterNackNode_p->ueIdx = INVALID_UE_ID; 
    delimiterNackNode_p->ttiCounter = 0;

#ifdef CIRC_QUEUE_IMPL
    ENQUEUE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(&(ulContainerQueue_p->nackCircQueueForTempUE),
            ContainerNackQueueNode,
            &(delimiterNackNode_p->nackNodeAnchor));
    /*CA Changes start  */
    semPostDefault(&waitForULNackForTempUE_g[internalCellIndex]);
    /*CA Changes end  */
#else
    putEntryInULContainerNackQueueUsingICC(ulContainerQueue_p->ulContainerNackQueueForTempUEIdUL,delimiterNackNode_p);
#endif
#endif



	return MAC_SUCCESS;
}

#endif
/*SPR_3305_FIX_END*/

/************************************************************************
 * Function Name  :  initULHarq
 * Inputs         :  numOfCells
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function inits the UL HARQ Timer node values.
 ************************************************************************/
/*CA Changes start */
 void initULHarq(UInt8 numOfCells)
{
    /*
        We INIT the UL HARQ Timer array fields , at first.
    */ 
    initULHarqTimerArray(numOfCells);

}
/*CA Changes end */
/************************************************************************
 * Function Name  :  initULHarqTimerArray
 * Inputs         :  numOfCells
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function inits the UL HARQ Timer Array by initializing the values.
 ************************************************************************/

/*CA Changes start  */
 void initULHarqTimerArray(UInt8 numOfCells)
{
    UInt8 i = 0;
    UInt8 j = 0;
    UInt8 k = 0;    
    UInt8 cellCount = 0;    
    ULHarqTimerEntityList* ulHarqEntityTimerList_p = PNULL;
    ULHarqTimerNode* ulharqTimerNode_p = PNULL;
	LTE_MAC_UT_LOG(LOG_DETAIL, LOG_UL_HARQ,"[%s] Entry %d\n",__ifunc__,getCurrentTick());
    
    for (cellCount = 0; cellCount < numOfCells; cellCount++ )
    {    
       for (k = MAX_UL_HARQ_TIMER; k; k-- )
       {    
           for (i = MAX_SUB_FRAME; i; i-- )
           { 
               ulHarqEntityTimerList_p = &ulHarqTimerEntityList_g[cellCount][k-1][i-1];
            for (j=MAX_UL_UE_SCHEDULED; j; j--)
            {
                ulharqTimerNode_p = &ulHarqEntityTimerList_p->ulHarqTimerNodeArr[j-1];
                ulharqTimerNode_p->harqProcessId = INVALID_HARQ_ID;
                ulharqTimerNode_p->isValid = HARQ_TIMER_NODE_INVALID;
                ulharqTimerNode_p->ueIndex = INVALID_UE_ID;
                ulharqTimerNode_p->ulHarqExpiredSubframe = MAX_SUB_FRAME;
                ulharqTimerNode_p->ulHarqExpiredSysframe = MAX_SFN_VALUE;
            }
            ulHarqEntityTimerList_p->count = 0;
        }
    }
   }       
    LTE_MAC_UT_LOG(LOG_INFO,LOG_UL_HARQ,"[%s] Exit CTTI %d\n",__func__,getCurrentTick());
}
/*CA Changes end  */
/*SPR_3305_FIX_START*/
/************************************************************************
 * Function Name  :  initULHarqForMsg3
 * Inputs         :  numOfCells
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function inits the UL HARQ Timer node values.
 ************************************************************************/
/*CA Changes start */
 void initULHarqForMsg3(UInt8 numOfCells)
{
    /*
        We INIT the UL HARQ Timer array fields , at first.
    */ 
    initULHarqTimerArrayForMsg3(numOfCells);

}
/*CA Changes end */
/************************************************************************
 * Function Name  :  initULHarqTimerArrayForMsg3
 * Inputs         :  numOfCells
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function inits the UL HARQ Timer Array by initializing the values
 ************************************************************************/
/*CA Changes start  */
 void initULHarqTimerArrayForMsg3(UInt8 numOfCells)
{
    UInt8 i = 0;
    UInt8 j = 0;
    UInt8 k = 0;
    UInt8 cellCount = 0;
    ULHarqTimerEntityForMsg3List* ulHarqEntityTimerForMsg3List_p = PNULL;
    ULHarqTimerForMsg3Node* ulharqTimerForMsg3Node_p = PNULL;
    for (cellCount = 0; cellCount < numOfCells; cellCount++ )
    {    
    for (k = MAX_UL_HARQ_TIMER; k; k-- )
    {    
        for (i = MAX_SUB_FRAME; i; i-- )
        {
                ulHarqEntityTimerForMsg3List_p = &ulHarqTimerEntityForMsg3List_g[cellCount][k-1][i-1];
            for (j=MAX_UL_UE_SCHEDULED; j; j--)
            {
                ulharqTimerForMsg3Node_p = &ulHarqEntityTimerForMsg3List_p->ulHarqForMsg3TimerNodeArr[j-1];
                ulharqTimerForMsg3Node_p->harqProcessId = INVALID_HARQ_ID;
                ulharqTimerForMsg3Node_p->isValid = HARQ_TIMER_NODE_INVALID;
                ulharqTimerForMsg3Node_p->ulHarqExpiredSubframe = MAX_SUB_FRAME;
                ulharqTimerForMsg3Node_p->ulHarqExpiredSysframe = MAX_SFN_VALUE;
                ulharqTimerForMsg3Node_p->ueIndex = INVALID_UE_ID;
            }
            ulHarqEntityTimerForMsg3List_p->count = 0;
        }
    }
}
}
/*SPR_3305_FIX_END*/

/************************************************************************
 * Function Name  :  expireULHarqTimerNodeForTickMiss
 * Inputs         :  loopOfNumOfSubFrames
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function will expire the HARQ Timer node in case of tick overshoot.
 ************************************************************************/
void expireULHarqTimerNodeForTickMiss(UInt8 loopOfNumOfSubFrames,
                                           /*CA Changes start  */
                                           InternalCellIndex  internalCellIndex)
                                           /*CA Changes end  */
{
    UInt8 counter = 0;
    UInt8 sysFrameIndexCounter = 0;
    UInt16 ulHarqEntityTimerArrCount = 0;
    UInt16 ueIndex = INVALID_UE_ID;
    ULUEContextInfo* ulUEContextInfo_p = PNULL;
    ULUEContextInfo* ulUeContextInfoArrayAccess_p = PNULL;
    ULUEContext*  ulUeContext_p = PNULL;        
    ULHarqInfo* ulHarqProcess_p = PNULL;
    ULHarqTimerEntityList* ulHarqEntityTimerList_p = PNULL;
    ULHarqTimerNode* ulharqTimerNode_p = PNULL;

    ulUeContextInfoArrayAccess_p = &ulUECtxInfoArr_g[0];

    for (counter = 0; counter < loopOfNumOfSubFrames; counter++)
    {
        for(sysFrameIndexCounter = 0;sysFrameIndexCounter < MAX_UL_HARQ_TIMER;sysFrameIndexCounter++)
        {
         ulHarqEntityTimerList_p = &ulHarqTimerEntityList_g[internalCellIndex][sysFrameIndexCounter][counter];    
            ulHarqEntityTimerArrCount = ulHarqEntityTimerList_p->count;
            while (ulHarqEntityTimerArrCount--)
            {
                ulharqTimerNode_p = \
                                    &(ulHarqEntityTimerList_p->ulHarqTimerNodeArr[ulHarqEntityTimerArrCount]);    
                if (ulharqTimerNode_p->isValid)
                {
                    ueIndex = ulharqTimerNode_p->ueIndex;
                    ulUEContextInfo_p = &ulUeContextInfoArrayAccess_p[ueIndex];

                    if (!ulUEContextInfo_p->pendingDeleteFlag)
                    {
                        ulUeContext_p = ulUEContextInfo_p->ulUEContext_p;
                        ulHarqProcess_p = \
                                          &(ulUeContext_p->ulresInfo[ulharqTimerNode_p->harqProcessId]);
                        /* SPR# 3860 Changes Starts */
                        /* SPR 16916 FIX START */
#ifdef KPI_STATS
                        freeULHarqProcess(ulHarqProcess_p,ulUeContext_p->ueIndex,internalCellIndex);
#else
                        freeULHarqProcess(ulHarqProcess_p);
#endif
                        /* SPR 16916 FIX END */
#ifdef FDD_CONFIG                    
                        updateTTIHarqMap(ulharqTimerNode_p->harqProcessId,
                                ulHarqProcess_p,
                                ulUeContext_p->ttiHarqMap);
#endif
                        /* SPR# 3860 Changes Ends */
                    }
                    ulharqTimerNode_p->harqProcessId = INVALID_HARQ_ID;
                    ulharqTimerNode_p->isValid = HARQ_TIMER_NODE_INVALID; 
                    ulharqTimerNode_p->ueIndex = INVALID_UE_ID;
                    ulharqTimerNode_p->ulHarqExpiredSubframe = MAX_SUB_FRAME;
                }
            }
            ulHarqEntityTimerList_p->count = 0;
        }
    }
 
}
#endif
