/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacMultiplexer.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the data structures and the function
 *                     decleration for the multiplexer. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacMultiplexer.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.29  2009/11/13 14:44:40  gur18550
 * DL Harq being freed for Q Load = 0 case
 *
 * Revision 1.28  2009/11/12 19:55:29  gur18550
 * Function Def Updated
 *
 * Revision 1.27  2009/08/13 10:20:26  gur18550
 * Warnings Removed
 *
 * Revision 1.26  2009/08/12 06:59:40  gur18550
 * Log Updated
 *
 * Revision 1.25  2009/07/23 07:11:19  gur18569
 * included lteMacUtilityAndSharedStructs.h for logint
 *
 * Revision 1.24  2009/07/16 08:11:06  gur18550
 * Removed warning of macUeOpp
 *
 * Revision 1.23  2009/07/09 08:19:43  gur18550
 * function made 
 *
 * Revision 1.22  2009/07/09 05:49:28  gur18550
 * Added RLC_SN_FIELD_LENGTH_SMALL
 *
 * Revision 1.21  2009/07/07 08:54:17  gur18550
 * Comments Inc
 *
 * Revision 1.20  2009/07/06 07:36:06  gur18550
 * Comments Inc
 *
 * Revision 1.19  2009/07/01 06:14:27  gur18550
 * File Updated
 *
 * Revision 1.18  2009/07/01 05:03:02  gur18550
 * Function Updated
 *
 * Revision 1.17  2009/06/26 07:09:28  gur18550
 * Function Introduced
 *
 * Revision 1.16  2009/06/25 12:53:28  gur18550
 * File Updated
 *
 * Revision 1.15  2009/06/24 12:08:45  gur18550
 * Final review Comments Inc
 *
 * Revision 1.14  2009/06/18 12:18:16  gur18550
 * Pending Delete related Changes
 *
 * Revision 1.13  2009/06/18 10:04:06  gur18550
 * CCE Related Changes
 *
 * Revision 1.12  2009/06/18 07:34:46  gur12140
 * Added UeInex in MultiplexerQueueNode Structure
 *
 * Revision 1.11  2009/06/17 14:42:47  gur18550
 * Added Feature for RLC HEADER
 *
 * Revision 1.10  2009/06/16 14:15:51  gur18569
 * added and argument to processMultiplexerQueue , delay to phy
 *
 * Revision 1.9  2009/06/11 10:01:03  gur18550
 * Function added cleanUpMUXQueue
 *
 * Revision 1.8  2009/06/11 09:22:47  gur18550
 * Function added initMUX
 *
 * Revision 1.7  2009/06/11 05:09:39  gur18569
 * comment decl of macUeOpputurnityInd as it's declared in lteLayer2RlcMacInterface file
 *
 * Revision 1.6  2009/06/08 15:42:59  gur12140
 * Removed linkineg errors
 *
 * Revision 1.5  2009/06/08 09:37:05  gur18550
 * Comments Inc
 *
 * Revision 1.4  2009/06/01 09:48:56  gur18550
 * Warnings Removed
 *
 * Revision 1.3  2009/06/01 08:54:05  gur12140
 *
 * Revision 1.2  2009/06/01 07:56:04  gur18550
 * File updated
 *
 * Revision 1.1  2009/04/30 05:10:30  gur18550
 * INITIAL VERSION
 ****************************************************************************/

#ifndef LTE_MAC_MULTIPLEXER_H
#define LTE_MAC_MULTIPLEXER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteLayer2MacRlcInterface.h"
#include "lteMacDLUEContext.h"
#include "lteMacDLSchedulerStrategy.h"
#include "lteMacDLScellStateManager.h"
#include "lteMacExecutionLegs.h"
#include "lteMacUtilityAndSharedStructs.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

#define TWO_BYTES                           2
#define THREE_BYTES                         3
#define FIVE_BYTES                          5 
/* + SPR 22352 fix */
#define FOUR_BYTES                          4 
/* - SPR 22352 fix */
/* SPR 3108 changes start */
#define RLC_FIXED_HEADER_SIZE_ONE_BYTE      0
#define RLC_FIXED_HEADER_SIZE_TWO_BYTE      0
/* SPR 3108 changes start */
#define MAC_SDU_SIZE_CHECKPOINT           127
#define MAC_MIN_SDU_SIZE                    5
#define INVALID_LC_ID                     0xFF
#define RLC_SN_FIELD_LENGTH_SMALL           5 
#define LOG_MULTIPLEXER         "MULTIPLEXER"                   

/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef enum TransmissionStatusT
{
     TB_ONE_VALID_AND_TB_TWO_VALID = 0,
     TB_ONE_VALID_AND_TB_TWO_RETRANS,
     TB_ONE_RETRANS_AND_TB_TWO_VALID,
     TB_ONE_VALID_AND_TB_TWO_INVALID,
     TB_ONE_INVALID_AND_TB_TWO_VALID
}TransmissionStatus;


typedef  struct LCsBytesInfoArrT
{
     UInt8 logicalChannelID;
     /*  
         This field will denote how many bytes of the corresponding
         logical channel has been allocated in this TB
     */
     UInt16   allocatedBytes; 
}LCsBytesInfoArr;

typedef struct SortedLCsLoadInfoT
{
      UInt8    logicalChannelID;
      UInt32  lcQLoad;
      UInt32  sortKey;
}SortedLCsLoadInfo;


typedef struct ScheduledLCInfoT
{
      UInt8 numOfLc;
      DLHARQProcess* dlHarqProcess_p;
      DLUEContext *ueContext_p;
      DLLogicalChannelInfo* logicalChannelArr_p;
      SortedLCsLoadInfo* sortedArrayOfLC_p;
}ScheduledLCInfo;



typedef struct MultiplexerQueueNodeT
{
    /*data type of anchor*/
    LTE_SNODE muxNodeAnchor;
	UInt32 sumWeightedQueueLoad;
	UInt32 sumLCPriority;
	UInt16 ueIndex;
	UInt8 harqProcessId;
	MsgType msgType;
	UInt8 cceIndex;
 /* + SPR 11208 Changes */
    InternalCellIndex internalCellIndex;
 /* - SPR 11208 Changes */
#ifdef TDD_CONFIG
       UInt8 dlSubframeNum;
       UInt32 phyDelay;
       UInt32 sysFrame;
#endif
}MultiplexerQueueNode;





/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/


/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* SPR 5599 changes start (ZIP ID 129059) */
void putEntryInMultiplexerQueue(MultiplexerQueueNode  * muxQueueNode_p,
        InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129059) */


MacRetType processMultiplexerQueue(
                            /* SPR 15909 fix start */
                            tickType_t currentGlobalTick,    
                            /* SPR 15909 fix end */
                            UInt8 untilLastFlag,
							UInt32 delayToPhy,
                            InternalCellIndex internalCellIndex);

/* SPR 7435 Changes Starts */
/* SPR 5599 changes start (ZIP ID 129059) */
 /* + SPR 11209 Changes */
/* SPR 15909 fix start */
 void processDummyNode(tickType_t scheduledTick, InternalCellIndex encoderCellIndex, InternalCellIndex rlcCellIndex);
 /* SPR 15909 fix end */ 
 /* - SPR 11209 Changes */
/* SPR 5599 changes end (ZIP ID 129059) */
/* SPR 7435 Changes Ends */

/* SPR 4168 */
MacRetType multiplexLogicalChannelsForUE(
        MultiplexerQueueNode* muxQueueNode_p,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        DLUEContext* ueContext_p,
        InternalCellIndex internalCellIndex);

/* SPR 4168 */
/* QOS MUX Change Start */
/* + SPR 13605 Fix */
MacRetType multiplexScheduledLcinQoS(
        MultiplexerQueueNode* muxQueueNode_p,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        DLUEContext* ueContext_p,
        InternalCellIndex internalCellIndex);
/* - SPR 13605 Fix */
/* QOS MUX Change End */

/* PDCCH_PDSCH_WITHOUT_MUX Start */
MacRetType lteLayer2MacLogicalChannelsMuxing(
        MultiplexerQueueNode  * muxQueueNode_p,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        DLUEContext* ueContext_p,
        InternalCellIndex internalCellIndex);
/* PDCCH_PDSCH_WITHOUT_MUX End */

/* SPR 4168 */
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType distributeSingleTBAmongLCs(
/* SPR 5599 changes end (ZIP ID 129059) */
        DLUEContext *ueContext_p,
        UInt8 loopCount,   
        UInt32 availableTBSize,
        TransmissionAndRLCData* transmissionAndRLCData_p,
        DLLogicalChannelInfo* logicalChannelArr,  
        SortedLCsLoadInfo* sortedArrayOfLC,
        InternalCellIndex internalCellIndex);

/* SPR 4168 */
/* SPR 4168 */
MacRetType singleTBDistribution(
        MultiplexerQueueNode* muxQueueNode_p,
        TransmissionAndRLCData * transmissionAndRLCData_p,
        DLUEContext *ueContext_p ,
        DLHARQProcess* dlHarqProcess_p,
        UInt8 tbIdentifier,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex);
/* SPR 4168 */

/* SPR 4168 */
MacRetType dualTBDistribution(
        MultiplexerQueueNode* muxQueueNode_p,
        TransmissionAndRLCData * transmissionAndRLCData_p,
        DLHARQProcess* dlHarqProcess_p,
        DLUEContext *ueContext_p,
        InternalCellIndex internalCellIndex);

/* SPR 4168 */

UInt8  generateSortedListOfLCs( 
        DLUEContext *ueContext_p,
        UInt8 numOfLC,
        SortedLCsLoadInfo* sortedArrayOfLC,
        /* SPR 15909 fix start */
        tickType_t scheduledTick,
        /* SPR 15909 fix end */
        MultiplexerQueueNode* muxQueueNode_p,
        InternalCellIndex internalCellIndex);


/* SPR 5599 changes start (ZIP ID 129059) */
void swapFunc(
/* SPR 5599 changes end (ZIP ID 129059) */
        SortedLCsLoadInfo sortedArrayOfLC[],
        UInt32 dex1, 
        UInt32 dex2);


/* SPR 5599 changes start (ZIP ID 129059) */
UInt32 medianOfThree(
/* SPR 5599 changes end (ZIP ID 129059) */
        SortedLCsLoadInfo sortedArrayOfLC[],
        UInt32 left, 
        UInt32 right); 


/* SPR 5599 changes start (ZIP ID 129059) */
UInt32 makePartition(
/* SPR 5599 changes end (ZIP ID 129059) */
        SortedLCsLoadInfo sortedArrayOfLC[],
        UInt32 left, 
        UInt32 right, 
        UInt32 pivot);

void mySort(
        SortedLCsLoadInfo sortedArrayOfLC[],
        UInt32 left, 
        UInt32 right);

void quickSort(
        SortedLCsLoadInfo sortedArrayOfLC[],
        UInt32 left, 
        UInt32 right,
        UInt32 maxValuesToBeSorted); 

/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType invokeMuxLogic(
/* SPR 5599 changes end (ZIP ID 129059) */
                        MultiplexerQueueNode* muxQueueNode_p,
                        /* SPR 15909 fix start */
                        tickType_t scheduledTick,
                        /* SPR 15909 fix end */
                        InternalCellIndex internalCellIndex);
/* SPR 5599 changes start (ZIP ID 129059) */
void initMUX(UInt8 numOfCells);
/* SPR 5599 changes end (ZIP ID 129059) */

/* SPR 5599 changes start (ZIP ID 129059) */
void cleanUpMUXQueue(InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129059) */


/* SPR 4168 */
/* SPR 5599 changes start (ZIP ID 129059) */
MacRetType distributeTwoTBAmongLCs( 
/* SPR 5599 changes end (ZIP ID 129059) */
                    UInt32 availableTBOneSize,
                    UInt32 availableTBTwoSize,
                    TransmissionAndRLCData* transmissionAndRLCData_p,
                    ScheduledLCInfo* scheduledLCInfo_p,
                    InternalCellIndex internalCellIndex);

/* SPR 4168 */
/*ca chngs start*/
extern void putEntryInPendingFailureDlCAMacCEQueue( UInt16 ueIndex,
                                  DLHARQProcess* dlHarqProcess_p,
                                  InternalCellIndex internalCellIndex);
/* SPR 20254 Fix Start */
void handleMuxFailure(MultiplexerQueueNode *muxQueueNode_p,
            tickType_t scheduledTick,
            DLUEContext *dlUeContext_p,
            InternalCellIndex internalCellIndex);
/* SPR 20254 Fix End */
#endif  /* FILENAME_H */
