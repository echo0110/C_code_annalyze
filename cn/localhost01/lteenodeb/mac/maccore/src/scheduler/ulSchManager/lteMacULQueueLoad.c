/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacULQueueLoad.c,v 1.1.1.1.6.1.6.1 2010/09/21 15:46:24 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the function definations related to 
 *                     UL Queue load.
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacULQueueLoad.c,v $
 * Revision 1.1.1.1.6.1.6.1  2010/09/21 15:46:24  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.16  2009/07/13 14:21:14  gur15697
 * second argument of LTE_MAC_UT_LOG modified
 *
 * Revision 1.15  2009/07/10 07:04:31  gur15697
 * compilation warnings removed
 *
 * Revision 1.14  2009/07/04 08:04:37  gur15697
 * updating of short bsr code updated
 *
 * Revision 1.13  2009/06/25 06:26:33  gur15697
 * bsrQueue_g removed
 *
 * Revision 1.12  2009/06/15 05:18:41  gur15697
 * UT bug fix
 *
 * Revision 1.11  2009/06/05 08:02:41  gur15697
 * init and cleanup functions added
 *
 * Revision 1.10  2009/06/03 12:04:11  gur15697
 * initULQueueLoadMgr() added
 *
 * Revision 1.9  2009/06/03 10:09:31  gur15697
 * bug fixes in UT cycle-1
 *
 * Revision 1.8  2009/06/01 06:44:01  gur18569
 * removed compilation errors
 *
 * Revision 1.7  2009/05/28 10:13:05  gur15697
 * macros NON_ZERO_TO_ZERO_BSR and  ZERO_TO_NON_ZERO_BSR removed
 *
 * Revision 1.6  2009/05/22 06:13:25  gur15697
 * as per the modified UL and DL context code modified
 *
 * Revision 1.5  2009/05/18 05:58:02  gur15697
 * Incorporated the comments
 *
 * Revision 1.4  2009/05/11 10:35:31  gur15697
 * short BSR handling added
 *
 * Revision 1.3  2009/05/08 14:17:24  gur15697
 * compilation errors removed
 *
 * Revision 1.2  2009/05/05 11:19:58  gur15697
 * modified to add logic in the functions
 *
 * Revision 1.1  2009/04/30 06:51:28  gur15697
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
#include "lteMacCellConfMgr.h"
#include "lteMacULQueueLoad.h"
#include "lteMacULSchedulerStrategy.h"
#include "lteMacULSpsScheduler.h"
#include "lteMacDemux.h"

#ifdef PERF_STATS
#include "ltePerfStats.h"
#endif
#include "lteCommonStatsManager.h"
/******************************************************************************
 Private Definitions
 *****************************************************************************/

/******************************************************************************
 Private Types
 *****************************************************************************/
UInt32 bsrIndexArray_g[64] = 
{
    0,     10,     12,     14,     17,     19,
    22,    26,     31,     36,     42,     49,
    57,    67,     78,     91,     107,    125,   
    146,   171,    200,    234,    274,    321,
    376,   440,    515,    603,    706,    826,   
    967,   1132,   1326,   1552,   1817,   2127,
    2490,  2915,   3413,   3995,   4677,   5476,
    6411,  7505,   8787,   10287,  12043,  14099,
    16507, 19325,  22624,  26487,  31009,  36304,  
    42502, 49759,  58255,  68201,  79846,  93479, 
    109439,128125, 150000, 160000
};

#define MAC_INITIAL_OPP 100;


/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/*CA Changes start  */
extern LTE_SQUEUE* ZeroNonZeroBSRTriggerQueue_gp[MAX_NUM_CELL];
/*CA Changes end  */
/* SRP 3608 changes start */
/*CA Changes start  */
extern LTE_SQUEUE* ulHighPriorityGrantQueue_gp[MAX_NUM_CELL];
/*CA Changes end  */
/* SPR 3608 changes end */

#ifdef PERF_STATS
extern UInt32 perfStatsULDelTrigger_g[PERF_MAX_UE_SUPPORTED];
#endif
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/****************************************************************************
 * Function Name  : fillLongBSR
 * Inputs         : bsrReport_p - Pointer to the BSR Report array contains the 
 *                  LcId and length of data per LcId
 *                  dataPtr_p - Points to the data
 * Outputs        : None
 * Returns        : dataPtr_p - Modified pointer to the data
 * Description    : function for decoding BSR and filling bsrQnode
 ****************************************************************************/
UInt8* fillLongBSR(UInt32 *bsrReport_p, UInt8 *dataPtr_p)
{
    /* it.s a Long BSR*/
    /*Don't send it to RLC and send it to Container's
     *BSR queue based on the thread number
     */
    LTE_MAC_UT_LOG(LOG_DETAIL,UL_QUEUELOAD_MGR,
            "[Entry] : Function [%s] \n",__func__);

    UInt8 bsrVal=0;
    bsrVal = ((*dataPtr_p & 0xFC)>>2);
    bsrReport_p[0] = FETCH_BUFFER_SIZE_FROM_BSR_INDEX(bsrVal);
    
    LTE_MAC_UT_LOG(LOG_DETAIL,UL_QUEUELOAD_MGR,
        "Long BSR Obtained for LCG id 0 and BSR Index is :: %d , Value :: %d\n",
        bsrVal,bsrReport_p[0]);

    bsrVal = ( (*dataPtr_p & 0x03 ) << 4);
    dataPtr_p++;
    bsrVal |=( (*dataPtr_p & 0xF0) >>4) ;
    bsrReport_p[1] = FETCH_BUFFER_SIZE_FROM_BSR_INDEX(bsrVal);
    
    LTE_MAC_UT_LOG(LOG_DETAIL,UL_QUEUELOAD_MGR,
        "Long BSR Obtained for LCG id 1 and BSR Index is :: %d ,Value :: %d\n",
        bsrVal,bsrReport_p[1]);

    bsrVal = ((*dataPtr_p & 0x0F)<<2)  ;
    dataPtr_p++;
    bsrVal |=( (*dataPtr_p & 0xC0) >> 6) ;
    bsrReport_p[2] = FETCH_BUFFER_SIZE_FROM_BSR_INDEX(bsrVal);
    
    LTE_MAC_UT_LOG(LOG_DETAIL,UL_QUEUELOAD_MGR,
         "Long BSR Obtained for LCG id 2 and BSR Index is :: %d, Value :: %d\n",
         bsrVal,bsrReport_p[2]);

    bsrVal = (*dataPtr_p & 0x3F);
    bsrReport_p[3] = FETCH_BUFFER_SIZE_FROM_BSR_INDEX(bsrVal);

    LTE_MAC_UT_LOG(LOG_DETAIL,UL_QUEUELOAD_MGR,
         "Long BSR Obtained for LCG id 3 and BSR Index is :: %d , Value :: %d\n",
         bsrVal,bsrReport_p[3]);

    LTE_MAC_UT_LOG(LOG_DETAIL,UL_QUEUELOAD_MGR,"[Exit] : Function [%s] \n",__func__);
    return dataPtr_p;
}

/****************************************************************************
 * Function Name  : updateShortOrTruncatedBSR
 * Inputs         : ulCntxt_p - pointer to the UL UE context 
 *                  dataPtr_p - Pointer to the data
 *                  thdIndex - Represents the ZeroNonZeroBSRTriggerQueue_g[] index 
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : LCG ID received in the short BSR
 * Description    : This function will update the received Short or Truncated 
 *                  BSR in UL UE context
 ****************************************************************************/
UInt32 updateShortOrTruncatedBSR(ULUEContext *ulCntxt_p,
                                 UInt8  *dataPtr_p,
                                 UInt32 thdIndex,
                                 /*CA Changes start  */
                                 InternalCellIndex  internalCellIndex
                                 /*CA Changes end  */
                                 /* SPR 16916 FIX START */
#ifdef KPI_STATS
                                 ,UInt8 *lcgIndex_p,
                                 UInt8 *addTrigLCGNodeForKpi
#endif
                                 /* SPR 16916 FIX END */
                                 )
{
    UInt16 bsrVal   = 0;
    UInt32 shortBSRLcgId = 0;
    bsrVal        = (*dataPtr_p & 0x3F);
    shortBSRLcgId = ((*dataPtr_p & 0xC0)>>6);

    UInt32 prevBsrPerLcg = 0;
 /*  SPR  3444 changes start */
/*
 *  shortBSRLcgId has the value of 2 bit from dataPtr_p that cannot exceed 
 *  LCGID_INVALID_VALUE (= 5) so commenting the below code
 *  if (shortBSRLcgId >= LCGID_INVALID_VALUE)
 *   {
 *       shortBSRLcgId = LCGID_INVALID_VALUE;
 *   }
 *   else
 *   {
 *   }
 */
 /*  SPR  3444 changes end */   
    
     prevBsrPerLcg = ulCntxt_p->bsrNet[shortBSRLcgId];
     if(ulCntxt_p->lcCount[shortBSRLcgId])
     {
         {
         /* BSR CHG */
         /*SPR 4020 Start*/
         /*CA Changes start  */
         if(UL_QOS_SCHEDULING != cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig.ulSchedulerStrategy)
         /*CA Changes end  */
         {
             /* SPR 6886 Changes starts */
             if(ulCntxt_p->bsrNetTotal >= ulCntxt_p->bsrNet[shortBSRLcgId])
             {
                 ulCntxt_p->bsrNetTotal -= ulCntxt_p->bsrNet[shortBSRLcgId];
             }
             else
             {
                 ulCntxt_p->bsrNetTotal  = 0;
             }
             /* SPR 6886 Changes ends*/
         }
         else
         {
             /* SPR 6886 Changes starts */
             if(ulCntxt_p->bsrNetTotal >= ulCntxt_p->ulLCGInfo[shortBSRLcgId].qLoad)
             {
                 ulCntxt_p->bsrNetTotal -= ulCntxt_p->ulLCGInfo[shortBSRLcgId].qLoad;
             }
             else
             {
                 ulCntxt_p->bsrNetTotal  = 0;
             }
             /* SPR 6886 Changes ends */
         }
         /*SPR 4020 End*/
         
         ulCntxt_p->bsrNet[shortBSRLcgId] = 
             FETCH_BUFFER_SIZE_FROM_BSR_INDEX(bsrVal);

         /*SPR 4020 Start*/
         (ulCntxt_p)->ulLCGInfo[shortBSRLcgId].qLoad = 
             (ulCntxt_p)->bsrNet[shortBSRLcgId];
         /*SPR 4020 End*/

         LTE_MAC_UT_LOG(LOG_INFO,UL_QUEUELOAD_MGR,  "[%s] [SHORTBSR or TRUNCATED BSR] Previous BSR value in UL context is %d\n",
                 __func__,prevBsrPerLcg,bsrVal);
         /* SPR 16916 FIX START */
#ifdef KPI_STATS
         /* Storing the LCG for which BSR has become non zero from zero
          * For KPI updation */
         if((prevBsrPerLcg == 0) && (ulCntxt_p->bsrNet[shortBSRLcgId] != 0) && ((*lcgIndex_p) < NUM_OF_LCG))
         {
             /* Adding the LCG for Active UE Kpi */
             addTrigLCGNodeForKpi[*lcgIndex_p] = shortBSRLcgId;
             (*lcgIndex_p)++;
         }
         else if ((0 != prevBsrPerLcg)  && (0 == ulCntxt_p->bsrNet[shortBSRLcgId]))
         {
             updateNumOfActiveUePerQciUl(ulCntxt_p->ueIndex,internalCellIndex);
         }
#endif
         /* SPR 16916 FIX END */
         /*CA Changes start  */
         if(UL_QOS_SCHEDULING == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->ulSchdConfig.ulSchedulerStrategy)
         /*CA Changes end  */
         {
             if(prevBsrPerLcg == 0 && ulCntxt_p->bsrNet[shortBSRLcgId]!= 0)
             {
                     /*SPR 9203 Fix Starts*/
                     /*CA Changes start  */
                     putEntryInBSRQueue(ulCntxt_p->ueIndex,shortBSRLcgId,thdIndex,ADD_TRIGGER, internalCellIndex);
                     /*CA Changes end  */
                 /* +- SPR 17404 */
             }
             else if(0 != prevBsrPerLcg  && 0 == ulCntxt_p->bsrNet[shortBSRLcgId])
             {
                 /*BSR has become Zero from nonZero*/
                 /*CA Changes start  */
                 putEntryInBSRQueue(ulCntxt_p->ueIndex,shortBSRLcgId,thdIndex,DELETE_TRIGGER, internalCellIndex);
                 /*CA Changes end  */
                 /* 32.425 Events code changes start*/
#ifdef KPI_STATS

                 /* Enqueue the Ue Id and lcId in the Circular Queue */
			     /* + PERF_CA GBL */
                 /* + SPR 17404 */
                 lteMacUpdateKpiStatsIPThpUlOnEnqueue(shortBSRLcgId,ulCntxt_p->ulLCGInfo,ulCntxt_p->ueIndex,
																										internalCellIndex);
                 /* - SPR 17404 */
				 /* - PERF_CA GBL */
#endif
                 /* 32.425 Events code changes end*/


             }
         }
         /* SPS_CHG */
         else
         {
             /*! \fn MacRetType updateShortOrTruncatedBSR()
              *  \brief Function Modification
              * */
             if(prevBsrPerLcg == 0 && ulCntxt_p->bsrNet[shortBSRLcgId] != 0 )
             {
                 ulSpsHandleActivationInRR(ulCntxt_p, shortBSRLcgId, internalCellIndex);

                 putEntryInBSRQueue(ulCntxt_p->ueIndex,shortBSRLcgId,thdIndex,ADD_TRIGGER, internalCellIndex);
             }
             else if(0 != prevBsrPerLcg  && 0 == ulCntxt_p->bsrNet[shortBSRLcgId])
             {
                 /* 32.425 Events code changes start*/
#ifdef KPI_STATS

                 /* Enqueue the Ue Id and lcId in the Circular Queue */
                 /* + SPR 17404 */
                 lteMacUpdateKpiStatsIPThpUlOnEnqueue(shortBSRLcgId,ulCntxt_p->ulLCGInfo,ulCntxt_p->ueIndex,internalCellIndex);
                 /* - SPR 17404 */
#endif
                 /* 32.425 Events code changes end*/
             }
             /* SPR 11864 Fix */

         }
         /* SPS_CHG */

         /*Add the new received BSR and decrement the previous BSR present*/
         ulCntxt_p->bsrNetTotal += ulCntxt_p->bsrNet[shortBSRLcgId];

         LOG_MAC_MSG(MAC_UL_SHORT_OR_TRUNCATED_BSR_LCH_MSG_ID, LOGBRIEF, MAC_PUSCH,
                 getCurrentTick(),
                 ulCntxt_p->ueIndex,
                 shortBSRLcgId,
                 bsrVal, 
                 ulCntxt_p->bsrNetTotal,
                 internalCellIndex,
                 DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
                 FUNCTION_NAME, "" );

         /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
         if(ulCntxt_p->bsrNetTotal > 0)
         {
             hdUeContextInfo_g.ulSrPresentFlag[ulCntxt_p->ueIndex] = TRUE;
         }
         else
         {
             hdUeContextInfo_g.ulSrPresentFlag[ulCntxt_p->ueIndex] = FALSE;
         }
#endif
         }
         /*HD FDD Changes End*/
     }

    return shortBSRLcgId;
}

/****************************************************************************
 * Function Name  : updateLongBSR
 * Inputs         : ulCntxt_p - pointer to the UL UE context 
 *                  dataPtr_p - Pointer to the data
 *                  thdIndex - Represents the ZeroNonZeroBSRTriggerQueue_g[] index 
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : dataPtr_p - Modified pointer to the data
 * Description    : This will update the Long BSR in UL UE context
 ****************************************************************************/
UInt8* updateLongBSR(ULUEContext *ulCntxt_p,
                     UInt8  *dataPtr_p,
                     UInt32 thdIndex,
                     /*CA Changes start  */
                     InternalCellIndex  internalCellIndex
                     /*CA Changes end  */
                     /* SPR 16916 FIX START */
#ifdef KPI_STATS
                     ,UInt8 *lcgIndex_p,
                     UInt8 *addTrigLCGNodeForKpi
#endif
                     /* SPR 16916 FIX END */
                     )
{
    /* BSR CHG */
    UInt32 bsrReport[NUM_OF_LCG] = {0};
    dataPtr_p = fillLongBSR(bsrReport, dataPtr_p);
    /* CA Changes start */
    UInt8  ulSchedulerStrategy = cellSpecificParams_g.\
         cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
         ulSchdConfig.ulSchedulerStrategy;
    /* CA Changes end */
    UInt32 lcgIndex      = 0;
    UInt32 tempLatestBsr = 0;
    UInt32 prevBsrPerLcg = 0;
    /* SPR 6917 changes start */
    UInt32 tempBsrForPriorityQ = 0;
    /* SPR 6917 changes end */
 
    LOG_MAC_MSG(MAC_UL_LONG_BSR_LCH_MSG_ID, LOGBRIEF, MAC_PUSCH,
            getCurrentTick(),
            ulCntxt_p->ueIndex,
            bsrReport[0],
            bsrReport[1], 
            bsrReport[2],
            bsrReport[3],
            DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME, "" );

    for(lcgIndex = 0; lcgIndex < NUM_OF_LCG; lcgIndex++)
    {
        if(ulCntxt_p->lcCount[lcgIndex])
        {        
            prevBsrPerLcg = ulCntxt_p->bsrNet[lcgIndex];

            LTE_MAC_UT_LOG(LOG_DETAIL,UL_QUEUELOAD_MGR,
                    "[%s] OLD BSR value:%d is"
                    "Overwritten by new BSR value:%d of LCG:%d \n",
                    __func__,ulCntxt_p->bsrNet[lcgIndex],
                    bsrReport[lcgIndex],lcgIndex);

            /* SPR 5060 start */
            if(!ulCntxt_p->prioritySchedulingReqUL)
            {
                (ulCntxt_p)->bsrNet[lcgIndex] =  bsrReport[lcgIndex];
                (ulCntxt_p)->ulLCGInfo[lcgIndex].qLoad = 
                    (ulCntxt_p)->bsrNet[lcgIndex];
            }
            /* SPR 6917 changes start */
            else
            {
                tempBsrForPriorityQ += bsrReport[lcgIndex];
            }
            /* SPR 6917 changes end */
            /* SPR 5060 end */
            /* SPR 16916 FIX START */
#ifdef KPI_STATS
            /* Storing the LCG for which BSR has become non zero from zero
             * For KPI updation */
            if((prevBsrPerLcg == 0) && (bsrReport[lcgIndex] != 0) && ((*lcgIndex_p) < NUM_OF_LCG))
            {
                /* Adding the LCG for Active UE Kpi */
                addTrigLCGNodeForKpi[*lcgIndex_p] = lcgIndex;
                (*lcgIndex_p)++;
            }
            else if ((0 != prevBsrPerLcg)  && (0 == bsrReport[lcgIndex]))
            {
                updateNumOfActiveUePerQciUl(ulCntxt_p->ueIndex,internalCellIndex);
            }
#endif
            /* SPR 16916 FIX END */

            /* SPR 11045 fix start */                
            if( UL_QOS_SCHEDULING == ulSchedulerStrategy &&
                0 == tempBsrForPriorityQ )
            /* SPR 11045 fix end */                
            {
                if(prevBsrPerLcg == 0 && bsrReport[lcgIndex]!= 0)
                {
                     /*SPR 9203 Fix Starts*/
                        putEntryInBSRQueue(ulCntxt_p->ueIndex,lcgIndex,thdIndex,ADD_TRIGGER, internalCellIndex);
                        /*CA Changes end  */
                        /* +- SPR 17404 */
                }
                else if(0 != prevBsrPerLcg  && 0 == bsrReport[lcgIndex])
                {
                    /*BSR has become Zero from nonZero*/
                    /*CA Changes start  */
                    putEntryInBSRQueue(ulCntxt_p->ueIndex,lcgIndex,thdIndex,DELETE_TRIGGER, internalCellIndex);
                    /*CA Changes end  */

                    /* 32.425 Events code changes start*/
#ifdef KPI_STATS

                    /* Enqueue the Ue Id and lcId in the Circular Queue */
			     	/* + PERF_CA GBL */
                    /* + SPR 17404 */
                    lteMacUpdateKpiStatsIPThpUlOnEnqueue(lcgIndex,ulCntxt_p->ulLCGInfo, ulCntxt_p->ueIndex,
																									internalCellIndex);
                    /* - SPR 17404 */
			     	/* - PERF_CA GBL */
#endif
                    /* 32.425 Events code changes end*/

                }
            }
            else
                /* SPS_CHG */
            {
                /*! \fn MacRetType updateLongBSR()
                 *  \brief Function Modification
                 * */
                if(prevBsrPerLcg == 0 && bsrReport[lcgIndex]!= 0 )
                {
                    ulSpsHandleActivationInRR(ulCntxt_p, lcgIndex, internalCellIndex);

                    putEntryInBSRQueue(ulCntxt_p->ueIndex,lcgIndex,thdIndex,ADD_TRIGGER, internalCellIndex);
                }
                else if(0 != prevBsrPerLcg && 0 == bsrReport[lcgIndex])
                {

#ifdef KPI_STATS
                    /* Enqueue the Ue Id and lcId in the Circular Queue */
                    /* + SPR 17404 */
                    lteMacUpdateKpiStatsIPThpUlOnEnqueue(lcgIndex,ulCntxt_p->ulLCGInfo,ulCntxt_p->ueIndex,internalCellIndex);
                    /* - SPR 17404 */
#endif
                    /* 32.425 Events code changes end*/

                }
                /* ADDEd new line end */ 
                /* SPR 11864 Fix */
            }
            /* SPS_CHG */
            tempLatestBsr += ulCntxt_p->bsrNet[lcgIndex];
        }    
    }
    ulCntxt_p->bsrNetTotal = tempLatestBsr;

    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    if(ulCntxt_p->bsrNetTotal > 0)
    {
        hdUeContextInfo_g.ulSrPresentFlag[ulCntxt_p->ueIndex] = TRUE;
    }
    else
    {
        hdUeContextInfo_g.ulSrPresentFlag[ulCntxt_p->ueIndex] = FALSE;
    }
#endif
    /*HD FDD Changes End*/
    /* SPR 6917 changes start */
    /* SPR 11045 fix start */                
    if( 0 != tempBsrForPriorityQ && ulCntxt_p->prioritySchedulingReqUL )
    /* SPR 11045 fix end */                
    {
        DLUEContext *dlCntxt_p = 
             dlUECtxInfoArr_g[ulCntxt_p->ueIndex].dlUEContext_p;
        /*CA Changes start  */
        putBSREntryInPriorityGrantQueue(ulCntxt_p->ueIndex, internalCellIndex);
        /*CA Changes end  */
        if( dlCntxt_p && dlCntxt_p->tcrntiFlag)
        {
            ulUECtxInfoArr_g[ulCntxt_p->ueIndex].pendingDeleteFlag = FALSE;
            dlUECtxInfoArr_g[ulCntxt_p->ueIndex].pendingDeleteFlag = FALSE;
            dlCntxt_p->tcrntiFlag = FALSE;
        }
    }
    /* SPR 6917 changes end */
    return dataPtr_p;
}

/****************************************************************************
 * Function Name  : putEntryInBSRQueue
 * Inputs         : ueIndex - UE Index
 *                  lcgId - logical channel group Id
 *                  thdIndex - Represents the ZeroNonZeroBSRTriggerQueue_g[] index 
 *                  flag - DELETE_TRIGGER/ADD_TRIGGER
 * Outputs        : None
 * Returns        : None
 * Description    : This will creates the bsr queue node and push node to Queue
 ****************************************************************************/
/*+COVERITY 5.3.0 - 53495*/
void putEntryInBSRQueue(UInt16 ueIndex,
                        UInt32 lcgId,
                        UInt32 thdIndex,
                        AddDeleteTrigger  flag,
                        /*CA Changes start  */
                        InternalCellIndex internalCellIndex)
                        /*CA Changes end  */
/*-COVERITY 5.3.0 - 53495*/
{
    bsrQueueNode *bsrQNode_p = PNULL;

    GET_MEM_FROM_POOL(bsrQueueNode , bsrQNode_p , sizeof(bsrQueueNode),PNULL);
   
    /* Coverity 24558 */
    if (PNULL != bsrQNode_p)
    {
        bsrQNode_p->ueIdx = ueIndex;
        bsrQNode_p->lcgId = lcgId;
        bsrQNode_p->addDeleteTrigger = flag;
#ifdef PERF_STATS
        if (flag == DELETE_TRIGGER)
        {
            perfStatsULDelTrigger_g[ueIndex] = 1;
        }
#endif

        /* + coverity 32523 */
        /*CA Changes start  */
        if (pushNode(ZeroNonZeroBSRTriggerQueue_gp[internalCellIndex] + thdIndex,
                     &(bsrQNode_p->bsrNodeAnchor)))
        /*CA Changes start  */
        {
            freeMemPool(bsrQNode_p);
        }
        /* - coverity 32523 */
    }
    /* Coverity 24558 */
}

/* BSR CHG start */
/************************************************************************
* Function Name : updateBsrNetOfLCG
* Inputs:         ulUEContext_p - UE Context for which bsrNet is updated. 
*                 tempAllocTbSize - TB Size allocated.
*                 ulHarqInfo_p -
*                 internalCellIndex - Cell-Index at MAC
* Returns:        NONE
* Description:    This function update bsrNet[NUM_OF_LCG] after allocating 
*                 UL Resources.
************************************************************************/
 void updateBsrNetOfLCG( ULUEContext* ulUEContext_p,
                          UInt32 tempAllocTbSize
                          /* SPR 10027 FIX START */
                          /* +- SPR 17777 */
                          ,ULHarqInfo* ulHarqInfo_p
                          /* SPR 10027 FIX END */
                           /* SPR 11864 Fix */
#ifdef KPI_STATS
                           ,InternalCellIndex internalCellIndex
#endif
                           /* +- SPR 17777 */
                            /* SPR 11864 Fix */
                          )
{
    UInt8 lcgIndex = 0;
    UInt32 tempTbSize = tempAllocTbSize;
#ifdef KPI_STATS
    /* SPR 11864 Fix */
    UInt32 prevBsrLcg = 0;
    /* SPR 11864 Fix */
#endif

    while (lcgIndex < NUM_OF_LCG)
    {
        if (!ulUEContext_p->lcCount[lcgIndex])
        {
            lcgIndex++;
            continue;
        }
        /* SPR 10027 FIX START */
        /* Updating the isDrbPresent Flag in Harq To be used in case of reTx 
         * Updating the lastScheduled Tick */
        if (TRUE == ulUEContext_p->ulLCGInfo[lcgIndex].isDrbPresent
            && tempAllocTbSize)
        {
            ulHarqInfo_p->isDrbPresent = TRUE;
            /* SPR 15744 Fix Start */
            /* Last scheduled Tick for UE Inactivity should be updated only at Demux. 
             * Code for updating last scheduled tick removed. */
            /* SPR 15744 Fix End */
        }
        /* SPR 10027 FIX END */

#ifdef KPI_STATS
    	/* SPR 11864 Fix */
        prevBsrLcg = ulUEContext_p->bsrNet[lcgIndex];
    	/* SPR 11864 Fix */
#endif

        if ( ulUEContext_p->bsrNet[lcgIndex] >= tempTbSize)
        {
            ulUEContext_p->bsrNet[lcgIndex] -= tempTbSize;
            tempTbSize = 0;
 /*SPR 21494 changes Start */
#ifdef KPI_STATS
    	    /* SPR 11864 Fix */
	    if(prevBsrLcg && (0 == ulUEContext_p->bsrNet[lcgIndex]))
		{
              /* + SPR 17404 */
              lteMacUpdateKpiStatsIPThpUlOnEnqueue(lcgIndex,ulUEContext_p->ulLCGInfo,ulUEContext_p->ueIndex,internalCellIndex);
              /* - SPR 17404 */
		}
    	     /* SPR 11864 Fix */
#endif
/*SPR 21494 changes End */

            break;
        }
        else
        {
            tempTbSize -=  ulUEContext_p->bsrNet[lcgIndex];
            ulUEContext_p->bsrNet[lcgIndex] = 0;
    	     /* SPR 11864 Fix */
 /*SPR 21494 changes Start */
#ifdef KPI_STATS
	    if(prevBsrLcg)
        {
            /* Enqueue the Ue Id and lcId in the Circular Queue */
            /* + SPR 17404 */
            lteMacUpdateKpiStatsIPThpUlOnEnqueue(lcgIndex,ulUEContext_p->ulLCGInfo, ulUEContext_p->ueIndex,internalCellIndex);
            /* - SPR 17404 */
        }
#endif
/*SPR 21494 changes End */
    	     /* SPR 11864 Fix */
        }
        lcgIndex++;
    }
    /* + SPR 8517 Fix*/
    if (tempTbSize)
    {
        LTE_MAC_UT_LOG(LOG_INFO,UL_QUEUELOAD_MGR,
                "[%s] tempTbSize is greater than sum of all bsrBet[NUM_OF_LCG].",
                __func__);
    }
    /* - SPR 8517 Fix*/
    return;
}
/* BSR CHG end */

/* SRP 3608 changes start */
/*****************************************************************************
 * Function Name  : putBSREntryInPriorityGrantQueue
 * Inputs         : ueIndex -
 *                  internalCellIndex - Cell-Index at MAC
 *
 * Outputs        :
 * Returns        :
 * Description    : This function creates a new srNode and inserts
 *                           this node in the globalPrioritySrQueue.*/
/*****************************************************************************/
/*CA Changes start  */
void putBSREntryInPriorityGrantQueue(UInt16 ueIndex, InternalCellIndex internalCellIndex)
/*CA Changes end  */
{
    LTE_MAC_UT_LOG(LOG_INFO,UL_QUEUELOAD_MGR,"[%s] UE Index [%d] Entry. ",
            __func__, ueIndex);

    if((ulUECtxInfoArr_g[ueIndex].ulUEContext_p)->prioritySchedulingReqUL)
    {
        ULGrantRequestQueueNode* bsrNode_p = PNULL;
        GET_MEM_FROM_POOL(ULGrantRequestQueueNode, bsrNode_p,sizeof(ULGrantRequestQueueNode),PNULL);

        /* Coverity 24557*/
        if (PNULL !=  bsrNode_p)
        {
            bsrNode_p->ueIndex            = ueIndex;
            bsrNode_p->ttiCounter         = 0;
            bsrNode_p->ulGrantRequestType = NEW_ULGRANT_REQ_MSG;

            /* + coverity 32522 */
            /*CA Changes start  */
            if(pushNode(ulHighPriorityGrantQueue_gp[internalCellIndex], &(bsrNode_p->ulGrantRequestNodeAnchor)))
            /*CA Changes start  */
            {
                freeMemPool(bsrNode_p);
            }
            /* - coverity 32522 */
        }
        /* Coverity 24557*/
    }
}
/* SPR 3608 changes end */
/* SPS_CHG*/
/**********************************************************************************************
 * Function Name  : ulProcessSpsUeForActDeact
 * Inputs         : ulUEContext_p - pointer to UE context
 *                  lcId -
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This functions checks for the different conditions for SPS Activation and
 *                  Deactivation. On detection of SPS Activation/Deactivation it pushes the entry
 *                  in respective queue to be processed by SPS Strategy.
**********************************************************************************************/
void ulProcessSpsUeForActDeact(ULUEContext *ulUEContext_p,
                               UInt8 lcId,
                               /*CA Changes start  */
                               InternalCellIndex internalCellIndex)
                               /*CA Changes end  */
{
    LP_MacULSpsActReq      macUlSpsActReq_p         = PNULL;
    LP_SpsULIntervalProfile   ulSpsIntervalProfile_p = PNULL;
    LP_SpsUlInfo              spsUlInfo_p            = PNULL;
    UInt16                 ulSpsInterval          = 0;
    /*!
     * \code
     * Calculate InterPacketArrival Time.
     * Get the SPS Interval for the UE.
     * \endcode
     * */
    spsUlInfo_p = &ulUEContext_p->spsUlInfo;
    ulSpsInterval = spsUlInfo_p->spsUlSetupInfo_p->\
                    semiPersistentSchedIntervalUL;
    ulSpsIntervalProfile_p = 
        /*CA Changes start  */
        &cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->\
        ulSchdConfig.spsUlIntervalProfile[ulSpsInterval] ;
        /*CA Changes end  */

    switch (spsUlInfo_p->spsState)
    {
        case SPS_CONFIGURED:
            {
                /*!
                 * \code
                 *  - If (UL UE State == SPS_CONFIGURED ) and 
                 *       (Minimum Sps MCS >= UE Mcs)
                 *    Activation condition satisfied. Push entry in ulSpsActQ_g 
                 *  \endcode
                 * */
                if (ulSpsIntervalProfile_p->ulMinMcsSps <=  ulUEContext_p->mcsIndex)
                {
                    spsUlInfo_p->lcId = lcId;
                    spsUlInfo_p->avgRlcSduSize = MAC_INITIAL_OPP; 

                    /* Allocate memory for Activation queue node */
                    macUlSpsActReq_p = (LP_MacULSpsActReq)getMemFromPool(
                            sizeof(MacULSpsActReq), PNULL);
                    if ( PNULL == macUlSpsActReq_p  )
                    {
                        LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                                getCurrentTick(), __LINE__, sizeof(MacULSpsActReq),
                                0,0,0, 0,0, __FILE__, __func__);
                        ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,
                                CRITICAL_ALARM);
                        ltePanic("getMemFromPool fails %s", __func__);
            			/* Klockwork warning Changes Start */
            			return ;
            			/* Klockwork warning Changes End */
                    }
                    /* Fill the activation queue node */
                    macUlSpsActReq_p->ueIndex = ulUEContext_p->ueIndex;
                    macUlSpsActReq_p->cause = FIRST_NON_ZERO_QUEUE_LOAD;
                    /* Enqueue in ulSpsActQ_g */
                    /*CA Changes start  */
                    /* + Coverity Id - 32620 */
                    if(!ENQUEUE_MAC_UL_SPS_Q(ulSpsActQ_gp[internalCellIndex], macUlSpsActReq_p))
                    {
                        freeMemPool(macUlSpsActReq_p);
                    }
                    /* - Coverity Id - 32620 */
                    /* Update Q Stats */
                    /* CA Stats Changes Start */
                    LTE_MAC_QUEUE_USED( UL_SPS_ACT_Q, 
                            QCOUNT_MAC_UL_SPS_Q(ulSpsActQ_gp[internalCellIndex]), 
                            internalCellIndex);
                    /* CA Stats Changes End */
                    /*CA Changes end  */
                }
                break;
            }  
            /*coverity-530 CID-32342-gcc warn*/
        default:
            {
                break;
            }
            /*coverity-530 CID-32342-gcc warn*/

    }    
}
/**********************************************************************************************
 * Function Name  : ulSpsHandleActivationInRR 
 * Inputs         : ulUEContext_p - pointer to UL UE context
 *                  lcgIndex -  lcgIndex for which BSR is reported
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This functions checks if there is any SPS lc in the Lcg reported
 *                  during BSR. If SPS Lc is present, process for SPS Activation
**********************************************************************************************/
void ulSpsHandleActivationInRR(ULUEContext *ueULContext_p,UInt32 lcgIndex, InternalCellIndex internalCellIndex )
{
    UInt8 lcIndex = 0;
    UInt8 lcId = 0;
    UInt8 counter = 0;
    counter =  ueULContext_p->ulLCGInfo[lcgIndex].numLC;
    for (;lcIndex < MAX_LOGICAL_CHANNEL && counter;)
    {
        lcId = ueULContext_p->ulLCGInfo[lcgIndex].lcId[lcIndex];
        if(lcId != INVALID_LCID)
        {
            --counter;
            if((TRUE == ueULContext_p->lcInfo[lcId].isSpsLc)&&
                    (SPS_SETUP == ueULContext_p->spsUlInfo.requestType) )
            {
                /*! \code
                 * If LC is an SPS LC
                 *    Call ulProcessSpsUeForActDeact()
                 * \endcode
                 * */
                ulProcessSpsUeForActDeact(ueULContext_p,
                                          lcId,
                                          /*CA Changes start  */
                                          internalCellIndex);
                                          /*CA Changes end  */
            }
        }
        lcIndex++;
    }
}

