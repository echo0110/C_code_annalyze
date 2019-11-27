/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright  (c) Aricent.
 *
 ******************************************************************************
 *
 *  $Id: ltePerfStats.c
 *
 ******************************************************************************
 *
 * File Description : This file contains the functionality that is invoked
 *                    from execution scheduler to obtain performance
 *                    stats in the file.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * Revision  1.10       2013/1/22  16:15:30   (gur22014) 
 * Initial version
 *
 ******************************************************************************/

#if  defined(PERF_STATS) && !defined(UE_SIM_TESTING)

/******************************************************************************
 * Project Includes
 ******************************************************************************/
#include <stddef.h>
#include "ltePerfStats.h"
#include "lteMisc.h"

/******************************************************************************
 * Private Definitions
 ******************************************************************************/
#define MAX_PERF_STATS 2

/*SPR 21545 Start*/
extern UInt8 instanceL2_g;
/*SPR 21545 End*/
LtePerfStatInfo gPerfStats;
LtePerfStatsControlInfo *gPerfStatsControlInfo_p;
/* + PERF_CA */
LteCellPerfStats *gMacCellPerfStats_p[MAX_NUM_CELL] ;
LteUePerfStats *gMacUePerfStats_p ;
LteCellPerfStats *gPdcpCellPerfStats_p[MAX_NUM_CELL] ;
LteUePerfStats *gPdcpUePerfStats_p;
LteCellPerfStats *gRlcCellPerfStats_p[MAX_NUM_CELL] ;
/* - PERF_CA */
LteUePerfStats *gRlcUePerfStats_p ;
PerfStatsScanner g_perfStatsScannerObj;

UInt32 overlapRBsMibSS_g[MAX_NUM_CELL] = {0};
/* + coverity 52755 */
/** CA-TDD Changes Start **/
PerfStatsMibSyncSigType perfStatsMibSyncSignalFlag_g[MAX_NUM_CELL] = {PERF_STATS_NO_OVERLAP};
/** CA-TDD Changes Start **/
/* - coverity 52755 */
UInt32 rePerRBPair_g[MAX_CP][MAX_CFI] =
{
    { 156, 144, 132 },
    { 132, 120, 108 }
};
PdcpLastTtiVolDl pdcpLastTtiTransVolDl_g [PERF_MAX_UE_SUPPORTED][MAX_LC_IDENTITY];
/******************************************************************************
* Function Name  : initPerfStats 
* Inputs         : none
* Outputs        : none
* Returns        : none
* Description    : It will initialize the shared memory for the PERF stats  
*                 
*******************************************************************************/
/* + SPR 17439 */
UInt32 initPerfStats(void)
/* - SPR 17439 */    
{
	/* + PERF_CA */
	InternalCellIndex cellIndex = 0;
	/* - PERF_CA */
    UInt32 index = 0;
    UInt32 lcId = 0;
    /* Get the shared memory ID */
    /* SPR 11826 Fix start */
    SInt32 shmId = 0;
    shmId = shmget_wrapper(PERF_STATS_SHM_KEY+ instanceL2_g ,
			(MAX_PERF_STATS * sizeof(LtePerfStats) + sizeof(LtePerfStatsControlInfo)) ,
			PERF_STATS_SHM_FLAGS);
    if(shmId >= 0)
    {
        /* IF key exist then remove SHM*/
        shmctl_wrapper_ipc_rmid(shmId);

    }

    /* Get the shared memory ID */
    gPerfStats.shmId = shmget_wrapper(PERF_STATS_SHM_KEY+ instanceL2_g ,
            (MAX_PERF_STATS * sizeof(LtePerfStats) + sizeof(LtePerfStatsControlInfo)) ,
            PERF_STATS_SHM_FLAGS);

    /* SPR 11826 Fix End */
    if (0 > gPerfStats.shmId)
    {
        perror("SHM get failed");
        return PERF_STATS_FAILURE;
    }

    /* Map Shared memory */
    gPerfStats.perfStatsControlInfo = shmat(gPerfStats.shmId, NULL, 0);
    gPerfStatsControlInfo_p = gPerfStats.perfStatsControlInfo;
   
    gPerfStats.perfStats = (LtePerfStats*)(gPerfStatsControlInfo_p + 1);

    if ((void *)-1 == gPerfStatsControlInfo_p)
    {
        perror("SHM attach failed");
        return PERF_STATS_FAILURE;
    }
    if ((void *)-1 == gPerfStats.perfStats)
    {
        perror("SHM attach failed");
        return PERF_STATS_FAILURE;
    }
    
	/* + PERF_CA */
	/* loop runs upto MAX_NUM_CELL beacuse MAC_INIT_LAYER_REQ API not received when initPerfStats func is called */
	for (cellIndex = 0; cellIndex < MAX_NUM_CELL; cellIndex++)
	{
    	gMacCellPerfStats_p[cellIndex] = &gPerfStats.perfStats[0].lteCellPerfStats[cellIndex];
    	gPdcpCellPerfStats_p[cellIndex] = &gPerfStats.perfStats[0].lteCellPerfStats[cellIndex];
    	gRlcCellPerfStats_p[cellIndex] = &gPerfStats.perfStats[0].lteCellPerfStats[cellIndex];
    }
	gMacUePerfStats_p = gPerfStats.perfStats[0].lteUePerfStats;
    gPdcpUePerfStats_p = gPerfStats.perfStats[0].lteUePerfStats;
	/* - PERF_CA */
    gRlcUePerfStats_p = gPerfStats.perfStats[0].lteUePerfStats;
    gPerfStatsControlInfo_p = gPerfStats.perfStatsControlInfo;
    /* Initialize shared memory */
    memSet(gPerfStatsControlInfo_p, 0,  sizeof(LtePerfStatsControlInfo));
    memSet(gPerfStats.perfStats, 0, MAX_PERF_STATS * sizeof(LtePerfStats));
    memSet(&g_perfStatsScannerObj, 0 , sizeof(PerfStatsScanner));
   
    /* Initialize the structure */
    for (index = 0;index < PERF_MAX_UE_SUPPORTED; index++)
    {
        for (lcId =0;lcId < MAX_LC_IDENTITY;lcId++)
        {
            pdcpLastTtiTransVolDl_g[index][lcId].currentIndex = 0;
            pdcpLastTtiTransVolDl_g[index][lcId].previousIndex = 1;
        }
    }

    return PERF_STATS_SUCCESS;
}
UInt32 csrREsExcludingFirstSymbol_g[MAX_NUM_CELL] = {0};
/* + PERF_CA GBL */
UInt32 reUsedForMibPerPRB_g[MAX_NUM_CELL] = {0};
UInt32 reUsedForPssSssPerPRB_g[MAX_NUM_CELL] = {0};
/* + PERF_CA GBL */
UInt32 central6RBStartIndex_g[MAX_NUM_CELL] = {0};
UInt32 perfStatsULDelTrigger_g[PERF_MAX_UE_SUPPORTED]={0};
#endif


