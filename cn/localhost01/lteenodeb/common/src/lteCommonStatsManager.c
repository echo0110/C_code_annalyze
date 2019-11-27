/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteMacStatsManager.c
 *
 ****************************************************************************
 *
 *  File Description : This file contains the functionality that is invoked 
 *                     from execution scheduler to obtain downlink and uplink
 *                     stats in the file.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *$Log: lteMacStatsManager.c,v $
 *Revision 1.1.1.1.6.1.6.3  2010/10/25 08:36:31  gur23971
 *stats related changes
 *
 *Revision 1.1.1.1.6.1.6.2  2010/10/22 12:44:10  gur04640
 *Statistics code merged from branch BRFramework_RRC1_0_RLC_AM_MAC_8_8_FAPI_optimizations
 *
 *Revision 1.1.1.1.6.1.6.1  2010/10/11 12:56:56  gur10121
 *SPR #706 fix
 *Revision 1.1.1.1.6.1.8.3  2010/10/22 06:12:03  gur04640
 *FAPI and per TTI stats implemented
 *
 *Revision 1.1.1.1.6.1.10.1  2010/10/08 07:42:39  gur10121
 * SPR 706 Fix - KlockWork Warnings removed
 *Revision 1.1.1.1.6.1.8.2  2010/10/20 10:24:38  gur04640
 *FAPI specific PHY interface stats added
 *
 *Revision 1.1.1.1.6.1.8.1  2010/10/13 10:59:37  gur04640
 *Changes done for implementing Stats using shared memory
 *
 *Revision 1.1.1.1.6.1.8.1  2010/10/12 02:45:30  gur04640
 *Changes done for implementing Stats using shared memory
 *
 *Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 *FrameworkSetupWith1.0_MAC1.2.4
 *
 *Revision 1.24  2009/11/15 12:57:25  gur11912
 *LTE_MAC_UT_LOG(LOG_WARNING) used instead of lteWarning
 *
 *Revision 1.23  2009/11/11 13:02:49  gur19413
 *file name changed -> DownlinkstatsInfo to downlinkstatsInfo
 *
 *Revision 1.22  2009/11/02 05:33:40  gur11912
 *updated for RA Policy
 *
 *Revision 1.21  2009/11/01 11:42:19  gur11912
 *RBBitMap printed in Hexadecimal format
 *
 *Revision 1.20  2009/10/25 11:10:46  gur11912
 *DL and UL stats functionality separated
 *
 *Revision 1.19  2009/10/21 05:58:19  gur19413
 *optimized stats functionality
 *
 *Revision 1.18  2009/09/03 14:27:44  gur19413
 *bug fixed
 *
 *Revision 1.17  2009/09/02 19:24:55  gur19413
 *bug fixed
 *
 *Revision 1.16  2009/09/02 13:49:46  gur19413
 *bug fixed
 *
 *Revision 1.15  2009/09/02 13:32:07  gur19413
 *bug fixed for customized file name
 *
 *Revision 1.14  2009/08/28 13:33:10  gur19413
 *bug fixed
 *
 *Revision 1.13  2009/08/28 13:29:38  gur19413
 *replace older logger system to new one
 *
 *Revision 1.12  2009/08/21 10:48:06  gur19413
 *bug fixed
 *
 *Revision 1.11  2009/08/20 06:42:55  gur19413
 *added stat functionality for PRACH and PUCCH Reports
 *
 *Revision 1.10  2009/08/03 10:38:24  gur11083
 *changed for multiple UE stats for UL/DL
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <stddef.h>
#include <lteMisc.h>
#include "lteCommonStatsManager.h"
//#include "lteMacStatsManager.h"
//#include "lteMacTypes.h"
//#include "lteFapiMacPhyInterface.h"
/*spr 22474 start*/
    /*CSPL Code removed*/
/*spr 22474 end*/

/****************************************************************************
 Private Definitions
 ****************************************************************************/
/* Since max size of DL CONFIG msg is 65535, and the minimum size of one pdu
 * is 12, so the maximumnumber of PDUs possible in one request are
 * 65536/12 = 5462 */
#define LTE_MAC_STATS_MAX_PDU   5462

/* CA Stats Changes Start */
/* + PERF_CA GBL */
#define LTE_MAC_STATS_RF_TTI_STATS(internalcellIndex) gMacStats.pStats->\
            phyStats[internalcellIndex].radioFrameStats \
            .ttiStats[lLteMacStatsRfMulFactor_g*MAX_SFN_VALUE + \
            lLteMacStatsRfSfn_g[internalcellIndex] % LTE_MAC_STATS_MAX_SFN][lLteMacStatsRfSf_g[internalcellIndex]]
/* - PERF_CA GBL */
/* CA Stats Changes End */

#ifdef CYCLE_COUNT_MAC_RLC_PDCP
/* CA Stats Changes Start */
#define LTE_MAC_STATS_CC_ITERATOR(internalCellIndex) (gMacStats.pStats->ccIterator[(internalCellIndex)]) 
/* CA Stats Changes End */
#endif
LteMacStatsInfo gMacStats;
/* SPR23194 Start */
UInt8 kpiLcToQciMapping_g[MAX_LCID_THP];
/* SPR23194 End */
UInt8 Mimo_NonMimo_Flag_g = 0;
/* CA Stats Changes Start */


#ifdef DL_UL_SPLIT
extern void lteMacUpdateStatsPerfEnd(UInt32, InternalCellIndex);
#else
extern void lteMacUpdateStatsPerfEnd(InternalCellIndex);
#endif

#ifdef DL_DATA_SEPARATION
extern void lteMacUpdateEncoderStatsPerfEnd(InternalCellIndex internalCellIndex);
#endif
/* CA Stats Changes End */


/****************************************************************************
 Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* + PERF_CA GBL */
UInt16   lLteMacStatsRfSfn_g[MAX_NUM_CELL] = {0};
UInt16   lLteMacStatsRfSf_g[MAX_NUM_CELL] = {0};
/* - PERF_CA GBL */
#if (MAX_SFN_VALUE < LTE_MAC_STATS_MAX_SFN)
//static UInt16   lLteMacStatsRfMulFactor_g = 0;
UInt16   lLteMacStatsRfMulFactor_g = 0;
#else
#define lLteMacStatsRfMulFactor_g 0
#endif

#if (defined (DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
/* + PERF_CA GBL */
static UInt8 timerStartEndFlag[MAX_NUM_CELL][MAX_OVERSHOOT_STATS] ={{0},{0}};
#else
static UInt8 timerStartEndFlag[MAX_NUM_CELL] = {0};
/* - PERF_CA GBL */
#endif
#ifdef DL_DATA_SEPARATION
/** CA-TDD Changes Start **/
static UInt8 encoderTimerStartEndFlag[MAX_NUM_CELL] = {0};
/** CA-TDD Changes End **/
#endif
/****************************************************************************
* Function Name  : lteMacUpdateStatsPerf 
* Inputs         : none
* Outputs        : none
* Returns        : none
* Description    : This function updates the performance stats.
*
****************************************************************************/
/* CA Stats Changes Start */
#if (defined (DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
void lteMacUpdateStatsPerf(UInt32 maxStats, InternalCellIndex internalCellIndex)
#else
void lteMacUpdateStatsPerf(InternalCellIndex internalCellIndex)
#endif
{
    TimeSpec     currentTime;
    long                nsecs;
    /* ASAN changes start*/
    UInt32              secs;
    /* ASAN changes end*/
    /* If stats memory is not mapped, just return */
    if (PNULL == gMacStats.pStats)
    {
        return;
    }

    clock_gettime_real_time(&currentTime);
#if (defined (DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
    secs = currentTime.tv_sec -
            gMacStats.pStats->perfStatsSplit[internalCellIndex][maxStats].lastSfiTimeStamp.tv_sec;

    nsecs = currentTime.tv_nsec -
            gMacStats.pStats->perfStatsSplit[internalCellIndex][maxStats].lastSfiTimeStamp.tv_nsec;

    gMacStats.pStats->perfStatsSplit[internalCellIndex][maxStats].tickTime[
        gMacStats.pStats->perfStatsSplit[internalCellIndex][maxStats].tickTimeIt] =
        /*coverity-530 CID 24599*/
        (UInt64)secs * 1000000000 + nsecs;
        /*coverity-530 CID 24599*/

    if (1000000 < gMacStats.pStats->perfStatsSplit[internalCellIndex][maxStats].tickTime[
                gMacStats.pStats->perfStatsSplit[internalCellIndex][maxStats].tickTimeIt])
    {
        gMacStats.pStats->perfStatsSplit[internalCellIndex][maxStats].numTickOvershoots++;
    }
    gMacStats.pStats->perfStatsSplit[internalCellIndex][maxStats].tickTimeIt++;
    gMacStats.pStats->perfStatsSplit[internalCellIndex][maxStats].numTicks++;
    if (SFI_PROFILE_COUNT ==
            gMacStats.pStats->perfStatsSplit[internalCellIndex][maxStats].tickTimeIt)
    {
        gMacStats.pStats->perfStatsSplit[internalCellIndex][maxStats].tickTimeIt = 0;
    }
    gMacStats.pStats->perfStatsSplit[internalCellIndex][maxStats].lastSfiTimeStamp = currentTime;

#else
    secs = currentTime.tv_sec -
            gMacStats.pStats->perfStats[internalCellIndex].lastSfiTimeStamp.tv_sec;

    nsecs = currentTime.tv_nsec -
            gMacStats.pStats->perfStats[internalCellIndex].lastSfiTimeStamp.tv_nsec;

    gMacStats.pStats->perfStats[internalCellIndex].tickTime[
        gMacStats.pStats->perfStats[internalCellIndex].tickTimeIt] =
        /*coverity-530 CID 24599*/
       (UInt64)secs * 1000000000 + nsecs;
        /*coverity-530 CID 24599*/

#if 0	//ysl modified 0717: time between subframe.ind is in 0.87~1.11 
    if (1000000 < gMacStats.pStats->perfStats[internalCellIndex].tickTime[
                gMacStats.pStats->perfStats[internalCellIndex].tickTimeIt])
#else
    if (1200000 < gMacStats.pStats->perfStats[internalCellIndex].tickTime[
                gMacStats.pStats->perfStats[internalCellIndex].tickTimeIt])
#endif
    {
        gMacStats.pStats->perfStats[internalCellIndex].numTickOvershoots++;
    }
    gMacStats.pStats->perfStats[internalCellIndex].tickTimeIt++;
    gMacStats.pStats->perfStats[internalCellIndex].numTicks++;
    if (SFI_PROFILE_COUNT ==
            gMacStats.pStats->perfStats[internalCellIndex].tickTimeIt)
    {
        gMacStats.pStats->perfStats[internalCellIndex].tickTimeIt = 0;
    }
    gMacStats.pStats->perfStats[internalCellIndex].lastSfiTimeStamp = currentTime;
#endif
}

/* SPR 23393 Changes Start */
/* SPR 23393 Changes End */
/****************************************************************************
* Function Name  : lteMacUpdateStatsPerfStart 
* Inputs         : none
* Outputs        : none
* Returns        : none
* Description    : This function updates the performance stats.
*
****************************************************************************/
#if (defined (DL_UL_SPLIT) || defined (DL_UL_SPLIT_TDD))
void lteMacUpdateStatsPerfStart(UInt32 maxStats, InternalCellIndex internalCellIndex)
#else
void lteMacUpdateStatsPerfStart(InternalCellIndex internalCellIndex)
#endif
{
#if (!defined (DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD))
/* + PERF_CA GBL */
     timerStartEndFlag[internalCellIndex] = 1;  
#endif
     /* If stats memory is not mapped, just return */
     if (PNULL == gMacStats.pStats)
      {
        return;
      }
#if (defined (DL_UL_SPLIT) || defined (DL_UL_SPLIT_TDD))
if(timerStartEndFlag[internalCellIndex][maxStats] != 1)
  {
     clock_gettime_real_time(&gMacStats.pStats->perfStatsSplit[internalCellIndex][maxStats].processingStartTime);
     timerStartEndFlag[internalCellIndex][maxStats] = 1;  
  }
/* - PERF_CA GBL */
#else
     clock_gettime_real_time(&gMacStats.pStats->perfStats[internalCellIndex].processingStartTime);
#endif
}

/****************************************************************************
* Function Name  : lteMacUpdateStatsPerfEnd
* Inputs         : none
* Outputs        : none
* Returns        : none
* Description    : This function updates the performance stats.
*
****************************************************************************/

#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
/* SPR 15909 fix start */
/* + SPR 17439 */
extern tickType_t getCurrentTick(void);
/* - SPR 17439 */
/* SPR 15909 fix end */
#endif

#if (defined (DL_UL_SPLIT) || defined (DL_UL_SPLIT_TDD))
void lteMacUpdateStatsPerfEnd(UInt32 maxStats, InternalCellIndex internalcellIndex)
#else
void lteMacUpdateStatsPerfEnd(InternalCellIndex internalcellIndex)
#endif
{
    TimeSpec     currentTime;
    long                nsecs;
    int                 secs;
    if (PNULL == gMacStats.pStats)
    {
        return;
    }

    clock_gettime_real_time(&currentTime);
#if (defined (DL_UL_SPLIT) || defined (DL_UL_SPLIT_TDD))
	/* + PERF_CA GBL */
    if(maxStats == DL_OVERSHOOT_STATS && timerStartEndFlag[internalcellIndex][maxStats] == 0)
    {
       gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingStartTime.tv_sec = \
                                       gMacStats.pStats->perfStatsSplit[internalcellIndex][UL_OVERSHOOT_STATS].processingStartTime.tv_sec;
       gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingStartTime.tv_nsec = \
                                       gMacStats.pStats->perfStatsSplit[internalcellIndex][UL_OVERSHOOT_STATS].processingStartTime.tv_nsec;
       timerStartEndFlag[internalcellIndex][maxStats] = 1;
    }
    if (timerStartEndFlag[internalcellIndex][maxStats] ==1)
    {
        timerStartEndFlag[internalcellIndex][maxStats] =0;
	/* + PERF_CA GBL */
    secs = currentTime.tv_sec -
        gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingStartTime.tv_sec;

    nsecs = currentTime.tv_nsec -
        gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingStartTime.tv_nsec;

    gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTime[
        gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeIt] =
        /*coverity-530 CID 24600*/
        (UInt64)secs * 1000000000 + nsecs;
        /*coverity-530 CID 24600*/

    LTE_MAC_STATS_RF_TTI_STATS(internalcellIndex).processingTime =
    	gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTime[
	        gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeIt];

    gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].totalProcessingTime[
        gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeCumIt] += 
        gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTime[
        gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeIt];

    if ( gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTime[
            gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeIt] >  
            gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].highestTime[
            gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeCumIt])
    {
        gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].highestTime[
            gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeCumIt] =
            gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTime[
            gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeIt];
    }

    if(gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTime[
		    gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeIt] >= 1000000)
    {
	    gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].numProcessingOvershoots++;
        /* SPR 23393 Changes Start */
        /* SPR 23393 Changes End */
    }

       /* gMacStats.pStats->perfStatsSplit[maxStats].numProcessingOvershoots[
            gMacStats.pStats->perfStatsSplit[maxStats].processingTimeCumIt]++;*/

    else if (gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTime[
            gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeIt] >= 800000)
       gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].countBw800_999++;
       /* gMacStats.pStats->perfStatsSplit[maxStats].countBw800_999[
            gMacStats.pStats->perfStatsSplit[maxStats].processingTimeCumIt]++;*/

    else if (gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTime[
            gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeIt]>= 500000)
        gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].countBw500_799++;
       /* gMacStats.pStats->perfStatsSplit[maxStats].countBw500_799[
            gMacStats.pStats->perfStatsSplit[maxStats].processingTimeCumIt]++;*/

    else 
        gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].countBw0_499++;
       /* gMacStats.pStats->perfStatsSplit[maxStats].countBw0_499[
            gMacStats.pStats->perfStatsSplit[maxStats].processingTimeCumIt]++;*/

    gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].numProcessings++;
    gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeIt++;
    if (SFI_PROFILE_COUNT ==
            gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeIt)
    {
        gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeIt = 0;
        gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeCumIt++;

        if(PERF_STATS_TIME ==
                gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeCumIt)
        {
            gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeCumIt = 0;
        }
        gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].highestTime[
            gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeCumIt]=0;
        gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].totalProcessingTime[
            gMacStats.pStats->perfStatsSplit[internalcellIndex][maxStats].processingTimeCumIt]=0;
       /* gMacStats.pStats->perfStatsSplit[maxStats].numProcessingOvershoots[
            gMacStats.pStats->perfStatsSplit[maxStats].processingTimeCumIt]=0;
        gMacStats.pStats->perfStatsSplit[maxStats].countBw800_999[
            gMacStats.pStats->perfStatsSplit[maxStats].processingTimeCumIt]=0;
        gMacStats.pStats->perfStatsSplit[maxStats].countBw500_799[
            gMacStats.pStats->perfStatsSplit[maxStats].processingTimeCumIt]=0;
        gMacStats.pStats->perfStatsSplit[maxStats].countBw0_499[
            gMacStats.pStats->perfStatsSplit[maxStats].processingTimeCumIt]=0;
        */

    }
    }
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* UL_OVERSHOOT_STATS check is applied so that this counter is not added twice in same TTI */
    if((UL_OVERSHOOT_STATS == maxStats) &&  
       !(getCurrentTick() % MAC_KPI_ACTIVE_USER_SAMPLING_TIMER_TTI))
    {
/* + KPI_CA */
            UInt8  qci = 0;
        /* SPR 23402 Changes Start */ 
        /* MOCN Changes Start */
        UInt8 operatorId = 0;
        /* MOCN Changes End */
        /* SPR 23402 Changes End */
	    UInt32 activeIndexForMacStats     = (gMacStats.pStats->lteCommonKPIStats[internalcellIndex].activeIndexForMacStats);
	    LteMacKPIStats *pLteMacKPIStats   = &(gMacStats.pStats->lteCommonKPIStats[internalcellIndex].lteMacKPIStats[activeIndexForMacStats]); 

	    for(qci = 0; qci < MAX_QCI; qci++)
	    {
		    pLteMacKPIStats->totalActiveUsersPerQCIDLCumulative[qci] += gMacStats.pStats->lteCommonKPIStats[internalcellIndex].totalActiveUsersPerQciDL[qci];
		    pLteMacKPIStats->totalActiveUsersPerQCIULCumulative[qci] += gMacStats.pStats->lteCommonKPIStats[internalcellIndex].totalActiveUsersPerQCIUL[qci];
            /* SPR 23402 Changes Start */ 
            for (operatorId = 0; operatorId < MAX_NUM_MOCN_OPERATOR; operatorId++)
            {    
                pLteMacKPIStats->totalActiveUsersPerOperatorPerQCIDLCumulative[operatorId][qci] += gMacStats.pStats->lteCommonKPIStats[internalcellIndex].totalActiveUsersPerOperatorPerQciDL[operatorId][qci];
                pLteMacKPIStats->totalActiveUsersPerOperatorPerQCIULCumulative[operatorId][qci] += gMacStats.pStats->lteCommonKPIStats[internalcellIndex].totalActiveUsersPerOperatorPerQciUL[operatorId][qci];
            }
            /* SPR 23402 Changes End */
	    }
/* - KPI_CA */
    }
#endif
#else		
	/* + PERF_CA GBL */
    if (timerStartEndFlag[internalcellIndex] ==1)
    {
        timerStartEndFlag[internalcellIndex] =0;
	/* - PERF_CA GBL */
        secs = currentTime.tv_sec -
        gMacStats.pStats->perfStats[internalcellIndex].processingStartTime.tv_sec;

    nsecs = currentTime.tv_nsec -
        gMacStats.pStats->perfStats[internalcellIndex].processingStartTime.tv_nsec;

    gMacStats.pStats->perfStats[internalcellIndex].processingTime[
        gMacStats.pStats->perfStats[internalcellIndex].processingTimeIt] =
        /*coverity-530 CID 24600*/
        (UInt64)secs * 1000000000 + nsecs;
        /*coverity-530 CID 24600*/

    LTE_MAC_STATS_RF_TTI_STATS(internalcellIndex).processingTime =
    	gMacStats.pStats->perfStats[internalcellIndex].processingTime[
	        gMacStats.pStats->perfStats[internalcellIndex].processingTimeIt];

    gMacStats.pStats->perfStats[internalcellIndex].totalProcessingTime[
        gMacStats.pStats->perfStats[internalcellIndex].processingTimeCumIt] += 
        gMacStats.pStats->perfStats[internalcellIndex].processingTime[
        gMacStats.pStats->perfStats[internalcellIndex].processingTimeIt];

    if ( gMacStats.pStats->perfStats[internalcellIndex].processingTime[
            gMacStats.pStats->perfStats[internalcellIndex].processingTimeIt] >  
            gMacStats.pStats->perfStats[internalcellIndex].highestTime[
            gMacStats.pStats->perfStats[internalcellIndex].processingTimeCumIt])
    {
        gMacStats.pStats->perfStats[internalcellIndex].highestTime[
            gMacStats.pStats->perfStats[internalcellIndex].processingTimeCumIt] =
            gMacStats.pStats->perfStats[internalcellIndex].processingTime[
            gMacStats.pStats->perfStats[internalcellIndex].processingTimeIt];
    }

    if(gMacStats.pStats->perfStats[internalcellIndex].processingTime[
            gMacStats.pStats->perfStats[internalcellIndex].processingTimeIt] >= 1000000)
/* SPR 23393 Changes Start */
    {
	    gMacStats.pStats->perfStats[internalcellIndex].numProcessingOvershoots++;

    }
/* SPR 23393 Changes End */

       /* gMacStats.pStats->perfStats.numProcessingOvershoots[
            gMacStats.pStats->perfStats.processingTimeCumIt]++;*/

    else if (gMacStats.pStats->perfStats[internalcellIndex].processingTime[
            gMacStats.pStats->perfStats[internalcellIndex].processingTimeIt] >= 800000)
        gMacStats.pStats->perfStats[internalcellIndex].countBw800_999++;
       /* gMacStats.pStats->perfStats.countBw800_999[
            gMacStats.pStats->perfStats.processingTimeCumIt]++;*/

    else if (gMacStats.pStats->perfStats[internalcellIndex].processingTime[
            gMacStats.pStats->perfStats[internalcellIndex].processingTimeIt]>= 500000)
        gMacStats.pStats->perfStats[internalcellIndex].countBw500_799++;
       /* gMacStats.pStats->perfStats.countBw500_799[
            gMacStats.pStats->perfStats.processingTimeCumIt]++;*/

    else 
        gMacStats.pStats->perfStats[internalcellIndex].countBw0_499++;
       /* gMacStats.pStats->perfStats.countBw0_499[
            gMacStats.pStats->perfStats.processingTimeCumIt]++;*/

    gMacStats.pStats->perfStats[internalcellIndex].numProcessings++;
    gMacStats.pStats->perfStats[internalcellIndex].processingTimeIt++;
    if (SFI_PROFILE_COUNT ==
            gMacStats.pStats->perfStats[internalcellIndex].processingTimeIt)
    {
		if (0 == (gMacStats.pStats->perfStats[internalcellIndex].processingTimeCumIt % 10))
		{
				fprintf(stderr,"TickOvershoots %d, procOvershoots %d, highestProcTime %d, totProcTime %d\n",
						gMacStats.pStats->perfStats[internalcellIndex].numTickOvershoots,
						gMacStats.pStats->perfStats[internalcellIndex].numProcessingOvershoots,
						gMacStats.pStats->perfStats[internalcellIndex].highestTime[
            					gMacStats.pStats->perfStats[internalcellIndex].processingTimeCumIt],
						gMacStats.pStats->perfStats[internalcellIndex].totalProcessingTime[
            					gMacStats.pStats->perfStats[internalcellIndex].processingTimeCumIt]);
		}

        gMacStats.pStats->perfStats[internalcellIndex].processingTimeIt = 0;
        gMacStats.pStats->perfStats[internalcellIndex].processingTimeCumIt++;

        if(PERF_STATS_TIME ==
                gMacStats.pStats->perfStats[internalcellIndex].processingTimeCumIt)
        {
            gMacStats.pStats->perfStats[internalcellIndex].processingTimeCumIt = 0;
        }
        gMacStats.pStats->perfStats[internalcellIndex].highestTime[
            gMacStats.pStats->perfStats[internalcellIndex].processingTimeCumIt]=0;
        gMacStats.pStats->perfStats[internalcellIndex].totalProcessingTime[
            gMacStats.pStats->perfStats[internalcellIndex].processingTimeCumIt]=0;
       /* gMacStats.pStats->perfStats.numProcessingOvershoots[
            gMacStats.pStats->perfStats.processingTimeCumIt]=0;
        gMacStats.pStats->perfStats.countBw800_999[
            gMacStats.pStats->perfStats.processingTimeCumIt]=0;
        gMacStats.pStats->perfStats.countBw500_799[
            gMacStats.pStats->perfStats.processingTimeCumIt]=0;
        gMacStats.pStats->perfStats.countBw0_499[
            gMacStats.pStats->perfStats.processingTimeCumIt]=0;
        */

    }
    }
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    if(!(getCurrentTick() % MAC_KPI_ACTIVE_USER_SAMPLING_TIMER_TTI))
    {
/* + KPI_CA */
            UInt8  qci = 0;
        /* SPR 23402 Changes Start */
        /* MOCN Changes Start */
        UInt8 operatorId = 0;
        /* MOCN Changes End */
        /* SPR 23402 Changes End */
	    UInt32 activeIndexForMacStats     = (gMacStats.pStats->lteCommonKPIStats[internalcellIndex].activeIndexForMacStats);
	    LteMacKPIStats *pLteMacKPIStats   = &(gMacStats.pStats->lteCommonKPIStats[internalcellIndex].lteMacKPIStats[activeIndexForMacStats]); 

	    for(qci = 0; qci < MAX_QCI; qci++)
	    {
		    pLteMacKPIStats->totalActiveUsersPerQCIDLCumulative[qci] += gMacStats.pStats->lteCommonKPIStats[internalcellIndex].totalActiveUsersPerQciDL[qci];
		    pLteMacKPIStats->totalActiveUsersPerQCIULCumulative[qci] += gMacStats.pStats->lteCommonKPIStats[internalcellIndex].totalActiveUsersPerQCIUL[qci];
            /* SPR 23402 Changes Start */
            for (operatorId = 0; operatorId < MAX_NUM_MOCN_OPERATOR; operatorId++)
            {    
                pLteMacKPIStats->totalActiveUsersPerOperatorPerQCIDLCumulative[operatorId][qci] += gMacStats.pStats->lteCommonKPIStats[internalcellIndex].totalActiveUsersPerOperatorPerQciDL[operatorId][qci];
                pLteMacKPIStats->totalActiveUsersPerOperatorPerQCIULCumulative[operatorId][qci] += gMacStats.pStats->lteCommonKPIStats[internalcellIndex].totalActiveUsersPerOperatorPerQciUL[operatorId][qci];
            }
            /* SPR 23402 Changes End */
	    }
/* - KPI_CA */
    }
#endif
#endif
}


#ifdef DL_DATA_SEPARATION
/****************************************************************************
* Function Name  : lteMacUpdateDLDataSepStatsPerfStart 
* Inputs         : none
* Outputs        : none
* Returns        : none
* Description    : 
*
****************************************************************************/
void lteMacUpdateDLDataSepStatsPerfStart(InternalCellIndex internalCellIndex)
{
     /** CA-TDD Changes Start **/
     encoderTimerStartEndFlag[internalCellIndex]= 1;  
     /** CA-TDD Changes End **/
     /* If stats memory is not mapped, just return */
     if (PNULL == gMacStats.pStats)
     {
        return;
     }
     clock_gettime_real_time(&gMacStats.pStats->encoderPerfStats[internalCellIndex].processingStartTime);
}

/****************************************************************************
* Function Name  : lteMacUpdateEncoderStatsPerfEnd 
* Inputs         : none
* Outputs        : none
* Returns        : none
* Description    :  
*
****************************************************************************/
void lteMacUpdateEncoderStatsPerfEnd(InternalCellIndex internalcellIndex)
{
    TimeSpec     currentTime;
    long                nsecs;
    /*TDD-SPLIT Cov:40764*/
    UInt64              secs;
    /*TDD-SPLIT Cov:40764*/
    /* If stats memory is not mapped, just return */
    if (PNULL == gMacStats.pStats)
    {
        return;
    }

    clock_gettime_real_time(&currentTime);

    /** CA-TDD Changes Start **/
    if (encoderTimerStartEndFlag[internalcellIndex] == 1)
    {
        encoderTimerStartEndFlag[internalcellIndex] =0;
    /** CA-TDD Changes End **/
        secs = currentTime.tv_sec -
        gMacStats.pStats->encoderPerfStats[internalcellIndex].processingStartTime.tv_sec;

        nsecs = currentTime.tv_nsec -
            gMacStats.pStats->encoderPerfStats[internalcellIndex].processingStartTime.tv_nsec;
        /* Coverity 40764 Fix start*/
        gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTime[
            gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeIt] =
           (UInt64)secs * 1000000000 + nsecs;
        /* Coverity 40764 Fix End*/
        LTE_MAC_STATS_RF_TTI_STATS(internalcellIndex).processingTime =
            gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTime[
                gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeIt];

        gMacStats.pStats->encoderPerfStats[internalcellIndex].totalProcessingTime[
            gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeCumIt] += 
            gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTime[
            gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeIt];

        if ( gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTime[
                gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeIt] >  
                gMacStats.pStats->encoderPerfStats[internalcellIndex].highestTime[
                gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeCumIt])
        {
            gMacStats.pStats->encoderPerfStats[internalcellIndex].highestTime[
                gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeCumIt] =
                gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTime[
                gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeIt];
        }

        if(gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTime[
                gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeIt] >= 1000000)
            gMacStats.pStats->encoderPerfStats[internalcellIndex].numProcessingOvershoots++;


        else if (gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTime[
                gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeIt] >= 800000)
            gMacStats.pStats->encoderPerfStats[internalcellIndex].countBw800_999++;

        else if (gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTime[
                gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeIt]>= 500000)
            gMacStats.pStats->encoderPerfStats[internalcellIndex].countBw500_799++;

        else 
            gMacStats.pStats->encoderPerfStats[internalcellIndex].countBw0_499++;

        gMacStats.pStats->encoderPerfStats[internalcellIndex].numProcessings++;
        gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeIt++;
        if (ENCODER_PROFILE_COUNT ==
                gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeIt)
        {
            gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeIt = 0;
            gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeCumIt++;

            if(ENCODER_PERF_STATS_TIME ==
                    gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeCumIt)
            {
                gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeCumIt = 0;
            }
            gMacStats.pStats->encoderPerfStats[internalcellIndex].highestTime[
                gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeCumIt]=0;
            gMacStats.pStats->encoderPerfStats[internalcellIndex].totalProcessingTime[
                gMacStats.pStats->encoderPerfStats[internalcellIndex].processingTimeCumIt]=0;
    }
    }
}
#endif

/* CA Stats Changes End */
/*spr 22474 start*/
    /*CSPL Code removed*/
/*spr 22474 end*/

#ifdef RLC_QUEUE_STATS
/****************************************************************************
 * * Function Name  : lteMacUpdateStatsQueuePerUePerLc
 * * Inputs         : Queue Identifier,UEId,LCId,queueCount
 * * Outputs        : none
 * * Returns        : none
 * * Description    : This function updates the queue occupancy stats for resppective
 * 		      Queue based on Queue Identifier occupied by a paricular UE per LC
 * *
 * ****************************************************************************/
void lteMacUpdateStatsQueuePerUePerLc(UInt16 perUeperLcQueueId,UInt16 UeId, UInt8 LcId,UInt16 queueCount)
{
    if(PNULL ==gMacStats.pStats)
    {
        return ;
    }

    gMacStats.pStats->queuePerUePerLcStats.qPerUePerLcStats[perUeperLcQueueId].currentOccupancyPerUePerLc[UeId][LcId] = queueCount;
    if(gMacStats.pStats->queuePerUePerLcStats.qPerUePerLcStats[perUeperLcQueueId].peakOccupancyPerUePerLc[UeId][LcId] < queueCount)
    {
        gMacStats.pStats->queuePerUePerLcStats.qPerUePerLcStats[perUeperLcQueueId].peakOccupancyPerUePerLc[UeId][LcId] = queueCount;
    }

}

/****************************************************************************
 * * Function Name  : lteMacUpdateStatsQueue
 * * Inputs         : Queue Identifier,UEId,LCId,queueCount
 * * Outputs        : none
 * * Returns        : none
 * * Description    : This function updates the queue occupancy stats for resppective
 * 		      Queue based on Queue Identifier not UEId or LCId specific
 * *
 * ****************************************************************************/
void lteMacUpdateStatsQueue(UInt16 queueId, UInt16 queueCount)
{
    if(PNULL ==gMacStats.pStats)
    {
        return ;
    }
    gMacStats.pStats->queueStats.qStats[queueId].currentOccupancy = queueCount;
    if(gMacStats.pStats->queueStats.qStats[queueId].peakOccupancy < queueCount)
       {
            gMacStats.pStats->queueStats.qStats[queueId].peakOccupancy = queueCount;
       } 


}
#endif

#ifdef ACK_NACK_COUNT_STATS


/****************************************************************************
 * * Function Name  : lteCommonUpdateAckNackStats
 * * Inputs         : TBtype,ueId,ackOrNack
 * * Outputs        : none
 * * Returns        : none
 * * Description    : This function updates the Ack Nack Count for resppective
 * 		      UE 
 * *
 * ****************************************************************************/
void lteCommonUpdateAckNackStats(UInt8 TBtype, UInt16 ueId, UInt8 ackOrNack)
{
    if(PNULL ==gMacStats.pStats)
    {
        return ;
    }
    switch(TBtype)
    {
     case MIMO_TB_ONE:
             gMacStats.pStats->debugStats.Mimo_NonMimo_Flag = 2;
	      if(ackOrNack == DL_ACK)
               {
	          gMacStats.pStats->debugStats.TBOneAckNack[ueId].ackCount ++;
               }
	     else
	          gMacStats.pStats->debugStats.TBOneAckNack[ueId].nackCount ++;
            break;

     case MIMO_TB_TWO:
              gMacStats.pStats->debugStats.Mimo_NonMimo_Flag = 2;
             if(ackOrNack == DL_ACK)
               {
	          gMacStats.pStats->debugStats.TBTwoAckNack[ueId].ackCount ++;
               }
	     else
	          gMacStats.pStats->debugStats.TBTwoAckNack[ueId].nackCount ++;
            break;
     case NON_MIMO:
             gMacStats.pStats->debugStats.Mimo_NonMimo_Flag = 0;
             if(ackOrNack == DL_ACK)
               {
	          gMacStats.pStats->debugStats.TBOneAckNack[ueId].ackCount ++;
               }
	     else
	          gMacStats.pStats->debugStats.TBOneAckNack[ueId].nackCount ++;
            break;
               default:
               fprintf(stderr,"\nNo a valid TB type reported for Ack Nack count");
	       break;
     }
  }   
#endif

/****************************************************************************
* Function Name  : getTimeInMicroSec 
* Inputs         : start and end timestamp
* Outputs        : none
* Returns        : none
* Description    : This function finds the time diff in microsec between 
*                  two timestamps
*
****************************************************************************/
UInt64 getTimeInMicroSec(TimeSpec *  start, TimeSpec *  end)
{
    /* + Coverity 32013 */
    ULong32 diffInMicroseconds = 0;
    ULong32 diffInMicroseconds_nsec = 0;
    ULong32 diffInMicroseconds_sec = 0;
    /* - Coverity 32013 */

    diffInMicroseconds_sec = (end->tv_sec - start->tv_sec)*1000000;
    diffInMicroseconds_nsec = (end->tv_nsec - start->tv_nsec)/1000;
    diffInMicroseconds = diffInMicroseconds_sec + diffInMicroseconds_nsec;
    return (diffInMicroseconds);
}

#ifdef CYCLE_COUNT_MAC_RLC_PDCP

__thread TimeSpec prof_start_clock_g = {0},prof_end_clock_g = {0}; 
__thread UInt64 prof_start_count_g = 0, prof_end_count_g = 0;
__thread UInt64 prof_cycle_count_used_g = 0;
__thread TimeSpec socket_start_clock_g, socket_end_clock_g;
__thread UInt64 socket_start_count_g, socket_end_count_g;
__thread UInt64 socket_cycle_count_used_g;
__thread UInt32 ExeLegId = 0;

/*SPR 10852 fix start */
#ifdef DL_DATA_SEPARATION
__thread TimeSpec prof_start_clock_DDS_g , prof_end_clock_DDS_g;
__thread UInt64 prof_start_count_DDS_g, prof_end_count_DDS_g;
__thread UInt64 prof_cycle_count_used_DDS_g;
#endif
/*SPR 10852 fix end */

/* CA Stats Changes Start */
#if defined(TDD_CONFIG) && !defined(DL_UL_SPLIT_TDD)
/****************************************************************************
* Function Name  : legUpdateCycleCount_clkTimeStats 
* Inputs         : Function Identifier and the cycle count value
* Outputs        : none
* Returns        : none
* Description    : This function updates the leg specific cycle count stats for
*                  respective fucntion based on the function identifier
*                  received and also increments the respective num of cycles.
*
****************************************************************************/
void legUpdateCycleCount_clkTimeStats(UInt32 funcId,UInt64 cycleCount,
                     TimeSpec *start, TimeSpec *end, InternalCellIndex internalcellIndex)
{
  if (PNULL == gMacStats.pStats)
  {
   return;
  }
  gMacStats.pStats->ccStats[internalcellIndex][LTE_MAC_STATS_CC_ITERATOR(internalcellIndex)].legCCStats[funcId].totalFuncCycleCount +=
  cycleCount;
#ifdef GETSTATS_EL_CLOCKTIME  
  UInt64 clkTime = 0;
  clkTime = getTimeInMicroSec(start,end);
  gMacStats.pStats->ccStats[internalcellIndex][LTE_MAC_STATS_CC_ITERATOR(internalcellIndex)].legCCStats[funcId].totalFuncClkTimeInMicroSec += 
  clkTime;
  LTE_MAC_STATS_RF_TTI_STATS(internalcellIndex).legFuncClkTimeInMicroSecPerTick[funcId]=clkTime;
 #else
    (void)start;
    (void)end;
#endif  
  gMacStats.pStats->ccStats[internalcellIndex][LTE_MAC_STATS_CC_ITERATOR(internalcellIndex)].legCCStats[funcId].numCycles++;
  LTE_MAC_STATS_RF_TTI_STATS(internalcellIndex).legCyclesPerTick[funcId]=cycleCount;

}
#endif
#if (defined(FDD_CONFIG) || defined(DL_UL_SPLIT_TDD))
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
/****************************************************************************
* Function Name  : updateCycleCount_clkTimeStats
* Inputs         : Function Identifier and the cycle count value
* Outputs        : none
* Returns        : none
* Description    : This function updates the mac/rlc cycle count stats for
*                  respective fucntion based on the function identifier
*                  received and also increments the respective num of cycles.
*
****************************************************************************/
void updateCycleCount_clkTimeStats(UInt32 funcId,UInt64 cycleCount,
                     TimeSpec *start, TimeSpec *end, InternalCellIndex internalcellIndex)
{
#ifdef GETSTATS_EL_CLOCKTIME
  UInt64 clkTime = 0;
#endif
  if (PNULL == gMacStats.pStats)
  {
   return;
  }
  gMacStats.pStats->ccStats[internalcellIndex][LTE_MAC_STATS_CC_ITERATOR(internalcellIndex)].cycleCountStats[funcId].totalFuncCycleCount +=
  cycleCount;
#ifdef GETSTATS_EL_CLOCKTIME
  clkTime = getTimeInMicroSec(start,end);
  gMacStats.pStats->ccStats[internalcellIndex][LTE_MAC_STATS_CC_ITERATOR(internalcellIndex)].cycleCountStats[funcId].totalFuncClkTimeInMicroSec +=
  clkTime;
  LTE_MAC_STATS_RF_TTI_STATS(internalcellIndex).macFuncClkTimeInMicroSecPerTick[funcId]=clkTime;
#else
    (void)start;
    (void)end;
#endif
  gMacStats.pStats->ccStats[internalcellIndex][LTE_MAC_STATS_CC_ITERATOR(internalcellIndex)].cycleCountStats[funcId].numCycles++;
  LTE_MAC_STATS_RF_TTI_STATS(internalcellIndex).macCyclesPerTick[funcId]=cycleCount;
 
}
#else
/****************************************************************************
* Function Name  : macUpdateCycleCount_clkTimeStats 
* Inputs         : Function Identifier and the cycle count value
* Outputs        : none
* Returns        : none
* Description    : This function updates the mac cycle count stats for 
*                  respective fucntion based on the function identifier
*                  received and also increments the respective num of cycles.
*
****************************************************************************/
void macUpdateCycleCount_clkTimeStats(UInt32 macFuncId,UInt64 cycleCount,
                     TimeSpec *start, TimeSpec *end, InternalCellIndex internalcellIndex)
{
    if (PNULL == gMacStats.pStats)
    {
        return;
    }
    gMacStats.pStats->ccStats[internalcellIndex][LTE_MAC_STATS_CC_ITERATOR(internalcellIndex)].macCCStats[macFuncId].totalFuncCycleCount +=
        cycleCount;
    /*SPR 3643 changes starts*/
#ifdef GETSTATS_EL_CLOCKTIME
    UInt64 clkTime = 0;
    /*SPR 3643 changes ends*/
    clkTime = getTimeInMicroSec(start,end);
    gMacStats.pStats->ccStats[internalcellIndex][LTE_MAC_STATS_CC_ITERATOR(internalcellIndex)].macCCStats[macFuncId].totalFuncClkTimeInMicroSec += 
        clkTime;
    LTE_MAC_STATS_RF_TTI_STATS(internalcellIndex).macFuncClkTimeInMicroSecPerTick[macFuncId]=clkTime;
    /*SPR 3643 changes starts*/
    /*SPR 18490 +-*/
#else
    (void)start;
    (void)end;
#endif 
    /*SPR 18490*/
    /*SPR 3643 changes ends*/
    gMacStats.pStats->ccStats[internalcellIndex][LTE_MAC_STATS_CC_ITERATOR(internalcellIndex)].macCCStats[macFuncId].numCycles++;
    LTE_MAC_STATS_RF_TTI_STATS(internalcellIndex).macCyclesPerTick[macFuncId]=cycleCount;
}

/****************************************************************************
* Function Name  : rlcUpdateCycleCount_clkTimeStats 
* Inputs         : Function Identifier and the cycle count value
* Outputs        : none
* Returns        : none
* Description    : This function updates the rlc cycle count stats for 
*                  respective fucntion based on the function identifier
*                  received and also increments the respective num of cycles.
*
****************************************************************************/
void rlcUpdateCycleCount_clkTimeStats(UInt32 rlcFuncId,UInt64 cycleCount,
                     TimeSpec *start, TimeSpec *end, InternalCellIndex internalCellIndex)
{
    if (PNULL == gMacStats.pStats)
    {
        return;
    }
    gMacStats.pStats->ccStats[internalCellIndex][LTE_MAC_STATS_CC_ITERATOR(internalCellIndex)].rlcCCStats[rlcFuncId].totalFuncCycleCount +=
        cycleCount;
    /*SPR 3643 changes starts*/
#ifdef GETSTATS_EL_CLOCKTIME
    UInt64 clkTime = 0;
    /*SPR 3643 changes ends*/
    clkTime = getTimeInMicroSec(start,end);
    gMacStats.pStats->ccStats[internalCellIndex][LTE_MAC_STATS_CC_ITERATOR(internalCellIndex)].rlcCCStats[rlcFuncId].totalFuncClkTimeInMicroSec += 
        clkTime;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).rlcFuncClkTimeInMicroSecPerTick[rlcFuncId]=clkTime;
    /*SPR 3643 changes starts*/
    /*SPR 18490 +-*/
#else
    (void)start;
    (void)end;
#endif 
    /*SPR 18490*/
    /*SPR 3643 changes ends*/
    gMacStats.pStats->ccStats[internalCellIndex][LTE_MAC_STATS_CC_ITERATOR(internalCellIndex)].rlcCCStats[rlcFuncId].numCycles++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).rlcCyclesPerTick[rlcFuncId]=cycleCount;
}
#endif
#endif
#if (!defined(TDD_CONFIG) || defined(DL_UL_SPLIT_TDD))
/****************************************************************************
* Function Name  : pdcpUpdateCycleCount_clkTimeStats 
* Inputs         : Function Identifier and the cycle count value
* Outputs        : none
* Returns        : none
* Description    : This function updates the pdcp cycle count stats for 
*                  respective fucntion based on the function identifier
*                  received and also increments the respective num of cycles.
*
****************************************************************************/
void pdcpUpdateCycleCount_clkTimeStats(UInt32 pdcpFuncId,UInt64 cycleCount,
                     TimeSpec *start,TimeSpec *end, InternalCellIndex internalCellIndex)
{
#if (!defined (DL_UL_SPLIT) || defined (GETSTATS_EL_CLOCKTIME))
  UInt64 clkTime;
#endif
  if (PNULL == gMacStats.pStats)
  {
   return;
  }
 gMacStats.pStats->ccStats[internalCellIndex][LTE_MAC_STATS_CC_ITERATOR(internalCellIndex)].pdcpCCStats[pdcpFuncId].totalFuncCycleCount +=
 cycleCount;
#if (!defined (DL_UL_SPLIT) || defined (GETSTATS_EL_CLOCKTIME))
 clkTime = getTimeInMicroSec(start,end);
 gMacStats.pStats->ccStats[internalCellIndex][LTE_MAC_STATS_CC_ITERATOR(internalCellIndex)].pdcpCCStats[pdcpFuncId].totalFuncClkTimeInMicroSec += 
 clkTime;
#ifdef GETSTATS_EL_CLOCKTIME
 LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).pdcpFuncClkTimeInMicroSecPerTick[pdcpFuncId]=clkTime;
#endif
#endif
 gMacStats.pStats->ccStats[internalCellIndex][LTE_MAC_STATS_CC_ITERATOR(internalCellIndex)].pdcpCCStats[pdcpFuncId].numCycles++;
 LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).pdcpCyclesPerTick[pdcpFuncId]=cycleCount;

}
#endif

#ifdef DL_DATA_SEPARATION
/****************************************************************************
* Function Name  : encoderUpdateCycleCount_clkTimeStats 
* Inputs         : Function Identifier and the cycle count value
* Outputs        : none
* Returns        : none
* Description    : This function updates the encoder cycle count stats for 
*                  respective fucntion based on the function identifier
*                  received and also increments the respective num of cycles.
*
****************************************************************************/
void encoderUpdateCycleCount_clkTimeStats(UInt32 encoderFuncId,UInt64 cycleCount,
                     TimeSpec *start, TimeSpec *end, InternalCellIndex internalCellIndex)
{
#ifdef GETSTATS_EL_CLOCKTIME
  UInt64 clkTime = 0;
#endif
  if (PNULL == gMacStats.pStats)
  {
   return;
  }
 gMacStats.pStats->ccStats[internalCellIndex][LTE_MAC_STATS_CC_ITERATOR(internalCellIndex)].encoderCCStats[encoderFuncId].totalFuncCycleCount +=
 cycleCount;
#ifdef GETSTATS_EL_CLOCKTIME
 clkTime = getTimeInMicroSec(start,end);
 gMacStats.pStats->ccStats[internalCellIndex][LTE_MAC_STATS_CC_ITERATOR(internalCellIndex)].encoderCCStats[encoderFuncId].totalFuncClkTimeInMicroSec += 
 clkTime;
//RD:  LTE_MAC_STATS_RF_TTI_STATS.encoderFuncClkTimeInMicroSecPerTick[encoderFuncId]=clkTime;
#else 
    (void)start;/*SPR 19428 +-*/
    (void)end;
#endif 
 gMacStats.pStats->ccStats[internalCellIndex][LTE_MAC_STATS_CC_ITERATOR(internalCellIndex)].encoderCCStats[encoderFuncId].numCycles++;
//RD:  LTE_MAC_STATS_RF_TTI_STATS.encoderCyclesPerTick[encoderFuncId]=cycleCount; 

}
#endif

void lteIncrementL2Cycles(InternalCellIndex internalcellIndex)
{
  if (PNULL == gMacStats.pStats)
  {
   return;
  }

    if (gMacStats.pStats->ccStats[internalcellIndex][LTE_MAC_STATS_CC_ITERATOR(internalcellIndex)].totalL2Cycles >= 
            NUM_L2_FUNC_CYCLES_LIMIT)
    {
        gMacStats.pStats->ccIterator[internalcellIndex] = 
            (gMacStats.pStats->ccIterator[internalcellIndex] + 1) % NUM_L2_CC_ITERATIONS;
  // pdcpCCStats.pdcpCCIterator = gMacStats.pStats->ccIterator; 
        gMacStats.pStats->ccStats[internalcellIndex][LTE_MAC_STATS_CC_ITERATOR(internalcellIndex)].totalL2Cycles = 0; 
  }
    gMacStats.pStats->ccStats[internalcellIndex][LTE_MAC_STATS_CC_ITERATOR(internalcellIndex)].totalL2Cycles++;
 
}

#if (!defined(TDD_CONFIG) || defined(DL_UL_SPLIT_TDD))
/****************************************************************************
* Function Name  : socketUpdateCycleCount_clkTimeStats 
* Inputs         : none
* Outputs        : none
* Returns        : none
* Description    : This function updates the socket cycle count stats for 
*                  respective Execution Leg identifier stored in global ExeLegId
*                  and also increments the respective num of cycles.
*
****************************************************************************/
void socketUpdateCycleCount_clkTimeStats(UInt32 FuncId,UInt64 cycleCount,
                     TimeSpec *start, TimeSpec *end, InternalCellIndex internalCellIndex)
{
    if (PNULL == gMacStats.pStats)
    {
        return;
    }
    gMacStats.pStats->ccStats[internalCellIndex][LTE_MAC_STATS_CC_ITERATOR(internalCellIndex)].socketCCStats[FuncId].totalFuncCycleCount +=
        cycleCount;
    /*SPR 3643 changes starts*/
#ifdef GETSTATS_EL_CLOCKTIME
    UInt64 clkTime = 0;
    /*SPR 3643 changes ends*/
    clkTime = getTimeInMicroSec(start,end);
    gMacStats.pStats->ccStats[internalCellIndex][LTE_MAC_STATS_CC_ITERATOR(internalCellIndex)].socketCCStats[FuncId].totalFuncClkTimeInMicroSec += 
        clkTime;
    /*SPR 3643 changes starts*/
    /*SPR 18490 +-*/
#else
    (void)start;
    (void)end;
#endif  
    /*SPR 3643 changes ends*/
    gMacStats.pStats->ccStats[internalCellIndex][LTE_MAC_STATS_CC_ITERATOR(internalCellIndex)].socketCCStats[FuncId].numCycles++;
}
/* CA Stats Changes End */
#endif
#endif


/*CA Changes start */
/******************************************************************************
 * FUNCTION NAME :updateCAPRBUsageStats
 * INPUTS :1)currentDLPRBUsage - Current value of the prb-usage.
 *         2)currentTick - it is the current tick.
 *         3)ldCalcWindowSize - it is the load calculation window size.
 *         4)dlAvailableRBs - total dl avilable RBs           
 *         5)internalCellIndex - Cell index to maintain stats for PCELL and 
 *                                SCELL.
 * OUTPUTS:1)None
 * DESCRIPTION: This function will update the totalDLPRBUsagePercent for 
 *              each cell. 
 ****************************************************************************/
                           /*SPR 15909 fix start*/
void updateCAPRBUsageStats(tickType_t currentTick,UInt64 currentDLPRBUsage,InternalCellIndex internalCellIndex,
                           /*SPR 15909 fix end*/
                            UInt8 ldCalcWindowSize,UInt32 dlAvailableRBs)
{  
#ifndef FLEXRAN
/* Random crashes are observed for FELXRAN in CA code.
 * CA is yet to be verified/intergated on CRAN hence commented out the code
 * */
    /*SPR 15909 fix start*/
    tickType_t tickDiff = 0;
    /*SPR 15909 fix end*/
    /*set the totalDLPRBUsage for the first tick also update the lastCalculatedTTI*/
    if(gMacStats.pStats->lteMacCADLPrbUsageStats[internalCellIndex].lastCalculatedTTI == 0)
    {
        gMacStats.pStats->lteMacCADLPrbUsageStats[internalCellIndex].lastCalculatedTTI = currentTick;
     
        gMacStats.pStats->lteMacCADLPrbUsageStats[internalCellIndex].totalDLPRBUsage += currentDLPRBUsage;

        /* + SPR 11658 Changes */
        gMacStats.pStats->lteMacCADLPrbUsageStats[internalCellIndex].totalDLPRBUsagePercent = \
           ((gMacStats.pStats->lteMacCADLPrbUsageStats[internalCellIndex].totalDLPRBUsage * 100) /(ldCalcWindowSize * \
            dlAvailableRBs));
        /* - SPR 11658 Changes */

    }
    else
    {
        /*set the totalDLPRBUsage also update the lastCalculatedTTI and totalDLPRBUsagePercent for 
          subsequent ticks when tick difference is > than ldCalcWindowSize*/
        tickDiff = currentTick - gMacStats.pStats->lteMacCADLPrbUsageStats[internalCellIndex].lastCalculatedTTI;
   
        gMacStats.pStats->lteMacCADLPrbUsageStats[internalCellIndex].totalDLPRBUsage += currentDLPRBUsage;
    
        /* + SPR 11658 Changes */
        if(tickDiff >= ldCalcWindowSize)
        {
            gMacStats.pStats->lteMacCADLPrbUsageStats[internalCellIndex].totalDLPRBUsagePercent = \
            ((gMacStats.pStats->lteMacCADLPrbUsageStats[internalCellIndex].totalDLPRBUsage * 100) /(ldCalcWindowSize * \
            dlAvailableRBs));
        /* - SPR 11658 Changes */
       
            /*update lastCalculatedTTI and reset totalDLPRBUsage*/   
            gMacStats.pStats->lteMacCADLPrbUsageStats[internalCellIndex].lastCalculatedTTI = currentTick;        
            gMacStats.pStats->lteMacCADLPrbUsageStats[internalCellIndex].totalDLPRBUsage = 0;
        }
    }
#endif
}
/*CA Changes end */
