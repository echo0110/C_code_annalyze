/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcStat.c,v 1.1.1.1.12.2.2.1 2010/09/21 15:50:14 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcStat.c,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:50:14  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:24:21  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.16.2.4  2010/07/20 05:47:46  gur19836
 * Code Review Comment: Indentation and 80 column width: Incorporated
 *
 * Revision 1.16.2.3  2010/07/17 11:43:58  gur23054
 * Review Comments fixed
 *
 * Revision 1.16.2.2  2010/06/18 07:30:57  gur19836
 * Corrected Indentation
 *
 * Revision 1.16.2.1  2010/06/17 10:59:54  gur19836
 * RLC AM Base Code Merged
 *
 * Revision 1.16  2010/06/14 12:02:40  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.15  2009/06/04 05:20:41  gur19836
 * Optimization Changes
 *
 * Revision 1.14  2009/04/15 16:04:22  gur19140
 * rlc latency + logging related changes
 *
 * Revision 1.13  2009/04/07 16:04:46  gur20548
 * update in rrc and oam
 *
 * Revision 1.12  2009/04/07 15:02:16  gur19140
 * peer to peer fixes
 *
 * Revision 1.11  2009/03/25 06:55:13  gur20439
 * LTE_LOG macro called modified as per the new macro.
 *
 * Revision 1.10  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.9  2009/03/02 05:34:11  gur19836
 * Updated for OAM Enhancement
 *
 * Revision 1.8  2009/02/20 06:23:03  gur19140
 * minor changes
 *
 * Revision 1.7  2009/02/19 08:36:33  gur19140
 * tx fixes
 *
 * Revision 1.6  2009/02/18 14:36:41  gur20294
 * added new function printStat
 *
 * Revision 1.5  2009/02/18 14:32:21  gur20294
 * updated with function
 *
 * Revision 1.4  2009/02/18 14:20:10  gur20294
 * added function printStat
 *
 * Revision 1.3  2009/02/05 10:58:22  gur19836
 * CR comments fixed
 *
 * Revision 1.2  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcStat.h"
#include "lteRlcUeContext.h"
#include "lteCommonStatsManager.h"
#include "lteMisc.h"


/****************************************************************************
 * Exported Variables
 ****************************************************************************/
UInt32 activeUEs_g = 0 ;
/* + PERF_CA GBL */
lteRlcStatInfo lteRlcStatInfo_g[MAX_NUM_CELL] ;
/* - PERF_CA GBL */
LteRlcShmStatsInfo gRlcStats;
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */
#ifdef  CYCLE_COUNT_MAC_RLC_PDCP
 __thread TimeSpec rlc_start_clock_g ={0} , rlc_end_clock_g = {0};
 __thread UInt32 rlc_start_count_g = 0 , rlc_end_count_g = 0;
 __thread UInt32 rlc_cycle_count_used_g = 0;
#endif
/****************************************************************************
* Function Name  : initRlcStats
* Inputs         : None
* Outputs        : None
* Returns        : None
* Description    : Increments the uplink tick index when all the RB info is
*                  filled by the fillULstatsinfo function for a tick.
*
****************************************************************************/
 /* + SPR 17439 */
UInt32 initRlcStats(void)
/* - SPR 17439 */    
{
    /*START:SR:CSR:00024693:SPR5275*/
    SInt32 shmId = 0;

    shmId = shmget_wrapper(RLC_STATS_SHM_KEY,0, RLC_STATS_SHM_FLAGS);
    if(shmId >= 0)
    {
        /* IF key exist then remove SHM*/
        shmctl_wrapper_ipc_rmid(shmId);

    }
    /*END:SR:CSR:00024693:SPR5275*/
    /* Get the shared memory ID */
    gRlcStats.shmId = shmget_wrapper(RLC_STATS_SHM_KEY, sizeof(LteRlcStats),
                             RLC_STATS_SHM_FLAGS);
    if (0 > gRlcStats.shmId)
    {
        /*START:SR:CSR:00024693:SPR5275*/
        ltePanic("SHM(%d) get failed",RLC_STATS_SHM_KEY);
        /*END:SR:CSR:00024693:SPR5275*/
        return RLC_FAILURE;
    }

    /* Map Shared memory */
    gRlcStats.pRlcStats = shmat_wrapper(gRlcStats.shmId, NULL, 0);
    if ((void *)-1 == gRlcStats.pRlcStats)
    {
        /*START:SR:CSR:00024693:SPR5275*/
        ltePanic("SHM(%d) attach failed",RLC_STATS_SHM_KEY);
        /*END:SR:CSR:00024693:SPR5275*/
        //perror("SHM attach failed");
        return RLC_FAILURE;
    }

    /* Initialize shared memory */
    memSet(gRlcStats.pRlcStats, 0, sizeof(LteRlcStats));
    //memSet(&(gRlcStats.pRlcStats->lteSysStat), 0, sizeof(LteRlcSysStatInfo)); 
    //memSet(&(gRlcStats.pRlcStats->lteRlcApiStat), 0, sizeof(LteRlcApiStatInfo)); 
    //memSet(&(gRlcStats.pRlcStats->lteRlcUeStat), 0, sizeof(LteRlcUEInfo)); 

    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : rlcResetStat
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to reset RLC Stat.
 ****************************************************************************/
/* + PERF_CA GBL */
void rlcResetStat(InternalCellIndex internalCellIndex)
{
	/*SPR 21769 Changes Start*/
    UInt16 index = 0; 
	/*SPR 21769 Changes End*/


    memSet(&(lteRlcStatInfo_g[internalCellIndex].lteUMStat),0,sizeof(lteRlcUMStatInfo));
    memSet(&(lteRlcStatInfo_g[internalCellIndex].lteAMStat),0,sizeof(lteRlcAMStatInfo));

    /* SPR 3092, 3520 START */
    for (index =0; index <activeUEs_g ; index ++)
    {
        lteRlcStatInfo_g[internalCellIndex].lteUEStat.rlcUeStats[index].sduReceivedFromPdcp = 0;
        lteRlcStatInfo_g[internalCellIndex].lteUEStat.rlcUeStats[index].pduTransmittedToMac = 0;
        lteRlcStatInfo_g[internalCellIndex].lteUEStat.rlcUeStats[index].sduTransmittedToPdcp = 0;
        lteRlcStatInfo_g[internalCellIndex].lteUEStat.rlcUeStats[index].pduReceivedFromMac = 0;
    }

    /* SPR 3092, 3520 END */
}
/* - PERF_CA GBL */
/****************************************************************************
 * Function Name  : updateRlcPdcpApiStats
 * Inputs         : rlcPdcpApiId - RLC PDCP API ID,
 *                  cyclecount - Count of cycles,
 *                  start - Pointer to TimeSpec,
 *                  end - Pointer to TimeSpec
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to calculate RLC-PDCP Api stats like
 *                  clocktime, cyclecount 
 ****************************************************************************/
void updateRlcPdcpApiStats(UInt32 rlcPdcpApiId,UInt32 cycleCount,
                             TimeSpec *start, TimeSpec *end)
{
    /* SPR 15909 fix start */
    tickType_t clkTime = 0;
    /* SPR 15909 fix end */
    LteRlcApiStats* rlcPdcpStats_p = &(gRlcStats.pRlcStats->lteRlcApiStat.
        rlcPdcpApiStats[rlcPdcpApiId]);
    rlcPdcpStats_p->totalCycleCount += cycleCount;
    rlcPdcpStats_p->numOfCalls ++;
    clkTime = getTimeInMicroSec(start,end);
    rlcPdcpStats_p->totalTime += clkTime;

    if(rlcPdcpStats_p->numOfCalls == 1)
    {
        rlcPdcpStats_p->highestTime = clkTime;
        rlcPdcpStats_p->highestCycleCount = cycleCount;
        rlcPdcpStats_p->highTimeStamp.tv_sec = start->tv_sec; 
        rlcPdcpStats_p->highTimeStamp.tv_nsec = start->tv_nsec;
        rlcPdcpStats_p->highTick = globalTTITickCount_g;
    }
    else
    {
        if(rlcPdcpStats_p->highestCycleCount < cycleCount)
        {
            rlcPdcpStats_p->highestTime = clkTime;
            rlcPdcpStats_p->highestCycleCount = cycleCount;
            rlcPdcpStats_p->highTimeStamp.tv_sec = start->tv_sec; 
            rlcPdcpStats_p->highTimeStamp.tv_nsec = start->tv_nsec;
            rlcPdcpStats_p->highTick = globalTTITickCount_g;
        }
    }
}

/****************************************************************************
 * Function Name  : updateRlcMacApiStats
 * Inputs         : rlcMacApiId - RLC MAC API ID,
 *                  cyclecount - Count of cycles,
 *                  start - Pointer to TimeSpec,
 *                  end - Pointer to TimeSpec
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to calculate RLC-PDCP Api stats like
 *                  clocktime, cyclecount.
 ****************************************************************************/
void updateRlcMacApiStats(UInt32 rlcMacApiId,UInt32 cycleCount,
                             TimeSpec *start, TimeSpec *end)
{
    /* SPR 15909 fix start */
    tickType_t clkTime = 0;
    /* SPR 15909 fix end */
    LteRlcApiStats* rlcMacStats_p = &(gRlcStats.pRlcStats->lteRlcApiStat.
        rlcMacApiStats[rlcMacApiId]);
    
    rlcMacStats_p->totalCycleCount += cycleCount;
    rlcMacStats_p->numOfCalls ++;
    clkTime = getTimeInMicroSec(start,end);
    rlcMacStats_p->totalTime += clkTime;


    
    if(rlcMacStats_p->numOfCalls == 1)
    {
        rlcMacStats_p->highestTime = clkTime;
        rlcMacStats_p->highestCycleCount = cycleCount;
        rlcMacStats_p->highTimeStamp.tv_sec = start->tv_sec; 
        rlcMacStats_p->highTimeStamp.tv_nsec = start->tv_nsec;
        rlcMacStats_p->highTick = globalTTITickCount_g;
    }
    else
    {
        if(rlcMacStats_p->highestCycleCount < cycleCount)
        {
            rlcMacStats_p->highestTime = clkTime;
            rlcMacStats_p->highestCycleCount = cycleCount;
            rlcMacStats_p->highTimeStamp.tv_sec = start->tv_sec; 
            rlcMacStats_p->highTimeStamp.tv_nsec = start->tv_nsec;
            rlcMacStats_p->highTick = globalTTITickCount_g;
        }
    }
}

/****************************************************************************
 * Function Name  : updateAndResetThrputStats
 * Inputs         : globalTick - Global TTI,
 *                  numSec - Number of secs
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to calculate UL and DL throughput of last x 
                    seconds and then reset throughput variables.
 ****************************************************************************/
/* SPR 15909 fix start */
void updateAndResetThrputStats(tickType_t globalTick, UInt32 numSec)
/* SPR 15909 fix end */
{
    UInt32 ueId = 0;
    UInt16 lcId = 0;
    
    LteRlcLCStats* lcStats_p =
            &(gRlcStats.pRlcStats->lteRlcUeStat[ueId].lcStats[lcId]);
    LteRlcUEInfo* ueStats_p = &(gRlcStats.pRlcStats->lteRlcUeStat[ueId]);

    LteRlcSysStatInfo* sysStats_p = &(gRlcStats.pRlcStats->lteSysStat);

   /*Coverity 94776 Fix Start*/ 
    if((globalTick - sysStats_p->lastTick) >= (numSec*((tickType_t)1000)))
    {
   /*Coverity 94776 Fix End*/ 
       for (ueId = 0; ueId < MAX_UE_SUPPORTED ; ueId ++)
       {
          for (lcId = 1; lcId < MAX_LCS; lcId ++)
          {
              lcStats_p->lastDlThroughput = lcStats_p->dlThroughput/(numSec);
              lcStats_p->lastUlThroughput = lcStats_p->ulThroughput/(numSec);
          }
          ueStats_p->lastDlThrput = ueStats_p->dlThrput/(numSec);
          ueStats_p->lastUlThrput = ueStats_p->ulThrput/(numSec);
       }
       sysStats_p->lastSysDlThroughput = sysStats_p->sysDlThroughput/(numSec);
       sysStats_p->lastSysUlThroughput = sysStats_p->sysUlThroughput/(numSec);
    }
    sysStats_p->lastTick = globalTick;
}
   
