/****************************************************************************

 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacULUEContext.h,v 1.1.1.1.6.1.4.2.2.2.8.1 2010/11/08 06:06:13 gur10121 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains Uplink UE Context related Structures
 *                     and global variables.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacULUEContext.h,v $
 * Revision 1.1.1.1.6.1.4.2.2.2.9.1  2012/07/20 14:50:13  gur31292
 * Changes related to DCI Format3A
 *
 * Revision 1.1.1.1.6.1.4.2.2.2.8.1  2010/11/08 06:06:13  gur10121
 * Semi static changes merging
 *
 * Revision 1.1.1.1.6.1.4.2.2.2.2.2  2010/10/27 17:13:47  gur10121
 * UT fixes + review comments incorporation
 *
 *
 * Revision 1.1.1.1.6.1.4.2.2.1.6.2  2010/10/15 13:32:06  gur20439
 * changes to support PUSCH config
 *
 * Revision 1.1.1.1.6.1.4.2.2.1.6.1  2010/10/14 08:38:33  gur10121
 * Semi Static flag MAC_SEMI_STATIC_INF
 *
 * Revision 1.1.1.1.6.1.4.2.2.2  2010/10/14 17:18:12  gur19413
 * DMRS FIXES
 * Revision 1.1.1.1.6.1.4.2.2.1  2010/09/21 15:45:50  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.16  2009/09/16 14:50:56  gur18550
 * Removed isStopAdvancedAllocation
 *
 * Revision 1.15  2009/09/01 15:14:18  gur12140
 * Solved Bug Related toUL and DL
 *
 * Revision 1.14  2009/07/30 14:02:48  gur19413
 * added new enum in Reports
 *
 * Revision 1.13  2009/07/17 14:35:56  gur19413
 * changed cqi, ri offset changed into 2 bytes.
 *
 * Revision 1.12  2009/07/13 04:40:42  gur19413
 * added new paramater PUCCH Conf node
 *
 * Revision 1.11  2009/06/23 05:57:56  gur18550
 * Added the riv field
 *
 * Revision 1.10  2009/06/12 07:04:23  gur11083
 * adding new parameter ulCodingRate in ULUEContext
 *
 * Revision 1.9  2009/06/10 14:46:51  gur19413
 * comments incorporated
 *
 * Revision 1.8  2009/06/09 09:08:40  gur18550
 * strictRBAllocation Added
 *
 * Revision 1.7  2009/06/09 09:00:43  gur18550
 * strictRBAllocation Added
 *
 * Revision 1.6  2009/06/03 16:43:40  gur18550
 * Updated HARQ
 *
 * Revision 1.5  2009/06/01 22:45:52  gur19413
 * added new parameters in PucchConfig struct
 *
 * Revision 1.4  2009/05/29 05:51:33  gur18569
 * removed compilation errors
 *
 * Revision 1.3  2009/05/28 12:29:49  gur20052
 * Added ULSchedulerAlgoInfo structure and  modulationScheme in UL Context
 *
 * Revision 1.2  2009/05/28 04:51:35  gur19413
 * Pending delete queue structure deleted
 *
 * Revision 1.1  2009/05/27 17:19:00  gur19413
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_UL_UE_CONTEXT_H
#define LTE_MAC_UL_UE_CONTEXT_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteCommonStatsManager.h"
#include "lteMacSpsCommonTypes.h"
#include "lteMacSRSMgr.h" /* SRS_CHG */ 
#include "lteMacPowerControl.h" /* CLPC CHANGE */
/* + E_CID_5.3 */
#include "lteMacECIDMgr.h"
#include "lteFapiMacPhyInterface.h"
/* - E_CID_5.3 */

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
 #ifdef TDD_CONFIG
 /* TDD Config 0 Changes Start */
#define MAX_UE_SCHEDULED_INFO_INSTANCES             6
 /* TDD Config 0 Changes End */
/* CRC_ULSCH_IND SPR 8722 Fix Start */
#define CRC_IND_RCVD_FLAG                           0x01
#define ULSCH_IND_RCVD_FLAG                         0x02
#define CRC_ULSCH_IND_RCVD_FLAG                     0x03
/* CRC_ULSCH_IND SPR 8722 Fix End */
#endif

/****************************************************************************
 * Exported Types
 ****************************************************************************/


typedef enum ULScheduleFlagT
{
    UL_QOS_DRX_ON_MEAS_GAP_ON_SYNC_LOSS_ON,
    UL_QOS_DRX_ON_MEAS_GAP_ON,
    UL_QOS_DRX_ON_SYNC_LOSS_ON,
    UL_QOS_MEAS_GAP_ON_SYNC_LOSS_ON,
    UL_QOS_DRX_ON,
    UL_QOS_MEAS_GAP_ON,
    UL_QOS_SYNC_LOSS_ON,
    UL_QOS_NO_TRANSMISSION,
    UL_QOS_ALREADY_TRANSMITED,
    UL_QOS_NEW_TRANSMISSION,
    UL_QOS_MAX_STATE,
}ULScheduleFlag;

/* SPR 10753 Changes */
typedef enum forceAckNackStatusT
{
  NON_ADAPTIVE_RETRANS = 0,
  ADAPTIVE_RETRANS_AFTER_HI_ACK,
  NON_ADAPTIVE_RETRANS_AFTER_HI_ACK,
}forceAckNackStatus;
/* SPR 10753 Changes */

 
typedef struct ULAmbrInfoT 
{
    /*Indicates that the Strategy LC node is inserted in the 
     * eligible list or not*/
    UInt8 isPresentInEligibleList;
    /*SPR_3244_Fix */
    UInt8 isLCNodePresent;
    /*SPR_3244_Fix */

    /*represents the lcId of a NGBR LC having minimum QCI*/
    UInt8 lcId;

    /* Indicates the lcg ID*/
    UInt8 lcgId;
    /* Indicates the AMBR */
/* L2_PERFORMANCE_OPT_CHANGES_START */
    UInt32     AMBR;
/* L2_PERFORMANCE_OPT_CHANGES_END */
    /* Indicates the rate at which token will be update every
     * TOKEN_UPDATE_PERIOD
     */
    UInt32  ambrTokenRate;
    /* Indicates the Tokens Available in bytes */
    SInt32 availAmbrToken;
    /* Indicates the token priority */
    UInt8 ambrTokenPriority;
    
    /*Indicates PDB priority*/
    UInt8  pdbPriority;
    /*Indicates QCI priority*/
    UInt8  qciPriority;
    /*Indicates Qload priority*/
    UInt8 qLoadPriority;
    
    /* DESIGN REVIEW CHANGES INCORPORATION  START */
    /* Indicates the token depth */
    TokenUpdateFlag  eTokenUpdateFlag;
    /* Indicates the token depth */
    UInt32  ambrTokenLimit;
    /* DESIGN REVIEW CHANGES INCORPORATION  END */
    /* Indicates the token depth */
    UInt32 ambrTokenDepth;
    
    UInt8  curTknWindowInd;
    
    /* Indicates the Total tokens scheduled in the scheduled
     * window */
    UInt32  totSchdTknInWindow;
    /* Stores the tokens scheduled in every
     * TOKEN_UPDATE_PERIOD in
     * last MAX_TKN_SCHD_WINDOW periods
     */
    UInt32 scheduledToken[MAX_TKN_SCHD_WINDOW];
    /* Indicates the current index in the scheduled
     * window array */
    
    /*Qload Table*/
    UInt32  lcQlTable[MAX_QLOAD_PRIORITIES];
    /*PDB Table*/
    UInt32  lcPdbTable[MAX_PDB_PRIORITIES];
    /*Token Table*/
    UInt32  lcTknTable[MAX_TOKEN_PRIORITIES];

    /*Indicates minimum QCI across all NGBR LCs*/
    UInt8  minQci;
    /*Indicates minimum PDB*/
    UInt16 minPdb;
    /*Indicates maximum PDB*/
    UInt16 maxPdb;

    /*Indicates the head of line packet delay*/
    UInt32 headOfLinePktDelay;
     /* pointer to the strategy node for the LC */
    struct ULQosStrategyLCNodeT* ulQosStrategyLCNode_p;
    /* Indicates the Credit Tokens in bytes */
    SInt32 creditToken;
    /* SPR 10984 FIX START */
    /* TDD Config 0 Chnages Statrt */
#ifdef TDD_CONFIG
    UInt32 prevAvailAmbrToken[MAX_UL_SUBFRAME_GRANT];
#endif
    /* TDD Config 0 Chnages End */
    /* SPR 10984 FIX END */
}ULAmbrInfo;

/* This structure is used in priortised LC list node, 
 * for storing the weight of the logical channel. Also 
 * to store the scheduled bytes of LC. 
 */

typedef struct ULQosStrategyLCNodeT
{
    /*Node anchor for Priortised LC tree*/
    LTE_TREE_NODE     lcTreeNodeAnchor;   
    /*Node anchor for Scheduled LC tree*/
    LTE_LIST_NODE     scheduledLcNodeAnchor;
    /*Node anchor for Eligible LC tree*/
    LTE_LIST_NODE     eligibleLcNodeAnchor;
    /*Indicates whether the LC is scheduled or not*/
    UInt8 isLCScheduled;
    /*Indicates channel type(GBR or NGBR)*/
    UInt8 channelType;    
    /* Indicates the LCG Id */
    UInt8 lcgId;
    /* Indicates the LC Id */
    UInt8 lchId;
    /* Indicates the UE Id */
    UInt16 ueId;
    /*Indicates that the Strategy LC node is inserted in the 
     * eligible list or not*/
    UInt8 isPresentInEligibleList;
    /*SPR_3244_Fix */
    UInt8 isLCNodePresent;
    /*SPR_3244_Fix */
    /* Indicate the Token Granted */
    /* TDD Config 0 Changes Start */
    /* requiredRB given by Staretgy used for TDD config 0 S subframe */
#ifdef TDD_CONFIG
    UInt16 scheduledBytes[MAX_UL_SUBFRAME_GRANT];
#else
    /* TDD Config 0 Changes End */
    UInt16 scheduledBytes;
#endif
    /* Indicate the qload Weight */
    UInt32 totLCWght;
    /*Pointer to GBR LC info*/
    struct ULLogicalChannelInfoT *lcInfo_p;
    /*Pointer to GBR LC info*/
    ULAmbrInfo *ulAmbrInfo_p;
}ULQosStrategyLCNode;


typedef struct ULLCGInfoT
{   
    /*This flag will be False if LCG belongs to only SRBs*/
    UInt8 isDrbPresent;
    /* Indicates the QLOAD priority*/
    UInt8 qLoadPriority;
    
    /*Indicates weight of SRB*/
    UInt16 srbWeight;
    
    /*indicates the remaining data available for this logical channel*/
    UInt32 qLoad;
    /*contain the information regarding when this logical channel was
     * scheduled*/
    /* SPR 15909 fix start */
    tickType_t lastScheduledTick;
    /* SPR 15909 fix end */

    UInt8 numLC;
    UInt8 lcId[MAX_NUMBER_OF_LOGICAL_CHANNEL];

}ULLCGInfo;



 typedef struct ULQosInfoT
{  
     /* Indicates the QCI of the Logical Channel */
     UInt8 QCI;
     /* Indicates the Type of the channel i.e. GBR or NON-GBR */
     UInt8 channelType;
     /*Indicates the priority of this logical channel for teh given QCI*/
     UInt8 priority;
     /*Indicates the priority of this logical channel*/
     UInt8  logicalChannelPriority;
     /* Indicates the Latency of the Logical channel */
     UInt16 pktDlyBdgt;
     /* Indicates the Gauranteed bit Rate */
     UInt32 GBR;
     /* Indicates the Maximum Bit Rate */
     UInt32 MBR;
     /* Indicates Packet Error Rate */
     UInt32 pktErrRate;
     /* Indicates the Token rate */
     UInt32 tokenRate;
     /* Indicates the Tokens Available */
     UInt32 availLcToken;
     /* DESIGN REVIEW CHANGES INCORPORATION  START */
     /* Indicates the whether tokens need to be incremented or decremented */
     TokenUpdateFlag  eTokenUpdateFlag;
     /* Indicates the minimum tokens to be maintained in the bucket */
     UInt32  tokenLimit;
     /* DESIGN REVIEW CHANGES INCORPORATION  END */
     /* Indicates the max token limit
      * TokenDepth */
     UInt32 tokenDepth;
     /* Last Scheduled Tick */
     /* SPR 15909 fix start */
     tickType_t lastScheduledTick;
     /* SPR 15909 fix end */
     /* Indicates the PDB priority*/
     UInt8  pdbPriority;
     /* Indicates the QCI priority*/
     UInt8  qciPriority;
     /* Indicates the Token priority*/
     UInt8 tokenPriority;
     /* Indicates the QLOAD priority*/
     UInt8 qLoadPriority;
     /* Indicates the tokens scheduled in the current
      * Scheduling Window */
     UInt32  totSchdTknInWindow;
     
     UInt8  curTknWindowInd;
     
     /* Array to store the total scheduled tokens per token
      * period across Scheduling window*/
     UInt32 scheduledToken[MAX_TKN_SCHD_WINDOW];
     /*Indicates whether strict scheduling will be applied or not*/
     UInt8 strictSchedulingFlag;
     /*Qload Table*/
     UInt32  lcQlTable[MAX_QLOAD_PRIORITIES];
     /*PDB Table*/
     UInt32  lcPdbTable[MAX_PDB_PRIORITIES];
     /*Token Table*/
     UInt32  lcTknTable[MAX_TOKEN_PRIORITIES];

     /* pointer to the strategy node for the LC */
     ULQosStrategyLCNode* ulQosStrategyLCNode_p;
     /*Indicated head of line packet delay*/
     UInt32 headOfLinePktDelay;
     /* Indicates the Credit Tokens */
     UInt32 creditToken;
    /* SPR 10984 FIX START */
     /* TDD Config 0 Changes Start */
#ifdef TDD_CONFIG
     /* Indicates the Tokens Available */
     UInt32 prevAvailLcToken[MAX_UL_SUBFRAME_GRANT];
#endif
     /* TDD Config 0 Changes End */
    /* SPR 10984 FIX END */
}ULQosInfo;

 
 /*structure for logical channel information of Uplink*/
typedef struct ULLogicalChannelInfoT{
     /* It represents the LchId is valid or Invalid*/
     UInt8               lchId;
     UInt8               lcGId;  /*if invalid, then Set XX value, where XX>3*/
     /* SPS_CHG */
     /* Flag to check whether the lc is SPS LC or not */
     UInt8               isSpsLc; 
     /* SPS_CHG */

     /* Added for QoS*/   
     ULQosInfo           ulQosInfo;
    /* SRP 3608 changes start */
    UInt8 uePSRReqFlag; 
    /* SPR 3608 changes end */
}ULLogicalChannelInfo;

#define INVALID_PRB_IDX           255
#define MAX_PUCCH_PERIODIC_REPORT 5

#define CQI_RI_PERIODIC_MODE           0
#define CQI_RI_APERIODIC_MODE          1
/* +- SPR 21777 fix */
/* Maximum CQI that supports QPSK = 6. 
 * Ref. Table 7.2.3-1: 4-bit CQI Table, 36.213.
 * Maximum SINR value corresponding to CQI = 6 from sinrToCQI_g = 146.
 */
/* SPR 2319 Start*/
#define MAC_QPSK_SINR_LIMIT 146 
/* SPR 2319 End*/

/* Maximum CQI that supports QPSK = 9. 
 * Ref. Table 7.2.3-1: 4-bit CQI Table, 36.213.
 * Maximum SINR value corresponding to CQI = 9 from sinrToCQI_g = 160.
 */
/* SPR 2319 Start*/
#define MAC_16QAM_SINR_LIMIT 160
/* SPR 2319 End*/

/* UL_CQI SINR has range 0-255, representing -64dB to 63.5dB, with 0.5dB 
 * step size. MAC_MAX_SINR represents the highest SINR that can be received.
 */
#define MAC_MAX_SINR 255
/* Rel9_upgrade_CR257 */
#define SHORT_SR_PERIODICITY        2
/* Rel9_upgrade_CR257 */

/* Target BLockErrorRate in UL*/
#define MAC_TARGET_BLER 10
/* SPR 3271 changes start */
#define DEFAULT_PATHLOSS 20
/* SPR 3271 changes end */

#define MAX_UL_HARQ_TIMER 2

/* SS_S2 */
#define  PERIODIC_MODE_ALL_TYPE_3_BITS                 1
/* 4x2 MIMO S */
#define  PERIODIC_MODE_ALL_TYPE_3_FOR_4_ANT_BITS       2
/* 4x2 MIMO E */
#define  PERIODIC_MODE_10_20_TYPE_4_BITS               4
#define  PERIODIC_MODE_11_21_TYPE_2_RI_1_BITS          6
/* + CQI_4.1 */
#define  PERIODIC_MODE_11_21_TYPE_2_RI_1_4_ANT_BITS    8 
#define  PERIODIC_MODE_11_21_TYPE_2_RI_GT_1_BITS       8 
#define  PERIODIC_MODE_11_21_TYPE_2_RI_GT_1_4_ANT_BITS 11 
#define  PERIODIC_MODE_20_21_TYPE_1_RI_1_BITS( cellParams_p) \
           ( 4 + cellParams_p->LBitLabel ) 
#define  PERIODIC_MODE_20_21_TYPE_1_RI_GT_1_BITS( cellParams_p)\
           ( 7 + cellParams_p->LBitLabel ) 
#define  APERIODIC_MODE_12_RI_1_BITS( cellParams_p) \
           ( 4 + (2 * cellParams_p->N) ) /* 4 + 2N */
#define  APERIODIC_MODE_12_RI_2_BITS( cellParams_p) \
                  ( 8 + cellParams_p->N )  /* 4 + 4 + N */
#define  APERIODIC_MODE_20_BITS( cellParams_p) \
                   ( 6 + cellParams_p->LBitLabelForAperiodic ) /* 4 + 2 + L */

#define  APERIODIC_MODE_22_RI_1_BITS( cellParams_p) \
       ( 10 + cellParams_p->LBitLabelForAperiodic ) /* 4 + 2 + L + 4 */
#define  APERIODIC_MODE_22_RI_2_BITS( cellParams_p)  \
            ( 14 + cellParams_p->LBitLabelForAperiodic ) /* 4 + 2 + 4 + 2 + L + 2 */
#define  APERIODIC_MODE_30_BITS( cellParams_p) \
                 ( 4 + (2 * cellParams_p->N) )  /* 4 + 2N */
#define  APERIODIC_MODE_31_RI_1_BITS( cellParams_p) \
                 ( 6 + (2 * cellParams_p->N) )  /* 4 + 2N + 2 */ 
#define  APERIODIC_MODE_31_RI_2_BITS( cellParams_p)           \
                   ( 9 + (4 * cellParams_p->N) ) /* 4 + 2N + 4 + 2N + 1 */
/* 4x2 MIMO S */
#define  APERIODIC_MODE_12_RI_1_FOR_4_ANT_BITS( cellParams_p) \
                             ( 4 + (4 * cellParams_p->N) ) /* 4 + 4N */
#define  APERIODIC_MODE_12_RI_GT_1_FOR_4_ANT_BITS( cellParams_p) \
                      ( 8 + (4 * cellParams_p->N) )  /* 4 + 4 + 4N */   
#define  APERIODIC_MODE_22_RI_1_FOR_4_ANT_BITS( cellParams_p)   \
                           ( 14 + cellParams_p->LBitLabelForAperiodic ) /* 4 + 2 + L + 8 */
#define  APERIODIC_MODE_22_RI_GT_1_FOR_4_ANT_BITS( cellParams_p) \
                           ( 20 + cellParams_p->LBitLabelForAperiodic ) /* 4 + 2 + 4 + 2 + L + 8 */
#define  APERIODIC_MODE_31_RI_1_FOR_4_ANT_BITS( cellParams_p) \
                           ( 8 + (2 * cellParams_p->N) )  /* 4 + 2N + 4 */
#define  APERIODIC_MODE_31_RI_GT_1_FOR_4_ANT_BITS( cellParams_p) \
                           ( 12 + (4 * cellParams_p->N) ) /* 4 + 2N + 4 + 2N + 4 */
#define  UE_CAT_5                                      5
/* SPR 11355 Fix Start */
#define  UE_CAT_8                                      8
/* SPR 11355 Fix End */
/*4X4 DL MIMO CHG START */
#define  RI_VALUE_FOR_4_LAYER                          4
/* 4X4 DL MIMO CHG END */
#define  RI_BIT_FOR_4_LAYER                            2
#define  RI_BIT_FOR_2_LAYER                            1
/* 4x2 MIMO E */

/* + CQI_4.1 */
#define  UE_ACK_NACK_TTI  4
#define  ONE_ADVANCE      1
/* - CQI_4.1 */
/* 4x2 MIMO S */
typedef enum MinRBsForAperiodicModesFor4Ant
{
    MIN_RB_APERIODIC_MODE_12_RI_1_20_MH_4_ANT     = 3, 
    MIN_RB_APERIODIC_MODE_12_RI_1_15_MH_4_ANT     = 3,
    MIN_RB_APERIODIC_MODE_12_RI_1_10_MH_4_ANT     = 3,
    MIN_RB_APERIODIC_MODE_12_RI_1_5_MH_4_ANT      = 2,
    MIN_RB_APERIODIC_MODE_12_RI_1_3_MH_4_ANT      = 2,
    MIN_RB_APERIODIC_MODE_12_RI_GT_1_20_MH_4_ANT  = 4,
    MIN_RB_APERIODIC_MODE_12_RI_GT_1_15_MH_4_ANT  = 3,
    MIN_RB_APERIODIC_MODE_12_RI_GT_1_10_MH_4_ANT  = 3,
    MIN_RB_APERIODIC_MODE_12_RI_GT_1_5_MH_4_ANT   = 3,
    MIN_RB_APERIODIC_MODE_12_RI_GT_1_3_MH_4_ANT   = 2,
    
    MIN_RB_APERIODIC_MODE_22_RI_1_20_MH_4_ANT     = 2,
    MIN_RB_APERIODIC_MODE_22_RI_1_15_MH_4_ANT     = 2, 
    MIN_RB_APERIODIC_MODE_22_RI_1_10_MH_4_ANT     = 2, 
    MIN_RB_APERIODIC_MODE_22_RI_1_5_MH_4_ANT      = 2, 
    MIN_RB_APERIODIC_MODE_22_RI_1_3_MH_4_ANT      = 2, 
    MIN_RB_APERIODIC_MODE_22_RI_GT_1_20_MH_4_ANT  = 3, 
    MIN_RB_APERIODIC_MODE_22_RI_GT_1_15_MH_4_ANT  = 3, 
    MIN_RB_APERIODIC_MODE_22_RI_GT_1_10_MH_4_ANT  = 3,
    MIN_RB_APERIODIC_MODE_22_RI_GT_1_5_MH_4_ANT   = 2,
    MIN_RB_APERIODIC_MODE_22_RI_GT_1_3_MH_4_ANT   = 2,

    MIN_RB_APERIODIC_MODE_31_RI_1_20_MH_4_ANT     = 3,
    MIN_RB_APERIODIC_MODE_31_RI_1_15_MH_4_ANT     = 2,
    MIN_RB_APERIODIC_MODE_31_RI_1_10_MH_4_ANT     = 2, 
    MIN_RB_APERIODIC_MODE_31_RI_1_5_MH_4_ANT      = 2, 
    MIN_RB_APERIODIC_MODE_31_RI_1_3_MH_4_ANT      = 2, 
    MIN_RB_APERIODIC_MODE_31_RI_GT_1_20_MH_4_ANT  = 4,
    MIN_RB_APERIODIC_MODE_31_RI_GT_1_15_MH_4_ANT  = 3,
    MIN_RB_APERIODIC_MODE_31_RI_GT_1_10_MH_4_ANT  = 3,
    MIN_RB_APERIODIC_MODE_31_RI_GT_1_5_MH_4_ANT   = 3,
    MIN_RB_APERIODIC_MODE_31_RI_GT_1_3_MH_4_ANT   = 2

}MinRBsForAperiodicModesFor4Ant;
/* 4x2 MIMO E */

/* This enum contains the minimum RBs required for the 
 * aperiodic reports */
typedef enum MinRBsForAperiodicModesAt20MHzT
{
    MIN_RB_APERIODIC_MODE_12_RI_1_20_MHz = 2,
    MIN_RB_APERIODIC_MODE_12_RI_2_20_MHz = 2,
    MIN_RB_APERIODIC_MODE_20_20_MHz = 2,
    MIN_RB_APERIODIC_MODE_22_RI_1_20_MHz = 2,
    MIN_RB_APERIODIC_MODE_22_RI_2_20_MHz = 2,
    MIN_RB_APERIODIC_MODE_30_20_MHz = 2,
    MIN_RB_APERIODIC_MODE_31_RI_1_20_MHz = 2,
    MIN_RB_APERIODIC_MODE_31_RI_2_20_MHz = 4
}MinRBsForAperiodicModesAt20MHz;

typedef enum MinRBsForAperiodicModesAt15MHzT
{
    MIN_RB_APERIODIC_MODE_12_RI_1_15_MHz = 2,
    MIN_RB_APERIODIC_MODE_12_RI_2_15_MHz = 2,
    MIN_RB_APERIODIC_MODE_20_15_MHz = 2,
    MIN_RB_APERIODIC_MODE_22_RI_1_15_MHz = 2,
    MIN_RB_APERIODIC_MODE_22_RI_2_15_MHz = 2,
    MIN_RB_APERIODIC_MODE_30_15_MHz = 2,
    MIN_RB_APERIODIC_MODE_31_RI_1_15_MHz = 2,
    MIN_RB_APERIODIC_MODE_31_RI_2_15_MHz = 3
}MinRBsForAperiodicModesAt15MHz;

typedef enum MinRBsForAperiodicModesAt10MHzT
{
    MIN_RB_APERIODIC_MODE_12_RI_1_10_MHz = 2,
    MIN_RB_APERIODIC_MODE_12_RI_2_10_MHz = 2,
    MIN_RB_APERIODIC_MODE_20_10_MHz = 2,
    MIN_RB_APERIODIC_MODE_22_RI_1_10_MHz = 2,
    MIN_RB_APERIODIC_MODE_22_RI_2_10_MHz = 2,
    MIN_RB_APERIODIC_MODE_30_10_MHz = 2,
    MIN_RB_APERIODIC_MODE_31_RI_1_10_MHz = 2,
    MIN_RB_APERIODIC_MODE_31_RI_2_10_MHz = 3
}MinRBsForAperiodicModesAt10MHz;

typedef enum MinRBsForAperiodicModesAt5MHzT
{
    MIN_RB_APERIODIC_MODE_12_RI_1_5_MHz = 2,
    MIN_RB_APERIODIC_MODE_12_RI_2_5_MHz = 1,
    MIN_RB_APERIODIC_MODE_20_5_MHz = 1,
    MIN_RB_APERIODIC_MODE_22_RI_1_5_MHz = 2,
    MIN_RB_APERIODIC_MODE_22_RI_2_5_MHz = 2,
    MIN_RB_APERIODIC_MODE_30_5_MHz = 2,
    MIN_RB_APERIODIC_MODE_31_RI_1_5_MHz = 2,
    MIN_RB_APERIODIC_MODE_31_RI_2_5_MHz = 3
}MinRBsForAperiodicModesAt5MHz;

typedef enum MinRBsForAperiodicModesAt3MHzT
{
    MIN_RB_APERIODIC_MODE_12_RI_1_3_MHz = 1,
    MIN_RB_APERIODIC_MODE_12_RI_2_3_MHz = 1,
    MIN_RB_APERIODIC_MODE_20_3_MHz = 1,
    MIN_RB_APERIODIC_MODE_22_RI_1_3_MHz = 1,
    MIN_RB_APERIODIC_MODE_22_RI_2_3_MHz = 2,
    MIN_RB_APERIODIC_MODE_30_3_MHz = 1,
    MIN_RB_APERIODIC_MODE_31_RI_1_3_MHz = 1,
    MIN_RB_APERIODIC_MODE_31_RI_2_3_MHz = 2
}MinRBsForAperiodicModesAt3MHz;

typedef struct AperiodicModeMinRBInfoT
{
    UInt8 minRBsForAperiodicMode12RI1;
    UInt8 minRBsForAperiodicMode12RI2;
    UInt8 minRBsForAperiodicMode20;
    UInt8 minRBsForAperiodicMode22RI1;
    UInt8 minRBsForAperiodicMode22RI2;
    UInt8 minRBsForAperiodicMode30;
    UInt8 minRBsForAperiodicMode31RI1;
    UInt8 minRBsForAperiodicMode31RI2;
}AperiodicModeMinRBInfo;

extern AperiodicModeMinRBInfo* aperiodicModesRBInfo_gp[MAX_NUM_CELL];
/* - CQI_4.1 */

/* This enum contains the valid valus of the report modes  for the 
 * periodic reports */
typedef enum PeriodicReportingModesT
{
    /*+COVERITY 5.3.0 - 32259*/
    PERIODIC_MODE_INVALID = 0,
    /*-COVERITY 5.3.0 - 32259*/
    PERIODIC_MODE_1_0 = 10,
    PERIODIC_MODE_1_1 = 11,
    PERIODIC_MODE_2_0 = 20,
    PERIODIC_MODE_2_1 = 21,
}PeriodicReportingModes;

/* This enum contains the configured value of the preiodic report, 
 * wideband or subband */
typedef enum PeriodicFeedbackTypeT
{
    PERIODIC_FEEDBACK_WIDEBAND = 0,
    PERIODIC_FEEDBACK_SUBBAND,
}PeriodicFeedbackType;
/* This structure contains the desrired parameters to be stored in 
 * PUCCH config of the UL UE context for periodic reports */
typedef struct PeriodicCqiPmiRiInfoT
{
    PeriodicReportingModes periodicRepMode; 
    PeriodicFeedbackType periodicFeedBackType;
    UInt8  k;
    /* + CQI_4.1 */
    UInt32 widebandPeriodicity;
    /* - CQI_4.1 */
    UInt32 widebandPeriodicity_2; /* EICIC +*/
}PeriodicCqiPmiRiInfo;

/* This structure contains the desrired parameters to be stored in 
 * PUCCH config of the UL UE context for aperiodic reports */
typedef struct AperiodicCqiPmiRiInfoT
{
 /* Coverity 32355 Fix Start */
 CQIMode cqiMode ;             
 /* Coverity 32355 Fix End */
}AperiodicCqiPmiRiInfo;

/* This enum contains the valid feedback reports that can come from UE 
 * This enum contains both  the periodic and aperiodic feedback types */
typedef enum ueExpectedReportTypeT
{
    EXPECTED_REPORT_INVALID = 0,
    APERIODIC_MODE_1_2,
    APERIODIC_MODE_2_0, 
    APERIODIC_MODE_2_2,
    APERIODIC_MODE_3_0,
    APERIODIC_MODE_3_1,
    PERIODIC_MODE_1_1_TYPE_2,
    PERIODIC_MODE_1_1_TYPE_3,
    PERIODIC_MODE_2_0_TYPE_1,
    PERIODIC_MODE_2_0_TYPE_3,
    PERIODIC_MODE_2_0_TYPE_4,
    PERIODIC_MODE_2_1_TYPE_1,
    PERIODIC_MODE_2_1_TYPE_2,
    PERIODIC_MODE_2_1_TYPE_3,
    PERIODIC_MODE_1_0_TYPE_3,
    PERIODIC_MODE_1_0_TYPE_4,
    PERIODIC_MODE_ALL_TYPE_3
}ueExpectedReportType;

/* This structure contains parameters that MAC needs to store till the time 
 * the report is fed back from UE */
typedef struct CqiPmiRiReportFormatMapT
{
    ueExpectedReportType ueExpectedReport;
     /* CA_phase2_csi_code start */
    /* Bitmask: Bit = 0 corresponds to cell idx = 0 7 so on */
    /* To be used in UL UE Pcell context only */
    UInt8 cellIdxOfExpectedReportType;
}CqiPmiRiReportFormatMap;
/* SS_S2 */

/* Rel9_upgrade_CR257 */
typedef struct ShortSrPucchPeriodicReportT
{
    LTE_LIST_NODE  nodeAnchor;
    UInt8  prbIndexSlot1;
    UInt8  prbIndexSlot2;
    UInt16 ueIndex;
    /* Review comment fix start PUCCH_ENH_1 */
    UInt32 subFrameOffset;
    /* Review comment fix end PUCCH_ENH_1 */
    struct ShortSrPucchPeriodicReportT *nextNode_p;
}ShortSrPucchPeriodicReport;

typedef struct ULDRXContextT
{
    UInt8 cqiMask;
    UInt8 isDRXShortCycleConfigured;
    UInt8 drxOnDurationTimer;
    UInt16 drxShortCycle;
    UInt16 drxLongCycle;
    UInt16 drxStartOffset;
    UInt16 drxOffsetModShortCycle;
#ifdef TDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t drxLongCycleCurrentOnDurationTTI;
    tickType_t drxShortCycleCurrentOnDurationTTI;
    /* SPR 15909 fix end */
#endif
}ULDRXContext;
/* Rel9_upgrade_CR396 */

/* Rel9_upgrade_CR257 */
typedef struct globalSrShortOpportunityT
{
    LTE_LIST_NODE nodeAnchor;
    UInt16 ueIndex;
    struct globalSrShortOpportunityT *nextNode_p;
}globalSrShortOpportunity;
/* Rel9_upgrade_CR257 */
typedef struct ReportBitMapT
{
    UInt16 exactSrWrapRound;
    UInt16 exactCQIWrapRound;
    UInt16 exactWbCQIWrapRound;
    UInt16 exactRIWrapRound;
    UInt16 exactCQIWrapRound2;
    UInt16 exactWbCQIWrapRound2;
    UInt16 exactRIWrapRound2;
    UInt8 ueReportAndOpportunity; //bit map to identify possible reports
}ReportBitMap;

typedef struct GlobalSrOpportunityAndPeriodicReportsT
{
    LTE_LIST_NODE nodeAnchor;
    LTE_SNODE snode;
    UInt16 ueIndex;
    /* keep refernce count of all the cells pointing to this node. 
       A node can be deleted only if refernce count is 0 */
    UInt8 refCntr; 
    ReportBitMap reportBitMapPCell;
    ReportBitMap reportBitMapSCell[MAX_NUM_SCELL];
}globalSrOpportunityAndPeriodicReports;

/* TDD SB_CQI */
typedef struct pucchNodeT
{
    /* reportType identiies the type of report contained in this PUCCH node 
    */
    globalSrOpportunityAndPeriodicReports * pucchNodeRef_p;
    UInt8 subFrameOffset;
}pucchNode;

typedef struct pucchNodeForReportT
{
    UInt8  count;
    pucchNode * pucchNodePtr;
}pucchNodeForReport;   
/* TDD SB_CQI */
/* PUCCH TDD ADAPTION CHANGES start */
/* PUCCH TDD ADAPTION CHANGES end */

typedef struct PucchConfigT
{
   
    UInt16    cqiPeriodicity;
    UInt16    cqiPeriodicity_2;
    UInt16    riPeriodicity;
    UInt16    riPeriodicity_2;
    UInt16     srPUCCHRBIndex;
    UInt16     cqiPUCCHRBIndex;
    UInt8     simultaneousAckNackAndCQI;
    UInt8     subbandCQI_k;
    /* + CQI_4.1 */
    UInt16    cqiPmiConfigIndex; 
    /* - CQI_4.1 */
    PeriodicCqiPmiRiInfo   cqiPmiRiInfo;
    AperiodicCqiPmiRiInfo  cqiPmiRiInfoAperiodic;

    /* Rel9_upgrade_CR257 */
    UInt8 shortSROffset;
    /* Rel9_upgrade_CR257 */
    /*SPR 16855 Fix Start*/
    /* pucchNodesQueue is accessed from Low priority thread only
     * as it contains only the reference nodes. But it has been made
     * 2-D for the two copies of maps it points to. */
    
    /* TDD SB_CQI */
    pucchNodeForReport pucchNodesForCqi[MAX_NUM_MAPS];
    pucchNodeForReport pucchNodesForWbCqi[MAX_NUM_MAPS];
    pucchNodeForReport pucchNodesForRI[MAX_NUM_MAPS];
    pucchNodeForReport pucchNodesForCqi2[MAX_NUM_MAPS];
    pucchNodeForReport pucchNodesForWbCqi2[MAX_NUM_MAPS];
    pucchNodeForReport pucchNodesForRI2[MAX_NUM_MAPS];
    /* TDD SB_CQI */

    pucchNodeForReport pucchNodesForSR[MAX_NUM_MAPS];
    globalSrShortOpportunity *pucchShortSRNodeHead_p[MAX_NUM_MAPS];
    UInt8 maxSrWrapAround[MAX_NUM_MAPS];
    UInt8 maxCQIWrapAround[MAX_NUM_MAPS];
    UInt8 maxRIWrapAround[MAX_NUM_MAPS];
    UInt8 maxWbCQIWrapAround[MAX_NUM_MAPS];
    UInt8 maxCQIWrapAround2[MAX_NUM_MAPS];
    UInt8 maxRIWrapAround2[MAX_NUM_MAPS];
    UInt8 maxWbCQIWrapAround2[MAX_NUM_MAPS];
    /*SPR 16855 Fix End*/
    /* Rel9_upgrade_CR410 */
    /* SPR 15909 fix start */
    tickType_t    srProhibitExpiryTTI;
    /* SPR 15909 fix end */
    UInt16      srProhibitTimerValv920;
    /* Rel9_upgrade_CR410 */
    /* SS_S2 */
    /* + CL-MIMO LJA*/
    /*This will store the RI Configuration index received from RRC*/
    /*SPR 1554 Start*/
    UInt16 riConfigIndex;
    /*SPR 1554 End*/
   /* Rel9_upgrade_CR257 : to be used both in fdd and tdd */
    UInt16    cqiOffset;
    UInt16    riOffset;
	 UInt16    cqiOffset_2;
    UInt16    riOffset_2;
    /*This will store the latest reported RI from the UE. 
     *Its value will be copied to dlRi only when nackQueueLoad is 0
     */
    UInt8     srPeriodicity;
    UInt8 ueReportedRI;
    /* - CL-MIMO LJA*/
     UInt8 ueReportedPeriodicRI;
    
   /* Rel9_upgrade_CR257 : to be used both in fdd and tdd */
    UInt8     srOffset;
    /* Review comment fix start PUCCH_ENH_2 */
    UInt8  prbIndexSlot1[MAX_NUM_MAPS][MAX_PUCCH_PERIODIC_REPORT];
    UInt8  prbIndexSlot2[MAX_NUM_MAPS][MAX_PUCCH_PERIODIC_REPORT];
    /* Review comment fix end PUCCH_ENH_2 */
    /* Rel9_upgrade_CR257 */
    /* Rel9_upgrade_CR257 */
    /* CA changes start */
    UInt8     simultaneousAckNackAndCQIV1020;
    UInt8     cqiformatIndicatorPeriodicV1020;
    UInt8     cqiformatIndicatorPeriodic_subband_kV1020;
    UInt8     trigger1r10; 
    UInt8     trigger2r10; 
    UInt16    riConfigIndexV1020;
    UInt16    cqiPmiConfigIndexV1020;
    UInt16    cqiPucchResourceIndexV1020;
    /* CA changes End */
    /*+EICIC */
    UInt16    cqiPmiConfigIndex_2;
    UInt16    riConfigIndex_2;
    UInt8    csiMeasurementSubset1[NUM_BITS_ABS_PATTERN];
    UInt8    csiMeasurementSubset2[NUM_BITS_ABS_PATTERN];
    
  /*-EICIC */

}PucchConfig;

/* Structure for PUSCH configuration parametrs */
typedef struct PuschConfigT
{
    UInt8 betaOffsetAckIndex;
    UInt8 betaOffsetRiIndex;
    UInt8 betaOffsetCQIIndex;
   
}PuschConfig;

/* SPR 1660 + SPR 1668 Fix starts */

    extern UInt8 currentIrvSequenceArray_g[];

/* SPR 1660 + SPR 1668 Fix ends */

typedef struct ULTokenInfoT
{
    UInt8 isLCScheduled;
    UInt32 scheduledToken;
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD)
    UInt32 lastScheduledToken;
#endif
}ULTokenInfo;

/* SPR 11331 Fix Start */
#ifdef TDD_CONFIG
#define MAX_UL_SF_CONTAINER 20
#endif
/* SPR 11331 Fix End */

/*SPR 16417 Changes Start*/
/** SPR 12364 fix Starts**/
typedef struct reqHarqInfoT{
#ifdef FDD_CONFIG
    UInt8        isTTIDataRcvd;
    UInt8        isTTIBundled;
    UInt8        isSpsEnabled;
#ifdef  HD_FDD_CONFIG
    UInt8        hdFddUlNackHandlingFlag;
#endif
#if defined(KPI_STATS) || defined(PERF_STATS)                                 
    UInt8        rbForKpiStats;                                     
#endif
#endif	
    UInt8        riLength;
    ULTokenInfo  ulTokenInfo[MAX_LOGICAL_CHANNEL];  
    UInt32       grantPerLcg[NUM_OF_LCG];
}reqHarqInfo;
/** SPR 12364 fix Ends **/
/*SPR 16417 Changes End*/

/*structure for the UL HARQ information*/
typedef struct ULHarqInfoT
{
    UInt8   riStart; /*indicates the starting index of the resorces*/
    UInt8   riLength;           /*indicates number of contigiously allocated RBs*/
    UInt8   riv;           /*indicates the riv value created for this HARQ Process */
    UInt8   ackNack;         /*indicates ACK or NACK (0 for NACK /1 for ACK) */
    UInt8   currentRetransCount;/*indicates the retransmissions for a UE*/
    /* SPR 1660 + SPR 1668 Fix starts */	
    UInt8   currentIrv; /* indicates the current redundancy version */
    /* SPR 1660 + SPR 1668 Fix ends */
    UInt8   newDataIndicator; /* Indicates whether New Data or Re-Transmission*/
    UInt8   nextReTransmissionType; /* Indicates the type of next re-trans*/
    UInt8   strictRBAllocation;
    UInt8   nPhichGrp;
    UInt8   nPhichSeq;
    UInt8   sfUsed;
    UInt8   nDmrs;
    /* UL_MU_MIMO_CHG Start */
    UInt8   buddyNdmrs;
#ifdef SSI_DEBUG_STATS
    UInt8   ulMuMimoStatus;/*1 -scheduled with ul mu-mimo,0 - not scheduled with ul mu-mimo i.e.final status wrt scheduling*/
#endif

    /* UL_MU_MIMO_CHG End */
    /*Semi Static changes Start */
    UInt8   ulTxMode;
    UInt8   mcsIndex;
    UInt8   freqEnabledFlag;
    UInt8   freqHoppingBits;
    /*Semi Static changes End */
    /*HD FDD Changes Start*/
#ifdef  HD_FDD_CONFIG
    /*This flag is used for handling the UL HARQ retransmission
     * cases in case of HD FDD UE*/
    UInt8   hdFddUlNackHandlingFlag;
#endif
    /*HD FDD Changes End*/
    UInt32  deltabsrLoad; /* UL_BSR_FIX*/
    /* msg3_retx_tdd_support_start */
    /*#ifdef ULHARQ_TIMER_PROC*/
    UInt8    ulHarqProcessId;
    /*#endif*/
    /* msg3_retx_tdd_support_start */
    UInt8   txIndicator;
    /* SPR 10753 Changes */
    UInt8   forceAckNextRetxType;
    /* SPR 10753 Changes */
#if defined (KPI_STATS) || defined (PERF_STATS)
    /* SPR 11331 Fix Start */
#ifdef FDD_CONFIG
    UInt8   rbForKpiStats;
#else
    UInt8   rbForKpiStats[MAX_UL_SF_CONTAINER];
#endif
    /* SPR 11331 Fix End */
    UInt8   qciInfo[MAX_QCI];
#endif 
    ULTokenInfo ulTokenInfo[MAX_LOGICAL_CHANNEL];
    /*Qload adjustment changes*/
    UInt32  grantPerLcg[NUM_OF_LCG];
    /*Qload adjustment changes*/
    UInt8   harqStatus; //TX_RETX
    UInt8   isCqiRequested;
    /* + SPR_17858_587_CHANGES */
    UInt8 harqTimerExpirySF;
    UInt16 harqTimerExpirySystemFrame;
    /* - SPR_17858_587_CHANGES */
/* SPR 19288 change start */
/* SPR 19288 change end */

    /*SPR 7086 Fix Start*/
    UInt8   nSrsInitial;
    /*SPR 7086 Fix End*/
    /* SPR 10027 FIX START */
    UInt8 isDrbPresent;
    /* SPR 10027 FIX END */
    /* CRC_ULSCH_IND SPR 8722 Fix Start */
#ifdef TDD_CONFIG
    UInt8 crcUlschIndFlag;
    /* TDD Config 0 Changes Start */
    /* requiredRB given by Staretgy used for TDD config 0 S subframe */
    UInt8 requiredRB;
    /* TDD Config 0 Changes End */
#endif
    /* CRC_ULSCH_IND SPR 8722 Fix End */
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD)
#ifdef FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t lastScheduledTTI;
#else
    tickType_t ulSchTTI;
    /* SPR 15909 fix start */
#endif
#if defined (KPI_STATS) || defined (PERF_STATS)
    UInt32 netlength[MAX_LOGICAL_CHANNEL];
    UInt32 tbSizeNet;
#endif
#endif

    /* TTIB_Code Start */
#ifdef FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t ttiBundleFirstTick;
    /* SPR 15909 fix end */
    UInt8 isTTIBundled;
    UInt8 isTTIDataRcvd;
    UInt8 isSpsEnabled;
#endif
    /* TTIB_Code End */
    /*TDD Config 0 Changes Start*/
#ifdef TDD_CONFIG
    UInt8   isPdcchAllocated ;
#endif
    /*TDD Config 0 Changes End*/
    /*PHICH_SPR Fix 12758 start */
    ForcedAckStatus forcedAckFlag;
    /*SPR Fix 12758 end */

    /** SPR 12364 fix Starts**/
    reqHarqInfo ulSchReqInfo;
    /** SPR 12364 fix Ends **/
    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    UInt16 startRbgIndexforSet1; /* RBG corresponding to s0 */
    UInt16 endRbgIndexforSet1; /* RBG corresponding to s1 */
    UInt16 startRbgIndexforSet2; /* RBG corresponding to s2 */
    UInt16 endRbgIndexforSet2; /* RBG corresponding to s3 */
    /* This flag will denote whether allocation is done through RAT0 or RAT1 */ 
    UInt8  RAT1Allocation;/* TRUE/FALSE */			 
#endif
    /* SPR 21958 PUSCH RAT1 Support End */
}ULHarqInfo;

typedef struct ULSchedulerAlgoInfoT
{
    /*for optimising on modscheme factor calc*/
    UInt32              uePriorityFactor;
    /*for optimising on modscheme factor calc*/
    UInt32              modulationSchemeFactor;
    /*Weighted Bsr Load of the UE*/
    UInt32        weightedBsrLoad;
    /*Weighted Priority of the UE based on scheduling algorithm*/
    UInt32        weightedPriority;
    /* This parameter indicates the calculation tick of weighted Priority*/
    /* SPR 15909 fix start */
    tickType_t        lastCompWTPriorityTick;
    /* SPR 15909 fix end */

}ULSchedulerAlgoInfo;

#ifdef TDD_CONFIG
typedef struct UeScheduledInfoT
{
    /*This is a enum having the identification of Node*/
    ULGrantRequestType ulGrantRequestType;
    UInt8 ueInsertedInPdcchDlUeInfo;
    UInt8 ulHarqProcessId;

    UInt8 ulCCEIndex;
    UInt8 dlCceIndex[MAX_SUB_FRAME];
    /* SPR 19679: TDD HARQ Multiplexing Changes Start */
    UInt8 dlCceIndexMuxPusch[MAX_SUB_FRAME];
    UInt8 harqSizeForMuxMOneInMIMO[MAX_SUB_FRAME];
    /* SPR 19679: TDD HARQ Multiplexing Changes End */
    UInt8 flag; /*Used for indicating ack/nack response*/
    UInt8 strictRBAllocation;
    UInt8 requiredRB;
    UInt8 uePosInPdcchULNewUEArray;
    UInt8 pcellHarqProcessId;
    /* SPR 15909 fix start */
    tickType_t ttiCounter;
    /* SPR 15909 fix end */
    UInt16 ulschUciInfoIndex;
    /* SS_S2 */
    UInt8  transMode; /* SS_S2 */ 
    /* SS_S2 */
    /* UL_MU_MIMO_CHG Start */
    /* This stores the Buddy Ue Index */
    UInt16  buddyUeIndex;
    /* UL_MU_MIMO_CHG End */
	/* + SPS_TDD_Changes */
	UInt8 isSPSCCE[MAX_SUB_FRAME];
	/* - SPS_TDD_Changes */
}UeScheduledInfo;
                 
/* msg3_retx_tdd_support_start */
typedef struct UeScheduledInfoForMsg3T
{
    UInt8 ulHarqProcessId;
    UInt8 ulCCEIndex;
    UInt8 flag; /*Used for indicating ack/nack response*/
    UInt8 strictRBAllocation;
    UInt8 requiredRB;
    UInt32 ttiCounter;
    UInt16 ulschUciInfoIndex;
}UeScheduledInfoForMsg3;
#endif                 
/* msg3_retx_tdd_support_end */
/* SRS_CHG */
/* This structure stores the dedicated SRS configuration received from RRC
during creating / reconfiguring UE*/ 
typedef struct SRSDedicatedSetupInfoT
{
    UInt8   srsBandwidth;
    UInt8   freqDomainPos;
    UInt8   srsHoppingBandwidth;
    UInt8   transmissionComb;
    UInt8   duration;
    UInt8   cyclicShift;
    UInt16  srsConfigIndex;
}SRSDedicatedSetupInfo;

/* This structure contains the configuration parameters for SRS 
stored in UE context*/
typedef struct SRSConfigInfoT
{
    /*Indicates SRS_CONFIG_SETUP / SRS_CONFIG_RELEASE*/
    UInt8                    srsConfigType;
#ifdef TDD_CONFIG
    /*Used to identify the SRS bandwidth collision with the total uplink
    bandwidth in upPts region of special subframes*/
    UInt8                    srsULBandwidthCollision; 
#endif
    /*Periodicity of SRS transmission based on srsConfigIndex*/
    UInt16                   srsPeriodicity;   
    /*Start subframe offset based on the srsConfigIndex*/
    UInt16                   srsSubFrameOffset;   
    /*Dedicated setup info received from RRC*/
    SRSDedicatedSetupInfo    srsDedicatedSetupInfo;
    /*Pointer to first SRS report node of the UE in the SRS subframe map*/
    /*SPR 16855 Fix Start*/
    SRSReportNode            *srsReportHeadNode_p[MAX_NUM_MAPS];
    /*SPR 16855 Fix End*/
}SRSConfigInfo;
/* SRS_CHG */
struct MeasGapOnOffTriggerNode;
typedef struct MeasGapContextULT
{
    MeasGapConfig              measGapConfig;
    /* + SPR_16082 */
    UInt8                      measGapStartOffsetUL[MAX_NUM_MAPS];
    UInt8                      measGapEndOffsetUL[MAX_NUM_MAPS];
    UInt8                      isMeasGapPeriodOn;
    UInt8                      measGapPeriodicity;
    /*SPR 16855 Fix Start*/
    MeasGapOnOffTriggerNode    *measGapStartNodeUL_p[MAX_NUM_MAPS];
    MeasGapOnOffTriggerNode    *measGapEndNodeUL_p[MAX_NUM_MAPS];
    /*SPR 16855 Fix End*/
    /* SPR 15909 fix start */
    tickType_t                     ulMeasGapNextStartTTI;
    tickType_t                     ulMeasGapNextEndTTI;
    /* SPR 15909 fix end */
    /* - SPR_16082 */
}MeasGapContextUL;

/* ULA_CHG */
typedef struct PRBToMCSInfoT
{
    /*Specifies the reported SINR on ULSCH or SRS indication*/ 
    UInt8 reportedSINR;
    /* Specifies the effective MCS on that PRB based on reportedSINR and 
     * current ueBLER.
     */
    UInt8 effectiveMCS;
}PRBToMCSInfo;
typedef struct MCSToPrbMapInfoT
{
    /* Specifies whether a PRB with a specific MCS has been reported on not. 
     * Index specifies the PRB for which this MCS has been reported.
     *
     * A value 'X' specifies that the next X contiguous indexes in the 
     * prbMap including this index have been reported with the 
     * current(or higher) MCS to which this entry belongs. 
     *
     * Value 0 means that for this PRB, current MCS has not been reported.
     *
     * For example, if PRBs 0 to 9 have all been reported to have 
     * SINR value 100, prbMap[0] will be 10, prbMap[1] will be 9,..... 
     * prbMap[8] will be 2, prbMap[9] will be 1. prbMap[10] will be 0.
     */
    UInt8 prbMap[MAC_MAX_RESOURCE_BLOCKS];
    /* Specifies the start index in prbMap where the first contiguous set of 
     * PRBs can be found
     */
    UInt8 startIdx;
    /* Specifies the end index in prbMap where the last index of the last 
     * contiguous set of PRBs can be found.
     */
    UInt8 endIdx;
    /* Specifies the maximum number of contiguous PRBs reported for this MCS*/
    UInt8 maxContiguous;
    /* Specifies the start index in prbMap where the first maxContiguous PRBs 
     * can be found.
     */
    UInt8 maxContiguousStartIdx;
}MCSToPrbMapInfo;

/* This structure shall contain the fields useful for calculating PUSCH BLER. */
typedef struct BlerInfoForPUSCHT
{
    /* Total BLER on PUSCH. */
    /* SPR 3530 START */
    SInt8 puschBler;
    /* SPR 3530 END */
    
    /* Bler changes start */
    /* Counter for the number of samples for which BLER has been calculated*/
    UInt16 blerCalculatedCounter;
    /* Bler changes end */
    
    /* Bler calculated for the number of samples.
     * this will be then overwritten to pucchBler;
     */
    SInt32 blerCalculated;

    /* Tick in which last BLER was calculated */
    UInt32 lastBlerCalculatedCounter;

    /* Bler changes start */
    /* Cumulative SINR Correction Factor based on BLER*/
    SInt32 sinrCorrectionFactor;
    /* Bler changes end */

}BlerInfoForPUSCH;

typedef struct CSIContextT
{
    /*Specifies the average reported SINR in UL.*/
    UInt8            averageSINR;
    /*Specifies the highest MCS received by MAC in UL*/
    UInt8            highestReportedMCS;
    /*Specifies the lowest MCS received by MAC in UL*/
    UInt8            lowestReportedMCS;
    /*Stores the SINR and effective MCS for a PRB group*/
    PRBToMCSInfo     prbToMCSMapArray[MAC_MAX_RESOURCE_BLOCKS];
    /*Stores the UE specific MCS to PRB Mapping information*/
    MCSToPrbMapInfo  *mcsToPrbMap_p;
    /*Stores the UE specific BLER information*/
    BlerInfoForPUSCH puschBlerInfo;  

}CSIContext;
/* ULA_CHG */
/*Power Control Change*/
typedef struct DedicatedULPowerControlInfoT
{
    /*Indicates UE specific component for calculating transmitted
     ** power for UE*/
    SInt8 p0UePusch;
    /*Used to calculate UE Transmit power for PUSCH */
    UInt8 deltaMcsEnabled;
    /*Indicates whether accumulated power control is enabled or not */
    UInt8 accumulationEnabled;
}DedicatedULPowerControlInfo;

typedef struct TpcPdcchConfigT
{
    /*Indicates whether PDCCH configuration is setup or released for the UE */
    UInt8 release;
    /*Indicates the index assigned to the UE for receiving TPC command through
     ** format 3/3A*/
    UInt8 tpcBitMapIndex;
    /*Indicates the assigned TPC RNTI for the UE (present if release== false)*/
    UInt16 tpcRnti;
    /*Configured for DCI Format 3 or 3A*/

    /* Coverity 54156 Fix Start */
    DCIFormat  isConfiguredForDci3Or3A;
    /* Coverity 54156 Fix End */

}TpcPdcchConfig;

/* CLPC_CHG */
/*This structure stores the information related to transmission power control 
 * command (TPC) send to UE for PUSCH power control. */
typedef struct TpcInfoForPuschT
{
    /* Indicates the tpc index as calculated using the tpc rnti asn start tpc
       pusch rnti range as configured furing cell config */
    UInt8 tpcRntiIndex;
    
    /* Indicates the offset configured for the UE  for accesing TPC using 
       bitmap sent in DCI format 3/3A */
    UInt8 tpcBitMapIndex;

    /* Indicate the configured TPC-RNTI for PUSCH that can be used to send the 
     * TPC using DCI Format 3/3A to the UE. It shall be set INVALID_TPC_RNTI, 
     * if no TPC RNTI is configured for the UE for PUSCH power control */
    UInt16 tpcRnti;

    /* Indicate the last Tick at which any TPC was applied by the UE for PUSCH.
     * If no TPC is sent to UE before, then this shall be zero. */
    /* SPR 15909 fix start */
    tickType_t lastAppliedTick;
    /* SPR 15909 fix end */

    /* Indicates the triggered wideband power control command that can be sent 
     * to UE. */
    SInt8 tpcTriggeredValue;

    /* Indicate whether the last TPC Value sent for this UE was fake Value
     * if yes then the TPC value would be either 1(if fakeTpcValue was 1)
     * or -1 (if fakeTpcValue was -1) this is relevant only for DCI3A */
    SInt8 fakeTpcValue;

    /* The pointer to UE node added in triggeredUeList in TPC-RNTI configured 
     * for PUSCH for the UE. This shall be NULL if the TPC is not triggered for
     * the UE using DCI format 3/3A. */
    TriggeredUEListNode * triggeredUeListNodeInTpcRntiContext_p;

    /* The pointer to UE node added in configuredUeList in context corresponding
     * to TPC.RNTI configured for PUSCH for the UE. This shall be NULL if the 
     * TPC is not configured for the UE using DCI format 3A. */
    ConfiguredUEListNode * configuredUeListNodeInTpcRntiContext_p;

    /* Counter to maintain when last SINR for PUSCH was calculated/updated 
     * to try trigger TPC */
    UInt32 lastTpcSinrCoolOffCounter;
    /* SPR 4609_4610 Start */
    UInt8                    minPowerReachedFlag;
    UInt8                    maxPowerReachedFlag;
    /* SPR 4609_4610 End */
}TpcInfoForPusch;

/* SPS_CHG */
/* Following structure for PUSCH res reservation successful */
typedef struct SpsULResourceAllocatorInfoT
{
    /* This stores the resource assignment information of allocated RBs */
    UInt16 allocatedRiv; 
    /* This stores the number of allocateRB */
    UInt8 allocatedRB;
    UInt8 startingVRBIndex;
    UInt8 allocatedMCS;
} *LP_SpsULResourceAllocatorInfo, SpsULResourceAllocatorInfo;


typedef struct spsHpIdCtxtT
{
    UInt8 harqId;
    UInt8 isDataReceived;
} spsHpIdCtxt;

/* Following structure contains the SPS information for UL for an UE */
typedef struct SpsUlInfoT
{
    /* SPS LC ID */
    UInt8  lcId;
    /* Contains the SPS State of the UE */
    /*coverity-530 CID-32342*/
    SpsStates  spsState;
    /*coverity-530 CID-32342*/
     /* 0 - RELEASE, 1 - SETUP */
    UInt8  requestType;
    /* Flag to check if UE MCS has reached below Min SPS Threshold*/
    UInt8 isUeUlWbMcsBelowThresh ;
    /* Number of sure Empty transmissions in SPS occasion */
    UInt8  numOfSureSpsEmptyTx;
    /* Number of unsure Empty transmissions in SPS occasion in case of max harq retx*/
    UInt8  numOfUnsureSpsEmptyTx;
    /* HARQ process Id used at the time of occasion or SDL processing */
    spsHpIdCtxt  hpId[2];
    /* SpsCrnti to be allocated when spsDlSetupInfo_p is recived 0 is invalid 
     * value */
    UInt16 SpsCrnti;
    /* Index used for Activation/Deactivation/Reactivation */
    UInt16 ttiIndex;
    /* Index used for Deactivation in SPS PUSCH */
    UInt16 actTtiIndex;
    /* Number of DCI attempts made for Activation/Deactivation/Reactivation */
    UInt16 numDciAttempts;
    /* Pointer to the node of UE either residing in SDL or occasion list */
    void   *sdlOccNode_p;
    /* Pointer to the second node of UE either residing in SDL or occasion list
     * for 20 ms periodicity */
    void   *sdlOccNode20_p;
	/* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
	/* Pointer to the third node of UE either residing in SDL or occasion list */
	void   *sdlOccNode40_p;
	/* Pointer to the fourth node of UE either residing in SDL or occasion list */
	void   *sdlOccNode60_p;
#endif
	/* - SPS_TDD_Changes */
    /* Head packet size in RLC SPS LC queue (size of PDCP PDU) */
    /* Latest Opportunity provided to SPS LC */
    UInt32 spsOpp;
    /* Average RLC PDU size */
    UInt32 avgRlcSduSize;
    /* Packet arrival TS */
    /* SPR 15909 fix start */
    tickType_t pktArrivalTs;
    /* SPR 15909 fix end */
    /* Present if request is Setup */
    spsUlSetupInfo *spsUlSetupInfo_p;
    /* Structure for RBs info UE */
    SpsULResourceAllocatorInfo ulResAllocInfo;
	/* + SPS_TDD_Changes */
#ifdef FDD_CONFIG
	/* This stores UE's DRX OFF index.To be used by RM */
	UInt8 ueDrxTTI[MAX_SPS_INTERVAL];
	/* This stores UE's MEAS GAP OFF index. To be used by RM */
	UInt8 ueMeasGapTTI[MAX_SPS_INTERVAL];
#elif TDD_CONFIG
	/* This stores UE's DRX OFF index.To be used by RM */
	UInt8 ueDrxTTI[MAX_UL_SPS_INTERVAL];
	/* This stores UE's MEAS GAP OFF index. To be used by RM */
	UInt8 ueMeasGapTTI[MAX_UL_SPS_INTERVAL];
#endif
	/* - SPS_TDD_Changes */
	/* Maximum number of transmissions for UL HARQ */
	UInt8       maxSpsUlHARQTx;
	/* SPS_CHG */
	UInt8 draRBsUpdationFlag;
    /* SPR 19038 Fix Start */
    /* Flag to indicate if ue's meas gap is colliding with sps occasion
     * in case meas gap has been configured after sps activation
     */
    UInt8 isUeMeasGapCollisionWithSpsOcc;
    /* SPR 19038 Fix End */
} *LP_SpsUlInfo, SpsUlInfo;
/* SPS_CHG */

/* CLPC_CHG END */
/* +DYNAMIC_ICIC */
typedef struct PhrTimerConfigT  
{
    UInt32 periodicPHRTimer;
    UInt32 prohibitPHRTimer;
}PhrTimerConfig;
typedef struct DeltaPowerMaskInfoT
{
    UInt8  lcgId;
    SInt32  maxQciDeltaPowerMask;
}DeltaPowerMaskInfo;

typedef struct QciDeltaPowerMaskInfoT
{
    UInt8  numOfQciInfo;
    DeltaPowerMaskInfo  deltaPowerMaskInfo[NUM_OF_LCG];
}QciDeltaPowerMaskInfo;

/* +DYNAMIC_ICIC */

/*UL_MU_MIMO_CHG_START*/

typedef struct ULMUMIMOInfoT
{
    /* This tells whether UL_MIMO_MIMO is enabled or disabled 
     * 0    - DISABLED
     * 1    - ENABLED
     */
    UInt8  ulMUMIMOStatus;
    
    /* Flag which tells whether resources are    
     * allocated to UE or not                    
     * 0 - Not Allocated                         
     * 1 - Allocated                             
     * */                                        
    UInt8 isPuschAllocated;                      

} ULMUMIMOInfo;

/*UL_MU_MIMO_CHG_END*/
/*  + E_CID_5.3  */
typedef struct EcidReportParamsT
{
    ReportTypesRequired typeofReportsRequired;
    UInt16 eCidTransactionId;
    UInt8 eCidMeasId;
    UInt8 reportStatus;
    UInt16 type1TAValue;
    /* + SPR_7499 */
    /* SPR 15909 fix start */
    tickType_t type1TaAOAReceivedTTI;
    UInt16 type2TAValue;
    UInt32 type2TAreceivedTTI;
    /* - SPR_7499 */
    UInt16 angleOfArrival[MAX_NUM_ANTENNA];
    LTE_LIST_NODE *dummyDCI0Entry_p;
    /* SPR 15909 fix start */
    tickType_t rrmReqRecvTick;
    /* SPR 15909 fix end */
    /* SPR_8672_FIX */
    UInt8 dummyDCI0PresentFlag;
    /* SPR_8672_FIX */
}EcidReportParams;
/*  - E_CID_5.3  */
/*CA changes start*/
typedef PucchConfig ScellPucchConfig;
/*This structure contains Scell specific information
*required for uplink processing*/
typedef struct ULUESCellContextT
{
    InternalCellIndex internalCellIndex;
    /* + CQI_5.0 */
    /* SPR 15909 fix start */
    tickType_t aperiodicCqiRequestTTI;
    tickType_t lastTriggeredAperiodicTTI;
    /* SPR 15909 fix end */
    UInt32 nextSubbandOccurrence;
    UInt32 nextWidebandOccurrence;
    /* EICIC +*/
    UInt32 nextSubbandOccurrence_2;
    UInt32 nextWidebandOccurrence_2;
    /* EICIC -*/
    UInt8 periodicCQIDroppedCounter;
    UInt8 aperiodicCQIToBeRequested;
    /* - CQI_5.0 */
    UInt8 isHDFddFlag;
    ScellPucchConfig scellpucchConfigInfo;

    UInt8  userLocationType;
    UInt8      isScellActivateStatus;
    UInt8 modScheme;
    /* CA_phase2_csi_code Start */
    UInt8 minRBs;
    /* + CQI_4.1 */    
    /* This will store for which bandwidth part number subband is reported */  
    UInt8        reportedBandwidthPartNum;
    UInt8        subframeToBandwidthPartNo[MAX_SUB_FRAME];
    /* EICIC +*/
    UInt8        reportedBandwidthPartNum_2;
    UInt8        subframeToBandwidthPartNo_2[MAX_SUB_FRAME];
    /* EICIC -*/
    /* + CQI_4.1 */
    UInt32 minDataSize;
/** SPR 14032 Fix : Start **/
#ifdef FDD_CONFIG
    CqiPmiRiReportFormatMap  cqiPmiRiReportFormatMapSCell[NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME];
#elif TDD_CONFIG
    CqiPmiRiReportFormatMap  cqiPmiRiReportFormatMapSCell[2][NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME];
#endif    
/** SPR 14032 Fix : End **/
    /* CA_phase2_csi_code End */
    /*pdcch changes start*/
    UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p;
    /*pdcch changes end*/
}ULUESCellContext;

typedef struct PucchFormatR10T 
{
	UInt8 requestType;
	UInt16 n1Pucch[MAX_PUCCH_AN_CS_LIST][MAX_PUCCH_AN_CS];
}PucchFormatR10;
/*CA changes end*/


/*structure of the Uplink UE Context*/
typedef struct ULUEContextT
{
    UInt16      crnti;
    UInt16      ueIndex;
    /* ULA_CHG */
    /*indicates the category of this UE*/
    UInt8       ueCategory;
    /* Maximum number of transmissions for UL HARQ */
    UInt8       maxUlHARQTx;
    /* ULA_CHG */
    /*indicates the priority of this UE*/
    UInt8       uePriority;
    /* ICIC changes start */
   
    /* Stores user location type 
     * Value 0 -> Cell Center
     * Value 1 -> Cell Edge */
    UInt8 userLocationType;
    /* isAlreadyInCEPriorityList is used in DL RR to prvent a duplicate entry in queue */
    UInt8 isAlreadyInCEPriorityList;
    /* ICIC changes end */

    /*HD FDD Changes Start*/
#ifdef HD_FDD_CONFIG
    /*This flag will indicate whether UE is HD FDD or not
     * 1 - UE is HD FDD
     * 0 - UE is FD HDD*/
    UInt8 isHDFddFlag;
#endif
    /*HD FDD Changes End*/


    UInt8       modulationScheme;
    /*indicates the modulation scheme to be used for particular UE*/
    UInt8       mcsIndex;

    /*Sum of the BSR of all the 4 logical channel groups */
    UInt32      bsrNetTotal;

    /* CLPC_CHG */
    /*Configured UE transmitted power according to its power class*/
    
    SInt8 pcMax;
    /* CLPC_CHG END */

    ULSchedulerAlgoInfo  ulSchedulerAlgoInfo;
    /* Indicates number of LCs configured for LCG */
    UInt8   lcCount[NUM_OF_LCG];
    /*CQI flag indicating CQI required or not (0 or 1) */
    UInt8   cqiInd;

    /* + CQI_4.1 */    
    /* This will store for which bandwidth part number subband is reported */  
    UInt8        reportedBandwidthPartNum;
    UInt8        subframeToBandwidthPartNo[MAX_SUB_FRAME];
    UInt8        reportedBandwidthPartNum_2;/* EICIC +-**/
    UInt8        subframeToBandwidthPartNo_2[MAX_SUB_FRAME];/* EICIC +-**/

    /* + CQI_4.1 */
    UInt32 nextSubbandOccurrence;
   UInt32 nextSubbandOccurrence_2 ;/* EICIC +-**/
    UInt32 nextWidebandOccurrence;
    UInt32 nextWidebandOccurrence_2; /* EICIC +-**/

    /* - CQI_4.1 */
    /* - CQI_4.1 */

#ifdef DL_UL_SPLIT    
    /* CAUTION: isPdcchAllocated Field is candidate for access contention. Please consult before
     * updating*/
#endif 
    /*This Flag is used for to prevent multiple copies of UE UL Adavnced
     * Queue */
    UInt8  isEntryPushedInAdvanceQueue;

/*TDD Config 0 Changes Start*/
    /*It represent the pddch allocated or not */
    UInt8   isPdcchAllocated ;
/*TDD Config 0 Changes End*/
    /* This duplication has been introduced in rel 1.2 */
    /* TDD Config 0 Chnages Start */
#ifdef FDD_CONFIG
    /* SPR 15909 fix start */
    tickType_t   isAlreadyConsideredFlag;
    /* SPR 15909 fix end */
#elif TDD_CONFIG
    /* For TDD config 0 S subframe isAlreadyConsideredFlag need to be 
     * UL Subframe wise as UL Staretgy will run two times in this case
     * and due to this isAlreadyConsideredFlag will be reset incorrectly */
    UInt32   isAlreadyConsideredFlag[MAX_SUB_FRAME];
#endif
    /* TDD Config 0 Chnages End */
    /* Pointer to UE Specific Search Space */
    UESpecificSearchSpaceInfo *ueSearchSpaceInfo_p;
    UESpecificSearchSpaceInfo *spsUeSearchSpaceInfo_p;
    /*A pointer to nodeAnchor of the UE in nonZeroBsrList_g*/
    LTE_LIST_NODE *nonZeroBsrList_p;
    /*TTI in which a UE was last scheduled*/
    /* SPR 15909 fix start */
    tickType_t   lastScheduledTick;
    /*START:6172*/
    tickType_t   advlastScheduledTick;
    /* SPR 15909 fix end */
    /*END:6172*/
    ULHarqInfo   ulresInfo[MAX_HARQ_PROCESS_NB];
    /*BSR information of the 4 logical channel groups*/
    UInt32      bsrNet[NUM_OF_LCG];
    UInt8       numLogicalChannels;
#if defined(OVERSHOOT_CORRECTION) || defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD)
    UInt8 maxConfiguredLcId; 
    UInt8 isDemuxIndexPresent;
#endif
    
    /* CLPC_CHG */
    /* Wideband CQI values for Codewords*/
    UInt8 isDLWideBandCQIReportAvailable;
    UInt8 dlWidebandCQICodeWordOne;
    UInt8 dlWidebandCQICodeWordTwo;
    /* +EICIC+*/
    UInt8 isDLWideBandCQIReportAvailable_2;
    UInt8 dlWidebandCQICodeWordOne_2;
    UInt8 dlWidebandCQICodeWordTwo_2;
    /* -EICIC-*/
    
     /* The current power control offset from initial power 
     * that is indicated to the UE for PUSCH */
    SInt8 currPowerCorrectionForPusch;

    /* The previous power control offset from initial power 
     * that was indicated to the UE for PUSCH */
    SInt8 prevPowerCorrectionForPusch;

    /* the current Pathloss value as calculated using PHR received from UE. */
    SInt16 pathLoss;
    /* CLPC_CHG END */

    /*indicates the C-RNTI assigned to this UE*/
    ULLogicalChannelInfo lcInfo[MAX_NUMBER_OF_LOGICAL_CHANNEL];
    /* SPR 2063 start */
    /* SPR 2063 end */
    PucchConfig pucchConfigInfo;
/* new structure added for PUSCH config parameters */
    PuschConfig puschConfigInfo;
    /* SS_S2 */
    /*SPR 5620 START*/
#ifdef FDD_CONFIG
    CqiPmiRiReportFormatMap   reportFormatMap[NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME];
#else
    CqiPmiRiReportFormatMap   reportFormatMap[2][NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME];
#endif
    /*SPR 5620 END*/
    /* SS_S2 */
    #ifdef FDD_CONFIG
#ifdef ULHARQ_TIMER_PROC
    UInt8    ulHarqTimerNodeArrrayIndex[MAX_UL_HARQ_TIMER][MAX_SUB_FRAME];
#endif
/* SPR_18211 start */ 
#else
/* SPR_18211 stop */
    UInt8 cumulativeCountOfPdsch;
    /** SPR 14204 Fix : Start **/
    UeScheduledInfo * ulUeScheduledInfoPointerArray[MAX_NUM_CELL][MAX_SUB_FRAME];
    UeScheduledInfo * dlUeScheduledInfoPointerArray[MAX_NUM_CELL][MAX_SUB_FRAME];
    UeScheduledInfo ueScheduledInfo[MAX_NUM_CELL][MAX_UE_SCHEDULED_INFO_INSTANCES];
    /** SPR 14204 Fix : End **/
    /* TDD Config 6 Changes Start */
    UInt8 ulHarqProcessIdToPuschSubFrameMAp [MAX_UL_HARQ_TIMER][MAX_SUB_FRAME];
    /* TDD Config 6 Changes End */
    UInt8 tddAckNackFeedbackMode;
    UInt8   ulHarqTimerNodeArrrayIndex[2][MAX_SUB_FRAME];
#endif
     /* CLPC_CHG */
    /*Contains information elements of dedicated uplink power control.*/
    DedicatedULPowerControlInfo dedicatedULPowerControlInfo;

    /* This structure stores the TPC information for PUSCH */
    TpcInfoForPusch tpcInfoForPusch;
    /* CLPC_CHG END */
   
    /* SRS_CHG */
    /*This is SRS config info received from RRC and SRS subframe map
    related parameters*/
    SRSConfigInfo   srsConfigInfo;
    /* SRS_CHG */

    /*indicates in which tick last UL data was received from UE*/
    /* SPR 15909 fix start */
    tickType_t lastActivityTick;
    /* SPR 15909 fix end */

    /* ULA_CHG */
    CSIContext csiContext;
    /* ULA_CHG */
    /* DRX_CHG */
    LTE_LIST_NODE   *nonZeroBsrListTempDeleted_p;
    UInt8           drxConfigType;
    DRXState        drxCurrentState;
    /* Rel9_upgrade_CR396 */
    ULDRXContext    ulDrxContext;
    /* Rel9_upgrade_CR396 */
    /* DRX_CHG */
    /* MEAS_GAP_CHG */
    UInt8           measGapConfigType;
    MeasGapContextUL  measGapContext;
    /* MEAS_GAP_CHG */
    /*UL Force Grant Changes Start */
    UInt16 numOfForceGrant;
    /* +- SPR 21777 fix */
    UInt8 firstBSRReceived;
    /*UL Force Grant Changes End */
    ULAmbrInfo ulAmbrInfo ;
    UInt16  mcsPriority;
    UInt16 maxTBSize;
    ULLCGInfo ulLCGInfo[NUM_OF_LCG];

    /*Indicates the state of the UE*/
     /* coverity-530 CID-32338*/ 
    ULScheduleFlag scheduleFlag;
     /* coverity-530 CID-32338*/ 

    /* Number of LC to be Schedule */
    UInt8   scheduledLcCount;
    
    /*Eligible LC list*/
    LTE_LIST ulEligibleLcList;
    /*Eligible LC list*/
    LTE_LIST ulScheduledLcList;
    /*Pointer to Strategic Transmission Node*/
    struct ULStrategyTxNodeT *ulStrategyTxNode_p;
    UInt32          maxRestrictedTB;
#ifdef KPI_STATS
    UInt8 qciInfoFlag[MAX_QCI];
#endif

    /* Time Averaging changes start */
     /* This array stores the value of deltaTF and Log10M factors per subframe 
     * so that while fetching the grant information for pathloss calculation 
     * when PHR is received at receiver module, there is no dependency on data
     * saved in Harq process. */
    SInt8 powerControlDeltaTFAndLogFactor[MAX_SUB_FRAME];
     
    /* This array stores the TPC triggered values sent on subframes */ 
    SInt8 powerControlPuschTPCValue;   
    /* Time Averaging changes end */
    /* SPR 3271 changes start */
    /*indicates the initial MCS configured for the UE at the time of 
     * context creation.
     * */
    UInt8       initialMcs;
    /* SPR 3271 changes end */
    /* SRP 3608 changes start */
    UInt8 prioritySchedulingReqUL;
    UInt8 uePsrCount;
    UInt8 recvdUePsrCount;
    /* SPR 3608 changes end */
    /* SR starvation fix start*/
    ULGrantRequestQueueNode *srNode_p;
    /* SR starvation fix end */

    /* + CQI_5.0 */
    /* SPR 15909 fix start */
    tickType_t aperiodicCqiRequestTTI;
    tickType_t lastTriggeredAperiodicTTI;
    /* SPR 15909 fix end */
    UInt8 periodicCQIDroppedCounter;
    UInt8 aperiodicCQIToBeRequested;
    /* - CQI_5.0 */
    /*This structure contains info when contention free RACH is triggered due to
     * UL Sync Loss or Handover*/
    UlSyncLossOrHOInfoInUL   ulSyncLossOrHOInfoInUL;
   #ifdef PERF_STATS
    UInt32 maxtbSizebyUeCategory;
   #endif 
    /*UL_MU_MIMO_CHG_START*/
    /* This stores the UL MU MIMO related Info */
    ULMUMIMOInfo ulMUMIMOInfo;
    /*UL_MU_MIMO_CHG_END*/
    SInt8 minUePower;

    /* SPS_CHG */
    UInt8       schType;
    SpsUlInfo   spsUlInfo; /* Contains SPS related information */
    /* +DYNAMIC_ICIC */
    UInt32 phrGrant;
    SInt32 maxQciDeltaPowerMask;
    /* keeps the maximum delta Power Mask of all the LCs */
    QciDeltaPowerMaskInfo qciDeltaPowerMaskInfo;
    PhrTimerConfig phrTimerCfg;
    /* SPR 15909 fix start */
    tickType_t phrProhibitTimerExpiryTick;
    /* SPR 15909 fix end */
    /* -DYNAMIC_ICIC */
    /* SPR 5713 changes start */
    UInt8 ueComplianceRelease;
    UInt8 ulGrantMinDataSize;
    /* SPR 5713 changes end */
    /*  + E_CID_5.3  */
    EcidReportParams eCidReportParams;
    /*  - E_CID_5.3  */
/* TTIB_Code Start */
    /*SPR 21501 fix start+-*/
    UInt8 ttiBundlingEnabled;
#ifdef FDD_CONFIG
    UInt8 ttiHarqMap[TTI_BUNDLE_HARQ_MAP];
    /** SPR 12364 fix Start **/
    UInt8 ttiPUSCHHarq[TTI_BUNDLE_HARQ_MAP];
    /** SPR 12364 fix Ends **/
#endif
    /* TTIB_Code End */
    /*CA changes start*/
    /* Bitmask: Bit = 0 corresponds to cell idx = 0 7 so on */
    InternalCellIndex internalCellIndex;
    UInt8 ueCategoryV1020;
    UInt8     trigger1r10; 
    UInt8     trigger2r10; 
    /*Count of Scells configured for this UE*/
    UInt8 scellCount;
    UInt8 scellDeactivationTimer;
    UInt8 activeScellBitmap; /*This Bitmap will store the active SCELL for a UE*/
    PucchFormatR10 pucchFormatR10;
    /*Secondary Cell contexts for each of the SCELL configured for the UE*/
    ULUESCellContext *ulUeScellContext_p[MAX_NUM_SCELL + 1];
    /*CA changes end*/
    /*+ Dyn UEs To Be SchPerTTI +*/
    UInt8 isUEConsideredForTotalActiveUeUL;
    /*- Dyn UEs To Be SchPerTTI -*/
    /*+ SPR 8564
     This is required to ensure that the CFR timer is 
     not stopped before reception of msg3. This happens
     when in case of E-CID, PDCCH Order is triggered but
     its possible that the grant was sent some TTI before
     this PO. This would result in ULSCH before RACH
     and it will be considered as msg3 by L2*/
    UInt8 waitForMsg3BeforeResetting;
    /*- SPR 8564*/
    /* TDD Config 0 changes start */
#ifdef TDD_CONFIG
    /* Denotes Number of U subframe, UL Strategy schedules in a TTI
     * In Each run scheduler schedules different U (maximum value 2) of a UE*/
    UInt8 scheduledCount;
#endif
    /* TDD Config 0 changes end */
    /* EICIC +*/
    UserType		userType;
    /* EICIC -*/
    /* SPR 19031 fix start */
#ifdef FDD_CONFIG
    tickType_t aperiodicCQIWithoutDataTTI;
#endif
    /* spr 21260 + */
    UInt8 isCqiDropped;
    tickType_t cqiDropTick;
    /* spr 21260 - */
    /* + SPR 21811 Fix */
    UInt8 firstSRSIndRecvd;
    /* - SPR 21811 Fix */
    /* SPR 19031 fix end */
    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    /* This flag denotes whether RAT1 is enabled or disabled for a particular UE at MAC */
    UInt8 isUplinkRAT1Supported;
#endif
    /* SPR 21958 PUSCH RAT1 Support End */
    /* SPR 23402 Changes Start */
    /* MOCN Changes Start */
    UInt8 operatorId;
    /* MOCN Changes End */
    /* SPR 23402 Changes End */
}ULUEContext;


typedef struct ULUEContextInfoT
{
    /* This flag is set by RRC interface when UE deletion request comes
       if PendingDelete is True or 1 then UE Context either
       deleted or is to be deleted.*/
    UInt8 pendingDeleteFlag;
    /* CA changes start */
    UInt8 scellRelFlag;
    /* CA changes end */
    ULUEContext *ulUEContext_p;
    /* SPR 13492 Changes Start */
#ifdef KPI_STATS
    UInt8 inSessionTimePerQCIFlag[MAX_QCI];
#endif
    /* SPR 13492 Changes End */
}ULUEContextInfo;

typedef struct allocatedResInfoT{
    UInt8   allocatedLength;
    UInt8   allocatedIndex;
    UInt8   allocatedRiv;
}allocatedResInfo;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern ULUEContextInfo ulUECtxInfoArr_g[MAX_UE_SUPPORTED];
extern LTE_SQUEUE    pendingDeleteULUEQueue_g;
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#endif  /* LTE_MAC_UL_UE_CONTEXT_H */

