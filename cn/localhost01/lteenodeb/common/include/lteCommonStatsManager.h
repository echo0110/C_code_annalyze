/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteMacStatsManager.h
 *
 ****************************************************************************
 *
 *  File Description : contains the function prototypes and data structures
 *  needed for stats manager to dump the stats info into the stats file.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/

#ifndef INCLUDED_COMMON_STATS_MANAGER_H
#define INCLUDED_COMMON_STATS_MANAGER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include "lteTypes.h"
#include "lteSQueue.h"
#include "lteLayer2CommanTypes.h"
#include "lteLayer2MacRlcInterface.h"
/*spr 22474 start*/
    /*CSPL Code removed*/
/*spr 22474 end*/
#include "lteCircQueue.h"
#include "lteCommonQueueDefs.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
****************************************************************************/
#define MAX_STATS_TICKS 100
/* SPR 21231 Fix Start */
#ifdef SSI_DEBUG_STATS
#if (MAX_UE_SUPPORTED > 500)
#define MAX_TTI_STATS 500
#else
#define MAX_TTI_STATS 1000
#endif
#else
#define MAX_TTI_STATS 1000
#endif
/* SPR 21231 Fix End */
#define MAX_RESOURCE_BLOCK 110
#define MAX_STRATEGY_LC_SCHEDULED 140
#define MAX_STRATEGY_LCG_SCHEDULED 100
#ifdef SSI_DEBUG_STATS
#define MAX_LC_CNT   4
#endif
#define MAX_LCG_COUNT  2
#define MAC_UL 0
#define MAC_DL 1
#define BUSY   1
#define INVALID_QCI 10
#define OAM_STAT_MGR "OAM_STAT_MGR"
#ifdef UTFWK_SIMULATION
#define MAX_UE_UTFWK 32
#define MAX_UTFWK_TICK MAX_TTI_STATS
#endif
#define INVALID_LC_ID    0xFF
#define MIN_LC_VALUE     1
/* Number of SPS periodicity supported */
#define MAX_DL_SPS_INTERVAL         40
/* + SPS_TDD_Changes */
/* +- SPR 18490 */
#ifdef FDD_CONFIG
#define MAX_UL_SPS_INTERVAL         40
#else
#define MAX_UL_SPS_INTERVAL         80
#endif
/* - SPS_TDD_Changes */
#define MAX_SPS_INTERVALS           16 
#define TOTAL_SPS_INTERVAL          10 /* warning fix */
/* +- SPR 18490 */
#define MAX_N1_PUCCH_AN_PERSISTENT  4
#define MAX_ROHC_PROFILE_SUPP       4
#define MAX_LCID 10
/* KPI changes start */
#define MAX_CQI_INDEX 16
/* KPI changes end */
/* SPR 23402 Changes Start */
/* MOCN Changes Start */
#define MAX_NUM_MOCN_OPERATOR 6
#define INVALID_OPERATOR_ID 0xFF
#define MOCN_PRIORITY_FACTOR_0     0
#define MOCN_PRIORITY_FACTOR_600   600
#define MOCN_PRIORITY_FACTOR_1200 1200
#define MOCN_SRB_PRIORITY_FACTOR 1200
/* MOCN Changes End */
/* SPR 23402 Changes End */
/* Following macro will return return 1 when bit for the given place is set else return 0 */
#define IS_KPI_PDCP_BITSET(bitmap,bitplace)    (bitmap & (1<<bitplace)) 
/* total numebr of batches for reporting throughput KPI */
#define NUMBER_OF_BATCHES        (MAX_UE_SUPPORTED/MAX_UE_PER_BATCH)
/* following number of TTI would be used as time when 
 * stats would be sent for one batch */
#define AVG_DURATION_TTI         1
/* invalid entry for throughput KPI */
#define INVALID_THP_ENTRY        0xFFFFFFFF

/* SPR 11749 fix */
#define INVALID_THP_STATS_ENTRY        0xFFFFFFFFFFFFFFFFULL
/* SPR 11749 fix */
/* TTI when THP stats would be reset */
#define THP_RESET_TTI            10000
/* min duration for THP KPI */
#define MIN_THP_CALC_TIME        5000
/* 100MS_SUPPORT : start */
#define WAIT_FOR_1SEC            0xFF
/* 100MS_SUPPORT : end */


/* MAC KPI Number of Active User Sampling Timer, 100 TTI */
#define MAC_KPI_ACTIVE_USER_SAMPLING_TIMER_TTI  100
/* SPR 13492 Changes Start */
#ifdef KPI_STATS
#define SAMPLING_TIME_FOR_KPI_IN_SESSION 100
#endif
/* SPR 13492 Changes End */

 /*KPI Container Changes*/
#define KPI_CONTAINER_OAM_MAC          0
#define KPI_CONTAINER_OAM_PDCP         1
#define KPI_CONTAINER_RRM_MAC          2
#define KPI_CONTAINER_RRM_PDCP         3
#define KPI_CONTAINER_SON_MAC          4
#define KPI_CONTAINER_SON_PDCP         5
/*  100MS_SUPPORT : start */
#define KPI_CONTAINER_THP_OAM_MAC      6  
#define KPI_CONTAINER_THP_OAM_PDCP     7  
#define KPI_CONTAINER_THP_RRM_MAC      8  
#define KPI_CONTAINER_THP_RRM_PDCP     9  
#define KPI_CONTAINER_THP_SON_MAC      10 
#define KPI_CONTAINER_THP_SON_PDCP     11 
/* Fix for Coverity Warning for 100ms KPI Starts */
#define MAX_KPI_CONTAINER              12
/* Fix for Coverity Warning for 100ms KPI Ends */
/*  100MS_SUPPORT : end */

#define OAM_MAC_CONTAINER_BIT_MASK     (0x01 << KPI_CONTAINER_OAM_MAC)
#define OAM_PDCP_CONTAINER_BIT_MASK    (0x01 << KPI_CONTAINER_OAM_PDCP)
#define RRM_MAC_CONTAINER_BIT_MASK     (0x01 << KPI_CONTAINER_RRM_MAC)
#define RRM_PDCP_CONTAINER_BIT_MASK    (0x01 << KPI_CONTAINER_RRM_PDCP)
#define SON_MAC_CONTAINER_BIT_MASK     (0x01 << KPI_CONTAINER_SON_MAC)
#define SON_PDCP_CONTAINER_BIT_MASK    (0x01 << KPI_CONTAINER_SON_PDCP)
/*  100MS_SUPPORT : start */
#define CELL_SPECIFIC_KPI             (OAM_MAC_CONTAINER_BIT_MASK   | \
                                       OAM_PDCP_CONTAINER_BIT_MASK  | \
                                       RRM_MAC_CONTAINER_BIT_MASK   | \
                                       RRM_PDCP_CONTAINER_BIT_MASK  | \
                                       SON_MAC_CONTAINER_BIT_MASK   | \
                                       SON_PDCP_CONTAINER_BIT_MASK)

#define OAM_MAC_THP_CONTAINER_BIT_MASK   (0x01 << KPI_CONTAINER_THP_OAM_MAC) 
#define OAM_PDCP_THP_CONTAINER_BIT_MASK  (0x01 << KPI_CONTAINER_THP_OAM_PDCP) 
#define RRM_MAC_THP_CONTAINER_BIT_MASK   (0x01 << KPI_CONTAINER_THP_RRM_MAC) 
#define RRM_PDCP_THP_CONTAINER_BIT_MASK  (0x01 << KPI_CONTAINER_THP_RRM_PDCP) 
#define SON_MAC_THP_CONTAINER_BIT_MASK   (0x01 << KPI_CONTAINER_THP_SON_MAC) 
#define SON_PDCP_THP_CONTAINER_BIT_MASK  (0x01 << KPI_CONTAINER_THP_SON_PDCP)
 
#define MAC_THP_CONTAINERS               (OAM_MAC_THP_CONTAINER_BIT_MASK | \
                                          RRM_MAC_THP_CONTAINER_BIT_MASK | \
                                          SON_MAC_THP_CONTAINER_BIT_MASK)

#define PDCP_THP_CONTAINERS               (OAM_PDCP_THP_CONTAINER_BIT_MASK | \
                                          RRM_PDCP_THP_CONTAINER_BIT_MASK | \
                                          SON_PDCP_THP_CONTAINER_BIT_MASK)
/*  100MS_SUPPORT : end */

/* Macros for storing data in per tti trx stat info.
 * comments show the bit used for data */
#define LTE_MAC_STATS_RF_TX_DL_CONFIG                   0x00000001
#define LTE_MAC_STATS_RF_TX_DL_CONFIG_DCI               0x00000002
#define LTE_MAC_STATS_RF_TX_DL_CONFIG_BCH               0x00000004
#define LTE_MAC_STATS_RF_TX_DL_CONFIG_MCH               0x00000008
#define LTE_MAC_STATS_RF_TX_DL_CONFIG_DLSCH             0x00000010
#define LTE_MAC_STATS_RF_TX_DL_CONFIG_PCH               0x00000020
#define LTE_MAC_STATS_RF_TX_UL_CONFIG                   0x00000040
#define LTE_MAC_STATS_RF_TX_UL_CONFIG_ULSCH             0x00000080
#define LTE_MAC_STATS_RF_TX_UL_CONFIG_ULSCH_CQI_RI      0x00000100
#define LTE_MAC_STATS_RF_TX_UL_CONFIG_ULSCH_HARQ        0x00000200
#define LTE_MAC_STATS_RF_TX_UL_CONFIG_ULSCH_CQI_HARQ_RI 0x00000400
#define LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_CQI           0x00000800
#define LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_SR            0x00001000
#define LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_HARQ          0x00002000
#define LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_SR_HARQ       0x00004000
#define LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_CQI_HARQ      0x00008000
#define LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_CQI_SR        0x00010000
#define LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_CQI_SR_HARQ   0x00020000
#define LTE_MAC_STATS_RF_TX_UL_CONFIG_SRS               0x00040000
#define LTE_MAC_STATS_RF_TX_TX_REQ                      0x00080000
#define LTE_MAC_STATS_RF_TX_TX_REQ_BCH                  0x00100000
#define LTE_MAC_STATS_RF_TX_TX_REQ_MCH                  0x00200000
#define LTE_MAC_STATS_RF_TX_TX_REQ_DLSCH                0x00400000
#define LTE_MAC_STATS_RF_TX_TX_REQ_PCH                  0x00800000
#define LTE_MAC_STATS_RF_TX_PARAM_REQ                   0x01000000
#define LTE_MAC_STATS_RF_TX_CELL_CONFIG_REQ             0x02000000
#define LTE_MAC_STATS_RF_TX_START_REQ                   0x04000000
#define LTE_MAC_STATS_RF_TX_STOP_REQ                    0x08000000
#define LTE_MAC_STATS_RF_TX_UE_CONFIG_REQ               0x10000000
#define LTE_MAC_STATS_RF_TX_HI_DCI0_REQ                 0x20000000
#define LTE_MAC_STATS_RF_TX_HI_DCI0_HI                  0x40000000
#define LTE_MAC_STATS_RF_TX_HI_DCI0_DCI0                0x80000000

#define LTE_MAC_STATS_RF_RX_PARAM_RESP                  0x00000001
#define LTE_MAC_STATS_RF_RX_CELL_CONFIG_RESP            0x00000002
#define LTE_MAC_STATS_RF_RX_STOP_IND                    0x00000004
#define LTE_MAC_STATS_RF_RX_UE_CONFIG_RESP              0x00000008
#define LTE_MAC_STATS_RF_RX_ERROR_IND                   0x00000010
#define LTE_MAC_STATS_RF_RX_SF_IND                      0x00000020
#define LTE_MAC_STATS_RF_RX_HARQ_IND                    0x00000040
#define LTE_MAC_STATS_RF_RX_CRC_IND                     0x00000080
#define LTE_MAC_STATS_RF_RX_RX_ULSCH_IND                0x00000100
#define LTE_MAC_STATS_RF_RX_RACH_IND                    0x00000200
#define LTE_MAC_STATS_RF_RX_SRS_IND                     0x00000400
#define LTE_MAC_STATS_RF_RX_RX_SR_IND                   0x00000800
#define LTE_MAC_STATS_RF_RX_RX_CQI_IND                  0x00001000
#if defined (KPI_STATS) || defined (PERF_STATS)
#define SCALE_UP_MULTIPLYING_FACTOR                     7
#endif

/* Maximum number of subframes for which per tti stats are stored */
#define LTE_MAC_STATS_MAX_SFN       1024
/* Macros for mapping stats shared memory */
#ifdef TARGET_ENB
#define STATS_SHM_KEY   34433
#else
#define STATS_SHM_KEY   34432
#endif
#define STATS_SHM_FLAGS (IPC_CREAT | 0666)

#define SFI_PROFILE_COUNT   1000
#define PERF_STATS_TIME     1000

#ifdef DL_DATA_SEPARATION
#define ENCODER_PROFILE_COUNT       1000 // i.e. Number of iteration for encoder thread
#define ENCODER_PERF_STATS_TIME     1000
#endif

/* Macros for capturing stats */
#ifdef CYCLE_COUNT_MAC_RLC_PDCP
#define NUM_L2_FUNC_CYCLES_LIMIT 100000
#define NUM_L2_CC_ITERATIONS  50
#endif

#define DL_ACK 1
#define DL_NACK 2
#ifdef ACK_NACK_COUNT_STATS
#define NON_MIMO 0
#define MIMO_TB_ONE 1
#define MIMO_TB_TWO 2
extern UInt8 Mimo_NonMimo_Flag_g;
#endif

#ifdef OAM_IPR
typedef struct timespec TimeSpec;
#endif

#ifdef CYCLE_COUNT_MAC_RLC_PDCP
extern __thread TimeSpec prof_start_clock_g , prof_end_clock_g;
extern __thread UInt64 prof_start_count_g, prof_end_count_g;
#if (defined (DL_UL_SPLIT) || defined (DL_UL_SPLIT_TDD))
extern __thread UInt64 prof_start_count_split_g[], prof_end_count_split_g[];
extern __thread UInt64 prof_cycle_count_used_split_g[];
extern __thread TimeSpec prof_start_clock_split_g[],prof_end_clock_split_g[];
#endif
/* SPR 10852 fix start*/
#ifdef DL_DATA_SEPARATION
extern __thread TimeSpec prof_start_clock_DDS_g , prof_end_clock_DDS_g;
extern __thread UInt64 prof_start_count_DDS_g, prof_end_count_DDS_g;
extern __thread UInt64 prof_cycle_count_used_DDS_g;
#endif
/* SPR 10852 fix end*/
extern __thread UInt64 prof_cycle_count_used_g;
extern __thread TimeSpec socket_start_clock_g, socket_end_clock_g;
extern __thread UInt64 socket_start_count_g, socket_end_count_g;
extern __thread UInt64 socket_cycle_count_used_g;
extern __thread UInt32 ExeLegId;
/* SPR 15640 fix start */
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
#define RRC_OAM_FLOW_ID (NUM_OF_THREADS - 1)
#endif
/* SPR 15640 fix end */
#ifdef TDD_CONFIG
#ifdef DL_UL_SPLIT_TDD
#define MAX_ULRLC_CC_FUNC 5
#ifdef GETSTATS_EL_CLOCKTIME
#define RDTSC_AND_CLOCK_START_SOCKET \
    do{\
        socket_start_count_g = rdtsc();\
        clock_gettime(CLOCK_REALTIME,&socket_start_clock_g);\
    }while (0);


#define RDTSC_AND_CLOCK_END_SOCKET\
  do {\
 clock_gettime(CLOCK_REALTIME,&socket_end_clock_g);\
 socket_end_count_g = rdtsc(); \
 if(socket_start_count_g > socket_end_count_g)\
 socket_cycle_count_used_g = RDTSC_MAX - socket_start_count_g + socket_end_count_g;\
 else \
 socket_cycle_count_used_g = socket_end_count_g - socket_start_count_g;\
 socketUpdateCycleCount_clkTimeStats((ExeLegId),(socket_cycle_count_used_g),\
     &(socket_start_clock_g), &(socket_end_clock_g)); \
      }while (0);


#define RDTSC_AND_CLOCK_START_SPLIT_TDD(threadId, funcId) \
        do{\
            prof_start_count_split_g[threadId] = rdtsc();\
            clock_gettime(CLOCK_REALTIME,&prof_start_clock_split_g[threadId]);\
            ExeLegId = funcId; \
        }while (0);

#define RDTSC_AND_CLOCK_END_SPLIT_TDD(threadId, funcId)\
      do {\
           clock_gettime(CLOCK_REALTIME,&prof_end_clock_split_g[threadId]);\
           prof_end_count_split_g[threadId] = rdtsc(); \
           if(prof_start_count_split_g[threadId] > prof_end_count_split_g[threadId])\
           prof_cycle_count_used_split_g[threadId] = RDTSC_MAX - prof_start_count_split_g[threadId] + prof_end_count_split_g[threadId];\
           else \
           prof_cycle_count_used_split_g[threadId] = prof_end_count_split_g[threadId] - prof_start_count_split_g[threadId];\
           updateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_split_g[threadId]),\
                        &(prof_start_clock_split_g[threadId]), &(prof_end_clock_split_g[threadId])); \
      }while (0);

#define RDTSC_AND_CLOCK_START_TDD(funcId) \
    do{\
        prof_start_count_g = rdtsc();\
        clock_gettime(CLOCK_REALTIME,&prof_start_clock_g);\
        ExeLegId = funcId; \
    }while (0);

#define RDTSC_AND_CLOCK_END_ULRLC(funcId)\
    do{\
 clock_gettime(CLOCK_REALTIME,&prof_end_clock_g);\
 prof_end_count_g = rdtsc(); \
 if(prof_start_count_g > prof_end_count_g)\
 prof_cycle_count_used_g = RDTSC_MAX- prof_start_count_g + prof_end_count_g;\
 else \
 prof_cycle_count_used_g = prof_end_count_g - prof_start_count_g;\
  updateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_g),\
     &(prof_start_clock_g), &(prof_end_clock_g)); \
      }while (0);


#define RDTSC_AND_CLOCK_START_FDD(funcId) \
        do{\
                    prof_start_count_g = rdtsc();\
                    clock_gettime(CLOCK_REALTIME,&prof_start_clock_g);\
                    ExeLegId = funcId; \
                }while (0);

#define RDTSC_AND_CLOCK_END_PDCP(funcId)\
    do{\
 clock_gettime(CLOCK_REALTIME,&prof_end_clock_g);\
 prof_end_count_g = rdtsc(); \
 if(prof_start_count_g > prof_end_count_g)\
 prof_cycle_count_used_g = RDTSC_MAX- prof_start_count_g + prof_end_count_g;\
 else \
 prof_cycle_count_used_g = prof_end_count_g - prof_start_count_g;\
 pdcpUpdateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_g),\
     &(prof_start_clock_g), &(prof_end_clock_g)); \
      }while (0);

#else 

#define RDTSC_AND_CLOCK_START_SOCKET \
    do{\
        socket_start_count_g = rdtsc();\
    }while (0);

#define RDTSC_AND_CLOCK_END_SOCKET\
  do {\
 socket_end_count_g = rdtsc(); \
 if(socket_start_count_g > socket_end_count_g)\
 socket_cycle_count_used_g = RDTSC_MAX - socket_start_count_g + socket_end_count_g;\
 else \
 socket_cycle_count_used_g = socket_end_count_g - socket_start_count_g;\
 socketUpdateCycleCount_clkTimeStats((ExeLegId),(socket_cycle_count_used_g),\
     &(socket_start_clock_g), &(socket_end_clock_g)); \
      }while (0);

#define RDTSC_AND_CLOCK_START_SPLIT_TDD(threadId, funcId) \
    do{\
        prof_start_count_split_g[threadId] = rdtsc();\
        ExeLegId = funcId; \
    }while (0);

#define RDTSC_AND_CLOCK_END_SPLIT_TDD(threadId, funcId)\
  do {\
 prof_end_count_split_g[threadId] = rdtsc(); \
 if(prof_start_count_split_g[threadId] > prof_end_count_split_g[threadId])\
 prof_cycle_count_used_split_g[threadId] = RDTSC_MAX - prof_start_count_split_g[threadId] + prof_end_count_split_g[threadId];\
 else \
 prof_cycle_count_used_split_g[threadId] = prof_end_count_split_g[threadId] - prof_start_count_split_g[threadId];\
 updateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_split_g[threadId]),\
     &(prof_start_clock_split_g[threadId]), &(prof_end_clock_split_g[threadId])); \
      }while (0);

#define RDTSC_AND_CLOCK_START_TDD(funcId) \
    do{\
        prof_start_count_g = rdtsc();\
        ExeLegId = funcId; \
    }while (0);

#define RDTSC_AND_CLOCK_END_ULRLC(funcId)\
    do{\
 prof_end_count_g = rdtsc(); \
 if(prof_start_count_g > prof_end_count_g)\
 prof_cycle_count_used_g = RDTSC_MAX- prof_start_count_g + prof_end_count_g;\
 else \
 prof_cycle_count_used_g = prof_end_count_g - prof_start_count_g;\
 updateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_g),\
     &(prof_start_clock_g), &(prof_end_clock_g)); \
      }while (0);

#define RDTSC_AND_CLOCK_START_FDD(funcId) \
    do{\
        prof_start_count_g = rdtsc();\
        ExeLegId = funcId; \
    }while (0);

#define RDTSC_AND_CLOCK_END_PDCP(funcId)\
    do{\
 prof_end_count_g = rdtsc(); \
 if(prof_start_count_g > prof_end_count_g)\
 prof_cycle_count_used_g = RDTSC_MAX- prof_start_count_g + prof_end_count_g;\
 else \
 prof_cycle_count_used_g = prof_end_count_g - prof_start_count_g;\
 pdcpUpdateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_g),\
     &(prof_start_clock_g), &(prof_end_clock_g)); \
      }while (0);

#endif

#define RDTSC_AND_CLOCK_END_MAC(funcId) if(0){}
#define RDTSC_AND_CLOCK_END_RLC(funcId) if(0){}
#else /* DL_UL_SPLIT_TDD */

#ifdef GETSTATS_EL_CLOCKTIME
#define RDTSC_AND_CLOCK_START_TDD(funcId) \
    do{\
        prof_start_count_g = rdtsc();\
        clock_gettime(CLOCK_REALTIME,&prof_start_clock_g);\
        ExeLegId = funcId; \
    }while (0);

/* FOR SPR 3643 Changes (UINT_MAX to RDTSC_MAX) in below Macro */
/* CA Stats Changes Start */
#define RDTSC_AND_CLOCK_END_LEG(funcId, internalCellIndex)\
  do {\
 clock_gettime(CLOCK_REALTIME,&prof_end_clock_g);\
 prof_end_count_g = rdtsc(); \
 if(prof_start_count_g > prof_end_count_g)\
 prof_cycle_count_used_g = RDTSC_MAX - prof_start_count_g + prof_end_count_g;\
 else \
 prof_cycle_count_used_g = prof_end_count_g - prof_start_count_g;\
 legUpdateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_g),\
     &(prof_start_clock_g), &(prof_end_clock_g), (internalCellIndex)); \
      }while (0);


#else
#define RDTSC_AND_CLOCK_START_TDD(funcId) \
    do{\
        prof_start_count_g = rdtsc();\
        ExeLegId = funcId; \
    }while (0);

/* FOR SPR 3643 Changes (UINT_MAX to RDTSC_MAX) in below Macro */
#define RDTSC_AND_CLOCK_END_LEG(funcId, internalCellIndex)\
  do {\
 prof_end_count_g = rdtsc(); \
 if(prof_start_count_g > prof_end_count_g)\
 prof_cycle_count_used_g = RDTSC_MAX - prof_start_count_g + prof_end_count_g;\
 else \
 prof_cycle_count_used_g = prof_end_count_g - prof_start_count_g;\
 legUpdateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_g),\
     &(prof_start_clock_g), &(prof_end_clock_g), (internalCellIndex)); \
      }while (0);
/* CA Stats Changes End */

#endif
#define RDTSC_AND_CLOCK_START_FDD(funcId) if(0){}
/* CA Stats Changes Start */
#define RDTSC_AND_CLOCK_END_MAC(funcId, internalCellIndex) if(0){} 
#define RDTSC_AND_CLOCK_END_RLC(funcId, internalCellIndex) if(0){}
#define RDTSC_AND_CLOCK_END_PDCP(funcId, internalCellIndex) if(0){}
#define RDTSC_AND_CLOCK_START_SOCKET if(0){}
#define RDTSC_AND_CLOCK_END_SOCKET if(0){}
#endif

#else /* TDD_CONFIG */
/*monza_30_changes_1 end*/

#define RDTSC_AND_CLOCK_START_TDD(funcId) if(0){}
#define RDTSC_AND_CLOCK_END_LEG(funcId, internalCellIndex) if(0){}      
/* CA Stats Changes End */
#define RDTSC_AND_CLOCK_START_SPLIT_TDD(funcId) if(0){}
#define RDTSC_AND_CLOCK_END_SPLIT_TDD(funcId) if(0){}
#define RDTSC_AND_CLOCK_END_ULRLC(funcId) if(0){}


#ifdef DL_UL_SPLIT
#ifdef GETSTATS_EL_CLOCKTIME
#define RDTSC_AND_CLOCK_START_SOCKET \
    do{\
        socket_start_count_g = rdtsc();\
        clock_gettime(CLOCK_REALTIME,&socket_start_clock_g);\
    }while (0);

/* FOR SPR 3643 Changes (UINT_MAX to RDTSC_MAX) in below Macro */
/* CA Stats Changes Start */
#define RDTSC_AND_CLOCK_END_SOCKET(internalCellIndex)\
  do {\
 clock_gettime(CLOCK_REALTIME,&socket_end_clock_g);\
 socket_end_count_g = rdtsc(); \
 if(socket_start_count_g > socket_end_count_g)\
 socket_cycle_count_used_g = RDTSC_MAX - socket_start_count_g + socket_end_count_g;\
 else \
 socket_cycle_count_used_g = socket_end_count_g - socket_start_count_g;\
 socketUpdateCycleCount_clkTimeStats((ExeLegId),(socket_cycle_count_used_g),\
     &(socket_start_clock_g), &(socket_end_clock_g), (internalCellIndex)); \
      }while (0);
/* CA Stats Changes End */

#define RDTSC_AND_CLOCK_START_SPLIT_FDD(threadId, funcId) \
        do{\
                    prof_start_count_split_g[threadId] = rdtsc();\
                    clock_gettime(CLOCK_REALTIME,&prof_start_clock_split_g[threadId]);\
                    ExeLegId = funcId; \
                }while (0);

/* CA Stats Changes Start */
#define RDTSC_AND_CLOCK_END_SPLIT_FDD(threadId, funcId, internalCellIndex)\
      do {\
           clock_gettime(CLOCK_REALTIME,&prof_end_clock_split_g[threadId]);\
           prof_end_count_split_g[threadId] = rdtsc(); \
           if(prof_start_count_split_g[threadId] > prof_end_count_split_g[threadId])\
           prof_cycle_count_used_split_g[threadId] = RDTSC_MAX - prof_start_count_split_g[threadId] + prof_end_count_split_g[threadId];\
           else \
           prof_cycle_count_used_split_g[threadId] = prof_end_count_split_g[threadId] - prof_start_count_split_g[threadId];\
           updateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_split_g[threadId]),\
                        &(prof_start_clock_split_g[threadId]), &(prof_end_clock_split_g[threadId]), (internalCellIndex)); \
                }while (0);
/* CA Stats Changes End */


#define RDTSC_AND_CLOCK_START_FDD(funcId) \
        do{\
                    prof_start_count_g = rdtsc();\
                    clock_gettime(CLOCK_REALTIME,&prof_start_clock_g);\
                    ExeLegId = funcId; \
                }while (0);

/* CA Stats Changes Start */
#define RDTSC_AND_CLOCK_END_PDCP(funcId, internalCellIndex)\
    do{\
 clock_gettime(CLOCK_REALTIME,&prof_end_clock_g);\
 prof_end_count_g = rdtsc(); \
 if(prof_start_count_g > prof_end_count_g)\
 prof_cycle_count_used_g = RDTSC_MAX- prof_start_count_g + prof_end_count_g;\
 else \
 prof_cycle_count_used_g = prof_end_count_g - prof_start_count_g;\
 pdcpUpdateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_g),\
     &(prof_start_clock_g), &(prof_end_clock_g), (internalCellIndex)); \
      }while (0);
/* CA Stats Changes End */
#else
#define RDTSC_AND_CLOCK_START_SOCKET \
    do{\
        socket_start_count_g = rdtsc();\
    }while (0);

/* CA Stats Changes Start */
#define RDTSC_AND_CLOCK_END_SOCKET(internalCellIndex)\
  do {\
 socket_end_count_g = rdtsc(); \
 if(socket_start_count_g > socket_end_count_g)\
 socket_cycle_count_used_g = RDTSC_MAX - socket_start_count_g + socket_end_count_g;\
 else \
 socket_cycle_count_used_g = socket_end_count_g - socket_start_count_g;\
 socketUpdateCycleCount_clkTimeStats((ExeLegId),(socket_cycle_count_used_g),\
     &(socket_start_clock_g), &(socket_end_clock_g), (internalCellIndex)); \
      }while (0);
/* CA Stats Changes End */

#define RDTSC_AND_CLOCK_START_SPLIT_FDD(threadId, funcId) \
    do{\
        prof_start_count_split_g[threadId] = rdtsc();\
        ExeLegId = funcId; \
    }while (0);

/* CA Stats Changes Start */
#define RDTSC_AND_CLOCK_END_SPLIT_FDD(threadId, funcId, internalCellIndex)\
  do {\
 prof_end_count_split_g[threadId] = rdtsc(); \
 if(prof_start_count_split_g[threadId] > prof_end_count_split_g[threadId])\
 prof_cycle_count_used_split_g[threadId] = RDTSC_MAX - prof_start_count_split_g[threadId] + prof_end_count_split_g[threadId];\
 else \
 prof_cycle_count_used_split_g[threadId] = prof_end_count_split_g[threadId] - prof_start_count_split_g[threadId];\
 updateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_split_g[threadId]),\
     &(prof_start_clock_split_g[threadId]), &(prof_end_clock_split_g[threadId]), (internalCellIndex)); \
      }while (0);
/* CA Stats Changes End */


#define RDTSC_AND_CLOCK_START_FDD(funcId) \
    do{\
        prof_start_count_g = rdtsc();\
        ExeLegId = funcId; \
    }while (0);

/* CA Stats Changes Start */
#define RDTSC_AND_CLOCK_END_PDCP(funcId, internalCellIndex)\
    do{\
 prof_end_count_g = rdtsc(); \
 if(prof_start_count_g > prof_end_count_g)\
 prof_cycle_count_used_g = RDTSC_MAX- prof_start_count_g + prof_end_count_g;\
 else \
 prof_cycle_count_used_g = prof_end_count_g - prof_start_count_g;\
 pdcpUpdateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_g),\
     &(prof_start_clock_g), &(prof_end_clock_g), (internalCellIndex)); \
      }while (0);
/* CA Stats Changes End */
#endif
#else
#ifdef GETSTATS_EL_CLOCKTIME
/* SPR 3643 Changes ends */

#define RDTSC_AND_CLOCK_START_SOCKET \
    do{\
        socket_start_count_g = rdtsc();\
        clock_gettime(CLOCK_REALTIME,&socket_start_clock_g);\
    }while (0);

/* FOR SPR 3643 Changes (UINT_MAX to RDTSC_MAX) in below Macro */
/* CA Stats Changes Start */
#define RDTSC_AND_CLOCK_END_SOCKET(internalCellIndex)\
  do {\
 clock_gettime(CLOCK_REALTIME,&socket_end_clock_g);\
 socket_end_count_g = rdtsc(); \
 if(socket_start_count_g > socket_end_count_g)\
 socket_cycle_count_used_g = RDTSC_MAX - socket_start_count_g + socket_end_count_g;\
 else \
 socket_cycle_count_used_g = socket_end_count_g - socket_start_count_g;\
 socketUpdateCycleCount_clkTimeStats((ExeLegId),(socket_cycle_count_used_g),\
     &(socket_start_clock_g), &(socket_end_clock_g), (internalCellIndex)); \
      }while (0);
/* CA Stats Changes End */

#define RDTSC_AND_CLOCK_START_FDD(funcId) \
    do{\
        prof_start_count_g = rdtsc();\
        clock_gettime(CLOCK_REALTIME,&prof_start_clock_g);\
        ExeLegId = funcId; \
    }while (0);

/* FOR SPR 3643 Changes (UINT_MAX to RDTSC_MAX) in below Macro */
/* CA Stats Changes Start */
#define RDTSC_AND_CLOCK_END_MAC(funcId, internalCellIndex)\
  do {\
 clock_gettime(CLOCK_REALTIME,&prof_end_clock_g);\
 prof_end_count_g = rdtsc(); \
 if(prof_start_count_g > prof_end_count_g)\
 prof_cycle_count_used_g = RDTSC_MAX - prof_start_count_g + prof_end_count_g;\
 else \
 prof_cycle_count_used_g = prof_end_count_g - prof_start_count_g;\
 macUpdateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_g),\
     &(prof_start_clock_g), &(prof_end_clock_g), (internalCellIndex)); \
      }while (0);

/* FOR SPR 3643 Changes (UINT_MAX to RDTSC_MAX) in below Macro */
#define RDTSC_AND_CLOCK_END_RLC(funcId, internalCellIndex)\
    do{\
 clock_gettime(CLOCK_REALTIME,&prof_end_clock_g);\
 prof_end_count_g = rdtsc(); \
 if(prof_start_count_g > prof_end_count_g)\
 prof_cycle_count_used_g = RDTSC_MAX - prof_start_count_g + prof_end_count_g;\
 else \
 prof_cycle_count_used_g = prof_end_count_g - prof_start_count_g;\
 rlcUpdateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_g),\
     &(prof_start_clock_g), &(prof_end_clock_g), (internalCellIndex)); \
      }while (0);

/* FOR SPR 3643 Changes (UINT_MAX to RDTSC_MAX) in below Macro */
#define RDTSC_AND_CLOCK_END_PDCP(funcId, internalCellIndex)\
    do{\
 clock_gettime(CLOCK_REALTIME,&prof_end_clock_g);\
 prof_end_count_g = rdtsc(); \
 if(prof_start_count_g > prof_end_count_g)\
 prof_cycle_count_used_g = RDTSC_MAX - prof_start_count_g + prof_end_count_g;\
 else \
 prof_cycle_count_used_g = prof_end_count_g - prof_start_count_g;\
 pdcpUpdateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_g),\
     &(prof_start_clock_g), &(prof_end_clock_g), (internalCellIndex)); \
      }while (0);
/* CA Stats Changes End */

/*SPR 3643 changes starts*/
#else

#define RDTSC_AND_CLOCK_START_SOCKET \
    do{\
        socket_start_count_g = rdtsc();\
    }while (0);

/* FOR SPR 3643 Changes (UINT_MAX to RDTSC_MAX) in below Macro */
/* CA Stats Changes Start */
#define RDTSC_AND_CLOCK_END_SOCKET(internalCellIndex)\
  do {\
 socket_end_count_g = rdtsc(); \
 if(socket_start_count_g > socket_end_count_g)\
 socket_cycle_count_used_g = RDTSC_MAX - socket_start_count_g + socket_end_count_g;\
 else \
 socket_cycle_count_used_g = socket_end_count_g - socket_start_count_g;\
 socketUpdateCycleCount_clkTimeStats((ExeLegId),(socket_cycle_count_used_g),\
     &(socket_start_clock_g), &(socket_end_clock_g), (internalCellIndex)); \
      }while (0);
/* CA Stats Changes End */

#define RDTSC_AND_CLOCK_START_FDD(funcId) \
    do{\
        prof_start_count_g = rdtsc();\
        ExeLegId = funcId; \
    }while (0);

/* CA Stats Changes Start */
#define RDTSC_AND_CLOCK_END_MAC(funcId, internalCellIndex)\
  do {\
 prof_end_count_g = rdtsc(); \
 if(prof_start_count_g > prof_end_count_g)\
 prof_cycle_count_used_g = RDTSC_MAX - prof_start_count_g + prof_end_count_g;\
 else \
 prof_cycle_count_used_g = prof_end_count_g - prof_start_count_g;\
 macUpdateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_g),\
     &(prof_start_clock_g), &(prof_end_clock_g), (internalCellIndex)); \
      }while (0);

#define RDTSC_AND_CLOCK_END_RLC(funcId, internalCellIndex)\
    do{\
 prof_end_count_g = rdtsc(); \
 if(prof_start_count_g > prof_end_count_g)\
 prof_cycle_count_used_g = RDTSC_MAX - prof_start_count_g + prof_end_count_g;\
 else \
 prof_cycle_count_used_g = prof_end_count_g - prof_start_count_g;\
 rlcUpdateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_g),\
     &(prof_start_clock_g), &(prof_end_clock_g), (internalCellIndex)); \
      }while (0);

#define RDTSC_AND_CLOCK_END_PDCP(funcId, internalCellIndex)\
    do{\
 prof_end_count_g = rdtsc(); \
 if(prof_start_count_g > prof_end_count_g)\
 prof_cycle_count_used_g = RDTSC_MAX- prof_start_count_g + prof_end_count_g;\
 else \
 prof_cycle_count_used_g = prof_end_count_g - prof_start_count_g;\
 pdcpUpdateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_g),\
     &(prof_start_clock_g), &(prof_end_clock_g), (internalCellIndex)); \
      }while (0);
/* CA Stats Changes End */

#endif 
#endif /* DL_UL_SPLIT */
#endif /* TDD_CONFIG */

/* SPR 10852 fix start*/
#ifdef DL_DATA_SEPARATION

#ifndef CLOCK_TIME

#define RDTSC_AND_CLOCK_START_ENCODER(funcId) \
    do{\
        prof_start_count_DDS_g = rdtsc();\
    }while (0);

/* CA Stats Changes Start */
#define RDTSC_AND_CLOCK_END_ENCODER(funcId, internalCellIndex)\
    do{\
 prof_end_count_DDS_g = rdtsc(); \
 if(prof_start_count_DDS_g > prof_end_count_DDS_g)\
 prof_cycle_count_used_DDS_g = RDTSC_MAX - prof_start_count_DDS_g + prof_end_count_DDS_g;\
 else \
 prof_cycle_count_used_DDS_g = prof_end_count_DDS_g - prof_start_count_DDS_g;\
 encoderUpdateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_DDS_g),\
     &(prof_start_clock_DDS_g), &(prof_end_clock_DDS_g),(internalCellIndex)); \
      }while (0);
/* CA Stats Changes End */
#else

#define RDTSC_AND_CLOCK_START_ENCODER(funcId) \
    do{\
        prof_start_count_DDS_g = rdtsc();\
        clock_gettime(CLOCK_THREAD_CPUTIME_ID,&prof_start_clock_DDS_g);\
    }while (0);

/* CA Stats Changes Start */
#define RDTSC_AND_CLOCK_END_ENCODER(funcId, internalCellIndex)\
    do{\
 clock_gettime(CLOCK_THREAD_CPUTIME_ID,&prof_end_clock_DDS_g);\
 prof_end_count_DDS_g = rdtsc(); \
 if(prof_start_count_DDS_g > prof_end_count_DDS_g)\
 prof_cycle_count_used_DDS_g = RDTSC_MAX - prof_start_count_DDS_g + prof_end_count_DDS_g;\
 else \
 prof_cycle_count_used_DDS_g = prof_end_count_DDS_g - prof_start_count_DDS_g;\
 encoderUpdateCycleCount_clkTimeStats((funcId),(prof_cycle_count_used_DDS_g),\
     &(prof_start_clock_DDS_g), &(prof_end_clock_DDS_g),(internalCellIndex)); \
      }while (0);
/* CA Stats Changes End */
#endif      
#endif
/*SPR 10852 fix end*/
#else /* CYCLE_COUNT_MAC_RLC_PDCP */
#ifdef DL_UL_SPLIT
#define RDTSC_AND_CLOCK_START_SPLIT_FDD(threadId, funcId) if(0){}
/* CA Stats Changes Start */
#define RDTSC_AND_CLOCK_END_SPLIT_FDD(threadId, funcId, internalCellIndex) if(0){}
/* CA Stats Changes End */
#endif
#define RDTSC_AND_CLOCK_START_FDD(funcId) if(0){}
/* CA Stats Changes Start */
#define RDTSC_AND_CLOCK_END_MAC(funcId, internalCellIndex) if(0){} 
#define RDTSC_AND_CLOCK_END_RLC(funcId, internalCellIndex) if(0){}
#define RDTSC_AND_CLOCK_END_PDCP(funcId, internalCellIndex) if(0){}
#ifdef DL_UL_SPLIT_TDD
#define RDTSC_AND_CLOCK_START_SPLIT_TDD(threadId, funcId) if(0){}
#define RDTSC_AND_CLOCK_END_SPLIT_TDD(threadId, funcId) if(0){}
#define RDTSC_AND_CLOCK_END_ULRLC(funcId) if(0){}
#endif
#define RDTSC_AND_CLOCK_START_TDD(functId) if(0){}
#define RDTSC_AND_CLOCK_END_LEG(funcId, internalCellIndex) if(0){} 
#define RDTSC_AND_CLOCK_END_SOCKET(internalCellIndex) if(0) {}
/* CA Stats Changes End */
#define RDTSC_AND_CLOCK_START_SOCKET if(0) {}

#ifdef DL_DATA_SEPARATION
#define RDTSC_AND_CLOCK_START_ENCODER(funcId) if(0){}
/* CA Stats Changes Start */
#define RDTSC_AND_CLOCK_END_ENCODER(funcId, internalCellIndex) if(0){}
/* CA Stats Changes End */
#endif

#endif

#ifdef SSI_DEBUG_STATS
/* CA Stats Changes Start */
#define LTE_MAC_UPDATE_STATS_FOR_PDSCH( ueContext_p,                           \
                                        dlStrategyTxNode_p,                    \
                                        harqProcess_p,                         \
                                        dciCCEInfo_p,                          \
                                        currentTick,excessPoolRb,                                   \
                                        internalCellIndex)                                          \
    UInt16 writeTTIIndex = 0;                                                  \
    LteMacPdschPerTTIPerUEStats *pLteMacPdschPerTTIPerUEStats = PNULL;         \
    UInt8  numScheduledUETotal;                                                \
    UInt16 pdschCurWriteIndex;                                                 \
    UEScheduledTTIInfo *pLtePdschUeSchdInfo = PNULL;                           \
    UInt16 ueIndex;                                                            \
    ueIndex = ueContext_p->ueIndex;                                            \
                                                                               \
    writeTTIIndex = gMacStats.pStats->writeTTIIndex[internalCellIndex];                             \
    gMacStats.pStats->pdschTTIStats[internalCellIndex][writeTTIIndex].absoluteTTI = currentTick+1; \
                                                                               \
    if ( dlStrategyTxNode_p->transmissionType != INVALID_TX )                  \
    {                                                                          \
        if ( RE_TX == dlStrategyTxNode_p->transmissionType )                   \
        {                                                                      \
            gMacStats.pStats->pdschTTIStats[internalCellIndex][writeTTIIndex].numScheduledUERetx++; \
        }                                                                      \
        else if ( NEW_TX == dlStrategyTxNode_p->transmissionType )             \
        {                                                                      \
            gMacStats.pStats->pdschTTIStats[internalCellIndex][writeTTIIndex].numScheduledUENewTx++;\
        }                                                                                           \
        }                                                                      \
    numScheduledUETotal =                                                                           \
            gMacStats.pStats->pdschTTIStats[internalCellIndex][writeTTIIndex].numScheduledUETotal;  \
                                                                                                    \
    pLteMacPdschPerTTIPerUEStats = &gMacStats.pStats->pdschTTIStats[internalCellIndex][             \
                   writeTTIIndex].pdschStatsPerScheduledUE[numScheduledUETotal];                    \
                                                                               \
    pLteMacPdschPerTTIPerUEStats->ueIndex = ueContext_p->ueIndex;              \
    pLteMacPdschPerTTIPerUEStats->transmissionType =                           \
                                          dlStrategyTxNode_p->transmissionType;                     \
    pLteMacPdschPerTTIPerUEStats->harqProcessId =                              \
                                          dlStrategyTxNode_p->harqProcessId;   \
    pLteMacPdschPerTTIPerUEStats->tbOneTBSize =                                \
                                        harqProcess_p->dlHARQTBOneInfo.tbSize; \
    pLteMacPdschPerTTIPerUEStats->tbTwoTBSize =                                \
                                        harqProcess_p->dlHARQTBTwoInfo.tbSize; \
    pLteMacPdschPerTTIPerUEStats->codeWordOneMcs =                             \
                                  ueContext_p->dlCQIInfo.mcsIndexCodeWordOne;  \
    pLteMacPdschPerTTIPerUEStats->codeWordTwoMcs =                             \
                                  ueContext_p->dlCQIInfo.mcsIndexCodeWordTwo;  \
    pLteMacPdschPerTTIPerUEStats->allocatedPRBs =                              \
                                       harqProcess_p->assignedResourcesNB;     \
    pLteMacPdschPerTTIPerUEStats->isSubBandScheduling = 0;                     \
    pLteMacPdschPerTTIPerUEStats->dciFormat =                                  \
                                       dciCCEInfo_p->dciPduInfo.dciFormat;     \
    if ((SPS_OCCASION == dciCCEInfo_p->msgType) ||                             \
        (SPS_ACTIVATION == dciCCEInfo_p->msgType))                             \
    {                                                                          \
        pLteMacPdschPerTTIPerUEStats->resourceAllocationBitMap =  0;           \
        pLteMacPdschPerTTIPerUEStats->spsResourceAllocationBitMap =            \
                              dciCCEInfo_p->dciPduInfo.resrcAssgmntBitMap;     \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        pLteMacPdschPerTTIPerUEStats->resourceAllocationBitMap =               \
                              dciCCEInfo_p->dciPduInfo.resrcAssgmntBitMap;     \
        pLteMacPdschPerTTIPerUEStats->spsResourceAllocationBitMap =  0;        \
    }                                                                          \
    pLteMacPdschPerTTIPerUEStats->ratPoilicy =                                 \
                              dciCCEInfo_p->dciPduInfo.resrcAllocType;         \
    pLteMacPdschPerTTIPerUEStats->tpc = dciCCEInfo_p->dciPduInfo.tpc;          \
    pLteMacPdschPerTTIPerUEStats->excessPoolRB = excessPoolRb;                                      \
    pLteMacPdschPerTTIPerUEStats->allocationFailureCause = 0;                  \
    if ( MAX_TTI_STATS == gMacStats.pStats->ueCommonInfoStats[ueIndex].        \
                                                         pdschCurWriteIndex[internalCellIndex] )    \
    {                                                                          \
        gMacStats.pStats->ueCommonInfoStats[ueIndex].pdschCurWriteIndex[internalCellIndex] = 0;     \
    }                                                                          \
    pdschCurWriteIndex = gMacStats.pStats->ueCommonInfoStats[ueIndex].         \
                                                         pdschCurWriteIndex[internalCellIndex]++;   \
    pLtePdschUeSchdInfo = &gMacStats.pStats->ueCommonInfoStats[ueIndex].       \
                            pdschScheduledTTIIndex[internalCellIndex][pdschCurWriteIndex];          \
    pLtePdschUeSchdInfo->ueIndexPos = numScheduledUETotal;                     \
    pLtePdschUeSchdInfo->absoluteTTI = currentTick + 1;                        \
    gMacStats.pStats->pdschTTIStats[internalCellIndex][writeTTIIndex].numScheduledUETotal =         \
      gMacStats.pStats->pdschTTIStats[internalCellIndex][writeTTIIndex].numFailedUeTx +           \
      gMacStats.pStats->pdschTTIStats[internalCellIndex][writeTTIIndex].numScheduledUERetx +      \
      gMacStats.pStats->pdschTTIStats[internalCellIndex][writeTTIIndex].numScheduledUENewTx;      
#else
#define LTE_MAC_UPDATE_STATS_FOR_PDSCH( a,b,c,d,e,f,g);
#endif

#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_STATS_ALLOCATION_FAILURE_CAUSE(allocationFailCause,ueIndex, internalCellIndex)  \
    UInt16 writeTTIIdx = 0;                                                    \
    LteMacPdschPerTTIPerUEStats *pLteMacPdschPerTTIPerUEStat = PNULL;          \
    UInt8  numScheduledUETtl;                                                  \
    writeTTIIdx = gMacStats.pStats->writeTTIIndex[(internalCellIndex)];         \
    numScheduledUETtl = gMacStats.pStats->pdschTTIStats[(internalCellIndex)]    \
                                          [writeTTIIdx].numScheduledUETotal;    \
    gMacStats.pStats->pdschTTIStats[(internalCellIndex)]                        \
                               [writeTTIIdx].absoluteTTI = getCurrentTick()+1;  \
    pLteMacPdschPerTTIPerUEStat = &gMacStats.pStats->                          \
                               pdschTTIStats[(internalCellIndex)][writeTTIIdx]. \
                                pdschStatsPerScheduledUE[numScheduledUETtl];   \
                                                                               \
    pLteMacPdschPerTTIPerUEStat->allocationFailureCause =                      \
                                                      (allocationFailCause); \
    pLteMacPdschPerTTIPerUEStat->ueIndex = ueIndex;                           \
    gMacStats.pStats->pdschTTIStats[(internalCellIndex)][writeTTIIdx].numFailedUeTx++;              \
       gMacStats.pStats->pdschTTIStats[(internalCellIndex)][writeTTIIdx].numScheduledUETotal =      \
      gMacStats.pStats->pdschTTIStats[(internalCellIndex)][writeTTIIdx].numFailedUeTx +             \
      gMacStats.pStats->pdschTTIStats[(internalCellIndex)][writeTTIIdx].numScheduledUERetx +        \
      gMacStats.pStats->pdschTTIStats[(internalCellIndex)][writeTTIIdx].numScheduledUENewTx;      
#else
#define LTE_MAC_UPDATE_STATS_ALLOCATION_FAILURE_CAUSE(allocationFailCause,ueIndex, internalCellIndex)
#endif
/* CA Stats Changes End */


#ifdef OVERSHOOT_CORRECTION
#ifdef DL_UL_SPLIT
extern UInt64 ccFirstSF_g ; //cycleCount when first tick is recvd
extern UInt64 ccDelayInSF_g ; // Delay in receiveing the current SFI
extern UInt64 ccInMs_g ; //cycle count in 1 Ms obtained at init time.
extern UInt64 ccAtTickUpdate_g ; //cycle count on recving tick from phy
extern UInt8 ueSchdIndxInCurrTick_g ;
extern __thread UInt64 phyTickSend_g;
extern __thread UInt64 phyMsgRecv_g;
extern __thread UInt8 noTimeToProcessflag_g;
#else /*DL_UL_SPLIT*/
extern __thread UInt64 ccFirstSF_g ; //cycleCount when first tick is recvd
extern __thread UInt64 ccDelayInSF_g ; // Delay in receiveing the current SFI
/** CA2 Changes Start **/
extern UInt64 ccInMs_g ; //cycle count in 1 Ms obtained at init time.
/** CA2 Changes End **/
extern __thread UInt64 ccAtTickUpdate_g ; //cycle count on recving tick from phy
/*+ Dyn UEs To Be SchPerTTI +*/
/** CA-TDD Changes Start **/
extern __thread UInt8 dlUeSchdIndxInCurrTick_g;
/** CA-TDD Changes Stop **/
extern __thread UInt8 ulUeSchdIndxInCurrTick_g ;
extern __thread UInt8 reqIndexForUeSchdInDlUl_g ;
/*- Dyn UEs To Be SchPerTTI -*/
extern __thread UInt64 phyTickSend_g;
extern __thread UInt64 phyMsgRecv_g;
extern __thread UInt8 noTimeToProcessflag_g;
#endif /*DL_UL_SPLIT*/


#define PROCESS_TIME_REQD 800
#define PROCESS_NODE_TIME_REQD_IN_US 50
#define PROCESS_NODE_TIME_REQD ((ccInMs_g) * (PROCESS_NODE_TIME_REQD_IN_US)/(1000)) 

#define GET_CYCLE_COUNT(cpuCycles)\
   do{\
       cpuCycles = rdtsc();\
     }while(0);

#define GET_CYCLES_DIFF(diffCycles)\
do{\
     if (phyMsgRecv_g > phyTickSend_g)\
        diffCycles = phyMsgRecv_g - phyTickSend_g;\
     else\
        diffCycles = RDTSC_MAX + phyMsgRecv_g - phyTickSend_g;\
  }while(0);

#define GET_FIRST_SFI_CC()\
   do{\
       ccFirstSF_g = rdtsc();\
   }while (0);

#define GET_FIRST_CC()\
    do{\
        ccAtTickUpdate_g = rdtsc();\
    }while(0);


#define SFI_PROCESSING_START(CurrentTick)\
   do{\
         ccAtTickUpdate_g = rdtsc();\
         if( ccDelayInSF_g > ccAtTickUpdate_g - (ccFirstSF_g  +( CurrentTick *  ccInMs_g)))\
             ccDelayInSF_g = ccAtTickUpdate_g - (ccFirstSF_g  +( CurrentTick *  ccInMs_g));\
         else\
         ccDelayInSF_g = RDTSC_MAX + ccAtTickUpdate_g - (ccFirstSF_g  +( CurrentTick *  ccInMs_g));\
     }while (0);

#define GET_USED_CYCLES(usedCycles)\
   do{\
      UInt64  cur_cycles = rdtsc();\
      if(cur_cycles > ccAtTickUpdate_g)\
          usedCycles = cur_cycles - ccAtTickUpdate_g;\
      else\
         usedCycles = RDTSC_MAX - ccAtTickUpdate_g + cur_cycles;\
     }while (0);

/* Review comment fix start LJ18 */
#ifdef ADJUST_SFI_DELAY_FOR_OVERSHOOT_CORRECTION
#define GET_PROCESSING_CYCLES_LEFT(rem_cycles) \
   do{\
      UInt64 usedCycles = 0;\
      UInt64  cur_cycles = rdtsc();\
      if(cur_cycles > ccAtTickUpdate_g)\
      {\
          usedCycles = cur_cycles - ccAtTickUpdate_g;\
      }\
      else\
      {\
         usedCycles = RDTSC_MAX - ccAtTickUpdate_g + cur_cycles;\
      }\
      if (rem_cycles >= (ccInMs_g  - ccDelayInSF_g - usedCycles))\
      {\
          rem_cycles =  ccInMs_g  - ccDelayInSF_g - usedCycles;\
      }\
      if( usedCycles > (rem_cycles * PROCESS_TIME_REQD/1000))\
      {\
          rem_cycles = 0;\
      }\
      else\
      {\
          rem_cycles =  (rem_cycles * PROCESS_TIME_REQD/1000)  - usedCycles;\
      }\
     }while(0);
#else
/* Review comment fix end LJ18 */
#define GET_PROCESSING_CYCLES_LEFT(rem_cycles) \
   do{\
      UInt64 usedCycles = 0;\
      UInt64  cur_cycles = rdtsc();\
      if(cur_cycles > ccAtTickUpdate_g)\
      {\
          usedCycles = cur_cycles - ccAtTickUpdate_g;\
      }\
      else\
      {\
         usedCycles = RDTSC_MAX - ccAtTickUpdate_g + cur_cycles;\
      }\
      /*  rem_cycles =  ccInMs_g  - ccDelayInSF_g - usedCycles;*/\
      if( usedCycles > (ccInMs_g * PROCESS_TIME_REQD/1000))\
      {\
          rem_cycles = 0;\
      }\
      else\
      {\
          rem_cycles =  (ccInMs_g * PROCESS_TIME_REQD/1000)  - usedCycles;\
      }\
     }while(0);
#endif

/** CA2 Changes Start **/
#ifdef DL_UL_SPLIT
    /*SPR 16855 +-*/
#define MIN_CC_REQD_TO_PROCEED(cyclesReqd) \
    do{\
        (cyclesReqd) = reqElProcessingCyclesUL[ueSchdIndxInCurrTick_g][5];\
        UInt32 i;\
        for(i = 4; i < 13; i++)\
        {\
            (cyclesReqd)+= reqElProcessingCyclesDL[ueSchdIndxInCurrTick_g][i];\
        }\
    }while (0);
#else /*DL_UL_SPLIT*/
#define MIN_CC_REQD_TO_PROCEED(cyclesReqd,internalCellIndex)\
  do{\
       /*+ Dyn UEs To Be SchPerTTI +*/\
       cyclesReqd = reqElProcessingCycles[internalCellIndex][reqIndexForUeSchdInDlUl_g][RLC_TX_OPP];\
       /*- Dyn UEs To Be SchPerTTI -*/\
       UInt32 i;\
       for(i = MAC_PUSCH_FOR_DELAY_ONE ; i < MAC_DEMUX_QUEUE; i++)\
       {\
       /*+ Dyn UEs To Be SchPerTTI +*/\
              cyclesReqd += reqElProcessingCycles[internalCellIndex][reqIndexForUeSchdInDlUl_g][i];\
       /*- Dyn UEs To Be SchPerTTI -*/\
       }\
    }while (0);
/* TDD Tick overshoot correction changes start*/
#ifdef TDD_CONFIG
/** CA-TDD Changes Start **/
#define MIN_CC_REQD_TO_PROCEED_AT_MUX(cyclesReqd,internalCellIndex)\
  do{\
       getSFAndSFN(&systemFrameNum,&subFrameNum,internalCellIndex);\
       /*+ Dyn UEs To Be SchPerTTI +*/\
       cyclesReqd = reqdProcessingCyclesAtMuxTDD_g[internalCellIndex][subFrameNum][reqIndexForUeSchdInDlUl_g];\
       /*- Dyn UEs To Be SchPerTTI -*/\
    }while (0);
/** CA-TDD Changes Stop **/
#endif /* TDD_CONFIG */ 
/* TDD Tick overshoot correction changes end*/
#endif /*DL_UL_SPLIT*/

/** CA2 Changes End **/
#define MAX_UES_SCHEDULED_INDX 9

typedef enum OverLoadLevelT
{
    OVERLOAD_LEVEL_NORMAL,
    OVERLOAD_LEVEL_MEDIUM,
    OVERLOAD_LEVEL_HIGH,
    OVERLOAD_LEVEL_CRITICAL
}OverLoadLevel;


#ifdef DL_UL_SPLIT
extern UInt32 reqElProcessingTimeDL[MAX_UES_SCHEDULED_INDX][
/*SPS_CHANGE_START*/
    /*SPR 16855 +-*/
    15 /* execution leg count */
    /*SPR 16855 +-*/
];
/*SPS_CHANGE_END*/
extern UInt64 reqElProcessingCyclesDL[MAX_UES_SCHEDULED_INDX][
/*SPS_CHANGE_START*/
    /*SPR 16855 +-*/
                            15 /* execution leg count */
    /*SPR 16855 +-*/
];
extern UInt32 reqElProcessingTimeUL[MAX_UES_SCHEDULED_INDX][
    /*SPR 16855 +-*/
                            20 /* execution leg count */
    /*SPR 16855 +-*/
];
extern UInt64 reqElProcessingCyclesUL[MAX_UES_SCHEDULED_INDX][
    /*SPR 16855 +-*/
                            20 /* execution leg count */
    /*SPR 16855 +-*/
] ;
/*SPS_CHANGE_END*/
extern OverLoadLevel currTickOverLoadDL_g ;  /*  over load level at current tick in DL Thread*/
extern OverLoadLevel currTickOverLoadUL_g ;  /*  over load level at current tick in UL Thread*/
extern OverLoadLevel macOverLoadLevel_g[MAX_NUM_CELL];     /*  mac overload level */

#else /*DL_UL_SPLIT*/

typedef enum CriticalExecutionLegsT{
 MAC_DL_HARQ_ACK_NACK_QUEUE = 1,
 MAC_HARQ_TIMER_EXPIRY,
 MAC_DL_CA_TIMER_EXPIRY,
/*SPS_CHANGE_START*/
 MAC_Dl_SPS_STRATEGY,
 MAC_SET_STRATEGY_FD,
 MAC_CHECK_STRATEGY_FD,
 MAC_DL_SCH_STRATEGY,
 MAC_SET_PDCCH_PDSCH_FD,
 MAC_UL_HARQ_TIMER_EXPIRY,
 MAC_Ul_SPS_STRATEGY,
 MAC_UL_SCH_STRATEGY,
 /* + E_CID_PRS_TO_5.3*/
 MAC_PRS_TRANSMISSION,
 /* - E_CID_PRS_TO_5.3*/
 MAC_CHECK_PDCCH_PDSCH_FD,
 MAC_PDCCH_PDSCH_WITH_MUX,
 MAC_SET_MUX_FD,
 MAC_CHECK_MUX_FD,
 MAC_MUX_QUEUE,
 RLC_TX_OPP,
 MAC_PUSCH_FOR_DELAY_ONE,
 MAC_SRS_REPORTS,
 MAC_SET_ENCODER_FD,
 MAC_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA,
 MAC_CHECK_ENCODER_FD,
 MAC_ENCODER_QUEUE_OF_OTHER_CELLS,
 MAC_DL_TX_CONTROL_MSG,
 MAC_DL_TX_DATA_MSG,
 MAC_DRX_FOR_ALL_UE,
 MAC_MEAS_GAP_FOR_ALL_UE,
 MAC_DEMUX_QUEUE,
 MAC_UL_RA,
 MAC_CQI_REPORTS,
 MAC_SPS_PDSCH,
 MAC_UL_CSI_SINR_REPORTS,
 MAC_SPS_PUSCH,
 /* + E_CID_PRS_TO_5.3*/
 MAC_E_CID_REPORTS, 
 MAC_UPDATE_DL_TOP_SCHED_LIST_SCELL_STATE,
 /* - E_CID_PRS_TO_5.3*/
 /*SPR 16855 +-*/
 MAC_UL_OAM_AND_RRC_MSG,
 MAC_DL_OAM_AND_RRC_MSG,
 MAC_UL_PENDING_DELETE_UE,
 /*SPR 16855 +-*/
 MAC_PENDING_DELETE_UE,
 MAC_PENDING_RESET_UE,
 MAC_AUDIT_REPORT_FOR_INACTIVE_UE,
 MAC_MISC_UPDATES,
 /*RLC_TX_OPP*/
/*SPS_CHANGE_END*/
#ifdef OVERSHOOT_CORRECTION 
RLC_TIMER_EXPIRY,
 RLC_HANDLE_RESTAB_Q,
#endif 
 MAX_CRITICAL_LEGS
} CriticalExecutionLegsInfo;

/** CA2 Changes Start **/
extern UInt64 reqElProcessingCycles[MAX_NUM_CELL][MAX_UES_SCHEDULED_INDX][MAX_CRITICAL_LEGS];
extern OverLoadLevel macOverLoadLevel_g[MAX_NUM_CELL] ; /*mac overload level */
/** CA2 Changes End **/
extern UInt32 reqElProcessingTime[MAX_UES_SCHEDULED_INDX][MAX_CRITICAL_LEGS];
extern OverLoadLevel currTickOverLoad_g[MAX_NUM_CELL] ;  /*  over load level at current tick*/

#endif /*DL_UL_SPLIT*/
#else /*OVERSHOOT_CORRECTION*/
#define GET_FIRST_SFI_CC()
#define GET_FIRST_CC() 
#define SFI_PROCESSING_START(CurrentTick)
#define GET_PROCESSING_CYCLES_LEFT(rem_cycles) 
#endif /*OVERSHOOT_CORRECTION*/

#ifdef KPI_STATS
/* 100MS_SUPPORT : start */
#define KPI_CONFIG_ENUM_BASE 10000
/* 100MS_SUPPORT : end */
/* + KPI_CA */
extern void prepareMACApiHeader( UInt8 *msgBuf, UInt16 msgId, UInt16 destModuleId,
                                 UInt16 transactionId, UInt16 length,
								 InternalCellIndex internalCellIndex);

extern void  pdcpResetKpiCounters(UInt32 passiveIndexForPdcpStats,
					InternalCellIndex internalCellIndex);

extern void resetKpiContainer(UInt8 moduleId, InternalCellIndex internalCellIndex);
/* SPR 15909 fix start */
extern tickType_t getCurrentTick(void);
/* SPR 15909 fix end */
extern void processStatsBuffBeforeConfig(UInt16 macBitmask, UInt8 pdcpBitmask, 
					InternalCellIndex internalCellIndex);

extern void  macResetModuleKpiCounters(void *localStats, UInt32 size);

/* SPR 10579 changes start*/
extern void  macResetModuleKpiThpCounters(void *localStats, UInt32 size);
/* SPR 10579 changes end*/

extern void resetPdcpThpStats(UInt32 passiveIndex);

/* SPR 10579 changes start*/
extern void resetMacThpStats(UInt32 passiveIndex);
/* SPR 10579 changes end*/

extern void updateStatsForAllModules( InternalCellIndex internalCellIndex);
/* - KPI_CA */
#endif

/****************************************************************************
 * Exported Types
 ***************************************************************************/

  
typedef enum ExecutionLegT
{
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
    /*enum for MAC*/
    PROCESS_DL_HARQ_ACK_NACK_QUEUE,
    PROCESS_DL_HARQ_TIMER_EXPIRY,
    PROCESS_DL_SPS_STRATEGY,
    PROCESS_DL_SCH_STRATEGY,
    PROCESS_PDCCH_PDSCH_WITH_MUX,
    /* PDCCH_PDSCH_WITHOUT_MUX Start */
    PROCESS_MUX_QUEUE,
    /* PDCCH_PDSCH_WITHOUT_MUX End */
    PROCESS_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA,
    TX_DL_CONTROL_MSG,
    TX_DL_DATA_MSG,
    PROCESS_DRX_FOR_ALL_UE,
    PROCESS_MEAS_GAP_FOR_ALL_UE,
    PROCESS_DELETE_SCHEDULE_LC_LIST,   /*CA_CHG_START*/
    PROCESS_DL_OAM_AND_RRC_MSG,
    PROCESS_PENDING_DELETE_UE,
    PROCESS_PENDING_RESET_UE,
    PROCESS_MISC_UPDATES,
    PROCESS_RX_UNTIL_SUBFRAME_IND,
    PROCESS_UL_HARQ_TIMER_EXPIRY,
    PROCESS_UL_SPS_STRATEGY,
    PROCESS_UL_SCH_STRATEGY,
    PROCESS_PUSCH_FOR_DELAY_ONE,
    PROCESS_DEMUX_QUEUE,
    PROCESS_SRS_REPORTS,
    PROCESS_UL_CSI_SINR_REPORTS,
    PROCESS_SPS_PDSCH,
    PROCESS_CQI_REPORTS,
    PROCESS_SPS_PUSCH,
    /* + E_CID_5.3*/
    PROCESS_E_CID_REPORTS, 
    /* - E_CID_5.3*/
    PROCESS_PRS_TRANSMISSION, /* PRS_CHANGE */
    PROCESS_UL_RA,
    PROCESS_UL_OAM_AND_RRC_MSG,
    PROCESS_UL_PENDING_DELETE_UE,
    PROCESS_UL_PENDING_RESET_UE,
    PROCESS_DL_PERFORMANCE_STATS_UPDATE,
    PROCESS_UL_PERFORMANCE_STATS_UPDATE,
    /*Start Changes for TDD DL UL SPLIT*/
#ifdef DL_UL_SPLIT_TDD
    PROCESS_SUB_FRAME_IND_UL,
    WAIT_FOR_TDD_SUBFRAME_IND,
    PROCESS_HARQ_TIMER_EXPIRY,
    PROCESS_ENCODER_QUEUE_WITHOUT_TX,
    PROCESS_UL_CONFIG_MSG,
    TX_UL_CONFIG_MSG,
    PROCESS_SRS_FOR_U_SUBFRAME,
    /* TDD Config 0 changes start */
    PROCESS_MERGE_UL_STARTEGY_NODES,
    /* TDD Config 0 changes end */
    /* EL for PRACH for S subframe for Preamble format 4 */
    PROCESS_PRACH_FOR_S_SUBFRAME,
    PROCESS_SRS_FOR_S_SUBFRAME,
    RLC_HANDLE_TX_OPPORTUNITY,
    RLC_HANDLE_HARQ_FAIL_IND,
    PROCESS_MEM_CLEANUP,
    TX_HI_DCI0_MSG,
    PROCESS_UL_PDCCH_WITH_MUX,
    PROCESS_DL_PDCCH_SYNC,
#ifdef FAPI_CONTROL_FLOW
    FAPI_DL_FLOW_SYNC,
    FAPI_UL_FLOW_SYNC,
#endif
#endif
    /*End Changes for TDD DL UL SPLIT*/
    SEND_AUDIT_REPORT_FOR_INACTIVE_UE,
    MAX_EL_MAC = SEND_AUDIT_REPORT_FOR_INACTIVE_UE,
#define MAX_MAC_CC_FUNC MAX_EL_MAC
    /*enum for MAC*/
    /*enum from RLC*/
    PROCESS_RLC_HANDLE_HARQ_FAILURE_IND = (MAX_EL_MAC + 1),
    PROCESS_RLC_UL_CHECK_TIMER_EXPIRY,
    PROCESS_RLC_UL_HDLR_REESTABLISH_Q,
    PROCESS_RLC_UL_HDLR_PEN_DEL_Q,
    /* SPR 9911 fix start */
    PROCESS_RLC_HANDLE_STATUS_REPORT,
    /* SPR 9911 fix end */
    PROCESS_RLC_HANDLE_DATA_FROM_MAC,
    PROCESS_RLC_DL_CHECK_TIMER_EXPIRY,
    PROCESS_RLC_DL_HDLR_REESTABLISH_Q,
    PROCESS_RLC_DL_HDLR_PEN_DEL_Q,
    PROCESS_RLC_HANDLE_TX_OPPORTUNITY,
    MAX_EL_RLC = PROCESS_RLC_HANDLE_TX_OPPORTUNITY,
#define MAX_RLC_CC_FUNC (MAX_EL_RLC - MAX_MAC_CC_FUNC)
    /*enum for RLC*/
    /*enum for Others*/
    PROCESS_START_PHY = (MAX_EL_RLC + 1),
    PROCESS_OAM_AND_RRC_MSG,
    WAIT_FOR_SUBFRAME_IND,
    DEFAULT_STARTUP_EXEC_LEG,
    REGISTER_WITH_PHY_FOR_SUBFRAME_IND_AFTER_INIT_MAC,
    INIT_DL_THREAD_PARAMS,
    INIT_UL_THREAD_PARAMS,
    INIT_RLC_PARAMS, 
    /*enum for others*/
    /*ca chngs */
    UPDATE_DL_TOP_SCH_LIST_SCELL_STATE,
    PROCESS_DL_CA_TIMER_EXPIRY,
#else 

    /* Wait for subframe indication */
    WAIT_FOR_SUBFRAME_IND = 1,
    /* Process for Downlink harq timer expiry if enough time is left in that tick*/
    PROCESS_HARQ_TIMER_EXPIRY_IF_TIME_LEFT,
    /* Process for CQI reports if enough time is left in that tick*/
    PROCESS_CQI_REPORTS_IF_TIME_LEFT,
    /* Process demultiplexing requests */
    PROCESS_DEMUX_QUEUE,
    /* Process demultiplexing requests only if enough time is left in that tick */
    PROCESS_DEMUX_QUEUE_IF_TIME_LEFT,
    /* Process multiplexing queue based on mux queue count*/
    PROCESS_MUX_QUEUE,
    /* Process multiplexing queue until last node sent by pusch scheduler*/
    PROCESS_MUX_QUEUE_UNTIL_LAST_NODE,
    /* Process certain number of RRC/OAM msgs only if enough time left*/
    PROCESS_N_RRC_OAM_MSG_IF_TIME_LEFT,
    /* Process PDCCH and PDSCH including MUX functionality */
    PROCESS_PDCCH_PDSCH_WITH_MUX,
    /* Process PDCCH and PDSCH without MUX functionality */
    PROCESS_PDCCH_PDSCH_WITHOUT_MUX,
    /* Process PDCCH and PDSCH including MUX functionality and send trigger to PUSCH
     * after processing PDCCH */
    PROCESS_PDCCH_PDSCH_WITH_MUX_AND_TRIGGER_PUSCH,
    /* Process PDCCH and PDSCH without MUX functionality and send trigger to PUSCH
     * after processing PDCCH */
    PROCESS_PDCCH_PDSCH_WITHOUT_MUX_AND_TRIGGER_PUSCH,
    /* Execute PUSCH processing for delay one */
    PROCESS_PUSCH_FOR_DELAY_ONE,
    TX_DL_DATA_MSG,
    PROCESS_TX_QUEUE_UNTIL_LAST_NODE,
    PROCESS_TX_QUEUE,
    TX_COMMON_CHANNEL_DATA,
    TX_DL_CONTROL_MSG,
    TX_DL_DATA_MSG_IF_DATA_READY,
    TX_DL_CONTROL_MSG_AND_TRIGGER,
    /* Process encoder request until dummy node which is the last node for that tick */
    PROCESS_ENCODER_QUEUE_WITH_TX_UNTIL_LAST_DATA,
    PROCESS_DL_HARQ_ACK_NACK_QUEUE,
    /*rel4-drop2 cganges*/
    /*SPR 16855 +-*/
    PROCESS_DL_OAM_AND_RRC_MSG,
    PROCESS_UL_OAM_AND_RRC_MSG,
    PROCESS_UL_PENDING_DELETE_UE,
    PROCESS_START_PHY,
    PROCESS_OAM_AND_RRC_MSG,
    /*SPR 16855 +-*/
    /*rel4-drop2 changes*/
    PROCESS_PENDING_DELETE_UE,
    PROCESS_PENDING_RESET_UE,
    SEND_AUDIT_REPORT_FOR_INACTIVE_UE,
    PROCESS_UL_RA,
    PROCESS_DL_SPS_STRATEGY,
    PROCESS_DL_SCH_STRATEGY,
    PROCESS_UL_SPS_STRATEGY,
    PROCESS_UL_SCH_STRATEGY,
    PROCESS_RX_CONTROL_DATA_WITH_DEMUX,
    PROCESS_HARQ_TIMER_EXPIRY,
    /** CA 2 Core Changes Start **/
    PROCESS_DL_HARQ_TIMER_EXPIRY,
    PROCESS_UL_HARQ_TIMER_EXPIRY,
    RLC_HANDLE_TX_OPPORTUNITY,
    /** CA 2 Core Changes End **/
	
    PROCESS_CQI_REPORTS,
    PROCESS_N_CQI_REPORTS,
    PROCESS_N_RRC_OAM_MSG,
    REGISTER_WITH_PHY_FOR_SUBFRAME_IND,
    REGISTER_WITH_PHY_FOR_SUBFRAME_IND_AFTER_INIT_MAC,
    WAIT_FOR_CELL_CONFIG,
    WAIT_FOR_PDCCH_READY_TRIGGER,
    PROCESS_N_DEMUX_QUEUE,
    WAIT_FOR_CONTROL_MSG_SENT_TRIGGER,
    PROCESS_RX_UNTIL_SUBFRAME_IND,
    PROCESS_RX_CONTROL_DATA_WITHOUT_DEMUX,
	
    PROCESS_ENCODER_QUEUE_WITH_TX,
    PROCESS_ENCODER_QUEUE_WITHOUT_TX,
    PROCESS_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA,
    PROCESS_PUSCH_FOR_DELAY_TWO,
    PROCESS_PUSCH_FOR_DELAY_THREE,
    PROCESS_UL_SCH_STRATEGY_IF_NOT_ALREADY_DONE,
    SEND_CONTROL_TRIGGER,
    RX_CONTROL_TRIGGER,
    DEFAULT_STARTUP_EXEC_LEG,
    PROCESS_UL_CONFIG_MSG,
    TX_UL_CONFIG_MSG,
    PROCESS_SPS_PDSCH,
    PROCESS_UL_CSI_SINR_REPORTS,
    PROCESS_SPS_PUSCH,
    PROCESS_E_CID_REPORTS, 
    PROCESS_PRS_TRANSMISSION,
#ifdef TDD_CONFIG
	WAIT_FOR_TDD_SUBFRAME_IND,
    /* EL for PRACH for S subframe for Preamble format 4 */
    PROCESS_PRACH_FOR_S_SUBFRAME,
    /* Execution leg for processing SRS for special subframes*/
    PROCESS_SRS_FOR_S_SUBFRAME,
    /*Execution leg for processing SRS for uplink subframes*/
    PROCESS_SRS_FOR_U_SUBFRAME,
 /* TDD Config 0 changes start */
    PROCESS_MERGE_UL_STARTEGY_NODES,
 /* TDD Config 0 changes end */
#endif
    /*Execution leg for processing SRS for uplink subframes*/
    PROCESS_SRS_REPORTS,
    PROCESS_DRX_FOR_ALL_UE, 
    PROCESS_MEAS_GAP_FOR_ALL_UE, 
    PROCESS_DELETE_SCHEDULE_LC_LIST, 
#ifdef TDD_CONFIG
    PDCP_TX_PROCESS_MESSAGE,
    RLC_HANDLE_CTRL_PDU_RLC_AM,
    RLC_HANDLE_HARQ_FAIL_IND,
    RLC_PDCP_HANDLE_DATA_FRM_MAC,
    RLC_TIMER_CHK_FOR_EXPIRY,
    RLC_TIMER_CHK_FOR_TX_EXPIRY,
    PDCP_TX_PROCESS_DELIVERY_IND,
    RLC_HANDLE_COMMON_CHANNEL_AND_PENDING_QUEUE,
    /* SPR 13373 fix start */
    RLC_HANDLE_PENDING_DELETE_UE,
    /* SPR 13373 fix end */
    PROCESS_RLC_PDCP_MSG,
#endif
    PROCESS_MISC_UPDATES,
#ifdef UTFWK_SIMULATION
    UPDATE_TICK_RX_MSQ,
    RLC_UPDATE_QLOAD,
    RLC_HANDLE_TX_OPPORTUNITY_QOS,
#endif

#ifdef PERF_STATS
    PROCESS_MAC_PERF_STATS,
#endif
#ifdef TDD_CONFIG
        PROCESS_MEM_CLEANUP,
#endif // TDD_CONFIG

/* SPR 8712 changes */
    UPDATE_DL_TOP_SCH_LIST_SCELL_STATE,
    PROCESS_DL_CA_TIMER_EXPIRY,
    PROCESS_PENDING_EVENT_FD,
#endif
#ifdef DL_DATA_SEPARATION
    PROCESS_RLC_HANDLE_TX_OPPORTUNITY_WITH_ENCODER_PER_UE,
    RLC_UPDATE_BUFF_OCC_TO_MAC,
    PROCESS_DL_SYNC_TX_DATA,
#endif
/* SPR 8712 changes */
    /**EL Sync Changes Start **/
/* SPR 14619 changes */
#ifndef DL_UL_SPLIT
/* SPR 14619 changes */
    PROCESS_ENCODER_QUEUE_OF_OTHER_CELLS,
    PROCESS_SET_STRATEGY_FD,
    PROCESS_CHECK_STRATEGY_FD,
    PROCESS_SET_PDCCH_PDSCH_FD,
    PROCESS_CHECK_PDCCH_PDSCH_FD,
    PROCESS_SET_MUX_FD,
    PROCESS_CHECK_MUX_FD,
    PROCESS_SET_ENCODER_FD,
    PROCESS_CHECK_ENCODER_FD,
#endif
    /**EL Sync Changes End **/
#ifdef LTE_EMBMS_SUPPORTED 
    RLC_HANDLE_PRE_OPPORTUNITY,
    PROCESS_EMBMS_STRATEGY,
#endif

    MAX_LAST_EL, /* Coverity CID 64932 28-JUN-2014 Fix */
    MAX_EXECUTION_LEGS
}ExecutionLeg;

typedef enum DLTxnTypeT
{
    MAC_RETX = 0,
    MAC_TX,
    MAC_NO_TX
}DLTxnType;


typedef enum ULGrantTypeT
{
    /*When UL Grant is request due to Scheduling Request*/
    MAC_SR_ULGRANT_REQ_MSG = 0,
    /*When UL Grant is request due to Contention Resolution*/
    MAC_CR_ULGRANT_REQ_MSG,
    /*When UL Grant is request for Aperiodic CQI Reports*/
    MAC_CQI_WITH_DATA_ULGRANT_REQ_MSG,
    /*When UL Grant is request for Aperiodic CQI Reports*/
    MAC_CQI_WITHOUT_DATA_ULGRANT_REQ_MSG,
    /*New UL Grant */
    MAC_NEW_ULGRANT_REQ_MSG,
    /* +COVERITY 19408 */
    /* When UL Grant is request for TPC WITH DATA */
    MAC_UL_TPC_WITH_DATA,
    /* When UL Grant is request for TPC WITHOUT DATA */
    MAC_UL_TPC_WITHOUT_DATA,
    MAC_UL_SPS_ACTIVATION,
    MAC_UL_SPS_DEACTIVATION,
    MAC_UL_SPS_OCCASION,
    /* +DYNAMIC_ICIC */
    MAC_UL_EXPLICIT_PHR_WITHOUT_DATA
    /* -DYNAMIC_ICIC */
    /* -COVERITY 19408 */
}ULGrantType;

#ifdef DL_DATA_SEPARATION
typedef enum EncoderFuncIdentifierT
{
 RLC_HANDLE_TX_OPPORTUNITY_WITH_ENCODER_PER_UE,
 PROCESS_MAC_TX_DL_DATA_MSG,
 MAX_ENC_CC_FUNC
}EncoderFuncIdentifier;
#endif

#if (defined(TDD_CONFIG) && !defined(DL_UL_SPLIT_TDD))
typedef enum statsLegFuncIdentifierT
{
 MAX_LEG_CC_FUNC = MAX_EXECUTION_LEGS
}statsLegFuncIdentifier;
#endif

#ifdef DL_UL_SPLIT_TDD

typedef enum macRlcFuncIdentifierT
{
 MAX_MAC_RLC_CC_FUNC = MAX_EXECUTION_LEGS
}macRlcLegFuncIdentifier;

#endif

#if (!defined DL_UL_SPLIT) && (!defined DL_UL_SPLIT_TDD)
/*Stats Related Changed Start*/
typedef enum MacFuncIdentifierT
{
 MAC_PROCESS_RX_UNTIL_SUBFRAME_IND = 0,
#ifdef UTFWK_SIMULATION
 RLC_UPDATE_QLOAD,
#endif
 MAC_PROCESS_DL_HARQ_ACK_NACK_QUEUE,
 MAC_PROCESS_HARQ_TIMER_EXPIRY,
 MAC_PROCESS_DL_CA_TIMER_EXPIRY,
 MAC_PROCESS_DL_SPS_STRATEGY,
 /* SPR 17912 Fix Start*/
 MAC_PROCESS_PENDING_EVENT_FD,
 /* SPR 17912 Fix End*/
 MAC_PROCESS_SET_STRATEGY_FD,
 MAC_PROCESS_CHECK_STRATEGY_FD,
 MAC_PROCESS_DL_SCH_STRATEGY,
 MAC_PROCESS_SET_PDCCH_PDSCH_FD,
#ifndef OVERSHOOT_CORRECTION
 MAC_PROCESS_DEMUX_QUEUE,
#endif
 MAC_PROCESS_UL_HARQ_TIMER_EXPIRY,
 MAC_PROCESS_UL_SPS_STRATEGY,
 MAC_PROCESS_UL_SCH_STRATEGY,
 MAC_PROCESS_PRS_TRANSMISSION,
 MAC_PROCESS_CHECK_PDCCH_PDSCH_FD,
 MAC_PROCESS_PDCCH_PDSCH_WITH_MUX,
 MAC_PROCESS_SET_MUX_FD,
 MAC_PROCESS_CHECK_MUX_FD,
 MAC_PROCESS_MUX_QUEUE,
 MAC_RLC_HANDLE_TX_OPPORTUNITY,
 /* SPR 17912 Fix Start*/
 MAC_PROCESS_SET_ENCODER_FD,
 MAC_PROCESS_PUSCH_FOR_DELAY_ONE,
 MAC_PROCESS_SRS_REPORTS,
 /* SPR 17912 Fix End*/
#ifndef DL_DATA_SEPARATION
 MAC_PROCESS_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA,
#endif
 MAC_PROCESS_CHECK_ENCODER_FD,
 MAC_PROCESS_ENCODER_QUEUE_OF_OTHER_CELLS,
 MAC_TX_DL_CONTROL_MSG,
#ifndef DL_DATA_SEPARATION
 MAC_TX_DL_DATA_MSG,
#endif
 MAC_PROCESS_DRX_FOR_ALL_UE, /* DRX_CHG */
 MAC_PROCESS_MEAS_GAP_FOR_ALL_UE, /* MEAS_GAP_CHG */
 MAC_PROCESS_DELETE_SCHEDULE_LC_LIST, 
#ifdef OVERSHOOT_CORRECTION
 MAC_PROCESS_DEMUX_QUEUE,
#endif
 MAC_PROCESS_UL_RA,
 MAC_PROCESS_CQI_REPORTS,
 MAC_PROCESS_SPS_PDSCH,
 MAC_PROCESS_UL_CSI_SINR_REPORTS,
 MAC_PROCESS_SPS_PUSCH,
 MAC_PROCESS_E_CID_REPORTS,
 MAC_UPDATE_DL_TOP_SCH_LIST_SCELL_STATE,
#ifdef UTFWK_SIMULATION
 RLC_HANDLE_TX_OPPORTUNITY_QOS,
#endif
#ifdef DL_DATA_SEPARATION 
 PROCESS_MAC_DL_SYNC_TX_DATA,
 RLC_UPDATE_BUFFER_OCC_TO_MAC,
#endif
 /*SPR 16855 +-*/
 MAC_PROCESS_UL_OAM_AND_RRC_MSG,
 MAC_PROCESS_DL_OAM_AND_RRC_MSG,
 MAC_PROCESS_UL_PENDING_DELETE_UE,
 /*SPR 16855 +-*/
 MAC_PROCESS_PENDING_DELETE_UE,
 MAC_PROCESS_PENDING_RESET_UE,
 MAC_SEND_AUDIT_REPORT_FOR_INACTIVE_UE,
 MAC_PROCESS_MISC_UPDATES,
 /*SPR 16855 +-*/
 MAC_PROCESS_START_PHY,
 NORMAL_MODE_FOR_MAC_EXEC_SCH,
 MAX_MAC_CC_FUNC
}MacFuncIdentifier;
/*Stats Related Changed End*/

typedef enum RlcFuncIdentifierT
{
 RLC_HANDLE_TRX_OPPORTUNITY = 0,
 RLC_HANDLE_CONTROL_PDU_RLC_AM,
 RLC_HANDLE_HARQ_FAILURE_IND,
 RLC_HANDLE_DATA_FROM_MAC,
 RLC_TIMER_UPD_TICKCHECK_FOR_TIMER_EXE,
 RLC_HANDLE_TX_EXPIRED_TIMER,
 RLC_HANDLE_RX_EXPIRED_TIMER,
 RLC_HANDLE_COMCH_DATA_FROM_MAC,
 RLC_HANDLE_COMCH_TX_OPPORTUNITY,
 RLC_HDLR_PEN_DEL_Q,
 RLC_HDLR_REESTABLISH_Q, 
 MAX_RLC_CC_FUNC 
}RlcFuncIdentifier;
#endif

#if (defined(DL_UL_SPLIT_TDD) || !defined(TDD_CONFIG))
typedef enum PdcpFuncIdentifierT
{
 PDCP_PROCESS_SRB_TX,
 PDCP_PROCESS_DRB_TX,
 PDCP_PROCESS_RX,
 PDCP_PROCESS_TX_DELIVERY_STATUS_IND,
 PDCP_ROHC_PROCESS_TIMERS,
 PDCP_PROCESS_DISCARD_TIMERS,
 PDCP_PROCESS_TX_CNTRL_PDU_IND,
#ifdef PDCP_ASYNC_INTERFACE
 PDCP_SECURITY_PROCESS_TX,
 PDCP_SECURITY_PROCESS_RX,
 PDCP_RX_PROCESS_DECIPHERED_PDU,
 PDCP_POLL_SECURITY_ENGINE,
#endif
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
 NORMAL_MODE_FOR_MAC_EXEC_SCH,
#endif
 MAX_PDCP_CC_FUNC 
}PdcpFuncIdentifer;

typedef enum SocketFuncIdentifierT
{
#ifdef DL_UL_SPLIT
 PDCP_SOCKET_PROCESS_RX = PDCP_PROCESS_RX,
 MAC_SOCKET_TX_DL_CONTROL_MSG = TX_DL_CONTROL_MSG,
/* SPR 8712 changes */
#ifndef DL_DATA_SEPARATION 
 MAC_SOCKET_TX_DL_DATA_MSG = TX_DL_DATA_MSG,
#else
 MAC_SOCKET_TX_DL_DATA_MSG = PROCESS_MAC_TX_DL_DATA_MSG,
#endif
/* SPR 8712 changes */
 MAC_SOCKET_OAM_AND_RRC_MSG = PROCESS_DL_OAM_AND_RRC_MSG,
 MAC_SOCKET_NORMAL_MODE_RECV_FROM = NORMAL_MODE_FOR_MAC_EXEC_SCH,
 MAC_SOCKET_RX_UNTIL_SUBFRAME_IND = PROCESS_RX_UNTIL_SUBFRAME_IND,
 MAC_SOCKET_START_PHY = PROCESS_START_PHY,
 MAX_SOCKET_CC_FUNC
#else
#ifdef DL_UL_SPLIT_TDD
 PDCP_SOCKET_PROCESS_RX = PDCP_PROCESS_RX,
 MAC_SOCKET_TX_DL_CONTROL_MSG = TX_DL_CONTROL_MSG,
 MAC_SOCKET_TX_DL_DATA_MSG = TX_DL_DATA_MSG,
 MAC_SOCKET_OAM_AND_RRC_MSG = PROCESS_DL_OAM_AND_RRC_MSG,
 MAC_SOCKET_RX_UNTIL_SUBFRAME_IND = PROCESS_RX_UNTIL_SUBFRAME_IND,
 MAC_SOCKET_SUB_FRAME_IND_UL = PROCESS_SUB_FRAME_IND_UL,
 MAC_SOCKET_WAIT_FOR_TDD_SUBFRAME_IND = WAIT_FOR_TDD_SUBFRAME_IND,
 MAC_SOCKET_START_PHY = PROCESS_START_PHY,
 MAC_SOCKET_NORMAL_MODE_RECV_FROM,
 MAX_SOCKET_CC_FUNC
#else
 MAC_SOCKET_RX_UNTIL_SUBFRAME_IND = MAC_PROCESS_RX_UNTIL_SUBFRAME_IND,
 PDCP_SOCKET_PROCESS_RX = PDCP_PROCESS_RX,
 MAC_SOCKET_TX_DL_CONTROL_MSG = MAC_TX_DL_CONTROL_MSG,
#ifndef DL_DATA_SEPARATION 
 MAC_SOCKET_TX_DL_DATA_MSG = MAC_TX_DL_DATA_MSG,
#else
 MAC_SOCKET_TX_DL_DATA_MSG = PROCESS_MAC_TX_DL_DATA_MSG,
#endif
 /*MAC_SOCKET_OAM_AND_RRC_MSG = MAC_PROCESS_OAM_AND_RRC_MSG,*/
 MAC_SOCKET_START_PHY = MAC_PROCESS_START_PHY,
 MAC_SOCKET_NORMAL_MODE_RECV_FROM = NORMAL_MODE_FOR_MAC_EXEC_SCH,
 MAX_SOCKET_CC_FUNC
#endif
#endif
}SocketFuncIdentifier;
#endif
/* MAC Changes Start */


typedef enum PdschAllocationFailureTypeT
{
    RETX_RB_NOT_AVAILABLE = 1,
    RB_NOT_AVAILABLE,
}PdschAllocationFailureType;

/* MAC Changes Start */
#ifdef RLC_QUEUE_STATS
typedef enum QueuePerUePerLcIdentifierT
{
UMD_RAW_SDU_Q,
AMD_RAW_SDU_Q,
MAX_UE_LC_QUEUE_COUNT
}QueuePerUePerLcIdentifier;

typedef enum QueueIdentifierT
{
MAC_UEDATA_IND_Q,
MAX_QUEUE_COUNT
}QueueIdentifier;
#endif

typedef struct UlUePRBTTIInfoT
{
   /* SPR 15909 fix start */
    tickType_t lastScheduledTick;        
    tickType_t currentTick;
     /* SPR 15909 fix end */
    UInt32 rbBitMap;          
    UInt32 ueQueueLoad;            
    UInt16 ueId;                   
    UInt8  numOfRb;
    UInt8  modulationScheme;       
    UInt8  uePriority;   
    UInt8  typeOfAllocation; 
    UInt8  startRBIndex;
    
}ulUePRBTTIInfo;


typedef struct DlUePRBTTIInfoT
{
     /* SPR 15909 fix start */
    tickType_t lastScheduledTick;        
    tickType_t currentTick;       
     /* SPR 15909 fix end */
    UInt32 rbBitMap;          
    UInt32 ueQueueLoad;            
    UInt16 ueId;                   
    UInt8  numOfRb;
    UInt8  modulationSchemeCodeword0;       
    UInt8  modulationSchemeCodeword1;       
    UInt8  uePriority;   
    UInt8  typeOfAllocation; 
    UInt8  numberOfTBs;
    UInt8  transmissionMode;
    UInt8  dciFormat;
    UInt8  aggregationLevel;
    UInt8  raPolicy;
    
}dlUePRBTTIInfo;

typedef struct ULStatsInfoRBT
{
    UInt8 currentRBIndex;
    ulUePRBTTIInfo rbAllocPerTTI[MAX_RESOURCE_BLOCK];
}ulStatsInfoRB;

typedef struct ULStatsInfoT
{
    UInt8 containerStatus;
    ulStatsInfoRB statsULInfoRB[MAX_STATS_TICKS];
}ulStatsInfo;

typedef struct DLStatsInfoRBT
{
    UInt8 currentRBIndex;
    dlUePRBTTIInfo rbAllocPerTTI[MAX_RESOURCE_BLOCK];
}dlStatsInfoRB;

typedef struct DLStatsInfoT
{
    UInt8 containerStatus;
    dlStatsInfoRB statsDLInfoRB[MAX_STATS_TICKS];
}dlStatsInfo;

typedef struct statsFileQueueNodeT
{
    LTE_SNODE statsFileQueueNodeAnchor;
    UInt8     statsArrIndex;
    /*SPR 15909 fix start*/
    tickType_t    statsTickIndex;
    /*SPR 15909 fix end*/
}statsFileQueueNode;
/* Structure to store I/O stats */
typedef struct LteMacIoStatsT
{
    UInt32              numMsgs;  /* Number of IO messages */
    unsigned long long  numBytes; /* number of IO bytes */
#ifdef DL_UL_SPLIT_TDD
    UInt32              numUlMsgs;  /* Number of IO messages */
    unsigned long long  numUlBytes; /* number of IO bytes */
#endif
} LteMacIoStats;

/* typedef to store TX stats */
typedef LteMacIoStats   LteMacTxStats;

/* typedef to store RX stats */
typedef LteMacIoStats   LteMacRxStats;

/* Structure to store Interface stats */
typedef struct LteMacInfStatsT
{
    /* TX stats */
    LteMacTxStats       txStats;   /* TX stats for this INF */

    /* RX stats */
    LteMacRxStats       rxStats;   /* RX stats for this INF */
}LteMacInfStats;

typedef struct LteMacPhyRfPerTickStatsT
{
    UInt32              msgTx;
    UInt32              msgRx;
    UInt8               numTb;
    UInt8               mcs;
    UInt16              dlSize;
    UInt16              numTxPdu;
    UInt16              ulSize;

#ifdef TDD_CONFIG
#ifdef DL_UL_SPLIT_TDD
    UInt32              macCyclesPerTick[MAX_EXECUTION_LEGS];
    UInt32              pdcpCyclesPerTick[MAX_PDCP_CC_FUNC];
#ifdef GETSTATS_EL_CLOCKTIME
    UInt32              macFuncClkTimeInMicroSecPerTick[MAX_EXECUTION_LEGS];
    UInt32              pdcpFuncClkTimeInMicroSecPerTick[MAX_PDCP_CC_FUNC];
#endif
#else
    UInt32		legCyclesPerTick[MAX_LEG_CC_FUNC];
    UInt32		legFuncClkTimeInMicroSecPerTick[MAX_LEG_CC_FUNC];
#endif
#else
#ifdef DL_UL_SPLIT
    UInt32              macCyclesPerTick[MAX_EXECUTION_LEGS];
    UInt32              macFuncClkTimeInMicroSecPerTick[MAX_EXECUTION_LEGS];
#else
    UInt32              macCyclesPerTick[MAX_MAC_CC_FUNC];
    UInt32              macFuncClkTimeInMicroSecPerTick[MAX_MAC_CC_FUNC];
#endif
    UInt32              rlcCyclesPerTick[MAX_RLC_CC_FUNC];
    UInt32              rlcFuncClkTimeInMicroSecPerTick[MAX_RLC_CC_FUNC];
    UInt32              pdcpCyclesPerTick[MAX_PDCP_CC_FUNC];
    UInt32              pdcpFuncClkTimeInMicroSecPerTick[MAX_PDCP_CC_FUNC];
#endif
    unsigned long long           processingTime; /* +- SPR 18490 */

}LteMacPhyRfPerTickStats;

typedef struct LteMacPhyRadioFrameStatsT
{
    LteMacPhyRfPerTickStats     ttiStats[LTE_MAC_STATS_MAX_SFN][MAX_SUB_FRAME];
}LteMacPhyRadioFrameStats;

typedef struct LteMacPhyFapiParReqStatsT
{
    UInt32      numMsgs;
}LteMacPhyFapiParReqStats;

typedef struct LteMacPhyFapiCellConfReqStatsT
{
    UInt32      numMsgs;
    UInt32      numTlvs;
}LteMacPhyFapiCellConfReqStats;

typedef struct LteMacPhyFapiStartReqStatsT
{
    UInt32      numMsgs;
}LteMacPhyFapiStartReqStats;

typedef struct LteMacPhyFapiStopReqStatsT
{
    UInt32      numMsgs;
}LteMacPhyFapiStopReqStats;

typedef struct LteMacPhyFapiUeConfReqStatsT
{
    UInt32      numMsgs;
    UInt32      numTlvs;
    UInt64      numBytes;
}LteMacPhyFapiUeConfReqStats;

typedef struct LteMacPhyFapiDlConfDciStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiDlConfDciStats;

typedef struct LteMacPhyFapiDlConfBchStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiDlConfBchStats;

typedef struct LteMacPhyFapiDlConfMchStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiDlConfMchStats;

typedef struct LteMacPhyFapiDlConfDlschStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiDlConfDlschStats;

typedef struct LteMacPhyFapiDlConfPchStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiDlConfPchStats;

typedef struct LteMacPhyFapiDlConfReqStatsT
{
    UInt32      numMsgs;
    UInt64      numBytes;
    UInt32      numDci;
    UInt32      numPdu;
    LteMacPhyFapiDlConfDciStats     dciStats;
    LteMacPhyFapiDlConfBchStats     bchStats;
    LteMacPhyFapiDlConfMchStats     mchStats;
    LteMacPhyFapiDlConfDlschStats   dlschStats;
    LteMacPhyFapiDlConfPchStats     pchStats;
}LteMacPhyFapiDlConfReqStats;

typedef struct LteMacPhyFapiUlConfUlschStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiUlConfUlschStats;

typedef struct LteMacPhyFapiUlConfUlschCqiRiStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiUlConfUlschCqiRiStats;

typedef struct LteMacPhyFapiUlConfUlschHarqStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiUlConfUlschHarqStats;

typedef struct LteMacPhyFapiUlConfUlschCqiHarqRiStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiUlConfUlschCqiHarqRiStats;

typedef struct LteMacPhyFapiUlConfUciCqiStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiUlConfUciCqiStats;

typedef struct LteMacPhyFapiUlConfUciSrStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiUlConfUciSrStats;

typedef struct LteMacPhyFapiUlConfUciHarqStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiUlConfUciHarqStats;

typedef struct LteMacPhyFapiUlConfUciSrHarqStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiUlConfUciSrHarqStats;

typedef struct LteMacPhyFapiUlConfUciCqiHarqStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiUlConfUciCqiHarqStats;

typedef struct LteMacPhyFapiUlConfUciCqiSrStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiUlConfUciCqiSrStats;

typedef struct LteMacPhyFapiUlConfUciCqiSrHarqStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiUlConfUciCqiSrHarqStats;

typedef struct LteMacPhyFapiUlConfSrsStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiUlConfSrsStats;

typedef struct LteMacPhyFapiUlConfReqStatsT
{
    UInt32      numMsgs;
    UInt64      numBytes;
    UInt32      numPdu;
    LteMacPhyFapiUlConfUlschStats       ulschStats;
    LteMacPhyFapiUlConfUlschCqiRiStats  ulschCqiRiStats;
    LteMacPhyFapiUlConfUlschHarqStats   ulschHarqStats;
    LteMacPhyFapiUlConfUlschCqiHarqRiStats  ulschCqiHarqRiStats;
    LteMacPhyFapiUlConfUciCqiStats      uciCqiStats;
    LteMacPhyFapiUlConfUciSrStats       uciSrStats;
    LteMacPhyFapiUlConfUciHarqStats     uciHarqStats;
    LteMacPhyFapiUlConfUciSrHarqStats   uciSrHarqStats;
    LteMacPhyFapiUlConfUciCqiHarqStats  uciCqiHarqStats;
    LteMacPhyFapiUlConfUciCqiSrStats    uciCqiSrStats;
    LteMacPhyFapiUlConfUciCqiSrHarqStats    uciCqiSrHarqStats;
    LteMacPhyFapiUlConfSrsStats         srsStats;
}LteMacPhyFapiUlConfReqStats;

typedef struct LteMacPhyFapiHiDci0ReqStatsT
{
    UInt32      numMsgs;
    UInt32      numHi;
    UInt32      numDci0;
}LteMacPhyFapiHiDci0ReqStats;

typedef struct LteMacPhyFapiTxReqDlschStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiTxReqDlschStats;

typedef struct LteMacPhyFapiTxReqBchStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiTxReqBchStats;

typedef struct LteMacPhyFapiTxReqMchStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiTxReqMchStats;

typedef struct LteMacPhyFapiTxReqPchStatsT
{
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiTxReqPchStats;

typedef struct LteMacPhyFapiTxReqStatsT
{
    UInt32      numMsgs;
    UInt32      numPdu;
    LteMacPhyFapiTxReqDlschStats        dlschStats;
    LteMacPhyFapiTxReqBchStats          bchStats;
    LteMacPhyFapiTxReqMchStats          mchStats;
    LteMacPhyFapiTxReqPchStats          pchStats;
}LteMacPhyFapiTxReqStats;

typedef struct LteMacFapiTxStatsT
{
    LteMacPhyFapiParReqStats        parReqStats;
    LteMacPhyFapiCellConfReqStats   cellConfReqStats;
    LteMacPhyFapiStartReqStats      startReqStats;
    LteMacPhyFapiStopReqStats       stopReqStats;
    LteMacPhyFapiUeConfReqStats     ueConfReqStats;
    LteMacPhyFapiDlConfReqStats     dlConfReqStats;
    LteMacPhyFapiUlConfReqStats     ulConfReqStats;
    LteMacPhyFapiHiDci0ReqStats     hiDci0ReqStats;
    LteMacPhyFapiTxReqStats         txReqStats;
}LteMacFapiTxStats;

typedef struct LteMacPhyFapiParRespStatsT
{
    UInt32      numMsgs;
    UInt32      numTlvs;
}LteMacPhyFapiParRespStats;

typedef struct LteMacPhyFapiCellConfRespStatsT
{
    UInt32      numOk;
    UInt32      numFail;
    UInt32      numInvalidTlvs;
    UInt32      numMissingTlvs;
}LteMacPhyFapiCellConfRespStats;

typedef struct LteMacPhyFapiStopIndStatsT
{
    UInt32      numMsgs;
}LteMacPhyFapiStopIndStats;

typedef struct LteMacPhyFapiUeConfRespStatsT
{
    UInt32      numOk;
    UInt32      numFail;
    UInt32      numInvalidTlvs;
    UInt32      numMissingTlvs;
    UInt64      numBytes;
}LteMacPhyFapiUeConfRespStats;

typedef struct LteMacPhyFapiErrIndStatsT
{
    UInt32      numMsgs;
    UInt64      numBytes;
}LteMacPhyFapiErrIndStats;

typedef struct LteMacPhyFapiSfIndStatsT 
{
    UInt64      numMsgs;
}LteMacPhyFapiSfIndStats;

typedef struct LteMacPhyFapiHarqIndStatsT
{
    UInt32      numMsgs;
    UInt32      numHarqs;
    UInt32      numAck;
    UInt32      numNack;
}LteMacPhyFapiHarqIndStats;

typedef struct LteMacPhyFapiCrcIndStatsT
{
    UInt32      numMsgs;
    UInt32      numOk;
    UInt32      numErr;
}LteMacPhyFapiCrcIndStats;

typedef struct LteMacPhyFapiRxUlschStatsT
{
    UInt32      numMsgs;
    UInt32      numPdu;
    UInt64      numBytes;
}LteMacPhyFapiRxUlschStats;

typedef struct LteMacPhyFapiRachIndStatsT
{
    UInt32      numMsgs;
    UInt32      numPreambles;
}LteMacPhyFapiRachIndStats;

typedef struct LteMacPhyFapiSrsIndStatsT
{
    UInt32      numMsgs;
    UInt64      numBytes;
}LteMacPhyFapiSrsIndStats;

typedef struct LteMacPhyFapiRxSrIndStatsT
{
    UInt32      numMsgs;
    UInt32      numSr;
}LteMacPhyFapiRxSrIndStats;

typedef struct LteMacPhyFapiCqiIndStatsT
{
    UInt32      numMsgs;
    UInt32      numCqi;
    UInt64      numBytes;
}LteMacPhyFapiCqiIndStats;


typedef struct LteMacFapiRxStatsT
{
    LteMacPhyFapiParRespStats       parRespStats;
    LteMacPhyFapiCellConfRespStats  cellConfRespStats;
    LteMacPhyFapiStopIndStats       stopIndStats;
    LteMacPhyFapiUeConfRespStats    ueConfRespStats;
    LteMacPhyFapiErrIndStats        errIndStats;
    LteMacPhyFapiSfIndStats         sfIndStats;
    LteMacPhyFapiHarqIndStats       harqIndStats;
    LteMacPhyFapiCrcIndStats        crcIndStats;
    LteMacPhyFapiRxUlschStats       rxUlschStats;
    LteMacPhyFapiRachIndStats       rachIndStats;
    LteMacPhyFapiSrsIndStats        srsIndStats;
    LteMacPhyFapiRxSrIndStats       rxSrIndStats;
    LteMacPhyFapiCqiIndStats        cqiIndStats;
}LteMacFapiRxStats;

/* typedef to store MAC-PHY interface stats */
typedef struct LteMacPhyInfStatsT
{
    /* TX stats */
    LteMacTxStats       txStats;   /* TX stats for this INF */

    /* RX stats */
    LteMacRxStats       rxStats;   /* RX stats for this INF */

    /* FAPI stats */
    LteMacFapiTxStats   fapiTxStats; /* Tx stats for FAPI msgs */

    LteMacFapiRxStats   fapiRxStats; /* Rx stats for FAPI msgs */

    /* Radio frame for MAC-PHY interface */
    LteMacPhyRadioFrameStats    radioFrameStats;
}LteMacPhyInfStats;

/* typedef to store MAC-RLC interface stats */
typedef struct  LteMacRlcInfStatsT
{
    /* TX stats */
    LteMacTxStats       txStats;   /* TX stats for this INF */

    /* RX stats */
    LteMacRxStats       rxStats;   /* RX stats for this INF */

    /* Number of HARQ failures*/
    UInt32              numHarqFailures;
} LteMacRlcInfStats;

/* typedef to store MAC-RRC interface stats */
typedef LteMacInfStats  LteMacRrcInfStats;

typedef struct LteMacPerfStatsT
{
    /* Time stamp of the last received sub frame indication */
    TimeSpec     lastSfiTimeStamp;

    /* Array containing last SFI_PROFILE_COUNT processing times */
    unsigned long long           tickTime[SFI_PROFILE_COUNT];/* +- SPR 18490 */

    /* iterator for filling processingTime */
    /*SPR 15909 fix start*/
    UInt64              tickTimeIt;
    /*SPR 15909 fix end*/

    /* Time stamp of the last received sub frame indication */
    TimeSpec     processingStartTime;

    /* Array containing last SFI_PROFILE_COUNT processing times */
    /* +- SPR 18490 */
    unsigned long long           processingTime[SFI_PROFILE_COUNT];
    unsigned long long           highestTime[PERF_STATS_TIME];
    unsigned long long           totalProcessingTime[PERF_STATS_TIME];
    /* +- SPR 18490 */

    /* iterator for filling current SFI processingTime */
    UInt32              processingTimeIt;
    /* iterator for filling Cummulative processing Time Sample*/
    UInt32              processingTimeCumIt;

    /* Number of times processing time was more than 1 ms */
    long long numProcessingOvershoots;


    /* Number of times processing time was in these range */
	long long              countBw0_499;
	long long              countBw500_799;
	long long              countBw800_999;

    /* Number of times processing time was more than 1 ms */
    UInt32              numTickOvershoots;

    /* Number of total ticks received */
    long long           numTicks;
    /* Number of total ticks received */
    long long           numProcessings;
#ifdef UTFWK_SIMULATION
    long long  pdbDelay0_25;
    long long  pdbDelay26_75;
    long long  pdbDelay76_125;
    long long  pdbDelay126_275;
    long long  pdbDelayAbove275;
    long long  pdbDelayFail;
    long long  aggrgtPdbDelay;
    UInt64     highestDelay;
    long long pdbDelayPerQci[10][5];
 /* QOS_PDB_CHANGES*/
    long long pdbUlDelayPerQci[10][5];
 /* QOS_PDB_CHANGES*/
    /* Parameters for GBR throughput Factor*/
    long long  gbrTputFactor_90_100;
    long long  gbrTputFactor_0_75;
    long long  gbrTputFactor_100_125;
    long long  gbrTputFactor_75_90;
    long long  gbrTputFactor_125;
    long long  totalNumberOfSamples;

#endif
}LteMacPerfStats;

/*spr 22474 start*/
    /*CSPL Code removed*/
/*spr 22474 end*/


#ifdef CYCLE_COUNT_MAC_RLC_PDCP
typedef struct LteCycleCountStatsT 
{
  UInt64   totalFuncCycleCount;
  UInt64   numCycles;
  UInt64   totalFuncClkTimeInMicroSec;
}LteCycleCountStats;

#if (defined(TDD_CONFIG) && !defined(DL_UL_SPLIT_TDD))
typedef struct Ltelayer2CycleCountStatsT
{
 UInt64                   totalL2Cycles;
 LteCycleCountStats       legCCStats[MAX_LEG_CC_FUNC];
#ifdef DL_DATA_SEPARATION
 LteCycleCountStats       encoderCCStats[MAX_ENC_CC_FUNC];
#endif
}Ltelayer2CycleCountStats;
#endif

#if (defined(FDD_CONFIG) || defined(DL_UL_SPLIT_TDD))
typedef struct Ltelayer2CycleCountStatsT
{
 UInt64                   totalL2Cycles;
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
 LteCycleCountStats       cycleCountStats[MAX_EXECUTION_LEGS];
#else
 LteCycleCountStats       macCCStats[MAX_MAC_CC_FUNC];
 LteCycleCountStats       rlcCCStats[MAX_RLC_CC_FUNC];
/* SPR 8712 changes */
#endif
#ifdef DL_DATA_SEPARATION
 LteCycleCountStats       encoderCCStats[MAX_ENC_CC_FUNC];
#endif
/* SPR 8712 changes */
 LteCycleCountStats       pdcpCCStats[MAX_PDCP_CC_FUNC];
 LteCycleCountStats       socketCCStats[MAX_SOCKET_CC_FUNC];
}Ltelayer2CycleCountStats;
#endif
#endif

/* UL_MU_MIMO_CHG_START */
typedef enum BuddyPairFailureCaseT
{
    STRICT_ALLOC_FAILURE = 0,
    NDMRS_PAIR_FAILURE,
    POWER_CHECK_FAILURE,
    AVGMCS_DIFF_THRESOLD_FAILURE,
    FSS_TO_NONFSS_TRANSITION,
    SHARED_RB_EXCEED_TOTAL_RB_REQ,
    MAX_FAILURE_CAUSE
}BuddyPairFailureCase;
typedef struct MuMimoStatsPerUeT
{
    UInt8 userLocType;/*0 - cell center , 1 - cell edge  */
    UInt8 ulMUMIMOStat;/*1 enabled , 0 disabled*/
    UInt8 nDmrsValue;/*vaild values 0-7 , invalid value 8*/
    UInt8 modScheme;
    UInt16 buddyIndex;/*Ue index of the buddy pair  Ue */
}MuMimoStatsPerUe;
/* UL_MU_MIMO_CHG_END */

/* ULA_CHG*/
typedef struct LteMacPuschPerTTIPerUEStatsT
{
    UInt16 ueIndex;
    UInt8 transmissionType; /* New Tx/Adaptive Retx/Non-Adaptive Retx */
    UInt8 harqProcessId;
    UInt8 allocatedPRBs;
    UInt8 startRBPosition;
    UInt8 ulGrantType;  /* SR/CR/CQI WIthout Data/CQI WIth Data */
    UInt8 frequencySeletiveScheduling;
    UInt32 oppGranted;    /* Number of bytes */
    UInt8 selectedMCS;
    UInt8 allocationFailureCause;
    SInt8 tpc;
    /* SPS_CHG */
    UInt8   allocatedSPSRBs;
    UInt8   startSPSVRBIndex;
    UInt8   allocatedSPSMcs;
    UInt16  spsResourceAllocationBitMap;
    /* SPS_CHG */
    /* ATB stats Changes*/
    SInt8 currentUePower;
    SInt8 maxQciDeltaPowerMask;
    SInt8 maxPower;
    UInt8 inputMCSForATB;
    UInt8 inputRBForATB;
    SInt8 currPowerCorrectionForPusch;
    SInt16 pathLoss;
     /* UL_MU_MIMO_CHG_START */
     UInt8 ulMUMIMOStatus;/*1 enabled , 0 disabled*/
     UInt8 userLocationType;/*0 - cell center , 1 - cell edge  */
     UInt8 nDmrs;/*vaild values 0-7 , invalid value 8*/
     UInt8 modulationScheme;
     UInt16 buddyUeIndex;/*Ue index of the buddy pair  Ue */
     /* UL_MU_MIMO_CHG_END */
}LteMacPuschPerTTIPerUEStats;

// These stats need to be reset every 'MAX_TTI_STATS' TTIs
typedef struct LteMacPuschTTIStatsT
{
    UInt8 isPuschTTIStatsValid; 
    UInt64 absoluteTTI;
    UInt8 numScheduledUERetx;
    UInt8 numScheduledUENewTx;
    /* Count of the UE Scehduled. Should be reset to 0 at the start of TTI */
    UInt8 numScheduledUETotal;
    /* UL_MU_MIMO_CHG_START */
    UInt8 totalRbShared;/*total num of rb's shared by buddy pairs */
    UInt8 totalBuddyPairSuccess;/*total num of successful buddy pairs */
    UInt8 ulMuMimoTotalUeRetx;/*total num of mu-mimo scheduled UE's retransmission */
    UInt8 buddyPairFailurePerCases[MAX_FAILURE_CAUSE/*MAX_FAILURE_CAUSE = 6*/];/*buddy pair failure count per cases */
    /* UL_MU_MIMO_CHG_END */
    LteMacPuschPerTTIPerUEStats
        puschStatsPerScheduledUE[50/*MAX_UL_UE_SCHEDULED+10*/];
} LteMacPuschTTIStats;

typedef struct UEDRXStatsInfoT
{
    /* Active time of the UE in last 50 long DRX Cycles */
    UInt32 lastActiveTimeUeDRX;
    /* Active time of the UE in last .longDRXCycleElapsed. long DRX Cycles */
    UInt32 currentActiveTimeofUeDRX; 
    UInt32 longDRXCyclesElapsed;
}UEDRXStatsInfo;

typedef struct UEScheduledTTInfoT
{
    /* UE offset position in to the puschStatsPerScheduledUE Array 
     * at which cheduled information is present */
    UInt16 ueIndexPos; 
    /* Absolute TTI value at which UE is scheduled */
    UInt64 absoluteTTI;
}UEScheduledTTIInfo;

#ifdef UTFWK_SIMULATION
typedef enum eTxTypeT
{
    MAC_QOS_RETX = 0,
    MAC_QOS_TX,
    MAC_QOS_TX_WITH_TA,
    MAC_QOS_TA,
    MAC_QOS_NO_TX
}eTxType;

typedef enum eUeEgblStateT
{
    MAC_QOS_UE_EGBL_STATE = 0,
    MAC_QOS_DRX,
    MAC_QOS_MEAS_GAP,
    MAC_QOS_UL_SYNC_LOSS
}eUeEgblState;
#endif
typedef struct LcgInfoT
{
    UInt32 lcgLoad;
    UInt32 qLoadeNB;
}LcgInfo;

typedef struct QosLCGOerTTIStatsUlT
{
    LcgInfo lcgInfo[MAX_TTI_STATS];
}
QosLCGOerTTIStatsUl;
typedef struct STXTUL
{
    UInt32 qLoad;
    UInt32 ulLcGrant;
    UInt32 availLcToken;
    UInt32 ulLcWt;
    UInt32 headOfLinePktDelay;
    UInt32 totSchdTknInWindow;
    UInt32 expectedGrantT;
    UInt32 pad;
    UInt8  qLoadPriority;
    UInt8  pdbPriority;
    UInt8  tokenPriority;
    /*QOS_PKT_DROP_CHANGES*/
    UInt64 numULMissPerTick;
    /*QOS_PKT_DROP_CHANGES*/
}STXUL;


typedef struct QoSLCPerTTIStatULT
{
    UInt32 TotalLCOppGranted ;
    UInt64 numULMiss;
    UInt64 numULPkts;
    STXUL STX[MAX_TTI_STATS];
}QoSLCPerTTIStatsUL;

#ifdef SSI_DEBUG_STATS
typedef struct STXT
{
    UInt32    qLoad;
    UInt32    OppGranted;
    SInt32    avaiableToken;
    UInt16    LCWeight;
    UInt8     qciPriority;
    UInt8     pdbPriority;
    UInt8     qLoadPriority;
    UInt8     tokenPriority;
    UInt32    headOfLinePktDelay;
    UInt32    totSchdTknInWindow;
    UInt64    numDLMissPerTick;
}STX1;

typedef struct QoSLCPerTTIStatT
{
    UInt32 TotalLCOppGranted ;
#ifdef UTFWK_SIMULATION
    UInt64 numDLPkts;
#endif
    UInt64     numDLMiss;
    UInt32     expGrntT;
    STX1       STX[MAX_TTI_STATS];
}QoSLCPerTTIStat;

/* Following structure contains per UE per tick SPS information */
typedef struct SpsPerUePerTTIStatsT
{
    UInt8   isSpsPerUePerTTIStatsValid; 
    UInt8   spsLcId;
    UInt8   spsState;
    UInt8   cause;
    UInt8   schType;
    UInt8   currMcs;
    UInt8   spsMcs;
    UInt8   harqProcessId;
    /* Number of Empty transmissions in SPS occasion */
    UInt8   numOfSpsEmptyTx;
    UInt8   numRBs;
    /* Index used for Activation/Deactivation/Reactivation */
    UInt16  ttiIndex;
    /* Number of DCI Attempts made by UE */
    UInt16 numDciAttempts;
    UInt32  spsQload;
    /* Absolute TTI stored */
    UInt64 absoluteTTI;
} SpsPerUePerTTIStats, *LP_SpsPerUePerTTIStats ;

typedef struct SpsPerUePerTTIStatsULT
{
    UInt8   isSpsPerUePerTTIStatsULValid; 
    UInt8   spsLcId;
    UInt8   spsState;
    UInt8   cause;
    UInt8   schType;
    UInt8   currMcs;
    UInt8   spsMcs;
    UInt8   harqProcessId;
    /* Number of sure Empty transmissions in SPS occasion */
    UInt8  numOfSureSpsEmptyTx;
    /* Number of unsure Empty transmissions in SPS occasion in case of max harq retx*/
    UInt8  numOfUnsureSpsEmptyTx;
    /* Number of Empty transmissions in SPS occasion */
    UInt8   numRBs;
    /* Index used for Activation/Deactivation/Reactivation */
    UInt16  ttiIndex;
    /* Number of DCI Attempts made by UE */
    UInt16 numDciAttempts;
    UInt32  spsUlLcgQload;
    /* Absolute TTI stored */
    UInt64 absoluteTTI;
    UInt32 avgRlcSduSize;
} SpsPerUePerTTIStatsUL, *LP_SpsPerUePerTTIStatsUL ;

typedef struct SpsPerUeHarqPerTTIStatsT
{
    UInt8 isSpsPerUeHarqPerTTIStatsValid; 
    /* Number of SPS Harq Process Used for New TX*/
    UInt8 numOfBusySpsHarqProcessForNewTx;
    /* Number of SPS Harq Process Used for Re TX*/
    UInt8 numOfBusySpsHarqProcessForReTx;
    /* Number of DRA Harq Process Used for New TX*/
    UInt8 numOfBusyDRAHarqProcessForNewTx;
    /* Number of DRA Harq Process Used for RE TX*/
    UInt8 numOfBusyDRAHarqProcessForReTx;
    /* Absolute TTI stored */
    /*SPR 15909 fix start*/
    UInt64 absoluteTTI;
    /*SPR 15909 fix end*/
     
} *LP_SpsPerUeHarqPerTTIStats, SpsPerUeHarqPerTTIStats ;

typedef struct SpsDebugPerUePerTTIStatsT
{
   UInt8 isSpsDebugPerUePerTTIStatsValid;
   /*SPR 15909 fix start*/
   UInt64 absoluteTTI; 
   /*SPR 15909 fix end*/
   UInt8 harqProcessId;
   UInt8 ackNackReceived; 
   UInt8 current_TX_NB;
    
} *LP_SpsDebugPerUePerTTIStats, SpsDebugPerUePerTTIStats;
typedef struct SpsDebugPerUePerTTIStatsULT
{
   UInt8 isSpsDebugPerUePerTTIStatsULValid;
   /*SPR 15909 fix start*/
   UInt64 absoluteTTI; 
   /*SPR 15909 fix end*/
   UInt8 harqProcessId;
   UInt8 schUsed;
   UInt8 crcReceived; 
   UInt8 current_TX_NB;
    
} *LP_SpsDebugPerUePerTTIStatsUL, SpsDebugPerUePerTTIStatsUL;
typedef struct QoSUELCStatT
{
#ifdef UTFWK_SIMULATION
    UInt64 TotalUeOPPGranted; 
#endif
    UInt64 BsrNet[MAX_TTI_STATS];
    QosLCGOerTTIStatsUl qoSLCGPerTTIStatsUl[MAX_LC_CNT];
    QoSLCPerTTIStatsUL qoSLCPerTTIStatsUl[MAX_LCID];
    QoSLCPerTTIStat       qoSLCPerTTIStat[MAX_LCID];
    UInt64 grant[MAX_TTI_STATS];
    UInt64 totUlQload[MAX_TTI_STATS];
    UInt8 harqId[MAX_TTI_STATS];
    UInt8 mcsIndexCodeWordOne[MAX_TTI_STATS];
    UInt16                dlTBSize[MAX_TTI_STATS];
    UInt8  mcs; //Stored for UL Stats

    /* SPS_CHG*/
    SpsPerUePerTTIStats  spsTTIStats[MAX_TTI_STATS];
    SpsPerUePerTTIStatsUL  spsTTIStatsUl[MAX_TTI_STATS];
    SpsPerUeHarqPerTTIStats spsPerUeHarqPerTTIStats[MAX_TTI_STATS];
    SpsDebugPerUePerTTIStats spsDebugPerUePerTTIStats[MAX_TTI_STATS];
    SpsDebugPerUePerTTIStatsUL spsDebugPerUePerTTIStatsUl[MAX_TTI_STATS];
    /* SPS_CHG*/
}QoSUELCStat;

/* Following structure defines the DL interval profile stats */
typedef struct SpsDlIntProfileStatsT
{
    UInt8  dlMinMcsSps;
    UInt8  dlMcsMargin;
    UInt8  dlJitterHandlingOption;
    UInt8  rlcSduThresForDra;
    UInt16 dlTbsizeMarginSps;
} SpsDlIntProfileStats ;

/* Following structure defines the UL interval profile stats */
typedef struct SpsUlIntProfileStatsT
{
    UInt8  ulMinMcsSps;
    UInt8  ulMcsMargin;
    UInt16 ulTbSizeMarginSps;
} SpsUlIntProfileStats ;

/* Following structure defines the Common interval profile stats */
typedef struct SpsCommIntProfileStatsT
{
    UInt8 overrideSpsOccassionForOtherUe;
    UInt8 strictSpsResourceUsageForSpsLc;
    UInt8 interPktDelayForSilenceDetection;
    UInt8 enableSilenceDetection;
    UInt32 maxPdcpPduSizeIpv4[MAX_ROHC_PROFILE_SUPP];
    UInt32 maxPdcpPduSizeIpv6[MAX_ROHC_PROFILE_SUPP];
    UInt8  minRbSavedReactivationThres;
    UInt8  spsBlerThres;
    UInt8 auditInterval;
    UInt8 spsResAllocTtiAdv;
} SpsCommIntProfileStats;

/* Structure for SPS DL Scheduling Info per TTI Stats */
typedef struct SpsDlSchInfoPerTtiStatsT
{
    UInt32 maxDlSpsDciPerTti;
    UInt32 maxDlSpsOccasionPerTti;
    UInt32 maxDlSpsRbsPerTti;
    UInt32 maxDlSpsRbsPerTtiPerInterval[MAX_SPS_INTERVALS];
}SpsDlSchInfoPerTtiStats;

/* Structure for SPS UL Scheduling Info per TTI Stats */
typedef struct SpsUlSchInfoPerTtiStatsT
{
    UInt32 maxUlSpsDciPerTti;
    UInt32 maxUlSpsOccasionPerTti;
    UInt32 maxUlSpsRbsPerTti;
    UInt32 maxUlSpsRbsPerTtiPerInterval[MAX_SPS_INTERVALS];
}SpsUlSchInfoPerTtiStats;

typedef enum SpsQNameT
{
    DL_SPS_STATUS_Q      = 0,
    DL_SPS_DEACT_Q       = 1, 
    DL_SPS_ACT_Q         = 2,
    DL_SPS_REACT_Q       = 3,
    DL_SPS_UE_EXCLUDE    = 4,
    DL_SPS_RES_RSV_REL_Q = 5,
    UL_SPS_STATUS_Q      = 0,
    UL_SPS_DEACT_Q       = 1, 
    UL_SPS_ACT_Q         = 2,
    UL_SPS_REACT_Q       = 3,
    UL_SPS_UE_EXCLUDE    = 4,
    UL_SPS_RES_RSV_REL_Q = 5

} SpsQName ;
typedef struct LteSpsQueuesUsedT
{
    UInt8  qName;
    UInt32 qCount;
}LteSpsQueuesUsed, *LP_LteSpsQueuesUsed ;

/* Following structure contains per UE and per LC stats */
typedef struct QoSUEAndPerLCTTIStatT
{
#ifdef UTFWK_SIMULATION
    UInt8 ueTmMode;
    UInt64 lastTick;
    UInt64 currentTick;
    UInt64 numTotDLPkts;
    UInt64 numTotDLPktsInSys;
    UInt64 numDLMissInSys;
    UInt64 numTotULPkts;
    UInt64 numTotULPktsInSys;
    UInt64 numULMissInSys;
#endif
    /* For DL */
    UInt16 tknPeriodicity;
    UInt16 noOfDLSchUE;
    UInt8 pdbWt;
    UInt8 qciWt;
    UInt8 tknWt;
    UInt8 qLoadWt;
    UInt8 mcsWt;

    /* For UL */
    UInt16 ulTknPeriodicity;
    UInt16 noOfULSchUE;
    UInt8 ulPdbWt;
    UInt8 ulQciWt;
    UInt8 ulTknWt;
    UInt8 ulQLoadWt;
    UInt8 ulMcsWt;

    /* system wide SPS stats */
    UInt8  enableSps;
    UInt8  maxSpsDciAttempt;
    UInt8  qciSpslist[MAX_QCI] ;
    UInt8  supportedSpsDlIntervalList[TOTAL_SPS_INTERVAL];
    UInt8  supportedSpsUlIntervalList[TOTAL_SPS_INTERVAL];

    /* DL SPS stats - interval profile */
    SpsDlIntProfileStats spsDlIntProf[TOTAL_SPS_INTERVAL];

    /* UL SPS stats - interval profile */
    SpsUlIntProfileStats spsUlIntProf[TOTAL_SPS_INTERVAL];

    /* Common SPS stats - interval profile */
    SpsCommIntProfileStats spsCommIntProf[TOTAL_SPS_INTERVAL];
 
    /* spsDlSchInfoPerTti STATS */
    SpsDlSchInfoPerTtiStats spsDlSchInfoPerTtiStats;
    /* spsUlSchInfoPerTti STATS */
    SpsUlSchInfoPerTtiStats spsUlSchInfoPerTtiStats;

   /* SPR 15909 fix start */
    UInt64 currentTickValue[MAX_TTI_STATS];
    /* SPR 15909 fix end */
    QoSUELCStat qoSUELCStat[MAX_UE_SUPPORTED];
}QoSUEAndPerLCTTIStat;

typedef struct LCScheduledTTIInfoT
{
    /* UE offset position in to the puschStatsPerScheduledUE Array 
     * at which cheduled information is present */
    UInt8  lcCurWriteIndex; 
    UInt16 lcIndexPos[MAX_LC_CNT]; 
    /* Absolute TTI value at which UE is scheduled */
    UInt64 absoluteTTI;
}LCScheduledTTIInfo;

typedef struct LCGScheduledTTIInfoT
{
    /* UE offset position in to the puschStatsPerScheduledUE Array 
     * at which cheduled information is present */
    UInt8  lcgCurWriteIndex; 
    UInt16 lcgIndexPos[MAX_LCG_COUNT]; 
    /* Absolute TTI value at which UE is scheduled */
    UInt64 absoluteTTI;
}LCGScheduledTTIInfo;

typedef struct DlLCInfoStatT
{
    UInt8 lcPos;
    UInt8 isValid;
    UInt8 qci;
    UInt8 lcMode;
    UInt8 lcId;
    UInt8 tokenPriority;
    UInt64 bitRate;
    UInt16 tokenOverflow;
    UInt16 tokenDepth;
    UInt16 tokenRate;
    UInt16 totalLCWeight;
    RlcAmStatus ctrlFlag;
}DlLCInfoStat;

typedef struct UlLCInfoStatT
{
    UInt8 lcPos;
    UInt8 isValid;
    UInt8 qci;
    UInt8 lcMode;
    UInt8 lcId;
    UInt8 lcgId;
    UInt64 bitRate;
    UInt16 tokenOverflow;
    UInt32 tokenDepth;
    UInt32 tokenRate;
    UInt8  tokenPriority;
    UInt16 totalLCWeight;
}UlLCInfoStat;

/* UE Specific Counters */
typedef struct LteUEInfoStatT
{
    UInt8 isUeActive;
    UInt16 rnti;
    UInt8 ueTxnMode;
    UInt8 ueCategory;
    UInt8 srbCount;
    UInt8 dlGbrLcCount;
    UInt8 ulGbrLcCount;
    UInt8 dlNonGbrLcCount;
    UInt8 ulNonGbrLcCount;
    UInt8 dlEligibleLcCount;
    UInt8 ulEligibleLcCount;
    UInt8 ulStrategyStatsIndex;
    /* L2_PERFORMANCE_OPT_CHANGES_START */    
    UInt32 ulAmbr;
    UInt32 dlAmbr;
    /* L2_PERFORMANCE_OPT_CHANGES_END */    
    UInt32 ulScheduledCount;
    UInt32 dlScheduledCount;
    UInt16 ulTreeLcCount;
    UInt16 dlTreeLcCount;
    UInt16 dlZeroNonZeroCount;
    UInt16 dlNonZeroZeroCount;
    UInt16 ulZeroNonZeroCount;
    UInt16 ulNonZeroZeroCount;
    UInt16 srbTokenCount;
    UInt16 dlDrxOnCount;
    UInt16 dlMeasGapOnCount;
    UInt16 dlSyncLossOnCount;
    UInt16 dlDrxOffCount;
    UInt16 dlMeasGapOffCount;
    UInt16 dlSyncLossOffCount;
    UInt16 ulDrxOnCount;
    UInt16 ulMeasGapOnCount;
    UInt16 ulSyncLossOnCount;
    UInt16 ulDrxOffCount;
    UInt16 ulMeasGapOffCount;
    UInt16 ulSyncLossOffCount;
    UInt32 dlNewTxnCount;
    UInt16 dlReTxnCount;
    UInt32 ulNewTxnCount;
    UInt16 ulReTxnCount;
    UInt16 dlStrategyFailCount;
    UInt16 ulStrategyFailCount;
    UInt16 pdcchFailCount;
    UInt16 pdschFailCount;
    UInt16 puschFailCount;
    UInt16 dlAmbrTokenOverflow;
    UInt16 ulAmbrTokenOverflow;
    UInt32 dlScheduleFlag;
    UInt32 ulScheduleFlag;
    /* +COVERITY 19410 */
    DlLCInfoStat dlLCInfoStat[MAX_LOGICAL_CHANNEL];
    UlLCInfoStat ulLCInfoStat[MAX_LOGICAL_CHANNEL];
    /* -COVERITY 19410 */
}LteUEInfoStat;

/* LC Stats */
/* DL LC Stats */
typedef struct LteQosDLStrategyLCTTIPerLcStatT
{
    UInt16 ueIndex;
    UInt8  lcId;
    UInt32 qLoad;
    UInt16 scheduledBytes;
    UInt16 resheduledBytes;
    UInt32 headOfLineDelay;
    UInt32 drxOnDurationDelay;
    UInt32 availToken;
    UInt16 totalLcWeight;
    UInt32 totTknSchedWndw;
}LteQosDLStrategyLCTTIPerLcStat;

typedef struct LteQosDLStrategyLCTTIStatT
{
    UInt64 absoluteTTI;
    UInt16 numScheduledLC;
    LteQosDLStrategyLCTTIPerLcStat dlStrategyPerLC[MAX_STRATEGY_LC_SCHEDULED];
}LteQosDLStrategyLCTTIStat;

/* UL LC Stats */
typedef struct LteQosULStrategyLCTTIPerLcStatT
{
    UInt16 ueIndex;
    UInt8  lcId;
    /* BSR Load when Scheduled */
    UInt16 BSRLoad;
    UInt16 qLoad; /* remaining Qload*/
    UInt16 scheduledBytes;
    UInt16 resheduledBytes;
    UInt32 headOfLineDelay;
    UInt32 drxOnDurationDelay;
    UInt32 availToken;
    UInt16 totalLcWeight;
    UInt32 totTknSchedWndw;
}LteQosULStrategyLCTTIPerLcStat;

typedef struct LteQosULStrategyLCTTIStatT
{
    UInt64 absoluteTTI;
    UInt16 numScheduledLC;
    LteQosULStrategyLCTTIPerLcStat ulStrategyPerLC[MAX_STRATEGY_LC_SCHEDULED];
}LteQosULStrategyLCTTIStat;

/* UL LC Stats */
typedef struct LteQosULStrategyLCGTTIPeStatT
{
    UInt16 BSRLoad;
    UInt16 scheduledBytes;
    UInt16 rescheduledBytes;
    UInt16 headOfLineDelay;
}LteQosULStrategyLCGTTIPerLcgStat;

typedef struct LteQosULStrategyLCGTTIStatT
{
    UInt64 absoluteTTI;
    UInt16 numScheduledLCG;
    LteQosULStrategyLCGTTIPerLcgStat ulStrategyPerLCG[MAX_STRATEGY_LCG_SCHEDULED];
}LteQosULStrategyLCGTTIStat;

/* UE Stats */
/* DL Strategy Stats */
typedef struct LteMacDlStrategyPerTTIPerUEStatsT
{
    UInt16  ueIndex;
    UInt16  minTBSize;
    UInt16  maxTBSize;
    UInt8   scheduledRB;
    UInt8   ueWbMCS;
    UInt8   recvdRI;
    UInt8   transmissionMode;
    DLTxnType transmissionType;/* New Tx/Adaptive Retx/Non-Adaptive Retx */
    UInt8   scheduleLcCount;
    UInt8   eligibleLcCount;
    UInt8   harqProcessId;
    UInt8   aperodicCQIFlag;
    UInt8   pendingTA;
    UInt8   pendingReTX;
    UInt8   isTBOneValid;
    UInt8   isTBTwoValid;
    UInt8   primaryDCIFormat;
    UInt8   secondaryDCIFormat;

}LteMacDlStrategyPerTTIPerUEStats;

#define MAX_STRATEGY_UE_SCHEDULED 50

typedef struct LteMacDlStrategyTTIStatsT
{
    UInt64 absoluteTTI;
    UInt8 numUeRetxnFailureQueue;
    UInt8 numUeRetxnQueue;
    UInt8 numTAFailureQueue;
    UInt8 numTAPendingQueue;
    UInt8 numTAFreshQueue;
    UInt8 numTAWithData;
    UInt8 scheduledReTXUE;
    UInt8 scheduledTAUE;
    UInt8 scheduledTxUE;
    UInt8 scheduledUETotal;
    LteMacDlStrategyPerTTIPerUEStats dlStrategyPerUE[MAX_STRATEGY_UE_SCHEDULED];
}LteMacDlStrategyTTIStats;

/* UL Strategy Stats */
typedef struct LteMacUlStrategyPerTTIPerUEStatsT
{
    UInt16 ueIndex;
    UInt16 tbSize;
    UInt16 minTBSize;
    UInt16 maxTBSize;
    ULGrantType ulGrantType;
    UInt8 ueAvgMCS;
#ifdef TDD_CONFIG
    UInt8 harqProcessId;
#endif
    UInt8 scheduledRB;
    UInt8 scheduleLcCount;
    UInt8 eligibleLcCount;
}LteMacUlStrategyPerTTIPerUEStats;

typedef struct LteMacUlStrategyTTIStatsT
{
    UInt64 absoluteTTI;
    UInt8 failedULGrants;
    UInt8 failedAndPendingSR;
    UInt8 failedAndPendingCR;
    UInt8 failedAndPendingApCQI;
    UInt8 pendingULGrants;
    UInt8 scheduledReTXUE;
    UInt8 scheduledSR;
    UInt8 scheduledCR;
    UInt8 scheduledApCQI;
    UInt8 scheduledDataApCQI;
    UInt8 scheduledTxUE;
    UInt8 scheduledTotalUE;
    LteMacUlStrategyPerTTIPerUEStats ulStrategyPerUE[MAX_STRATEGY_UE_SCHEDULED];
}LteMacUlStrategyTTIStats;

/* PDCCH Stats */
typedef struct LteMacPdcchPerTTIPerUEStatsT
{
    UInt8 cceIndex;
    UInt8 aggregationLevel;
    UInt8 dciFormat;
}LteMacPdcchPerTTIPerUEStats;

#define MAX_PDCCH_UE_SCHEDULED 35
#define MAX_PDCCH_CCCH_SCHEDULED 5
typedef struct LteMacPdcchTTIStatsT
{
    UInt64 absoluteTTI;
    UInt8 cceAllocFailDLCount;
    UInt8 cceAllocFailULCount;
    UInt8 scheduledDLReTXUE;
    UInt8 scheduledDLTxUE;
    UInt8 scheduledULReTXUE;
    UInt8 scheduledULTxUE;
    UInt8 scheduledCCCH;
    UInt8 totalCCEAllocated;
    LteMacPdcchPerTTIPerUEStats pdcchPerUE[MAX_PDCCH_UE_SCHEDULED];
    LteMacPdcchPerTTIPerUEStats pdcchPerCCCH[MAX_PDCCH_CCCH_SCHEDULED];
}LteMacPdcchTTIStats;

/* PDSCH Stats */
typedef struct LteMacPdschPerTTIPerUEStatsT
{
    UInt16  ueIndex;
    UInt8   transmissionType; /* New Tx/Adaptive Retx/Non-Adaptive Retx */
    UInt8   harqProcessId;
    UInt16  tbOneTBSize;
    UInt16  tbTwoTBSize;
    UInt8   codeWordOneMcs;
    UInt8   codeWordTwoMcs;
    UInt8   allocatedPRBs;
    UInt8   isSubBandScheduling;
    UInt16  ueWbMcs;
    UInt8   dciFormat;
    UInt32  resourceAllocationBitMap;
    UInt32  spsResourceAllocationBitMap;
    UInt8   ratPoilicy;
    UInt8   excessPoolRB;
    UInt8   allocationFailureCause;
    SInt8   tpc;
    UInt8   allocatedSPSRBs;
    UInt8   startSPSVRBIndex;
    UInt8   allocatedSPSMcs;
}LteMacPdschPerTTIPerUEStats;

// These stats need to be reset every 'MAX_TTI_STATS' TTIs
typedef struct LteMacPdschTTIStatsT
{
    UInt8 isPdschTTIStatsValid; 
    UInt64 absoluteTTI;
    UInt8 numScheduledUERetx;
    UInt8 numScheduledUENewTx;
    UInt8 numFailedUeTx;
    /* Count of the UE Scehduled. Should be reset to 0 at the start of TTI */
    UInt8 numScheduledUETotal;
    LteMacPdschPerTTIPerUEStats pdschStatsPerScheduledUE[MAX_UL_UE_SCHEDULED+10];
} LteMacPdschTTIStats;
/* Qos changes stop*/

/* Following structure contains the SPS specific info for an SPS LC of an UE */
typedef struct LteMacSpsConfigRespStatsT
{
    UInt8   isUeActive;
    UInt16  ueIndex;
    /* SPS config response contains non zero spsCrnti */
    UInt16  spsCrnti;
}LteMacSpsConfigRespStats;
typedef struct LteMacDlSpsUeSpecificStatsT
{
    UInt8   isUeActive;
    UInt16  ueIndex;
    UInt8   isSpsConfigured;
    UInt8   ipPktType;
    UInt8   rohcProfileUsed;
    UInt8   spsLc;
    UInt16  spsCrnti;
    UInt16  semiPersistentSchedIntervalDL;
    UInt8   numberOfConfSpsProcesses;  
    UInt8   maxSpsHarqRetx;
    UInt8   explicitReleaseAfter;
    UInt8   numOfN1PucchAnPersistent;
    UInt16  N1PucchAnPersistent[MAX_N1_PUCCH_AN_PERSISTENT];
    UInt8   ueDrxTTI[MAX_DL_SPS_INTERVAL];
    UInt8   ueMeasGapTTI[MAX_DL_SPS_INTERVAL];
} *LP_LteMacDlSpsUeSpecificStats, LteMacDlSpsUeSpecificStats;

typedef struct LteMacUlSpsUeSpecificStatsT
{
    UInt8   isUlUeActive;
    UInt16  ueIndex;
    /*SPR 15909 fix start*/
    UInt64 absoluteTTI; 
    /*SPR 15909 fix end*/
    UInt8   spsLc;
    UInt8   isSpsConfigured;
    UInt16  spsCrnti;
    UInt16  semiPersistentSchedIntervalUL;
    UInt8   implicitReleaseAfter;
    UInt8   twoIntervalsConfig;
    SInt8 p0NominalPuschPersistent;
    SInt8 p0UePuschPersistent;
} LteMacUlSpsUeSpecificStats;

typedef struct LteSpsDlRmStatsT
{
    UInt8   isDlSpsRmStatsValid; 
    UInt8   dlSpsRbsPerTTI[MAX_DL_SPS_INTERVAL];
    UInt8   dlSpsRbsPerTTIPerInterval[MAX_DL_SPS_INTERVAL][MAX_SPS_INTERVALS];
    UInt8   dlSpsDciPerTTI[MAX_DL_SPS_INTERVAL];
    UInt8   dlSpsOccasionPerTTI[MAX_DL_SPS_INTERVAL];
    UInt64  absoluteTTI;
} *LP_LteSpsDlRmStats, LteSpsDlRmStats;

typedef struct LteSpsUlRmStatsT
{
    UInt8   isUlSpsRmStatsValid; 
    UInt8   ulSpsRbsPerTTI[MAX_UL_SPS_INTERVAL];
    UInt8   ulSpsRbsPerTTIPerInterval[MAX_UL_SPS_INTERVAL][MAX_SPS_INTERVALS];
    UInt8   ulSpsDciPerTTI[MAX_UL_SPS_INTERVAL];
    UInt8   ulSpsOccasionPerTTI[MAX_UL_SPS_INTERVAL];
    UInt64  absoluteTTI;
} *LP_LteSpsUlRmStats, LteSpsUlRmStats;
#endif

// These stats need to be reset every 'MAX_TTI_STATS' TTIs
typedef struct LteUECommonInfoStatsT
{
    UEDRXStatsInfo ueDRXStats;
#ifdef SSI_DEBUG_STATS
    /* Current Write Index in puschSchedulingTTIndex stores the TTI position 
     * among 'MAX_TTI' TTIs when UE is scheduled */
    UInt16 puschCurWriteIndex;
    UEScheduledTTIInfo puschScheduledTTIIndex[MAX_TTI_STATS];
    /* Contains general UE specific Counters and UE Details */
    LteUEInfoStat ueInfoStat;
   
    /* CA Stats Changes Start */
    UInt16 pdschCurWriteIndex[MAX_NUM_CELL];
    UEScheduledTTIInfo pdschScheduledTTIIndex[MAX_NUM_CELL][MAX_TTI_STATS];
    
    UInt16 pdcchCurWriteIndex[MAX_NUM_CELL];
    UEScheduledTTIInfo pdcchScheduledTTIIndex[MAX_NUM_CELL][MAX_TTI_STATS];
    
    UInt16 dlStrategyCurWriteIndex[MAX_NUM_CELL];
    UEScheduledTTIInfo dlstartegyScheduledTTIIndex[MAX_NUM_CELL][MAX_TTI_STATS];
    /* CA Stats Changes End */
    
    UInt16 ulStrategyCurWriteIndex;
    UEScheduledTTIInfo ulstrategyScheduledTTIIndex[MAX_TTI_STATS];
    
    UInt16 dlStrategyLcCurWriteIndex;
    LCScheduledTTIInfo dlstrategyLcTTIIndex[MAX_TTI_STATS];
    
    UInt16 ulStrategyLcCurWriteIndex;
    LCScheduledTTIInfo ulstrategyLcTTIIndex[MAX_TTI_STATS];
    
    UInt16 ulStrategyhLcgCurWriteIndex;
    LCGScheduledTTIInfo ulstrategyLcgTTIIndex[MAX_TTI_STATS];
   
    /* Following field contains SPS specific information */
    LteMacDlSpsUeSpecificStats dlSpsUeInfoStats;
    LteMacUlSpsUeSpecificStats ulSpsUeInfoStats;
    LteMacSpsConfigRespStats   spsConfigRespStats;
#endif
}LteUECommonInfoStats;
/* ULA_CHG*/


typedef enum KPIStatsIndexT
{
    ACTIVE_KPI_INDEX = 0,
    PASSIVE_KPI_INDEX,
    /* SPR 11733_12826 changes start */
    MAXIMUM_KPI_INDEX
    /* SPR 11733_12826 changes end */
}KPIStatsIndex;

typedef struct LtePdcpKPIStatsT
{
   /* SPR 15909 fix start */
   UInt64 lastCalculatedTTI;
   /* SPR 15909 fix end */
   UInt32 totalDlDrbSduRcvd[MAX_QCI];
   UInt32 totalUlDrbSduRcvd[MAX_QCI];
   UInt32 totalDlDrbDiscardRatePerQci[MAX_QCI];
/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/
   UInt32 totalUlLossRatePerQci[MAX_QCI];
   /* SPR 3947 Changes Start */
   UInt32 totalDlDrbRlcDiscardRatePerQci[MAX_QCI];
   /* SPR 3947 Changes Stop */
/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/
/* KPI changes start */
    /* Average DL cell control plane PDCP SDU bit-rate  */
    /* 36.425 R10 Events */
    UInt32 dlCtrlPdcpSduBitRate; 
    /* Average UL cell control plane PDCP SDU bit-rate  */
    UInt32 ulCtrlPdcpSduBitRate; 
/* KPI changes end */
}LtePdcpKPIStats;

typedef struct LteMacKPIStatsT
{
    /* SPR 15909 fix start */
    UInt64 lastCalculatedTTI;
    /* SPR 15909 fix end */
    UInt32 uplinkCellBandwidth;
    /*SPR 15909 fix start*/
    UInt64 totalUplinkTTI;
    /*SPR 15909 fix end*/
    UInt64 totalUplinkBytesSent;
    UInt64 totalUplinkPRBUsage;
#if defined (KPI_STATS) || defined (PERF_STATS)
    /* L2_PERFORMANCE_OPT_CHANGES_START */    
    UInt64 totalDLPRBUsage;
    UInt64 totalDLPRBUsagePerQCI[MAX_QCI];
    UInt64 totalULPRBUsagePerQCI[MAX_QCI];
    /* SPR 23402 Changes Start */
    /* MOCN Changes Start */
    UInt64 totalDLPRBUsagePerOperatorPerQCI[MAX_NUM_MOCN_OPERATOR][MAX_QCI];
    UInt64 totalULPRBUsagePerOperatorPerQCI[MAX_NUM_MOCN_OPERATOR][MAX_QCI];
    /* MOCN Changes End */
    /* SPR 23402 Changes End */
     /* +- SPR 15429 */
    UInt64 totalActiveUsersPerQCIDLCumulative[MAX_QCI];
    UInt64 totalActiveUsersPerQCIULCumulative[MAX_QCI];
    /* SPR 23402 Changes Start */
    /* MOCN Changes Start */
    UInt64 totalActiveUsersPerOperatorPerQCIDLCumulative[MAX_NUM_MOCN_OPERATOR][MAX_QCI];
    UInt64 totalActiveUsersPerOperatorPerQCIULCumulative[MAX_NUM_MOCN_OPERATOR][MAX_QCI];
    /* MOCN Changes End */
    /* SPR 23402 Changes End */
    UInt64 totalRcvdRandomAccessPreamblesContentionFree;
    UInt64 totalRcvdRandomAccessPreamblesGroupA;
    UInt64 totalRcvdRandomAccessPreamblesGroupB;
    /* L2_PERFORMANCE_OPT_CHANGES_END */    
    /* KPI_CHANGES*/
    UInt64 totalPktDelayInDLPerQCI[MAX_QCI];
    /* L2_PERFORMANCE_OPT_CHANGES_START */    
    UInt64 totalPktAckedInDLPerQCI[MAX_QCI];
    /* L2_PERFORMANCE_OPT_CHANGES_END */  
    /* SPR 13492 Changes Start */
    UInt64 totalSessionTimeUE;
    UInt64 totalSessionTimePerQCI[MAX_QCI];
    /* SPR 13492 Changes End */
     /* KPI changes start */
    /* 36.425 R10 Events */
    /* Total number of times CQI 0 - 15 reported at CW1
     */
    UInt32 totalCQICW1Reported[MAX_CQI_INDEX];
    /* Total number of times CQI 0 - 15 reported at CW2
     */
    UInt32 totalCQICW2Reported[MAX_CQI_INDEX];
    /* KPI changes end */
    /*EICIC Changes Starts*/
    UInt32 AbsSubframeCount;
    /*EICIC Changes Ends*/
/*spr 19296 fix start*/
   UInt32 totalDlDrbUuLossRatePerQci[MAX_QCI];
   UInt32 totalSduAckRcvdDlperQci[MAX_QCI];
/*spr 19296 fix end*/
#endif
}LteMacKPIStats;

#if defined (KPI_STATS) || defined (PERF_STATS)
typedef struct LteMacKPIConfigT 
{
    UInt16  kpiBitmap;
    UInt8  kpiConfigured;
    UInt8  periodicReporting;
    /*SPR 15909 fix start*/
    UInt64 durationTTI;
    /*SPR 15909 fix end*/
    UInt16  transactionId;
}LteMacKPIConfig;
typedef struct LteMacRrmKPIConfigT 
{
    UInt16  kpiBitmapMac;
    UInt16  kpiBitmapPdcp;
    UInt8  kpiConfigured;
    UInt8  periodicReporting;
    /*SPR 15909 fix start*/
    UInt64 durationTTI;
    /*SPR 15909 fix end*/
    UInt16  transactionId;
}LteMacRrmKPIConfig;
#endif

/*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
typedef struct LteMacHdFddStatsT
{
    /*Number of HD FDD UE attached to the eNodeB*/
    UInt32 numOfHdFddUE;

    /* Total number of times UL or DL schedule for the HD UEs */
    UInt32 numDLSchedule;
    UInt32 numULSchedule;

    /* Flag indicating the index which are for Half duplex UEs */
    UInt8 hdFddFlag[MAX_UE_SUPPORTED];

    /* Total number of times UE denied DL resource because UL were already allocated */
    UInt32 totalNumTimesUEdeniedDL[MAX_UE_SUPPORTED];
    /* Total number of times UE denied UL resource because DL were already allocated */
    UInt32 totalNumTimesUEdeniedUL[MAX_UE_SUPPORTED];

    /* Number of times UL or DL schedule for the HD UEs */
    UInt32 numTimesUESchedDL[MAX_UE_SUPPORTED];
    UInt32 numTimesUESchedUL[MAX_UE_SUPPORTED];
}LteMacHdFddStats;
#endif
/*HD FDD Changes End*/

#if defined (KPI_STATS) || defined (PERF_STATS)

/* 32.425 Events code changes start*/
/*Node structure of the KPI UL IP Thp Queues */
typedef struct KPIUlIpThpQueueNodeT 
{
    /*CAUTION: Required circular queue enqueue changes if Anchor node moved down in the structure */
    LTE_SNODE   kpiQueueNode;
    /*UE ID of the UE*/
    UInt32     ueId;
    UInt32     lcId;
    /* SPR 15909 fix start */
    UInt64     timeStamp;
    /* SPR 15909 fix end */
    UInt32     trigger ;
}KPIUlIpThpQueueNode;
/* + KPI_CA GBL */
#define QINIT_KPI_STATS_UL_IP_THP_Q() {\
		UInt8 internalCellIndex = 0;\
		for( ;internalCellIndex < MAX_NUM_CELL;internalCellIndex++){\
        circQInit( &kpiStatsUlIpThpQueue_g[internalCellIndex], MAX_NODES_SPLIT);\
		}\
	}

#define ENQUEUE_KPI_STATS_UL_IP_THP_Q( node_p, internalCellIndex)\
        pushNodeCircQ( &kpiStatsUlIpThpQueue_g[internalCellIndex], node_p)

#define DEQUEUE_KPI_STATS_UL_IP_THP_Q( node_p, internalCellIndex) {\
        popNodeCircQ( &kpiStatsUlIpThpQueue_g[internalCellIndex], (void **)&(node_p));\
        node_p = (KPIUlIpThpQueueNode *) node_p;\
}\

#define QUEUE_COUNT_KPI_STATS_UL_IP_THP_Q( internalCellIndex)\
        circQueueCount( &kpiStatsUlIpThpQueue_g[internalCellIndex])\
/* - KPI_CA GBL */

typedef struct ScheduledIPThroughputDLT
{
    /* SPR 15909 fix start */
    UInt64 t1[30];
    UInt64 t2[30];
    /*SPR 11812 Fix Start*/
    UInt64 t1prev[30];
    /*SPR 11812 Fix End*/
    UInt8 commonIndex;
    UInt32 pdcpAckBytes[30];
    UInt64 pdcpTick[30];
    /* SPR 15909 fix end */
}ScheduledIPThroughputDLT;

typedef struct ScheduledIPThroughputULT
{
    /* SPR 15909 fix start */
    UInt64 t1;
    UInt64 t2;
    UInt64 t1prev;
    /* SPR 15909 fix end */
    UInt32 pdcpBytes;
    UInt32 lastPdcpSduBytes;
}ScheduledIPThroughputULT;

extern ScheduledIPThroughputDLT scheduledIPThroughputDL_g[MAX_UE_SUPPORTED][MAX_LOGICAL_CHANNEL - 3];
/* 32.425 Events code changes end*/

typedef struct LteKPIThpStatusT
{
    UInt16 servingRespId;
    UInt16 startUeIdNxtBatch;
    /* SPR 10579 changes start*/
    UInt32 kpiThpStatIndFlag;
    UInt32 kpiGetThpStatRespCount;
    /* SPR 10579 changes end*/
    UInt32 moduleBitmap;
    /* +- SPR 15692 */
}LteKPIThpStatus;

typedef struct LtePdcpKPIThpStatsT
{
    /*SPR 10021 Fix Start*/
    /*SPR 15909 fix start*/
    UInt64 durationTTI;
    /*SPR 15909 fix end*/
    /*SPR 10021 Fix End*/
    /* 100MS_SUPPORT : start */
/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/
    /* 100MS_SUPPORT : end */
    /* Average DL cell PDCP SDU bit-rate stats */
    /*SPR 10021 Fix Start*/
    
    /* SPR 15692 fix start */
    /* One additional index is to keep the stats for the deleted RABs QCI wise
     * Since there are 8 DRBs and Total of 9 QCIs and we need to maintain the 
     * data QCI wise for deleted RAB (for other RABs for which Delete LC 
     * request is not yet received, is maintained LC wise only), hence 
     * second index is indexed to MAX_QCI as MAX_QCI > MAX_LOGICAL_CHANNEL - 3
     * */

    UInt64 SharedMemoryDLPDCPSDU[(MAX_UE_SUPPORTED + MAX_NUM_CELL)][MAX_QCI];
    UInt64 SharedMemoryULPDCPSDU[(MAX_UE_SUPPORTED + MAX_NUM_CELL)][MAX_QCI];
    /* SPR 15692 fix end */
    /* + SPR 17415 */
    /* Following parameter is to maintain the size of PDCP SDUs that are
     * received in UL forwarding for each LC of UE */
     UInt64 uLFwdedPdcpSdu[(MAX_UE_SUPPORTED + 1)][MAX_QCI];
     /* - SPR 17415 */

    /* Average DL cell PDCP SDU bit-rate stats */
/* 32.425 Events code changes start*/
/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/
/* 32.425 Events code changes end*/
}LtePdcpKPIThpStats;

typedef struct LteMacKPIThpStatsT
{  
    /*SPR 15909 fix start*/
    UInt64 durationTTI;
    /*SPR 15909 fix end*/
/*spr 19296 fix start*/
    /* SPR23194 Start */
    /*Code deleted totalDlLcThp KPI discarded at MAC*/
    UInt64 scheduledIPThpVolDL[MAX_UE_SUPPORTED + MAX_NUM_CELL][MAX_LOGICAL_CHANNEL - 3];
    UInt64 scheduledIPThpVolUL[MAX_UE_SUPPORTED + MAX_NUM_CELL][MAX_LOGICAL_CHANNEL - 3];
    UInt32 scheduledIPThpTimeDL[MAX_UE_SUPPORTED + MAX_NUM_CELL][MAX_LOGICAL_CHANNEL - 3];
    UInt32 scheduledIPThpTimeUL[MAX_UE_SUPPORTED + MAX_NUM_CELL][MAX_LOGICAL_CHANNEL - 3];
    UInt32 sharedMemoryLatSample[MAX_UE_SUPPORTED + MAX_NUM_CELL][MAX_LOGICAL_CHANNEL - 3]; 
    UInt32 SharedMemoryLatTime[MAX_UE_SUPPORTED + MAX_NUM_CELL][MAX_LOGICAL_CHANNEL - 3];
    /* SPR23194 End */
/*spr 19296 fix end*/
}LteMacKPIThpStats;


typedef struct LtePdcpKPIConfigT 
{
    UInt8  kpiBitmap;
    UInt8  kpiConfigured;
    UInt8  periodicReporting;
    /*SPR 15909 fix start*/
    UInt64 durationTTI;
    /*SPR 15909 fix end*/
    UInt16 transactionId;
}LtePdcpKPIConfig;

typedef enum KpiStatsTypeT
{
  MAC_KPI_STATS  = 1,
  PDCP_KPI_STATS = 2,
  /* 100MS_SUPPORT : start */
  MAC_KPI_THP_STATS = 3,
  PDCP_KPI_THP_STATS = 4
  /* 100MS_SUPPORT : end */
}KpiStatsType;

/* + KPI_CA */
typedef struct KpiStatsContainerT
{
    KpiStatsType     macOrPdcpStats;
    union{
    LteMacKPIConfig  lteMacKPIConfig;
    LtePdcpKPIConfig ltePdcpKPIConfig;
    }config;
    union{
    LteMacKPIStats   lteMacKPIStats[MAX_NUM_CELL];
    LtePdcpKPIStats  ltePdcpKPIStats[MAX_NUM_CELL];
    /* SPR 10579 changes start*/
    LteMacKPIThpStats  lteMacKPIThpStats;
    LtePdcpKPIThpStats ltePdcpKPIThpStats;
    }stats;
    union{
    LteKPIThpStatus    lteMacKPIThpStatus;
    LteKPIThpStatus    ltePdcpKPIThpStatus;
    }status;
    /* SPR 10579 changes end*/
}KpiStatsContainer;
/* - KPI_CA */

typedef struct kpiScannerT
{
    /* 100MS_SUPPORT : start */
    UInt16             module_bitmask;
    /* 100MS_SUPPORT : end */
    /* SPR 15909 fix start */
    UInt64        timeout_tti[MAX_KPI_CONTAINER];
    UInt64        prevkpi_tick;
    /* SPR 15909 fix end */
    KpiStatsContainer kpiStatsContainer[MAX_KPI_CONTAINER];
    /* SPR 13492 Changes Start */
    /*SPR 15909 fix start*/
    UInt64            prevKpiInSessionTick;
    /*SPR 15909 fix end*/
    UInt32            timeoutInSessionTTI[MAX_KPI_CONTAINER];
    /* SPR 13492 Changes End */
}kpiScanner;

typedef struct kpiModuleInfoT
{
    UInt16 srcModuleId;
    UInt16 dstModuleId;
    UInt16 hdrLen;
    UInt16 configCnfMsgType;
    SInt32 (*sendToModule)(UInt8 *, UInt16
            );
    UInt16 periodicIndMsgType;
    UInt16 aperiodicCnfMsgType;
    UInt16 periodicThpIndMsgType;
    UInt16 aperiodicThpCnfMsgType;
}kpiModuleInfo;

extern kpiScanner g_kpiScannerObj;
/* Fix for Coverity Warning for 100ms KPI Starts */
extern kpiModuleInfo g_kpiModuleInfo[MAX_KPI_CONTAINER];
/* Fix for Coverity Warning for 100ms KPI Ends */
extern SInt32 pdcpOamMsgEventFd_g;
#endif
typedef enum PdcpKpiSupportedT
{
    KPI_TOTAL_DL_DRB_DISCARD_RATE = 0, //bit 0
/*spr 19296 fix start*/
    KPI_TOTAL_UL_DRB_LOSS_RATE    = 1, //bit 2
    KPI_PDCP_DL_LC_THROUGHPUT     = 2, //bit 3
    KPI_PDCP_UL_LC_THROUGHPUT     = 3, //bit 4
    /* KPI changes start */
    KPI_PDCP_DL_CTR_SDU_BITRATE       = 4, 
    KPI_PDCP_UL_CTR_SDU_BITRATE       = 5,
/*spr 19296 fix end*/
    /* KPI changes end */
    KPI_PDCP_MAX_VALUE
}PdcpKpiSupported;
/* 100MS_SUPPORT : start */
/* SPR 12314 changes start */
#define CELL_SPECIFIC_PDCP_COUNTERS_BITMAP 0x67 
/* SPR 12314 changes end */
/* 100MS_SUPPORT : end */
typedef struct LteCommonKPIStatsT
{
#if defined(KPI_STATS) || defined(PERF_STATS)

    /* SPR 20831 Changes Start */
    /*code deleted*/
    /* SPR 20831 Changes End*/
    /* PDCP Stats*/
    UInt32             activeIndexForPdcpStats;
    /* SPR 11733_12826 changes start */
    LtePdcpKPIStats    ltePdcpKPIStats[MAXIMUM_KPI_INDEX];
    /* SPR 11733_12826 changes end */

    UInt32             activeIndexForMacStats;
    /* SPR 11733_12826 changes start */
    LteMacKPIStats     lteMacKPIStats[MAXIMUM_KPI_INDEX];
    /* SPR 11733_12826 changes end */

    /* L2_PERFORMANCE_OPT_CHANGES_START */    
    UInt64 totalActiveUsersPerQciDL[MAX_QCI];
    UInt64 totalActiveUsersPerQCIUL[MAX_QCI];
    /* L2_PERFORMANCE_OPT_CHANGES_END */    
    /* SPR 23402 Changes Start */ 
    /* MOCN Changes Start */
    UInt64 totalActiveUsersPerOperatorPerQciDL[MAX_NUM_MOCN_OPERATOR][MAX_QCI];
    UInt64 totalActiveUsersPerOperatorPerQciUL[MAX_NUM_MOCN_OPERATOR][MAX_QCI];
    /* MOCN Changes End */
    /* SPR 23402 Changes End */
#endif
#ifdef UTFWK_SIMULATION
#ifdef KPI_STATS
    LteMacKPIStats lteMacKPIStatsForPrinting;
    UInt64         initialTTI;
    UInt64         finalTTI;
#endif
#endif
}LteCommonKPIStats;
/* + E_CID_5.3*/
typedef struct LteMacEcidMeasStatsT
{
    UInt16 numOfPdcchOrders;
}LteMacEcidMeasStats;
/* - E_CID_5.3 */
#ifdef RLC_QUEUE_STATS
typedef struct PerUePerLcQueueOccupancyStatsT
{
UInt16 currentOccupancyPerUePerLc[MAX_UE_SUPPORTED][MAX_LOGICAL_CHANNEL];
UInt16 peakOccupancyPerUePerLc[MAX_UE_SUPPORTED][MAX_LOGICAL_CHANNEL];
}PerUePerLcQueueOccupancyStats;

typedef struct LteMacPerUePerLcQueueStatsT
{
PerUePerLcQueueOccupancyStats qPerUePerLcStats[MAX_UE_LC_QUEUE_COUNT];
}LteMacPerUePerLcQueueStats;

typedef struct QueueOccupancyStatsT
{
UInt16 currentOccupancy;
UInt16 peakOccupancy;
}QueueOccupancyStats;

typedef struct LteMacQueueStatsT
{
QueueOccupancyStats qStats[MAX_QUEUE_COUNT];
}LteMacQueueStats;
#endif

#ifdef ACK_NACK_COUNT_STATS

typedef struct AckNackStatsT
{
UInt32 ackCount;
UInt32 nackCount;
}AckNackStats;

typedef struct LteCommonDebugStatsT
{
AckNackStats TBOneAckNack[MAX_UE_SUPPORTED];
AckNackStats TBTwoAckNack[MAX_UE_SUPPORTED];
UInt8 Mimo_NonMimo_Flag;
}LteCommonDebugStats;

#endif
#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
typedef enum maxSplitStatsT
{
 UL_OVERSHOOT_STATS = 0,
 DL_OVERSHOOT_STATS,
 MAX_OVERSHOOT_STATS
}maxSplitStats;
#endif
/*Structure to calculate the PRBUsage stats for load balancing in the CA*/
typedef struct LteMacCADLPrbUsageStatsT
{
    UInt8 totalDLPRBUsagePercent;
    /*SPR 15909 fix start*/
    UInt64 lastCalculatedTTI;
    /*SPR 15909 fix end*/
    UInt64 totalDLPRBUsage;
} LteMacCADLPrbUsageStats; 

#if defined (KPI_STATS) || defined (PERF_STATS)
/* + KPI_CA */
typedef struct LteCommonUeKPIStatsT
{
    /* PDCP Throughput Stats*/
	/* + SPR 10579 Merge Fix */
    /* LteKPIThpStatus    ltePdcpKPIThpStatus; */
	/* - SPR 10579 Merge Fix */
    UInt32             activeIndexForPdcpThpStats;
    /* SPR 11733_12826 changes start */
    LtePdcpKPIThpStats ltePdcpKPIThpStats[MAXIMUM_KPI_INDEX];
    /* SPR 11733_12826 changes end */

    /* MAC Throughput Stats*/
	/* + SPR 10579 Merge Fix */
    /* LteKPIThpStatus    lteMacKPIThpStatus; */
	/* - SPR 10579 Merge Fix */
    UInt32             activeIndexForMacThpStats;
    /* SPR 11733_12826 changes start */
    LteMacKPIThpStats  lteMacKPIThpStats[MAXIMUM_KPI_INDEX];    
    /* SPR 11733_12826 changes start */

}LteCommonUeKPIStats;
/* - KPI_CA */

#endif
/* + stats_review_comments_ipc */ 
#ifdef SSI_DEBUG_STATS
/* typedef	struct ThpStatsPCellScellT
{
    UInt32 totalDlBytesSend;
}ThpStatsPcellScell; */
#endif


/* - stats_review_comments_ipc */ 
/* Structure to maintain LTE MAC layer stats */
typedef struct LteMacStatsT
{
    /* CA Stats Changes Start */
    /* Stats for PHY interface */
    LteMacPhyInfStats   phyStats[MAX_NUM_CELL];
    /* CA Stats Changes End */

    /* Stats for RLC interface */
    LteMacRlcInfStats   rlcStats;

    /* Stats for RRC interface */
    LteMacRrcInfStats   rrcStats;

    /* Performance stats */
    /* CA Stats Changes Start */
#if (defined (DL_UL_SPLIT) ||defined(DL_UL_SPLIT_TDD))
    LteMacPerfStats     perfStatsSplit[MAX_NUM_CELL][MAX_OVERSHOOT_STATS];
#else
    LteMacPerfStats     perfStats[MAX_NUM_CELL];
#endif
#ifdef DL_DATA_SEPARATION
    LteMacPerfStats     encoderPerfStats[MAX_NUM_CELL];
#endif
    /* CA Stats Changes End */
    LteMacPerfStats     ulPerfStats;

/*spr 22474 start*/
    /*CSPL Code removed*/
/*spr 22474 end*/
#ifdef RLC_QUEUE_STATS
	/*Per Ue Per Lc queue occupancy stats */
	LteMacPerUePerLcQueueStats     queuePerUePerLcStats;
	/*queue occupancy stats */
	LteMacQueueStats     queueStats;
#endif
#ifdef ACK_NACK_COUNT_STATS
        LteCommonDebugStats debugStats;
#endif
   
    /* Cycle Count Stats*/
#ifdef CYCLE_COUNT_MAC_RLC_PDCP
    /* CA Stats Changes Start */
    SInt8                     ccIterator[MAX_NUM_CELL];
    Ltelayer2CycleCountStats ccStats[MAX_NUM_CELL][NUM_L2_CC_ITERATIONS];
#endif
    /* ULA_CHG */
    /* globalTick modulo MAX_TTI_STATS - indicates the write position 
     * in to TTI array */
    UInt16		  writeTTIIndex[MAX_NUM_CELL]; 
#ifdef SSI_DEBUG_STATS
	/* + stats_review_comments_ipc */
	UInt8 numActiveCells;
	/* - stats_eeview_comments_ipc */
    /* Captures the number of active UEs in system */
    UInt16 numActiveUe[MAX_NUM_CELL];
	/* + stats_review_comments_ipc */ 
	/* These stats keep Throughput Stats for a UE on P-Cell and All its S-Cell */ 
    /* ThpStatsPcellScell thpStats[MAX_UE_SUPPORTED][MAX_NUM_CELL]; */
	/* - stats_review_comments_ipc */ 
	
	/* These stats need to be reset every 'MAX_TTI_STATS' TTIs */
    LteMacPuschTTIStats   puschTTIStats[MAX_NUM_CELL][MAX_TTI_STATS]; 

    /* These stats need to be reset every 'MAX_TTI_STATS' TTIs */
    LteMacPdcchTTIStats   pdcchTTIStats[MAX_NUM_CELL][MAX_TTI_STATS]; 

    /* These stats need to be reset every 'MAX_TTI_STATS' TTIs */
    LteMacPdschTTIStats   pdschTTIStats[MAX_NUM_CELL][MAX_TTI_STATS]; 

    /* These stats need to be reset every 'MAX_TTI_STATS' TTIs */
    LteMacDlStrategyTTIStats   dlStrategyTTIStats[MAX_NUM_CELL][MAX_TTI_STATS]; 

    /* These stats need to be reset every 'MAX_TTI_STATS' TTIs */
    LteMacUlStrategyTTIStats   ulStrategyTTIStats[MAX_NUM_CELL][MAX_TTI_STATS]; 

    LteQosDLStrategyLCTTIStat  dlStrategyLcTTIStats[MAX_NUM_CELL][MAX_TTI_STATS];

    LteQosULStrategyLCTTIStat  ulStrategyLcTTIStats[MAX_NUM_CELL][MAX_TTI_STATS];

    LteQosULStrategyLCGTTIStat ulStrategyLcgTIStats[MAX_NUM_CELL][MAX_TTI_STATS];
    
    QoSUEAndPerLCTTIStat qoSUEAndPerLCTTIStat[MAX_NUM_CELL];

    /* Write TTI Idx in which the stats are last updated */
    UInt16  lastWriteIdx[MAX_NUM_CELL];
    LteSpsDlRmStats  dlSpsRmStats[MAX_NUM_CELL][MAX_TTI_STATS];
    UInt16  lastUlWriteIdx[MAX_NUM_CELL];
    LteSpsUlRmStats  ulSpsRmStats[MAX_NUM_CELL][MAX_TTI_STATS];
    
    LteSpsQueuesUsed     spsQueueUsed[MAX_NUM_CELL][MAX_TTI_STATS];
#endif

    LteUECommonInfoStats  ueCommonInfoStats[MAX_UE_SUPPORTED];
#if defined (KPI_STATS) || defined (PERF_STATS)
	/* + KPI_CA */
    LteCommonUeKPIStats lteCommonUeKPIStats;
    LteCommonKPIStats	  lteCommonKPIStats[MAX_NUM_CELL];
#endif
	/* - KPI_CA */

/**************************************************************
 * This structure contains the Performance stats that are to be 
 * reported from L2 to OAM. 
 * ************************************************************/
    /* ULA_CHG */
    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    LteMacHdFddStats lteMacHdFddStats;
#endif
    /*HD FDD Changes End*/
    /* + E_CID_5.3 */
    LteMacEcidMeasStats lteMacEcidMeasStats[MAX_UE_SUPPORTED];
    /* - E_CID_5.3*/
    LteMacCADLPrbUsageStats lteMacCADLPrbUsageStats[MAX_NUM_CELL];
}LteMacStats;

/* Structure for storing Stat shared memory info */
typedef struct LteMacStatsInfoT
{
    SInt32          shmId;
    LteMacStats     *pStats;
} LteMacStatsInfo;

#ifdef FLEXRAN
typedef struct
{
    UInt32  psScanningReqAdvanceSf;
    UInt32 prachFormat4Threshold;
    UInt32 prachFormat0Threshold;

}eltaAstriParamThreshold;
#endif
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern ULong32  SMThID_g;
/* MAC stats handle */
extern LteMacStatsInfo gMacStats;


/* + PERF_CA GBL */
extern UInt16   lLteMacStatsRfSfn_g[MAX_NUM_CELL] ;
extern UInt16   lLteMacStatsRfSf_g[MAX_NUM_CELL] ;
/* - PERF_CA GBL */
#if (MAX_SFN_VALUE < LTE_MAC_STATS_MAX_SFN)
extern UInt16   lLteMacStatsRfMulFactor_g ;
#else
#define lLteMacStatsRfMulFactor_g 0
#endif

/* CA Stats Changes Start */
#if (defined (DL_UL_SPLIT) || defined (DL_UL_SPLIT_TDD))
void lteMacUpdateStatsPerfEnd(UInt32, InternalCellIndex);
#else
void lteMacUpdateStatsPerfEnd(InternalCellIndex);
#endif

#ifdef DL_DATA_SEPARATION
void lteMacUpdateDLDataSepStatsPerfStart(InternalCellIndex internalCellIndex);
void encoderUpdateCycleCount_clkTimeStats(UInt32 encoderFuncId,UInt64 cycleCount,
        TimeSpec *start, TimeSpec *end,InternalCellIndex internalCellIndex);/*SPR 19428 +-*/
#endif
/* CA Stats Changes End */

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#ifdef RLC_QUEUE_STATS
extern void lteMacUpdateStatsQueuePerUePerLc(UInt16, UInt16, UInt8, UInt16);
extern void lteMacUpdateStatsQueue(UInt16, UInt16);
#endif

#ifdef ACK_NACK_COUNT_STATS
extern void lteCommonUpdateAckNackStats(UInt8, UInt16, UInt8);
#endif


#ifdef CYCLE_COUNT_MAC_RLC_PDCP
/* CA Stats Changes Start */
#if (defined(DL_UL_SPLIT_TDD) || defined(FDD_CONFIG))
void pdcpUpdateCycleCount_clkTimeStats(UInt32 fucnId,UInt64 cycleCount,
            TimeSpec *start, TimeSpec *end, InternalCellIndex internalCellIndex);
void socketUpdateCycleCount_clkTimeStats(UInt32 FuncId,UInt64 cycleCount,
            TimeSpec *start, TimeSpec *end, InternalCellIndex internalCellIndex);
#if (defined(DL_UL_SPLIT_TDD) || defined(DL_UL_SPLIT))
void updateCycleCount_clkTimeStats(UInt32 funcId,UInt64 cycleCount,
            TimeSpec *start, TimeSpec *end, InternalCellIndex internalcellIndex);
#else
void macUpdateCycleCount_clkTimeStats(UInt32 fucnId,UInt64 cycleCount,
            TimeSpec *start, TimeSpec *end, InternalCellIndex internalCellIndex);
void rlcUpdateCycleCount_clkTimeStats(UInt32 fucnId,UInt64 cycleCount,
            TimeSpec *start, TimeSpec *end, InternalCellIndex internalCellIndex);
#endif
#endif
#if (defined(TDD_CONFIG) && !defined(DL_UL_SPLIT_TDD))
void legUpdateCycleCount_clkTimeStats(UInt32 fucnId,UInt64 cycleCount,
            TimeSpec *start, TimeSpec *end, UInt8 internalCellIndex);
#endif
void encoderUpdateCycleCount_clkTimeStats(UInt32 encoderFuncId,UInt64 cycleCount,
            TimeSpec *start, TimeSpec *end, InternalCellIndex internalCellIndex);
void lteIncrementL2Cycles(UInt8 internalcellIndex);
/* CA Stats Changes End */
#endif
#ifdef UTFWK_SIMULATION
/*SPR 15909 fix start*/
void lteMacQosUpdateBsrStats(UInt32 BsrNet, UInt32 ueIndex, tickType_t currentTick);
void lteMacQosUpdateLcgStats(UInt32 lcgIndex,UInt32 ueIndex, tickType_t currentTick,UInt32 lcgLoad);
void lteMacQosUpdateLcStats(UInt32 lcIndex,UInt32 ueIndex, tickType_t currentTick,UInt32 lcQLoad);
/*SPr 15909 fix end*/
#endif
/* ULA_CHG */
/* CA Stats Changes Start */
void lteMacUpdateStatsWriteTTIIndex(InternalCellIndex internalCellIndex);
void lteMacResetPuschStats(InternalCellIndex internalCellIndex);
/* CA Stats Changes End */

void lteMacUpdateStatsPuschPerTTIPerUe(UInt8 numScheduledUETotal,
        UInt16 ueIndex,
        UInt8 transmissionType,
        UInt8 harqIndex,
        UInt8 numRBsAllocated,
        UInt8 startRBPosition,
        UInt8 ulGrantRequestType,
        UInt32 oppGranted,
        UInt8 frequencySeletiveScheduling,
        UInt8 selectedMCS,
        UInt8 allocationFailureCause,
        SInt8 tpc
        /* UL_MU_MIMO_CHG_START */
        ,MuMimoStatsPerUe *muMimoStatsPerUe,
        /* UL_MU_MIMO_CHG_END */
        /* CA Stats Changes Start */
        InternalCellIndex internalCellIndex
        /* CA Stats Changes End */
        );

void lteMacUpdateStatsUeCommonInfo(
        /* +- SPR 18268 */
        UInt16 ueIndex,
        /* +- SPR 18268 */
        UInt16 ueIndexPos,
        UInt64 currentAbsoluteTTI
        );

void lteMacUpdateStatsCommonKPIStats(UInt8 totalUplinkPRBUsage);

#ifdef KPI_STATS
/* + KPI_CA */ 
extern void sendPdcpKpiStatsInd(LtePdcpKPIStats  *pLtePdcpKPIStats,
                                LtePdcpKPIConfig *pLtePdcpKPIConfig,
                                kpiModuleInfo    *pKpiModuleInfo,
								InternalCellIndex internalCellIndex);
/* - KPI_CA */ 
extern SInt32 pdcpOamSendMessage(UInt8 *data_p, UInt16 size);

#endif



/* ULA_CHG */

/*UL strategy changes start*/
/*UE Common INFO stats start*/

/*UE category*/
#ifdef SSI_DEBUG_STATS
#define \
        LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UE_ACTIVE(ueIndex,\
                        value)\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.isUeActive =\
     value
#else
#define \
        LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UE_ACTIVE(ueIndex,\
                        ueCat)
#endif



/*Rnti*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_RNTI(ueIndex,value)\
        gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.rnti = value
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_RNTI(ueIndex,value)
#endif

/*UE Tx Mode*/
#ifdef SSI_DEBUG_STATS
#define \
        LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_TX_MODE(ueIndex,txnMode) \
        gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ueTxnMode = \
        txnMode
#else
#define \
        LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_TX_MODE(ueIndex,txnMode)
#endif

/*UE category*/
#ifdef SSI_DEBUG_STATS
#define \
        LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UE_CATEGORY(ueIndex,\
                        ueCat)\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ueCategory =\
    ueCat
#else
#define \
        LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UE_CATEGORY(ueIndex,\
                        ueCat)
#endif

/*srb count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_SRB_COUNT(ueIndex)\
            ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.srbCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_SRB_COUNT(ueIndex)
#endif

/*UL Gbr count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_GBR_COUNT(ueIndex)\
            ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulGbrLcCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_GBR_COUNT(ueIndex)
#endif

#ifdef SSI_DEBUG_STATS
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_GBR_COUNT(ueIndex)\
            --gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulGbrLcCount
#else
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_GBR_COUNT(ueIndex)
#endif

/*DL Gbr count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_GBR_COUNT(ueIndex)\
            ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlGbrLcCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_GBR_COUNT(ueIndex)
#endif

#ifdef SSI_DEBUG_STATS
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_GBR_COUNT(ueIndex)\
            --gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlGbrLcCount
#else
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_GBR_COUNT(ueIndex)
#endif


/*UL NGbr count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_NGBR_COUNT(ueIndex)\
            ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulNonGbrLcCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_NGBR_COUNT(ueIndex)
#endif

#ifdef SSI_DEBUG_STATS
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_NGBR_COUNT(ueIndex)\
            --gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulNonGbrLcCount
#else
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_NGBR_COUNT(ueIndex)
#endif

/*DL NGbr count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_NGBR_COUNT(ueIndex)\
            ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlNonGbrLcCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_NGBR_COUNT(ueIndex)
#endif

#ifdef SSI_DEBUG_STATS
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_NGBR_COUNT(ueIndex)\
            --gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlNonGbrLcCount
#else
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_NGBR_COUNT(ueIndex)
#endif


/*UL AMBR*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_AMBR(ueIndex,ambr)\
            gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulAmbr =\
ambr
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_AMBR(ueIndex,ambr)
#endif


/*DL AMBR*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_AMBR(ueIndex,ambr)\
            gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlAmbr =\
ambr
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_AMBR(ueIndex,ambr)
#endif

/*DL UE scheduled count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_SCH_COUNT(ueIndex)\
            ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlScheduledCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_SCH_COUNT(ueIndex)
#endif

/*Increment UL UE scheduled count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_SCH_COUNT(ueIndex)\
            ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulScheduledCount
#else
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_SCH_COUNT(ueIndex)
#endif

/*Decrement UL UE scheduled count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_SCH_COUNT(ueIndex)\
            --gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulScheduledCount
#else
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_SCH_COUNT(ueIndex)
#endif



/*DRX ON count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DRX_ON_DL_COUNT(ueIndex)\
            ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlDrxOnCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DRX_ON_DL_COUNT(ueIndex)
#endif

/*MEAS GAP ON count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_MEAS_GAP_ON_DL_COUNT(ueIndex)\
            ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlMeasGapOnCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_MEAS_GAP_ON_DL_COUNT(ueIndex)
#endif

/*UL SYNC LOSS ON count*/
#ifdef SSI_DEBUG_STATS
#define  LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_SYNC_LOSS_ON_DL_COUNT(ueIndex)\
                ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlSyncLossOnCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_SYNC_LOSS_ON_DL_COUNT(ueIndex)
#endif

    /*DRX OFF count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DRX_OFF_DL_COUNT(ueIndex)\
                ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlDrxOffCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DRX_OFF_DL_COUNT(ueIndex)
#endif

    /*MEAS GAP OFF count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_MEAS_GAP_OFF_DL_COUNT(ueIndex)\
                ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlMeasGapOffCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_MEAS_GAP_OFF_DL_COUNT(ueIndex)
#endif

    /*UL SYNC LOSS OFF count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_SYNC_LOSS_OFF_DL_COUNT(ueIndex)\
                ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlSyncLossOffCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_SYNC_LOSS_OFF_DL_COUNT(ueIndex)
#endif

/* Stats for Triggers to UL Startegy */

/*DRX ON count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DRX_ON_UL_COUNT(ueIndex)\
            ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulDrxOnCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DRX_ON_UL_COUNT(ueIndex)
#endif

/*MEAS GAP ON count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_MEAS_GAP_ON_UL_COUNT(ueIndex)\
            ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulMeasGapOnCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_MEAS_GAP_ON_UL_COUNT(ueIndex)
#endif

/*UL SYNC LOSS ON count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_SYNC_LOSS_ON_UL_COUNT(ueIndex)\
                ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulSyncLossOnCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_SYNC_LOSS_ON_UL_COUNT(ueIndex)
#endif

    /*DRX OFF count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DRX_OFF_UL_COUNT(ueIndex)\
                ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulDrxOffCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DRX_OFF_UL_COUNT(ueIndex)
#endif

    /*MEAS GAP OFF count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_MEAS_GAP_OFF_UL_COUNT(ueIndex)\
                ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulMeasGapOffCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_MEAS_GAP_OFF_UL_COUNT(ueIndex)
#endif

    /*UL SYNC LOSS OFF count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_SYNC_LOSS_OFF_UL_COUNT(ueIndex)\
                ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulSyncLossOffCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_SYNC_LOSS_OFF_UL_COUNT(ueIndex)
#endif

    /*Increment DL  Tree count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_TREE_COUNT(ueIndex)\
                ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlTreeLcCount
#else
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_TREE_COUNT(ueIndex)
#endif

    /*Increment DL  Tree count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_TREE_COUNT(ueIndex)\
                --gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlTreeLcCount
#else
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_TREE_COUNT(ueIndex)
#endif
 
   /*Increment UL  Tree count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_TREE_COUNT(ueIndex)\
                ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulTreeLcCount
#else
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_TREE_COUNT(ueIndex)
#endif
 
   /*Decrement UL  Tree count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_TREE_COUNT(ueIndex)\
                --gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulTreeLcCount
#else
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_TREE_COUNT(ueIndex)
#endif




    /*Update SRB Tokens*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_SRB_TOKENS(ueIndex, value)\
          gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.srbTokenCount = value
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_SRB_TOKENS(ueIndex, value)
#endif


    /*Increment DL  Eligible LC count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_ELIGIBLE_LC_COUNT(ueIndex)\
                ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlEligibleLcCount
#else
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_ELIGIBLE_LC_COUNT(ueIndex)
#endif

    /*Decrement DL  Eligible LC count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_ELIGIBLE_LC_COUNT(ueIndex)\
                --gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlEligibleLcCount
#else
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_ELIGIBLE_LC_COUNT(ueIndex)
#endif


    /*Increment UL  Eligible LC count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_ELIGIBLE_LC_COUNT(ueIndex)\
                ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulEligibleLcCount
#else
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_ELIGIBLE_LC_COUNT(ueIndex)
#endif

    /*Decrement UL  Eligible LC count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_ELIGIBLE_LC_COUNT(ueIndex)\
                --gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulEligibleLcCount
#else
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_ELIGIBLE_LC_COUNT(ueIndex)
#endif



    /*Increment DL  Zero Non-Zero count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_ZERO_NONZERO_COUNT(ueIndex)\
                ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlZeroNonZeroCount
#else
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_ZERO_NONZERO_COUNT(ueIndex)
#endif

    /*Decrement DL  Zero Non-Zero count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_ZERO_NONZERO_COUNT(ueIndex)\
                --gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlZeroNonZeroCount
#else
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_ZERO_NONZERO_COUNT(ueIndex)
#endif


    /*Increment DL  Zero Non-Zero count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_NONZERO_ZERO_COUNT(ueIndex)\
                ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlNonZeroZeroCount
#else
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_NONZERO_ZERO_COUNT(ueIndex)
#endif

    /*Decrement DL  Zero Non-Zero count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_NONZERO_ZERO_COUNT(ueIndex)\
                --gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlNonZeroZeroCount
#else
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_NONZERO_ZERO_COUNT(ueIndex)
#endif

    /*Increment UL  Zero Non-Zero count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_ZERO_NONZERO_COUNT(ueIndex)\
                ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulZeroNonZeroCount
#else
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_ZERO_NONZERO_COUNT(ueIndex)
#endif

    /*Decrement UL  Zero Non-Zero count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_ZERO_NONZERO_COUNT(ueIndex)\
                --gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulZeroNonZeroCount
#else
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_ZERO_NONZERO_COUNT(ueIndex)
#endif


    /*Increment UL  Zero Non-Zero count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_NONZERO_ZERO_COUNT(ueIndex)\
                ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulNonZeroZeroCount
#else
#define LTE_MAC_INCREMENT_UE_COMMON_INFO_UE_INFO_STAT_DL_NONZERO_ZERO_COUNT(ueIndex)
#endif

    /*Decrement UL  Zero Non-Zero count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_NONZERO_ZERO_COUNT(ueIndex)\
                --gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulNonZeroZeroCount
#else
#define LTE_MAC_DECREMENT_UE_COMMON_INFO_UE_INFO_STAT_UL_NONZERO_ZERO_COUNT(ueIndex)
#endif






/* DL New Tx count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_NEW_TX_COUNT(ueIndex)\
            ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlNewTxnCount;
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_NEW_TX_COUNT(ueIndex) 
#endif

/* DL Re Tx count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_RE_TX_COUNT(ueIndex)\
         ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlReTxnCount;
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_RE_TX_COUNT(ueIndex)
#endif


/* UL New Tx count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_NEW_TX_COUNT(ueIndex)\
            ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulNewTxnCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_NEW_TX_COUNT(ueIndex)
#endif

/* UL Re Tx count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_RE_TX_COUNT(ueIndex)\
         ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulReTxnCount;
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_RE_TX_COUNT(ueIndex)
#endif

    /*PDCCH fail count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_PDCCH_FAIL_COUNT(ueIndex)\
                ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.pdcchFailCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_PDCCH_FAIL_COUNT(ueIndex)
#endif
    
/*PDSCH fail count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_PDSCH_FAIL_COUNT(ueIndex)\
            ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.pdschFailCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_PDSCH_FAIL_COUNT(ueIndex)
#endif

/*PUSCH fail count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_PUSCH_FAIL_COUNT(ueIndex)\
            ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.puschFailCount
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_PUSCH_FAIL_COUNT(ueIndex)
#endif


/*DL UE INFO - AMBR token overlow*/
#ifdef SSI_DEBUG_STATS
#define \
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_AMBR_TOKEN_OVERFLOW(ueIndex)\
++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlAmbrTokenOverflow
#else
#define \
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_AMBR_TOKEN_OVERFLOW(ueIndex)
#endif


/*UL UE INFO - AMBR token overlow*/
#ifdef SSI_DEBUG_STATS
#define \
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_AMBR_TOKEN_OVERFLOW(ueIndex)\
++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulAmbrTokenOverflow
#else
#define \
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_AMBR_TOKEN_OVERFLOW(ueIndex)
#endif




/*DL LC INFO*/
#ifdef SSI_DEBUG_STATS
#define \
LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO(ueIndex,lcIndex,qciValue,\
        mode,bitRateValue,depth,tokenRateValue)\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlLCInfoStat[lcIndex].\
    isValid = TRUE;\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlLCInfoStat[lcIndex].\
    qci = qciValue;\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlLCInfoStat[lcIndex].\
    lcMode = mode;\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlLCInfoStat[lcIndex].\
    lcId = lcIndex;\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlLCInfoStat[lcIndex].\
    bitRate = bitRateValue;\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlLCInfoStat[lcIndex].\
    tokenDepth =\
    depth;\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlLCInfoStat[lcIndex].\
    totalLCWeight = 0;\
        gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlLCInfoStat[lcIndex].\
        tokenRate = tokenRateValue
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO(ueIndex,lcIndex,qciValue,\
                mode,bitRateValue,depth,tokenRateValue)
#endif

/*DL LC INFO - token overlow*/
#ifdef SSI_DEBUG_STATS
#define  LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO_TOKEN_OVERFLOW(ueIndex,lcIndex)\
++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlLCInfoStat[lcIndex].\
tokenOverflow
#else
#define \
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO_TOKEN_OVERFLOW(ueIndex,lcIndex)
#endif

/*DL LC INFO - token overlow*/
#ifdef SSI_DEBUG_STATS
#define \
  LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO_TOKEN_PRIORITY(ueIndex,lcIndex,value)\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlLCInfoStat[lcIndex].\
       tokenPriority = value; 

#else
#define \
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO_TOKEN_PRIORITY(ueIndex,lcIndex, value) 
#endif

/*DL LC INFO - token overlow*/
#ifdef SSI_DEBUG_STATS
#define \
  LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO_TOTAL_LC_WIEGHT(ueIndex,lcIndex,value)\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlLCInfoStat[lcIndex].\
       totalLCWeight = value; 

#else
#define \
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO_TOTAL_LC_WIEGHT(ueIndex,lcIndex, value) 
#endif


/*DL LC INFO - is Invalid */
#ifdef SSI_DEBUG_STATS
#define \
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO_IS_NOT_VALID(ueIndex,lcIndex)\
gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.dlLCInfoStat[lcIndex].\
isValid = FALSE; 
#else
#define \
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_LC_INFO_IS_NOT_VALID(ueIndex,lcIndex)
#endif

/*DL LC INFO - is Invalid */
#ifdef SSI_DEBUG_STATS
#define \
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_LC_INFO_IS_NOT_VALID(ueIndex,lcIndex)\
gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulLCInfoStat[lcIndex].\
isValid = FALSE; 
#else
#define \
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_LC_INFO_IS_NOT_VALID(ueIndex,lcIndex)
#endif



    /*UL LC INFO*/
#ifdef SSI_DEBUG_STATS
#define \
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_LC_INFO(ueIndex,lcIndex,qciValue,\
            mode,bitRateValue,depth,tokenRateValue)\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulLCInfoStat[lcIndex].\
    isValid = TRUE;\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulLCInfoStat[lcIndex].\
    qci = qciValue;\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulLCInfoStat[lcIndex].\
    lcMode = mode;\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulLCInfoStat[lcIndex].\
    lcId = lcIndex;\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulLCInfoStat[lcIndex].\
    bitRate = bitRateValue;\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulLCInfoStat[lcIndex].\
    tokenDepth = depth;\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulLCInfoStat[lcIndex].\
    tokenRate = tokenRateValue
#else
#define \
        LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_LC_INFO(ueIndex,lcIndex,qciValue,\
                mode,bitRateValue,depth,tokenRateValue)
#endif

/*UL LC INFO - token Priority*/
#ifdef SSI_DEBUG_STATS
#define \
  LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_LC_INFO_TOKEN_PRIORITY(ueIndex,lcIndex,value)\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulLCInfoStat[lcIndex].\
       tokenPriority = value; 

#else
#define \
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_LC_INFO_TOKEN_PRIORITY(ueIndex,lcIndex, value) 
#endif

/*UL LC INFO - total LC weight */
#ifdef SSI_DEBUG_STATS
#define \
  LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_LC_INFO_TOTAL_LC_WIEGHT(ueIndex,lcIndex,value)\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulLCInfoStat[lcIndex].\
       totalLCWeight = value; 

#else
#define \
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_LC_INFO_TOTAL_LC_WIEGHT(ueIndex,lcIndex, value) 
#endif



    /*UL LC INFO - token overlow*/
#ifdef SSI_DEBUG_STATS
#define \
        LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_LC_INFO_TOKEN_OVERFLOW(ueIndex,lcIndex)\
            ++gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.ulLCInfoStat[lcIndex].\
            tokenOverflow
#else
#define \
        LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_LC_INFO_TOKEN_OVERFLOW(ueIndex,lcIndex)
#endif


/*DL schedule flag*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_SCHEDULE_FLAG(ueIndex,\
        state)\
                gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.\
                        dlScheduleFlag = state
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_SCHEDULE_FLAG(ueIndex,\
        state)
#endif
/*UL schedule flag*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_SCHEDULE_FLAG(ueIndex,\
        state)\
                gMacStats.pStats->ueCommonInfoStats[ueIndex].ueInfoStat.\
                        ulScheduleFlag = state
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_SCHEDULE_FLAG(ueIndex,\
        state)
#endif


#ifdef SSI_DEBUG_STATS
#define UL_STRATEGY_WRITE_INDEX \
gMacStats.pStats->ueCommonInfoStats[ueIndex].ulStrategyCurWriteIndex
#endif

#ifdef SSI_DEBUG_STATS
#define INCREMENT_UL_STRATEGY_WRITE_INDEX(ueId) \
{\
    UInt16 *ulStrategyCurWriteIndex_p =\
        &gMacStats.pStats->ueCommonInfoStats[(ueId)].ulStrategyCurWriteIndex;\
    *ulStrategyCurWriteIndex_p += 1;\
    if(*ulStrategyCurWriteIndex_p == MAX_TTI_STATS)\
    {\
        *ulStrategyCurWriteIndex_p = 0;\
    }\
}
#endif



#ifdef SSI_DEBUG_STATS
#define \
LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_STRATEGY_SCHEDULED_TTI_INDEX_INFO(ueId,currentTick,numScheduledUE)\
        gMacStats.pStats->ueCommonInfoStats[(ueId)].ulstrategyScheduledTTIIndex[UL_STRATEGY_WRITE_INDEX].\
        ueIndexPos = numScheduledUE;\
        gMacStats.pStats->ueCommonInfoStats[(ueId)].ulstrategyScheduledTTIIndex[UL_STRATEGY_WRITE_INDEX].\
        absoluteTTI = currentTick;
#else
#define \
LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_STRATEGY_SCHEDULED_TTI_INDEX_INFO(ueId,currentTick,numScheduledUE)
#endif

#ifdef SSI_DEBUG_STATS
#define UL_STRATEGY_LC_WRITE_INDEX\
        gMacStats.pStats->ueCommonInfoStats[ueIndex].ulStrategyLcCurWriteIndex
#endif

#ifdef SSI_DEBUG_STATS
#define UL_STARTEGY_UE_LC_INDEX \
        gMacStats.pStats->ueCommonInfoStats[ueIndex].ulstrategyLcTTIIndex[UL_STRATEGY_LC_WRITE_INDEX].\
    lcCurWriteIndex
#endif

    /*Absolute TTI*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_STRATEGY_LC_SCHEDULED_PER_TTI_ABSOLUTE_TTI(ueIndex,value)\
        gMacStats.pStats->ueCommonInfoStats[ueIndex].ulstrategyLcTTIIndex[UL_STRATEGY_WRITE_INDEX].\
        absoluteTTI = value;
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_STRATEGY_LC_SCHEDULED_PER_TTI_ABSOLUTE_TTI(ueIndex,value)
#endif

    /*LC Index*/
#ifdef SSI_DEBUG_STATS
#define  LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_STRATEGY_LC_SCHEDULED_PER_TTI_LC_INDEX(ueIndex,value)\
        gMacStats.pStats->ueCommonInfoStats[ueIndex].ulstrategyLcTTIIndex[UL_STRATEGY_WRITE_INDEX].\
        lcIndexPos[UL_STARTEGY_UE_LC_INDEX] = value;\
        ++UL_STARTEGY_UE_LC_INDEX
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_STRATEGY_LC_SCHEDULED_PER_TTI_LC_INDEX(ueIndex,value)
#endif

#ifdef SSI_DEBUG_STATS
#define UL_STRATEGY_LCG_WRITE_INDEX \
    gMacStats.pStats->ueCommonInfoStats[ueIndex].ulStrategyhLcgCurWriteIndex
#endif

#ifdef SSI_DEBUG_STATS
/* CA Stats Changes Start */
#define WRITE_TTI_INDEX(internalCellIndex) \
            gMacStats.pStats->writeTTIIndex[(internalCellIndex)]
#else
#define WRITE_TTI_INDEX(internalCellIndex)
#endif
#define INCREMENT_WRITE_TTI_INDEX( currTick, internalCellIndex) \
    ++gMacStats.pStats->writeTTIIndex[(internalCellIndex)];\
	if(MAX_TTI_STATS == WRITE_TTI_INDEX(internalCellIndex))\
	{\
		WRITE_TTI_INDEX(internalCellIndex) = 0;\
	}\
    gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].currentTickValue[ \
          WRITE_TTI_INDEX(internalCellIndex)] = currTick + 1;\
    INITIALIZE_TTI_STATS(internalCellIndex);
#ifdef SSI_DEBUG_STATS
#define INITIALIZE_TTI_STATS(internalCellIndex) \
    UInt16 writeTTIIndex = 0;\
    UInt16 ueId = 0;\
    writeTTIIndex = gMacStats.pStats->writeTTIIndex[(internalCellIndex)];\
    gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][writeTTIIndex].scheduledReTXUE = 0;\
    gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][writeTTIIndex].scheduledTAUE = 0;\
    gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][writeTTIIndex].scheduledTxUE = 0;\
    gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][writeTTIIndex].scheduledUETotal = 0;\
    gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][writeTTIIndex].numTAFreshQueue = 0;\
    gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][writeTTIIndex].numTAWithData = 0;\
    gMacStats.pStats->dlStrategyLcTTIStats[(internalCellIndex)][writeTTIIndex].numScheduledLC = 0;\
    gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][writeTTIIndex].scheduledReTXUE = 0;\
    gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][writeTTIIndex].scheduledTxUE = 0;\
    gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][writeTTIIndex].scheduledTotalUE = 0;\
    gMacStats.pStats->ulStrategyLcTTIStats[(internalCellIndex)][writeTTIIndex].numScheduledLC = 0;\
    gMacStats.pStats->pdschTTIStats[(internalCellIndex)][writeTTIIndex].isPdschTTIStatsValid = 0;\
    gMacStats.pStats->pdschTTIStats[(internalCellIndex)][writeTTIIndex].numScheduledUETotal = 0;\
    gMacStats.pStats->pdschTTIStats[(internalCellIndex)][writeTTIIndex].numFailedUeTx = 0;\
    gMacStats.pStats->pdschTTIStats[(internalCellIndex)][writeTTIIndex].numScheduledUERetx = 0;\
    gMacStats.pStats->pdschTTIStats[(internalCellIndex)][writeTTIIndex].numScheduledUENewTx = 0;\
    gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIndex].isPuschTTIStatsValid = 0;\
    gMacStats.pStats->dlSpsRmStats[(internalCellIndex)][writeTTIIndex].isDlSpsRmStatsValid = 0;\
    gMacStats.pStats->ulSpsRmStats[(internalCellIndex)][writeTTIIndex].isUlSpsRmStatsValid = 0;\
    /*+COVERITY 27497*/\
    for( ueId = 0; ueId < MAX_DL_UE_SCHEDULED; ueId++ ) \
    {\
        gMacStats.pStats->pdschTTIStats[(internalCellIndex)][writeTTIIndex].pdschStatsPerScheduledUE[ueId].allocationFailureCause = 0;\
    }\
    for( ueId = 0; ueId < MAX_UL_UE_SCHEDULED; ueId++ ) \
    {\
        gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIndex].puschStatsPerScheduledUE[ueId].allocationFailureCause = 0;\
    }\
    /*-COVERITY 27497*/\
    for( ueId = 0; ueId < MAX_UE_SUPPORTED; ueId++ ) \
    { \
        gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].qoSUELCStat[ueId].spsTTIStats[writeTTIIndex].isSpsPerUePerTTIStatsValid = 0;\
        gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].qoSUELCStat[ueId].spsTTIStatsUl[writeTTIIndex].isSpsPerUePerTTIStatsULValid = 0;\
        gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].qoSUELCStat[ueId].spsPerUeHarqPerTTIStats[writeTTIIndex].isSpsPerUeHarqPerTTIStatsValid = 0;\
        gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].qoSUELCStat[ueId].spsPerUeHarqPerTTIStats[writeTTIIndex].numOfBusySpsHarqProcessForNewTx = 0;\
        gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].qoSUELCStat[ueId].spsPerUeHarqPerTTIStats[writeTTIIndex].numOfBusySpsHarqProcessForReTx = 0;\
        gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].qoSUELCStat[ueId].spsPerUeHarqPerTTIStats[writeTTIIndex].numOfBusyDRAHarqProcessForNewTx = 0;\
        gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].qoSUELCStat[ueId].spsPerUeHarqPerTTIStats[writeTTIIndex].numOfBusyDRAHarqProcessForReTx = 0;\
        gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].qoSUELCStat[ueId].spsDebugPerUePerTTIStats[writeTTIIndex].isSpsDebugPerUePerTTIStatsValid = 0;\
        gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].qoSUELCStat[ueId].spsDebugPerUePerTTIStatsUl[writeTTIIndex].isSpsDebugPerUePerTTIStatsULValid = 0;\
    }
#else
#define INITIALIZE_TTI_STATS(internalCellIndex)
#endif


/*Absolute TTI*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_ABSOLUTE_TTI(currTTI, internalCellIndex)\
            gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].absoluteTTI =\
            currTTI
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_ABSOLUTE_TTI(currTTI, internalCellIndex)
#endif

/*Failed UL grants*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_FAILED_UL_GRANTS(count, internalCellIndex)\
            gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].failedULGrants \
            = count
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_FAILED_UL_GRANTS(count, internalCellIndex)
#endif

    /*Failed and pending SR*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_FAILED_AND_PENDING_SR(internalCellIndex) \
        ++gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].failedAndPendingSR
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_FAILED_AND_PENDING_SR(internalCellIndex)
#endif

    /*Failed and pending CR*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_FAILED_AND_PENDING_CR(internalCellIndex)\
        ++gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].failedAndPendingCR
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_FAILED_AND_PENDING_CR(internalCellIndex)
#endif

    /*Failed and pending Aperiodic CQI*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_FAILED_AND_PENDING_AP_CQI(internalCellIndex)\
        ++gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].failedAndPendingApCQI 
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_FAILED_AND_PENDING_AP_CQI(internalCellIndex)
#endif
    

    /*UE Common INFO stats end*/

    /*Pending UL Grants*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PENDING_UL_GRANTS(count, internalCellIndex)\
        gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].pendingULGrants =\
        count
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PENDING_UL_GRANTS(count, internalCellIndex)
#endif


    /*Scheduled ReTx UE*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_RETX_UE(count, internalCellIndex)\
        gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].scheduledReTXUE =\
    count
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_RETX_UE(count, internalCellIndex)
#endif

    /*Scheduled SR*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_SR(internalCellIndex)\
        ++gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].scheduledSR
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_SR(internalCellIndex)
#endif

    /*Scheduled CR*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_CR(internalCellIndex)\
        ++gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].scheduledCR
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_CR(internalCellIndex)
#endif

    /*Scheduled Aperiodic CQI*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_AP_CQI(internalCellIndex)\
        ++gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].scheduledApCQI
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_AP_CQI(internalCellIndex)
#endif

    /*Scheduled DATA Aperiodic CQI*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_AP_DATA_CQI(internalCellIndex)\
        ++gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].scheduledDataApCQI
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_AP_DATA_CQI(internalCellIndex)
#endif

    /*Scheduled Tx UE*/
 #ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_TX_UE(internalCellIndex)\
        ++gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].scheduledTxUE
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_SCHEDULED_TX_UE(internalCellIndex)
#endif

   
#ifdef SSI_DEBUG_STATS
#define UL_SCHEDULED_TOTAL_UE(internalCellIndex) \
        gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].scheduledTotalUE
#endif


    /*UL strategy per UE per TTI*/
    /*UE index*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_UE_INDEX(value1, value2)\
        gMacStats.pStats->ulStrategyTTIStats[(value2)][WRITE_TTI_INDEX(value2)].\
    ulStrategyPerUE[UL_SCHEDULED_TOTAL_UE(value2)].ueIndex = value1; \
    gMacStats.pStats->ueCommonInfoStats[value1].ueInfoStat.ulStrategyStatsIndex = \
    UL_SCHEDULED_TOTAL_UE(value2)
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_UE_INDEX(value1, value2)
#endif


    /*min TB size*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_MIN_TB_SIZE(value1, value2)\
        gMacStats.pStats->ulStrategyTTIStats[(value2)][WRITE_TTI_INDEX(value2)].\
    ulStrategyPerUE[UL_SCHEDULED_TOTAL_UE(value2)].minTBSize = value1
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_MIN_TB_SIZE(value1, value2)
#endif
 
   /*TB size*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_TB_SIZE(value1,value2, value3)\
        gMacStats.pStats->ulStrategyTTIStats[(value3)][WRITE_TTI_INDEX(value3)].\
    ulStrategyPerUE[\
        gMacStats.pStats->ueCommonInfoStats[value1].ueInfoStat.ulStrategyStatsIndex].tbSize = value2
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_TB_SIZE(value1,value2, value3)
#endif


    /*max TB size*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_MAX_TB_SIZE(value1, value2)\
        gMacStats.pStats->ulStrategyTTIStats[(value2)][WRITE_TTI_INDEX(value2)].\
    ulStrategyPerUE[UL_SCHEDULED_TOTAL_UE(internalCellIndex)].maxTBSize = value1
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_MAX_TB_SIZE(value1, value2)
#endif

    /*UL grant type*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_UL_GRANT_TYPE(value1, value2)\
        gMacStats.pStats->ulStrategyTTIStats[(value2)][WRITE_TTI_INDEX(value2)].\
    ulStrategyPerUE[UL_SCHEDULED_TOTAL_UE(value2)].ulGrantType = value1
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_UL_GRANT_TYPE(value1, value2)
#endif

    /*avg MCS*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_AVG_MCS(value1, value2)\
        gMacStats.pStats->ulStrategyTTIStats[(value2)][WRITE_TTI_INDEX(value2)].\
    ulStrategyPerUE[UL_SCHEDULED_TOTAL_UE(value2)].ueAvgMCS = value1
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_AVG_MCS(value1, value2)
#endif



    /*Harq Process ID*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_HARQ_PROCESS_ID(value1, value2)\
        gMacStats.pStats->ulStrategyTTIStats[(value2)][WRITE_TTI_INDEX(value2)].\
    ulStrategyPerUE[UL_SCHEDULED_TOTAL_UE(value2)].harqProcessId = value1
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_HARQ_PROCESS_ID(value1, value2)
#endif

    /*Scheduled RB*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_SCHEDULED_RB(value1, value2)\
        gMacStats.pStats->ulStrategyTTIStats[(value2)][WRITE_TTI_INDEX(value2)].\
    ulStrategyPerUE[UL_SCHEDULED_TOTAL_UE(value2)].scheduledRB = value1
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_SCHEDULED_RB(value1, value2)
#endif


    /*Eligible LC count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_ELIGIBLE_LC_COUNT(internalCellIndex)\
        ++gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].\
    ulStrategyPerUE[UL_SCHEDULED_TOTAL_UE(internalCellIndex)].eligibleLcCount
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_ELIGIBLE_LC_COUNT(internalCellIndex)
#endif
    
    /*Scheduled LC count*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_SCHEDULED_LC_COUNT(ueId, internalCellIndex)\
        ++gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].\
    ulStrategyPerUE[UL_SCHEDULED_TOTAL_UE(internalCellIndex)].scheduleLcCount;\
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_UL_STRATEGY_SCHEDULED_TTI_INDEX_INFO( (ueId) ,\
            gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].absoluteTTI,\
            UL_SCHEDULED_TOTAL_UE(internalCellIndex));\
    ++UL_SCHEDULED_TOTAL_UE(internalCellIndex);\
    gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].\
    ulStrategyPerUE[UL_SCHEDULED_TOTAL_UE(internalCellIndex)].eligibleLcCount = 0;\
    gMacStats.pStats->ulStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].\
    ulStrategyPerUE[UL_SCHEDULED_TOTAL_UE(internalCellIndex)].scheduleLcCount = 0;\
    INCREMENT_UL_STRATEGY_WRITE_INDEX( (ueId) );
#else
    #define LTE_MAC_UPDATE_UL_STRATEGY_TTI_STATS_PER_UE_SCHEDULED_LC_COUNT(ueId, internalCellIndex)
#endif
/* CA Stats Changes End */

    /*UL strategy per TTI stats end*/



    /*UL Strategy per LC per TTI start*/

    /*ABSOLUTE TTI*/
#ifdef SSI_DEBUG_STATS
/* CA Stats Changes Start */
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_ABSOLUTE_TTI(currTTI, internalCellIndex)\
        gMacStats.pStats->ulStrategyLcTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].\
    absoluteTTI = currTTI
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_ABSOLUTE_TTI(currTTI, internalCellIndex)
#endif

    /*Number of scheduled LC*/
#ifdef SSI_DEBUG_STATS
#define UL_NUM_SCHEDULED_LC(internalCellIndex) \
        gMacStats.pStats->ulStrategyLcTTIStats[(internalCellIndex)]\
                      [WRITE_TTI_INDEX(internalCellIndex)].numScheduledLC
#endif

/*UE id and LC Id*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_UE_ID_LC_ID(value1,value2, value3)\
        gMacStats.pStats->ulStrategyLcTTIStats[(value3)][WRITE_TTI_INDEX(value3)].\
    ulStrategyPerLC[UL_NUM_SCHEDULED_LC(value3)].ueIndex = value1;\
        gMacStats.pStats->ulStrategyLcTTIStats[(value3)][WRITE_TTI_INDEX(value3)].\
    ulStrategyPerLC[UL_NUM_SCHEDULED_LC(value3)].lcId = value2;\
    gMacStats.pStats->ueCommonInfoStats[value1].ueInfoStat.ulLCInfoStat[value2].lcPos = UL_NUM_SCHEDULED_LC(value3);
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_UE_ID_LC_ID(value1,value2, value3)
#endif


    /*BSR load*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_BSR_LOAD(value1, value2)\
        gMacStats.pStats->ulStrategyLcTTIStats[(value2)][WRITE_TTI_INDEX(value2)].\
    ulStrategyPerLC[UL_NUM_SCHEDULED_LC(value2)].BSRLoad = value1
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_BSR_LOAD(value1, value2)
#endif


/*Scheduled Bytes*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_SCHEDULED_BYTES(value1, value2)\
        gMacStats.pStats->ulStrategyLcTTIStats[(value2)][WRITE_TTI_INDEX(value2)].\
    ulStrategyPerLC[UL_NUM_SCHEDULED_LC(value2)].scheduledBytes = value1

#else
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_SCHEDULED_BYTES(value1, value2)
#endif
    /*ReScheduled Bytes*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_RESCHEDULED_BYTES(value1,value2,value3, value4)\
        gMacStats.pStats->ulStrategyLcTTIStats[(value4)][WRITE_TTI_INDEX(value4)].\
    ulStrategyPerLC[gMacStats.pStats->ueCommonInfoStats[value1].ueInfoStat.ulLCInfoStat[value2].lcPos].resheduledBytes = value3
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_RESCHEDULED_BYTES(value1,value2,value3i, value4)
#endif

    /*head of line delay*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_HEAD_OF_LINE_DLY(value1, value2)\
        gMacStats.pStats->ulStrategyLcTTIStats[(value2)][WRITE_TTI_INDEX(value2)].\
    ulStrategyPerLC[UL_NUM_SCHEDULED_LC(value2)].headOfLineDelay = value1
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_HEAD_OF_LINE_DLY(value1, value2)
#endif

    /*Drx on duration delay*/
#ifdef SSI_DEBUG_STATS
#define \
    LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_DRX_ON_DURATION_DLY(value1, value2)\
        gMacStats.pStats->ulStrategyLcTTIStats[(value2)][WRITE_TTI_INDEX(value2)].\
        ulStrategyPerLC[UL_NUM_SCHEDULED_LC(value2)].drxOnDurationDelay = value1
#else
#define \
    LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_DRX_ON_DURATION_DLY(value1, value2)
#endif

    /*Available Tokens*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_AVAILABLE_TOKEN(value1, value2)\
        gMacStats.pStats->ulStrategyLcTTIStats[(value2)][WRITE_TTI_INDEX(value2)].\
    ulStrategyPerLC[UL_NUM_SCHEDULED_LC(value2)].availToken = value1
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_AVAILABLE_TOKEN(value1, value2)
#endif
 
 /*Remaining Qload*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_REMAINING_QLOAD(value1, value2)\
        gMacStats.pStats->ulStrategyLcTTIStats[(value2)][WRITE_TTI_INDEX(value2)].\
    ulStrategyPerLC[UL_NUM_SCHEDULED_LC(value2)].qLoad = value1
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_REMAINING_QLOAD(value1, value2)
#endif



    /*Total LC weight*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_TOTAL_LC_WEIGHT(value1, value2)\
        gMacStats.pStats->ulStrategyLcTTIStats[(value2)][WRITE_TTI_INDEX(value2)].\
    ulStrategyPerLC[UL_NUM_SCHEDULED_LC(value2)].totalLcWeight = value1;\
            ++UL_NUM_SCHEDULED_LC(value2)
#else
#define LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_TOTAL_LC_WEIGHT(value1, value2)
#endif

    /*Total Token scheduling window*/
#ifdef SSI_DEBUG_STATS
#define \
    LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_TOTAL_TOKEN_SCHED_WINDOW(value1,value2,value3, value4)\
        gMacStats.pStats->ulStrategyLcTTIStats[(value4)][WRITE_TTI_INDEX(value4)].\
        ulStrategyPerLC[gMacStats.pStats->ueCommonInfoStats[value1].ueInfoStat.ulLCInfoStat[value2].lcPos].totTknSchedWndw = value3;
#else
#define \
    LTE_MAC_UPDATE_UL_STRATEGY_LC_TTI_STATS_PER_LC_TOTAL_TOKEN_SCHED_WINDOW(value1,value2,value3, value4)
#endif
/* CA Stats Changes End */

    /*UL Strategy per LC per TTI end*/

    /*UL strategy changes stop*/


/*DL strategy changes start*/

/*UE Common INFO stats start*/

/* CA Stats Changes Start */
#ifdef SSI_DEBUG_STATS
#define DL_STRATEGY_WRITE_INDEX(ueIndex, internalCellIndex) \
gMacStats.pStats->ueCommonInfoStats[ueIndex].dlStrategyCurWriteIndex[(internalCellIndex)]
#endif

#ifdef SSI_DEBUG_STATS
#define INCREMENT_DL_STRATEGY_WRITE_INDEX(ueIndex, internalCellIndex) \
{\
    UInt16 *dlStrategyCurWriteIndex_p =\
        &gMacStats.pStats->ueCommonInfoStats[ueIndex].dlStrategyCurWriteIndex[(internalCellIndex)];\
    (*dlStrategyCurWriteIndex_p) += 1;\
    if((*dlStrategyCurWriteIndex_p) == MAX_TTI_STATS)\
    {\
        (*dlStrategyCurWriteIndex_p) = 0;\
    }\
}
#endif
/* CA Stats Changes End */



#ifdef SSI_DEBUG_STATS
#define DL_STRATEGY_LC_WRITE_INDEX(ueIndex)\
    gMacStats.pStats->ueCommonInfoStats[ueIndex].dlStrategyLcCurWriteIndex
#endif


#ifdef SSI_DEBUG_STATS
#define INCREMENT_DL_STRATEGY_LC_WRITE_INDEX(ueIndex) \
{\
    UInt16 *dlStrategyLcCurWriteIndex_p =\
        &gMacStats.pStats->ueCommonInfoStats[ueIndex].dlStrategyLcCurWriteIndex;\
    (*dlStrategyLcCurWriteIndex_p) += 1;\
    if((*dlStrategyLcCurWriteIndex_p) == MAX_TTI_STATS)\
    {\
        (*dlStrategyLcCurWriteIndex_p) = 0;\
    }\
}
#endif

#ifdef SSI_DEBUG_STATS
#define DL_STARTEGY_UE_LC_INDEX(ueIndex) \
        gMacStats.pStats->ueCommonInfoStats[ueIndex].dlstrategyLcTTIIndex[DL_STRATEGY_LC_WRITE_INDEX(ueIndex)].\
    lcCurWriteIndex
#endif

    /*Absolute TTI*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_STRATEGY_LC_SCHEDULED_PER_TTI_ABSOLUTE_TTI(ueIndex,value) \
        gMacStats.pStats->ueCommonInfoStats[ueIndex].dlstrategyLcTTIIndex[(DL_STRATEGY_LC_WRITE_INDEX(ueIndex))].\
absoluteTTI = value;
#else
#define  LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_STRATEGY_LC_SCHEDULED_PER_TTI_ABSOLUTE_TTI(ueIndex,value)
#endif

    /*LC Index*/
#ifdef SSI_DEBUG_STATS
#define  LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_STRATEGY_LC_SCHEDULED_PER_TTI_LC_INDEX(ueIndex,value)\
        gMacStats.pStats->ueCommonInfoStats[ueIndex].dlstrategyLcTTIIndex[DL_STRATEGY_LC_WRITE_INDEX(ueIndex)].\
        lcIndexPos[DL_STARTEGY_UE_LC_INDEX(ueIndex)] = value;\
        ++(DL_STARTEGY_UE_LC_INDEX(ueIndex));
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_STRATEGY_LC_SCHEDULED_PER_TTI_LC_INDEX(ueIndex,value)
#endif



/* CA Stats Changes Start */
    /*Update the UE positon in common UE Info TTI*/
#ifdef SSI_DEBUG_STATS
#define  LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_STRATEGY_UE_SCHEDULED_PER_TTI_UE_POSTION(ueIndex,curTTI, value, internalCellIndex)\
        gMacStats.pStats->ueCommonInfoStats[ueIndex].dlstartegyScheduledTTIIndex[\
              (internalCellIndex)][DL_STRATEGY_WRITE_INDEX(ueIndex, internalCellIndex)].\
        absoluteTTI = curTTI;\
        gMacStats.pStats->ueCommonInfoStats[ueIndex].dlstartegyScheduledTTIIndex[\
              (internalCellIndex)][DL_STRATEGY_WRITE_INDEX(ueIndex, internalCellIndex)].\
        ueIndexPos = value;
#else
#define LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_STRATEGY_UE_SCHEDULED_PER_TTI_UE_POSTION(ueIndex,curTTI, value, internalCellIndex)
#endif



/*Absolute TTI*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_ABSOLUTE_TTI(currTTI, internalCellIndex)\
            gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].absoluteTTI =\
            currTTI
#else
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_ABSOLUTE_TTI(currTTI,internalCellIndex)
#endif

/* Number of UE in ReTxn Failuer Queue*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_UE_RETXN_FAILURE_QUEUE(count, internalCellIndex)\
        gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].numUeRetxnFailureQueue \
            = count
#else
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_UE_RETXN_FAILURE_QUEUE(count, internalCellIndex)
#endif

    /* Number of UE in Retxn Queue */
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_UE_RETXN_QUEUE(count, internalCellIndex)\
        gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].numUeRetxnQueue = \
        count
#else
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_UE_RETXN_QUEUE(count, internalCellIndex)
#endif

    /* Number of TA in Pending TA queue */
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_TA_FAILURE_QUEUE(count, internalCellIndex)\
        gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].numTAFailureQueue = \
        count
#else
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_TA_FAILURE_QUEUE(count, internalCellIndex)
#endif


    /* Number of TA in Pending TA queue */
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_TA_PENDING_QUEUE(count, internalCellIndex)\
        gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].numTAPendingQueue = \
        count
#else
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_TA_PENDING_QUEUE(count, internalCellIndex)
#endif

    /* Number of TA in TA Queue*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_TA_FRESH_QUEUE(count, internalCellIndex)\
        gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].numTAFreshQueue = \
        count
#else
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_TA_FRESH_QUEUE(count, internalCellIndex)
#endif
    

    /* Number of TA in TA Queue*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_TA_WITH_DATA(internalCellIndex)\
        gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].numTAWithData++;\
        gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].numTAFreshQueue-- 
#else
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_NUM_TA_WITH_DATA(internalCellIndex)
#endif
    

    /* Number of retxn scheduled */
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_SCHEDULED_RETXN_UE(count, internalCellIndex)\
        gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].scheduledReTXUE =\
        count
#else
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_SCHEDULED_RETXN_UE(count, internalCellIndex)
#endif

    /* Number of TA Scheduled */
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STAS_SCHEDULE_TA_UE(count, internalCellIndex)\
        gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].numTAPendingQueue = \
        count
#else
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_SCHEDULED_TA_UE(count, internalCellIndex)
#endif

    /* Number of New Txn scheduled */
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_SCHEDULED_NEW_TXN_UE(count, internalCellIndex)\
        gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].scheduledTxUE = \
        count
#else
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_SCHEDULED_NEW_TXN_UE(count, internalCellIndex)
#endif

    /* Increment Number of retxn scheduled */
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_INCREMENT_DL_STRATEGY_TTI_STATS_SCHEDULED_RETXN_UE(internalCellIndex)\
        ++gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].scheduledReTXUE
#else
#define LTE_MAC_INCREMENT_DL_STRATEGY_TTI_STATS_SCHEDULED_RETXN_UE(internalCellIndex)
#endif

    /* Number of TA Scheduled */
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_INCREMENT_DL_STRATEGY_TTI_STAS_SCHEDULE_TA_UE(internalCellIndex)\
        ++gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].scheduledTAUE;
#else
#define LTE_MAC_INCREMENT_DL_STRATEGY_TTI_STAS_SCHEDULE_TA_UE(internalCellIndex)
#endif

    /* Number of New Txn scheduled */
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_INCREMENT_DL_STRATEGY_TTI_STATS_SCHEDULED_NEW_TXN_UE(internalCellIndex)\
        ++gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].scheduledTxUE 
#else
#define LTE_MAC_INCREMENT_DL_STRATEGY_TTI_STATS_SCHEDULED_NEW_TXN_UE(internalCellIndex)
#endif

    /* Increment number of Txn scheduled */
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_INCREMENT_DL_STRATEGY_TTI_STATS_SCHEDULED_TOTAL_UE(internalCellIndex)\
        ++gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].scheduledUETotal;
#else
#define LTE_MAC_INCREMENT_DL_STRATEGY_TTI_STATS_SCHEDULED_TOTAL_UE(internalCellIndex)
#endif


    /*DL strategy per TTI stats end */

    /*DL strategy per UE per TTI*/

    /*Scheduled Tx UE*/
#ifdef SSI_DEBUG_STATS
#define DL_SCHEDULED_TOTAL_UE(internalCellIndex) \
        gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].scheduledUETotal
#endif


    /*DL strategy per UE per TTI*/

    /*max TB size*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_PER_UE_SCHEDULED\
(ueDLContext_p, harqProcess_p, dlStrategyTxNode_p, internalCellIndex) \
{\
    UInt16 ueIndex = ueDLContext_p->ueIndex;\
    LteMacDlStrategyPerTTIPerUEStats *dlStrategyPerUE_p = \
    &(gMacStats.pStats->dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].\
            dlStrategyPerUE[DL_SCHEDULED_TOTAL_UE(internalCellIndex)]);\
    \
    ULUEContext *ueULContext_p = ulUECtxInfoArr_g[ueIndex].ulUEContext_p;\
    \
    dlStrategyPerUE_p->ueIndex = ueIndex;\
    \
    dlStrategyPerUE_p->minTBSize = dlStrategyTxNode_p->minTBSize;\
    \
    dlStrategyPerUE_p->maxTBSize = dlStrategyTxNode_p->maxTBSize;\
    \
    dlStrategyPerUE_p->scheduledRB = harqProcess_p->assignedResourcesNB;\
    \
    dlStrategyPerUE_p->recvdRI = ueULContext_p->pucchConfigInfo.ueReportedRI;\
    \
    dlStrategyPerUE_p->transmissionMode = (ueDLContext_p->dlMIMOInfo).transmissionMode;\
    \
    dlStrategyPerUE_p->transmissionType = dlStrategyTxNode_p->transmissionType;\
    \
    dlStrategyPerUE_p->scheduleLcCount = ueDLContext_p->scheduledLcCount;\
    \
    dlStrategyPerUE_p->eligibleLcCount = (UInt8)listCount(&ueDLContext_p->dlEligibleLcList);\
    \
    dlStrategyPerUE_p->harqProcessId = dlStrategyTxNode_p->harqProcessId;\
    \
    dlStrategyPerUE_p->aperodicCQIFlag = dlStrategyTxNode_p->isCQIRequestedULGrantMsg;\
    \
    dlStrategyPerUE_p->ueWbMCS = harqProcess_p->dlHARQTBOneInfo.mcsIndex;\
    dlStrategyPerUE_p->isTBOneValid = harqProcess_p->isTBOneValid;  \
    dlStrategyPerUE_p->isTBTwoValid = harqProcess_p->isTBTwoValid;        \
    dlStrategyPerUE_p->primaryDCIFormat = dlStrategyTxNode_p->primaryDCIFormat; \
    dlStrategyPerUE_p->secondaryDCIFormat = dlStrategyTxNode_p->secondaryDCIFormat;\
    \
    if(NEW_TX == dlStrategyTxNode_p->transmissionType)\
    {\
        LTE_MAC_INCREMENT_DL_STRATEGY_TTI_STATS_SCHEDULED_NEW_TXN_UE(internalCellIndex); \
        LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_NEW_TX_COUNT(ueIndex);\
    }\
    else\
    {\
        LTE_MAC_INCREMENT_DL_STRATEGY_TTI_STATS_SCHEDULED_RETXN_UE(internalCellIndex);\
        LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_RE_TX_COUNT(ueIndex);\
    }\
    \
    \
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_STRATEGY_UE_SCHEDULED_PER_TTI_UE_POSTION\
    (ueDLContext_p->ueIndex, gMacStats.pStats->\
     dlStrategyTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].absoluteTTI, \
     DL_SCHEDULED_TOTAL_UE(internalCellIndex), internalCellIndex)\
    \
    INCREMENT_DL_STRATEGY_WRITE_INDEX(ueIndex, internalCellIndex)\
    \
    LTE_MAC_INCREMENT_DL_STRATEGY_TTI_STATS_SCHEDULED_TOTAL_UE(internalCellIndex);\
}
#else
#define LTE_MAC_UPDATE_DL_STRATEGY_TTI_STATS_PER_UE_SCHEDULED(a,b,c, d)
#endif



    /*DL Strategy per LC per TTI start*/
    /*Number of scheduled LC*/
#ifdef SSI_DEBUG_STATS
#define DL_NUM_SCHEDULED_LC(internalCellIndex) \
        gMacStats.pStats->dlStrategyLcTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].numScheduledLC
#endif

#ifdef SSI_DEBUG_STATS
#define INCREMENT_DL_NUM_SCHEDULED_LC(internalCellIndex) \
        ++DL_NUM_SCHEDULED_LC(internalCellIndex);\
        if(MAX_STRATEGY_LC_SCHEDULED == DL_NUM_SCHEDULED_LC(internalCellIndex))\
        {\
            DL_NUM_SCHEDULED_LC(internalCellIndex) = 0;\
        }  
#endif


    /* Per LC Stat Info */
#ifdef SSI_DEBUG_STATS
/* Compilation 10-JUN-2014 fix start */
#define LTE_MAC_UPDATE_DL_STRATEGY_LC_TTI_STATS_PER_LC\
(ueIdx, currentGlobalTick, dlQosStrategyLCNode_p, lcInfo_p, dlLcQosInfo_p, internalCellIndex) \
{\
    LteQosDLStrategyLCTTIPerLcStat *dlStrategyPerLC_p = PNULL;\
    gMacStats.pStats->dlStrategyLcTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].\
            absoluteTTI = currentGlobalTick;\
    dlStrategyPerLC_p = \
    &(gMacStats.pStats->dlStrategyLcTTIStats[(internalCellIndex)][WRITE_TTI_INDEX(internalCellIndex)].\
            dlStrategyPerLC[DL_NUM_SCHEDULED_LC(internalCellIndex)]);\
    \
    dlStrategyPerLC_p->ueIndex = ueIdx;\
    \
    dlStrategyPerLC_p->lcId = lcInfo_p->logicalChannelId;\
    \
    gMacStats.pStats->ueCommonInfoStats[ueIdx].ueInfoStat.dlLCInfoStat[dlStrategyPerLC_p->lcId].lcPos = DL_NUM_SCHEDULED_LC(internalCellIndex);\
    dlStrategyPerLC_p->qLoad = lcInfo_p->queueLoad;\
    \
    dlStrategyPerLC_p->scheduledBytes = dlQosStrategyLCNode_p->schedulableBytes;\
    \
    dlStrategyPerLC_p->resheduledBytes = 0;\
    \
    dlStrategyPerLC_p->headOfLineDelay = /*currentGlobalTick - dlLcQosInfo_p->pktArrivalTS*/ 0 ;\
    \
    dlStrategyPerLC_p->availToken = dlLcQosInfo_p->availToken ;\
    \
    dlStrategyPerLC_p->totalLcWeight = dlQosStrategyLCNode_p->totLCWeight;\
    \
    dlStrategyPerLC_p->totTknSchedWndw = dlLcQosInfo_p->totSchdTknInWindow;\
\
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_STRATEGY_LC_SCHEDULED_PER_TTI_ABSOLUTE_TTI(ueIdx, currentGlobalTick);\
    \
    LTE_MAC_UPDATE_UE_COMMON_INFO_UE_INFO_STAT_DL_STRATEGY_LC_SCHEDULED_PER_TTI_LC_INDEX(ueIdx, DL_NUM_SCHEDULED_LC(internalCellIndex))\
\
    INCREMENT_DL_STRATEGY_LC_WRITE_INDEX(ueIdx)\
   \
    INCREMENT_DL_NUM_SCHEDULED_LC(internalCellIndex)\
}
/* Compilation 10-JUN-2014 fix end */
#else
#define LTE_MAC_UPDATE_DL_STRATEGY_LC_TTI_STATS_PER_LC(a,b,c,d,e,f);
#endif

    /*ReScheduled Bytes*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_DL_STRATEGY_LC_TTI_STATS_PER_LC_RESCHEDULED_BYTES(value1,value2,value3, value4)\
        gMacStats.pStats->dlStrategyLcTTIStats[(value4)][WRITE_TTI_INDEX(value4)].\
    dlStrategyPerLC[gMacStats.pStats->ueCommonInfoStats[value1].ueInfoStat.dlLCInfoStat[value2].lcPos].resheduledBytes = value3
#else
#define LTE_MAC_UPDATE_DL_STRATEGY_LC_TTI_STATS_PER_LC_RESCHEDULED_BYTES(value1,value2,value3, value4)
#endif

    /* Update Scheduled Bytes*/
#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_DL_STRATEGY_LC_TTI_STATS_PER_LC_SCHEDULED_BYTES(value1,value2,value3, value4)\
        gMacStats.pStats->dlStrategyLcTTIStats[(value4)][WRITE_TTI_INDEX(value4)].\
    dlStrategyPerLC[gMacStats.pStats->ueCommonInfoStats[value1].ueInfoStat.dlLCInfoStat[value2].lcPos].scheduledBytes = value3
#else
#define LTE_MAC_UPDATE_DL_STRATEGY_LC_TTI_STATS_PER_LC_SCHEDULED_BYTES(value1,value2,value3, value4)
#endif
/* CA Stats Changes End */


/*DL strategy changes stop*/
 /* + KPI_CA */
#ifdef KPI_STATS
#define LTE_MAC_UPDATE_STATS_COMM_KPI(totalUplinkPRBUsage, activeIndexForMacStats,internalCellIndex) \
{\
    if(PNULL != gMacStats.pStats)\
    {\
        gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats].\
        totalUplinkPRBUsage += totalUplinkPRBUsage;\
        gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats].totalUplinkTTI++;\
    }\
}
#else
#define LTE_MAC_UPDATE_STATS_COMM_KPI(totalUplinkPRBUsage, activeIndexForMacStats,internalCellIndex) 
#endif
 /* - KPI_CA */

UInt64 getTimeInMicroSec(TimeSpec *  start, TimeSpec *  end);

/* SPS stats */
#ifdef SSI_DEBUG_STATS
/* CA Stats Changes Start */
#define LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCxt_p, currTick, Cause, dciAttempts, internalCellIndex){\
    LP_SpsDlInfo spsInfo_p = &(dlUeCxt_p->spsDlInfo); \
    UInt16 writeTTIIndex = gMacStats.pStats->writeTTIIndex[(internalCellIndex)]; \
    LP_SpsPerUePerTTIStats spsTtiStats_p = \
      &(gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].qoSUELCStat[ueIndex].\
                                                spsTTIStats[writeTTIIndex]);\
    spsTtiStats_p->absoluteTTI = currTick + 1;\
    spsTtiStats_p->isSpsPerUePerTTIStatsValid = TRUE;\
    spsTtiStats_p->spsState = spsInfo_p->spsState;\
    spsTtiStats_p->spsLcId = spsInfo_p->lcId;\
    spsTtiStats_p->cause = (Cause);\
    spsTtiStats_p->spsQload = spsInfo_p->headPktSize ;\
    spsTtiStats_p->schType = dlUeCxt_p->schType;\
    spsTtiStats_p->currMcs = dlUeCxt_p->dlCQIInfo.mcsIndexCodeWordOne;\
    spsTtiStats_p->spsMcs = spsInfo_p->resAllocInfo.allocatedMCS;\
    spsTtiStats_p->ttiIndex = spsInfo_p->ttiIndex;\
    spsTtiStats_p->numOfSpsEmptyTx = spsInfo_p->numOfSpsEmptyTx; \
    spsTtiStats_p->numRBs = spsInfo_p->resAllocInfo.allocatedRB;\
    spsTtiStats_p->numDciAttempts = dciAttempts; \
}

#define LTE_MAC_UPDATE_HARQ_PROCESS_ID_USED( ueIndex, hpId, internalCellIndex){\
    UInt16 writeTTIIndex = gMacStats.pStats->writeTTIIndex[(internalCellIndex)]; \
    LP_SpsPerUePerTTIStats spsTtiStats_p = \
      &(gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].qoSUELCStat[ueIndex].\
                                                spsTTIStats[writeTTIIndex]);\
    spsTtiStats_p->isSpsPerUePerTTIStatsValid = TRUE;\
    spsTtiStats_p->harqProcessId = hpId;\
}

#define LTE_MAC_INCREMENT_SPS_HARQ_STATS( ueIndex, dlUeCxt_p, currTick,schType, cause ){\
    UInt8 internalCellIndex = dlUeCxt_p->internalCellIndex;\
    UInt16 writeTTIIndex = gMacStats.pStats->writeTTIIndex[internalCellIndex]; \
     LP_SpsPerUeHarqPerTTIStats spsPerUeHarqPerTTIStats_p = \
      &(gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].qoSUELCStat[ueIndex].\
                                                spsPerUeHarqPerTTIStats[writeTTIIndex]);\
    spsPerUeHarqPerTTIStats_p->absoluteTTI = currTick + 1; \
    spsPerUeHarqPerTTIStats_p->isSpsPerUeHarqPerTTIStatsValid = TRUE; \
    if(SPS_SCHEDULING == schType) {\
        if(NEW_TX == cause)\
        {\
            spsPerUeHarqPerTTIStats_p->numOfBusySpsHarqProcessForNewTx++ ;\
        }\
        else \
        {\
            spsPerUeHarqPerTTIStats_p->numOfBusySpsHarqProcessForReTx++ ; \
        }\
    } else {\
        if(NEW_TX == cause)\
        {\
            spsPerUeHarqPerTTIStats_p->numOfBusyDRAHarqProcessForNewTx++ ;\
        }\
        else \
        {\
            spsPerUeHarqPerTTIStats_p->numOfBusyDRAHarqProcessForReTx++ ; \
        }\
    }\
}
#define LTE_MAC_PER_UE_DEBUG_STATS(ueId, currTick, hpId, ackNack, txNB, internalCellIndex)\
{\
    UInt16 writeTTIIndex = gMacStats.pStats->writeTTIIndex[(internalCellIndex)]; \
    LP_SpsDebugPerUePerTTIStats spsDebugPerUePerTTIStats_p = \
    &(gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].qoSUELCStat[ueId].\
            spsDebugPerUePerTTIStats[writeTTIIndex]);\
    spsDebugPerUePerTTIStats_p->absoluteTTI = currTick + 1;\
    spsDebugPerUePerTTIStats_p->isSpsDebugPerUePerTTIStatsValid = TRUE;\
    spsDebugPerUePerTTIStats_p->harqProcessId = hpId;\
    spsDebugPerUePerTTIStats_p->ackNackReceived =  ackNack ;\
    spsDebugPerUePerTTIStats_p->current_TX_NB = txNB; \
} 
#define LTE_MAC_UPDATE_DCI_ATTEMPT_STATS(ueIndex, dciAttempts, internalCellIndex){\
    UInt16 writeTTIIndex = gMacStats.pStats->writeTTIIndex[(internalCellIndex)]; \
    LP_SpsPerUePerTTIStatsUL spsUlTtiStats_p = \
      &(gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].qoSUELCStat[ueIndex].\
                                                spsTTIStatsUl[writeTTIIndex]);\
    spsUlTtiStats_p->numDciAttempts = dciAttempts; \
}
#define LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, currTick,Cause, dciAttempts, internalCellIndex ){\
    LP_SpsUlInfo spsInfo_p = &(ulUeCtx_p->spsUlInfo); \
    UInt16 writeTTIIndex = gMacStats.pStats->writeTTIIndex[(internalCellIndex)]; \
    UInt8 lcgId = ulUeCtx_p->lcInfo[spsInfo_p->lcId].lcGId; \
    LP_SpsPerUePerTTIStatsUL spsUlTtiStats_p = \
      &(gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].qoSUELCStat[ueIndex].\
                                                spsTTIStatsUl[writeTTIIndex]);\
    spsUlTtiStats_p->absoluteTTI = currTick + 1;\
    spsUlTtiStats_p->isSpsPerUePerTTIStatsULValid = TRUE;\
    spsUlTtiStats_p->spsState = spsInfo_p->spsState;\
    spsUlTtiStats_p->spsLcId = spsInfo_p->lcId;\
    spsUlTtiStats_p->cause = (Cause);\
    spsUlTtiStats_p->spsUlLcgQload = ulUeCtx_p->ulLCGInfo[lcgId].qLoad ;\
    spsUlTtiStats_p->schType = ulUeCtx_p->schType;\
    spsUlTtiStats_p->currMcs = ulUeCtx_p->mcsIndex;\
    spsUlTtiStats_p->spsMcs = spsInfo_p->ulResAllocInfo.allocatedMCS;\
    spsUlTtiStats_p->ttiIndex = spsInfo_p->ttiIndex;\
    spsUlTtiStats_p->avgRlcSduSize = spsInfo_p->avgRlcSduSize;\
    spsUlTtiStats_p->numOfSureSpsEmptyTx = spsInfo_p->numOfSureSpsEmptyTx; \
    spsUlTtiStats_p->numOfUnsureSpsEmptyTx = spsInfo_p->numOfUnsureSpsEmptyTx; \
    spsUlTtiStats_p->numRBs = spsInfo_p->ulResAllocInfo.allocatedRB;\
    spsUlTtiStats_p->numDciAttempts = dciAttempts; \
}
#define LTE_MAC_UPDATE_UL_HARQ_PROCESS_ID_USED( ueIndex, hpId, internalCellIndex){\
    UInt16 writeTTIIndex = gMacStats.pStats->writeTTIIndex[(internalCellIndex)]; \
    LP_SpsPerUePerTTIStatsUL spsTtiStats_p = \
      &(gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].qoSUELCStat[ueIndex].\
                                                spsTTIStatsUl[writeTTIIndex]);\
    spsTtiStats_p->isSpsPerUePerTTIStatsULValid = TRUE;\
    spsTtiStats_p->harqProcessId = hpId;\
}
#define LTE_MAC_UL_PER_UE_DEBUG_STATS(ueId, currTick, hpId,schType,  ackNack, txNB, internalCellIndex)\
{\
    UInt16 writeTTIIndex = gMacStats.pStats->writeTTIIndex[(internalCellIndex)]; \
    LP_SpsDebugPerUePerTTIStatsUL spsDebugUlPerUePerTTIStats_p = \
    &(gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].qoSUELCStat[ueId].\
            spsDebugPerUePerTTIStatsUl[writeTTIIndex]);\
    spsDebugUlPerUePerTTIStats_p->absoluteTTI = currTick + 1;\
    spsDebugUlPerUePerTTIStats_p->isSpsDebugPerUePerTTIStatsULValid = TRUE;\
    spsDebugUlPerUePerTTIStats_p->harqProcessId = hpId;\
    spsDebugUlPerUePerTTIStats_p->crcReceived =  ackNack ;\
    spsDebugUlPerUePerTTIStats_p->current_TX_NB = txNB; \
    spsDebugUlPerUePerTTIStats_p->schUsed= schType; \
} 

#else
#define LTE_MAC_UPDATE_SPS_DL_STR_STATS( ueIndex, dlUeCxt_p, currTick, Cause, dciAttempts, internalCellIndex)
#define LTE_MAC_UPDATE_HARQ_PROCESS_ID_USED( ueIndex, hpId , internalCellIndex)
#define LTE_MAC_INCREMENT_SPS_HARQ_STATS( ueIndex, dlUeCxt_p, currTick, schType, cause );
#define LTE_MAC_PER_UE_DEBUG_STATS(ueId, currTick, hpId, ackNack, txNB, internalCellIndex);
#define LTE_MAC_UPDATE_SPS_UL_STR_STATS(ueIndex, ulUeCtx_p, currTick,Cause, dciAttempts, internalCellIndex );
#define LTE_MAC_UPDATE_DCI_ATTEMPT_STATS(ueIndex, dciAttempts, internalCellIndex);
#define LTE_MAC_UPDATE_UL_HARQ_PROCESS_ID_USED( ueIndex, hpId, internalCellIndex);
#define LTE_MAC_UL_PER_UE_DEBUG_STATS(ueId, currTick, hpId, schType, ackNack, txNB, internalCellIndex);
#endif

#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_STATS_FOR_SPS_PDSCH_RESERVE( dlUeCxt_p , currentTick, internalCellIndex){\
    LP_SpsDlInfo spsInfo_p = &(dlUeCxt_p->spsDlInfo); \
    UInt16 writeTTIIdx = gMacStats.pStats->writeTTIIndex[(internalCellIndex)]; \
    gMacStats.pStats->pdschTTIStats[(internalCellIndex)][writeTTIIdx].absoluteTTI = currentTick+1;\
    gMacStats.pStats->pdschTTIStats[(internalCellIndex)][writeTTIIdx].isPdschTTIStatsValid = TRUE;\
    UInt16 numSchedUETotal = gMacStats.pStats->pdschTTIStats[(internalCellIndex)][writeTTIIdx].\
                                                       numScheduledUETotal; \
    gMacStats.pStats->pdschTTIStats[(internalCellIndex)][writeTTIIdx].\
        pdschStatsPerScheduledUE[numSchedUETotal].allocatedSPSRBs = spsInfo_p->resAllocInfo.allocatedRB; \
    gMacStats.pStats->pdschTTIStats[(internalCellIndex)][writeTTIIdx].\
         pdschStatsPerScheduledUE[numSchedUETotal].startSPSVRBIndex=spsInfo_p->resAllocInfo.startingVRBIndex; \
    gMacStats.pStats->pdschTTIStats[(internalCellIndex)][writeTTIIdx].\
         pdschStatsPerScheduledUE[numSchedUETotal].allocatedSPSMcs= spsInfo_p->resAllocInfo.allocatedMCS; \
}

#define LTE_MAC_UPDATE_SPS_PDSCH_TTI_MAP_STATS( spsInfo_p, ttiCtx_p, ttiCtx20_p, currentTick, internalCellIndex){\
    UInt8 spsIntDL = \
       spsInfo_p->spsDlSetupInfo_p->semiPersistentSchedIntervalDL;\
    UInt8 ttiIdx = ttiCtx_p->index; \
    UInt16 writeTTIIdx = gMacStats.pStats->writeTTIIndex[(internalCellIndex)]; \
    UInt16 lastWrtIdx = gMacStats.pStats->lastWriteIdx[(internalCellIndex)]; \
    LP_LteSpsDlRmStats ttiStats_p = \
          &gMacStats.pStats->dlSpsRmStats[(internalCellIndex)][writeTTIIdx];\
    LP_LteSpsDlRmStats prevTtiStats_p = \
          &gMacStats.pStats->dlSpsRmStats[(internalCellIndex)][lastWrtIdx];\
    ttiStats_p->absoluteTTI = currentTick + 1;\
    ttiStats_p->isDlSpsRmStatsValid = TRUE;\
    ttiStats_p->dlSpsRbsPerTTI[ttiIdx] = ttiCtx_p->numDlSpsRbsPerTti; \
    ttiStats_p->dlSpsRbsPerTTIPerInterval[ttiIdx][spsIntDL] = \
         ttiCtx_p->numSpsRbsPerTtiPerInterval[spsIntDL] ;\
    ttiStats_p->dlSpsDciPerTTI[ttiIdx] = \
         prevTtiStats_p->dlSpsDciPerTTI[ttiIdx];\
    ttiStats_p->dlSpsOccasionPerTTI[ttiIdx] = \
         prevTtiStats_p->dlSpsOccasionPerTTI[ttiIdx];\
    if( SPS_INTERVAL_20 == spsIntervalMap_g[spsIntDL] ) \
    { \
        ttiIdx = ttiCtx20_p->index; \
        ttiStats_p->dlSpsRbsPerTTI[ttiIdx] = ttiCtx20_p->numDlSpsRbsPerTti; \
        ttiStats_p->dlSpsRbsPerTTIPerInterval[ttiIdx][spsIntDL] = \
            ttiCtx20_p->numSpsRbsPerTtiPerInterval[spsIntDL] ;\
        ttiStats_p->dlSpsDciPerTTI[ttiIdx] = \
            prevTtiStats_p->dlSpsDciPerTTI[ttiIdx];\
        ttiStats_p->dlSpsOccasionPerTTI[ttiIdx] = \
            prevTtiStats_p->dlSpsOccasionPerTTI[ttiIdx];\
    } \
    gMacStats.pStats->lastWriteIdx[(internalCellIndex)] = writeTTIIdx; \
}

#define LTE_MAC_UPDATE_DL_SPS_TTI_MAP_STATS( ttiCtx_p, ttiCtx20_p, \
                                        currentTick, spsIntDL, internalCellIndex){\
    UInt16 writeTTIIdx = gMacStats.pStats->writeTTIIndex[(internalCellIndex)]; \
    UInt16 lastWrtIdx = gMacStats.pStats->lastWriteIdx[(internalCellIndex)]; \
    UInt8 idx = ttiCtx_p->index; \
    LP_LteSpsDlRmStats ttiStats_p = \
          &gMacStats.pStats->dlSpsRmStats[(internalCellIndex)][writeTTIIdx];\
    LP_LteSpsDlRmStats prevTtiStats_p = \
          &gMacStats.pStats->dlSpsRmStats[(internalCellIndex)][lastWrtIdx];\
    ttiStats_p->absoluteTTI = currentTick + 1;\
    ttiStats_p->isDlSpsRmStatsValid = TRUE;\
    ttiStats_p->dlSpsDciPerTTI[idx] = ttiCtx_p->numDlSpsDciPerTti; \
    ttiStats_p->dlSpsOccasionPerTTI[idx] = ttiCtx_p->numDlSpsOccasionPerTti; \
    ttiStats_p->dlSpsRbsPerTTI[idx] = prevTtiStats_p->dlSpsRbsPerTTI[idx];\
    memCpy( ttiStats_p->dlSpsRbsPerTTIPerInterval[idx], \
            prevTtiStats_p->dlSpsRbsPerTTIPerInterval[idx], 10 ); \
    if( SPS_INTERVAL_20 == spsIntervalMap_g[spsIntDL] ) \
    { \
        idx = ttiCtx20_p->index; \
        ttiStats_p->dlSpsDciPerTTI[idx] = ttiCtx20_p->numDlSpsDciPerTti; \
        ttiStats_p->dlSpsOccasionPerTTI[idx] = \
              ttiCtx20_p->numDlSpsOccasionPerTti; \
        ttiStats_p->dlSpsRbsPerTTI[idx] = prevTtiStats_p->dlSpsRbsPerTTI[idx];\
        memCpy( ttiStats_p->dlSpsRbsPerTTIPerInterval[idx], \
              prevTtiStats_p->dlSpsRbsPerTTIPerInterval[idx], 10 ); \
    }\
    gMacStats.pStats->lastWriteIdx[(internalCellIndex)] = writeTTIIdx; \
}

#define LTE_MAC_UPDATE_UL_SPS_TTI_MAP_STATS( ttiCtx_p, ttiCtx20_p, \
                                        currentTick, spsIntUL, internalCellIndex){\
    UInt16 writeTTIIdx = gMacStats.pStats->writeTTIIndex[(internalCellIndex)]; \
    UInt16 lastWrtIdx = gMacStats.pStats->lastUlWriteIdx[(internalCellIndex)]; \
    UInt8 idx = ttiCtx_p->index; \
    LP_LteSpsUlRmStats ttiUlStats_p = \
          &gMacStats.pStats->ulSpsRmStats[(internalCellIndex)][writeTTIIdx];\
    LP_LteSpsUlRmStats prevTtiUlStats_p = \
          &gMacStats.pStats->ulSpsRmStats[(internalCellIndex)][lastWrtIdx];\
    ttiUlStats_p->absoluteTTI = currentTick + 1;\
    ttiUlStats_p->isUlSpsRmStatsValid = TRUE;\
    ttiUlStats_p->ulSpsDciPerTTI[idx] = ttiCtx_p->numULSpsDciPerTti; \
    ttiUlStats_p->ulSpsOccasionPerTTI[idx] = ttiCtx_p->numULSpsOccasionPerTti; \
    ttiUlStats_p->ulSpsRbsPerTTI[idx] = ttiUlStats_p->ulSpsRbsPerTTI[idx];\
    memCpy( ttiUlStats_p->ulSpsRbsPerTTIPerInterval[idx], \
            prevTtiUlStats_p->ulSpsRbsPerTTIPerInterval[idx], 10 ); \
    if( SPS_INTERVAL_20 == spsIntervalMap_g[spsIntUL] ) \
    { \
        idx = ttiCtx20_p->index; \
        ttiUlStats_p->ulSpsDciPerTTI[idx] = ttiCtx20_p->numULSpsDciPerTti; \
        ttiUlStats_p->ulSpsOccasionPerTTI[idx] = \
              ttiCtx20_p->numULSpsOccasionPerTti; \
        ttiUlStats_p->ulSpsRbsPerTTI[idx] = prevTtiUlStats_p->ulSpsRbsPerTTI[idx];\
        memCpy( ttiUlStats_p->ulSpsRbsPerTTIPerInterval[idx], \
              prevTtiUlStats_p->ulSpsRbsPerTTIPerInterval[idx], 10 ); \
    }\
    gMacStats.pStats->lastUlWriteIdx[(internalCellIndex)] = writeTTIIdx; \
}

#define LTE_MAC_UPDATE_STATS_FOR_SPS_PUSCH_RESERVE( ulUeCxt_p , currentTick, internalCellIndex ){\
    LP_SpsUlInfo spsInfo_p = &(ulUeCxt_p->spsUlInfo); \
    UInt16 writeTTIIdx = gMacStats.pStats->writeTTIIndex[(internalCellIndex)]; \
    gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIdx].absoluteTTI = currentTick+1;\
    gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIdx].isPuschTTIStatsValid = TRUE;\
    UInt16 numSchedUETotal = gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIdx].\
                                                       numScheduledUETotal; \
    gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIdx].\
        puschStatsPerScheduledUE[numSchedUETotal -1].allocatedSPSRBs = spsInfo_p->ulResAllocInfo.allocatedRB; \
    gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIdx].\
         puschStatsPerScheduledUE[numSchedUETotal -1].startSPSVRBIndex=spsInfo_p->ulResAllocInfo.startingVRBIndex; \
    gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIdx].\
         puschStatsPerScheduledUE[numSchedUETotal -1].allocatedSPSMcs= spsInfo_p->ulResAllocInfo.allocatedMCS; \
    gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIdx].\
         puschStatsPerScheduledUE[numSchedUETotal -1].spsResourceAllocationBitMap= spsInfo_p->ulResAllocInfo.allocatedRiv; \
}

#define LTE_MAC_UPDATE_SPS_PUSCH_TTI_MAP_STATS( spsInfo_p, ttiCtx_p, ttiCtx20_p, currentTick, internalCellIndex){\
    UInt8 spsIntUL = \
       spsInfo_p->spsUlSetupInfo_p->semiPersistentSchedIntervalUL;\
    UInt8 ttiIdx = ttiCtx_p->index; \
    UInt16 writeTTIIdx = gMacStats.pStats->writeTTIIndex[(internalCellIndex)]; \
    UInt16 lastWrtIdx = gMacStats.pStats->lastWriteIdx[(internalCellIndex)]; \
    LP_LteSpsUlRmStats ttiUlStats_p = \
          &gMacStats.pStats->ulSpsRmStats[(internalCellIndex)][writeTTIIdx];\
    LP_LteSpsUlRmStats prevTtiUlStats_p = \
          &gMacStats.pStats->ulSpsRmStats[(internalCellIndex)][lastWrtIdx];\
    ttiUlStats_p->absoluteTTI = currentTick + 1;\
    ttiUlStats_p->ulSpsRbsPerTTI[ttiIdx] = ttiCtx_p->numULSpsRbsPerTti; \
    ttiUlStats_p->ulSpsRbsPerTTIPerInterval[ttiIdx][spsIntUL] = \
         ttiCtx_p->numULSpsRbsPerTtiPerInterval[spsIntUL] ;\
    ttiUlStats_p->ulSpsDciPerTTI[ttiIdx] = \
         prevTtiUlStats_p->ulSpsDciPerTTI[ttiIdx];\
    ttiUlStats_p->ulSpsOccasionPerTTI[ttiIdx] = \
         prevTtiUlStats_p->ulSpsOccasionPerTTI[ttiIdx];\
    if( SPS_INTERVAL_20 == spsIntervalMap_g[spsIntUL] ) \
    { \
        ttiIdx = ttiCtx20_p->index; \
        ttiUlStats_p->ulSpsRbsPerTTI[ttiIdx] = ttiCtx20_p->numULSpsRbsPerTti; \
        ttiUlStats_p->ulSpsRbsPerTTIPerInterval[ttiIdx][spsIntUL] = \
            ttiCtx20_p->numULSpsRbsPerTtiPerInterval[spsIntUL] ;\
        ttiUlStats_p->ulSpsDciPerTTI[ttiIdx] = \
            prevTtiUlStats_p->ulSpsDciPerTTI[ttiIdx];\
        ttiUlStats_p->ulSpsOccasionPerTTI[ttiIdx] = \
            prevTtiUlStats_p->ulSpsOccasionPerTTI[ttiIdx];\
    } \
    gMacStats.pStats->lastWriteIdx[(internalCellIndex)] = writeTTIIdx; \
}
#define LTE_MAC_QUEUE_USED( queueName, qCnt, internalCellIndex ){\
    UInt16 writeTTIIdx = gMacStats.pStats->writeTTIIndex[(internalCellIndex)]; \
    LP_LteSpsQueuesUsed spsQueueUsed_p = \
         &(gMacStats.pStats->spsQueueUsed[(internalCellIndex)][writeTTIIdx]); \
    spsQueueUsed_p->qName = (queueName); \
    spsQueueUsed_p->qCount = qCnt; \
}

/* ATB stats Changes */
#define LTE_MAC_UPDATE_STATS_FOR_ICIC_SFR(ulUEContext_p, currentTick,maxPower,totalUePower,mcs,inputRBs, internalCellIndex){\
    UInt16 writeTTIIdx = gMacStats.pStats->writeTTIIndex[(internalCellIndex)]; \
    gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIdx].absoluteTTI = currentTick+1;\
    gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIdx].isPuschTTIStatsValid = TRUE;\
    UInt16 numSchedUETotal = gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIdx].\
                                                       numScheduledUETotal; \
    gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIdx].\
        puschStatsPerScheduledUE[numSchedUETotal].currentUePower = totalUePower;\
    gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIdx].\
        puschStatsPerScheduledUE[numSchedUETotal].maxQciDeltaPowerMask = ulUEContext_p->maxQciDeltaPowerMask;\
    gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIdx].\
        puschStatsPerScheduledUE[numSchedUETotal].maxPower = maxPower;\
    gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIdx].\
        puschStatsPerScheduledUE[numSchedUETotal].inputMCSForATB = mcs;\
    gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIdx].\
        puschStatsPerScheduledUE[numSchedUETotal].inputRBForATB = inputRBs;\
    gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIdx].\
        puschStatsPerScheduledUE[numSchedUETotal].currPowerCorrectionForPusch = ulUEContext_p->currPowerCorrectionForPusch;\
    gMacStats.pStats->puschTTIStats[(internalCellIndex)][writeTTIIdx].\
        puschStatsPerScheduledUE[numSchedUETotal].pathLoss = ulUEContext_p->pathLoss;\
}

#else
#define LTE_MAC_UPDATE_STATS_FOR_SPS_PDSCH_RESERVE( a, b, c )
#define LTE_MAC_UPDATE_STATS_FOR_SPS_PUSCH_RESERVE( ulUeCxt_p , currentTick , internalCellIndex);
#define LTE_MAC_UPDATE_SPS_PDSCH_TTI_MAP_STATS( spsInfo_p, ttiCtx_p, \
                                        ttiCtx20_p, currentTick, internalCellIndex)
#define LTE_MAC_UPDATE_SPS_PUSCH_TTI_MAP_STATS( spsInfo_p, ttiCtx_p, \
                                        ttiCtx20_p, currentTick, internalCellIndex)
#define LTE_MAC_UPDATE_DL_SPS_TTI_MAP_STATS( ttiCtx_p, ttiCtx20_p, \
                                        currentTick, spsIntDL, internalCellIndex)
#define LTE_MAC_UPDATE_UL_SPS_TTI_MAP_STATS( ttiCtx_p, ttiCtx20_p, \
                                        currentTick, spsIntUL, internalCellIndex);
#define LTE_MAC_QUEUE_USED( queueName, qCount, internalCellIndex )
/* ATB stats Changes */
#define LTE_MAC_UPDATE_STATS_FOR_ICIC_SFR(ulUEContext_p,currentTick ,maxPower,totalPower,mcs,inputRBs, internalCellIndex)
/* CA Stats Changes End */
#endif

#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_DL_DRX_MEAS_GAP_TTI( ueId, ueCtx_p ){\
    LP_SpsDlInfo dlSpsInfo_p = &(ueCtx_p->spsDlInfo); \
    LP_LteMacDlSpsUeSpecificStats dlSpsUeInfoStats_p = \
        &gMacStats.pStats->ueCommonInfoStats[ueId].dlSpsUeInfoStats;\
    memCpy( dlSpsUeInfoStats_p->ueDrxTTI, dlSpsInfo_p->ueDrxTTI, \
            MAX_DL_SPS_INTERVAL); \
    memCpy( dlSpsUeInfoStats_p->ueMeasGapTTI, dlSpsInfo_p->ueMeasGapTTI, \
            MAX_DL_SPS_INTERVAL); \
}

#define LTE_MAC_UPDATE_DOWNLINK_SPS_UE_EXIST_STATE( ueId, isUeAct){\
    LteMacDlSpsUeSpecificStats *dlSpsUeInfoStats_p = PNULL; \
    dlSpsUeInfoStats_p= &gMacStats.pStats->\
                              ueCommonInfoStats[ueId].dlSpsUeInfoStats;\
    if( LTE_TRUE == isUeAct ){\
        dlSpsUeInfoStats_p->isUeActive = isUeAct;\
    }\
    else\
    {\
        dlSpsUeInfoStats_p->isUeActive = isUeAct;\
        dlSpsUeInfoStats_p->ipPktType = IP_NOT_DETERMINED;\
        dlSpsUeInfoStats_p->ueIndex = ueId;\
        dlSpsUeInfoStats_p->rohcProfileUsed = MAX_ROHC_PROFILE;\
        dlSpsUeInfoStats_p->isSpsConfigured = SPS_RELEASE;\
        dlSpsUeInfoStats_p->spsCrnti = 0;\
        dlSpsUeInfoStats_p->semiPersistentSchedIntervalDL = 0;\
        dlSpsUeInfoStats_p->numberOfConfSpsProcesses = 0;\
        dlSpsUeInfoStats_p->maxSpsHarqRetx = 0;\
        dlSpsUeInfoStats_p->explicitReleaseAfter = 0;\
        dlSpsUeInfoStats_p->numOfN1PucchAnPersistent = 0;\
    }\
}
#define LTE_MAC_UPDATE_UPLINK_SPS_UE_EXIST_STATE( ueId, isUeAct,currentTick){\
    LteMacUlSpsUeSpecificStats *ulSpsUeInfoStats_p = PNULL; \
    ulSpsUeInfoStats_p = &gMacStats.pStats->\
                           ueCommonInfoStats[ueId].ulSpsUeInfoStats;\
    ulSpsUeInfoStats_p->absoluteTTI = currentTick;\
    if( LTE_TRUE == isUeAct ){\
        ulSpsUeInfoStats_p->isUlUeActive = isUeAct;\
    }\
    else\
    {\
        ulSpsUeInfoStats_p->isUlUeActive = isUeAct;\
        ulSpsUeInfoStats_p->ueIndex = ueId;\
        ulSpsUeInfoStats_p->isSpsConfigured = SPS_RELEASE;\
        ulSpsUeInfoStats_p->spsCrnti = 0;\
        ulSpsUeInfoStats_p->semiPersistentSchedIntervalUL = 0;\
        ulSpsUeInfoStats_p->implicitReleaseAfter = 0;\
    }\
}
#else 
#define LTE_MAC_UPDATE_DL_DRX_MEAS_GAP_TTI( ueId, dlSpsInfo_p )
#define LTE_MAC_UPDATE_DOWNLINK_SPS_UE_EXIST_STATE(a,b);
#define LTE_MAC_UPDATE_UPLINK_SPS_UE_EXIST_STATE( ueId, isUeAct,currentTick);
#endif

#ifdef SSI_DEBUG_STATS
#define LTE_MAC_UPDATE_DOWNLINK_SPS_UE_STATS(ueId, dlSpsInfo_p, dlLCh_p){ \
    LteMacDlSpsUeSpecificStats *dlSpsUeInfoStats_p = PNULL; \
    dlSpsUeInfoStats_p= &gMacStats.pStats->\
                         ueCommonInfoStats[(ueId)].dlSpsUeInfoStats;\
    dlSpsUeInfoStats_p->ueIndex = (ueId); \
    dlSpsUeInfoStats_p->ipPktType = IP_NOT_DETERMINED; \
    dlSpsUeInfoStats_p->spsLc = INVALID_LCID;\
    dlSpsUeInfoStats_p->rohcProfileUsed = MAX_ROHC_PROFILE;\
    if ( PNULL != (dlLCh_p) )\
    {\
        dlSpsUeInfoStats_p->spsLc = (dlLCh_p)->logicalChannelId; \
        dlSpsUeInfoStats_p->rohcProfileUsed = (dlLCh_p)->rohcProfile; \
    }\
    dlSpsUeInfoStats_p->isSpsConfigured = (dlSpsInfo_p)->requestType; \
    dlSpsUeInfoStats_p->spsCrnti = (dlSpsInfo_p)->SpsCrnti; \
    if ( ((dlSpsInfo_p)->requestType == SPS_SETUP) && \
         ((dlSpsInfo_p)->spsDlSetupInfo_p != PNULL) )\
    {\
        dlSpsUeInfoStats_p->semiPersistentSchedIntervalDL = \
            spsIntervalMap_g[(dlSpsInfo_p)->spsDlSetupInfo_p->\
            semiPersistentSchedIntervalDL];\
        dlSpsUeInfoStats_p->numberOfConfSpsProcesses = \
            (dlSpsInfo_p)->spsDlSetupInfo_p->numberOfConfSpsProcesses;\
        dlSpsUeInfoStats_p->maxSpsHarqRetx = \
            (dlSpsInfo_p)->spsDlSetupInfo_p->maxSpsHarqRetx;\
        dlSpsUeInfoStats_p->explicitReleaseAfter = \
            (dlSpsInfo_p)->spsDlSetupInfo_p->explicitReleaseAfter;\
        dlSpsUeInfoStats_p->numOfN1PucchAnPersistent =\
            (dlSpsInfo_p)->spsDlSetupInfo_p->N1PucchAnPersistentListVal.\
            numOfN1PucchAnPersistent;\
        dlSpsUeInfoStats_p->N1PucchAnPersistent[0] = \
            (dlSpsInfo_p)->spsDlSetupInfo_p->N1PucchAnPersistentListVal.\
            N1PucchAnPersistent[0];\
        dlSpsUeInfoStats_p->N1PucchAnPersistent[1] = \
            (dlSpsInfo_p)->spsDlSetupInfo_p->N1PucchAnPersistentListVal.\
            N1PucchAnPersistent[1];\
        dlSpsUeInfoStats_p->N1PucchAnPersistent[2] = \
            (dlSpsInfo_p)->spsDlSetupInfo_p->N1PucchAnPersistentListVal.\
             N1PucchAnPersistent[2];\
        dlSpsUeInfoStats_p->N1PucchAnPersistent[3] = \
            (dlSpsInfo_p)->spsDlSetupInfo_p->N1PucchAnPersistentListVal.\
            N1PucchAnPersistent[3];\
    }\
}  
#define LTE_MAC_UPDATE_UPLINK_SPS_UE_STATS(ueId, ulSpsInfo_p){ \
    LteMacUlSpsUeSpecificStats *ulSpsUeInfoStats_p = PNULL; \
    ulSpsUeInfoStats_p= &gMacStats.pStats->\
                         ueCommonInfoStats[(ueId)].ulSpsUeInfoStats;\
    ulSpsUeInfoStats_p->ueIndex = (ueId); \
    if (PNULL != ulSpsInfo_p) {\
        ulSpsUeInfoStats_p->spsLc = (ulSpsInfo_p)->lcId; \
        ulSpsUeInfoStats_p->isSpsConfigured = (ulSpsInfo_p)->requestType; \
        ulSpsUeInfoStats_p->spsCrnti = (ulSpsInfo_p)->SpsCrnti; \
        if ((ulSpsInfo_p)->requestType == SPS_SETUP) \
        {\
            ulSpsUeInfoStats_p->semiPersistentSchedIntervalUL = \
                spsIntervalMap_g[(ulSpsInfo_p)->spsUlSetupInfo_p->\
                semiPersistentSchedIntervalUL];\
            ulSpsUeInfoStats_p->implicitReleaseAfter = \
                (ulSpsInfo_p)->spsUlSetupInfo_p->implicitReleaseAfter;\
            ulSpsUeInfoStats_p->p0NominalPuschPersistent = \
                 (ulSpsInfo_p)->spsUlSetupInfo_p->p0PersistentInfo.\
                 p0NominalPuschPersistent;\
            ulSpsUeInfoStats_p->p0UePuschPersistent = \
                 (ulSpsInfo_p)->spsUlSetupInfo_p->p0PersistentInfo.\
                 p0NominalPuschPersistent;\
        }\
    }\
} 
#define LTE_MAC_UPDATE_SPS_CONFIG_RESP_STATS(isUeAct,ueId,SpsCrnti){ \
    LteMacSpsConfigRespStats *spsConfigRespStats_p = PNULL; \
    spsConfigRespStats_p = &gMacStats.pStats-> \
                            ueCommonInfoStats[(ueId)].spsConfigRespStats; \
    spsConfigRespStats_p->isUeActive = (isUeAct); \
    spsConfigRespStats_p->spsCrnti = 0; \
    if (spsConfigRespStats_p->isUeActive) \
    { \
        spsConfigRespStats_p->ueIndex = (ueId);\
        spsConfigRespStats_p->spsCrnti = (SpsCrnti); \
    }\
}
#else
#define LTE_MAC_UPDATE_DOWNLINK_SPS_UE_STATS( a,b,c);
#define LTE_MAC_UPDATE_UPLINK_SPS_UE_STATS( a,b);
#define LTE_MAC_UPDATE_SPS_CONFIG_RESP_STATS(a,b,c);
#endif

#ifdef SSI_DEBUG_STATS
/* CA Stats Changes Start */
/* CA Changes start */
#define LTE_MAC_UPDATE_SYSTEM_WIDE_STATS( sysParams_p, internalCellIndex){ \
     QoSUEAndPerLCTTIStat *spsStats_p = \
               &gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)]; \
     spsStats_p->enableSps = sysParams_p->enableSps; \
     spsStats_p->maxSpsDciAttempt = sysParams_p->maxSpsDciAttempt; \
     memCpy( spsStats_p->qciSpslist, sysParams_p->qciSpslist, MAX_QCI) ; \
     memCpy( spsStats_p->supportedSpsDlIntervalList, \
                sysParams_p->supportedSpsDlIntervalList, TOTAL_SPS_INTERVAL); \
     memCpy( spsStats_p->supportedSpsUlIntervalList, \
                sysParams_p->supportedSpsUlIntervalList, TOTAL_SPS_INTERVAL); \
}
/* CA Changes end */
#define LTE_MAC_UPDATE_DL_INTERVAL_STATS( dlIntParams_p, ctr, internalCellIndex){\
     SpsDlIntProfileStats *spsStats_p = \
               &gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].spsDlIntProf[ctr] ;\
     spsStats_p->dlMinMcsSps = dlIntParams_p.dlMinMcsSps;\
     spsStats_p->dlMcsMargin = dlIntParams_p.dlMcsMargin;\
     spsStats_p->dlTbsizeMarginSps = dlIntParams_p.dlTbsizeMarginSps;\
     spsStats_p->rlcSduThresForDra = dlIntParams_p.rlcSduThresForDra;\
     spsStats_p->dlJitterHandlingOption = \
                                dlIntParams_p.dlJitterHandlingOption;\
}
#define LTE_MAC_UPDATE_UL_INTERVAL_STATS( ulIntParams_p, ctr, internalCellIndex){\
     SpsUlIntProfileStats *spsStats_p = \
         &gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].spsUlIntProf[ctr] ; \
     spsStats_p->ulMinMcsSps = ulIntParams_p.ulMinMcsSps;\
     spsStats_p->ulMcsMargin = ulIntParams_p.ulMcsMargin;\
     spsStats_p->ulTbSizeMarginSps = ulIntParams_p.ulTbSizeMarginSps;\
}

#define LTE_MAC_UPDATE_COMM_INTERVAL_STATS( commIntParams_p, ctr, internalCellIndex){\
     SpsCommIntProfileStats *spsStats_p =\
         &gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].spsCommIntProf[ctr];\
     spsStats_p->overrideSpsOccassionForOtherUe =\
               commIntParams_p.overrideSpsOccassionForOtherUe;\
     spsStats_p->strictSpsResourceUsageForSpsLc =\
               commIntParams_p.strictSpsResourceUsageForSpsLc;\
     spsStats_p->interPktDelayForSilenceDetection=\
               commIntParams_p.interPktDelayForSilenceDetection;\
     spsStats_p->enableSilenceDetection =\
               commIntParams_p.enableSilenceDetection;\
     memCpy(spsStats_p->maxPdcpPduSizeIpv4,\
               commIntParams_p.maxPdcpPduSizeIpv4,sizeof(spsStats_p->maxPdcpPduSizeIpv4));\
     spsStats_p->maxPdcpPduSizeIpv4[NO_ROHC_PROFILE] = commIntParams_p.maxPdcpPduSizeIpv4[MAC_ROHC_PROFILE - 1];\
     memCpy(spsStats_p->maxPdcpPduSizeIpv6,\
               commIntParams_p.maxPdcpPduSizeIpv6,sizeof(spsStats_p->maxPdcpPduSizeIpv6));\
     spsStats_p->maxPdcpPduSizeIpv6[NO_ROHC_PROFILE] = commIntParams_p.maxPdcpPduSizeIpv6[MAC_ROHC_PROFILE - 1];\
     spsStats_p->minRbSavedReactivationThres =\
               commIntParams_p.minRbSavedReactivationThres;\
     spsStats_p->spsBlerThres = commIntParams_p.spsBlerThres;\
     spsStats_p->auditInterval = commIntParams_p.auditInterval;\
     spsStats_p->spsResAllocTtiAdv = commIntParams_p.spsResAllocTtiAdv;\
}
#define LTE_MAC_UPDATE_SPS_DL_SCH_INFO_PER_TTI_STATS(spsDlSchInfoPerTti_p, internalCellIndex){\
    SpsDlSchInfoPerTtiStats *spsStats_p =\
        &gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].spsDlSchInfoPerTtiStats;\
    if (PNULL != (spsDlSchInfoPerTti_p))\
    {\
        spsStats_p->maxDlSpsDciPerTti = (spsDlSchInfoPerTti_p)->maxDlSpsDciPerTti;\
        spsStats_p->maxDlSpsOccasionPerTti = (spsDlSchInfoPerTti_p)->maxDlSpsOccasionPerTti;\
        spsStats_p->maxDlSpsRbsPerTti = (spsDlSchInfoPerTti_p)->maxDlSpsRbsPerTti;\
        memCpy(spsStats_p->maxDlSpsRbsPerTtiPerInterval,\
        (spsDlSchInfoPerTti_p)->maxDlSpsRbsPerTtiPerInterval,MAX_SPS_INTERVALS * sizeof(UInt32));\
    }\
}
#define LTE_MAC_UPDATE_SPS_UL_SCH_INFO_PER_TTI_STATS(spsUlSchInfoPerTti_p, internalCellIndex){\
    SpsUlSchInfoPerTtiStats *spsStats_p =\
        &gMacStats.pStats->qoSUEAndPerLCTTIStat[(internalCellIndex)].spsUlSchInfoPerTtiStats;\
    if (PNULL != (spsUlSchInfoPerTti_p))\
    {\
        spsStats_p->maxUlSpsDciPerTti = (spsUlSchInfoPerTti_p)->maxUlSpsDciPerTti;\
        spsStats_p->maxUlSpsOccasionPerTti = (spsUlSchInfoPerTti_p)->maxUlSpsOccasionPerTti;\
        spsStats_p->maxUlSpsRbsPerTti = (spsUlSchInfoPerTti_p)->maxUlSpsRbsPerTti;\
        memCpy(spsStats_p->maxUlSpsRbsPerTtiPerInterval,\
        (spsUlSchInfoPerTti_p)->maxUlSpsRbsPerTtiPerInterval,MAX_SPS_INTERVALS * sizeof(UInt32));\
    }\
}
#else
#define LTE_MAC_UPDATE_SYSTEM_WIDE_STATS( sysParams, internalCellIndex)
#define LTE_MAC_UPDATE_DL_INTERVAL_STATS( dlIntParams_p, ctr, internalCellIndex)
#define LTE_MAC_UPDATE_UL_INTERVAL_STATS( ulIntParams_p, ctr, internalCellIndex)
#define LTE_MAC_UPDATE_COMM_INTERVAL_STATS(commIntParams_p, ctr, internalCellIndex)
#define LTE_MAC_UPDATE_SPS_DL_SCH_INFO_PER_TTI_STATS(a, b)
#define LTE_MAC_UPDATE_SPS_UL_SCH_INFO_PER_TTI_STATS(a, b)
/* CA Stats Changes End */
#endif

/*CA chgs start*/
/*SPR 15909 fix start*/
void updateCAPRBUsageStats(tickType_t currentTick,UInt64 currentDLPRBUsage,InternalCellIndex internalCellIndex,
/*SPR 15909 fix end*/        
                           UInt8 ldCalcWindowSize,UInt32 dlAvailableRBs);
#define GET_CA_DLPRB_USAGE(internalCellIndex) \
        (gMacStats.pStats->lteMacCADLPrbUsageStats[internalCellIndex].totalDLPRBUsagePercent)
/*CA chgs end*/
#endif  /* INCLUDED_STATS_MANAGER_H */
