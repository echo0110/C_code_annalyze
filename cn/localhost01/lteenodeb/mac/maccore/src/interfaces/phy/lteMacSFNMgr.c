/******************************************************************************
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacSFNMgr.c,v 1.2.6.1.4.2 2010/11/08 06:06:24 gur10121 Exp $
 *
 ******************************************************************************
 *
 *  File Description : 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacSFNMgr.c,v $
 * Revision 1.2.6.1.4.2.2.3.4.1  2010/11/08 06:06:24  gur10121
 * Semi static changes merging
 *
 * Revision 1.2.6.1.4.2.2.3  2010/10/25 08:11:44  gur23971
 * removed warning
 *
 * Revision 1.2.6.1.4.2.2.2.2.1  2010/10/29 10:25:46  gur10121
 *  Semi Static UT fixes
 *
 * Revision 1.2.6.1.4.2.2.2  2010/10/22 12:44:11  gur04640
 *
 * Revision 1.2.6.1.4.2.2.1.2.1  2010/10/13 10:56:11  gur04640
 * Changes done for implementing Stats using shared memory
 *
 * Revision 1.2.6.1.4.2.2.1.2.1  2010/10/12 02:45:30  gur04640
 * Changes done for implementing Stats using shared memory
 *
 * Revision 1.2.6.1.4.2.2.1  2010/09/21 15:46:10  gur20491
 * FAPI changes
 *
 * Revision 1.2.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.5  2009/07/16 08:11:04  gur19413
 * changed to  func getSFNAndSF
 *
 * Revision 1.4  2009/07/13 04:28:29  gur19413
 * changed log type
 *
 * Revision 1.3  2009/06/10 12:41:16  gur19413
 * comments incorportated
 *
 * Revision 1.2  2009/06/05 11:11:13  gur19413
 * remove macro and added function getSFAndSF
 *
 * Revision 1.1  2009/06/05 09:45:52  gur19413
 * Initial Version
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
#include "lteMacSFNMgr.h"
#include "lteMacPCCH.h"
#include "lteLayer2CommanTypes.h"
#include "lteMacStatsManager.h"
#include "lteCommonStatsManager.h"
#ifdef UTFWK_SIMULATION
#include "lteSync.h"
#endif
#include "lteMacOAMInterface.h"
/* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
#include "lteMacDLSpsScheduler.h"
#include "lteMacTddMgr.h"
#include "lteMacULSpsScheduler.h"
#endif
/* - SPS_TDD_Changes */

/******************************************************************************
  Private Definitions
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

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
extern UInt16 numErrIndDlConfig_g[MAX_NUM_CELL];
extern UInt16 numErrIndHiDci0_g[MAX_NUM_CELL];
/* + SPR_12546_12547 */
#ifdef KPI_STATS
/* + SPR 17439 */
extern void resetKPILastCalculatedTicks(void);
/* - SPR 17439 */
#endif
/* EICIC +-*/
extern UInt8 absPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN]; /* Aggressor*/
extern UInt8 usableAbsPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN]; /* Victim*/
extern UInt8 startAbsDelay_g[MAX_NUM_CELL];
extern UInt8 applyAbsConfiguration_g[MAX_NUM_CELL];
extern UInt8 absPatternIndex_g[MAX_NUM_CELL];
extern  UInt8 usableAbsPatternIndex_g[MAX_NUM_CELL];
/*SPR 18697 fix start*/
#ifdef LTE_EMBMS_SUPPORTED
extern UInt8 cellStartAfterDelete_g[MAX_NUM_CELL];
#endif
/*SPR 18697 fix end*/
/* EICIC -*/
/* - SPR_12546_12547 */
/* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
extern void ulSpsResetOccSchType( LP_SpsULStrIntervalTTIContext ttiContext_p,
    UInt8 count );
extern void ulSpsResetSdlSchType( LP_SpsULStrIntervalTTIContext ttiContext_p,
    UInt8 count );
extern SpsULStrIntervalTTIContext *spsULStrTTIMap_gp[MAX_NUM_CELL];
#endif
/* - SPS_TDD_Changes */
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
 /* CA changes start*/
 /* gloabl tick will be maintained cell wise at MAC */
/* CA Stats Changes Start*/
/* SPR 15909 fix start */
tickType_t currentTick_g[MAX_NUM_CELL];
/* SPR 15909 fix end */
/* CA Stats Changes End */
UInt32 systemFrameNum_g[MAX_NUM_CELL];
UInt32 pagingSFN_g[MAX_NUM_CELL];
UInt32 subFrameNum_g[MAX_NUM_CELL];
UInt8  pagingCycle_g[MAX_NUM_CELL];
/*mandeep changes start*/
/*mandeep changes stop*/
/* This global tick is used for Log/HSM */
/* SPR 15909 fix start */
tickType_t macTTITickCount_g;
/* SPR 15909 fix end */

#ifdef UTFWK_SIMULATION
extern UInt32 count_mutex;
extern UInt32  condition_var;
#endif

#ifdef FAPI_L2_MASTER
/* FAPI L2 AS MASTER START*/
/*
** These variables are used to store the calculated tick, system frame
** and subframe number after receiving tick from the system.
** 
*/
/* SPR 15909 fix start */
STATIC tickType_t currentTickFrmL2Tick_g[MAX_NUM_CELL];
/* SPR 15909 fix end */
STATIC UInt16 systemFrameNumFrmL2Tick_g[MAX_NUM_CELL];
STATIC UInt8  subFrameNumFrmL2Tick_g[MAX_NUM_CELL];

/* FAPI L2 AS MASTER END*/
#endif


#ifdef DL_UL_SPLIT_TDD
/* Global variables for TDD UL Thread*/
/* SPR 15909 fix start */
static tickType_t currentTickUl_g = 0xFFFFFFFFFFFFFFFF; 
/* SPR 15909 fix end */
static UInt32 systemFrameNumUl_g;
static UInt32 subFrameNumUl_g;
#endif

/* + SPS_TDD_Changes*/
#ifdef TDD_CONFIG
void dlSpsResetOccSchType( LP_SpsStrIntervalTTIContext ttiContext_p,
    UInt8 count );
void dlSpsResetSdlSchType( LP_SpsStrIntervalTTIContext ttiContext_p,
    UInt8 count );
#endif
/* - SPS_TDD_Changes*/
/*****************************************************************************
 * Function Name  : macRegistrationCnf 
 * Inputs         : sysFrameNum - It represent the system Frame Number 
 *                   subFrameNum - It represent the Sub Frame Number
 *                   internalCellIndex - internal cell Index                   
 * Outputs        : None 
 * Returns        : None
 * Description    : This function Initialize the Current Tick and System Frame
 *                  Number/SubFrame Number.
 *****************************************************************************/
 void macRegistrationCnf( UInt32 sysFrameNum,
                                UInt32 subFrameNum,
                                /* CA Changes start */
                                InternalCellIndex internalCellIndex
                                /* CA Changes end */
                              )
{ 
        LTE_MAC_UT_LOG(LOG_INFO,SFN_MGR,
                   "[%s]: function Entry\n", __func__);
                              
      /* CA changes start */ 
      systemFrameNum_g[internalCellIndex] = sysFrameNum;
      subFrameNum_g[internalCellIndex]    = subFrameNum;
      currentTick_g[internalCellIndex] = sysFrameNum * MAX_SUB_FRAME + subFrameNum;
      /* CA changes start */ 

      LTE_MAC_UT_LOG(LOG_INFO,SFN_MGR,
                   "[%s]: function Exit\n", __func__);
}


void update_frameinfo(void);
void update_frameinfo_1(void);
#ifdef MAC_AUT_TEST
void update_frameinfo()
{
InternalCellIndex internalCellIndex=0;
systemFrameNum_g[internalCellIndex]=0;
subFrameNum_g[internalCellIndex]=0;
}

void update_frameinfo_1()
{
InternalCellIndex internalCellIndex=0;
systemFrameNum_g[internalCellIndex]=1;
subFrameNum_g[internalCellIndex]=0;
}

#endif
        
/* SPR 1535 Changes Start*/
/*****************************************************************************
 * Function Name  : processFirstSubFrameInd
 * Inputs         : sysFrameNum - It represent the system Frame Number 
 *                  subFrameNum - It represent the Sub Frame Number
 *                  internalCellIndex - It represents the cell index number
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : This function is invoked by PHY Layer to indicate the Tick.
 *                  This function will calculate currentTick_g from SFN and SF
 *                  After getting the tick indication at Mac Layer,
 *                  also update the system Frame number and subframe number.
 *****************************************************************************/
/* CA changes start */ 
/* SPR 8712 changes */
#ifdef DL_DATA_SEPARATION
extern SEM_T waitForCellSetupIndSemEncThrd[MAX_NUM_CELL];
#endif    
/* SPR 8712 changes */
#if (!defined (DL_UL_SPLIT) && !defined (DL_UL_SPLIT_TDD))
extern LTE_SEM cellSyncSem_g;
#endif
 void processFirstSubFrameInd(UInt32 sysFrameNum, UInt32 subFrameNum,
                   InternalCellIndex internalCellIndex)
{
    systemFrameNum_g[internalCellIndex] = sysFrameNum;
    subFrameNum_g[internalCellIndex]    = subFrameNum;
#if (!defined (DL_UL_SPLIT) && !defined (DL_UL_SPLIT_TDD))
    UInt8 isOtherCellStateRunning = 0;
    
    if (IS_CA_ENABLED())
    {
        semWait(&cellSyncSem_g);
        isOtherCellStateRunning = CHECK_CELL_STATE(!internalCellIndex);
        semPost(&cellSyncSem_g);
    }

    if(isOtherCellStateRunning)
    {
        if( (systemFrameNum_g[internalCellIndex] == systemFrameNum_g[!internalCellIndex]) && 
            (subFrameNum_g[internalCellIndex] == subFrameNum_g[!internalCellIndex])) 
        {
            currentTick_g[internalCellIndex] = currentTick_g[!internalCellIndex];
        }
        else if( ((systemFrameNum_g[internalCellIndex] == systemFrameNum_g[!internalCellIndex]) && 
                  (subFrameNum_g[internalCellIndex] == (subFrameNum_g[!internalCellIndex] + 1))) ||
                 ((systemFrameNum_g[internalCellIndex] == systemFrameNum_g[!internalCellIndex] +1 ) &&
                  (subFrameNum_g[internalCellIndex] == 0)))
        {
            currentTick_g[internalCellIndex] = currentTick_g[!internalCellIndex] + 1;
        }
    }
    else
#endif
    {
        currentTick_g[internalCellIndex] = sysFrameNum * MAX_SUB_FRAME + subFrameNum;
    }
    pagingSFN_g[internalCellIndex]   = sysFrameNum;
   /*+SPR #3444*/ 
    if( pagingSFN_g[internalCellIndex] < MAX_SFN_VALUE)
   /*-SPR #3444*/ 
    {
        pagingCycle_g[internalCellIndex] = 0;
    }
    /* CA changes start */
    /* MAC will update the global to check which is the first active cell
     * among multiple cells */
    if (INVALID_CELL_INDEX == layer2CommonGetActiveCellTick())
    {
        layer2CommonSetActiveCellTick (internalCellIndex);
    }
    /* this global TTI is updated for Logging/HSM 
     * This will be updated only for first cell which is active for
     * tick updation */
    if (internalCellIndex == layer2CommonGetActiveCellTick())
    {
        macTTITickCount_g = sysFrameNum * MAX_SUB_FRAME + subFrameNum;
    /* + SPR_12546_12547 */
#ifdef KPI_STATS
	/* KPI tti variables are reset */
        resetKPILastCalculatedTicks(); 
#endif
    /* - SPR_12546_12547 */
    }
    /*Fix for for __SPR_634__ START*/
    macFirstSubFrameInd(sysFrameNum, subFrameNum, internalCellIndex);
    /*Fix for for __SPR_634__ END*/
#ifdef DL_UL_SPLIT
    lteMacUpdateStatsPerf(DL_OVERSHOOT_STATS, internalCellIndex);
    lteMacUpdateStatsPerf(UL_OVERSHOOT_STATS, internalCellIndex);
#elif DL_UL_SPLIT_TDD
    lteMacUpdateStatsPerfStart(DL_OVERSHOOT_STATS, internalCellIndex);
    lteMacUpdateStatsPerf(DL_OVERSHOOT_STATS, internalCellIndex);
#else
    lteMacUpdateStatsPerf(internalCellIndex);
#endif
/* SPR 8712 changes */
#ifdef DL_DATA_SEPARATION
    semPostDefault(&waitForCellSetupIndSemEncThrd[internalCellIndex]);
#endif

/*SPR 18697 fix start*/
#ifdef LTE_EMBMS_SUPPORTED
    cellStartAfterDelete_g[internalCellIndex] = LTE_TRUE;  
#endif
/*SPR 18697 fix end*/

/* SPR 8712 changes */
/* CA changes end */ 
}
/* SPR 1535 Changes End*/
/* SPR 19288 change start */
/* SPR 19288 change end */

 /* SPR 20283 Start - EICIC Restablishment */
/*****************************************************************************
 * Function Name  : macProcessEICICAbsPattern
 * Inputs         : sysFrameNum - It represent the system Frame Number and
 *                  subFrameNum - It represent the Sub Frame Number
 *                  internalCellIndex - It represents the cell index number
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : This function is invoked by processSubFrameInd()/processTickReceivedOnL2()
 *                  to calculate the bits in the csi pattern and to check whether absconfiguration
 *                  should be applied or not.
 *****************************************************************************/
 void macProcessEICICAbsPattern(UInt16 sysFrameNum, UInt8 subFrameNum,
                     InternalCellIndex internalCellIndex)
{
    UInt8 numBitsCsiPattern = 0;
#ifdef FDD_CONFIG
       numBitsCsiPattern = NUM_BITS_ABS_PATTERN;
#else
    if (UL_DL_CONFIG_0 ==
           cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->subFrameAssign)
   {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN;
    }
    else if (UL_DL_CONFIG_6 ==
              cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->subFrameAssign)
   {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
   }
   else
   {
        numBitsCsiPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
   }
#endif

    if (LTE_TRUE == startAbsDelay_g[internalCellIndex])
    {
        if (0 == (((sysFrameNum * MAX_SUB_FRAME) + subFrameNum) % numBitsCsiPattern))
        {
            applyAbsConfiguration_g[internalCellIndex] = LTE_TRUE;
            startAbsDelay_g[internalCellIndex] = LTE_FALSE;
        }
    }
}
/* SPR 20283 End - EICIC Restablishment */

/*****************************************************************************
 * Function Name  : processSubFrameInd
 * Inputs         : sysFrameNum - It represent the system Frame Number and
 *                  subFrameNum - It represent the Sub Frame Number
 *                  internalCellIndex - It represents the cell index number 
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : This function is invoked by PHY Layer to indicate the Tick.
 *                  After getting the tick indication at Mac Layer,update current
 *                  Tick and also update the system Frame number and subframe number.
 *****************************************************************************/
 void processSubFrameInd(UInt16 sysFrameNum, UInt8 subFrameNum, 
        InternalCellIndex internalCellIndex)
{
/* EICIC +-*/
    /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
    UInt8                        spsOccasionUeCount  = 0;
    UInt8                        sdlCount            = 0;
    UInt16                       index               = 0;
    UInt16                       prevIndex           = 0;
    LP_SpsULStrIntervalTTIContext  prevTtiCtx_p = PNULL;
    LP_SpsStrIntervalTTIContext  prevTtiCtxDl_p = PNULL;
#endif
    /* - SPS_TDD_Changes */
#ifdef FLEXRAN
/* Fix for crash because of SF and tick mismatch + */
    /* get current System Number and SubFrame Number */
    UInt32 curSysFrameNum = 0;
    UInt32 curSubFrameNum = 0;
    getSFAndSFN(&curSysFrameNum, &curSubFrameNum, internalCellIndex);
    if( curSysFrameNum > 1023)
    {
        curSysFrameNum = 0;
    }

    /* SPR 4727 changes start */ 
    if (sysFrameNum >= MAX_SFN_VALUE)
    {
        /*Wrap around condition*/
        sysFrameNum -= MAX_SFN_VALUE;
    }
    /* SPR 4727 changes end */ 

#if 0
    /* SPR 15909 fix start */
    tickType_t curTTI = (curSysFrameNum*10 + curSubFrameNum);
    tickType_t recvTTI = (sysFrameNum*10 + subFrameNum);
    /* SPR 15909 fix end */
    UInt32 offset = 0;
    if(curTTI > recvTTI)
    {     
        offset = (MAX_SFN_VALUE * MAX_SUB_FRAME) ;
    }
#endif
	UInt32 curTTI = (curSysFrameNum*10 + curSubFrameNum);
	UInt32 recvTTI = (sysFrameNum*10 + subFrameNum);
	UInt32 offset;
	if (curTTI > recvTTI)
	{
		offset = (MAX_SFN_VALUE * MAX_SUB_FRAME) + recvTTI - curTTI;
	}
	else
	{
		offset = recvTTI - curTTI;
	}

	if (offset != 1)
	{
		fprintf(stderr,"tick miss on cell %d! offset %d, currentTick %d\n",
				internalCellIndex, offset, currentTick_g[internalCellIndex]);
	}

    systemFrameNum_g[internalCellIndex] = sysFrameNum;
    subFrameNum_g[internalCellIndex]    = subFrameNum;
    currentTick_g[internalCellIndex] += offset;
/* Fix for crash because of SF and tick mismatch - */
#else
    systemFrameNum_g[internalCellIndex] = sysFrameNum;
    subFrameNum_g[internalCellIndex]    = subFrameNum;


    currentTick_g[internalCellIndex]++;
#endif
#ifdef OVERSHOOT_CORRECTION
    SFI_PROCESSING_START(currentTick_g[internalCellIndex]);
#endif
  
    /* Update the Paging SFN */ 
    if ( 0 == subFrameNum )
    {
        (pagingSFN_g[internalCellIndex])++;
        if (0 == (pagingSFN_g[internalCellIndex] % MAX_SFN_VALUE))
        {
            (pagingCycle_g[internalCellIndex])++;
        }
        if (pagingSFN_g[internalCellIndex] > MAX_PAGING_SFN_RANGE)
        { 
            pagingSFN_g[internalCellIndex]   = 0;
            pagingCycle_g[internalCellIndex] = 0;
        }
    }
    gettimeofday_wrapper(&logMetaData.tm, NULL);
    logMetaData.currentTick = currentTick_g[internalCellIndex];
                            
    LOG_UT(MAC_PHY_SF_IND_ID,LOGDEBUG,MAC_L1_INF,\
            currentTick_g[internalCellIndex],
            systemFrameNum_g[internalCellIndex],subFrameNum_g[internalCellIndex],0,0,0,
            0.0,0.0,__func__,"MAC_PHY_SF_IND");

	if (0 == (currentTick_g[internalCellIndex] % 2000))
	{
	    LOG_MAC_MSG(MAC_TTI_CELL_LOG_INFO,LOGWARNING,MAC_OAM_INF,\
	            currentTick_g[internalCellIndex],
	            currentTick_g[internalCellIndex],
	            systemFrameNum_g[internalCellIndex],subFrameNum_g[internalCellIndex],macCrntiUeNum_g[internalCellIndex],macTCrntiUeNum_g[internalCellIndex],
	            0.0,0.0,__func__,"");
	}

    /* this global TTI is updated for Logging 
     * This will be updated only for first cell which is active for
     * tick updation */
    if (internalCellIndex == layer2CommonGetActiveCellTick())
    {
        macTTITickCount_g++;
    }
    /* SPR 23393 Changes Start */
    /* SPR 23393 Changes End */

    /*Fix for for __SPR_634__ START*/
    macSubFrameInd(internalCellIndex);
    /*Fix for for __SPR_634__ END*/
    /* CA Stats Changes Start */
#ifdef DL_UL_SPLIT
    lteMacUpdateStatsPerf(DL_OVERSHOOT_STATS, internalCellIndex);
    lteMacUpdateStatsPerf(UL_OVERSHOOT_STATS, internalCellIndex);
#elif DL_UL_SPLIT_TDD
    lteMacUpdateStatsPerf(DL_OVERSHOOT_STATS, internalCellIndex);
#else
    lteMacUpdateStatsPerf(internalCellIndex);
#endif
    /* CA Stats Changes End */
#ifdef SSI_DEBUG_STATS    
    /* CA Stats Changes Start */
    lteMacUpdateStatsWriteTTIIndex(internalCellIndex);
    lteMacResetPuschStats(internalCellIndex);
    /* CA Stats Changes End */
#endif
/* SPR 2302 changes start */
/* SPR 2127 changes start */
    if( 0 == 
         (currentTick_g[internalCellIndex] % MAC_STATS_INT_TICK_COUNT))
    {
        calculateDownlinkThroughPut(internalCellIndex);
#ifndef DL_UL_SPLIT_TDD
        calculateUplinkThroughPut(internalCellIndex);
#endif
    }
/* SPR 2127 changes end */
/* SPR 2302 changes end */
    /* Decrement the error indication count for DL Config on receipt of sub
     * frame indication */
    if( 0 < numErrIndDlConfig_g[internalCellIndex] )
    {
        numErrIndDlConfig_g[internalCellIndex]--;
    }
    /* Decrement the error indication count for HI_DCI0 on receipt of sub
     * frame indication */
    if( 0 < numErrIndHiDci0_g[internalCellIndex] )
    {
        numErrIndHiDci0_g[internalCellIndex]--;
    }
    /* SPR 20283 Start - EICIC Restablishment */
    macProcessEICICAbsPattern (systemFrameNum_g[internalCellIndex], 
                subFrameNum_g[internalCellIndex], internalCellIndex);
    /* SPR 20283 End - EICIC Restablishment */

    /* + SPS_TDD_Changes */
    /* Reset the schType for previous TTI in TDD*/
#ifdef TDD_CONFIG
    if(TRUE == cellSpecificParams_g.\
            cellConfigAndInitParams_p[internalCellIndex]->initParams_p->spsSysWideParam.enableSps)
    {
        index = getCurrentTick()  % MAX_UL_SPS_INTERVAL;
        if( 0 == index  )
        {
            prevIndex = MAX_UL_SPS_INTERVAL - 1;
        }
        else
        {
            prevIndex = index - 1;
        }
        prevTtiCtx_p =  spsULStrTTIMap_gp[internalCellIndex] + prevIndex;
        spsOccasionUeCount = prevTtiCtx_p->numULSpsOccasionPerTti;
        if (0 != spsOccasionUeCount)
        {
            ulSpsResetOccSchType( prevTtiCtx_p, spsOccasionUeCount );
        }

        sdlCount = prevTtiCtx_p->numULSpsDciPerTti;
        if( 0 != sdlCount )
        {
            ulSpsResetSdlSchType( prevTtiCtx_p, sdlCount );
        }
        /*! \code
         *                * Reset schType of each UE present in SDL and occasion list in previous
         *                               * ttiIndex
         *                                              * \endcode
         *                                                             * */
        index = (getCurrentTick() + DL_ADVANCE_TICK) % MAX_SPS_INTERVAL;
        if( 0 == index  )
        {
            prevIndex = MAX_SPS_INTERVAL - 1;
        }
        else
        {
            prevIndex = index - 1;
        }
        prevTtiCtxDl_p =  &spsStrTTIMap_g[internalCellIndex][prevIndex];
        /* Reset schType for all UEs in occ list in prev tick */
        spsOccasionUeCount = prevTtiCtxDl_p->numDlSpsOccasionPerTti;
        if (0 != spsOccasionUeCount)
        {
            dlSpsResetOccSchType( prevTtiCtxDl_p, spsOccasionUeCount );
        }

        /* Reset schType for all UEs in SDL in prev tick */
        sdlCount = prevTtiCtxDl_p->numDlSpsDciPerTti;
        if( 0 != sdlCount )
        {
            dlSpsResetSdlSchType( prevTtiCtxDl_p, sdlCount );
        }

    }
#endif
    /* - SPS_TDD_Changes */
}

#ifdef DL_UL_SPLIT_TDD
/*****************************************************************************
 * Function Name  : processSubFrameIndUl
 * Inputs         : sysFrameNum - It represent the system Frame Number
 *                  subFrameNum - It represent the Sub Frame Number
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : This function is invoked by PHY Layer to indicate the Tick 
 *                  to UL thread. After getting the tick indication at Mac 
 *                  Layer,update currentTick and also update the system Frame 
 *                  number and subframe number.
 *****************************************************************************/
 void processSubFrameIndUl(UInt16 sysFrameNum, UInt8 subFrameNum)
{
    systemFrameNumUl_g = sysFrameNum;
    subFrameNumUl_g    = subFrameNum;
    currentTickUl_g++;
    lteMacUpdateStatsPerf(UL_OVERSHOOT_STATS);
    if( 0 == 
         (currentTickUl_g % MAC_STATS_INT_TICK_COUNT))
    {
        calculateUplinkThroughPut();
    }
}

/*****************************************************************************
 * Function Name  : processFirstSubFrameIndUl
 * Inputs         : sysFrameNum - It represent the system Frame Number 
 *                  subFrameNum - It represent the Sub Frame Number
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : This function is invoked by PHY Layer to indicate the First UL 
 *                  Tick. It will calculate currentTickUl_g from SFN and SF
 *                  After getting the tick indication it also updates the 
 *                  system Frame number and subframe number.
 *****************************************************************************/
 void processFirstSubFrameIndUl(UInt16 sysFrameNum, UInt8 subFrameNum)
{
    systemFrameNumUl_g = sysFrameNum;
    subFrameNumUl_g    = subFrameNum;
    currentTickUl_g    = sysFrameNum * MAX_SUB_FRAME + subFrameNum;

    lteMacUpdateStatsPerfStart(UL_OVERSHOOT_STATS);
    lteMacUpdateStatsPerf(UL_OVERSHOOT_STATS);
}
#endif


/*****************************************************************************
 * Function Name  : getSystemFrameNum
 * Inputs         : internalCellIndex - Internal Cell index
 * Outputs        : NONE
 * Returns        : systemFrameNum_g
 * Description    : This function will return the current System frame number.
 *****************************************************************************/
/* CA Changes start */
UInt32 getSysFrameNumer(InternalCellIndex internalCellIndex)
{
#ifdef FAPI_L2_MASTER
    return systemFrameNumFrmL2Tick_g[internalCellIndex];
#else
    return systemFrameNum_g[internalCellIndex];
#endif
}

/* SPR 23393 Changes Start */
/* SPR 23393 Changes End */
/*****************************************************************************
 * Function Name  : getSubFrameNumber
 * Inputs         : internalCellIndex - Internal Cell index
 * Outputs        : NONE
 * Returns        : subFrameNum_g
 * Description    : This function will return the current sub frame number.
 *****************************************************************************/
UInt32 getSubFrameNumber(InternalCellIndex internalCellIndex)
{
#ifdef FAPI_L2_MASTER
    return subFrameNumFrmL2Tick_g[internalCellIndex];
#else
    return subFrameNum_g[internalCellIndex];
#endif
}

/*****************************************************************************
 * Function Name  : getCurrentTick
 * Inputs         : NONE
 * Outputs        : NONE
 * Returns        : current Tick
 * Description    : This function return the current Tick at Mac layer
 *****************************************************************************/
/* SPR 15909 fix start */
tickType_t getCurrentTick()
/* SPR 15909 fix end */
{
#ifdef FAPI_L2_MASTER
    return currentTickFrmL2Tick_g[cellIndexMac_g]; 
#else
    return currentTick_g[cellIndexMac_g];
#endif
}

/***********************************************************
 * Function Name  : getCurrentTickForCell
 * Inputs         : internalCellIndex
 * Outputs        : NONE
 * Returns        : current Tick for cell
 * Description    : This function return the current Tick at Mac layer
 *****************************************************************************/
/* SPR 15909 fix start */
tickType_t getCurrentTickForCell(InternalCellIndex internalCellIndex)
/* SPR 15909 fix end */
{
    if(internalCellIndex < MAX_NUM_CELL)
    {
#ifdef FAPI_L2_MASTER
    	return currentTickFrmL2Tick_g[internalCellIndex]; 
#else
        return currentTick_g[internalCellIndex];
#endif
    }
    else
    {
        return CURRENT_TICK_INIT_VALUE;
    }
}

/*****************************************************************************
 * Function Name  : getCurrentUlTick
 * Inputs         : NONE
 * Outputs        : NONE
 * Returns        : current Tick
 * Description    : This function return the current Ul Tick at Mac layer
 *                  for UL thread
 *****************************************************************************/
/* SPR 15909 fix start */
/* + SPR 17439 */
tickType_t getCurrentTickUl(void)
/* - SPR 17439 */
{
/* SPR 20257 fix start */
/* SPR 15909 fix end */
#ifdef DL_UL_SPLIT_TDD
    return currentTickUl_g;
#else
    return currentTick_g[cellIndexMac_g];
#endif
}
/* SPR 20257 fix end */
/* SPR 3932 Chg */
/*****************************************************************************
 * Function Name  : resetCurrentTick
 * Inputs         : InternalCellIndex- internal cell index 
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : This function reset current Tick at Mac layer.
 *****************************************************************************/
void resetCurrentTick(InternalCellIndex internalCellIndex)
{
    if(internalCellIndex)
    {
        currentTick_g[internalCellIndex] =  0;
        subFrameNum_g[internalCellIndex] = 0;
        systemFrameNum_g[internalCellIndex] = 0;
    }
}

/*****************************************************************************
 * Function Name  : initCurrentTick
 * Inputs         : NONE 
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : This function initializes current Tick at Mac layer.
 *****************************************************************************/
/* + SPR 17439 */
void initCurrentTick(void)
/* - SPR 17439 */
{
    InternalCellIndex cellIndex;
    for(cellIndex =0 ; cellIndex < MAX_NUM_CELL ; cellIndex ++)
    {
        currentTick_g[cellIndex] = CURRENT_TICK_INIT_VALUE;
#ifdef FAPI_L2_MASTER
        currentTickFrmL2Tick_g[cellIndex] = CURRENT_TICK_INIT_VALUE;
/* SPR22974 Start */
        subFrameNumFrmL2Tick_g[cellIndex] = SF_TICK_INIT_VALUE;
/* SPR22974 End */
#endif
    }
    return;
}
/* CA changes end */
/* SPR 3932 Chg */

#ifdef UTFWK_SIMULATION
/*****************************************************************************
 * Function Name  : updateTick
 * Inputs         : NONE
 * Outputs        : NONE
 * Returns        : 0
 * Description    : This function updates the current Tick at Mac layer.
 *****************************************************************************/
/* SPR 15909 fix start */
tickType_t updateTick()
{
    /* SPR 15909 fix end */
    lteMacUpdateStatsPerf();
    lteMacUpdateStatsPerfStart();
    
    /* SPR 15909 fix start */
    if(currentTick_g == 0xFFFFFFFFFFFFFFFF)
    {  
     /* SPR 15909 fix end */
        systemFrameNum_g[internalCellIndex] = 0;
      	subFrameNum_g    = 0;
      	currentTick_g = 0;
    } 
    else
    {
    	subFrameNum_g++;
    	if(subFrameNum_g == MAX_SUB_FRAME)
    	{
        	subFrameNum_g = 0;
            systemFrameNum_g[internalCellIndex]++;
            if(systemFrameNum_g[internalCellIndex] == MAX_SFN_VALUE)
        	{
                systemFrameNum_g[internalCellIndex] = 0;
        	}
    	}
    	currentTick_g++;
     }

    return 0;
}
#endif

/*****************************************************************************
 * Function Name  : getSFAndSFN
 * Inputs         : sysFrameNum_p - It points to sysFrameNumber
 *                  subFrameNum_p - It points to subFrameNumber
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the value of sysFrameNum_p, 
 *                  subFrameNum_p.
*****************************************************************************/
/* CA Changes start */
void getSFAndSFN( UInt32* sysFrameNum_p , 
                  UInt32* subFrameNum_p,
                  InternalCellIndex internalCellIndex)
{
#ifdef FAPI_L2_MASTER
    *sysFrameNum_p = systemFrameNumFrmL2Tick_g[internalCellIndex];
    *subFrameNum_p = subFrameNumFrmL2Tick_g[internalCellIndex];
#else
    *sysFrameNum_p = systemFrameNum_g[internalCellIndex];
    *subFrameNum_p = subFrameNum_g[internalCellIndex];
#endif
}

/* SPR 23393 Changes Start */
/* SPR 23393 Changes End */
#ifdef DL_UL_SPLIT_TDD
/*****************************************************************************
 * Function Name  : getSFAndSFN
 * Inputs         : sysFrameNum_p - Its point to sysFrameNumber
 *                  subFrameNum_p - Its point to subFrameNumber
 * Outputs        : NONE
 * Returns        : current Tick
 * Description    : This function fills the value of sysFrameNum_p, 
 *                  subFrameNum_p
*****************************************************************************/
 void getUlSFAndSFN( UInt32* sysFrameNum_p , 
                  UInt32* subFrameNum_p
                 )
{
    *sysFrameNum_p = systemFrameNumUl_g;
    *subFrameNum_p = subFrameNumUl_g;
}
#endif

/*****************************************************************************
 * Function Name  : getPagingCycle
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : pagingCycle_g
 * Description    : This function will return the current Paging Cycle Count.
 *****************************************************************************/

 UInt8 getPagingCycle(InternalCellIndex internalCellIndex)
{
    return pagingCycle_g[internalCellIndex];
}

/* FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
/*****************************************************************************
 * Function Name  : processTickReceivedOnL2
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates the tick, system frame and subframe
 *                  number after receiving tick on L2 fromt the system.
 *****************************************************************************/

 void processTickReceivedOnL2( InternalCellIndex internalCellIndex )
{
 /*CID 83624 FixStart*/
 currentTickFrmL2Tick_g[internalCellIndex]++;
 subFrameNumFrmL2Tick_g[internalCellIndex]++;
 /*CID 83624 FixEnd*/
    if (subFrameNumFrmL2Tick_g[internalCellIndex] >= MAX_SUB_FRAME)
    {
        subFrameNumFrmL2Tick_g[internalCellIndex] = 0;
        systemFrameNumFrmL2Tick_g[internalCellIndex]++;
        if (systemFrameNumFrmL2Tick_g[internalCellIndex] >= MAX_SFN_VALUE)
        {
            systemFrameNumFrmL2Tick_g[internalCellIndex] = 0;
        }
    }
/* SPR 19288 change start */
/* SPR 19288 change end */

    /* SPR 20283 Start - EICIC Restablishment */
    macProcessEICICAbsPattern (systemFrameNumFrmL2Tick_g[internalCellIndex], 
                 subFrameNumFrmL2Tick_g[internalCellIndex], internalCellIndex);
    /* SPR 20283 End - EICIC Restablishment */

#ifdef OTA_DL_ADVANCE_2
#ifdef TDD_CONFIG
    GET_REQUIRED_TICK(systemFrameNumFrmL2Tick_g[internalCellIndex] ,
        subFrameNumFrmL2Tick_g[internalCellIndex], 1);
#endif
#endif
/* SPR 19288 change start */
/* SPR 19288 change end */
}

/*****************************************************************************
 * Function Name  : getCurrentTickFrmL2Tick
 * Inputs         : internalCellIndex
 * Outputs        : currentTickFrmL2Tick_g
 * Returns        : currentTickFrmL2Tick_g
 * Description    : This function returns current tick which is calculated
 *                  on the receipt of L2 Tick.
 *****************************************************************************/
/* SPR 15909 fix start */
 tickType_t getCurrentTickFrmL2Tick( InternalCellIndex internalCellIndex )
/* SPR 15909 fix end */
{
    return currentTickFrmL2Tick_g[internalCellIndex];
}

/*****************************************************************************
 * Function Name  : initL2TickBeforeELExecution
 * Inputs         : internalCellIndex 
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates the systemFrameNum_g, subFrameNum_g
 *                  currentTick_g variables and other structures or globals
 *                  which are required to initialize before executing ELs after
 *                  getting new tick in L2 from the system.
 *****************************************************************************/
 void initL2TickBeforeELExecution(InternalCellIndex internalCellIndex)
{
    /* Update Tick which are being used in MAC */
    systemFrameNum_g[internalCellIndex] = 
        systemFrameNumFrmL2Tick_g[internalCellIndex];
    subFrameNum_g[internalCellIndex] =  
        subFrameNumFrmL2Tick_g[internalCellIndex];
    currentTick_g[internalCellIndex] = 
        currentTickFrmL2Tick_g[internalCellIndex];

#ifdef OVERSHOOT_CORRECTION
    SFI_PROCESSING_START(currentTick_g[internalCellIndex]);
#endif

    /* Update the Paging SFN */
    if ( 0 == subFrameNum_g[internalCellIndex] )
    {
        pagingSFN_g[internalCellIndex]++;
        if (0 == (pagingSFN_g[internalCellIndex] % MAX_SFN_VALUE))
        {
            pagingCycle_g[internalCellIndex]++;
        }
        if (pagingSFN_g[internalCellIndex] > MAX_PAGING_SFN_RANGE)
        {
            pagingSFN_g[internalCellIndex]   = 0;
            pagingCycle_g[internalCellIndex] = 0;
        }
    }

    gettimeofday_wrapper(&logMetaData.timestamp, NULL);
    logMetaData.currentTick = currentTick_g[internalCellIndex];

    LOG_UT(MAC_PHY_SF_IND_ID,LOGDEBUG,MAC_L1_INF,
            currentTick_g[internalCellIndex],
            systemFrameNum_g[internalCellIndex],
            subFrameNum_g[internalCellIndex],0,0,0,
            0.0,0.0,__func__,"MAC_PHY_SF_IND");
    /*Fix for for __SPR_634__ START*/
    macSubFrameInd(internalCellIndex);
    /*Fix for for __SPR_634__ END*/
#ifdef DL_UL_SPLIT
    lteMacUpdateStatsPerf(DL_OVERSHOOT_STATS, internalCellIndex);
    lteMacUpdateStatsPerf(UL_OVERSHOOT_STATS, internalCellIndex);
#else
    lteMacUpdateStatsPerf( internalCellIndex );
#endif
#ifdef SSI_DEBUG_STATS
    lteMacUpdateStatsWriteTTIIndex(internalCellIndex);
    lteMacResetPuschStats(internalCellIndex);
#endif
/* SPR 2302 changes start */
/* SPR 2127 changes start */
    if( 0 ==
         (currentTick_g[internalCellIndex] % MAC_STATS_INT_TICK_COUNT))
    {
        calculateDownlinkThroughPut(internalCellIndex);
        calculateUplinkThroughPut(internalCellIndex);
    }
    /* SPR 2127 changes end */
    /* SPR 2302 changes end */
    /* Decrement the error indication count for DL Config on receipt of sub
     * frame indication */
    if( 0 < numErrIndDlConfig_g[internalCellIndex] )
    {
        numErrIndDlConfig_g[internalCellIndex]--;
    }
    /* Decrement the error indication count for HI_DCI0 on receipt of sub
     * frame indication */
    if( 0 < numErrIndHiDci0_g[internalCellIndex] )
    {
        numErrIndHiDci0_g[internalCellIndex]--;
    }
}

/*****************************************************************************
 * Function Name  : initL2TickBeforeFirstELExecution
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates the systemFrameNum_g, subFrameNum_g
 *                  currentTick_g variables and other structures or globals
 *                  which are required to initialize before executing ELs first
 *                  time.
 *****************************************************************************/
 void initL2TickBeforeFirstELExecution( 
        InternalCellIndex internalCellIndex )
{
    systemFrameNum_g[internalCellIndex] = 
        systemFrameNumFrmL2Tick_g[internalCellIndex];
    subFrameNum_g[internalCellIndex] = 
        subFrameNumFrmL2Tick_g[internalCellIndex];
    currentTick_g[internalCellIndex] =
        currentTickFrmL2Tick_g[internalCellIndex];
   /*SPR 16855 +-*/
    pagingSFN_g[internalCellIndex]   = systemFrameNum_g[internalCellIndex];
   /*+SPR #3444*/
    if( pagingSFN_g[internalCellIndex] < MAX_SFN_VALUE)
   /*-SPR #3444*/
    {
        pagingCycle_g[internalCellIndex] = 0;
    }

    /*Fix for for __SPR_634__ START*/
    macFirstSubFrameIndFrmL2Tick(currentTick_g[internalCellIndex]);
    /*Fix for for __SPR_634__ END*/
#ifdef DL_UL_SPLIT
    lteMacUpdateStatsPerf(DL_OVERSHOOT_STATS, internalCellIndex);
    lteMacUpdateStatsPerf(UL_OVERSHOOT_STATS, internalCellIndex);
#else
    lteMacUpdateStatsPerf( internalCellIndex );
#endif
/* SPR 8712 changes */
#ifdef DL_DATA_SEPARATION
    semPostDefault(&waitForCellSetupIndSemEncThrd[internalCellIndex]);
#endif
/* SPR 8712 changes */
#ifdef DL_UL_SPLIT
     lteMacUpdateStatsPerfStart(DL_OVERSHOOT_STATS, internalCellIndex );
     lteMacUpdateStatsPerfStart(UL_OVERSHOOT_STATS, internalCellIndex );
#else
     lteMacUpdateStatsPerfStart( internalCellIndex );
#endif
}

/*****************************************************************************
 * Function Name  : validateSFIFrmL1
 * Inputs         : sysFrameNum - It represent the system Frame Number and
 *                  subFrameNum - It represent the Sub Frame Number
 *                  internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function validates sysFrame number and subframe Number
 *                  which is received in SFI message.
 *****************************************************************************/

 void validateSFIFrmL1(UInt16 sysFrameNum, UInt8 subFrameNum
        ,InternalCellIndex internalCellIndex)
{
    /*
    ** Check whether systemframe or subframe number which is received in SubFrame
    ** Indication message is equal to calculated system Frame and subframe number
    ** using L2 Tick.
    */
    if ((sysFrameNum != 
                systemFrameNumFrmL2Tick_g[internalCellIndex]) || 
        (subFrameNum != subFrameNumFrmL2Tick_g[internalCellIndex]))
    {
        LOG_MAC_MSG(MAC_NOT_EQ_SFN_SF_IN_SFI_WITH_L2_TICK,LOGWARNING,MAC_L1_INF,\
                currentTick_g[internalCellIndex], \
                __LINE__, \
                subFrameNum, \
                sysFrameNum, \
                subFrameNumFrmL2Tick_g[internalCellIndex], \
                systemFrameNumFrmL2Tick_g[internalCellIndex],\
                0.0,0.0,__func__,"");
    }
}

/*****************************************************************************
 * Function Name  : getSFAndSFNFrmL2Tick
 * Inputs         : sysFrameNum_p - It points to sysFrameNumber
 *                  subFrameNum_p - It points to subFrameNumber
 *                  internalCellindex
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the value of sysFrameNum_p, 
 *                  subFrameNum_p from L2 tick.
*****************************************************************************/
 void getSFAndSFNFrmL2Tick(UInt32* sysFrameNum_p , UInt32* subFrameNum_p
        ,InternalCellIndex internalCellIndex)
{
    *sysFrameNum_p = systemFrameNumFrmL2Tick_g[internalCellIndex];
    *subFrameNum_p = subFrameNumFrmL2Tick_g[internalCellIndex];
}

#endif
#ifdef MAC_AUT_TEST
/* SPR 15909 fix start */
 void setCurrentTick(tickType_t tick,InternalCellIndex internalCellIndex )
{
    /* SPR 15909 fix end */
    currentTick_g[internalCellIndex] = tick;
    subFrameNum_g[internalCellIndex] = (currentTick_g[internalCellIndex] % MAX_SUB_FRAME);
    systemFrameNum_g[internalCellIndex] = 
        (UInt16)((currentTick_g[internalCellIndex] % (MAX_SFN_VALUE * MAX_SUB_FRAME))/MAX_SUB_FRAME);
}
#endif




/*mandeep change stop*/
