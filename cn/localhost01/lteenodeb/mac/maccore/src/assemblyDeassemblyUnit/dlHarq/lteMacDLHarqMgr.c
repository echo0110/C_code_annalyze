/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacDLHarqMgr.c,v 1.2.6.1.4.2.2.3 2010/10/29 10:45:10 gur20056 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains all the functions realated to DL
 *                     HARQ process. 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacDLHarqMgr.c,v $
 * Revision 1.2.6.1.4.2.2.3  2010/10/29 10:45:10  gur20056
 * SPR 618 fix - Deepa
 *
 * Revision 1.2.6.1.4.2.2.2  2010/09/29 05:08:19  gur04640
 * Warning uncommented
 *
 * Revision 1.2.6.1.4.2.2.1  2010/09/21 15:46:00  gur20491
 * FAPI changes
 *
 * Revision 1.52.24.2  2010/08/30 13:48:58  gur24420
 * Updated RLC AM fixes
 *
 * Revision 1.52.24.1  2010/08/09 15:02:32  gur23951
 * ADDED SUPPORT FOR RLC AM MODE
 *
 * Revision 1.52  2010/02/24 05:15:05  gur18550
 * The mimo bug solved of freeing memory
 *
 * Revision 1.51  2009/12/22 14:22:51  gur11912
 * added dlHarqEntityTimerList_p->count--
 *
 * Revision 1.50  2009/12/22 05:07:23  gur11912
 * modified ackNackNode_p->ttiCounter to ackNackNode_p->packetSubFrame
 *
 * Revision 1.49  2009/12/18 10:44:59  gur11912
 * added warnings and removed subFrameNumber = subFrameNumber %MAX_SUB_FRAME
 *
 * Revision 1.48  2009/11/03 14:00:09  gur11912
 * commented HarqTimerNodeArrayIndex = INVALID_COUNT
 *
 * Revision 1.47  2009/10/05 08:08:33  gur11912
 * removed printf
 *
 * Revision 1.46  2009/09/22 16:26:42  gur19413
 * handling delay recieved ack/nack
 *
 * Revision 1.45  2009/09/22 07:03:14  gur11912
 * optimization related changes
 *
 * Revision 1.44  2009/09/21 12:29:49  gur18550
 * Optimisation related changes
 *
 * Revision 1.43  2009/08/13 10:21:41  gur18550
 * Warnings Removed
 *
 * Revision 1.42  2009/08/12 06:55:23  gur18550
 * Function updateNackQueueLoadForReTransComplete Introduced
 *
 * Revision 1.41  2009/08/03 06:08:19  gur18550
 * Harq Updated
 *
 * Revision 1.40  2009/07/31 07:40:45  gur18550
* MIMO case HARQ Functionality Changed
*
* Revision 1.39  2009/07/30 15:07:21  gur18550
* Error Handling Improved in case of MIMO
*
* Revision 1.38  2009/07/30 11:10:00  gur18550
* Functionality of TA DRX on completion of Re trans
*
* Revision 1.37  2009/07/24 11:31:31  gur18550
* Logs Updated
*
* Revision 1.36  2009/07/23 14:04:21  gur18569
* UT testing changes
*
* Revision 1.35  2009/07/23 08:58:52  gur18569
* modification in harq freeing code
*
* Revision 1.34  2009/07/22 15:54:26  gur18569
* added UT_TESTING logs
*
* Revision 1.33  2009/07/22 10:30:53  gur18550
* More Logs added
*
* Revision 1.32  2009/07/16 10:28:14  gur18550
* Function Update
*
* Revision 1.31  2009/07/16 08:51:24  gur18550
* Logs Updated
*
* Revision 1.30  2009/07/13 11:08:53  gur18569
*  fixed UT  BUG : removing  msgFree((void*)dlHarqTbInfo_p->transmissionAndRLCData_p) because this in never zero buffer
*
* Revision 1.29  2009/07/13 10:41:22  gur18550
* File updated
*
* Revision 1.28  2009/07/11 05:30:26  gur18550
* Bug Fixed
*
* Revision 1.27  2009/07/10 06:49:49  gur18550
* File Updated
*
* Revision 1.23  2009/07/08 09:22:45  gur18550
* Log Changed
*
* Revision 1.22  2009/07/08 08:46:32  gur20052
*  Added Logs for debugging
*
* Revision 1.21  2009/07/07 12:38:24  gur18550
* Function Updated
*
* Revision 1.20  2009/07/07 08:06:48  gur20052
* changes related to the HARQ implementation
*
* Revision 1.19  2009/06/23 13:23:05  gur18550
* Error Removed due to change in name by strategy
*
* Revision 1.18  2009/06/22 14:30:52  gur18550
* initDLHarq
*
* Revision 1.17  2009/06/19 13:02:50  gur18550
* Removed Memory Error
*
* Revision 1.16  2009/06/19 07:15:13  gur18550
* Funtion
*
* Revision 1.15  2009/06/19 07:14:14  gur18550
* Funtion
*
* Revision 1.14  2009/06/19 06:57:02  gur18550
* Second Review Comments Inc
*
* Revision 1.13  2009/06/18 06:57:39  gur18550
* HarqProcessID
*
* Revision 1.12  2009/06/12 05:04:08  gur18550
* Added INVALID_TX
*
* Revision 1.11  2009/06/11 08:28:42  gur18550
* Function Updated
*
* Revision 1.10  2009/06/08 15:43:58  gur12140
* Removed Linking Errors
*
* Revision 1.9  2009/06/08 11:24:00  gur18550
* Comments Inc
*
* Revision 1.8  2009/06/08 09:41:08  gur18550
* Function Updated
*
* Revision 1.7  2009/06/08 04:57:15  gur18550
* Comments Inc.
*
* Revision 1.6  2009/06/08 04:46:41  gur18550
* Comments Inc.
*
* Revision 1.5  2009/06/05 15:35:03  gur18550
* Comments Inc.
*
* Revision 1.4  2009/06/02 10:16:05  gur18550
* File Updated
*
* Revision 1.3  2009/05/28 12:00:36  gur18550
* Function Updated
*
* Revision 1.2  2009/05/26 13:05:46  gur18550
* Function Updated
*
* Revision 1.1  2009/05/26 11:25:49  gur18550
* *** empty log message ***
*
*
*****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteMacDLHarqMgr.h"
#include "lteMacCellConfMgr.h"
#include "lteMacRandomAccess.h"
#include "lteMacParseOAMMsg.h"
#include "lteMacCQI.h"
#ifdef TDD_CONFIG
#include "lteMacTddMgr.h"
#endif
#ifdef ACK_NACK_COUNT_STATS
#include "lteCommonStatsManager.h"
#endif
#include "lteMacRRMPeriodicMgr.h"
#include "ltePerfStats.h"
#include "lteMacDLSpsScheduler.h"
#include "lteCircQueue.h"
 /* SPR 19679: TDD HARQ Multiplexing Changes Start */
#include "lteMacPUSCH.h"
 /* SPR 19679: TDD HARQ Multiplexing Changes End */

/******************************************************************************
  Private Definitions
 *****************************************************************************/



/******************************************************************************
  Private Types
 *****************************************************************************/
#ifdef TDD_CONFIG

/* This is 2D array for maintaining expiry list
 * for 10 subframe.
 * 2D array is taken because we have to handle the DL ACK/NACK
 * which need to be expired in next system frame.
 * */
/** CA-TDD Changes Start **/
DLHarqTimerEntityList tddDlHarqTimerEntityList_g[MAX_NUM_CELL][MAX_TDD_HARQ_CONT][MAX_SUB_FRAME];
/** CA-TDD Changes End **/
#define HARQ_TX_BUFFER 0
#define RLC_BUFFER 1
#define MAC_RLC_INTF_BUFFER 2
/* SPR 11230 fix start */
/* HARQ map container container */
/* CA TDD Changes Start */
HarqSentMap harqSentMap_g[MAX_NUM_CELL][MAX_HARQ_CONTAINER_MAP_SIZE];
/* CA TDD Changes End */
/* SPR 11230 fix end */
#endif
/* SPR 21029 fix start */
#define RESET 0
/* SPR 21029 fix end */
#ifdef KPI_STATS
/* SPR 16613 fix start */
void lteMacUpdateKpiStatsIPThpDlOnHarqNack(DLHARQProcess* dlHarqProcess_p,
        UInt16 ueIndex,UInt32 rcvTTI,InternalCellIndex internalCellIndex);/* +- SPR 17745 */ 
/* SPR 16613 fix end */
void lteMacUpdateKpiStatsIPThpDlOnHarqAck (
        UInt16 ueIndex,
        DLHARQProcess* dlHarqProcess_p,
        /* SPR 15909 fix start */
        /* +- SPR 17777 */
        tickType_t rcvTTI
        /* SPR 15909 fix end */
#ifdef TDD_CONFIG
		,InternalCellIndex internalCellIndex
#endif
        );
/* +- SPR 17777 */
/* - KPI_CA */
#endif

#ifndef DL_UL_SPLIT
extern LTE_SQUEUE* ackNackQueue_gp[MAX_NUM_CELL];
#endif
extern UInt8 totalNumberOfTickDiffDl_g[MAX_NUM_CELL];
extern UInt16 totalTickMissErrIndDl_g[MAX_NUM_CELL];

/*EICIC +*/
extern UInt8 usableAbsPattern_g[MAX_NUM_CELL][UPDATE_TYPE_INDEX][NUM_BITS_ABS_PATTERN];
extern  UInt8 usableAbsPatternIndex_g[MAX_NUM_CELL];
UInt32 eicicHarqSubframeNumber_g = 0 ; 
UInt32 eicicHarqSystemframeNumber_g = 0;
UInt8 eicicFindNumOfBitsinABSPattern(InternalCellIndex internalCellIndex);

/* EICIC +*/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* RETX_CHG */
    /* CA_phase2_harq_code Start */
extern MacRetType putEntryInDLRetransmissionQueue(
        DLUEContext*  ueContext_p, UInt8 harqProcessId,
        /* SPR 15909 fix start */
        tickType_t harqRTTExpiryTTI, UInt8 schUsed,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex
           );

extern MacRetType macPushHarqNackInfoToRetxQueue( DLUEContext *ueContext_p,
                             DLRetxQueueNode retxNodeFilled ,
                             UInt8 *nackFlag_p,
                             InternalCellIndex internalCellIndex
#ifdef TDD_CONFIG 
                             ,UInt8 currentSubframe
#endif                             
                             );
    /* CA_phase2_harq_code End */
/* RETX_CHG */

/* CLPC_CHG*/
extern SInt8 checkTPCForPUCCHHarqBler(UInt8 bler/* +- SPR 17777 */
        , InternalCellIndex internalCellIndex);
/*SPR 15208 Fix*/ 
extern void handleDLTimingAdvance(DLUEContext *newUEcontext_p, InternalCellIndex internalCellIndex);
/*SPR 15208 Fix*/ 

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/*
 * This is array which contains the mask bit that is  
 * used for setting bitmask in the DL UE Context with
 * respect to the TX subframe number. 
 * */
extern UInt32 bitmask[MAX_SUB_FRAME];
/* 
 * This is array which contains the mask bit that is  
 * used for resetting bitmask in the DL UE Context with
 * respect to the TX subframe number. 
 * */
extern UInt32 resetBitmask[MAX_SUB_FRAME];

/* + CQI_5.0 */
SInt16 dlBlerCorrectionFactor_g[DL_BLER_CORRECTION_FACTOR_TABLE_SIZE];
/* - CQI_5.0 */

#ifdef DL_UL_SPLIT
extern SInt32 ackNackQueueIdDL_g[MAX_NUM_CELL];
extern LTE_SEM waitForDLPerfStatsUpd_g[MAX_NUM_CELL];
/* warning removed*/
/* +- SPR 17777 */
extern SInt32 recvMsgWithIccQ(SInt32 mqdes, const void *msg_ptr);
/* +- SPR 17777 */
#endif

#if defined(CIRC_QUEUE_IMPL) && !defined(DL_UL_SPLIT_TDD)
extern SEM_T waitForDLHarqAckNack_g[MAX_NUM_CELL];
#endif

/* SPR 5531 changes start */
PENDING_DL_MEMORY_POOL_FREE_TYPE *pendingDLMemoryPoolFreeQ_gp[MAX_NUM_CELL] = {PNULL};
/* SPR 5531 changes end */

#ifdef FDD_CONFIG
DLHarqGlobals dlHarqGlobals_g[MAX_NUM_CELL];
#endif

/* SPR 11402 fix */
void sendARQIndicationToRLC (UInt32 ueIndex,
			     DLHARQTBInfo *tbInfo,TbValue TBFlag,InternalCellIndex internalCellIndex);
/* SPR 11402 fix */

/*this is the global structure array containg scell timer event triggers */
DLCAGlobals dlCAGlobals_g[MAX_NUM_CELL];
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/* RETX_CHG */
LTE_SQUEUE *dlRetransmissionQueue_gp[MAX_NUM_CELL];
LTE_SQUEUE *dlRetransmissionFailureQueue_gp[MAX_NUM_CELL];
/* RETX_CHG */
/* + SPR 17733 */
#ifdef FDD_CONFIG
LTE_SQUEUE Msg4RetransmissionQueue_gp[MAX_NUM_CELL];
LTE_SQUEUE Msg4RetransmissionFailureQueue_gp[MAX_NUM_CELL];
#endif
/* - SPR 17733 */
#ifdef TDD_CONFIG

/* + SPR 13927 Changes */
extern DlAssociationSetNode * globalDlAssociationSet_gp[MAX_NUM_CELL] ;
/* - SPR 13927 Changes */

/* Pointer to TddPdschTxMapOfDLHarq for current UL/DL config.
 * This pointer is used for Harq RTT.
 * */
/** CA-TDD Changes Start **/
TddPdschTxMapOfDLHarq *tddPdschTxMapOfDLHarq_p[MAX_NUM_CELL];
/** CA-TDD Changes End **/
/*
 * This is the prototype of 
 * Function pointer for DL Harq Expiry function 
 * */

typedef MacRetType ( *tddExpireNodeFromHarqTimerListType) ( 
        UInt32 sfn
        ,UInt8 freeHarqOnly
        /** CA-TDD Changes Start **/
        ,InternalCellIndex internalCellIndex
        /** CA-TDD Changes End **/
        );

/*
 * This is 2D array for maintaining the 
 * UL Subframe - where downlink ack/nack will be
 *               recieved for dl transmitted.
 *
 * sfnFactor -   This sfnFactor is used to get the 
 *                container index.
 *
 * harqRttTimerFactor - It is used to get the value
 *                      RTT timer for a particular
 *                      subframe.
 * */
TddPdschTxMapOfDLHarq tddPdschTxMapOfDLHarq_g
[TOTAL_NUM_UL_DL_CONFIG][MAX_SUB_FRAME] =
{
    /* UL DL Config 0 */
    {
        /*   Subframe No 0*/
        {
            UL_IN_SUB_FRAME_NO_4, /*dlAckNackSf*/
/*TDD Config 0 Changes Start */
            SYSTEM_FRAME_BACK_0, /*sfnFactor*/
/*TDD Config 0 Changes End */
            DL_HARQ_RTT_TIMER_4_PLUS_4 /*harqRttTimerFactor */
        },
        /*   Subframe No 1*/
        {
            UL_IN_SUB_FRAME_NO_7, /*dlAckNackSf*/
/*TDD Config 0 Changes Start */
            SYSTEM_FRAME_BACK_0, /*sfnFactor*/
/*TDD Config 0 Changes End */
            DL_HARQ_RTT_TIMER_6_PLUS_4 /*harqRttTimerFactor */
        },
        /*   Subframe No 2*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 3*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 4*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 5*/
        {
            UL_IN_SUB_FRAME_NO_9, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_0, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_4_PLUS_4 /*harqRttTimerFactor */
        },
        /*   Subframe No 6*/
        {

            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_6_PLUS_4 /*harqRttTimerFactor */
        },
        /*   Subframe No 7*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 8*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 9*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        }                                                                     	

    },
    /* UL DL Config 1 */                                                                
    {
        /*   Subframe No 0*/
        {
            UL_IN_SUB_FRAME_NO_7, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_0,  /*sfnFactor*/
            DL_HARQ_RTT_TIME_11   /*harqRttTimerFactor 7 + 4 + 3*/
        },
        /*   Subframe No 1*/
        {
            UL_IN_SUB_FRAME_NO_7, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_0, /*sfnFactor*/
            DL_HARQ_RTT_TIME_10 /*harqRttTimerFactor 6 + 4*/
        },
        /*   Subframe No 2*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 3*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 4*/
        {
            UL_IN_SUB_FRAME_NO_8, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_0,  /*sfnFactor*/
            DL_HARQ_RTT_TIME_8   /*harqRttTimerFactor 4 + 4 + 3*/
        },
        /*   Subframe No 5*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1,  /*sfnFactor*/
            DL_HARQ_RTT_TIME_11   /*harqRttTimerFactor 7 + 4 */
        },
        /*   Subframe No 6*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1,  /*sfnFactor*/
            DL_HARQ_RTT_TIME_10   /*harqRttTimerFactor 6 + 4 */
        },
        /*   Subframe No 7*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 8*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 9*/
        {
            UL_IN_SUB_FRAME_NO_3, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1,  /*sfnFactor*/
            DL_HARQ_RTT_TIME_8   /*harqRttTimerFactor 4 + 4 + 3*/
        }
    },
    /* UL DL Config 2 */                                                                
    {
        /*   Subframe No 0*/
        {
            UL_IN_SUB_FRAME_NO_7, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_0, /*sfnFactor*/
            DL_HARQ_RTT_TIME_11 /*harqRttTimerFactor 7 + 4 + 3 */
        },
        /*   Subframe No 1*/
        {
            UL_IN_SUB_FRAME_NO_7, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_0, /*sfnFactor*/
            DL_HARQ_RTT_TIME_10 /*harqRttTimerFactor 6 + 4 */
        },
        /*   Subframe No 2*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 3*/
        {
            UL_IN_SUB_FRAME_NO_7, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_0,  /*sfnFactor*/
            DL_HARQ_RTT_TIME_8   /*harqRttTimerFactor 4 + 4 + 4 */
        },
        /*   Subframe No 4*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIME_12 /*harqRttTimerFactor 8 + 4 + 2 */
        },
        /*   Subframe No 5*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIME_11 /*harqRttTimerFactor 7 + 4 + 3 */
        },
        /*   Subframe No 6*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIME_10 /*harqRttTimerFactor 6 + 4 */
        },
        /*   Subframe No 7*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 8*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIME_8 /*harqRttTimerFactor 4 + 4 + 4 */
        },
        /*   Subframe No 9*/
        {
            UL_IN_SUB_FRAME_NO_7, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIME_12 /*harqRttTimerFactor 8 + 4 + 2*/  
        }                                                   
    },
    /* UL DL Config 3 */                                                                
    {
        /*   Subframe No 0*/
        {
            UL_IN_SUB_FRAME_NO_4, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_0, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_4_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 1*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_11_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 2*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 3*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 4*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 5*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_7_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 6*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_6_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 7*/
        {
            UL_IN_SUB_FRAME_NO_3, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_6_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 8*/
        {
            UL_IN_SUB_FRAME_NO_3, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_5_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 9*/
        {
            UL_IN_SUB_FRAME_NO_4, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_5_PLUS_4 /*harqRttTimerFactor*/  
        }
    },
    /* UL DL Config 4 */                                                                
    {
        /*   Subframe No 0*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_12_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 1*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_11_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 2*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 3*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 4*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_8_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 5*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_7_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 6*/
        {
            UL_IN_SUB_FRAME_NO_3, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_7_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 7*/
        {
            UL_IN_SUB_FRAME_NO_3, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_6_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 8*/
        {
            UL_IN_SUB_FRAME_NO_3, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_5_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 9*/
        {
            UL_IN_SUB_FRAME_NO_3, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_4_PLUS_4 /*harqRttTimerFactor*/  
        }
    },
    /* UL DL Config 5 */                                                                
    {
        /*   Subframe No 0*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_12_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 1*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_11_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 2*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 3*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_9_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 4*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_8_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 5*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_7_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 6*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_6_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 7*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_5_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 8*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_4_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 9*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_0, /*sfnFactor*/   
            DL_HARQ_RTT_TIMER_13_PLUS_4 /*harqRttTimerFactor*/  
        }
    },

    /* UL DL Config 6 */                                                                
    {
        /*   Subframe No 0*/
        {
            UL_IN_SUB_FRAME_NO_7, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_0, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_7_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 1*/
        {
            UL_IN_SUB_FRAME_NO_8, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_0, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_7_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 2*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 3*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 4*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 5*/
        {
            UL_IN_SUB_FRAME_NO_2, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_7_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 6*/
        {
            UL_IN_SUB_FRAME_NO_3, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_7_PLUS_4 /*harqRttTimerFactor*/  
        },
        /*   Subframe No 7*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 8*/
        {
            0, /*dlAckNackSf*/
            0, /*sfnFactor*/
            0 /*harqRttTimerFactor*/
        },
        /*   Subframe No 9*/
        {
            UL_IN_SUB_FRAME_NO_4, /*dlAckNackSf*/
            SYSTEM_FRAME_BACK_1, /*sfnFactor*/
            DL_HARQ_RTT_TIMER_5_PLUS_4 /*harqRttTimerFactor*/  
        }                                                                                      
    }
};

/*
 *  Funtion pointer table of DL harq expiry functions
 *  which are called for expirying the HARQ for which 
 *  ACK/NACK is not received for DL Data transmitted
 *  Maintained for all UL-DL Configuration
 */
tddExpireNodeFromHarqTimerListType 
tddExpireNodeFromHarqTimerList_g[TOTAL_NUM_UL_DL_CONFIG][MAX_SUB_FRAME]=
{
    /* For UL/DL Config-0*/

    {
        unexpectedEvent,
        unexpectedEvent,
        tddExpireNodeForConfig0Subframe2,
        unexpectedEvent,
        tddExpireNodeForConfig0Subframe4,
        unexpectedEvent,
        unexpectedEvent,
        tddExpireNodeForConfig0Subframe7,
        unexpectedEvent,
        tddExpireNodeForConfig0Subframe9,
    },

    /* For UL/DL Config-1*/
    {
	    unexpectedEvent,
	    unexpectedEvent,
	    tddExpireNodeForConfig1Subframe2,
	    tddExpireNodeForConfig1Subframe3,
	    unexpectedEvent,
	    unexpectedEvent,
	    unexpectedEvent,
	    tddExpireNodeForConfig1Subframe7,
	    tddExpireNodeForConfig1Subframe8,
	    unexpectedEvent,
    },
    /* For UL/DL Config-2*/
    {
        unexpectedEvent,
        unexpectedEvent,
        tddExpireNodeForConfig2Subframe2,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        tddExpireNodeForConfig2Subframe7,
        unexpectedEvent,
        unexpectedEvent,
    },
    /* For UL/DL Config-3*/
    {
        unexpectedEvent,
        unexpectedEvent,
        tddExpireNodeForConfig3Subframe2,
        tddExpireNodeForConfig3Subframe3,
        tddExpireNodeForConfig3Subframe4,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
    },

    /* For UL/DL Config-4*/
    {
        unexpectedEvent,
        unexpectedEvent,
        tddExpireNodeForConfig4Subframe2,
        tddExpireNodeForConfig4Subframe3,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
    },

    /* For UL/DL Config-5*/
    {
        unexpectedEvent,
        unexpectedEvent,
        tddExpireNodeForConfig5Subframe2,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
    },

    /* For UL/DL Config-6*/
    {

        unexpectedEvent,
        unexpectedEvent,
        tddExpireNodeForConfig6Subframe2,       
        tddExpireNodeForConfig6Subframe3,
        tddExpireNodeForConfig6Subframe4,
        unexpectedEvent,
        unexpectedEvent,
        tddExpireNodeForConfig6Subframe7,
        tddExpireNodeForConfig6Subframe8,
        unexpectedEvent,
    }
};

/* The pointer of tddExpireNodeFromHarqTimerListType
 * to hold the function pointer table for configured
 * UL/DL configurations. 
 */
tddExpireNodeFromHarqTimerListType *tddExpireNodeFromHarqTimerList_p[
     MAX_NUM_CELL];

/* SPR 11110 fix start */
/* Funtion pointer table for DL harq expiry functions to expire all nodes
 * for which ACK/NACK is not received for DL Data transmitted
 * maintained for all UL-DL Configuration
 */
tddExpireNodeFromHarqTimerListType
tddExpireAllNodeFromHarqTimerList_g[TOTAL_NUM_UL_DL_CONFIG][MAX_SUB_FRAME]=
{
    /* For UL/DL Config-0*/
    {
        unexpectedEvent,
        unexpectedEvent,
        tddExpireAllNodeForConfig0Subframe2,
        unexpectedEvent,
        tddExpireAllNodeForConfig0Subframe4,
        unexpectedEvent,
        unexpectedEvent,
        tddExpireAllNodeForConfig0Subframe7,
        unexpectedEvent,
        tddExpireAllNodeForConfig0Subframe9,
    },

    /* For UL/DL Config-1*/
    {
        unexpectedEvent,
        unexpectedEvent,
        tddExpireAllNodeForConfig1Subframe2,
        tddExpireAllNodeForConfig1Subframe3,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        tddExpireAllNodeForConfig1Subframe7,
        tddExpireAllNodeForConfig1Subframe8,
        unexpectedEvent,
    },
    /* For UL/DL Config-2*/
    {
        unexpectedEvent,
        unexpectedEvent,
        tddExpireAllNodeForConfig2Subframe2,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        tddExpireAllNodeForConfig2Subframe7,
        unexpectedEvent,
        unexpectedEvent,
    },
    /* For UL/DL Config-3*/
    {
        unexpectedEvent,
        unexpectedEvent,
        tddExpireAllNodeForConfig3Subframe2,
        tddExpireAllNodeForConfig3Subframe3,
        tddExpireAllNodeForConfig3Subframe4,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
    },

    /* For UL/DL Config-4*/
    {
        unexpectedEvent,
        unexpectedEvent,
        tddExpireAllNodeForConfig4Subframe2,
        tddExpireAllNodeForConfig4Subframe3,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
    },

    /* For UL/DL Config-5*/
    {
        unexpectedEvent,
        unexpectedEvent,
        tddExpireAllNodeForConfig5Subframe2,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
        unexpectedEvent,
    },

    /* For UL/DL Config-6*/
    {
        unexpectedEvent,
        unexpectedEvent,
        tddExpireAllNodeForConfig6Subframe2,
        tddExpireAllNodeForConfig6Subframe3,
        tddExpireAllNodeForConfig6Subframe4,
        unexpectedEvent,
        unexpectedEvent,
        tddExpireAllNodeForConfig6Subframe7,
        tddExpireAllNodeForConfig6Subframe8,
        unexpectedEvent,
    }
};

/* The pointer of tddExpireNodeFromHarqTimerListType
 * to hold the function pointer table for configured
 * UL/DL configurations.
 */
tddExpireNodeFromHarqTimerListType *tddExpireAllNodeFromHarqTimerList_p[
   MAX_NUM_CELL];

STATIC MacRetType tddExpireAllNodeForConfig0(UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex );
STATIC MacRetType tddExpireNodesForContainerIdxConfig1SubFrame7(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex );
STATIC MacRetType tddExpireNodesForContainerIdxConfig1SubFrame8(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex );
STATIC MacRetType tddExpireNodesForContainerIdxConfig1SubFrame2(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex );
STATIC MacRetType tddExpireNodesForContainerIdxConfig1SubFrame3(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex );
STATIC MacRetType tddExpireNodesForContainerIdxConfig2SubFrame7(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex );
STATIC MacRetType tddExpireNodesForContainerIdxConfig2SubFrame2(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex );
STATIC MacRetType tddExpireNodesForContainerIdxConfig3SubFrame2(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex );
STATIC MacRetType tddExpireNodesForContainerIdxConfig3SubFrame3(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex );
STATIC MacRetType tddExpireNodesForContainerIdxConfig3SubFrame4(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex );
STATIC MacRetType tddExpireNodesForContainerIdxConfig4SubFrame2(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex );
STATIC MacRetType tddExpireNodesForContainerIdxConfig4SubFrame3(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex );
STATIC MacRetType tddExpireNodesForContainerIdxConfig5SubFrame2(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex );
STATIC MacRetType tddExpireNodesForContainerIdxConfig6SubFrames78(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex );
STATIC MacRetType tddExpireNodesForContainerIdxConfig6SubFrames234(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex );
/*SPR 11110 fix end */
#endif

/* SPR 6333 TI_TA_FIX_START */
#define RESET_TA_AVG_VALUE(ueContext_p, dlHarqTB_p)\
    switch (dlHarqTB_p->taDrxFlag)\
    {\
        case TA_SPATIAL_MUL:\
        case TA_DRX_SPATIAL_MUL:\
        case TA_NON_SPATIAL_MUL:\
        case TA_DRX_NON_SPATIAL_MUL:\
        case TA_MSG:\
        case TA_DRX_MSG:\
        case TA_WITH_DATA_SPATIAL_MUL:\
        case TA_DRX_WITH_DATA_SPATIAL_MUL:\
        case TA_WITH_DATA_NON_SPATIAL_MUL:\
        case TA_DRX_WITH_DATA_NON_SPATIAL_MUL:\
             {\
  /* CA_phase2_tat Start */ \
                ueContext_p->lastCalculatedTAAverage =\
                MAC_TIMING_ADVANCE_ADJUSTMENT_VAL_31*100;\
  /* CA_phase2_tat End */ \
             }\
             break;\
            \
        default:\
            break;\
    }\
/* SPR 6333 TI_TA_FIX_END */
/* + SPR 22243 Fix*/
#define RESET_PENDING_DRX_ACK_NACK_VALUE(ueContext_p, taDrxFlag)\
    if ((taDrxFlag == DRX_MSG) || (taDrxFlag == TA_DRX_MSG))\
{\
    ueContext_p->pendingDRXAckNack = FALSE;\
}\
/* - SPR 22243 Fix*/
/* Cyclomatic Complexity changes - starts here */
STATIC  void updateAndCheckHarqBlerForPucch (
        DLUEContext  *dlUEContext_p, 
        UInt8        oprTb,
        /* SPR 15909 fix start */
        tickType_t       currentTick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex);
#ifdef TDD_CONFIG
static  void processDLHarqMIMOAckNack ( UInt8 operation, 
        UInt16        ueIndex,
        /* SPR 15909 fix start */
        tickType_t        globalTick,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
        DLUEContext   *dlUeContext_p, 
        DLHARQProcess *dlHarqProcess_p,
        /* + SPS_TDD_CHANGES */
        tddAckNackQueueNode  *ackNackNode_p,
        /* - SPS_TDD_CHANGES */
#ifdef KPI_STATS
        /* SPR_Fix_17928_start */
        /* + 32.425 Events changes */
        UInt8          recvTTI,
        /* + 32.425 Events changes */
#endif
        /* +- SPR 17777 */
        /* + CA_TDD_HARQ_CHANGES */
        InternalCellIndex ueInternalCellIndex
	/* SPR_Fix_17928_end */
        ,InternalCellIndex  servingIndex
        ,UInt8 *bitMaskForAperiodicReport_p        
        ,UInt8 *nackFlag_p
        /* - CA_TDD_HARQ_CHANGES */
        );
static  void processDLHarqNonMIMOAckNack ( UInt8 operation, 
        UInt16        ueIndex,
        /* SPR 15909 fix start */
        tickType_t       globalTick,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
        DLUEContext   *dlUeContext_p, 
        DLHARQProcess *dlHarqProcess_p,
        /*SPR 11812 Fix Start*/
        /* + SPS_TDD_CHANGES */
        tddAckNackQueueNode  *ackNackNode_p,
        /* - SPS_TDD_CHANGES */
#ifdef KPI_STATS
        /* SPR_Fix_17928_start */
        UInt8          recvTTI,
#endif
        /* +- SPR 17777 */
        /*SPR 11812 Fix End*/
        /* + CA_TDD_HARQ_CHANGES */
        InternalCellIndex ueInternalCellIndex
	/* SPR_Fix_17928_end */
        ,InternalCellIndex  servingIndex
        ,UInt8 *bitMaskForAperiodicReport_p        
        ,UInt8         *nackFlag_p
        /* - CA_TDD_HARQ_CHANGES */
        );
STATIC void  setHarqProcessInfoForSCell(UInt16  ueIndex, UInt8 currentSubframe,
                               UInt32 currentSFNIndex, UInt8 indexSfn, 
                               UInt8 harqProcessId, /* SPR 15909 fix start */
                               tickType_t dlHarqRttExpiryTick,
                               /* SPR 15909 fix end */
                               InternalCellIndex internalCellIndex,
                               UInt32  dlAckNackSf);

STATIC void  setHarqProcessInfoForPCell(UInt16  ueIndex, UInt8 currentSubframe,
                               UInt32 currentSFNIndex, UInt8 indexSfn, 
                               UInt8 harqProcessId, /* SPR 15909 fix start */
                               tickType_t dlHarqRttExpiryTick,
                               /* SPR 15909 fix end */
                               InternalCellIndex internalCellIndex,
                               UInt32  dlAckNackSf);

STATIC void tddExpireHarqNodeOfPCell(UInt16 ueIndex, UInt32 containerIndex,
                                     UInt8 dlAckNackSubFrame, UInt8 dlDatTxSubframe,
                                     UInt8 txSFNIndex, UInt8 freeHarqOnly,
                                     InternalCellIndex internalCellIndex,
                                     UInt8 *nackFlag_p);

STATIC void tddExpireHarqNodeOfSCell(UInt16 ueIndex, UInt32 containerIndex,
                                     UInt8 dlAckNackSubFrame, UInt8 dlDatTxSubframe,
                                     UInt8 txSFNIndex, UInt8 freeHarqOnly,
                                     /* +- SPR 17777 */
                                     InternalCellIndex ueServCellIndex,
                                     UInt8 *nackFlag_p);
STATIC void tddExpireHarqNode(UInt16 ueIndex, UInt32 containerIndex,
                               UInt8 dlAckNackSubFrame, UInt8 dlDatTxSubframe,
                               UInt8 txSFNIndex, UInt8 freeHarqOnly,
                               InternalCellIndex internalCellIndex);

STATIC void checkTBsAndPutEntryInRetxQueue(DLUEContext* dlUeContext_p,
                             DLHARQProcess* dlHarqProcess_p,
                             InternalCellIndex internalCellIndex,
                             UInt8 *nackFlag_p,
                             UInt8 freeHarqOnly,
                             /* + SPS_TDD_Changes */
                             UInt32 schUsed);
/* - SPS_TDD_Changes */

void macHandleRetxQueueFailure(UInt16 ueIndex,
                               UInt8 *nackFlag_p,
                               DLRetxQueueNode retxNodeFilled);
#endif

/* EICIC +*/
/* Updating the subframe and system frame number in a global */
#define UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_HARQ_RECEIVED(internalCellIndex) \
{\
    UInt32 Subframe; \
    UInt32 Sysframe; \
    getSFAndSFN(&Sysframe, &Subframe,internalCellIndex);\
    if(eicicHarqSubframeNumber_g>Subframe) \
    	Sysframe -= 1; \
    	eicicHarqSystemframeNumber_g = Sysframe;\
}   
/* EICIC -*/

#ifdef FDD_CONFIG
/* +- SPR 17777*/
STATIC  void isHarqOnPucchRecvdOnTBOneTBTwo (
        UInt8       isHarqOnPucch, 
        UInt8       oprTb1, 
        UInt8       oprTb2, 
        DLUEContext *dlUeContext_p, 
        /* SPR 15909 fix start */
        tickType_t      globalTick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex);
/* coverity id 62977*
static  void isHarqOnPucchRecvdOnTBTwo (UInt16 ueIndex, 
        UInt32      subFrameNumber, 
        UInt8       isHarqOnPucch, 
        UInt8       oprTb2, 
        DLUEContext *dlUeContext_p, 
        tickType_t      globalTick,
        InternalCellIndex internalCellIndex);*/
                                        /* SPR 15909 fix start */
STATIC  void processDLAckForTBOne (tickType_t globalTick, 
                                        /* SPR 15909 fix end */
        DLHARQProcess *dlHarqProcess_p, 
        /* +- SPR 17777*/
        DLUEContext   *dlUeContext_p,
        InternalCellIndex internalCellIndex,
        /* SPR 12054 fix start */
        UInt32 schUsed,
        UInt8 ueServCellIndex );
        /* SPR 12054 fix end */
static  void processDLNackForTBOne (UInt16 ueIndex, 
        UInt8         maxRetxCount,
        /* +- SPR 17777*/
        DLHARQProcess *dlHarqProcess_p, 
        DLUEContext   *dlUeContext_p
        /* CA_phase2_harq_code Start */
        ,UInt8         *nackFlag_p
        /* CA_phase2_harq_code End */
        ,InternalCellIndex internalCellIndex
        );
static  void processDLNackForTBTwo (UInt16 ueIndex, 
        /* +- SPR 17777*/
        DLHARQProcess *dlHarqProcess_p, 
        DLUEContext   *dlUeContext_p
        /* CA_phase2_harq_code Start */
        ,UInt8         *nackFlag_p
        /* CA_phase2_harq_code End */
        ,InternalCellIndex internalCellIndex
        );
static  void processDLAckForTBOneNackForTBTwo (UInt16 ueIndex, 
        /* +- SPR 17777*/
        DLHARQProcess *dlHarqProcess_p, 
        DLUEContext   *dlUeContext_p
        /* CA_phase2_harq_code Start */
        ,UInt8         *nackFlag_p
        /* CA_phase2_harq_code End */
        ,InternalCellIndex internalCellIndex
        );
static  void processDLAckForTBTwoNackForTBOne (UInt16 ueIndex, 
        /* +- SPR 17777*/
        DLHARQProcess *dlHarqProcess_p, 
        DLUEContext   *dlUeContext_p
        /* CA_phase2_harq_code Start */
        ,UInt8         *nackFlag_p
        /* CA_phase2_harq_code End */
        ,InternalCellIndex internalCellIndex
        );
static  void processDLNackForTBOneTBTwo (UInt16 ueIndex, 
        /* +- SPR 17777*/
        DLHARQProcess *dlHarqProcess_p, 
        DLUEContext   *dlUeContext_p
        /* + SPR 17745 */
#ifdef KPI_STATS
        ,UInt32 recvTTI
#endif
        /* - SPR 17745 */
        /* CA_phase2_harq_code Start */
        ,UInt8         *nackFlag_p
        /* CA_phase2_harq_code End */
        ,InternalCellIndex internalCellIndex
        );
#endif

/* SPR 16843 start */
/* SPR 11402 Fix */
#ifdef KPI_STATS
STATIC  void updateKpiStatsForTotalSduAckRcvdDlperQci(DLHARQTBInfo *TBInfo,TbValue TBFlag,
                         InternalCellIndex internalCellIndex,UInt16 ueIndex);
#endif
/* SPR 11402 Fix */
/* SPR 16843 end */
/* Cyclomatic Complexity changes - ends here */

/*CYCLOMATIC_CHANGES_6.1_START*/
STATIC  void freeRLCDataBuffer(DLHARQTBInfo* dlHarqTbInfo_p,
                                     DLHARQProcess* dlHarqProcess_p);
/* SPR 16916 FIX START */
STATIC  void enqueuePendingDLMemoryPoolFreeQ(DLHARQProcess* dlHarqProcess_p, 
                                                   DLUEContext* dlUeContext_p,
                                                   /* +- SPR 17777 */
                                                   UInt8 tbIdentifier
#ifdef KPI_STATS
                                                   ,InternalCellIndex internalCellIndex
#endif
                                                   );
/* +- SPR 17777 */
/* SPR 16916 FIX END */
/*****************************************************************************
 * Function Name  :  updateBlerCW0_2
 * Inputs         :  internalCellIndex - Cell-Index at MAC
 *                   ueULContext_p - The pointer to ULUEContext which contains
 *                                 the Uplink UE Context,
                     currentDLBLERForCW0_2 - Current value of Downlink BLER 
 *                   ueServCellIndex - serving cell index
 *                   ueIndex-The UE index of the UE
 * Outputs        :  None
 * Returns        :  None
 * Description    :  Update BLER for CodeWord 0_2 
 *****************************************************************************/
void updateBlerCW0_2(InternalCellIndex internalCellIndex,ULUEContext* ueULContext_p,
                     SInt8 currentDLBLERForCW0_2,UInt8 ueServCellIndex,
                     UInt16 ueIndex)
{
    if(IS_PCELL_OF_UE(ueULContext_p , internalCellIndex))
    {
        updateDLBlerCW0_2(ueIndex,
                currentDLBLERForCW0_2, internalCellIndex);
    }
    else 
    {
        macUpdateSCellDLBlerCW0_2(ueIndex,currentDLBLERForCW0_2,
                internalCellIndex,ueServCellIndex);		
    }
}
/*Complexity End*/
/*CYCLOMATIC_CHANGES_6.1_END*/
/*****************************************************************************
 * Function Name  :  freeDLHarqProcessTBSpecific
 * Inputs         :  ueContext_p - The pointer to DLUEContext which contains
 *                                 the Downlink UE Context,
 *                   dlHarqProcess_p - The pointer to the DLHARQProcess corresponding
 *                                      to appropriate HARQ Process,
 *                   tbIdentifier - The value (TB_ONE/TB_TWO) decides which
 *                                  Tb has to be freed
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function is used when the entire Harq Process associated 
 *                   of a particular TB (TB_ONE /TB_TWO) has to be freed. 
 *****************************************************************************/

 void freeDLHarqProcessTBSpecific(
        DLUEContext*  ueContext_p,
        DLHARQProcess* dlHarqProcess_p,
        UInt8 tbIdentifier,
        InternalCellIndex internalCellIndex)
{
    DLHARQTBInfo* dlHarqTbInfo_p = PNULL;
    /*  UInt8 harqProcessId = INVALID_HARQ_ID; TDD Warning Fix*/

    /* CA_phase2_harq_code Start */
    UInt32 maxPermissibleTBSize = 0;
    UInt32 *permissibleTBSize_p  = PNULL;
    UInt8 ueServCellIndex  = 0;
    DLUESCellContext *dlUeScellContext_p = PNULL;
    /* CA_phase2_harq_code End */

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    /* CSR 51542 fix start */
    UInt8 count = 0;
    /* CSR 51542 fix end */
    /*  harqProcessId = dlHarqProcess_p->harqProcessId;/

        LOG_UT(MAC_FREEING_DLHARQ_ID,LOGDEBUG,MAC_DL_HARQ,\
        getCurrentTick(),
        ueContext_p->ueIndex,
        harqProcessId,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,""); TDD Warning Fix */
#ifdef KPI_STATS
    /* SPR 16613 fix start */
    UInt32 index = 0;
    for (index =3; index < MAX_LOGICAL_CHANNEL; index++)
    {
        dlHarqProcess_p->commonIndex[index - 3] = 0;
        dlHarqProcess_p->pdcpBytes[index - 3] = 0;
    }
    /* SPR 16613 fix end */
#endif

    /* CA_phase2_harq_code Start */

    if(IS_PCELL_OF_UE(ueContext_p, internalCellIndex))
    {
        permissibleTBSize_p  = &(ueContext_p->permissibleTBSize);
        maxPermissibleTBSize = ueContext_p->maxPermissibleTBSize;
    }
    else
    {
        for(ueServCellIndex = 1 ; ueServCellIndex <= ueContext_p->scellCount && \
                ueContext_p->dlUeScellContext_p[ueServCellIndex]; ueServCellIndex++)
        {
            dlUeScellContext_p = ueContext_p->\
                                 dlUeScellContext_p[ueServCellIndex];

            if(internalCellIndex == dlUeScellContext_p->internalCellIndex)
            {
                permissibleTBSize_p  = &(dlUeScellContext_p->permissibleTBSize);
                maxPermissibleTBSize = 
                    dlUeScellContext_p->maxPermissibleTBSize;
                break;
            }
        }
    }

    /* CA_phase2_harq_code End */
    /* SPR 13084 Fix Start */
    dlHarqProcess_p->scellActBitmask = 0;
    dlHarqProcess_p->scellDeactBitmask = 0;
    /* SPR 13084 Fix End */


    if (tbIdentifier == TB_ONE)
    {
        dlHarqProcess_p->isTBOneValid = TB_INVALID;
        dlHarqTbInfo_p = &dlHarqProcess_p->dlHARQTBOneInfo;   

        /* SPR 6333 TI_TA_FIX_START */
        RESET_TA_AVG_VALUE(ueContext_p, dlHarqTbInfo_p);
        /* SPR 6333 TI_TA_FIX_END */
        /* + SPR 22243 Fix*/
        RESET_PENDING_DRX_ACK_NACK_VALUE(ueContext_p,dlHarqTbInfo_p->taDrxFlag);
        /* - SPR 22243 Fix*/

        if (dlHarqProcess_p->transmissionBuffer_p)
        {
            /* SPR 16916 FIX START */
#ifdef KPI_STATS
            updateKpiStatsForNumOfActiveUeDlPerQci(ueContext_p->ueIndex,
                    &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,dlHarqProcess_p->harqProcessId,internalCellIndex);
#endif
            /* SPR 16916 FIX END */
            /* CSR 51542 fix start */
            if (dlHarqTbInfo_p->transmissionAndRLCData_p)
            {
                count = dlHarqProcess_p->\
                        dlHARQTBOneInfo.transmissionAndRLCData_p->lcCountTBOne; 
            }
            /* CSR 51542 fix end */
            /* SPR 5531 changes start */
            if(!(ENQUEUE_PENDING_DL_MEMORY_POOL_FREE_Q(DLHARQTBInfo,
                            (void *)(dlHarqProcess_p->transmissionBuffer_p),
                /** SPR 12777 Changes Start **/
                ueContext_p->internalCellIndex)))
                /** SPR 12777 Changes End **/
            {
                lteWarning("pushNode() failed for "
                        "pendingDLMemoryPoolFreeQ_g queue");
                msgFree(dlHarqProcess_p->transmissionBuffer_p);
            }
            /* SPR 5531 changes end */
            dlHarqProcess_p->transmissionBuffer_p = PNULL;
        }
    }
    else if (tbIdentifier == TB_TWO)
    {
        dlHarqProcess_p->isTBTwoValid= TB_INVALID;
        dlHarqTbInfo_p = &dlHarqProcess_p->dlHARQTBTwoInfo;                  

        /* SPR 6333 TI_TA_FIX_START */
        RESET_TA_AVG_VALUE(ueContext_p, dlHarqTbInfo_p);
        /* SPR 6333 TI_TA_FIX_END */
        /* + SPR 22243 Fix*/
        RESET_PENDING_DRX_ACK_NACK_VALUE(ueContext_p,dlHarqTbInfo_p->taDrxFlag);
        /* - SPR 22243 Fix*/
        if (dlHarqProcess_p->transmissionBufferTB2_p)
        {
            /* SPR 16916 FIX START */
#ifdef KPI_STATS
            updateKpiStatsForNumOfActiveUeDlPerQci(ueContext_p->ueIndex,
                    &dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,dlHarqProcess_p->harqProcessId,internalCellIndex);
#endif
            /* SPR 16916 FIX END */
            /* CSR 51542 fix start */
            if (dlHarqTbInfo_p->transmissionAndRLCData_p)
            {
                count = dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p->lcCountTBTwo; 
            }
            /* CSR 51542 fix end */
            /* SPR 5572 changes start */        
            /* SPR 5531 changes start */
            if(!(ENQUEUE_PENDING_DL_MEMORY_POOL_FREE_Q(DLHARQTBInfo,
                            (void *)(dlHarqProcess_p->transmissionBufferTB2_p),
                /** SPR 12777 Changes Start **/
                ueContext_p->internalCellIndex)))
                /** SPR 12777 Changes End **/
            {
                lteWarning("pushNode() failed for "
                        "pendingDLMemoryPoolFreeQ_g queue");
                msgFree(dlHarqProcess_p->transmissionBufferTB2_p);
            }
            /* SPR 5531 changes end */
            dlHarqProcess_p->transmissionBufferTB2_p = PNULL;
        }
        /* SPR 5572 changes end */        
    }
    else
    {
        lteWarning("Undefined tbIdentifier [%d]",tbIdentifier);
        return;
    }

    /*Soft Buffer Changes Start*/
    /* CA_phase2_harq_code Start */
    /*** Coverity 62970 Fix : Start ****/
    if (permissibleTBSize_p)
    {
        *permissibleTBSize_p += dlHarqTbInfo_p->tbSize;
        if (*permissibleTBSize_p > maxPermissibleTBSize)
        {
            *permissibleTBSize_p = maxPermissibleTBSize;
        }
    }
    /*** Coverity 62970 Fix : End ****/
    /* CA_phase2_harq_code End*/

    /*Soft Buffer Changes End*/

    /* + SPR_3939*/
    dlHarqTbInfo_p->taDrxFlag = INVALID_TA_DRX_MSG;
    dlHarqTbInfo_p->txIndicator = INVALID_TX;
    dlHarqTbInfo_p->current_TX_NB = 0;
    dlHarqTbInfo_p->irVersion = 0;
    dlHarqTbInfo_p->tbSize = 0;
    dlHarqTbInfo_p->retransmissionTBSize = 0;
    dlHarqTbInfo_p->paddingBytes = 0;
    dlHarqTbInfo_p->macScellCEFlag= INVALID_SCELL_ACT_DEACT_MSG;

    if (dlHarqTbInfo_p->transmissionAndRLCData_p)
    {
        /* SPR# 9438,9471 fix */
        if(dlHarqTbInfo_p->transmissionAndRLCData_p->numValidTBs)
        {
            dlHarqTbInfo_p->transmissionAndRLCData_p->numValidTBs--;
        }

        /* CSR 51542 fix start */
        LTE_QUEUE *sem_p = PNULL;
        RLCDataBuffer *buff = PNULL;
        UInt8 i =0;
        for(i=0; i< count; i++)
        {
            buff = &(dlHarqTbInfo_p->transmissionAndRLCData_p->rlcDataBufferArr[i]);
            sem_p =&(buff->reTxRlcPduQ); 
            semDestroy(&(sem_p->sem_lock));
        }
        /* CSR 51542 fix end */
        /* - SPR_3939*/

        freeMemPool((void*)dlHarqTbInfo_p->transmissionAndRLCData_p);
        if(dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p == 
                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p)
        {
            dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
            dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;
        }
        else
        {
            dlHarqTbInfo_p->transmissionAndRLCData_p = PNULL;
        }
    }
    /*
       The following section of code will be implemented later.

       for (i=0; i<MAX_LOGICAL_CHANNEL; i++)
       {
       dlHarqTbInfo_p->arqInfo[i].lcID = 255;
       dlHarqTbInfo_p->arqInfo[i].seqNumber = 0;
       }
     */
    /* + SPR 5844 */
#ifndef DL_HARQ_REUSE_AFTER_EIGHT_TTI
    dlHarqProcess_p->dlharqRttExpiryTick = 0;
#endif
    /* - SPR 5844 */
    dlHarqProcess_p->totalTBSize = 0;
    dlHarqProcess_p->assignedResourcesNB = 0;
    /*SPR 3778 Fix Start*/
    dlHarqProcess_p->dciFormat = MAX_NUM_DCI_FORMAT;
    /*SPR 3778 Fix End*/
    /* SPS_CHG */
    dlHarqProcess_p->isSpsOnlyPaddingPresent = 0;        
    /* SPS_CHG */


    /*  dlHarqProcess_p->harqProcessId = INVALID_HARQ_ID;TDD Warning Fix */
    dlHarqProcess_p->harqStatus = HARQ_PROCESS_FREE;

    /*  LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ, \
        "\n[%s]The DL HARQ Process ID :: %d of has been succesfully freed",__func__,
        harqProcessId);  TDD Warning Fix */
    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
}

/*****************************************************************************
 * Function Name  :   freeDLHarqProcess 
 * Inputs         :   ueContext_p - The pointer to DLUEContext which contains
 *                                  the Downlink UE Context,
 *                    dlHarqProcess_p - The pointer to the DLHARQProcess corresponding
 *                                      to appropriate HARQ Process,
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs        :   None
 * Returns        :   None
 * Description    :   This function when called will remove the entire HARQ
 *                    process of the concerned UE (whether in MIMO or Non-MIMO).
 *****************************************************************************/

 void freeDLHarqProcess(
        DLUEContext*  ueContext_p,
        DLHARQProcess* dlHarqProcess_p,
        InternalCellIndex internalCellIndex)
{
    DLHARQTBInfo* dlHarqTbInfo_p = PNULL;

    /* CA_phase2_harq_code Start */
    UInt32 maxPermissibleTBSize = 0;
    UInt32 *permissibleTBSize_p  = PNULL;
    UInt8 ueServCellIndex  = 0;
    DLUESCellContext *dlUeScellContext_p = PNULL;
    /* CA_phase2_harq_code End */


    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    /*    harqProcessId = dlHarqProcess_p->harqProcessId; 

          LOG_UT(MAC_FREEING_DLHARQ_ID,LOGDEBUG,MAC_DL_HARQ,\
          getCurrentTick(),
          ueContext_p->ueIndex,
          harqProcessId,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
          DEFAULT_FLOAT_VALUE,FUNCTION_NAME,""); TDD Warning Fix */
#ifdef KPI_STATS
    /* SPR 16613 fix start */
    UInt32 index = 0; 
    for (index =3; index < MAX_LOGICAL_CHANNEL; index++)
    {
        dlHarqProcess_p->commonIndex[index - 3] = 0;
        dlHarqProcess_p->pdcpBytes[index - 3] = 0; 
    }
    /* SPR 16613 fix end */
#endif

    /* CA_phase2_harq_code Start */
    if(IS_PCELL_OF_UE(ueContext_p, internalCellIndex))
    {
        permissibleTBSize_p  = &(ueContext_p->permissibleTBSize);
        maxPermissibleTBSize = ueContext_p->maxPermissibleTBSize;
    }
    else
    {
        for(ueServCellIndex = 1 ; ueServCellIndex <= ueContext_p->scellCount && \
                ueContext_p->dlUeScellContext_p[ueServCellIndex]; ueServCellIndex++)
        {
            dlUeScellContext_p = ueContext_p->\
                                 dlUeScellContext_p[ueServCellIndex];

            if(internalCellIndex == dlUeScellContext_p->internalCellIndex)
            {
                permissibleTBSize_p  = &(dlUeScellContext_p->permissibleTBSize);
                maxPermissibleTBSize = 
                    dlUeScellContext_p->maxPermissibleTBSize;
                break;
            }
        }
    }
    /* CA_phase2_harq_code End */
    dlHarqTbInfo_p = &dlHarqProcess_p->dlHARQTBOneInfo;                     
    /* SPR 6333 TI_TA_FIX_START */
    RESET_TA_AVG_VALUE(ueContext_p, dlHarqTbInfo_p);
    /* SPR 6333 TI_TA_FIX_END */
    /* + SPR 22243 Fix*/
    RESET_PENDING_DRX_ACK_NACK_VALUE(ueContext_p,dlHarqTbInfo_p->taDrxFlag);
    /* - SPR 22243 Fix*/
    /*Soft Buffer Changes Start*/
    /*** Coverity 63553 Fix : Start ****/
    if(permissibleTBSize_p)
    {
        *permissibleTBSize_p += dlHarqTbInfo_p->tbSize;
        if (*permissibleTBSize_p > maxPermissibleTBSize)
        {
            *permissibleTBSize_p = maxPermissibleTBSize;
        }
    }
    /*** Coverity 63553 Fix : End ****/
    /*Soft Buffer Changes End*/
    /*cyclomatic fix start*/
    /* SPR 16961 FIX START */
    /* +- SPR 17777 */
    freeDLHarqProcessForTBOne(dlHarqProcess_p,dlHarqTbInfo_p
#ifdef KPI_STATS
    ,ueContext_p->ueIndex,internalCellIndex
#endif
    );
    /* +- SPR 17777 */
    /* SPR 16961 FIX END */
    /*cyclomatic fix end*/
    /*
       We now consider TB_TWO
     */
    dlHarqProcess_p->isTBTwoValid = TB_INVALID;
    dlHarqTbInfo_p = &dlHarqProcess_p->dlHARQTBTwoInfo;
    /* SPR 6333 TI_TA_FIX_START */
    RESET_TA_AVG_VALUE(ueContext_p, dlHarqTbInfo_p);
    /* SPR 6333 TI_TA_FIX_END */
    /* + SPR 22243 Fix*/
    RESET_PENDING_DRX_ACK_NACK_VALUE(ueContext_p,dlHarqTbInfo_p->taDrxFlag);
    /* - SPR 22243 Fix*/
    /*Soft Buffer Changes Start*/
    /*** Coverity 62969 Fix : Start ****/
    if (permissibleTBSize_p)
    {
        *permissibleTBSize_p += dlHarqTbInfo_p->tbSize;
        if (*permissibleTBSize_p > maxPermissibleTBSize)
        {
            *permissibleTBSize_p = maxPermissibleTBSize;
        }
    }
    /*** Coverity 62969 Fix : End ****/
    /*Soft Buffer Changes End*/
    /*cyclomatic fix start*/
    /* SPR 16961 FIX START */
    /* +- SPR 17777 */
    freeDLHarqProcessForTBTwo(dlHarqProcess_p,dlHarqTbInfo_p
#ifdef KPI_STATS
    ,ueContext_p->ueIndex,internalCellIndex
#endif
    );
    /* +- SPR 17777 */
    /* SPR 16961 FIX END */
    /*cyclomatic fix end*/
    /*
       Now we consider the common sections of both the TBs.
     */
    /* + SPR 5844 */
#ifndef DL_HARQ_REUSE_AFTER_EIGHT_TTI
    dlHarqProcess_p->dlharqRttExpiryTick = 0;
#endif
    /* - SPR 5844 */
    dlHarqProcess_p->totalTBSize = 0;
    dlHarqProcess_p->assignedResourcesNB = 0;
    /* SPS_CHG */
    dlHarqProcess_p->isSpsOnlyPaddingPresent = 0;        
    /* SPS_CHG */

    if (dlHarqProcess_p->transmissionBuffer_p)
    {
        /* SPR 5531 changes start */
        if(!(ENQUEUE_PENDING_DL_MEMORY_POOL_FREE_Q(DLHARQTBInfo,
                        (void *)(dlHarqProcess_p->transmissionBuffer_p),
            /** SPR 12777 Changes Start **/
            ueContext_p->internalCellIndex)))
            /** SPR 12777 Changes End **/
        {
            lteWarning("pushNode() failed for "
                    "pendingDLMemoryPoolFreeQ_g queue");
            msgFree(dlHarqProcess_p->transmissionBuffer_p);
        }
        /* SPR 5531 changes end */
        dlHarqProcess_p->transmissionBuffer_p = PNULL;
    }

    /* SPR 3679 Changes Start */
    if (dlHarqProcess_p->transmissionBufferTB2_p)
    {
        /* SPR 5572 changes start */        
        /* SPR 5531 changes start */
        if(!(ENQUEUE_PENDING_DL_MEMORY_POOL_FREE_Q(DLHARQTBInfo,
                        (void *)(dlHarqProcess_p->transmissionBufferTB2_p),
            /** SPR 12777 Changes Start **/
            ueContext_p->internalCellIndex)))
            /** SPR 12777 Changes End **/
        {
            lteWarning("pushNode() failed for "
                    "pendingDLMemoryPoolFreeQ_g queue");
            msgFree(dlHarqProcess_p->transmissionBufferTB2_p);
        }
        /* SPR 5531 changes end */
        /* SPR 5572 changes end */        
        dlHarqProcess_p->transmissionBufferTB2_p = PNULL;
    }
    /* SPR 3679 Changes End */

    /* SPR 21857 changes */
    dlHarqProcess_p->pmiIndex = WIDEBAND_PMI;
    /* SPR 21857 changes */

    dlHarqProcess_p->harqProcessId = INVALID_HARQ_ID;
    dlHarqProcess_p->harqStatus = HARQ_PROCESS_FREE;

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
}


/*****************************************************************************
 * Function Name  :   freeDLHarqTB
 * Inputs         :   ueContext_p - The pointer to DLUEContext which contains
 *                                  the Downlink UE Context,
 *                    dlHarqProcess_p - The pointer to the DLHARQProcess corresponding
 *                                      to appropriate HARQ Process,
 *                    tbIdentifier - The value is (TB_ONE/TB_TWO)
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs        :   None 
 * Returns        :   None
 * Description    :   This function will free the TB resources of an HARQ Process
 *                    in accordance to the field of tbIdentifier.
 *****************************************************************************/

 void freeDLHarqTB(
        DLUEContext*  dlUeContext_p,
        DLHARQProcess* dlHarqProcess_p,
        UInt8 tbIdentifier, 
        InternalCellIndex internalCellIndex)
{

    DLHARQTBInfo* dlHarqTbInfo_p = PNULL;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");


    LOG_MAC_MSG(MAC_FREEING_DLHARQ_ID,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            dlUeContext_p->ueIndex,
            dlHarqProcess_p->harqProcessId,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
#ifdef KPI_STATS
    /* SPR 16613 fix start */
    UInt32 index = 0;
    for (index =3; index < MAX_LOGICAL_CHANNEL; index++)
    {
        dlHarqProcess_p->commonIndex[index - 3] = 0;
        dlHarqProcess_p->pdcpBytes[index - 3] = 0;
    }
    /* SPR 16613 fix end */
#endif

    if (tbIdentifier == TB_ONE)
    {
        dlHarqProcess_p->isTBOneValid = TB_INVALID;
        dlHarqTbInfo_p = &dlHarqProcess_p->dlHARQTBOneInfo;
        /* SPR 6333 TI_TA_FIX_START */
        RESET_TA_AVG_VALUE(dlUeContext_p, dlHarqTbInfo_p);
        /* SPR 6333 TI_TA_FIX_END */
        /* + SPR 22243 Fix*/
        RESET_PENDING_DRX_ACK_NACK_VALUE(dlUeContext_p,dlHarqTbInfo_p->taDrxFlag);
        /* - SPR 22243 Fix*/
        
        /*CYCLOMATIC_CHANGES_6.1_START*/
        /* SPR 16916 FIX START */
        /* +- SPR 17777 */
        enqueuePendingDLMemoryPoolFreeQ(dlHarqProcess_p, dlUeContext_p,
                                        tbIdentifier
#ifdef KPI_STATS
                                        ,internalCellIndex
#endif
                                        );
        /* +- SPR 17777 */
        /* SPR 16916 FIX END */
        /*CYCLOMATIC_CHANGES_6.1_END*/
    }
    else if (tbIdentifier == TB_TWO)
    {
        dlHarqProcess_p->isTBTwoValid = INVALID;
        dlHarqTbInfo_p = &dlHarqProcess_p->dlHARQTBTwoInfo;
        /* SPR 6333 TI_TA_FIX_START */
        RESET_TA_AVG_VALUE(dlUeContext_p, dlHarqTbInfo_p);
        /* SPR 6333 TI_TA_FIX_END */
        /* + SPR 22243 Fix*/
        RESET_PENDING_DRX_ACK_NACK_VALUE(dlUeContext_p,dlHarqTbInfo_p->taDrxFlag);
        /* - SPR 22243 Fix*/
        
        /*CYCLOMATIC_CHANGES_6.1_START*/
        /* SPR 16916 FIX START */
        enqueuePendingDLMemoryPoolFreeQ(dlHarqProcess_p, dlUeContext_p,
                /* +- SPR 17777 */
                                        tbIdentifier
#ifdef KPI_STATS
                                        ,internalCellIndex
#endif
                                        );
        /* +- SPR 17777 */
        /* SPR 16916 FIX END */
        /*CYCLOMATIC_CHANGES_6.1_END*/
    }
    else
    {
        lteWarning("Invalid TB Identifier - tbIdentifier [%d]",tbIdentifier);
        return;
    }
    /*Soft Buffer Changes Start*/
    if(IS_PCELL_OF_UE(dlUeContext_p, internalCellIndex))
    {
        dlUeContext_p->permissibleTBSize += dlHarqTbInfo_p->tbSize;
        if ( dlUeContext_p->permissibleTBSize >  dlUeContext_p->maxPermissibleTBSize)
        {
            dlUeContext_p->permissibleTBSize = dlUeContext_p->maxPermissibleTBSize; 
        }
    }
    else
    {
        dlUeContext_p->dlUeScellContext_p[1]->permissibleTBSize += dlHarqTbInfo_p->tbSize;
        if ( dlUeContext_p->dlUeScellContext_p[1]->permissibleTBSize >  
                dlUeContext_p->dlUeScellContext_p[1]->maxPermissibleTBSize)
        {
            dlUeContext_p->dlUeScellContext_p[1]->permissibleTBSize = 
                dlUeContext_p->dlUeScellContext_p[1]->maxPermissibleTBSize; 
        }
    }
    /*Soft Buffer Changes End*/

    dlHarqTbInfo_p->taDrxFlag = INVALID_TA_DRX_MSG;
    dlHarqTbInfo_p->txIndicator = INVALID_TX;
    dlHarqTbInfo_p->current_TX_NB = 0;
    dlHarqTbInfo_p->irVersion = 0;
    dlHarqTbInfo_p->tbSize = 0;
    dlHarqTbInfo_p->retransmissionTBSize = 0;
    dlHarqTbInfo_p->paddingBytes = 0;
    dlHarqTbInfo_p->macScellCEFlag= INVALID_SCELL_ACT_DEACT_MSG;
    /*
       WE now check whether all the TB of this HARQ Process has been already
       freed then we will free the rlcDataBufferArr_p
     */ 
     
     /*CYCLOMATIC_CHANGES_6.1_START*/
     freeRLCDataBuffer(dlHarqTbInfo_p,dlHarqProcess_p);
     /*CYCLOMATIC_CHANGES_6.1_END*/
    /*
       The following section of code will be implemented later.

       for (i=0; i<MAX_LOGICAL_CHANNEL; i++)
       {
       dlHarqTbInfo_p->arqInfo[i].lcID = 255;
       dlHarqTbInfo_p->arqInfo[i].seqNumber = 0;
       }
     */
    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
}

#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  :   dlHarqTimerStart
 * Inputs             ueIndex - The UE index of the UE for which a HARQ Timer  
 *                              has to be started,
 *                    harqProcessId - The the harq process ID for which the timer 
 *                                    has to  be started,
 *                    frameNumber - The sub frame number for which the UE data
 *                                  will be on the PHYSICAL layer,
 *                    dlHarqRttExpiryTick - The RTT Timer expiry tick,
 *                    schUsed - Shared Channel used
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs        :   None
 * Returns        :   MAC_FAILURE/MAC_SUCCESS
 * Description    :   This function will start the HARQ Timer functionality.
 *****************************************************************************/


 MacRetType dlHarqTimerStart( 
        UInt16  ueIndex,
        UInt8   harqProcessId,
        UInt32  frameNumber,
        /* SPR 15909 fix start */
        tickType_t  dlHarqRttExpiryTick,
        /* SPR 15909 fix end */
        SchType schUsed,
        InternalCellIndex internalCellIndex)
{
    UInt8 dlHarqEntityTimerArrIndex = 0;
    DLHarqTimerEntityList* dlHarqEntityTimerList_p = PNULL;
    DLHarqTimerNode* dlharqTimerNode_p = PNULL;
    DLUEContext*  ueContext_p = PNULL;       
    DLUEContextInfo* dlUeContextInfo_p = PNULL;

    /* CA_phase2_harq_code Start */
    DLHARQEntity     *dlHarqContext_p = PNULL;
    InternalCellIndex ueServCellIndex  = 0;
    DLUESCellContext *dlUeScellContext_p = PNULL;
    /* CA_phase2_harq_code End*/


    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    /*
       In this function we do not impose an extra check on the harqProcessId for its
       value as it is being assumed that the corresponding value has been received 
       from the checkFreeDLHarqProcess ( ).
     */
    dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];

    ueContext_p = dlUeContextInfo_p->dlUEContext_p;

    /* CA_phase2_harq_code Start */

    dlHarqEntityTimerList_p = 
        &(dlHarqGlobals_g[ueContext_p->internalCellIndex].dlHarqTimerEntityList[frameNumber]);

    if(internalCellIndex == ueContext_p->internalCellIndex)
    {
        dlHarqContext_p = ueContext_p->dlHarqContext_p;
    }
    else
    {
        for(ueServCellIndex = 1 ; ueServCellIndex <= ueContext_p->scellCount && \
           ueContext_p->dlUeScellContext_p[ueServCellIndex]; ueServCellIndex++)
        {
            dlUeScellContext_p = ueContext_p->\
                                 dlUeScellContext_p[ueServCellIndex];

            if(internalCellIndex == dlUeScellContext_p->internalCellIndex)
            {
                dlHarqContext_p = dlUeScellContext_p->ScelldlHarqContext_p;
                break;
            }
        }
    }
    /*** Coverity 62971 Fix : Start ****/
    if (!dlHarqContext_p)
    {
        return MAC_FAILURE;
    }
    /*** Coverity 62971 Fix : End ****/
    /* CA_phase2_harq_code End */
 
 /*
NOTE:- The value of dlHarqEntityTimerList_p->count was initialised to 0
(invalid value) and hence we have to increase it after it is used as 
array index, on the first occasion on the other occasion it just gives
the proper array index.
     */ 

    dlHarqEntityTimerArrIndex = dlHarqEntityTimerList_p->count;

    /* SPR 15122 Start */
    if ( dlHarqEntityTimerList_p->count >= MAX_DL_UE_SCHEDULED  )
    {
        LOG_MAC_MSG(MAC_EXPIRE_DL_HARQ_TIMER_NODE, LOGINFO, MAC_DL_HARQ,
                getCurrentTick(), frameNumber, harqProcessId, ueIndex,
                dlHarqEntityTimerArrIndex, 0, 0,0, __func__, "COUNT_MORE_THAN_MAX_DL_UE_SCHEDULED");

        return MAC_FAILURE;
    }
    /* SPR 15122 End */

    LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,"[%s]dlHarqEntityTimerList_p->count  %d,"\
            "dlHarqEntityTimerArrIndex %d\n",
            __func__,dlHarqEntityTimerList_p->count,dlHarqEntityTimerArrIndex);

    dlharqTimerNode_p = \
                        &(dlHarqEntityTimerList_p->dlHarqTimerNodeArr[dlHarqEntityTimerArrIndex]);

    dlharqTimerNode_p->ueIndex = ueIndex;
    dlharqTimerNode_p->harqProcessId = harqProcessId;
    dlharqTimerNode_p->isValid = HARQ_TIMER_NODE_VALID;
    dlharqTimerNode_p->schUsed = schUsed;
    /* CA_phase2_harq_code Start */

    dlHarqContext_p->dlHarqTimerNodeArrrayIndex[frameNumber] 
        = dlHarqEntityTimerArrIndex;
    dlHarqContext_p->harqProcess[harqProcessId].dlharqRttExpiryTick 
        = dlHarqRttExpiryTick;
    /* CA_phase2_harq_code End */

    dlHarqEntityTimerList_p->count++;

    LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,
            "[%s]The dlharqTimerNode_p Timer is started for UE ID %d"\
            " Harq ID %d, expiry Tick %d  \n The dlharqTimerNode_p Timer frameNumber %d"\
            " dlHarqEntityTimerArrIndex %d,UE_Context_dlHarqTimerNodeArrrayIndex %d,"\
            " frameNum %d dlharqTimerNode_p->isValid = %d \n",
            __func__,ueIndex, harqProcessId,dlHarqRttExpiryTick,
            frameNumber,dlHarqEntityTimerArrIndex,
            ueContext_p->dlHarqContext_p->dlHarqTimerNodeArrrayIndex[frameNumber],
            frameNumber,dlharqTimerNode_p->isValid );

    LOG_UT(MAC_DL_HARQ_TIMER_START_INFO_1,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
            ueIndex,\
            harqProcessId,\
            frameNumber,\
            dlHarqRttExpiryTick,dlharqTimerNode_p->isValid,\
            (UDouble32)dlHarqEntityTimerArrIndex, (UDouble32)dlHarqEntityTimerList_p->count,\
            FUNCTION_NAME,"");

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),\
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;

}       

/* + SPR 17733 */
/*****************************************************************************
 * Function Name  :   dlHarqTimerStartForMsg4
 * Inputs         :   ueIndex - The UE index of the UE for which a HARQ Timer  
 *                    has to be started. 
 *                :   harqProcessId - The the harq process ID for which the timer 
 *                    has to  be started.
 *                :   frameNumber - The sub frame number for which the UE data
 *                    will be on the PHYSICAL layer.
 *                :   dlHarqRttExpiryTick - The RTT Timer expiry tick.
                      internalCellIndex - Cell-Index at MAC
 * Outputs        :   None 
 * Returns        :   MAC_FAILURE/MAC_SUCCESS
 * Description    :   This function will start the HARQ Timer functionality.
 *****************************************************************************/
 MacRetType dlHarqTimerStartForMsg4( UInt16 ueIndex, UInt8 harqProcessId,
        UInt32  frameNumber,
        /* SPR 15909 fix start */
        tickType_t  dlHarqRttExpiryTick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
{

/*SPR 21653 changes start*/
    UInt16 dlHarqEntityTimerArrIndex = 0;
/*SPR 21653 changes end*/
    DLHarqTimerEntityForMsg4List* dlHarqEntityTimerList_p = PNULL;
    DLHarqTimerForMsg4Node* dlharqTimerNode_p = PNULL;
    TempCRNTICtx * tempCRNTICtx_p = PNULL;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    dlHarqEntityTimerList_p = &(dlHarqGlobals_g[internalCellIndex].dlHarqTimerEntityForMsg4List[frameNumber]);

    dlHarqEntityTimerArrIndex = dlHarqEntityTimerList_p->count;

    dlharqTimerNode_p = 
        &(dlHarqEntityTimerList_p->dlHarqTimerNodeForMsg4Arr[dlHarqEntityTimerArrIndex]);
    tempCRNTICtx_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;

    dlharqTimerNode_p->ueIndex = ueIndex;
    dlharqTimerNode_p->harqProcessId = harqProcessId;
    dlharqTimerNode_p->isValid = HARQ_TIMER_NODE_VALID;
    tempCRNTICtx_p->dlHarqCtx_p->dlHarqTimerNodeArrrayIndex[frameNumber] = 
        dlHarqEntityTimerArrIndex;
    tempCRNTICtx_p->dlHarqCtx_p->harqProcess[harqProcessId].dlharqRttExpiryTick= 
        dlHarqRttExpiryTick;
    dlHarqEntityTimerList_p->count++;

    LOG_UT(MAC_DL_HARQ_TIMER_START_INFO_1, LOGDEBUG, MAC_DL_HARQ,
            getCurrentTick(), ueIndex, harqProcessId, frameNumber,
            dlHarqRttExpiryTick, 0, 0,0, __func__ ,"");

    LOG_UT(MAC_DL_HARQ_TIMER_START_INFO_2,LOGDEBUG,MAC_DL_HARQ,
            getCurrentTick(),
            dlharqTimerNode_p->isValid ,
            dlHarqEntityTimerArrIndex,
            dlHarqEntityTimerList_p->count,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}
/* - SPR 17733 */
#endif

#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  : tddHarqRttTimerFactor
 * Inputs         : subFrame - The sub frame number for which the UE data will
 *                             be on the PHYSICAL layer.
 *                  cellId - cellIndex for the cell for which the function
 *                           needs to be called.
 * Outputs        : None
 * Returns        : RTT Timer factor value.
 * Description    : This function will return the RTT timer value for different
 *                  TDD Configs and different Sub-frames.
 ****************************************************************************/
/* SPR 12198 fix start */
/* SPR 23393 Changes Start */
 UInt8 tddHarqRttTimerFactor( UInt8 subFrame, InternalCellIndex cellId )
/* SPR 23393 Changes End */
{
    return (tddPdschTxMapOfDLHarq_p[cellId][subFrame].harqRttTimerFactor ); 
}
/* SPR 12198 fix end */

/* SPR 11230 fix start */
/* CA TDD Changes Start */
/*****************************************************************************
 * Function Name  :  resetEntryInHarqSentMap
 * Inputs         :  mapIdx, internalCellIndex
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function will reset the harqSentMap_g for DL HARQ
 *****************************************************************************/
/* SPR 23393 Changes Start */
 void resetEntryInHarqSentMap( UInt8 mapIdx, InternalCellIndex internalCellIndex)
/* SPR 23393 Changes End */
{
    /* Reset the TPC map entry before actually filling in */
    harqSentMap_g[internalCellIndex][mapIdx].numUe = 0;
    harqSentMap_g[internalCellIndex][mapIdx].tick  = 0;
}

/*****************************************************************************
 * Function Name  :  putEntryInHarqSentMap
 * Inputs         :  currentTick, mapIdx, ueIndex, indexSfn, dlAckNackSf,
 *                   internalCellIndex
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function will update the harqSentMap_g for DL HARQ
 *****************************************************************************/
 /* SPR 15909 fix start */
void putEntryInHarqSentMap( tickType_t currentTick, UInt8 mapIdx, 
        UInt16 ueIndex, UInt8 indexSfn, UInt8 dlAckNackSf,
		InternalCellIndex internalCellIndex)
/* SPR 15909 fix end */
{
    UInt8 count = 0;
    /* Reset the num of UE if the function is called for first time */
    if( currentTick != harqSentMap_g[internalCellIndex][mapIdx].tick )
    {
        harqSentMap_g[internalCellIndex][mapIdx].numUe = 0;
    }
    count = harqSentMap_g[internalCellIndex][mapIdx].numUe;
    harqSentMap_g[internalCellIndex][mapIdx].tick = currentTick;
    harqSentMap_g[internalCellIndex][mapIdx].harqSentInfo[count].ueIndex = ueIndex;
    harqSentMap_g[internalCellIndex][mapIdx].harqSentInfo[count].indexSfn = indexSfn;
    harqSentMap_g[internalCellIndex][mapIdx].harqSentInfo[count].dlAckNackSf = dlAckNackSf;
    harqSentMap_g[internalCellIndex][mapIdx].numUe++;
}
/* CA TDD Changes End */
/* SPR 11230 fix end */



/*****************************************************************************
 * Function Name  :   tddDlHarqTimerStart
 * Inputs         :   ueIndex - The UE index of the UE for which a HARQ Timer
 *                              has to be started,
 *                    harqProcessId - The the harq process ID for which the timer
 *                                    has to  be started,
 *                    currentSubframe- The sub frame number for which the UE data
 *                                     will be on the PHYSICAL layer,
 *                    dlHarqRttExpiryTick - The RTT Timer expiry tick,
 *                    sysFrameNumber - current system frame number
 *                    internalCellIndex - Cell-Index at MAC
 * Outputs        :
 * Returns        :   MAC_FAILURE/MAC_SUCCESS
 * Description    :   This function will start the HARQ Timer functionality.
 *****************************************************************************/

/** CA-TDD Changes Start **/
 MacRetType tddDlHarqTimerStart( 
        UInt16  ueIndex,
        UInt8   harqProcessId,
        UInt32  currentSubframe,
        /* SPR 15909 fix start */
        tickType_t  dlHarqRttExpiryTick,
        /* SPR 15909 fix end */
        UInt16  sysFrameNumber,
        /* + SPS_TDD_Changes */
        UInt8   schUsed,
        /* - SPS_TDD_Changes */
        InternalCellIndex internalCellIndex
        )
/** CA-TDD Changes End **/
{

    UInt8 dlAckNackSf   = 0;      
    UInt8 sfnFactor     = 0;  
    UInt8 indexSfn      = 0;  
    /* SPR 11230 fix start */
    UInt8 mapIdx = dlHarqRttExpiryTick % MAX_HARQ_CONTAINER_MAP_SIZE; 
    /* SPR 11230 fix end */

    DLUEContext*  ueContext_p           = PNULL;       
    DLUEContextInfo* dlUeContextInfo_p  = PNULL;
    TDDHarqTimerExpiryListNode  *node_p = PNULL;
    UInt8 currentSFNIndex = 0;


    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    LTE_MAC_UT_LOG(LOG_DETAIL,Log_DL_HARQ,"[%s] C_TTI [%d] SFN [%d] SF[%d] hpId [%d] ",
            __func__, dlHarqRttExpiryTick, 
            sysFrameNumber, currentSubframe, harqProcessId, ueIndex);

    /*
       In this function we do not impose an extra check on the harqProcessId for its
       value as it is being assumed that the corresponding value has been received 
       from the checkFreeDLHarqProcess ( ).
     */
    dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
    ueContext_p       = dlUeContextInfo_p->dlUEContext_p;
	/* + SPS_TDD_Changes */
	ueContext_p->dlHarqContext_p->harqProcess[harqProcessId].schUsed = schUsed;
	/* - SPS_TDD_Changes */

    /* + CA_TDD_HARQ_CHANGES */
    /** CA-TDD Changes Start **/
    dlAckNackSf =  tddPdschTxMapOfDLHarq_p[internalCellIndex][currentSubframe].dlAckNackSf;
    sfnFactor   =  tddPdschTxMapOfDLHarq_p[internalCellIndex][currentSubframe].sfnFactor;
    /** CA-TDD Changes End **/

    /*
     * fetch the AckNackContainerIndex & harqContainer Index 
     * for starting the harq timer.
     */

    getContainerIdxForDlTimer ( sysFrameNumber,
		    currentSubframe,
		    sfnFactor,
		    &indexSfn    /*ackNackContainerIdx*/,
		    &currentSFNIndex /*hpIdContainerIdx*/
		    );
    if(IS_PCELL_OF_UE(ueContext_p, internalCellIndex))
    {  
          ueContext_p->tempRetxNode.isNodeInvalid = TRUE;

          setHarqProcessInfoForPCell(ueIndex, currentSubframe, currentSFNIndex,
                indexSfn, harqProcessId, dlHarqRttExpiryTick, internalCellIndex,
                dlAckNackSf);
    }
    else
    {
          setHarqProcessInfoForSCell(ueIndex, currentSubframe, currentSFNIndex,
                 indexSfn, harqProcessId, dlHarqRttExpiryTick, internalCellIndex,
                 dlAckNackSf);
    }
    /* If timer is expired for Pcell then node will be inserted here.
     * If Scell is also scheduled and expired then same node will
     * be filled for scell as node address is saved in UE context.
     * If only Scell scheduled then also node will be inserted here.
     */

    if ((FALSE == ueContext_p->tempRetxNode.isNodeInvalid) && 
        (PNULL == ueContext_p->retxQueueNode_p[currentSubframe]) &&
        (MAC_FAILURE == macPushHarqNackInfoToRetxQueue(ueContext_p,
             ueContext_p->tempRetxNode,ueContext_p->nackFlag,
             ueContext_p->internalCellIndex, 
             currentSubframe)))
    {
        macHandleRetxQueueFailure(ueIndex,ueContext_p->nackFlag,
                ueContext_p->tempRetxNode);
    }
    ueContext_p->tempRetxNode.isNodeInvalid = TRUE;
    ueContext_p->nackFlag[PCELL_SERV_CELL_IX] = FALSE;
    ueContext_p->nackFlag[START_SCELL_INDEX] = FALSE;
    /* - CA_TDD_HARQ_CHANGES */

    /* SPR 11230 fix start */
    /* Put the entry in HARQ MAP for the error handling */
    putEntryInHarqSentMap( dlHarqRttExpiryTick, mapIdx, ueIndex, indexSfn,
            dlAckNackSf ,internalCellIndex);
    /* SPR 11230 fix end */


    /* Insert the node for harq timer expiry */
    if (!(ueContext_p->tddHarqTimerExpiryNode[indexSfn][dlAckNackSf]))
    {
        GET_MEM_FROM_POOL(TDDHarqTimerExpiryListNode,node_p,\
                      sizeof(TDDHarqTimerExpiryListNode), PNULL);
        
        /*+COVERITY 5.3.0 - 25307*/
        if ( PNULL != node_p )
        /*-COVERITY 5.3.0 - 25307*/
        {
            node_p->ueIndex = ueIndex;
            /* + SPS_TDD_Changes*/
            node_p->schUsed = schUsed;
            /* - SPS_TDD_Changes*/

            /* SPR 11230 fix start */
            node_p->isErrForNode = LTE_FALSE;
            /* SPR 11230 fix end */

            /** CA-TDD Changes Start **/
            listInsertNode(
               &(tddDlHarqTimerEntityList_g[ueContext_p->internalCellIndex][indexSfn][dlAckNackSf].\
                        dlExpiryList_p),&(node_p->nodeAnchor));
            /** CA-TDD Changes End **/

            /* Store the reference in the dl ue context.*/ 
            ueContext_p->tddHarqTimerExpiryNode[indexSfn][dlAckNackSf] = node_p;

	/* SPR 20636 Changes Start*/
            LOG_MAC_MSG(MAC_TDD_DL_HARQ_TIMER_START_INFO_2,LOGDEBUG,MAC_DL_HARQ,\
                    getCurrentTick(),\
                    sysFrameNumber,\
                    ((ADDR)node_p),\
                     ueIndex,\
                    ((ADDR)ueContext_p->tddHarqTimerExpiryNode[indexSfn][dlAckNackSf]),\
                    DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
	/* SPR 20636 Changes End*/
        }
    }
    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}       
#endif


#ifdef FDD_CONFIG
/************************************************************************
 * Function Name  :   dlHarqTimerStop 
 * Inputs         :   ueContext_p - A pointer to the DLUEContext,
 *                    frameNumber - The sub frame number for which the UE data
 *                                 was sent on the PHYSICAL layer
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :   None
 * Returns        :   MAC_SUCESS/MAC_FAILURE.
 * Description    :   This functon will stop the HARQ Timer functionality and
 *                    update the relevent fields of the DL UE Context.
 ************************************************************************/


 MacRetType dlHarqTimerStop(
        DLUEContext* ueContext_p,
        UInt32     frameNumber,
        InternalCellIndex internalCellIndex) 
{

    DLHarqTimerEntityList* dlHarqEntityTimerList_p = PNULL;
    DLHarqTimerNode* dlharqTimerNode_p = PNULL;
    UInt8 dlHarqTimerNodeArrIndex = -1;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
                 DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                 DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    dlHarqEntityTimerList_p = &(dlHarqGlobals_g[internalCellIndex].dlHarqTimerEntityList[frameNumber]);


    if (!dlHarqEntityTimerList_p->count)
    {
        /*
           The count is Zero symbolysing that there is no nodes avaliable in the 
           DL HARQ TIMER List Array.
         */
        LOG_UT(MAC_FUNCTION_EXIT,LOGWARNING,MAC_DL_HARQ,\
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"TimerList_p->count is Zero");
        return MAC_FAILURE;
    }
    dlHarqTimerNodeArrIndex = \
                              ueContext_p->dlHarqContext_p->dlHarqTimerNodeArrrayIndex[frameNumber];

    if (dlHarqTimerNodeArrIndex == INVALID_COUNT)
    {
        LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"dlHarqTimerNodeArrIndex is Invalid");
        return MAC_FAILURE;
    }
    dlharqTimerNode_p = \
                        &(dlHarqEntityTimerList_p->dlHarqTimerNodeArr[dlHarqTimerNodeArrIndex]);          
    if (dlharqTimerNode_p->isValid) 
    { 
        dlharqTimerNode_p->isValid = HARQ_TIMER_NODE_INVALID;
        /*
           We now fill the array dlHarqTimerNodeIndex with an invalid entry.
         */
        ueContext_p->dlHarqContext_p->dlHarqTimerNodeArrrayIndex[frameNumber] = \
                                                                                INVALID_COUNT;
        dlHarqEntityTimerList_p->count--;
    }
    else
    {
        LOG_MAC_MSG(MAC_TDD_DL_HARQ_TIMER_NODE_ISVALID_IS_NOT_SET,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
            dlharqTimerNode_p->isValid,\
            dlHarqEntityTimerList_p->count,\
            ueContext_p->dlHarqContext_p->dlHarqTimerNodeArrrayIndex[frameNumber],\
            frameNumber,\
            DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    } 
    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}
#endif
/* CA_phase2_harq_code Start */
/*****************************************************************************
 * Function Name  :  macHandleRetxQueueFailure
 * Inputs         :  ueIndex ,nackFlag_p,retxNodeFilled
 * Outputs        :  void
 * Returns        :  void.
 * Description    :  This function will handle failure case of 
 *                   pushing retransmission node into Retransmission Queue.
 *                   Harq TB which needs to be retransmiited will be send to
 *                   RLC and their reTransmission Count will be set
 *                   to MaxReTransmission Count.
 *****************************************************************************/
void macHandleRetxQueueFailure(UInt16 ueIndex,
                               UInt8 *nackFlag_p,
                               DLRetxQueueNode retxNodeFilled)
{
    DLUEContext*  dlUeContext_p = PNULL;        
    DLHARQProcess* dlHarqProcess_p = PNULL;

    UInt8   harqReTxCount  = 0;
    UInt8   ueCellTravesId  = 0;
    UInt8   maxReTxCount = 0;
    UInt8   arrIndex = 0;
    UInt8   harqID = 0;

    InternalCellIndex cellIndex = 0;


    dlUeContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

    for(ueCellTravesId = 0;ueCellTravesId < MAX_NUM_CELL ; ueCellTravesId++)
    {
        if(nackFlag_p[ueCellTravesId] == FALSE)
        {
            /*
             * For this cell there are no retrans TB's
             *
             */
            continue;
        }
        else
        {
            if(!ueCellTravesId)
            {
                harqID = retxNodeFilled.harqProcessId;
                cellIndex = dlUeContext_p->internalCellIndex;
                dlHarqProcess_p =
                    &(dlUeContext_p->dlHarqContext_p->harqProcess[harqID]);
            }
            else
            {
                arrIndex  = ueCellTravesId - 1;
                harqID = retxNodeFilled.sCellRetxInfo[arrIndex].harqProcessId;
                cellIndex = dlUeContext_p->dlUeScellContext_p[ueCellTravesId]->\
                internalCellIndex;
                            
                dlHarqProcess_p = 
                    &(dlUeContext_p->dlUeScellContext_p[ueCellTravesId]->
                            ScelldlHarqContext_p->harqProcess[harqID]);
            }

            maxReTxCount = cellSpecificParams_g.\
                    cellConfigAndInitParams_p[cellIndex]->cellParams_p->\
                    maxHarqRetransmission - 1;

            if(dlHarqProcess_p->isTBOneValid)
            {
                harqReTxCount = dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB;
                if(harqReTxCount < maxReTxCount && 
                        LTE_FALSE == dlHarqProcess_p->isSpsOnlyPaddingPresent)
                {
                    /* SPR 11402 fix */
                    sendARQIndicationToRLC(
                                           ueIndex,
                                            &dlHarqProcess_p->dlHARQTBOneInfo,
                                            TB_ONE,cellIndex);
                    /* SPR 11402 fix */
                    freeDLHarqProcessTBSpecific
                    (dlUeContext_p,dlHarqProcess_p,TB_ONE,cellIndex);
                }

            }
            if(dlHarqProcess_p->isTBTwoValid)
            {
                harqReTxCount = dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB;
                if(harqReTxCount < maxReTxCount) 
                {
                    /* SPR 11402 fix */
                    sendARQIndicationToRLC(
                            ueIndex,&dlHarqProcess_p->dlHARQTBTwoInfo,
                             TB_TWO,cellIndex);
                    /* SPR 11402 fix */
                    freeDLHarqProcessTBSpecific
                    (dlUeContext_p,dlHarqProcess_p,TB_TWO,cellIndex);
                }
            }

        }

    }

}
#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  :  macProcessExpireHarqNode
 * Inputs         :  dlHarqProcess_p ,maxRetxCount,tb1Valid,tb2Valid,internalCellIndex
 * Outputs        :  nackFlag_p
 * Returns        :  void.
 * Description    :  This function checks harq retransmissions of valid TB 
 *                   with maxReTxCount.If retransmissions reaches 
 *                   maxReTxCount then ARQ Indication will be given to 
 *                   RLC else harq info will be filled into nackFlag.
 *****************************************************************************/
void macProcessExpireHarqNode(UInt16 ueIndex,
        DLHARQProcess* dlHarqProcess_p,
        UInt8 maxRetxCount,
        UInt8 tb1Valid,
        UInt8 tb2Valid,
        UInt8 *nackFlag_p,
        InternalCellIndex internalCellIndex)
{

    DLUEContext*  dlUeContext_p = PNULL;        
    DLHARQTBInfo* dlHARQTBOneInfo_p = PNULL;
    DLHARQTBInfo* dlHARQTBTwoInfo_p = PNULL;

    UInt8         harqReTxCount  = 0;
    /*SPR 12744 Start*/
    UInt8         harqReTxCountTBTwo = 0;
    /*SPR 12744 End*/

   
    dlUeContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;

    /*SPR 12744 Start*/
    if(LTE_TRUE == dlHarqProcess_p->isSpsOnlyPaddingPresent)
    {
        /*Free Both TBs by setting nackFlag_p FALSE */
        return;
    }
    if((TRUE == tb1Valid) && (TRUE == tb2Valid))
    {
        dlHARQTBOneInfo_p = &(dlHarqProcess_p->dlHARQTBOneInfo);
        harqReTxCount = dlHARQTBOneInfo_p->current_TX_NB;

        dlHARQTBTwoInfo_p = &(dlHarqProcess_p->dlHARQTBTwoInfo);
        harqReTxCountTBTwo = dlHARQTBTwoInfo_p->current_TX_NB;

        if((harqReTxCount < maxRetxCount) && (harqReTxCountTBTwo < maxRetxCount))
        {
            /*
             *Both TBs need to be reTransmitted 
             */
            *nackFlag_p = TRUE;
        }
        else if ((harqReTxCount < maxRetxCount) && !(harqReTxCountTBTwo < maxRetxCount))
        {
            /*
             *TBOne needs to be reTransmitted 
             */
            *nackFlag_p = TRUE;

            /*TBTwo Needs to be Freed After Sending ARQ Indication*/
            CHECK_TADRXFLAG_RLC_TX_BUFF(dlHARQTBTwoInfo_p->taDrxFlag ,&(dlHarqProcess_p->dlHARQTBTwoInfo), dlHARQTBTwoInfo_p->macScellCEFlag)
            {
                /*
                 *The reTransmit count of TB reaches 
                 * max ReTrans Threshold 
                 */
                sendARQIndicationToRLC(
                        ueIndex,&(dlHarqProcess_p->dlHARQTBTwoInfo),TB_TWO
                        ,internalCellIndex);
            }
            /*Free Valid TBTwo*/
            freeDLHarqTB(dlUeContext_p,dlHarqProcess_p,TB_TWO,internalCellIndex);

        }
        else if (!(harqReTxCount < maxRetxCount) && (harqReTxCountTBTwo < maxRetxCount))
        {
            /*
             *TBTwo needs to be reTransmitted 
             */
            *nackFlag_p = TRUE;

            /*TBOne Needs to be Freed After Sending ARQ Indication*/
            CHECK_TADRXFLAG_RLC_TX_BUFF(dlHARQTBOneInfo_p->taDrxFlag ,&(dlHarqProcess_p->dlHARQTBOneInfo), dlHARQTBOneInfo_p->macScellCEFlag)
            {
                /*
                 *The reTransmit count of TB reaches 
                 * max ReTrans Threshold 
                 */
                sendARQIndicationToRLC(
                        ueIndex,&(dlHarqProcess_p->dlHARQTBOneInfo),
                        TB_ONE,internalCellIndex);
            }
            /*Free Valid TBOne*/
            freeDLHarqTB(dlUeContext_p,dlHarqProcess_p,TB_ONE,internalCellIndex);

        }
        else
        {
            /*Both TBs Need to be Freed After Sending ARQ Indication*/
            CHECK_TADRXFLAG_RLC_TX_BUFF(dlHARQTBOneInfo_p->taDrxFlag ,&(dlHarqProcess_p->dlHARQTBOneInfo), dlHARQTBOneInfo_p->macScellCEFlag)
            {
                /*
                 *The reTransmit count of TB reaches 
                 * max ReTrans Threshold 
                 */
                sendARQIndicationToRLC(
                        ueIndex,&(dlHarqProcess_p->dlHARQTBOneInfo),
                        TB_ONE,internalCellIndex);
            }
            CHECK_TADRXFLAG_RLC_TX_BUFF(dlHARQTBTwoInfo_p->taDrxFlag ,&(dlHarqProcess_p->dlHARQTBTwoInfo), dlHARQTBTwoInfo_p->macScellCEFlag)
            {
                /*
                 *The reTransmit count of TB reaches 
                 * max ReTrans Threshold 
                 */
                sendARQIndicationToRLC(
                        ueIndex,&(dlHarqProcess_p->dlHARQTBTwoInfo),TB_TWO
                        ,internalCellIndex);
            }
            /*Free Both the Valid TBs*/
            *nackFlag_p = FALSE;
            return;
        }
    }
    else
    {
    /*SPR 12744 End*/
        if(TRUE == tb1Valid) 
        {
            dlHARQTBOneInfo_p = &(dlHarqProcess_p->dlHARQTBOneInfo);
            harqReTxCount = dlHARQTBOneInfo_p->current_TX_NB;

            /*
             * In case of NON SPS Scheduling 
             * isSpsOnlyPaddingPresent is always "FALSE"
             * In SPS Scheduling if isSpsOnlyPaddingPresent is "TRUE"
             * then there  will be no retransmissions required.
             * ARQ Indication will be given to RLC and Harq Process
             * will be freed.
             */

            /*SPR 12744 Start*/
            if(harqReTxCount < maxRetxCount)
            /*SPR 12744 End*/
            {
                /*
                 *This TB needs to be reTransmitted 
                 */
                *nackFlag_p = TRUE;
            }
            else
            {

                /*Coverity ID <65217> Fix Changes Start*/
                CHECK_TADRXFLAG_RLC_TX_BUFF(dlHARQTBOneInfo_p->taDrxFlag ,&(dlHarqProcess_p->dlHARQTBOneInfo), dlHARQTBOneInfo_p->macScellCEFlag)
                    /*Coverity ID <65217> Fix Changes End*/
                {
                    /*
                     *The reTransmit count of TB reaches 
                     * max ReTrans Threshold 
                     */
                    /* SPR 11402 fix */
                    sendARQIndicationToRLC(
                            ueIndex,&(dlHarqProcess_p->dlHARQTBOneInfo),
                            TB_ONE,internalCellIndex);
                    /* SPR 11402 fix */
                }
                /*SPR 12744 Start*/
                /*Free the LeftOver Valid TBs by setting nackFlag_p FALSE */
                *nackFlag_p = FALSE;
                return;
                /*SPR 12744 End*/
            }
        }
        /*SPR 12744 Start*/
        else if(TRUE == tb2Valid) 
        /*SPR 12744 End*/
        {
            dlHARQTBTwoInfo_p = &(dlHarqProcess_p->dlHARQTBTwoInfo);
            harqReTxCount = dlHARQTBTwoInfo_p->current_TX_NB;

            if(harqReTxCount < maxRetxCount)
            {
                /*
                 *This TB needs to be reTransmitted 
                 */
                *nackFlag_p = TRUE;
            }
            else
            {
                /*Coverity ID <65217> Fix Changes Start*/
                CHECK_TADRXFLAG_RLC_TX_BUFF(dlHARQTBTwoInfo_p->taDrxFlag ,&(dlHarqProcess_p->dlHARQTBTwoInfo), dlHARQTBTwoInfo_p->macScellCEFlag)
                    /*Coverity ID <65217> Fix Changes End*/
                {
                    /*
                     *The reTransmit count of TB reaches 
                     * max ReTrans Threshold 
                     */
                    /* SPR 11402 fix */
                    sendARQIndicationToRLC(
                            ueIndex,&(dlHarqProcess_p->dlHARQTBTwoInfo),TB_TWO
                            ,internalCellIndex);
                    /* SPR 11402 fix */
                }
                /*SPR 12744 Start*/
                /*Free the LeftOver Valid TBs by setting nackFlag_p FALSE */
                *nackFlag_p = FALSE;
                return;
                /*SPR 12744 End*/
            }
        }
    /*SPR 12744 Start*/
    }
    /*SPR 12744 End*/


}
/* CA_phase2_harq_code End */
/*****************************************************************************
 * Function Name  :  expireAllNodeFromHarqTimerList 
 * Inputs         :  loopOfNumOfSubFrames - Represent the number of previous
 *                                          subframes including the current tick 
 *                                          one for which the entry corresponding
 *                                          to HARQ TIMER ARRAY has to be deleted,
 *                   currentSubFrame - the present Sub Frame,   
 *                   delaySubFrame - Delay Sub Frame
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer 
 *                   corresponding to particular sub frame, this function puts 
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType expireAllNodeFromHarqTimerList (
        UInt8 loopOfNumOfSubFrames,
        UInt32 currentSubFrame,
        UInt8 delaySubFrame
        ,InternalCellIndex internalCellIndex)
{
    UInt16 dlHarqEntityTimerArrCount = 0;        
    UInt16 dlHarqTimerArrayloopIndex = 0;        
    DLHarqTimerEntityList* dlHarqEntityTimerList_p = PNULL;
    DLHarqTimerEntityList* dlHarqEntityTimerListAccess_p = PNULL;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContextInfo* dlUeContextInfoArrayAccess_p = PNULL;
    UInt8 frameNumber = 0;

/* CA_phase2_harq_code Start*/
    UInt8 tempHarqID = INVALID_HARQ_ID;
    UInt8 maxRetxCount = 0;
    UInt8 ueCellLoopCntr = 0;
    UInt8 cellArrIndex = 0;
    UInt8 numOfTxAnteenas = 0;
    UInt8 tb1Valid = 0;
    UInt8 tb2Valid = 0;
    UInt8 nackFlag[MAX_NUM_CELL] = {FALSE,};

    UInt16 ueIndex = INVALID_UE_ID;
    UInt16  dlHarqTimerArrayIndex = 0;

    InternalCellIndex secCellIndex = 0;
    TransmissonMode transMode = INVALID_TX_MODE;

    DLHARQProcess* dlHarqProcess_p = PNULL;
    DLHARQEntity*   dlHarqEntity_p = PNULL;

    DLHarqTimerNode* dlharqTimerNode_p = PNULL;

    DLUEContext*  dlUeContext_p = PNULL;        
    DLUESCellContext *dlUeScellContext_p  = PNULL;

    DLRetxQueueNode  retxNode; // for filling Retx Node

    CellConfigParams* cellParams_p = PNULL;
/* CA_phase2_harq_code End */

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    dlUeContextInfoArrayAccess_p = &dlUECtxInfoArr_g[0];
    dlHarqEntityTimerListAccess_p = &(dlHarqGlobals_g[internalCellIndex].dlHarqTimerEntityList[0]);


    if (0 == currentSubFrame)
    {
        frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME - delaySubFrame;
    }
    else if (currentSubFrame >= delaySubFrame)
    {
        frameNumber = currentSubFrame - delaySubFrame;
    }
    else 
    {
        delaySubFrame -= currentSubFrame;
        frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME - delaySubFrame;
    }

    /* SPR 3444 changes start */ 
    if (frameNumber >= MAX_SUB_FRAME)
    {
        frameNumber = (MAX_SUB_FRAME - 1);
    }    
    /* SPR 3444 changes end */ 

    /* If the difference is more or equal to the threshold, all DL 
     * HARQ processes will be freed and is informed to OAM. */
    if(loopOfNumOfSubFrames >= MAX_NUM_OF_TICK_MISSED)
    {
       /* SPR# 3860 Changes Starts */
        loopOfNumOfSubFrames = MAX_SUB_FRAME;
       /* SPR# 3860 Changes Ends */
        expireDLHarqTimerNodeForTickMiss(loopOfNumOfSubFrames, internalCellIndex);

        return MAC_SUCCESS ;
    }

    while (loopOfNumOfSubFrames--)
    {
        dlHarqEntityTimerList_p = &dlHarqEntityTimerListAccess_p[frameNumber];    
        dlHarqEntityTimerArrCount = dlHarqEntityTimerList_p->count;

        dlHarqTimerArrayloopIndex = 0;        

        if (!dlHarqEntityTimerArrCount)
        {
            /*
               As there is no node in this Index of Array hence nothing is to be deleted
               remember the valid value of an array starts form zero and the above
               variable dlHarqEntityTimerList_p->count represents the array index.
             */
            LOG_UT(MAC_DL_HARQ_ENTITY_TIMER_ARRAY_COUNT_IS_ZERO,LOGDEBUG,MAC_DL_HARQ,\
                    getCurrentTick(),
                    frameNumber,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
            
            if(!frameNumber)
            {
                frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME -1;
            }
            else
            {
                frameNumber--;
            }
            continue;
        }

        /* CA_phase2_harq_ut_code Start */
        while ( dlHarqTimerArrayloopIndex < dlHarqEntityTimerArrCount)
        {
            dlharqTimerNode_p = \
                          &(dlHarqEntityTimerList_p->\
                          dlHarqTimerNodeArr[dlHarqTimerArrayloopIndex]);
               
            /* CA_phase2_harq_ut_code End */

            /* CA_phase2_harq_code Start */
            /*
             * Intializing of HARQ ReTx Node
             */
            SET_HARQ_RETX_NODE(retxNode); 
            /* CA_phase2_harq_code End */

            if (dlharqTimerNode_p->isValid)
            {
                ueIndex = dlharqTimerNode_p->ueIndex;
                dlUeContextInfo_p = &dlUeContextInfoArrayAccess_p[ueIndex];
                /*
                 * We ascertain whether th eUE under consideration is already 
                 * on the process of deletion from the RRC
                 */  
                /* SPR 6654 FIX START */
                if ((!dlUeContextInfo_p->pendingDeleteFlag) &&
                    (PNULL != dlUeContextInfo_p->dlUEContext_p))
                /* SPR 6654 FIX END */
                {
                    dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;

                    /*
                     * For CA UE secondary cell may exist.
                     * Processing Harq node of Secondary cell also
                     * as Harq Timer starts for both cells at a time.
                     */
                    for(ueCellLoopCntr = 0; ueCellLoopCntr <= dlUeContext_p->scellCount;
                        ueCellLoopCntr++)
                    {

                        /* + SPR 11201 Changes */
                        retxNode.ueIndex = ueIndex;
                        /* - SPR 11201 Changes */
                        if(!ueCellLoopCntr)
                        {
                           /*
                            * Geting harqID from Timer Node.
                            * Geting harq Process from UE context by using 
                            * harq ID for Primary Cell
                            */
                            tempHarqID = dlharqTimerNode_p->harqProcessId;
                            dlHarqEntity_p= dlUeContext_p->dlHarqContext_p;
                            /** SPR 12320 Changes Start **/
                            if(tempHarqID == INVALID_HARQ_ID)
                            {
                                continue;
                            }
                            /** SPR 12320 Changes End **/

                            /*
                             * In case at time of Harq Process allocation
                             * for Harq Process is not allocated
                             * for primary serving cell of UE.
                             */

                            dlHarqTimerArrayIndex  = dlHarqEntity_p->
                                dlHarqTimerNodeArrrayIndex[frameNumber];

                            if( INVALID_COUNT == dlHarqTimerArrayIndex )
                            {
                                continue;
                            }

                            dlHarqProcess_p = 
                                &(dlHarqEntity_p->harqProcess[tempHarqID]);

                            transMode = \
                            dlUeContext_p->dlMIMOInfo.transmissionMode;

                            cellParams_p = cellSpecificParams_g.
                            cellConfigAndInitParams_p[internalCellIndex]->\
                            cellParams_p;

                           /*
                            * Filling Retransmission 
                            * Node for Primary Cell
                            */

                            retxNode.harqProcessId = tempHarqID;
                            retxNode.schUsed = dlharqTimerNode_p->schUsed;
                        }
                        else
                        {
                           /*
                            * Getting TimerArrayIndex from Harq Entity of 
                            * UE Seconadary Cell.
                            * Getting Harq Timer Node from harq Timer list.
                            * Geting harqID from Harq Timer Node.
                            * Geting harq Process from UE context by using 
                            * harq ID for Secondary Cell
                            */

                            if(PNULL == 
                                dlUeContext_p->dlUeScellContext_p[ueCellLoopCntr])
                            {
                                continue;
                            }

                            dlUeScellContext_p  =
                                dlUeContext_p->dlUeScellContext_p[ueCellLoopCntr];

                            secCellIndex = dlUeScellContext_p->internalCellIndex;

                            dlHarqEntity_p= dlUeScellContext_p->\
                                ScelldlHarqContext_p;
                            /** SPR 12320 Changes Start **/
                            if(!dlHarqEntity_p)
                            {
                                continue;
                            }
                            /** SPR 12320 Changes End **/

                            dlHarqTimerArrayIndex = dlUeScellContext_p->\
                                   ScelldlHarqContext_p->\
                                   dlHarqTimerNodeArrrayIndex[frameNumber];

                            /*
                             * In case at time of Harq Process allocation
                             * for Harq Process is not allocated
                             * for secondary serving cell of UE.
                             *
                             */

                            if( INVALID_COUNT == dlHarqTimerArrayIndex )
                            {
                                continue;
                            }

                            dlharqTimerNode_p = &(dlHarqEntityTimerList_p->\
                                dlHarqTimerNodeArr[dlHarqTimerArrayIndex]);

                            tempHarqID = dlharqTimerNode_p->harqProcessId;
                            /** SPR 12320 Changes Start **/
                            if(tempHarqID == INVALID_HARQ_ID)
                            {
                                continue;
                            }
                            /** SPR 12320 Changes End **/

                            dlHarqProcess_p = 
                                &(dlHarqEntity_p->harqProcess[tempHarqID]);

                            transMode = \
                            dlUeScellContext_p->ScelldlMIMOInfo.transmissionMode;

                            cellParams_p = cellSpecificParams_g.
                            cellConfigAndInitParams_p[secCellIndex]->\
                            cellParams_p;

                           /*
                            * Filling Retransmission 
                            * Node for Seconary Cell
                            */

                            cellArrIndex = ueCellLoopCntr - 1;

                            retxNode.sCellRetxInfo[cellArrIndex].\
                                servCellIndex = ueCellLoopCntr;
                            retxNode.sCellRetxInfo[cellArrIndex].\
                                harqProcessId = tempHarqID;
                            retxNode.sCellRetxInfo[cellArrIndex].\
                                schUsed = dlharqTimerNode_p->schUsed;

                        }
                        /*
                          * Since calling the dlHarqTimerStop would be an overhead as we are 
                          * in any case have to access the global structure of the TIMER node
                          * hence in the next few lines are performing the function of 
                          * dlHarqTimerStopr. This would help in improving 
                          * the our performance.
                          */

                        dlharqTimerNode_p->isValid = HARQ_TIMER_NODE_INVALID;
                        dlharqTimerNode_p->harqProcessId = INVALID_HARQ_ID;
                        dlharqTimerNode_p->ueIndex = INVALID_UE_ID;

                        LOG_MAC_MSG(MAC_EXPIRE_DL_HARQ_TIMER_NODE,LOGINFO,
                                MAC_DL_HARQ,getCurrentTick(),
                                frameNumber,tempHarqID,
                                ueIndex,DEFAULT_INT_VALUE,
                                DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

                        LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ,
                                "[%s]global TICK %d " 
                                " Inside of pendingDeleteFlag ue ID %d,"
                                "harq Process ID : %d frameNumber %d,"
                                "DLUE_Contxt_dlHarqTimerNodeArrrayIndex %d \n"
                                ,__func__,getCurrentTick(),dlUeContext_p->ueIndex ,
                                tempHarqID,frameNumber,
                                dlUeContext_p->dlHarqContext_p->\
                                dlHarqTimerNodeArrrayIndex[frameNumber]);

                        /*
                         *  We now have to check the UE Transmission Mode 
                         *  (MIMO/Non-MIMO) as in the case when both the TBs
                         *  are on re-transmission then retransmission for
                         *  both TB's at at a time.
                         */

                        numOfTxAnteenas = cellParams_p->numOfTxAnteenas; 
                        maxRetxCount = cellParams_p->maxHarqRetransmission - 1;

                        IS_MIMO_CONFIGURED(transMode ,numOfTxAnteenas)
                        {
                            /*
                             * In case of MIMO either TB1/TB2 will be valid
                             * or both can be valid also.
                             */
                            tb1Valid = dlHarqProcess_p->isTBOneValid;
                            tb2Valid = dlHarqProcess_p->isTBTwoValid;
                        }
                        else
                        {
                            /*
                             * In case of SISO only TB1 will be valid
                             */
                            tb1Valid = dlHarqProcess_p->isTBOneValid;
                            tb2Valid = FALSE;
                            if((SPS_SCHEDULING == dlharqTimerNode_p->schUsed)
                                    /* SPR 19104 Fix Start */
                                    && (dlUeContext_p->spsDlInfo.spsDlSetupInfo_p))
                                /* SPR 19104 Fix End */
                            {
                                /*
                                 * In case of SPS maxRetxCount  will
                                 * be handled from SPS context of UE
                                 */
                                maxRetxCount = dlUeContext_p->spsDlInfo.\
                                          spsDlSetupInfo_p->maxSpsHarqRetx;
                            }
                        }

                        /*
                        * This function checks harq retransmissions of valid 
                        * TB with maxReTxCount. 
                        * If retransmissions reaches maxReTxCount
                        * then ARQ Indication will be given to RLC else
                        * harq info will be filled into nackFlag
                        */
                        macProcessExpireHarqNode(ueIndex,dlHarqProcess_p,
                              maxRetxCount,tb1Valid,tb2Valid,
                              &nackFlag[ueCellLoopCntr],internalCellIndex);


#ifdef FDD_CONFIG
                        /*SPR 16026 changes start*/
                        if(dlHarqProcess_p->scellActBitmask || dlHarqProcess_p->scellDeactBitmask)
                        {
                          scellHarqFailureForMaxRetx(dlHarqProcess_p,dlUeContext_p,!nackFlag[ueCellLoopCntr]);
                        }
                        /*SPR 16026 changes end*/
#endif
                        if(nackFlag[ueCellLoopCntr] == FALSE)
                        {
                            freeDLHarqProcess(dlUeContext_p,
                                    dlHarqProcess_p,internalCellIndex);
                        }
                        else
                        {
                            retxNode.isNodeInvalid = FALSE;
                        }
                        /*
                         * We now fill the array 
                         *  dlHarqTimerNodeIndex with an invalid entry.
                         */
                        /* SPR 6136 start */
                        dlHarqEntity_p->dlHarqTimerNodeArrrayIndex[frameNumber]\
                        = INVALID_COUNT;
                        /* SPR 6136 end */

                    }//for loop of scell

                    if((FALSE == retxNode.isNodeInvalid) && MAC_FAILURE ==
                            macPushHarqNackInfoToRetxQueue(
                                dlUeContext_p,retxNode,
                                nackFlag,internalCellIndex))
                    {
                        macHandleRetxQueueFailure(ueIndex,nackFlag,retxNode);
                    }
                    /*
                     * Resetting NACK Flag
                     */
                    nackFlag[0] = FALSE;
                    nackFlag[1] = FALSE;

                    /* CA_phase2_harq_code End */

                }
                else
                {
                    LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ, \
                            "\nThe Pending delete Flag has been set by RRC for the UE Index = %d"
                            " \n in side the function expireAllNodeFromHarqTimerList", ueIndex);
                    /* + SPR_10929 */
                    dlHarqTimerArrayloopIndex++; 
                    /* - SPR_10929 */
                    continue;
                }
            }
            /* CA_phase2_harq_ut_code Start */
            dlHarqTimerArrayloopIndex++; 
            /* CA_phase2_harq_ut_code Start */

        }

        dlHarqEntityTimerList_p->count = 0;

        LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ, \
                "The value of count in DLHarqEntityTimerList is ZERO  for the sub"
                "frame no. %d", frameNumber);
        if(!frameNumber)
        {
            frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME -1;
        }
        else
        {
            frameNumber--;
        }
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}

/* + SPR 17733 */
/*****************************************************************************
 * Function Name  : putEntryInMsg4ReTxQueue
 * Inputs         : ueContext_p- A pointer to TempCRNTICtx
 *                  harqProcessId - The harq process id whose NACK has been
 *                               received.
 *                  harqRTTExpiryTTI - Expiry TTI of HARQ RTT timer
                    internalCellIndex - Cell-Index at MAC
 * Outputs        :
 * Returns        : None
 * Description    : When handling NACK or timer expiry ,timer thread
 *                  calls this function to put the harq process id in the global
 *                  msg4 retransmission Queue.
 *****************************************************************************/
void putEntryInMsg4ReTxQueue(
        TempCRNTICtx *  ueContext_p, UInt8 harqProcessId,
        InternalCellIndex internalCellIndex)
{
    DLRetxQueueNode *retxNode_p = PNULL;
    DLHARQProcess* dlHarqProcess_p = PNULL;

    GET_MEM_FROM_POOL (DLRetxQueueNode, retxNode_p,
            sizeof (DLRetxQueueNode), PNULL);
    if( PNULL == retxNode_p )
    {
        return;
    }
    RNTIInfo *rntiInfo_p = getUeIdxFromRNTIMap(ueContext_p->tcRNTI,internalCellIndex);
    /* coverity fix 96900 */
    if(PNULL == rntiInfo_p)
    {
        freeMemPool(retxNode_p);
        return;
    }
    retxNode_p->harqProcessId = harqProcessId;
    retxNode_p->ueIndex = rntiInfo_p->index;
    retxNode_p->isNodeInvalid = FALSE;
    dlHarqProcess_p =
        &(ueContext_p->dlHarqCtx_p->harqProcess[harqProcessId]); 
    dlHarqProcess_p->dlHARQTBOneInfo.txIndicator = RE_TX;

    if(pushNode(&Msg4RetransmissionQueue_gp[internalCellIndex], &(retxNode_p->anchorNode)))
    {
        freeMemPool(retxNode_p);
    }
}

/*****************************************************************************
 * Function Name  :  expireAllNodeFromHarqTimerListForMsg4 
 * Inputs         :  loopOfNumOfSubFrames - Represent the number of previous
 *                   subframes including the current tick one for which the
 *                   entry corresponding to HARQ TIMER ARRAY has to be deleted.
 *                :  currentGlobalTick - the present global tick.
 *                :  internalCellIndex - Cell-Index at MAC
 * Outputs        :  None  
 * Returns        :  None 
 * Description    :  This function will remove all the nodes in the HARQ Timer 
 *                   corresponding to particular sub frame, this function puts 
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into Msg4 retransmission queue.
 *****************************************************************************/
void expireAllNodeFromHarqTimerListForMsg4(
        UInt32 loopOfNumOfSubFrames,
        tickType_t currentSubFrame,
        UInt8 delaySubFrame,
        InternalCellIndex internalCellIndex)
{
    UInt32 activeIdx = 
    cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
          rachConfigInfo.rachConfiguration.activeIdxContentionRes;
    RACHConfiguration *rachConfig_p = 
        &cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                    rachConfigInfo.rachConfiguration;
    UInt16 dlHarqEntityTimerArrCount = 0;        
    DLHarqTimerEntityForMsg4List *dlHarqEntityTimerList_p = PNULL;
    DLHarqTimerEntityForMsg4List *dlHarqEntityTimerListAccess_p = PNULL;
    DLHarqTimerForMsg4Node* dlharqTimerNode_p = PNULL;
    DLHARQProcess* dlHarqProcess_p = PNULL;
    DLHARQEntity*   dlHarqEntity_p = PNULL;
    /* coverity fix 96910 */
    UInt16 ueIndex = 0;
    UInt8 tempHarqID = 0;
    UInt8 frameNumber = 0;
    UInt8 msg4MaxHarqRetx = rachConfig_p->msg4MaxHarqRetransmission[activeIdx];
    TempCRNTICtx * tempCRNTICtx_p = PNULL;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    dlHarqEntityTimerListAccess_p = &(dlHarqGlobals_g[internalCellIndex].dlHarqTimerEntityForMsg4List[0]);

    if (0 == currentSubFrame)
    {
        frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME - delaySubFrame;
    }
    else if (currentSubFrame >= delaySubFrame)
    {
        frameNumber = currentSubFrame - delaySubFrame;
    }
    else 
    {
        delaySubFrame -= currentSubFrame;
        frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME - delaySubFrame;
    }

    if (frameNumber >= MAX_SUB_FRAME)
    {
        frameNumber = (MAX_SUB_FRAME - 1);
    }    
    if(loopOfNumOfSubFrames > MAX_NUM_OF_TICK_MISSED)
    {
        loopOfNumOfSubFrames = MAX_SUB_FRAME;
        expireDLHarqTimerNodeForTickMiss(loopOfNumOfSubFrames,internalCellIndex );
    }

    while (loopOfNumOfSubFrames--)
    {
        dlHarqEntityTimerList_p = &dlHarqEntityTimerListAccess_p[frameNumber]; 
        dlHarqEntityTimerArrCount = dlHarqEntityTimerList_p->count;

        if (!dlHarqEntityTimerArrCount)
        {
            /*
               As there is no node in this Index of Array hence nothing is to be deleted
               remember the valid value of an array starts form zero and the above
               variable dlHarqEntityTimerList_p->count represents the array index.
             */
            LOG_UT(MAC_DL_HARQ_ENTITY_TIMER_ARRAY_COUNT_IS_ZERO,LOGDEBUG,MAC_DL_HARQ,
                    getCurrentTick(),
                    frameNumber,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
            if(!frameNumber)
            {
                frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME -1;
            }
            else
            {
                frameNumber--;
            }
            continue;
        }

        while (dlHarqEntityTimerArrCount--)
        {
            dlharqTimerNode_p = 
                &(dlHarqEntityTimerList_p->dlHarqTimerNodeForMsg4Arr[dlHarqEntityTimerArrCount]);    
            if (dlharqTimerNode_p->isValid)
            {
                ueIndex = dlharqTimerNode_p->ueIndex;
                tempCRNTICtx_p = tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;
               /* SPR 21180 fix start */ 
                if ((tempCRNTICtx_p)&&(tempCRNTICtx_p->status != CONTENTION_ACK_RECD))
                    /* SPR 21180 fix end */
                {
                    dlHarqEntity_p = tempCRNTICtx_p->dlHarqCtx_p;
                    dlHarqProcess_p = &(dlHarqEntity_p->
                            harqProcess[dlharqTimerNode_p->harqProcessId]);
                    /*
                       Since calling the dlHarqTimerStop would be an overhead as we are 
                       in any case have to access the global structure of the TIMER node
                       hence in the next few lines are performing the function of dlHarqTimerStop
                       This would help in improving the our performance.
                     */

                    dlharqTimerNode_p->isValid = HARQ_TIMER_NODE_INVALID;
                    tempHarqID = dlharqTimerNode_p->harqProcessId;
                    dlharqTimerNode_p->harqProcessId = INVALID_HARQ_ID;
                    dlharqTimerNode_p->ueIndex = INVALID_UE_ID;

                    LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ,
                            "[%s]global TICK %d Inside of pendingDeleteFlag ue ID %d,"
                            "harq Process ID : %d frameNumber %d\n"
                            ,__func__,getCurrentTick(),dlharqTimerNode_p->ueIndex,
                            tempHarqID,frameNumber);
                    /*
                       We now fill the array dlHarqTimerNodeIndex with an invalid entry.
                     */
                    dlHarqEntityTimerList_p->count--;

                    if ( dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB < 
                            (msg4MaxHarqRetx - 1))     
                    {
                        /*
                           Here the TB one is yet to complete the max re-trans.
                         */
                        LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,\
                                "[%s] current_TX_NB %d \n",__func__, 
                                dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB);
                        putEntryInMsg4ReTxQueue(
                                tempCRNTICtx_p, tempHarqID,
                                internalCellIndex);
                    }
                    /*
                       We now consider the case When TB_One has reached it's
                       max re trans limit.
                     */
                    else
                    {
                        freeDLHarqProcessForMsg4(
                                dlHarqProcess_p);
                    }
                }
                else
                {
                    LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ, \
                            "\n tempCRNTICtx_p is NULL");
                    continue;
                }
            }
            else
            {
                lteWarning("[%s] The dlharqTimerNode_p->isValid is INVALID \n", __func__);
            }
        }

        dlHarqEntityTimerList_p->count = 0;

        LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ, \
                "The value of count in DLHarqEntityTimerList is ZERO  for the sub"
                "frame no. %d", frameNumber); 
        if(!frameNumber)
        {
            frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME -1;
        }
        else
        {
            frameNumber--;
        }
    }
    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
}
/* - SPR 17733 */
#endif
/*****************************************************************************
 * Function Name  : checkForFreeHarqProcess
 * Inputs         : dlUeCtx_p - The UE context pointer,
 *                  dlHarqEntity_p,
 *                  schType - if for SPS/DRA
 *                  internalCellIndex
 * Outputs        : pointer to Free Harq Process
 * Returns        : Pointer to free HARQProcess if found otherwise PNULL
 * Description    : This function checks for any free Harq Process within a
 *                  UE Context.
 *****************************************************************************/
DLHARQProcess *checkForFreeHarqProcess(
        DLUEContext *dlUeCtx_p, 
	DLHARQEntity* dlHarqEntity_p, 
        SchType schType,
	InternalCellIndex internalCellIndex)
{
    UInt8 hpId = 0;
    /* SPR 3444 changes start */
    /* SPR 3444 changes end */
    DLHARQProcess* dlHarqProcess_p = PNULL;
    /* FDD_Warning_Fix */
  
    UInt8 maxHarqProcess = dlHarqEntity_p->numHARQProcess;
    
    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
#ifdef FDD_CONFIG    
    LP_SpsDlInfo      spsDlInfo_p      = PNULL;
    LP_SpsDlSetupInfo spsDlSetupInfo_p = PNULL;

    spsDlInfo_p = &dlUeCtx_p->spsDlInfo;
    spsDlSetupInfo_p = spsDlInfo_p->spsDlSetupInfo_p;
    /* HARQ Process greater than maxarqProcess configured by RRC are 
       initialized to HARQ_PROCESS_IN_USE by default*/
    /* If no HARQ processes are reserved for SPS by an UE, i.e. SPS is released
     * or state is SPS_DEACTIVATION_INITIATED */
    if( PNULL == spsDlSetupInfo_p || 
        ( SPS_DEACTIVATION_INITIATED == spsDlInfo_p->spsState &&
          SPS_SCHEDULING == schType ) )
    {
        /* + SPR 5844 */
#ifdef DL_HARQ_REUSE_AFTER_EIGHT_TTI
        /* SPR 15909 fix start */
        tickType_t currentTTI = getCurrentTick(); 
        /* SPR 15909 fix end */
        for ( hpId = 0;  hpId < maxHarqProcess; hpId++)
        {
            if( (dlHarqEntity_p->harqProcess[hpId].harqStatus == HARQ_PROCESS_FREE) &&
                ((currentTTI + PHY_DL_DELAY) >= (dlHarqEntity_p->harqProcess[hpId].dlharqRttExpiryTick)) )
            {
                dlHarqProcess_p = &dlHarqEntity_p->harqProcess[hpId];
                dlHarqProcess_p->dlharqRttExpiryTick = 0;
                dlHarqProcess_p->harqProcessId = hpId;
                dlHarqProcess_p->harqStatus = HARQ_PROCESS_IN_USE;
                /* Update Stats for HARQ Process Id resrved */
                LTE_MAC_UPDATE_HARQ_PROCESS_ID_USED( dlUeCtx_p->ueIndex,            
                        dlHarqProcess_p->harqProcessId, 
                        dlUeCtx_p->internalCellIndex);
                /* SPR 5161 fix start */
                dlHarqProcess_p->transmissionBuffer_p    =  PNULL;
                dlHarqProcess_p->transmissionBufferTB2_p    =  PNULL;
                /* SPR 5161 fix end */
                dlHarqProcess_p->numOfLayer = dlUeCtx_p->dlMIMOInfo.dlRi;
                return dlHarqProcess_p;
            }
        }
#else
        if ((dlHarqEntity_p->harqProcess[hpId].harqStatus == HARQ_PROCESS_FREE) 
                ||( dlHarqEntity_p->harqProcess[++hpId].harqStatus == HARQ_PROCESS_FREE)
                ||( dlHarqEntity_p->harqProcess[++hpId].harqStatus == HARQ_PROCESS_FREE)
                ||( dlHarqEntity_p->harqProcess[++hpId].harqStatus == HARQ_PROCESS_FREE)
                ||( dlHarqEntity_p->harqProcess[++hpId].harqStatus == HARQ_PROCESS_FREE) 
                ||( dlHarqEntity_p->harqProcess[++hpId].harqStatus == HARQ_PROCESS_FREE) 
                ||( dlHarqEntity_p->harqProcess[++hpId].harqStatus == HARQ_PROCESS_FREE) 
                ||( dlHarqEntity_p->harqProcess[++hpId].harqStatus == HARQ_PROCESS_FREE))
        {
            dlHarqProcess_p = &dlHarqEntity_p->harqProcess[hpId];
            LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,\
                    "[%s] HARQ PROCESS ID %d In dlHarqProcess\n ",\
                    __func__,dlHarqProcess_p->harqProcessId);
            dlHarqProcess_p->harqProcessId = hpId;
            dlHarqProcess_p->harqStatus = HARQ_PROCESS_IN_USE;

	    /* SPR 21857 changes */
            dlHarqProcess_p->pmiIndex = WIDEBAND_PMI;
	    /* SPR 21857 changes */

            LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
                    getCurrentTick(),
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
		    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
            /* Update Stats for HARQ Process Id resrved */
            LTE_MAC_UPDATE_HARQ_PROCESS_ID_USED( dlUeCtx_p->ueIndex,
                    dlHarqProcess_p->harqProcessId,
                    dlUeCtx_p->internalCellIndex);
            /* SPR 5161 fix start */
            dlHarqProcess_p->transmissionBuffer_p    =  PNULL;
            dlHarqProcess_p->transmissionBufferTB2_p    =  PNULL;
            /* SPR 5161 fix end */
            dlHarqProcess_p->numOfLayer = dlUeCtx_p->dlMIMOInfo.dlRi;

            /* Reset scellActBitmask/scellDeactBitmask before using */
            /* + SPR 11198 Changes */
            dlHarqProcess_p->scellActBitmask = 0;  
            dlHarqProcess_p->scellDeactBitmask = 0; 
            /* - SPR 11198 Changes */

            return dlHarqProcess_p;
        }
#endif
        /* - SPR 5844 */
    }
    else
    {
        /* Check the scheduling type */
        if( SPS_SCHEDULING == schType )
        {
            /* !
             * \code
             * - Get the Sps Harq Process Id
             * - spsHpId = (currentTick_g / semiPersistentSchedIntervalDL)
             *                                  % numberOfConfSpsProcesses
             * - If harqStatus is Not HARQ_PROCESS_FREE
             *       - Free the DL Harq Process.
             * - Initialize the HarqProcess 
             * - Return the HarqProcess
             * \endcode
             * */
            hpId = ((getCurrentTick() + PHY_DL_DELAY) / spsIntervalMap_g[
                         spsDlSetupInfo_p->semiPersistentSchedIntervalDL]) %
                         (spsDlSetupInfo_p->numberOfConfSpsProcesses);
            dlHarqProcess_p = &dlHarqEntity_p->harqProcess[hpId];
            
            if( HARQ_PROCESS_FREE != dlHarqProcess_p->harqStatus)
            {
                /* free the HARQ process for SPS NEW_TX */
                freeDLHarqProcessTBSpecific( dlUeCtx_p, dlHarqProcess_p,
                        TB_ONE, internalCellIndex);
            }

            dlHarqProcess_p->harqProcessId = hpId;
            dlHarqProcess_p->harqStatus = HARQ_PROCESS_IN_USE;
            /* Update Stats for HARQ Process Id resrved */
            LTE_MAC_UPDATE_HARQ_PROCESS_ID_USED( dlUeCtx_p->ueIndex,            
                    dlHarqProcess_p->harqProcessId,
                    dlUeCtx_p->internalCellIndex);
            LTE_MAC_INCREMENT_SPS_HARQ_STATS( dlUeCtx_p->ueIndex, dlUeCtx_p,
                    getCurrentTick(), schType, NEW_TX );
            LOG_UT( MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,
                    getCurrentTick(), 0, 0, 0, 0, 0, 0, 0, __func__,"");
            /* SPR 5161 fix start */
            dlHarqProcess_p->transmissionBuffer_p    =  PNULL;
            dlHarqProcess_p->transmissionBufferTB2_p    =  PNULL;
            /* SPR 5161 fix end */
            dlHarqProcess_p->numOfLayer = dlUeCtx_p->dlMIMOInfo.dlRi;
            return dlHarqProcess_p;
        }
        else
        {
            /*SPR 7104 SPS Fix start*/
            hpId = 0;
            /* Start looking after reserved SPS HARQs */
            if (spsDlSetupInfo_p != PNULL)
            {
                hpId = spsDlSetupInfo_p->numberOfConfSpsProcesses;
            }
            /*SPR 7104 SPS Fix end*/

            /* Start looking after reserved SPS HARQs */
            for ( ; hpId < maxHarqProcess; hpId++)
            {
                if( HARQ_PROCESS_FREE == 
                        dlHarqEntity_p->harqProcess[hpId].harqStatus)
                {
                    dlHarqProcess_p = &dlHarqEntity_p->harqProcess[hpId];
                    dlHarqProcess_p->harqProcessId = hpId;
                    dlHarqProcess_p->harqStatus = HARQ_PROCESS_IN_USE;

                    /* Update Stats for HARQ Process Id resrved */
                    LTE_MAC_UPDATE_HARQ_PROCESS_ID_USED( dlUeCtx_p->ueIndex,            
                            dlHarqProcess_p->harqProcessId,
                            dlUeCtx_p->internalCellIndex);
                    LTE_MAC_INCREMENT_SPS_HARQ_STATS( dlUeCtx_p->ueIndex, 
                            dlUeCtx_p, getCurrentTick(), schType, NEW_TX );
                    LOG_UT( MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,
                            getCurrentTick(), 0, 0, 0, 0, 0, 0, 0, __func__,"");
                    /* SPR 5161 fix start */
                    dlHarqProcess_p->transmissionBuffer_p    =  PNULL;
                    dlHarqProcess_p->transmissionBufferTB2_p    =  PNULL;
                    /* SPR 5161 fix end */
                    dlHarqProcess_p->numOfLayer = dlUeCtx_p->dlMIMOInfo.dlRi;
                    return dlHarqProcess_p;
                }
            }
        }
    }
#elif TDD_CONFIG
	LP_SpsDlInfo      spsDlInfo_p      = PNULL;
	LP_SpsDlSetupInfo spsDlSetupInfo_p = PNULL;
	spsDlInfo_p = &dlUeCtx_p->spsDlInfo;
	spsDlSetupInfo_p = spsDlInfo_p->spsDlSetupInfo_p;
	/* HARQ Process greater than maxarqProcess configured by RRC are 
	   initialized to HARQ_PROCESS_IN_USE by default*/
	/* If no HARQ processes are reserved for SPS by an UE, i.e. SPS is released
	 * or state is SPS_DEACTIVATION_INITIATED */
	if( PNULL == spsDlSetupInfo_p || 
			( SPS_DEACTIVATION_INITIATED == spsDlInfo_p->spsState &&
			  SPS_SCHEDULING == schType ) )
    {
        for ( hpId = 0;  hpId < maxHarqProcess; hpId++)
        {
            if (dlHarqEntity_p->harqProcess[hpId].harqStatus == HARQ_PROCESS_FREE)
            {
                dlHarqProcess_p = &dlHarqEntity_p->harqProcess[hpId];
                LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,\
                        "[%s] HARQ PROCESS ID %d In dlHarqProcess\n ",\
                        __func__,dlHarqProcess_p->harqProcessId);
                dlHarqProcess_p->harqProcessId = hpId;
                dlHarqProcess_p->harqStatus = HARQ_PROCESS_IN_USE;
                dlHarqProcess_p->subFrameType = NA;
                /* SPR 5161 fix start */
                dlHarqProcess_p->transmissionBuffer_p    =  PNULL;
                dlHarqProcess_p->transmissionBufferTB2_p    =  PNULL;
                /* SPR 5161 fix end */
                /*4x4 DL MIMO CHG START*/
                dlHarqProcess_p->numOfLayer = dlUeCtx_p->dlMIMOInfo.dlRi;
                /*4x4 DL MIMO CHG END*/
                LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,"[%s] Exit After"
                        " allocation of HARQ PROCESS ID %d\n ",__func__,hpId);
                LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
                        getCurrentTick(),
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
                return dlHarqProcess_p;
            }
        }
    }
	else
	{
		/* Check the scheduling type */
		if( SPS_SCHEDULING == schType )
		{
			/* !
			 * \code
			 * - Get the Sps Harq Process Id
			 * - spsHpId = (currentTick_g / semiPersistentSchedIntervalDL)
			 *                                  % numberOfConfSpsProcesses
			 * - If harqStatus is Not HARQ_PROCESS_FREE
			 *       - Free the DL Harq Process.
			 * - Initialize the HarqProcess 
			 * - Return the HarqProcess
			 * \endcode
			 * */
			hpId = ((getCurrentTick() + DL_ADVANCE_TICK) / spsIntervalMap_g[
					spsDlSetupInfo_p->semiPersistentSchedIntervalDL]) %
				(spsDlSetupInfo_p->numberOfConfSpsProcesses);
			dlHarqProcess_p = &dlHarqEntity_p->harqProcess[hpId];

			if( HARQ_PROCESS_FREE != dlHarqProcess_p->harqStatus)
			{
				/* free the HARQ process for SPS NEW_TX */
				freeDLHarqProcessTBSpecific( dlUeCtx_p, dlHarqProcess_p,
						TB_ONE, internalCellIndex);
			}

			dlHarqProcess_p->harqProcessId = hpId;
			dlHarqProcess_p->harqStatus = HARQ_PROCESS_IN_USE;
			/* Update Stats for HARQ Process Id resrved */
			LTE_MAC_UPDATE_HARQ_PROCESS_ID_USED( dlUeCtx_p->ueIndex,            
					dlHarqProcess_p->harqProcessId, dlUeCtx_p->internalCellIndex );
			LTE_MAC_INCREMENT_SPS_HARQ_STATS( dlUeCtx_p->ueIndex, dlUeCtx_p,
					getCurrentTick(), schType, NEW_TX );
			LOG_UT( MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,
					getCurrentTick(), 0, 0, 0, 0, 0, 0, 0, __func__,"");
			/* SPR 5161 fix start */
			dlHarqProcess_p->transmissionBuffer_p    =  PNULL;
			dlHarqProcess_p->transmissionBufferTB2_p    =  PNULL;
			/* SPR 5161 fix end */
			dlHarqProcess_p->subFrameType = NA;
			/*4x4 DL MIMO CHG START*/
			dlHarqProcess_p->numOfLayer = dlUeCtx_p->dlMIMOInfo.dlRi;
			/*4x4 DL MIMO CHG END*/
			LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
					getCurrentTick(),
					DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
					DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
					DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
			return dlHarqProcess_p;
		}
		else
		{
			/*SPR 7104 SPS Fix start*/
			hpId = 0;
			/* Start looking after reserved SPS HARQs */
			if (spsDlSetupInfo_p != PNULL)
			{
				hpId = spsDlSetupInfo_p->numberOfConfSpsProcesses;
			}
			/*SPR 7104 SPS Fix end*/

			/* Start looking after reserved SPS HARQs */
			for ( ; hpId < maxHarqProcess; hpId++)
			{
				if( HARQ_PROCESS_FREE == 
						dlHarqEntity_p->harqProcess[hpId].harqStatus)
				{
					dlHarqProcess_p = &dlHarqEntity_p->harqProcess[hpId];
					dlHarqProcess_p->harqProcessId = hpId;
					dlHarqProcess_p->harqStatus = HARQ_PROCESS_IN_USE;

					/* Update Stats for HARQ Process Id resrved */
					LTE_MAC_UPDATE_HARQ_PROCESS_ID_USED( dlUeCtx_p->ueIndex,            
							dlHarqProcess_p->harqProcessId, dlUeCtx_p->internalCellIndex );
					LTE_MAC_INCREMENT_SPS_HARQ_STATS( dlUeCtx_p->ueIndex, 
							dlUeCtx_p, getCurrentTick(), schType, NEW_TX );
					LOG_UT( MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,
							getCurrentTick(), 0, 0, 0, 0, 0, 0, 0, __func__,"");
					/* SPR 5161 fix start */
					dlHarqProcess_p->transmissionBuffer_p    =  PNULL;
					dlHarqProcess_p->transmissionBufferTB2_p    =  PNULL;
					/* SPR 5161 fix end */
					/*4x4 DL MIMO CHG START*/
					dlHarqProcess_p->numOfLayer = dlUeCtx_p->dlMIMOInfo.dlRi;
					/*4x4 DL MIMO CHG END*/
					dlHarqProcess_p->subFrameType = NA;
					LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
							getCurrentTick(),
							DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
							DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
							DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
					return dlHarqProcess_p;
				}
			}
		}
	}
	/* - SPS_TDD_Changes */
#endif
    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
		DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return PNULL;
}

/* + SPR 17733 */
#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : checkForFreeHarqProcessForTcrnti
 * Inputs         : dlHarqEntity_p
 * Outputs        : pointer to Free Harq Process
 * Returns        : Pointer to free HARQProcess if found otherwise PNULL
 * Description    : This function checks for any free Harq Process within a
 *                  UE Context.
 *****************************************************************************/
DLHARQProcess *checkForFreeHarqProcessForTcrnti(DLHARQEntity* dlHarqEntity_p)
{
    UInt8 hpId = 0;  
    DLHARQProcess* dlHarqProcess_p = PNULL;

    UInt8 maxHarqProcess = dlHarqEntity_p->numHARQProcess;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    for ( ; hpId < maxHarqProcess; hpId++)
    {
        if( HARQ_PROCESS_FREE == 
                dlHarqEntity_p->harqProcess[hpId].harqStatus)
        {
            dlHarqProcess_p = &dlHarqEntity_p->harqProcess[hpId];
            dlHarqProcess_p->harqProcessId = hpId;
            dlHarqProcess_p->harqStatus = HARQ_PROCESS_IN_USE;

            /* Update Stats for HARQ Process Id resrved */
            LOG_UT( MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,
                    getCurrentTick(), 0, 0, 0, 0, 0, 0, 0, __func__,"");
            dlHarqProcess_p->transmissionBuffer_p    =  PNULL;
            dlHarqProcess_p->transmissionBufferTB2_p    =  PNULL;
            dlHarqProcess_p->numOfLayer = 1;
            return dlHarqProcess_p;
        }
    }
    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return PNULL;
}
#endif
/* - SPR 17733 */

#ifdef KPI_STATS
/*****************************************************************************
 * Function Name  :  macUpdateKpiCntrsForPktDelayPerQci
 * Inputs         :  TBInfo,
 *                   ueIndex,
 *                   TBFlag
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function updates KPI counters for Packet Delay.
 *****************************************************************************/
/* + KPI_CA */
/* SPR 12783 Changes Start */
void macUpdateKpiCntrsForPktDelayPerQci(DLHARQTBInfo *TBInfo, UInt16 ueIndex,
		UInt8 TBFlag, InternalCellIndex internalCellIndex, 
        /* SPR 15909 fix start */
        tickType_t recvTTI)
        /* SPR 15909 fix end */
{
    RLCDataBuffer* rlcDataBufferTB = PNULL;
    rlcDataBufferTB = &(TBInfo->transmissionAndRLCData_p->rlcDataBufferArr[0]);
    UInt8 lc = 0, qci = 0, i = 0,lcCount = 0, sduCount = 0;                                 
    UInt32 activeIndexForMacStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
    LteMacKPIStats * pLteMacKPIStats = &(gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
    /*spr 19296 fix start*/
	/*code deleted*/
    /*spr 19296 fix end*/
/* - KPI_CA */

    if(rlcDataBufferTB != NULL)
    {
        if(TBFlag == 1)
            lcCount = TBInfo->transmissionAndRLCData_p->lcCountTBOne; 
        else
            lcCount = TBInfo->transmissionAndRLCData_p->lcCountTBTwo; 

        for(lc = 0; lc < lcCount; lc++)
        {
            /* SPR 22075 Changes Start */
            /* SPR 22010 Changes Start */
            if (rlcDataBufferTB[lc].lcID < 3 || 
                    rlcDataBufferTB[lc].lcID >= MAX_NUMBER_OF_LOGICAL_CHANNEL)
            {
                continue;
            }
            /* SPR 22010 Changes End */
            /* SPR 22075 Changes End */

            qci = rlcDataBufferTB[lc].qci;
            if (qci > 0 && qci <= MAX_QCI)      
            {
                qci--;
                if(rlcDataBufferTB[lc].sduCount > MAX_RLC_SDU_IN_PDU_FOR_KPI)
                    sduCount = MAX_RLC_SDU_IN_PDU_FOR_KPI;
                else
                    sduCount = rlcDataBufferTB[lc].sduCount;
                /*SPR_8403_CHANGES_START*/
                /* 100MS_SUPPORT : start */  
                if(sduCount == 0 ||  rlcDataBufferTB[lc].requestedDataSize == 0)
                {
                    continue;
                }
                /* 100MS_SUPPORT : end */  

                /* Instead of calculating the delay using Current Tick we
                 * will calculate it using the Tick where HARQ was received */
                for(i=0; i < sduCount; i++)
                {
		   /* SPR 22296 Fix Start */
                   if(recvTTI > rlcDataBufferTB[lc].pktArrTime[i])
                   {
                    pLteMacKPIStats->totalPktDelayInDLPerQCI[qci] +=
                        recvTTI - rlcDataBufferTB[lc].pktArrTime[i];
                }
		   /* SPR 22296 Fix End */
                }
                /* SPR 12783 Changes End */
                pLteMacKPIStats->totalPktAckedInDLPerQCI[qci] += sduCount;
                /* SPR 22075 Changes Start */
                /* If condition has been moved before FOR loop as it has no
                 * impact of putting the condition after loop. 
                 * Here, in case of SRB garbage values
                 * were updated in the stats so now on moving SRB packets will
                 * be discarded */
                /* SPR 22075 Changes End */
                /*SPR_8403_CHANGES_END*/
            }
        }
    }
}

#endif

/* CA_phase2_harq_code Start */

/*****************************************************************************
 * Function Name  :  macDecodeDLAckNackFlag
 * Inputs         :  dlUeContext_p , dlAckNackFlag, isHarqOnPucch
 * Outputs        :  dlHarqFlag_p 
 * Returns        :  void.
 * Description    :  This function fills dlHarqFlag_p based on 
 *                   dlAckNackFlag received in harqPduIndication based
 *                   no of TB's configured per cell and Validation of each TB
 *                   from DLUEContext.
 *****************************************************************************/
void macDecodeDLAckNackFlag( DLUEContext *dlUeContext_p,
                             UInt16 dlAckNackFlag,
                             UInt8 *dlHarqFlag_p,
                             UInt8 isHarqOnPucch)
{
/*CID 109435*/
    UInt8 tbsSupportedCell[MAX_NUM_CELL] = {0,0};
    UInt8  numOfTxAnteenas  = 0; /*CID 110802*/
    InternalCellIndex servCellIndex  = 0; /*CID 110801*/
    InternalCellIndex cellIndex  = 0;   /*Coverity Fix 111440+-*/
/*CID 109435 fix*/

    UInt8  cellLoopCounter  = 0;
    UInt16 decodeAckNackFlag = 0;
    UInt8  ackNackType = 0;
    TransmissonMode transmissionMode = INVALID_TX_MODE;/*CID 110803*/
    DLUESCellContext *dlUeScellContext_p = PNULL;   /* CID Fix 111515 */

    /*
     * Num of TB's supported by Primary Cell and Secondary Cell
     */
    for(cellLoopCounter = 0 ; cellLoopCounter <= dlUeContext_p->scellCount;
            cellLoopCounter++)
    {
        if(!cellLoopCounter)
        {
            cellIndex = dlUeContext_p->internalCellIndex;/*Coverity Fix 111440+-*/
            transmissionMode = dlUeContext_p->dlMIMOInfo.transmissionMode; /*CID 110803*/
            servCellIndex = cellLoopCounter;/*CID 110801*/
        }
        else
        {
            if(PNULL == dlUeContext_p->dlUeScellContext_p[cellLoopCounter])
            {
                continue;
            }
            /* CID Fix 111515 +*/
            dlUeScellContext_p = dlUeContext_p->\
                                 dlUeScellContext_p[cellLoopCounter];
            /* CID Fix 111515 -*/
            cellIndex = dlUeScellContext_p->internalCellIndex;/*Coverity Fix 111440+-*/


            transmissionMode = dlUeScellContext_p->\
                               ScelldlMIMOInfo.transmissionMode; /*CID 110803*/
            servCellIndex = cellLoopCounter;/*CID 110801*/

        }

        numOfTxAnteenas =
            cellSpecificParams_g.cellConfigAndInitParams_p[cellIndex]
            ->cellParams_p->numOfTxAnteenas;

        TBS_SUPPORTED_IN_CELL(transmissionMode , numOfTxAnteenas , servCellIndex);
    }

  /*
   * Decoding No of Ack/Nacks Received
   */

   if(dlAckNackFlag == 1 ) 
   {
       dlAckNackFlag = 0x11;
   }
   decodeAckNackFlag = dlAckNackFlag;

   DECODE_ACK_NACK_TYPE(decodeAckNackFlag,ackNackType,isHarqOnPucch)

   /*
    * Below logic is based on section 12.1
    * HARQ on PUCCH  and HARQ on PUSCH
    * from CA Design Document
    */
   switch(ackNackType)
   {

       /* 
        * LSB (first byte) contains harq status of first cell
        * and MSB Second byte contians harq status of secondary cell
        */

       case RCVD_ACK_NACK_4_ON_PUCCH : 
       case RCVD_ACK_NACK_4_ON_PUSCH : 

        dlHarqFlag_p[HARQ_PRIMARY_CELL] = (dlAckNackFlag) & 0xFF;
        dlHarqFlag_p[HARQ_SECONDARY_CELL] = (dlAckNackFlag >> 8) & 0xFF;

       break;

       case RCVD_ACK_NACK_3_ON_PUCCH : 
       if(tbsSupportedCell[HARQ_SECONDARY_CELL] == TWO_TB_SUPPORTED)
       {
           /* 
            * LSB (first byte) contains harq status of Secondary cell
            * and 4 least bits (0-3) of MSB (second byte) contians 
            * harq status of Primary cell
            */
           dlHarqFlag_p[HARQ_SECONDARY_CELL] = (dlAckNackFlag) & 0xFF;
           dlHarqFlag_p[HARQ_PRIMARY_CELL] = (dlAckNackFlag >> 8) & 0xFF;
       }
       else
       {
           /* 
            * LSB (first byte) contains harq status of Primary cell
            * and 4 least bits (0-3) of MSB (second byte) contians 
            * harq status of Secondary cell
            */
           dlHarqFlag_p[HARQ_PRIMARY_CELL] = (dlAckNackFlag) & 0xFF;
           dlHarqFlag_p[HARQ_SECONDARY_CELL] = (dlAckNackFlag >> 8) & 0xFF;

       }

       break;

       case RCVD_ACK_NACK_3_ON_PUSCH : 
       if(tbsSupportedCell[HARQ_SECONDARY_CELL] == TWO_TB_SUPPORTED)
       {
           /* 
            * 4 least bits (0 -3) of LSB (first byte) contains 
            * harq status of Primary cell
            * and 4 most bits (4 - 7) of LSB (first byte) and 
            * 4 least bits (0-3) of MSB (second byte) contians 
            * harq status of Secondary cell
            */
           dlHarqFlag_p[HARQ_PRIMARY_CELL] = (dlAckNackFlag) & 0xF;
           dlHarqFlag_p[HARQ_SECONDARY_CELL] = (dlAckNackFlag >> 4) & 0xFF;
       }
       else
       {
           /* 
            * LSB (first byte) contains harq status of Primary cell
            * and 4 least bits (0-3) of MSB (second byte) contians 
            * harq status of Secondary cell
            */
           dlHarqFlag_p[HARQ_PRIMARY_CELL] = (dlAckNackFlag) & 0xFF;
           dlHarqFlag_p[HARQ_SECONDARY_CELL] = (dlAckNackFlag >> 8) & 0xFF;
       }

       break;

       /* 
        * Occurs in case of each cell is configured with 
        * one TB or harq collides with CSI 
        * Four least significant bits of (0-3) byte contains 
        * harq status of first cell
        * and four most significant bits of (4-7) byte contians 
        * harq status of secondary cell
        * index 0 stands for primary cell harq TB info
        * index 1 stands for Seconday cell harq TB info
        */

       case RCVD_ACK_NACK_2_ON_PUCCH : 
       if(tbsSupportedCell[HARQ_SECONDARY_CELL] == TWO_TB_SUPPORTED && 
          tbsSupportedCell[HARQ_PRIMARY_CELL] != TWO_TB_SUPPORTED )
       {
           dlHarqFlag_p[HARQ_SECONDARY_CELL] = (dlAckNackFlag) & 0x0F;
           dlHarqFlag_p[HARQ_PRIMARY_CELL] = (dlAckNackFlag >> 4) & 0x0F;

       }
       else
       {
           dlHarqFlag_p[HARQ_PRIMARY_CELL] = (dlAckNackFlag) & 0x0F;
           dlHarqFlag_p[HARQ_SECONDARY_CELL] = (dlAckNackFlag >> 4) & 0x0F;
       }

       break;

       case RCVD_ACK_NACK_2_ON_PUSCH : 

       dlHarqFlag_p[HARQ_PRIMARY_CELL] = (dlAckNackFlag) & 0x0F;
       dlHarqFlag_p[HARQ_SECONDARY_CELL] = (dlAckNackFlag >> 4) & 0x0F;

       break;
   }
   /* CA_phase2_harq_ut_code End */
}

/* CA_phase2_harq_code End*/
/*****************************************************************************
 * Function Name  :  processDLAckNackQueue
 * Inputs         :  subFrame,
 *                   globalTick
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function reads from the AckNackQueue and does the 
 *                   requisites in accordance whether ACk has been received or 
 *                   NACK has been received.
 *****************************************************************************/
#ifdef FDD_CONFIG
                                                 /* SPR 15909 fix start */
/* +- SPR 17777 */
MacRetType processDLAckNackQueue(tickType_t globalTick,
                                                 /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
{
    AckNackQueueNode *ackNackNode_p = PNULL;
    /* + SPR 17733 */
    /* +- SPR 18490 */
#ifndef DL_UL_SPLIT
#ifndef CIRC_QUEUE_IMPL
    UInt32 ackNackQueueCountForTcrnti = 0;
    TempCRNTICtx *tempUEContext_p = PNULL;
    UInt8 operation = 0;
#endif    
#endif    
    /* +- SPR 18490 */
    /* - SPR 17733 */
    /* CA_phase2_harq_code Start */
#ifndef DL_UL_SPLIT
    UInt32 ackNackQueueCount = 0;
#endif    
    /* CA_phase2_harq_code End */
    UInt16 ueIndex =0;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    UInt32 subFrameNumber = 0;
    DLHarqTimerEntityList* dlHarqEntityTimerList_p = PNULL;
    UInt8 tempHarqID = INVALID_HARQ_ID;

    /* CA_phase2_harq_code Start */
    UInt8   oprTb1 = 0; 
    UInt8   oprTb2 = 0;
    UInt8   arrIndex = 0;
    UInt8   ueServCellIndex  = 0;
    UInt8   sCellCount  = 0;
    UInt8  isHarqOnPucch = FALSE;
    UInt8   nackFlag[MAX_NUM_CELL] = {FALSE,};
    UInt8   dlHarqFlag[MAX_NUM_CELL] = {0,}; 
    
    InternalCellIndex ueInternalCellIndex = 0;

    UInt16  dlAckNackFlag = 0; 
    UInt16  dlHarqTimerArrayIndex = INVALID_UE_ID;/*SPR 18490 +-*/


    DLUEContext*      dlUeContext_p       = PNULL;
    DLHARQProcess*    dlHarqProcess_p     = PNULL;
    DLHARQEntity*     dlHarqEntity_p      = PNULL;
    DLHarqTimerNode*  dlharqTimerNode_p   = PNULL;
    DLUESCellContext* dlUeScellContext_p  = PNULL;
    DLCQIInfo*        dlCQIInfo_p = NULL;
    DLMIMOInfo*       dlMIMOInfo_p = PNULL;

    /* 
     * For CA UE both retrans cell info shall 
     * be enqueued into retrans Queue of primary
     * cell
     */
    DLRetxQueueNode  retxNode; // for filling Retx Node of Sec Cell

    /* CA_phase2_harq_code End */
    /* CA_phase2_csi_code Start */
    /* bit 0 will be set for Pcell, bit 1 set for scell with servCellIndex 1 and
     * so on */
    UInt8 bitMaskForAperiodicReport;
    /* CA_phase2_csi_code End */

    /* + CQI_5.0 */
    ULUEContext* ueULContext_p  = PNULL;
    LP_MacDlSpsStatus macDlSpsStatus_p = PNULL;
#ifdef PERF_STATS
    UInt32 dlBytesSent = 0;
#endif

    /* - CQI_5.0 */
    /* SPS_CHG */
    /*! \fn static MacRetType processDLAckNackQueue(
     *                                             UInt32 subFrame,
     *                                             UInt32 globalTick) 
     *  \brief Function Modification
     * */
    UInt8 maxRetxCount = 0;
    /* SPS_CHG */

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

#ifndef DL_UL_SPLIT
    ackNackQueueCount = sQueueCount(ackNackQueue_gp[internalCellIndex]);

    LOG_UT(MAC_DL_HARQ_ACK_NACK_QUEUE_COUNT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            ackNackQueueCount,\
            DEFAULT_INT_VALUE ,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
#ifndef CIRC_QUEUE_IMPL
 /* + SPR 17733 */
    ackNackQueueCountForTcrnti = sQueueCount((ackNackQueueForTcrnti_gp[internalCellIndex]));
    while(ackNackQueueCountForTcrnti)
    {
        ackNackNode_p = (AckNackQueueNode *)popNode((ackNackQueueForTcrnti_gp[internalCellIndex]));
        if(PNULL != ackNackNode_p)
        {
            ueIndex = ackNackNode_p->ueIndex;
            operation = ackNackNode_p->operationType;
            subFrameNumber = (ackNackNode_p->packetSubFrame & 0x00ff);
            tempUEContext_p =
                tempCrntiCtx_gp[internalCellIndex]->tempCRNTICtxArray[ueIndex].tempCRNTICtx_p;
            if(PNULL == tempUEContext_p)
            {
                freeMemPool(ackNackNode_p);
                ackNackQueueCountForTcrnti--;
                continue;
            }
            if (MAC_FAILURE ==
                    processDLAckNackForMsg4(tempUEContext_p,subFrameNumber,operation,internalCellIndex))
            {
                freeMemPool(ackNackNode_p);
                ackNackQueueCountForTcrnti--;
                continue;
            }
            freeMemPool(ackNackNode_p);
        }
        ackNackQueueCountForTcrnti--;
    } /* End of WHILE */
    /* - SPR 17733 */
#endif
#endif

#ifndef DL_UL_SPLIT
    while(ackNackQueueCount)
#else
    // Let the loop continues forever till it receives a delimiter Node 
    // NOTE :: CHECKING REQUIRED SO THAT IT CANNOT HOG THE CPU and LET OTHER THREADS SCHEDULE EVEN WHEN NO DATA IS THERE 
    while(1)
#endif
        {
           /* CA_phase2_csi_code Start */
            bitMaskForAperiodicReport = 0;
           /* CA_phase2_csi_code End */
#ifndef DL_UL_SPLIT
            LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,"[%s]ackNackQueueCount %d ",
                    __func__,ackNackQueueCount);
            ackNackNode_p = (AckNackQueueNode *)popNode(ackNackQueue_gp[internalCellIndex]);
#else
#ifdef CIRC_QUEUE_IMPL
            semWaitDefault(&waitForDLHarqAckNack_g[internalCellIndex]);
            DEQUEUE_ACK_NACK_CIRC_Q(AckNackQueueNode, (void *)&ackNackNode_p, internalCellIndex);
#else
            /* +- SPR 17777 */
            recvMsgWithIccQ(ackNackQueueIdDL_g[internalCellIndex], (const void *)&ackNackNode_p);
            /* +- SPR 17777 */
#endif
#endif
            if(PNULL != ackNackNode_p)
            {
                /* CA_phase2_harq_code Start */
                /*
                 * Intializing of HARQ ReTx Node
                 */
                SET_HARQ_RETX_NODE(retxNode); 

                isHarqOnPucch = FALSE;

                /* CA_phase2_harq_code End */
                /* SPR 21029 fix start */
                dlHarqFlag[PCELL_SERV_CELL_IX]  = RESET;
                /* SPR 21029 fix end */
                /* SPR 21997 Fix Start */
                dlHarqFlag[START_SCELL_INDEX]  = RESET;
                /* SPR 21997 Fix End */
                ueIndex = ackNackNode_p->ueIndex;
                /* CA_phase2_harq_code Start */
                dlAckNackFlag = ackNackNode_p->operationType;
                /* CA_phase2_harq_code End */
#ifdef DL_UL_SPLIT            
                if (ueIndex == INVALID_UE_ID)
                {
                    /* Delimeter node has been received so no more ACK/NACKs will be received in his tick so
                     * Move out of it and move to next EL*/
                    // Free the memory of delimeter node
                    FREE_MEM_NODE_ACK_NACK_CIRC_Q(ackNackNode_p);
                    /*Change DROP1*/
                    semWait(&waitForDLPerfStatsUpd_g[internalCellIndex]);
                    /*Change DROP1*/
                    break;
                }
#endif
                /* ----SPR 3702 start ----*/
                subFrameNumber = (ackNackNode_p->packetSubFrame & 0x00ff);
                /* ----SPR 3702 end ----*/

                /* EICIC +*/
                eicicHarqSubframeNumber_g = subFrameNumber;

                UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_HARQ_RECEIVED(internalCellIndex)


                /* Check if subframe number is < 4 then decrease the system frame number
                       by 1 as the UL grant has been done in N-4 subframe*/	

                if (subFrameNumber < FDD_HARQ_OFFSET)
                { 
                    eicicHarqSystemframeNumber_g = eicicHarqSystemframeNumber_g - 1;
                    eicicHarqSubframeNumber_g = MAX_SUB_FRAME + subFrameNumber - FDD_HARQ_OFFSET;

                }
                else
                {
                    eicicHarqSubframeNumber_g = subFrameNumber - FDD_HARQ_OFFSET;
                }

                /* EICIC -*/

                LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,"[%s]For ueIndex  %d"\
                        " subFrameNumber %d"\
                        ,__func__,ueIndex, ackNackNode_p->packetSubFrame);
                LOG_MAC_MSG(MAC_DL_HARQ_ACK_NACK_NODE_INFO,LOGBRIEF,MAC_DL_HARQ,\
                        getCurrentTick(),
                        ueIndex,\
                        DEFAULT_INT_VALUE,\
                        subFrameNumber,\
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                        DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

                LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,"[%s]For ueIndex  %d"\
                               " subFrameNumber %d"\
                               ,__func__,ueIndex, ackNackNode_p->packetSubFrame);
                LOG_MAC_MSG(MAC_DL_HARQ_ACK_NACK_NODE_INFO,LOGBRIEF,MAC_DL_HARQ,\
                      getCurrentTick(),
                      ueIndex,\
                      DEFAULT_INT_VALUE,\
                      subFrameNumber,\
                      DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                      DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
                dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
                /* + CQI_5.0 */
                ueULContext_p  = (&ulUECtxInfoArr_g[ueIndex])->ulUEContext_p;
                /* - CQI_5.0 */
                if (!dlUeContextInfo_p->pendingDeleteFlag)
                {
                    dlUeContext_p = PNULL;
                    dlHarqProcess_p = PNULL;
                    dlHarqEntity_p = PNULL;
                    dlharqTimerNode_p = PNULL;
                    /* +- coverity 111606 */

                    dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;


                    if(dlUeContext_p->internalCellIndex != internalCellIndex)
                    {
                        FREE_MEM_NODE_ACK_NACK_CIRC_Q(ackNackNode_p);
#ifndef DL_UL_SPLIT
                        ackNackQueueCount--;
#endif
                        continue;
                    }
                    dlHarqEntityTimerList_p =  
                        &(dlHarqGlobals_g[internalCellIndex].\
                                dlHarqTimerEntityList[subFrameNumber]);

                    sCellCount = dlUeContext_p->scellCount;

                    CHECK_IS_HARQ_BLER_RECEIVED_ON_PUCCH
                        (ueIndex, subFrameNumber, isHarqOnPucch);
                    /* + SPR_15971 */ 
                    if(sCellCount)
                    /* - SPR_15971 */ 
                    {
                        macDecodeDLAckNackFlag( dlUeContext_p,
                                dlAckNackFlag, dlHarqFlag,isHarqOnPucch);
                    }
                    else
                    {
                        /*
                         * In case of UE has only primary cell
                         * or for Non CA UE
                         */
                        /* + SPR_15971 */ 
                        dlHarqFlag[PCELL_SERV_CELL_IX] |= dlAckNackFlag & 0x00FF;

                        /* In case Scell is added and reported format and format is 1A */
                        if (sCellCount)
                        { 
                            dlHarqFlag[START_SCELL_INDEX] =  0x11;
                        }
                        /* - SPR_15971 */ 
                    }

#ifdef MAC_AUT_TEST
                    macAutDlHarqFlagVer_g[0] = dlHarqFlag[0];
                    macAutDlHarqFlagVer_g[1] = dlHarqFlag[1];
#endif

                    for(ueServCellIndex = 0; ueServCellIndex <= sCellCount ;
                        ueServCellIndex++)
                    {
                        if(!ueServCellIndex)
                        {
                            dlCQIInfo_p = &(dlUeContext_p->dlCQIInfo);
                            dlHarqEntity_p= dlUeContext_p->dlHarqContext_p;
                            ueInternalCellIndex = internalCellIndex;
                            dlMIMOInfo_p = &(dlUeContext_p->dlMIMOInfo);
                        }
                        else
                        {
                           /* SPR 21801 Fix + */
                           if((PNULL == dlUeContext_p->dlUeScellContext_p[ueServCellIndex]) ||
                             (PNULL == dlUeContext_p->dlUeScellContext_p[ueServCellIndex]->ScelldlHarqContext_p))
                           {
                               continue;
                           }
                           /* SPR 21801 Fix - */

                            arrIndex  = ueServCellIndex - 1;

                            dlUeScellContext_p =
                                dlUeContext_p->dlUeScellContext_p[ueServCellIndex];
                            dlHarqEntity_p =
                                dlUeScellContext_p->ScelldlHarqContext_p;
                            dlCQIInfo_p =
                                &(dlUeScellContext_p->ScelldlCQIInfo);
                            ueInternalCellIndex =
                                dlUeScellContext_p->internalCellIndex;
                            dlMIMOInfo_p =
                                &(dlUeScellContext_p->ScelldlMIMOInfo);

                        }

                        dlHarqTimerArrayIndex = 
                            dlHarqEntity_p->\
                            dlHarqTimerNodeArrrayIndex[subFrameNumber];
                        /* CA_phase2_harq_code End */

                        /* SPR 2320 changes start */
                        if (INVALID_COUNT == dlHarqTimerArrayIndex)
                        {
                            /* CA_phase2_harq_code Start */

                            /*
                             * Node is freeing 
                             * after 
                             * if(!dlUeContextInfo_p->pendingDeleteFlag)
                             */

                            /*
                             * Next serving cell of UE will 
                             * be processed
                             */
                            continue;
                            /* CA_phase2_harq_code End */
                        }
                        /* SPR 2320 changes end */
                        /* DL_HARQ_OPT */    
#ifdef ACK_NACK_COUNT_STATS
                        UInt8 statsTB1 = 0;
                        UInt8 statsTB2 = 0; 
                        if (dlUeContext_p->dlMIMOInfo.transmissionMode == TX_MODE_3 ||
                                dlUeContext_p->dlMIMOInfo.transmissionMode == TX_MODE_4 ||
                                /* + TM7_8 Changes Start */
                                ((dlUeContext_p->dlMIMOInfo.transmissionMode == TX_MODE_8) && 
                                 (1 < cellSpecificParams_g.cellConfigAndInitParams_p[ueInternalCellIndex]->cellParams_p->\
                                  numOfTxAnteenas)))
                            /* - TM7_8 Changes End */
                        {
                            statsTB1 = MIMO_TB_ONE;
                            statsTB2 = MIMO_TB_TWO;
                        }
                        else
                        {
                            statsTB1 = NON_MIMO;
                            statsTB2 = NON_MIMO;
                        }
#endif
                        /* DL_HARQ_OPT */    

                        dlharqTimerNode_p = 
                            &(dlHarqEntityTimerList_p->
                                    dlHarqTimerNodeArr[dlHarqTimerArrayIndex]);
                        tempHarqID = dlharqTimerNode_p->harqProcessId;

                        /* + SPR 11201 Changes */
                        retxNode.ueIndex = ueIndex;
                        /* - SPR 11201 Changes */
                        /* CA_phase2_harq_code Start */
                        if(!ueServCellIndex)
                        {
                            retxNode.harqProcessId = tempHarqID;
                            retxNode.schUsed = ackNackNode_p->schUsed;

                        }
                        else
                        {
                            retxNode.sCellRetxInfo[arrIndex].harqProcessId =
                                tempHarqID;
                            retxNode.sCellRetxInfo[arrIndex].servCellIndex =
                                ueServCellIndex; 
                            /*
                             * According to 12.7.2 in 
                             * CA Arch Design Doc
                             * HARQ PROC will not be allocated with 
                             * SPS Scheduling in Secondary Cells
                             */
                            retxNode.sCellRetxInfo[arrIndex].schUsed = 
                                DRA_SCHEDULING; 
                        }
                        /* CA_phase2_harq_code End */

                        LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,\
                                "[%s]subFrameNumber %d global Tick %d"\
                                " Inside IF of pendingDeleteFlag for ueIndex %d,"\
                                "dlharqTimerNode_p->ueIndex %d  harq Process ID %d,"\
                                "dlHarqTimerArrayIndex %d , isValid %d  #########\n",\
                                __func__,subFrameNumber , getCurrentTick(),ueIndex,\
                                dlharqTimerNode_p->ueIndex, tempHarqID , \
                                dlHarqTimerArrayIndex,dlharqTimerNode_p->isValid );

                        LOG_MAC_MSG(MAC_DL_HARQ_ACK_NACK_NODE_INFO,LOGBRIEF,MAC_DL_HARQ,\
                                getCurrentTick(),
                                tempHarqID,\
                                dlHarqTimerArrayIndex,\
                                dlharqTimerNode_p->isValid,\
                                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

                        /*
                           Since calling the dlHarqTimerStop would be an overhead as we are 
                           in any case have to access the global structure of the TIMER node
                           hence in the next few lines are performing the function of dlHarqTimerStop
                           This would help in improving the our performance.
                           */

                        /* SPR 2320 changes start */
                        if (HARQ_TIMER_NODE_VALID == dlharqTimerNode_p->isValid) 
                            /* SPR 2320 changes end */
                        {
                            /* DL_HARQ_OPT */
                            /*UInt8 isHarqOnPucch = FALSE;*/
                            /* DL_HARQ_OPT */	
                            LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,\
                                    "[%s]Inside dlharqTimerNode_p->isValid = %d\n",__func__,dlharqTimerNode_p->isValid);

                            dlharqTimerNode_p->isValid = HARQ_TIMER_NODE_INVALID;

                            dlHarqProcess_p =  &(dlHarqEntity_p->harqProcess[tempHarqID]);

                            LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,\
                                    "[%s]After setting it INVALID dlharqTimerNode_p->isValid = %d\n",
                                    __func__,dlharqTimerNode_p->isValid);
                            /*
                               We now fill the array dlHarqTimerNodeIndex with an invalid entry.
                               */
                            /* SPR 6136 start */
                            /* SPR 6136 end */

                            /* CA_phase2_harq_code Start */
                            dlHarqEntity_p->\
                                dlHarqTimerNodeArrrayIndex[subFrameNumber] = INVALID_COUNT;
                            /* CA_phase2_harq_code End */
                            /*SPR 20732 Fix Start*/
                            /*SPR 15208 Fix*/ 
                            handleDLTimingAdvance(dlUeContext_p,internalCellIndex);
                            /*SPR 15208 Fix*/ 
                            /*SPR 20732 Fix End*/

                            LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ, 
                                    "[%s]  \n current_TX_NB_TB_ONE = %d current_TX_NB_TB_TWO = %d"
                                    "assignedResourcesNB = %d \n",__func__,
                                    dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB,
                                    dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB,
                                    dlHarqProcess_p->assignedResourcesNB);
                            /* DL_HARQ_OPT */    
                            if (dlHarqProcess_p->isTBOneValid && 
                                dlHarqProcess_p->isTBTwoValid)
                            {
                                /* CA_phase2_harq_code Start */

                                oprTb1 = dlHarqFlag[ueServCellIndex] & 0x07; 
                                oprTb2 = (dlHarqFlag[ueServCellIndex] & 0x70)>>4;
                                /* CA_phase2_harq_code End */


                                /* Check if the HARQ information is received on PUCCH */
                                /* SPR 5033 changes start*/
                                /* Update isHarqOnPucch to indicate whether the HARQ is 
                                 * received on PUCCH or not */

                                /* CA_phase2_harq_code Start */


                                if(TRUE ==  isHarqOnPucch)
                                {
                                    /*
                                     * Harq is received on PUCCH
                                     * Below logic is based on section 12.3
                                     * HARQ collision with SR on PUCCH from CA Design Document
                                     *
                                     * If both TBs are scheduled on a serving cell, 
                                     * the corresponding bit shall indicate ACK/Nack status of both TBs 
                                     * i.e., both TBs shall be either acked or nacked
                                     *
                                     */

                                    if(!oprTb2)
                                    {
                                        oprTb2 = oprTb1;
                                    }

                                    /* Cyclomatic Complexity changes - starts here */
                                    /* +- SPR 17777*/
                                    isHarqOnPucchRecvdOnTBOneTBTwo(
                                            isHarqOnPucch,oprTb1,oprTb2
                                            ,dlUeContext_p, globalTick,
                                            ueInternalCellIndex);
                                    /* Cyclomatic Complexity changes - ends here */
                                }
                                /* CA_phase2_harq_code End */

                                /* Convert the operation into the format mentioned in above note */ 
                                if (oprTb1 == ACK && oprTb2 == ACK)
                                    // Both TB ACK scenario
                                {
#ifdef ACK_NACK_COUNT_STATS
                                    lteCommonUpdateAckNackStats(statsTB1, ueIndex, DL_ACK);
                                    lteCommonUpdateAckNackStats(statsTB2, ueIndex, DL_ACK);
#endif

#ifdef PERF_STATS
                                    UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_ACK ,
                                            dlHarqProcess_p->dlHARQTBOneInfo.mcsIndex);

                                    UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_ACK ,
                                            dlHarqProcess_p->dlHARQTBTwoInfo.mcsIndex);
                                    dlBytesSent += dlHarqProcess_p->dlHARQTBOneInfo.tbSize +\
                                                   dlHarqProcess_p->dlHARQTBTwoInfo.tbSize;
#endif
                                    /* 32.425 Events code changes start*/
#ifdef KPI_STATS
                                    /* This will update the Shared Memory Time and Shared Memory Data */ 
                                    /* +- SPR 17777*/
                                    lteMacUpdateKpiStatsIPThpDlOnHarqAck(ueIndex,dlHarqProcess_p, ackNackNode_p->recvTTI
#ifdef TDD_CONFIG
                                            , internalCellIndex
#endif
                                            );
                                    /* +- SPR 17777*/
                                    /* SPR 11402 Fix  */
                                    /* SPR 16843 start */
                                    updateKpiStatsForTotalSduAckRcvdDlperQci(&dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex,ueIndex);
                                    updateKpiStatsForTotalSduAckRcvdDlperQci(&dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex,ueIndex);
                                    /* SPR 16843 end */
                                    /* SPR 11402 Fix  */

#endif
                                    /* 32.425 Events code changes end*/
                                    /* + CQI_5.0 */
                                    /* BLER calculation for TB1 */
                                    /* Cyclomatic Complexity changes - starts here */
                                    /* CA_phase2_harq_code Start */
                                    calculateDLBlerForAckTB1(globalTick, 
                                            ueULContext_p,
                                            dlCQIInfo_p,
                                            ueIndex, 
                                            internalCellIndex,
                                            /* CA_phase2_csi_code Start */
                                            ueServCellIndex,
                                            &bitMaskForAperiodicReport,
                                            dlMIMOInfo_p);
                                            /* CA_phase2_csi_code End */

                                    /* BLER calculation for TB2 */
                                    calculateDLBlerForAckTB2(globalTick, 
                                            ueULContext_p,
                                            dlCQIInfo_p,
                                            /* +- SPR 17777 */
                                            ueIndex, 
                                            internalCellIndex,
                                            /* CA_phase2_csi_code Start */
                                            ueServCellIndex,
                                            &bitMaskForAperiodicReport);
                                            /* CA_phase2_csi_code End */
                                    /* CA_phase2_harq_code End */
                                    /* Cyclomatic Complexity changes - ends here */
                                    /* - CQI_5.0 */

                                    if ((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag
                                                == TA_MSG) ||
                                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag
                                             == TA_DRX_MSG))
                                    {
                                        dlUeContext_p->lastTASentTick =
                                            getCurrentTick() - PHY_UL_DATA_DELAY - FDD_HARQ_OFFSET;
                                    }
                                    /*SPR 17561 FIX*/
#ifdef FDD_CONFIG
                                    /*SPR 16026 changes start*/ 
                                    if((dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag != INVALID_SCELL_ACT_DEACT_MSG) || \
                                         (dlHarqProcess_p->dlHARQTBTwoInfo.macScellCEFlag != INVALID_SCELL_ACT_DEACT_MSG))
                                    {
                                            ScellStartDlCATimer(dlUeContext_p,dlHarqProcess_p);
                                    }
                                    /*SPR 16026 changes end*/ 
#endif
                                    /*SPR 17561 FIX*/
#ifdef KPI_STATS
                                    /*Update the pktDelay KPI counters for packets in 
                                     * DL which are positively acked based on HARQ
                                     * feedback*/
                                    /* SPR 12783 Changes Start */
                                    macUpdateKpiCntrsForPktDelayPerQci(&dlHarqProcess_p->dlHARQTBOneInfo, ueIndex, 1,
                                            internalCellIndex, ackNackNode_p->recvTTI);
                                    macUpdateKpiCntrsForPktDelayPerQci(&dlHarqProcess_p->dlHARQTBTwoInfo, ueIndex, 2,
                                            internalCellIndex, ackNackNode_p->recvTTI);
                                    /* SPR 12783 Changes End */
#endif
                                    freeDLHarqProcess(dlUeContext_p, dlHarqProcess_p, ueInternalCellIndex);
                                }
                                else if (oprTb1 == ACK)
                                {
#ifdef ACK_NACK_COUNT_STATS
                                    lteCommonUpdateAckNackStats(statsTB1, ueIndex, DL_ACK);
                                    lteCommonUpdateAckNackStats(statsTB2, ueIndex, DL_NACK);
#endif

#ifdef PERF_STATS
                                    UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_ACK ,
                                            dlHarqProcess_p->dlHARQTBOneInfo.mcsIndex);

                                    UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_NACK ,
                                            dlHarqProcess_p->dlHARQTBTwoInfo.mcsIndex);
                                    dlBytesSent += dlHarqProcess_p->dlHARQTBOneInfo.tbSize ;
#endif
                                    /* 32.425 Events code changes start*/
#ifdef KPI_STATS
                                    /* This will update the Shared Memory Time and Shared Memory Data */ 
                                    /* +- SPR 17777*/
                                    lteMacUpdateKpiStatsIPThpDlOnHarqAck(ueIndex,dlHarqProcess_p, ackNackNode_p->recvTTI
#ifdef TDD_CONFIG
                                            , internalCellIndex
#endif
                                            );
                                    /* +- SPR 17777 */
                                    /* SPR 11402 Fix  */
                                    /* SPR 16843 start */
                                    updateKpiStatsForTotalSduAckRcvdDlperQci(&dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex,ueIndex);
                                    /* SPR 16843 end */
                                    /* SPR 11402 Fix  */
#endif
                                    /* 32.425 Events code changes end*/

                                    /* + CQI_5.0 */
                                    /* TDD SB_CQI */
                                    /* BLER calculation for TB1 */
                                    /* Cyclomatic Complexity changes - starts here */
                                    /* CA_phase2_harq_code Start */
                                    calculateDLBlerForAckTB1(globalTick, \
                                            ueULContext_p,dlCQIInfo_p, 
                                            ueIndex,
                                            internalCellIndex,
                                            /* CA_phase2_csi_code Start */
                                            ueServCellIndex,
                                            &bitMaskForAperiodicReport,
                                            dlMIMOInfo_p);
                                    /* CA_phase2_csi_code End */


                                    /* BLER calculation for TB2 */
                                    calculateDLBlerForNackTB2(globalTick, \
                                            ueULContext_p,dlCQIInfo_p,
                                            /* +- SPR 17777 */
                                            ueIndex,ueInternalCellIndex,
                                            /* CA_phase2_csi_code Start */
                                            ueServCellIndex,
                                            &bitMaskForAperiodicReport);
                                    /* CA_phase2_csi_code End */
                                    /* CA_phase2_harq_code End */
                                    /* Cyclomatic Complexity changes - ends here */
                                    /* TDD SB_CQI */
                                    /* - CQI_5.0 */
                                    /*SPR 17561 FIX*/
#ifdef FDD_CONFIG
                                    /*SPR 16026 changes start*/ 
                                    if(dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag != INVALID_SCELL_ACT_DEACT_MSG)
                                    {
                                        ScellStartDlCATimer(dlUeContext_p,dlHarqProcess_p);
                                    }
                                    /*SPR 16026 changes end*/ 
#endif
                                    /*SPR 17561 FIX*/

                                /* SPR 16474 Fix Start */
                                if ((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag
                                            == TA_MSG) ||
                                        (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag
                                         == TA_DRX_MSG))
                                {
                                    dlUeContext_p->lastTASentTick =
                                        getCurrentTick() - PHY_UL_DATA_DELAY - FDD_HARQ_OFFSET;
                                }
                                /* SPR 16474 Fix End */

                                    /* Cyclomatic Complexity changes - starts here */
                                /* +- SPR 17777 */
                                    processDLAckForTBOneNackForTBTwo 
                                        (ueIndex,
                                            dlHarqProcess_p, dlUeContext_p
                                            /* CA_phase2_harq_code Start */
                                            ,&nackFlag[ueServCellIndex] 
                                            /* CA_phase2_harq_code End */
                                            ,ueInternalCellIndex);
                                    /* Cyclomatic Complexity changes - ends here */
                                }
                                else if (oprTb2 == ACK) /* oprTb2 = ACK */ 
                                {
                                    /* SPR 3679 Changes Start */
#ifdef ACK_NACK_COUNT_STATS
                                    lteCommonUpdateAckNackStats(statsTB1, ueIndex, DL_NACK);
                                    lteCommonUpdateAckNackStats(statsTB2, ueIndex, DL_ACK);
#endif

#ifdef PERF_STATS
                                    UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_NACK ,
                                            dlHarqProcess_p->dlHARQTBOneInfo.mcsIndex);

                                    UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_ACK ,
                                            dlHarqProcess_p->dlHARQTBTwoInfo.mcsIndex);
                                    dlBytesSent += dlHarqProcess_p->dlHARQTBTwoInfo.tbSize ;
#endif

                                    /* 32.425 Events code changes start*/
#ifdef KPI_STATS
                                    /* This will update the Shared Memory Time and Shared Memory Data */ 
                                    /* +- SPR 17777*/
                                    lteMacUpdateKpiStatsIPThpDlOnHarqAck(ueIndex,
                                            dlHarqProcess_p,
                                            ackNackNode_p->recvTTI
#ifdef TDD_CONFIG
                                            , internalCellIndex
#endif
                                            );
                                    /* +- SPR 17777 */
                                    /* SPR 11402 Fix  */
                                    /* SPR 16843 start */
                                    updateKpiStatsForTotalSduAckRcvdDlperQci(&dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex,ueIndex);
                                    /* SPR 16843 end */
                                    /* SPR 11402 Fix  */

#endif
                                    /* 32.425 Events code changes end*/
                                    /* - CQI_5.0 */
                                    /* TDD SB_CQI */
                                    /* BLER calculation for TB2 */
                                    /* Cyclomatic Complexity changes - starts here */
                                    /* CA_phase2_harq_code Start */
                                    calculateDLBlerForAckTB2(globalTick, \
                                            ueULContext_p,
                                            dlCQIInfo_p,
                                            /* +- SPR 17777 */
                                            ueIndex, 
                                            internalCellIndex,
                                            /* CA_phase2_csi_code Start */
                                            ueServCellIndex,
                                            &bitMaskForAperiodicReport);
                                            /* CA_phase2_csi_code End */

                                    /* BLER calculation for TB1 */
                                    calculateDLBlerForNackTB1(globalTick,
                                            ueULContext_p,dlCQIInfo_p, 
                                            ueIndex,ueInternalCellIndex,
                                            dlMIMOInfo_p,
                                            /* CA_phase2_csi_code Start */
                                            ueServCellIndex,
                                            &bitMaskForAperiodicReport);
                                            /* CA_phase2_csi_code End */
                                    /* CA_phase2_harq_code End */
                                    /* Cyclomatic Complexity changes - ends here */
                                    /* TDD SB_CQI */
                                    /* - CQI_5.0 */
                                    /*SPR 17561 FIX*/
#ifdef FDD_CONFIG
                                    /*SPR 16026 changes start*/ 
                                    if(dlHarqProcess_p->dlHARQTBTwoInfo.macScellCEFlag != INVALID_SCELL_ACT_DEACT_MSG)
                                    {
                                        ScellStartDlCATimer(dlUeContext_p,dlHarqProcess_p);
                                    }
                                    /*SPR 16026 changes end*/ 
#endif
                                    /*SPR 17561 FIX*/

                                    /* Cyclomatic Complexity changes - starts here */
                                    /* +- SPR 17777 */
                                    processDLAckForTBTwoNackForTBOne 
                                        (ueIndex,
                                         dlHarqProcess_p, dlUeContext_p
                                         /* CA_phase2_harq_code Start */
                                         ,&nackFlag[ueServCellIndex] 
                                         /* CA_phase2_harq_code End */
                                         ,ueInternalCellIndex
                                        );
                                    /* Cyclomatic Complexity changes - ends here */
                                }
                                else /* NACK on both TB */
                                {
#ifdef ACK_NACK_COUNT_STATS
                                    lteCommonUpdateAckNackStats(statsTB1, ueIndex, DL_NACK);
                                    lteCommonUpdateAckNackStats(statsTB2, ueIndex, DL_NACK);
#endif

#ifdef PERF_STATS
                                    UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_NACK ,
                                            dlHarqProcess_p->dlHARQTBOneInfo.mcsIndex);

                                    UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_NACK ,
                                            dlHarqProcess_p->dlHARQTBTwoInfo.mcsIndex);
#endif
                                    /* + CQI_5.0 */
                                    /* TDD SB_CQI */
                                    /* BLER calculation for TB1 */
                                    /* Cyclomatic Complexity changes - starts here */
                                    /* CA_phase2_harq_code Start */
                                    calculateDLBlerForNackTB1(globalTick,
                                            ueULContext_p,dlCQIInfo_p, 
                                            ueIndex,ueInternalCellIndex,
                                            dlMIMOInfo_p,
                                            /* CA_phase2_csi_code Start */
                                            ueServCellIndex,
                                            &bitMaskForAperiodicReport);
                                            /* CA_phase2_csi_code End */

                                    /* BLER calculation for TB2 */
                                    calculateDLBlerForNackTB2(globalTick,
                                            ueULContext_p,dlCQIInfo_p,
                                            /* +- SPR 17777 */
                                            ueIndex,ueInternalCellIndex,
                                            /* CA_phase2_csi_code Start */
                                            ueServCellIndex,
                                            &bitMaskForAperiodicReport);
                                            /* CA_phase2_csi_code End */
                                    /* CA_phase2_harq_code End */
                                    /* Cyclomatic Complexity changes - ends here */
                                    /* TDD SB_CQI */
                                    /* - CQI_5.0 */
                                    /*
                                       We now check for the re-transmission counter of both
                                       the TBs.
                                       */
                                    /* Cyclomatic Complexity changes - starts here */
                                    /* +- SPR 17777*/
                                    processDLNackForTBOneTBTwo
                                        (ueIndex,
                                         dlHarqProcess_p, dlUeContext_p
                                         /* + SPR 17745 */
#ifdef KPI_STATS                                         
                                         ,ackNackNode_p->recvTTI
#endif                                         
                                         /* - SPR 17745 */
                                         /* CA_phase2_harq_code Start */
                                         ,&nackFlag[ueServCellIndex] 
                                         /* CA_phase2_harq_code End */
                                         ,ueInternalCellIndex);
                                    /* +- SPR 17777*/
                                    /* Cyclomatic Complexity changes - ends here */

                                    /* SPR 4450 Fix Start */ 
                                    //break;
                                    /* SPR 4450 Fix End */ 
                                }
                            }
                            else if (dlHarqProcess_p->isTBOneValid &&
                                !dlHarqProcess_p->isTBTwoValid)
                            {

                                /* CA_phase2_harq_code Start */
                                oprTb1 = dlHarqFlag[ueServCellIndex] & 0x07; 

                                LP_SpsDlInfo spsDlInfo_p = &dlUeContext_p->spsDlInfo;

                                /* SPR 19704 + */
                                if ( (DRA_SCHEDULING == ackNackNode_p->schUsed) && 
                                     (TRUE == cellSpecificParams_g.
                                         cellConfigAndInitParams_p[ueServCellIndex]->
                                         initParams_p->spsSysWideParam.enableSps) &&
                                     ((PNULL != spsDlInfo_p) &&
                                         (PNULL != spsDlInfo_p->spsDlSetupInfo_p)) )
                                {
                                    if( (dlHarqProcess_p->harqProcessId <
                                            spsDlInfo_p->spsDlSetupInfo_p->numberOfConfSpsProcesses) || 
                                            (TRUE == dlHarqProcess_p->isSpsOnlyPaddingPresent) )
                                    {
                                        /* HARQ is coming on normal RNTI for
                                         * reserved HP-ID, hence assume the 
                                         * same as SPS */
                                        ackNackNode_p->schUsed = SPS_SCHEDULING;
                                        retxNode.schUsed = ackNackNode_p->schUsed;
                                    }
                                    else
                                    {
                                        ackNackNode_p->schUsed = DRA_SCHEDULING;
                                    }
                                }
                                /* SPR 19704 - */

                                /* Check if the HARQ information is received on PUCCH */
                                /* SPR 5033 changes start*/
                                /* Update isHarqOnPucch to indicate whether the HARQ is 
                                   received on PUCCH or not */


                               /*
                                * This CHECK_IS_HARQ_BLER_RECEIVED_ON_PUCCH
                                * is moved to above
                                */
                                
                                /* CA_phase2_harq_code End */

                                if(TRUE ==  isHarqOnPucch)
                                {
                                    /*SPR_4001_CHANGE_START*/
                                    /* Cyclomatic Complexity changes - starts here */
                                    /* CA_phase2_harq_code Start */
                                    updateAndCheckHarqBlerForPucch(dlUeContext_p,
                                            oprTb1, globalTick,ueInternalCellIndex);
                                    /* CA_phase2_harq_code End */
                                    /* Cyclomatic Complexity changes - ends here */
                                    /*SPR_4001_CHANGE_END*/
                                }
                                /* SPR 5033 changes end */
                                /* Update HARQ Debug Stats */
                                LTE_MAC_PER_UE_DEBUG_STATS(dlUeContext_p->ueIndex, 
                                      globalTick, tempHarqID, oprTb1, 
                                      dlHarqProcess_p->\
                                      dlHARQTBOneInfo.current_TX_NB,ueInternalCellIndex);
                                /* Convert the operation into the format mentioned in above note */ 
                                if (oprTb1 == ACK)
                                {
#ifdef ACK_NACK_COUNT_STATS
                                    lteCommonUpdateAckNackStats(statsTB1, ueIndex, DL_ACK);
#endif

#ifdef PERF_STATS
                                    UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_ACK ,
                                            dlHarqProcess_p->dlHARQTBOneInfo.mcsIndex);
                                    dlBytesSent += dlHarqProcess_p->dlHARQTBOneInfo.tbSize ;
#endif
                                    /* 32.425 Events code changes start*/
#ifdef KPI_STATS
                                    /* This will update the Shared Memory Time and Shared Memory Data */ 
                                    /* +- SPR 17777*/
                                    lteMacUpdateKpiStatsIPThpDlOnHarqAck(ueIndex,
                                            dlHarqProcess_p,
                                            ackNackNode_p->recvTTI
#ifdef TDD_CONFIG
                                            , internalCellIndex
#endif
                                            );
                                    /* +- SPR 17777 */
                                    /* SPR 11402 Fix  */
                                    /* SPR 16843 start */
                                    updateKpiStatsForTotalSduAckRcvdDlperQci(&dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex,ueIndex);
                                    /* SPR 16843 end */
                                    /* SPR 11402 Fix  */
#endif
                                    /* CA_phase2_harq_code Start */
                                    processDLAckForTBOne (globalTick,
                                            /* +- SPR 17777*/
                                            dlHarqProcess_p,
                                            /* +- SPR 17777*/
                                            dlUeContext_p,ueInternalCellIndex,
                                            /* SPR 12054 fix start */
                                            ackNackNode_p->schUsed, ueServCellIndex );
                                            /* SPR 12054 fix end */
                                    /*
                                     * Moved out from function
                                     * processDLAckForTBOne 
                                     * and added here
                                     */
                                    /*SPR 17561 FIX*/
#ifdef FDD_CONFIG
                                    /*SPR 16026 changes start*/ 
                                    if(dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag != INVALID_SCELL_ACT_DEACT_MSG)
                                    {
                                        ScellStartDlCATimer(dlUeContext_p,dlHarqProcess_p);
                                    }
                                    /*SPR 16026 changes end*/ 
#endif
                                    /*SPR 17561 FIX*/

                                    calculateDLBlerForAckTB1(globalTick, 
                                            ueULContext_p,dlCQIInfo_p
                                            ,ueIndex,
                                            internalCellIndex,
                                            /* CA_phase2_csi_code Start */
                                            ueServCellIndex,
                                            &bitMaskForAperiodicReport,
                                            dlMIMOInfo_p);
                                            /* CA_phase2_csi_code End */
                                    /* CA_phase2_harq_code End */
#ifdef KPI_STATS
                                    /*Update the pktDelay KPI counters for packets in 
                                     * DL which are positively acked based on HARQ
                                     * feedback*/

                                    /* SPR 12783 Changes Start */
                                    macUpdateKpiCntrsForPktDelayPerQci(&dlHarqProcess_p->dlHARQTBOneInfo, ueIndex, 1,
                                            internalCellIndex, ackNackNode_p->recvTTI);
                                    /* SPR 12783 Changes End */
#endif
                                    freeDLHarqProcessTBSpecific(
                                            dlUeContext_p,
                                            dlHarqProcess_p,
                                            TB_ONE,internalCellIndex);

                                }
                                else /* Other than ACK */
                                {
#ifdef ACK_NACK_COUNT_STATS
                                    lteCommonUpdateAckNackStats(statsTB1, ueIndex, DL_NACK);
#endif
                                    /* SPR 5402 Fix Start */
#ifdef PERF_STATS
                                    UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_NACK ,
                                            dlHarqProcess_p->dlHARQTBOneInfo.mcsIndex);
#endif
                                    /* SPR 5402 Fix End */

                                    /* 32.425 Events code changes start*/
#ifdef KPI_STATS
                                    if (dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB ==\
                                            (cellSpecificParams_g.cellConfigAndInitParams_p[ueInternalCellIndex]->\
                                             cellParams_p->maxHarqRetransmission - 1))
                                    {
                                        /* Reset pdcpAckBytes,T1 and T2 at harq Common Index  */
                                    /* SPR 16613 fix start */
                                    /* + SPR 17745 */
                                    lteMacUpdateKpiStatsIPThpDlOnHarqNack(dlHarqProcess_p,
                                            ueIndex,ackNackNode_p->recvTTI,internalCellIndex);
                                    /* - SPR 17745 */
                                    /* SPR 16613 fix end */
                                    }
#endif
                                    /* 32.425 Events code changes end*/

                                    /* SPS_CHG */
                                    /*! \code
                                     * -If (UE State == SPS_ACTIVATION_INITIATED or
                                     *    SPS_REACTIVATION_INITIATED ) && First NACK is Received,
                                     *     Push entry in dlSpsStatusQ for strategy.
                                     * \endcode
                                     * */
                                    /* SPR 18626  Fix Start */
                                    if(((SPS_ACTIVATION_INITIATED == spsDlInfo_p->spsState )||
                                                ( SPS_REACTIVATION_INITIATED == spsDlInfo_p->spsState))
                                            &&(!dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB) 
                                            && (FAPI_NACK == oprTb1)
                                            && (spsDlInfo_p->spsDlSetupInfo_p != PNULL) /*CID 88287 */
                                            && (dlHarqProcess_p->harqProcessId <
                                                spsDlInfo_p->spsDlSetupInfo_p->numberOfConfSpsProcesses)
#ifndef UTFWK_SIMULATION
/* SPR 19704 + */

                                            && ((dlHarqProcess_p->dlharqRttExpiryTick - globalTick) == 1)
/* SPR 19704 - */
#else
                                            && ((dlHarqProcess_p->dlharqRttExpiryTick - 
                                                    (globalTick + PHY_DL_DELAY) ) == 1)
#endif
                                            /* SPR 12054 fix start */
                                            && ( SPS_SCHEDULING == ackNackNode_p->schUsed ) 
                                            /* if primary cell */
                                            && ( 0 == ueServCellIndex )
                                            /* SPR 12054 fix end */
                                      )
                                        /* SPR 18626  Fix End */
                                    {
                                        GET_MEM_FROM_POOL(MacDlSpsStatus, macDlSpsStatus_p, 
                                                sizeof(MacDlSpsStatus), PNULL);
                                        /*coverity-530 CID-25227*/
                                        if(PNULL == macDlSpsStatus_p)
                                        {
                                            freeDLHarqProcessTBSpecific( dlUeContext_p, 
                                                    dlHarqProcess_p, 
                                                    TB_ONE,internalCellIndex);
                                            continue;
                                        }
                                        /*coverity-530 CID-25227*/
                                        macDlSpsStatus_p->ueIndex = dlUeContext_p->ueIndex;
                                        macDlSpsStatus_p->response = FIRST_ACK_NACK;
                                        /* Coverity 63538 Fix Start */
                                        if(!ENQUEUE_MAC_DL_SPS_Q(dlSpsStatusQ_gp[ueInternalCellIndex], 
                                                macDlSpsStatus_p))
                                        {
                                            freeMemPool(macDlSpsStatus_p);
                                            macDlSpsStatus_p = PNULL;
                                            freeDLHarqProcessTBSpecific( dlUeContext_p, 
                                                    dlHarqProcess_p, 
                                                    TB_ONE,internalCellIndex);
                                            continue;
                                        }
                                        /* Coverity 63538 Fix End */
                                        /* Update Q Stats */
                                        LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q, 
                                                QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[ueInternalCellIndex]),
                                                ueInternalCellIndex);
                                        if( LTE_TRUE == dlHarqProcess_p->isSpsOnlyPaddingPresent )
                                        {
                                            LTE_MAC_PER_UE_DEBUG_STATS(dlUeContext_p->ueIndex, 
                                                    globalTick, tempHarqID, oprTb1, 
                                                    dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB,
                                                    ueInternalCellIndex);
                                            /* Cyclomatic Complexity changes - starts here */
                                            calculateDLBlerForNackTB1(globalTick,
                                                    ueULContext_p,dlCQIInfo_p,
                                                    ueIndex,ueInternalCellIndex,
                                                    dlMIMOInfo_p,
                                                    /* CA_phase2_csi_code Start */
                                                    ueServCellIndex,
                                                    &bitMaskForAperiodicReport);
                                                    /* CA_phase2_csi_code End */

                                            /* Cyclomatic Complexity changes - ends here */
                                            /* Free HARQ occupied as the PDU need not be re-transmitted 
                                             * as padding is only sent */	
                                            freeDLHarqProcessTBSpecific( dlUeContext_p, 
                                                    dlHarqProcess_p, 
                                                    TB_ONE,internalCellIndex);
                                            continue;
                                        }
                                    }
                                    /*!
                                     * \code
                                     * -If (UE State == SPS_DEACTIVATION_INITIATED)
                                     *   -If NACK Received
                                     *     Push entry in dlSpsStausQ with cause as FIRST_ACK_NACK 
                                     *   -If Unsure case
                                     *      Push entry in dlSpsStatusQ with cause as MAX_SPS_HARQ_RETX
                                     *   Do not try for next retransmission, since SPS_DEACTIVATION is not
                                     *   associated with any TB.  
                                     * \endcode
                                     * */
                                    else if((SPS_DEACTIVATION_INITIATED == spsDlInfo_p->spsState)
                                            && ( dlHarqProcess_p->harqProcessId == spsDlInfo_p->hpId )
#ifndef UTFWK_SIMULATION
/* SPR 19704 + */
                                            && ((dlHarqProcess_p->dlharqRttExpiryTick - globalTick) == 1)
/* SPR 19704 - */
#else
                                            && ((dlHarqProcess_p->dlharqRttExpiryTick - 
                                                    (globalTick + PHY_DL_DELAY) ) == 1)
#endif
                                            /* SPR 12054 fix start */
                                            && ( SPS_SCHEDULING == ackNackNode_p->schUsed ) 
                                            /* if primary cell */
                                            && ( 0 == ueServCellIndex )
                                            /* SPR 12054 fix end */
                                           )
                                    { 
                                        GET_MEM_FROM_POOL(MacDlSpsStatus, 
                                                macDlSpsStatus_p, 
                                                sizeof(MacDlSpsStatus), PNULL);
                                        /*coverity-530 CID-25227*/
                                        if(PNULL == macDlSpsStatus_p)
                                        {
                                            freeDLHarqProcessTBSpecific( dlUeContext_p, 
                                                    dlHarqProcess_p, 
                                                    TB_ONE,internalCellIndex);
                                            continue;
                                        }
                                        /*coverity-530 CID-25227*/
                                        macDlSpsStatus_p->ueIndex = dlUeContext_p->ueIndex;
                                        /* SPR 19704 + */
                                        if(FAPI_NACK == oprTb1) 
                                        /* SPR 19704 - */
                                        {
                                            macDlSpsStatus_p->response = FIRST_ACK_NACK;
                                        }
                                        else
                                        {
                                            macDlSpsStatus_p->response = MAX_SPS_HARQ_RETX; 
                                        }
                                        /* Coverity 63538 Fix Start */
                                        if(!ENQUEUE_MAC_DL_SPS_Q(dlSpsStatusQ_gp[ueInternalCellIndex], 
                                                macDlSpsStatus_p))
                                        {
                                            freeMemPool(macDlSpsStatus_p);
                                            macDlSpsStatus_p = PNULL;
                                        }
                                        /* Coverity 63538 Fix End */
                                        /* Update Q Stats */
                                        LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q, 
                                                QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[ueInternalCellIndex]),
                                                ueInternalCellIndex);
                                        LTE_MAC_PER_UE_DEBUG_STATS(dlUeContext_p->ueIndex, 
                                                globalTick, tempHarqID, oprTb1, 
                                                dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB,
                                                ueInternalCellIndex);
                                        /* Cyclomatic Complexity changes - starts here */
                                        /* CA_phase2_harq_code Start */
                                        calculateDLBlerForNackTB1(globalTick,
                                                ueULContext_p,dlCQIInfo_p,
                                                ueIndex,ueInternalCellIndex,
                                                dlMIMOInfo_p,
                                                 /* CA_phase2_csi_code Start */
                                                ueServCellIndex,
                                                &bitMaskForAperiodicReport);
                                                 /* CA_phase2_csi_code End */
                                        /* CA_phase2_harq_code End */
                                        /* Cyclomatic Complexity changes - ends here */
                                        /* Free HARQ occupied as the PDU need not be re-transmitted 
                                         * since in SPS_DEACTIVATION_INITATED state */	
                                        freeDLHarqProcessTBSpecific( dlUeContext_p, 
                                                dlHarqProcess_p, 
                                                TB_ONE,internalCellIndex);
                                        continue; 
                                    }
                                    /* SPS_CHG */   
                                    /* + CQI_5.0 */
                                    /* BLER calculation for TB1 */
                                    /* Cyclomatic Complexity changes - starts here */
                                    /* CA_phase2_harq_code Start */
                                    calculateDLBlerForNackTB1(globalTick,
                                            ueULContext_p,dlCQIInfo_p,
                                            ueIndex,ueInternalCellIndex,
                                            dlMIMOInfo_p,
                                            /* CA_phase2_csi_code Start */
                                            ueServCellIndex,
                                            &bitMaskForAperiodicReport);
                                            /* CA_phase2_csi_code End */
                                    /* CA_phase2_harq_code End */
                                    /* Cyclomatic Complexity changes - ends here */
                                    /* - CQI_5.0 */

                                    /*255 UE Attach DL UL ON Fix Start*/
                                    if(SPS_SCHEDULING == ackNackNode_p->schUsed && TRUE == cellSpecificParams_g.\
                                            cellConfigAndInitParams_p[ueInternalCellIndex]->initParams_p->spsSysWideParam.enableSps )  
                                        /*255 UE Attach DL UL ON Fix End*/
                                    {
                                        maxRetxCount = dlUeContext_p->spsDlInfo.spsDlSetupInfo_p->maxSpsHarqRetx;  
                                    } 
                                    else
                                    {
                                        maxRetxCount = cellSpecificParams_g.cellConfigAndInitParams_p[ueInternalCellIndex]->\
                                                       cellParams_p->maxHarqRetransmission -1 ;
                                    }
                                    /* Cyclomatic Complexity changes - starts here */
                                    /* +- SPR 17777*/
                                    processDLNackForTBOne (ueIndex, maxRetxCount,
                                            dlHarqProcess_p, dlUeContext_p
                                            /* CA_phase2_harq_code Start */
                                            ,&nackFlag[ueServCellIndex] 
                                            ,ueInternalCellIndex);
                                    /* CA_phase2_harq_code End*/
                                    /* Cyclomatic Complexity changes - ends here */

                                }
                            }
                            else if (!dlHarqProcess_p->isTBOneValid && dlHarqProcess_p->isTBTwoValid)
                            {
                                /* SPR 21997 Fix Start */
#if defined(FAPI_RELEASE9) || defined(FAPI_RELEASE10) || defined(FAPI_RELEASE8)
                                /* In release 9 or 10 in case of only one TB either TB1/TB2 feedback present it will
                                 * be stored in the lowest nibble */
                                oprTb2 = (dlHarqFlag[ueServCellIndex] & 0x07);
#else
                                oprTb2 = (dlHarqFlag[ueServCellIndex] & 0x70) >> 4; 
#endif
                                /* SPR 21997 Fix End */
                                /* Check if the HARQ information is received on PUCCH */
                                /* Cyclomatic Complexity changes - starts here */


                                /* CA_phase2_harq_code Start */
                                if(TRUE == isHarqOnPucch)
                                {
                                    /* + SPR_13763 */ 
                                    updateAndCheckHarqBlerForPucch(dlUeContext_p,
                                            oprTb2, globalTick,ueInternalCellIndex);
                                    /* - SPR_13763 */ 
                                }
                                /* CA_phase2_harq_code End */


                                /* Cyclomatic Complexity changes - ends here */

                                if (oprTb2 == ACK) // ACK on TB Two
                                {
#ifdef ACK_NACK_COUNT_STATS
                                    lteCommonUpdateAckNackStats(statsTB2, ueIndex, DL_ACK);
#endif

#ifdef PERF_STATS
                                    UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_ACK ,
                                            dlHarqProcess_p->dlHARQTBTwoInfo.mcsIndex);
                                    dlBytesSent += dlHarqProcess_p->dlHARQTBTwoInfo.tbSize ;
#endif
                                    /* 32.425 Events code changes start*/
#ifdef KPI_STATS
                                    /* This will update the Shared Memory Time and Shared Memory Data */ 
                                    /* +- SPR 17777*/
                                    lteMacUpdateKpiStatsIPThpDlOnHarqAck(ueIndex,
                                            dlHarqProcess_p,
                                            ackNackNode_p->recvTTI
#ifdef TDD_CONFIG
                                            , internalCellIndex
#endif
                                            );
                                    /* +- SPR 17777*/
                                    /* SPR 11402 Fix  */
                                    /* SPR 16843 start */
                                    updateKpiStatsForTotalSduAckRcvdDlperQci(&dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex,ueIndex);
                                    /* SPR 16843 end */
                                    /* SPR 11402 Fix  */
#endif
                                    /* 32.425 Events code changes end*/

                                    /* + CQI_5.0 */
                                    /*SPR 17561 FIX*/
#ifdef FDD_CONFIG
                                    /*SPR 16026 changes start*/ 
                                    if(dlHarqProcess_p->dlHARQTBTwoInfo.macScellCEFlag != INVALID_SCELL_ACT_DEACT_MSG)
                                    {
                                        ScellStartDlCATimer(dlUeContext_p,dlHarqProcess_p);
                                    }
                                    /*SPR 16026 changes end*/ 
#endif
                                    /*SPR 17561 FIX*/
                                    /* BLER calculation for TB2 */
                                    /* Cyclomatic Complexity changes - starts here */
                                    /* CA_phase2_harq_code Start */
                                    calculateDLBlerForAckTB2(globalTick, \
                                            ueULContext_p,
                                            dlCQIInfo_p,
                                            /* +- SPR 17777*/
                                            ueIndex, 
                                            internalCellIndex,
                                            /* CA_phase2_csi_code Start */
                                            ueServCellIndex,
                                            &bitMaskForAperiodicReport);
                                            /* CA_phase2_csi_code End */
                                    /* CA_phase2_harq_code End */
                                    /* Cyclomatic Complexity changes - ends here */
                                    /* - CQI_5.0 */

#ifdef KPI_STATS
                                    /*Update the pktDelay KPI counters for packets in 
                                     * DL which are positively acked based on HARQ
                                     * feedback*/
                                    /* SPR 12783 Changes Start */
                                    macUpdateKpiCntrsForPktDelayPerQci(&dlHarqProcess_p->dlHARQTBTwoInfo, ueIndex, 2,
                                            internalCellIndex, ackNackNode_p->recvTTI);
                                    /* SPR 12783 Changes End */
#endif
                                    freeDLHarqProcessTBSpecific(
                                            dlUeContext_p,
                                            dlHarqProcess_p,
                                            TB_TWO,internalCellIndex);
                                }
                                else
                                {
#ifdef ACK_NACK_COUNT_STATS
                                    lteCommonUpdateAckNackStats(statsTB2, ueIndex, DL_NACK);
#endif

#ifdef PERF_STATS
                                    UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_NACK ,
                                            dlHarqProcess_p->dlHARQTBTwoInfo.mcsIndex);
#endif
                                    /* 32.425 Events code changes start*/
#ifdef KPI_STATS
                                    if (dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB ==\
                                            (cellSpecificParams_g.cellConfigAndInitParams_p[ueInternalCellIndex]->\
                                             cellParams_p->maxHarqRetransmission - 1))
                                    {

                                        /* Reset pdcpAckBytes,T1 and T2 at harq Common Index  */
                                    /* SPR 16613 fix start */
                                        /* + SPR 17745 */
                                    lteMacUpdateKpiStatsIPThpDlOnHarqNack(dlHarqProcess_p,
                                            ueIndex,ackNackNode_p->recvTTI,internalCellIndex);
                                        /* - SPR 17745 */
                                    /* SPR 16613 fix end */
                                    }
#endif
                                    /* 32.425 Events code changes end*/

                                    /* + CQI_5.0 */
                                    /* + CQI_5.0 */
                                    /* TDD SB_CQI */
                                    /* BLER calculation for TB2 */
                                    /* Cyclomatic Complexity changes - starts here */
                                    /* CA_phase2_harq_code Start */
                                    calculateDLBlerForNackTB2(globalTick, \
                                            ueULContext_p,dlCQIInfo_p,
                                            /* +- SPR 17777*/
                                            ueIndex,ueInternalCellIndex,
                                            /* CA_phase2_csi_code Start */
                                            ueServCellIndex,
                                            &bitMaskForAperiodicReport);
                                   /* CA_phase2_csi_code End */
                                    /* CA_phase2_harq_code End */
                                    /* Cyclomatic Complexity changes - ends here */
                                    /* TDD SB_CQI */
                                    /* - CQI_5.0 */

                                    /* Cyclomatic Complexity changes - starts here */
                                    /* +- SPR 17777*/
                                    processDLNackForTBTwo (ueIndex,
                                            dlHarqProcess_p, dlUeContext_p
                                            /* CA_phase2_harq_code Start */
                                            ,&nackFlag[ueServCellIndex] 
                                            ,ueInternalCellIndex);
                                    /* CA_phase2_harq_code End*/
                                    /* Cyclomatic Complexity changes - ends here */

                                }
                            }
                            else
                            {
                                LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ, \
                                        "\n BOTH THE TBs ARE INVALID OF THIS NODE");                            
                                    /* CA_phase2_harq_code Start */
                                /*
                                 * Node is freeing 
                                 * after for loop
                                 * if(!dlUeContextInfo_p->pendingDeleteFlag)
                                 */
                                /*
                                 * Next serving cell of UE will 
                                 * be processed
                                 */
                                continue;
                                    /* CA_phase2_harq_code End*/
                            }
                            /* +COVERITY 25152 */
#ifdef PERF_STATS

                            /*EVENT : EVENT_PARAM_RADIOTHP_RES_DL, The total amount of physical resources used for 
                             * transmission in the downlink. Both successful and unsuccessful transmissions are counted. 
                             * Unit: ResourceElements */
                            gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats.\
                                totalDLREsUsage += dlHarqProcess_p->totalREsUsed;
#endif
                            /* -COVERITY 25152 */

                            /* CA_phase2_harq_code Start */
                            if(TRUE == nackFlag[ueServCellIndex])
                            {
                                retxNode.isNodeInvalid = FALSE;
                            }
                            /* CA_phase2_harq_code End */
                        } /* End of if ( INVALID HARQ ID ) */
                        else
                        {
                            LTE_MAC_UT_LOG(LOG_WARNING,Log_DL_HARQ,
                                    "Else of dlharqTimerNode_p->isValid UE Index [%d] Harq ID [%d]\n", ueIndex, tempHarqID);
                        }
                        /* CA_phase2_harq_code Start */
                    }  // for_loop 
                    /* commented this part for making compilation
                     * through  as it is harq code */
                    if((FALSE == retxNode.isNodeInvalid) && (MAC_FAILURE ==
                                macPushHarqNackInfoToRetxQueue(dlUeContext_p ,retxNode ,
                                    nackFlag,internalCellIndex) ))
                    {
                        macHandleRetxQueueFailure(ueIndex,nackFlag,retxNode);
                    }
                    nackFlag[0] = FALSE;
                    nackFlag[1] = FALSE;
                    retxNode.isNodeInvalid = TRUE;

                    /* CA_phase2_harq_code End */
                    /* CA_phase2_csi_code Start */
                    /* As part of CA FDD changes,15.5.9 b)of Arch doc, single entry
                     * of ulGrantAperiodicCqiQueue_gp(of PCell)if for CA UEs
                     * condition to trigger Aperiodic CSI occurs for single or both
                     * cells and update node variable aperiodicCQIToBeRequested */
                    if(bitMaskForAperiodicReport)
                    {
                        putEntryInULAperiodicCQIQueue(ueIndex, internalCellIndex,
                                bitMaskForAperiodicReport);
                    }
                    /* CA_phase2_csi_code End */

                } /* End of IF(pending delete flag */    
                /*
                   We must free this node now as the pendingDeleteFlag has been set
                   */
                else
                {
                    LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ,
                            "Else of !Pending Delete flag \n");
                }     
#ifndef DL_UL_SPLIT
                freeMemPool(ackNackNode_p);
#else
                FREE_MEM_NODE_ACK_NACK_CIRC_Q(ackNackNode_p);
#endif
            } /* End of IF(ackNackNode_p....) */
            else
            { 
                LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ, \
                        "\n The ACK NACK NODE IS NULL\n");
            }
#ifndef DL_UL_SPLIT
            ackNackQueueCount--;    
#endif
#ifdef PERF_STATS
            /* Update the Counters for the total successfully transferred data volume on MAC level in the downlink
             *  * This Includes Transmission and Retransmission Bytes */
            gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats.\
                totalDlBytesSent += dlBytesSent;
#endif
    } /* End of WHILE */
    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}

/* Cyclomatic Complexity changes - starts here */
/*****************************************************************************
 * Function Name  : isHarqOnPucchRecvdOnTBOneTBTwo 
 * Inputs         : ueIndex,
 *                  isHarqOnPucch,
 *                  oprTb1,
 *                  oprTb2,
 *                  dlUeContext_p,
 *                  globalTick
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function checks whether the HARQ on PUCCH is received  
 *                  when both TB1 and TB2 are valid.
 *****************************************************************************/
/* +- SPR 17777*/
STATIC  void isHarqOnPucchRecvdOnTBOneTBTwo (
        UInt8       isHarqOnPucch_p, 
        UInt8       oprTb1, 
        UInt8       oprTb2, 
        DLUEContext *dlUeContext_p, 
        /* SPR 15909 fix start */
        tickType_t      globalTick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)
{
    UInt8 oprTb = 0;

    if (isHarqOnPucch_p)
    {
        oprTb = oprTb1;
        // Use any one of the oprTBx only when both the TBs has 
        // Sure case (i.e. either ACK or NACK)
        // else make unsure case
        if (oprTb1 >= 3 || oprTb2 >= 3)
        {
            // Make it Unsure (greater than 2 i.e. other Than ACK or NACK)
            oprTb = 3;   
        }
        /*SPR_4001_CHANGE_START*/
        /* Cyclomatic Complexity changes - starts here */
        updateAndCheckHarqBlerForPucch(dlUeContext_p, oprTb, globalTick
                , internalCellIndex);
        /* Cyclomatic Complexity changes - ends here */
        /*SPR_4001_CHANGE_END*/
    }
    /* SPR 5033 changes end */
}
/*coverity 62977*/
/*****************************************************************************
 * Function Name  : isHarqOnPucchRecvdOnTBTwo 
 * Inputs         : ueIndex,
 *                  subFrameNumber,
 *                  isHarqOnPucch,
 *                  oprTb2,
 *                  dlUeContext_p,
 *                  globalTick
 * Outputs        : None
 * Returns        : None
 * Description    : This function checks whether the HARQ on PUCCH is received  
 *                  when both TB1 and TB2 are valid.
 *****************************************************************************/
/*static  void isHarqOnPucchRecvdOnTBTwo (UInt16 ueIndex, 
        UInt32      subFrameNumber, 
        UInt8       isHarqOnPucch, 
        UInt8       oprTb2, 
        DLUEContext *dlUeContext_p,
        tickType_t      globalTick,
        InternalCellIndex internalCellIndex)
{*/
    /* SPR 5033 changes start*/
    /* Update isHarqOnPucch to indicate whether the HARQ is 
       received on PUCCH or not */
   /* CHECK_IS_HARQ_BLER_RECEIVED_ON_PUCCH(ueIndex, subFrameNumber, isHarqOnPucch);
    if (isHarqOnPucch)
    {*/
        /*SPR_4001_CHANGE_START*/
        /* Cyclomatic Complexity changes - starts here */
       /* updateAndCheckHarqBlerForPucch(dlUeContext_p, oprTb2, globalTick
                ,internalCellIndex);*/
        /* Cyclomatic Complexity changes - ends here */
        /*SPR_4001_CHANGE_END*/
    /*}*/
    /* SPR 5033 changes end*/
/*}*/

/*****************************************************************************
 * Function Name  : processDLAckForTBOne 
 * Inputs         : globalTick,
 *                  dlHarqProcess_p,
 *                  ueULContext_p,
 *                  dlUeContext_p, internalCellIndex,
 *                  schUsed - if ack processed is for SPS or DRA
 *                  ueServCellIndex - primary cell/secondary cell for which ack
 *                         needs to be processed. Assume following values:-
 *                         0 - Primary cell
 *                         1 - secondary cell
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the DL HARQ ACK for TB1 case  .
 *****************************************************************************/
        /* SPR 15909 fix start */                  
/* + SPR 17439 */
STATIC  void processDLAckForTBOne (tickType_t globalTick, 
/* - SPR 17439 */        
        /* SPR 15909 fix end */
        DLHARQProcess *dlHarqProcess_p, 
        /* +- SPR 17777*/
        DLUEContext   *dlUeContext_p,
        InternalCellIndex internalCellIndex,
        /* SPR 12054 fix start */
        UInt32 schUsed,
        UInt8 ueServCellIndex )
        /* SPR 12054 fix end */
{
    LP_SpsDlInfo spsDlInfo_p = &dlUeContext_p->spsDlInfo;
    LP_MacDlSpsStatus macDlSpsStatus_p = PNULL;
    /* SPS_CHG */
    /*!
     * \code
     *  If (UE State ==  SPS_ACTIVATION_INITIATED 
     *       or SPS_DEACTIVATION_INITIATED)
     *    and First ACK is Received, 
     *    Push entry in dlSpsStatusQ for strategy.
     * \endcode
     * */
    if(((SPS_ACTIVATION_INITIATED == spsDlInfo_p->spsState )||
                (SPS_REACTIVATION_INITIATED == spsDlInfo_p->spsState ))
            &&(!dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB)
            &&( (PNULL != spsDlInfo_p->spsDlSetupInfo_p) && 
                (dlHarqProcess_p->harqProcessId <
                 spsDlInfo_p->spsDlSetupInfo_p->numberOfConfSpsProcesses) )
#ifndef UTFWK_SIMULATION
/* SPR 19704 + */
           && ((dlHarqProcess_p->dlharqRttExpiryTick - globalTick) == 1)
/* SPR 19704 - */
#else
            && ((dlHarqProcess_p->dlharqRttExpiryTick -\
                    (globalTick + PHY_DL_DELAY) ) == 1) 
#endif
            /* SPR 12054 fix start */
            && ( SPS_SCHEDULING == schUsed ) 
            /* if primary cell */
            && ( 0 == ueServCellIndex )
            /* SPR 12054 fix end */
      )
    {
        GET_MEM_FROM_POOL(MacDlSpsStatus, macDlSpsStatus_p, 
                sizeof(MacDlSpsStatus), PNULL);
        /*+COVERITY 5.3.0 - 32724*/
        if(PNULL == macDlSpsStatus_p)
        {
	       	ltePanic("Memory Allocation failed in %s\n",__func__);
            /*KLOCKWORK_CHANGES_START_12*/
            return;
            /*KLOCKWORK_CHANGES_STOP_12*/
        }
        /*+COVERITY 5.3.0 - 32724*/
        macDlSpsStatus_p->ueIndex = dlUeContext_p->ueIndex;
        macDlSpsStatus_p->response = FIRST_ACK_NACK;
        /* Coverity 63538 Fix Start */
        if(!ENQUEUE_MAC_DL_SPS_Q(dlSpsStatusQ_gp[internalCellIndex], 
                macDlSpsStatus_p))
        {
            freeMemPool(macDlSpsStatus_p);
        }
        /* Coverity 63538 Fix End */
        /* Update Q Stats */
        /* CA Stats Changes Start */
        LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q, 
                QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex] ),
                internalCellIndex);
        /* CA Stats Changes End */
    }
    else if( (SPS_DEACTIVATION_INITIATED == spsDlInfo_p->spsState) 
            && ( dlHarqProcess_p->harqProcessId == spsDlInfo_p->hpId )   
#ifndef UTFWK_SIMULATION
/* SPR 19704 + */
            && ((dlHarqProcess_p->dlharqRttExpiryTick - globalTick) == 1) 
/* SPR 19704 - */
#else
            && ((dlHarqProcess_p->dlharqRttExpiryTick -\
                    (globalTick + PHY_DL_DELAY) ) == 1) 
#endif
            /* SPR 12054 fix start */
            && ( SPS_SCHEDULING == schUsed ) 
            /* if primary cell */
            && ( 0 == ueServCellIndex )
            /* SPR 12054 fix end */
           )
    {
        GET_MEM_FROM_POOL(MacDlSpsStatus, macDlSpsStatus_p, 
                sizeof(MacDlSpsStatus), PNULL);
        /*+COVERITY 5.3.0 - 32724*/
        if(PNULL == macDlSpsStatus_p)
        {
	       	ltePanic("Memory Allocation failed in %s\n",__func__);
            /*KLOCKWORK_CHANGES_START_12*/
            return;
            /*KLOCKWORK_CHANGES_STOP_12*/
        }
        /*+COVERITY 5.3.0 - 32724*/
        macDlSpsStatus_p->ueIndex = dlUeContext_p->ueIndex;
        macDlSpsStatus_p->response = FIRST_ACK_NACK;
        /* Coverity 63538 Fix Start */
        if(!ENQUEUE_MAC_DL_SPS_Q(dlSpsStatusQ_gp[internalCellIndex], 
                macDlSpsStatus_p))
        {
            freeMemPool(macDlSpsStatus_p);
        }
        /* Coverity 63538 Fix End */
        /* Update Q Stats */
        /* CA Stats Changes Start */
        LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q, 
                QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex] ),
                internalCellIndex);
        /* CA Stats Changes End */
    }
    /* SPS_CHG */
    /* + CQI_5.0 */
    /* BLER calculation for TB1 */
    /* Cyclomatic Complexity changes - starts here */
  /*  CA_phase2_harq_code start */
/* CA_phase2_harq_code End */
  /*  CA_phase2_harq_code end */
    /* Cyclomatic Complexity changes - ends here */
    /* - CQI_5.0 */

    if ((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag
                == TA_MSG) ||
            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag
             == TA_DRX_MSG))
    {
        dlUeContext_p->lastTASentTick =
            getCurrentTick()- PHY_UL_DATA_DELAY - FDD_HARQ_OFFSET;
    }
}

/*****************************************************************************
 * Function Name  : processDLNackForTBOne 
 * Inputs         : ueIndex,
 *                  maxRetxCount,
 *                  tempHarqID,
 *                  dlHarqProcess_p, 
 *                  dlUeContext_p,
 *                  triggerContainerIndex,
 *                  nackFlag_p,
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the DL HARQ NACK for TB1 case.
 *****************************************************************************/
static  void processDLNackForTBOne (UInt16 ueIndex, 
        UInt8         maxRetxCount,
        /* +- SPR 17777*/
        DLHARQProcess *dlHarqProcess_p, 
        DLUEContext   *dlUeContext_p
    /* CA_phase2_harq_code Start */
        ,UInt8         *nackFlag_p
    /* CA_phase2_harq_code End */
        , InternalCellIndex internalCellIndex
        )
{
    if( dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB
            < maxRetxCount )
        /* SPR 2249 Fixes Ends */
    {
    /* CA_phase2_harq_code Start */
       /* 
        * For CA UE both retrans cell info shanll 
        * be enqueued into retrans Queue of primary
        * cell
        */

     *nackFlag_p = TRUE;
    /* CA_phase2_harq_code End */
#ifdef FDD_CONFIG
        /*SPR 16026 changes start*/
        if(dlHarqProcess_p->scellActBitmask || dlHarqProcess_p->scellDeactBitmask)
        {
            scellHarqFailureForMaxRetx(dlHarqProcess_p,dlUeContext_p,FALSE);
        }
        /*SPR 16026 changes end*/
#endif

    }
    else
    {
#ifdef PERF_STATS
        gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats.\
            totalHarqFail++;
#endif
        /*DL_HARQ_OPT*/
        if (dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p)
            /*DL_HARQ_OPT*/
        { 
            /* SPR 11402 fix */

            sendARQIndicationToRLC(ueIndex, 
                    &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex);
            /* SPR 11402 fix */
        }
#ifdef FDD_CONFIG
        /*SPR 16026 changes start*/
        if(dlHarqProcess_p->scellActBitmask || dlHarqProcess_p->scellDeactBitmask)
        {
            scellHarqFailureForMaxRetx(dlHarqProcess_p,dlUeContext_p,TRUE);
        }
        /*SPR 16026 changes end*/
#endif
        /* SPR 16054 Fix Start */
        freeDLHarqProcessTBSpecific( dlUeContext_p, dlHarqProcess_p, TB_ONE, internalCellIndex);
        /* SPR 16054 Fix End */
    }
}

/*****************************************************************************
 * Function Name  : processDLNackForTBTwo 
 * Inputs         : ueIndex,
 *                  dlHarqProcess_p, 
 *                  dlUeContext_p, 
 *                  triggerContainerIndex,
 *                  nackFlag_p,
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the DL HARQ NACK for TB2 case.
 *****************************************************************************/
static  void processDLNackForTBTwo (UInt16 ueIndex, 
        /* +- SPR 17777*/
        DLHARQProcess *dlHarqProcess_p, 
        DLUEContext   *dlUeContext_p
    /* CA_phase2_harq_code Start */
        ,UInt8         *nackFlag_p
    /* CA_phase2_harq_code End */
        ,InternalCellIndex internalCellIndex
        )
{
			/*SPR 17561 FIX*/
			/*SPR 17561 FIX*/

    if (dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB < \
            (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
             cellParams_p->maxHarqRetransmission - 1))     
        /* SPR 2249 Fixes Ends */
    {
        /*
           Here the TB one is yet to complete the max re-trans.
           */

        /* 
         * For CA UE both retrans cell info shanll 
         * be enqueued into retrans Queue of primary
         * cell
         */
        *nackFlag_p = TRUE;
#ifdef FDD_CONFIG
        /*SPR 16026 changes start*/
        if(dlHarqProcess_p->scellActBitmask || dlHarqProcess_p->scellDeactBitmask)
        {
            scellHarqFailureForMaxRetx(dlHarqProcess_p,dlUeContext_p,FALSE);
        }
        /*SPR 16026 changes end*/
#endif
    }
    else
    {
#ifdef PERF_STATS
        gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats.\
            totalHarqFail++;
#endif
        /*DL_HARQ_OPT*/
        if (dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p)
            /*DL_HARQ_OPT*/
        { 
            /* SPR 11402 fix */
            sendARQIndicationToRLC(ueIndex, 
                    &dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex);
            /* SPR 11402 fix */

        }
#ifdef FDD_CONFIG
        /*SPR 16026 changes start*/
        if(dlHarqProcess_p->scellActBitmask || dlHarqProcess_p->scellDeactBitmask)
        {
            scellHarqFailureForMaxRetx(dlHarqProcess_p,dlUeContext_p,TRUE);
        }
        /*SPR 16026 changes end*/
#endif
        /* SPR 16054 Fix Start */
        freeDLHarqProcessTBSpecific(dlUeContext_p, dlHarqProcess_p, TB_TWO, internalCellIndex);
        /* SPR 16054 Fix End */
    }
}

/*****************************************************************************
 * Function Name  : processDLAckForTBOneNackForTBTwo 
 * Inputs         : ueIndex,
 *                  tempHarqID,
 *                  dlHarqProcess_p, 
 *                  dlUeContext_p,
 *                  triggerContainerIndex,
 *                  nackFlag_p,
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the DL HARQ ACK for TB1 and NACK for
 *                  TB2 case.
 *****************************************************************************/
static  void processDLAckForTBOneNackForTBTwo (UInt16 ueIndex, 
        /* +- SPR 17777*/
        DLHARQProcess *dlHarqProcess_p, 
        DLUEContext   *dlUeContext_p
    /* CA_phase2_harq_code Start */
        ,UInt8         *nackFlag_p
    /* CA_phase2_harq_code End */
        ,InternalCellIndex internalCellIndex
        )
{

    /* SPR 3679 Changes Start */
    if (dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB == \
            (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
             cellParams_p->maxHarqRetransmission - 1))     
    {
#ifdef PERF_STATS
        gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats.\
            totalHarqFail++;
#endif
#ifdef FDD_CONFIG
        /*SPR 16026 changes start*/
        if(dlHarqProcess_p->scellActBitmask || dlHarqProcess_p->scellDeactBitmask)
        {
            scellHarqFailureForMaxRetx(dlHarqProcess_p,dlUeContext_p,TRUE);
        }
        /*SPR 16026 changes end*/
#endif
        /*DL_HARQ_OPT*/
        /* _RLC_AM_MODE_ */
        if (dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p)
            /*DL_HARQ_OPT*/
        {									                                        
            /* SPR 11402 fix */
            sendARQIndicationToRLC(ueIndex, 
                    &dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex);
            /* SPR 11402 fix */
        }
        /* _RLC_AM_MODE_ */

        freeDLHarqProcess(dlUeContext_p,
                dlHarqProcess_p,
                internalCellIndex);
    }
    else
    {
    /* CA_phase2_harq_code Start */
        *nackFlag_p = TRUE;

       /* 
        * For CA UE both retrans cell info shanll 
        * be enqueued into retrans Queue of primary
        * cell
        */
#ifdef FDD_CONFIG
        /*SPR 16026 changes start*/
        if(dlHarqProcess_p->scellActBitmask || dlHarqProcess_p->scellDeactBitmask)
        {
            scellHarqFailureForMaxRetx(dlHarqProcess_p,dlUeContext_p,FALSE);
        }
        /*SPR 16026 changes end*/
#endif
        freeDLHarqTB(dlUeContext_p, dlHarqProcess_p,
                TB_ONE, internalCellIndex);
    }
}

/*****************************************************************************
 * Function Name  : processDLAckForTBTwoNackForTBOne 
 * Inputs         : ueIndex,
 *                  tempHarqID,
 *                  dlHarqProcess_p, 
 *                  dlUeContext_p,
 *                  triggerContainerIndex,
 *                  nackFlag_p
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the DL HARQ ACK for TB2 and NACK for
 *                  TB1 case.
 *****************************************************************************/
static  void processDLAckForTBTwoNackForTBOne (UInt16 ueIndex, 
        /* +- SPR 17777*/
        DLHARQProcess *dlHarqProcess_p, 
        DLUEContext   *dlUeContext_p
    /* CA_phase2_harq_code Start */
        ,UInt8         *nackFlag_p
    /* CA_phase2_harq_code End */
        ,InternalCellIndex internalCellIndex
        )
{
			/*SPR 17561 FIX*/
			/*SPR 17561 FIX*/

    if (dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB == \
            (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
             cellParams_p->maxHarqRetransmission - 1))     
    {

#ifdef PERF_STATS
        gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats.\
            totalHarqFail++;
#endif

        /* _RLC_AM_MODE_ */
        /*DL_HARQ_OPT*/
        if (dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p)
            /*DL_HARQ_OPT*/
        { 

            /* SPR 11402 fix */
            sendARQIndicationToRLC(
                    ueIndex, 
                    &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex);
            /* SPR 11402 fix */
        }
        /* _RLC_AM_MODE_ */
#ifdef FDD_CONFIG
			/*SPR 16026 changes start*/
			if(dlHarqProcess_p->scellActBitmask || dlHarqProcess_p->scellDeactBitmask)
			{
				scellHarqFailureForMaxRetx(dlHarqProcess_p,dlUeContext_p,TRUE);
			}
			/*SPR 16026 changes end*/
#endif
        freeDLHarqProcess(dlUeContext_p,
                dlHarqProcess_p,
                internalCellIndex);
    }
    else
    {
    /* CA_phase2_harq_code Start */
    
       *nackFlag_p = TRUE;

       /* 
        * For CA UE both retrans cell info shanll 
        * be enqueued into retrans Queue of primary
        * cell
        */
#ifdef FDD_CONFIG
			/*SPR 16026 changes start*/
			if(dlHarqProcess_p->scellActBitmask || dlHarqProcess_p->scellDeactBitmask)
			{
				scellHarqFailureForMaxRetx(dlHarqProcess_p,dlUeContext_p,FALSE);
			}
			/*SPR 16026 changes end*/
#endif

        freeDLHarqTB(dlUeContext_p, dlHarqProcess_p,
                TB_TWO, internalCellIndex);
    }
}

/*****************************************************************************
 * Function Name  : processDLNackForTBOneTBTwo 
 * Inputs         : ueIndex,
 *                  tempHarqID,
 *                  dlHarqProcess_p, 
 *                  dlUeContext_p,
 *                  triggerContainerIndex,
 *                  nackFlag_p,
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function process the DL HARQ NACK for both TB1 and 
 *                  TB2 case.
 *****************************************************************************/
static  void processDLNackForTBOneTBTwo (UInt16 ueIndex, 
        /* +- SPR 17777*/
        DLHARQProcess *dlHarqProcess_p, 
        DLUEContext   *dlUeContext_p
        /* + SPR 17745 */
#ifdef KPI_STATS
        ,UInt32 recvTTI
#endif
        /* - SPR 17745 */
    /* CA_phase2_harq_code Start */
        ,UInt8         *nackFlag_p
    /* CA_phase2_harq_code End */
        ,InternalCellIndex internalCellIndex
        )
{
    /* SPR 2249 Fixes Begins */
    if (dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB != \
            (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
             cellParams_p->maxHarqRetransmission - 1)
            || dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB != \
            (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
             cellParams_p->maxHarqRetransmission - 1))     
    {
        if (dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB ==\
                (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                 cellParams_p->maxHarqRetransmission - 1))
        {
#ifdef PERF_STATS
            gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats.\
                totalHarqFail++;
#endif
#ifdef FDD_CONFIG
            /*SPR 16026 changes start*/
            if(dlHarqProcess_p->scellActBitmask || dlHarqProcess_p->scellDeactBitmask)
            {
                scellHarqFailureForMaxRetx(dlHarqProcess_p,dlUeContext_p,TRUE);
            }
            /*SPR 16026 changes end*/
#endif
            /* 32.425 Events code changes start*/
#ifdef KPI_STATS
            /* Reset pdcpAckBytes,T1 and T2 at harq Common Index  */
            /* + SPR 17745 */
            lteMacUpdateKpiStatsIPThpDlOnHarqNack(dlHarqProcess_p, ueIndex,
                    recvTTI,internalCellIndex );
            /* - SPR 17745 */
#endif
            /* 32.425 Events code changes end*/
            /*
               We send the ARQ indication to RLC twice, one for each
               TB and then we free the HARQ Process completely.
               */

            /* _RLC_AM_MODE_ */
            /*DL_HARQ_OPT*/
            if(dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p)
                /*DL_HARQ_OPT*/
            { 

                /* SPR 11402 fix */
                sendARQIndicationToRLC(
                        ueIndex, 
                        &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex);
                /* SPR 11402 fix */
            }
            freeDLHarqTB(dlUeContext_p, dlHarqProcess_p, TB_ONE, internalCellIndex);
            /* _RLC_AM_MODE_ */
        }
        else if(dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB ==\
                (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                 cellParams_p->maxHarqRetransmission - 1))
        {
#ifdef PERF_STATS
            gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats.\
                totalHarqFail++;
#endif
#ifdef FDD_CONFIG
            /*SPR 16026 changes start*/
            if(dlHarqProcess_p->scellActBitmask || dlHarqProcess_p->scellDeactBitmask)
            {
                scellHarqFailureForMaxRetx(dlHarqProcess_p,dlUeContext_p,TRUE);
            }
            /*SPR 16026 changes end*/
#endif

            /* 32.425 Events code changes start*/
#ifdef KPI_STATS
            /* Reset pdcpAckBytes,T1 and T2 at harq Common Index  */
            /* + SPR 17745 */
            lteMacUpdateKpiStatsIPThpDlOnHarqNack(dlHarqProcess_p ,ueIndex,
                    recvTTI ,internalCellIndex);
            /* - SPR 17745 */

#endif
            /* 32.425 Events code changes end*/

            /*
               We send the ARQ indication to RLC twice, one for each
               TB and then we free the HARQ Process completely.
               */

            /* _RLC_AM_MODE_ */
            if (dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p)
                /*DL_HARQ_OPT*/
            {									                                        
                /* SPR 11402 fix */
                sendARQIndicationToRLC(
                        ueIndex, 
                        &dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex);
                /* SPR 11402 fix */
            }
            /*SPR 17561 FIX*/
            freeDLHarqTB(dlUeContext_p, dlHarqProcess_p,
                    TB_TWO, internalCellIndex);
        }
        /* CA_phase2_harq_code Start */
        *nackFlag_p = TRUE;
        /* 
         * For CA UE both retrans cell info shanll 
         * be enqueued into retrans Queue of primary
         * cell
         */
        /* CA_phase2_harq_code End */
    }
    else    /* SPR 2249 Fixes Ends  */
    {
#ifdef PERF_STATS
        gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats.\
            totalHarqFail =+ 2;
#endif

        /*
           We send the ARQ indication to RLC twice, one for each
           TB and then we free the HARQ Process completely.
           */

        /* _RLC_AM_MODE_ */
        /*DL_HARQ_OPT*/
        if(dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p)
            /*DL_HARQ_OPT*/
        { 

            /* SPR 11402 fix */
            sendARQIndicationToRLC(
                    ueIndex, 
                    &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex);
            /* SPR 11402 fix */
        }
        /* _RLC_AM_MODE_ */
        /*DL_HARQ_OPT*/
        if (dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p)
            /*DL_HARQ_OPT*/
        {									                                        
            /* SPR 11402 fix */
            sendARQIndicationToRLC(
                    ueIndex, 
                    &dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex);
            /* SPR 11402 fix */
        }
        /* SPR 13888 fix start */
#ifdef KPI_STATS
        /* Reset pdcpAckBytes,T1 and T2 at harq Common Index  */
        /* + SPR 17745 */
        lteMacUpdateKpiStatsIPThpDlOnHarqNack(dlHarqProcess_p ,ueIndex,
                recvTTI ,internalCellIndex);
        /* - SPR 17745 */
#endif
        /* SPR 13888 fix end */
#ifdef FDD_CONFIG
        /*SPR 16026 changes start*/
        if(dlHarqProcess_p->scellActBitmask || dlHarqProcess_p->scellDeactBitmask)
        {
            scellHarqFailureForMaxRetx(dlHarqProcess_p,dlUeContext_p,TRUE);
        }
        /*SPR 16026 changes end*/
#endif

        freeDLHarqProcess(dlUeContext_p,
                dlHarqProcess_p,
                internalCellIndex);
    }
}
/* Cyclomatic Complexity changes - ends here */
#endif
/* +- SPR 17777 */
#ifdef FDD_CONFIG
/* + SPR 17733 */

/************************************************************************
 * Function Name  :  processDLAckNackForMsg4
 * Inputs         :  tempUEContext_p -> A pointer to TempCRNTICtx,
 *                   subFrameNumber,
 *                   operation,
 *                   internalCellIndex
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function inits the DL Ack/Nack For Msg4
 ************************************************************************/
MacRetType processDLAckNackForMsg4(TempCRNTICtx * tempUEContext_p, 
                                   UInt32 subFrameNumber, UInt8 operation,InternalCellIndex internalCellIndex)
{
    UInt32 activeIdx = 
        cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
        rachConfigInfo.rachConfiguration.activeIdxContentionRes;
    RACHConfiguration *rachConfig_p = 
        &(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
                rachConfigInfo.rachConfiguration);
    DLHARQProcess* dlHarqProcess_p = PNULL;
    DLHARQEntity*   dlHarqEntity_p = PNULL;
    DLHarqTimerEntityForMsg4List *dlHarqEntityTimerList_p = PNULL;
    DLHarqTimerForMsg4Node *dlharqTimerNode_p = PNULL;
    /* coverity fix 96903 */
    UInt8 tempHarqID = 0;
    UInt16 dlHarqTimerArrayIndex = 0;
    MacRetType retVal = MAC_FAILURE;
    UInt8 msg4MaxHarqRetx = rachConfig_p->msg4MaxHarqRetransmission[activeIdx];

    dlHarqEntity_p = tempUEContext_p->dlHarqCtx_p;
    dlHarqEntityTimerList_p =  &(dlHarqGlobals_g[internalCellIndex].dlHarqTimerEntityForMsg4List[subFrameNumber]);    
    dlHarqTimerArrayIndex =  dlHarqEntity_p->dlHarqTimerNodeArrrayIndex[subFrameNumber];

    if (INVALID_COUNT == dlHarqTimerArrayIndex)
    {
        LOG_MSG(MAC_EXPIRE_DL_HARQ_TIMER_NODE,LOGINFO,MAC_DL_HARQ,
                getCurrentTick(),tempUEContext_p->tcRNTI,LINE_NUMBER,0,
                0,0,DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
        return MAC_FAILURE;
    }
    dlharqTimerNode_p = &(dlHarqEntityTimerList_p->dlHarqTimerNodeForMsg4Arr[
            dlHarqTimerArrayIndex]);
    tempHarqID = dlharqTimerNode_p->harqProcessId;
    LOG_MAC_MSG(MAC_DL_HARQ_ACK_NACK_NODE_INFO,LOGBRIEF,MAC_DL_HARQ,
            getCurrentTick(),
            tempHarqID,
            dlHarqTimerArrayIndex,
            dlharqTimerNode_p->isValid,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    /*
       Since calling the dlHarqTimerStop would be an overhead as we are 
       in any case have to access the global structure of the TIMER node
       hence in the next few lines are performing the function of dlHarqTimerStop
       This would help in improving the our performance.
     */

    if (HARQ_TIMER_NODE_VALID == dlharqTimerNode_p->isValid) 
    {
        dlHarqEntityTimerList_p->count--;
        dlharqTimerNode_p->isValid = HARQ_TIMER_NODE_INVALID;
        dlHarqProcess_p =  &(dlHarqEntity_p->harqProcess[tempHarqID]);
        operation &= ACK_NACK_MASK;

        /* coverity fix 96898 */
        if(operation == ACK)
        {
            /* In case of ACK we simply need to free the HARQ
               Process.
             */
            freeDLHarqProcessForMsg4(dlHarqProcess_p);

            retVal = MAC_SUCCESS;

        }
        else
        {
            if( dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB < 
                    (msg4MaxHarqRetx - 1) )
            {
                putEntryInMsg4ReTxQueue(tempUEContext_p,
                        tempHarqID,
                        internalCellIndex);
            }
            else
            {
                freeDLHarqProcessForMsg4(dlHarqProcess_p);
            }
            retVal = MAC_SUCCESS;

        }

    }
    else
    {
        retVal = MAC_FAILURE;
    }
    return retVal;
}

/*****************************************************************************
 * Function Name  :  freeDLHarqProcessForMsg4
 * Inputs         :  ueContext_p - Pointer to TempCRNTICtx
 *                :  dlHarqProcess_p - The pointer to the DLHARQProcess corresponding
 *                   to appropriate HARQ Process.
 * Outputs        :  None 
 * Returns        :  None
 * Description    :  This function is used when the entire Harq Process associated 
 *                   of a particular TB has to be freed. 
 *****************************************************************************/
void freeDLHarqProcessForMsg4(DLHARQProcess* dlHarqProcess_p)
{
    DLHARQTBInfo* dlHarqTbInfo_p = PNULL;

    dlHarqProcess_p->isTBOneValid = TB_INVALID;
    dlHarqProcess_p->dciFormat = MAX_NUM_DCI_FORMAT;

    dlHarqTbInfo_p = &dlHarqProcess_p->dlHARQTBOneInfo;   
    dlHarqProcess_p->isTBTwoValid= TB_INVALID;
    dlHarqTbInfo_p->taDrxFlag = INVALID_TA_DRX_MSG;
    dlHarqTbInfo_p->txIndicator = INVALID_TX;
    dlHarqTbInfo_p->current_TX_NB = 0;
    dlHarqTbInfo_p->irVersion = 0;
    dlHarqTbInfo_p->tbSize = 0;
    dlHarqTbInfo_p->retransmissionTBSize = 0;
    dlHarqTbInfo_p->paddingBytes = 0;

    dlHarqProcess_p->dlharqRttExpiryTick = 0;
    dlHarqProcess_p->totalTBSize = 0;
    dlHarqProcess_p->assignedResourcesNB = 0;

    if (dlHarqProcess_p->transmissionBuffer_p)
    {
		msgFree((void*)dlHarqProcess_p->transmissionBuffer_p);
        dlHarqProcess_p->transmissionBuffer_p = PNULL;
    }
    dlHarqProcess_p->harqProcessId = INVALID_HARQ_ID;
    dlHarqProcess_p->harqStatus = HARQ_PROCESS_FREE;
}

/************************************************************************
 * Function Name  :  initDLHarqTimerArrayForMsg4
 * Inputs         :  numOfCells - Number of cells configured
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function inits the DL HARQ Timer Array for Msg4
 *                   by initializing the values
 ************************************************************************/
 void initDLHarqTimerArrayForMsg4(UInt8 numOfCells)
{
    UInt8 i = 0;
/*SPR 21653 changes start*/
    UInt16 j = 0;
/*SPR 21653 changes end*/
    UInt8 cellIndex = 0;
    DLHarqTimerEntityForMsg4List *dlHarqEntityTimerList_p = PNULL;
    DLHarqTimerForMsg4Node *dlharqTimerNode_p = PNULL;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    
    for(cellIndex = 0;cellIndex < numOfCells;cellIndex++)
    {

	    for (i = MAX_SUB_FRAME; i; i-- )
	    {
		    dlHarqEntityTimerList_p = &(dlHarqGlobals_g[cellIndex].dlHarqTimerEntityForMsg4List[i-1]);
		    for (j=MAX_DL_UE_SCHEDULED; j; j--)
		    {
			    dlharqTimerNode_p = &dlHarqEntityTimerList_p->dlHarqTimerNodeForMsg4Arr[j-1];
			    dlharqTimerNode_p->harqProcessId = INVALID_HARQ_ID;
			    dlharqTimerNode_p->isValid = HARQ_TIMER_NODE_INVALID;
			    dlharqTimerNode_p->ueIndex = INVALID_UE_ID;
		    }
		    dlHarqEntityTimerList_p->count = 0;
	    }
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
}
/* - SPR 17733 */
/************************************************************************
* Function Name  :  initDLHarqTimerArray
* Inputs         :  numOfCells - Number of cells configured
* Outputs        :  None
* Returns        :  None
* Description    :  This function inits the DL HARQ Timer Array by initializing the values.
************************************************************************/
 void initDLHarqTimerArray(UInt8 numOfCells)
{
     UInt8 subFrameIndex = 0;
     UInt8 indexId = 0;
     UInt8 cellIndex = 0;
     DLHarqTimerEntityList* dlHarqEntityTimerList_p = PNULL;
     DLHarqTimerNode* dlharqTimerNode_p = PNULL;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    for(cellIndex = 0;cellIndex < numOfCells;cellIndex++)
    {
        for (subFrameIndex = MAX_SUB_FRAME; subFrameIndex; subFrameIndex-- )
        {
            dlHarqEntityTimerList_p = &(dlHarqGlobals_g[cellIndex].dlHarqTimerEntityList[subFrameIndex-1]);
            for (indexId=MAX_DL_UE_SCHEDULED; indexId; indexId--)
            {
                dlharqTimerNode_p = &dlHarqEntityTimerList_p->dlHarqTimerNodeArr[indexId-1];
                dlharqTimerNode_p->harqProcessId = INVALID_HARQ_ID;
                dlharqTimerNode_p->isValid = HARQ_TIMER_NODE_INVALID;
                dlharqTimerNode_p->ueIndex = INVALID_UE_ID;
            }
            dlHarqEntityTimerList_p->count = 0;
        }
    }
    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    /* It does not required for TDD, just return */
    return;
}
#endif

/************************************************************************
 * Function Name  :  initDLHarq
 * Inputs         :  numOfCells
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function inits the DL HARQ Timer node values and
 *                   the RetransmitQueue values.
 ************************************************************************/
 void initDLHarq(UInt8 numOfCells)
{
    /* SPR 609 changes start
     * i, j parameter are used inside TDD Config
     * */   
#ifdef TDD_CONFIG    
    UInt8 i = 0;
    UInt16 j = 0;   /*128ue_tdd_support*/
#endif
    InternalCellIndex cellIndex = 0;
    UInt8 dlBler = 0;

    /*
       We INIT the DL HARQ Timer array fields , at first.
     */ 
    /* + SPR 17733 */
#ifdef FDD_CONFIG
    initDLHarqTimerArray(numOfCells);
    initDLHarqTimerArrayForMsg4(numOfCells);
#endif
    /* - SPR 17733 */

    /* INIT the DL CA Timer array fields */
    initDLCATimerArray(numOfCells);
    /*
       We now create the memory pool for the RetransmitQueue.
     */

    createMemPool(sizeof(RetransmitQueueNode),MAX_DL_UE_SCHEDULED*numOfCells);
    /*
     ** Creating Missing TDDHarqTimerExpiryListNode and ContainerNackQueueNode mempool
     ** Assuming 10% of MAX_HARQ_RETX at any time.
     */
#ifdef TDD_CONFIG
    createMemPool(sizeof(TDDHarqTimerExpiryListNode),((MAX_DL_UE_SCHEDULED *MAX_HARQ_RETRANSMISSION_COUNT * DL_HARQ_RTT_TIMER_13_PLUS_4)/MAC_LTE_MEMORY_HARQ_RTX_FACT));
    createMemPool(sizeof(ContainerNackQueueNode),((MAX_DL_UE_SCHEDULED *MAX_HARQ_RETRANSMISSION_COUNT * DL_HARQ_RTT_TIMER_13_PLUS_4)/MAC_LTE_MEMORY_HARQ_RTX_FACT));
#else
    createMemPool(sizeof(DLRetxQueueNode),((MAX_DL_UE_SCHEDULED *MAX_HARQ_RETRANSMISSION_COUNT * 17)/MAC_LTE_MEMORY_HARQ_RTX_FACT)*numOfCells);
    createMemPool(sizeof(ContainerNackQueueNode),((MAX_DL_UE_SCHEDULED *MAX_HARQ_RETRANSMISSION_COUNT * 17)/MAC_LTE_MEMORY_HARQ_RTX_FACT)*numOfCells);
#endif
    createMemPool(sizeof(LTE_SQUEUE), numOfCells);
    createMemPool(sizeof(LTE_SQUEUE), numOfCells);

    for(cellIndex = 0;cellIndex < numOfCells;cellIndex++)
    {
        GET_MEM_FROM_POOL(LTE_SQUEUE,dlRetransmissionQueue_gp[cellIndex],sizeof(LTE_SQUEUE), PNULL);
        sQueueInit(dlRetransmissionQueue_gp[cellIndex]);
        GET_MEM_FROM_POOL(LTE_SQUEUE,dlRetransmissionFailureQueue_gp[cellIndex],sizeof(LTE_SQUEUE), PNULL);
        sQueueInit(dlRetransmissionFailureQueue_gp[cellIndex]);
        /* + SPR 19939 */
#ifdef FDD_CONFIG    
        /* Initialize the Retransmission queue for MSG4 */
        sQueueInit(&Msg4RetransmissionQueue_gp[cellIndex]);
        sQueueInit(&Msg4RetransmissionFailureQueue_gp[cellIndex]);
#endif
        /* - SPR 19939 */
    }

#ifdef TDD_CONFIG
    /* CA Changes Start */
    for(cellIndex = 0;cellIndex < numOfCells;cellIndex++)
    {
        for (j = 0; j < 2; j++ )
        {
            for ( i = 0 ; i < MAX_SUB_FRAME; i++)
            {
                listInit(&tddDlHarqTimerEntityList_g[cellIndex][j][i].dlExpiryList_p);
            }
        }

        tddExpireNodeFromHarqTimerList_p[cellIndex] = PNULL;
        /* SPR 11110 fix start */
        tddExpireAllNodeFromHarqTimerList_p[cellIndex] = PNULL;
        /* SPR 11110 fix end */
    }
    /* CA Changes End */
#endif

    /* SPR 5544/7981 changes start */
    /* The index of array represents the BLER(in the range 0 to 100%).
     * The value of correction factor represents the quantity by which correction
     * should be applied to MCS based on the formula
     * correctionFactor =
     *((TARGET_DL_BLER_VALUE - dlBler) * DL_RESOLUTION_FACTOR)*  DL_BLER_SCALING_VALUE
     */

    for (dlBler = 0; dlBler <= 100; )
    {
        dlBlerCorrectionFactor_g[dlBler] = 
            ( (((TARGET_DL_BLER_VALUE - dlBler) * DL_RESOLUTION_FACTOR) * DL_BLER_SCALING_VALUE) / 100);
        dlBler++;
    }
    /* SPR 5544/7981 changes end */
}

#ifdef TDD_CONFIG
/************************************************************************
 * Function Name  :  setDLHarqHarqTimerList
 * Inputs         :  Internal cell Index
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function inits the DL HARQ Timer list to
 *                   the corresponding UL/DL config.
 ************************************************************************/
 void setDLHarqHarqTimerList(InternalCellIndex cellIndex)
{
    UInt16 subFrameAssign = cellSpecificParams_g.cellConfigAndInitParams_p[
        cellIndex]->cellParams_p->subFrameAssign;

    /* Set the timer node to value based on configured DL/UL config */
    tddExpireNodeFromHarqTimerList_p[cellIndex] = 
        &tddExpireNodeFromHarqTimerList_g[subFrameAssign][0];
    /* SPR 11110 fix start */
    tddExpireAllNodeFromHarqTimerList_p[cellIndex] =
        &tddExpireAllNodeFromHarqTimerList_g[subFrameAssign][0];
    /* SPR 11110 fix end */
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeFromHarqTimerList
 * Inputs         :  dlUeContextInfo_p - pointer to DLUEContextInfo,
 *                   frameNumber - frame number,
 *                   ueIndex - Index of UE,
 *                   indexSfn - Index of System frame number
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs. 
 *                   internalCellIndex - Cell-Index at MAC
 *                   nackFlag_p - To check retx 
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeFromHarqTimerList(DLUEContextInfo* dlUeContextInfo_p,
        UInt8 frameNumber,
        UInt16 ueIndex,
        UInt8 indexSfn
        ,UInt8 freeHarqOnly
		/* + SPS_TDD_Changes */
		,UInt32 schUsed
		/* - SPS_TDD_Changes */
        ,InternalCellIndex internalCellIndex
        ,UInt8 *nackFlag_p
        )
{
    UInt8 tempHarqID = INVALID_HARQ_ID;
    DLHARQEntity*   dlHarqEntity_p = PNULL;
    DLHARQProcess* dlHarqProcess_p = PNULL;
    DLUEContext*  dlUeContext_p = PNULL;
    /* + CA_TDD_HARQ_CHANGES */
    DLUESCellContext *dlUeScellContext_p = PNULL;
    TransmissonMode     transmissionMode = INVALID_TX_MODE;
    /* - CA_TDD_HARQ_CHANGES */
    dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    if (!dlUeContextInfo_p->pendingDeleteFlag)
    {
        /* + CA_TDD_HARQ_CHANGES */
        if(IS_PCELL_OF_UE(dlUeContext_p, internalCellIndex))
        {

            tempHarqID = dlUeContext_p->harqProcessId[indexSfn][frameNumber];
            dlHarqEntity_p = dlUeContext_p->dlHarqContext_p;
            dlHarqProcess_p =  &(dlHarqEntity_p->harqProcess[tempHarqID]);

            /*
             *  Since calling the dlHarqTimerStop would be an overhead as we are 
             *  in any case have to access the global structure of the TIMER node
             *  hence in the next few lines are performing the function of dlHarqTimerStop
             *  This would help in improving the our performance.
             * */
            /* + 13997 SPR Changes */
            dlUeContext_p->harqProcessId[indexSfn][frameNumber] = INVALID_HARQ_ID;
            /* - 13997 SPR Changes */
            transmissionMode  = dlUeContext_p->dlMIMOInfo.transmissionMode;

            LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ,
                    "[%s]global TICK %d Inside of pendingDeleteFlag ue ID %d,"
                    "harq Process ID : %d frameNumber %d,"
                    "DLUE_Contxt_dlHarqTimerNodeArrrayIndex %d \n"
                    ,__func__,getCurrentTick(),dlUeContext_p->ueIndex ,
                    tempHarqID,frameNumber,
                    dlHarqEntity_p->dlHarqTimerNodeArrrayIndex[frameNumber] );
        }
        else
        {
            dlUeScellContext_p = dlUeContext_p->dlUeScellContext_p[START_SCELL_INDEX];
            tempHarqID = dlUeScellContext_p->harqProcessSCellId[indexSfn][frameNumber];
            dlHarqEntity_p = dlUeScellContext_p->ScelldlHarqContext_p;
            dlHarqProcess_p =  &(dlHarqEntity_p->harqProcess[tempHarqID]);

            /*
             *  Since calling the dlHarqTimerStop would be an overhead as we are 
             *  in any case have to access the global structure of the TIMER node
             *  hence in the next few lines are performing the function of dlHarqTimerStop
             *  This would help in improving the our performance.
             * */
            /* + 13997 SPR Changes */
            dlUeScellContext_p->harqProcessSCellId[indexSfn][frameNumber] = INVALID_HARQ_ID; 
            /* - 13997 SPR Changes */
            transmissionMode  =  dlUeScellContext_p->ScelldlMIMOInfo.transmissionMode;

            LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ,
                    "[%s]global TICK %d Inside of pendingDeleteFlag ue ID %d,"
                    "harq Process ID : %d frameNumber %d,"
                    "DLUE_Contxt_dlHarqTimerNodeArrrayIndex %d \n"
                    ,__func__,getCurrentTick(),dlUeContext_p->ueIndex ,
                    tempHarqID,frameNumber,
                    dlHarqEntity_p->dlHarqTimerNodeArrrayIndex[frameNumber] );

        }
        /* - CA_TDD_HARQ_CHANGES */
        LOG_MAC_MSG(MAC_EXPIRE_DL_HARQ_TIMER_NODE, LOGINFO, MAC_DL_HARQ,
                getCurrentTick(), frameNumber, tempHarqID, ueIndex, 
                freeHarqOnly, 0, 0,0, __func__, "");
        /*
           We now have to check the UE Transmission Mode (MIMO/Non-MIMO)
           as in the case when both the TBs are on re-transmission we have to 
           call the function putEntryInUERetransmissionQueue () only once.
           */
        if (transmissionMode == TX_MODE_3 || transmissionMode == TX_MODE_4 ||
                /* + TM7_8 Changes Start */
                ((transmissionMode == TX_MODE_8) && 
                 (1 < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                  cellParams_p->numOfTxAnteenas)))
            /* - TM7_8 Changes End */
        {

            checkTBsAndPutEntryInRetxQueue(dlUeContext_p,dlHarqProcess_p,
                                     internalCellIndex,nackFlag_p,freeHarqOnly,
                                     /* + SPS_TDD_Changes */
                                     schUsed
                                     /* - SPS_TDD_Changes */
                    );    
        }
        /*
           The UE is running in NON-MINO mode and hence the Handling below.
           */
        else
        {
            /* SPR 2249 Fixes Begins */
            if (dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB < \
                    (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                     cellParams_p->maxHarqRetransmission - 1)   
                    && (freeHarqOnly == 0)
               )     
                /* SPR 2249 Fixes Ends */
            {
                /*
                   Here the TB one is yet to complete the max re-trans.
                   */
                LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,\
                        "[%s] current_TX_NB %d \n",__func__, 
                        dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB);
                *nackFlag_p = TRUE;
            }
            /*
               We now consider the case When TB_One has reached it's
               max re trans limit.
               */
            else
            {
                /* _RLC_AM_MODE_ */
                if (!((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_MSG) ||
                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == DRX_MSG) ||
                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_DRX_MSG) || 
                            (dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                        (dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p))
                { 

                    /* SPR 11402 fix */
                    sendARQIndicationToRLC(
                            ueIndex, 
                            &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex);
                    /* SPR 11402 fix */
                }

                freeDLHarqProcessTBSpecific(
                        dlUeContext_p,
                        dlHarqProcess_p,
                        TB_ONE,
                        internalCellIndex);

            }
        }
    }
    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}


/*****************************************************************************
 * Function Name  :  tddExpireNodeFromHarqTimerListForConfig5
 * Inputs         :  dlUeContextInfo_p - pointer to DLUEContextInfo,
 *                   frameNumber - frame number,
 *                   ueIndex - Index of UE,
 *                   indexSfn - Index of System frame number
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs. 
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeFromHarqTimerListForConfig5(DLUEContextInfo* dlUeContextInfo_p,
                                             UInt8 frameNumber,
                                             UInt16 ueIndex,
                                             UInt8 indexSfn
                                            ,UInt8 freeHarqOnly
                                            ,InternalCellIndex internalCellIndex
                                                     )
{
    UInt8 tempHarqID = INVALID_HARQ_ID;
    DLHARQEntity*   dlHarqEntity_p = PNULL;
    DLHARQProcess* dlHarqProcess_p = PNULL;
    DLUEContext*  dlUeContext_p = PNULL;        
    dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;
    tempHarqID = dlUeContext_p->harqProcessId[indexSfn][frameNumber];

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    if (!dlUeContextInfo_p->pendingDeleteFlag)
    {
        dlHarqEntity_p= dlUeContext_p->dlHarqContext_p;
        dlHarqProcess_p = \
                          &(dlHarqEntity_p->harqProcess[dlUeContext_p->harqProcessId[indexSfn][frameNumber]]);
        /*
           Since calling the dlHarqTimerStop would be an overhead as we are 
           in any case have to access the global structure of the TIMER node
           hence in the next few lines are performing the function of dlHarqTimerStop
           This would help in improving the our performance.
         */
        dlUeContext_p->harqProcessId[indexSfn][frameNumber] = HARQ_PROCESS_FREE;

        LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ,
                "[%s]global TICK %d Inside of pendingDeleteFlag ue ID %d,"
                "harq Process ID : %d frameNumber %d,DLUE_Contxt_dlHarqTimerNodeArrrayIndex %d \n"
                ,__func__,getCurrentTick(),dlUeContext_p->ueIndex ,
                tempHarqID,frameNumber,
                dlUeContext_p->dlHarqContext_p->dlHarqTimerNodeArrrayIndex[frameNumber] );
        
        LOG_MAC_MSG(MAC_EXPIRE_DL_HARQ_TIMER_NODE, LOGINFO, MAC_DL_HARQ,
                getCurrentTick(), frameNumber, tempHarqID, ueIndex, 
                freeHarqOnly, 0, 0,0, __func__, "");
        /*
           We now have to check the UE Transmission Mode (MIMO/Non-MIMO)
           as in the case when both the TBs are on re-transmission we have to 
           call the function putEntryInUERetransmissionQueue () only once.
         */
        if (dlUeContext_p->dlMIMOInfo.transmissionMode == TX_MODE_3 ||
                dlUeContext_p->dlMIMOInfo.transmissionMode == TX_MODE_4 ||
               /* + TM7_8 Changes Start */
               ((dlUeContext_p->dlMIMOInfo.transmissionMode == TX_MODE_8) && 
                (1 < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                 cellParams_p->numOfTxAnteenas)))
               /* - TM7_8 Changes End */
        {
            if (dlHarqProcess_p->isTBOneValid) 
            {
                /*
                   We first consider the case when both the TBs are VALID.
                 */
                if (dlHarqProcess_p->isTBTwoValid)
                {

                    /*
                       We now check for the re-transmission counter of both
                       the TBs.
                     */
                    LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ, "[%s]"
                            "The vlue of current_TX_NB_TB_ONE = %d "\
                            "The vlue of current_TX_NB_TB_TWO = %d \n",__func__,
                            dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB,
                            dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB);
                    /* SPR 2249 Fixes Begins */
                    if ((dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB == \
                            (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                             cellParams_p->maxHarqRetransmission - 1)
                            && dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB == \
                            (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                             cellParams_p->maxHarqRetransmission - 1))
                        || (freeHarqOnly == 1)
                            )     
                    /* SPR 2249 Fixes Ends */
                    {
                        /* _RLC_AM_MODE_ */
                        if (!((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_MSG) ||
                                    (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == DRX_MSG) ||
                                    (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_DRX_MSG) || 
                                    (dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                                    (dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p))
                        { 

                            /* SPR 11402 fix */
                            sendARQIndicationToRLC(
                                    ueIndex, 
                                    &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex);
                            /* SPR 11402 fix */
                        }

                        /* _RLC_AM_MODE_ */
                        if (!((dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_MSG) ||
                                    (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == DRX_MSG) ||
                                    (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_DRX_MSG) || 
                                    (dlHarqProcess_p->dlHARQTBTwoInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                                    (dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p))
                        {																		   
                            /* SPR 11402 fix */
                            sendARQIndicationToRLC(
                                    ueIndex, 
                                    &dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex);
                            /* SPR 11402 fix */
                        }

                        /*
                           We send the ARQ indication to RLC twice, one for each
                           TB and then we free the HARQ Process completely.
                         */
                        /* 
                           The following section of code will be implemented Later

                           sendARQIndicationToRLC(
                           ueIndex, 
                           dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p->lcCountTBOne, 
                           &(dlHarqProcess_p->dlHARQTBOneInfo.arqInfo[0]));
                           sendARQIndicationToRLC(
                           ueIndex, 
                           dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p->lcCountTBTwo, 
                           &(dlHarqProcess_p->dlHARQTBTwoInfo.arqInfo[0]));
                         */
                        freeDLHarqProcess(dlUeContext_p,
                                dlHarqProcess_p,
                                internalCellIndex);
                    }
                    else
                    {
                        /* RETX_CHG */
                        /* SPS_CHG */
                        putEntryInDLRetransmissionQueue(dlUeContext_p, 
                                tempHarqID, dlHarqProcess_p->
                                dlharqRttExpiryTick, DRA_SCHEDULING,
                                internalCellIndex); 
                        /* SPS_CHG */
                    }
                }
                /*
                   We now consider the case when Only TB ONE is Valid and 
                   TB_TWO is invalid (MIMO case).
                 */
                else
                {
                    /* SPR 2249 Fixes Begins */
                    if (dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB < \
                            (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                             cellParams_p->maxHarqRetransmission - 1)     
                        && (freeHarqOnly == 0)
                            )     
                    /* SPR 2249 Fixes Ends */
                    {
                        /*
                           Here the TB one is yet to complete the max re-trans.
                         */
                        /* RETX_CHG */
                        /* SPS_CHG */
                        putEntryInDLRetransmissionQueue(dlUeContext_p, 
                                tempHarqID, dlHarqProcess_p->
                                dlharqRttExpiryTick, DRA_SCHEDULING,
                                internalCellIndex); 
                        /* SPS_CHG */
                        /* RETX_CHG */

                    }
                    /*
                       We now consider the case When TB_One 
                       has reached it's max re trans limit.
                     */
                    else
                    {
                        /* _RLC_AM_MODE_ */
                        if (!((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_MSG) ||
                                    (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == DRX_MSG) ||
                                    (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_DRX_MSG) || 
                                    (dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                                    (dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p))
                        { 

                            /* SPR 11402 fix */
                            sendARQIndicationToRLC(
                                    ueIndex, 
                                    &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex);
                            /* SPR 11402 fix */
                        }


                        freeDLHarqProcessTBSpecific(
                                dlUeContext_p,
                                dlHarqProcess_p,
                                TB_ONE,
                                internalCellIndex);
                    }
                }
            }
            /*
               We now consider the Case when ONLY  TB Two is Valid and
               TB One is invalid (MIMO case).
             */
            else
            {
                /* SPR 2249 Fixes Begins */
                if (dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB < \
                        (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                         cellParams_p->maxHarqRetransmission - 1)     
                                        && (freeHarqOnly == 0)
                                   )     
                /* SPR 2249 Fixes Ends */
                {
                    /*
                       Here the TB one is yet to complete the max re-trans.
                     */
                    /* RETX_CHG */
                    /* SPS_CHG */
                    putEntryInDLRetransmissionQueue(dlUeContext_p, 
                            tempHarqID, dlHarqProcess_p->
                                dlharqRttExpiryTick, DRA_SCHEDULING,
                                internalCellIndex); 
                    /* SPS_CHG */
                    /* RETX_CHG */
                }
                /*
                   We now consider the case When TB_Two has reached it's
                   max re trans limit.
                 */
                else
                {
                    /* _RLC_AM_MODE_ */
                    if (!((dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_MSG) ||
                                (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == DRX_MSG) ||
                                (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_DRX_MSG) || 
                                (dlHarqProcess_p->dlHARQTBTwoInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                                (dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p))
                    {																		   
                        /* SPR 11402 fix */
                        sendARQIndicationToRLC(
                                ueIndex, 
                                &dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex);
                        /* SPR 11402 fix */
                    }

                    freeDLHarqProcessTBSpecific(
                            dlUeContext_p,
                            dlHarqProcess_p,
                            TB_TWO,
                            internalCellIndex);
                }
            }
        }
        /*
           The UE is running in NON-MINO mode and hence the Handling below.
         */
        else
        {
            /* SPR 2249 Fixes Begins */
            if (dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB < \
                    (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                     cellParams_p->maxHarqRetransmission - 1)   
                    && (freeHarqOnly == 0)
                                   )     
            /* SPR 2249 Fixes Ends */
            {
                /*
                   Here the TB one is yet to complete the max re-trans.
                 */
                LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,\
                        "[%s] current_TX_NB %d \n",__func__, 
                        dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB);
                /* RETX_CHG */
                /* SPS_CHG */
                putEntryInDLRetransmissionQueue(dlUeContext_p, 
                        tempHarqID, dlHarqProcess_p->
                        dlharqRttExpiryTick, DRA_SCHEDULING,
                        internalCellIndex); 
                /* SPS_CHG */
                /* RETX_CHG */
            }
            /*
               We now consider the case When TB_One has reached it's
               max re trans limit.
             */
            else
            {
                /* _RLC_AM_MODE_ */
                if (!((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_MSG) ||
                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == DRX_MSG) ||
                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_DRX_MSG) || 
                            (dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                            (dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p))
                { 

                    /* SPR 11402 fix */
                    sendARQIndicationToRLC(
                            ueIndex, 
                            &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex);
                    /* SPR 11402 fix */
                }

                freeDLHarqProcessTBSpecific(
                        dlUeContext_p,
                        dlHarqProcess_p,
                        TB_ONE,
                        internalCellIndex);

            }
        }
    }
    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
				DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  :  tddExpireAllNodeFromHarqTimerList 
 * Inputs         :  frameNumber,
 *                   currentSubFrame,
 *                   delaySubFrame,
 *                   sfn
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs. 
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer 
 *                   corresponding to particular sub frame, this function puts 
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireAllNodeFromHarqTimerList (
        UInt32 frameNumber,
        UInt32 currentSubFrame,
        UInt8 delaySubFrame,
        UInt32 sfn,
        UInt8 freeHarqOnly,
        InternalCellIndex internalCellIndex )
{
    UInt8 loopNumberOfSubFrames = 0;
    UInt16 maxTickDiff = MAC_MAX_VAL( 
            totalTickMissErrIndDl_g[internalCellIndex],
            totalNumberOfTickDiffDl_g[internalCellIndex] );

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    if ( maxTickDiff )
    {
        loopNumberOfSubFrames = maxTickDiff;

        /* SPR 11230 fix start */
        /* Check if it is the error indication scenario and 
         * above threshold. In case of above threshold value, freeHarqOnly 
         * will be set to TRUE else it will be FALSE as 
         * calculateHarqExpTickMissAndArgs will not be called. */
        if( 0 < totalTickMissErrIndDl_g[internalCellIndex] && 
            LTE_TRUE == freeHarqOnly )
        {
            /* Need to free all the nodes */
            loopNumberOfSubFrames = MAX_NUM_OF_TICK_MISSED;
        }
        else if( 0 == totalTickMissErrIndDl_g[internalCellIndex] )
        {
            if( maxTickDiff >= MAX_NUM_OF_TICK_MISSED )
            {
                /* Need to free all the nodes */
                freeHarqOnly = LTE_TRUE;
                loopNumberOfSubFrames = MAX_NUM_OF_TICK_MISSED;
            }
            else
            {
                /* Put the entry for re-transmissions as the value is tick 
                 * miss diff is less than thresold */
                freeHarqOnly = LTE_FALSE;
            }
        }
        /* SPR 11230 fix end */

    }

    /*TDD Tick overshoot changes start*/
    if (loopNumberOfSubFrames)
    {
        /* SPR 11230 fix start */
        LOG_MAC_MSG( MAC_EL_DL_EXPIRY, LOGDEBUG, MAC_DL_HARQ, getCurrentTick(),
                frameNumber, loopNumberOfSubFrames, freeHarqOnly, 
                totalTickMissErrIndDl_g[internalCellIndex], 
                totalNumberOfTickDiffDl_g[internalCellIndex],delaySubFrame,currentSubFrame,
                __func__, "");
        /* SPR 11230 fix end */
        do{
            /* SPR 11110 fix start */
            /* SPR 11230 fix start */
            if( LTE_TRUE == freeHarqOnly )
            /* SPR 11230 fix end */
            {
                tddExpireAllNodeFromHarqTimerList_p[internalCellIndex]
                    [frameNumber]( sfn, freeHarqOnly, internalCellIndex);
            }
            else
            {
                tddExpireNodeFromHarqTimerList_p[internalCellIndex]
                    [frameNumber](sfn, freeHarqOnly, internalCellIndex );
            }
            /* SPR 11110 fix end */

            if (frameNumber == 0)
            { 
                frameNumber = NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME - 1;
                if (sfn == 0)
                {
                    sfn = MAX_SFN_VALUE - 1;
                }
                else
                {
                    --sfn;
                }
            }
            else
            {
                --frameNumber;
            }
        }while(--loopNumberOfSubFrames);
    }
    else
    {
        tddExpireNodeFromHarqTimerList_p[internalCellIndex][frameNumber]( 
                sfn, freeHarqOnly, internalCellIndex );
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    /* Reset totalTickMissErrIndDl_g at the end of EL */
    totalTickMissErrIndDl_g[internalCellIndex] = 0;
    return MAC_SUCCESS;
}

/* SPR 11110 fix start */
/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig0
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   dlDatTxSubframe - DL Data Tx Subframe Number
 *                   dlAckNackSubFrame - DL Data Ack Nack Subframe Number
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach
 *                   the maximun value) into retransmission queue.
 *****************************************************************************/
STATIC MacRetType tddExpireAllNodeForConfig0(UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex )
{
    UInt16 ueIndex = INVALID_UE_ID;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUeContext_p        = PNULL;
    LTE_LIST_NODE * node_p             = PNULL;
    LTE_LIST_NODE * nextNode_p          = PNULL;
    UInt8 txSFNIndex              = 0;
    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    node_p = getListNode( &(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                                       [dlAckNackSubFrame].dlExpiryList_p), 0);

    while (PNULL != node_p)
    {
        ueIndex = ((TDDHarqTimerExpiryListNode*)node_p)->ueIndex;
        dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;


        if (!dlUeContextInfo_p->pendingDeleteFlag)
        {
            /* + CA_TDD_HARQ_CHANGES */ 
            /* SPR 11087 Fix Start */ 
            if(SUB_FRAME_6 == dlDatTxSubframe) 
            { 
                txSFNIndex = MODULO_TWO(containerIndex + 1); 
            }
            else 
            { 
                /* SPR 11087 Fix End */ 
                txSFNIndex = containerIndex; 
            }
          
            tddExpireHarqNode(ueIndex,containerIndex,
                              dlAckNackSubFrame, dlDatTxSubframe,
                              txSFNIndex,freeHarqOnly,internalCellIndex);

            dlUeContext_p->tddHarqTimerExpiryNode[containerIndex][dlAckNackSubFrame]
                                                                           = PNULL;
        }
        nextNode_p = getNextListNode(node_p);
        listDeleteNode( &tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex][dlAckNackSubFrame].dlExpiryList_p,
                      (node_p));
        freeMemPool((void *)node_p);
        node_p = nextNode_p;

        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        /* Resetting bit corresponding to UL sf when PUSCH was sent with DCI0 */
        RESET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(ueIndex,dlAckNackSubFrame);
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig0Subframe2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig0Subframe2( UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 dlDatTxSubframe = SUB_FRAME_6;
    UInt8 dlAckNackSubFrame = SUB_FRAME_2;
    UInt8 containerIndex  = 0;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireAllNodeForConfig0( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig0Subframe4
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig0Subframe4(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 dlDatTxSubframe = SUB_FRAME_0;
    UInt8 dlAckNackSubFrame = SUB_FRAME_4;
    UInt8 containerIndex  = 0;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireAllNodeForConfig0( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame, 
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig0Subframe7
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig0Subframe7(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 dlDatTxSubframe = SUB_FRAME_1;
    UInt8 dlAckNackSubFrame = SUB_FRAME_7;
    UInt8 containerIndex  = 0;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireAllNodeForConfig0( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame, 
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig0Subframe9
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig0Subframe9(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 dlDatTxSubframe = SUB_FRAME_5;
    UInt8 dlAckNackSubFrame = SUB_FRAME_9;
    UInt8 containerIndex  = 0;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireAllNodeForConfig0( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame, 
                internalCellIndex );
    }

    return MAC_SUCCESS;
}
/* SPR 11110 fix end */

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig0Subframe4
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig0Subframe4( UInt32 sfn, 
        UInt8 freeHarqOnly,
        InternalCellIndex internalCellIndex )
{
    /* TDD Config 0 Changes Start */
    UInt8 dlDatTxSubframe = SUB_FRAME_0;
    UInt8 dlAckNackSubFrame = SUB_FRAME_4;
    /* TDD Config 0 Changes End */
    /* SPR 11110 fix start */
    UInt8 containerIndex    = 0;

    /* Fetch the ackNack Container index*/
    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );

    tddExpireAllNodeForConfig0( containerIndex, freeHarqOnly, 
            dlDatTxSubframe,  dlAckNackSubFrame, internalCellIndex );
    /* SPR 11110 fix end */

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig0Subframe7
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig0Subframe7(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
    /* TDD Config 0 Changes Start */
    UInt8 dlDatTxSubframe = SUB_FRAME_1;
    UInt8 dlAckNackSubFrame = SUB_FRAME_7;
    /* TDD Config 0 Changes End */
    /* SPR 11110 fix start */
    UInt8 containerIndex    = 0;

    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );

    tddExpireAllNodeForConfig0( containerIndex, freeHarqOnly, 
            dlDatTxSubframe,  dlAckNackSubFrame, internalCellIndex );
    /* SPR 11110 fix end */

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig0Subframe9
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig0Subframe9(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
    /* TDD Config 0 Changes Start */
    UInt8 dlDatTxSubframe = SUB_FRAME_5;
    UInt8 dlAckNackSubFrame = SUB_FRAME_9;
    /* TDD Config 0 Changes End */
    /* SPR 11110 fix start */
    UInt8 containerIndex    = 0;

    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );

    tddExpireAllNodeForConfig0( containerIndex, freeHarqOnly, 
            dlDatTxSubframe,  dlAckNackSubFrame, internalCellIndex );    
    /* SPR 11110 fix end */

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig0Subframe2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireNodeForConfig0Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
    /* TDD Config 0 Changes Start */
    UInt8 dlDatTxSubframe = SUB_FRAME_6;
    UInt8 dlAckNackSubFrame = SUB_FRAME_2;
    /* TDD Config 0 Changes End */
    /* SPR 11110 fix start */
    UInt8 containerIndex    = 0;

    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );

    tddExpireAllNodeForConfig0( containerIndex, freeHarqOnly, 
            dlDatTxSubframe,  dlAckNackSubFrame, internalCellIndex );
    /* SPR 11110 fix end */

    return MAC_SUCCESS;
}

/* SPR 11110 fix start */
/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig1Subframe7
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig1Subframe7(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 dlDatTxSubframe = SUB_FRAME_0;
    UInt8 dlAckNackSubFrame = SUB_FRAME_7;
    UInt8 containerIndex  = 0;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireNodesForContainerIdxConfig1SubFrame7( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame, 
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig1Subframe7
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig1Subframe7(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        )
{
    UInt8 dlDatTxSubframe = SUB_FRAME_0;
    UInt8 dlAckNackSubFrame = SUB_FRAME_7;
    UInt8 containerIndex    = 0;

    /* Fetch the ackNack Container index*/
    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );

    tddExpireNodesForContainerIdxConfig1SubFrame7( containerIndex,
            freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame, 
            internalCellIndex );

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodesForContainerIdxConfig1SubFrame7
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   dlDatTxSubframe
 *                   dlAckNackSubFrame
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
STATIC MacRetType tddExpireNodesForContainerIdxConfig1SubFrame7(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame,
        InternalCellIndex internalCellIndex )
/*SPR 11110 fix end */
{
    UInt16 ueIndex = INVALID_UE_ID;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUeContext_p        = PNULL;
    LTE_LIST_NODE * node_p             = PNULL;
    LTE_LIST_NODE * nextNode_p          = PNULL;
    UInt8 txSFNIndex              = 0;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    node_p = getListNode
        (&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex][dlAckNackSubFrame].dlExpiryList_p), 0); 

    while (PNULL != node_p)
    {
        ueIndex = ((TDDHarqTimerExpiryListNode*)node_p)->ueIndex;
        dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;

        if (dlUeContextInfo_p->pendingDeleteFlag)
        {
            nextNode_p = getNextListNode(node_p);
            listDeleteNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                        [dlAckNackSubFrame].dlExpiryList_p), node_p);
            freeMemPool(node_p);
            node_p = nextNode_p;
            continue;
        }

        /* Tx SFN index for SF 0 and SF 1 */
        txSFNIndex = containerIndex;

        /* check whether the data is transmitted in dlDatTxSubframe */
        dlDatTxSubframe = SUB_FRAME_1;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlDatTxSubframe = SUB_FRAME_0;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlUeContext_p->tddHarqTimerExpiryNode[containerIndex][dlAckNackSubFrame] 
                                                                        = PNULL;
        nextNode_p = getNextListNode(node_p);
        /** CA-TDD Changes End **/
        listDeleteNode( &tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                [dlAckNackSubFrame].dlExpiryList_p,(node_p));
        /** CA-TDD Changes End **/
        freeMemPool((void *)node_p);

        node_p = nextNode_p;

        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        /* Resetting bit corresponding to UL sf when PUSCH was sent with DCI0 */
        RESET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(ueIndex,dlAckNackSubFrame);
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}

/*SPR 11110 fix start */
/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig1Subframe8
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig1Subframe8(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 dlAckNackSubFrame = SUB_FRAME_8;
    UInt8 dlDatTxSubframe = SUB_FRAME_4;
    UInt8 containerIndex  = 0;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireNodesForContainerIdxConfig1SubFrame8( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame, 
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig1Subframe8
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index                  
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig1Subframe8(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
    UInt8 dlAckNackSubFrame = SUB_FRAME_8;
    UInt8 dlDatTxSubframe = SUB_FRAME_4;
    UInt8 containerIndex    = 0;

    /* Fetch the ackNack Container index*/
    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );

    tddExpireNodesForContainerIdxConfig1SubFrame8( containerIndex,
            freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame, 
            internalCellIndex );

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodesForContainerIdxConfig1SubFrame8
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   dlAckNackSubFrame
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
STATIC MacRetType tddExpireNodesForContainerIdxConfig1SubFrame8(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame,
        /*SPR 11110 fix end */
        InternalCellIndex internalCellIndex )
{
    UInt16 ueIndex = INVALID_UE_ID;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUeContext_p        = PNULL;
    LTE_LIST_NODE * node_p             = PNULL;
    LTE_LIST_NODE * nextNode_p          = PNULL;
    UInt8 txSFNIndex              = 0;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    node_p = getListNode
        (&(tddDlHarqTimerEntityList_g[internalCellIndex]
           [containerIndex][dlAckNackSubFrame].dlExpiryList_p), 0); 

    while (PNULL != node_p)
    {
        ueIndex = ((TDDHarqTimerExpiryListNode*)node_p)->ueIndex;
        dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;

        if (dlUeContextInfo_p->pendingDeleteFlag)
        {
            nextNode_p = getNextListNode(node_p);
            /** CA-TDD Changes Start **/
            listDeleteNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                        [dlAckNackSubFrame].dlExpiryList_p), node_p);
            /** CA-TDD Changes End **/
            freeMemPool(node_p);
            node_p = nextNode_p;
            continue;
        }

        /* check whether the data is transmitted in dlDatTxSubframe */
        dlAckNackSubFrame = SUB_FRAME_8;
        txSFNIndex = containerIndex;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlUeContext_p->tddHarqTimerExpiryNode[containerIndex][dlAckNackSubFrame] 
                                                                        = PNULL;
        nextNode_p = getNextListNode(node_p);
        /** CA-TDD Changes Start **/
        listDeleteNode( &tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                [dlAckNackSubFrame].dlExpiryList_p,(node_p));
        /** CA-TDD Changes End **/
        freeMemPool((void *)node_p);

        node_p = nextNode_p;

        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        /* Resetting bit corresponding to UL sf when PUSCH was sent with DCI0 */
        RESET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(ueIndex,dlAckNackSubFrame);
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}

/*SPR 11110 fix start */
/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig1Subframe2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig1Subframe2(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 dlDatTxSubframe = SUB_FRAME_5;
    UInt8 dlAckNackSubFrame = SUB_FRAME_2;
    UInt8 containerIndex = 0;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireNodesForContainerIdxConfig1SubFrame2( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame, 
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig1Subframe2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig1Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        )
{
    UInt8 dlDatTxSubframe = SUB_FRAME_5;
    UInt8 dlAckNackSubFrame = SUB_FRAME_2;
    UInt8 containerIndex    = 0;

    /* Fetch the ackNack Container index*/
    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );

    tddExpireNodesForContainerIdxConfig1SubFrame2( containerIndex,
            freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
            internalCellIndex );

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodesForContainerIdxConfig1SubFrame2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   dlAckNackSubFrame -
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
STATIC MacRetType tddExpireNodesForContainerIdxConfig1SubFrame2(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex )
/*SPR 11110 fix end */
{
    UInt16 ueIndex = INVALID_UE_ID;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUeContext_p        = PNULL;
    LTE_LIST_NODE * node_p             = PNULL;
    LTE_LIST_NODE * nextNode_p          = PNULL;
    UInt8 txSFNIndex              = 0;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    node_p = 
        getListNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                    [dlAckNackSubFrame].dlExpiryList_p), 0); 

    while (PNULL != node_p)
    {
        ueIndex = ((TDDHarqTimerExpiryListNode*)node_p)->ueIndex;
        dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;

        if (dlUeContextInfo_p->pendingDeleteFlag)
        {
            nextNode_p = getNextListNode(node_p);
            /** CA-TDD Changes Start **/
            listDeleteNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                        [dlAckNackSubFrame].dlExpiryList_p), node_p);
            /** CA-TDD Changes Start **/
            freeMemPool(node_p);
            node_p = nextNode_p;
            continue;
        }
        /* Tx SFN index for SF 5 and SF 6 */
        txSFNIndex = MODULO_TWO((containerIndex+ 1));

        /* check whether the data is transmitted in dlDatTxSubframe */
        dlDatTxSubframe = SUB_FRAME_6;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlDatTxSubframe = SUB_FRAME_5;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlUeContext_p->tddHarqTimerExpiryNode[containerIndex][dlAckNackSubFrame] 
                                                                        = PNULL;
        nextNode_p = getNextListNode(node_p);
        /** CA-TDD Changes Start **/
        listDeleteNode( &tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                [dlAckNackSubFrame].dlExpiryList_p,(node_p));
        /** CA-TDD Changes Start **/
        freeMemPool((void *)node_p);

        node_p = nextNode_p;

        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        /* Resetting bit corresponding to UL sf when PUSCH was sent with DCI0 */
        RESET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(ueIndex,dlAckNackSubFrame);
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}

/*SPR 11110 fix start */
/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig1Subframe3
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig1Subframe3(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 dlDatTxSubframe = SUB_FRAME_9;
    UInt8 dlAckNackSubFrame = SUB_FRAME_3;
    UInt8 containerIndex  = 0;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireNodesForContainerIdxConfig1SubFrame3( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig1Subframe3
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig1Subframe3(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        )
{
    UInt8 dlDatTxSubframe = SUB_FRAME_9;
    UInt8 dlAckNackSubFrame = SUB_FRAME_3;
    UInt8 containerIndex               = 0;

    /* Fetch the ackNack Container index*/
    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );

    tddExpireNodesForContainerIdxConfig1SubFrame3( containerIndex,
            freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
            internalCellIndex );

    return MAC_SUCCESS;

}

/*****************************************************************************
 * Function Name  :  tddExpireNodesForContainerIdxConfig1SubFrame3
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   dlAckNackSubFrame
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
STATIC MacRetType tddExpireNodesForContainerIdxConfig1SubFrame3(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex )
/*SPR 11110 fix end */
{
    UInt16 ueIndex = INVALID_UE_ID;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUeContext_p        = PNULL;
    LTE_LIST_NODE * node_p             = PNULL;
    LTE_LIST_NODE * nextNode_p          = PNULL;
    UInt8 txSFNIndex              = 0;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    node_p = 
        getListNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                    [dlAckNackSubFrame].dlExpiryList_p), 0); 

    while (PNULL != node_p)
    {
        ueIndex = ((TDDHarqTimerExpiryListNode*)node_p)->ueIndex;
        dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];

        if (!dlUeContextInfo_p->pendingDeleteFlag)
        {
            dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;

            /* check whether the data is transmitted in dlDatTxSubframe */
            dlDatTxSubframe = SUB_FRAME_9;
            txSFNIndex = MODULO_TWO((containerIndex + 1));

            tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

            dlUeContext_p->tddHarqTimerExpiryNode[containerIndex][dlAckNackSubFrame] 
                                                                            = PNULL;
        }
        nextNode_p = getNextListNode(node_p);
        /** CA-TDD Changes Start **/
        listDeleteNode( &tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                [dlAckNackSubFrame].dlExpiryList_p,(node_p));
        /** CA-TDD Changes End **/
        freeMemPool((void *)node_p);

        node_p = nextNode_p;

        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        /* Resetting bit corresponding to UL sf when PUSCH was sent with DCI0 */
        RESET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(ueIndex,dlAckNackSubFrame);
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}

/*SPR 11110 fix start */
/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig2Subframe7
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig2Subframe7(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 dlDatTxSubframe = SUB_FRAME_9;
    UInt8 dlAckNackSubFrame = SUB_FRAME_7;
    UInt8 containerIndex = 0;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireNodesForContainerIdxConfig2SubFrame7( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig2Subframe7
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig2Subframe7(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        )
{
    UInt8 dlDatTxSubframe = SUB_FRAME_9;
    UInt8 dlAckNackSubFrame = SUB_FRAME_7;
    UInt8 containerIndex               = 0;

    /* Fetch the ackNack Container index*/
    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );

    tddExpireNodesForContainerIdxConfig2SubFrame7( containerIndex,
            freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
            internalCellIndex );

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodesForContainerIdxConfig2SubFrame7
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   dlAckNackSubFrame -
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
STATIC MacRetType tddExpireNodesForContainerIdxConfig2SubFrame7(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex )
/*SPR 11110 fix end */
{
    UInt16 ueIndex = INVALID_UE_ID;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUeContext_p        = PNULL;
    LTE_LIST_NODE * node_p             = PNULL;
    LTE_LIST_NODE * nextNode_p         = PNULL;
    UInt8 txSFNIndex = 0;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    node_p = 
        getListNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                    [dlAckNackSubFrame].dlExpiryList_p), 0); 

    while (PNULL != node_p)
    {
        ueIndex = ((TDDHarqTimerExpiryListNode*)node_p)->ueIndex;
        dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];

        if (!dlUeContextInfo_p->pendingDeleteFlag)
        {
            dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;

            dlDatTxSubframe = SUB_FRAME_9;
            /* SPR 18122 Changes Start*/
            txSFNIndex = MODULO((containerIndex + 1),2);
            /* SPR 18122 Changes End*/

            tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex); 

            /* Tx SFN index for SF 0, 1 and 3 */
            txSFNIndex = containerIndex;

            dlDatTxSubframe = SUB_FRAME_1;
            tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

            dlDatTxSubframe = SUB_FRAME_0;
            tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

            dlDatTxSubframe = SUB_FRAME_3;
            tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

            dlUeContext_p->tddHarqTimerExpiryNode[containerIndex][dlAckNackSubFrame] 
                                                                            = PNULL;
        }
        nextNode_p = getNextListNode(node_p);
        /** CA-TDD Changes Start **/
        listDeleteNode( &tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                [dlAckNackSubFrame].dlExpiryList_p,(node_p));
        /** CA-TDD Changes End **/
        freeMemPool((void *)node_p);

        node_p = nextNode_p;

        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        /* Resetting bit corresponding to UL sf when PUSCH was sent with DCI0 */
        RESET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(ueIndex,dlAckNackSubFrame);
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}

/*SPR 11110 fix start */
/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig2Subframe2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig2Subframe2(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 dlDatTxSubframe = SUB_FRAME_4;
    UInt8 dlAckNackSubFrame = SUB_FRAME_2;
    UInt8 containerIndex = 0;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireNodesForContainerIdxConfig2SubFrame2( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame, 
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig2Subframe2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the node in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig2Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        )
/* + coverity 55313 */
{
    UInt8 dlDatTxSubframe = SUB_FRAME_4;
    UInt8 dlAckNackSubFrame = SUB_FRAME_2;
    UInt8 containerIndex               = 0;

    /* Fetch the ackNack Container index */
    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );

    tddExpireNodesForContainerIdxConfig2SubFrame2( containerIndex,
            freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
            internalCellIndex );

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodesForContainerIdxConfig2SubFrame2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   dlAckNackSubFrame -
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
STATIC MacRetType tddExpireNodesForContainerIdxConfig2SubFrame2(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex )
/*SPR 11110 fix end */
{
    UInt16 ueIndex = INVALID_UE_ID;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUeContext_p        = PNULL;
    LTE_LIST_NODE * node_p             = PNULL;
    LTE_LIST_NODE * nextNode_p          = PNULL;
    UInt8 txSFNIndex              = 0;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    node_p = 
        getListNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                    [dlAckNackSubFrame].dlExpiryList_p), 0); 
    /** CA-TDD Changes End **/

    while (PNULL != node_p)
    {
        ueIndex = ((TDDHarqTimerExpiryListNode*)node_p)->ueIndex;
        dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];

        if (!dlUeContextInfo_p->pendingDeleteFlag)
        {
            dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;
            /* Tx SFN index for SF 4,5,6,8 */
            /* SPR 18122 Changes Start*/
            txSFNIndex = MODULO((containerIndex + 1),2);
            /* SPR 18122 Changes End*/

            dlDatTxSubframe = SUB_FRAME_6;
            tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

            dlDatTxSubframe = SUB_FRAME_4;
            tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

            dlDatTxSubframe = SUB_FRAME_5;
            tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

            dlDatTxSubframe = SUB_FRAME_8;
            tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);
            dlUeContext_p->tddHarqTimerExpiryNode[containerIndex][dlAckNackSubFrame]
                                                                            = PNULL;

        }    
        nextNode_p = getNextListNode(node_p);
        /** CA-TDD Changes Start **/
        listDeleteNode( &tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]\
                [dlAckNackSubFrame].dlExpiryList_p,(node_p));
        /** CA-TDD Changes End **/
        freeMemPool((void *)node_p);

        node_p = nextNode_p;

        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        /* Resetting bit corresponding to UL sf when PUSCH was sent with DCI0 */
        RESET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(ueIndex,dlAckNackSubFrame);
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}

/*SPR 11110 fix start */
/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig3Subframe2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig3Subframe2(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 dlDatTxSubframe = SUB_FRAME_1;
    UInt8 dlAckNackSubFrame = SUB_FRAME_2;
    UInt8 containerIndex = 0;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireNodesForContainerIdxConfig3SubFrame2( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig3Subframe2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the node in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig3Subframe2(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
    UInt8 dlDatTxSubframe = SUB_FRAME_1;
    UInt8 dlAckNackSubFrame = SUB_FRAME_2;
    UInt8 containerIndex    = 0;

    /* Fetch the ackNack Container index*/
    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );
        
    tddExpireNodesForContainerIdxConfig3SubFrame2( containerIndex,
            freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
            internalCellIndex );

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodesForContainerIdxConfig3SubFrame2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   dlAckNackSubFrame -
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
STATIC MacRetType tddExpireNodesForContainerIdxConfig3SubFrame2(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex )
/*SPR 11110 fix end */
{
    UInt16 ueIndex = INVALID_UE_ID;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUeContext_p        = PNULL;
    LTE_LIST_NODE * node_p             = PNULL;
    LTE_LIST_NODE * nextNode_p         = PNULL;
    UInt8 txSFNIndex              = 0;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    node_p = 
        getListNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                    [dlAckNackSubFrame].dlExpiryList_p), 0); 

    while (PNULL != node_p)
    {
        ueIndex = ((TDDHarqTimerExpiryListNode*)node_p)->ueIndex;
        dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;

        if (dlUeContextInfo_p->pendingDeleteFlag)
        {
            nextNode_p = getNextListNode(node_p);
            listDeleteNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                        [dlAckNackSubFrame].dlExpiryList_p), node_p);
            freeMemPool(node_p);
            node_p = nextNode_p;
            continue;
        }
        /* Tx SFN undex for SF 1,5,6 */
        /* SPR 18122 Changes Start*/
        txSFNIndex = MODULO((containerIndex + 1),2);
        /* SPR 18122 Changes End*/

        dlDatTxSubframe = SUB_FRAME_1;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlDatTxSubframe = SUB_FRAME_5;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlDatTxSubframe = SUB_FRAME_6;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlUeContext_p->tddHarqTimerExpiryNode[containerIndex][dlAckNackSubFrame] 
                                                                        = PNULL;
        nextNode_p = getNextListNode(node_p);
        /** CA-TDD Changes Start **/
        listDeleteNode( &tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]\
                [dlAckNackSubFrame].dlExpiryList_p,(node_p));
        /** CA-TDD Changes End **/
        freeMemPool((void *)node_p);

        node_p = nextNode_p;

        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        /* Resetting bit corresponding to UL sf when PUSCH was sent with DCI0 */
        RESET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(ueIndex,dlAckNackSubFrame);
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}

/*SPR 11110 fix start */
/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig3Subframe3
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig3Subframe3(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 dlDatTxSubframe = SUB_FRAME_7;
    UInt8 dlAckNackSubFrame = SUB_FRAME_3;
    UInt8 containerIndex = 0;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireNodesForContainerIdxConfig3SubFrame3( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
                internalCellIndex );
    }

    return MAC_SUCCESS;
}


/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig3Subframe3
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the node in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig3Subframe3(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        )
{
    UInt8 dlDatTxSubframe = SUB_FRAME_7;
    UInt8 dlAckNackSubFrame = SUB_FRAME_3;
    UInt8 containerIndex    = 0;

    /* Fetch the ackNack Container index*/
    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );

    tddExpireNodesForContainerIdxConfig3SubFrame3( containerIndex,
            freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
            internalCellIndex );

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodesForContainerIdxConfig3SubFrame3
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   dlDatTxSubframe -
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
STATIC MacRetType tddExpireNodesForContainerIdxConfig3SubFrame3(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex )
/*SPR 11110 fix end */
{
    UInt16 ueIndex = INVALID_UE_ID;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUeContext_p        = PNULL;
    LTE_LIST_NODE * node_p             = PNULL;
    LTE_LIST_NODE * nextNode_p          = PNULL;
    UInt8 txSFNIndex              = 0;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    node_p = 
        getListNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                    [dlAckNackSubFrame].dlExpiryList_p), 0); 
    /** CA-TDD Changes End **/

    while (PNULL != node_p)
    {
        ueIndex = ((TDDHarqTimerExpiryListNode*)node_p)->ueIndex;
        dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;

        if (dlUeContextInfo_p->pendingDeleteFlag)
        {
            nextNode_p = getNextListNode(node_p);
            /** CA-TDD Changes Start **/
            listDeleteNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                        [dlAckNackSubFrame].dlExpiryList_p), node_p);
            /** CA-TDD Changes End **/
            freeMemPool(node_p);
            node_p = nextNode_p;
            continue;
        }
        /* Tx SFN index for SF 7 and SF 8 */
        /* SPR 18122 Changes Start*/
        txSFNIndex = MODULO((containerIndex + 1),2);
        /* SPR 18122 Changes End*/

        dlDatTxSubframe = SUB_FRAME_7;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlDatTxSubframe = SUB_FRAME_8;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlUeContext_p->tddHarqTimerExpiryNode[containerIndex][dlAckNackSubFrame] 
                                                                        = PNULL;
        nextNode_p = getNextListNode(node_p);
        /** CA-TDD Changes Start **/
        listDeleteNode( &tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                [dlAckNackSubFrame].dlExpiryList_p,(node_p));
        /** CA-TDD Changes End **/
        freeMemPool((void *)node_p);

        node_p = nextNode_p;

        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        /* Resetting bit corresponding to UL sf when PUSCH was sent with DCI0 */
        RESET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(ueIndex,dlAckNackSubFrame);
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}

/*SPR 11110 fix start */
/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig3Subframe4
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig3Subframe4(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 dlDatTxSubframe = SUB_FRAME_9;
    UInt8 dlAckNackSubFrame = SUB_FRAME_4;
    UInt8 containerIndex = 0;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireNodesForContainerIdxConfig3SubFrame4( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig3Subframe4
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the node in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig3Subframe4(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        )
{
    UInt8 dlDatTxSubframe = SUB_FRAME_9;
    UInt8 dlAckNackSubFrame = SUB_FRAME_4;
    UInt8 containerIndex    = 0;

    /* Fetch the ackNack Container index*/
    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );

    tddExpireNodesForContainerIdxConfig3SubFrame4( containerIndex,
            freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
            internalCellIndex );

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodesForContainerIdxConfig3SubFrame4
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   dlDatTxSubframe
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
STATIC MacRetType tddExpireNodesForContainerIdxConfig3SubFrame4(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex )
/*SPR 11110 fix end */
{
    UInt16 ueIndex = INVALID_UE_ID;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUeContext_p        = PNULL;
    LTE_LIST_NODE * node_p             = PNULL;
    LTE_LIST_NODE * nextNode_p          = PNULL;
    UInt8 txSFNIndex  = 0;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    node_p = 
        getListNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                    [dlAckNackSubFrame].dlExpiryList_p), 0); 
    /** CA-TDD Changes End **/

    while (PNULL != node_p)
    {
        ueIndex = ((TDDHarqTimerExpiryListNode*)node_p)->ueIndex;
        dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;

        if (dlUeContextInfo_p->pendingDeleteFlag)
        {
            nextNode_p = getNextListNode(node_p);
            /** CA-TDD Changes Start **/
            listDeleteNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                        [dlAckNackSubFrame].dlExpiryList_p), node_p);
            /** CA-TDD Changes End **/
            freeMemPool(node_p);
            node_p = nextNode_p;
            continue;
        }

        dlDatTxSubframe = SUB_FRAME_9;
        /* SPR 18122 Changes Start*/
        txSFNIndex = MODULO((containerIndex + 1),2);
        /* SPR 18122 Changes End*/
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlDatTxSubframe = SUB_FRAME_0;
        txSFNIndex = containerIndex;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlUeContext_p->tddHarqTimerExpiryNode[containerIndex][dlAckNackSubFrame] = PNULL;

        nextNode_p = getNextListNode(node_p);
        /** CA-TDD Changes Start **/
        listDeleteNode( &tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                [dlAckNackSubFrame].dlExpiryList_p,(node_p));
        /** CA-TDD Changes End **/
        freeMemPool((void *)node_p);

        node_p = nextNode_p;

        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        /* Resetting bit corresponding to UL sf when PUSCH was sent with DCI0 */
        RESET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(ueIndex,dlAckNackSubFrame);
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    return MAC_SUCCESS;
}

/*SPR 11110 fix start */
/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig4Subframe2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig4Subframe2(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 dlDatTxSubframe = SUB_FRAME_0;
    UInt8 dlAckNackSubFrame = SUB_FRAME_2;
    UInt8 containerIndex = 0;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireNodesForContainerIdxConfig4SubFrame2( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig4Subframe2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the node in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig4Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        )
{
    UInt8 dlDatTxSubframe = SUB_FRAME_0;
    UInt8 dlAckNackSubFrame = SUB_FRAME_2;
    UInt8 containerIndex    = 0;
    
    /* Fetch the ackNack Container index*/
    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );
    
    tddExpireNodesForContainerIdxConfig4SubFrame2( containerIndex,
            freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
            internalCellIndex );

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodesForContainerIdxConfig4SubFrame2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   dlDatTxSubframe -
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
STATIC MacRetType tddExpireNodesForContainerIdxConfig4SubFrame2(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex )
/*SPR 11110 fix end */
{
    UInt16 ueIndex = INVALID_UE_ID;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUeContext_p        = PNULL;
    LTE_LIST_NODE * node_p             = PNULL;
    LTE_LIST_NODE * nextNode_p          = PNULL;
    UInt8 txSFNIndex              = 0;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    node_p = 
        getListNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                    [dlAckNackSubFrame].dlExpiryList_p), 0); 

    while (PNULL != node_p)
    {
        ueIndex = ((TDDHarqTimerExpiryListNode*)node_p)->ueIndex;
        dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;

        if (dlUeContextInfo_p->pendingDeleteFlag)
        {
            nextNode_p = getNextListNode(node_p);
            /** CA-TDD Changes Start **/
            listDeleteNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                        [dlAckNackSubFrame].dlExpiryList_p), node_p);
            /** CA-TDD Changes End **/
            freeMemPool(node_p);
            node_p = nextNode_p;
            continue;
        }
        /* Tx sfn index for 0,1,4,5 */
        /* SPR 18122 Changes Start*/
        txSFNIndex = MODULO((containerIndex + 1),2);
        /* SPR 18122 Changes End*/

        dlDatTxSubframe = SUB_FRAME_1;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlDatTxSubframe = SUB_FRAME_0;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlDatTxSubframe = SUB_FRAME_4;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlDatTxSubframe = SUB_FRAME_5;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlUeContext_p->tddHarqTimerExpiryNode[containerIndex][dlAckNackSubFrame] = PNULL;
        nextNode_p = getNextListNode(node_p);
        /** CA-TDD Changes Start **/
        listDeleteNode( &tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                [dlAckNackSubFrame].dlExpiryList_p,(node_p));
        /** CA-TDD Changes End **/
        freeMemPool((void *)node_p);

        node_p = nextNode_p;

        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        /* Resetting bit corresponding to UL sf when PUSCH was sent with DCI0 */
        RESET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(ueIndex,dlAckNackSubFrame);
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");


    return MAC_SUCCESS;
}

/*SPR 11110 fix start */
/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig4Subframe3
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - cell Id
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig4Subframe3(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 containerIndex = 0;
    UInt8 dlDatTxSubframe   = SUB_FRAME_6;
    UInt8 dlAckNackSubFrame = SUB_FRAME_3;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireNodesForContainerIdxConfig4SubFrame3( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig4Subframe3
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the node in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig4Subframe3(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
    UInt8 dlDatTxSubframe = SUB_FRAME_6;
    UInt8 dlAckNackSubFrame = SUB_FRAME_3;
    UInt8 containerIndex    = 0;
    
    /* Fetch the ackNack Container index*/
    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );
        
    tddExpireNodesForContainerIdxConfig4SubFrame3( containerIndex,
            freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
            internalCellIndex );

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodesForContainerIdxConfig4SubFrame3
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   dlDatTxSubframe -
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
STATIC MacRetType tddExpireNodesForContainerIdxConfig4SubFrame3(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex )
/*SPR 11110 fix end */
{
    UInt16 ueIndex = INVALID_UE_ID;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUeContext_p        = PNULL;
    LTE_LIST_NODE * node_p             = PNULL;
    LTE_LIST_NODE * nextNode_p          = PNULL;
    UInt8 txSFNIndex              = 0;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    node_p = 
        getListNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex][dlAckNackSubFrame].dlExpiryList_p), 0); 

    while (PNULL != node_p)
    {
        ueIndex = ((TDDHarqTimerExpiryListNode*)node_p)->ueIndex;
        dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;

        if (dlUeContextInfo_p->pendingDeleteFlag)
        {
            nextNode_p = getNextListNode(node_p);
            /** CA-TDD Changes Start **/
            listDeleteNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                        [dlAckNackSubFrame].dlExpiryList_p), node_p);
            /** CA-TDD Changes End **/
            freeMemPool(node_p);
            node_p = nextNode_p;
            continue;
        }
        /* Tx SFN Index for SF 6,7,8,9 */
        /* SPR 18122 Changes Start*/
        txSFNIndex = MODULO((containerIndex + 1),2);
        /* SPR 18122 Changes End*/

        dlDatTxSubframe = SUB_FRAME_6;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlDatTxSubframe = SUB_FRAME_7;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlDatTxSubframe = SUB_FRAME_8;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlDatTxSubframe = SUB_FRAME_9;
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlUeContext_p->tddHarqTimerExpiryNode[containerIndex][dlAckNackSubFrame] = PNULL;
        nextNode_p = getNextListNode(node_p);
        /** CA-TDD Changes Start **/
        listDeleteNode( &tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                [dlAckNackSubFrame].dlExpiryList_p,(node_p));
        /** CA-TDD Changes End **/
        freeMemPool((void *)node_p);

        node_p = nextNode_p;

        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        /* Resetting bit corresponding to UL sf when PUSCH was sent with DCI0 */
        RESET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(ueIndex,dlAckNackSubFrame);
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    return MAC_SUCCESS;
}

/*SPR 11110 fix start */
/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig5Subframe2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig5Subframe2(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 containerIndex = 0;
    UInt8 dlDatTxSubframe = SUB_FRAME_0;
    UInt8 dlAckNackSubFrame = SUB_FRAME_2;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++ )
    {
        tddExpireNodesForContainerIdxConfig5SubFrame2( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame, 
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig5Subframe2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the node in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig5Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        )
{
    UInt8 containerIndex  = 0;
    UInt8 dlDatTxSubframe = SUB_FRAME_0;
    UInt8 dlAckNackSubFrame = SUB_FRAME_2;

    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex  );

    tddExpireNodesForContainerIdxConfig5SubFrame2( containerIndex,
            freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame, 
            internalCellIndex );

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodesForContainerIdxConfig5SubFrame2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
STATIC MacRetType tddExpireNodesForContainerIdxConfig5SubFrame2(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex )
/*SPR 11110 fix end */
{
    UInt16 ueIndex = INVALID_UE_ID;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUeContext_p        = PNULL;
    LTE_LIST_NODE * node_p             = PNULL;
    LTE_LIST_NODE * nextNode_p          = PNULL;
    UInt8 txSFNIndex              = 0;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    node_p = 
        getListNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                    [dlAckNackSubFrame].dlExpiryList_p), 0); 
    /** CA-TDD Changes End **/

    while (PNULL != node_p)
    {
        ueIndex = ((TDDHarqTimerExpiryListNode*)node_p)->ueIndex;
        dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;

        if (dlUeContextInfo_p->pendingDeleteFlag)
        {
            nextNode_p = getNextListNode(node_p);
            /** CA-TDD Changes Start **/
            listDeleteNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                        [dlAckNackSubFrame].dlExpiryList_p), node_p);
            /** CA-TDD Changes End **/
            freeMemPool(node_p);
            node_p = nextNode_p;
            continue;
        }

        dlDatTxSubframe = SUB_FRAME_9;
        if (( dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame])&
                (bitmask[dlDatTxSubframe]))
        {
            dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame] &=
                (resetBitmask[dlDatTxSubframe]) ;

            txSFNIndex = containerIndex;

            if (dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] != INVALID_HARQ_ID)
            {
                /* + CA_TDD_HARQ_CHANGES */
                tddExpireNodeFromHarqTimerListForConfig5( dlUeContextInfo_p,
                        dlDatTxSubframe,
                        ueIndex,
                        txSFNIndex
                        ,freeHarqOnly
                        ,internalCellIndex
                        );
                  /* - CA_TDD_HARQ_CHANGES */
                dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] = INVALID_HARQ_ID ;
            }
            else
            {
                ltePanic("[%s]:Invalid HARQ process ID\n",__func__);
            }
        }

        dlDatTxSubframe = SUB_FRAME_0;
        if (( dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame])&
                (bitmask[dlDatTxSubframe]))
        {
            dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame] &= 
                (resetBitmask[dlDatTxSubframe]) ;

            /* SPR 18122 Changes Start*/
            txSFNIndex = MODULO((containerIndex + 1),2);
            /* SPR 18122 Changes End*/

            if (dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] != INVALID_HARQ_ID)
            {
                /* + CA_TDD_HARQ_CHANGES */
                tddExpireNodeFromHarqTimerListForConfig5( dlUeContextInfo_p,
                        dlDatTxSubframe,
                        ueIndex,
                        txSFNIndex
                        ,freeHarqOnly,
                        internalCellIndex );
                /* - CA_TDD_HARQ_CHANGES */

                dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] = INVALID_HARQ_ID ;
            }
            else
            {
                ltePanic("[%s]:Invalid HARQ process ID\n",__func__);
            }
        }

        dlDatTxSubframe = SUB_FRAME_1;
        if (( dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame])&
                (bitmask[dlDatTxSubframe]))
        {
            dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame] &=
                (resetBitmask[dlDatTxSubframe]) ;

            /* SPR 18122 Changes Start*/
            txSFNIndex = MODULO((containerIndex + 1),2);
            /* SPR 18122 Changes End*/

            if (dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] != INVALID_HARQ_ID)
            {
                /* + CA_TDD_HARQ_CHANGES */
                tddExpireNodeFromHarqTimerListForConfig5( dlUeContextInfo_p,
                        dlDatTxSubframe,
                        ueIndex,
                        txSFNIndex
                        ,freeHarqOnly
                        ,internalCellIndex
                        );
                /* - CA_TDD_HARQ_CHANGES */


                dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] = INVALID_HARQ_ID ;
            }
            else
            {
                ltePanic("[%s]:Invalid HARQ process ID\n",__func__);
            }
        }

        dlDatTxSubframe = SUB_FRAME_3;
        if (( dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame])&
                (bitmask[dlDatTxSubframe]))
        {
            dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame] &=
                (resetBitmask[dlDatTxSubframe]) ;
            /* SPR 18122 Changes Start*/
            txSFNIndex = MODULO((containerIndex + 1),2);
            /* SPR 18122 Changes End*/

            if (dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] != INVALID_HARQ_ID)
            {
                /* + CA_TDD_HARQ_CHANGES */
                tddExpireNodeFromHarqTimerListForConfig5( dlUeContextInfo_p,
                        dlDatTxSubframe,
                        ueIndex,
                        txSFNIndex
                        ,freeHarqOnly,
                        internalCellIndex );
                /* - CA_TDD_HARQ_CHANGES */

                dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] = INVALID_HARQ_ID ;
            }
            else
            {
                ltePanic("[%s]:Invalid HARQ process ID\n",__func__);
            }
        }
        dlDatTxSubframe = SUB_FRAME_4;
        if (( dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame])&
                (bitmask[dlDatTxSubframe]))
        {
            dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame] &=
                (resetBitmask[dlDatTxSubframe]) ;

            /* SPR 18122 Changes Start*/
            txSFNIndex = MODULO((containerIndex + 1),2);
            /* SPR 18122 Changes End*/

            if (dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] != INVALID_HARQ_ID)
            {
                /* + CA_TDD_HARQ_CHANGES */
                tddExpireNodeFromHarqTimerListForConfig5( dlUeContextInfo_p,
                        dlDatTxSubframe,
                        ueIndex,
                        txSFNIndex
                        ,freeHarqOnly,
                        internalCellIndex );
                /* - CA_TDD_HARQ_CHANGES */

                dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] = INVALID_HARQ_ID ;
            }
            else
            {
                ltePanic("[%s]:Invalid HARQ process ID\n",__func__);
            }
        }
        dlDatTxSubframe = SUB_FRAME_5;
        if (( dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame])&
                (bitmask[dlDatTxSubframe]))
        {
            dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame] &=
                (resetBitmask[dlDatTxSubframe]) ;

            /* SPR 18122 Changes Start*/
            txSFNIndex = MODULO((containerIndex + 1),2);
            /* SPR 18122 Changes End*/

            if (dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] != INVALID_HARQ_ID)
            {
                /* + CA_TDD_HARQ_CHANGES */
                tddExpireNodeFromHarqTimerListForConfig5( dlUeContextInfo_p,
                        dlDatTxSubframe,
                        ueIndex,
                        txSFNIndex
                        ,freeHarqOnly
                        ,internalCellIndex
                        );
                /* - CA_TDD_HARQ_CHANGES */

                dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] = INVALID_HARQ_ID ;
            }
            else
            {
                ltePanic("[%s]:Invalid HARQ process ID\n",__func__);
            }
        }
        dlDatTxSubframe = SUB_FRAME_6;
        if (( dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame])&
                (bitmask[dlDatTxSubframe]))
        {
            dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame] &=
                (resetBitmask[dlDatTxSubframe]) ;

            /* SPR 18122 Changes Start*/
            txSFNIndex = MODULO((containerIndex + 1),2);
            /* SPR 18122 Changes End*/

            if (dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] != INVALID_HARQ_ID)
            {
                /* + CA_TDD_HARQ_CHANGES */
                tddExpireNodeFromHarqTimerListForConfig5( dlUeContextInfo_p,
                        dlDatTxSubframe,
                        ueIndex,
                        txSFNIndex
                        ,freeHarqOnly,
                        internalCellIndex );
                /* - CA_TDD_HARQ_CHANGES */

                dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] = INVALID_HARQ_ID ;
            }
            else
            {
                ltePanic("[%s]:Invalid HARQ process ID\n",__func__);
            }
        }
        dlDatTxSubframe = SUB_FRAME_7;
        if (( dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame])&
                (bitmask[dlDatTxSubframe]))
        {
            dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame] &=
                (resetBitmask[dlDatTxSubframe]) ;

            /* SPR 18122 Changes Start*/
            txSFNIndex = MODULO((containerIndex + 1),2);
            /* SPR 18122 Changes End*/

            if (dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] != INVALID_HARQ_ID)
            {
                /* + CA_TDD_HARQ_CHANGES */
                tddExpireNodeFromHarqTimerListForConfig5( dlUeContextInfo_p,
                        dlDatTxSubframe,
                        ueIndex,
                        txSFNIndex
                        ,freeHarqOnly
                        ,internalCellIndex
                        );
                /* - CA_TDD_HARQ_CHANGES */

                dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] = INVALID_HARQ_ID ;
            }
            else
            {
                ltePanic("[%s]:Invalid HARQ process ID\n",__func__);
            }
        }
        dlDatTxSubframe = SUB_FRAME_8;
        if (( dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame])&
                (bitmask[dlDatTxSubframe]))
        {
            dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame] &=
                (resetBitmask[dlDatTxSubframe]) ;

            /* SPR 18122 Changes Start*/
            txSFNIndex = MODULO((containerIndex + 1),2);
            /* SPR 18122 Changes End*/

            if (dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] != INVALID_HARQ_ID)
            {
                /* + CA_TDD_HARQ_CHANGES */
                tddExpireNodeFromHarqTimerListForConfig5( dlUeContextInfo_p,
                        dlDatTxSubframe,
                        ueIndex,
                        txSFNIndex
                        ,freeHarqOnly, 
                        internalCellIndex );
                /* - CA_TDD_HARQ_CHANGES */
                dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] = INVALID_HARQ_ID ;
            }
            else
            {
                ltePanic("[%s]:Invalid HARQ process ID\n",__func__);
            }
        }

        dlUeContext_p->tddHarqTimerExpiryNode[containerIndex][dlAckNackSubFrame] = PNULL;
        nextNode_p = getNextListNode(node_p);
        /** CA-TDD Changes Start **/
        listDeleteNode( &tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                [dlAckNackSubFrame].dlExpiryList_p,(node_p));
        /** CA-TDD Changes End **/
        freeMemPool((void *)node_p);

        node_p = nextNode_p;
    }


    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}

/*SPR 11110 fix start */
/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig6Subframe7
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig6Subframe7(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 containerIndex = 0;
    UInt8 dlDatTxSubframe = SUB_FRAME_0;
    UInt8 dlAckNackSubFrame = SUB_FRAME_7;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireNodesForContainerIdxConfig6SubFrames78( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame, 
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig6Subframe7
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig6Subframe7(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        )
{
    UInt8 containerIndex  = 0;
    UInt8 dlDatTxSubframe = SUB_FRAME_0;
    UInt8 dlAckNackSubFrame = SUB_FRAME_7;

    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );
        
    tddExpireNodesForContainerIdxConfig6SubFrames78( containerIndex,
            freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame, 
            internalCellIndex );

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig6Subframe8
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig6Subframe8(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 containerIndex = 0;
    UInt8 dlDatTxSubframe = SUB_FRAME_1;
    UInt8 dlAckNackSubFrame = SUB_FRAME_8;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireNodesForContainerIdxConfig6SubFrames78( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig6Subframe8
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig6Subframe8(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
    UInt8 containerIndex    = 0;
    UInt8 dlDatTxSubframe = SUB_FRAME_1;
    UInt8 dlAckNackSubFrame = SUB_FRAME_8;

    /* Fetch the ackNack Container index*/
    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );

    tddExpireNodesForContainerIdxConfig6SubFrames78( containerIndex,
            freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame, 
            internalCellIndex );

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodesForContainerIdxConfig6SubFrames78
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   dlDatTxSubframe -
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
STATIC MacRetType tddExpireNodesForContainerIdxConfig6SubFrames78(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex )
/*SPR 11110 fix end */
{
    UInt16 ueIndex = INVALID_UE_ID;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUeContext_p        = PNULL;
    LTE_LIST_NODE * node_p             = PNULL;
    LTE_LIST_NODE * nextNode_p          = PNULL;
    UInt8 txSFNIndex              = 0;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    node_p = 
        getListNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                    [dlAckNackSubFrame].dlExpiryList_p), 0); 

    while (PNULL != node_p)
    {
        ueIndex = ((TDDHarqTimerExpiryListNode*)node_p)->ueIndex;
        dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;

        if (dlUeContextInfo_p->pendingDeleteFlag)
        {
            nextNode_p = getNextListNode(node_p);
            /** CA-TDD Changes Start **/
            listDeleteNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                        [dlAckNackSubFrame].dlExpiryList_p), node_p);
            /** CA-TDD Changes End **/
            freeMemPool(node_p);
            node_p = nextNode_p;
            continue;
        }
        txSFNIndex = containerIndex;

        /* + SPR_13754 */
        /* dlDatTxSubframe = SUB_FRAME_0; */
        /* - SPR_13754 */

        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlUeContext_p->tddHarqTimerExpiryNode[containerIndex][dlAckNackSubFrame] = PNULL;
        nextNode_p = getNextListNode(node_p);
        /** CA-TDD Changes Start **/
        listDeleteNode( &tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                [dlAckNackSubFrame].dlExpiryList_p,(node_p));
        /** CA-TDD Changes End **/
        freeMemPool((void *)node_p);

        node_p = nextNode_p;
        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        /* Resetting bit corresponding to UL sf when PUSCH was sent with DCI0 */
        RESET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(ueIndex,dlAckNackSubFrame);
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    return MAC_SUCCESS;
}

/*SPR 11110 fix start */
/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig6Subframe2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the node in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig6Subframe2(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 containerIndex = 0;
    UInt8 dlDatTxSubframe = SUB_FRAME_5;
    UInt8 dlAckNackSubFrame = SUB_FRAME_2;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireNodesForContainerIdxConfig6SubFrames234( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig6Subframe2
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig6Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        )
{
    UInt8 containerIndex  = 0;
    UInt8 dlDatTxSubframe = SUB_FRAME_5;
    UInt8 dlAckNackSubFrame = SUB_FRAME_2;

    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );
        
    tddExpireNodesForContainerIdxConfig6SubFrames234( containerIndex,
            freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
            internalCellIndex );

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig6Subframe3
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig6Subframe3(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 containerIndex = 0;
    UInt8 dlDatTxSubframe = SUB_FRAME_6;
    UInt8 dlAckNackSubFrame = SUB_FRAME_3;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireNodesForContainerIdxConfig6SubFrames234( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig6Subframe3
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig6Subframe3(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
    UInt8 containerIndex    = 0;
    UInt8 dlDatTxSubframe = SUB_FRAME_6;
    UInt8 dlAckNackSubFrame = SUB_FRAME_3;

    /* Fetch the ackNack Container index*/
    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );

    tddExpireNodesForContainerIdxConfig6SubFrames234( containerIndex,
            freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame, 
            internalCellIndex );

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireAllNodeForConfig6Subframe4
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType  tddExpireAllNodeForConfig6Subframe4(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
 /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(sfn)
    UInt8 containerIndex = 0;
    UInt8 dlDatTxSubframe = SUB_FRAME_9;
    UInt8 dlAckNackSubFrame = SUB_FRAME_4;

    for( containerIndex = 0; containerIndex < MAX_TDD_HARQ_CONT;
            containerIndex++)
    {
        tddExpireNodesForContainerIdxConfig6SubFrames234( containerIndex,
                freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
                internalCellIndex );
    }

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  tddExpireNodeForConfig6Subframe4
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
MacRetType tddExpireNodeForConfig6Subframe4(UInt32 sfn,
        UInt8 freeHarqOnly, InternalCellIndex internalCellIndex )
{
    UInt8 containerIndex    = 0;
    UInt8 dlDatTxSubframe = SUB_FRAME_9;
    UInt8 dlAckNackSubFrame = SUB_FRAME_4;

    /* Fetch the ackNack Container index*/
    getContainerIdxForTimerExpiry( dlAckNackSubFrame,
            sfn, &containerIndex );

    tddExpireNodesForContainerIdxConfig6SubFrames234( containerIndex,
            freeHarqOnly, dlDatTxSubframe,  dlAckNackSubFrame,
            internalCellIndex );

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : tddExpireNodesForContainerIdxConfig6SubFrames234
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   dlDatTxSubframe - 
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function will remove all the noda in the HARQ Timer
 *                   corresponding to particular sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.
 *****************************************************************************/
STATIC MacRetType tddExpireNodesForContainerIdxConfig6SubFrames234(
        UInt32 containerIndex,
        UInt8 freeHarqOnly, UInt8 dlDatTxSubframe,
        UInt8 dlAckNackSubFrame, InternalCellIndex internalCellIndex )
/*SPR 11110 fix end */
{
    UInt16 ueIndex = INVALID_UE_ID;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUeContext_p        = PNULL;
    LTE_LIST_NODE * node_p             = PNULL;
    LTE_LIST_NODE * nextNode_p          = PNULL;
    UInt8 txSFNIndex              = 0;

    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    node_p = 
        getListNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                    [dlAckNackSubFrame].dlExpiryList_p), 0); 

    while (PNULL != node_p)
    {
        ueIndex = ((TDDHarqTimerExpiryListNode*)node_p)->ueIndex;
        dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
        dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;

        if (dlUeContextInfo_p->pendingDeleteFlag)
        {
            nextNode_p = getNextListNode(node_p);
            /** CA-TDD Changes Start **/
            listDeleteNode(&(tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                        [dlAckNackSubFrame].dlExpiryList_p), node_p);
            /** CA-TDD Changes End **/
            freeMemPool(node_p);
            node_p = nextNode_p;
            continue;
        }

        /* SPR 18122 Changes Start*/
        txSFNIndex = MODULO((containerIndex + 1),2);
        /* SPR 18122 Changes End*/
        tddExpireHarqNode(ueIndex,containerIndex,
                          dlAckNackSubFrame, dlDatTxSubframe,
                          txSFNIndex,freeHarqOnly,internalCellIndex);

        dlUeContext_p->tddHarqTimerExpiryNode[containerIndex][dlAckNackSubFrame] 
                                                                        = PNULL;
        nextNode_p = getNextListNode(node_p);
        /** CA-TDD Changes Start **/
        listDeleteNode( &tddDlHarqTimerEntityList_g[internalCellIndex][containerIndex]
                [dlAckNackSubFrame].dlExpiryList_p,(node_p));
        /** CA-TDD Changes End **/
        freeMemPool((void *)node_p);

        node_p = nextNode_p;

        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
        /* Resetting bit corresponding to UL sf when PUSCH was sent with DCI0 */
        RESET_BIT_ON_UL_NEW_OR_ADAPTIVE_TRANSMISSION(ueIndex,dlAckNackSubFrame);
        /* SPR 19679: TDD HARQ Multiplexing Changes End */
    }

    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

    return MAC_SUCCESS;
}

/*****************************************************************************
 * Function Name  :  unexpectedEvent
 * Inputs         :  sfn - current system frame number is used to obtain
 *                         the ack/nack & harq container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs.
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_FAILURE
 * Description    :  This is a dummy function.
 *****************************************************************************/
MacRetType unexpectedEvent(UInt32 sfn
        ,UInt8 freeHarqOnly
         /** CA-TDD Changes Start **/
        ,InternalCellIndex internalCellIndex
        /** CA-TDD Changes End **/
        )
{
    /* +- SPR 17777 */
    LOG_MAC_MSG(MAC_INVALID_HARQ,LOGERROR,MAC_DL_HARQ,\
            getCurrentTick(),
            sfn,
            INVALID_HARQ_ID,freeHarqOnly,internalCellIndex,
            DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    /* Have to put logs here*/
    return MAC_FAILURE;
}

#endif

#ifdef TDD_CONFIG
/*****************************************************************************
 * Function Name  :  eicicUpdateHarqSystemframeNumber
 * Inputs         :  subFrameNumber- Current subframe number
 *                   ackNackNode_p-  pointer to a node from tddAckNackQueue      
 * Outputs        :  None
 * Returns        :  None
 * Description    :  Updates Harq system frame number for EICIC
 *****************************************************************************/
/*Complexity Start*/
void eicicUpdateHarqSystemframeNumber(
        UInt32 subFrameNumber,
        tddAckNackQueueNode *ackNackNode_p)
{
    if (ackNackNode_p->dlSubframeNum > subFrameNumber)
    {
        eicicHarqSystemframeNumber_g = eicicHarqSystemframeNumber_g - 1;
    }
}
/*Complexity End*/

/*****************************************************************************
 * Function Name  :  processDLAckNackQueue
 * Inputs         :  
 *                   globalTick
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  MAC_SUCCESS/MAC_FAILURE
 * Description    :  This function reads from the AckNackQueue and does the 
 *                   requisites in accordance whether ACk has been received or 
 *                   NACK has been received.
 *****************************************************************************/
                                               /* SPR 15909 fix start */
 /* +- SPR 17777 */
MacRetType processDLAckNackQueue(tickType_t globalTick, 
                                               /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex )
{
    tddAckNackQueueNode  *ackNackNode_p = PNULL;
    UInt32 ackNackQueueCount            = 0;
    UInt16 ueIndex                      = 0;
    DLUEContextInfo* dlUeContextInfo_p  = PNULL;
    DLUEContext*  dlUeContext_p         = PNULL;        
    DLHARQProcess* dlHarqProcess_p      = PNULL;
    DLHARQEntity*   dlHarqEntity_p      = PNULL;
    UInt32 subFrameNumber               = 0;
    UInt8 harqProcessId                 = 0;
    UInt8 indexSfn                      = 0;
    // added for DL-MIMO in TDD
    UInt8 value1, value2 = 0;
    UInt8 isHarqOnPucch = FALSE;
    /* + CA_TDD_HARQ_CHANGES */
    UInt8 dlHarqFlag[MAX_NUM_CELL]       = {0}; 
    UInt16  dlAckNackFlag = 0; 
    UInt8   sCellCount  = 0;
    DLUESCellContext* dlUeScellContext_p  = PNULL;
    InternalCellIndex ueServCellIndex = 0;
    InternalCellIndex ueInternalCellIndex = 0;
    TransmissonMode     transmissionMode = INVALID_TX_MODE;
    UInt8 bitMaskForAperiodicReport = 0;
    UInt8 harqProcessIdInAckNackNode[MAX_NUM_CELL] = {0,};
    UInt8   nackFlag[MAX_NUM_CELL] = {FALSE,};
    UInt8   arrIndex = 0;
    /* SPR 15909 fix start */
#ifdef KPI_STATS    
    tickType_t recvTTI = 0; /*SPR 18490 +-*/
#endif    
    /* SPR 15909 fix end */
    UInt8 modeType = 0;
    /* 
     * For CA UE both retrans cell info shall 
     * be enqueued into retrans Queue of primary
     * cell.
     */
    DLRetxQueueNode  retxNode; 
    /* - CA_TDD_HARQ_CHANGES */
#ifdef MAC_AUT_TEST
    UInt8 numOfNodes = 0;
#endif     
    LOG_UT(MAC_FUNCTION_ENTRY,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    /** CA-TDD Changes Start:06 **/
    ackNackQueueCount = sQueueCount(&(tddAckNackQueue_g[internalCellIndex]));
    /** CA-TDD Changes End:06 **/

    while(ackNackQueueCount)
    {
        LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,"[%s]ackNackQueueCount %d ",
                __func__,ackNackQueueCount);
        /* + CA_TDD_HARQ_CHANGES */
        bitMaskForAperiodicReport = 0;
        /* - CA_TDD_HARQ_CHANGES */

        /** CA-TDD Changes Start:06 **/
        ackNackNode_p = (tddAckNackQueueNode*)popNode(&(tddAckNackQueue_g[internalCellIndex]));
        /** CA-TDD Changes End:06 **/
        if(PNULL != ackNackNode_p)
        {
            ueIndex        = ackNackNode_p->ueIndex;
            dlAckNackFlag  = ackNackNode_p->operationType;
            subFrameNumber = ackNackNode_p->packetSubFrame;
            indexSfn       = ackNackNode_p->indexSfn;
            modeType       = ackNackNode_p->modeType;
            /* + Coverity_26433 */
            /* SPR 21029 fix start */
            dlHarqFlag[PCELL_SERV_CELL_IX]  = RESET;
            /* SPR 21029 fix end */
            /* SPR 21997 Fix Start */
            dlHarqFlag[START_SCELL_INDEX] = RESET;
            /* SPR 21997 Fix End */
#ifdef KPI_STATS
            recvTTI        = ackNackNode_p->recvTTI;
#endif
            harqProcessIdInAckNackNode[PCELL_SERV_CELL_IX] = 
                ackNackNode_p->harqProcessId[PCELL_SERV_CELL_IX]; 
            harqProcessIdInAckNackNode[START_SCELL_INDEX]  = 
                ackNackNode_p->harqProcessId[START_SCELL_INDEX]; 
            /* - Coverity_26433 */
            LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,"[%s]For ueIndex  %d"\
                    " dlAckNackFlag %d  subFrameNumber %d"\
                    ,__func__,ueIndex,dlAckNackFlag, ackNackNode_p->packetSubFrame);
            dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
            /* EICIC - TDD +*/

            /* Save the current recevied UL harq subframe number and calculate
               the system frame number accordingly*/

            eicicHarqSubframeNumber_g = subFrameNumber;

            UPDATE_SYSTEM_FRAME_AND_SUB_FRAME_ON_WHICH_HARQ_RECEIVED(internalCellIndex)

                /* If subframe number for which ack is recevied is greater than current subframe 
                   number then it means system frame is the last sys frame , so reduce the 
                   system frame by 1*/
                /*Complexity Start */
                eicicUpdateHarqSystemframeNumber(subFrameNumber,ackNackNode_p);
            /*Complexity End */

            /* Update the Subframe number with subframe for which ACK is recevied 
               this shall be used to calculate if that subframe was ABS or not */	

            eicicHarqSubframeNumber_g = ackNackNode_p->dlSubframeNum;

            /* EICIC TDD -*/
            if (!dlUeContextInfo_p->pendingDeleteFlag)
            {
                dlUeContext_p  = dlUeContextInfo_p->dlUEContext_p;
                /* + CA_TDD_HARQ_CHANGES */
                if(dlUeContext_p->internalCellIndex != internalCellIndex)
                {
                    FREE_MEM_NODE_ACK_NACK_CIRC_Q(ackNackNode_p);
#ifndef DL_UL_SPLIT
                    ackNackQueueCount--;
#endif
                    continue;
                }

                if (!dlUeContext_p->tddHarqTimerExpiryNode[indexSfn][subFrameNumber])
                {    
                    /* + Coverity_69535 */
                    SET_HARQ_RETX_NODE(retxNode); 
                    /* - Coverity_69535 */

                    sCellCount = dlUeContext_p->scellCount;

                    /* SPR 15958 Start */ 
                    handleDLTimingAdvance(dlUeContext_p,internalCellIndex);
                    /* SPR 15958 End */ 
                    /* Update isHarqOnPucch to indicate whether the HARQ is
                     * received on PUCCH or not */
                    CHECK_IS_HARQ_BLER_RECEIVED_ON_PUCCH(ueIndex, subFrameNumber, isHarqOnPucch);

                    if (sCellCount && (HARQ_CHANNEL_SELECTION_MODE == modeType))
                    {
                        /* + SPR 13927 Changes */
                        if (HARQ_MULTIPLEX_ONE == 
                                globalDlAssociationSet_gp[internalCellIndex][subFrameNumber].count)
                            /* - SPR 13927 Changes */
                        {
                            macDecodeDLAckNackFlag( dlUeContext_p,
                                    dlAckNackFlag, dlHarqFlag,isHarqOnPucch);
                        }    
                        else
                        {
                            /* First nibble of dlAckNackFlag contains response of primary cell
                             * and second nibble contains secondary cell response.
                             * Also,this is channel selection case so both CWs will have 
                             * same ACK-NACK value.
                             */ 
                            dlHarqFlag[PCELL_SERV_CELL_IX] |= 
                                (dlAckNackFlag & 0x0F) |((dlAckNackFlag & 0x0F) << 4);
                            dlHarqFlag[START_SCELL_INDEX] |= 
                                ((dlAckNackFlag & 0xF0) >> 4) | ((dlAckNackFlag & 0xF0));

                        }    

                    }    
                    else
                    {
                        /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                        /* In case of UE has only primary cell or special
                         * bundling  or for Non CA UE
                         */
                        dlHarqFlag[PCELL_SERV_CELL_IX] |= dlAckNackFlag & 0x00FF;

                        /* In case Scell is added and reported format 
                         * is special bundling */
                        if(sCellCount)
                        {
                            if(HARQ_SPECIAL_BUNDLING_MODE == modeType)
                            {   
                                dlHarqFlag[START_SCELL_INDEX] |= dlAckNackFlag & 0x00FF;
                            }  
                            else 
                            {
                                dlHarqFlag[START_SCELL_INDEX] |=  0x11;
                            }
                        }
                        /* SPR 19679: TDD HARQ Multiplexing Changes End */
                    }   
#ifdef MAC_AUT_TEST
                    macAutTddDlHarqFlagVer_g[numOfNodes][0] = dlHarqFlag[0]; 
                    macAutTddDlHarqFlagVer_g[numOfNodes][1] = dlHarqFlag[1];
                    numOfNodes++;
#endif                    
                    for(ueServCellIndex = 0; ueServCellIndex <= sCellCount ;
                            ueServCellIndex++)
                    {
                        retxNode.ueIndex = ueIndex;
                        if(!ueServCellIndex)
                        {
                            dlHarqEntity_p= dlUeContext_p->dlHarqContext_p;
                            ueInternalCellIndex = internalCellIndex;
                            transmissionMode = dlUeContext_p->\
                                               dlMIMOInfo.transmissionMode;
                            retxNode.harqProcessId =
                                harqProcessIdInAckNackNode[ueServCellIndex];
                            /* + SPR 20690 */
                            retxNode.schUsed = ackNackNode_p->schUsed;
                            /* - SPR 20690 */
                        }
                        else
                        {
                            if(PNULL ==
                                    dlUeContext_p->dlUeScellContext_p[ueServCellIndex])
                            {
                                continue;
                            }

                            dlUeScellContext_p =
                                dlUeContext_p->dlUeScellContext_p[ueServCellIndex];
                            dlHarqEntity_p =
                                dlUeScellContext_p->ScelldlHarqContext_p;
                            ueInternalCellIndex =
                                dlUeScellContext_p->internalCellIndex;

                            transmissionMode = dlUeScellContext_p->\
                                               ScelldlMIMOInfo.transmissionMode;

                            arrIndex  = ueServCellIndex - 1;

                            retxNode.sCellRetxInfo[arrIndex].harqProcessId =
                                harqProcessIdInAckNackNode[ueServCellIndex];
                            retxNode.sCellRetxInfo[arrIndex].servCellIndex =
                                ueServCellIndex;
                            retxNode.sCellRetxInfo[arrIndex].schUsed =
                                DRA_SCHEDULING;

                        }
                        /* + Coverity_26433 */
                        harqProcessId  = harqProcessIdInAckNackNode[ueServCellIndex];
                        /* - Coverity_26433 */
                        if (harqProcessId == INVALID_HARQ_ID)
                        {
                            continue;
                        }    

                        dlHarqProcess_p =  &(dlHarqEntity_p->harqProcess[harqProcessId]);

                        LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ, 
                                "[%s]  \n current_TX_NB_TB_ONE = %d current_TX_NB_TB_TWO = %d"
                                "assignedResourcesNB = %d \n",__func__,
                                dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB,
                                dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB,
                                dlHarqProcess_p->assignedResourcesNB);


                        if ( (transmissionMode == TX_MODE_3) ||
                                (transmissionMode == TX_MODE_4) ||
                                /* + TM7_8 Changes Start */
                                ((transmissionMode == TX_MODE_8) && 
                                 (1 < cellSpecificParams_g.cellConfigAndInitParams_p[ueInternalCellIndex]->\
                                  cellParams_p->numOfTxAnteenas)))
                            /* - TM7_8 Changes End */
                        {
                            LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,\
                                    "[%s]######## TX_MODE_3 || TX_MODE_4\n",__func__);

                            if (dlHarqProcess_p->isTBOneValid && dlHarqProcess_p->isTBTwoValid)
                            {
                                /* Calculating Harq bler based on the harq indication received and
                                 * convert the value of harq dlHarqFlag[ueServCellIndex] such that it represents
                                 * either ack or nack using 2 LSB bits for both the TBs
                                 */

                                UInt8 oprTb1 = dlHarqFlag[ueServCellIndex] & 0x07;
                                UInt8 oprTb2 = dlHarqFlag[ueServCellIndex] & 0x70;
                                if (isHarqOnPucch)
                                {
                                    UInt8 oprTb = oprTb1;
                                    // Use any one of the oprTBx only when both the TBs has
                                    // Sure case (i.e. either ACK or NACK)
                                    if ((oprTb1 < 3) && ((oprTb2 >> 4) < 3))
                                    {
                                        oprTb = oprTb1;
                                    }
                                    else
                                    {
                                        // Make it Unsure (i.e. other Than ACK or NACK)
                                        oprTb = 3;
                                    }

                                    /*SPR_4001_CHANGE_START*/
                                    /* Cyclomatic Complexity changes - starts here */
                                    updateAndCheckHarqBlerForPucch(dlUeContext_p, oprTb, globalTick
                                            , ueInternalCellIndex);
                                    /* Cyclomatic Complexity changes - ends here */
                                    /*SPR_4001_CHANGE_END*/
                                }

                                value1 = (oprTb1 == ACK)?ACK:NACK;
                                value2 = (oprTb2 == 0x10)?ACK:NACK;

                                dlHarqFlag[ueServCellIndex] = 0;

                                if(value1 == ACK && value2 == ACK)
                                {
                                    dlHarqFlag[ueServCellIndex] |= MIMO_ACK_NACK_MASK;
                                    dlHarqFlag[ueServCellIndex] |= BOTH_TB_VALID_MASK;
                                }
                                else if(value1 == ACK && value2 == NACK)
                                {
                                    dlHarqFlag[ueServCellIndex] |= MIMO_ACK_NACK_ON_TB_ONE_ONLY_MASK;
                                    dlHarqFlag[ueServCellIndex] |= BOTH_TB_VALID_MASK;
                                }
                                else if(value1 == NACK && value2 == ACK)
                                {
                                    dlHarqFlag[ueServCellIndex] |= MIMO_ACK_NACK_ON_TB_TWO_ONLY_MASK;
                                    dlHarqFlag[ueServCellIndex] |= BOTH_TB_VALID_MASK;
                                }
                                else if(value1 == NACK && value2 == NACK)
                                {
                                    dlHarqFlag[ueServCellIndex] |= BOTH_TB_VALID_MASK;
                                }

                            }
                            else if (dlHarqProcess_p->isTBOneValid && !dlHarqProcess_p->isTBTwoValid)
                            {
                                /* Calculating Harq bler based on the harq indication received and
                                 * convert the value of harq dlHarqFlag[ueServCellIndex] such that it represents
                                 * either ack or nack using 2 LSB bits for the TB
                                 */
                                UInt8 oprTb1 = (dlHarqFlag[ueServCellIndex] & 0x07);

                                if (isHarqOnPucch)
                                {
                                    /*SPR_4001_CHANGE_START*/
                                    /* Cyclomatic Complexity changes - starts here */
                                    updateAndCheckHarqBlerForPucch(dlUeContext_p, oprTb1, globalTick
                                            , ueInternalCellIndex);
                                    /* Cyclomatic Complexity changes - ends here */
                                    /*SPR_4001_CHANGE_END*/
                                }

                                value1 = (oprTb1 == ACK)?ACK:NACK;
                                dlHarqFlag[ueServCellIndex] = 0;

                                if(value1 == ACK)
                                {
                                    dlHarqFlag[ueServCellIndex] |= MIMO_ACK_NACK_ON_TB_ONE_ONLY_MASK;
                                    dlHarqFlag[ueServCellIndex] |=  ONLY_TB_ONE_VALID;
                                }
                                else if(value1 == NACK)
                                {
                                    dlHarqFlag[ueServCellIndex] |=  ONLY_TB_ONE_VALID;
                                }
                            }
                            else if (!dlHarqProcess_p->isTBOneValid && dlHarqProcess_p->isTBTwoValid)
                            {
                                /* Calculating Harq bler based on the harq indication received and
                                 * convert the value of harq dlHarqFlag[ueServCellIndex] such that it represents
                                 * either ack or nack using 2 LSB bits for the TB
                                 */
                                /* SPR 19679: TDD HARQ Multiplexing Changes Start */
                                UInt8 oprTb2 = 0;
                                if(HARQ_MULTIPLEXING_MODE == modeType)
                                {
                                    oprTb2 = dlHarqFlag[ueServCellIndex] & 0x07;
                                }
                                else 
                                {
                                    oprTb2 =  dlHarqFlag[ueServCellIndex] & 0x70;
                                }    
                                /* SPR 19679: TDD HARQ Multiplexing Changes End */

                                if (isHarqOnPucch)
                                {
                                    /*SPR_4001_CHANGE_START*/
                                    /* Cyclomatic Complexity changes - starts here */
                                    updateAndCheckHarqBlerForPucch(dlUeContext_p, oprTb2 >> 4, globalTick
                                            , ueInternalCellIndex);
                                    /* Cyclomatic Complexity changes - ends here */
                                    /*SPR_4001_CHANGE_END*/
                                }

                                value2 = (oprTb2 == 0x10)?ACK:NACK;
                                dlHarqFlag[ueServCellIndex] = 0;

                                if(value2== ACK)
                                {
                                    dlHarqFlag[ueServCellIndex]  |= MIMO_ACK_NACK_ON_TB_TWO_ONLY_MASK;
                                    dlHarqFlag[ueServCellIndex] |= ONLY_TB_TWO_VALID;
                                }
                                if(value2== NACK)
                                {
                                    dlHarqFlag[ueServCellIndex] |= ONLY_TB_TWO_VALID;
                                }
                            }
                            else 
                            {
                                LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ, \
                                        "\n BOTH THE TBs ARE INVALID OF THIS NODE");                            
                                dlHarqFlag[ueServCellIndex] = 0;
                                continue;
                            }

                            /* Cyclomatic Complexity changes - starts here */
                            /* + 32.425 Events changes */
                            /* +- SPR 17777 */
                            processDLHarqMIMOAckNack (dlHarqFlag[ueServCellIndex], ueIndex, 
                                    globalTick, dlUeContext_p, 
                                    dlHarqProcess_p,
                                    /* + SPS_TDD */
                                    ackNackNode_p,
                                    /* - SPS_TDD */
#ifdef KPI_STATS
                                    recvTTI,
#endif
                                    ueInternalCellIndex,
                                    ueServCellIndex,&bitMaskForAperiodicReport,
                                    &nackFlag[ueServCellIndex] 
                                    );
                            /* +- SPR 17777 */

                            /* + 32.425 Events changes */
                            /* Cyclomatic Complexity changes - ends here */

                        }/* End of if checking for MIMO mode*/
                        /*
                           We now Handle the Non - MIMO case
                           */
                        else
                        {
                            LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,\
                                    "[%s] Other Than TX_MODE_3/TX_MODE_4\n",__func__);
                            /* Calculating Harq bler based on the harq indication received and
                             * convert the value of harq dlHarqFlag[ueServCellIndex] such that it represents
                             * either ack or nack using 2 LSB bits for the TB
                             */
                            UInt8 oprTb1 = dlHarqFlag[ueServCellIndex] & 0x07;
                            if (isHarqOnPucch)
                            {
                                /*SPR_4001_CHANGE_START*/
                                /* Cyclomatic Complexity changes - starts here */
                                updateAndCheckHarqBlerForPucch(dlUeContext_p, oprTb1, globalTick
                                        , ueInternalCellIndex);
                                /* Cyclomatic Complexity changes - ends here */
                                /*SPR_4001_CHANGE_END*/
                            }
                            dlHarqFlag[ueServCellIndex] = (oprTb1 == ACK)?ACK:NACK;
                            /* Cyclomatic Complexity changes - starts here */
                            processDLHarqNonMIMOAckNack (dlHarqFlag[ueServCellIndex], ueIndex, 
                                    /* +- SPR 17777 */
                                    globalTick, dlUeContext_p, 
                                    dlHarqProcess_p,
                                    /*SPR 11812 Fix Start*/
                                    /* + SPS_TDD */
                                    ackNackNode_p,
                                    /* - SPS_TDD */
                                    /* +- SPR 17777 */
#ifdef KPI_STATS
                                    recvTTI,
#endif
                                    /* +- SPR 17777 */
                                    /*SPR 11812 Fix End*/
                                    ueInternalCellIndex,
                                    ueServCellIndex,
                                    &bitMaskForAperiodicReport
                                    ,&nackFlag[ueServCellIndex]);
                            /* Cyclomatic Complexity changes - ends here */

                        }
                        if(TRUE == nackFlag[ueServCellIndex])
                        {
                            retxNode.isNodeInvalid = FALSE;
                        }
                    }  /* End of loop of serving cells */
                    /* + SPR 14139 Changes */
                    if((FALSE == retxNode.isNodeInvalid) && (MAC_FAILURE ==
                                macPushHarqNackInfoToRetxQueue(dlUeContext_p ,retxNode ,
                                    nackFlag,internalCellIndex, subFrameNumber) ))
                        /* - SPR 14139 Changes */
                    {
                        macHandleRetxQueueFailure(ueIndex,nackFlag,retxNode);
                    }
                    nackFlag[PCELL_SERV_CELL_IX] = FALSE;
                    nackFlag[START_SCELL_INDEX] = FALSE;
                    retxNode.isNodeInvalid = TRUE;

                    dlHarqFlag[PCELL_SERV_CELL_IX] = 0;
                    dlHarqFlag[START_SCELL_INDEX] = 0; 

                    /* single entry of ulGrantAperiodicCqiQueue_gp(of PCell)
                     * if for CA UEs condition to trigger Aperiodic CSI o
                     * ccurs for single or both cells and update node variable
                     * aperiodicCQIToBeRequested */
                    if(bitMaskForAperiodicReport)
                    {
                        /* + SPR_14408 */
                        putEntryInULAperiodicCQIQueue(ueIndex, internalCellIndex, bitMaskForAperiodicReport);
                        /* - SPR_14408 */
                    }

                }  
                /* End of if ( INVALID HARQ ID ) */
                else
                {
                    lteWarning("Else of dlharqTimerNode_p->isValid UE Index [%d] Harq ID [%d]\n", ueIndex, harqProcessId);

                }
                /* - CA_TDD_HARQ_CHANGES */

            } /* End of IF(pending delete flag */    
            /*
               We must free this node now as the pendingDeleteFlag has been set
               */
            else
            {
                LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ,
                        "Else of !Pending Delete flag \n");
            }     
            freeMemPool((void *)ackNackNode_p);
        } /* End of IF(ackNackNode_p....) */
        else
        { 
            LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ, \
                    "\n The ACK NACK NODE IS NULL\n");
        }
        ackNackQueueCount--;    
    } /* End of WHILE */
    LOG_UT(MAC_FUNCTION_EXIT,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,\
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
    return MAC_SUCCESS;
}
/*****************************************************************************
 * Function Name  : processDLHarqMIMOAckNack 
 * Inputs         : operation - Harq operation,
 *                  ueIndex - UE index,
 *                  globalTick - Current tick,
 *                  harqProcessId - Indicates Harq process Id,
 *                  dlUeContext_p - Pointer to DLUEContext,
 *                  dlHarqProcess_p - Pointer to DLHARQProcess
 *                  recvTTI - Received TTI in node
 *                  ueInternalCellIndex - Cell-Index at MAC
 *                  servingIndex - serving Index 
 *                  bitMaskForAperiodicReport_p - Bitmap to set trigger for
 *                                aperiodic CQI.
 *                  nackFlag_p - Nack flag to check retx.              
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the DL Harq ACK/NACK based on the
 *                  presence of TB1 and/or TB2 for MIMO mode.
 *****************************************************************************/
static  void processDLHarqMIMOAckNack ( UInt8 operation, 
        UInt16        ueIndex,
        /* SPR 15909 fix start */
        tickType_t        globalTick, 
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
        DLUEContext   *dlUeContext_p, 
        DLHARQProcess *dlHarqProcess_p,
		/* + 32.425 Events changes */
        /* +- SPR 17777 */
        /* + SPS_TDD */
        tddAckNackQueueNode  *ackNackNode_p,
        /* - SPS_TDD */
#ifdef KPI_STATS
         UInt8          recvTTI,
#endif
         /* +- SPR 17777 */
        /* + 32.425 Events changes */
        /* + CA_TDD_HARQ_CHANGES */
        InternalCellIndex ueInternalCellIndex,
        InternalCellIndex  servingIndex,
        UInt8 *bitMaskForAperiodicReport_p,        
        UInt8 *nackFlag_p
        /* - CA_TDD_HARQ_CHANGES */
        )
{
    /* TDD SB_CQI */
    ULUEContext* ueULContext_p  = PNULL;
    DLCQIInfo   *dlCQIInfo_p   = PNULL;
    InternalCellIndex internalCellIndex = 0;

    DLMIMOInfo *dlMIMOInfo_p   = PNULL;

    ueULContext_p  = (&ulUECtxInfoArr_g[ueIndex])->ulUEContext_p;
     /* + CA_TDD_HARQ_CHANGES */
    if(IS_PCELL_OF_UE(dlUeContext_p, ueInternalCellIndex))
    {
        /* CA_phase2_harq_code Start */
        dlCQIInfo_p  = &(dlUeContext_p->dlCQIInfo);
        /* CA_phase2_harq_code End */
    	dlMIMOInfo_p = &(dlUeContext_p->dlMIMOInfo);
    }
    else 
    {
        dlCQIInfo_p =
           &(dlUeContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo);
    	dlMIMOInfo_p = &(dlUeContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo);
    }     
    internalCellIndex = dlUeContext_p->internalCellIndex;
    /* - CA_TDD_HARQ_CHANGES */

    /* TDD SB_CQI */
    /* + SPS_TDD_Changes */
    UInt8 maxRetxCount = 0;
    LP_SpsDlInfo spsDlInfo_p = PNULL;
    LP_MacDlSpsStatus macDlSpsStatus_p = PNULL;
    spsDlInfo_p = &dlUeContext_p->spsDlInfo;
    /* - SPS_TDD_Changes */
    switch (operation)
    {
        /*
           We first consider the case of NACK being received on the 
           TB_TWO while Ack being received on TB_ONE.
           */
        case BOTH_TB_VALID_NACK_ON_TB_TWO_AND_ACK_ON_TB_ONE:
#ifdef ACK_NACK_COUNT_STATS
            lteCommonUpdateAckNackStats(MIMO_TB_ONE, ueIndex, DL_ACK);
            lteCommonUpdateAckNackStats(MIMO_TB_TWO, ueIndex, DL_NACK);
#endif
            /* + 32.425 Events changes */
#ifdef KPI_STATS
            /* This will update the Shared Memory Time and Shared Memory Data */ 
            /* + KPI_CA */
            lteMacUpdateKpiStatsIPThpDlOnHarqAck(ueIndex,dlHarqProcess_p, 
                    /* +- SPR 17777*/
                    recvTTI
#ifdef TDD_CONFIG
                    , internalCellIndex
#endif
                    );
            /* +- SPR 17777*/
            /* - KPI_CA */
            /* SPR 11402 Fix  */
            /* SPR 16843 start */
            updateKpiStatsForTotalSduAckRcvdDlperQci(&dlHarqProcess_p->dlHARQTBOneInfo,
                                                     TB_ONE,internalCellIndex,ueIndex);
            /* SPR 16843 end */
            /* SPR 11402 Fix  */
#endif
            /* + 32.425 Events changes */

            /* SPR 5124 changes start */
            /* BLER calculation for TB1 */
            /* Cyclomatic Complexity changes - starts here */
            /* CA_phase2_harq_code Start */
            /* + CA_TDD_HARQ_CHANGES */
            calculateDLBlerForAckTB1(globalTick, 
                    ueULContext_p,
                    dlCQIInfo_p,
                    ueIndex, 
                    internalCellIndex,
                    servingIndex,
                    bitMaskForAperiodicReport_p,
      		        dlMIMOInfo_p
                    );

            /* BLER calculation for TB2 */
            calculateDLBlerForNackTB2( globalTick,
                    ueULContext_p,
                    dlCQIInfo_p,
                    /* +- SPR 17777*/
                    ueIndex,
                    ueInternalCellIndex,
                    servingIndex,
                    bitMaskForAperiodicReport_p);
            /* - CA_TDD_HARQ_CHANGES */
            /* CA_phase2_harq_code End */
            /* Cyclomatic Complexity changes - ends here */
            /* SPR 5124 changes end */
            /* SPR 16474 Fix Start */
            if ((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag
                        == TA_MSG) ||
                    (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag
                     == TA_DRX_MSG))
            {
                dlUeContext_p->lastTASentTick =
                    getCurrentTick() - PHY_UL_DATA_DELAY;
            }
            /* SPR 16474 Fix End */

#ifdef PERF_STATS
            UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_ACK ,
                    dlHarqProcess_p->dlHARQTBOneInfo.mcsIndex);

            UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_NACK ,
                    dlHarqProcess_p->dlHARQTBTwoInfo.mcsIndex);
#endif
            /* SPR 3679 Changes Start */
            /* + CA_TDD_HARQ_CHANGES */
            if (dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB == \
                (cellSpecificParams_g.cellConfigAndInitParams_p[ueInternalCellIndex]->\
                     cellParams_p->maxHarqRetransmission - 1))     
            /* - CA_TDD_HARQ_CHANGES */
            {
#ifdef PERF_STATS
                gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats.\
                    totalHarqFail++;
#endif

                /* _RLC_AM_MODE_ */
                if (!((dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_MSG) ||
                      (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == DRX_MSG) ||
                      (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_DRX_MSG) || 
                      (dlHarqProcess_p->dlHARQTBTwoInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                      (dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p))
                {									                                        
                    /* SPR 11402 fix */
                    sendARQIndicationToRLC(
                            ueIndex, 
                            &dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex);
                    /* SPR 11402 fix */
                }
                /* _RLC_AM_MODE_ */

                freeDLHarqProcess(dlUeContext_p,
                        dlHarqProcess_p,
                       /* + CA_TDD_HARQ_CHANGES */
                        ueInternalCellIndex);
                       /* - CA_TDD_HARQ_CHANGES */
            }
            else
            {
                *nackFlag_p = TRUE;
                freeDLHarqTB(dlUeContext_p, dlHarqProcess_p,
                        TB_ONE, ueInternalCellIndex);
                       /* - CA_TDD_HARQ_CHANGES */
            }
            break;
            /* SPR 3679 Changes End */

            /*
               We first consider the case of NACK being received on both the 
               TB_ONE and Ack on TB_TWO of MIMO case.
               */
        case BOTH_TB_VALID_ACK_ON_TB_TWO_AND_NACK_ON_TB_ONE:
            /* SPR 3679 Changes Start */
#ifdef ACK_NACK_COUNT_STATS
            lteCommonUpdateAckNackStats(MIMO_TB_ONE, ueIndex, DL_NACK);
            lteCommonUpdateAckNackStats(MIMO_TB_TWO, ueIndex, DL_ACK);
#endif
            /* + 32.425 Events changes */
#ifdef KPI_STATS
            /* This will update the Shared Memory Time and Shared Memory Data */ 
            /* + KPI_CA */
            /* +- SPR 17777 */
            lteMacUpdateKpiStatsIPThpDlOnHarqAck(ueIndex,dlHarqProcess_p,
                    recvTTI
#ifdef TDD_CONFIG
                    , internalCellIndex
#endif
                    );
            /* +- SPR 17777 */
            /* - KPI_CA */
            /* SPR 11402 Fix  */
            /* SPR 16843 start */
            updateKpiStatsForTotalSduAckRcvdDlperQci(&dlHarqProcess_p->dlHARQTBTwoInfo,
                    TB_TWO,internalCellIndex,ueIndex);
            /* SPR 16843 end */
            /* SPR 11402 Fix  */
#endif
            /* + 32.425 Events changes */

            /* SPR 5124 changes start */
            /* BLER calculation for TB1 */
            /* Cyclomatic Complexity changes - starts here */
            /* CA_phase2_harq_code Start */
            /* + CA_TDD_HARQ_CHANGES */
            calculateDLBlerForNackTB1( globalTick,
                    ueULContext_p,
                    dlCQIInfo_p,
                    ueIndex,
                    ueInternalCellIndex,
                    dlMIMOInfo_p,
                    servingIndex,
                    bitMaskForAperiodicReport_p);

            /* BLER calculation for TB2 */
            calculateDLBlerForAckTB2( globalTick,
                    ueULContext_p,
                    dlCQIInfo_p,
                    /* +- SPR 17777*/
                    ueIndex,
                    internalCellIndex,
                    servingIndex,
                    bitMaskForAperiodicReport_p); 
            /* - CA_TDD_HARQ_CHANGES */
            /* CA_phase2_harq_code End */
            /* Cyclomatic Complexity changes - ends here */
            /* SPR 5124 changes end */

#ifdef PERF_STATS
            UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_NACK ,
                    dlHarqProcess_p->dlHARQTBOneInfo.mcsIndex);

            UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_ACK ,
                    dlHarqProcess_p->dlHARQTBTwoInfo.mcsIndex);
#endif
            /* + CA_TDD_HARQ_CHANGES */
            if (dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB == \
                    (cellSpecificParams_g.cellConfigAndInitParams_p[ueInternalCellIndex]->\
                     cellParams_p->maxHarqRetransmission - 1))     
           /* - CA_TDD_HARQ_CHANGES */
            {
#ifdef PERF_STATS
                gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats.\
                    totalHarqFail++;
#endif

                /* _RLC_AM_MODE_ */
                if (!((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_MSG) ||
                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == DRX_MSG) ||
                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_DRX_MSG) || 
                            (dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                        (dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p))
                { 
                    /* SPR 11402 fix */
                    sendARQIndicationToRLC(
                            ueIndex, 
                            &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex);
                    /* SPR 11402 fix */
                }
                /* _RLC_AM_MODE_ */
                freeDLHarqProcess(dlUeContext_p,
                        dlHarqProcess_p,
                        /* + CA_TDD_HARQ_CHANGES */
                        ueInternalCellIndex);
                       /* - CA_TDD_HARQ_CHANGES */
            }
            else
            {
                *nackFlag_p = TRUE;
                freeDLHarqTB(dlUeContext_p, dlHarqProcess_p,
                        TB_TWO, ueInternalCellIndex);
                     /* - CA_TDD_HARQ_CHANGES */
            }

            break;
            /* SPR 3679 Changes End */

            /*
               We first consider the case of NACK being received on both the 
               TBs of MIMO case.
               */
        case BOTH_TB_VALID_NACK_ON_BOTH_TB:
            LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ, \
                    "\nCase: BOTH_TB_VALID_NACK_ON_BOTH_TB\n");
#ifdef ACK_NACK_COUNT_STATS
            lteCommonUpdateAckNackStats(TB_ONE, ueIndex, DL_NACK);
            lteCommonUpdateAckNackStats(TB_TWO, ueIndex, DL_NACK);
#endif
            /* SPR 5124 changes start */
            /* BLER calculation for TB1 */
            /* Cyclomatic Complexity changes - starts here */
            /* CA_phase2_harq_code Start */
            /* + CA_TDD_HARQ_CHANGES */
            calculateDLBlerForNackTB1( globalTick,
                    ueULContext_p,
                    dlCQIInfo_p,
                    ueIndex, 
                    ueInternalCellIndex,
                    dlMIMOInfo_p,
                    servingIndex,
                    bitMaskForAperiodicReport_p);


            /* BLER calculation for TB2 */
            calculateDLBlerForNackTB2( globalTick,
                    ueULContext_p,
                    dlCQIInfo_p,
                    /* +- SPR 17777*/
                    ueIndex, 
                    ueInternalCellIndex,
                    servingIndex,
                    bitMaskForAperiodicReport_p);
            /* - CA_TDD_HARQ_CHANGES */
            /* CA_phase2_harq_code End */
            /* Cyclomatic Complexity changes - ends here */
            /* SPR 5124 changes end */
#ifdef PERF_STATS
            UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_NACK ,
                    dlHarqProcess_p->dlHARQTBOneInfo.mcsIndex);

            UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_NACK ,
                    dlHarqProcess_p->dlHARQTBTwoInfo.mcsIndex);
#endif
            /* 
               We now check for the re-transmission counter of both
               the TBs.
               */
            /* SPR 5612 Start */
             /* + CA_TDD_HARQ_CHANGES */
            if (dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB != \
                    (cellSpecificParams_g.cellConfigAndInitParams_p[ueInternalCellIndex]->\
                     cellParams_p->maxHarqRetransmission - 1)
                    || dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB != \
                    (cellSpecificParams_g.cellConfigAndInitParams_p[ueInternalCellIndex]->\
                     cellParams_p->maxHarqRetransmission - 1))	  
            {
                if (dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB ==\
                        (cellSpecificParams_g.cellConfigAndInitParams_p[ueInternalCellIndex]->\
                         cellParams_p->maxHarqRetransmission - 1))
             /* - CA_TDD_HARQ_CHANGES */
                {
                    /*
                       We send the ARQ indication to RLC twice, one for each
                       TB and then we free the HARQ Process completely.
                       */

                    /* _RLC_AM_MODE_ */

                    if (!((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_MSG) ||
                                (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == DRX_MSG) ||
                                (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_DRX_MSG) || 
                                (dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                            (dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p))		

                    { 
                        /* SPR 11402 fix */
                        sendARQIndicationToRLC(
                                ueIndex, 
                                &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex);
                        /* SPR 11402 fix */
                    }
                       /* SPR 13888 fix start */
#ifdef KPI_STATS
                     /* Reset pdcpAckBytes,T1 and T2 at harq Common Index  */
                    /* + SPR 17745 */
                    lteMacUpdateKpiStatsIPThpDlOnHarqNack(dlHarqProcess_p ,
                            ueIndex, recvTTI,internalCellIndex );
                    /* - SPR 17745 */

#endif
                    /* SPR 13888 fix end */
                   
                    
                    /* + CA_TDD_HARQ_CHANGES */
                    freeDLHarqTB(dlUeContext_p, dlHarqProcess_p, TB_ONE, ueInternalCellIndex);
                     /* - CA_TDD_HARQ_CHANGES */
                    /* _RLC_AM_MODE_ */
                }
                 /* - CA_TDD_HARQ_CHANGES */
                else if(dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB ==\
                        (cellSpecificParams_g.cellConfigAndInitParams_p[ueInternalCellIndex]->\
                         cellParams_p->maxHarqRetransmission - 1))
                 /* - CA_TDD_HARQ_CHANGES */
                {
                    /*
                       We send the ARQ indication to RLC twice, one for each
                       TB and then we free the HARQ Process completely.
                       */

                    /* _RLC_AM_MODE_ */
                    if (!((dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_MSG) ||
                                (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == DRX_MSG) ||
                                (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_DRX_MSG) || 
                                (dlHarqProcess_p->dlHARQTBTwoInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                            (dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p))
                    {																			
                        /* SPR 11402 fix */
                        sendARQIndicationToRLC(
                                ueIndex, 
                                &dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex);
                        /* SPR 11402 fix */
                    }
                    /* SPR 13888 fix start */
#ifdef KPI_STATS
                    /* Reset pdcpAckBytes,T1 and T2 at harq Common Index  */
                    /* + SPR 17745 */
                    lteMacUpdateKpiStatsIPThpDlOnHarqNack(dlHarqProcess_p ,ueIndex,
                            recvTTI,internalCellIndex );
                    /* - SPR 17745 */
#endif
                    /* SPR 13888 fix end */
 

                    freeDLHarqTB(dlUeContext_p, dlHarqProcess_p,
                            /* + CA_TDD_HARQ_CHANGES */
                            TB_TWO, ueInternalCellIndex);
                            /* - CA_TDD_HARQ_CHANGES */
                }

                *nackFlag_p = TRUE;
            }
            else	/* SPR 2249 Fixes Ends	*/
            {

                /*
                   We send the ARQ indication to RLC twice, one for each
                   TB and then we free the HARQ Process completely.
                   */

                /* _RLC_AM_MODE_ */
                if (!((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_MSG) ||
                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == DRX_MSG) ||
                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_DRX_MSG) || 
                            (dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                        (dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p))
                { 
                    /* SPR 11402 fix */
                    sendARQIndicationToRLC(
                            ueIndex, 
                            &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex);
                    /* SPR 11402 fix */
                }
                /* _RLC_AM_MODE_ */
                if (!((dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_MSG) ||
                            (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == DRX_MSG) ||
                            (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_DRX_MSG) || 
                            (dlHarqProcess_p->dlHARQTBTwoInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                        (dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p))

                {																			
                    /* SPR 11402 fix */
                    sendARQIndicationToRLC(
                            ueIndex, 
                            &dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex);
                    /* SPR 11402 fix */
                }
                  /* SPR 13888 fix start */
#ifdef KPI_STATS
                /* Reset pdcpAckBytes,T1 and T2 at harq Common Index  */
                /* + SPR 17745 */
                lteMacUpdateKpiStatsIPThpDlOnHarqNack(dlHarqProcess_p ,ueIndex,
                        recvTTI ,internalCellIndex);
                /* - SPR 17745 */

#endif
                /* SPR 13888 fix end */
 

                freeDLHarqProcess(dlUeContext_p,
                        dlHarqProcess_p,
                        /* + CA_TDD_HARQ_CHANGES */
                        ueInternalCellIndex);
                        /* - CA_TDD_HARQ_CHANGES */
            }

            /* SPR 2249 Fixes Begins */
            /* SPR 5612 End */

            break;                                     
            /*
               We first consider the case of ACK being received on both the 
               TBs of MIMO case.
               */
        case BOTH_TB_VALID_ACK_ON_BOTH_TB:
            LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ, \
                    "\nCase: BOTH_TB_VALID_ACK_ON_BOTH_TB\n");
#ifdef ACK_NACK_COUNT_STATS
            lteCommonUpdateAckNackStats(TB_ONE, ueIndex, DL_ACK);
            lteCommonUpdateAckNackStats(TB_TWO, ueIndex, DL_ACK);
#endif
            /* + 32.425 Events changes */
#ifdef KPI_STATS
            /* This will update the Shared Memory Time and Shared Memory Data */ 
            /* + KPI_CA */
            /* +- SPR 17777 */
            lteMacUpdateKpiStatsIPThpDlOnHarqAck(ueIndex,dlHarqProcess_p, 
                    recvTTI
#ifdef TDD_CONFIG
                    , internalCellIndex
#endif
                    );
            /* +- SPR 17777 */
            /* - KPI_CA */
            /* SPR 11402 Fix  */
            /* SPR 16843 start */
            updateKpiStatsForTotalSduAckRcvdDlperQci(&dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex,ueIndex);
            updateKpiStatsForTotalSduAckRcvdDlperQci(&dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex,ueIndex);
            /* SPR 16843 end */
            /* SPR 11402 Fix  */
#endif

            /* + 32.425 Events changes */
            /* SPR 5124 changes start */
            /* BLER calculation for TB1 */
            /* Cyclomatic Complexity changes - starts here */
            /* + CA_TDD_HARQ_CHANGES */
            calculateDLBlerForAckTB1( globalTick,
                    ueULContext_p,
                    dlCQIInfo_p,
                    ueIndex,
                    internalCellIndex,
                    servingIndex,
                    bitMaskForAperiodicReport_p,
		            dlMIMOInfo_p);

            /* BLER calculation for TB2 */
            calculateDLBlerForAckTB2( globalTick,
                    ueULContext_p,
                    dlCQIInfo_p, 
                    /* +- SPR 17777*/
                    ueIndex,
                    internalCellIndex,
                    servingIndex,
                    bitMaskForAperiodicReport_p);
            /* - CA_TDD_HARQ_CHANGES */
            /* Cyclomatic Complexity changes - ends here */
            /* SPR 5124 changes end */
#ifdef PERF_STATS
            UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_ACK ,
                    dlHarqProcess_p->dlHARQTBOneInfo.mcsIndex);

            UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_ACK ,
                    dlHarqProcess_p->dlHARQTBTwoInfo.mcsIndex);
#endif
            /*Fix for __SPR_1988__ START*/
            /*Update the lastTASentTick to maintain that UE
              is in UL Sync at the current TTI. This will be
              updated whenever ACK is received for TB*/
            if ((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag
                        == TA_MSG) ||
                    (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag
                     == TA_DRX_MSG))
            {
                dlUeContext_p->lastTASentTick =
                    getCurrentTick() - PHY_UL_DATA_DELAY;
            }
            /*Fix for __SPR_1988__ END*/

#ifdef KPI_STATS
            /*Update the pktDelay KPI counters for packets in 
             * DL which are positively acked based on HARQ
             * feedback*/

			/* + KPI_CA */
            /* SPR 12783 Changes Start */
            macUpdateKpiCntrsForPktDelayPerQci(&dlHarqProcess_p->dlHARQTBOneInfo,
					ueIndex, 1, internalCellIndex, recvTTI);
            macUpdateKpiCntrsForPktDelayPerQci(&dlHarqProcess_p->dlHARQTBTwoInfo, 
					ueIndex, 2, internalCellIndex, recvTTI);
            /* SPR 12783 Changes End */
			/* - KPI_CA */
#endif
            LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ, \
                    "\nCase: BOTH_TB_VALID_ACK_ON_BOTH_TB\n");
            /*
               Since both the TB has received ACK hence
               we remove the entire HARQ process.
               */

            freeDLHarqProcess(dlUeContext_p,
                    dlHarqProcess_p,
                    /* + CA_TDD_HARQ_CHANGES */
                    ueInternalCellIndex);
                    /* - CA_TDD_HARQ_CHANGES */
            break;                                    

            /* SPR 3679 Changes Start */
        case TB_TWO_VALID_TB_ONE_INVALID_NACK_ON_TB_TWO:
#ifdef ACK_NACK_COUNT_STATS
            lteCommonUpdateAckNackStats(MIMO_TB_TWO, ueIndex, DL_NACK);
#endif
            /* SPR 5124 changes start */
            /* BLER calculation for TB2 */
            /* Cyclomatic Complexity changes - starts here */
            /* + CA_TDD_HARQ_CHANGES */
            calculateDLBlerForNackTB2( globalTick,
                    ueULContext_p,
                    dlCQIInfo_p,
                    /* +- SPR 17777*/
                    ueIndex, 
                    ueInternalCellIndex,
                    servingIndex,
                    bitMaskForAperiodicReport_p);
            /* - CA_TDD_HARQ_CHANGES */
            /* Cyclomatic Complexity changes - ends here */
            /* SPR 5124 changes end */

#ifdef PERF_STATS
            UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_NACK ,
                    dlHarqProcess_p->dlHARQTBTwoInfo.mcsIndex);
#endif
            /*Fix for __SPR_1988__ START*/
            /* + CA_TDD_HARQ_CHANGES */
            if (dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB == \
                    (cellSpecificParams_g.cellConfigAndInitParams_p[ueInternalCellIndex]->\
                     cellParams_p->maxHarqRetransmission - 1))     
            /* - CA_TDD_HARQ_CHANGES */
            {
#ifdef PERF_STATS
                gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats.\
                    totalHarqFail++;
#endif

                /* _RLC_AM_MODE_ */
                if (!((dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_MSG) ||
                            (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == DRX_MSG) ||
                            (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_DRX_MSG) ||
                            (dlHarqProcess_p->dlHARQTBTwoInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                        (dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p))
                {									                                        
                    /* SPR 11402 fix */
                    sendARQIndicationToRLC(
                            ueIndex, 
                            &dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex);
                    /* SPR 11402 fix */
                }
                /* _RLC_AM_MODE_ */
                  /* SPR 13888 fix start */
#ifdef KPI_STATS
                /* Reset pdcpAckBytes,T1 and T2 at harq Common Index  */
                /* + SPR 17745 */
                lteMacUpdateKpiStatsIPThpDlOnHarqNack(dlHarqProcess_p, ueIndex,
                        recvTTI,internalCellIndex );
                /* - SPR 17745 */
#endif
                /* SPR 13888 fix end */
 

                freeDLHarqProcessTBSpecific(
                        dlUeContext_p,
                        dlHarqProcess_p,
                        TB_TWO,
                        /* + CA_TDD_HARQ_CHANGES */
                        ueInternalCellIndex);
                        /* - CA_TDD_HARQ_CHANGES */
            }
            else
            {
                *nackFlag_p = TRUE;
            }
            break;

        case TB_TWO_VALID_TB_ONE_INVALID_ACK_ON_TB_TWO:
#ifdef ACK_NACK_COUNT_STATS
            lteCommonUpdateAckNackStats(MIMO_TB_TWO, ueIndex, DL_ACK);
#endif
            /* + 32.425 Events changes */
#ifdef KPI_STATS
            /* This will update the Shared Memory Time and Shared Memory Data */ 
            /* + KPI_CA */
            /* +- SPR 17777 */
            lteMacUpdateKpiStatsIPThpDlOnHarqAck(ueIndex,dlHarqProcess_p, 
                    recvTTI
#ifdef TDD_CONFIG
                    , internalCellIndex
#endif
                    );
            /* +- SPR 17777 */
            /* - KPI_CA */
            /* SPR 11402 Fix  */
            /* SPR 16843 start */
            updateKpiStatsForTotalSduAckRcvdDlperQci(&dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex,ueIndex);
            /* SPR 16843 end */
            /* SPR 11402 Fix  */
#endif

            /* + 32.425 Events changes */
            /* SPR 5124 changes start */
            /* BLER calculation for TB2 */
            /* Cyclomatic Complexity changes - starts here */
            /* + CA_TDD_HARQ_CHANGES */
            calculateDLBlerForAckTB2( globalTick,
                    ueULContext_p,
                    dlCQIInfo_p, 
                    /* +- SPR 17777*/
                    ueIndex,
                    internalCellIndex,
                    servingIndex,
                    bitMaskForAperiodicReport_p);
            /* - CA_TDD_HARQ_CHANGES */
            /* Cyclomatic Complexity changes - ends here */
            /* SPR 5124 changes end */

#ifdef KPI_STATS
            /*Update the pktDelay KPI counters for packets in 
             * DL which are positively acked based on HARQ
             * feedback*/
			/* + KPI_CA */
            /* SPR 12783 Changes Start */
            macUpdateKpiCntrsForPktDelayPerQci(&dlHarqProcess_p->dlHARQTBTwoInfo,
					ueIndex, 1, internalCellIndex, recvTTI);
            /* SPR 12783 Changes End */
			/* - KPI_CA */
#endif

#ifdef PERF_STATS
            UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_ACK ,
                    dlHarqProcess_p->dlHARQTBTwoInfo.mcsIndex);
#endif
            /*Fix for __SPR_1988__ START*/
            freeDLHarqProcessTBSpecific(
                    dlUeContext_p,
                    dlHarqProcess_p,
                    TB_TWO,
                   /* + CA_TDD_HARQ_CHANGES */
                   ueInternalCellIndex);
                   /* - CA_TDD_HARQ_CHANGES */
            break;
            /* SPR 3679 Changes End */

            /*
               Since we have received the ACK on the only valid TB ie TB
               ONE we just need to free the HARQ PROCESS.
               */
        case TB_TWO_INVALID_TB_ONE_VALID_ACK_ON_TB_ONE:
            LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ, \
                    "\nCase: TB_TWO_INVALID_TB_ONE_VALID_ACK_ON_TB_ONE\n");
#ifdef ACK_NACK_COUNT_STATS
            lteCommonUpdateAckNackStats(TB_ONE, ueIndex, DL_ACK);
#endif
            /* + 32.425 Events changes */
#ifdef KPI_STATS
            /* This will update the Shared Memory Time and Shared Memory Data */ 
            /* + KPI_CA */
            /* +- SPR 17777 */
            lteMacUpdateKpiStatsIPThpDlOnHarqAck(ueIndex,dlHarqProcess_p, 
                    recvTTI
#ifdef TDD_CONFIG
                    , internalCellIndex
#endif
                    );
            /* +- SPR 17777 */
            /* - KPI_CA */
            /* SPR 11402 Fix  */
            /* SPR 16843 start */
            updateKpiStatsForTotalSduAckRcvdDlperQci(&dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex,ueIndex);
            /* SPR 16843 end */
            /* SPR 11402 Fix  */
#endif
            /* + 32.425 Events changes */

            /* SPR 5124 changes start */
            /* BLER calculation for TB1 */
            /* Cyclomatic Complexity changes - starts here */
            /* + CA_TDD_HARQ_CHANGES */
            calculateDLBlerForAckTB1( globalTick,
                    ueULContext_p,
                    dlCQIInfo_p,
                    ueIndex,
                    internalCellIndex,
                    servingIndex,
                    bitMaskForAperiodicReport_p,
		            dlMIMOInfo_p);
            /* - CA_TDD_HARQ_CHANGES */
            /* Cyclomatic Complexity changes - ends here */
            /* SPR 5124 changes end */

#ifdef PERF_STATS
            UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_ACK ,
                    dlHarqProcess_p->dlHARQTBOneInfo.mcsIndex);
#endif
            /* + SPS_TDD_Changes */
            /* SPS_CHG */
            /*!
             * \code
             *  If (UE State ==  SPS_ACTIVATION_INITIATED 
             *       or SPS_DEACTIVATION_INITIATED)
             *    and First ACK is Received, 
             *    Push entry in dlSpsStatusQ for strategy.
             * \endcode
             * */
            if(((SPS_ACTIVATION_INITIATED == spsDlInfo_p->spsState )||
                        (SPS_REACTIVATION_INITIATED == spsDlInfo_p->spsState ))
                    &&(!dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB)
                    &&( (PNULL != spsDlInfo_p->spsDlSetupInfo_p) &&
                        (dlHarqProcess_p->harqProcessId <
                         spsDlInfo_p->spsDlSetupInfo_p->numberOfConfSpsProcesses) )
              )
            {
                GET_MEM_FROM_POOL(MacDlSpsStatus, macDlSpsStatus_p, 
                        sizeof(MacDlSpsStatus), PNULL);
                /*+COVERITY 5.3.0 - 32724*/
                if(PNULL == macDlSpsStatus_p)
                {
                    ltePanic("Memory Allocation failed in %s\n",__func__);
                }
                /*+COVERITY 5.3.0 - 32724*/
                macDlSpsStatus_p->ueIndex = dlUeContext_p->ueIndex;
                macDlSpsStatus_p->response = FIRST_ACK_NACK;
                if (MAC_FAILURE == ENQUEUE_MAC_DL_SPS_Q(dlSpsStatusQ_gp[internalCellIndex], 
                        macDlSpsStatus_p))
                {
                    ltePanic("Enque Failed\n");
                }
                /* Update Q Stats */
                LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q, 
                        QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex]),
                        internalCellIndex);
            }
            else if( (SPS_DEACTIVATION_INITIATED == spsDlInfo_p->spsState) 
                    && ( dlHarqProcess_p->harqProcessId == spsDlInfo_p->hpId )  
                   )
            {
                GET_MEM_FROM_POOL(MacDlSpsStatus, macDlSpsStatus_p, 
                        sizeof(MacDlSpsStatus), PNULL);
                /*+COVERITY 5.3.0 - 32724*/
                if(PNULL == macDlSpsStatus_p)
                {
                    ltePanic("Memory Allocation failed in %s\n",__func__);
                }
                /*+COVERITY 5.3.0 - 32724*/
                macDlSpsStatus_p->ueIndex = dlUeContext_p->ueIndex;
                macDlSpsStatus_p->response = FIRST_ACK_NACK;
                if (MAC_FAILURE == ENQUEUE_MAC_DL_SPS_Q(dlSpsStatusQ_gp[internalCellIndex], 
                        macDlSpsStatus_p))
                {
                    ltePanic("Enque Failed\n");
                }
                /* Update Q Stats */
                LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q, 
                        QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex]),
                        internalCellIndex);
            }
            /* SPS_CHG */
            /* - SPS_TDD_Changes */
            /*Fix for __SPR_1988__ START*/

            /*Fix for __SPR_1988__ START*/
            /*Update the lastTASentTick to maintain that UE
              is in UL Sync at the current TTI. This will be
              updated whenever ACK is received for TB*/
            if ((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag
                        == TA_MSG) ||
                    (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag
                     == TA_DRX_MSG))
            {
                dlUeContext_p->lastTASentTick =
                    /* SPR 16474 Fix Start */
                    getCurrentTick() - PHY_UL_DATA_DELAY;
                    /* SPR 16474 Fix End */
            }
            /*Fix for __SPR_1988__ END*/

#ifdef KPI_STATS
            /*Update the pktDelay KPI counters for packets in 
             * DL which are positively acked based on HARQ
             * feedback*/
			/* + KPI_CA */
            /* SPR 12783 Changes Start */
            macUpdateKpiCntrsForPktDelayPerQci(&dlHarqProcess_p->dlHARQTBOneInfo,
					ueIndex, 1, internalCellIndex, recvTTI);
            /* SPR 12783 Changes End */
			/* - KPI_CA */
#endif
            LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ, \
                    "\nCase: TB_TWO_INVALID_TB_ONE_VALID_ACK_ON_TB_ONE\n");
            freeDLHarqProcessTBSpecific(
                    dlUeContext_p,
                    dlHarqProcess_p,
                    TB_ONE,
                    /* + CA_TDD_HARQ_CHANGES */
                    ueInternalCellIndex);
                   /* - CA_TDD_HARQ_CHANGES */
            break;
            /*
               As only TB ONE is valid We need to  perform the processing of TB
               ONE only and hence the following code.
               */
        case TB_TWO_INVALID_TB_ONE_VALID_NACK_ON_TB_ONE:
            LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ, \
                    "\nCase: TB_TWO_INVALID_TB_ONE_VALID_NACK_ON_TB_ONE\n");
#ifdef ACK_NACK_COUNT_STATS
            lteCommonUpdateAckNackStats(TB_ONE, ueIndex, DL_NACK);
#endif
            /* SPR 5124 changes start */
            /* BLER calculation for TB1 */
            /* Cyclomatic Complexity changes - starts here */
            /* + CA_TDD_HARQ_CHANGES */
            calculateDLBlerForNackTB1( globalTick, 
                    ueULContext_p,
                    dlCQIInfo_p,
                    ueIndex,
                    ueInternalCellIndex,
                    dlMIMOInfo_p,
                    servingIndex,
                    bitMaskForAperiodicReport_p);
            /* - CA_TDD_HARQ_CHANGES */
            /* SPR 5124 changes end */
            /* Cyclomatic Complexity changes - ends here */
#ifdef PERF_STATS
            UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_NACK ,
                    dlHarqProcess_p->dlHARQTBOneInfo.mcsIndex);
#endif
            /*Fix for __SPR_1988__ START*/
            /* SPR 2249 Fixes Begins */
            /* + CA_TDD_HARQ_CHANGES */
            /* + SPS_TDD_Changes */
            /*! \code
             * -If (UE State == SPS_ACTIVATION_INITIATED or
             *    SPS_REACTIVATION_INITIATED ) && First NACK is Received,
             *     Push entry in dlSpsStatusQ for strategy.
             * \endcode
             * */
            //SPR_16899_Changes
            if(((SPS_ACTIVATION_INITIATED == spsDlInfo_p->spsState )||
                        ( SPS_REACTIVATION_INITIATED == spsDlInfo_p->spsState))
                    &&(!dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB) 
                    && (FAPI_NACK == (ackNackNode_p->operationType & 0x07))
                    && ( (PNULL != spsDlInfo_p->spsDlSetupInfo_p) && 
                        (dlHarqProcess_p->harqProcessId <
                         spsDlInfo_p->spsDlSetupInfo_p->numberOfConfSpsProcesses) )
              )
            //SPR_16899_Changes
            {
                GET_MEM_FROM_POOL(MacDlSpsStatus, macDlSpsStatus_p, 
                        sizeof(MacDlSpsStatus), PNULL);
                /*coverity-530 CID-25227*/
                if(PNULL == macDlSpsStatus_p)
                {
                    freeDLHarqProcessTBSpecific( dlUeContext_p, 
                            dlHarqProcess_p, 
                            TB_ONE,
                            ueInternalCellIndex);
                    break;
                }
                /*coverity-530 CID-25227*/
                macDlSpsStatus_p->ueIndex = dlUeContext_p->ueIndex;
                macDlSpsStatus_p->response = FIRST_ACK_NACK;
                if (MAC_FAILURE == ENQUEUE_MAC_DL_SPS_Q(dlSpsStatusQ_gp[internalCellIndex], 
                        macDlSpsStatus_p))
                {
                    ltePanic("Enque Failed\n");
                }
                /* Update Q Stats */
                LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q, 
                        QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex]),
                        internalCellIndex);
                if( LTE_TRUE == dlHarqProcess_p->isSpsOnlyPaddingPresent )
                {
                    LTE_MAC_PER_UE_DEBUG_STATS(dlUeContext_p->ueIndex, 
                            globalTick, dlHarqProcess_p->harqProcessId, operation, 
                            dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB,
                            internalCellIndex);
                    /* Free HARQ occupied as the PDU need not be re-transmitted 
                     * as padding is only sent */	
                    freeDLHarqProcessTBSpecific( dlUeContext_p, 
                            dlHarqProcess_p, 
                            TB_ONE,
                            ueInternalCellIndex);
                    break;
                }
            }
            /*!
             * \code
             * -If (UE State == SPS_DEACTIVATION_INITIATED)
             *   -If NACK Received
             *     Push entry in dlSpsStausQ with cause as FIRST_ACK_NACK 
             *   -If Unsure case
             *      Push entry in dlSpsStatusQ with cause as MAX_SPS_HARQ_RETX
             *   Do not try for next retransmission, since SPS_DEACTIVATION is not
             *   associated with any TB.  
             * \endcode
             * */
            else if((SPS_DEACTIVATION_INITIATED == spsDlInfo_p->spsState)
                    && ( dlHarqProcess_p->harqProcessId == spsDlInfo_p->hpId )
                   )
            { 
                GET_MEM_FROM_POOL(MacDlSpsStatus, 
                        macDlSpsStatus_p, 
                        sizeof(MacDlSpsStatus), PNULL);
                /*coverity-530 CID-25227*/
                if(PNULL == macDlSpsStatus_p)
                {
                    freeDLHarqProcessTBSpecific( dlUeContext_p, 
                            dlHarqProcess_p, 
                            TB_ONE,
                            ueInternalCellIndex);
                    break;
                }
                /*coverity-530 CID-25227*/
                macDlSpsStatus_p->ueIndex = dlUeContext_p->ueIndex;
            //SPR_16899_Changes
                if(FAPI_NACK == (ackNackNode_p->operationType & 0x07))
            //SPR_16899_Changes
                {
                    macDlSpsStatus_p->response = FIRST_ACK_NACK;
                }
                else
                {
                    macDlSpsStatus_p->response = MAX_SPS_HARQ_RETX; 
                }
                if (MAC_FAILURE == ENQUEUE_MAC_DL_SPS_Q(dlSpsStatusQ_gp[internalCellIndex], 
                        macDlSpsStatus_p))
                {
                    ltePanic("Enque Failed\n");
                }
                /* Update Q Stats */
                LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q, 
                        QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex]),
                        internalCellIndex);
                LTE_MAC_PER_UE_DEBUG_STATS(dlUeContext_p->ueIndex, 
                        globalTick, dlHarqProcess_p->harqProcessId, operation, 
                        dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB,
                        internalCellIndex);
                /* Free HARQ occupied as the PDU need not be re-transmitted 
                 * since in SPS_DEACTIVATION_INITATED state */	
                freeDLHarqProcessTBSpecific( dlUeContext_p, 
                        dlHarqProcess_p, 
                        TB_ONE,
                        ueInternalCellIndex);
                break; 
            }
            /* + SPR 20690 */
            /* Do not retransmit as only padding is transmitted
             * for SPS occasion */
            if( SPS_SCHEDULING == ackNackNode_p->schUsed && 
                    LTE_TRUE == dlHarqProcess_p->isSpsOnlyPaddingPresent )
            {
                LTE_MAC_PER_UE_DEBUG_STATS(dlUeContext_p->ueIndex, 
                        globalTick, dlHarqProcess_p->harqProcessId, operation, 
                        dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB,
                        internalCellIndex);
                /* Free HARQ occupied as the PDU need not be re-transmitted 
                 * as padding is only sent */	
                freeDLHarqProcessTBSpecific( dlUeContext_p, 
                        dlHarqProcess_p, 
                        TB_ONE,
                        ueInternalCellIndex);
                break;
            }
            /* - SPR 20690 */
            if(SPS_SCHEDULING == ackNackNode_p->schUsed && TRUE == cellSpecificParams_g.\
                    cellConfigAndInitParams_p[internalCellIndex]->initParams_p->spsSysWideParam.enableSps )  
            {
                maxRetxCount = dlUeContext_p->spsDlInfo.spsDlSetupInfo_p->maxSpsHarqRetx;  
            } 
            else
            {
                maxRetxCount = cellSpecificParams_g.cellConfigAndInitParams_p[ueInternalCellIndex]->\
                     cellParams_p->maxHarqRetransmission - 1;     

            }
            /*Fix for __SPR_1988__ START*/
            /* SPR 2249 Fixes Begins */
            if (dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB < maxRetxCount)
            /* - CA_TDD_HARQ_CHANGES */
                /* SPR 2249 Fixes Ends */
            {
                *nackFlag_p = TRUE;
            }
                /* - SPS_TDD_Changes */
            /*
               We now consider the case When TB_One has reached it's
               max re trans limit.
               */
            else
            {
#ifdef PERF_STATS
                gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats.\
                    totalHarqFail++;
#endif
                /* _RLC_AM_MODE_ */
                if (!((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_MSG) ||
                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == DRX_MSG) ||
                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_DRX_MSG) || 
                            (dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                        (dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p))
                { 

                    /* SPR 11402 fix */
                    sendARQIndicationToRLC(
                            ueIndex, 
                            &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex);
                    /* SPR 11402 fix */
                }
                /* SPR 13888 fix start */
#ifdef KPI_STATS
                /* Reset pdcpAckBytes,T1 and T2 at harq Common Index  */
                /* + SPR 17745 */
                lteMacUpdateKpiStatsIPThpDlOnHarqNack(dlHarqProcess_p, ueIndex,
                        recvTTI ,internalCellIndex);
                /* - SPR 17745 */
#endif
                /* SPR 13888 fix end */
 

                freeDLHarqProcess(dlUeContext_p,
                        dlHarqProcess_p,
                        /* + CA_TDD_HARQ_CHANGES */
                        ueInternalCellIndex);
                       /* - CA_TDD_HARQ_CHANGES */
            }
            break;                                     
            /*
               Since we have received the ACK on the only valid TB ie TB
               ONE we just need to free the HARQ PROCESS.
               */

        default:    
            LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ, \
                    "\n Invalid value of Operation received\n");
            /*
               As a part of Invalid scenario error Handling
               we reset the timer array as this would be 
               got re-set by the expire all nodes function.
               */
    }/* End of Switch */                           
}

/*****************************************************************************
 * Function Name  : processDLHarqNonMIMOAckNack 
 * Inputs         : operation - Harq operation,
 *                  ueIndex - UE index,
 *                  globalTick - Current tick,
 *                  harqProcessId - Indicates Harq process Id,
 *                  dlUeContext_p - Pointer to DLUEContext,
 *                  dlHarqProcess_p - Pointer to DLHARQProcess
 *                  recvTTI -  Received TTI in node
 *                  ueInternalCellIndex - Cell-Index at MAC
 *                  servingIndex - serving Index 
 *                  bitMaskForAperiodicReport_p - Bitmap to set trigger for
 *                                aperiodic CQI.
 *                  nackFlag_p - Nack flag to check retx.              
 * Outputs        : None
 * Returns        : None
 * Description    : This function processes the DL Harq ACK/NACK received
 *                  for Non-MIMO mode.
 *****************************************************************************/
static  void processDLHarqNonMIMOAckNack ( UInt8 operation, 
        UInt16        ueIndex, 
        /* SPR 15909 fix start */
        tickType_t        globalTick,
        /* SPR 15909 fix end */
        /* +- SPR 17777 */
        DLUEContext   *dlUeContext_p, 
        DLHARQProcess *dlHarqProcess_p,
        /*SPR 11812 Fix Start*/
        /* + SPS_TDD */
        tddAckNackQueueNode  *ackNackNode_p,
        /* - SPS_TDD */
        /* +- SPR 17777 */
#ifdef KPI_STATS
		UInt8          recvTTI,
#endif
        /* +- SPR 17777 */
        /*SPR 11812 Fix End*/
        /* + CA_TDD_HARQ_CHANGES */
        InternalCellIndex ueInternalCellIndex
        ,InternalCellIndex  servingIndex
        ,UInt8 *bitMaskForAperiodicReport_p        
        ,UInt8 *nackFlag_p
        )
        /* - CA_TDD_HARQ_CHANGES */
{
    /* TDD SB_CQI */
    ULUEContext* ueULContext_p  = PNULL;
    InternalCellIndex internalCellIndex = 0;
    ueULContext_p  = (&ulUECtxInfoArr_g[ueIndex])->ulUEContext_p;
    /* TDD SB_CQI */
    /* + SPS_TDD_Changes */
    UInt8 maxRetxCount = 0;
    LP_SpsDlInfo      spsDlInfo_p      = PNULL;
    spsDlInfo_p = &dlUeContext_p->spsDlInfo;
    LP_MacDlSpsStatus macDlSpsStatus_p = PNULL;
    /* - SPS_TDD_Changes */

    DLCQIInfo   *dlCQIInfo_p   = PNULL;
    DLMIMOInfo  *dlMIMOInfo_p  = PNULL;
    /* + CA_TDD_HARQ_CHANGES */
    if(IS_PCELL_OF_UE(dlUeContext_p, ueInternalCellIndex))
    {
        /* CA_phase2_harq_code Start */
        dlCQIInfo_p  = &(dlUeContext_p->dlCQIInfo);
        /* CA_phase2_harq_code End */
    	dlMIMOInfo_p = &(dlUeContext_p->dlMIMOInfo);
    }
    else 
    {
        dlCQIInfo_p =
           &(dlUeContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo);
        dlMIMOInfo_p = &(dlUeContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlMIMOInfo);
    }     
    internalCellIndex = dlUeContext_p->internalCellIndex;
    /* - CA_TDD_HARQ_CHANGES */
    switch (operation)
    {
        case NACK:
            LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,\
                    "[%s] NACK operation \n current_TX_NB= %d \n",
                    __func__,dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB);

            /* SPR 5124 changes start */
            /* BLER calculation for TB1 */
            /* Cyclomatic Complexity changes - starts here */
            /* CA_phase2_harq_code Start */
            /* + CA_TDD_HARQ_CHANGES */
            calculateDLBlerForNackTB1( globalTick,
                    ueULContext_p,
                    dlCQIInfo_p,
                    ueIndex, 
                    ueInternalCellIndex,
                    dlMIMOInfo_p,
                    servingIndex,
                    bitMaskForAperiodicReport_p);
            /* - CA_TDD_HARQ_CHANGES */
            /* CA_phase2_harq_code End */
            /* Cyclomatic Complexity changes - ends here */
            /* SPR 5124 changes end */
#ifdef PERF_STATS
            UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_NACK ,
                    dlHarqProcess_p->dlHARQTBOneInfo.mcsIndex);
#endif
            /* + SPS_TDD_Changes */
            /*! \code
             * -If (UE State == SPS_ACTIVATION_INITIATED or
             *    SPS_REACTIVATION_INITIATED ) && First NACK is Received,
             *     Push entry in dlSpsStatusQ for strategy.
             * \endcode
             * */
            //SPR_16899_Changes
            if(((SPS_ACTIVATION_INITIATED == spsDlInfo_p->spsState )||
                        ( SPS_REACTIVATION_INITIATED == spsDlInfo_p->spsState))
                    &&(!dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB) 
                    && (FAPI_NACK == (ackNackNode_p->operationType & 0x07))
                    && ( (PNULL != spsDlInfo_p->spsDlSetupInfo_p) && 
                          (dlHarqProcess_p->harqProcessId <
                           spsDlInfo_p->spsDlSetupInfo_p->numberOfConfSpsProcesses) )
              )
            {
                //SPR_16899_Changes
                GET_MEM_FROM_POOL(MacDlSpsStatus, macDlSpsStatus_p, 
                        sizeof(MacDlSpsStatus), PNULL);
                /*coverity-530 CID-25227*/
                if(PNULL == macDlSpsStatus_p)
                {
                    freeDLHarqProcessTBSpecific( dlUeContext_p, 
                            dlHarqProcess_p, 
                            TB_ONE,
                            ueInternalCellIndex);
                    break;
                }
                /*coverity-530 CID-25227*/
                macDlSpsStatus_p->ueIndex = dlUeContext_p->ueIndex;
                macDlSpsStatus_p->response = FIRST_ACK_NACK;

                if (MAC_FAILURE == ENQUEUE_MAC_DL_SPS_Q(dlSpsStatusQ_gp[internalCellIndex], 
                            macDlSpsStatus_p))
                {
                    ltePanic("Enque Failed\n");
                }
                /* Update Q Stats */
                LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q, 
                        QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex]),
                        internalCellIndex);
                if( LTE_TRUE == dlHarqProcess_p->isSpsOnlyPaddingPresent )
                {
                    LTE_MAC_PER_UE_DEBUG_STATS(dlUeContext_p->ueIndex, 
                            globalTick, dlHarqProcess_p->harqProcessId, operation, 
                            dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB,
                            internalCellIndex);
                    /* Free HARQ occupied as the PDU need not be re-transmitted 
                     * as padding is only sent */	
                    freeDLHarqProcessTBSpecific( dlUeContext_p, 
                            dlHarqProcess_p, 
                            TB_ONE,
                            ueInternalCellIndex);
                    break;
                }
            }
            /*!
             * \code
             * -If (UE State == SPS_DEACTIVATION_INITIATED)
             *   -If NACK Received
             *     Push entry in dlSpsStausQ with cause as FIRST_ACK_NACK 
             *   -If Unsure case
             *      Push entry in dlSpsStatusQ with cause as MAX_SPS_HARQ_RETX
             *   Do not try for next retransmission, since SPS_DEACTIVATION is not
             *   associated with any TB.  
             * \endcode
             * */
            else if((SPS_DEACTIVATION_INITIATED == spsDlInfo_p->spsState)
                    && ( dlHarqProcess_p->harqProcessId == spsDlInfo_p->hpId )
                   )
            { 
                GET_MEM_FROM_POOL(MacDlSpsStatus, 
                        macDlSpsStatus_p, 
                        sizeof(MacDlSpsStatus), PNULL);
                /*coverity-530 CID-25227*/
                if(PNULL == macDlSpsStatus_p)
                {
                    freeDLHarqProcessTBSpecific( dlUeContext_p, 
                            dlHarqProcess_p, 
                            TB_ONE,
                            ueInternalCellIndex);
                    break;
                }
                /*coverity-530 CID-25227*/
                macDlSpsStatus_p->ueIndex = dlUeContext_p->ueIndex;
                //SPR_16899_Changes
                if(FAPI_NACK == (ackNackNode_p->operationType & 0x07))
                    //SPR_16899_Changes
                {
                    macDlSpsStatus_p->response = FIRST_ACK_NACK;
                }
                else
                {
                    macDlSpsStatus_p->response = MAX_SPS_HARQ_RETX; 
                }
                if ( MAC_FAILURE == ENQUEUE_MAC_DL_SPS_Q(dlSpsStatusQ_gp[internalCellIndex], 
                            macDlSpsStatus_p))
                {
                    ltePanic("Enque Failed\n");
                }
                /* Update Q Stats */
                LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q, 
                        QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex]),
                        internalCellIndex);
                LTE_MAC_PER_UE_DEBUG_STATS(dlUeContext_p->ueIndex, 
                        globalTick, dlHarqProcess_p->harqProcessId, operation, 
                        dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB,
                        internalCellIndex);
                /* Free HARQ occupied as the PDU need not be re-transmitted 
                 * since in SPS_DEACTIVATION_INITATED state */	
                freeDLHarqProcessTBSpecific( dlUeContext_p, 
                        dlHarqProcess_p, 
                        TB_ONE,
                        ueInternalCellIndex);
                break; 
            }
            /* + SPR 20690 */
            /* Do not retransmit as only padding is transmitted
             * for SPS occasion */
            if( SPS_SCHEDULING == ackNackNode_p->schUsed && 
                    LTE_TRUE == dlHarqProcess_p->isSpsOnlyPaddingPresent )
            {
                LTE_MAC_PER_UE_DEBUG_STATS(dlUeContext_p->ueIndex, 
                        globalTick, dlHarqProcess_p->harqProcessId, operation, 
                        dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB,
                        internalCellIndex);
                /* Free HARQ occupied as the PDU need not be re-transmitted 
                 * as padding is only sent */	
                freeDLHarqProcessTBSpecific( dlUeContext_p, 
                        dlHarqProcess_p, 
                        TB_ONE,
                        ueInternalCellIndex);
                break;
            }
            /* - SPR 20690 */
            if(SPS_SCHEDULING == ackNackNode_p->schUsed && TRUE == cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->initParams_p->spsSysWideParam.enableSps )  
            {
                maxRetxCount = dlUeContext_p->spsDlInfo.spsDlSetupInfo_p->maxSpsHarqRetx;  
            } 
            else
            {
                maxRetxCount = cellSpecificParams_g.cellConfigAndInitParams_p[ueInternalCellIndex]->\
                               cellParams_p->maxHarqRetransmission - 1;
            }
            /*Fix for __SPR_1988__ START*/
            /* SPR 2249 Fixes Begins */
            /* + CA_TDD_HARQ_CHANGES */  
            if (dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB < maxRetxCount)
                /* - CA_TDD_HARQ_CHANGES */
                /* SPR 2249 Fixes Ends */
            {
                *nackFlag_p = TRUE;
            }
            /* - SPS_TDD_Changes */
            /*
               We now consider the case When TB_One has reached it's
               max re trans limit.
               */
            else
            {
#ifdef PERF_STATS
                gMacUePerfStats_p[ueIndex].lteUeMacDLPerfStats.\
                    totalHarqFail++;
#endif
                /* _RLC_AM_MODE_ */
                if (!((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_MSG) ||
                      (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == DRX_MSG) ||
                      (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_DRX_MSG) || 
                      (dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                      (dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p))
                { 

                    /* SPR 11402 fix */
                    sendARQIndicationToRLC(
                            ueIndex, 
                            &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,
                            internalCellIndex);
                    /* SPR 11402 fix */
                } 
                /* SPR 13888 fix start */
#ifdef KPI_STATS
                /* Reset pdcpAckBytes,T1 and T2 at harq Common Index  */
                /* + SPR 17745 */
                lteMacUpdateKpiStatsIPThpDlOnHarqNack(dlHarqProcess_p, ueIndex,
                        recvTTI ,internalCellIndex);
                /* - SPR 17745 */

#endif
                /* SPR 13888 fix end */
                freeDLHarqProcessTBSpecific(
                        dlUeContext_p,
                        dlHarqProcess_p,
                        TB_ONE,
                        /* + CA_TDD_HARQ_CHANGES */
                        ueInternalCellIndex
                        /* - CA_TDD_HARQ_CHANGES */
                        );
            }
            break;
        case ACK:
            /*
               In case of ACK we simply need to free the HARQ
               Process.
               */
#ifdef PERF_STATS
            UPDATE_ACK_NACK_MODSCHEME_STATS(ueIndex, DL_ACK ,
                    dlHarqProcess_p->dlHARQTBOneInfo.mcsIndex);
#endif
            LTE_MAC_UT_LOG(LOG_INFO,Log_DL_HARQ,\
                    "[%s] ACK has been Received\n",__func__);
            /*SPR 11812 Fix Start*/
#ifdef KPI_STATS
            /* This will update the Shared Memory Time and Shared Memory Data */ 
            /* +- SPR 17777 */
            lteMacUpdateKpiStatsIPThpDlOnHarqAck(ueIndex,dlHarqProcess_p, recvTTI
#ifdef TDD_CONFIG
                    , internalCellIndex
#endif
                    );
            /* +- SPR 17777 */
            /* SPR 11402 Fix  */
            /* SPR 16843 start */
            updateKpiStatsForTotalSduAckRcvdDlperQci(&dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex,ueIndex);
            /* SPR 16843 end */
            /* SPR 11402 Fix  */
#endif
            /*SPR 11812 Fix End*/
            /* SPR 5124 changes start */
            /* BLER calculation for TB1 */
            /* Cyclomatic Complexity changes - starts here */
            /* + CA_TDD_HARQ_CHANGES */
            calculateDLBlerForAckTB1( globalTick,
                    ueULContext_p,
                    dlCQIInfo_p,
                    ueIndex,
                    internalCellIndex,
                    servingIndex,
                    bitMaskForAperiodicReport_p,
			        dlMIMOInfo_p);
            /* - CA_TDD_HARQ_CHANGES */
            /* Cyclomatic Complexity changes - ends here */
            /* SPR 5124 changes end */
        /* + SPS_TDD_Changes */
        /* SPS_CHG */
        /*!
         * \code
         *  If (UE State ==  SPS_ACTIVATION_INITIATED 
         *       or SPS_DEACTIVATION_INITIATED)
         *    and First ACK is Received, 
         *    Push entry in dlSpsStatusQ for strategy.
         * \endcode
         * */
        if(((SPS_ACTIVATION_INITIATED == spsDlInfo_p->spsState )||
                    (SPS_REACTIVATION_INITIATED == spsDlInfo_p->spsState ))
                &&(!dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB)
                &&( (PNULL != spsDlInfo_p->spsDlSetupInfo_p) && 
                    (dlHarqProcess_p->harqProcessId <
                     spsDlInfo_p->spsDlSetupInfo_p->numberOfConfSpsProcesses) )
          )
        {
            GET_MEM_FROM_POOL(MacDlSpsStatus, macDlSpsStatus_p, 
                    sizeof(MacDlSpsStatus), PNULL);
            /*+COVERITY 5.3.0 - 32724*/
            if(PNULL == macDlSpsStatus_p)
            {
                ltePanic("Memory Allocation failed in %s\n",__func__);
            }
            /*+COVERITY 5.3.0 - 32724*/
            macDlSpsStatus_p->ueIndex = dlUeContext_p->ueIndex;
            macDlSpsStatus_p->response = FIRST_ACK_NACK;
            if( MAC_FAILURE == ENQUEUE_MAC_DL_SPS_Q(dlSpsStatusQ_gp[internalCellIndex], 
                        macDlSpsStatus_p))
            {
                ltePanic("Enque Failed\n");
            }
            /* Update Q Stats */
            LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q, 
                    QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex]),
                    internalCellIndex);
        }
        else if( (SPS_DEACTIVATION_INITIATED == spsDlInfo_p->spsState) 
                && ( dlHarqProcess_p->harqProcessId == spsDlInfo_p->hpId )   
               )
        {
            GET_MEM_FROM_POOL(MacDlSpsStatus, macDlSpsStatus_p, 
                    sizeof(MacDlSpsStatus), PNULL);
            /*+COVERITY 5.3.0 - 32724*/
            if(PNULL == macDlSpsStatus_p)
            {
                ltePanic("Memory Allocation failed in %s\n",__func__);
            }
            /*+COVERITY 5.3.0 - 32724*/
            macDlSpsStatus_p->ueIndex = dlUeContext_p->ueIndex;
            macDlSpsStatus_p->response = FIRST_ACK_NACK;
            if ( MAC_FAILURE == ENQUEUE_MAC_DL_SPS_Q(dlSpsStatusQ_gp[internalCellIndex], 
                        macDlSpsStatus_p))
            {
                ltePanic("Enque Failed\n");
            }
            /* Update Q Stats */
            LTE_MAC_QUEUE_USED( DL_SPS_STATUS_Q, 
                    QCOUNT_MAC_DL_SPS_Q( dlSpsStatusQ_gp[internalCellIndex]),
                    internalCellIndex);
        }
        /* - SPS_TDD_Changes */

            /*Fix for __SPR_1988__ START*/
            /*Update the lastTASentTick to maintain that UE
              is in UL Sync at the current TTI. This will be
              updated whenever ACK is received for TB*/
            if ((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag
                        == TA_MSG) ||
                    (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag
                     == TA_DRX_MSG))
            {
                dlUeContext_p->lastTASentTick =
                    getCurrentTick() - PHY_UL_DATA_DELAY;
            }
            /*Fix for __SPR_1988__ END*/
#ifdef KPI_STATS
            /*Update the pktDelay KPI counters for packets in 
             * DL which are positively acked based on HARQ
             * feedback*/
			/* + KPI_CA */
            /* SPR 12783 Changes Start */
            macUpdateKpiCntrsForPktDelayPerQci(&dlHarqProcess_p->dlHARQTBOneInfo,
					ueIndex, 1, internalCellIndex, recvTTI);
            /* SPR 12783 Changes End */
			/* - KPI_CA */
#endif
            freeDLHarqProcessTBSpecific(
                    dlUeContext_p,
                    dlHarqProcess_p,
                    TB_ONE,
                    /* + CA_TDD_HARQ_CHANGES */
                    ueInternalCellIndex);
                    /* - CA_TDD_HARQ_CHANGES */
            break;
        default:    
            lteWarning("Invalid value of Operation received In NON MIMO case\n");
    } /* End of switch case */
}

/*************************************************************************
 * Function Name  : assignTddPdschTxMapOfDLHarqPointer
 * Inputs         : ulDlConfiguration
 * Inputs         : internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function will allocate index to the appropriate
 *                  function pointers based on the Special Sub Frame 
 *                  configuration and the UL/DL Configuration.
 ************************************************************************/
void assignTddPdschTxMapOfDLHarqPointer(UInt8 ulDlConfiguration,InternalCellIndex internalCellIndex)
{
    switch(ulDlConfiguration)
    {
        case UL_DL_CONFIG_0:
        {
            tddPdschTxMapOfDLHarq_p[internalCellIndex] = 
                &tddPdschTxMapOfDLHarq_g[UL_DL_CONFIG_0][0];
        }
        break;
        case UL_DL_CONFIG_1:
        {
            tddPdschTxMapOfDLHarq_p[internalCellIndex] = 
                &tddPdschTxMapOfDLHarq_g[UL_DL_CONFIG_1][0];
        }
        break;
        case UL_DL_CONFIG_2:
        {
            tddPdschTxMapOfDLHarq_p[internalCellIndex] = 
                &tddPdschTxMapOfDLHarq_g[UL_DL_CONFIG_2][0];
        }
        break;
        case UL_DL_CONFIG_3:
        {
            tddPdschTxMapOfDLHarq_p[internalCellIndex] = 
                &tddPdschTxMapOfDLHarq_g[UL_DL_CONFIG_3][0];
        }
        break;
        case UL_DL_CONFIG_4:
        {
            tddPdschTxMapOfDLHarq_p[internalCellIndex] = 
                &tddPdschTxMapOfDLHarq_g[UL_DL_CONFIG_4][0];
        }
        break;
        case UL_DL_CONFIG_5:
        {
            tddPdschTxMapOfDLHarq_p[internalCellIndex] = 
                &tddPdschTxMapOfDLHarq_g[UL_DL_CONFIG_5][0];
        }
        break;
        case UL_DL_CONFIG_6:
        {
            tddPdschTxMapOfDLHarq_p[internalCellIndex] = 
                &tddPdschTxMapOfDLHarq_g[UL_DL_CONFIG_6][0];
            /** CA-TDD Changes End **/
        }
        break;
        default:
        {
            /* Some Error Codes */
        }
    }
}
#endif

/*****************************************************************************
 * Function Name  : resetMACDLHarqEntity 
 * Inputs         : dlHarqContext_pp - Ptr to DL Harq Entity
 * Outputs        : None
 * Returns        : None
 * Description    : This function set the DL Harq Status as to be reset so that
 *                  it can be handled in DL Strategy while processing 
 *                  retransmission node. 
 *****************************************************************************/                  
/*SPR 2446 Fix begins*/
/* Review comment fix start MAC_RESET_4 */
 void resetMACDLHarqEntity ( DLHARQEntity **dlHarqContext_pp)
/* Review comment fix end MAC_RESET_4 */
/*SPR 2446 Fix Ends*/
{
    DLHARQEntity  *dlHarqContext_p = PNULL;
    DLHARQProcess *dlHarqProcess_p     = PNULL;
    dlHarqContext_p = *dlHarqContext_pp;
    UInt8 n = 0;

    for (n = 0; n < MAX_HARQ_PROCESS_NB ; n++)            
    {
        dlHarqProcess_p = &dlHarqContext_p->harqProcess[n];
        if(HARQ_PROCESS_FREE != dlHarqProcess_p->harqStatus)
        {
            dlHarqProcess_p->harqStatus = HARQ_PROCESS_RESET;
        }
    }
}
/*ca phase-3 changes start*/
/*****************************************************************************
 * Function Name  : resetMACDLScellHarqEntity 
 * Inputs         : dlHarqContext_pp - Ptr to DL Harq Entity
 * Outputs        : None
 * Returns        : None
 * Description    : This function set the DL Harq Status as to be Free 
 *                  for the scell which is deactivated.
 *****************************************************************************/                  
 void resetMACDLScellHarqEntity ( DLHARQEntity **dlHarqContext_pp,
        /* SPR 12630 Fix start */
        DLUEContext* dlUEContext_p, InternalCellIndex internalCellIndex)
        /* SPR 12630 Fix end */
{
    DLHARQEntity  *dlHarqContext_p = PNULL;
    DLHARQProcess *dlHarqProcess_p     = PNULL;
    dlHarqContext_p = *dlHarqContext_pp;
    UInt8 n = 0;
    /* + SPR_14483 */
    UInt8 maxHarqProcess = MAX_HARQ_PROCESS_NB;   
#ifdef TDD_CONFIG
    maxHarqProcess = dlHarqContext_p->numHARQProcess;
#endif 
    for (n = 0; n < maxHarqProcess; n++)           
    /* - SPR_14483 */
    {
        dlHarqProcess_p = &dlHarqContext_p->harqProcess[n];
        if(HARQ_PROCESS_FREE != dlHarqProcess_p->harqStatus)
        {
            /* SPR 12630 Fix Start */
            freeDLHarqProcess(dlUEContext_p,dlHarqProcess_p,
                    internalCellIndex);
            /* SPR 12630 Fix End */
        }
    }
}
/*ca phase-3 changes end*/
#ifdef FDD_CONFIG
/*****************************************************************************
 * Function Name  : expireDLHarqTimerNodeForTickMiss
 * Inputs         : loopOfNumOfSubFrames
 * Inputs         : internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    :  This function will remove all the nodea in the HARQ Timer
 *                   corresponding to loop number of sub frame, this function puts
 *                   all those Nodes (for which re-transmission is yet reach the
 *                   maximun value) into retransmission queue.This process at 
 *                   time of Tick overshoot occurs
 *****************************************************************************/                  
void expireDLHarqTimerNodeForTickMiss(UInt8 loopOfNumOfSubFrames, InternalCellIndex internalCellIndex)
{
    UInt8 frameNumber = 0;
    UInt16 dlHarqEntityTimerArrCount = 0;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContextInfo* dlUeContextInfoArrayAccess_p = PNULL;
    DLHarqTimerEntityList* dlHarqEntityTimerListAccess_p = PNULL;
    DLUEContext*  dlUeContext_p = PNULL;        
    DLHARQProcess* dlHarqProcess_p = PNULL;
    DLHarqTimerEntityList* dlHarqEntityTimerList_p = PNULL;
    DLHarqTimerNode* dlharqTimerNode_p = PNULL;
    DLHARQEntity*   dlHarqEntity_p = PNULL;
    UInt16 ueIndex = INVALID_UE_ID;
    dlUeContextInfoArrayAccess_p = &dlUECtxInfoArr_g[0];
    dlHarqEntityTimerListAccess_p = &(dlHarqGlobals_g[internalCellIndex].dlHarqTimerEntityList[0]);

    for (frameNumber = 0; frameNumber < loopOfNumOfSubFrames; frameNumber++)
    {
        dlHarqEntityTimerList_p = &dlHarqEntityTimerListAccess_p[frameNumber];
        dlHarqEntityTimerArrCount = dlHarqEntityTimerList_p->count;
        while (dlHarqEntityTimerArrCount--)
        {
            dlharqTimerNode_p = \
                                &(dlHarqEntityTimerList_p->dlHarqTimerNodeArr[dlHarqEntityTimerArrCount]);    
            if(dlharqTimerNode_p->isValid)
            {
                ueIndex = dlharqTimerNode_p->ueIndex;
                dlUeContextInfo_p = &dlUeContextInfoArrayAccess_p[ueIndex];

                if (!dlUeContextInfo_p->pendingDeleteFlag)
                {
                    dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;
                    /* + SPR 11469 Changes */
                    if(IS_PCELL_OF_UE(dlUeContext_p, internalCellIndex))
                    {
                        dlHarqEntity_p= dlUeContext_p->dlHarqContext_p;
                    }
                    else
                    {
                        dlHarqEntity_p= dlUeContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p;
                    }
                    /* - SPR 11469 Changes */
                    dlHarqProcess_p = 
                        &(dlHarqEntity_p->harqProcess[dlharqTimerNode_p->harqProcessId]);
                    
                    LOG_MAC_MSG(MAC_EXPIRE_DL_HARQ_TIMER_NODE, LOGINFO, MAC_DL_HARQ,
                            getCurrentTick(), frameNumber,
                            dlharqTimerNode_p->harqProcessId, ueIndex, TRUE, 
                            DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
                            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

                    /* For MIMO */
                    if (dlUeContext_p->dlMIMOInfo.transmissionMode == TX_MODE_3 ||
                            dlUeContext_p->dlMIMOInfo.transmissionMode == TX_MODE_4 ||
                            /* + TM7_8 Changes Start */
                            ((dlUeContext_p->dlMIMOInfo.transmissionMode == TX_MODE_8) && 
                             (1 < cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                              cellParams_p->numOfTxAnteenas)))
                            /* - TM7_8 Changes End */
                    {   /* When TB1 is Valid */
                        if (dlHarqProcess_p->isTBOneValid)
                        {
                            if (dlHarqProcess_p->isTBTwoValid)
                            {

                                /* _RLC_AM_MODE_ */
                                if (!((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_MSG) ||
                                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == DRX_MSG) ||
                                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_DRX_MSG)|| 
                                            (dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                                        (dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p))
                                { 

                                    /* SPR 11402 fix */
                                    sendARQIndicationToRLC(
                                            ueIndex, 
                                            &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex);
                                    /* SPR 11402 fix */
                                }
                                /* _RLC_AM_MODE_ */
                                if (!((dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_MSG) ||
                                            (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == DRX_MSG) ||
                                            (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_DRX_MSG)|| 
                                            (dlHarqProcess_p->dlHARQTBTwoInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                                            (dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p))
                                {																		   
                                    /* SPR 11402 fix */
                                    sendARQIndicationToRLC(
                                            ueIndex, 
                                            &dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex);
                                    /* SPR 11402 fix */
                                }
                                freeDLHarqProcess(dlUeContext_p,
                                        dlHarqProcess_p,
                                        internalCellIndex);
                            }
                            else
                            {
                                if (!((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_MSG) ||
                                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == DRX_MSG) ||
                                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_DRX_MSG)||
                                            (dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                                            (dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p))
                                { 

                                    /* SPR 11402 fix */
                                    sendARQIndicationToRLC(
                                            ueIndex, 
                                            &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex);
                                    /* SPR 11402 fix */
                                } 
                                freeDLHarqProcessTBSpecific(
                                        dlUeContext_p,
                                        dlHarqProcess_p,
                                        TB_ONE,
                                        internalCellIndex);
                            }
                        }/* When Only TB2 is Valid*/
                        else
                        {
                            if (!((dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_MSG) ||
                                        (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == DRX_MSG) ||
                                        (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_DRX_MSG) ||
                                        (dlHarqProcess_p->dlHARQTBTwoInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG)) &&
                                        (dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p))
                            { 

                                /* SPR 11402 fix */
                                sendARQIndicationToRLC(
                                        ueIndex, 
                                        &dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex);
                                /* SPR 11402 fix */
                            } 
                            freeDLHarqProcessTBSpecific(
                                    dlUeContext_p,
                                    dlHarqProcess_p,
                                    TB_TWO,
                                    internalCellIndex);
                        }
                    }/* For SISO */
                    else
                    {
                        if (!((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_MSG) ||
                                (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == DRX_MSG) ||
                                (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_DRX_MSG)||
                                (dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG)) &&
                                (dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p))
                        { 

                            /* SPR 11402 fix */
                            sendARQIndicationToRLC(
                                    ueIndex, 
                                    &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex);
                            /* SPR 11402 fix */
                        } 
                        freeDLHarqProcessTBSpecific(
                                dlUeContext_p,
                                dlHarqProcess_p,
                                TB_ONE,
                                internalCellIndex);
                    }
                    /* + SPR 11469 Changes */
                    if(IS_PCELL_OF_UE(dlUeContext_p, internalCellIndex))
                    {
                        dlUeContext_p->dlHarqContext_p->dlHarqTimerNodeArrrayIndex[frameNumber] = \
                                                                                    INVALID_COUNT;
                    }
                    else
                    {
                        dlUeContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlHarqContext_p->\
                                dlHarqTimerNodeArrrayIndex[frameNumber] = INVALID_COUNT;
                    }
                    /* - SPR 11469 Changes */
                }
		/* SPR# 3860 Changes Starts */
                dlharqTimerNode_p->harqProcessId = INVALID_HARQ_ID;
		/* SPR# 3860 Changes Ends */
                dlharqTimerNode_p->isValid = HARQ_TIMER_NODE_INVALID;
                dlharqTimerNode_p->ueIndex = INVALID_UE_ID;
            }
            dlHarqEntityTimerList_p->count = 0;
        }
    }
}
#endif //FDD_CONFIG


/* SPR 5531 changes start */
/*****************************************************************************
 * Function Name  : freePendingMemoryPools 
 * Inputs         : internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function will free the pending mem pools.
 *****************************************************************************/                  
void freePendingMemoryPools(InternalCellIndex internalCellIndex)
{
    UInt16 queueCount = 0;
    void* dlPendingMemPoolsFree_p = PNULL;
    queueCount = COUNT_PENDING_DL_MEMORY_POOL_FREE_Q(internalCellIndex);

    /* +SPR 15234 */
    UInt32 returnType = MAC_FAILURE;

    while(queueCount)
    {
        returnType = DEQUEUE_PENDING_DL_MEMORY_POOL_FREE_Q(DLHARQTBInfo, (void**)&dlPendingMemPoolsFree_p,internalCellIndex);
        if(MAC_FAILURE == returnType)
        {
            ltePanic("In [%s], POP Failed!! No Node in Queue, queueCount[%d]\n",__func__,queueCount);
        }
        if(MAC_PARTIAL_SUCCESS == returnType)
        {
            lteWarning("Simultaneous Push operation runnig for QueueCount[%d] at TTI[%d]\n", queueCount, getCurrentTick()); 
            /* Pending Mempool deletion can be done in later TTI but hence returning in case simulatanenous access of same queue occur 
             * in l2 HP and LP thread */
            return;
        }

        dlPendingMemPoolsFree_p = (void *)dlPendingMemPoolsFree_p;
        if(dlPendingMemPoolsFree_p)
        {
            msgFree(dlPendingMemPoolsFree_p);
            dlPendingMemPoolsFree_p = PNULL;
        }
        /* -SPR 15234 */
        else
        {
            ltePanic("Failure cleaning up memory\n");
        }
    	queueCount--;
    }
}
/* SPR 5531 changes end */

/* Cyclomatic Complexity changes - starts here */
/*****************************************************************************
 * Function Name  : calculateDLBlerForAckTB1 
 * Inputs         : currentGlobalTick,
 *                  ueULContext_p - pointer to ULUEContext,
 *                  dlUeContext_p - pointer to DLUEContext,
 *                  ueIndex
 *                  internalCellIndex - Cell-Index at MAC,
 *                  ueServCellIndex,
 *                  bitMaskForAperiodicReport_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function calculates the DL BLER for ACK TB1.
 *****************************************************************************/                  
        /* SPR 15909 fix start */
 void calculateDLBlerForAckTB1 (tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        ULUEContext     *ueULContext_p,
        DLCQIInfo        *dlCQIInfo,
        UInt16            ueIndex,
        InternalCellIndex internalCellIndex,
        /* CA_phase2_csi_code Start */
        UInt8 ueServCellIndex,
        UInt8 *bitMaskForAperiodicReport_p,
    	DLMIMOInfo       *dlMIMOInfo	
        /* CA_phase2_csi_code End */
        )
{

    /* EICIC + */
    UInt8  absSubFrameNum     = 0;
    UInt32 numBitsAbsPattern  =  0;


#ifdef FDD_CONFIG
    numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
#else
    if (UL_DL_CONFIG_0 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
    }
    else if (UL_DL_CONFIG_6 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
    }
    else
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
    }
#endif

    absSubFrameNum = ((((eicicHarqSystemframeNumber_g )*MAX_SUB_FRAME) +(eicicHarqSubframeNumber_g)) %numBitsAbsPattern) ;
    /* Check if subframe for which ack is received was ABS or not */

    if (LTE_TRUE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum]) /* Work as victim*/
    {  
        dlCQIInfo->previousDLBLERForCW0_2 = 
            dlCQIInfo->previousDLBLERForCW0_2 - ACK_STEP_SIZE;
        dlCQIInfo->dlBlerCalculatedCounterForCW0_2++; 
    }
    else
    {
        dlCQIInfo->previousDLBLERForCW0 = 
             dlCQIInfo->previousDLBLERForCW0 - ACK_STEP_SIZE;
        /* Increment the DL BLER counter */
        dlCQIInfo->dlBlerCalculatedCounterForCW0++; 
    }

    /* SPR 13571 fix start */
     calculateDLBlerForTB1 (currentGlobalTick,ueULContext_p,dlCQIInfo,ueIndex,
             internalCellIndex,dlMIMOInfo
             ,ueServCellIndex
             ,bitMaskForAperiodicReport_p
             );
    /* SPR 13571 fix end */
}

/*****************************************************************************
 * Function Name  : calculateDLBlerForAckTB2
 * Inputs         : currentGlobalTick,
 *                  ueULContext_p - pointer to ULUEContext,
 *                  dlCQIInfo - pointer to DLCQIInfo,
 *                  dlMIMOInfo - pointer to DLMIMOInfo,
 *                  ueIndex
 *                  internalCellIndex - Cell-Index at MAC,
 *                  ueServCellIndex,
 *                  bitMaskForAperiodicReport_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function calculates the DL BLER for ACK TB2.
 *****************************************************************************/                  
        /* SPR 15909 fix start */
 void calculateDLBlerForAckTB2 (tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        ULUEContext     *ueULContext_p,
        DLCQIInfo        *dlCQIInfo,
        /* +- SPR 17777*/
        UInt16            ueIndex,
        InternalCellIndex internalCellIndex,
        /* CA_phase2_csi_code Start */
        UInt8 ueServCellIndex,
        UInt8 *bitMaskForAperiodicReport_p
        /* CA_phase2_csi_code End */
        )
{
    /* EICIC +*/
    UInt8  absSubFrameNum     = 0;
    UInt32 numBitsAbsPattern = 0;



#ifdef FDD_CONFIG
    numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
#else
    if (UL_DL_CONFIG_0 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
    }
    else if (UL_DL_CONFIG_6 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
    }
    else
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
    }
#endif

    absSubFrameNum = ((((eicicHarqSystemframeNumber_g )*MAX_SUB_FRAME) +(eicicHarqSubframeNumber_g)) %numBitsAbsPattern) ;

    if (LTE_TRUE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum]) /* Work as victim*/
    {
        /*EICIC*/
        dlCQIInfo->previousDLBLERForCW1_2 = 
            dlCQIInfo->previousDLBLERForCW1_2 - ACK_STEP_SIZE;
        /* Increment the DL BLER counter */
        dlCQIInfo->dlBlerCalculatedCounterForCW1_2++; 
        /*EICIC*/
    }
    else
    {
    dlCQIInfo->previousDLBLERForCW1 = 
        dlCQIInfo->previousDLBLERForCW1 - ACK_STEP_SIZE;
    /* Increment the DL BLER counter */
    dlCQIInfo->dlBlerCalculatedCounterForCW1++; 
    }
    /* SPR 13571 fix start */
    /* +- SPR 17777 */
    calculateDLBlerForTB2
        (currentGlobalTick,ueULContext_p,dlCQIInfo,
        ueIndex,internalCellIndex
            ,ueServCellIndex
            ,bitMaskForAperiodicReport_p
            );
    /* +- SPR 17777 */
    /* SPR 13571 fix end */
}

/*****************************************************************************
 * Function Name  : calculateDLBlerForNackTB1
 * Inputs         : currentGlobalTick,
 *                  ueULContext_p - pointer to ULUEContext,
 *                  dlCQIInfo - pointer to DLCQIInfo,
 *                  dlMIMOInfo - pointer to DLMIMOInfo,
 *                  ueIndex
 *                  internalCellIndex - Cell-Index at MAC,
 *                  ueServCellIndex,
 *                  bitMaskForAperiodicReport_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function calculates the DL BLER for NACK TB1.
 *****************************************************************************/                  
        /* SPR 15909 fix start */
 void calculateDLBlerForNackTB1 (tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        ULUEContext     *ueULContext_p,
        DLCQIInfo        *dlCQIInfo,
        UInt16            ueIndex,
        InternalCellIndex internalCellIndex,
        DLMIMOInfo       *dlMIMOInfo
        /* CA_phase2_csi_code Start */
        ,UInt8 ueServCellIndex,
        UInt8 *bitMaskForAperiodicReport_p
        /* CA_phase2_csi_code End */
        )
{
    /* EICIC +*/
    UInt8  absSubFrameNum     = 0;
    UInt8 numBitsAbsPattern = 0;



#ifdef FDD_CONFIG
    numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
#else
    if (UL_DL_CONFIG_0 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
    }
    else if (UL_DL_CONFIG_6 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
    }
    else
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
    }
#endif

    absSubFrameNum = ((((eicicHarqSystemframeNumber_g )*MAX_SUB_FRAME) +(eicicHarqSubframeNumber_g)) %numBitsAbsPattern) ;

    if(LTE_TRUE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum])
    {
        /*EICIC*/
        dlCQIInfo->previousDLBLERForCW0_2 = 
            dlCQIInfo->previousDLBLERForCW0_2 + NACK_STEP_SIZE;
        /* Increment the DL BLER counter */
        dlCQIInfo->dlBlerCalculatedCounterForCW0_2++; 
        /*EICIC*/
    }
    else
    {
        dlCQIInfo->previousDLBLERForCW0 = 
            dlCQIInfo->previousDLBLERForCW0 + NACK_STEP_SIZE;
        /* Increment the DL BLER counter */
        dlCQIInfo->dlBlerCalculatedCounterForCW0++; 
    }
    /* SPR 13571 fix start */
    calculateDLBlerForTB1
        (currentGlobalTick,ueULContext_p,dlCQIInfo,ueIndex,internalCellIndex,dlMIMOInfo
            ,ueServCellIndex
            ,bitMaskForAperiodicReport_p
            );
    /* SPR 13571 fix end */
  /* CA_phase2_harq_code End */  
}

/*****************************************************************************
 * Function Name  : calculateDLBlerForNackTB2
 * Inputs         : currentGlobalTick,
 *                  ueULContext_p - pointer to ULUEContext,
 *                  dlUeContext_p - pointer to DLUEContext,
 *                  ueIndex,
 *                  internalCellIndex - Cell-Index at MAC,
 *                  ueServCellIndex,
 *                  bitMaskForAperiodicReport_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function calculates the DL BLER for NACK TB2.
 *****************************************************************************/                  
        /* SPR 15909 fix start */
 void calculateDLBlerForNackTB2 (tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        ULUEContext     *ueULContext_p,
        DLCQIInfo        *dlCQIInfo,
        /* +- SPR 17777*/
        UInt16            ueIndex,
        InternalCellIndex internalCellIndex
        /* CA_phase2_csi_code Start */
        ,UInt8 ueServCellIndex,
        UInt8 *bitMaskForAperiodicReport_p
        /* CA_phase2_csi_code End */
        )
{
    /* EICIC +*/
    UInt8  absSubFrameNum     = 0;
    UInt8 numBitsAbsPattern = 0;
#ifdef FDD_CONFIG
    numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
#else
    if (UL_DL_CONFIG_0 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN;
    }
    else if (UL_DL_CONFIG_6 == cellSpecificParams_g.cellConfigAndInitParams_p\
            [internalCellIndex]->cellParams_p->subFrameAssign)
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG6;
    }
    else
    {
        numBitsAbsPattern = NUM_BITS_ABS_PATTERN_CONFIG1_5;
    }
#endif

    absSubFrameNum = ((((eicicHarqSystemframeNumber_g )*MAX_SUB_FRAME) +(eicicHarqSubframeNumber_g)) %numBitsAbsPattern) ;
    if(LTE_TRUE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum])
    {

        /*EICIC*/
        dlCQIInfo->previousDLBLERForCW1_2 = 
            dlCQIInfo->previousDLBLERForCW1_2 + NACK_STEP_SIZE;
        /* Increment the DL BLER counter */
        dlCQIInfo->dlBlerCalculatedCounterForCW1_2++; 
        /*EICIC*/
    }
    else
    {
        dlCQIInfo->previousDLBLERForCW1 = 
            dlCQIInfo->previousDLBLERForCW1 + NACK_STEP_SIZE;
        /* Increment the DL BLER counter */
        dlCQIInfo->dlBlerCalculatedCounterForCW1++;
    }
    /* SPR 13571 fix start */
    /* +- SPR 17777 */
    calculateDLBlerForTB2(currentGlobalTick,ueULContext_p,dlCQIInfo,
    ueIndex,internalCellIndex
            ,ueServCellIndex
            ,bitMaskForAperiodicReport_p
            );
    /* SPR 13571 fix end */
    /* CA_phase2_harq_code End */  
}
/* SPR 13571 fix start */
/*****************************************************************************
 * Function Name  : calculateDLBlerForTB1
 * Inputs         : currentGlobalTick,
 *                  ueULContext_p - pointer to ULUEContext,
 *                  dlCQIInfo - pointer to DL CQI Info
                    ueIndex
                    internalCellIndex
                    ueServCellIndex - serving cell index
                    bitMaskForAperiodicReport_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function calculates the DL BLER for TB1.
 *****************************************************************************/                  

        /* SPR 15909 fix start */
void calculateDLBlerForTB1 (tickType_t currentGlobalTick,
        /* SPR 15909 fix start */
        ULUEContext     *ueULContext_p,
        DLCQIInfo        *dlCQIInfo,
        UInt16            ueIndex,
        InternalCellIndex internalCellIndex,
        DLMIMOInfo       *dlMIMOInfo
        ,UInt8 ueServCellIndex,
        UInt8 *bitMaskForAperiodicReport_p
        )
{
    PucchConfig pucchConfigInfo = {0};
    UInt8  absSubFrameNum     = 0;
    UInt32 numBitsAbsPattern  =  0;
    if(IS_PCELL_OF_UE(ueULContext_p , internalCellIndex))
    {
        pucchConfigInfo = ueULContext_p->pucchConfigInfo ;
    }
    else
    {
        pucchConfigInfo =
            ueULContext_p->ulUeScellContext_p[ueServCellIndex]->scellpucchConfigInfo;
    }

    /* Check for DL BLER counter for MAX DL BLER Samples */
    /* EICIC + */

    /*Complexity Start*/ 
    numBitsAbsPattern=eicicFindNumOfBitsinABSPattern(internalCellIndex);
    /*Complexity End*/

    absSubFrameNum = ((((eicicHarqSystemframeNumber_g )*MAX_SUB_FRAME) +(eicicHarqSubframeNumber_g)) %numBitsAbsPattern) ;
    /* Check if subframe for which ack is received was ABS or not */

    if (LTE_TRUE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum]) /* Work as victim*/
    {  
        if(MAX_NUM_OF_DL_BLER_SAMPLES == 
                dlCQIInfo->dlBlerCalculatedCounterForCW0_2) 
        { 
            dlCQIInfo->currentDLBLERForCW0_2 = TARGET_DL_BLER_VALUE + 
                (dlCQIInfo->previousDLBLERForCW0_2/MAX_NUM_OF_DL_BLER_SAMPLES);
            /* Validate DL BLER value */
            VALIDATE_DL_BLER_VALUE(dlCQIInfo->currentDLBLERForCW0_2); 
            dlCQIInfo->dlBlerCalculatedCounterForCW0_2 = 0; 
            dlCQIInfo->previousDLBLERForCW0_2 = 0;
            dlCQIInfo->cqiCorrectionFactorForCW0_2 = 
                dlCQIInfo->cqiCorrectionFactorForCW0_2 + 
                dlBlerCorrectionFactor_g[dlCQIInfo->currentDLBLERForCW0_2];
            /* Validate CQI Correction Factor */
            VALIDATE_CQI_CORRECTION_FACTOR(dlCQIInfo->cqiCorrectionFactorForCW0_2); 

            /* Check for Aperiodic CQI mode */
            if ( (APERIODIC_CQI_REPORTING_MODE == dlCQIInfo->cqiTypeIndicator) || 
                    (PERIODIC_APERIODIC_MODE == dlCQIInfo->cqiTypeIndicator) ) 
            {
                if( ((dlCQIInfo->currentDLBLERForCW0_2 - TARGET_DL_BLER_VALUE) > 
                            DL_BLER_THRESHOLD) &&  
                        (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI
                                              + APERIODIC_CQI_REQ_OFFSET)) && 
                        (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI 
                                              + APERIODIC_CQI_REQ_OFFSET)) ) 
                {
                    /* CA_phase2_csi_code Start */
                    /* As part of CA FDD changes,15.5.9 b)of Arch doc, single entry
                     * of ulGrantAperiodicCqiQueue_gp(of PCell)if for CA UEs
                     * condition to trigger Aperiodic CSI occurs for single or both
                     * cells and update node variable aperiodicCQIToBeRequested */
                    *bitMaskForAperiodicReport_p |= servingCellBitMask_g[ueServCellIndex]; 
                    /* CA_phase2_csi_code End */
                    dlCQIInfo->aperiodicCqiRequestTTI = currentGlobalTick; 
                }
                else if( (((dlCQIInfo->currentDLBLERForCW0_2 - 
                                    dlCQIInfo->prevDlBlerCalculatedForCW0_2) > MIN_DL_BLER_DIFF_VALUE) && 
                            (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI 
                                                  + APERIODIC_CQI_REQ_OFFSET)) && 
                            (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI
                                                  + APERIODIC_CQI_REQ_OFFSET))) || 
                        (((TX_MODE_3 == dlMIMOInfo->transmissionMode) ||
                          (TX_MODE_4 == dlMIMOInfo->transmissionMode)||
                          (TX_MODE_8 == dlMIMOInfo->transmissionMode)) && 
                         (RANK_INDICATOR_1 == dlMIMOInfo->dlRi) &&
                         (dlCQIInfo->widebandCQICodeWordOne_2 >= SIXTY_FOUR_QAM_CQI) &&
                         (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI
                                               + MS_TO_TRIGGER_AP_CQI)) && 
                         (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI 
                                               + MS_TO_TRIGGER_AP_CQI))) ) 
                {
                    /* SPR 13571 fix end */
                    /* CA_phase2_csi_code Start */
                    /* As part of CA FDD changes,15.5.9 b)of Arch doc, single entry
                     * of ulGrantAperiodicCqiQueue_gp(of PCell)if for CA UEs
                     * condition to trigger Aperiodic CSI occurs for single or both
                     * cells and update node variable aperiodicCQIToBeRequested */
                    *bitMaskForAperiodicReport_p |= servingCellBitMask_g[ueServCellIndex];
                    /* CA_phase2_csi_code End */
                    dlCQIInfo->aperiodicCqiRequestTTI = currentGlobalTick; 

                }
            }
            else if(PERIODIC_APERIODIC_MODE == dlCQIInfo->cqiTypeIndicator) 
            {
                if( ((dlCQIInfo->currentDLBLERForCW0_2 - TARGET_DL_BLER_VALUE) > 
                            DL_BLER_THRESHOLD) &&  
                        (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI
                                              + APERIODIC_CQI_REQ_OFFSET)) && 
                        (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI 
                                              + APERIODIC_CQI_REQ_OFFSET)) ) 
                {
                    /* As part of CA FDD changes,15.5.9 b)of Arch doc, single entry
                     * of ulGrantAperiodicCqiQueue_gp(of PCell)if for CA UEs
                     * condition to trigger Aperiodic CSI occurs for single or both
                     * cells and update node variable aperiodicCQIToBeRequested */
                    *bitMaskForAperiodicReport_p |= servingCellBitMask_g[ueServCellIndex];
                    dlCQIInfo->aperiodicCqiRequestTTI = currentGlobalTick; 

                }
                /* SPR 13571 fix start */
                else if( ((dlCQIInfo->widebandCQICodeWordOne_2) && 
                            (dlCQIInfo->widebandCQICodeWordOne_2 < (MAX_CQI_INDEX - 1))) && 
                        /* SPR 13571 fix end */
                        (dlCQIInfo->currentDLBLERForCW0_2 < MIN_DL_BLER_THRESHOLD) &&  
                        (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI 
                                              + (10 * ueULContext_p->pucchConfigInfo.cqiPmiRiInfo.widebandPeriodicity))) && 
                        (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI 
                                              + (10 * ueULContext_p->pucchConfigInfo.cqiPmiRiInfo.widebandPeriodicity))) ) 
                {

                    /* As part of CA FDD changes,15.5.9 b)of Arch doc, single entry
                     * of ulGrantAperiodicCqiQueue_gp(of PCell)if for CA UEs
                     * condition to trigger Aperiodic CSI occurs for single or both
                     * cells and update node variable aperiodicCQIToBeRequested */
                    *bitMaskForAperiodicReport_p |= servingCellBitMask_g[ueServCellIndex];
                    dlCQIInfo->aperiodicCqiRequestTTI = currentGlobalTick; 

                }

            }



        }
        dlCQIInfo->prevDlBlerCalculatedForCW0_2 = dlCQIInfo->currentDLBLERForCW0_2;
    }
    else
    {
        if(MAX_NUM_OF_DL_BLER_SAMPLES == 
                dlCQIInfo->dlBlerCalculatedCounterForCW0) 
        { 
            dlCQIInfo->currentDLBLERForCW0 = TARGET_DL_BLER_VALUE + 
                (dlCQIInfo->previousDLBLERForCW0/MAX_NUM_OF_DL_BLER_SAMPLES);
            /* Validate DL BLER value */
            VALIDATE_DL_BLER_VALUE(dlCQIInfo->currentDLBLERForCW0); 
            dlCQIInfo->dlBlerCalculatedCounterForCW0 = 0; 
            dlCQIInfo->previousDLBLERForCW0 = 0;
            dlCQIInfo->cqiCorrectionFactorForCW0 = 
                dlCQIInfo->cqiCorrectionFactorForCW0 + 
                dlBlerCorrectionFactor_g[dlCQIInfo->currentDLBLERForCW0];
            /* Validate CQI Correction Factor */
            VALIDATE_CQI_CORRECTION_FACTOR(dlCQIInfo->cqiCorrectionFactorForCW0); 

            /* Check for Aperiodic CQI mode */
            /* SPR 9427 changes start */\
                if(APERIODIC_CQI_REPORTING_MODE == dlCQIInfo->cqiTypeIndicator)  
                    /* SPR 9427 changes end */\
                    {
                        if( ((dlCQIInfo->currentDLBLERForCW0 - TARGET_DL_BLER_VALUE) > 
                                    DL_BLER_THRESHOLD) &&  
                                (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI
                                                      + APERIODIC_CQI_REQ_OFFSET)) && 
                                (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI 
                                                      + APERIODIC_CQI_REQ_OFFSET)) ) 
                        {
                            /* CA_phase2_csi_code Start */
                            /* As part of CA FDD changes,15.5.9 b)of Arch doc, single entry
                             * of ulGrantAperiodicCqiQueue_gp(of PCell)if for CA UEs
                             * condition to trigger Aperiodic CSI occurs for single or both
                             * cells and update node variable aperiodicCQIToBeRequested */
                            *bitMaskForAperiodicReport_p |= servingCellBitMask_g[ueServCellIndex]; 
                            /* CA_phase2_csi_code End */
                            dlCQIInfo->aperiodicCqiRequestTTI = currentGlobalTick; 
                        }
                        /* SPR 9427 changes start */
                        /* SPR 13571 fix start */
                        else if( (((dlCQIInfo->currentDLBLERForCW0 - 
                                            dlCQIInfo->prevDlBlerCalculatedForCW0) > MIN_DL_BLER_DIFF_VALUE) && 
                                    (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI 
                                                          + APERIODIC_CQI_REQ_OFFSET)) && 
                                    (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI
                                                          + APERIODIC_CQI_REQ_OFFSET))) || 
                                (((TX_MODE_3 == dlMIMOInfo->transmissionMode) ||
                                  (TX_MODE_4 == dlMIMOInfo->transmissionMode)||
                                  (TX_MODE_8 == dlMIMOInfo->transmissionMode)) && 
                                 (RANK_INDICATOR_1 == dlMIMOInfo->dlRi) &&
                                 (dlCQIInfo->widebandCQICodeWordOne >= SIXTY_FOUR_QAM_CQI) &&
                                 (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI
                                                       + MS_TO_TRIGGER_AP_CQI)) && 
                                 (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI 
                                                       + MS_TO_TRIGGER_AP_CQI))) ) 
                        {
                            /* SPR 13571 fix end */
                            /* CA_phase2_csi_code Start */
                            *bitMaskForAperiodicReport_p |= servingCellBitMask_g[ueServCellIndex]; 
                            /* CA_phase2_csi_code End */
                            dlCQIInfo->aperiodicCqiRequestTTI = currentGlobalTick; 
                        }
                    }
                else if(PERIODIC_APERIODIC_MODE == dlCQIInfo->cqiTypeIndicator) 
                {
                    if( ((dlCQIInfo->currentDLBLERForCW0 - TARGET_DL_BLER_VALUE) > 
                                DL_BLER_THRESHOLD) &&  
                            (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI
                                                  + APERIODIC_CQI_REQ_OFFSET)) && 
                            (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI 
                                                  + APERIODIC_CQI_REQ_OFFSET)) ) 
                    {
                        *bitMaskForAperiodicReport_p |= servingCellBitMask_g[ueServCellIndex]; 
                        dlCQIInfo->aperiodicCqiRequestTTI = currentGlobalTick; 
                    }
                    /* SPR 13571 fix start */
                    else if( ((dlCQIInfo->widebandCQICodeWordOne) && 
                                (dlCQIInfo->widebandCQICodeWordOne < (MAX_CQI_INDEX - 1))) && 
                            /* SPR 13571 fix end */
                            (dlCQIInfo->currentDLBLERForCW0 < MIN_DL_BLER_THRESHOLD) &&  
                            /* + coverity 96998 */
                            (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI 
                                                  + (10 * (tickType_t)pucchConfigInfo.cqiPmiRiInfo.widebandPeriodicity))) && 
                            (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI 
                                                  + (10 * (tickType_t)pucchConfigInfo.cqiPmiRiInfo.widebandPeriodicity))) ) 
                        /* - coverity 96998 */
                    {
                        *bitMaskForAperiodicReport_p |= servingCellBitMask_g[ueServCellIndex]; 
                        dlCQIInfo->aperiodicCqiRequestTTI = currentGlobalTick; 
                    }
                }
        }
        /* SPR 9427 changes end */
        /* SPR 13571 fix start */
        /* Update DL BLER for Code Word 0 */
        dlCQIInfo->prevDlBlerCalculatedForCW0 = dlCQIInfo->currentDLBLERForCW0;
    }
    /* Update DL BLER for Code Word 0 */
    /* CA_phase2_csi_code Start */
    if((MAX_NUM_OF_DL_BLER_SAMPLES == dlCQIInfo->dlBlerCalculatedCounterForCW0)||
            (MAX_NUM_OF_DL_BLER_SAMPLES == dlCQIInfo->dlBlerCalculatedCounterForCW0_2))		
    {
        if(VICTIM_UE == ueULContext_p->userType)
        {
            /*Complexity Start*/
            updateBlerCW0(internalCellIndex,ueULContext_p, dlCQIInfo->currentDLBLERForCW0_2,
                    ueServCellIndex,
                    ueIndex);
            /*Complexity End*/                        
        }
        else if((LTE_FALSE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum]) && (NON_VICTIM_UE == ueULContext_p->userType))
        {   /* For Non Victim It will be as usual*/
            /*Complexity Start*/
            updateBlerCW0(internalCellIndex,ueULContext_p, dlCQIInfo->currentDLBLERForCW0,
                    ueServCellIndex,
                    ueIndex);
            /*Complexity End*/

        }
        else if((LTE_TRUE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum]) && (NON_VICTIM_UE == ueULContext_p->userType))
        {   /* For Non Victim It will be as usual*/
            /*Complexity Start*/
            updateBlerCW0_2(internalCellIndex,ueULContext_p, dlCQIInfo->currentDLBLERForCW0_2,
                        ueServCellIndex,
                        ueIndex);
            /*Complexity End*/
        }
        /*-EICIC */
        /* CA_phase2_csi_code End */
        /* CA_phase2_csi_code End */
        /* SPR 13571 fix end */
    }
} 

/*****************************************************************************
 * Function Name  : calculateDLBlerForTB2
 * Inputs         : currentGlobalTick,
 *                  ueULContext_p - pointer to ULUEContext,
 *                  dlCQIInfo - pointer to DL CQI Info
 *                  dlMIMOInfo - pointer to DL MIMO Info
 *                  ueIndex
 *                  internalCellIndex
 *                  ueServCellIndex - serving cell index
 *                  bitMaskForAperiodicReport_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function calculates the DL BLER for TB2.
 *****************************************************************************/                  

        /* SPR 15909 fix start */
void calculateDLBlerForTB2 (tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        ULUEContext     *ueULContext_p,
        DLCQIInfo        *dlCQIInfo,
        /* +- SPR 17777 */
        UInt16            ueIndex,
        InternalCellIndex internalCellIndex
        ,UInt8 ueServCellIndex,
        UInt8 *bitMaskForAperiodicReport_p
        )
{
    PucchConfig pucchConfigInfo = {0};
    /* EICIC +*/
    UInt8  absSubFrameNum     = 0;
    UInt32 numBitsAbsPattern = 0;

    if(IS_PCELL_OF_UE(ueULContext_p , internalCellIndex))
    {
        pucchConfigInfo = ueULContext_p->pucchConfigInfo ;
    }
    else
    {
        pucchConfigInfo =
            ueULContext_p->ulUeScellContext_p[ueServCellIndex]->scellpucchConfigInfo;
    }
    /*Complexity Start*/
    numBitsAbsPattern=eicicFindNumOfBitsinABSPattern(internalCellIndex);
    /*Complexity End*/

    absSubFrameNum = ((((eicicHarqSystemframeNumber_g )*MAX_SUB_FRAME) +(eicicHarqSubframeNumber_g)) %numBitsAbsPattern) ;

    if (LTE_TRUE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum]) /* Work as victim*/
    {
        if(MAX_NUM_OF_DL_BLER_SAMPLES == 
                dlCQIInfo->dlBlerCalculatedCounterForCW1_2) 
        { 
            dlCQIInfo->currentDLBLERForCW1_2 = TARGET_DL_BLER_VALUE + 
                (dlCQIInfo->previousDLBLERForCW1_2/MAX_NUM_OF_DL_BLER_SAMPLES);
            /* Validate DL BLER value */
            VALIDATE_DL_BLER_VALUE(dlCQIInfo->currentDLBLERForCW1_2); 
            dlCQIInfo->dlBlerCalculatedCounterForCW1_2 = 0; 
            dlCQIInfo->previousDLBLERForCW1_2 = 0;
            dlCQIInfo->cqiCorrectionFactorForCW1_2 = 
                dlCQIInfo->cqiCorrectionFactorForCW1_2 + 
                dlBlerCorrectionFactor_g[dlCQIInfo->currentDLBLERForCW1_2];
            /* Validate CQI Correction Factor */
            VALIDATE_CQI_CORRECTION_FACTOR(dlCQIInfo->cqiCorrectionFactorForCW1_2); 

            /* Check for Aperiodic CQI mode */
            if ( (APERIODIC_CQI_REPORTING_MODE == dlCQIInfo->cqiTypeIndicator) || 
                    (PERIODIC_APERIODIC_MODE == dlCQIInfo->cqiTypeIndicator) ) 
            {
                if( ((dlCQIInfo->currentDLBLERForCW1_2 - TARGET_DL_BLER_VALUE) > 
                            DL_BLER_THRESHOLD) &&  
                        (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI
                                              + APERIODIC_CQI_REQ_OFFSET)) && 
                        (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI 
                                              + APERIODIC_CQI_REQ_OFFSET)) ) 
                {
                    /* CA_phase2_csi_code Start */
                    /* As part of CA FDD changes,15.5.9 b)of Arch doc, single entry
                     * of ulGrantAperiodicCqiQueue_gp(of PCell)if for CA UEs
                     * condition to trigger Aperiodic CSI occurs for single or both
                     * cells and update node variable aperiodicCQIToBeRequested */
                    *bitMaskForAperiodicReport_p |= servingCellBitMask_g[ueServCellIndex]; 
                    /* CA_phase2_csi_code End */
                    dlCQIInfo->aperiodicCqiRequestTTI = currentGlobalTick; 
                }
                else if( ((dlCQIInfo->currentDLBLERForCW1_2 - 
                                dlCQIInfo->prevDlBlerCalculatedForCW1_2) > MIN_DL_BLER_DIFF_VALUE) && 
                        (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI 
                                              + APERIODIC_CQI_REQ_OFFSET)) && 
                        (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI
                                              + APERIODIC_CQI_REQ_OFFSET)))  
                {
                    /* SPR 13571 fix end */
                    /* CA_phase2_csi_code Start */
                    /* As part of CA FDD changes,15.5.9 b)of Arch doc, single entry
                     * of ulGrantAperiodicCqiQueue_gp(of PCell)if for CA UEs
                     * condition to trigger Aperiodic CSI occurs for single or both
                     * cells and update node variable aperiodicCQIToBeRequested */
                    *bitMaskForAperiodicReport_p |= servingCellBitMask_g[ueServCellIndex]; 
                    /* CA_phase2_csi_code End */
                    dlCQIInfo->aperiodicCqiRequestTTI = currentGlobalTick; 
                } 
            }   
            else if(PERIODIC_APERIODIC_MODE == dlCQIInfo->cqiTypeIndicator) 
            {
                if( ((dlCQIInfo->currentDLBLERForCW1_2 - TARGET_DL_BLER_VALUE) > 
                            DL_BLER_THRESHOLD) &&  
                        (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI
                                              + APERIODIC_CQI_REQ_OFFSET)) && 
                        (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI 
                                              + APERIODIC_CQI_REQ_OFFSET)) ) 
                {
                    /* As part of CA FDD changes,15.5.9 b)of Arch doc, single entry
                     * of ulGrantAperiodicCqiQueue_gp(of PCell)if for CA UEs
                     * condition to trigger Aperiodic CSI occurs for single or both
                     * cells and update node variable aperiodicCQIToBeRequested */

                    *bitMaskForAperiodicReport_p |= servingCellBitMask_g[ueServCellIndex]; 
                    dlCQIInfo->aperiodicCqiRequestTTI = currentGlobalTick; 
                }
                /* SPR 13571 fix start */
                else if( ((dlCQIInfo->widebandCQICodeWordTwo_2) &&
                            (dlCQIInfo->widebandCQICodeWordTwo_2 < (MAX_CQI_INDEX - 1))) && 
                        /* SPR 13571 fix end */
                        (dlCQIInfo->currentDLBLERForCW1_2 < MIN_DL_BLER_THRESHOLD) &&  
                        (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI 
                                              + (10 * ueULContext_p->pucchConfigInfo.cqiPmiRiInfo.widebandPeriodicity))) && 
                        (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI 
                                              + (10 * ueULContext_p->pucchConfigInfo.cqiPmiRiInfo.widebandPeriodicity))) ) 
                {	
                    /* As part of CA FDD changes,15.5.9 b)of Arch doc, single entry
                     * of ulGrantAperiodicCqiQueue_gp(of PCell)if for CA UEs
                     * condition to trigger Aperiodic CSI occurs for single or both
                     * cells and update node variable aperiodicCQIToBeRequested */
                    *bitMaskForAperiodicReport_p |= servingCellBitMask_g[ueServCellIndex]; 
                    dlCQIInfo->aperiodicCqiRequestTTI = currentGlobalTick; 
                }
            }

        }
        dlCQIInfo->prevDlBlerCalculatedForCW1_2 = dlCQIInfo->currentDLBLERForCW1_2;
    }
    else
    {
        /* Check for DL BLER counter for MAX DL BLER Samples */
        if(MAX_NUM_OF_DL_BLER_SAMPLES == 
                dlCQIInfo->dlBlerCalculatedCounterForCW1) 
        { 
            dlCQIInfo->currentDLBLERForCW1 = TARGET_DL_BLER_VALUE + 
                (dlCQIInfo->previousDLBLERForCW1/MAX_NUM_OF_DL_BLER_SAMPLES);
            /* Validate DL BLER value */
            VALIDATE_DL_BLER_VALUE(dlCQIInfo->currentDLBLERForCW1); 
            dlCQIInfo->dlBlerCalculatedCounterForCW1 = 0; 
            dlCQIInfo->previousDLBLERForCW1 = 0;
            dlCQIInfo->cqiCorrectionFactorForCW1 = 
                dlCQIInfo->cqiCorrectionFactorForCW1 + 
                dlBlerCorrectionFactor_g[dlCQIInfo->currentDLBLERForCW1];
            /* Validate CQI Correction Factor */
            VALIDATE_CQI_CORRECTION_FACTOR(dlCQIInfo->cqiCorrectionFactorForCW1); 

            /* Check for Aperiodic CQI mode */
            /* SPR 9427 changes start */\
            if(APERIODIC_CQI_REPORTING_MODE == dlCQIInfo->cqiTypeIndicator)  
                    /* SPR 9427 changes end */\
            {
                if( ((dlCQIInfo->currentDLBLERForCW1 - TARGET_DL_BLER_VALUE) > 
                                  DL_BLER_THRESHOLD) &&  
                               (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI
                                                      + APERIODIC_CQI_REQ_OFFSET)) && 
                                (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI 
                                                      + APERIODIC_CQI_REQ_OFFSET)) ) 
                {
                       /* CA_phase2_csi_code Start */
                       *bitMaskForAperiodicReport_p |= servingCellBitMask_g[ueServCellIndex]; 
                       /* CA_phase2_csi_code End */
                       dlCQIInfo->aperiodicCqiRequestTTI = currentGlobalTick; 
                }
                 /* SPR 13571 fix start */
                 else if( ((dlCQIInfo->currentDLBLERForCW1 - 
                                dlCQIInfo->prevDlBlerCalculatedForCW1) > MIN_DL_BLER_DIFF_VALUE) && 
                             (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI 
                                                    + APERIODIC_CQI_REQ_OFFSET)) && 
                              (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI
                                                      + APERIODIC_CQI_REQ_OFFSET)))  
                  {
                            /* SPR 13571 fix start */
                            /* CA_phase2_csi_code start */
                            *bitMaskForAperiodicReport_p |= servingCellBitMask_g[ueServCellIndex]; 

                            /* CA_phase2_csi_code End */
                            dlCQIInfo->aperiodicCqiRequestTTI = currentGlobalTick; 
                  }
              }
              else if(PERIODIC_APERIODIC_MODE == dlCQIInfo->cqiTypeIndicator) 
              {
                    if( ((dlCQIInfo->currentDLBLERForCW1 - TARGET_DL_BLER_VALUE) > 
                                DL_BLER_THRESHOLD) &&  
                            (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI
                                                  + APERIODIC_CQI_REQ_OFFSET)) && 
                            (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI 
                                                  + APERIODIC_CQI_REQ_OFFSET)) ) 
                    {
                        /* CA_phase2_csi_code start */
                        *bitMaskForAperiodicReport_p |= servingCellBitMask_g[ueServCellIndex]; 

                        /* CA_phase2_csi_code End */
                        dlCQIInfo->aperiodicCqiRequestTTI = currentGlobalTick; 
                    }
                    /* SPR 13571 fix start */
                    else if( ((dlCQIInfo->widebandCQICodeWordTwo) &&
                                (dlCQIInfo->widebandCQICodeWordTwo < (MAX_CQI_INDEX - 1))) && 
                            /* SPR 13571 fix end */
                            /* + coverity 97003 */
                            (dlCQIInfo->currentDLBLERForCW1 < MIN_DL_BLER_THRESHOLD) &&  
                            (currentGlobalTick > (dlCQIInfo->aperiodicCqiRequestTTI 
                                                  + (10 * (tickType_t)pucchConfigInfo.cqiPmiRiInfo.widebandPeriodicity))) && 
                            (currentGlobalTick > (ueULContext_p->aperiodicCqiRequestTTI 
                                                  + (10 * (tickType_t)pucchConfigInfo.cqiPmiRiInfo.widebandPeriodicity))) ) 
                        /* - coverity 97003 */
                    {
                        *bitMaskForAperiodicReport_p |= servingCellBitMask_g[ueServCellIndex]; 

                        dlCQIInfo->aperiodicCqiRequestTTI = currentGlobalTick; 
                    }
                }
            /* SPR 9427 changes end */
        }   /* SPR 13571 fix start */
        dlCQIInfo->prevDlBlerCalculatedForCW1 = dlCQIInfo->currentDLBLERForCW1;
        /* SPR 13571 fix end */
    }
    /* Update DL BLER for Code Word 1 */
    if((MAX_NUM_OF_DL_BLER_SAMPLES == dlCQIInfo->dlBlerCalculatedCounterForCW1) || 
            (MAX_NUM_OF_DL_BLER_SAMPLES == dlCQIInfo->dlBlerCalculatedCounterForCW1_2))
    {
        /* Update DL BLER for Code Word 1 */
        /* CA_phase2_csi_code Start */
        if(VICTIM_UE == ueULContext_p->userType) 
        {
            /*Complexity Start*/
            updateBlerCW1(internalCellIndex,ueULContext_p, dlCQIInfo->currentDLBLERForCW1_2
                    ,ueServCellIndex,ueIndex);
            /*Complexity End*/

        }
        else if((LTE_FALSE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum]) && (NON_VICTIM_UE == ueULContext_p->userType))
        {
            /*Complexity Start*/
            updateBlerCW1(internalCellIndex,ueULContext_p,
                    dlCQIInfo->currentDLBLERForCW1,
                    ueServCellIndex,
                    ueIndex);
            /*Complexity End*/

        }
        else if((LTE_TRUE == usableAbsPattern_g[internalCellIndex][usableAbsPatternIndex_g[internalCellIndex]][absSubFrameNum]) && (NON_VICTIM_UE == ueULContext_p->userType))
        {
            /*Complexity Start*/
            updateBlerCW1_2(internalCellIndex,ueULContext_p,
                    dlCQIInfo->currentDLBLERForCW1_2,
                    ueServCellIndex, ueIndex);
            /*Complexity End*/
        }

    }
    /* CA_phase2_csi_code Start */
    /* CA_phase2_csi_code End */
}
/* SPR 13571 fix end */

/* This Macro Check if the PUCCH CLPC BLER calculation is enabled or not           
If enabled then check if the cool off period is expired              
then check the BLER for PUCCH and trigger TPC for PUCCH if required 
*/                                                                     
/*SPR_4001_CHANGE_START*/       
/*****************************************************************************
 * Function Name  : updateAndCheckHarqBlerForPucch 
 * Inputs         : dlUeContext_p - Pointer to DLUEcontext,
 *                  oprTb - Provides ACK/NACK info,
 *                  currentTick - Global current tick
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates and checks the HARQ BLER information 
 *                  for PUCCH.
 *****************************************************************************/
STATIC  void updateAndCheckHarqBlerForPucch (
        DLUEContext  *dlUEContext_p, 
        UInt8        oprTb,
        /* SPR 15909 fix start */
        tickType_t       currentTick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex)               
{                                                                         
    SInt8 tpcValue = TPC_NO_CHANGE;                                      
    if(dlUEContext_p->tpcInfoForPucch.lastAppliedTick <= currentTick)   
    {                                                                  
        if (oprTb > 2) /*it means that operation > 2 */               
        {                                                            
            /* Operation is other than ACK/NACK */                  
            dlUEContext_p->blerInfoForPUCCH.blerCalculated         
                += PUCCH_BLER_UNSURE_STEP_SIZE(internalCellIndex); 
        }                                  
        else                              
        {                                
            /* Its an ACK or NACK, decrement the bler by PUCCH_BLER_STEP_SIZE */
            dlUEContext_p->blerInfoForPUCCH.blerCalculated -= PUCCH_BLER_SURE_STEP_SIZE(internalCellIndex);
        }                                                                        

        dlUEContext_p->blerInfoForPUCCH.blerCalculatedCounter++;                

        if (dlUEContext_p->blerInfoForPUCCH.blerCalculatedCounter                
                >= MAX_NUM_OF_PUCCH_BLER_SAMPLES)                                
        {                                                                      
            dlUEContext_p->blerInfoForPUCCH.pucchBler =                      
                (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->
                deltaBlerToTPCMapForPUCCH.pucchBLERTarget +       
                /* SPR 3530 START */ 
                (SInt8)(dlUEContext_p->blerInfoForPUCCH.blerCalculated/             
                        /* SPR 3530 END */ 
                        MAX_NUM_OF_PUCCH_BLER_SAMPLES);  
            if (dlUEContext_p->blerInfoForPUCCH.pucchBler < 1)               
            {                                                               
                dlUEContext_p->blerInfoForPUCCH.pucchBler = 0;             
            }                                                             
            else if (dlUEContext_p->blerInfoForPUCCH.pucchBler > 100)    
            {                                                           
                dlUEContext_p->blerInfoForPUCCH.pucchBler = 100;       
            }                                                         
            dlUEContext_p->blerInfoForPUCCH.blerCalculatedCounter = 0;
            dlUEContext_p->blerInfoForPUCCH.blerCalculated = 0;      
            /* SPR 5033 changes start */
            updatePucchBler(dlUEContext_p->ueIndex, 
                    dlUEContext_p->blerInfoForPUCCH.pucchBler,
                    internalCellIndex);
            /* SPR 5033 changes end */
        }
    }
    /* SPR 5033 changes start*/
    if ((cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->\
            powerControlEnableInfo.harqBlerClpcPucchEnable) &&
            (dlUEContext_p->tpcInfoForPucch.lastAppliedTick <= currentTick)&&
            (dlUEContext_p->blerInfoForPUCCH.lastBlerCoolOffCounter++ >= 
             (macReconfigSchedulerParamsDL_gp[internalCellIndex] + schParamsIndexDL_g[internalCellIndex])->\
             coolOffPeriodForBLERTriggeredPUCCHTPC))
        /* SPR 5033 changes end*/
    {                       
        /* +- SPR 17777*/
        tpcValue = checkTPCForPUCCHHarqBler(                               
                dlUEContext_p->blerInfoForPUCCH.pucchBler,
                 internalCellIndex);  
        /* +- SPR 17777*/
        if ((tpcValue != TPC_NO_CHANGE) &&                               
                (dlUEContext_p->tpcInfoForPucch.tpcTriggeredValue == TPC_NO_CHANGE))
        {                                                                   
            addEntryToTpcTriggerQueueForPucch(tpcValue, dlUEContext_p->ueIndex,
                    &(dlUEContext_p->tpcInfoForPucch),                      
                    currentTick,internalCellIndex);                                         
        }                                                                

        /* Reset the blerInfoForPUCCH.lastBlerCoolOffCounter to 0 */     
        dlUEContext_p->blerInfoForPUCCH.lastBlerCoolOffCounter = 0;     
    }
    /*SPR_4001_CHANGE_END*/
    LOG_MAC_MSG(HARQ_BLER_CALC_PUCCH_ID, LOGDEBUG, MAC_UL_PC,                   
            currentTick,                                                   
            dlUEContext_p->ueIndex,                                       
            dlUEContext_p->blerInfoForPUCCH.pucchBler,                   
            tpcValue,                                                   
            DEFAULT_INT_VALUE,                                         
            DEFAULT_INT_VALUE,                                        
            DEFAULT_FLOAT_VALUE,                                     
            DEFAULT_FLOAT_VALUE,                                    
            FUNCTION_NAME,                                         
            "CHECK_HARQ_BLER_FOR_PUCCH");                         
}                                                                           
/*SPR_4001_CHANGE_END*/
/* Cyclomatic Complexity changes - ends here */

/* SPR 11402 Fix  */
/*****************************************************************************
 * Function Name  :  updateKpiStatsForTotalSduAckRcvdDlperQci
 * Inputs         :  TBInfo,
 *                   TBFlag
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function updates KPI counters for the number of DL packets
 * 		     which have been transmitted and positively acknowledged.
 *****************************************************************************/

#ifdef KPI_STATS
/* SPR 16843 start */
STATIC  void updateKpiStatsForTotalSduAckRcvdDlperQci(DLHARQTBInfo *TBInfo,TbValue TBFlag,
                       InternalCellIndex internalCellIndex,UInt16 ueIndex)
/* SPR 16843 end */
{
    UInt8 lc = 0;
    UInt8 qci = 0; 
    UInt8 lcCount = 0;
    UInt8 sduCount = 0;                                 
    RLCDataBuffer* rlcDataBufferTB = PNULL;
    rlcDataBufferTB = &(TBInfo->transmissionAndRLCData_p->rlcDataBufferArr[0]);

/*spr 19296 fix start*/
    UInt32 activeIndexForMacStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForMacStats);
    LteMacKPIStats * pLteMacKPIStats = &(gMacStats.
      pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats[activeIndexForMacStats]);
/*spr 19296 fix end*/
    if(rlcDataBufferTB != NULL)
    {
        if(TBFlag == TB_ONE)
            lcCount = TBInfo->transmissionAndRLCData_p->lcCountTBOne; 
        else
            lcCount = TBInfo->transmissionAndRLCData_p->lcCountTBTwo; 

       for(lc = 0; lc < lcCount; lc++)
        {
            qci = rlcDataBufferTB[lc].qci;
            if (qci > 0 && qci <= MAX_QCI)      
            {
                /* SPR 16843 start */
                DLUEContext *dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
                if( PNULL != dlUEContext_p )
                {
                    if (RLC_UM_MODE == dlUEContext_p->logicalChannel[rlcDataBufferTB[lc].lcID].rlcMode)
                    {
                        if(rlcDataBufferTB[lc].sduCount > MAX_RLC_SDU_IN_PDU_FOR_KPI)
                            sduCount = MAX_RLC_SDU_IN_PDU_FOR_KPI;
                        else
                            sduCount = rlcDataBufferTB[lc].sduCount;

/*spr 19296 fix start*/
                        /*Update the KPI Counters for UU Loss Rate*/
                        pLteMacKPIStats->totalSduAckRcvdDlperQci[--qci] += sduCount;
/*spr 19296 fix end*/
                    }
                }
                /* SPR 16843 end */
            }
        }
    }
}
#endif
/*Com2plexity Start*/
/*****************************************************************************
 * Function Name  :  updateBlerCW1
 * Inputs         :  internalCellIndex - Cell-Index at MAC
 *                   ueULContext_p - The pointer to ULUEContext which contains
 *                                 the Uplink UE Context,
 *                   currentDLBLERForCW1- Current value for Downlink BLER
 *                   ueServCellIndex - serving cell index
 *                   ueIndex-The UE index of the UE
 * Outputs        :  None
 * Returns        :  None 
 * Description    :  Update BLER for CodeWord 1 
 *****************************************************************************/
void updateBlerCW1(InternalCellIndex internalCellIndex,
                   ULUEContext* ueULContext_p,
                   SInt8 currentDLBLERForCW1,
                   UInt8 ueServCellIndex,
                  UInt16 ueIndex)
{
    if(IS_PCELL_OF_UE(ueULContext_p , internalCellIndex))
    {
        updateDLBlerCW1(ueIndex, 
                currentDLBLERForCW1,internalCellIndex);

    }
    else
    {                        
        /* +- SPR 17777 */
        macUpdateSCellDLBlerCW1(ueIndex,currentDLBLERForCW1,
                ueServCellIndex);
        /* +- SPR 17777 */

    }
}				 

/*****************************************************************************
 * Function Name  :  updateBlerCW0
 * Inputs         :  internalCellIndex - Cell-Index at MAC
 *                   ueULContext_p - The pointer to ULUEContext which contains
 *                                 the Uplink UE Context,
 *                   currentDLBLERForCW0- Current value for Downlink BLER
 *                   ueServCellIndex - serving cell index
 *                   ueIndex-The UE index of the UE
 * Outputs        :  None
 * Returns        :  None
 * Description    :  Update BLER for CodeWord 0 
 *****************************************************************************/
void updateBlerCW0(InternalCellIndex internalCellIndex,
                   ULUEContext* ueULContext_p,SInt8 currentDLBLERForCW0,
                   UInt8 ueServCellIndex,UInt16 ueIndex)
{
    if(IS_PCELL_OF_UE(ueULContext_p , internalCellIndex))
    {
        updateDLBlerCW0(ueIndex, 
                currentDLBLERForCW0,internalCellIndex);

    }
    else
    {                        
        /* +- SPR 17777 */
        macUpdateSCellDLBlerCW0(ueIndex,currentDLBLERForCW0,
                ueServCellIndex);
        /* +- SPR 17777 */

    }
}	
/*****************************************************************************
 * Function Name  :  updateBlerCW1_2
 * Inputs         :  internalCellIndex - Cell-Index at MAC
 *                   ueULContext_p - The pointer to ULUEContext which contains
 *                                 the Uplink UE Context,
 *                   currentDLBLERForCW1_2 - Current value for Downlink BLER
 *                   ueServCellIndex - serving cell index
 *                   ueIndex-The UE index of the UE
 * Outputs        :  None 
 * Returns        :  None
 * Description    :  Update BLER for CodeWord 1_2 
 *****************************************************************************/
void updateBlerCW1_2(InternalCellIndex internalCellIndex,ULUEContext* ueULContext_p,SInt8 currentDLBLERForCW1_2,
                     UInt8 ueServCellIndex,
                     UInt16 ueIndex)
{
    if(IS_PCELL_OF_UE(ueULContext_p , internalCellIndex))
    {
        updateDLBlerCW1_2(ueIndex,
                currentDLBLERForCW1_2, internalCellIndex);
    }
    else 
    {
        macUpdateSCellDLBlerCW1_2(ueIndex,currentDLBLERForCW1_2,
                internalCellIndex,ueServCellIndex);		
    }
}

/* SPR 11402 Fix  */
/* SPR 11402 Fix  */
#ifdef TDD_CONFIG
/* + CA_HARQ_TDD_START */
/*****************************************************************************
 * Function Name  :  setHarqProcessInfoForPCell
 * Inputs         :  ueIndex - UeIndex of UE
 *                   currentSubframe - DL subframe of Harq
 *                   currentSFNIndex - ackNack Container Index
 *                   indexSfn - hpId Container Index
 *                   harqProcessId - Harq process Id
 *                   dlHarqRttExpiryTick - RRT expiry tick 
 *                   internalCellIndex - internal index of PCell
 *                   dlAckNackSf -  UpLink subFrame no. on which HARQ received.
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function update harqProcessId array and 
 *                   pdschTxMapContainer  of  PCell.
 *****************************************************************************/

STATIC void  setHarqProcessInfoForPCell(UInt16  ueIndex, UInt8 currentSubframe,
                               UInt32 currentSFNIndex, UInt8 indexSfn, 
                               UInt8 harqProcessId,
                               /* SPR 15909 fix start */
                               tickType_t dlHarqRttExpiryTick,
                               /* SPR 15909 fix end */
                               InternalCellIndex internalCellIndex,
                               UInt32  dlAckNackSf)
{

    DLUEContext*  ueContext_p           = PNULL;       
    DLUEContextInfo* dlUeContextInfo_p  = PNULL;
    TDDHarqTimerExpiryListNode *expiry_node_p  = PNULL;
    UInt32 harqRttTimerFactor = 0;
    DLRetxQueueNode *tempRetxNode_p = PNULL;
    UInt8   nackFlag[MAX_NUM_CELL] = {FALSE,};
    dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
    ueContext_p       = dlUeContextInfo_p->dlUEContext_p;
    UInt8 tempHarqId = INVALID_HARQ_ID;
    /* + Coverity 96923 */
	/* + SPS_TDD_Changes */
	UInt32 schUsed = 0;
	/* - SPS_TDD_Changes */
    /* - Coverity 96923 */
    harqRttTimerFactor =
        tddPdschTxMapOfDLHarq_p[internalCellIndex][currentSubframe].harqRttTimerFactor;

    ueContext_p->nackFlag[PCELL_SERV_CELL_IX] = FALSE;

    /* checking whether the harq process id is used */
    if ( ueContext_p->harqProcessId[currentSFNIndex][currentSubframe] == 
                                                        INVALID_HARQ_ID )
    {
        LOG_MAC_MSG(MAC_TDD_TIMER_START_HARQ_SET_INFO,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),\
                currentSFNIndex,\
                currentSubframe,\
                harqProcessId,\
                dlHarqRttExpiryTick,DEFAULT_INT_VALUE,\
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

        ueContext_p->harqProcessId[currentSFNIndex][currentSubframe] = 
                                                          harqProcessId;
    }
    else
    {
        LOG_MAC_MSG(MAC_TDD_TIMER_START_HARQ_NOT_FREED,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),\
            ueContext_p->harqProcessId[currentSFNIndex][currentSubframe],\
            ueContext_p->pdschTxMapContainer[indexSfn][dlAckNackSf],\
            currentSubframe,\
            currentSFNIndex,\
            DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"");

        expiry_node_p = ueContext_p->tddHarqTimerExpiryNode[indexSfn][dlAckNackSf];

        /* + Coverity 96923 */
        /* + SPS_TDD_Changes */
        if(expiry_node_p)
        {
            schUsed = ((TDDHarqTimerExpiryListNode*)expiry_node_p)->schUsed;
        }
        /* - SPS_TDD_Changes */
        /* - Coverity 96923 */

        ueContext_p->pdschTxMapContainer[indexSfn][dlAckNackSf] 
            &= (resetBitmask[dlAckNackSf]);

        tempHarqId =   ueContext_p->harqProcessId[currentSFNIndex][currentSubframe];
        if (!(UL_DL_CONFIG_5 == cellSpecificParams_g.\
              cellConfigAndInitParams_p[internalCellIndex]->cellParams_p->subFrameAssign))
        {    
            tddExpireNodeFromHarqTimerList( dlUeContextInfo_p,
                    currentSubframe,
                    ueIndex,
                    currentSFNIndex
                    ,0 /* freeHarqOnly = 0*/
                    /* + SPS_TDD_Changes */
                    ,schUsed
                    /* - SPS_TDD_Changes */
                    ,internalCellIndex
                    ,&nackFlag[PCELL_SERV_CELL_IX]
                    );
        }
        else
        {
            tddExpireNodeFromHarqTimerListForConfig5(dlUeContextInfo_p, 
                    currentSubframe,
                    ueIndex,
                    currentSFNIndex
                    ,0 /* freeHarqOnly = 0*/
                    ,internalCellIndex
                    );

        }   
            
        if (TRUE == nackFlag[PCELL_SERV_CELL_IX])
        {
            tempRetxNode_p = &ueContext_p->tempRetxNode; 
            tempRetxNode_p->ueIndex = ueIndex;
            tempRetxNode_p->harqProcessId = tempHarqId;
            tempRetxNode_p->schUsed = DRA_SCHEDULING;
            tempRetxNode_p->isNodeInvalid = FALSE;
            tempRetxNode_p->sCellCount = 0;
            ueContext_p->nackFlag[PCELL_SERV_CELL_IX] = TRUE;
        }   

        ueContext_p->harqProcessId[currentSFNIndex][currentSubframe] 
                                                       = INVALID_HARQ_ID;

        if ((expiry_node_p) &&
                (!ueContext_p->pdschTxMapContainer[indexSfn][dlAckNackSf]))
        {
            ueContext_p->tddHarqTimerExpiryNode[indexSfn][dlAckNackSf] = PNULL;
            /** CA-TDD Changes Start **/
            listDeleteNode( &tddDlHarqTimerEntityList_g[internalCellIndex][indexSfn][dlAckNackSf].\
			    dlExpiryList_p,&(expiry_node_p->nodeAnchor));
            /** CA-TDD Changes Start **/
            freeMemPool((void *)expiry_node_p);
        }
        LTE_MAC_UT_LOG(LOG_MAJOR,Log_DL_HARQ,"\n###C_SFN [%d] C_SF[%d] HarqPID [%d]\n",
                currentSFNIndex, currentSubframe, harqProcessId);
        ueContext_p->harqProcessId[currentSFNIndex][currentSubframe] = harqProcessId;
    }

    ueContext_p->dlHarqContext_p->harqProcess[harqProcessId].dlharqRttExpiryTick =
              dlHarqRttExpiryTick + harqRttTimerFactor;

    /* Set the bitmask for the current subframe number (ACK/NACK bundling)*/ 
    ueContext_p->pdschTxMapContainer[indexSfn][dlAckNackSf] |= (bitmask[currentSubframe]);

    LOG_MAC_MSG(MAC_TDD_DL_HARQ_TIMER_START_INFO_1,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),
            ueContext_p->dlHarqContext_p->harqProcess[harqProcessId].dlharqRttExpiryTick,\
            dlAckNackSf,\
            ueContext_p->pdschTxMapContainer[indexSfn][dlAckNackSf],\
            bitmask[currentSubframe],\
            DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

}

/*****************************************************************************
 * Function Name  :  setHarqProcessInfoForSCell
 * Inputs         :  ueIndex - UeIndex of UE
 *                   currentSubframe - DL subframe of Harq
 *                   currentSFNIndex - ackNack Container Index
 *                   indexSfn - hpId Container Index
 *                   harqProcessId -  Harq process Id
 *                   dlHarqRttExpiryTick - RRT expiry tick 
 *                   internalCellIndex - internal cell index of Scell
 *                   dlAckNackSf -  UpLink subFrame no. on which HARQ received.
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function update harqProcessSCellId array and 
 *                   pdschTxMapContainerForSCell  of SCell.
 *****************************************************************************/
STATIC void  setHarqProcessInfoForSCell(UInt16  ueIndex, UInt8 currentSubframe,
                               UInt32 currentSFNIndex, UInt8 indexSfn, 
                               UInt8 harqProcessId, /* SPR 15909 fix start */
                               tickType_t dlHarqRttExpiryTick,
                               /* SPR 15909 fix end */
                               InternalCellIndex internalCellIndex,
                               UInt32  dlAckNackSf)
{

    DLUESCellContext  *ueSCellContext_p           = PNULL;       
    DLUEContextInfo *dlUeContextInfo_p  = PNULL;
    DLUEContext  *ueContext_p           = PNULL;       
    TDDHarqTimerExpiryListNode *expiry_node_p  = PNULL;
    UInt32 harqRttTimerFactor = 0;
    DLRetxQueueNode *tempRetxNode_p = PNULL;
    UInt8 nackFlag = FALSE;
    UInt8 arrIndex = 0;
    InternalCellIndex   sCellIndex      = INVALID_SERV_CELL_INDEX;
    UInt8 tempHarqId = INVALID_HARQ_ID;
    /* + Coverity 96923 */
	/* + SPS_TDD_Changes */
	UInt32 schUsed = 0;
	/* - SPS_TDD_Changes */
    /* - Coverity 96923 */
    dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
    ueContext_p = dlUeContextInfo_p->dlUEContext_p;
    sCellIndex = getServeCellIndexFromInternalCellId(ueContext_p,internalCellIndex);
    ueSCellContext_p = dlUeContextInfo_p->dlUEContext_p->\
                              dlUeScellContext_p[sCellIndex];
    harqRttTimerFactor = 
        tddPdschTxMapOfDLHarq_p[internalCellIndex][currentSubframe].harqRttTimerFactor;

    ueContext_p->nackFlag[sCellIndex] = FALSE;

    /* checking whether the harq process id is used */
    if (ueSCellContext_p->harqProcessSCellId[currentSFNIndex][currentSubframe] 
                                                             == INVALID_HARQ_ID)
    {
        LOG_MAC_MSG(MAC_TDD_TIMER_START_HARQ_SET_INFO,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),\
                currentSFNIndex,\
                currentSubframe,\
                harqProcessId,\
                dlHarqRttExpiryTick,DEFAULT_INT_VALUE,\
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

        ueSCellContext_p->harqProcessSCellId[currentSFNIndex][currentSubframe] =
                                                                   harqProcessId;
    }
    else
    {
        LOG_MAC_MSG(MAC_TDD_TIMER_START_HARQ_NOT_FREED,LOGDEBUG,MAC_DL_HARQ,\
                getCurrentTick(),\
            ueSCellContext_p->harqProcessSCellId[currentSFNIndex][currentSubframe],\
            ueSCellContext_p->pdschTxMapContainerForSCell[indexSfn][dlAckNackSf],\
            currentSubframe,\
            currentSFNIndex,\
            DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,
            DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");

        expiry_node_p = ueContext_p->tddHarqTimerExpiryNode[indexSfn][dlAckNackSf];
        /* + Coverity 96923 */
        /* + SPS_TDD_Changes */
        if(expiry_node_p)
        {
            schUsed = ((TDDHarqTimerExpiryListNode*)expiry_node_p)->schUsed;
        }
        /* - SPS_TDD_Changes */
        /* - Coverity 96923 */

        ueSCellContext_p->pdschTxMapContainerForSCell[indexSfn][dlAckNackSf] 
            &= (resetBitmask[dlAckNackSf]);

        tempHarqId =   ueSCellContext_p->harqProcessSCellId[currentSFNIndex][currentSubframe];
        tddExpireNodeFromHarqTimerList( dlUeContextInfo_p,
                currentSubframe,
                ueIndex,
                currentSFNIndex
                ,0 /* freeHarqOnly = 0*/
                /* + SPS_TDD_Changes */
                ,schUsed
                /* - SPS_TDD_Changes */
                ,internalCellIndex
                ,&nackFlag
                );

        if (TRUE == nackFlag)
        {  
            if (ueContext_p->retxQueueNode_p[currentSubframe] == PNULL)
            {    
               tempRetxNode_p = &ueContext_p->tempRetxNode; 
            }   
            else
            {
               tempRetxNode_p = ueContext_p->retxQueueNode_p[currentSubframe]; 
            }
            tempRetxNode_p->ueIndex = ueIndex;
            tempRetxNode_p->isNodeInvalid = FALSE;
            tempRetxNode_p->sCellCount++;

            arrIndex = sCellIndex - 1;
            tempRetxNode_p->sCellRetxInfo[arrIndex].servCellIndex =
                                                        sCellIndex;
            tempRetxNode_p->sCellRetxInfo[arrIndex].harqProcessId =
                 tempHarqId;
            tempRetxNode_p->sCellRetxInfo[arrIndex].schUsed = DRA_SCHEDULING;

            ueContext_p->nackFlag[sCellIndex] = TRUE;
        }   

        ueSCellContext_p->harqProcessSCellId[currentSFNIndex][currentSubframe] 
            = INVALID_HARQ_ID;

        if ((expiry_node_p) &&
            (!ueSCellContext_p->pdschTxMapContainerForSCell[indexSfn][dlAckNackSf]))
        {
            ueContext_p->tddHarqTimerExpiryNode[indexSfn][dlAckNackSf] = PNULL;
            /** CA-TDD Changes Start **/
            listDeleteNode( &tddDlHarqTimerEntityList_g[ueContext_p->internalCellIndex][indexSfn][dlAckNackSf].\
			    dlExpiryList_p,&(expiry_node_p->nodeAnchor));
            /** CA-TDD Changes Start **/
            freeMemPool((void *)expiry_node_p);
        }
        LTE_MAC_UT_LOG(LOG_MAJOR,Log_DL_HARQ,"\n###C_SFN [%d] C_SF[%d] HarqPID [%d]\n",
                currentSFNIndex, currentSubframe, harqProcessId);

        ueSCellContext_p->harqProcessSCellId[currentSFNIndex][currentSubframe] =
                                                                   harqProcessId;
    }

    ueSCellContext_p->ScelldlHarqContext_p->harqProcess[harqProcessId].dlharqRttExpiryTick =
              dlHarqRttExpiryTick + harqRttTimerFactor;

    /* Set the bitmask for the current subframe number (ACK/NACK bundling)*/ 
    ueSCellContext_p->pdschTxMapContainerForSCell[indexSfn][dlAckNackSf] |=
                                                     (bitmask[currentSubframe]);

    LOG_MAC_MSG(MAC_TDD_DL_HARQ_TIMER_START_INFO_1,LOGDEBUG,MAC_DL_HARQ,\
            getCurrentTick(),
            ueSCellContext_p->ScelldlHarqContext_p->\
            harqProcess[harqProcessId].dlharqRttExpiryTick,\
            dlAckNackSf,\
            ueSCellContext_p->pdschTxMapContainerForSCell[indexSfn][dlAckNackSf],\
            bitmask[currentSubframe],\
            DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"");

}

/*****************************************************************************
 * Function Name  :  tddExpireHarqNodeOfPCell
 * Inputs         :  ueIndex - UeIndex of UE
 *                   containerIndex - Container index for tx map
 *                   dlAckNackSubFrame - DL Data Ack Nack Subframe Number 
 *                   dlDatTxSubframe - DL Data Tx Subframe Number
 *                   txSFNIndex - SFN  Container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs
 *                   internalCellIndex - Cell-Index at MAC
 *                   nackFlag_p - Flag to check retransmission on Pcell
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function expire harq node for PCell of UE.
 *****************************************************************************/
STATIC void tddExpireHarqNodeOfPCell(UInt16 ueIndex, UInt32 containerIndex,
                                     UInt8 dlAckNackSubFrame, UInt8 dlDatTxSubframe,
                                     UInt8 txSFNIndex, UInt8 freeHarqOnly,
                                     InternalCellIndex internalCellIndex,
                                     UInt8 *nackFlag_p)
{    
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUeContext_p        = PNULL;
    /* + Coverity 96923 */
	/* + SPS_TDD_Changes */
	UInt32 schUsed = 0;
	/* - SPS_TDD_Changes */
    /* - Coverity 96923 */

    dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
    dlUeContext_p = dlUeContextInfo_p->dlUEContext_p;

    if (( dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame])&
            (bitmask[dlDatTxSubframe]))
    {
        dlUeContext_p->pdschTxMapContainer[containerIndex][dlAckNackSubFrame] &= 
            (resetBitmask[dlDatTxSubframe]) ;

        if (dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] !=
                                                         INVALID_HARQ_ID)
        {
            /* + SPS_TDD_Changes */
            TDDHarqTimerExpiryListNode *expiry_node_p  = PNULL;
            expiry_node_p = dlUeContext_p->tddHarqTimerExpiryNode[containerIndex][dlAckNackSubFrame];
            /* + Coverity 96923 */
            if(expiry_node_p)
            {
                schUsed = ((TDDHarqTimerExpiryListNode*)expiry_node_p)->schUsed;
            }
            /* - Coverity 96923 */
            /* - SPS_TDD_Changes */

            tddExpireNodeFromHarqTimerList( dlUeContextInfo_p,
                    dlDatTxSubframe,
                    ueIndex,
                    txSFNIndex
                    ,freeHarqOnly
                    /* + SPS_TDD_Changes */
                    ,schUsed
                    /* - SPS_TDD_Changes */
                    ,internalCellIndex
                    ,nackFlag_p
                    );

            dlUeContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe] = 
                                                       INVALID_HARQ_ID ;
        }
        else
        {
            LOG_MAC_MSG(MAC_INVALID_HARQ,LOGERROR,MAC_DL_HARQ,\
                    getCurrentTick(),
                    dlDatTxSubframe,
                    INVALID_HARQ_ID,ueIndex,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
            ltePanic("[%s]:Invalid HARQ process ID for PCell \n",__func__);
        }
    }
}            

/*****************************************************************************
 * Function Name  :  tddExpireHarqNodeOfSCell
 * Inputs         :  ueIndex - UeIndex of UE
 *                   containerIndex - Container index for tx map
 *                   dlAckNackSubFrame - DL Data Ack Nack Subframe Number 
 *                   dlDatTxSubframe - DL Data Tx Subframe Number
 *                   txSFNIndex - SFN  Container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occur
 *                   internalCellIndex - Cell-Index at MAC
 *                   ueServCellIndex - Serving cell index
 *                   nackFlag_p - Flag to check retx
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function expire harq node for SCell of UE.
 *****************************************************************************/
STATIC void tddExpireHarqNodeOfSCell(UInt16 ueIndex, UInt32 containerIndex,
                                     UInt8 dlAckNackSubFrame,UInt8 dlDatTxSubframe,
                                     UInt8 txSFNIndex,UInt8 freeHarqOnly,
                                     /* +- SPR 17777 */
                                     InternalCellIndex ueServCellIndex,
                                      UInt8 *nackFlag_p)
{    
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUESCellContext *dlUeScellContext_p  = PNULL;

    dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
    dlUeScellContext_p = dlUeContextInfo_p->\
                          dlUEContext_p->dlUeScellContext_p[ueServCellIndex];

    if ((dlUeScellContext_p->pdschTxMapContainerForSCell[containerIndex][dlAckNackSubFrame])&
            (bitmask[dlDatTxSubframe]))
    {
        dlUeScellContext_p->pdschTxMapContainerForSCell[containerIndex][dlAckNackSubFrame] &= 
            (resetBitmask[dlDatTxSubframe]) ;

        if (dlUeScellContext_p->harqProcessSCellId[txSFNIndex][dlDatTxSubframe] !=
                   INVALID_HARQ_ID)
        {
            tddExpireNodeFromHarqTimerList( dlUeContextInfo_p,
                    dlDatTxSubframe,
                    ueIndex,
                    txSFNIndex
                    ,freeHarqOnly
                    /* + SPS_TDD_Changes */
                    ,DRA_SCHEDULING
                    /* - SPS_TDD_Changes */
                    /* + 13998 SPR Changes */
                    ,dlUeScellContext_p->internalCellIndex
                    /* - 13998 SPR Changes */
                    ,nackFlag_p
                    );

            dlUeScellContext_p->harqProcessSCellId[txSFNIndex][dlDatTxSubframe] =
                                                           INVALID_HARQ_ID ;
        }
        else
        {
            LOG_MAC_MSG(MAC_INVALID_HARQ,LOGERROR,MAC_DL_HARQ,\
                    getCurrentTick(),
                    dlDatTxSubframe,
                    INVALID_HARQ_ID,ueIndex,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_FLOAT_VALUE,\
                    DEFAULT_FLOAT_VALUE,FUNCTION_NAME,"");
            ltePanic("[%s]:Invalid HARQ process ID for SCell\n",__func__);
        }
    }
}            

/*****************************************************************************
 * Function Name  :  tddExpireHarqNode
 * Inputs         :  ueIndex - UeIndex of UE
 *                   containerIndex - Container index for tx map
 *                   dlAckNackSubFrame - DL Data Ack Nack Subframe Number 
 *                   dlDatTxSubframe - DL Data Tx Subframe Number
 *                   txSFNIndex - SFN  Container index
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs
 *                   internalCellIndex - Cell-Index at MAC
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function expire harq node for both PCell and 
 *                   SCell(s) of UE.
 *****************************************************************************/
STATIC void  tddExpireHarqNode(UInt16 ueIndex, UInt32 containerIndex,
                        UInt8 dlAckNackSubFrame,UInt8 dlDatTxSubframe,
                        UInt8 txSFNIndex,UInt8 freeHarqOnly,
                        InternalCellIndex internalCellIndex)

{    
    UInt8   nackFlag[MAX_NUM_CELL] = {FALSE,};
    UInt8 arrIndex = 0;
    UInt8 sCellCount = 0;
    InternalCellIndex ueServCellIndex = 0;
    DLUEContextInfo* dlUeContextInfo_p = PNULL;
    DLUEContext*  dlUEContext_p        = PNULL;
    /* +- SPR 18490 */

    /* To store re-tx info of pcell and scell */
    DLRetxQueueNode  retxNode;
    /* + Coverity_69536 */
    SET_HARQ_RETX_NODE(retxNode); 
    /* - Coverity_69536 */

    dlUeContextInfo_p = &dlUECtxInfoArr_g[ueIndex];
    dlUEContext_p = dlUeContextInfo_p->dlUEContext_p;

    retxNode.isNodeInvalid = TRUE; 

    /* Storing Pcell retx info */
    retxNode.ueIndex = ueIndex;
    retxNode.harqProcessId =
        dlUEContext_p->harqProcessId[txSFNIndex][dlDatTxSubframe];
    /* + SPS_TDD_CHANGES */
    TDDHarqTimerExpiryListNode *expiry_node_p  = PNULL;
    expiry_node_p = dlUEContext_p->tddHarqTimerExpiryNode[containerIndex][dlAckNackSubFrame];
    /* + Coverity 96923 */
    /* +- SPR 18490 */
    if(expiry_node_p)
    {
        retxNode.schUsed = ((TDDHarqTimerExpiryListNode*)expiry_node_p)->schUsed;
    }
    else
    {
        retxNode.schUsed = 0;
    }
    /* +- SPR 18490 */
    /* - Coverity 96923 */
    /* - SPS_TDD_CHANGES */

    /* Check for Pcell */ 
    tddExpireHarqNodeOfPCell(ueIndex,containerIndex,
            dlAckNackSubFrame, dlDatTxSubframe,
            txSFNIndex,freeHarqOnly,internalCellIndex,
            &nackFlag[PCELL_SERV_CELL_IX]);

    if (nackFlag[PCELL_SERV_CELL_IX])
    {
        retxNode.isNodeInvalid = FALSE;
    }    

    /* Check if CA is enable and Scell of UE context exists then 
     * update harq info for Scell.
     */

    sCellCount = dlUEContext_p->scellCount;

    if ((IS_CA_ENABLED()) && sCellCount)
    {
        for(ueServCellIndex = START_SCELL_INDEX; ueServCellIndex <= sCellCount ;
                ueServCellIndex++)
        {    
            /* + SPR 14839 changes */
            if(!(IS_SCELL_ACTIVE(ueIndex, ueServCellIndex)))
            /* - SPR 14839 changes */
            {
                continue;
            }

            arrIndex  = ueServCellIndex - 1;

            /* Storing Scell retx info */
            retxNode.sCellRetxInfo[arrIndex].harqProcessId =
              dlUEContext_p->dlUeScellContext_p[ueServCellIndex]->\
                harqProcessSCellId[txSFNIndex][dlDatTxSubframe];
            retxNode.sCellRetxInfo[arrIndex].servCellIndex =
                ueServCellIndex;
            /* + SPS_TDD_CHANGES */
            /* +- SPR 18490 */
            TDDHarqTimerExpiryListNode *expiry_node_p  = PNULL;
            expiry_node_p = dlUEContext_p->tddHarqTimerExpiryNode[txSFNIndex][dlAckNackSubFrame];
            /* + Coverity 96923 */
            /* +- SPR 18490 */
            if(expiry_node_p)
            {
                retxNode.sCellRetxInfo[arrIndex].schUsed =
                    ((TDDHarqTimerExpiryListNode*)expiry_node_p)->schUsed;
            }
            else
            {
                retxNode.sCellRetxInfo[arrIndex].schUsed = 0;
            }
            /* +- SPR 18490 */
            /* - Coverity 96923 */
            /* - SPS_TDD_CHANGES */

            tddExpireHarqNodeOfSCell(ueIndex,containerIndex, 
                    dlAckNackSubFrame, dlDatTxSubframe,
                    /* +- SPR 17777 */
                    txSFNIndex,freeHarqOnly,
                    ueServCellIndex,
                    &nackFlag[ueServCellIndex]);

            if(TRUE == nackFlag[ueServCellIndex])
            {
                retxNode.isNodeInvalid = FALSE;
            }    

        }
    }

    if((FALSE == retxNode.isNodeInvalid) && (MAC_FAILURE ==
                macPushHarqNackInfoToRetxQueue(dlUEContext_p ,retxNode ,
                    nackFlag,internalCellIndex, 1) ))
    {
        macHandleRetxQueueFailure(ueIndex,nackFlag,retxNode);
    }
}           


/*****************************************************************************
 * Function Name  :  checkTBsAndPutEntryInRetxQueue
 * Inputs         :  dlUeContext_p - DL context pointer
 *                   dlHarqProcess_p - Harq process pointer
 *                   internalCellIndex - Cell-Index at MAC
 *                   nackFlag_p - Nack flag to retx flag
 *                   freeHarqOnly - Free harq flag in case of overshoot occurs
 * Outputs        :  None
 * Returns        :  None
 * Description    :  This function check for TB valid and put entry in  Retx 
 *                   queue or send ARQ ind.
 *****************************************************************************/
STATIC void checkTBsAndPutEntryInRetxQueue(DLUEContext* dlUeContext_p,
                             DLHARQProcess *dlHarqProcess_p,
                             InternalCellIndex internalCellIndex,
                             UInt8 *nackFlag_p,
                             UInt8 freeHarqOnly,
                             /* + SPS_TDD_Changes */
                             UInt32 schUsed)
    /* - SPS_TDD_Changes */
{   


    UInt16 ueIndex = dlUeContext_p->ueIndex;
	/* + SPS_TDD_Changes */
	UInt8 maxRetxCount = 0;
	/* - SPS_TDD_Changes */

    if (dlHarqProcess_p->isTBOneValid) 
    {
        /*
           We first consider the case when both the TBs are VALID.
           */
        if (dlHarqProcess_p->isTBTwoValid)
        {
            /*
               We now check for the re-transmission counter of both
               the TBs.
               */
            LTE_MAC_UT_LOG(LOG_DETAIL, Log_DL_HARQ, "[%s]"
                    "The vlue of current_TX_NB_TB_ONE = %d "\
                    "The vlue of current_TX_NB_TB_TWO = %d \n",__func__,
                    dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB,
                    dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB);
            /* SPR 2249 Fixes Begins */
            if ((dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB == \
                        (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                         cellParams_p->maxHarqRetransmission - 1)
                        && dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB == \
                        (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                         cellParams_p->maxHarqRetransmission - 1))
                    || (freeHarqOnly == 1)
               )     
                /* SPR 2249 Fixes Ends */
            {
                /* _RLC_AM_MODE_ */
                if (!((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_MSG) ||
                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == DRX_MSG) ||
                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_DRX_MSG) || 
                            (dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                        (dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p))
                { 

                    /* SPR 11402 fix */
                    sendARQIndicationToRLC(
                            ueIndex, 
                            &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex);
                    /* SPR 11402 fix */
                }

                /* _RLC_AM_MODE_ */
                if (!((dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_MSG) ||
                            (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == DRX_MSG) ||
                            (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_DRX_MSG) || 
                            (dlHarqProcess_p->dlHARQTBTwoInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                        (dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p))
                {																		   
                    /* SPR 11402 fix */
                    sendARQIndicationToRLC(
                            ueIndex, 
                            &dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex);
                    /* SPR 11402 fix */
                }

                /*
                   We send the ARQ indication to RLC twice, one for each
                   TB and then we free the HARQ Process completely.
                   */
                freeDLHarqProcess(dlUeContext_p,
                        dlHarqProcess_p,
                        internalCellIndex);
            }
            else
            {
                /* RETX_CHG */
                /* SPS_CHG */
                *nackFlag_p = TRUE;
                /* SPS_CHG */
            }
        }
        /*
           We now consider the case when Only TB ONE is Valid and 
           TB_TWO is invalid (MIMO case).
           */
        else
        {
            /* + SPS_TDD_Changes */
            /*SPS_16019_16023_changes Start*/
            if(SPS_SCHEDULING == schUsed )  
            {
                maxRetxCount = dlUeContext_p->spsDlInfo.\
                               spsDlSetupInfo_p->maxSpsHarqRetx;  
            } 
            else
            {
                maxRetxCount = cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                               cellParams_p->maxHarqRetransmission - 1;
            }
            /* SPR 2249 Fixes Begins */
            if ((dlHarqProcess_p->dlHARQTBOneInfo.current_TX_NB < maxRetxCount)
                    && (freeHarqOnly == 0)
               )     
                /* SPR 2249 Fixes Ends */
            {
                /*
                   Here the TB one is yet to complete the max re-trans.
                   */
                /* RETX_CHG */
                /* SPS_CHG */
                *nackFlag_p = TRUE;
                /* RETX_CHG */

            }
            /* - SPS_TDD_Changes */
            /*
               We now consider the case When TB_One 
               has reached it's max re trans limit.
               */
            else
            {
                /* _RLC_AM_MODE_ */
                if (!((dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_MSG) ||
                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == DRX_MSG) ||
                            (dlHarqProcess_p->dlHARQTBOneInfo.taDrxFlag == TA_DRX_MSG) || 
                            (dlHarqProcess_p->dlHARQTBOneInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                        (dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p))
                { 

                    /* SPR 11402 fix */
                    sendARQIndicationToRLC(
                            ueIndex, 
                            &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,internalCellIndex);
                    /* SPR 11402 fix */
                }


                freeDLHarqProcessTBSpecific(
                        dlUeContext_p,
                        dlHarqProcess_p,
                        TB_ONE,
                        internalCellIndex);
            }
        }
    }
    /*
       We now consider the Case when ONLY  TB Two is Valid and
       TB One is invalid (MIMO case).
       */
    else
    {
        /* SPR 2249 Fixes Begins */
        if (dlHarqProcess_p->dlHARQTBTwoInfo.current_TX_NB < \
                (cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->\
                 cellParams_p->maxHarqRetransmission - 1)     
                && (freeHarqOnly == 0)
           )     
            /* SPR 2249 Fixes Ends */
        {
            /*
               Here the TB one is yet to complete the max re-trans.
               */
            *nackFlag_p = TRUE;
        }
        /*
           We now consider the case When TB_Two has reached it's
           max re trans limit.
           */
        else
        {
            /* _RLC_AM_MODE_ */
            if (!((dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_MSG) ||
                        (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == DRX_MSG) ||
                        (dlHarqProcess_p->dlHARQTBTwoInfo.taDrxFlag == TA_DRX_MSG) || 
                        (dlHarqProcess_p->dlHARQTBTwoInfo.macScellCEFlag == SCELL_ACT_DEACT_MSG))&& 
                    (dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p))
            {																		   
                /* SPR 11402 fix */
                sendARQIndicationToRLC(
                        ueIndex, 
                        &dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,internalCellIndex);
                /* SPR 11402 fix */
            }

            freeDLHarqProcessTBSpecific(
                    dlUeContext_p,
                    dlHarqProcess_p,
                    TB_TWO,
                    internalCellIndex);
        }
    }
}            
/* - CA_HARQ_TDD_START */
#endif
/*cyclomatic fix start*/
/*****************************************************************************
 * Function Name  :   freeDLHarqProcessForTBOne 
 * Inputs         :   dlHarqTbInfo_p - The pointer to DLHARQTBInfo which contains
 *                                  the harq ptr to TB1,
 *                    dlHarqProcess_p - The pointer to the DLHARQProcess corresponding
 *                                      to appropriate HARQ Process,
 * Outputs        :   None
 * Returns        :   None
 * Description    :   This function when called will remove the entire HARQ
 *                    process of the TB one of UE (whether in MIMO or Non-MIMO).
 *****************************************************************************/
/* SPR 16916 FIX START */
 void freeDLHarqProcessForTBOne(DLHARQProcess* dlHarqProcess_p,
        /* +- SPR 17777 */
                                      DLHARQTBInfo* dlHarqTbInfo_p
#ifdef KPI_STATS
                                      ,UInt16 ueIndex,
                                      InternalCellIndex internalCellIndex
#endif
                                      )
/* +- SPR 17777 */
/* SPR 16916 FIX END */
{
    dlHarqProcess_p->isTBOneValid = TB_INVALID;
    /*SPR 3778 Fix Start*/
    dlHarqProcess_p->dciFormat = MAX_NUM_DCI_FORMAT;
    /*SPR 3778 Fix End*/

    dlHarqTbInfo_p->taDrxFlag = INVALID_TA_DRX_MSG;
    dlHarqTbInfo_p->txIndicator = INVALID_TX;
    dlHarqTbInfo_p->current_TX_NB = 0;
    dlHarqTbInfo_p->irVersion = 0;
    dlHarqTbInfo_p->tbSize = 0;
    dlHarqTbInfo_p->retransmissionTBSize = 0;
    dlHarqTbInfo_p->paddingBytes = 0;
    dlHarqTbInfo_p->macScellCEFlag= INVALID_SCELL_ACT_DEACT_MSG;
    /* SPR 13084 Fix Start */
    dlHarqProcess_p->scellActBitmask = 0;
    dlHarqProcess_p->scellDeactBitmask = 0;
    /* SPR 13084 Fix End */

    /* _RLC_AM_MODE_ */
    if (dlHarqTbInfo_p->transmissionAndRLCData_p)
    {
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        updateKpiStatsForNumOfActiveUeDlPerQci(ueIndex,
                &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,dlHarqProcess_p->harqProcessId,internalCellIndex);
#endif
        /* SPR 16916 FIX END */

        /* SPR# 9438,9471 fix */ 
        if(dlHarqTbInfo_p->transmissionAndRLCData_p->numValidTBs)
        {
            dlHarqTbInfo_p->transmissionAndRLCData_p->numValidTBs--;
        }

        if (!dlHarqTbInfo_p->transmissionAndRLCData_p->numValidTBs)
        {
            /* CSR 51542 fix start */
            LTE_QUEUE *sem_p = PNULL;
            RLCDataBuffer *buff = PNULL;
            UInt8 i = 0;
            for (i = 0; i<dlHarqTbInfo_p->transmissionAndRLCData_p->lcCountTBOne; i++)
            {
                buff = &(dlHarqTbInfo_p->transmissionAndRLCData_p->rlcDataBufferArr[i]);
                sem_p =&(buff->reTxRlcPduQ); 
                semDestroy(&(sem_p->sem_lock));
            }
            /* CSR 51542 fix end */
            /******/
            freeMemPool((void*)dlHarqTbInfo_p->transmissionAndRLCData_p);
            if(dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p == 
                    dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p)
            {
                dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;
            }
            else
            {
                dlHarqTbInfo_p->transmissionAndRLCData_p = PNULL;
            }
        }
        else
        {
            dlHarqTbInfo_p->transmissionAndRLCData_p = PNULL;
        }
    }
    /*
       The following section of code will be implemented later.

       for (i=0; i<MAX_LOGICAL_CHANNEL; i++)
       {
       dlHarqTbInfo_p->arqInfo[i].lcID = 255;
       dlHarqTbInfo_p->arqInfo[i].seqNumber = 0;
       }
     */
}    
/*****************************************************************************
 * Function Name  :   freeDLHarqProcessForTBTwo 
 * Inputs         :   dlHarqTbInfo_p - The pointer to DLHARQTBInfo which contains
 *                                  the harq ptr to TB1,
 *                    dlHarqProcess_p - The pointer to the DLHARQProcess corresponding
 *                                      to appropriate HARQ Process,
 * Outputs        :   None
 * Returns        :   None
 * Description    :   This function when called will remove the entire HARQ
 *                    process of the TB Two of UE (whether in MIMO or Non-MIMO).
 *****************************************************************************/
/* SPR 16916 FIX START */
 void freeDLHarqProcessForTBTwo(DLHARQProcess* dlHarqProcess_p,
        /* +- SPR 17777 */
                                      DLHARQTBInfo* dlHarqTbInfo_p
#ifdef KPI_STATS
                                      ,UInt16 ueIndex,
                                      InternalCellIndex internalCellIndex
#endif
                                      )
/* +- SPR 17777 */
/* SPR 16916 FIX END */
{
    dlHarqTbInfo_p->taDrxFlag = INVALID_TA_DRX_MSG;
    dlHarqTbInfo_p->txIndicator = INVALID_TX;
    dlHarqTbInfo_p->current_TX_NB = 0;
    dlHarqTbInfo_p->irVersion = 0;
    dlHarqTbInfo_p->tbSize = 0;
    dlHarqTbInfo_p->retransmissionTBSize = 0;
    dlHarqTbInfo_p->paddingBytes = 0;
    dlHarqTbInfo_p->macScellCEFlag= INVALID_SCELL_ACT_DEACT_MSG;

    if (dlHarqTbInfo_p->transmissionAndRLCData_p)
    {
        /* SPR 16916 FIX START */
#ifdef KPI_STATS
        updateKpiStatsForNumOfActiveUeDlPerQci(ueIndex,
                &dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,dlHarqProcess_p->harqProcessId,internalCellIndex);
#endif
        /* SPR 16916 FIX END */

        /* SPR# 9438,9471 fix */ 
        if(dlHarqTbInfo_p->transmissionAndRLCData_p->numValidTBs)
        {
            dlHarqTbInfo_p->transmissionAndRLCData_p->numValidTBs--;
        }

        if (!dlHarqTbInfo_p->transmissionAndRLCData_p->numValidTBs)
        {
            /* CSR 51542 fix start */
            LTE_QUEUE *sem_p = PNULL;
            RLCDataBuffer *buff = PNULL;
            UInt8 i = 0;
            for (i = 0; i<dlHarqTbInfo_p->transmissionAndRLCData_p->lcCountTBTwo; i++)
            {
                buff = &(dlHarqTbInfo_p->transmissionAndRLCData_p->rlcDataBufferArr[i]);
                sem_p =&(buff->reTxRlcPduQ); 
                semDestroy(&(sem_p->sem_lock));
            }
            /* CSR 51542 fix end */
            /******/
            freeMemPool((void*)dlHarqTbInfo_p->transmissionAndRLCData_p);
            if(dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p == 
                    dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p)
            {
                dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;
            }
            else
            {
                dlHarqTbInfo_p->transmissionAndRLCData_p = PNULL;
            }
        }
        else
        {
            if(dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p == 
                    dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p)
            {
                dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;
            }
            else
            {
                dlHarqTbInfo_p->transmissionAndRLCData_p = PNULL;
            }
        }
    }
    /*
       for (i=0; i<MAX_LOGICAL_CHANNEL; i++)
       {
       dlHarqTbInfo_p->arqInfo[i].lcID = 255;
       dlHarqTbInfo_p->arqInfo[i].seqNumber = 0;
       }
     */
} 
/*cyclomatic fix end*/

/*CYCLOMATIC_CHANGES_6.1_START*/
/*****************************************************************************
 *  * Function Name  :  freeRLCDataBuffer
 *  * Inputs         :  dlHarqTbInfo_p - The pointer to dlHarqTbInfo_p
 *  *                               corresponding to TB1 or TB2,
 *  *                   dlHarqProcess_p - The pointer to the DLHARQProcess
 *  *                             corresponding to appropriate HARQ Process
 *  * Outputs        :  The rlcDataBuffer will be set free
 *  * Returns        :  None
 *  * Description    :  This function will check if the TB of this HARQ Process 
 *  *                   has already been freed and then will free the 
 *  *                   rlcDataBufferArr_p
 ******************************************************************************/
STATIC  void freeRLCDataBuffer(DLHARQTBInfo* dlHarqTbInfo_p,
                                     DLHARQProcess* dlHarqProcess_p)
{
    if (dlHarqTbInfo_p->transmissionAndRLCData_p)
    {
        /* SPR# 9438,9471 fix */
        if(dlHarqTbInfo_p->transmissionAndRLCData_p->numValidTBs)
        {
            dlHarqTbInfo_p->transmissionAndRLCData_p->numValidTBs--;
        }

        if (!dlHarqTbInfo_p->transmissionAndRLCData_p->numValidTBs)
        {
            /******/

            freeMemPool((void*)dlHarqTbInfo_p->transmissionAndRLCData_p);
            if(dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p ==
                    dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p)
            {
                dlHarqProcess_p->dlHARQTBOneInfo.transmissionAndRLCData_p = PNULL;
                dlHarqProcess_p->dlHARQTBTwoInfo.transmissionAndRLCData_p = PNULL;
            }
            else
            {
                dlHarqTbInfo_p->transmissionAndRLCData_p = PNULL;
            }
        }
        else
        {
            dlHarqTbInfo_p->transmissionAndRLCData_p = PNULL;
        }
    }
}
/*****************************************************************************
 *  * Function Name  :  enqueuePendingDLMemoryPoolFreeQ
 *  * Inputs         :  dlUeContext_p - The pointer to DLUEContext which contains
 *  *                                 the Downlink UE Context,
 *  *                   dlHarqProcess_p - The pointer to the DLHARQProcess
 *  *                             orresponding to appropriate HARQ Process,
 *  *                   tbIdentifier - The value (TB_ONE/TB_TWO)
 *  * Outputs        :  The transmission buffer will be enqueued and then set
 *  *                   free
 *  * Returns        :  None
 *  * Description    :  This function is used to enqueue the transmission buffer 
 *  *                   for TB1 and TB2
 ******************************************************************************/
/* SPR 16916 FIX START */
STATIC  void enqueuePendingDLMemoryPoolFreeQ(DLHARQProcess* dlHarqProcess_p,
                                                   DLUEContext* dlUeContext_p,
                                                   /* +- SPR 17777 */
                                                   UInt8 tbIdentifier
#ifdef KPI_STATS
                                                   ,InternalCellIndex internalCellIndex
#endif
                                                   )
/* +- SPR 17777 */
/* SPR 16916 FIX END */
{
    if (tbIdentifier == TB_ONE)
    {
        if ( dlHarqProcess_p->transmissionBuffer_p )
        {
            /* SPR 16916 FIX START */
#ifdef KPI_STATS
            updateKpiStatsForNumOfActiveUeDlPerQci(dlUeContext_p->ueIndex,
                    &dlHarqProcess_p->dlHARQTBOneInfo,TB_ONE,dlHarqProcess_p->harqProcessId,internalCellIndex);
#endif
            /* SPR 16916 FIX END */

            /* SPR 5531 changes start */
            if(!(ENQUEUE_PENDING_DL_MEMORY_POOL_FREE_Q(DLHARQTBInfo,
                            (void *)(dlHarqProcess_p->transmissionBuffer_p),
                            /*SPR 12777 Changes Start*/
                            dlUeContext_p->internalCellIndex)))
                            /** SPR 12777 Changes End **/
            {
                lteWarning("pushNode() failed for "
                        "pendingDLMemoryPoolFreeQ_g queue");
                msgFree(dlHarqProcess_p->transmissionBuffer_p);
            }
            /* SPR 5531 changes end */
            dlHarqProcess_p->transmissionBuffer_p = PNULL;
        }

    }
    else if (tbIdentifier == TB_TWO)
    {    
        if (dlHarqProcess_p->transmissionBufferTB2_p )
        {
            /* SPR 16916 FIX START */
#ifdef KPI_STATS
            updateKpiStatsForNumOfActiveUeDlPerQci(dlUeContext_p->ueIndex,
                    &dlHarqProcess_p->dlHARQTBTwoInfo,TB_TWO,dlHarqProcess_p->harqProcessId,internalCellIndex);
#endif
            /* SPR 16916 FIX END */

            /* SPR 5531 changes start */
            if(!(ENQUEUE_PENDING_DL_MEMORY_POOL_FREE_Q(DLHARQTBInfo,
                            (void *)(dlHarqProcess_p->transmissionBufferTB2_p),
                            /*SPR 12777 Changes Start*/
                            dlUeContext_p->internalCellIndex)))
                /** SPR 12777 Changes End **/
            {
                lteWarning("pushNode() failed for "
                        "pendingDLMemoryPoolFreeQ_g queue");
                msgFree(dlHarqProcess_p->transmissionBufferTB2_p);
            }
            /* SPR 5531 changes end */
            dlHarqProcess_p->transmissionBufferTB2_p = PNULL;
        }
    }
}
/*CYCLOMATIC_CHANGES_6.1_END*/
/****************************************************************************
 * Function Name  : initNbDLHarq 
 * Inputs         : Number of cells configured
 * Outputs        : Initialize the Queue's & List
 * Returns        : None 
 * Description    : This function is used to initialize the Queue's and List
 ****************************************************************************/
