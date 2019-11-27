/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacTypes.h,v 1.1.1.1.6.2.4.2.2.2.14.1 2010/11/08 06:06:11 gur10121 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains different structures, MACROS that 
 *                     are shared between various modules of MAC. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacTypes.h,v $
 * Revision 1.1.1.1.6.2.4.2.2.2.14.1  2010/11/08 06:06:11  gur10121
 * Semi static changes merging
 *
 * Revision 1.1.1.1.6.2.4.2.2.2.8.1  2010/10/29 15:09:37  gur10121
 * PHY_UL_CONTROL_DELAY value changed to 2.
 *
 * Revision 1.1.1.1.6.2.4.2.2.2  2010/09/27 16:49:13  gur10121
 * SPR 662 Fix
 *
 * Revision 1.1.1.1.6.2.4.2.2.1  2010/09/21 15:45:49  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.2  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.116  2009/11/14 14:32:29  gur11912
 * added INVALID_TX_MODE
 *
 * Revision 1.115  2009/10/26 09:05:28  gur18550
 * Changes due to 500 UEs
 *
 * Revision 1.114  2009/10/05 08:02:25  gur11912
 * removed warnings
 *
 * Revision 1.113  2009/08/25 14:05:14  gur12140
 * Optimzied DCi CCE Array
 *
 * Revision 1.112  2009/08/04 10:54:18  gur12140
 * Added #define MAX_NUM_PUSHED_IN_DL_ADVANCE_QUEUE
 *
 * Revision 1.111  2009/08/03 08:22:39  gur20052
 * Fix for UT_LOGS
 *
 * Revision 1.110  2009/08/01 09:23:34  gur20052
 * Added a new hash def for MIN_RB_FOR_BOTH_TA_DRX_ONLY
 *
 * Revision 1.109  2009/07/29 10:41:58  gur18550
 * Added enum Update
 *
 * Revision 1.108  2009/07/28 07:21:46  gur11912
 * Added File Description
 *
 * Revision 1.107  2009/07/23 13:59:36  gur18569
 * added PRINT_UE_QL
 *
 * Revision 1.106  2009/07/22 14:37:50  gur18550
 * CCE_NOT_ALLOCATED set to 250
 *
 * Revision 1.105  2009/07/21 05:31:18  gur20052
 * Removed the redundant enums from MsgType
 *
 * Revision 1.104  2009/07/20 07:57:22  gur18550
 * Added CCE_NOT_ALLOCATED
 *
 * Revision 1.103  2009/07/17 15:31:33  gur18550
 * Added Non_adaptive_re_trans
 *
 * Revision 1.102  2009/07/15 11:54:44  gur19413
 * changed MAX_NUM_OF_LOGICAL_CHANNEL to 11
 *
 * Revision 1.101  2009/07/14 08:54:54  gur20052
 * updated for ADAPTIVE_RE_TX enum
 *
 * Revision 1.100  2009/07/14 06:07:58  gur18569
 * Replacing define MAX_ENCODER_CONTAINERS MAX_CCE_CONTAINER with MAX_ENCODER_CONTAINERS 4 for the second time
 *
 * Revision 1.99  2009/07/13 15:44:58  gur20052
 * updated for prefix string DL_Q_LOAD_UPDATE
 *
 * Revision 1.98  2009/07/13 12:23:43  gur20052
 * added DL and UL Strategy prefix string macro
 *
 * Revision 1.97  2009/07/13 07:53:04  gur18569
 * defined the definition of LTE_MAC_UT_LOG inside UT_TESTING
 *
 * Revision 1.96  2009/07/13 07:30:26  gur18569
 * added encoder prefix string macro
 *
 * Revision 1.95  2009/07/13 07:17:09  gur18569
 * replaced lteLog with lteShmLog
 *
 * Revision 1.94  2009/07/11 14:09:01  gur18550
 * Added UNSET_FLAG
 *
 * Revision 1.93  2009/07/10 11:03:19  gur19413
 * replaced lteShmLog.h with lteShmLogger.h
 *
 * Revision 1.92  2009/07/10 10:59:31  gur18569
 * included lteShmLog.h
 *
 * Revision 1.91  2009/07/10 05:48:07  gur19413
 * added New Macro SHIFT_EIGHT_BITS
 *
 * Revision 1.90  2009/07/07 15:16:00  gur18569
 * set MAX_ENCODER_CONTAINERS to MAX_CCE_CONTAINER
 *
 * Revision 1.89  2009/07/07 08:08:22  gur20052
 * changes related to the HARQ implementation
 *
 * Revision 1.88  2009/07/06 06:31:32  gur18550
 * comments inc
 *
 * Revision 1.87  2009/07/03 14:02:47  gur20052
 * Added UDouble32 in the NG calculation
 *
 * Revision 1.86  2009/07/03 13:37:21  gur15697
 * DATA_MSG_API_HEADER_LEN updated to 18
 *
 * Revision 1.85  2009/07/03 07:53:20  gur12140
 * Incorporated Changes .Make RE_TX = 0 and New_TX = 1
 *
 * Revision 1.84  2009/07/02 16:24:29  gur19413
 * changed BCCH_MSG value to 1 from 0
 *
 * Revision 1.83  2009/07/02 08:38:38  gur20052
 * updated the msgType enum for UEs scheduled for TA/DRX with Data
 *
 * Revision 1.82  2009/07/02 08:01:38  gur12140
 * Incorporated Comments
 *
 * Revision 1.81  2009/06/30 14:40:46  gur18569
 * removed compilation error , keeping second arg of lteShmLog PNULL
 *
 * Revision 1.80  2009/06/30 14:16:38  gur18569
 * replaced lteLog with lteShmLog
 *
 * Revision 1.79  2009/06/29 13:42:48  gur12140
 * Incorporated Comments
 *
 * Revision 1.78  2009/06/26 10:58:04  gur15697
 * DATA_MSG_API_HEADER_LEN changed to 20
 *
 * Revision 1.77  2009/06/26 07:24:56  gur12140
 * Added MAX_PRB_IN_SUBSET #def
 *
 * Revision 1.76  2009/06/25 16:18:44  gur20052
 *  Updated the MAcro of LTE_MAC_UT_LOG
 *
 * Revision 1.75  2009/06/25 11:24:33  gur11083
 * change value of CQIModeT enum
 *
 * Revision 1.74  2009/06/25 09:53:33  gur12140
 * Added Enum = 1 for PORT_CONF_ENUm
 *
 * Revision 1.73  2009/06/23 15:08:39  gur20052
 *  Updated Default value of MAX_NUM_DEMUX_THREADS to 1
 *
 * Revision 1.72  2009/06/23 15:01:03  gur18550
 * INVALID_UE_ID
 *
 * Revision 1.71  2009/06/22 16:17:53  gur20052
 *  Added a new enum CQIMode for different CQI MODES
 *
 * Revision 1.70  2009/06/22 10:15:10  gur12140
 * Incorporated Comments
 *
 * Revision 1.69  2009/06/22 04:54:28  gur15697
 * compilation warnings removed
 *
 * Revision 1.68  2009/06/19 12:35:58  gur18569
 * setting TX_MODE to 1
 *
 * Revision 1.67  2009/06/19 06:27:59  gur20052
 * Added MAC_MAX_UE_SCHEDULED from lteMacParseOAMMsg.h
 *
 * Revision 1.66  2009/06/18 13:43:22  gur20052
 * Updated the message type enum
 *
 * Revision 1.65  2009/06/18 13:34:54  gur20052
 * changed the default value of MAX_ADVANCE_DL_SUBFRAMES_ALLOC to 1 from 4
 *
 * Revision 1.64  2009/06/18 11:09:48  gur18550
 * Added Divide by 1024
 *
 * Revision 1.63  2009/06/18 10:20:36  gur15697
 * CCCH_TX added
 *
 * Revision 1.62  2009/06/17 13:03:08  gur20052
 * Added hash def GAP_VALUE_FOR_RA_DISTRIBUTED
 *
 * Revision 1.61  2009/06/17 05:28:08  gur18569
 * added invalid enum in data type
 *
 * Revision 1.60  2009/06/16 12:24:21  gur18550
 * ONE_BYTE = 1
 *
 * Revision 1.59  2009/06/12 05:02:35  gur18550
 * Added INVALID_TX
 *
 * Revision 1.58  2009/06/11 11:32:43  gur19413
 * added new macro INVALID_ALREADY_CONSIDERED_FLAG
 *
 * Revision 1.57  2009/06/10 14:40:12  gur18569
 * marked enums to be removed under start comment and end comment
 *
 * Revision 1.56  2009/06/10 13:36:25  gur18569
 * replaced lteLog with lteShmLog
 *
 * Revision 1.55  2009/06/10 13:03:41  gur20052
 *  Updated the message type and commented unused message types
 *
 * Revision 1.54  2009/06/09 14:17:23  gur15697
 * MCS_INDEX_FOR_CCCH changed
 *
 * Revision 1.53  2009/06/09 07:24:08  gur18550
 * Added RECEIVED_NACK_RE_TRANS_COMPELETE
 *
 * Revision 1.52  2009/06/09 06:31:39  gur12140
 * Added Coding rate 1/3
 *
 * Revision 1.51  2009/06/05 16:55:37  gur20052
 * Removed Hash defined
 *
 * Revision 1.50  2009/06/05 06:59:39  gur19413
 * Removed Compialtion Errors
 *
 * Revision 1.49  2009/06/05 06:36:03  gur20052
 * Added Length of TA/DRX/TA_DRX in BYTES
 *
 * Revision 1.48  2009/06/04 12:47:17  gur15697
 * UE_CONTENTION_RESOLUTION_IDENTITY_LCH  added
 *
 * Revision 1.47  2009/06/04 09:21:33  gur20052
 * Added Header in BYTES
 *
 * Revision 1.46  2009/06/04 08:17:50  gur12140
 * Removed Compilation Error
 *
 * Revision 1.45  2009/06/04 07:53:23  gur18550
 * Structure added
 *
 * Revision 1.44  2009/06/04 07:48:30  gur15697
 * Merged the changes of 1.42 with 1.41 as previous merge was improper
 *
 * Revision 1.43  2009/06/03 16:45:11  gur18550
 * File Updated
 *
 * Revision 1.42  2009/06/03 11:59:33  gur15697
 * AGGREGATION_LEVEL_FOR_COMMON_SPACE  added
 *
 * Revision 1.41  2009/06/03 07:05:43  gur18569
 *  changes regarding DATA_MSG_API_LENGTH
 *
 * Revision 1.40  2009/06/02 13:20:32  gur20052
 * updated LTE_MAC_UT_LOG
 *
 * Revision 1.39  2009/06/02 12:28:17  gur18550
 * Updation for TB
 *
 * Revision 1.38  2009/06/02 06:53:16  gur20470
 * added MAC_PARTIAL_SUCCESS enum
 *
 * Revision 1.37  2009/05/29 10:22:23  gur18569
 * removed compilation error
 *
 * Revision 1.36  2009/05/28 10:00:44  gur20052
 * Added AddDeleteTrigger enum
 *
 * Revision 1.35  2009/05/28 05:48:48  gur15697
 * RATPolicyT modified as per the review comments
 *
 * Revision 1.34  2009/05/27 10:01:59  gur15697
 * macros for Random access added
 *
 * Revision 1.33  2009/05/25 16:21:25  gur20052
 * Added new MAX_UNUSED_RB_IN_A_SUBFRAME parameter
 *
 * Revision 1.32  2009/05/25 12:38:55  gur12140
 * Removed Compilation Errors
 *
 * Revision 1.31  2009/05/23 11:15:16  gur18550
 * DL HARQ Timer Index
 *
 * Revision 1.30  2009/05/21 06:31:56  gur20052
 * Removed Compilation error
 *
 * Revision 1.29  2009/05/21 05:13:27  gur19413
 * new macro MAX_SUB_FRAME added
 *
 * Revision 1.28  2009/05/20 13:03:29  gur20052
 * Updated the message Type
 *
 * Revision 1.27  2009/05/20 12:11:38  gur12140
 * PDSCH and PDCCH Comments Incorporated
 *
 * Revision 1.26  2009/05/20 08:03:01  gur20052
 * Added new Hash Define
 *
 * Revision 1.25  2009/05/19 16:05:59  gur18569
 * incorporated review comments
 *
 * Revision 1.24  2009/05/19 06:47:11  gur20052
 * Added NUM_OF_UE_TO_BE_SCHEDULED
 *
 * Revision 1.23  2009/05/18 15:27:41  gur15697
 * SPLIT_ZERO_BUFFER_COPY macro added, Truncated BSR macro added
 *
 * Revision 1.22  2009/05/15 05:37:44  gur19413
 * added new MACRO INVALID LCID & RLC_MODE
 *
 * Revision 1.21  2009/05/12 10:40:54  gur15697
 * macro defined for BCCH
 *
 * Revision 1.20  2009/05/12 10:29:46  gur18550
 * File update for acknackStatus
 *
 * Revision 1.19  2009/05/12 08:34:28  gur19413
 * added new Macro INVALID_MCS_INDEX
 *
 * Revision 1.18  2009/05/12 06:51:42  gur15697
 * macros for  BCCH added
 *
 * Revision 1.17  2009/05/10 13:12:22  gur18569
 * added MAC_TA_LCH MAC_DRX_LCH
 *
 * Revision 1.16  2009/05/10 12:07:17  gur12140
 * Added msgIdnetifer in DciCCE Info strcture
 *
 * Revision 1.15  2009/05/10 11:33:50  gur12140
 * Changed isNewData variable name in TB Structure to txIndicator
 *
 * Revision 1.14  2009/05/10 09:18:37  gur12140
 * Added PortConf Enum
 *
 * Revision 1.13  2009/05/10 08:14:50  gur19413
 * added harq related macro
 *
 * Revision 1.12  2009/05/10 07:56:20  gur12140
 * Changed Structure of DCI CCE Info Structure
 *
 * Revision 1.10  2009/05/09 10:21:09  gur20052
 * Added MACROS for the Modulo, Division and Multiplication (powers of 2)
 *
 * Revision 1.9  2009/05/09 09:49:49  gur20052
 * added new MACRO LTE_MAC_UT_LOG for Logs
 *
 * Revision 1.8  2009/05/08 10:26:38  gur15697
 * New Macros added
 *
 * Revision 1.7  2009/05/08 10:08:34  gur20052
 * Added new enum
 *
 * Revision 1.6  2009/05/08 09:59:46  gur15697
 * compiling
 *
 * Revision 1.5  2009/05/08 09:53:16  gur15697
 * updated for Logical channel ID macros
 *
 * Revision 1.4  2009/05/08 05:40:41  gur12140
 * Added Enum For MsgType to for Spatial Mul and NOn Spatial Mul
 *
 * Revision 1.2  2009/05/06 11:30:53  gur12140
 * File Modified .Revision History Deleted
 *
 * Revision 1.1  2009/05/06 11:28:51  gur12140
 * File Added in which all common defintion is there which is needed by different Module
 *
 ****************************************************************************/

#ifndef LTE_MAC_TYPES_H
#define LTE_MAC_TYPES_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
/* SPR 21857 changes */
/*This is just the indication that actual wideband PMI value should be used*/
#define WIDEBAND_PMI 7
/* SPR 21857 changes */

#ifdef TDD_CONFIG
/* SPR 5333 changes start */
#define MAX_VALUE_M_I 3
/* SPR 5333 changes end */
/* SPR 609 changes start */
#ifndef _GNU_SOURCE 
/* SPR 609 changes end */
#define _GNU_SOURCE
/* SPR 609 changes start */
#endif 
/* SPR 609 changes end */
#endif
#include "lteTypes.h"
#include "lteLog.h"
#include "logging.h"
#include "lteHash.h"
#include "lteMemPool.h"
#include "lteList.h"
#include "lteTree.h"
#include "lteQueue.h"
#include "lteSQueue.h"
#include "lteThread.h"
#include "lteSync.h"
#include "port.h"
#include "lteMisc.h"
#include "lteMsgPool.h"
#include "alarm.h"
#include "lteCommonStatsManager.h"
#include "lteMacOAMInterface.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* MACROS to determine maximum of two values */
#define MAC_MAX_VAL(A,B) \
     ((A) > (B)) ? (A) :(B)

/* MACROS to determine minimum of two values */
#define MAC_MIN_VAL(A,B) \
    ((A) < (B) ) ? (A) : (B)

#define ADVANCE_ALLOC_INIT_DONE   2
#define LAYER_3_IDX_0      0
#define LAYER_4_IDX_1      1
#define MCS_27_IDX_0       0
#define MCS_28_IDX_1       1
#define TB_ON_LAYER_3_OR_4 2
#define MCS_27_OR_28       2
#define UE_CAT_5           5
/* SPR 11355 Fix Start */
#define UE_CAT_8           8
/* SPR 11355 Fix End */
#define SHIFT_BY_1_BIT     1
#define SHIFT_BY_2_BIT     2
#define SHIFT_BY_3_BIT     3
#define SHIFT_BY_4_BIT     4
#define SHIFT_BY_5_BIT     5
#define SHIFT_BY_6_BIT     6
#define SHIFT_BY_7_BIT     7
#define PMI_VALUE_7        7
#define CBSR_IDX_0         0
#define CBSR_IDX_1         1
#define CBSR_IDX_2         2
#define CBSR_IDX_3         3
#define CBSR_IDX_4         4
#define CBSR_IDX_5         5
#define CBSR_IDX_6         6
#define CBSR_IDX_7         7
#define NUMBER_OF_BITS_IN_1BYTE 8
#define MAC_LTE_MEMORY_HARQ_RTX_FACT 10

/* TDD Config 6 Changes Start */
/* MACRO to Get SFN and SF from TTI */
#define GET_SFN_SF_FROM_TTI(tti, sfn, sf) \
{\
    sf  = (tti) % MAX_SUB_FRAME;\
    sfn = (MODULO_ONEZEROTWOFOUR((tti)/MAX_SUB_FRAME));\
}
/* TDD Config 6 Changes End */


#if (defined(DL_UL_SPLIT) || defined(DL_UL_SPLIT_TDD))
#define SINR_CONTAINER_SIZE 3
#define MAC_TICK_UPDATED    1
#endif



typedef struct mapInfoT
{
    UInt32 passiveIndex;
    UInt32 activeIndex;
}mapInfo;

extern mapInfo* measGap40msInfo_gp[MAX_NUM_CELL];
extern mapInfo* measGap80msInfo_gp[MAX_NUM_CELL];
#ifdef ENDC_ENABLED
/*Meas_Gap_Changes_Start*/
extern mapInfo* measGap20msInfo_gp[MAX_NUM_CELL];
extern mapInfo* measGap160msInfo_gp[MAX_NUM_CELL];
/*Meas_Gap_Changes_End*/
#endif
extern mapInfo* shortPeriodicSRSMapInfo_gp[MAX_NUM_CELL];
extern mapInfo* longPeriodicSRSMapInfo_gp[MAX_NUM_CELL];
extern mapInfo* longPeriodicSRCQIPMIRIMapInfo_gp[MAX_NUM_CELL];
extern mapInfo* shortPeriodicSRMapShortInfo_gp[MAX_NUM_CELL];
/* This global tick is used for Log/HSM */
/* SPR 15909 fix start */
extern tickType_t macTTITickCount_g;
/* SPR 15909 fix end */
extern UInt8 servingCellBitMask_g[MAX_NUM_CELL];

/*EICIC CHANGES STARTS*/
#ifdef FDD_CONFIG
#define NUM_BITS_ABS_PATTERN 40
#else
#define NUM_BITS_ABS_PATTERN 70
#define NUM_BITS_ABS_PATTERN_CONFIG6 60
#define NUM_BITS_ABS_PATTERN_CONFIG1_5 20
#endif

#define ABS_PATTERN_TYPE				0
#define USABLE_ABS_PATTERN_TYPE		1
#define UPDATE_TYPE_INDEX				2
/*EICIC CHANGES ENDS*/

/*SPR 16855 +-*/
#define MAX_CELL_INDEX              8
#define MAX_SCELL_INDEX             7
#define START_SCELL_INDEX           1
#define CELL_PHY_RUNNING_TIMEOUT_NSEC       5000000

#ifdef FDD_CONFIG
/***********************************************************************
 * * QueueName              : ttibScheduleQueue_g[MAX_TTIB_SCHEDULE_QUEUE]
 * * Structure of Queue Node: ttibScheduleNode
 * * Queue Type             : Circular Queue 
 * ***********************************************************************/
#define TTI_BUNDLE_SIZE       4
#define TTI_BUNDLING_RTT          16
#define MAX_TTI_BUNDLE_RB         3
#define MIN_TTI_BUNDLE_RB         1
#define MAX_TTI_BUNDLE_MCS        10
#define MAX_TTIB_SCHEDULE_QUEUE   TTI_BUNDLING_RTT
#define  TTI_BUNDLE_HARQ_MAP      TTI_BUNDLING_RTT
/* This is used as for calculating DCI etc. during retx of TTIB 
        transmission after 9 Ticks of PHICH
    so TTI_PHICH_TO_DCI_OFFSET + Normal Retx transmission offset = 9*/
#define TTI_PHICH_TO_DCI_OFFSET   5
#define TTIB_FIRST_TO_LAST_TICK_OFFSET 3

#define TTI_MAP_FREE              8
//Coverity_36234 fixed
#define MAX_RB_NODE_TO_CHECK      16
typedef struct {
    UInt16 ueIndex;
    UInt8 harqProcessId;
    UInt8 isSpsEnable;
}TtiBScheduleNode;

typedef struct {
    UInt16 numOfUe;
    UInt8 totalNumOfRbUsed;
}TtiBundleMapQueueInfo;

/* Queue for TTI bundling scheduling */
/* SPR 6363 FIX start */
extern   TTIB_SCHEDULE_TYPE*      ttibScheduleQueue_gp[MAX_NUM_CELL];
extern   TtiBundleMapQueueInfo   ttiBundleMapQueueInfo_g[MAX_TTIB_SCHEDULE_QUEUE];
/* SPR 6363 FIX END */
#define GET_TTIB_HARQ_ID(Tick) DIVIDE_BY_FOUR( MODULO_SIXTEEN(Tick) ) 
typedef struct LteMacTtibFddStatsT
{
    /*  Number of TTI bundle UEs */
    UInt64 numOfTtibUE;
    /*  Number of times TTI Bundle UE scheduled at uplink scheduler. */
    UInt64 numOfTtibSchedule;
    /*  Number of times TTI Bundle UE denied at uplink scheduler. */
    UInt64 numOfTtibDeniedUlSchedular;
    /*  Number of times TTI Bundle UE denied at PUCCH. */
    UInt64 numOfTtibDeniedPUCCH;
    /*  Number of times TTI Bundle UE denied at PUSCH. */
    UInt64 numOfTtibDeniedPUSCH;
    /*  Number of non-adaptive retransmission. */
    UInt64 numOfTtibNonAdaptReTx;
    /*  Number of adaptive retransmission. */
    UInt64 numOfTtibAdaptReTx;
    /*  Number of Harq timers expired for TTI bundle Harq */
    UInt64 numOfTtibHarqTimerExpiry;
}LteMacTtibFddStats;
#endif
#define MAX_RES_BLOCKS              100

#define MAX_NUM_MAPS        2
/*spr 19296 fix start*/
typedef struct TlvHeaderT
{
    UInt16      transactionId;
    UInt16      sourceModuleId;
    UInt16      destinationModuleId;
    UInt16      apiId;
    UInt16      length;
/* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING    
    UInt8       cellIndex;
    UInt8       reserved;
/*SPR 21653 changes start*/
    UInt32      reserved2;
/*SPR 21653 changes end*/
#endif    
/* - Layer2 NON CA Changes */
} TlvHeader, *LP_TlvHeader;

typedef struct TlvMessageT
{
    TlvHeader  header;
    UInt8      data[];
} TlvMessage, *LP_TlvMessage;
/*spr 19296 fix end*/
/*Data and Control Info delay at PHY in UL direction in milliseconds*/
#define PHY_UL_CONTROL_DELAY    2
#define PHY_UL_DATA_DELAY       2

#ifdef FDD_CONFIG
/*Delay at PHY in DL direction in milliseconds*/
#define PHY_DL_DELAY            1
#elif TDD_CONFIG
#define PHY_DL_DELAY            0
#endif
#if   FLEXRAN
#define PHY_DELAY               5       /*delay in milliseconds*/
#else
#define PHY_DELAY               3       /*delay in milliseconds*/
#endif	

#define TAG_FIELD_LEN           2
#define LENGTH_FIELD_LEN        2
#define SFN_FIELD_LEN           2
/*Fix for __SPR_1988__ START*/
/* + SPR_3939 */
#define UL_SYNC_TIMER_DIFF_THRESHOLD_FOR_SENDING_TA 50
/* - SPR_3939 */
#define UL_SYNC_PDCCH_ORDER_DELAY PHY_UL_DATA_DELAY
/*Fix for __SPR_1988__ END*/
/*SPR_3061_FIX*/
#define TIME_ALIGNMENT_TIMER_INFINITY        65535
#define UL_SYNC_TIMER_INFINITY               0
/*SPR_3061_FIX*/
 
 /* Maximum duration between transmission of TA and reception of ACK   
  * In case of FDD the value is 4 
  * In case of TDD in the case of Configuration 5 
  * if TA is transmitted in SF 9 and SFN N
  * ACK will be recieved in SF 2 and SFN N+2. The value is 13 Milliseconds 
  */

#ifdef FDD_CONFIG
#define UL_SYNC_TIME_TO_PROCESS_TA (4 + PHY_DL_DELAY  + PHY_UL_DATA_DELAY) 
#elif TDD_CONFIG
#define UL_SYNC_TIME_TO_PROCESS_TA (13 + PHY_DL_DELAY + PHY_UL_DATA_DELAY) 
#endif


/* SPR 1542 changes start */
#define SI_MAPPING_FIELD_LEN    1
/* SPR 1542 changes end */
#define DATA_MSG_API_HEADER_LEN 0

#define MAX_SYS_FRAME_NUM   1023

/*SPR_3305_FIX_START*/
#define MAX_SUB_FRAME_FOR_PHICH 10
 /*SPR_3305_FIX_END*/

#define MAX_LOCALIZED_VRB 100
#define MAX_DISTRIBUTED_VRB 100
#define GAP_VALUE_ONE 1
#define GAP_VALUE_TWO 2

 
/* valid ranges */
#define MAC_MAX_UE_SCHEDULED 40

/* SPR 4692 Fix Start */
#define MAC_RACH_PDCCH_DELAY 1
/* SPR 4692 Fix End */

/* MAC default values */
/* MAC Downlink Scheduler Weights */
#define MAC_SCH_MAX_DWLK_UE_NACK_QL_WT    20
#define MAC_SCH_DEFAULT_DWLK_UE_NACK_QL_WT     10

//HARQ Processes
#define MAC_MIN_HARQ_PROCESSES 6
#define MAC_MAX_HARQ_PROCESSES 8
#define MAC_DEFAULT_NUM_HARQ_PROCESSES 8

/*HARQ Timer*/
#define INVALID_HARQ_ID 255
#define MAC_MIN_HARQ_TIMER_VALUE 5
#define MAC_MAX_HARQ_TIMER_VALUE 8
#define MAC_DEFAULT_HARQ_TIMER_VALUE 6

/* ULSCH UL TX Mode*/
#define MAC_UL_TX_MODE_SISO 0
#define MAC_UL_TX_MODE_MIMO 1

/*HARQ Retransmissions*/
/*SPR 3691 changes start*/
#define MAC_MIN_HARQ_RETRANSMISSIONS 1
#define MAC_MAX_HARQ_RETRANSMISSIONS 8
/*SPR 3691 changes end*/
#define MAC_DEFAULT_HARQ_RETRANSMISSIONS 6

#define MAC_DEFAULT_NUM_RESOURCE_BLOCKS 25
#define MAC_DEFAULT_MODULATION_SCHEME 0
#define MAC_DEFAULT_HARQ_SCHEME 2

#define MAC_MIN_RESOURCE_BLOCKS 6
#define MAC_MAX_RESOURCE_BLOCKS 110
#define MAC_MIN_RANGE_VALUE 1
#define MAC_MAX_RANGE_VALUE 110
#define CODING_RATE_ONE_BY_THREE  3
#define MIN_ALLOCATABLE_RB 1
#define MAX_NUM_PUSHED_IN_DL_ADVANCE_QUEUE 2
#ifdef DL_UL_SPLIT
#define INVALID_OPERATION_TYPE  255
#endif

/* SPR 1571 changes start */
#define RAT_LOCALIZED 1
#define RAT_DISTRIBUTED 2
/* SPR 1571 changes end */
#define MAX_PDCCH_ORDER_RETRIES 4 
/* SPR 9427 changes start */\
#define MS_TO_TRIGGER_AP_CQI        1000  //This is configurable and can be changed in future
/* SPR 9427 changes end */\
/*Dynamic PDCCH + 5117 Changes Start*/ 
/*coverity fix start*/
#define MAX_NUM_OVERLAPPING_RB 7
/*coverity fix End*/
/*Dynamic PDCCH + 5117 Changes End*/

/******** MAC MODULE PREFIX STRINGS*******/
#define MAC_MOD_ENCODER     "ENCODER"
#define DL_STRATEGY         "DL_STRATEGY"
#define DL_PF_STRATEGY      "DL_PF_STRATEGY"
#define DL_RR_STRATEGY      "DL_RR_STRATEGY"
#define UL_STRATEGY         "UL_STRATEGY"
#define UL_PF_STRATEGY      "UL_PF_STRATEGY"
#define UL_RR_STRATEGY      "UL_RR_STRATEGY"
#define DL_Q_LOAD_UPDATE  "DL_QUEUE_LOAD_UPDATE"

/******** MAC MODULE PREFIX STRINGS*******/


/* SPR#662 Changes Start */
    /*
    * In Non UT case a File Logger is used
    * UT SHM logger creates a logger thread that enables
    * which is later supposed to be later processed by
    * logger thread.
    * This model of logging is advantageous if Multi Core(>2)
    * or Hyper threaded CPU is taken for developement.
    * Otherwise in 2 or lesser cores it adds a extra
    * thread to be processed by our expensive resource: CPU
    */
    #define LTE_MAC_UT_LOG(param1,param2,...)
/* SPR#662 Changes End */
 
 
#define MAX_SUB_BAND                       28
#define MAX_RBG                             28
#define MAX_RBG_SUBSET                     4
#define MAX_PRBS_IN_SUBBAND                 28
#define MAC_INVALID_DCI                     0xFF
#define MAC_INVALID_HARQ_ID                 0xFF
#define INVALID_DAI_VALUE                   4
#define DAI_VALUE_ONE                       1
#define MAX_ADVANCE_DL_SUBFRAMES_ALLOC      1
#define MAX_ADVANCE_UL_SUBFRAMES_ALLOC      1
#define MAX_UNUSED_RB_IN_A_SUBFRAME         6


/* TTIB_Code Start */
#ifdef FDD_CONFIG
#define MAX_CCE_CONTAINER 6
#else
#define MAX_CCE_CONTAINER 3
#endif
/* TTIB_Code End */

#define MAX_CCE 100
/* SPS_CHG_START */
/* + SPS_TDD_Changes */
#define MAX_SPS_CCE 10
/* - SPS_TDD_Changes */

/* CA_phase2_harq_code Start */

/* phase2_RJ_21 Fix Start */
#define MAX_CCE_INDEX MAX_CCE
/* phase2_RJ_21 Fix End */

/* CA_phase2_harq_code End */

/* SPS_CHG_END */
/** SPR 3657 Changes Start **/
/* As MAC PDU can have max 100 RLC data PDU, 6 MAC Control Element,
 * 2 or 1 Byte for Padding, so herder size can be= ((100*3) + (6*1)+ 2)) */
/* #define MAX_MAC_HEADER_MAC_CCE_LEN 300*/
#define MAX_MAC_HEADER_MAC_CCE_LEN 308
/** SPR 3657 Changes End  **/
#define CCE_FREE 1
#define CCE_ALLOCATED 0

#define NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME  MAX_SUB_FRAME
#define VRB_BUSY 0
#define VRB_FREE 1
#define MIN_TB_SIZE 4
#define MAX_PRB_IN_SUBSET 28

/*These are the minimum number of allocation done when only TA / DRX 
   or TPC need to send by Strategy */
#define MIN_RB_FOR_TA_DRX_ONLY 1
#define MIN_RB_FOR_TPC_ONLY     1

#define MIN_RB_FOR_BOTH_TA_DRX_ONLY 2

#ifdef TDD_CONFIG
#define MIN_RB_FOR_BOTH_TA_DRX_ONLY_TDD 3
#endif
/*These are Length of TA/DRX/TA_DRX/TPC in BYTES*/
#define LENGTH_OF_TA                            2
#define LENGTH_OF_DRX                           1
#define LENGTH_OF_TA_DRX                        3
#define LENGTH_OF_TPC                           1

#define LENGTH_OF_SCELL_ACT_DEACT_CE            2
#define MIN_RB_FOR_SCELL_ACT_DEACT_ONLY         1
/* + SPR 22352 fix */
#define LENGTH_OF_TA_SCELL_ACT_DEACT_CE         4
#define LENGTH_OF_TA_DRX_SCELL_ACT_DEACT_CE     5
#define MIN_RB_FOR_TA_DRX_SCELL_ACT_DEACT_ONLY    3
/* - SPR 22352 fix */
/*This field indiactes for how many Subframe in advance the scheduler
  strategy executed and distribute the RBs*/
#define MAX_ADVACNE_ALLOC_SUBFRAME 4

/*This is maximum number of RBs distributed to the UE at the time of 
 Round robin Scheduling*/
#define MAX_RB_ALLOC_IN_ROUND_ROBIN_SCH 10

#define MAX_SUBBAND 58
#define MAX_NUM_POLICY 4
#define MAX_NUM_SUBSET 4
#define MCS_INDEX_TB_DISABLED 0
#define REDUNDANCY_VER_TB_DISABLED 1
#define ULSCH_UCI_INVALID_INDEX MAX_UE_SUPPORTED
/* Time Averaging chages start */
#define MAX_MCS_INDEX_ARRAY 29
/* Time Averaging chages end */
/* Review comment fix start GG16 */
#define INVALID_RB_MAP_NUM 255
/* Review comment fix end GG16 */
/* +-Review comment fix */
#define CCE_NOT_ALLOCATED 250
#define INVALID_CCE_INDEX 255
#define INVALID_DRX_VALUE 255
#define INVALID_TA_VALUE 255
#define INVALID_PRECODING_MATRIX_INDEX 255
#define INVALID_RANK_INDICATOR 255
#define INVALID_SCELL_ACT_DEACT_VALUE 1

/* SPR 3256 START */
#define INVALID_CQI_VALUE   0
/* SPR 3256 END */
#define MAX_COMMOM_CH_MSG_SCHEDULED 4
#define MAX_CCCH_UE_SCHEDULED 4
#define MAX_DL_COMMON_CHANNEL_MSG 10
#define TB_VALID 1
#define TB_INVALID 0
#ifdef TDD_CONFIG
#define MAX_DL_GROUP_SIZE_TDD 9
#endif

#ifdef DL_UL_SPLIT
#define MAX_NUM_TICK_MISS 8
#endif

#define MAX_DL_SCH_ARR 64
#define MAX_UL_SCH_ARR 64

/* SPR 1101 changes start*/
#define MAX_CONTENTION_UE_SCHEDULED 10
/* SPR 1101 changes end*/

/*As per spec Max TB size in DCI 1A is 456*/
#define MAX_TB_SIZE_FOR_DCI_FORMAT_1A 456

#define DCI_FORMAT_FOR_PCCH   DCI_FORMAT_1A
#define DCI_FORMAT_FOR_BCCH   DCI_FORMAT_1A
#define DCI_FORMAT_1A_FOR_CCCH DCI_FORMAT_1A
#define DCI_FORMAT_1_FOR_CCCH  DCI_FORMAT_1
#define DCI_FORMAT_FOR_RAR     DCI_FORMAT_1A
#define DCI_FORMAT_FOR_CONTENTION_MSG     DCI_FORMAT_1A

#define AGGREGATION_LEVEL_FOR_COMMON_SPACE AGGREGATION_LEVEL_FOUR
#define AGGREGATION_LEVEL_FOR_PDCCH_ORDER_MSG AGGREGATION_LEVEL_FOUR
#define AGGREGATION_LEVEL_FOR_CCCH_MSG AGGREGATION_LEVEL_FOUR
#ifdef FLEXRAN
/* ASTRI FLERAN L1 supports values upto 2 (-3 db).
 * */
#define DEFAULT_PA_VALUE 2
#else
#define DEFAULT_PA_VALUE 4
#endif

#define MCS_INDEX_FOR_CCCH 0

/* +Bandwidth Fix */
#define MIN_REQD_RB_FOR_CCCH 15
#define MAX_MCS_INDEX_FOR_CCCH 6
/* -Bandwidth Fix */
#define TCRNTI_HARQ_ID 1

#define PRNTI 0xFFFE
#define SIRNTI 0xFFFF
#ifdef LTE_EMBMS_SUPPORTED
 #define MRNTI  0xFFFD
#endif
/*SPR 1273 START*/
#define MAX_TEMP_QUEUELOAD_ARR_SIZE  1
/*SPR 1273 END*/

#define MAX_MUX_CONTAINERS 4
#define MAX_ENCODER_CONTAINERS MAX_CCE_CONTAINER
#define MAX_NUM_UE                         MAX_UE_SUPPORTED
/*******Memory Handling changes 64UE Start**************************/
/* SPR 22075 Changes Start */
/*#define NUM_UE_MSG_POOL_EXT_1  64
#define NUM_UE_MSG_POOL_EXT_2  128 */
/* SPR 22075 Changes End */
/** SPR 13295 Changes Start **/
#define NUM_16K_MSG_BUF_EXT_2  1900 
/* SPR 22075 Changes Start */
/* #define NUM_16K_MSG_BUF_EXT_1  900 */
/* SPR 22075 Changes End */
/** SPR 13295 Changes End **/
/*******Memory Handling changes 64UE End****************************/
#define MAX_NB_MULTIPLEXER_THREADS         4
#define MAX_NB_ENCODER_THREADS             6

#define HARQ_PROCESS_FREE                  0
#define HARQ_PROCESS_IN_USE                1
#define HARQ_PROCESS_RESET                 2
#define TO_BE_DELETED                      1
#define DELETION_COMPLETE                  0
#define MAC_CONTROL_ELEMENT_SUBHEADER_SIZE 1
#define MAC_CRNTI_SIZE                     2
#define MAC_PADDING_SUBHEADER_SIZE         1
#define MAC_SDU_SUBHEADER_LEN_TWO_BYTES    2
#define MAC_SDU_SUBHEADER_LEN_THREE_BYTES  3
#define ONE_BYTE                           1
#define SHIFT_EIGHT_BITS                   8
#define HARQ_EXPIRY_TICK                   8
#define MAX_RETRANSMISSION_ALLOWED         10

#ifdef FDD_CONFIG
#define MAX_HARQ_PROCESS_NB                8
#elif  TDD_CONFIG
#define MAX_HARQ_PROCESS_NB                15
#endif

/** SPR 3657 Changes Start **/
#define MAX_NUM_UL_PKTS        106
/** SPR 3657 Changes End **/
/* SPR 20834 Changes Start */
/* MAX_NUM_CE_IN_UL_PDU value is considered MAC CE (BSR, PHR, CCCH, PADDING and CRNTI)
** which can be received simultaneously in uplink packet with 5 buffer, other than RLC PDU. 
** Note: CCCH and Padding are generally not stored, butkept for reference
*/
#define MAX_NUM_CE_IN_UL_PDU 10
/* This fix is temporarily removed for Intel board as it is increasing CPU usage. 
 * It is under discussion and will update the changes once discussion is done */
#define MAX_NUM_OF_SUBHEADER_IN_UL_PDU (MAX_NUM_UL_PKTS + MAX_NUM_CE_IN_UL_PDU)
/* SPR 20834 Changes End */
#define MAC_CONTROL_ELEMENT_SUBHEADER_SIZE 1
#define MAC_CRNTI_SIZE                     2
#define MAC_PADDING_SUBHEADER_SIZE         1
#define MAC_SDU_SUBHEADER_LEN_TWO_BYTES    2
#define MAC_SDU_SUBHEADER_LEN_THREE_BYTES  3

#define MAX_TICK_TTI_COUNT                 4294967295
#define MAX_LC_PRIORITY_RANGE              16
#define MIN_LC_PRIORITY_RANGE              1
#define MAX_WEIGHTED_LC_PRIORITY           17
#define MAX_UE_PRIORITY_RANGE              3
#define MIN_UE_PRIORITY_RANGE              0
#define MAX_WEIGHTED_UE_PRIORITY           4
#define RLC_HEADER_SIZE                    16
#define HARQ_TB_MINIMUM_SIZE               48 //(3 Byte SDU HEADER + 
                         // 3 Bytes of Data (2 Bytes of RLC Header 
                         // + 1 Byte of MAC SDU))

/* SPR 3108 changes start */
/* SPR 8540 Fix Start */
#define HARQ_TB_MINIMUM_SIZE_IN_BYTES      6
/* SPR 8540 Fix End */
/* SPR 3108 changes start */
#define MAC_SDU_SUBHEADER_SIZE             24
#define MAC_SDU_SUBHEADER_SIZE_IN_BYTES    3


/* DL QOS Strategy */
/* DL Qos Changes Start */
#define MAX_TKN_SCHD_WINDOW         10
#define MAX_QLOAD_PRIORITIES        5
#define MAX_PDB_PRIORITIES          5
#define MAX_TOKEN_PRIORITIES        5
#define NUM_TICKS_PER_SECOND        1000

/*moved from Encoder to here as it is used in the DL Strategy */
#define MAC_SUBHEADER_LENGTH_THREE_BYTE  3

#define MAX_MCS_PRIORITIES          3
#define MAX_QCI_PRIORITIES          9
/* CA changes start */
#define MAX_UE_CATEGORY             8
/* CA changes end */
/* DL Qos Changes Stop */
/*chandes for UL scheduler Phase -2 */
/* ULA_CHG */
#define MAX_HARQ_RETRANSMISSION_COUNT      8
/* ULA_CHG */
/* SPR_1987 Start */
#define MAX_UL_HARQ_RETRANSMISSION_COUNT   28
/* SPR_1987 End */
/*Long BSR :3 Bytes MCSIndex 0->2 No.of RBS */
#define BSR_LENGTH                         5  
/*short BSR : 1 Bytes  MCSIndex 0 ->1 No.of RBS */
#define UL_RETRANS_TYPE                    1

#define TX_DELAY_UL                        1
#define TX_DELAY_DL                        1/*3*/
#define UE_ADV_TTI                         4
#define UE_NACK_RT_TIME                    4
#define UE_NACK_RECV_RETRAN                4

#define MAX_CONTAINER_NUM                  8
#define LAST_CONTAINER_TO_START_SR         4
/*This is to process last Five containers*/
#define LAST_START_CONTAINER (LAST_CONTAINER_TO_START_SR  - MAX_CONTAINER_NUM)
/*minimum 80 bits : RBs allocated is 4 */
#define RB_FOR_RAR                         4
#define RA_WINDOW_BEGIN                    3
#define RA_WINDOW_END                      6
/*In RA_WINDOW_SIZE's upper ceiling is set. It is because after the 6 (ceil), 
it will start conflicting with the resources of the presnt tick's other 
messages i.e. NACK etc.*/
#define RA_WINDOW_SIZE     ((RA_WINDOW_END - RA_WINDOW_BEGIN) > (4) ? (4) : (RA_WINDOW_END - RA_WINDOW_BEGIN))           
#define MAX_NUM_UL_RBS                     40
#define MAX_NUM_DL_RBS                     40
#define BACKOFF_INDEX                      1

#define MAX_BSR_INDEX                      64
#define MAX_SUPPORTED_USER                 MAX_UE_SUPPORTED
#define LTE_LAYER2_DELAY                   1
#define MAX_WEIGHTED_LCG_PRIORITY          4
#define INVALID_NACK_BIT                   255
#define INVALID_SR_PERIODICITY             0
#define NACK_UL_ALLOC_TYPE                 0
#define ACK_UL_ALLOC_TYPE                  1
#define SR_UL_ALLOC_TYPE                   2
#define NEW_UL_ALLOC_TYPE                  3
                                                                             
#define SIZE_OF_UL_GRANT_TO_PHY            19
#define MAX_TTI_MAP                        8
#define UL_ADAPTIVE_RETX                   1
#define UL_NON_ADAPTIVE_RETX               0                             
#define NACK_MESSAGE_TYPE                  0
#define ACK_MESSAGE_TYPE                   1
#define SR_MESSAGE_TYPE                    2
#define UL_TB_MINIMUM_SIZE                 48                                                                             
#define UL_TB_MINIMUM_SIZE_IN_BYTES        6
#define UE_RA_START_LISTEN                 3                                                            
#define MAX_RA_RESOURCE_LIST_SIZE          16                                                                           
#define RB_FOR_AP_CQI                      1              
#define TBL_SIZE                           MAX_UE_SUPPORTED

#define MAC_SCH_MIN_UPLK_UE_BSR_WT         0
#define MAC_SCH_MAX_UPLK_UE_BSR_WT         20
#define MAC_SCH_DEFAULT_UPLK_UE_BSR_WT     10
#define MAC_SCH_DEFAULT_UPLK_RETRANS_TYPE  0

/*This is assumption for the considering the uplink mac header sise*/
#define MAX_BSR_HEADER_OFFSET 5   
#define START_INDEX                   0
#define BIT_ONE                       0x01
#define MAX_RBG                       28 
#define MAX_SUBSETS                   4 
#define MAX_RBG_IN_SUBSET             8 
#define MAX_PRBS_IN_SUBSET            28
#define MAX_SUBBANDS                  28
/*SPR 20899 Fix Start */
#define MID_SUBBANDS                  14
/*SPR 20899 Fix Stop*/

/*SPR 5508 Start*/
/* SPR 18122 Changes Start*/
#define MAX_SUBBANDS_PMI_INFO         DIVIDE_BY_TWO(MAX_SUBBANDS)
/* SPR 18122 Changes End*/
/*SPR 5508 End*/
#define CODING_MULTIPLIER             1024
/* + TM7_8 Changes Start */
#define MAX_VALUE_OF_AOA              719 
#define INVALID_VALUE_OF_AOA          720 
#define NUM_OF_TX_ANTEENA_VALUE_4     4
/* - TM7_8 Changes End */

/* SPR 2063 start */
/* SPR 2063 end */

#define INVALID_MCS_INDEX             255
#define MCS_INDEX_QPSK_0              0
#define MCS_INDEX_QPSK_1              1
#define MCS_INDEX_QPSK_2              2
#define MCS_INDEX_QPSK_3              3
#define MCS_INDEX_QPSK_4              4
#define MCS_INDEX_QPSK_5              5
#define MCS_INDEX_QPSK_6              6
#define MCS_INDEX_QPSK_7              7
#define MCS_INDEX_QPSK_8              8
#define MCS_INDEX_QPSK_9              9
#define MCS_INDEX_16QAM_10            10
#define MCS_INDEX_16QAM_11            11
#define MCS_INDEX_16QAM_12            12
#define MCS_INDEX_16QAM_13            13
#define MCS_INDEX_16QAM_14            14
#define MCS_INDEX_16QAM_15            15
#define MCS_INDEX_16QAM_16            16
#define MCS_INDEX_64QAM_17            17
#define MCS_INDEX_64QAM_18            18
#define MCS_INDEX_64QAM_19            19
#define MCS_INDEX_64QAM_20            20
#define MCS_INDEX_64QAM_21            21
#define MCS_INDEX_64QAM_22            22
#define MCS_INDEX_64QAM_23            23
#define MCS_INDEX_64QAM_24            24
#define MCS_INDEX_64QAM_25            25
#define MCS_INDEX_64QAM_26            26
#define MCS_INDEX_64QAM_27            27
#define MCS_INDEX_64QAM_28            28

/* SPR 1868 */
/*ULA change */
#define UL_MCS_INDEX_QPSK_0            0
/*ULA change */
#define UL_MCS_INDEX_QPSK_10            10
#define UL_MCS_INDEX_16QAM_11            11
#define UL_MCS_INDEX_16QAM_12            12
#define UL_MCS_INDEX_16QAM_13            13
#define UL_MCS_INDEX_16QAM_14            14
#define UL_MCS_INDEX_16QAM_15            15
#define UL_MCS_INDEX_16QAM_16            16
#define UL_MCS_INDEX_16QAM_17            17
#define UL_MCS_INDEX_16QAM_18            18
#define UL_MCS_INDEX_16QAM_19            19
#define UL_MCS_INDEX_16QAM_20            20
#define UL_MCS_INDEX_64QAM_21            21
#define UL_MCS_INDEX_64QAM_22            22
#define UL_MCS_INDEX_64QAM_23            23
#define UL_MCS_INDEX_64QAM_24            24
#define UL_MCS_INDEX_64QAM_25            25
#define UL_MCS_INDEX_64QAM_26            26
#define UL_MCS_INDEX_64QAM_27            27
#define UL_MCS_INDEX_64QAM_28            28


/* SPR 1868 */

#define MCS_TA                        0
#define RB_TA                         1
/*The following macros are used for createMemPool*/
#define VALUES_PER_RNTI               4

#define MAX_NUM_DEMUX_THREADS  1

#define NUM_OF_LCG    4
#define LCGID_INVALID_VALUE   5

#define INVALID_RLC_MODE   255
#define INVALID_LCID    255
#define INVALID_SEQ_NUM 65535
#define INVALID_SOS_START 65535
#define INVALID_SOS_END 65535
#define HARQ_FAILURE_REPORTING_OFF 0
#define MAC_UL_CCCH_LCH     0
#define MAC_LCID_1          1
#define MAC_LCID_2          2
#define MAC_LCID_3          3
#define MAC_LCID_4          4
#define MAC_LCID_5          5
#define MAC_LCID_6          6
#define MAC_LCID_7          7
#define MAC_LCID_8          8
#define MAC_LCID_9          9
#define MAC_LCID_10         10
#define MAC_LCID_11         11
#define MAC_LCID_12         12
#define MAC_LCID_13         13
#define MAC_LCID_14         14
#define MAC_LCID_15         15
#define MAC_LCID_16         16
#define MAC_LCID_17         17
#define MAC_LCID_18         18
#define MAC_LCID_19         19
#define MAC_LCID_20         20
#define MAC_LCID_21         21
#define MAC_LCID_22         22
#define MAC_LCID_23         23
#define MAC_LCID_24         24
#define MAC_LCID_25         25
#define MAC_POWER_HEAD_LCH  26
#define MAC_CRNTI_LCH       27
#define MAC_TRUNCATED_BSR   28
#define MAC_S_BSR_LCH       29
#define MAC_L_BSR_LCH       30
#define MAC_PADDING_LCH     31
#define MAC_TA_LCH          29
#define MAC_DRX_LCH         30
#define MAC_SCELL_LCH       27
#define UE_CONTENTION_RESOLUTION_IDENTITY_LCH 28

/* TTIB_Code Start */
#ifdef FDD_CONFIG
#define MAX_PDCCH_CONTAINER 6
#else
#define MAX_PDCCH_CONTAINER 3
#endif
/* TTIB_Code End */

/* TDD Config 0 Changes Start */
#define MAX_ULSCH_UCI_CONTAINER 4
/* TDD Config 0 Changes Start */


#define MAX_PDCCH_ORDERS_PER_TTI 8

      /* SPR 4692 Fix */
#define MAC_RACH_PDCCH_DELAY 1
      /* SPR 4692 Fix */

/* SPR 5599 changes start (ZIP ID 129080) */            
/* Avoid redefining TRUE\FALSE several times */
#ifndef TRUE
#define TRUE                1
#endif
#ifndef FALSE
#define FALSE               0
#endif
/* SPR 5599 changes end (ZIP ID 129080) */            
#define ONE                 1
#define ZERO                0
#define NUM_OF_BITS_IN_32_BIT_WORD 32

#define MAX_NUM_CCE_AGGR_LVL_ONE_UE_SPACE 6
#define MAX_NUM_CCE_AGGR_LVL_TWO_UE_SPACE 12
#define MAX_NUM_CCE_AGGR_LVL_FOUR_UE_SPACE 8
#define MAX_NUM_CCE_AGGR_LVL_EIGHT_UE_SPACE 16

#define MAX_NUM_CCE_AGGR_LVL_FOUR_COMMON_SPACE 16
#define MAX_NUM_CCE_AGGR_LVL_EIGHT_COMMON_SPACE 16
#define MAX_NUM_CCE_FOR_COMMON_SPACE 16

#define QUEUE_LOAD_ZERO               0
#define MAX_RETX_COUNT_FOR_STRICT_SUBBAND  2
                                                                             
#define MODULO_TWO(x) ((x) & 1)
#define MODULO_FOUR(x) ((x) & 3)
#define MODULO_EIGHT(x) ((x) & 7)
#define MODULO_SIXTEEN(x) ((x) & 15)
#define MODULO_ONEZEROTWOFOUR(x) ((x) & 1023)
/* SPR 23811 fix start */
#define MODULO_ONEZEROTWOFOURZERO(x) ((x) % 10240)
/* SPR 23811 fix end */
/* SPR 18122 Changes Start*/
/*
#define DIVIDE_BY_TWO(x) ((x) >> 1)
#define DIVIDE_BY_FOUR(x) ((x) >> 2)
#define DIVIDE_BY_EIGHT(x) ((x) >> 3)
#define DIVIDE_BY_SIXTEEN(x) ((x) >> 4)
*/
/* SPR 18122 Changes End*/
                                                                             
#define NG_0_MAPPED_TO_ONE_BY_SIX ((UDouble32)1 / 6)
#define NG_1_MAPPED_TO_ONE_BY_TWO ((UDouble32)1 / 2) 
#define NG_2_MAPPED_TO_ONE  1
#define NG_3_MAPPED_TO_TWO 2

#define MAC_SEND_ERROR -1
#define MAC_RECV_ERROR -1 

#define BITMASK_0    0x1 
#define BITMASK_1    0x2 
#define BITMASK_2    0x4 
#define BITMASK_3    0x8 
#define BITMASK_4    0x10 
#define BITMASK_5    0x20 
#define BITMASK_6    0x40 
#define BITMASK_7    0x80 
#define BITMASK_8    0x100 
#define BITMASK_9    0x200 

#define RESET_BITMASK_0  0x03fe 
#define RESET_BITMASK_1  0x03fd
#define RESET_BITMASK_2  0x03fb
#define RESET_BITMASK_3  0x03f7
#define RESET_BITMASK_4  0x03ef
#define RESET_BITMASK_5  0x03df
#define RESET_BITMASK_6  0x03bf
#define RESET_BITMASK_7  0x037f
/*TDD config fix  start*/
#define RESET_BITMASK_8  0x02ff
#define RESET_BITMASK_9  0x01ff
/* TDD config fix end */

/* This should be removed */
#define MAC_MIN_COUNT_OF_CONTENTION_BASED_RA_PREAMBLES 4
#define MAC_MAX_COUNT_OF_CONTENTION_BASED_GROUPA_PREAMBLES 60
#define MAC_MAX_NUM_OF_RA_PREAMBLES 64                                                                             
#define INVALID_PREAMBLE_IDX MAC_MAX_NUM_OF_RA_PREAMBLES
                                                                             
#define SPLIT_ZERO_BUFFER_COPY(org_p, dst_p, splitPos)\
{\
    dst_p = msgAlloc(PNULL, 0, 0, 0);\
    msgSplit((void *)org_p, splitPos, (void *)dst_p);\
}
#define INVALID_ALREADY_CONSIDERED_FLAG  255
#define GAP_VALUE_FOR_RA_DISTRIBUTED 1

/*SPR 1437 Start*/
/* SPR 2240 Fix Begins*/
/* SPR 19309 + */
#define MIN_POSSIBLE_CFI_VALUE 1
/* SPR 19309 - */
/* SPR 5384 changes start */
#define MAX_POSSIBLE_CFI_VALUE 4
/* SPR 5384 changes end */
/*SPR 2240 Fix Ends*/
/*SPR 1437 End*/
/* SPR 1467 changes start */       
/*Dynamic PDCCH + 5117 Changes Start*/
/*SPR_10093_FIX_START */
/*SPR_17742_FIX_START */
/*SPR_17742_FIX_END */
/*SPR_10093_FIX_END */
/*Dynamic PDCCH + 5117 Changes End*/
/* SPR 1467 changes end */        
#define MAX_TRANSMISSION_COUNT 8
#define ZERO_DRX_DELAY 0

/*Dynamic PDCCH + 5117 changes starts */
#define MAX_NUM_OF_RB 100
/* SPR 6136 start */
#define MAX_SFN_INDEX 3
/* SPR 6136 end */
#define MAX_MCS_INDEX 29
#define MAX_MOD_SCHEME 3
#define MAX_CFI_VALUE 4 
#define CFI_ONE 1
/*Dynamic PDCCH + 5117 changes ends */

/* + CQI_4.1 */
#define DL_64QAM_HIGHEST_CQI_INDEX  15
#define DL_64QAM_LOWEST_CQI_INDEX   10 
#define DL_16QAM_HIGHEST_CQI_INDEX   9
#define DL_16QAM_LOWEST_CQI_INDEX    7
#define DL_QPSK_HIGHEST_CQI_INDEX    6
#define DL_QPSK_LOWEST_CQI_INDEX     1
/* - CQI_4.1 */

/* +DYNAMIC_ICIC */
#define PHR_GUARD_TIMER 5
/* -DYNAMIC_ICIC */
/* ICIC changes start */
#define MAX_USER_LOCATION 2

/*SPR 8451 Changes Start*/
#ifdef ULCODERATE_PLUGIN
/*In Normal UL: CFI=2, With SRS in UL: CFI=3*/
#define UL_DEFAULT_CFI     2
#define UL_EXTENDED_CFI    3
#define MAX_UL_CFI_VALUE   3
/*#define IS_64QAM_ON_ULCODERATE 1*/
#define ULCODERATE_MOD_INDEX 3
#define ULCODERATE_MAX_MCS_INDEX 10
#define UL_CODERATE      0.93
#endif
/*SPR 8451 Changes End*/


 /* response to resource allocation req*/
typedef struct AllocatedResInfoT{
    UInt8   allocatedLength;
    UInt8   allocatedIndex;
    /* SPR 1786 changes start */
    UInt16   allocatedRiv;
    /* SPR 1786 changes end */
    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    /* This parameter is combinatorial index for set1 and set2 to be
       sent over the interface to Layer1. Set1 has s0 and s1-1 and set2 s2 and s3-1 */
    UInt32 rbCodingForPUSCHRAT1;
    UInt16 startRbgIndexforSet1; /* RBG corresponding to s0 */
    UInt16 endRbgIndexforSet1; /* RBG corresponding to s1 */
    UInt16 startRbgIndexforSet2; /* RBG corresponding to s2 */
    UInt16 endRbgIndexforSet2; /* RBG corresponding to s3 */
    UInt16 totalRBsAllocatedForPUSCHRAT1; /* Total Number of RBs allocated */
    UInt8 RAT1Allocation; /* TRUE/FALSE */
#endif
    /* SPR 21958 PUSCH RAT1 Support End */
}AllocatedResInfo;

typedef struct BufferPoolT
{
        UInt8 totalRBs;
            UInt8 cellEdgeRBs;
}BufferPool;

/* ICIC changes end */

/*UL_MU_MIMO_CHG_START*/
/*For three UL modulation schemes, namely, QPSK, 16QAM and 64QAM*/
#define MAC_MAX_NUM_MOD_SCHEME  3

/*UL_MU_MIMO_CHG_END*/


#ifdef TDD_CONFIG
typedef enum subframeT
{
    SUB_FRAME_0 = 0,
    SUB_FRAME_1,
    SUB_FRAME_2,
    SUB_FRAME_3,
    SUB_FRAME_4,
    SUB_FRAME_5,
    SUB_FRAME_6,
    SUB_FRAME_7,
    SUB_FRAME_8,
    SUB_FRAME_9
}subframe;

#endif

/* Transport channels used by the physical layer*/
typedef enum TransportChannelT
{
    DLSCH = 0,
    BCH,
    PCH,
    ULSCH
} TransportChannel;

/*coverity-530 CID-32309*/
    
typedef enum BoolRetTypeT
{
    RET_FALSE = 0,
    RET_TRUE
}BoolRetType;
    /*coverity-530 CID-32309*/
/* Structure for defining the MsgType*/
typedef enum UplinkMsgTypeT
{
    NACK = 0,
    ACK,
    DATA,
    CONTENTION_ACK
}UplinkMsgType;
/* + SPS_TDD_Changes Start */
//From PHY we receive value as 2 for Nack
#define FAPI_NACK 2
/* - SPS_TDD_Changes */
/*Boolean enum : To return values true or false */
typedef enum MacRetTypeT
{
	MAC_FAILURE,
	MAC_SUCCESS,
	MAC_PARTIAL_SUCCESS
	,MAC_REQ_MSG_IN_USE
	/* Remove sleep MAC RESET changes start*/	
	,MAC_PENDING_RESET = 6
	/* Remove sleep MAC RESET changes end*/	
//Coverity_36234 fixed
/* TTIB_Code Start */
#ifdef FDD_CONFIG
	,MAC_CHECK_NEXT_TICK = 7
#endif
/* TTIB_Code End*/
#ifdef DL_DATA_SEPARATION
    , MAC_NULL_ENCODER_NODE_RECVD,
    MAC_UE_PENDING_FOR_DELETION,
    MAC_NULL_UE_CONTEXT_FOUND,
    MAC_TICK_MISMATCH_ERROR
#endif
    /*--coverity-530-54311*/
    ,MAC_SYNTAX_ERROR_VAL =12     
    /*--coverity-530-54311*/
#ifdef DL_DATA_SEPARATION
    ,MAC_ENCODING_FAIL
#endif
    /* + coverity 32236 */
    ,MAC_LOG_CH_ERROR_VAL = 30
    /* - coverity 32236 */
#ifdef LTE_EMBMS_SUPPORTED
    ,MAC_NO_AREA_CHANGE =31 
#endif
}MacRetType;

/* Cell Start Stop Chg */

/* MAC Cell State */
typedef enum MacCellStateTypeT
{
    MAC_IDLE,
    MAC_CONFIG_WIHOUT_EXP_START,
    MAC_CONFIG_WITH_EXP_START,
    MAC_RUNNING,
}MacCellStateType;
/* Cell Start Stop Chg */

/*Modulation Scheme */
typedef enum ModulationSchemeT
{
    /* SPR 2063 start */
    INVALID_MOD_SCHEME = 0,
    /* SPR 2063 end */
    QPSK = 2,
    SIXTEENQAM = 4,
    SIXFOURQAM = 6
} ModulationScheme;

typedef enum enumDataTypeT
{
    RETRANSMISSION = 0, 
    NEW_TRANSMISSION,
    RANDOM_ACCESS_RESPONSE,
    TA,
    NEW_RETX,
    INVALID_DATA_TYPE
}enumDataType;


typedef enum TbValueT
{
    TB_ONE,
    TB_TWO,
    MAX_NUM_TB
}TbValue;

typedef enum DCIFormatT
{
    DCI_FORMAT_0 = 0,
    DCI_FORMAT_1, 
    DCI_FORMAT_1A,
    DCI_FORMAT_1B,
    DCI_FORMAT_1C,
    DCI_FORMAT_1D, 
    DCI_FORMAT_2,
    DCI_FORMAT_2A,
    /* +- SPR 18481 */
    DCI_FORMAT_2B,
    DCI_FORMAT_2C,
    DCI_FORMAT_3, 
    DCI_FORMAT_3A,
    DCI_FORMAT_4,
    DCI_FORMAT_0_MSG3,
    /* +- SPR 18481 */
    MAX_NUM_DCI_FORMAT,
/* Cov_100831_Fix_End */
    /* + Coverity 36210 */
    INVALID_DCI_FORMAT = 255
    /* - Coverity 36210 */
} DCIFormat;


/* _HARQ_PROC_NUM_ */

typedef enum HARQProcNumT
{
    HARQ_PROC_0=0,
    RESERVED_HARQ_PROC=HARQ_PROC_0,
    HARQ_PROC_1,
    HARQ_PROC_2,
    HARQ_PROC_3,
    HARQ_PROC_4,
    HARQ_PROC_5,
    HARQ_PROC_6,
    HARQ_PROC_7,
}HARQProcNum;

/* _RED_VER_ */

typedef enum RedundancyVerValT
{
    RED_VER_0 = 0,
    RED_VER_1,
    RED_VER_2,
    RED_VER_3,
    MAX_NUM_RED_VER
} RedundancyVerVal;

typedef enum MsgTypeT
{
    /*coverity-530 CID-32316*/
    INVALID_MSG = 0,
    /*coverity-530 CID-32316*/
    BCCH_MSG = 1,
    PCCH_MSG,
    RAR_MSG,
    
    /*SPR_3305_FIX_START*/
    CCCH_CONN_SETUP_MSG,
   /* +- SPR 16962 */
    CCCH_OTHER_MSG, /*This is any other CCCH Msg like Reestablishment */
    /*SPR_3305_FIX_END*/

    CCCH_MSG,

    TA_SPATIAL_MUL,
    DRX_SPATIAL_MUL,
    TA_DRX_SPATIAL_MUL,

    TA_NON_SPATIAL_MUL,
    DRX_NON_SPATIAL_MUL,
    TA_DRX_NON_SPATIAL_MUL,

    SPATIAL_MUL,
    NON_SPATIAL_MUL,   /*14*/

    TA_MSG,
    DRX_MSG,
    TA_DRX_MSG,
    INVALID_TA_DRX_MSG,/*18*/
    CONTENTION_RESL_MSG,
    DUMMY_MSG,

    TA_WITH_DATA_SPATIAL_MUL,/*21*/
    DRX_WITH_DATA_SPATIAL_MUL,
    TA_DRX_WITH_DATA_SPATIAL_MUL,

    TA_WITH_DATA_NON_SPATIAL_MUL,/* 24*/
    DRX_WITH_DATA_NON_SPATIAL_MUL,
    TA_DRX_WITH_DATA_NON_SPATIAL_MUL,

    PDCCH_ORDER_MSG,

    /* CLPC_CHG */
    TPC_WITHOUT_DATA,/*28*/
    TPC_WITH_DATA,
    TPC_MSG,
    /* CLPC_CHG END */
    /* SPS_CHG_START */
 /*!
  * \code
  * Add Message Type for SPS DCI
  * This will be used by the Transmitter for sending DCI with correct value.
  * typedef enum MsgTypeT
  * {
  *     ...
  *     SPS_ACTIVATION,
  *     SPS_OCCASION,
  *     SPS_DEACTIVATION,
  *     SPS_REACTIVATION,
  * }
  * \endcode
  */
    SPS_ACTIVATION,   /* 31 */
    SPS_OCCASION,
    SPS_DEACTIVATION,
    SPS_REACTIVATION,
    /*SPS_CHG */
    SPS_RETX,
    /*SPS_CHG */
    /* SPS_CHG_END */

/* SPR 3608 Changes Start */
    HIGH_PRIORITY_DATA,
/* SPR 3608 Changes End */

    SCELL_ACT_DEACT_MSG,/*37*/
    INVALID_SCELL_ACT_DEACT_MSG,

    TA_SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL, /*39*/
    SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL,    
    TA_SCELL_ACT_DEACT_NON_SPATIAL_MUL,           
    SCELL_ACT_DEACT_NON_SPATIAL_MUL,              
    /* + SPR 12118 Changes */
    SCELL_ACT_DEACT_SPATIAL_MUL, /*43*/
    /* - SPR 12118 Changes */
    TA_SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL,     
#ifdef LTE_EMBMS_SUPPORTED
    ,EMBMS_DCI1C_NOTIFICATION
#endif
    SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL,
  /* + SPR 22352 fix */    
    TA_SCELL_ACT_DEACT_MSG,/*46*/
    DRX_SCELL_ACT_DEACT_MSG,
    TA_DRX_SCELL_ACT_DEACT_MSG,

    TA_SCELL_ACT_DEACT_SPATIAL_MUL,/*49*/
    DRX_SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL,
    DRX_SCELL_ACT_DEACT_NON_SPATIAL_MUL,           
    DRX_SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL,
    DRX_SCELL_ACT_DEACT_SPATIAL_MUL,           

    TA_DRX_SCELL_ACT_DEACT_WITH_DATA_NON_SPATIAL_MUL,/*54*/
    TA_DRX_SCELL_ACT_DEACT_NON_SPATIAL_MUL,           
    TA_DRX_SCELL_ACT_DEACT_WITH_DATA_SPATIAL_MUL,
    TA_DRX_SCELL_ACT_DEACT_SPATIAL_MUL           
    /* - SPR 22352 fix */    

}MsgType;




typedef enum TransmissionTypeT
{
    RE_TX = 0,
    NEW_TX,
    ADAPTIVE_RE_TX,
    NON_ADAPTIVE_RE_TX,
    CCCH_TX,
    INVALID_TX
}TransmissionType;

typedef  enum RATPolicyT
{
    RA_POLICY_0,
    RA_POLICY_1,
    RA_POLICY_2_LOCALIZED,
    RA_POLICY_2_DISTRIBUTED
}RATPolicy;

typedef enum TransmissonModeT
{
/* SPR 545 Fix starts */
    TX_MODE_ATTACH_NOT_COMPLETE = 0,
/* SPR 545 Fix ends */
    TX_MODE_1 = 1,
    TX_MODE_2,
    TX_MODE_3,
    TX_MODE_4,
    TX_MODE_5,
    TX_MODE_6,
    TX_MODE_7,
    /* + TM7_8 Changes Start */
    TX_MODE_8,
    /* - TM7_8 Changes End */
    INVALID_TX_MODE
}TransmissonMode;

/* ULA_CHG */
typedef enum UECategoryT
{
    UE_CATEGORY_1 = 1,
    UE_CATEGORY_2, /*2*/
    UE_CATEGORY_3, /*3*/
    UE_CATEGORY_4, /*4*/
    UE_CATEGORY_5, /*5*/
    UE_CATEGORY_6, /*6*/
    UE_CATEGORY_7, /*7*/
    UE_CATEGORY_8, /*8*/
    INVALID_UE_CATEGORY = 0xFF
}UECategory;
/* ULA_CHG */


typedef enum DrxTrggerT
{
        DRX_OFF,
        DRX_ON
}DrxTrigger;


/* Moved from multiplexer to here
 * Since it is used in DL Strategy 
 */
typedef enum RlcModesT
{
    RLC_TM_MODE,          
    RLC_UM_MODE,
    RLC_AM_MODE
}RlcModes;

/* Enumeration for Different bandwidth supported */
typedef enum  DlBandwidthT
{
    DL_BW_1DOT4_MHZ = 0,
    DL_BW_3_MHZ,
    DL_BW_5_MHZ,
    DL_BW_10_MHZ,
    DL_BW_15_MHZ,
    DL_BW_20_MHZ,
    MAX_DL_BW
}DlBandwidth;


/* Enumeration for MAX RBs supported */
typedef enum DlResBlksT
{
    DL_RES_BLKS_6RBS = 6,
    DL_RES_BLKS_15RBS = 15,
    DL_RES_BLKS_25RBS = 25,
    DL_RES_BLKS_50RBS = 50,
    DL_RES_BLKS_75RBS = 75,
    DL_RES_BLKS_100RBS = 100
}DlResBlks;
/* RX_IND_PORTING Changes */
typedef struct ReceivePhyDataQueueNodeT
{
   LTE_SNODE nodeAnchor;
   UInt8 * recvDataBuffer_p;
   SInt32 byteRead;

#ifdef TDD_CONFIG    
   SInt32 byteReadControl;
#endif   
}ReceivePhyDataQueueNode;
/* RX_IND_PORTING Changes */
/* + TM7_8 Changes Start */
typedef enum BeamFormingAlgoT
{
    SINR_BASED_BF_ALGO = 0,
    AOA_BASED_BF_ALGO,
    BF_ALGO_NOT_AVAILABLE = 255
}BeamFormingAlgo;

/* - TM7_8 Changes Start */


/*
typedef struct ackNackStatusInfoT
{
    NACK_RECEIVED = 0,
    ACK_RECEIVED,
    NO_INFO_ABOUT_ACK_OR_NACK,
    REUSED_AGAINST_ACK,
    NACK_RECEIVED_AND_RETRANSMISSION_COMPLETE,
    ALREADY_RESOURCES_ALLOCATED
}ackNackStatusInfo;
*/
/* SPR 22352 fix */
#define TA_BIT      0x01
#define DRX_BIT     0x02
#define SCELL_ACT_BIT 0x04
#define SCELL_DEACT_BIT 0x08
#define RESET_SCELL_ID_BIT    0x0F 
/* SPR 22352 fix */

/* SPR# 891 Fix start */
#define MIN_DCI_HARQ_PROCESS_NUM 0
#define MAX_DCI_HARQ_PROCESS_NUM 7
/* SPR# 891 Fix End */
/* SPR# 893 Fix start */
#define MIN_DLSCH_NUM_LAYER 1
/* SPR# 893 Fix End */

/* SPR# 903 Fix start */
#define MIN_DLSCH_UE_CATEGORY_CAPACITY  0
#define MAX_DLSCH_UE_CATEGORY_CAPACITY 5
/* SPR# 903 Fix End */

#ifdef FDD_CONFIG
#define MAX_NUM_OF_TICK_MISSED 8
#elif TDD_CONFIG
#define MAX_NUM_OF_TICK_MISSED 10
#endif

#define MAC_ROHC_PROFILE                             8
#define MAC_MIN_INTERPKT_DELAY_FOR_SILENCE_DETECTION 1
#define MAC_MAX_INTERPKT_DELAY_FOR_SILENCE_DETECTION 10
#define MAC_DEFAULT_INTERPKT_DELAY_FOR_SILENCE_DETECTION 2
#define MAC_MAX_ENABLE_SILENCE_DETECTION             1
#define MAC_MAX_PDCP_PDU_SIZE_IPV4_ROHC_PROFILE_0    100
#define MAC_MIN_PDCP_PDU_SIZE_IPV4_ROHC_PROFILE_0    1
#define MAC_MAX_PDCP_PDU_SIZE_IPV4_ROHC_PROFILE_1    100
#define MAC_MIN_PDCP_PDU_SIZE_IPV4_ROHC_PROFILE_1    1
#define MAC_MAX_PDCP_PDU_SIZE_IPV4_ROHC_PROFILE_2    100
#define MAC_MIN_PDCP_PDU_SIZE_IPV4_ROHC_PROFILE_2    1
#define MAC_MAX_PDCP_PDU_SIZE_IPV4_NO_ROHC_PROFILE   100
#define MAC_MIN_PDCP_PDU_SIZE_IPV4_NO_ROHC_PROFILE   1
#define MAC_MAX_PDCP_PDU_SIZE_IPV6_ROHC_PROFILE_0    100
#define MAC_MIN_PDCP_PDU_SIZE_IPV6_ROHC_PROFILE_0    1
#define MAC_MAX_PDCP_PDU_SIZE_IPV6_ROHC_PROFILE_1    100
#define MAC_MIN_PDCP_PDU_SIZE_IPV6_ROHC_PROFILE_1    1
#define MAC_MAX_PDCP_PDU_SIZE_IPV6_ROHC_PROFILE_2    100
#define MAC_MIN_PDCP_PDU_SIZE_IPV6_ROHC_PROFILE_2    1
#define MAC_MAX_PDCP_PDU_SIZE_IPV6_NO_ROHC_PROFILE   100
#define MAC_MIN_PDCP_PDU_SIZE_IPV6_NO_ROHC_PROFILE   1
#define MAC_MIN_SPS_DCI_ATTEMPT                      1                   
#define MAC_MAX_SPS_DCI_ATTEMPT                      8
#define MAC_DEFAULT_SPS_DCI_ATTEMPT                  4
#define MAC_MIN_SPS_BLER_THRES                       1
#define MAC_MAX_SPS_BLER_THRES                       30
#define MAC_DEFAULT_SPS_BLER_THRES                   10
#define MAC_MIN_DL_MIN_MCS_SPS                       0
#define MAC_MAX_DL_MIN_MCS_SPS                       15
#define MAC_DEFAULT_DL_MIN_MCS_SPS                   6
#define MIN_AUDIT_INTERVAL                           0
#define MAX_AUDIT_INTERVAL                           20
#define MAC_DEFAULT_AUDIT_INTERVAL                   5
#define SPS_MIN_RES_ALLOC_TTI_ADV                    1
#define SPS_MAX_RES_ALLOC_TTI_ADV                    5
#define SPS_DEFAULT_RES_ALLOC_TTI_ADV                2
#define MAC_MIN_DL_MIN_MCS_MARGIN                    0
#define MAC_MAX_DL_MIN_MCS_MARGIN                    8
#define MAC_DEFAULT_DL_MIN_MCS_MARGIN                4
#define MAC_MIN_DL_TBSIZE_MARGIN_SPS                 0
#define MAC_MAX_DL_TBSIZE_MARGIN_SPS                 20
#define MAC_DEFAULT_DL_TBSIZE_MARGIN_SPS             4
#define MAC_MIN_RB_SAVED_REACTIVATION_THRES          0
#define MAC_MAX_RB_SAVED_REACTIVATION_THRES          10
#define MAC_DEFAULT_RB_SAVED_REACTIVATION_THRES      2
#define MAC_MIN_RLC_SDU_THRES_FOR_DRA                2
#define MAC_MAX_RLC_SDU_THRES_FOR_DRA                10
#define MAC_DEFAULT_RLC_SDU_THRES_FOR_DRA            4
#define MAC_MIN_UL_MIN_MCS_MARGIN                    0
#define MAC_MAX_UL_MIN_MCS_MARGIN                    8
#define MAC_DEFAULT_UL_MIN_MCS_MARGIN                4
#define MAC_MIN_UL_MIN_MCS_SPS                       0
#define MAC_MAX_UL_MIN_MCS_SPS                       15
#define MAC_DEFAULT_UL_MIN_MCS_SPS                   6
#define MAC_MIN_UL_TBSIZE_MARGIN_SPS                 0
#define MAC_MAX_UL_TBSIZE_MARGIN_SPS                 20
#define MAC_DEFAULT_UL_TBSIZE_MARGIN_SPS             4
/* ++ SPR 19287 */
#define NUM_OF_MEASGAP_TICKS_FOR_DCI0                4
/* -- SPR 19287 */
/* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
#define MAX_RBG_INDEX 		            		    25
#define NUM_OF_RBG_BW1DOT4                          6
#define NUM_OF_RBG_BW3                              8
#define NUM_OF_RBG_BW5                              13
#define NUM_OF_RBG_BW10                             17
#define NUM_OF_RBG_BW15                             19
#define NUM_OF_RBG_BW20                             25
#define MAX_RBG_SUBSET_RAT1			                4
#define RBG_SIZE_FOR_BW1DOT4                        1
#define RBG_SIZE_FOR_BW3                            2
#define RBG_SIZE_FOR_BW5                            2
#define RBG_SIZE_FOR_BW10                           3
#define RBG_SIZE_FOR_BW15                           4
#define RBG_SIZE_FOR_BW20                           4

#define NUM_SUBBAND_INDICES		            	    4
#define BITMASK_27BITS 				                0xF8000000
#define REFRESHBITMASK_27BITS 			            0x07FFFFFF
#define FIRST8  				                    0x000000FF
#define NUMZERO					                    0
#define NUMONE					                    1
#define NUMTHREE				                    3
#define NUMEIGHT				                    8
#define INVALID_RBG_INDEX			                255
#define SHIFT27BITS				                    27
#define RAT_1_NOT_SUPPORTED                         0 

typedef struct RBGSetT
{
   UInt16 startRBG;
   UInt16 size;
}RBGSet;
#endif
/* SPR 21958 PUSCH RAT1 Support End */

typedef enum ackNackStatusT
{
    RECEIVED_NACK,
    RECEIVED_ACK,
    ACK_OR_NACK,
    ALREADY_RESOURCES_ALLOCATED,
    COLLISION_OBSERVED,
    RECEIVED_NACK_RE_TRANS_COMPELETE,
    NON_ADAPTIVE_RE_TRANS,
    NEW_ALLOCATION,
    MEAS_GAP_RE_TX_NO_CCE_NO_GRANT, /* MEAS_GAP_CHG */
    UNSET_FLAG,
    /*SPR_3305_FIX_START*/
    NON_ADAPTIVE_NO_CCE_FOR_MSG3,
        /*SPR_3305_FIX_END*/
    /* SPR 5798 Fix Start */
    MAX_UE_SCHEDULED_NO_RETX_NO_GRANT,
    MAX_UE_SCHEDULED_NO_RETX_NO_GRANT_FOR_MSG3,
    /* SPR 5798 Fix End */
    /* FORCED_ACK SPR#9374 Changes Start */
    ADAPTIVE_RETRANS_NO_CCE
    /* FORCED_ACK SPR#9374 Changes End */
}ackNackStatus;

typedef enum AntennaConfT
{
    ONE_PORT_CONF = 1,
    TWO_PORT_CONF = 2,
    FOUR_PORT_CONF = 4
}AntennaConf;

typedef enum UpdateT
{
    DO_NOT_UPDATE = 0,
    UPDATE
}Update;


typedef struct TBInfoT
{
    UInt16 tbSize;
    UInt8  modulationType;
    UInt8 redundancyVer;
    UInt8 txIndicator;
}TBInfo;




typedef struct DCIPduInfoT
{
    UInt32 resrcAssgmntBitMap;
    TBInfo tb1Info;
    TBInfo tb2Info;
    /* Used in virtual resource block distribution
    Valid for DCI formats: 1B,1C,1D
    1= NGAP1
    2=  NGAP2   */
    UInt8 nGAP;
/* SPR 941 starts */   
    UInt8 isTBOneValid;
    UInt8 isTBTwoValid;
/* SPR 941 ends */
    DCIFormat dciFormat;
    UInt16 rnti;
    UInt16 ueIndex;
    UInt8 cceIndex;
    UInt8 aggregationLvl;
    UInt8 numAntennaPort;
    UInt8 resrcAllocType;
    UInt8 transmissionMode;
    UInt8 numTB;
    UInt8 numLayer;
    UInt8 codeBookIndex;
    UInt8 tbSwapFlag;
    UInt8 ueCategory;
    UInt8 pA;
    UInt8 virtulResrcBlkType;
    UInt8 *dataTB_p;
    /* SPR 3679 Changes Start */
    UInt8 *dataTB2_p; 
    /* SPR 3679 Changes End */
    UInt8 tpc;
    UInt8 harqProcessId;
    UInt8 pmiConfirmation;
    UInt8 transmittedPMI;
    UInt8 cqiRequest;  
    UInt8 startRBIndex;
    UInt8 numOfRBs;
    UInt8 nPhichGrp;
    UInt8 nPhichSeq;
    UInt8 nDmrs;
    /* + CQI_5.0 */
    UInt8 transmissionScheme;
    /* - CQI_5.0 */
#ifdef TDD_CONFIG
	UInt8 ulIndex;
    UInt8 dai;
#endif
    /* SPR 1492 changes start*/
    /* This value is used in DLSCH (valid only for SIRNTI or RA-RNTI) and it
     * * should
     * * be match tpc value in dci 1A format*/
    UInt8 nPRB;
    /* SPR 1492 changes end*/
    /*This value is used to send PDCCH Order to UE to trigger contn-free-rach*/
    UInt8 preambleIdx; 
	
    /* Offset to the reference signal power. Value: 0 . 10000, 
     * representing -6 dB to 4 dB in 0.001 dB steps */
    UInt16 txPower;

    /* This stores the bitmap needs to be transmitted in DCI Format 3/3A */
    UInt32 tpcBitmap;
    /* + CQI_5.0 */	
    UInt8 numberOfSubbands;
    /* SPR 18122 Changes Start*/
    UInt8 subbandtoPMIInfo[DIVIDE_BY_TWO(MAX_SUBBANDS)];
    /* SPR 18122 Changes End*/
    /* - CQI_5.0 */	
    /* + TM7_8 Changes Start */
    /* used with DCI format 2B and 2C , valid valus are 0 and 1 */
    UInt8 scramblingId;
    /*Beam Forming vector value for 4 anteenas*/
    UInt16 bfValue[NUM_OF_TX_ANTEENA_VALUE_4];
    /*Ratio of PDSCH EPRE to UE specific Reference signal
     *Valid only when Transmission Mode is 7 and modulation scheme is QPSK */
    SInt8 pdschEpreToUeRsRatio;
    /* - TM7_8 Changes End */
    /* CA_phase2_csi_code Start */
    /* Rel 10 Parameter */
    UInt8 csiFieldSize;
    /* CA_phase2_csi_code End */

    /* SPR 21958 PUSCH RAT1 Support Start */
#ifdef FDD_CONFIG
    /* This parameter is combinatorial index for set1 and set2 */
    /* set1 has s0 and s1-1 and set2 s2 and s3-1 */
    UInt32 rbCodingForPUSCHRAT1;
#endif
    /* SPR 21958 PUSCH RAT1 Support End */
}DCIPduInfo;

typedef  enum ContentionFreePreambleStatusT
{
	PREAMBLE_FREE = 0,
	PREAMBLE_BUSY,
    PREAMBLE_INVALID
}ContentionFreePreambleStatus;


/*This structure contains info when contention free RACH is triggered due to
 * UL Sync Loss or Handover*/
typedef struct UlSyncLossOrHOInfoInDLT
{
    /*State which tells whther UE is in sync loss state or not,
     * UL_SYNC_LOSS_ON=1 and UL_SYNC_LOSS_OFF=0 are the supported values, at
     * initialization it will be set to UL_SYNC_LOSS_OFF */
    UInt8   ulSyncLossState;
    /*Flag indicates whether PDCCH Order is sent for this UE or not when in 
      UL Sync Loss ON State, TRUE means pending to be sent, FALSE means 
      already sent*/
    UInt8 pendingPDCCHOrder;

    /*Preamble Index allocated to UE for contention free RACH. It will be set to
     * INVALID_PREAMBLE_IDX=64 at initialization. This preambleIdx can also have
     * a valid value during HO at target eNodeB*/
    UInt8  preambleIdx;

    ContentionFreePreambleStatus preambleStatus;
}UlSyncLossOrHOInfoInDL;


/*This structure contains info when contention free RACH is triggered due to
 * UL Sync Loss or Handover*/

typedef struct UlSyncLossOrHOInfoInULT
{
    /*State which tells whther UE is in sync loss state or not,
     * UL_SYNC_LOSS_ON=1 and UL_SYNC_LOSS_OFF=0 are the supported values, at
     * initialization it will be set to UL_SYNC_LOSS_OFF */
    UInt8   ulSyncLossState;
    /*Number of times PDCCH Order has already been sent for the UE consecutively
     * without recovering, at initialization it will be set to 0*/
    UInt8   pdcchOrderSentCount;
    /*Preamble Index allocated to UE for contention free RACH. It will be set to
     * INVALID_PREAMBLE_IDX=64 at initialization. This preambleIdx can also have
     * a valid value during HO at target eNodeB*/
    UInt8  preambleIdx;

    /* In case PDCCH Order is triggered with preamble Index 0
     * pointer to timer Node is stored in UL UE Context */
    void *pdcchOrderTimerNode;
} UlSyncLossOrHOInfoInUL;


typedef struct DciCCEInfoT
{
    DCIPduInfo dciPduInfo;
    MsgType msgType;   
    UInt8 cceIndex;
    UInt8 isCCEFree;
    UInt8 msgIdentifier;
}DciCCEInfo;

typedef struct DCICCEInfoCmnChannelT
{
    MsgType msgType;
    UInt8 cceIndex;    
}DCICCEInfoCmnChannel;

typedef struct DciCCEContainerInfoT
{
    /* SPS_CHG_START */
    /*!
     * \code
     *  - Increase DciCCEInfo Range for SPS Occasion Handling.
     *  - Increase arrayCCEAllocated Range for SPS Occasion Handling.
     * \endcode
     * */
    DciCCEInfo dciCCEInfo[MAX_CCE + MAX_SPS_CCE];   
    DCICCEInfoCmnChannel  dciCCEInfoCmnChannel[MAX_DL_COMMON_CHANNEL_MSG];
    UInt8 arrayCCEAllocated[ MAX_CCE + MAX_SPS_CCE];
    UInt8 *mibData_p;
    UInt16 mibDataLen;
    UInt8 countCmnChannelMsg;
    UInt8 countOfCCEAllocated;   
    /* DL DATA SEP Changes */
    UInt8 CFIValueForThisContainer;
    /* + TDD - Reset CCE Changes */
    UInt8 MIValueForThisContainer;
    /* - TDD - Reset CCE Changes */
    /*pdcch for scell changes start*/
    UInt8 arrayExtraCCEAllocated[MAX_DL_UE_SCHEDULED];
    UInt8 countOfExtraCCEAllocated; 
    /*pdcch for scell changes end*/
}DciCCEContainerInfo;
extern DciCCEContainerInfo *dciCCEContainerInfo_gp[MAX_NUM_CELL];

typedef struct CommonSearchSpaceInfoT
{
    /*This is array of UInt8 whose size is equal to number of CCE at 
      Aggregation Level Four and each index stores the CCE Index .For example
      Index 0 --- First CCE Index 
      Index 1----Second CCE Index .. So on.*/
    UInt8 cceAggrLvlFourInfo[ MAX_NUM_CCE_AGGR_LVL_FOUR_UE_SPACE];
    /*This is array of UInt8 whose size is equal to number of CCE at 
      Aggregation Level Eight and each index stores the CCE Index .For example
      Index 0 --- First CCE Index 
      Index 1----Second CCE Index .. So on.*/
    UInt8 cceAggrLvlEightInfo[MAX_NUM_CCE_AGGR_LVL_EIGHT_UE_SPACE ];
}CommonSearchSpaceInfo;

typedef struct UESpecificSearchSpaceInfoT
{
    /*This is array of UInt8 whose size is equal to number of CCE at 
      Aggregation Level One and each index stores the CCE Index .For example
      Index 0 --- First CCE Index 
      Index 1----Second CCE Index .. So on.*/
/*START : DYN_PDCCH*/
    UInt8 cceAggrLvlOneInfo[MAX_CFI_VALUE][MAX_NUM_CCE_AGGR_LVL_ONE_UE_SPACE];/*dpdcchPhase2*/    
/*END   : DYN_PDCCH*/
    /*This is array of UInt8 whose size is equal to number of CCE at 
      Aggregation Level Two and each index stores the CCE Index .For example
      Index 0 --- First CCE Index 
      Index 1----Second CCE Index .. So on.*/
/*START : DYN_PDCCH*/
    UInt8 cceAggrLvlTwoInfo[MAX_CFI_VALUE][MAX_NUM_CCE_AGGR_LVL_TWO_UE_SPACE ];/*dpdcchPhase2*/    
/*END   : DYN_PDCCH*/
    /*This is array of UInt8 whose size is equal to number of CCE at 
      Aggregation Level Four and each index stores the CCE Index .For example
      Index 0 --- First CCE Index 
      Index 1----Second CCE Index .. So on.*/
/*START : DYN_PDCCH*/
    UInt8 cceAggrLvlFourInfo[MAX_CFI_VALUE][MAX_NUM_CCE_AGGR_LVL_FOUR_UE_SPACE];/*dpdcchPhase2*/
/*END   : DYN_PDCCH*/
    /*This is array of UInt8 whose size is equal to number of CCE at 
      Aggregation Level Eight and each index stores the CCE Index .For example
      Index 0 --- First CCE Index 
      Index 1----Second CCE Index .. So on.*/
/*START : DYN_PDCCH*/
    UInt8 cceAggrLvlEightInfo[MAX_CFI_VALUE][MAX_NUM_CCE_AGGR_LVL_EIGHT_UE_SPACE ];/*dpdcchPhase2*/
/*END   : DYN_PDCCH*/
}UESpecificSearchSpaceInfo;

/*This Enum is define for identifying the Node of the UL Grant Request*/

typedef enum ULGrantRequestTypeT
{
    /*When UL Grant is request due to Scheduling Request*/
    SR_ULGRANT_REQ_MSG = 0,
    /*When UL Grant is request due to Contention Resolution*/
    CR_ULGRANT_REQ_MSG,
    /*When UL Grant is request for Aperiodic CQI Reports*/
    CQI_WITH_DATA_ULGRANT_REQ_MSG,
    /*When UL Grant is request for Aperiodic CQI Reports*/
    CQI_WITHOUT_DATA_ULGRANT_REQ_MSG,
    /*New UL Grant */
    NEW_ULGRANT_REQ_MSG,
    /* When UL Grant is request for TPC WITH DATA */
    UL_TPC_WITH_DATA,
    /* When UL Grant is request for TPC WITHOUT DATA */
    UL_TPC_WITHOUT_DATA,
    UL_SPS_ACTIVATION,
    UL_SPS_DEACTIVATION,
    UL_SPS_OCCASION,
    UL_EXPLICIT_PHR_WITHOUT_DATA,
#ifdef FDD_CONFIG
    NEW_TTIB_ULGRANT_REQ_MSG,
    TTIB_UL_SPS_OCCASION,
#endif
    INVALID_GRANT
} ULGrantRequestType;

/*This is generic Node structure of all the Queues meant for UL Grant request
 i.e. globalSrQueue_g,contentionResULGrantQueue_g,pendingULStrategyQueue and 
 aperiodicCQIQueue*/

typedef struct ULGrantRequestQueueNodeT
{
    /*Node Anchor of the Node*/
    LTE_SNODE ulGrantRequestNodeAnchor;
    /*UE Identification in MAC*/
    UInt16     ueIndex;
    /* CA_phase2_csi_code Start */
    /* Arch doc 15.5.8- variable aperiodicCQIToBeRequested  shall
     * be updated bit-wise for each cell.The rightmost bit shall be for PCell of
     * UE because the serving cell index of PCell is .0..The other bits shall be
     * set in order of serving cell index*/
    UInt8 aperiodicCQIToBeRequested;
    /* CA_phase2_csi_code End */
    /*This is time stamp of the message*/
    /* SPR 15909 fix start */
    tickType_t  ttiCounter;
    /* SPR 15909 fix end */
    /*This is a enum having the identification of Node*/
    ULGrantRequestType ulGrantRequestType;
}ULGrantRequestQueueNode;

/*Boolean enum : To inform  about a request is to delete or to add the UE 
 in Non Zero QueueLoad / BSR List*/
typedef enum AddDeleteTriggerT
{
    ADD_TRIGGER,
    DELETE_TRIGGER
}AddDeleteTrigger;

/* DL Qos Changes Start */
typedef enum ActiveUEAddDeleteTriggerT
{
    MEAS_GAP_ON,
    MEAS_GAP_OFF
    /* ++ SPR 19287 */
    /*This is specifically added to SPS MeasGap Map.Generally MeasGap map is filled for actual
     * startOffset minus 4. So when it is require to check if MeasGap and SPS occassion 
     * is overlapping then , those non measGap Map ticks also counts.
     * To cater this issue,below has been added. When MeasGap Map and SPS ocassion map is 
     * compared then overlapping with below enum will not be counted as overlapping*/
    ,MEAS_GAP_ON_FOR_DCI0
    /* -- SPR 19287 */
}ActiveUEAddDeleteTrigger;

typedef enum ULSyncLossTriggerT
{
    SYNC_LOSS_ON,
    SYNC_LOSS_OFF
}ULSyncLossTrigger;

typedef enum TokenUpdateFlagT
{
    DECREMENT_TOKENS =0,
    INCREMENT_TOKENS
}TokenUpdateFlag;



/*
 * This structure is used in the ZeroNonZeroTriggerNode structure
 * To maintain the Add/Delete trigger LC wise rather than UE wise
 */
typedef struct AddDelTrigLCNodeT
{
    UInt8    lcId;
    AddDeleteTrigger addDeleteTrigger;
}AddDelTrigLCNode;

/* This is the queue node for the DRX ON/OFF Trigger queue */

typedef struct MeasGapTriggerQueueNodeT
{
    LTE_SNODE        anchorNode;
    UInt16           ueIndex;
    /*Its is used to differentiate between the Adding or deleting UE*/
    ActiveUEAddDeleteTrigger onOffTrigger;
} MeasGapTriggerQueueNode;

/* ICIC changes start end  */

typedef enum CQIModeT
{
    CQI_MODE_1_0 =10,
    CQI_MODE_1_1=11, 
    CQI_MODE_1_2=12,
    CQI_MODE_2_0=20,
    CQI_MODE_2_1=21,
    CQI_MODE_2_2=22,
    CQI_MODE_3_0=30, 
    CQI_MODE_3_1=31,
    CQI_MODE_INVALID = 255
} CQIMode;

typedef enum HarqTimerNodeValidityT
{
     HARQ_TIMER_NODE_INVALID=0,
     HARQ_TIMER_NODE_VALID
}HarqTimerNodeValidity;
/*ca chngs start*/
typedef enum CATimerNodeValidityT
{
     CA_TIMER_NODE_INVALID=0,
     CA_TIMER_NODE_VALID
}CATimerNodeValidity;
/*ca chngs end*/

#define INVALID_COUNT 255
#define MAC_HEADER_SIZE(nbCtrl,nbLC) (ONE_BYTE * \
                         ((nbCtrl * MAC_CONTROL_ELEMENT_SUBHEADER_SIZE) \
                      + (nbLC * MAC_SDU_SUBHEADER_LEN_THREE_BYTES))) 

#define MAC_HEADER_SIZE_IN_BYTES(nbCtrl,nbLC)\
      (((nbCtrl * MAC_CONTROL_ELEMENT_SUBHEADER_SIZE) \
                 + (nbLC * MAC_SDU_SUBHEADER_LEN_THREE_BYTES)))

typedef enum MeasGapTriggerT
{
    MEASUREMENT_PERIOD_INVALID = 0,
    MEASUREMENT_PERIOD_OFF_DL,
    MEASUREMENT_PERIOD_ON_DL,
    MEASUREMENT_PERIOD_OFF_UL,
    MEASUREMENT_PERIOD_ON_UL
}MeasGapTrigger; 
/* This structure contains all the parameters that are stored in the 
 * Measurement Trigger Node*/
typedef struct MeasGapOnOffTriggerNodeT
{
    LTE_LIST_NODE     anchorNode;
    UInt16            ueIndex;
    MeasGapTrigger    onOffTrigger; 
}MeasGapOnOffTriggerNode;

/* MEAS_GAP_CHG */
typedef struct MeasGapConfigT
{
    UInt8                      gapPatternId;
    UInt8                      gapOffset;
}MeasGapConfig;
/* MEAS_GAP_CHG */
/* DRX_CHG */
/* This enum describes the Current DRX states of a UE */
typedef enum DRXStateT
{
    DRX_STATE_INVALID = 0,
    DRX_STATE_OFF,
    DRX_STATE_ON
}DRXState; 
/* DRX_CHG */

/* SPR 2127 changes start */
#define MAC_STATS_INT_TICK_COUNT 5000
/* SPR 2127 changes end */

/*Enum defined to set state in DL UE Context for UL Sync Loss for a UE and to
 * send a trigger to scheduler to add/delete UE from active list*/
typedef enum UlSyncLossOnOffT
{
    UL_SYNC_LOSS_OFF = 0,
    UL_SYNC_LOSS_ON = 1,
    UL_SYNC_LOSS_INVALID = 2,
}UlSyncLossOnOff;

/*Queue Node for UL Sync Loss Trigger queue to delete/add an entry in active
  UE List for scheduling in UL/DL Scheduler */
typedef struct ULSyncLossQueueNodeT
{
    /*CAUTION: Required circular queue enqueue changes if Anchor node moved down in the structure */
    LTE_SNODE        anchorNode;
    UInt16           ueIndex;
    UlSyncLossOnOff  onOffTrigger;
} ULSyncLossQueueNode;
/* SPS_CHG */
typedef enum RohcProfileE
{
    ROHC_PROFILE0  = 0,
    ROHC_PROFILE1  = 1,
    ROHC_PROFILE2  = 2,
    NO_ROHC_PROFILE = 3,
    MAX_ROHC_PROFILE
}RohcProfile;

typedef struct SpsCommIntervalProfileT
{
    /* When enabled, if SPS occasion is not utilized by SPS UE (due to zero Q
     * load), then the SPS resources can be allocated to other UE in DL only 
     * for given TTI.*/
    UInt8 overrideSpsOccassionForOtherUe;
    /* When enabled, data of only SPS LC can be sent in MAC TB 
       for SPS occasion in DL*/
    UInt8 strictSpsResourceUsageForSpsLc;
    /* When enabled, eNB shall detect silence period and deactivate SPS */
    UInt8 enableSilenceDetection;
    /* Interpacket arrival time of silence packets after which silence 
       period is detected. The unit of parameter is number of SPS interval */
    UInt8 interPktDelayForSilenceDetection;
    /* Inter-packet arrival time of silence packets after which silence period
     * is detected. The unit of parameter is number of SPS interval. */
    /* Max PDCP PDU size per ROHC Profile . The unit is bytes.
     * the index for array will support following values -
     *    0 = RTP Profile
     *    1 = IP Profile
     *    2 = UDP Profile
     *    3-6 RohcProfile
     *    7 = NO ROHC */
    UInt32 maxPdcpPduSizeIpv4[MAC_ROHC_PROFILE];
    /* Max PDCP PDU size per ROHC Profile . The unit is bytes.
     * the index for array will support following values -
     *    0 = RTP Profile
     *    1 = IP Profile
     *    2 = UDP Profile
     *    3-6 ROHC Profile
     *    7 = NO ROHC */
    UInt32 maxPdcpPduSizeIpv6[MAC_ROHC_PROFILE];
    /* If due to MCS improvement or lower packet size, number of RBs that can 
     * be saved is equal to this number than reactivation shall be triggered by
     * SPS strategy */
    UInt8  minRbSavedReactivationThres;
    /* The sps BLER Threshold.If SPS BLER is more than this value then, SPS 
     * should be released */
    UInt8  spsBlerThres;
    /* Duration, in terms of sps interval, after which a configured SPS 
     * resource should be reevaluated for allocation of SPS resouces, which can
     * lead to saving RBs due to improved channel conditions, reduced SDU size
     * or availablity of a TTI where RBs are free on the UEs better subband. 
     * When 0, no audit shall be done */
    UInt8 auditInterval;
    /* Number of TTIs in advance before which SPS resources are allocated and
     * indicated to SPS strategy, so that activation/reactivation can be
     * initiated */
    UInt8 spsResAllocTtiAdv;
}SpsCommIntervalProfile, *LP_SpsCommIntervalProfile;

/* ICIC changes start */

/* Enum defined for User Location Type */
typedef enum UserLocationT
{
    /* Cell Center User */
    CC_USER = 0,

    /* Cell Edge User */
    CE_USER,
    INVALID_USER_LOCATION_TYPE
}UserLocation;

/*UL_MU_MIMO_CHG_START*/

/* EICIC +*/

typedef enum UserTypeT
{
NON_VICTIM_UE = 0,
VICTIM_UE,
INVALID_UE_TYPE
}UserType;

/* EICIC -*/
#define MAX_VALID_UE_TYPE 2

typedef struct ULMUMIMORrmConfigT
{

    /* This parameter signifies that whether the selection of a buddy UE will be
     * random or any other type (however currently Random selection is usable).
     * .    0 - UL MU-MIMO Random Selection is enabled.  Default value of this
     * parameter should be set to 0 (the same will be used if the parameter is not
     * present)
     * .    Other values are for future use
     * */
    UInt32 ulMUMIMOType;
    /* Threshold value of MCS required as an input to consider a UE candidate for
     * Uplink MU-MIMO transmission. Range of the IE is from 0 to 28. Default value
     * should be set to 6
     * */
    UInt32 ulMUMIMOMcsThreshold;
}ULMUMIMORrmConfig;


/*UL_MU_MIMO_CHG_END*/

/* Enum defined for resource region type */
typedef enum ResourceRegionT
{
    /* Combined Cell center and cell edge resources */
    CC_CE_REGION = 0,
    /* SPR 13494 fix start */
    CE_REGION
    /* SPR 13494 fix end */
}ResourceRegion;

/* Enum defined for preferred CE allocation region
 * This enum is applicable only for CE user to decide whether 
 * resources can be given from any region or CE region is preferred over CC  
 */

typedef enum preferredCEAllocRegionT
{
    /* Combined Cell center and cell edge resources */
    CC_CE_ANY_REGION =0,
    CE_REGION_PREFERRED,
}preferredCEAllocRegion;

/* RB is blocked in current ICIC partition */
#define BLOCKED_REGION          0
/* indicates RB is present in cell centre region */
#define CELL_CENTRE_ONLY_REGION 1
/* indicates RB is present in cell edge region */
#define CELL_EDGE_ONLY_REGION 2

/* Cell Edge Priority UE Node */
typedef struct CEUserPriorityQueueNodeT
{
    LTE_SNODE cellEdgePriorityQueueNodeAnchor;
    UInt8 isNodeInvalid;

    /* stores UE ID*/
    UInt16 ueIndex;
}CEUserPriorityQueueNode;
/* ICIC changes end */

/* EICIC +*/
typedef struct pendingVictimPriorityQueueNodeT
{
    LTE_SNODE pendingVictimPriorityQueueNodeAnchor;
    UInt8 isNodeInvalid;

    /* stores UE ID*/
    UInt16 ueIndex;
}pendingVictimPriorityQueueNode;
/* EICIC -*/

typedef struct RRCRachInterfaceNodeT
{
    UInt16 rnti;
    /* +- SPR 18268 */
    UInt16 ueIndex;
    /* +- SPR 18268 */
}RRCRachInterfaceNode;

typedef struct PDCCHRachNodeT
{
    UInt16 rnti;
}PDCCHRachNode;

typedef struct RRCRachHOInterfaceNodeT
{
  UInt32 transactionId;
  UInt16 ueIndex;
}RRCRachHOInterfaceNode;

typedef struct ConnHOEntityFailureInterfaceNodeT
{
    UInt8 preambleIndex;
    /* SPR 14117 fix start */
    UInt16 tcRnti;
    /* SPR 14117 fix end */
}ConnHOEntityFailureInterfaceNode;

typedef struct HORachInfoT
{
    UInt8 preambleIndex;
    UInt8 prachMaskIndex;
    UInt16 tcRNTI; 
    /* SPR 8379 changes start */
    UInt16 resp;
    /* SPR 8379 changes end */
}HORachInfo;

/*SPR 16855 +-*/
/* + CL-MIMO LJA*/
typedef ConnHOEntityFailureInterfaceNode RRCRachDeleteUeHoInterface;
typedef struct RRCRachDeleteUeInterfaceT
{
    UInt16 rnti;
    UInt8 preambleIndex;
}RRCRachDeleteUeInterface;

typedef struct RRCRachChangeCRNTIInterfaceT
{
   UInt16 ueIndex;
   UInt16 oldCrnti;
   UInt16 newCrnti;
}RRCRachChangeCRNTIInterface;

typedef enum codebookSubsetRestrictionTypeT
{
    N2_TX_ANTENNA_TM3=0,
    N4_TX_ANTENNA_TM3=1,
    N2_TX_ANTENNA_TM4=2,
    N4_TX_ANTENNA_TM4=3,
    N2_TX_ANTENNA_TM5=4,
    N4_TX_ANTENNA_TM5=5,
    N2_TX_ANTENNA_TM6=6,
    N4_TX_ANTENNA_TM6=7,
    CBSR_INVALID = 255
} codebookSubsetRestrictionType;

#define MAX_CBSR_SIZE 8

typedef struct codebookSubsetRestrictionInfoT
{
    codebookSubsetRestrictionType cbsrType;
    UInt8 cbsrValue[MAX_CBSR_SIZE];
} codebookSubsetRestrictionInfo;
/* - CL-MIMO LJA*/

/* This strucure contains DRX configurable parameters*/
typedef struct DRXConfigT
{
    UInt8     isDRXShortCycleConfigured;
    UInt8     drxOnDurationTimerIndex;
    UInt8     drxInactivityTimerIndex;
    UInt8     shortDRXCycleIndex;
    UInt8     longDRXCycleIndex;
    UInt8     drxOnDurationTimer;
    UInt8     drxRetransmissionTimer;
    UInt8     drxShortCycleTimer;
    UInt16    drxInactivityTimer;
    UInt16    shortDRXCycle;
    UInt16    longDRXCycle;
    UInt16    drxStartOffset;
    /*START : DRX_MAC_CE*/
    UInt16    drxMacCeTimer;
    UInt16    drxMacCeCmdThreshold;
    UInt8     isdrxMacCeCmdConfigured;
    /*END   : DRX_MAC_CE*/
}DRXConfig;

typedef struct MeasGapContextDLT
{
    MeasGapConfig              measGapConfig;
    /* + SPR_16082 */
    UInt8                      measGapStartOffsetDL[MAX_NUM_MAPS];
    UInt8                      measGapEndOffsetDL[MAX_NUM_MAPS];
    /* - SPR_16082 */
    UInt8                      isMeasGapPeriodOn;
    MeasGapOnOffTriggerNode    *measGapStartNodeDL_p[MAX_NUM_MAPS];
    MeasGapOnOffTriggerNode    *measGapEndNodeDL_p[MAX_NUM_MAPS] ;
}MeasGapContextDL;

/* + Phase2 -  */
typedef struct DLUESCellStartegyNodeT
{
    InternalCellIndex internalCellIndex;
    UInt16            schedulableBytesScell ; // will be used for RB deduction 
    SInt32            schedulableTokensScell; //as per RRM ratio
} DLUESCellStartegyNode;
/* - Phase2 -  */

typedef struct DLAmbrInfoT
{
    /* Array NON-GBR LC Ids */
    UInt8 nGbrLcInfo[MAX_NUMBER_OF_LOGICAL_CHANNEL];
    /* Indicates the NON-GBR LC count */
    UInt8 nGbrLcCount;
    /* Indicates the token priority */
    UInt8 ambrTokenPriority;
    /* Indicates the current index in the
     * scheduled window array */
    UInt8  curTknWindowInd;
    /* Indicates the AMBR */
/* L2_PERFORMANCE_OPT_CHANGES_START */
    UInt32 AMBR;
/* L2_PERFORMANCE_OPT_CHANGES_END */
    /* Indicates whether Qload is present for NON_GBR LC's */
    UInt32 queueLoad;
    /* Indicates the Total tokens scheduled in the
     * scheduled window */
    UInt32  totSchdTknInWindow;
    /* */
    UInt32 curScheduledTkn;
    /* Stores the tokens scheduled in every
     * TOKEN_UPDATE_PERIOD in
     *      * last MAX_TKN_SCHD_WINDOW
     *      periods
     *           */
    UInt32 scheduledToken[MAX_TKN_SCHD_WINDOW];
    /* Indicates the rate at which token
     * will be update every
     *      * TOKEN_UPDATE_PERIOD
     *           */
    UInt32  ambrTokenRate;
    /* Indicates the Tokens
     * Available in bytes */
    SInt32 availAmbrToken;
    /* Indicates the Max Tokens
     * in bytes */
    UInt32 ambrTokenDepth;
    /* DESIGN REVIEW CHANGES
     * INCORPORATION  START
     * */
    /* Indicates the token
     * depth */
    TokenUpdateFlag  eTokenUpdateFlag;
    /* Indicates the
     * token depth */
    /* SPR 11115 Fix Start */
    SInt32  ambrTokenLimit;
    /* SPR 11115 Fix End */
    /* DESIGN REVIEW
     * CHANGES
     * INCORPORATION
     * END */
    /* Indicates the
     * map between
     * the token
     * value and
     * index to
     * priority */
    UInt32  lcTknTable[MAX_TOKEN_PRIORITIES];

}DLAmbrInfo;

/*SPR 16855 +-*/
/*Dynamic PDCCH + 5117 changes starts */
typedef struct McsRBInfoForNonSpatialMultiplexT
{
    UInt8 mcsIndexForRat0;
    UInt8 mcsIndexForRat0WithLastRBG;
    UInt8 mcsIndexForRat1AndRat2;
    UInt8 numberOfresourceBlocksForRat0;
    UInt8 numberOfresourceBlocksForRat0WithLastRBG;
    UInt8 numberOfresourceBlocksForRat1AndRat2;
}McsRBInfoForNonSpatialMultiplex;

typedef struct McsRBInfoForSpatialMultiplexT
{
    UInt8 mcsIndexForTbOneRat0;
    UInt8 mcsIndexForTbOneRat0WithLastRBG;
    UInt8 mcsIndexForTbOneRat1AndRat2;
    UInt8 mcsIndexForTbTwoRat0;
    UInt8 mcsIndexForTbTwoRat0WithLastRBG;
    UInt8 mcsIndexForTbTwoRat1AndRat2;
    UInt8 numberOfresourceBlocksForRat0;
    UInt8 numberOfresourceBlocksForRat0WithLastRBG;
    UInt8 numberOfresourceBlocksForRat1AndRat2;
}McsRBInfoForSpatialMultiplex;

typedef struct McsRBInfoT
{
    McsRBInfoForNonSpatialMultiplex mcsRBTBSIzeInfoForNonSpatialMultiplex[MAX_UE_CATEGORY][MAX_NUM_OF_RB];
    McsRBInfoForSpatialMultiplex mcsRBTBSIzeInfoForSpatialMultiplex[MAX_UE_CATEGORY][MAX_MCS_INDEX][MAX_NUM_OF_RB];
}McsRBInfo;

/*SPR 8451 Changes Start*/
#ifdef ULCODERATE_PLUGIN
/*UL Code rate map*/
typedef struct McsULRBInfoNonSpatialT
{
    UInt8 mcsIndexForUL;
    UInt8 numberOfresourceBlocksForUL;
}McsULRBInfoNonSpatial;

typedef struct McsULRBInfoT
{
    McsULRBInfoNonSpatial mcsULRBInfoNonSpatial[ULCODERATE_MOD_INDEX][MAX_NUM_OF_RB][ULCODERATE_MAX_MCS_INDEX];
}McsULRBInfo;
#endif
/*SPR 8451 Changes End*/

/* 4x4 DL MIMO CHG START */
typedef struct mcsRBInfo4x4
{
    McsRBInfoForNonSpatialMultiplex mcsRBTBSIzeInfoForNonSpatialMultiplex[TB_ON_LAYER_3_OR_4][MCS_27_OR_28][MAX_NUM_OF_RB];
    McsRBInfoForSpatialMultiplex mcsRBTBSIzeInfoForSpatialMultiplex[TB_ON_LAYER_3_OR_4][MCS_27_OR_28][MAX_MCS_INDEX][MAX_NUM_OF_RB];
}mcsRBInfo4x4;
/*Dynamic PDCCH + 5117 changes ends */
/* 4x4 DL MIMO CHG END */
/*Dynamic PDCCH + 5117 changes ends */

/* Structure containing all the configuration parameters needed 
*  by MAC as per configured by RRC. */
/* CA Changes start */

/*Possible Cell states */
typedef enum CellStateT
{
    CELL_INIT                   = 0,
    CELL_WAITING_FOR_PHY_CAPABILITY,
    CELL_PHY_INIT,
    CELL_WAITING_FOR_PHY_CONFIG,
    CELL_PHY_CONFIG,
    CELL_WAIT_FOR_PHY_START,
    CELL_WAITING_FOR_FIRST_SFI,
    CELL_PHY_RECONFIG_RECVD_AFTER_CELL_STOP,
    CELL_PHY_RUNNING_CONFIG_RECVD,
    /* + SPR_10941 */
    CELL_WAITING_FOR_PHY_STOP_AFTER_DELETE_REQ,
    CELL_PHY_STOP,
    CELL_WAITING_FOR_PHY_STOP_AFTER_STOP_REQ, 
    /* - SPR_10941 */
    CELL_PHY_RUNNING,
/*+ SPR 18399*/
/*- SPR 18399*/
    CELL_CTRL_MAX_ST
}CellState;



#define GET_MEMORY_FOR_CELL_INIT_PARAMS(internalCellIndex,isSocketTobeOpened)\
{\
    if (PNULL == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]) \
    {\
         GET_MEM_FROM_POOL(CellAndInitParams , cellSpecificParams_g.\
             cellConfigAndInitParams_p[internalCellIndex], sizeof(CellAndInitParams),PNULL);\
         if (PNULL == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex])  \
             return;\
         \
         GET_MEM_FROM_POOL(InitParams,cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
                      ->initParams_p, sizeof(InitParams),PNULL)\
         if (PNULL == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p)\
         {\
             freeMemPool(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]);\
             cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex] = PNULL;\
             return;\
         }\
         memSet(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p,\
               0, sizeof(InitParams));   \
           \
         GET_MEM_FROM_POOL(CellConfigParams,cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
            ->cellParams_p,sizeof(CellConfigParams),PNULL);\
         if (PNULL == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p)\
         {\
            freeMemPool(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p); \
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p = PNULL;\
            freeMemPool(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]);\
            cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex] = PNULL;\
             return;\
         } \
         memSet(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p\
                     ,0, sizeof(CellConfigParams));\
        \
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellState = CELL_INIT;\
        \
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                    explicitStartRequired = FALSE;\
        cellSpecificParams_g.numOfCells++;\
        \
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]\
           ->cellParams_p->gapValue = 1;\
        /* +- SPR 19066 */ \
        isSocketTobeOpened = TRUE;\
    }\
}
#define FREE_MEMORY_FOR_CELL_INIT_PARAMS(internalCellIndex)\
{\
    if (PNULL != cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]) \
    {\
        freeMemPool(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p); \
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p = PNULL;\
        \
        freeMemPool(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p); \
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p = PNULL; \
        \
        freeMemPool(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]); \
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex] = PNULL;\
        \
        cellSpecificParams_g.numOfCells--;\
        \
    }\
}
#define MAX_PUCCH_AN_CS_LIST 2
#define MAX_PUCCH_AN_CS 4

#define MAX_PRS_MUTING_BITS 16
#define MAX_MIB_FROM_RRC    256

#define MAX_NO_SI_MSG_SGMNTS 64
#define RES_BIT_MAP_ARRAY_SIZE 15
/* Here TBS Index for DCI 1A represent as 
 * 0 - 56 TB Size
 * 1 - 72 
 * 2 - 120
 * 3 - 144
 * 4 - 176
 * 5 - 208
 * 6 - 224
 * 7 - 256
 * 8 - 296
 * 9 - 328
 * 10 - 392
 * 11 - 488
 * 12 - 552
 * 13 - 600
 * 14 - 632
 * 15 - 696
 * 16 - 776
 * 17 - 840
 * 18 - 904
 * 19 - 1000
 * 20 - 1064
 * 21 - 1128
 * 22 - 1288
 * 23 - 1384
 * 24 - 1480
 * 25 - 1608
 * 26 - 1736 */
#define MAX_IBS_INDEX_DCI_1A 27
#define MAX_TBS_INDEX_DCI_1A MAX_IBS_INDEX_DCI_1A


/* Here TBS Index for DCI 1C represent as 
 * 0 -  40 TB Size
 * 1 -  56 
 * 2 -  72
 * 3 -  120
 * 4 -  136
 * 5 -  144
 * 6 -  176
 * 7 -  208
 * 8 -  224
 * 9 -  256
 * 10 - 280
 * 11 - 296 
 * 12 - 328 
 * 13 - 336 
 * 14 - 392
 * 15 - 488
 * 16 - 552
 * 17 - 600
 * 18 - 632
 * 19 - 696
 * 20 - 776
 * 21 - 840
 * 22 - 904
 * 23 - 1000
 * 24 - 1064
 * 25 - 1128
 * 26 - 1224
 * 27 - 1288
 * 28 - 1384
 * 29 - 1480
 * 30 - 1608
 * 31 - 1736
 * */
#define MAX_IBS_INDEX_DCI_1C 32
#define MAX_TBS_INDEX_DCI_1C MAX_IBS_INDEX_DCI_1C

/* SPR 22499 Fix Start */
#ifdef FDD_CONFIG
#define MAX_PHICH_CONTAINER  12
#endif
/* SPR 22499 Fix End */

/*coverity 30422 fix*/
/* SPR 1868 changes start */
#ifdef FDD_CONFIG
#define MAX_PHICH_GRP  25 /* For 100 RB bandwidth */
#else
/* SPR 10732 Fix Start */
/* For TDD Config 0, SF 0 and SF 5 numOfPhichGroup is twice*/
#define MAX_PHICH_GRP  50 /* For 100 RB bandwidth */
/* SPR 10732 Fix End */
#endif

/* SPR 1868 changes end */
#define MAX_DB_REDUCED_THROUGH_TABLE 6
#define MAX_POWER_REDUCTION_FOR_MCS_RB        MAX_DB_REDUCED_THROUGH_TABLE

typedef enum SIMsgTypeT
{
    INVALID_SI_MSG = 0,
    SI_MSG_1,
    SI_MSG_2,
    SI_MSG_3,
    SI_MSG_4,
    SI_MSG_5,
    SI_MSG_6,
    SI_MSG_7,
    SI_MSG_8,
    SI_MSG_9,
    SI_MSG_10,
    SI_MSG_11,
    SI_MSG_12,
    SI_MSG_13,
    SI_MSG_14,
    SI_MSG_15,
    SI_MSG_16,
    SI_MSG_17,
    SI_MSG_18,
    SI_MSG_19,
    SI_MSG_20,
    SI_MSG_21,
    SI_MSG_22,
    SI_MSG_23,
    SI_MSG_24,
    SI_MSG_25,
    SI_MSG_26,
    SI_MSG_27,
    SI_MSG_28,
    SI_MSG_29,
    SI_MSG_30,
    SI_MSG_31,
    SI_MSG_32,
    MAX_SI_MSG = SI_MSG_32
}SIMsgType;

typedef struct TPCRntiRangeT
{
    /*Indicates start value for range of TPC RNTI to be configured by L3*/
    UInt16 startTpcRntiPucch;
    /*Indicates end value for range of TPC RNTI to be configured by L3*/
    UInt16 endTpcRntiPucch;
    /*Indicates start value for range of TPC RNTI to be configured by L3*/
    UInt16 startTpcRntiPusch;
    /*Indicates end value for range of TPC RNTI to be configured by L3*/
    UInt16 endTpcRntiPusch;
}TPCRntiRange;

typedef struct CommonDLPowerControlInfoT
{
    /*Power per antenna of the PCFICH w.r.t reference signal*/
    UInt16 pcfichPowerOffset;
    /*Power per antenna of the PHICH w.r.t reference signal*/
    UInt16 phichPowerOffset;
    /*The configured transmission power for PDCCH*/
    UInt16 pdcchPowerOffset;
    /*Configured transmission power for PBCH*/
    UInt16 pbchTransmissionPower;
    /*Configured transmission power for PCH*/
    UInt16 pchTransmissionPower;
}CommonDLPowerControlInfo;

/*For Power Control*/
typedef struct PDSCHConfigCommonInfoT
{
    /*Indicates the downlink reference-signal transmit 
     * power used to derive downlink reference-signal EPRE*/
    /*CLPC_REWORK_CHG*/
    SInt8 referenceSigPower;
    /*CLPC_REWORK_CHG*/
    /*Indicates the ratio of PDSCH EPRE to cell-specific RS EPRE*/
    UInt8 pB;
}PDSCHConfigCommonInfo;

typedef struct PowerControlEnableInfoT
{
    /*Indicates whether closed loop power control feature based 
     * on HARQ.s BLER received on PUCCH is enabled or not*/
    UInt8 harqBlerClpcPucchEnable;
    /*Indicates whether closed loop power control feature based 
     * on DCI.s SINR received on PUCCH is enabled or not*/
    UInt8 cqiSinrClpcPucchEnable;
    /*Indicates whether closed loop power control feature on PUSCH 
     * is enabled or not*/
    UInt8 clpcPuschEnable;
    /*Indicates whether power control or aggregation  on PDCCH is 
     * enabled or not*/
    UInt8 pdcchPowOrAggregationEnable;
    /*Optional parameter that indicates if closed loop power control 
     * feature is enabled then frequency selective power control is 
     * enabled or not. Absence of this parameter indicates disabled 
     * frequency selective power control feature*/
    UInt8 clpcPuschfreqSelectiveEnable;
}PowerControlEnableInfo;

typedef struct CommonULPowerControlInfoT
{
    /*Indicates a cell specific nominal component for PUSCH*/
    SInt8 p0NominalPUSCH;
    /*A 3-bit cell specific parameter for power control*/
    UInt8 alpha;
}CommonULPowerControlInfo;

/* SRS_CHG */
/* This structure contains parameters that are received from SRS in
 * SoundingRS-UL-ConfigCommon Setup 
 */
typedef struct SRSCommonSetupInfoT
{
    UInt8   srsBandwidthConfig;
    /*SRS common config index*/
    UInt8   srsSubframeConfig;
    /*TRUE / FALSE indicating simultaneous transmission of ackNack & SRS*/
    UInt8   ackNackSRSSimultaneousTrans;
#ifdef TDD_CONFIG
    UInt8   srsMaxUpPts;  
#endif
}SRSCommonSetupInfo;
/* SRS_CHG */

typedef struct ULPRBUsageStatisticT
{
    UDouble64 totalPRBUsage;
/* L2_PERFORMANCE_OPT_CHANGES_START */
    UInt32    cumulativePRBsScheduled;
    UInt32    cumulativePRBsAvailable;
/* L2_PERFORMANCE_OPT_CHANGES_END */
    /* SPR 15909 fix start */
    tickType_t    lastULPRBUsageCalcTTI;
    /* SPR 15909 fix end */
}ULPRBUsageStatistic;

typedef struct SpsCommonCellInfoT
{
    /* startSpsCRnti indicates the starting value for range of SPS CRNTI */
    UInt16 startSpsCRnti;
    
    /* The end value for range of SPS CRNTI */
    UInt16 endSpsCRnti;
    
} *LP_SpsCommonCellInfo, SpsCommonCellInfo;

/* SS_S2 */
/* + PRS_CHANGES */
typedef struct PRSResInfoT
{
    UInt32 rat0PRSBitmap;
    UInt8 startPRB;
    UInt8 endPRB;
    UInt8 prsReserveRBMap[MAX_RES_BLOCKS];
    UInt8 prsAllowedWithICIC;
} PRSResInfo;

typedef struct PRSConfigInfoT
{
#define MAX_PRS_INDEX 2
    UInt8 isPRSConfigured;
    DlBandwidth prsBandwidth;
    UInt8 prsSubframes;
    UInt16 prsTransmissionPower;
    UInt8 prsMutingConfig[MAX_PRS_MUTING_BITS];
    UInt8 prsOccasionCounter;
    UInt8 prsSubframeCounter;
    PRSResInfo prsResInfo[MAX_PRS_INDEX];
    UInt8 icicConfigured; 
    /* SPR 15909 fix start */
    tickType_t expiredPRSSubframeCounterTTI; 
    /* SPR 15909 fix end */
    UInt8 dlPRSMapIndex;
    UInt8 dlPRSRATPoliciesIndex;
}PRSConfigInfo;
/* - PRS_CHANGES */

typedef struct DlUeCategoryInfoT
{
    /* UE Category */
    UInt8 ueCategory;
    /* Maximum number of supported layers for spatial multiplexing in DL */
    UInt16 maxLayersSpaMux;
    /* Maximum number of DL-SCH transport block bits received within a TTI */
    UInt32 maxDLSCHTbBits;
    /* Maximum number of bits of a DL-SCH transport block received within a TTI */
    UInt32 maxBitsDLSCHTb;
    /* Total number of soft channel bits */
    UInt32 totSoftChBits;
} DlUeCategoryInfo;



/*RACH parameters configured from RRC*/
typedef struct RACHConfigurationT
{
    /*Update flag is set when new configuration received from RRC*/
    UInt8  raRspWindowUpdateFlag;

    /*Current active index of raRspWindowSize[] array*/
    UInt8  activeIdxRARspWindow;
    UInt8  raRspWindowSize[2];   /*2, 3, 4, 5, 6, 7, 8, 10 subframes*/

    /*Update flag is set when new configuration received from RRC*/
    UInt8  contentionResUpdateFlag;

    /*Current active index of macContentionResTimer[] array*/
    UInt8  activeIdxContentionRes;
    
    UInt8 countOfContentionBasedRAPreambles;
    
    UInt8 numOfPreamblesInGroupA;
    
    UInt16 messageSizeGroupA;
    
    UInt8 preambleGroupBConfigPresent;
    
    UInt8 mcsIndexForMSG3GroupARACHRequest;
    
    UInt8 numRBReqdForMSG3GroupARACHRequest;
    
    UInt8 mcsIndexForMSG3GroupBRACHRequest;
    
    UInt8 numRBReqdForMSG3GroupBRACHRequest;
    
    UInt8 mcsIndexForConFreeHandOver;

    UInt8 numRBReqdForConFreeHandOver;
    
    UInt8 mcsIndexForConFreePDCCHOrder;
    
    UInt8 numRBReqdForConFreePDCCHOrder;
    
    UInt8 minimumRBReqdForSingleRAR;
    
    UInt8 maxRBReqdForAnyRAR;
    
    UInt8  macContentionResTimer[2]; /*8, 16, 24, 32, 40, 48, 56 or 64 subframes*/
    /*SPR_3305_FIX_START*/
    /*Max Number of Msg3 Transmission*/
    UInt8   maxHarqMsg3Tx;
    /*SPR_3305_FIX_END*/
     UInt8 preambleInitialReceivedTargetPower;
    /* + SPR 17733 */
#ifdef FDD_CONFIG
    /* This parameter is kept for Retransmission of MSG4 */
    UInt8  msg4MaxHarqRetransmission[2];
#endif
    /* - SPR 17733 */
}RACHConfiguration;

/*PRACH parameters configured from RRC*/
typedef struct PRACHConfigurationT
{
    UInt8  prachConfigIndex;      /*0 to 63*/
    UInt8  prachFrequencyOffset;  /*0 to 94*/

}PRACHConfiguration;

typedef struct RACHConfigInfoT
{
    RACHConfiguration  rachConfiguration;
    PRACHConfiguration prachConfiguration[2];

    /*SFN from which new received information should be used*/
    UInt16 sfn;

    /*Update flag is set when new configuration received from RRC*/
    UInt8  prachUpdateFlag;

    /*Current active index of prachConfiguration[] array*/
    UInt8  prachActiveIndex;
}RACHConfigInfo;

typedef struct MIBMsgT
{
    /*Contains the MIB message including DATA_MSG_API_HEADER_LEN*/
    UInt8 *msg_p;
    UInt8 dataLen;

    /*If this flag is TRUE means MIB message present*/
    UInt8 isMIBPresent;


}MIBMsg;

typedef struct MIBMsgInfoT
{
    /*Contains the (2 * MAX_MIB_FROM_RRC) array contains the MIB
     *Information
     */
    MIBMsg mibMsg[2][MAX_MIB_FROM_RRC];
    
    /*It is the index of the currently active mibMsg[] array*/
    UInt8  activeIndex;
    
    /*RRC will set this flag if new set if 256 messages received
     *basend on the update flag activeIndex is modified to use the 
     *new set of MIB messages
     */
    UInt8  updateFlag;
    
    /*This flag is set when first MIB scheduled after the MAC initialization*/
    UInt8  firstMIBsentFlag;
}MIBMsgInfo;

typedef struct SIBType1MsgT
{
    UInt8  *msg_p;
    UInt16 sfn; /*From this SFN new received SIB1 msg should be used*/
    /* SPR 13250 fix start */
    UInt16  msgLen;
    /* SPR 13250 fix end */
    /* SPR 1542 changes start */
    UInt8  si_mapping_changed;
    /* SPR 1542 changes end */
    UInt8  numRB;
    UInt8  mcsIndex;
    /* SPR 3062 Chg */
    UInt8 tpc; /* It used to store tpc value for SIB1 message */
    /* SPR 3062 Chg Ends */
}SIBType1Msg;

typedef struct SIBType1InfoT
{
    SIBType1Msg sibType1Msg[2];
    UInt8 updateFlag;
    
    /*This flag is set when first SIB1 scheduled after the MAC initialization*/
    UInt8 firstSIBT1SentFlag;
    UInt8 activeIndex;
}SIBType1Info;

/*This structure is used to temprarly store the decoded SI messages
 *when received from RRC 
 */
typedef struct SIMsgInfoFromRRCT
{
    UInt8  startSubFrame;
    UInt16 siPeriodicity;
    UInt16 startSFN;
    /* Indicates type of format used for scheduling of SI mesage */
    UInt8  siMsgSgmnts;  
    UInt16  siMsgReptitions;  
    UInt8  siMsgTransmissions;  
    void   *msg_p[MAX_NO_SI_MSG_SGMNTS];    
    UInt16  msgLen[MAX_NO_SI_MSG_SGMNTS];
    UInt8  numRB[MAX_NO_SI_MSG_SGMNTS];   
    UInt8  mcsIndex[MAX_NO_SI_MSG_SGMNTS];
    UInt8  tpc[MAX_NO_SI_MSG_SGMNTS];
    /* SPR 3067 Chg */
    UInt8 remaingWindowSize;
    UInt8 siWindowSize;
    UInt8 transmissionCount;
    UInt8 currentTransmissionCount;
    /* offsetValue used to store offsets at which SI to
     * schedule */
    UInt8 offsetValue[8];
    /* SPR 3067 Chg Ends */

    /* SIB8_Code Start */
    UInt8  si8Flag;
    /* SIB8_Code End */
}SIMsgInfoFromRRC;
/** opt **/
/* @struct resAllocInitInfo
 * @brief  this strcuture contains information that is calculated at
 *          cell configuration and used during Resource Allocation
 */
typedef struct resAllocInitInfo
{
    UInt8 nRBStepDCI1C;
    UInt8 vDlRBDCI1C;
    UInt32 numberOfBitsForAllocationDCI1C;
    UInt32 numberOfBitsForAllocationNonDCI1C;
    UInt16 lsbForFillRar;
    UInt16 msbForFillRar;
}resAllocInitInfo_t;
/** opt **/

/* CR changes start */
#ifdef FDD_CONFIG
typedef struct PHICHSequenceNoPerPhichGroupT
{
  UInt8 bitMask;
  UInt8 count;
}PHICHSequenceNoPerPhichGroup;
#endif
/* CR changes end */

typedef struct StatsInfoT
{
    UInt32 totalUplinkBytesSend;
    /* SPR 15909 fix start */
    tickType_t ulStartThroughputTick;
    tickType_t dlStartThroughputTick;
    /* SPR 15909 fix end */
    UInt32 dlThroughPutEnable;
    UInt32 ulThroughPutEnable;
    UInt32 totalDlBytesSend;
    /* SPR 15909 fix start */
    tickType_t ulStartSpecEffTick;
    tickType_t dlStartSpecEffTick;
    /* SPR 15909 fix end */
    UDouble32 lastMacUlThroughput;
    UDouble32 lastMacDlThroughput;
}StatsInfo;

typedef struct PreferredMcsRbForATBTblT
{
    UInt8 preferredMcsForATB;
    UInt8 preferredRbForATB;
}PreferredMcsRbForATBTbl;

/*valid for all cells*/
typedef struct MacParamsAllCellT
{
    /* Number of cells to be configured indicated by OAM */
    UInt8 numCellsConfigured;
    /* Number of cells requested for INIT by OAM */
    UInt8 numInitReceived;
    /* Carrier aggregation feature supported or not */
    UInt8 isCASupported;
    UInt8 siSchedulingRATPolicy;
    UInt8 ulConfigAdvanceSubFrames;
    /* + SPR_11589 */ 
    UInt8 isInitDone;
    /* - SPR_11589 */ 
    InternalCellIndex	lowestRunningCellIndex; 
    /* + Critical section changes */
    /* SPR 15909 fix start */
    tickType_t lastStrategyExecutionTTI;
    /* SPR 15909 fix end */
    /* - Critical section changes */
    DCIFormat preferredPrimaryDCIFormatForSingleTB[MAX_TRANSMISSION_MODES + 1];
    DCIFormat preferredSecondaryDCIFormatForSingleTB[MAX_TRANSMISSION_MODES + 1];
    UInt32 macHealthMonitoringTimeInterval;
    /* Indicates the QCI details */
    QosQciInfo qosQciInfo[MAX_QCI];
    PreferredMcsRbForATBTbl preferredMcsRbForATBTbl[MAX_MCS_INDEX][MAX_RES_BLOCKS][MAX_POWER_REDUCTION_FOR_MCS_RB];
    UInt16 macMaxNumOfLcSupported;
    /* Spr 23393 Changes Start */
    /* Num Rar message Per TTI to be sent will be provided by OAM /
     * The Value for this parameter is between 1 to 4  */
    UInt8 rarPduPerTTI;
    /* Spr 23393 Changes End */

} MacParamsAllCell;

extern MacParamsAllCell macParams_g;

/* CA Changes end */


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#ifdef TDD_CONFIG
typedef UInt32 ( *determineSizeOfTB) (
         UInt32  mcsIndex, UInt32 nbRB);

#define FOUR_THIRD(x) ((x * 3) >> 2); 

#endif

#ifdef UTFWK_SIMULATION

#define CHECK_QOS()\
{                  \
    return;        \
}


/*QOS_CHG*/
extern void checkProcessTa();
/*QOS_CHG*/

#define PROCESS_TA_REQ()        \
{                               \
        checkProcessTa();       \
}
                      
#else

#define CHECK_QOS()\
{                  \
}                  

#define PROCESS_TA_REQ()\
{                       \
}

#endif

/** SPR 3657 Changes Start **/

#define INIT_RLC_RETX_Q(transmissionAndRLCData_p,lcCount)\
{\
   UInt32 count=0;\
   for(;count < lcCount; count++)\
   {\
       queueInit(&(transmissionAndRLCData_p->rlcDataBufferArr[count].reTxRlcPduQ));\
   }\
}

/** SPR 3657 Changes End **/

#define GET_MEM_FROM_POOL(dataType,data_p,size,pSize) \
{\
    data_p=(dataType *)getMemFromPool(size,pSize);\
    if(PNULL == data_p)\
    {\
        LOG_MAC_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,\
                getCurrentTick(),\
                __LINE__,size,0,0,0,\
                0.0,0.0,__FILE__,__func__);\
        ALARM_MSG (MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);\
    }\
}

/* DL Qos Changes */
                    
#define DL_QOS_GET_BASE_ADDRESS_FROM_ELIGIBLE_LC_ANCHOR(dlEligibleLcNodeVoid_p,\
                                                 dlEligibleLcNode_p) \
            dlEligibleLcNode_p =  (DLQosStrategyLCNode *) \
                  ((UInt8 *) dlEligibleLcNodeVoid_p - \
                   offsetof(DLQosStrategyLCNode , eligibleLcAnchor));
                            
#define DL_QOS_GET_BASE_ADDRESS_FROM_SCHEDULED_LC_ANCHOR(dlStrategyLcNodeVoid_p,\
                                                  dlStrategyLcNode_p) \
           dlStrategyLcNode_p =  (DLQosStrategyLCNode *) \
                  ((UInt8 *) dlStrategyLcNodeVoid_p - \
                   offsetof(DLQosStrategyLCNode , scheduledLcAnchor));


/*START : DYN_PDCCH*/
typedef enum DLULConfigTypeT
{
    BOTH_DL_UL = 0,
    ONLY_UL,
    ONLY_DL
}DLULConfigType;
#define MAX_CONTENTION_FACTOR 10
typedef enum AggregationLevelT
{
    AL1 = 0,
    AL2,
    AL4,
    AL8,
    AGGREGATION_LEVEL_MAX
} AggregationLevel;

#define CALCULATE_AND_UPDATE_CCE_CORRECTION_FACTOR(aggregationLevel, macRetType, internalCellIndex)\
  do\
  {\
    UInt8 index = 0;\
    UInt8 successAdjust = 0;\
    UInt8 failureAdjust = 0;\
    if (AGGREGATION_LEVEL_ONE == aggregationLevel)\
    {\
       index = AL1;\
       successAdjust = rrmMacCellGlobal_gp[internalCellIndex]->cceCorrectionStepDownFactor[index];\
       failureAdjust = rrmMacCellGlobal_gp[internalCellIndex]->cceCorrectionStepUpFactor[index];\
    }\
    if (AGGREGATION_LEVEL_TWO == aggregationLevel)\
    {\
       index = AL2;\
       successAdjust = rrmMacCellGlobal_gp[internalCellIndex]->cceCorrectionStepDownFactor[index];\
       failureAdjust = rrmMacCellGlobal_gp[internalCellIndex]->cceCorrectionStepUpFactor[index];\
    }\
    if (AGGREGATION_LEVEL_FOUR == aggregationLevel)\
    {\
       index = AL4;\
       successAdjust = rrmMacCellGlobal_gp[internalCellIndex]->cceCorrectionStepDownFactor[index];\
       failureAdjust = rrmMacCellGlobal_gp[internalCellIndex]->cceCorrectionStepUpFactor[index];\
    }\
    if (AGGREGATION_LEVEL_EIGHT == aggregationLevel)\
    {\
       index = AL8;\
       successAdjust = rrmMacCellGlobal_gp[internalCellIndex]->cceCorrectionStepDownFactor[index];\
       failureAdjust = rrmMacCellGlobal_gp[internalCellIndex]->cceCorrectionStepUpFactor[index];\
    }\
    if (MAC_SUCCESS == macRetType)\
    {\
      contentionFactor_g[index] = ((contentionFactor_g[index] > successAdjust)?\
					(contentionFactor_g[index] - successAdjust):\
					0);\
    }\
    else\
    {\
      contentionFactor_g[index] += failureAdjust;\
      if (contentionFactor_g[index] >= MAX_CONTENTION_FACTOR)\
      {\
        contentionFactor_g[index] = MAX_CONTENTION_FACTOR;\
      }\
    }\
  }while(0)
/*END : DYN_PDCCH*/
/*+ Dyn UEs To Be SchPerTTI:CR +*/
typedef enum ModifyValidateT
{
    NO_ACTION = 0,
    MODIFY_ONLY,
    VALIDATE_ONLY
}ModifyValidate;
/*- Dyn UEs To Be SchPerTTI:CR -*/
/*PHICH_SPR Fix 12758 start */
typedef enum ForcedAckStatusT
{
    FORCED_ACK_RESET = 0,
    FORCED_ACK_SENT,
}ForcedAckStatus;
/*SPR Fix 12758 end */
typedef enum DeInitTriggerT
{
    DE_QUEUE_NODES = 0,
    DE_INIT_QUEUE
}DeInitTrigger;

#define MAX_TPC_IN_A_TTI       100
#define MAX_UL_CONFIG_IN_A_TTI 100
#define PHY_ERROR_IND_DELAY_DL 1 
#define PHY_ERROR_IND_DELAY_UL 1 
#ifdef FDD_CONFIG
#define MAX_TPC_CONTAINER_MAP_SIZE 10
#define MAX_UL_CONFIG_CONTAINER_MAP_SIZE 10
#else
#define MAX_TPC_CONTAINER_MAP_SIZE 16
#define MAX_UL_CONFIG_CONTAINER_MAP_SIZE 16
/* SPR 11110 fix start */
#define TICK_MISS_SCENARIO  0
#define ERROR_SCENARIO      1
/* SPR 11110 fix end */
#endif

typedef enum ErrIndPowerControlTypeT 
{
    ERR_IND_TPC_FOR_PUCCH = 0,
    ERR_IND_TPC_FOR_PUSCH = 1,
}ErrIndPowerControlType;

/* Container to store the TPC sent */
typedef struct TpcSentT
{
    /* Power control on PUCCH = 0 or PUSCH = 1 */
    ErrIndPowerControlType pcType;
    /* UE rnti for which TPC was sent */
    UInt16 rnti;
    /* power correction value */
    SInt8 powerCorrectionVal;
}*LP_TpcSent, TpcSent;

typedef struct TpcSentMapT
{
    /* Number UE's for which TPC has been sent */
    UInt8 numUe;
    /* Tick counter whent TPC was sent */
    /* SPR 15909 fix start */
    tickType_t tick;
    /* SPR 15909 fix end */
    /* TPC Information */
    TpcSent tpcSentInfo[MAX_TPC_IN_A_TTI];
}*LP_TpcSentMap, TpcSentMap;

/* Container to store the UL Config Information */
typedef struct UlConfigInfoT
{
    /* UE rnti for which UL_CONFIG was sent */
    UInt16 rnti;
    /* PDU Type in UL_CONFIG msg */
    UInt8 pduType;
    /* ul harq process Id used */
    UInt8 ulHarqProcessId;

}UlConfigInfo;
typedef struct UlConfigMapT
{
    /*CSR 152109 debug +-*/
    /* Number UE's for which UL Config has been sent */
    UInt16 numUe;
	UInt16 totalCount;
	/*CSR 152109 debug -+*/
    /* Tick counter whent UL Config was sent */
    /* SPR 15909 fix start */
    tickType_t tick;
    /* SPR 15909 fix end */
    /* TPC Information */
    UlConfigInfo ulConfigInfo[MAX_UL_CONFIG_IN_A_TTI];
}UlConfigMap;

/* SPR 11230 fix start */
#ifdef TDD_CONFIG
#define MAX_HARQ_IN_A_TTI 100
#define MAX_HARQ_CONTAINER_MAP_SIZE 16

/* HARQ map container structure */
typedef struct HarqSentT
{
    UInt16 ueIndex;
    UInt8  indexSfn;
    UInt8  dlAckNackSf;
} *LP_HarqSent, HarqSent;

typedef struct HarqSentMapT
{
    /* Number UE's for which HARQ has been sent */
    UInt8 numUe;
    /* Tick counter when HARQ was sent */
    /* SPR 15909 fix start */
    tickType_t  tick;
    /* SPR 15909 fix end */
    /* HARQ Information */
    HarqSent harqSentInfo[MAX_HARQ_IN_A_TTI];
} *LP_HarqSentMap, HarqSentMap;

/* HARQ map container container */
/* CA TDD Changes Start */
extern HarqSentMap harqSentMap_g[MAX_NUM_CELL][MAX_HARQ_CONTAINER_MAP_SIZE];
/* CA TDD Changes End */
#endif
/* SPR 11230 fix end */

/* tpc sent container */
extern TpcSentMap tpcSentMap_g[MAX_NUM_CELL][MAX_TPC_CONTAINER_MAP_SIZE];

/* Cyclomatic_complexity_changes_start */
extern void getModSchemeFromMcsIndex(UInt8 mcs, UInt8 *modType_p);
extern void getUlModSchemeFromMcsIndex(UInt8 mcs,
        UInt8 *modType_p,
        UInt8 enb64QAMFlag,
        UInt8 ueCategory);
/* Cyclomatic_complexity_changes_end */
/*SPR 8451 Changes Start*/
#ifdef ULCODERATE_PLUGIN
/*This macro is used only for UL Coderate only
 * if enb64QAMFlag=0 OR ueCategory < 5, then modType=SIXTEENQAM to be used for TB size determination*/
#define GET_UL_MOD_SCHEME_FOR_CODERATE(mcs,modType,modSchemeIndex)\
{\
    if(mcs <= UL_MCS_INDEX_QPSK_10)\
    {\
        modType = QPSK;\
    }\
    else if((UL_MCS_INDEX_16QAM_11 <=mcs) && (mcs<= UL_MCS_INDEX_16QAM_20))\
    {\
        modType = SIXTEENQAM;\
    }\
    else if((UL_MCS_INDEX_64QAM_21 <=mcs) && (mcs<= UL_MCS_INDEX_64QAM_28))\
    {\
        if (modSchemeIndex == 1)\
        {\
            modType = SIXTEENQAM;\
        }\
        else if (modSchemeIndex == 2)\
        {\
            modType = SIXFOURQAM;\
        }\
    }\
}
#endif
/*SPR 8451 Changes End*/

/* EICIC +*/
/*CA Changes start  */
extern UL_DRX_ON_OFF_TRIGGER_TYPE* ulDRXOnOffTriggerQueue_gp[MAX_NUM_CELL];
/*CA Changes end  */
/* TDD Config 0 Changes Start */
typedef enum CountT
{
    NUM_ZERO = 0,
    NUM_ONE,
    NUM_TWO,
    NUM_THREE,
    NUM_FOUR,
    NUM_FIVE,
    NUM_SIX,
    NUM_SEVEN,
    NUM_EIGHT,
    NUM_NINE,
    NUM_TEN
}Count;
#ifdef TDD_CONFIG
#define MAX_UL_SUBFRAME_GRANT 2
#define GET_UL_SUBFRAME_OFFSET(ulSubFrameOffSet,subFrameNum,ulSubFrameNum)\
if( ulSubFrameNum < subFrameNum )\
{\
    ulSubFrameOffSet = (MAX_SUB_FRAME + ulSubFrameNum) - subFrameNum;\
}\
else\
{\
    ulSubFrameOffSet = ulSubFrameNum - subFrameNum;\
}
#endif
/* TDD Config 0 Changes End */

#endif  
