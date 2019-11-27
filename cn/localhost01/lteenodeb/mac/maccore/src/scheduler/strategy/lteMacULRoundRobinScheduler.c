/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacULRoundRobinScheduler.c,v 1.2.6.1.4.2.2.1 2010/09/21 15:46:21 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file includes the functionality of distributing the
 Uplink RBs in round robin fashion.The number of RBs
 distributed to particular UE will be depending upon the
 predefined fixed values.
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacULRoundRobinScheduler.c,v $
 * Revision 1.2.6.1.4.2.2.1  2010/09/21 15:46:21  gur20491
 * FAPI changes
 *
 * Revision 1.2.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.21  2009/11/11 14:04:32  gur11912
 * fixed when nonZeroBsrListStartRoundRobin_p is PNULL
 *
 * Revision 1.20  2009/11/10 13:02:54  gur11912
 * modified for warning UE not present in non zero lists
 *
 * Revision 1.19  2009/10/14 08:17:38  gur11912
 * optimization changes
 *
 * Revision 1.18  2009/07/24 12:39:34  gur20052
 * Splint Warnings are removed
 *
 * Revision 1.17  2009/07/20 17:45:04  gur20052
 * UT Defects are Fixed
 *
 * Revision 1.16  2009/07/13 14:06:30  gur20052
 * UT Bugs Fixed
 *
 * Revision 1.14  2009/07/10 13:50:38  gur20052
 * MACModuleLogDetail_g is replaced with the Module Specific STRINGS
 *
 * Revision 1.13  2009/07/07 05:32:13  gur20052
 * UT Defects are fixed
 *
 * Revision 1.12  2009/06/23 16:55:42  gur20052
 * Review comments are incorporated
 *
 * Revision 1.10  2009/06/05 16:56:28  gur20052
 * Incorporated the reveiw comments
 *
 * Revision 1.9  2009/06/05 16:20:23  gur20052
 * Done the Changes for the Enhancements
 *
 * Revision 1.8  2009/06/04 07:39:50  gur20052
 * Review Comments are incorporated
 *
 * Revision 1.7  2009/06/02 07:19:08  gur20052
 * Updated during comments incorporation and dispostion
 *
 * Revision 1.6  2009/05/28 11:33:01  gur20052
 * The changes for the UE contexts are done
 *
* Revision 1.5  2009/05/26 10:16:04  gur20052
* Updated to incorporated UE Context Changes
*
* Revision 1.4  2009/05/25 16:20:01  gur20052
* Updated the version to support the UL Fair and Round Robin Scheduler
*
* Revision 1.3  2009/05/21 17:22:46  gur20052
* Updated the Basic Framework of UL Strategy
*
* Revision 1.2  2009/05/11 06:18:32  gur20052
* Removed compilation errors
*
* Revision 1.1  2009/04/30 17:13:28  gur20052
* Added inital version for Uplink Round Robin Scheduler Algorithm
*
*
*****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/

#include "lteMacCellConfMgr.h"
#include "lteMacULSchedulerStrategy.h"
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
#endif
// SPR 3486  - START
#include "lteMacULPowerControlData.h"
// SPR 3486  - END
/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* ICIC changes start */
/*CA Changes start  */
 MacRetType putEntryInULCellEdgePriorityQueue(UInt16 ueIndex, InternalCellIndex internalCellIndex);
/*CA Changes end  */
MacRetType processULPriorityCellEdgeUserList(
             UInt32 cePriorityListCount,
             ULUserListRR *ulUserList_p,
             /* SPR 15909 fix start */
             tickType_t currentGlobalTTITickCount,
             /* SPR 15909 fix end */
             UInt8  * maxUEsToBeScheduledUL_p,
             /* SPR 21068 start */
             tickType_t referenceFlag
             /* SPR 21068 end   */ 
#ifdef FDD_CONFIG
             ,UInt32 ulDelay
#endif
             /* TDD Config 0 Changes Start */
              /*CA Changes start  */
             ,InternalCellIndex internalCellIndex
             /*CA Changes end  */
#ifdef TDD_CONFIG
             ,UInt8 ulSubFrameNum
#endif
             /* TDD Config 0 Changes End */
           );


/* ICIC changes end */
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/*A pointer to nodeAnchor of the UE in ueNonZeroQueueLoadList*/
LTE_LIST_NODE * nonZeroBsrListStartRoundRobin_p[MAX_NUM_CELL] = {PNULL};

/* SPR 1010 changes start */
extern UInt8  maxpowerOfTwoThreeFive_g[MAX_NUM_RBS + 1];
extern UInt8  minpowerOfTwoThreeFive_g[MAX_NUM_RBS + 1];
//
/* SPR 1010 changes end */

#ifdef DL_UL_SPLIT
/*CA Changes start  */
extern MacRetType putEntryInULSchedulerNewTxQueueUsingICC(ULStrategyTxNode *ulStrategyTxNode_p, InternalCellIndex cellIndex);
/*CA Changes end  */
#endif


/* ICIC changes start */
/* A List to maintain pending Cell edge UE's */
/*CA Changes start  */
LTE_SQUEUE* ulCellEdgeUserPriorityQueue_gp[MAX_NUM_CELL];
SInt32 unusedULRb_g[MAX_NUM_CELL];
/*CA Changes end  */
/* ICIC changes end */

/* +DYNAMIC_ICIC */
extern ULUESchedulerInfo* ulScheduleInfo_gp[MAX_NUM_CELL];
/* -DYNAMIC_ICIC */
extern UInt8 checkSpsOccasionTti( ULUEContext *ulCntxt_p,
                             UInt8  harqIndex);
extern UInt8 checkSpsActTti( ULUEContext *ulCntxt_p,
                             UInt8  harqIndex);
#ifdef FDD_CONFIG
/*CA Changes start  */
extern UInt32 availableRBForTtiBundlingPerTick_g[MAX_NUM_CELL];
/*CA Changes end  */
#endif

/* EICIC +*/
extern UInt8 ueTypeToScheduleUL_g[MAX_NUM_CELL];
/* EICIC -*/

/* ICIC Change Start */
/*****************************************************************************
 * Function Name  : processULSchedulableList
 * Inputs         : ulUserList - CC or CE User list
 *                  userLocationType - CC or CE user type.
 *                  availableRB_p - pointer to current available RBs.
 *                  allocatedRBFromRoundRobin- The number of RBs allocated as
 *                     per RR strategy.
 *                  currentGlobalTTITickCount - Global TTI Tick Count.
 *                  ulDelay - Number of ticks UL is getting processed in Advance.
 *                  internalCellIndex - cell index used at MAC.
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is
 *                     being given.
 * Outputs        : Distribution of RBs among UEs on the basis of Round Robin
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function works on Cell Edge and Cell Center user list
 *                  and allocate resources to user by calling function
 *                  processULRoundRobinNewDataGeneral.
 *****************************************************************************/

MacRetType processULSchedulableList(ULUserListRR *ulUserList,
        /* +- SPR 17777 */
        UInt32 *numberOfAvailableRBs_p,
        UInt32 allocatedRBFromRoundRobin,
        /* +- SPR 17777 */
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
#ifdef FDD_CONFIG
        ,UInt32 ulDelay
#endif
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
        
{
    MacRetType retType = MAC_SUCCESS;
    ULUEContextInfo *uplinkContextInfo_p = PNULL;
    ULUEContext *ueULContext_p = PNULL;

    UInt16 ueCount = ulUserList->count;
    UInt32 strictAllocRBFlag = FALSE;
    UInt16 ueIndex = 0 ;
    /* FDD_Warning_Fix */
#ifdef TDD_CONFIG
    UInt8 ulHarqProcessId  = 0;
#endif    
    /* FDD_Warning_Fix */
    UInt32 usedRB          = 0;
    UInt32 minDataSize     = 0;
    UInt32 maxDataSize     = 0;
    UInt32 rbAllocated     = 0;
    UInt16 index           = 0;
    /* ATB Changes Start */
    UInt8 mcsVal           = 0;
    /* ATB Changes End */

    
    while(index < ueCount)
    {
        if (!(*numberOfAvailableRBs_p))
        {
            break;
        }

        ueIndex =  ulUserList->ulSchedulableUENode[index].ueIndex; 
        /* FDD_Warning_Fix */
#ifdef TDD_CONFIG
        ulHarqProcessId = ulUserList->ulSchedulableUENode[index].harqProcessId;
#endif
        /* FDD_Warning_Fix */
        uplinkContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
        ueULContext_p = uplinkContextInfo_p->ulUEContext_p;

        /*Calling the function to Validate the RBs and then enqueue the
          UE with all these updated fields to PDCCH module */
        /* ULA_CHG */
        rbAllocated = allocatedRBFromRoundRobin + unusedULRb_g[internalCellIndex];
        usedRB = validateUplinkRB(ueULContext_p ,
                rbAllocated,
                *numberOfAvailableRBs_p, &strictAllocRBFlag,&minDataSize,&maxDataSize,
                 /* ATB Changes Start */
                 &mcsVal,
                /* SPR 13532 Fix Start */
#ifdef FDD_CONFIG
                 (currentGlobalTTITickCount + ulDelay)
#else
                 /*SPR 16417 Changes Start*/
                 currentGlobalTTITickCount,
                 ulSubFrameNum
                 /*SPR 16417 Changes End*/
#endif
                 /* ATB Changes End */
                 /* SPR 13532 Fix End */
                 ,internalCellIndex);

        if(rbAllocated > usedRB)
        {
            LOG_UT(MAC_UL_RR_SUBTRACT_USED_RB_FROM_ALLOCATED_RB,LOGDEBUG,MAC_DL_Strategy,
                   getCurrentTick(),
                   usedRB,rbAllocated,
                   unusedULRb_g[internalCellIndex],
                   (rbAllocated - usedRB),LINE_NUMBER,
                   DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                   FUNCTION_NAME,"");
            unusedULRb_g[internalCellIndex] = rbAllocated - usedRB;
        }
        else
        {
            LOG_UT(MAC_UL_RR_MAKING_UNUSED_RB_ZERO,LOGDEBUG,MAC_DL_Strategy,
                   getCurrentTick(),
                   usedRB,rbAllocated,
                   DEFAULT_INT_VALUE,
                   DEFAULT_INT_VALUE,LINE_NUMBER,
                   DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                   FUNCTION_NAME,"");
            unusedULRb_g[internalCellIndex] = 0;
        }
#ifdef FDD_CONFIG
        if (ueULContext_p->ttiBundlingEnabled)
        {    
            if ( usedRB > MAX_TTI_BUNDLE_RB ) 
            {
                usedRB = MAX_TTI_BUNDLE_RB;
            }
            if ( mcsVal > MAX_TTI_BUNDLE_MCS ) 
            {
                mcsVal = MAX_TTI_BUNDLE_MCS;
            }
        	/* + coverity 72765 */
			if ( usedRB > 0 )
			{
            	maxDataSize = determineUlTBSize(mcsVal,usedRB);
			}
			else
			{
				return MAC_FAILURE;
			}
        	/* - coverity 72765 */
            minDataSize = determineUlTBSize(mcsVal,MIN_TTI_BUNDLE_RB);
            /*CA Changes start  */
            if ( (availableRBForTtiBundlingPerTick_g[internalCellIndex] < usedRB ) 
            /*CA Changes end  */
                    || (MAC_FAILURE == processMeasGapValidation(
                            ueULContext_p,currentGlobalTTITickCount,ulDelay )))
            {
                index++;
                continue;
            }
            /*CA Changes start  */
            availableRBForTtiBundlingPerTick_g[internalCellIndex] -= usedRB;
            /*CA Changes end  */
            LTE_MAC_UT_LOG(LOG_INFO,UL_RR_STRATEGY,"[UL-Strategy]"\
                    "[%s][%d]:TTIB UE ID %d scheduled RB:%d MCS:%d \n",
                    __func__,getCurrentTick(),ueIndex,usedRB,mcsVal);
        }
#endif        

        /* ULA_CHG */
        if (usedRB)
        {
            /* ULA_CHG */
#ifdef TDD_CONFIG
            assignULRBsAndSendToPDCCHRoundRobin(ueIndex,usedRB,
                    numberOfAvailableRBs_p,currentGlobalTTITickCount,
                    strictAllocRBFlag, ulHarqProcessId,
                    minDataSize,maxDataSize,
                    /* ATB Changes Start */
                    mcsVal
                    /* ATB Changes End */
                    /*CA Changes start  */
                    ,internalCellIndex
                    /*CA Changes end  */
                    /* TDD Config 0 Changes Start */
                    ,ulSubFrameNum
                    /* TDD Config 0 Changes End */
                    );
#elif FDD_CONFIG
            assignULRBsAndSendToPDCCHRoundRobin(ueIndex,usedRB,
                    numberOfAvailableRBs_p,currentGlobalTTITickCount,
                    strictAllocRBFlag,minDataSize,maxDataSize,
                    /* ATB Changes Start */
                    mcsVal
                    /* ATB Changes End */
                    ,ueULContext_p->ttiBundlingEnabled
                    ,internalCellIndex
                    );
#endif
        }
        else
        {
            /*LTE_MAC_UT_LOG(LOG_INFO,UL_RR_STRATEGY,
              "[UL-Strategy][%s] used RB are Zero ......\n",
              __func__);*/
        }
        index++;
    }
    return retType;


}

/* ICIC Change End */

/* cyclomatic changes */
/*****************************************************************************
 * Function Name  : getNextULRRUEToSchedule
 * Inputs         : node_p - pointer to the current node,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : node_p - pointer to the next node
 * Returns        : node_p
 * Description    : It retuns the pointer to the next node.
 *****************************************************************************/
static  LTE_LIST_NODE * getNextULRRUEToSchedule
(
  LTE_LIST_NODE *node_p,
  /*CA Changes start */
  InternalCellIndex internalCellIndex
  /*CA Changes end */
)
{
     node_p = getNextListNode(node_p);
     if(NULL == node_p)
     {
         /*CA Changes start  */
         node_p = getListNode(nonZeroBsrList_gp[internalCellIndex],0);
         /*CA Changes end  */
     }
     return node_p;
}
    /*UL Force Grant Changes Start */

/*****************************************************************************
 * Function Name  : checkAndProvideForceGrant
 * Inputs         : ueULContext_p - UL UE context
 * Outputs        : *deleteUEFlag - 
 *                  1 - if UE needs to be deleted from non zero BSR queue.
 *                  0 - if UE delete is not required.
 *                  internalCellIndex - internal cell Index
 * Returns        : void
 * Description    : This function checks and provide false grant to UE. It 
 *                  will only be called when bsrNetTotal is zero.
 *****************************************************************************/
 /*SPR 21494 changes Start */
void checkAndProvideForceGrant( UInt8 *deleteUEFlag,
        ULUEContext *ueULContext_p, InternalCellIndex internalCellIndex )
{

    /*SPR 3070 changes start */
    /* SPR 19310 + */
    CellConfigParams *cellParams_p = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /* SPR 21777 fix start */
    /* SPR 19310 - */
    /* Force Grant Chg */
    if (ueULContext_p->numOfForceGrant)
    {
        ueULContext_p->numOfForceGrant--;
        ueULContext_p->bsrNetTotal = cellParams_p->sizeEUL;
    }

    /* Force Grant Chg */
    if (!ueULContext_p->bsrNetTotal)
    {
        /* SPR 21777 fix end */
        *deleteUEFlag = TRUE;
        return;
    }
    return;
}
 /*UL Force Grant Changes End */
/*SPR 21494 changes End */
/*****************************************************************************
 * Function Name : validateUEForULRBDistribution 
 * Inputs        : ueIndex - UE identifier
 *                 currentGlobalTTITickCount - current global TTI
 *                 referenceFlag - flag to check wheather UE is considered already
 *                 numberOfAvailableRBs_p - pointer to  number of avaiable RB's
 *                 deleteUEFlag - flag to set weather UE deleting in progess
 *                 ulHarqProcessId_p - Pointer to UL UL Harq Process ID to store
 *                 ulDelay - delay in uplink path
 *                 internalCellIndex - cell index used at MAC.
 *                 ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs       : deleteUEFlag - flag to set weather UE deleting in progess
 *                 ulHarqProcessId_p - Pointer to UL UL Harq Process ID to store
 * Returns       : MAC_SUCCESS/MAC_FAILURE
 * Description   : This API will be validating the UE for allocation of RB in 
 *                 UL in Round Robin manner. If UE can be scheduled then it 
 *                 return the free harqprocess for scheduling, if available.
*****************************************************************************/
static  MacRetType validateUEForULRBDistribution(
    UInt16 ueIndex,
    /* SPR 15909 fix start */
    tickType_t currentGlobalTTITickCount,
    /* SPR 15909 fix end */
    /* SPR 21068 start */    
    tickType_t referenceFlag,
    /* SPR 21068 end */
    UInt32 *numberOfAvailableRBs_p,
    UInt8  *deleteUEFlag,
    UInt8  *ulHarqProcessId_p
#ifdef FDD_CONFIG
    ,UInt8 ulDelay
#endif
    /*CA Changes start  */
    ,InternalCellIndex internalCellIndex
    /*CA Changes end  */
    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
    ,UInt8 ulSubFrameNum
#endif
    /* TDD Config 0 Changes End */
    )
{
    DLUEContextInfo   *ueDLContextInfo_p = PNULL;
    ULUEContextInfo   *tempUplinkContextInfo_p = PNULL;
    ULUEContext       *ueULContext_p = PNULL;
    UInt8             userLocationType = CC_USER;
    UInt8             ulHarqProcessId = 255;
#ifdef FDD_CONFIG 
    UInt8 ttiMapId1 = TTI_MAP_FREE;
    UInt8 ttiMapId2= TTI_MAP_FREE;
#endif    

    ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
    tempUplinkContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
    ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];

    if (tempUplinkContextInfo_p->pendingDeleteFlag )
    {
        /* SPR 15408 Fix Start */
        /* Code Removed*/
        /* SPR 15408 Fix End */
        /* + coverity 32778 */
        return MAC_FAILURE;
        /* - coverity 32778 */
    }
    ueULContext_p = tempUplinkContextInfo_p->ulUEContext_p;
    userLocationType = ueULContext_p->userLocationType;
    if( SPS_SCHEDULING == ueULContext_p->schType )
    {
        /* + coverity 32778 */
        return MAC_FAILURE;
        /* - coverity 32778 */
    }
    if(UL_SYNC_LOSS_ON == checkUlSyncLossForUE(ueDLContextInfo_p->dlUEContext_p,
                                         currentGlobalTTITickCount,
                                         internalCellIndex))
    {
        /* + coverity 32778 */
        return MAC_FAILURE;
        /* - coverity 32778 */
    }
#ifdef HD_FDD_CONFIG
    /** check if this UE is HD FDD UE **/
    if(ueULContext_p->isHDFddFlag)
    {
        /* Check Ul can be scheduled for HD UE in schedule map of current tick*/
        if (!CAN_SCHEDULE_UL_FOR_HD_UE(currentGlobalTTITickCount + PHY_DL_DELAY, ueIndex , hdUeContextInfo_g.hdfddUeSchedMap ))
        {
            /* Increment the statistics counter UE denied UL resources due to
             * DL clash in the same TTI of HD FDD UE*/
            UPDATE_UE_DENIED_UL_RESOURCE(ueIndex);
            /* + coverity 32775 */
            return MAC_FAILURE;
            /* - coverity 32775 */
        }
    }
#endif
    /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
    if (ueULContext_p->isAlreadyConsideredFlag == referenceFlag)
#elif TDD_CONFIG
    if (ueULContext_p->isAlreadyConsideredFlag[ulSubFrameNum] 
                == referenceFlag)
#endif
    /* TDD Config 0 Changes End */
    {
        /* + coverity 32777 */
        return MAC_FAILURE;
        /* - coverity 32777 */
    }
    if (!(ueULContext_p->bsrNetTotal))
    {
        /*UL Force Grant Changes Start */
	/* SPR 3070 Changes start */
	checkAndProvideForceGrant(deleteUEFlag,ueULContext_p, internalCellIndex);	
	/* SPR 3070 Changes stop */
        if(*deleteUEFlag == TRUE)
        {
            return MAC_FAILURE;
        }
         /*UL Force Grant Changes End */


        *deleteUEFlag = TRUE;
        /* + coverity 32778 */
        return MAC_FAILURE;
        /* - coverity 32778 */

    }
    if ((userLocationType == CE_USER) && 
            (numberOfAvailableRBs_p[CE_USER] == ZERO || (!maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex])))
    {
        return MAC_FAILURE;
    }
#ifdef TDD_CONFIG
    /* TDD Config 0 and 6 Changes Start */
    /* SPR 23393 Changes Start */
    UInt32 subframeNum = 0;
    UInt32 sysFrameNum = 0;
    GET_SFN_SF_FROM_TTI(currentGlobalTTITickCount, sysFrameNum, subframeNum)
    /* SPR 23393 Changes End */
        /* +- SPR 17777 */
    LTE_GCC_UNUSED_PARAM(sysFrameNum)
    /* SPR 11257 Changes Start*/    
    ulHarqProcessId = getFreeULHarqId(ueULContext_p, subframeNum,
				internalCellIndex,ulSubFrameNum, currentGlobalTTITickCount);
    /* +- SPR 17777 */
    /* SPR 11257 Changes End*/
    /* TDD Config 0 and 6 Changes End */
#elif FDD_CONFIG
    /* TTIB_Code Start */
    ulHarqProcessId = MODULO_EIGHT(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
    if ( ueULContext_p->ttiBundlingEnabled )
    {
        ulHarqProcessId = GET_TTIB_HARQ_ID( currentGlobalTTITickCount +ulDelay + FDD_HARQ_OFFSET );    
    }
    ttiMapId1 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
    ttiMapId2 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET + MAX_HARQ_PROCESS_NB );
    /* TTIB_Code End */
#endif

#ifdef TDD_CONFIG
    if (BUSY_ALL_HARQ == ulHarqProcessId) 
#elif FDD_CONFIG
        /* TTIB_Code Start */
        if((HARQ_PROCESS_FREE != ueULContext_p->ulresInfo[ulHarqProcessId].harqStatus)
                ||(ueULContext_p->ttiBundlingEnabled &&
                    (MAC_FAILURE == processTtiBHarqCheck(currentGlobalTTITickCount,ulDelay,ueULContext_p)))
                ||(!(ueULContext_p->ttiBundlingEnabled)&&
                    ((TTI_MAP_FREE != ueULContext_p->ttiHarqMap[ttiMapId1]) ||
                     ( TTI_MAP_FREE != ueULContext_p->ttiHarqMap[ttiMapId2])))
                /* + SPS_TDD_Changes */
                /*This code has been commented as we will not block harq process to be used for SPS only */
                /*SPR 7944 START*/
                /*
                || checkSpsOccasionTti(ueULContext_p,ulHarqProcessId)
                || checkSpsActTti(ueULContext_p,ulHarqProcessId)
                */
                /*SPR 7944 END*/
                /* - SPS_TDD_Changes */
          )
            /* TTIB_Code End */
#endif
        {
            /* + coverity 32776 */
            return MAC_FAILURE;
            /* - coverity 32776 */
        }
    *ulHarqProcessId_p = ulHarqProcessId;
    /* + coverity 32779 */
    return MAC_SUCCESS;
    /* - coverity 32779 */
}

/*****************************************************************************
 * Function Name : processNonZeroBsrUEsInRRManner
 * Inputs        : maxUEsToBeScheduledUL_p - Maximum number of UE's to be
 *                    Scheduled in Uplink
 *                 numberOfAvailableRBs_p - pointer to  number of avaiable RB's
 *                 currentGlobalTTITickCount - current global TTI
 *                 referenceFlag - flag to check wheather UE is considered already
 *                 ulUserList - pointer to UL User list in RR
 *                 ulDelay - delay in uplink path
 *                 internalCellIndex - cell index used at MAC.
 *                 ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs       : ulUserList - pointer to UL User list in RR 
 * Returns       : None 
 * Description   : This API will be used in distributing the uplink RB in 
                   round robin fashion. The number of RBs distributed to 
                   particular UE will be fixed to a maximum value and then 
                   depending upon the the available queue load those 
                   allocated RBs are validated
*****************************************************************************/
static  void processNonZeroBsrUEsInRRManner(
     UInt8 *maxUEsToBeScheduledUL_p,
     /*SPR 21068 start */
     tickType_t referenceFlag,
     /*SPR 21068 end   */
     UInt32 *numberOfAvailableRBs_p,
     /* SPR 15909 fix start */
     tickType_t currentGlobalTTITickCount,
     /* SPR 15909 fix end */
     ULUserListRR *ulUserList
#ifdef FDD_CONFIG
     ,UInt8 ulDelay
#endif
     /*CA Changes start  */
     ,InternalCellIndex internalCellIndex
     /*CA Changes end  */
     /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
     ,UInt8 ulSubFrameNum
#endif
     /* TDD Config 0 Changes End */
     )
{
    LTE_LIST_NODE     *node_p = PNULL, *newNode_p=NULL;
    UInt16            ueIndex;
    MacRetType        ret_val = MAC_FAILURE;
    UInt8             deleteUEFlag = FALSE;
    ULUEContextInfo   *tempUplinkContextInfo_p = PNULL;
    ULUEContext       *ueULContext_p = PNULL;
    UInt8             userLocationType = CC_USER;
	/*SPR 21769 Changes Start*/
    UInt16             ueCount =0;
	/*SPR 21769 Changes End*/
    UInt8             ulHarqProcessId = 255;
    /* SPR 14765 Fix Start */
    UInt32 	      iterationCounter = 0;
    UInt32            listCnt        = 0;
    /* SPR 14765 Fix End */

    if (nonZeroBsrListStartRoundRobin_p[internalCellIndex])
    {
        LTE_MAC_UT_LOG(LOG_INFO,UL_RR_STRATEGY,
                "[UL-Strategy][%s]Round Robin Pointer is NOT NULL,Hence pointer"
                " to start is nonZeroBsrListStartRoundRobin_p ",__func__);
        /*SPR 7470 Fix start */
        node_p = nonZeroBsrListStartRoundRobin_p[internalCellIndex];
        /*SPR 7470 Fix end */
    }
    else
    {
        LTE_MAC_UT_LOG(LOG_INFO,UL_RR_STRATEGY,
                "[UL-Strategy][%s]Round Robin Pointer is NULL,Hence pointer"
                " to start is First Node in Non Zero BSR LIST",__func__);

        /*CA Changes start  */
        node_p = getListNode(nonZeroBsrList_gp[internalCellIndex],0);
        /*CA Changes end  */
        nonZeroBsrListStartRoundRobin_p[internalCellIndex] = node_p;
    }
    
    /* SPR 14765 Fix Start */
    listCnt = listCount((nonZeroBsrList_gp[internalCellIndex])); 
    while ((*maxUEsToBeScheduledUL_p >0 ) && (listCnt >= iterationCounter))
    {
        iterationCounter++;
        /* SPR 14765 Fix End */
        LTE_MAC_UT_LOG(LOG_INFO,UL_RR_STRATEGY,
                "[UL-Strategy][%s]Total number of available RBs :%d, Cell Edge available RBs :%d"
                " maxUEsToBeScheduledUL=%d "
                " currentGlobalTTITickCount %d *referenceFlag %d \n",
                __func__,numberOfAvailableRBs_p[CC_CE_REGION],numberOfAvailableRBs_p[CE_REGION],  *maxUEsToBeScheduledUL_p,
                currentGlobalTTITickCount,referenceFlag );

        /* + Coverity 24591 */
        if( PNULL == node_p )
        {
            break;
        }
        /* - Coverity 24591 */
        if(newNode_p == nonZeroBsrListStartRoundRobin_p[internalCellIndex] && node_p != NULL)
        {
            node_p = newNode_p;    
            break;
        }

        ueIndex = ((nonZeroBsrListNode*)node_p)->ueIndex;
        tempUplinkContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
        ueULContext_p = tempUplinkContextInfo_p->ulUEContext_p;
        /* SPR 17827 Start */
        if ( dlUECtxInfoArr_g[ueIndex].dlUEContext_p == PNULL || ulUECtxInfoArr_g[ueIndex].ulUEContext_p == PNULL )
        {
            listDeleteNode(nonZeroBsrList_gp[internalCellIndex], node_p);
            freeMemPool((void *)node_p);
            node_p = PNULL;
            break;
        }
        /* SPR 17827 End */

if (!tempUplinkContextInfo_p->pendingDeleteFlag)
        {
	        /* EICIC +*/       
	            
			if ((VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (NON_VICTIM_UE == ueULContext_p->userType))
			{              
	      			 newNode_p = getNextULRRUEToSchedule(node_p, internalCellIndex);
	      			 node_p= newNode_p;
                              deleteUEFlag = FALSE;

	               continue;
			}
			else if ((NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) && (VICTIM_UE == ueULContext_p->userType))
			{    
	      			 newNode_p = getNextULRRUEToSchedule(node_p, internalCellIndex);
	      			 node_p= newNode_p;
                             deleteUEFlag = FALSE;

	               continue;
			}
			
		/* EICIC -*/

        }
        ret_val = validateUEForULRBDistribution(
                                 ueIndex,
                                 currentGlobalTTITickCount,
                                 referenceFlag,
                                 numberOfAvailableRBs_p,
                                 &deleteUEFlag,
                                 &ulHarqProcessId
#ifdef FDD_CONFIG
                                 ,ulDelay
#endif
                                 /*CA Changes start  */
                                 ,internalCellIndex
                                 /*CA Changes end  */
                                 /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                                 ,ulSubFrameNum
#endif
                                 /* TDD Config 0 Changes End */
                                 );

        if(ret_val)
        {
             /* Schedule this UE */
             /* ICIC changes end */
             /* ICIC changes start*/
              userLocationType = ueULContext_p->userLocationType;
             ueCount = ulUserList[userLocationType].count;
             ulUserList[userLocationType].ulSchedulableUENode[ueCount].ueIndex = ueIndex;
             ulUserList[userLocationType].ulSchedulableUENode[ueCount].harqProcessId = ulHarqProcessId;
             ulUserList[userLocationType].count++;
             /* ULA_CHG */
             /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
             ueULContext_p->isAlreadyConsideredFlag = referenceFlag;
#elif TDD_CONFIG
             ueULContext_p->isAlreadyConsideredFlag[ulSubFrameNum] = referenceFlag;
#endif
             /* TDD Config 0 Changes End */
             *maxUEsToBeScheduledUL_p = (*maxUEsToBeScheduledUL_p) - 1;
             /* ICIC Change Start */
             if (CE_USER == ueULContext_p->userLocationType)
             {
                 /*CA Changes start  */
                 maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]--;
                 /*CA Changes end  */
        }
             /* ICIC changes end */
        }
        newNode_p = getNextULRRUEToSchedule(node_p, internalCellIndex);
        if(deleteUEFlag)
        {
            /*delete the node from non ZERO BSR List and dont consider
              this UE goto next UE in the non Zero BSR LIST*/
		/* + Coverity 72913 */
		deleteEntryFromNonZeroBSRList(ueULContext_p, node_p, internalCellIndex);

        
        LOG_MAC_MSG(MAC_UE_DELETED_FROM_ACTIVE_LIST_IN_UL_LOG_ID
                ,LOGBRIEF, MAC_UL_Strategy, 
                getCurrentTick(), ueULContext_p->ueIndex, 
                ueULContext_p->bsrNetTotal,
                DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE, DEFAULT_FLOAT_VALUE, 
                DEFAULT_FLOAT_VALUE, __func__,"");

		
		/* - Coverity 72913 */
            node_p = PNULL;    
            /* SPR 9268 (Bug 557) fix start */
            if ( PNULL != tempUplinkContextInfo_p->ulUEContext_p  )
            {	
                tempUplinkContextInfo_p->ulUEContext_p->nonZeroBsrList_p = PNULL;
                /* SPR 9286 changes start */ 
                tempUplinkContextInfo_p->ulUEContext_p->nonZeroBsrListTempDeleted_p = PNULL; 
                /* SPR 9286 changes end */ 
            }	
            /* SPR 9268 (Bug 557) fix end */

	    /* SPR 10961 Fix Start */
            if (PNULL == nonZeroBsrListStartRoundRobin_p[internalCellIndex])
	    {
		break;
	    }  
	    /* SPR 10961 Fix End*/
            }	


        node_p= newNode_p;
        deleteUEFlag = FALSE;
    }
    nonZeroBsrListStartRoundRobin_p[internalCellIndex] = node_p;
}
#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name : processNonZeroBsrUEsInRRManner_dummy
 * Inputs        : maxUEsToBeScheduledUL_p - Maximum number of UE's to be
 *                    Scheduled in Uplink
 *                 numberOfAvailableRBs_p - pointer to  number of avaiable RB's
 *                 currentGlobalTTITickCount - current global TTI
 *                 referenceFlag - flag to check wheather UE is considered already
 *                 ulUserList - pointer to UL User list in RR
 *                 ulDelay - delay in uplink path
 *                 internalCellIndex - cell index used at MAC.
 *                 ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs       : ulUserList - pointer to UL User list in RR 
 * Returns       : None 
 * Description   : This calls processNonZeroBsrUEsInRRManner
                
*****************************************************************************/
void processNonZeroBsrUEsInRRManner_dummy(
     UInt8 *maxUEsToBeScheduledUL_p,
     /*SPR 21068 start */
     tickType_t referenceFlag,
     /*SPR 21068 end */
     UInt32 *numberOfAvailableRBs_p,
     /* SPR 15909 fix start */
     tickType_t currentGlobalTTITickCount,
     /* SPR 15909 fix end */
     ULUserListRR *ulUserList
     #ifdef FDD_CONFIG
     ,UInt8 ulDelay
     #endif
     /*CA Changes start  */
     ,InternalCellIndex internalCellIndex
     /*CA Changes end  */
     )
{
processNonZeroBsrUEsInRRManner(
     maxUEsToBeScheduledUL_p,
     referenceFlag,
     numberOfAvailableRBs_p,
     currentGlobalTTITickCount,
     ulUserList,
     #ifdef FDD_CONFIG
     ulDelay,
     #endif
     /*CA Changes start  */
     internalCellIndex
     /*CA Changes end  */
     );
}
#endif
/* cyclomatic changes */

/*****************************************************************************
 * Function Name  : ulRoundRobinScheduler
 * Inputs         : referenceFlag - Used for checking whether the US is 
 *                     considered already or not.If UE's rbsReservedForUE is equal 
 *                     to referenceFlag, that means the UE is already considered
 *                  numberOfAvailableRBs_p - pointer to current available RBs.
 *                  maxUEsToBeScheduledUL -  max UE to be scheduled in UL  
 *                  currentGlobalTTITickCount - Current Global Tick
 *                  ulDelay - delay in uplink path
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : Distribution of RB on round robin basis
 * Returns        : MAC_SUCCESS
 * Description    : This API will be used in distributing the Uplink RB in 
 *                  round robin fashion. The number of RBs distributed to 
 *                  particular UE will be fixed to a maximum value and then 
 *                  depending upon the the available BSR load those allocated 
 *                  RBs are validated
 *****************************************************************************/
/* SPR 2446 Fix Begins*/
UInt32 ulRoundRobinScheduler(
        /* SPR 21068 start */
        tickType_t referenceFlag,
        /* SPR 21068 end  */
        UInt32 *numberOfAvailableRBs_p,
        UInt8 maxUEsToBeScheduledUL,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount
        /* SPR 15909 fix end */
#ifdef FDD_CONFIG
        ,UInt8 ulDelay
#endif
        /*CA Changes start */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
#endif
        /* TDD Config 0 Changes End */
        )
/* SPR 2446 Fix Ends*/
{
    ULUEContext * ueULContext_p = PNULL;
    UInt32 ueIndex ;
    /* ICIC changes start */
    UInt32 maxULRBToBeAllocInRoundRobin  = 0;
    UInt32 maxCellEdgeRBToBeAllocInRoundRobin = 0;
    /* Stores List of Schedulable CC and CE users */
    ULUserListRR ulUserList[MAX_USER_LOCATION];
    UInt8 userLocationType = CC_USER;
    UInt32 tempAvailRB = 0;
    UInt32 numOfUeToSchedule = 0;
    UInt32 ueCount = 0;
    /* ICIC changes end */

    /*CA Changes start  */
    UInt32 tempTotalActiveUE    = listCount(nonZeroBsrList_gp[internalCellIndex]);
    /*CA Changes end  */
    // SPR 3486  - START
    UInt8 minRb = 0;
    // SPR 3486  - END

    /* ICIC changes start */
    UInt32 cePriorityListCount     = 0;
    /* +DYNAMIC_ICIC */
    UInt16 numOfUE = 0;
    /* -DYNAMIC_ICIC */

    /* SPR 11975 Fix start */
    if ( !maxUEsToBeScheduledUL )
    {
        return MAC_SUCCESS;
    }
    /* SPR 11975 Fix end */

    /* Initializing count to 0 for CC and CE user */
    ulUserList[CC_USER].count=0;
    ulUserList[CE_USER].count=0;
    /* Initializing unused RB for UL */
    unusedULRb_g[internalCellIndex] = 0;
    /* ICIC changes end */
#ifdef FDD_CONFIG 
    UInt8 ttiMapId1 = TTI_MAP_FREE;
    UInt8 ttiMapId2= TTI_MAP_FREE;
#endif    
    if (!tempTotalActiveUE)
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,UL_RR_STRATEGY,
                "[UL-Strategy][%s]Returning because tempTotalActiveUE is ZERO\n",
                __func__);
        /*LTE_MAC_UT_LOG(LOG_INFO,UL_RR_STRATEGY,
          "[UL-Strategy][%s]Returning because tempTotalActiveUE is ZERO\n",
          __func__);*/
        return MAC_SUCCESS;
    }
    /*SPR 5757 Start*/  
    else if (1 == tempTotalActiveUE){
        /*LTE_MAC_UT_LOG(LOG_INFO,UL_RR_STRATEGY,
          "[UL-Strategy][%s]tempTotalActiveUE is %d\n",
          __func__,tempTotalActiveUE);*/
        nonZeroBsrListStartRoundRobin_p[internalCellIndex] = PNULL;
        /*SPR 5757 End*/ 
    }
    LTE_MAC_UT_LOG(LOG_DETAIL,UL_RR_STRATEGY,
            "[UL-Strategy][%s]Total UEs in Non Zero BSR List = %d \n"
            "max UE to be scheduled = %d "
            " Number Of Total Available RBs = %d, Cell Edge Available RBs = %d ",
            __func__, tempTotalActiveUE , maxUEsToBeScheduledUL,
            numberOfAvailableRBs_p[CC_CE_REGION],numberOfAvailableRBs_p[CE_REGION]);

    /*CA Changes start  */
    minRb = (MAC_SCH_MIN_UL_GRANT > MIN_NUM_RB_FOR_PL_PHR_CALC(internalCellIndex)) ?\
            MAC_SCH_MIN_UL_GRANT: MIN_NUM_RB_FOR_PL_PHR_CALC(internalCellIndex);
    /*CA Changes end  */

    /* SPR 5036 Fix Start */
    /* calculating max number of users that can be scheduled using minRB */
    /*
     ** Calculating the no. of UEs which can scheduled as per minimum RBs
     ** available for each of them for scheduling.
     */
    numOfUeToSchedule = numberOfAvailableRBs_p[CC_CE_REGION]/minRb;
    /* SPR 5036 Fix End */
    if(maxUEsToBeScheduledUL > numOfUeToSchedule)
    {
        LOG_UT(MAC_UL_RR_MAX_UE_SCHEDULE_GT_NUM_OF_UE_SCHEDULE,LOGDEBUG,MAC_DL_Strategy,
                getCurrentTick(),
                maxUEsToBeScheduledUL,numOfUeToSchedule,tempTotalActiveUE,
                /*CA Changes start  */
                maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex],LINE_NUMBER,
                /*CA Changes end  */
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");

        /* SPR 5036 Fix Start */
        /* 
         ** No UE will be scheduled if available RBs are less than min RBs
         ** required for scheduling
         */
        if (!numOfUeToSchedule)
        {
            return MAC_SUCCESS;
        }
        /* SPR 5036 Fix End */
        maxUEsToBeScheduledUL = numOfUeToSchedule;
    }

    if(maxUEsToBeScheduledUL > tempTotalActiveUE)
    {
        LOG_UT(MAC_UL_RR_MAX_UE_SCHEDULE_GT_TOTAL_ACTIVE_UE,LOGDEBUG,MAC_DL_Strategy,
                getCurrentTick(),
                maxUEsToBeScheduledUL,numOfUeToSchedule,tempTotalActiveUE,
                /*CA Changes start  */
                maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex],LINE_NUMBER,
                /*CA Changes end  */
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
        maxUEsToBeScheduledUL = tempTotalActiveUE;
    }

    /* SPR 5036 Fix Start */
    /*
     ** Calculating the max no. of CE UEs which can scheduled as per minimum CE RBs
     ** available for each of them for scheduling.
     */
    numOfUeToSchedule =  numberOfAvailableRBs_p[CE_REGION]/ minRb;
    /* SPR 5036 Fix End */
    /*CA Changes start  */
    if(maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] > numOfUeToSchedule)
    {
        LOG_UT(MAC_UL_RR_MAX_CE_UE_GT_NUM_OF_UE_SCHEDULE,LOGDEBUG,MAC_DL_Strategy,
                getCurrentTick(),
                maxUEsToBeScheduledUL,numOfUeToSchedule,tempTotalActiveUE,
                maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex],LINE_NUMBER,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
        maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] = numOfUeToSchedule;
    }

    if(maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] > tempTotalActiveUE)
    {
        LOG_UT(MAC_UL_RR_MAX_CE_UE_GT_TOTAL_ACTIVE_UE,LOGDEBUG,MAC_DL_Strategy,
                getCurrentTick(),
                maxUEsToBeScheduledUL,numOfUeToSchedule,tempTotalActiveUE,
                maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex],LINE_NUMBER,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
        maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] = tempTotalActiveUE;
    }
    /*CA Changes end  */


#ifdef FLOAT_OPERATIONS
    /* ICIC changes start */
    maxULRBToBeAllocInRoundRobin = ceilf_wrapper(((UDouble32)
                (numberOfAvailableRBs_p[CC_CE_REGION]))/maxUEsToBeScheduledUL);
#else
    maxULRBToBeAllocInRoundRobin = ( numberOfAvailableRBs_p[CC_CE_REGION])/
        maxUEsToBeScheduledUL ;
    /* SPR 5052 Fix Start*/
#ifndef FDD_CONFIG
    if ((numberOfAvailableRBs_p[CC_CE_REGION]) % maxUEsToBeScheduledUL)
    {
        maxULRBToBeAllocInRoundRobin++;
    }
#endif
    /* SPR 5052 Fix end*/
#endif

    // SPR 3486  - START

    /* ICIC changes start */
    if (maxULRBToBeAllocInRoundRobin < minRb)
    {
        maxULRBToBeAllocInRoundRobin = minRb;
    }
    /* ICIC changes end */

    // SPR 3486  - END
    LTE_MAC_UT_LOG(LOG_DETAIL,UL_RR_STRATEGY,
            "[UL-Strategy][%s]Total RBs to be distribute per UE :%d "
            "in Round Robin", __func__, maxULRBToBeAllocInRoundRobin);

    /*Finding Out the pointer in Non Zero List from where the Round Robin 
      scheduler need to start.It May be start of the list or the Node from
      where last time Scheduler left scheduling */

    /* ICIC changes start */
    /*CA Changes start  */
    cePriorityListCount = sQueueCount(ulCellEdgeUserPriorityQueue_gp[internalCellIndex]);

    if (cePriorityListCount && maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]
    /*CA Changes end  */
            && numberOfAvailableRBs_p[CE_USER] > minRb)
    {
        LOG_UT(MAC_UL_RR_PROCESSING_CE_PRIORITY_Q,LOGDEBUG,MAC_DL_Strategy,
                getCurrentTick(),
                cePriorityListCount,maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex],
                numberOfAvailableRBs_p[CE_USER],
                DEFAULT_INT_VALUE,LINE_NUMBER,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
        processULPriorityCellEdgeUserList(cePriorityListCount,
                &ulUserList[CE_USER],
                currentGlobalTTITickCount,
                &maxUEsToBeScheduledUL,
                referenceFlag
#ifdef FDD_CONFIG
                ,ulDelay
#endif
                ,internalCellIndex
                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                ,ulSubFrameNum
#endif
                /* TDD Config 0 Changes End */
                );
    }
    /* ICIC changes end */

    /* +DYNAMIC_ICIC */
    numOfUE = ulScheduleInfo_gp[internalCellIndex]->numOfUe;
    if(numOfUE)
    {
        while(maxUEsToBeScheduledUL && numOfUE)
        {
            ueIndex = ulScheduleInfo_gp[internalCellIndex]->ulUeSchedulerInfoArr[numOfUE -1].ueIndex;
            ueULContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
            userLocationType = ueULContext_p->userLocationType;
#ifdef TDD_CONFIG
            UInt8 ulHarqProcessId = BUSY_ALL_HARQ;
            /* TDD Config 0 and 6 Changes Start */
    /* SPR 23393 Changes Start */
            UInt32 subframeNum = 0;
            UInt32 sysFrameNum = 0;
            GET_SFN_SF_FROM_TTI(currentGlobalTTITickCount, sysFrameNum, subframeNum)
            /* SPR 23393 Changes End */
                /* +- SPR 17777 */
              LTE_GCC_UNUSED_PARAM(sysFrameNum)
            /* Get the free ID in ulHarqProcessId 
               if all the harq id is busy the skip this node and goto the next node in 
               the list and start this procedure again.
             */
            /* SPR 11257 Changes Start*/
    	    ulHarqProcessId = getFreeULHarqId(ueULContext_p, subframeNum,
				internalCellIndex,ulSubFrameNum,currentGlobalTTITickCount);
            /* +- SPR 17777 */
            /* SPR 11257 Changes End*/
            /* TDD Config 0 and 6 Changes End */
#elif FDD_CONFIG
            /* TTIB_Code Start */
            UInt8 ulHarqProcessId = MODULO_EIGHT(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
            if ( ueULContext_p->ttiBundlingEnabled )
            {
                ulHarqProcessId = GET_TTIB_HARQ_ID( currentGlobalTTITickCount +ulDelay + FDD_HARQ_OFFSET );    
            }
            ttiMapId1 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
            ttiMapId2 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET + MAX_HARQ_PROCESS_NB );
            /* TTIB_Code End */
#endif

#ifdef TDD_CONFIG
            if (BUSY_ALL_HARQ != ulHarqProcessId) 
#elif FDD_CONFIG
                /* TTIB_Code Start */
                if((HARQ_PROCESS_FREE != ueULContext_p->ulresInfo[ulHarqProcessId].harqStatus)
                        ||(ueULContext_p->ttiBundlingEnabled &&
                            (MAC_FAILURE == processTtiBHarqCheck(currentGlobalTTITickCount,ulDelay,ueULContext_p)))
                        ||(!(ueULContext_p->ttiBundlingEnabled)&&
                            ((TTI_MAP_FREE != ueULContext_p->ttiHarqMap[ttiMapId1]) ||
                             ( TTI_MAP_FREE != ueULContext_p->ttiHarqMap[ttiMapId2])))
                        /* + SPS_TDD_Changes */
                        /*This code has been commented as we will not block harq process to be used for SPS only */
                        /*SPR 7944 START*/
                        /*
                           ||checkSpsOccasionTti(ueULContext_p,ulHarqProcessId)
                           ||checkSpsActTti(ueULContext_p,ulHarqProcessId)
                           */
                        /*SPR 7944 END*/
                        /* - SPS_TDD_Changes */
                  )
                    /* TTIB_Code End */
#endif
                {
                    numOfUE--;
                }
                else
                {
                    ueCount = ulUserList[userLocationType].count;
                    ulUserList[userLocationType].ulSchedulableUENode
                        [ueCount].ueIndex = ueIndex;
                    ulUserList[userLocationType].ulSchedulableUENode
                        [ueCount].harqProcessId = ulHarqProcessId;

                    ulUserList[userLocationType].count++;
                    /* ULA_CHG */
                    /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                    ueULContext_p->isAlreadyConsideredFlag = referenceFlag;
#elif TDD_CONFIG
                    ueULContext_p->isAlreadyConsideredFlag[ulSubFrameNum] = referenceFlag;
#endif
                    /* TDD Config 0 Changes End */
                    maxUEsToBeScheduledUL--;
                    numOfUE--;
                    if (CE_USER == ueULContext_p->userLocationType)
                    {
                        /*CA Changes start  */
                        maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]--;
                        /*CA Changes end  */
                    }

                }
        }
    }
    /* -DYNAMIC_ICIC */

    /*Start distributing until the RBs are exhausted */
    /* cyclomatic changes */
    processNonZeroBsrUEsInRRManner(&maxUEsToBeScheduledUL,
            referenceFlag,
            numberOfAvailableRBs_p,
            currentGlobalTTITickCount,
            ulUserList                       
#ifdef FDD_CONFIG
            ,ulDelay
#endif
            /*CA Changes start  */
            ,internalCellIndex
            /*CA Changes end  */
            /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
            ,ulSubFrameNum
#endif
            /* TDD Config 0 Changes End */
            );
    /* cyclomatic changes */

    /* ICIC Change Start */
    /* Processing CE users */
    ueCount = ulUserList[CE_USER].count;
    if(ueCount)
    {
#ifdef FLOAT_OPERATIONS
        /* ICIC changes start */
        maxCellEdgeRBToBeAllocInRoundRobin = ceilf_wrapper(((UDouble32)
                    (numberOfAvailableRBs_p[CE_REGION]))/ueCount);
#else
        maxCellEdgeRBToBeAllocInRoundRobin = ( numberOfAvailableRBs_p[CE_REGION])/
            ueCount ;

        if ((numberOfAvailableRBs_p[CE_REGION]) % ueCount)
        {
            maxCellEdgeRBToBeAllocInRoundRobin++;
        }
#endif
        if(maxCellEdgeRBToBeAllocInRoundRobin > maxULRBToBeAllocInRoundRobin)
        {
            maxCellEdgeRBToBeAllocInRoundRobin = maxULRBToBeAllocInRoundRobin;
        }

        if(maxCellEdgeRBToBeAllocInRoundRobin < minRb)
        {
            maxCellEdgeRBToBeAllocInRoundRobin = minRb;
        }

        tempAvailRB = numberOfAvailableRBs_p[CE_USER];

        processULSchedulableList(&ulUserList[CE_USER],
                /* +- SPR 17777 */
                &numberOfAvailableRBs_p[CE_USER],
                maxCellEdgeRBToBeAllocInRoundRobin,
                /* +- SPR 17777 */
                currentGlobalTTITickCount
#ifdef FDD_CONFIG 
                ,ulDelay
#endif
                ,internalCellIndex
                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                ,ulSubFrameNum
#endif
                /* TDD Config 0 Changes End */
                );

        /* updating CC_CE_REGION Resources */
        numberOfAvailableRBs_p[CC_CE_REGION] -= 
            tempAvailRB - numberOfAvailableRBs_p[CE_REGION];
    }

    /* Processing CC users */
    ueCount = ulUserList[CC_USER].count;

    if(ueCount)
    {
        /* resetting unused RB to 0 */
        unusedULRb_g[internalCellIndex] = 0;
        /* calculating max RB for CC user again so that unused RB can be
         * distributed equally among CC user.*/
#ifdef FLOAT_OPERATIONS
        /* ICIC changes start */
        maxULRBToBeAllocInRoundRobin = ceilf_wrapper(((UDouble32)
                    (numberOfAvailableRBs_p[CC_CE_REGION]))/ueCount);
#else
        maxULRBToBeAllocInRoundRobin = ( numberOfAvailableRBs_p[CC_CE_REGION])/
            ueCount ;
        /* SPR 5052 Fix Start*/
#ifndef FDD_CONFIG
        if ((numberOfAvailableRBs_p[CC_CE_REGION]) % ueCount)
        {
            maxULRBToBeAllocInRoundRobin++;
        }
#endif
        /* SPR 5052 Fix end*/
#endif


        if (maxULRBToBeAllocInRoundRobin < minRb)
        {
            maxULRBToBeAllocInRoundRobin = minRb;
        }

        processULSchedulableList(&ulUserList[CC_USER],
                /* +- SPR 17777 */
                &numberOfAvailableRBs_p[CC_CE_REGION],
                maxULRBToBeAllocInRoundRobin,
                /* +- SPR 17777 */
                currentGlobalTTITickCount
#ifdef FDD_CONFIG 
                ,ulDelay
#endif
                ,internalCellIndex
                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                ,ulSubFrameNum
#endif
                /* TDD Config 0 Changes End */
                );

        if(numberOfAvailableRBs_p[CC_CE_REGION] <
                numberOfAvailableRBs_p[CE_REGION])
        {
            numberOfAvailableRBs_p[CE_REGION]=numberOfAvailableRBs_p[CC_CE_REGION];
        }

    }
    /* ICIC Change End */

    LOG_MAC_MSG (UL_MAX_UE_SCHEDULED_AND_AVAILABLE_RB_PER_TICK, LOGBRIEF,
            MAC_DL_Strategy, getCurrentTick (),
            (tempTotalActiveUE - maxUEsToBeScheduledUL),
            numberOfAvailableRBs_p[CC_CE_REGION],
            0,0,0,
            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : assignULRBsAndSendToPDCCHRoundRobin 
 * Inputs         : ueIndex - UE Index for which the processing needs to be done
 *                  allocatedRB- The Number of RBs Allocated Orginally
 *                  numberOfAvailableRBs_p - Pointer to the number of Avavailable RBs
 *                  currentGlobalTTITickCount - Global Tick Count
 *                  strictAllocRBFlag - Flag to indicate Strict Allocation
 *                  minDataSize,maxDataSize,mcsVal,ttiBundlingEnabled,ulHarqProcessId,
 *                  internalCellIndex
 *                  ulSubFrameNum - ulSubframe bumber for which Grant is being given
 * Outputs        : UL Startegy Node to PDCCH Module
 * Returns        : MAC_FAILURE or MAC_SUCCESS
 * Description    : This Function checks whether the UEs is having the nack Queue
 *                  Load or Queue Load Avavailable. Depending upon the
 *                  Availability of the either of the Queue Load, and depepnding
 *                  upon the transmission Mode, UE are validated and Allocated
 *                  Finally The DL Strategy Node is been prepared an provided
 *                  to PDCCH module
 *****************************************************************************/
#ifdef TDD_CONFIG
MacRetType  assignULRBsAndSendToPDCCHRoundRobin(
        UInt32 ueIndex,
        UInt32 allocatedRB,
        UInt32 * numberOfAvailableRBs_p,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 strictAllocRBFlag,
        UInt32 ulHarqProcessId
        /* ULA_CHG */
        ,UInt32 minDataSize,
        UInt32 maxDataSize,
        /* ULA_CHG */
        /* ATB changes Start */
        UInt8 mcsVal
        /* ATB changes End */
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */
        /* TDD Config 0 Changes Start */
        ,UInt8 ulSubFrameNum
        /* TDD Config 0 Changes End */
        )
#elif FDD_CONFIG
MacRetType  assignULRBsAndSendToPDCCHRoundRobin(
            UInt32 ueIndex,
            UInt32 allocatedRB,
            UInt32 * numberOfAvailableRBs_p,
            /* SPR 15909 fix start */
            tickType_t currentGlobalTTITickCount,
            /* SPR 15909 fix end */
            UInt32 strictAllocRBFlag
            /* ULA_CHG */
            ,UInt32 minDataSize,
            UInt32 maxDataSize,
            /* ATB changes Start */
            UInt8 mcsVal
            /* ATB changes End */
            /* ULA_CHG */
            ,UInt8 ttiBundlingEnabled
            /*CA Changes start  */
            ,InternalCellIndex internalCellIndex
            /*CA Changes end  */
            )
#endif
{
    ULStrategyTxNode *ulStrategyTxNode_p = PNULL;
    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
    ULUEContext * tempUplinkContext_p = 
        ulUECtxInfoArr_g[ueIndex].ulUEContext_p;
#endif
    /* TDD Config 0 Changes End */

    /*Preparing the UL Strategy outPut Node*/
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
    GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else
    GET_MEM_FROM_POOL(ULStrategyTxNode,ulStrategyTxNode_p,sizeof(ULStrategyTxNode) ,PNULL);
#endif

    /* + coverity 24549 */
    if (PNULL == ulStrategyTxNode_p)
    {
        return MAC_FAILURE;
    }
    /* - coverity 24549 */

    ulStrategyTxNode_p->ueIndex     = (UInt16)ueIndex;
    ulStrategyTxNode_p->requiredRB  = allocatedRB;
    ulStrategyTxNode_p->strictAllocRB = strictAllocRBFlag;
#ifdef FDD_CONFIG
    if(ttiBundlingEnabled)
    {
        ulStrategyTxNode_p->ulGrantRequestType = NEW_TTIB_ULGRANT_REQ_MSG;
    }
    else 
#endif
    {    
        ulStrategyTxNode_p->ulGrantRequestType = NEW_ULGRANT_REQ_MSG;
    }    
    ulStrategyTxNode_p->ttiCounter = currentGlobalTTITickCount;

    /* ICIC changes start */
    UInt8 avgMcs = 0;
    /* ICIC changed end */
    /* ATB changes Start */
    avgMcs = mcsVal;
    /* ATB changes Start */
#ifdef TDD_CONFIG       
    ulStrategyTxNode_p->ulHarqProcessId = ulHarqProcessId;
    /*TDD Config 0 Changes Start*/
    ulStrategyTxNode_p->scheduledCount = 0;
    /*TDD Config 0 Changes End*/
#endif
    /* ULA_CHG */
    ulStrategyTxNode_p->minDataSize = minDataSize;
    ulStrategyTxNode_p->maxDataSize = maxDataSize;
    ulStrategyTxNode_p->avgMcs = avgMcs;
    /* ULA_CHG */

    /* ICIC changes start */
    *numberOfAvailableRBs_p = 
        *numberOfAvailableRBs_p - allocatedRB;

    /* ICIC changed end */

    /*putEntry of ULStrategyTXNode in PDCCH Queue */
    /* ULA_CHG */
    /*CA Changes start  */
    if (!(putEntryInULSchedulerNewTxQueue(
                    ulStrategyTxNode_p
                    ,internalCellIndex
                    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                    ,currentGlobalTTITickCount
                    ,ulSubFrameNum
                    ,tempUplinkContext_p
#endif
                    /* TDD Config 0 Changes End */
                    )))
    {
        LOG_MAC_MSG(MAC_UL_STRATEGY_ERROR_ID,\
                LOGERROR,MAC_UL_Strategy,\
                currentGlobalTTITickCount,\
                ulStrategyTxNode_p->ueIndex,0,0,0,0,0.0,0.0,\
                __func__,"ulStrategyTxNode_p entry failed");
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
        FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p);
#else
        freeMemPool(ulStrategyTxNode_p);
#endif
    }
    else
    {
        LOG_UT(MAC_UL_STRATEGY_TX_NODE_ID,\
                LOGDEBUG,MAC_UL_Strategy,\
                currentGlobalTTITickCount,\
                ulStrategyTxNode_p->ueIndex,\
                ulStrategyTxNode_p->requiredRB,\
                ulStrategyTxNode_p->strictAllocRB,\
                ulStrategyTxNode_p->minDataSize,\
                ulStrategyTxNode_p->maxDataSize,\
                ulStrategyTxNode_p->avgMcs,\
                ulStrategyTxNode_p->ttiCounter,\
                __func__,"ulStrategyTxNode_p parameters");
    }
    /* ULA_CHG */

    return MAC_SUCCESS;
}

/* ICIC changes start */
/*****************************************************************************
 * Function Name  : processULPriorityCellEdgeUserList 
 * Inputs         : cePriorityListCount - Cell Edge Priority List count
 *                  ulUserList_p - pointer to  CC or CE User list
 *                  currentGlobalTTITickCount:Current Global Tick
 *                  maxUEsToBeScheduledUL_p - Max UE to can be sceduled in UL
 *                  referenceFlag - Used for checking whether the US is 
 *                     considered already or not.If UE's rbsReservedForUE is equal 
 *                     to referenceFlag, that means the UE is already considered
 *                  ulDelay delay in Uplink path
 *                  internalCellIndex - cell index used at MAC.
 *                  ulSubFrameNum : ulSubframe for which Grant is being given.
 * Outputs        : It will allocate RBs to pending cell edge UE's
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : Its main function will be to process the priority list
 *                  which conatains pending Cell Edge UE. Try to allocate RBs
 *                  to them, if failed to do so, push it again in list.
 *****************************************************************************/
MacRetType processULPriorityCellEdgeUserList(
             UInt32 cePriorityListCount,
             ULUserListRR *ulUserList_p,
             /* SPR 15909 fix start */
             tickType_t currentGlobalTTITickCount,
             /* SPR 15909 fix end */
             UInt8  * maxUEsToBeScheduledUL_p,
             /* SPR 21068 start */ 
             tickType_t referenceFlag
             /* SPR 21068 end   */
#ifdef FDD_CONFIG
             ,UInt32 ulDelay
#endif
             /*CA Changes start  */
             ,InternalCellIndex internalCellIndex
             /*CA Changes end  */
             /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
             ,UInt8 ulSubFrameNum
#endif
             /* TDD Config 0 Changes End */
           )
{
    UInt8 firstCellEdgeNode                   = TRUE;
    ULUEContextInfo * tempUplinkContextInfo_p = PNULL;
    ULUEContext * ueULContext_p               = PNULL;
    UInt32 ueIndex ;
    DLUEContextInfo * ueDLContextInfo_p       = PNULL;
/*    DLUEContext *ueDLContext_p                = PNULL; TDD Warning Fix*/
    CEUserPriorityQueueNode *ceUserPriorityQueueNode_p = PNULL;   
    CEUserPriorityQueueNode *ceUserPriorityQueueNodeTmp_p = PNULL;
   /*UInt8 userLocationType = CE_USER ; TDD warning Fix */
    UInt8 ueCount = 0;
#ifdef FDD_CONFIG
    UInt8 ttiMapId1 = TTI_MAP_FREE;
    UInt8 ttiMapId2 = TTI_MAP_FREE;
#endif
    /*CA Changes start  */
    ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *)getFirstNode(ulCellEdgeUserPriorityQueue_gp[internalCellIndex]);
    /*CA Changes end  */
    while (ceUserPriorityQueueNode_p && (cePriorityListCount--) && 
            /*CA Changes start  */
            (maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]) && (*maxUEsToBeScheduledUL_p))
            /*CA Changes end  */
    {
        ueIndex                 = ceUserPriorityQueueNode_p->ueIndex;
        tempUplinkContextInfo_p = &ulUECtxInfoArr_g[ueIndex];
        ueDLContextInfo_p       = &dlUECtxInfoArr_g[ueIndex];
        ueULContext_p           = tempUplinkContextInfo_p->ulUEContext_p;

/*      ueDLContext_p = ueDLContextInfo_p->dlUEContext_p; TDD Warning Fix */

        if (ceUserPriorityQueueNode_p->isNodeInvalid)
        {
            if(firstCellEdgeNode)
            {
                /*CA Changes start  */
                ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) popNode (ulCellEdgeUserPriorityQueue_gp[internalCellIndex]);
                /*CA Changes end  */
                /* +COVERITY 11434 */
                if (NULL == ceUserPriorityQueueNode_p)
                {
                    break;
                }
                /* -COVERITY 11434 */

                ceUserPriorityQueueNodeTmp_p = ceUserPriorityQueueNode_p;
                ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *)
                    getNextNode ((const LTE_SNODE *) (ceUserPriorityQueueNodeTmp_p));

                freeMemPool (ceUserPriorityQueueNodeTmp_p);
                continue;
            }
            else
            {
                ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) getNextNode
                    ((const LTE_SNODE *) (ceUserPriorityQueueNode_p));
                continue;
            }
        } 

        if (!(tempUplinkContextInfo_p->pendingDeleteFlag ))
        {
                /* EICIC +*/
/*Complexity Start*/
		if (((VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) 
               && (NON_VICTIM_UE == ueULContext_p->userType)) || 
                ((NON_VICTIM_UE == ueTypeToScheduleUL_g[internalCellIndex]) 
                    && (VICTIM_UE == ueULContext_p->userType)))
		{              
                    firstCellEdgeNode = FALSE;
                	ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) getNextNode
                    ((const LTE_SNODE *) (ceUserPriorityQueueNode_p));
                    continue;
		}

/*Complexity End*/
		/* EICIC -*/
	
            /* TX_RETX */            
        /* userLocationType = ueULContext_p->userLocationType;
           TDD Warning Fix */
            /* \code
             * if DRX is ON or Meas Gap is ON or schType == SPS_SCHEDULING,
             *      then do not schedule this UE.
             * */
            if ((DRX_STATE_ON == ueULContext_p-> drxCurrentState) || 
                (TRUE == ueULContext_p->measGapContext.isMeasGapPeriodOn) ||
                ( SPS_SCHEDULING == ueULContext_p->schType ) )
            {
                if(firstCellEdgeNode)
                {
                    /*CA Changes start  */
                    ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) popNode (ulCellEdgeUserPriorityQueue_gp[internalCellIndex]);
                    /*CA Changes end  */
                   
                    /* +COVERITY 11434 */
                    if (NULL == ceUserPriorityQueueNode_p)
                    {
                        break;
                    }
                    /* -COVERITY 11434 */

                    ceUserPriorityQueueNodeTmp_p = ceUserPriorityQueueNode_p;
                    ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *)
                        getNextNode ((const LTE_SNODE *) (ceUserPriorityQueueNodeTmp_p));

                    freeMemPool (ceUserPriorityQueueNodeTmp_p);
                }
                else
                {
                    ceUserPriorityQueueNode_p->isNodeInvalid = TRUE;
                    ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) getNextNode
                        ((const LTE_SNODE *) (ceUserPriorityQueueNode_p));
                }
                
                ueULContext_p->isAlreadyInCEPriorityList = FALSE;
               
                continue;
            }

            if(UL_SYNC_LOSS_ON !=
                    checkUlSyncLossForUE(ueDLContextInfo_p->dlUEContext_p, 
                            currentGlobalTTITickCount, internalCellIndex))
            {

                /* HD FDD  Changes Start */
#ifdef HD_FDD_CONFIG

                /** check if this UE is HD FDD UE **/
                if(ueULContext_p->isHDFddFlag)
                {
                    /* Check UL can be scheduled for HD UE in cell edge node of current tick*/
                    if (!CAN_SCHEDULE_UL_FOR_HD_UE(currentGlobalTTITickCount + PHY_DL_DELAY, ueIndex , hdUeContextInfo_g.hdfddUeSchedMap ))
                    {
                        if(firstCellEdgeNode)
                        {
                            /*CA Changes start  */
                            ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) popNode (ulCellEdgeUserPriorityQueue_gp[internalCellIndex]);
                            /*CA Changes end  */
                            /*HD FDD Coverity Fix Start*/
                            if(PNULL != ceUserPriorityQueueNode_p)
                            {
                                ceUserPriorityQueueNodeTmp_p = ceUserPriorityQueueNode_p;
                                ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) getNextNode 
                                    ((const LTE_SNODE *) (ceUserPriorityQueueNodeTmp_p));
                                freeMemPool (ceUserPriorityQueueNodeTmp_p);
                            }
                            /*HD FDD Coverity Fix End*/
                        }
                        else
                        {
                            ceUserPriorityQueueNode_p->isNodeInvalid = TRUE;
                            ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) getNextNode
                                ((const LTE_SNODE *) (ceUserPriorityQueueNode_p));
                        }
                        ueULContext_p->isAlreadyInCEPriorityList = FALSE;
                        /* Increment the statistics counter UE denied UL resources due to
                         * DL clash in the same TTI of HD FDD UE*/
                        UPDATE_UE_DENIED_UL_RESOURCE(ueIndex);
                        continue;
                    }
                }
                /* HD FDD  Changes End */
#endif


                /*SPR_3065_FIX*/
#ifdef TDD_CONFIG
                UInt8 ulHarqProcessId = BUSY_ALL_HARQ;
                /* TDD Config 0 and 6 Changes Start */
    /* SPR 23393 Changes Start */
                UInt32 subframeNum = 0;
                UInt32 sysFrameNum = 0;
                GET_SFN_SF_FROM_TTI(currentGlobalTTITickCount, sysFrameNum, subframeNum)
            /* SPR 23393 Changes End */
                    /* +- SPR 17777 */
                  LTE_GCC_UNUSED_PARAM(sysFrameNum)
                /* Get the free ID in ulHarqProcessId 
                   if all the harq id is busy the skip this node and goto the next node in 
                   the list and start this procedure again.
                   */
                /* SPR 11257 Changes Start*/
                ulHarqProcessId = getFreeULHarqId(ueULContext_p, subframeNum,
						internalCellIndex, ulSubFrameNum,currentGlobalTTITickCount);
                /* +- SPR 17777 */
                /* SPR 11257 Changes End*/
                /* TDD Config 0 and 6 Changes End */
#elif FDD_CONFIG
                /* TTIB_Code Start */
                UInt8 ulHarqProcessId = MODULO_EIGHT(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
                if ( ueULContext_p->ttiBundlingEnabled )
                {
                    ulHarqProcessId = GET_TTIB_HARQ_ID( currentGlobalTTITickCount +ulDelay + FDD_HARQ_OFFSET );    
                }
                ttiMapId1 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
                ttiMapId2 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET + MAX_HARQ_PROCESS_NB );
                /* TTIB_Code End */
#endif

#ifdef TDD_CONFIG
                if (BUSY_ALL_HARQ == ulHarqProcessId) 
#elif FDD_CONFIG
                /* TTIB_Code Start */
                    if((HARQ_PROCESS_FREE != ueULContext_p->ulresInfo[ulHarqProcessId].harqStatus)
                                ||(ueULContext_p->ttiBundlingEnabled &&
                                    (MAC_FAILURE == processTtiBHarqCheck(currentGlobalTTITickCount,ulDelay,ueULContext_p)))
                                ||(!(ueULContext_p->ttiBundlingEnabled)&&
                                    ((TTI_MAP_FREE != ueULContext_p->ttiHarqMap[ttiMapId1]) ||
                                     ( TTI_MAP_FREE != ueULContext_p->ttiHarqMap[ttiMapId2])))
                                /*SPR 5944 START*/
                                /* + SPS_TDD_Changes */
                                /*This code has been commented as we will not block harq process to be used for SPS only */
                                /*
                                   || checkSpsOccasionTti(ueULContext_p,ulHarqProcessId)
                                   || checkSpsActTti(ueULContext_p,ulHarqProcessId)
                                   */
                                /* - SPS_TDD_Changes */
                                /*SPR 5944 END*/
                      )
                /* TTIB_Code End */
#endif
                    {
                        //(*maxUEsToBeScheduledUL_p)--;
                        firstCellEdgeNode = FALSE;
                        ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) getNextNode
                            ((const LTE_SNODE *) (ceUserPriorityQueueNode_p));
                        continue;
                    }

                /*Checking if Already being considered for RB Distribution*/
                    /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                if (ueULContext_p->isAlreadyConsideredFlag != referenceFlag)
#elif TDD_CONFIG
                if (ueULContext_p->isAlreadyConsideredFlag[ulSubFrameNum] 
                    != referenceFlag)
#endif
                {
#ifdef FDD_CONFIG
                    ueULContext_p->isAlreadyConsideredFlag = referenceFlag;
#elif TDD_CONFIG
                    ueULContext_p->isAlreadyConsideredFlag[ulSubFrameNum] = 
                    referenceFlag;
#endif
                    /* TDD Config 0 Changes End */
                    ueCount = ulUserList_p->count;
                    ulUserList_p->ulSchedulableUENode
                        [ueCount].ueIndex = ueIndex;
                    ulUserList_p->ulSchedulableUENode
                        [ueCount].harqProcessId = ulHarqProcessId;

                    ulUserList_p->count++;

                    (*maxUEsToBeScheduledUL_p)--;
                    /*CA Changes start  */
                    maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] --;
                    /*CA Changes end  */

                }
            }
             /* SPR 21034 start */ 
             ueULContext_p->isAlreadyInCEPriorityList = FALSE;
             /* SPR 21034 end   */       
        }

        if(firstCellEdgeNode)
        {
            /*CA Changes start  */
            ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) popNode (ulCellEdgeUserPriorityQueue_gp[internalCellIndex]);
            /*CA Changes end  */
            /* +COVERITY 11434 */
            if (NULL == ceUserPriorityQueueNode_p)
            {
                break;
            }
            /* -COVERITY 11434 */
            ueULContext_p->isAlreadyInCEPriorityList = FALSE;

            ceUserPriorityQueueNodeTmp_p = ceUserPriorityQueueNode_p;
            ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *)
                getNextNode ((const LTE_SNODE *) (ceUserPriorityQueueNodeTmp_p));

            freeMemPool (ceUserPriorityQueueNodeTmp_p);
        }
        else
        {
            firstCellEdgeNode = FALSE;
            ceUserPriorityQueueNode_p->isNodeInvalid = TRUE;
            ceUserPriorityQueueNode_p = (CEUserPriorityQueueNode *) getNextNode
                ((const LTE_SNODE *) (ceUserPriorityQueueNode_p));
        }
        /* SPR 21034 start */
        /*  code removed   */
        /* SPR 21034 end   */
    }/* end of while */

    return MAC_SUCCESS;
}
/* ICIC changes start */
/*****************************************************************************
 * Function Name  : putEntryInULCellEdgePriorityQueue
 * Inputs         : ueIndex - Index of UEs,
 *                  internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : MAC_SUCCES or MAC_FAILURE
 * Description    : Its main function is to insert node in will be to process 
 *                  ulCellEdgeUserPriorityQueue_g
 *****************************************************************************/
 MacRetType putEntryInULCellEdgePriorityQueue(UInt16 ueIndex,
                                                    /*CA Changes start  */
                                                    InternalCellIndex internalCellIndex)
                                                    /*CA Changes end  */
{

    CEUserPriorityQueueNode *ceUserPriorityQueueNode_p = PNULL;
    GET_MEM_FROM_POOL(CEUserPriorityQueueNode,ceUserPriorityQueueNode_p,
            sizeof(CEUserPriorityQueueNode),PNULL);

    /* + coverity 24550 */
    if (PNULL == ceUserPriorityQueueNode_p)
    {
        return MAC_FAILURE;
    }
    /* - coverity 24550 */

    ceUserPriorityQueueNode_p->ueIndex        = ueIndex;
    ceUserPriorityQueueNode_p->isNodeInvalid  = FALSE;
    ulUECtxInfoArr_g[ueIndex].ulUEContext_p->isAlreadyInCEPriorityList = TRUE;
    /*CA Changes start  */
    if(pushNode(ulCellEdgeUserPriorityQueue_gp[internalCellIndex],
    /*CA Changes end  */
                &(ceUserPriorityQueueNode_p->cellEdgePriorityQueueNodeAnchor)))
    {
        freeMemPool((void *)ceUserPriorityQueueNode_p);
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}


