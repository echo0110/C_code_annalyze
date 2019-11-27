/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacEmtcDLQosScheduler.h 
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
 *
 ****************************************************************************/

#ifndef LTE_MAC_EMTC_DL_QOS_STRATEGY_H
#define LTE_MAC_EMTC_DL_QOS_STRATEGY_H
/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacMPDCCHMgr.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/// This files contains the data strcuture being used by for DL qosscheduling for eMTC
/// UEs

#define EMTC_TA_MAX_TX_ATTEMPT 2
#define EMTC_INVALID_HARQ 0xFF
#define EMTC_MAX_RBS_PER_NB 6
#define MAX_IBS_INDEX_PDSCH_CE_MODE_A 15
#define MAX_IBS_INDEX_PDSCH_CE_MODE_B 10
#define MAX_ABS_SUBFRAME_COUNT        10240 

#define EMTC_INVALID_NB_INDEX 255
#define NUM_OF_EMTC_TA_SCHEDULED_PER_TTI 1

/// Sliding Window length
/// Max(PDSCH Sliding Window length(258), PUSCH sliding Window Length (261), HARQ Sliding window length (2312))
#define EMTC_SLIDING_WINDOW_LENGTH 2312 

#define MAX_EMTC_UE_SCHEDULED_IN_DL_PER_TTI 1

/// Create an array of priority list per narrowband to optimize the processing since
/// we don't need to consider a list associated with that narrowband if no more resources
/// available in that narrowband. All the lists are arranged in decreasing order of priority
/// which means PRS List shall be processed first then Retransmission Failure List and so on.
#define EMTC_MIN_PRIORITY_LIST 0

/// eMTC PRS List
#define EMTC_DL_PRS_LIST EMTC_MIN_PRIORITY_LIST

/// eMTC Retransmission Failure List
#define EMTC_DL_RETX_FAILURE_LIST 1

/// eMTC Retransmission List
#define EMTC_DL_RETX_LIST 2

/// eMTC TA Queue
/// keep the single queue to process the all type of TA failures
/// including MPDCCH/PDSCH Failure, Pending TA
#define EMTC_DL_TA_LIST 3

/// eMTC DRX Queue
#define EMTC_DL_DRX_LIST 4

#define EMTC_DL_PUCCH_TPC_LIST 5

/// Max eMTC Priority List 

#define EMTC_MAX_LIST EMTC_DL_PUCCH_TPC_LIST 

/// MAX RBs available per UE
#define EMTC_MAX_RB_PER_ALLOCATION 6

/// Max TB Size for EMTC
/// 1000/8 bytes
#define EMTC_MAX_DL_TB_SIZE 125


#define EMTC_NUM_CONTROL_SET_INFO_PER_PRB_PAIR 4
#define EMTC_MAX_RB_PER_NB_BITMASK 0x3F
#define EMTC_MAX_RB_LIST_DEPTH     0x06
#define EMTC_MAX_RB_COUNT_PER_NB   0x06 


#define EMTC_MAX_CCE_BITSET_LENGTH 24
#define EMTC_INVALID_ECCE_INDEX    255

/// This utility function is used to insert a node in  perTickLcScheduledUeList List
typedef struct ScheduledEmtcUeT
{
   LTE_LIST_NODE anchorNode;
   UInt16        ueIndex;
}ScheduledEmtcUe;


/// This is a helper function used to get the normalize the SFN/SF by considering the
/// PHY delay.
/* SPR 23717 changes start */
inline void getNormalizedSfnSf(UInt32* sysFrameNum,
                        UInt32* subFrameNum,
                        UInt32  phyDelay,
			InternalCellIndex internalCellIndex);


/// This function shall process the all Priority queues available for eMTC
inline void processEmtcDLPriorityQueues(UInt8*            numDlUEsToBeScheduled,
                                 /* SPR 23488 fix + */
                                 SInt32 *numberOfAvailableRBs_p,
                                 /* SPR 23488 fix - */
                                 UInt32            dlDelay,
                                 tickType_t        currentGlobalTTITickCount,
                                 InternalCellIndex internalCellIndex);

/// Check whether UE specific search space available on current tick to schedule this UE
/// since MPDCCH can only be scheduled on specific ocassions as per the configuration of start SF and
/// can only be send on specific RBs based on the ResourceAssignment configuration
/// We have to check the following under function uessAvailable()
/// - This is a Valid MPDCCH Ocassion for this UE as per configuration And
/// - Number of required MPDCCH Resources are available in terms of RB. The possible
///   combinations are 2, 4 or 6 RB
MacRetType uessAvailable(DLUEContext*      ueDLContext_p,
                         UInt8             narrowbandIndex,
                         UInt16*           startEcceIndex,
                         UInt16*           endEcceIndex,
                         UInt8*            startPrbIndex,
                         UInt8*            endPrbIndex,
                         tickType_t        currentGlobalTTITickCount,
/*+ SPR 22819*/
                         UInt8             dlDelay,
/*- SPR 22819*/
                         InternalCellIndex internalCellIndex);


/// This utility function is used to compute the start and end index of PRB Pair
/// The PRB pair set shall be the continuous set of PRBs
UInt8 getPrbPairStartIndex(UInt8 prbPairBitset, UInt8 numPrbPairs);

/// This utility function is used to check if ECCEs are available for this ue
/// with the given PRB Pair set.
MacRetType
checkIfControlInfoResAvailable(DLUEContext*      ueDLContext_p,
                               UInt8             narrowbandIndex,
                               UInt16*           ecceStartIndex,
                               UInt16*           ecceEndIndex,
                               UInt8*            prbPairStartIndex,
                               UInt8*            prbPairEndIndex,
                               tickType_t        currentGlobalTTITickCount,
                               InternalCellIndex internalCellIndex);

/// This is a utility function to check whether current tick
/// lies within the planned schedulable tick ranges
MacRetType isValidNode(tickType_t currentGlobalTTITickCount,
            tickType_t startTick,
            tickType_t endTick);



/// This function is used to detect the collission
MacRetType
checkForCollission(tickType_t  scheduledStartTick,
                   tickType_t  scheduledEndTick,
                   tickType_t  requestedStartTick,
                   tickType_t  requestedEndTick);


/// This is a utility function to check the UL/DL collission to implement
/// the HD-FDD functionality.
/// A Return Value MAC_FAILURE shall be considered as collission occured
MacRetType  
checkForHdFddCollissionsForDlTx(UInt16            ueIndex,
                                UInt16            startEcceIndex,
                                UInt16            endEcceIndex,
                                tickType_t*       endMpdcchTti,
                                UInt32*           numMpdcchRepetition,
                                tickType_t*       pdschStartTick,
                                tickType_t*       pdschEndTick,
                                UInt32*           numEmtcPdschRep,
                                tickType_t*       harqStartTti,
                                tickType_t*       harqEndTti,
                                UInt16*           harqAbsoluteSubframe,
                                tickType_t        currentTick,
                                InternalCellIndex internalCellIndex);


/// This function is used to process the DL priority Queues
inline void dlQosProcessEmtcPriorityQueue(LTE_CIRCQUEUE*    dlPriorityQueue_p,
                                   /* SPR 23488 fix + */
                                   SInt32 *numberOfAvailableRBs_p,
                                   /* SPR 23488 fix - */
                                   UInt8*            numDlUEsToBeScheduled_p,
                                   UInt32            sfn,
                                   UInt32            sf,
                                   UInt32            dlDelay,
                                   tickType_t        currentGlobalTTITickCount,
                                   InternalCellIndex internalCellIndex);


/// This utility function is used to get the valid eMTC DL tick for PDSCH transmission
/// from the current subframe;
tickType_t getNormalizedTickBasedOnValidDlSubframes(tickType_t currentGlobalTTITickCount);

/* SPR 23614 fix starts */
/// Get the RB's in future for PDSCH transmission
UInt8
getAvailableDLRbsAtPlusX(UInt8             rbsRequired,
                       UInt8             narrowbandIndex,
                       UInt8             rbBitmask,
                       UInt8*            startPdschRbIndex,
                       UInt8*            endPdschRbIndex,
                       UInt8             isStrictAllocation,
                       tickType_t        currentGlobalTTITickCount,
                       InternalCellIndex internalCellIndex);


/// Get the RB's in future for PUSCH transmission
/* SPR 23614 fix end */
UInt8
getAvailableRbsAtPlusX(UInt8             rbsRequired,
                       UInt8             narrowbandIndex,
                       UInt8             rbBitmask,
                       UInt8*            startPdschRbIndex,
                       UInt8*            endPdschRbIndex,
                       UInt8             isStrictAllocation,
                       tickType_t        currentGlobalTTITickCount,
                       InternalCellIndex internalCellIndex);


/// This function is used to get the maximum resource available at +2 index
/// by considering the all repetitions those needs to be perform for this transmission
inline void
scanAvailableRbsAtPlus2(UInt8             nbIndex,
                        tickType_t        currentGlobalTTITickCount,
                        InternalCellIndex internalCellIndex);
/// This function is required to compute the number of bits pending for transmission
/// in all LCs for this UE
UInt32
computeDlDataPendingInAllLc(DLUEContextInfo* ueDLContextInfo_p,
        LP_DlEmtcSchLcNode *emtcSchLcNode_p );

/// This function is used to initialize the EMTC DL QOS parameter
/// This function shall only be called once during initialization
/* SPR 23035 Fix + */
 inline void initEmtcDlQosParams(UInt8 internalCellIndex);
/* SPR 23035 Fix - */

/// Determine Max TB Size. We can't schedule more than 1000 bits per DLSCH PDU
/// for an eMTC UE
 UInt16 getEmtcDlMaxSchedulableTbSize(UInt8             mcsIndex,
                                      UInt16            dataSize,
                                      InternalCellIndex internalCellIndex);

/// This function returns the number of RB according to the transport block size and 
/// mcsIndex for eMTC UEs
UInt32 
determineRBsForEmtc(UInt16            mcsIndex,
                    UInt16*           dataSize_p,
                    UInt8             direction,
                    UInt8             ceMode);

/// This function check if the UE retransmission is possible in current tick.
 MacRetType isEmtcReTxFailNodeValidForRetx(DLRetxQueueNode* dlReTxFailNode_p,
                               UInt8*                  deleteNode,
                               tickType_t              globalTick,
                               InternalCellIndex       internalCellIndex);

/// This function is used to process the eMTC Retransmission queue
inline void processEmtcDlQosRetxQueue(LTE_SQUEUE*       dlEmtcRetxQueue_p,
                               /* SPR 23488 fix + */
                               SInt32 *numberOfAvailableRBs_p,
                               /* SPR 23488 fix - */
                               UInt8*            numDlUEsToBeScheduled_p,
                               UInt32            sfn,
                               UInt32            sf,
                               UInt32            dlDelay,
                               tickType_t        currentGlobalTTITickCount,
                               InternalCellIndex internalCellIndex);

/// This function is used to check the validity of DL Retx Node
 MacRetType isEmtcReTxNodeValid(DLRetxQueueNode*    dlReTxNode_p,
                                      UInt8*              deleteNode,
                                      tickType_t          globalTick,
                                      InternalCellIndex   internalCellIndex);

/// This function is used to process the TA queue for eMTC UEs
inline void processEmtcDlTAQueue(LTE_CIRCQUEUE*    emtcDlTAQueue,
                          /* SPR 23488 fix + */
                          SInt32 *numberOfAvailableRBs_p,
                          /* SPR 23488 fix - */
                          UInt8*            numDlUEsToBeScheduled_p,
                          UInt8*            numTAToSchedule_p,
                          UInt32            sfn,
                          UInt32            sf,
                          UInt32            dlDelay,
                          tickType_t        currentGlobalTTITickCount,
                          InternalCellIndex internalCellIndex);

/// Utility function to process the TA Node;
inline void processEmtcPendingTAQueue(DLUEContext*          ueDLContext_p,
                               TimeAdvanceQueueNode* taNode_p,
                               LTE_CIRCQUEUE*        emtcDlTAQueue,
                               InternalCellIndex     internalCellIndex);


/// This function is used to process the DRX queue for eMTC UEs
inline void processEmtcDrxQueue(LTE_CIRCQUEUE*    emtcDrxQueue,
                         /* SPR 23488 fix + */
                         SInt32 *numberOfAvailableRBs_p,
                         /* SPR 23488 fix - */
                         UInt8*            numDlUEsToBeScheduled_p,
                         UInt32            sfn,
                         UInt32            sf,
                         UInt32            dlDelay,
                         tickType_t        currentGlobalTTITickCount,
                         InternalCellIndex internalCellIndex);


/// Deep scan is required to calculate the number of eMTC UEs
/// are being scheduled in current tick
inline void deepScanMpdcchInfo(EMtcUlDlSchedulingInfo* schedulableNodeInfo_p,
                        UInt8*                   numDlUEsToBeScheduled_p,
                        tickType_t               currentGlobalTTITickCount,
                        UInt32                   dlDelay,
                        UInt8                    nbIndex,
                        InternalCellIndex        internalCellIndex);


/// This is a utility function to check whether current tick
/// lies within the planned schedulable tick ranges
 MacRetType isScheduledOnCurrentTick(tickType_t currentGlobalTTITickCount,
                         tickType_t startTick,
                         tickType_t endTick);


/// This function is used to scan the Narrowbands and to perform the following actions
/// - Prepare the DL strategy node for Control/Data for a UE whose repetitions planned in this tick
/// - Compute the number of resources remained in current tick
/// - Compute the number of UEs scheduled in current tick
/// This function shall be invoked after scheduling the MIB/SIB/SIs and common channel
/// control/data information. This function shall scan the all narrowbands to
/// schedule the UE specific control/data information if
/// - Resources are available in that narrowband And
/// - More UEs can be scheduled in this tick Configured threshold Max UE /TTI threshold is not hit
inline void scanDlNarrowbands(UInt8*            numDlUEsToBeScheduled_p,
                       /* SPR 23488 fix + */
                       SInt32 *numberOfAvailableRBs_p,
                       /* SPR 23488 fix - */
                       tickType_t        currentGlobalTTITickCount,
                       UInt32            dlDelay,
                       InternalCellIndex internalCellIndex);

/// This function is used to schedule and reserve resource for an eMTC UE
MacRetType
scheduleAndReserveRbsForDLTx(DLUEContext*      ueDLContext_p,
                             UInt8             isStrictAllocation,
                             UInt8             isRetx,
                             EmtcDLConTrolInfoType controlInfoType,
                             UInt8             requiredRbs,
                             UInt32            dataSize,
                             UInt8             harqProcessId,
                             UInt8*            numDlUEsToBeScheduled_p,
                             DLStrategyTxNode** dlStrategyTxNode_p,
                             UInt8             checkIfUeSchedulable,
                             tickType_t        currentGlobalTTITickCount,
/*+ SPR 22819*/
                             UInt8             dlDelay,
/*- SPR 22819*/
                             /* SPR 23488 fix + */
                             SInt32 *numberOfAvailableRBs_p,
                             /* SPR 23488 fix - */
                             InternalCellIndex internalCellIndex);

/// This utility function will check if specific RBs are available
/// to schedule a UE or not
 MacRetType checkIfRbAvailableWithDepthIndex(UInt8 startDepthIndex,
                                 UInt8 endDepthIndex,
                                 UInt8 resourceBitmask);

/// This utility function will mark the RBs reserve in per narrowband container
 UInt8 markRbsReservedforEmtcUE(UInt8  startDepthIndex,
                         UInt8  endDepthIndex,
                         UInt8* rbsNbBitmask_p);

/// This Utility function is used to mark the appropriate bits as busy
/// for ECCEs in a narrowband
/*+ SPR_22837 fix*/
inline void
markEccesReserved(UInt8  startCceIndex,
                  UInt8  endCceIndex,
                  UInt32* ecceBitset,
                  DLUEContext* ueDLContext_p);
/*- SPR_22837 fix*/

/// This Utility function is used to check the status of Ecces within the given range
MacRetType isEcceAvailableInPrbPair(UInt8             prbPairStartEcceIndex,
                                    UInt8             prbPairEndEcceIndex,
                                    UInt8             startEcceIndex,
                                    UInt8             aggregationLevel,
                                    UInt8             nbIndex,
                                    InternalCellIndex internalCellIndex);

/// This utility function is used to get and return the ecce startIndex when
/// the given number of ecces are available on a index within the given ecce
/// range
UInt8
getAvailableEcce(UInt8             startEcceIndexRange,
                 UInt8             endEcceIndexRange,
                 UInt8             numEccesRequired,
                 UInt8             nbIndex,
                 InternalCellIndex internalCellIndex);

/// This is a Utility Function to Check if RBs are available in a narrowband or not
 MacRetType isRbAvailableInNb(UInt8             nbIndex,
                  InternalCellIndex internalCellIndex);


/// This function shall get the schedulable Node in current tick. This function consider
/// the available rb bitmask to schedule
/// Additionally, this function shall remove any stale node that belongs to past
EMtcUlDlSchedulingInfo* getSchedulableNode(LTE_LIST*         emtcMpdcchPdschList,
                                           tickType_t        currentGlobalTTITickCount,
                                           tickType_t        nbScanTick,
                                           UInt8             nbIndex,
                                           UInt8             rbBitMapPerNb,
                                           UInt8*            currentRbDepthIndex,
                                           InternalCellIndex internalCellIndex);



/// This function shall create the DL Tx Strategy node by using the Harq
/// Information
DLStrategyTxNode* createEmtcDlStrategyNodeForRetx(DLUEContext*      ueDLContext_p,
                                                  DLHARQProcess*    harqProcess_p,
                                                  InternalCellIndex internalCellIndex);

/// This function create a DL Tx Strategy Node for New Transmission
DLStrategyTxNode*
createEmtcDlStrategyNodeForNewTx(DLUEContext*      ueDLContext_p,
                                 EmtcDLConTrolInfoType controlInfoType,
                                 tickType_t        currentGlobalTTITickCount,
                                 InternalCellIndex internalCellIndex);


/// This function shall check whether a UE is schedulable or not on current tick
MacRetType
isSchedulableEmtcUe(DLUEContext*         ueDLContext_p,
                    UInt8*               numDlUEsToBeScheduled_p,
                    DLStrategyTxNode*    dlStrategyTxNode_p,
                    tickType_t           currentGlobalTTITickCount,
/*+ SPR 22819*/
                    UInt8                dlDelay,
/*- SPR 22819*/
                    InternalCellIndex    internalCellIndex);

/// This function shall check whether a LC is schedulable or not on current tick
MacRetType
isSchedulableEmtcUeLc(DLUEContext*         ueDLContext_p,
        DLQosStrategyLCNode* dlQosStrategyLCNode_p,
        tickType_t           currentGlobalTTITickCount,
        InternalCellIndex    internalCellIndex,
        UInt8 gbrSchedulingFlag );

/// This function is used to schedule the eMTC Ues for scheduled LC List
inline void
processScheduledDlEmtcUeList(UInt8             nbIndex,
                           tickType_t        currentGlobalTTITickCount,
                           InternalCellIndex internalCellIndex);



/// This function shall schedule the LC of an eMTC UE
MacRetType scheduleEmtcUeLcForNewDlTx( DLUEContext* ueDLContext_p,
        UInt8*                numDlUEsToBeScheduled_p,
        DLQosStrategyLCNode*  dlQosStrategyLCNode_p,
        UInt8*                gbrSchedulingFlag_p,
        UInt8*                reStartTraversing_p,
        UInt32                tempDlAggregateGbrPerTTI[MAX_NUM_CELL][MAX_VALID_UE_TYPE],
        tickType_t            currentGlobalTTITickCount,
        /*+ SPR 22819*/
        UInt8                 dlDelay,  
        /*- SPR 22819*/
        InternalCellIndex     internalCellIndex,
        UInt16 scheduledGbrBytes[MAX_NUM_CELL] );

/* Compute the Data Size that can be transmitted for a LC considering the 
 * max TB size for that UE. */
UInt32 computeDlTxDataSizeForLc( DLUEContext* ueDLContext_p,
        DLQosStrategyLCNode*  dlQosStrategyLCNode_p,
        UInt16 scheduledGbrBytes[MAX_NUM_CELL],
        UInt32 tempDlAggregateGbrPerTTI[MAX_NUM_CELL][MAX_VALID_UE_TYPE], 
        UInt8* gbrSchedulingFlag_p,
        UInt8* reStartTraversing_p, InternalCellIndex internalCellIndex );

/// This function shall initialize the data structure being used for eMTC DL scheduling
inline void initPerTickEmtcDlStrategy(tickType_t  currentGlobalTTITickCount, InternalCellIndex internalCellIndex);

/// This function is used to schedule the eMTC Ues for scheduled LC List
inline void processScheduledEmtcDlUeList(UInt8*            numDlUEsToBeScheduled_p,
                                  /* SPR 23488 fix + */
                                  SInt32 *numberOfAvailableRBs_p,
                                  /* SPR 23488 fix - */
        tickType_t        currentGlobalTTITickCount,
        /*+ SPR 22819*/
        UInt8             dlDelay,
        /*- SPR 22819*/
        InternalCellIndex internalCellIndex);

/*****************************************************************************
 * Function Name  : initEmtcDlQosParams 
 * Inputs         : internalCellIndex - cell index used at MAC.
 * Outputs        : None
 * Returns        : None
 * Description    : This function initialise the parameter required by the EMTC 
 *                  DL scheduler 
 *****************************************************************************/
/* SPR 23035 Fix + */
inline void initEmtcDlQosParams(UInt8 internalCellIndex);
/* SPR 23035 Fix - */

/// This utility function is used to compute the absolute subframe and tti when HARQ is expected on a subframe 
/// in UL direction.
inline void
macCalculateHarqAbsTTI(tickType_t   endPdschTTI,
                       UInt16*      harqAbsoluteSf,
                       tickType_t*  harqStartTti,
                       tickType_t*  harqEndTti,
                       DLUEContext* ueDLContext_p);

/// This utility function is used to clean up the DL strategy node
inline void cleanupDlStrategyNode(DLUEContext*         ueDLContext_p,
                                  DLStrategyTxNode*    dlStrategyTxNode_p,
                                  UInt8                shouldCleanHarqProcess,
                                  InternalCellIndex    internalCellIndex);

/// This function is used to get the MPDCCH NarrowBand index of an eMTC UE
inline UInt8 getMpdcchNbIndex(DLUEContext* ueDLContext_p);

/// This Utility Function is used to unmark the appropriate bits as free
/// for ECCEs in a narrowband
inline void
markEccesUnreserved(UInt8   startEcceIndex,
                    UInt8   endEcceIndex,
                    UInt32* ecceBitset);

/// This function is used to initialize the per tick data
inline void
initEmtcDlQosPerTickParams(InternalCellIndex internalCellIndex);

/* SPR 22877 fix start */
UInt8
markSingleRbReservedforEmtcUE(UInt8  startDepthIndex,
        UInt8  endDepthIndex,
        UInt8* rbsNbBitmask_p);
/* SPR 22877 fix end */


#endif
