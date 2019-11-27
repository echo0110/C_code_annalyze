/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacULFairScheduler.c,v 1.1.1.1.6.1.4.2.2.1 2010/09/21 15:46:20 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This is used for distributing the Uplink RBs based on 
                       the basis of proportionally fair scheduling. This 
                       function calculates the Weighted priority of a UE
                       considering different factors i.e. UE Priority Factor,
                       Modulation Factor,Delay Factor and BSR Queue Load factor
                       and then allocatesResource blocks to the UEs based on 
                       the value of their respective weighted priorities.
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacULFairScheduler.c,v $
 * Revision 1.1.1.1.6.1.4.2.2.1  2010/09/21 15:46:20  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.20  2009/07/27 06:09:22  gur20052
 * UT Defects are fixed
 *
 * Revision 1.19  2009/07/24 12:39:34  gur20052
 * Splint Warnings are removed
 *
 * Revision 1.18  2009/07/16 17:36:14  gur20052
 * Added code for Setting up of aperiodicCQIRequested flag in DL UE Context and UT Bug Fix
 *
 * Revision 1.17  2009/07/13 12:25:10  gur20052
 *
 * Revision 1.16  2009/07/10 13:50:38  gur20052
 * MACModuleLogDetail_g is replaced with the Module Specific STRINGS
 *
 * Revision 1.15  2009/07/07 05:32:13  gur20052
 * UT Defects are fixed
 *
 * Revision 1.14  2009/06/23 16:55:42  gur20052
 * Review comments are incorporated
 *
 * Revision 1.12  2009/06/08 09:59:34  gur20052
 * removed the linking errors
 *
 * Revision 1.11  2009/06/05 16:56:28  gur20052
 * Incorporated the reveiw comments
 *
 * Revision 1.10  2009/06/05 16:20:23  gur20052
 * Done the Changes for the Enhancements
 *
 * Revision 1.9  2009/06/04 07:39:50  gur20052
 * Review Comments are incorporated
 *
 * Revision 1.8  2009/06/02 07:19:08  gur20052
 * Updated during comments incorporation and dispostion
 *
 * Revision 1.7  2009/05/28 11:33:01  gur20052
 * The changes for the UE contexts are done
 *
 * Revision 1.6  2009/05/26 10:16:04  gur20052
 * Updated to incorporated UE Context Changes
 *
 * Revision 1.5  2009/05/25 16:20:01  gur20052
 * Updated the version to support the UL Fair and Round Robin Scheduler
 *
 * Revision 1.4  2009/05/21 17:22:46  gur20052
 * Updated the Basic Framework of UL Strategy
 *
 * Revision 1.3  2009/05/19 14:29:31  gur20052
 * Updated with DL Fair Scheduler module specific changes
 *
 * Revision 1.2  2009/05/11 06:18:32  gur20052
 * Removed compilation errors
 *
 * Revision 1.1  2009/04/30 17:36:43  gur20052
 * Added inital version
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
#include "lteMacULFairScheduler.h"
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
/* SPR 3083 Fix begin */
#ifdef TDD_CONFIG
static UInt8 tddNumOfULSf[MAX_UL_DL_CONFIG] = {6, 4, 2, 3, 2, 1, 5};
#endif
/* SPR 3083 Fix end */
/******************************************************************************
 Private Types
 *****************************************************************************/
static SortNodeT *leastPriorityUe_p[MAX_NUM_CELL] = {PNULL};
/* ICIC changes start */
static SortNodeT *leastPriorityCEUe_p[MAX_NUM_CELL] = {PNULL};
/*CA Changes start */
ULPfsGlobals *ulPfsGlobals_gp[MAX_NUM_CELL] = {PNULL};
/*CA Changes end */
/* ICIC changes end */

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* This function is used to compare the weighted Priority from first n
 * number of nodes of nonZeroBsrList to sort top n nodes.
 */
static void ulPfsCheckRemainingNodes(
          LTE_LIST_NODE *node_p, //node of nonZeroBsrList_g
          UInt8  maxUEsToBeScheduledUL,
          /* SPR 21068 start */ 
          tickType_t ulReferenceFlag,
          /* SPR 21068 end   */
          /*CA Changes start  */
          InternalCellIndex internalCellIndex
          /*CA Changes end  */
          /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
          ,UInt8 ulSubFrameNum
#endif
          /* TDD Config 0 Changes End */
          );

/* ICIC changes start */
static void ulPfsInsertCENodeInList(
                      LTE_LIST_NODE *pSortNodeT,
                      SortNodeT *pSortNodeAdd,
                      /*CA Changes start  */
                      InternalCellIndex internalCellIndex);
                      /*CA Changes end  */
static void ulPfsReplaceCENodeInList(
                      SortNodeT *pSortNodeT,
                      SortNodeT *pSortNodeAdd,
                      /*CA Changes start  */
                      InternalCellIndex internalCellIndex);
                      /*CA Changes end  */

static void ulPfsInsertNodeAtHead(
                      SortNodeT *pSortNodeAdd,
                      ULUEContext  *ueDLContext_p,
                      UInt8 *additionalNodeToBeDeleted_p,
                      /*CA Changes start  */
                      InternalCellIndex internalCellIndex);
                      /*CA Changes end  */
/* ICIC changes end */

/* This function is sort top n node from nonZeroBsrList List */
static void ulPfsCheckIfNodetoBeScheduled(
              LTE_LIST_NODE *node_p, //node of nonZeroBsrList
              ULUEContext  *ueULContext_p ,
              UInt8  maxUeToBeSorted,
              /*CA Changes start  */
              InternalCellIndex internalCellIndex);
              /*CA Changes end  */

/*Initilisation function for UL PFS Scheduler*/
void ulPfsInitMacSchedulerData(InternalCellIndex internalCellIndex);

/* Initialisation of Static Memory Pool for UL PFS Scheduler */
static void ulPfsInitMemAllocElem(InternalCellIndex internalCellIndex);

/*Function for Allocation of UL PFS Static Memory Pool */
static SortNodeT *ulPfsAllocSortNodeElem(InternalCellIndex internalCellIndex);

/*Function for Deallocation of UL PFS Static Memory Pool */
/*CA Changes start  */
static void ulPfsFreeSortNumElem (SortNodeT *pSortNode, InternalCellIndex internalCellIndex);
/*CA Changes end  */

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/* SPR 1010 changes start */
extern UInt8 maxpowerOfTwoThreeFive_g[MAX_NUM_RBS + 1]; 
extern UInt8 minpowerOfTwoThreeFive_g[MAX_NUM_RBS + 1]; 
// SPR 3486  - START
static UInt32 unUsedRBUL_g[MAX_NUM_CELL];
// SPR 3486  - END
/* SPR 1010 changes end */
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
/*****************************************************************************
 * Function Name  : processULSchedulableListPFS 
 * Inputs         : ulUserList - CC or CE user list,
 *                  ulScheduleData_p - Pointer to ULSchedulerDataElement,
 *                  availableRB_p - pointer to current available RBs,
 *                  minRB - Minimum RB,
 *                  currentGlobalTTITickCount - Current Global TTI Tick,
 *                  sumWeightedPriority_p - Weighted Priority,
 *                  ulDelay - UL Delay,
 *					internalCellIndex
 * Outputs        : Distribution of RBs among UEs on the basis of PFS
 * Returns        : MAC_SUCCESS/MAC_FAILURE 
 * Description    : This function works on Cell Edge and Cell Center user list
 *                  and allocate resources in proportion.
 *****************************************************************************/

MacRetType processULSchedulableListPFS(ULUserListPFS *ulUserList,
        ULSchedulerDataElement *ulScheduleData_p,
        UInt32 *availableRB_p,
        UInt8 minRB,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
         /* SPR 15909 fix end */
        UInt32 *sumWeightedPriority_p
#ifdef FDD_CONFIG
        ,UInt8 ulDelay
#endif
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */
        /* SPR 11978 Fix Start */
        /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
        ,UInt8 ulSubFrameNum
        /* TDD Config 0 Changes End */
        /* SPR 11978 Fix End */
#endif
        )
{
    UInt32 ueCount = 0;
    UInt32 listIndex = 0;
    UInt16 ueIndex = 0;
    ULUEContext *ueULContext_p = PNULL;
    UInt32 strictAllocRBFlag        = FALSE;
    ULSchedulerDataNodeInfo *schedulerInfoArr_p =PNULL;		    
    UInt32 minDataSize		    = 0;
    UInt32 maxDataSize		    = 0;
    UInt32 ueToSchedule = 0;
    UInt32 usedRB       = 0;
    UInt32 allocatedRB  = 0;
    UInt32 schedWeightedPriority    = 0;
    UInt8 userLocationType = CC_USER;
    /* Calculating initial Available RBs for CE region which can be 
     * distributed in proportion among CE users*/
    UInt32 initialAvailableRB = *availableRB_p;
    UInt32 rbWeightage = 0;

    /* ATB Changes Start */
    UInt8 mcsVal = 0;
    /* ATB Changes  End */


    ueCount = ulUserList->count;
    schedulerInfoArr_p = ulScheduleData_p->ulSchedulerDataNodeArr;
    /* Distrubute the RBs for selected UEs */
    while(listIndex < ueCount && *availableRB_p > minRB)
    {
        ueToSchedule = ulUserList->ulSchedulableUENode[listIndex].
            indexToDlSchedulerDataElement;
        ueULContext_p = schedulerInfoArr_p[ueToSchedule].ueULContext_p;
        userLocationType = ueULContext_p->userLocationType;
        ueIndex = ueULContext_p->ueIndex;
        schedWeightedPriority = 
            schedulerInfoArr_p[ueToSchedule].weightedPriority;

#ifdef FLOAT_OPERATIONS
        allocatedRB= ceilf_wrapper(((UDouble32)schedWeightedPriority *
                    (initialAvailableRB))/sumWeightedPriority_p[userLocationType]);
#else

        rbWeightage = schedWeightedPriority * initialAvailableRB;
        allocatedRB = rbWeightage/sumWeightedPriority_p[userLocationType];

        if (rbWeightage % sumWeightedPriority_p[userLocationType])
        {
            allocatedRB++;
        }
#endif
        /* Unused RB will be added directly to allocated RB */
        allocatedRB += unUsedRBUL_g[internalCellIndex];

        /* ULA_UTP */
        /*If RBs Allocated is Less Than MAC_SCH_MIN_UL_GRANT  after the distribution
          and UE has non Zero Weighted Priority,Then UE is allocated 
          MAC_SCH_MIN_UL_GRANT RBs*/
        // SPR 3486  - START
        if ( allocatedRB < minRB)
        {
            allocatedRB = minRB;
        }
        // SPR 3486  - END
        /* ULA_UTP */


        LTE_MAC_UT_LOG(LOG_DETAIL,UL_PF_STRATEGY,
                "[UL-Strategy][%s] Before Validate allocatedRB :%d\
                weightedPriority :%d by sumWeightedPriority %d \
                multiply by numberOfAvailableRBs::%d\n",__func__,
                allocatedRB,schedWeightedPriority, 
                sumWeightedPriority_p[userLocationType],*availableRB_p);                

        /*Validating RBs which are received after the distribution*/
        /* ULA_CHG */

        usedRB = validateUplinkRB 
            (ueULContext_p, allocatedRB, *availableRB_p,
             &strictAllocRBFlag,&minDataSize,&maxDataSize,
             /* ATB Changes Start */
             &mcsVal,
             /* SPR 13532 Fix Start */
#ifdef FDD_CONFIG
             currentGlobalTTITickCount + ulDelay
#else
             /*SPR 16417 Changes Start*/
             currentGlobalTTITickCount,
             ulSubFrameNum
             /*SPR 16417 Changes End*/
#endif
             /* ATB Changes End */
             ,internalCellIndex);
             /* SPR 13532 Fix End */

        /* ULA_CHG */

        if(allocatedRB > usedRB)
        {
            unUsedRBUL_g[internalCellIndex] = allocatedRB - usedRB;
        }
        else
        {
            unUsedRBUL_g[internalCellIndex] = 0;
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

            /* Coverity_73450 Fix Start */
            if(0 < usedRB)
            {    
            maxDataSize = determineUlTBSize(mcsVal,usedRB);
            }
            /* Coverity_73450 Fix End */

            minDataSize = determineUlTBSize(mcsVal,MIN_TTI_BUNDLE_RB);
            /*CA Changes start  */
            if ( (availableRBForTtiBundlingPerTick_g[internalCellIndex] < usedRB ) 
            /*CA Changes end  */
                    || (MAC_FAILURE == processMeasGapValidation(
                            ueULContext_p,currentGlobalTTITickCount,ulDelay )))
            {
                listIndex++;
                continue;
            }
            /*CA Changes start  */
            availableRBForTtiBundlingPerTick_g[internalCellIndex] -= usedRB;
            /*CA Changes end  */
            LTE_MAC_UT_LOG(LOG_INFO,UL_STRATEGY,"[UL-PFS-Strategy]"\
                    "[%s][%d]:TTIB UE ID %d scheduled RB:%d MCS:%d \n",
                    __func__,getCurrentTick(),ueIndex,usedRB,mcsVal);
        }
#endif        

        if (usedRB)
        {            
            schedulerInfoArr_p[ueToSchedule].numOfRBs = usedRB;

#ifdef TDD_CONFIG
            if (! assignULRBsAndSendToPDCCH(ueIndex,usedRB,
                        availableRB_p,
                        currentGlobalTTITickCount,
                        strictAllocRBFlag,
                        ulUserList->ulSchedulableUENode[listIndex].harqProcessId,
                        /* SPR 11978 Fix Start */
                        /* TDD Config 0 Changes Start */
                        ulSubFrameNum,
                        /* TDD Config 0 Changes End */
                        /* SPR 11978 Fix End */
                        /* ULA_CHG */
                        minDataSize,
                        maxDataSize,
                        /* ULA_CHG */
                        /* ATB Changes Start */
                        mcsVal, 
                        /* ATB Changes End */
                        internalCellIndex
                        ))  
#elif FDD_CONFIG		    
                if (! assignULRBsAndSendToPDCCH(ueIndex,usedRB,
                            availableRB_p,
                            currentGlobalTTITickCount,
                            strictAllocRBFlag,
                            /* ULA_CHG */
                            minDataSize,
                            maxDataSize,
                            /* ULA_CHG */
                            /* ATB Changes Start */
                            mcsVal
                             /* ATB Changes End */
                             /* For TTIB_Code */
                            ,ueULContext_p->ttiBundlingEnabled
                            ,internalCellIndex
                            ))  
#endif		 
                {
                    LTE_MAC_UT_LOG(LOG_INFO,UL_PF_STRATEGY,
                            "[UL-Strategy][%s]Scheduling of UE ID %d is Failed \n"
                            , __func__,ueIndex);
                }
                LOG_MAC_MSG (UL_FAIR_SCHEDULED_LOG_ID, LOGDEBUG,
                                MAC_UL_Strategy, getCurrentTick (), ueULContext_p->ueIndex,
                                0,
                                usedRB,
                                ueULContext_p->bsrNetTotal,
                                allocatedRB,
                                DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");


        }
        listIndex++;
    }
    return MAC_SUCCESS;
}
/********************************************************************************
 * Function Name  : ulProportionalFairScheduler
 * Inputs         : ulReferenceFlag - Used for checking whether the US is
 *                                    considered already or not. If UE's
 *                                    rbsReservedForUE is equal to referenceFlag
 *                                    that means the UE is already considered,
 *                  numberOfAvailableRBs_p - pointer to current available RBs,
 *                  maxUEsToBeScheduledUL - max UE to be scheduled in UL,
 *                  currentGlobalTTITickCount - Current Global Tick,
 *                  ulDelay - UL Delay,
 *					internalCellIndex
 * Outputs        : Distribution of RB based on the UE's Weighted priority
 * Returns        : MAC_SUCCESS
 * Description    : This is used for distributing the RBs based on the
 *                  proportionally fair scheduling. This function allocates 
 *                  Resource blocks to the UEs based on the value of weighted 
 *                  priorities.Weighted priority of a UE is calculated
 *                  considering different factors i.e. UE Priority Factor,
 *                  Modulation Factor,Delay Factor and Queue Load factor and 
 *                  then allocatesResource blocks to the UEs based on the value
 *                  of their respective weighted priorities.
 *****************************************************************************/
/* SPR 2446 Fix Begins*/
UInt32 ulProportionalFairScheduler(
    /*SPR 21068 start */
    tickType_t ulReferenceFlag,
    /*SPR 21068 end   */
    UInt32 * numberOfAvailableRBs_p,
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
	ULSchedulerDataElement ulScheduleData  = {0};
	UInt32 tempTotalActiveUE = listCount(nonZeroBsrList_gp[internalCellIndex]);
	ULUEContext* ueULContext_p  = PNULL;
	SortNodeT *sortNode_p = PNULL;
	LTE_LIST_NODE *node_p = PNULL;
	UInt32 i ,j, numUeRotate = 0;
	UInt32 tempTotalUE = 0;
	ULSchedulerDataNodeInfo  *tempULSchedulerDataNodeArr_p
		=  ulScheduleData.ulSchedulerDataNodeArr;
	ULUEContextInfo * ueULContextInfo_p = PNULL;
	static UInt32 bsrLoadWeightedMean_g[MAX_NUM_CELL] = {0};

	if ( !tempTotalActiveUE )
	{
		/*LTE_MAC_UT_LOG(LOG_DETAIL,UL_PF_STRATEGY,
		  "[UL-Strategy][%s]Returning because tempTotalActiveUE is ZERO\n",
		  __func__);*/
		return 0;
	}

	/* 
	 ** Calculate mean Bsrload for all UEs after every 
	 ** calcBsrloadMeanItr_g ticks
	 */
	if ( !(ulPfsGlobals_gp[internalCellIndex]->calcBsrloadMeanItr) )
	{
		if ( cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
             ulSchdConfig.uplkUEBSRLoadWt )
		{    
			/* 
			 ** calc weightedBsrLoad for each UE present in 
			 ** ulSchedulerDataNodeArr and iterate through NON
			 ** Zero BSR List containig Entries of UE with
			 ** Non-Zero BSR Queue Load 
			 */
			iterateNonZeroListsAndCalculateWeightedBSRLoad
				(&bsrLoadWeightedMean_g[internalCellIndex], &tempTotalUE , ulReferenceFlag
                ,internalCellIndex
                 /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                 ,ulSubFrameNum
#endif
                 /* TDD Config 0 Changes End */
                );
			/****CALCULATION OF Weighted Mean of BSR Load ( F_UE_QL )*****/    
			/*                     SUM [ p1*q1 + p2*q2 .....pn*qn] (of all ues')
					       bsrLoadWeightedMean = -----------------------
					       totalUEs          */
			if ( 0 < tempTotalUE )
			{
				bsrLoadWeightedMean_g[internalCellIndex] = 
					bsrLoadWeightedMean_g[internalCellIndex]/tempTotalUE;
			}
			else
			{
				return 0 ;
			}
			/*
			 ** This needs to be discussed why calcBsrloadMeanItr_g is not
			 ** equal NUM_ITER_TO_CALC_MEAN_Q_LOAD_UL
			 */
            ulPfsGlobals_gp[internalCellIndex]->calcBsrloadMeanItr = NUM_ITER_TO_CALC_MEAN_Q_LOAD_UL;
            ulPfsGlobals_gp[internalCellIndex]->bsrLoadComputed = TRUE;
		}
	}
	else
	{
		ulPfsGlobals_gp[internalCellIndex]->calcBsrloadMeanItr--;
	}

	/* 
	 ** 2. Calculate weighted priority for UEs scheduled in last tick
	 ** Since these UEs were scheduled, their priorities now are incorrect
	 */

	while ( (sortNode_p = 
				(SortNodeT * )getFirstListNode(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList))) )
	{
		ueULContext_p =  ulUECtxInfoArr_g[sortNode_p->ueIndex].ulUEContext_p;

		if ( !(ulUECtxInfoArr_g[sortNode_p->ueIndex].pendingDeleteFlag) ) 
		{
			if ( ueULContext_p )
			{
				ueULContext_p->ulSchedulerAlgoInfo.weightedPriority  =
					calculateULWeightedPriority( ueULContext_p,
							bsrLoadWeightedMean_g[internalCellIndex],
							NULL,
							currentGlobalTTITickCount,
                            internalCellIndex);
			}

		}
        listDeleteNode(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList) , (LTE_LIST_NODE *)sortNode_p);
        /*CA Changes start  */
        ulPfsFreeSortNumElem(sortNode_p, internalCellIndex);
        /*CA Changes end  */
	}
	leastPriorityUe_p[internalCellIndex] = PNULL;
	/* ICIC changes start */
	leastPriorityCEUe_p[internalCellIndex] = PNULL;
	/* ICIC changes end */
	/* 
	 ** 3. Calculate The Weighted Priority for first eligible
	 ** NUM_UES_COMPUTED_ONE_TICK_UL present in nonZeroBsrList
	 */

	node_p = getListNode(nonZeroBsrList_gp[internalCellIndex],0);

	for (j=0 , numUeRotate =0; (j < NUM_UES_COMPUTED_ONE_TICK_UL) && node_p; )
	{
		ueULContextInfo_p = &ulUECtxInfoArr_g[\
				    ((nonZeroBsrListNode*)node_p)->ueIndex];
		/* 
		 ** Null check for ueULContextInfo_p not made as it should always be non
		 ** null if valid node exists in nonZeroBsrList 
		 */
		if ( !(ueULContextInfo_p->pendingDeleteFlag) )
        {
            ueULContext_p = ueULContextInfo_p->ulUEContext_p;

            /* SPR 14402 Changes Starts */
            if(ueULContext_p->bsrNetTotal != 0)
                /* SPR 14402 Changes Ends*/
            {
                /* Checking if Already being considered for RB Distribution */
                /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                if ( ueULContext_p->isAlreadyConsideredFlag != ulReferenceFlag )
#elif TDD_CONFIG
                    if ( ueULContext_p->isAlreadyConsideredFlag[ulSubFrameNum] != 
                            ulReferenceFlag )
#endif
                        /* TDD Config 0 Changes End */
                    {
                        if ( ueULContext_p->ulSchedulerAlgoInfo.lastCompWTPriorityTick
                                != currentGlobalTTITickCount )
                        {
                            ueULContext_p->ulSchedulerAlgoInfo.weightedPriority =
                                calculateULWeightedPriority(ueULContext_p,
                                        bsrLoadWeightedMean_g[internalCellIndex],
                                        NULL,
                                        currentGlobalTTITickCount,
                                        internalCellIndex);
                        }
                        /* ICIC changes start */
                        ulPfsCheckIfNodetoBeScheduled(node_p, 
                                ueULContext_p, 
                                maxUEsToBeScheduledUL,
                                internalCellIndex);
                        /* ICIC changes end */
                        j++;

                    }
                node_p = getNextListNode(node_p);
                numUeRotate++;
            }
            /* SPR 14402 Changes Starts */
            else
            {
                node_p = getNextListNode(node_p);
                deleteEntryFromNonZeroBSRList(ueULContext_p,ueULContext_p->nonZeroBsrList_p, internalCellIndex);
            }
            /* SPR 14402 Changes Ends */
        }
		else
		{
			LTE_LIST_NODE * oldNode_p = PNULL;
			oldNode_p = getNextListNode(node_p);
			/* In case of RESET pendingDelete is also set hence don't free the node here as 
			   this will be freed from resetMACULUEEntity*/
			/*listDeleteNode(&nonZeroBsrList, node_p);
			  freeMemPool((void *)node_p);*/
			node_p= oldNode_p;
		}
	}

	/* 
	 ** 4. Find maxUEsToBeScheduledUL UEs with max priorities out of all UEs
	 */
	if ( node_p )
	{
		ulPfsCheckRemainingNodes(node_p, maxUEsToBeScheduledUL, ulReferenceFlag
                ,internalCellIndex
                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                ,ulSubFrameNum
#endif
                /* TDD Config 0 Changes End */
                );
	}

	/*
	 ** 5. Top n UEs are present in lastTickScheduledList_g. Pick these for
	 ** scheduling 
	 */
	/* Calculate the sum weighted priority UEs */
	sortNode_p = (SortNodeT *) getFirstListNode(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList));
	i = 0;
	while ( sortNode_p )
	{
		ueULContext_p = ulUECtxInfoArr_g[sortNode_p->ueIndex].ulUEContext_p;

		if ( ueULContext_p->ulSchedulerAlgoInfo.lastCompWTPriorityTick
				== currentGlobalTTITickCount )
		{
			tempULSchedulerDataNodeArr_p[i].ueULContext_p = ueULContext_p;

			tempULSchedulerDataNodeArr_p[i].weightedPriority =
				ueULContext_p->ulSchedulerAlgoInfo.weightedPriority;
			tempULSchedulerDataNodeArr_p[i].weightedBsrLoad =
				ueULContext_p->ulSchedulerAlgoInfo.weightedBsrLoad ;
		}
		else
		{
			tempULSchedulerDataNodeArr_p[i].weightedPriority =
				calculateULWeightedPriority(
						ueULContext_p,bsrLoadWeightedMean_g[internalCellIndex],
						&(tempULSchedulerDataNodeArr_p[i]),
						currentGlobalTTITickCount,
                        internalCellIndex);
		}
		tempULSchedulerDataNodeArr_p[i].ueULContext_p = ueULContext_p;
		/* Update the last scheduled TICK. */
		/* SPR 8807 changes start */
		//        ueULContext_p->lastScheduledTick = currentGlobalTTITickCount;
		/* SPR 8807 changes end */
		ulScheduleData.sumWeightedPriority +=
			tempULSchedulerDataNodeArr_p[i].weightedPriority;
		i++;

		/* update weighted priority in nonZeroBsrList;*/
		sortNode_p = (SortNodeT * )getNextListNode((LTE_LIST_NODE *)sortNode_p);
	}

	ulScheduleData.totalUE = i;
	for ( i = 0; i< numUeRotate  ;i++ )
	{
        node_p = getFirstListNode(nonZeroBsrList_gp[internalCellIndex]);
        listDeleteNode (nonZeroBsrList_gp[internalCellIndex], node_p);
        listPushNode(nonZeroBsrList_gp[internalCellIndex], node_p);
	}

    ulPfsGlobals_gp[internalCellIndex]->bsrLoadComputed = FALSE;

	LTE_MAC_UT_LOG(LOG_DETAIL,UL_PF_STRATEGY,"[UL-Strategy][%s]total\
			UE's in the scheduleQ %d \n", __func__,
			ulScheduleData.totalUE);

	/*If UEs are present in the scheduler Array,
	  Sort the UEs on the basis of the UEs Weighted Priority and the allocate 
	  RBs as per the Weighted priority*/
	if ( ulScheduleData.totalUE )
	{
		/*Allocating RBs after the Sorting of UE done*/
		/*SPR 2446 Fix Begins*/
		allocULRBsToSortedUEs(numberOfAvailableRBs_p,
				&ulScheduleData,maxUEsToBeScheduledUL,currentGlobalTTITickCount
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
		/*SPR 2446 Fix Ends*/

	}
	return MAC_SUCCESS;
}

/*************************************************************************************
 * Function Name  : calculateULWeightedPriority
 * Inputs         : ueULContext_p - UE Context of the UE for which the 
 *                                  Weighted priority needs to be calculated,
 *                  bsrLoadWeightedMean - Its Mean of Weighted BSR Load of UE,
 *                  ulSchedulerDataNodeArr_p - pointer to node which contains the 
 *                                             information related to the scheduling
 *                                             i.e number of RBs allocated, Weighted
 *                                             Priority, etc
 *                  currentGlobalTTITickCount - Current Gloabal Tick,
 *					internalCellIndex
 * Outputs        : Weighted priority of the UE
 * Returns        : weightedPriorityReturned - The Weighted priority of the UE
 *                                             After considering all the factors
 * Description    : This function calculates the weighted priority for
 *                  the UEContext passed as parameter and return the weighted 
 *                  priority.Weighted priority of a UE is calculated
 *                  considering different factors i.e. UE Priority Factor,
 *                  Modulation Factor,Delay Factor and Queue Load factor.
 *****************************************************************************/
 /* CA Changes start */
UInt32 calculateULWeightedPriority(
	ULUEContext* ueULContext_p,
	UInt32 bsrLoadWeightedMean,
	ULSchedulerDataNodeInfo *ulSchedulerDataNodeArr_p,
    /* SPR 15909 fix start */
	tickType_t currentGlobalTTITickCount,
    /* SPR 15909 fix end */
    InternalCellIndex internalCellIndex)
/* + coverity 55314 */
{
    UInt32 weightedPriorityReturned = 0;
    UInt32 fUeBSRl                  = 0;    /*Bsr load factor*/
    UInt32 fUeDelay                 = 0;    /*Delay load factor*/
    UInt32 percentUeBSRLoad         = 0;
    UInt32 weightedBsrLoad          = 0;
    UInt8  j                        = 0;

    ULSchedulerStrategyConfig *ulSchdConfig_p = & cellSpecificParams_g.\
       cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig;
    /* SPR 3083 Fix begin */
#ifdef TDD_CONFIG
    UInt8 numOfULSf;
#endif
    /* SPR 3083 Fix end */

    /*UE Priority Factor is only calculated if the UE Priority factor 
      is not calculated before*/
    if ( !ueULContext_p->ulSchedulerAlgoInfo.uePriorityFactor ) 
    {
        ueULContext_p->ulSchedulerAlgoInfo.uePriorityFactor = 
            ulSchdConfig_p->uplkUEPriorityWt * (
                    MAX_WEIGHTED_UE_PRIORITY -  ueULContext_p->uePriority);
    }

#ifdef FDD_CONFIG
    /*Delay Factor Calculation  */
    fUeDelay = (ulSchdConfig_p->uplkUEDelayWt) *
        (currentGlobalTTITickCount - 
         ueULContext_p->lastScheduledTick);
    /* SPR 3083 Fix begin */
#elif TDD_CONFIG
    numOfULSf = tddNumOfULSf[cellSpecificParams_g.
       cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->subFrameAssign];

    /*Delay Factor Calculation based on the number of UL SF in
     * a radio frame 
     */
    fUeDelay = (ulSchdConfig_p->uplkUEDelayWt) *
        (((currentGlobalTTITickCount - 
           ueULContext_p->lastScheduledTick) * numOfULSf) / 10);
#endif
    /* SPR 3083 Fix end */

    /**********************Bsr Load Factor***********************
      If the Weighted Bsr Load mean is present, Then the UE Bsr 
      Load Factor need to be calculated*/
    if ( bsrLoadWeightedMean )
    {
        if ( ulPfsGlobals_gp[internalCellIndex]->bsrLoadComputed )
        {
            weightedBsrLoad = ueULContext_p->ulSchedulerAlgoInfo.weightedBsrLoad;
        }
        else
        {
            for (j = NUM_OF_LCG; j-- ;)
            {
                if ( (ueULContext_p->lcCount[j] != INVALID_LCID)
                        && (ueULContext_p->bsrNet[j]) )
                {

                    weightedBsrLoad +=  ((ulSchdConfig_p->uplkLCGPriorityWt)
                            *(MAX_WEIGHTED_LCG_PRIORITY - j)
                            * (ueULContext_p->bsrNet[j]));
                }
            }


            if( ulSchedulerDataNodeArr_p )
            {
                ulSchedulerDataNodeArr_p->weightedBsrLoad = weightedBsrLoad;
            }
        }

        percentUeBSRLoad = 
            (weightedBsrLoad * 100) /bsrLoadWeightedMean;

        if ( percentUeBSRLoad > 1 )
        {
            LTE_MAC_UT_LOG(LOG_INFO,UL_PF_STRATEGY,
                    "[UL-Strategy][%s]Queue Load Factor(>1) is = %f",
                    __func__,(ulSchdConfig_p->uplkUEBSRLoadWt)*
                    ((((UDouble32)logint(percentUeBSRLoad)))/1024));


            fUeBSRl = (UInt32 )(ulSchdConfig_p->uplkUEBSRLoadWt)*
                ((((UDouble32)logint(percentUeBSRLoad)))/1024);
        }
        else
        {
            fUeBSRl = ulSchdConfig_p->uplkUEBSRLoadWt;
        }
    }
    /*****************************************************************/

    /**********************Modulation Scheme Factor*******************/
    /*SPR 6424 Fix Start*/
    if ( ueULContext_p->modulationScheme == QPSK )
    {
        ueULContext_p->ulSchedulerAlgoInfo.modulationSchemeFactor  =
            (ulSchdConfig_p->uplkMSWt) * 
            (ulSchdConfig_p->ulMsQPSKFactorWt );
    }
    else if ( ueULContext_p->modulationScheme  == SIXTEENQAM )
    {
        ueULContext_p->ulSchedulerAlgoInfo.modulationSchemeFactor  =
            (ulSchdConfig_p->uplkMSWt) * (ulSchdConfig_p->ulMs16QAMFactorWt);
    }
    else if ( ueULContext_p->modulationScheme == SIXFOURQAM )
    {
        ueULContext_p->ulSchedulerAlgoInfo.modulationSchemeFactor  =
            (ulSchdConfig_p->uplkMSWt) * (ulSchdConfig_p->ulMs64QAMFactorWt);
    }
    /*SPR 6424 Fix End*/
    /***********************************************************************/
    LTE_MAC_UT_LOG(LOG_DETAIL,UL_PF_STRATEGY,"[%s]UE Index %d,Global tick%d\n"
            "UE Priority:%d,UE Priority Weight:%d, UE Priority Factor:%d\n"
            "Last Scheduled Tick:%d, UE Delay Weight:%d,UE Delay Factor:%d\n"
            "UE Weighted BSR Load:%d,BSR Load Weight:%d,BSR Load Factor:%d\n"
            "UE modulation Scheme:%d,MS Weight:%d,Mod Scheme Factor:%d\n"
            "Weighted Priority of UE:%d\n",__func__,ueULContext_p->ueIndex,
            currentGlobalTTITickCount,ueULContext_p->uePriority,
            ulSchdConfig_p->uplkUEPriorityWt,
            ueULContext_p->ulSchedulerAlgoInfo.uePriorityFactor,
            ueULContext_p->lastScheduledTick,ulSchdConfig_p->uplkUEDelayWt,
            fUeDelay,bsrLoadWeightedMean,ulSchdConfig_p->uplkUEBSRLoadWt,fUeBSRl,
            ueULContext_p->modulationScheme,ulSchdConfig_p->uplkMSWt,
            ueULContext_p->ulSchedulerAlgoInfo.modulationSchemeFactor,
            ueULContext_p->ulSchedulerAlgoInfo.uePriorityFactor+
            ueULContext_p->ulSchedulerAlgoInfo.modulationSchemeFactor+fUeBSRl 
            +fUeDelay);

    ueULContext_p->ulSchedulerAlgoInfo.lastCompWTPriorityTick =  
        currentGlobalTTITickCount;
    /*****************WEIGHTED PRIORITY CALCULATION************************/

    weightedPriorityReturned = ueULContext_p->ulSchedulerAlgoInfo.uePriorityFactor
        + ueULContext_p->ulSchedulerAlgoInfo.modulationSchemeFactor +
        fUeBSRl +
        fUeDelay ;

    /***********************************************************************/
    /* SPR 11786 fix start */
    if(!weightedPriorityReturned)
    {
        weightedPriorityReturned = 1;
    }
    /* SPR 11786 fix end */

    return weightedPriorityReturned;
}
 /* CA Changes end */
/* - coverity 55314 */

/*****************************************************************************
 * Function Name  : iterateNonZeroListsAndCalculateWeightedBSRLoad 
 * Inputs         : bsrLoadWeightedMean_p - pointer to the weighted BSR Load 
 *                                          Mean,
 *                  totalUECount - Total UE Count,
 *                  referenceFlag - Used for checking whether the US is
 *                                  considered already or not. If UE's
 *                                  rbsReservedForUE is equalto referenceFlag,
 *                                  that means the UE is already considered,
 *					internalCellIndex
 *                  ulSubFrameNum - uplink subframe nuber for which grant is
 *                                  being given.
 * Outputs        : It calculate the Wegted BSR Load of all the UE present 
 *                  in Non- Zero BSR List
 * Returns        : None
 * Description    : This function iterates through non zero BSR load list and 
 *                  calculates their weighted BSR load.
 ******************************************************************************/
void iterateNonZeroListsAndCalculateWeightedBSRLoad(
    UInt32 *bsrLoadWeightedMean_p,
    UInt32 *totalUECount,
    /* SPR 21068 start */
    tickType_t referenceFlag
    /* SPR 21068 end   */
    ,InternalCellIndex internalCellIndex
    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
    ,UInt8 ulSubFrameNum
#endif
    /* TDD Config 0 Changes End */
    )
{
    LTE_LIST_NODE * node_p      = PNULL;
    LTE_LIST_NODE * oldNode_p   = PNULL;
    ULUEContext* ueULContext_p  = PNULL;  
    ULUEContextInfo * tempUplinkContextInfo_p=PNULL;
    UInt32 ueIndex;
    UInt32 numLC                = 0;	
    UInt32 j                    = 0;
    UInt32 tempWeightedBsrLoad  = 0;
    UInt32 weightedBsrLoad      = 0;
    /* SPR 11786 fix start  */	
    UInt32 tempBsrNet		= 0;
    /* SPR 11786 fix end  */	
    DLUEContextInfo * ueDLContextInfo_p = PNULL;
    DLUEContext* ueDLContext_p       = PNULL;  

    /*CA Changes start  */
    node_p = getListNode(nonZeroBsrList_gp[internalCellIndex],0);
    /*CA Changes end  */
    while (node_p)
    {
        ueIndex = ((nonZeroBsrListNode *)node_p)->ueIndex;
        tempUplinkContextInfo_p = &ulUECtxInfoArr_g[ueIndex];      
        ueDLContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        if (!(tempUplinkContextInfo_p->pendingDeleteFlag))
        {
            ueULContext_p = tempUplinkContextInfo_p->ulUEContext_p;
            ueDLContext_p = ueDLContextInfo_p->dlUEContext_p;
            /* Checking if Already being considered for RB Distribution */
            if (
                    /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
                (ueULContext_p->isAlreadyConsideredFlag != referenceFlag)
#elif TDD_CONFIG
                (ueULContext_p->isAlreadyConsideredFlag[ulSubFrameNum] 
                != referenceFlag)
#endif
                /* TDD Config 0 Changes End */
                    && (UL_SYNC_LOSS_OFF == 
                        ueDLContext_p->ulSyncLossOrHOInfoInDL.ulSyncLossState))    
            {
                weightedBsrLoad = 0;
                /* SPR 11786 fix start  */	
                tempBsrNet      = 0;
                /* SPR 11786 fix end  */	
                numLC = 0;
                /* Calculation of the Weighted BSR Load */
                /* Searching for BSR load in all the Logical channel groups */
                for ( j=0; j < NUM_OF_LCG ; j++)        
                {
                    if (((ueULContext_p->bsrNet[j])) &&
                            ((ueULContext_p->lcCount[j])!= 0))
                    {
                        LTE_MAC_UT_LOG(LOG_DETAIL,UL_PF_STRATEGY,
                                "[UL-Strategy][%s]UE ID:%d UL LCG Priority:%d\
                                LCG %d 's BSR Net is: %d\n",__func__,ueIndex,
                                (MAX_WEIGHTED_LCG_PRIORITY-j),j,
                                ueULContext_p->bsrNet[j]);

                        /*
                         ** LCG priority calculated after multiplying it with 
                         ** its LCG Priority Weight & Calculation of Weighted
                         ** BSR Load of particular LCG
                         */

                        /* SPR 11786 fix start  */
                        tempBsrNet += ueULContext_p->bsrNet[j];
                        /* SPR 11786 fix end  */
                        tempWeightedBsrLoad = 
                            ((cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                              initParams_p->ulSchdConfig.uplkLCGPriorityWt)*
                             (MAX_WEIGHTED_LCG_PRIORITY-j))*
                            (ueULContext_p->bsrNet[j]);     

                        LTE_MAC_UT_LOG(LOG_DETAIL,UL_PF_STRATEGY,
                                "[UL-Strategy][%s]tempWeightedBsrLoad %d\n",
                                __func__,tempWeightedBsrLoad);

                        weightedBsrLoad += tempWeightedBsrLoad;
                        numLC = numLC + ueULContext_p->lcCount[j];
                        if (numLC  == ueULContext_p->numLogicalChannels)
                        {
                            break;
                        }

                        /*num of Non active Logical channels in non Zero LCG*/
                        /*LTE_MAC_UT_LOG(LOG_DETAIL,UL_PF_STRATEGY,
                          "[UL-Strategy][%s]Cum weightedBsrLoad:%f\n",
                          __func__,weightedBsrLoad);*/

                        /* p_lc1 * ql_lc1 + p_lc2 * ql_lc2 of each UE*/
                    }
                    else
                    {
                        LTE_MAC_UT_LOG(LOG_DETAIL,UL_PF_STRATEGY,
                                "[UL-Strategy][%s]Either bsrNet\
                                or LC Count is Zero for LCG %d of UE ID %d\n",
                                __func__,j,ueIndex);
                    }
                }

                ueULContext_p->ulSchedulerAlgoInfo.weightedBsrLoad =
                    weightedBsrLoad;

                /* 
                 ** Add the weightedQueueLoad calculate to 
                 ** bSRLoadWeightedMean_g 
                 */
                *bsrLoadWeightedMean_p += weightedBsrLoad;

                /* SPR 10149 changes start */ 
                node_p = getNextListNode(node_p);
                /* SPR 10149 changes end */ 

                /* SPR_8524_FIX_START */
                /* SPR 11786 fix start  */	
                if(!tempBsrNet)
                {
                    /* SPR 11786 fix end  */	
                    deleteEntryFromNonZeroBSRList(ueULContext_p,ueULContext_p->nonZeroBsrList_p, internalCellIndex);
                }
                else
                {
                    (*totalUECount)++;
                }
                /* SPR_8524_FIX_END */
                if ( (*totalUECount) > 
                        MAX_UL_SCH_ARR * MAX_ADVANCE_UL_SUBFRAMES_ALLOC )
                {
                    (*totalUECount)--;
                    break;	
                }

            }
            /*end of already considered flag check, The UE which are
              already considered will be deleted automatically */
            else
            {
                LTE_MAC_UT_LOG(LOG_INFO,UL_PF_STRATEGY,
                        "[UL-Strategy][%s]UE is already considered,"
                        " hence NOT  considering UE ID:%d",__func__,ueIndex);
                node_p = getNextListNode(node_p);
            }
        }
        else
        {
            /*delete the node from non ZERO BSR List and dont consider
              this UE goto next UE in the non Zero BSR LIST*/
            oldNode_p = getNextListNode(node_p);
            LTE_MAC_UT_LOG(LOG_DETAIL,UL_PF_STRATEGY,
                    "[UL-Strategy][%s] UE ID:%d's Context is NULL or "
                    "Pending to delele\n",__func__,ueIndex);
            /* In case of RESET pendingDelete is also set hence don't free the node here as 
               this will be freed from resetMACULUEEntity*/
            /* listDeleteNode(&nonZeroBsrList, node_p);
               freeMemPool((void *)node_p);*/
            node_p = oldNode_p;
        }
    }
    return;
}

/*****************************************************************************
 * Function Name  : allocULRBsToSortedUEs
 * Inputs         : numberOfAvailableRBs_p - Pointer to total RBs to be 
 *                                           distributed,
 *                  ulScheduleData_p - pointer to the array contains
 *                                     weightedPriority and UE index of all
 *                                     UEs with non zero BSR load
 *                  maxUEsToBeScheduledUL - indicates the maximum number of
 *                                          UEs to be  scehduled,
 *                  currentGlobalTTITickCount: Current Global Tick,
 *                  ulDelay - UL Delay,
 *					internalCellIndex
 * Outputs        : Allocate RBs to Sorted UE based on Weighted priority
 * Returns        : MAC_SUCCESS
 * Description    : This function sorts the ulScheduleData_p array based on their
 *                  weightedPriority and then assigns RBs based on their 
 *                  weightedPriorities.
 *****************************************************************************/
/*SPR 2446 Fix Begins*/
UInt32 allocULRBsToSortedUEs (
	UInt32 *numberOfAvailableRBs_p,
	ULSchedulerDataElement *ulScheduleData_p,
    UInt8 maxUEsToBeScheduledUL,
    	/* SPR 15909 fix start */
	tickType_t currentGlobalTTITickCount
	/* SPR 15909 fix end */
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
/*SPR 2446 Fix Ends*/
{
    /* +- SPR 17777 */
#ifdef TDD_CONFIG    
    UInt16 sysFrameNum = 0;
    /* +- SPR 18490 */
    UInt8 subframeNum = 0;
    /* TDD Config 6 Changes Start */
    GET_SFN_SF_FROM_TTI(currentGlobalTTITickCount, sysFrameNum, subframeNum)
    LTE_GCC_UNUSED_PARAM(sysFrameNum)
    /* +- SPR 17777 */
    /* TDD Config 6 Changes End */
#endif
    /* +- SPR 18490 */
    UInt32 ueCount = 0;
    UInt8 ulHarqProcessId=0;
    UInt32 userCount = 0;
    UInt32 ueToSchedule = 0;
    UInt32 availableRB  = numberOfAvailableRBs_p[CC_CE_REGION];
    /* SPR 2261 changes start */
    UInt32 sumWeightedPriority[MAX_USER_LOCATION]  = {0};
    /* SPR 2261 changes end */
    ULUEContext* ueULContext_p      = PNULL;  
    ULSchedulerDataNodeInfo *schedulerInfoArr_p =PNULL;		    
    UInt32 tempArrTotalUE           = ulScheduleData_p->totalUE;
    UInt32 finalCount               = 0;
    /* ICIC changes start */
    UInt32 tempRBCount = 0;
    UInt8 ueLocationType = CC_USER;
    UInt8 numOfUeToSchedule = 0;
    /* ICIC changes end */

    /* +DYNAMIC_ICIC */
    UInt16  numOfUE = 0;
    UInt16  loopCount = 0;
    /* -DYNAMIC_ICIC */
    // SPR 3486  - START
    UInt8 minRb = 0; 
    // SPR 3486  - END
    ULUserListPFS ulUserListPFS[MAX_USER_LOCATION];
    ulUserListPFS[CC_USER].count = 0;
    ulUserListPFS[CE_USER].count = 0;
    /*For TTIB_Code */
#ifdef FDD_CONFIG  
    UInt8 ttiMapId1 = TTI_MAP_FREE;
    UInt8 ttiMapId2 = TTI_MAP_FREE;
#endif    

    LTE_MAC_UT_LOG(LOG_DETAIL,UL_PF_STRATEGY,
            "[UL-Strategy][%s]Entry : totalRBs to Schedule %d,"
            "Total UE to be schedule in Scheduler Arr = %d\n",
            __func__,availableRB,tempArrTotalUE);

    // SPR 3486  - START
    /*CA Changes start  */
    minRb = (MAC_SCH_MIN_UL_GRANT > MIN_NUM_RB_FOR_PL_PHR_CALC(internalCellIndex))?\
            MAC_SCH_MIN_UL_GRANT : MIN_NUM_RB_FOR_PL_PHR_CALC(internalCellIndex);
    /*CA Changes end  */
    // SPR 3486  - END
    //
    /* calculating max number of users that can be scheduled using minRB */
    numOfUeToSchedule = maxUEsToBeScheduledUL / minRb;
    if(maxUEsToBeScheduledUL > numOfUeToSchedule)
    {
        maxUEsToBeScheduledUL = numOfUeToSchedule;
    }

    /*Iterate through sorted ulScheduleData_p array & assign RBs to each UE*/	
    schedulerInfoArr_p = ulScheduleData_p->ulSchedulerDataNodeArr;

    if ( maxUEsToBeScheduledUL > tempArrTotalUE)
    {
        finalCount = tempArrTotalUE;
    }
    else
    {
        finalCount = maxUEsToBeScheduledUL;
    }

    /*If there are no more RBs available to distribute*/
    /* ULA_UTP */
    // SPR 3486  - START
    if (availableRB < minRb)
        // SPR 3486  - END
        /* ULA_UTP */
    {
        return MAX_ADVANCE_UL_SUBFRAMES_ALLOC;
    }
    // SPR 3486  - END
    /* +DYNAMIC_ICIC */
    numOfUE = ulScheduleInfo_gp[internalCellIndex]->numOfUe;
    if(numOfUE)
    {
        while(finalCount && numOfUE)
        {
            ueToSchedule = ulScheduleInfo_gp[internalCellIndex]->ulUeSchedulerInfoArr[loopCount].ueIndex;
            ueULContext_p = ulUECtxInfoArr_g[ueToSchedule].ulUEContext_p;
            ueLocationType = ueULContext_p->userLocationType;
            /*CA Changes start  */
            if(CE_USER == ueLocationType && !maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex])
            /*CA Changes end  */
            {
                /* If max CE users have been scheduled then continue */
                continue;
            }
#ifdef TDD_CONFIG
            ulHarqProcessId = BUSY_ALL_HARQ;
            
            /* TDD Config 0 and 6 Changes Start */
            /* Get the free ID in ulHarqProcessId 
               if all the harq id is busy the skip this node and goto the next node in 
               the list and start this procedure again.
               */
            /* SPR 11257 Changes Start*/
            ulHarqProcessId = getFreeULHarqId(ueULContext_p, subframeNum,
                    /* +- SPR 17777 */
					internalCellIndex,ulSubFrameNum,currentGlobalTTITickCount);
            /* +- SPR 17777 */
            /* SPR 11257 Changes End*/
            /* TDD Config 0 and 6 Changes End */

#elif FDD_CONFIG
            ulHarqProcessId = MODULO_EIGHT(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
        /* TTIB_Code Start */
            if ( ueULContext_p->ttiBundlingEnabled )
            {
                ulHarqProcessId = GET_TTIB_HARQ_ID( currentGlobalTTITickCount +ulDelay + FDD_HARQ_OFFSET );    
            }
            ttiMapId1 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
            ttiMapId2 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET +
                        MAX_HARQ_PROCESS_NB);
        /* TTIB_Code End */
#endif

#ifdef TDD_CONFIG 
            if (BUSY_ALL_HARQ == ulHarqProcessId) 
                /* TX_RETX */            
#elif FDD_CONFIG
                if((HARQ_PROCESS_FREE != ueULContext_p->ulresInfo[ulHarqProcessId].harqStatus)
                    /* TX_RETX */            
        /* TTIB_Code Start */
                    ||(ueULContext_p->ttiBundlingEnabled &&
                      (MAC_FAILURE == processTtiBHarqCheck(currentGlobalTTITickCount,ulDelay,ueULContext_p)))
                    ||(!(ueULContext_p->ttiBundlingEnabled)&&
                      ((TTI_MAP_FREE != ueULContext_p->ttiHarqMap[ttiMapId1]) ||
                      ( TTI_MAP_FREE != ueULContext_p->ttiHarqMap[ttiMapId2])))
                    /* + SPS_TDD_Changes */
                    /*SPR 7944 START*/
                    /*
                    || checkSpsOccasionTti(ueULContext_p,ulHarqProcessId)
                    || checkSpsActTti(ueULContext_p,ulHarqProcessId)*/
                    /*SPR 7944 END*/
                    /* - SPS_TDD_Changes */
        /* TTIB_Code End */
                  )
#endif
                {
                    continue;
                }
            /* Harq process has been allocated */

            sumWeightedPriority[ueLocationType] += 
            ueULContext_p->ulSchedulerAlgoInfo.weightedPriority;
            userCount = ulUserListPFS[ueLocationType].count;
            ulUserListPFS[ueLocationType].ulSchedulableUENode[userCount]. \
                indexToDlSchedulerDataElement = ueToSchedule;
            ulUserListPFS[ueLocationType].ulSchedulableUENode[userCount]. \
                harqProcessId = ulHarqProcessId;
            ulUserListPFS[ueLocationType].count++;	
            if(CE_USER == ueLocationType)
            {
                /*CA Changes start  */
                maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]--;
                /*CA Changes end  */
            }
            finalCount--;
            numOfUE--;
            loopCount++;
        }
    }

    /* -DYNAMIC_ICIC */
    /* Distrubute the RBs for selected UEs */
    for (ueToSchedule = 0; (ueCount < finalCount)
                 && (ueToSchedule < tempArrTotalUE); ueToSchedule++)
    {
        LTE_MAC_UT_LOG(LOG_INFO,UL_PF_STRATEGY,
                "[UL-Strategy][%s]Count of Schedule UE :%d\n",
                __func__,ueToSchedule);

        ueULContext_p = schedulerInfoArr_p[ueToSchedule].ueULContext_p;

        /* ICIC changes start */
        ueLocationType = ueULContext_p->userLocationType;
        /* \code
         * if schType == SPS_SCHEDULING, then do not schedule this UE.
         * */
        if( SPS_SCHEDULING == ueULContext_p->schType )
        {
           continue;
        }
        /* ICIC changes end */
#ifdef TDD_CONFIG
        /*cov 85811 +-*/ 
        /* TDD Config 0 and 6 Changes Start */
        /* Get the free ID in ulHarqProcessId 
           if all the harq id is busy the skip this node and goto the next node in 
           the list and start this procedure again.
           */
        /* SPR 11257 Changes Start*/
        ulHarqProcessId = getFreeULHarqId(ueULContext_p, subframeNum,
                /* +- SPR 17777 */
					internalCellIndex,ulSubFrameNum, currentGlobalTTITickCount);
        /* +- SPR 17777 */
        /* SPR 11257 Changes End*/
        /* TDD Config 0 and 6 Changes End */

#elif FDD_CONFIG
        ulHarqProcessId = MODULO_EIGHT(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
        /* TTIB_Code Start */
            if ( ueULContext_p->ttiBundlingEnabled )
            {
                ulHarqProcessId = GET_TTIB_HARQ_ID( currentGlobalTTITickCount +ulDelay + FDD_HARQ_OFFSET );    
            }
            ttiMapId1 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET);
            ttiMapId2 = MODULO_SIXTEEN(currentGlobalTTITickCount + ulDelay + FDD_HARQ_OFFSET +
                        MAX_HARQ_PROCESS_NB);
        /* TTIB_Code End */
#endif

#ifdef TDD_CONFIG
        if (BUSY_ALL_HARQ == ulHarqProcessId)
        /* TX_RETX */
#elif FDD_CONFIG
            if((HARQ_PROCESS_FREE != ueULContext_p->ulresInfo[ulHarqProcessId].harqStatus)
            /* TX_RETX */
        /* TTIB_Code Start */
                 ||(ueULContext_p->ttiBundlingEnabled &&
                   (MAC_FAILURE == processTtiBHarqCheck(currentGlobalTTITickCount,ulDelay,ueULContext_p)))
                 ||(!(ueULContext_p->ttiBundlingEnabled)&&
                   ((TTI_MAP_FREE != ueULContext_p->ttiHarqMap[ttiMapId1]) ||
                   ( TTI_MAP_FREE != ueULContext_p->ttiHarqMap[ttiMapId2])))
        /* TTIB_Code End */
            /* + SPS_TDD_Changes */
        /*SPR 7944 START*/
                 /*
        || checkSpsOccasionTti(ueULContext_p,ulHarqProcessId)
        || checkSpsActTti(ueULContext_p,ulHarqProcessId)*/
        /*SPR 7944 END*/
            /* - SPS_TDD_Changes */

              )
#endif
        {
            continue;
        }
        /* ICIC changes start */
        /* Harq process has been allocated */

        sumWeightedPriority[ueLocationType] += 
            schedulerInfoArr_p[ueToSchedule].weightedPriority;
        userCount = ulUserListPFS[ueLocationType].count;
        ulUserListPFS[ueLocationType].ulSchedulableUENode[userCount]. \
            indexToDlSchedulerDataElement = ueToSchedule;
        ulUserListPFS[ueLocationType].ulSchedulableUENode[userCount]. \
            harqProcessId = ulHarqProcessId;
        ulUserListPFS[ueLocationType].count++;	
        ueCount++;    
    }
    /* ICIC changes  end*/

    /* ICIC changes start */
    /* Processing CE and CC user list one by one */
    userCount = ulUserListPFS[CE_USER].count;
    if(userCount)
    {
        /* Resetting unUsedRBUL_g[internalCellIndex] */
        unUsedRBUL_g[internalCellIndex] = 0;

        tempRBCount = numberOfAvailableRBs_p[CE_USER];
        processULSchedulableListPFS(&ulUserListPFS[CE_USER],
                ulScheduleData_p,
                &numberOfAvailableRBs_p[CE_USER],
                minRb,
                currentGlobalTTITickCount,
                sumWeightedPriority
#ifdef FDD_CONFIG
                ,ulDelay
                /* SPR 11978 Fix Start */
#endif
                /* SPR 11978 Fix End */
                ,internalCellIndex
                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                ,ulSubFrameNum
#endif
                /* TDD Config 0 Changes End */
                );

        /* Updating CC_CE_REGION RB count after allocating resources
         * to all of CE users*/
        numberOfAvailableRBs_p[CC_CE_REGION] -= 
            tempRBCount - numberOfAvailableRBs_p[CE_USER];
    } 

    userCount = ulUserListPFS[CC_USER].count;
    if(userCount)
    {
        /* As unUsedRBUL_g[internalCellIndex] for Cell Edge RBs is already added to initial
         * available RB for Cell Center. Hence resetting this to 0 */
        unUsedRBUL_g[internalCellIndex] = 0;

        processULSchedulableListPFS(&ulUserListPFS[CC_USER],
                ulScheduleData_p,
                &numberOfAvailableRBs_p[CC_USER],
                minRb,
                currentGlobalTTITickCount,
                sumWeightedPriority
#ifdef FDD_CONFIG
                ,ulDelay
                /* SPR 11978 Fix Start */
#endif
                ,internalCellIndex
                /* SPR 11978 Fix Start */
                /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                ,ulSubFrameNum
                /* TDD Config 0 Changes End */
                /* SPR 11978 Fix End */
#endif
                );

        /* Updating CE Region RB count*/
        if(numberOfAvailableRBs_p[CC_CE_REGION] < 
                numberOfAvailableRBs_p[CE_REGION])
        {
            numberOfAvailableRBs_p[CE_REGION] = 
                numberOfAvailableRBs_p[CC_CE_REGION];
        }
    }

    /* +- SPR 17777 */
    LOG_MAC_MSG (UL_MAX_UE_SCHEDULED_AND_AVAILABLE_RB_PER_TICK, LOGBRIEF,
                   MAC_DL_Strategy, getCurrentTick (),
                   ueToSchedule,
                   numberOfAvailableRBs_p[CC_CE_REGION],
#ifdef TDD_CONFIG
                   sysFrameNum,
#else
                   0,
#endif
                   0,0,
                   DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, " ");
    return MAC_SUCCESS;
}

/************************************************************************************
 * Function Name  : assignULRBsAndSendToPDCCH
 * Inputs         : ueIndex - UE Index for which the processing needs to be done,
 *                  allocatedRB - The Number of RBs Allocated Orginally,
 *                  numberOfAvailableRBs_p - Pointer to the number of Available RBs,
 *                  currentGlobalTTITickCount - Global Tick Count,
 *                   strictAllocRBFlag - Its is to tell whether the RB are strictly
 *                                       assigned,
 *                  ulHarqProcessId - UL HARQ Process ID,
 *                  minDataSize - Min Data Size,
 *                  maxDataSize - Max Data Size,
 *                  avgMcs - Average MCS,
 *				 	internalCellIndex	
 * Outputs        : UL Startegy Node to PDCCH Module
 * Returns        : MAC_FAILURE/MAC_SUCCESS
 * Description    : The UL Strategy Node is been prepared an provided 
 *                  to PDCCH module.
 ***********************************************************************************/
#ifdef TDD_CONFIG
MacRetType  assignULRBsAndSendToPDCCH( 
        UInt32 ueIndex, 
        UInt32 allocatedRB,
        UInt32 * numberOfAvailableRBs_p,        
        /* SPR 15909 fix start */
        tickType_t currentGlobalTTITickCount,
        /* SPR 15909 fix end */
        UInt32 strictAllocRBFlag,
        UInt8 ulHarqProcessId
        /* SPR 11978 Fix Start */
        /* TDD Config 0 Changes Start */
        ,UInt8 ulSubFrameNum
        /* TDD Config 0 Changes End */
        /* SPR 11978 Fix End */
        /* ULA_CHG */
        ,UInt32 minDataSize,
        UInt32 maxDataSize,
        UInt8 avgMcs
        /* ULA_CHG */
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */
        )
#elif FDD_CONFIG	
MacRetType  assignULRBsAndSendToPDCCH( 
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
        UInt8 avgMcs
        /* ULA_CHG */
        /* For TTIB_Code */
        ,UInt8 ttiBundling
        /*CA Changes start  */
        ,InternalCellIndex internalCellIndex
        /*CA Changes end  */
        )
#endif	
{
    ULStrategyTxNode *ulStrategyTxNode_p = PNULL;

    /*Preparing the UL Strategy outPut Node*/
#if defined(DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)
    GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(ulStrategyTxNode_p, ULStrategyTxNode);
#else
    GET_MEM_FROM_POOL(ULStrategyTxNode,ulStrategyTxNode_p,sizeof(ULStrategyTxNode),PNULL);
#endif


    /* + coverity 24547 */
    if (PNULL == ulStrategyTxNode_p)
    {
        return MAC_FAILURE;
    }
    /* - coverity 24547 */

    ulStrategyTxNode_p->ueIndex =  (UInt16)ueIndex;
    ulStrategyTxNode_p->requiredRB = allocatedRB;

#ifdef FDD_CONFIG
    if( ttiBundling )
    {    
        ulStrategyTxNode_p->ulGrantRequestType = NEW_TTIB_ULGRANT_REQ_MSG;
    }    
    else
#endif
    {    
        ulStrategyTxNode_p->ulGrantRequestType = NEW_ULGRANT_REQ_MSG;
    }

    ulStrategyTxNode_p->strictAllocRB = strictAllocRBFlag;

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
    *numberOfAvailableRBs_p -= allocatedRB;
    /* ICIC changes end */

    /*putEntry of ULStrategyTXNode in PDCCH Queue */
    /* ULA_CHG */
    /*CA Changes start  */
    if (!(putEntryInULSchedulerNewTxQueue(
                    ulStrategyTxNode_p
                    ,internalCellIndex

                    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
                    ,currentGlobalTTITickCount
                    /* SPR 11978 Fix Start */
                    ,ulSubFrameNum
                    /* SPR 11978 Fix End */
                    ,ulUECtxInfoArr_g[ueIndex].ulUEContext_p
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

/*****************************************************************************
 * Function Name  : ulSwap
 * Inputs         : data- scheduler Data,
 *                  dex1- element one,
 *                  dex2- element two
 * Outputs        : Swap of the two Elements
 * Returns        : None
 * Description    : swaps two elements.
 *****************************************************************************/
void ulSwap(
        ULSchedulerDataNodeInfo data[],
        UInt32 dex1, 
        UInt32 dex2)
{ 
	ULSchedulerDataNodeInfo temp = data[dex1];
    data[dex1]= data[dex2];
    data[dex2]= temp;
}

/*****************************************************************************
 * Function Name  : medianOf3
 * Inputs         : data - array to be sorted,
 *                  left - start index of array to be sorted,
 *                  right - end index of array to be sorted
 * Outputs        : It calculates median value
 * Returns        : median value
 * Description    : Called by ulRecQuickSort to select a pivot element.
 *****************************************************************************/
UInt32 medianOf3(
        ULSchedulerDataNodeInfo data[],
        UInt32 left, 
        UInt32 right) 
{
    UInt32 center = (left + right) / 2;
    /*order left & center*/
    if (data[left].weightedPriority < data[center].weightedPriority)
      ulSwap(data,left, center);
    /* order left & right*/
    if (data[left].weightedPriority < data[right].weightedPriority)
      ulSwap(data,left, right);
    /* order center & right*/
    if (data[center].weightedPriority < data[right].weightedPriority)
      ulSwap(data,center, right);

    /*put pivot on right */
    ulSwap(data,center, right - 1);

    /*return median value*/
    return data[right - 1].weightedPriority; 
}



/*****************************************************************************
 * Function Name  : ulPartitionIt
 * Inputs         : data - array to be sorted,
 *                  left - start index of array to be sorted,
 *                  right - end index of array to be sorted,
 *                  pivot - for pivot location
 * Outputs        : Finding out the Pivot position
 * Returns        : pivot location
 * Description    : Called by ulRecQuickSort to fix position of one element.
 *****************************************************************************/
UInt32 ulPartitionIt(
        ULSchedulerDataNodeInfo data[],
        UInt32 left, 
        UInt32 right, 
        UInt32 pivot)
{
    /* right of first elem*/
    UInt32 leftPtr = left; 
    /* left of pivot*/
    UInt32 rightPtr = right - 1; 

    while (1) 
    {
      /*  find bigger*/
      while (data[++leftPtr].weightedPriority > pivot)
        ;
      /*find smaller*/
      while (data[--rightPtr].weightedPriority < pivot)
        ;
      /*if pointers cross, partition done*/
      if (leftPtr >= rightPtr) 
      {
        break;    
      }
      else
      {
      /* not crossed, so ulSwap elements */
        ulSwap(data,leftPtr, rightPtr); 
      }
    }
    /* restore pivot*/
    ulSwap(data,leftPtr, right - 1); 
    /* return pivot location*/
    return leftPtr; 
}


/*****************************************************************************
 * Function Name  : ulManualSort
 * Inputs         : data - array to be sorted,
 *                  left - start index of array to be sorted,
 *                  right - end index of array to be sorted
 * Outputs        : Sorts the input array
 * Returns        : None
 * Description    : It does the sorting of the array from the start and and of 
*                   array index.
 *****************************************************************************/
void ulManualSort(
        ULSchedulerDataNodeInfo data[],
        UInt32 left, 
        UInt32 right)
{
    UInt32 size = right - left + 1;

    /*no sort necessary */
    if (size <= 1)
    {
        return; 
    }
    /*2-sort left and right */
    if (size == 2) 
    {
        if (data[left].weightedPriority < data[right].weightedPriority)
        {
            ulSwap(data,left, right);
        }
        return;
    }
    /*when size is 3 */
    else // size is 3
    { 
        if (data[left].weightedPriority < data[right - 1].weightedPriority)
        {
            /*left, center */
            ulSwap(data,left, right - 1);
        }
        if (data[left].weightedPriority < data[right].weightedPriority)
        {
            /* left, right */
            ulSwap(data,left, right);
        }
        if (data[right - 1].weightedPriority < data[right].weightedPriority)
        {
            /*  center, right */
            ulSwap(data,right - 1, right);      
        }
    }
}

/**********************************************************************************
 * Function Name  : ulPfsCheckIfNodetoBeScheduled 
 * Inputs         : node_p - Pointer to LTE_LIST_NODE and node of nonZeroBsrList,
 *                  ueULContext_p - Pointer to ULUEContext,
 *                  maxUeToBeSorted - Max UE's to be Sorted,
 *				 	internalCellIndex	
 * Outputs        : None 
 * Returns        : None 
 * Description    : This func prepares a list with first n elements as 
 *                  the top n. n is same as the max UEs to be scheduled. 
 *                  The min of top n is tracked. Any new element is checked 
 *                  against this min (maxUEsToBeScheduledNode.weightedPriority). 
 *                  Attempt at sorting is done only if the new element is larger,
 *                  else ignore. This further optimizes sorting of top n
 *                  Note that only CE UEs having highest Priorities and equal 
 *                  maxCellEdgeUEsToBeScheduledUL_g will be present in this list.
 *********************************************************************************/
void  ulPfsCheckIfNodetoBeScheduled(
    LTE_LIST_NODE *node_p, //node of nonZeroBsrList
    ULUEContext  *ueULContext_p ,
    UInt8  maxUeToBeSorted,
    /*CA Changes start  */
    InternalCellIndex internalCellIndex)
    /*CA Changes end  */
{
    /* ICIC changes start */
    SortNodeT    *pSortNodeT = PNULL;
    SortNodeT    *pSortNodeAdd = PNULL;
    UInt32        weightedPriority;

    /* Flag indicating if additional node added in list needs to be deleted */
    UInt8 additionalNodeToBeDeleted = FALSE;
    /* Pointer to store the location of least Priority UE in the list */
    pSortNodeT = leastPriorityUe_p[internalCellIndex];

    UInt16 counter = listCount(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList));

    /* 
    ** If the incoming UE is CE and no CE UE is allowed in the List,
    ** skip the CE UE.
    */
    if ( (ueULContext_p->userLocationType == CE_USER) 
         /*CA Changes start  */
         && (!maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]) 
         /*CA Changes end  */
         && (!leastPriorityCEUe_p[internalCellIndex]) )
    {
        /* There is no need to process CE UE */
        return;
    }
    else
    {
        /* 
        ** Add NUM_BUFFER_UES_UL Ues to maxUeToBeSorted to hold two extra 
        ** Ues in lastTickScheduledList_g List. This is done to 
        ** avoid less UE scheduling.
        */
        maxUeToBeSorted = maxUeToBeSorted + NUM_BUFFER_UES_UL;
        weightedPriority = ueULContext_p->ulSchedulerAlgoInfo.weightedPriority;
        /* If new node is smaller than smallest of n, then ignore */
        if ( counter < maxUeToBeSorted )
        {
            pSortNodeAdd = ulPfsAllocSortNodeElem(internalCellIndex);
            pSortNodeAdd->weightedPriority = weightedPriority;
            pSortNodeAdd->ueIndex = ((nonZeroBsrListNode*)node_p)->ueIndex;
            /* ICIC changes start */
            /* 
            ** A new parameter userLocationType is added to store
            ** location Type of UE 
            */
            pSortNodeAdd->userLocationType = ueULContext_p->userLocationType;
            /* ICIC Changes end */

            if ( !leastPriorityUe_p[internalCellIndex] )
            {
                listInsertNodeAtHead(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList), 
                                     (LTE_LIST_NODE *) pSortNodeAdd);
                leastPriorityUe_p[internalCellIndex] = pSortNodeAdd;

                /* 
                ** If first incoming UE is CE, then its address needs to be stored.
                ** Scenario: Now if another CE UE is arrived which has more
                ** Priority then the already existing CE UE in list and
                ** maxCellEdgeUEsToBeScheduledUL_g = 0, then this CE UE is placed
                ** according to its Priority weightage while the CE UE having
                ** priority more than the lowest priority CE UE will be marked as
                ** lowest Priority CE UE and earlier lowest priority CE UE in
                ** list will be deleted.
                */
                if ( (ueULContext_p->userLocationType == CE_USER)
                /*+++530-coverity-32867*/
                /*CA Changes start  */
                && (maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]))
                /*CA Changes end  */
                /*+++530-coverity-32867*/
                {
                    leastPriorityCEUe_p[internalCellIndex] = pSortNodeAdd;
                    /*CA Changes start  */
                    maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]--;
                    /*CA Changes end  */
                }
                return;
            }

            if ( weightedPriority > leastPriorityUe_p[internalCellIndex]->weightedPriority )
            {
                /* Check the place of insertion of the Node */
                while ( (pSortNodeT = 
                  (SortNodeT  *)getPrevListNode((LTE_LIST_NODE *)pSortNodeT)) )
                {
                    if ( pSortNodeT->weightedPriority >= weightedPriority )
                    {
                        if ( ueULContext_p->userLocationType == CE_USER ) 
                        {
                            /*CA Changes start  */
                            if ( maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] )
                            /*CA Changes end  */
                            {
                                /* Insert CE UE in List */
                                ulPfsInsertCENodeInList(
                                              (LTE_LIST_NODE *) pSortNodeT,
                                              pSortNodeAdd,
                                              internalCellIndex);
                            }
                            else
                            {
                                /* Replace CE UE in List */
                                ulPfsReplaceCENodeInList(
                                                      pSortNodeT,
                                                      pSortNodeAdd,
                                                      internalCellIndex);
                            }
                        }
                        else
                        {
                            listInsertNodeAfter(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList),
                                                (LTE_LIST_NODE *) pSortNodeT,
                                                (LTE_LIST_NODE *) pSortNodeAdd);
                        }
                        break;
                    }
                }

                if ( PNULL == pSortNodeT )
                {
                    /* Insert Node at head of List */
                    ulPfsInsertNodeAtHead(pSortNodeAdd,
                                          ueULContext_p,
                                          &additionalNodeToBeDeleted,
                                          /*CA Changes start  */
                                          internalCellIndex);
                                          /*CA Changes end  */
                }
            }
            else
            {

                /* Since List has space to accomosate UEs, flag will be ZERO */
                additionalNodeToBeDeleted = FALSE;
                /* 
                ** If the incoming lower priority UE is CE then some calc.
                ** is required as mentioned below Else CC UE can be added in list
                ** & pointer pointing to lowest priority UE is adjusted 
                ** accordingly
                */
                if ( ueULContext_p->userLocationType == CE_USER )
                {
                    /* 
                    ** If new UE is CE, it is added in list and pointers 
                    ** pointing to lowest CE and CC UEs will point 
                    ** to this CE UE.
                    */
                    /*CA Changes start  */
                    if ( maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] )
                    /*CA Changes end  */
                    {
                        listInsertNodeAfter(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList),
                                            (LTE_LIST_NODE *)pSortNodeT, 
                                            (LTE_LIST_NODE *) pSortNodeAdd);
                        leastPriorityCEUe_p[internalCellIndex] = pSortNodeAdd;
                        leastPriorityUe_p[internalCellIndex] = pSortNodeAdd;
                        /*CA Changes start  */
                        maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]--;
                        /*CA Changes end  */
                    }
                    else
                    {
                        /*
                        ** If the new UE is CE and has lower priority than the
                        ** lowest priority UE of the existing list then there
                        ** is no need to add it in the list as 
                        ** to maxCellEdgeUEsToBeScheduledUL_g = 0.
                        */
                        /*CA Changes start  */
                        ulPfsFreeSortNumElem(pSortNodeAdd, internalCellIndex);
                        /*CA Changes end  */
                    }
                }
                else
                {
                    listInsertNodeAfter(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList),
                                        (LTE_LIST_NODE *)pSortNodeT,
                                        (LTE_LIST_NODE *) pSortNodeAdd);
                    leastPriorityUe_p[internalCellIndex] = pSortNodeAdd;
                }
            }
        }
        else
        {
            if ( weightedPriority > leastPriorityUe_p[internalCellIndex]->weightedPriority )
            {
                pSortNodeAdd = ulPfsAllocSortNodeElem(internalCellIndex);
                pSortNodeAdd->weightedPriority = weightedPriority;
                pSortNodeAdd->ueIndex = ((nonZeroBsrListNode*)node_p)->ueIndex;
                /* ICIC changes start */
                /* 
                ** A new parameter userLocationType is added to store 
                ** location Type of UE 
                */
                pSortNodeAdd->userLocationType = ueULContext_p->userLocationType;
                /* ICIC Changes end */

                /* 
                ** Keep track of smallest of top n - so that 
                ** new nodes are compared with smallest only.
                */

                while ( (pSortNodeT = 
                  (SortNodeT  *)getPrevListNode((LTE_LIST_NODE *)pSortNodeT)) )
                {
                    if ( pSortNodeT->weightedPriority >= weightedPriority )
                    {
                        if ( ueULContext_p->userLocationType == CE_USER ) 
                        {
                            /*CA Changes start  */
                            if ( maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] )
                            /*CA Changes end  */
                            {
                                /* Insert CE UE in List */
                                ulPfsInsertCENodeInList(
                                              (LTE_LIST_NODE *) pSortNodeT,
                                              pSortNodeAdd,
                                              internalCellIndex);
                                additionalNodeToBeDeleted = TRUE;
                            }
                            else
                            {
                                /* Replace CE UE in List */
                                ulPfsReplaceCENodeInList(
                                                pSortNodeT,
                                                pSortNodeAdd,
                                                internalCellIndex);
                                additionalNodeToBeDeleted = FALSE;
                            }
                        }
                        else
                        {
                            listInsertNodeAfter(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList),
                                                (LTE_LIST_NODE *) pSortNodeT,
                                                (LTE_LIST_NODE *) pSortNodeAdd);
                            additionalNodeToBeDeleted = TRUE;
                        }
                        break;
                    }
                }

                if ( PNULL == pSortNodeT )
                {
                    additionalNodeToBeDeleted = TRUE;
                    /* Insert Node at head of List */
                    ulPfsInsertNodeAtHead(pSortNodeAdd,
                                          ueULContext_p,
                                          &additionalNodeToBeDeleted,
                                          /*CA Changes start  */
                                          internalCellIndex);
                                          /*CA Changes end  */
                }
            }
        }
    
        if ( additionalNodeToBeDeleted )
        {
            leastPriorityUe_p[internalCellIndex] = 
                     (SortNodeT *) getLastListNode(( LTE_LIST *)&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList));
            listDeleteNode(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList),
                           (LTE_LIST_NODE *) leastPriorityUe_p[internalCellIndex]);
            /*CA Changes start  */
            ulPfsFreeSortNumElem(leastPriorityUe_p[internalCellIndex], internalCellIndex);
            /*CA Changes end  */
            leastPriorityUe_p[internalCellIndex] = 
                        (SortNodeT *) getLastListNode(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList));
        }
    }
    /* ICIC changes end */
}

/*****************************************************************************
 * Function Name  : ulPfsCheckRemainingNodes
 * Inputs         : node_p node of nonZeroBsrList
 *                  maxUEsToBeScheduledUL - Maximum nnumber of UE to be Scheduled
 *                                          in Downlink,
 *                  ulReferenceFlag - used for checking whether the UE is 
 *                                    considered already or not. If UE's
 *                                    isAlreadyConsideredFlag is equal to
 *                                    ulReferenceFlag, that means the UE is
 *                                    already considered,
 *					internalCellIndex
 *                  ulSubFrameNum - UP Link Subframe Number for which grant is
 *                                  being given.
 * Outputs        : None
 * Returns        : None
 * Description    : It gives the modulation scheme value As the 
 *                  nonZeroBsrList is parsed to identify UEs to be
 *                  computed, the nodes are also checked against top n and 
 *                  sorted if they are in top n. The below function runs through 
 *                  the remaining nodes of nonZeroBsrList to 
 *                  complete finding top n out of all UEs.
 ******************************************************************************/
static void ulPfsCheckRemainingNodes(
    LTE_LIST_NODE *node_p, //node of nonZeroBsrList_g
    UInt8  maxUEsToBeScheduledUL, 
    /* SPR 21068 start */
    tickType_t ulReferenceFlag
    /* SPR 21068 end   */
    /*CA Changes start  */
    ,InternalCellIndex internalCellIndex
    /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
    ,UInt8 ulSubFrameNum
#endif
    /* TDD Config 0 Changes End */
    )
    /*CA Changes end  */
  
{
	ULUEContext  *ueULContext_p = PNULL;
	ULUEContextInfo * ueULContextInfo_p = PNULL;
	while (node_p )
	{
		ueULContextInfo_p = &ulUECtxInfoArr_g[\
				    ((nonZeroBsrListNode*)node_p)->ueIndex];
		if ( !(ueULContextInfo_p->pendingDeleteFlag) )
		{ 
			ueULContext_p = ueULContextInfo_p->ulUEContext_p ;

            /* TDD Config 0 Changes Start */
#ifdef FDD_CONFIG
			if ( ueULContext_p->isAlreadyConsideredFlag != ulReferenceFlag )
#elif TDD_CONFIG
			if ( ueULContext_p->isAlreadyConsideredFlag[ulSubFrameNum]
                != ulReferenceFlag )
#endif
            /* TDD Config 0 Changes End */
			{
				/* ICIC changes start */
				ulPfsCheckIfNodetoBeScheduled(node_p, 
						ueULContext_p,
						maxUEsToBeScheduledUL,
                        internalCellIndex);
				/* ICIC changes end */
			}
			node_p = getNextListNode(node_p);
		}
		else
		{
			LTE_LIST_NODE * oldNode_p = PNULL;
			oldNode_p = getNextListNode(node_p);
            /* In case of RESET pendingDelete is also set hence don't free the node here as 
               this will be freed from resetMACULUEEntity*/
			/*listDeleteNode(&nonZeroBsrList_g, node_p);
			  freeMemPool((void *)node_p);*/
			node_p= oldNode_p;

		}
	}

}


/*****************************************************************************
 * Function Name  : ulPfsInitMacSchedulerData 
 * Inputs         : internalCellIndex   
 * Outputs        : None 
 * Returns        : None 
 * Description    : This function is used to initialise the data structure 
 *                  used in LTE MAC UL Fair Scheduler.
 ******************************************************************************/
void ulPfsInitMacSchedulerData(InternalCellIndex internalCellIndex)
{
    listInit(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList));
    ulPfsInitMemAllocElem(internalCellIndex);
}

/*****************************************************************************
 * Function Name  : ulPfsInitMemAllocElem 
 * Inputs         : internalCellIndex
 * Outputs        : None 
 * Returns        : None  
 * Description    : This function is used to initialise the 
 *                  ulSortNodeMemAllocElem_g for memory allocation.
 *****************************************************************************/
/*CA Changes start */
static void ulPfsInitMemAllocElem(InternalCellIndex internalCellIndex)
{
    UInt16 idx = 0;
    ulPfsGlobals_gp[internalCellIndex]->ulSortNodeMemAllocElem.freeHeadIdx = 0;
    for (  ; idx < NUM_UES_COMPUTED_ONE_TICK ; idx++ )
    {
        ulPfsGlobals_gp[internalCellIndex]->ulSortNodeMemAllocElem.sortListNode[idx].ueIndex = 0xFFFF;
        ulPfsGlobals_gp[internalCellIndex]->ulSortNodeMemAllocElem.sortListNode[idx].weightedPriority = 0;
        ulPfsGlobals_gp[internalCellIndex]->ulSortNodeMemAllocElem.sortListNode[idx].nxtIdx =  idx + 1 ;
        ulPfsGlobals_gp[internalCellIndex]->ulSortNodeMemAllocElem.sortListNode[idx].Idx = idx;
        if ( NUM_UES_COMPUTED_ONE_TICK  == (idx + 1) )
        {
            ulPfsGlobals_gp[internalCellIndex]->ulSortNodeMemAllocElem.sortListNode[idx].nxtIdx = 0XFFFF;
        }
    }
}


/*****************************************************************************
 * Function Name  : ulPfsAllocSortNodeElem 
 * Inputs         : internalCellIndex  
 * Outputs        : None 
 * Returns        : SortNodeT 
 * Description    : This function is used for memory allocation from Array.
 *****************************************************************************/
static SortNodeT *ulPfsAllocSortNodeElem(InternalCellIndex internalCellIndex)
{
    UInt8 freeHeadIndex  = ulPfsGlobals_gp[internalCellIndex]->ulSortNodeMemAllocElem.freeHeadIdx;
    ulPfsGlobals_gp[internalCellIndex]->ulSortNodeMemAllocElem.freeHeadIdx =
    ulPfsGlobals_gp[internalCellIndex]->ulSortNodeMemAllocElem.sortListNode
        [ulPfsGlobals_gp[internalCellIndex]->ulSortNodeMemAllocElem.freeHeadIdx].nxtIdx;

    return &ulPfsGlobals_gp[internalCellIndex]->ulSortNodeMemAllocElem.sortListNode[freeHeadIndex];
}

/*****************************************************************************
* Function Name  : ulPfsFreeSortNumElem
* Inputs         : pSortNode - Pointer to SortNodeT,
*				   internalCellIndex
* Outputs        : None 
* Returns        : None  
* Description    : This function is used for memory free from Array 
******************************************************************************/
static void ulPfsFreeSortNumElem ( SortNodeT *pSortNode,
                                   InternalCellIndex internalCellIndex
                                 )
{
    ulPfsGlobals_gp[internalCellIndex]->ulSortNodeMemAllocElem.sortListNode
        [pSortNode->Idx].nxtIdx =
        ulPfsGlobals_gp[internalCellIndex]->ulSortNodeMemAllocElem.freeHeadIdx;
    ulPfsGlobals_gp[internalCellIndex]->ulSortNodeMemAllocElem.freeHeadIdx = pSortNode->Idx;
}
/*CA Changes end */

/* ICIC changes start */
/********************************************************************************
 * Function Name  : ulPfsInsertCENodeInList
 * Inputs         : pSortNodeT - node in list after which CE Node is to be added,
 *                  pSortNodeAdd - new CE Node to be added,
 *					internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function inserts CE Node after a Node in linked List.
 *********************************************************************************/
static void ulPfsInsertCENodeInList(
                      LTE_LIST_NODE *pSortNodeT,
                      SortNodeT *pSortNodeAdd,
                      /*CA Changes start  */
                      InternalCellIndex internalCellIndex)
                      /*CA Changes end  */
{
    listInsertNodeAfter(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList),
                        (LTE_LIST_NODE *) pSortNodeT, 
                        (LTE_LIST_NODE *) pSortNodeAdd);

    /* Make this as least CE UE if it is first one */
    if ( !leastPriorityCEUe_p[internalCellIndex] )
    {
        leastPriorityCEUe_p[internalCellIndex] = pSortNodeAdd;    
    }
    /* 
    ** Change the pointer to lowest Priority CE UE to this new
    ** UE as it has lower Priority then already existing 
    ** Lowest Priority CE UE. 
    */
    else if ( leastPriorityCEUe_p[internalCellIndex]->weightedPriority > 
                              pSortNodeAdd->weightedPriority )
    {
        leastPriorityCEUe_p[internalCellIndex] = pSortNodeAdd;
    }

    /*CA Changes start  */
    maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]--;
    /*CA Changes end  */
}
/* ICIC changes end */

/* ICIC changes start */
/********************************************************************************
 * Function Name  : ulPfsReplaceCENodeInList
 * Inputs         : pSortNodeT - node in list after which CE Node is to be added,
 *                  pSortNodeAdd - new CE Node to be added,
 *					internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function replace CE Node in linked List. It adds new
 *                  high priority CE node and remove low Priority CE node 
 *                  from the list.
 ********************************************************************************/
static void ulPfsReplaceCENodeInList(
                      SortNodeT *pSortNodeT,
                      SortNodeT *pSortNodeAdd,
                      /*CA Changes start  */
                      InternalCellIndex internalCellIndex)
                      /*CA Changes end  */
{
    SortNodeT    *lowestPriorityCEUe_p = PNULL;
    /* 
    ** If the new UE is CE having less priority than
    ** the least Priority CE UE and there is no space
    ** to accomodate CE UE anymore it is discarded
    */
    if ( leastPriorityCEUe_p[internalCellIndex]->weightedPriority >= 
                           pSortNodeAdd->weightedPriority )
    {
        /* Discard this CE UE as it has low priority */
        /*CA Changes start  */
        ulPfsFreeSortNumElem(pSortNodeAdd, internalCellIndex);
        /*CA Changes end  */
    }
    else
    {
        /* 
        ** Since this CE UE has high priority than 
        ** lowest Priority CE UE, this has to be
        ** added in list while least Priority 
        ** CE UE has to be deleted
        */
        listInsertNodeAfter(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList),
                        (LTE_LIST_NODE *) pSortNodeT, 
                        (LTE_LIST_NODE *) pSortNodeAdd);
        lowestPriorityCEUe_p = leastPriorityCEUe_p[internalCellIndex];
    
        /* 
        ** Now search for next lowest priority CE UE
        ** before deleting this lowest priority CE UE
        */
        pSortNodeT = leastPriorityCEUe_p[internalCellIndex];
        
        if (leastPriorityCEUe_p[internalCellIndex] == leastPriorityUe_p[internalCellIndex])
        {
            leastPriorityUe_p[internalCellIndex] = (SortNodeT *) leastPriorityUe_p[internalCellIndex]->node.previous;     
        }

        while ( (pSortNodeT = 
                (SortNodeT *)getPrevListNode((LTE_LIST_NODE *)pSortNodeT)))
        {
            if ( pSortNodeT->userLocationType == CE_USER )
            {
                leastPriorityCEUe_p[internalCellIndex] = pSortNodeT;
                break;
            }
        }

        listDeleteNode(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList), 
                       (LTE_LIST_NODE *)lowestPriorityCEUe_p);
        /*CA Changes start  */
        ulPfsFreeSortNumElem(lowestPriorityCEUe_p, internalCellIndex);
        /*CA Changes end  */
    }
}
/* ICIC changes end */

/* ICIC changes start */
/*****************************************************************************
 * Function Name  : ulPfsInsertNodeAtHead
 * Inputs         : pSortNodeAdd - new CE Node to be added,
 *                  ueULContext_p - Pointer to ULUEContext,
 *					internalCellIndex
 * Outputs        : additionalNodeToBeDeleted_p flag indicating additional node
 *                  in the list to be deleted or not
 * Returns        : None
 * Description    : This function adds new Node at the head of linked List.
 *                  I also returns a flag which indicates whether additional  
 *                  node from the list needs to be deleted or not.
 ******************************************************************************/
/*CA Changes start  */
static void ulPfsInsertNodeAtHead(
                      SortNodeT *pSortNodeAdd,
                      ULUEContext  *ueULContext_p,
                      UInt8 *additionalNodeToBeDeleted_p,
                      InternalCellIndex internalCellIndex)
{
    SortNodeT    *lowestPriorityCEUe_p = PNULL;
    SortNodeT    *pSortNodeT = PNULL;
    listInsertNodeAtHead(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList), (LTE_LIST_NODE *)pSortNodeAdd);
    if ( ueULContext_p->userLocationType == CE_USER )
    {   
        /*CA Changes start  */
        if ( maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex] )
        /*CA Changes end  */
            {
            if ( !leastPriorityCEUe_p[internalCellIndex] )
            {
                leastPriorityCEUe_p[internalCellIndex] = pSortNodeAdd;    
            }

            /*CA Changes start  */
            maxCellEdgeUEsToBeScheduledUL_g[internalCellIndex]--;
            /*CA Changes end  */
        }
        else
        {
            if ( !leastPriorityCEUe_p[internalCellIndex] )
            {
                listDeleteNode(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList), 
                           (LTE_LIST_NODE *)pSortNodeAdd);
                /*CA Changes start  */
                ulPfsFreeSortNumElem(pSortNodeAdd, internalCellIndex);
                /*CA Changes end  */
                ltePanic("This case never occur\n");
            }
            else
            {
                /* 
                ** This is an additional CE UE. Delete lowest Priority
                ** CE UE from list then get the pointer pointing to lowest
                ** CE UE.
                */
                lowestPriorityCEUe_p = leastPriorityCEUe_p[internalCellIndex];
                pSortNodeT = leastPriorityCEUe_p[internalCellIndex];

                if (leastPriorityCEUe_p[internalCellIndex] == leastPriorityUe_p[internalCellIndex])
                {
                    leastPriorityUe_p[internalCellIndex] = (SortNodeT *) leastPriorityUe_p[internalCellIndex]->node.previous;     
                }

                while ( (pSortNodeT = 
                    (SortNodeT *)getPrevListNode((LTE_LIST_NODE *)pSortNodeT)))
                {
                    /* Get the pointer pointing to lowest CE UE in list */
                    if ( pSortNodeT->userLocationType == CE_USER )
                    {
                        leastPriorityCEUe_p[internalCellIndex] = pSortNodeT;
                        break;
                    }
                }

                listDeleteNode(&(ulPfsGlobals_gp[internalCellIndex]->lastTickScheduledList), 
                               (LTE_LIST_NODE *)lowestPriorityCEUe_p);
                /*CA Changes start  */
                ulPfsFreeSortNumElem(lowestPriorityCEUe_p, internalCellIndex);
                /*CA Changes end  */
            }
            *additionalNodeToBeDeleted_p = FALSE;
        }
    }
}
/*CA Changes end  */
/* ICIC changes end */
