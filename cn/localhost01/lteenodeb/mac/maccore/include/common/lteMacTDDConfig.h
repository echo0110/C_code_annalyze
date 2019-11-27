/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2010 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: 
 *
 ****************************************************************************
 *
 *  File Description : This file defines the functionality for calculating
 *                    the Transport block size for the given MCS Index and
 *                    number of Resource Block.
 *
 ****************************************************************************
 ****************************************************************************/

#ifndef LTE_MAC_TDD_H
#define LTE_MAC_TDD_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacULUEContext.h"
#include "lteMacDLUEContext.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
typedef void (*tddPutEntryInAckNackQueueForDiffConfig)
    (
     UInt32 ueIndex,
     UInt32 operation,
    /* SPR 15909 fix start */
     tickType_t recvTTI,
    /* SPR 15909 fix end */
     UInt16 sfn,
     UInt16 sf
     ,InternalCellIndex internalCellIndex
     ,UInt8 modeType
    );
/** CA-TDD Changes Start:06 **/
extern tddPutEntryInAckNackQueueForDiffConfig tddPutEntryInAckNackQueueForCurConfig_p[MAX_NUM_CELL];
/** CA-TDD Changes End:06 **/

/****************************************************************************
 * Exported Types
 ****************************************************************************/
#define TOTAL_NUM_UL_DL_CONFIG 7
#define TDD_CONFIGURATION "TDD CONFIGURATION"
#define INVALID_HARQ 255
#define BUSY_ALL_HARQ 255
#define MAX_UL_DL_CONFIG 7
#define MAX_SPECIAL_SF_CONFIG 9
#define UL_HARQ_EXPIRY_TIME 3
#define  UL_HARQ_TIMER_NODE_INVALID 0
#define  UL_HARQ_TIMER_NODE_VALID 1
#define  UL_INVALID_COUNT 255

/* TDD Config 0, 6 Changes Start */
#define MAX_HARQ_MAP_CONFIG6_SIZE 60
#define MAX_HARQ_MAP_CONFIG0_SIZE 70
#define GET_ULGRANT_SFN_FOR_DOWNLINK_DATA_ONLY     1
#define GET_ULGRANT_SFN_FOR_DWNLINK_DATA_AND_DCI0  0
/* TDD Config 0, 6 Changes End */
/* TDD Config 0 Changes Start */
#define ULTX_OFFSET 7 /* Indicates ULTx in n+7 */
/*It signifies Init is done but Advance PUCCH and PRACH is not done*/
#define TDD_ONLY_INIT_DONE 2
/* TDD Config 0 Changes End */


#ifdef TDD_CONFIG
/* Cyclomatic_complexity_changes_start */
extern void setSubFrameTypeForDlHarq(UInt8           isSplSubFrame,
                              DLHARQProcess   *harqProcess_p);

/* + SPS_TDD_Changes */
#define GET_UL_HARQ_ID_FOR_TDD(subframe, ulHarqProcessId_p, delayToPhy, internalCellIndex)\
{\
	ulHarqProcessId_p = getULHarqId(subframe, delayToPhy, internalCellIndex);\
	if (BUSY_ALL_HARQ == ulHarqProcessId_p)\
	{\
		ltePanic("TDD - BUSY_ALL_HARQ\n");\
	}\
}
/* - SPS_TDD_Changes */
#endif
/* Cyclomatic_complexity_changes_end */


/* TDD Config 6 Changes Start */
#define GET_UL_SYS_FRAME_NUM_FROM_PHICH(prevUlSubFrameNum, subFrameNum, prevUlSysFrameNum, sysFrameNum) \
{ \
    if (prevUlSubFrameNum > subFrameNum) \
    { \
        /* UL Subframe in previous system Frame */ \
        if (sysFrameNum == 0) \
        { \
            prevUlSysFrameNum = MAX_SYS_FRAME_NUM; \
        } \
        else \
        { \
        prevUlSysFrameNum = sysFrameNum - 1; \
        } \
    } \
    else \
    { \
        prevUlSysFrameNum = sysFrameNum; \
    } \
}
/* TDD Config 6 Changes End */

typedef enum ULDLSPFrameT
{
    UL = 0, /* Uplink Subframe */
    DL,     /* Downlink Subframe */
    SP,     /* Special Subframe */
    NA = 255 /* Not Applicable*/
}ULDLSPFrame;

/*
MAC TDD Configuration 0 - 6
*/
enum
{

  UL_DL_CONFIG_0 = 0,
  UL_DL_CONFIG_1,
  UL_DL_CONFIG_2,
  UL_DL_CONFIG_3,
  UL_DL_CONFIG_4,
  UL_DL_CONFIG_5,
  UL_DL_CONFIG_6,
  UL_DL_CONFIG_MAX
};

/*
This union is used to get the valid
UL & DL subframe for UL Grant and 
HI pdu.
*/
typedef union UlGrantHIPduInfoT
{
    UInt8 ulGrantSubFrameNum;
    UInt8 ulPHICHSubFrameNum;
}UlGrantPhichInfo;

/*
This union is used to get the
rbMapNum for reserved in advance in Non Adaptive
case or get the value for already Reserve the 
RB Map.
*/
typedef union RBMapNumT
{
    UInt8 rbMapNumForNonAdaptive;
    UInt8 alreadyReserveRBNum;
}RBMapNum;

/*
This structure is used in future
its only store the previous 
DL Subframe in which DCI-0 is valid
and store the subframe number,
advanced reserved rb map number.
*/
typedef struct PrevRecordForULT
{
   UInt8 prevSubFrameforDCI_O_From_currentSubFrame;
   UInt8 exactPrevSubFrameNum;
   UInt8 advanceReservedRBMapNumFromPrevSubFrame;
}PrevRecordForUL;

/*
  This is the main Map or (hash table)
  to get the value of valid DL or UL subframe
  or getting the value of valid UL or DL subframe
  corresponding to the UL/DL Configuration.
  This sturcture is also store the resource block
  map number and advance reserve rb map number.
*/
typedef struct UlGrantHIPduSubFrameInfoT 
{
    ULDLSPFrame ulDlSpFrame;
    UlGrantPhichInfo ulGrantPhichInfo;
    RBMapNum         rbMapNum;
    PrevRecordForUL  prevRecordForUL;
}UlGrantHIPduSubFrameInfo;

typedef struct DLAckNackSubFrameNumInfoT
{
   UInt8 dlAckNackSubFrameNumber;

}DLAckNackSubFrameNumInfo;

/* Maximum Harq corresponding to UL/DL configuration*/
typedef struct MaxHarqProcessNumULDLT
{
    UInt8 maxDLHarqProcess;
    UInt8 maxULHarqProcess;
}MaxHarqProcessNumULDL;


/* Max number of container for crc */
typedef struct MaxContainerForCRCHandlingT
{
    UInt8 maxCrcContainer;
    UInt8 maxULResouceAllocatorMap;
}MaxContainerForCRCHandling;

/* Uplink Harq Timer Node */
typedef struct ULHarqTimerNodeT
{
    UInt8   isValid;
    UInt8   harqProcessId;
    /* + SPS_TDD_Changes */
    UInt8 schUsed;
    /* - SPS_TDD_Changes */
    UInt16 ueIndex;
}ULHarqTimerNode;

typedef struct ULHarqTimerEntityListT
{
    /* The count will contain the number of valid field in the array*/
    UInt16 count;
    /* This array will store the harqTimerNode */
    /* SPR 9381 changes start */
    /*CSR 40799 chg start*/
    //ULHarqTimerNode ulHarqTimerNodeArr[MAX_UE_SUPPORTED];
    ULHarqTimerNode ulHarqTimerNodeArr[MAX_UL_UE_SCHEDULED];
    /*CSR 40799 chg end*/
    /* SPR 9381 changes end */
}ULHarqTimerEntityList;

typedef struct SfForRachULConfigT
{
    UInt8 ulGrantSfForMsg3;
    UInt8 containerIndex;
}SfForRachULConfig;

typedef struct TddConfigInfoT
{
    UInt8 noOfDlSchedulable;
    UInt8 noOfUlSchedulable;
}TddConfigInfo;

typedef struct TddSfToTimerMapT
{ 
    UInt8    timerValue[10];  /* num of PDCCH SFs from a particular SF (<10) */
}TddSfToTimerMap;

typedef struct TddTimerConvMapT
{
    UInt8                  numPDCCHSF;  /* Num PDCCH SFs in a System Frame of 
                                           a particular TDD config */ 
    TddSfToTimerMap     tddSFToTimerMap[10];    
}TddTimerConvMap; 
    
typedef struct TddTimerConvMapInfoT
{
    TddTimerConvMap    tddDlTimerConvMap;
    TddTimerConvMap    tddUlTimerConvMap;
}TddTimerConvMapInfo;

/*
This macro is used in lteMacULRoundRobinScheduler.c
to check the harq process id valid for this subframe number
if yes then get the first free harq Process Id.

*/
/*  TDD REL 3.1.1 */
/*  TDD REL 3.1.1 */
/*define the assert for debugging the code */
#define MAC_ASSERT(x) 

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* TDD Config 6 Changes Start */
/*
** This function is used to get UL subframe for which PHICH is being 
** processed in subframeNum.
*/
extern UInt8 getUlSfOnPhichProcessing(UInt8 subframeNum ,InternalCellIndex internalCellIndex);
/* TDD Config 6 Changes End */

/* this array will store the harqs of UE on per subframe basis.*/

/* Intialize the structure correspondence to the ul dl configuration*/
/* SPR 5599 changes start (ZIP ID 129100) */
/** CA-TDD Changes Start:05 **/
void initMacTDDConfig(InternalCellIndex internalCellIndex);
/** CA-TDD Changes End:05 **/
/* SPR 5599 changes end (ZIP ID 129100) */


/* SPR 5333 changes start */
extern UInt8 miValueForSubframe(
                UInt8 subframe,
                InternalCellIndex internalCellIndex);
/* SPR 5333 changes end */
/* SPR 5599 changes end (ZIP ID 129100) */


/* This function is used in PDCCH Module to set cce indedx in UL UE Context
   (dlCCEIndex[MAX_SUB_FRAME])*/
/* SPR 5599 changes start (ZIP ID 129100) */
MacRetType setCCEIndexInULContex( UInt16 ueIndex, 
        /* SPR 5599 changes end (ZIP ID 129100) */
        UInt8 subframe, 
        UInt8 cceIndex,
        /** SPR 14204 Fix : Start **/
        InternalCellIndex internalCellIndex
        /** SPR 14204 Fix : End **/
        /* SPR 19679: TDD HARQ Multiplexing  Changes Start */
        ,UInt8 isTBOneValid 
        ,UInt8 isTBTwoValid 
        /* SPR 19679: TDD HARQ Multiplexing  Changes End */
        );

/*This function is used to get the number of uplink container used in
  current UL/DL Configuration.
*/                                              
/* SPR 5599 changes start (ZIP ID 129100) */
UInt8 getMaxULCrcContainer(InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129100) */

/* 
This function is used to get how many subframe after next UL subframe
*/
/* SPR 5599 changes start (ZIP ID 129100) */
/** CA-TDD Changes Start:05 **/
UInt8 getULGrantSubframeNum (UInt8 subframeNum, UInt8 onlyDownlinkUeSchFlag,
                             InternalCellIndex internalCellIndex);
/** CA-TDD Changes End:05 **/
/* SPR 5599 changes end (ZIP ID 129100) */

/*
This function is used to get the next Valid DL Subframe for Send HI pdu.
*/
/* SPR 5599 changes start (ZIP ID 129100) */
/* +GCC 4.7 */
 UInt8 getHiPduSubframeNum( 
            UInt8 subframeNum,
            InternalCellIndex internalCellIndex);
			
/* BUG:10430:start */
/* BUG:10430:end */
/* + SPS_TDD_CHANGES */
 UInt8 getULHarqId( UInt8 subFrame,
        UInt32 delayToPhy,
        InternalCellIndex internalCellIndex);
/* - SPS_TDD_CHANGES */

/* -GCC 4.7 */
/* SPR 5599 changes end (ZIP ID 129100) */

/*
This function is used to get the Maximum dl harq supported corresponding to
the UL/DL Configuration.
*/
/* SPR 5599 changes start (ZIP ID 129100) */
UInt8 getMaxDLHarq(InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129100) */

/*
This function is used to get the maximum uplink resources map corresponding to 
the UL/DL configuration.
*/
/* SPR 5599 changes start (ZIP ID 129100) */
UInt8 getMaxULRBMaps(InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129100) */

/* 
This function is used to get the RB map number corresponding to the 
next valid UL subframe number.

*/
/* SPR 5599 changes start (ZIP ID 129100) */
UInt8 getNextRBMapNumForNonAdaptive( 
            UInt8 subframeNum,
            InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129100) */

/*
This function is used to get the number of advance Reserve RB Map.
*/

/* SPR 5599 changes start (ZIP ID 129100) */
UInt8 getAdvanceReserveRBMap( 
        UInt32 recvdULDataSubframe,
        InternalCellIndex internalCellIndex
        );
/* SPR 5599 changes end (ZIP ID 129100) */

/*
This function is used to get the first free UL harq Process Index.
*/
/*  TDD REL 3.1.1 */
/* SPR 5599 changes start (ZIP ID 129100) */
/* +GCC 4.7 */
/* TDD Config 0, 6 Changes Start */
 UInt8 getFreeULHarqId(
        ULUEContext *ulUEContext_p, 
        UInt8 subFrame,
        InternalCellIndex internalCellIndex,
        UInt8 ulSubFrame,
        /* SPR 11257 Changes Start*/
        /* +- SPR 17777 */
        /* SPR 15909 fix start */
        tickType_t globalTick
        /* SPR 15909 fix end */
        );
        /* SPR 11257 Changes End*/
/* TDD Config 0, 6 Changes End */
/* -GCC 4.7 */
/* SPR 5599 changes end (ZIP ID 129100) */
/*  TDD REL 3.1.1 */
/*
This function is used to set the harq busy correspondence to the
subframe number.
*/

/* SPR 5599 changes start (ZIP ID 129100) */
UInt8 setHarqBusy( 
        UInt8 subframe, 
        /* SPR 5599 changes end (ZIP ID 129100) */
        UInt16 harqId, 
        ULUEContext *ulUEContext_p,
        InternalCellIndex internalCellIndex,
        /* TDD Config 6 Changes Start */
        UInt16 sysFrameNum
        );
/* TDD Config 6 Changes End */

/*
This function is used to reset the harq Id.
*/
/* SPR 5599 changes start (ZIP ID 129100) */
void resetHarqId( 
        UInt8 subframe,
        /* SPR 5599 changes end (ZIP ID 129100) */
        ULUEContext *ulUEContext_p,
        InternalCellIndex internalCellIndex,
        /* TDD Config 6 Changes Start */
        UInt16 sysFrameNum
        /* TDD Config 6 Changes End */
        );

/*
This function is used to get the UL Harq Process Id corresponding to
the subframe number.
*/

/* SPR 5599 changes start (ZIP ID 129100) */
UInt8 getULHarqIDFromSubFrame ( 
        UInt8 subframeNum,
        /* SPR 5599 changes end (ZIP ID 129100) */
        ULUEContext *ulUEContext_p,
        InternalCellIndex internalCellIndex,
        /* TDD Config 6 Changes Start */
        UInt16 sysFrameNum
        /* TDD Config 6 Changes End */
        );
/* msg3_retx_tdd_support_start */
 UInt8 getULHarqIDForMsg3Tdd (
/* TDD Config 0 Chnages Strat */
    /* SPR 11257 Changes Start*/
    UInt8 subFrameNum,
    UInt8 ulSubFrameNum,
    /* SPR 15909 fix start */
    tickType_t globalTick,
    /* SPR 15909 fix end */
    InternalCellIndex internalCellIndex);
    /* SPR 11257 Changes End*/
/* TDD Config 0 Changes End */

/* msg3_retx_tdd_support_end */
/*
This function is used to check the current subframe is valid 
UL subframe if yes then return the valid subframe for HI pdu.
*/

/* SPR 5599 changes start (ZIP ID 129100) */
MacRetType isValidULSubFrameForHIPdu (
        UInt8 subframeNum,
        /* SPR 5599 changes end (ZIP ID 129100) */
        UInt8 *hiPduSubFrameNum,
        InternalCellIndex internalCellIndex
                                            );

/*
This function is used to check the valid DL Subframe number for sending
DCI -0
*/                                            
/* SPR 5599 changes start (ZIP ID 129100) */
MacRetType isValidSubframeNumForDCI_0 (
        UInt8 subFrameNum,
        InternalCellIndex internalCellIndex
        );

UInt8 getExactULSubFrame( 
            UInt8 subframeNum,
            InternalCellIndex internalCellIndex
            );
MacRetType  deleteEntryFromTimerList(
/* SPR 5599 changes end (ZIP ID 129100) */
                                 DLUEContext *dlUEContext_p,
                                 UInt8 containerIndex,
                                 UInt8 currentULSubFrame,
                                 InternalCellIndex internalCellIndex
                               );

/* SPR 5599 changes start (ZIP ID 129100) */
void getContainerIdxForDlTimer ( UInt16 sysFrameNumber,
/* SPR 5599 changes end (ZIP ID 129100) */
                                        UInt8  subFrameNumber,
                                        UInt8  sfnFactor,
                                        UInt8 *ackNackContainerIdx,
                                        UInt8 *hpIdContainerIdx
                                        );

/* SPR 5599 changes start (ZIP ID 129100) */
void getContainerIdxForTimerExpiry( UInt8 dlAckNackSubFrame,
/* SPR 5599 changes end (ZIP ID 129100) */
                                           UInt8 sfn,
                                           UInt8 *containerIndex
                                         );

/*
 * Function pointer table for the function to add an entry in the 
 * Ack/Nack queue for the received ack nack
 * for different UL-DL configuration
 */

extern tddPutEntryInAckNackQueueForDiffConfig 
         tddPutEntryInAckNackQueueForDiffConfig_g[TOTAL_NUM_UL_DL_CONFIG];


/* This function is used by CRC ACk/Nack handling to stop
 * the UL Harq Process timer and update the Delta BSR
 * and return the harq process Id.
 */
/* SPR 5599 changes start (ZIP ID 129100) */
                                /* SPR 15909 fix start */
UInt8 resetHarqAndUpdateDeltaBsr( tickType_t recvTTI,
                                /* SPR 15909 fix end */
/* SPR 5599 changes end (ZIP ID 129100) */
                                        ULUEContext *ulUEContext_p,
                                        /* CA Changes Start */
                                        InternalCellIndex internalCellIndex
                                        /* CA Changes Start */
                                       );
/* SPR 5599 changes start (ZIP ID 129100) */
UInt8  checkForSpecialSubframe ( 
        UInt8 subframe,
        InternalCellIndex internalCellIndex
        );
/* SPR 5599 changes end (ZIP ID 129100) */

/* This function retrive the Subframe Type from structure
 * ulGrantPhichTableInfo_g for current UL-DL config and
 * input subframe number and return the same
 */ 
/* SPR 5599 changes start (ZIP ID 129100) */
/* + Coverity 96932 */
 ULDLSPFrame getSubframeType( UInt8 subframe, 
        /* - Coverity 96932 */
        /* CA Changes Start */
        InternalCellIndex internalCellIndex
        /* CA Changes End */
        );

/* EICIC - TDD +*/
 UInt16 getUplinkToGrantSubframeNumber( UInt8 subframe, 
        InternalCellIndex internalCellIndex
        );
/* EICIC - TDD -*/

void initTddConfigInfo(UInt8 subFrameAssign, 
/* SPR 5599 changes end (ZIP ID 129100) */
        UInt8 splSubFramePatterns,
        InternalCellIndex internalCellIndex );

/* This function will check that it is valid subframe to send DCI-0 */
/* SPR 5599 changes start (ZIP ID 129100) */
MacRetType isValidSfForDCI_0 ( UInt8 subframeNum, InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129100) */

/* msg3 ulconfig fix start */

/* This function will return the index and sub frame number where DCI 0 for the
 * Msg3 corresponding to the RAR sent in the subframeNum is to be sent */
/* SPR 5599 changes start (ZIP ID 129100) */
UInt8 getSfForRachULConfig (UInt8 subframeNum,
/* SPR 5599 changes end (ZIP ID 129100) */
                           UInt8 *index,
                           InternalCellIndex internalCellIndex);
/* msg3 ulconfig fix end */

/*It calculates the SFN Factor based on the UL sub frame on 
 * which DL Ack NAck is to be recieved. */
/* SPR 5599 changes start (ZIP ID 129100) */
UInt8 getDlAckNackSfnFactor(UInt8 subframeNum, InternalCellIndex internalCellIndex);

UInt8 getUlSfnFactor(UInt8 subframeNum
        /* +- SPR 17777 */
		    /*TDD Config 0 Changes Start*/
		    ,UInt8 ulSubFrame
            );
		    /*TDD Config 0 Changes End*/

MacRetType ulHarqTimerStart( UInt8 subframe, 
/* SPR 5599 changes end (ZIP ID 129100) */
                                    UInt16 sysFrameNum,
                                    UInt16 ueIndex,
                                    UInt8 harqProcessId,
                                    /* + SPS_TDD_Changes */
                                    UInt8 schUsed,
                                    /* - SPS_TDD_Changes */
                                    /* CA Code Change */
                                    InternalCellIndex internalCellIndex,
                                    /* CA Code Change */
                                    /*TDD Config 0 Changes Start*/
                                    UInt8 ulSubFrame
                                    /*TDD Config 0 Changes End*/
                                    );

                                    
/* msg3 ulconfig fix start */
/* This function UL SF where Msg3 is to be recieved. */
/* SPR 5599 changes start (ZIP ID 129100) */
UInt8 getULSfForMsg3 (UInt8 subframeNum, InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129100) */

/* This function  gets the tick where PUCCH is called for Msg3 */
/* SPR 5599 changes start (ZIP ID 129100) */
UInt8 getTickForPUCCHCalForMsg3 (UInt8 subframeNum,
                                InternalCellIndex internalCellIndex);  
/* SPR 5599 changes end (ZIP ID 129100) */
/* msg3 ulconfig fix end */

/* This function shall convert the expiry tick such that the expiry tick 
   shall be a UL grant sub frame in TDD mode. */
/* SPR 5599 changes start (ZIP ID 129100) */
tickType_t tddConvertTimerValForUlGrant (
/* SPR 5599 changes end (ZIP ID 129100) */
        UInt16 timerVal,
        /* SPR 15909 fix start */
        tickType_t globalTTI,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex);

/* SPR 5599 changes start (ZIP ID 129100) */
tickType_t tddConvertTimerValForDlGrant(
/* SPR 5599 changes end (ZIP ID 129100) */
        UInt16  timerVal,
        /* SPR 15909 fix start */
        tickType_t globalTTI,
        /* SPR 15909 fix end */
        InternalCellIndex internalCellIndex );
/*Start Changes for TDD DL UL SPLIT*/
#ifdef DL_UL_SPLIT_TDD
void* executionTDDFlowThread(void* executionFlowThreadArgs_p); 
#endif
/*End Changes for TDD DL UL SPLIT*/

/*TDD SB_CQI */
UInt8 getULSfForUpdatingBandwidthPart(UInt8 subframeNum, InternalCellIndex internalCellIndex);
/*TDD SB_CQI */
/* + SPS_TDD_Changes */
 SInt8 getSubFrameOffsetForSpsTDD (UInt8 subFrameNum,InternalCellIndex internalCellIndex);
/*****************************************************************************
 * Description    : This function retrive the Subframe Type from structure
 *                  ulGrantPhichTableInfo_g for current UL-DL config and
 *                  input subframe number and return the same
 ******************************************************************************/
 UInt8 getPrevSubFrameforDCI0( UInt8 ulSubframe,InternalCellIndex internalCellIndex);
/* - SPS_TDD_Changes */
/* TDD Config 0 Changes Start */
/* This function checks if the current subframe is TDD config 0
 * Special Subframe or not */
 UInt8 isTddConfig0SsubFrame(UInt8 ulSubFrameNum,InternalCellIndex internalCellIndex);
 UInt8 getnextUlSfOffsetFromCurrentUlSf(UInt8 ulSf,
                /* CA TDD CHANGES START */
                InternalCellIndex internalCellIndex
                /* CA TDD CHANGES END */
                );
 UInt8 roundTripTime(UInt8 ulSubframenum, 
                /* CA TDD CHANGES START */
                InternalCellIndex internalCellIndex
                /* CA TDD CHANGES END */
                );
 UInt8 getUlSubframe(UInt8 prevUlSfOffset, UInt8 hiSf,InternalCellIndex internalCellIndex);
UInt8 getprevUlSfOffsetFromCurrentUlSf(UInt8 ulSf,
                /* CA TDD CHANGES START */
                InternalCellIndex internalCellIndex
                /* CA TDD CHANGES END */
                );
 UInt8 getUlOffset(UInt8 hiSf,UInt8 ulSf);

typedef struct PuschTransmissionSfT
{
  UInt8 prevUlSfOffset;
  UInt8 nextUlSfOffset;
}PuschTransmissionSf;
/* SPR 11115 Fix Start */
/* This function gets number of DL or UL sfs in a second */
 UInt16 getNumTicksPerSecond(ULDLSPFrame sfDirection, InternalCellIndex internalCellIndex);
/* SPR 11115 Fix End */
/*TDD Config 0 Changes End*/
#ifdef LTE_EMBMS_SUPPORTED
UInt8 getNextDLOccasion(UInt8 sib13sf, InternalCellIndex internalCellIndex);
#endif

#endif
