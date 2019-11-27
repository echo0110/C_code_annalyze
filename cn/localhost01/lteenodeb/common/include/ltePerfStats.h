/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: ltePerfStats.h
 *
 ****************************************************************************
 *
 *  File Description : contains the function prototypes and data structures
 *  needed for KPI performance stats.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/

#ifndef LTE_PERF_STATS_H
#define PERF_STATS_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
/* SPR 15876 FIX START */
//#ifdef PERF_STATS
/* SPR 15876 FIX END */
#include "lteTypes.h"
#include "lteLayer2CommanTypes.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MAX_PERF_STATS_INDEX 2
/* SPR  SPR 5450: Fix Start */
#define MAX_LC_IDENTITY  8
/* SPR  SPR 5450: Fix End */
#define MAX_PDCP_LC_ID 11
#define MAX_QCI 9
#define CRC_ACK   0
#define MOD_SCHEME 3
#define MAX_RANK_INDICATION 2
#define MAX_CQI_INDEX 16
#define MAX_CFI 4
#define MAX_CCE_UTIL_INDEX 20
#define RI_ONE 1
#define EVENT_VALUE_SRB  0 
#define EVENT_VALUE_DRB  1
#define EVENT_VALUE_ACK_MODE 0 
#define EVENT_VALUE_UNACK_MODE 1
#define MAX_SINR_DIFF_INDEX 8
#define SINR_VALUE_98 98
#define SINR_VALUE_104 104
#define SINR_VALUE_110 110
#define SINR_VALUE_116 116
#define SINR_VALUE_122 122
#define SINR_VALUE_128 128
#define SINR_VALUE_134 134
#define SINR_VALUE_118 118
#define SINR_VALUE_124 124
#define SINR_VALUE_132 132
#define SINR_VALUE_140 140
#define SINR_VALUE_148 148
#define SINR_VALUE_156 156
#define SINR_VALUE_162 162
#define MAX_UTIL_INDEX 100
#define MAX_TA_RANGE 12
#define MAX_SRB 3
#define INVALID_VALUE 255
#define SRB_0_LCID 0
#define SRB_1_LCID 1
#define SRB_2_LCID 2
#define RSHIFT_NUM 7
#define MAX_TA_INDEX_VAL 20
#define MAX_TA_ACT_INDEX_VAL 11
#define NORMAL_CYCLIC_PREFIX 1
#define EXTENTED_CYCLIC_PREFIX 2
#define NUM_OF_RES_IN_NCP_EXCULDING_DMRS_SRS  132
#define NUM_OF_RES_IN_NCP_EXCULDING_DMRS  144
#define NUM_OF_RES_IN_ECP_EXCULDING_DMRS_SRS  108
#define NUM_OF_RES_IN_ECP_EXCULDING_DMRS 120 
/* This macro is used for declaring array of variable length */
#define VAR_SIZE(x) 1
#define PERF_STATS_SUCCESS 1
#define PERF_STATS_FAILURE 0
#define PERF_MAX_UE_SUPPORTED MAX_UE_SUPPORTED
#define MAX_CP 2
 /* Macros for mapping stats shared memory */
#ifdef TARGET_ENB
#define PERF_STATS_SHM_KEY   34438
#else
#define PERF_STATS_SHM_KEY   34439
#endif

#define SET_PERIODIC_FLAG 1
#define UNSET_PERIODIC_FLAG 0

/* Perf stats RRM-MAC start */
#ifdef PERF_STATS
#define MAC_MAX_VAL_PERF_TIMER_T 60*60
#define MAC_MIN_VAL_PERF_TIMER_T 1 
#endif
/* Perf stats RRM-MAC end */


 /*Perf Stats Container Changes*/
#define PERF_STATS_CONTAINER_OAM_MAC          0
#define PERF_STATS_CONTAINER_OAM_PDCP         1
#define PERF_STATS_CONTAINER_OAM_RLC          2
/* Perf stats RRM-MAC start */
#define PERF_STATS_CONTAINER_RRM_MAC          3
#define MAX_PERF_STATS_CONTAINER              4
/* Perf stats RRM-MAC end */


#define PERF_STATS_CONTAINER_OAM_MAC_RESET_CELL_CONTAINER  0
#define PERF_STATS_CONTAINER_OAM_MAC_RESET_UE_CONTAINER    1
#define PERF_STATS_CONTAINER_OAM_PDCP_RESET_CELL_CONTAINER 2
#define PERF_STATS_CONTAINER_OAM_PDCP_RESET_UE_CONTAINER   3
#define PERF_STATS_CONTAINER_OAM_RLC_RESET_UE_CONTAINER    4
/* Perf stats RRM-MAC start */
#define PERF_STATS_CONTAINER_RRM_MAC_RESET_CELL_CONTAINER  5
#define PERF_STATS_CONTAINER_RRM_MAC_RESET_UE_CONTAINER    6
/* Perf stats RRM-MAC end */

#define PERF_STATS_CONTAINER_OAM_MAC_BITMASK         (0x01 << PERF_STATS_CONTAINER_OAM_MAC)
#define PERF_STATS_CONTAINER_OAM_PDCP_BITMASK        (0x01 << PERF_STATS_CONTAINER_OAM_PDCP) 
#define PERF_STATS_CONTAINER_OAM_RLC_BITMASK         (0x01 << PERF_STATS_CONTAINER_OAM_RLC) 
/* Perf stats RRM-MAC start */
#define PERF_STATS_CONTAINER_RRM_MAC_BITMASK         (0x01 << PERF_STATS_CONTAINER_RRM_MAC )
/* Perf stats RRM-MAC end */

/* This macro is used for declaring array of variable length */
#define MAC_OAM_VAR_SIZE(x) 1
#define MAX_OAM_PERF_STATS_SIZE      10000

/* This macro is used for declaring array of variable length */
#define PDCP_OAM_VAR_SIZE(x) 1
/* This macro is used for declaring array of variable length */
#define RLC_OAM_VAR_SIZE(x) 1

#define PERF_STATS_SHM_FLAGS (IPC_CREAT | 0666)
#define MAX_INDEX 2
/*Wiresect Changes*/
#define UPDATE_ACK_NACK_MODSCHEME_STATS( ueIndex, ackOrNack , mcs) \
{\
    if(mcs <= MCS_INDEX_QPSK_9)\
    {\
	    if (DL_ACK == ackOrNack)\
	    {\
		    gMacUePerfStats_p[ueIndex].\
		    lteUeMacDLPerfStats.totalDLHarqAck[0]++;\
	    }\
	    else\
	    {\
		    gMacUePerfStats_p[ueIndex].\
		    lteUeMacDLPerfStats.totalDLHarqNack[0]++;\
	    }\
    }\
    else if ((MCS_INDEX_16QAM_10 <= mcs) && (mcs <= MCS_INDEX_16QAM_16))\
    {\
        if (DL_ACK == ackOrNack)\
	    {\
		    gMacUePerfStats_p[ueIndex].\
		    lteUeMacDLPerfStats.totalDLHarqAck[1]++;\
	    }\
	else\
	    {\
		    gMacUePerfStats_p[ueIndex].\
		    lteUeMacDLPerfStats.totalDLHarqNack[1]++;\
	    }\
    }\
    else if ((MCS_INDEX_64QAM_17 <= mcs) && (mcs <= MCS_INDEX_64QAM_28))\
	{\
		if (DL_ACK == ackOrNack)\
		{\
			gMacUePerfStats_p[ueIndex].\
			lteUeMacDLPerfStats.totalDLHarqAck[2]++;\
		}\
		else\
		{\
			gMacUePerfStats_p[ueIndex].\
			lteUeMacDLPerfStats.totalDLHarqNack[2]++;\
		}\
    }\
	\
}



#define UPDATE_UE_HARQ_UL_SUCC_FAIL_MODSCHEME_STATS(ueIndex , crcAckNack, modType)\
{\
	if (QPSK == modType)\
	{\
		if (CRC_ACK == crcAckNack)\
		{\
			gMacUePerfStats_p[ueIndex].\
			lteUeMacULPerfStats.totalULHarqSucc[0]++; \
		}\
		else\
		{\
			gMacUePerfStats_p[ueIndex].\
			lteUeMacULPerfStats.totalULHarqFail[0]++; \
		}\
	}\
	else if (SIXTEENQAM == modType)\
	{\
		if (CRC_ACK == crcAckNack)\
		{\
			gMacUePerfStats_p[ueIndex].\
			lteUeMacULPerfStats.totalULHarqSucc[1]++; \
		}\
		else\
		{\
			gMacUePerfStats_p[ueIndex].\
			lteUeMacULPerfStats.totalULHarqFail[1]++; \
		}\
	}\
}

#define UPDATE_UE_TX_MODE_RI_STATS(ueIndex,transmissionScheme)\
{\
   DLUEContext *dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;\
   UInt8 dlRi; \
   if (PNULL != dlUEContext_p) \
   { \
       dlRi = dlUEContext_p->dlMIMOInfo.dlRi; \
 \
       switch(transmissionScheme)\
       {\
            case FAPI_TX_DIVERSITY:\
                if (RI_ONE == dlRi)\
	        {\
	             gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats.\
		         	   totalDLTransmissionsWithTX2RI1++;\
                }\
                break;\
\
            case FAPI_LARGE_DELAY_CDD:\
\
                gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats.\
                           totalDLTransmissionsWithTX3RI[dlRi - 1]++;\
                break;\
\
            case FAPI_CLOSED_LOOP_SPATIAL_MULTIPLEXING:\
\
                gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats.\
                          totalDLTransmissionsWithTX4RI[dlRi - 1]++;\
                break;\
\
            default:\
            break;\
\
   }\
  } \
\
}
#define UPDATE_UE_PUCCH_SINR_STATS(ueIndex, ulCQI ,subFrameNum) \
{\
   if (0!= (pucchUciUlschMap_g[ueIndex] & bitmask[subFrameNum])) \
   { \
        if (SINR_VALUE_98 >= ulCQI)\
	{ \
	    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalPUCCHSINRReported[0]++; \
	} \
	else if(SINR_VALUE_104 >= ulCQI) \
	{ \
	    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalPUCCHSINRReported[1]++; \
	} \
	else if(SINR_VALUE_110 >= ulCQI) \
	{ \
	    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalPUCCHSINRReported[2]++; \
        } \
	else if(SINR_VALUE_116 >= ulCQI) \
	{ \
	    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalPUCCHSINRReported[3]++; \
	} \
	else if(SINR_VALUE_122 >= ulCQI) \
	{ \
	    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalPUCCHSINRReported[4]++;\
	} \
	else if(SINR_VALUE_128 >= ulCQI) \
	{ \
            gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalPUCCHSINRReported[5]++; \
	} \
	else if(SINR_VALUE_134 >= ulCQI) \
	{ \
	    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalPUCCHSINRReported[6]++; \
	} \
	else if(SINR_VALUE_134 < ulCQI) \
	{ \
	    gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalPUCCHSINRReported[7]++; \
	} \
  } \
}


#define UPDATE_UE_PUSCH_SINR_STATS(ueIndex, ulCQI) \
{\
   if (SINR_VALUE_118 >= ulCQI)\
   { \
       gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalPUSCHSINRReported[0]++; \
   } \
   else if(SINR_VALUE_124 >= ulCQI) \
   { \
       gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalPUSCHSINRReported[1]++; \
   } \
   else if(SINR_VALUE_132 >= ulCQI) \
   { \
       gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalPUSCHSINRReported[2]++; \
   } \
   else if(SINR_VALUE_140 >= ulCQI) \
   { \
       gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalPUSCHSINRReported[3]++; \
   } \
   else if(SINR_VALUE_148 >= ulCQI) \
   { \
       gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalPUSCHSINRReported[4]++;\
   } \
   else if(SINR_VALUE_156 >= ulCQI) \
   { \
       gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalPUSCHSINRReported[5]++; \
   } \
   else if(SINR_VALUE_162 >= ulCQI) \
   { \
       gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalPUSCHSINRReported[6]++; \
   } \
   else if(SINR_VALUE_162 < ulCQI) \
   { \
       gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalPUSCHSINRReported[7]++; \
   } \
}
/* + PERF_CA GBL */
/** CA-TDD Changes Start **/
#define CALCULATE_DL_RESOURCE_ELEMENTS(allocatedRBs,internalCellIndex) \
{\
    UInt32 unUsedREs = 0;\
    if(overlapRBsMibSS_g[internalCellIndex])\
    {\
            if (perfStatsMibSyncSignalFlag_g[internalCellIndex] == PERF_STATS_MIB_SS)\
            {\
                unUsedREs = (reUsedForMibPerPRB_g[internalCellIndex] + reUsedForPssSssPerPRB_g[internalCellIndex]) * overlapRBsMibSS_g[internalCellIndex];\
            }\
            else if(perfStatsMibSyncSignalFlag_g[internalCellIndex] == PERF_STATS_ONLY_SS)\
            {\
                unUsedREs = reUsedForPssSssPerPRB_g[internalCellIndex] * overlapRBsMibSS_g[internalCellIndex];\
            }\
            overlapRBsMibSS_g[internalCellIndex] = 0;\
    }\
            harqProcess_p->totalREsUsed =\
                 allocatedRBs * (rePerRBPair_g[cellSpecificParams_g.\
                   cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->cyclicPrefix-1][MAX_CFI-1]\
                       - csrREsExcludingFirstSymbol_g[internalCellIndex]) - unUsedREs;\
}

/** CA-TDD Changes Stop **/
/* - PERF_CA GBL */
#define UPDATE_RADIOTHP_RES_UL_STATS(ueIndex, ulHarqProcessId, subframe ,internalCellIndex)\
{\
    UInt32 numRBs = 0;\
    if ( ulHarqProcessId != INVALID_HARQ_ID)\
    {\
        numRBs = ulUECtxInfoArr_g[ueIndex].ulUEContext_p->\
                           ulresInfo[ulHarqProcessId].riLength;\
        switch (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                cellParams_p->cyclicPrefix)\
        {\
\
           case NORMAL_CYCLIC_PREFIX:\
\
              if (*(srsCellConfigMap_gp[internalCellIndex] + subframe) == LTE_TRUE)\
              {\
                  gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalREsUsedInUL += \
                         numRBs * NUM_OF_RES_IN_NCP_EXCULDING_DMRS_SRS;   \
              }\
              else  \
              {\
                  gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalREsUsedInUL += \
                         numRBs * NUM_OF_RES_IN_NCP_EXCULDING_DMRS;   \
              }\
              break;\
\
           case EXTENTED_CYCLIC_PREFIX:\
 \
              if (*(srsCellConfigMap_gp[internalCellIndex] + subframe) == LTE_TRUE)\
              {\
                  gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalREsUsedInUL += \
                         numRBs * NUM_OF_RES_IN_ECP_EXCULDING_DMRS_SRS;   \
              }\
              else \
              {\
                  gMacUePerfStats_p[ueIndex].lteUeMacULPerfStats.totalREsUsedInUL += \
                         numRBs * NUM_OF_RES_IN_ECP_EXCULDING_DMRS;   \
              }\
              break;\
\
            default:\
              break;\
        } \
    }\
}

/****************************************************************************
 * Exported Types
 ***************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
typedef enum
{
    PERF_STATS_NO_OVERLAP,
    PERF_STATS_ONLY_SS,
    PERF_STATS_MIB_SS
}PerfStatsMibSyncSigType;

/* This enum is used for the type of stats in the Perf Stats Container */
typedef enum PerfStatsTypeT
{
  MAC_PERF_STATS  = 1,
  PDCP_PERF_STATS = 2,
  RLC_PERF_STATS  = 3
}PerfStatsType;

/* Enum for storing MAC Stats According to the bits */
typedef enum UeMacPerfStatsSupportedT
{
    PERF_STATS_CQI                   =0,  //bit 0
    PERF_STATS_HARQ                  =1,  //bit 1
    PERF_STATS_UE_CAPABILITY         =2,  //bit 2
    PERF_STATS_POWER_LIMITED         =3,  //bit 3
    PERF_STATS_TX_MODE_RI            =4,  //bit 4
    PERF_STATS_PUCCH_PUSCH_SINR      =5,  //bit 5
    PERF_STATS_DATA_VOLUME           =6,  //bit 6
    PERF_STATS_UE_PRB_USAGE          =7,  //bit 7
    UE_MAC_PERF_STATS_MAX_VALUE     
}UeMacPerfStatsSupported;

typedef enum CellMacPerfStatsSupportedT
{
    PERF_STATS_PRB_USAGE_HARQ            =0, //bit 0
    PERF_STATS_RACH_SUCC_FAIL            =1, //bit 1
    PERF_STATS_PDCCH_UTILIZATION         =2, //bit 2
    PERF_STATS_PUCCH_UTILIZATION         =3, //bit 3
    PERF_STATS_CELL_PRB_USAGE            =4, //bit 4
    /*+ Dyn UEs To Be SchPerTTI +*/
    PERF_STATS_TOTAL_ACT_UE_IN_UL_DL     =5, //bit 5
    /*- Dyn UEs To Be SchPerTTI -*/
    CELL_MAC_PERF_STATS_MAX_VALUE     
}CellMacPerfStatsSupported;

/* Enum for storing RLC Stats According to the bits */
typedef enum UeRlcPerfStatsSupportedT
{
    PERF_STATS_RB                 =0, //bit 0
    PERF_STATS_ReTx_EXCEEDED      =1, //bit 1
    PERF_STATS_UE_ACK_NACK        =2, //bit 2
    UE_RLC_PERF_STATS_MAX_VALUE
}UeRlcPerfStatsSupported;

/* Enum for storing PDCP Stats According to the bits */
typedef enum UePdcpPerfStatsSupportedT
{
    PERF_STATS_PDCP_RB                      =0,  //bit 0
    PERF_STATS_PDCP_RADIO_BEARER_INFO       =1,  //bit 1
    PERF_STATS_PDCP_VOLUME                  =2,  //bit 2
    UE_PDCP_PERF_STATS_MAX_VALUE
}UePdcpPerfStatsSupported;

typedef enum CellPdcpPerfStatsSupportedT
{
    PERF_STATS_ROHC                       =0,  //bit0
    CELL_PDCP_PERF_STATS_MAX_VALUE
}CellPdcpPerfStatsSupported;


/****************************************************************************
 * Exported Variables
 ****************************************************************************/


/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* Structure Name  : LteUeMacULPerfStats
 * Purpose         : Contains the statistics for UL MAC UE Perf Stats
 *                   This will be updated by MAC Thread */
typedef struct  LteUeMacULPerfStatsT
{   
    /* Total number of times CQI 0 - 15 reported at CW1
     * Event Name : EVENT_PARAM_CQI_REPORTED_0 .. 15
     */
    UInt32 totalCQICW1Reported[MAX_CQI_INDEX];

    /* Total number of times CQI 0 - 15 reported at CW2
     *  Event Name : EVENT_PARAM_CQI2_REPORTED_0 .. 15
     */
    UInt32 totalCQICW2Reported[MAX_CQI_INDEX];
    
    /* Total number of times RI = 1 and RI = 2 is reported.
     * Events Name : EVENT_PARAM_RANK_REPORTED_0 
     *               EVENT_PARAM_RANK_REPORTED_1 
     */ 
    UInt32 totalRankReported[MAX_RANK_INDICATION];

    /* total number of successful HARQ ACK from UE in UL using 
     * QPSK /16QAM/64QAM modulation. 
     * Event Name : EVENT_PARAM_UE_HARQ_UL_SUCC_QPSK 
     EVENT_PARAM_UE_HARQ_UL_SUCC_16QAM */ 
    UInt32 totalULHarqSucc[MOD_SCHEME]; 

    /* total number of successful HARQ NACK from UE in UL using 
     * QPSK /16QAM/64QAM modulation. 
     * Event name  : EVENT_PARAM_UE_HARQ_UL_FAIL_QPSK
     EVENT_PARAM_UE_HARQ_UL_FAIL_16QAM */   
    UInt32 totalULHarqFail[MOD_SCHEME];
     /* total successfully transferred data volume on MAC level in the Uplink
      * This includes Transmission and retransmission
      * Event Name: EVENT_PARAM_RADIOTHP_VOL_UL*/
    UInt32 totalUlBytesReceived; 

    /* Partcular index defines PUCCH SINR lies in some particular range.
     * Events Name : EVENT_PARAM_SINR_MEAS_PUCCH_0 .. 7   
     */   
    UInt32 totalPUCCHSINRReported[MAX_SINR_DIFF_INDEX];

    /* Partcular index defines PUSCH SINR lies in some particular range.
     * Events Name : EVENT_PARAM_SINR_MEAS_PUSCH_0 .. 7   
     */   
    UInt32 totalPUSCHSINRReported[MAX_SINR_DIFF_INDEX];

    /* The UL volume used for UL UE Throughput. It comprises of the MAC SDU volume 
     * received on Uu, excluding the volume received in the first 4 data receptions of 
     * an UL buffer transfer and the TTI emptying the UL buffer.
     * EVENT : EVENT_PARAM_PER_UE_THP_MACVOL_TRUNK_UL*/
    UInt32 totalUlThpPerUeTrunk;

   /* The total amount of physical resources used for transmission in the uplink. 
    *  Both successful and unsuccessful transmissions are counted. 
    *  Event Name : EVENT_PARAM_RADIOTHP_RES_UL
    */
    UInt32 totalREsUsedInUL;

}LteUeMacULPerfStats;


/* Structure Name  : LteUeMacDLPerfStats
 * Purpose         : Contains the statistics for MAC UE Perf Stats
 *                   This will be updated by MAC Thread */
typedef struct  LteUeMacDLPerfStatsT
{   
    /* total number of successful HARQ ACK from UE in DL using 
     * QPSK /16QAM/64QAM modulation. 
     * Event Name : EVENT_PARAM_UE_HARQ_DL_ACK_QPSK
     *              EVENT_PARAM_UE_HARQ_DL_ACK_16QAM*/
    UInt32 totalDLHarqAck[MOD_SCHEME];

    /* total number of successful HARQ NACK from UE in DL using 
     * QPSK /16QAM/64QAM modulation. 
     * Event name : EVENT_PARAM_UE_HARQ_DL_NACK_64QAM
     *              EVENT_PARAM_UE_HARQ_DL_NACK_16QAM  */
    UInt32 totalDLHarqNack[MOD_SCHEME];

    /* Number of times the HARQ fails  */
    UInt32 totalHarqFail; 
     /* total successfully transferred data volume on MAC level in the downlink
      * This includes Transmission and retransmission 
      * Event Name: EVENT_PARAM_RADIOTHP_VOL_DL*/
    UInt32 totalDlBytesSent;

    /* Total number of samples with RANK=1 and TX=2 used towards an UE  
     * Event Name : EVENT_PARAM_RANK_TX_0
     */  
    UInt32 totalDLTransmissionsWithTX2RI1;
     
    /* Samples with RANK=1/2 and TX=3 used towards an UE 
     * Events Name:  EVENT_PARAM_RANK_TX_1, Open Loop SM Rank 1 
     *               EVENT_PARAM_RANK_TX_2, Open Loop SM Rank 2  
     */ 
    UInt32 totalDLTransmissionsWithTX3RI[MAX_RANK_INDICATION];

    /* Samples with RANK=1/2 and TX=4 used towards an UE
     * Events Name: EVENT_PARAM_RANK_TX_3, Closed Loop SM rank 1
     * EVENT_PARAM_RANK_TX_4, Closed Loop SM rank 2 
     */  
    UInt32 totalDLTransmissionsWithTX4RI[MAX_RANK_INDICATION];

    /* The total amount of physical resources used for transmission in the uplink.
     *Both successful and unsuccessful transmissions are counted.
     * EVENT : EVENT_PARAM_RADIOTHP_RES_DL
     */
    UInt32 totalDLREsUsage;

}LteUeMacDLPerfStats;

/* Structure Name  : LteUeMacSchedULPerfStats
 * Purpose         : Contains the statistics for UL MAC Scheduler UE Perf Stats
 *                   This will be updated by MAC Thread */
typedef struct  LteUeMacSchedULPerfStatsT
{
    /*  number of ms where respective UE is limited in the
     *   UL direction by its UE capability
     *   Event Name : EVENT_PARAM_PER_SCHED_RESTRICT_UE_CAT_UL */
    UInt32 totalTimeRestrictUeInUlbyUeCap;
    /* number of Transport Blocks on MAC level scheduled in uplink
    *  where the UE was considered to be power limited
    *  Event name : EVENT_PARAM_TBSPWRRESTRICTED */
    UInt32 totalTbScheduledOnPowerRestriction;
     /* The number of Transport Blocks on MAC level scheduled in uplink
    *  where the UE was NOT considered to be power limited.
    *  Event Name : EVENT_PARAM_TBSPWRUNRESTRICTED */
    UInt32 totalTbScheduledInUlOnNoPowerRestriction ;

}LteUeMacSchedULPerfStats;

/* Structure Name  : LteUeMacSchedDLPerfStats
 * Purpose         : Contains the statistics for DL MAC Scheduler UE Perf Stats
 *                   This will be updated by MAC Thread */
typedef struct  LteUeMacSchedDLPerfStatsT
{  

    /*  number of ms where respective UE is limited in the
      *   DL direction by its UE capability 
      *   Event Name: EVENT_PARAM_PER_SCHED_RESTRICT_UE_CAT_DL */
    UInt32 totalTimeRestrictUeInDlbyUeCap;
 
}LteUeMacSchedDLPerfStats;

/* Structure name  :LteUePdcpULPerfStats 
 * Purpose         :Contains statistics for UL PDCP UE Perf Stats.
 *                  This will be updated by PDCP thread  */
typedef struct  LteUePdcpULPerfStatsT
{
    /* Indicates Total SRB volume in UL for PDCP SDUs 
     * Event Name : EVENT_PARAM_PER_UE_PDCP_SRB_TRANSVOL_DL */
    UInt32 totalUlSrbBytesRcvd;

    /* Indicates DRB volume in UL for PDCP SDUs
     * Event Name : EVENT_PARAM_PER_DRB_PDCP_RECVOL_UL */
    UInt32 totalUlDrbBytesSent[MAX_LC_IDENTITY];

    /*Indicates Total RB volume (SRB+DRB Volume) in UL for PDCP SDUs */
    UInt32 totalUlRbBytesSent; 

    /*Indicates Number of PDCP SDUs received for a DRB, UL
     * Event Name : EVENT_PARAM_PER_DRB_PACKET_REC_UL */
    UInt32 numberOfUlDrbSduReceived[MAX_LC_IDENTITY];
}LteUePdcpULPerfStats;


/* Structure name  :LteUePdcpDLPerfStats 
 * Purpose         :Contains statistics for UL PDCP UE Perf Stats.
 *                  This will be updated by PDCP thread  */
typedef struct  LteUePdcpDLPerfStatsT
{
    /*Indicates Total SRB volume that has been transmitted in the 
     *downlink direction in the PDCP layer 
     * Event Name : EVENT_PARAM_PER_UE_PDCP_SRB_RECVOL_UL */
    UInt32 totalDlSrbBytesSent;
     /* Indicates the total volume (PDCP SDU) on Data Radio Bearers that has
      * been transmitted and acknowledged in the downlink direction */
    UInt32 totalDLSrbBytesSentAndAck;

    /* Indicates DRB volume that has been transmitted in the downlink 
     * direction in the PDCP layer.
     * Event Name : EVENT_PARAM_PER_DRB_PDCP_TRANSVOL_DL */
    UInt32 totalDlDrbBytesSent[MAX_LC_IDENTITY];

    /* Indicates Total SRB volume that has been transmitted in the 
     * downlink direction in the PDCP layer  */
    UInt32 totalDlRbBytesSent; 

    /*Indicates Number of DL PDCP SDUs received per DRB 
     * Event Name : EVENT_PARAM_PER_DRB_PACKET_REC_DL*/
    UInt32 numberOfDlDrbSduReceived[MAX_LC_IDENTITY];
    
    /* Number of DL PDCP SDUs discarded due to HO per DRB 
     * EVENT : EVENT_PARAM_PER_DRB_PACKET_LOST_HO_DL */
    UInt32 totalDlDrbSduDiscardedHO[MAX_LC_IDENTITY];
    /* The DL transmitted PDCP volume for the last piece of data
      in large data bursts on RB level */
     UInt32 pdcpLastTtiTransVolDl [MAX_LC_IDENTITY];
}LteUePdcpDLPerfStats;

/* Structure name  : LteeUePdcpRadioBearerInfoStats
 * Purpose         : Contains the statistics for PDCP UE radio bearer Info 
 *                   Perf Stats.This will be updated by PDCP Thread 
 */

typedef struct  LteUePdcpRadioBearerInfoStatsT
{

    /* Indicates Signalling Radio Bearer Id or Data Radio Bearer Id.
     * Event Name : EVENT_PARAM_BEARER_ID 
     */
    UInt32 bearerId;

   /* Indicates type of the bearer: Signaling Radio Bearer (SRB) 
    * or Data Radio Bearer (DRB)
    * EVENT_VALUE_SRB=0 , EVENT_VALUE_DRB=1
    * Event Name : EVENT_PARAM_BEARER_TYPE 
    */
   UInt32 bearerType;
 
   /* Indicates Radio Bearer QCI 
    * Event Name : EVENT_PARAM_PER_RB_QCI  */
   UInt32 qci;

   /* Indicates RLC Mode used:
    *   EVENT_VALUE_ACK_MODE=0, EVENT_VALUE_UNACK_MODE=1
    * Event Name : EVENT_PARAM_RLC_MODE  
    */ 
   UInt32 rlcMode;  

}LteUePdcpRadioBearerInfoStats;

/* Structure name  : LteUeRlcDLPerfStats
 * Purpose         : Contains the statistics for RLC UE Perf Stats
 *                   This will be updated by RLC Thread */
typedef struct  LteUeRlcDLPerfStatsT
{
    /*Indicates total number of RLC SDU transmitted to MAC 
     *in DL for RLC UM bearers per LC
     * Event Name : EVENT_PARAM_PER_RLC_UM_SDU_TRANSMITTED */
    UInt32 totalDlRLCUMSduTransmitted[MAX_LC_IDENTITY];
    
   /* indicates total number of times  the maximum number 
    * of RLC retransmissions has exceeded.
    * Event Name : LOCAL_MAX_NR_RLC_RETRANSMISSIONS_EXCEEDED 
    */
    UInt32  totalMaxRetxExceeded;

    /* The total number of successful RLC PDU transmissions (ACKs) 
     * in the downlink direction
     * EVENT : EVENT_PARAM_PER_UE_RLC_ACK_DL*/
    UInt32 totalDLRLCAcks;
    
    
}LteUeRlcDLPerfStats;

/* Structure name  : LteUeRlcULPerfStats
 * Purpose         : Contains the statistics for RLC UE Perf Stats
 *                   This will be updated by RLC Thread */
typedef struct  LteUeRlcULPerfStatsT
{
    /* The total number of successful RLC PDU transmissions 
     * (ACKs) in the uplink direction
     * EVENT : EVENT_PARAM_PER_UE_RLC_ACK_UL */
    UInt32 totalULRLCAcks;

    /* The total number of unsuccessful RLC PDU and RLC PDU segment transmissions 
     * (NACKs) in the uplink direction.
     * EVENT : EVENT_PARAM_PER_UE_RLC_NACK_UL */
    UInt32 totalULRLCNacks;
}LteUeRlcULPerfStats;


/* Structure name  : LteCellMacULPerfStats
 * Purpose         : Contains the system wise statistics for MAC CELL Perf Stats */
typedef struct  LteCellMacULPerfStatsT
{   
    /*Number of successfully detected RA Msg3 for CBRA
     * Event Name : EVENT_PARAM_RA_SUCC_CBRA */
    UInt32 totalRcvdMsg3RandomAccessPreamblesCBRA;
    /*Number of successfully completed CFRA procedures 
     * Event Name : EVENT_PARAM_RA_SUCC_CFRA*/
    UInt32 totalRcvdMsg3RandomAccessPreamblesCFRA;
    /*Number of CBRA preambles for which no RA Msg2 was sent
     * Event Name : EVENT_PARAM_RA_FAIL_CBRA_MSG2_DISC*/
    UInt32 totalFailedRandomAccessPreamblesMsg2CBRA;
    /*Number of CFRA preambles for which no RA Msg2 was sent
     * Event Name : EVENT_PARAM_RA_FAIL_CFRA_MSG2_DISC */
    UInt32 totalFailedRandomAccessPreamblesMsg2CFRA;
    UInt32 totalAllocatedRandomAccessPreambles;
    /* Number of detected successfull random access with a TA value in the range 0..9%, 
     * 10..19% and so on of the maximum possible TA value for the configured cell range. 
     * EVENT : EVENT_PARAM_RA_SUCC_TA_x where x range from 0 to 11*/
    UInt32 totalSuccRandomAccessTA[MAX_TA_RANGE];
    /* The number of detected CBRA preambles that are not allocated to any UE
     * Event Name : EVENT_PARAM_RA_UNASSIGNED_CFRA_FALSE */
    UInt32 totalUnassignedFalseCFRA;
    /* The total number of unassigned CBRA preambles at each 
     * PRACH occasion during the reporting period.
     * Event Name : EVENT_PARAM_RA_UNASSIGNED_CFRA_SUM */
    UInt32 totalUnassignedSumCFRA;
    /* UL PRB usage for SRB
     * EVENT : EVENT_PARAM_DCCH_CCCH_UL_RES */
    UInt32 totalULPRBUsageSRB[MAX_SRB];
}LteCellMacULPerfStats;

/* Structure name  : LteCellMacDLPerfStats
 * Purpose         : Contains the system wise statistics for DL MAC CELL Perf Stats */
typedef struct  LteCellMacDLPerfStatsT
{   
    UInt32 totalDLPRBUsageforHarq[MAX_QCI]; 
    UInt32 totalActiveDLUe[MAX_QCI];
    /* Number of subframes with PDCCH CFI set to 1 ,
     *  CFI set to 2 and CFI set to 2
     * Event Name: EVENT_PARAM_PER_PDCCH_CFI_UTIL_DISTR_1
     * Event Name: EVENT_PARAM_PER_PDCCH_CFI_UTIL_DISTR_2
     * Event Name: EVENT_PARAM_PER_PDCCH_CFI_UTIL_DISTR_3*/
    UInt32 noOfSubFrameWithPdcchCfi[MAX_CFI];

    /* The total number of Physical Resource Blocks (PRB) for HARQ initial transmissions 
     * used for Signaling Radio Bearers (SRB) in the downlink
     * EVENT : EVENT_PARAM_DCCH_CCCH_DL_RES */
    UInt32 totalDLPRBUsageSRB[MAX_SRB];
}LteCellMacDLPerfStats;

/* Structure name  : LteCellMacSchedULPerfStats
 * Purpose         : Contains the system wise statistics for UL MAC Scheduler CELL Perf Stats */
typedef struct  LteCellMacSchedULPerfStatsT
{
    /* Aggregated number of attempts to allocate PUCCH resources for CQI and SR.
     * Event Name : EVENT_PARAM_SR_CQI_SAMPL 
     */   
    UInt32  totalPUCCHResAllocationAttempts;
    /*EVENT : EVENT_PARAM_AVAILABLE_UL_RES, Aggregating Available PRBs a
     * available during a given sampling time*/
    UInt32 totalULPRBAvailable;
   
}LteCellMacSchedULPerfStats;

/* Structure name  : LteCellMacSchedDLPerfStats
 * Purpose         : Contains the system wise statistics for DL MAC Scheduler CELL Perf Stats */
typedef struct  LteCellMacSchedDLPerfStatsT
{
    /* Number of samples with a PDCCH utilization in the range 0-5% 
     * Event Name: EVENT_PARAM_PER_PDCCH_CCE_UTIL_DISTR */
    UInt32 totalPdcchCceUtil[MAX_CCE_UTIL_INDEX];
    /* Number of PRBs used in DL per cell for HARQ retransmissions 
     * Event name : EVENT_PARAM_PER_CELL_PRB_USED_RETX_DL */
    UInt32 totalDLPRBUsageforHarqRetx;
     /* total max number of CCEs available in a cell 
      * Event Name :EVENT_PARAM_PER_CELL_CCE_AVAIL */
    UInt32 totalCceAvail;
    /* DL PRB usage for BCCH Channel
     * EVENT : EVENT_PARAM_BCCH_DL_RES*/
    UInt32 totalDLPRBUsageBCCH;
    /* DL PRB usage for PCCH Channel
     * EVENT : EVENT_PARAM_PCCH_DL_RES*/
    UInt32 totalDLPRBUsagePCCH;
    /*EVENT : EVENT_PARAM_AVAILABLE_DL_RES, Aggregating Available PRBs 
     * available during a given sampling time*/
    UInt32 totalDLPRBAvailable;

}LteCellMacSchedDLPerfStats;

/* Structure name  :LteCellPdcpULPerfStats
 * Purpose         :Contains the system wise statistics for PDCP CELL Perf Stats */
typedef struct  LteCellPdcpULPerfStatsT
{
    /*Indicates Total decompressed or uncompressed header volume that has 
     * been transmitted in the uplink direction in the PDCP layer per QCI 
     *  Event Name  : EVENT_PARAM_PER_PDCPROHC_HDRVOL_UL_RB_QCI */
    UInt32 totalRohcUnCmpHdrVolUL[MAX_QCI];
    /*Indicates Total compressed header volume that has been 
     * received in the uplink direction in the PDCP layer per QCI 
     * Event Name  : EVENT_PARAM_PER_PDCPROHC_CMPHDRVOL_UL_RB_QCI*/
    UInt32 totalRohcCmpHdrVolUL[MAX_QCI];	
}LteCellPdcpULPerfStats;

/* Structure name  :LteCellPdcpDLPerfStats
 * Purpose         :Contains the system wise statistics for PDCP CELL Perf Stats */
typedef struct  LteCellPdcpDLPerfStatsT
{
    /*Indicates Total uncompressed header volume that has been received in
     * the downlink direction in the PDCP layer per QCI 
     * Event name  : EVENT_PARAM_PER_PDCPROHC_HDRVOL_DL_RB_QCI*/
    UInt32 totalRohcUnCmpHdrVolDL[MAX_QCI];
    /*Indicates Total compressed header volume that has been transmitted in 
     *the downlink direction in the PDCP layer per QCI 
     * Event name   : EVENT_PARAM_PER_PDCPROHC_CMPHDRVOL_DL_RB_QCI */
    UInt32 totalRohcCmpHdrVolDL[MAX_QCI];
}LteCellPdcpDLPerfStats;

/* Structure name  :LteCellRlcULPerfStats
 * Purpose         :Contains the system wise statistics for UL Rlc CELL Perf Stats */
typedef struct  LteCellRlcULPerfStatsT
{
    /* Fields will be added in future releases */
}LteCellRlcULPerfStats;

/* Structure name  :LteCellRlcDLPerfStats
 * Purpose         :Contains the system wise statistics for DL Rlc CELL Perf Stats */
typedef struct  LteCellRlcDLPerfStatsT
{
    /* Fields will be added in future releases */
}LteCellRlcDLPerfStats;

/* Structure Name : LteCellPerfStats
 * Purpose        : Contains the system wise statistics for Cell Perf Stats*/
typedef struct  LteCellPerfStatsT
{
    LteCellMacULPerfStats lteCellMacULPerfStats;
    LteCellMacDLPerfStats lteCellMacDLPerfStats;

    LteCellMacSchedULPerfStats lteCellMacSchedULPerfStats;
    LteCellMacSchedDLPerfStats lteCellMacSchedDLPerfStats;

    LteCellPdcpULPerfStats lteCellPdcpULPerfStats;
    LteCellPdcpDLPerfStats lteCellPdcpDLPerfStats;

    LteCellRlcULPerfStats lteCellRlcULPerfStats;
    LteCellRlcDLPerfStats lteCellRlcDLPerfStats;
}LteCellPerfStats;

/* Structure Name : LteUePerfStats
 * Purpose        : Contains the statistics for UE Perf Stats*/
typedef struct  LteUePerfStatsT
{
    LteUeMacULPerfStats lteUeMacULPerfStats;
    LteUeMacDLPerfStats lteUeMacDLPerfStats;

    LteUeMacSchedULPerfStats lteUeMacSchedULPerfStats;
    LteUeMacSchedDLPerfStats lteUeMacSchedDLPerfStats;

    LteUePdcpULPerfStats lteUePdcpULPerfStats;
    LteUePdcpDLPerfStats lteUePdcpDLPerfStats;
    LteUePdcpRadioBearerInfoStats lteUePdcpRadioBearerInfoStats[MAX_PDCP_LC_ID]; 
    
    LteUeRlcDLPerfStats lteUeRlcDLPerfStats;
    LteUeRlcULPerfStats lteUeRlcULPerfStats;
}LteUePerfStats;


/* Structure Name : LtePerfStatsConfig
 * Purpose        : Contains configuration both cell specific and UE specific  stats*/
typedef struct LtePerfStatsControlInfoT
{
    /* Stores active index for MAC shared memory stats */
    UInt8 macActiveIndex;

    /* Stores active index for RLC DL shared memory stats */
    UInt8 rlcDLActiveIndex;

    /* Stores active index for RLC UL shared memory stats */
    UInt8 rlcULActiveIndex;

    /* Stores active index for PDCP shared memory stats */
    UInt8 pdcpActiveIndex;

}LtePerfStatsControlInfo;

typedef struct LtePerfStatsT
{
    /*Stores last calculated TTI for MAC */
    /* SPR 15909 fix start  */
    tickType_t macLastCalculatedTTI;

    /*Stores last calculated TTI for RLC DL*/
    tickType_t rlcDLLastCalculatedTTI;

    /*Stores last calculated TTI for RLC UL*/
    tickType_t rlcULLastCalculatedTTI;

    /*Stores last calculated TTI for PDCP*/
    tickType_t pdcpLastCalculatedTTI;
    /* SPR 15909 fix end */
	/* + PERF_CA */
    LteCellPerfStats lteCellPerfStats[MAX_NUM_CELL];
	/* - PERF_CA */

    LteUePerfStats lteUePerfStats[PERF_MAX_UE_SUPPORTED];
}LtePerfStats;

/* Structure Name : LtePerfStatsInfo
 * Purpose        : Shared Memory structure used for PerfStats*/
typedef struct LtePerfStatInfoT
{
    SInt32 shmId;
    LtePerfStatsControlInfo *perfStatsControlInfo;
    LtePerfStats *perfStats;
}LtePerfStatInfo;

/* Structure name  :PdcpLastTtiVolDlT
 * Purpose      :Contains statistics for pdcpLastTtiTransVolDl  UE Perf Stats. */

typedef struct PdcpLastTtiVolDlT
{
 UInt32 pdcpDataSize[MAX_INDEX];
 UInt32 currentIndex;
 UInt32 previousIndex;
}PdcpLastTtiVolDl;
 
/* Structure name  : CqiRIPerfStats
 * Purpose         : Indicates CQI Stats */
typedef struct CqiRIPerfStatsT {
    UInt32 totalCQICW1Reported[MAX_CQI_INDEX];  //Samples with CQI reported by UE
    UInt32 totalCQICW2Reported[MAX_CQI_INDEX];  //Samples with CQI related to rank 2 transmissions, reported by UE.
    UInt32 totalRankReported[MAX_RANK_INDICATION]; /*Samples with RANK reported by UE in open and closed 
                                                         loop Spatial Multiplexing. */
} CqiRIPerfStats;

/* Structure name  : HarqPerfStats
 * Purpose         : Indicates HARQ Stats */
typedef struct HarqPerfStatsT {
    UInt32 totalDLHarqAck[MOD_SCHEME];  /* total number of successful HARQ ACK from UE in DL 
                                           using QPSK /16QAM/64QAM modulation. */
    UInt32 totalDLHarqNack[MOD_SCHEME];  /*total number of successful HARQ NACK from UE in DL 
                                             using QPSK /16QAM/64QAM modulation */
    UInt32 totalULHarqSucc[MOD_SCHEME];  /* total number of successful HARQ ACK from UE in UL
                                             using QPSK /16QAM/64QAM modulation */
    UInt32 totalULHarqFail[MOD_SCHEME];  /* total number of successful HARQ NACK from UE in UL
                                             using QPSK /16QAM/64QAM modulation */
    UInt32 totalHarqFail;         /* Number of times the HARQ fails  */
}  HarqPerfStats;

/* Structure name  : UeCapabilityStats
 * Purpose         : Contains the statistics for UE Capability   */
typedef struct UeCapabilityStatsT
{
    UInt32 totalTimeRestrictUeInDlbyUeCap;  /*Total contributions(in ms) from each UE where 
                                              respective UE is limited in the DL direction 
                                              by its UE capability. */
    UInt32 totalTimeRestrictUeInUlbyUeCap;  /*Total contributions(in ms) from each UE where 
                                              respective UE is limited in the UL direction 
                                              by its UE capability. */
} UeCapabilityStats;
    

/* Structure name  : UePowerLimitedStats
 * Purpose         : Contains UE Power limited Statistics   */
typedef struct UePowerLimitedStatsT
{
    UInt32 totalTbScheduledInUlOnPowerRestriction; /*Total Transport Blocks on MAC level 
                                                    scheduled in uplink where the UE was 
                                                    considered to be power limited. */
    UInt32 totalTbScheduledInUlOnNoPowerRestriction; /*Total Transport Blocks on MAC level 
                                                      scheduled in uplink where the UE was 
                                                      Not considered to be power limited. */
} UePowerLimitedStats;

/* Structure name  : UeTransmissionModeRIStats
 * Purpose         : Contains Stats for UE Transmission at particular
 *                   TX Mode and RI Index     */
typedef struct UeTransmissionModeRIStatsT
{
    UInt32 totalDLTransmissionsWithTX2RI1;         /*Total number of transmissions with 
                                                     transmit diversity and RI = 1. */
    UInt32 totalDLTransmissionsWithTX3RI[MAX_RANK_INDICATION]; /*Total number of transmissions with 
                                                                   open loop MIMO  and RI = 1/2. */
    UInt32 totalDLTransmissionsWithTX4RI[MAX_RANK_INDICATION]; /*Total number of transmissions with 
                                                                  closed loop MIMO  and RI = 1/2 */
} UeTransmissionModeRIStats;

/* Structure name  : UeSINRPerfStats
 * Purpose         : Contains the statistics for UE SINR   */
typedef struct UeSINRPerfStatsT
{
    UInt32 totalPUCCHSINRReported[MAX_SINR_DIFF_INDEX]; /*Samples with SINR at PUCCH */
    UInt32 totalPUSCHSINRReported[MAX_SINR_DIFF_INDEX]; /* Samples with SINR at PUSCH */
} UeSINRPerfStats;

/* Structure Name  : UePRBUsageStats
 * Purpose         : Contains the statistics for UE PRB Usage  */
typedef struct UePRBUsageStatsT
{
    UInt32 totalREsUsedInUL; /* Total RE's used for transmission in the UL. */  
    UInt32 totalDLREsUsage;  /* Total RE's used for transmission in the DL. */
} UePRBUsageStats;

/* Structure name  : RadioThpVolStats
 * Purpose         : Contains the statistics for data volume on MAC  */
typedef struct RadioThpVolStatsT
{
    UInt32 totalDlBytesSent;   /*The total successfully transferred data volume on 
                                        MAC level in the downlink. */
    UInt32 totalUlByteReceived; /*The total successfully transferred data volume on 
                                        MAC level in the uplink. */
    UInt32 totalUlThpPerUeTrunk; /*The UL volume used for UL UE Throughput. */
} RadioThpVolStats;

/* Structure name  : UeMacPerfStatsResp
 * Purpose         : Contains the statistics for UE MAC Perf Stats   */
typedef struct UeMacPerfStatsRespT {  
    UInt32 ueindex;
    CqiRIPerfStats cqiRIPerfStats;     /*Indicates CQI Stats */
    HarqPerfStats harqPerfStats;       /*Indicates HARQ Stats*/
    UeCapabilityStats ueCapabilityStats; /*Indicates UE Capability Stats*/
    UePowerLimitedStats uePowerLimitedStats;/*Indicates UE Power Limited Stats*/
    UeTransmissionModeRIStats ueTransmissionModeRIStats; /*Indicates UE Transmission at particular 
								TX mode and RI Stats */
    UeSINRPerfStats ueSINRPerfStats;   /*Indicates UE SINR Stats */
    UePRBUsageStats uePRBUsageStats;   /*Indicates UE PRB Usage Stats */
    RadioThpVolStats radioThpVolStats; /*Indicates data volume on MAC */
} UeMacPerfStatsResp ;

/*Wiresect Changes*//*Done*/
/* Structure Name  : CellHarqPerfStats
 * Purpose         : Contains Statistics for  number of PRBs used for 
 *                   HARQ initial transmissions or retransmissions.    */
typedef struct CellHarqPerfStatsT
{
    UInt32 totalDLPRBUsageforHarq[MAX_QCI];  /*^ M, 0, OCTET_STRING, FIXED ^*/ /*Number of PRBs used in DL per cell per 
                                              QCI for HARQ initial transmissions. */
    UInt32 totalDLPRBUsageforHarqRetx;      /*^ M, 0 ^*/ /*Number of PRBs used in DL per cell for 
                                                  HARQ retransmissions. */
} CellHarqPerfStats;

/* Structure Name  : RachSuccFailPerfStats
 * Purpose         : Indicates the RACH related Performance Stats  */
typedef struct RachSuccFailPerfStatsT {   
    UInt32 totalRcvdMsg3RandomAccessPreamblesCBRA;   /*^ M, 0 ^*/ /*Number of successfully detected 
                                                          RA Msg3 for CBRA */
    UInt32 totalRcvdMsg3RandomAccessPreamblesCFRA;  /*^ M, 0 ^*/ /*Number of successfully completed 
                                                          CFRA procedures */
    UInt32 totalFailedRandomAccessPreamblesMsg2CBRA;  /*^ M, 0 ^*/ /*Number of CBRA preambles for which 
                                                          no RA Msg2 was sent */
    UInt32 totalFailedRandomAccessPreamblesMsg2CFRA;  /*^ M, 0 ^*/ /*Number of CFRA preambles for which 
                                                          no RA Msg2 was sent */
    UInt32 totalAllocatedRandomAccessPreambles;     /*^ M, 0 ^*/ /*Number of allocated CFRA preambles. */
    UInt32 totalSuccRandomAccessTA[MAX_TA_RANGE];   /*^ M, 0, OCTET_STRING, FIXED ^*/ /*Number of detected successfull random 
                                                    access with a TA value in the range 
                                                    0..9%, 10..19%, 20..29% and so on. */
    UInt32 totalUnassignedFalseCFRA;   /*^ M, 0 ^*/ /*The number of detected Contention Free Random Access 
                                        preambles that are not allocated to any UE */
    UInt32 totalUnassignedSumCFRA;   /*^ M, 0 ^*/ /* The total number of unassigned Contention Free Random 
                                       Access preambles at each PRACH occasion during the 
                                        reporting period. */
} RachSuccFailPerfStats;

/* Structure Name  : PdcchUtilizationStats
 * Purpose         : Contains PDCCH Utilizatin Statistics  */
typedef struct PdcchUtilizationStatsT
{
    UInt32 totalPdcchCceUtil[MAX_CCE_UTIL_INDEX];  /*^ M, 0, OCTET_STRING, FIXED ^*/ /*Number of samples with PDCCH utilization 
                                                      in the range 0-5%. */
    UInt32 noOfSubFrameWithCceUtilMoreThanZero;  /*^ M, 0 ^*/ /*Number of subframes with PDCCH CCE 
                                                  utilization greater than 0 CCEs.*/
    UInt32 noOfSubFrameWithPdcchCfi[MAX_CFI];  /*^ M, 0, OCTET_STRING, FIXED ^*/ /*Number of subframes with PDCCH CFI set to 1, 
                                                    CFI set to 2 and CFI set to 2.*/
    UInt32 totalCceAvail;   /*^ M, 0 ^*/ /* The total max number of CCEs available in a cell */
} PdcchUtilizationStats;

/* Structure Name  : CellPRBUsagePerfStats
 * Purpose         : Contains Cell PRB Usage Statistics  */
typedef struct CellPRBUsagePerfStatsT
{
    UInt32 totalULPRBUsageSRB[MAX_SRB]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt32 totalDLPRBUsageSRB[MAX_SRB]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt32 totalDLPRBUsagePCCH; /*^ M, 0 ^*/
    UInt32 totalDLPRBUsageBCCH; /*^ M, 0 ^*/
    UInt32 totalDLPRBAvailable; /*^ M, 0 ^*/
    UInt32 totalULPRBAvailable; /*^ M, 0 ^*/
} CellPRBUsagePerfStats;

/* Structure Name  : CellMacPerfStatsResp
 * Purpose         : Contains the statistics for Cell MAC Perf Stats   */

/*+ Dyn UEs To Be SchPerTTI +*/
/* Structure Name : TotalActiveUEsInDlUlStats 
 * Purpose        : Contains Total Active UE in DL/UL */
typedef struct TotalActiveUEsInDlUlStatsT
{
    UInt16 totalActiveUEsInDl; /*^ M, 0 ^*/
    UInt16 totalActiveUEsInUl; /*^ M, 0 ^*/
} TotalActiveUEsInDlUlStats;

/*- Dyn UEs To Be SchPerTTI -*/
typedef struct CellMacPerfStatsRespT {
    /* SPR 15909 fix start */
    tickType_t durationTTI; /*^ M, 0 ^*/      /*Indicates the duration for which PERF STATS 
                                    need to be provided to OAM. */
    /* SPR 15909 fix end */
    CellHarqPerfStats cellHarqPerfStats; /*^ M, 0 ^*/ /*Indicates number of PRBs used for HARQ 
                                           initial transmissions or retransmissions */
    RachSuccFailPerfStats rachSuccFailPerfStats; /*^ M, 0 ^*/ /*structure indicating the RACH related 
                                                       Performance Stats */
    PdcchUtilizationStats pdcchUtilizationStats; /*^ M, 0 ^*/ /*structure indicating PDCCH Utilization Stats */
    UInt32 totalPUCCHResAllocationAttempts; /*^ M, 0 ^*/ /*Structure indicating PUCCH Utilization Stats */
    CellPRBUsagePerfStats cellPRBUsagePerfStats; /*^ M, 0 ^*/ /*Structure indicating cell PRB Usage stats */
    /*+ Dyn UEs To Be SchPerTTI +*/
    TotalActiveUEsInDlUlStats activeUEsInDlUlStats; /*^ M, 0 ^*/ /* Structure indicating Total Active 
                                                            UE in DL/UL Stats */
    /*- Dyn UEs To Be SchPerTTI -*/
} CellMacPerfStatsResp; /*^ API, RRM_MAC_CELL_PERF_STATS_IND ^*/

/*Wiresect Changes*/
/* Structure Name   : PdcpRohcPerfStats
 * Purpose          : Indicates PDCP ROHC PERF STATS.  */
typedef struct PdcpRohcPerfStatsT {
/* Indicates Total uncompressed header volume that has been
 * received in the downlink direction in the PDCP layer per QCI */
    UInt32 totalRohcUnCmpHdrVolDL[MAX_QCI];
/* Indicates Total compressed header volume that has been
 * transmitted in the downlink direction in the PDCP layer per QCI */
    UInt32 totalRohcCmpHdrVolDL[MAX_QCI];
/* Indicates Total decompressed or uncompressed header volume
 * that has been transmitted in the uplink direction in the PDCP layer per QCI */
    UInt32 totalRohcUnCmpHdrVolUL[MAX_QCI];
/* Indicates Total compressed header volume that has been received
 * in the uplink direction in the PDCP layer per QCI */
    UInt32 totalRohcCmpHdrVolUL[MAX_QCI];
} PdcpRohcPerfStats;

/* Structure Name   : PdcpCellPerfStatsResp
 * Purpose          : Contains the statistics  for Cell PDCP PERF STATS   */
typedef struct PdcpCellPerfStatsRespT {
    UInt32 durationt;  //duration for which PERF STATS need to be provided to OAM.
    PdcpRohcPerfStats pdcpRohcPerfStats; //Indicates PDCP ROHC PERF STATS. 
} PdcpCellPerfStatsResp , *LP_PdcpCellPerfStatsResp ;

/* Structure Name     : PdcpRbPerfStats
 * Purpose            : Indicates PDCP RB PERF STATS */
typedef struct PdcpRbPerfStatsT {
/*  Indicates DRB volume that has been transmitted in the 
 *  downlink direction in the PDCP layer. */
    UInt32 totalDlDrbBytesSent[MAX_LC_IDENTITY];
/* Indicates DRB volume in UL for PDCP SDUs */
    UInt32 totalUlDrbBytesSent[MAX_LC_IDENTITY];
/*  Indicates Total SRB volume that has been transmitted in the 
 *  downlink direction in the PDCP layer */
    UInt32 totalDlSrbBytesSent;
/*  Indicates Total SRB volume in UL for PDCP SDUs */    
    UInt32 totalUlSrbBytesRcvd;
/* Indicates Total RB volume(SRB+DRB Volume) that has been transmitted 
 * in the downlink direction in the PDCP layer. */
    UInt32 totalDlRbBytesSent;
/* Indicates Total RB volume (SRB+DRB Volume) in UL for PDCP SDUs */
    UInt32 totalUlRbBytesSent;
/* Indicates Number of PDCP SDUs received for a DRB, UL */
    UInt32 numberOfUlDrbSduReceived[MAX_LC_IDENTITY];
/* Indicates Number of DL PDCP SDUs received per DRB */
    UInt32 numberOfDlDrbSduReceived[MAX_LC_IDENTITY];
} PdcpRbPerfStats;

/* Structure Name     : PdcpUeRadioBearerInfoStats
 * Purpose            : Contains UE Radio Bearer Statistics  */
typedef struct PdcpUeRadioBearerInfoStatsT
{
    UInt32 bearerId;   /*Indicates bearer Id. */
    UInt32 bearerType; /*Indicates bearer Type. */
    UInt32 qci;       /*Indicates QCI value */
    UInt32 rlcMode;   /* Indicates RLC Mode used */
} PdcpUeRadioBearerInfoStats;

/* Structure Name     : PdcpUeVolPerfStats
 * Purpose            : Contains Statistics for PDCP volume, measured on UE level */
typedef struct PdcpUeVolPerfStatsT
{ 
/* Number of DL PDCP SDUs discarded due to HO per DRB */
    UInt32 totalDlDrbSduDiscardedHO[MAX_LC_IDENTITY];
/* Indicates the total volume (PDCP SDU) on Data Radio Bearers that has been
 * transmitted and acknowledged in the downlink direction */
    UInt32 totalDLSrbBytesSentAndAck;
/* Indicates the  transmitted PDCP volume for the last piece of data
 *  in large data bursts on RB level */
    UInt32 pdcpLastTtiVolDl[MAX_LC_IDENTITY]; 
} PdcpUeVolPerfStats;

/* Structure Name     : PdcpUePerfStatsResp
 * Purpose            : Contains the statistics for Ue PDCP PERF STATS   */
typedef struct PdcpUePerfStatsRespT {
    UInt32 ueIndex; /* Indicates the UE Index */
    /*Structure containing PDCP RB PERF STATS */
    PdcpRbPerfStats pdcpRbPerfStats;
    /*Indicates UE radio bearer Info STATS */
    PdcpUeRadioBearerInfoStats pdcpUeRadioBearerInfoStats[MAX_PDCP_LC_ID];
    /*It indicates the PDCP volume, measured on UE level. */
    PdcpUeVolPerfStats pdcpUeVolPerfStats;
} PdcpUePerfStatsResp , *LP_PdcpUePerfStatsResp ;

/* Structure Name   :  RlcUePerfStatsResp
 * Purpose          :  Contains the Statistics for Ue RLC PERF  */
typedef struct RlcRBPerfStatsT {
     /* SPR 5453: Fix Start */
   UInt32 totalDlRLCUMSduTransmitted[MAX_LC_IDENTITY] ;     /*Indicates total number of RLC SDU transmitted to MAC
                                                              in DL for RLC UM bearers per LC */
     /* SPR 5453: Fix End */
} RlcRBPerfStats, *LP_RlcRBPerfStats; 

/* Structure Name   : UeAckNackPerfStats
 * Purpose          : Contains UE Ack Nack Perf Stats  */
typedef struct UeAckNackPerfStatsT
{
    UInt32 totalDLRLCAcks; /*The total number of RLC ACK transmitted in DL */
    UInt32 totalULRLCAcks; /*The total number of RLC ACK received in UL */
    UInt32 totalULRLCNacks; /*The total number of RLC NACK received in UL */
} UeAckNackPerfStats, *LP_UeAckNackPerfStats;

/* Structure Name   :  RlcUePerfStatsResp
 * Purpose          :  Contains the Statistics for Ue RLC PDCP PERF  */
typedef struct RlcUePerfStatsRespT {
    UInt32 ueIndex;          /*Indicates UE index */
    RlcRBPerfStats rlcRBPerfStat ; /*Indicates RLC RB PERF STATS.*/
    UInt32 totalMaxRetxExceeded; /*Total number of times maximum number of RLC 
                                      retransmissions have exceeded */
    UeAckNackPerfStats ueAckNackPerfStats; /*Indicates the UE Ack Nack Perf Stats */
} RlcUePerfStatsResp, *LP_RlcUePerfStatsResp ; 

/* Structure Name : MacPerfStats
 * Purpose        : Contains the statistics for Mac Stats */
typedef struct MacPerfStatsT
{
   /* SPR 15909 fix start */
    tickType_t lastMACCalculatedTTI;
    /* SPR 15909 fix end */
    UeMacPerfStatsResp    lteMacUePerfStats[PERF_MAX_UE_SUPPORTED];
	/* + PERF_CA */
    CellMacPerfStatsResp  lteMacCellPerfStats[MAX_NUM_CELL];
	/* - PERF_CA */
}MacPerfStats;

/* Structure Name : PdcpPerfStats
 * Purpose        : Contains the statistics for PDCP Stats  */
typedef struct PdcpPerfStatsT
{
    /* SPR 15909 fix start */
    tickType_t lastPDCPCalculatedTTI;
    /* SPR 15909 fix end */
    PdcpUePerfStatsResp   ltePdcpUePerfStats[PERF_MAX_UE_SUPPORTED];
	/* + PERF_CA */
    PdcpCellPerfStatsResp ltePdcpCellPerfStats[MAX_NUM_CELL];
	/* - PERF_CA */
}PdcpPerfStats;

/* Structure Name : RlcPerfStats
 * Purpose        : Contains the statistics for RLC stats */
typedef struct RlcPerfStatsT
{
    /* SPR 15909 fix start */
    tickType_t lastRLCCalculatedTTI;
    /* SPR 15909 fix end */
    RlcUePerfStatsResp    lteRlcUePerfStats[PERF_MAX_UE_SUPPORTED];
}RlcPerfStats;

/* Structure Name : LtePerfConfig
 * Purpose        : Contains common information for PERF STAT 
 *                  This will come from OAM */ 
typedef struct LtePerfStatsConfigT 
{
    /*Indicates the group of events to be reported to OAM  */
    UInt32  cellPerfStatsToReportBitMap;
    
    /*Indicates whether  is configured or not */
    UInt32  uePerfStatsToReportBitMap;
    
    /*Indicates Periodic Reporting Required or not */
    UInt32  periodicReporting;
    
    /*Indicates the duration for which PERF STATS need to be provided */
    UInt32  duration;

    UInt32  transactionId;

}LtePerfStatsConfig;

/* Structure Name : PerfStatsContainer
 * Purpose        : Contains the statistics for container type for Perf Stats Container */
typedef struct PerfStatsContainerT
{
    PerfStatsType     macOrPdcpOrRlcStats;
    union{
	LtePerfStatsConfig  lteMacPerfStatsConfig;
	LtePerfStatsConfig  ltePdcpPerfStatsConfig;
	LtePerfStatsConfig  lteRlcPerfStatsConfig;
    }perfConfig;
    union{
	MacPerfStats lteMacPerfStats;
	PdcpPerfStats ltePdcpPerfStats;
	RlcPerfStats lteRlcPerfStats;
    }perfStats;
}PerfStatsContainer;

/* Structure Name : PerfStatsScanner
 * Purpose        : Contains the statistics for Perf Stats Scanner object */
typedef struct PerfStatsScannerT
{
    UInt8              module_bitmask;
    /* SPR 15909 fix start  */
    tickType_t             timeoutTTI[MAX_PERF_STATS_CONTAINER];
    tickType_t             prevPerfStats_tick;
    /* SPR 15909 fix end */
    PerfStatsContainer perfStatsContainer[MAX_PERF_STATS_CONTAINER];
}PerfStatsScanner;

/* Structure definition for API : RLC_CONFIGURE_PERF_STATS_REQ
 * Structure Name               : RlcConfigurePerfStatsReq  */
typedef struct RlcConfigurePerfStatsReqT {
    UInt32 duration ;                   /*Indicates timer value */
    UInt32 periodicReporting ;        /*Indicates Periodic Reporting Required or not*/
    UInt32 uePerfStatsToReportBitMap ; /*Each bit of this bitmap represents PERF STATS for the UE*/
} RlcConfigurePerfStatsReq , *LP_RlcConfigurePerfStatsReq ;

/* Structure definition for API : RLC_CONFIGURE_PERF_STATS_CNF
 * Structure Name               : RlcConfigurePerfStatsCnf */
typedef struct RlcConfigurePerfStatsCnfT  {
    UInt32 responseCode ;               //RLC_SUCCESS or an Error Code
} RlcConfigurePerfStatsCnf , *LP_RlcConfigurePerfStatsCnf ;

/* Structure definition for API : RLC_UE_PERF_STATS_IND
 * Structure Name               : RlcUePerfStatsInd   */
typedef struct RlcUePerfStatsIndT  {
    UInt32 duration;  //It indicates the duration for which PERF STATS need to be provided to OAM
    UInt32 numOfUePresent;  //Indicates Number of UE Present
    RlcUePerfStatsResp rlcUePerfStatsResp[RLC_OAM_VAR_SIZE(numOfUePresent)];   // Contains the statistics for Ue RLC PDCP PERF
} RlcUePerfStatsInd , *LP_RlcUePerfStatsInd;
 
/* Structure definition for API : RLC_GET_PERF_STATS_REQ
 * Structure name               : RlcGetPerfStatsReq   */
typedef struct RlcGetPerfStatsReqT {
    UInt32 resetStats ;             /* 0 (false) , 1 (true) */
    UInt32 uePerfStatsToGetBitMap ; /*Each bit of this bitmap represents PERF STATS for the UE*/
}  RlcGetPerfStatsReq , *LP_RlcGetPerfStatsReq ;
 
/* Structure definition for API : RLC_GET_UE_PERF_STATS_CNF
 * Structure Name               : RlcGetUePerfStatsReqCnf */
typedef struct RlcGetUePerfStatsCnf {
    UInt32 responseCode;  /*RLC_SUCCESS or an Error Code*/
    UInt32 duration;    /*It indicates the duration for which PERF STATS 
                             need to be provided to OAM. */
    UInt32 numOfUePresent;   /*Indicates Number of UE Present */
    RlcUePerfStatsResp rlcUePerfStatsResp[RLC_OAM_VAR_SIZE(numOfUePresent)]; /*Contains the statistics 
                                                                               for Ue RLC PDCP PERF */
} RlcGetUePerfStatsCnf , *LP_RlcGetUePerfStatsCnf ;

/* Structure definitions for API : MAC_CONFIGURE_PERF_STATS_REQ
 * Structure name                : MacConfigurePerfStatsReqParams  */
typedef struct MacConfigurePerfStatsReqParamsT  {
    UInt32 duration;                        //Indicates timer value
    UInt32 periodicReporting;               //Indicates Periodic Reporting Required or not
    UInt32 UePerfStatsToReportBitMap;       //Each bit of this bitmap represents Ue Perf Stats
    UInt32 CellPerfStatsToReportBitMap;     //Each bit of this bitmap represents Cell Perf Stats
} MacConfigurePerfStatsReqParams;
/* SPR 9680 changes start */
/* Structure definitions for API : MAC_CONFIGURE_PERF_STATS_RESP
 * Structure name                : MacConfigurePerfStatsCnfParams  */
/* SPR 9680 changes end */
typedef struct  MacConfigurePerfStatsCnfParamsT {
    UInt32  responseCode;              // MAC_SUCCESS or an Error Code
} MacConfigurePerfStatsCnfParams;

/* Structure definitions for API : MAC_UE_PERF_STATS_IND
 * Structure name                : MacUePerfStatsIndParams */
typedef struct MacUePerfStatsIndParamsT {
    /* SPR 10577 changes start */
    UInt8 endIndicator;
    /* padding is added for byte alignment,this is reserved and can 
     * be used for new fields in future.Its value should be ignored*/
    UInt8 padding[3];	
    /* SPR 10577 changes end */
    UInt32 duration;
    UInt32 numOfUePresent;
    UeMacPerfStatsResp ueMacPerfStatsResp[MAC_OAM_VAR_SIZE(numOfUePresent)];  //Contains the statistics for Ue Mac Perf Stats
} MacUePerfStatsIndParams;

/* Structure definitions for API : MAC_CELL_PERF_STATS_IND
 * Structure name                : MacCellPerfStatsIndParams  */
typedef struct MacCellPerfStatsIndParamsT {
    CellMacPerfStatsResp cellMacPerfStatsResp;  //Contains the statistics for Cell Mac Perf Stats
} MacCellPerfStatsIndParams ;

/* Structure definitions for API : MAC_GET_PERF_STATS_REQ
 * Structure name                : MacGetPerfStatsReqParams  */
typedef struct  MacGetPerfStatsReqParamsT {
    UInt32 resetStats;                 //0 (false) , 1 (true)
    UInt32 uePerfStatsToGetBitMap;     //Each bit of this bitmap represents UE Perf Stats
    UInt32 cellPerfStatsToGetBitMap; //Each bit of this bitmap represents CELL Perf Stats
}  MacGetPerfStatsReqParams;

/* SPR 9680 changes start */
/* Structure definitions for API : MAC_GET_UE_PERF_STATS_RESP
 * Structure name                : MacGetUePerfStatsCnfParams */
/* SPR 9680 changes end */
typedef struct  MacGetUePerfStatsCnfParamsT {
    UInt32 responseCode;                      // MAC_SUCCESS or an Error Code
    /* SPR 10577 changes start */
    /* padding is added for byte alignment,this is reserved and can 
     * be used for new fields in future.Its value should be ignored*/
    UInt8 endIndicator;
    UInt8 padding[3];	
    /* SPR 10577 changes end */
    UInt32 duration;
    UInt32 numOfUePresent;
    UeMacPerfStatsResp ueMacPerfStatsResp[MAC_OAM_VAR_SIZE(numOfUePresent)]; //Contains the statistics for UE MAC Perf Stats
}  MacGetUePerfStatsCnfParams;

/* SPR 9680 changes start */
/* Structure definitions for API : MAC_GET_CELL_PERF_STATS_RESP
 * Structure name                : MacGetCellPerfStatsCnfParams  */
/* SPR 9680 changes end */
typedef struct MacGetCellPerfStatsCnfParamsT {
    UInt32 responseCode;                            // MAC_SUCCESS or an Error Code
    CellMacPerfStatsResp cellMacPerfStatsResp;  //Contains the statistics for cell MAC Perf Stats
} MacGetCellPerfStatsCnfParams ;

/* Structure Definition for API : PDCP_CONFIGURE_PERF_STATS_REQ
 * Structure name		: PdcpConfigurePerfStatsReqParams */
typedef struct PdcpConfigurePerfStatsReqParamsT {
    UInt32 duration;                      // Indicates timer value
    UInt32 periodicReporting;             //Indicates Periodic Reporting Required or not
    UInt32 uePerfStatsToReportBitMap;   //Each bit of this bitmap represents PERF STATS for the UE 
    UInt32 cellPerfStatsToReportBitMap; //Each bit of this bitmap represents PERF STATS for the Cell  
} PdcpConfigurePerfStatsReqParams , *LP_PdcpConfigurePerfStatsReqParams;

/* Structure Definitoin for API : PDCP_CONFIGURE_PERF_STATS_CNF
 * Structure name               : PdcpConfigurePerfStatsCnfParams */
typedef struct PdcpConfigurePerfStatsCnfParamsT {
    UInt32 responseCode;              // PDCP_SUCCESS or an Error Code
}PdcpConfigurePerfStatsCnfParams , *LP_PdcpConfigurePerfStatsCnfParams;

/* Structure Definition for API : PDCP_CELL_PERF_STATS_IND
 * Structure name                : PdcpCellPerfStatsIndParams  */
typedef struct  PdcpCellPerfStatsIndParamsT {
    PdcpCellPerfStatsResp pdcpCellPerfStatsResp ;   //Contains the statistics for Cell PDCP PERF STATS
} PdcpCellPerfStatsIndParams , *LP_PdcpCellPerfStatsIndParams;

/* Structure Definition for API : PDCP_UE_PERF_STATS_IND
 * Structure name               : PdcpUePerfStatsIndParams  */
typedef struct PdcpUePerfStatsIndParamsT {
    /* SPR 10577 changes start */
    UInt8 endIndicator;
    /* padding is added for byte alignment,this is reserved and can 
     * be used for new fields in future.Its value should be ignored*/
    UInt8 padding[3]; 
    /* SPR 10577 changes end */
    UInt32 duration;    //duration for which PERF STATS need to be provided to OAM
    UInt32 numOfUePresent;  //Indicates Number of UE Present
    PdcpUePerfStatsResp pdcpUePerfStatsResp[PDCP_OAM_VAR_SIZE(numOfUePresent)] ;    //Contains the statistics  for  Ue PDCP PERF STATS 
} PdcpUePerfStatsIndParams , *LP_PdcpUePerfStatsIndParams ;

/* Structure Definition for API : PDCP_GET_PERF_STATS_REQ
 * Structure name               : PdcpGetPerfStatsReqParams */
typedef struct  PdcpGetPerfStatsReqParamsT {
    UInt32 resetStats;                  // 0 (false) , 1 (true)
    UInt32 uePerfStatsToGetBitMap;     //Each bit of this bitmap represents PERF STATS for the UE
    UInt32 cellPerfStatsToGetBitMap;   //Each bit of this bitmap represents PERF STATS for the Cell
}  PdcpGetPerfStatsReqParams , *LP_PdcpGetPerfStatsReqParams ;

/* Structure Definition for API : PDCP_GET_CELL_PERF_STATS_CNF
 * Structure name               : PdcpGetCellPerfStatsCnfParams   */
typedef struct PdcpGetCellPerfStatsCnfParamsT {
    UInt32 responseCode;                             //PDCP_SUCCESS or an Error code
    PdcpCellPerfStatsResp pdcpCellPerfStatsResp;     //Contains the statistics  for Cell PDCP PERF STATS 
} PdcpGetCellPerfStatsCnfParams , *LP_PdcpGetCellPerfStatsCnfParams;

/* Structure Definition for API : PDCP_GET_UE_PERF_STATS_CNF
 * Structure name               : PdcpGetUePerfStatsCnfParams   */
typedef struct PdcpGetUePerfStatsCnfParamsT {
    UInt32 responseCode;      //PDCP_SUCCESS or an Error code
    /* SPR 10577 changes start */
    UInt8 endIndicator; 
    /* padding is added for byte alignment,this is reserved and can 
     * be used for new fields in future.Its value should be ignored*/
    UInt8 padding[3]; 
    /* SPR 10577 changes end */
    UInt32 duration;         //It indicates the duration for which PERF STATS need to be provided to OAM
    UInt32 numOfUePresent;   //Indicates Number of UE Present
    PdcpUePerfStatsResp pdcpUePerfStatsResp[PDCP_OAM_VAR_SIZE(numOfUePresent)];//Contains the statistics for UePDCP PERF STATS 
} PdcpGetUePerfStatsCnfParams , *LP_PdcpGetUePerfStatsCnfParams ;

extern LtePerfStatInfo gPerfStats;
extern LtePerfStatsControlInfo *gPerfStatsControlInfo_p;
/* + PERF_CA */
extern LteCellPerfStats *gMacCellPerfStats_p[MAX_NUM_CELL] ;
extern LteUePerfStats *gMacUePerfStats_p;
extern LteCellPerfStats *gPdcpCellPerfStats_p[MAX_NUM_CELL];
extern LteUePerfStats *gPdcpUePerfStats_p;
extern LteCellPerfStats *gRlcCellPerfStats_p[MAX_NUM_CELL];
/* - PERF_CA */
extern LteUePerfStats *gRlcUePerfStats_p;
/** CA-TDD Changes Start **/
extern PerfStatsMibSyncSigType perfStatsMibSyncSignalFlag_g[MAX_NUM_CELL];
/** CA-TDD Changes Stop **/
extern UInt32 overlapRBsMibSS_g[MAX_NUM_CELL];
/* + PERF_CA GBL */
extern UInt32 reUsedForPssSssPerPRB_g[MAX_NUM_CELL];
extern UInt32 reUsedForMibPerPRB_g[MAX_NUM_CELL];
/* - PERF_CA GBL */
extern UInt32 csrREsExcludingFirstSymbol_g[MAX_NUM_CELL];
extern UInt32 central6RBStartIndex_g[MAX_NUM_CELL];
extern UInt32 rePerRBPair_g[MAX_CP][MAX_CFI];

/* End of change for VARIBLE Declaration */

extern PdcpLastTtiVolDl pdcpLastTtiTransVolDl_g [PERF_MAX_UE_SUPPORTED][MAX_LC_IDENTITY];

/* SPR 5449 changes start */
extern UInt8 ueResponseBuf_g[40000];
/* SPR 5449 changes end */

extern PerfStatsScanner g_perfStatsScannerObj;

/* Perf stats RRM-MAC start */
/* + SPR 17439 */
extern void updatePdcpPerfContainer(void);
extern void updateRlcPerfContainer(void);
/* - SPR 17439 */
extern void  rlcResetUePerfCounters (LteUePerfStats *pLteUePerfStats);
extern void  pdcpResetCellPerfCounters(LteCellPerfStats *pLteCellPerfStats);
extern void  pdcpResetUePerfCounters(LteUePerfStats *pLteUePerfStats);
extern void macResetCellPerfStatCounters(LteCellPerfStats *cellPerfStats_p);
extern void macResetUePerfStatCounters(LteUePerfStats *uePerfStats_p);
extern void resetMacPerfContainer(UInt8 resetId);
/* Perf stats RRM-MAC end */
/*Wiresect Changes*//*Done*/

/* SPR 15876 FIX START */
//#endif
/* SPR 15876 FIX END */
#endif
