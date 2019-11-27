/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcStat.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:39 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRlcStat.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:39  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:14:04  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.11.2.2  2010/07/17 11:35:31  gur23054
 * Review Comments fixed
 *
 * Revision 1.11.2.1  2010/06/17 10:59:42  gur19836
 * RLC AM Base Code Merged
 *
 * Revision 1.11  2010/06/14 11:57:59  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.10  2009/04/07 16:04:41  gur20548
 * update in rrc and oam
 *
 * Revision 1.9  2009/04/07 15:02:16  gur19140
 * peer to peer fixes
 *
 * Revision 1.8  2009/04/02 14:27:51  gur19140
 * changes for intg and peer to peer testing
 *
 * Revision 1.7  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.6  2009/03/02 05:32:50  gur19836
 * *** empty log message ***
 *
 * Revision 1.5  2009/02/19 08:36:33  gur19140
 * tx fixes
 *
 * Revision 1.4  2009/02/18 14:36:40  gur20294
 * added new function printStatPerUe
 *
 * Revision 1.3  2009/02/05 10:59:40  gur19836
 * CR comments fixed
 *
 * Revision 1.2  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 *
 ****************************************************************************/

#ifndef RLCSTAT_H
#define RLCSTAT_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcTypes.h"
#include "lteRlcOamInterface.h"
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MAX_NUM_DRB 9


extern UInt32 rxOutOfOrderPdu_g ;
extern UInt32 activeUEs_g  ;
void rlcInitStatFactory (void);
void rlcResetStatFactory (void);
/* + PERF_CA GBL */
void rlcResetStat(InternalCellIndex internalCellIndex);
/* + PERF_CA GBL */

/**************************************************************************
 * RLC SYSTEM STATS data structures
***************************************************************************/

typedef enum RlcPdcpApiIdentifierT{
RLC_UE_DATA_REQ=0,
RLC_UE_DATA_IND,
RLC_DLVRY_STATUS_IND,
RLC_DATA_DISCARD_REQ,
MAX_RLC_PDCP_API_ID
}RlcPdcpApiIdentifier;

typedef enum RlcMacApiIdentifierT{
MAC_UE_OPPOR_IND=0,
MAC_UE_DATA_IND,
MAC_COMCH_OPPOR_IND,
MAC_COMCH_DATA_IND,
MAC_UE_HARQ_FAILURE_IND,
MAX_RLC_MAC_API_ID
}RlcMAcApiIdentifier;

typedef struct LteRlcLCStatsT{
    UInt32 sduNumDl;
    UInt32 sduBytesDl;
    UInt32 pduNumDl;
    UInt32 pduBytesDl;
    UInt32 sduNumUl;
    UInt32 sduBytesUl;
    UInt32 pduNumUl;
    UInt32 pduBytesUl;
    UInt32 numRetx;
    UInt32 numDlvryInd;
    UInt32 numPktsDropped;
    UInt32 numPktsDroppedUl;
    UInt32 numPktsDiscarded;
    UInt32 lastDlThroughput;
    UInt32 lastUlThroughput;
    UInt32 dlThroughput;
    UInt32 ulThroughput;
}LteRlcLCStats;

typedef struct LteRlcUEInfoT{
UInt32 lastDlThrput;
UInt32 lastUlThrput;
UInt32 dlThrput;
UInt32 ulThrput;
UInt16 numActiveLC;
LteRlcLCStats lcStats[MAXIMUM_LC_ID];
}LteRlcUEInfo;


typedef struct lteRlcUEStatInfoT {
UInt16 numOfActiveUE;
lteRlcUEStats rlcUeStats[MAX_UE_SUPPORTED];
}lteRlcUEStatInfo;

typedef struct LteRlcSysStatInfo{
UInt32 sysDlThroughput;
UInt32 sysUlThroughput;
UInt32 lastSysDlThroughput;
UInt32 lastSysUlThroughput;
/* SPR 15909 fix start */
tickType_t lastTick;
/* SPR 15909 fix end */
UInt32 numActiveUE;
}LteRlcSysStatInfo;

typedef struct LteRlcApiStatsT{
    /* SPR 15909 fix start */
    tickType_t totalTime;
    UInt32 totalCycleCount;
    UInt32 numOfCalls;
    tickType_t highestTime;
    UInt32 highestCycleCount;
    tickType_t highTick;
    /* SPR 15909 fix end */
    TimeSpec highTimeStamp;
}LteRlcApiStats;

typedef struct LteRlcApiStatInfoT{
LteRlcApiStats rlcPdcpApiStats[MAX_RLC_PDCP_API_ID];
LteRlcApiStats rlcMacApiStats [MAX_RLC_MAC_API_ID];
}LteRlcApiStatInfo;

/** RLC GLOBAL data structure to Update System Stats **/
typedef struct lteRlcStatInfoT {
lteRlcTMStatInfo  lteTMStat;
lteRlcUMStatInfo  lteUMStat;
lteRlcAMStatInfo  lteAMStat;
lteRlcUEStatInfo  lteUEStat;
}lteRlcStatInfo;

typedef struct LteRlcStatsT{
LteRlcSysStatInfo lteSysStat;
LteRlcApiStatInfo lteRlcApiStat;
LteRlcUEInfo      lteRlcUeStat[MAX_UE_SUPPORTED];
}LteRlcStats;

typedef struct LteRlcShmStatsInfoT{
    SInt32      shmId;
    LteRlcStats *pRlcStats;
}LteRlcShmStatsInfo;

#define THRPUT_DURATION 10
#define RLC_STATS_SHM_FLAGS (IPC_CREAT | 0666)
#define RLC_STATS_SHM_KEY   32100

#ifdef CYCLE_COUNT_MAC_RLC_PDCP
extern __thread TimeSpec rlc_start_clock_g , rlc_end_clock_g;
extern __thread UInt32 rlc_start_count_g, rlc_end_count_g;
extern __thread UInt32 rlc_cycle_count_used_g;
#endif

#ifndef CYCLE_COUNT_MAC_RLC_PDCP

#define CLOCK_START_RLC_API(apiId)   
#define CLOCK_END_RLC_PDCP_API(apiId)  

#define CLOCK_END_RLC_MAC_API(apiId) 
#else

#define CLOCK_START_RLC_API(apiId) \
do{\
    rlc_start_count_g = rdtsc();\
    clock_gettime_real_time(&rlc_start_clock_g);\
}while (0);

#define CLOCK_END_RLC_PDCP_API(apiId)\
      do {\
           clock_gettime_real_time(&rlc_end_clock_g);\
           rlc_end_count_g = rdtsc(); \
           if(rlc_start_count_g > rlc_end_count_g)\
           rlc_cycle_count_used_g = UINT_MAX - rlc_start_count_g + rlc_end_count_g;\
           else \
           rlc_cycle_count_used_g = rlc_end_count_g - rlc_start_count_g;\
           updateRlcPdcpApiStats((apiId),(rlc_cycle_count_used_g),\
                        &(rlc_start_clock_g), &(rlc_end_clock_g)); \
                }while (0);


#define CLOCK_END_RLC_MAC_API(apiId)\
      do {\
           clock_gettime_real_time(&rlc_end_clock_g);\
           rlc_end_count_g = rdtsc(); \
           if(rlc_start_count_g > rlc_end_count_g)\
           rlc_cycle_count_used_g = UINT_MAX - rlc_start_count_g + rlc_end_count_g;\
           else \
           rlc_cycle_count_used_g = rlc_end_count_g - rlc_start_count_g;\
           updateRlcMacApiStats((apiId),(rlc_cycle_count_used_g),\
                        &(rlc_start_clock_g), &(rlc_end_clock_g)); \
                }while (0);
#endif

void updateRlcMacApiStats(UInt32 rlcMacApiId,UInt32 cycleCount,
                             TimeSpec *start, TimeSpec *end);
void updateRlcPdcpApiStats(UInt32 rlcPdcpApiId,UInt32 cycleCount,
                             TimeSpec *start, TimeSpec *end);
                             
/* + SPR 17439 */                             
UInt32 initRlcStats(void);
/* - SPR 17439 */
/* SPR 15909 fix start */
void updateAndResetThrputStats(tickType_t globalTick, UInt32 numSec);
/* SPR 15909 fix end */

/**************************************************************************
 *  * Global Declartion
 *   * ***********************************************************************/
/* + PERF_CA GBL */
extern lteRlcStatInfo lteRlcStatInfo_g[MAX_NUM_CELL];
/* - PERF_CA GBL */
extern LteRlcShmStatsInfo gRlcStats;
/* SPR 15909 fix start */
void updateAndResetThrputStats(tickType_t globalTick, UInt32 numSec);
/* SPR 15909 fix end */
/* + SPR 17439 */
UInt32 initRlcStats(void);
/* - SPR 17439 */
#endif  /* RLCSTAT_H */
