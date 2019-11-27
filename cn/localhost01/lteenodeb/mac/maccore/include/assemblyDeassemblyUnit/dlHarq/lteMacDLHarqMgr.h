/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacDLHarqMgr.h,v 1.1.1.1.6.1.4.2.2.1 2010/09/21 15:45:47 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This function contains the Structures, enum and the  
 *                     function declaration.
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacDLHarqMgr.h,v $
 * Revision 1.1.1.1.6.1.4.2.2.1  2010/09/21 15:45:47  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.21  2009/12/18 10:47:57  gur11912
 * modified for delaySubFramesForExpiry
 *
 * Revision 1.20  2009/09/22 16:27:23  gur19413
 * changed data type UInt8 to UInt32
 *
 * Revision 1.19  2009/08/12 06:56:01  gur18550
 * Function updateNackQueueLoadForReTransComplete
 *
 * Revision 1.18  2009/07/31 10:11:09  gur18550
 * File Updated
 *
 * Revision 1.17  2009/07/31 07:40:58  gur18550
 * MIMO case HARQ Functionality Changed
 *
 * Revision 1.16  2009/07/30 11:10:14  gur18550
 * Functionality of TA DRX on completion of Re trans
 *
 * Revision 1.15  2009/07/24 11:30:40  gur18550
 * Log Updated
 *
 * Revision 1.14  2009/07/14 09:12:46  gur20052
 * Changed the size of count from UInt8 to UInt16
 *
 * Revision 1.13  2009/06/22 14:31:18  gur18550
 * initDLHarq
 *
 * Revision 1.12  2009/06/19 06:57:18  gur18550
 * Second Review Comments Inc
 *
 * Revision 1.11  2009/06/15 14:49:33  gur18569
 * set INVALID_UE_ID to MAX_UE_SUPPORTED
 *
 * Revision 1.10  2009/06/08 11:24:18  gur18550
 * Comments Inc
 *
 * Revision 1.9  2009/06/08 04:46:20  gur18550
 * Function Updated
 *
 * Revision 1.8  2009/06/05 15:35:28  gur18550
 * Comments Inc
 *
 * Revision 1.5  2009/06/02 10:16:33  gur18550
 * Function Updated
 *
 * Revision 1.4  2009/05/29 10:22:14  gur18569
 * removed compilation error
 *
 * Revision 1.3  2009/05/29 05:56:02  gur18569
 * removed compilation errors
 *
 * Revision 1.2  2009/05/28 12:00:56  gur18550
 * Function Updated
 *
 * Revision 1.1  2009/05/26 11:24:36  gur18550
 * *** empty log message ***
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_DL_HARQ_MGR_H
#define LTE_MAC_DL_HARQ_MGR_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacDLUEContext.h"
#include "lteMacReceiver.h"
#include "lteMacPHICH.h"
#include "lteMacTypes.h"
#include "lteMacDLSchedulerStrategy.h"
#include "lteMacDLScellStateManager.h"
#include "lteMacMultiplexer.h"
#include "lteMacPucchPowerControl.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MIMO_ACK_NACK_ON_TB_ONE_ONLY_MASK 0x01
#define MIMO_ACK_NACK_ON_TB_TWO_ONLY_MASK 0x02
#define MIMO_ACK_NACK_MASK 0x03
#define ACK_NACK_MASK 0x01
#define BOTH_TB_VALID_MASK 0x0c
#define ONLY_TB_TWO_VALID 0x08
#define ONLY_TB_ONE_VALID 0x04
#define INVALID_COUNT 255
#define MAX_DL_HARQ_RETRANSMISSION (MAX_HARQ_RETRANSMISSION_COUNT - 1)
#define IS_CONTENTION_ACK 16
#define Log_DL_HARQ "DL_HARQ"
/*CLPC CHANGES */
#define PUCCH_POWER_CORRECTION_STEP_SIZE    10

/* + CQI_4.1 */
#define APERIODIC_CQI_REQ_OFFSET    (2 * MAX_NUM_OF_DL_BLER_SAMPLES)
#define ACK_STEP_SIZE               10 
#define NACK_STEP_SIZE              (100 - ACK_STEP_SIZE) 
#define MIN_DL_BLER_VALUE           0
#define MAX_DL_BLER_VALUE           100
#define MAX_NUM_OF_DL_BLER_SAMPLES  100
/* - CQI_4.1 */
/* + CQI_5.0 */
/* SPR 9427 changes start */\
#define DL_BLER_THRESHOLD           20
#define MIN_DL_BLER_THRESHOLD       5
/* SPR 9427 changes end */\
/* SPR 13571 fix start */
#define MIN_DL_BLER_DIFF_VALUE      10
#define SIXTY_FOUR_QAM_CQI          10
/* SPR 13571 fix end */
#define PERIODIC_CQI_DROP_OFFSET    3
/* SPR 7964/7981 changes Start */
/*SPR 20424 fix start*/
#define MIN_CQI_CORRECTION_VALUE                (-3 * DL_RESOLUTION_FACTOR)
/*SPR 20424 fix end*/

#define MAX_CQI_CORRECTION_VALUE                (3 * DL_RESOLUTION_FACTOR)
/* SPR 7964/7981 changes End */
/* - CQI_5.0 */

/****************************************************************************
 * Exported Types
 ****************************************************************************/

#ifdef FDD_CONFIG 
typedef struct DLHarqTimerNodeT
{
    UInt8   isValid;
    UInt8   harqProcessId; 
    UInt16 ueIndex;
    UInt8  schUsed; 
}DLHarqTimerNode;

typedef struct DLHarqTimerEntityListT
{
    /* The count will contain the number of valid field in the array*/
    UInt16 count;
    /* This array will store the harqTimerNode */  
    /*CSR 40799 chg start*/     
    /* SPR 9381 changes start */
    DLHarqTimerNode dlHarqTimerNodeArr[MAX_DL_UE_SCHEDULED];
    /* SPR 9381 changes end */
    /*CSR 40799 chg end*/
}DLHarqTimerEntityList;

/* + SPR 17733 */
typedef struct DLHarqTimerForMsg4NodeT
{
    UInt8   isValid;
    UInt8   harqProcessId; 
    UInt16 ueIndex;
}DLHarqTimerForMsg4Node;

typedef struct DLHarqTimerEntityforMsg4ListT
{
    /* The count will contain the number of valid field in the array*/
    UInt16 count;
    /* This array will store the harqTimerNode */       
    DLHarqTimerForMsg4Node dlHarqTimerNodeForMsg4Arr[MAX_UE_SUPPORTED];
}DLHarqTimerEntityForMsg4List;
/* - SPR 17733 */

typedef struct DLHarqGlobalsT
{
        /* this array will store the harqs of UE on per subframe basis.*/
        DLHarqTimerEntityList dlHarqTimerEntityList[MAX_SUB_FRAME];
        /* + SPR 17733 */
        DLHarqTimerEntityForMsg4List dlHarqTimerEntityForMsg4List[MAX_SUB_FRAME];
        /* - SPR 17733 */
}DLHarqGlobals;
extern DLHarqGlobals dlHarqGlobals_g[MAX_NUM_CELL];
#elif TDD_CONFIG
typedef struct DLHarqTimerEntityListT
{
    LTE_LIST   dlExpiryList_p;
}DLHarqTimerEntityList;

typedef struct DLAckNackTimerExpiryT
{
  UInt8 count;
  UInt8 dlUEScheduleSubFrameNum[MAX_SUB_FRAME];
}DLAckNackTimerExpiry;

typedef struct  TddPdschTxMapOfDLHarqT
{
    /* The field dlAckNackSf contains the UpLink subFrame no on which
     * DL ACK/NACK/DTX should be received
     * The field SFNFactor contains the number of system Frames
     * from the present system frame when the ACK NACK are
     * to be received*/
    UInt32 dlAckNackSf;
    UInt32 sfnFactor;
    UInt32 harqRttTimerFactor;
}TddPdschTxMapOfDLHarq;


#endif


typedef enum MIMIOAckNackT
{
     TB_TWO_INVALID_TB_ONE_VALID_NACK_ON_TB_ONE = 0x04,
     TB_TWO_INVALID_TB_ONE_VALID_ACK_ON_TB_ONE, 
     TB_TWO_VALID_TB_ONE_INVALID_NACK_ON_TB_TWO = 0x08,
     TB_TWO_VALID_TB_ONE_INVALID_ACK_ON_TB_TWO = 0x0a,
     BOTH_TB_VALID_NACK_ON_BOTH_TB = 0x0c,
     BOTH_TB_VALID_NACK_ON_TB_TWO_AND_ACK_ON_TB_ONE,
     BOTH_TB_VALID_ACK_ON_TB_TWO_AND_NACK_ON_TB_ONE,
     BOTH_TB_VALID_ACK_ON_BOTH_TB,
}MIMIOAckNack;

typedef enum ValidityT
{
    INVALID = 0,
    VALID          
}Validity;


#ifdef TDD_CONFIG
typedef enum UlSubFrameNoT
{
    UL_IN_SUB_FRAME_NO_0 = 0,
    UL_IN_SUB_FRAME_NO_1,
    UL_IN_SUB_FRAME_NO_2,
    UL_IN_SUB_FRAME_NO_3,
    UL_IN_SUB_FRAME_NO_4,    
    UL_IN_SUB_FRAME_NO_5,
    UL_IN_SUB_FRAME_NO_6,
    UL_IN_SUB_FRAME_NO_7,
    UL_IN_SUB_FRAME_NO_8,
    UL_IN_SUB_FRAME_NO_9    
}UlSubFrameNo;

typedef struct PendingMemCleanupT{
    void * data_p;
    UInt8 bufferType;
}PendingMemCleanup;
#endif

/* SPR 5531 changes start */
#ifdef FDD_CONFIG
extern LTE_CIRCQUEUE pendingDLMemoryPoolFree_g;
#endif
/* SPR 5531 changes end */

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/* + SPR 17733 */
#ifdef FDD_CONFIG
extern LTE_SQUEUE Msg4RetransmissionQueue_gp[MAX_NUM_CELL];
extern LTE_SQUEUE Msg4RetransmissionFailureQueue_gp[MAX_NUM_CELL];
#endif
/* - SPR 17733 */
#define CODE_WORD_0 0
#define CODE_WORD_1 1

#ifdef TDD_CONFIG
#define MAX_TDD_HARQ_CONT 2
#define DL_HARQ_EXPIRY_TIME 3
/** CA-TDD Changes Start **/
extern DLHarqTimerEntityList tddDlHarqTimerEntityList_g[MAX_NUM_CELL][2][MAX_SUB_FRAME];
/** CA-TDD Changes End **/

MacRetType unexpectedEvent(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig0Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig0Subframe4(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig0Subframe7(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig0Subframe9(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig1Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig1Subframe3(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig1Subframe7(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig1Subframe8(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig2Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig2Subframe7(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig3Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig3Subframe3(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig3Subframe4(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig4Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig4Subframe3(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig5Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig6Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig6Subframe3(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig6Subframe4(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig6Subframe7(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeForConfig6Subframe8(UInt32 sfn
        ,UInt8 freeHarqOnly
        ,InternalCellIndex internalCellIndex
        );
/* SPR 11110 fix start */
MacRetType tddExpireAllNodeForConfig0Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig0Subframe4(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig0Subframe7(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig0Subframe9(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig1Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig1Subframe3(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig1Subframe7(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig1Subframe8(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig2Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig2Subframe7(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig3Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig3Subframe3(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig3Subframe4(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig4Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig4Subframe3(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig5Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig6Subframe2(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig6Subframe3(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig6Subframe4(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig6Subframe7(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
MacRetType tddExpireAllNodeForConfig6Subframe8(UInt32 sfn
        ,UInt8 freeHarqOnly, InternalCellIndex internalCellIndex );
/* SPR 11110 fix end  */
#endif

 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* SPR 5599 changes start (ZIP ID 129059) */
void freeDLHarqProcessTBSpecific(
/* SPR 5599 changes end (ZIP ID 129059) */
                                 DLUEContext*  ueContext_p,
                                 DLHARQProcess* dlHarqProcess_p,
                                 UInt8 tbIdentifier,
                                 InternalCellIndex internalCellIndex);

/* SPR 5599 changes start (ZIP ID 129059) */
void freeDLHarqProcess(
/* SPR 5599 changes end (ZIP ID 129059) */
                         DLUEContext*  ueContext_p,
                         DLHARQProcess* dlHarqProcess_p,
                         InternalCellIndex internalCellIndex);
/*cyclomatic fix start*/
/* SPR 16916 FIX START */
void freeDLHarqProcessForTBOne(
                         DLHARQProcess* dlHarqProcess_p,
                         /* +- SPR 17777 */
                         DLHARQTBInfo* dlHarqTbInfo_p
#ifdef KPI_STATS
                         ,UInt16 ueIndex,
                         InternalCellIndex internalCellIndex
#endif
                         );
                         /* +- SPR 17777 */
void freeDLHarqProcessForTBTwo(
                         DLHARQProcess* dlHarqProcess_p,
                         DLHARQTBInfo* dlHarqTbInfo_p
#ifdef KPI_STATS
                         ,UInt16 ueIndex,
                         InternalCellIndex internalCellIndex
#endif
                         );
                         /* +- SPR 17777 */
/* SPR 16916 FIX END */
/*cyclomatic fix end*/


/* SPR 5599 changes start (ZIP ID 129059) */
void freeDLHarqTB(
/* SPR 5599 changes end (ZIP ID 129059) */
                     DLUEContext*  dlUeContext_p,
                     DLHARQProcess* dlHarqProcess_p,
                     UInt8 tbIdentifier,
                     InternalCellIndex internalCellIndex);

#ifdef FDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType dlHarqTimerStart( 
/* SPR 5599 changes end (ZIP ID 129059) */
                             UInt16  ueIndex,
                             UInt8   harqProcessId,
                             UInt32  frameNumber,
                             /* SPR 15909 fix start */
                             tickType_t  dlHarqRttExpiryTick,
                             /* SPR 15909 fix end */
                             SchType   schUsed,
                             InternalCellIndex internalCellIndex );
#elif TDD_CONFIG
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType tddDlHarqTimerStart( 
/* SPR 5599 changes end (ZIP ID 129059) */
                             UInt16  ueIndex,
                             UInt8   harqProcessId,
                             UInt32  subframeNumWithDelay,
                             /* SPR 15909 fix start */
                             tickType_t  dlHarqRttExpiryTick,
                             /* SPR 15909 fix end */
                             UInt16 sysFrameNumber,
                             /* + SPS_TDD_Changes */
                             UInt8   schUsed,
                             /* - SPS_TDD_Changes */
                             InternalCellIndex internalCellIndex
                             );
#endif


/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType dlHarqTimerStop(
/* SPR 5599 changes end (ZIP ID 129059) */
                           DLUEContext* ueContext_p,
                           UInt32     frameNumber,
                           InternalCellIndex internalCellIndex); 

MacRetType expireAllNodeFromHarqTimerList (
                                           UInt8 loopOfNumOfSubFrames,
                                           UInt32 currentSubFrame, 
                                           UInt8  delaySubFramesForExpiry,
                                           InternalCellIndex internalCellIndex);

/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType putEntryInUERetransmissionQueue(
/* SPR 5599 changes end (ZIP ID 129059) */
                                           DLUEContext*  ueContext_p,
                                           UInt8 harqProcessId);


DLHARQProcess *checkForFreeHarqProcess(DLUEContext*  dlUeCtx_p, 
        DLHARQEntity* dlHarqEntity_p,  SchType schType,
	InternalCellIndex internalCellIndex);

#ifdef FDD_CONFIG
/* SPR 15909 fix start */
/* +- SPR 17777 */
MacRetType processDLAckNackQueue(tickType_t globalTick, InternalCellIndex internalCellIndex);
/* +- SPR 17777 */
#elif TDD_CONFIG
MacRetType processDLAckNackQueue(tickType_t globalTick, InternalCellIndex internalCellIndex);
/* SPR 15909 fix end */
/* +- SPR 17777 */
#endif

/*Complexity Start*/
#ifdef TDD_Config
void eicicUpdateHarqSystemframeNumber(
	UInt32 subFrameNumber,
	tddAckNackQueueNode *ackNackNode_p);
#endif
/*Complexity End*/

/* + KPI_CA */
/* SPR 12783 Changes Start */
/* SPR 15909 fix start */
void macUpdateKpiCntrsForPktDelayPerQci(DLHARQTBInfo *TBInfo, UInt16 ueIndex, 
				UInt8 lcCount,InternalCellIndex internalCellIndex, tickType_t recvTTI);
/* SPR 15909 fix end */
/* SPR 12783 Changes End */
/* - KPI_CA */

/* SPR 5599 changes start (ZIP ID 129059) */
/* +- SPR 17777 */
#ifdef FDD_CONFIG
void initDLHarqTimerArray(UInt8 numCells);
#endif
/* +- SPR 17777 */
#ifdef TDD_CONFIG
void setDLHarqHarqTimerList(InternalCellIndex internalCellIndex);
#endif
void initDLHarq(UInt8 numCells);
/*SPR 2446 Fix Begins*/
/* Review comment fix start MAC_RESET_4 */
void resetMACDLHarqEntity ( DLHARQEntity **dlHarqContext_pp);
/* Review comment fix end MAC_RESET_4 */
/* SPR 5599 changes end (ZIP ID 129059) */
void resetMACDLScellHarqEntity ( DLHARQEntity **dlHarqContext_pp,
        DLUEContext* dlUEContext_p, InternalCellIndex internalCellIndex);
/*SPR 2446 Fix Ends*/
/* SPR# 3860 Changes Starts */
void expireDLHarqTimerNodeForTickMiss(UInt8 loopOfNumOfSubFrames,
                                      InternalCellIndex internalCellIndex);
/* SPR# 3860 Changes Ends */
#ifdef TDD_CONFIG												  

extern void assignTddPdschTxMapOfDLHarqPointer(UInt8 ulDlConfiguration, 
            InternalCellIndex internalCellIndex);


extern MacRetType tddExpireAllNodeFromHarqTimerList(
                                           UInt32 loopOfNumOfSubFrames,
                                           UInt32 currentSubFrame,
                                           UInt8 delaySubFrame,
                                           UInt32 sfn
        ,UInt8 freeHarqOnly
                                           ,InternalCellIndex internalCellIndex
        );
MacRetType tddExpireNodeFromHarqTimerList(DLUEContextInfo* dlUeContextInfo_p,
                                          UInt8 frameNumber,
                                          UInt16 ueIndex,
                                          UInt8 indexSfn
                                          ,UInt8 freeHarqOnly, 
                                          /* + SPS_TDD_Changes */
                                          UInt32 schUsed,
                                          /* - SPS_TDD_Changes */
                                          InternalCellIndex internalCellIndex,
                                          UInt8 *nackFlag_p
                                          );

/* Following function will return the RTT timer value for different
 * TDD Configs and different Sub-frames. */
/* SPR 23393 Changes Start */
 UInt8 tddHarqRttTimerFactor( UInt8 subFrame, InternalCellIndex cellId );
/* SPR 23393 Changes End */

/* SPR 11230 fix start */
/* This function will reset the harqSentMap_g for DL HARQ */
/* CA TDD Changes Start */
/* SPR 23393 Changes Start */
 void resetEntryInHarqSentMap( UInt8 mapIdx, InternalCellIndex internalCellIndex );
/* SPR 23393 Changes End */
/* Following function will update the harqSentMap_g for DL HARQ */
/* SPR 15909 fix start */
void putEntryInHarqSentMap( tickType_t currentTick, UInt8 mapIdx, 
        UInt16 ueIndex, UInt8 indexSfn, UInt8 dlAckNackSf,
		InternalCellIndex internalCellIndex);
/* SPR 15909 fix end */
/* CA TDD Changes End */
/* SPR 11230 fix end */
 
#endif
/* + SPR 17733 */
#ifdef FDD_CONFIG
MacRetType processDLAckNackForMsg4( TempCRNTICtx * tempUEContext_p, 
        UInt32 subFrameNumber, UInt8 operation, InternalCellIndex internalCellIndex);
void putEntryInMsg4ReTxQueue(
        TempCRNTICtx *  ueContext_p, UInt8 harqProcessId,
        /* SPR 15909 fix start*/
        InternalCellIndex internalCellIndex);
        /* SPR 15909 fix end */
void expireAllNodeFromHarqTimerListForMsg4 (
        UInt32 loopOfNumOfSubFrames,
        /* SPR 15909 fix start */
        tickType_t currentGlobalTick,
        /* SPR 15909 fix end */
        UInt8  delaySubFramesForExpiry,
        InternalCellIndex internalCellIndex);
 MacRetType dlHarqTimerStartForMsg4( UInt16 ueIndex, UInt8 harqProcessId,
        UInt32  frameNumber, 
        /* SPR 15909 fix start */
        tickType_t dlHarqRttExpiryTick,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex);
void freeDLHarqProcessForMsg4( DLHARQProcess *dlHarqProcess_p);
DLHARQProcess *checkForFreeHarqProcessForTcrnti(DLHARQEntity* dlHarqEntity_p);
#endif
/* - SPR 17733 */

/* + CQI_4.1 */
#define VALIDATE_DL_BLER_VALUE(calculatedBlerValue) \
{\
    if(calculatedBlerValue < MIN_DL_BLER_VALUE)  \
    {\
       calculatedBlerValue = MIN_DL_BLER_VALUE; \
    }\
    else if(calculatedBlerValue > MAX_DL_BLER_VALUE)  \
    {\
       calculatedBlerValue =  MAX_DL_BLER_VALUE;\
    }\
}
/* - CQI_4.1 */

/* + CQI_5.0 */
#define VALIDATE_CQI_CORRECTION_FACTOR(cqiCorrectionFactor) \
{\
    if(cqiCorrectionFactor < MIN_CQI_CORRECTION_VALUE)  \
    {\
       cqiCorrectionFactor = MIN_CQI_CORRECTION_VALUE; \
    }\
    else if(cqiCorrectionFactor > MAX_CQI_CORRECTION_VALUE)  \
    {\
       cqiCorrectionFactor =  MAX_CQI_CORRECTION_VALUE;\
    }\
}


/* CA_phase2_harq_code Start */

#define HARQ_PRIMARY_CELL  0
#define HARQ_SECONDARY_CELL 1

#define RCVD_ACK_NACK_4_ON_PUCCH 14
#define RCVD_ACK_NACK_4_ON_PUSCH 4
#define RCVD_ACK_NACK_3_ON_PUCCH 13
#define RCVD_ACK_NACK_3_ON_PUSCH 3
#define RCVD_ACK_NACK_2_ON_PUCCH 12
#define RCVD_ACK_NACK_2_ON_PUSCH 2

#define ONE_TB_SUPPORTED NUM_OF_TX_ANTENNAS_1
#define TWO_TB_SUPPORTED NUM_OF_TX_ANTENNAS_2

#define DECODE_ACK_NACK_TYPE(decodeAckNackFlag,ackNackType,isHarqOnPucch) \
{ \
    while(decodeAckNackFlag) \
    { \
        ackNackType++; \
        decodeAckNackFlag = decodeAckNackFlag >> 4; \
    } \
    ackNackType = (10 * isHarqOnPucch) + ackNackType; \
}

#define IS_MIMO_CONFIGURED(transMode ,numOfTxAnteenas) \
  if( transMode == TX_MODE_3 || transMode == TX_MODE_4 || \
   ((transMode == TX_MODE_8) && (1 < numOfTxAnteenas)))

#define TBS_SUPPORTED_IN_CELL(tranmissionMode , numOfTxAnteenas , servCellIndex) \
{ \
    IS_MIMO_CONFIGURED(tranmissionMode , numOfTxAnteenas ) \
    { \
        tbsSupportedCell[servCellIndex] = TWO_TB_SUPPORTED;\
    } \
    else \
    { \
        tbsSupportedCell[servCellIndex] = ONE_TB_SUPPORTED;\
    }\
}

#define SET_HARQ_RETX_NODE(retxNode) \
retxNode.ueIndex = INVALID_UE_ID; \
retxNode.harqProcessId = INVALID_HARQ_ID; \
retxNode.isNodeInvalid = TRUE; \
retxNode.schUsed = DRA_SCHEDULING; \
retxNode.sCellCount = 0; \
retxNode.sCellRetxInfo[0].servCellIndex = INVALID_CELL_INDEX; \
retxNode.sCellRetxInfo[0].harqProcessId = INVALID_HARQ_ID; \
retxNode.sCellRetxInfo[0].schUsed =  DRA_SCHEDULING;

#define CHECK_TADRXFLAG_RLC_TX_BUFF(taDrxFlag ,rlcData_p, macScellCEFlag) \
 if (!((taDrxFlag == TA_MSG) || (taDrxFlag == DRX_MSG ) \
     || (taDrxFlag == TA_DRX_MSG )  || (macScellCEFlag == SCELL_ACT_DEACT_MSG)) && (rlcData_p))

/* CA_phase2_harq_code End */

/* Cyclomatic Complexity changes - ends here */
/* SPR 15909 fix start */
 void calculateDLBlerForAckTB1 (tickType_t currentGlobalTick,
/* SPR 15909 fix end */
        ULUEContext     *ueULContext_p,
  /* CA_phase2_harq_code Start */  
        DLCQIInfo        *dlCQIInfo_p, 
        UInt16            ueIndex,
  /* CA_phase2_harq_code End */  
        InternalCellIndex internalCellIndex,
        /* CA_phase2_csi_code Start */
        UInt8 ueServCellIndex,
        UInt8 *bitMaskForAperiodicReport_p,
        DLMIMOInfo       *dlMIMOInfo
        /* CA_phase2_csi_code End */
        );

/* SPR 15909 fix start */
 void calculateDLBlerForAckTB2 (tickType_t currentGlobalTick,
/* SPR 15909 fix end */
        ULUEContext     *ueULContext_p,
  /* CA_phase2_harq_code Start */  
        DLCQIInfo        *dlCQIInfo_p,
        /* +- SPR 17777 */
        UInt16            ueIndex,
  /* CA_phase2_harq_code End */  
        InternalCellIndex internalCellIndex,
        /* CA_phase2_csi_code Start */
        UInt8 ueServCellIndex,
        UInt8 *bitMaskForAperiodicReport_p
        /* CA_phase2_csi_code End */
        );

/* SPR 15909 fix start */
 void calculateDLBlerForNackTB1 (tickType_t currentGlobalTick,
/* SPR 15909 fix end */
        ULUEContext     *ueULContext_p,
  /* CA_phase2_harq_code Start */  
        DLCQIInfo        *dlCQIInfo_p, 
        UInt16            ueIndex,
  /* CA_phase2_harq_code End */  
        InternalCellIndex internalCellIndex,
        DLMIMOInfo       *dlMIMOInfo
        /* CA_phase2_csi_code Start */
        ,UInt8 ueServCellIndex,
        UInt8 *bitMaskForAperiodicReport_p
        /* CA_phase2_csi_code End */
        );

/* SPR 15909 fix start */
 void calculateDLBlerForNackTB2 (tickType_t currentGlobalTick,
/* SPR 15909 fix end */
        ULUEContext     *ueULContext_p,
        DLCQIInfo        *dlCQIInfo_p, 
        /* +- SPR 17777 */
        UInt16            ueIndex,
        InternalCellIndex internalCellIndex
        ,UInt8 ueServCellIndex,
        UInt8 *bitMaskForAperiodicReport_p
        );
/* SPR 13571 fix start */
/* SPR 15909 fix start */
 void calculateDLBlerForTB1 (tickType_t currentGlobalTick,
/* SPR 15909 fix end */
        ULUEContext     *ueULContext_p,
        DLCQIInfo        *dlCQIInfo,
        UInt16            ueIndex,
        InternalCellIndex internalCellIndex,
        DLMIMOInfo       *dlMIMOInfo
        ,UInt8 ueServCellIndex,
        UInt8 *bitMaskForAperiodicReport_p
        );
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
        );
/* SPR 13571 fix end */

/*Complexity Start*/
void updateBlerCW1(InternalCellIndex internalCellIndex,
        ULUEContext* ueULContext_p,
        SInt8 currentDLBLERForCW1,
        UInt8 ueServCellIndex,
        UInt16 ueIndex);

void updateBlerCW0(InternalCellIndex internalCellIndex,
        ULUEContext* ueULContext_p,
        SInt8 currentDLBLERForCW0,
        UInt8 ueServCellIndex,
        UInt16 ueIndex);

void updateBlerCW0_2(InternalCellIndex internalCellIndex,
        ULUEContext* ueULContext_p,
        SInt8 currentDLBLERForCW1_2,
        UInt8 ueServCellIndex,
        UInt16 ueIndex);

void updateBlerCW1_2(InternalCellIndex internalCellIndex,
        ULUEContext* ueULContext_p,
        SInt8 currentDLBLERForCW1_2,
        UInt8 ueServCellIndex,
        UInt16 ueIndex);

/*Complexity End*/

#define IS_PCELL_OF_UE(ueDLContext_p, servingCellIndex) \
    ((servingCellIndex == ueDLContext_p->internalCellIndex) ? LTE_TRUE : LTE_FALSE)

/*#warning Common Branch AUT Fix */
/** CA AUT FIX : Start **/    
#ifdef MAC_AUT_TEST
#define MAX_NUM_NODE 4
UInt8 macAutDlHarqFlagVer_g[MAX_NUM_CELL];
UInt8 macAutTddDlHarqFlagVer_g[MAX_NUM_NODE][MAX_NUM_CELL];
#endif
/** CA AUT FIX : End **/    
/* TDD SB_CQI */
#endif  
/* LTE_MAC_DL_HARQ_MGR_H */
